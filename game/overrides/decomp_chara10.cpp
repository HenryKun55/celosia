// Decomp D10: hand-written natives of the characters and motion area's hot functions that D7's lifted copies made slower
// (exact; in the function table while SOTC_LIFTD10 is on, swapped with the generated code by the liftd10 A/B, as a lifted
// function: namespace lift_d1 so mix7.py's sampler split finds them). See docs/decomp.md "Decomp D10".
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include "runtime/ps2_memory.h"
#include <ps2_recompiled_functions.h>

#include <cstring>

namespace lift_d1
{
    // nico_horseClothOverSurfDetectFunc (0x13970A0): Agro's cloth's "over the surface" callback. class =
    // GetCharacterClass(a0) (object +8 type 9 or 0x16: object +0x90, else 0); returns 1 when the class byte +0x362 is 0 or
    // the class float +0x10C4 is not above 1.2f (0x3F999999), else 0. As the code leaves them: ra stored at sp - 0x10,
    // v1 = the class, a0 = the result, at = 0x3F999999, f0 = 1.2f and f1 = +0x10C4 (only past the byte test), the
    // condition bit from c.lt.s (only then), v0 = the result. Falls back to the generated code when the jal at 0x13970A8
    // no longer targets GetCharacterClass or the object is out of RAM.
    void nico_horseClothOverSurfDetectFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t object = GPR_U32(ctx, 4);
        if (sotc_decomp::jalTarget(rdram, 0x013970A8u) != 0x010C0A70u || object + 8u >= PS2_RAM_SIZE)
            return sotc_decomp::recompiled(0x013970A0u)(rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x10u;
        const uint64_t ra = GPR_U64(ctx, 31);
        std::memcpy(rdram + (sp & PS2_RAM_MASK), &ra, 8);
        const uint8_t type = rdram[object + 8u];
        const uint32_t cls = (type == 9u || type == 0x16u) ? object + 0x90u : 0u;
        SET_GPR_U64(ctx, 3, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(cls))));
        const int8_t flag = static_cast<int8_t>(rdram[(cls + 0x362u) & PS2_RAM_MASK]);
        uint32_t result = 1u;
        if (flag != 0)
        {
            float f1, f0;
            std::memcpy(&f1, rdram + ((cls + 0x10C4u) & PS2_RAM_MASK), 4);
            const uint32_t k = 0x3F999999u;
            std::memcpy(&f0, &k, 4);
            ctx->f[1] = f1;
            ctx->f[0] = f0;
            SET_GPR_U64(ctx, 1, k);
            const bool lt = FPU_C_OLT_S(f0, f1);
            ctx->fcr31 = lt ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (lt)
                result = 0u;
        }
        SET_GPR_U64(ctx, 4, result);
        SET_GPR_U64(ctx, 2, result);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_LIFT_FUNCTION_D10(0x013970A0u, nico_horseClothOverSurfDetectFunc, lift_d1::nico_horseClothOverSurfDetectFunc,
                              nico_horseClothOverSurfDetectFunc_0x13970a0);
