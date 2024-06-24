#include <Tmx.h>

#include "../Tiled.h"
#include "../../Debug.h"

#ifndef USE_NEW_TILED_LIB

using namespace Gin::Tiled;

// @TODO: Duplicate, move somewhere else
static std::string GetPropertyValue(const std::map<std::string, std::string>& propertyMap, const std::string& key)
{
    auto s = std::string();

    auto it = propertyMap.find(key);
    bool foundIt = (it != propertyMap.end());
    if (foundIt)
        s = it->second;

    return s;
}

static bool LoadTileset(const Tmx::Tileset* tmxTileset, Tileset& tileset)
{
    if (!tmxTileset)
        return false;

    // This will be set later on after assets have loaded
    tileset.textureId = 0;

    tileset.firstTiledId = tmxTileset->GetFirstGid();
    tileset.nTiles = tmxTileset->GetTileCount();
    tileset.tilesPerRow = tmxTileset->GetColumns();
    tileset.tileWidth = tmxTileset->GetTileWidth();
    tileset.tileHeight = tmxTileset->GetTileHeight();
    tileset.texturePath = tmxTileset->GetImage()->GetSource();

    LOGDEBUG(tmxTileset->GetName() << " - firstTileId = " << tileset.firstTiledId);

    // Load animations
    for (const auto& pTsxTile : tmxTileset->GetTiles())
    {
        if (pTsxTile->GetFrames().empty())
            continue;

        Animation animation;
        for (const auto& tsxFrame : pTsxTile->GetFrames())
        {
            AnimationFrame frame;
            frame.id = tsxFrame.GetTileID();
            frame.duration = tsxFrame.GetDuration();

            animation.frames.emplace_back(frame);
        }
        tileset.animationsMap.try_emplace(pTsxTile->GetId(), animation);
    }

    // Load collision boxes
    for (const auto& pTsxTile : tmxTileset->GetTiles())
    {
        auto pObjGroup = pTsxTile->GetObjectGroup();
        if (!pObjGroup)
            continue;
            
        std::vector<Rectf> boxes;
        for (const auto& pObj : pObjGroup->GetObjects())
        {
            Rectf box(pObj->GetX(), pObj->GetY(), pObj->GetWidth(), pObj->GetHeight());
            boxes.emplace_back(box);
        }
        tileset.boxesMap.try_emplace(pTsxTile->GetId(), boxes);
    }

    return true;
}

static bool LoadTmxMap(const Tmx::Map& tmxMap, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap, std::vector<Object>& objects)
{
    //tilemap.name = tmxMap.GetFilename();
    tilemap.mapWidth = tmxMap.GetWidth();
    tilemap.mapHeight = tmxMap.GetHeight();
    tilemap.tileWidth = tmxMap.GetTileWidth();
    tilemap.tileHeight = tmxMap.GetTileHeight();

    // Load all tilesets
    for (const auto pTmxTileset : tmxMap.GetTilesets())
    {
        Tileset tileset;
        if (!LoadTileset(pTmxTileset, tileset))
            continue;

        tilesetMap.try_emplace(tileset.firstTiledId, tileset);
    }

    // Load all tile layers
    uint64_t nTiles = tilemap.mapWidth * tilemap.mapHeight;
    int firstLayerZIdx = tmxMap.GetTileLayer(0)->GetParseOrder();
    for (const auto pLayer : tmxMap.GetTileLayers())
    {
        TileLayer layer;
        layer.zIdx = pLayer->GetParseOrder() - firstLayerZIdx;
        layer.tiles.reserve(nTiles);

        for (uint64_t tileIdx = 0; tileIdx < nTiles; ++tileIdx)
        {
            auto tmxTile = pLayer->GetTile(tileIdx);
            auto tilesetId = 0;
            for (const auto& tileset : tilesetMap)
            {
                if (tmxTile.gid >= tileset.second.firstTiledId)
                    tilesetId = tileset.second.firstTiledId;
            }
            
            layer.tiles.push_back(Tile(tmxTile.id, tmxTile.gid, tilesetId));
        }

        tilemap.layers.push_back(layer);
    }

    // Get the z-index of the object layer
    int objLayerZIdx = -1;
    for (uint i = 0; i < tilemap.layers.size(); i++)
    {
        int currZ = tilemap.layers[i].zIdx;
        int nextZ = tilemap.layers[i + 1].zIdx;
        if (currZ != nextZ - 1)
        {
            objLayerZIdx = currZ + 1;
            break;
        }
    }
    LOGDEBUG("Obj layer zIdx = " << objLayerZIdx);

    if (objLayerZIdx < 0)
    {
        LOGERROR("WorldMap::LoadFromTmx() - Object layer not found.");
        return false;
    }

    // Load objects
    for (const auto& pObjGroup : tmxMap.GetObjectGroups())
    {
        if (!pObjGroup)
            continue;

        for (const auto& pObj : pObjGroup->GetObjects())
        {
            if (!pObj)
                continue;

            Object object;
            object.pos.x = static_cast<double>(pObj->GetX());
            object.pos.y = static_cast<double>(pObj->GetY());
            object.width = pObj->GetWidth();
            object.height = pObj->GetHeight();
            object.tilesetId = pObj->GetGid();
            object.zIdx = objLayerZIdx;

            object.props = pObj->GetProperties().GetList();

            objects.emplace_back(object);
        }
    }

    return true;
}

bool Gin::Tiled::LoadTiledMapFromFile(const std::string& tmxPath, Tiled::Map& theMap)
{
    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - Using 'tmxparser'.");

    auto tmxMap = Tmx::Map();

    tmxMap.ParseFile(tmxPath);
    if (tmxMap.HasError())
        return false;

    if (!LoadTmxMap(tmxMap, theMap.tilemap, theMap.tilesetMap, theMap.objects))
        return false;

    return true;
}

#endif
