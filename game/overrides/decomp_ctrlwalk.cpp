// manager_sub_010CA230 (round 41): the control definition walker, the
// player's and the AI's state machine step (0.76% of the game thread
// recompiled, round 40's profile). Called by manager_sub_010CABA0.
//
// a0 the object, a1 its character class (s1), a2 its control status (s4),
// a3 "test the mask bits". *(s1 + 0x750) is a handle to the current list of
// entries (its table's +0xc the count, 16 bytes each: four handles). For
// each entry:
//   +0  its conditions (0x30 bytes each, up to one whose type is 1): each
//       through manager_sub_010C7B28 (decomp_ctrldef.cpp's `cond`, called
//       through the function table: native when it is); the first false one
//       fails the entry;
//   +4  with a3, a mask bit record: bit *(r + 4) of the status's two masks
//       (+0x1c0 | +0x1c8); type 2 (+8): a set bit fails the entry, any other
//       type: a set bit passes it. A pass keeps the bit number (the result,
//       sp + 0x18);
//   +8  on a pass, the action: a record compared with 25 known ones (boot-ELF
//       data the module is linked to: each a relocated lui / lw pair, read
//       from the code): RequestCharacterClassMotionDirection with one of
//       several vectors (constants, the status's, computed ones on the frame
//       by VU0: a direction scaled by -1 and normalised, the Y rotation of
//       the class's angle +-90 or as it is), the GP top / tail directions,
//       RootOffset (sqrt(len) x a vector), TurnLevel (+-1, or a ratio of the
//       status's z and the class's +0x9c0), the play speed, wind, slip
//       requests, a jump to another list (+0xc: the list restarts), or by
//       default RequestCharacterClassMotionChangeWithParam (unless the
//       record's +0 bit 0); then the record's +4 callback if any (a0 the object);
//   +0xc the list to jump to / the motion change's parameter.
// At the end the kept bit number, if the class's +0x930 is that number, the
// class's 28 bytes at +0x930 go to +0xc98 (v0 their second doubleword), else
// +0xc98 = -1 (v0 -1); +0x9c5 = 0 either way.
//
// Everything natively: the conditions, the requests, the callbacks and
// iosGetRotYVector as guest calls with the original's ra and frame (sp -
// 0x90, its words written as the original writes them: the prologue's saved
// registers, a0 / a3 at +0x10 / +0x14, the result, index, count and mask at
// +0x18 .. +0x28, the vectors at +0). The FPU / VU0 statements of the action
// cases as the generated code has them (FPU_* / PS2_V* macros, ps2FpuDivide,
// ps2FpuSqrt), on the context's registers (rare: only on a pass); f20 is 0
// there (the original's mtc1, restored at its end). The within-module jals
// go to their fixed targets as the generated code's do; iosGetRotYVector's
// is read from the site (relocated), as the generated code reads it.
// SOTC_MOTION=0: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ctrlwalk
{
    using sotc_decomp::codeWord;
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kWalk = 0x010CA230u, kCond = 0x010C7B28u;
    constexpr uint32_t kDirection = 0x010D2B48u, kByGPTop = 0x010D2B60u, kByGPTail = 0x010D2B70u, kRootOffset = 0x010D2BA0u,
                       kTurnLevel = 0x010D2B90u, kSpeedUp = 0x010D2BB0u, kSpeedDown = 0x010D2BC0u, kSpeedDown2 = 0x010D2BD0u,
                       kWind = 0x010D2BE0u, kSlip = 0x010D2A88u, kNoClimbSlip = 0x010D2AA0u, kChange = 0x010D2B18u;

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        // The 0x3ffff / index handle as the walker resolves it (`empty`: the
        // address holding the empty table's pointer).
        uint32_t resolve(uint32_t h, uint32_t empty) const
        {
            const uint32_t v = u32(h), off = v & 0x3FFFFu;
            const uint32_t t = off ? u32(h - off) : 0u;
            if (t == 0u || t == u32(empty))
                return 0u;
            return u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * u32(u32(t + 8u) + 0xCu);
        }
        void call(uint32_t target, uint32_t ra, uint32_t sp) const { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp); }
    };

    // The action records' lui / lw sites, in the compare chain's order.
    constexpr uint32_t kActions[25][2] = {
        {0x010CA4FCu, 0x010CA500u}, {0x010CA508u, 0x010CA528u}, {0x010CA530u, 0x010CA550u}, {0x010CA558u, 0x010CA578u},
        {0x010CA580u, 0x010CA5A0u}, {0x010CA5A8u, 0x010CA5C8u}, {0x010CA5D0u, 0x010CA5E8u}, {0x010CA5F0u, 0x010CA608u},
        {0x010CA610u, 0x010CA658u}, {0x010CA660u, 0x010CA6A8u}, {0x010CA6B0u, 0x010CA6E8u}, {0x010CA6F0u, 0x010CA708u},
        {0x010CA710u, 0x010CA758u}, {0x010CA760u, 0x010CA778u}, {0x010CA780u, 0x010CA798u}, {0x010CA7A0u, 0x010CA810u},
        {0x010CA818u, 0x010CA830u}, {0x010CA838u, 0x010CA850u}, {0x010CA858u, 0x010CA870u}, {0x010CA878u, 0x010CA890u},
        {0x010CA898u, 0x010CA8B0u}, {0x010CA8B8u, 0x010CA8D0u}, {0x010CA8D8u, 0x010CA948u}, {0x010CA950u, 0x010CA9D8u},
        {0x010CA9E0u, 0x010CAA58u}};

    // vf5 = v (xyz scaled by vf6x = -1.0: fp's low word), normalised by
    // vrsqrt, stored at sp (cases 0x10ca614 / 0x10ca664).
    void negNormalised(const G &g, uint32_t src, uint32_t sp)
    {
        R5900Context *ctx = g.ctx;
        __m128 *vf = ctx->vu0_vf;
        // qmtc2 $fp: fp = 0xffffffffbf800000 (lui), its upper half as it was.
        __m128i fpv = ctx->r[30];
        fpv = _mm_insert_epi64(fpv, static_cast<int64_t>(0xFFFFFFFFBF800000ull), 0);
        vf[5] = g.q(src);
        vf[6] = _mm_castsi128_ps(fpv);
        { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
        g.putq(sp, vf[5]);
        vf[5] = g.q(sp);
        { __m128 res = PS2_VMUL(vf[5], vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
        g.putq(sp, vf[5]);
    }

    // The quaternion at *(s1 + 0x13c) into a direction (cases 0x10ca988 /
    // 0x10ca9ec), stored at sp.
    void quatDirection(const G &g, uint32_t s1, uint32_t sp)
    {
        R5900Context *ctx = g.ctx;
        __m128 *vf = ctx->vu0_vf;
        vf[4] = g.q(g.u32(s1 + 0x13Cu));
        { __m128 res = PS2_VMUL(vf[4], vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf[4], vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[6], vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[6], vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[8] = PS2_VBLEND(vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[0], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[8], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[0], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[0], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[6], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
        g.putq(sp, vf[6]);
    }

    inline float fbits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline void condBit(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // TurnLevel's ratio (cases 0x10ca714 / 0x10ca7a4): f3 = +-1 by the
    // sign of the status's z (`neg`: -1 when z < 0), f12 = f3 x (|z| or z
    // minus the class's +0x9c0, as the original) / (1 - it).
    void turnRatio(const G &g, uint32_t s1, uint32_t s5, bool negWhenBelow)
    {
        R5900Context *ctx = g.ctx;
        const float zero = 0.0f;
        ctx->f[1] = g.f32(s5 + 8u);
        bool c = FPU_C_OLT_S(ctx->f[1], zero);
        condBit(ctx, c);
        ctx->f[3] = fbits(negWhenBelow ? 0xBF800000u : 0x3F800000u);
        ctx->f[2] = g.f32(s1 + 0x9C0u);
        if (!c)
        {
            ctx->f[3] = fbits(negWhenBelow ? 0x3F800000u : 0xBF800000u);
            c = FPU_C_OLT_S(ctx->f[1], zero);
            condBit(ctx, c);
        }
        ctx->f[0] = fbits(0x3F800000u);
        ctx->f[12] = FPU_SUB_S(ctx->f[0], ctx->f[2]);
        if (c)
        {
            ctx->f[0] = FPU_NEG_S(ctx->f[1]);
            ctx->f[0] = FPU_SUB_S(ctx->f[0], ctx->f[2]);
        }
        else
            ctx->f[0] = FPU_SUB_S(ctx->f[1], ctx->f[2]);
        ctx->f[0] = FPU_MUL_S(ctx->f[3], ctx->f[0]);
        ctx->f[12] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[12]);
    }

    void walk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kWalk)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x90u;
        const uint32_t obj = GPR_U32(ctx, 4), s1 = GPR_U32(ctx, 5), s4 = GPR_U32(ctx, 6), a3in = GPR_U32(ctx, 7);
        const uint32_t s5 = s4 + 0x80u;
        // The prologue's stores.
        for (int r = 16; r <= 23; ++r)
            g.put64(sp + 0x30u + 8u * static_cast<uint32_t>(r - 16), GPR_U64(ctx, r));
        g.put64(sp + 0x70u, GPR_U64(ctx, 30));
        g.put64(sp + 0x78u, GPR_U64(ctx, 31));
        {
            uint32_t f20;
            std::memcpy(&f20, &ctx->f[20], 4);
            g.put32(sp + 0x80u, f20);
        }
        g.put32(sp + 0x10u, obj);
        g.put32(sp + 0x14u, a3in);

        const uint32_t emptyCount = relocated(rdram, 0x010CA2B0u, 0x010CA2B4u), emptyList = relocated(rdram, 0x010CA2F8u, 0x010CA2FCu);
        const uint32_t s7 = codeWord(rdram, 0x010CA360u) << 16;
        auto low = [&](uint32_t lo) { return s7 + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(codeWord(rdram, lo) & 0xFFFFu))); };
        const uint32_t emptyEntry = low(0x010CA388u), emptyMask = low(0x010CA428u), emptyAction = low(0x010CA4D0u),
                       emptyJump = low(0x010CA900u), emptyParam = low(0x010CAA98u);

        uint32_t list = g.u32(s1 + 0x750u);
        for (;;)
        {
            // 0x10ca274
            g.put32(sp + 0x18u, 0xFFFFFFFFu);
            if (list == 0u)
                break;
            {
                const uint32_t v = g.u32(list), off = v & 0x3FFFFu;
                const uint32_t t = off ? g.u32(list - off) : 0u;
                g.put32(sp + 0x20u, t == 0u || t == g.u32(emptyCount) ? 0u : g.u32(t + 0xCu));
            }
            uint32_t s3 = g.resolve(list, emptyList);
            g.put32(sp + 0x1Cu, 0u);
            g.put64(sp + 0x28u, g.u64(s4 + 0x1C0u) | g.u64(s4 + 0x1C8u));
            if (g.s32(sp + 0x20u) <= 0)
                break;
            bool restart = false;
            for (;;)
            {
                // 0x10ca368: the entry's conditions
                uint32_t s0 = g.resolve(s3, emptyEntry);
                bool pass = true;
                if (g.s32(s0) != 1)
                {
                    for (;;)
                    {
                        SET_GPR_U32(ctx, 4, g.u32(sp + 0x10u));
                        SET_GPR_U32(ctx, 5, s1);
                        SET_GPR_U32(ctx, 6, s4);
                        SET_GPR_U32(ctx, 7, s0);
                        g.call(kCond, 0x010CA3E0u, sp);
                        const uint64_t v0 = GPR_U64(ctx, 2);
                        s0 += 0x30u;
                        if (v0 == 0u)
                        {
                            pass = false;
                            break;
                        }
                        if (g.s32(s0) == 1)
                            break;
                    }
                }
                // 0x10ca3fc: the mask bit
                if (g.u32(sp + 0x14u) != 0u)
                {
                    const uint32_t r = g.resolve(s3 + 4u, emptyMask);
                    if (r != 0u)
                    {
                        const uint64_t bit = g.u64(sp + 0x28u) & (1ull << (g.u32(r + 4u) & 63u));
                        const int32_t type = g.s32(r + 8u);
                        if (type != 1 && type == 2)
                        {
                            if (bit != 0u)
                                pass = false;
                        }
                        else if (bit != 0u)
                            pass = true;
                    }
                    if (pass && r != 0u)
                        g.put32(sp + 0x18u, g.u32(r + 4u));
                }
                if (pass)
                {
                    // 0x10ca4a8: the action
                    s0 = g.resolve(s3 + 8u, emptyAction);
                    int which = 25;
                    for (int k = 0; k < 25; ++k)
                        if (s0 == g.u32(relocated(rdram, kActions[k][0], kActions[k][1])))
                        {
                            which = k;
                            break;
                        }
                    auto direction = [&](uint32_t a1, uint32_t ra)
                    {
                        SET_GPR_U32(ctx, 4, s1);
                        SET_GPR_U32(ctx, 5, a1);
                        g.call(kDirection, ra, sp);
                    };
                    auto plain = [&](uint32_t target, uint32_t ra)
                    {
                        SET_GPR_U32(ctx, 4, s1);
                        g.call(target, ra, sp);
                    };
                    auto rotY = [&]()
                    {
                        // 0x10caa38: iosGetRotYVector(sp, sp, f12 x f0), then the direction from sp
                        SET_GPR_U32(ctx, 4, sp);
                        SET_GPR_U32(ctx, 5, sp);
                        ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[0]);
                        g.call(sotc_decomp::jalTarget(rdram, 0x010CAA3Cu), 0x010CAA44u, sp);
                        direction(sp, 0x010CAA50u);
                    };
                    bool callback = true;
                    switch (which)
                    {
                    case 0:
                        if (g.u32(s4 + 0x100u) != 0u)
                            direction(s5 + 0x10u, 0x010CA520u);
                        break;
                    case 1: direction(relocated(rdram, 0x010CA534u, 0x010CA540u), 0x010CA544u); break;
                    case 2: direction(relocated(rdram, 0x010CA55Cu, 0x010CA568u), 0x010CA56Cu); break;
                    case 3: direction(relocated(rdram, 0x010CA584u, 0x010CA590u), 0x010CA594u); break;
                    case 4: direction(relocated(rdram, 0x010CA5ACu, 0x010CA5B8u), 0x010CA5BCu); break;
                    case 5: plain(kByGPTop, 0x010CA5DCu); break;
                    case 6: direction(s4 + 0x190u, 0x010CA600u); break;
                    case 7:
                        negNormalised(g, s4 + 0x190u, sp);
                        direction(sp, 0x010CAA50u);
                        break;
                    case 8:
                        negNormalised(g, s4 + 0x160u, sp);
                        direction(sp, 0x010CAA50u);
                        break;
                    case 9:
                    {
                        ctx->f[0] = g.f32(s5);
                        ctx->f[0] = ps2FpuSqrt(ctx->fcr31, ctx->f[0]);
                        uint32_t bits;
                        std::memcpy(&bits, &ctx->f[0], 4);
                        __m128i v1 = ctx->r[3];
                        v1 = _mm_insert_epi64(v1, static_cast<int64_t>(static_cast<int32_t>(bits)), 0);
                        __m128 *vf = ctx->vu0_vf;
                        vf[5] = g.q(s5 + 0x10u);
                        vf[6] = _mm_castsi128_ps(v1);
                        { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
                        g.putq(sp, vf[5]);
                        SET_GPR_U32(ctx, 4, s1);
                        SET_GPR_U32(ctx, 5, sp);
                        g.call(kRootOffset, 0x010CA6E0u, sp);
                        break;
                    }
                    case 10: plain(kByGPTail, 0x010CA6FCu); break;
                    case 11:
                        turnRatio(g, s1, s5, false);
                        plain(kTurnLevel, 0x010CA804u);
                        break;
                    case 12:
                        ctx->f[12] = fbits(0xBF800000u);
                        plain(kTurnLevel, 0x010CA804u);
                        break;
                    case 13:
                        ctx->f[12] = fbits(0x3F800000u);
                        plain(kTurnLevel, 0x010CA804u);
                        break;
                    case 14:
                        turnRatio(g, s1, s5, true);
                        plain(kTurnLevel, 0x010CA804u);
                        break;
                    case 15: plain(kSpeedUp, 0x010CA824u); break;
                    case 16: plain(kSpeedDown, 0x010CA844u); break;
                    case 17: plain(kSpeedDown2, 0x010CA864u); break;
                    case 18: plain(kWind, 0x010CA884u); break;
                    case 19: plain(kSlip, 0x010CA8A4u); break;
                    case 20: plain(kNoClimbSlip, 0x010CA8C4u); break;
                    case 21:
                    {
                        // 0x10ca8dc: another list (+0xc); the list restarts
                        const uint32_t next = g.resolve(s3 + 0xCu, emptyJump);
                        const uint32_t cur = g.u32(s1 + 0x750u);
                        if (cur != next)
                        {
                            g.put32(s1 + 0x754u, cur);
                            g.put32(s1 + 0x750u, next);
                        }
                        list = next;
                        restart = true;
                        callback = false;
                        break;
                    }
                    case 22:
                    {
                        ctx->f[12] = g.f32(s1 + 0xF50u);
                        const bool c = FPU_C_OLT_S(ctx->f[12], 0.0f);
                        condBit(ctx, c);
                        ctx->f[0] = fbits(0x42B40000u);
                        if (c)
                        {
                            ctx->f[0] = FPU_ADD_S(ctx->f[12], ctx->f[0]);
                            ctx->f[0] = FPU_NEG_S(ctx->f[0]);
                        }
                        else
                            ctx->f[0] = FPU_SUB_S(ctx->f[0], ctx->f[12]);
                        quatDirection(g, s1, sp);
                        ctx->f[12] = fbits(0x3C8EFA35u);
                        rotY();
                        break;
                    }
                    case 23:
                        ctx->f[12] = g.f32(s1 + 0xF50u);
                        quatDirection(g, s1, sp);
                        ctx->f[0] = fbits(0x3C8EFA35u);
                        rotY();
                        break;
                    case 24: break;
                    default:
                        // 0x10caa64: the motion change, unless the record's bit 0
                        if ((g.u32(s0) & 1u) == 0u)
                        {
                            const uint32_t param = g.resolve(s3 + 0xCu, emptyParam);
                            ctx->f[12] = 0.0f;
                            SET_GPR_U32(ctx, 4, s1);
                            SET_GPR_U32(ctx, 5, s0);
                            SET_GPR_U32(ctx, 6, param);
                            g.call(kChange, 0x010CAAD4u, sp);
                        }
                        break;
                    }
                    if (restart)
                        break;
                    if (callback)
                    {
                        // 0x10caad8: the record's callback
                        const uint32_t fn = g.u32(s0 + 4u);
                        if (fn != 0u)
                        {
                            SET_GPR_U32(ctx, 4, g.u32(sp + 0x10u));
                            g.call(fn, 0x010CAAE8u, sp);
                        }
                    }
                }
                // 0x10caaec: the next entry
                s3 += 0x10u;
                const int32_t i = g.s32(sp + 0x1Cu) + 1;
                g.put32(sp + 0x1Cu, static_cast<uint32_t>(i));
                if (!(i < g.s32(sp + 0x20u)))
                    break;
            }
            if (!restart)
                break;
        }
        // 0x10cab04: the kept bit number against the class's +0x930
        const int32_t kept = g.s32(sp + 0x18u);
        uint64_t v0 = ~0ull;
        if (kept >= 0 && kept == g.s32(s1 + 0x930u))
        {
            uint8_t buf[0x1C];
            std::memcpy(buf, rdram + ((s1 + 0x930u) & PS2_RAM_MASK), sizeof(buf));
            std::memcpy(&v0, buf + 8, 8);
            std::memcpy(rdram + ((s1 + 0xC98u) & PS2_RAM_MASK), buf, sizeof(buf));
        }
        else
            g.put32(s1 + 0xC98u, 0xFFFFFFFFu);
        rdram[(s1 + 0x9C5u) & PS2_RAM_MASK] = 0u;
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010CA230u, manager_sub_010CA230, decomp_ctrlwalk::walk);
