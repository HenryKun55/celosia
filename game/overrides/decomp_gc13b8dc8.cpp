// gamecore_sub_013B8DC8 (round 41, 0.48% of the game thread recompiled): a
// strand solver of the gamecore module. a0 the strand set (*a0: +4 the
// strands, +8 the points a strand, +0x28 the per-strand limits; a0 +0xc the
// point arrays, 0x18 a strand), a1 the owner (+0x158 its matrix block), a2
// the parameters (+0..+0xc four floats, +0x10 a scale x 0.2, +0x12 / +0x14
// two int16 angles). It
//   1. relaxes each pair of strands from both ends: a point further than the
//      limit from the one before is pulled back to it (vrsqrt of the squared
//      length against the limit, the offset x limit x Q added);
//   2. builds the two anchor frames (iosTransMatrixF of +0 / +4 through the
//      owner's matrices, iosMulRotMatrixXInt16 by +0x12 / +0x14), the
//      anchors' points through them, the frame's inverse (the 3x3 transposed
//      by PEXTLW / PEXTUW / PCPYLD / PCPYUD, -R^T t);
//   3. when the anchor is in front (z >= 0): a direction frame from the two
//      cross products, GetQuaternionByAxisDirZY, its rotation matrix (the
//      same VU0 block as GetMotionNodeBasePos);
//   4. walks the strands again against that frame with the owner's per-point
//      callback (the jalr at 0x13b99fc) and the limits.
//
// Written as the recompiled code's own statements lifted onto host locals
// (r41/gc13b8dc8/lift.py + gen.py, from build/recomp/game/output): the GPRs
// (low halves, and the high halves of those used as 128-bit values), FPRs,
// fcr31 and the VU0 registers / ACC / Q / status live in locals for the whole
// call, so none of the ~1 100 instructions goes through the context; guest
// memory is read and written by the same macros in the same order, the same
// PS2_V* / FPU_* operations. Each call (iosTransMatrixF x2,
// iosMulRotMatrixXInt16 x2, GetQuaternionByAxisDirZY, the callback) stores
// the locals to the context, runs the callee with sotc_decomp::callGuest
// (ra and sp as the original sets them) and loads them back. The frame
// (sp - 0x2b0, the saved registers) is written as the original writes it,
// so the epilogue's loads restore s0-s7, fp, ra and f20-f24 as they were.
// Exact by construction: every register and every byte of memory as the
// recompiled code leaves them; the loops' checkpoints are left out (as in
// every native).
//
// Falls back to the recompiled code with SOTC_MOTION=0 or when one of the
// five relocated jal sites does not name its expected function.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-label"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

namespace decomp_gc13b8dc8
{
    constexpr uint32_t kAddress = 0x013B8DC8u;

    bool sites(const uint8_t *rdram)
    {
        static constexpr uint32_t kSites[5][2] = {{0x013B8FFCu, 0x001B9628u}, {0x013B901Cu, 0x001B9628u}, {0x013B902Cu, 0x001B96D0u},
                                                  {0x013B903Cu, 0x001B96D0u}, {0x013B9294u, 0x001BC308u}};
        for (const auto &s : kSites)
            if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                return false;
        return true;
    }

// The lifted register file.
#define RU32(n) (static_cast<uint32_t>(r##n))
#define RS32(n) (static_cast<int32_t>(static_cast<uint32_t>(r##n)))
#define RU64(n) (r##n)
#define RS64(n) (static_cast<int64_t>(r##n))
#define RVEC(n) (_mm_set_epi64x(static_cast<int64_t>(rh##n), static_cast<int64_t>(r##n)))
#define WS32(n, val) (r##n = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(val))))
#define WU64(n, val) (r##n = static_cast<uint64_t>(val))
#define WVEC(n, val)                                \
    do                                              \
    {                                               \
        const __m128i _wv = (val);                  \
        uint64_t _w[2];                             \
        std::memcpy(_w, &_wv, 16);                  \
        r##n = _w[0];                               \
        rh##n = _w[1];                              \
    } while (0)
#define LIFT_LOAD \
    r1 = GPR_U64(ctx, 1); \
    r2 = GPR_U64(ctx, 2); \
    r3 = GPR_U64(ctx, 3); \
    r4 = GPR_U64(ctx, 4); \
    r5 = GPR_U64(ctx, 5); \
    r6 = GPR_U64(ctx, 6); \
    r7 = GPR_U64(ctx, 7); \
    r8 = GPR_U64(ctx, 8); \
    r9 = GPR_U64(ctx, 9); \
    r10 = GPR_U64(ctx, 10); \
    r11 = GPR_U64(ctx, 11); \
    r12 = GPR_U64(ctx, 12); \
    r13 = GPR_U64(ctx, 13); \
    r14 = GPR_U64(ctx, 14); \
    r15 = GPR_U64(ctx, 15); \
    r16 = GPR_U64(ctx, 16); \
    r17 = GPR_U64(ctx, 17); \
    r18 = GPR_U64(ctx, 18); \
    r19 = GPR_U64(ctx, 19); \
    r20 = GPR_U64(ctx, 20); \
    r21 = GPR_U64(ctx, 21); \
    r22 = GPR_U64(ctx, 22); \
    r23 = GPR_U64(ctx, 23); \
    r25 = GPR_U64(ctx, 25); \
    r29 = GPR_U64(ctx, 29); \
    r30 = GPR_U64(ctx, 30); \
    r31 = GPR_U64(ctx, 31); \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[2], 16); rh2 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[7], 16); rh7 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[8], 16); rh8 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[9], 16); rh9 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[10], 16); rh10 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[11], 16); rh11 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[12], 16); rh12 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[13], 16); rh13 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[14], 16); rh14 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[15], 16); rh15 = w[1]; } \
    { uint64_t w[2]; std::memcpy(w, &ctx->r[25], 16); rh25 = w[1]; } \
    f0 = ctx->f[0]; \
    f1 = ctx->f[1]; \
    f2 = ctx->f[2]; \
    f3 = ctx->f[3]; \
    f4 = ctx->f[4]; \
    f5 = ctx->f[5]; \
    f12 = ctx->f[12]; \
    f13 = ctx->f[13]; \
    f14 = ctx->f[14]; \
    f20 = ctx->f[20]; \
    f21 = ctx->f[21]; \
    f22 = ctx->f[22]; \
    f23 = ctx->f[23]; \
    f24 = ctx->f[24]; \
    v0 = ctx->vu0_vf[0]; \
    v1 = ctx->vu0_vf[1]; \
    v4 = ctx->vu0_vf[4]; \
    v5 = ctx->vu0_vf[5]; \
    v6 = ctx->vu0_vf[6]; \
    v7 = ctx->vu0_vf[7]; \
    v8 = ctx->vu0_vf[8]; \
    v10 = ctx->vu0_vf[10]; \
    v11 = ctx->vu0_vf[11]; \
    v12 = ctx->vu0_vf[12]; \
    v13 = ctx->vu0_vf[13]; \
    v14 = ctx->vu0_vf[14]; \
    v15 = ctx->vu0_vf[15]; \
    v16 = ctx->vu0_vf[16]; \
    v17 = ctx->vu0_vf[17]; \
    v24 = ctx->vu0_vf[24]; \
    v25 = ctx->vu0_vf[25]; \
    v26 = ctx->vu0_vf[26]; \
    v28 = ctx->vu0_vf[28]; \
    v29 = ctx->vu0_vf[29]; \
    acc = ctx->vu0_acc; \
    vq = ctx->vu0_q; \
    vstat = ctx->vu0_status; \
    fcr31 = ctx->fcr31;

#define LIFT_STORE \
    SET_GPR_U64(ctx, 1, r1); \
    { const uint64_t w[2] = {r2, rh2}; std::memcpy(&ctx->r[2], w, 16); } \
    SET_GPR_U64(ctx, 3, r3); \
    SET_GPR_U64(ctx, 4, r4); \
    SET_GPR_U64(ctx, 5, r5); \
    SET_GPR_U64(ctx, 6, r6); \
    { const uint64_t w[2] = {r7, rh7}; std::memcpy(&ctx->r[7], w, 16); } \
    { const uint64_t w[2] = {r8, rh8}; std::memcpy(&ctx->r[8], w, 16); } \
    { const uint64_t w[2] = {r9, rh9}; std::memcpy(&ctx->r[9], w, 16); } \
    { const uint64_t w[2] = {r10, rh10}; std::memcpy(&ctx->r[10], w, 16); } \
    { const uint64_t w[2] = {r11, rh11}; std::memcpy(&ctx->r[11], w, 16); } \
    { const uint64_t w[2] = {r12, rh12}; std::memcpy(&ctx->r[12], w, 16); } \
    { const uint64_t w[2] = {r13, rh13}; std::memcpy(&ctx->r[13], w, 16); } \
    { const uint64_t w[2] = {r14, rh14}; std::memcpy(&ctx->r[14], w, 16); } \
    { const uint64_t w[2] = {r15, rh15}; std::memcpy(&ctx->r[15], w, 16); } \
    SET_GPR_U64(ctx, 16, r16); \
    SET_GPR_U64(ctx, 17, r17); \
    SET_GPR_U64(ctx, 18, r18); \
    SET_GPR_U64(ctx, 19, r19); \
    SET_GPR_U64(ctx, 20, r20); \
    SET_GPR_U64(ctx, 21, r21); \
    SET_GPR_U64(ctx, 22, r22); \
    SET_GPR_U64(ctx, 23, r23); \
    { const uint64_t w[2] = {r25, rh25}; std::memcpy(&ctx->r[25], w, 16); } \
    SET_GPR_U64(ctx, 29, r29); \
    SET_GPR_U64(ctx, 30, r30); \
    SET_GPR_U64(ctx, 31, r31); \
    ctx->f[0] = f0; \
    ctx->f[1] = f1; \
    ctx->f[2] = f2; \
    ctx->f[3] = f3; \
    ctx->f[4] = f4; \
    ctx->f[5] = f5; \
    ctx->f[12] = f12; \
    ctx->f[13] = f13; \
    ctx->f[14] = f14; \
    ctx->f[20] = f20; \
    ctx->f[21] = f21; \
    ctx->f[22] = f22; \
    ctx->f[23] = f23; \
    ctx->f[24] = f24; \
    ctx->vu0_vf[1] = v1; \
    ctx->vu0_vf[4] = v4; \
    ctx->vu0_vf[5] = v5; \
    ctx->vu0_vf[6] = v6; \
    ctx->vu0_vf[7] = v7; \
    ctx->vu0_vf[8] = v8; \
    ctx->vu0_vf[10] = v10; \
    ctx->vu0_vf[11] = v11; \
    ctx->vu0_vf[12] = v12; \
    ctx->vu0_vf[13] = v13; \
    ctx->vu0_vf[14] = v14; \
    ctx->vu0_vf[15] = v15; \
    ctx->vu0_vf[16] = v16; \
    ctx->vu0_vf[17] = v17; \
    ctx->vu0_vf[24] = v24; \
    ctx->vu0_vf[25] = v25; \
    ctx->vu0_vf[26] = v26; \
    ctx->vu0_vf[28] = v28; \
    ctx->vu0_vf[29] = v29; \
    ctx->vu0_acc = acc; \
    ctx->vu0_q = vq; \
    ctx->vu0_status = vstat; \
    ctx->fcr31 = fcr31;

