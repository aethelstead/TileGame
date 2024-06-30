#ifndef PLATFORM_H
#define PLATFORM_H

#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace Gin::Platform
{
    using Window = SDL_Window;

    using Renderer = SDL_Renderer;
    using Texture = SDL_Texture;
    using Event = SDL_Event;
    using Surface = SDL_Surface;
    using PixelFormat = SDL_PixelFormat;
    using Music = Mix_Music;
    using Chunk = Mix_Chunk;
    using Rect = SDL_Rect;
    using Colour = SDL_Color;

    bool Init();

    void Quit();

    // Clock
    uint32_t Ticks();
    uint64_t Ticks64();
    void Sleep(uint ms);

    // Window
    Window* CreateWindow(const char* title, uint width, uint height, bool fullscreen = false);
    void DestroyWindow(Window* pWindow);
    void GetWindowSize(Window* pWindow, int& x, int& y);

    // Surface
    Surface* CreateSurface(uint width, uint height);
    Surface* LoadSurface(const char* path);
    void DestroySurface(Surface* pSurface);

    uint32_t MapRGB(const PixelFormat* pFormat, uint8_t r, uint8_t g, uint8_t b);
    uint32_t MapRGBA(const PixelFormat* pFormat, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    void GetRGB(const uint32_t& pixel, const PixelFormat* pFormat, uint8_t& r, uint8_t& g, uint8_t& b);
    void GetRGBA(const uint32_t& pixel, const PixelFormat* pFormat, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

    void BlitSurface(Surface* pSrc, Rect* pSrcRect, Surface* pDst, Rect* pDstRect);
    void BlitScaled(Surface* pSrc, Rect* pSrcRect, Surface* pDst, Rect* pDstRect);

    // Renderer
    Renderer* CreateRenderer(Window* pWindow);
    void DestroyRenderer(Renderer* pRenderer);
    void RenderClear(Renderer* pRenderer);
    void RenderCopy(Renderer* pRenderer, Texture* pTexture, const Rect* pSrc, const Rect* pDest, double angle = 0.0, int flip = 0);
    void RenderPresent(Renderer* pRenderer);
    void SetRenderDrawColour(Renderer* pRenderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void RenderDrawPoint(Renderer* pRenderer, int& px, int& py);
    void RenderDrawLine(Renderer* pRenderer, int& startX, int& startY, int& endX, int& endY);
    void RenderDrawRect(Renderer* pRenderer, const Rect& rect);
    void RenderFillRect(Renderer* pRenderer, const Rect& rect);
    
    //Texture
    Texture* CreateTexture(uint width, uint height, uint32_t format, Renderer* pRenderer);
    Texture* CreateTextureFromFile(const char* path, Renderer* pRenderer);
    Texture* CreateTextureFromSurface(Surface* pSurface, Renderer* pRenderer);
    void DestroyTexture(Texture* texture);
    void SetTextureColourMod(Texture* texture, uint8_t r, uint8_t g, uint8_t b);
    void SetTextureAlphaMod(Texture* texture, uint8_t a);

    // Mixer
    Music* CreateMusic(const char* path);
    void DestoryMusic(Music* pMusic);
    void PlayMusic(Music* pMusic);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    Chunk* CreateChunk(const char* path);
    void DestoryChunk(Chunk* pChunk);
    void PlayChunk(Chunk* pChunk);

    // Events
    using ScanCode = SDL_Scancode;
    using KeyCode = SDL_Keycode;
    using EventType = SDL_EventType;
    using Event = SDL_Event;
    enum class MouseButton : int
    {
        Left,
        Middle,
        Right,
        X1,
        X2
    };
    int PollEvent(Event* event);
    const uint8_t* GetKeyboardState();

    // TTF
    using TTFont = TTF_Font;
    TTFont* OpenFont(const char* path, int ptSize);
    void CloseFont(TTFont* font);
    Surface* RenderText(TTFont* font, const char* text, Colour colour);
}

#endif
