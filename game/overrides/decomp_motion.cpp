// The motion resources' accessors (round 41): the character's animation
// handles and the per-node queries the pose and the IK make of them.
//
// A motion is reached by a handle (decomp_chara.cpp's lists: the low 18 bits
// back to the owning table, bits 18-29 the index). The table's type (+8)
// says what it is: a plain motion (the element's +0xc is its resource, whose
// +0x20 is the header: +0 the version, +0x18 the option nodes' count, +0x28
// their physical ids, one byte each) or a blend (getBlendLastMotion: the last
// of its 12-byte handle chain, +0x50). The two types and the empty table are
// boot-ELF data the module is linked to at run time: read from the code at
// each site (relocated lui / addiu or lw pairs), as the recompiled code does.
// A plain motion without its resource calls iosJumpRecoverPoint (a longjmp):
// that call goes to the recompiled code from the start (nothing is written
// before it).
//
// GetMotionNodeBasePos / GetMotionNodeBaseQuaternion: a node's position /
// rotation up its parent chain (the skeleton's +8 nodes, 0x40 each: +0x10
// the offset, +0x38 the parent), the recompiled code's VU0 statements on
// locals (the same PS2_V* macros in the same order); the VU0 registers they
// write go back to the context as the original leaves them.
//
// SOTC_MOTION=0 puts every function of this file back to the recompiled
// code; one alone with SOTC_DECOMP_SKIP=<name>.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdlib>
#include <cstring>

