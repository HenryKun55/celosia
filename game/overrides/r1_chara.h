// Round R1 (switch r1char, SOTC_R1CHAR, kR1Char; off by default; docs/decomp_r1.md): the characters' per-frame data walks
// laid out for the Vita's Cortex-A9.
//
// The console (docs/vita_perf_guide.md section 0): ~106 k L1 D misses a frame on the game thread, ~46 stall cycles each,
// 32-byte lines, and NO hardware prefetcher (ACTLR 0x149): a walk over guest records costs one miss per line it touches
// first, in series, unless the code asks for the lines ahead (PLD = __builtin_prefetch). R1 acts inside d1native's D bodies
// (SOTC_D1NATIVE=1; with d1native off the old bodies run and r1char changes nothing):
//
//   - the cloth step (dc2::d1b::clothStepD1Core<true>): the next row's three point buffers (velocity, positions as read,
//     positions written) asked for one row ahead, a line of each every second point of the current row; the row records
//     up front and two rows ahead; the pin pass's row parameters up front and the next row's two pin-entry lists ahead;
//   - the buffers' flip (getClothDynamics' D1 adapter, clothFlipAhead): the grid and every row record asked for before the
//     flip, the first two rows' buffers after it (before the step's guest call);
//   - the link list scan (decomp_chara::d3l::charaLinksD3Core<true>): the 28-byte entries asked for four ahead, and the two
//     handle columns' tables cached apart (each column alternates with the other's table in the one-slot cache);
//   - the pose (s3chara::poseR1, called by GetMotionMatrix's D3 core): the rig compared with the guest's bone records once a
//     game frame (C1's rule) and the bone offsets from its contiguous host array (P1's), the outputs' lines asked for two
//     bones ahead and the locals' lines up front.
//
// Exactness: prefetches change no value; the mirrors are compared with the guest's words before their use; the order of
// every guest read and write and every float operation is the D body's. The difftest (SOTC_DIFFTEST_SELF=1
// SOTC_DIFFTEST_SWITCH=r1char) runs a native with r1char on against itself with it off.
//
// Census (SOTC_R1CHAR_CENSUS=1, any switch state, game thread): per walk, per frame, the calls, records, bytes and the
// guest lines touched first in the frame (a stamp per 32-byte line of the 32 MB guest RAM: 2 MB, made at the first use);
// SOTC_R1CHAR_LOG=<frames> prints [r1char] lines.
#pragma once

