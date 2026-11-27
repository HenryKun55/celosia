#pragma once
// The curve decoder's out-registers (decomp_curve.cpp), shared with decomp_gaze.cpp.
// (One definition: the two sources may share a unity batch.)
#include "d1_native.h"

namespace decomp_curve
{
    namespace d2c
    {
        // What the decoder leaves: f0, fcr31, vf1 (= a2's 128 bits after mfc1 a2 / qmtc2 a2), vf7, Q, the status, v0.
        struct CurveRegs
        {
            float f0;
            uint32_t fcr31;
            __m128 vf1, vf7, a2q;
            float q;
            uint16_t status;
            uint64_t v0;
            bool setF0, vu1, vu7;
        };
    }
}
