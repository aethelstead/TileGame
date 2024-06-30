#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>

#include "../Platform/Platform.h"
#include "RenderContext.h"
#include "Colour.h"

namespace Gin
{
    class Texture
    {
    public:
        static std::unique_ptr<Texture> Create(uint width, uint height, const std::unique_ptr<RenderContext>& pRenderer);

        static std::unique_ptr<Texture> Create(const std::string& filepath, const std::unique_ptr<RenderContext>& pRenderer);

        static std::unique_ptr<Texture> Create(Platform::Surface* pSurface, const std::unique_ptr<RenderContext>& pRenderer);

    private:
        uint m_width = 0;
        uint m_height = 0;
        Platform::Texture* m_pInternalTexture = nullptr;

    public:
        Colour4i m_colourMod = Colour4i::White();

        Texture()
        {}

        Texture(Platform::Texture* pInternalTexture)
            : m_pInternalTexture( pInternalTexture )
        {}

        ~Texture();

        uint Width() const { return m_width; }

        uint Height() const { return m_height; }

        Platform::Texture* GetInternal() const { return m_pInternalTexture; }

        bool IsRGBAModded() const { return (m_colourMod != Colour4i::White()); }
    };
}

#endif
