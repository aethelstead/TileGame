#include "../Platform.h"

Gin::Platform::Surface* Gin::Platform::LoadSurface(const char* path)
{
    return IMG_Load(path);
}

void Gin::Platform::DestroySurface(Platform::Surface* pSurface)
{
    SDL_FreeSurface(pSurface);
}

uint32_t Gin::Platform::MapRGB(const PixelFormat* pFormat, uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(pFormat, r, g, b);
}

uint32_t Gin::Platform::MapRGBA(const PixelFormat* pFormat, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_MapRGBA(pFormat, r, g, b, a);
}

void Gin::Platform::GetRGB(const uint32_t& pixel, const PixelFormat* pFormat, uint8_t& r, uint8_t& g, uint8_t& b)
{
    SDL_GetRGB(pixel, pFormat, &r, &g, &b);
}

void Gin::Platform::GetRGBA(const uint32_t& pixel, const PixelFormat* pFormat, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a)
{
    SDL_GetRGBA(pixel, pFormat, &r, &g, &b, &a);
}
