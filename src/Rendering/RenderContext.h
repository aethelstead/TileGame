#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <memory>

#include "Colour.h"
#include "../Platform/Platform.h"
#include "../Window.h"

namespace Gin
{
    class RenderContext
    {
    public:
        static std::unique_ptr<RenderContext> Create(const std::unique_ptr<Window>& pWindow);

    private:
        Platform::Renderer* m_pInternalRenderer = nullptr;

    public:
        RenderContext()
        {}

        RenderContext(Platform::Renderer* pInternalRenderer)
            : m_pInternalRenderer( pInternalRenderer )
        {}

        ~RenderContext();

        Platform::Renderer* GetInternal() const { return m_pInternalRenderer; }
    };
}

#endif
