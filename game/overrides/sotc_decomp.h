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
// SOTC_DIFFTEST_DRAWS=1 also compares the native renderer's 3D draws of the
// two legs (native_renderer::setDrawTap; with SOTC_ASYNC_BUILDS=0).
#pragma once

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"

#if __has_include("ps2_recompiled_table.h")
#include "ps2_recompiled_table.h" // the table's bounds as constants (callGuest's lookup folds)
#endif

#include <algorithm>
#include <cstdint>
#include <cstdlib>
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

    // Counts the heap's garbage moves (copyViaSpr, sotc_overrides.cpp: blocks
    // moved down in RAM, MBs at an area change). The native hooks' memos by
    // address (packetId, decodedTexture) take a packet or texture drawn the
    // frame before as it was; after a move another one can be at that
    // address, so a memo made before the last move samples again (round 11).
    inline uint32_t g_guestMoveSerial = 0;
    // Called before a garbage move (sotc_native: the model jobs on the worker
    // use the memos by address; round 23).
    inline void (*g_beforeGuestMove)() = nullptr;
    // The game frame (sotc_native syncFrame), for in-run A/B switches.
    inline uint64_t g_gameFrame = 0;
    // The idle thread's turn (round 25, sotc_overrides.cpp): called at each
    // game frame's end on the game thread.
    void idleTurn(uint8_t *rdram, PS2Runtime *runtime);
    // The garbage pass's counts (decomp_gctick.cpp, round 14), read by the
    // [loader] line's "gc" part (SOTC_FRAME_LOG / SOTC_AUDIO_LOG): calls,
    // blocks looked at, moves, calls stopped by its clock (no block left to
    // it), bytes moved (copyViaSpr), the time check's last / largest elapsed
    // T0 ticks (2123463 is its limit, ~14.4 ms after the VBlank thread's tick).
    struct GcStats
    {
        uint64_t calls = 0, blocks = 0, moves = 0, stopped = 0, stoppedAtOnce = 0, bytes = 0;
        uint64_t budgetStops = 0, budgetGoes = 0; // the host-time budget's stops / blocks given past the clock
        int64_t lastElapsed = 0, maxElapsed = 0;
    };
    inline GcStats g_gcStats;
    // The last garbage moves made as one memmove (round 13): [src, src + len)
    // now at dst, each with the serial it took. A memo by address made before
    // a move finds its packet or texture at the new address through these
    // (packetId, decodedTexture), so an area change's moves (the same ~2 000
    // packets moved every few frames, hundreds of moves a frame) are not
    // hashed again each time.
    struct GuestMove
    {
        uint32_t src, dst, len, serial;
    };
    constexpr uint32_t kGuestMoves = 4096; // the ring's length (a power of two)
    inline std::vector<GuestMove> g_guestMoves(kGuestMoves);
    inline uint32_t g_guestMoveCount = 0; // moves made (the ring holds the last kGuestMoves)
    inline void noteGuestMove(uint32_t src, uint32_t dst, uint32_t len)
    {
        g_guestMoves[g_guestMoveCount++ & (kGuestMoves - 1u)] = {src, dst, len, g_guestMoveSerial};
    }
    // The ring sorted by destination (made again when moves were added):
    // move numbers (count - 1 - age), by dst.
    struct GuestMoveIndex
    {
        std::vector<uint32_t> byDst; // move numbers
        uint32_t builtAt = ~0u, maxLen = 0;
        uint32_t first = 0; // the oldest move number the run of recorded serials reaches
    };
    inline GuestMoveIndex g_guestMoveIndex;
    inline void buildGuestMoveIndex()
    {
        GuestMoveIndex &x = g_guestMoveIndex;
        if (x.builtAt == g_guestMoveCount)
            return;
        x.builtAt = g_guestMoveCount;
        const uint32_t n = std::min(g_guestMoveCount, kGuestMoves);
        // Only the newest moves with serials one after the other (a chunked
        // copy in between was not recorded: what came before it is not used).
        uint32_t expect = g_guestMoveSerial, used = 0;
        for (; used < n; ++used)
        {
            const GuestMove &m = g_guestMoves[(g_guestMoveCount - 1u - used) & (kGuestMoves - 1u)];
            if (m.serial != expect)
                break;
            --expect;
        }
        x.first = g_guestMoveCount - used;
        x.byDst.resize(used);
        x.maxLen = 0;
        for (uint32_t i = 0; i < used; ++i)
        {
            x.byDst[i] = x.first + i;
            x.maxLen = std::max(x.maxLen, g_guestMoves[(x.first + i) & (kGuestMoves - 1u)].len);
        }
        std::sort(x.byDst.begin(), x.byDst.end(), [](uint32_t a, uint32_t b)
        {
            const GuestMove &ma = g_guestMoves[a & (kGuestMoves - 1u)], &mb = g_guestMoves[b & (kGuestMoves - 1u)];
            return ma.dst != mb.dst ? ma.dst < mb.dst : a < b;
        });
    }
    // The newest recorded move before move number `before` whose destination
    // holds `addr` (false: none).
    inline bool guestMoveInto(uint32_t addr, uint32_t before, uint32_t &number)
    {
        const GuestMoveIndex &x = g_guestMoveIndex;
        // Moves with dst in (addr - maxLen, addr]: the last with dst <= addr, back.
        auto it = std::upper_bound(x.byDst.begin(), x.byDst.end(), addr, [](uint32_t a, uint32_t n) { return a < g_guestMoves[n & (kGuestMoves - 1u)].dst; });
        bool found = false;
        // A few back only (round 13: a scan down to maxLen walked thousands
        // a lookup once one move was MBs long, ~100 ms a frame on the Vita).
        // A move missed here costs a hash, not a wrong id.
        for (int left = 8; left > 0 && it != x.byDst.begin(); --left)
        {
            --it;
            const GuestMove &m = g_guestMoves[*it & (kGuestMoves - 1u)];
            if (addr - m.dst >= x.maxLen)
                break; // dst too far below: no later one reaches
            if (addr - m.dst < m.len && *it - x.first < before - x.first && (!found || *it - x.first > number - x.first))
            {
                number = *it;
                found = true;
            }
        }
        return found;
    }
    // Where `addr` was before each recorded move that brought it here, newest
    // first: `f(oldAddress, moveSerial)` until it returns true (at most 8 moves back).
    template <typename F>
    inline bool walkGuestMoves(uint32_t addr, F &&f)
    {
        if (!g_guestMoveCount)
            return false;
        buildGuestMoveIndex();
        uint32_t before = g_guestMoveCount, at = addr, number = 0;
        for (int step = 0; step < 8 && guestMoveInto(at, before, number); ++step)
        {
            const GuestMove &m = g_guestMoves[number & (kGuestMoves - 1u)];
            at = m.src + (at - m.dst);
            if (f(at, m.serial))
                return true;
            before = number;
        }
        return false;
    }

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
    // For a hook installed before install() (sotc_native's layer stack and
    // camera context, round 42): what the hook should call for the function
    // at that address. The registered native (its difftest checker with
    // SOTC_DIFFTEST) unless SOTC_DECOMP=0 / SOTC_DECOMP_SKIP names it or none
    // is registered (then the table's function, as lookupFunction). install()
    // then leaves that table entry to the hook.
    PS2Runtime::RecompiledFunction behindHook(PS2Runtime &runtime, uint32_t address);
    // SignalSema's uncontended case in the table (decomp_sema.cpp, round 43;
    // SOTC_SEMA_FAST=0: the stub stays). Called by install().
    void installSemaFast(PS2Runtime &runtime);
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

    // callGuest's rare parts, out of line: the callee left with pc on a
    // continuation (run on to the return), no table entry (lookupFunction),
    // SOTC_CALLGUEST_STATS's count (Mac).
    void finishGuestCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp);
    void callGuestSlow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp);
