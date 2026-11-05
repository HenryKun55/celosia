// Native decompilation of particle emitter display and world matrix functions:
// - ptclDispEmitterNode (manager_sub_01140ED0, 0x01140ED0)
// - ptclEmitterLightDir (manager_sub_0113B408, 0x0113B408)
// - disp_particle_emitter (0x0113D988)
// - get_world_matrixSR (0x01140418)
// - ptclEmitterMatrixMode (manager_sub_01141E80, 0x01141E80)
// - ptclWorldMatrixSR (manager_sub_01141EA8, 0x01141EA8)
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ptclemitter
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kPtclDispEmitterNode = 0x01140ED0u;
    constexpr uint32_t kPtclEmitterLightDir = 0x0113B408u;
    constexpr uint32_t kDispParticleEmitter = 0x0113D988u;
    constexpr uint32_t kGetWorldMatrixSR = 0x01140418u;
    constexpr uint32_t kPtclEmitterMatrixMode = 0x01141E80u;
    constexpr uint32_t kPtclWorldMatrixSR = 0x01141EA8u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    // ptclDispEmitterNode (manager_sub_01140ED0, 0x01140ED0)
    void ptclDispEmitterNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint64_t rc_2;
        uint64_t rc_3;
        uint64_t rc_4;
        uint64_t rc_5;
        uint64_t rc_6;
        uint64_t rc_7;
        uint64_t rc_8;
        uint64_t rc_16;
        uint64_t rc_17;
        uint64_t rc_29;
        uint64_t rc_31;

        // 0x1140ed0: addiu $sp, $sp, -0x20
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294967264u));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x1140ed4: daddu $a3, $a1, $zero
        rc_5 = Ps2GprU64(ctx, 5);
        RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_5) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 7, rc_7);
        // 0x1140ed8: sd $s0, 0x0($sp)
        rc_16 = Ps2GprU64(ctx, 16);
        WRITE64(ADD32(RC_U32(rc_29), 0), RC_U64(rc_16));
        // 0x1140edc: daddu $t0, $a2, $zero
        rc_6 = Ps2GprU64(ctx, 6);
        RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_6) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 8, rc_8);
        // 0x1140ee0: sd $s1, 0x8($sp)
        rc_17 = Ps2GprU64(ctx, 17);
        WRITE64(ADD32(RC_U32(rc_29), 8), RC_U64(rc_17));
        // 0x1140ee4: daddu $s1, $a0, $zero
        rc_4 = Ps2GprU64(ctx, 4);
        RC_SET_U64(rc_17, (uint64_t)RC_U64(rc_4) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 17, rc_17);
        // 0x1140ee8: sd $ra, 0x10($sp)
        rc_31 = Ps2GprU64(ctx, 31);
        WRITE64(ADD32(RC_U32(rc_29), 16), RC_U64(rc_31));
        // 0x1140eec: addiu $s0, $s1, 0x10
        RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(rc_17), 16));
        Ps2SetGpr64(ctx, 16, RC_U64X(rc_16));
        // 0x1140ef0: daddu $a0, $s0, $zero
        RC_SET_U64(rc_4, (uint64_t)RC_U64X(rc_16) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4));
        // 0x1140ef4: lw $v0, 0x0($s1)
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 0)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x1140ef8: lw $a1, 0x48($v0)
        RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_2), 72)));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x1140f00: lw $a2, 0xD4($a1) (Delay Slot)
        RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_5), 212)));
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6));
        // 0x1140efc: jal 0x0113B408 (ptclEmitterLightDir)
        callGuest(rdram, ctx, runtime, kPtclEmitterLightDir, 0x01140F04u, RC_U32(rc_29));

        // 0x1140f04: lw $v1, 0x164($s0)
        rc_16 = Ps2GprU64(ctx, 16);
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_16), 356)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x1140f08: addiu $v0, $zero, 0x1
        RC_SET_S32(rc_2, 1);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x1140f0c: bne $v1, $v0, label_1140f38
        // 0x1140f10: daddu $a0, $s0, $zero (Delay Slot)
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        if (RC_U64X(rc_3) != RC_U64X(rc_2))
        {
            rc_29 = Ps2GprU64(ctx, 29);
            RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 0)));
            Ps2SetGpr64(ctx, 16, rc_16);
            RC_SET_U64(rc_17, READ64(ADD32(RC_U32(rc_29), 8)));
            Ps2SetGpr64(ctx, 17, rc_17);
            RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 16)));
            Ps2SetGpr64(ctx, 31, rc_31);
            RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 32));
            Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
            returnTo(ctx);
            return;
        }

        // 0x1140f14: lw $v0, 0x0($s1)
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_17), 0)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x1140f18: ld $s0, 0x0($sp)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 0)));
        Ps2SetGpr64(ctx, 16, rc_16);
        // 0x1140f1c: lw $a2, 0x4C($v0)
        RC_SET_S32(rc_6, (int32_t)READ32(ADD32(RC_U32(rc_2), 76)));
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6));
        // 0x1140f20: lw $a1, 0x48($v0)
        RC_SET_S32(rc_5, (int32_t)READ32(ADD32(RC_U32(rc_2), 72)));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x1140f24: ld $s1, 0x8($sp)
        RC_SET_U64(rc_17, READ64(ADD32(RC_U32(rc_29), 8)));
        Ps2SetGpr64(ctx, 17, rc_17);
        // 0x1140f28: ld $ra, 0x10($sp)
        RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 16)));
        Ps2SetGpr64(ctx, 31, rc_31);
        // 0x1140f30: addiu $sp, $sp, 0x20 (Delay Slot)
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 32));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x1140f2c: j 0x0113D988 (disp_particle_emitter)
        callGuest(rdram, ctx, runtime, kDispParticleEmitter, RC_U32(rc_31), RC_U32(rc_29));
    }

    // ptclEmitterLightDir (manager_sub_0113B408, 0x0113B408)
    void ptclEmitterLightDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)runtime;
        uint64_t rc_1;
        uint64_t rc_2;
        uint64_t rc_3;
        uint64_t rc_4;
        uint64_t rc_5;
        uint64_t rc_6;
        uint64_t rc_7;
        uint64_t rc_8;
        uint64_t rc_9;
        uint64_t rc_10;
        uint64_t rc_11;
        uint64_t rc_12;
        uint64_t rc_29;
        float rf_0;
        float rf_1;
        float rf_2;

        // 0x113b408: daddu $t1, $a0, $zero
        rc_4 = Ps2GprU64(ctx, 4);
        RC_SET_U64(rc_9, (uint64_t)RC_U64(rc_4) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 9, rc_9);
        // 0x113b40c: addiu $sp, $sp, -0x10
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294967280u));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x113b410: lw $v0, 0x110($t1)
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_9), 272)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113b414: daddu $t3, $a1, $zero
        rc_5 = Ps2GprU64(ctx, 5);
        RC_SET_U64(rc_11, (uint64_t)RC_U64(rc_5) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 11, rc_11);
        // 0x113b418: bnez $v0, label_113b604
        {
            const bool branch_taken_0x113b418 = (RC_U64X(rc_2) != RC_U64(0));
            // 0x113b41c: daddu $t4, $t0, $zero (Delay Slot)
            rc_8 = Ps2GprU64(ctx, 8);
            RC_SET_U64(rc_12, (uint64_t)RC_U64(rc_8) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 12, rc_12);
            if (branch_taken_0x113b418)
            {
                goto label_113b604;
            }
        }
        // 0x113b420: lui $v0, 0x119
        RC_SET_S32(rc_2, (int32_t)((uint32_t)281 << 16));
        // 0x113b424: lw $t0, 0x64($t3)
        RC_SET_S32(rc_8, (int32_t)READ32(ADD32(RC_U32(rc_11), 100)));
        Ps2SetGpr64(ctx, 8, RC_U64X(rc_8));
        // 0x113b428: addiu $v0, $v0, 0x58B0
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_2), 22704));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113b42c: addiu $a1, $zero, 0x1
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x113b430: ld $a0, 0x8($v0)
        RC_SET_U64(rc_4, READ64(ADD32(RC_U32(rc_2), 8)));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113b434: daddu $t2, $zero, $zero
        RC_SET_U64(rc_10, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 10, RC_U64X(rc_10));
        // 0x113b438: ld $v1, 0x0($v0)
        RC_SET_U64(rc_3, READ64(ADD32(RC_U32(rc_2), 0)));
        Ps2SetGpr64(ctx, 3, rc_3);
        // 0x113b43c: sd $a0, 0x8($sp)
        WRITE64(ADD32(RC_U32(rc_29), 8), RC_U64(rc_4));
        // 0x113b440: beq $t0, $a1, label_113b4c0
        {
            const bool branch_taken_0x113b440 = (RC_U64X(rc_8) == RC_U64X(rc_5));
            // 0x113b444: sd $v1, 0x0($sp) (Delay Slot)
            WRITE64(ADD32(RC_U32(rc_29), 0), RC_U64(rc_3));
            if (branch_taken_0x113b440)
            {
                goto label_113b4c0;
            }
        }
        // 0x113b448: beqz $t0, label_113b4d0
        // 0x113b44c: lui $v0, 0x1E (Delay Slot, relocated with 0x113b4d0)
        if (RC_U64X(rc_8) == RC_U64(0))
        {
            RC_SET_S32(rc_2, (int32_t)relocated(rdram, 0x0113B44Cu, 0x0113B4D0u));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            goto label_113b4d0;
        }
        // 0x113b450: sltiu $v0, $t0, 0x6
        RC_SET_U64(rc_2, ((uint64_t)RC_U64X(rc_8) < (uint64_t)(int64_t)(int32_t)6) ? 1 : 0);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113b454: beqz $v0, label_113b4d0
        // 0x113b458: lui $v0, 0x1E (Delay Slot, relocated with 0x113b4d0)
        if (RC_U64X(rc_2) == RC_U64(0))
        {
            RC_SET_S32(rc_2, (int32_t)relocated(rdram, 0x0113B458u, 0x0113B4D0u));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            goto label_113b4d0;
        }
        // 0x113b45c: addiu $v0, $a3, 0x10
        rc_7 = Ps2GprU64(ctx, 7);
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_7), 16));
        // 0x113b460: lqc2 $vf5, 0x0($a3)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_7), 0)));
        // 0x113b464: lqc2 $vf6, 0x0($v0)
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b468: vadd.xyz $vf5, $vf5, $vf6
        {
            __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b46c: sqc2 $vf5, 0x0($sp)
        WRITE128(ADD32(RC_U32(rc_29), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b470: addiu $v1, $a3, 0x20
        RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_7), 32));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113b474: lqc2 $vf5, 0x0($sp)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        // 0x113b478: lqc2 $vf6, 0x0($v1)
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_3), 0)));
        // 0x113b47c: vadd.xyz $vf5, $vf5, $vf6
        {
            __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b480: sqc2 $vf5, 0x0($sp)
        WRITE128(ADD32(RC_U32(rc_29), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b484: lui $at, 0x3EAA
        RC_SET_S32(rc_1, (int32_t)((uint32_t)16042 << 16));
        // 0x113b488: ori $at, $at, 0xAAAA
        RC_SET_U64(rc_1, RC_U64X(rc_1) | (uint64_t)(uint16_t)43690);
        Ps2SetGpr64(ctx, 1, RC_U64X(rc_1));
        // 0x113b48c: mtc1 $at, $f0
        {
            uint32_t bits = RC_U32(rc_1);
            std::memcpy(&rf_0, &bits, sizeof(bits));
        }
        ctx->f[0] = rf_0;
        // 0x113b490: mfc1 $v0, $f0
        {
            uint32_t bits;
            std::memcpy(&bits, &rf_0, sizeof(bits));
            RC_SET_U32(rc_2, bits);
        }
        // 0x113b494: lqc2 $vf5, 0x0($sp)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        // 0x113b498: qmtc2.ni $v0, $vf6
        ctx->vu0_vf[6] = _mm_castsi128_ps(RC_VECX(ctx, 2, rc_2));
        // 0x113b49c: vmulx.xyz $vf5, $vf5, $vf6x
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b4a0: sqc2 $vf5, 0x0($sp)
        WRITE128(ADD32(RC_U32(rc_29), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b4a4: addiu $v0, $a3, 0x30
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_7), 48));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113b4a8: lqc2 $vf5, 0x0($sp)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        // 0x113b4ac: lqc2 $vf6, 0x0($v0)
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b4b0: vadd.xyz $vf5, $vf5, $vf6
        {
            __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b4b4: sqc2 $vf5, 0x0($sp)
        WRITE128(ADD32(RC_U32(rc_29), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b4b8: b label_113b4dc
        // 0x113b4bc: addiu $t2, $zero, 0x1 (Delay Slot)
        RC_SET_S32(rc_10, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 10, RC_U64X(rc_10));
        goto label_113b4dc;

    label_113b4c0:
        // 0x113b4c0: lq $v0, 0x30($a3)
        rc_7 = Ps2GprU64(ctx, 7);
        RC_SET_VEC(ctx, 2, rc_2, READ128(ADD32(RC_U32(rc_7), 48)));
        // 0x113b4c4: addiu $t2, $zero, 0x1
        RC_SET_S32(rc_10, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 10, RC_U64X(rc_10));
        // 0x113b4c8: b label_113b4dc
        // 0x113b4cc: sq $v0, 0x0($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        WRITE128(ADD32(RC_U32(rc_29), 0), GPR_VEC(ctx, 2));
        goto label_113b4dc;

    label_113b4d0:
        // 0x113b4d4: lq $v1, 0x0($v0)
        RC_SET_VEC(ctx, 3, rc_3, READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b4d8: sq $v1, 0x70($t1)
        rc_9 = Ps2GprU64(ctx, 9);
        WRITE128(ADD32(RC_U32(rc_9), 112), GPR_VEC(ctx, 3));

    label_113b4dc:
        // 0x113b4dc: addiu $v0, $zero, 0x1
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113b4e0: bnel $t2, $v0, label_113b57c
        {
            rc_10 = Ps2GprU64(ctx, 10);
            const bool branch_taken_0x113b4e0 = (RC_U64(rc_10) != RC_U64X(rc_2));
            rc_7 = Ps2GprU64(ctx, 7);
            if (branch_taken_0x113b4e0)
            {
                // 0x113b4e4: lq $v0, 0x30($a3) (Delay Slot)
                RC_SET_VEC(ctx, 2, rc_2, READ128(ADD32(RC_U32(rc_7), 48)));
                goto label_113b57c;
            }
        }
        // 0x113b4e8: lwc1 $f0, 0x6C($t3)
        rc_11 = Ps2GprU64(ctx, 11);
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_11), 108));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_0 = f;
        }
        ctx->f[0] = rf_0;
        // 0x113b4ec: mfc1 $v1, $f0
        {
            uint32_t bits;
            std::memcpy(&bits, &rf_0, sizeof(bits));
            RC_SET_U32(rc_3, bits);
        }
        // 0x113b4f0: lqc2 $vf5, 0x0($sp)
        rc_29 = Ps2GprU64(ctx, 29);
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        // 0x113b4f4: qmtc2.ni $v1, $vf6
        ctx->vu0_vf[6] = _mm_castsi128_ps(RC_VECX(ctx, 3, rc_3));
        // 0x113b4f8: vmulx.xyz $vf5, $vf5, $vf6x
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b4fc: sqc2 $vf5, 0x0($sp)
        WRITE128(ADD32(RC_U32(rc_29), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b500: mtc1 $zero, $f2
        {
            uint32_t bits = RC_U32(0);
            std::memcpy(&rf_2, &bits, sizeof(bits));
        }
        ctx->f[2] = rf_2;
        // 0x113b504: daddu $v0, $sp, $zero
        RC_SET_U64(rc_2, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 2, rc_2);
        // 0x113b508: addiu $v1, $zero, 0x2
        RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(0), 2));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));

    label_113b510:
        // 0x113b510: lwc1 $f0, 0x0($v0)
        rc_2 = Ps2GprU64(ctx, 2);
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_2), 0));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_0 = f;
        }
        ctx->f[0] = rf_0;
        // 0x113b514: c.lt.s $f0, $f2
        rf_2 = ctx->f[2];
        ctx->fcr31 = (FPU_C_OLT_S(rf_0, rf_2)) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        // 0x113b518: bc1f label_113b528
        if (!(ctx->fcr31 & 0x800000u))
        {
            goto label_113b528;
        }
        // 0x113b520: b label_113b53c
        // 0x113b524: sw $zero, 0x0($v0) (Delay Slot)
        WRITE32(ADD32(RC_U32(rc_2), 0), RC_U32(0));
        goto label_113b53c;

    label_113b528:
        // 0x113b528: lui $at, 0x3F80
        RC_SET_S32(rc_1, (int32_t)((uint32_t)16256 << 16));
        Ps2SetGpr64(ctx, 1, RC_U64X(rc_1));
        // 0x113b52c: mtc1 $at, $f1
        {
            uint32_t bits = RC_U32(rc_1);
            std::memcpy(&rf_1, &bits, sizeof(bits));
        }
        ctx->f[1] = rf_1;
        // 0x113b530: c.lt.s $f1, $f0
        rf_0 = ctx->f[0];
        ctx->fcr31 = (FPU_C_OLT_S(rf_1, rf_0)) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        // 0x113b534: bc1tl label_113b53c
        if (ctx->fcr31 & 0x800000u)
        {
            // 0x113b538: swc1 $f1, 0x0($v0) (Delay Slot)
            float f = rf_1;
            uint32_t bits;
            std::memcpy(&bits, &f, sizeof(bits));
            WRITE32(ADD32(RC_U32(rc_2), 0), bits);
        }

    label_113b53c:
        // 0x113b53c: addiu $v1, $v1, -0x1
        rc_3 = Ps2GprU64(ctx, 3);
        RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_3), 4294967295u));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113b540: bgez $v1, label_113b510
        // 0x113b544: addiu $v0, $v0, 0x4 (Delay Slot)
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_2), 4));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        if (RC_S32(rc_3) >= 0)
        {
            goto label_113b510;
        }

        // 0x113b548 / 0x113b550: relocated default light vector
        rc_9 = Ps2GprU64(ctx, 9);
        RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(rc_9), 112));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        RC_SET_S32(rc_2, (int32_t)relocated(rdram, 0x0113B548u, 0x0113B550u));
        // 0x113b554: lwc1 $f0, 0x68($t3)
        rc_11 = Ps2GprU64(ctx, 11);
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_11), 104));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_0 = f;
        }
        ctx->f[0] = rf_0;
        // 0x113b558: mfc1 $t0, $f0
        {
            uint32_t bits;
            std::memcpy(&bits, &rf_0, sizeof(bits));
            RC_SET_U32(rc_8, bits);
        }
        Ps2SetGpr64(ctx, 8, RC_U64X(rc_8));
        // 0x113b55c: qmtc2.ni $t0, $vf7
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        // 0x113b560: lqc2 $vf5, 0x0($sp)
        rc_29 = Ps2GprU64(ctx, 29);
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        // 0x113b564: lqc2 $vf6, 0x0($v0)
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b568: vsubx.w $vf8, $vf0, $vf7x
        {
            __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0, 0, 0, 0)));
            __m128i mask = _mm_set_epi32(-1, 0, 0, 0);
            ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask));
        }
        // 0x113b56c: vmulax.xyz $ACC, $vf5, $vf7x
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        }
        // 0x113b570: vmaddw.xyz $vf5, $vf6, $vf8w
        {
            __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3, 3, 3, 3)));
            __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b574: sqc2 $vf5, 0x0($v1)
        WRITE128(ADD32(RC_U32(rc_3), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b578: lq $v0, 0x30($a3)
        rc_7 = Ps2GprU64(ctx, 7);
        RC_SET_VEC(ctx, 2, rc_2, READ128(ADD32(RC_U32(rc_7), 48)));

    label_113b57c:
        // 0x113b57c: addiu $a0, $zero, 0x1
        RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4));
        // 0x113b580: sq $v0, 0x80($t1)
        rc_2 = Ps2GprU64(ctx, 2);
        rc_9 = Ps2GprU64(ctx, 9);
        WRITE128(ADD32(RC_U32(rc_9), 128), GPR_VEC(ctx, 2));
        // 0x113b584: lw $v1, 0xB0($t3)
        rc_11 = Ps2GprU64(ctx, 11);
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_11), 176)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113b588: beq $v1, $a0, label_113b59c
        {
            const bool branch_taken_0x113b588 = (RC_U64X(rc_3) == RC_U64X(rc_4));
            // 0x113b58c: addiu $v0, $zero, 0x2 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            if (branch_taken_0x113b588)
            {
                goto label_113b59c;
            }
        }
        // 0x113b590: lw $v1, 0x164($a2)
        rc_6 = Ps2GprU64(ctx, 6);
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_6), 356)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113b594: bne $v1, $v0, label_113b600
        {
            const bool branch_taken_0x113b594 = (RC_U64X(rc_3) != RC_U64X(rc_2));
            // 0x113b598: addiu $v0, $zero, 0x1 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            if (branch_taken_0x113b594)
            {
                goto label_113b600;
            }
        }

    label_113b59c:
        // 0x113b59c: lwc1 $f1, 0x0($t4)
        rc_12 = Ps2GprU64(ctx, 12);
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_12), 0));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_1 = f;
        }
        // 0x113b5a0: addiu $v0, $t1, 0x90
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_9), 144));
        // 0x113b5a4: lui $at, 0xBF80
        RC_SET_S32(rc_1, (int32_t)((uint32_t)49024 << 16));
        Ps2SetGpr64(ctx, 1, RC_U64X(rc_1));
        // 0x113b5a8: mtc1 $at, $f2
        {
            uint32_t bits = RC_U32(rc_1);
            std::memcpy(&rf_2, &bits, sizeof(bits));
        }
        ctx->f[2] = rf_2;
        // 0x113b5ac: swc1 $f1, 0x90($t1)
        {
            float f = rf_1;
            uint32_t bits;
            std::memcpy(&bits, &f, sizeof(bits));
            WRITE32(ADD32(RC_U32(rc_9), 144), bits);
        }
        // 0x113b5b0: mfc1 $v1, $f2
        {
            uint32_t bits;
            std::memcpy(&bits, &rf_2, sizeof(bits));
            RC_SET_U32(rc_3, bits);
        }
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113b5b4: lwc1 $f0, 0x10($t4)
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_12), 16));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_0 = f;
        }
        ctx->f[0] = rf_0;
        // 0x113b5b8: swc1 $f0, 0x94($t1)
        {
            float f = rf_0;
            uint32_t bits;
            std::memcpy(&bits, &f, sizeof(bits));
            WRITE32(ADD32(RC_U32(rc_9), 148), bits);
        }
        // 0x113b5bc: lwc1 $f1, 0x20($t4)
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_12), 32));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_1 = f;
        }
        ctx->f[1] = rf_1;
        // 0x113b5c0: sw $zero, 0x9C($t1)
        WRITE32(ADD32(RC_U32(rc_9), 156), RC_U32(0));
        // 0x113b5c4: swc1 $f1, 0x98($t1)
        {
            float f = rf_1;
            uint32_t bits;
            std::memcpy(&bits, &f, sizeof(bits));
            WRITE32(ADD32(RC_U32(rc_9), 152), bits);
        }
        // 0x113b5c8: lqc2 $vf5, 0x0($v0)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b5cc: qmtc2.ni $v1, $vf6
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
        // 0x113b5d0: vmulx.xyz $vf5, $vf5, $vf6x
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b5d4: sqc2 $vf5, 0x0($v0)
        WRITE128(ADD32(RC_U32(rc_2), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b5d8: lqc2 $vf5, 0x0($v0)
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        // 0x113b5dc: vmul.xyz $vf7, $vf5, $vf5
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
        }
        // 0x113b5e0: vmulax.w $ACC, $vf0, $vf7x
        {
            __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
        }
        // 0x113b5e4: vmadday.w $ACC, $vf0, $vf7y
        {
            __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1, 1, 1, 1)));
            __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
            ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
        }
        // 0x113b5e8: vmaddz.w $vf7, $vf0, $vf7z
        {
            __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2, 2, 2, 2)));
            __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
            __m128i mask = _mm_set_epi32(-1, 0, 0, 0);
            ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
        }
        // 0x113b5ec: vrsqrt $Q, $vf0w, $vf7w
        {
            float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0, 0, 0, 3)));
            float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0, 0, 0, 3)));
            ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft);
        }
        // 0x113b5f4: vmulq.xyz $vf5, $vf5, $Q
        {
            __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q));
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        }
        // 0x113b5f8: sqc2 $vf5, 0x0($v0)
        WRITE128(ADD32(RC_U32(rc_2), 0), _mm_castps_si128(ctx->vu0_vf[5]));
        // 0x113b5fc: addiu $v0, $zero, 0x1
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));

    label_113b600:
        // 0x113b600: sw $v0, 0x110($t1)
        rc_2 = Ps2GprU64(ctx, 2);
        rc_9 = Ps2GprU64(ctx, 9);
        WRITE32(ADD32(RC_U32(rc_9), 272), RC_U32(rc_2));

    label_113b604:
        // 0x113b608: addiu $sp, $sp, 0x10 (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 16));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        returnTo(ctx);
    }

    // disp_particle_emitter (0x0113D988)
    void disp_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint64_t rc_2;
        uint64_t rc_3;
        uint64_t rc_4;
        uint64_t rc_5;
        uint64_t rc_6;
        uint64_t rc_7;
        uint64_t rc_8;
        uint64_t rc_16;
        uint64_t rc_17;
        uint64_t rc_18;
        uint64_t rc_19;
        uint64_t rc_20;
        uint64_t rc_21;
        uint64_t rc_22;
        uint64_t rc_29;
        uint64_t rc_31;
        float rf_12;

        // 0x113d988: addiu $sp, $sp, -0x90
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294967152u));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x113d98c: addiu $v0, $zero, 0x1
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113d990: sd $s1, 0x58($sp)
        rc_17 = Ps2GprU64(ctx, 17);
        WRITE64(ADD32(RC_U32(rc_29), 88), RC_U64(rc_17));
        // 0x113d994: daddu $s1, $a0, $zero
        rc_4 = Ps2GprU64(ctx, 4);
        RC_SET_U64(rc_17, (uint64_t)RC_U64(rc_4) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 17, rc_17);
        // 0x113d998: sd $s2, 0x60($sp)
        rc_18 = Ps2GprU64(ctx, 18);
        WRITE64(ADD32(RC_U32(rc_29), 96), RC_U64(rc_18));
        // 0x113d99c: daddu $s2, $a1, $zero
        rc_5 = Ps2GprU64(ctx, 5);
        RC_SET_U64(rc_18, (uint64_t)RC_U64(rc_5) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 18, rc_18);
        // 0x113d9a0: sd $s5, 0x78($sp)
        rc_21 = Ps2GprU64(ctx, 21);
        WRITE64(ADD32(RC_U32(rc_29), 120), RC_U64(rc_21));
        // 0x113d9a4: sd $s0, 0x50($sp)
        rc_16 = Ps2GprU64(ctx, 16);
        WRITE64(ADD32(RC_U32(rc_29), 80), RC_U64(rc_16));
        // 0x113d9a8: sd $s3, 0x68($sp)
        rc_19 = Ps2GprU64(ctx, 19);
        WRITE64(ADD32(RC_U32(rc_29), 104), RC_U64(rc_19));
        // 0x113d9ac: sd $s4, 0x70($sp)
        rc_20 = Ps2GprU64(ctx, 20);
        WRITE64(ADD32(RC_U32(rc_29), 112), RC_U64(rc_20));
        // 0x113d9b0: sd $s6, 0x80($sp)
        rc_22 = Ps2GprU64(ctx, 22);
        WRITE64(ADD32(RC_U32(rc_29), 128), RC_U64(rc_22));
        // 0x113d9b4: sd $ra, 0x88($sp)
        rc_31 = Ps2GprU64(ctx, 31);
        WRITE64(ADD32(RC_U32(rc_29), 136), RC_U64(rc_31));
        // 0x113d9b8: lw $s4, 0x198($s1)
        RC_SET_S32(rc_20, (int32_t)READ32(ADD32(RC_U32(rc_17), 408)));
        Ps2SetGpr64(ctx, 20, RC_U64X(rc_20));
        // 0x113d9bc: bne $s4, $v0, label_113dbcc
        {
            const bool branch_taken_0x113d9bc = (RC_U64X(rc_20) != RC_U64X(rc_2));
            // 0x113d9c0: daddu $s5, $a2, $zero (Delay Slot)
            rc_6 = Ps2GprU64(ctx, 6);
            RC_SET_U64(rc_21, (uint64_t)RC_U64(rc_6) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 21, rc_21);
            if (branch_taken_0x113d9bc)
            {
                goto label_113dbcc;
            }
        }
        // 0x113d9c4: lw $v1, 0x118($s1)
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_17), 280)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113d9c8: addiu $s6, $zero, 0x4
        RC_SET_S32(rc_22, (int32_t)ADD32(RC_U32(0), 4));
        Ps2SetGpr64(ctx, 22, RC_U64X(rc_22));
        // 0x113d9cc: lw $a0, 0xD4($s2)
        RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_18), 212)));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4));
        // 0x113d9d0..0x113d9e0: v0 = v1 * 0x1B8
        RC_SET_S32(rc_2, (int32_t)SLL32(RC_U32(rc_3), 3));
        RC_SET_S32(rc_2, (int32_t)SUB32(RC_U32(rc_2), RC_U32(rc_3)));
        RC_SET_S32(rc_2, (int32_t)SLL32(RC_U32(rc_2), 4));
        RC_SET_S32(rc_2, (int32_t)SUB32(RC_U32(rc_2), RC_U32(rc_3)));
        RC_SET_S32(rc_2, (int32_t)SLL32(RC_U32(rc_2), 2));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113d9e4: addu $s3, $a0, $v0
        RC_SET_S32(rc_19, (int32_t)ADD32(RC_U32(rc_4), RC_U32(rc_2)));
        Ps2SetGpr64(ctx, 19, RC_U64X(rc_19));
        // 0x113d9e8: lw $s0, 0xC($s3)
        RC_SET_S32(rc_16, (int32_t)READ32(ADD32(RC_U32(rc_19), 12)));
        Ps2SetGpr64(ctx, 16, RC_U64X(rc_16));
        // 0x113d9ec: beq $s0, $s6, label_113da1c
        {
            const bool branch_taken_0x113d9ec = (RC_U64X(rc_16) == RC_U64X(rc_22));
            // 0x113d9f0: daddu $a1, $zero, $zero (Delay Slot)
            RC_SET_U64(rc_5, (uint64_t)RC_U64(0) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
            if (branch_taken_0x113d9ec)
            {
                goto label_113da1c;
            }
        }
        // 0x113d9f4: lw $v1, 0x8($a0)
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_4), 8)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x113d9f8..0x113da00: v0 = v1 * 12
        RC_SET_S32(rc_2, (int32_t)SLL32(RC_U32(rc_3), 1));
        RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(rc_2), RC_U32(rc_3)));
        RC_SET_S32(rc_2, (int32_t)SLL32(RC_U32(rc_2), 2));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113da04..0x113da0c: lw $a0, -0x2FB0(0x01190000 + $v0)
        RC_SET_S32(rc_4, (int32_t)((uint32_t)281 << 16));
        RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_4), RC_U32(rc_2)));
        RC_SET_S32(rc_4, (int32_t)READ32(ADD32(RC_U32(rc_4), 4294955088u)));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4));
        // 0x113da14: daddu $a2, $zero, $zero (Delay Slot)
        RC_SET_U64(rc_6, 0ULL);
        Ps2SetGpr64(ctx, 6, RC_U64X(rc_6));
        // 0x113da10: jal 0x010AAD48 (texTransRequest)
        callGuest(rdram, ctx, runtime, 0x010AAD48u, 0x0113DA18u, RC_U32(rc_29));

        // 0x113da18: lw $s0, 0xC($s3)
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_S32(rc_16, (int32_t)READ32(ADD32(RC_U32(rc_19), 12)));
        Ps2SetGpr64(ctx, 16, RC_U64X(rc_16));

    label_113da1c:
        // 0x113da1c: bnez $s0, label_113da30
        rc_16 = Ps2GprU64(ctx, 16);
        if (RC_U64(rc_16) != RC_U64(0))
        {
            goto label_113da30;
        }
        // 0x113da24: break 0
        ctx->pc = 0x0113DA24u;
        runtime->handleBreak(rdram, ctx);
        // 0x113da2c: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113da30:
        // 0x113da30: bne $s0, $s4, label_113db38
        {
            rc_16 = Ps2GprU64(ctx, 16);
            rc_20 = Ps2GprU64(ctx, 20);
            const bool branch_taken_0x113da30 = (RC_U64(rc_16) != RC_U64(rc_20));
            // 0x113da34: addiu $v0, $zero, 0x2 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            if (branch_taken_0x113da30)
            {
                goto label_113db38;
            }
        }
        // 0x113da38: lw $v0, 0x18C($s3)
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_19), 396)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113da3c: bnez $v0, label_113db08
        {
            const bool branch_taken_0x113da3c = (RC_U64X(rc_2) != RC_U64(0));
            // 0x113da40: daddu $a0, $sp, $zero (Delay Slot)
            rc_29 = Ps2GprU64(ctx, 29);
            RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 4, rc_4);
            if (branch_taken_0x113da3c)
            {
                goto label_113db08;
            }
        }
        // 0x113da44: lw $v0, 0xB0($s2)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_18), 176)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113da48: bnel $v0, $s0, label_113da68
        if (RC_U64X(rc_2) != RC_U64(rc_16))
        {
            // 0x113da4c: lw $v0, 0x120($s2) (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_18), 288)));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            goto label_113da68;
        }
        // 0x113da50: daddu $a0, $s1, $zero
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113da54: daddu $a1, $s3, $zero
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113da5c: daddu $a2, $s2, $zero (Delay Slot)
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113da58: jal 0x0113E940 (particleDraw)
        callGuest(rdram, ctx, runtime, 0x0113E940u, 0x0113DA60u, RC_U32(rc_29));
        // 0x113da64: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113da68:
        // 0x113da68: slti $v0, $v0, 0x2
        rc_2 = Ps2GprU64(ctx, 2);
        RC_SET_U64(rc_2, ((int64_t)RC_S64(rc_2) < (int64_t)(int32_t)2) ? 1 : 0);
        // 0x113da6c: sltiu $v0, $v0, 0x1
        RC_SET_U64(rc_2, ((uint64_t)RC_U64X(rc_2) < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113da70: bne $v0, $s0, label_113daa8
        {
            rc_16 = Ps2GprU64(ctx, 16);
            const bool branch_taken_0x113da70 = (RC_U64X(rc_2) != RC_U64(rc_16));
            // 0x113da74: addiu $a1, $s1, 0x30 (Delay Slot)
            rc_17 = Ps2GprU64(ctx, 17);
            RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_17), 48));
            Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
            if (branch_taken_0x113da70)
            {
                goto label_113daa8;
            }
        }
        // 0x113da78: daddu $a0, $sp, $zero
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113da7c: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113da84: addiu $a3, $zero, 0x1 (Delay Slot)
        RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113da80: jal 0x01140418 (get_world_matrixSR)
        callGuest(rdram, ctx, runtime, kGetWorldMatrixSR, 0x0113DA88u, RC_U32(rc_29));
        // 0x113da88: daddu $a0, $s1, $zero
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113da8c: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113da90: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113da98: daddu $a3, $sp, $zero (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 7, rc_7);
        // 0x113da94: jal 0x0113E6C8
        callGuest(rdram, ctx, runtime, 0x0113E6C8u, 0x0113DA9Cu, RC_U32(rc_29));
        // 0x113daa0: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113daa8:
        // 0x113daac: daddu $a0, $s2, $zero (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113daa8: jal 0x01141E80 (ptclEmitterMatrixMode)
        rc_29 = Ps2GprU64(ctx, 29);
        callGuest(rdram, ctx, runtime, kPtclEmitterMatrixMode, 0x0113DAB0u, RC_U32(rc_29));
        // 0x113dab0: daddu $s4, $v0, $zero
        rc_2 = Ps2GprU64(ctx, 2);
        RC_SET_U64(rc_20, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 20, rc_20);
        // 0x113dab4: bne $s4, $s0, label_113dad8
        {
            rc_16 = Ps2GprU64(ctx, 16);
            const bool branch_taken_0x113dab4 = (RC_U64(rc_20) != RC_U64(rc_16));
            // 0x113dab8: daddu $a0, $sp, $zero (Delay Slot)
            rc_29 = Ps2GprU64(ctx, 29);
            RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 4, rc_4);
            if (branch_taken_0x113dab4)
            {
                goto label_113dad8;
            }
        }
        // 0x113dabc: addiu $a1, $s1, 0x30
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_17), 48));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x113dac0: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113dac8: daddu $a3, $zero, $zero (Delay Slot)
        RC_SET_U64(rc_7, 0ULL);
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113dac4: jal 0x01140418 (get_world_matrixSR)
        callGuest(rdram, ctx, runtime, kGetWorldMatrixSR, 0x0113DACCu, RC_U32(rc_29));
        // 0x113dad0: daddu $a0, $s1, $zero (Delay Slot)
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        goto label_113daec;

    label_113dad8:
        // 0x113dad8: addiu $a1, $s1, 0x30
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_17), 48));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x113dadc: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113dae4: addiu $a3, $zero, 0x1 (Delay Slot)
        RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113dae0: jal 0x01140418 (get_world_matrixSR)
        callGuest(rdram, ctx, runtime, kGetWorldMatrixSR, 0x0113DAE8u, RC_U32(rc_29));
        // 0x113dae8: daddu $a0, $s1, $zero
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);

    label_113daec:
        // 0x113daec: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113daf0: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113daf4: daddu $a3, $s4, $zero
        rc_20 = Ps2GprU64(ctx, 20);
        RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_20) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 7, rc_7);
        // 0x113dafc: daddu $t0, $sp, $zero (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 8, rc_8);
        // 0x113daf8: jal 0x0113DBF8
        callGuest(rdram, ctx, runtime, 0x0113DBF8u, 0x0113DB00u, RC_U32(rc_29));
        // 0x113db04: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113db08:
        // 0x113db08: addiu $a1, $s1, 0x30
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_17), 48));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x113db0c: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113db14: daddu $a3, $zero, $zero (Delay Slot)
        RC_SET_U64(rc_7, 0ULL);
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113db10: jal 0x01140418 (get_world_matrixSR)
        rc_29 = Ps2GprU64(ctx, 29);
        callGuest(rdram, ctx, runtime, kGetWorldMatrixSR, 0x0113DB18u, RC_U32(rc_29));
        // 0x113db18: daddu $a0, $s1, $zero
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113db1c: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113db20: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113db28: daddu $a3, $sp, $zero (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 7, rc_7);
        // 0x113db24: jal 0x01141D20
        callGuest(rdram, ctx, runtime, 0x01141D20u, 0x0113DB2Cu, RC_U32(rc_29));
        // 0x113db30: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113db38:
        // 0x113db38: bne $s0, $v0, label_113db78
        {
            rc_2 = Ps2GprU64(ctx, 2);
            rc_16 = Ps2GprU64(ctx, 16);
            const bool branch_taken_0x113db38 = (RC_U64(rc_16) != RC_U64(rc_2));
            // 0x113db3c: addiu $v0, $zero, 0x3 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 3));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            if (branch_taken_0x113db38)
            {
                goto label_113db78;
            }
        }
        // 0x113db40: lw $v0, 0xB0($s2)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_18), 176)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x113db44: bne $v0, $s4, label_113db60
        {
            rc_20 = Ps2GprU64(ctx, 20);
            const bool branch_taken_0x113db44 = (RC_U64X(rc_2) != RC_U64(rc_20));
            // 0x113db48: daddu $a0, $s1, $zero (Delay Slot)
            rc_17 = Ps2GprU64(ctx, 17);
            RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
            Ps2SetGpr64(ctx, 4, rc_4);
            if (branch_taken_0x113db44)
            {
                goto label_113db60;
            }
        }
        // 0x113db4c: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113db54: daddu $a2, $s2, $zero (Delay Slot)
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113db50: jal 0x0113EC40
        rc_29 = Ps2GprU64(ctx, 29);
        callGuest(rdram, ctx, runtime, 0x0113EC40u, 0x0113DB58u, RC_U32(rc_29));
        // 0x113db5c: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113db60:
        // 0x113db60: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113db68: daddu $a2, $s2, $zero (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113db64: jal 0x0113DEA0
        rc_29 = Ps2GprU64(ctx, 29);
        callGuest(rdram, ctx, runtime, 0x0113DEA0u, 0x0113DB6Cu, RC_U32(rc_29));
        // 0x113db70: ld $s0, 0x50($sp) (Delay Slot)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);
        goto label_113dbd0;

    label_113db78:
        // 0x113db78: beql $s0, $v0, label_113dbd0
        rc_2 = Ps2GprU64(ctx, 2);
        rc_16 = Ps2GprU64(ctx, 16);
        rc_29 = Ps2GprU64(ctx, 29);
        if (RC_U64(rc_16) == RC_U64(rc_2))
        {
            // 0x113db7c: ld $s0, 0x50($sp) (Delay Slot)
            RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
            Ps2SetGpr64(ctx, 16, rc_16);
            goto label_113dbd0;
        }
        // 0x113db80: bnel $s0, $s6, label_113dbd0
        rc_22 = Ps2GprU64(ctx, 22);
        if (RC_U64(rc_16) != RC_U64(rc_22))
        {
            // 0x113db84: ld $s0, 0x50($sp) (Delay Slot)
            RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
            Ps2SetGpr64(ctx, 16, rc_16);
            goto label_113dbd0;
        }
        // 0x113db88: lwc1 $f12, 0x16C($s1)
        rc_17 = Ps2GprU64(ctx, 17);
        {
            uint32_t bits = READ32(ADD32(RC_U32(rc_17), 364));
            float f;
            std::memcpy(&f, &bits, sizeof(f));
            rf_12 = f;
        }
        ctx->f[12] = rf_12;
        // 0x113db8c: addiu $a0, $sp, 0x40
        RC_SET_S32(rc_4, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 4, RC_U64X(rc_4));
        // 0x113db90: lw $a3, 0x118($s1)
        RC_SET_S32(rc_7, (int32_t)READ32(ADD32(RC_U32(rc_17), 280)));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113db94: daddu $a2, $s5, $zero
        rc_21 = Ps2GprU64(ctx, 21);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_21) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113db9c: daddu $a1, $s2, $zero (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113db98: jal 0x01145BA8 (get_particle_parameter_blend_disp)
        callGuest(rdram, ctx, runtime, 0x01145BA8u, 0x0113DBA0u, RC_U32(rc_29));
        // 0x113dba0: addiu $a1, $s1, 0x30
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_17), 48));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x113dba4: daddu $a0, $sp, $zero
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113dba8: addiu $a3, $zero, 0x1
        RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113dbb0: daddu $a2, $s2, $zero (Delay Slot)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113dbac: jal 0x01140418 (get_world_matrixSR)
        callGuest(rdram, ctx, runtime, kGetWorldMatrixSR, 0x0113DBB4u, RC_U32(rc_29));
        // 0x113dbb4: daddu $a0, $s1, $zero
        rc_17 = Ps2GprU64(ctx, 17);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_17) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x113dbb8: daddu $a1, $s3, $zero
        rc_19 = Ps2GprU64(ctx, 19);
        RC_SET_U64(rc_5, (uint64_t)RC_U64(rc_19) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 5, rc_5);
        // 0x113dbbc: daddu $a2, $s2, $zero
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_18) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x113dbc0: addiu $a3, $sp, 0x40
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(rc_29), 64));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x113dbc8: daddu $t0, $sp, $zero (Delay Slot)
        RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 8, rc_8);
        // 0x113dbc4: jal 0x0113E218
        callGuest(rdram, ctx, runtime, 0x0113E218u, 0x0113DBCCu, RC_U32(rc_29));

    label_113dbcc:
        // 0x113dbcc: ld $s0, 0x50($sp)
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 16, rc_16);

    label_113dbd0:
        // 0x113dbd0..0x113dbf0: restore callee-saved registers and return
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_17, READ64(ADD32(RC_U32(rc_29), 88)));
        Ps2SetGpr64(ctx, 17, rc_17);
        RC_SET_U64(rc_18, READ64(ADD32(RC_U32(rc_29), 96)));
        Ps2SetGpr64(ctx, 18, rc_18);
        RC_SET_U64(rc_19, READ64(ADD32(RC_U32(rc_29), 104)));
        Ps2SetGpr64(ctx, 19, rc_19);
        RC_SET_U64(rc_20, READ64(ADD32(RC_U32(rc_29), 112)));
        Ps2SetGpr64(ctx, 20, rc_20);
        RC_SET_U64(rc_21, READ64(ADD32(RC_U32(rc_29), 120)));
        Ps2SetGpr64(ctx, 21, rc_21);
        RC_SET_U64(rc_22, READ64(ADD32(RC_U32(rc_29), 128)));
        Ps2SetGpr64(ctx, 22, rc_22);
        RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 136)));
        Ps2SetGpr64(ctx, 31, rc_31);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 144));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        returnTo(ctx);
    }

    // get_world_matrixSR (0x01140418)
    void get_world_matrixSR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint64_t rc_2;
        uint64_t rc_5;
        uint64_t rc_6;
        uint64_t rc_7;
        uint64_t rc_8;
        uint64_t rc_29;
        uint64_t rc_31;

        // 0x1140418: addiu $sp, $sp, -0x10
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294967280u));
        // 0x114041c: daddu $v0, $a2, $zero
        rc_6 = Ps2GprU64(ctx, 6);
        RC_SET_U64(rc_2, (uint64_t)RC_U64(rc_6) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 2, rc_2);
        // 0x1140420: sd $ra, 0x0($sp)
        rc_31 = Ps2GprU64(ctx, 31);
        WRITE64(ADD32(RC_U32(rc_29), 0), RC_U64(rc_31));
        // 0x1140424: daddu $t0, $a3, $zero
        rc_7 = Ps2GprU64(ctx, 7);
        RC_SET_U64(rc_8, (uint64_t)RC_U64(rc_7) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 8, rc_8);
        // 0x1140428: daddu $a2, $a1, $zero
        rc_5 = Ps2GprU64(ctx, 5);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_5) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x114042c: addiu $a1, $v0, 0xE0
        RC_SET_S32(rc_5, (int32_t)ADD32(RC_U32(rc_2), 224));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x1140430: ld $ra, 0x0($sp)
        RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 0)));
        Ps2SetGpr64(ctx, 31, rc_31);
        // 0x1140434: daddu $a3, $v0, $zero
        RC_SET_U64(rc_7, (uint64_t)RC_U64(rc_2) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 7, rc_7);
        // 0x114043c: addiu $sp, $sp, 0x10 (Delay Slot)
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 16));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x1140438: j 0x01141EA8 (ptclWorldMatrixSR)
        callGuest(rdram, ctx, runtime, kPtclWorldMatrixSR, RC_U32(rc_31), RC_U32(rc_29));
    }

    // ptclEmitterMatrixMode (manager_sub_01141E80, 0x01141E80)
    void ptclEmitterMatrixMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (void)runtime;
        uint64_t rc_2;
        uint64_t rc_3;
        uint64_t rc_4;

        // 0x1141e80: lw $v0, 0x34($a0)
        rc_4 = Ps2GprU64(ctx, 4);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_4), 52)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x1141e84: addiu $v1, $zero, 0x1
        RC_SET_S32(rc_3, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x1141e88: bne $v0, $v1, label_1141ea0
        {
            const bool branch_taken_0x1141e88 = (RC_U64X(rc_2) != RC_U64X(rc_3));
            // 0x1141e8c: addiu $v0, $zero, 0x2 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            if (branch_taken_0x1141e88)
            {
                returnTo(ctx);
                return;
            }
        }
        // 0x1141e90: lw $v1, 0xDC($a0)
        RC_SET_S32(rc_3, (int32_t)READ32(ADD32(RC_U32(rc_4), 220)));
        Ps2SetGpr64(ctx, 3, RC_U64X(rc_3));
        // 0x1141e94: beqz $v1, label_1141ea0
        // 0x1141e98: addiu $v0, $zero, 0x1 (Delay Slot)
        // 0x1141e9c: addiu $v0, $zero, 0x2
        RC_SET_S32(rc_2, (RC_U64X(rc_3) == RC_U64(0)) ? 1 : 2);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        returnTo(ctx);
    }

    // ptclWorldMatrixSR (manager_sub_01141EA8, 0x01141EA8)
    void ptclWorldMatrixSR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint64_t rc_2;
        uint64_t rc_4;
        uint64_t rc_5;
        uint64_t rc_6;
        uint64_t rc_7;
        uint64_t rc_8;
        uint64_t rc_16;
        uint64_t rc_17;
        uint64_t rc_18;
        uint64_t rc_19;
        uint64_t rc_29;
        uint64_t rc_31;

        // 0x1141ea8: addiu $sp, $sp, -0x70
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 4294967184u));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        // 0x1141eac: sd $s2, 0x50($sp)
        rc_18 = Ps2GprU64(ctx, 18);
        WRITE64(ADD32(RC_U32(rc_29), 80), RC_U64(rc_18));
        // 0x1141eb0: daddu $s2, $a3, $zero
        rc_7 = Ps2GprU64(ctx, 7);
        RC_SET_U64(rc_18, (uint64_t)RC_U64(rc_7) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 18, rc_18);
        // 0x1141eb4: sd $s0, 0x40($sp)
        rc_16 = Ps2GprU64(ctx, 16);
        WRITE64(ADD32(RC_U32(rc_29), 64), RC_U64(rc_16));
        // 0x1141eb8: daddu $s0, $a2, $zero
        rc_6 = Ps2GprU64(ctx, 6);
        RC_SET_U64(rc_16, (uint64_t)RC_U64(rc_6) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 16, rc_16);
        // 0x1141ebc: sd $s1, 0x48($sp)
        rc_17 = Ps2GprU64(ctx, 17);
        WRITE64(ADD32(RC_U32(rc_29), 72), RC_U64(rc_17));
        // 0x1141ec0: daddu $s1, $a0, $zero
        rc_4 = Ps2GprU64(ctx, 4);
        RC_SET_U64(rc_17, (uint64_t)RC_U64(rc_4) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 17, rc_17);
        // 0x1141ec4: sd $s3, 0x58($sp)
        rc_19 = Ps2GprU64(ctx, 19);
        WRITE64(ADD32(RC_U32(rc_29), 88), RC_U64(rc_19));
        // 0x1141ec8: sd $ra, 0x60($sp)
        rc_31 = Ps2GprU64(ctx, 31);
        WRITE64(ADD32(RC_U32(rc_29), 96), RC_U64(rc_31));
        // 0x1141ed0: daddu $s3, $t0, $zero (Delay Slot)
        rc_8 = Ps2GprU64(ctx, 8);
        RC_SET_U64(rc_19, (uint64_t)RC_U64(rc_8) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 19, rc_19);
        // 0x1141ecc: jal (relocated iosCopyMatrix)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01141ECCu), 0x01141ED4u, RC_U32(rc_29));

        // 0x1141ed4: lw $v0, 0x60($s2)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_S32(rc_2, (int32_t)READ32(ADD32(RC_U32(rc_18), 96)));
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        // 0x1141edc: daddu $a2, $s0, $zero
        rc_16 = Ps2GprU64(ctx, 16);
        RC_SET_U64(rc_6, (uint64_t)RC_U64(rc_16) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 6, rc_6);
        // 0x1141ee0: addiu $s0, $zero, 0x1
        RC_SET_S32(rc_16, (int32_t)ADD32(RC_U32(0), 1));
        Ps2SetGpr64(ctx, 16, RC_U64X(rc_16));
        // 0x1141ed8 / 0x1141ee4: relocated $a1
        RC_SET_S32(rc_5, (int32_t)relocated(rdram, 0x01141ED8u, 0x01141EE4u));
        Ps2SetGpr64(ctx, 5, RC_U64X(rc_5));
        // 0x1141ee8: daddu $a0, $sp, $zero
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_4, (uint64_t)RC_U64(rc_29) + (uint64_t)RC_U64(0));
        Ps2SetGpr64(ctx, 4, rc_4);
        // 0x1141eec: bne $v0, $s0, label_1141f6c
        {
            const bool branch_taken_0x1141eec = (RC_U64X(rc_2) != RC_U64X(rc_16));
            // 0x1141ef0: addiu $a3, $zero, 0x1 (Delay Slot)
            RC_SET_S32(rc_7, (int32_t)ADD32(RC_U32(0), 1));
            Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
            if (branch_taken_0x1141eec)
            {
                goto label_1141f6c;
            }
        }
        // 0x1141ef4: jal 0x011337E8 (iosRotMatrixFromDirection)
        callGuest(rdram, ctx, runtime, 0x011337E8u, 0x01141EFCu, RC_U32(rc_29));

        // 0x1141efc..0x1141f68: VU0 matrix multiply (sp * s1 -> s1)
        rc_29 = Ps2GprU64(ctx, 29);
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 0)));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 16)));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 32)));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_29), 48)));
        rc_17 = Ps2GprU64(ctx, 17);
        ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 0)));
        ctx->vu0_vf[25] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 16)));
        ctx->vu0_vf[26] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 32)));
        ctx->vu0_vf[27] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 48)));
        for (int i = 24; i <= 27; ++i)
        {
            const __m128 v = ctx->vu0_vf[i];
            {
                __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
                ctx->vu0_vf[i] = _mm_blendv_ps(ctx->vu0_vf[i], res, _mm_castsi128_ps(mask));
            }
        }
        WRITE128(ADD32(RC_U32(rc_17), 0), _mm_castps_si128(ctx->vu0_vf[24]));
        WRITE128(ADD32(RC_U32(rc_17), 16), _mm_castps_si128(ctx->vu0_vf[25]));
        WRITE128(ADD32(RC_U32(rc_17), 32), _mm_castps_si128(ctx->vu0_vf[26]));
        WRITE128(ADD32(RC_U32(rc_17), 48), _mm_castps_si128(ctx->vu0_vf[27]));

    label_1141f6c:
        // 0x1141f6c: bne $s3, $s0, label_1142014
        {
            rc_16 = Ps2GprU64(ctx, 16);
            rc_19 = Ps2GprU64(ctx, 19);
            const bool branch_taken_0x1141f6c = (RC_U64(rc_19) != RC_U64(rc_16));
            // 0x1141f70: ld $s0, 0x40($sp) (Delay Slot)
            rc_29 = Ps2GprU64(ctx, 29);
            RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 64)));
            Ps2SetGpr64(ctx, 16, rc_16);
            if (branch_taken_0x1141f6c)
            {
                goto label_1142014;
            }
        }
        // 0x1141f74: lw $a3, 0x34($s2)
        rc_18 = Ps2GprU64(ctx, 18);
        RC_SET_S32(rc_7, (int32_t)READ32(ADD32(RC_U32(rc_18), 52)));
        Ps2SetGpr64(ctx, 7, RC_U64X(rc_7));
        // 0x1141f78: bnel $a3, $s3, label_1141f90
        if (RC_U64X(rc_7) != RC_U64(rc_19))
        {
            // 0x1141f7c: addiu $v0, $zero, 0x2 (Delay Slot)
            RC_SET_S32(rc_2, (int32_t)ADD32(RC_U32(0), 2));
            Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
            goto label_1141f90;
        }
        // 0x1141f80..0x1141f88: $v0 = 0x01216E40
        RC_SET_S32(rc_2, (int32_t)0x01216E40u);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));
        goto label_1141fa0;

    label_1141f90:
        // 0x1141f90: bne $a3, $v0, label_1142014
        {
            rc_2 = Ps2GprU64(ctx, 2);
            rc_7 = Ps2GprU64(ctx, 7);
            const bool branch_taken_0x1141f90 = (RC_U64(rc_7) != RC_U64(rc_2));
            // 0x1141f94: ld $s0, 0x40($sp) (Delay Slot)
            rc_29 = Ps2GprU64(ctx, 29);
            RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 64)));
            Ps2SetGpr64(ctx, 16, rc_16);
            if (branch_taken_0x1141f90)
            {
                goto label_1142014;
            }
        }
        // 0x1141f98..0x1141f9c: $v0 = 0x01216E80
        RC_SET_S32(rc_2, (int32_t)0x01216E80u);
        Ps2SetGpr64(ctx, 2, RC_U64X(rc_2));

    label_1141fa0:
        // 0x1141fa0..0x1142010: VU0 matrix multiply (v0 * s1 -> s1)
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 0)));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 16)));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 32)));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_2), 48)));
        rc_17 = Ps2GprU64(ctx, 17);
        ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 0)));
        ctx->vu0_vf[25] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 16)));
        ctx->vu0_vf[26] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 32)));
        ctx->vu0_vf[27] = _mm_castsi128_ps(READ128(ADD32(RC_U32(rc_17), 48)));
        for (int i = 24; i <= 27; ++i)
        {
            const __m128 v = ctx->vu0_vf[i];
            {
                __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            {
                __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
                __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res);
                __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
                ctx->vu0_vf[i] = _mm_blendv_ps(ctx->vu0_vf[i], res, _mm_castsi128_ps(mask));
            }
        }
        WRITE128(ADD32(RC_U32(rc_17), 0), _mm_castps_si128(ctx->vu0_vf[24]));
        WRITE128(ADD32(RC_U32(rc_17), 16), _mm_castps_si128(ctx->vu0_vf[25]));
        WRITE128(ADD32(RC_U32(rc_17), 32), _mm_castps_si128(ctx->vu0_vf[26]));
        WRITE128(ADD32(RC_U32(rc_17), 48), _mm_castps_si128(ctx->vu0_vf[27]));
        // 0x1142010: ld $s0, 0x40($sp)
        RC_SET_U64(rc_16, READ64(ADD32(RC_U32(rc_29), 64)));
        Ps2SetGpr64(ctx, 16, rc_16);

    label_1142014:
        // 0x1142014..0x1142028: restore callee-saved registers and return
        rc_29 = Ps2GprU64(ctx, 29);
        RC_SET_U64(rc_17, READ64(ADD32(RC_U32(rc_29), 72)));
        Ps2SetGpr64(ctx, 17, rc_17);
        RC_SET_U64(rc_18, READ64(ADD32(RC_U32(rc_29), 80)));
        Ps2SetGpr64(ctx, 18, rc_18);
        RC_SET_U64(rc_19, READ64(ADD32(RC_U32(rc_29), 88)));
        Ps2SetGpr64(ctx, 19, rc_19);
        RC_SET_U64(rc_31, READ64(ADD32(RC_U32(rc_29), 96)));
        Ps2SetGpr64(ctx, 31, rc_31);
        RC_SET_S32(rc_29, (int32_t)ADD32(RC_U32(rc_29), 112));
        Ps2SetGpr64(ctx, 29, RC_U64X(rc_29));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01140ED0u, ptclDispEmitterNode, decomp_ptclemitter::ptclDispEmitterNode);
SOTC_DECOMP_FUNCTION(0x0113B408u, ptclEmitterLightDir, decomp_ptclemitter::ptclEmitterLightDir);
SOTC_DECOMP_FUNCTION(0x0113D988u, disp_particle_emitter, decomp_ptclemitter::disp_particle_emitter);
SOTC_DECOMP_FUNCTION(0x01140418u, get_world_matrixSR, decomp_ptclemitter::get_world_matrixSR);
SOTC_DECOMP_FUNCTION(0x01141E80u, ptclEmitterMatrixMode, decomp_ptclemitter::ptclEmitterMatrixMode);
SOTC_DECOMP_FUNCTION(0x01141EA8u, ptclWorldMatrixSR, decomp_ptclemitter::ptclWorldMatrixSR);
