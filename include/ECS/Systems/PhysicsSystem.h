#ifndef ECS_PHYSICS_SYSTEM_H
#define ECS_PHYSICS_SYSTEM_H

#include <memory>
#include <unordered_map>
#include <btBulletDynamicsCommon.h>
#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/RigidBody.h"
#include "../Components/Collider.h"

namespace ECS {

class PhysicsSystem : public System {
private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btDbvtBroadphase> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    
    std::unordered_map<uint32_t, btRigidBody*> rigidBodies;
    std::unordered_map<uint32_t, btCollisionShape*> collisionShapes;
    
    bool gravityEnabled;
    glm::vec3 gravity;
    
public:
    PhysicsSystem();
    ~PhysicsSystem();
    
    void Initialize();
    void Cleanup();
    
    void Update(float deltaTime) override;
    
    void CreateRigidBody(std::shared_ptr<Entity> entity);
    void DestroyRigidBody(std::shared_ptr<Entity> entity);
    void UpdateRigidBody(std::shared_ptr<Entity> entity);
    
    void SetGravity(const glm::vec3& g);
    void EnableGravity(bool enable);
    bool IsGravityEnabled() const { return gravityEnabled; }
    
    void ApplyForce(std::shared_ptr<Entity> entity, const glm::vec3& force, const glm::vec3& relativePos = glm::vec3(0.0f));
    void ApplyTorque(std::shared_ptr<Entity> entity, const glm::vec3& torque);
    void ApplyImpulse(std::shared_ptr<Entity> entity, const glm::vec3& impulse, const glm::vec3& relativePos = glm::vec3(0.0f));
    void SetLinearVelocity(std::shared_ptr<Entity> entity, const glm::vec3& velocity);
    void SetAngularVelocity(std::shared_ptr<Entity> entity, const glm::vec3& velocity);
    
    void SyncTransformToBullet(std::shared_ptr<Entity> entity);
    
    btDiscreteDynamicsWorld* GetDynamicsWorld() { return dynamicsWorld.get(); }
    
private:
    btCollisionShape* CreateCollisionShape(const Collider& collider);
    btRigidBody* CreateBulletRigidBody(const Transform& transform, const RigidBody& rb, const Collider& collider);
    void SyncTransformFromBullet(std::shared_ptr<Entity> entity);
    
    static glm::vec3 BulletToGLM(const btVector3& v);
    static btVector3 GLMToBullet(const glm::vec3& v);
    static glm::quat BulletToGLM(const btQuaternion& q);
    static btQuaternion GLMToBullet(const glm::quat& q);
};

}

#endif