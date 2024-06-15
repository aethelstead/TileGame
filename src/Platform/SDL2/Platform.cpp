#include "../Platform.h"

bool Gin::Platform::Init()
{
    const int MIX_CHUNKSIZE = 2048;
    const int MIX_FREQUENCY_KHZ = 44100;

    if (SDL_Init( SDL_INIT_EVERYTHING ) != 0)
        return false;
    if (IMG_Init( IMG_INIT_PNG | IMG_INIT_WEBP ) == 0)
        return false;
    if (Mix_Init( MIX_INIT_MP3 ) == 0)
        return false;
    if (Mix_OpenAudio(MIX_FREQUENCY_KHZ, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIX_CHUNKSIZE) == -1)
        return false;
    if (TTF_Init() == -1)
        return false;

    return true;
}

void Gin::Platform::Quit()
{
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
