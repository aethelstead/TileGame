#ifndef NPC_H
#define NPC_H

#include "Entity.h"

namespace Gin
{
    struct NPC : public Entity
    {
        NPC()
        {}

        NPC(UniqueID uid, uint64_t tilesetId, Vector2f pos, uint64_t width, uint64_t height, uint zIdx)
            : Entity( uid, tilesetId, pos, width, height, zIdx )
        {
            type = EntityType::NPC;
            speed = 2;
            //animId = static_cast<uint>(PlayerAnimationType::IdleSouth);
            //ready = true;
        }

        void GetRidOfMe() override {}
    };
}

#endif
