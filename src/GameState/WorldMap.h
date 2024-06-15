#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "Tilemaps/Tilemap.h"
#include "../GameState/Entities/EntityProto.h"
#include "../Maths/Rect.h"

namespace Gin
{
    class WorldMap
    {
    public:
        static bool LoadFromTmx(const std::string& tmxPath, WorldMap& worldMap);

    //private:
        Tilemap m_tilemap;
        std::map<uint64_t, Tileset> m_tilesetMap;
        std::vector<EntityProto> m_entityProtos;

    public:
    
        WorldMap()
        {}

        Rectf Bounds() const
        {
            return Rectf(
                0, 0, 
                static_cast<double>(m_tilemap.mapWidth * m_tilemap.m_tileWidth), 
                static_cast<double>(m_tilemap.mapHeight * m_tilemap.m_tileHeight)
            );
        }

    };
}

#endif
