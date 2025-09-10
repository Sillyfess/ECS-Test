#ifndef ECS_RENDERABLE_H
#define ECS_RENDERABLE_H

#include <glm/glm.hpp>
#include "../Component.h"

namespace ECS {

enum class MeshType {
    Cube,
    Sphere,
    Plane,
    Custom
};

class Renderable : public Component {
public:
    MeshType meshType;
    glm::vec3 color;
    bool visible;
    float opacity;
    
    Renderable(MeshType type = MeshType::Cube,
               const glm::vec3& col = glm::vec3(1.0f),
               bool vis = true,
               float op = 1.0f)
        : meshType(type), color(col), visible(vis), opacity(op) {}
};

}

#endif