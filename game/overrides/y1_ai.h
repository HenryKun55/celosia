// Round Y1 (SOTC_Y1=1, off by default; docs/decomp_y1.md): the small animals' AI library as typed native C++ (y1_ai.cpp:
// the switch, the check, the counts; y1_steer.cpp, y1_move.cpp, y1_step.cpp: the functions). The old natives
// (decomp_hot43i::steer, decomp_hot44c::animalMove, decomp_hot43d::animalStep) hand over at their entry.
#pragma once

#include "ps2_runtime.h"

#include <cstdint>

namespace y1ai
{
    enum Fn : int
    {
        kSteer,
        kMove,
        kStep,
        kFnCount
    };
    using Body = void (*)(uint8_t *, R5900Context *, PS2Runtime *);

    extern signed char g_on;
    void init();
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_on != 0;
    }
    bool partOn(Fn f);
    // Runs `body` (Y1's function) as the call, or the check around it (SOTC_Y1_CHECK=1).
    void enter(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void countGuestCall(Fn f);
    void countFallback(Fn f);

    // The functions (each returns false without touching anything when its entry test fails: the old native runs).
    bool steer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool animalMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool animalStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
