#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <set>

#include "Config.h"
#include "GameApp.h"
#include "Tiled/Tiled.h"

constexpr uint XRES_INTERNAL = 640;
constexpr uint YRES_INTERNAL = 360;

using namespace Gin;

bool GameApp::Init()
{
    LOGINFO("GameApp::Init() - Initialising...");

    // Load config
    Config config;
    {
        auto iniPath = "config.ini";
        if (config.LoadFromIni(iniPath))
        {
            LOGINFO("GameApp::Init() - Loaded config from '" << iniPath << "'.");
        }
        else
        {
            LOGWARN("GameApp::Init() - Failed to load config from '" << iniPath << "'. Default config will be used instead.");
        }
    }

    m_vsync = config.vsync;
    m_targetFPS = config.fps;
    LOGINFO("GameApp::Init() - FPS = " << m_targetFPS << " (Will be ignored if vsync is enabled)");

    // Init platform, create Window and create Rendererer
    {
        if (!Platform::Init())
        {
            LOGERROR("Platform::Init() - Failed.");
            return false;
        }
        LOGINFO("GameApp::Init() - Platform initialised.");

        m_pWindow = Window::Create(config.windowTitle.c_str(), config.windowWidth, config.windowHeight, config.windowFullscreen);
        if (!m_pWindow)
        {
            LOGERROR("GameApp::Init() - Failed to create platform window.");
            return false;
        }
        LOGINFO("GameApp::Init() - Created Platform Window.");

        m_pRenderer = Renderer::Create(m_pWindow, m_vsync);
        if (!m_pRenderer)
        {
            LOGERROR("GameApp::Init() - Failed to create platform renderer.");
            return false;
        }
        LOGINFO("GameApp::Init() - Created Platform Renderer.");
    }
    
    InitText();

    m_viewRect = Recti(0, 0, XRES_INTERNAL, YRES_INTERNAL);

    // Create a texture to render everything to (rendered at internal resolution then gets upscaled to display resolution)
    m_renderTexture = Texture::Create((uint)m_viewRect.w, (uint)m_viewRect.h, m_pRenderer->GetContext());
    if (!m_renderTexture)
    {
        LOGERROR("GameApp::Init() - Failed to create render texture.");
        return false;
    }

    // Init menus
    {
        Recti screenRect(0, 0, m_pWindow->Width(), m_pWindow->Height());

        // @TODO: Error checking LoadMenu
        // Add all of the required menus to the menu map
        m_viewMap[ GameViewType::Main ] = VGui::View::Create("assets/menu/main.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Save ] = VGui::View::Create("assets/menu/save.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Load ] = VGui::View::Create("assets/menu/load.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Settings ] = VGui::View::Create("assets/menu/settings.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Equipment ] = VGui::View::Create("assets/menu/equipment.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Inventory ] = VGui::View::Create("assets/menu/inventory.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Map ] = VGui::View::Create("assets/menu/map.xml", screenRect, m_pRenderer);
        m_viewMap[ GameViewType::Stats ] = VGui::View::Create("assets/menu/stats.xml", screenRect, m_pRenderer);
    }

    m_state.Init(m_viewRect.w, m_viewRect.h);

    // Load initial game map and change map in GameState
    {
        Tiled::Map tiledMap;
        if (!config.initMapName.empty())
        {
            if (!LoadGameMap(config.initMapName, tiledMap))
            {
                LOGERROR("GameApp::Init() - Initial game map: '" << config.initMapName << "' failed to load.");
                return false;
            }
        }
        else
        {
            LOGWARN("GameApp::Init() - Game map not loaded because no map was specified in config.");
        }
        LOGINFO("GameApp::Init() - Loaded initial game map: " << config.initMapName);
    
        m_state.ChangeMap(tiledMap);
    }

    //m_savedState = m_state;

    LOGINFO("GameApp::Init() - Initialisation successful.");

    return true;
}

