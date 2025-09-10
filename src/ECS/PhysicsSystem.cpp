#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/World.h"
#include <iostream>

namespace ECS {

PhysicsSystem::PhysicsSystem() : gravityEnabled(true), gravity(0.0f, -9.81f, 0.0f) {
    RequireComponents<Transform, RigidBody, Collider>();
    SetPriority(50);  // Run AFTER movement and player controller, but before render
    Initialize();
}

PhysicsSystem::~PhysicsSystem() {
    Cleanup();
}

void PhysicsSystem::Initialize() {
    collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    
    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher.get(),
        overlappingPairCache.get(),
        solver.get(),
        collisionConfiguration.get()
    );
    
    dynamicsWorld->setGravity(GLMToBullet(gravity));
}

void PhysicsSystem::Cleanup() {
    for (auto& [id, body] : rigidBodies) {
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeRigidBody(body);
        delete body;
    }
    rigidBodies.clear();
    
    for (auto& [id, shape] : collisionShapes) {
        delete shape;
    }
    collisionShapes.clear();
}

void PhysicsSystem::Update(float deltaTime) {
    std::bitset<MAX_COMPONENTS> mask;
    mask.set(Component::GetTypeId<Transform>());
    mask.set(Component::GetTypeId<RigidBody>());
    mask.set(Component::GetTypeId<Collider>());
    auto entities = world->GetEntitiesWithComponents(mask);
    
    static bool firstRun = true;
    if (firstRun) {
        std::cout << "PhysicsSystem: Found " << entities.size() << " entities with physics components" << std::endl;
        firstRun = false;
    }
    
    static int createdCount = 0;
    for (auto& entity : entities) {
        auto* rb = world->GetComponent<RigidBody>(entity);
        if (!rb->bulletBody) {
            CreateRigidBody(entity);
            if (rb->bulletBody) {
                createdCount++;
                if (createdCount % 10 == 0) {
                    std::cout << "Created " << createdCount << " physics bodies" << std::endl;
                }
            }
        }
    }
    
    dynamicsWorld->stepSimulation(deltaTime, 10);
    
    static int frameCount = 0;
    frameCount++;
    int syncCount = 0;
    for (auto& entity : entities) {
        auto* rb = world->GetComponent<RigidBody>(entity);
        if (rb && rb->bulletBody && rb->IsDynamic()) {
            SyncTransformFromBullet(entity);
            syncCount++;
        }
    }
    if (frameCount % 60 == 0) {  // Every second at 60fps
        std::cout << "Syncing " << syncCount << " dynamic bodies from physics" << std::endl;
    }
}

void PhysicsSystem::CreateRigidBody(std::shared_ptr<Entity> entity) {
    auto* transform = world->GetComponent<Transform>(entity);
    auto* rb = world->GetComponent<RigidBody>(entity);
    auto* collider = world->GetComponent<Collider>(entity);
    
    if (!transform || !rb || !collider) {
        std::cout << "ERROR: Missing components for physics body creation!" << std::endl;
        return;
    }
    
    btCollisionShape* shape = CreateCollisionShape(*collider);
    if (!shape) return;
    
    collisionShapes[entity->GetId()] = shape;
    
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(GLMToBullet(transform->position));
    startTransform.setRotation(GLMToBullet(transform->rotation));
    
    btVector3 localInertia(0, 0, 0);
    float mass = rb->IsStatic() ? 0.0f : rb->mass;
    
    if (mass != 0.0f) {
        shape->calculateLocalInertia(mass, localInertia);
    }
    
    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
    rbInfo.m_friction = rb->friction;
    rbInfo.m_restitution = rb->restitution;
    rbInfo.m_linearDamping = rb->linearDamping;
    rbInfo.m_angularDamping = rb->angularDamping;
    
    btRigidBody* body = new btRigidBody(rbInfo);
    
    if (rb->IsKinematic()) {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
    } else if (rb->IsDynamic()) {
        // Keep dynamic bodies active, especially important for player
        body->setActivationState(DISABLE_DEACTIVATION);
    }
    
    body->setLinearVelocity(GLMToBullet(rb->linearVelocity));
    body->setAngularVelocity(GLMToBullet(rb->angularVelocity));
    
    dynamicsWorld->addRigidBody(body);
    
    rb->bulletBody = body;
    rigidBodies[entity->GetId()] = body;
}

void PhysicsSystem::DestroyRigidBody(std::shared_ptr<Entity> entity) {
    auto it = rigidBodies.find(entity->GetId());
    if (it != rigidBodies.end()) {
        btRigidBody* body = it->second;
        
        if (body->getMotionState()) {
            delete body->getMotionState();
        }
        
        dynamicsWorld->removeRigidBody(body);
        delete body;
        
        rigidBodies.erase(it);
    }
    
    auto shapeIt = collisionShapes.find(entity->GetId());
    if (shapeIt != collisionShapes.end()) {
        delete shapeIt->second;
        collisionShapes.erase(shapeIt);
    }
    
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb) {
        rb->bulletBody = nullptr;
        rb->collisionShape = nullptr;
    }
}

