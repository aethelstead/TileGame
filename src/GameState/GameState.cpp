#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "GameState.h"
#include "../Assets.h"
#include "../Debug.h"
#include "../Maths/Vector2.h"
#include "Entities/EntityFactory.h"

using namespace Gin;

void GameState::Init(uint64_t viewWidth, uint64_t viewHeight)
{
    camera.width = viewWidth;
    camera.height = viewHeight;
}

void GameState::ChangeMap(const Tiled::Map& tiledMap, const Vector2f* pSpawnPos)
{
    m_entities.clear();

    // Load tilemap
    m_tilemap = tiledMap.tilemap;
    
    // Load tilesets
    for (const auto& [id, tileset] : tiledMap.tilesetMap)
    {
        m_tilesetMap.try_emplace(id, tileset);
    }

    // Load entities from Tiled objects
    EntityFactory entityFactory;
    for (const auto& obj : tiledMap.objects)
    {
        auto pEntity = entityFactory.CreateEntity(obj);
        m_entities.emplace_back(pEntity);
    }

    // Get pointers to the player, NPCs and Triggers and add to GameState
    for (auto& pEntity : m_entities)
    {
        if (pEntity->type == EntityType::Player)
            m_pPlayer = dynamic_cast<Player*>(pEntity.get());
        else if (pEntity->type == EntityType::NPC)
            m_npcs.emplace_back(dynamic_cast<NPC*>(pEntity.get()));
        else if (pEntity->type == EntityType::Trigger)
            m_triggers.emplace_back(dynamic_cast<Trigger*>(pEntity.get()));
    }

    if (pSpawnPos)
    {
        LOGDEBUG("GameState::ChangeMap() - pSpawnPos was specified. Overriding player spawn pos...");
        m_pPlayer->pos.x = pSpawnPos->x;
        m_pPlayer->pos.y = pSpawnPos->y;
    }
    else
    {
        LOGDEBUG("GameState::ChangeMap() - pSpawnPos was NOT specified. Player will spawn at default position for map.");
    }
    
    Rectf bounds(0, 0, tiledMap.tilemap.mapWidth * tiledMap.tilemap.tileWidth, tiledMap.tilemap.mapHeight * tiledMap.tilemap.tileHeight);
    camera.Init(m_pPlayer->Box(), bounds);

    //LOGDEBUG("GameState::ChangeMap() - WorldMap.Bounds = (0, 0, " << this->tiledMap.Bounds().w << ", " << this->worldMap.Bounds().h << ").");
    LOGDEBUG("GameState::ChangeMap() - Camera.Bounds = (0, 0, " << camera.bounds.w << ", " << camera.bounds.h << ").");
    LOGDEBUG("GameState::ChangeMap() - Loaded: " << m_entities.size() << " Entities in total.");
}

static bool CheckCollision(const Rectf& a, const Rectf& b, Vector2f& direction)
{
    bool collision = (a.Left() < b.Right() && a.Right() > b.Left() && a.Top() < b.Bottom() && a.Bottom() > b.Top());
    if (!collision)
        return false;

    Vector2f c;
    c.x = a.Left() - b.Left();
    c.y = a.Top() - b.Top();
    if (std::abs(c.x) > std::abs(c.y))
    {
        if (c.x < 0)
            direction = Vector2f::East();
        else if (c.x > 0)
            direction = Vector2f::West();
    }
    else
    {
        if (c.y < 0)
            direction = Vector2f::South();
        else if (c.y > 0)
            direction = Vector2f::North();
    }

    return collision;
}

