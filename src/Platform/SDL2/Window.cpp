#include "../Platform.h"

Gin::Platform::Window* Gin::Platform::CreateWindow(const char* title, uint width, uint height, bool resizeable)
{
    Uint32 flags = 0;
    if (resizeable)
        flags |= SDL_WINDOW_RESIZABLE;

    // @TODO: Allow user to specify window X/ Y pos rather than always centering
    return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
}

void Gin::Platform::DestroyWindow(Window* window)
{
    SDL_DestroyWindow(window);
}

void Gin::Platform::GetWindowSize(Window* window, int& x, int& y)
{
    SDL_GetWindowSize(window, &x, &y);
}
