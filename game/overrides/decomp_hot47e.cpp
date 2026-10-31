// Round 47 (SOTC_HOTLIST47=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two particle functions,
// the manager's per-frame step and the emitter parameter blend.
//
// - doit_particle_manager (0x113a340): the particle manager's frame.
//   a0 / a1 / a2 kept in the manager's globals (0x0118CFE0 / E4 / EC), a3
//   the camera argument of manager_sub_0113A5A8 (the view matrices), f12 /
//   f13 the frame's two times. Three clocks (0x01216E08 / 18 / 28; +0 the
//   step, +4 step x step / 2, +8 the time left over, +0xc whole 30 fps
//   frames): the first steps by f12, the third by f13, the second by a fixed
//   1/30 (0x3D088888); each one's whole frames (cvt.w.s of time x 30) leave
//   the remainder (frames x 0x3D088887). Then the view (0113A5A8), every
//   particle geodisp of the group list at 0x01216E00 (isysGroupForFirst /
//   Next / Exit, the relocated jal sites read from the code words):
//   doit_particle_geodisp and auto_end_check_and_check_free_particle_geodisp
//   (with sp + 0 the geodisp word); 0x0118CFF4 cleared; and for each of the
//   five layers s4 = 4 .. 0 every geodisp's emitter list
//   (get_top_emitter_list_from_particle_geodisp): each emitter (+0x1b0 next)
//   whose record (+0) has +0x28 == the layer gets manager_sub_011408E0
//   (emitter + 0x10, the record's +0x48) into emitter + 0x1a8.
// - get_particle_parameter_blend_doit (0x1144f68): an emitter's parameters
//   (0x160 bytes into a0) from a1's parameter set (+0xd4 the records, 0x1bc
//   bytes each, a3 the index) blended with a2's (0: a straight copy).
//   With a2: manager_sub_01145C70 (a1, a2, the record) the other record,
//   manager_sub_01145CD0 (sp + 0, sp + 4, f12) the two weights; then every
//   field w0 x a + w1 x b (+8 an integer, the blend through floats and
//   cvt.w.s); three fields (+0x40 / +0x6c / +0xec) only when both sides are
//   above 0 (else the first side's value). The fields' source offsets are
//   one table (kSrc), the destination +0xc + 4 i.
//
// The recompiled code's statements in its order: the FPU operations (FPU_*
// macros; f0 - f7 / f20 and the condition bit as the code leaves them), all
// the stores (the frame's saves, sp + 0 the geodisp word); the saved
// registers come back from the frame as the original reloads them; the
// s-registers hold the code's values at every guest call (round 47).
// Guest calls through the function table (callGuest) with the original's
// ra and frame sp.
// Not as the original: the scratch GPRs (only the calls' arguments and v0 /
// v1 where the code leaves them before a call; at / t0 - t3 not), and the
// loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47e
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;

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
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- doit_particle_manager --------------------------------------------
    constexpr uint32_t kManager = 0x0113A340u;
    constexpr uint32_t kView = 0x0113A5A8u;        // manager_sub_0113A5A8
    constexpr uint32_t kGeodisp = 0x0113F500u;     // doit_particle_geodisp
    constexpr uint32_t kAutoEnd = 0x0113F4A0u;     // auto_end_check_and_check_free_particle_geodisp
    constexpr uint32_t kTopEmitters = 0x0113F378u; // get_top_emitter_list_from_particle_geodisp
    constexpr uint32_t kEmitterStep = 0x011408E0u; // manager_sub_011408E0
    // The isysGroupFor* jal sites (relocated: the code word read as the
    // generated code does).
    constexpr uint32_t kSiteFirst1 = 0x0113A484u, kSiteNext1 = 0x0113A4B8u, kSiteExit1 = 0x0113A4D4u;
    constexpr uint32_t kSiteFirst2 = 0x0113A4F0u, kSiteNext2 = 0x0113A550u, kSiteExit2 = 0x0113A56Cu;
    constexpr uint32_t kGroups = 0x01216E00u; // s5 + 0x6e00: the geodisp group list
    constexpr uint32_t kClockA = 0x01216E08u, kClockB = 0x01216E18u, kClockC = 0x01216E28u;
    constexpr uint32_t kArg0 = 0x0118CFE0u, kArg1 = 0x0118CFE4u, kArg2 = 0x0118CFECu, kCleared = 0x0118CFF4u;

    void manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kManager, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x40u, ra);
        SET_GPR_U64(ctx, 17, 0u);
        SET_GPR_S32(ctx, 21, 0x01210000u);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };

        // The three clocks.
        f[10] = fb(0x3D088888u); // 1/30
        f[5] = fb(READ32(kClockA + 8u));
        f[7] = fb(READ32(kClockB + 8u));
        f[4] = fb(READ32(kClockC + 8u));
        f[5] = FPU_ADD_S(f[5], f[12]);
        f[7] = FPU_ADD_S(f[7], f[10]);
        f[0] = fb(0x41F00000u); // 30
        f[4] = FPU_ADD_S(f[4], f[13]);
        WRITE32(kArg0, GPR_U32(ctx, 4));
        WRITE32(kArg1, GPR_U32(ctx, 5));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 7));
        f[1] = FPU_MUL_S(f[5], f[0]);
        f[8] = fb(0x3D088887u);
        f[3] = fb(0x3F000000u); // 0.5
        f[2] = FPU_MUL_S(f[4], f[0]);
        f[9] = fb(0x3A11A2B2u);
        f[0] = FPU_MUL_S(f[7], f[0]);
        const uint32_t framesA = static_cast<uint32_t>(FPU_CVT_W_S(f[1])); // t1
        f[6] = fb(framesA);
        f[6] = FPU_MUL_S(f[13], f[3]);
        WRITE32(kArg2, GPR_U32(ctx, 6));
        const uint32_t framesC = static_cast<uint32_t>(FPU_CVT_W_S(f[2])); // t3
        f[1] = fb(framesC);
        WRITE32(kClockB + 4u, bf(f[9]));
        const uint32_t framesB = static_cast<uint32_t>(FPU_CVT_W_S(f[0])); // a1
        f[1] = fb(framesB);
        SET_GPR_U32(ctx, 5, framesB);
        WRITE32(kClockA + 0u, bf(f[12]));
        f[2] = FPU_CVT_S_W(static_cast<int32_t>(framesA));
        WRITE32(kClockA + 0xCu, framesA);
        f[3] = FPU_MUL_S(f[12], f[3]);
        WRITE32(kClockB + 0u, bf(f[10]));
        f[1] = FPU_CVT_S_W(static_cast<int32_t>(framesC));
        f[0] = FPU_CVT_S_W(static_cast<int32_t>(framesB));
        WRITE32(kClockB + 0xCu, framesB);
        f[2] = FPU_MUL_S(f[2], f[8]);
        WRITE32(kClockC + 0u, bf(f[13]));
        f[3] = FPU_MUL_S(f[3], f[12]);
        WRITE32(kClockC + 0xCu, framesC);
        f[1] = FPU_MUL_S(f[1], f[8]);
        f[0] = FPU_MUL_S(f[0], f[8]);
        f[5] = FPU_SUB_S(f[5], f[2]);
        f[6] = FPU_MUL_S(f[6], f[13]);
        WRITE32(kClockA + 4u, bf(f[3]));
        f[4] = FPU_SUB_S(f[4], f[1]);
        f[7] = FPU_SUB_S(f[7], f[0]);
        WRITE32(kClockA + 8u, bf(f[5]));
        WRITE32(kClockC + 4u, bf(f[6]));
        WRITE32(kClockC + 8u, bf(f[4]));
        WRITE32(kClockB + 8u, bf(f[7]));
        // v0 / v1 / a3 as the code leaves them (the callee reads only a0).
        SET_GPR_S32(ctx, 2, kClockA);
        SET_GPR_S32(ctx, 3, kClockC);
        SET_GPR_S32(ctx, 7, 0x01190000u);
        call(kView, 0x0113A480u);

        // Every geodisp: its step and the auto-end check.
        uint64_t s1 = 0u;
        SET_GPR_S32(ctx, 4, kGroups);
        SET_GPR_U64(ctx, 5, 0u);
        call(jalTarget(rdram, kSiteFirst1), 0x0113A48Cu);
        uint64_t s0 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 16, s0);
        if (s0 != 0u)
        {
            s1 = sx(READ32(static_cast<uint32_t>(s0)));
            for (;;)
            {
                SET_GPR_U64(ctx, 17, s1);
                SET_GPR_U64(ctx, 4, s1);
                WRITE32(sp + 0u, static_cast<uint32_t>(s1));
                call(kGeodisp, 0x0113A4ACu);
                SET_GPR_S32(ctx, 4, sp);
                call(kAutoEnd, 0x0113A4B4u);
                SET_GPR_U64(ctx, 4, s0);
                SET_GPR_U64(ctx, 5, 0u);
                call(jalTarget(rdram, kSiteNext1), 0x0113A4C0u);
                s0 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 16, s0);
                if (s0 == 0u)
                    break;
                s1 = sx(READ32(static_cast<uint32_t>(s0)));
            }
        }
        SET_GPR_U64(ctx, 17, s1);
        SET_GPR_U64(ctx, 4, s1);
        if (s1 != 0u)
        {
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, kSiteExit1), 0x0113A4DCu);
        }

        // The five layers (s4 = 4 .. 0): each emitter of the layer stepped.
        SET_GPR_S32(ctx, 2, 0x01190000u);
        WRITE32(kCleared, 0u);
        for (int32_t s4 = 4; s4 >= 0; --s4)
        {
            SET_GPR_S32(ctx, 20, static_cast<uint32_t>(s4));
            SET_GPR_S32(ctx, 4, kGroups);
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, kSiteFirst2), 0x0113A4F8u);
            uint64_t s2 = GPR_U64(ctx, 2), s3 = 0u;
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, 0u);
            if (s2 != 0u)
            {
                s3 = sx(READ32(static_cast<uint32_t>(s2)));
                for (;;)
                {
                    SET_GPR_U64(ctx, 19, s3);
                    SET_GPR_U64(ctx, 4, s3);
                    call(kTopEmitters, 0x0113A510u);
                    s1 = sx(READ32(GPR_U32(ctx, 2)));
                    SET_GPR_U64(ctx, 17, s1);
                    while (s1 != 0u)
                    {
                        const uint32_t emitter = static_cast<uint32_t>(s1);
                        const uint32_t record = READ32(emitter);
                        SET_GPR_S32(ctx, 3, record);
                        const uint64_t layer = sx(READ32(record + 0x28u));
                        SET_GPR_U64(ctx, 2, layer);
                        if (layer == sx(static_cast<uint32_t>(s4)))
                        {
                            const uint32_t s0e = emitter + 0x10u;
                            SET_GPR_S32(ctx, 16, s0e);
                            SET_GPR_S32(ctx, 5, READ32(record + 0x48u));
                            SET_GPR_S32(ctx, 4, s0e);
                            call(kEmitterStep, 0x0113A53Cu);
                            WRITE32(s0e + 0x198u, GPR_U32(ctx, 2));
                        }
                        s1 = sx(READ32(emitter + 0x1B0u));
                        SET_GPR_U64(ctx, 17, s1);
                    }
                    SET_GPR_U64(ctx, 4, s2);
                    SET_GPR_U64(ctx, 5, 0u);
                    call(jalTarget(rdram, kSiteNext2), 0x0113A558u);
                    s2 = GPR_U64(ctx, 2);
                    SET_GPR_U64(ctx, 18, s2);
                    if (s2 == 0u)
                        break;
                    s3 = sx(READ32(static_cast<uint32_t>(s2)));
                }
            }
            SET_GPR_U64(ctx, 4, s3);
            if (s3 != 0u)
            {
                SET_GPR_U64(ctx, 5, 0u);
                call(jalTarget(rdram, kSiteExit2), 0x0113A574u);
            }
        }
        SET_GPR_S32(ctx, 20, 0xFFFFFFFFu);
        SET_GPR_S32(ctx, 4, kGroups);

        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- get_particle_parameter_blend_doit ----------------------------------
    constexpr uint32_t kBlend = 0x01144F68u;
    constexpr uint32_t kOtherRecord = 0x01145C70u; // manager_sub_01145C70
    constexpr uint32_t kWeights = 0x01145CD0u;     // manager_sub_01145CD0

    // The record's source offset of each output field (+0xc + 4 i).
    constexpr uint16_t kSrc[85] = {
        0x018, 0x01C, 0x020, 0x024, 0x028, 0x02C, 0x030, 0x034, 0x038, 0x03C, 0x040, // 0..10
        0x044, 0x048, 0x190, 0x194,                                                   // 11..14
        0x198, 0x04C, 0x050, 0x054, 0x058, 0x05C, 0x060,                              // 15..21
        0x064, 0x068, 0x19C, 0x1A0,                                                   // 22..25
        0x1A4, 0x06C, 0x070, 0x074, 0x078, 0x07C, 0x080, 0x084, 0x088, 0x08C, 0x090,  // 26..36
        0x094, 0x098, 0x09C, 0x0A0, 0x0A4, 0x0A8, 0x0AC, 0x0B0, 0x0BC, 0x0C0, 0x0C4,  // 37..47
        0x0CC, 0x0D0, 0x0D4, 0x0D8, 0x0DC, 0x0E0,                                     // 48..53
        0x0E4, 0x0E8, 0x1A8, 0x1AC,                                                   // 54..57
        0x1B0, 0x0EC, 0x0F0, 0x0F4, 0x0F8, 0x0FC, 0x100, 0x114, 0x118, 0x120, 0x128,  // 58..68
        0x12C, 0x130, 0x134, 0x138, 0x144, 0x148, 0x14C, 0x150, 0x154, 0x158, 0x168,  // 69..79
        0x178, 0x17C, 0x184, 0x188,                                                   // 80..83
        0x1B8,                                                                        // 84
    };
    constexpr uint32_t dstOf(int i) { return 0xCu + 4u * static_cast<uint32_t>(i); }

    void blend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kBlend, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        const uint64_t a0 = GPR_U64(ctx, 4), set = GPR_U64(ctx, 5), other = GPR_U64(ctx, 6), index = GPR_U64(ctx, 7);
        const uint32_t out = static_cast<uint32_t>(a0), s3 = static_cast<uint32_t>(set), s4 = static_cast<uint32_t>(other);
        // v1 = index x 0x1bc (sll 3, subu, sll 4, subu, sll 2: 32-bit steps).
        const uint32_t i32 = static_cast<uint32_t>(index);
        uint32_t v1 = i32 << 3;
        v1 = v1 - i32;
        v1 = v1 << 4;
        v1 = v1 - i32;
        v1 = v1 << 2;
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE32(sp + 0x40u, bf(ctx->f[20]));
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x38u, ra);
        const uint32_t records = READ32(s3 + 0xD4u);
        const uint32_t s1 = records + v1; // the record
        SET_GPR_U64(ctx, 2, index);
        SET_GPR_S32(ctx, 3, v1);
        SET_GPR_U64(ctx, 16, a0);
        SET_GPR_S32(ctx, 17, s1);
        SET_GPR_U64(ctx, 19, set);
        SET_GPR_U64(ctx, 20, other);

        if (s4 == 0u)
        {
            // No other set: the record copied (lwc1 / swc1 through f0 / f1).
            SET_GPR_S32(ctx, 5, records);
            float *f = ctx->f;
            f[0] = fb(READ32(s3 + 0x78u));
            WRITE32(out + 0u, bf(f[0]));
            f[1] = fb(READ32(s3 + 0xACu));
            WRITE32(out + 4u, bf(f[1]));
            const uint32_t count = READ32(s1 + 4u);
            SET_GPR_S32(ctx, 2, count);
            WRITE32(out + 8u, count);
            for (int i = 0; i < 85; ++i)
            {
                const uint32_t bits = READ32(s1 + kSrc[i]);
                WRITE32(out + dstOf(i), bits);
                f[(i & 1) ? 1 : 0] = fb(bits);
            }
        }
        else
        {
            SET_GPR_U64(ctx, 5, other);
            SET_GPR_S32(ctx, 6, s1);
            SET_GPR_U64(ctx, 4, set);
            callGuest(rdram, ctx, runtime, kOtherRecord, 0x01144FCCu, sp);
            ctx->f[12] = FPU_MOV_S(ctx->f[20]);
            SET_GPR_S32(ctx, 4, sp);
            SET_GPR_S32(ctx, 5, sp + 4u);
            const uint32_t s2 = GPR_U32(ctx, 2); // the other record
            SET_GPR_U64(ctx, 18, GPR_U64(ctx, 2));
            callGuest(rdram, ctx, runtime, kWeights, 0x01144FE0u, sp);

            float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7];
            auto ld = [&](uint32_t a) __attribute__((always_inline)) { return fb(READ32(a)); };
            auto st = [&](uint32_t a, float v) __attribute__((always_inline)) { WRITE32(a, bf(v)); };
            // A run of fields w0 x a + w1 x b through f1 / f2 alternately.
            auto run = [&](int from, int to) __attribute__((always_inline))
            {
#pragma GCC unroll 64
                for (int i = from; i <= to; ++i)
                {
                    const uint32_t src = kSrc[i];
                    if (((i - from) & 1) == 0)
                    {
                        f1 = ld(s1 + src);
                        f0 = ld(s2 + src);
                        f1 = FPU_MUL_S(f3, f1);
                        f0 = FPU_MUL_S(f4, f0);
                        f1 = FPU_ADD_S(f1, f0);
                        st(out + dstOf(i), f1);
                    }
                    else
                    {
                        f2 = ld(s1 + src);
                        f0 = ld(s2 + src);
                        f2 = FPU_MUL_S(f3, f2);
                        f0 = FPU_MUL_S(f4, f0);
                        f2 = FPU_ADD_S(f2, f0);
                        st(out + dstOf(i), f2);
                    }
                }
            };
            // A run's last field: the weights' registers used up (f3 / f4).
            auto last = [&](int i) __attribute__((always_inline))
            {
                f0 = ld(s1 + kSrc[i]);
                f1 = ld(s2 + kSrc[i]);
                f3 = FPU_MUL_S(f3, f0);
                f4 = FPU_MUL_S(f4, f1);
                f3 = FPU_ADD_S(f3, f4);
                st(out + dstOf(i), f3);
            };
            // The next one with the weights' copies (f5 / f6).
            auto copyWeights = [&](int i) __attribute__((always_inline))
            {
                f1 = ld(s1 + kSrc[i]);
                f0 = ld(s2 + kSrc[i]);
                f1 = FPU_MUL_S(f5, f1);
                f0 = FPU_MUL_S(f6, f0);
                f1 = FPU_ADD_S(f1, f0);
                st(out + dstOf(i), f1);
            };
            // A field blended only when both sides are above 0.
            auto positive = [&](int i) __attribute__((always_inline))
            {
                const uint32_t dst = out + dstOf(i);
                f0 = ld(s1 + kSrc[i]);
                bool c = FPU_C_OLT_S(f7, f0);
                ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (!c)
                    return st(dst, f0);
                f1 = ld(s2 + kSrc[i]);
                c = FPU_C_OLT_S(f7, f1);
                ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (!c)
                    return st(dst, f0);
                f0 = FPU_MUL_S(f5, f0);
                f1 = FPU_MUL_S(f6, f1);
                f0 = FPU_ADD_S(f0, f1);
                st(dst, f0);
            };
            // The field after it, the weights reloaded from the frame (with
            // their copies and f7 = 0 again when reset).
            auto reload = [&](int i, bool reset) __attribute__((always_inline))
            {
                f0 = ld(s1 + kSrc[i]);
                f1 = ld(s2 + kSrc[i]);
                f0 = FPU_MUL_S(f5, f0);
                f3 = ld(sp + 0u);
                f1 = FPU_MUL_S(f6, f1);
                f4 = ld(sp + 4u);
                if (reset)
                {
                    f5 = FPU_MOV_S(f3);
                    f7 = fb(0u);
                    f6 = FPU_MOV_S(f4);
                }
                f0 = FPU_ADD_S(f0, f1);
                st(out + dstOf(i), f0);
            };

            f3 = ld(sp + 0u);
            f4 = ld(sp + 4u);
            // +0 / +4 from the sets themselves (+0x78 / +0xac).
            f2 = ld(s3 + 0x78u);
            f5 = FPU_MOV_S(f3);
            f0 = ld(s4 + 0x78u);
            f6 = FPU_MOV_S(f4);
            f2 = FPU_MUL_S(f3, f2);
            f0 = FPU_MUL_S(f4, f0);
            f7 = fb(0u);
            f2 = FPU_ADD_S(f2, f0);
            st(out + 0u, f2);
            f1 = ld(s3 + 0xACu);
            f0 = ld(s4 + 0xACu);
            f1 = FPU_MUL_S(f3, f1);
            f0 = FPU_MUL_S(f4, f0);
            f1 = FPU_ADD_S(f1, f0);
            st(out + 4u, f1);
            // +8: the record's +4, an integer.
            f2 = FPU_CVT_S_W(static_cast<int32_t>(READ32(s1 + 4u)));
            f0 = FPU_CVT_S_W(static_cast<int32_t>(READ32(s2 + 4u)));
            f2 = FPU_MUL_S(f3, f2);
            f0 = FPU_MUL_S(f4, f0);
            f2 = FPU_ADD_S(f2, f0);
            const uint32_t count = static_cast<uint32_t>(FPU_CVT_W_S(f2));
            f0 = fb(count);
            WRITE32(out + 8u, count);

            run(0, 10);
            last(11);
            copyWeights(12);
            positive(13);
            reload(14, true);
            run(15, 21);
            last(22);
            copyWeights(23);
            positive(24);
            reload(25, true);
            run(26, 53);
            last(54);
            copyWeights(55);
            positive(56);
            reload(57, false);
            run(58, 83);
            last(84);

            float *f = ctx->f;
            f[0] = f0;
            f[1] = f1;
            f[2] = f2;
            f[3] = f3;
            f[4] = f4;
            f[5] = f5;
            f[6] = f6;
            f[7] = f7;
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        ctx->f[20] = fb(READ32(sp + 0x40u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }
}

SOTC_DECOMP_FUNCTION(0x0113A340u, doit_particle_manager, decomp_hot47e::manager);
SOTC_DECOMP_FUNCTION(0x01144F68u, get_particle_parameter_blend_doit, decomp_hot47e::blend);
