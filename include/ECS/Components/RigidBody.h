#ifndef ECS_RIGIDBODY_H
#define ECS_RIGIDBODY_H

#include <glm/glm.hpp>
#include "../Component.h"

class btRigidBody;
class btCollisionShape;

namespace ECS {

enum class RigidBodyType {
    Static = 0,
    Dynamic = 1,
    Kinematic = 2
};

class RigidBody : public Component {
public:
    float mass;
    RigidBodyType type;
    glm::vec3 linearVelocity;
    glm::vec3 angularVelocity;
    float linearDamping;
    float angularDamping;
    float friction;
    float restitution;
    
    btRigidBody* bulletBody;
    btCollisionShape* collisionShape;
    
    RigidBody(float m = 1.0f, RigidBodyType t = RigidBodyType::Dynamic)
        : mass(m), type(t),
          linearVelocity(0.0f), angularVelocity(0.0f),
          linearDamping(0.0f), angularDamping(0.0f),
          friction(0.5f), restitution(0.0f),
          bulletBody(nullptr), collisionShape(nullptr) {}
    
    ~RigidBody() {
    }
    
    bool IsStatic() const { return type == RigidBodyType::Static; }
    bool IsDynamic() const { return type == RigidBodyType::Dynamic; }
    bool IsKinematic() const { return type == RigidBodyType::Kinematic; }
};

}

#endif