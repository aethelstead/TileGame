#include <sstream>
#include <tmx.h>

#include "../Tiled.h"
#include "../../Debug.h"

using namespace Gin::Tiled;

static void LoadTilesetAnimations(const tmx_tileset* pTmxTileset, Tileset& tileset)
{
    for (size_t i = 0; i < pTmxTileset->tilecount; ++i)
    {
        const auto& tile = pTmxTileset->tiles[i];
        if (!tile.animation)
            continue;

        Animation animation;
        for (size_t j = 0; j < tile.animation_len; ++j)
        {
            const auto& anim = tile.animation[j];

            AnimationFrame frame;
            frame.id = anim.tile_id;
            frame.duration = anim.duration;

            animation.frames.emplace_back(frame);
        }
        tileset.animationsMap.try_emplace(tile.id, animation);
    }
}

static void LoadTilesetCollisionBoxes(const tmx_tileset* pTmxTileset, Tileset& tileset)
{
    for (size_t i = 0; i < pTmxTileset->tilecount; ++i)
    {
        const auto& tile = pTmxTileset->tiles[i];
        if (!tile.collision)
            continue;

        std::vector<Rectf> boxes;
        auto pObjNode = tile.collision;
        while (pObjNode)
        {
            Rectf box(pObjNode->x, pObjNode->y, pObjNode->width, pObjNode->height);
            boxes.emplace_back(box);

            pObjNode = pObjNode->next;
        }
    
        tileset.boxesMap.try_emplace(tile.id, boxes);
    }
}

static bool LoadTileset(const tmx_tileset* pTmxTileset, Tileset& tileset)
{
    if (!pTmxTileset)
        return false;

    // This will be set later on after assets have loaded
    tileset.textureId = 0;

    tileset.nTiles = pTmxTileset->tilecount;
    // This assumes there is no margin/ offset, or spacing between the cells of the Tileset
    // tmx lib doesn't seem to have a 'tilesPerRow' property
    tileset.tilesPerRow = (pTmxTileset->image->width / pTmxTileset->tile_width);
    tileset.tileWidth = pTmxTileset->tile_width;
    tileset.tileHeight = pTmxTileset->tile_height;
    tileset.texturePath = pTmxTileset->image->source;

    LoadTilesetAnimations(pTmxTileset, tileset);
    LoadTilesetCollisionBoxes(pTmxTileset, tileset);

    return true;
}

static bool LoadTilesets(const tmx_map* pTmxMap, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap, std::vector<Object>& objects)
{
    auto pTsxListNode = pTmxMap->ts_head;
    while (pTsxListNode)
    {
        Tileset tileset;
        tileset.firstTiledId = pTsxListNode->firstgid;

        if(LoadTileset(pTsxListNode->tileset, tileset))
            tilesetMap.try_emplace(tileset.firstTiledId, tileset);

        pTsxListNode = pTsxListNode->next;
    }

    return true;
}

static void tmx_add_property(tmx_property *property, void *userdata)
{
    auto pPropsMap = reinterpret_cast<std::map<std::string, std::string>*>(userdata);

    // @TODO: Support more than just string properties. Means more parsing/ loading overhead handled here and not when loading entities
    pPropsMap->try_emplace(property->name, property->value.string);
}

static bool LoadTileLayer(const tmx_layer* pTmxLayer, const size_t& layerIdx, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap)
{
    if (!pTmxLayer || pTmxLayer->type != L_LAYER)
        return false;

    auto pTileGids = pTmxLayer->content.gids;

    TileLayer layer;
    layer.zIdx = layerIdx;

    size_t nTiles = (tilemap.mapWidth * tilemap.mapHeight);
    layer.tiles.reserve(nTiles);
                
    for (size_t y = 0; y < tilemap.mapHeight; ++y)
    {
        for (size_t x = 0; x < tilemap.mapWidth; ++x)
        {
            size_t tileIdx = (y * tilemap.mapWidth) + x;
            int tileId = (pTileGids[tileIdx] & TMX_FLIP_BITS_REMOVAL) - 1;
            uint64_t tilesetId = 0;
            uint64_t gid = 0;
            if (tileId >= 0)
            {
                for (const auto& [id, tileset] : tilesetMap)
                {
                    gid = tileId + tileset.firstTiledId;
                    if (gid >= tileset.firstTiledId)
                        tilesetId = tileset.firstTiledId;
                        break;
                }
            }

            layer.tiles.emplace_back(Tile(tileId, gid, tilesetId));
        }
    }

    tilemap.layers.emplace_back(layer);

    return true;
}

static bool LoadObjectLayer(const tmx_layer* pTmxLayer, const size_t& layerIdx, std::map<uint64_t, Tileset>& tilesetMap, std::vector<Object>& objects)
{
    if (!pTmxLayer || pTmxLayer->type != L_OBJGR)
        return false;

    auto pObjNode = pTmxLayer->content.objgr->head;
    while (pObjNode)
    {
        Object object;
        object.pos.x = pObjNode->x;
        object.pos.y = pObjNode->y;
        object.width = pObjNode->width;
        object.height = pObjNode->height;
        object.tilesetId = pObjNode->content.gid;
        object.zIdx = layerIdx;
                    
        tmx_property_foreach(pObjNode->properties, &tmx_add_property, &object.props);

        objects.emplace_back(object);

        pObjNode = pObjNode->next;
    }

    return true;
}

static bool LoadTmxLayers(const tmx_map* pTmxMap, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap, std::vector<Object>& objects)
{
    size_t layerIdx = 0;
    auto pLayerNode = pTmxMap->ly_head;
    while (pLayerNode)
    {
        if (pLayerNode->type == L_LAYER)
        {
            LoadTileLayer(pLayerNode, layerIdx, tilemap, tilesetMap);
        }
        else if (pLayerNode->type == L_OBJGR)
        {
            LoadObjectLayer(pLayerNode, layerIdx, tilesetMap, objects);
        }
        else
        {
            LOGWARN("Gin::Tiled::LoadTiledMapFromFile() - Layer type not supported.");
        }

        ++layerIdx;
        pLayerNode = pLayerNode->next;
    }

    return true;
}

static bool LoadTmxMap(const tmx_map* pTmxMap, Tilemap& tilemap, std::map<uint64_t, Tileset>& tilesetMap, std::vector<Object>& objects)
{
    tilemap.mapWidth = pTmxMap->width;
    tilemap.mapHeight =  pTmxMap->height;
    tilemap.tileWidth = pTmxMap->tile_width;
    tilemap.tileHeight = pTmxMap->tile_height;

    LoadTilesets(pTmxMap, tilemap, tilesetMap, objects);
    LoadTmxLayers(pTmxMap, tilemap, tilesetMap, objects);

    return true;
}

bool Gin::Tiled::LoadTiledMapFromFile(const std::string& tmxPath, Tiled::Map& theMap)
{
    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - Using 'libtmx'");

    tmx_map* pTmxMap = tmx_load(tmxPath.c_str());
    if (!pTmxMap)
        return false;

    if (!LoadTmxMap(pTmxMap, theMap.tilemap, theMap.tilesetMap, theMap.objects))
        return false;

    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - TMX Map at: '" << tmxPath << "' was loaded successfully.");
    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - nTileLayers: " << theMap.tilemap.layers.size());
    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - nTilesets: " << theMap.tilesetMap.size());
    LOGDEBUG("Gin::Tiled::LoadTiledMapFromFile() - nObjects: " << theMap.objects.size());

    return true;
}
