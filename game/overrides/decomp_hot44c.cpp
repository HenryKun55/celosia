// Round 44 (SOTC_HOTLIST44=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions from the
// game thread's profile, transcribed from build/runner's generated code.
//
// - gamecore_sub_013448C0 (0x13448c0): the attachment fix-up of a group's
//   objects (a0: the group). For each object s5 of the group
//   (isysGroupForFirst / isysGroupForNext): when its layout class has a
//   geometry pointer (+0x124), its legal parent (SCRGetGetGeometryPointerLegalParentObj
//   of class + 0xc0) is looked for in the rest of the group
//   (isysGetGroupSrhNext); found, the object is moved right after it
//   (_isysDeleteGroup, isysAddGroupRelativeWithLinkParam(obj, group, entry,
//   1, 0, 0)). Otherwise, until one such move was made (fp), a character of
//   kind 2 (+0x370) is moved after the last entry of the rest of the group
//   that is a character of kind 3 / 4, or whose geometry parent is a
//   character of kind 4. The end: when the last object word read is not 0,
//   isysGroupForExit(it, 0) as a tail jump (its v0 is the result), else a
//   plain return (v0 as the last call left it).
// - gamecore_sub_0141F310 (0x141f310): a camera parameter condition table's
//   match. a0 the manager, a1 the camera work (a1 + 0x1270 the segment
//   status, a1 + 0x23c the target), a2 / a3 / t0 / t1 / t2 the condition's
//   keys. The frame (sp - 0xb0) holds the record the table is matched
//   against: +0 a3, +4 t0, +8 gamecore_sub_01426218(0), +0xc the count of
//   target entries (0, 1 or 2: the target character's +0x950 object and the
//   target itself when they differ, gamecore_sub_01426100 fills +0x1c / +0x28
//   and +0x20 / +0x2c), +0x10 / +0x14 their objects, +0x34 t1, +0x38 t2,
//   +0x40 the table's count, +0x44.. the arguments. Each 0x48-byte entry
//   (from the status call's result) matches when +4 / +8 / +0x10 equal the
//   record's (+8 and +0x10 also match the wildcard 1000000) and, for one of
//   the target entries, +0x24 equals its +0x10.. word and +0x2c its +0x28..
//   word (or the wildcard) and is_pass_camera_etc_condition(+0x34, t1 + 0x10)
//   is 1. The first match: reset_game_camera_param_update_data_manager(a0,
//   1), set_update_data_game_camera_param_update_data_manager(a0, 0, entry,
//   a2, a1) with f12 = 1, v0 1; none: v0 0.
// - gamecore_sub_013E30D0 (0x13e30d0): a small animal's movement step (a0
//   its AI record s1; s4 = isysGetThisSubThread). Only while the state
//   (+0x26c) is below 18 (else v0 0). The motion speed (+0x30c - 1) x
//   *(+0x20) + 1, at least 0.1, to SCRSetCharaMotionPlaySpeedRatioDirect; q
//   = |+0x2e0| (vsqrt); the turn rate f12 (from +0x760 when the kind +0x274
//   is 0 or 10, limited to +-1.2566, plus -+0x2f4's term, each limited to
//   +-1.0995) and f13 for AI_UpdateVelocity(+0x308, *(+0x20)); the heading
//   (+0x8d4 eased for type 6) to gamecore_sub_013D8B38. Then the layout
//   class s2 of the object: sp+0 = the character's +0x9e0 / speed; for the
//   states 14 / 15 the tuning vector (relocated, x the block's +0x190) and
//   sp+0 rotated by the layout's quaternion (+0x13c), for the others but 20
//   the same with AI_GetYRotQuaternion's rotation of +0x2e0; their sum added
//   to the layout's position (+0x134, y no higher than +0x79c); last the
//   position += +0x8e0. v0 the last GetLayoutClass's. The FPU, the
//   condition bit and the VU0 registers as the code leaves them (the
//   generator's statements on the context's registers; the five rotation
//   blocks are one helper: the generator's text of each is the same).
//
// Not as the original: the scratch GPRs (only the call arguments and the
// registers the code's callees are given are set); the callee-saved
// registers are never changed in the context (the code restores every one
// from its frame), the frame's saves are written as the code writes them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot44c
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- gamecore_sub_013448C0 ------------------------------------------------
    constexpr uint32_t kGroupFix = 0x013448C0u;
    constexpr uint32_t kLegalParent = 0x0137D640u; // same module: a constant jal
    // The relocated jal sites.
    constexpr uint32_t kSiteForFirst = 0x013448F8u, kSiteLayout1 = 0x01344910u, kSiteDelete1 = 0x0134495Cu, kSiteAdd1 = 0x01344978u;
    constexpr uint32_t kSiteSrhNext1 = 0x01344988u, kSiteChara1 = 0x013449B0u, kSiteChara2 = 0x013449E8u, kSiteLayout2 = 0x01344A0Cu;
    constexpr uint32_t kSiteGeometry = 0x01344A2Cu, kSiteChara3 = 0x01344A34u, kSiteSrhNext2 = 0x01344A58u, kSiteDelete2 = 0x01344A7Cu;
    constexpr uint32_t kSiteAdd2 = 0x01344A98u, kSiteForNext = 0x01344AA4u, kSiteForExit = 0x01344AECu;

    void groupFix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kGroupFix, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        const uint64_t group = GPR_U64(ctx, 4); // s6
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x48u, ra);

        auto call = [&](uint32_t site) { callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };
        // _isysDeleteGroup(obj, group, 0); isysAddGroupRelativeWithLinkParam(obj, group, after, 1, 0, 0).
        auto move = [&](uint64_t obj, uint64_t after, uint32_t siteDelete, uint32_t siteAdd)
        {
            SET_GPR_U64(ctx, 4, obj);
            SET_GPR_U64(ctx, 5, group);
            SET_GPR_U64(ctx, 6, 0u);
            call(siteDelete);
            SET_GPR_U64(ctx, 4, obj);
            SET_GPR_U64(ctx, 5, group);
            SET_GPR_U64(ctx, 6, after);
            SET_GPR_U64(ctx, 7, 1u);
            SET_GPR_U64(ctx, 8, 0u);
            SET_GPR_U64(ctx, 9, 0u);
            call(siteAdd);
        };

        uint64_t s5 = 0u, fp = 0u;
        SET_GPR_U64(ctx, 5, 0u);
        call(kSiteForFirst);
        uint64_t s2 = GPR_U64(ctx, 2);
        while (s2 != 0u)
        {
            s5 = sx(READ32(static_cast<uint32_t>(s2)));
            SET_GPR_U64(ctx, 4, s5);
            call(kSiteLayout1);
            const uint32_t cls = GPR_U32(ctx, 2);
            if (cls != 0u && READ32(cls + 0x124u) != 0u)
            {
                SET_GPR_U32(ctx, 4, cls + 0xC0u);
                callGuest(rdram, ctx, runtime, kLegalParent, 0x01344938u, sp);
                const uint64_t parent = GPR_U64(ctx, 2);
                if (parent != 0u)
                {
                    for (uint64_t s0 = s2; s0 != 0u;)
                    {
                        if (sx(READ32(static_cast<uint32_t>(s0))) == parent)
                        {
                            move(s5, s0, kSiteDelete1, kSiteAdd1);
                            break;
                        }
                        SET_GPR_U64(ctx, 4, s0);
                        call(kSiteSrhNext1);
                        s0 = GPR_U64(ctx, 2);
                    }
                }
            }
            else if (fp == 0u)
            {
                SET_GPR_U64(ctx, 4, s5);
                call(kSiteChara1);
                const uint32_t chara = GPR_U32(ctx, 2);
                if (chara != 0u && static_cast<int16_t>(READ16(chara + 0x370u)) == 2)
                {
                    uint64_t s3 = 0u, s4 = 0u;
                    for (uint64_t s0 = s2; s0 != 0u;)
                    {
                        const uint64_t s1 = sx(READ32(static_cast<uint32_t>(s0)));
                        SET_GPR_U64(ctx, 4, s1);
                        call(kSiteChara2);
                        const uint32_t c = GPR_U32(ctx, 2);
                        bool take = false;
                        if (c != 0u && static_cast<uint32_t>(static_cast<uint32_t>(READ16(c + 0x370u)) - 3u) < 2u)
                            take = true;
                        else
                        {
                            SET_GPR_U64(ctx, 4, s1);
                            call(kSiteLayout2);
                            const uint32_t lc = GPR_U32(ctx, 2);
                            if (lc != 0u && READ32(lc + 0x124u) != 0u)
                            {
                                SET_GPR_U32(ctx, 4, lc + 0xC0u);
                                call(kSiteGeometry);
                                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
                                call(kSiteChara3);
                                const uint32_t pc = GPR_U32(ctx, 2);
                                take = pc != 0u && static_cast<int16_t>(READ16(pc + 0x370u)) == 4;
                            }
                        }
                        if (take)
                        {
                            s3 = s1;
                            s4 = s0;
                        }
                        SET_GPR_U64(ctx, 4, s0);
                        call(kSiteSrhNext2);
                        s0 = GPR_U64(ctx, 2);
                    }
                    if (s3 != 0u)
                    {
                        fp = 1u;
                        move(s5, s4, kSiteDelete2, kSiteAdd2);
                    }
                }
            }
            SET_GPR_U64(ctx, 4, s2);
            SET_GPR_U64(ctx, 5, 0u);
            call(kSiteForNext);
            s2 = GPR_U64(ctx, 2);
        }
        SET_GPR_U64(ctx, 4, s5);
        if (s5 != 0u)
        {
            // j isysGroupForExit with the frame popped: it returns to our caller.
            SET_GPR_U64(ctx, 5, 0u);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteForExit), static_cast<uint32_t>(ra), static_cast<uint32_t>(entrySp));
        }
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- gamecore_sub_0141F310 ------------------------------------------------
    constexpr uint32_t kCamCond = 0x0141F310u;
    // Same module: constant jals.
    constexpr uint32_t kPlaceStatus = 0x01413350u, kTargetObj = 0x0141A7B8u, kSub01426218 = 0x01426218u, kSub01426100 = 0x01426100u;
    constexpr uint32_t kPassCondition = 0x014119E8u, kResetParam = 0x0142C288u, kSetParam = 0x0142C330u;
    constexpr uint32_t kSiteChara = 0x0141F38Cu; // GetCharacterClass (relocated)
    constexpr uint32_t kAny = 0x000F4240u;        // the wildcard 1000000

    void camCondition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kCamCond, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);
        const uint64_t a3 = GPR_U64(ctx, 7), t0 = GPR_U64(ctx, 8), t1 = GPR_U64(ctx, 9), t2 = GPR_U64(ctx, 10);
        WRITE32(sp + 0x48u, a1);
        WRITE32(sp + 0x44u, a0);
        WRITE32(sp + 0x4Cu, a2);
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 30));
        WRITE64(sp + 0xA8u, ra);
        WRITE32(sp + 0x40u, 0u);
        auto finish = [&](uint64_t v0)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };

        SET_GPR_U32(ctx, 4, sp + 0x40u);
        SET_GPR_U64(ctx, 5, 0u);
        SET_GPR_U32(ctx, 6, READ32(sp + 0x48u) + 0x1270u);
        callGuest(rdram, ctx, runtime, kPlaceStatus, 0x0141F374u, sp);
        WRITE32(sp + 0x50u, GPR_U32(ctx, 2));
        SET_GPR_U32(ctx, 4, READ32(sp + 0x48u) + 0x23Cu);
        callGuest(rdram, ctx, runtime, kTargetObj, 0x0141F384u, sp);
        const uint64_t target = GPR_U64(ctx, 2); // s3
        SET_GPR_U64(ctx, 4, target);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteChara), kSiteChara + 8u, sp);
        const uint32_t chara = GPR_U32(ctx, 2); // s2
        WRITE32(sp + 0x0u, static_cast<uint32_t>(a3));
        WRITE32(sp + 0x4u, static_cast<uint32_t>(t0));
        SET_GPR_U64(ctx, 4, 0u);
        callGuest(rdram, ctx, runtime, kSub01426218, 0x0141F3A8u, sp);
        WRITE32(sp + 0x8u, GPR_U32(ctx, 2));
        uint32_t entries = 0u; // s5
        if (chara != 0u)
        {
            WRITE32(sp + 0x10u, READ32(chara + 0x950u));
            entries = 1u;
            SET_GPR_U32(ctx, 4, sp + 0x1Cu);
            SET_GPR_U32(ctx, 5, sp + 0x28u);
            SET_GPR_U64(ctx, 7, t1);
            SET_GPR_S32(ctx, 6, READ32(chara + 0x950u));
            callGuest(rdram, ctx, runtime, kSub01426100, 0x0141F3D0u, sp);
            if (sx(READ32(chara + 0x950u)) != target)
            {
                SET_GPR_U32(ctx, 4, sp + 0x20u);
                SET_GPR_U32(ctx, 5, sp + 0x2Cu);
                SET_GPR_U64(ctx, 6, target);
                SET_GPR_U64(ctx, 7, t1);
                WRITE32(sp + 0x14u, static_cast<uint32_t>(target));
                callGuest(rdram, ctx, runtime, kSub01426100, 0x0141F3F0u, sp);
                entries = 2u;
            }
        }
        WRITE32(sp + 0x38u, static_cast<uint32_t>(t2));
        WRITE32(sp + 0xCu, entries);
        WRITE32(sp + 0x34u, static_cast<uint32_t>(t1));

        // The table's count is read again after each entry (the code's lw in the loop's branch).
        uint32_t entry = READ32(sp + 0x50u); // s2
        for (int32_t i = 0; i < static_cast<int32_t>(READ32(sp + 0x40u)); ++i, entry += 0x48u)
        {
            bool found = false;
            do
            {
                if (READ32(entry + 4u) != READ32(sp))
                    break;
                const uint32_t k1 = READ32(entry + 8u);
                if (k1 != READ32(sp + 4u) && k1 != kAny)
                    break;
                const uint32_t k2 = READ32(entry + 0x10u);
                if (k2 != READ32(sp + 8u) && k2 != kAny)
                    break;
                // The target entries: +0x1c.. objects' words against +0x24, +0x28.. against +0x2c.
                for (int32_t j = 0; j < static_cast<int32_t>(READ32(sp + 0xCu)); ++j)
                {
                    if (READ32(entry + 0x24u) != READ32(sp + 0x1Cu + 4u * static_cast<uint32_t>(j)))
                        continue;
                    const uint32_t k3 = READ32(entry + 0x2Cu);
                    if (k3 != READ32(sp + 0x28u + 4u * static_cast<uint32_t>(j)) && k3 != kAny)
                        continue;
                    SET_GPR_S32(ctx, 4, READ32(entry + 0x34u));
                    SET_GPR_U32(ctx, 5, READ32(sp + 0x34u) + 0x10u);
                    callGuest(rdram, ctx, runtime, kPassCondition, 0x0141F4C0u, sp);
                    if (GPR_U64(ctx, 2) == 1u)
                    {
                        found = true;
                        break;
                    }
                }
            } while (false);
            if (!found)
                continue;
            SET_GPR_S32(ctx, 4, READ32(sp + 0x44u));
            SET_GPR_U64(ctx, 5, 1u);
            callGuest(rdram, ctx, runtime, kResetParam, 0x0141F4F8u, sp);
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_S32(ctx, 4, READ32(sp + 0x44u));
            SET_GPR_U32(ctx, 6, entry);
            SET_GPR_U64(ctx, 1, sx(0x3F800000u));
            ctx->f[12] = 1.0f;
            SET_GPR_S32(ctx, 7, READ32(sp + 0x4Cu));
            SET_GPR_S32(ctx, 8, READ32(sp + 0x48u));
            callGuest(rdram, ctx, runtime, kSetParam, 0x0141F518u, sp);
            return finish(1u);
        }
        finish(0u);
    }
    // ---- gamecore_sub_013E30D0 ------------------------------------------------
    constexpr uint32_t kAnimalMove = 0x013E30D0u;
    // Same module: constant jals.
    constexpr uint32_t kSetSpeedRatio = 0x0137CA60u, kUpdateVelocity = 0x013D3DA0u, kSub013D8B38 = 0x013D8B38u, kYRotQuaternion = 0x013D3C68u;
    // The relocated jal sites (isysGetThisSubThread twice, GetLayoutClass twice, GetCharacterClass).
    constexpr uint32_t kSiteThread1 = 0x013E30F0u, kSiteLayout3 = 0x013E33DCu, kSiteThread2 = 0x013E33E8u, kSiteChara4 = 0x013E3428u;
    constexpr uint32_t kSiteLayout4 = 0x013E37FCu;
    constexpr uint32_t kTuning = 0x01465650u; // the AI tuning block (a constant lui in the code)

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

    // A vector through a quaternion's rotation matrix (the code's five
    // identical VU0 blocks, e.g. 0x13e34b0-0x13e3524): vf8 = *src, vf11 =
    // *quat, ..., *dst = vf10.
    inline void rotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t src, uint32_t quat, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(src));
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(quat));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
    }
    // vf5 = *src; vf6 = the GPR's 128 bits with the float's bits in its low 64 (qmtc2.ni after mfc1); vmulx.xyzw; *dst = vf5.
    inline void scaled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t src, int gpr, float x, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(src));
        ctx->vu0_vf[6] = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[gpr], sx(bf(x))));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // vf5 = *a; vf6 = *b; vadd.xyz; *dst = vf5.
    inline void addXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    void animalMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kAnimalMove, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        const uint32_t s1 = GPR_U32(ctx, 4); // the animal's AI record
        float *f = ctx->f;
        WRITE64(sp + 0x78u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x98u, ra);
        WRITE32(sp + 0xA0u, bf(f[20]));
        auto finish = [&]()
        {
            f[20] = fb(READ32(sp + 0xA0u));
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };
        auto call = [&](uint32_t site) { callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };

        call(kSiteThread1);
        const uint64_t s4 = GPR_U64(ctx, 2); // this sub-thread's object
        if (!(static_cast<int32_t>(READ32(s1 + 0x26Cu)) < 18))
        {
            SET_GPR_U64(ctx, 2, 0u);
            return finish();
        }
        SET_GPR_U64(ctx, 2, 1u);

        // The motion speed: (+0x30c - 1) x *(+0x20) + 1, at least 0.1.
        SET_GPR_U64(ctx, 1, sx(0x3F800000u));
        f[2] = fb(0x3F800000u);
        f[0] = fb(READ32(s1 + 0x30Cu));
        const uint32_t params = READ32(s1 + 0x20u);
        SET_GPR_S32(ctx, 2, params);
        f[0] = FPU_SUB_S(f[0], f[2]);
        SET_GPR_U64(ctx, 1, sx(0x3DCCCCCCu));
        f[3] = fb(0x3DCCCCCCu);
        f[1] = fb(READ32(params));
        f[0] = FPU_MUL_S(f[0], f[1]);
        f[20] = FPU_ADD_SF(f[0], f[2]);
        setC(ctx, FPU_C_OLT_S(f[20], f[3]));
        if (cBit(ctx))
            f[20] = FPU_MOV_S(f[3]);
        SET_GPR_U64(ctx, 4, s4);
        f[12] = FPU_MOV_S(f[20]);
        callGuest(rdram, ctx, runtime, kSetSpeedRatio, 0x013E3150u, sp);

        // q = |+0x2e0| (vf7's xyz squared and summed into w, vsqrt).
        const uint32_t s3 = s1 + 0x2E0u;
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(s3));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->vu0_q, sizeof(bits));
            SET_GPR_S32(ctx, 2, bits);
            f[4] = fb(bits);
        }
        f[12] = fb(0u);
        const uint32_t a1 = READ32(s1 + 0x20u);
        SET_GPR_S32(ctx, 5, a1);
        SET_GPR_U32(ctx, 4, s1 + 0x308u);
        const uint32_t kind = READ32(s1 + 0x274u);
        SET_GPR_S32(ctx, 3, kind);
        f[13] = fb(READ32(a1 + 0xB8u));
        f[5] = fb(READ32(s1 + 0x2F4u));

        // The turn: f12 from +0x760 (limited to +-1.2566) unless the kind
        // (+0x274) is neither 0 nor 10, plus -+0x2f4's term (limited to
        // +-1.0995), the sum limited to +-1.0995.
        bool turn = kind == 0u;
        if (!turn)
        {
            SET_GPR_U64(ctx, 2, 10u);
            turn = kind == 10u;
        }
        if (turn)
        {
            f[0] = fb(READ32(s1 + 0x760u));
            f[2] = fb(READ32(a1 + 0x28u));
            f[0] = FPU_NEG_S(f[0]);
            SET_GPR_U64(ctx, 1, sx(0x3FA0D97Au));
            f[3] = fb(0x3FA0D97Au);
            f[1] = fb(0u);
            f[0] = FPU_MUL_S(f[0], f[4]);
            f[12] = ps2FpuDivide(ctx->fcr31, f[0], f[2]);
            setC(ctx, FPU_C_OLT_S(f[12], f[1]));
            bool limit;
            if (cBit(ctx))
            {
                f[0] = FPU_NEG_S(f[12]);
                setC(ctx, FPU_C_OLT_S(f[3], f[0]));
                limit = cBit(ctx);
            }
            else
            {
                setC(ctx, FPU_C_OLT_S(f[3], f[12]));
                limit = cBit(ctx);
            }
            if (limit)
            {
                f[0] = fb(0u);
                SET_GPR_U64(ctx, 1, sx(0x3FA0D97Au));
                f[1] = fb(0x3FA0D97Au);
                setC(ctx, FPU_C_OLT_S(f[12], f[0]));
                if (cBit(ctx))
                    f[1] = FPU_NEG_S(f[1]);
                f[12] = FPU_MOV_S(f[1]);
            }
        }
        f[0] = FPU_NEG_S(f[5]);
        SET_GPR_U64(ctx, 1, sx(0x40490FDAu));
        f[1] = fb(0x40490FDAu);
        f[2] = fb(READ32(a1 + 0x28u));
        SET_GPR_U64(ctx, 1, sx(0x3F8CBE4Bu));
        f[3] = fb(0x3F8CBE4Bu);
        f[0] = FPU_MUL_S(f[0], f[1]);
        f[0] = FPU_MUL_S(f[0], f[4]);
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[2]);
        setC(ctx, FPU_C_OLT_S(f[3], f[0]));
        if (cBit(ctx))
            f[0] = FPU_MOV_S(f[3]);
        else
        {
            SET_GPR_U64(ctx, 1, sx(0xBF8CBE4Bu));
            f[1] = fb(0xBF8CBE4Bu);
            setC(ctx, FPU_C_OLT_S(f[0], f[1]));
            if (cBit(ctx))
                f[0] = FPU_MOV_S(f[1]);
        }
        f[12] = FPU_ADD_S(f[12], f[0]);
        f[0] = fb(0u);
        setC(ctx, FPU_C_OLT_S(f[12], f[0]));
        bool clamp;
        if (cBit(ctx))
        {
            SET_GPR_U64(ctx, 1, sx(0xBF8CBE4Bu));
            f[0] = fb(0xBF8CBE4Bu);
            setC(ctx, FPU_C_OLT_S(f[12], f[0]));
            clamp = cBit(ctx);
        }
        else
        {
            SET_GPR_U64(ctx, 1, sx(0x3F8CBE4Bu));
            f[0] = fb(0x3F8CBE4Bu);
            setC(ctx, FPU_C_OLT_S(f[0], f[12]));
            clamp = cBit(ctx);
        }
        if (clamp)
        {
            f[0] = fb(0u);
            SET_GPR_U64(ctx, 1, sx(0xBF8CBE4Bu));
            f[1] = fb(0xBF8CBE4Bu);
            setC(ctx, FPU_C_OLT_S(f[12], f[0]));
            const bool negative = cBit(ctx);
            f[12] = FPU_MOV_S(f[1]);
            if (!negative)
            {
                SET_GPR_U64(ctx, 1, sx(0x3F8CBE4Bu));
                f[1] = fb(0x3F8CBE4Bu);
                f[12] = FPU_MOV_S(f[1]);
            }
        }
        // f13 x (1 -+ (+0x308 - f12)) for the velocity.
        f[0] = fb(READ32(s1 + 0x308u));
        f[1] = fb(0u);
        f[2] = FPU_SUB_S(f[0], f[12]);
        setC(ctx, FPU_C_OLT_S(f[2], f[1]));
        SET_GPR_U64(ctx, 1, sx(0x3F800000u));
        f[0] = fb(0x3F800000u);
        if (cBit(ctx))
            f[0] = FPU_SUB_S(f[0], f[2]);
        else
            f[0] = FPU_ADD_S(f[2], f[0]);
        f[13] = FPU_MUL_S(f[13], f[0]);
        callGuest(rdram, ctx, runtime, kUpdateVelocity, 0x013E333Cu, sp);

        // The heading towards +0x308 by its distance from 1.0995.
        SET_GPR_U64(ctx, 1, sx(0x3F8CBE4Bu));
        f[1] = fb(0x3F8CBE4Bu);
        f[14] = fb(READ32(s1 + 0x308u));
        f[0] = fb(0u);
        setC(ctx, FPU_C_OLT_S(f[14], f[0]));
        if (cBit(ctx))
            f[0] = FPU_ADD_S(f[14], f[1]);
        else
            f[0] = FPU_SUB_S(f[1], f[14]);
        f[1] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
        f[0] = fb(READ32(s1 + 0x8D0u));
        SET_GPR_U64(ctx, 3, 6u);
        const uint32_t type = READ32(s1);
        SET_GPR_S32(ctx, 2, type);
        f[0] = FPU_MUL_S(f[0], f[1]);
        f[0] = FPU_ADD_S(f[14], f[0]);
        if (static_cast<int32_t>(type) == 6)
        {
            f[14] = fb(READ32(s1 + 0x8D4u));
            SET_GPR_U64(ctx, 2, sx(0x01460000u));
            SET_GPR_U64(ctx, 1, sx(0x40400000u));
            f[2] = fb(0x40400000u);
            SET_GPR_U32(ctx, 4, s1);
            f[0] = FPU_SUB_S(f[0], f[14]);
            f[1] = fb(READ32(0x014657E0u));
            f[12] = fb(READ32(s1 + 0x2F4u));
            f[13] = fb(READ32(s1 + 0x2FCu));
            f[0] = FPU_MUL_SF(f[0], f[2]);
            f[0] = FPU_MUL_S(f[0], f[1]);
            f[14] = FPU_ADD_S(f[14], f[0]);
            WRITE32(s1 + 0x8D4u, bf(f[14]));
            callGuest(rdram, ctx, runtime, kSub013D8B38, 0x013E33BCu, sp);
        }
        else
        {
            f[12] = fb(READ32(s1 + 0x2F4u));
            f[14] = FPU_MOV_S(f[0]);
            f[13] = fb(READ32(s1 + 0x2FCu));
            SET_GPR_U32(ctx, 4, s1);
            callGuest(rdram, ctx, runtime, kSub013D8B38, 0x013E33DCu, sp);
        }

        SET_GPR_U64(ctx, 4, s4);
        call(kSiteLayout3);
        const uint32_t s2 = GPR_U32(ctx, 2); // the layout class
        call(kSiteThread2);
        // The speed's inverse (f20 at least 0.1).
        SET_GPR_U64(ctx, 1, sx(0x3DCCCCCCu));
        f[0] = fb(0x3DCCCCCCu);
        SET_GPR_U64(ctx, 1, sx(0x3F800000u));
        f[12] = fb(0x3F800000u);
        setC(ctx, FPU_C_OLT_S(f[20], f[0]));
        if (cBit(ctx))
            f[20] = FPU_MOV_S(f[0]);
        setC(ctx, FPU_C_OLE_S(f[12], f[20]));
        f[0] = FPU_SUB_S(f[20], f[12]);
        f[0] = FPU_ADD_SF(f[0], f[12]);
        f[20] = ps2FpuDivide(ctx->fcr31, f[12], f[0]);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        call(kSiteChara4);
        // sp+0 = the character's +0x9e0 x f20.
        scaled(rdram, ctx, runtime, GPR_U32(ctx, 2) + 0x9E0u, 3, f[20], sp);
        const uint32_t state = READ32(s1 + 0x26Cu);
        SET_GPR_S32(ctx, 3, state);
        SET_GPR_U64(ctx, 2, 20u);
        uint32_t sum, add; // the vectors summed at 0x13e37c4 (v1, a0)
        if (state - 14u < 2u)
        {
            const uint32_t vec = sotc_decomp::relocated(rdram, 0x013E3460u, 0x013E3468u);
            f[0] = fb(READ32(kTuning + 0x190u));
            SET_GPR_U32(ctx, 6, sp);
            f[2] = fb(READ32(s1 + 0x79Cu));
            SET_GPR_S32(ctx, 7, bf(f[0]));
            scaled(rdram, ctx, runtime, vec, 7, f[0], sp + 0x20u);
            f[1] = fb(READ32(kTuning + 0x190u));
            SET_GPR_S32(ctx, 5, bf(f[1]));
            scaled(rdram, ctx, runtime, vec, 5, f[1], sp + 0x10u);
            rotate(rdram, ctx, runtime, sp + 0x20u, READ32(s2 + 0x13Cu), sp + 0x20u);
            rotate(rdram, ctx, runtime, sp, READ32(s2 + 0x13Cu), sp + 0x30u);
            addXyz(rdram, ctx, runtime, sp + 0x20u, sp + 0x30u, sp + 0x20u);
            sum = sp + 0x20u;
            add = sp + 0x10u;
        }
        else if (state == 20u)
        {
            sum = add = 0u;
        }
        else
        {
            SET_GPR_S32(ctx, 5, READ32(s2 + 0x13Cu));
            SET_GPR_U32(ctx, 4, sp + 0x20u);
            callGuest(rdram, ctx, runtime, kYRotQuaternion, 0x013E35DCu, sp);
            rotate(rdram, ctx, runtime, s3, sp + 0x20u, sp + 0x10u);
            const uint32_t vec = sotc_decomp::relocated(rdram, 0x013E365Cu, 0x013E3664u);
            f[0] = fb(READ32(kTuning + 0x190u));
            SET_GPR_U32(ctx, 5, sp + 0x10u);
            SET_GPR_U32(ctx, 6, sp);
            f[2] = fb(READ32(s1 + 0x79Cu));
            SET_GPR_S32(ctx, 7, bf(f[0]));
            scaled(rdram, ctx, runtime, vec, 7, f[0], sp + 0x50u);
            f[1] = fb(READ32(kTuning + 0x190u));
            // mfc1 a0 (the register cache keeps it out of the context)
            scaled(rdram, ctx, runtime, sp + 0x10u, 4, f[1], sp + 0x40u);
            rotate(rdram, ctx, runtime, sp + 0x50u, READ32(s2 + 0x13Cu), sp + 0x50u);
            rotate(rdram, ctx, runtime, sp, READ32(s2 + 0x13Cu), sp + 0x60u);
            addXyz(rdram, ctx, runtime, sp + 0x50u, sp + 0x60u, sp + 0x50u);
            sum = sp + 0x50u;
            add = sp + 0x40u;
        }
        if (sum != 0u)
        {
            SET_GPR_U32(ctx, 3, sum);
            SET_GPR_U32(ctx, 4, add);
            addXyz(rdram, ctx, runtime, sum, add, sum);
            // The layout's position (+0x134) moved by it; its y no higher than f2 (+0x79c).
            const uint32_t at = READ32(s2 + 0x134u);
            addXyz(rdram, ctx, runtime, at, sum, at);
            const uint32_t pos = READ32(s2 + 0x134u);
            SET_GPR_S32(ctx, 2, pos);
            f[0] = fb(READ32(pos + 4u));
            setC(ctx, FPU_C_OLE_S(f[2], f[0]));
            if (cBit(ctx))
                WRITE32(pos + 4u, bf(f[2]));
        }
        SET_GPR_U64(ctx, 4, s4);
        call(kSiteLayout4);
        // The layout's position += +0x8e0.
        const uint32_t pos = READ32(GPR_U32(ctx, 2) + 0x134u);
        SET_GPR_U32(ctx, 4, s1 + 0x8E0u);
        SET_GPR_S32(ctx, 3, pos);
        addXyz(rdram, ctx, runtime, pos, s1 + 0x8E0u, pos);
        finish();
    }
}

SOTC_DECOMP_FUNCTION(0x013448C0u, gamecore_sub_013448C0, decomp_hot44c::groupFix);
SOTC_DECOMP_FUNCTION(0x0141F310u, gamecore_sub_0141F310, decomp_hot44c::camCondition);
SOTC_DECOMP_FUNCTION(0x013E30D0u, gamecore_sub_013E30D0, decomp_hot44c::animalMove);
