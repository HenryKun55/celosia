// Round Z4 (docs/decomp_z4.md): direct brackets inside the renderer's frame hand-over (SOTC_P2BRACKET >= 3, the Vita
// only; the game thread's alone: ps2x_p2b_enter ignores other threads). Nothing elsewhere.
#pragma once

#include <cstdint>

#if defined(__vita__)
extern "C" bool ps2x_p2b_wanted();
extern "C" int ps2x_p2b_level();
extern "C" int ps2x_p2b_register(const char *name);
extern "C" uint32_t ps2x_p2b_enter(int id);
extern "C" void ps2x_p2b_leave(uint32_t token);
#endif

namespace z4br
{
    enum Id : int
    {
        kHoPre,    // native_renderer::endFrame before the backend (async builds' flush, the hold test)
        kHoLock,   // the GL hand-over: the mutex and waitData (the render thread's wait is inside)
        kHoSwap,   // the GL hand-over: the counters, the swaps, clearFrame
        kHoEvict,  // the GL hand-over: evictKept, trimRecording, the memory log
        kHoNotify, // the GL hand-over: the render thread woken, wait143's account
        kCount
    };
#if defined(__vita__)
    inline int g_id[kCount] = {-2, -2, -2, -2, -2};
    inline int id(Id which)
    {
        if (__builtin_expect(g_id[0] == -2, 0))
        {
            static const char *const kNames[kCount] = {"frame:z4HoPre", "frame:z4HoLock", "frame:z4HoSwap", "frame:z4HoEvict", "frame:z4HoNotify"};
            const bool on = ps2x_p2b_wanted() && ps2x_p2b_level() >= 3;
            for (int i = 0; i < kCount; ++i)
                g_id[i] = on ? ps2x_p2b_register(kNames[i]) : -1;
        }
        return g_id[which];
    }
    struct Part
    {
        uint32_t token;
        explicit Part(Id which) : token(id(which) >= 0 ? ps2x_p2b_enter(id(which)) : 0u) {}
        void close()
        {
            if (token)
                ps2x_p2b_leave(token);
            token = 0u;
        }
        ~Part() { close(); }
        Part(const Part &) = delete;
        Part &operator=(const Part &) = delete;
    };
#else
    struct Part
    {
        explicit Part(Id) {}
        void close() {}
    };
#endif
}
