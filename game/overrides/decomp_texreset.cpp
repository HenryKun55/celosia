// manager_sub_010AB618: the texture manager's per-frame reset of its
// records' transfer state (texTransResolve calls it three times a frame,
// texScaleClutByName runs into it): for every node of the list at
// *0x118c184 (+0xc next, +4 the record), a record with +0x24 set gets
// +0x10..+0x1c zeroed. ~1 000 nodes a frame in the attract demo: 0.6% of
// the throttled Vita3K game thread in the recompiled loop (four guest
// loads / stores and a checkpoint test a node).
//
// A node or record outside main RAM (or a list longer than 1 << 20)
// hands the walk to the recompiled code at its loop head (0x10ab630) with
// the registers it has there. v0 / v1 / a0 as the original leaves them.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "a9sweep.h"

namespace decomp_texreset
{
    constexpr uint32_t kReset = 0x010AB618u, kHead = 0x0118C184u;

    // The fallback to the recompiled loop (both paths): a0 the node, v1 its
    // record (when readable); nothing done yet: the whole function.
    __attribute__((noinline, cold)) void handOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t node, bool first)
    {
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(node));
        if (first)
            return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
        uint32_t rec;
        std::memcpy(&rec, rdram + ((node + 4u) & PS2_RAM_MASK), 4);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
        ctx->pc = 0x010AB630u;
        return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
    }

    // Round 49 (SOTC_FASTNATIVE): the same walk with the node's record and next pointer loaded
    // together (next re-read after a clear: a record may overlap its node),
    // the next node prefetched as soon as its address is known, the length
    // limit a down-counter.
    inline uint32_t ld32(const uint8_t *p)
    {
        uint32_t v;
        std::memcpy(&v, p, 4);
        return v;
    }
    void resetFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint32_t node;
        std::memcpy(&node, rdram + kHead, 4);
        if (!node)
        {
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_U64(ctx, 4, 0u);
            return sotc_decomp::returnTo(ctx);
        }
        uint32_t rec, flag;
        uint32_t n = 1u << 20; // 1 << 20 minus the node's index (1 << 20: the first, nothing done)
        for (;;)
        {
            // ok(node, 0x10) / ok(rec, 0x28): a <= size - bytes.
            if (__builtin_expect(node > PS2_RAM_SIZE - 0x10u, 0))
                return handOff(rdram, ctx, runtime, node, n == (1u << 20));
            const uint8_t *const np = rdram + node;
            rec = ld32(np + 4u);
            uint32_t next = ld32(np + 0xCu);
            if (__builtin_expect(rec > PS2_RAM_SIZE - 0x28u, 0))
                return handOff(rdram, ctx, runtime, node, n == (1u << 20));
            __builtin_prefetch(rdram + next); // a hint: no fault whatever the address
            flag = ld32(rdram + rec + 0x24u);
            if (__builtin_expect(flag != 0u, 0))
            {
                std::memset(rdram + rec + 0x10u, 0, 16);
                next = ld32(np + 0xCu);
            }
            node = next;
            if (!node)
                break;
            // The original's n >= 1 << 20 test at the next node (its three
            // tests all hand off the same way, so their order is free).
            if (__builtin_expect(--n == 0u, 0))
                return handOff(rdram, ctx, runtime, node, false);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(flag));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
        SET_GPR_U64(ctx, 4, 0u);
        sotc_decomp::returnTo(ctx);
    }

    // (Round F1: round 128's host index of the list, texr128 / SOTC_TEXR128, deleted: no gain in Vita3K, its console A/B never ran.)
    void reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("texResetTransfers");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return resetFast(rdram, ctx, runtime);
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        auto ok = [](uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; };
        uint32_t node = u32(kHead);
        if (!node)
        {
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_U64(ctx, 4, 0u);
            return sotc_decomp::returnTo(ctx);
        }
        uint32_t rec = 0u, flag = 0u;
        bool first = true;
        for (uint32_t n = 0; node; ++n)
        {
            if (!ok(node, 0x10u) || (rec = u32(node + 4u), !ok(rec, 0x28u)) || n >= (1u << 20))
            {
                // The recompiled loop from its head: a0 the node, v1 its record (when readable).
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(node));
                if (first)
                {
                    // Nothing done yet: the whole function.
                    return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
                }
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(u32(node + 4u)));
                ctx->pc = 0x010AB630u;
                return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
            }
            first = false;
            flag = u32(rec + 0x24u);
            if (flag)
                std::memset(rdram + rec + 0x10u, 0, 16);
            node = u32(node + 0xCu);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(flag));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
        SET_GPR_U64(ctx, 4, 0u);
        sotc_decomp::returnTo(ctx);
    }
}

