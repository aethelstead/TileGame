#ifndef BUTTON_H
#define BUTTON_H

#include <memory>

#include "Element.h"

namespace Gin::VGui
{
    struct Button : public Element
    {
        int action = 0;

        Button()
        {}

        bool FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox) override;
    };
}

#endif
