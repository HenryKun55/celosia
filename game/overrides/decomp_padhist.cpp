// FlushPadHistory (0x114b0a8, round 41): the two pads' input history folded
// into their per-frame state, under the pad semaphore (*0x121b04c:
// WaitSema / SignalSema, both relocated jal / j sites read from the code).
//
// Two pad blocks of 0xf98 bytes at 0x118d680; the ring of 30 samples
// (0x78 bytes each, from block + 8) holds the samples the pad thread wrote
// since the last flush: *0x118d63c of them, the newest before *0x118d638.
// Per block: +8 / +0xc / +0x10 cleared (both blocks first), +0 cleared and
// the 32 hold-time words at +0xf08..+0xf84 cleared; for each new sample:
// the buttons (+0x70) into +4 (current), +8 (pressed: newly set), +0xc
// (released), +0 (any held), +0x10 |= the sample's +0x80; its four analog
// bytes (+0xc0) to +0x54; each button's hold counter (+0xe88) counted up
// while held, else its last run kept as the longer of it and +0xf08 and the
// counter cleared. Then for each analog byte the number of ring samples
// back (from the head) within 0x60 of it, 30 at most, into +0xf88. The
// count is cleared at the end.
//
// All plain loads and stores in the original's order (the 4-byte copy's
// source is past the destination: no overlap). The block addresses are
// absolute in the generated code (no relocation). v0 as SignalSema leaves
// it; no float register is written.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_padhist
{
    using sotc_decomp::returnTo;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
    };

    constexpr uint32_t kFlush = 0x0114B0A8u, kBlocks = 0x0118D680u, kCount = 0x0118D63Cu, kHead = 0x0118D638u, kSema = 0x0121B04Cu;
    constexpr uint32_t kWaitSite = 0x0114B0DCu, kSignalSite = 0x0114B3C4u;

    void flush(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kFlush)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x50u;
        // WaitSema(*0x121b04c)
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.u32(kSema)));
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, kWaitSite), kWaitSite + 8u, sp);

        for (uint32_t b = kBlocks; b < kBlocks + 0x1F30u; b += 0xF98u)
        {
            r.put32(b + 8u, 0u);
            r.put32(b + 0xCu, 0u);
            r.put32(b + 0x10u, 0u);
        }
        if (r.u32(kCount) != 0u)
        {
            for (uint32_t pad = 0; pad < 2u; ++pad)
            {
                const uint32_t t0 = kBlocks + pad * 0xF98u, t7 = t0 + 8u;
                int32_t t6 = static_cast<int32_t>(r.u32(kHead) - r.u32(kCount));
                if (t6 < 0)
                    t6 += 0x1E;
                r.put32(t0, 0u);
                for (uint32_t a = t0 + 0xF84u; a >= t0 + 0xF08u; a -= 4u)
                    r.put32(a, 0u);
                for (int32_t s0 = 0; s0 < static_cast<int32_t>(r.u32(kCount)); ++s0)
                {
                    int32_t idx = s0 + t6;
                    if (!(idx < 0x1E))
                        idx -= 0x1E;
                    const uint32_t sample = t7 + static_cast<uint32_t>(idx) * 0x78u;
                    const uint32_t prev = r.u32(t0 + 4u), pressed = r.u32(t7);
                    const uint32_t t1 = r.u32(sample + 0x70u);
                    const uint32_t changed = t1 ^ prev;
                    r.put32(t7, pressed | (changed & t1));
                    r.put32(t0 + 4u, t1);
                    const uint32_t c = r.u32(t0 + 0xCu), held = r.u32(t0);
                    r.put32(t0 + 0xCu, c | (changed & ~t1));
                    r.put32(t0, held | t1);
                    r.put32(t0 + 0x10u, r.u32(t0 + 0x10u) | r.u32(sample + 0x80u));
                    const uint32_t src = t0 + 0xCu + static_cast<uint32_t>(idx) * 0x78u + 0xC0u;
                    for (uint32_t k = 0; k < 4u; ++k)
                        r.put8(t0 + 0x54u + k, r.u8(src + k));
                    uint32_t a1 = t7 + 0xE80u, a2 = t7 + 0xF00u;
                    for (uint32_t bit = 0; bit < 32u; ++bit, a1 += 4u, a2 += 4u)
                    {
                        if ((t1 >> bit) & 1u)
                            r.put32(a1, r.u32(a1) + 1u);
                        else
                        {
                            const uint32_t run = r.u32(a1);
                            uint32_t best = r.u32(a1 + 0x80u);
                            if (best < run)
                                best = run;
                            r.put32(a2, best);
                            r.put32(a1, 0u);
                        }
                    }
                }
                // the analog bytes against the ring back from the head
                const int32_t head = static_cast<int32_t>(r.u32(kHead));
                const uint32_t t5 = t0 + 0xCu;
                for (uint32_t t4 = 0; t4 < 4u; ++t4)
                {
                    const uint32_t a3 = r.u8(t0 + 0x54u + t4);
                    uint32_t t1 = 0u;
                    bool far = false;
                    int32_t a1 = head - 1;
                    for (int32_t a2 = 2; ; ++a2)
                    {
                        const int32_t i = a1 + 0x1E < 0x1E ? a1 + 0x1E : a1;
                        const uint32_t v1 = r.u8(t4 + static_cast<uint32_t>(i) * 0x78u + t5 + 0xC0u);
                        const int32_t d = static_cast<int32_t>(v1 - a3);
                        const int32_t dn = static_cast<int32_t>(a3 - v1);
                        if (d < 0 ? !(dn < 0x61) : !(d < 0x61))
                        {
                            far = true;
                            break;
                        }
                        ++t1;
                        if (!(a2 < 0x1E))
                            break;
                        a1 = head - a2;
                    }
                    if (!far)
                        t1 = 0x1Eu;
                    r.put32(t0 + 0xF88u + 4u * t4, t1);
                }
            }
            r.put32(kCount, 0u);
        }
        // SignalSema(*0x121b04c): the original's tail jump, returning to its caller.
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.u32(kSema)));
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, kSignalSite), ra0, sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0114B0A8u, FlushPadHistory, decomp_padhist::flush);
