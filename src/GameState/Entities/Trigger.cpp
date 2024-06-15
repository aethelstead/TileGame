#include "Trigger.h"
#include "../../Debug.h"

using namespace Gin;

static std::string GetPropertyValue(const std::map<std::string, std::string>& propertyMap, const std::string& key)
{
    auto s = std::string();

    if (propertyMap.empty() || key.empty())
        return s;

    auto it = propertyMap.find(key);
    bool foundIt = (it != propertyMap.end());
    if (foundIt)
        s = it->second;

    return s;
}

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

void Trigger::FromProto(const EntityProto& proto)
{
    Entity::FromProto(proto);
    type = EntityType::Trigger;

    nextMapName = GetPropertyValue(proto.props, "nextMap");
    auto spawnPosPropVal = GetPropertyValue(proto.props, "spawnPos");
    LOGDEBUG(spawnPosPropVal);
    if (!spawnPosPropVal.empty())
    {
        if (!ParsePoint(spawnPosPropVal, nextMapSpawnPos))
        {
            LOGWARN("Trigger::FromProto() - Failed to parse next map spawn pos property on map trigger entity. Default spawn pos will be used for that map instead.");
        }
        else
        {
            LOGDEBUG("Trigger::FromProto() - Parsed map trigger nextPos attribute: " << nextMapSpawnPos.x << ", " << nextMapSpawnPos.y);
        }
    }
    else
    {
        LOGWARN("Trigger::FromProto() - No next map spawn pos property was specified for map trigger entity. Default spawn pos will be used for that map instead.");
    }
    
}
