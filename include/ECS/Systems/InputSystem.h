#ifndef ECS_INPUT_SYSTEM_H
#define ECS_INPUT_SYSTEM_H

#include <GLFW/glfw3.h>
#include "../System.h"
#include "../Components/Input.h"

namespace ECS {

class InputSystem : public System {
public:
    InputSystem(GLFWwindow* window) : window(window), firstMouse(true) {
        RequireComponents<Input>();
        SetPriority(-100); // Run first
        
        lastMouseX = 0.0f;
        lastMouseY = 0.0f;
    }
    
    void Update(float deltaTime) override {
        if (!window) return;
        
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Input>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        for (auto& entity : entities) {
            auto* input = world->GetComponent<Input>(entity);
            if (!input) continue;
            
            // Update keyboard state
            UpdateKeyboardState(input);
            
            // Update mouse state
            UpdateMouseState(input);
            
            // Update scroll (handled via callback)
        }
    }
    
    void PostUpdate(float deltaTime) override {
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Input>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        for (auto& entity : entities) {
            auto* input = world->GetComponent<Input>(entity);
            if (input) {
                input->EndFrame();
            }
        }
    }
    
    void SetScrollDelta(float xoffset, float yoffset) {
        scrollX = xoffset;
        scrollY = yoffset;
    }
    
private:
    void UpdateKeyboardState(Input* input) {
        // Common game keys
        int keysToCheck[] = {
            GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D,
            GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL,
            GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
            GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_TAB,
            GLFW_KEY_Q, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_F,
            GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5
        };
        
        for (int key : keysToCheck) {
            bool pressed = glfwGetKey(window, key) == GLFW_PRESS;
            input->UpdateKey(key, pressed);
        }
    }
    
    void UpdateMouseState(Input* input) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if (firstMouse) {
            lastMouseX = static_cast<float>(xpos);
            lastMouseY = static_cast<float>(ypos);
            firstMouse = false;
        }
        
        float deltaX = static_cast<float>(xpos) - lastMouseX;
        float deltaY = lastMouseY - static_cast<float>(ypos); // Reversed
        
        input->mousePosition = glm::vec2(xpos, ypos);
        input->mouseDelta = glm::vec2(deltaX, deltaY);
        input->scrollDelta = glm::vec2(scrollX, scrollY);
        
        lastMouseX = static_cast<float>(xpos);
        lastMouseY = static_cast<float>(ypos);
        
        // Mouse buttons
        input->mouseButtons[0] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        input->mouseButtons[1] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        input->mouseButtons[2] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
        
        // Reset scroll for next frame
        scrollX = 0.0f;
        scrollY = 0.0f;
    }
    
    GLFWwindow* window;
    bool firstMouse;
    float lastMouseX;
    float lastMouseY;
    float scrollX = 0.0f;
    float scrollY = 0.0f;
};

}

#endif