// CalcNodeQuaternionWithIK (0x10bba60, round 41): a character's IK step,
// called once a character a frame by ExecCharaGeometry. a0 the object, a1
// its motion slot (+0x30 the IK set's handle), a2 / a3 the IK's arguments,
// f12 the frame time. The character class c is a0 + 0x90 for object types
// 9 and 0x16, else 0.
//
// With an IK set (the handle's table +0x10, the empty table excepted) and
// c + 0x52c set, the IK runs: GetMotionMatrix into c's pose, the IK nodes'
// callbacks (jalr per node, GetSkeletonFocusNode, an alloca'd mark table)
// and the blend back of the unmarked nodes. That path goes to the
// recompiled function from its start (nothing is written before the test).
//
// Without it (the common case), natively:
//   - every IK node record (c +0x31c, 0xd0 bytes each, *(c + 0x358) + 0x20
//     of them): +0xc4 / +0xc5 = 0, +0xb0 / +0xb4 / +0xc0 = 0, +0xb8 = 1.0,
//     +0xbc = 100.0 (f1 / f0 as the original leaves them);
//   - each record's +0x40 = c's +0x460 qword, +0xa0 = the unit quaternion
//     at 0x1e0f40 (relocated), c re-read from the object's type;
//   - the per-node weights (c +0x320 / +0x324, *(c + 0x358) + 4 of them) = 0;
// then manager_sub_010C32E8(a0) as a guest call (v0 and f0 as it leaves
// them). The loads are re-done per iteration as the original does them.
// A negative record count (the original's loop would run ~2^32 times):
// the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ikquat
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kSelf = 0x010BBA60u, kPost = 0x010C32E8u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
        void copyq(uint32_t dst, uint32_t src) const
        {
            uint8_t t[16];
            std::memcpy(t, rdram + (src & PS2_RAM_MASK & ~0xFu), 16);
            std::memcpy(rdram + (dst & PS2_RAM_MASK & ~0xFu), t, 16);
        }
        uint32_t chara(uint32_t obj) const
        {
            const uint8_t t = u8(obj + 8u);
            return t == 9u || t == 0x16u ? obj + 0x90u : 0u;
        }
    };

    void calcNodeQuaternionWithIK(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || sotc_decomp::jalTarget(rdram, 0x010BC190u) != kPost)
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t obj = GPR_U32(ctx, 4), slot = GPR_U32(ctx, 5) + 0x30u;
        const uint32_t c = r.chara(obj);
        // The IK set: the handle's table +0x10 (none for no table / the empty one).
        uint32_t ik = 0u;
        {
            const uint32_t off = r.u32(slot) & 0x3FFFFu;
            const uint32_t t = off ? r.u32(slot - off) : 0u;
            if (t != 0u && t != r.u32(relocated(rdram, 0x010BBAF8u, 0x010BBAFCu)))
                ik = r.u32(t + 0x10u);
        }
        if (ik != 0u && r.u8(c + 0x52Cu) != 0u)
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        uint32_t skel = r.u32(c + 0x358u);
        if (skel != 0u && r.s32(skel + 0x20u) < 0)
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x180u;
        const float f12 = ctx->f[12];
        // 0x10bc084: the records' flags
        if (skel != 0u)
        {
            int32_t n = r.s32(skel + 0x20u);
            if (n > 0)
            {
                ctx->f[1] = 1.0f;
                ctx->f[0] = 100.0f;
                uint32_t rec = r.u32(c + 0x31Cu);
                for (; n > 0; --n, rec += 0xD0u)
                {
                    r.put8(rec + 0xC4u, 0u);
                    r.put32(rec + 0xB0u, 0u);
                    r.put32(rec + 0xB8u, 0x3F800000u);
                    r.put32(rec + 0xB4u, 0u);
                    r.put8(rec + 0xC5u, 0u);
                    r.put32(rec + 0xBCu, 0x42C80000u);
                    r.put32(rec + 0xC0u, 0u);
                }
            }
        }
        // 0x10bc0e4: the records' rest pose and quaternion (c re-read)
        {
            const uint32_t c2 = r.chara(obj);
            int32_t n = r.s32(r.u32(c2 + 0x358u) + 0x20u);
            if (n > 0)
            {
                const uint32_t unit = relocated(rdram, 0x010BC114u, 0x010BC11Cu);
                for (uint32_t off = 0u; n != 0; --n, off += 0xD0u)
                {
                    const uint32_t rec = r.u32(c2 + 0x31Cu) + off;
                    r.copyq(rec + 0x40u, c2 + 0x460u);
                    r.copyq(rec + 0xA0u, unit);
                }
            }
        }
        // 0x10bc14c: the weights
        skel = r.u32(c + 0x358u);
        if (skel != 0u)
        {
            const int32_t n = r.s32(skel + 4u);
            for (int32_t i = 0; i < n; ++i)
            {
                r.put32(r.u32(c + 0x320u) + 4u * static_cast<uint32_t>(i), 0u);
                r.put32(r.u32(c + 0x324u) + 4u * static_cast<uint32_t>(i), 0u);
            }
        }
        // 0x10bc190: manager_sub_010C32E8(a0), f20 = f12 as the original has it there
        const float f20 = ctx->f[20];
        ctx->f[20] = f12;
        SET_GPR_U32(ctx, 4, obj);
        sotc_decomp::callGuest(rdram, ctx, runtime, kPost, 0x010BC198u, sp);
        ctx->f[20] = f20;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010BBA60u, CalcNodeQuaternionWithIK, decomp_ikquat::calcNodeQuaternionWithIK);
