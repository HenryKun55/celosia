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
// the function runs. A prefetch never changes what is read: wrong guesses
// cost a line fill and nothing else.
//
// Since round 7 the two functions are native (decomp_walk.cpp), which asks
// for the lines here (aheadClip / aheadDisp) before its own reads;
// SOTC_OBJ_PREFETCH=0 leaves the prefetch out.
#include "sotc_decomp.h"
#include "lines67.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_stats.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace decomp_prefetch
{
    struct Ram
    {
        const uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            lines67::touch(a);
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~3u), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            lines67::touch(a);
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~7u), 8);
            return v;
        }
    };

    // GetLayoutClass (decomp_layout.cpp) for the cases it answers natively; 0 otherwise.
    uint32_t layoutClass(const Ram m, uint32_t object)
    {
        lines67::touch(object + 8u);
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
        uint32_t lines[28];
        void add(uint32_t addr)
        {
            if (count < 28u && addr)
                lines[count++] = addr & ~31u;
        }
    };

    // The common part: the object, its layout class, the instance, the
    // model header and its LOD record (+8), the matrix (+0x15c).
    void objectLines(const Ram m, uint32_t object, Record &r)
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
        size_t asked = 0; // round 67: last[] records before this one already asked for this frame
        uint64_t frame = ~0ull;
        uint64_t walkFrame = ~0ull; // round 67: the frame whose window the walk itself moves (aheadWalk)

        // The index in the last frame's calls of the record after
        // `object`'s, or npos when `object` is not found near the cursor.
        size_t locate(uint64_t f, uint32_t object)
        {
            if (f != frame)
            {
                frame = f;
                last.swap(now);
                now.clear();
                cursor = 0;
                asked = 0;
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
                    return ~size_t(0);
            }
            cursor = at + 1u;
            return cursor;
        }
        // The last frame's record after `object`'s, or nullptr.
        const Record *next(uint64_t f, uint32_t object)
        {
            const size_t at = locate(f, object);
            return at < last.size() ? &last[at] : nullptr;
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
        {
            lines67::pref(r.lines[i]);
            __builtin_prefetch(rdram + (r.lines[i] & PS2_RAM_MASK));
        }
    }

    // Round 67 (SOTC_WALK67): how many of the last frame's next calls are
    // asked for ahead (SOTC_WALK67_AHEAD, 3 by default): each call asks
    // only for the one that comes into the window (the first call of a
    // frame, or after a resync, for the whole window), so a call's lines
    // have ~ahead calls of time to arrive instead of one.
    size_t ahead67()
    {
        static const size_t n = [] {
            const char *v = std::getenv("SOTC_WALK67_AHEAD");
            const long a = v ? std::atol(v) : 3;
            return static_cast<size_t>(a < 1 ? 1 : a > 16 ? 16 : a);
        }();
        return n;
    }

    // Round 67: the lines the walk itself reads around the call that
    // objectLines leaves out (DispDispObjGroupModel's pass flags at
    // +0x1b6..+0x1b8, the draw's +0x280 / +0x3c0, the sort node: the
    // geometry class's own first line, its +4 key and +8 link), the
    // record's duplicates dropped (each PLD of
    // a line already asked for costs an issue slot and a fill buffer).
    void objectLines67(const Ram m, uint32_t object, Record &r)
    {
        objectLines(m, object, r);
        if (r.count == 0u)
            return;
        r.add(object + 0x1B6u);
        // (DispLayoutObjModel's reads at +0x280 / +0x3c0, the lines the Mac's
        // proxy found most often missed in the draw pass.)
        r.add(object + 0x280u);
        r.add(object + 0x3C0u);
        const uint32_t lc = layoutClass(m, object);
        if (lc)
            r.add(m.u32(lc + 0x140u));
        uint32_t n = 0;
        for (uint32_t i = 0; i < r.count; ++i)
        {
            bool dup = false;
            for (uint32_t j = 0; j < n && !dup; ++j)
                dup = r.lines[j] == r.lines[i];
            if (!dup)
                r.lines[n++] = r.lines[i];
        }
        r.count = n;
    }

    // The window of the last frame's next calls after `object`'s, asked for
    // up to ahead67() calls on (those already asked for skipped).
    void window(Sequence &seq, uint64_t f, uint32_t object, const uint8_t *rdram)
    {
        const size_t at = seq.locate(f, object);
        if (at == ~size_t(0))
            return;
        const size_t end = std::min(seq.last.size(), at + ahead67());
        for (size_t i = std::max(at, seq.asked); i < end; ++i)
            ask(rdram, seq.last[i]);
        seq.asked = std::max(seq.asked, end);
    }

    void note(Sequence &seq, uint32_t object, const uint8_t *rdram)
    {
        if (!enabled())
            return;
        const Ram m{rdram};
        const uint64_t f = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        const bool w67 = sotc_decomp::walk67On();
        if (w67)
        {
            if (seq.walkFrame != f)
                window(seq, f, object, rdram);
            else if (f != seq.frame)
                seq.locate(f, object); // (a new frame: the lists turned over)
        }
        else if (const Record *next = seq.next(f, object))
            ask(rdram, *next);
        if (seq.now.size() < 4096u)
        {
            seq.now.emplace_back();
            if (w67)
                objectLines67(m, object, seq.now.back());
            else
                objectLines(m, object, seq.now.back());
        }
    }

    // Called by the native walk (decomp_walk.cpp) at the start of its
    // ClipLayoutObjModel / DispLayoutObjModel.
    void aheadClip(const uint8_t *rdram, uint32_t object) { note(g_clip, object, rdram); }
    void aheadDisp(const uint8_t *rdram, uint32_t object) { note(g_disp, object, rdram); }
    // Round 67: DispDispObjGroupModel's first pass moves the clip window at
    // the top of each object's turn (its own reads of the object come
    // before the clip callback's), and the draw pass the draw window; the
    // callbacks then only record their object.
    void aheadWalkClip(const uint8_t *rdram, uint32_t object)
    {
        if (!enabled() || !sotc_decomp::walk67On())
            return;
        const uint64_t f = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        window(g_clip, f, object, rdram);
        g_clip.walkFrame = f;
    }
    void aheadWalkDisp(const uint8_t *rdram, uint32_t object)
    {
        if (!enabled() || !sotc_decomp::walk67On())
            return;
        const uint64_t f = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        window(g_disp, f, object, rdram);
        g_disp.walkFrame = f;
    }
}
