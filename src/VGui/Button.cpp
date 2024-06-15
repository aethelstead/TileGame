#include "../Debug.h"
#include "Button.h"

using namespace Gin::VGui;

bool Button::FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox)
{
    Element::FromXML(pXmlElem, parentBox);

    action = 99;

    return true;
}
