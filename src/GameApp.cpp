#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <set>

#include "Config.h"
#include "GameApp.h"

using namespace Gin;

bool GameApp::Init()
{
    LOGINFO("Game::Init() - Initialising...");

    // Load config
    Config config;
    {
        auto iniPath = "config.ini";
        if (config.LoadFromIni(iniPath))
        {
            LOGINFO("Game::Init() - Loaded config from '" << iniPath << "'.");
        }
        else
        {
            LOGWARN("Game::Init() - Failed to load config from '" << iniPath << "'. Default config will be used instead.");
        }
    }

    // Init platform, create Window and create Rendererer
    {
        if (!Platform::Init())
        {
            LOGERROR("Platform::Init() - Failed.");
            return false;
        }
        LOGINFO("Game::Init() - Platform initialised.");

        m_pWindow = Window::Create(config.windowTitle.c_str(), config.windowWidth, config.windowHeight);
        if (!m_pWindow)
        {
            LOGERROR("Game::Init() - Failed to create platform window.");
            return false;
        }
        LOGINFO("Game::Init() - Created Platform Window.");

        m_pRenderer = Renderer::Create(m_pWindow);
        if (!m_pRenderer)
        {
            LOGERROR("Game::Init() - Failed to create platform renderer.");
            return false;
        }
        LOGINFO("Game::Init() - Created Platform Renderer.");
    }

    // Init text
    {
        m_pFont = Platform::OpenFont("assets/font/arial_bold.ttf", 18);
        if (!m_pFont)
        {
            LOGWARN("Game::Init() - Failed to open font: 'assets/font/arial_bold.ttf'");
        }
    }

    m_viewRect = Recti(0, 0, config.windowWidth, config.windowHeight);

    // Init menus
    {
        VGui::ViewLoader viewLoader(m_viewRect, m_pRenderer, m_pFont);

        // @TODO: Error checking LoadMenu
        // Add all of the required menus to the menu map
        m_viewMap[ GameViewType::Main ] = viewLoader.LoadView("assets/menu/main.xml");
        m_viewMap[ GameViewType::Save ] = viewLoader.LoadView("assets/menu/save.xml");
        m_viewMap[ GameViewType::Load ] = viewLoader.LoadView("assets/menu/load.xml");
        m_viewMap[ GameViewType::Settings ] = viewLoader.LoadView("assets/menu/settings.xml");
        m_viewMap[ GameViewType::Equipment ] = viewLoader.LoadView("assets/menu/equipment.xml");
        m_viewMap[ GameViewType::Inventory ] = viewLoader.LoadView("assets/menu/inventory.xml");
        m_viewMap[ GameViewType::Map ] = viewLoader.LoadView("assets/menu/map.xml");
        m_viewMap[ GameViewType::Stats ] = viewLoader.LoadView("assets/menu/stats.xml");
    }

    m_state.Init(config.windowWidth, config.windowHeight);

    // Load initial game map and change map in GameState
    {
        WorldMap worldMap;
        if (!config.initMapName.empty())
        {
            if (!LoadGameMap(config.initMapName, worldMap))
            {
                LOGWARN("Game::Init() - Initial game map could not be loaded.");
                return false;
            }
        }
        else
        {
            LOGWARN("Game::Init() - Game map not loaded because no map was specified in config.");
        }
        LOGINFO("Game::Init() - Loaded initial game map: " << config.initMapName);
    
        m_state.ChangeMap(worldMap);
    }

    m_savedState = m_state;

    LOGINFO("Game::Init() - Initialisation successful.");

    {
        const auto& tileset = m_state.worldMap.m_tilesetMap[m_state.m_pPlayer->tilesetId];
        for (const auto& [tileId, animation] : tileset.animationsMap)
        {
            LOGDEBUG(tileId << ": " << animation.frames.size());
        }
    }

    return true;
}

void GameApp::Quit()
{
    //Platform::CloseFont(m_pFont);
    LOGINFO("Game::Quit() - Quitting...");
    SDL_Quit();
}

