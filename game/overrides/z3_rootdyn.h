// Round Z3 (SOTC_Z3=1, off by default; docs/decomp_z3.md): the characters' root-dynamics helpers under xz as typed
// native C++ (z3_rootdyn.cpp: the switch, the check, the counts; z3_funcs.inc, included at the end of
// decomp_collision.cpp: the functions on that file's helpers).
#pragma once

#include "ps2_runtime.h"

#include <cstdint>

namespace z3rd
{
    enum Fn : int
    {
        kSlide,      // charrootdyn_slidecollision (0x10dc020)
        kSlideCB,    // charrootdyn_slidecollisionCB (0x10d6e30), the slide collision's body
        kParentGeo,  // charrootdyn_updateCharacterClassParentGeometry (0x10df218)
        kLayoutGeo,  // UpdateLayoutClassParentGeometry (0x10f22b8)
        kGeoObj,     // GetGeometryPointerObject (0x11329c0)
        kSurfAttr,   // GetGeometryPointerSurfaceAttribute (0x1132af0)
        kDBAttr,     // GetCollisionDBAttribute (0x1125a08)
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
    // Runs `body` (Z3's function; false: its entry test failed, nothing written) as the call, or the check around it
    // (SOTC_Z3_CHECK=1). False: the caller runs the old function.
    bool enter(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void countGuestCall(Fn f);
    void countInline(Fn f); // a call made inside another Z3 function as host code (counted, never checked on its own)

    // The functions (decomp_collision.cpp, z3_funcs.inc).
    bool slide(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool slideCB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool parentGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool layoutGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool geoObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool surfAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool dbAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

// The hook at a lift's entry (after its resume-pc test).
#define SOTC_Z3_HOOK(fn, body)                                                                                              \
    if (__builtin_expect(::z3rd::on(), 0) && ::z3rd::partOn(::z3rd::fn) && ::z3rd::enter(::z3rd::fn, &::z3rd::body, rdram, ctx, runtime)) \
    {                                                                                                                       \
        return;                                                                                                             \
    }
