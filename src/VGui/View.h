#ifndef VIEW_H
#define VIEW_H

#include <memory>

#include "Element.h"
#include "../Rendering/Renderer.h"

namespace Gin::VGui
{
    class View : public Element
    {
    public:
        static std::unique_ptr<View> Create(std::string_view xmlPath, const Recti& viewRect, const std::unique_ptr<Renderer>& pRenderer);

    public:
        View()
        {}

        //static std::unique_ptr<View> Create(const std::string& xmlPath, const Recti& viewRect, const std::unique_ptr<Renderer>& pRenderer);

        bool FromXML(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox) override
        {
            // View is the only element that cannot be parsed from XML, so just return false.
            return false;
        }

        void Render(const std::unique_ptr<Renderer>& pRenderer) const;

    private:
        void RenderElementTree(const std::shared_ptr<Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const;
        void RenderElement(const std::shared_ptr<Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const;
        void RenderBorder(const std::shared_ptr<Element>& pElem, const std::unique_ptr<Renderer>& pRenderer) const;
    };
}

#endif
