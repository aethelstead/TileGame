#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <string>

#include "Assets.h"
#include "Window.h"
#include "Debug.h"
#include "GameRenderer.h"
#include "VGui/VGui.h"
#include "Platform/Platform.h"
#include "GameState/GameState.h"
#include "Maths/Vector2.h"
#include "Maths/Rect.h"
#include "Rendering/Renderer.h"
#include "Text/GlyphSheet.h"

namespace Gin
{
    enum class GameViewType : uint
    {
        None = 0,
        Main,
        Save,
        Load,
        Settings,
        Equipment,
        Inventory,
        Map,
        Stats,
        Count
    };

    enum class GameFontType : uint
    {
        Arial16,
        ArialBold16,
        Calibri16,
        CalibriBold16,
        Consolas16,
        ConsolasBold16,
        Verdana16,
        VerdanaBold16,
        Times16,
        TimesBold16
    };

    class GameApp
    {
    private:
        bool m_running = false;
        bool m_paused = false;
        bool m_vsync = false;


        uint m_targetFPS = 0;
        uint m_fps = 0;
        uint m_frameCount = 0;

        Recti m_viewRect;

        std::unique_ptr<Window> m_pWindow;
        std::unique_ptr<Renderer> m_pRenderer;

        TextureMap m_textureMap;
        MusicMap m_musicMap;
        SfxMap m_sfxMap;
    
        GameState m_state;
        //GameState m_savedState;

        GameRenderer m_gameRenderer;

        std::unique_ptr<Texture> m_renderTexture;

        std::unordered_map<GameViewType, std::shared_ptr<VGui::View>> m_viewMap;
        std::vector<std::shared_ptr<VGui::View>> m_activeViews;

        std::unordered_map<GameFontType, std::unique_ptr<GlyphSheet>> m_glyphsheetMap;

    public:
        GameApp()
        {}

        bool Init();

        void Quit();

        void Loop();

    private:

        bool InitText();

        bool LoadGameMap(const std::string& mapName, Tiled::Map& tiledMap);

        UniqueID LoadTexture(const std::string& path);

        void QuickSave();
        void QuickLoad();

        void PauseGame();
        void ResumeGame();
        void ToggleGamePaused();

        void ShowView(GameViewType menuType);
        void HideTopView();
        void ToggleViewShow(const GameViewType& viewType);

        void HandlePlatformEvents();

        void Update(double dt);
        void UpdateAnimations(double dt);

        void Render();
        void RenderDebugOverlay();
        void RenderText(std::string_view text, GlyphSheet& glypsheet, const Vector2i& pos, Colour4i colour = Colour4i::White());
    };
}

#endif
