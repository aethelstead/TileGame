#ifndef VIEW_LOADER_H
#define VIEW_LOADER_H

#include <string>
#include <map>
#include <memory>
#include <tinyxml2.h>

#include "View.h"

namespace Gin::VGui
{
    class ViewLoader
    {
    private:
        Recti m_viewRect;
        const std::unique_ptr<Renderer>& m_pRenderer;
        Platform::TTFont* m_pFont;

    public:
        ViewLoader(const Recti& viewRect, const std::unique_ptr<Renderer>& pRenderer, Platform::TTFont* pFont)
            : m_viewRect( viewRect )
            , m_pRenderer( pRenderer )
            , m_pFont( pFont )
        {}

        std::unique_ptr<View> LoadView(const std::string& xmlPath);
    
    private:
        void ParseElementTree(const tinyxml2::XMLElement* pXmlElem, const Recti& parentBox, std::vector<std::shared_ptr<Element>>& parentElems);
    };
}

#endif
