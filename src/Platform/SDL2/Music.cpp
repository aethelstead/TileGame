#include "../Platform.h"

Gin::Platform::Music* Gin::Platform::CreateMusic(const char* path)
{
    return Mix_LoadMUS(path);
}

void Gin::Platform::DestoryMusic(Music* pMusic)
{
    Mix_FreeMusic(pMusic);
}

void Gin::Platform::PlayMusic(Music* pMusic)
{
    Mix_PlayMusic(pMusic, -1);
}

void Gin::Platform::StopMusic()
{
    Mix_HaltMusic();
}

void Gin::Platform::PauseMusic()
{
    Mix_PauseMusic();
}

void Gin::Platform::ResumeMusic()
{
    Mix_ResumeMusic();
}
