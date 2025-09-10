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
    
    world.AddSystem(std::make_unique<ECS::PlayerControllerSystem>());
    world.AddSystem(std::make_unique<ECS::MovementSystem>());
    world.AddSystem(std::make_unique<ECS::BoundsSystem>());
    
    auto renderSystem = std::make_unique<ECS::RenderSystem>(&cubeRenderer, &shader);
    ECS::RenderSystem* renderSysPtr = renderSystem.get();
    world.AddSystem(std::move(renderSystem));

    // Create player entity
    auto player = world.CreateEntity();
    world.AddComponent<ECS::Transform>(player, 
        ECS::Transform(glm::vec3(0.0f, 5.0f, 0.0f)));
    world.AddComponent<ECS::Velocity>(player, ECS::Velocity());
    world.AddComponent<ECS::Renderable>(player, 
        ECS::Renderable(ECS::MeshType::Cube, glm::vec3(0.2f, 0.8f, 0.2f)));
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
        
        glm::vec3 position(posDistX(gen), posDistY(gen), posDistZ(gen));
        glm::vec3 scale(scaleDist(gen));
        world.AddComponent<ECS::Transform>(cube, 
            ECS::Transform(position, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), scale));
        
        glm::vec3 linearVel(velDist(gen), velDist(gen), velDist(gen));
        glm::vec3 angularVel(angVelDist(gen), angVelDist(gen), angVelDist(gen));
        world.AddComponent<ECS::Velocity>(cube, 
            ECS::Velocity(linearVel, angularVel));
        
        glm::vec3 color(colorDist(gen), colorDist(gen), colorDist(gen));
        world.AddComponent<ECS::Renderable>(cube, 
            ECS::Renderable(ECS::MeshType::Cube, color));
        
        world.AddComponent<ECS::Tag>(cube, ECS::Tag("Cube"));
    }

    // Create some static cubes as ground
    for (int x = -5; x <= 5; x++) {
        for (int z = -5; z <= 5; z++) {
            auto ground = world.CreateEntity();
            world.AddComponent<ECS::Transform>(ground,
                ECS::Transform(glm::vec3(x * 4.0f, -2.0f, z * 4.0f),
                              glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                              glm::vec3(2.0f, 0.1f, 2.0f)));
            world.AddComponent<ECS::Renderable>(ground,
                ECS::Renderable(ECS::MeshType::Cube, glm::vec3(0.3f, 0.3f, 0.3f)));
            world.AddComponent<ECS::Tag>(ground, ECS::Tag("Ground"));
        }
    }

    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move player" << std::endl;
    std::cout << "  Space - Jump" << std::endl;
    std::cout << "  Shift - Move down" << std::endl;
    std::cout << "  Q/E or Arrow Keys - Rotate player" << std::endl;
    std::cout << "  Right Mouse + Move - Look around" << std::endl;
    std::cout << "  1 - Spawn new cube at player position" << std::endl;
    std::cout << "  2 - Remove random cube" << std::endl;
    std::cout << "  3 - Toggle cube visibility" << std::endl;
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
                        world.AddComponent<ECS::Velocity>(cube,
                            ECS::Velocity(playerTransform->GetForward() * 10.0f, glm::vec3(1.0f, 2.0f, 0.5f)));
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
                world.DestroyEntity(cubes[cubeDist(gen)]);
                std::cout << "Removed a cube! (" << cubes.size() - 1 << " remaining)" << std::endl;
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

        // Apply continuous spin to non-player cubes if enabled
        if (cubeSpin) {
            spinTime += deltaTime;
            for (auto& entity : world.GetAllEntities()) {
                auto* tag = world.GetComponent<ECS::Tag>(entity);
                if (tag && tag->name == "Cube") {
                    auto* velocity = world.GetComponent<ECS::Velocity>(entity);
                    if (!velocity) {
                        world.AddComponent<ECS::Velocity>(entity, 
                            ECS::Velocity(glm::vec3(0.0f), glm::vec3(0.5f, 1.0f, 0.2f)));
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