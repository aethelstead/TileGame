#include <iomanip>
#include <iostream>
#include <sstream>

#include "Colour.h"

using namespace Gin;

Platform::PixelFormat* Colour4i::pFormat = nullptr;

std::ostream& operator<< (std::ostream& os, const Colour4i& colour)
{
    os << std::setfill('0');

    int r = static_cast<int>(colour.m_r);
    int g = static_cast<int>(colour.m_g);
    int b = static_cast<int>(colour.m_b);
    int a = static_cast<int>(colour.m_a);

    os  << "#" 
        << std::hex << std::setw(2) << r 
        << std::hex << std::setw(2) << g 
        << std::hex << std::setw(2) << b
        << std::hex << std::setw(2) << a;
    return os;
}

std::istream& operator>> (std::istream& is, Colour4i& colour)
{
    /*
    char ch = is.get();
    if (ch != '#')
        return is;

    const uint UINT32_BYTES = 4;
    const uint BUFFER_SIZE = UINT32_BYTES + 1;  // + 1 for null terminator
    char buffer[BUFFER_SIZE] = { is.get(), is.get(), is.get(), is.get(), '\0' };

    std::stringstream ss;
    ss << std::dec << buffer;

    int num = std::atoi(ss.str().c_str());
    uint32_t unum = static_cast<uint32_t>(num);
    colour = (Colour3i)(unum);*/

    return is;
}

void Colour4i::FromInt32(uint32_t n)
{
    Platform::GetRGBA(n, Colour4i::pFormat, m_r, m_g, m_b, m_a);
}

uint32_t Colour4i::ToInt32()
{
    return Platform::MapRGBA(Colour4i::pFormat, m_r, m_g, m_b, m_a);
}

std::string Colour4i::ToString()
{
    std::stringstream ss;
    ss << std::setfill('0');

    int r = static_cast<int>(m_r);
    int g = static_cast<int>(m_g);
    int b = static_cast<int>(m_b);
    int a = static_cast<int>(m_a);

    ss  << "#" 
        << std::hex << std::setw(2) << r 
        << std::hex << std::setw(2) << g 
        << std::hex << std::setw(2) << b 
        << std::hex << std::setw(2) << a;
    
    return ss.str();
}

static uint32_t HexStringToUint32(const std::string& s)
{
    uint x;
    std::stringstream ss;
    ss << std::hex << s;
    ss >> x;
    return x;
}

void Colour4i::FromString(const std::string& s)
{
    // RGB or RGBA strings must be exactly 9 chars long (e.g: #f0f0f0ff)
    const uint RGBA_STRLEN = 9;
    if (s.length() != RGBA_STRLEN)
        return;

    if (s[0] != '#')
        return;

    std::string strR = s.substr(1,2);
    std::string strG = s.substr(3,2);
    std::string strB = s.substr(5,2);
    std::string strA = s.substr(7,2);

    m_r = static_cast<uint8_t>(HexStringToUint32(strR));
    m_g = static_cast<uint8_t>(HexStringToUint32(strG));
    m_b = static_cast<uint8_t>(HexStringToUint32(strB));
    m_a = static_cast<uint8_t>(HexStringToUint32(strA));
}
