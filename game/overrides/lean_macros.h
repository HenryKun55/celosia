// Leaner guest memory access for recompiled code (an experiment, see
// lean_hot.cpp). Included after ps2_runtime_macros.h, it redefines READ* /
// WRITE*:
//  - one compare on the fast path: an address below 32 MB is read / written
//    in place (rdram has a 16-byte tail, ps2_memory.cpp: a guest access is
//    aligned, so it never runs past the end; the old path's wrap test and
//    mask are left out);
//  - everything else (I/O, scratchpad, the uncached / kseg mirrors) in one
//    out-of-line call per access size instead of the range tests and the
//    runtime call inlined at every site;
//  - no write-watch test (g_ps2WatchWriteEnabled, PS2X_WATCH_WRITE) on stores.
// The results are the same as the old macros' for every aligned access.
#pragma once

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"

namespace ps2_lean
{
#define PS2_LEAN_SLOW __attribute__((noinline, cold))
    PS2_LEAN_SLOW uint8_t read8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_LEAN_SLOW uint16_t read16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_LEAN_SLOW uint32_t read32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_LEAN_SLOW uint64_t read64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_LEAN_SLOW __m128i read128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_LEAN_SLOW void write8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint8_t v);
    PS2_LEAN_SLOW void write16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint16_t v);
    PS2_LEAN_SLOW void write32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t v);
    PS2_LEAN_SLOW void write64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint64_t v);
    PS2_LEAN_SLOW void write128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128i v);

    template <typename T>
    static PS2_FORCE_INLINE T load(const uint8_t *rdram, uint32_t a)
    {
        T v;
        std::memcpy(&v, rdram + a, sizeof(T));
        return v;
    }
    template <typename T>
    static PS2_FORCE_INLINE void store(uint8_t *rdram, uint32_t a, T v)
    {
        std::memcpy(rdram + a, &v, sizeof(T));
    }
}

#undef READ8
#undef READ16
#undef READ32
#undef READ64
#undef READ128
#undef WRITE8
#undef WRITE16
#undef WRITE32
#undef WRITE64
#undef WRITE128

#define PS2_LEAN_READ(T, fn, addr) ([&]() -> T {                                                        \
    const uint32_t _addr = (uint32_t)(addr);                                                             \
    return PS2_EXPECT(_addr < PS2_RAM_SIZE, 1) ? ps2_lean::load<T>(rdram, _addr) : ps2_lean::fn(rdram, ctx, runtime, _addr); }())
#define PS2_LEAN_WRITE(T, fn, addr, val)                     \
    do                                                       \
    {                                                        \
        const uint32_t _addr = (uint32_t)(addr);             \
        const T _val = (T)(val);                             \
        if (PS2_EXPECT(_addr < PS2_RAM_SIZE, 1))             \
            ps2_lean::store<T>(rdram, _addr, _val);          \
        else                                                 \
            ps2_lean::fn(rdram, ctx, runtime, _addr, _val);  \
    } while (0)

#define READ8(addr) PS2_LEAN_READ(uint8_t, read8, addr)
#define READ16(addr) PS2_LEAN_READ(uint16_t, read16, addr)
#define READ32(addr) PS2_LEAN_READ(uint32_t, read32, addr)
#define READ64(addr) PS2_LEAN_READ(uint64_t, read64, addr)
#define READ128(addr) PS2_LEAN_READ(__m128i, read128, (uint32_t)(addr) & ~0xFu)
#define WRITE8(addr, val) PS2_LEAN_WRITE(uint8_t, write8, addr, val)
#define WRITE16(addr, val) PS2_LEAN_WRITE(uint16_t, write16, addr, val)
#define WRITE32(addr, val) PS2_LEAN_WRITE(uint32_t, write32, addr, val)
#define WRITE64(addr, val) PS2_LEAN_WRITE(uint64_t, write64, addr, val)
#define WRITE128(addr, val) PS2_LEAN_WRITE(__m128i, write128, (uint32_t)(addr) & ~0xFu, val)
