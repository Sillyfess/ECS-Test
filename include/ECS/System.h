#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <vector>
#include <bitset>
#include <memory>
#include "Entity.h"
#include "Component.h"

namespace ECS {

class World;

class System {
public:
    System() : priority(0), enabled(true) {}
    virtual ~System() = default;
    
    virtual void Update(float deltaTime) = 0;
    virtual void PreUpdate(float deltaTime) {}
    virtual void PostUpdate(float deltaTime) {}
    
    void SetWorld(World* world) { this->world = world; }
    World* GetWorld() const { return world; }
    
    void SetEnabled(bool enable) { enabled = enable; }
    bool IsEnabled() const { return enabled; }
    
    void SetPriority(int p) { priority = p; }
    int GetPriority() const { return priority; }
    
    bool MatchesEntity(const Entity& entity) const {
        if (!enabled || requiredComponents.none()) {
            return false;
        }
        return (entity.GetComponentMask() & requiredComponents) == requiredComponents;
    }
    
protected:
    void SetRequiredComponents(const std::bitset<MAX_COMPONENTS>& mask) {
        requiredComponents = mask;
    }
    
    template<typename... Components>
    void RequireComponents() {
        requiredComponents.reset();
        (requiredComponents.set(Component::GetTypeId<Components>()), ...);
    }
    
    World* world = nullptr;
    std::bitset<MAX_COMPONENTS> requiredComponents;
    int priority;
    bool enabled;
};

}

#endif