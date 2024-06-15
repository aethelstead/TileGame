#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "../Maths/Rect.h"
#include "../Window.h"
#include "Texture.h"
#include "RenderContext.h"
#include "Colour.h"

namespace Gin
{
    class Renderer
    {
    public:
        static std::unique_ptr<Renderer> Create(const std::unique_ptr<Window>& pWindow);

    private:
        std::unique_ptr<RenderContext> m_pContext = nullptr;

    public:
        Renderer()
        {}

        std::unique_ptr<RenderContext>& GetContext() { return m_pContext; }

        void Clear(const Colour4i colour = Colour4i::Black());

        void Copy(const std::unique_ptr<Texture>& pTexture, const Recti& src, const Recti& dest);

        void Present();

        void DrawRect(const Recti& rect, const Colour4i colour = Colour4i::White());

        void FillRect(const Recti& rect, const Colour4i colour = Colour4i::White());

        std::unique_ptr<Texture> PreRenderText(Platform::TTFont* pFont, const char* text, Recti box, Colour4i colour = Colour4i::White());

    private:
        void SetDrawColour(const Colour4i& colour);
    };
}

#endif
