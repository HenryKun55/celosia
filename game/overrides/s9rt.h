#pragma once
// Round S9 (SOTC_S9RT, default on; A/B name s9rt): the game thread's "Runtime" costs riding (Vita3K 222 MHz, ride7:
// 11% of the game thread, ~3.5 ms a frame). What the sampler found by caller (docs/runtime_s9.md) and what S9 does:
//
//   kEvict (1)   the renderer's eviction pass at the hand-over (native_renderer_gl.cpp GlBackend::evictKept): riding, the
//                heap sits above its high mark, so every 16 frames a "heap-only" pressure pass walks every kept mesh
//                (~7 600) and puts every draw's mesh / texture in two hash sets, and evicts nothing: in that pass a mesh
//                can only go when aged (every 256th frame) or under a budget cut, a texture only when it lives in the
//                newlib heap. S9 runs that pass over the textures alone (and builds the kept-texture set only when a
//                texture in the heap is old enough to go): the same evictions in the same cases. (~1.3% of the thread)
//   kClock (2)   S5's particle batch stamped its hand-over with a clock read every frame for the [s5ptcl] log's start
//                lag (and the join its waits): read only with the log on. (~0.75% in Vita3K: an HLE call there)
//   kCopy (4)    the camera parameter copy (decomp_leaves: 0x654 bytes, ~ a hundred a frame riding) went through newlib's
//                memcpy to sceClibMemcpy (> 512 bytes): a NEON copy here, the same bytes. (~0.56%)
//   kShadow (8)  shadowCapture read a skinned shadow's bone palette into a 16 KB picture, then copied its rows into the
//                job (sceClibMemmove): read straight into the job as the model hook's lean path does (the same values:
//                zero where no UNPACK wrote, later UNPACKs over earlier ones). (~0.47%)
//   kZero (16)   ctrlTurn zeroed its 32 VU0 registers (a 512-byte memset: sceClibMemset) and then loaded 19 of them:
//                only the other 13 are zeroed now (the same values). (~0.2%)
//   kHookTime (32) with the frame log on (SOTC_FRAME_LOG=1, the console's env) every display hook on the game thread
//                (modelDispHook, ptclS1Commit, meshDispHook, shadowCapture, ...) read the clock twice for the log's
//                "hooks" time: ~350 sceKernelGetProcessTimeWide a frame riding (a syscall on the console). Not read
//                (the log's hooks time reads 0 on those frames) unless SOTC_S9RT_HOOKTIME=1.
//
// SOTC_S9RT_PARTS=<mask> (default all) limits the switch to some parts (the rest stay as before); SOTC_S9RT_CHECK=1 runs
// the checkable parts both ways (kEvict: the full pass's candidate list against S9's; kShadow: the two palettes; kCopy:
// the bytes) and prints "[s9rt]" counts every SOTC_S9RT_LOG frames (default 600 with the check).
#include <cstdint>
#include <cstdlib>
#include <cstring>
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif
#include "sotc_decomp.h"

namespace s9rt
{
    enum Part : uint32_t
    {
        kEvict = 1u,
        kClock = 2u,
        kCopy = 4u,
        kShadow = 8u,
        kZero = 16u,
        kHookTime = 32u,
        kAll = 63u
    };
    // Read once at install (s9rt::init from sotc_native's install): plain loads in the hot paths, no static guards.
    inline uint32_t g_parts = kAll;
    inline bool g_check = false;
    inline uint32_t g_log = 0;
    inline bool g_hookTimeAll = false; // SOTC_S9RT_HOOKTIME=1: the hooks' clock reads kept with S9 on
    inline void init()
    {
        const char *v = std::getenv("SOTC_S9RT_PARTS");
        g_parts = v ? static_cast<uint32_t>(std::strtoul(v, nullptr, 0)) & kAll : static_cast<uint32_t>(kAll);
        v = std::getenv("SOTC_S9RT_CHECK");
        g_check = v && std::atoi(v) != 0;
        v = std::getenv("SOTC_S9RT_LOG");
        g_log = v ? static_cast<uint32_t>(std::atoi(v)) : (g_check ? 600u : 0u);
        v = std::getenv("SOTC_S9RT_HOOKTIME");
        g_hookTimeAll = v && std::atoi(v) != 0;
    }
    inline bool on(uint32_t part) { return (g_parts & part) != 0u && sotc_decomp::switchOn(sotc_decomp::kS9Rt); }

    // Counts for the log (game thread only; the renderer's eviction counts are its own: native_renderer_gl.cpp).
    struct Stats
    {
        uint64_t copies = 0, copiesOff = 0, shadows = 0, shadowsOff = 0, zeros = 0, zerosOff = 0, clockSkips = 0;
        uint64_t checks = 0, differ = 0;
    };
    inline Stats g_st;

    // A copy of n bytes between non-overlapping buffers without a libc call (newlib's memcpy sends > 512 bytes to
    // sceClibMemcpy). NEON 16-byte loads / stores (any alignment), then the tail byte by byte.
    inline void copyBytes(uint8_t *__restrict to, const uint8_t *__restrict from, uint32_t n)
    {
        uint32_t i = 0;
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        for (; i + 64u <= n; i += 64u)
        {
            const uint8x16_t a = vld1q_u8(from + i), b = vld1q_u8(from + i + 16u), c = vld1q_u8(from + i + 32u), d = vld1q_u8(from + i + 48u);
            vst1q_u8(to + i, a);
            vst1q_u8(to + i + 16u, b);
            vst1q_u8(to + i + 32u, c);
            vst1q_u8(to + i + 48u, d);
        }
        for (; i + 16u <= n; i += 16u)
            vst1q_u8(to + i, vld1q_u8(from + i));
#endif
        for (; i < n; ++i)
        {
            volatile uint8_t b = from[i]; // (no loop-to-memcpy pattern)
            to[i] = b;
        }
    }
}
