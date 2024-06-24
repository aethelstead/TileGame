#ifndef TILED_OBJECT_H
#define TILED_OBJECT_H

#include <cstdint>
#include <string>
#include <map>

#include "../Maths/Vector2.h"

namespace Gin::Tiled
{
    struct Object
    {
        Vector2f pos;
        uint64_t width = 0;
        uint64_t height = 0;
        uint64_t zIdx = 0;
        uint64_t tilesetId = 0;
        std::map<std::string, std::string> props;
    };
}

#endif
