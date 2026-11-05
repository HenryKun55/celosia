#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_fog_ptclman
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kFogState = 0x012147D8u;
    constexpr uint32_t kPtclmanState = 0x01215BE8u;

    inline bool on()
    {
        static const bool enabled = !std::getenv("SOTC_FOG_PTCLMAN") || std::atoi(std::getenv("SOTC_FOG_PTCLMAN")) != 0;
        return enabled;
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // getFogFactor0 (0x01089B70)
    void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x01089B70u, rdram, ctx, runtime);
        (void)runtime;
        SET_GPR_U64(ctx, 2, sx(kFogState));
        float rf_2 = fb(READ32(kFogState + 8u));
        const float rf_3 = fb(READ32(kFogState + 4u));
        ctx->f[3] = rf_3;
        const float rf_4 = fb(READ32(kFogState + 16u));
        ctx->f[4] = rf_4;
        float rf_1 = fb(READ32(kFogState + 12u));
        float rf_0 = FPU_MUL_S(rf_2, rf_3);
        rf_2 = FPU_SUB_S(rf_2, rf_3);
        ctx->f[2] = rf_2;
        rf_1 = FPU_SUB_S(rf_1, rf_4);
        ctx->f[1] = rf_1;
        rf_0 = FPU_MUL_SF(rf_0, rf_1);
        rf_0 = ps2FpuDivide(ctx->fcr31, rf_0, rf_2);
        ctx->f[0] = rf_0;
        returnTo(ctx);
    }

    // getFogFactor1 (0x01089BB0)
    void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x01089BB0u, rdram, ctx, runtime);
        (void)runtime;
        SET_GPR_U64(ctx, 2, sx(kFogState));
        float rf_2 = fb(READ32(kFogState + 12u));
        float rf_3 = fb(READ32(kFogState + 16u));
        float rf_1 = fb(READ32(kFogState + 8u));
        const float rf_4 = fb(READ32(kFogState + 4u));
        ctx->f[4] = rf_4;
        float rf_0 = FPU_SUB_S(rf_3, rf_2);
        rf_3 = FPU_ADD_S(rf_3, rf_2);
        ctx->f[3] = rf_3;
        const float rf_5 = fb(READ32(kFogState + 20u));
        ctx->f[5] = rf_5;
        rf_2 = FPU_ADD_S(rf_1, rf_4);
        ctx->f[2] = rf_2;
        rf_1 = FPU_SUB_S(rf_1, rf_4);
        ctx->f[1] = rf_1;
        rf_0 = FPU_MUL_SF(rf_0, rf_2);
        rf_0 = ps2FpuDivide(ctx->fcr31, rf_0, rf_1);
        rf_0 = FPU_ADD_SF(rf_0, rf_3);
        rf_0 = FPU_MUL_S(rf_0, rf_5);
        ctx->f[0] = rf_0;
        returnTo(ctx);
    }

    // ptclmanGet (0x0109A3D8)
    void ptclmanGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A3D8u, rdram, ctx, runtime);
        (void)runtime;
        SET_GPR_U64(ctx, 3, sx(kPtclmanState));
        const uint32_t count = READ32(kPtclmanState + 4u);
        const uint32_t base = READ32(kPtclmanState + 0u);
        SET_GPR_U64(ctx, 4, sx(base));
        SET_GPR_U64(ctx, 2, sx(base + (count << 4)));
        returnTo(ctx);
    }

    // ptclmanPut (0x0109A3F8)
    void ptclmanPut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A3F8u, rdram, ctx, runtime);
        (void)runtime;
        SET_GPR_U64(ctx, 3, sx(kPtclmanState));
        const uint32_t nextCount = READ32(kPtclmanState + 4u) + GPR_U32(ctx, 4);
        SET_GPR_U64(ctx, 2, sx(nextCount));
        WRITE32(kPtclmanState + 4u, nextCount);
        returnTo(ctx);
    }

    // ptclmanBegin (0x0109A1F8)
    void ptclmanBegin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A1F8u, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp, GPR_U64(ctx, 31));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A200u), 0x0109A208u, sp);
        SET_GPR_U64(ctx, 31, READ64(sp));
        SET_GPR_U64(ctx, 3, sx(kPtclmanState));
        WRITE32(kPtclmanState + 0u, GPR_U32(ctx, 2));
        WRITE32(kPtclmanState + 4u, 0u);
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // ptclmanEnd (0x0109A228)
    void ptclmanEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A228u, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp, GPR_U64(ctx, 31));
        callGuest(rdram, ctx, runtime, 0x0109A3D8u, 0x0109A238u, sp);
        SET_GPR_U64(ctx, 3, sx(0x60000000u));
        const uint32_t pkt = GPR_U32(ctx, 2);
        WRITE32(pkt + 0u, 0x60000000u);
        WRITE32(pkt + 12u, 0x13000000u);
        SET_GPR_U64(ctx, 4, 1u);
        WRITE32(pkt + 4u, 0u);
        WRITE32(pkt + 8u, 0u);
        callGuest(rdram, ctx, runtime, 0x0109A3F8u, 0x0109A258u, sp);
        callGuest(rdram, ctx, runtime, 0x0109A3D8u, 0x0109A260u, sp);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A260u), 0x0109A268u, sp);
        SET_GPR_U64(ctx, 2, sx(0x01210000u));
        SET_GPR_U64(ctx, 4, sx(READ32(kPtclmanState)));
        SET_GPR_U64(ctx, 31, READ64(sp));
        SET_GPR_U64(ctx, 29, sx(sp0));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A274u), GPR_U32(ctx, 31), sp0);
    }

    // ptclDisplayListGet (0x0109A1A8)
    void ptclDisplayListGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A1A8u, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 16u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A1B8u), 0x0109A1C0u, sp);
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A1C0u), 0x0109A1C8u, sp);
        const uint32_t s0_32 = GPR_U32(ctx, 16);
        const uint64_t s1_64 = GPR_U64(ctx, 17);
        const uint32_t s1_32 = static_cast<uint32_t>(s1_64);
        const uint64_t v1_64 = sx(s1_32 + (((s0_32 << 1) + s0_32) << 4));
        SET_GPR_U64(ctx, 3, v1_64);
        const bool outOfSpace = GPR_U64(ctx, 2) < v1_64;
        SET_GPR_U64(ctx, 2, outOfSpace ? 0u : s1_64);
        SET_GPR_U64(ctx, 31, READ64(sp + 16u));
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // ptclDisplayListPut (0x0109A170)
    void ptclDisplayListPut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0109A170u, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A17Cu), 0x0109A184u, sp);
        const uint32_t s0_32 = GPR_U32(ctx, 16);
        const uint32_t a0_32 = GPR_U32(ctx, 2) + (((s0_32 << 1) + s0_32) << 4);
        SET_GPR_U64(ctx, 4, sx(a0_32));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A190u), 0x0109A198u, sp);
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FLOAT_FUNCTION(0x01089B70u, getFogFactor0, decomp_fog_ptclman::getFogFactor0);
SOTC_DECOMP_FLOAT_FUNCTION(0x01089BB0u, getFogFactor1, decomp_fog_ptclman::getFogFactor1);
SOTC_DECOMP_FUNCTION(0x0109A3D8u, ptclmanGet, decomp_fog_ptclman::ptclmanGet);
SOTC_DECOMP_FUNCTION(0x0109A3F8u, ptclmanPut, decomp_fog_ptclman::ptclmanPut);
SOTC_DECOMP_FUNCTION(0x0109A1F8u, ptclmanBegin, decomp_fog_ptclman::ptclmanBegin);
SOTC_DECOMP_FUNCTION(0x0109A228u, ptclmanEnd, decomp_fog_ptclman::ptclmanEnd);
SOTC_DECOMP_FUNCTION(0x0109A1A8u, ptclDisplayListGet, decomp_fog_ptclman::ptclDisplayListGet);
SOTC_DECOMP_FUNCTION(0x0109A170u, ptclDisplayListPut, decomp_fog_ptclman::ptclDisplayListPut);
