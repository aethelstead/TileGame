#include <algorithm>

#include "EntityType.h"

static void to_lower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c)
        {
            return std::tolower(c);
        });
}

Gin::EntityType Gin::EntityTypeFromString(std::string name)
{
    EntityType type = EntityType::None;

    if (!name.empty())
    {
        to_lower(name);

        if (name == "player")
            type = EntityType::Player;
        else if (name == "npc")
            type = EntityType::NPC;
        else if (name == "trigger")
            type = EntityType::Trigger;
    }

    return type;
}