bool GameApp::LoadGameMap(const std::string& mapName, WorldMap& worldMap)
{
    m_textureMap.clear();
    
    // Get map path from name
    std::string mapPath;
    {
        std::stringstream ss;
        ss << MAP_PATH << mapName << ".tmx";
        mapPath = ss.str();
    }
    LOGINFO("GameApp::LoadGameMap() - Loading game map from: " << mapPath);

    if (!worldMap.LoadFromTmx(mapPath, worldMap))
        return false;

    // Load texture for each tileset
    std::stringstream ss;
    for (auto& [_, tileset] : worldMap.m_tilesetMap)
    {
        // @TODO: Tiled returns tileset.texturePath's as "img/tileset.tsx". Inconsistent with game map loading etc.
        // Fix texture img filepath
        std::string imgpath;
        {
            std::stringstream ss;
            ss.clear();
            ss << "assets/tmx/tsx/" <<  tileset.texturePath;
            imgpath = ss.str();
        }

        auto textureId = LoadTexture(imgpath);
        if (textureId == 0)
        {
            LOGWARN("Game::LoadGameMap() - Failed to load img file for texture: " << imgpath);
        }

        tileset.textureId = textureId;

        LOGINFO("Game::LoadGameMap() - Loaded texture: " << imgpath);
    }

    return true;
}

UniqueID GameApp::LoadTexture(const std::string& path)
{
    if (!m_pRenderer || !m_pRenderer->GetContext())
        return 0;

    if (path.empty())
        return 0;

    auto pTexture = Texture::Create(path, m_pRenderer->GetContext());
    if (!pTexture)
        return 0;

    auto assetId = UniqueIDGenerator::Next();
    m_textureMap.try_emplace(assetId, std::move(pTexture));

    return assetId;
}

void GameApp::QuickSave()
{
    m_savedState = m_state;
    LOGINFO("GameApp::QuickSave() - Quick save done.");
}

void GameApp::QuickLoad()
{
    m_state = m_savedState;
    LOGINFO("GameApp::QuickLoad() - Quick load done.");
}

void GameApp::PauseGame()
{
    m_paused = true;
}

void GameApp::ResumeGame()
{
    m_paused = false;
}

void GameApp::ToggleGamePaused()
{
    m_paused = !m_paused;
}

void GameApp::ShowView(GameViewType viewType)
{
    if (viewType == GameViewType::None)
        return;

    const auto& pMenu = m_viewMap[viewType];
    if (!pMenu)
        return;

    m_activeViews.push_back(pMenu);
}

void GameApp::HideTopView()
{
    if (!m_activeViews.empty())
    {
        m_activeViews.pop_back();
    }
}

void GameApp::ToggleViewShow(const GameViewType& menuType)
{
    static bool isOpen = true;
    isOpen = !isOpen;

    if (isOpen)
    {
        HideTopView();
        ResumeGame();
    }
    else
    {
        PauseGame();
        ShowView(menuType);
    }
}

void GameApp::HandlePlatformEvents()
{
    Platform::Event e;
    while (Platform::PollEvent(&e) > 0)
    {
        if (e.type == SDL_QUIT)
        {
            m_running = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    ToggleViewShow(GameViewType::Main);
                    break;
                case SDLK_m:
                    ToggleViewShow(GameViewType::Map);
                    break;
                case SDLK_TAB:
                    ToggleViewShow(GameViewType::Stats);
                    break;
                case SDLK_F5:
                    QuickSave();
                    break;
                case SDLK_F6:
                    QuickLoad();
                    break;
                case SDLK_w:
                    m_state.PlayerWalk(Direction::North);
                    break;
                case SDLK_d:
                    m_state.PlayerWalk(Direction::East);
                    break;
                case SDLK_s:
                    m_state.PlayerWalk(Direction::South);
                    break;
                case SDLK_a:
                    m_state.PlayerWalk(Direction::West);
                    break;
                case SDLK_SPACE:
                    m_state.PlayerAttack();
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                    m_state.PlayerStopWalk(Direction::North);
                    break;
                case SDLK_d:
                    m_state.PlayerStopWalk(Direction::East);
                    break;
                case SDLK_s:
                    m_state.PlayerStopWalk(Direction::South);
                    break;
                case SDLK_a:
                    m_state.PlayerStopWalk(Direction::West);
                    break;
            }
        }
    }

    bool menuOpen = (!m_activeViews.empty());
    if (!menuOpen)
        return;

    //const auto& pTopMenu = m_activeViews.back();
    //pTopMenu->HandleKeyboard(Platform::GetKeyboardState());
}

