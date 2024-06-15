#include "Texture.h"

using namespace Gin;

std::unique_ptr<Texture> Texture::Create(const std::string& filepath, const std::unique_ptr<RenderContext>& pRenderer)
{
    if (filepath.empty())
        return nullptr;

    auto pInternalTexture = Platform::CreateTexture(filepath.c_str(), pRenderer->GetInternal());
    if (!pInternalTexture)
        return nullptr;

    auto pTexture = std::make_unique<Texture>(pInternalTexture);
    return pTexture;
}

std::unique_ptr<Texture> Texture::Create(Platform::Surface* pSurface, const std::unique_ptr<RenderContext>& pRenderer)
{
    if (!pSurface)
        return nullptr;

    auto pInternalTexture = Platform::CreateTextureFromSurface(pSurface, pRenderer->GetInternal());
    if (!pInternalTexture)
        return nullptr;

    auto pTexture = std::make_unique<Texture>(pInternalTexture);

    uint32_t format;
    int access = 0;
    int w = 0;
    int h = 0;
    SDL_QueryTexture(pInternalTexture, &format, &access, &w, &h);
    pTexture->m_width = static_cast<uint>(w);
    pTexture->m_height = static_cast<uint>(h);

    return pTexture;
}

Texture::~Texture()
{
    Platform::DestroyTexture(m_pInternalTexture);
}
