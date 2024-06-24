#ifndef TILELAYER_H
#define TILELAYER_H

#include <vector>

#include "Tile.h"

namespace Gin::Tiled
{
    struct TileLayer
    {
        int zIdx = -1;
        std::vector<Tile> tiles;
    };
}

#endif
