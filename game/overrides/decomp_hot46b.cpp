// Round 46 (SOTC_HOTLIST46=0 puts it back to the recompiled code; the table
// entry alone with SOTC_DECOMP_SKIP=gamecore_sub_01391E30): the horse's
// (Agro's) body step that comes right after nico_horseBodyExpression,
// transcribed from the generator's code (build/recomp, build/runner's VU0
// statements).
//
// - gamecore_sub_01391E30 (0x1391e30): a0 the horse's object, a1 its control
//   work s7 (s7 + 0x1c0: the 64-bit control flags, s7 + 0x80 = fp: the
//   steering record, fp + 8 the steer input). s4 = GetSCRCharaAIWorkPtr()
//   (the horse's AI work: +0x41c4 its tuning block, +0x41c8.. timers and
//   latches), the layout class (sp + 0x234) and the character class (sp +
//   0x238) of the object. The phases:
//   1. the rider (character +0x960 another object): the flags keep only
//      three control bits; the steer input is cleared when the rider's
//      definition (+0x750) is in the table at a relocated global.
//   2. the ride timer +0x41c8 (counts while +0x530 for the five "riding"
//      definitions), the layout position copied to +0x41d0.
//   3. the latches +0x41e0.., the three cool-downs +0x4210..+0x4218 and
//      the stop timer +0x421c (a control bit set when it starts), +0x41f0
//      (gamecore_sub_01394F18: the pad) with its bit.
//   4. the pending flags +0x4208 merged into the control flags; sp + 0x240
//      = 1 while bits outside eleven definitions' bits are set.
//   5. the steer input f21 for two definitions: the input's dead zones
//      (soft-float doubles: fptodp / dpcmp / dpsub / dpadd / dptofp),
//      getValueInterp to last frame's (+0x41ec), clamped to [-1, 1]; the
//      speed check against tuning +0xa8 (a control bit, or the steering
//      record cleared).
//   6. the character's control definition table (+0x750, handle words):
//      CheckControlDef / checkScriptControlDef per 16-byte entry; sp +
//      0x250 the last entry whose action is none of 22 globals.
//   7. +0x41fc / +0x41f8: the definition entry's action against two globals
//      (latched, a control bit while latched).
//   8. the passage line under the horse (getPassageWork /
//      getLineDataByAreaIdx): the lane kind s1 (1 or 2).
//   9. the ground's geometry attribute (nico_GetGeometryPointerAttr of the
//      layout's geometry pointer, +0xc0): kinds 0x08 / 0x0c / 0x10 run
//      gamecore_sub_01393E58 with tuning +0x80 / +0x84 / +0x88 and clear
//      their bits.
//  10. unless sp + 0x240 or the horse rides itself, the probes: the body's
//      heading from the node quaternion (GetMotionNodeQuaternionPos, VU0),
//      its side vector, three rays forward from the layout position;
//      the objects of the obstacle group inside the boundary
//      (CheckCollisionInsideOfObjectBoundary, at most 32), the rays
//      against them (ClipCollisionDirectCallBackObjAry, or
//      ClipCollisionDirectCallBack against the group), the ground wall
//      test (attribute 0x04, GetTurnYAngleFromGlobalDir), the centre ray,
//      the edge links (ReconstructCollisionEdgeLinkDBVWallFinder,
//      ClipEdgeLinkDBFinder).
//  11. the stop decisions by definition (gamecore_sub_01394580 /
//      gamecore_sub_01395960 with the tuning block's thresholds, the
//      slope / cliff test gamecore_sub_01395BC8), sp + 0x25c the result.
//  12. the turn toward the target angle +0x4224 (iosGetATan2fExact of the
//      body's heading, adjustAnglePI, gamecore_sub_01394150), the stop
//      latch +0x4200, the steer input written back to +0x41ec / fp + 8.
//   v0 at the return as the code leaves it (1, 0, 0x30000, the tuning block
//   pointer, dpcmp's result or what phase 9 left), the FPU registers, the
//   condition bit and the VU0 registers as the code leaves them (the FPU
//   statements on the context's registers, the VU0 blocks the generator's
//   statements).
//
// Not as the original: the scratch GPRs (only the call arguments and v0 at
// the return are set; HI / LO as the handle decodes' mult leaves them); the
// callee-saved GPRs are never changed in the context (the code restores
// every one from its frame), f20-f22 are worked on in the context and put
// back from the frame as the epilogue does; the frame's saves and scratch
// are written as the code writes them. The loops' checkpoints are left out.
// Every relocated lui / lw pair is read from the live code words at its own
// sites (where two paths reach one lw with different lui sites, the path's
// lui), as the generated code does.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46b
{
    using sotc_decomp::callGuest;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    constexpr uint32_t kSelf = 0x01391E30u;
    // Same module: constant jals.
    constexpr uint32_t kAiWork = 0x013CFAE8u, kPad = 0x01394F18u, kValueInterp = 0x01447178u, kPassageWork = 0x01431CE0u;
    constexpr uint32_t kLineData = 0x014333B0u, kGeomAttr = 0x0134C400u, kGroundKind = 0x01393E58u, kStopFront = 0x01394580u;
    constexpr uint32_t kStopCheck = 0x01395960u, kSlope = 0x01395BC8u, kAdjustPi = 0x01446F80u, kTurn = 0x01394150u;
    // The module's data (not relocated).
    constexpr uint32_t kFrameTime = 0x014657E0u;   // the frame's time step
    constexpr uint32_t kTimeBase = 0x01465650u;    // + 0x190 = kFrameTime
    constexpr uint32_t kObstacles = 0x01465884u;   // the obstacle group
    constexpr uint32_t kCoolDone = 0x014688C8u;    // the cool-downs' "done" value
    constexpr uint32_t kNoTarget = 0x014688CCu;    // +0x4224's "no target angle"
    constexpr uint32_t kDeadLo = 0x014688D0u, kDeadHi = 0x014688D8u, kDead2Lo = 0x014688E0u, kDead2Hi = 0x014688E8u; // doubles
    constexpr uint32_t kRoundUp = 0x014688F0u, kRoundDown = 0x014688F8u, kStopEps = 0x01468900u;                   // doubles
    constexpr uint32_t kClipObjCb = 0x013958B8u, kClipCentreCb = 0x01395830u; // the clip callbacks
    constexpr uint32_t kRadToDeg = 0x42652EE0u;                               // 57.29578

    struct Step
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;     // the frame (entry sp - 0x2e0)
        uint64_t s7;     // a1: the control work
        uint32_t fp;     // s7 + 0x80: the steering record
        uint32_t obj;    // a0 (sp + 0x230)
        uint32_t s4;     // the AI work
        uint32_t layout; // sp + 0x234
        uint32_t chara;  // sp + 0x238
        uint64_t v0 = 0; // v0 as the code leaves it at the return

        // ---- memory, registers ---------------------------------------------
        uint32_t u32(uint32_t a) { return READ32(a); }
        void put32(uint32_t a, uint32_t v) { WRITE32(a, v); }
        uint64_t get64(uint32_t a) { return READ64(a); }
        void put64(uint32_t a, uint64_t v) { WRITE64(a, v); }
        uint32_t reloc(uint32_t hi, uint32_t lo) const { return sotc_decomp::relocated(rdram, hi, lo); }
        // The word at a relocated global (lui hi; lw lo).
        uint32_t global(uint32_t hi, uint32_t lo) { return READ32(reloc(hi, lo)); }
        // A relocated lui's own value (lui hi), as the register holds it.
        uint64_t luiValue(uint32_t hi) const { return sx((sotc_decomp::codeWord(rdram, hi) & 0xFFFFu) << 16); }
        void lwc1(int r, uint32_t a)
        {
            const uint32_t b = READ32(a);
            std::memcpy(&ctx->f[r], &b, 4);
        }
        void swc1(int r, uint32_t a)
        {
            uint32_t b;
            std::memcpy(&b, &ctx->f[r], 4);
            WRITE32(a, b);
        }
        void mtc1(int r, uint32_t bits) { std::memcpy(&ctx->f[r], &bits, 4); }
        float &f(int r) { return ctx->f[r]; }
        // c.cond.s: fcr31's condition bit as the original sets it.
        bool cond(bool c)
        {
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        }
        void arg(int r, uint64_t v) { SET_GPR_U64(ctx, r, v); }
        void call(uint32_t target, uint32_t ra) { callGuest(rdram, ctx, runtime, target, ra, sp); }
        // A jal whose target the game patched at link time.
        void callAt(uint32_t site) { call(sotc_decomp::jalTarget(rdram, site), site + 8u); }
        uint64_t rv() const { return GPR_U64(ctx, 2); }
        uint32_t rvw() const { return GPR_U32(ctx, 2); }
        // mfc1 gpr, f; qmtc2 gpr, vf: the vector register takes the GPR's 128 bits.
        void vfFromGpr(int vf, int gpr, int fr)
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->f[fr], 4);
            SET_GPR_U32(ctx, gpr, bits);
            ctx->vu0_vf[vf] = _mm_castsi128_ps(GPR_VEC(ctx, gpr));
        }

        // A handle word at p: the low 18 bits back to the owner record, the
        // next 12 an index into the owner's array (+0x10 base, +8 -> +0xc
        // stride); 0 for none or the null record (the word at nullAt).
        uint32_t handle(uint32_t p, uint32_t nullAt)
        {
            const uint32_t w = READ32(p), back = w & 0x3FFFFu;
            if (!back)
                return 0u;
            const uint32_t owner = READ32(p - back);
            if (!owner || owner == READ32(nullAt))
                return 0u;
            const int64_t prod = static_cast<int64_t>(static_cast<int32_t>((w >> 18) & 0xFFFu)) *
                                 static_cast<int64_t>(static_cast<int32_t>(READ32(READ32(owner + 8u) + 0xCu)));
            ctx->lo = sx(static_cast<uint32_t>(prod));
            ctx->hi = sx(static_cast<uint32_t>(static_cast<uint64_t>(prod) >> 32));
            return READ32(owner + 0x10u) + static_cast<uint32_t>(prod);
        }

        // The control flags (s7 + 0x1c0) and a definition's bit (its record's +4).
        uint32_t flagsAt() const { return static_cast<uint32_t>(s7) + 0x1C0u; }
        uint64_t bitOf(uint32_t record) { return 1ull << (READ32(record + 4u) & 0x3Fu); }
        uint64_t bitAt(uint32_t hi, uint32_t lo) { return bitOf(global(hi, lo)); }
        void setBit(uint32_t hi, uint32_t lo)
        {
            const uint64_t flags = get64(flagsAt());
            put64(flagsAt(), flags | bitAt(hi, lo));
        }
        void clearBit(uint32_t hi, uint32_t lo)
        {
            const uint64_t flags = get64(flagsAt());
            put64(flagsAt(), flags & ~bitAt(hi, lo));
        }
        uint32_t definition() { return u32(chara + 0x750u); }
        uint32_t tuning() { return u32(s4 + 0x41C4u); }
        // fp + 0x34..0x44 and fp + 4: the steering record's state.
        void clearSteerState(bool withInput)
        {
            put32(fp + 0x34u, 0u);
            put32(fp + 0x38u, 0u);
            put32(fp + 0x3Cu, 0u);
            put32(fp + 0x40u, 0u);
            put32(fp + 0x44u, 0u);
            if (withInput)
                put32(fp + 4u, 0u);
        }

        // ---- the VU0 blocks (the generator's statements) ---------------------
        // The quaternion at src's z axis (2(xz + yw), 2(yz - xw), 1 - 2(x^2 + y^2), 0) to dst.
        void quatAxis(uint32_t src, uint32_t dst)
        {
            ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(src));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[4], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[4], ctx->vu0_vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[8] = PS2_VBLEND(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[8], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[6]));
        }
        // xyz at a scaled to length 1 (vrsqrt), in place.
        void normalize(uint32_t a)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(a, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        // dst.xyz = src.xyz * f(fr) (mfc1 / qmtc2 through the GPR gpr; vmulx.xyz).
        void scale(uint32_t src, uint32_t dst, int gpr, int fr)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(src));
            vfFromGpr(6, gpr, fr);
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        // a.xyz += b.xyz (vadd.xyz vf5, vf5, vf6).
        void addXyz(uint32_t a, uint32_t b)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
            { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(a, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        // a = a +- b, all four lanes (vadd / vsub.xyzw vf7, vf5, vf6).
        void addAll(uint32_t a, uint32_t b, bool sub)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
            { __m128 res = sub ? PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]) : PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            WRITE128(a, _mm_castps_si128(ctx->vu0_vf[7]));
        }

        void prologue(uint64_t a0, uint64_t ra);
        void rider();
        void rideTimerAndPosition();
        void latchesAndTimers(bool &stopping);
        void pendingFlags();
        void steerInput(bool stopping);
        void steerSpeedCheck(bool stopping);
        uint32_t controlDefinitions(bool &none);
        void definitionLatches();
        uint32_t passageLane();
        void groundKind(uint32_t lane);
        uint64_t probes(uint32_t defHi);
        void stopCheck(uint32_t off, uint32_t off16, uint64_t centre, uint32_t ra);
        bool entryActionIs(uint32_t nullHi, uint32_t nullLo, uint32_t actHi, uint32_t actLo);
        void stopSteering(uint32_t hi1, uint32_t lo1, uint32_t hi2, uint32_t lo2, uint32_t hi3, uint32_t lo3);
        bool stopLimits(bool g);
        uint32_t stopByDefinition(uint32_t slope, uint64_t centre);
        bool slopeAhead(uint32_t plane);
        uint32_t turn();
        void stopLatch();
        void finish(uint64_t ra, uint64_t entrySp);
        void run(uint64_t a0, uint64_t ra, uint64_t entrySp);
    };

    void Step::prologue(uint64_t a0, uint64_t ra)
    {
        put64(sp + 0x288u, GPR_U64(ctx, 19));
        put64(sp + 0x290u, GPR_U64(ctx, 20));
        put64(sp + 0x2A8u, GPR_U64(ctx, 23));
        put64(sp + 0x2B0u, GPR_U64(ctx, 30));
        put32(sp + 0x230u, static_cast<uint32_t>(a0));
        put64(sp + 0x270u, GPR_U64(ctx, 16));
        put64(sp + 0x278u, GPR_U64(ctx, 17));
        put64(sp + 0x280u, GPR_U64(ctx, 18));
        put64(sp + 0x298u, GPR_U64(ctx, 21));
        put64(sp + 0x2A0u, GPR_U64(ctx, 22));
        put64(sp + 0x2B8u, ra);
        swc1(22, sp + 0x2D0u);
        swc1(21, sp + 0x2C8u);
        swc1(20, sp + 0x2C0u);
    }

    // ---- 1. the rider ---------------------------------------------------------
    void Step::rider()
    {
        // Only three control bits kept while someone else rides.
        const uint32_t g1 = global(0x01391EB4u, 0x01391EBCu), g2 = global(0x01391EB8u, 0x01391EC4u), g3 = global(0x01391EC0u, 0x01391ECCu);
        const uint32_t table = reloc(0x01391ED8u, 0x01391EE0u); // +0xc count, +0x10 handle words
        const uint64_t keep = bitOf(g1) | bitOf(g2) | bitOf(g3);
        put64(flagsAt(), get64(flagsAt()) & keep);
        const int32_t count = static_cast<int32_t>(u32(table + 0xCu));
        uint32_t words = u32(table + 0x10u);
        arg(4, sx(u32(chara + 0x960u)));
        callAt(0x01391F14u); // GetCharacterClass(rider)
        if (count <= 0)
            return;
        const uint32_t riderDef = u32(rvw() + 0x750u);
        const uint32_t nullAt = reloc(0x01391F28u, 0x01391F30u);
        for (int32_t i = 0; i < count;)
        {
            const uint32_t t = handle(words, nullAt);
            words += 4u;
            ++i;
            if (riderDef == t)
            {
                put32(fp + 8u, 0u); // the steer input
                return;
            }
        }
    }

    // ---- 2. the ride timer, the position --------------------------------------
    void Step::rideTimerAndPosition()
    {
        lwc1(21, fp + 8u); // the steer input (f21 from here on)
        uint32_t pos;
        if (obj == u32(chara + 0x960u))
        {
            put32(s4 + 0x41C8u, 0u);
            pos = u32(layout + 0x134u);
        }
        else
        {
            const uint32_t def = definition();
            const bool riding = global(0x01391FF4u, 0x01391FF8u) == def || global(0x01392004u, 0x01392008u) == def ||
                                global(0x01392010u, 0x01392014u) == def || global(0x0139201Cu, 0x01392020u) == def ||
                                global(0x01392028u, 0x0139202Cu) == def;
            if (riding && static_cast<int8_t>(READ8(chara + 0x530u)) != 0)
            {
                lwc1(0, s4 + 0x41C8u);
                lwc1(1, kFrameTime);
                f(0) = FPU_ADD_S(f(0), f(1));
                swc1(0, s4 + 0x41C8u);
                return;
            }
            put32(s4 + 0x41C8u, 0u);
            arg(4, sx(obj));
            callAt(0x01392064u); // GetLayoutClass
            pos = u32(rvw() + 0x134u);
        }
        WRITE128(s4 + 0x41D0u, READ128(pos)); // the layout position
    }

    // ---- 3. the latches and timers --------------------------------------------
    void Step::latchesAndTimers(bool &stopping)
    {
        const uint32_t was = u32(s4 + 0x41E4u);
        put32(s4 + 0x41E4u, 0u);
        put32(s4 + 0x41E0u, was);
        {
            const uint32_t def = definition();
            if (global(0x0139207Cu, 0x0139208Cu) != def && global(0x01392098u, 0x0139209Cu) != def)
            {
                put32(s4 + 0x41E0u, 0u);
                put32(s4 + 0x41E8u, 0u);
            }
        }
        // The three cool-downs +0x4210..: counting while >= 0, "done" past tuning +0xa4.
        mtc1(3, 0u);
        lwc1(2, kCoolDone);
        uint32_t at = s4 + 0x4210u;
        for (int32_t n = 2; n >= 0; --n, at += 4u)
        {
            lwc1(1, at);
            if (!cond(FPU_C_OLE_S(f(3), f(1))))
                continue;
            lwc1(0, kTimeBase + 0x190u);
            f(0) = FPU_ADD_S(f(1), f(0));
            swc1(0, at);
            lwc1(1, tuning() + 0xA4u);
            if (cond(FPU_C_OLT_S(f(1), f(0))))
                swc1(2, at);
        }
        if (u32(fp + 0x34u) != 0u)
        {
            lwc1(0, s4 + 0x4214u);
            swc1(0, s4 + 0x4218u);
            lwc1(0, s4 + 0x4210u);
            swc1(0, s4 + 0x4214u);
            put32(s4 + 0x4210u, 0u);
        }
        // The stop timer +0x421c.
        lwc1(0, kCoolDone);
        lwc1(1, s4 + 0x4218u);
        if (!cond(FPU_C_EQ_S(f(1), f(0))))
        {
            const uint32_t t = tuning();
            lwc1(0, t + 0xA4u);
            if (cond(FPU_C_OLE_S(f(1), f(0))))
            {
                lwc1(0, t + 0xACu);
                swc1(0, s4 + 0x421Cu);
                setBit(0x0139215Cu, 0x01392168u);
            }
        }
        lwc1(1, s4 + 0x421Cu);
        mtc1(0, 0u);
        if (!cond(FPU_C_OLT_S(f(0), f(1))))
            swc1(0, s4 + 0x421Cu);
        else
        {
            stopping = true; // s3
            lwc1(0, kFrameTime);
            f(0) = FPU_SUB_S(f(1), f(0));
            swc1(0, s4 + 0x421Cu);
        }
        // +0x41f0: the pad's hold time.
        if (obj == u32(chara + 0x960u))
        {
            put32(s4 + 0x41F0u, 0u);
            return;
        }
        arg(4, sx(obj));
        call(kPad, 0x013921C4u);
        if (rv() == 0u)
        {
            put32(s4 + 0x41F0u, 0u);
            return;
        }
        lwc1(1, s4 + 0x41F0u);
        lwc1(0, kFrameTime);
        const uint32_t t = tuning();
        f(1) = FPU_ADD_S(f(1), f(0));
        swc1(1, s4 + 0x41F0u);
        lwc1(0, t + 0x14u);
        if (!cond(FPU_C_OLT_S(f(0), f(1))))
            return;
        if (global(0x013921F4u, 0x013921F8u) != definition())
            setBit(0x01392204u, 0x0139220Cu);
    }

    // ---- 4. the pending flags -------------------------------------------------
    void Step::pendingFlags()
    {
        const uint64_t pending = get64(s4 + 0x4208u);
        if (pending == 0u)
            return;
        put32(sp + 0x240u, 1u);
        // Eleven definitions' bits (the code's order of loads).
        const uint32_t r0 = global(0x01392234u, 0x0139223Cu), r1 = global(0x01392238u, 0x01392264u), r2 = global(0x01392244u, 0x0139226Cu);
        const uint32_t r3 = global(0x01392248u, 0x01392274u), r4 = global(0x0139224Cu, 0x0139227Cu), r5 = global(0x01392254u, 0x01392280u);
        const uint32_t r6 = global(0x01392250u, 0x01392284u), r7 = global(0x01392258u, 0x01392288u);
        uint64_t mask = bitOf(r1) | bitOf(r0) | bitOf(r2) | bitOf(r3) | bitOf(r4) | bitOf(r6) | bitOf(r5) | bitOf(r7);
        const uint32_t r8 = global(0x01392260u, 0x013922C4u), r9 = global(0x01392270u, 0x013922D4u), r10 = global(0x01392278u, 0x013922E0u);
        const uint64_t b8 = bitOf(r8), b9 = bitOf(r9), b10 = bitOf(r10);
        const uint64_t flags = get64(flagsAt()) | pending;
        put64(flagsAt(), flags);
        put64(s4 + 0x4208u, 0u);
        mask |= b8 | b9;
        if ((get64(flagsAt()) & ~(mask | b10)) == 0u)
            put32(sp + 0x240u, 0u);
    }

    // ---- 5. the steer input ---------------------------------------------------
    void Step::steerInput(bool stopping)
    {
        if (obj == u32(chara + 0x960u))
            return;
        const uint32_t def = definition();
        if (!(global(0x0139234Cu, 0x01392354u) == def || global(0x0139235Cu, 0x01392360u) == def))
        {
            // The code goes on to the speed check's definition test with
            // v0 the first global again (its second global differs: none
            // of the two match).
            const uint32_t g = global(0x0139234Cu, 0x01392368u);
            if (g == def || global(0x01392570u, 0x01392574u) == def)
                steerSpeedCheck(stopping);
            return;
        }
        // Dead zones on |input| (soft-float doubles).
        lwc1(20, fp + 8u);
        uint64_t s1 = get64(kDeadLo);
        f(12) = FPU_ABS_S(f(20));
        callAt(0x01392378u); // fptodp
        arg(5, s1);
        arg(4, rv());
        callAt(0x01392388u); // dpcmp(|input|, dead zone)
        if (static_cast<int32_t>(rvw()) <= 0)
        {
            // Inside: eased toward 0 from last frame's.
            const uint32_t t = tuning();
            lwc1(15, kFrameTime);
            lwc1(14, t + 0x10u);
            mtc1(13, 0u);
            lwc1(12, s4 + 0x41ECu);
            call(kValueInterp, 0x01392528u);
            f(21) = FPU_MOV_S(f(0));
        }
        else
        {
            f(12) = FPU_MOV_S(f(20));
            mtc1(20, 0u);
            f(21) = FPU_MOV_S(f(20));
            callAt(0x013923A0u); // fptodp(input)
            arg(5, s1);
            uint64_t s0 = rv();
            mtc1(22, 0x3FB6DB6Du); // 1.4285715
            arg(4, s0);
            callAt(0x013923C0u); // dpcmp(input, dead zone)
            bool outside = static_cast<int32_t>(rvw()) > 0;
            if (!outside)
            {
                s1 = get64(kDeadHi);
                arg(4, s0);
                arg(5, s1);
                callAt(0x013923E0u); // dpcmp(input, -dead zone)
                outside = static_cast<int32_t>(rvw()) < 0;
            }
            if (outside)
            {
                arg(5, s1);
                arg(4, s0);
                callAt(0x013923F8u); // dpsub
                arg(4, rv());
                callAt(0x01392404u); // dptofp
                f(20) = FPU_MOV_S(f(0));
            }
            lwc1(12, s4 + 0x41ECu);
            s1 = get64(kDead2Lo);
            f(20) = FPU_MUL_S(f(20), f(22));
            callAt(0x0139241Cu); // fptodp(last frame's)
            s0 = rv();
            arg(5, s1);
            arg(4, s0);
            callAt(0x01392430u); // dpcmp
            outside = static_cast<int32_t>(rvw()) > 0;
            if (!outside)
            {
                s1 = get64(kDead2Hi);
                arg(4, s0);
                arg(5, s1);
                callAt(0x01392450u); // dpcmp
                outside = static_cast<int32_t>(rvw()) < 0;
            }
            if (outside)
            {
                arg(5, s1);
                arg(4, s0);
                callAt(0x01392468u); // dpsub
                arg(4, rv());
                callAt(0x01392474u); // dptofp
                f(21) = FPU_MOV_S(f(0));
            }
            const uint32_t t = tuning();
            f(12) = FPU_MUL_S(f(21), f(22));
            f(13) = FPU_MOV_S(f(20));
            lwc1(14, t + 0x10u);
            lwc1(15, kFrameTime);
            call(kValueInterp, 0x0139249Cu);
            mtc1(1, 0u);
            f(20) = ps2FpuDivide(ctx->fcr31, f(0), f(22));
            uint32_t round = 0u;
            if (cond(FPU_C_OLT_S(f(1), f(20))))
                round = kRoundUp;
            else if (cond(FPU_C_OLT_S(f(20), f(1))))
                round = kRoundDown;
            if (round)
            {
                f(12) = FPU_MOV_S(f(20));
                callAt(round == kRoundUp ? 0x013924B8u : 0x013924DCu); // fptodp
                arg(5, get64(round));
                arg(4, rv());
                callAt(0x013924F0u); // dpadd
                arg(4, rv());
                callAt(0x013924FCu); // dptofp
                f(20) = FPU_MOV_S(f(0));
            }
            f(21) = FPU_MOV_S(f(20));
        }
        // Clamped to [-1, 1].
        mtc1(0, 0xBF800000u);
        f(1) = FPU_MOV_S(f(0));
        if (!cond(FPU_C_OLE_S(f(21), f(1))))
            f(1) = FPU_MOV_S(f(21));
        mtc1(0, 0x3F800000u);
        f(21) = FPU_MOV_S(f(1));
        if (!cond(FPU_C_OLE_S(f(0), f(21))))
            f(0) = FPU_MOV_S(f(21));
        f(21) = FPU_MOV_S(f(0));
        const uint32_t def2 = definition();
        if (global(0x0139234Cu, 0x01392568u) == def2 || global(0x01392570u, 0x01392574u) == def2)
            steerSpeedCheck(stopping);
    }

    // The speed against tuning +0xa8 (sp + 0x23c: the character's +0x1c0 block).
    void Step::steerSpeedCheck(bool stopping)
    {
        if (stopping)
            return;
        const uint32_t t = tuning();
        lwc1(0, t + 0xA8u);
        lwc1(1, chara + 0x5C4u);
        const uint32_t block = u32(sp + 0x23Cu);
        f(1) = FPU_MUL_S(f(1), f(0));
        lwc1(2, block + 0x1Cu);
        if (cond(FPU_C_OLT_S(f(1), f(2))))
        {
            setBit(0x013925ACu, 0x013925B4u);
            return;
        }
        if (u32(fp + 0x38u) == 0u)
            return;
        mtc1(0, 0x3C23D70Au); // 0.01
        f(0) = FPU_SUB_S(f(1), f(0));
        if (!cond(FPU_C_OLT_S(f(0), f(2))))
            return;
        clearSteerState(true);
    }

    // ---- 6. the control definition table --------------------------------------
    // Returns sp + 0x250's entry; none: the character has no table (+0x750 0).
    uint32_t Step::controlDefinitions(bool &none)
    {
        arg(4, sx(obj));
        callAt(0x0139260Cu); // GetCharacterClass
        const uint64_t s6 = rv();
        const uint32_t tableWord = u32(static_cast<uint32_t>(s6) + 0x750u);
        if (tableWord == 0u)
        {
            none = true;
            return 0u;
        }
        // The entry count (the owner's +0xc), the entries (16 bytes each).
        int32_t count = 0;
        {
            const uint32_t back = u32(tableWord) & 0x3FFFFu;
            const uint32_t owner = back ? u32(tableWord - back) : 0u;
            if (owner && owner != u32(reloc(0x01392648u, 0x0139264Cu)))
                count = static_cast<int32_t>(u32(owner + 0xCu));
        }
        uint32_t entry = handle(tableWord, reloc(0x01392688u, 0x0139268Cu)); // s1
        const uint64_t flags = get64(flagsAt()) | get64(static_cast<uint32_t>(s7) + 0x1C8u);
        put32(sp + 0x250u, 0u);
        put64(sp + 0x248u, flags);
        if (count <= 0)
            return 0u;
        static const uint32_t kNoAction[][2] = {
            {0x01392804u, 0x01392808u}, {0x01392810u, 0x01392814u}, {0x0139281Cu, 0x01392820u}, {0x01392828u, 0x0139282Cu},
            {0x01392834u, 0x01392838u}, {0x01392840u, 0x01392844u}, {0x0139284Cu, 0x01392850u}, {0x01392858u, 0x0139285Cu},
            {0x01392864u, 0x01392868u}, {0x01392870u, 0x01392874u}, {0x0139287Cu, 0x01392880u}, {0x01392888u, 0x0139288Cu},
            {0x01392894u, 0x01392898u}, {0x013928A0u, 0x013928A4u}, {0x013928ACu, 0x013928B0u}, {0x013928B8u, 0x013928BCu},
            {0x013928C4u, 0x013928C8u}, {0x013928D0u, 0x013928D4u}, {0x013928E4u, 0x013928E8u}, {0x013928F0u, 0x013928F4u},
            {0x013928FCu, 0x01392900u},
        };
        for (int32_t left = count; left != 0; --left, entry += 0x10u)
        {
            arg(4, sx(obj));
            arg(6, s7);
            arg(5, s6);
            arg(7, sx(entry));
            callAt(0x013926F4u); // CheckControlDef(obj, chara, work, entry)
            const uint64_t checked = rv();
            const uint32_t condition = handle(entry + 4u, reloc(0x013926E4u, 0x01392728u));
            const uint32_t action = handle(entry + 8u, reloc(0x013926E4u, 0x01392778u));
            arg(4, get64(sp + 0x248u));
            arg(5, sx(condition));
            arg(6, checked);
            arg(7, sx(action));
            callAt(0x013927A8u); // checkScriptControlDef(flags, condition, checked, action)
            if (static_cast<int32_t>(rvw()) <= 0)
                continue;
            const uint32_t a = handle(entry + 8u, reloc(0x013926E4u, 0x013927D8u));
            bool listed = false;
            for (const auto &g : kNoAction)
                if (a == global(g[0], g[1]))
                {
                    listed = true;
                    break;
                }
            if (listed)
                continue;
            if (a != global(0x01392908u, 0x0139290Cu))
                put32(sp + 0x250u, entry);
        }
        return u32(sp + 0x250u);
    }

    // ---- 7. +0x41fc / +0x41f8 -------------------------------------------------
    void Step::definitionLatches()
    {
        struct Latch
        {
            uint32_t field, defHi1, defLo1, defHi2, defLo2, nullHi, nullLo, actHi, actLo, bitHi, bitLo;
        };
        static const Latch kLatches[2] = {
            {0x41FCu, 0x01392930u, 0x01392934u, 0x01392940u, 0x01392944u, 0x01392988u, 0x0139298Cu, 0x013929BCu, 0x013929C0u, 0x013929E8u, 0x013929F0u},
            {0x41F8u, 0x01392A20u, 0x01392A24u, 0x01392A2Cu, 0x01392A30u, 0x01392A70u, 0x01392A74u, 0x01392AA4u, 0x01392AA8u, 0x01392AC8u, 0x01392AD0u},
        };
        for (const Latch &l : kLatches)
        {
            const uint32_t def = definition();
            if (!(global(l.defHi1, l.defLo1) == def || global(l.defHi2, l.defLo2) == def))
            {
                put32(s4 + l.field, 0u);
                continue;
            }
            const uint32_t entry = u32(sp + 0x250u);
            if (entry != 0u && handle(entry + 4u, reloc(l.nullHi, l.nullLo)) == global(l.actHi, l.actLo))
            {
                put32(s4 + l.field, 1u);
                continue;
            }
            if (u32(s4 + l.field) != 0u)
                setBit(l.bitHi, l.bitLo);
        }
    }

    // ---- 8. the passage line's lane kind ----------------------------------------
    uint32_t Step::passageLane()
    {
        if (u32(s4 + 0x4204u) == 0u)
            return 0u;
        const uint32_t area = s4 + 0x10u;
        arg(4, sx(area));
        arg(5, 0u);
        call(kPassageWork, 0x01392B04u);
        const uint32_t work = rvw();
        if (!work)
            return 0u;
        arg(4, sx(area));
        arg(6, sx(u32(work + 0x1Cu)));
        arg(5, sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(READ16(work + 0x16u))))));
        call(kLineData, 0x01392B18u);
        const uint32_t line = rvw();
        if (!line)
            return 0u;
        const int16_t a = static_cast<int16_t>(READ16(line + 0x10u));
        if (a == 1)
            return 1u;
        const int16_t b = static_cast<int16_t>(READ16(line + 0x12u));
        if (b == 1)
            return 1u;
        if (a == 2 || b == 2)
            return 2u;
        return 0u;
    }

    // ---- 9. the ground's geometry attribute ------------------------------------
    // v0 as the code leaves it at 0x1392ef4 (the early return's).
    void Step::groundKind(uint32_t lane)
    {
        const uint32_t geometry = layout + 0xC0u; // s2
        if (u32(layout + 0x124u) == 0u)
        {
            v0 = 0u;
            return;
        }
        constexpr uint64_t kKindMask = 0x7C000000u;
        auto attr = [&](uint32_t ra)
        {
            arg(4, sx(geometry));
            call(kGeomAttr, ra);
            return rv() & kKindMask;
        };
        // gamecore_sub_01393E58(sp + 0x23c's block, fp, entry, work, f12 = tuning + off).
        auto groundCall = [&](uint32_t off, uint32_t ra)
        {
            const uint32_t t = tuning();
            arg(5, sx(fp));
            arg(4, sx(u32(sp + 0x23Cu)));
            arg(7, s7);
            arg(6, sx(u32(sp + 0x250u)));
            lwc1(12, t + off);
            call(kGroundKind, ra);
        };
        bool done = false; // the code's 0x1392e24 / 0x1392e28 join
        if (attr(0x01392B70u) == 0x08000000u || lane == 1u)
        {
            const uint32_t def = definition();
            if (global(0x01392B8Cu, 0x01392B90u) != def && global(0x01392B9Cu, 0x01392BA0u) != def)
            {
                groundCall(0x80u, 0x01392BC4u);
                put32(fp + 0x34u, 0u);
                put32(s4 + 0x41FCu, 0u);
                clearBit(0x01392BCCu, 0x01392BD8u);
                clearBit(0x01392BD4u, 0x01392BF4u);
            }
            clearBit(0x01392C0Cu, 0x01392C14u);
            const uint32_t entry = u32(sp + 0x250u);
            if (entry != 0u && handle(entry + 4u, reloc(0x01392C68u, 0x01392C6Cu)) == global(0x01392C9Cu, 0x01392CA0u))
                setBit(0x01392CACu, 0x01392CB4u);
            done = true;
        }
        if (!done && (attr(0x01392CD8u) == 0x0C000000u || lane == 2u))
        {
            const uint32_t def = definition();
            if (global(0x01392CF4u, 0x01392CF8u) != def && global(0x01392D04u, 0x01392D08u) != def)
            {
                groundCall(0x84u, 0x01392D2Cu);
                const uint32_t def2 = definition();
                if (global(0x01392D2Cu, 0x01392D34u) == def2 || global(0x01392D40u, 0x01392D44u) == def2 ||
                    global(0x01392D4Cu, 0x01392D50u) == def2 || global(0x01392D58u, 0x01392D5Cu) == def2)
                {
                    put32(fp + 0x34u, 0u);
                    put32(s4 + 0x41F8u, 0u);
                    clearBit(0x01392D6Cu, 0x01392D80u);
                    clearBit(0x01392D78u, 0x01392D9Cu);
                    clearBit(0x01392D7Cu, 0x01392DB4u);
                }
            }
            done = true;
        }
        if (!done && attr(0x01392DD8u) == 0x10000000u)
        {
            const uint32_t def = definition();
            if (global(0x01392DECu, 0x01392DF4u) != def && global(0x01392DFCu, 0x01392E00u) != def)
                groundCall(0x88u, 0x01392E24u);
        }
        // The kinds 0x08 / 0x0c: the entry's action bit cleared.
        uint64_t k = attr(0x01392E30u);
        if (k != 0x08000000u)
        {
            k = attr(0x01392E48u);
            if (k != 0x0C000000u)
            {
                v0 = k;
                return;
            }
        }
        v0 = k;
        const uint32_t entry = u32(sp + 0x250u);
        if (entry == 0u)
            return;
        const uint32_t action = handle(entry + 4u, reloc(0x01392E90u, 0x01392E94u));
        v0 = luiValue(0x01392EC4u);
        if (action != global(0x01392EC4u, 0x01392EC8u))
            return;
        const uint32_t bit = u32(action + 4u);
        v0 = sx(bit);
        put64(flagsAt(), get64(flagsAt()) & ~(1ull << (bit & 0x3Fu)));
    }

    // ---- 10. the probes -------------------------------------------------------
    // The frame: +0x20 / +0x70 / +0xc0 the clip results, +0xe0.. the three
    // rays' starts, +0x110.. their ends, +0x140 the heading, +0x150 the side
    // vector, +0x160 the node's quaternion, +0x170 the objects inside the
    // boundary, +0x1f0 the boundary's centre, +0x200 the wall's plane
    // quaternion, +0x210 a plane / the body's heading, +0x220 / +0x224 /
    // +0x228 / +0x22c the stop tests' outputs, +0x254 the ray that hit an
    // object (+1), +0x258 the edge link ray (+1), +0x25c the result, +0x260
    // the definition's wall test.
    // defHi: the lui of the first definition test (0x1392f18 or 0x1392f38 by the path).
    // Returns the centre ray's result (s3).
    uint64_t Step::probes(uint32_t defHi)
    {
        {
            const uint32_t g = global(defHi, 0x01392F3Cu);
            const uint32_t def = definition();
            put32(sp + 0x254u, 0u);
            put32(sp + 0x258u, 0u);
            put32(sp + 0x25Cu, 0u);
            put32(sp + 0x260u, 0u);
            if (g == def || global(0x01392F58u, 0x01392F5Cu) == def)
                put32(sp + 0x260u, 1u);
        }
        const uint32_t heading = sp + 0x140u, side = sp + 0x150u, quat = sp + 0x160u;
        // GetMotionNodeQuaternionPos(quat, heading, obj, chara + 0x378, 1).
        arg(6, sx(obj));
        arg(7, sx(u32(chara + 0x378u)));
        arg(5, sx(heading));
        arg(4, sx(quat));
        arg(8, 1u);
        callAt(0x01392F8Cu);
        quatAxis(quat, heading);
        // Level unless the definition at 0x1392fe8 is the character's +0x1c0.
        lwc1(0, heading + 4u);
        mtc1(1, 0u);
        uint32_t upHi = 0x01393024u;
        if (cond(FPU_C_OLT_S(f(0), f(1))))
        {
            swc1(1, heading + 4u);
            normalize(heading);
        }
        else if (global(0x01392FE8u, 0x01392FF0u) != u32(chara + 0x1C0u))
            upHi = 0x01392FF8u;
        else
        {
            swc1(1, heading + 4u);
            normalize(heading);
        }
        // The side vector: heading x up, length tuning +8.
        {
            const uint32_t up = reloc(upHi, 0x0139302Cu);
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(heading));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(up));
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            WRITE128(side, _mm_castps_si128(ctx->vu0_vf[7]));
        }
        normalize(side);
        lwc1(0, tuning() + 8u);
        scale(side, side, 5, 0);
        put32(sp + 0x15Cu, 0u);
        // The rays: from the layout position (+0xe0) and +-side, to heading x
        // tuning +0x18 ahead (+0x110) and +-side.
        WRITE128(sp + 0xE0u, READ128(u32(layout + 0x134u)));
        lwc1(0, tuning() + 0x18u);
        scale(heading, sp + 0x110u, 5, 0);
        addXyz(sp + 0x110u, u32(layout + 0x134u));
        mtc1(3, 0x3F800000u);
        const __m128i start = READ128(sp + 0xE0u);
        swc1(3, sp + 0x11Cu);
        WRITE128(sp + 0x100u, start);
        const __m128i end = READ128(sp + 0x110u);
        WRITE128(sp + 0xF0u, start);
        WRITE128(sp + 0x130u, end);
        WRITE128(sp + 0x120u, end);
        addAll(sp + 0xF0u, side, false);
        addAll(sp + 0x120u, side, false);
        addAll(sp + 0x100u, side, true);
        addAll(sp + 0x130u, side, true);
        // The boundary: centre heading x tuning +0x18 / 2 ahead, radius 1.05 x
        // |(tuning +8, tuning +0x18 / 2)|.
        {
            const uint32_t t = tuning();
            mtc1(1, 0x3F000000u);
            lwc1(2, t + 0x18u);
            lwc1(0, t + 8u);
            f(2) = FPU_MUL_S(f(2), f(1));
            f(0) = FPU_MUL_S(f(0), f(0));
            f(1) = FPU_MUL_S(f(2), f(2));
            f(0) = FPU_ADD_S(f(0), f(1));
            f(0) = ps2FpuSqrt(ctx->fcr31, f(0));
        }
        scale(heading, sp + 0x1F0u, 3, 2);
        addXyz(sp + 0x1F0u, sp + 0xE0u);
        mtc1(1, 0x3F866666u); // 1.05
        f(20) = FPU_MUL_S(f(0), f(1));
        swc1(3, sp + 0x1FCu);

        // The obstacle group's objects inside the boundary (at most 32).
        arg(4, sx(kObstacles));
        arg(5, 0u);
        callAt(0x013931BCu); // isysGroupForFirst
        int32_t count = 0; // s3
        uint32_t slot = sp + 0x170u, last = 0u;
        put32(sp + 0x264u, 0u);
        for (uint32_t it = rvw();;)
        {
            f(12) = FPU_MOV_S(f(20));
            if (!it)
                break;
            last = u32(it);
            arg(5, sx(sp + 0x1F0u));
            arg(4, sx(last));
            callAt(0x01393204u); // CheckCollisionInsideOfObjectBoundary(obj, centre, f12)
            arg(4, sx(it));
            arg(5, 0u);
            if (rv() != 0u)
            {
                if (count >= 0x20)
                {
                    put32(sp + 0x264u, 1u);
                    break;
                }
                put32(slot, last);
                slot += 4u;
                ++count;
            }
            callAt(0x013931E8u); // isysGroupForNext
            it = rvw();
        }
        if (last != 0u)
        {
            arg(4, sx(last));
            arg(5, 0u);
            callAt(0x01393234u); // isysGroupForExit
        }

        // The three rays: against the objects (or the whole group when there
        // were too many); a ground wall hit (attribute 0x04) facing the
        // heading within tuning +0xc does not count.
        auto wallFacing = [&](uint32_t result, uint32_t siteAttr, uint32_t siteQuat, uint32_t siteTurn)
        {
            arg(4, sx(result));
            callAt(siteAttr); // GetGeometryPointerCharColAttr
            if ((rv() & 0x7C000000u) != 0x04000000u)
                return false;
            arg(5, sx(result));
            arg(4, sx(sp + 0x200u));
            callAt(siteQuat); // GetGeometryPointerSurfaceYTopZMinusPlaneCoordQuaternion
            const uint32_t t = tuning();
            arg(5, sx(heading));
            arg(4, sx(sp + 0x200u));
            callAt(siteTurn); // GetTurnYAngleFromGlobalDir
            lwc1(1, t + 0xCu);
            f(0) = FPU_ABS_S(f(0));
            return cond(FPU_C_OLT_S(f(0), f(1)));
        };
        const int32_t objects = u32(sp + 0x264u) == 0u ? count : -1; // s5
        for (uint32_t ray = 0u, off = 0u; ray < 3u; off += 0x10u)
        {
            uint64_t hit = 0u; // s1
            if (objects > 0)
            {
                arg(5, sx(sp + 0xE0u + off));
                arg(6, sx(sp + 0x110u + off));
                arg(4, sx(sp + 0x20u));
                arg(7, sx(sp + 0x170u));
                arg(8, sx(static_cast<uint32_t>(objects)));
                arg(9, 0u);
                arg(10, 0u);
                arg(11, 0u);
                put32(sp + 0u, kClipObjCb);
                put32(sp + 8u, 0u);
                callAt(0x01393290u); // ClipCollisionDirectCallBackObjAry
                hit = rv();
            }
            else if (objects < 0)
            {
                arg(5, sx(sp + 0xE0u + off));
                arg(4, sx(sp + 0x20u));
                arg(6, sx(sp + 0x110u + off));
                arg(7, sx(kObstacles));
                arg(8, 0u);
                arg(9, 0u);
                arg(10, 0u);
                arg(11, sx(kClipObjCb));
                put32(sp + 0u, 0u);
                callAt(0x013932D8u); // ClipCollisionDirectCallBack
                hit = rv();
            }
            if (u32(sp + 0x260u) != 0u)
            {
                if (hit == 0u)
                {
                    ++ray;
                    continue;
                }
                if (wallFacing(sp + 0x20u, 0x013932F8u, 0x01393318u, 0x0139332Cu))
                    hit = 0u;
            }
            ++ray;
            if (hit != 0u)
            {
                put32(sp + 0x254u, ray);
                break;
            }
        }

        // The centre ray against the group.
        arg(7, sx(kObstacles));
        arg(4, sx(sp + 0x70u));
        arg(11, sx(kClipCentreCb));
        arg(5, sx(sp + 0xE0u));
        arg(6, sx(sp + 0x110u));
        arg(8, 0u);
        arg(9, 0u);
        arg(10, 0u);
        put32(sp + 0u, 0u);
        callAt(0x01393390u); // ClipCollisionDirectCallBack
        uint64_t centre = rv(); // s3
        if (u32(sp + 0x260u) != 0u && centre != 0u && wallFacing(sp + 0x70u, 0x013933ACu, 0x013933D0u, 0x013933E0u))
            centre = 0u;

        // The edge links (the cliffs' edges) against the three rays.
        arg(4, sx(u32(chara + 0xE40u)));
        arg(5, sx(chara + 0xE20u));
        callAt(0x01393408u); // ReconstructCollisionEdgeLinkDBVWallFinder
        for (uint32_t ray = 0u; ray < 3u;)
        {
            const uint32_t off = ray << 4;
            mtc1(12, 0x40400000u); // 3.0
            arg(7, sx(u32(chara + 0xE40u)));
            arg(6, sx(sp + 0x110u + off));
            arg(5, sx(sp + 0xE0u + off));
            arg(4, sx(sp + 0xC0u));
            callAt(0x01393448u); // ClipEdgeLinkDBFinder
            ++ray;
            if (rv() != 0u)
            {
                put32(sp + 0x258u, ray);
                break;
            }
        }
        return centre;
    }

    // ---- 11. the stop decisions -----------------------------------------------
    // gamecore_sub_01395960(obj, +0x20, +0xc0, +0x70, +0x254, +0x258, centre,
    // fp; work, +0x220, +0x224 on the stack; f12-f15 tuning off.., f16 tuning
    // off16): sp + 0x25c its result.
    void Step::stopCheck(uint32_t off, uint32_t off16, uint64_t centre, uint32_t ra)
    {
        const uint32_t t = tuning();
        arg(8, sx(u32(sp + 0x254u)));
        arg(7, sx(sp + 0x70u));
        lwc1(16, t + off16);
        arg(10, centre);
        lwc1(12, t + off);
        arg(5, sx(sp + 0x20u));
        lwc1(13, t + off + 4u);
        arg(6, sx(sp + 0xC0u));
        lwc1(14, t + off + 8u);
        arg(11, sx(fp));
        lwc1(15, t + off + 12u);
        arg(9, sx(u32(sp + 0x258u)));
        arg(4, sx(obj));
        put32(sp + 8u, sp + 0x220u);
        put32(sp + 0x10u, sp + 0x224u);
        put32(sp + 0u, static_cast<uint32_t>(s7));
        call(kStopCheck, ra);
        put32(sp + 0x25Cu, rvw());
    }

    // The entry's action (sp + 0x250 + 4) is one of the globals at the sites.
    bool Step::entryActionIs(uint32_t nullHi, uint32_t nullLo, uint32_t actHi, uint32_t actLo)
    {
        const uint32_t entry = u32(sp + 0x250u);
        return handle(entry + 4u, reloc(nullHi, nullLo)) == global(actHi, actLo);
    }

    // The steering record cleared and three bits (0x1393900).
    void Step::stopSteering(uint32_t hi1, uint32_t lo1, uint32_t hi2, uint32_t lo2, uint32_t hi3, uint32_t lo3)
    {
        put32(fp + 4u, 0u);
        clearBit(hi1, lo1);
        clearBit(hi2, lo2);
        clearBit(hi3, lo3);
    }

    // The speed and turn limits after a stop test: sp + 0x220 below tuning
    // +0x9c and |sp + 0x224| (degrees) above tuning +0xa0.
    // g: the slope stop's (f1 loaded by the caller, f0 = +0x224 in the
    // branch's delay slot either way).
    bool Step::stopLimits(bool g)
    {
        const uint32_t t = tuning();
        if (!g)
            lwc1(1, sp + 0x220u);
        lwc1(0, t + 0x9Cu);
        const bool slow = cond(FPU_C_OLT_S(f(1), f(0)));
        if (g)
            lwc1(0, sp + 0x224u);
        if (!slow)
            return false;
        if (!g)
            lwc1(0, sp + 0x224u);
        mtc1(1, kRadToDeg);
        lwc1(2, t + 0xA0u);
        f(0) = FPU_MUL_S(f(0), f(1));
        f(0) = FPU_ABS_S(f(0));
        return cond(FPU_C_OLT_S(f(2), f(0)));
    }

    // The definitions' stop tests. Returns the result (sp + 0x25c, the
    // code's a1 / a0 at 0x1393c1c / 0x1393b84); v0 1 if it is not 0.
    uint32_t Step::stopByDefinition(uint32_t slope, uint64_t centre)
    {
        const uint32_t def = definition();
        if (global(0x013934CCu, 0x013934D4u) == def || global(0x013934DCu, 0x013934E0u) == def)
        {
            // The front stop (gamecore_sub_01394580), for the entry's three actions.
            if (u32(sp + 0x250u) == 0u)
                return u32(sp + 0x25Cu);
            if (!entryActionIs(0x01393520u, 0x01393524u, 0x01393554u, 0x01393558u) &&
                !entryActionIs(0x01393588u, 0x0139358Cu, 0x013935BCu, 0x013935C0u) &&
                !entryActionIs(0x013935F0u, 0x013935F4u, 0x01393624u, 0x01393628u))
                return u32(sp + 0x25Cu);
            const uint32_t t = tuning();
            arg(7, sx(u32(sp + 0x254u)));
            arg(5, sx(sp + 0x20u));
            lwc1(12, t + 0x8Cu);
            arg(8, sx(u32(sp + 0x258u)));
            arg(6, sx(sp + 0xC0u));
            arg(11, sx(u32(sp + 0x250u)));
            arg(9, sx(fp));
            arg(4, sx(obj));
            arg(10, s7);
            put32(sp + 0u, sp + 0x220u);
            put32(sp + 8u, sp + 0x224u);
            call(kStopFront, 0x01393670u);
            if (rv() != 0u)
            {
                clearSteerState(false);
                put32(sp + 0x25Cu, 1u);
                stopSteering(0x01393680u, 0x01393904u, 0x01393690u, 0x01393920u, 0x01393698u, 0x01393938u);
                return u32(sp + 0x25Cu);
            }
            if (stopLimits(false))
            {
                clearBit(0x013936E4u, 0x013936ECu);
                return u32(sp + 0x25Cu);
            }
            return u32(sp + 0x25Cu);
        }
        if (global(0x013934E8u, 0x01393710u) == def || global(0x01393718u, 0x0139371Cu) == def)
        {
            // The slope stop (thresholds +0x6c / +0x38), then the speed limits for result 1.
            if (slope != 0u)
                stopCheck(0x6Cu, 0x7Cu, centre, 0x013937B4u);
            else
                stopCheck(0x38u, 0x48u, centre, 0x013937B4u);
            if (u32(sp + 0x250u) == 0u)
                return u32(sp + 0x25Cu);
            if (entryActionIs(0x013937F0u, 0x013937F4u, 0x01393824u, 0x01393828u))
                return u32(sp + 0x25Cu);
            const bool listed = entryActionIs(0x01393858u, 0x0139385Cu, 0x0139388Cu, 0x01393890u);
            const uint32_t result = u32(sp + 0x25Cu);
            if (listed)
                return result;
            lwc1(1, sp + 0x220u);
            if (result != 1u)
                return result;
            if (!stopLimits(true))
                return result;
            clearSteerState(false);
            stopSteering(0x013938DCu, 0x01393904u, 0x013938F0u, 0x01393920u, 0x013938F8u, 0x01393938u);
            return u32(sp + 0x25Cu);
        }
        if (global(0x01393724u, 0x01393958u) == def || global(0x01393960u, 0x01393964u) == def)
        {
            if (slope != 0u)
                stopCheck(0x5Cu, 0x7Cu, centre, 0x01393C14u);
            else
                stopCheck(0x28u, 0x48u, centre, 0x01393C14u);
            return u32(sp + 0x25Cu);
        }
        if (!(global(0x0139396Cu, 0x013939E8u) == def || global(0x013939F0u, 0x013939F4u) == def ||
              global(0x013939FCu, 0x01393A00u) == def || global(0x01393A08u, 0x01393A0Cu) == def))
            return u32(sp + 0x25Cu);
        // The cliff / slope test of the plane ahead (when the definition at
        // 0x1393a1c is the character's +0x1c0), then the stop test.
        if (global(0x01393A1Cu, 0x01393A24u) == u32(chara + 0x1C0u))
        {
            uint32_t plane = 0u; // s0
            if (u32(sp + 0x258u) != 0u)
            {
                WRITE128(sp + 0x210u, READ128(sp + 0xC0u));
                plane = sp + 0xD0u;
            }
            else if (u32(sp + 0x254u) != 0u)
            {
                WRITE128(sp + 0x210u, READ128(sp + 0x60u));
                arg(4, sx(sp + 0x20u));
                callAt(0x01393A60u); // GetGeometryPointerSurfacePlane
                plane = rvw();
            }
            if (plane != 0u && slopeAhead(plane))
                return 1u; // 0x1393e10
        }
        if (u32(sp + 0x25Cu) != 0u)
            return u32(sp + 0x25Cu);
        if (slope != 0u)
            stopCheck(0x4Cu, 0x7Cu, centre, 0x01393C14u);
        else
            stopCheck(0x18u, 0x48u, centre, 0x01393C14u);
        return u32(sp + 0x25Cu);
    }

    // The plane's slope (GetPlaneSlopeDegree) at least the character's +0x400
    // (radians to degrees), gamecore_sub_01395BC8's height below tuning +0x90
    // and its angle outside +-(pi - tuning +0x94 degrees), the steer input
    // within tuning +0: a control bit (which by the angle's side), result 1.
    bool Step::slopeAhead(uint32_t plane)
    {
        arg(4, sx(plane));
        mtc1(0, kRadToDeg);
        lwc1(20, chara + 0x400u);
        f(20) = FPU_MUL_S(f(20), f(0));
        callAt(0x01393A88u); // GetPlaneSlopeDegree
        f(0) = FPU_ABS_S(f(0));
        if (!cond(FPU_C_OLE_S(f(20), f(0))))
            return false;
        arg(4, sx(obj));
        arg(8, sx(plane));
        arg(5, sx(sp + 0x228u));
        arg(6, sx(sp + 0x22Cu));
        arg(7, sx(sp + 0x210u));
        call(kSlope, 0x01393AB8u);
        const uint32_t t = tuning();
        lwc1(1, sp + 0x228u);
        lwc1(0, t + 0x90u);
        if (!cond(FPU_C_OLT_S(f(1), f(0))))
            return false;
        lwc1(0, t + 0x94u);
        mtc1(1, 0x3C8EFA35u); // pi / 180
        mtc1(2, 0xC0490FDAu); // -pi
        f(1) = FPU_MUL_S(f(0), f(1));
        lwc1(3, sp + 0x22Cu);
        f(2) = FPU_ADD_S(f(1), f(2));
        if (!cond(FPU_C_OLE_S(f(3), f(2))))
        {
            mtc1(0, 0x40490FDAu); // pi
            f(0) = FPU_SUB_S(f(0), f(1));
            if (!cond(FPU_C_OLE_S(f(0), f(3))))
                return false;
        }
        lwc1(0, fp + 8u);
        lwc1(1, t);
        f(0) = FPU_ABS_S(f(0));
        if (!cond(FPU_C_OLE_S(f(0), f(1))))
            return false; // 0x1393b80: the caller reads sp + 0x25c
        const bool downhill = cond(FPU_C_OLE_S(f(2), f(3)));
        const uint64_t flags = get64(flagsAt());
        const uint32_t record = downhill ? global(0x01393B48u, 0x01393B54u) : global(0x01393B58u, 0x01393B60u);
        put32(sp + 0x25Cu, 1u);
        put64(flagsAt(), flags | bitOf(record));
        return true;
    }

    // ---- 12. the turn, the stop latch -----------------------------------------
    // Returns the result (sp + 0x25c) for 0x1393d98.
    uint32_t Step::turn()
    {
        lwc1(3, s4 + 0x4224u);
        lwc1(4, kNoTarget);
        if (cond(FPU_C_EQ_S(f(3), f(4))))
            return u32(sp + 0x25Cu);
        // The target dropped while the input is past tuning +0 or two bits are set.
        lwc1(2, fp + 8u);
        const uint32_t t = tuning();
        f(1) = FPU_ABS_S(f(2));
        lwc1(0, t);
        bool drop = cond(FPU_C_OLE_S(f(0), f(1)));
        if (!drop)
        {
            const uint32_t r1 = global(0x01393C5Cu, 0x01393C60u), r2 = global(0x01393C58u, 0x01393C68u);
            const uint64_t mask = bitOf(r2) | bitOf(r1);
            drop = (get64(flagsAt()) & mask) != 0u;
            lwc1(0, kNoTarget);
        }
        if (drop)
        {
            swc1(4, s4 + 0x4224u);
            f(3) = FPU_MOV_S(f(4));
            lwc1(0, kNoTarget);
        }
        if (cond(FPU_C_EQ_S(f(3), f(0))))
            return u32(sp + 0x25Cu);
        // The body's heading (the character's +0x470 quaternion), level.
        quatAxis(chara + 0x470u, sp + 0x210u);
        put32(sp + 0x214u, 0u);
        normalize(sp + 0x210u);
        lwc1(12, sp + 0x210u);
        lwc1(13, sp + 0x218u);
        callAt(0x01393D18u); // iosGetATan2fExact
        lwc1(12, s4 + 0x4224u);
        f(12) = FPU_SUB_S(f(12), f(0));
        call(kAdjustPi, 0x01393D2Cu);
        arg(5, sx(fp));
        arg(4, sx(obj));
        f(12) = FPU_MOV_S(f(0));
        arg(6, s7);
        call(kTurn, 0x01393D40u);
        if (rv() != 0u)
        {
            put32(sp + 0x25Cu, 3u);
            return 3u;
        }
        if (global(0x01393D4Cu, 0x01393D50u) != definition())
            return u32(sp + 0x25Cu);
        put32(sp + 0x25Cu, 3u);
        setBit(0x01393D60u, 0x01393D68u);
        return u32(sp + 0x25Cu);
    }

    // 0x1393da0..: the stop latch +0x4200 (kept while |+0x41ec| is past
    // kStopEps and the input within tuning +0), the steer input written
    // back. v0 as the code leaves it.
    void Step::stopLatch()
    {
        v0 = 1u;
        if (u32(s4 + 0x4200u) == 0u)
        {
            swc1(21, fp + 8u);
            f(2) = FPU_MOV_S(f(21));
            swc1(2, s4 + 0x41ECu);
            return;
        }
        lwc1(12, s4 + 0x41ECu);
        f(12) = FPU_ABS_S(f(12));
        callAt(0x01393DB0u); // fptodp
        arg(5, get64(kStopEps));
        arg(4, rv());
        callAt(0x01393DC4u); // dpcmp
        bool keep = true;
        if (static_cast<int32_t>(rvw()) < 0)
        {
            v0 = rv();
            put32(s4 + 0x4200u, 0u);
            keep = false;
            lwc1(2, fp + 8u);
        }
        else
        {
            lwc1(2, fp + 8u);
            const uint32_t t = tuning();
            v0 = sx(t);
            f(1) = FPU_ABS_S(f(2));
            lwc1(0, t);
            if (cond(FPU_C_OLT_S(f(0), f(1))))
            {
                put32(s4 + 0x4200u, 0u);
                keep = false;
                lwc1(2, fp + 8u);
            }
        }
        if (!keep)
        {
            swc1(21, fp + 8u);
            f(2) = FPU_MOV_S(f(21));
        }
        swc1(2, s4 + 0x41ECu);
    }

    void Step::finish(uint64_t ra, uint64_t entrySp)
    {
        lwc1(22, sp + 0x2D0u);
        lwc1(21, sp + 0x2C8u);
        lwc1(20, sp + 0x2C0u);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    void Step::run(uint64_t a0, uint64_t ra, uint64_t entrySp)
    {
        sp = static_cast<uint32_t>(entrySp) - 0x2E0u;
        obj = static_cast<uint32_t>(a0);
        s7 = GPR_U64(ctx, 5);
        fp = static_cast<uint32_t>(s7) + 0x80u;
        prologue(a0, ra);

        // ---- the classes ----
        call(kAiWork, 0x01391E7Cu); // GetSCRCharaAIWorkPtr
        s4 = rvw();
        arg(4, sx(obj));
        callAt(0x01391E80u); // GetLayoutClass
        layout = rvw();
        put32(sp + 0x234u, layout);
        arg(4, sx(obj));
        callAt(0x01391E8Cu); // GetCharacterClass
        chara = rvw();
        put32(sp + 0x238u, chara);
        put32(sp + 0x240u, 0u);
        put32(sp + 0x23Cu, chara + 0x1C0u);

        if (u32(chara + 0x960u) != obj)
            rider();
        rideTimerAndPosition();
        bool stopping = false; // s3
        latchesAndTimers(stopping);
        pendingFlags();
        steerInput(stopping);
        bool none = false;
        controlDefinitions(none);
        if (none)
        {
            v0 = 0x30000u; // lui v0, 3 in the branch's delay slot
            lwc1(2, fp + 8u);
            swc1(2, s4 + 0x41ECu);
            return finish(ra, entrySp);
        }
        definitionLatches();
        groundKind(passageLane());

        // 0x1392ef4: no probes while sp + 0x240, or when the horse is its own
        // rider without +0x41f4.
        auto keepInput = [&]()
        {
            swc1(21, fp + 8u);
            swc1(21, s4 + 0x41ECu);
            put32(s4 + 0x4200u, 0u);
            finish(ra, entrySp);
        };
        if (u32(sp + 0x240u) != 0u)
            return keepInput();
        uint32_t defHi = 0x01392F38u;
        if (obj == u32(chara + 0x960u))
        {
            v0 = sx(u32(s4 + 0x41F4u));
            if (v0 == 0u)
                return keepInput();
            defHi = 0x01392F18u;
        }
        const uint64_t centre = probes(defHi);

        // 0x1393460: no hit, no edge, no centre hit: straight to the turn.
        uint32_t result;
        const uint32_t hitRay = u32(sp + 0x254u);
        if (hitRay == 0u && u32(sp + 0x258u) == 0u && centre == 0u)
            result = u32(sp + 0x25Cu);
        else
        {
            // The hit ray's ground kind (bits 32-35 of the attribute: 2 / 0xb8 >> 3).
            uint32_t slope = 0u; // s1
            if (hitRay != 0u)
            {
                constexpr uint64_t kSlopeMask = 0xF80000000ull;
                arg(4, sx(sp + 0x20u));
                call(kGeomAttr, 0x01393494u);
                if ((rv() & kSlopeMask) == 0x200000000ull)
                    slope = 1u;
                else
                {
                    arg(4, sx(sp + 0x20u));
                    call(kGeomAttr, 0x013934B0u);
                    if ((rv() & kSlopeMask) == 0xB80000000ull)
                        slope = 1u;
                }
            }
            result = stopByDefinition(slope, centre);
        }
        if (result == 0u)
            result = turn();
        if (result != 0u)
        {
            v0 = 1u;
            put32(s4 + 0x4200u, 1u);
            lwc1(2, fp + 8u);
            swc1(2, s4 + 0x41ECu);
            return finish(ra, entrySp);
        }
        stopLatch();
        finish(ra, entrySp);
    }

    void horseStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kSelf, rdram, ctx, runtime);
        Step s{rdram, ctx, runtime};
        s.run(GPR_U64(ctx, 4), GPR_U64(ctx, 31), GPR_U64(ctx, 29));
    }
}

SOTC_DECOMP_FUNCTION(0x01391E30u, gamecore_sub_01391E30, decomp_hot46b::horseStep);