#if !defined(__vita__)
    inline bool g_callGuestStats = false; // SOTC_CALLGUEST_STATS (install)
#endif

    // The table's entry for a guest address (a constant address's slot is a
    // constant: one load).
    inline PS2Runtime::RecompiledFunction guestFunction(const PS2Runtime *runtime, uint32_t target)
    {
#if defined(PS2X_RECOMP_TABLE_BASE) && defined(PS2X_RECOMP_TABLE_SLOTS)
        (void)runtime;
        return PS2Runtime::guestFunctionIn<PS2X_RECOMP_TABLE_BASE, PS2X_RECOMP_TABLE_SLOTS>(target);
#else
        return runtime->guestFunctionAt(target);
#endif
    }

    // Calls guest code from a decompiled function as a jal would: ra and sp
    // set (sp: the original's frame at that call), run until it returns there.
    // Inline (round 42): the call was ~45 Vita instructions out of line
    // (6 000 a frame on the route, 3% of the game thread), most of it the
    // argument moves, the lookup and the run-to-return loop's set-up. The
    // callee is whatever the table holds: a native (its switch, SOTC_DECOMP_SKIP,
    // a difftest checker, SOTC_AB, a profiler wrapper all still apply) or the
    // recompiled code; a native returns to ra, so the loop is not entered.
    inline void callGuest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
#if !defined(__vita__)
        if (__builtin_expect(!fn || g_callGuestStats, 0))
#else
        if (__builtin_expect(!fn, 0))
