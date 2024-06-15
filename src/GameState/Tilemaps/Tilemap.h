#ifndef TILEMAP_H
#define TILEMAP_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Tile.h"
#include "TileLayer.h"
#include "Tileset.h"

namespace Gin
{
    struct Tilemap
    {
        uint64_t mapWidth;
        uint64_t mapHeight;
        uint64_t m_tileWidth;
        uint64_t m_tileHeight;
        std::vector<TileLayer> m_layers;
    };
}

#endif
