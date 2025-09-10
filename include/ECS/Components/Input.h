#ifndef ECS_INPUT_H
#define ECS_INPUT_H

#include <glm/glm.hpp>
#include <unordered_map>
#include "../Component.h"

namespace ECS {

enum class KeyState {
    Released,
    Pressed,
    Held
};

class Input : public Component {
public:
    std::unordered_map<int, KeyState> keys;
    glm::vec2 mousePosition;
    glm::vec2 mouseDelta;
    glm::vec2 scrollDelta;
    bool mouseButtons[3] = {false, false, false}; // Left, Right, Middle
    
    Input() : mousePosition(0.0f), mouseDelta(0.0f), scrollDelta(0.0f) {}
    
    bool IsKeyPressed(int key) const {
        auto it = keys.find(key);
        return it != keys.end() && it->second == KeyState::Pressed;
    }
    
    bool IsKeyHeld(int key) const {
        auto it = keys.find(key);
        return it != keys.end() && 
               (it->second == KeyState::Pressed || it->second == KeyState::Held);
    }
    
    bool IsKeyReleased(int key) const {
        auto it = keys.find(key);
        return it != keys.end() && it->second == KeyState::Released;
    }
    
    void UpdateKey(int key, bool pressed) {
        auto it = keys.find(key);
        if (pressed) {
            if (it == keys.end() || it->second == KeyState::Released) {
                keys[key] = KeyState::Pressed;
            } else if (it->second == KeyState::Pressed) {
                keys[key] = KeyState::Held;
            }
        } else {
            keys[key] = KeyState::Released;
        }
    }
    
    void EndFrame() {
        for (auto& [key, state] : keys) {
            if (state == KeyState::Pressed) {
                state = KeyState::Held;
            }
        }
        mouseDelta = glm::vec2(0.0f);
        scrollDelta = glm::vec2(0.0f);
    }
};

}

#endif