#endif
            return callGuestSlow(rdram, ctx, runtime, target, ra, sp);
        fn(rdram, ctx, runtime);
        if (__builtin_expect(ctx->pc != ra || GPR_U32(ctx, 29) != sp, 0))
            finishGuestCall(rdram, ctx, runtime, ra, sp);
    }
    // The same with the callee's native known (a native calling another one,
    // round 42): a direct call while the table holds it, else as callGuest.
    inline void callNative(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, PS2Runtime::RecompiledFunction known, uint32_t ra,
                           uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
#if !defined(__vita__)
        if (__builtin_expect(fn != known || g_callGuestStats, 0))
#else
        if (__builtin_expect(fn != known, 0))
#endif
            return callGuest(rdram, ctx, runtime, target, ra, sp);
        known(rdram, ctx, runtime);
        if (__builtin_expect(ctx->pc != ra || GPR_U32(ctx, 29) != sp, 0))
            finishGuestCall(rdram, ctx, runtime, ra, sp);
    }
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
        inline __m128 mulN(__m128 a, __m128 b) { return ps2VuResult(_mm_mul_ps(a, b)); }
        inline __m128 addN(__m128 a, __m128 b) { return ps2VuResult(_mm_add_ps(a, b)); }
        inline __m128 subN(__m128 a, __m128 b) { return ps2VuResult(_mm_sub_ps(a, b)); }
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

    // The rounds' subsystem switches (round 44: one table, so the console's
    // A/B can turn them off and on in one run: SOTC_AB_FLAG's camcoll,
    // collprobe, motion, leaves, hotlist, hotlist44, hotlist46, hotlist47; sotc_native's
    // abSwitches). Each is read from its env variable at first use (the
    // Vita's env.txt comes after static init), "0" off; a native checks its
    // switch at entry, so a change takes effect at the next call.
    enum Switch : int
    {
        kCamColl,
        kCollProbe,
        kMotion,
        kLeaves,
        kHotList,
        kHotList44,
        kHotList46,
        kHotList47,
        kSwitchCount
    };
    inline const char *const kSwitchEnv[kSwitchCount] = {"SOTC_CAMCOLL", "SOTC_COLLPROBE", "SOTC_MOTION", "SOTC_LEAVES", "SOTC_HOTLIST", "SOTC_HOTLIST44", "SOTC_HOTLIST46", "SOTC_HOTLIST47"};
    inline signed char g_switch[kSwitchCount] = {-1, -1, -1, -1, -1, -1, -1, -1}; // -1: not read yet
    inline bool switchOn(int which)
    {
        signed char v = g_switch[which];
        if (__builtin_expect(v < 0, 0))
        {
            const char *e = std::getenv(kSwitchEnv[which]);
            v = !(e && e[0] == '0');
            g_switch[which] = v;
        }
        return v != 0;
    }
    inline void setSwitch(int which, bool on) { g_switch[which] = on ? 1 : 0; }

    // Round 39's natives (the camera's collision path: its wrappers, filters
    // and the segment's matrix, decomp_camcoll.cpp / decomp_collision.cpp /
    // decomp_quat.cpp). SOTC_CAMCOLL=0 puts the recompiled code back for all
    // of them (read at first use: the Vita's env.txt comes after static init).
    inline bool camCollOn() { return switchOn(kCamColl); }
    // Round 40's natives (the scripts' collision callback, the id filters
    // left as guest code, the character's slope / water probes;
    // decomp_collision.cpp). SOTC_COLLPROBE=0 puts the recompiled code back
    // for all of them (read at first use).
    inline bool collProbeOn() { return switchOn(kCollProbe); }
    // Round 41's natives (the motion accessors and the pose's root,
    // decomp_motion.cpp / decomp_skeleton.cpp; the character's geometry
    // subsystem, decomp_charageo.cpp). SOTC_MOTION=0 puts the recompiled
    // code back for all of them (read at first use).
    inline bool motionOn() { return switchOn(kMotion); }
    // Round 42's natives (the small leaves the natives call most, the layer
    // stack and the camera context behind sotc_native's hooks, the 2D sprite,
    // the collision work flush and the IK attribute filter; decomp_leaves.cpp,
    // decomp_collision.cpp). SOTC_LEAVES=0 puts the recompiled code back for
    // all of them (read at first use).
    inline bool leavesOn() { return switchOn(kLeaves); }
    // Round 43's natives (the next functions down the game thread's profile:
    // the slope probe, the camera record fill, the lights' layout pass, the
    // texture resolve's work path, libm's asinf ...; decomp_hot43*.cpp).
    // SOTC_HOTLIST=0 puts the recompiled code back for all of them (read at
    // first use).
    inline bool hotListOn() { return switchOn(kHotList); }
    // Round 44's natives (the next functions down the profile: the clip
    // keep's full test, the collision DB lookups, the Euler quaternion, the
    // condition walkers ...; decomp_hot44*.cpp). SOTC_HOTLIST44=0 puts the
    // recompiled code back for all of them (read at first use).
    inline bool hotList44On() { return switchOn(kHotList44); }
    // Round 46's natives (the next functions down the profile and the
    // colossus fight's: decomp_hot46*.cpp). SOTC_HOTLIST46=0 puts the
    // recompiled code back for all of them (read at first use).
    inline bool hotList46On() { return switchOn(kHotList46); }
    // Round 47's natives (the next functions down the game thread's profile:
    // decomp_hot47*.cpp). SOTC_HOTLIST47=0 puts the recompiled code back for
    // all of them (read at first use).
    inline bool hotList47On() { return switchOn(kHotList47); }
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