#define LIFT_CALL(target)                                                                                   \
    do                                                                                                     \
    {                                                                                                      \
        LIFT_STORE                                                                                         \
        sotc_decomp::callGuest(rdram, ctx, runtime, (target), static_cast<uint32_t>(r31), static_cast<uint32_t>(r29)); \
        LIFT_LOAD                                                                                          \
    } while (0)

    void strands(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sites(rdram))
            return sotc_decomp::recompiled(kAddress)(rdram, ctx, runtime);
        const uint64_t r0 = 0u, rh0 = 0u; (void)r0; (void)rh0;
        uint64_t r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r25, r29, r30, r31;
        uint64_t rh2, rh7, rh8, rh9, rh10, rh11, rh12, rh13, rh14, rh15, rh25;
        float f0, f1, f2, f3, f4, f5, f12, f13, f14, f20, f21, f22, f23, f24;
        __m128 v0, v1, v4, v5, v6, v7, v8, v10, v11, v12, v13, v14, v15, v16, v17, v24, v25, v26, v28, v29;
        __m128 acc;
        float vq;
        uint16_t vstat;
        uint32_t fcr31;
        LIFT_LOAD
        // 0x13b8dc8: 0x27bdfd50  addiu       $sp, $sp, -0x2B0
        WS32(29, (int32_t)ADD32(RU32(29), 4294966608));
        // 0x13b8dcc: 0x240f0001  addiu       $t7, $zero, 0x1
        WS32(15, (int32_t)ADD32(RU32(0), 1));
        // 0x13b8dd0: 0xffb10238  sd          $s1, 0x238($sp)
        WRITE64(ADD32(RU32(29), 568), RU64(17));
        // 0x13b8dd4: 0xc0882d  daddu       $s1, $a2, $zero
        WU64(17, (uint64_t)RU64(6) + (uint64_t)RU64(0));
        // 0x13b8dd8: 0xffb40250  sd          $s4, 0x250($sp)
        WRITE64(ADD32(RU32(29), 592), RU64(20));
        // 0x13b8ddc: 0x80a02d  daddu       $s4, $a0, $zero
        WU64(20, (uint64_t)RU64(4) + (uint64_t)RU64(0));
        // 0x13b8de0: 0xffb50258  sd          $s5, 0x258($sp)
        WRITE64(ADD32(RU32(29), 600), RU64(21));
        // 0x13b8de4: 0xffbe0270  sd          $fp, 0x270($sp)
        WRITE64(ADD32(RU32(29), 624), RU64(30));
        // 0x13b8de8: 0xa0f02d  daddu       $fp, $a1, $zero
        WU64(30, (uint64_t)RU64(5) + (uint64_t)RU64(0));
        // 0x13b8dec: 0xffb00230  sd          $s0, 0x230($sp)
        WRITE64(ADD32(RU32(29), 560), RU64(16));
        // 0x13b8df0: 0xffb20240  sd          $s2, 0x240($sp)
        WRITE64(ADD32(RU32(29), 576), RU64(18));
        // 0x13b8df4: 0xffb30248  sd          $s3, 0x248($sp)
        WRITE64(ADD32(RU32(29), 584), RU64(19));
        // 0x13b8df8: 0xffb60260  sd          $s6, 0x260($sp)
        WRITE64(ADD32(RU32(29), 608), RU64(22));
        // 0x13b8dfc: 0xffb70268  sd          $s7, 0x268($sp)
        WRITE64(ADD32(RU32(29), 616), RU64(23));
        // 0x13b8e00: 0xffbf0278  sd          $ra, 0x278($sp)
        WRITE64(ADD32(RU32(29), 632), RU64(31));
        // 0x13b8e04: 0xe7b802a0  swc1        $f24, 0x2A0($sp)
        { float f = f24; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 672), bits); }
        // 0x13b8e08: 0xe7b70298  swc1        $f23, 0x298($sp)
        { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 664), bits); }
        // 0x13b8e0c: 0xe7b60290  swc1        $f22, 0x290($sp)
        { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 656), bits); }
        // 0x13b8e10: 0xe7b50288  swc1        $f21, 0x288($sp)
        { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 648), bits); }
        // 0x13b8e14: 0xe7b40280  swc1        $f20, 0x280($sp)
        { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 640), bits); }
        // 0x13b8e18: 0x8e920000  lw          $s2, 0x0($s4)
        WS32(18, (int32_t)READ32(ADD32(RU32(20), 0)));
        // 0x13b8e1c: 0x8e84000c  lw          $a0, 0xC($s4)
        WS32(4, (int32_t)READ32(ADD32(RU32(20), 12)));
        // 0x13b8e20: 0x8e450004  lw          $a1, 0x4($s2)
        WS32(5, (int32_t)READ32(ADD32(RU32(18), 4)));
        // 0x13b8e24: 0x8e420008  lw          $v0, 0x8($s2)
        WS32(2, (int32_t)READ32(ADD32(RU32(18), 8)));
        // 0x13b8e28: 0x1e5182a  slt         $v1, $t7, $a1
        WU64(3, ((int64_t)RS64(15) < (int64_t)RS64(5)) ? 1 : 0);
        // 0x13b8e2c: 0x10600062  beqz        $v1, . + 4 + (0x62 << 2)
        {
            const bool branch_taken_0x13b8e2c = (RU64(3) == RU64(0));
            // 0x13b8e30: 0x2455ffff  addiu       $s5, $v0, -0x1 (Delay Slot)
            WS32(21, (int32_t)ADD32(RU32(2), 4294967295));
            if (branch_taken_0x13b8e2c) {
                goto label_13b8fb8;
            }
        }
        // 0x13b8e34: 0x51040  sll         $v0, $a1, 1
        WS32(2, (int32_t)SLL32(RU32(5), 1));
        // 0x13b8e38: 0x518c0  sll         $v1, $a1, 3
        WS32(3, (int32_t)SLL32(RU32(5), 3));
        // 0x13b8e3c: 0x451021  addu        $v0, $v0, $a1
        WS32(2, (int32_t)ADD32(RU32(2), RU32(5)));
        // 0x13b8e40: 0x651821  addu        $v1, $v1, $a1
        WS32(3, (int32_t)ADD32(RU32(3), RU32(5)));
        // 0x13b8e44: 0x210c0  sll         $v0, $v0, 3
        WS32(2, (int32_t)SLL32(RU32(2), 3));
        // 0x13b8e48: 0x31880  sll         $v1, $v1, 2
        WS32(3, (int32_t)SLL32(RU32(3), 2));
        // 0x13b8e4c: 0x441021  addu        $v0, $v0, $a0
        WS32(2, (int32_t)ADD32(RU32(2), RU32(4)));
        // 0x13b8e50: 0x246effb8  addiu       $t6, $v1, -0x48
        WS32(14, (int32_t)ADD32(RU32(3), 4294967224));
        // 0x13b8e54: 0x244dffe0  addiu       $t5, $v0, -0x20
        WS32(13, (int32_t)ADD32(RU32(2), 4294967264));
        // 0x13b8e58: 0x248c0028  addiu       $t4, $a0, 0x28
        WS32(12, (int32_t)ADD32(RU32(4), 40));
        // 0x13b8e5c: 0x24afffff  addiu       $t7, $a1, -0x1
        WS32(15, (int32_t)ADD32(RU32(5), 4294967295));
        // 0x13b8e60: 0x27b70010  addiu       $s7, $sp, 0x10
        WS32(23, (int32_t)ADD32(RU32(29), 16));
        // 0x13b8e64: 0x2416ffff  addiu       $s6, $zero, -0x1
        WS32(22, (int32_t)ADD32(RU32(0), 4294967295));
        // 0x13b8e68: 0x982d  daddu       $s3, $zero, $zero
        WU64(19, (uint64_t)RU64(0) + (uint64_t)RU64(0));
        // 0x13b8e6c: 0x0  nop
        // NOP
    label_13b8e70:
        // 0x13b8e70: 0x8e420028  lw          $v0, 0x28($s2)
        WS32(2, (int32_t)READ32(ADD32(RU32(18), 40)));
        // 0x13b8e74: 0x26abffff  addiu       $t3, $s5, -0x1
        WS32(11, (int32_t)ADD32(RU32(21), 4294967295));
        // 0x13b8e78: 0x8d83ffe8  lw          $v1, -0x18($t4)
        WS32(3, (int32_t)READ32(ADD32(RU32(12), 4294967272)));
        // 0x13b8e7c: 0x1c22021  addu        $a0, $t6, $v0
        WS32(4, (int32_t)ADD32(RU32(14), RU32(2)));
        // 0x13b8e80: 0x2621021  addu        $v0, $s3, $v0
        WS32(2, (int32_t)ADD32(RU32(19), RU32(2)));
        // 0x13b8e84: 0x8c460000  lw          $a2, 0x0($v0)
        WS32(6, (int32_t)READ32(ADD32(RU32(2), 0)));
        // 0x13b8e88: 0x24690010  addiu       $t1, $v1, 0x10
        WS32(9, (int32_t)ADD32(RU32(3), 16));
        // 0x13b8e8c: 0x8c850000  lw          $a1, 0x0($a0)
        WS32(5, (int32_t)READ32(ADD32(RU32(4), 0)));
        // 0x13b8e90: 0x8d820000  lw          $v0, 0x0($t4)
        WS32(2, (int32_t)READ32(ADD32(RU32(12), 0)));
        // 0x13b8e94: 0x8da30018  lw          $v1, 0x18($t5)
        WS32(3, (int32_t)READ32(ADD32(RU32(13), 24)));
        // 0x13b8e98: 0x8da40000  lw          $a0, 0x0($t5)
        WS32(4, (int32_t)READ32(ADD32(RU32(13), 0)));
        // 0x13b8e9c: 0x24470010  addiu       $a3, $v0, 0x10
        WS32(7, (int32_t)ADD32(RU32(2), 16));
        // 0x13b8ea0: 0x24630010  addiu       $v1, $v1, 0x10
        WS32(3, (int32_t)ADD32(RU32(3), 16));
        // 0x13b8ea4: 0xc4c40004  lwc1        $f4, 0x4($a2)
        { uint32_t bits = READ32(ADD32(RU32(6), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
        // 0x13b8ea8: 0x24840010  addiu       $a0, $a0, 0x10
        WS32(4, (int32_t)ADD32(RU32(4), 16));
        // 0x13b8eac: 0xc4a30004  lwc1        $f3, 0x4($a1)
        { uint32_t bits = READ32(ADD32(RU32(5), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
        // 0x13b8eb0: 0xc4c20000  lwc1        $f2, 0x0($a2)
        { uint32_t bits = READ32(ADD32(RU32(6), 0)); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
        // 0x13b8eb4: 0x11760038  beq         $t3, $s6, . + 4 + (0x38 << 2)
        {
            const bool branch_taken_0x13b8eb4 = (RU64(11) == RU64(22));
            // 0x13b8eb8: 0xc4a10000  lwc1        $f1, 0x0($a1) (Delay Slot)
            { uint32_t bits = READ32(ADD32(RU32(5), 0)); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            if (branch_taken_0x13b8eb4) {
                goto label_13b8f98;
            }
        }
        // 0x13b8ebc: 0x2e0502d  daddu       $t2, $s7, $zero
        WU64(10, (uint64_t)RU64(23) + (uint64_t)RU64(0));
        // 0x13b8ec0: 0x2410ffff  addiu       $s0, $zero, -0x1
        WS32(16, (int32_t)ADD32(RU32(0), 4294967295));
        // 0x13b8ec4: 0x0  nop
        // NOP
    label_13b8ec8:
        // 0x13b8ec8: 0xe0282d  daddu       $a1, $a3, $zero
        WU64(5, (uint64_t)RU64(7) + (uint64_t)RU64(0));
        // 0x13b8ecc: 0x80302d  daddu       $a2, $a0, $zero
        WU64(6, (uint64_t)RU64(4) + (uint64_t)RU64(0));
        // 0x13b8ed0: 0xd8e50000  lqc2        $vf5, 0x0($a3)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b8ed4: 0xd9260000  lqc2        $vf6, 0x0($t1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 0)));
        // 0x13b8ed8: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b8edc: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8ee0: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8ee4: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8ee8: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b8eec: 0xfba80000  sqc2        $vf8, 0x0($sp)
        WRITE128(ADD32(RU32(29), 0), _mm_castps_si128(v8));
        // 0x13b8ef0: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b8ef4: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b8ef8: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b8efc: 0x46040034  c.lt.s      $f0, $f4
        fcr31 = (FPU_C_OLT_S(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b8f00: 0x45000009  bc1f        . + 4 + (0x9 << 2)
        {
            const bool branch_taken_0x13b8f00 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b8f00) {
                goto label_13b8f28;
            }
        }
        // 0x13b8f08: 0x46001002  mul.s       $f0, $f2, $f0
        f0 = FPU_MUL_S(f2, f0);
        // 0x13b8f0c: 0xd9270000  lqc2        $vf7, 0x0($t1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 0)));
        // 0x13b8f10: 0xdba50000  lqc2        $vf5, 0x0($sp)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(29), 0)));
        // 0x13b8f14: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b8f18: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b8f1c: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f20: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f24: 0xf8a70000  sqc2        $vf7, 0x0($a1)
        WRITE128(ADD32(RU32(5), 0), _mm_castps_si128(v7));
    label_13b8f28:
        // 0x13b8f28: 0xd8c50000  lqc2        $vf5, 0x0($a2)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b8f2c: 0xd8660000  lqc2        $vf6, 0x0($v1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b8f30: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f34: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f38: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f3c: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f40: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b8f44: 0xf9480000  sqc2        $vf8, 0x0($t2)
        WRITE128(ADD32(RU32(10), 0), _mm_castps_si128(v8));
        // 0x13b8f48: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b8f4c: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b8f50: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b8f54: 0x46030034  c.lt.s      $f0, $f3
        fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b8f58: 0x4502000a  bc1fl       . + 4 + (0xA << 2)
        {
            const bool branch_taken_0x13b8f58 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b8f58) {
                // 0x13b8f5c: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b8f84;
            }
        }
        // 0x13b8f60: 0x46000802  mul.s       $f0, $f1, $f0
        f0 = FPU_MUL_S(f1, f0);
        // 0x13b8f64: 0xd8670000  lqc2        $vf7, 0x0($v1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b8f68: 0xd9450000  lqc2        $vf5, 0x0($t2)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 0)));
        // 0x13b8f6c: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b8f70: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b8f74: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f78: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b8f7c: 0xf8c70000  sqc2        $vf7, 0x0($a2)
        WRITE128(ADD32(RU32(6), 0), _mm_castps_si128(v7));
        // 0x13b8f80: 0x256bffff  addiu       $t3, $t3, -0x1
        WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
    label_13b8f84:
        // 0x13b8f84: 0x25290010  addiu       $t1, $t1, 0x10
        WS32(9, (int32_t)ADD32(RU32(9), 16));
        // 0x13b8f88: 0x24e70010  addiu       $a3, $a3, 0x10
        WS32(7, (int32_t)ADD32(RU32(7), 16));
        // 0x13b8f8c: 0x24630010  addiu       $v1, $v1, 0x10
        WS32(3, (int32_t)ADD32(RU32(3), 16));
        // 0x13b8f90: 0x1570ffcd  bne         $t3, $s0, . + 4 + (-0x33 << 2)
        {
            const bool branch_taken_0x13b8f90 = (RU64(11) != RU64(16));
            // 0x13b8f94: 0x24840010  addiu       $a0, $a0, 0x10 (Delay Slot)
            WS32(4, (int32_t)ADD32(RU32(4), 16));
            if (branch_taken_0x13b8f90) {
                goto label_13b8ec8;
            }
        }
    label_13b8f98:
        // 0x13b8f98: 0x25efffff  addiu       $t7, $t7, -0x1
        WS32(15, (int32_t)ADD32(RU32(15), 4294967295));
        // 0x13b8f9c: 0x25ceffdc  addiu       $t6, $t6, -0x24
        WS32(14, (int32_t)ADD32(RU32(14), 4294967260));
        // 0x13b8fa0: 0x26730024  addiu       $s3, $s3, 0x24
        WS32(19, (int32_t)ADD32(RU32(19), 36));
        // 0x13b8fa4: 0x25adffe8  addiu       $t5, $t5, -0x18
        WS32(13, (int32_t)ADD32(RU32(13), 4294967272));
        // 0x13b8fa8: 0x15e0ffb1  bnez        $t7, . + 4 + (-0x4F << 2)
        {
            const bool branch_taken_0x13b8fa8 = (RU64(15) != RU64(0));
            // 0x13b8fac: 0x258c0018  addiu       $t4, $t4, 0x18 (Delay Slot)
            WS32(12, (int32_t)ADD32(RU32(12), 24));
            if (branch_taken_0x13b8fa8) {
                goto label_13b8e70;
            }
        }
        // 0x13b8fb0: 0x10000003  b           . + 4 + (0x3 << 2)
        {
            const bool branch_taken_0x13b8fb0 = (RU64(0) == RU64(0));
            // 0x13b8fb4: 0x86220010  lh          $v0, 0x10($s1) (Delay Slot)
            WS32(2, (int16_t)READ16(ADD32(RU32(17), 16)));
            if (branch_taken_0x13b8fb0) {
                goto label_13b8fc0;
            }
        }
    label_13b8fb8:
        // 0x13b8fb8: 0x27b70010  addiu       $s7, $sp, 0x10
        WS32(23, (int32_t)ADD32(RU32(29), 16));
        // 0x13b8fbc: 0x86220010  lh          $v0, 0x10($s1)
        WS32(2, (int16_t)READ16(ADD32(RU32(17), 16)));
    label_13b8fc0:
        // 0x13b8fc0: 0x27b600a0  addiu       $s6, $sp, 0xA0
        WS32(22, (int32_t)ADD32(RU32(29), 160));
        // 0x13b8fc4: 0x4480c000  mtc1        $zero, $f24
        { uint32_t bits = RU32(0); std::memcpy(&f24, &bits, sizeof(bits)); }
        // 0x13b8fc8: 0x2c0202d  daddu       $a0, $s6, $zero
        WU64(4, (uint64_t)RU64(22) + (uint64_t)RU64(0));
        // 0x13b8fcc: 0x44820800  mtc1        $v0, $f1
        { uint32_t bits = RU32(2); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b8fd0: 0x46800860  cvt.s.w     $f1, $f1
        { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
        // 0x13b8fd4: 0x8fc50158  lw          $a1, 0x158($fp)
        WS32(5, (int32_t)READ32(ADD32(RU32(30), 344)));
        // 0x13b8fd8: 0x3c013e4c  lui         $at, 0x3E4C
        WS32(1, (int32_t)((uint32_t)15948 << 16));
        // 0x13b8fdc: 0x3421cccc  ori         $at, $at, 0xCCCC
        WU64(1, RU64(1) | (uint64_t)(uint16_t)52428);
        // 0x13b8fe0: 0x44810000  mtc1        $at, $f0
        { uint32_t bits = RU32(1); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b8fe4: 0x4600c306  mov.s       $f12, $f24
        f12 = FPU_MOV_S(f24);
        // 0x13b8fe8: 0xc62e0000  lwc1        $f14, 0x0($s1)
        { uint32_t bits = READ32(ADD32(RU32(17), 0)); float f; std::memcpy(&f, &bits, sizeof(f)); f14 = f; }
        // 0x13b8fec: 0x4600c346  mov.s       $f13, $f24
        f13 = FPU_MOV_S(f24);
        // 0x13b8ff0: 0x24a50280  addiu       $a1, $a1, 0x280
        WS32(5, (int32_t)ADD32(RU32(5), 640));
        // 0x13b8ff4: 0xc6350008  lwc1        $f21, 0x8($s1)
        { uint32_t bits = READ32(ADD32(RU32(17), 8)); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
        // 0x13b8ff8: 0x46000d02  mul.s       $f20, $f1, $f0
        f20 = FPU_MUL_S(f1, f0);
        // 0x13b8ffc: 0xc06e58a  jal         func_1B9628
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x13B8FFCu) & 0x3FFFFFFu) << 2);
            WS32(31, 0x13B9004u);
            // 0x13b9000: 0xc636000c  lwc1        $f22, 0xC($s1) (Delay Slot)
            { uint32_t bits = READ32(ADD32(RU32(17), 12)); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
            LIFT_CALL(jumpTarget);
        }
    label_13b9004:
        // 0x13b9004: 0x8fc50158  lw          $a1, 0x158($fp)
        WS32(5, (int32_t)READ32(ADD32(RU32(30), 344)));
        // 0x13b9008: 0x27b500e0  addiu       $s5, $sp, 0xE0
        WS32(21, (int32_t)ADD32(RU32(29), 224));
        // 0x13b900c: 0xc62e0004  lwc1        $f14, 0x4($s1)
        { uint32_t bits = READ32(ADD32(RU32(17), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); f14 = f; }
        // 0x13b9010: 0x4600c306  mov.s       $f12, $f24
        f12 = FPU_MOV_S(f24);
        // 0x13b9014: 0x4600c346  mov.s       $f13, $f24
        f13 = FPU_MOV_S(f24);
        // 0x13b9018: 0x2a0202d  daddu       $a0, $s5, $zero
        WU64(4, (uint64_t)RU64(21) + (uint64_t)RU64(0));
        // 0x13b901c: 0xc06e58a  jal         func_1B9628
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x13B901Cu) & 0x3FFFFFFu) << 2);
            WS32(31, 0x13B9024u);
            // 0x13b9020: 0x3c130146  lui         $s3, 0x146 (Delay Slot)
            WS32(19, (int32_t)((uint32_t)326 << 16));
            LIFT_CALL(jumpTarget);
        }
    label_13b9024:
        // 0x13b9024: 0x2c0202d  daddu       $a0, $s6, $zero
        WU64(4, (uint64_t)RU64(22) + (uint64_t)RU64(0));
        // 0x13b9028: 0x86260012  lh          $a2, 0x12($s1)
        WS32(6, (int16_t)READ16(ADD32(RU32(17), 18)));
        // 0x13b902c: 0xc06e5b4  jal         func_1B96D0
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x13B902Cu) & 0x3FFFFFFu) << 2);
            WS32(31, 0x13B9034u);
            // 0x13b9030: 0x2c0282d  daddu       $a1, $s6, $zero (Delay Slot)
            WU64(5, (uint64_t)RU64(22) + (uint64_t)RU64(0));
            LIFT_CALL(jumpTarget);
        }
    label_13b9034:
        // 0x13b9034: 0x2a0202d  daddu       $a0, $s5, $zero
        WU64(4, (uint64_t)RU64(21) + (uint64_t)RU64(0));
        // 0x13b9038: 0x86260014  lh          $a2, 0x14($s1)
        WS32(6, (int16_t)READ16(ADD32(RU32(17), 20)));
        // 0x13b903c: 0xc06e5b4  jal         func_1B96D0
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x13B903Cu) & 0x3FFFFFFu) << 2);
            WS32(31, 0x13B9044u);
            // 0x13b9040: 0x2a0282d  daddu       $a1, $s5, $zero (Delay Slot)
            WU64(5, (uint64_t)RU64(21) + (uint64_t)RU64(0));
            LIFT_CALL(jumpTarget);
        }
    label_13b9044:
        // 0x13b9044: 0x266269f0  addiu       $v0, $s3, 0x69F0
        WS32(2, (int32_t)ADD32(RU32(19), 27120));
        // 0x13b9048: 0xe4550004  swc1        $f21, 0x4($v0)
        { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(2), 4), bits); }
        // 0x13b904c: 0xd8480000  lqc2        $vf8, 0x0($v0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b9050: 0xdace0000  lqc2        $vf14, 0x0($s6)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 0)));
        // 0x13b9054: 0xdacf0010  lqc2        $vf15, 0x10($s6)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 16)));
        // 0x13b9058: 0xdad00020  lqc2        $vf16, 0x20($s6)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 32)));
        // 0x13b905c: 0xdad10030  lqc2        $vf17, 0x30($s6)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b9060: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9064: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9068: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b906c: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9070: 0xfaea0000  sqc2        $vf10, 0x0($s7)
        WRITE128(ADD32(RU32(23), 0), _mm_castps_si128(v10));
        // 0x13b9074: 0xe4560004  swc1        $f22, 0x4($v0)
        { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(2), 4), bits); }
        // 0x13b9078: 0xd8480000  lqc2        $vf8, 0x0($v0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b907c: 0xdaae0000  lqc2        $vf14, 0x0($s5)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 0)));
        // 0x13b9080: 0xdaaf0010  lqc2        $vf15, 0x10($s5)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 16)));
        // 0x13b9084: 0xdab00020  lqc2        $vf16, 0x20($s5)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 32)));
        // 0x13b9088: 0xdab10030  lqc2        $vf17, 0x30($s5)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b908c: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9090: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9094: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9098: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b909c: 0xfbaa0000  sqc2        $vf10, 0x0($sp)
        WRITE128(ADD32(RU32(29), 0), _mm_castps_si128(v10));
        // 0x13b90a0: 0x27b20120  addiu       $s2, $sp, 0x120
        WS32(18, (int32_t)ADD32(RU32(29), 288));
        // 0x13b90a4: 0xdad80030  lqc2        $vf24, 0x30($s6)
        v24 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b90a8: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
        { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v25 = _mm_blendv_ps(v25, res, _mm_castsi128_ps(mask)); }
        // 0x13b90ac: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
        { __m128 res = ps2VuMulK<0, 1>(v24, _mm_shuffle_ps(v25, v25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v28 = _mm_blendv_ps(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b90b0: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
        { __m128 res = PS2_VSUB(v28, v28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v28 = PS2_VBLEND(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b90b4: 0x7ac80000  lq          $t0, 0x0($s6)
        WVEC(8, READ128(ADD32(RU32(22), 0)));
        // 0x13b90b8: 0x7ac90010  lq          $t1, 0x10($s6)
        WVEC(9, READ128(ADD32(RU32(22), 16)));
        // 0x13b90bc: 0x7aca0020  lq          $t2, 0x20($s6)
        WVEC(10, READ128(ADD32(RU32(22), 32)));
        // 0x13b90c0: 0x482b0000  qmfc2.ni    $t3, $vf0
        WVEC(11, _mm_castps_si128(v0));
        // 0x13b90c4: 0x71286488  pextlw      $t4, $t1, $t0
        WVEC(12, PS2_PEXTLW(RVEC(9), RVEC(8)));
        // 0x13b90c8: 0x71286ca8  pextuw      $t5, $t1, $t0
        WVEC(13, PS2_PEXTUW(RVEC(9), RVEC(8)));
        // 0x13b90cc: 0x716a7488  pextlw      $t6, $t3, $t2
        WVEC(14, PS2_PEXTLW(RVEC(11), RVEC(10)));
        // 0x13b90d0: 0x716a7ca8  pextuw      $t7, $t3, $t2
        WVEC(15, PS2_PEXTUW(RVEC(11), RVEC(10)));
        // 0x13b90d4: 0x71cc4389  pcpyld      $t0, $t6, $t4
        WVEC(8, PS2_PCPYLD(RVEC(14), RVEC(12)));
        // 0x13b90d8: 0x718e4ba9  pcpyud      $t1, $t4, $t6
        WVEC(9, _mm_unpackhi_epi64(RVEC(12), RVEC(14)));
        // 0x13b90dc: 0x71ed5389  pcpyld      $t2, $t7, $t5
        WVEC(10, PS2_PCPYLD(RVEC(15), RVEC(13)));
        // 0x13b90e0: 0x71af5ba9  pcpyud      $t3, $t5, $t7
        WVEC(11, _mm_unpackhi_epi64(RVEC(13), RVEC(15)));
        // 0x13b90e4: 0x48a8c000  qmtc2.ni    $t0, $vf24
        v24 = _mm_castsi128_ps(RVEC(8));
        // 0x13b90e8: 0x48a9c800  qmtc2.ni    $t1, $vf25
        v25 = _mm_castsi128_ps(RVEC(9));
        // 0x13b90ec: 0x48aad000  qmtc2.ni    $t2, $vf26
        v26 = _mm_castsi128_ps(RVEC(10));
        // 0x13b90f0: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
        { __m128 res = PS2_VMUL(v24, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b90f4: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
        { __m128 mul_res = PS2_VMUL(v25, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b90f8: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
        { __m128 mul_res = PS2_VMUL(v26, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b90fc: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
        { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v29 = _mm_blendv_ps(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9100: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
        { __m128 res = ps2VuSubK<1, 1>(v29, v29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9104: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
        { __m128 res = ps2VuAddK<1, 0>(v29, v0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9108: 0xfa580000  sqc2        $vf24, 0x0($s2)
        WRITE128(ADD32(RU32(18), 0), _mm_castps_si128(v24));
        // 0x13b910c: 0xfa590010  sqc2        $vf25, 0x10($s2)
        WRITE128(ADD32(RU32(18), 16), _mm_castps_si128(v25));
        // 0x13b9110: 0xfa5a0020  sqc2        $vf26, 0x20($s2)
        WRITE128(ADD32(RU32(18), 32), _mm_castps_si128(v26));
        // 0x13b9114: 0xfa5d0030  sqc2        $vf29, 0x30($s2)
        WRITE128(ADD32(RU32(18), 48), _mm_castps_si128(v29));
        // 0x13b9118: 0x27a20160  addiu       $v0, $sp, 0x160
        WS32(2, (int32_t)ADD32(RU32(29), 352));
        // 0x13b911c: 0xdba80000  lqc2        $vf8, 0x0($sp)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(29), 0)));
        // 0x13b9120: 0xda4e0000  lqc2        $vf14, 0x0($s2)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 0)));
        // 0x13b9124: 0xda4f0010  lqc2        $vf15, 0x10($s2)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 16)));
        // 0x13b9128: 0xda500020  lqc2        $vf16, 0x20($s2)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 32)));
        // 0x13b912c: 0xda510030  lqc2        $vf17, 0x30($s2)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 48)));
        // 0x13b9130: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9134: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9138: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b913c: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9140: 0xf84a0000  sqc2        $vf10, 0x0($v0)
        WRITE128(ADD32(RU32(2), 0), _mm_castps_si128(v10));
        // 0x13b9144: 0xc7a00168  lwc1        $f0, 0x168($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 360)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b9148: 0x46180034  c.lt.s      $f0, $f24
        fcr31 = (FPU_C_OLT_S(f0, f24)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b914c: 0x45010083  bc1t        . + 4 + (0x83 << 2)
        {
            const bool branch_taken_0x13b914c = ((fcr31 & 0x800000));
            // 0x13b9150: 0x27b90180  addiu       $t9, $sp, 0x180 (Delay Slot)
            WS32(25, (int32_t)ADD32(RU32(29), 384));
            if (branch_taken_0x13b914c) {
                goto label_13b935c;
            }
        }
        // 0x13b9154: 0x3c02001e  lui         $v0, 0x1E
        { const uint32_t _dynInsn = READ32(0x13B9154u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; WS32(2, (int32_t)((uint32_t)_dynImm << 16)); }
        // 0x13b9158: 0x27a40190  addiu       $a0, $sp, 0x190
        WS32(4, (int32_t)ADD32(RU32(29), 400));
        // 0x13b915c: 0x24420ff0  addiu       $v0, $v0, 0xFF0
        { const uint32_t _dynInsn = READ32(0x13B915Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; WS32(2, (int32_t)ADD32(RU32(2), _dynSimm)); }
        // 0x13b9160: 0xd8480000  lqc2        $vf8, 0x0($v0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b9164: 0xdaae0000  lqc2        $vf14, 0x0($s5)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 0)));
        // 0x13b9168: 0xdaaf0010  lqc2        $vf15, 0x10($s5)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 16)));
        // 0x13b916c: 0xdab00020  lqc2        $vf16, 0x20($s5)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 32)));
        // 0x13b9170: 0xdab10030  lqc2        $vf17, 0x30($s5)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b9174: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9178: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b917c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9180: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9184: 0xf88a0000  sqc2        $vf10, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v10));
        // 0x13b9188: 0x3c03001e  lui         $v1, 0x1E
        { const uint32_t _dynInsn = READ32(0x13B9188u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; WS32(3, (int32_t)((uint32_t)_dynImm << 16)); }
        // 0x13b918c: 0x27a201a0  addiu       $v0, $sp, 0x1A0
        WS32(2, (int32_t)ADD32(RU32(29), 416));
        // 0x13b9190: 0x24630fa0  addiu       $v1, $v1, 0xFA0
        { const uint32_t _dynInsn = READ32(0x13B9190u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; WS32(3, (int32_t)ADD32(RU32(3), _dynSimm)); }
        // 0x13b9194: 0xd8680000  lqc2        $vf8, 0x0($v1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9198: 0xdace0000  lqc2        $vf14, 0x0($s6)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 0)));
        // 0x13b919c: 0xdacf0010  lqc2        $vf15, 0x10($s6)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 16)));
        // 0x13b91a0: 0xdad00020  lqc2        $vf16, 0x20($s6)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 32)));
        // 0x13b91a4: 0xdad10030  lqc2        $vf17, 0x30($s6)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b91a8: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b91ac: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b91b0: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b91b4: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b91b8: 0xf84a0000  sqc2        $vf10, 0x0($v0)
        WRITE128(ADD32(RU32(2), 0), _mm_castps_si128(v10));
        // 0x13b91bc: 0xd8850000  lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b91c0: 0xd8460000  lqc2        $vf6, 0x0($v0)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b91c4: 0x4bc62968  vadd.xyz    $vf5, $vf5, $vf6
        { __m128 res = PS2_VADD(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b91c8: 0xf8850000  sqc2        $vf5, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v5));
        // 0x13b91cc: 0x27a50170  addiu       $a1, $sp, 0x170
        WS32(5, (int32_t)ADD32(RU32(29), 368));
        // 0x13b91d0: 0xdae50000  lqc2        $vf5, 0x0($s7)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(23), 0)));
        // 0x13b91d4: 0xdba60000  lqc2        $vf6, 0x0($sp)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(29), 0)));
        // 0x13b91d8: 0x4bc6296c  vsub.xyz    $vf5, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b91dc: 0xf8a50000  sqc2        $vf5, 0x0($a1)
        WRITE128(ADD32(RU32(5), 0), _mm_castps_si128(v5));
        // 0x13b91e0: 0x27b90180  addiu       $t9, $sp, 0x180
        WS32(25, (int32_t)ADD32(RU32(29), 384));
        // 0x13b91e4: 0xd8850000  lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b91e8: 0xd8a60000  lqc2        $vf6, 0x0($a1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b91ec: 0x4bc62afe  vopmula.xyz $ACC, $vf5, $vf6
        { __m128 fs_yzx = _mm_shuffle_ps(v5, v5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b91f0: 0x4bc531ee  vopmsub.xyz $vf7, $vf6, $vf5
        { __m128 fs_yzx = _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v5, v5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b91f4: 0x4a2739ec  vsub.w      $vf7, $vf7, $vf7
        { __m128 res = PS2_VSUB(v7, v7); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b91f8: 0xfb270000  sqc2        $vf7, 0x0($t9)
        WRITE128(ADD32(RU32(25), 0), _mm_castps_si128(v7));
        // 0x13b91fc: 0xd8850000  lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9200: 0xdb260000  lqc2        $vf6, 0x0($t9)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 0)));
        // 0x13b9204: 0x4bc62afe  vopmula.xyz $ACC, $vf5, $vf6
        { __m128 fs_yzx = _mm_shuffle_ps(v5, v5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b9208: 0x4bc531ee  vopmsub.xyz $vf7, $vf6, $vf5
        { __m128 fs_yzx = _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v5, v5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b920c: 0x4a2739ec  vsub.w      $vf7, $vf7, $vf7
        { __m128 res = PS2_VSUB(v7, v7); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9210: 0xf8a70000  sqc2        $vf7, 0x0($a1)
        WRITE128(ADD32(RU32(5), 0), _mm_castps_si128(v7));
        // 0x13b9214: 0xd8a50000  lqc2        $vf5, 0x0($a1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9218: 0x4bc529ea  vmul.xyz    $vf7, $vf5, $vf5
        { __m128 res = PS2_VMUL(v5, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b921c: 0x4a2701bc  vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9220: 0x4a2700bd  vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9224: 0x4a2701ca  vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9228: 0x4be703be  vrsqrt      $Q, $vf0w, $vf7w
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b922c: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9230: 0x4bc0295c  vmulq.xyz   $vf5, $vf5, $Q
        { __m128 res = PS2_VMUL(v5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9234: 0xf8a50000  sqc2        $vf5, 0x0($a1)
        WRITE128(ADD32(RU32(5), 0), _mm_castps_si128(v5));
        // 0x13b9238: 0xdb250000  lqc2        $vf5, 0x0($t9)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 0)));
        // 0x13b923c: 0x4bc529ea  vmul.xyz    $vf7, $vf5, $vf5
        { __m128 res = PS2_VMUL(v5, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9240: 0x4a2701bc  vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9244: 0x4a2700bd  vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9248: 0x4a2701ca  vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b924c: 0x4be703be  vrsqrt      $Q, $vf0w, $vf7w
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9250: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9254: 0x4bc0295c  vmulq.xyz   $vf5, $vf5, $Q
        { __m128 res = PS2_VMUL(v5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9258: 0xfb250000  sqc2        $vf5, 0x0($t9)
        WRITE128(ADD32(RU32(25), 0), _mm_castps_si128(v5));
        // 0x13b925c: 0xdba50000  lqc2        $vf5, 0x0($sp)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(29), 0)));
        // 0x13b9260: 0xdae60000  lqc2        $vf6, 0x0($s7)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(23), 0)));
        // 0x13b9264: 0x4be629ec  vsub.xyzw   $vf7, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9268: 0x4bc739ea  vmul.xyz    $vf7, $vf7, $vf7
        { __m128 res = ps2VuMulK<1, 1>(v7, v7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b926c: 0x4a2701bc  vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9270: 0x4a2700bd  vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // 0x13b9274: 0x4a2701ca  vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<0, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9278: 0x4ba703bd  .word       0x4BA703BD                   # vsqrt       $Q, $vf7w # 00200000 <InstrIdType: R5900_COP2_SPECIAL2>
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
        // 0x13b927c: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9280: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b9284: 0x4482c000  mtc1        $v0, $f24
        { uint32_t bits = RU32(2); std::memcpy(&f24, &bits, sizeof(bits)); }
        // 0x13b9288: 0x27b001b0  addiu       $s0, $sp, 0x1B0
        WS32(16, (int32_t)ADD32(RU32(29), 432));
        // 0x13b928c: 0x320302d  daddu       $a2, $t9, $zero
        WU64(6, (uint64_t)RU64(25) + (uint64_t)RU64(0));
        // 0x13b9290: 0x200202d  daddu       $a0, $s0, $zero
        WU64(4, (uint64_t)RU64(16) + (uint64_t)RU64(0));
        // 0x13b9294: 0xc06f0c2  jal         func_1BC308
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x13B9294u) & 0x3FFFFFFu) << 2);
            WS32(31, 0x13B929Cu);
            // 0x13b9298: 0x7fb90220  sq          $t9, 0x220($sp) (Delay Slot)
            WRITE128(ADD32(RU32(29), 544), RVEC(25));
            LIFT_CALL(jumpTarget);
        }
    label_13b929c:
        // 0x13b929c: 0x2e0182d  daddu       $v1, $s7, $zero
        WU64(3, (uint64_t)RU64(23) + (uint64_t)RU64(0));
        // 0x13b92a0: 0x27a20020  addiu       $v0, $sp, 0x20
        WS32(2, (int32_t)ADD32(RU32(29), 32));
        // 0x13b92a4: 0x4bc00303  vaddw.xyz   $vf12, $vf0, $vf0w
        { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
        // 0x13b92a8: 0xda0b0000  lqc2        $vf11, 0x0($s0)
        v11 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b92ac: 0x4a2e033d  vmr32.w     $vf14, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
        // 0x13b92b0: 0x4a2f033d  vmr32.w     $vf15, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // 0x13b92b4: 0x4a30033d  vmr32.w     $vf16, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        // 0x13b92b8: 0x4bcc5afe  vopmula.xyz $ACC, $vf11, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b92bc: 0x4bc002e9  vmadd.xyz   $vf11, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
        // 0x13b92c0: 0x4bcc5aa9  vmadd.xyz   $vf10, $vf11, $vf12
        { __m128 mul_res = ps2VuMulK<1, 0>(v11, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b92c4: 0x4bca5b6a  vmul.xyz    $vf13, $vf11, $vf10
        { __m128 res = ps2VuMulK<1, 1>(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        // 0x13b92c8: 0x4bca5afe  vopmula.xyz $ACC, $vf11, $vf10
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b92cc: 0x4bcb53cb  vmaddw.xyz  $vf15, $vf10, $vf11w
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // 0x13b92d0: 0x4bcb540f  vmsubw.xyz  $vf16, $vf10, $vf11w
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        // 0x13b92d4: 0x4bcc6afe  vopmula.xyz $ACC, $vf13, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 0>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b92d8: 0x4bcc6c69  vmadd.xyz   $vf17, $vf13, $vf12
        { __m128 mul_res = ps2VuMulK<1, 0>(v13, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
        // 0x13b92dc: 0x4bcc7afe  vopmula.xyz $ACC, $vf15, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 0>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // 0x13b92e0: 0x4bc003e9  vmadd.xyz   $vf15, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // 0x13b92e4: 0x4bd163ac  vsub.xyz    $vf14, $vf12, $vf17
        { __m128 res = ps2VuSubK<0, 1>(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
        // 0x13b92e8: 0x4ad1733c  vmove.yz    $vf17, $vf14
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v17 = _mm_blendv_ps(v17, v14, _mm_castsi128_ps(mask)); }
        // 0x13b92ec: 0x4a8e833c  vmove.y     $vf14, $vf16
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v14 = _mm_blendv_ps(v14, v16, _mm_castsi128_ps(mask)); }
        // 0x13b92f0: 0x4a907b3c  vmove.y     $vf16, $vf15
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v16 = _mm_blendv_ps(v16, v15, _mm_castsi128_ps(mask)); }
        // 0x13b92f4: 0x4a8f8b3c  vmove.y     $vf15, $vf17
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v15 = _mm_blendv_ps(v15, v17, _mm_castsi128_ps(mask)); }
        // 0x13b92f8: 0x4a4e7b3c  vmove.z     $vf14, $vf15
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v14 = _mm_blendv_ps(v14, v15, _mm_castsi128_ps(mask)); }
        // 0x13b92fc: 0x4a4f833c  vmove.z     $vf15, $vf16
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v15 = _mm_blendv_ps(v15, v16, _mm_castsi128_ps(mask)); }
        // 0x13b9300: 0x4a508b3c  vmove.z     $vf16, $vf17
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v16 = _mm_blendv_ps(v16, v17, _mm_castsi128_ps(mask)); }
        // 0x13b9304: 0xf84e0000  sqc2        $vf14, 0x0($v0)
        WRITE128(ADD32(RU32(2), 0), _mm_castps_si128(v14));
        // 0x13b9308: 0xf84f0010  sqc2        $vf15, 0x10($v0)
        WRITE128(ADD32(RU32(2), 16), _mm_castps_si128(v15));
        // 0x13b930c: 0xf8500020  sqc2        $vf16, 0x20($v0)
        WRITE128(ADD32(RU32(2), 32), _mm_castps_si128(v16));
        // 0x13b9310: 0xd8640000  lqc2        $vf4, 0x0($v1)
        v4 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9314: 0x4a24033c  vmove.w     $vf4, $vf0
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v4 = _mm_blendv_ps(v4, v0, _mm_castsi128_ps(mask)); }
        // 0x13b9318: 0xf8440030  sqc2        $vf4, 0x30($v0)
        WRITE128(ADD32(RU32(2), 48), _mm_castps_si128(v4));
        // 0x13b931c: 0x4600a007  neg.s       $f0, $f20
        f0 = FPU_NEG_S(f20);
        // 0x13b9320: 0x266269f0  addiu       $v0, $s3, 0x69F0
        WS32(2, (int32_t)ADD32(RU32(19), 27120));
        // 0x13b9324: 0x27a30050  addiu       $v1, $sp, 0x50
        WS32(3, (int32_t)ADD32(RU32(29), 80));
        // 0x13b9328: 0x27a40020  addiu       $a0, $sp, 0x20
        WS32(4, (int32_t)ADD32(RU32(29), 32));
        // 0x13b932c: 0xe4400004  swc1        $f0, 0x4($v0)
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(2), 4), bits); }
        // 0x13b9330: 0xd8480000  lqc2        $vf8, 0x0($v0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b9334: 0xd88e0000  lqc2        $vf14, 0x0($a0)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9338: 0xd88f0010  lqc2        $vf15, 0x10($a0)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 16)));
        // 0x13b933c: 0xd8900020  lqc2        $vf16, 0x20($a0)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 32)));
        // 0x13b9340: 0xd8910030  lqc2        $vf17, 0x30($a0)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 48)));
        // 0x13b9344: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9348: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b934c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9350: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9354: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b9358: 0x7bb90220  lq          $t9, 0x220($sp)
        WVEC(25, READ128(ADD32(RU32(29), 544)));
    label_13b935c:
        // 0x13b935c: 0x4615a001  sub.s       $f0, $f20, $f21
        f0 = FPU_SUB_S(f20, f21);
        // 0x13b9360: 0x3c013f80  lui         $at, 0x3F80
        WS32(1, (int32_t)((uint32_t)16256 << 16));
        // 0x13b9364: 0x44810800  mtc1        $at, $f1
        { uint32_t bits = RU32(1); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b9368: 0xafa00120  sw          $zero, 0x120($sp)
        WRITE32(ADD32(RU32(29), 288), RU32(0));
        // 0x13b936c: 0x27a200d0  addiu       $v0, $sp, 0xD0
        WS32(2, (int32_t)ADD32(RU32(29), 208));
        // 0x13b9370: 0xafa00128  sw          $zero, 0x128($sp)
        WRITE32(ADD32(RU32(29), 296), RU32(0));
        // 0x13b9374: 0xe7a1012c  swc1        $f1, 0x12C($sp)
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 300), bits); }
        // 0x13b9378: 0x46000007  neg.s       $f0, $f0
        f0 = FPU_NEG_S(f0);
        // 0x13b937c: 0xe7a00124  swc1        $f0, 0x124($sp)
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 292), bits); }
        // 0x13b9380: 0xda480000  lqc2        $vf8, 0x0($s2)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 0)));
        // 0x13b9384: 0xdace0000  lqc2        $vf14, 0x0($s6)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 0)));
        // 0x13b9388: 0xdacf0010  lqc2        $vf15, 0x10($s6)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 16)));
        // 0x13b938c: 0xdad00020  lqc2        $vf16, 0x20($s6)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 32)));
        // 0x13b9390: 0xdad10030  lqc2        $vf17, 0x30($s6)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b9394: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9398: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b939c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b93a0: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b93a4: 0xf84a0000  sqc2        $vf10, 0x0($v0)
        WRITE128(ADD32(RU32(2), 0), _mm_castps_si128(v10));
        // 0x13b93a8: 0x4616a001  sub.s       $f0, $f20, $f22
        f0 = FPU_SUB_S(f20, f22);
        // 0x13b93ac: 0xe7a1013c  swc1        $f1, 0x13C($sp)
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 316), bits); }
        // 0x13b93b0: 0x27a30110  addiu       $v1, $sp, 0x110
        WS32(3, (int32_t)ADD32(RU32(29), 272));
        // 0x13b93b4: 0xafa00130  sw          $zero, 0x130($sp)
        WRITE32(ADD32(RU32(29), 304), RU32(0));
        // 0x13b93b8: 0x27a20130  addiu       $v0, $sp, 0x130
        WS32(2, (int32_t)ADD32(RU32(29), 304));
        // 0x13b93bc: 0xafa00138  sw          $zero, 0x138($sp)
        WRITE32(ADD32(RU32(29), 312), RU32(0));
        // 0x13b93c0: 0x46000007  neg.s       $f0, $f0
        f0 = FPU_NEG_S(f0);
        // 0x13b93c4: 0xe7a00134  swc1        $f0, 0x134($sp)
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32(ADD32(RU32(29), 308), bits); }
        // 0x13b93c8: 0xd8480000  lqc2        $vf8, 0x0($v0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b93cc: 0xdaae0000  lqc2        $vf14, 0x0($s5)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 0)));
        // 0x13b93d0: 0xdaaf0010  lqc2        $vf15, 0x10($s5)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 16)));
        // 0x13b93d4: 0xdab00020  lqc2        $vf16, 0x20($s5)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 32)));
        // 0x13b93d8: 0xdab10030  lqc2        $vf17, 0x30($s5)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b93dc: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b93e0: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b93e4: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b93e8: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b93ec: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b93f0: 0x27a20140  addiu       $v0, $sp, 0x140
        WS32(2, (int32_t)ADD32(RU32(29), 320));
        // 0x13b93f4: 0xdad80030  lqc2        $vf24, 0x30($s6)
        v24 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b93f8: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
        { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v25 = _mm_blendv_ps(v25, res, _mm_castsi128_ps(mask)); }
        // 0x13b93fc: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
        { __m128 res = ps2VuMulK<0, 1>(v24, _mm_shuffle_ps(v25, v25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v28 = _mm_blendv_ps(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b9400: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
        { __m128 res = PS2_VSUB(v28, v28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v28 = PS2_VBLEND(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b9404: 0x7ac80000  lq          $t0, 0x0($s6)
        WVEC(8, READ128(ADD32(RU32(22), 0)));
        // 0x13b9408: 0x7ac90010  lq          $t1, 0x10($s6)
        WVEC(9, READ128(ADD32(RU32(22), 16)));
        // 0x13b940c: 0x7aca0020  lq          $t2, 0x20($s6)
        WVEC(10, READ128(ADD32(RU32(22), 32)));
        // 0x13b9410: 0x482b0000  qmfc2.ni    $t3, $vf0
        WVEC(11, _mm_castps_si128(v0));
        // 0x13b9414: 0x71286488  pextlw      $t4, $t1, $t0
        WVEC(12, PS2_PEXTLW(RVEC(9), RVEC(8)));
        // 0x13b9418: 0x71286ca8  pextuw      $t5, $t1, $t0
        WVEC(13, PS2_PEXTUW(RVEC(9), RVEC(8)));
        // 0x13b941c: 0x716a7488  pextlw      $t6, $t3, $t2
        WVEC(14, PS2_PEXTLW(RVEC(11), RVEC(10)));
        // 0x13b9420: 0x716a7ca8  pextuw      $t7, $t3, $t2
        WVEC(15, PS2_PEXTUW(RVEC(11), RVEC(10)));
        // 0x13b9424: 0x71cc4389  pcpyld      $t0, $t6, $t4
        WVEC(8, PS2_PCPYLD(RVEC(14), RVEC(12)));
        // 0x13b9428: 0x718e4ba9  pcpyud      $t1, $t4, $t6
        WVEC(9, _mm_unpackhi_epi64(RVEC(12), RVEC(14)));
        // 0x13b942c: 0x71ed5389  pcpyld      $t2, $t7, $t5
        WVEC(10, PS2_PCPYLD(RVEC(15), RVEC(13)));
        // 0x13b9430: 0x71af5ba9  pcpyud      $t3, $t5, $t7
        WVEC(11, _mm_unpackhi_epi64(RVEC(13), RVEC(15)));
        // 0x13b9434: 0x48a8c000  qmtc2.ni    $t0, $vf24
        v24 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9438: 0x48a9c800  qmtc2.ni    $t1, $vf25
        v25 = _mm_castsi128_ps(RVEC(9));
        // 0x13b943c: 0x48aad000  qmtc2.ni    $t2, $vf26
        v26 = _mm_castsi128_ps(RVEC(10));
        // 0x13b9440: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
        { __m128 res = PS2_VMUL(v24, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9444: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
        { __m128 mul_res = PS2_VMUL(v25, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9448: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
        { __m128 mul_res = PS2_VMUL(v26, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b944c: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
        { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v29 = _mm_blendv_ps(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9450: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
        { __m128 res = ps2VuSubK<1, 1>(v29, v29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9454: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
        { __m128 res = ps2VuAddK<1, 0>(v29, v0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9458: 0xf8580000  sqc2        $vf24, 0x0($v0)
        WRITE128(ADD32(RU32(2), 0), _mm_castps_si128(v24));
        // 0x13b945c: 0xf8590010  sqc2        $vf25, 0x10($v0)
        WRITE128(ADD32(RU32(2), 16), _mm_castps_si128(v25));
        // 0x13b9460: 0xf85a0020  sqc2        $vf26, 0x20($v0)
        WRITE128(ADD32(RU32(2), 32), _mm_castps_si128(v26));
        // 0x13b9464: 0xf85d0030  sqc2        $vf29, 0x30($v0)
        WRITE128(ADD32(RU32(2), 48), _mm_castps_si128(v29));
        // 0x13b9468: 0xdab80030  lqc2        $vf24, 0x30($s5)
        v24 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b946c: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
        { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v25 = _mm_blendv_ps(v25, res, _mm_castsi128_ps(mask)); }
        // 0x13b9470: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
        { __m128 res = ps2VuMulK<0, 1>(v24, _mm_shuffle_ps(v25, v25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v28 = _mm_blendv_ps(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b9474: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
        { __m128 res = PS2_VSUB(v28, v28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v28 = PS2_VBLEND(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b9478: 0x7aa80000  lq          $t0, 0x0($s5)
        WVEC(8, READ128(ADD32(RU32(21), 0)));
        // 0x13b947c: 0x7aa90010  lq          $t1, 0x10($s5)
        WVEC(9, READ128(ADD32(RU32(21), 16)));
        // 0x13b9480: 0x7aaa0020  lq          $t2, 0x20($s5)
        WVEC(10, READ128(ADD32(RU32(21), 32)));
        // 0x13b9484: 0x482b0000  qmfc2.ni    $t3, $vf0
        WVEC(11, _mm_castps_si128(v0));
        // 0x13b9488: 0x71286488  pextlw      $t4, $t1, $t0
        WVEC(12, PS2_PEXTLW(RVEC(9), RVEC(8)));
        // 0x13b948c: 0x71286ca8  pextuw      $t5, $t1, $t0
        WVEC(13, PS2_PEXTUW(RVEC(9), RVEC(8)));
        // 0x13b9490: 0x716a7488  pextlw      $t6, $t3, $t2
        WVEC(14, PS2_PEXTLW(RVEC(11), RVEC(10)));
        // 0x13b9494: 0x716a7ca8  pextuw      $t7, $t3, $t2
        WVEC(15, PS2_PEXTUW(RVEC(11), RVEC(10)));
        // 0x13b9498: 0x71cc4389  pcpyld      $t0, $t6, $t4
        WVEC(8, PS2_PCPYLD(RVEC(14), RVEC(12)));
        // 0x13b949c: 0x718e4ba9  pcpyud      $t1, $t4, $t6
        WVEC(9, _mm_unpackhi_epi64(RVEC(12), RVEC(14)));
        // 0x13b94a0: 0x71ed5389  pcpyld      $t2, $t7, $t5
        WVEC(10, PS2_PCPYLD(RVEC(15), RVEC(13)));
        // 0x13b94a4: 0x71af5ba9  pcpyud      $t3, $t5, $t7
        WVEC(11, _mm_unpackhi_epi64(RVEC(13), RVEC(15)));
        // 0x13b94a8: 0x48a8c000  qmtc2.ni    $t0, $vf24
        v24 = _mm_castsi128_ps(RVEC(8));
        // 0x13b94ac: 0x48a9c800  qmtc2.ni    $t1, $vf25
        v25 = _mm_castsi128_ps(RVEC(9));
        // 0x13b94b0: 0x48aad000  qmtc2.ni    $t2, $vf26
        v26 = _mm_castsi128_ps(RVEC(10));
        // 0x13b94b4: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
        { __m128 res = PS2_VMUL(v24, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b94b8: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
        { __m128 mul_res = PS2_VMUL(v25, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b94bc: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
        { __m128 mul_res = PS2_VMUL(v26, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b94c0: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
        { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v29 = _mm_blendv_ps(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b94c4: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
        { __m128 res = ps2VuSubK<1, 1>(v29, v29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b94c8: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
        { __m128 res = ps2VuAddK<1, 0>(v29, v0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b94cc: 0xfb380000  sqc2        $vf24, 0x0($t9)
        WRITE128(ADD32(RU32(25), 0), _mm_castps_si128(v24));
        // 0x13b94d0: 0xfb390010  sqc2        $vf25, 0x10($t9)
        WRITE128(ADD32(RU32(25), 16), _mm_castps_si128(v25));
        // 0x13b94d4: 0xfb3a0020  sqc2        $vf26, 0x20($t9)
        WRITE128(ADD32(RU32(25), 32), _mm_castps_si128(v26));
        // 0x13b94d8: 0xfb3d0030  sqc2        $vf29, 0x30($t9)
        WRITE128(ADD32(RU32(25), 48), _mm_castps_si128(v29));
        // 0x13b94dc: 0x27a30060  addiu       $v1, $sp, 0x60
        WS32(3, (int32_t)ADD32(RU32(29), 96));
        // 0x13b94e0: 0x27a40020  addiu       $a0, $sp, 0x20
        WS32(4, (int32_t)ADD32(RU32(29), 32));
        // 0x13b94e4: 0xd8980030  lqc2        $vf24, 0x30($a0)
        v24 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 48)));
        // 0x13b94e8: 0x4b000647  vsubw.x     $vf25, $vf0, $vf0w
        { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v25 = _mm_blendv_ps(v25, res, _mm_castsi128_ps(mask)); }
        // 0x13b94ec: 0x4bd9c718  vmulx.xyz   $vf28, $vf24, $vf25x
        { __m128 res = ps2VuMulK<0, 1>(v24, _mm_shuffle_ps(v25, v25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v28 = _mm_blendv_ps(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b94f0: 0x4a3ce72c  vsub.w      $vf28, $vf28, $vf28
        { __m128 res = PS2_VSUB(v28, v28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v28 = PS2_VBLEND(v28, res, _mm_castsi128_ps(mask)); }
        // 0x13b94f4: 0x78880000  lq          $t0, 0x0($a0)
        WVEC(8, READ128(ADD32(RU32(4), 0)));
        // 0x13b94f8: 0x78890010  lq          $t1, 0x10($a0)
        WVEC(9, READ128(ADD32(RU32(4), 16)));
        // 0x13b94fc: 0x788a0020  lq          $t2, 0x20($a0)
        WVEC(10, READ128(ADD32(RU32(4), 32)));
        // 0x13b9500: 0x482b0000  qmfc2.ni    $t3, $vf0
        WVEC(11, _mm_castps_si128(v0));
        // 0x13b9504: 0x71286488  pextlw      $t4, $t1, $t0
        WVEC(12, PS2_PEXTLW(RVEC(9), RVEC(8)));
        // 0x13b9508: 0x71286ca8  pextuw      $t5, $t1, $t0
        WVEC(13, PS2_PEXTUW(RVEC(9), RVEC(8)));
        // 0x13b950c: 0x716a7488  pextlw      $t6, $t3, $t2
        WVEC(14, PS2_PEXTLW(RVEC(11), RVEC(10)));
        // 0x13b9510: 0x716a7ca8  pextuw      $t7, $t3, $t2
        WVEC(15, PS2_PEXTUW(RVEC(11), RVEC(10)));
        // 0x13b9514: 0x71cc4389  pcpyld      $t0, $t6, $t4
        WVEC(8, PS2_PCPYLD(RVEC(14), RVEC(12)));
        // 0x13b9518: 0x718e4ba9  pcpyud      $t1, $t4, $t6
        WVEC(9, _mm_unpackhi_epi64(RVEC(12), RVEC(14)));
        // 0x13b951c: 0x71ed5389  pcpyld      $t2, $t7, $t5
        WVEC(10, PS2_PCPYLD(RVEC(15), RVEC(13)));
        // 0x13b9520: 0x71af5ba9  pcpyud      $t3, $t5, $t7
        WVEC(11, _mm_unpackhi_epi64(RVEC(13), RVEC(15)));
        // 0x13b9524: 0x48a8c000  qmtc2.ni    $t0, $vf24
        v24 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9528: 0x48a9c800  qmtc2.ni    $t1, $vf25
        v25 = _mm_castsi128_ps(RVEC(9));
        // 0x13b952c: 0x48aad000  qmtc2.ni    $t2, $vf26
        v26 = _mm_castsi128_ps(RVEC(10));
        // 0x13b9530: 0x4bfcc1bc  vmulax.xyzw $ACC, $vf24, $vf28x
        { __m128 res = PS2_VMUL(v24, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9534: 0x4bfcc8bd  vmadday.xyzw $ACC, $vf25, $vf28y
        { __m128 mul_res = PS2_VMUL(v25, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9538: 0x4bfcd0be  vmaddaz.xyzw $ACC, $vf26, $vf28z
        { __m128 mul_res = PS2_VMUL(v26, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b953c: 0x4bfc074b  vmaddw.xyzw $vf29, $vf0, $vf28w
        { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v28, v28, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v29 = _mm_blendv_ps(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9540: 0x4a3def6c  vsub.w      $vf29, $vf29, $vf29
        { __m128 res = ps2VuSubK<1, 1>(v29, v29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9544: 0x4a20ef68  vadd.w      $vf29, $vf29, $vf0
        { __m128 res = ps2VuAddK<1, 0>(v29, v0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v29 = PS2_VBLEND(v29, res, _mm_castsi128_ps(mask)); }
        // 0x13b9548: 0xf8780000  sqc2        $vf24, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v24));
        // 0x13b954c: 0xf8790010  sqc2        $vf25, 0x10($v1)
        WRITE128(ADD32(RU32(3), 16), _mm_castps_si128(v25));
        // 0x13b9550: 0xf87a0020  sqc2        $vf26, 0x20($v1)
        WRITE128(ADD32(RU32(3), 32), _mm_castps_si128(v26));
        // 0x13b9554: 0xf87d0030  sqc2        $vf29, 0x30($v1)
        WRITE128(ADD32(RU32(3), 48), _mm_castps_si128(v29));
        // 0x13b9558: 0x3c020146  lui         $v0, 0x146
        WS32(2, (int32_t)((uint32_t)326 << 16));
        // 0x13b955c: 0x8c4369e0  lw          $v1, 0x69E0($v0)
        WS32(3, (int32_t)READ32(ADD32(RU32(2), 27104)));
        // 0x13b9560: 0x382d  daddu       $a3, $zero, $zero
        WU64(7, (uint64_t)RU64(0) + (uint64_t)RU64(0));
        // 0x13b9564: 0x10600007  beqz        $v1, . + 4 + (0x7 << 2)
        {
            const bool branch_taken_0x13b9564 = (RU64(3) == RU64(0));
            // 0x13b9568: 0x482d  daddu       $t1, $zero, $zero (Delay Slot)
            WU64(9, (uint64_t)RU64(0) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9564) {
                goto label_13b9584;
            }
        }
        // 0x13b956c: 0x86220010  lh          $v0, 0x10($s1)
        WS32(2, (int16_t)READ16(ADD32(RU32(17), 16)));
        // 0x13b9570: 0x18400005  blez        $v0, . + 4 + (0x5 << 2)
        {
            const bool branch_taken_0x13b9570 = (RS32(2) <= 0);
            // 0x13b9574: 0x3c020146  lui         $v0, 0x146 (Delay Slot)
            WS32(2, (int32_t)((uint32_t)326 << 16));
            if (branch_taken_0x13b9570) {
                goto label_13b9588;
            }
        }
        // 0x13b9578: 0x3c020146  lui         $v0, 0x146
        WS32(2, (int32_t)((uint32_t)326 << 16));
        // 0x13b957c: 0x60382d  daddu       $a3, $v1, $zero
        WU64(7, (uint64_t)RU64(3) + (uint64_t)RU64(0));
        // 0x13b9580: 0x8c4969e4  lw          $t1, 0x69E4($v0)
        WS32(9, (int32_t)READ32(ADD32(RU32(2), 27108)));
    label_13b9584:
        // 0x13b9584: 0x3c020146  lui         $v0, 0x146
        WS32(2, (int32_t)((uint32_t)326 << 16));
    label_13b9588:
        // 0x13b9588: 0x8c4369e8  lw          $v1, 0x69E8($v0)
        WS32(3, (int32_t)READ32(ADD32(RU32(2), 27112)));
        // 0x13b958c: 0x10600006  beqz        $v1, . + 4 + (0x6 << 2)
        {
            const bool branch_taken_0x13b958c = (RU64(3) == RU64(0));
            if (branch_taken_0x13b958c) {
                goto label_13b95a8;
            }
        }
        // 0x13b9594: 0x86220010  lh          $v0, 0x10($s1)
        WS32(2, (int16_t)READ16(ADD32(RU32(17), 16)));
        // 0x13b9598: 0x4410003  bgez        $v0, . + 4 + (0x3 << 2)
        {
            const bool branch_taken_0x13b9598 = (RS32(2) >= 0);
            // 0x13b959c: 0x3c020146  lui         $v0, 0x146 (Delay Slot)
            WS32(2, (int32_t)((uint32_t)326 << 16));
            if (branch_taken_0x13b9598) {
                goto label_13b95a8;
            }
        }
        // 0x13b95a0: 0x60382d  daddu       $a3, $v1, $zero
        WU64(7, (uint64_t)RU64(3) + (uint64_t)RU64(0));
        // 0x13b95a4: 0x8c4969ec  lw          $t1, 0x69EC($v0)
        WS32(9, (int32_t)READ32(ADD32(RU32(2), 27116)));
    label_13b95a8:
        // 0x13b95a8: 0x10e00121  beqz        $a3, . + 4 + (0x121 << 2)
        {
            const bool branch_taken_0x13b95a8 = (RU64(7) == RU64(0));
            // 0x13b95ac: 0x2404ffff  addiu       $a0, $zero, -0x1 (Delay Slot)
            WS32(4, (int32_t)ADD32(RU32(0), 4294967295));
            if (branch_taken_0x13b95a8) {
                goto label_13b9a30;
            }
        }
        // 0x13b95b0: 0x8e820000  lw          $v0, 0x0($s4)
        WS32(2, (int32_t)READ32(ADD32(RU32(20), 0)));
        // 0x13b95b4: 0x8e97000c  lw          $s7, 0xC($s4)
        WS32(23, (int32_t)READ32(ADD32(RU32(20), 12)));
        // 0x13b95b8: 0x8c530004  lw          $s3, 0x4($v0)
        WS32(19, (int32_t)READ32(ADD32(RU32(2), 4)));
        // 0x13b95bc: 0x8c430028  lw          $v1, 0x28($v0)
        WS32(3, (int32_t)READ32(ADD32(RU32(2), 40)));
        // 0x13b95c0: 0x2673ffff  addiu       $s3, $s3, -0x1
        WS32(19, (int32_t)ADD32(RU32(19), 4294967295));
        // 0x13b95c4: 0x8c420008  lw          $v0, 0x8($v0)
        WS32(2, (int32_t)READ32(ADD32(RU32(2), 8)));
        // 0x13b95c8: 0xc4750008  lwc1        $f21, 0x8($v1)
        { uint32_t bits = READ32(ADD32(RU32(3), 8)); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
        // 0x13b95cc: 0x12640235  beq         $s3, $a0, . + 4 + (0x235 << 2)
        {
            const bool branch_taken_0x13b95cc = (RU64(19) == RU64(4));
            // 0x13b95d0: 0xc4740004  lwc1        $f20, 0x4($v1) (Delay Slot)
            { uint32_t bits = READ32(ADD32(RU32(3), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
            if (branch_taken_0x13b95cc) {
                goto label_13b9ea4;
            }
        }
        // 0x13b95d4: 0x2442fffe  addiu       $v0, $v0, -0x2
        WS32(2, (int32_t)ADD32(RU32(2), 4294967294));
        // 0x13b95d8: 0x241effff  addiu       $fp, $zero, -0x1
        WS32(30, (int32_t)ADD32(RU32(0), 4294967295));
        // 0x13b95dc: 0xafa201f0  sw          $v0, 0x1F0($sp)
        WRITE32(ADD32(RU32(29), 496), RU32(2));
    label_13b95e0:
        // 0x13b95e0: 0x8fb201f0  lw          $s2, 0x1F0($sp)
        WS32(18, (int32_t)READ32(ADD32(RU32(29), 496)));
        // 0x13b95e4: 0x125e010c  beq         $s2, $fp, . + 4 + (0x10C << 2)
        {
            const bool branch_taken_0x13b95e4 = (RU64(18) == RU64(30));
            // 0x13b95e8: 0x8ee60010  lw          $a2, 0x10($s7) (Delay Slot)
            WS32(6, (int32_t)READ32(ADD32(RU32(23), 16)));
            if (branch_taken_0x13b95e4) {
                goto label_13b9a18;
            }
        }
        // 0x13b95ec: 0x3c013d23  lui         $at, 0x3D23
        WS32(1, (int32_t)((uint32_t)15651 << 16));
        // 0x13b95f0: 0x3421d708  ori         $at, $at, 0xD708
        WU64(1, RU64(1) | (uint64_t)(uint16_t)55048);
        // 0x13b95f4: 0x4481b800  mtc1        $at, $f23
        { uint32_t bits = RU32(1); std::memcpy(&f23, &bits, sizeof(bits)); }
        // 0x13b95f8: 0x27b401c0  addiu       $s4, $sp, 0x1C0
        WS32(20, (int32_t)ADD32(RU32(29), 448));
        // 0x13b95fc: 0x3c013e4c  lui         $at, 0x3E4C
        WS32(1, (int32_t)((uint32_t)15948 << 16));
        // 0x13b9600: 0x3421cccc  ori         $at, $at, 0xCCCC
        WU64(1, RU64(1) | (uint64_t)(uint16_t)52428);
        // 0x13b9604: 0x4481b000  mtc1        $at, $f22
        { uint32_t bits = RU32(1); std::memcpy(&f22, &bits, sizeof(bits)); }
        // 0x13b9608: 0x27b101d0  addiu       $s1, $sp, 0x1D0
        WS32(17, (int32_t)ADD32(RU32(29), 464));
        // 0x13b960c: 0x0  nop
        // NOP
    label_13b9610:
        // 0x13b9610: 0x24d00010  addiu       $s0, $a2, 0x10
        WS32(16, (int32_t)ADD32(RU32(6), 16));
        // 0x13b9614: 0xda050000  lqc2        $vf5, 0x0($s0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9618: 0xd8c60000  lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b961c: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b9620: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9624: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9628: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b962c: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9630: 0xfa880000  sqc2        $vf8, 0x0($s4)
        WRITE128(ADD32(RU32(20), 0), _mm_castps_si128(v8));
        // 0x13b9634: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9638: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b963c: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9640: 0x46150034  c.lt.s      $f0, $f21
        fcr31 = (FPU_C_OLT_S(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9644: 0x4500000a  bc1f        . + 4 + (0xA << 2)
        {
            const bool branch_taken_0x13b9644 = (!(fcr31 & 0x800000));
            // 0x13b9648: 0x27a20140  addiu       $v0, $sp, 0x140 (Delay Slot)
            WS32(2, (int32_t)ADD32(RU32(29), 320));
            if (branch_taken_0x13b9644) {
                goto label_13b9670;
            }
        }
        // 0x13b964c: 0x4600a002  mul.s       $f0, $f20, $f0
        f0 = FPU_MUL_S(f20, f0);
        // 0x13b9650: 0xd8c70000  lqc2        $vf7, 0x0($a2)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b9654: 0xda850000  lqc2        $vf5, 0x0($s4)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(20), 0)));
        // 0x13b9658: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b965c: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9660: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9664: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9668: 0xfa070000  sqc2        $vf7, 0x0($s0)
        WRITE128(ADD32(RU32(16), 0), _mm_castps_si128(v7));
        // 0x13b966c: 0x27a20140  addiu       $v0, $sp, 0x140
        WS32(2, (int32_t)ADD32(RU32(29), 320));
    label_13b9670:
        // 0x13b9670: 0xda080000  lqc2        $vf8, 0x0($s0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9674: 0xd84e0000  lqc2        $vf14, 0x0($v0)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 0)));
        // 0x13b9678: 0xd84f0010  lqc2        $vf15, 0x10($v0)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 16)));
        // 0x13b967c: 0xd8500020  lqc2        $vf16, 0x20($v0)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 32)));
        // 0x13b9680: 0xd8510030  lqc2        $vf17, 0x30($v0)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(2), 48)));
        // 0x13b9684: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9688: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b968c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9690: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9694: 0xfa2a0000  sqc2        $vf10, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v10));
        // 0x13b9698: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b969c: 0x44800800  mtc1        $zero, $f1
        { uint32_t bits = RU32(0); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b96a0: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b96a4: 0x45010004  bc1t        . + 4 + (0x4 << 2)
        {
            const bool branch_taken_0x13b96a4 = ((fcr31 & 0x800000));
            if (branch_taken_0x13b96a4) {
                goto label_13b96b8;
            }
        }
        // 0x13b96ac: 0x4601c032  c.eq.s      $f24, $f1
        fcr31 = (FPU_C_EQ_S(f24, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b96b0: 0x45000063  bc1f        . + 4 + (0x63 << 2)
        {
            const bool branch_taken_0x13b96b0 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b96b0) {
                goto label_13b9840;
            }
        }
    label_13b96b8:
        // 0x13b96b8: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b96bc: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b96c0: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b96c4: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b96c8: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b96cc: 0x46170034  c.lt.s      $f0, $f23
        fcr31 = (FPU_C_OLT_S(f0, f23)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b96d0: 0x4500002c  bc1f        . + 4 + (0x2C << 2)
        {
            const bool branch_taken_0x13b96d0 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b96d0) {
                goto label_13b9784;
            }
        }
        // 0x13b96d8: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b96dc: 0x0  nop
        // NOP
        // 0x13b96e0: 0x0  nop
        // NOP
        // 0x13b96e4: 0x4600b003  div.s       $f0, $f22, $f0
        f0 = ps2FpuDivide(fcr31, f22, f0);
        // 0x13b96e8: 0xda250000  lqc2        $vf5, 0x0($s1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b96ec: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b96f0: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b96f4: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b96f8: 0xfa250000  sqc2        $vf5, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v5));
        // 0x13b96fc: 0xda280000  lqc2        $vf8, 0x0($s1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9700: 0xdace0000  lqc2        $vf14, 0x0($s6)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 0)));
        // 0x13b9704: 0xdacf0010  lqc2        $vf15, 0x10($s6)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 16)));
        // 0x13b9708: 0xdad00020  lqc2        $vf16, 0x20($s6)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 32)));
        // 0x13b970c: 0xdad10030  lqc2        $vf17, 0x30($s6)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(22), 48)));
        // 0x13b9710: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9714: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9718: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b971c: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9720: 0xfa0a0000  sqc2        $vf10, 0x0($s0)
        WRITE128(ADD32(RU32(16), 0), _mm_castps_si128(v10));
        // 0x13b9724: 0x200182d  daddu       $v1, $s0, $zero
        WU64(3, (uint64_t)RU64(16) + (uint64_t)RU64(0));
        // 0x13b9728: 0x27a401e0  addiu       $a0, $sp, 0x1E0
        WS32(4, (int32_t)ADD32(RU32(29), 480));
        // 0x13b972c: 0xda050000  lqc2        $vf5, 0x0($s0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9730: 0xd8c60000  lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b9734: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b9738: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b973c: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9740: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9744: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9748: 0xf8880000  sqc2        $vf8, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v8));
        // 0x13b974c: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9750: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b9754: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9758: 0x46150034  c.lt.s      $f0, $f21
        fcr31 = (FPU_C_OLT_S(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b975c: 0x45000009  bc1f        . + 4 + (0x9 << 2)
        {
            const bool branch_taken_0x13b975c = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b975c) {
                goto label_13b9784;
            }
        }
        // 0x13b9764: 0x4600a002  mul.s       $f0, $f20, $f0
        f0 = FPU_MUL_S(f20, f0);
        // 0x13b9768: 0xd8c70000  lqc2        $vf7, 0x0($a2)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b976c: 0xd8850000  lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9770: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9774: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9778: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b977c: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9780: 0xf8670000  sqc2        $vf7, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v7));
    label_13b9784:
        // 0x13b9784: 0xda080000  lqc2        $vf8, 0x0($s0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9788: 0xdb2e0000  lqc2        $vf14, 0x0($t9)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 0)));
        // 0x13b978c: 0xdb2f0010  lqc2        $vf15, 0x10($t9)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 16)));
        // 0x13b9790: 0xdb300020  lqc2        $vf16, 0x20($t9)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 32)));
        // 0x13b9794: 0xdb310030  lqc2        $vf17, 0x30($t9)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 48)));
        // 0x13b9798: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b979c: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b97a0: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b97a4: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b97a8: 0xfa2a0000  sqc2        $vf10, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v10));
        // 0x13b97ac: 0x44800800  mtc1        $zero, $f1
        { uint32_t bits = RU32(0); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b97b0: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b97b4: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b97b8: 0x45010004  bc1t        . + 4 + (0x4 << 2)
        {
            const bool branch_taken_0x13b97b8 = ((fcr31 & 0x800000));
            if (branch_taken_0x13b97b8) {
                goto label_13b97cc;
            }
        }
        // 0x13b97c0: 0x4601c032  c.eq.s      $f24, $f1
        fcr31 = (FPU_C_EQ_S(f24, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b97c4: 0x45020089  bc1fl       . + 4 + (0x89 << 2)
        {
            const bool branch_taken_0x13b97c4 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b97c4) {
                // 0x13b97c8: 0x7fa70200  sq          $a3, 0x200($sp) (Delay Slot)
                WRITE128(ADD32(RU32(29), 512), RVEC(7));
                goto label_13b99ec;
            }
        }
    label_13b97cc:
        // 0x13b97cc: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b97d0: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b97d4: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b97d8: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b97dc: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b97e0: 0x46170034  c.lt.s      $f0, $f23
        fcr31 = (FPU_C_OLT_S(f0, f23)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b97e4: 0x45020081  bc1fl       . + 4 + (0x81 << 2)
        {
            const bool branch_taken_0x13b97e4 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b97e4) {
                // 0x13b97e8: 0x7fa70200  sq          $a3, 0x200($sp) (Delay Slot)
                WRITE128(ADD32(RU32(29), 512), RVEC(7));
                goto label_13b99ec;
            }
        }
        // 0x13b97ec: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b97f0: 0x0  nop
        // NOP
        // 0x13b97f4: 0x0  nop
        // NOP
        // 0x13b97f8: 0x4600b003  div.s       $f0, $f22, $f0
        f0 = ps2FpuDivide(fcr31, f22, f0);
        // 0x13b97fc: 0xda250000  lqc2        $vf5, 0x0($s1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9800: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9804: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9808: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b980c: 0xfa250000  sqc2        $vf5, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v5));
        // 0x13b9810: 0xda280000  lqc2        $vf8, 0x0($s1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9814: 0xdaae0000  lqc2        $vf14, 0x0($s5)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 0)));
        // 0x13b9818: 0xdaaf0010  lqc2        $vf15, 0x10($s5)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 16)));
        // 0x13b981c: 0xdab00020  lqc2        $vf16, 0x20($s5)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 32)));
        // 0x13b9820: 0xdab10030  lqc2        $vf17, 0x30($s5)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b9824: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9828: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b982c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9830: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9834: 0xfa0a0000  sqc2        $vf10, 0x0($s0)
        WRITE128(ADD32(RU32(16), 0), _mm_castps_si128(v10));
        // 0x13b9838: 0x10000054  b           . + 4 + (0x54 << 2)
        {
            const bool branch_taken_0x13b9838 = (RU64(0) == RU64(0));
            // 0x13b983c: 0x200182d  daddu       $v1, $s0, $zero (Delay Slot)
            WU64(3, (uint64_t)RU64(16) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9838) {
                goto label_13b998c;
            }
        }
    label_13b9840:
        // 0x13b9840: 0xda080000  lqc2        $vf8, 0x0($s0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9844: 0xdb2e0000  lqc2        $vf14, 0x0($t9)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 0)));
        // 0x13b9848: 0xdb2f0010  lqc2        $vf15, 0x10($t9)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 16)));
        // 0x13b984c: 0xdb300020  lqc2        $vf16, 0x20($t9)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 32)));
        // 0x13b9850: 0xdb310030  lqc2        $vf17, 0x30($t9)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(25), 48)));
        // 0x13b9854: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9858: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b985c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9860: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9864: 0xfa2a0000  sqc2        $vf10, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v10));
        // 0x13b9868: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b986c: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9870: 0x4500001f  bc1f        . + 4 + (0x1F << 2)
        {
            const bool branch_taken_0x13b9870 = (!(fcr31 & 0x800000));
            // 0x13b9874: 0x27a30060  addiu       $v1, $sp, 0x60 (Delay Slot)
            WS32(3, (int32_t)ADD32(RU32(29), 96));
            if (branch_taken_0x13b9870) {
                goto label_13b98f0;
            }
        }
        // 0x13b9878: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b987c: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9880: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9884: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9888: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b988c: 0x46170034  c.lt.s      $f0, $f23
        fcr31 = (FPU_C_OLT_S(f0, f23)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9890: 0x45020056  bc1fl       . + 4 + (0x56 << 2)
        {
            const bool branch_taken_0x13b9890 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9890) {
                // 0x13b9894: 0x7fa70200  sq          $a3, 0x200($sp) (Delay Slot)
                WRITE128(ADD32(RU32(29), 512), RVEC(7));
                goto label_13b99ec;
            }
        }
        // 0x13b9898: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b989c: 0x0  nop
        // NOP
        // 0x13b98a0: 0x0  nop
        // NOP
        // 0x13b98a4: 0x4600b003  div.s       $f0, $f22, $f0
        f0 = ps2FpuDivide(fcr31, f22, f0);
        // 0x13b98a8: 0xda250000  lqc2        $vf5, 0x0($s1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b98ac: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b98b0: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b98b4: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b98b8: 0xfa250000  sqc2        $vf5, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v5));
        // 0x13b98bc: 0xda280000  lqc2        $vf8, 0x0($s1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b98c0: 0xdaae0000  lqc2        $vf14, 0x0($s5)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 0)));
        // 0x13b98c4: 0xdaaf0010  lqc2        $vf15, 0x10($s5)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 16)));
        // 0x13b98c8: 0xdab00020  lqc2        $vf16, 0x20($s5)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 32)));
        // 0x13b98cc: 0xdab10030  lqc2        $vf17, 0x30($s5)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(21), 48)));
        // 0x13b98d0: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b98d4: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b98d8: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b98dc: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b98e0: 0xfa0a0000  sqc2        $vf10, 0x0($s0)
        WRITE128(ADD32(RU32(16), 0), _mm_castps_si128(v10));
        // 0x13b98e4: 0x10000029  b           . + 4 + (0x29 << 2)
        {
            const bool branch_taken_0x13b98e4 = (RU64(0) == RU64(0));
            // 0x13b98e8: 0x200182d  daddu       $v1, $s0, $zero (Delay Slot)
            WU64(3, (uint64_t)RU64(16) + (uint64_t)RU64(0));
            if (branch_taken_0x13b98e4) {
                goto label_13b998c;
            }
        }
        // 0x13b98ec: 0x0  nop
        // NOP
    label_13b98f0:
        // 0x13b98f0: 0xda080000  lqc2        $vf8, 0x0($s0)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b98f4: 0xd86e0000  lqc2        $vf14, 0x0($v1)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b98f8: 0xd86f0010  lqc2        $vf15, 0x10($v1)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 16)));
        // 0x13b98fc: 0xd8700020  lqc2        $vf16, 0x20($v1)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 32)));
        // 0x13b9900: 0xd8710030  lqc2        $vf17, 0x30($v1)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 48)));
        // 0x13b9904: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9908: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b990c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9910: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9914: 0xfa2a0000  sqc2        $vf10, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v10));
        // 0x13b9918: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b991c: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9920: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9924: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9928: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b992c: 0x46170034  c.lt.s      $f0, $f23
        fcr31 = (FPU_C_OLT_S(f0, f23)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9930: 0x4502002e  bc1fl       . + 4 + (0x2E << 2)
        {
            const bool branch_taken_0x13b9930 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9930) {
                // 0x13b9934: 0x7fa70200  sq          $a3, 0x200($sp) (Delay Slot)
                WRITE128(ADD32(RU32(29), 512), RVEC(7));
                goto label_13b99ec;
            }
        }
        // 0x13b9938: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b993c: 0x0  nop
        // NOP
        // 0x13b9940: 0x0  nop
        // NOP
        // 0x13b9944: 0x4600b003  div.s       $f0, $f22, $f0
        f0 = ps2FpuDivide(fcr31, f22, f0);
        // 0x13b9948: 0xda250000  lqc2        $vf5, 0x0($s1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b994c: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9950: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9954: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9958: 0xfa250000  sqc2        $vf5, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v5));
        // 0x13b995c: 0x27a40020  addiu       $a0, $sp, 0x20
        WS32(4, (int32_t)ADD32(RU32(29), 32));
        // 0x13b9960: 0xda280000  lqc2        $vf8, 0x0($s1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9964: 0xd88e0000  lqc2        $vf14, 0x0($a0)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9968: 0xd88f0010  lqc2        $vf15, 0x10($a0)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 16)));
        // 0x13b996c: 0xd8900020  lqc2        $vf16, 0x20($a0)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 32)));
        // 0x13b9970: 0xd8910030  lqc2        $vf17, 0x30($a0)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 48)));
        // 0x13b9974: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9978: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b997c: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9980: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9984: 0xfa0a0000  sqc2        $vf10, 0x0($s0)
        WRITE128(ADD32(RU32(16), 0), _mm_castps_si128(v10));
        // 0x13b9988: 0x200182d  daddu       $v1, $s0, $zero
        WU64(3, (uint64_t)RU64(16) + (uint64_t)RU64(0));
    label_13b998c:
        // 0x13b998c: 0x27a401e0  addiu       $a0, $sp, 0x1E0
        WS32(4, (int32_t)ADD32(RU32(29), 480));
        // 0x13b9990: 0xda050000  lqc2        $vf5, 0x0($s0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9994: 0xd8c60000  lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b9998: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b999c: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b99a0: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b99a4: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b99a8: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b99ac: 0xf8880000  sqc2        $vf8, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v8));
        // 0x13b99b0: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b99b4: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b99b8: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b99bc: 0x46150034  c.lt.s      $f0, $f21
        fcr31 = (FPU_C_OLT_S(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b99c0: 0x4502000a  bc1fl       . + 4 + (0xA << 2)
        {
            const bool branch_taken_0x13b99c0 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b99c0) {
                // 0x13b99c4: 0x7fa70200  sq          $a3, 0x200($sp) (Delay Slot)
                WRITE128(ADD32(RU32(29), 512), RVEC(7));
                goto label_13b99ec;
            }
        }
        // 0x13b99c8: 0x4600a002  mul.s       $f0, $f20, $f0
        f0 = FPU_MUL_S(f20, f0);
        // 0x13b99cc: 0xd8c70000  lqc2        $vf7, 0x0($a2)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b99d0: 0xd8850000  lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b99d4: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b99d8: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b99dc: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b99e0: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b99e4: 0xf8670000  sqc2        $vf7, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v7));
        // 0x13b99e8: 0x7fa70200  sq          $a3, 0x200($sp)
        WRITE128(ADD32(RU32(29), 512), RVEC(7));
    label_13b99ec:
        // 0x13b99ec: 0x120282d  daddu       $a1, $t1, $zero
        WU64(5, (uint64_t)RU64(9) + (uint64_t)RU64(0));
        // 0x13b99f0: 0x7fa90210  sq          $t1, 0x210($sp)
        WRITE128(ADD32(RU32(29), 528), RVEC(9));
        // 0x13b99f4: 0x200202d  daddu       $a0, $s0, $zero
        WU64(4, (uint64_t)RU64(16) + (uint64_t)RU64(0));
        // 0x13b99f8: 0x7fb90220  sq          $t9, 0x220($sp)
        WRITE128(ADD32(RU32(29), 544), RVEC(25));
        // 0x13b99fc: 0xe0f809  jalr        $a3
        {
            const uint32_t jumpTarget = RU32(7);
            WS32(31, 0x13B9A04u);
            // 0x13b9a00: 0x2652ffff  addiu       $s2, $s2, -0x1 (Delay Slot)
            WS32(18, (int32_t)ADD32(RU32(18), 4294967295));
            LIFT_CALL(jumpTarget);
        }
    label_13b9a04:
        // 0x13b9a04: 0x200302d  daddu       $a2, $s0, $zero
        WU64(6, (uint64_t)RU64(16) + (uint64_t)RU64(0));
        // 0x13b9a08: 0x7ba70200  lq          $a3, 0x200($sp)
        WVEC(7, READ128(ADD32(RU32(29), 512)));
        // 0x13b9a0c: 0x7ba90210  lq          $t1, 0x210($sp)
        WVEC(9, READ128(ADD32(RU32(29), 528)));
        // 0x13b9a10: 0x165efeff  bne         $s2, $fp, . + 4 + (-0x101 << 2)
        {
            const bool branch_taken_0x13b9a10 = (RU64(18) != RU64(30));
            // 0x13b9a14: 0x7bb90220  lq          $t9, 0x220($sp) (Delay Slot)
            WVEC(25, READ128(ADD32(RU32(29), 544)));
            if (branch_taken_0x13b9a10) {
                goto label_13b9610;
            }
        }
    label_13b9a18:
        // 0x13b9a18: 0x2673ffff  addiu       $s3, $s3, -0x1
        WS32(19, (int32_t)ADD32(RU32(19), 4294967295));
        // 0x13b9a1c: 0x167efef0  bne         $s3, $fp, . + 4 + (-0x110 << 2)
        {
            const bool branch_taken_0x13b9a1c = (RU64(19) != RU64(30));
            // 0x13b9a20: 0x26f70018  addiu       $s7, $s7, 0x18 (Delay Slot)
            WS32(23, (int32_t)ADD32(RU32(23), 24));
            if (branch_taken_0x13b9a1c) {
                goto label_13b95e0;
            }
        }
        // 0x13b9a24: 0x10000120  b           . + 4 + (0x120 << 2)
        {
            const bool branch_taken_0x13b9a24 = (RU64(0) == RU64(0));
            // 0x13b9a28: 0xdfb00230  ld          $s0, 0x230($sp) (Delay Slot)
            WU64(16, READ64(ADD32(RU32(29), 560)));
            if (branch_taken_0x13b9a24) {
                goto label_13b9ea8;
            }
        }
        // 0x13b9a2c: 0x0  nop
        // NOP
    label_13b9a30:
        // 0x13b9a30: 0x8e820000  lw          $v0, 0x0($s4)
        WS32(2, (int32_t)READ32(ADD32(RU32(20), 0)));
        // 0x13b9a34: 0x8e93000c  lw          $s3, 0xC($s4)
        WS32(19, (int32_t)READ32(ADD32(RU32(20), 12)));
        // 0x13b9a38: 0x8c4d0004  lw          $t5, 0x4($v0)
        WS32(13, (int32_t)READ32(ADD32(RU32(2), 4)));
        // 0x13b9a3c: 0x8c430028  lw          $v1, 0x28($v0)
        WS32(3, (int32_t)READ32(ADD32(RU32(2), 40)));
        // 0x13b9a40: 0x25adffff  addiu       $t5, $t5, -0x1
        WS32(13, (int32_t)ADD32(RU32(13), 4294967295));
        // 0x13b9a44: 0x8c420008  lw          $v0, 0x8($v0)
        WS32(2, (int32_t)READ32(ADD32(RU32(2), 8)));
        // 0x13b9a48: 0xc4630008  lwc1        $f3, 0x8($v1)
        { uint32_t bits = READ32(ADD32(RU32(3), 8)); float f; std::memcpy(&f, &bits, sizeof(f)); f3 = f; }
        // 0x13b9a4c: 0x11a40115  beq         $t5, $a0, . + 4 + (0x115 << 2)
        {
            const bool branch_taken_0x13b9a4c = (RU64(13) == RU64(4));
            // 0x13b9a50: 0xc4620004  lwc1        $f2, 0x4($v1) (Delay Slot)
            { uint32_t bits = READ32(ADD32(RU32(3), 4)); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
            if (branch_taken_0x13b9a4c) {
                goto label_13b9ea4;
            }
        }
        // 0x13b9a54: 0x2457fffe  addiu       $s7, $v0, -0x2
        WS32(23, (int32_t)ADD32(RU32(2), 4294967294));
        // 0x13b9a58: 0x2414ffff  addiu       $s4, $zero, -0x1
        WS32(20, (int32_t)ADD32(RU32(0), 4294967295));
        // 0x13b9a5c: 0x0  nop
        // NOP
    label_13b9a60:
        // 0x13b9a60: 0x2e0582d  daddu       $t3, $s7, $zero
        WU64(11, (uint64_t)RU64(23) + (uint64_t)RU64(0));
        // 0x13b9a64: 0x1174010c  beq         $t3, $s4, . + 4 + (0x10C << 2)
        {
            const bool branch_taken_0x13b9a64 = (RU64(11) == RU64(20));
            // 0x13b9a68: 0x8e650010  lw          $a1, 0x10($s3) (Delay Slot)
            WS32(5, (int32_t)READ32(ADD32(RU32(19), 16)));
            if (branch_taken_0x13b9a64) {
                goto label_13b9e98;
            }
        }
        // 0x13b9a6c: 0x27b101d0  addiu       $s1, $sp, 0x1D0
        WS32(17, (int32_t)ADD32(RU32(29), 464));
        // 0x13b9a70: 0x27a20140  addiu       $v0, $sp, 0x140
        WS32(2, (int32_t)ADD32(RU32(29), 320));
        // 0x13b9a74: 0x27a30060  addiu       $v1, $sp, 0x60
        WS32(3, (int32_t)ADD32(RU32(29), 96));
        // 0x13b9a78: 0x27a40020  addiu       $a0, $sp, 0x20
        WS32(4, (int32_t)ADD32(RU32(29), 32));
        // 0x13b9a7c: 0x3c013d23  lui         $at, 0x3D23
        WS32(1, (int32_t)((uint32_t)15651 << 16));
        // 0x13b9a80: 0x3421d708  ori         $at, $at, 0xD708
        WU64(1, RU64(1) | (uint64_t)(uint16_t)55048);
        // 0x13b9a84: 0x44812800  mtc1        $at, $f5
        { uint32_t bits = RU32(1); std::memcpy(&f5, &bits, sizeof(bits)); }
        // 0x13b9a88: 0x27b201c0  addiu       $s2, $sp, 0x1C0
        WS32(18, (int32_t)ADD32(RU32(29), 448));
        // 0x13b9a8c: 0x3c013e4c  lui         $at, 0x3E4C
        WS32(1, (int32_t)((uint32_t)15948 << 16));
        // 0x13b9a90: 0x3421cccc  ori         $at, $at, 0xCCCC
        WU64(1, RU64(1) | (uint64_t)(uint16_t)52428);
        // 0x13b9a94: 0x44812000  mtc1        $at, $f4
        { uint32_t bits = RU32(1); std::memcpy(&f4, &bits, sizeof(bits)); }
        // 0x13b9a98: 0x40802d  daddu       $s0, $v0, $zero
        WU64(16, (uint64_t)RU64(2) + (uint64_t)RU64(0));
        // 0x13b9a9c: 0x220382d  daddu       $a3, $s1, $zero
        WU64(7, (uint64_t)RU64(17) + (uint64_t)RU64(0));
        // 0x13b9aa0: 0x2c0782d  daddu       $t7, $s6, $zero
        WU64(15, (uint64_t)RU64(22) + (uint64_t)RU64(0));
        // 0x13b9aa4: 0x320502d  daddu       $t2, $t9, $zero
        WU64(10, (uint64_t)RU64(25) + (uint64_t)RU64(0));
        // 0x13b9aa8: 0x2a0482d  daddu       $t1, $s5, $zero
        WU64(9, (uint64_t)RU64(21) + (uint64_t)RU64(0));
        // 0x13b9aac: 0x60702d  daddu       $t6, $v1, $zero
        WU64(14, (uint64_t)RU64(3) + (uint64_t)RU64(0));
        // 0x13b9ab0: 0x80602d  daddu       $t4, $a0, $zero
        WU64(12, (uint64_t)RU64(4) + (uint64_t)RU64(0));
        // 0x13b9ab4: 0x0  nop
        // NOP
    label_13b9ab8:
        // 0x13b9ab8: 0x24a30010  addiu       $v1, $a1, 0x10
        WS32(3, (int32_t)ADD32(RU32(5), 16));
        // 0x13b9abc: 0xd8650000  lqc2        $vf5, 0x0($v1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9ac0: 0xd8a60000  lqc2        $vf6, 0x0($a1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9ac4: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b9ac8: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9acc: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9ad0: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9ad4: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9ad8: 0xfa480000  sqc2        $vf8, 0x0($s2)
        WRITE128(ADD32(RU32(18), 0), _mm_castps_si128(v8));
        // 0x13b9adc: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9ae0: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b9ae4: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9ae8: 0x46030034  c.lt.s      $f0, $f3
        fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9aec: 0x4500000a  bc1f        . + 4 + (0xA << 2)
        {
            const bool branch_taken_0x13b9aec = (!(fcr31 & 0x800000));
            // 0x13b9af0: 0x220202d  daddu       $a0, $s1, $zero (Delay Slot)
            WU64(4, (uint64_t)RU64(17) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9aec) {
                goto label_13b9b18;
            }
        }
        // 0x13b9af4: 0x46001002  mul.s       $f0, $f2, $f0
        f0 = FPU_MUL_S(f2, f0);
        // 0x13b9af8: 0xd8a70000  lqc2        $vf7, 0x0($a1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9afc: 0xda450000  lqc2        $vf5, 0x0($s2)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(18), 0)));
        // 0x13b9b00: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9b04: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9b08: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9b0c: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9b10: 0xf8670000  sqc2        $vf7, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v7));
        // 0x13b9b14: 0x220202d  daddu       $a0, $s1, $zero
        WU64(4, (uint64_t)RU64(17) + (uint64_t)RU64(0));
    label_13b9b18:
        // 0x13b9b18: 0xd8680000  lqc2        $vf8, 0x0($v1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9b1c: 0xda0e0000  lqc2        $vf14, 0x0($s0)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 0)));
        // 0x13b9b20: 0xda0f0010  lqc2        $vf15, 0x10($s0)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 16)));
        // 0x13b9b24: 0xda100020  lqc2        $vf16, 0x20($s0)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 32)));
        // 0x13b9b28: 0xda110030  lqc2        $vf17, 0x30($s0)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(16), 48)));
        // 0x13b9b2c: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9b30: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9b34: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9b38: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9b3c: 0xf88a0000  sqc2        $vf10, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v10));
        // 0x13b9b40: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b9b44: 0x44800800  mtc1        $zero, $f1
        { uint32_t bits = RU32(0); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b9b48: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9b4c: 0x45010004  bc1t        . + 4 + (0x4 << 2)
        {
            const bool branch_taken_0x13b9b4c = ((fcr31 & 0x800000));
            if (branch_taken_0x13b9b4c) {
                goto label_13b9b60;
            }
        }
        // 0x13b9b54: 0x4601c032  c.eq.s      $f24, $f1
        fcr31 = (FPU_C_EQ_S(f24, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9b58: 0x45000063  bc1f        . + 4 + (0x63 << 2)
        {
            const bool branch_taken_0x13b9b58 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9b58) {
                goto label_13b9ce8;
            }
        }
    label_13b9b60:
        // 0x13b9b60: 0xd8870000  lqc2        $vf7, 0x0($a0)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9b64: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9b68: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9b6c: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9b70: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9b74: 0x46050034  c.lt.s      $f0, $f5
        fcr31 = (FPU_C_OLT_S(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9b78: 0x4500002c  bc1f        . + 4 + (0x2C << 2)
        {
            const bool branch_taken_0x13b9b78 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9b78) {
                goto label_13b9c2c;
            }
        }
        // 0x13b9b80: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b9b84: 0x0  nop
        // NOP
        // 0x13b9b88: 0x0  nop
        // NOP
        // 0x13b9b8c: 0x46002003  div.s       $f0, $f4, $f0
        f0 = ps2FpuDivide(fcr31, f4, f0);
        // 0x13b9b90: 0xd8e50000  lqc2        $vf5, 0x0($a3)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9b94: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9b98: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9b9c: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9ba0: 0xf8e50000  sqc2        $vf5, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v5));
        // 0x13b9ba4: 0xd8e80000  lqc2        $vf8, 0x0($a3)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9ba8: 0xd9ee0000  lqc2        $vf14, 0x0($t7)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(15), 0)));
        // 0x13b9bac: 0xd9ef0010  lqc2        $vf15, 0x10($t7)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(15), 16)));
        // 0x13b9bb0: 0xd9f00020  lqc2        $vf16, 0x20($t7)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(15), 32)));
        // 0x13b9bb4: 0xd9f10030  lqc2        $vf17, 0x30($t7)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(15), 48)));
        // 0x13b9bb8: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9bbc: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9bc0: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9bc4: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9bc8: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b9bcc: 0x60202d  daddu       $a0, $v1, $zero
        WU64(4, (uint64_t)RU64(3) + (uint64_t)RU64(0));
        // 0x13b9bd0: 0x27a601e0  addiu       $a2, $sp, 0x1E0
        WS32(6, (int32_t)ADD32(RU32(29), 480));
        // 0x13b9bd4: 0xd8650000  lqc2        $vf5, 0x0($v1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9bd8: 0xd8a60000  lqc2        $vf6, 0x0($a1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9bdc: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b9be0: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9be4: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9be8: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9bec: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9bf0: 0xf8c80000  sqc2        $vf8, 0x0($a2)
        WRITE128(ADD32(RU32(6), 0), _mm_castps_si128(v8));
        // 0x13b9bf4: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9bf8: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b9bfc: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9c00: 0x46030034  c.lt.s      $f0, $f3
        fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9c04: 0x45000009  bc1f        . + 4 + (0x9 << 2)
        {
            const bool branch_taken_0x13b9c04 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9c04) {
                goto label_13b9c2c;
            }
        }
        // 0x13b9c0c: 0x46001002  mul.s       $f0, $f2, $f0
        f0 = FPU_MUL_S(f2, f0);
        // 0x13b9c10: 0xd8a70000  lqc2        $vf7, 0x0($a1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9c14: 0xd8c50000  lqc2        $vf5, 0x0($a2)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b9c18: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9c1c: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9c20: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9c24: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9c28: 0xf8870000  sqc2        $vf7, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v7));
    label_13b9c2c:
        // 0x13b9c2c: 0xd8680000  lqc2        $vf8, 0x0($v1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9c30: 0xd94e0000  lqc2        $vf14, 0x0($t2)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 0)));
        // 0x13b9c34: 0xd94f0010  lqc2        $vf15, 0x10($t2)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 16)));
        // 0x13b9c38: 0xd9500020  lqc2        $vf16, 0x20($t2)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 32)));
        // 0x13b9c3c: 0xd9510030  lqc2        $vf17, 0x30($t2)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 48)));
        // 0x13b9c40: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9c44: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9c48: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9c4c: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9c50: 0xfa2a0000  sqc2        $vf10, 0x0($s1)
        WRITE128(ADD32(RU32(17), 0), _mm_castps_si128(v10));
        // 0x13b9c54: 0x44800800  mtc1        $zero, $f1
        { uint32_t bits = RU32(0); std::memcpy(&f1, &bits, sizeof(bits)); }
        // 0x13b9c58: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b9c5c: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9c60: 0x45010004  bc1t        . + 4 + (0x4 << 2)
        {
            const bool branch_taken_0x13b9c60 = ((fcr31 & 0x800000));
            if (branch_taken_0x13b9c60) {
                goto label_13b9c74;
            }
        }
        // 0x13b9c68: 0x4601c032  c.eq.s      $f24, $f1
        fcr31 = (FPU_C_EQ_S(f24, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9c6c: 0x45020088  bc1fl       . + 4 + (0x88 << 2)
        {
            const bool branch_taken_0x13b9c6c = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9c6c) {
                // 0x13b9c70: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b9e90;
            }
        }
    label_13b9c74:
        // 0x13b9c74: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9c78: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9c7c: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9c80: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9c84: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9c88: 0x46050034  c.lt.s      $f0, $f5
        fcr31 = (FPU_C_OLT_S(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9c8c: 0x45020080  bc1fl       . + 4 + (0x80 << 2)
        {
            const bool branch_taken_0x13b9c8c = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9c8c) {
                // 0x13b9c90: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b9e90;
            }
        }
        // 0x13b9c94: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b9c98: 0x0  nop
        // NOP
        // 0x13b9c9c: 0x0  nop
        // NOP
        // 0x13b9ca0: 0x46002003  div.s       $f0, $f4, $f0
        f0 = ps2FpuDivide(fcr31, f4, f0);
        // 0x13b9ca4: 0xd8e50000  lqc2        $vf5, 0x0($a3)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9ca8: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9cac: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9cb0: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9cb4: 0xf8e50000  sqc2        $vf5, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v5));
        // 0x13b9cb8: 0xd8e80000  lqc2        $vf8, 0x0($a3)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9cbc: 0xd92e0000  lqc2        $vf14, 0x0($t1)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 0)));
        // 0x13b9cc0: 0xd92f0010  lqc2        $vf15, 0x10($t1)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 16)));
        // 0x13b9cc4: 0xd9300020  lqc2        $vf16, 0x20($t1)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 32)));
        // 0x13b9cc8: 0xd9310030  lqc2        $vf17, 0x30($t1)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 48)));
        // 0x13b9ccc: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9cd0: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9cd4: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9cd8: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9cdc: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b9ce0: 0x10000053  b           . + 4 + (0x53 << 2)
        {
            const bool branch_taken_0x13b9ce0 = (RU64(0) == RU64(0));
            // 0x13b9ce4: 0x60202d  daddu       $a0, $v1, $zero (Delay Slot)
            WU64(4, (uint64_t)RU64(3) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9ce0) {
                goto label_13b9e30;
            }
        }
    label_13b9ce8:
        // 0x13b9ce8: 0xd8680000  lqc2        $vf8, 0x0($v1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9cec: 0xd94e0000  lqc2        $vf14, 0x0($t2)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 0)));
        // 0x13b9cf0: 0xd94f0010  lqc2        $vf15, 0x10($t2)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 16)));
        // 0x13b9cf4: 0xd9500020  lqc2        $vf16, 0x20($t2)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 32)));
        // 0x13b9cf8: 0xd9510030  lqc2        $vf17, 0x30($t2)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(10), 48)));
        // 0x13b9cfc: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d00: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d04: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d08: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9d0c: 0xf8ea0000  sqc2        $vf10, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v10));
        // 0x13b9d10: 0xc7a001d8  lwc1        $f0, 0x1D8($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 472)); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        // 0x13b9d14: 0x46010034  c.lt.s      $f0, $f1
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9d18: 0x4500001f  bc1f        . + 4 + (0x1F << 2)
        {
            const bool branch_taken_0x13b9d18 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9d18) {
                goto label_13b9d98;
            }
        }
        // 0x13b9d20: 0xd8870000  lqc2        $vf7, 0x0($a0)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(4), 0)));
        // 0x13b9d24: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9d28: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9d2c: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9d30: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9d34: 0x46050034  c.lt.s      $f0, $f5
        fcr31 = (FPU_C_OLT_S(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9d38: 0x45020055  bc1fl       . + 4 + (0x55 << 2)
        {
            const bool branch_taken_0x13b9d38 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9d38) {
                // 0x13b9d3c: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b9e90;
            }
        }
        // 0x13b9d40: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b9d44: 0x0  nop
        // NOP
        // 0x13b9d48: 0x0  nop
        // NOP
        // 0x13b9d4c: 0x46002003  div.s       $f0, $f4, $f0
        f0 = ps2FpuDivide(fcr31, f4, f0);
        // 0x13b9d50: 0xd8e50000  lqc2        $vf5, 0x0($a3)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9d54: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9d58: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9d5c: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9d60: 0xf8e50000  sqc2        $vf5, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v5));
        // 0x13b9d64: 0xd8e80000  lqc2        $vf8, 0x0($a3)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9d68: 0xd92e0000  lqc2        $vf14, 0x0($t1)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 0)));
        // 0x13b9d6c: 0xd92f0010  lqc2        $vf15, 0x10($t1)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 16)));
        // 0x13b9d70: 0xd9300020  lqc2        $vf16, 0x20($t1)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 32)));
        // 0x13b9d74: 0xd9310030  lqc2        $vf17, 0x30($t1)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(9), 48)));
        // 0x13b9d78: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d7c: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d80: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9d84: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9d88: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b9d8c: 0x10000028  b           . + 4 + (0x28 << 2)
        {
            const bool branch_taken_0x13b9d8c = (RU64(0) == RU64(0));
            // 0x13b9d90: 0x60202d  daddu       $a0, $v1, $zero (Delay Slot)
            WU64(4, (uint64_t)RU64(3) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9d8c) {
                goto label_13b9e30;
            }
        }
        // 0x13b9d94: 0x0  nop
        // NOP
    label_13b9d98:
        // 0x13b9d98: 0xd8680000  lqc2        $vf8, 0x0($v1)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9d9c: 0xd9ce0000  lqc2        $vf14, 0x0($t6)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(14), 0)));
        // 0x13b9da0: 0xd9cf0010  lqc2        $vf15, 0x10($t6)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(14), 16)));
        // 0x13b9da4: 0xd9d00020  lqc2        $vf16, 0x20($t6)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(14), 32)));
        // 0x13b9da8: 0xd9d10030  lqc2        $vf17, 0x30($t6)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(14), 48)));
        // 0x13b9dac: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9db0: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9db4: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9db8: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9dbc: 0xf8ea0000  sqc2        $vf10, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v10));
        // 0x13b9dc0: 0xda270000  lqc2        $vf7, 0x0($s1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(17), 0)));
        // 0x13b9dc4: 0x4b8739ea  vmul.xy     $vf7, $vf7, $vf7
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9dc8: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9dcc: 0x48223800  qmfc2.ni    $v0, $vf7
        WVEC(2, _mm_castps_si128(v7));
        // 0x13b9dd0: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9dd4: 0x46050034  c.lt.s      $f0, $f5
        fcr31 = (FPU_C_OLT_S(f0, f5)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9dd8: 0x4502002d  bc1fl       . + 4 + (0x2D << 2)
        {
            const bool branch_taken_0x13b9dd8 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9dd8) {
                // 0x13b9ddc: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b9e90;
            }
        }
        // 0x13b9de0: 0x46000004  c1          0x4
        f0 = ps2FpuSqrt(fcr31, f0);
        // 0x13b9de4: 0x0  nop
        // NOP
        // 0x13b9de8: 0x0  nop
        // NOP
        // 0x13b9dec: 0x46002003  div.s       $f0, $f4, $f0
        f0 = ps2FpuDivide(fcr31, f4, f0);
        // 0x13b9df0: 0xd8e50000  lqc2        $vf5, 0x0($a3)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9df4: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9df8: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9dfc: 0x4b862958  vmulx.xy    $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e00: 0xf8e50000  sqc2        $vf5, 0x0($a3)
        WRITE128(ADD32(RU32(7), 0), _mm_castps_si128(v5));
        // 0x13b9e04: 0xd8e80000  lqc2        $vf8, 0x0($a3)
        v8 = _mm_castsi128_ps(READ128(ADD32(RU32(7), 0)));
        // 0x13b9e08: 0xd98e0000  lqc2        $vf14, 0x0($t4)
        v14 = _mm_castsi128_ps(READ128(ADD32(RU32(12), 0)));
        // 0x13b9e0c: 0xd98f0010  lqc2        $vf15, 0x10($t4)
        v15 = _mm_castsi128_ps(READ128(ADD32(RU32(12), 16)));
        // 0x13b9e10: 0xd9900020  lqc2        $vf16, 0x20($t4)
        v16 = _mm_castsi128_ps(READ128(ADD32(RU32(12), 32)));
        // 0x13b9e14: 0xd9910030  lqc2        $vf17, 0x30($t4)
        v17 = _mm_castsi128_ps(READ128(ADD32(RU32(12), 48)));
        // 0x13b9e18: 0x4be871bc  vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9e1c: 0x4be878bd  vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9e20: 0x4be880be  vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // 0x13b9e24: 0x4be88a8b  vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(v17, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e28: 0xf86a0000  sqc2        $vf10, 0x0($v1)
        WRITE128(ADD32(RU32(3), 0), _mm_castps_si128(v10));
        // 0x13b9e2c: 0x60202d  daddu       $a0, $v1, $zero
        WU64(4, (uint64_t)RU64(3) + (uint64_t)RU64(0));
    label_13b9e30:
        // 0x13b9e30: 0x27a601e0  addiu       $a2, $sp, 0x1E0
        WS32(6, (int32_t)ADD32(RU32(29), 480));
        // 0x13b9e34: 0xd8650000  lqc2        $vf5, 0x0($v1)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(3), 0)));
        // 0x13b9e38: 0xd8a60000  lqc2        $vf6, 0x0($a1)
        v6 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9e3c: 0x4be62a2c  vsub.xyzw   $vf8, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e40: 0x4bc841ea  vmul.xyz    $vf7, $vf8, $vf8
        { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e44: 0x4b0739c1  vaddy.x     $vf7, $vf7, $vf7y
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e48: 0x4b0739c2  vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e4c: 0x4a6703be  vrsqrt      $Q, $vf0w, $vf7x
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
        // 0x13b9e50: 0xf8c80000  sqc2        $vf8, 0x0($a2)
        WRITE128(ADD32(RU32(6), 0), _mm_castps_si128(v8));
        // 0x13b9e54: 0x4a0003bf  vwaitq
        // VWAITQ (Q already resolved in this runtime)
        // 0x13b9e58: 0x4842b000  cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); WS32(2, bits); }
        // 0x13b9e5c: 0x44820000  mtc1        $v0, $f0
        { uint32_t bits = RU32(2); std::memcpy(&f0, &bits, sizeof(bits)); }
        // 0x13b9e60: 0x46030034  c.lt.s      $f0, $f3
        fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        // 0x13b9e64: 0x4502000a  bc1fl       . + 4 + (0xA << 2)
        {
            const bool branch_taken_0x13b9e64 = (!(fcr31 & 0x800000));
            if (branch_taken_0x13b9e64) {
                // 0x13b9e68: 0x256bffff  addiu       $t3, $t3, -0x1 (Delay Slot)
                WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
                goto label_13b9e90;
            }
        }
        // 0x13b9e6c: 0x46001002  mul.s       $f0, $f2, $f0
        f0 = FPU_MUL_S(f2, f0);
        // 0x13b9e70: 0xd8a70000  lqc2        $vf7, 0x0($a1)
        v7 = _mm_castsi128_ps(READ128(ADD32(RU32(5), 0)));
        // 0x13b9e74: 0xd8c50000  lqc2        $vf5, 0x0($a2)
        v5 = _mm_castsi128_ps(READ128(ADD32(RU32(6), 0)));
        // 0x13b9e78: 0x44080000  mfc1        $t0, $f0
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); WS32(8, bits); }
        // 0x13b9e7c: 0x48a83000  qmtc2.ni    $t0, $vf6
        v6 = _mm_castsi128_ps(RVEC(8));
        // 0x13b9e80: 0x4bc62958  vmulx.xyz   $vf5, $vf5, $vf6x
        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e84: 0x4bc539e8  vadd.xyz    $vf7, $vf7, $vf5
        { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // 0x13b9e88: 0xf8870000  sqc2        $vf7, 0x0($a0)
        WRITE128(ADD32(RU32(4), 0), _mm_castps_si128(v7));
        // 0x13b9e8c: 0x256bffff  addiu       $t3, $t3, -0x1
        WS32(11, (int32_t)ADD32(RU32(11), 4294967295));
    label_13b9e90:
        // 0x13b9e90: 0x1574ff09  bne         $t3, $s4, . + 4 + (-0xF7 << 2)
        {
            const bool branch_taken_0x13b9e90 = (RU64(11) != RU64(20));
            // 0x13b9e94: 0x60282d  daddu       $a1, $v1, $zero (Delay Slot)
            WU64(5, (uint64_t)RU64(3) + (uint64_t)RU64(0));
            if (branch_taken_0x13b9e90) {
                goto label_13b9ab8;
            }
        }
    label_13b9e98:
        // 0x13b9e98: 0x25adffff  addiu       $t5, $t5, -0x1
        WS32(13, (int32_t)ADD32(RU32(13), 4294967295));
        // 0x13b9e9c: 0x15b4fef0  bne         $t5, $s4, . + 4 + (-0x110 << 2)
        {
            const bool branch_taken_0x13b9e9c = (RU64(13) != RU64(20));
            // 0x13b9ea0: 0x26730018  addiu       $s3, $s3, 0x18 (Delay Slot)
            WS32(19, (int32_t)ADD32(RU32(19), 24));
            if (branch_taken_0x13b9e9c) {
                goto label_13b9a60;
            }
        }
    label_13b9ea4:
        // 0x13b9ea4: 0xdfb00230  ld          $s0, 0x230($sp)
        WU64(16, READ64(ADD32(RU32(29), 560)));
    label_13b9ea8:
        // 0x13b9ea8: 0xdfb10238  ld          $s1, 0x238($sp)
        WU64(17, READ64(ADD32(RU32(29), 568)));
        // 0x13b9eac: 0xdfb20240  ld          $s2, 0x240($sp)
        WU64(18, READ64(ADD32(RU32(29), 576)));
        // 0x13b9eb0: 0xdfb30248  ld          $s3, 0x248($sp)
        WU64(19, READ64(ADD32(RU32(29), 584)));
        // 0x13b9eb4: 0xdfb40250  ld          $s4, 0x250($sp)
        WU64(20, READ64(ADD32(RU32(29), 592)));
        // 0x13b9eb8: 0xdfb50258  ld          $s5, 0x258($sp)
        WU64(21, READ64(ADD32(RU32(29), 600)));
        // 0x13b9ebc: 0xdfb60260  ld          $s6, 0x260($sp)
        WU64(22, READ64(ADD32(RU32(29), 608)));
        // 0x13b9ec0: 0xdfb70268  ld          $s7, 0x268($sp)
        WU64(23, READ64(ADD32(RU32(29), 616)));
        // 0x13b9ec4: 0xdfbe0270  ld          $fp, 0x270($sp)
        WU64(30, READ64(ADD32(RU32(29), 624)));
        // 0x13b9ec8: 0xdfbf0278  ld          $ra, 0x278($sp)
        WU64(31, READ64(ADD32(RU32(29), 632)));
        // 0x13b9ecc: 0xc7b802a0  lwc1        $f24, 0x2A0($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 672)); float f; std::memcpy(&f, &bits, sizeof(f)); f24 = f; }
        // 0x13b9ed0: 0xc7b70298  lwc1        $f23, 0x298($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 664)); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
        // 0x13b9ed4: 0xc7b60290  lwc1        $f22, 0x290($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 656)); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
        // 0x13b9ed8: 0xc7b50288  lwc1        $f21, 0x288($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 648)); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
        // 0x13b9edc: 0xc7b40280  lwc1        $f20, 0x280($sp)
        { uint32_t bits = READ32(ADD32(RU32(29), 640)); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
        // 0x13b9ee0: 0x3e00008  jr          $ra
        {
            const uint32_t jumpTarget = RU32(31);
            // 0x13b9ee4: 0x27bd02b0  addiu       $sp, $sp, 0x2B0 (Delay Slot)
            WS32(29, (int32_t)ADD32(RU32(29), 688));
            goto lift_done;
        }
    lift_done:
        LIFT_STORE
        ctx->pc = static_cast<uint32_t>(r31);
    }

#undef RU32
#undef RS32
#undef RU64
#undef RS64
#undef RVEC
#undef WS32
#undef WU64
#undef WVEC
#undef LIFT_LOAD
#undef LIFT_STORE
#undef LIFT_CALL
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

SOTC_DECOMP_FUNCTION(0x013B8DC8u, gamecore_sub_013B8DC8, decomp_gc13b8dc8::strands);
