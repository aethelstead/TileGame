#include "RenderContext.h"

using namespace Gin;

std::unique_ptr<RenderContext> RenderContext::Create(const std::unique_ptr<Window>& pWindow)
{
    if (!pWindow || !pWindow->GetInternal())
        return nullptr;

    auto pInternalRenderer = Platform::CreateRenderer(pWindow->GetInternal());
    if (!pInternalRenderer)
        return nullptr;

    auto pRenderer = std::make_unique<RenderContext>(pInternalRenderer);
    return pRenderer;
}

RenderContext::~RenderContext()
{
    Platform::DestroyRenderer(m_pInternalRenderer);
}
