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
#include "r1sort.h"
#include "d1_native.h"

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

// ---- Round R1: AddSortObj as host code (r1sort.h; docs/native_r1.md) ------------------------------------------
// The same reads, the same order of compares (the EE FPU's compare / multiply / convert helpers), the same stores as
// addSortObj above; the list and nodes through typed host pointers, nothing in the context. Rec: the stores recorded
// for SOTC_R1SORT_CHECK instead of applied.
namespace decomp_dl
{
    namespace
    {
        inline bool okNode(uint32_t a) { return Ram::ok(a, 0x10u) && (a & 3u) == 0u; }
        template <bool Rec>
        struct Out
        {
            uint8_t *rdram;
            SortWrites *w;
            inline void put(uint32_t a, uint32_t v) const
            {
                if (Rec)
                {
                    if (w->n < SortWrites::kMax)
                    {
                        w->addr[w->n] = a;
                        w->val[w->n] = v;
                        ++w->n;
                    }
                }
                else
                    *reinterpret_cast<uint32_t *>(rdram + a) = v;
            }
            inline void watch(uint32_t a) const
            {
                if (Rec && w->nw < SortWrites::kWatch)
                    w->watch[w->nw++] = a;
            }
        };

        template <bool Rec>
        inline bool addSortT(uint8_t *rdram, uint32_t list, uint32_t obj, SortWrites *w)
        {
            if (!okNode(list) || !okNode(obj))
                return false;
            const Out<Rec> out{rdram, w};
            const SortList &L = *reinterpret_cast<const SortList *>(rdram + list);
            const float key = reinterpret_cast<const SortNode *>(rdram + obj)->key;
            const int32_t count = L.count;
            const float product = FPU_MUL_S(key, L.scale);
            const int32_t word = FPU_CVT_W_S(product);
            int32_t index = word < count ? (word < 0 ? 0 : word) : count - 1;
            const int32_t mode = L.mode;
            if (mode == 1)
                index = count - index - 1;
            const uint32_t slot = L.buckets + (static_cast<uint32_t>(index) << 2);
            if (!Ram::ok(slot, 4u) || (slot & 3u))
                return false;
            const uint32_t head = *reinterpret_cast<const uint32_t *>(rdram + slot);
            out.watch(slot);
            out.watch(obj + 8u);
            out.watch(obj + 0xCu);
            if (head == 0u)
            {
                out.put(slot, obj);
                out.put(obj + 8u, 0u);
                out.put(obj + 0xCu, 0u);
                return true;
            }
            uint32_t node = head, next = 0u;
            bool toPrev = false;
            for (uint32_t steps = 0;; ++steps)
            {
                if (!okNode(node) || steps > (1u << 20))
                    return false;
                const SortNode &N = *reinterpret_cast<const SortNode *>(rdram + node);
                out.watch(node + 8u);
                out.watch(node + 0xCu);
                const bool goPrev = mode == 0 ? FPU_C_OLT_S(N.key, key) : mode == 1 ? FPU_C_OLT_S(key, N.key) : false;
                if (goPrev)
                {
                    next = N.toward;
                    if (next != 0u)
                    {
                        node = next;
                        continue;
                    }
                    toPrev = true;
                }
                else
                    next = N.back;
                break;
            }
            if (toPrev)
            {
                out.put(obj + 8u, 0u);
                out.put(obj + 0xCu, node);
                out.put(node + 8u, obj);
                return true;
            }
            if (next == 0u)
            {
                out.put(slot, obj);
                out.put(obj + 0xCu, 0u);
                out.put(obj + 8u, node);
                out.put(node + 0xCu, obj);
                return true;
            }
            if (!Ram::ok(next, 0x10u))
                return false;
            out.watch(next + 8u);
            out.put(next + 8u, obj);
            // The original reads node+0xc after that store (the same word only if next + 8 == node + 0xc).
            const uint32_t v0 = (next + 8u == node + 0xCu) ? obj : reinterpret_cast<const SortNode *>(rdram + node)->back;
            out.put(obj + 8u, node);
            out.put(obj + 0xCu, v0);
            out.put(node + 0xCu, obj);
            return true;
        }
    }

