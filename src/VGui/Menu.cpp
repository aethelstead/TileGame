#include "../Debug.h"
#include "Menu.h"

using namespace Gin::VGui;

bool Menu::FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox)
{
    Element::FromXML(pXmlElem, parentBox);

    return true;
}

void Menu::HandleKeyboard(const uint8_t* pKeyboard)
{
    if (pKeyboard[ SDL_SCANCODE_UP ])
    {
        MoveCursorUp();
    }
    else if (pKeyboard[ SDL_SCANCODE_DOWN ])
    {
        MoveCursorDown();
    }
    else if (pKeyboard[ SDL_SCANCODE_LEFT ])
    {
        LOGDEBUG("LEFT");
    }
    else if (pKeyboard[ SDL_SCANCODE_RIGHT ])
    {
        LOGDEBUG("RIGHT");
    }
}

void Menu::MoveCursorUp()
{
    /*
    if (cursorBtnIdx > 0)
        cursorBtnIdx--;

    UpdateCursor();*/
}

void Menu::MoveCursorDown()
{
    /*
    if (cursorBtnIdx < buttons.size())
        cursorBtnIdx++;

    UpdateCursor();*/
}

void Menu::UpdateCursor()
{
    /*
    if (cursorBtnIdx < buttons.size())
    {
        const auto& btn = buttons.at(cursorBtnIdx);
        cursor = btn.box;
    }*/
}
