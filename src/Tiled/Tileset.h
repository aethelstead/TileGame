#ifndef TILESET_H
#define TILESET_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "../Maths/Rect.h"

namespace Gin::Tiled
{
    struct AnimationFrame
    {
        uint id = 0;
        uint duration = 0;
    };

    struct Animation
    {
        uint currFrameIdx = 0;
        uint nextFrameMs = 0;
        std::vector<AnimationFrame> frames;
    };

    struct Tileset
    {
        uint64_t textureId;
        std::string texturePath;
        uint64_t firstTiledId;
        uint64_t nTiles;
        uint64_t tilesPerRow;
        uint64_t tileWidth;
        uint64_t tileHeight;
        std::map<uint64_t, Animation> animationsMap;
        std::map<uint64_t, std::vector<Rectf>> boxesMap;
    };
}

#endif
