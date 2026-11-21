// typed_a1_rt.h: what round A1's typed functions (typed_a1.h, tools/typed_lift.py; docs/decomp_a1.md) are written
// against: guest memory through a base pointer, the PS2 float helpers by name, the switch.
//
// Memory. `ram` is main RAM (32 MB). A1_R32B(base, off) / A1_W32B(base, off, v) are a MIPS `lw / sw rt, off(base)`:
// a base in [0x10000, 0x1FF0000) with an immediate offset is an address in main RAM, one host load / store (N1's base
// test: the same test, the same plain access); any other base goes to the runtime's slow path with the full address
// (scratchpad, I/O, the mirrors: `rt` is the memory environment, a guest worker's scratchpad is its own). A1_R32(addr)
// is an access at a full address (the generators' `lw rt, 0(base)` and lwl / lwr forms): a constant address folds its
// test away. No R5900 context: the slow path's context argument (used only to signal an address error, which this
// game never takes, and by the Mac's store trace) is a static dummy.
//
// Float. The PS2 FPU differs from IEEE single precision in its rounding (the game thread runs round toward zero with
// flush to zero: ps2SetEeFloatingPointMode), in add / subtract (the operands aligned to the larger exponent first),
// in clamping (no Inf / NaN: results clamp to +-FLT_MAX), in compares (a key that orders -0 / +0 and denormals as the
// PS2 does) and in divide by zero (a signed max and the fcr31 flags). Where lift_d1's macro is a plain C operation
// (mov, neg, abs, cvt.s.w, the finite multiply) the typed code is that plain C operation; where it is not, a named
// helper below (a1f::) does exactly what the macro does. docs/decomp_a1.md section 3.
#pragma once

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"

#include <cmath>
#include <cstdint>
#include <cstring>

#ifndef PS2X_A1_CHECK
#define PS2X_A1_CHECK 0
#endif

// The switch a1typed (sotc_decomp::kA1Typed, SOTC_A1TYPED): the lifted functions' adapters and the N4 sites call the
// typed functions while it is on. Set once a frame (sotc_native.cpp endFrame) and at install.
extern bool g_a1Typed;
#define A1_ON g_a1Typed

namespace a1
{
    struct RetVF // a function that returns both v0 and f0
    {
        uint64_t v0;
        float f0;
    };
}

