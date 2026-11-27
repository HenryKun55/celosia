#pragma once
// eulerYXZ's state (decomp_hot44b.cpp), shared with decomp_hot44d.cpp.
// (One definition: the two sources may share a unity batch.)
#include "d1_native.h"

namespace decomp_hot44b
{
    namespace d1e
    {
        using V4 = __m128;
        struct EulerRegs
        {
            V4 v11, v12, v13, v14, v15, v16, acc;
            float f0, f1, q;
            uint32_t fcr31, g4, g5, g6;
            uint16_t status;
            bool used; // an axis turned (v0 1; at, a1, f0, f1 set)
        };
    }
}
