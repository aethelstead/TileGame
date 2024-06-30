#include "../Platform.h"

Gin::Platform::Texture* Gin::Platform::CreateTexture(uint width, uint height, uint32_t format, Renderer* pRenderer)
{
    return SDL_CreateTexture(pRenderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
}

Gin::Platform::Texture* Gin::Platform::CreateTextureFromFile(const char* path, Renderer* pRenderer)
{
    return IMG_LoadTexture(pRenderer, path);
}

Gin::Platform::Texture* Gin::Platform::CreateTextureFromSurface(Surface* pSurface, Renderer* pRenderer)
{
    return SDL_CreateTextureFromSurface(pRenderer, pSurface);
}

void Gin::Platform::DestroyTexture(Texture* pTexture)
{
    SDL_DestroyTexture(pTexture);
}

void Gin::Platform::SetTextureColourMod(Texture* texture, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_SetTextureColorMod(texture, r, g, b);
}

void Gin::Platform::SetTextureAlphaMod(Texture* texture, uint8_t a)
{
    SDL_SetTextureAlphaMod(texture, a);
}
