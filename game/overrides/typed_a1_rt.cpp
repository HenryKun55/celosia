// typed_a1_rt.cpp: round A1's typed functions' runtime side (typed_a1_rt.h; docs/decomp_a1.md): the switch's flag and
// the memory slow path (an address outside main RAM: scratchpad, I/O, the mirrors), through the runtime's own
// functions.
#include "typed_a1_rt.h"

extern "C" uint8_t *g_ps2gmRdram;
extern "C" PS2Runtime *g_ps2gmRuntime;

bool g_a1Typed = true; // sotc_native.cpp sets it from the switch a1typed at install and once a frame
bool g_a2Typed = true; // round A2 (typed_a2_rt.h): the switch a2typed, likewise
#include "typed_a2_rt.h"
#if PS2X_A2_CHECK
A2GoStats g_a2Go; // round A2's check build: the adapters' test, counted
#endif
bool g_a3Typed = true; // round A3 (typed_a3_rt.h): the switch a3typed, likewise
#include "typed_a3_rt.h"
#if PS2X_A3_CHECK
A3Stats g_a3; // round A3's check build: the adapters' test and the deferred checkpoints, counted
A3Stats *g_a3cur = &g_a3;
#endif
bool g_a4Typed = true; // round A4 (typed_a4_rt.h): the switch a4typed, likewise
#include "typed_a4_rt.h"
#if PS2X_A4_CHECK
A3Stats g_a4s; // round A4's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif

namespace a1m
{
    // The slow path's context: ps2_guest_mem's functions take one to signal an address error (never taken by this
    // game) and for the Mac's store trace. A static one, so the typed functions need none.
    R5900Context g_a1SlowCtx{};
    R5900Context *noCtx() { return &g_a1SlowCtx; }
    // The lifted code's slow path on the Vita is the trampoline (PS2X_GUEST_MEM_TRAMP), which takes RAM and the runtime
    // from these globals, not from its caller: the typed code does the same there, so both reach the same memory. Elsewhere
    // (the Mac) the plain slow path with the caller's runtime, as the lifted code's.
#if PS2X_GUEST_MEM_TRAMP
#define A1_ENV() (ram = g_ps2gmRdram, rt = g_ps2gmRuntime)
#else
#define A1_ENV() ((void)0)
#endif
    uint8_t slowRead8(uint8_t *ram, PS2Runtime *rt, uint32_t a) { A1_ENV(); return ps2_guest_mem::read8(ram, noCtx(), rt, a); }
    uint16_t slowRead16(uint8_t *ram, PS2Runtime *rt, uint32_t a) { A1_ENV(); return ps2_guest_mem::read16(ram, noCtx(), rt, a); }
    uint32_t slowRead32(uint8_t *ram, PS2Runtime *rt, uint32_t a) { A1_ENV(); return ps2_guest_mem::read32(ram, noCtx(), rt, a); }
    uint64_t slowRead64(uint8_t *ram, PS2Runtime *rt, uint32_t a) { A1_ENV(); return ps2_guest_mem::read64(ram, noCtx(), rt, a); }
    void slowWrite8(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint8_t v) { A1_ENV(); ps2_guest_mem::write8(ram, noCtx(), rt, a, v); }
    void slowWrite16(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint16_t v) { A1_ENV(); ps2_guest_mem::write16(ram, noCtx(), rt, a, v); }
    void slowWrite32(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint32_t v) { A1_ENV(); ps2_guest_mem::write32(ram, noCtx(), rt, a, v); }
    void slowWrite64(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint64_t v) { A1_ENV(); ps2_guest_mem::write64(ram, noCtx(), rt, a, v); }
}