    bool addSortHost(uint8_t *rdram, uint32_t list, uint32_t obj) { return addSortT<false>(rdram, list, obj, nullptr); }
    bool addSortRecord(uint8_t *rdram, uint32_t list, uint32_t obj, SortWrites &w) { return addSortT<true>(rdram, list, obj, &w); }
}

// Round 42 (SOTC_LEAVES=0: recompiled).
// iosDlGetLayer (0x1c0198): the current layer (0x2132d8 + 0x1c), v1 0x2132d8.
// transMicroCode (0x1114b50, a0 a VU1 microcode id): a DMA tag
// (iosDlChainTailCurrent, a0 0x1195480) in the current layer, then, when the
// layer's last microcode (0x1216d68 + 4 x layer) is another one, the id's
// upload tag (iosDlChainTailCurrent of *(0x118cd90 + 4 x id)) and the id
// noted. iosDlChainTailCurrent's native called directly while the table
// holds it (its jal sites are relocated: checked once).
namespace decomp_dl
{
    void getLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(0x001C0198u)(rdram, ctx, runtime);
        const Ram m{rdram};
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(kDl));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(m.u32(kDl + 0x1Cu)));
        returnTo(ctx);
    }

    void transMicroCode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool sitesOk = sotc_decomp::jalTarget(rdram, 0x01114B60u) == 0x001C0198u && sotc_decomp::jalTarget(rdram, 0x01114B80u) == kChainTail &&
                                    sotc_decomp::jalTarget(rdram, 0x01114BA0u) == kChainTail;
        if (!sotc_decomp::leavesOn() || !sitesOk)
            return sotc_decomp::recompiled(0x01114B50u)(rdram, ctx, runtime);
        const Ram m{rdram};
        const uint32_t id = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x20u;
        // iosDlGetLayer
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(kDl));
        const uint32_t layer = m.u32(kDl + 0x1Cu);
        const uint32_t last = 0x01216D68u + (layer << 2);
        SET_GPR_S32(ctx, 4, 0x01195480);
        sotc_decomp::callNative(rdram, ctx, runtime, kChainTail, &chainTail, 0x01114B88u, sp);
        const uint32_t current = m.u32(last);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(id << 2));
        if (current == id)
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(current));
        else
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(m.u32(0x0118CD90u + (id << 2))));
            sotc_decomp::callNative(rdram, ctx, runtime, kChainTail, &chainTail, 0x01114BA8u, sp);
            m.put32(last, id);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}
SOTC_DECOMP_FUNCTION(0x001C0198u, iosDlGetLayer, decomp_dl::getLayer);
namespace decomp_dl
{
    void transMicroCodeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
}
SOTC_DECOMP_FUNCTION(0x01114B50u, transMicroCode, decomp_dl::transMicroCodeSw);
D1_ALIAS(0x01114B50u, transMicroCode, decomp_dl::transMicroCodeSw, decomp_dl::transMicroCode);

namespace decomp_dl
{
    void addSortObjSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void chainTailSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_AB_FUNCTION(0x0110DC20u, AddSortObj, decomp_dl::addSortObjSw);
D1_ALIAS(0x0110DC20u, AddSortObj, decomp_dl::addSortObjSw, decomp_dl::addSortObj);
SOTC_DECOMP_AB_FUNCTION(0x001BFE88u, iosDlChainTailCurrent, decomp_dl::chainTailSw);
D1_ALIAS(0x001BFE88u, iosDlChainTailCurrent, decomp_dl::chainTailSw, decomp_dl::chainTail);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): AddSortObj and iosDlChainTailCurrent as typed cores -------------------------
// AddSortObj: the old body's reads, checks (before any store), compares and stores on d1::Mem; FPU_MUL_S -> d1::mul,
// FPU_C_OLT_S -> d1::lt (the keys are finite game data), the conversion through the same ps2FpuConvertToWord; f0 / f1 /
// f2, the condition bit and the GPRs the old body leaves written by the adapter from the core's result.
// iosDlChainTailCurrent: the packet's stores, the two re-reads of the layer's tail and the tail's link on d1::Mem in the
// old order; a layout where a store before a re-read could reach the word re-read (or an address outside main RAM) goes
// to the old body before anything is written, so every re-read is in the range checked at the entry.
// (transMicroCode calls iosDlChainTailCurrent through callNative(&chainTail): with d1native on the slot holds the
// dispatcher, so that call takes callNative's table path: correct, a little slower.)
namespace decomp_dl
{
    D1_COUNTER(c_d2AddSortObj, "AddSortObj");
    D1_COUNTER(c_d2ChainTail, "iosDlChainTailCurrent");

