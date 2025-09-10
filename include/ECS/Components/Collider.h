#ifndef ECS_COLLIDER_H
#define ECS_COLLIDER_H

#include <glm/glm.hpp>
#include "../Component.h"

namespace ECS {

enum class ColliderType {
    Box,
    Sphere,
    Capsule,
    Plane,
    Mesh
};

class Collider : public Component {
public:
    ColliderType type;
    glm::vec3 size;
    glm::vec3 offset;
    float radius;
    float height;
    
    Collider(ColliderType t = ColliderType::Box, const glm::vec3& s = glm::vec3(1.0f))
        : type(t), size(s), offset(0.0f), radius(0.5f), height(1.0f) {}
    
    static Collider Box(const glm::vec3& size, const glm::vec3& offset = glm::vec3(0.0f)) {
        Collider c(ColliderType::Box, size);
        c.offset = offset;
        return c;
    }
    
    static Collider Sphere(float radius, const glm::vec3& offset = glm::vec3(0.0f)) {
        Collider c(ColliderType::Sphere);
        c.radius = radius;
        c.offset = offset;
        return c;
    }
    
    static Collider Capsule(float radius, float height, const glm::vec3& offset = glm::vec3(0.0f)) {
        Collider c(ColliderType::Capsule);
        c.radius = radius;
        c.height = height;
        c.offset = offset;
        return c;
    }
    
    static Collider Plane(const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f)) {
        Collider c(ColliderType::Plane);
        c.size = normal;
        return c;
    }
};

}

#endif