void GameState::Update(double dt, bool& mapChange, std::string& nextMapName, Vector2f& spawnPos)
{
    UpdateWorldTilesInView();

    // @TODO: Make this a std::set instead
    std::vector<Vector2f> collisionDirs;

    if (m_pPlayer->IsMoving())
        m_pPlayer->dir = m_pPlayer->vel;

    // Check player collision with map edge
    {
        /*
        if (m_pPlayer->pos.y <= worldMap.Bounds().y)
            collisionDirs.emplace_back(Vector2f::North());
        else if (m_pPlayer->pos.y + m_pPlayer->height >= worldMap.Bounds().h)
            collisionDirs.emplace_back(Vector2f::South());

        if (m_pPlayer->pos.x <= worldMap.Bounds().x)
            collisionDirs.emplace_back(Vector2f::West());
        else if (m_pPlayer->pos.x + m_pPlayer->width >= worldMap.Bounds().w)
            collisionDirs.emplace_back(Vector2f::East());*/
    }

    // @TODO: This assumes the player only has one collideable
    const auto& playerTileset = m_tilesetMap[m_pPlayer->tilesetId];
    const auto& playerBoxes = playerTileset.boxesMap.at(0);
    auto playerBox = playerBoxes.front();
    playerBox.x = m_pPlayer->pos.x + playerBox.x;
    playerBox.y = m_pPlayer->pos.y + playerBox.y;

    // Check player collision with solid tiles
    {
        for (const auto& worldTile : m_worldTilesInView)
        {
            for (const auto& box : worldTile.boxes)
            {
                Vector2f collDir;
                if (CheckCollision(playerBox, box, collDir))
                {
                    collisionDirs.emplace_back(collDir);
                }
            }
        }
    }

    // Check player collision with other entities
    for (const auto& pEntity : m_entities)
    {
        if (pEntity->type == EntityType::NPC)
        {
            const auto& tileset = m_tilesetMap[pEntity->tilesetId];
            const auto& it = tileset.boxesMap.find(0);
            // NPC has no collision boxes specified so is it's not a collideable-entity. Continue on.
            if (it == tileset.boxesMap.end())
                continue;

            const auto& boxes = it->second;
            for (const auto& box : boxes)
            {
                Rectf worldBox(pEntity->pos.x + box.x, pEntity->pos.y + box.y, box.w, box.h);

                Vector2f collDir;
                if (CheckCollision(playerBox, worldBox, collDir))
                {
                    collisionDirs.emplace_back(collDir);
                }
            }

        }
        else if (pEntity->type == EntityType::Trigger)
        {
            Vector2f coll;
            if (CheckCollision(m_pPlayer->Box(), pEntity->Box(), coll))
            {
                const auto pTrigger = dynamic_cast<Trigger*>(pEntity.get());
                mapChange = true;
                nextMapName = pTrigger->nextMapName;
                spawnPos = pTrigger->nextMapSpawnPos;
            }
        }
    }
    
    // Zero-out the relevant velocity if there was a collision
    for (auto& collisionDir : collisionDirs)
    {
        if (collisionDir == Vector2f::North() && m_pPlayer->vel.y < 0)
            m_pPlayer->vel.y = 0;
        else if (collisionDir == Vector2f::South() && m_pPlayer->vel.y > 0)
            m_pPlayer->vel.y = 0;

        if (collisionDir == Vector2f::West() && m_pPlayer->vel.x < 0)
            m_pPlayer->vel.x = 0;
        else if (collisionDir == Vector2f::East() && m_pPlayer->vel.x > 0)
            m_pPlayer->vel.x = 0;
    }

    // Set player animation
    {
        if (m_pPlayer->attack)
        {
            if (m_pPlayer->dir == Vector2f::North())
                m_pPlayer->animId = (uint)PlayerAnimationType::AttackNorth;
            else if (m_pPlayer->dir == Vector2f::East())
                m_pPlayer->animId = (uint)PlayerAnimationType::AttackEast;
            else if (m_pPlayer->dir == Vector2f::South())
                m_pPlayer->animId = (uint)PlayerAnimationType::AttackSouth;
            else if (m_pPlayer->dir == Vector2f::West())
                m_pPlayer->animId = (uint)PlayerAnimationType::AttackWest;
        }
        else if (m_pPlayer->IsMoving())
        {
            if (m_pPlayer->dir == Vector2f::North())
                m_pPlayer->animId = (uint)PlayerAnimationType::WalkNorth;
            else if (m_pPlayer->dir == Vector2f::East())
                m_pPlayer->animId = (uint)PlayerAnimationType::WalkEast;
            else if (m_pPlayer->dir == Vector2f::South())
                m_pPlayer->animId = (uint)PlayerAnimationType::WalkSouth;
            else if (m_pPlayer->dir == Vector2f::West())
                m_pPlayer->animId = (uint)PlayerAnimationType::WalkWest;
        }
        else 
        {
            if (m_pPlayer->dir == Vector2f::North())
                m_pPlayer->animId = (uint)PlayerAnimationType::IdleNorth;
            else if (m_pPlayer->dir == Vector2f::East())
                m_pPlayer->animId = (uint)PlayerAnimationType::IdleEast;
            else if (m_pPlayer->dir == Vector2f::South())
                m_pPlayer->animId = (uint)PlayerAnimationType::IdleSouth;
            else if (m_pPlayer->dir == Vector2f::West())
                m_pPlayer->animId = (uint)PlayerAnimationType::IdleWest;
        }
    }

    m_pPlayer->pos += (m_pPlayer->vel * m_pPlayer->speed);
    camera.Update(m_pPlayer->Box());
}

