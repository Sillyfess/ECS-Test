#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <cstdint>
#include <bitset>

namespace ECS {

constexpr size_t MAX_COMPONENTS = 64;

class Entity {
public:
    Entity(uint32_t id) : id(id), active(true) {}
    
    uint32_t GetId() const { return id; }
    bool IsActive() const { return active; }
    void SetActive(bool isActive) { active = isActive; }
    
    void AddComponentType(uint32_t typeId) {
        componentMask.set(typeId);
    }
    
    void RemoveComponentType(uint32_t typeId) {
        componentMask.reset(typeId);
    }
    
    bool HasComponentType(uint32_t typeId) const {
        return componentMask.test(typeId);
    }
    
    const std::bitset<MAX_COMPONENTS>& GetComponentMask() const {
        return componentMask;
    }
    
private:
    uint32_t id;
    bool active;
    std::bitset<MAX_COMPONENTS> componentMask;
};

}

#endif