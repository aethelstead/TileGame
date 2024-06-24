#ifndef Entity_H
#define Entity_H

#include <memory>

#include "../../Utilities/UniqueID.h"
#include "../Direction.h"
#include "EntityType.h"
#include "../../Maths/Rect.h"

namespace Gin
{
    struct Entity
    {
        Entity()
        {}

        Entity(UniqueID uid, uint64_t tilesetId, Vector2f pos, uint64_t width, uint64_t height, uint zIdx)
            : id( uid )
            , tilesetId( tilesetId )
            , pos( pos )
            , width( width )
            , height( height )
            , zIdx( zIdx )
        {}

        virtual void GetRidOfMe() = 0;

        Rectf Box() const
        {
            return Rectf(pos.x, pos.y, width, height);
        }

        bool IsMoving() const
        {
            return (vel.x != 0 || vel.y != 0);
        }

        UniqueID id = 0;
        uint64_t tilesetId = 0;
        EntityType type = EntityType::None;
        Vector2f pos = Vector2f::Zero();
        Vector2f vel = Vector2f::Zero();
        Vector2f dir = Vector2f::South();
        uint64_t width = 0;
        uint64_t height = 0;
        uint zIdx;
        double speed = 0;
        uint animId = 0;
        uint frameIdx = 0;
        
    };
}

#endif