    inline bool d2Ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }
    inline bool d2Overlap(uint32_t a, uint32_t an, uint32_t b, uint32_t bn) { return a < b + bn && b < a + an; }

    struct SortOut
    {
        enum Exit
        {
            kEmpty, // the bucket was empty
            kPrev,  // dd14: first in the +8 direction
            kHead,  // dc90: the bucket's new head
            kMid    // dcc0: between node and its +0xc neighbour
        } exit;
        float key, f0;
        int32_t word, mode;
        bool cond;
        uint32_t offset, buckets, slot, node, next, v0;
    };

    // False: an address the old body sends to the recompiled code (nothing written).
    bool addSortCore(const d1::Mem m, uint32_t list, uint32_t obj, bool condIn, SortOut &o)
    {
        if (!d2Ok(list, 0x10u) || !d2Ok(obj, 0x10u) || (list & 3u) || (obj & 3u))
            return false;
        const float key = m.f32(obj + 4u);
        const int32_t count = m.s32(list + 4u);
        const float product = d1::mul(key, m.f32(list + 8u));
        const int32_t word = ps2FpuConvertToWord(product);
        int32_t index;
        if (word < count)
            index = word < 0 ? 0 : word;
        else
            index = count - 1;
        const int32_t mode = m.s32(list + 0xCu);
        if (mode == 1)
            index = count - index - 1;
        const uint32_t offset = static_cast<uint32_t>(index) << 2;
        const uint32_t buckets = m.u32(list);
        const uint32_t slot = offset + buckets;
        if (!d2Ok(slot, 4u) || (slot & 3u))
            return false;
        o.key = key;
        o.word = word;
        o.mode = mode;
        o.offset = offset;
        o.buckets = buckets;
        o.slot = slot;
        const uint32_t head = m.u32(slot);
        float f0 = product;
        bool cond = condIn;
        if (head == 0u)
        {
            m.set<uint32_t>(slot, obj);
            m.set<uint32_t>(obj + 8u, 0u);
            m.set<uint32_t>(obj + 0xCu, 0u);
            o.exit = SortOut::kEmpty;
            o.f0 = f0;
            o.cond = cond;
            return true;
        }
        uint32_t node = head, next = 0u;
        bool toPrev = false;
        for (uint32_t steps = 0;; ++steps)
        {
            if (!d2Ok(node, 0x10u) || (node & 3u) || steps > (1u << 20))
                return false;
            bool goPrev = false;
            if (mode == 0)
            {
                f0 = m.f32(node + 4u);
                cond = d1::lt(f0, key);
                goPrev = cond;
            }
            if (!goPrev && mode == 1)
            {
                f0 = m.f32(node + 4u);
                cond = d1::lt(key, f0);
                goPrev = cond;
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
        o.f0 = f0;
        o.cond = cond;
        o.node = node;
        o.next = next;
        if (toPrev)
        {
            m.set<uint32_t>(obj + 8u, 0u);
            m.set<uint32_t>(obj + 0xCu, node);
            m.set<uint32_t>(node + 8u, obj);
            o.exit = SortOut::kPrev;
            return true;
        }
        if (next == 0u)
        {
            m.set<uint32_t>(slot, obj);
            m.set<uint32_t>(obj + 0xCu, 0u);
            m.set<uint32_t>(obj + 8u, node);
            m.set<uint32_t>(node + 0xCu, obj);
            o.exit = SortOut::kHead;
            return true;
        }
        if (!d2Ok(next, 0x10u))
            return false; // nothing written yet
        m.set<uint32_t>(next + 8u, obj);
        const uint32_t v0 = m.u32(node + 0xCu); // after that store, as the old body reads it
        m.set<uint32_t>(obj + 8u, node);
        m.set<uint32_t>(obj + 0xCu, v0);
        m.set<uint32_t>(node + 0xCu, obj);
        o.v0 = v0;
        o.exit = SortOut::kMid;
        return true;
    }

    void addSortObjD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d2AddSortObj.n;
        SortOut o;
        if (!addSortCore(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), (ctx->fcr31 & 0x800000u) != 0u, o))
            return addSortObj(rdram, ctx, runtime);
        ctx->f[1] = o.key;
        std::memcpy(&ctx->f[2], &o.word, 4);
        ctx->f[0] = o.f0;
        if (o.exit == SortOut::kEmpty)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.offset));
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(o.buckets));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(o.slot));
            SET_GPR_S32(ctx, 8, o.mode);
            return returnTo(ctx);
        }
        setCond(ctx, o.cond);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(o.buckets));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(o.node));
        SET_GPR_S32(ctx, 7, o.mode);
        SET_GPR_S32(ctx, 8, o.mode);
        SET_GPR_U64(ctx, 9, 1u);
        switch (o.exit)
        {
        case SortOut::kPrev:
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.offset));
            break;
        case SortOut::kHead:
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.slot));
            break;
        default:
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(o.next));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.v0));
            break;
        }
        returnTo(ctx);
    }

    struct ChainOut
    {
        uint64_t v0, addr;
        uint32_t tail, packet, record;
    };

    // tagHi: a0's low word (the DMA tag's upper word). False: outside main RAM, misaligned, or a store that could reach
    // a word read after it (nothing written).
    bool chainTailCore(const d1::Mem m, uint32_t tagHi, ChainOut &o)
    {
        const uint32_t layer = m.u32(kDl + 0x1Cu), buf = m.u32(kDl + 0x18u);
        const uint32_t record = m.u32(kDl + 8u + (buf << 2)) + layer * 48u;
        const uint32_t packet = m.u32(kDl + 0x10u);
        if (!d2Ok(record, 0x28u) || !d2Ok(packet, 0x20u) || (packet & 7u))
            return false;
        const uint32_t tail0 = m.u32(record + 0x24u);
        if (!d2Ok(tail0, 0x18u) || (tail0 & 7u))
            return false;
        // The layer's tail word (re-read twice) written by none of the stores before those reads.
        if (d2Overlap(record + 0x24u, 4u, kDl + 0x10u, 4u) || d2Overlap(record + 0x24u, 4u, packet, 0x20u))
            return false;
        m.set<uint32_t>(kDl + 0x10u, packet + 0x20u);
        __builtin_prefetch(m.at<uint8_t>(packet + 0x40u));
        const uint64_t a0 = (static_cast<uint64_t>(tagHi) << 32) | 0x50000000u;
        m.set<uint32_t>(packet + 0x1Cu, 0u);
        m.set<uint64_t>(packet, a0);
        m.set<uint32_t>(packet + 0x18u, 0u);
        m.set<uint32_t>(packet + 0xCu, 0u);
        m.set<uint32_t>(packet + 8u, 0u);
        const uint64_t addr = static_cast<uint64_t>(packet & 0x7FFFFFFFu) << 32;
        const uint32_t first = m.u32(record + 0x24u); // tail0 (not written above)
        const uint64_t link = (static_cast<uint64_t>(m.u32(first + 0x14u) & 0x7FFFFFFFu) << 32) | 0x20000000u;
        m.set<uint64_t>(packet + 0x10u, link);
        const uint32_t tail = m.u32(record + 0x24u); // tail0 again
        const uint64_t v0 = (m.get<uint64_t>(tail + 0x10u) & 0x80000000FFFFFFFFull) | addr;
        m.set<uint64_t>(tail + 0x10u, v0);
        m.set<uint32_t>(record + 0x24u, packet);
        o = {v0, addr, tail, packet, record};
        return true;
    }

    void chainTailD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d2ChainTail.n;
        ChainOut o;
        if (!chainTailCore(d1::Mem{rdram}, GPR_U32(ctx, 4), o))
            return chainTail(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, o.v0);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(o.tail));
        SET_GPR_U64(ctx, 4, o.addr);
        SET_GPR_U64(ctx, 5, 0x7FFFFFFFu);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(o.packet));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(o.record));
        SET_GPR_U64(ctx, 8, 0x20000000u);
        SET_GPR_U64(ctx, 9, 0x80000000FFFFFFFFull);
        returnTo(ctx);
    }

    void addSortObjSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            addSortObjD1(rdram, ctx, runtime);
        else
            addSortObj(rdram, ctx, runtime);
    }

    void chainTailSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            chainTailD1(rdram, ctx, runtime);
        else
            chainTail(rdram, ctx, runtime);
    }
}

