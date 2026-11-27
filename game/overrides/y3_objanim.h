// Round Y3 (SOTC_Y3=1, off by default; docs/decomp_y3.md): the object animation library as native C++.
// y3_objanim.cpp holds ExecObjectAnimation's native body; the samplers' cores it calls directly live with their old
// bodies (decomp_hot46h.cpp: y3PosKeyLerp / y3KeyVec3, thin entries over the D2 / D3 cores).
#pragma once

#include "ps2_runtime.h"

#include <cstdint>

namespace y3
{
    // The VU0 registers the position lerp and the vec3 blend read (partial-lane writes keep the other lanes) and leave,
    // and t0's 128 bits (vf7 is qmtc2'd from t0: its high half rides along).
    struct Vu
    {
        __m128 vf5, vf6, vf7, vf8, acc, t0;
    };

    bool on();
    // The scripts:y3ObjAnim bracket's id (SOTC_Y3_BRACKET=1 with SOTC_P2BRACKET >= 1; else -1), both halves of an A/B.
    int execBracket();
    // ExecObjectAnimation (0x110a130): false when Y3 does not take this call (the old body runs, nothing written).
    bool execObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
} // namespace y3

namespace decomp_hot46h
{
    // manager_sub_010AEAF8 (posKeyLerp) on Y3's registers: D2's posKeyLerpCore with VU0 / fcr31 in and out; the scratch
    // registers (f0-f7, at, v0, v1) are not kept. The caller has checked posKeyLerpD1's gates (it is the old body's
    // case otherwise).
    void y3PosKeyLerp(uint8_t *rdram, uint32_t out, uint64_t format, uint32_t table, bool hasTable, uint32_t i0, uint32_t i1, float frac, uint32_t sp,
                      y3::Vu &vu, uint32_t &fcr31);
    // manager_sub_010ADE08 (keyVec3): D3's d3Vec3Core (no VU0, no fcr31; scratch registers not kept).
    void y3KeyVec3(uint8_t *rdram, uint32_t out, uint64_t format, uint32_t table, uint32_t index);
} // namespace decomp_hot46h
