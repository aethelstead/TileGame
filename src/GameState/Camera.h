#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <iostream>

#include "../Maths/Vector2.h"
#include "../Maths/Rect.h"
#include "Entities/Entity.h"
#include "../Debug.h"

namespace Gin
{
    class Camera
    {
    public:
        Vector2f pos = Vector2f::Zero();
        uint64_t width = 0;
        uint64_t height = 0;
        Rectf bounds;
        bool scrollsX = false;
        bool scrollsY = false;

        Camera()
        {}

        Rectf Box() const
        {
            return Rectf(pos.x, pos.y, width, height);
        }

        void Init(const Rectf& entityBox, const Rectf& bounds)
        {
            this->bounds = bounds;
            if (this->bounds.w > width)
            {
                LOGDEBUG("Camera::Init() - Camera will scroll horizontally.");
                scrollsX = true;
            }
            else
            {
                LOGDEBUG("Camera::Init() - Camera will NOT scroll horizontally.");
            }
            
            if (this->bounds.h > height)
            {
                LOGDEBUG("Camera::Init() - Camera will scroll vertically.");
                scrollsY = true;
            }
            else
            {
                LOGDEBUG("Camera::Init() - Camera will NOT scroll vertically.");
            }

            Update(entityBox);
        }

        void Update(const Rectf& entityBox)
        {
            auto newPos = entityBox.AbsCenter() - this->Box().RelCenter();
            if (scrollsX)
            {
                pos.x = newPos.x;

                // Clamp the new X pos to the map bounds
                if (pos.x < bounds.x)
                    pos.x = bounds.x;
                else if (pos.x + width > bounds.x + bounds.w)
                    pos.x = bounds.w - width;
            }
            
            if (scrollsY)
            {
                pos.y = newPos.y;

                // Clamp the new Y pos to the map bounds
                if (pos.y < bounds.y)
                    pos.y = bounds.y;
                else if (pos.y + height > bounds.y + bounds.h)
                    pos.y = bounds.h - height;
            }
        }
    };
}

#endif
