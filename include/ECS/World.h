#ifndef ECS_WORLD_H
#define ECS_WORLD_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <algorithm>
#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace ECS {

class World {
public:
    World() : nextEntityId(1) {}
    ~World() = default;
    
    std::shared_ptr<Entity> CreateEntity() {
        auto entity = std::make_shared<Entity>(nextEntityId++);
        entities.push_back(entity);
        return entity;
    }
    
    void DestroyEntity(std::shared_ptr<Entity> entity) {
        if (!entity) return;
        
        entity->SetActive(false);
        
        for (auto& [typeId, componentMap] : components) {
            componentMap.erase(entity->GetId());
        }
        
        entities.erase(
            std::remove(entities.begin(), entities.end(), entity),
            entities.end()
        );
    }
    
    template<typename T, typename... Args>
    T* AddComponent(std::shared_ptr<Entity> entity, Args&&... args) {
        if (!entity || !entity->IsActive()) return nullptr;
        
        uint32_t typeId = Component::GetTypeId<T>();
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        
        components[typeId][entity->GetId()] = std::move(component);
        entity->AddComponentType(typeId);
        
        return ptr;
    }
    
    template<typename T>
    T* GetComponent(std::shared_ptr<Entity> entity) {
        if (!entity || !entity->IsActive()) return nullptr;
        
        uint32_t typeId = Component::GetTypeId<T>();
        auto it = components.find(typeId);
        if (it == components.end()) return nullptr;
        
        auto compIt = it->second.find(entity->GetId());
        if (compIt == it->second.end()) return nullptr;
        
        return static_cast<T*>(compIt->second.get());
    }
    
    template<typename T>
    void RemoveComponent(std::shared_ptr<Entity> entity) {
        if (!entity) return;
        
        uint32_t typeId = Component::GetTypeId<T>();
        auto it = components.find(typeId);
        if (it != components.end()) {
            it->second.erase(entity->GetId());
            entity->RemoveComponentType(typeId);
        }
    }
    
    template<typename T>
    bool HasComponent(std::shared_ptr<Entity> entity) {
        if (!entity || !entity->IsActive()) return false;
        return entity->HasComponentType(Component::GetTypeId<T>());
    }
    
    void AddSystem(std::unique_ptr<System> system) {
        system->SetWorld(this);
        systems.push_back(std::move(system));
        
        std::sort(systems.begin(), systems.end(),
            [](const auto& a, const auto& b) {
                return a->GetPriority() < b->GetPriority();
            });
    }
    
    template<typename T>
    T* GetSystem() {
        for (auto& system : systems) {
            if (auto* s = dynamic_cast<T*>(system.get())) {
                return s;
            }
        }
        return nullptr;
    }
    
    void Update(float deltaTime) {
        for (auto& system : systems) {
            if (system->IsEnabled()) {
                system->PreUpdate(deltaTime);
                system->Update(deltaTime);
                system->PostUpdate(deltaTime);
            }
        }
    }
    
    std::vector<std::shared_ptr<Entity>> GetEntitiesWithComponents(
        const std::bitset<MAX_COMPONENTS>& componentMask) {
        std::vector<std::shared_ptr<Entity>> result;
        
        for (auto& entity : entities) {
            if (entity->IsActive() && 
                (entity->GetComponentMask() & componentMask) == componentMask) {
                result.push_back(entity);
            }
        }
        
        return result;
    }
    
    const std::vector<std::shared_ptr<Entity>>& GetAllEntities() const {
        return entities;
    }
    
    void Clear() {
        entities.clear();
        components.clear();
        systems.clear();
        nextEntityId = 1;
    }
    
private:
    uint32_t nextEntityId;
    std::vector<std::shared_ptr<Entity>> entities;
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::unique_ptr<Component>>> components;
    std::vector<std::unique_ptr<System>> systems;
};

}

#endif