// ---- Round D3 (d1native, SOTC_D1NATIVE=1): transMicroCode as a typed core -------------------------------------------------
// The layer (iosDlGetLayer inline), the DMA tag chained (iosDlChainTailCurrent of 0x1195480) and, when the layer's last
// microcode is another one, the id's upload tag chained and the id noted: D2's chainTailCore called directly (calls direct
// 2, while the table holds iosDlChainTailCurrent's dispatcher), integer only, 0 math. The registers the old body leaves
// (the last chain's v0..t1, v1 = id x 4 / v0 = the id when nothing was uploaded) written by the adapter. A first chain the
// core cannot do (outside main RAM ...: nothing written) -> the old body; a second one -> that call through the table as
// the old body makes it, then the note.
namespace decomp_dl
{
    D1_COUNTER(c_d3TransMicro, "transMicroCode");

    struct D3MicroOut
    {
        ChainOut first, second;
        uint32_t current, upload, last;
    };

    // 0: the first chain not done (nothing written); 1: done, nothing uploaded; 2: done with the upload; 3: the upload's
    // chain not done (the first one done; the caller chains `upload` and notes the id).
    int d3MicroCore(const d1::Mem m, uint32_t id, D3MicroOut &o)
    {
        const uint32_t layer = m.u32(kDl + 0x1Cu);
        o.last = 0x01216D68u + (layer << 2);
        if (!chainTailCore(m, 0x01195480u, o.first))
            return 0;
        o.current = m.u32(o.last);
        if (o.current == id)
            return 1;
        o.upload = m.u32(0x0118CD90u + (id << 2));
        if (!chainTailCore(m, o.upload, o.second))
            return 3;
        m.set<uint32_t>(o.last, id);
        return 2;
    }

