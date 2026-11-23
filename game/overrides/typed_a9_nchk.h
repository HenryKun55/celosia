// typed_a9_nchk.h: round A9's check of a hand native against what its typed callers take for it
// (docs/a9_natives.txt, the A9 cores of typed_a9_ext.h, GetLayoutClass's slot holder; docs/decomp_a9.md). A check build only (PS2X_A9_CHECK=1): A9_NCHK(address) at the top of the native
// runs the typed function first on the native's entry state, its stores on the check's logged memory (nothing written),
// then lets the native run and compares, when it returns, its return value and every byte the typed function stored
// (outside the dead stack below the entry sp) with what the native left. Elsewhere it is nothing.
#pragma once

#include <cstdint>

#ifndef PS2X_A9_CHECK
#define PS2X_A9_CHECK 0
#endif

#if PS2X_A9_CHECK
struct R5900Context;
class PS2Runtime;
namespace a9ck
{
    struct NChk
    {
        NChk(uint32_t addr, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
        ~NChk();
        static constexpr uint32_t kMax = 512;
        bool on = false, over = false, ext = false, slot = false;
        int kind = 0;
        uint32_t addr = 0, n = 0;
        uint64_t v0 = 0;
        uint32_t f0 = 0;
        uint8_t *rdram = nullptr;
        R5900Context *ctx = nullptr;
        uint32_t a[kMax];
        uint8_t v[kMax];
    };
}
#define A9_NCHK(addr) ::a9ck::NChk a9_nchk_{(addr), rdram, ctx, runtime}
#else
#define A9_NCHK(addr) ((void)0)
#endif
