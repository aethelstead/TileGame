#ifndef MENU_H
#define MENU_H

#include <cstdint>
#include <vector>
#include <memory>

#include "../Platform/Platform.h"
#include "../Maths/Rect.h"
#include "../Rendering/Renderer.h"
#include "Element.h"

namespace Gin::VGui
{
    struct Menu : public Element
    {
        std::unique_ptr<Texture> pTexture = nullptr;
        uint cursorBtnIdx = 0;
        Recti cursor;

        Menu()
        {}

        bool FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox) override;

        void HandleKeyboard(const uint8_t* pKeyboard);

        void MoveCursorUp();
        void MoveCursorDown();

        void UpdateCursor();
    };
}

#endif
