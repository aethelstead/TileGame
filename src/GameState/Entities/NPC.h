#ifndef NPC_H
#define NPC_H

#include "Entity.h"

namespace Gin
{
    struct NPC : public Entity
    {
        NPC()
        {}

        void FromProto(const EntityProto& proto) override
        {
            Entity::FromProto(proto);
            type = EntityType::NPC;
            speed = 2;
        }
    };
}

#endif
