#include "../Debug.h"
#include "TextBox.h"

using namespace Gin::VGui;

bool TextBox::FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox)
{
    Element::FromXML(pXmlElem, parentBox);
    
    text = pXmlElem->GetText();

    return true;
}
