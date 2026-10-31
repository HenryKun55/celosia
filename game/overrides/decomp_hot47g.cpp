// Round 47 (SOTC_HOTLIST47=0 puts all three back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): the texture manager's
// VRAM block allocator next to texTransRequest (0x010AAD48). A manager (a0 of
// the allocator) keeps +0x50 a rover block, +0x54 / +0x58 the free list (head,
// count) and +0x5c / +0x60 the used list (head, count); unused nodes sit in
// the pool at 0x0118C464 (head, +4 count). A node is +0 prev / +4 next (the
// list links) and +8 its block record: +0 the state (1 free, 2 used), +4 the
// packed word (bit 0 and bit 31 kept, bits 1-15 the size, bits 16-30 the
// start), +8 the owning manager, +0xc the node itself, +0x10 / +0x14 the
// previous / next block in address order.
//
// - manager_sub_010AB6D0 (0x10ab6d0, leaf): places the new used node a0 into
//   the free block of node a1. Same size: the new block takes the free one's
//   neighbours, the free node leaves the free list for the pool. Smaller: the
//   new block goes in before it, the free block keeps the rest (size minus,
//   start plus the new size). Either way the manager's rover (+0x50) moves to
//   a0 if it was a1. v0: the rover word as read.
// - manager_sub_010AB848 (0x10ab848): allocate a1 units from manager a0:
//   the first free block (the free list from +0x54) of size >= a1; a pool node
//   goes to the used list, its record (used, size a1, the free block's start,
//   the manager, no neighbours yet) and manager_sub_010AB6D0(node, free node).
//   v0 the new node (0 when nothing fits).
// - manager_sub_010AB9A0 (0x10ab9a0, leaf): frees node a0: state free, from
//   the used list to the free list; a free next block is merged into it (the
//   next's node to the pool), then the block merged into a free previous one
//   (its own node to the pool). v0: the pool count after the second merge, else 0.
//
// Every load and store in the original's order (the lists' head checks, the
// unlinks' re-reads), the 64-bit compares of the argument registers against
// sign-extended loads as the code makes them; manager_sub_010AB848's frame
// (s0 / ra saves at sp - 0x10) and s0 = the new node during its call, s0 / ra
// restored from the frame. The call goes through the function table
// (callGuest) with the original's ra and frame sp.
// Not as the original: the scratch GPRs (v1, a0-a3, t0-t6: only the call's
// arguments a0 / a1 are set, and v0 at the return), the loop's EE checkpoint
// (left out), the pref instructions (no effect).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

namespace decomp_hot47g
{
    using sotc_decomp::callGuest;

    constexpr uint32_t kPlace = 0x010AB6D0u, kAlloc = 0x010AB848u, kFree = 0x010AB9A0u;
    constexpr uint32_t kPool = 0x0118C464u; // lui 0x119; addiu -0x3b9c: the unused nodes (head, count)

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // The packed record word's fields.
    inline uint32_t sizeOf(uint32_t word) { return (word >> 1) & 0x7fffu; }
    inline uint32_t startOf(uint32_t word) { return (word >> 16) & 0x7fffu; }

