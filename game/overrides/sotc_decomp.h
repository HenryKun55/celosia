// Decompiled game code: guest functions rewritten by hand in C++, put in
// place of their recompiled versions.
//
// Each decomp_*.cpp registers its functions with SOTC_DECOMP_FUNCTION. They
// replace the recompiled code by default (SOTC_DECOMP=0: none of them,
// SOTC_DECOMP_SKIP=name,name: those left recompiled).
//
// Differential test, SOTC_DIFFTEST=all | name,name: each listed function is
// checked against its recompiled version on the same input. The native one
// runs first on the live state; its registers and every byte it changed in
// RAM and the scratchpad are kept aside; the state is put back and the
// recompiled one runs (the game goes on with its result). Return registers
// (v0, v1, f0), the callee-saved ones (s0-s7, fp, gp, sp, ra) and memory are
// compared; the caller-saved scratch registers may differ. Every call is
// checked until SOTC_DIFFTEST_FIRST (default 200) calls, then one in
// SOTC_DIFFTEST_EVERY (default 1000): a check copies 32 MB twice.
#pragma once

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"

#include <cstdint>
#include <cstring>
#include <vector>

namespace sotc_decomp
{
    struct Function
    {
        uint32_t address;
        const char *name;
        PS2Runtime::RecompiledFunction native;
        bool returnsFloat = false; // the result in f0 (v0 is scratch then)
        // A context switch (a coroutine's wake / sleep): it saves one
        // register set below sp and loads another. Checked with every
        // register and the stack below sp compared; `once`: the recompiled
        // code is run once and not to its return (a sleep returns to the
        // other context, which goes on for a frame or more).
        bool switches = false, once = false;
        // Part of SOTC_AB's "new" set (sotc_decomp.cpp: alternated with the
        // recompiled code, with the runtime's fast paths, for an A/B in one run).
        bool ab = false;
    };

    std::vector<Function> &registry();

    struct Registrar
    {
        Registrar(uint32_t address, const char *name, PS2Runtime::RecompiledFunction native, bool returnsFloat = false, bool switches = false,
                  bool once = false, bool ab = false)
        {
            registry().push_back({address, name, native, returnsFloat, switches, once, ab});
        }
    };

    // Puts the registered functions in place (and the checks SOTC_DIFFTEST asks for).
    void install(PS2Runtime &runtime);
    // SOTC_AB=<frames>: every that many game frames the "new" set (the
    // functions registered with SOTC_DECOMP_AB_FUNCTION, the runtime's
    // ps2_fast_paths) goes off or back on; "[ab] frame F: new | old" at each
    // change, and the [prof] lines say which ran ("ab: new | old | mixed").
    void abTick(uint64_t frame);
    // SOTC_AB's state: true while the new set runs (always, without SOTC_AB),
    // for a native hook with a new path of that round to ask.
    bool abNew();

    // The recompiled function a decompiled one replaced (for a native
    // function's rare cases left to the original code), nullptr if none.
    PS2Runtime::RecompiledFunction recompiled(uint32_t address);

    // Calls guest code from a decompiled function as a jal would: ra and sp
    // set (sp: the original's frame at that call), run until it returns there.
    void callGuest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp);
    // The same for the recompiled code a decompiled function replaced (its
    // rare cases, from a native caller): ra and sp set, run to its return.
    void callRecompiled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address, uint32_t ra, uint32_t sp);

    // Relocation sites: the XFF modules' references to the boot ELF and to
    // each other are patched into the code when the game links them, so the
    // values are read from the live instruction words, as the recompiled code
    // does (_dynInsn), not taken from the disassembly.
    inline uint32_t codeWord(const uint8_t *rdram, uint32_t at)
    {
        uint32_t v;
        std::memcpy(&v, rdram + (at & 0x01FFFFFFu), 4);
        return v;
    }
    // lui at hi + the signed immediate at lo (addiu / lw / sw ...).
    inline uint32_t relocated(const uint8_t *rdram, uint32_t hi, uint32_t lo)
    {
        return (codeWord(rdram, hi) << 16) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(codeWord(rdram, lo) & 0xFFFFu)));
    }
    // The target of the jal at a site.
    inline uint32_t jalTarget(const uint8_t *rdram, uint32_t at) { return (codeWord(rdram, at) & 0x03FFFFFFu) << 2; }

    // VU0 arithmetic with fewer clamps. PS2_VMUL / PS2_VADD / PS2_VSUB clamp
    // both operands and the result (ps2VuNormalize: denormals to signed
    // zero, Inf / NaN to signed FLT_MAX), and the clamp is idempotent, so a
    // value that is already a clamped result (or clamped once, like a matrix
    // row used for four columns) need not be clamped again: vmulN(a, b) ==
    // PS2_VMUL(a, b) when a and b are clamped, bit for bit.
    namespace vu
    {
        inline __m128 clamp(__m128 v) { return ps2VuNormalize(v); }
        inline __m128 mulN(__m128 a, __m128 b) { return ps2VuNormalize(_mm_mul_ps(a, b)); }
        inline __m128 addN(__m128 a, __m128 b) { return ps2VuNormalize(_mm_add_ps(a, b)); }
        inline __m128 subN(__m128 a, __m128 b) { return ps2VuNormalize(_mm_sub_ps(a, b)); }
        template <int L>
        inline __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
        struct Rows
        {
            __m128 r[4];
        };
        // Four rows clamped once.
        inline Rows clampRows(const __m128 *rows) { return {{clamp(rows[0]), clamp(rows[1]), clamp(rows[2]), clamp(rows[3])}}; }
        // vmulax / vmadday / vmaddaz / vmaddw: rows weighted by v's x, y, z, w (rows clamped, v raw).
        inline __m128 apply(const Rows &a, __m128 v, __m128 *acc = nullptr)
        {
            const __m128 c = clamp(v);
            __m128 t = mulN(a.r[0], lane<0>(c));
            t = addN(t, mulN(a.r[1], lane<1>(c)));
            t = addN(t, mulN(a.r[2], lane<2>(c)));
            if (acc)
                *acc = t;
            return addN(t, mulN(a.r[3], lane<3>(c)));
        }
    }

    // Returning from a decompiled function: the jr ra the recompiled code ends with.
    inline void returnTo(R5900Context *ctx) { ctx->pc = GPR_U32(ctx, 31); }
}

#define SOTC_DECOMP_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn}
// One of SOTC_AB's new set (this round's functions).
#define SOTC_DECOMP_AB_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, true}
#define SOTC_DECOMP_FLOAT_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, true}
// A context switch that returns to its caller after a round trip (wake).
#define SOTC_DECOMP_SWITCH_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, false}
// A context switch that returns into the other context (sleep).
#define SOTC_DECOMP_SWITCH_ONCE_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, true}
