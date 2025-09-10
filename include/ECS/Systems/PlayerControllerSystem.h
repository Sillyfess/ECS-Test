#ifndef ECS_PLAYER_CONTROLLER_SYSTEM_H
#define ECS_PLAYER_CONTROLLER_SYSTEM_H

#include <GLFW/glfw3.h>
#include <iostream>
#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/Velocity.h"
#include "../Components/Input.h"
#include "../Components/Tag.h"
#include "../Components/RigidBody.h"
#include "PhysicsSystem.h"

namespace ECS {

class PlayerControllerSystem : public System {
public:
    float moveSpeed = 10.0f;
    float moveForce = 200.0f;  // Increased from 50
    float rotateSpeed = 2.0f;
    float jumpSpeed = 15.0f;
    float jumpForce = 5000.0f;  // Greatly increased for better jump
    
    // Jump state tracking
    bool canJump = true;
    float jumpCooldown = 0.5f;
    float jumpTimer = 0.0f;
    
    // Mouse look state
    bool wasRightMousePressed = false;
    
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
            
            // Check if entity has physics
            auto* rb = world->GetComponent<RigidBody>(entity);
            
            if (rb && rb->bulletBody) {
                // Physics-based movement
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
                
                // Apply movement force
                if (glm::length(movement) > 0.0f) {
                    movement = glm::normalize(movement) * moveForce;
                    auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                    if (physicsSystem) {
                        physicsSystem->ApplyForce(entity, movement);
                    }
                }
                
                // Jump - use IsKeyHeld for more reliable detection
                // Update jump cooldown
                if (jumpTimer > 0.0f) {
                    jumpTimer -= deltaTime;
                    if (jumpTimer <= 0.0f) {
                        canJump = true;
                    }
                }
                
                if (input->IsKeyHeld(GLFW_KEY_SPACE) && canJump) {
                    std::cout << "JUMP! Applying impulse of " << jumpForce << std::endl;
                    auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                    if (physicsSystem) {
                        // Apply large upward impulse
                        physicsSystem->ApplyImpulse(entity, glm::vec3(0, jumpForce, 0), glm::vec3(0,0,0));
                        canJump = false;
                        jumpTimer = jumpCooldown;
                    }
                }
                
                // Downward force
                if (input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
                    auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                    if (physicsSystem) {
                        physicsSystem->ApplyForce(entity, glm::vec3(0, -moveForce * 2, 0));
                    }
                }
            } else {
                // Non-physics movement (original code)
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
            }
            
            // Rotation from arrow keys or Q/E - use torque for physics bodies
            if (rb && rb->bulletBody) {
                // Physics-based rotation
                float torqueStrength = 100.0f;
                if (input->IsKeyHeld(GLFW_KEY_LEFT) || input->IsKeyHeld(GLFW_KEY_Q)) {
                    auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                    if (physicsSystem) {
                        physicsSystem->ApplyTorque(entity, glm::vec3(0, torqueStrength, 0));
                    }
                } else if (input->IsKeyHeld(GLFW_KEY_RIGHT) || input->IsKeyHeld(GLFW_KEY_E)) {
                    auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                    if (physicsSystem) {
                        physicsSystem->ApplyTorque(entity, glm::vec3(0, -torqueStrength, 0));
                    }
                }
            } else {
                // Non-physics rotation
                if (input->IsKeyHeld(GLFW_KEY_LEFT) || input->IsKeyHeld(GLFW_KEY_Q)) {
                    velocity->angular.y = rotateSpeed;
                } else if (input->IsKeyHeld(GLFW_KEY_RIGHT) || input->IsKeyHeld(GLFW_KEY_E)) {
                    velocity->angular.y = -rotateSpeed;
                } else {
                    velocity->angular.y *= 0.9f; // Angular friction
                }
            }
            
            // Mouse look - track button state changes
            if (input->mouseButtons[1]) { // Right mouse button held
                if (!wasRightMousePressed) {
                    std::cout << "Right mouse pressed - mouse look enabled" << std::endl;
                    wasRightMousePressed = true;
                }
                
                float sensitivity = 0.005f;
                // Only rotate if mouse is actually moving
                if (glm::length(input->mouseDelta) > 0.01f) {
                    std::cout << "Mouse delta: " << input->mouseDelta.x << ", " << input->mouseDelta.y << std::endl;
                    glm::quat yaw = glm::angleAxis(-input->mouseDelta.x * sensitivity, glm::vec3(0, 1, 0));
                    transform->rotation = yaw * transform->rotation;
                    
                    // Sync rotation to physics body if it exists
                    if (rb && rb->bulletBody) {
                        auto* physicsSystem = world->GetSystem<PhysicsSystem>();
                        if (physicsSystem) {
                            physicsSystem->SyncTransformToBullet(entity);
                        }
                    }
                }
            } else {
                if (wasRightMousePressed) {
                    std::cout << "Right mouse released - mouse look disabled" << std::endl;
                    wasRightMousePressed = false;
                }
            }
        }
    }
};

}

#endif