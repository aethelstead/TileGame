#include "../Platform.h"

Gin::Platform::Chunk* Gin::Platform::CreateChunk(const char* path)
{
    return Mix_LoadWAV(path);
}

void Gin::Platform::DestoryChunk(Chunk* pChunk)
{
    Mix_FreeChunk(pChunk);
}

void Gin::Platform::PlayChunk(Chunk* pChunk)
{
    Mix_PlayChannel(-1, pChunk, 0);
}
