#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdint>

namespace Gin
{
    struct Config
    {
        std::string windowTitle;
        uint windowWidth = 0;
        uint windowHeight = 0;
        bool windowFullscreen = false;
        std::string initMapName;
        bool vsync = false;
        uint fps = 0;

        bool LoadFromIni(const std::string& iniPath);
    };

    inline Config DEFAULT_CONFIG = 
    { 
        .windowTitle = "TileGame",
        .windowWidth = 1280,
        .windowHeight = 800,
        .windowFullscreen = false,
        .initMapName = "overworld",
        .vsync = true,
        .fps = 60
    };
}

#endif
