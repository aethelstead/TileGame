#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "Platform/Platform.h"

namespace Gin
{
    class Window
    {
    public:
        static std::unique_ptr<Window> Create(const std::string& title, uint width, uint height, bool fullscreen);

    private:
        Platform::Window* m_pInternalWindow = nullptr;
        uint m_width;
        uint m_height;

    public:
        Window()
        {}

        ~Window();

        Window(Platform::Window* pInternalWindow)
            : m_pInternalWindow( pInternalWindow )
        {}

        uint Width() const { return m_width; }
        uint Height() const { return m_height; }

        Platform::Window* GetInternal() const { return m_pInternalWindow; }
    };
}

#endif
