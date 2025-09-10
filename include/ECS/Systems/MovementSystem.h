#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include <glm/gtc/quaternion.hpp>
#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/Velocity.h"
#include "../Components/RigidBody.h"

namespace ECS {

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponents<Transform, Velocity>();
        SetPriority(0);
    }
    
    void Update(float deltaTime) override {
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Transform>());
        mask.set(Component::GetTypeId<Velocity>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        for (auto& entity : entities) {
            auto* rb = world->GetComponent<RigidBody>(entity);
            
            if (rb && rb->bulletBody) {
                continue;
            }
            
            auto* transform = world->GetComponent<Transform>(entity);
            auto* velocity = world->GetComponent<Velocity>(entity);
            
            if (!transform || !velocity) continue;
            
            // Update position
            transform->position += velocity->linear * deltaTime;
            
            // Update rotation
            if (glm::length(velocity->angular) > 0.0f) {
                glm::vec3 axis = glm::normalize(velocity->angular);
                float angle = glm::length(velocity->angular) * deltaTime;
                glm::quat deltaRotation = glm::angleAxis(angle, axis);
                transform->rotation = deltaRotation * transform->rotation;
                transform->rotation = glm::normalize(transform->rotation);
            }
        }
    }
};

}

#endif