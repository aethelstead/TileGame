#include <map>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "GameState.h"
#include "../Assets.h"
#include "../Debug.h"
#include "../Maths/Vector2.h"

using namespace Gin;

void GameState::Init(uint64_t viewWidth, uint64_t viewHeight)
{
    camera.width = viewWidth;
    camera.height = viewHeight;
}

void GameState::ChangeMap(const WorldMap& worldMap, const Vector2f* pSpawnPos)
{
    m_entities.clear();

    this->worldMap = worldMap;

    // Load entities from entity protos
    for (const auto& proto : this->worldMap.m_entityProtos)
    {
        std::shared_ptr<Entity> pEntity = nullptr;
        if (proto.type == EntityType::Player)
        {
            pEntity = std::make_shared<Player>();
            pEntity->FromProto(proto);

            LOGINFO("GameState::ChangeMap() - Created Player from EntityProto");
        }
        else if (proto.type  == EntityType::NPC)
        {
            pEntity = std::make_shared<NPC>();
            pEntity->FromProto(proto);

            LOGINFO("GameState::ChangeMap() - Created NPC from EntityProto");
        }
        else if (proto.type  == EntityType::Trigger)
        {
            pEntity = std::make_shared<Trigger>();
            pEntity->FromProto(proto);

            LOGINFO("GameState::ChangeMap() - Created Trigger from EntityProto");
        }
        pEntity->id = UniqueIDGenerator::Next();

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
    
    camera.Init(m_pPlayer->Box(), this->worldMap.Bounds());

    const int nTiles = worldMap.m_tilemap.m_tileWidth * worldMap.m_tilemap.m_tileHeight;
    m_worldTilesInView.reserve(nTiles);
    LOGDEBUG("GameState::ChangeMap() - Tilemap has " << nTiles << "tiles.");

    LOGDEBUG("GameState::ChangeMap() - WorldMap.Bounds = (0, 0, " << this->worldMap.Bounds().w << ", " << this->worldMap.Bounds().h << ").");
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

    std::vector<Vector2f> collisionDirs;

    if (m_pPlayer->IsMoving())
        m_pPlayer->dir = m_pPlayer->vel;

    // Check player collision with map edge
    {
        if (m_pPlayer->pos.y <= worldMap.Bounds().y)
            collisionDirs.emplace_back(Vector2f::North());
        else if (m_pPlayer->pos.y + m_pPlayer->height >= worldMap.Bounds().h)
            collisionDirs.emplace_back(Vector2f::South());

        if (m_pPlayer->pos.x <= worldMap.Bounds().x)
            collisionDirs.emplace_back(Vector2f::West());
        else if (m_pPlayer->pos.x + m_pPlayer->width >= worldMap.Bounds().w)
            collisionDirs.emplace_back(Vector2f::East());
    }

    // Check player collision with solid tiles

    // Check player collision with other entities
    for (const auto& pEntity : m_entities)
    {
        if (pEntity->type == EntityType::Player)
        {
            // Ignore collision with self
        }
        else if (pEntity->type == EntityType::NPC)
        {
            //const auto& tileset = worldMap.m_tilesetMap[pEntity->tilesetId];
            //auto coll = CheckCollisionDirection(m_pPlayer->Box(), pEntity->Box());
            //collisionDirs.emplace_back(coll);

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
    m_worldTilesInView.clear();

    bool scrollsX = (camera.width < (worldMap.m_tilemap.mapWidth * worldMap.m_tilemap.m_tileWidth));
    bool scrollsY = (camera.height < (worldMap.m_tilemap.mapHeight * worldMap.m_tilemap.m_tileHeight));

    // +1 extra tile X & Y ways for rendering purposes.
    uint64_t maxTilesInViewX = (scrollsX) ? (camera.width / worldMap.m_tilemap.m_tileWidth) + 2 : worldMap.m_tilemap.mapWidth;
    uint64_t maxTilesInViewY = (scrollsY) ? (camera.height / worldMap.m_tilemap.m_tileHeight) + 2 : worldMap.m_tilemap.mapHeight;

    uint64_t startX = (camera.pos.x / worldMap.m_tilemap.m_tileWidth);
    uint64_t startY = (camera.pos.y / worldMap.m_tilemap.m_tileHeight);

    uint64_t endX = startX + maxTilesInViewX;
    uint64_t endY = startY + maxTilesInViewY;
    
    // Clamp the endX/Y indexes to the tilemap width/ height if either goes over.
    if (endX > worldMap.m_tilemap.mapWidth)
        endX = worldMap.m_tilemap.mapWidth;

    if (endY > worldMap.m_tilemap.mapHeight)
        endY = worldMap.m_tilemap.mapHeight;

    for (const auto& layer : worldMap.m_tilemap.m_layers)
    {
        for (uint64_t y = startY; y < endY; ++y)
        {
            for (uint64_t x = startX; x < endX; ++x)
            {
                auto tileIdx = (y * worldMap.m_tilemap.mapWidth) + x;
                auto tile = layer.tiles[tileIdx];
                if (tile.tilesetId == 0)
                    continue;

                WorldTile viewTile(tile);

                viewTile.zIdx = layer.zIdx;

                // Set destination rect
                viewTile.box.x = (x * worldMap.m_tilemap.m_tileWidth);
                viewTile.box.y = (y * worldMap.m_tilemap.m_tileHeight);
                viewTile.box.w = worldMap.m_tilemap.m_tileWidth;
                viewTile.box.h = worldMap.m_tilemap.m_tileHeight;

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