inline uint64_t a1_sx64(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
inline uint32_t a1_bits(float f)
{
    uint32_t b;
    std::memcpy(&b, &f, 4);
    return b;
}
inline float a1_flt(uint32_t b)
{
    float f;
    std::memcpy(&f, &b, 4);
    return f;
}

namespace a1m
{
    R5900Context *noCtx(); // the slow path's dummy context (typed_a1_rt.cpp)
    [[gnu::noinline, gnu::cold]] uint8_t slowRead8(uint8_t *ram, PS2Runtime *rt, uint32_t a);
    [[gnu::noinline, gnu::cold]] uint16_t slowRead16(uint8_t *ram, PS2Runtime *rt, uint32_t a);
    [[gnu::noinline, gnu::cold]] uint32_t slowRead32(uint8_t *ram, PS2Runtime *rt, uint32_t a);
    [[gnu::noinline, gnu::cold]] uint64_t slowRead64(uint8_t *ram, PS2Runtime *rt, uint32_t a);
    [[gnu::noinline, gnu::cold]] void slowWrite8(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint8_t v);
    [[gnu::noinline, gnu::cold]] void slowWrite16(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint16_t v);
    [[gnu::noinline, gnu::cold]] void slowWrite32(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint32_t v);
    [[gnu::noinline, gnu::cold]] void slowWrite64(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint64_t v);

    // The inline slow path: the same call the lifted code makes (ps2_guest_mem::*_slow: on the Vita the register-saving
    // trampoline, so an inlined typed body keeps its values in registers across it; elsewhere the plain slow path with
    // the given runtime). The context argument is a static one (g_a1SlowCtx).
    extern R5900Context g_a1SlowCtx;
    template <typename T>
    PS2_FORCE_INLINE T slowRead(uint8_t *ram, PS2Runtime *rt, uint32_t a)
    {
        if constexpr (sizeof(T) == 1)
            return ps2_guest_mem::read8_slow(ram, &g_a1SlowCtx, rt, a);
        else if constexpr (sizeof(T) == 2)
            return ps2_guest_mem::read16_slow(ram, &g_a1SlowCtx, rt, a);
        else if constexpr (sizeof(T) == 4)
            return ps2_guest_mem::read32_slow(ram, &g_a1SlowCtx, rt, a);
        else
            return ps2_guest_mem::read64_slow(ram, &g_a1SlowCtx, rt, a);
    }
    template <typename T>
    PS2_FORCE_INLINE void slowWrite(uint8_t *ram, PS2Runtime *rt, uint32_t a, T v)
    {
        if constexpr (sizeof(T) == 1)
            ps2_guest_mem::write8_slow(ram, &g_a1SlowCtx, rt, a, v);
        else if constexpr (sizeof(T) == 2)
            ps2_guest_mem::write16_slow(ram, &g_a1SlowCtx, rt, a, v);
        else if constexpr (sizeof(T) == 4)
            ps2_guest_mem::write32_slow(ram, &g_a1SlowCtx, rt, a, v);
        else
            ps2_guest_mem::write64_slow(ram, &g_a1SlowCtx, rt, a, v);
    }
    // lw rt, off(base): N1's base test
    template <typename T>
    PS2_FORCE_INLINE T ldb(uint8_t *ram, PS2Runtime *rt, uint32_t b, int32_t off)
    {
        if (PS2_N1_BASE_OK(b))
        {
            ps2_guest_mem::readWatch<T>(b + static_cast<uint32_t>(off));
            T v;
            std::memcpy(&v, (ram + b) + off, sizeof(T));
            return v;
        }
        return slowRead<T>(ram, rt, b + static_cast<uint32_t>(off));
    }
    template <typename T>
    PS2_FORCE_INLINE void stb(uint8_t *ram, PS2Runtime *rt, uint32_t b, int32_t off, T v)
    {
        if (PS2_N1_BASE_OK(b))
        {
            ps2_guest_mem::trace<T>(ram, b + static_cast<uint32_t>(off), v, "WRITE", &g_a1SlowCtx);
            std::memcpy((ram + b) + off, &v, sizeof(T));
        }
        else
            slowWrite<T>(ram, rt, b + static_cast<uint32_t>(off), v);
    }
    // an access at a full address
    template <typename T>
    PS2_FORCE_INLINE T lda(uint8_t *ram, PS2Runtime *rt, uint32_t a)
    {
        if (PS2_EXPECT(a < PS2_RAM_SIZE, 1))
        {
            ps2_guest_mem::readWatch<T>(a);
            T v;
            std::memcpy(&v, ram + a, sizeof(T));
            return v;
        }
        return slowRead<T>(ram, rt, a);
    }
    template <typename T>
    PS2_FORCE_INLINE void sta(uint8_t *ram, PS2Runtime *rt, uint32_t a, T v)
    {
        if (PS2_EXPECT(a < PS2_RAM_SIZE, 1))
        {
            ps2_guest_mem::trace<T>(ram, a, v, "WRITE", &g_a1SlowCtx);
            std::memcpy(ram + a, &v, sizeof(T));
        }
        else
            slowWrite<T>(ram, rt, a, v);
    }
}

#define A1_R8B(b, o) a1m::ldb<uint8_t>(ram, rt, (uint32_t)(b), (int32_t)(o))
#define A1_R16B(b, o) a1m::ldb<uint16_t>(ram, rt, (uint32_t)(b), (int32_t)(o))
#define A1_R32B(b, o) a1m::ldb<uint32_t>(ram, rt, (uint32_t)(b), (int32_t)(o))
#define A1_R64B(b, o) a1m::ldb<uint64_t>(ram, rt, (uint32_t)(b), (int32_t)(o))
#define A1_W8B(b, o, v) a1m::stb<uint8_t>(ram, rt, (uint32_t)(b), (int32_t)(o), (uint8_t)(v))
#define A1_W16B(b, o, v) a1m::stb<uint16_t>(ram, rt, (uint32_t)(b), (int32_t)(o), (uint16_t)(v))
#define A1_W32B(b, o, v) a1m::stb<uint32_t>(ram, rt, (uint32_t)(b), (int32_t)(o), (uint32_t)(v))
#define A1_W64B(b, o, v) a1m::stb<uint64_t>(ram, rt, (uint32_t)(b), (int32_t)(o), (uint64_t)(v))
#define A1_R8(a) a1m::lda<uint8_t>(ram, rt, (uint32_t)(a))
#define A1_R16(a) a1m::lda<uint16_t>(ram, rt, (uint32_t)(a))
#define A1_R32(a) a1m::lda<uint32_t>(ram, rt, (uint32_t)(a))
#define A1_R64(a) a1m::lda<uint64_t>(ram, rt, (uint32_t)(a))
#define A1_W8(a, v) a1m::sta<uint8_t>(ram, rt, (uint32_t)(a), (uint8_t)(v))
#define A1_W16(a, v) a1m::sta<uint16_t>(ram, rt, (uint32_t)(a), (uint16_t)(v))
#define A1_W32(a, v) a1m::sta<uint32_t>(ram, rt, (uint32_t)(a), (uint32_t)(v))
#define A1_W64(a, v) a1m::sta<uint64_t>(ram, rt, (uint32_t)(a), (uint64_t)(v))

// The PS2 FPU's operations where they are not plain C (each is the runtime's own function lift_d1's macro calls)
namespace a1f
{
    PS2_FORCE_INLINE float add(float a, float b) { return ps2FpuAdd(a, b); }             // operands aligned, result clamped
    PS2_FORCE_INLINE float sub(float a, float b) { return ps2FpuSub(a, b); }
    PS2_FORCE_INLINE float addFinite(float a, float b) { return ps2FpuAddFinite(a, b); } // lift_d1's finite form (no clamp)
    PS2_FORCE_INLINE float subFinite(float a, float b) { return ps2FpuSubFinite(a, b); }
    PS2_FORCE_INLINE float mul(float a, float b) { return PS2X_FPU_OUT(a * b); }         // result clamped
    PS2_FORCE_INLINE float divClamp(float a, float b) { return ps2FpuClamp(a / b); }
    PS2_FORCE_INLINE float sqrtClamp(float a) { return ps2FpuClamp(std::sqrt(std::fabs(a))); }
    PS2_FORCE_INLINE float div(uint32_t &fcr31, float a, float b) { return ps2FpuDivide(fcr31, a, b); } // by zero: signed max
    PS2_FORCE_INLINE float sqrt(uint32_t &fcr31, float a) { return ps2FpuSqrt(fcr31, a); }
    PS2_FORCE_INLINE int32_t cvtw(float a) { return ps2FpuConvertToWord(a); }                // saturating
    PS2_FORCE_INLINE bool lt(float a, float b) { return ps2FpuCompareKey(a) < ps2FpuCompareKey(b); }
    PS2_FORCE_INLINE bool le(float a, float b) { return ps2FpuCompareKey(a) <= ps2FpuCompareKey(b); }
    PS2_FORCE_INLINE bool eq(float a, float b) { return ps2FpuCompareKey(a) == ps2FpuCompareKey(b); }
}

// The in-run check (a check build, PS2X_A1_CHECK=1): each typed call is first compared with the lifted text on the
// same state (a1_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A1_CHECK
namespace a1ck
{
#include "typed_a1_check.inc"
}
#define A1_CHK(fn, ...) a1ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A1_CHK(fn, ...) ((void)0)
#endif
