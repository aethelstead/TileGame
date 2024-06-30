#ifndef COLOUR_H
#define COLOUR_H

#include <cstdint>
#include <string>

#include "../Platform/Platform.h"

namespace Gin
{
    struct Colour4i
    {
        static Platform::PixelFormat* pFormat;

        static Colour4i Black()     { return Colour4i(  0,       0,      0,     0xff    ); };
        static Colour4i White()     { return Colour4i(  0xff,    0xff,   0xff,  0xff    ); };
        static Colour4i Red()       { return Colour4i(  0xff,    0,      0,     0xff    ); };
        static Colour4i Green()     { return Colour4i(  0,       0xff,   0,     0xff    ); };
        static Colour4i Blue()      { return Colour4i(  0,       0,      0xff,  0xff    ); };
        static Colour4i Yellow()    { return Colour4i(  0xff,    0xff,   0,     0xff    ); };
        static Colour4i Magneta()   { return Colour4i(  0xff,    0,      0xff,  0xff    ); };
        static Colour4i Cyan()      { return Colour4i(  0,       0xff,   0xff,  0xff    ); };

        uint8_t m_r = 0;
        uint8_t m_g = 0;
        uint8_t m_b = 0;
        uint8_t m_a = 0;

        Colour4i()
        {}

        Colour4i(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
            : m_r( r )
            , m_g( g )
            , m_b( b )
            , m_a( a )
        {}

        bool operator==(const Colour4i& c) const
        {
            return (this->m_r == c.m_r && this->m_g == c.m_g && this->m_b == c.m_b && this->m_a == c.m_a);
        }

        bool operator!=(const Colour4i& c) const
        {
            return (this->m_r != c.m_r || this->m_g != c.m_g || this->m_b != c.m_b || this->m_a != c.m_a);
        }

        void FromInt32(uint32_t n);
        uint32_t ToInt32();

        std::string ToString();
        void FromString(const std::string& s);

        friend std::ostream& operator<< (std::ostream& os, const Colour4i& colour);
        friend std::istream& operator>> (std::istream& is, Colour4i& colour);
    };
}

#endif
