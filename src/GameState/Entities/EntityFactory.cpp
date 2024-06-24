#include <map>
#include <string>

#include "EntityFactory.h"
#include "../../Debug.h"

using namespace Gin;

// @TODO: Duplicate, move somewhere else
static std::string GetPropertyValue(const std::map<std::string, std::string>& propertyMap, const std::string& key)
{
    auto s = std::string();

    auto it = propertyMap.find(key);
    bool foundIt = (it != propertyMap.end());
    if (foundIt)
        s = it->second;

    return s;
}

// @TODO: Duplicate, move somewhere else
static bool ParsePoint(const std::string& s, Vector2f& v)
{
    if (s.empty())
        return false;

    auto xPos = s.find('x');
    if (xPos == s.npos)
        return false;

    auto beforeStr = s.substr(0, xPos);
    auto afterStr = s.substr(xPos + 1);

    v.x = std::atoi(beforeStr.c_str());
    v.y = std::atoi(afterStr.c_str());

    return true;
}

std::shared_ptr<Entity> EntityFactory::CreateEntity(const Tiled::Object& obj) const
{
    std::shared_ptr<Entity> pEntity;

    auto typeVal = GetPropertyValue(obj.props, "type");

    if (typeVal == "player")
    {
        pEntity = CreatePlayer(obj);

        LOGDEBUG("EntityFactory::CreateFromTiledObject() - Created Player from Tiled::Object.");
    }
    else if (typeVal == "npc")
    {
        pEntity = CreateNPC(obj);

        LOGDEBUG("EntityFactory::CreateFromTiledObject() - Created NPC from Tiled::Object.");
    }
    else if (typeVal == "trigger")
    {
        pEntity = CreateTrigger(obj);

        LOGDEBUG("EntityFactory::CreateFromTiledObject() - Created Trigger from Tiled::Object.");
    }
    else
    {
        LOGERROR("EntityFactory::CreateFromTiledObject() - Failed to create entity. Tiled::Object type was missing or not recognised.");
    }

    return pEntity;
}

std::shared_ptr<Player> EntityFactory::CreatePlayer(const Tiled::Object& obj) const
{
    std::shared_ptr<Player> pPlayer = std::make_shared<Player>(UniqueIDGenerator::Next(), obj.tilesetId, obj.pos, obj.width, obj.height, obj.zIdx);
    return pPlayer;
}

std::shared_ptr<NPC> EntityFactory::CreateNPC(const Tiled::Object& obj) const
{
    std::shared_ptr<NPC> pNPC = std::make_shared<NPC>(UniqueIDGenerator::Next(), obj.tilesetId, obj.pos, obj.width, obj.height, obj.zIdx);
    return pNPC;
}

std::shared_ptr<Trigger> EntityFactory::CreateTrigger(const Tiled::Object& obj) const
{
    std::shared_ptr<Trigger> pTrigger = std::make_shared<Trigger>(UniqueIDGenerator::Next(), obj.tilesetId, obj.pos, obj.width, obj.height, obj.zIdx);
    return pTrigger;
}