void GameApp::Update()
{
    if (!m_paused)
    {
        bool mapChange = false;
        std::string nextMapName;
        Vector2f spawnPos;
        m_state.Update(m_rtClock, mapChange, nextMapName, spawnPos);

        UpdateAnimations();

        if (mapChange)
        {
            WorldMap worldMap;
            if (!LoadGameMap(nextMapName, worldMap))
            {
                LOGDEBUG("GameApp::Update() - Failed to load new game map.");
                return;
            }

            m_state.ChangeMap(worldMap, &spawnPos);
        } 
    } 
}

void GameApp::UpdateAnimations()
{
    // @TODO: This seems to be updating all animations regardless of whether or not they are shown

    // Update Tileset animations
    for (auto& [_, tileset] : m_state.worldMap.m_tilesetMap)
    {
        for (auto& [_, animation] : tileset.animationsMap)
        {
            const auto& currFrame = animation.frames[animation.currFrameIdx];

            animation.nextFrameMs += m_rtClock.Delay();
            
            bool frameDurationExceeded = (animation.nextFrameMs >= currFrame.duration);
            if (!frameDurationExceeded)
                continue;

            animation.nextFrameMs = 0;
            ++animation.currFrameIdx;

            bool resetFrameIdx = (animation.currFrameIdx >= animation.frames.size());
            if (resetFrameIdx)
                animation.currFrameIdx = 0;
        }
    }

    // Update Tile animations (TilesInView only)
    {
        auto& viewTiles = m_state.WorldTilesInView();
        for (auto& viewTile : viewTiles)
        {
            const auto& tileset = m_state.worldMap.m_tilesetMap[viewTile.tilesetId];
            const auto& it = tileset.animationsMap.find(viewTile.id);
            
            // Tile has no animations. Continue on to the next tile.
            if (it == tileset.animationsMap.end())
                continue;
                
            const auto& animation = it->second;
            viewTile.frameIdx = animation.currFrameIdx;
        }
    }

    // Update Entity animations
    {
        for (auto& pEntity : m_state.m_entities)
        {
            const auto& tileset = m_state.worldMap.m_tilesetMap[pEntity->tilesetId];
            const auto& it = tileset.animationsMap.find(pEntity->animId);

            if (it == tileset.animationsMap.end())
                continue;
                
            const auto& animation = it->second;
            pEntity->frameIdx = animation.frames[animation.currFrameIdx].id;
        }
    }
}

void GameApp::Loop()
{
    LOGINFO("Game::Loop() - Running...");

    static const uint TARGET_FPS = 60.0;
    static const uint MAX_FRAME_TIME = 1000.0 / TARGET_FPS;

    m_rtClock.Start();
    m_running = true;
    while (m_running)
    {
        HandlePlatformEvents();
        Update();

        m_pRenderer->FillRect(m_viewRect, Colour4i::Black());
        m_gameRenderer.Render(m_pRenderer, m_state, m_textureMap);

        // Render any active views
        for (const auto& pView : m_activeViews)
        {   
            pView->Render(m_pRenderer);
        }

        m_pRenderer->Present();

        m_rtClock.Update();

        Platform::Sleep(m_rtClock.Delay());
    }
}
