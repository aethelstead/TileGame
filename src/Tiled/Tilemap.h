#ifndef TILEMAP_H
#define TILEMAP_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Tile.h"
#include "TileLayer.h"
#include "Tileset.h"

namespace Gin::Tiled
{
    struct Tilemap
    {
        uint64_t mapWidth;
        uint64_t mapHeight;
        uint64_t tileWidth;
        uint64_t tileHeight;
        std::vector<TileLayer> layers;
    };
}

#endif
