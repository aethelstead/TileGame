#ifndef ELEMENT_H
#define ELEMENT_H

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <tinyxml2.h>

#include "../Rendering/Colour.h"
#include "../Maths/Rect.h"
#include "ElementType.h"

namespace Gin::VGui
{
    struct Element
    {
        uint id = 0;
        ElementType type = ElementType::None;
        Recti box;
        Colour4i colour;
        uint borderWidth = 0;
        Colour4i borderColour;
        std::vector<std::shared_ptr<Element>> childElements;

        Element()
        {}

        virtual bool FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox);
    };
}

#endif
