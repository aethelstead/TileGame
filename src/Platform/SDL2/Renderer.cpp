#include <SDL2/SDL.h>

#include "../Platform.h"

Gin::Platform::Renderer* Gin::Platform::CreateRenderer(Window* pWindow)
{
    return (SDL_Renderer*)SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
}

void Gin::Platform::DestroyRenderer(Renderer* pRenderer)
{
    SDL_DestroyRenderer(pRenderer);
}

void Gin::Platform::RenderClear(Renderer* pRenderer)
{
    SDL_RenderClear(pRenderer);
}

void Gin::Platform::RenderCopy(Renderer* pRenderer, Texture* pTexture, const Rect& src, const Rect& dest, double angle, int flip)
{
    SDL_RenderCopyEx(pRenderer, pTexture, &src, &dest, angle, NULL, (SDL_RendererFlip)flip);
}

void Gin::Platform::RenderPresent(Renderer* pRenderer)
{
    SDL_RenderPresent(pRenderer);
}

void Gin::Platform::SetRenderDrawColour(Renderer* pRenderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
}

void Gin::Platform::RenderDrawPoint(Renderer* pRenderer, int& px, int& py)
{
    SDL_RenderDrawPoint(pRenderer, px, py);
}

void Gin::Platform::RenderDrawLine(Renderer* pRenderer, int& startX, int& startY, int& endX, int& endY)
{
    SDL_RenderDrawLine(pRenderer, startX, startY, endX, endY);
}

void Gin::Platform::RenderDrawRect(Renderer* pRenderer, const Rect& rect)
{
    SDL_RenderDrawRect(pRenderer, &rect);
}

void Gin::Platform::RenderFillRect(Renderer* pRenderer, const Rect& rect)
{
    SDL_RenderFillRect(pRenderer, &rect);
}
