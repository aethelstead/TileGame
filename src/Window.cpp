#include "Window.h"

using namespace Gin;

std::unique_ptr<Window> Window::Create(const std::string& title, uint width, uint height, bool fullscreen)
{
    auto pInternalWindow = Platform::CreateWindow(title.c_str(), width, height, fullscreen);
    if (!pInternalWindow)
        return nullptr;

    return std::make_unique<Window>(pInternalWindow);
}

Window::~Window()
{
    Platform::DestroyWindow(m_pInternalWindow);
}
