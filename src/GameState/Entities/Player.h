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
        
        void FromProto(const EntityProto& proto) override
        {
            Entity::FromProto(proto);
            type = EntityType::Player;
            speed = 2;
            animId = static_cast<uint>(PlayerAnimationType::IdleSouth);
            ready = true;
        }
    };
}

#endif
