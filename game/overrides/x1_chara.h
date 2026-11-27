// Round X1 (docs/decomp_x1.md): ExecCharaGeometry (0x10bf8f0) as native C++ on the character class, behind SOTC_X1=1
// (off by default; needs SOTC_D1NATIVE=1, as on the console). x1_charageo.cpp.
//
// The old body (decomp_charageo.cpp, round 41 / 47) is the recompiled code's statements kept context-exact: every VU0
// op a PS2_V* macro on ctx->vu0_vf[...] (a load, the op, the operand / result clamps, a full blend, a store per op),
// every guest field through a masked Ram read, the bone products and the node quaternions through out-of-line helpers.
// X1 keeps what the game state depends on and nothing else:
//   - the class as a host pointer, its fields read once into locals (a typed view, x1::Chara);
//   - VU0 math on host vectors in NEON registers, the same ops / operand and result clamps in the same order (the bits
//     do not change), written to the context only where the original leaves them for its callees and its caller:
//     at a guest call and at the exit;
//   - the node quaternions: four dot products at a time (round W2's lane mapping), the rsqrt per node in order;
//   - the bone products (+0x160 = +0x15c x +0x2d8): the same NEON chain as pose82 with the next bones' lines asked
//     for (PLD) ahead: the A9's prefetcher is off on the Vita;
//   - the guest calls, their arguments, the frame slots callees read, s0-s7 / fp / f20 / f21 at each call and the
//     epilogue's restores exactly as the original (a callee saves them into its frame; round 47).
// SOTC_X1_CHECK=1 (the proof, game thread): X1's run is recorded, then the old body replays the same call against the
// record (the callees' results as X1's calls got them); at every guest call and at the exit the two are compared (every byte either wrote, GPRs, FPRs, fcr31, VU0 vf1-vf31 /
// ACC / Q / status, the call's site and target). Rare states (pose82 off, far-character halving on, the per-callee
// timing log) run the old body.
#pragma once

#include <cstdint>
#include <cstdlib>

class PS2Runtime;
struct R5900Context;

namespace x1
{
    inline signed char g_on = -1, g_check = -1;
    // Read at first use (the Vita's env.txt is read after static init).
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
        {
            const char *e = std::getenv("SOTC_X1");
            g_on = (e && e[0] == '1') ? 1 : 0;
        }
        return g_on != 0;
    }
    inline bool checkOn()
    {
        if (__builtin_expect(g_check < 0, 0))
        {
            const char *e = std::getenv("SOTC_X1_CHECK");
            g_check = (e && e[0] == '1') ? 1 : 0;
        }
        return g_check != 0;
    }
    // SOTC_X1_CHECK_FROM=<frame>: the check stays off until that game frame (the run at full speed until then).
    inline long long g_checkFrom = -1;
    bool checkFromReached();
    // ExecCharaGeometry's body (decomp_charageo's execCharaGeometryBody calls it first when on): true when it ran,
    // false (nothing done) for the states the old body keeps.
    bool execCharaGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
