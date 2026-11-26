#pragma once
// iosGetScaleMatrixV's core's out-registers (decomp_math.cpp), shared with decomp_layoutsimple.cpp's D3 body.
// (One definition: the two sources may share a unity batch.)
#include "d1_native.h"

namespace decomp_copymatrix
{
    struct D3ScaleMatrixOut
    {
        __m128 rows[4];
        uint32_t f0, f1;
    };
}
