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

// Round N4b (docs/native_n4b.md, tools/n4_native.py): a NOCHK call into a loop-free, ABI-proven leaf calls the leaf's
// native entry (`lift_d1::<name>_n4`: register inputs as C++ values, v0 returned) when PS2_N4_FAST holds: exactly when
// the NOCHK call's fast form would run (the slot holds the known function, the host stack is above m_noCheckFloor,
// which is the top of the address space unless NOCHK is on; the worker's shell runtime never sets it) and the switch
// n4native is on (g_ps2N4Mask 0; UINTPTR_MAX off, set once a frame in sotc_native.cpp's endFrame). Then the call's
// dispatch cycles are charged (PS2_N4_ENTER), the native entry runs, and the call's own post-test (PS2_N4_DONE) is
// made; otherwise the call as it was. PS2X_N4_NATIVE=0 (define): never. Difftrace builds: never (the trace events).
// PS2X_N4_CHECK=1 (define, a check build): every native call compares the values it passes with the context
// (PS2_N4_ARG) and, after it, every local it did not read back with the context (PS2_N4_KEEP); SOTC_N4_LOG prints them.
#ifndef PS2X_N4_NATIVE
#define PS2X_N4_NATIVE 1
#endif
#ifndef PS2X_N4_CHECK
#define PS2X_N4_CHECK 0
#endif
extern uintptr_t g_ps2N4Mask;
#if PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 14
#define PS2_N4_SP() reinterpret_cast<uintptr_t>(__builtin_stack_address())
#else
#define PS2_N4_SP() reinterpret_cast<uintptr_t>(__builtin_frame_address(0))
#endif
#define PS2_N4_FAST(fnexpr, known) PS2_EXPECT((fnexpr) == &known && PS2_N4_SP() >= (runtime->m_noCheckFloor | g_ps2N4Mask), 1)
#else
#define PS2_N4_FAST(fnexpr, known) false
#endif
#define PS2_N4_ENTER() ((void)runtime->m_eeClock.charge(PS2Runtime::kEeDispatchCycles), runtime->m_guestJumpSerial)
#define PS2_N4_DONE(ret) (ctx->pc == (ret) && !runtime->m_eeClock.unwinding)
struct Ps2N4Stats
{
    uint64_t calls = 0;     // native calls made (check builds)
    uint64_t argDiff = 0;   // a value passed that differed from the context
    uint64_t keepDiff = 0;  // a local kept (not read back) that differed from the context
    uint32_t firstSite = 0; // the return address of the first difference
};
extern Ps2N4Stats g_ps2N4Stats;
template <typename T, typename U>
inline void ps2N4Keep(const T &local, U ctxValue, uint32_t site, bool arg)
{
    const T v = static_cast<T>(ctxValue);
    if (std::memcmp(&local, &v, sizeof(T)) != 0)
    {
        (arg ? g_ps2N4Stats.argDiff : g_ps2N4Stats.keepDiff)++;
        if (!g_ps2N4Stats.firstSite)
            g_ps2N4Stats.firstSite = site;
    }
}
#if PS2X_N4_CHECK
#define PS2_N4_COUNT() (++g_ps2N4Stats.calls)
#define PS2_N4_ARG(T, value, ctxValue, site) ps2N4Keep<T>(static_cast<T>(value), (ctxValue), (site), true)
#define PS2_N4_KEEP(local, ctxValue, site) ps2N4Keep((local), (ctxValue), (site), false)
#else
#define PS2_N4_COUNT() ((void)0)
#define PS2_N4_ARG(T, value, ctxValue, site) ((void)0)
#define PS2_N4_KEEP(local, ctxValue, site) ((void)0)
#endif

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
        // decomp_liftd6_*.cpp, kLiftD7 for decomp_liftd7_*.cpp, kLiftD8 for decomp_liftd8_*.cpp,
        // kLiftD9 for decomp_liftd9_*.cpp, kLiftD10 for decomp_liftd10_*.cpp,
        // kLiftD11 for decomp_liftd11_*.cpp, kLiftD12 for decomp_liftd12_*.cpp, kLiftD13 for decomp_liftd13_*.cpp, kLiftD14 for decomp_liftd14_*.cpp, kLiftD15 for decomp_liftd15_*.cpp, kLiftD16 for decomp_liftd16_*.cpp, kLiftD17 for decomp_liftd17_*.cpp, kLiftD18 for decomp_liftd18_*.cpp, kLiftD20 for decomp_liftd20_*.cpp).
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
    // Round P3's check (SOTC_P3PAR_CHECK, p3_par.h): called right after a garbage move (source / destination offsets in main RAM, bytes).
    inline void (*g_afterGuestMove)(uint32_t src, uint32_t dst, uint32_t bytes) = nullptr;
    // Round P3's diagnostics: the move g_beforeGuestMove is called for (source, destination, bytes; guest addresses as copyViaSpr has them).
    inline uint32_t g_moveSrc = 0, g_moveDst = 0, g_moveBytes = 0;
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
    // Round B6 (b6_sync.cpp): the function the idle-turn hook on iosRecvMsg (sotc_overrides.cpp) calls inside it, replaced by
    // f; returns the one it held (null: the hook is not installed, nothing replaced).
    PS2Runtime::RecompiledFunction swapIosRecvInner(PS2Runtime::RecompiledFunction f);
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
    // Round P4 (p4_move.h): the hook worker sees the garbage moves as they were when the job it runs was handed over (the
    // serial, the count, its own index of the ring), so a move that went on while older jobs were still queued is not seen
    // by them: exactly what they saw before P4, when every move joined the whole queue first. Defined in sotc_native.cpp;
    // t_moveView is set only on the hook worker. (Equal to the globals whenever no move went on beside queued jobs.)
    extern thread_local bool t_moveView;
    extern thread_local uint32_t t_moveViewSerial, t_moveViewCount;
    inline GuestMoveIndex g_guestMoveIndexView; // the hook worker's index (only it builds it)
    inline uint32_t moveSerialSeen() { return t_moveView ? t_moveViewSerial : g_guestMoveSerial; }
    inline uint32_t moveCountSeen() { return t_moveView ? t_moveViewCount : g_guestMoveCount; }
    inline GuestMoveIndex &moveIndexSeen() { return t_moveView ? g_guestMoveIndexView : g_guestMoveIndex; }
    inline void buildGuestMoveIndex()
    {
        GuestMoveIndex &x = moveIndexSeen();
        const uint32_t count = moveCountSeen();
        if (x.builtAt == count)
            return;
        x.builtAt = count;
        const uint32_t n = std::min(count, kGuestMoves);
        // Only the newest moves with serials one after the other (a chunked
        // copy in between was not recorded: what came before it is not used).
        uint32_t expect = moveSerialSeen(), used = 0;
        for (; used < n; ++used)
        {
            const GuestMove &m = g_guestMoves[(count - 1u - used) & (kGuestMoves - 1u)];
            if (m.serial != expect)
                break;
            --expect;
        }
        x.first = count - used;
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
        const GuestMoveIndex &x = moveIndexSeen();
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
        if (!moveCountSeen())
            return false;
        buildGuestMoveIndex();
        uint32_t before = moveCountSeen(), at = addr, number = 0;
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
        kLiftD7,   // decomp D7: the next batch and the earlier rounds' speed cuts, every one checked with the trace check (exact; decomp_liftd7_*.cpp)
        kLiftD8,   // decomp D8: the particle area's rest and the functions new routes reach (teleport, new game), trace-checked (exact; decomp_liftd8_*.cpp)
        kLiftD9,   // decomp D9: the collision and physics area's rest (ledges, climbing, swimming, fights) and natives' fallback copies, trace-checked (exact; decomp_liftd9_*.cpp)
        kLiftD10,  // decomp D10: the characters and motion area's rest (skeleton, motion, IK, the characters' state machines) and natives' fallback copies, trace-checked (exact; decomp_liftd10_*.cpp)
        kLiftD11,  // decomp D11: the display and models area's rest (model / mesh draw prep, GS packets, textures) and newly reached functions, trace-checked (exact; decomp_liftd11_*.cpp)
        kLiftD12,  // decomp D12: the functions the routes reach that were still recompiled (script, collision, the climbing route), trace-checked (exact; decomp_liftd12_*.cpp)
        kLiftD13,  // decomp D13: the colossus-switch route's and the ledge route's functions still recompiled, the lifter's last refusals (branches out, bc0 waits), trace-checked (exact; decomp_liftd13_*.cpp)
        kLiftD14,  // decomp D14: the colossus-grip route's functions (Wander on Avion: grip, parent points, pendulum, weak points) and the rest still recompiled, trace-checked (exact; decomp_liftd14_*.cpp)
        kLiftD15,  // decomp D15: the per-colossus grip routes' functions (each colossus climbed and stabbed) and the rest still recompiled, trace-checked (exact; decomp_liftd15_*.cpp)
        kLiftD16,  // decomp D16: the arena routes' functions (the colossi woken in their arenas, the deaths) and the rest still recompiled, trace-checked (exact; decomp_liftd16_*.cpp)
        kLiftD17,  // decomp D17: the colossi killed from their checkpoint places (deaths, cutscenes) and D16's unchecked, trace-checked (exact; decomp_liftd17_*.cpp)
        kLiftD18,  // decomp D18: the weak-point and armour aids' kill routes (Malus, Cenobia and the others) and D16's still unchecked, trace-checked (exact; decomp_liftd18_*.cpp)
        kLiftD20,  // decomp D20: the colossi's completed deaths (the bdie aid: the killing hit's own ending) and D19's reached, trace-checked (exact; decomp_liftd20_*.cpp)
        kLiftAlt,  // decomp D4 (test): off puts each lifted function's alternative copy (SOTC_DECOMP_LIFT_ALT: another lifter
                   // version of the same function, decomp_liftalt_*.cpp in test builds only) in its place: an A/B of two lifters
        kS1Ptcl,   // round S1: the particle subsystem's first redesign: draw records from a host snapshot taken at the
                   // emitter's step, written straight into the renderer's job (s1_particles.cpp; port, observable-equivalence checked)
        kS2Coll,   // round S2: the collision subsystem redesign: finder clips on host list views (s2_coll.h), the water probes pre-tested
                   // (s2_water.h); exact, checked in-run against the original (SOTC_S2COLL_CHECK)
        kS3Chara,  // round S3: the characters' skeleton work redesigned: the pose over a host rig of each skeleton (visit order,
                   // parents, links validated each call), optionally split into jobs on the S3 worker (s3_chara.cpp; exact, checked
                   // in-run bit for bit against round 82's pose: SOTC_S3CHARA_CHECK)
        kA9Cmp,    // decomp D11 (part A): NEON float compares that GCC (arm-vita-eabi 15) scalarises (four vcmpe + vmrs + a stack
                   // round trip) as integer compares giving the same mask for every input (a9cmp.h; exact)
        kS4Disp,   // round S4: display / draw prep without PS2 packets nobody reads under SOTC_NATIVE: the screen passes' host-only
                   // variants (s4_disp.cpp; checked in-run against the original's guest writes and host emissions: SOTC_S4DISP_CHECK)
        kA9Sweep,  // round A9 sweep: A9 codegen costs in hot code (function-local static guards: a load and a dmb ish per call;
                   // emulated TLS calls) replaced by values read once at install (a9sweep.h; no change of behaviour)
        kS5Ptcl,   // round S5: the particles' step off the game thread: the batch built in the emitter loop, stepped by a host
                   // worker as host code, joined before any reader (s5_particles.cpp; exact, checked in-run: SOTC_S5PTCL_CHECK)
        kS6Coll,   // round S6: the water probes over a host index of the water mesh, made and vouched for by a host worker
                   // (s6_coll.cpp; the same triangles visited as S2's loop: exact, checked in-run: SOTC_S6COLL_CHECK)
        kS9Rt,     // round S9: the game thread's runtime costs riding (s9rt.h): the renderer's eviction pass without its no-op
                   // mesh walk, clock reads kept for the logs, guest copies / zeroing without the libc's long-copy calls,
                   // the shadow palette read once (exact: SOTC_S9RT_CHECK, SOTC_S9RT_PARTS picks parts)
        kS10Draw,  // round S10: the model draw path without the PS2's GS state on the game thread: the layer cache *0x118bfc8
                   // (modelSetSurface's "last surface per layer", GS packet state only) left out under SOTC_NATIVE, so its
                   // readers no longer join the modelDispState lane (sotc_native.cpp s10draw; checked in-run: SOTC_S10DRAW_CHECK)
        kS11Rt,    // round S11: the PS2 machinery on the game thread (s11rt.h): the isys lock's kernel calls inline from the
                   // scheduler's state, the IOP in larger batches (fewer slow checkpoints); SOTC_S11RT_PARTS picks parts
        kS12Draw,  // round S12 (off unless SOTC_S12DRAW=1): the display's immediate quads (the strips) as host vertex records: one host pass per quad,
                   // the gl block written once, the vertices straight into the 2D assembler without GS A+D pairs
                   // (sotc_native.cpp s12draw, decomp_walk.cpp quad12; checked in-run: SOTC_S12DRAW_CHECK)
        kS13Mot,   // round S13: the motion data as host code over named views (s13mot.h, s13_motion.cpp): kMotion's packed root
                   // keys exact, the root getters host routines, the time slice stretched while nothing could preempt
                   // (exact: SOTC_S13MOT_CHECK; SOTC_S13MOT_PARTS picks parts)
        kS14Hook,  // round S14 (off unless SOTC_S14HOOK=1): the model hook's bone palette as a host record dispSkinned writes, not parsed out of the
                   // instance chain's VIF UNPACKs by the hook / the skinned shadow (s14hook.h; checked in-run: SOTC_S14HOOK_CHECK)
        kS17Iop,   // round S17: a checkpoint that would unwind the running thread to the EE scheduler (the IOP's replies waking
                   // the streaming / loader threads, the interrupt handlers) runs the dispatch nested there and lets the
                   // thread's frames go on when it is picked again (s17iop.h, EeScheduler::s17Nest; SOTC_S17IOP_CHECK)
        kS16Plan,  // round S16 (off unless SOTC_S16PLAN=1): the model draw plans vouched for by the game's model setup calls
                   // (modelInitialize / modelReInitialize / modelInitDispModelClass, hooked), not checked against the guest
                   // packet list every frame (s16plan.h, sotc_native.cpp planChecked; exact: SOTC_S16PLAN_CHECK)
        kS19Amb,   // round S19 (off unless SOTC_S19AMB=1): getAmbientColorC's light-grid lookups without guest calls: the ambient light group's links
                   // walked as host reads (no isysGroupForFirst / Next / Exit, GetAmbientLightClass calls), floorf called
                   // as the native it is (decomp_ambient.cpp; exact: SOTC_S19AMB_CHECK)
        kS21Light, // round S21 (SOTC_S21LIGHT=0 off): the global light query's lights as host records: each light slot's inverse
                   // matrix and registers kept while its quaternion / position bytes stay, the range test through one host
                   // pointer (decomp_light.cpp; exact: SOTC_S21LIGHT_CHECK)
        kS22Ptcl,  // round S22: the particle draw list (0113ACB8) without a guest call per node: its runs' sort as a host
                   // copy of the libc qsort over the nodes' depths, each node's view pass and disp_particle_emitter called
                   // straight (decomp_ptcl117.cpp; exact: SOTC_S22PTCL_CHECK)
        kS23Light, // round S23 (off unless SOTC_S23LIGHT=1): the light pass's second and third walks over the layout objects the first walk found (a host
                   // list), not the whole sort list again (decomp_light.cpp; exact: SOTC_S23LIGHT_CHECK)
        kN2Snd,    // round N2 (SOTC_N2SND=0 off): the sound engine's remote layer on the host: Sg2RemoteSend / Sg2RemoteTickProc
                   // hand their commands to the native driver straight (no guest ring, no SIF RPC), the reply written into the
                   // game's status block; the mixer's N2 paths (sound_n2.cpp, sound_mixer.cpp; docs/sound_n2.md; SOTC_N2SND_CHECK)
        kN5Sched,  // round N5 (N5b: on unless SOTC_N5SCHED=0, from GAMECORE's entry): the EE kernel's own rule for equal priorities: no time slice, only a
                   // strictly higher-priority ready thread preempts at a checkpoint; threads switch at blocking / yielding /
                   // waking points only (EeScheduler.cpp g_ps2xN5Sched; docs/native_n5.md)
        kN3Direct, // round N3 (SOTC_N3DIRECT=0 off): generated code's calls to lifted functions without the table read while every
                   // direct target's slot holds its lifted function (ps2_runtime_macros.h PS2_CALL_GUEST_DIRECT, tools/n3_direct.py;
                   // docs/native_n3.md)
        kR1Sort,   // round R1 (SOTC_R1SORT=0 off): the display walk's AddSortObj calls as host code (typed sort list / nodes,
                   // no context, no table call; decomp_dl.cpp addSortHost, r1sort.h; exact: SOTC_R1SORT_CHECK; docs/native_r1.md)
        kN6NoChk,  // round N6 (SOTC_N6NOCHK=0 off; only while n5sched is in effect): generated calls into functions with no loop and no
                   // thread switch below them (docs/n5_noloop.txt) charge the dispatch checkpoint without testing it
                   // (ps2_runtime_macros.h PS2_CALL_GUEST_NOCHK, tools/n6_nochk.py; SOTC_N6_LOG; docs/native_n6.md)
        kN4Native, // round N4b (SOTC_N4NATIVE=0 off; only while NOCHK is on): NOCHK calls into loop-free ABI-proven leaves
                   // call their native entries (register inputs as values, v0 returned; sotc_decomp.h PS2_N4_FAST,
                   // tools/n4_native.py, docs/n4b_native.txt; SOTC_N4_LOG; docs/native_n4b.md)
        kC2Scene,  // round C2 (off unless SOTC_C2SCENE=1): the display group as a host scene list kept from the last walk: the clip
                   // helper's list without walking the guest chain, the group pass / InitLayoutObjMatrix / the light pass asking
                   // for their objects' lines ahead from it (c2scene.h, decomp_walk.cpp; docs/console_c2.md; SOTC_C2SCENE_CHECK)
        kC1Keys,   // round C1 (off unless SOTC_C1KEYS=1): the motion keys' decoded rows kept per motion in host memory (rows by frame,
                   // validated once a frame against the motion's records), so GetMotionFloatFrameNoCaribration / GetMotionIntFrame
                   // read a compact host row instead of every channel's record and key words (docs/console_c1.md)
        kC1Rig,    // round C1 (off unless SOTC_C1RIG=1): S3's skeleton rig compared with the guest's bone records once a game frame (the
                   // frame's first pose of that skeleton) instead of at every pose (docs/console_c1.md)
        kDc1Mot,   // round DC1: the motion accessors, the node base position / rotation and the option nodes as real C++ on typed
                   // views (dc1_motion.cpp), called directly by ExecCharaGeometry and the aim IK (docs/decomp_dc1.md)
        kA1Typed,  // round A1 (SOTC_A1TYPED=0 off): the typed functions (typed_a1.h, tools/typed_lift.py): the lifted leaves' adapters
                   // and the N4 sites call real C++ functions without the R5900 context (docs/decomp_a1.md; SOTC_A1_LOG)
        kA2Typed,  // round A2 (SOTC_A2TYPED=0 off): the typed functions with frames and calls (typed_a2.h, tools/typed_lift2.py): the lifted functions' adapters
        kA3Typed,  // round A3 (SOTC_A3TYPED=0 off): the typed functions with loops (typed_a3.h, tools/typed_lift2.py stage 3): back edges charge the
                   // EE clock, the checkpoint deferred to after the call (n5sched + NOCHK only); extra outputs (docs/decomp_a3.md)
        kA4Typed,  // round A4 (SOTC_A4TYPED=0 off): the typed VU0 / 128-bit functions (typed_a4.h, tools/typed_lift2.py stage 4): the VU0
                   // registers, ACC, Q, status and GPR upper halves as reference parameters (the VU0 ABI), quadword accesses typed (docs/decomp_a4.md)
        kP1Chara,  // round P1 (off unless SOTC_P1CHARA=1): the character chain on host layouts for the Vita's memory system: C1's key rows
                   // and once-a-frame rig, the pose reading the rig's host offsets with prefetch, the gaze keys from a host table per gaze
                   // motion (no curveEval guest calls), the skin palette's lines asked ahead (p1_chara.cpp; docs/perf_p1.md; SOTC_P1CHARA_CHECK)
        kA5Typed,  // round A5 (SOTC_A5TYPED=0 off): stage 5's typed functions (typed_a5.h, tools/typed_lift2.py stage 5): the typed ABI
                   // proof, relocated calls (A5_JAL), guest frames (sp a local guest address), 64-bit inputs, jump tables (docs/decomp_a5.md)
        kP2Ptcl,   // round P2 (off unless SOTC_P2PTCL=1): the particle frame's per-geodisp overhead cut and moved to the S5 worker
                   // (p2_particles.cpp; docs/perf_p2.md; SOTC_P2PTCL_CHECK)
        kA6Typed,  // round A6 (SOTC_A6TYPED=0 off): stage 6's typed functions (typed_a6.h, tools/typed_lift2.py stage 6): never-lifted
                   // functions and MIPS-step hand natives typed from their MIPS, the typed libc / thread-id stubs, break (docs/decomp_a6.md)
        kDc2Chara, // round DC2 (off unless SOTC_DC2CHARA=1): the hottest character-chain functions by console time as real C++ on typed views
                   // (the cloth step / constraint / velocity / tension and getClothDynamics, the character / animation class getters,
                   // findRecord, GetMotionIntFrame): no R5900 context, no callGuest between them (dc2_chara.cpp; docs/decomp_dc2.md; SOTC_DC2_CHECK)
        kP3Par,    // round P3 (off unless SOTC_P3PAR=1): the heap's garbage moves stop waiting for the deferred kick pass (the skinned
                   // palettes' pass on the hook worker, which reads no guest memory): it runs on cores 1-2 beside the move and the
                   // game's logic (p3_par.h, sotc_native.cpp; docs/perf_p3.md; SOTC_P3PAR_CHECK)
        kP4Move,   // round P4 (off unless SOTC_P4MOVE=1): a heap garbage move waits only for the queued hook jobs whose guest reads
                   // meet the moved span (source or destination), not for the whole queue (p4_move.h, sotc_native.cpp; docs/perf_p4.md;
                   // SOTC_P4MOVE_CHECK in a PS2X_P4_CHECK build)
        kA7Typed,  // round A7 (SOTC_A7TYPED=0 off): stage 7's typed functions (typed_a7.h, tools/typed_lift2.py stage 7): typed callers take
                   // their callees' extra outputs back, the printf-family cores, more MIPS-step natives (docs/decomp_a7.md)
        kB1Sound,  // round B1 (off unless SOTC_B1SOUND=1): the VBlank / sound thread (0x1c80a0) runs on a host carrier thread (one baton
                   // with the EE dispatcher: no unwinding, no resume entries), tickProcSound / Sg2CtrlTickProc as host functions
                   // (b1_sound.cpp, EeScheduler's carrier; docs/decomp_b1.md; SOTC_B1SOUND_CHECK)
        kB2Load,   // round B2 (off unless SOTC_B2LOAD=1): the loader / CD streaming thread (0x115e4c0, the sheet manager) runs on a host
                   // carrier of its own (B1's carrier as a table; its CD / stream waits park in place) (b2_load.cpp; docs/decomp_b2.md;
                   // SOTC_B2LOAD_CHECK)
        kA8Typed,  // round A8 (SOTC_A8TYPED=0 off): stage 8's typed functions (typed_a8.h, tools/typed_lift2.py stage 8): the varargs
                   // functions' spills, more MIPS-step natives (docs/a8_natives.txt; docs/decomp_a8.md)
        kB3Threads, // round B3 (off unless SOTC_B3THREADS=1): the callback thread t4, the idle thread t5, the GP thread t10 and the
                    // main thread t9 on host carriers (with B1 / B2: every live thread after GAMECORE), the semaphore / sleep
                    // syscalls in place on a carrier (b3_threads.cpp, b3_sync.h; docs/decomp_b3.md; SOTC_B3_CHECK)
        kA9Typed,  // round A9 (SOTC_A9TYPED=0 off): stage 9's typed functions (typed_a9.h, tools/typed_lift2.py stage 9): the quat122 /
                   // geo126 natives as typed calls to their shared host-float cores (typed_a9_ext.h; docs/decomp_a9.md)
        kB4Fast,    // round B4 (off unless SOTC_B4FAST=1): a carrier that yields runs the dispatcher's loop in place (the handlers,
                    // the woken threads, other carriers resumed from there) and goes on with no hand-over when its thread is
                    // picked again (EeScheduler::carrierNest; needs b1sound / b2load / b3threads' carriers; docs/decomp_b4.md)
        kA10Typed, // round A10 (SOTC_A10TYPED=0 off): stage 10's typed functions (typed_a10.h, tools/typed_lift2.py stage 10):
                   // recursion, the upper-half class, the camera hook's note on the typed path and the slot holders proven
                   // equal to their typed functions (typed_a10_note.h, typed_a6_rt.h a6Is; docs/decomp_a10.md)
        kA11Typed, // round A11 (SOTC_A11TYPED=0 off): stage 11's typed functions (typed_a11.h, tools/typed_lift2.py stage 11):
                   // the matrix stack (iosPush / PopCurrentMatrix as typed calls on VU0's data memory, vi15 a slot of the
                   // VU0 interface: typed_a11_ext.h) and the MMI instructions' exact hi / lo uses (docs/decomp_a11.md)
        kB6Sync,    // round B6 (off unless SOTC_B6SYNC=1): the message boxes with carriers at both ends as host wait lists (the
                    // receive wakes the senders itself, no callback-thread hop) and the EE semaphore stubs as host calls on a
                    // carrier (b6_sync.cpp; needs the carrier switches; docs/decomp_b6.md; SOTC_B6_CHECK)
        kA12Typed, // round A12 (SOTC_A12TYPED=0 off): stage 12's typed functions (typed_a12.h, tools/typed_lift2.py stage 12):
                   // the motion natives typed from their MIPS (docs/a12_natives.txt, taken per native: g_a12HoldMask), the spin
                   // rule's list walk, a likely branch's restore, 64-bit pass-through via extra outputs (docs/decomp_a12.md)
        kB7Irq,     // round B7 (off unless SOTC_B7IRQ=1): the live interrupt handlers (VBlank start 0x1c8180, GS 0x1c37f0) as host
                    // functions run where the dispatcher would run their invocations, and the carriers' checkpoints without the
                    // IOP batch (b7_irq.cpp; needs the carrier switches; docs/decomp_b7.md; SOTC_B7_CHECK)
        kB8Sched,   // round B8 (off unless SOTC_B8SCHED=1): host-form interrupts delivered by the dispatcher without an invocation
                    // record (a slot per thread: the run a plain host call, the pop a flush test), and a timer read on a carrier
                    // re-setting the EE clock's gate in place (b8_sched.cpp; needs b7irq and the carrier switches;
                    // docs/decomp_b8.md; SOTC_B8_CHECK)
        kA13Typed, // round A13 (SOTC_A13TYPED=0 off): stage 13's typed functions (typed_a13.h, tools/typed_lift2.py stage 13):
                   // the non-GPR extra outputs (docs/decomp_a13.md)
        kB9Loop,    // round B9 (off unless SOTC_B9LOOP=1): a host-form delivery in one turn of the dispatcher's loop (push, run,
                    // pop and the thread's own path while the tops between them would do nothing), the ready queues as a
                    // host bitmap (b9_loop.cpp; needs b8sched, b7irq and the carrier switches; docs/decomp_b9.md; SOTC_B9_CHECK)
        kA14Typed, // round A14 (off unless SOTC_A14TYPED=1): stage 14's typed functions (typed_a14.h, tools/typed_lift2.py stage 14):
                   // the runtime state lift_d1 left opaque as VU0-interface slots, a likely branch's break slot; acts only with
                   // a13typed's flag (so only with SOTC_A12TYPED=1) (docs/decomp_a14.md)
        kSwitchCount
    };
    inline const char *const kSwitchEnv[kSwitchCount] = {"SOTC_CAMCOLL", "SOTC_COLLPROBE", "SOTC_MOTION", "SOTC_LEAVES", "SOTC_HOTLIST", "SOTC_HOTLIST44", "SOTC_HOTLIST46", "SOTC_HOTLIST47",
                                                         "SOTC_SCRIPTVM", "SOTC_ISYS", "SOTC_HOTLIST48", "SOTC_FASTNATIVE", "SOTC_FASTNATIVE2", "SOTC_FASTNATIVE3", "SOTC_PTCL62", "SOTC_COLL63", "SOTC_TEX64", "SOTC_WALK67", "SOTC_PREF69", "SOTC_COLL71", "SOTC_CHARLOD78", "SOTC_PTCL79", "SOTC_SEG80", "SOTC_POSE82", "SOTC_WALK83", "SOTC_PTCL83", "SOTC_PROBE84", "SOTC_FIND84", "SOTC_GROUND85", "SOTC_PTCL89", "SOTC_PTCL91", "SOTC_JOIN95", "SOTC_DRAW95", "SOTC_EARLY95", "SOTC_OBJ96", "SOTC_SEL96", "SOTC_SEL97", "SOTC_LINKS97", "SOTC_PATH97", "SOTC_PLAN97", "SOTC_WALK101", "SOTC_SKIN101", "SOTC_LEAF101", "SOTC_OBJ101", "SOTC_SW102", "SOTC_PTCL103", "SOTC_SOLID103", "SOTC_GRP106", "SOTC_OAN106", "SOTC_AI106", "SOTC_COLL115", "SOTC_PKT117", "SOTC_GLV117", "SOTC_DRAW117", "SOTC_LEAF117", "SOTC_SYM120", "SOTC_HEAP120", "SOTC_FLUSH119", "SOTC_WALK119", "SOTC_PART119", "SOTC_VIEW119", "SOTC_PTCL123", "SOTC_BLEND123", "SOTC_SEG123", "SOTC_GAZE122", "SOTC_CLOTH122", "SOTC_QUAT122", "SOTC_KEYS122", "SOTC_FIN125", "SOTC_OBJ125", "SOTC_SCROLL127", "SOTC_GEO126", "SOTC_CTL126", "SOTC_IMM128", "SOTC_TEXR128", "SOTC_PF134", "SOTC_CLIP134", "SOTC_POSE136", "SOTC_KEY136", "SOTC_EULER136", "SOTC_PF135", "SOTC_FIN138", "SOTC_SUB139", "SOTC_RND139", "SOTC_GC139", "SOTC_SORT140", "SOTC_QUAD140", "SOTC_CLIP140", "SOTC_PF141", "SOTC_PFC141", "SOTC_LIFTD1", "SOTC_LIFTD2", "SOTC_LIFTD3", "SOTC_LIFTD4", "SOTC_LIFTD5", "SOTC_LIFTD6", "SOTC_LIFTD7", "SOTC_LIFTD8", "SOTC_LIFTD9", "SOTC_LIFTD10", "SOTC_LIFTD11", "SOTC_LIFTD12", "SOTC_LIFTD13", "SOTC_LIFTD14", "SOTC_LIFTD15", "SOTC_LIFTD16", "SOTC_LIFTD17", "SOTC_LIFTD18", "SOTC_LIFTD20", "SOTC_LIFTALT", "SOTC_S1PTCL", "SOTC_S2COLL", "SOTC_S3CHARA", "SOTC_A9CMP", "SOTC_S4DISP", "SOTC_A9SWEEP", "SOTC_S5PTCL", "SOTC_S6COLL", "SOTC_S9RT", "SOTC_S10DRAW", "SOTC_S11RT", "SOTC_S12DRAW", "SOTC_S13MOT", "SOTC_S14HOOK", "SOTC_S17IOP", "SOTC_S16PLAN", "SOTC_S19AMB", "SOTC_S21LIGHT", "SOTC_S22PTCL", "SOTC_S23LIGHT", "SOTC_N2SND", "SOTC_N5SCHED", "SOTC_N3DIRECT", "SOTC_R1SORT", "SOTC_N6NOCHK", "SOTC_N4NATIVE", "SOTC_C2SCENE", "SOTC_C1KEYS", "SOTC_C1RIG", "SOTC_DC1MOT", "SOTC_A1TYPED", "SOTC_A2TYPED", "SOTC_A3TYPED", "SOTC_A4TYPED", "SOTC_P1CHARA", "SOTC_A5TYPED", "SOTC_P2PTCL", "SOTC_A6TYPED", "SOTC_DC2CHARA", "SOTC_P3PAR", "SOTC_P4MOVE", "SOTC_A7TYPED", "SOTC_B1SOUND", "SOTC_B2LOAD", "SOTC_A8TYPED", "SOTC_B3THREADS", "SOTC_A9TYPED", "SOTC_B4FAST", "SOTC_A10TYPED", "SOTC_A11TYPED", "SOTC_B6SYNC", "SOTC_A12TYPED", "SOTC_B7IRQ", "SOTC_B8SCHED", "SOTC_A13TYPED", "SOTC_B9LOOP", "SOTC_A14TYPED"};
    inline signed char g_switch[kSwitchCount] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // -1: not read yet (kA9Sweep: 0, off, until a9sweep::init reads it; kN5Sched: 0 until sotc_native's init reads SOTC_N5SCHED)
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
            // Round S12: s12draw off unless SOTC_S12DRAW=1: exact (checks: 0 differences) but about even riding (the frame
            // time's A/B +0.30 / +0.15 ms, +0.11 / -0.08, below z 2; the sampler: the new quad path 99-101% of the old).
            // Round S14: s14hook off unless SOTC_S14HOOK=1: exact (checks: 0 differences in 306 575 palettes taken) but even in the
            // frame time's A/B (ride7 -0.21 / +0.12 ms, below z 2; the sampler: readChainPaletteLean 0.57% -> 0.28% in the mixed run).
            // Round S16: s16plan off unless SOTC_S16PLAN=1 (docs/display_s16.md).
            // Round S19: s19amb off unless SOTC_S19AMB=1: exact (checks: 0 differences in 373 514 calls) but ~15 calls a frame
            // riding, below what the frame time's A/B resolves (docs/lights_s19.md).
            // Round S21: s21light on (SOTC_S21LIGHT=0 off): exact (checks: 0 differences in 355 850 calls), its A/Bs pooled
            // +0.13 +- 0.05 ms (z about 2.4; docs/lights_s21.md).
            // Round S22: s22ptcl off unless SOTC_S22PTCL=1 (docs/ptcl_s22.md).
            // Round N6: n6nochk on (SOTC_N6NOCHK=0 off): the pooled A/B +0.030 +- 0.073 ms, the rides / boot normal (docs/native_n6.md).
            // Round N4b: n4native on (SOTC_N4NATIVE=0 off): exact (check build: 0 differences), the pooled A/B +0.071 +- 0.071 ms (docs/native_n4b.md).
            // Round S23: s23light off unless SOTC_S23LIGHT=1: exact (checks: 0 differences in 70 926 walks) but even in the
            // frame time's A/B (the walks it shortens are ~0.05 ms a frame riding; docs/lights_s23.md).
            // Round C2: c2scene off unless SOTC_C2SCENE=1: exact by construction (prefetches and the clip helper's list only);
            // a cache-miss change Vita3K does not model: the console's A/B decides (docs/console_c2.md).
            // Round P1: p1chara off unless SOTC_P1CHARA=1: exact (SOTC_P1CHARA_CHECK), a memory-traffic change Vita3K does not
            // model: the console's A/B decides (docs/perf_p1.md).
            // Round P2: p2ptcl off unless SOTC_P2PTCL=1 (docs/perf_p2.md): the console's A/B decides.
            // Round DC2: dc2chara off unless SOTC_DC2CHARA=1: exact (SOTC_DC2_CHECK: 0 differences in 26 M calls compared) but
            // ~0.1 ms slower in Vita3K's A/B (pooled -0.093 +- 0.030 ms, z -3; docs/decomp_dc2.md): the console's A/B decides.
            // Round P3: p3par off unless SOTC_P3PAR=1: exact (SOTC_P3PAR_CHECK); a wait the console has (its hook worker is
            // behind at the garbage moves) and Vita3K mostly does not: the console's A/B decides (docs/perf_p3.md).
            // Round B1: b1sound off unless SOTC_B1SOUND=1 (docs/decomp_b1.md).
            // Round B2: b2load off unless SOTC_B2LOAD=1 (docs/decomp_b2.md).
            // Round B3: b3threads off unless SOTC_B3THREADS=1 (docs/decomp_b3.md).
            // Round B4: b4fast off unless SOTC_B4FAST=1 (docs/decomp_b4.md).
            // Round B6: b6sync off unless SOTC_B6SYNC=1 (docs/decomp_b6.md).
            // Round B7: b7irq off unless SOTC_B7IRQ=1 (docs/decomp_b7.md).
            // Round B8: b8sched off unless SOTC_B8SCHED=1 (docs/decomp_b8.md).
            // Round B9: b9loop off unless SOTC_B9LOOP=1 (docs/decomp_b9.md). Round A14: a14typed off unless SOTC_A14TYPED=1 (docs/decomp_a14.md).
            v = (which == kFind84 || which == kPtcl91 || which == kEarly95 || which == kObj96 || which == kCharLod78 ||
                 which == kPtcl79 || which == kOan106 || which == kFlush119 || which == kWalk119 || which == kPart119 || which == kView119 ||
                 which == kGaze122 || which == kTexr128 || which == kCtl126 || which == kSeg123 || which == kPose136 || which == kS12Draw || which == kS14Hook || which == kS16Plan || which == kS19Amb || which == kS22Ptcl || which == kS23Light || which == kC2Scene || which == kC1Keys || which == kC1Rig || which == kP1Chara || which == kP2Ptcl || which == kDc2Chara || which == kP3Par || which == kP4Move || which == kB1Sound || which == kB2Load || which == kB3Threads || which == kB4Fast || which == kB6Sync || which == kB7Irq || which == kB8Sched || which == kB9Loop || which == kA14Typed)
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
    // Round P1 (SOTC_P1CHARA, A/B p1chara; off by default): the character chain for the Vita's memory system. While on it
    // also turns on C1's key rows and once-a-frame rig (c1keys / c1rig: exact, docs/console_c1.md), whatever their own
    // switches say (p1_chara.h, docs/perf_p1.md).
    inline bool p1charaOn() { return switchOn(kP1Chara); }
    // Round P2 (SOTC_P2PTCL, A/B p2ptcl; off by default): the particle frame (p2_particles.cpp, docs/perf_p2.md).
    inline bool p2ptclOn() { return switchOn(kP2Ptcl); }
    // Round DC2 (SOTC_DC2CHARA, A/B dc2chara): the character chain's hottest functions as real C++ (dc2_chara.cpp, docs/decomp_dc2.md).
    inline bool dc2charaOn() { return switchOn(kDc2Chara); }
    // Round P3 (SOTC_P3PAR, A/B p3par; off by default): the garbage moves do not wait for the deferred kick pass (p3_par.h).
    inline bool p3parOn() { return switchOn(kP3Par); }
    inline bool p4moveOn() { return switchOn(kP4Move); }
    // Round B1 (SOTC_B1SOUND, A/B b1sound; off by default): the VBlank / sound thread on a host carrier (b1_sound.cpp).
    inline bool b1soundOn() { return switchOn(kB1Sound); }
    // Round B2 (SOTC_B2LOAD, A/B b2load; off by default): the loader / CD streaming thread on a host carrier (b2_load.cpp).
    inline bool b2loadOn() { return switchOn(kB2Load); }
    // Round B3 (SOTC_B3THREADS, A/B b3threads; off by default): t4 / t5 / t10 / t9 on host carriers (b3_threads.cpp).
    inline bool b3threadsOn() { return switchOn(kB3Threads); }
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
    // Round S10 (SOTC_S10DRAW, A/B `s10draw`): the model draw path without the GS layer cache (sotc_native.cpp s10draw).
    inline bool s10DrawOn() { return switchOn(kS10Draw); }
    // Round S12 (SOTC_S12DRAW, A/B `s12draw`): the immediate quads as host vertex records (sotc_native.cpp s12draw).
    inline bool s12DrawOn() { return switchOn(kS12Draw); }
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
// Decomp D7: a lifted function of D7's batch, in the table while SOTC_LIFTD7 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D7(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD7}
// Decomp D8: a lifted function of D8's batch, in the table while SOTC_LIFTD8 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D8(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD8}
// Decomp D9: a lifted function of D9's batch, in the table while SOTC_LIFTD9 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D9(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD9}
// Decomp D10: a lifted function of D10's batch, in the table while SOTC_LIFTD10 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D10(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD10}
// Decomp D11: a lifted function of D11's batch, in the table while SOTC_LIFTD11 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D11(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD11}
// Decomp D12: a lifted function of D12's batch, in the table while SOTC_LIFTD12 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D12(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD12}
// Decomp D13: a lifted function of D13's batch, in the table while SOTC_LIFTD13 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D13(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD13}
// Decomp D14: a lifted function of D14's batch, in the table while SOTC_LIFTD14 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D14(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD14}
// Decomp D15: a lifted function of D15's batch, in the table while SOTC_LIFTD15 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D15(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD15}
// Decomp D16: a lifted function of D16's batch, in the table while SOTC_LIFTD16 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D16(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD16}
// Decomp D17: a lifted function of D17's batch, in the table while SOTC_LIFTD17 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D17(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD17}
// Decomp D20: a lifted function of D20's batch, in the table while SOTC_LIFTD20 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D20(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD20}
// Decomp D18: a lifted function of D18's batch, in the table while SOTC_LIFTD18 is on.
#define SOTC_DECOMP_LIFT_FUNCTION_D18(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD18}
// Decomp D5: a lifted copy of a function a native replaces (its slot holds the native): what sotc_decomp::recompiled()
// gives the natives (their fallbacks to the original code) while SOTC_LIFTD5 is on. Registered as "<name>_fb", so
// SOTC_DECOMP_SKIP=<name> (the native left out) puts the copy in the table, where the difftest can check it.
#define SOTC_DECOMP_LIFT_FALLBACK(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD5}
// Decomp D6: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D6's round: served while SOTC_LIFTD6 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D6(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD6}
// Decomp D7: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D7's round: served while SOTC_LIFTD7 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D7(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD7}
// Decomp D8: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D8's round: served while SOTC_LIFTD8 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D8(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD8}
// Decomp D9: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D9's round: served while SOTC_LIFTD9 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D9(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD9}
// Decomp D10: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D10's round: served while SOTC_LIFTD10 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D10(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD10}
// Decomp D11: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D11's round: served while SOTC_LIFTD11 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D11(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD11}
// Decomp D12: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D12's round: served while SOTC_LIFTD12 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D12(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD12}
// Decomp D13: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D13's round: served while SOTC_LIFTD13 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D13(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD13}
// Decomp D14: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D14's round: served while SOTC_LIFTD14 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D14(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD14}
// Decomp D15: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D15's round: served while SOTC_LIFTD15 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D15(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD15}
// Decomp D16: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D16's round: served while SOTC_LIFTD16 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D16(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD16}
// Decomp D17: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D17's round: served while SOTC_LIFTD17 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D17(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD17}
// Decomp D20: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D20's round: served while SOTC_LIFTD20 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D20(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD20}
// Decomp D18: a fallback copy (as SOTC_DECOMP_LIFT_FALLBACK) lifted in D18's round: served while SOTC_LIFTD18 is on.
#define SOTC_DECOMP_LIFT_FALLBACK_D18(address, name, fn, original) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name##_fb{address, #name "_fb", fn, false, false, false, false, false, false, true, original, ::sotc_decomp::kLiftD18}
// Decomp D4 (test builds): another lifter version of a lifted function, in its place while liftalt is off.
#define SOTC_DECOMP_LIFT_ALT(address, name, fn) \
    static const ::sotc_decomp::LiftAltRegistrar sotc_decomp_liftalt_##name{address, fn}
// A context switch that returns to its caller after a round trip (wake).
#define SOTC_DECOMP_SWITCH_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, false}
// A context switch that returns into the other context (sleep).
#define SOTC_DECOMP_SWITCH_ONCE_FUNCTION(address, name, fn) \
    static const ::sotc_decomp::Registrar sotc_decomp_registrar_##name{address, #name, fn, false, true, true}
