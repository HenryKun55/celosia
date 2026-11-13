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
#include <cstdio>
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
        // Round 48: a coroutine's code (a sub-thread body or a function that
        // can sleep under one: the scripts' frame end, scrRun ...). Never put
        // behind a difftest checker (a check would run the other context
        // nested); checked through isysWakeupSubThread with
        // SOTC_DIFFTEST_SWITCH (sotc_decomp.cpp). `resume`: the entry is a
        // label inside the function (a return address after a call that can
        // sleep) where the sub-thread is resumed by pc.
        bool coroutine = false, resume = false;
        // Decomp D1: a function tools/lift_d1.py lifted (SOTC_LIFTD1): put in place only while that switch is on; an
        // A/B window that turns it off puts the recompiled function back in the table (no check in the function).
        bool lift = false;
        // The generated function a lifted one stands for: installed only while the table still holds it (a hook
        // installed before, a sysnative / Vita runtime override, keeps its place).
        PS2Runtime::RecompiledFunction original = nullptr;
        // Decomp D2: the switch a lifted function belongs to (-1: kLiftD1, D1's; kLiftD2 for decomp_liftd2_*.cpp,
        // kLiftD3 for decomp_liftd3_*.cpp, kLiftD4 for decomp_liftd4_*.cpp, kLiftD5 for decomp_liftd5_*.cpp, kLiftD6 for
        // decomp_liftd6_*.cpp).
        int liftSwitch = -1;
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
    // Round 65: the camera context GetCameraContext last returned (sotc_native's
    // hook; +0x140 the camera's position), for the Quality menu's cuts.
    inline uint32_t g_cameraContextAddr = 0;
    // Round 61 (SOTC_MDS_LANE): the VU0 CLIP register while modelDispState
    // runs on its lane (sotc_native mdslane). The register holds the last
    // four judgements (6 bits each); the lane's walk leaves it as its last
    // tests do, so on the game thread it is pending until the lane is joined.
    // Code that only shifts judgements in and decides on its own ones
    // (clipByMinMaxCube's corners) records them here instead (`known`, the
    // last `shifts` judgements, newest lowest); anything that reads the
    // register calls clipNeeded first (the lane joined, the true value put
    // in the context). `tracking` (SOTC_MDS_LANE_VERIFY): the same records
    // kept while nothing is pending, to check them against the register.
    struct LazyClip
    {
        R5900Context *ctx = nullptr;
        bool pending = false, tracking = false;
        uint32_t known = 0, shifts = 0;
    };
    inline LazyClip g_lazyClip;
    inline void (*g_clipJoin)() = nullptr;
    // Round 64: the offload thread's work joined (decomp_tex64.cpp; sotc_native's
    // hook joins call it: the frame's end, the heap's moves, the 2D / mesh hooks).
    inline void (*g_offloadJoin)(int site) = nullptr;
    inline bool clipPending(const R5900Context *ctx) { return g_lazyClip.pending && g_lazyClip.ctx == ctx; }
    inline void clipNeeded(const R5900Context *ctx)
    {
        if ((g_lazyClip.pending || g_lazyClip.tracking) && g_lazyClip.ctx == ctx) [[unlikely]]
            g_clipJoin();
    }
    // One judgement shifted in by code that decides on its own judgements
    // only: true when it was recorded (the register is pending or tracked).
    inline void clipShift(const R5900Context *ctx, uint32_t judgement)
    {
        if ((g_lazyClip.pending || g_lazyClip.tracking) && g_lazyClip.ctx == ctx)
        {
            g_lazyClip.known = ((g_lazyClip.known << 6) | judgement) & 0xFFFFFFu;
            ++g_lazyClip.shifts;
        }
    }
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
                  bool once = false, bool ab = false, bool coroutine = false, bool resume = false, bool lift = false,
                  PS2Runtime::RecompiledFunction original = nullptr, int liftSwitch = -1)
        {
            registry().push_back({address, name, native, returnsFloat, switches, once, ab, coroutine, resume, lift, original, liftSwitch});
        }
    };

    // Decomp D4 (test): the alternative copies of lifted functions (SOTC_DECOMP_LIFT_ALT), by guest address.
    std::vector<std::pair<uint32_t, PS2Runtime::RecompiledFunction>> &liftAlts();
    struct LiftAltRegistrar
    {
        LiftAltRegistrar(uint32_t address, PS2Runtime::RecompiledFunction fn) { liftAlts().push_back({address, fn}); }
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
    // Round 139 (sem139, decomp_sema.cpp): GetThreadId / PollSema for the natives, the scheduler's fast paths when they apply.
    void getThreadId139(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void pollSema139(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
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

    // GAMECORE.XFF has started (sotc_overrides.cpp's entry hook). Before it,
    // module placement depends on the boot's guest timing: natives that skip
    // EE cycles there move the modules (decomp_reloc.cpp).
    bool gamecoreEntered();

    // callGuest's rare parts, out of line: the callee left with pc on a
    // continuation (run on to the return), no table entry (lookupFunction),
    // SOTC_CALLGUEST_STATS's count (Mac).
    void finishGuestCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp);
    void callGuestSlow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp);
#if !defined(__vita__)
    inline bool g_callGuestStats = false; // SOTC_CALLGUEST_STATS (install)
#endif

    // Round 108: the two threads natives run on: 0 the game thread (and any
    // other EE thread: they never run beside it), 1 round 91's guest worker
    // (its runtime shell). A native's host caches are one per lane, and its
    // memos (answers kept across entries) are lane 0's only: on the worker
    // they are passed over (the computation runs), so a job can neither read
    // nor fill the game thread's kept answers.
    inline int lane(const PS2Runtime *runtime) { return ps2_worker::isShell(runtime) ? 1 : 0; }
    inline bool onWorkerLane(const PS2Runtime *runtime) { return ps2_worker::isShell(runtime); }

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
        if (ps2_worker::isShell(runtime)) // round 91: on the worker thread (sotc_worker.cpp)
        {
            ps2_worker::g_hooks.call(runtime, rdram, ctx, target, ra);
            return;
        }
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
#if !defined(__vita__)
        if (__builtin_expect(!fn || g_callGuestStats, 0))
#else
        if (__builtin_expect(!fn, 0))
#endif
            return callGuestSlow(rdram, ctx, runtime, target, ra, sp);
        fn(rdram, ctx, runtime);
        runtime->checkTransfer(); // a thread switch below: off the stack (round 52)
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
        if (ps2_worker::isShell(runtime)) // round 91: on the worker thread (sotc_worker.cpp)
        {
            ps2_worker::g_hooks.call(runtime, rdram, ctx, target, ra);
            return;
        }
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
#if !defined(__vita__)
        if (__builtin_expect(fn != known || g_callGuestStats, 0))
#else
        if (__builtin_expect(fn != known, 0))
#endif
            return callGuest(rdram, ctx, runtime, target, ra, sp);
        known(rdram, ctx, runtime);
        runtime->checkTransfer();
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

    // ---- coroutines (round 48: the script system's sub-threads) -------------
    // The game's sub-threads (decomp_subthread.cpp) are coroutines: a sleep
    // (isysSleepSubThread) saves the sub-thread's registers on its stack, loads
    // the waker's and returns into isysWakeupSubThread's caller; the next wake
    // calls the sub-thread at the return address after its sleep. The
    // recompiled code unwinds through every call between (each returns with pc
    // on the other context) and is entered again by pc at that label.
    // sotc_decomp::callGuest cannot be used for a call that can sleep: its
    // run-to-return loop would go on running the waker's code nested inside
    // the sleeping sub-thread. A native that calls such code uses callMaySleep
    // and returns at once when it says the sub-thread slept; the sub-thread is
    // resumed at the return label, in the native's resume entry
    // (SOTC_DECOMP_RESUME) or in the recompiled function (the frames and
    // registers are the original's, so either can go on from there).
    //
    // The waker's frame (0x2c0 bytes: decomp_subthread's kMainSp at 0x270bb8)
    // ends at the wake's sp - 0x10: a call that comes back with sp there and
    // pc not on its return has switched to the waker.
    inline bool onWakerContext(const uint8_t *rdram, const R5900Context *ctx)
    {
        uint32_t main;
        std::memcpy(&main, rdram + 0x00270BB8u, 4);
        return main != 0u && GPR_U32(ctx, 29) == main + 0x2D0u;
    }
    // A guest call that can sleep: true when it came back to ra at sp (the
    // continuations run on, as callGuest's loop), false when the sub-thread
    // slept (the context is the waker's: the caller must return at once and
    // leave ctx as it is) or the dispatcher must take over (no function).
    bool callMaySleepRest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp);
    inline bool callMaySleep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        if (ps2_worker::isShell(runtime)) // round 91: the worker thread has no sub-threads
            return ps2_worker::g_hooks.call(runtime, rdram, ctx, target, ra);
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
        if (__builtin_expect(!fn, 0))
            return false; // the dispatcher looks it up (pc is on it)
        fn(rdram, ctx, runtime);
        runtime->checkTransfer();
        if (__builtin_expect(ctx->pc == ra && GPR_U32(ctx, 29) == sp, 1))
            return true;
        return callMaySleepRest(rdram, ctx, runtime, ra, sp);
    }
    // The same with the callee's native known: a direct call while the table holds it.
    inline bool callNativeMaySleep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, PS2Runtime::RecompiledFunction known,
                                   uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        if (ps2_worker::isShell(runtime)) // round 91
            return ps2_worker::g_hooks.call(runtime, rdram, ctx, target, ra);
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
        if (__builtin_expect(fn != known, 0))
            return callMaySleep(rdram, ctx, runtime, target, ra, sp);
        known(rdram, ctx, runtime);
        runtime->checkTransfer();
        if (__builtin_expect(ctx->pc == ra && GPR_U32(ctx, 29) == sp, 1))
            return true;
        return callMaySleepRest(rdram, ctx, runtime, ra, sp);
    }
    // A tail jump (j) that can sleep: the callee returns to the jumper's
    // caller (ra and sp as the jump leaves them); nothing runs after it here.
    inline void tailMaySleep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        if (ps2_worker::isShell(runtime)) // round 91
            return ps2_worker::g_hooks.jump(runtime, rdram, ctx, nullptr, target);
        const PS2Runtime::RecompiledFunction fn = guestFunction(runtime, target);
        if (__builtin_expect(fn != nullptr, 1))
            fn(rdram, ctx, runtime);
    }

    // The callee-saved registers as the original holds them (round 48's
    // helper; round 47's rule): a callee saves s0-s7 / fp into its frame and a
    // later callee can read such a stale slot as an uninitialised local, so
    // at every guest call they must hold the original's values. SFrame names
    // the prologue's save slots (offsets from the frame's sp, -1: not saved):
    // save() stores the incoming values there as the prologue does, set()
    // gives a register the value the code puts in it, restore() reloads the
    // saved ones as the epilogue does.
    struct SFrame
    {
        uint8_t *rdram;
        R5900Context *ctx;
        uint32_t sp;
        int16_t off[9]; // s0..s7, fp
        static constexpr int kFp = 8;
        static int reg(int i) { return i == kFp ? 30 : 16 + i; }
        void save() const
        {
            for (int i = 0; i < 9; ++i)
                if (off[i] >= 0)
                {
                    const uint64_t v = GPR_U64(ctx, reg(i));
                    std::memcpy(rdram + ((sp + static_cast<uint32_t>(off[i])) & 0x01FFFFFFu), &v, 8);
                }
        }
        void set(int i, uint64_t v) const { SET_GPR_U64(ctx, reg(i), v); }
        void setS32(int i, uint32_t v) const { SET_GPR_U64(ctx, reg(i), static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v)))); }
        void restore() const
        {
            for (int i = 0; i < 9; ++i)
                if (off[i] >= 0)
                {
                    uint64_t v;
                    std::memcpy(&v, rdram + ((sp + static_cast<uint32_t>(off[i])) & 0x01FFFFFFu), 8);
                    SET_GPR_U64(ctx, reg(i), v);
                }
        }
    };

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
        kScriptVm,
        kIsys,
        kHotList48,
        kFastNative,
        kFastNative2,
        kFastNative3,
        kParticles62, // round 62: the particle geodisps' clip pass (decomp_ptcl62)
        kColl63,      // round 63: the collision leaf's plane test first (decomp_collision)
        kTex64,       // round 64: texReset's texture loop on the offload thread (decomp_tex64)
        kWalk67,      // round 67: the per-object walks' lines asked for ahead (decomp_prefetch, decomp_walk, sotc_native, decomp_ptcldraw, decomp_collision)
        kPref69,      // round 69: the walk's prefetch records deduplicated through a hash (decomp_prefetch)
        kColl71,      // round 71: the root dynamics' collision filters natively, the finder's leaf / duplicate test (decomp_collision)
        kCharLod78,   // round 78: off-screen characters' geometry at a lower rate through the game's own view interval (decomp_charlod78)
        kPtcl79,      // round 79: off-screen particle systems stepped at a lower rate with their time kept, not drawn (decomp_hot47e, decomp_walk)
        kSeg80,       // round 80: the segment tests' answers reused while the segment stays within 1 cm, for at most 5 frames (decomp_collision)
        kPose82,      // round 82: the characters' pose (GetMotionMatrix) and bone passes for the A9, not as VU0 code (decomp_pose82)
        kWalk83,      // round 83: the display walk's light colours kept while their inputs stay, the layout objects' clip as plain A9 code (decomp_walk83)
        kPtcl83,      // round 83: the particle manager's per-system round trips as host code (decomp_hot47e); on by default since round 89
        kProbe84,     // round 84: the world probes that need no per-frame answer (the AI path's segment tests at path scale, the camera's water search) (decomp_collision, decomp_probe84)
        kFind84,      // round 84 (measuring, off by default): the collision finder's candidate surfaces of a still object kept while the query stays within 3 cm (decomp_hot47j)
        kGround85,    // round 85: Wander's ground step's near surfaces in a host array (decomp_ground85)
        kPtcl89,      // round 89: the particle emitters' frame without its leaf round trips (the camera context, the effect's name and step kind, as host code; decomp_ptclupdate)
        kPtcl91,      // round 91: the particle systems' step on the guest worker thread beside the display walk's group pass (sotc_worker, decomp_hot47e, decomp_walk)
        kJoin95,      // round 95: with ptcl91, the particle geodisps' clip in the job and the walk's join inside the light pass (decomp_walk, decomp_light, decomp_hot47e)
        kDraw95,      // round 95: with join95, the walk's join moved into its draw loop (the particle geodisps put in the drawn list when the job is done or their turn comes; decomp_walk)
        kEarly95,     // round 95 (default off): with ptcl91, the particle manager (and its job's hand-over) run before the camera manager (decomp_hot47e)
        kObj96,       // round 96: small creatures the camera cannot see woken once in 4 frames by the script pass (decomp_obj96, decomp_hot44d)
        kSel96,       // round 96: ground85's kept surfaces tested sphere and plane first, the tree path after (exact; decomp_hot47j)
        kSel97,       // round 97: sel96's sphere and plane tests four surfaces at a time in NEON (exact; decomp_hot47j)
        kLinks97,     // round 97: charaLinks' second scan from the first entry it acts on, noted by the first scan (exact; decomp_chara)
        kPath97,      // round 97: execPointListSort's area search once an area, not once a point (exact; decomp_pointsort)
        kPlan97,      // round 97: the model plans' map lookup through a direct-mapped table of its nodes (exact; sotc_native)
        kWalk101,     // round 101: the segment test's tree walk as one loop, the segment's ends in host memory (exact; decomp_collision)
        kSkin101,     // round 101: ClipCollisionDirectCallBack's skinned surfaces: the segment's matrix clamped once, the plain rejection in NEON (exact; decomp_collision)
        kLeaf101,     // round 101: the leaf's two matrices clamped once and kept between leaves while no guest code runs (exact; decomp_collision)
        kObj101,      // round 101: ClipCollisionDirectCallBack's objects, parts and surfaces: the segment's matrix clamped once while no guest code runs (exact; decomp_collision)
        kSw102,       // round 102: the sub-threads' register frames saved / loaded by inline 16-byte copies, the frame checks inlined (exact; decomp_subthread)
        kPtcl103,     // round 103: particleDraw's ptclman packets (for the PS2's VU1 only) not built: the renderer's job made from their inputs (decomp_ptcldraw, sotc_native)
        kSolid103,    // round 103: the solid models' draw without the PS2's VU1 matrix blocks no native code reads (decomp_walk)
        kGrp106,      // round 106: the frame start's group fix-up (013448C0) with its pure leaf calls inline (exact; decomp_hot44c)
        kOan106,      // round 106: three of the object animations' key evaluators (010AF2A0, 010AF4D8, 010B06A8) native (exact but no faster: default off, SOTC_OAN106=1; decomp_hot46h)
        kAi106,       // round 106: the small animals' leader search (013E1520) and helpers as A9 code (exact; decomp_ai106)
        kColl115,     // round 115: the segment test's walk and leaves on host-format collision data (coll115.h; seg80's reuse off with it)
        kPkt117,      // round 117: the other particle drawers' ptclman packets not built (0113DBF8, 0113E6C8, 0113DEA0; exact; decomp_hot46j, sotc_native)
        kGlv117,      // round 117: the immediate-mode quads' four vertices a glBegin in one pass (the heat haze strips, 01142350's particle quads; exact; decomp_walk)
        kDraw117,     // round 117: the particle geodisps' draw chain native (disp_particle_geodisp .. disp_particle_emitter; exact; decomp_ptcl117)
        kLeaf117,     // round 117: the emitter frame's leaves native and called straight (exact; decomp_ptclupdate, decomp_hot47e)
        kSym120,      // round 120: the module system's symbol resolution (sheetMgrResolve, the imports' retry, the hash lookups) as A9 code (exact; decomp_sym120)
        kHeap120,     // round 120: the game heap's allocator (malloc's block search and split, the free lists) as A9 code (exact; decomp_heap120)
        kFlush119,    // round 119: FlushCollisionWorkByWorkStruct passes over empty modified lists (exact; decomp_collision)
        kWalk119,     // round 119: coll115's walk pushes a node's second side only when the segment reaches it (exact; coll115.h)
        kPart119,     // round 119: transform() / matrixProduct() without the row clamps when every lane is small (exact; decomp_collision)
        kView119,     // round 119: the finder lists' surfaces as host contact sets (plane addresses), the clip / keep loops' plane test from them (exact; coll119.h)
        kPtcl123,     // round 123: each particle's step in plain host floats, called straight from the emitter's loop (port, not exact; decomp_particles)
        kBlend123,    // round 123: the emitter parameter blend in plain host floats (port, not exact; decomp_hot47e)
        kSeg123,      // round 123: a segment test's answer kept while its ends stay within 1 mm and nothing it can hit moved (validated memo; coll123.h)
        kGaze122,     // round 122: the characters' gaze as host code on decoded keys (not bit-exact: same to the eye; decomp_char122)
        kCloth122,    // round 122: the cloths' step / constraint / velocity passes as one host loop, the reins' walks in plain NEON (not bit-exact; decomp_char122, decomp_bridle)
        kQuat122,     // round 122: the boot ELF's quaternion / arc helpers as plain float code (not bit-exact; decomp_char122)
        kKeys122,     // round 122: the motions' per-frame keys as host code (not bit-exact; decomp_char122)
        kFin125,      // round 125: the finder clips on host contact sets with whole triangles (round 135: exact, every kept surface validated against what ClipCollisionDB reads; coll125.h)
        kObj125,      // round 125: ClipCollisionDirectCallBack's object / part loop in host locals and plain floats (port, not exact; decomp_collision)
        kScroll127,   // round 127: the model hooks' texture scroll row addresses kept across frames per plan, checked by their chains (exact; sotc_native)
        kGeo126,      // round 126: the geometry pointers' position / quaternion getters and their quaternion helpers as host code (port; decomp_cam126)
        kCtl126,      // round 126: the characters' control helpers (movement history, the y turn, the root track's key, the option nodes' getters) as host code (port; decomp_cam126, decomp_hot47a)
        kImm128,      // round 128: the immediate-mode quads' GS pairs straight to the native 2D assembler, no guest GIF packet / glBegin / glEnd (same draws; decomp_walk, sotc_native)
        kTexr128,     // round 128: texreset's list from a host index (exact; default off: slower in Vita3K, for the console's A/B; decomp_texreset)
        kPf134,       // round 134: cache lines asked for ahead (PLD) where the console waits on memory: the sub-threads' register frames, the segment tests' next objects (exact; decomp_subthread, decomp_collision)
        kClip134,     // round 134: the immediate quads' vertex kicks (quad117) record their CLIP judgements (clipShift) instead of joining the model lane: they decide on their own judgements only (exact; decomp_walk)
        kPose136,     // round 136: GetMotionMatrix's bones kept per buffer set; a bone whose inputs and parent are the same bits takes its kept results (exact; default off: no faster in Vita3K, for the console's A/B; decomp_pose82)
        kKey136,      // round 136: GetMotionIntFrame's keys decoded as host code (round 122's decodeKey), no curveEval guest calls (not bit-exact: keys122's tolerance; decomp_motionframe, decomp_char122)
        kEuler136,    // round 136: GetEulerYXZFromQuaternion in plain floats (not bit-exact: quat122's tolerance; decomp_quat, decomp_char122)
        kPf135,       // round 135: cache lines asked for ahead in the finder leaf's boundary scan (the next surfaces' records and flags) and obj125's part loop (the next part's record and matrix) (exact; decomp_collision)
        kFin138,      // round 138: fin125's validation reads the chain's links at their kept addresses, all asked for at once (PLD), the next entry's ahead (exact; coll125.h)
        kSub139,      // round 139: the sub-threads' register frames keep only the registers that live across the switch (the ABI's callee-saved, v0's lo1, the waker's a0-a3) (port; decomp_subthread)
        kRnd139, // round 139: iosGetRandom's VU0 R steps in a host register (exact; decomp_r139)
        kGc139, // round 139: the garbage pass's blocks with nothing to move walked in host locals (exact; decomp_gctick)
        kSort140,  // round 140: the display walk's sort lists cleared / scanned as host words (exact; decomp_walk)
        kQuad140,  // round 140: the strips' quads (quad117) with their per-vertex helpers inlined (exact; decomp_walk)
        kClip140,  // round 140: the walk's layout objects' ClipLayoutObjModel run ahead on the walk helper threads (exact; decomp_walk)
        kPf141,    // round 141: the characters' noted memory asked for (PLD) one script-pass segment ahead on a helper thread (prefetch only; decomp_pf141)
        kPfc141,   // round 141: pf141's notes from the collision natives too (the objects / parts / kept surfaces each segment's tests read; prefetch only)
        kLiftD1,   // decomp D1: the functions tools/lift_d1.py lifted to host locals (exact: the recompiler's semantics; decomp_liftd1_*.cpp)
        kLiftD2,   // decomp D2: the next batch tools/lift_d1.py lifted, by game-thread time (exact; decomp_liftd2_*.cpp)
        kLiftD3,   // decomp D3: D2's call-heavy functions lifted with the 64-bit entry-value fix (exact; decomp_liftd3_*.cpp)
        kLiftD4,   // decomp D4: the next batch, lifted with the callee summaries (exact; decomp_liftd4_*.cpp)
        kLiftD5,   // decomp D5: the next batch, functions the Mac runs reach (exact; decomp_liftd5_*.cpp)
        kLiftD6,   // decomp D6: functions that may block or switch threads, checked with the switch-aware difftest (exact; decomp_liftd6_*.cpp)
        kLiftAlt,  // decomp D4 (test): off puts each lifted function's alternative copy (SOTC_DECOMP_LIFT_ALT: another lifter
                   // version of the same function, decomp_liftalt_*.cpp in test builds only) in its place: an A/B of two lifters
        kSwitchCount
    };
    inline const char *const kSwitchEnv[kSwitchCount] = {"SOTC_CAMCOLL", "SOTC_COLLPROBE", "SOTC_MOTION", "SOTC_LEAVES", "SOTC_HOTLIST", "SOTC_HOTLIST44", "SOTC_HOTLIST46", "SOTC_HOTLIST47",
                                                         "SOTC_SCRIPTVM", "SOTC_ISYS", "SOTC_HOTLIST48", "SOTC_FASTNATIVE", "SOTC_FASTNATIVE2", "SOTC_FASTNATIVE3", "SOTC_PTCL62", "SOTC_COLL63", "SOTC_TEX64", "SOTC_WALK67", "SOTC_PREF69", "SOTC_COLL71", "SOTC_CHARLOD78", "SOTC_PTCL79", "SOTC_SEG80", "SOTC_POSE82", "SOTC_WALK83", "SOTC_PTCL83", "SOTC_PROBE84", "SOTC_FIND84", "SOTC_GROUND85", "SOTC_PTCL89", "SOTC_PTCL91", "SOTC_JOIN95", "SOTC_DRAW95", "SOTC_EARLY95", "SOTC_OBJ96", "SOTC_SEL96", "SOTC_SEL97", "SOTC_LINKS97", "SOTC_PATH97", "SOTC_PLAN97", "SOTC_WALK101", "SOTC_SKIN101", "SOTC_LEAF101", "SOTC_OBJ101", "SOTC_SW102", "SOTC_PTCL103", "SOTC_SOLID103", "SOTC_GRP106", "SOTC_OAN106", "SOTC_AI106", "SOTC_COLL115", "SOTC_PKT117", "SOTC_GLV117", "SOTC_DRAW117", "SOTC_LEAF117", "SOTC_SYM120", "SOTC_HEAP120", "SOTC_FLUSH119", "SOTC_WALK119", "SOTC_PART119", "SOTC_VIEW119", "SOTC_PTCL123", "SOTC_BLEND123", "SOTC_SEG123", "SOTC_GAZE122", "SOTC_CLOTH122", "SOTC_QUAT122", "SOTC_KEYS122", "SOTC_FIN125", "SOTC_OBJ125", "SOTC_SCROLL127", "SOTC_GEO126", "SOTC_CTL126", "SOTC_IMM128", "SOTC_TEXR128", "SOTC_PF134", "SOTC_CLIP134", "SOTC_POSE136", "SOTC_KEY136", "SOTC_EULER136", "SOTC_PF135", "SOTC_FIN138", "SOTC_SUB139", "SOTC_RND139", "SOTC_GC139", "SOTC_SORT140", "SOTC_QUAD140", "SOTC_CLIP140", "SOTC_PF141", "SOTC_PFC141", "SOTC_LIFTD1", "SOTC_LIFTD2", "SOTC_LIFTD3", "SOTC_LIFTD4", "SOTC_LIFTD5", "SOTC_LIFTD6", "SOTC_LIFTALT"};
    inline signed char g_switch[kSwitchCount] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // -1: not read yet
    inline bool switchOn(int which)
    {
        signed char v = g_switch[which];
        if (__builtin_expect(v < 0, 0))
        {
            const char *e = std::getenv(kSwitchEnv[which]);
            // Round 83 left ptcl83 off (below its 1 ms bar); round 89 turns it on (exact, a gain in all three
            // places measured: -0.16 ms riding, -0.23 the altar, -0.42 route 6). Round 84: find84 off unless SOTC_FIND84=1.
            // Round 91: ptcl91 off unless SOTC_PTCL91=1 (or 2: the job handed over and joined at once, 3: run here through the same split).
            // 2026-11-07 console: obj96 left Avion (a colossus the active-colossus test missed) frozen at the screen's
            // edges and saved only ~1 ms matched; off unless SOTC_OBJ96=1.
            // 2026-11-07 console: with charlod78, ptcl79, seg80, probe84 and ground85 off together Avion's intro
            // (the reveal) played in full, cut short with them on; frame time unchanged within noise. All five off
            // until each is proven on its own (SOTC_xxx=1 turns one on).
            // Console recheck: with seg80, probe84 and ground85 back on the reveal played in full; on again.
            // Round 106: oan106 is exact (0 differences) but measured no faster than the recompiled code (the
            // sampler: 0.30% of the game thread either way); off unless SOTC_OAN106=1.
            // Round 115: coll115 on (SOTC_COLL115=0 off): the answers the original's (checks: 0 differ since p2),
            // faster riding, even at the shrine / altar / route 6, with seg80's reuse off (every segment answered
            // each frame) unless SOTC_SEG80=1.
            // Round 119: flush119, walk119, part119 and view119 off unless SOTC_<name>=1: exact (0 differ) and each
            // touched call faster in the same run (2-9%), but the whole frame in Vita3K's A/B was even to slightly
            // slower with them (route 6 -0.2, riding -0.5..+0.14: the largest riding run +0.14); the console's A/B (env-console) decides.
            // Round 122: gaze122 off unless SOTC_GAZE122=1: its checks found calls (~1 in 300 riding) where the old path
            // leaves the neck / head nodes unchanged and the new one turns them by the gaze (0.2-6 deg); not traced yet.
            // cloth122, quat122 and keys122 on (SOTC_<name>=0 off): within their checks' tolerances, a win in the A/B.
            // Round 128: texr128 off unless SOTC_TEXR128=1: exact (0 differ) but its per-node checks cost Vita3K, which has no
            // memory latency for the independent loads to hide (sampler 0.53 -> 1.00%); the console's A/B decides.
            // Round 126: geo126 on (SOTC_GEO126=0 off): within its checks' tolerances, a win in the sampler.
            // Round 131: seg123 and fin125 off unless SOTC_<name>=1 (gameplay first: Agro dropped out of the gallop on the
            // console, r127). fin125's check (SOTC_FIN125_CHECK=1, attract demo, Vita3K) found clips where the original
            // hits a surface of the finder list and the host cache answers no hit (4 in ~30 000: a probe that misses
            // the ground for a frame changes the character's state); seg123 keeps answers for ends up to 1 cm away for up
            // to 60 frames and validates them against fingerprints made once a frame (an object that moves later in the
            // frame is not seen until the next one): not exact by design.
            // Round 135: fin125 on again (SOTC_FIN125=0 off): its two causes found and fixed (the attribute address kept at the
            // making went stale on Agro's surfaces, so the IK feet's filter said no where the game's lookup says yes; objects
            // replaced in their slots passed round 125's check on their old bytes): the check (each clip the original way
            // first, compared) found 0 differences in the attract demo's ride and on the console card's save (coll125.h).
            // Round 131: ctl126 off unless SOTC_CTL126=1: with it on, route 6 hangs after the shrine cutscene (Wander's
            // position jumps to ~(410, 48, 110), then reads (0, 0, 0.69), the camera freezes; Vita3K, r128 / r131), and on the
            // console (r129/p1) Wander loaded under the ground and moved only by jumping; SOTC_CTL126=0 fixes both.
            // Round 136: pose136 off unless SOTC_POSE136=1: exact (checks: 0 bones differ in ~1.7 M), but its kept bones
            // (17-43% of them) do not pay for the kept arrays' stores and compares (Vita3K host sampler: the pose 1.19 -> 1.22%
            // of the game thread); the console's A/B (env-console turns it on) decides. key136 and euler136 on (SOTC_<name>=0 off).
            v = (which == kFind84 || which == kPtcl91 || which == kEarly95 || which == kObj96 || which == kCharLod78 ||
                 which == kPtcl79 || which == kOan106 || which == kFlush119 || which == kWalk119 || which == kPart119 || which == kView119 ||
                 which == kGaze122 || which == kTexr128 || which == kCtl126 || which == kSeg123 || which == kPose136)
                    ? (e && e[0] >= '1' && e[0] <= '3')
                    : !(e && e[0] == '0');
            g_switch[which] = v;
        }
        return v != 0;
    }
    // Called when a switch changes at run time (an A/B window): decomp D1's lifted functions are swapped in the table.
    inline void (*g_switchChanged)(int which, bool on) = nullptr;
    inline void setSwitch(int which, bool on)
    {
        const signed char v = on ? 1 : 0;
        if (g_switch[which] == v)
            return;
        g_switch[which] = v;
        if (g_switchChanged)
            g_switchChanged(which, on);
    }

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
    // Round 48's subsystems: the script system (the sub-threads' frame end,
    // scrRun / scrRunM, the SCR data interpreter execSCRBaseFunc and its hot
    // opcodes, a script body; decomp_scriptvm*.cpp) and the isys object
    // lifetime (the group deletes, the display list swap, the letters;
    // decomp_isys48.cpp), and the rest of round 48's list (decomp_hot48*.cpp).
    // SOTC_SCRIPTVM=0 / SOTC_ISYS=0 / SOTC_HOTLIST48=0 put the recompiled code
    // back (read at first use; a coroutine native checks it at its entry and
    // at each resume label too).
    inline bool scriptVmOn() { return switchOn(kScriptVm); }
    inline bool isysOn() { return switchOn(kIsys); }
    inline bool hotList48On() { return switchOn(kHotList48); }
    // Round 49: the hottest natives' inner loops rewritten for the Cortex-A9
    // (host pointers, values in locals, plain NEON where the clamps provably
    // change nothing, fewer guest round trips), bit-exact with round 48's
    // code. SOTC_FASTNATIVE=0 puts round 48's code paths back (read at first
    // use; each native checks it at its entry).
    inline bool fastNativeOn() { return switchOn(kFastNative); }
    // Round 54: the next natives' fast paths (the same rules as round 49's),
    // SOTC_FASTNATIVE2=0 puts the previous code back.
    inline bool fastNative2On() { return switchOn(kFastNative2); }
    // Round 58: the next fast paths (the same rules), SOTC_FASTNATIVE3=0 off.
    inline bool fastNative3On() { return switchOn(kFastNative3); }
    // Round 67 (SOTC_WALK67=0: off): the per-object walks' guest lines asked
    // for further ahead and level by level (prefetch only: what is read,
    // written and called is the same either way).
    inline bool walk67On() { return switchOn(kWalk67); }
    inline bool pref69On() { return switchOn(kPref69); }
    inline bool coll71On() { return switchOn(kColl71); }
    inline bool charLod78On() { return switchOn(kCharLod78); }
    inline bool ptcl79On() { return switchOn(kPtcl79); }
    inline bool seg80On() { return switchOn(kSeg80); }
    inline bool pose82On() { return switchOn(kPose82); }
    inline bool walk83On() { return switchOn(kWalk83); }
    inline bool ptcl83On() { return switchOn(kPtcl83); }
    inline bool probe84On() { return switchOn(kProbe84); }
    inline bool find84On() { return switchOn(kFind84); }
    inline bool ground85On() { return switchOn(kGround85); }
    inline bool ptcl89On() { return switchOn(kPtcl89); }
    inline bool obj96On() { return switchOn(kObj96); }
    inline bool sel96On() { return switchOn(kSel96); }
    inline bool sel97On() { return switchOn(kSel97); }
    inline bool plan97On() { return switchOn(kPlan97); }
    inline bool path97On() { return switchOn(kPath97); }
    inline bool links97On() { return switchOn(kLinks97); }
    inline bool skin101On() { return switchOn(kSkin101); }
    inline bool obj101On() { return switchOn(kObj101); }
    inline bool leaf101On() { return switchOn(kLeaf101); }
    inline bool sw102On() { return switchOn(kSw102); }
    inline bool ptcl103On() { return switchOn(kPtcl103); }
    inline bool solid103On() { return switchOn(kSolid103); }
    inline bool grp106On() { return switchOn(kGrp106); }
    inline bool oan106On() { return switchOn(kOan106); }
    inline bool ai106On() { return switchOn(kAi106); }
    inline bool pkt117On() { return switchOn(kPkt117); }
    inline bool glv117On() { return switchOn(kGlv117); }
    inline bool imm128On() { return switchOn(kImm128); }
    inline bool pf134On() { return switchOn(kPf134); }
    inline bool clip134On() { return switchOn(kClip134); }
    inline bool pf135On() { return switchOn(kPf135); }
    inline bool gc139On() { return switchOn(kGc139); } // round 139
    inline bool sort140On() { return switchOn(kSort140); } // round 140
    inline bool quad140On() { return switchOn(kQuad140); } // round 140
    inline bool clip140On() { return switchOn(kClip140); } // round 140
    inline bool pf141On() { return switchOn(kPf141); } // round 141
    inline bool pfc141On() { return switchOn(kPfc141); } // round 141
    inline bool rnd139On() { return switchOn(kRnd139); } // round 139
    inline bool sub139On() { return switchOn(kSub139); } // round 139 (decomp_subthread)
    inline bool draw117On() { return switchOn(kDraw117); }
    inline bool leaf117On() { return switchOn(kLeaf117); }
    // Round 91 (sotc_worker): SOTC_PTCL91=1 the particle step on the worker beside the walk's group pass,
    // 2 handed over and joined at once (no overlap), 3 run on the game thread through the same split.
    inline bool ptcl91On() { return switchOn(kPtcl91); }
    inline int ptcl91Mode()
    {
        static const int m = [] {
            const char *e = std::getenv("SOTC_PTCL91");
            return e ? std::atoi(e) : 0;
        }();
        return m;
    }
    // Round 95 (SOTC_JOIN95, default on; only with ptcl91): the job also runs
    // the particle geodisps' clip (their class's update, clip_particle_geodisp)
    // after the layers, and the display walk joins it inside the light pass,
    // after the layout objects' light callbacks (decomp_walk, decomp_light).
    inline bool join95On() { return switchOn(kJoin95); }
    // Round 95 (SOTC_DRAW95, default on; only with join95): the walk joins
    // inside its draw loop instead, the held particle geodisps put in the
    // second list then (those whose turn has passed drawn at once), their
    // light callbacks after the light pass (its result copied to RAM by
    // LoopExecAfterLightColMan, read through the same pointer).
    inline bool draw95On() { return switchOn(kDraw95); }
    // Round 95 (SOTC_EARLY95=1, default off; only with ptcl91 mode 1): the
    // particle manager is called at the camera manager's entry (the same
    // arguments its task would give it) and skipped at its own call, so the
    // job starts a camera earlier. Not exact: the step sees the camera of the
    // frame before (culling, distances); the clip is left to the walk.
    inline bool early95On() { return switchOn(kEarly95); }
    // Round 95: an early hand-over happened since the last call (the display walk asks once a walk; decomp_hot47e).
    bool early95Kicked();
    // Round 95: the geodisp's clip was run by the last job (decomp_hot47e; read after the join).
    bool ptcl95Clipped(uint32_t object);
    // Round 95: the light pass's hook after its light callbacks (decomp_light calls it when set, then the
    // light callbacks of the sort nodes it returns; the display walk sets it around its light pass call).
    using LightMid95 = void (*)(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, std::vector<uint32_t> &nodes);
    inline LightMid95 g_lightMid95 = nullptr;
    // Round 79 (decomp_hot47e.cpp): the particle geodisp object the manager
    // found wholly outside the camera's clip volume this frame and the frame
    // before (its step put off, its draw not needed).
    bool ptcl79Culled(uint32_t object);
    // Round 91: the geodisp was freed by the auto-end check of the last job (decomp_hot47e; read after the join).
    bool ptcl91Freed(uint32_t object);
    // Round 83 (decomp_hot43b.cpp): the light pass's start and end, the light colours' cache around them.
    void walk83LightBegin(uint8_t *rdram, PS2Runtime *runtime);
    void walk83LightEnd();
    bool walk83ParticleLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t id, uint32_t sp, uint32_t ra);

    // Round 49's measurements. NativeCount: calls of a native (counted
    // whatever SOTC_FASTNATIVE says, at its entry); with
    // SOTC_FASTNATIVE_STATS=1 each one's total is printed every 600 game
    // frames ("[fncount] frame F name calls"), so the profile's instructions
    // a frame divide into instructions a call. FastCheck: a Vita-only fast
    // path's result compared with the exact code's when
    // SOTC_FASTNATIVE_CHECK=1 (the exact result is the one used), counted in
    // "[fastcheck] name checks mismatches" lines.
    inline signed char g_fnStats = -1, g_fnCheck = -1;
    inline bool flagEnv(signed char &v, const char *name)
    {
        if (__builtin_expect(v < 0, 0))
        {
            const char *e = std::getenv(name);
            v = (e && e[0] == '1') ? 1 : 0;
        }
        return v != 0;
    }
    inline bool fastStatsOn() { return flagEnv(g_fnStats, "SOTC_FASTNATIVE_STATS"); }
    inline bool fastCheckOn() { return flagEnv(g_fnCheck, "SOTC_FASTNATIVE_CHECK"); }
    // constexpr: a function-local static NativeCount needs no guard; hit()
    // is one 32-bit add (and a cold enrolment at the first call); the
    // frame tick (fastStatsTick, sotc_native's frame loop) prints them.
    struct NativeCount
    {
        const char *name;
        uint32_t calls = 0;
        NativeCount *next = nullptr;
        constexpr explicit NativeCount(const char *n) : name(n) {}
        inline __attribute__((always_inline)) void hit()
        {
            if (__builtin_expect(calls++ == 0u, 0))
                enrol();
        }
        void enrol();
    };
    inline NativeCount *g_nativeCounts = nullptr;
    // (the attributes on the definition: GCC warns about an inline definition
    // after a noinline declaration)
    __attribute__((noinline, cold)) inline void NativeCount::enrol()
    {
        next = g_nativeCounts;
        g_nativeCounts = this;
    }
    inline void fastStatsTick(uint64_t frame)
    {
        if (frame % 600u != 0u || !fastStatsOn())
            return;
        for (NativeCount *c = g_nativeCounts; c; c = c->next)
            std::fprintf(stderr, "[fncount] frame %llu %s %u\n", (unsigned long long)frame, c->name, c->calls);
    }
    struct FastCheck
    {
        const char *name;
        uint64_t checks = 0, bad = 0;
        explicit FastCheck(const char *n) : name(n) {}
        __attribute__((noinline)) void note(bool same)
        {
            ++checks;
            if (!same)
                ++bad;
            if ((!same && bad <= 8u) || (checks & 0x3FFFu) == 0u)
                std::fprintf(stderr, "[fastcheck] %s %llu %llu%s\n", name, (unsigned long long)checks, (unsigned long long)bad, same ? "" : " MISMATCH");
        }
    };
}

