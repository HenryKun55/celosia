// Round W1 (docs/decomp_w1.md): the display walk Vita-style (SOTC_W1=1, off by default; one switch for the console's A/B).
//
// The display walk (DispDispObjGroupModel's tree) is the game thread's heaviest subsystem on the console (P3's brackets:
// 3.9 M cycles a frame, 8.8 ms). W1 changes, all exact (the same guest reads' values, the same writes, the same draws):
//  - the group as a host scene (round C2's scene list, on with W1, prefetch level 2): round 140's clip helper takes the
//    last walk's objects from a dense host array instead of a guest chain walk at the walk's start; the group pass,
//    InitLayoutObjMatrix and the light pass ask (PLD) for their next objects' lines from it, independent addresses a few
//    objects ahead (the A9's own prefetcher is off on the Vita);
//  - the draw loop's iosDlLockObj as host code while the walk holds the isys lock (the nested case: no semaphore, no
//    scheduler call): the object's delete lock, its link into the display list's group (isysAddGroupWithLinkParam's
//    free-list pop and list inserts), the group's count, without the three guest calls through the table per draw;
//  - CalcLightLayoutObj's getters (GetLayoutClass's inline cases, GetCharacterClass, GetGeometryPointerObject) as host
//    reads, not guest calls through the table (up to 2 + 4 per attachment step per lit object);
//  - the model hook's bone palette (readChainPaletteLean): one pass with a bulk copy per UNPACK, no zero fill when the
//    UNPACKs cover the range, no per-qword bounds test (the same floats).
// SOTC_W1_CHECK=1: the palette compared against the old copy on every call; [w1] counts every 600 walks (SOTC_W1_LOG=0:
// none). The draw-stream check of the whole switch: SOTC_W1DRAWS=<file> (a hash of each frame's draws, sotc_native.cpp).
#pragma once

#include <cstdint>
#include <cstdlib>

namespace w1
{
    inline signed char g_on = -1, g_check = -1;
    // Read at first use (the Vita's env.txt is read after static init).
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
        {
            const char *e = std::getenv("SOTC_W1");
            g_on = (e && e[0] == '1') ? 1 : 0;
        }
        return g_on != 0;
    }
    // SOTC_W1_PARTS (debugging; default all): bit 0 the host scene, 1 the lock, 2 the light getters, 3 the palette.
    enum : unsigned
    {
        kScene = 1u,
        kLock = 2u,
        kGetters = 4u,
        kPalette = 8u
    };
    inline int g_parts = -1;
    inline bool part(unsigned p)
    {
        if (__builtin_expect(g_parts < 0, 0))
        {
            const char *e = std::getenv("SOTC_W1_PARTS");
            g_parts = e ? static_cast<int>(std::strtoul(e, nullptr, 0) & 15u) : 15;
        }
        return on() && (static_cast<unsigned>(g_parts) & p) != 0u;
    }
    inline bool checkOn()
    {
        if (__builtin_expect(g_check < 0, 0))
        {
            const char *e = std::getenv("SOTC_W1_CHECK");
            g_check = (e && e[0] == '1') ? 1 : 0;
        }
        return g_check != 0;
    }
    struct Stats
    {
        uint64_t lockHost = 0, lockGuest = 0, lockChecks = 0, lockDiffer = 0;
        uint64_t palHost = 0, palChecks = 0, palDiffer = 0, palBytes = 0;
        uint64_t lightGetters = 0, lightGuest = 0, lightChecks = 0, lightDiffer = 0;
        uint64_t walks = 0;
    };
    inline Stats g_st;
    void report(); // a [w1] line every 600 walks (decomp_walk.cpp)
}
