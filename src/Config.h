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
        std::string initMapName;

        bool LoadFromIni(const std::string& iniPath);
    };

    inline Config DEFAULT_CONFIG = 
    { 
        .windowTitle = "TileGame",
        .windowWidth = 640,
        .windowHeight = 480,
        .initMapName = "overworld"
    };
}

#endif
