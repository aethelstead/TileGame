#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdint>

namespace Gin
{
    // Enum representing one of the four cardinal directions: North, East, South, West
    enum class Direction : uint
    {
        None = 0,
        North,
        East,
        South,
        West
    };
}

#endif
