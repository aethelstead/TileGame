#ifndef ENTITY_TYPE_H
#define ENTITY_TYPE_H

#include <cstdint>
#include <string>

namespace Gin
{
    enum class EntityType : uint
    {
        None = 0,
        Player,
        NPC,
        Trigger
    };

    EntityType EntityTypeFromString(std::string name);
}

#endif
