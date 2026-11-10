// Round 141 (SOTC_PF141, A/B `pf141`; decomp_pf141.cpp): the characters' memory asked for ahead on a helper thread.
//
// The console's character work (7.1 ms of the game thread riding, round 135's profile) is the pose, the cloths, the
// motion keys and the class / record walks of a few characters, each touching tens of KB of guest RAM and host
// arrays once a frame: cold lines on the A9 (DRAM ~250 ns a miss, round 66), which Vita3K does not model. The
// characters' natives note the ranges they read (`note`, game thread only); the ranges are kept by script-pass
// segment (a segment ends at each character's ExecCharaGeometry, the last one runs to the next pass) and, one frame
// later, a helper thread on cores 1-2 asks for the next segment's lines (PLD) while the game thread runs the current
// one: they are in the shared 2 MB L2 when the game thread gets there. Prefetch only: nothing is written, nothing the
// game reads or decides changes (the helper reads the noted ranges' addresses, not their contents).
#pragma once

#include <cstdint>

namespace pf141
{
    struct Range
    {
        const uint8_t *p;
        uint32_t n;
    };
    constexpr int kSegs = 96;
    constexpr uint32_t kRanges = 32768;
    struct Set
    {
        Range r[kRanges];
        uint32_t start[kSegs + 2];
        uint32_t obj[kSegs + 1];
        uint32_t count;
        int segs; // segments closed (the open one is segs)
        uint64_t frame;
    };
    // The set being recorded (nullptr: not recording: pf141 off or no pass yet).
    inline Set *g_cur = nullptr;
    inline const uint8_t *g_lastP = nullptr;

    // A range the game thread's character work reads (host pointer, bytes).
    inline void note(const void *p, uint32_t n)
    {
        Set *s = g_cur;
        if (__builtin_expect(s == nullptr, 1))
            return;
        const uint8_t *b = static_cast<const uint8_t *>(p);
        if (b == g_lastP || n == 0u)
            return;
        g_lastP = b;
        if (s->count < kRanges)
            s->r[s->count++] = Range{b, n > 0x10000u ? 0x10000u : n};
    }
    // A guest range (address masked into main RAM).
    inline void noteGuest(const uint8_t *rdram, uint32_t a, uint32_t n)
    {
        if (__builtin_expect(g_cur == nullptr, 1))
            return;
        a &= 0x01FFFFFFu;
        if (a < 0x1000u || a + n > 0x02000000u)
            return;
        note(rdram + a, n);
    }

    // Round 141's second switch (pfc141): the collision natives note too. Read once a frame by passBegin.
    inline bool g_coll = false;
    inline void noteColl(const uint8_t *rdram, uint32_t a, uint32_t n)
    {
        if (__builtin_expect(g_cur != nullptr && g_coll, 0))
            noteGuest(rdram, a, n);
    }

    // A cloth instance's lines (its record, grid, row records and the rows' four buffers: positions / previous,
    // velocities, normals); a no-op when not recording.
    void noteClothSlow(const uint8_t *rdram, uint32_t cloth);
    inline void noteCloth(const uint8_t *rdram, uint32_t cloth)
    {
        if (__builtin_expect(g_cur != nullptr, 0))
            noteClothSlow(rdram, cloth);
    }

    // The script pass's start (decomp_hot44d scriptPass, game thread): a new frame's set.
    void passBegin();
    // A character's ExecCharaGeometry ended (decomp_scriptvm3 charaGeometry, game thread): the segment closes.
    void segmentEnd(uint32_t obj);
}
