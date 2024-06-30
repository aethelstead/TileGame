#ifndef GLYPHSHEET_H
#define GLYPHSHEET_H

#include <memory>
#include <string>

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
        uint m_width = 0;
        uint m_height = 0;
        uint m_nCells = 0;
        uint m_cellsPerRow = 0;

        GlyphSheet()
        {}

        int GetGlyphAdvance(const char& ch);

        bool Load(const std::unique_ptr<Renderer>& pRenderer, std::string_view ttfPath, uint fontSize);
    };
}

#endif
