#ifndef ECS_VELOCITY_H
#define ECS_VELOCITY_H

#include <glm/glm.hpp>
#include "../Component.h"

namespace ECS {

class Velocity : public Component {
public:
    glm::vec3 linear;
    glm::vec3 angular;
    
    Velocity(const glm::vec3& lin = glm::vec3(0.0f),
             const glm::vec3& ang = glm::vec3(0.0f))
        : linear(lin), angular(ang) {}
};

}

#endif