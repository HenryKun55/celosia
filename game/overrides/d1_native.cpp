// Round D1 (d1_native.h): the run counts of the D1 bodies.
#include "d1_native.h"
#include "runtime/ps2_diag.h" // round F1: debug / player switches through ps2_diag::env

#include <cstdio>

namespace d1
{
    static Counter *g_counters = nullptr;
    Counter::Counter(const char *nm) : name(nm), next(g_counters) { g_counters = this; }

    void report(unsigned long long frame)
    {
        uint64_t total = 0;
        int n = 0;
        for (const Counter *c = g_counters; c; c = c->next)
            total += c->n, ++n;
        std::fprintf(stderr, "[d1] frame %llu: d1native %d; %d bodies, %llu calls\n", frame, on() ? 1 : 0, n, (unsigned long long)total);
        for (const Counter *c = g_counters; c; c = c->next)
            if (c->n)
                std::fprintf(stderr, "[d1]   %s %llu\n", c->name, (unsigned long long)c->n);
    }
} // namespace d1

#include "d1_alias.h"

namespace d1
{
    void syncTable(PS2Runtime *runtime, bool on)
    {
        static int state = -1;
        if (!runtime || state == (on ? 1 : 0))
            return;
        state = on ? 1 : 0;
        int swapped = 0, other = 0;
        for (uint32_t i = 0; i < g_aliasCount; ++i)
        {
            const AliasEntry &e = g_aliasList[i];
            const GuestFn from = on ? e.old : e.sw, to = on ? e.sw : e.old;
            const GuestFn now = runtime->lookupFunction(e.address);
            if (now == from)
            {
                runtime->registerFunction(e.address, to);
                ++swapped;
            }
            else if (now != to)
                ++other;
        }
        static uint64_t changes = 0;
        if (changes++ < 4 || changes % 1000 == 0)
            std::fprintf(stderr, "[d1] table: d1native %s, %d slots swapped, %d held by something else (change %llu)\n", on ? "on" : "off", swapped, other,
                         (unsigned long long)changes);
    }
} // namespace d1

#include <cstdlib>
#include <cstring>

namespace d1
{
    // Round D3: the difftest's fixed game step. SotC steps its world by the EE clock between frame starts (docs/decomp.md round 10:
    // gamecore_sub_013455F8, capped at 8 VBlanks by the host clock). Under SOTC_DIFFTEST every checked call runs twice on 32 MB
    // copies, the frames take 100-250 ms in Vita3K, and the game takes 133 ms steps: Wander's path out of the shrine and Agro's
    // follow change, and ride7 never mounted under the difftest (D2, also old body against old body). The game's own fixed-step
    // switch (frameRateMode, 0x14659E8: non-zero = the clock is the dummy count 0x14659F0, which gamecore_sub_013454B8 advances by
    // 4 999 999.5 ticks a frame, 1/30 s) gives every frame a normal 1/30 s step whatever the frame took on the host: the route
    // plays like a 30 fps run. SOTC_FIXED_STEP=1 turns it on, =0 off; default on with SOTC_DIFFTEST set, off otherwise. Set once
    // GAMECORE's code is in place (the two code words below), with the dummy count rebased on the last frame's clock (+0x1465650)
    // so the first fixed step is one frame long; set again (rebased) if the game ever clears it (a GAMECORE reload).
    void fixedStepFrameEnd(uint8_t *rdram, unsigned long long frame)
    {
        static const int want = [] {
            const char *e = ps2_diag::env("SOTC_FIXED_STEP");
            if (e)
                return std::atoi(e);
            return ps2_diag::env("SOTC_DIFFTEST") ? 1 : 0;
        }();
        if (!want || !rdram)
            return;
        uint32_t w1, w2, mode;
        std::memcpy(&w1, rdram + 0x01345618u, 4); // lw $v1, 0x59E8($v0) in gamecore_sub_013455F8
        std::memcpy(&w2, rdram + 0x01345510u, 4); // lui $at, 0x4A98 (the step, 4 999 999.5) in gamecore_sub_013454B8
        if (w1 != 0x8C4359E8u || w2 != 0x3C014A98u)
            return;
        std::memcpy(&mode, rdram + 0x014659E8u, 4);
        if (mode != 0u)
            return;
        uint64_t now;
        std::memcpy(&now, rdram + 0x01465650u, 8);
        std::memcpy(rdram + 0x014659F0u, &now, 8);
        mode = 1u;
        std::memcpy(rdram + 0x014659E8u, &mode, 4);
        static unsigned sets = 0;
        if (sets++ < 8u)
            std::fprintf(stderr, "[fixedstep] frame %llu: the game's step fixed at 1/30 s (frameRateMode 1, clock rebased at %llu)\n", frame,
                         (unsigned long long)now);
    }
} // namespace d1