    // The registers iosDlChainTailCurrent's old body leaves (chainTailD1's exit).
    inline void d3MicroChainRegs(R5900Context *ctx, const ChainOut &c)
    {
        SET_GPR_U64(ctx, 2, c.v0);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(c.tail));
        SET_GPR_U64(ctx, 4, c.addr);
        SET_GPR_U64(ctx, 5, 0x7FFFFFFFu);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(c.packet));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(c.record));
        SET_GPR_U64(ctx, 8, 0x20000000u);
        SET_GPR_U64(ctx, 9, 0x80000000FFFFFFFFull);
    }

    void transMicroCodeD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool sitesOk = sotc_decomp::jalTarget(rdram, 0x01114B60u) == 0x001C0198u && sotc_decomp::jalTarget(rdram, 0x01114B80u) == kChainTail &&
                                    sotc_decomp::jalTarget(rdram, 0x01114BA0u) == kChainTail;
        const uint32_t id = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x20u;
        if (!sotc_decomp::leavesOn() || !sitesOk || sotc_decomp::guestFunction(runtime, kChainTail) != &chainTailSw || id > 0x00FFFFFFu)
            return transMicroCode(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        D3MicroOut o;
        const int r = d3MicroCore(m, id, o);
        if (r == 0)
            return transMicroCode(rdram, ctx, runtime);
        ++c_d3TransMicro.n;
        d3MicroChainRegs(ctx, o.first);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(id << 2));
        if (r == 1)
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.current));
        else if (r == 2)
            d3MicroChainRegs(ctx, o.second);
        else
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(o.upload));
            sotc_decomp::callNative(rdram, ctx, runtime, kChainTail, &chainTail, 0x01114BA8u, sp);
            m.set<uint32_t>(o.last, id);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void transMicroCodeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            transMicroCodeD3(rdram, ctx, runtime);
        else
            transMicroCode(rdram, ctx, runtime);
    }
}