    // The unlink of node n from the list at L (L + 4 its count), after the
    // head check: prev->next = n->next, next->prev = n->prev, count - 1.
    inline void unlinkNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n, uint32_t list)
    {
        uint32_t p = READ32(n);
        if (p != 0u)
        {
            WRITE32(p + 4u, READ32(n + 4u));
            p = READ32(n + 4u);
        }
        else
            p = READ32(n + 4u);
        if (p != 0u)
            WRITE32(p, READ32(n));
        WRITE32(list + 4u, READ32(list + 4u) - 1u);
    }

    // The push of node n at the head of the list at L; the new count.
    inline uint32_t pushNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n, uint32_t list)
    {
        const uint32_t head = READ32(list);
        if (head != 0u)
            WRITE32(head, n);
        WRITE32(n, 0u);
        WRITE32(n + 4u, READ32(list));
        WRITE32(list, n);
        const uint32_t count = READ32(list + 4u) + 1u;
        WRITE32(list + 4u, count);
        return count;
    }

    // The unlink of block record r from the address-order chain (+0x10 / +0x14).
    inline void unlinkRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t r)
    {
        uint32_t p = READ32(r + 0x10u);
        if (p != 0u)
        {
            WRITE32(p + 0x14u, READ32(r + 0x14u));
            p = READ32(r + 0x14u);
        }
        else
            p = READ32(r + 0x14u);
        if (p != 0u)
            WRITE32(p + 0x10u, READ32(r + 0x10u));
    }

    // ---- manager_sub_010AB6D0 (0x10ab6d0) ---------------------------------
    void place(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kPlace, rdram, ctx, runtime);

        const uint64_t a0 = GPR_U64(ctx, 4), a1 = GPR_U64(ctx, 5); // t3 the new node, t1 the free node
        const uint32_t t3 = static_cast<uint32_t>(a0), t1 = static_cast<uint32_t>(a1);
        const uint32_t rec = t3 + 8u, freeRec = t1 + 8u; // a3, t0
        const uint32_t size = sizeOf(READ32(rec + 4u));
        const uint32_t freeSize = sizeOf(READ32(freeRec + 4u));
        const uint32_t mgr = READ32(rec + 8u); // t2

        if (size == freeSize)
        {
            // The new block takes the free one's place in the chain.
            uint32_t v = READ32(freeRec + 0x10u);
            if (v != 0u)
                WRITE32(v + 0x14u, rec);
            v = READ32(freeRec + 0x14u);
            if (v != 0u)
                WRITE32(v + 0x10u, rec);
            const uint32_t freeList = mgr + 0x54u; // a1
            WRITE32(rec + 0x10u, READ32(freeRec + 0x10u));
            WRITE32(rec + 0x14u, READ32(freeRec + 0x14u));
            WRITE32(freeRec, 0u); // state 0
            // The free node from the free list to the pool.
            if (sx(READ32(freeList)) == a1)
                WRITE32(freeList, READ32(t1 + 4u));
            unlinkNode(rdram, ctx, runtime, t1, freeList);
            pushNode(rdram, ctx, runtime, t1, kPool);
        }
        else
        {
            // The new block before the free one; the free block keeps the rest.
            const uint32_t prev = READ32(freeRec + 0x10u);
            if (prev == 0u)
                WRITE32(rec + 0x10u, prev);
            else
            {
                WRITE32(prev + 0x14u, rec);
                WRITE32(rec + 0x10u, READ32(freeRec + 0x10u));
            }
            WRITE32(rec + 0x14u, freeRec);
            WRITE32(freeRec + 0x10u, rec);
            uint32_t word = READ32(freeRec + 4u);
            word = (word & 0xffff0001u) | (((sizeOf(word) - sizeOf(READ32(rec + 4u))) & 0x7fffu) << 1);
            WRITE32(freeRec + 4u, word);
            word = (word & 0x8000ffffu) | (((startOf(word) + sizeOf(READ32(rec + 4u))) & 0x7fffu) << 16);
            WRITE32(freeRec + 4u, word);
        }

        // The rover moves to the new node if it was the free one.
        const uint32_t rover = READ32(mgr + 0x50u);
        if (sx(rover) == a1)
            WRITE32(mgr + 0x50u, t3);

        SET_GPR_U64(ctx, 2, sx(rover));
        sotc_decomp::returnTo(ctx);
    }

    // ---- manager_sub_010AB848 (0x10ab848) ---------------------------------
    // The frame (sp = entry sp - 0x10): +0 the s0 save, +8 the ra save.
    void alloc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kAlloc, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x10u;
        const uint32_t mgr = GPR_U32(ctx, 4);    // t3
        const int64_t want = GPR_S64(ctx, 5);    // a2: the size wanted
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, 0u); // move s0, zero

        uint64_t result = 0u;
        uint32_t node = READ32(mgr + 0x54u); // a1: the free list
        while (node != 0u)
        {
            const uint32_t word = READ32(node + 0xcu);
            if (static_cast<int64_t>(sizeOf(word)) < want)
            {
                node = READ32(node + 4u);
                continue;
            }
            // A pool node (the head) to the manager's used list.
            const uint32_t t1 = READ32(kPool);
            WRITE32(kPool, READ32(t1 + 4u));
            unlinkNode(rdram, ctx, runtime, t1, kPool);
            const uint32_t used = mgr + 0x5cu; // t2
            pushNode(rdram, ctx, runtime, t1, used);
            SET_GPR_U64(ctx, 16, sx(t1)); // move s0, t1
            // Its record: used, size wanted, the free block's start.
            const uint32_t rec = t1 + 8u;
            WRITE32(rec, 2u);
            uint32_t v = READ32(rec + 4u);
            WRITE32(rec + 8u, mgr);
            v &= 0xfffffffeu;
            WRITE32(rec + 0x14u, 0u);
            v &= 0x8000ffffu;
            WRITE32(rec + 0xcu, t1);
            v |= startOf(word) << 16;
            WRITE32(rec + 0x10u, 0u);
            v &= 0xffff0001u;
            v |= (static_cast<uint32_t>(want) & 0x7fffu) << 1;
            WRITE32(rec + 4u, v);
            // manager_sub_010AB6D0(the new node, the free node)
            SET_GPR_U64(ctx, 4, sx(t1));
            SET_GPR_U64(ctx, 5, sx(node));
            callGuest(rdram, ctx, runtime, kPlace, 0x010AB974u, sp);
            result = GPR_U64(ctx, 16); // move v0, s0
            break;
        }

        SET_GPR_U64(ctx, 2, result);
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        SET_GPR_U64(ctx, 29, entrySp);
        sotc_decomp::returnTo(ctx);
    }

    // The end of a merge (0x10abab4 / 0x10abbb8): the merged-away block's node
    // goneNode (state 0) from the manager's free list to the pool; the pool count.
    inline uint32_t mergeTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t rec, uint32_t goneNode)
    {
        const uint32_t freeList = READ32(rec + 8u) + 0x54u;
        WRITE32(goneNode + 8u, 0u); // its state 0
        if (READ32(freeList) == goneNode)
            WRITE32(freeList, READ32(goneNode + 4u));
        unlinkNode(rdram, ctx, runtime, goneNode, freeList);
        return pushNode(rdram, ctx, runtime, goneNode, kPool);
    }

    // ---- manager_sub_010AB9A0 (0x10ab9a0) ---------------------------------
    void freeBlock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kFree, rdram, ctx, runtime);

        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint32_t n = static_cast<uint32_t>(a0);
        const uint32_t rec = n + 8u; // a3
        const uint32_t mgr = READ32(rec + 8u);
        WRITE32(rec, 1u); // state free
        const uint32_t used = mgr + 0x5cu, freeList = mgr + 0x54u; // a2, v1
        const uint32_t usedHead = READ32(used);
        const uint32_t next = READ32(rec + 0x14u); // t0
        const uint32_t prev = READ32(rec + 0x10u); // t1
        if (sx(usedHead) == a0)
            WRITE32(used, READ32(n + 4u));
        unlinkNode(rdram, ctx, runtime, n, used);
        pushNode(rdram, ctx, runtime, n, freeList);

        // A free next block merged into this one.
        if (next != 0u && READ32(next) == 1u)
        {
            uint32_t word = READ32(rec + 4u);
            const uint32_t nextSize = sizeOf(READ32(next + 4u));
            const uint32_t r = READ32(rec + 0x14u); // v1
            word = (word & 0xffff0001u) | (((sizeOf(word) + nextSize) & 0x7fffu) << 1);
            WRITE32(rec + 4u, word);
            unlinkRecord(rdram, ctx, runtime, r);
            mergeTail(rdram, ctx, runtime, rec, READ32(next + 0xcu));
        }

        // This block merged into a free previous one.
        uint32_t v0 = 0u;
        if (prev != 0u && READ32(prev) == 1u)
        {
            uint32_t word = READ32(prev + 4u);
            const uint32_t mySize = sizeOf(READ32(rec + 4u));
            word = (word & 0xffff0001u) | (((sizeOf(word) + mySize) & 0x7fffu) << 1);
            WRITE32(prev + 4u, word);
            unlinkRecord(rdram, ctx, runtime, rec);
            v0 = mergeTail(rdram, ctx, runtime, rec, READ32(rec + 0xcu));
        }

        SET_GPR_U64(ctx, 2, sx(v0));
        sotc_decomp::returnTo(ctx);
    }
} // namespace decomp_hot47g

SOTC_DECOMP_FUNCTION(0x010AB6D0u, manager_sub_010AB6D0, decomp_hot47g::place);
SOTC_DECOMP_FUNCTION(0x010AB848u, manager_sub_010AB848, decomp_hot47g::alloc);
SOTC_DECOMP_FUNCTION(0x010AB9A0u, manager_sub_010AB9A0, decomp_hot47g::freeBlock);
