#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <string>

#include "Element.h"
#include "../Rendering/Texture.h"

namespace Gin::VGui
{
    struct TextBox : public Element
    {
        std::string text;
        std::unique_ptr<Texture> pTexture = nullptr;

        TextBox()
        {}

        bool FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox) override;
    };
}

#endif
