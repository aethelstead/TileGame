#include "../Platform.h"

Gin::Platform::Texture* Gin::Platform::CreateTexture(const char* path, Renderer* pRenderer)
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
