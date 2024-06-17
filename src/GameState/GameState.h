#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <cstdint>
#include <vector>

#include "Camera.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/NPC.h"
#include "Entities/Trigger.h"
#include "WorldMap.h"

namespace Gin
{
    struct WorldTile : Tile
    {
        Rectf box;
        uint zIdx = 0;
        uint frameIdx = 0;

        std::vector<Rectf> boxes;

        WorldTile()
        {}

        WorldTile(Tile tile)
            : Tile( tile )
        {}
    };

    struct GameState
    {
        GameState()
        {}

        void Init(uint64_t viewWidth, uint64_t viewHeight);

        void ChangeMap(const WorldMap& worldMap, const Vector2f* pSpawnPos = nullptr);

        void Update(double dt, bool& mapChange, std::string& nextMapName, Vector2f& spawnPos);

        void PlayerWalk(const Direction& direction);
        void PlayerStopWalk(const Direction& direction);
        void PlayerAttack();

        std::vector<WorldTile>& WorldTilesInView() { return m_worldTilesInView; };

        void UpdateWorldTilesInView();

        Camera camera;
        WorldMap worldMap;

        std::vector<WorldTile> m_worldTilesInView;
        
        std::vector<std::shared_ptr<Entity>> m_entities;
        Player* m_pPlayer;
        std::vector<std::shared_ptr<NPC>> m_npcs;
        std::vector<std::shared_ptr<Trigger>> m_triggers;
    };
}

#endif
