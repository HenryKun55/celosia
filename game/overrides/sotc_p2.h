// Round P2 (docs/decomp_p2.md): counter brackets (SOTC_P2BRACKET=1) around top-level guest functions; sotc_p2.cpp.
#pragma once

#include <cstdint>

class PS2Runtime;

namespace sotc_p2
{
    // Wraps the functions (from sotc_prof::install, last of the overrides). Nothing unless SOTC_P2BRACKET=1 on the Vita.
    void install(PS2Runtime &runtime);

    // Round W3: brackets opened by native code around a stretch of it (not a table entry). named(): an id when
    // SOTC_P2BRACKET >= level (else -1: open / close do nothing); open() a token (0: not timed), close() it.
    int named(const char *name, int level);
    uint32_t open(int id);
    void close(uint32_t token);
    struct Scope
    {
        uint32_t token;
        explicit Scope(int id) : token(id < 0 ? 0u : open(id)) {}
        ~Scope() { close(token); } // also when a guest unwind passes through
        Scope(const Scope &) = delete;
        Scope &operator=(const Scope &) = delete;
    };

    // Round W1 (docs/decomp_w1.md): brackets opened from native code (parts of a native, natives called directly), with
    // SOTC_P2BRACKET >= 1; the same counters and report lines as the table's. Each is in both halves of an A/B.
    enum Direct : int
    {
        kW1GroupPass, // DispDispObjGroupModel's group pass (the clips, the sort adds)
        kW1CopyList,  // its second sort list's copy
        kW1LightPass, // the light pass (manager_sub_010F4FB8) as the walk calls it
        kW1DrawLoop,  // the draw loop (lock, lights, draw callbacks)
        kW1Palette,   // the model hook's bone palette copy (readChainPaletteLean)
        kW1CalcLight, // CalcLightLayoutObj
        kDirectCount
    };
    extern int g_direct[kDirectCount]; // the bracket ids (-1: not registered)
    uint32_t directEnter(int id);
    void directLeave(uint32_t token);
    struct DirectScope
    {
        uint32_t token;
        explicit DirectScope(int which) : token(g_direct[which] >= 0 ? directEnter(g_direct[which]) : 0u) {}
        void close()
        {
            if (token)
                directLeave(token);
            token = 0u;
        }
        ~DirectScope() { close(); }
        DirectScope(const DirectScope &) = delete;
        DirectScope &operator=(const DirectScope &) = delete;
    };
}
