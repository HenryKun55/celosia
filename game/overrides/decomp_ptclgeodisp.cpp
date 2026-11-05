// Particle geodisp light and display list passes:
// - calc_light_particle_geodisp (0x0113F600): resolves the particle geodisp
//   class (GetGeoDispClass, 0x010ED688) and its emitter list head (+0xE8),
//   runs ptclLightListPass (0x01140C80), and when any emitter needs lighting
//   copies the head node's position (+0x20, w=1.0f) into class +0x10, clears
//   bit 0 of class +0xC0, and tail-calls SetGeoDispClassLightColMan (0x010F64C0).
// - ptclLightListPass / manager_sub_01140C80 (0x01140C80): walks the emitter
//   linked list (+0x1B0 next) and returns 1 as soon as ptclEmitterNeedLight
//   (0x01140E60) returns 1 for any emitter, else 0.
// - ptclEmitterNeedLight / manager_sub_01140E60 (0x01140E60): checks whether
//   an emitter (+0x10 of node), its effect data (record +0x48), and its
//   particle type entry (+0xD4 of effect data) require dynamic lighting.
// - ptclEmitterSortCmp / entry_1140ea8 (0x01140EA8): qsort comparator
//   ordering emitter node pointers by camera depth (+0x144 of node, +0x134
//   of emitter).
// - disp_particle_geodisp (0x0113F6A8): resolves the particle geodisp class
//   and emitter list head (+0xE8) and tail-calls ptclDispEmitterList
//   (0x0113ACB8) with a0 = head, a1 = class + 0x80, a2 = class + 0x40.
// - ptclDispEmitterList / manager_sub_0113ACB8 (0x0113ACB8): selects and
//   pushes the display list layer (iosDlPushLayer, optionally resetting
//   surface 0x12 via modelResetSurface), groups consecutive emitters with
//   matching priority (+0x34 of record), sorts multi-emitter priority groups
//   with qsort (0x0116A310, comparator 0x01140EA8), dispatches each emitter
//   node to manager_sub_01140ED0, and pops the display list layer
//   (iosDlPopLayer).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ptclgeodisp
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    inline uint64_t sx(uint32_t v)
    {
        return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v)));
    }

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

    void ptclEmitterSortCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    // ptclEmitterNeedLight / manager_sub_01140E60 (0x01140E60)
    void ptclEmitterNeedLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(ctx->pc == 0x01140EA8u, 0))
        {
            ptclEmitterSortCmp(rdram, ctx, runtime);
            return;
        }
        (void)runtime;

        // 0x01140E60: lw $v0, 0x110($a0)
        const uint32_t a0 = GPR_U32(ctx, 4);
        uint64_t v0 = sx(READ32(a0 + 0x110u));
        SET_GPR_U64(ctx, 2, v0);
        if (v0 != 0u)
        {
            // 0x01140EA0: jr $ra; daddu $v0, $zero, $zero
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
            return;
        }

        // 0x01140E6C: lw $v0, 0x64($a1)
        const uint32_t a1 = GPR_U32(ctx, 5);
        v0 = sx(READ32(a1 + 0x64u));
        SET_GPR_U64(ctx, 2, v0);
        if (v0 != 0u)
        {
            // 0x01140E98: jr $ra; addiu $v0, $zero, 1
            SET_GPR_U64(ctx, 2, 1u);
            returnTo(ctx);
            return;
        }

        // 0x01140E78: lw $v1, 0xB0($a1); addiu $v0, $zero, 1
        uint64_t v1 = sx(READ32(a1 + 0xB0u));
        SET_GPR_U64(ctx, 3, v1);
        SET_GPR_U64(ctx, 2, 1u);
        if (v1 == 1u)
        {
            // 0x01140E98: jr $ra; addiu $v0, $zero, 1
            SET_GPR_U64(ctx, 2, 1u);
            returnTo(ctx);
            return;
        }

        // 0x01140E88: lw $v1, 0x164($a2); addiu $v0, $zero, 2
        const uint32_t a2 = GPR_U32(ctx, 6);
        v1 = sx(READ32(a2 + 0x164u));
        SET_GPR_U64(ctx, 3, v1);
        SET_GPR_U64(ctx, 2, 2u);
        if (v1 != 2u)
        {
            // 0x01140EA0: jr $ra; daddu $v0, $zero, $zero
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
            return;
        }

        // 0x01140E98: jr $ra; addiu $v0, $zero, 1
        SET_GPR_U64(ctx, 2, 1u);
        returnTo(ctx);
    }

    // ptclEmitterSortCmp / entry_1140ea8 (0x01140EA8)
    void ptclEmitterSortCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)runtime;

        // 0x01140EA8: lw $a2, 0x0($a0); lw $v1, 0x0($a1)
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint64_t a2 = sx(READ32(a0 + 0u));
        SET_GPR_U64(ctx, 6, a2);

        const uint32_t a1 = GPR_U32(ctx, 5);
        const uint64_t v1 = sx(READ32(a1 + 0u));
        SET_GPR_U64(ctx, 3, v1);

        // 0x01140EB0: lwc1 $f1, 0x144($a2); lwc1 $f0, 0x144($v1)
        const float rf_1 = fb(READ32(static_cast<uint32_t>(a2) + 0x144u));
        ctx->f[1] = rf_1;
        const float rf_0 = fb(READ32(static_cast<uint32_t>(v1) + 0x144u));
        ctx->f[0] = rf_0;

        // 0x01140EB8: c.lt.s $f1, $f0; bc1t +2 (delay: addiu $v0, $zero, 1); addiu $v0, $zero, -1
        const bool lt = FPU_C_OLT_S(rf_1, rf_0);
        ctx->fcr31 = lt ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        SET_GPR_U64(ctx, 2, lt ? 1u : sx(0xFFFFFFFFu));
        returnTo(ctx);
    }

    // ptclLightListPass / manager_sub_01140C80 (0x01140C80)
    void ptclLightListPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        uint64_t s0 = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 16, s0);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 16u, GPR_U64(ctx, 31));

        if (s0 == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            const uint32_t sp_cur = GPR_U32(ctx, 29);
            SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
            SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
            SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
            SET_GPR_U64(ctx, 29, sx(sp_cur + 0x20u));
            returnTo(ctx);
            return;
        }

        SET_GPR_U64(ctx, 17, 1u);
        uint64_t v0 = sx(READ32(static_cast<uint32_t>(s0) + 0u));
        SET_GPR_U64(ctx, 2, v0);

        for (;;)
        {
            s0 = GPR_U64(ctx, 16);
            v0 = GPR_U64(ctx, 2);
            const uint64_t a0 = sx(static_cast<uint32_t>(s0) + 0x10u);
            SET_GPR_U64(ctx, 4, a0);
            const uint64_t a1 = sx(READ32(static_cast<uint32_t>(v0) + 0x48u));
            SET_GPR_U64(ctx, 5, a1);
            const uint64_t a2 = sx(READ32(static_cast<uint32_t>(a1) + 0xD4u));
            SET_GPR_U64(ctx, 6, a2);

            callGuest(rdram, ctx, runtime, 0x01140E60u, 0x01140CB0u, GPR_U32(ctx, 29));

            if (GPR_U64(ctx, 2) == GPR_U64(ctx, 17))
            {
                const uint32_t sp_cur = GPR_U32(ctx, 29);
                SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
                SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
                SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
                SET_GPR_U64(ctx, 29, sx(sp_cur + 0x20u));
                returnTo(ctx);
                return;
            }

            s0 = sx(READ32(GPR_U32(ctx, 16) + 0x1B0u));
            SET_GPR_U64(ctx, 16, s0);
            if (s0 != 0u)
            {
                v0 = sx(READ32(static_cast<uint32_t>(s0) + 0u));
                SET_GPR_U64(ctx, 2, v0);
                continue;
            }

            SET_GPR_U64(ctx, 2, 0u);
            const uint32_t sp_cur = GPR_U32(ctx, 29);
            SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
            SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
            SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
            SET_GPR_U64(ctx, 29, sx(sp_cur + 0x20u));
            returnTo(ctx);
            return;
        }
    }

    // calc_light_particle_geodisp (0x0113F600)
    void calc_light_particle_geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 16u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4));

        // 0x0113F610: jal GetGeoDispClass (0x010ED688)
        callGuest(rdram, ctx, runtime, 0x010ED688u, 0x0113F618u, sp);

        // 0x0113F618: daddu $s1, $v0, $zero
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));

        // 0x0113F61C: beqz $s0, 0x0113F638; daddu $a0, $s0, $zero
        const uint64_t s0_in = GPR_U64(ctx, 16);
        SET_GPR_U64(ctx, 4, s0_in);
        if (s0_in == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
        }
        else
        {
            callGuest(rdram, ctx, runtime, 0x010ED688u, 0x0113F62Cu, sp);
            SET_GPR_U64(ctx, 2, sx(GPR_U32(ctx, 2) + 0xE8u));
        }

        // 0x0113F63C: lw $s0, 0x0($v0); jal 0x01140C80; daddu $a0, $s0, $zero
        const uint64_t s0_head = sx(READ32(GPR_U32(ctx, 2)));
        SET_GPR_U64(ctx, 16, s0_head);
        SET_GPR_U64(ctx, 4, s0_head);
        callGuest(rdram, ctx, runtime, 0x01140C80u, 0x0113F648u, sp);

        // 0x0113F648: addiu $v1, $zero, 1; bne $v0, $v1, 0x0113F690; daddu $a0, $s1, $zero
        SET_GPR_U64(ctx, 3, 1u);
        const uint64_t s1_cls = GPR_U64(ctx, 17);
        SET_GPR_U64(ctx, 4, s1_cls);
        if (GPR_U64(ctx, 2) == 1u)
        {
            const uint32_t s1_32 = static_cast<uint32_t>(s1_cls);
            uint64_t v1 = READ64(s1_32 + 0xC0u);
            const uint64_t mask = sx(0xFFFFFFFEu);
            SET_GPR_U64(ctx, 2, mask);
            const uint32_t s0_32 = GPR_U32(ctx, 16);
            const __m128i pos_vec = READ128(s0_32 + 0x20u);
            SET_GPR_VEC(ctx, 5, pos_vec);
            SET_GPR_U64(ctx, 1, sx(0x3F800000u));
            ctx->f[0] = 1.0f;
            v1 &= mask;
            SET_GPR_U64(ctx, 3, v1);
            WRITE128(s1_32 + 0x10u, GPR_VEC(ctx, 5));
            WRITE64(s1_32 + 0xC0u, v1);
            WRITE32(s1_32 + 0x1Cu, bf(ctx->f[0]));

            const uint32_t sp_cur = GPR_U32(ctx, 29);
            SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
            SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
            SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
            const uint32_t sp_out = sp_cur + 0x20u;
            SET_GPR_U64(ctx, 29, sx(sp_out));
            // 0x0113F684: j SetGeoDispClassLightColMan (0x010F64C0)
            callGuest(rdram, ctx, runtime, 0x010F64C0u, GPR_U32(ctx, 31), sp_out);
            return;
        }

        const uint32_t sp_cur = GPR_U32(ctx, 29);
        SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
        SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
        SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
        SET_GPR_U64(ctx, 29, sx(sp_cur + 0x20u));
        returnTo(ctx);
    }

    // disp_particle_geodisp (0x0113F6A8)
    void disp_particle_geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        const uint64_t s0_in = GPR_U64(ctx, 5);
        SET_GPR_U64(ctx, 16, s0_in);
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 16u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 4, s0_in);

        // 0x0113F6BC: jal GetGeoDispClass (0x010ED688)
        callGuest(rdram, ctx, runtime, 0x010ED688u, 0x0113F6C4u, sp);

        // 0x0113F6C4: daddu $s1, $v0, $zero; daddu $v0, $zero, $zero
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        SET_GPR_U64(ctx, 2, 0u);

        // 0x0113F6CC: beqz $s0, 0x0113F6E0; daddu $a0, $s0, $zero
        const uint64_t s0 = GPR_U64(ctx, 16);
        SET_GPR_U64(ctx, 4, s0);
        if (s0 != 0u)
        {
            callGuest(rdram, ctx, runtime, 0x010ED688u, 0x0113F6DCu, sp);
            SET_GPR_U64(ctx, 2, sx(GPR_U32(ctx, 2) + 0xE8u));
        }

        // 0x0113F6E0: addiu $a2, $s1, 0x40; addiu $a1, $s1, 0x80; lw $a0, 0x0($v0)
        const uint32_t s1_32 = GPR_U32(ctx, 17);
        SET_GPR_U64(ctx, 6, sx(s1_32 + 0x40u));
        SET_GPR_U64(ctx, 5, sx(s1_32 + 0x80u));
        SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 2))));

        const uint32_t sp_cur = GPR_U32(ctx, 29);
        SET_GPR_U64(ctx, 16, READ64(sp_cur + 0u));
        SET_GPR_U64(ctx, 17, READ64(sp_cur + 8u));
        SET_GPR_U64(ctx, 31, READ64(sp_cur + 16u));
        const uint32_t sp_out = sp_cur + 0x20u;
        SET_GPR_U64(ctx, 29, sx(sp_out));
        // 0x0113F6F8: j ptclDispEmitterList (0x0113ACB8)
        callGuest(rdram, ctx, runtime, 0x0113ACB8u, GPR_U32(ctx, 31), sp_out);
    }

    // ptclDispEmitterList / manager_sub_0113ACB8 (0x0113ACB8)
    void ptclDispEmitterList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x70u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x38u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 6));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 22));
        SET_GPR_U64(ctx, 22, GPR_U64(ctx, 5));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 30));
        SET_GPR_U64(ctx, 30, sx(sp));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 31));

        // 0x0113ACF0: beqz $s3, 0x0113AEC0
        if (GPR_U64(ctx, 19) != 0u)
        {
            const uint32_t s3_0 = GPR_U32(ctx, 19);
            uint32_t v1_32 = READ32(s3_0 + 0x128u);
            SET_GPR_U64(ctx, 3, sx(v1_32));
            SET_GPR_U64(ctx, 7, 1u);
            uint32_t a0_32 = READ32(s3_0 + 0u);
            SET_GPR_U64(ctx, 16, 0xBu);
            uint32_t v0_32 = (v1_32 << 3) - v1_32;
            const uint32_t a1_32 = READ32(a0_32 + 0x48u);
            SET_GPR_U64(ctx, 5, sx(a1_32));
            v0_32 = (v0_32 << 4) - v1_32;
            a0_32 = READ32(a1_32 + 0xD4u);
            const uint32_t a2_32 = READ32(a1_32 + 0xC0u);
            SET_GPR_U64(ctx, 6, sx(a2_32));
            v0_32 <<= 2;
            SET_GPR_U64(ctx, 2, sx(v0_32));
            a0_32 += v0_32;
            SET_GPR_U64(ctx, 4, sx(a0_32));

            bool earlyExit = false;
            if (GPR_U64(ctx, 6) == GPR_U64(ctx, 7))
            {
                SET_GPR_U64(ctx, 2, sx(0x01210000u));
                v1_32 = READ32(0x01216ED0u);
                SET_GPR_U64(ctx, 3, sx(v1_32));
                if (GPR_U64(ctx, 3) == 0u)
                {
                    earlyExit = true;
                }
            }

            if (!earlyExit)
            {
                SET_GPR_U64(ctx, 8, 2u);
                if (GPR_U64(ctx, 6) != GPR_U64(ctx, 8))
                {
                    v1_32 = READ32(a0_32 + 0xCu);
                    SET_GPR_U64(ctx, 3, sx(v1_32));
                }
                else
                {
                    SET_GPR_U64(ctx, 2, sx(0x01210000u));
                    v1_32 = READ32(0x01216ED0u);
                    SET_GPR_U64(ctx, 3, sx(v1_32));
                    if (GPR_U64(ctx, 3) == GPR_U64(ctx, 7))
                    {
                        earlyExit = true;
                    }
                    else
                    {
                        v1_32 = READ32(a0_32 + 0xCu);
                        SET_GPR_U64(ctx, 3, sx(v1_32));
                    }
                }
            }

            if (!earlyExit)
            {
                SET_GPR_U64(ctx, 2, 4u);
                const bool is4 = (GPR_U64(ctx, 3) == GPR_U64(ctx, 2));
                SET_GPR_U64(ctx, 3, sx(0x01210000u));
                if (is4)
                {
                    SET_GPR_U64(ctx, 16, 0x13u);
                    SET_GPR_U64(ctx, 2, 0x12u);
                }
                else
                {
                    v0_32 = READ32(0x01216ED0u);
                    SET_GPR_U64(ctx, 2, sx(v0_32));
                    a0_32 = READ32(a1_32 + 0xC4u);
                    SET_GPR_U64(ctx, 4, sx(a0_32));
                    if (GPR_U64(ctx, 2) == 0u)
                    {
                        SET_GPR_U64(ctx, 2, 0x12u);
                        if (GPR_U64(ctx, 4) == 0u)
                        {
                            SET_GPR_U64(ctx, 16, 0x12u);
                        }
                    }
                    else
                    {
                        if (GPR_U64(ctx, 4) == GPR_U64(ctx, 7))
                        {
                            SET_GPR_U64(ctx, 16, 0x12u);
                            SET_GPR_U64(ctx, 2, 0x12u);
                        }
                        else
                        {
                            SET_GPR_U64(ctx, 2, 0x12u);
                            if (GPR_U64(ctx, 4) != 0u)
                            {
                                if (GPR_U64(ctx, 4) == GPR_U64(ctx, 8))
                                {
                                    SET_GPR_U64(ctx, 16, 0x16u);
                                }
                                else
                                {
                                    SET_GPR_U64(ctx, 2, 0x12u);
                                }
                            }
                        }
                    }
                }

                // 0x0113ADB4: bne $s0, $v0, 0x0113ADC4
                if (GPR_U64(ctx, 16) == GPR_U64(ctx, 2))
                {
                    SET_GPR_U64(ctx, 4, 0x12u);
                    callGuest(rdram, ctx, runtime, 0x010993C8u, 0x0113ADC4u, GPR_U32(ctx, 29));
                }

                // 0x0113ADC4: jal iosDlPushLayer (dynamic JAL target at 0x0113ADC4)
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 16));
                callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113ADC4u), 0x0113ADCCu, GPR_U32(ctx, 29));

                // 0x0113ADCC: lui $s7, 0x114; daddu $a2, $s3, $zero
                SET_GPR_U64(ctx, 23, sx(0x01140000u));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 19));

                // Outer loop over priority groups (0x0113ADD8)
                for (;;)
                {
                    SET_GPR_U64(ctx, 18, 0u);
                    SET_GPR_U64(ctx, 19, sx(READ32(GPR_U32(ctx, 19) + 0x1B0u)));

                    // Inner loop counting consecutive emitters with equal priority (+0x34)
                    for (;;)
                    {
                        const bool s3_zero = (GPR_U64(ctx, 19) == 0u);
                        SET_GPR_U64(ctx, 18, sx(GPR_U32(ctx, 18) + 1u));
                        if (s3_zero)
                        {
                            break;
                        }
                        SET_GPR_U64(ctx, 3, sx(READ32(GPR_U32(ctx, 19) + 0u)));
                        SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 6) + 0u)));
                        SET_GPR_U64(ctx, 5, sx(READ32(GPR_U32(ctx, 3) + 0x34u)));
                        SET_GPR_U64(ctx, 2, sx(READ32(GPR_U32(ctx, 4) + 0x34u)));
                        if (GPR_U64(ctx, 5) == GPR_U64(ctx, 2))
                        {
                            SET_GPR_U64(ctx, 19, sx(READ32(GPR_U32(ctx, 19) + 0x1B0u)));
                            continue;
                        }
                        break;
                    }

                    // 0x0113AE04: slti $v0, $s2, 2
                    const bool lt2 = (static_cast<int64_t>(GPR_U64(ctx, 18)) < 2);
                    SET_GPR_U64(ctx, 2, lt2 ? 1u : 0u);
                    if (lt2)
                    {
                        // Single-emitter group: call manager_sub_01140ED0 directly
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 6));
                        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 21));
                        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 22));
                        callGuest(rdram, ctx, runtime, 0x01140ED0u, 0x0113AE20u, GPR_U32(ctx, 29));
                    }
                    else
                    {
                        // Multi-emitter group: allocate 16-byte aligned array on stack, qsort, and draw
                        uint32_t allocBytes = GPR_U32(ctx, 18) << 2;
                        SET_GPR_U64(ctx, 2, sx(allocBytes));
                        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 29));
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 6));
                        allocBytes = ((allocBytes + 0xFu) >> 4) << 4;
                        SET_GPR_U64(ctx, 2, sx(allocBytes));
                        const uint32_t sp_alloc = GPR_U32(ctx, 29) - allocBytes;
                        SET_GPR_U64(ctx, 29, sx(sp_alloc));
                        const bool countLeZero = (static_cast<int32_t>(GPR_U32(ctx, 18)) <= 0);
                        SET_GPR_U64(ctx, 17, sx(sp_alloc));

                        if (!countLeZero)
                        {
                            SET_GPR_U64(ctx, 2, sx(sp_alloc));
                            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 18));
                            for (;;)
                            {
                                WRITE32(GPR_U32(ctx, 2), GPR_U32(ctx, 4));
                                SET_GPR_U64(ctx, 2, sx(GPR_U32(ctx, 2) + 4u));
                                SET_GPR_U64(ctx, 16, sx(GPR_U32(ctx, 16) - 1u));
                                const bool more = (GPR_U64(ctx, 16) != 0u);
                                SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 4) + 0x1B0u)));
                                if (!more)
                                {
                                    break;
                                }
                            }
                        }

                        // 0x0113AE6C: jal qsort (0x0116A310) with comparator 0x01140EA8
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
                        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 18));
                        SET_GPR_U64(ctx, 6, 4u);
                        SET_GPR_U64(ctx, 7, sx(GPR_U32(ctx, 23) + 0xEA8u));
                        callGuest(rdram, ctx, runtime, 0x0116A310u, 0x0113AE80u, GPR_U32(ctx, 29));

                        if (static_cast<int32_t>(GPR_U32(ctx, 18)) <= 0)
                        {
                            SET_GPR_U64(ctx, 29, GPR_U64(ctx, 20));
                        }
                        else
                        {
                            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 18));
                            SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 17) + 0u)));
                            for (;;)
                            {
                                SET_GPR_U64(ctx, 17, sx(GPR_U32(ctx, 17) + 4u));
                                SET_GPR_U64(ctx, 16, sx(GPR_U32(ctx, 16) - 1u));
                                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 22));
                                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 21));
                                callGuest(rdram, ctx, runtime, 0x01140ED0u, 0x0113AEA4u, GPR_U32(ctx, 29));
                                if (GPR_U64(ctx, 16) != 0u)
                                {
                                    SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 17) + 0u)));
                                    continue;
                                }
                                break;
                            }
                            SET_GPR_U64(ctx, 29, GPR_U64(ctx, 20));
                        }
                    }

                    // 0x0113AEB0: bnel $s3, $zero, 0x0113ADD8; daddu $a2, $s3, $zero
                    if (GPR_U64(ctx, 19) != 0u)
                    {
                        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 19));
                        continue;
                    }
                    break;
                }

                // 0x0113AEB8: jal iosDlPopLayer (dynamic JAL target at 0x0113AEB8)
                callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113AEB8u), 0x0113AEC0u, GPR_U32(ctx, 29));
            }
        }

        // Epilogue (0x0113AEC0)
        const uint64_t fp = GPR_U64(ctx, 30);
        SET_GPR_U64(ctx, 29, fp);
        const uint32_t sp_end = static_cast<uint32_t>(fp);
        SET_GPR_U64(ctx, 16, READ64(sp_end + 0x20u));
        SET_GPR_U64(ctx, 17, READ64(sp_end + 0x28u));
        SET_GPR_U64(ctx, 18, READ64(sp_end + 0x30u));
        SET_GPR_U64(ctx, 19, READ64(sp_end + 0x38u));
        SET_GPR_U64(ctx, 20, READ64(sp_end + 0x40u));
        SET_GPR_U64(ctx, 21, READ64(sp_end + 0x48u));
        SET_GPR_U64(ctx, 22, READ64(sp_end + 0x50u));
        SET_GPR_U64(ctx, 23, READ64(sp_end + 0x58u));
        SET_GPR_U64(ctx, 30, READ64(sp_end + 0x60u));
        SET_GPR_U64(ctx, 31, READ64(sp_end + 0x68u));
        SET_GPR_U64(ctx, 29, sx(sp_end + 0x70u));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0113F600u, calc_light_particle_geodisp, decomp_ptclgeodisp::calc_light_particle_geodisp);
SOTC_DECOMP_FUNCTION(0x01140C80u, ptclLightListPass, decomp_ptclgeodisp::ptclLightListPass);
SOTC_DECOMP_FUNCTION(0x01140E60u, ptclEmitterNeedLight, decomp_ptclgeodisp::ptclEmitterNeedLight);
SOTC_DECOMP_FUNCTION(0x01140EA8u, ptclEmitterSortCmp, decomp_ptclgeodisp::ptclEmitterSortCmp);
SOTC_DECOMP_FUNCTION(0x0113F6A8u, disp_particle_geodisp, decomp_ptclgeodisp::disp_particle_geodisp);
SOTC_DECOMP_FUNCTION(0x0113ACB8u, ptclDispEmitterList, decomp_ptclgeodisp::ptclDispEmitterList);
