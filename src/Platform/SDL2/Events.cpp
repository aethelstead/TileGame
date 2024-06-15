#include "../Platform.h"

int Gin::Platform::PollEvent(Event* event)
{
    return SDL_PollEvent(event);
}

const uint8_t* Gin::Platform::GetKeyboardState()
{
    return SDL_GetKeyboardState(NULL);
}
