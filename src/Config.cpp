#include <fstream>
#include <sstream>

#include "Config.h"
#include "Debug.h"
#include "Utilities/IniParser.h"

using namespace Gin;

static std::string ReadFileText(const std::string& path)
{
    using namespace std;

    ifstream file(path);
    return string(istreambuf_iterator<char>(file),
                  istreambuf_iterator<char>());
}

static std::string GetPropertyValue(const std::string& propName, const std::map<std::string, std::string>& props)
{
    const auto it = props.find(propName);
    if (it == props.end())
        return std::string();

    std::string propVal = it->second;
    return propVal;
}

bool Config::LoadFromIni(const std::string& iniPath)
{
    *this = DEFAULT_CONFIG;

    auto rawText = ReadFileText(iniPath);
    bool fileReadSuccess = (rawText.length() > 0);
    if (!fileReadSuccess)
        return false;

    IniParser iniParser;
    const auto props = iniParser.Parse(rawText);

    if (auto propVal = GetPropertyValue("windowTitle", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'windowTitle', from " << iniPath);
        this->windowTitle = propVal;
    }

    if (auto propVal = GetPropertyValue("windowWidth", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'windowWidth', from " << iniPath);
        this->windowWidth = std::atoi(propVal.c_str());
    }

    if (auto propVal = GetPropertyValue("windowHeight", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'windowHeight', from " << iniPath);
        this->windowHeight = std::atoi(propVal.c_str());
    }

    if (auto propVal = GetPropertyValue("windowFullscreen", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'windowFullscreen', from " << iniPath);
        this->windowFullscreen = (propVal == "true" ? true : false);
    }

    if (auto propVal = GetPropertyValue("initMapName", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'initMapName', from " << iniPath);
        this->initMapName = propVal;
    }

    if (auto propVal = GetPropertyValue("vsync", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'vsync', from " << iniPath);
        this->vsync = (propVal == "true" ? true : false);
    }

    if (auto propVal = GetPropertyValue("fps", props); !propVal.empty())
    {
        LOGDEBUG("Config::LoadFromIni() - Applying config property: 'fps', from " << iniPath);
        this->fps = std::atoi(propVal.c_str());
    }

    return true;
}
