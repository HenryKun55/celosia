// Native decompilation of particle DMA/VIF1 packet builders:
// - ptclmanBuildMatPacket (0x01099E00)
// - ptclmanBuildEnvPacket (0x0109A280)
// - ptclmanBuildVtxPacket (0x01099F38)
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ptclpackets
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kBuildMatPacket = 0x01099E00u;
    constexpr uint32_t kBuildVtxPacket = 0x01099F38u;
    constexpr uint32_t kBuildEnvPacket = 0x0109A280u;

    constexpr uint32_t kPtclPacketState = 0x01215BE8u;
    constexpr uint32_t kFogFactor0 = 0x01089B70u;
    constexpr uint32_t kFogFactor1 = 0x01089BB0u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline bool on()
    {
        static const bool enabled = !std::getenv("SOTC_PTCLPACKETS") || std::atoi(std::getenv("SOTC_PTCLPACKETS")) != 0;
        return enabled;
    }
    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline void setCondition(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // ptclmanBuildMatPacket (0x01099E00)
    void buildMatPacket(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kBuildMatPacket, rdram, ctx, runtime);
        (void)runtime;
        ctx->f[0] = 1.0f;

        const uint32_t count0 = READ32(kPtclPacketState + 4u);
        const bool le12 = FPU_C_OLE_S(ctx->f[12], 1.0f);
        setCondition(ctx, le12);
        const uint32_t base = READ32(kPtclPacketState);
        const uint32_t t3 = base + (count0 << 4);

        WRITE64(t3 + 0x20u, 0x00051011ULL);
        WRITE64(t3 + 0x28u, 0x47ULL);
        WRITE64(t3 + 0x38u, 0x8ULL);

        const uint64_t a1_clamp = le12 ? 1ULL : 0ULL;
        const bool le13 = FPU_C_OLE_S(ctx->f[13], 1.0f);
        setCondition(ctx, le13);
        const uint64_t clampVal = le13 ? (a1_clamp | 4ULL) : a1_clamp;
        WRITE64(t3 + 0x30u, clampVal);

        const uint64_t a0 = GPR_U64(ctx, 4);
        uint64_t v1_blend = 0ULL;
        uint64_t a1_blend = 1ULL;
        if (a0 == 1ULL)
        {
            a1_blend = 2ULL;
        }
        else if (static_cast<int64_t>(a0) >= 2 && a0 == 2ULL)
        {
            v1_blend = 2ULL;
            a1_blend = 0ULL;
        }

        const uint64_t alphaVal = ((v1_blend << 32) >> 32) | (0x8000ULL << 24) | (a1_blend << 2) | 0x40ULL;
        WRITE64(t3 + 0x40u, alphaVal);
        WRITE64(t3 + 0x48u, 0x42ULL);
        WRITE64(t3 + 0x18u, 0xEULL);
        WRITE32(t3 + 0x00u, 0x10000004u);
        WRITE32(t3 + 0x08u, 0x13000000u);
        WRITE32(t3 + 0x0Cu, 0x50000004u);
        WRITE64(t3 + 0x10u, 0x1000000000008003ULL);
        WRITE32(t3 + 0x04u, 0u);

        const uint32_t newCount = READ32(kPtclPacketState + 4u) + 5u;
        WRITE32(kPtclPacketState + 4u, newCount);
        SET_GPR_U64(ctx, 2, sx(newCount));
        SET_GPR_U64(ctx, 3, sx(0x10000004u));
        returnTo(ctx);
    }

    // ptclmanBuildEnvPacket (0x0109A280)
    void buildEnvPacket(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kBuildEnvPacket, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp1 = sp0 - 0x40u;
        SET_GPR_U64(ctx, 29, sx(sp1));

        WRITE64(sp1 + 0x28u, GPR_U64(ctx, 21));
        const uint64_t s5 = sx(kPtclPacketState);
        const uint32_t count0 = READ32(kPtclPacketState + 4u);
        const uint32_t base = READ32(kPtclPacketState);
        WRITE64(sp1 + 0x08u, GPR_U64(ctx, 17));
        const uint32_t v1 = base + (count0 << 4);
        const uint64_t s1 = GPR_U64(ctx, 5);
        WRITE64(sp1 + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp1 + 0x10u, GPR_U64(ctx, 18));
        const uint32_t s2_u32 = v1 + 0x20u;
        const uint64_t s2 = sx(s2_u32);
        WRITE64(sp1 + 0x18u, GPR_U64(ctx, 19));
        const uint64_t s3 = GPR_U64(ctx, 7);
        WRITE64(sp1 + 0x20u, GPR_U64(ctx, 20));
        const uint64_t s4 = GPR_U64(ctx, 8);
        WRITE64(sp1 + 0x30u, GPR_U64(ctx, 31));

        WRITE32(kPtclPacketState + 8u, static_cast<uint32_t>(s1));
        WRITE32(v1 + 0x00u, 0x30000012u);
        const uint32_t envAddr = READ32(0x0118BFF4u) & 0x0FFFFFFFu;
        WRITE32(v1 + 0x08u, 0x13000000u);
        WRITE32(v1 + 0x0Cu, 0x6C120000u);
        const uint64_t s0 = GPR_U64(ctx, 4);
        WRITE32(v1 + 0x10u, 0x1000000Bu);
        WRITE32(v1 + 0x1Cu, 0x6C0A0012u);
        WRITE32(v1 + 0x04u, envAddr);
        const uint64_t a2 = GPR_U64(ctx, 6);
        WRITE32(v1 + 0x14u, 0u);
        WRITE32(v1 + 0x18u, 0u);

        SET_GPR_U64(ctx, 16, s0);
        SET_GPR_U64(ctx, 17, s1);
        SET_GPR_U64(ctx, 18, s2);
        SET_GPR_U64(ctx, 19, s3);
        SET_GPR_U64(ctx, 20, s4);
        SET_GPR_U64(ctx, 21, s5);
        SET_GPR_U64(ctx, 4, s2);
        SET_GPR_U64(ctx, 5, a2);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0109A328u), 0x0109A330u, sp1);

        WRITE128(s2_u32 + 0x50u, READ128(0x0118BFE0u));
        WRITE128(s2_u32 + 0x60u, READ128(static_cast<uint32_t>(s0)));
        callGuest(rdram, ctx, runtime, kFogFactor0, 0x0109A34Cu, sp1);

        WRITE32(s2_u32 + 0x70u, bf(ctx->f[0]));
        callGuest(rdram, ctx, runtime, kFogFactor1, 0x0109A354u, sp1);

        WRITE32(s2_u32 + 0x7Cu, bf(ctx->f[0]));
        WRITE32(s2_u32 + 0x40u, static_cast<uint32_t>(s1));
        if (s3 != 0ULL)
        {
            WRITE128(s2_u32 + 0x80u, READ128(static_cast<uint32_t>(s3)));
        }
        if (s4 != 0ULL)
        {
            WRITE128(s2_u32 + 0x90u, READ128(static_cast<uint32_t>(s4)));
        }

        WRITE32(s2_u32 + 0xACu, 0x01000101u);
        WRITE32(s2_u32 + 0xA0u, 0x15000000u);
        WRITE32(s2_u32 + 0xA4u, 0x03000028u);
        WRITE32(s2_u32 + 0xA8u, 0x0200018Cu);

        SET_GPR_U64(ctx, 16, READ64(sp1 + 0x00u));
        const uint32_t newCount = READ32(kPtclPacketState + 4u) + 0xDu;
        SET_GPR_U64(ctx, 17, READ64(sp1 + 0x08u));
        SET_GPR_U64(ctx, 2, sx(newCount));
        SET_GPR_U64(ctx, 3, sx(0x01000101u));
        SET_GPR_U64(ctx, 18, READ64(sp1 + 0x10u));
        WRITE32(kPtclPacketState + 4u, newCount);
        SET_GPR_U64(ctx, 19, READ64(sp1 + 0x18u));
        SET_GPR_U64(ctx, 20, READ64(sp1 + 0x20u));
        SET_GPR_U64(ctx, 21, READ64(sp1 + 0x28u));
        SET_GPR_U64(ctx, 31, READ64(sp1 + 0x30u));
        SET_GPR_U64(ctx, 29, sx(sp1 + 0x40u));
        returnTo(ctx);
    }

    // ptclmanBuildVtxPacket (0x01099F38)
    void buildVtxPacket(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kBuildVtxPacket, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp1 = sp0 - 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp1));

        uint64_t t8 = GPR_U64(ctx, 5);
        WRITE64(sp1 + 0x00u, GPR_U64(ctx, 16));
        uint64_t s0 = 0ULL;
        WRITE64(sp1 + 0x08u, GPR_U64(ctx, 17));
        uint64_t s1 = GPR_U64(ctx, 4);
        WRITE64(sp1 + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp1 + 0x18u, GPR_U64(ctx, 19));

        const uint64_t s3 = sx(0x01210000u);
        const uint32_t count0 = READ32(kPtclPacketState + 4u);
        const uint32_t base = READ32(kPtclPacketState);
        const uint64_t s2 = sx(base + (count0 << 4));

        if (t8 != 0ULL)
        {
            do
            {
                const uint32_t a1_off = static_cast<uint32_t>(s0) << 4;
                s0 = sx(static_cast<uint32_t>(s0) + 4u);
                const uint32_t t5 = static_cast<uint32_t>(s2) + a1_off;

                WRITE32(t5 + 0x00u, 0x10000002u);
                WRITE32(t5 + 0x0Cu, 0x6C028000u);
                WRITE32(t5 + 0x08u, 0x01000101u);
                WRITE32(t5 + 0x04u, 0u);

                uint64_t a0 = 1ULL;
                const uint64_t t6 = (t8 < 0x1Fu) ? t8 : 0x1Eu;
                uint64_t a3 = 0ULL;
                const uint64_t v1 = sx(READ32(kPtclPacketState + 8u));
                uint32_t t3 = static_cast<uint32_t>(t6) << 1;
                uint64_t t4 = 0ULL;
                const uint64_t t9 = sx(t3 + static_cast<uint32_t>(t6));
                uint64_t a2 = 0ULL;
                uint64_t t7 = 0ULL;
                uint64_t v0 = 0x4DULL;

                if (v1 < 9ULL)
                {
                    const uint32_t jumpTarget = READ32(0x01192000u + (static_cast<uint32_t>(v1) << 2));
                    switch (jumpTarget)
                    {
                        case 0x0109A000u:
                            a0 <<= 32;
                            a3 = 2ULL;
                            t4 = 0x7EULL;
                            a2 = 3ULL;
                            t7 = 0x412ULL;
                            break;
                        case 0x0109A010u:
                            a0 <<= 32;
                            a3 = 4ULL;
                            t4 = 0x7CULL;
                            a2 = 3ULL;
                            t7 = 0x412ULL;
                            break;
                        case 0x0109A020u:
                            a0 <<= 32;
                            a3 = 6ULL;
                            t4 = 0x7DULL;
                            a2 = 3ULL;
                            t7 = 0x412ULL;
                            break;
                        case 0x0109A038u:
                            a3 = 4ULL;
                            t4 = 0x6CULL;
                            a2 = 3ULL;
                            t7 = 0x412ULL;
                            a0 = 0ULL;
                            break;
                        case 0x0109A060u:
                            a0 <<= 32;
                            break;
                        default:
                            SET_GPR_U64(ctx, 2, sx(static_cast<uint32_t>(v1) << 2));
                            SET_GPR_U64(ctx, 3, sx(jumpTarget));
                            SET_GPR_U64(ctx, 4, a0);
                            SET_GPR_U64(ctx, 5, sx(kPtclPacketState));
                            SET_GPR_U64(ctx, 6, a2);
                            SET_GPR_U64(ctx, 7, a3);
                            SET_GPR_U64(ctx, 11, sx(t3));
                            SET_GPR_U64(ctx, 12, t4);
                            SET_GPR_U64(ctx, 13, sx(t5));
                            SET_GPR_U64(ctx, 14, t6);
                            SET_GPR_U64(ctx, 15, t7);
                            SET_GPR_U64(ctx, 16, s0);
                            SET_GPR_U64(ctx, 17, s1);
                            SET_GPR_U64(ctx, 18, s2);
                            SET_GPR_U64(ctx, 19, s3);
                            SET_GPR_U64(ctx, 24, t8);
                            SET_GPR_U64(ctx, 25, t9);
                            ctx->pc = jumpTarget;
                            runtime->jumpGuest(rdram, ctx, jumpTarget);
                            return;
                    }
                }
                else
                {
                    a0 = 0x8000ULL << 17;
                }

                a0 = (a0 >> 28) | v0 | 0x20ULL;
                const uint64_t v0_hi = a2 << 60;
                uint64_t a1_val = (a3 | 0x8000ULL) | v0_hi | (t4 << 47) | (0x8000ULL << 31);
                uint64_t t4_val = static_cast<uint64_t>(static_cast<int64_t>(a0 << 32) >> 32);
                t4_val = (t4_val << 47) | 0x3000400000008000ULL;

                const uint32_t a2_off = static_cast<uint32_t>(s0) << 4;
                s0 = sx(static_cast<uint32_t>(s0) + 1u);
                t3 = (t3 + static_cast<uint32_t>(t6)) << 4;
                const uint32_t a3_w = (static_cast<uint32_t>(t9) << 16) | 0x6C008002u;
                const uint32_t a2_ptr = a2_off + static_cast<uint32_t>(s2);
                const uint32_t t0_w = static_cast<uint32_t>(s1) & 0x0FFFFFFFu;
                const uint32_t t1_w = static_cast<uint32_t>(t9) | 0x30000000u;

                t8 = sx(static_cast<uint32_t>(t8) - static_cast<uint32_t>(t6));
                WRITE64(t5 + 0x18u, t7);
                s1 = sx(static_cast<uint32_t>(s1) + t3);
                WRITE32(t5 + 0x3Cu, a3_w);
                WRITE64(t5 + 0x10u, a1_val);
                WRITE64(t5 + 0x20u, t4_val);
                WRITE64(t5 + 0x28u, 0x412ULL);
                WRITE32(t5 + 0x30u, t1_w);
                WRITE32(t5 + 0x34u, t0_w);
                WRITE32(t5 + 0x38u, 0x01000101u);
                WRITE32(t5 + 0x1Cu, static_cast<uint32_t>(t6));

                WRITE32(a2_ptr + 0x0Cu, 0x17000000u);
                WRITE32(a2_ptr + 0x00u, 0x10000000u);
                WRITE32(a2_ptr + 0x04u, 0u);
                WRITE32(a2_ptr + 0x08u, 0u);
            } while (t8 != 0ULL);
        }

        SET_GPR_U64(ctx, 19, READ64(sp1 + 0x18u));
        const uint32_t newCount = READ32(kPtclPacketState + 4u) + static_cast<uint32_t>(s0);
        SET_GPR_U64(ctx, 17, READ64(sp1 + 0x08u));
        SET_GPR_U64(ctx, 2, sx(newCount));
        SET_GPR_U64(ctx, 3, sx(kPtclPacketState));
        SET_GPR_U64(ctx, 16, READ64(sp1 + 0x00u));
        SET_GPR_U64(ctx, 18, READ64(sp1 + 0x10u));
        WRITE32(kPtclPacketState + 4u, newCount);
        SET_GPR_U64(ctx, 29, sx(sp1 + 0x20u));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01099E00u, ptclmanBuildMatPacket, decomp_ptclpackets::buildMatPacket);
SOTC_DECOMP_FUNCTION(0x0109A280u, ptclmanBuildEnvPacket, decomp_ptclpackets::buildEnvPacket);
SOTC_DECOMP_FUNCTION(0x01099F38u, ptclmanBuildVtxPacket, decomp_ptclpackets::buildVtxPacket);