void PhysicsSystem::UpdateRigidBody(std::shared_ptr<Entity> entity) {
    auto* transform = world->GetComponent<Transform>(entity);
    auto* rb = world->GetComponent<RigidBody>(entity);
    
    if (!transform || !rb || !rb->bulletBody) return;
    
    if (rb->IsKinematic()) {
        SyncTransformToBullet(entity);
    }
}

void PhysicsSystem::SetGravity(const glm::vec3& g) {
    gravity = g;
    if (dynamicsWorld) {
        dynamicsWorld->setGravity(GLMToBullet(gravity));
    }
}

void PhysicsSystem::EnableGravity(bool enable) {
    gravityEnabled = enable;
    if (dynamicsWorld) {
        btVector3 newGravity = enable ? GLMToBullet(gravity) : btVector3(0, 0, 0);
        dynamicsWorld->setGravity(newGravity);
        std::cout << "Physics: Gravity set to " << newGravity.getY() << std::endl;
        
        // Wake up all bodies when gravity changes
        for (auto& [id, body] : rigidBodies) {
            if (body) {
                body->activate();
            }
        }
    }
}

void PhysicsSystem::ApplyForce(std::shared_ptr<Entity> entity, const glm::vec3& force, const glm::vec3& relativePos) {
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb && rb->bulletBody && rb->IsDynamic()) {
        rb->bulletBody->applyForce(GLMToBullet(force), GLMToBullet(relativePos));
        rb->bulletBody->activate();
    }
}

void PhysicsSystem::ApplyTorque(std::shared_ptr<Entity> entity, const glm::vec3& torque) {
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb && rb->bulletBody && rb->IsDynamic()) {
        rb->bulletBody->applyTorque(GLMToBullet(torque));
        rb->bulletBody->activate();
    }
}

void PhysicsSystem::ApplyImpulse(std::shared_ptr<Entity> entity, const glm::vec3& impulse, const glm::vec3& relativePos) {
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb && rb->bulletBody && rb->IsDynamic()) {
        rb->bulletBody->applyImpulse(GLMToBullet(impulse), GLMToBullet(relativePos));
        rb->bulletBody->activate();
    }
}

void PhysicsSystem::SetLinearVelocity(std::shared_ptr<Entity> entity, const glm::vec3& velocity) {
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb && rb->bulletBody) {
        rb->bulletBody->setLinearVelocity(GLMToBullet(velocity));
        rb->bulletBody->activate();
    }
}

void PhysicsSystem::SetAngularVelocity(std::shared_ptr<Entity> entity, const glm::vec3& velocity) {
    auto* rb = world->GetComponent<RigidBody>(entity);
    if (rb && rb->bulletBody) {
        rb->bulletBody->setAngularVelocity(GLMToBullet(velocity));
        rb->bulletBody->activate();
    }
}

btCollisionShape* PhysicsSystem::CreateCollisionShape(const Collider& collider) {
    switch (collider.type) {
        case ColliderType::Box:
            return new btBoxShape(GLMToBullet(collider.size * 0.5f));
            
        case ColliderType::Sphere:
            return new btSphereShape(collider.radius);
            
        case ColliderType::Capsule:
            return new btCapsuleShape(collider.radius, collider.height);
            
        case ColliderType::Plane:
            return new btStaticPlaneShape(GLMToBullet(collider.size), 0);
            
        default:
            return new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    }
}

void PhysicsSystem::SyncTransformFromBullet(std::shared_ptr<Entity> entity) {
    auto* transform = world->GetComponent<Transform>(entity);
    auto* rb = world->GetComponent<RigidBody>(entity);
    
    if (!transform || !rb || !rb->bulletBody) return;
    
    btTransform bulletTransform;
    rb->bulletBody->getMotionState()->getWorldTransform(bulletTransform);
    
    transform->position = BulletToGLM(bulletTransform.getOrigin());
    transform->rotation = BulletToGLM(bulletTransform.getRotation());
    
    rb->linearVelocity = BulletToGLM(rb->bulletBody->getLinearVelocity());
    rb->angularVelocity = BulletToGLM(rb->bulletBody->getAngularVelocity());
}

void PhysicsSystem::SyncTransformToBullet(std::shared_ptr<Entity> entity) {
    auto* transform = world->GetComponent<Transform>(entity);
    auto* rb = world->GetComponent<RigidBody>(entity);
    
    if (!transform || !rb || !rb->bulletBody) return;
    
    btTransform bulletTransform;
    bulletTransform.setOrigin(GLMToBullet(transform->position));
    bulletTransform.setRotation(GLMToBullet(transform->rotation));
    
    rb->bulletBody->getMotionState()->setWorldTransform(bulletTransform);
    rb->bulletBody->setWorldTransform(bulletTransform);
    rb->bulletBody->activate(); // Wake up the body
}

glm::vec3 PhysicsSystem::BulletToGLM(const btVector3& v) {
    return glm::vec3(v.x(), v.y(), v.z());
}

btVector3 PhysicsSystem::GLMToBullet(const glm::vec3& v) {
    return btVector3(v.x, v.y, v.z);
}

glm::quat PhysicsSystem::BulletToGLM(const btQuaternion& q) {
    return glm::quat(q.w(), q.x(), q.y(), q.z());
}

btQuaternion PhysicsSystem::GLMToBullet(const glm::quat& q) {
    return btQuaternion(q.x, q.y, q.z, q.w);
}

}