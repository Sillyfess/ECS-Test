#ifndef ECS_BOUNDS_SYSTEM_H
#define ECS_BOUNDS_SYSTEM_H

#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/Velocity.h"

namespace ECS {

class BoundsSystem : public System {
public:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    bool wrapAround;
    
    BoundsSystem(const glm::vec3& min = glm::vec3(-50.0f, -20.0f, -50.0f),
                  const glm::vec3& max = glm::vec3(50.0f, 20.0f, 50.0f),
                  bool wrap = false)
        : minBounds(min), maxBounds(max), wrapAround(wrap) {
        RequireComponents<Transform>();
        SetPriority(10);
    }
    
    void Update(float deltaTime) override {
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Transform>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        for (auto& entity : entities) {
            auto* transform = world->GetComponent<Transform>(entity);
            if (!transform) continue;
            
            auto* velocity = world->GetComponent<Velocity>(entity);
            
            // Check each axis
            for (int i = 0; i < 3; ++i) {
                if (transform->position[i] < minBounds[i]) {
                    if (wrapAround) {
                        transform->position[i] = maxBounds[i];
                    } else {
                        transform->position[i] = minBounds[i];
                        if (velocity) {
                            velocity->linear[i] = -velocity->linear[i] * 0.8f; // Bounce with damping
                        }
                    }
                } else if (transform->position[i] > maxBounds[i]) {
                    if (wrapAround) {
                        transform->position[i] = minBounds[i];
                    } else {
                        transform->position[i] = maxBounds[i];
                        if (velocity) {
                            velocity->linear[i] = -velocity->linear[i] * 0.8f; // Bounce with damping
                        }
                    }
                }
            }
        }
    }
};

}

#endif