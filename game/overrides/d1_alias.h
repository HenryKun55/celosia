// Round D1 (docs/decomp_d1.md): a D1 native's slot holds its dispatcher (`<fn>Sw`: the D1 body with d1native on, the old
// body otherwise) instead of the old function, so the "does the table hold X" tests (a6Is: the typed adapters' fast
// paths of rounds A6-A14; natives' own slot tests) take the dispatcher as the old function: d1::aliasOf(got) is the old
// function a dispatcher stands for (nullptr for anything else). The pairs are registered in d1_aliases.cpp.
#pragma once

#include "ps2_runtime.h"

#include <cstdint>

namespace d1
{
    using GuestFn = PS2Runtime::RecompiledFunction;
    struct AliasSlot
    {
        GuestFn sw;
        GuestFn old;
    };
    // round D2: 128 -> 256 (D1's 56 + D2's); round D3: 256 -> 512 (D1 + D2 + D3's pairs stay under a third full: aliasOf is on the
    // typed adapters' hot slot tests, and an unsuccessful lookup's linear probe grows with the load)
    inline constexpr uint32_t kAliasSlots = 512u;
    inline AliasSlot g_alias[kAliasSlots] = {};
    inline uint32_t aliasHash(GuestFn f) { return static_cast<uint32_t>((reinterpret_cast<uintptr_t>(f) >> 2) * 2654435761u) >> 23; }
    inline GuestFn aliasOf(GuestFn f)
    {
        for (uint32_t i = aliasHash(f), n = 0; n < kAliasSlots; ++n, i = (i + 1u) & (kAliasSlots - 1u))
        {
            if (g_alias[i].sw == f)
                return g_alias[i].old;
            if (!g_alias[i].sw)
                return nullptr;
        }
        return nullptr;
    }
    // Every pair with its guest address, for the table swap (d1::syncTable, d1_native.cpp): with d1native off the slot holds
    // the old function itself (the off state is main's table exactly: the old bodies' own slot tests, PGO-profiled code), with
    // it on the dispatcher.
    struct AliasEntry
    {
        uint32_t address;
        GuestFn sw;
        GuestFn old;
    };
    inline AliasEntry g_aliasList[kAliasSlots] = {};
    inline uint32_t g_aliasCount = 0;
    struct AliasReg
    {
        AliasReg(uint32_t address, GuestFn sw, GuestFn old)
        {
            if (g_aliasCount < kAliasSlots)
                g_aliasList[g_aliasCount++] = {address, sw, old};
            for (uint32_t i = aliasHash(sw), n = 0; n < kAliasSlots; ++n, i = (i + 1u) & (kAliasSlots - 1u))
                if (!g_alias[i].sw || g_alias[i].sw == sw)
                {
                    g_alias[i] = {sw, old};
                    return;
                }
        }
    };
    // Puts the dispatchers (d1native on) or the old functions (off) in the table where the slot holds the other one (a
    // difftest checker, a profiler wrapper ... is left alone). Called once a frame and at install.
    void syncTable(PS2Runtime *runtime, bool on);
    // Round D3: the difftest's fixed game step (d1_native.cpp). Called at the game thread's frame end.
    void fixedStepFrameEnd(uint8_t *rdram, unsigned long long frame);
} // namespace d1

// After a native's registration line: D1_ALIAS(0x..., <name>, dispatcher, old function).
#define D1_ALIAS(address, name, sw, old) static const ::d1::AliasReg d1_alias_##name{address, sw, old}
