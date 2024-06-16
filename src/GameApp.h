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

    class GameApp
    {
    private:
        bool m_running = false;
        bool m_paused = false;
        bool m_vsync = false;
        uint m_targetFPS = 0;
        uint m_fps = 0;

        Recti m_viewRect;

        std::unique_ptr<Window> m_pWindow;
        std::unique_ptr<Renderer> m_pRenderer;

        TextureMap m_textureMap;
        MusicMap m_musicMap;
        SfxMap m_sfxMap;
    
        GameState m_state;
        GameState m_savedState;

        GameRenderer m_gameRenderer;

        std::unique_ptr<Texture> m_renderTexture;

        std::unordered_map<GameViewType, std::shared_ptr<VGui::View>> m_viewMap;
        std::vector<std::shared_ptr<VGui::View>> m_activeViews;

        Platform::TTFont* m_pFont;

    public:
        GameApp()
        {}

        GameApp(const GameApp&) = delete;

	    GameApp(GameApp&&) = delete;

        ~GameApp()
        {}

        bool Init();

        void Quit();

        void Loop();

    private:

        bool LoadGameMap(const std::string& mapName, WorldMap& worldMap);

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
    };
}

#endif
