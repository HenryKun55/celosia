#pragma once
// Round S14 (SOTC_S14HOOK; A/B name s14hook): the model hook's palette as a host record, not parsed out of the VIF chain.
//
// A skinned model's bone palette is written by dispSkinned (decomp_skinpal.cpp: object +0x160 x the camera into the
// palette block, model +0x104) and the instance chain's second packet carries it to VU1 with UNPACKs. Before S14 the
// model hook (modelDispHook's lean path) and the skinned shadow (shadowCapture) read it back from guest memory each draw:
// readChainPaletteLean walked the packet's VIF codes twice, then copied every qword with its bounds check, read note and
// prefetch (~0.6% of the game thread riding).
//
//   kPalette (1)  dispSkinned writes each bone's four rows into the chain's host record as well (rows lo..hi, the VU
//                 qword addresses the UNPACKs carry them to), the qwords of the packet that are not bone rows copied from
//                 the guest after the loop; the hook and the shadow take the record (one copy of host floats) when
//                 dispSkinned wrote it in this frame. The record's layout (each code word of the packet, where every
//                 UNPACK's data lands, which bone row that is) is learned from a read the old way the first time, and
//                 dispSkinned checks the code words, the tag, the packet's size, the palette block and the bone count
//                 against it before each write; anything else (not learned yet, changed, a layout with a bone row
//                 carried twice or only partly, the recompiled fallback paths) reads the chain as before.
//
// SOTC_S14HOOK_PARTS=<mask> (default all) picks parts; SOTC_S14HOOK_CHECK=1 reads the chain the old way at every take
// as well and compares the hook's / shadow's results (lo, hi, packet, size, the palette's floats) and uses the old
// way's; SOTC_S14HOOK_LOG=<frames> prints "[s14hook]" counts (default 600 with the check).
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include "sotc_decomp.h"

namespace sotc_native
{
    extern uint64_t g_frame;
}

namespace s14hook
{
    enum Part : uint32_t
    {
        kPalette = 1u,
        kAll = 1u
    };
    // Round F1: the switch (s14hook / SOTC_S14HOOK) and its knobs (PARTS / CHECK / LOG) deleted, class E: exact but even.
    // on() is a constant false, so every use below compiles out (the bodies are left for a cleanup round).
    constexpr bool g_check = false;
    constexpr uint32_t g_log = 0;
    constexpr bool on(uint32_t) { return false; }

    struct Stats
    {
        uint64_t written = 0, declined = 0, learned = 0, unusable = 0, taken = 0, shadowTaken = 0, missed = 0, checks = 0, differ = 0;
    };
    inline Stats g_st;
    inline uint64_t g_logFrame = 0;

    struct Code
    {
        uint32_t at, word;
    };
    struct Other
    {
        uint32_t row, from; // a qword of the record not from a bone row: copied from the guest (word-aligned, in main RAM)
    };
    // One instance chain (keyed by its first tag's address, the instance's +0xc / the skinned model's display object).
    struct Chain
    {
        uint32_t own = 0;
        bool wanted = false;  // dispSkinned wrote this chain's palette: the hook learns the layout at its next read
        bool layout = false;  // usable layout learned
        bool bad = false;     // the layout is not one the record can carry (until the palette block / bone count change)
        uint32_t ret = 0, qwc = 0, lo = 0, hi = 0, bytes = 0;
        uint32_t palette = 0, bones = 0; // the palette block and the bone count the layout was mapped for
        std::vector<Code> codes;
        std::vector<int32_t> boneRow;    // per bone: its first row in `rows` (qwords from lo), -1: not carried
        std::vector<Other> others;
        std::vector<float> rows;         // (hi - lo) * 4 floats: what readChainPaletteLean reads (zero where no UNPACK wrote)
        uint64_t frame = ~0ull;          // the display frame dispSkinned last wrote `rows` in (~0: not valid)
    };
    constexpr uint32_t kSlots = 512;
    inline Chain g_chains[kSlots];
    inline Chain *find(uint32_t own, bool insert)
    {
        if (!own)
            return nullptr;
        uint32_t i = (own * 0x9E3779B1u) >> 23;
        for (uint32_t n = 0; n < kSlots; ++n, i = (i + 1u) & (kSlots - 1u))
        {
            Chain &c = g_chains[i];
            if (c.own == own)
                return &c;
            if (c.own == 0u)
            {
                if (!insert)
                    return nullptr;
                c.own = own;
                return &c;
            }
        }
        return nullptr;
    }

