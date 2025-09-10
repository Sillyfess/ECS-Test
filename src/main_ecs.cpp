#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Camera.h"
#include "CubeRenderer.h"

// ECS includes
#include "ECS/World.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Renderable.h"
#include "ECS/Components/Input.h"
#include "ECS/Components/Tag.h"
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/BoundsSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Collider.h"
#include "ECS/Systems/PhysicsSystem.h"

#include <iostream>
#include <vector>
#include <random>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 10.0f, 50.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Global pointer to input system for scroll callback
ECS::InputSystem* g_inputSystem = nullptr;
ECS::World* g_world = nullptr;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ECS Cube Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Initialize rendering
    Shader shader("shaders/cube.vert", "shaders/cube.frag");
    CubeRenderer cubeRenderer;
    cubeRenderer.initialize();

    // Create ECS World
    ECS::World world;
    g_world = &world;

    // Add systems
    auto inputSystem = std::make_unique<ECS::InputSystem>(window);
    g_inputSystem = inputSystem.get();
    world.AddSystem(std::move(inputSystem));
    
    auto physicsSystem = std::make_unique<ECS::PhysicsSystem>();
    ECS::PhysicsSystem* physicsSysPtr = physicsSystem.get();
    world.AddSystem(std::move(physicsSystem));
    
    world.AddSystem(std::make_unique<ECS::PlayerControllerSystem>());
    world.AddSystem(std::make_unique<ECS::MovementSystem>());
    world.AddSystem(std::make_unique<ECS::BoundsSystem>());
    
    auto renderSystem = std::make_unique<ECS::RenderSystem>(&cubeRenderer, &shader);
    ECS::RenderSystem* renderSysPtr = renderSystem.get();
    world.AddSystem(std::move(renderSystem));

    // Create player entity with physics
    auto player = world.CreateEntity();
    world.AddComponent<ECS::Transform>(player, 
        ECS::Transform(glm::vec3(0.0f, 5.0f, 0.0f)));
    world.AddComponent<ECS::Velocity>(player, ECS::Velocity());
    world.AddComponent<ECS::Renderable>(player, 
        ECS::Renderable(ECS::MeshType::Cube, glm::vec3(0.2f, 0.8f, 0.2f)));
    world.AddComponent<ECS::RigidBody>(player, 
        ECS::RigidBody(1.0f, ECS::RigidBodyType::Dynamic));
    world.AddComponent<ECS::Collider>(player, 
        ECS::Collider::Box(glm::vec3(1.0f, 1.0f, 1.0f)));
    world.AddComponent<ECS::Input>(player, ECS::Input());
    world.AddComponent<ECS::Tag>(player, ECS::Tag("Player"));

    // Create random cubes
    const int CUBE_COUNT = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> posDistX(-40.0f, 40.0f);
    std::uniform_real_distribution<> posDistY(-15.0f, 15.0f);
    std::uniform_real_distribution<> posDistZ(-40.0f, 40.0f);
    std::uniform_real_distribution<> velDist(-5.0f, 5.0f);
    std::uniform_real_distribution<> angVelDist(-1.0f, 1.0f);
    std::uniform_real_distribution<> scaleDist(0.5f, 2.0f);
    std::uniform_real_distribution<> colorDist(0.3f, 1.0f);

    for (int i = 0; i < CUBE_COUNT; i++) {
        auto cube = world.CreateEntity();
        
        glm::vec3 position(posDistX(gen), posDistY(gen) + 10.0f, posDistZ(gen));
        glm::vec3 scale(scaleDist(gen));
        world.AddComponent<ECS::Transform>(cube, 
            ECS::Transform(position, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), scale));
        
        // Add physics to cubes
        auto rb = ECS::RigidBody(scale.x * 1.0f, ECS::RigidBodyType::Dynamic);
        rb.linearVelocity = glm::vec3(velDist(gen), 0, velDist(gen));
        rb.angularVelocity = glm::vec3(angVelDist(gen), angVelDist(gen), angVelDist(gen));
        rb.friction = 0.5f;
        rb.restitution = 0.3f;
        world.AddComponent<ECS::RigidBody>(cube, rb);
        world.AddComponent<ECS::Collider>(cube, 
            ECS::Collider::Box(scale));
        
        glm::vec3 color(colorDist(gen), colorDist(gen), colorDist(gen));
        world.AddComponent<ECS::Renderable>(cube, 
            ECS::Renderable(ECS::MeshType::Cube, color));
        
        world.AddComponent<ECS::Tag>(cube, ECS::Tag("Cube"));
    }

    // Create static ground plane with physics
    auto ground = world.CreateEntity();
    world.AddComponent<ECS::Transform>(ground,
        ECS::Transform(glm::vec3(0.0f, -5.0f, 0.0f),
                      glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                      glm::vec3(100.0f, 0.1f, 100.0f)));
    world.AddComponent<ECS::RigidBody>(ground,
        ECS::RigidBody(0.0f, ECS::RigidBodyType::Static));
    world.AddComponent<ECS::Collider>(ground,
        ECS::Collider::Box(glm::vec3(100.0f, 0.1f, 100.0f)));
    world.AddComponent<ECS::Renderable>(ground,
        ECS::Renderable(ECS::MeshType::Cube, glm::vec3(0.3f, 0.3f, 0.3f)));
    world.AddComponent<ECS::Tag>(ground, ECS::Tag("Ground"));

    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move player" << std::endl;
    std::cout << "  Space - Jump" << std::endl;
    std::cout << "  Shift - Move down" << std::endl;
    std::cout << "  Q/E or Arrow Keys - Rotate player" << std::endl;
    std::cout << "  Right Mouse + Move - Look around" << std::endl;
    std::cout << "  1 - Spawn new cube at player position" << std::endl;
    std::cout << "  2 - Remove random cube" << std::endl;
    std::cout << "  3 - Toggle cube spinning" << std::endl;
    std::cout << "  4 - Toggle gravity" << std::endl;
    std::cout << "  5 - Apply random impulse to cubes" << std::endl;
    std::cout << "  6 - Reset cube positions" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;

    bool cubeSpin = true;
    float spinTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check for exit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Interactive features
        static bool key1Pressed = false;
        static bool key2Pressed = false;
        static bool key3Pressed = false;
        static bool key4Pressed = false;
        static bool key5Pressed = false;
        static bool key6Pressed = false;

        // Spawn cube at player position (key 1)
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed) {
            key1Pressed = true;
            
            // Find player
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && tag->name == "Player") {
                    auto* playerTransform = world.GetComponent<ECS::Transform>(entity);
                    if (playerTransform) {
                        auto cube = world.CreateEntity();
                        glm::vec3 spawnPos = playerTransform->position + playerTransform->GetForward() * 3.0f;
                        world.AddComponent<ECS::Transform>(cube,
                            ECS::Transform(spawnPos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f)));
                        
                        // Add physics to spawned cube
                        auto rb = ECS::RigidBody(1.0f, ECS::RigidBodyType::Dynamic);
                        rb.linearVelocity = playerTransform->GetForward() * 10.0f;
                        rb.angularVelocity = glm::vec3(1.0f, 2.0f, 0.5f);
                        rb.friction = 0.4f;
                        rb.restitution = 0.6f;
                        world.AddComponent<ECS::RigidBody>(cube, rb);
                        world.AddComponent<ECS::Collider>(cube,
                            ECS::Collider::Box(glm::vec3(1.0f)));
                        
                        world.AddComponent<ECS::Renderable>(cube,
                            ECS::Renderable(ECS::MeshType::Cube, glm::vec3(1.0f, 0.0f, 1.0f)));
                        world.AddComponent<ECS::Tag>(cube, ECS::Tag("Spawned"));
                        std::cout << "Spawned new cube!" << std::endl;
                    }
                    break;
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
            key1Pressed = false;
        }

        // Remove random cube (key 2)
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed) {
            key2Pressed = true;
            
            std::vector<std::shared_ptr<ECS::Entity>> cubes;
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && (tag->name == "Cube" || tag->name == "Spawned")) {
                    cubes.push_back(entity);
                }
            }
            
            if (!cubes.empty()) {
                std::uniform_int_distribution<> cubeDist(0, cubes.size() - 1);
                auto cubeToRemove = cubes[cubeDist(gen)];
                
                // Remove physics body first
                auto* rb = world.GetComponent<ECS::RigidBody>(cubeToRemove);
                if (rb && rb->bulletBody) {
                    physicsSysPtr->DestroyRigidBody(cubeToRemove);
                }
                
                world.DestroyEntity(cubeToRemove);
                std::cout << "Removed a cube! (" << (cubes.size() - 1) << " remaining)" << std::endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) {
            key2Pressed = false;
        }

        // Toggle spin (key 3)
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed) {
            key3Pressed = true;
            cubeSpin = !cubeSpin;
            std::cout << "Cube spin: " << (cubeSpin ? "ON" : "OFF") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) {
            key3Pressed = false;
        }
        
        // Toggle gravity (key 4)
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !key4Pressed) {
            key4Pressed = true;
            bool gravityEnabled = physicsSysPtr->IsGravityEnabled();
            physicsSysPtr->EnableGravity(!gravityEnabled);
            std::cout << "Gravity: " << (!gravityEnabled ? "ON" : "OFF") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) {
            key4Pressed = false;
        }
        
        // Apply random impulse to all cubes (key 5)
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !key5Pressed) {
            key5Pressed = true;
            int count = 0;
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                auto* rb = world.GetComponent<ECS::RigidBody>(entity);
                if (tag && (tag->name == "Cube" || tag->name == "Spawned")) {
                    if (rb && rb->bulletBody) {
                        glm::vec3 impulse(
                            (rand() % 20 - 10) * 20.0f,  // Increased force
                            (rand() % 10 + 5) * 30.0f,   // Increased force
                            (rand() % 20 - 10) * 20.0f   // Increased force
                        );
                        physicsSysPtr->ApplyImpulse(entity, impulse);
                        count++;
                    } else {
                        std::cout << "WARNING: Cube has no physics body!" << std::endl;
                    }
                }
            }
            std::cout << "Applied random impulse to " << count << " cubes!" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE) {
            key5Pressed = false;
        }
        
        // Reset cube positions (key 6)
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !key6Pressed) {
            key6Pressed = true;
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && (tag->name == "Cube" || tag->name == "Spawned")) {
                    auto* transform = world.GetComponent<ECS::Transform>(entity);
                    auto* rb = world.GetComponent<ECS::RigidBody>(entity);
                    if (transform && rb && rb->bulletBody) {
                        transform->position.y = std::abs(transform->position.y) + 15.0f;
                        transform->position.x = (rand() % 80 - 40) * 0.5f;
                        transform->position.z = (rand() % 80 - 40) * 0.5f;
                        // Sync transform to physics for dynamic bodies
                        physicsSysPtr->SyncTransformToBullet(entity);
                        physicsSysPtr->SetLinearVelocity(entity, glm::vec3(0.0f));
                        physicsSysPtr->SetAngularVelocity(entity, glm::vec3(0.0f));
                    }
                }
            }
            std::cout << "Reset all cube positions!" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE) {
            key6Pressed = false;
        }

        // Apply continuous spin to non-player cubes if enabled
        if (cubeSpin) {
            spinTime += deltaTime;
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && (tag->name == "Cube" || tag->name == "Spawned")) {
                    auto* rb = world.GetComponent<ECS::RigidBody>(entity);
                    if (rb && rb->bulletBody) {
                        // Set angular velocity for physics bodies
                        physicsSysPtr->SetAngularVelocity(entity, glm::vec3(0.5f, 1.0f, 0.2f));
                    } else {
                        // For non-physics entities, use velocity component
                        auto* velocity = world.GetComponent<ECS::Velocity>(entity);
                        if (!velocity) {
                            world.AddComponent<ECS::Velocity>(entity, 
                                ECS::Velocity(glm::vec3(0.0f), glm::vec3(0.5f, 1.0f, 0.2f)));
                        }
                    }
                }
            }
        } else {
            // Stop spinning when disabled
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && (tag->name == "Cube" || tag->name == "Spawned")) {
                    auto* rb = world.GetComponent<ECS::RigidBody>(entity);
                    if (rb && rb->bulletBody) {
                        physicsSysPtr->SetAngularVelocity(entity, glm::vec3(0.0f));
                    }
                }
            }
        }

        // Update ECS
        world.Update(deltaTime);

        // Render
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        
        // Follow player with camera
        for (auto& entity : world.GetAllEntities()) {
            auto* tag = world.GetComponent<ECS::Tag>(entity);
            if (tag && tag->name == "Player") {
                auto* transform = world.GetComponent<ECS::Transform>(entity);
                if (transform) {
                    glm::vec3 cameraOffset = transform->position - transform->GetForward() * 10.0f + glm::vec3(0, 5, 0);
                    camera.Position = cameraOffset;
                    camera.Front = glm::normalize(transform->position - cameraOffset);
                }
                break;
            }
        }
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        shader.setVec3("lightPos", glm::vec3(10.0f, 20.0f, 10.0f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));

        renderSysPtr->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cubeRenderer.cleanup();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_inputSystem) {
        g_inputSystem->SetScrollDelta(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}