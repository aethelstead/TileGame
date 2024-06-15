#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <memory>

#include "Assets.h"
#include "VGui/VGui.h"
#include "GameState/GameState.h"
#include "Rendering/Renderer.h"

namespace Gin
{
    class GameRenderer
    {
    public:
        void Render(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap);

    private:
        void RenderViewTiles(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap, const std::vector<WorldTile>& viewTiles);
        //void RenderTilemap(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap, uint baseLayerIdx = 0, uint topLayerIdx = UINT32_MAX);

        void RenderEntity(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const std::shared_ptr<Entity>& pEntity, const TextureMap& textureMap);

        void RenderEntities(const std::unique_ptr<Renderer>& pRenderer, GameState& state, const TextureMap& textureMap);
    };
}

#endif
