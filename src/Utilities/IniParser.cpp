#include <vector>
#include <sstream>

#include "IniParser.h"

using namespace Gin;

std::map<std::string, std::string> IniParser::Parse(const std::string& rawText)
{
    std::stringstream ss(rawText);
    std::string line;
    std::vector<std::string> lines;

    // @TODO: Generic string tokenizer function???
    // Break the raw INI text up into lines
    while (std::getline(ss, line, '\n'))
        lines.push_back(line);

    std::map<std::string, std::string> iniProps;
    for (const auto& line : lines)
    {
        std::string keyText;
        std::string valueText;

        if (!ParseProperty(line, keyText, valueText))
        {
            // @TODO: Log a warning to say that we couldn't parse the property
            continue;
        }
        
        iniProps[keyText] = valueText;
    }

    return iniProps;
}

bool IniParser::ParseProperty(const std::string& rawText, std::string& keyText, std::string& valueText)
{
    // Find the index of the equals symbol in the text. Refuse to parse if not found.
    auto equalsSymbolIdx = rawText.find("=");
    if (equalsSymbolIdx == rawText.npos)
        return false;

    // Extract the key text and value text from the raw text.
    keyText = rawText.substr(0, equalsSymbolIdx);
    valueText = rawText.substr(equalsSymbolIdx + 1);

    return true;
}
