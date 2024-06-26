#ifndef GLYPHSHEET_H
#define GLYPHSHEET_H

#include <memory>

#include "../Platform/Platform.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Renderer.h"

namespace Gin
{
    struct GlyphSheet
    {
        std::unique_ptr<Texture> m_pTexture;
        Platform::TTFont* m_pFont;
        uint m_fontSize = 0;

        GlyphSheet()
        {}

        bool Load(const std::unique_ptr<Renderer>& pRenderer, std::string_view ttfPath, uint fontSize)
        {
            m_fontSize = fontSize;

            m_pFont = Platform::OpenFont(ttfPath.data(), fontSize);
            if (!m_pFont)
                return false;

            uint nChars = '~' - ' ';
            uint sheetWidth = 160;
            uint sheetHeight = 160;

            Platform::Colour white{ 0xff, 0xff, 0xff, 0xff };
            Platform::Surface* pSheetSurface = Platform::CreateSurface(sheetWidth, sheetHeight);
            if (!pSheetSurface)
                return false;
        
            int x = 0;
            int y = 0;
            for (char ch = ' '; ch <= '~'; ++ch)
            {
                const char s[] = { ch, '\0' };
                auto pGlyphSurface = TTF_RenderUTF8_Blended(m_pFont, s, white);
                if (!pGlyphSurface)
                    return false;

                Platform::Rect dest = { x, y, 16, 16 };
                Platform::BlitSurface(pGlyphSurface, nullptr, pSheetSurface, &dest);

                x += fontSize;
                if (x >= sheetWidth)
                {
                    y += fontSize;
                    x = 0;
                }
            }

            m_pTexture = Texture::Create(pSheetSurface, pRenderer->GetContext());

            return true;
        }
    };
}

#endif
