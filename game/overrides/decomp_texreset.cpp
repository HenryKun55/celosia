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
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

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

    // ---- Round 128 (SOTC_TEXR128, default on; A/B `texr128`, `r128all`): the
    // list as a host index kept across calls: its nodes and their records in
    // list order. A call checks each kept node's record and next words (the
    // addresses known in advance: the loads independent, prefetched ahead,
    // instead of one dependent load after another) and clears as the walk
    // does; at the first node whose words moved it walks on from there the
    // original way and keeps what it finds. The two-pass order (next read
    // before the clear) is the walk's only when no record's cleared bytes
    // (+0x10..+0x1f) overlap a node's record / next words: checked when the
    // index is made, else it is not used. SOTC_TEXR128_LOG=1: [texr128].
    struct Texr128
    {
        std::vector<uint32_t> nodes, recs;
        bool valid = false, overlap = false;
        uint64_t calls = 0, made = 0, kept = 0, overlapCalls = 0;
    };
    Texr128 g_texr;

    bool overlaps128(const Texr128 &t)
    {
        // The nodes' word ranges [+4, +8) and [+0xc, +0x10), sorted by start; each record's [+0x10, +0x20) against them.
        std::vector<uint32_t> starts;
        starts.reserve(t.nodes.size() * 2u);
        for (uint32_t n : t.nodes)
        {
            starts.push_back(n + 4u);
            starts.push_back(n + 0xCu);
        }
        std::sort(starts.begin(), starts.end());
        for (uint32_t r : t.recs)
        {
            const uint32_t lo = r + 0x10u, hi = r + 0x20u;
            // a word [s, s + 4) overlaps [lo, hi) when s < hi and s + 4 > lo: the first s > lo - 4
            auto it = std::upper_bound(starts.begin(), starts.end(), lo >= 4u ? lo - 4u : 0u);
            if (it != starts.end() && *it < hi)
                return true;
        }
        return false;
    }

    void resetIndexed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint32_t node;
        std::memcpy(&node, rdram + kHead, 4);
        if (!node)
        {
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_U64(ctx, 4, 0u);
            return sotc_decomp::returnTo(ctx);
        }
        Texr128 &t = g_texr;
        // Overlapping records (never seen): the walk, the index tried again every 256 calls.
        if (__builtin_expect(t.overlap, 0) && (++t.overlapCalls & 255u) != 0u)
            return resetFast(rdram, ctx, runtime);
        ++t.calls;
        uint32_t rec = 0u, flag = 0u;
        size_t i = 0;
        const size_t n = t.nodes.size();
        if (t.valid && n != 0u && t.nodes[0] == node)
        {
            const uint32_t *const nodes = t.nodes.data(), *const recs = t.recs.data();
            for (; i < n; ++i)
            {
                if (i + 8u < n)
                {
                    __builtin_prefetch(rdram + nodes[i + 8u]);
                    __builtin_prefetch(rdram + recs[i + 8u] + 0x10u);
                }
                const uint8_t *const np = rdram + nodes[i];
                const uint32_t r = ld32(np + 4u), next = ld32(np + 0xCu);
                if (__builtin_expect(r != recs[i] || next != (i + 1u < n ? nodes[i + 1u] : 0u), 0))
                    break;
                rec = r;
                flag = ld32(rdram + r + 0x24u);
                if (flag != 0u)
                    std::memset(rdram + r + 0x10u, 0, 16);
            }
            if (i == n)
            {
                ++t.kept;
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(flag));
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
                SET_GPR_U64(ctx, 4, 0u);
                return sotc_decomp::returnTo(ctx);
            }
            node = nodes[i]; // reached by the walk; its words moved: the walk from here
        }
        else
            i = 0;
        ++t.made;
        t.valid = false;
        t.nodes.resize(i);
        t.recs.resize(i);
        uint32_t left = (1u << 20) - static_cast<uint32_t>(i);
        for (;;)
        {
            if (__builtin_expect(node > PS2_RAM_SIZE - 0x10u, 0))
                return handOff(rdram, ctx, runtime, node, t.nodes.empty());
            const uint8_t *const np = rdram + node;
            rec = ld32(np + 4u);
            if (__builtin_expect(rec > PS2_RAM_SIZE - 0x28u, 0))
                return handOff(rdram, ctx, runtime, node, t.nodes.empty());
            t.nodes.push_back(node);
            t.recs.push_back(rec);
            flag = ld32(rdram + rec + 0x24u);
            if (flag != 0u)
                std::memset(rdram + rec + 0x10u, 0, 16);
            node = ld32(np + 0xCu);
            if (!node)
                break;
            if (__builtin_expect(--left == 0u, 0))
            {
                t.nodes.clear();
                t.recs.clear();
                return handOff(rdram, ctx, runtime, node, false);
            }
        }
        t.overlap = overlaps128(t);
        t.valid = !t.overlap;
        static const bool log = ps2_diag::env("SOTC_TEXR128_LOG") != nullptr;
        if (log && (t.made <= 4u || t.made % 200u == 0u))
            std::fprintf(stderr, "[texr128] calls %llu kept %llu made %llu (nodes %u, from %u; usable %d)\n", (unsigned long long)t.calls,
                         (unsigned long long)t.kept, (unsigned long long)t.made, (unsigned)t.nodes.size(), (unsigned)i, t.valid ? 1 : 0);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(flag));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
        SET_GPR_U64(ctx, 4, 0u);
        sotc_decomp::returnTo(ctx);
    }

    void reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("texResetTransfers");
        count.hit();
        if (sotc_decomp::switchOn(sotc_decomp::kTexr128))
        {
            // SOTC_TEXR128_CHECK=1: the list walked read-only first (each record, its flag, its 16 bytes),
            // then the indexed call; every record must hold zeros where flagged, its bytes otherwise.
            static const bool check = ps2_diag::env("SOTC_TEXR128_CHECK") != nullptr;
            if (!check)
                return resetIndexed(rdram, ctx, runtime);
            struct Seen
            {
                uint32_t rec, flag;
                uint8_t bytes[16];
            };
            static std::vector<Seen> seen;
            static uint64_t checks = 0, differ = 0;
            seen.clear();
            uint32_t node = ld32(rdram + kHead);
            for (uint32_t k = 0; node && node <= PS2_RAM_SIZE - 0x10u && k < (1u << 20); ++k)
            {
                const uint32_t rec = ld32(rdram + node + 4u);
                if (rec > PS2_RAM_SIZE - 0x28u)
                    break;
                Seen s{rec, ld32(rdram + rec + 0x24u), {}};
                std::memcpy(s.bytes, rdram + rec + 0x10u, 16);
                seen.push_back(s);
                node = ld32(rdram + node + 0xCu);
            }
            resetIndexed(rdram, ctx, runtime);
            static const uint8_t zero[16] = {};
            uint32_t bad = 0;
            for (const Seen &s : seen)
                bad += std::memcmp(rdram + s.rec + 0x10u, s.flag ? zero : s.bytes, 16) != 0 ? 1u : 0u;
            ++checks;
            if (bad && differ++ < 20u)
                std::fprintf(stderr, "[texr128c] DIFFERS: %u of %u records\n", bad, (unsigned)seen.size());
            if (checks % 3000u == 0u)
                std::fprintf(stderr, "[texr128c] %llu calls checked, %llu differ (kept %llu made %llu, nodes %u)\n", (unsigned long long)checks,
                             (unsigned long long)differ, (unsigned long long)g_texr.kept, (unsigned long long)g_texr.made, (unsigned)seen.size());
            return;
        }
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

SOTC_DECOMP_AB_FUNCTION(0x010AB618u, texResetTransfers, decomp_texreset::reset);
