// The game thread's time by subsystem (SOTC_SUBSYS_PROF=1, Debug
// 'Subsystem timing log'): guest functions wrapped in the function table,
// the sub-threads' steps (isysWakeupSubThread) by task, and scopes in our
// own hooks. Inclusive and self time, calls; one [prof] line every 120 game
// frames, in ms per frame. With it, a one-shot [probe] of the machine at
// start: memory latency, guest RAM reads, cold calls into the generated code.
//
// Off (no SOTC_SUBSYS_PROF at start): nothing is wrapped, a scope is one
// load and a branch.
#pragma once

#include <cstdint>

class PS2Runtime;

namespace sotc_prof
{
    // Timing on (installed and the tuning toggle on).
    extern bool g_on;

    // Scopes in our hooks (sotc_native.cpp).
    enum Hook : int
    {
        kHookModelDisp, // modelDispHook's host part
        kHookState,     // modelDispState (the game's modelDisp state, natively)
        kHookPartTest,  // the parts against the camera
        kHookPalette,   // readPalette
        kHookPackets,   // the packet loop (self: draws, lookups)
        kHookPacketId,  // packetId
        kHookBuild,     // a packet's mesh built on the game thread
        kHookSkin,      // a skinned packet posed (palette / CPU pose)
        kHookCount
    };

    uint64_t ticks();
    // A timed region opened (its serial, 0: none) and closed.
    uint32_t enter(int probe, uint64_t now);
    void leave(uint32_t serial, uint64_t now);

    struct Scope
    {
        uint32_t serial = 0;
        explicit Scope(int hook)
        {
            if (g_on)
                serial = enter(hook, ticks());
        }
        ~Scope()
        {
            if (serial)
                leave(serial, ticks());
        }
        Scope(const Scope &) = delete;
        Scope &operator=(const Scope &) = delete;
    };

    // Wraps the functions (last of the overrides: it wraps what is there).
    void install(PS2Runtime &runtime);
    // A game frame ended (sotc_native syncFrame): the report every 120.
    void frameTick(uint64_t frame);
}

#define SOTC_PROF_SCOPE(hook) const ::sotc_prof::Scope sotc_prof_scope_##hook{::sotc_prof::hook}
