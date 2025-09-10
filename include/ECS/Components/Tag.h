#ifndef ECS_TAG_H
#define ECS_TAG_H

#include <string>
#include "../Component.h"

namespace ECS {

class Tag : public Component {
public:
    std::string name;
    
    Tag(const std::string& tagName = "") : name(tagName) {}
};

}

#endif