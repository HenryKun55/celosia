// Small hot leaves of the display list and the display sort (round 6):
// each a few dozen instructions called hundreds of times a frame, ~0.4-0.6%
// of the calibrated Vita3K game thread apiece in the recompiled code.
//
// AddSortObj (0x110dc20, a0 the sort list, a1 the object): the object goes
// into the bucket of its key (+4) x the list's scale (+8, a float),
// truncated and clamped to 0..count-1 (+4), reversed when the list's mode
// (+0xc) is 1, and into that bucket's chain (+8 one way, +0xc the other)
// in key order: mode 0 walks +8 while the chain's key is below the new one,
// mode 1 while it is above, any other mode goes straight to the +0xc end.
//
// iosDlChainTailCurrent (0x1bfe88, a0 the DMA tag's upper word): a 32-byte
// packet taken from the display list's work buffer (*0x2132e8, +0x20), made
// a tag (qword 0: a0 << 32 | 0x50000000, qword 1: the current tail's +0x14
// address << 32 | 0x20000000) and linked behind the current layer's tail
// (+0x24 of the layer record: its tag's address bits 32..62 set to the new
// packet), which becomes the tail.
//
// Not here: iosDlPushLayer / iosDlPopLayer (0x1c0210 / 0x1bf8c0, ~0.4% each).
// sotc_native.cpp hooks them (the native renderer's layer stack), and
// sotc_decomp::install runs after the hooks: a native registered for them
// replaces the hook (the first try did, round 6). They need the hook to call
// the native one instead.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_dl
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kAddSortObj = 0x0110DC20u, kChainTail = 0x001BFE88u;
    constexpr uint32_t kDl = 0x002132D8u;

    struct Ram
    {
        uint8_t *rdram;
        static bool ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
    };

    inline void setCond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // ---- AddSortObj ------------------------------------------------------
    void addSortObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram m{rdram};
        const uint32_t list = GPR_U32(ctx, 4), obj = GPR_U32(ctx, 5);
        if (!Ram::ok(list, 0x10u) || !Ram::ok(obj, 0x10u) || (list & 3u) || (obj & 3u))
            return sotc_decomp::recompiled(kAddSortObj)(rdram, ctx, runtime);
        const float key = m.f32(obj + 4u);
        const int32_t count = static_cast<int32_t>(m.u32(list + 4u));
        const float product = FPU_MUL_S(key, m.f32(list + 8u));
        const int32_t word = FPU_CVT_W_S(product);
        int32_t index;
        if (word < count)
            index = word < 0 ? 0 : word;
        else
            index = count - 1;
        const int32_t mode = static_cast<int32_t>(m.u32(list + 0xCu));
        if (mode == 1)
            index = count - index - 1;
        const uint32_t offset = static_cast<uint32_t>(index) << 2;
        const uint32_t buckets = m.u32(list);
        const uint32_t slot = offset + buckets;
        if (!Ram::ok(slot, 4u) || (slot & 3u))
            return sotc_decomp::recompiled(kAddSortObj)(rdram, ctx, runtime);
        // The chain checked before anything is written.
        uint32_t head = m.u32(slot);
        float f0 = product;
        bool cond = (ctx->fcr31 & 0x800000u) != 0u;
        ctx->f[1] = key;
        {
            int32_t w = word;
            std::memcpy(&ctx->f[2], &w, 4);
        }
        if (head == 0u)
        {
            m.put32(slot, obj);
            m.put32(obj + 8u, 0u);
            m.put32(obj + 0xCu, 0u);
            ctx->f[0] = f0;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(offset));
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(buckets));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(slot));
            SET_GPR_S32(ctx, 8, mode);
            return returnTo(ctx);
        }
        // Walk read-only first (the chain is not written until the end).
        uint32_t node = head, next = 0u;
        bool toPrev = false; // the walk's exit: false at a +0xc end (dcb8), true at a +8 end (dd0c)
        for (uint32_t steps = 0;; ++steps)
        {
            if (!Ram::ok(node, 0x10u) || (node & 3u) || steps > (1u << 20))
                return sotc_decomp::recompiled(kAddSortObj)(rdram, ctx, runtime);
            bool goPrev = false;
            if (mode == 0)
            {
                f0 = m.f32(node + 4u);
                cond = FPU_C_OLT_S(f0, key);
                goPrev = cond;
            }
            if (!goPrev)
            {
                if (mode != 1)
                    goPrev = false;
                else
                {
                    f0 = m.f32(node + 4u);
                    cond = FPU_C_OLT_S(key, f0);
                    goPrev = cond;
                }
            }
            if (goPrev)
            {
                next = m.u32(node + 8u);
                if (next != 0u)
                {
                    node = next;
                    continue;
                }
                toPrev = true;
            }
            else
                next = m.u32(node + 0xCu);
            break;
        }
        ctx->f[0] = f0;
        setCond(ctx, cond);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(buckets));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(node));
        SET_GPR_S32(ctx, 7, mode);
        SET_GPR_S32(ctx, 8, mode);
        SET_GPR_U64(ctx, 9, 1u);
        if (toPrev)
        {
            // dd14: the new object first in the +8 direction.
            m.put32(obj + 8u, 0u);
            m.put32(obj + 0xCu, node);
            m.put32(node + 8u, obj);
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(offset));
            return returnTo(ctx);
        }
        if (next == 0u)
        {
            // dc90: the bucket's head becomes the new object, after `node`.
            m.put32(slot, obj);
            m.put32(obj + 0xCu, 0u);
            m.put32(obj + 8u, node);
            m.put32(node + 0xCu, obj);
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(slot));
            return returnTo(ctx);
        }
        // dcc0: between `node` and its +0xc neighbour.
        if (!Ram::ok(next, 0x10u))
            return sotc_decomp::recompiled(kAddSortObj)(rdram, ctx, runtime); // nothing written yet
        m.put32(next + 8u, obj);
        const uint32_t v0 = m.u32(node + 0xCu);
        m.put32(obj + 8u, node);
        m.put32(obj + 0xCu, v0);
        m.put32(node + 0xCu, obj);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(next));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    // ---- iosDlChainTailCurrent --------------------------------------------
    void chainTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram m{rdram};
        const uint32_t layer = m.u32(kDl + 0x1Cu), buf = m.u32(kDl + 0x18u);
        const uint32_t record = m.u32(kDl + 8u + (buf << 2)) + layer * 48u;
        const uint32_t packet = m.u32(kDl + 0x10u);
        if (!Ram::ok(record, 0x28u) || !Ram::ok(packet, 0x20u) || (packet & 7u))
            return sotc_decomp::recompiled(kChainTail)(rdram, ctx, runtime);
        const uint32_t tail0 = m.u32(record + 0x24u);
        if (!Ram::ok(tail0, 0x18u) || (tail0 & 7u))
            return sotc_decomp::recompiled(kChainTail)(rdram, ctx, runtime);
        m.put32(kDl + 0x10u, packet + 0x20u);
        __builtin_prefetch(rdram + ((packet + 0x40u) & PS2_RAM_MASK));
        const uint64_t a0 = (static_cast<uint64_t>(GPR_U32(ctx, 4)) << 32) | 0x50000000u;
        m.put32(packet + 0x1Cu, 0u);
        m.put64(packet, a0);
        m.put32(packet + 0x18u, 0u);
        m.put32(packet + 0xCu, 0u);
        m.put32(packet + 8u, 0u);
        const uint64_t addr = static_cast<uint64_t>(packet & 0x7FFFFFFFu) << 32;
        // The tail read again after the packet's stores, as the original does.
        const uint32_t first = m.u32(record + 0x24u);
        const uint64_t link = (static_cast<uint64_t>(READ32(ADD32(first, 0x14u)) & 0x7FFFFFFFu) << 32) | 0x20000000u;
        m.put64(packet + 0x10u, link);
        const uint32_t tail = m.u32(record + 0x24u);
        const uint64_t v0 = (READ64(ADD32(tail, 0x10u)) & 0x80000000FFFFFFFFull) | addr;
        WRITE64(ADD32(tail, 0x10u), v0);
        m.put32(record + 0x24u, packet);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(tail));
        SET_GPR_U64(ctx, 4, addr);
        SET_GPR_U64(ctx, 5, 0x7FFFFFFFu);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(packet));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(record));
        SET_GPR_U64(ctx, 8, 0x20000000u);
        SET_GPR_U64(ctx, 9, 0x80000000FFFFFFFFull);
        returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x0110DC20u, AddSortObj, decomp_dl::addSortObj);
SOTC_DECOMP_AB_FUNCTION(0x001BFE88u, iosDlChainTailCurrent, decomp_dl::chainTail);
