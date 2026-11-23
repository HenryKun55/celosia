// typed_a10_nchk.h: round A10's check of a hand native against what its typed callers take for it
// (the slot holders round A10's a6Is takes: the camera hook, the cam126 / collision natives; docs/decomp_a10.md). A check build
// only (PS2X_A10_CHECK=1): A10_NCHK(address) at the top of the holder runs the typed function (the camera hook: the typed
// iosgluGetCameraContext and its note) or the lifted function itself (the natives) first on the entry state, its stores on the
// check's logged memory (nothing written), then lets the holder run and compares, when it returns, v0 (the note), every register
// and every byte the leg stored (outside the dead stack below the entry sp) with what the holder left ([a10hold]). Elsewhere it is
// nothing.
#pragma once

#include <cstdint>

#ifndef PS2X_A10_CHECK
#define PS2X_A10_CHECK 0
#endif

#if PS2X_A10_CHECK
struct R5900Context;
class PS2Runtime;
namespace a10ck
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
#define A10_NCHK(addr) ::a10ck::NChk a10_nchk_{(addr), rdram, ctx, runtime}
#else
#define A10_NCHK(addr) ((void)0)
#endif
