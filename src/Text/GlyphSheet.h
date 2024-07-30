#ifndef GLYPHSHEET_H
#define GLYPHSHEET_H

#include <memory>
#include <string>

#include "../Platform/Platform.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Renderer.h"

namespace Gin
{
    class GlyphSheet
    {
    public:
        static std::unique_ptr<GlyphSheet> Create(std::string_view fontPath, uint ptSize, Renderer& renderer);

    //private:
        std::unique_ptr<Texture> m_pTexture;
        Platform::TTFont* m_pFont;
        uint m_fontSize = 0;
        uint m_width = 0;
        uint m_height = 0;
        uint m_nCells = 0;
        uint m_cellsPerRow = 0;

    public:
        GlyphSheet()
        {}

        GlyphSheet(uint ptSize);

        int GetGlyphAdvance(const char& ch);
    };
}

#endif