bool GameApp::InitText()
{
    if (!m_pRenderer)
    {
        LOGERROR("GameApp::InitText() - Failed to init text. Renderer was NULL.");
        return false;
    }

    // Arial 16
    {
        const std::string fontPath = "assets/font/arial.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::Arial16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // ArialBold 16
    {
        const std::string fontPath = "assets/font/arialbd.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::ArialBold16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // Calibri 16
    {
        const std::string fontPath = "assets/font/calibri.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::Calibri16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // CalibriBold 16
    {
        const std::string fontPath = "assets/font/calibrib.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::CalibriBold16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // Consolas 16
    {
        const std::string fontPath = "assets/font/consola.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::Consolas16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // ConsolasBold 16
    {
        const std::string fontPath = "assets/font/consolab.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::ConsolasBold16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // Verdana 16
    {
        const std::string fontPath = "assets/font/verdana.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::Verdana16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // VerdanaBold 16
    {
        const std::string fontPath = "assets/font/verdanab.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::VerdanaBold16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // Times 16
    {
        const std::string fontPath = "assets/font/times.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::Times16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }

    // TimesBold 16
    {
        const std::string fontPath = "assets/font/timesbd.ttf";
        auto pGlyphsheet = GlyphSheet::Create(fontPath, 16, *m_pRenderer);
        if (pGlyphsheet)
        {
            LOGINFO("GameApp::InitText() - Created GlypSheet for font at: '" << fontPath << "'.");
            m_glyphsheetMap.try_emplace(GameFontType::TimesBold16, std::move(pGlyphsheet));
        }
        else
            LOGWARN("GameApp::InitText() - Failed to create Glyphsheet for font at: '" << fontPath << "'. Any text using this font may not be shown.");
    }
    
    return true;
}

void GameApp::Quit()
{
    //Platform::CloseFont(m_pFont);
    LOGINFO("Game::Quit() - Quitting...");
    SDL_Quit();
}

bool GameApp::LoadGameMap(const std::string& mapName, Tiled::Map& tiledMap)
{
    m_textureMap.clear();
    
    // Get map path from name
    std::string mapPath;
    {
        std::stringstream ss;
        ss << MAP_PATH << mapName << ".tmx";
        mapPath = ss.str();
    }
    LOGINFO("GameApp::LoadGameMap() - Trying to load game map from: " << mapPath << "...");

    if (!Tiled::LoadTiledMapFromFile(mapPath, tiledMap))
        return false;

    // Load texture for each tileset
    std::stringstream ss;
    for (auto& [_, tileset] : tiledMap.tilesetMap)
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
    //m_savedState = m_state;
    LOGINFO("GameApp::QuickSave() - Quick save done.");
}

void GameApp::QuickLoad()
{
    //m_state = m_savedState;
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

void GameApp::Update(double dt)
{
    if (!m_paused)
    {
        bool mapChange = false;
        std::string nextMapName;
        Vector2f spawnPos;
        m_state.Update(dt, mapChange, nextMapName, spawnPos);

        UpdateAnimations(dt);

        if (mapChange)
        {
            Tiled::Map tiledMap;
            if (!LoadGameMap(nextMapName, tiledMap))
            {
                LOGDEBUG("GameApp::Update() - Failed to load new game map.");
                return;
            }

            m_state.ChangeMap(tiledMap, &spawnPos);
        } 
    }
}

void GameApp::UpdateAnimations(double dt)
{
    // @TODO: This seems to be updating all animations regardless of whether or not they are shown

    // Update Tileset animations
    for (auto& [_, tileset] : m_state.m_tilesetMap)
    {
        for (auto& [_, animation] : tileset.animationsMap)
        {
            const auto& currFrame = animation.frames[animation.currFrameIdx];

            animation.nextFrameMs += dt;
            
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
            const auto& tileset = m_state.m_tilesetMap[viewTile.tilesetId];
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
            const auto& tileset = m_state.m_tilesetMap[pEntity->tilesetId];
            const auto& it = tileset.animationsMap.find(pEntity->animId);

            if (it == tileset.animationsMap.end())
                continue;
                
            const auto& animation = it->second;
            pEntity->frameIdx = animation.frames[animation.currFrameIdx].id;
        }
    }
}

void GameApp::Render()
{
    SDL_SetRenderTarget(m_pRenderer->GetContext()->GetInternal(), m_renderTexture->GetInternal());
    m_pRenderer->FillRect(m_viewRect, Colour4i::Black());
    m_gameRenderer.Render(m_pRenderer, m_state, m_textureMap);
    SDL_SetRenderTarget(m_pRenderer->GetContext()->GetInternal(), nullptr);
    Recti dest(0, 0, m_pWindow->Width(), m_pWindow->Height());
    m_pRenderer->Copy(m_renderTexture, m_viewRect, dest);

    // Render any active views
    for (const auto& pView : m_activeViews)
    {   
        pView->Render(m_pRenderer);
    }
    
    RenderDebugOverlay();

    m_pRenderer->Present();
}

void GameApp::RenderDebugOverlay()
{
    // Render frame counter
    {
        std::stringstream ss;
        ss << "Frame Count = " << m_frameCount; 
        std::string s = ss.str().c_str();
    
        Vector2i pos(10, 10);
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Black());
        pos.x+=1;pos.y+=1;
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Yellow());
    }
    // Render FPS
    {
        std::stringstream ss;
        ss << "FPS = " << m_fps; 
        std::string s = ss.str().c_str();
    
        Vector2i pos(10, 30);
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Black());
        pos.x+=1;pos.y+=1;
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Yellow());
    }
    // Render World info
    {
        std::stringstream ss;
        ss << "EntityCount = " << m_state.m_entities.size(); 
        std::string s = ss.str().c_str();
    
        Vector2i pos(10, 50);
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Black());
        pos.x+=1;pos.y+=1;
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Yellow());
    }
    // Render World info
    {
        std::stringstream ss;
        ss << "PlayerWorld = (" << m_state.m_pPlayer->pos.x << ", " << m_state.m_pPlayer->pos.y << ")"; 
        std::string s = ss.str().c_str();
    
        Vector2i pos(10, 70);
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Black());
        pos.x+=1;pos.y+=1;
        RenderText(s, *m_glyphsheetMap[GameFontType::ArialBold16], pos, Colour4i::Yellow());
    }
}

void GameApp::RenderText(std::string_view text, GlyphSheet& glypsheet, const Vector2i& pos, Colour4i colour)
{
    glypsheet.m_pTexture->m_colourMod = colour;

    int ASCII_START = static_cast<int>(' ');

    uint idx = 0;
    Recti src(0, 0, glypsheet.m_fontSize, glypsheet.m_fontSize);
    Recti dst(pos.x, pos.y, glypsheet.m_fontSize, glypsheet.m_fontSize);
    for (const auto& ch : text)
    {
        int cellIdx = (ch - ASCII_START);
        int cellX = (cellIdx % glypsheet.m_cellsPerRow) * glypsheet.m_fontSize;
        int cellY = (cellIdx / glypsheet.m_cellsPerRow) * glypsheet.m_fontSize;
        src.x = cellX;
        src.y = cellY + 2;

        int glyphAdvance = 0;
        if (idx > 0)
        {
            char prev = text.at(idx - 1);
            glyphAdvance = glypsheet.GetGlyphAdvance(prev);
        }
        dst.x += glyphAdvance;

        m_pRenderer->Copy(glypsheet.m_pTexture, src, dst);
        ++idx;
    }
}

void GameApp::Loop()
{
    LOGINFO("Game::Loop() - Running...");

    const uint MAX_DELTA = (1000 / m_targetFPS);

    double dt = 0.0;

    m_running = true;
    while (m_running)
    {
        uint64_t startTicks = Platform::Ticks64();

        HandlePlatformEvents();
        Update(dt);
        
        Render();
        uint64_t currTicks = Platform::Ticks64();

        int64_t delta = (currTicks - startTicks);
        if (delta < 0 || delta > MAX_DELTA)         // @TODO: Revise this. (Why is second condition necessary?)
            delta = 0;

        if (!m_vsync)
        {
            // If vsync is disabled then need to account for the game running too quick
            delta = MAX_DELTA - delta;

            Platform::Sleep(delta);
        }
        m_frameCount++;

        const uint ONE_SEC = 1000;
        if (currTicks > ONE_SEC)
        {
            auto secsPassed = (currTicks / ONE_SEC);
            m_fps = m_frameCount / secsPassed;
        }

        dt = delta;
    }
}
