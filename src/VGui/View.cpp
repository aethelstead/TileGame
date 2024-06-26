#include "View.h"
#include "Button.h"
#include "TextBox.h"

using namespace Gin::VGui;

void View::Render(const std::unique_ptr<Renderer>& pRenderer) const
{
    // View will only have one child element, which is the menu.
    const std::shared_ptr<Element>& pMenu = childElements.front();
    if (!pMenu)
        return;
        
    RenderElementTree(pMenu, pRenderer);
}

void View::RenderElementTree(const std::shared_ptr<VGui::Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const
{
    RenderElement(pElem, pRenderer);

    for (const auto& pe : pElem->childElements)
    {
        RenderElementTree(pe, pRenderer);
    }
}

void View::RenderElement(const std::shared_ptr<VGui::Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const
{
    pRenderer->FillRect(pElem->box, pElem->colour);
    if (pElem->borderWidth > 0)
        RenderBorder(pElem, pRenderer);

    if (pElem->type == VGui::ElementType::TextBox)
    {
        const auto pTextBox = dynamic_cast<TextBox*>(pElem.get());
        pRenderer->DrawRect(pTextBox->box, Colour4i::Black());

        if (pTextBox->pTexture)
        {
            Recti src(0, 0, pTextBox->pTexture->Width(), pTextBox->pTexture->Height());
            Recti dst(pTextBox->box.x, pTextBox->box.y, pTextBox->pTexture->Width(), pTextBox->pTexture->Height());
            pRenderer->Copy(pTextBox->pTexture, src, dst);
        }
    }
}

void View::RenderBorder(const std::shared_ptr<Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const
{
    Recti topRect(pElem->box.x, pElem->box.y, pElem->box.w, pElem->borderWidth);
    Recti rightRect((pElem->box.x + pElem->box.w) - pElem->borderWidth, pElem->box.y, pElem->borderWidth, pElem->box.h);
    Recti bottomRect(pElem->box.x, (pElem->box.y + pElem->box.h) - pElem->borderWidth, pElem->box.w, pElem->borderWidth);
    Recti leftRect(pElem->box.x, pElem->box.y, pElem->borderWidth, pElem->box.h);
    pRenderer->FillRect(topRect, pElem->borderColour);
    pRenderer->FillRect(rightRect, pElem->borderColour);
    pRenderer->FillRect(bottomRect, pElem->borderColour);
    pRenderer->FillRect(leftRect, pElem->borderColour);
}