void GameState::UpdateWorldTilesInView()
{
    // @TODO: Reserve space for world tiles in view ahead of time

    m_worldTilesInView.clear();

    bool scrollsX = (camera.width < (m_tilemap.mapWidth * m_tilemap.tileWidth));
    bool scrollsY = (camera.height < (m_tilemap.mapHeight * m_tilemap.tileHeight));

    // +1 extra tile X & Y ways for rendering purposes.
    uint64_t maxTilesInViewX = (scrollsX) ? (camera.width / m_tilemap.tileWidth) + 2 : m_tilemap.mapWidth;
    uint64_t maxTilesInViewY = (scrollsY) ? (camera.height / m_tilemap.tileHeight) + 2 : m_tilemap.mapHeight;

    uint64_t startX = (camera.pos.x / m_tilemap.tileWidth);
    uint64_t startY = (camera.pos.y / m_tilemap.tileHeight);

    uint64_t endX = startX + maxTilesInViewX;
    uint64_t endY = startY + maxTilesInViewY;
    
    // Clamp the endX/Y indexes to the tilemap width/ height if either goes over.
    if (endX > m_tilemap.mapWidth)
        endX = m_tilemap.mapWidth;

    if (endY > m_tilemap.mapHeight)
        endY = m_tilemap.mapHeight;

    for (const auto& layer : m_tilemap.layers)
    {
        for (uint64_t y = startY; y < endY; ++y)
        {
            for (uint64_t x = startX; x < endX; ++x)
            {
                auto tileIdx = (y * m_tilemap.mapWidth) + x;
                auto tile = layer.tiles[tileIdx];
                if (tile.tilesetId == 0)
                    continue;

                WorldTile viewTile(tile);

                viewTile.zIdx = layer.zIdx;

                // Set destination rect
                viewTile.box.x = (x * m_tilemap.tileWidth);
                viewTile.box.y = (y * m_tilemap.tileHeight);
                viewTile.box.w = m_tilemap.tileWidth;
                viewTile.box.h = m_tilemap.tileHeight;

                const auto& tileset = m_tilesetMap[tile.tilesetId];
                const auto& it = tileset.boxesMap.find(tile.id);
                if (it != tileset.boxesMap.end())
                {
                    const auto& boxes = it->second;
                    for (const auto& box : boxes)
                    {
                        Rectf worldBox;
                        
                        // Adjust from local tileset space to world space
                        worldBox.x = viewTile.box.x + box.x;
                        worldBox.y = viewTile.box.y + box.y;

                        worldBox.w = box.w;
                        worldBox.h = box.h;

                        viewTile.boxes.emplace_back(worldBox);
                    }
                }

                m_worldTilesInView.emplace_back(viewTile);
            }
        }
    }
}

void GameState::PlayerWalk(const Direction& direction)
{
    switch (direction) 
    {
        case Direction::North:
            m_pPlayer->vel = Vector2f::North();
            break;
        case Direction::East:
            m_pPlayer->vel = Vector2f::East();
            break;
        case Direction::South:
            m_pPlayer->vel = Vector2f::South();
            break;
        case Direction::West:
            m_pPlayer->vel = Vector2f::West();
            break;
    }
}

void GameState::PlayerStopWalk(const Direction& direction)
{
    switch (direction)
    {
        case Direction::North:
            if (m_pPlayer->vel.y < 0)
                m_pPlayer->vel.y = 0;
            break;
        case Direction::East:
            if (m_pPlayer->vel.x > 0)
                m_pPlayer->vel.x = 0;
            break;
        case Direction::South:
            if (m_pPlayer->vel.y > 0)
                m_pPlayer->vel.y = 0;
            break;
        case Direction::West:
            if (m_pPlayer->vel.x < 0)
                m_pPlayer->vel.x = 0;
            break;
    }
}

void GameState::PlayerAttack()
{
    if (m_pPlayer->ready)
    {
        m_pPlayer->attack = true;
        m_pPlayer->ready = false;
    }
}
