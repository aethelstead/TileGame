#include "../Platform.h"

Gin::Platform::Window* Gin::Platform::CreateWindow(const char* title, uint width, uint height, bool fullscreen)
{
    Uint32 flags = 0;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

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
