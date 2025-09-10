#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include <cstdint>
#include <typeindex>
#include <typeinfo>

namespace ECS {

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
    
    template<typename T>
    static uint32_t GetTypeId() {
        static uint32_t typeId = nextTypeId++;
        return typeId;
    }
    
private:
    static uint32_t nextTypeId;
};

}

#endif