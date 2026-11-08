// Round 120 (SOTC_HEAP120, A/B `heap120`): the game heap's allocator as
// plain A9 code. kernel_sub_001ADDC0 (0x1addc0) is the heaps' block
// allocator (iosMalloc / iosMallocAlign and the module loader end there):
// a0 the heap, a1 the size, a2 the alignment, a3 the owner word; v0 the
// data address or 0. kernel_sub_001ADBC0 (0x1adbc0) puts a free block in its
// size class's list (a0 the block, a1 the classes, a2 their count, a3 the
// heap). Riding the attract demo both were ~0.37% of the game thread, the
// shrine ~0.24% (the sampler).
//
// A heap: +0 the classes' word index (24 bits), +4 their count (u16). A
// class: 8 bytes, its first and last free block. A block: a 0x20-byte
// header, words packed as 24-bit word indexes under a byte of flags: +0 its
// size in words, +4 / +8 / +0xc / +0x14 / +0x18 links and flags, +0x1c the
// next free block in its class (a byte address). The heap at 0x1e0c88 has
// its own class table for small sizes (the bytes at 0x1e0c00) and a free
// count at 0x1e0bf0.
//
// Natively, exact: the same class, the same first fitting block in the same
// walk, the same split and the same words written in the same order (the
// recompiled code's packing, its sra / srl and signed / unsigned compares).
// The free block made by a split goes into its list by the same code as
// 0x1adbc0 (inline). Not written: the callee-saved registers' spills in the
// native's frame (dead stack below the caller's sp; no guest code runs in
// between). v0 as the original returns it.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_heap120
{
    constexpr uint32_t kAlloc = 0x001ADDC0u, kInsert = 0x001ADBC0u;
    constexpr uint32_t kSmallHeap = 0x001E0C88u, kSmallClass = 0x001E0C00u, kSmallFree = 0x001E0BF0u;
    constexpr uint32_t kIdx = 0x00FFFFFFu, kTop = 0xFF000000u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline int32_t sra(uint32_t v, unsigned n) { return static_cast<int32_t>(v) >> n; }

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        int8_t s8(uint32_t a) const { return static_cast<int8_t>(rdram[a & PS2_RAM_MASK]); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
    };

    // The size class of `bytes` (a multiple of 4) as 0x1addc0 computes it
    // (before the - 1): the small heap's table and steps, else one plus the
    // bits above 2 KB.
    inline int32_t allocClass(const Ram &ram, uint32_t heap, uint32_t t1)
    {
        const int32_t s = static_cast<int32_t>(t1);
        if (heap == kSmallHeap)
        {
            if (s < 0x401)
            {
                if (s < 0x81)
                    return ram.s8(kSmallClass + t1);
                if (s < 0xC1)
                    return 8;
                if (s < 0x101)
                    return 9;
                return s < 0x201 ? 0xA : 0xB;
            }
            const int32_t a0 = s < 0 ? s + 0x3FF : s;
            int32_t v1 = a0 >> 10;
            int32_t a1 = 0xC;
            if ((v1 & 0xFF00) != 0)
            {
                v1 = a0 >> 18;
                a1 = 0x14;
            }
            v1 >>= 1;
            while (v1 != 0)
            {
                v1 >>= 1;
                ++a1;
            }
            return a1;
        }
        const int32_t v0 = s < 0 ? s + 0x3FF : s;
        int32_t v1 = v0 >> 11;
        int32_t a1 = 1;
        while (v1 != 0)
        {
            v1 >>= 1;
            ++a1;
        }
        return a1;
    }

    // 0x1adbc0: block t0 into its class (the list kept by size, the new one
    // before the first larger one).
    // Returns v0 as the original leaves it (the last word it wrote).
    uint32_t insertFree(const Ram &ram, uint32_t t0, uint32_t classes, uint32_t count, uint32_t heap)
    {
        const uint32_t t1 = (ram.u32(t0) & kIdx) << 2;
        int32_t v0;
        if (heap == kSmallHeap)
        {
            const int32_t s = static_cast<int32_t>(t1);
            if (s < 0x401)
            {
                if (s < 0x81)
                    v0 = ram.s8(kSmallClass + t1) - 1;
                else if (s < 0xC1)
                    v0 = 7;
                else if (s < 0x101)
                    v0 = 8;
                else
                    v0 = (s < 0x201 ? 0xA : 0xB) - 1;
            }
            else
            {
                const int32_t a0 = s;
                int32_t v1 = a0 >> 10;
                if ((v1 & 0xFF00) != 0)
                {
                    v1 = a0 >> 18;
                    v0 = 0x14;
                }
                else
                    v0 = 0xC;
                v1 >>= 1;
                while (v1 != 0)
                {
                    v1 >>= 1;
                    ++v0;
                }
                v0 -= 1;
            }
        }
        else
        {
            uint32_t v1 = t1 >> 11; // srl
            int32_t n = 1;
            int32_t w = static_cast<int32_t>(v1);
            while (w != 0)
            {
                w >>= 1;
                ++n;
            }
            v0 = n - 1;
        }
        if (!(static_cast<uint32_t>(v0) < count))
            v0 = static_cast<int32_t>(count - 1u);
        const uint32_t bin = classes + (static_cast<uint32_t>(v0) << 3);
        uint32_t a2 = ram.u32(bin);
        if (a2 == 0u)
        {
            ram.put32(bin, t0);
            ram.put32(bin + 4u, t0);
            ram.put32(t0 + 0x1Cu, 0u);
            const uint32_t v = ram.u32(t0 + 0x14u) & kTop;
            ram.put32(t0 + 0x14u, v);
            return v;
        }
        const uint32_t words = static_cast<uint32_t>(sra(t1, 2));
        const uint32_t self = static_cast<uint32_t>(sra(t0, 2)) & kIdx;
        for (;;)
        {
            if (words < (ram.u32(a2) & kIdx))
            {
                // Before a2.
                const uint32_t prev = ram.u32(a2 + 0x14u) & kIdx;
                const uint32_t v1 = (ram.u32(t0 + 0x14u) & kTop) | prev;
                ram.put32(t0 + 0x14u, v1);
                if (prev != 0u)
                    ram.put32(((v1 & kIdx) << 2) + 0x1Cu, t0);
                else
                    ram.put32(bin, t0);
                ram.put32(t0 + 0x1Cu, a2);
                const uint32_t v = (ram.u32(a2 + 0x14u) & kTop) | self;
                ram.put32(a2 + 0x14u, v);
                return v;
            }
            const uint32_t next = ram.u32(a2 + 0x1Cu);
            if (next == 0u)
                break;
            a2 = next;
        }
        // At the end.
        const uint32_t v1 = static_cast<uint32_t>(sra(a2, 2));
        ram.put32(bin + 4u, t0);
        ram.put32(a2 + 0x1Cu, t0);
        ram.put32(t0 + 0x1Cu, 0u);
        const uint32_t v = (ram.u32(t0 + 0x14u) & kTop) | (v1 & kIdx);
        ram.put32(t0 + 0x14u, v);
        return v;
    }

    void insertFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kHeap120))
        {
            ctx->pc = kInsert;
            return sotc_decomp::recompiled(kInsert)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        SET_GPR_U64(ctx, 2, sx(insertFree(ram, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7))));
        sotc_decomp::returnTo(ctx);
    }

    void allocFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kHeap120))
        {
            ctx->pc = kAlloc;
            return sotc_decomp::recompiled(kAlloc)(rdram, ctx, runtime);
        }
        const uint32_t heap = GPR_U32(ctx, 4), owner = GPR_U32(ctx, 7);
        if (heap >= PS2_RAM_SIZE)
        {
            ctx->pc = kAlloc;
            return sotc_decomp::recompiled(kAlloc)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        uint32_t t1 = GPR_U32(ctx, 5);
        if (t1 < 4u)
            t1 = 4u;
        int32_t t4 = static_cast<int32_t>(GPR_U32(ctx, 6));
        if (t4 < 4)
            t4 = 4;
        const uint32_t classes = (ram.u32(heap) & kIdx) << 2;
        t1 = (t1 + 3u) & ~3u;
        const int32_t cls = allocClass(ram, heap, t1);
        const uint32_t count = ram.u16(heap + 4u);
        uint32_t index = static_cast<uint32_t>(cls - 1);
        const uint32_t words = static_cast<uint32_t>(sra(t1, 2));
        uint32_t result = 0u;
        if (index < count)
        {
            const uint32_t a2 = static_cast<uint32_t>(t4 - 1), mask = ~a2;
            uint32_t bin = classes + (index << 3);
            uint32_t s0 = 0u, s1 = 0u, s2 = 0u, t3 = 0u;
            bool found = false;
            for (; index < count && !found; ++index, bin += 8u)
            {
                for (s0 = ram.u32(bin); s0 != 0u; s0 = ram.u32(s0 + 0x1Cu))
                {
                    const uint32_t size = ram.u32(s0) & kIdx;
                    if (size < words)
                        continue;
                    t3 = s0 + 0x20u + (size << 2);
                    s2 = (s0 + 0x24u + a2) & mask;
                    s1 = s2 + t1;
                    if (!(t3 < s1))
                    {
                        ram.put32(s2 - 4u, s0);
                        found = true;
                        break;
                    }
                }
                if (found)
                    break;
            }
            if (found)
            {
                // Off its class's list.
                const uint32_t prev = (ram.u32(s0 + 0x14u) & kIdx) << 2;
                const uint32_t next = ram.u32(s0 + 0x1Cu);
                if (prev == 0u)
                    ram.put32(bin, next);
                else
                    ram.put32(prev + 0x1Cu, next);
                if (next != 0u)
                    ram.put32(next + 0x14u, (ram.u32(next + 0x14u) & kTop) | (ram.u32(s0 + 0x14u) & kIdx));
                else
                    ram.put32(bin + 4u, prev);
                // In use, its size in words wanted, the alignment's log.
                ram.put32(s0 + 0xCu, (ram.u32(s0 + 0xCu) | 0x01000000u) & 0xFDFFFFFFu);
                ram.put32(s0 + 4u, (ram.u32(s0 + 4u) & kTop) | (words & kIdx));
                uint32_t lg = 0u;
                if ((t4 >> 3) != 0)
                {
                    lg = 1u;
                    while ((t4 >> (lg + 3u)) != 0)
                        ++lg;
                }
                ram.put32(s0 + 0xCu, (ram.u32(s0 + 0xCu) & 0xE3FFFFFFu) | ((lg & 7u) << 26));
                if (s1 + 0x20u < t3)
                {
                    // The rest becomes a free block after the data.
                    const uint32_t self = static_cast<uint32_t>(sra(s0, 2)) & kIdx;
                    const uint32_t neu = static_cast<uint32_t>(sra(s1, 2)) & kIdx;
                    ram.put32(s1 + 0xCu, (ram.u32(s1 + 0xCu) & kTop) | self);
                    ram.put32(s1 + 8u, (ram.u32(s1 + 8u) & kTop) | (ram.u32(s0 + 8u) & kIdx));
                    ram.put32(s0 + 8u, (ram.u32(s0 + 8u) & kTop) | neu);
                    const uint32_t after = ram.u32(s1 + 8u) & kIdx;
                    if (after != 0u)
                    {
                        const uint32_t at = after << 2;
                        ram.put32(at + 0xCu, (ram.u32(at + 0xCu) & kTop) | neu);
                    }
                    const uint32_t flags = (static_cast<uint32_t>(ram.u8(s0 + 0xBu)) << 16) | (static_cast<uint32_t>(ram.u8(s0 + 7u)) << 8) | ram.u8(s0 + 3u);
                    const uint32_t restWords = static_cast<uint32_t>(sra(t3 - s1 - 0x20u, 2)) & kIdx;
                    const uint32_t c = ram.u32(s1 + 0xCu);
                    ram.put8(s1 + 3u, static_cast<uint8_t>(flags));
                    ram.put8(s1 + 0xBu, static_cast<uint8_t>(flags >> 16));
                    ram.put8(s1 + 7u, static_cast<uint8_t>(flags >> 8));
                    ram.put32(s1, (ram.u32(s1) & kTop) | restWords);
                    ram.put32(s1 + 0xCu, c & 0xFEFFFFFFu);
                    insertFree(ram, s1, classes, ram.u16(heap + 4u), heap);
                    const uint32_t used = static_cast<uint32_t>(sra(s1 - s0 - 0x20u, 2)) & kIdx;
                    ram.put32(s0, (ram.u32(s0) & kTop) | used);
                }
                if (heap == kSmallHeap)
                    ram.put32(kSmallFree, ram.u32(kSmallFree) - (ram.u32(s0) & kIdx));
                const uint32_t a0 = ram.u32(s0 + 0x18u);
                const uint32_t c = ram.u32(s0 + 0xCu);
                ram.put32(s0 + 0xCu, c & 0x9FFFFFFFu);
                ram.put32(s0 + 0x18u, (a0 & kTop) | (static_cast<uint32_t>(sra(owner, 2)) & kIdx));
                ram.put8(s0 + 0x13u, 0u);
                ram.put8(s0 + 0x1Bu, 0u);
                ram.put8(s0 + 0x17u, 0u);
                result = s2;
            }
        }
        SET_GPR_U64(ctx, 2, sx(result));
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x001ADDC0u, kernel_sub_001ADDC0, decomp_heap120::allocFn);
SOTC_DECOMP_FUNCTION(0x001ADBC0u, kernel_sub_001ADBC0, decomp_heap120::insertFn);
