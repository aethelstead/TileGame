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
        bool windowFullscreen;
        std::string initMapName;

        bool LoadFromIni(const std::string& iniPath);
    };

    inline Config DEFAULT_CONFIG = 
    { 
        .windowTitle = "TileGame",
        .windowWidth = 1280,
        .windowHeight = 800,
        .windowFullscreen = false,
        .initMapName = "overworld"
    };
}

#endif
