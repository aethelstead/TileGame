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
        
        void FromProto(const EntityProto& proto) override;
    };
}

#endif
