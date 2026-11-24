// a13_watch.h: round A13's freeze hunt (docs/decomp_a13.md section 1): where stage 12's typed code runs on a guest worker's
// runtime shell (ps2_worker, sotc_worker.cpp), and the ride watchdog (sotc_replay's horse step: Agro and Wander still while
// mounted after they moved).
//
// a13diag::shellTyped(fn): the measuring probe of the first A13 pass (an A12 adapter's test reached on a worker shell, whose
// zeroed storage left m_noCheckFloor 0, so every A*_GO, PS2_N4_FAST and callGuestKnownNoCheck passed there). Round A13b set the
// shell's floor to UINTPTR_MAX in sotc_worker.cpp, so A12_GO no longer calls it; kept for the [a13w] line (counts stay 0).
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