#define SOTC_DECOMP_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn}
// One of SOTC_AB's new set (this round's functions).
#define SOTC_DECOMP_AB_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, true}
#define SOTC_DECOMP_FLOAT_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, true}
// Round 48: a coroutine native (never behind a difftest checker), and one of
// its resume entries (a label after a call that can sleep; `tag` makes the
// registrar's name unique, the name stays the function's for SOTC_DECOMP_SKIP).
#define SOTC_DECOMP_COROUTINE(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, true, false}
#define SOTC_DECOMP_RESUME(address, name, tag, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_##tag{address, #name, fn, false, false, false, false, true, true}
// A native whose difftest legs would each change host state the check does
// not put back (a semaphore taken or given in one leg only: the second leg's
// WaitSema would block for good): installed like a coroutine, never checked.
#define SOTC_DECOMP_UNCHECKED(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, true, false}
// Decomp D1: a lifted function (tools/lift_d1.py), in the table while SOTC_LIFTD1 is on.
#define SOTC_DECOMP_LIFT_FUNCTION(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original}
// Decomp D2: a lifted function of D2's batch, in the table while SOTC_LIFTD2 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D2(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD2}
// Decomp D3: a lifted function of D3's batch, in the table while SOTC_LIFTD3 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D3(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD3}
// Decomp D4: a lifted function of D4's batch, in the table while SOTC_LIFTD4 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D4(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD4}
// Decomp D5: a lifted function of D5's batch, in the table while SOTC_LIFTD5 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D5(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD5}
// Decomp D6: a lifted function of D6's batch, in the table while SOTC_LIFTD6 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D6(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD6}
// Decomp D5: a lifted copy of a function a native replaces (its slot holds the native): what sotc_decomp::recompiled()
// gives the natives (their fallbacks to the original code) while SOTC_LIFTD5 is on. Registered as "<name>_fb", so
// SOTC_DECOMP_SKIP=<name> (the native left out) puts the copy in the table, where the difftest can check it.
#define SOTC_DECOMP_LIFT_FALLBACK(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD5}
// Decomp D6: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D6's round: served while SOTC_LIFTD6 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D6(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD6}
// Decomp D4 (test builds): another lifter version of a lifted function, in its place while liftalt is off.
#define SOTC_DECOMP_LIFT_ALT(address, name, fn) \
    static const ::sotc_decomp::LiftAltRegistrar sotc_decomp_liftalt_##name{address, fn}
// A context switch that returns to its caller after a round trip (wake).
#define SOTC_DECOMP_SWITCH_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, false}
// A context switch that returns into the other context (sleep).
#define SOTC_DECOMP_SWITCH_ONCE_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, true}
