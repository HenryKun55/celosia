// manager_sub_0113E940: a particle system's draw (1.6% of the Vita's game
// thread recompiled, throttled attract demo). a0 the system (+0x124 the
// count, +0x128 the first particle, +0x140 / +0x144 alpha / size scales,
// +0x70.. colour scales), a1 its material, a2 its effect record (+0x1c the
// display list, +0xb8 the brightness factor, +0xb4). For each live particle
// (+0x40 not 0; next at +0xa8) it writes 48 bytes into the display list
// (ptclDisplayListGet): the position, the size x scale, the colour x
// scales clamped to 0..1 (vminiw / vmaxx), the alpha x scale x
// 01142030(particle), and sin / cos of its angle + pi/4 (iosGetSinCosf).
// Then the env matrix and the packets (ptclman*), once.
//
// Natively: the prologue (its frame as the recompiled code writes it),
// the particle loop (the recompiled code's FPU / VU operations in the same
// order; 01142030 inline and iosGetSinCosf called straight), the VU0 matrix
// multiply, the packet calls (ptclman*), and the epilogue.
#include "sotc_decomp.h"
#include "lines67.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_math
{
    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_ptcldraw
{
    constexpr uint32_t kDraw = 0x0113E940u;

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    bool plain(uint32_t a, uint32_t size)
    {
        return !Ps2IsSpecialAddress(a) && !Ps2IsSpecialAddress(a + size - 1u) && (a & PS2_RAM_MASK) + size <= PS2_RAM_SIZE;
    }

    struct Ram
    {
        uint8_t *rdram;
        uint8_t *p(uint32_t a) const
        {
            lines67::touch(a);
            return rdram + (a & PS2_RAM_MASK);
        }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, p(a), 8);
            return v;
        }
        __m128 q128(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, p(a & ~0xFu), 16);
            return v;
        }
        float f32(uint32_t a) const { return bits(u32(a)); }
        void putf(uint32_t a, float f) const { std::memcpy(p(a), &f, 4); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(p(a), &v, 8); }
        void put128(uint32_t a, __m128 v) const { std::memcpy(p(a & ~0xFu), &v, 16); }
    };

    void draw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t s2 = GPR_U32(ctx, 4), fp = GPR_U32(ctx, 5), s4 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp1 = sp0 - 0xF0u;
        const lines67::Scope l67(lines67::kPtcl, true, sp0);
        if ((sp1 & 0xFu) != 0u || !plain(sp1, 0xF0u) || !plain(s2, 0x150u) || !plain(s4, 0xC0u))
            return sotc_decomp::recompiled(kDraw)(rdram, ctx, runtime);
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20),
                       e21 = GPR_U64(ctx, 21), e22 = GPR_U64(ctx, 22), e23 = GPR_U64(ctx, 23), e30 = GPR_U64(ctx, 30), e31 = GPR_U64(ctx, 31);
        // The prologue's frame (the epilogue reloads it).
        SET_GPR_U32(ctx, 29, sp1);
        ram.put64(sp1 + 0xA0u, e18);
        ram.put64(sp1 + 0xB0u, e20);
        ram.put64(sp1 + 0xC8u, e23);
        ram.put64(sp1 + 0xD0u, e30);
        ram.put64(sp1 + 0x90u, e16);
        ram.put64(sp1 + 0x98u, e17);
        ram.put64(sp1 + 0xA8u, e19);
        ram.put64(sp1 + 0xB8u, e21);
        ram.put64(sp1 + 0xC0u, e22);
        ram.put64(sp1 + 0xD8u, e31);
        ram.putf(sp1 + 0xE8u, ctx->f[21]);
        ram.putf(sp1 + 0xE0u, ctx->f[20]);
        SET_GPR_U32(ctx, 18, s2);
        SET_GPR_U32(ctx, 20, s4);
        SET_GPR_U32(ctx, 30, fp);
        auto call = [&](uint32_t target, uint32_t ra) { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp1); };

        SET_GPR_U32(ctx, 4, ram.u32(s4 + 0x1Cu));
        call(0x0109A1A8u, 0x0113E988u); // ptclDisplayListGet
        const uint32_t s7 = GPR_U32(ctx, 2);
        SET_GPR_U32(ctx, 4, s2 + 0x80u);
        SET_GPR_U64(ctx, 23, GPR_U64(ctx, 2));
        call(0x0108F7E0u, 0x0113E994u); // gluGetBrightness
        const float one = bits(0x3F800000u);
        float f0 = ctx->f[0], f1 = FPU_MUL_S(ram.f32(s4 + 0xB8u), f0), f2 = one;
        const bool c0 = FPU_C_OLT_S(f2, f1);
        ctx->fcr31 = c0 ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (!c0)
        {
            f0 = 0.0f;
            f2 = f0;
            const bool c1 = FPU_C_OLT_S(f1, f0);
            ctx->fcr31 = c1 ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (!c1)
                f2 = f1;
        }
        auto finish = [&]()
        {
            SET_GPR_U64(ctx, 16, ram.u64(sp1 + 0x90u));
            SET_GPR_U64(ctx, 17, ram.u64(sp1 + 0x98u));
            SET_GPR_U64(ctx, 18, ram.u64(sp1 + 0xA0u));
            SET_GPR_U64(ctx, 19, ram.u64(sp1 + 0xA8u));
            SET_GPR_U64(ctx, 20, ram.u64(sp1 + 0xB0u));
            SET_GPR_U64(ctx, 21, ram.u64(sp1 + 0xB8u));
            SET_GPR_U64(ctx, 22, ram.u64(sp1 + 0xC0u));
            SET_GPR_U64(ctx, 23, ram.u64(sp1 + 0xC8u));
            SET_GPR_U64(ctx, 30, ram.u64(sp1 + 0xD0u));
            SET_GPR_U64(ctx, 31, ram.u64(sp1 + 0xD8u));
            ctx->f[21] = ram.f32(sp1 + 0xE8u);
            ctx->f[20] = ram.f32(sp1 + 0xE0u);
            SET_GPR_U32(ctx, 29, sp0);
            sotc_decomp::returnTo(ctx);
        };
        SET_GPR_U64(ctx, 22, 0u); // s6
        if (s7 == 0u)
        {
            ctx->f[0] = f0;
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            return finish();
        }

        f0 = FPU_SUB_S(one, f2);
        f1 = ram.f32(s4 + 0xB4u);
        ram.putf(sp1 + 0x50u, f2);
        ram.putf(sp1 + 0x5Cu, f1);
        ram.put32(sp1 + 0x58u, 0u);
        ram.putf(sp1 + 0x54u, f0);
        uint64_t v0, v1;
        std::memcpy(&v0, ram.p(sp1 + 0x58u), 8);
        std::memcpy(&v1, ram.p(sp1 + 0x50u), 8);
        ram.put64(sp1 + 0x48u, v0);
        ram.put64(sp1 + 0x40u, v1);
        uint32_t count = ram.u32(s2 + 0x124u), s0 = ram.u32(s2 + 0x128u);
        uint32_t s5 = 0u, s6 = 0u;
        if (count != 0u)
        {
            const float f21 = ram.f32(sp1 + 0x58u), f20 = bits(0x3F490FDAu);
            uint32_t s1 = s7, s3 = s7 + 0x20u;
            ctx->f[21] = f21;
            ctx->f[20] = f20;
            const float scale144 = ram.f32(s2 + 0x144u), r70 = ram.f32(s2 + 0x70u), r74 = ram.f32(s2 + 0x74u), r78 = ram.f32(s2 + 0x78u);
            const __m128 vf0 = ctx->vu0_vf[0];
            const __m128 wv = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3, 3, 3, 3)), xv = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0, 0, 0, 0));
            __m128 vf7 = ctx->vu0_vf[7];
            // Round 67 (SOTC_WALK67): a runner kAhead particles in front of
            // the loop along the +0xa8 links, asking for each particle's
            // lines (+0x00 .. +0xa8: the position, the scales, the fade
            // words, the angle, the link) as it reaches it, and the display
            // list's 48-byte slots as far ahead (written in turn). Only
            // reads and PLDs: the loop below is unchanged (the runner reads
            // a link the loop reads later; nothing is written in between but
            // the display list and the context, and a link it gets wrong
            // only costs a wasted line).
            const bool w67 = sotc_decomp::walk67On();
            constexpr uint32_t kAhead = 4u;
            uint32_t runner = s0, outAhead = s1;
            auto askParticle = [&](uint32_t p)
            {
                for (uint32_t off = 0; off < 0xB0u; off += 0x20u)
                {
                    lines67::pref(p + off);
                    __builtin_prefetch(rdram + ((p + off) & PS2_RAM_MASK));
                }
            };
            auto askOut = [&](uint32_t o)
            {
                lines67::pref(o), lines67::pref(o + 0x2Fu);
                __builtin_prefetch(rdram + (o & PS2_RAM_MASK));
                __builtin_prefetch(rdram + ((o + 0x2Fu) & PS2_RAM_MASK));
            };
            if (w67)
            {
                for (uint32_t i = 0; i < kAhead && runner != 0u && plain(runner, 0xB0u); ++i)
                {
                    askParticle(runner);
                    askOut(outAhead);
                    outAhead += 0x30u;
                    if (i + 1u < kAhead)
                        runner = ram.u32(runner + 0xA8u);
                }
            }
            do
            {
                if (w67 && runner != 0u && plain(runner, 0xB0u))
                {
                    runner = ram.u32(runner + 0xA8u); // asked for kAhead - 1 particles ago
                    if (runner != 0u && plain(runner, 0xB0u))
                        askParticle(runner);
                    askOut(outAhead);
                    outAhead += 0x30u;
                }
                f0 = ram.f32(s0 + 0x40u);
                if (FPU_C_EQ_S(f0, f21))
                {
                    ctx->f[0] = f0;
                    ++s6;
                }
                else
                {
                    std::memcpy(ram.p(s1 & ~0xFu), ram.p(s0 & ~0xFu), 16); // lq / sq $v0
                    ram.putf(s1 + 0xCu, FPU_MUL_S(scale144, ram.f32(s0 + 0x4Cu)));
                    ram.putf(s1 + 0x10u, FPU_MUL_S(r70, ram.f32(s0 + 0x68u)));
                    ram.putf(s1 + 0x14u, FPU_MUL_S(r74, ram.f32(s0 + 0x70u)));
                    ram.putf(s1 + 0x18u, FPU_MUL_S(r78, ram.f32(s0 + 0x78u)));
                    // 01142030 (the particle's fade: its life +0x44 against
                    // the fade-in end +0x58 and the fade-out start +0x5c), inline.
                    SET_GPR_U32(ctx, 4, s0);
                    {
                        const float life = ram.f32(s0 + 0x44u);
                        float fade;
                        if (FPU_C_OLT_S(life, ram.f32(s0 + 0x58u)))
                            fade = FPU_MUL_S(life, ram.f32(s0 + 0x60u));
                        else
                        {
                            const float out = ram.f32(s0 + 0x5Cu);
                            ctx->f[1] = out;
                            fade = one;
                            if (!FPU_C_OLT_S(life, out))
                            {
                                ctx->f[1] = ram.f32(s0 + 0x64u);
                                fade = FPU_MUL_S(FPU_SUB_S(ram.f32(s0 + 0x40u), life), ctx->f[1]);
                            }
                        }
                        ctx->f[2] = life;
                        ctx->f[0] = fade;
                    }
                    const float alpha = FPU_MUL_S(FPU_MUL_S(ram.f32(s2 + 0x140u), ram.f32(s0 + 0x54u)), ctx->f[0]);
                    ram.putf(s1 + 0x1Cu, alpha);
                    const uint32_t c = (s1 + 0x10u) & ~0xFu;
                    std::memcpy(&vf7, ram.p(c), 16);
                    vf7 = _mm_min_ps(vf7, wv);
                    vf7 = _mm_max_ps(vf7, xv);
                    std::memcpy(ram.p(c), &vf7, 16);
                    ctx->vu0_vf[7] = vf7;
                    const float f12 = FPU_ADD_S(ram.f32(s0 + 0x98u), f20);
                    SET_GPR_U32(ctx, 4, s3);
                    s1 += 0x30u;
                    ++s5;
                    s3 += 0x30u;
                    ctx->f[12] = f12;
                    // iosGetSinCosf: straight to decomp_math's when the table holds it.
                    {
                        const uint32_t target = sotc_decomp::jalTarget(rdram, 0x0113EAC4u);
                        if (target == 0x001BDDB0u && runtime->guestFunctionAt(target) == &decomp_math::getSinCos)
                        {
                            SET_GPR_U32(ctx, 31, 0x0113EACCu);
                            decomp_math::getSinCos(rdram, ctx, runtime);
                        }
                        else
                            call(target, 0x0113EACCu);
                    }
                    count = ram.u32(s2 + 0x124u);
                    ++s6;
                }
                s0 = ram.u32(s0 + 0xA8u);
            } while (s6 < count);
            SET_GPR_U32(ctx, 17, s1);
            SET_GPR_U32(ctx, 19, s3);
            f0 = ctx->f[0];
        }
        else
            ctx->f[0] = f0;
        SET_GPR_U32(ctx, 16, s0);
        SET_GPR_U32(ctx, 21, s5);
        SET_GPR_U32(ctx, 22, s6);
        // iosScaleMatrix(sp, +0x50, +0x54, 1), then the VU0 matrix multiply and ptclman* packets.
        ctx->f[20] = one;
        SET_GPR_U32(ctx, 4, sp1);
        ctx->f[12] = ram.f32(s4 + 0x50u);
        ctx->f[13] = ram.f32(s4 + 0x54u);
        ctx->f[14] = one;
        call(sotc_decomp::jalTarget(rdram, 0x0113EAF4u), 0x0113EAFCu);
        constexpr uint32_t kEnvMat = 0x01216E40u;
        SET_GPR_U32(ctx, 2, kEnvMat);
        ctx->vu0_vf[14] = ram.q128(kEnvMat + 0x00u);
        ctx->vu0_vf[15] = ram.q128(kEnvMat + 0x10u);
        ctx->vu0_vf[16] = ram.q128(kEnvMat + 0x20u);
        ctx->vu0_vf[17] = ram.q128(kEnvMat + 0x30u);
        ctx->vu0_vf[24] = ram.q128(sp1 + 0x00u);
        ctx->vu0_vf[25] = ram.q128(sp1 + 0x10u);
        ctx->vu0_vf[26] = ram.q128(sp1 + 0x20u);
        ctx->vu0_vf[27] = ram.q128(sp1 + 0x30u);
        for (int i = 24; i <= 27; ++i)
        {
            const __m128 v = ctx->vu0_vf[i];
            ctx->vu0_acc = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))));
            ctx->vu0_vf[i] = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3))));
        }
        ram.put128(sp1 + 0x00u, ctx->vu0_vf[24]);
        ram.put128(sp1 + 0x10u, ctx->vu0_vf[25]);
        ram.put128(sp1 + 0x20u, ctx->vu0_vf[26]);
        ram.put128(sp1 + 0x30u, ctx->vu0_vf[27]);
        SET_GPR_U32(ctx, 16, s2 + 0x90u);
        if (s5 == 0u)
            return finish();
        SET_GPR_U32(ctx, 2, 0x01190000u);
        SET_GPR_U32(ctx, 3, ram.u32(0x0118CFF0u));
        if (GPR_U64(ctx, 3) == 0u)
            return finish();
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 21));
        call(0x0109A170u, 0x0113EB94u); // ptclDisplayListPut
        SET_GPR_U32(ctx, 4, 7u);
        call(0x01114B50u, 0x0113EB9Cu); // transMicroCode
        call(0x0109A1F8u, 0x0113EBA4u); // ptclmanBegin
        ctx->f[13] = ram.f32(s4 + 0x5Cu);
        ctx->f[12] = ram.f32(s4 + 0x58u);
        ram.putf(sp1 + 0x84u, ctx->f[13]);
        ram.putf(sp1 + 0x80u, ctx->f[12]);
        ram.put32(sp1 + 0x88u, 0u);
        ram.put32(sp1 + 0x8Cu, 0u);
        const uint64_t m0 = ram.u64(sp1 + 0x80u);
        SET_GPR_U64(ctx, 2, m0);
        SET_GPR_U32(ctx, 4, ram.u32(fp + 0x14u));
        const uint64_t m1 = ram.u64(sp1 + 0x88u);
        SET_GPR_U64(ctx, 3, m1);
        ram.put64(sp1 + 0x70u, m0);
        ram.put64(sp1 + 0x78u, m1);
        call(0x01099E00u, 0x0113EBD4u); // ptclmanBuildMatPacket
        SET_GPR_U32(ctx, 4, sp1 + 0x70u);
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 16));
        SET_GPR_U32(ctx, 6, sp1);
        ctx->f[12] = FPU_SUB_S(ctx->f[20], ram.f32(s4 + 0x24u));
        SET_GPR_U32(ctx, 8, sp1 + 0x40u);
        SET_GPR_U32(ctx, 5, 4u);
        call(0x0109A280u, 0x0113EBF4u); // ptclmanBuildEnvPacket
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 23));
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 21));
        call(0x01099F38u, 0x0113EC00u); // ptclmanBuildVtxPacket
        call(0x0109A228u, 0x0113EC08u); // ptclmanEnd
        finish();
    }
}

SOTC_DECOMP_FUNCTION(0x0113E940u, particleDraw, decomp_ptcldraw::draw);
