#ifndef RUNTIME_CLOCK_H
#define RUNTIME_CLOCK_H

#include <cstdint>

#include "../Platform/Platform.h"

namespace Gin
{
    // @TODO: Use std::chrono instead of SDL_GetTicks ? Or SDL_GetPerformanceCounter

    struct RuntimeClock
    {
        // @TODO: Vsync???
        const uint64_t TARGET_FPS = 60;
        const uint64_t ONE_SECOND = 1000;
        const uint64_t MAX_DELTA = (ONE_SECOND / TARGET_FPS);

        uint64_t m_startTicks = 0;
        uint64_t m_currTicks = 0;
        // Elapsed time in milliseconds
        uint64_t m_elapsed = 0;
        uint64_t m_frameCount = 0;

        uint64_t m_fps = 0;

        uint64_t m_delay = 0;

        void Start() { m_startTicks = Platform::Ticks64(); }

        uint64_t Delay()   { return m_delay; }
        uint64_t FPS()     { return (Seconds() > 0) ?  : TARGET_FPS; }
        uint64_t Seconds() { return (m_elapsed / ONE_SECOND); }
        uint64_t Milliseconds() { return m_elapsed; }

        void Update()
        {
            m_currTicks = Platform::Ticks64();
            m_elapsed += (m_currTicks - m_startTicks);
            ++m_frameCount;
            m_startTicks = m_currTicks;

            //m_fps = (m_frameCount / Seconds());

            const uint64_t delta = ((m_currTicks - m_startTicks) / ONE_SECOND);
            m_delay = ((MAX_DELTA - delta) >= 0) ? (MAX_DELTA - delta) : 0;
        }
    };
}

#endif
