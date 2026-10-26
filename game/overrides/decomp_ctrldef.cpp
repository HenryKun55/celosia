// manager_sub_010C7B28 (round 11): one condition of a character's control
// definition (the player's and the AI's state machine, 010CA230 /
// 010C9B10 / CheckControlDef: ~380 calls a frame on the route's walk, 1.2%
// of the calibrated Vita3K game thread recompiled).
//
// a0 the object, a1 its character class (t0), a2 its control status, a3 the
// condition: +0 the type (a jump table at 0x11936c0 for 2..0x83), +4 / +0xc
// a range's low / high (f20 / f21: below -10000 / -180 / 0, depending on the
// type, the bound is not tested), +0x14 a packed link (the record s0 points
// at), +0x18 / +0x20 a mask and its value (s4 / s5), +0x28 the sense (0: the
// result inverted). v0 the result; 0 when the case set the "abort" word
// (sp + 0x44; none of the native cases do).
//
// The ~25 types that make 90% of the calls on the route (SOTC_CTRLDEF_STATS)
// are here, each as its code has it (the same loads, compares and guest
// calls; f0 as the case leaves it, fcr31's condition bit as its last
// compare); every other type, or a jump table not as expected, runs the
// recompiled function from its start (nothing written before).
//
// SOTC_CTRLDEF_STATS=1 (measuring): the condition types seen, every 20 000 calls.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_ctrldef
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kCond = 0x010C7B28u, kTable = 0x011936C0u, kSurfaceAttribute = 0x01132AF0u;

    struct R
    {
        const uint8_t *rdram;
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
        int8_t s8(uint32_t a) const { return static_cast<int8_t>(rdram[a & PS2_RAM_MASK]); }
        float f(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
    };

    // The case kinds.
    enum Kind : uint8_t
    {
        None,
        Range,      // optional pre-test word, then x against the bounds
        RangeSq,    // type 0x40: x against the bounds squared
        Bit,        // a bit of the 64-bit word at a2 + 0xf8 (dsll n; dsra32; andi 1)
        Word,       // a word (lw) or byte (lb) nonzero
        Mask,       // a doubleword (ld) masked by s4 against s5
        BitOfS0,    // types 0x38 / 0x39: bit *(s0 + 4) of a2 + 0x1c0 / 0x1c8
        EqS0,       // type 0x54: *(t0 + 0x1c0) == s0
        Target,     // type 0x49: the character class of *(t0 + 0x960), its +0x750 == s0
        Owner,      // type 0x45
        EqS4,       // type 0x57: *(a2 + 0x1a4) == s4
        SubAnim,    // type 0x55: SearchSubAnimation(t0, s0) >= 0
        Surface,    // types 0x58 / 0x27: a pre-test word, GetGeometryPointerSurfaceAttribute, then Mask
        Flag3a,     // type 0x3a
    };
    // Bounds: which "not tested" flag each side has and its compare.
    enum Flag : uint8_t
    {
        F10000, // below -10000 (t2 for low, t5 for high)
        F180,   // below -180 (s2 / s3)
        F0,     // below 0 (t3 / t7)
    };
    struct Case
    {
        uint8_t type;
        uint32_t target; // the jump table's entry
        Kind kind;
        // Range: base register (0: a2 + 0x80 = a0, 1: a2 + 0x100 = t1, 2: t0), pre-test offset (or -1), x offset,
        // flags, low strict (f20 < x) or not (f20 <= x), high strict (x < f21) or not (x <= f21),
        // the pre-test's result when its word is 0 (s1 then).
        uint8_t base = 0;
        int32_t pre = -1;
        uint32_t off = 0;
        Flag flag = F0;
        bool lowStrict = false, highStrict = false, preTrue = false;
        bool byte = false; // Word: lb
        uint32_t shift = 0; // Bit
        uint32_t frameAt = 0, jalSite = 0; // Surface: the result's frame slot, the jal
    };

    // From the disassembly (the address each case's code starts at, as the
    // table holds it).
    constexpr Case kCases[] = {
        {0x49, 0x010C7FD8u, Target},
        {0x58, 0x010C8060u, Surface, 1, 0xA8, 0x210, F0, false, false, false, false, 0, 0x20, 0x010C806Cu},
        {0x27, 0x010C8298u, Surface, 2, 0xFBC, 0xF70, F0, false, false, false, false, 0, 0x30, 0x010C82A4u},
        {0x38, 0x010C7CC0u, BitOfS0, 0, -1, 0x1C0},
        {0x39, 0x010C7CD0u, BitOfS0, 0, -1, 0x1C8},
        {0x40, 0x010C86D8u, RangeSq, 0, -1, 0x0, F0, true, false},
        {0x57, 0x010C8170u, EqS4},
        {0x45, 0x010C7F88u, Owner},
        {0x65, 0x010C9698u, Bit, 0, -1, 0, F0, false, false, false, false, 0x1D},
        {0x66, 0x010C96C8u, Bit, 0, -1, 0, F0, false, false, false, false, 0x1C},
        {0x6b, 0x010C96E0u, Bit, 0, -1, 0, F0, false, false, false, false, 0x1E},
        {0x55, 0x010C8668u, SubAnim},
        {0x04, 0x010C8278u, Mask, 2, -1, 0x690},
        {0x6d, 0x010C9418u, Range, 0, 0x38, 0x3C, F0, true, false},
        {0x68, 0x010C9360u, Range, 0, 0x24, 0x28, F0, true, false},
        {0x54, 0x010C8650u, EqS0},
        {0x41, 0x010C8800u, Range, 1, 0x0, 0x4, F180, false, false, true},
        {0x43, 0x010C8768u, Range, 0, -1, 0x4, F180, true, false},
        {0x6c, 0x010C9410u, Word, 0, -1, 0x34},
        {0x67, 0x010C9358u, Word, 0, -1, 0x20},
        {0x02, 0x010C88A0u, Range, 2, 0x124, 0x6B4, F180, false, false},
        {0x4d, 0x010C8950u, Range, 2, 0x124, 0x6BC, F180, false, false},
        {0x05, 0x010C91A8u, Range, 1, -1, 0x10, F10000, true, true},
        {0x06, 0x010C91F0u, Range, 1, -1, 0x14, F10000, true, true},
        {0x07, 0x010C9238u, Range, 1, -1, 0x18, F10000, true, true},
        {0x3a, 0x010C9108u, Flag3a},
    };

    struct Table
    {
        const Case *byType[0x84] = {};
        Table()
        {
            for (const Case &c : kCases)
                byType[c.type] = &c;
        }
    };
    const Table &table()
    {
        static const Table t;
        return t;
    }

    inline void setFloatBits(float &dst, uint32_t bits) { std::memcpy(&dst, &bits, 4); }

    void condStats(uint32_t type)
    {
        static const bool on = std::getenv("SOTC_CTRLDEF_STATS") != nullptr;
        if (!on)
            return;
        static uint64_t counts[128] = {}, calls = 0;
        ++counts[type < 127u ? type : 127u];
        if (++calls % 20000u == 0u)
        {
            std::fprintf(stderr, "[ctrldef] %llu calls:", (unsigned long long)calls);
            for (int i = 0; i < 128; ++i)
                if (counts[i])
                    std::fprintf(stderr, " %d:%llu", i, (unsigned long long)counts[i]);
            std::fprintf(stderr, "\n");
        }
    }

    void cond(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const R m{rdram};
        const uint32_t a0In = GPR_U32(ctx, 4), t0 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6), c = GPR_U32(ctx, 7);
        const uint32_t type = m.u32(c);
        condStats(type);
        const Case *k = type - 2u < 0x82u ? table().byType[type] : nullptr;
        if (!k || m.u32(kTable + (type - 2u) * 4u) != k->target)
        {
            ctx->pc = kCond;
            return sotc_decomp::recompiled(kCond)(rdram, ctx, runtime);
        }
        const uint32_t spIn = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31), sp = spIn - 0xB0u;
        uint32_t fcr31 = ctx->fcr31;
        auto cmp = [&](bool r)
        {
            fcr31 = r ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
            return r;
        };
        // The prologue's bounds and their flags (in its order of compares).
        const float f21 = m.f(c + 0xCu), f20 = m.f(c + 4u);
        const float minus10000 = [] { float f; setFloatBits(f, 0xC61C4000u); return f; }();
        const float minus180 = [] { float f; setFloatBits(f, 0xC3340000u); return f; }();
        const float zero = 0.0f;
        const bool t5 = cmp(FPU_C_OLT_S(f21, minus10000));
        const bool t2 = cmp(FPU_C_OLT_S(f20, minus10000));
        const bool s3 = cmp(FPU_C_OLT_S(f21, minus180));
        const bool s2 = cmp(FPU_C_OLT_S(f20, minus180));
        const bool t7 = cmp(FPU_C_OLT_S(f21, zero));
        const bool t3 = cmp(FPU_C_OLT_S(f20, zero));
        float f0 = zero;
        const uint64_t s4 = m.u64(c + 0x18u), s5 = m.u64(c + 0x20u);
        const uint32_t t1 = a2 + 0x100u, a0 = a2 + 0x80u, t6 = t0 + 0x200u, s7 = t0 + 0x1C0u;
        // s0: the record the packed link at +0x14 names (0 when none).
        auto recordS0 = [&]() -> uint32_t
        {
            const uint32_t at = c + 0x14u, t4 = m.u32(at), off = t4 & 0x3FFFFu;
            const uint32_t owner = off ? m.u32(at - off) : 0u;
            if (owner == 0u || owner == m.u32(0x001E0ED4u))
                return 0u;
            const int32_t index = static_cast<int32_t>((t4 >> 18) & 0xFFFu), size = m.s32(m.u32(owner + 8u) + 0xCu);
            return m.u32(owner + 0x10u) + static_cast<uint32_t>(index * size);
        };
        auto base = [&](uint8_t b) { return b == 0 ? a0 : (b == 1 ? t1 : t0); };
        auto maskTest = [&](uint64_t v) -> bool
        {
            v &= s4;
            return static_cast<int64_t>(s5) >= 0 ? v == s5 : v != 0u;
        };
        // A range: the low side tested unless its flag, then the high side.
        auto range = [&](float x, Flag flag, bool lowStrict, bool highStrict, float lo, float hi, bool loaded) -> bool
        {
            const bool lowOff = flag == F10000 ? t2 : (flag == F180 ? s2 : t3);
            const bool highOff = flag == F10000 ? t5 : (flag == F180 ? s3 : t7);
            bool low = true, high = true;
            if (!lowOff)
            {
                if (loaded)
                    f0 = x;
                low = cmp(lowStrict ? FPU_C_OLT_S(lo, x) : FPU_C_OLE_S(lo, x));
            }
            if (!highOff)
            {
                if (loaded)
                    f0 = x;
                high = cmp(highStrict ? FPU_C_OLT_S(x, hi) : FPU_C_OLE_S(x, hi));
            }
            return low && high;
        };
        bool s1 = false;
        bool f0FromCtx = false; // a guest call left f0 in the context
        switch (k->kind)
        {
        case Range:
        {
            const uint32_t b = base(k->base);
            if (k->pre >= 0 && m.u32(b + static_cast<uint32_t>(k->pre)) == 0u)
            {
                s1 = k->preTrue;
                break;
            }
            s1 = range(m.f(b + k->off), k->flag, k->lowStrict, k->highStrict, f20, f21, true);
            break;
        }
        case RangeSq:
        {
            const float f2 = FPU_MUL_S(f20, f20), f3 = FPU_MUL_S(f21, f21);
            s1 = range(m.f(a0 + k->off), F0, true, false, f2, f3, true);
            break;
        }
        case Bit:
            s1 = ((static_cast<int64_t>(m.u64(a0 + 0x78u) << k->shift) >> 32) & 1) != 0;
            break;
        case Word:
            s1 = k->byte ? m.s8(base(k->base) + k->off) != 0 : m.u32(base(k->base) + k->off) != 0u;
            break;
        case Mask:
            s1 = maskTest(m.u64(base(k->base) + k->off));
            break;
        case BitOfS0:
        {
            const uint32_t s0 = recordS0();
            const uint32_t bit = m.u32(s0 + 4u) & 63u;
            s1 = (m.u64(a2 + k->off) & (1ull << bit)) != 0u;
            break;
        }
        case EqS0:
            s1 = m.u32(s7) == recordS0();
            break;
        case Target:
        {
            const uint32_t target = m.u32(t0 + 0x960u);
            if (target == 0u)
                break;
            const uint8_t t = rdram[(target + 8u) & PS2_RAM_MASK];
            const uint32_t cc = (t == 9u || t == 0x16u) ? target + 0x90u : 0u; // GetCharacterClass
            s1 = m.u32(cc + 0x750u) == recordS0();
            break;
        }
        case Owner:
            s1 = a0In != m.u32(t0 + 0x960u) ? true : m.u32(a2 + 0x1ECu) != 0u;
            break;
        case EqS4:
            s1 = static_cast<int64_t>(m.s32(t1 + 0xA4u)) == static_cast<int64_t>(s4);
            break;
        case SubAnim:
        {
            // SearchSubAnimation (0x10d2a38, a leaf): the index of the entry
            // (0x80 bytes, at *(t0 + 0x30c)) whose +4 is s0, those with +0x44 =
            // -2 skipped; -1 when none.
            const uint32_t s0 = recordS0();
            const int32_t n = m.s32(t0 + 0x3FCu);
            uint32_t e = m.u32(t0 + 0x30Cu);
            for (int32_t i = 0; i < n; ++i, e += 0x80u)
                if (m.s32(e + 0x44u) != -2 && m.u32(e + 4u) == s0)
                {
                    s1 = true;
                    break;
                }
            break;
        }
        case Surface:
        {
            const uint32_t b = base(k->base);
            if (m.u32(b + static_cast<uint32_t>(k->pre)) == 0u)
                break;
            SET_GPR_U32(ctx, 4, sp + k->frameAt);
            SET_GPR_U32(ctx, 5, (k->base == 1 ? t6 + 0x10u : t0 + k->off));
            ctx->fcr31 = fcr31;
            ctx->f[0] = f0;
            sotc_decomp::callGuest(rdram, ctx, runtime, kSurfaceAttribute, k->jalSite + 8u, sp);
            fcr31 = ctx->fcr31;
            f0FromCtx = true;
            s1 = maskTest(m.u64(sp + k->frameAt));
            break;
        }
        case Flag3a:
            if (m.s8(t0 + 0x35Cu) == 0)
                break;
            if (static_cast<int64_t>(s4) < 0)
            {
                s1 = true;
                break;
            }
            s1 = maskTest(m.u64(t0 + 0x698u));
            break;
        case None:
            break;
        }
        // The epilogue: +0x28 = 0 inverts.
        const bool result = m.u32(c + 0x28u) == 0u ? !s1 : s1;
        if (!f0FromCtx)
            ctx->f[0] = f0;
        ctx->fcr31 = fcr31;
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U64(ctx, 2, result ? 1u : 0u);
        returnTo(ctx);
    }

    SOTC_DECOMP_FUNCTION(kCond, manager_sub_010C7B28, cond);
}
