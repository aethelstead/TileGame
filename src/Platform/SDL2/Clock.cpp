#include "../Platform.h"

uint32_t Gin::Platform::Ticks()
{
    return SDL_GetTicks();
}

uint64_t Gin::Platform::Ticks64()
{
    return SDL_GetTicks64();
}

void Gin::Platform::Sleep(uint ms)
{
    SDL_Delay(ms);
}
