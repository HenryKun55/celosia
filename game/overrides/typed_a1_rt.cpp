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
bool g_a5Typed = true; // round A5 (typed_a5_rt.h): the switch a5typed, likewise
#include "typed_a5_rt.h"
#if PS2X_A5_CHECK
A3Stats g_a5s; // round A5's check build: its adapters' test and deferred checkpoints (A3's counters)
uint64_t g_a5Reloc = 0; // adapters stopped by a relocated site's word (A5_JAL)
#endif
#include <cstdio>
#include <cstdlib>
namespace a5
{
    // a jump table's value outside the recompiler's case list (lift_d1's text would jump out to it; never in this game)
    void badJump(uint32_t site, uint32_t target)
    {
        std::fprintf(stderr, "[a5] jump table at 0x%08x: target 0x%08x outside its case list\n", site, target);
        std::abort();
    }
}
bool g_a6Typed = true; // round A6 (typed_a6_rt.h): the switch a6typed, likewise
#include "typed_a6_rt.h"
#if PS2X_A6_CHECK
A3Stats g_a6s; // round A6's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
A6Brk g_a6Brk = {0u, 0u, 0u, 0u};
bool g_a7Typed = true; // round A7 (typed_a7_rt.h): the switch a7typed, likewise
#include "typed_a7_rt.h"
#if PS2X_A7_CHECK
A3Stats g_a7s; // round A7's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
bool g_a8Typed = true; // round A8 (typed_a8_rt.h): the switch a8typed, likewise
#include "typed_a8_rt.h"
#if PS2X_A8_CHECK
A3Stats g_a8s; // round A8's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
bool g_a9Typed = true; // round A9 (typed_a9_rt.h): a9typed and the quat122 / geo126 default paths, set once a frame
uint64_t g_a9Outside[8] = {}; // round A9: the cores' calls outside their native's default path (typed_a9_ext.h)
#include "typed_a9_rt.h"
#if PS2X_A9_CHECK
A3Stats g_a9s; // round A9's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
bool g_a10Typed = true;    // round A10 (typed_a10_rt.h): a10typed and A9's flag, set once a frame
bool g_a10CamHook = false; // round A10 (typed_a10_note.h): the camera hook holds iosgluGetCameraContext's slot over its native
// round A10 (typed_a6_rt.h a6Is): the slot holders taken as their typed functions while a10typed is on, one bit each
// (A10H_CAM the camera hook, A10H_PARENT GetMotionOptionNodeParentId's, A10H_LOCALQP GetCharacterClassOptionNodeLocalQuaternionPos's,
// A10H_PLANE GetCollisionDBPlane's, A10H_ATTR GetCollisionDBAttributeAdr's native); SOTC_A10HOLD=<mask> overrides (sotc_native)
uint32_t g_a10HoldMask = A10H_DEFAULT;
#include "typed_a10_rt.h"
#if PS2X_A10_CHECK
A3Stats g_a10s; // round A10's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
namespace a6
{
    // the breaks a typed call took, on the adapter's context after the call: handleBreak at the first site and, when more
    // were taken, at the last (each sets EPC to its site, Cause's code, Status.EXL and clears the delay-slot flag, so the
    // first and the last leave the context as every break in order would); ctx->pc is set to the return after this
    void applyBrks(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const A6Brk b = g_a6Brk;
        g_a6Brk = {0u, 0u, 0u, 0u};
        // round A14 (BRK14): a site with bit 0 set is a break in a likely branch's delay slot (site & ~1; the branch 4 bytes
        // before): lift_d1's text there sets pc, in_delay_slot and branch_pc before handleBreak (EPC = the branch, Cause.BD)
        const auto one = [&](uint32_t site) {
            if (site & 1u)
            {
                ctx->pc = site & ~1u;
                ctx->in_delay_slot = true;
                ctx->branch_pc = (site & ~1u) - 4u;
                runtime->handleBreak(rdram, ctx);
                ctx->in_delay_slot = false;
                return;
            }
            ctx->pc = site;
            runtime->handleBreak(rdram, ctx);
        };
        one(b.first);
        if (b.n > 1u)
            one(b.last);
        if (b.slot) // the last slot break's branch_pc stays (a plain break after it does not set branch_pc)
            ctx->branch_pc = (b.slot & ~1u) - 4u;
    }
}

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
