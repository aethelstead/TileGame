#include <cmath>

#include "GlyphSheet.h"

using namespace Gin;

static constexpr char* CHARSET = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

int GlyphSheet::GetGlyphAdvance(const char& ch)
{
    Recti r;
    int advance =  0;
    TTF_GlyphMetrics32(m_pFont, ch, &r.x, &r.w, &r.y, &r.h, &advance);
    return advance;
}

bool GlyphSheet::Load(const std::unique_ptr<Renderer>& pRenderer, std::string_view ttfPath, uint fontSize)
{
    m_fontSize = fontSize;

    m_pFont = Platform::OpenFont(ttfPath.data(), m_fontSize);
    if (!m_pFont)
        return false;

    std::string charset = CHARSET;
    uint sizeSquared = std::ceil(std::sqrt(charset.size()));

    m_width = sizeSquared * m_fontSize;
    m_height = sizeSquared * m_fontSize;
    m_nCells = charset.size();
    m_cellsPerRow = (m_width / m_fontSize);

    Platform::Colour white{ 0xff, 0xff, 0xff, 0xff };
    Platform::Surface* pSheetSurface = Platform::CreateSurface(m_width, m_height);
    if (!pSheetSurface)
        return false;

    int x = 0;
    int y = 0;
    for (size_t idx = 0; idx < m_nCells; ++idx)
    {

        const char s[] = { charset.at(idx), '\0' };
        auto pGlyphSurface = TTF_RenderUTF8_Blended(m_pFont, s, white);
        if (!pGlyphSurface)
            return false;

        Platform::Rect dest = { x, y, static_cast<int>(m_fontSize), static_cast<int>(m_fontSize) };
        Platform::BlitSurface(pGlyphSurface, nullptr, pSheetSurface, &dest);

        Platform::DestroySurface(pGlyphSurface);

        x += m_fontSize;
        if (x >= m_width)
        {
            y += m_fontSize;
            x = 0;
        }
    }

    m_pTexture = Texture::Create(pSheetSurface, pRenderer->GetContext());
    Platform::DestroySurface(pSheetSurface);

    return true;
}
