// a13_watch.h: round A13's freeze hunt (docs/decomp_a13.md section 1): where stage 12's typed code runs on a guest worker's
// runtime shell (ps2_worker, sotc_worker.cpp), and the ride watchdog (sotc_replay's horse step: Agro and Wander still while
// mounted after they moved).
//
// a13diag::shellTyped(fn): an A12 adapter's test reached on a worker shell. The shell is zeroed storage (sotc_worker.cpp), so
// its m_noCheckFloor is 0, not UINTPTR_MAX: every A*_GO (and PS2_N4_FAST, callGuestKnownNoCheck) passes there, and the typed
// body runs beside the game thread without the worker's admit / park (its typed callees are C++ calls, not hookCall's). The
// count per adapter (the lifted function's __func__) is kept; the answer is false while g_shellOff (SOTC_A13_WSHELL=0, or the
// watchdog's first step).
#pragma once

#include <cstdint>

class PS2Runtime;

namespace a13diag
{
    extern bool g_shellOff;  // A12_GO false on a worker shell (SOTC_A13_WSHELL=0; the watchdog's step 1)
    extern bool g_forceOff;  // a12typed off whatever the switch says (the watchdog's step 2; sotc_native's a12FlagsNow)
    bool shellTyped(const char *fn); // counts; returns !g_shellOff
    void noteFrame(bool a12typed);   // sotc_native, once a frame: the flag's history (64 frames)
    // sotc_replay's horse step (game thread): h the ridden horse (0 on foot), still the steps both places were bit-identical
    void rideStep(PS2Runtime *runtime, uint8_t *rdram, uint32_t player, uint32_t horse, uint32_t h, uint32_t still, uint64_t frame);
    void report(uint64_t frame); // the [a13w] shell counts (sotc_native's [a12] line)
}
