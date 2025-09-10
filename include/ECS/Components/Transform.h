#ifndef ECS_TRANSFORM_H
#define ECS_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../Component.h"

namespace ECS {

class Transform : public Component {
public:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    
    Transform(const glm::vec3& pos = glm::vec3(0.0f),
              const glm::quat& rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
              const glm::vec3& scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}
    
    glm::mat4 GetMatrix() const {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 r = glm::mat4_cast(rotation);
        glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
        return t * r * s;
    }
    
    glm::vec3 GetForward() const {
        return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }
    
    glm::vec3 GetRight() const {
        return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    glm::vec3 GetUp() const {
        return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }
};

}

#endif