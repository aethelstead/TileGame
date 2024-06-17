#include "Window.h"

using namespace Gin;

std::unique_ptr<Window> Window::Create(const std::string& title, uint width, uint height, bool fullscreen)
{
    auto pInternalWindow = Platform::CreateWindow(title.c_str(), width, height, fullscreen);
    if (!pInternalWindow)
        return nullptr;

    auto pWindow = std::make_unique<Window>(pInternalWindow);

    // Retreive window width and height form the SDL_Window
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(pInternalWindow, &w, &h);
    pWindow->m_width = static_cast<uint>(w);
    pWindow->m_height = static_cast<uint>(h);

    return pWindow;
}

Window::~Window()
{
    Platform::DestroyWindow(m_pInternalWindow);
}
