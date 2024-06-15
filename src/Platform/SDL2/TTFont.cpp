#include "../Platform.h"

Gin::Platform::TTFont* Gin::Platform::OpenFont(const char* path, int ptSize)
{
    return TTF_OpenFont(path, ptSize);
}

void Gin::Platform::CloseFont(Platform::TTFont* font)
{
    TTF_CloseFont(font);
}

Gin::Platform::Surface* Gin::Platform::RenderText(TTFont* font, const char* text, Colour colour)
{
    return TTF_RenderText_Solid(font, text, colour);
}
