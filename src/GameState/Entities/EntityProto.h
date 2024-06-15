#ifndef ENTITY_PROTO
#define ENTITY_PROTO

#include <cstdint>
#include <string>
#include <map>

#include "EntityType.h"
#include "../../Maths/Vector2.h"

namespace Gin
{
    struct EntityProto
    {
        Vector2f pos;
        uint64_t width = 0;
        uint64_t height = 0;
        uint64_t zIdx = 0;
        uint64_t tilesetId = 0;
        EntityType type = EntityType::None;
        std::string archetypeName;
        std::map<std::string, std::string> props;
    };
}

#endif
