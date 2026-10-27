// The skinned collision finder's triangle corners (round 14):
// manager_sub_01116108, called by the finder over a character's skinned
// collision mesh (manager_sub_01116958, under GetCharaRootDynamics and the
// camera's tests) for each candidate triangle: ~0.9% of the calibrated
// Vita3K game thread on the plain, with ~1% more in its caller.
//
// manager_sub_01116108(a0 out[3], a1 pairs out or 0, a2 blend manager,
// a3 managers (0x60 each), t0 bone matrices (0x40 each), t1 vertex lists
// (0x90 each, +4 the vertices), t2 blend lists, t3 the corners' (bone,
// vertex) pairs). For each of the three corners (a1: its pair copied out
// first, 8 bytes):
// - bone >= 0: the vertex in the bone's manager. Not done yet (the
//   manager's flag byte +8[vertex] clear): the vertex times the bone's
//   matrix (vmulax / vmadday / vmaddaz / vmaddw) into the manager's cache
//   (+0x14 + 16 vertex), and EntryModifiedManager(manager, vertex). out[k]
//   = that cache entry.
// - bone -1: a blended vertex: out[k] = the blend manager's cache entry
//   (+0x14 + 16 vertex); not done yet: its list (t2[vertex]: a count, then
//   (bone, vertex) pairs, each made as above) summed in order (vadd), the
//   sum stored there and EntryModifiedManager(blend manager, vertex).
// EntryModifiedManager (0x11478b8) natively too: the manager's count +0
// goes up by one, the vertex goes into its list (+0x10: bytes, halves or
// words for the types +0xc 1, 2, 4; other types none) at the old count,
// and its flag byte is set.
//
// Left as the original leaves them: v0 = v1 = -1 (the loop counter), the
// saved registers; VU0: vf8 / vf10 / vf14-vf17 / ACC of the last product,
// vf29-vf31 of the last sum; and its stack frame (0x70 below sp: a2, a3,
// t2, the counter, the saved registers, t3 where the original stores it).
// The frame matters: with 0 difftest mismatches but the frame left out,
// Wander walked off a ledge on the Mac's fixed-30 fps route (one of the
// ~35 callers reads it later as uninitialised locals; the difftest does not
// compare the dead stack). A blend list with no entry (the original would
// count down from -2 and run away) goes to the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_skinfinder
{
    namespace vu = sotc_decomp::vu;
    using sotc_decomp::returnTo;

    constexpr uint32_t kCorners = 0x01116108u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
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
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put16(uint32_t a, uint16_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 2); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
    };

    // EntryModifiedManager(manager, index).
    inline void entry(const Ram &ram, uint32_t manager, uint32_t index)
    {
        const uint32_t count = ram.u32(manager);
        const int32_t type = static_cast<int32_t>(ram.u32(manager + 0xCu));
        ram.put32(manager, count + 1u);
        if (type == 2)
            ram.put16(ram.u32(manager + 0x10u) + count * 2u, static_cast<uint16_t>(index));
        else if (type == 4)
            ram.put32(ram.u32(manager + 0x10u) + count * 4u, index);
        else if (type == 1)
            ram.put8(ram.u32(manager + 0x10u) + count, static_cast<uint8_t>(index));
        ram.put8(ram.u32(manager + 8u) + index, 1u);
    }

    struct Last
    {
        __m128 v, rows[4], out, acc;
        bool any = false;
    };

    // The vertex (bone, vertex) made in its manager's cache if not done
    // yet; its cache entry's address.
    inline uint32_t vertexIn(const Ram &ram, uint32_t managers, uint32_t matrices, uint32_t lists, uint32_t bone, uint32_t vertex, Last &last)
    {
        const uint32_t manager = managers + bone * 0x60u;
        const uint32_t dst = ram.u32(manager + 0x14u) + vertex * 16u;
        if (ram.u8(ram.u32(manager + 8u) + vertex) == 0u)
        {
            const __m128 v = ram.q(vertex * 16u + ram.u32(bone * 0x90u + lists + 4u));
            const uint32_t m = bone * 0x40u + matrices;
            const __m128 raw[4] = {ram.q(m), ram.q(m + 0x10u), ram.q(m + 0x20u), ram.q(m + 0x30u)};
            const vu::Rows rows = vu::clampRows(raw);
            __m128 acc;
            const __m128 out = vu::apply(rows, v, &acc);
            ram.putq(dst, out);
            entry(ram, manager, vertex);
            last.v = v;
            for (int k = 0; k < 4; ++k)
                last.rows[k] = raw[k];
            last.out = out;
            last.acc = acc;
            last.any = true;
        }
        return dst;
    }

    void corners(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t out = GPR_U32(ctx, 4), pairsOut = GPR_U32(ctx, 5), blend = GPR_U32(ctx, 6), managers = GPR_U32(ctx, 7);
        const uint32_t matrices = GPR_U32(ctx, 8), lists = GPR_U32(ctx, 9), blendLists = GPR_U32(ctx, 10), pairs = GPR_U32(ctx, 11);
        // A blend list without entries: the recompiled code (as it is).
        for (uint32_t k = 0; k < 3u; ++k)
            if (ram.u32(pairs + k * 8u) == 0xFFFFFFFFu)
            {
                const uint32_t vertex = ram.u32(pairs + k * 8u + 4u);
                if (ram.u8(ram.u32(blend + 8u) + vertex) == 0u && static_cast<int32_t>(ram.u32(ram.u32(blendLists + vertex * 4u))) < 1)
                    return sotc_decomp::recompiled(kCorners)(rdram, ctx, runtime);
            }
        // SOTC_SKINFINDER_STATS=1 (checking): the corners by path.
        static const bool stats = std::getenv("SOTC_SKINFINDER_STATS") != nullptr;
        static uint64_t calls = 0, plain = 0, plainMade = 0, blended = 0, blendMade = 0;
        // The frame as the original leaves it (callers' uninitialised locals
        // may read it later): a2, a3, t2, the loop counter (-1 at the end),
        // the saved registers; t3 (as a quadword) only where the inner loop
        // of a blend with pairs out made a vertex.
        const uint32_t frame = GPR_U32(ctx, 29) - 0x70u;
        {
            static constexpr int saved[10] = {16, 17, 18, 19, 20, 21, 22, 23, 30, 31};
            for (int i = 0; i < 10; ++i)
            {
                const uint64_t v = GPR_U64(ctx, saved[i]);
                std::memcpy(rdram + ((frame + 0x20u + i * 8u) & PS2_RAM_MASK), &v, 8);
            }
            ram.put32(frame, blend);
            ram.put32(frame + 4u, managers);
            ram.put32(frame + 8u, blendLists);
            ram.put32(frame + 0xCu, 0xFFFFFFFFu);
        }
        bool wroteT3 = false;
        Last last;
        bool summed = false;
        __m128 sum = ctx->vu0_vf[31], before = ctx->vu0_vf[29], added = ctx->vu0_vf[30];
        for (uint32_t k = 0; k < 3u; ++k)
        {
            const uint32_t pair = pairs + k * 8u;
            if (pairsOut)
            {
                uint8_t b[8];
                std::memcpy(b, rdram + (pair & PS2_RAM_MASK), 8);
                std::memcpy(rdram + ((pairsOut + k * 8u) & PS2_RAM_MASK), b, 8);
            }
            const uint32_t bone = ram.u32(pair), vertex = ram.u32(pair + 4u);
            if (bone != 0xFFFFFFFFu)
            {
                if (stats)
                    ++plain, plainMade += ram.u8(ram.u32(managers + bone * 0x60u + 8u) + vertex) == 0u;
                const uint32_t dst = vertexIn(ram, managers, matrices, lists, bone, vertex, last);
                ram.put32(out + k * 4u, dst);
                continue;
            }
            const uint32_t dst = ram.u32(blend + 0x14u) + vertex * 16u;
            ram.put32(out + k * 4u, dst);
            blended += stats;
            if (ram.u8(ram.u32(blend + 8u) + vertex) != 0u)
                continue;
            blendMade += stats;
            const uint32_t list = ram.u32(blendLists + vertex * 4u);
            const int32_t n = static_cast<int32_t>(ram.u32(list));
            uint32_t at = list + 4u;
            sum = ram.q(vertexIn(ram, managers, matrices, lists, ram.u32(at), ram.u32(at + 4u), last));
            at += 8u;
            for (int32_t i = 1; i < n; ++i, at += 8u)
            {
                before = sum;
                const bool was = last.any;
                last.any = false;
                added = ram.q(vertexIn(ram, managers, matrices, lists, ram.u32(at), ram.u32(at + 4u), last));
                if (last.any && pairsOut)
                    wroteT3 = true;
                last.any = last.any || was;
                sum = vu::addN(vu::clamp(before), vu::clamp(added));
            }
            summed = true;
            ram.putq(ram.u32(out + k * 4u), sum);
            entry(ram, blend, vertex);
        }
        if (wroteT3)
        {
            // sq t3: its low half -1 (addiu), the high half as it came.
            __m128i t3 = ctx->r[11];
            const uint64_t low = ~0ull;
            std::memcpy(&t3, &low, 8);
            std::memcpy(rdram + ((frame + 0x10u) & PS2_RAM_MASK & ~0xFu), &t3, 16);
        }
        if (last.any)
        {
            ctx->vu0_vf[8] = last.v;
            for (int k = 0; k < 4; ++k)
                ctx->vu0_vf[14 + k] = last.rows[k];
            ctx->vu0_vf[10] = last.out;
            ctx->vu0_acc = last.acc;
        }
        if (summed)
        {
            ctx->vu0_vf[29] = before;
            ctx->vu0_vf[30] = added;
            ctx->vu0_vf[31] = sum;
        }
        if (stats && (++calls & 0xFFFFu) == 0u)
            std::fprintf(stderr, "[skinfinder] %llu calls: corners plain %llu (made %llu), blended %llu (made %llu)\n", (unsigned long long)calls,
                         (unsigned long long)plain, (unsigned long long)plainMade, (unsigned long long)blended, (unsigned long long)blendMade);
        SET_GPR_S64(ctx, 2, -1);
        SET_GPR_S64(ctx, 3, -1);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01116108u, skinFinderCorners, decomp_skinfinder::corners);
