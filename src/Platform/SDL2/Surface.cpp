#include "../Platform.h"

Gin::Platform::Surface* Gin::Platform::CreateSurface(uint width, uint height)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    uint rmask = 0xff000000;
    uint gmask = 0x00ff0000;
    uint bmask = 0x0000ff00;
    uint amask = 0x000000ff;
#else
    uint rmask = 0x000000ff;
    uint gmask = 0x0000ff00;
    uint bmask = 0x00ff0000;
    uint amask = 0xff000000;
#endif

    return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

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

void Gin::Platform::BlitSurface(Surface* pSrc, Rect* pSrcRect, Surface* pDst, Rect* pDstRect)
{
    SDL_BlitSurface(pSrc, pSrcRect, pDst, pDstRect);
}

void Gin::Platform::BlitScaled(Surface* pSrc, Rect* pSrcRect, Surface* pDst, Rect* pDstRect)
{
    SDL_BlitScaled(pSrc, pSrcRect, pDst, pDstRect);
}
