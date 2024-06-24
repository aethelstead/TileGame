#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <memory>

#include "../../Tiled/Object.h"
#include "Entity.h"
#include "EntityType.h"
#include "Player.h"
#include "NPC.h"
#include "Trigger.h"

namespace Gin
{
    class EntityFactory
    {
    public:
        EntityFactory()
        {}

        std::shared_ptr<Entity> CreateEntity(const Tiled::Object& obj) const;
    private:
        std::shared_ptr<Player> CreatePlayer(const Tiled::Object& obj) const;
        std::shared_ptr<NPC> CreateNPC(const Tiled::Object& obj) const;
        std::shared_ptr<Trigger> CreateTrigger(const Tiled::Object& obj) const;
    };
}

#endif
