// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): the particle systems'
// display-list writers and the geometry pointer kick, from the colossus
// fight's profile, transcribed from build/runner's generated code (the same
// FPU_* / VU0 operations in order, guest calls through the table with the
// original's ra and frame sp).
//
// Three particle draws, siblings of particleDraw (0113E940,
// decomp_ptcldraw.cpp). Each takes a0 the system (+0x124 the particle count,
// +0x128 the first particle, next at +0xa8; +0x140 / +0x144 alpha / size
// scales, +0x70 / +0x74 / +0x78 colour scales), a2 the effect record (+0x1c
// its display list, +0x24 the fade, +0x58 / +0x5c or +0x12c / +0x130 the
// sizes for the matrix). For each live particle (+0x40 not 0.0) a 48-byte
// record goes into the display list (ptclDisplayListGet): the particle's
// first qword, the size x scale, the colour x scales clamped to 0..1
// (vminiw / vmaxx on vf7), the alpha x scale x the fade (01142030, a
// leaf, inline here: f0 / f1 / f2 and the condition bit as it leaves them),
// then a per-function last qword. If any was written and the particle
// renderer is on (*0x0118CFF0), ptclDisplayListPut(n), transMicroCode(7),
// ptclmanBegin, ptclmanBuildMatPacket, ptclmanBuildEnvPacket,
// ptclmanBuildVtxPacket, ptclmanEnd.
// - manager_sub_0113DBF8 (a1 the material, a3 the blend mode, t0 the env
//   argument): last qword sin / cos of the angle (+0x98) + pi/4
//   (iosGetSinCosf). Mode 7 takes the +0x12c / +0x130 sizes and a unit
//   matrix scale, the others +0x58 / +0x5c; the vertex packet only for
//   modes 1..5 and 7.
// - manager_sub_0113E6C8 (a1 the material, a3 the env argument): the
//   animation frame int(+0x90 / fx+0x128) (cvt.w.s), sin / cos of the
//   angle + pi/4 at +0x20, the frame's texture offsets (+0x28: the frame
//   below x fx+0x12c, +0x2c: the frame x fx+0x130).
// - manager_sub_0113DEA0 (a1 the material, +0x1b4 picks the variant): 0,
//   the particles whose span 01142198 accepts (it writes the last qword);
//   else every live one with the system's +0x10 qword. Env mode 3 with
//   the relocated table at 0x001E0F00 (lui / addiu sites read live).
//
// manager_sub_01108960 (the geometry pointer kick, 48 types through a jump
// table): see its own block below. Every case is native; a table word the
// generated code does not know goes to the recompiled code from the start.
//
// Round 48 (01108960 only): s0-s7 / fp now hold the code's values in the
// context at every guest call (Kick::sync before each one; parentNode sets
// s0 to the frame quaternion before the motion call as the code does) and
// are reloaded from the frame saves at the end as the epilogue does; the
// code keeps nothing in f20-f31. Round 47's stale-slot bug: the kickLWS,
// updateSEGeo, 01104BA0, 0110C530 / 0110C660 callees reach 010EFD08.
//
// Not as the original: the scratch GPRs ($at, a0-t3 apart from the calls'
// arguments) are not all set as the code sets them; in the particle draws
// the callee-saved registers are never changed in the context (the code
// restores every one from its frame), the frame's saves are written as the
// code writes them;
// 01142030 inline (no table call, ra not set to its site); the particle
// loops' checkpoints are left out. In 01108960 the scratch GPRs are set as
// the code sets them on the paths that reach a return, $at only where an
// FPU constant goes through it.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_math
{
    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_hot46j
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000u) != 0u; }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- the particle draws' shared parts ---------------------------------
    constexpr uint32_t kDisplayListGet = 0x0109A1A8u, kDisplayListPut = 0x0109A170u, kTransMicroCode = 0x01114B50u, kPtclmanBegin = 0x0109A1F8u;
    constexpr uint32_t kBuildMatPacket = 0x01099E00u, kBuildEnvPacket = 0x0109A280u, kBuildVtxPacket = 0x01099F38u, kPtclmanEnd = 0x0109A228u;
    constexpr uint32_t kSpanTest = 0x01142198u; // manager_sub_01142198
    constexpr uint32_t kRendererOn = 0x0118CFF0u; // lui 0x119 / lw -0x3010: constant in the code
    constexpr uint32_t kRendererHi = 0x01190000u; // v0 as that lui leaves it
    constexpr uint32_t kOne = 0x3F800000u, kQuarterPi = 0x3F490FDAu;
    constexpr uint32_t kRecordSize = 0x30u;

    // Particle record fields.
    constexpr uint32_t kPtLife = 0x40u, kPtAge = 0x44u, kPtNext = 0xA8u, kPtAngle = 0x98u;

    // 01142030(a0 the particle): its fade, a leaf. The age (+0x44) against
    // the fade-in end (+0x58: age x +0x60) and the fade-out start (+0x5c:
    // (life - age) x +0x64), else 1. f0 the result; f1 / f2 / the
    // condition bit as the code leaves them.
    inline void fade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p)
    {
        float *f = ctx->f;
        f[2] = fb(READ32(p + kPtAge));
        f[0] = fb(READ32(p + 0x58u));
        setC(ctx, FPU_C_OLT_S(f[2], f[0]));
        if (cBit(ctx))
        {
            f[0] = fb(READ32(p + 0x60u));
            f[0] = FPU_MUL_S(f[2], f[0]);
            return;
        }
        f[1] = fb(READ32(p + 0x5Cu)); // bc1fl's delay slot
        f[0] = fb(kOne);
        setC(ctx, FPU_C_OLT_S(f[2], f[1]));
        if (cBit(ctx))
            return;
        f[0] = fb(READ32(p + kPtLife));
        f[1] = fb(READ32(p + 0x64u));
        f[0] = FPU_SUB_S(f[0], f[2]);
        f[0] = FPU_MUL_S(f[0], f[1]);
    }

    // The first 0x20 bytes of a particle's display-list record at `dst`
    // from particle `p` of system `sys`: the qword copy (lq / sq through
    // v0), +0xc size, +0x10..+0x18 colour, the fade, +0x1c alpha, the colour
    // qword clamped to [vf0.x, vf0.w] (vf7). `addrReg` is the GPR the code
    // puts dst + 0x10 in (v0, or v1 in 0113DEA0's second loop).
    inline void record(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sys, uint32_t p, uint32_t dst, int addrReg)
    {
        float *f = ctx->f;
        SET_GPR_VEC(ctx, 2, READ128(p));
        WRITE128(dst, GPR_VEC(ctx, 2));
        WRITE32(dst + 0x0Cu, bf(FPU_MUL_S(fb(READ32(sys + 0x144u)), fb(READ32(p + 0x4Cu)))));
        WRITE32(dst + 0x10u, bf(FPU_MUL_S(fb(READ32(sys + 0x70u)), fb(READ32(p + 0x68u)))));
        WRITE32(dst + 0x14u, bf(FPU_MUL_S(fb(READ32(sys + 0x74u)), fb(READ32(p + 0x70u)))));
        WRITE32(dst + 0x18u, bf(FPU_MUL_S(fb(READ32(sys + 0x78u)), fb(READ32(p + 0x78u)))));
        SET_GPR_U32(ctx, 4, p);
        fade(rdram, ctx, runtime, p);
        f[1] = fb(READ32(sys + 0x140u));
        f[2] = fb(READ32(p + 0x54u));
        SET_GPR_S32(ctx, addrReg, dst + 0x10u);
        f[1] = FPU_MUL_S(f[1], f[2]);
        f[1] = FPU_MUL_S(f[1], f[0]);
        WRITE32(dst + 0x1Cu, bf(f[1]));
        // lqc2 vf7 / vminiw.xyzw vf7, vf7, vf0w / vmaxx.xyzw vf7, vf7, vf0x / sqc2.
        __m128 v = _mm_castsi128_ps(READ128(dst + 0x10u));
        v = _mm_min_ps(v, _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3, 3, 3, 3)));
        v = _mm_max_ps(v, _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0, 0, 0, 0)));
        ctx->vu0_vf[7] = v;
        WRITE128(dst + 0x10u, _mm_castps_si128(v));
    }

    // iosGetSinCosf (f12 the angle, a0 the sin / cos pair) from a relocated
    // jal site: straight to decomp_math's while the table holds it.
    inline void sinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t site, uint32_t sp)
    {
        sotc_decomp::callNative(rdram, ctx, runtime, jalTarget(rdram, site), &decomp_math::getSinCos, site + 8u, sp);
    }

    // ---- manager_sub_0113DBF8 -----------------------------------------------
    constexpr uint32_t kDrawSinCos = 0x0113DBF8u;

    void drawSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kDrawSinCos, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xA0u;
        const uint32_t sys = GPR_U32(ctx, 4);  // s2
        const uint32_t fx = GPR_U32(ctx, 6);   // s4
        const uint64_t mode = GPR_U64(ctx, 7); // s7
        WRITE64(sp + 0x50u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x88u, ra);
        WRITE32(sp + 0x98u, bf(f[21]));
        WRITE32(sp + 0x90u, bf(f[20]));
        auto finish = [&]()
        {
            f[21] = fb(READ32(sp + 0x98u));
            f[20] = fb(READ32(sp + 0x90u));
            SET_GPR_U64(ctx, 29, entrySp);
            SET_GPR_U64(ctx, 31, ra);
            returnTo(ctx);
        };
        auto call = [&](uint32_t target, uint32_t retAddr) { callGuest(rdram, ctx, runtime, target, retAddr, sp); };

        SET_GPR_S32(ctx, 4, READ32(fx + 0x1Cu));
        WRITE32(sp + 0x30u, GPR_U32(ctx, 5)); // the material
        WRITE32(sp + 0x34u, GPR_U32(ctx, 8)); // the env argument
        call(kDisplayListGet, 0x0113DC48u);
        const uint64_t list = GPR_U64(ctx, 2); // fp
        if (list == 0u)
            return finish();
        SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
        uint32_t p = READ32(sys + 0x128u); // s0
        uint32_t written = 0u;             // s6
        if (GPR_U64(ctx, 3) != 0u)
        {
            f[21] = 0.0f; // mtc1 zero
            f[20] = fb(kQuarterPi);
            uint32_t dst = static_cast<uint32_t>(list), pair = static_cast<uint32_t>(list) + 0x20u; // s1, s3
            uint64_t n = 0u;                                                                       // s5
            do
            {
                f[0] = fb(READ32(p + kPtLife));
                setC(ctx, FPU_C_EQ_S(f[0], f[21]));
                SET_GPR_U32(ctx, 4, p);
                if (!cBit(ctx))
                {
                    record(rdram, ctx, runtime, sys, p, dst, 2);
                    f[12] = fb(READ32(p + kPtAngle));
                    SET_GPR_U32(ctx, 4, pair);
                    dst += kRecordSize;
                    ++written;
                    f[12] = FPU_ADD_S(f[12], f[20]);
                    pair += kRecordSize;
                    sinCos(rdram, ctx, runtime, 0x0113DD18u, sp);
                    SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
                }
                n = sx(static_cast<uint32_t>(n) + 1u);
                SET_GPR_U64(ctx, 2, n < GPR_U64(ctx, 3) ? 1u : 0u);
                p = READ32(p + kPtNext);
            } while (GPR_U64(ctx, 2) != 0u);
        }
        SET_GPR_S32(ctx, 2, kRendererHi);
        if (written == 0u)
            return finish();
        SET_GPR_S32(ctx, 3, READ32(kRendererOn));
        if (GPR_U64(ctx, 3) == 0u)
            return finish();
        SET_GPR_U64(ctx, 4, written);
        call(kDisplayListPut, 0x0113DD50u);
        SET_GPR_S32(ctx, 4, 7);
        call(kTransMicroCode, 0x0113DD58u);
        call(kPtclmanBegin, 0x0113DD60u);
        SET_GPR_S32(ctx, 2, 7);
        if (mode != 7u)
        {
            f[12] = fb(READ32(fx + 0x58u));
            f[13] = fb(READ32(fx + 0x5Cu));
            SET_GPR_S32(ctx, 2, READ32(sp + 0x30u));
            WRITE32(sp + 0x20u, bf(f[12]));
            WRITE32(sp + 0x24u, bf(f[13]));
            WRITE32(sp + 0x28u, 0u);
            WRITE32(sp + 0x2Cu, 0u);
            SET_GPR_S32(ctx, 4, READ32(GPR_U32(ctx, 2) + 0x14u));
            SET_GPR_U64(ctx, 3, READ64(sp + 0x28u));
            SET_GPR_U64(ctx, 2, READ64(sp + 0x20u));
            WRITE64(sp + 0x18u, GPR_U64(ctx, 3));
            WRITE64(sp + 0x10u, GPR_U64(ctx, 2));
            call(kBuildMatPacket, 0x0113DE10u);
            f[0] = fb(READ32(fx + 0x24u));
            SET_GPR_S32(ctx, 4, sp + 0x10u);
            f[12] = fb(kOne);
            SET_GPR_U64(ctx, 5, mode);
            SET_GPR_S32(ctx, 6, READ32(sp + 0x34u));
            SET_GPR_U64(ctx, 7, 0u);
            f[12] = FPU_SUB_S(f[12], f[0]);
            SET_GPR_U64(ctx, 8, 0u);
            call(kBuildEnvPacket, 0x0113DE38u);
        }
        else
        {
            f[0] = fb(READ32(fx + 0x130u));
            f[1] = fb(READ32(fx + 0x12Cu));
            SET_GPR_S32(ctx, 2, READ32(sp + 0x30u));
            WRITE32(sp + 0x14u, bf(f[0]));
            WRITE32(sp + 0x10u, bf(f[1]));
            WRITE32(sp + 0x18u, 0u);
            WRITE32(sp + 0x1Cu, 0u);
            f[12] = fb(kOne);
            SET_GPR_S32(ctx, 4, READ32(GPR_U32(ctx, 2) + 0x14u));
            SET_GPR_U64(ctx, 3, READ64(sp + 0x10u));
            f[13] = FPU_MOV_S(f[12]);
            SET_GPR_U64(ctx, 2, READ64(sp + 0x18u));
            WRITE64(sp + 0x00u, GPR_U64(ctx, 3));
            WRITE64(sp + 0x08u, GPR_U64(ctx, 2));
            call(kBuildMatPacket, 0x0113DDACu);
            f[0] = fb(READ32(fx + 0x24u));
            SET_GPR_U64(ctx, 4, sp);
            f[12] = fb(kOne);
            SET_GPR_S32(ctx, 5, 7);
            SET_GPR_S32(ctx, 6, READ32(sp + 0x34u));
            SET_GPR_U64(ctx, 7, 0u);
            f[12] = FPU_SUB_S(f[12], f[0]);
            SET_GPR_U64(ctx, 8, 0u);
            call(kBuildEnvPacket, 0x0113DDD4u);
        }
        // The vertex packet for modes 1..5 and 7.
        SET_GPR_U64(ctx, 2, mode < 6u ? 1u : 0u);
        bool vertices = false;
        if (mode != 0u)
        {
            SET_GPR_U64(ctx, 4, list);
            if (GPR_U64(ctx, 2) != 0u)
                vertices = true;
            else
            {
                SET_GPR_S32(ctx, 2, 7);
                vertices = mode == 7u;
            }
        }
        if (vertices)
        {
            SET_GPR_U64(ctx, 5, written);
            call(kBuildVtxPacket, 0x0113DE5Cu);
        }
        call(kPtclmanEnd, 0x0113DE64u);
        finish();
    }

    // ---- manager_sub_0113E6C8 -----------------------------------------------
    constexpr uint32_t kDrawAnimated = 0x0113E6C8u;

    void drawAnimated(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kDrawAnimated, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x90u;
        const uint32_t sys = GPR_U32(ctx, 4);      // s2
        const uint32_t material = GPR_U32(ctx, 5); // s7
        const uint32_t fx = GPR_U32(ctx, 6);       // s3
        const uint64_t envArg = GPR_U64(ctx, 7);   // fp
        WRITE64(sp + 0x40u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x78u, ra);
        WRITE32(sp + 0x88u, bf(f[21]));
        WRITE32(sp + 0x80u, bf(f[20]));
        auto finish = [&]()
        {
            f[21] = fb(READ32(sp + 0x88u));
            f[20] = fb(READ32(sp + 0x80u));
            SET_GPR_U64(ctx, 29, entrySp);
            SET_GPR_U64(ctx, 31, ra);
            returnTo(ctx);
        };
        auto call = [&](uint32_t target, uint32_t retAddr) { callGuest(rdram, ctx, runtime, target, retAddr, sp); };

        SET_GPR_S32(ctx, 4, READ32(fx + 0x1Cu));
        call(kDisplayListGet, 0x0113E714u);
        const uint64_t list = GPR_U64(ctx, 2); // s6
        if (list == 0u)
            return finish();
        f[1] = fb(READ32(fx + 0x128u)); // the frame length
        f[0] = fb(kOne);
        uint32_t written = 0u; // s4
        SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
        f[21] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
        uint32_t p = READ32(sys + 0x128u); // s0
        if (GPR_U64(ctx, 3) != 0u)
        {
            uint32_t dst = static_cast<uint32_t>(list); // s1
            uint64_t n = 0u;                            // s5
            do
            {
                f[1] = fb(READ32(p + kPtLife));
                f[0] = 0.0f;
                setC(ctx, FPU_C_EQ_S(f[1], f[0]));
                SET_GPR_U32(ctx, 4, p);
                if (!cBit(ctx))
                {
                    // The frame: int(+0x90 x 1 / length) (cvt.w.s), as a float in f20.
                    f[0] = fb(READ32(p + 0x90u));
                    f[0] = FPU_MUL_S(f[0], f[21]);
                    {
                        const int32_t frame = FPU_CVT_W_S(f[0]);
                        std::memcpy(&f[1], &frame, 4);
                        SET_GPR_U32(ctx, 3, static_cast<uint32_t>(frame));
                        f[20] = FPU_CVT_S_W(frame);
                    }
                    record(rdram, ctx, runtime, sys, p, dst, 2);
                    f[0] = fb(READ32(p + kPtAngle));
                    SET_GPR_S32(ctx, 4, dst + 0x20u);
                    f[12] = fb(kQuarterPi);
                    ++written;
                    f[12] = FPU_ADD_S(f[0], f[12]);
                    sinCos(rdram, ctx, runtime, 0x0113E808u, sp);
                    // The texture offsets: int(+0x90 - frame x length) x fx+0x12c, frame x fx+0x130.
                    f[0] = fb(READ32(fx + 0x128u));
                    f[1] = fb(READ32(p + 0x90u));
                    f[0] = FPU_MUL_S(f[20], f[0]);
                    f[2] = fb(READ32(fx + 0x12Cu));
                    f[1] = FPU_SUB_S(f[1], f[0]);
                    {
                        const int32_t within = FPU_CVT_W_S(f[1]);
                        SET_GPR_U32(ctx, 2, static_cast<uint32_t>(within));
                        f[0] = FPU_CVT_S_W(within);
                    }
                    f[0] = FPU_MUL_S(f[0], f[2]);
                    WRITE32(dst + 0x28u, bf(f[0]));
                    f[1] = fb(READ32(fx + 0x130u));
                    f[20] = FPU_MUL_S(f[20], f[1]);
                    WRITE32(dst + 0x2Cu, bf(f[20]));
                    dst += kRecordSize;
                    SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
                }
                n = sx(static_cast<uint32_t>(n) + 1u);
                SET_GPR_U64(ctx, 2, n < GPR_U64(ctx, 3) ? 1u : 0u);
                p = READ32(p + kPtNext);
            } while (GPR_U64(ctx, 2) != 0u);
        }
        SET_GPR_S32(ctx, 2, kRendererHi);
        if (written == 0u)
            return finish();
        SET_GPR_S32(ctx, 3, READ32(kRendererOn));
        if (GPR_U64(ctx, 3) == 0u)
            return finish();
        SET_GPR_U64(ctx, 4, written);
        call(kDisplayListPut, 0x0113E87Cu);
        SET_GPR_S32(ctx, 4, 7);
        call(kTransMicroCode, 0x0113E884u);
        call(kPtclmanBegin, 0x0113E88Cu);
        f[1] = fb(READ32(fx + 0x12Cu));
        f[0] = fb(READ32(fx + 0x130u));
        WRITE32(sp + 0x10u, bf(f[1]));
        WRITE32(sp + 0x14u, bf(f[0]));
        WRITE32(sp + 0x18u, 0u);
        WRITE32(sp + 0x1Cu, 0u);
        f[12] = fb(kOne);
        SET_GPR_S32(ctx, 4, READ32(material + 0x14u));
        SET_GPR_U64(ctx, 3, READ64(sp + 0x10u));
        f[13] = FPU_MOV_S(f[12]);
        SET_GPR_U64(ctx, 2, READ64(sp + 0x18u));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 3));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 2));
        call(kBuildMatPacket, 0x0113E8C8u);
        f[0] = fb(READ32(fx + 0x24u));
        SET_GPR_U64(ctx, 4, sp);
        f[12] = fb(kOne);
        SET_GPR_U64(ctx, 6, envArg);
        SET_GPR_U64(ctx, 7, 0u);
        SET_GPR_U64(ctx, 8, 0u);
        f[12] = FPU_SUB_S(f[12], f[0]);
        SET_GPR_S32(ctx, 5, 7);
        call(kBuildEnvPacket, 0x0113E8F0u);
        SET_GPR_U64(ctx, 4, list);
        SET_GPR_U64(ctx, 5, written);
        call(kBuildVtxPacket, 0x0113E8FCu);
        call(kPtclmanEnd, 0x0113E904u);
        finish();
    }

    // ---- manager_sub_0113DEA0 -----------------------------------------------
    constexpr uint32_t kDrawSpan = 0x0113DEA0u;

    // The packets after either loop (`d` 0 for the first, 0x17c for the
    // second: the same code at another address). s0 the env table from the
    // relocated lui / addiu pair.
    void drawSpanPackets(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t material, uint32_t fx, uint64_t list,
                         uint32_t written, uint32_t d)
    {
        float *f = ctx->f;
        auto call = [&](uint32_t target, uint32_t retAddr) { callGuest(rdram, ctx, runtime, target, retAddr + d, sp); };
        // lui v0, hi / addiu s0, v0, lo (relocated: read live).
        SET_GPR_S32(ctx, 2, (sotc_decomp::codeWord(rdram, 0x0113DFCCu + d) & 0xFFFFu) << 16);
        const uint32_t envTable = GPR_U32(ctx, 2) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(sotc_decomp::codeWord(rdram, 0x0113DFD4u + d) & 0xFFFFu)));
        if (written == 0u)
            return;
        SET_GPR_S32(ctx, 2, kRendererHi);
        SET_GPR_S32(ctx, 3, READ32(kRendererOn));
        if (GPR_U64(ctx, 3) == 0u)
            return;
        SET_GPR_U64(ctx, 4, written);
        call(kDisplayListPut, 0x0113DFF0u);
        SET_GPR_S32(ctx, 4, 7);
        call(kTransMicroCode, 0x0113DFF8u);
        call(kPtclmanBegin, 0x0113E000u);
        f[13] = fb(READ32(fx + 0x5Cu));
        f[12] = fb(READ32(fx + 0x58u));
        WRITE32(sp + 0x24u, bf(f[13]));
        WRITE32(sp + 0x20u, bf(f[12]));
        WRITE32(sp + 0x28u, 0u);
        WRITE32(sp + 0x2Cu, 0u);
        SET_GPR_U64(ctx, 2, READ64(sp + 0x20u));
        SET_GPR_S32(ctx, 4, READ32(material + 0x14u));
        SET_GPR_U64(ctx, 3, READ64(sp + 0x28u));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 2));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 3));
        call(kBuildMatPacket, 0x0113E030u);
        f[0] = fb(READ32(fx + 0x24u));
        SET_GPR_S32(ctx, 4, sp + 0x10u);
        f[12] = fb(kOne);
        SET_GPR_S32(ctx, 6, envTable);
        SET_GPR_U64(ctx, 7, 0u);
        SET_GPR_U64(ctx, 8, 0u);
        f[12] = FPU_SUB_S(f[12], f[0]);
        SET_GPR_S32(ctx, 5, 3);
        call(kBuildEnvPacket, 0x0113E058u);
        SET_GPR_U64(ctx, 4, list);
        SET_GPR_U64(ctx, 5, written);
        call(kBuildVtxPacket, 0x0113E064u);
        call(kPtclmanEnd, 0x0113E06Cu);
    }

    void drawSpan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kDrawSpan, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x80u;
        const uint32_t sys = GPR_U32(ctx, 4);      // s2
        const uint32_t material = GPR_U32(ctx, 5); // s7
        const uint32_t fx = GPR_U32(ctx, 6);       // s3
        WRITE64(sp + 0x40u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x70u, ra);
        WRITE32(sp + 0x78u, bf(f[20]));
        auto finish = [&]()
        {
            f[20] = fb(READ32(sp + 0x78u));
            SET_GPR_U64(ctx, 29, entrySp);
            SET_GPR_U64(ctx, 31, ra);
            returnTo(ctx);
        };

        SET_GPR_S32(ctx, 2, READ32(material + 0x1B4u));
        const bool allLive = GPR_U64(ctx, 2) != 0u;
        uint32_t p = READ32(sys + 0x128u); // s1
        const uint32_t d = allLive ? 0x17Cu : 0u; // the second copy's addresses
        SET_GPR_S32(ctx, 4, READ32(fx + 0x1Cu));
        callGuest(rdram, ctx, runtime, kDisplayListGet, 0x0113DEECu + (allLive ? 0x194u : 0u), sp);
        const uint64_t list = GPR_U64(ctx, 2); // s6
        if (list == 0u)
            return finish();
        SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
        uint32_t written = 0u; // s5
        if (GPR_U64(ctx, 3) != 0u)
        {
            f[20] = 0.0f;
            uint32_t dst = static_cast<uint32_t>(list); // s0
            uint64_t n = 0u;                            // s4
            do
            {
                f[0] = fb(READ32(p + kPtLife));
                setC(ctx, FPU_C_EQ_S(f[0], f[20]));
                if (!allLive)
                {
                    SET_GPR_S32(ctx, 4, dst + 0x20u);
                    if (!cBit(ctx))
                    {
                        // 01142198(dst + 0x20, p): the span's last qword, v0 1 when drawn.
                        SET_GPR_U32(ctx, 5, p);
                        callGuest(rdram, ctx, runtime, kSpanTest, 0x0113DF28u, sp);
                        SET_GPR_U32(ctx, 4, p);
                        if (GPR_U64(ctx, 2) != 0u)
                        {
                            record(rdram, ctx, runtime, sys, p, dst, 2);
                            dst += kRecordSize;
                            ++written;
                        }
                        SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
                    }
                }
                else
                {
                    SET_GPR_U32(ctx, 4, p);
                    if (!cBit(ctx))
                    {
                        record(rdram, ctx, runtime, sys, p, dst, 3);
                        SET_GPR_VEC(ctx, 2, READ128(sys + 0x10u));
                        ++written;
                        WRITE128(dst + 0x20u, GPR_VEC(ctx, 2));
                        dst += kRecordSize;
                        SET_GPR_S32(ctx, 3, READ32(sys + 0x124u));
                    }
                }
                n = sx(static_cast<uint32_t>(n) + 1u);
                SET_GPR_U64(ctx, 2, n < GPR_U64(ctx, 3) ? 1u : 0u);
                p = READ32(p + kPtNext);
            } while (GPR_U64(ctx, 2) != 0u);
        }
        drawSpanPackets(rdram, ctx, runtime, sp, material, fx, list, written, d);
        finish();
    }

    // ---- manager_sub_01108960: the geometry pointer kick --------------------
    //
    // a0 the manager (s3: +0x338 the layout id table, +0x360 the effect
    // owner), a1 the object's layout id (s6), a2 an owner word (frame
    // +0x400), a3 the parent layout id (s4), t0 the parent's motion node
    // (s1: < 0 none, bit 16 set: ask the motion), t1 the geometry pointer
    // record (s2: +0 its type 1..48, +0x10 the position, +0x20 the
    // rotation quaternion, +0x30 the scale), t2 / t3 the kick's arguments
    // (fp, s0: +4 an id, +8 a byte flag), the fifth argument on the stack
    // (s5). f12 = f13. The type picks one of 48 entries of the jump table at
    // 0x01195030 (read live, as the code does); each case calls that
    // type's update (updateObjectGeo, updateParticleGeo, updateSEGeo, the
    // fog and light updates ...), most after placing the geometry pointer
    // on its parent: the parent's node matrix (the layout's +0x15c node
    // matrices, +0x154 node quaternions, or GetMotionNodeQuaternionPosToolCoord
    // into the frame turned into a matrix), the pointer's rotation composed
    // with the node's (VU0 quaternion product, normalized) and its position
    // through the node matrix, written to the object's layout (+0x134
    // position, +0x13c quaternion, +0x158 matrix).
    //
    // The VU0 blocks are five helpers below, each the generator's statements
    // of one instance verbatim (every instance's statements are the same
    // text: checked over the generated file, work-j/blocks.py) with the
    // addresses as parameters.
    inline void vuQuatMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t quat, uint32_t mat, uint32_t pos)
    {
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(quat, 0)));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(mat, 0), _mm_castps_si128(ctx->vu0_vf[14]));
        WRITE128(ADD32(mat, 16), _mm_castps_si128(ctx->vu0_vf[15]));
        WRITE128(ADD32(mat, 32), _mm_castps_si128(ctx->vu0_vf[16]));
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(ADD32(pos, 0)));
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(mat, 48), _mm_castps_si128(ctx->vu0_vf[4]));
    }
    inline void vuQuatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(a, 0)));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(ADD32(b, 0)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(dst, 0), _mm_castps_si128(ctx->vu0_vf[13]));
    }
    inline void vuQuatNormalize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q)
    {
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(q, 0)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(q, 0), _mm_castps_si128(ctx->vu0_vf[15]));
    }
    inline void vuTransform(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t vec, uint32_t mat, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(vec, 0)));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(mat, 0)));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(ADD32(mat, 16)));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(ADD32(mat, 32)));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(ADD32(mat, 48)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(dst, 0), _mm_castps_si128(ctx->vu0_vf[10]));
    }
    inline void vuQuatTransformW(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t vec, uint32_t wRow, uint32_t quat, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(vec, 0)));
        ctx->vu0_vf[18] = _mm_castsi128_ps(READ128(ADD32(wRow, 0)));
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(quat, 0)));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[18], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(dst, 0), _mm_castps_si128(ctx->vu0_vf[10]));
    }

    constexpr uint32_t kKick = 0x01108960u;
    constexpr uint32_t kJumpTable = 0x01195030u; // lui 0x119 / lw 0x5030: constant in the code
    constexpr uint32_t kLightDefault = 0x01194FF0u; // lui 0x119 / addiu 0x4ff0: the default colour qword pair
    // Same-module callees (constant jals).
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kUpdateObjectGeo = 0x01103FC8u, kUpdateDamageGeo = 0x01107420u, kUpdateParticleGeo = 0x01104368u;
    constexpr uint32_t kSetNodePosState = 0x01132C60u, kForceParent = 0x010F20C0u, kUpdateParentGeometry = 0x010F22B8u, kOutsideScriptCtrl = 0x010CC370u;
    constexpr uint32_t kUpdateVolumeShape = 0x01108658u, kSub01104868 = 0x01104868u, kUpdateWindGeo = 0x01104ED0u, kUpdateWindBoxGeo = 0x01105248u;
    constexpr uint32_t kMotionNodeQuatPos = 0x010B63F8u, kCameraVibration = 0x011694F8u, kUpdateSEGeo = 0x01105620u, kParticleResourceId = 0x01169778u;
    constexpr uint32_t kNpeBlendRatio = 0x0113F878u, kSub0110C530 = 0x0110C530u, kSub0110C660 = 0x0110C660u, kSub01104BA0 = 0x01104BA0u;
    constexpr uint32_t kKickLWS = 0x01106BF8u, kSetScaleElem = 0x010F2DB0u, kIsFogType = 0x010ED528u, kUpdateFogCol = 0x010ED440u, kUpdateFogAnim = 0x010ED3A8u;
    constexpr uint32_t kIsGlobalLightType = 0x010F8CB0u, kGlobalLightIn = 0x010F8D30u, kGlobalLightOut = 0x010F8D60u, kGlobalLightAmbient = 0x010F8E40u;
    constexpr uint32_t kIsDistantLightType = 0x010F8F90u, kGlobalLightDistant = 0x010F8EA8u, kIsGameLightType = 0x010F7678u, kUpdateGameLight = 0x010F7690u;
    constexpr uint32_t kUpdateClipGameLight = 0x010F69F0u;
    // Function pointers the code passes (same module: constant lui / addiu).
    constexpr uint32_t kParticleCallback = 0x0110B500u, kParticleCallbackB = 0x0110B518u, kParticleCallbackC = 0x0110B530u, kVolumeCallback = 0x0110C918u;
    constexpr uint32_t kAskMotion = 0x10000u; // the node index's "ask the motion" bit

    // Layout class fields.
    constexpr uint32_t kLcPosition = 0x134u, kLcQuat = 0x13Cu, kLcNodeQuats = 0x154u, kLcMatrix = 0x158u, kLcNodeMatrices = 0x15Cu, kLcEnvelope = 0x124u;

    struct Kick
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        // The code's callee-saved registers (round 48: copied into the
        // context before every guest call, see sync()).
        uint64_t s0, s1, s2, s3, s4, s5, s6, s7, fp;

        uint64_t r(int i) const { return GPR_U64(ctx, i); }
        void set(int i, uint64_t v) { SET_GPR_U64(ctx, i, v); }
        void set32(int i, uint32_t v) { SET_GPR_S32(ctx, i, v); }
        uint64_t lw(uint32_t a) const { return sx(READ32(a)); }
        uint64_t lb(uint32_t a) const { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int8_t>(READ8(a)))); }
        static uint32_t u(uint64_t v) { return static_cast<uint32_t>(v); }
        static uint64_t add(uint64_t a, uint32_t b) { return sx(static_cast<uint32_t>(a) + b); } // addiu / addu
        // Round 48: s0-s7 / fp in the context as the code holds them at a
        // call (a callee saves them into its frame; a later callee on the
        // kickLWS / ExecEff / kickEffect / 01104BA0 / 0110C530 / 0110C660
        // -> CreateScriptAnimationSequence -> 010EFD08 chain can read such a
        // stale slot and copy it into a new layout object, round 47's bug).
        // Every write of these registers on a path to a call is mirrored in
        // the fields above (delay-slot writes before the call included).
        void sync() const
        {
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_U64(ctx, 20, s4);
            SET_GPR_U64(ctx, 21, s5);
            SET_GPR_U64(ctx, 22, s6);
            SET_GPR_U64(ctx, 23, s7);
            SET_GPR_U64(ctx, 30, fp);
        }
        // jal `target` at `site` (ra site + 8, the frame's sp).
        void call(uint32_t target, uint32_t site)
        {
            sync();
            callGuest(rdram, ctx, runtime, target, site + 8u, sp);
        }
        // A relocated jal site.
        void callAt(uint32_t site) { call(jalTarget(rdram, site), site); }
        // lui at `hi` + the addiu at `lo`, both relocated (read live).
        uint64_t reloc(uint32_t hi, uint32_t lo) const { return sx(sotc_decomp::relocated(rdram, hi, lo)); }
        uint64_t relocHi(uint32_t hi) const { return sx(sotc_decomp::codeWord(rdram, hi) << 16); }
        int16_t relocLo(uint32_t lo) const { return static_cast<int16_t>(sotc_decomp::codeWord(rdram, lo) & 0xFFFFu); }

        // The parent's node frame (the code's shared shape at five cases): v0
        // the parent's layout class (the second GetLayoutClass). s1 < 0, or
        // the ask bit set: GetMotionNodeQuaternionPosToolCoord(quatOut,
        // posOut, s4, s1, 1) into the frame (`quatOff` / `posOff`) and the
        // matrix made from them at `matOff`; else the layout's node
        // quaternion (+0x154 + s1 x 16) and matrix (+0x15c + s1 x 64). The
        // quaternion is s0 in the code; the matrix a1, or s1 in 01104BA0's
        // case (`matInS1`: its own register order, s1 overwritten).
        struct Node
        {
            uint64_t quat, mat;
        };
        Node parentNode(uint32_t motionSite, uint32_t quatOff, uint32_t posOff, uint32_t matOff, bool matInS1)
        {
            const uint64_t lc = r(2);
            set(4, lc); // a0 = v0 (delay slot)
            bool ask = static_cast<int32_t>(u(s1)) < 0;
            if (!ask && static_cast<int32_t>(u(s1)) > 0)
            {
                set32(2, kAskMotion);
                set(2, s1 & r(2));
                ask = r(2) != 0u;
            }
            if (!ask)
            {
                set(5, lw(u(lc) + kLcNodeMatrices));
                if (matInS1)
                {
                    set32(2, u(s1) << 4);
                    set(4, lw(u(lc) + kLcNodeQuats));
                    set32(3, u(s1) << 6);
                    s1 = add(r(5), u(r(3)));
                    return {add(r(4), u(r(2))), s1};
                }
                set32(3, u(s1) << 6);
                set(4, lw(u(lc) + kLcNodeQuats));
                set32(2, u(s1) << 4);
                set(5, add(r(5), u(r(3))));
                return {add(r(4), u(r(2))), r(5)};
            }
            const uint64_t quat = sx(sp + quatOff);
            s0 = quat; // addiu s0, sp, quatOff before the motion call (round 48)
            set(7, s1);
            set(4, quat);
            set32(5, sp + posOff);
            set(6, s4);
            set32(8, 1u);
            if (matInS1)
                s1 = sx(sp + matOff);
            call(kMotionNodeQuatPos, motionSite);
            if (matInS1)
            {
                set32(3, sp + posOff);
                set(2, s1);
            }
            else
            {
                set32(5, sp + matOff);
                set32(2, sp + posOff);
                set(3, r(5));
            }
            vuQuatMatrix(rdram, ctx, runtime, u(quat), sp + matOff, sp + posOff);
            return {quat, sx(sp + matOff)};
        }
        // A layout's own matrix from its quaternion and position (v0 / v1 /
        // a0 loaded as the code loads them).
        void layoutMatrix(uint64_t lc)
        {
            set(4, lw(u(lc) + kLcPosition));
            set(3, lw(u(lc) + kLcMatrix));
            set(2, lw(u(lc) + kLcQuat));
            vuQuatMatrix(rdram, ctx, runtime, u(r(2)), u(r(3)), u(r(4)));
        }
    };

    // The cases of the kick (`k` holds the prologue's registers; each
    // returns to the epilogue, which only restores).

    // 0x11089e0 / 0x1108af0's first half: updateObjectGeo(s6, s4, s1, s2, fp, +4, 0).
    void caseObject(Kick &k, uint32_t site)
    {
        k.set(9, k.lw(Kick::u(k.s0) + 4u));
        k.set(4, k.s6);
        k.set(5, k.s4);
        k.set(6, k.s1);
        k.set(7, k.s2);
        k.set(8, k.fp);
        k.set(10, 0u);
        k.call(kUpdateObjectGeo, site);
    }

    // 0x1108af0: a layout object's geometry, then (+4 < 0, the pointer's
    // +0x40 above 0.5) its character attached to the node: the node's
    // position state, ForceParentLayoutObj, the parent geometry, the
    // character's outside-script status.
    void caseObjectAttach(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        (void)runtime;
        float *f = ctx->f;
        caseObject(k, 0x01108B08u);
        k.set(2, k.lw(Kick::u(k.s0) + 4u));
        if (static_cast<int32_t>(Kick::u(k.r(2))) >= 0)
            return;
        f[1] = fb(READ32(Kick::u(k.s2) + 0x40u));
        SET_GPR_S32(ctx, 1, 0x3F000000u);
        f[0] = fb(0x3F000000u);
        setC(ctx, FPU_C_OLT_S(f[0], f[1]));
        if (!cBit(ctx))
            return;
        k.set(4, k.s6);
        k.call(kGetLayoutClass, 0x01108B34u);
        k.s1 = k.lw(Kick::u(k.r(2)) + 0x17Cu);
        k.s0 = k.relocHi(0x01108B44u);
        if (k.s1 == 0u)
            return;
        k.set(7, k.relocHi(0x01108B48u));
        k.s0 = Kick::add(k.s0, static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01108B4Cu))));
        k.set32(4, k.sp + 0x20u);
        k.set(7, Kick::add(k.r(7), static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01108B54u)))));
        k.set(8, k.s0);
        k.set(6, 0u);
        k.set(5, k.s6);
        k.call(kSetNodePosState, 0x01108B60u);
        k.set(4, k.s1);
        k.set32(5, k.sp + 0x20u);
        k.call(kForceParent, 0x01108B6Cu);
        k.set(4, k.s1);
        k.call(kGetLayoutClass, 0x01108B74u);
        k.set(5, k.s0);
        k.set(4, k.r(2));
        k.set(6, k.r(5));
        k.call(kUpdateParentGeometry, 0x01108B84u);
        k.set(3, k.relocHi(0x01108B8Cu));
        k.set(2, k.lw(Kick::u(k.r(3)) + static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01108B90u)))));
        k.set(4, k.s1);
        k.set(5, k.lw(Kick::u(k.r(2)) + 4u));
        k.call(kOutsideScriptCtrl, 0x01108B98u);
    }

    // 0x1108a30: updateParticleGeo(+0x360, s6, a2, s4, s1, s2 + 0x10, fp, 0;
    // stack +4, the +8 byte, 0110B500, 0). When the flag word +8 has bit 16,
    // a2 is the layout id at +0x338[(+8 >> 17) x 8] if its class's +0x90 is
    // 1.0, else 0; otherwise a2 is the frame's +0x400 word.
    void caseParticle(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        (void)runtime;
        float *f = ctx->f;
        k.set(3, k.lw(Kick::u(k.s0) + 8u));
        k.set32(2, kAskMotion);
        k.set(2, k.r(3) & k.r(2));
        const bool linked = k.r(2) != 0u;
        k.set32(2, Kick::u(k.r(3)) >> 17);
        if (linked)
        {
            k.set(3, k.lw(Kick::u(k.s3) + 0x338u));
            k.set32(2, Kick::u(k.r(2)) << 3);
            k.set32(2, Kick::u(k.r(2)) + Kick::u(k.r(3)));
            k.s5 = k.lw(Kick::u(k.r(2)));
            k.set(4, k.s5);
            k.call(kGetLayoutClass, 0x01108A54u);
            f[1] = fb(READ32(Kick::u(k.r(2)) + 0x90u));
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(kOne);
            setC(ctx, FPU_C_EQ_S(f[1], f[0]));
            k.set32(3, 1u);
            if (!cBit(ctx))
                k.set(3, 0u);
            k.set(6, 0u);
            k.set(4, k.lw(Kick::u(k.s3) + 0x360u));
            k.set(12, k.lb(Kick::u(k.s0) + 8u));
            k.set32(2, 0x01110000u);
            if (k.r(3) != 0u) // movn a2, s5, v1: the whole register
                SET_GPR_VEC(ctx, 6, Ps2GprVecWithLow(ctx->r[21], k.s5));
            k.set(3, k.lw(Kick::u(k.s0) + 4u));
            k.set32(2, kParticleCallback);
            k.set(5, k.s6);
        }
        else
        {
            k.set(4, k.lw(Kick::u(k.s3) + 0x360u));
            k.set32(2, 0x01110000u);
            k.set(12, k.lb(Kick::u(k.s0) + 8u));
            k.set32(2, kParticleCallback);
            k.set(3, k.lw(Kick::u(k.s0) + 4u));
            k.set(5, k.s6);
            k.set(6, k.lw(k.sp + 0x400u));
        }
        k.set(7, k.s4);
        k.set(8, k.s1);
        k.set(9, Kick::add(k.s2, 0x10u));
        k.set(10, k.fp);
        k.set(11, 0u);
        WRITE32(k.sp + 0x00u, Kick::u(k.r(3)));
        WRITE32(k.sp + 0x08u, Kick::u(k.r(12)));
        WRITE32(k.sp + 0x10u, Kick::u(k.r(2)));
        WRITE32(k.sp + 0x18u, 0u);
        k.call(kUpdateParticleGeo, 0x01108ADCu);
    }

    // 0x1108c70: a camera vibration source on a node: the pointer's
    // position through the parent node's matrix into the object's layout
    // position, the object's matrix, then (the +8 byte 0 and the pointer's
    // +0x20 byte set) SetGeometryPointerNodePosState and
    // SequenceKickCameraVibration(+4, state).
    void caseCameraVibration(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint32_t sp = k.sp;
        k.set(4, k.s6);
        k.fp = k.lw(Kick::u(k.s0) + 4u);
        k.s7 = k.lb(Kick::u(k.s0) + 8u);
        k.call(kGetLayoutClass, 0x01108C78u);
        k.s3 = k.r(2);
        k.set(4, k.s4);
        k.s5 = k.lw(Kick::u(k.s3) + kLcPosition);
        k.call(kGetLayoutClass, 0x01108C88u);
        k.s2 = Kick::add(k.s2, 0x10u);
        k.set(3, k.r(2));
        bool ask = static_cast<int32_t>(Kick::u(k.s1)) <= 0;
        if (!ask)
        {
            k.set32(2, kAskMotion);
            k.set(2, k.s1 & k.r(2));
            k.set32(4, sp + 0xB0u);
            ask = k.r(2) != 0u;
        }
        if (!ask)
        {
            k.set(3, k.lw(Kick::u(k.r(3)) + kLcNodeMatrices));
            k.set32(2, Kick::u(k.s1) << 6);
            k.s0 = Kick::add(k.r(3), Kick::u(k.r(2)));
        }
        else
        {
            k.set32(4, sp + 0xB0u);
            k.set(6, k.s4);
            k.set(7, k.s1);
            k.set32(5, sp + 0xA0u);
            k.set32(8, 1u);
            k.s0 = sx(sp + 0x60u);
            k.call(kMotionNodeQuatPos, 0x01108CD4u);
            k.set(2, k.s0);
            k.set32(3, sp + 0xB0u);
            k.set32(4, sp + 0xA0u);
            vuQuatMatrix(rdram, ctx, runtime, sp + 0xB0u, Kick::u(k.s0), sp + 0xA0u);
        }
        vuTransform(rdram, ctx, runtime, Kick::u(k.s2), Kick::u(k.s0), Kick::u(k.s5));
        k.layoutMatrix(k.s3);
        if (k.s7 != 0u)
            return;
        k.set(2, k.lb(Kick::u(k.s2) + 0x20u));
        k.set(7, k.relocHi(0x01108E1Cu));
        if (k.r(2) == 0u)
            return;
        k.set(8, k.relocHi(0x01108E20u));
        k.set(5, k.s6);
        k.set32(4, sp + 0xC0u);
        k.set(7, Kick::add(k.r(7), static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01108E2Cu)))));
        k.set(8, Kick::add(k.r(8), static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01108E30u)))));
        k.set(6, 0u);
        k.call(kSetNodePosState, 0x01108E34u);
        k.set(4, k.fp);
        k.set32(5, sp + 0xC0u);
        k.call(kCameraVibration, 0x01108E40u);
    }

    // 0x1108e80 / 0x1108ed8: updateParticleGeo(0, s6, +0x400, s4, s1, s2 +
    // 0x10, fp, s5; stack +4, the +8 byte, 0110B518, 0110B530); the second
    // then (s5 not 0) the emitter's blend ratio: set_npe_blend_ratio_particle_emitter(+4,
    // KickParticleGetResourceId(s5), f12 the pointer's +0x50).
    void caseParticleKick(Kick &k, bool blend)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        (void)runtime;
        const uint64_t byte8 = k.lb(Kick::u(k.s0) + 8u);
        const uint64_t id = k.lw(Kick::u(k.s0) + 4u);
        if (!blend)
        {
            k.set(13, byte8);
            k.set(12, id);
            k.set32(3, kParticleCallbackB);
            k.set32(2, kParticleCallbackC);
            k.set(6, k.lw(k.sp + 0x400u));
            k.set(9, Kick::add(k.s2, 0x10u));
        }
        else
        {
            k.set(2, byte8);
            k.s3 = Kick::add(k.s2, 0x10u);
            k.set32(3, kParticleCallbackB);
            k.set32(12, kParticleCallbackC);
            k.s2 = id;
            k.set(6, k.lw(k.sp + 0x400u));
            k.set(9, k.s3);
        }
        k.set(5, k.s6);
        k.set(7, k.s4);
        k.set(8, k.s1);
        k.set(10, k.fp);
        k.set(11, k.s5);
        k.set(4, 0u);
        if (!blend)
        {
            WRITE32(k.sp + 0x00u, Kick::u(k.r(12)));
            WRITE32(k.sp + 0x08u, Kick::u(k.r(13)));
            WRITE32(k.sp + 0x10u, Kick::u(k.r(3)));
            WRITE32(k.sp + 0x18u, Kick::u(k.r(2)));
            k.call(kUpdateParticleGeo, 0x01108EC4u);
            return;
        }
        WRITE32(k.sp + 0x08u, Kick::u(k.r(2)));
        WRITE32(k.sp + 0x10u, Kick::u(k.r(3)));
        WRITE32(k.sp + 0x18u, Kick::u(k.r(12)));
        WRITE32(k.sp + 0x00u, Kick::u(k.s2));
        k.call(kUpdateParticleGeo, 0x01108F20u);
        if (k.s5 == 0u)
            return;
        k.set(4, k.s5);
        k.call(kParticleResourceId, 0x01108F30u);
        k.set(4, k.s2);
        ctx->f[12] = fb(READ32(Kick::u(k.s3) + 0x40u));
        k.set(5, k.r(2));
        k.call(kNpeBlendRatio, 0x01108F40u);
    }

    // 0x1108f50 / 0x11091d8: an attached object's place (0110C530 /
    // 0110C660(+0x360, +4, +0x400, s6, s4)): its rotation = node quaternion
    // x the pointer's (+0x20), normalized, into the layout's +0x13c; its
    // position through the node matrix into +0x134; then the layout's
    // quaternion turned by SetQuaternionByAxisRotateVWithNoRegularize(-0x8000
    // about the relocated axis) and its matrix made.
    struct AttachCase
    {
        uint32_t ownerOff, idOff;                            // the frame words of +0x360 / +4
        uint32_t siteLc1, siteLc2, siteMotion;               // jal sites
        uint32_t quatOff, posOff, matOff, turnOff;           // frame blocks
        uint32_t siteAxisHi, siteAxisLo, siteTurn, siteCall; // relocated axis, the turn's jal, the final call
        uint32_t target;
    };
    constexpr AttachCase kAttachC530 = {0x404u, 0x408u, 0x01108F68u, 0x01108F7Cu, 0x01108FD4u, 0x140u, 0x150u, 0x100u, 0x160u,
                                        0x011090E0u, 0x011090E4u, 0x011090F0u, 0x011091C4u, kSub0110C530};
    constexpr AttachCase kAttachC660 = {0x40Cu, 0x410u, 0x011091F0u, 0x01109204u, 0x0110925Cu, 0x1B0u, 0x1C0u, 0x170u, 0x1D0u,
                                        0x01109368u, 0x0110936Cu, 0x01109378u, 0x0110944Cu, kSub0110C660};

    void caseAttach(Kick &k, const AttachCase &c)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint32_t sp = k.sp;
        k.s3 = k.lw(Kick::u(k.s3) + 0x360u);
        k.set(4, k.s6);
        k.fp = Kick::add(k.s2, 0x20u);
        k.s2 = Kick::add(k.s2, 0x10u);
        WRITE32(sp + c.ownerOff, Kick::u(k.s3));
        k.s0 = k.lw(Kick::u(k.s0) + 4u);
        WRITE32(sp + c.idOff, Kick::u(k.s0));
        k.call(kGetLayoutClass, c.siteLc1);
        k.s5 = k.r(2);
        k.set(4, k.s4);
        k.s3 = k.lw(Kick::u(k.s5) + kLcQuat);
        k.s7 = k.lw(Kick::u(k.s5) + kLcPosition);
        k.call(kGetLayoutClass, c.siteLc2);
        const Kick::Node node = k.parentNode(c.siteMotion, c.quatOff, c.posOff, c.matOff, false);
        k.s0 = node.quat;
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), Kick::u(k.fp), Kick::u(k.s3));
        vuQuatNormalize(rdram, ctx, runtime, Kick::u(k.s3));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s2), Kick::u(node.mat), Kick::u(k.s7));
        k.set(6, k.reloc(c.siteAxisHi, c.siteAxisLo));
        k.set32(4, sp + c.turnOff);
        k.set32(5, 0xFFFF8000u);
        k.s0 = k.lw(Kick::u(k.s5) + kLcQuat);
        k.callAt(c.siteTurn);
        k.set32(2, sp + c.turnOff);
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), sp + c.turnOff, Kick::u(k.s0));
        k.layoutMatrix(k.s5);
        k.set(4, k.lw(sp + c.ownerOff));
        k.set(7, k.s6);
        k.set(5, k.lw(sp + c.idOff));
        k.set(8, k.s4);
        k.set(6, k.lw(sp + 0x400u));
        k.call(c.target, c.siteCall);
    }

    // 0x1109460: 01104BA0(+0x360, +4, +0x400, s6, s4, the +0xa byte, frame
    // +0x1e0 the world position, +0x200 the world rotation (node x pointer,
    // normalized); stack word: frame +0x1f0, the pointer's +0x30 vector
    // through the world rotation plus the world position; f12 that vector's
    // squared length (vmul / vaddy / vaddz, x lane)).
    void caseWorldFrame(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint32_t sp = k.sp;
        k.set(2, k.lw(Kick::u(k.s0) + 4u));
        k.set(4, k.s4);
        k.fp = k.lw(Kick::u(k.s3) + 0x360u);
        k.s3 = sx(sp + 0x200u);
        WRITE32(sp + 0x418u, Kick::u(k.r(2)));
        k.s5 = sx(sp + 0x1E0u);
        k.s7 = Kick::add(k.s2, 0x20u);
        k.s2 = Kick::add(k.s2, 0x10u);
        k.s0 = k.lb(Kick::u(k.s0) + 0xAu);
        WRITE32(sp + 0x414u, Kick::u(k.s0));
        k.call(kGetLayoutClass, 0x01109484u);
        const Kick::Node node = k.parentNode(0x011094E0u, 0x250u, 0x260u, 0x210u, true);
        k.s0 = node.quat;
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), Kick::u(k.s7), Kick::u(k.s3));
        vuQuatNormalize(rdram, ctx, runtime, Kick::u(k.s3));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s2), Kick::u(k.s1), Kick::u(k.s5));
        k.set(3, k.s3);
        k.set32(4, sp + 0x1F0u);
        k.set(2, Kick::add(k.s2, 0x20u));
        vuQuatTransformW(rdram, ctx, runtime, Kick::u(k.r(2)), Kick::u(k.s5), Kick::u(k.r(3)), sp + 0x1F0u);
        k.set(3, Kick::add(k.s2, 0x20u));
        // lqc2 vf7 / vmul.xyz / vaddy.x / vaddz.x / qmfc2.ni v0 / mtc1 v0, f12.
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(ADD32(Kick::u(k.r(3)), 0)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[7]));
        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&ctx->f[12], &bits, sizeof(bits)); }
        k.set(5, k.lw(sp + 0x418u));
        k.set32(2, sp + 0x1F0u);
        k.set(6, k.lw(sp + 0x400u));
        k.set(4, k.fp);
        k.set(9, k.lw(sp + 0x414u));
        k.set(7, k.s6);
        k.set(8, k.s4);
        k.set32(10, sp + 0x1E0u);
        k.set32(11, sp + 0x200u);
        WRITE32(sp + 0x00u, Kick::u(k.r(2)));
        k.call(kSub01104BA0, 0x011096B4u);
    }

    // 0x11096c8: a light wave source (kickLWS): the object placed on the
    // node as caseAttach (without the turn), its matrix; then, the
    // pointer's +0x30 byte and +4 set, SetGeometryPointerNodePosState and
    // kickLWS(+4, +0x400, s4, fp, +0x360, state).
    void caseKickLWS(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint32_t sp = k.sp;
        k.s3 = k.lw(Kick::u(k.s3) + 0x360u);
        k.set(4, k.s6);
        k.s7 = Kick::add(k.s2, 0x10u);
        WRITE32(sp + 0x41Cu, Kick::u(k.s3));
        k.s0 = k.lw(Kick::u(k.s0) + 4u);
        WRITE32(sp + 0x420u, Kick::u(k.s0));
        k.call(kGetLayoutClass, 0x011096DCu);
        k.s5 = k.r(2);
        k.set(2, Kick::add(k.s2, 0x20u));
        k.s3 = k.lw(Kick::u(k.s5) + kLcQuat);
        k.set(4, k.s4);
        WRITE32(sp + 0x428u, Kick::u(k.r(2)));
        k.set(2, k.lw(Kick::u(k.s5) + kLcPosition));
        WRITE32(sp + 0x424u, Kick::u(k.r(2)));
        k.call(kGetLayoutClass, 0x011096FCu);
        const Kick::Node node = k.parentNode(0x01109754u, 0x2B0u, 0x2C0u, 0x270u, false);
        k.s0 = node.quat;
        k.set(2, k.lw(sp + 0x428u));
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), Kick::u(k.r(2)), Kick::u(k.s3));
        vuQuatNormalize(rdram, ctx, runtime, Kick::u(k.s3));
        k.set(2, k.lw(sp + 0x424u));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s7), Kick::u(node.mat), Kick::u(k.r(2)));
        k.layoutMatrix(k.s5);
        const uint64_t flag = k.lb(Kick::u(k.s2) + 0x30u);
        k.set(2, k.lw(sp + 0x420u)); // beqz's delay slot
        if (flag == 0u)
            return;
        k.set(7, k.relocHi(0x011098FCu));
        if (k.r(2) == 0u)
            return;
        k.set(8, k.relocHi(0x01109900u));
        k.set(5, k.s6);
        k.set(7, Kick::add(k.r(7), static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x01109908u)))));
        k.set(8, Kick::add(k.r(8), static_cast<uint32_t>(static_cast<int32_t>(k.relocLo(0x0110990Cu)))));
        k.set32(4, sp + 0x2D0u);
        k.set(6, 0u);
        k.call(kSetNodePosState, 0x01109914u);
        k.set(6, k.s4);
        k.set(4, k.lw(sp + 0x420u));
        k.set(7, k.fp);
        k.set(5, k.lw(sp + 0x400u));
        k.set32(9, sp + 0x2D0u);
        k.set(8, k.lw(sp + 0x41Cu));
        k.call(kKickLWS, 0x01109930u);
    }

    // 0x1109940: the object placed on the node (rotation and position), no call after.
    void casePlace(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        k.set(4, k.s6);
        k.s5 = Kick::add(k.s2, 0x10u);
        k.call(kGetLayoutClass, 0x01109944u);
        k.set(4, k.s4);
        k.s6 = k.lw(Kick::u(k.r(2)) + kLcPosition);
        k.s2 = Kick::add(k.s2, 0x20u);
        k.s3 = k.lw(Kick::u(k.r(2)) + kLcQuat);
        k.call(kGetLayoutClass, 0x01109958u);
        const Kick::Node node = k.parentNode(0x011099B4u, 0x350u, 0x360u, 0x310u, false);
        k.s0 = node.quat;
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), Kick::u(k.s2), Kick::u(k.s3));
        vuQuatNormalize(rdram, ctx, runtime, Kick::u(k.s3));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s5), Kick::u(node.mat), Kick::u(k.s6));
    }

    // 0x1109ac8: the object placed in its parent's frame (the parent's
    // quaternion x the pointer's, its position through the parent's
    // matrix) and SetLayoutClassScaleElem(class, pointer + 0x30, 1).
    void caseScale(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        k.set(4, k.s4);
        k.s1 = Kick::add(k.s2, 0x10u);
        k.call(kGetLayoutClass, 0x01109ACCu);
        k.set(4, k.s6);
        k.s0 = k.r(2);
        k.call(kGetLayoutClass, 0x01109AD8u);
        k.set(4, k.lw(Kick::u(k.r(2)) + kLcQuat));
        k.set(3, Kick::add(k.s2, 0x20u));
        k.set(6, k.lw(Kick::u(k.s0) + kLcQuat));
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.r(6)), Kick::u(k.r(3)), Kick::u(k.r(4)));
        k.set(5, k.lw(Kick::u(k.s0) + kLcMatrix));
        k.set(3, k.lw(Kick::u(k.r(2)) + kLcPosition));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s1), Kick::u(k.r(5)), Kick::u(k.r(3)));
        k.set(4, k.r(2));
        k.set(5, Kick::add(k.s2, 0x30u));
        k.set32(6, 1u);
        k.call(kSetScaleElem, 0x01109B58u);
    }

    // The fog / light cases. `plain`: is-type(s5[, s2]); when it says no,
    // the parent class's +0x124 must be set; then update(s5, arg).
    // `placed`: is-type(s5); yes, or +0x124 set: the pointer's quaternion
    // (+0x20) turned by the parent's (+0x13c) and its position (+0x10)
    // through the parent's matrix (+0x158), in place; update(s5, s2).
    struct EnvCase
    {
        uint32_t siteLc, siteIs, isTarget, siteUpdate, update;
        bool isTakesPointer; // IsEnvelopeDistantLightType(s5, s2)
        bool placed;
        bool needEnvelope;   // UpdateFogAnim: is-type no is a plain return
    };

    void caseEnv(Kick &k, const EnvCase &c)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint64_t arg = c.update == kUpdateFogCol ? Kick::add(k.s2, 0x10u) : k.s2;
        k.set(4, k.s4);
        if (c.update == kUpdateFogCol)
            k.s1 = arg;
        k.call(kGetLayoutClass, c.siteLc);
        k.s0 = k.r(2);
        if (k.s5 == 0u)
            return;
        k.set(4, k.s5);
        if (c.isTakesPointer)
            k.set(5, k.s2);
        k.call(c.isTarget, c.siteIs);
        if (!c.placed)
        {
            k.set(4, k.s5);
            if (k.r(2) == 0u)
            {
                k.set(2, k.lw(Kick::u(k.s0) + kLcEnvelope));
                if (k.r(2) == 0u)
                    return;
            }
            k.set(5, arg);
            k.call(c.update, c.siteUpdate);
            return;
        }
        const bool yes = k.r(2) != 0u;
        k.set(2, Kick::add(k.s2, 0x20u));
        if (!yes)
        {
            if (c.needEnvelope)
                return;
            k.set(2, k.lw(Kick::u(k.s0) + kLcEnvelope));
            const bool envelope = k.r(2) != 0u;
            k.set(2, Kick::add(k.s2, 0x20u));
            if (!envelope)
                return;
        }
        k.set(3, k.lw(Kick::u(k.s0) + kLcQuat));
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.r(3)), Kick::u(k.r(2)), Kick::u(k.r(2)));
        k.set(4, k.lw(Kick::u(k.s0) + kLcMatrix));
        k.set(2, Kick::add(k.s2, 0x10u));
        vuTransform(rdram, ctx, runtime, Kick::u(k.r(2)), Kick::u(k.r(4)), Kick::u(k.r(2)));
        k.set(4, k.s5);
        k.set(5, k.s2);
        k.call(c.update, c.siteUpdate);
    }
    constexpr EnvCase kFogCol = {0x01109B6Cu, 0x01109B7Cu, kIsFogType, 0x01109B98u, kUpdateFogCol, false, false, false};
    constexpr EnvCase kFogAnim = {0x01109BA8u, 0x01109BB8u, kIsFogType, 0x01109C34u, kUpdateFogAnim, false, true, true};
    constexpr EnvCase kLightIn = {0x01109C48u, 0x01109C58u, kIsGlobalLightType, 0x01109C74u, kGlobalLightIn, false, false, false};
    constexpr EnvCase kLightOut = {0x01109C88u, 0x01109C98u, kIsGlobalLightType, 0x01109D20u, kGlobalLightOut, false, true, false};
    constexpr EnvCase kLightAmbient = {0x01109D30u, 0x01109D40u, kIsGlobalLightType, 0x01109D5Cu, kGlobalLightAmbient, false, false, false};
    constexpr EnvCase kLightDistant = {0x01109D70u, 0x01109D84u, kIsDistantLightType, 0x01109DA0u, kGlobalLightDistant, true, false, false};
    constexpr EnvCase kGameLight = {0x01109DB0u, 0x01109DC0u, kIsGameLightType, 0x01109E48u, kUpdateGameLight, false, true, false};
    constexpr EnvCase kClipGameLight = {0x01109E58u, 0x01109E68u, kIsGameLightType, 0x01109EF0u, kUpdateClipGameLight, false, true, false};

    // 0x1109f00 (types 39..42): the object placed on the node with the
    // pointer's +0x30 scale x the colour (the parent class's +0x90, or the
    // default pair at 0x01194FF0) kept in the class's +0x90.
    void caseTinted(Kick &k)
    {
        R5900Context *ctx = k.ctx;
        uint8_t *rdram = k.rdram;
        PS2Runtime *runtime = k.runtime;
        const uint32_t sp = k.sp;
        k.set32(2, 0x01190000u);
        k.set(4, k.s6);
        k.set32(2, kLightDefault);
        k.set(3, READ64(kLightDefault + 8u));
        k.set(5, READ64(kLightDefault));
        WRITE64(sp + 0x398u, k.r(3));
        WRITE64(sp + 0x390u, k.r(5));
        k.call(kGetLayoutClass, 0x01109F18u);
        k.s0 = k.r(2);
        if (k.s4 != 0u)
        {
            k.set(4, k.s4);
            k.call(kGetLayoutClass, 0x01109F28u);
            if (k.r(2) != 0u)
            {
                SET_GPR_VEC(ctx, 2, READ128(Kick::u(k.r(2)) + 0x90u));
                WRITE128(sp + 0x390u, GPR_VEC(ctx, 2));
            }
        }
        k.set(2, static_cast<int64_t>(k.s7) < 43 ? 1u : 0u);
        if (k.r(2) == 0u)
            return;
        k.set(2, static_cast<int64_t>(k.s7) < 39 ? 1u : 0u);
        if (k.r(2) != 0u)
            return;
        SET_GPR_VEC(ctx, 3, READ128(Kick::u(k.s2) + 0x10u));
        k.set32(5, sp + 0x380u);
        k.set(4, Kick::add(k.s2, 0x30u));
        k.set32(2, sp + 0x390u);
        WRITE128(sp + 0x370u, GPR_VEC(ctx, 3));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(Kick::u(k.r(4)), 0)));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(Kick::u(k.r(2)), 0)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(Kick::u(k.r(5)), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        k.set(4, k.s4);
        k.s5 = Kick::add(k.s2, 0x20u);
        k.s7 = sx(sp + 0x370u);
        k.s3 = k.lw(Kick::u(k.s0) + kLcPosition);
        k.s2 = k.lw(Kick::u(k.s0) + kLcQuat);
        k.call(kGetLayoutClass, 0x01109F8Cu);
        k.s6 = Kick::add(k.s0, 0x90u);
        const Kick::Node node = k.parentNode(0x01109FECu, 0x3E0u, 0x3F0u, 0x3A0u, false);
        k.s0 = node.quat;
        vuQuatMul(rdram, ctx, runtime, Kick::u(k.s0), Kick::u(k.s5), Kick::u(k.s2));
        vuQuatNormalize(rdram, ctx, runtime, Kick::u(k.s2));
        vuTransform(rdram, ctx, runtime, Kick::u(k.s7), Kick::u(node.mat), Kick::u(k.s3));
        SET_GPR_VEC(ctx, 2, READ128(sp + 0x380u));
        WRITE128(Kick::u(k.s6), GPR_VEC(ctx, 2));
    }

    void kick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kKick, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        Kick k{rdram, ctx, runtime, static_cast<uint32_t>(entrySp) - 0x480u, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        const uint32_t sp = k.sp;
        // The prologue.
        ctx->f[12] = FPU_MOV_S(ctx->f[13]);
        WRITE64(sp + 0x430u, GPR_U64(ctx, 16));
        k.s0 = GPR_U64(ctx, 11);
        WRITE64(sp + 0x438u, GPR_U64(ctx, 17));
        k.s1 = GPR_U64(ctx, 8);
        WRITE64(sp + 0x440u, GPR_U64(ctx, 18));
        k.s2 = GPR_U64(ctx, 9);
        WRITE64(sp + 0x448u, GPR_U64(ctx, 19));
        k.s3 = GPR_U64(ctx, 4);
        WRITE64(sp + 0x450u, GPR_U64(ctx, 20));
        k.s4 = GPR_U64(ctx, 7);
        WRITE64(sp + 0x458u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x460u, GPR_U64(ctx, 22));
        k.s6 = GPR_U64(ctx, 5);
        WRITE64(sp + 0x470u, GPR_U64(ctx, 30));
        k.fp = GPR_U64(ctx, 10);
        WRITE64(sp + 0x468u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x478u, ra);
        k.s5 = k.lw(static_cast<uint32_t>(entrySp));
        k.s7 = k.lw(Kick::u(k.s2));
        k.set32(3, Kick::u(k.s7) - 1u);
        k.set(2, k.r(3) < 0x30u ? 1u : 0u);
        WRITE32(sp + 0x400u, GPR_U32(ctx, 6));
        auto finish = [&]()
        {
            // The epilogue's reloads (round 48: the cases change s0-s7 / fp
            // in the context through Kick::call).
            SET_GPR_U64(ctx, 16, READ64(sp + 0x430u));
            SET_GPR_U64(ctx, 17, READ64(sp + 0x438u));
            SET_GPR_U64(ctx, 18, READ64(sp + 0x440u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0x448u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0x450u));
            SET_GPR_U64(ctx, 21, READ64(sp + 0x458u));
            SET_GPR_U64(ctx, 22, READ64(sp + 0x460u));
            SET_GPR_U64(ctx, 23, READ64(sp + 0x468u));
            SET_GPR_U64(ctx, 30, READ64(sp + 0x470u));
            SET_GPR_U64(ctx, 29, entrySp);
            SET_GPR_U64(ctx, 31, ra);
            returnTo(ctx);
        };
        if (k.r(2) == 0u)
            return finish();
        k.set32(2, Kick::u(k.r(3)) << 2);
        k.set32(3, (kJumpTable & 0xFFFF0000u) + Kick::u(k.r(2))); // lui 0x119 / addu
        k.set(3, k.lw(Kick::u(k.r(3)) + (kJumpTable & 0xFFFFu)));  // lw 0x5030: the table word for type - 1
        switch (Kick::u(k.r(3)))
        {
        case 0x011089E0u: caseObject(k, 0x011089F8u); break;
        case 0x01108A08u:
            k.set(8, k.lw(Kick::u(k.s0) + 4u));
            k.set(4, k.s6);
            k.set(5, k.s4);
            k.set(6, k.s1);
            k.set(7, k.s2);
            k.set(9, k.s7);
            k.call(kUpdateDamageGeo, 0x01108A1Cu);
            break;
        case 0x01108A30u: caseParticle(k); break;
        case 0x01108AF0u: caseObjectAttach(k); break;
        case 0x01108BA8u:
        case 0x01108C48u:
            // updateVolumeShapePQSGeo(s6, s4, s1, s2, +4, the +8 byte, callback or 0).
            k.set(9, k.lb(Kick::u(k.s0) + 8u));
            k.set(8, k.lw(Kick::u(k.s0) + 4u));
            k.set(4, k.s6);
            k.set(5, k.s4);
            k.set(6, k.s1);
            k.set(7, k.s2);
            k.set32(10, Kick::u(k.r(3)) == 0x01108BA8u ? kVolumeCallback : 0u);
            k.call(kUpdateVolumeShape, Kick::u(k.r(3)) == 0x01108BA8u ? 0x01108BC4u : 0x01108C60u);
            break;
        case 0x01108BD8u:
            k.set(8, k.lw(Kick::u(k.s0) + 4u));
            k.set(4, k.s6);
            k.set(5, k.s4);
            k.set(6, k.s1);
            k.set(7, k.s2);
            k.call(kSub01104868, 0x01108BE8u);
            break;
        case 0x01108BF8u:
        case 0x01108C20u:
        {
            // updateWindGeo / updateWindBoxGeo(s6, s4, s1, s2, +4, the +8 byte).
            const bool box = Kick::u(k.r(3)) == 0x01108C20u;
            k.set(9, k.lb(Kick::u(k.s0) + 8u));
            k.set(4, k.s6);
            k.set(8, k.lw(Kick::u(k.s0) + 4u));
            k.set(5, k.s4);
            k.set(6, k.s1);
            k.set(7, k.s2);
            k.call(box ? kUpdateWindBoxGeo : kUpdateWindGeo, box ? 0x01108C34u : 0x01108C0Cu);
            break;
        }
        case 0x01108C70u: caseCameraVibration(k); break;
        case 0x01108E50u:
            // updateSEGeo(+0x360, s6, +0x400, s4, s1, s2 + 0x10, +4, the +8 byte).
            k.set(4, k.lw(Kick::u(k.s3) + 0x360u));
            k.set(5, k.s6);
            k.set(11, k.lb(Kick::u(k.s0) + 8u));
            k.set(7, k.s4);
            k.set(6, k.lw(sp + 0x400u));
            k.set(8, k.s1);
            k.set(10, k.lw(Kick::u(k.s0) + 4u));
            k.set(9, Kick::add(k.s2, 0x10u));
            k.call(kUpdateSEGeo, 0x01108E6Cu);
            break;
        case 0x01108E80u: caseParticleKick(k, false); break;
        case 0x01108ED8u: caseParticleKick(k, true); break;
        case 0x01108F50u: caseAttach(k, kAttachC530); break;
        case 0x011091D8u: caseAttach(k, kAttachC660); break;
        case 0x01109460u: caseWorldFrame(k); break;
        case 0x011096C8u: caseKickLWS(k); break;
        case 0x01109940u: casePlace(k); break;
        case 0x01109AC8u: caseScale(k); break;
        case 0x01109B68u: caseEnv(k, kFogCol); break;
        case 0x01109BA8u: caseEnv(k, kFogAnim); break;
        case 0x01109C48u: caseEnv(k, kLightIn); break;
        case 0x01109C88u: caseEnv(k, kLightOut); break;
        case 0x01109D30u: caseEnv(k, kLightAmbient); break;
        case 0x01109D70u: caseEnv(k, kLightDistant); break;
        case 0x01109DB0u: caseEnv(k, kGameLight); break;
        case 0x01109E58u: caseEnv(k, kClipGameLight); break;
        case 0x01109F00u: caseTinted(k); break;
        case 0x0110A100u: break;
        default:
            // A table word the generated code does not know (PS2_JUMP_GUEST):
            // the recompiled code from the start (nothing above it is seen by
            // a caller: the frame is below sp, f12 = f13 again).
            return fallback(kKick, rdram, ctx, runtime);
        }
        finish();
    }
}

SOTC_DECOMP_FUNCTION(0x0113DBF8u, manager_sub_0113DBF8, decomp_hot46j::drawSinCos);
SOTC_DECOMP_FUNCTION(0x0113E6C8u, manager_sub_0113E6C8, decomp_hot46j::drawAnimated);
SOTC_DECOMP_FUNCTION(0x0113DEA0u, manager_sub_0113DEA0, decomp_hot46j::drawSpan);
SOTC_DECOMP_FUNCTION(0x01108960u, manager_sub_01108960, decomp_hot46j::kick);
