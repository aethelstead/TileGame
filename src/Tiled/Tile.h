#ifndef TILE_H
#define TILE_H

#include <cstdint>

namespace Gin::Tiled
{
    struct Tile
    {
        uint64_t id = 0;
        uint64_t gid = 0;
        uint64_t tilesetId = 0;

        Tile()
        {}

        Tile (uint64_t id, uint64_t gid, uint64_t tilesetId)
            : id( id )
            , gid( gid )
            , tilesetId( tilesetId )
        {}
    };
}

#endif
