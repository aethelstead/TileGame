#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../Debug.h"
#include "Element.h"

using namespace Gin::VGui;

static void to_lower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c)
        {
            return std::tolower(c);
        });
}

static ElementType GetRawElementType(const tinyxml2::XMLElement* pXmlElem)
{
    if (!pXmlElem)
        return ElementType::None;

    ElementType elemType = ElementType::None;
    std::string elemName = pXmlElem->Name();
    to_lower(elemName);

    if (elemName == "menu")
        elemType = ElementType::Menu;
    else if (elemName == "button")
        elemType = ElementType::Button;
    else if (elemName == "textbox")
        elemType = ElementType::TextBox;

    return elemType;
}

bool Element::FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox)
{
    if (!pXmlElem)
    {
        LOGERROR("MenuLoader::ParseElement() - Failed to parse element because XML Element was NULL.");
        return false;
    }
        
    id = pXmlElem->IntAttribute("id");

    type = GetRawElementType(pXmlElem);
    if (type == ElementType::None)
    {
        LOGERROR("MenuLoader::ParseElement() - Failed to parse element of unrecognised type: '" << pXmlElem->Name() << "'.");
        return false;
    }

    box.x = parentBox.x + pXmlElem->IntAttribute("x");
    box.y = parentBox.y + pXmlElem->IntAttribute("y");
    box.w = pXmlElem->IntAttribute("width");
    box.h = pXmlElem->IntAttribute("height");

    if (pXmlElem->BoolAttribute("centerX"))
    {
        // Override Panel's X position if centerX is defined as true
        box.x = parentBox.x + ((parentBox.w / 2) - (box.w / 2));
    }

    if (pXmlElem->BoolAttribute("centerY"))
    {
        // Override Panel's Y position if centerY is defined as true
        box.y = parentBox.y + ((parentBox.h / 2) - (box.h / 2));
    }

    
    if (auto pszColour = pXmlElem->Attribute("colour"); pszColour)
    {
        auto s = std::string(pszColour);
        Colour4i col;
        col.FromString(s);
        colour = col;
    }

    if (auto pszBorder = pXmlElem->Attribute("border"); pszBorder)
    {
        auto s = std::string(pszBorder);
        
        // Get the index of the start of the 'px' sequence of chars
        auto pxIdx = s.find("px");
        // Get the text just before. Should just be a one or two digit number
        auto rawPx = s.substr(0, pxIdx);
        auto pxWidth = std::atoi(rawPx.c_str());
        borderWidth = pxWidth;

        // Get the index of the '#' this should be the colour hex string
        auto hashIdx = s.find("#");
        auto rawColour = s.substr(hashIdx, s.length() - hashIdx);
        Colour4i colour;
        colour.FromString(rawColour);
        borderColour = colour;
    }

    return true;
}
