// Round S1: the particle step's batch (decomp_particles.cpp) shared with the emitter's step (decomp_ptclupdate.cpp).
#pragma once

#include <cstdint>

struct R5900Context;
class PS2Runtime;

namespace decomp_particles
{
    // An emitter's per-frame values for its particles' step, derived once (prep123) instead of per particle.
    // Phases: [0] before the parameter's age limit, [1] after; decays already raised to the frame's VBlanks.
    struct Emitter123
    {
        bool ok = false;
        uint32_t list = 0, a2 = 0, kind = 0, params = 0, t1 = 0, sp0 = 0;
        float dt = 0, dt2 = 0;
        uint32_t shape = 0, space = 0, spin = 0, collision = 0;
        bool jitter = false, randomAxis = false, rotation = false, rotWraps = false, rotSignRule = false, spinY = false;
        int32_t vbl = 0;
        float m[16];
        float accLim = 0, accScale[2] = {1, 1}, velDecay[2] = {1, 1}; // accScale applied only when accLim > 0
        float growLim = 0, growth[2] = {0, 0}, growDecay[2] = {1, 1};
        float turnLim = 0, turnK[2] = {0, 0}, turnDecay[2] = {1, 1};
        float chK[3] = {0, 0, 0}, chDecay[3] = {1, 1, 1};
        float spinCh = 0, spinDecay = 1, ax[3] = {0, 0, 0}, centre[3] = {0, 0, 0}, drift[3] = {0, 0, 0}, X[3] = {0, 0, 0};
        float jitterAmp = 0, jitterHalf = 0, axisBase = 0, axisRange = 0;
        float margin = 0, floorY = 0, ceilingY = 0, floorK[3] = {1, 1, 1}, ceilK[3] = {1, 1, 1};
        uint32_t floorNormal = 0, ceilNormal = 0;
        float rotStart = 0, rotLim = 0, rotCh = 0, rotDecay = 1;
        float span2Ch = 0, span2Decay = 1, spinYAdd = 0;
    };
    // false: the emitter stays with update123 (pointers out of RAM, the damage test on).
    bool prep123(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t list, uint32_t a2, uint32_t kind, uint32_t params, uint32_t t1, uint32_t sp0,
                 Emitter123 &e);
    // One particle (update123's answer: false = the original's path); every n-th also checked against update123.
    bool step123Checked(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Emitter123 &e, uint32_t p, float &f0out, uint32_t every);
    // A live particle the batch does not step: the caller's original path (p, nodes left, its link, f0 in / out).
    using Fallback = void (*)(void *arg, uint32_t p, int32_t left, uint32_t next, float &f0);
    // The emitter's first n list nodes as one batch; returns the particles it stepped (every n-th checked).
    uint32_t stepRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Emitter123 &e, uint32_t first, int32_t n, uint32_t every, float &f0,
                     Fallback fallback, void *arg, uint32_t &last);
}
