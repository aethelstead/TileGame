#include <algorithm>

#include "Debug.h"
#include "GameRenderer.h"

using namespace Gin;

void GameRenderer::Render(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap)
{
    uint playerZIdx = state.m_pPlayer->zIdx;

    const auto& viewTiles = state.WorldTilesInView();
    std::vector<WorldTile> baseTiles;
    std::vector<WorldTile> topTiles;

    for (const auto& viewTile : viewTiles)
    {
        if (viewTile.zIdx < playerZIdx)
            baseTiles.emplace_back(viewTile);
        else if (viewTile.zIdx > playerZIdx)
            topTiles.emplace_back(viewTile);
    }

    RenderViewTiles(pRenderer, state, textureMap, baseTiles);
    RenderEntities(pRenderer, state, textureMap);
    RenderViewTiles(pRenderer, state, textureMap, topTiles);
}

void GameRenderer::RenderViewTiles(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap, const std::vector<WorldTile>& viewTiles)
{
    for (const auto& viewTile : viewTiles)
    {
        const auto& tileset = state.m_tilesetMap.at(viewTile.tilesetId);
        auto& pTexture = textureMap.at(tileset.textureId);

        Recti src;
        src.x = ((viewTile.id + viewTile.frameIdx) % tileset.tilesPerRow) * tileset.tileWidth;
        src.y = ((viewTile.id + viewTile.frameIdx) / (tileset.nTiles / tileset.tilesPerRow)) * tileset.tileHeight;
        src.w = tileset.tileWidth;
        src.h = tileset.tileHeight;

        Recti dest;
        dest.x = static_cast<int>(viewTile.box.x - state.camera.pos.x);
        dest.y = static_cast<int>(viewTile.box.y - state.camera.pos.y);
        dest.w = static_cast<int>(viewTile.box.w);
        dest.h = static_cast<int>(viewTile.box.h);
    
        pRenderer->Copy(pTexture, src, dest);
    }
}

void GameRenderer::RenderEntity(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const std::shared_ptr<Entity>& pEntity, const TextureMap& textureMap)
{
    if (pEntity->type == EntityType::Trigger)
    {
        Colour4i c;
        c.m_r = 0xff;
        c.m_a = 0xaa;
        Recti dest = { 
            static_cast<int>(pEntity->pos.x - state.camera.pos.x),
            static_cast<int>(pEntity->pos.y - state.camera.pos.y),
            static_cast<int>(pEntity->width),
            static_cast<int>(pEntity->height)
        };
        pRenderer->FillRect(dest, c);
    }

    if (pEntity->tilesetId == 0)
        return;

    const auto& tileset = state.m_tilesetMap.at(pEntity->tilesetId);
    
    Recti src(
        (pEntity->frameIdx % tileset.tilesPerRow) * tileset.tileWidth,
        (pEntity->frameIdx / (tileset.nTiles / tileset.tilesPerRow)) * tileset.tileHeight,
        tileset.tileWidth,
        tileset.tileHeight
    );
    Recti dest = { 
        static_cast<int>(pEntity->pos.x - state.camera.pos.x),
        static_cast<int>(pEntity->pos.y - state.camera.pos.y),
        static_cast<int>(pEntity->width),
        static_cast<int>(pEntity->height)
    };

    const auto& pTexture = textureMap.at(tileset.textureId);
    if (!pTexture)
    {
        LOGERROR("Failed texture lookup!");
        return;
    }

    pRenderer->Copy(pTexture, src, dest);
}

void GameRenderer::RenderEntities(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap)
{
    for (auto& pEntity : state.m_entities)
    {
        RenderEntity(pRenderer, state, pEntity, textureMap);
    }
}
