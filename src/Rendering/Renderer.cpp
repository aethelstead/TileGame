#include "Renderer.h"
#include "../Debug.h"

using namespace Gin;

std::unique_ptr<Renderer> Renderer::Create(const std::unique_ptr<Window>& pWindow, bool vsync)
{
    if (!pWindow)
        return nullptr;

    auto pRenderer = std::make_unique<Renderer>();
    pRenderer->m_pContext = RenderContext::Create(pWindow);

    if (vsync)
    {
        SDL_RenderSetVSync(pRenderer->GetContext()->GetInternal(), 1);
        LOGINFO("Renderer::Create() - Vsync was enabled.");
    }

    // Enable alpha blending
    SDL_SetRenderDrawBlendMode(pRenderer->GetContext()->GetInternal(), SDL_BLENDMODE_BLEND);

    const auto pWindowSurface = SDL_GetWindowSurface(pWindow->GetInternal());
    Colour4i::pFormat = pWindowSurface->format;

    return pRenderer;
}

void Renderer::Clear(const Colour4i colour)
{
    if (!m_pContext || !m_pContext->GetInternal())
        return;

    SetDrawColour(colour);

    Platform::RenderClear(m_pContext->GetInternal());
}

void Renderer::Copy(const std::unique_ptr<Texture>& pTexture)
{
    if (pTexture->IsRGBAModded())
    {
        SetTextureRGBAMod(pTexture);
    }

    Platform::RenderCopy(m_pContext->GetInternal(), pTexture->GetInternal(), nullptr, nullptr);
}

void Renderer::Copy(const std::unique_ptr<Texture>& pTexture, const Recti& dest)
{
    if (pTexture->IsRGBAModded())
    {
        SetTextureRGBAMod(pTexture);
    }

    Platform::Rect destRect = { 
        .x = dest.x,
        .y = dest.y,
        .w = dest.w,
        .h = dest.h
    };

    Platform::RenderCopy(m_pContext->GetInternal(), pTexture->GetInternal(), nullptr, &destRect);
}

void Renderer::Copy(const std::unique_ptr<Texture>& pTexture, const Recti& src, const Recti& dest)
{
    if (pTexture->IsRGBAModded())
    {
        SetTextureRGBAMod(pTexture);
    }

    Platform::Rect srcRect = { 
        .x = src.x,
        .y = src.y,
        .w = src.w,
        .h = src.h
    };

    Platform::Rect destRect = { 
        .x = dest.x,
        .y = dest.y,
        .w = dest.w,
        .h = dest.h
    };

    Platform::RenderCopy(m_pContext->GetInternal(), pTexture->GetInternal(), &srcRect, &destRect);
}

void Renderer::Present()
{
    if (!m_pContext || !m_pContext->GetInternal())
        return;

    Platform::RenderPresent(m_pContext->GetInternal());   
}

void Renderer::DrawRect(const Recti& rect, const Colour4i colour)
{
    SetDrawColour(colour);

    Platform::Rect r = { 
        .x = rect.x,
        .y = rect.y,
        .w = rect.w,
        .h = rect.h
    };

    Platform::RenderDrawRect(m_pContext->GetInternal(), r);
}

void Renderer::FillRect(const Recti& rect, const Colour4i colour)
{
    SetDrawColour(colour);

    Platform::Rect r = { 
        .x = rect.x,
        .y = rect.y,
        .w = rect.w,
        .h = rect.h
    };

    Platform::RenderFillRect(m_pContext->GetInternal(), r);
}

void Renderer::SetDrawColour(const Colour4i& colour)
{
    Platform::SetRenderDrawColour(m_pContext->GetInternal(), colour.m_r, colour.m_g, colour.m_b, colour.m_a);
}

void Renderer::SetTextureRGBAMod(const std::unique_ptr<Texture>& pTexture)
{
    Platform::SetTextureColourMod(pTexture->GetInternal(), pTexture->m_colourMod.m_r, pTexture->m_colourMod.m_g, pTexture->m_colourMod.m_b);
    Platform::SetTextureAlphaMod(pTexture->GetInternal(), pTexture->m_colourMod.m_a);
}
