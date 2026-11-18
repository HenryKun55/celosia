#pragma once
// Round S13 (SOTC_S13MOT; A/B name s13mot): the motion data as host code over S11's named views, and the EE scheduler's
// time slice (docs/motion_s13.md).
//
//   kPacked (1) kMotion's packed root keys (int16 x3 a frame, each times its scale x 2^-15 (1 + 2^-15) plus its base) as
//               host arithmetic: S11 sent them to the recompiled function (~9 of kMotion's ~16 calls a frame riding).
//   kRoot (2)   the root getters on kMotion's views as host routines (s13_motion.cpp): GetMotionRootIntFrame (0x10b4098:
//               the root key at an integer frame) and GetMotionRootInfo (0x10b67a8: the root's move between two frames,
//               scaled, from two or four root keys), no table calls, no lifted code.
//   kSlice (4)  the EE scheduler's time slice renewed 8 slices long when it ends with nothing ready at or above the
//               running thread's priority (g_ps2xSliceStretch, EeScheduler.cpp): then a slice end only renews the slice,
//               ~10 slow checkpoints a frame riding. A thread made ready at that priority during the stretch cuts it
//               back to the plain slices' next boundary (counted: sliceCuts), a higher one preempts at once as before.
//   kZeroDiv (8) the FPU divide of a zero (or flushed denormal) by a normal divisor and the root of a zero answered as
//               the signed zero they are in every rounding mode (g_ps2xFpuZeroFast, ps2_runtime_macros.h), not through
//               the rounding-mode switch (two FPSCR writes and fegetround / fesetround calls each); the switch's writes
//               also left Vita3K's translated code rounding to nearest for a while (docs/motion_s13.md).
//
// SOTC_S13MOT_PARTS=<mask> (default all) limits the switch to some parts; SOTC_S13MOT_CHECK=1 runs kPacked / kRoot's
// calls both ways on the game thread (the host answer, then the recompiled leg on the live state) and compares the
// outputs, v0, f0 and the VU0 registers; SOTC_S13MOT_LOG=<frames> prints "[s13]" counts; SOTC_S13MOT_STRETCH=<n> the
// slice stretch (8).
#include <cstdint>
#include <cstdlib>
#include "sotc_decomp.h"
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace s13mot
{
    enum Part : uint32_t
    {
        kPacked = 1u,
        kRoot = 2u,
        kSlice = 4u,
        kZeroDiv = 8u,
        kAll = 15u
    };
    inline uint32_t g_parts = kAll;
    inline uint32_t g_log = 0;
    inline bool g_check = false;
    inline uint32_t g_stretch = 8u;
    inline void init()
    {
        // Off unless SOTC_S13MOT=1 (A/B riding even: +0.12 / -0.10 ms, docs/motion_s13.md); the A/B's turns still flip it.
        // (set here, not in sotc_decomp.h's default list: that header is in every translation unit)
        const char *e = std::getenv("SOTC_S13MOT");
        sotc_decomp::g_switch[sotc_decomp::kS13Mot] = (e && e[0] >= '1' && e[0] <= '3') ? 1 : 0;
        const char *v = std::getenv("SOTC_S13MOT_PARTS");
        g_parts = v ? static_cast<uint32_t>(std::strtoul(v, nullptr, 0)) & kAll : static_cast<uint32_t>(kAll);
        v = std::getenv("SOTC_S13MOT_LOG");
        g_log = v ? static_cast<uint32_t>(std::atoi(v)) : 0u;
        v = std::getenv("SOTC_S13MOT_CHECK");
        g_check = v && std::atoi(v) != 0;
        v = std::getenv("SOTC_S13MOT_STRETCH");
        if (v && std::atoi(v) >= 1)
            g_stretch = static_cast<uint32_t>(std::atoi(v));
    }
    inline bool on(uint32_t part) { return (g_parts & part) != 0u && sotc_decomp::switchOn(sotc_decomp::kS13Mot); }

    struct Stats
    {
        uint64_t packedHost = 0;                                          // kMotion calls answered with a packed key
        uint64_t intHost = 0, intFallback = 0, infoHost = 0, infoFallback = 0;
        uint64_t checks = 0, differ = 0, unwound = 0;                     // kPacked / kRoot checks
        uint64_t differBy[4] = {};
        uint64_t packedDiffer[8] = {}; // a packed key's difference by which host answers matched the leg: 1 again before it,
                                       // 2 again after the control word written back, 4 again after the leg                                        // by routine: 0 the float frame, 1 int frame, 2 root info, 3 the root key
        uint64_t fpscrMoved = 0;                                          // check: the float mode changed inside the recompiled leg
    };
    inline Stats g_st;
    // A difference found by the check: counted by routine; true to print it (the first 8 of each).
    inline bool differ(int which)
    {
        ++g_st.differ;
        return g_st.differBy[which]++ < 8u;
    }
    // Writes the float unit's control word back unchanged (the check's experiment: an emulator that keeps the rounding
    // mode per translated block sees the write).
    inline void fpscrRewrite()
    {
#if defined(__arm__)
        uint32_t r;
        __asm__ __volatile__("vmrs %0, fpscr" : "=r"(r));
        __asm__ __volatile__("vmsr fpscr, %0" : : "r"(r));
#endif
    }

    // The float unit's control word (the rounding mode: bits 22-23 on the A9, 0 round to nearest, 3 toward zero).
    uint32_t fpscr();
    // The check's guard against Vita3K's float mode (docs/motion_s13.md): after the rounding-mode switch of a divide /
    // root by a non-normal operand (ps2FpuDivideNearestCsr: two FPSCR writes), the emulator's translated code can go on
    // rounding to nearest for a while although the FPSCR reads toward zero, until its next exit to the host (a kernel
    // call). The check makes one (a clock read) before each compared call, so both legs run in the mode the FPSCR
    // says. A no-op outside the check, and on the console's hardware the FPSCR is what rounds.
    inline void emuSync()
    {
#if defined(__vita__)
        (void)sceKernelGetProcessTimeLow();
#endif
    }
    inline uint32_t fpscrInline()
    {
#if defined(__arm__)
        uint32_t r;
        __asm__ __volatile__("vmrs %0, fpscr" : "=r"(r));
        return r;
#elif defined(__aarch64__)
        uint64_t r;
        __asm__ __volatile__("mrs %0, fpcr" : "=r"(r));
        return static_cast<uint32_t>(r);
#else
        return 0u;
#endif
    }
}
