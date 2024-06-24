#ifndef TRIGGER_H
#define TRIGGER_H

#include <string>

#include "Entity.h"

namespace Gin
{
    struct Trigger : public Entity
    {
        std::string nextMapName;
        Vector2f nextMapSpawnPos;

        Trigger()
        {}

        Trigger(UniqueID uid, uint64_t tilesetId, Vector2f pos, uint64_t width, uint64_t height, uint zIdx)
            : Entity( uid, tilesetId, pos, width, height, zIdx )
        {
            type = EntityType::Trigger;
        }

        void GetRidOfMe() override {}
    };
}

#endif
