#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <map>
#include <string>

namespace Gin
{
    class IniParser
    {
    public:
        std::map<std::string, std::string> Parse(const std::string& rawText);

    private:
        bool ParseProperty(std::string& rawLine, std::string& keyText, std::string& valueText);
    };
}

#endif
