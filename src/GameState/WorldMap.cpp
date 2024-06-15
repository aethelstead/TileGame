#include <Tmx.h>

#include "WorldMap.h"
#include "../Debug.h"

using namespace Gin;

static std::string GetPropertyValue(const std::map<std::string, std::string>& propertyMap, const std::string& key)
{
    auto s = std::string();

    if (propertyMap.empty() || key.empty())
        return s;

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

    for (const auto& pTsxTile : tmxTileset->GetTiles())
    {
        /*
        auto pObjGroup = pTsxTile->GetObjectGroup();
        if (!pObjGroup)
            continue;*/

        if (!pTsxTile->GetFrames().empty())
        {
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
    }

    return true;
}

static bool LoadTmxMap(const Tmx::Map& tmxMap, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap, std::vector<EntityProto>& entityProtos)
{
    //tilemap.name = tmxMap.GetFilename();
    tilemap.mapWidth = tmxMap.GetWidth();
    tilemap.mapHeight = tmxMap.GetHeight();
    tilemap.m_tileWidth = tmxMap.GetTileWidth();
    tilemap.m_tileHeight = tmxMap.GetTileHeight();

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

        tilemap.m_layers.push_back(layer);
    }

    // Get the z-index of the object layer
    int objLayerZIdx = -1;
    for (uint i = 0; i < tilemap.m_layers.size(); i++)
    {
        int currZ = tilemap.m_layers[i].zIdx;
        int nextZ = tilemap.m_layers[i + 1].zIdx;
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

    // Load entity prototypes
    for (const auto& pObjGroup : tmxMap.GetObjectGroups())
    {
        if (!pObjGroup)
            continue;

        for (const auto& pObj : pObjGroup->GetObjects())
        {
            if (!pObj)
                continue;

            EntityProto proto;
            proto.pos.x = static_cast<double>(pObj->GetX());
            proto.pos.y = static_cast<double>(pObj->GetY());
            proto.width = pObj->GetWidth();
            proto.height = pObj->GetHeight();
            proto.tilesetId = pObj->GetGid();
            proto.zIdx = objLayerZIdx;

            proto.props = pObj->GetProperties().GetList();

            auto typePropVal = GetPropertyValue(proto.props, "type");
            if (typePropVal.empty())
            {
                LOGWARN("WorldMap::LoadFromTmx() - Map object did not have an associated 'type' property. Object will not be added to the WorldMap.");
                continue;
            }

            proto.type = EntityTypeFromString(typePropVal);
            if (proto.type == EntityType::None)
            {
                LOGWARN("WorldMap::LoadFromTmx() - Map object did not have a recognised EntityType. Object will not be added to the WorldMap.");
                continue;
            }

            auto archetypePropVal = GetPropertyValue(proto.props, "archetype");
            if (archetypePropVal.empty())
            {
                LOGWARN("WorldMap::LoadFromTmx() - Map object did not have an associated 'archetype' property. Object will not be added to the WorldMap.");
                continue;
            }

            proto.archetypeName = archetypePropVal;

            entityProtos.emplace_back(proto);
        }
    }

    return true;
}

bool WorldMap::LoadFromTmx(const std::string& tmxPath, WorldMap& worldMap)
{
    auto tmxMap = Tmx::Map();

    tmxMap.ParseFile(tmxPath);
    if (tmxMap.HasError())
        return false;

    if (!LoadTmxMap(tmxMap, worldMap.m_tilemap, worldMap.m_tilesetMap, worldMap.m_entityProtos))
        return false;

    return true;
}
