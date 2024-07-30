#include <algorithm>

#include "View.h"
#include "Button.h"
#include "TextBox.h"
#include "Menu.h"
#include "../Debug.h"

using namespace Gin::VGui;

static void to_lower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c)
        {
            return std::tolower(c);
        });
}

static ElementType GetRawElementType(const tinyxml2::XMLElement* pXmlElem)
{
    if (!pXmlElem)
        return ElementType::None;

    ElementType elemType = ElementType::None;
    std::string elemName = pXmlElem->Name();
    to_lower(elemName);

    if (elemName == "menu")
        elemType = ElementType::Menu;
    else if (elemName == "button")
        elemType = ElementType::Button;
    else if (elemName == "textbox")
        elemType = ElementType::TextBox;

    return elemType;
}

static void ParseElementTree(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox, std::vector<std::shared_ptr<Element>>& parentElems)
{
    if (!pXmlElem)
        return;
    
    auto elemType = GetRawElementType(pXmlElem);
    if (elemType == ElementType::None)
        return;

    std::shared_ptr<Element> pElem = nullptr;
    if (elemType == ElementType::Menu)
    {
        pElem = std::make_shared<Gin::VGui::Menu>();
        pElem->FromXML(pXmlElem, parentBox);
    }
    else if (elemType == ElementType::Button)
    {
        pElem = std::make_shared<Gin::VGui::Button>();
        pElem->FromXML(pXmlElem, parentBox);
    }
    else if (elemType == ElementType::TextBox)
    {
        pElem = std::make_shared<Gin::VGui::TextBox>();
        pElem->FromXML(pXmlElem, parentBox);

        auto pTextBox = dynamic_cast<Gin::VGui::TextBox*>(pElem.get());
        pTextBox->text = pXmlElem->GetText();
    }

    if (!pElem)
        return;

    auto pFirstChild = pXmlElem->FirstChildElement();
    if (pFirstChild)
    {
        ParseElementTree(pFirstChild, pElem->box, pElem->childElements);
    }

    auto pSiblingElem = pXmlElem->NextSiblingElement();
    if (pSiblingElem)
    {
        ParseElementTree(pSiblingElem, parentBox, parentElems);
    }

    parentElems.emplace_back(std::move(pElem));
}

std::unique_ptr<View> View::Create(std::string_view xmlPath, const Recti& viewRect, const std::unique_ptr<Renderer>& pRenderer)
{
    if (xmlPath.empty())
    {
        LOGERROR("View::Create() - Failed to load view from XML. Path to XML file was empty.");
        return nullptr;
    }

    std::unique_ptr<View> pView = nullptr;
    {
        using namespace tinyxml2;

        XMLDocument doc;
        if (doc.LoadFile(xmlPath.data()) != XML_SUCCESS)
        {
            LOGERROR("View::Create() - Failed to load view from XML at: '" << xmlPath << "'. Could not load XML file to document.");
            return nullptr;
        }

        auto pRoot = doc.FirstChild();
        if (!pRoot)
        {
            LOGERROR("View::Create() - Failed to load view from XML at: '" << xmlPath << "'. Could not get root node.");
            return nullptr;
        }

        auto pRootElem = pRoot->ToElement();
        if (!pRootElem)
        {
            LOGERROR("View::Create() - Failed to load view from XML at: '" << xmlPath << "'. Could not get root element.");
            return nullptr;
        }
        
        auto rootElemType = GetRawElementType(pRootElem);
        if (rootElemType != ElementType::Menu)
        {
            LOGERROR("View::Create() - Failed to load view from XML at: '" << xmlPath << "'. Root element must be 'menu'.");
            return nullptr;
        }

        // Create the view element which will act as the root of all other elements, which are loaded from XML.
        pView = std::make_unique<View>();
        pView->type = ElementType::View;
        pView->box = viewRect;

        ParseElementTree(pRootElem, pView->box, pView->childElements);
    }

    return pView;
}

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

        if (!pTextBox->text.empty())
        {
            
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