    inline uint32_t ramWord(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, ram + (a & 0x1FFFFFCu), 4);
        return v;
    }

    // dispSkinned, every call (any path): the chain's record is not valid until the native loop writes it again.
    inline void invalidate(uint32_t own)
    {
        if (Chain *c = find(own, false))
            c->frame = ~0ull;
    }
    // dispSkinned's native loop is about to write `bones` bones to `palette` for chain `own`: the record's rows to write
    // them into (nullptr: no usable layout, the hook reads the chain as before; the chain is marked to be learned).
    inline Chain *begin(const uint8_t *ram, uint32_t own, uint32_t palette, uint32_t bones)
    {
        Chain *c = find(own, true);
        if (!c)
            return nullptr;
        c->frame = ~0ull;
        if (!c->layout)
        {
            if (c->palette != palette || c->bones != bones)
                c->bad = false;
            c->palette = palette, c->bones = bones; // what learn() maps the layout for
            c->wanted = !c->bad;
            ++g_st.declined;
            return nullptr;
        }
        bool ok = c->layout && c->palette == palette && c->bones == bones && ((ramWord(ram, own) >> 28) & 7u) == 2u &&
                  ramWord(ram, own + 4u) == c->ret && (ramWord(ram, c->ret) & 0xFFFFu) == c->qwc;
        for (uint32_t i = 0; ok && i < c->codes.size(); ++i)
            ok = ramWord(ram, c->codes[i].at) == c->codes[i].word;
        if (!ok)
        {
            c->layout = false;
            c->bad = false;
            c->palette = palette, c->bones = bones;
            c->wanted = true;
            ++g_st.declined;
            return nullptr;
        }
        return c;
    }
    // After the loop: the qwords that are not bone rows, then the record is this frame's.
    inline void finish(const uint8_t *ram, Chain *c)
    {
        float *const rows = c->rows.data();
        for (const Other &o : c->others)
            std::memcpy(rows + o.row * 4u, ram + o.from, 16);
        c->frame = sotc_native::g_frame;
        ++g_st.written;
    }
    // The hook / shadow: the record when dispSkinned wrote it this frame (nullptr: read the chain as before).
    inline const Chain *take(uint32_t own)
    {
        Chain *c = find(own, false);
        if (c && c->frame == sotc_native::g_frame)
            return c;
        return nullptr;
    }
    // After a read the old way of a chain dispSkinned writes: its layout (the same walk as readChainPaletteLean).
    inline void learn(const uint8_t *ram, uint32_t own)
    {
        Chain *c = find(own, false);
        if (!c || !c->wanted || c->layout)
            return;
        c->wanted = false;
        c->bad = true; // until it is learned below
        c->codes.clear();
        c->others.clear();
        c->boneRow.clear();
        c->frame = ~0ull;
        if (((ramWord(ram, own) >> 28) & 7u) != 2u)
            return;
        const uint32_t ret = ramWord(ram, own + 4u), qwc = ramWord(ram, ret) & 0xFFFFu;
        if (qwc <= 5u)
            return;
        // The palette block and bone count dispSkinned's begin() noted for this chain.
        const uint32_t palette = c->palette, bones = c->bones;
        const uint32_t end = ret + 16u + qwc * 16u;
        uint32_t lo = 1024u, hi = 0u;
        std::vector<uint32_t> src(1024u, 0u); // per VU qword: the guest address its value comes from (0: none)
        for (uint32_t p = ret + 8u; p < end;)
        {
            const uint32_t code = ramWord(ram, p);
            c->codes.push_back(Code{p, code});
            p += 4;
            const uint32_t cmd = (code >> 24) & 0x7Fu;
            if ((cmd & 0x6Fu) != 0x6Cu)
                continue;
            const uint32_t n = ((code >> 16) & 0xFFu) ? ((code >> 16) & 0xFFu) : 256u, at = code & 0x3FFu;
            const uint32_t last = n + at < 1024u ? at + n : 1024u;
            for (uint32_t q = at; q < last; ++q)
            {
                const uint32_t from = p + (q - at) * 16u;
                if ((from & 0x1FFFFFCu) + 16u > 0x2000000u)
                    return; // (outside main RAM: not kept)
                src[q] = from & 0x1FFFFFCu;
            }
            if (last > at)
            {
                lo = lo < at ? lo : at;
                hi = hi > last ? hi : last;
            }
            p += n * 16u;
        }
        if (hi <= lo)
            return;
        c->ret = ret;
        c->qwc = qwc;
        c->lo = lo;
        c->hi = hi;
        c->bytes = end - ret;
        c->rows.assign((hi - lo) * 4u, 0.0f);
        // Each bone's rows: carried to four consecutive VU qwords, once each, or not at all.
        std::vector<uint8_t> claimed(hi - lo, 0u);
        c->boneRow.assign(bones, -1);
        for (uint32_t i = 0; i < bones; ++i)
        {
            const uint32_t base = (palette + ((i * 4u + ((i + 1u) >> 6)) << 4)) & 0x1FFFFF0u;
            int32_t first = -1;
            uint32_t found = 0;
            for (uint32_t q = lo; q < hi; ++q)
                for (uint32_t k = 0; k < 4u; ++k)
                    if (src[q] == base + k * 16u)
                    {
                        ++found;
                        if (k == 0u)
                            first = static_cast<int32_t>(q - lo);
                    }
            if (found == 0u)
                continue;
            if (found != 4u || first < 0 || static_cast<uint32_t>(first) + 4u > hi - lo)
            {
                ++g_st.unusable;
                return;
            }
            for (uint32_t k = 0; k < 4u; ++k)
                if (src[lo + first + k] != base + k * 16u || claimed[first + k])
                {
                    ++g_st.unusable;
                    return;
                }
            for (uint32_t k = 0; k < 4u; ++k)
                claimed[first + k] = 1u;
            c->boneRow[i] = first;
        }
        for (uint32_t q = lo; q < hi; ++q)
            if (!claimed[q - lo] && src[q])
                c->others.push_back(Other{q - lo, src[q]});
        c->layout = true;
        c->bad = false;
        ++g_st.learned;
    }
    inline void noteFrame()
    {
        const uint64_t frame = sotc_native::g_frame;
        if (g_log && frame >= g_logFrame + g_log)
        {
            g_logFrame = frame;
            const Stats &s = g_st;
            std::fprintf(stderr, "[s14hook] frame %llu: palettes written %llu, declined %llu, layouts learned %llu (unusable %llu); taken: hook %llu, shadow %llu, palettes read the old way %llu; checked %llu, differ %llu\n",
                         (unsigned long long)frame, (unsigned long long)s.written, (unsigned long long)s.declined, (unsigned long long)s.learned,
                         (unsigned long long)s.unusable, (unsigned long long)s.taken, (unsigned long long)s.shadowTaken, (unsigned long long)s.missed,
                         (unsigned long long)s.checks, (unsigned long long)s.differ);
        }
    }
}
