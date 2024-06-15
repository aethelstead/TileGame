#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../Debug.h"
#include "../Rendering/Renderer.h"
#include "ViewLoader.h"
#include "Menu.h"
#include "Button.h"
#include "TextBox.h"

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

void ViewLoader::ParseElementTree(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox, std::vector<std::shared_ptr<Element>>& parentElems)
{
    if (!pXmlElem)
        return;
    
    auto elemType = GetRawElementType(pXmlElem);
    if (elemType == ElementType::None)
        return;

    std::shared_ptr<Element> pElem = nullptr;
    if (elemType == ElementType::Menu)
    {
        pElem = std::make_shared<Menu>();
        pElem->FromXML(pXmlElem, parentBox);
    }
    else if (elemType == ElementType::Button)
    {
        pElem = std::make_shared<Button>();
        pElem->FromXML(pXmlElem, parentBox);
    }
    else if (elemType == ElementType::TextBox)
    {
        pElem = std::make_shared<TextBox>();
        pElem->FromXML(pXmlElem, parentBox);

        auto pTextBox = dynamic_cast<TextBox*>(pElem.get());
        pTextBox->pTexture = m_pRenderer->PreRenderText(m_pFont, pTextBox->text.c_str(), pTextBox->box);
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

std::unique_ptr<View> ViewLoader::LoadView(const std::string& xmlPath)
{
    if (xmlPath.empty())
    {
        LOGERROR("ViewLoader::LoadView() - Failed to load view from XML. Path to XML file was empty.");
        return nullptr;
    }

    std::unique_ptr<View> pView = nullptr;
    {
        using namespace tinyxml2;

        XMLDocument doc;
        if (doc.LoadFile(xmlPath.c_str()) != XML_SUCCESS)
        {
            LOGERROR("ViewLoader::LoadView() - Failed to load view from XML at: '" << xmlPath << "'. Could not load XML file to document.");
            return nullptr;
        }

        auto pRoot = doc.FirstChild();
        if (!pRoot)
        {
            LOGERROR("ViewLoader::LoadView() - Failed to load view from XML at: '" << xmlPath << "'. Could not get root node.");
            return nullptr;
        }

        auto pRootElem = pRoot->ToElement();
        if (!pRootElem)
        {
            LOGERROR("ViewLoader::LoadView() - Failed to load view from XML at: '" << xmlPath << "'. Could not get root element.");
            return nullptr;
        }
        
        auto rootElemType = GetRawElementType(pRootElem);
        if (rootElemType != ElementType::Menu)
        {
            LOGERROR("MenuLoViewLoaderader::LoadView() - Failed to load view from XML at: '" << xmlPath << "'. Root element must be 'menu'.");
            return nullptr;
        }

        // Create the view element which will act as the root of all other elements, which are loaded from XML.
        pView = std::make_unique<View>();
        pView->type = ElementType::View;
        pView->box = m_viewRect;

        ParseElementTree(pRootElem, pView->box, pView->childElements);
    }

    return pView;
}
