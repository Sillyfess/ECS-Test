#ifndef ECS_RENDER_SYSTEM_H
#define ECS_RENDER_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include "../System.h"
#include "../Components/Transform.h"
#include "../Components/Renderable.h"
#include "../../CubeRenderer.h"
#include "../../Shader.h"

namespace ECS {

class RenderSystem : public System {
public:
    RenderSystem(CubeRenderer* renderer, Shader* shader) 
        : cubeRenderer(renderer), shader(shader) {
        RequireComponents<Transform, Renderable>();
        SetPriority(100); // Run last
    }
    
    void Update(float deltaTime) override {
        if (!cubeRenderer || !shader) return;
        
        std::bitset<MAX_COMPONENTS> mask;
        mask.set(Component::GetTypeId<Transform>());
        mask.set(Component::GetTypeId<Renderable>());
        auto entities = world->GetEntitiesWithComponents(mask);
        
        std::vector<glm::mat4> modelMatrices;
        std::vector<glm::vec3> colors;
        
        for (auto& entity : entities) {
            auto* transform = world->GetComponent<Transform>(entity);
            auto* renderable = world->GetComponent<Renderable>(entity);
            
            if (!transform || !renderable || !renderable->visible) continue;
            
            if (renderable->meshType == MeshType::Cube) {
                modelMatrices.push_back(transform->GetMatrix());
                colors.push_back(renderable->color);
            }
        }
        
        // Store for actual rendering in render phase
        cachedMatrices = modelMatrices;
        cachedColors = colors;
    }
    
    void Render() {
        if (!cubeRenderer || !shader || cachedMatrices.empty()) return;
        
        shader->use();
        
        // Set object color uniform for each cube (simplified approach)
        // In a real implementation, you'd use instanced rendering with color attributes
        for (size_t i = 0; i < cachedMatrices.size(); ++i) {
            shader->setVec3("objectColor", cachedColors[i]);
        }
        
        cubeRenderer->render(*shader, cachedMatrices);
    }
    
    const std::vector<glm::mat4>& GetCachedMatrices() const { 
        return cachedMatrices; 
    }
    
private:
    CubeRenderer* cubeRenderer;
    Shader* shader;
    std::vector<glm::mat4> cachedMatrices;
    std::vector<glm::vec3> cachedColors;
};

}

#endif