namespace decomp_motion
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    using sotc_decomp::motionOn;

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
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }

        // The table a handle at h belongs to, 0 for none or the empty one
        // (whose pointer is at `empty`).
        uint32_t table(uint32_t h, uint32_t empty) const
        {
            const uint32_t off = u32(h) & 0x3FFFFu;
            const uint32_t t = off ? u32(h - off) : 0u;
            return t != 0u && t != u32(empty) ? t : 0u;
        }
        uint32_t resolve(uint32_t h, uint32_t empty) const
        {
            const uint32_t v = u32(h), t = table(h, empty);
            if (t == 0u)
                return 0u;
            return u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * u32(u32(t + 8u) + 0xCu);
        }
    };

    // getBlendLastMotion (0x10b28e0): a blend's last motion. From the
    // blend's object, the handles 12 bytes apart (+0xc, +0x18, ...) up to the
    // last that resolves; that one's object + 0x50.
    uint32_t blendLast(const Ram r, uint32_t h)
    {
        const uint8_t *m = r.rdram;
        uint32_t a3 = r.resolve(h, relocated(m, 0x010B2908u, 0x010B290Cu));
        const uint32_t empty2 = relocated(m, 0x010B2940u, 0x010B2948u);
        while (r.resolve(a3 + 0xCu, empty2) != 0u)
            a3 += 0xCu;
        return r.resolve(a3, relocated(m, 0x010B29ECu, 0x010B29F0u)) + 0x50u;
    }
    void getBlendLastMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B28E0u)(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(blendLast(Ram{rdram}, GPR_U32(ctx, 4))));
        returnTo(ctx);
    }

    // The plain motion a handle leads to, through blends: its element (the
    // table's object), or 0 when the chain ends in neither type (the callers
    // return -1 then). `missing`: a plain motion without its resource (the
    // original's iosJumpRecoverPoint).
    struct Sites
    {
        uint32_t emptyA[2], plain[2], emptyB[2], emptyC[2], blend[2];
    };
    uint32_t plainMotion(const Ram r, uint32_t h, const Sites &s, bool &missing)
    {
        const uint8_t *m = r.rdram;
        missing = false;
        for (;;)
        {
            if (r.u32(r.table(h, relocated(m, s.emptyA[0], s.emptyA[1])) + 8u) == relocated(m, s.plain[0], s.plain[1]))
            {
                const uint32_t e = r.resolve(h, relocated(m, s.emptyB[0], s.emptyB[1]));
                missing = r.u32(e + 0xCu) == 0u;
                return e;
            }
            if (r.u32(r.table(h, relocated(m, s.emptyC[0], s.emptyC[1])) + 8u) != relocated(m, s.blend[0], s.blend[1]))
                return 0u;
            h = blendLast(r, h);
        }
    }

    // GetMotionOptionNodePhysicalId (0x10b3810): the option node whose
    // physical id is a1 (the whole register compared with each byte), -1 if none.
    constexpr Sites kPhysicalId{{0x010B3850u, 0x010B3854u}, {0x010B381Cu, 0x010B3868u}, {0x010B3898u, 0x010B389Cu},
                                {0x010B3960u, 0x010B3964u}, {0x010B3974u, 0x010B397Cu}};
    void getMotionOptionNodePhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B3810u)(rdram, ctx, runtime);
        const Ram r{rdram};
        bool missing;
        const uint32_t e = plainMotion(r, GPR_U32(ctx, 4), kPhysicalId, missing);
        if (missing)
            return sotc_decomp::recompiled(0x010B3810u)(rdram, ctx, runtime);
        int32_t v0 = -1;
        if (e != 0u)
        {
            const uint32_t header = r.u32(r.u32(e + 0xCu) + 0x20u);
            const int32_t n = r.s32(header + 0x18u);
            const uint32_t ids = r.u32(header + 0x28u);
            const uint64_t id = GPR_U64(ctx, 5);
            for (int32_t i = 0; i < n; ++i)
                if (r.u8(ids + static_cast<uint32_t>(i)) == id)
                {
                    v0 = i;
                    break;
                }
        }
        SET_GPR_S32(ctx, 2, v0);
        returnTo(ctx);
    }

    // GetMotionVersion (0x10b39b0): the header's first halfword (signed), -1 if none.
    constexpr Sites kVersion{{0x010B39E8u, 0x010B39ECu}, {0x010B39BCu, 0x010B3A00u}, {0x010B3A30u, 0x010B3A34u},
                             {0x010B3AB0u, 0x010B3AB4u}, {0x010B3AC4u, 0x010B3ACCu}};
    void getMotionVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B39B0u)(rdram, ctx, runtime);
        const Ram r{rdram};
        bool missing;
        const uint32_t e = plainMotion(r, GPR_U32(ctx, 4), kVersion, missing);
        if (missing)
            return sotc_decomp::recompiled(0x010B39B0u)(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 2, e != 0u ? r.s16(r.u32(r.u32(e + 0xCu) + 0x20u)) : -1);
        returnTo(ctx);
    }

    // The accessors as values, for the natives below (each with its own
    // sites; `missing` as plainMotion's).
    int32_t motionVersion(const Ram r, uint32_t h, bool &missing)
    {
        const uint32_t e = plainMotion(r, h, kVersion, missing);
        return e != 0u ? r.s16(r.u32(r.u32(e + 0xCu) + 0x20u)) : -1;
    }
    int32_t physicalId(const Ram r, uint32_t h, uint64_t id, bool &missing)
    {
        const uint32_t e = plainMotion(r, h, kPhysicalId, missing);
        if (e == 0u || missing)
            return -1;
        const uint32_t header = r.u32(r.u32(e + 0xCu) + 0x20u);
        const int32_t n = r.s32(header + 0x18u);
        const uint32_t ids = r.u32(header + 0x28u);
        for (int32_t i = 0; i < n; ++i)
            if (r.u8(ids + static_cast<uint32_t>(i)) == id)
                return i;
        return -1;
    }

    // GetMotionNbOptionNodes (0x10b36c8): the header's +0x18, -1 if none.
    constexpr Sites kNbOptionNodes{{0x010B3700u, 0x010B3704u}, {0x010B36D4u, 0x010B3718u}, {0x010B3748u, 0x010B374Cu},
                                   {0x010B37C8u, 0x010B37CCu}, {0x010B37DCu, 0x010B37E4u}};
    int32_t nbOptionNodes(const Ram r, uint32_t h, bool &missing)
    {
        const uint32_t e = plainMotion(r, h, kNbOptionNodes, missing);
        return e != 0u && !missing ? r.s32(r.u32(r.u32(e + 0xCu) + 0x20u) + 0x18u) : -1;
    }
    void getMotionNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B36C8u)(rdram, ctx, runtime);
        bool missing;
        const int32_t n = nbOptionNodes(Ram{rdram}, GPR_U32(ctx, 4), missing);
        if (missing)
            return sotc_decomp::recompiled(0x010B36C8u)(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 2, n);
        returnTo(ctx);
    }

    // GetOptionNodePhysicalNodeId (0x10c6710): a0's count (+0) above 0 and
    // the motion a1's version 5 or more: GetMotionOptionNodePhysicalId(a1,
    // a2); else a2 when it is below GetMotionNbOptionNodes(a1) (the whole
    // registers compared), -1 if not.
    bool optionNodeId(const Ram r, uint32_t list, uint32_t motion, uint64_t index, int64_t &v0)
    {
        bool missing = false;
        if (r.s32(list) > 0)
        {
            const int32_t version = motionVersion(r, motion, missing);
            if (missing)
                return false;
            if (version >= 5)
            {
                v0 = physicalId(r, motion, index, missing);
                return !missing;
            }
        }
        const int32_t n = nbOptionNodes(r, motion, missing);
        if (missing)
            return false;
        v0 = static_cast<int64_t>(index) < n ? static_cast<int64_t>(index) : -1;
        return true;
    }
    void getOptionNodePhysicalNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010C6710u)(rdram, ctx, runtime);
        int64_t v0 = -1;
        if (!optionNodeId(Ram{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U64(ctx, 6), v0))
            return sotc_decomp::recompiled(0x010C6710u)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(v0));
        returnTo(ctx);
    }

    constexpr uint32_t kSubGeometry = 0x010C3650u;
    // AddCharacterClassOptionNodeSubGeometries (0x10c6798): for each of the
    // character a2's 0x80-byte slots (+0x30c, count +0x3fc) that is not
    // empty (+0x44 != -2), AddCharacterClassOptionNodeSubGeometry(a0, a1,
    // a2, slot + 4, a3) (guest). v0: the loop's last compare (0), untouched
    // with no slots.
    void subGeometries(const Ram r, R5900Context *ctx, PS2Runtime *runtime, uint32_t a0, uint32_t a1, uint32_t chara, uint64_t a3, uint32_t sp0)
    {
        const uint32_t sp = sp0 - 0x40u;
        int32_t n = r.s32(chara + 0x3FCu);
        if (n <= 0)
            return;
        uint32_t slot = r.u32(chara + 0x30Cu);
        for (int32_t i = 0; i < n; ++i, slot += 0x80u)
        {
            if (r.u32(slot + 0x44u) == 0xFFFFFFFEu)
                continue;
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            SET_GPR_U32(ctx, 6, chara);
            SET_GPR_U32(ctx, 7, slot + 4u);
            SET_GPR_U64(ctx, 8, a3);
            sotc_decomp::callGuest(r.rdram, ctx, runtime, kSubGeometry, 0x010C6800u, sp);
            n = r.s32(chara + 0x3FCu);
        }
        SET_GPR_U64(ctx, 2, 0u);
    }
    void addSubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn() || sotc_decomp::jalTarget(rdram, 0x010C67F8u) != kSubGeometry)
            return sotc_decomp::recompiled(0x010C6798u)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        subGeometries(Ram{rdram}, ctx, runtime, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U64(ctx, 7), sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // manager_sub_010BC4F0 (ExecCharaGeometry's option nodes): for each
    // option node i of the character a0's list (+0x358: +0x20 the count,
    // +0x24 the nodes, 0x40 each), its quaternion (+0x2ec) and position
    // (+0x2f0) from the motion at +0x1c0 (+0x50) when the node is in it
    // (GetOptionNodePhysicalNodeId, GetMotionOptionNodeFloatFrame at frame
    // +0x1c8, guest), else the node's own (+0x20 / +0x10) and the constant
    // qword at 0x1e0fe0 (relocated) on the frame; the sub geometries when
    // +0x52e is set (AddCharacterClassOptionNodeSubGeometries natively, the
    // +0xd20 one when +0xd90 is 2 and +0xd20 is set or +0x734 < 1.0); the
    // position scaled by +0x90 (xyz); the node's three flags (+0x2fc, 3
    // bytes) cleared, then from GetMotionOptionNodeBooleanInfoFloatFrame
    // (guest) when the list's +0 is above 0, the motion's version is 5 or
    // more and the node is in it. GetMotionVersion / ..PhysicalId /
    // GetOptionNodePhysicalNodeId natively (a motion without its resource:
    // the recompiled code from the start if nothing was written yet, else
    // the guest functions for that node).
    constexpr uint32_t kOptionNodes = 0x010BC4F0u;
    bool optionNodesSites(const uint8_t *rdram)
    {
        static constexpr uint32_t kSites[7][2] = {{0x010BC590u, 0x010C6710u}, {0x010BC5B0u, 0x010B6650u}, {0x010BC5F4u, 0x010C6798u},
                                                  {0x010BC640u, 0x010C3650u}, {0x010BC68Cu, 0x010B39B0u}, {0x010BC6A4u, 0x010B3810u},
                                                  {0x010BC6C0u, 0x010B3508u}};
        for (const auto &s : kSites)
            if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                return false;
        return sotc_decomp::jalTarget(rdram, 0x010C67F8u) == kSubGeometry;
    }
    void optionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn() || !optionNodesSites(rdram))
            return sotc_decomp::recompiled(kOptionNodes)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t chara = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x70u;
        uint32_t list = r.u32(chara + 0x358u);
        int32_t count = 0;
        if (list != 0u)
        {
            count = r.s32(list + 0x20u);
            SET_GPR_S32(ctx, 2, count);
        }
        uint32_t quat = r.u32(chara + 0x2ECu), pos = r.u32(chara + 0x2F0u), flags = r.u32(chara + 0x2FCu);
        auto guest = [&](uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp); };
        for (int32_t i = 0; i < count; ++i, pos += 0x10u, quat += 0x10u, flags += 3u)
        {
            if (i > 0)
                list = r.u32(chara + 0x358u);
            const uint32_t node = r.u32(list + 0x24u) + (static_cast<uint32_t>(i) << 6);
            const uint32_t motion = r.u32(chara + 0x1C0u);
            int64_t id = -1;
            if (motion != 0u && motion + 0x50u != 0u)
            {
                if (!optionNodeId(r, list, motion + 0x50u, static_cast<uint64_t>(i), id))
                {
                    if (i == 0)
                        return sotc_decomp::recompiled(kOptionNodes)(rdram, ctx, runtime);
                    SET_GPR_U32(ctx, 4, list);
                    SET_GPR_U32(ctx, 5, motion + 0x50u);
                    SET_GPR_S32(ctx, 6, i);
                    guest(0x010BC590u);
                    id = static_cast<int64_t>(GPR_U64(ctx, 2));
                }
            }
            if (id >= 0)
            {
                SET_GPR_U32(ctx, 4, quat);
                SET_GPR_U32(ctx, 5, pos);
                SET_GPR_U32(ctx, 6, sp);
                SET_GPR_U32(ctx, 7, motion + 0x50u);
                SET_GPR_U64(ctx, 8, static_cast<uint64_t>(id));
                std::memcpy(&ctx->f[12], rdram + ((chara + 0x1C8u) & PS2_RAM_MASK), 4);
                guest(0x010BC5B0u); // (the original's a2 is fp = sp; fp itself is not read by the callee)
            }
            else
            {
                r.putq(quat, r.q(node + 0x20u));
                r.putq(pos, r.q(node + 0x10u));
                r.putq(sp, r.q(relocated(rdram, 0x010BC5C4u, 0x010BC5C8u)));
            }
            if (static_cast<int8_t>(r.u8(chara + 0x52Eu)) != 0)
            {
                subGeometries(r, ctx, runtime, quat, pos, chara, static_cast<uint64_t>(i), sp);
                if (r.u8(chara + 0xD90u) == 2u)
                {
                    bool call = r.u32(chara + 0xD20u) != 0u;
                    if (!call)
                    {
                        float f1;
                        std::memcpy(&f1, rdram + ((chara + 0x734u) & PS2_RAM_MASK), 4);
                        ctx->f[0] = 1.0f;
                        call = f1 < 1.0f;
                    }
                    if (call)
                    {
                        SET_GPR_U32(ctx, 4, quat);
                        SET_GPR_U32(ctx, 5, pos);
                        SET_GPR_U32(ctx, 6, chara);
                        SET_GPR_U32(ctx, 7, chara + 0xD20u);
                        SET_GPR_S32(ctx, 8, i);
                        guest(0x010BC640u);
                    }
                }
            }
            __m128 v5 = r.q(pos);
            const __m128 v6 = r.q(chara + 0x90u);
            { __m128 res = PS2_VMUL(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
            r.putq(pos, v5);
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            const uint32_t m1 = r.u32(chara + 0x1C0u);
            const uint32_t m = m1 != 0u ? m1 + 0x50u : 0u;
            const uint32_t list2 = r.u32(chara + 0x358u);
            r.rdram[(flags + 2u) & PS2_RAM_MASK] = 0u;
            r.rdram[(flags + 1u) & PS2_RAM_MASK] = 0u;
            r.rdram[flags & PS2_RAM_MASK] = 0u;
            if (m != 0u && r.s32(list2) > 0)
            {
                bool missing = false;
                const int32_t version = motionVersion(r, m, missing);
                int32_t pid = -1;
                if (!missing && version >= 5)
                    pid = physicalId(r, m, static_cast<uint64_t>(i), missing);
                if (missing)
                {
                    // the guest functions for this node, as the original calls them
                    SET_GPR_U32(ctx, 4, m);
                    guest(0x010BC68Cu);
                    pid = -1;
                    if (static_cast<int32_t>(GPR_U32(ctx, 2)) >= 5)
                    {
                        SET_GPR_U32(ctx, 4, m);
                        SET_GPR_S32(ctx, 5, i);
                        guest(0x010BC6A4u);
                        pid = static_cast<int32_t>(GPR_U32(ctx, 2));
                    }
                }
                if (version >= 5 && pid >= 0)
                {
                    SET_GPR_U32(ctx, 4, flags);
                    SET_GPR_U32(ctx, 5, m);
                    SET_GPR_S32(ctx, 6, pid);
                    std::memcpy(&ctx->f[12], rdram + ((chara + 0x1C8u) & PS2_RAM_MASK), 4);
                    std::memcpy(&ctx->f[13], rdram + ((chara + 0x1CCu) & PS2_RAM_MASK), 4);
                    guest(0x010BC6C0u);
                }
            }
            SET_GPR_U64(ctx, 2, i + 1 < count ? 1u : 0u);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // GetMotionNodeBasePos (0x10b6e80): a0 = the constant qword at 0x1e0fd0
    // (relocated), then for node a3 up to the root: a0 rotated by node's
    // quaternion (a2 + 16 node: its matrix rows, then vmulax .. vmaddw with
    // vf0), plus the node's offset (xyz).
    void getMotionNodeBasePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B6E80u)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t out = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5), quats = GPR_U32(ctx, 6);
        const uint32_t base = relocated(rdram, 0x010B6E80u, 0x010B6E88u);
        const __m128i first = _mm_castps_si128(r.q(base));
        r.putq(out, _mm_castsi128_ps(first));
        SET_GPR_VEC(ctx, 3, first);
        const __m128 v0 = ctx->vu0_vf[0];
        __m128 v5 = ctx->vu0_vf[5], v6 = ctx->vu0_vf[6], v8 = ctx->vu0_vf[8], v10 = ctx->vu0_vf[10], v11 = ctx->vu0_vf[11],
               v12 = ctx->vu0_vf[12], v13 = ctx->vu0_vf[13], v14 = ctx->vu0_vf[14], v15 = ctx->vu0_vf[15], v16 = ctx->vu0_vf[16],
               v17 = ctx->vu0_vf[17], acc = ctx->vu0_acc;
        bool any = false;
        uint32_t node = GPR_U32(ctx, 7);
        if (GPR_U64(ctx, 7) != ~0ull)
        {
            any = true;
            do
            {
                v8 = r.q(out);
                v11 = r.q(quats + (node << 4));
                { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = PS2_VMUL(v11, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v13, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v17 = _mm_blendv_ps(v17, v14, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v14 = _mm_blendv_ps(v14, v16, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v16 = _mm_blendv_ps(v16, v15, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v15 = _mm_blendv_ps(v15, v17, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v14 = _mm_blendv_ps(v14, v15, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v15 = _mm_blendv_ps(v15, v16, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v16 = _mm_blendv_ps(v16, v17, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
                r.putq(out, v10);
                const uint32_t n = r.u32(skel + 8u) + (node << 6);
                v5 = r.q(out);
                v6 = r.q(n + 0x10u);
                { __m128 res = PS2_VADD(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
                r.putq(out, v5);
                node = r.u32(n + 0x38u);
            } while (node != 0xFFFFFFFFu);
        }
        // v0: the base's address when the chain is empty (the branch's delay slot is the store).
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(any ? node << 4 : base));
        if (any)
        {
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.u32(skel + 8u)));
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            ctx->vu0_vf[8] = v8;
            ctx->vu0_vf[10] = v10;
            ctx->vu0_vf[11] = v11;
            ctx->vu0_vf[12] = v12;
            ctx->vu0_vf[13] = v13;
            ctx->vu0_vf[14] = v14;
            ctx->vu0_vf[15] = v15;
            ctx->vu0_vf[16] = v16;
            ctx->vu0_vf[17] = v17;
            ctx->vu0_acc = acc;
        }
        returnTo(ctx);
    }

    // GetMotionNodeBaseQuaternion (0x10b6f60): a0 = vf0, then for node a3
    // up to the root: a0 = node's quaternion (a2 + 16 node) x a0.
    void getMotionNodeBaseQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!motionOn())
            return sotc_decomp::recompiled(0x010B6F60u)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t out = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5), quats = GPR_U32(ctx, 6);
        r.putq(out, ctx->vu0_vf[0]);
        __m128 v11 = ctx->vu0_vf[11], v12 = ctx->vu0_vf[12], v13 = ctx->vu0_vf[13], v14 = ctx->vu0_vf[14], v15 = ctx->vu0_vf[15],
               v16 = ctx->vu0_vf[16], acc = ctx->vu0_acc;
        bool any = false;
        uint32_t node = GPR_U32(ctx, 7);
        if (GPR_U64(ctx, 7) != ~0ull)
        {
            any = true;
            do
            {
                v11 = r.q(quats + (node << 4));
                v12 = r.q(out);
                { __m128 res = PS2_VMUL(v11, v12); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v12, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v11, _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v14, v15); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v13, v16); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                WRITE128(ADD32(GPR_U32(ctx, 4), 0), _mm_castps_si128(v13));
                r.putq(out, v13);
                node = r.u32(r.u32(skel + 8u) + (node << 6) + 0x38u);
            } while (node != 0xFFFFFFFFu);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(node << 4));
        if (any)
        {
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.u32(skel + 8u)));
            ctx->vu0_vf[11] = v11;
            ctx->vu0_vf[12] = v12;
            ctx->vu0_vf[13] = v13;
            ctx->vu0_vf[14] = v14;
            ctx->vu0_vf[15] = v15;
            ctx->vu0_vf[16] = v16;
            ctx->vu0_acc = acc;
        }
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010B28E0u, getBlendLastMotion, decomp_motion::getBlendLastMotion);
SOTC_DECOMP_FUNCTION(0x010B3810u, GetMotionOptionNodePhysicalId, decomp_motion::getMotionOptionNodePhysicalId);
SOTC_DECOMP_FUNCTION(0x010B39B0u, GetMotionVersion, decomp_motion::getMotionVersion);
SOTC_DECOMP_FUNCTION(0x010B6E80u, GetMotionNodeBasePos, decomp_motion::getMotionNodeBasePos);
SOTC_DECOMP_FUNCTION(0x010B6F60u, GetMotionNodeBaseQuaternion, decomp_motion::getMotionNodeBaseQuaternion);
SOTC_DECOMP_FUNCTION(0x010B36C8u, GetMotionNbOptionNodes, decomp_motion::getMotionNbOptionNodes);
SOTC_DECOMP_FUNCTION(0x010C6710u, GetOptionNodePhysicalNodeId, decomp_motion::getOptionNodePhysicalNodeId);
SOTC_DECOMP_FUNCTION(0x010C6798u, AddCharacterClassOptionNodeSubGeometries, decomp_motion::addSubGeometries);
SOTC_DECOMP_FUNCTION(0x010BC4F0u, manager_sub_010BC4F0, decomp_motion::optionNodes);
