#pragma once
// Round 130: the diagnostics' switches in one place.
//
// The dev build (default) reads every diagnostic switch from the environment
// (the Vita's env.txt) as before. PS2X_VITA_RELEASE=ON (CMake; SOTC_RELEASE=1)
// builds the version the player plays: ps2_diag::env() is a constexpr nullptr
// for the switches routed through it (*_CHECK, *_VERIFY, *_LOG, *_STATS, the
// difftest, the A/B rotation, the profilers), so `static const bool x =
// ps2_diag::env("SOTC_X_CHECK") != nullptr` is a constant false and the
// compiler drops the check / log / timer paths behind it. kTimers says whether
// the per-call timers exist at all (the frame log's own per-frame clock reads
// stay: the frame summary works in both builds).
#include <cstdlib>

#if !defined(SOTC_RELEASE)
#define SOTC_RELEASE 0
#endif

namespace ps2_diag
{
    constexpr bool kRelease = SOTC_RELEASE != 0;
    // The per-call timers (a hook's own time, a job's hand-over stamp, a join's wait, the subsystems' [xxxt] lines).
    constexpr bool kTimers = !kRelease;
#if SOTC_RELEASE
    constexpr const char *env(const char *) { return nullptr; }
#else
    inline const char *env(const char *name) { return std::getenv(name); }
#endif
}