namespace decomp_texreset
{
    void resetSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_AB_FUNCTION(0x010AB618u, texResetTransfers, decomp_texreset::resetSw);
D1_ALIAS(0x010AB618u, texResetTransfers, decomp_texreset::resetSw, decomp_texreset::reset);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): the default path (round 49's resetFast) as a typed core -------------------------
// The same walk on d1::Mem: each node's record and next word, the record's +0x24 flag, the 16 bytes at +0x10 cleared
// when it is set and the next word read again after the clear (a record may overlap its node), the same limit. A node
// or record outside main RAM, or a list too long, ends the core where resetFast hands off; the adapter then calls the
// same handOff (the recompiled loop from that node, or the whole function when nothing was done), as resetFast does.
// fastnative off: the old body.
namespace decomp_texreset
{
    D1_COUNTER(c_d2TexReset, "texResetTransfers");

    struct ResetOut
    {
        uint32_t flag, rec;
        uint32_t handNode; // handing off: the node
        bool empty;        // the list was empty
        bool handOff;      // the recompiled loop from handNode
        bool first;        // ... nothing done yet
    };

    void resetCore(const d1::Mem m, ResetOut &o)
    {
        o = ResetOut{0u, 0u, 0u, false, false, false};
        uint32_t node = m.u32(kHead);
        if (!node)
        {
            o.empty = true;
            return;
        }
        uint32_t n = 1u << 20; // 1 << 20 minus the node's index
        for (;;)
        {
            if (__builtin_expect(node > PS2_RAM_SIZE - 0x10u, 0))
            {
                o.handOff = true, o.handNode = node, o.first = n == (1u << 20);
                return;
            }
            const uint32_t rec = m.u32(node + 4u);
            uint32_t next = m.u32(node + 0xCu);
            if (__builtin_expect(rec > PS2_RAM_SIZE - 0x28u, 0))
            {
                o.handOff = true, o.handNode = node, o.first = n == (1u << 20);
                return;
            }
            __builtin_prefetch(m.at<uint8_t>(next)); // a hint
            o.rec = rec;
            o.flag = m.u32(rec + 0x24u);
            if (__builtin_expect(o.flag != 0u, 0))
            {
                std::memset(m.at<uint8_t>(rec + 0x10u), 0, 16);
                next = m.u32(node + 0xCu);
            }
            node = next;
            if (!node)
                return;
            if (__builtin_expect(--n == 0u, 0))
            {
                o.handOff = true, o.handNode = node, o.first = false;
                return;
            }
        }
    }

    void resetD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d2TexReset.n;
        if (!sotc_decomp::fastNativeOn())
            return reset(rdram, ctx, runtime);
        ResetOut o;
        resetCore(d1::Mem{rdram}, o);
        if (o.empty)
        {
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_U64(ctx, 4, 0u);
            return sotc_decomp::returnTo(ctx);
        }
        if (o.handOff)
            return handOff(rdram, ctx, runtime, o.handNode, o.first);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.flag));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(o.rec));
        SET_GPR_U64(ctx, 4, 0u);
        sotc_decomp::returnTo(ctx);
    }

    void resetSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            resetD1(rdram, ctx, runtime);
        else
            reset(rdram, ctx, runtime);
    }
}
