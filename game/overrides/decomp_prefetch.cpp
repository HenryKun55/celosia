// The display walk's per-object calls, with the next object's guest lines
// asked for while the current one runs (the Vita's Cortex-A9: no useful
// hardware prefetch, ~250 ns a DRAM miss).
//
// DispDispObjGroupModel calls ClipLayoutObjModel (0x10f4890, through the
// layout class's +0xdc) for every object of the display group, ~200 a frame
// in a heavy scene, then DispLayoutObjModel (0x10f5920, +0xd0) for every one
// it drew. Each reads its object, the layout class, the model instance, its
// header and LOD record, the matrix and the part table's root, one line
// after the other, each the first time that frame: on the console
// modelClipSolid took ~12 us a call (0.17 us on the Mac). The objects come in
// the same order every frame (the group, the sort list), so the lines the
// last frame's next call read (kept by address) are asked for (PLD) before
// the recompiled function runs; the function itself is the recompiled one,
// unchanged. A prefetch never changes what is read: wrong guesses cost a
// line fill and nothing else.
//
// Both are in SOTC_AB's new set; SOTC_OBJ_PREFETCH=0 leaves them out.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_stats.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace decomp_prefetch
{
    constexpr uint32_t kClip = 0x010F4890u, kDisp = 0x010F5920u;

    struct Ram
    {
        const uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~3u), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~7u), 8);
            return v;
        }
    };

    // GetLayoutClass (decomp_layout.cpp) for the cases it answers natively; 0 otherwise.
    uint32_t layoutClass(const Ram &m, uint32_t object)
    {
        const uint32_t index = static_cast<uint32_t>(m.rdram[(object + 8u) & PS2_RAM_MASK]) - 9u;
        if (index >= 0xEu)
            return 0u;
        const uint32_t target = m.u32(0x01194070u + index * 4u);
        if (target == 0x010F1A58u)
            return object + 0x20u;
        if (target == 0x010F1A98u)
            return object + 0x90u;
        return 0u;
    }

    // The lines one call reads (as the last frame's call for that object read them).
    struct Record
    {
        uint32_t object = 0;
        uint32_t count = 0;
        uint32_t lines[24];
        void add(uint32_t addr)
        {
            if (count < 24u && addr)
                lines[count++] = addr & ~31u;
        }
    };

    // The common part: the object, its layout class, the instance, the
    // model header and its LOD record (+8), the matrix (+0x15c).
    void objectLines(const Ram &m, uint32_t object, Record &r)
    {
        r.object = object;
        r.count = 0;
        r.add(object);
        const uint32_t lc = layoutClass(m, object);
        if (!lc)
            return;
        r.add(lc + 0x9Cu);
        r.add(lc + 0x140u);
        r.add(lc + 0x160u);
        r.add(lc + 0x180u);
        const uint32_t geo = m.u32(lc + 0x140u), instance = m.u32(lc + 0x144u), matrix = m.u32(lc + 0x15Cu);
        r.add(geo + 0x20u);
        r.add(geo + 0xC0u);
        r.add(geo + 0xE0u);
        if (!instance)
            return;
        r.add(matrix);
        r.add(matrix + 0x20u);
        r.add(instance);
        r.add(instance + 0x220u);
        r.add(instance + 0x240u);
        r.add(instance + 0x260u);
        const uint32_t model = m.u32(instance);
        r.add(model + 0x20u);
        const uint32_t header = m.u32(model + 0x20u);
        if (!header)
            return;
        r.add(header);
        r.add(header + 0x20u);
        r.add(header + 0x40u);
        r.add(header + 0x60u);
        const uint32_t lodRecord = m.u32(header + 8u);
        r.add(lodRecord);
        r.add(lodRecord + 0x20u);
        // The part table's root for the instance's LOD (bits 40-42 of +0x250).
        const uint32_t lod = static_cast<uint32_t>((m.u64(instance + 0x250u) >> 40) & 7u);
        const uint32_t table = m.u32(header + 0x30u + (lod & 3u) * 4u);
        r.add(table);
        r.add(table + 0x20u);
    }

    // This frame's calls and the last frame's, in order.
    struct Sequence
    {
        std::vector<Record> now, last;
        size_t cursor = 0;
        uint64_t frame = ~0ull;

        // The last frame's record after `object`'s, or nullptr.
        const Record *next(uint64_t f, uint32_t object)
        {
            if (f != frame)
            {
                frame = f;
                last.swap(now);
                now.clear();
                cursor = 0;
            }
            size_t at = cursor;
            if (at >= last.size() || last[at].object != object)
            {
                at = ~size_t(0);
                for (size_t i = cursor, end = std::min(last.size(), cursor + 8u); i < end; ++i)
                    if (last[i].object == object)
                    {
                        at = i;
                        break;
                    }
                if (at == ~size_t(0))
                    return nullptr;
            }
            cursor = at + 1u;
            return cursor < last.size() ? &last[cursor] : nullptr;
        }
    };
    Sequence g_clip, g_disp;

    bool enabled()
    {
        static const bool on = [] {
            const char *v = std::getenv("SOTC_OBJ_PREFETCH");
            return !v || std::atoi(v) != 0;
        }();
        return on;
    }

    inline void ask(const uint8_t *rdram, const Record &r)
    {
        for (uint32_t i = 0; i < r.count; ++i)
            __builtin_prefetch(rdram + (r.lines[i] & PS2_RAM_MASK));
    }

    void run(Sequence &seq, uint32_t address, uint32_t object, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (enabled())
        {
            const Ram m{rdram};
            if (const Record *next = seq.next(ps2_stats::vif1Chains.load(std::memory_order_relaxed), object))
                ask(rdram, *next);
            if (seq.now.size() < 4096u)
            {
                seq.now.emplace_back();
                objectLines(m, object, seq.now.back());
            }
        }
        return sotc_decomp::recompiled(address)(rdram, ctx, runtime);
    }

    // ClipLayoutObjModel(object)
    void clip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { run(g_clip, kClip, GPR_U32(ctx, 4), rdram, ctx, runtime); }
    // DispLayoutObjModel(camera, object)
    void disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { run(g_disp, kDisp, GPR_U32(ctx, 5), rdram, ctx, runtime); }
}

SOTC_DECOMP_AB_FUNCTION(0x010F4890u, ClipLayoutObjModel, decomp_prefetch::clip);
SOTC_DECOMP_AB_FUNCTION(0x010F5920u, DispLayoutObjModel, decomp_prefetch::disp);
