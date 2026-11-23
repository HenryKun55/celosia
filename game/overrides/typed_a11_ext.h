// typed_a11_ext.h: round A11's typed calls of the matrix stack (docs/decomp_a11.md; tools/typed_lift2.py A11_EXT).
// iosPushCurrentMatrix (0x1B8638: vsqi vf1..vf4 at (vi15++)) and iosPopCurrentMatrix (0x1B8650: vlqd vf4..vf1 at (--vi15)) as
// functions of values: the current matrix's four rows are the caller's VU0 locals, vi15 a slot of the VU0 interface (the
// adapters load it from ctx->vi[15] and store it back), and the stack itself is VU0's data memory, the same bytes the recompiled
// functions use: rt->memory().getVU0Data() (on a worker shell, that worker's own copy, as the recompiled text's runtime there).
// Every access is the recompiled text's: the quadword index vi15 & 0xFF (16 bytes each, 4 KB), the 16-bit vi15 stepped by one
// per register, all four lanes moved whole (the dest mask xyzw). So typed and untyped callers interleave exactly: a lifted
// function may push and a typed one pop, or the other way round.
//
// pushCore / popCore are the one compiled copy (out of line): the typed callers, the offline harness and A11's slot check
// (A11_NCHK, against the recompiled functions on the same entry state) all run them. The check translation unit
// (A11_CHECK_TU) has its own a11x on the check's logged memory (typed_a11_check_rt.h), around the same cores.
#pragma once

#include "typed_a4_rt.h"

#include <cstdint>
#include <cstring>

class PS2Runtime;

namespace a11x
{
    // vi15 & 0xFF selects the quadword (VU0's data memory: 4 KB, 256 quadwords)
    inline uint32_t vuOff(uint16_t vi) { return (static_cast<uint32_t>(vi) & 0xFFu) << 4; }
    // vf1..vf4 into vu0 at vi15, vi15 += 4 (one register a step: four separate 16-bit increments, as the four vsqi)
    __attribute__((noinline)) void pushCore(uint8_t *vu0, uint16_t &vi15, const a4::vec4 &v1, const a4::vec4 &v2, const a4::vec4 &v3,
                                            const a4::vec4 &v4);
    // vf4..vf1 from vu0 below vi15, vi15 -= 4
    __attribute__((noinline)) void popCore(const uint8_t *vu0, uint16_t &vi15, a4::vec4 &v1, a4::vec4 &v2, a4::vec4 &v3, a4::vec4 &v4);
    uint8_t *vu0Data(PS2Runtime *rt); // rt->memory().getVU0Data() (typed_a11_rt.cpp: keeps ps2_runtime.h out of the typed headers)
}

#if !defined(A11_CHECK_TU)
namespace a11x
{
    // the typed calls (A11_EXT: no parameters, the VU0 interface vf1..vf4 and vi15 by reference)
    inline void pushMatrix(uint8_t *, PS2Runtime *rt, a4::vec4 &vf1, a4::vec4 &vf2, a4::vec4 &vf3, a4::vec4 &vf4, uint16_t &vi15)
    {
        pushCore(vu0Data(rt), vi15, vf1, vf2, vf3, vf4);
    }
    inline void popMatrix(uint8_t *, PS2Runtime *rt, a4::vec4 &vf1, a4::vec4 &vf2, a4::vec4 &vf3, a4::vec4 &vf4, uint16_t &vi15)
    {
        popCore(vu0Data(rt), vi15, vf1, vf2, vf3, vf4);
    }
}
#endif
