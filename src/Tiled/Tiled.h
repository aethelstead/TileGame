#ifndef TILED_H
#define TILED_H

#include <vector>
#include <map>

#include "Tile.h"
#include "Tileset.h"
#include "TileLayer.h"
#include "Tilemap.h"
#include "Object.h"

namespace Gin::Tiled
{
    struct Map
    {
        Map()
        {}

        Tiled::Tilemap tilemap;
        std::map<uint64_t, Tiled::Tileset> tilesetMap;
        std::vector<Tiled::Object> objects;
    };

    bool LoadTiledMapFromFile(const std::string& tmxPath, Tiled::Map& theMap);
}

#endif