#include "sotc_decomp.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace r1chara
{
    inline bool on() { return sotc_decomp::r1charOn(); }

    // A guest line asked for ahead (host pointer into guest RAM; read intent, keep).
    PS2_FORCE_INLINE void pf(const void *p) { __builtin_prefetch(p, 0, 3); }
    // ... with write intent (the line will be written).
    PS2_FORCE_INLINE void pfw(const void *p) { __builtin_prefetch(p, 1, 3); }
    // Every line of [p, p + n) (n bytes; at most `cap` lines).
    PS2_FORCE_INLINE void pfRange(const uint8_t *p, uint32_t n, uint32_t cap = 16u)
    {
        const uintptr_t a = reinterpret_cast<uintptr_t>(p) & ~static_cast<uintptr_t>(31u), e = reinterpret_cast<uintptr_t>(p) + n;
        for (uintptr_t q = a; q < e && cap != 0u; q += 32u, --cap)
            __builtin_prefetch(reinterpret_cast<const void *>(q), 0, 3);
    }

    // ---- census ----------------------------------------------------------------------------------------------------
    enum Walk
    {
        kWClothStep,  // the step's row records, point buffers, pin lists, pin entries, bone matrices
        kWClothPins,  // (the pin part of the step, counted apart)
        kWClothFlip,  // the flip's row records
        kWLinks,      // charaLinks' entries, handle slots and tables
        kWPose,       // the pose's locals, matrices, quaternions written, bone records compared
        kWCount
    };
    inline const char *const kWalkNames[kWCount] = {"clothStep", "clothPins", "clothFlip", "charaLinks", "pose"};
    struct Census
    {
        uint64_t calls[kWCount] = {}, recs[kWCount] = {}, bytes[kWCount] = {}, lines[kWCount] = {}, newLines[kWCount] = {};
    };
    inline int g_census = -1, g_log = -1;
    inline uint16_t *g_stamp = nullptr; // per 32-byte guest line: the frame (low 16 bits) + 1 it was first touched in
    inline Census g_c;                  // since the last log line
    inline uint64_t g_frame = ~0ull, g_logFrom = 0, g_frames = 0;

    __attribute__((noinline, cold)) inline void readEnv()
    {
        const char *c = std::getenv("SOTC_R1CHAR_CENSUS");
        const char *l = std::getenv("SOTC_R1CHAR_LOG");
        g_census = c ? std::atoi(c) : 0;
        g_log = l ? std::atoi(l) : 0;
        if (g_census > 0 && g_log <= 0)
            g_log = 600;
    }
    PS2_FORCE_INLINE bool census()
    {
        if (__builtin_expect(g_census < 0, 0))
            readEnv();
        return g_census > 0;
    }

    __attribute__((noinline, cold)) inline void report(uint64_t fr)
    {
        const double n = static_cast<double>(g_frames ? g_frames : 1u);
        char buf[900];
        int k = 0;
        for (int w = 0; w < kWCount && k < 820; ++w)
            k += std::snprintf(buf + k, sizeof(buf) - static_cast<size_t>(k), " %s calls %.1f recs %.1f bytes %.0f lines %.1f first-in-frame %.1f;", kWalkNames[w],
                               static_cast<double>(g_c.calls[w]) / n, static_cast<double>(g_c.recs[w]) / n, static_cast<double>(g_c.bytes[w]) / n,
                               static_cast<double>(g_c.lines[w]) / n, static_cast<double>(g_c.newLines[w]) / n);
        std::fprintf(stderr, "[r1char] frame %llu: r1char %d d1native %d; per frame over %llu frames:%s\n", (unsigned long long)fr, on() ? 1 : 0,
                     sotc_decomp::switchOn(sotc_decomp::kD1Native) ? 1 : 0, (unsigned long long)g_frames, buf);
        g_c = Census{};
        g_frames = 0;
    }
    __attribute__((noinline, cold)) inline void roll()
    {
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (g_frame != ~0ull)
            ++g_frames;
        g_frame = fr;
        if (g_log > 0 && fr - g_logFrom >= static_cast<uint64_t>(g_log))
        {
            if (g_frames)
                report(fr);
            g_logFrom = fr;
        }
    }
    // A record of a walk (call `call` once a call). Only when census() is true, only on the game thread.
    PS2_FORCE_INLINE void call(Walk w)
    {
        if (__builtin_expect(g_frame != sotc_decomp::g_gameFrame, 0))
            roll();
        ++g_c.calls[w];
    }
    // `n` bytes of guest RAM at guest address `a` read or written by walk w (one record).
    __attribute__((noinline)) inline void touch(Walk w, uint32_t a, uint32_t n)
    {
        if (!g_stamp)
            g_stamp = static_cast<uint16_t *>(std::calloc(0x02000000u / 32u, sizeof(uint16_t)));
        if (!g_stamp || n == 0u)
            return;
        ++g_c.recs[w];
        g_c.bytes[w] += n;
        const uint16_t s = static_cast<uint16_t>((sotc_decomp::g_gameFrame & 0x7FFFu) + 1u);
        const uint32_t lo = (a & 0x01FFFFFFu) >> 5, hi = ((a & 0x01FFFFFFu) + n - 1u) >> 5;
        for (uint32_t l = lo; l <= hi && l < 0x02000000u / 32u; ++l)
        {
            ++g_c.lines[w];
            if (g_stamp[l] != s)
            {
                g_stamp[l] = s;
                ++g_c.newLines[w];
            }
        }
    }
} // namespace r1chara
