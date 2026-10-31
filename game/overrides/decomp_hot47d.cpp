// Round 47 (SOTC_HOTLIST47=0 puts it back to the recompiled code; the table
// entry alone with SOTC_DECOMP_SKIP=GetCharaRootDynamics): the character
// root's dynamics step, transcribed from the generator's code
// (build/recomp/game/output/GetCharaRootDynamics_0x10d9fc0.cpp, the same
// FPU_* / PS2_V* statements in order).
//
// - GetCharaRootDynamics (0x10d9fc0): a0 the object, a1 the dynamics kind
//   (fp: compared with the kind words, each a relocated lui / lw pair), a2 /
//   a3 / t1 passed on to the kind's step (s4 / s5 / s2), t0 a vector (copied
//   to the frame's +0, its +4 clamped to >= 0.01 for the xz kinds; the
//   frame is the steps' a1), f12-f15 four floats (f23 / f22 / f21 / f20).
//   The class (GetCharacterClass): +0x13c the rotation quaternion (frame
//   +0x8c), +0x134 the position (+0x90), +0x438 the current kind.
//   1. The kind change: when the class's kind is one of two kinds, the
//      +0x529 flag set and the new kind differs, the +0x4b0 quaternion
//      multiplied into the rotation and normalised (VU0). The class's kind
//      equal to the reset kind: the root's work reset (+0xa20, the flags,
//      +0xab0 / +0x9d0 / +0x30 cleared, the geometry +0x19c's records from
//      +0x30 and a relocated default); else manager_sub_010D3950 (f21, f20).
//      With +0x124 and a legal collision geometry pointer (+0xc0) whose
//      layout's scale (+0x90 / +0x94 / +0x9c) has a component below 0.01:
//      the same reset and the frame's +0x88 "unparent" flag.
//   2. The kind's handler table (fp's packed word: a handle in the low 18
//      bits back from fp, an index in bits 18..29, records of 0x10 bytes):
//      the record whose target is 0 or the class's old kind, its callback
//      (+4) called with (object, class, frame); the class's +0xa27 flag
//      clears the old kind for the walk.
//   3. The step's reset (the flags, the water status, the parent links:
//      UnlinkSecureParentInfo / DeleteTwoWayLink, +0x6a0 from a relocated
//      default, FlushCollisionWork), the root's velocity cleared unless the
//      kind is one of six, then the kind's step: xz / xzfit (the 0.01 clamp),
//      010D8340 (t1 0 or 1, +0x10a7 set), edgepoint, parentpointxzfit,
//      edgexz, pendulum, charnodeparent, norootdynamics (after
//      StabilizeToYRotQuaternion and the layout's position from +0x30 x f21,
//      for two kinds), swim, or the inline "follow" kind (the class's
//      position: the root offset (+0x1080 or the class's +0) plus +0x70,
//      blended by f22 into +0x134, the velocity (+0x30) from the move x f20,
//      charrootdyn_getRegularColFinderPosition, the geometry's +0x8c set).
//   4. One kind's +0x34 eased towards 0 by 20 x f21; the position's w 1;
//      ForceUnparentLayoutObj on the frame flag; the +0x1020 parent link
//      dropped when it differs from the collision geometry; the interp
//      point (charrootdyn_getInterpPoint, f22); the parent link by the
//      interp record's flag (bit 25 of +0x68): the move turned by the
//      rotation (VU0 quaternion to matrix), LinkSecureParentInfoByDBPos or
//      SetGeometryPointerGlobalState; the rotation normalised again; +0x51c
//      / +0x520 the height above +0x10b4; for parented kinds (+0x370 2 or
//      3) the parent character's link callback (+0x18c) and
//      isysAddGroupWithLinkParam (boot ELF, relocated jal);
//      GetLayoutClassWindGeometry; the up vector from the rotation, two
//      GetPlaneFittingVector calls (the geometry's +0x60 / +0x70) whose
//      length (+0x514, vsqrt) and squared length (+0x518) are stored.
//   v0 the last qmfc2 (vf7, 128 bits), f0 its x.
//
// The recompiled code's statements in its order: the FPU operations
// (FPU_* macros and the condition bit at each compare, on the context's
// registers), the VU0 ones (the PS2_V* forms the generator writes, on the
// context's vf / ACC / Q; a blend with a full mask is the plain assignment
// it is), HI / LO at the handle walk's mult; every store the original makes
// (its frame included: the vector at +0, the flags at +0x80 .. +0x90, the
// temporaries at +0x10 .. +0x70, the saves). The callee-saved registers
// (s0-s7, fp) are set in the context as the code sets them (the same 64-bit
// values: lui / lw / addiu sign-extended, daddu copies) and come back from
// the frame as the epilogue reloads them; f20-f23 likewise. Guest calls go
// through the function table (callGuest) with the original's ra and frame
// sp; the boot ELF's jal sites (StabilizeToYRotQuaternion,
// isysAddGroupWithLinkParam) from the live code words, the kind words and
// the defaults from the live lui / lw pairs (relocated), as the generated
// code reads them.
// Not as the original: the scratch GPRs (only the call arguments, and the
// GPRs the VU0 moves read, are set; at / v1 / a0 ... in between are not),
// the loop's EE checkpoint (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47d
{
    using sotc_decomp::callGuest;
    using sotc_decomp::codeWord;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kSelf = 0x010D9FC0u;

    // The callees (constant jal targets of the generated code).
    constexpr uint32_t kCharacterClass = 0x010C0A70u;
    constexpr uint32_t kSub3950 = 0x010D3950u;         // manager_sub_010D3950
    constexpr uint32_t kCheckLegalGP = 0x01132D58u;    // CheckLegalCollisionGP
    constexpr uint32_t kGeomObject = 0x011329C0u;      // GetGeometryPointerObject
    constexpr uint32_t kLayoutClass = 0x010F1A20u;     // GetLayoutClass
    constexpr uint32_t kWaterReset = 0x010C11B8u;      // ResetCharacterClassWaterStatus
    constexpr uint32_t kUnlinkParent = 0x010C2BE8u;    // UnlinkSecureParentInfo
    constexpr uint32_t kDeleteLink = 0x01147AB0u;      // DeleteTwoWayLink
    constexpr uint32_t kFlushColl = 0x010F1D60u;       // FlushCollisionWork
    constexpr uint32_t kXz = 0x010E7610u, kXzFit = 0x010EB858u, kSub8340 = 0x010D8340u, kEdgePoint = 0x010E2D28u;
    constexpr uint32_t kParentXzFit = 0x010E48B8u, kEdgeXz = 0x010E3B28u, kCharNodeParent = 0x010D9660u;
    constexpr uint32_t kNoRootDyn = 0x010D9AA0u, kPendulum = 0x010E4FE8u, kSwim = 0x010E6AE8u;
    constexpr uint32_t kForceUnparent = 0x010F1FC0u;   // ForceUnparentLayoutObj
    constexpr uint32_t kColFinderPos = 0x010DE1A0u;    // charrootdyn_getRegularColFinderPosition
    constexpr uint32_t kInterpPoint = 0x010DC220u;     // charrootdyn_getInterpPoint
    constexpr uint32_t kLinkByDBPos = 0x010C2C88u;     // LinkSecureParentInfoByDBPos
    constexpr uint32_t kSetGPGlobal = 0x011328F8u;     // SetGeometryPointerGlobalState
    constexpr uint32_t kIsParentChar = 0x010CC628u;    // IsParentCharOrLwsOrient
    constexpr uint32_t kWindGeometry = 0x010F2C78u;    // GetLayoutClassWindGeometry
    constexpr uint32_t kPlaneFit = 0x01134628u;        // GetPlaneFittingVector
    // The boot ELF's jal sites (read from the code words, as the generated code does).
    constexpr uint32_t kSiteStabilize = 0x010DA720u;   // StabilizeToYRotQuaternion
    constexpr uint32_t kSiteAddGroup1 = 0x010DACD4u, kSiteAddGroup2 = 0x010DAD28u; // isysAddGroupWithLinkParam

    constexpr uint32_t kOne = 0x3F800000u, kMinusOne = 0xBF800000u, kHundredth = 0x3C23D70Au;
    constexpr uint32_t kTwenty = 0x41A00000u, kMinusTwenty = 0xC1A00000u;

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
    inline __m128 lanes(int a, int b, int c, int d) { return _mm_castsi128_ps(_mm_set_epi32(a, b, c, d)); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // A lui at hi: its register's value (sign-extended).
    inline uint64_t luiValue(const uint8_t *rdram, uint32_t hi) { return sx(codeWord(rdram, hi) << 16); }
    // lw rX, lo(lui at hi): a word at a relocated address (sign-extended).
    inline uint64_t relocWord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t hi, uint32_t lo)
    {
        return sx(READ32(relocated(rdram, hi, lo)));
    }

    // lqc2 vf14, (q); its normalisation (vrsqrt of the dot); sqc2 vf15, (q).
    inline void normaliseQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q)
    {
        __m128 *const vf = ctx->vu0_vf;
        vf[14] = _mm_castsi128_ps(READ128(q));
        vf[15] = PS2_VMUL(vf[14], vf[14]);
        { __m128 res = PS2_VADD(vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(1, 1, 1, 1))); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, 0, 0, -1)); }
        { __m128 res = PS2_VADD(vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(2, 2, 2, 2))); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, 0, 0, -1)); }
        { __m128 res = PS2_VADD(vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3, 3, 3, 3))); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, 0, 0, -1)); }
        {
            float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 0, 0, 3)));
            float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft);
        }
        vf[15] = PS2_VMUL(vf[14], _mm_set1_ps(ctx->vu0_q));
        WRITE128(q, _mm_castps_si128(vf[15]));
    }

    void rootDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kSelf, rdram, ctx, runtime);
        float *f = ctx->f;
        __m128 *const vf = ctx->vu0_vf;
        const uint64_t entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x110u;
        const uint64_t spX = sx(sp); // daddu rX, sp: the register's 64-bit value
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };

        // The prologue: the saves, s2 = t1, s4 = a2, s5 = a3, s6 = the kind
        // words' lui, fp = a1, f23..f20 = f12..f15, a0 / t0 into the frame.
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 9));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 19));
        WRITE64(sp + 0xC0u, GPR_U64(ctx, 20));
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 6));
        WRITE64(sp + 0xC8u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 7));
        WRITE64(sp + 0xD0u, GPR_U64(ctx, 22));
        constexpr uint32_t kSiteS6 = 0x010D9FE4u;
        SET_GPR_U64(ctx, 22, luiValue(rdram, kSiteS6));
        WRITE64(sp + 0xE0u, GPR_U64(ctx, 30));
        const uint64_t fp = GPR_U64(ctx, 5); // the dynamics kind
        SET_GPR_U64(ctx, 30, fp);
        WRITE32(sp + 0x108u, bf(f[23]));
        f[23] = FPU_MOV_S(f[12]);
        WRITE32(sp + 0x100u, bf(f[22]));
        f[22] = FPU_MOV_S(f[13]);
        WRITE32(sp + 0xF8u, bf(f[21]));
        f[21] = FPU_MOV_S(f[14]);
        WRITE32(sp + 0xF0u, bf(f[20]));
        f[20] = FPU_MOV_S(f[15]);
        WRITE32(sp + 0x80u, GPR_U32(ctx, 4));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0xD8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xE8u, GPR_U64(ctx, 31));
        WRITE32(sp + 0x84u, GPR_U32(ctx, 8));
        call(kCharacterClass, 0x010DA02Cu); // a0 the object

        const uint64_t s3 = GPR_U64(ctx, 2);
        const uint32_t cls = static_cast<uint32_t>(s3);
        SET_GPR_U64(ctx, 19, s3);
        auto object = [&]() { return sx(READ32(sp + 0x80u)); };
        // The step's arguments (a0 the object, a1 the frame, a2 s2, a3 s4,
        // t0 s5, f12 f22, f13 f21, f14 f20, f15 f23).
        auto stepArgs = [&]()
        {
            SET_GPR_U64(ctx, 4, object());
            SET_GPR_U64(ctx, 6, GPR_U64(ctx, 18));
            f[14] = FPU_MOV_S(f[20]);
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
            f[15] = FPU_MOV_S(f[23]);
            SET_GPR_U64(ctx, 8, GPR_U64(ctx, 21));
            SET_GPR_U64(ctx, 5, spX);
            f[12] = FPU_MOV_S(f[22]);
            f[13] = FPU_MOV_S(f[21]);
        };

        {
            const uint32_t t0v = READ32(sp + 0x84u);
            const uint32_t quat = READ32(cls + 0x13Cu);
            const __m128i vec = READ128(t0v);
            WRITE32(sp + 0x8Cu, quat);
            const uint32_t pos = READ32(cls + 0x134u);
            WRITE32(sp + 0x88u, 0u);
            WRITE32(sp + 0x90u, pos);
            WRITE128(sp, vec);
        }

        // 1. The kind change (s7 from the lui the path takes).
        uint32_t s7Site = 0x010DA06Cu;
        uint64_t oldKind = sx(READ32(cls + 0x438u));
        bool changeCheck = oldKind == relocWord(rdram, ctx, runtime, kSiteS6, 0x010DA040u);
        if (!changeCheck)
            changeCheck = oldKind == relocWord(rdram, ctx, runtime, 0x010DA060u, 0x010DA064u);
        if (changeCheck)
        {
            s7Site = 0x010DA078u;
            if (static_cast<int8_t>(READ8(cls + 0x529u)) != 0 && fp != oldKind)
            {
                // The +0x4b0 quaternion times the rotation, normalised.
                const uint32_t q = READ32(sp + 0x8Cu);
                vf[11] = _mm_castsi128_ps(READ128(cls + 0x4B0u));
                vf[12] = _mm_castsi128_ps(READ128(q));
                vf[13] = PS2_VMUL(vf[11], vf[12]);
                { __m128 res = PS2_VADD(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(1, 1, 1, 1))); vf[13] = _mm_blendv_ps(vf[13], res, lanes(0, 0, 0, -1)); }
                { __m128 res = PS2_VADD(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(2, 2, 2, 2))); vf[13] = _mm_blendv_ps(vf[13], res, lanes(0, 0, 0, -1)); }
                { __m128 res = PS2_VSUB(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(0, 0, 0, 0))); vf[13] = _mm_blendv_ps(vf[13], res, lanes(-1, 0, 0, 0)); }
                { __m128 res = PS2_VMUL(vf[12], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 3, 3, 3))); vf[14] = _mm_blendv_ps(vf[14], res, lanes(0, -1, -1, -1)); }
                { __m128 res = PS2_VMUL(vf[11], _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 3, 3, 3))); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, -1, -1, -1)); }
                {
                    __m128 fs_yzx = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 0, 2, 1));
                    __m128 ft_zxy = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 1, 0, 2));
                    __m128 res = PS2_VMUL(fs_yzx, ft_zxy);
                    ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(0, -1, -1, -1));
                }
                {
                    __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 0, 2, 1));
                    __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 1, 0, 2));
                    __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy);
                    __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res);
                    vf[16] = _mm_blendv_ps(vf[16], res, lanes(0, -1, -1, -1));
                }
                { __m128 res = PS2_VADD(vf[14], vf[15]); vf[13] = PS2_VBLEND(vf[13], res, lanes(0, -1, -1, -1)); }
                { __m128 res = PS2_VADD(vf[13], vf[16]); vf[13] = PS2_VBLEND(vf[13], res, lanes(0, -1, -1, -1)); }
                WRITE128(q, _mm_castps_si128(vf[13]));
                normaliseQuat(rdram, ctx, runtime, q);
                oldKind = sx(READ32(cls + 0x438u));
                s7Site = 0x010DA0E4u;
            }
        }
        SET_GPR_U64(ctx, 23, luiValue(rdram, s7Site));

        // The root's work reset (the defaults' lui / addiu pair at hi / lo).
        auto resetRoot = [&](uint32_t hi, uint32_t lo, bool flag)
        {
            WRITE8(cls + 0x10A1u, 0u);
            WRITE8(cls + 0xC96u, 0u);
            WRITE128(cls + 0xAB0u, _mm_setzero_si128());
            WRITE8(cls + 0xC95u, 0u);
            WRITE128(cls + 0x9D0u, _mm_setzero_si128());
            WRITE128(cls + 0x30u, _mm_setzero_si128());
            const uint32_t geo0 = READ32(cls + 0x19Cu);
            const __m128i v30a = READ128(cls + 0x30u);
            const uint32_t defaults = relocated(rdram, hi, lo);
            if (flag)
                WRITE32(sp + 0x88u, 1u);
            WRITE128(geo0, v30a);
            const uint32_t geo1 = READ32(cls + 0x19Cu);
            const __m128i v30b = READ128(cls + 0x30u);
            WRITE128(geo1 + 0x10u, v30b);
            const uint32_t geo2 = READ32(cls + 0x19Cu);
            WRITE8(geo2 + 0x8Eu, READ8(cls + 0x124u));
            const __m128i def = READ128(defaults);
            const uint32_t geo3 = READ32(cls + 0x19Cu);
            WRITE128(geo3 + 0x30u, def);
            const uint32_t geo4 = READ32(cls + 0x19Cu);
            WRITE32(geo4 + 0x88u, 0u);
        };
        if (oldKind == relocWord(rdram, ctx, runtime, s7Site, 0x010DA0E8u))
        {
            WRITE32(cls + 0xA20u, 0u);
            resetRoot(0x010DA120u, 0x010DA128u, false);
        }
        else
        {
            f[12] = FPU_MOV_S(f[21]);
            f[13] = FPU_MOV_S(f[20]);
            SET_GPR_U64(ctx, 4, s3);
            call(kSub3950, 0x010DA16Cu);
        }
        // A collision geometry whose layout is scaled to (almost) nothing.
        if (READ32(cls + 0x124u) != 0u)
        {
            const uint64_t s0 = sx(cls + 0xC0u);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 4, s0);
            call(kCheckLegalGP, 0x010DA184u);
            if (GPR_U64(ctx, 2) != 0u)
            {
                SET_GPR_U64(ctx, 4, s0);
                call(kGeomObject, 0x010DA194u);
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
                call(kLayoutClass, 0x010DA19Cu);
                const uint32_t layout = GPR_U32(ctx, 2);
                f[1] = fb(kHundredth);
                f[0] = fb(READ32(layout + 0x90u));
                setC(ctx, FPU_C_OLT_S(f[0], f[1]));
                bool small = FPU_C_OLT_S(f[0], f[1]);
                if (!small)
                {
                    f[0] = fb(READ32(layout + 0x94u));
                    small = FPU_C_OLT_S(f[0], f[1]);
                    setC(ctx, small);
                    if (!small)
                    {
                        f[0] = fb(READ32(layout + 0x9Cu));
                        small = FPU_C_OLT_S(f[0], f[1]);
                        setC(ctx, small);
                    }
                }
                if (small)
                {
                    WRITE32(cls + 0xA20u, 0u);
                    resetRoot(0x010DA20Cu, 0x010DA214u, true);
                }
            }
        }

        // 2. The kind's handler table.
        {
            const uint32_t word = READ32(static_cast<uint32_t>(fp));
            const uint32_t back = word & 0x3FFFFu;
            uint64_t s1 = sx(READ32(cls + 0x438u));
            SET_GPR_U64(ctx, 17, s1);
            const uint64_t table = back != 0u ? sx(READ32(static_cast<uint32_t>(fp) - back)) : 0u;
            uint64_t s0 = 0u;
            if (table != 0u && table != relocWord(rdram, ctx, runtime, 0x010DA278u, 0x010DA27Cu))
            {
                const uint32_t index = (word >> 18) & 0xFFFu;
                const uint32_t base = READ32(static_cast<uint32_t>(table) + 0x10u);
                const uint32_t stride = READ32(READ32(static_cast<uint32_t>(table) + 8u) + 0xCu);
                const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(stride));
                ctx->lo = sx(static_cast<uint32_t>(product));
                ctx->hi = sx(static_cast<uint32_t>(product >> 32));
                s0 = sx(base + static_cast<uint32_t>(product));
            }
            SET_GPR_U64(ctx, 16, s0);
            if (s0 != 0u)
            {
                SET_GPR_U64(ctx, 4, object());
                call(kCharacterClass, 0x010DA2C0u);
                const uint64_t cls2 = GPR_U64(ctx, 2);
                if (static_cast<int8_t>(READ8(static_cast<uint32_t>(cls2) + 0xA27u)) != 0)
                {
                    WRITE8(static_cast<uint32_t>(cls2) + 0xA27u, 0u);
                    s1 = 0u;
                    SET_GPR_U64(ctx, 17, 0u);
                }
                for (;;)
                {
                    const uint32_t entry = static_cast<uint32_t>(s0);
                    const uint32_t w = READ32(entry);
                    const uint32_t b = w & 0x3FFFFu;
                    const uint64_t t = b != 0u ? sx(READ32(entry - b)) : 0u;
                    uint64_t target = 0u;
                    if (t != 0u && t != relocWord(rdram, ctx, runtime, 0x010DA2D8u, 0x010DA300u))
                    {
                        const uint32_t index = (w >> 18) & 0xFFFu;
                        const uint32_t base = READ32(static_cast<uint32_t>(t) + 0x10u);
                        const uint32_t stride = READ32(READ32(static_cast<uint32_t>(t) + 8u) + 0xCu);
                        const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(stride));
                        ctx->lo = sx(static_cast<uint32_t>(product));
                        ctx->hi = sx(static_cast<uint32_t>(product >> 32));
                        target = sx(base + static_cast<uint32_t>(product));
                    }
                    if (target == 0u || target == s1)
                    {
                        const uint64_t callback = sx(READ32(entry + 4u));
                        if (callback != 0u)
                        {
                            SET_GPR_U64(ctx, 2, callback); // jalr v0
                            SET_GPR_U64(ctx, 4, object());
                            SET_GPR_U64(ctx, 5, cls2);
                            SET_GPR_U64(ctx, 6, spX);
                            call(static_cast<uint32_t>(callback), 0x010DA354u);
                        }
                        break;
                    }
                    s0 = sx(entry + 0x10u);
                    SET_GPR_U64(ctx, 16, s0);
                }
            }
        }

        // 3. The step's reset.
        WRITE32(READ32(sp + 0x90u) + 0xCu, kOne);
        f[0] = fb(kOne);
        WRITE8(cls + 0x531u, 0u);
        WRITE8(cls + 0x10C0u, 0u);
        WRITE8(cls + 0x35Du, 0u);
        WRITE8(cls + 0x35Cu, 0u);
        WRITE8(cls + 0x35Eu, 0u);
        WRITE8(cls + 0x35Fu, 0u);
        WRITE32(cls + 0xFC0u, 0u);
        WRITE8(cls + 0x530u, 0u);
        WRITE8(cls + 0x535u, 0u);
        WRITE8(cls + 0x10A4u, 0u);
        WRITE8(cls + 0x10A3u, 0u);
        WRITE8(cls + 0x10A1u, 0u);
        WRITE8(cls + 0x10A6u, 0u);
        SET_GPR_U64(ctx, 4, s3);
        call(kWaterReset, 0x010DA3B4u);
        SET_GPR_U64(ctx, 4, sx(cls + 0xF70u));
        call(kUnlinkParent, 0x010DA3BCu);
        if (READ32(cls + 0xF3Cu) != 0u)
        {
            WRITE32(cls + 0xF3Cu, 0u);
            SET_GPR_U64(ctx, 4, sx(cls + 0xF30u));
            call(kDeleteLink, 0x010DA3D4u);
        }
        if (READ32(cls + 0xE70u) != 0u)
        {
            WRITE32(cls + 0xE70u, 0u);
            SET_GPR_U64(ctx, 4, sx(cls + 0xF60u));
            call(kDeleteLink, 0x010DA3ECu);
        }
        SET_GPR_U64(ctx, 4, sx(cls + 0xDA0u));
        call(kUnlinkParent, 0x010DA3F4u);
        WRITE32(cls + 0xFD0u, 0u);
        WRITE32(cls + 0x610u, 0u);
        {
            const uint32_t defaults = relocated(rdram, 0x010DA3FCu, 0x010DA404u);
            WRITE32(cls + 0x670u, 0u);
            WRITE64(cls + 0x690u, 0u);
            WRITE32(cls + 0x6B4u, 0u);
            WRITE32(cls + 0xEE8u, 0u);
            WRITE32(cls + 0x6B8u, 0u);
            WRITE32(cls + 0x6C0u, 0u);
            const uint32_t link = READ32(cls + 0xFF0u);
            const __m128i def = READ128(defaults);
            WRITE8(cls + 0xA24u, 1u);
            WRITE128(cls + 0x6A0u, def);
            WRITE8(cls + 0x10A7u, 0u);
            WRITE8(cls + 0xA95u, 0u);
            WRITE8(cls + 0xA94u, 0u);
            if (link != 0u)
            {
                WRITE32(cls + 0xFF0u, 0u);
                SET_GPR_U64(ctx, 4, sx(cls + 0x1010u));
                call(kDeleteLink, 0x010DA44Cu);
            }
        }
        WRITE32(cls + 0xFF4u, 0u);
        SET_GPR_U64(ctx, 4, object());
        call(kFlushColl, 0x010DA458u);

        // The root's velocity cleared unless the kind is one of six.
        constexpr uint32_t kSiteA0 = 0x010DA458u; // a0's lui, kept to 0x10da4b8
        if (fp != relocWord(rdram, ctx, runtime, kSiteA0, 0x010DA45Cu) && fp != relocWord(rdram, ctx, runtime, kSiteS6, 0x010DA464u) &&
            fp != relocWord(rdram, ctx, runtime, 0x010DA46Cu, 0x010DA470u) && fp != relocWord(rdram, ctx, runtime, 0x010DA478u, 0x010DA47Cu) &&
            fp != relocWord(rdram, ctx, runtime, 0x010DA484u, 0x010DA488u) && fp != relocWord(rdram, ctx, runtime, 0x010DA490u, 0x010DA494u))
        {
            WRITE8(cls + 0x10A1u, 0u);
            WRITE8(cls + 0xC96u, 0u);
            WRITE128(cls + 0xAB0u, _mm_setzero_si128());
            WRITE8(cls + 0xC95u, 0u);
            WRITE128(cls + 0x9D0u, _mm_setzero_si128());
        }

        // The xz kinds' frame +4 at least 0.01.
        auto clampHundredth = [&]()
        {
            f[0] = fb(READ32(sp + 4u));
            f[1] = fb(kHundredth);
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            if (!FPU_C_OLT_S(f[1], f[0]))
                f[0] = FPU_MOV_S(f[1]);
        };
        // The layout's position from its velocity (+0x30) x f21 (two kinds,
        // into +0x134 from +0x138 plus the move; a GPR moved into vf6 as qmtc2).
        auto layoutMove = [&](uint32_t layout, int reg)
        {
            { uint32_t bits = bf(f[21]); SET_GPR_U32(ctx, reg, bits); }
            vf[5] = _mm_castsi128_ps(READ128(layout + 0x30u));
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, reg));
            { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))); vf[5] = _mm_blendv_ps(vf[5], res, lanes(0, -1, -1, -1)); }
            WRITE128(sp + 0x10u, _mm_castps_si128(vf[5]));
            const uint32_t from = READ32(layout + 0x138u);
            const uint32_t to = READ32(layout + 0x134u);
            vf[5] = _mm_castsi128_ps(READ128(from));
            vf[6] = _mm_castsi128_ps(READ128(sp + 0x10u));
            { __m128 res = PS2_VADD(vf[5], vf[6]); vf[5] = PS2_VBLEND(vf[5], res, lanes(0, -1, -1, -1)); }
            WRITE128(to, _mm_castps_si128(vf[5]));
        };

        // The kind's step (a1's lui at 0x10da9c8 from the site the path takes).
        uint32_t a1Site;
        if (fp == relocWord(rdram, ctx, runtime, kSiteA0, 0x010DA4B8u))
        {
            clampHundredth();
            stepArgs();
            WRITE32(sp + 4u, bf(f[0]));
            call(kXz, 0x010DA50Cu);
            a1Site = 0x010DA510u;
        }
        else if (fp == relocWord(rdram, ctx, runtime, 0x010DA4C0u, 0x010DA518u))
        {
            clampHundredth();
            stepArgs();
            WRITE32(sp + 4u, bf(f[0]));
            call(kXzFit, 0x010DA56Cu);
            a1Site = 0x010DA570u;
        }
        else
        {
            int sub8340 = -1; // t1 of 010D8340's call
            if (fp == relocWord(rdram, ctx, runtime, kSiteS6, 0x010DA520u) || fp == relocWord(rdram, ctx, runtime, 0x010DA57Cu, 0x010DA580u))
                sub8340 = 0;
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA588u, 0x010DA5B8u))
                sub8340 = 1;
            if (sub8340 >= 0)
            {
                stepArgs();
                SET_GPR_U64(ctx, 9, static_cast<uint64_t>(sub8340));
                call(kSub8340, 0x010DA5F4u);
                WRITE8(cls + 0x10A7u, 1u);
                a1Site = 0x010DA9C4u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA5C0u, 0x010DA600u))
            {
                stepArgs();
                call(kEdgePoint, 0x010DA634u);
                a1Site = 0x010DA638u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA608u, 0x010DA640u))
            {
                stepArgs();
                call(kParentXzFit, 0x010DA674u);
                a1Site = 0x010DA678u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA648u, 0x010DA680u))
            {
                stepArgs();
                call(kEdgeXz, 0x010DA6B4u);
                a1Site = 0x010DA6B8u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA688u, 0x010DA6C0u))
            {
                stepArgs();
                call(kPendulum, 0x010DA984u);
                a1Site = 0x010DA988u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA6C8u, 0x010DA6CCu))
            {
                stepArgs();
                call(kCharNodeParent, 0x010DA700u);
                a1Site = 0x010DA704u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA6D4u, 0x010DA708u))
            {
                // The rotation stabilised to a y turn, the layout moved, then norootdynamics.
                SET_GPR_U64(ctx, 4, object());
                call(kLayoutClass, 0x010DA71Cu);
                const uint64_t s0 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 16, s0);
                SET_GPR_U64(ctx, 4, sx(READ32(static_cast<uint32_t>(s0) + 0x13Cu)));
                call(jalTarget(rdram, kSiteStabilize), 0x010DA728u);
                layoutMove(static_cast<uint32_t>(s0), 4);
                stepArgs();
                call(kNoRootDyn, 0x010DA7ECu);
                a1Site = 0x010DA7F0u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA710u, 0x010DA768u))
            {
                SET_GPR_U64(ctx, 4, object());
                call(kLayoutClass, 0x010DA77Cu);
                layoutMove(GPR_U32(ctx, 2), 5);
                stepArgs();
                call(kNoRootDyn, 0x010DA7ECu);
                a1Site = 0x010DA7F0u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA770u, 0x010DA7B8u))
            {
                stepArgs();
                call(kNoRootDyn, 0x010DA7ECu);
                a1Site = 0x010DA7F0u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, s7Site, 0x010DA7C0u))
            {
                // The follow kind (inline): s1 the frame, s2 the class, s6 its
                // rotation, s0 +0x60, s4 its position, s5 +0x70, s7 +0x138.
                SET_GPR_U64(ctx, 4, object());
                SET_GPR_U64(ctx, 17, spX);
                call(kCharacterClass, 0x010DA80Cu);
                const uint64_t s2 = GPR_U64(ctx, 2);
                const uint32_t c2 = static_cast<uint32_t>(s2);
                SET_GPR_U64(ctx, 4, object());
                SET_GPR_U64(ctx, 18, s2);
                const uint32_t s6 = READ32(c2 + 0x13Cu);
                SET_GPR_U64(ctx, 22, sx(s6));
                const uint32_t s0 = c2 + 0x60u;
                SET_GPR_U64(ctx, 16, sx(s0));
                const uint32_t s4 = READ32(c2 + 0x134u);
                SET_GPR_U64(ctx, 20, sx(s4));
                const uint32_t s5 = c2 + 0x70u;
                SET_GPR_U64(ctx, 21, sx(s5));
                const uint32_t s7 = READ32(c2 + 0x138u);
                SET_GPR_U64(ctx, 23, sx(s7));
                call(kForceUnparent, 0x010DA82Cu);

                WRITE128(s6, _mm_castps_si128(vf[0]));
                WRITE128(s0, _mm_castps_si128(vf[0]));
                WRITE128(s5, _mm_setzero_si128());
                // sp + 0x30: the frame's vector x the class's +0xa0 scale.
                vf[5] = _mm_castsi128_ps(READ128(sp));
                vf[6] = _mm_castsi128_ps(READ128(c2 + 0xA0u));
                { __m128 res = PS2_VMUL(vf[5], vf[6]); vf[5] = PS2_VBLEND(vf[5], res, lanes(0, -1, -1, -1)); }
                WRITE128(sp + 0x30u, _mm_castps_si128(vf[5]));
                // sp + 0x20: the root (+0x1080 when +0x1070 is 1, else +0) plus it, plus +0x70.
                vf[5] = _mm_castsi128_ps(READ128(READ32(c2 + 0x1070u) == 1u ? c2 + 0x1080u : c2));
                vf[6] = _mm_castsi128_ps(READ128(sp + 0x30u));
                { __m128 res = PS2_VADD(vf[5], vf[6]); vf[5] = PS2_VBLEND(vf[5], res, lanes(0, -1, -1, -1)); }
                WRITE128(sp + 0x20u, _mm_castps_si128(vf[5]));
                vf[5] = _mm_castsi128_ps(READ128(sp + 0x20u));
                vf[6] = _mm_castsi128_ps(READ128(s5));
                { __m128 res = PS2_VADD(vf[5], vf[6]); vf[5] = PS2_VBLEND(vf[5], res, lanes(0, -1, -1, -1)); }
                WRITE128(sp + 0x20u, _mm_castps_si128(vf[5]));
                f[0] = fb(kOne);
                setC(ctx, FPU_C_OLE_S(f[0], f[22]));
                if (FPU_C_OLE_S(f[0], f[22]))
                {
                    WRITE128(s4, READ128(sp + 0x20u));
                }
                else
                {
                    // The position blended towards it by f22.
                    { uint32_t bits = bf(f[22]); SET_GPR_U32(ctx, 8, bits); }
                    vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                    vf[5] = _mm_castsi128_ps(READ128(sp + 0x20u));
                    vf[6] = _mm_castsi128_ps(READ128(s4));
                    { __m128 res = PS2_VSUB(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0))); vf[8] = _mm_blendv_ps(vf[8], res, lanes(-1, 0, 0, 0)); }
                    ctx->vu0_acc = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0)));
                    {
                        __m128 mul_res = PS2_VMUL(vf[6], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3, 3, 3, 3)));
                        vf[9] = PS2_VADD(ctx->vu0_acc, mul_res);
                    }
                    WRITE128(s4, _mm_castps_si128(vf[9]));
                    f[0] = fb(kOne);
                }
                // The velocity (+0x30): (position - previous) x f20, w 0; previous = position.
                WRITE32(s4 + 0xCu, bf(f[0]));
                vf[5] = _mm_castsi128_ps(READ128(s4));
                vf[6] = _mm_castsi128_ps(READ128(s7));
                vf[7] = PS2_VSUB(vf[5], vf[6]);
                WRITE128(sp + 0x40u, _mm_castps_si128(vf[7]));
                { uint32_t bits = bf(f[20]); SET_GPR_U32(ctx, 4, bits); }
                vf[5] = _mm_castsi128_ps(READ128(sp + 0x40u));
                vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 4));
                { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))); vf[5] = _mm_blendv_ps(vf[5], res, lanes(0, -1, -1, -1)); }
                WRITE128(c2 + 0x30u, _mm_castps_si128(vf[5]));
                WRITE32(c2 + 0x3Cu, 0u);
                SET_GPR_U64(ctx, 6, sx(s6));
                SET_GPR_U64(ctx, 7, sx(s4));
                SET_GPR_U64(ctx, 4, sx(c2 + 0x4C0u));
                const __m128i position = READ128(s4);
                SET_GPR_U64(ctx, 5, s2);
                WRITE128(s7, position);
                call(kColFinderPos, 0x010DA938u);
                const uint32_t geo = READ32(c2 + 0x19Cu);
                if (geo != 0u)
                    WRITE8(geo + 0x8Cu, 1u);
                a1Site = 0x010DA9C4u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA7FCu, 0x010DA950u))
            {
                stepArgs();
                call(kPendulum, 0x010DA984u);
                a1Site = 0x010DA988u;
            }
            else if (fp == relocWord(rdram, ctx, runtime, 0x010DA958u, 0x010DA990u))
            {
                stepArgs();
                call(kSwim, 0x010DA9C4u);
                a1Site = 0x010DA9C4u;
            }
            else
                a1Site = 0x010DA998u;
        }

        // 4. One kind's +0x34 eased by 20 x f21 (towards 0 from above).
        if (fp == relocWord(rdram, ctx, runtime, a1Site, 0x010DA9C8u))
        {
            f[12] = fb(READ32(cls + 0x34u));
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[0], f[12]));
            if (FPU_C_OLT_S(f[0], f[12]))
            {
                setC(ctx, FPU_C_OLT_S(f[12], f[0]));
                if (FPU_C_OLT_S(f[12], f[0]))
                {
                    f[0] = fb(kTwenty);
                    f[0] = FPU_MUL_S(f[21], f[0]);
                }
                else
                {
                    f[0] = fb(kMinusTwenty);
                    f[0] = FPU_MUL_S(f[21], f[0]);
                }
                f[0] = FPU_ADD_S(f[12], f[0]);
                WRITE32(cls + 0x34u, bf(f[0]));
            }
        }
        f[0] = fb(kOne);
        WRITE32(READ32(sp + 0x90u) + 0xCu, bf(f[0]));
        if (READ32(sp + 0x88u) != 0u)
        {
            SET_GPR_U64(ctx, 4, object());
            WRITE8(cls + 0x531u, 1u);
            call(kForceUnparent, 0x010DAA44u);
        }
        // The +0x1020 parent link dropped when it is not the collision geometry's.
        if (READ32(cls + 0x106Cu) != 0u && READ32(cls + 0x124u) != 0u)
        {
            const uint64_t s1 = sx(cls + 0x1020u);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 4, s1);
            call(kGeomObject, 0x010DAA68u);
            SET_GPR_U64(ctx, 4, sx(cls + 0xC0u));
            const uint64_t s0 = GPR_U64(ctx, 2);
            SET_GPR_U64(ctx, 16, s0);
            call(kGeomObject, 0x010DAA74u);
            if (s0 != GPR_U64(ctx, 2))
            {
                SET_GPR_U64(ctx, 4, s1);
                call(kUnlinkParent, 0x010DAA84u);
            }
        }

        // The interp point and the parent link by its record's flag.
        SET_GPR_U64(ctx, 4, object());
        call(kCharacterClass, 0x010DAA8Cu);
        {
            f[12] = FPU_MOV_S(f[22]);
            const uint64_t s1 = GPR_U64(ctx, 2);
            const uint32_t c1 = static_cast<uint32_t>(s1);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x84u)));
            SET_GPR_U64(ctx, 4, sx(c1 + 0x700u));
            const uint32_t s0 = c1 + 0x1C0u;
            SET_GPR_U64(ctx, 16, sx(s0));
            SET_GPR_U64(ctx, 6, sx(c1 + 0x700u));
            call(kInterpPoint, 0x010DAAA8u);
            const uint32_t record = READ32(s0);
            const uint32_t bit = (READ32(record + 0x68u) >> 25) & 1u;
            if (bit == 0u)
            {
                SET_GPR_U64(ctx, 4, sx(c1 + 0x1020u));
                call(kUnlinkParent, 0x010DABF8u);
            }
            else if (READ32(c1 + 0x106Cu) == 0u)
            {
                f[0] = fb(kOne);
                setC(ctx, FPU_C_OLE_S(f[0], f[22]));
                if (FPU_C_OLE_S(f[0], f[22]))
                {
                    // sp + 0x10: -(t0's vector) turned by the rotation (the
                    // quaternion's matrix, translation the position, w 1).
                    f[0] = fb(kMinusOne);
                    SET_GPR_U64(ctx, 18, sx(sp + 0x10u));
                    { uint32_t bits = bf(f[0]); SET_GPR_U32(ctx, 5, bits); }
                    vf[5] = _mm_castsi128_ps(READ128(READ32(sp + 0x84u)));
                    vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 5));
                    { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))); vf[5] = _mm_blendv_ps(vf[5], res, lanes(0, -1, -1, -1)); }
                    WRITE128(sp + 0x20u, _mm_castps_si128(vf[5]));
                    const uint32_t pos = READ32(sp + 0x90u);
                    vf[8] = _mm_castsi128_ps(READ128(sp + 0x20u));
                    vf[18] = _mm_castsi128_ps(READ128(pos));
                    vf[18] = _mm_blendv_ps(vf[18], vf[0], lanes(-1, 0, 0, 0));
                    const uint32_t q = READ32(sp + 0x8Cu);
                    { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3, 3, 3, 3))); vf[12] = _mm_blendv_ps(vf[12], res, lanes(0, -1, -1, -1)); }
                    vf[11] = _mm_castsi128_ps(READ128(q));
                    { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 3, 2, 1)); vf[14] = _mm_blendv_ps(vf[14], res, lanes(-1, 0, 0, 0)); }
                    { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 3, 2, 1)); vf[15] = _mm_blendv_ps(vf[15], res, lanes(-1, 0, 0, 0)); }
                    { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 3, 2, 1)); vf[16] = _mm_blendv_ps(vf[16], res, lanes(-1, 0, 0, 0)); }
                    {
                        __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 0, 2, 1));
                        __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 1, 0, 2));
                        __m128 res = PS2_VMUL(fs_yzx, ft_zxy);
                        ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(0, -1, -1, -1));
                    }
                    { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[11] = _mm_blendv_ps(vf[11], res, lanes(0, -1, -1, -1)); }
                    { __m128 mul_res = PS2_VMUL(vf[11], vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[10] = _mm_blendv_ps(vf[10], res, lanes(0, -1, -1, -1)); }
                    { __m128 res = PS2_VMUL(vf[11], vf[10]); vf[13] = PS2_VBLEND(vf[13], res, lanes(0, -1, -1, -1)); }
                    {
                        __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 0, 2, 1));
                        __m128 ft_zxy = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3, 1, 0, 2));
                        __m128 res = PS2_VMUL(fs_yzx, ft_zxy);
                        ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(0, -1, -1, -1));
                    }
                    { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 3, 3, 3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, -1, -1, -1)); }
                    { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3, 3, 3, 3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); vf[16] = _mm_blendv_ps(vf[16], res, lanes(0, -1, -1, -1)); }
                    {
                        __m128 fs_yzx = _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(3, 0, 2, 1));
                        __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 1, 0, 2));
                        __m128 res = PS2_VMUL(fs_yzx, ft_zxy);
                        ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(0, -1, -1, -1));
                    }
                    { __m128 mul_res = PS2_VMUL(vf[13], vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[17] = _mm_blendv_ps(vf[17], res, lanes(0, -1, -1, -1)); }
                    {
                        __m128 fs_yzx = _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3, 0, 2, 1));
                        __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3, 1, 0, 2));
                        __m128 res = PS2_VMUL(fs_yzx, ft_zxy);
                        ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(0, -1, -1, -1));
                    }
                    { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[15] = _mm_blendv_ps(vf[15], res, lanes(0, -1, -1, -1)); }
                    { __m128 res = PS2_VSUB(vf[12], vf[17]); vf[14] = PS2_VBLEND(vf[14], res, lanes(0, -1, -1, -1)); }
                    vf[17] = _mm_blendv_ps(vf[17], vf[14], lanes(0, -1, -1, 0));
                    vf[14] = _mm_blendv_ps(vf[14], vf[16], lanes(0, 0, -1, 0));
                    vf[16] = _mm_blendv_ps(vf[16], vf[15], lanes(0, 0, -1, 0));
                    vf[15] = _mm_blendv_ps(vf[15], vf[17], lanes(0, 0, -1, 0));
                    vf[14] = _mm_blendv_ps(vf[14], vf[15], lanes(0, -1, 0, 0));
                    vf[15] = _mm_blendv_ps(vf[15], vf[16], lanes(0, -1, 0, 0));
                    vf[16] = _mm_blendv_ps(vf[16], vf[17], lanes(0, -1, 0, 0));
                    ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0, 0, 0, 0)));
                    { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1, 1, 1, 1))); ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res); }
                    { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2, 2, 2, 2))); ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res); }
                    { __m128 mul_res = PS2_VMUL(vf[18], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3, 3, 3, 3))); vf[10] = PS2_VADD(ctx->vu0_acc, mul_res); }
                    WRITE128(sp + 0x10u, _mm_castps_si128(vf[10]));
                    if (READ32(c1 + 0x124u) != 0u && READ32(c1 + 0xF0u) - 2u < 2u)
                    {
                        // A collision-parented kind (+0xf0 2 or 3): the link by the DB position.
                        const uint64_t link = sx(c1 + 0x1020u), geom = sx(c1 + 0xC0u);
                        SET_GPR_U64(ctx, 16, link);
                        SET_GPR_U64(ctx, 17, geom);
                        SET_GPR_U64(ctx, 4, link);
                        call(kUnlinkParent, 0x010DABC0u);
                        SET_GPR_U64(ctx, 4, link);
                        SET_GPR_U64(ctx, 5, geom);
                        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 18));
                        call(kLinkByDBPos, 0x010DABD0u);
                    }
                    else
                    {
                        SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x8Cu)));
                        SET_GPR_U64(ctx, 4, sx(c1 + 0x1020u));
                        SET_GPR_U64(ctx, 6, sx(sp + 0x10u));
                        call(kSetGPGlobal, 0x010DABE4u);
                        WRITE32(c1 + 0x106Cu, 1u);
                    }
                }
            }
            WRITE8(cls + 0xC95u, 0u);
        }

        // The rotation normalised; the height above +0x10b4.
        normaliseQuat(rdram, ctx, runtime, READ32(sp + 0x8Cu));
        {
            const uint32_t pos = READ32(sp + 0x90u);
            f[1] = fb(READ32(cls + 0x10B4u));
            f[0] = fb(READ32(pos + 4u));
            const int8_t flag35d = static_cast<int8_t>(READ8(cls + 0x35Du));
            f[1] = FPU_SUB_S(f[1], f[0]);
            WRITE32(cls + 0x51Cu, bf(f[1]));
            if (flag35d != 0 || static_cast<int8_t>(READ8(cls + 0x361u)) != 0)
                WRITE32(cls + 0x520u, bf(f[1]));
        }
        // Parented kinds (+0x370 2 or 3): the parent character's link callback and group.
        if (static_cast<uint32_t>(READ16(cls + 0x370u)) - 2u < 2u)
        {
            SET_GPR_U64(ctx, 4, object());
            call(kIsParentChar, 0x010DAC6Cu);
            const uint64_t parent = GPR_U64(ctx, 2);
            if (parent != 0u)
            {
                const uint64_t s1 = sx(static_cast<uint32_t>(parent) + 0xC0u);
                SET_GPR_U64(ctx, 17, s1);
                SET_GPR_U64(ctx, 4, s1);
                call(kGeomObject, 0x010DAC80u);
                const uint64_t s0 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 16, s0);
                if (s0 != object())
                {
                    SET_GPR_U64(ctx, 4, s0);
                    call(kCharacterClass, 0x010DAC98u);
                    SET_GPR_U64(ctx, 4, s0);
                    const uint64_t s2 = GPR_U64(ctx, 2);
                    SET_GPR_U64(ctx, 18, s2);
                    call(kLayoutClass, 0x010DACA4u);
                    const uint64_t layout = GPR_U64(ctx, 2);
                    SET_GPR_U64(ctx, 16, layout);
                    if (layout != 0u)
                    {
                        const uint64_t callback = sx(READ32(static_cast<uint32_t>(layout) + 0x18Cu));
                        if (callback != 0u)
                        {
                            SET_GPR_U64(ctx, 2, callback); // jalr v0
                            SET_GPR_U64(ctx, 4, object());
                            SET_GPR_U64(ctx, 5, s1);
                            call(static_cast<uint32_t>(callback), 0x010DACC4u);
                        }
                    }
                    if (s2 != 0u)
                    {
                        SET_GPR_U64(ctx, 4, object());
                        SET_GPR_U64(ctx, 5, sx(static_cast<uint32_t>(s2) + 0x10E4u));
                        SET_GPR_U64(ctx, 6, 0u);
                        SET_GPR_U64(ctx, 7, 0u);
                        call(jalTarget(rdram, kSiteAddGroup1), 0x010DACDCu);
                    }
                    else if (layout != 0u)
                    {
                        const uint64_t geom = sx(static_cast<uint32_t>(layout) + 0xC0u);
                        SET_GPR_U64(ctx, 17, geom);
                        if (geom != 0u)
                        {
                            SET_GPR_U64(ctx, 4, geom);
                            call(kGeomObject, 0x010DAD04u);
                            if (GPR_U64(ctx, 2) != 0u)
                            {
                                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
                                call(kCharacterClass, 0x010DAD14u);
                                const uint64_t c = GPR_U64(ctx, 2);
                                SET_GPR_U64(ctx, 18, c);
                                if (c != 0u)
                                {
                                    SET_GPR_U64(ctx, 5, sx(static_cast<uint32_t>(c) + 0x10E4u));
                                    SET_GPR_U64(ctx, 4, object());
                                    SET_GPR_U64(ctx, 6, 0u);
                                    SET_GPR_U64(ctx, 7, 0u);
                                    call(jalTarget(rdram, kSiteAddGroup2), 0x010DAD30u);
                                }
                            }
                        }
                    }
                }
            }
        }
        SET_GPR_U64(ctx, 5, sx(READ32(cls + 0x19Cu)));
        SET_GPR_U64(ctx, 4, s3);
        call(kWindGeometry, 0x010DAD3Cu);

        // The up vector from the rotation (sp + 0x50), and the two plane fits.
        {
            const uint32_t q = READ32(cls + 0x13Cu);
            const uint32_t s0 = sp + 0x50u;
            SET_GPR_U64(ctx, 16, sx(s0));
            vf[4] = _mm_castsi128_ps(READ128(q));
            vf[5] = PS2_VMUL(vf[4], vf[4]);
            vf[6] = _mm_shuffle_ps(vf[4], vf[4], _MM_SHUFFLE(0, 3, 2, 1));
            vf[7] = PS2_VMUL(vf[6], vf[4]);
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, -1, 0, 0)); }
            { __m128 res = PS2_VSUB(vf[6], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(3, 3, 3, 3))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, -1, 0, 0)); }
            { __m128 res = PS2_VSUB(vf[0], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(0, 0, 0, 0))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, 0, -1, 0)); }
            { __m128 res = PS2_VSUB(vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(2, 2, 2, 2))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, 0, -1, 0)); }
            { __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, 0, 0, -1)); }
            { __m128 res = PS2_VSUB(vf[0], vf[0]); vf[6] = PS2_VBLEND(vf[6], res, lanes(-1, 0, 0, 0)); }
            { __m128 res = PS2_VADD(vf[0], vf[0]); vf[5] = PS2_VBLEND(vf[5], res, lanes(-1, 0, 0, 0)); }
            { __m128 res = PS2_VMUL(vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3, 3, 3, 3))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, -1, -1, -1)); }
            { __m128 res = PS2_VADD(vf[6], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3, 3, 3, 3))); vf[6] = _mm_blendv_ps(vf[6], res, lanes(0, 0, -1, 0)); }
            WRITE128(s0, _mm_castps_si128(vf[6]));

            SET_GPR_U64(ctx, 4, sx(sp + 0x60u));
            SET_GPR_U64(ctx, 5, sx(s0));
            SET_GPR_U64(ctx, 6, sx(READ32(cls + 0x19Cu) + 0x60u));
            call(kPlaneFit, 0x010DAD90u);
            // Its length (vsqrt of the squared sum in w) into +0x514.
            vf[7] = _mm_castsi128_ps(READ128(sp + 0x60u));
            { __m128 res = PS2_VMUL(vf[7], vf[7]); vf[7] = PS2_VBLEND(vf[7], res, lanes(0, -1, -1, -1)); }
            { __m128 res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(-1, 0, 0, 0)); }
            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, lanes(-1, 0, 0, 0)); }
            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); vf[7] = _mm_blendv_ps(vf[7], res, lanes(-1, 0, 0, 0)); }
            {
                float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 3)));
                ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft);
            }
            { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); SET_GPR_U32(ctx, 2, bits); f[0] = fb(bits); }
            WRITE32(cls + 0x514u, bf(f[0]));
            SET_GPR_U64(ctx, 4, sx(sp + 0x70u));
            SET_GPR_U64(ctx, 5, sx(s0));
            SET_GPR_U64(ctx, 6, sx(READ32(cls + 0x19Cu) + 0x70u));
            call(kPlaneFit, 0x010DADD0u);
            // Its squared length (x) into +0x518; v0 the whole vf7 (qmfc2).
            vf[7] = _mm_castsi128_ps(READ128(sp + 0x70u));
            { __m128 res = PS2_VMUL(vf[7], vf[7]); vf[7] = PS2_VBLEND(vf[7], res, lanes(0, -1, -1, -1)); }
            { __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1))); vf[7] = _mm_blendv_ps(vf[7], res, lanes(0, 0, 0, -1)); }
            { __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2))); vf[7] = _mm_blendv_ps(vf[7], res, lanes(0, 0, 0, -1)); }
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[7]));
            f[0] = fb(GPR_U32(ctx, 2));
            WRITE32(cls + 0x518u, bf(f[0]));
        }

        // The epilogue: the saves back from the frame.
        SET_GPR_U64(ctx, 16, READ64(sp + 0xA0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0xA8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0xB0u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0xB8u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xC0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xC8u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0xD0u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0xD8u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0xE0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xE8u));
        f[23] = fb(READ32(sp + 0x108u));
        f[22] = fb(READ32(sp + 0x100u));
        f[21] = fb(READ32(sp + 0xF8u));
        f[20] = fb(READ32(sp + 0xF0u));
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x010D9FC0u, GetCharaRootDynamics, decomp_hot47d::rootDynamics);
