#ifndef ECS_PLAYER_CONTROLLER_SYSTEM_H
#define ECS_PLAYER_CONTROLLER_SYSTEM_H

#include <GLFW/glfw3.h>
#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/Velocity.h"
#include "../Components/Input.h"
#include "../Components/Tag.h"

namespace ECS {

class PlayerControllerSystem : public System {
public:
    float moveSpeed = 10.0f;
    float rotateSpeed = 2.0f;
    float jumpSpeed = 15.0f;
    
    PlayerControllerSystem() {
        RequireComponents<Transform, Input, Tag>();
        SetPriority(-50); // Run after input but before movement
    }
    
    void Update(float deltaTime) override {
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Transform>());
        mask.set(Component::GetTypeId<Input>());
        mask.set(Component::GetTypeId<Tag>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        for (auto& entity : entities) {
            auto* tag = world->GetComponent<Tag>(entity);
            if (!tag || tag->name != "Player") continue;
            
            auto* transform = world->GetComponent<Transform>(entity);
            auto* input = world->GetComponent<Input>(entity);
            auto* velocity = world->GetComponent<Velocity>(entity);
            
            if (!transform || !input) continue;
            
            // Create velocity component if it doesn't exist
            if (!velocity) {
                velocity = world->AddComponent<Velocity>(entity, Velocity());
            }
            
            // Movement input
            glm::vec3 movement(0.0f);
            
            if (input->IsKeyHeld(GLFW_KEY_W)) {
                movement += transform->GetForward();
            }
            if (input->IsKeyHeld(GLFW_KEY_S)) {
                movement -= transform->GetForward();
            }
            if (input->IsKeyHeld(GLFW_KEY_A)) {
                movement -= transform->GetRight();
            }
            if (input->IsKeyHeld(GLFW_KEY_D)) {
                movement += transform->GetRight();
            }
            
            // Normalize diagonal movement
            if (glm::length(movement) > 0.0f) {
                movement = glm::normalize(movement) * moveSpeed;
                velocity->linear.x = movement.x;
                velocity->linear.z = movement.z;
            } else {
                velocity->linear.x *= 0.9f; // Friction
                velocity->linear.z *= 0.9f;
            }
            
            // Vertical movement
            if (input->IsKeyPressed(GLFW_KEY_SPACE)) {
                velocity->linear.y = jumpSpeed;
            }
            if (input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
                velocity->linear.y = -moveSpeed;
            }
            
            // Apply gravity if not on ground (simplified)
            if (transform->position.y > 0.1f) {
                velocity->linear.y -= 20.0f * deltaTime; // Gravity
            } else if (velocity->linear.y < 0.0f) {
                velocity->linear.y = 0.0f;
                transform->position.y = 0.0f;
            }
            
            // Rotation from arrow keys or Q/E
            if (input->IsKeyHeld(GLFW_KEY_LEFT) || input->IsKeyHeld(GLFW_KEY_Q)) {
                velocity->angular.y = rotateSpeed;
            } else if (input->IsKeyHeld(GLFW_KEY_RIGHT) || input->IsKeyHeld(GLFW_KEY_E)) {
                velocity->angular.y = -rotateSpeed;
            } else {
                velocity->angular.y *= 0.9f; // Angular friction
            }
            
            // Mouse look (optional - can be toggled)
            if (input->mouseButtons[1]) { // Right mouse button held
                float sensitivity = 0.005f;
                glm::quat yaw = glm::angleAxis(-input->mouseDelta.x * sensitivity, glm::vec3(0, 1, 0));
                transform->rotation = yaw * transform->rotation;
            }
        }
    }
};

}

#endif