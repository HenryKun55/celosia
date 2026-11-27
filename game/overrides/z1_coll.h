// Round Z1 (SOTC_Z1=1, off by default; docs/decomp_z1.md): the collision library the characters call, as typed native C++
// (z1_coll.cpp: the switch, the check, the counts; z1_oneobj.inc, included at the end of decomp_collision.cpp: the
// functions on that file's collision helpers).
#pragma once

#include "ps2_runtime.h"

#include <cstdint>

namespace z1coll
{
    enum Fn : int
    {
        kOneObj,   // ClipCollisionDirectCallBackOneObj (0x112ca30)
        kMultiDep, // ps2__CalcCollisionMultiDepSurfBoundary (0x1124368), the skinned surface's boundary
        kDirect,   // ClipCollisionDirect (0x112ed58), the same tests over an isys group, no filters
        kFnCount
    };
    using Body = bool (*)(uint8_t *, R5900Context *, PS2Runtime *);

    extern signed char g_on;
    void init();
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_on != 0;
    }
    bool partOn(Fn f);
    // Runs `body` (Z1's function; false: its entry test failed, nothing written) as the call, or the check around it
    // (SOTC_Z1_CHECK=1). False: the caller runs the old function.
    bool enter(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void countGuestCall(Fn f);
    void countFallback(Fn f);
    void countInline(Fn f); // a call made inside another Z1 function as host code (counted, never checked on its own)

    // The functions (decomp_collision.cpp, z1_oneobj.inc).
    bool oneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool multiDep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool direct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
