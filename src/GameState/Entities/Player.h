#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

namespace Gin
{
    enum class PlayerAnimationType : uint
    {
        IdleSouth = 0,
        IdleEast = 3,
        IdleWest = 4,
        IdleNorth = 5,

        WalkEast = 6,
        WalkWest = 8,
        WalkNorth = 10,
        WalkSouth = 29,

        AttackSouth = 13,
        AttackEast = 20,
        AttackWest = 25,
        AttackNorth = 22
    };

    struct Player : public Entity
    {
        bool attack = false;
        bool ready = false;

        Player()
        {}

        Player(UniqueID uid, uint64_t tilesetId, Vector2f pos, uint64_t width, uint64_t height, uint zIdx)
            : Entity( uid, tilesetId, pos, width, height, zIdx )
        {
            type = EntityType::Player;
            speed = 3;
            animId = static_cast<uint>(PlayerAnimationType::IdleSouth);
            ready = true;
        }

        void GetRidOfMe() override {}
    };
}

#endif
