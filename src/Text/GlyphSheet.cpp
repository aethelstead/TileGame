#include <cmath>

#include "GlyphSheet.h"
#include "../Debug.h"

using namespace Gin;

static constexpr char* CHARSET = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

std::unique_ptr<GlyphSheet> GlyphSheet::Create(std::string_view fontPath, uint ptSize, Renderer& renderer)
{
    std::unique_ptr<GlyphSheet> pGlyphsheet = std::make_unique<GlyphSheet>(ptSize);

    pGlyphsheet->m_pFont = Platform::OpenFont(fontPath.data(), pGlyphsheet->m_fontSize);
    if (!pGlyphsheet->m_pFont)
    {
        LOGERROR("GlyphSheet::Create() - Failed to create GlyphSheet. Reason: Failed to open font at: " << fontPath << ".");
        return nullptr;
    }
    //glyphsheet.m_pFont = std::make_unique<Platform::TTFont>(std::move(pFont));

    Platform::Colour white{ 0xff, 0xff, 0xff, 0xff };
    Platform::Surface* pSheetSurface = Platform::CreateSurface(pGlyphsheet->m_width, pGlyphsheet->m_height);
    if (!pSheetSurface)
    {
        LOGERROR("GlyphSheet::Create() - Failed to create GlyphSheet. Reason: Failed to open font at create Surface.");
        return nullptr;
    }

    int x = 0;
    int y = 0;
    for (size_t idx = 0; idx < pGlyphsheet->m_nCells; ++idx)
    {
        const char s[] = { CHARSET[idx], '\0' };
        auto pGlyphSurface = TTF_RenderUTF8_Blended(pGlyphsheet->m_pFont, s, white);
        if (!pGlyphSurface)
            break;

        Platform::Rect dest = { x, y, static_cast<int>(pGlyphsheet->m_fontSize), static_cast<int>(pGlyphsheet->m_fontSize) };
        Platform::BlitSurface(pGlyphSurface, nullptr, pSheetSurface, &dest);

        Platform::DestroySurface(pGlyphSurface);

        x += pGlyphsheet->m_fontSize;
        if (x >= pGlyphsheet->m_width)
        {
            y += pGlyphsheet->m_fontSize;
            x = 0;
        }
    }

    pGlyphsheet->m_pTexture = Texture::Create(pSheetSurface, renderer.GetContext());
    Platform::DestroySurface(pSheetSurface);

    return pGlyphsheet;
}

GlyphSheet::GlyphSheet(uint ptSize)
{
    m_fontSize = ptSize;

    std::string charset = CHARSET;
    uint sizeSquared = std::ceil(std::sqrt(charset.size()));
    m_width = sizeSquared * m_fontSize;
    m_height = sizeSquared * m_fontSize;

    m_nCells = charset.size();
    m_cellsPerRow = (m_width / m_fontSize);
}

int GlyphSheet::GetGlyphAdvance(const char& ch)
{
    Recti r;
    int advance =  0;
    TTF_GlyphMetrics32(m_pFont, ch, &r.x, &r.w, &r.y, &r.h, &advance);
    return advance;
}

/*
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
}*/
