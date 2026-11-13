// The decompiled functions' registry and their differential test (see sotc_decomp.h).
#include "sotc_decomp.h"
#if !defined(__vita__)
#include <dlfcn.h>
#endif
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_fast_paths.h"
#include "sotc_prof.h"
#include "runtime/native_renderer.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <cstdarg>
#include <cstddef>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

namespace sotc_decomp
{
    std::vector<Function> &registry()
    {
        static std::vector<Function> functions;
        return functions;
    }

    std::vector<std::pair<uint32_t, PS2Runtime::RecompiledFunction>> &liftAlts()
    {
        static std::vector<std::pair<uint32_t, PS2Runtime::RecompiledFunction>> alts;
        return alts;
    }

    namespace
    {
        bool listed(const char *list, const char *name)
        {
            if (!list)
                return false;
            if (std::strcmp(list, "all") == 0)
                return true;
            const std::string needle = std::string(",") + name + ",";
            return (std::string(",") + list + ",").find(needle) != std::string::npos;
        }

        // ---- differential test --------------------------------------------
        // One slot (a trampoline) per checked function. Round 40: was 64, so
        // `all` left every function registered after the 64th unchecked.
        constexpr int kMaxChecked = 512;
        struct Checked
        {
            const Function *function = nullptr;
            PS2Runtime::RecompiledFunction recompiled = nullptr;
            std::atomic<uint64_t> calls{0}, checks{0}, mismatches{0};
            uint64_t next = 0; // the next call checked past SOTC_DIFFTEST_FIRST
        };
        // SOTC_DIFFTEST_MAX=<n> (round 40; default 1000 with `all`, 0 = off
        // for a list of names): after each n checks of a function past
        // SOTC_DIFFTEST_FIRST its stride (SOTC_DIFFTEST_EVERY) doubles, so a
        // hot function's checks spread over the whole run without taking it
        // over, and `all` stays fast with every registered function in it.
        uint64_t g_maxChecks = 0;
        // Checks in progress (nested: a checked function the native or the
        // recompiled leg of another check reaches). Each depth has its own
        // snapshots; deeper than kMaxDepth the call runs recompiled, unchecked.
        constexpr int kMaxDepth = 3;
        int g_depth = 0;
        std::array<Checked, kMaxChecked> g_checked;
        int g_checkedUsed = 0; // slots given out (install, behindHook)
        // Natives a hook calls (behindHook): install() leaves their table entry to the hook.
        std::vector<uint32_t> g_behindHook;
        std::unordered_map<uint32_t, PS2Runtime::RecompiledFunction> g_recompiled;
        // recompiled()'s lookups, read-only once install() has run: open
        // addressing on a power-of-two table (the map's prime modulo is a
        // software division on the Vita: ~0.4% of the game thread, round 22).
        struct RecompiledSlot
        {
            uint32_t address = 0;
            PS2Runtime::RecompiledFunction fn = nullptr;
        };
        std::vector<RecompiledSlot> g_recompiledTable;
        uint32_t g_recompiledMask = 0;
        inline uint32_t recompiledHash(uint32_t a) { return (a >> 2) * 2654435761u; }
        // Decomp D5: lifted copies of functions whose slots hold a native or a hook (SOTC_DECOMP_LIFT_FALLBACK, or any
        // lifted function left to a hook): recompiled() gives the copy while its switch is on, the generated function
        // otherwise (patched in place when the switch changes: helper threads may be reading the table).
        struct LiftFallback
        {
            uint32_t address;
            PS2Runtime::RecompiledFunction lifted, generated;
            int liftSwitch;
        };
        std::vector<LiftFallback> g_liftFallback;
        void applyLiftFallbacks(int which)
        {
            if (g_recompiledMask == 0u)
                return;
            for (const LiftFallback &fb : g_liftFallback)
            {
                if (which >= 0 && fb.liftSwitch != which)
                    continue;
                const PS2Runtime::RecompiledFunction fn = switchOn(fb.liftSwitch) ? fb.lifted : fb.generated;
                for (uint32_t i = recompiledHash(fb.address) & g_recompiledMask;; i = (i + 1u) & g_recompiledMask)
                {
                    if (g_recompiledTable[i].address == fb.address)
                    {
                        g_recompiledTable[i].fn = fn;
                        break;
                    }
                    if (g_recompiledTable[i].address == 0u)
                        break;
                }
            }
        }
        void buildRecompiledTable()
        {
            uint32_t size = 64;
            while (size < g_recompiled.size() * 4u)
                size <<= 1;
            g_recompiledTable.assign(size, RecompiledSlot{});
            g_recompiledMask = size - 1u;
            for (const auto &[address, fn] : g_recompiled)
            {
                uint32_t i = recompiledHash(address) & g_recompiledMask;
                while (g_recompiledTable[i].address != 0u)
                    i = (i + 1u) & g_recompiledMask;
                g_recompiledTable[i] = {address, fn};
            }
            applyLiftFallbacks(-1);
        }

#if !defined(__vita__)
        // SOTC_DIFFTEST_WATCH=1 (Mac, round 47): at a function's first RAM
        // mismatch the native leg runs again from the same input with its
        // guest calls (any depth) through callGuestSlow, which prints each
        // call that changes the differing byte: target, return address,
        // depth and the registers it was entered with.
        uint32_t g_watchAddr = 0, g_watchOn = 0;
        int g_watchDepth = 0;
        // The first (innermost) call seen changing it; the second rerun logs
        // every store made inside that call (g_ps2LogAllWrites).
        uint32_t g_watchTarget = 0, g_watchRa = 0, g_watchLogTarget = 0, g_watchLogRa = 0;
#endif

        // The registers a caller may rely on after the call.
        // (v1 too would be for 128-bit returns, which none of these has.)
        constexpr int kCompared[] = {2, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31};
        // Below the caller's sp: the callee's own frame, dead once it returns.
        constexpr uint32_t kDeadStack = 0x10000u;

        struct Snapshot
        {
            std::vector<uint8_t> ram, scratch;
        };

        void report(const Checked &c, const R5900Context &input, const char *what)
        {
            const R5900Context *in = &input;
            std::fprintf(stderr, "[difftest] %s @%08x MISMATCH (%s) a0 %016llx a1 %016llx a2 %016llx a3 %016llx f12 %g f13 %g\n", c.function->name,
                         c.function->address, what, (unsigned long long)GPR_U64(in, 4), (unsigned long long)GPR_U64(in, 5),
                         (unsigned long long)GPR_U64(in, 6), (unsigned long long)GPR_U64(in, 7), in->f[12], in->f[13]);
        }

        // Runs recompiled code until it returns to its caller: a generated
        // function can leave with pc on a continuation in another one (a
        // branch past its end, a checkpoint), which the dispatcher would run.
        void runToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction fn)
        {
            const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
            fn(rdram, ctx, runtime);
            runtime->checkTransfer(); // round 52: a thread switch below takes this frame off as before
            while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
            {
                PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                // Decomp D6 (checking): SOTC_DIFFTEST_LEGLOG=1: each continuation a leg needed (a checkpoint or a
                // deferred call left the code at ctx->pc).
                static const bool legLog = ps2_diag::env("SOTC_DIFFTEST_LEGLOG") != nullptr;
                if (legLog)
                    std::fprintf(stderr, "[difftest] leg continues at %08x (ra %08x sp %08x, want %08x / %08x) fn %p unwinding %d\n", ctx->pc, GPR_U32(ctx, 31),
                                 GPR_U32(ctx, 29), ra, sp, reinterpret_cast<void *>(next), runtime->m_eeClock.unwinding ? 1 : 0);
                if (!next)
                    break;
                next(rdram, ctx, runtime);
                runtime->checkTransfer();
            }
        }

#if !defined(__vita__)
        // ---- Decomp D6: the switch-aware difftest (trace check), SOTC_DIFFTEST_TRACE=all|<names> --------------------
        // The two-leg check above runs the native and then the recompiled code from the same input. A function that may
        // block or switch threads cannot be run twice: its callees change the kernel's state (semaphores, the thread
        // queues, the IOP's requests), which the RAM snapshot does not hold, and a thread switch leaves the leg for
        // good (D5's dt5 hung the Mac's load that way). The trace check runs the generated function once, for real,
        // and records each event its own body makes (ps2_difftrace.h: a call, a jump out, a checkpoint that reaches the
        // scheduler, a syscall / break, an I/O access): the state the body left at it (every register and the VU0 /
        // FPU state in the context, the EE clock, main RAM, the scratchpad, VU0's data memory) and the state after it.
        // Then the lifted function runs from the same input with the events answered from the record: at each one its
        // state must be the generated code's (else a mismatch, and the lifted run is wound up), then the recorded state
        // after the event is put in place and its result returned; at its end its state must be the generated code's
        // end state. Last the generated code's end state is put back and the game goes on from it (a thread switch's
        // exception rethrown). Sound because the lifted code reaches nothing but its context, guest memory and the
        // clock between events (lift_d1.py refuses any other runtime access; a lifted function's own code ends at a
        // thread switch: the rest runs in the generated function, resumed at its labels), so equal states at every
        // event and at the end are equal behaviour on that input. Not for functions lifted with --inline or --summary
        // (an inlined call is no event; a summary keeps registers in locals across a call).
        namespace trace
        {
            constexpr size_t kRamPage = 4096u, kSmallPage = 256u;
            struct Diff
            {
                std::vector<uint32_t> pages;
                std::vector<uint8_t> data;
            };
            struct Clock
            {
                uint64_t cycle = 0;
#if PS2X_EE_CLOCK64
                uint64_t gate = 0;
#else
                uint32_t run = 0, limit = 0;
#endif
                bool unwinding = false, transferred = false;
                uint64_t now() const
                {
#if PS2X_EE_CLOCK64
                    return cycle;
#else
                    return cycle + run;
#endif
                }
            };
            struct Event
            {
                ps2_difftrace::Kind kind = ps2_difftrace::kCall;
                uint32_t a = 0, b = 0;
                uint64_t v = 0, v2 = 0;
                R5900Context pre, post;
                Clock preClock, postClock;
                uint32_t preSerial = 0, postSerial = 0;
                Diff preRam, postRam, preSpr, postSpr, preVu, postVu;
                uint64_t ret = 0, retHi = 0;
                bool threw = false;
            };
            struct State
            {
                bool replay = false, stopped = false, overflow = false, threwInEvent = false;
                size_t cursor = 0, bytes = 0;
                std::vector<Event> events;
                uint8_t *rdram = nullptr, *spr = nullptr, *vu = nullptr;
                R5900Context *ctx = nullptr;
                std::vector<uint8_t> ramBefore, ramShadow, ramExp, sprBefore, sprShadow, sprExp, vuBefore, vuShadow, vuExp;
                std::string what; // the first difference
            };
            State g_t;
            bool g_active = false; // a trace check is running (nested checks run the generated code, unchecked)

            const char *kindName(ps2_difftrace::Kind k)
            {
                static const char *const names[] = {"call", "jump", "tail jump", "checkpoint", "syscall", "break", "I/O read", "I/O write"};
                return names[k];
            }
            Clock clockNow(PS2Runtime *rt)
            {
                const EeClock &k = rt->m_eeClock;
                Clock c;
                c.cycle = k.cycle;
#if PS2X_EE_CLOCK64
                c.gate = k.gate;
#else
                c.run = k.run;
                c.limit = k.limit;
#endif
                c.unwinding = k.unwinding;
                c.transferred = k.transferred;
                return c;
            }
            void clockSet(PS2Runtime *rt, const Clock &c)
            {
                EeClock &k = rt->m_eeClock;
                k.cycle = c.cycle;
#if PS2X_EE_CLOCK64
                k.gate = c.gate;
#else
                k.run = c.run;
                k.limit = c.limit;
#endif
                k.unwinding = c.unwinding;
                k.transferred = c.transferred;
                if (k.pending.load(std::memory_order_relaxed)) // an event posted meanwhile: the next checkpoint still sees it
                    k.closeGate();
            }
            // Replay: the gate where the generated code took its next checkpoint (the clock it had there), else open:
            // the lifted code's checkpoints fall where the generated code's did, whatever other threads did to the gate
            // during either run (an early one is answered "nothing due", see onEvent).
            void armGate(PS2Runtime *rt)
            {
                State &t = g_t;
                EeClock &k = rt->m_eeClock;
                const bool next = t.cursor < t.events.size() && t.events[t.cursor].kind == ps2_difftrace::kCheckpoint;
#if PS2X_EE_CLOCK64
                k.gate = next ? t.events[t.cursor].preClock.cycle : ~uint64_t{0};
#else
                if (next && t.events[t.cursor].preClock.cycle == k.cycle)
                    k.limit = t.events[t.cursor].preClock.run;
                else if (next)
                    k.limit = 0u; // the base moved (cannot between events): at once, the clock compare decides
                else
                    k.limit = static_cast<uint32_t>(EeClock::kMaxLimit);
#endif
            }
            void diff(const uint8_t *live, uint8_t *shadow, size_t size, size_t page, Diff &out)
            {
                out.pages.clear();
                out.data.clear();
                for (size_t off = 0; off < size; off += page)
                    if (std::memcmp(live + off, shadow + off, page) != 0)
                    {
                        out.pages.push_back(static_cast<uint32_t>(off));
                        out.data.insert(out.data.end(), live + off, live + off + page);
                        std::memcpy(shadow + off, live + off, page);
                    }
            }
            void apply(const Diff &d, uint8_t *dst, size_t page)
            {
                for (size_t i = 0; i < d.pages.size(); ++i)
                    std::memcpy(dst + d.pages[i], d.data.data() + i * page, page);
            }
            // The first difference of two contexts in what code can leave behind (not insn_count / COP0).
            bool ctxDiffers(const R5900Context &x, const R5900Context &y, bool pcToo, std::string &what)
            {
                char buf[192];
                for (int r = 0; r < 32; ++r)
                    if (std::memcmp(&x.r[r], &y.r[r], 16) != 0)
                    {
                        uint64_t a[2], b[2];
                        std::memcpy(a, &x.r[r], 16);
                        std::memcpy(b, &y.r[r], 16);
                        std::snprintf(buf, sizeof(buf), "r%d lifted %016llx:%016llx generated %016llx:%016llx", r, (unsigned long long)a[1], (unsigned long long)a[0],
                                      (unsigned long long)b[1], (unsigned long long)b[0]);
                        what = buf;
                        return true;
                    }
                for (int i = 0; i < 32; ++i)
                    if (std::memcmp(&x.f[i], &y.f[i], 4) != 0)
                    {
                        uint32_t a, b;
                        std::memcpy(&a, &x.f[i], 4);
                        std::memcpy(&b, &y.f[i], 4);
                        std::snprintf(buf, sizeof(buf), "f%d lifted %08x generated %08x", i, a, b);
                        what = buf;
                        return true;
                    }
                for (int i = 0; i < 32; ++i)
                    if (std::memcmp(&x.vu0_vf[i], &y.vu0_vf[i], 16) != 0)
                    {
                        std::snprintf(buf, sizeof(buf), "vf%d", i);
                        what = buf;
                        return true;
                    }
                struct Field
                {
                    const char *name;
                    size_t offset, size;
                };
#define SOTC_TRACE_FIELD(f) Field{#f, offsetof(R5900Context, f), sizeof(R5900Context::f)}
                static const Field fields[] = {SOTC_TRACE_FIELD(hi), SOTC_TRACE_FIELD(lo), SOTC_TRACE_FIELD(hi1), SOTC_TRACE_FIELD(lo1), SOTC_TRACE_FIELD(sa),
                                               SOTC_TRACE_FIELD(vi), SOTC_TRACE_FIELD(vu0_q), SOTC_TRACE_FIELD(vu0_p), SOTC_TRACE_FIELD(vu0_i), SOTC_TRACE_FIELD(vu0_r),
                                               SOTC_TRACE_FIELD(vu0_acc), SOTC_TRACE_FIELD(vu0_status), SOTC_TRACE_FIELD(vu0_mac_flags),
                                               SOTC_TRACE_FIELD(vu0_clip_flags), SOTC_TRACE_FIELD(vu0_clip_flags2), SOTC_TRACE_FIELD(vu0_cf),
                                               SOTC_TRACE_FIELD(in_delay_slot), SOTC_TRACE_FIELD(branch_pc), SOTC_TRACE_FIELD(f_acc), SOTC_TRACE_FIELD(fcr31),
                                               SOTC_TRACE_FIELD(llbit), SOTC_TRACE_FIELD(lladdr)};
#undef SOTC_TRACE_FIELD
                const uint8_t *px = reinterpret_cast<const uint8_t *>(&x), *py = reinterpret_cast<const uint8_t *>(&y);
                for (const Field &f : fields)
                    if (std::memcmp(px + f.offset, py + f.offset, f.size) != 0)
                    {
                        what = f.name;
                        return true;
                    }
                if (pcToo && x.pc != y.pc)
                {
                    std::snprintf(buf, sizeof(buf), "pc lifted %08x generated %08x", x.pc, y.pc);
                    what = buf;
                    return true;
                }
                return false;
            }
            bool memDiffers(const uint8_t *live, const uint8_t *expected, size_t size, uint32_t base, const char *where, std::string &what)
            {
                if (std::memcmp(live, expected, size) == 0)
                    return false;
                size_t i = 0;
                while (live[i] == expected[i])
                    ++i;
                char buf[128];
                std::snprintf(buf, sizeof(buf), "%s %08x lifted %02x generated %02x", where, base + static_cast<uint32_t>(i), live[i], expected[i]);
                what = buf;
                return true;
            }
            void fail(const char *fmt, ...)
            {
                if (!g_t.what.empty())
                    return;
                char buf[256];
                va_list ap;
                va_start(ap, fmt);
                std::vsnprintf(buf, sizeof(buf), fmt, ap);
                va_end(ap);
                g_t.what = buf;
            }

            uint64_t onEvent(PS2Runtime *rt, ps2_difftrace::Kind kind, uint8_t *, R5900Context *ctx, uint32_t a, uint32_t b, uint64_t v, uint64_t v2,
                             ps2_difftrace::Real real, void *arg)
            {
                State &t = g_t;
                static const size_t maxEvents = ps2_diag::env("SOTC_DIFFTEST_TRACE_MAX") ? std::strtoull(ps2_diag::env("SOTC_DIFFTEST_TRACE_MAX"), nullptr, 0) : 1500u;
                if (!t.replay)
                {
                    // the generated code, live: the state at the event, the event itself, the state after it
                    if (t.events.size() >= maxEvents || (ctx && ctx != t.ctx))
                    {
                        t.overflow = true; // past the budget (or another context): this call is not checked
                        ps2_difftrace::g_top = false;
                        return real(arg);
                    }
                    const size_t idx = t.events.size();
                    t.events.emplace_back();
                    {
                        Event &e = t.events[idx];
                        e.kind = kind;
                        e.a = a;
                        e.b = b;
                        e.v = v;
                        e.v2 = v2;
                        e.pre = *t.ctx;
                        e.preClock = clockNow(rt);
                        e.preSerial = rt->m_guestJumpSerial;
                        diff(t.rdram, t.ramShadow.data(), PS2_RAM_SIZE, kRamPage, e.preRam);
                        diff(t.spr, t.sprShadow.data(), PS2_SCRATCHPAD_SIZE, kSmallPage, e.preSpr);
                        diff(t.vu, t.vuShadow.data(), PS2_VU0_DATA_SIZE, kSmallPage, e.preVu);
                    }
                    ps2_difftrace::g_top = false;
                    auto after = [&](bool threw, uint64_t r)
                    {
                        Event &e = t.events[idx];
                        e.threw = threw;
                        e.ret = r;
                        e.retHi = (kind == ps2_difftrace::kIoRead && b == 16u) ? ps2_difftrace::g_ioHi : 0u;
                        e.post = *t.ctx;
                        e.postClock = clockNow(rt);
                        e.postSerial = rt->m_guestJumpSerial;
                        diff(t.rdram, t.ramShadow.data(), PS2_RAM_SIZE, kRamPage, e.postRam);
                        diff(t.spr, t.sprShadow.data(), PS2_SCRATCHPAD_SIZE, kSmallPage, e.postSpr);
                        diff(t.vu, t.vuShadow.data(), PS2_VU0_DATA_SIZE, kSmallPage, e.postVu);
                        t.bytes += sizeof(Event) + e.preRam.data.size() + e.postRam.data.size() + e.preSpr.data.size() + e.postSpr.data.size();
                    };
                    uint64_t r = 0;
                    try
                    {
                        r = real(arg);
                    }
                    catch (...)
                    {
                        after(true, 0u); // the body is left by a thread switch's exception: no more events
                        t.threwInEvent = true;
                        throw;
                    }
                    after(false, r);
                    if (t.bytes > (size_t(768) << 20))
                        t.overflow = true; // the record past 768 MB: this call is not checked, the rest runs untraced
                    ps2_difftrace::g_top = !t.overflow;
                    return r;
                }
                // the lifted code: the event answered from the record
                auto windUp = [&]() -> uint64_t
                {
                    t.stopped = true;
                    rt->m_eeClock.unwinding = true; // the lifted code returns at once (as on a thread switch)
                    return kind == ps2_difftrace::kCheckpoint ? 1u : 0u;
                };
                if (t.stopped)
                    return windUp();
                if (ctx && ctx != t.ctx)
                {
                    fail("an event on another context");
                    return windUp();
                }
                const size_t i = t.cursor;
                if (kind == ps2_difftrace::kCheckpoint)
                {
                    // The gate is armed at the generated code's next checkpoint (armGate); one taken before it is another
                    // host thread closing the gate (an event posted: the IOP, the VBlank timer): nothing is due then, the
                    // lifted code goes on (its slow path wrote its registers back and reads them again: no effect).
                    const Event *nx = i < t.events.size() ? &t.events[i] : nullptr;
                    const uint64_t now = clockNow(rt).now();
                    if (!nx || nx->kind != ps2_difftrace::kCheckpoint || now < nx->preClock.now())
                    {
                        armGate(rt);
                        return 0u;
                    }
                }
                if (i >= t.events.size())
                {
                    fail("event %zu (%s %08x) past the generated code's %zu", i, kindName(kind), a, t.events.size());
                    return windUp();
                }
                const Event &e = t.events[t.cursor++];
                if (e.kind != kind || e.a != a || e.b != b || e.v != v || e.v2 != v2)
                {
                    fail("event %zu: lifted %s %08x/%08x/%llx, generated %s %08x/%08x/%llx", i, kindName(kind), a, b, (unsigned long long)v, kindName(e.kind), e.a,
                         e.b, (unsigned long long)e.v);
                    return windUp();
                }
                std::string what;
                const bool io = kind == ps2_difftrace::kIoRead || kind == ps2_difftrace::kIoWrite;
                if (ctxDiffers(*t.ctx, e.pre, !io, what))
                {
                    fail("event %zu (%s %08x): %s", i, kindName(kind), a, what.c_str());
                    return windUp();
                }
                const Clock now = clockNow(rt);
                if (now.now() != e.preClock.now())
                {
                    fail("event %zu (%s %08x): EE clock lifted %llu generated %llu", i, kindName(kind), a, (unsigned long long)now.now(),
                         (unsigned long long)e.preClock.now());
                    return windUp();
                }
                apply(e.preRam, t.ramExp.data(), kRamPage);
                apply(e.preSpr, t.sprExp.data(), kSmallPage);
                apply(e.preVu, t.vuExp.data(), kSmallPage);
                if (memDiffers(t.rdram, t.ramExp.data(), PS2_RAM_SIZE, 0u, "ram", what) ||
                    memDiffers(t.spr, t.sprExp.data(), PS2_SCRATCHPAD_SIZE, 0x70000000u, "scratchpad", what) ||
                    memDiffers(t.vu, t.vuExp.data(), PS2_VU0_DATA_SIZE, 0x11004000u, "vu0 data", what))
                {
                    fail("event %zu (%s %08x): %s", i, kindName(kind), a, what.c_str());
                    return windUp();
                }
                // the event's result in place
                *t.ctx = e.post;
                clockSet(rt, e.postClock);
                armGate(rt);
                rt->m_guestJumpSerial = e.postSerial;
                apply(e.postRam, t.ramExp.data(), kRamPage);
                apply(e.postRam, t.rdram, kRamPage);
                apply(e.postSpr, t.sprExp.data(), kSmallPage);
                apply(e.postSpr, t.spr, kSmallPage);
                apply(e.postVu, t.vuExp.data(), kSmallPage);
                apply(e.postVu, t.vu, kSmallPage);
                if (e.threw)
                {
                    // the generated code's body ended here (a thread switch's exception): so does the lifted one's
                    t.stopped = true;
                    rt->m_eeClock.unwinding = true;
                    return kind == ps2_difftrace::kCheckpoint ? 1u : 0u;
                }
                if (kind == ps2_difftrace::kIoRead && b == 16u)
                    ps2_difftrace::g_ioHi = e.retHi;
                return e.ret;
            }

            // One trace check of c (its generated code live, then its lifted code against the record).
            void check(Checked &c, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
            {
                State &t = g_t;
                uint8_t *spr = runtime->memory().getScratchpad(), *vu = runtime->memory().getVU0Data();
                if (t.ramBefore.empty())
                {
                    t.ramBefore.resize(PS2_RAM_SIZE);
                    t.ramShadow.resize(PS2_RAM_SIZE);
                    t.ramExp.resize(PS2_RAM_SIZE);
                    t.sprBefore.resize(PS2_SCRATCHPAD_SIZE);
                    t.sprShadow.resize(PS2_SCRATCHPAD_SIZE);
                    t.sprExp.resize(PS2_SCRATCHPAD_SIZE);
                    t.vuBefore.resize(PS2_VU0_DATA_SIZE);
                    t.vuShadow.resize(PS2_VU0_DATA_SIZE);
                    t.vuExp.resize(PS2_VU0_DATA_SIZE);
                    ps2_difftrace::g_event = &onEvent;
                }
                std::memcpy(t.ramBefore.data(), rdram, PS2_RAM_SIZE);
                std::memcpy(t.ramShadow.data(), rdram, PS2_RAM_SIZE);
                std::memcpy(t.sprBefore.data(), spr, PS2_SCRATCHPAD_SIZE);
                std::memcpy(t.sprShadow.data(), spr, PS2_SCRATCHPAD_SIZE);
                std::memcpy(t.vuBefore.data(), vu, PS2_VU0_DATA_SIZE);
                std::memcpy(t.vuShadow.data(), vu, PS2_VU0_DATA_SIZE);
                const R5900Context input = *ctx;
                const Clock clock0 = clockNow(runtime);
                const uint32_t serial0 = runtime->m_guestJumpSerial;
                const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
                t.events.clear();
                t.replay = t.stopped = t.overflow = t.threwInEvent = false;
                t.cursor = 0;
                t.bytes = 0;
                t.what.clear();
                t.rdram = rdram;
                t.spr = spr;
                t.vu = vu;
                t.ctx = ctx;
                g_active = true;
                // 1. the generated code, live (a thread switch by returning comes back here with the clock's flags set)
                bool threw = false;
                ps2_difftrace::g_top = true;
                try
                {
                    c.recompiled(rdram, ctx, runtime);
                }
                catch (const EeDispatcherTransfer &)
                {
                    threw = true;
                }
                catch (...)
                {
                    ps2_difftrace::g_top = false;
                    g_active = false;
                    throw;
                }
                ps2_difftrace::g_top = false;
                const R5900Context out = *ctx;
                const Clock clockOut = clockNow(runtime);
                const uint32_t serialOut = runtime->m_guestJumpSerial;
                Diff endRam, endSpr, endVu;
                diff(rdram, t.ramShadow.data(), PS2_RAM_SIZE, kRamPage, endRam); // ramShadow: the generated code's end state now
                diff(spr, t.sprShadow.data(), PS2_SCRATCHPAD_SIZE, kSmallPage, endSpr);
                diff(vu, t.vuShadow.data(), PS2_VU0_DATA_SIZE, kSmallPage, endVu);
                const size_t events = t.events.size();
                bool checked = false;
                if (!t.overflow && (threw == t.threwInEvent))
                {
                    // 2. the lifted code from the input, its events answered from the record
                    std::memcpy(rdram, t.ramBefore.data(), PS2_RAM_SIZE);
                    std::memcpy(t.ramExp.data(), t.ramBefore.data(), PS2_RAM_SIZE);
                    std::memcpy(spr, t.sprBefore.data(), PS2_SCRATCHPAD_SIZE);
                    std::memcpy(t.sprExp.data(), t.sprBefore.data(), PS2_SCRATCHPAD_SIZE);
                    std::memcpy(vu, t.vuBefore.data(), PS2_VU0_DATA_SIZE);
                    std::memcpy(t.vuExp.data(), t.vuBefore.data(), PS2_VU0_DATA_SIZE);
                    *ctx = input;
                    clockSet(runtime, clock0);
                    runtime->m_guestJumpSerial = serial0;
                    t.replay = true;
                    armGate(runtime);
                    ps2_difftrace::g_top = true;
                    // SOTC_DIFFTEST_CONTROL=1: the generated code replayed against its own record (the mechanism's control)
                    static const bool control = ps2_diag::env("SOTC_DIFFTEST_CONTROL") != nullptr;
                    try
                    {
                        (control ? c.recompiled : c.function->native)(rdram, ctx, runtime);
                    }
                    catch (...)
                    {
                        fail("the lifted code threw");
                    }
                    ps2_difftrace::g_top = false;
                    t.replay = false;
                    if (!t.stopped && t.what.empty())
                    {
                        std::string what;
                        const Clock clockL = clockNow(runtime);
                        apply(endRam, t.ramExp.data(), kRamPage);
                        apply(endSpr, t.sprExp.data(), kSmallPage);
                        apply(endVu, t.vuExp.data(), kSmallPage);
                        if (t.cursor != events)
                            fail("the lifted code ends after %zu of the generated code's %zu events", t.cursor, events);
                        else if (ctxDiffers(*ctx, out, true, what))
                            fail("at the end: %s", what.c_str());
                        else if (clockL.now() != clockOut.now() || clockL.unwinding != clockOut.unwinding || clockL.transferred != clockOut.transferred)
                            fail("at the end: EE clock lifted %llu generated %llu", (unsigned long long)clockL.now(), (unsigned long long)clockOut.now());
                        else if (memDiffers(rdram, t.ramExp.data(), PS2_RAM_SIZE, 0u, "ram", what) ||
                                 memDiffers(spr, t.sprExp.data(), PS2_SCRATCHPAD_SIZE, 0x70000000u, "scratchpad", what) ||
                                 memDiffers(vu, t.vuExp.data(), PS2_VU0_DATA_SIZE, 0x11004000u, "vu0 data", what))
                            fail("at the end: %s", what.c_str());
                    }
                    checked = true;
                    // 3. the game goes on from the generated code's end state
                    std::memcpy(rdram, t.ramShadow.data(), PS2_RAM_SIZE);
                    std::memcpy(spr, t.sprShadow.data(), PS2_SCRATCHPAD_SIZE);
                    std::memcpy(vu, t.vuShadow.data(), PS2_VU0_DATA_SIZE);
                    *ctx = out;
                    clockSet(runtime, clockOut);
                    runtime->m_guestJumpSerial = serialOut;
                }
                g_active = false;
                if (checked)
                {
                    const bool bad = !t.what.empty();
                    if (bad)
                    {
                        char what[320];
                        std::snprintf(what, sizeof(what), "trace, %zu events%s: %s", events, threw ? ", left by a thread switch" : "", t.what.c_str());
                        report(c, input, what);
                        c.mismatches.fetch_add(1, std::memory_order_relaxed);
                    }
                    static std::unordered_map<const Checked *, uint64_t> eventCount, switches;
                    eventCount[&c] += events;
                    if (threw || clockOut.transferred || clockOut.unwinding)
                        ++switches[&c];
                    const uint64_t checks = c.checks.fetch_add(1, std::memory_order_relaxed) + 1u;
                    if (checks == 1u || checks % 100u == 0u)
                        std::fprintf(stderr, "[difftest] %s: %llu checks, %llu mismatches (%llu calls; trace: %llu events, %llu left by a switch or checkpoint)\n",
                                     c.function->name, (unsigned long long)checks, (unsigned long long)c.mismatches.load(),
                                     (unsigned long long)c.calls.load(), (unsigned long long)eventCount[&c], (unsigned long long)switches[&c]);
                }
                else
                {
                    static uint64_t skipped = 0;
                    if ((++skipped & 63u) == 1u)
                        std::fprintf(stderr, "[difftest] %s: trace not checked (%s; %llu such calls in all)\n", c.function->name,
                                     t.overflow ? "past SOTC_DIFFTEST_TRACE_MAX events" : "an exception outside an event", (unsigned long long)skipped);
                }
                if (threw)
                    throw EeDispatcherTransfer{};
                // a continuation left in another function (a branch past the end), as runToReturn: live, after the check
                if (!clockOut.unwinding && !clockOut.transferred)
                    while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
                    {
                        PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                        if (!next)
                            break;
                        next(rdram, ctx, runtime);
                        if (runtime->m_eeClock.unwinding || runtime->m_eeClock.transferred)
                            break;
                    }
            }
            bool wanted(const Checked &c)
            {
                static const char *const list = ps2_diag::env("SOTC_DIFFTEST_TRACE");
                return list && listed(list, c.function->name);
            }
        }
#endif

        void checkedCallImpl(Checked &c, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            static const uint64_t first = ps2_diag::env("SOTC_DIFFTEST_FIRST") ? std::strtoull(ps2_diag::env("SOTC_DIFFTEST_FIRST"), nullptr, 0) : 200u;
            static const uint64_t every = std::max<uint64_t>(1u, ps2_diag::env("SOTC_DIFFTEST_EVERY") ? std::strtoull(ps2_diag::env("SOTC_DIFFTEST_EVERY"), nullptr, 0) : 1000u);
#if !defined(__vita__)
            // Decomp D6: inside a trace check's live run (a callee): the generated code, unchecked.
            if (trace::g_active)
            {
                c.recompiled(rdram, ctx, runtime);
                return;
            }
#endif
            const uint64_t n = c.calls.fetch_add(1, std::memory_order_relaxed);
#if !defined(__vita__)
            if (trace::wanted(c))
            {
                // a resume through the entry slot (ctx->pc on a label), the stride, or a two-leg check running: as without
                if (ctx->pc != c.function->address || g_depth > 0 || (n >= first && n < c.next))
                {
                    c.recompiled(rdram, ctx, runtime);
                    return;
                }
                if (n >= first)
                {
                    const uint64_t past = c.checks.load(std::memory_order_relaxed) > first ? c.checks.load(std::memory_order_relaxed) - first : 0u;
                    const unsigned doublings = g_maxChecks ? static_cast<unsigned>(std::min<uint64_t>(40u, past / g_maxChecks)) : 0u;
                    c.next = n + (every << doublings);
                }
                trace::check(c, rdram, ctx, runtime);
                return;
            }
#endif
            // Too deep (a chain of checked functions reached from each
            // other's legs): the recompiled code, unchecked.
            // Round 126: SOTC_DIFFTEST_DEPTH=<1..3> (default 3): how deep checks nest (each level holds two 32 MB copies).
            static const int maxDepth = ps2_diag::env("SOTC_DIFFTEST_DEPTH") ? std::max(1, std::min(kMaxDepth, std::atoi(ps2_diag::env("SOTC_DIFFTEST_DEPTH")))) : kMaxDepth;
            if (g_depth >= maxDepth || (n >= first && n < c.next))
            {
                c.recompiled(rdram, ctx, runtime);
                return;
            }
            // Round 52: a thread switch inside a checked leg throws, as it
            // always did (the legs go on after their guest code).
            const EeScheduler::ThrowScope noReturn(runtime->eeScheduler());
            if (n >= first)
            {
                const uint64_t past = c.checks.load(std::memory_order_relaxed) > first ? c.checks.load(std::memory_order_relaxed) - first : 0u;
                const unsigned doublings = g_maxChecks ? static_cast<unsigned>(std::min<uint64_t>(40u, past / g_maxChecks)) : 0u;
                c.next = n + (every << doublings);
            }
            uint8_t *scratch = runtime->memory().getScratchpad();
            const int depth = g_depth;
            static Snapshot snapshots[kMaxDepth][2];
            Snapshot &before = snapshots[depth][0], &nativeAfter = snapshots[depth][1];
            before.ram.assign(rdram, rdram + PS2_RAM_SIZE);
            before.scratch.assign(scratch, scratch + PS2_SCRATCHPAD_SIZE);
            const R5900Context input = *ctx;
            const bool unwindingIn = runtime->m_eeClock.unwinding, pendingIn = runtime->m_eeClock.pending.load(std::memory_order_relaxed);

            ++g_depth;
            // Decomp D4: a thread switch inside a leg throws past this frame (ThrowScope above); the depth went up for
            // good each time, and after kMaxDepth such throws every later call ran unchecked (D4's walk: no check after
            // boot). The guard puts it back and says so (that call is not checked).
            struct DepthGuard
            {
                const char *name;
                bool armed = true;
                ~DepthGuard()
                {
                    if (!armed)
                        return;
                    --g_depth;
                    std::fprintf(stderr, "[difftest] %s: a leg left by a thread switch, this call not checked\n", name);
                }
            } depthGuard{c.function->name};
            // SOTC_DIFFTEST_DRAWS=1: the native renderer's draws of each leg
            // (native_renderer::setDrawTap) compared too.
            static const bool drawsOn = ps2_diag::env("SOTC_DIFFTEST_DRAWS") != nullptr;
            const bool draws = drawsOn && depth == 0; // the outermost check's draws only
            static std::vector<uint64_t> drawsNative, drawsRecompiled;
            drawsNative.clear();
            drawsRecompiled.clear();
            if (draws)
                native_renderer::setDrawTap(&drawsNative);
            // SOTC_DIFFTEST_CONTROL=1: the recompiled code in both runs, for
            // the mismatches that come from the state, not the rewrite (a
            // function whose result depends on the clock, interrupts).
            static const bool control = ps2_diag::env("SOTC_DIFFTEST_CONTROL") != nullptr;
            // Decomp D6 (checking): SOTC_DIFFTEST_SWAP=1: the recompiled code in the first leg and the native in the second
            // (reported with the names as without it; the game goes on from the native's leg): a difference that follows
            // the order is the state, not the code.
            static const bool swapLegs = ps2_diag::env("SOTC_DIFFTEST_SWAP") != nullptr;
            const PS2Runtime::RecompiledFunction leg = control || swapLegs ? c.recompiled : c.function->native;
            const PS2Runtime::RecompiledFunction leg2 = swapLegs ? c.function->native : c.recompiled;
            // SOTC_DIFFTEST_CYCLES=1 (checking, round 15): the EE cycles each
            // leg charges (the game's time step comes from the EE clock).
            static const bool cyclesOn = ps2_diag::env("SOTC_DIFFTEST_CYCLES") != nullptr;
            const bool cycles = cyclesOn && depth == 0;
            static uint64_t cyclesNative = 0, cyclesRecompiled = 0;
            const uint64_t clock0 = runtime->m_eeClock.now();
            const uint64_t clockBase = runtime->m_eeClock.cycle;
#if !PS2X_EE_CLOCK64
            const uint32_t clockRun = runtime->m_eeClock.run;
#endif
            // SOTC_DIFFTEST_SPOISON=1 (round 48, checking): both legs run with
            // the caller's s0-s7 / fp replaced by marks (0x5e5e...r), so where
            // the native leaves a caller's register in place at a guest call
            // while the original has set its own value, a callee that saves it
            // and another that copies the stale slot into RAM (round 47's leak)
            // shows as a mismatch whatever the caller held; a mark left in a
            // register at the end is the caller's value put back.
            static const bool spoison = ps2_diag::env("SOTC_DIFFTEST_SPOISON") != nullptr;
            const bool poison = spoison && !c.function->switches;
            auto mark = [](int r) { return 0x5E5E5E5E5E5E0000ull | static_cast<uint64_t>(r); };
            if (poison)
            {
                for (int r = 16; r <= 23; ++r)
                    SET_GPR_U64(ctx, r, mark(r));
                SET_GPR_U64(ctx, 30, mark(30));
            }
            if (c.function->once)
                leg(rdram, ctx, runtime);
            else
                runToReturn(rdram, ctx, runtime, leg);
            if (poison)
            {
                for (int r = 16; r <= 23; ++r)
                    if (GPR_U64(ctx, r) == mark(r))
                        std::memcpy(&ctx->r[r], &input.r[r], 16);
                if (GPR_U64(ctx, 30) == mark(30))
                    std::memcpy(&ctx->r[30], &input.r[30], 16);
            }
            const uint64_t clock1 = runtime->m_eeClock.now();
            const R5900Context nativeOut = *ctx;
            const R5900Context *native = &nativeOut;
            nativeAfter.ram.assign(rdram, rdram + PS2_RAM_SIZE);
            nativeAfter.scratch.assign(scratch, scratch + PS2_SCRATCHPAD_SIZE);

            std::memcpy(rdram, before.ram.data(), PS2_RAM_SIZE);
            std::memcpy(scratch, before.scratch.data(), PS2_SCRATCHPAD_SIZE);
            *ctx = input;
            if (draws)
                native_renderer::setDrawTap(&drawsRecompiled);
            // SOTC_DIFFTEST_SAMECLOCK=1 (round 48, checking): the recompiled leg
            // starts at the EE clock the native one started at (a sub-thread a
            // wake runs reads the timer: the legs saw different times).
            static const bool sameClock = ps2_diag::env("SOTC_DIFFTEST_SAMECLOCK") != nullptr;
            if (sameClock && depth == 0)
            {
                runtime->m_eeClock.cycle = clockBase;
#if !PS2X_EE_CLOCK64
                runtime->m_eeClock.run = clockRun;
#endif
                runtime->m_eeClock.closeGate();
            }
            // SOTC_DIFFTEST_SWITCH=scriptvm,isys,... (round 48): those subsystem
            // switches off during the recompiled leg, so a check of a function
            // above them (isysWakeupSubThread: a whole resumed sub-thread)
            // compares the subsystem's natives with the recompiled code.
            static const std::vector<int> legSwitches = []
            {
                std::vector<int> out;
                const char *list = ps2_diag::env("SOTC_DIFFTEST_SWITCH");
                for (int i = 0; list && i < kSwitchCount; ++i)
                {
                    std::string name = kSwitchEnv[i] + 5; // past "SOTC_"
                    for (char &ch : name)
                        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
                    if (listed(list, name.c_str()))
                        out.push_back(i);
                }
                return out;
            }();
            const bool offLeg = depth == 0 && !legSwitches.empty();
            if (offLeg)
                for (int i : legSwitches)
                    setSwitch(i, false);
            // The same marks in the recompiled leg: a register the original
            // leaves alone is the caller's (the mark) in both legs, so only
            // one the native fails to set as the original does can differ.
            if (poison)
            {
                for (int r = 16; r <= 23; ++r)
                    SET_GPR_U64(ctx, r, mark(r));
                SET_GPR_U64(ctx, 30, mark(30));
            }
            if (c.function->once)
                leg2(rdram, ctx, runtime);
            else
                runToReturn(rdram, ctx, runtime, leg2);
            if (poison)
            {
                for (int r = 16; r <= 23; ++r)
                    if (GPR_U64(ctx, r) == mark(r))
                        std::memcpy(&ctx->r[r], &input.r[r], 16);
                if (GPR_U64(ctx, 30) == mark(30))
                    std::memcpy(&ctx->r[30], &input.r[30], 16);
            }
            if (offLeg)
                for (int i : legSwitches)
                    setSwitch(i, true);
            // The clock never goes back past where the native leg left it (a
            // wait loop around a checked call must still see time pass).
            if (sameClock && depth == 0 && runtime->m_eeClock.now() < clock1)
            {
                runtime->m_eeClock.fold();
                runtime->m_eeClock.cycle = clock1;
                runtime->m_eeClock.closeGate();
            }
            depthGuard.armed = false;
            --g_depth;
            if (cycles)
            {
                cyclesNative += clock1 - clock0;
                cyclesRecompiled += runtime->m_eeClock.now() - clock1;
                // The recompiled leg's charge stays (the clock as without the check).
                runtime->m_eeClock.cycle -= clock1 - clock0; // the base: the clock moves the same
                runtime->m_eeClock.closeGate();                // the gate counted again from it
                if ((c.checks.load(std::memory_order_relaxed) + 1u) % 100u == 0u)
                    std::fprintf(stderr, "[difftest] %s: EE cycles native %llu recompiled %llu (all checks)\n", c.function->name,
                                 (unsigned long long)cyclesNative, (unsigned long long)cyclesRecompiled);
            }
            c.checks.fetch_add(1, std::memory_order_relaxed);
            if (draws)
            {
                native_renderer::setDrawTap(nullptr);
                static std::atomic<uint64_t> drawChecks{0}, drawMismatches{0}, drawCount{0};
                const bool same = drawsNative == drawsRecompiled;
                drawCount += drawsRecompiled.size();
                const uint64_t n = ++drawChecks, bad = drawMismatches += same ? 0u : 1u;
                if (!same)
                {
                    size_t i = 0;
                    while (i < drawsNative.size() && i < drawsRecompiled.size() && drawsNative[i] == drawsRecompiled[i])
                        ++i;
                    std::fprintf(stderr, "[difftest] %s DRAWS MISMATCH: native %zu recompiled %zu entries, first difference at %zu\n", c.function->name,
                                 drawsNative.size(), drawsRecompiled.size(), i);
                }
                if (n == 1u || n % 100u == 0u)
                    std::fprintf(stderr, "[difftest] %s: draws compared in %llu checks (%llu entries), %llu differ\n", c.function->name,
                                 (unsigned long long)n, (unsigned long long)drawCount.load(), (unsigned long long)bad);
            }

            bool bad = false;
            // Round 126: SOTC_DIFFTEST_NOV0=1: v0 not compared (void natives that leave another v0 than the original's last scratch value).
            static const bool noV0 = std::getenv("SOTC_DIFFTEST_NOV0") != nullptr;
            for (int r : kCompared)
                if ((r != 2 || (!c.function->returnsFloat && !noV0)) && GPR_U64(ctx, r) != GPR_U64(native, r))
                {
                    char what[96];
                    std::snprintf(what, sizeof(what), "r%d recompiled %016llx native %016llx", r, (unsigned long long)GPR_U64(ctx, r),
                                  (unsigned long long)GPR_U64(native, r));
                    report(c, input, what);
                    bad = true;
                    break;
                }
            // SOTC_DIFFTEST_ALLREGS=1: every register for every function (two
            // generators' code for the same function: SOTC_LEAN_CHECK with the
            // lean copies from the other generator).
            static const bool allRegs = ps2_diag::env("SOTC_DIFFTEST_ALLREGS") != nullptr;
            if (!bad && (c.function->switches || allRegs))
            {
                // Every register: the other context's set was loaded.
                for (int r = 0; r < 32 && !bad; ++r)
                    if (std::memcmp(&ctx->r[r], &native->r[r], 16) != 0)
                    {
                        // Decomp D6: both legs' 128 bits (high:low) and the input's, so an upper-half difference shows as one.
                        char what[192];
                        uint64_t rv[2], nv[2], iv[2];
                        std::memcpy(rv, &ctx->r[r], 16);
                        std::memcpy(nv, &native->r[r], 16);
                        std::memcpy(iv, &input.r[r], 16);
                        std::snprintf(what, sizeof(what), "r%d (128 bits) recompiled %016llx:%016llx native %016llx:%016llx input %016llx:%016llx", r,
                                      (unsigned long long)rv[1], (unsigned long long)rv[0], (unsigned long long)nv[1], (unsigned long long)nv[0],
                                      (unsigned long long)iv[1], (unsigned long long)iv[0]);
                        report(c, input, what);
                        bad = true;
                    }
                for (int i = 0; i < 32 && !bad; ++i)
                    if (std::memcmp(&ctx->f[i], &native->f[i], 4) != 0)
                    {
                        char what[96];
                        uint32_t x, y;
                        std::memcpy(&x, &ctx->f[i], 4);
                        std::memcpy(&y, &native->f[i], 4);
                        std::snprintf(what, sizeof(what), "f%d recompiled %08x native %08x", i, x, y);
                        report(c, input, what);
                        bad = true;
                    }
                if (!bad && (ctx->hi != native->hi || ctx->lo != native->lo || ctx->hi1 != native->hi1 || ctx->lo1 != native->lo1 || ctx->sa != native->sa))
                {
                    report(c, input, "hi / lo / sa");
                    bad = true;
                }
            }
            // SOTC_DIFFTEST_VU0=1 (checking, round 14): the VU0 registers too
            // (vf0-vf31, ACC), which a caller may go on using.
            static const bool vu0Regs = std::getenv("SOTC_DIFFTEST_VU0") != nullptr;
            if (!bad && vu0Regs)
            {
                for (int i = 0; i < 32 && !bad; ++i)
                    if (std::memcmp(&ctx->vu0_vf[i], &native->vu0_vf[i], 16) != 0)
                    {
                        char what[32];
                        std::snprintf(what, sizeof(what), "vf%d", i);
                        report(c, input, what);
                        bad = true;
                    }
                if (!bad && std::memcmp(&ctx->vu0_acc, &native->vu0_acc, 16) != 0)
                {
                    report(c, input, "ACC");
                    bad = true;
                }
            }
            // Round 126: SOTC_DIFFTEST_FTOL=<relative> (port natives, not bit-exact): a word of f0 or RAM that differs
            // passes when both are finite floats within that relative difference (or 1e-5 absolute); the largest
            // relative difference accepted is reported with each check line ([difftest] ftol ...).
            static const float ftol = ps2_diag::env("SOTC_DIFFTEST_FTOL") ? std::strtof(ps2_diag::env("SOTC_DIFFTEST_FTOL"), nullptr) : 0.0f;
            auto floatClose = [&](uint32_t a, uint32_t b, double &rel)
            {
                float fa, fb;
                std::memcpy(&fa, &a, 4);
                std::memcpy(&fb, &b, 4);
                if (!std::isfinite(fa) || !std::isfinite(fb))
                    return false;
                const double d = std::fabs(static_cast<double>(fa) - fb), m = std::max(std::fabs(static_cast<double>(fa)), std::fabs(static_cast<double>(fb)));
                rel = m > 0.0 ? d / m : 0.0;
                return d <= 1e-5 || rel <= ftol;
            };
            static double ftolWorst = 0.0;
            static uint64_t ftolWords = 0;
            bool f0Close = false;
            if (!bad && ftol > 0.0f && std::memcmp(&ctx->f[0], &nativeOut.f[0], 4) != 0)
            {
                uint32_t a, b;
                double rel = 0.0;
                std::memcpy(&a, &ctx->f[0], 4);
                std::memcpy(&b, &nativeOut.f[0], 4);
                if (floatClose(a, b, rel))
                {
                    f0Close = true;
                    ftolWorst = std::max(ftolWorst, rel);
                    ++ftolWords;
                }
            }
            if (!bad && !f0Close && std::memcmp(&ctx->f[0], &nativeOut.f[0], 4) != 0)
            {
                char what[96];
                uint32_t a, b;
                std::memcpy(&a, &ctx->f[0], 4);
                std::memcpy(&b, &nativeOut.f[0], 4);
                std::snprintf(what, sizeof(what), "f0 recompiled %08x native %08x", a, b);
                report(c, input, what);
                bad = true;
            }
            if (!bad && ctx->pc != nativeOut.pc)
            {
                report(c, input, "returns elsewhere");
                bad = true;
            }
            auto compareMemory = [&](const uint8_t *live, uint8_t *native, size_t size, uint32_t base, const char *where)
            {
                if (bad || std::memcmp(live, native, size) == 0)
                    return;
                if (ftol > 0.0f)
                {
                    // the words that differ: close floats are made equal in the native copy, then compared again
                    for (size_t k = 0; k + 4 <= size; k += 4)
                    {
                        uint32_t a, b;
                        std::memcpy(&a, live + k, 4);
                        std::memcpy(&b, native + k, 4);
                        double rel = 0.0;
                        if (a != b && floatClose(a, b, rel))
                        {
                            std::memcpy(native + k, live + k, 4);
                            ftolWorst = std::max(ftolWorst, rel);
                            ++ftolWords;
                        }
                    }
                    static uint64_t shown = 0;
                    if ((++shown & 63u) == 1u)
                        std::fprintf(stderr, "[difftest] ftol %s words %llu worst rel %.3g\n", c.function->name, (unsigned long long)ftolWords, ftolWorst);
                    if (std::memcmp(live, native, size) == 0)
                        return;
                }
                size_t i = 0;
                while (live[i] == native[i])
                    ++i;
                char what[128];
                std::snprintf(what, sizeof(what), "%s %08x recompiled %02x native %02x", where, base + static_cast<uint32_t>(i), live[i], native[i]);
                report(c, input, what);
                // SOTC_DIFFTEST_DUMP=1: the next differing bytes too.
                static const bool dump = ps2_diag::env("SOTC_DIFFTEST_DUMP") != nullptr;
                for (size_t j = i + 1, shown = 0; dump && j < size && shown < 16; ++j)
                    if (live[j] != native[j])
                    {
                        std::fprintf(stderr, "[difftest]   also %08x recompiled %02x native %02x\n", base + static_cast<uint32_t>(j), live[j], native[j]);
                        ++shown;
                    }
                bad = true;
            };
            if (!c.function->switches)
            {
                const R5900Context *in = &input;
                const uint32_t sp = GPR_U32(in, 29) & (PS2_RAM_SIZE - 1u);
                // SOTC_DIFFTEST_DEADSTACK=<bytes> (checking, round 15): that much
                // of the dead stack right below sp compared too (the callee's
                // frame and its callees' frames, which a caller may read later
                // as uninitialised locals).
                static const uint32_t deadChecked = ps2_diag::env("SOTC_DIFFTEST_DEADSTACK")
                                                        ? static_cast<uint32_t>(std::strtoul(ps2_diag::env("SOTC_DIFFTEST_DEADSTACK"), nullptr, 0))
                                                        : 0u;
                const uint32_t low = sp > kDeadStack ? sp - kDeadStack : 0u;
                const uint32_t top = sp - std::min(deadChecked, sp - low);
                std::memcpy(nativeAfter.ram.data() + low, rdram + low, top - low); // not compared
            }
            // SOTC_DIFFTEST_IGNORE=start-end,...: RAM ranges left out (bytes
            // that differ between two runs of the same code, e.g. what a
            // sub-thread run by a wake round trip writes from a timer).
            static const std::vector<std::pair<uint32_t, uint32_t>> ignored = []
            {
                std::vector<std::pair<uint32_t, uint32_t>> out;
                if (const char *list = ps2_diag::env("SOTC_DIFFTEST_IGNORE"))
                    for (const char *p = list; *p;)
                    {
                        char *end = nullptr;
                        const uint32_t lo = static_cast<uint32_t>(std::strtoul(p, &end, 16));
                        if (*end != '-')
                            break;
                        const uint32_t hi = static_cast<uint32_t>(std::strtoul(end + 1, &end, 16));
                        if (lo < hi && hi <= PS2_RAM_SIZE)
                            out.emplace_back(lo, hi);
                        p = *end == ',' ? end + 1 : end;
                        if (!*end)
                            break;
                    }
                return out;
            }();
            for (const auto &range : ignored)
                std::memcpy(nativeAfter.ram.data() + range.first, rdram + range.first, range.second - range.first);
            compareMemory(rdram, nativeAfter.ram.data(), PS2_RAM_SIZE, 0u, "ram");
            compareMemory(scratch, nativeAfter.scratch.data(), PS2_SCRATCHPAD_SIZE, 0x70000000u, "scratchpad");
#if !defined(__vita__)
            static const bool watch = ps2_diag::env("SOTC_DIFFTEST_WATCH") != nullptr;
            if (bad && watch && depth == 0 && !c.function->switches && std::memcmp(rdram, nativeAfter.ram.data(), PS2_RAM_SIZE) != 0)
            {
                size_t i = 0;
                while (rdram[i] == nativeAfter.ram[i])
                    ++i;
                // The recompiled leg's result kept aside, the native leg again from the input.
                const std::vector<uint8_t> keepRam(rdram, rdram + PS2_RAM_SIZE), keepScratch(scratch, scratch + PS2_SCRATCHPAD_SIZE);
                const R5900Context keepCtx = *ctx;
                std::memcpy(rdram, before.ram.data(), PS2_RAM_SIZE);
                std::memcpy(scratch, before.scratch.data(), PS2_SCRATCHPAD_SIZE);
                *ctx = input;
                g_watchAddr = static_cast<uint32_t>(i);
                g_watchOn = 1;
                g_watchDepth = 0;
                const bool stats = g_callGuestStats;
                g_callGuestStats = true;
                std::fprintf(stderr, "[watch] %s: %08x recompiled %02x native %02x, before %02x; sp %08x ra %08x\n", c.function->name, g_watchAddr,
                             keepRam[i], nativeAfter.ram[i], before.ram[i], GPR_U32(&input, 29), GPR_U32(&input, 31));
                g_watchTarget = g_watchRa = 0;
                ++g_depth;
                runToReturn(rdram, ctx, runtime, c.function->native);
                --g_depth;
                std::fprintf(stderr, "[watch] rerun ends with %02x\n", rdram[g_watchAddr]);
                static int logged = 0;
                if (g_watchTarget && logged++ < 2)
                {
                    std::memcpy(rdram, before.ram.data(), PS2_RAM_SIZE);
                    std::memcpy(scratch, before.scratch.data(), PS2_SCRATCHPAD_SIZE);
                    *ctx = input;
                    g_watchLogTarget = g_watchTarget;
                    g_watchLogRa = g_watchRa;
                    ++g_depth;
                    runToReturn(rdram, ctx, runtime, c.function->native);
                    --g_depth;
                    g_watchLogTarget = g_watchLogRa = 0;
                    std::fprintf(stderr, "[watch] second rerun (stores inside %08x from %08x) ends with %02x\n", g_watchTarget, g_watchRa, rdram[g_watchAddr]);
                }
                g_callGuestStats = stats;
                g_watchOn = 0;
                std::memcpy(rdram, keepRam.data(), PS2_RAM_SIZE);
                std::memcpy(scratch, keepScratch.data(), PS2_SCRATCHPAD_SIZE);
                *ctx = keepCtx;
            }
#endif
            // SOTC_DIFFTEST_SPOISON: the legs ran on marks; the game goes on
            // from the recompiled code run once more on the real registers.
            if (poison)
            {
                std::memcpy(rdram, before.ram.data(), PS2_RAM_SIZE);
                std::memcpy(scratch, before.scratch.data(), PS2_SCRATCHPAD_SIZE);
                *ctx = input;
                ++g_depth;
                if (c.function->once)
                    c.recompiled(rdram, ctx, runtime);
                else
                    runToReturn(rdram, ctx, runtime, c.recompiled);
                --g_depth;
            }
            // Decomp D6: SOTC_DIFFTEST_REGDUMP=1: at a function's first mismatch, every GPR of the input and of both legs'
            // results (low 64 bits), pc and the depth (which register and where the difference comes from).
            static const bool regDump = ps2_diag::env("SOTC_DIFFTEST_REGDUMP") != nullptr;
            if (bad && regDump && c.mismatches.load(std::memory_order_relaxed) == 0u)
            {
                std::fprintf(stderr, "[difftest] regdump %s: input pc %08x, depth %d, out pc recompiled %08x native %08x, unwinding %d (in %d, pending in %d); reg input / recompiled / native\n",
                             c.function->name, input.pc, depth, ctx->pc, native->pc, runtime->m_eeClock.unwinding ? 1 : 0, unwindingIn ? 1 : 0, pendingIn ? 1 : 0);
                for (int r = 0; r < 32; ++r)
                    std::fprintf(stderr, "[difftest]   r%-2d %016llx %016llx %016llx\n", r, (unsigned long long)GPR_U64(&input, r),
                                 (unsigned long long)GPR_U64(ctx, r), (unsigned long long)GPR_U64(native, r));
#if !defined(__vita__)
                {
                    Dl_info di{}, dn{};
                    dladdr(reinterpret_cast<void *>(c.recompiled), &di);
                    dladdr(reinterpret_cast<void *>(c.function->native), &dn);
                    std::fprintf(stderr, "[difftest]   legs: recompiled %s, native %s\n", di.dli_sname ? di.dli_sname : "?", dn.dli_sname ? dn.dli_sname : "?");
                }
#endif
                const uint32_t a1 = GPR_U32(&input, 5) & (PS2_RAM_SIZE - 1u) & ~3u;
                if (a1 + 0x40u <= PS2_RAM_SIZE)
                {
                    uint32_t w[16];
                    std::memcpy(w, before.ram.data() + a1, sizeof(w));
                    std::fprintf(stderr, "[difftest]   words at a1 (before): %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", w[0], w[1],
                                 w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15]);
                }
                // SOTC_DIFFTEST_PEEK=<hex address>: 16 words there too (before the call).
                if (const char *peek = ps2_diag::env("SOTC_DIFFTEST_PEEK"))
                {
                    const uint32_t pa = static_cast<uint32_t>(std::strtoul(peek, nullptr, 16)) & (PS2_RAM_SIZE - 1u) & ~3u;
                    if (pa + 0x40u <= PS2_RAM_SIZE)
                    {
                        uint32_t w[16];
                        std::memcpy(w, before.ram.data() + pa, sizeof(w));
                        std::fprintf(stderr, "[difftest]   words at %08x: %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n", pa, w[0],
                                     w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15]);
                    }
                }
            }
            if (bad)
                c.mismatches.fetch_add(1, std::memory_order_relaxed);
            const uint64_t checks = c.checks.load(std::memory_order_relaxed);
            if (checks == 1u || checks % 100u == 0u)
                std::fprintf(stderr, "[difftest] %s: %llu checks, %llu mismatches (%llu calls)\n", c.function->name, (unsigned long long)checks,
                             (unsigned long long)c.mismatches.load(), (unsigned long long)(n + 1u));
        }

        template <int Slot>
        void checkedCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            checkedCallImpl(g_checked[Slot], rdram, ctx, runtime);
        }

        template <int... Slots>
        constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeChecked(std::integer_sequence<int, Slots...>)
        {
            return {&checkedCall<Slots>...};
        }
    }

#if !defined(__vita__)
    namespace
    {
        // SOTC_CALLGUEST_STATS=<frames> (Mac, round 42): the natives' guest
        // calls by target and by call site, every that many game frames:
        // calls a frame, and whether the table holds a native there.
        struct CallStats
        {
            std::unordered_map<uint32_t, uint64_t> byTarget, bySite;
            uint64_t calls = 0, from = 0;
        };
        uint64_t callStatsPeriod()
        {
            static const uint64_t p = ps2_diag::env("SOTC_CALLGUEST_STATS") ? std::strtoull(ps2_diag::env("SOTC_CALLGUEST_STATS"), nullptr, 0) : 0u;
            return p;
        }
        bool isNative(PS2Runtime::RecompiledFunction fn)
        {
            for (const Function &f : registry())
                if (f.native == fn)
                    return true;
            return false;
        }
        void noteCall(PS2Runtime *runtime, uint32_t target, uint32_t ra)
        {
            static CallStats s;
            const uint64_t period = callStatsPeriod();
            if (!s.calls)
                s.from = g_gameFrame;
            ++s.calls;
            ++s.byTarget[target];
            ++s.bySite[ra - 8u];
            if (g_gameFrame - s.from < period)
                return;
            const double frames = static_cast<double>(g_gameFrame - s.from);
            std::vector<std::pair<uint64_t, uint32_t>> t, c;
            for (const auto &[a, n] : s.byTarget)
                t.emplace_back(n, a);
            for (const auto &[a, n] : s.bySite)
                c.emplace_back(n, a);
            std::sort(t.rbegin(), t.rend());
            std::sort(c.rbegin(), c.rend());
            uint64_t nativeCalls = 0;
            for (const auto &[n, a] : t)
                if (isNative(runtime->guestFunctionAt(a)))
                    nativeCalls += n;
            std::fprintf(stderr, "[callguest] frames %llu-%llu: %.1f calls a frame, %.1f to natives\n", (unsigned long long)s.from,
                         (unsigned long long)g_gameFrame, s.calls / frames, nativeCalls / frames);
            for (size_t i = 0; i < t.size() && i < 60; ++i)
                std::fprintf(stderr, "[callguest] target %08x %8.1f %s\n", t[i].second, t[i].first / frames,
                             isNative(runtime->guestFunctionAt(t[i].second)) ? "native" : "");
            for (size_t i = 0; i < c.size() && i < 60; ++i)
                std::fprintf(stderr, "[callguest] site %08x %8.1f\n", c[i].second, c[i].first / frames);
            s = CallStats{};
        }
    }
#endif

    bool callMaySleepRest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        // finishGuestCall's loop, stopping when the sub-thread slept.
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            if (onWakerContext(rdram, ctx))
                return false;
            PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
            if (!next)
                return false;
            next(rdram, ctx, runtime);
            runtime->checkTransfer();
        }
        return true;
    }

    void finishGuestCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        // runToReturn's loop, after the callee's first return.
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
            if (!next)
                break;
            next(rdram, ctx, runtime);
            runtime->checkTransfer();
        }
    }

    void callGuestSlow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
#if !defined(__vita__)
        if (g_callGuestStats && callStatsPeriod() != 0u)
            noteCall(runtime, target, ra);
        if (g_watchOn)
        {
            const uint8_t was = rdram[g_watchAddr];
            const R5900Context in = *ctx;
            ++g_watchDepth;
            const bool logStores = g_watchLogTarget == target && g_watchLogRa == ra;
            const bool watchWas = g_ps2WatchWriteEnabled;
            if (logStores)
            {
                std::fprintf(stderr, "[watch] stores inside %08x:\n", target);
                g_ps2WatchWriteEnabled = g_ps2LogAllWrites = true;
            }
            PS2Runtime::RecompiledFunction fn = runtime->guestFunctionAt(target);
            runToReturn(rdram, ctx, runtime, fn ? fn : runtime->lookupFunction(target));
            if (logStores)
            {
                g_ps2LogAllWrites = false;
                g_ps2WatchWriteEnabled = watchWas;
            }
            --g_watchDepth;
            if (rdram[g_watchAddr] != was && !g_watchTarget)
            {
                g_watchTarget = target;
                g_watchRa = ra;
            }
            if (rdram[g_watchAddr] != was)
            {
                const R5900Context *r = &in;
                std::fprintf(stderr,
                             "[watch] depth %d call %08x ra %08x sp %08x: %02x -> %02x; a0 %08x a1 %08x a2 %08x a3 %08x t0 %08x t1 %08x v0 %08x v1 %08x"
                             " s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x s6 %08x s7 %08x fp %08x\n",
                             g_watchDepth, target, ra, sp, was, rdram[g_watchAddr], GPR_U32(r, 4), GPR_U32(r, 5), GPR_U32(r, 6), GPR_U32(r, 7),
                             GPR_U32(r, 8), GPR_U32(r, 9), GPR_U32(r, 2), GPR_U32(r, 3), GPR_U32(r, 16), GPR_U32(r, 17), GPR_U32(r, 18),
                             GPR_U32(r, 19), GPR_U32(r, 20), GPR_U32(r, 21), GPR_U32(r, 22), GPR_U32(r, 23), GPR_U32(r, 30));
            }
            return;
        }
#endif
        PS2Runtime::RecompiledFunction fn = runtime->guestFunctionAt(target);
        runToReturn(rdram, ctx, runtime, fn ? fn : runtime->lookupFunction(target));
    }

    void callRecompiled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = address;
        runToReturn(rdram, ctx, runtime, recompiled(address));
    }

    PS2Runtime::RecompiledFunction recompiled(uint32_t address)
    {
        if (g_recompiledMask != 0u && address != 0u)
        {
            for (uint32_t i = recompiledHash(address) & g_recompiledMask;; i = (i + 1u) & g_recompiledMask)
            {
                const RecompiledSlot &slot = g_recompiledTable[i];
                if (slot.address == address)
                    return slot.fn;
                if (slot.address == 0u)
                    return nullptr;
            }
        }
        const auto it = g_recompiled.find(address);
        return it == g_recompiled.end() ? nullptr : it->second;
    }

    namespace
    {
        struct AbEntry
        {
            uint32_t address;
            PS2Runtime::RecompiledFunction native, recompiled;
        };
        std::vector<AbEntry> g_ab;
        uint64_t g_abPeriod = 0;
        bool g_abNew = true, g_abSpr = true, g_abPrefetch = true, g_abFastPaths = false;
        PS2Runtime *g_abRuntime = nullptr;
    }

    bool abNew() { return g_abNew; }

    // Decomp D1's lifted functions (tools/lift_d1.py): their table entries swapped when SOTC_LIFTD1 changes at run
    // time (an A/B window, SOTC_AB_FLAG=liftd1), as abTick swaps SOTC_AB's set.
    // Decomp D2: the same for D2's batch (SOTC_LIFTD2, SOTC_AB_FLAG=liftd2), its own list; D3 likewise (SOTC_LIFTD3,
    // liftd3), D4 likewise (SOTC_LIFTD4, liftd4), D5 likewise (SOTC_LIFTD5, liftd5), D6 likewise (SOTC_LIFTD6, liftd6).
    // Decomp D4 (test): liftalt off puts each lifted function's alternative copy (SOTC_DECOMP_LIFT_ALT) where the lifted
    // function is, on puts the lifted function back: two lifter versions of the same functions A/B'd in one run.
    namespace
    {
        std::vector<AbEntry> g_lift, g_lift2, g_lift3, g_lift4, g_lift5, g_lift6;
        int g_liftHooked = 0;
        PS2Runtime *g_liftRuntime = nullptr;
        void liftAltChanged(bool on)
        {
            int swapped = 0, remapped = 0;
            for (const auto &[address, alt] : liftAlts())
                for (const std::vector<AbEntry> *v : {&g_lift, &g_lift2, &g_lift3, &g_lift4, &g_lift5, &g_lift6})
                    for (const AbEntry &e : *v)
                    {
                        if (e.address != address || !e.native)
                            continue;
                        const PS2Runtime::RecompiledFunction from = on ? alt : e.native, to = on ? e.native : alt;
                        if (g_liftRuntime->lookupFunction(address) == from)
                        {
                            g_liftRuntime->registerFunction(address, to);
                            ++swapped;
                        }
                        else
                        {
                            sotc_prof::remapWrapped(from, to);
                            ++remapped;
                        }
                    }
            std::fprintf(stderr, "[liftalt] frame %llu: %s (%d swapped, %d of %zu elsewhere)\n", static_cast<unsigned long long>(g_gameFrame),
                         on ? "on (the lifted functions)" : "off (their alternative copies)", swapped, remapped, liftAlts().size());
        }
        void liftSwitchChanged(int which, bool on)
        {
            if (which == kLiftAlt && g_liftRuntime)
                return liftAltChanged(on);
            applyLiftFallbacks(which);
            if ((which != kLiftD1 && which != kLiftD2 && which != kLiftD3 && which != kLiftD4 && which != kLiftD5 && which != kLiftD6) || !g_liftRuntime)
                return;
            int swapped = 0;
            for (const AbEntry &e : which == kLiftD1 ? g_lift : which == kLiftD2 ? g_lift2 : which == kLiftD3 ? g_lift3 : which == kLiftD4 ? g_lift4 : which == kLiftD5 ? g_lift5 : g_lift6)
            {
                if (!e.recompiled || !e.native)
                    continue;
                const PS2Runtime::RecompiledFunction from = on ? e.recompiled : e.native, to = on ? e.native : e.recompiled;
                if (g_liftRuntime->lookupFunction(e.address) == from)
                {
                    g_liftRuntime->registerFunction(e.address, to);
                    ++swapped;
                }
                else
                    sotc_prof::remapWrapped(from, to);
            }
            std::fprintf(stderr, "[%s] frame %llu: %s (%d swapped)\n", which == kLiftD1 ? "liftd1" : which == kLiftD2 ? "liftd2" : which == kLiftD3 ? "liftd3" : which == kLiftD4 ? "liftd4" : which == kLiftD5 ? "liftd5" : "liftd6",
                         static_cast<unsigned long long>(g_gameFrame), on ? "on" : "off", swapped);
        }
    }

    void abTick(uint64_t frame)
    {
        if (!g_abPeriod)
            return;
        const bool on = (frame / g_abPeriod) % 2u == 0u;
        if (on == g_abNew)
            return;
        g_abNew = on;
        for (const AbEntry &e : g_ab)
        {
            if (!e.recompiled)
                continue;
            const PS2Runtime::RecompiledFunction from = on ? e.recompiled : e.native, to = on ? e.native : e.recompiled;
            // In the table, or behind a SOTC_SUBSYS_PROF wrapper.
            if (g_abRuntime->lookupFunction(e.address) == from)
                g_abRuntime->registerFunction(e.address, to);
            else
                sotc_prof::remapWrapped(from, to);
        }
        // The scratchpad and DMA fast paths (2026-10-22's round) stay on:
        // SOTC_AB_FASTPATHS=1 alternates them too.
        if (g_abFastPaths)
        {
            ps2_fast_paths::g_sprFast = on && g_abSpr;
            ps2_fast_paths::g_dmaPrefetch = on && g_abPrefetch;
        }
        sotc_prof::noteAb(on);
        std::fprintf(stderr, "[ab] frame %llu: %s\n", static_cast<unsigned long long>(frame), on ? "new" : "old");
    }

    namespace
    {
        const std::array<PS2Runtime::RecompiledFunction, kMaxChecked> &checkers()
        {
            static const auto table = makeChecked(std::make_integer_sequence<int, kMaxChecked>{});
            return table;
        }
    }

    PS2Runtime::RecompiledFunction behindHook(PS2Runtime &runtime, uint32_t address)
    {
        const PS2Runtime::RecompiledFunction original = runtime.lookupFunction(address);
        if (const char *on = std::getenv("SOTC_DECOMP"); on && std::atoi(on) == 0)
            return original;
        const Function *f = nullptr;
        for (const Function &g : registry())
            if (g.address == address)
                f = &g;
        if (!f || !runtime.hasFunction(address))
            return original;
        g_recompiled.emplace(address, original);
        g_behindHook.push_back(address);
        if (listed(std::getenv("SOTC_DECOMP_SKIP"), f->name))
            return original;
        const char *test = ps2_diag::env("SOTC_DIFFTEST");
        if (listed(test, f->name) && g_checkedUsed < kMaxChecked)
        {
            if (const char *max = ps2_diag::env("SOTC_DIFFTEST_MAX"))
                g_maxChecks = std::strtoull(max, nullptr, 0);
            else if (test && std::strcmp(test, "all") == 0)
                g_maxChecks = 1000u;
            g_checked[g_checkedUsed].function = f;
            g_checked[g_checkedUsed].recompiled = original;
            return checkers()[g_checkedUsed++];
        }
        return f->native;
    }

#if !defined(__vita__)
    namespace
    {
        // SOTC_DIFFTEST_PROBE=<target>@<donor> (Mac, round 47): a function the
        // route never reaches checked on a sibling's inputs. At each call of the
        // donor (a function the route reaches, with the same argument
        // registers) the target runs first from the donor's input, through its
        // table entry (the difftest's checker when SOTC_DIFFTEST names it: both
        // legs compared), then RAM, scratchpad and the context are put back as
        // they were and the donor runs as usual: the game goes on unchanged.
        uint32_t g_probeTarget = 0, g_probeDonor = 0;
        PS2Runtime::RecompiledFunction g_probeDonorFn = nullptr;
        void probeDonor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            static std::vector<uint8_t> ram, spr;
            uint8_t *scratch = runtime->memory().getScratchpad();
            ram.assign(rdram, rdram + PS2_RAM_SIZE);
            spr.assign(scratch, scratch + PS2_SCRATCHPAD_SIZE);
            const R5900Context input = *ctx;
            ctx->pc = g_probeTarget;
            runToReturn(rdram, ctx, runtime, runtime->guestFunctionAt(g_probeTarget));
            std::memcpy(rdram, ram.data(), PS2_RAM_SIZE);
            std::memcpy(scratch, spr.data(), PS2_SCRATCHPAD_SIZE);
            *ctx = input;
            g_probeDonorFn(rdram, ctx, runtime);
        }
        void installProbe(PS2Runtime &runtime)
        {
            const char *probe = ps2_diag::env("SOTC_DIFFTEST_PROBE");
            if (!probe || !std::strchr(probe, '@'))
                return;
            const std::string target(probe, std::strchr(probe, '@')), donor(std::strchr(probe, '@') + 1);
            for (const Function &f : registry())
            {
                if (target == f.name)
                    g_probeTarget = f.address;
                if (donor == f.name)
                    g_probeDonor = f.address;
            }
            if (!g_probeTarget || !g_probeDonor)
            {
                std::cerr << "[difftest] SOTC_DIFFTEST_PROBE: " << probe << ": not two registered natives" << std::endl;
                return;
            }
            g_probeDonorFn = runtime.lookupFunction(g_probeDonor);
            runtime.registerFunction(g_probeDonor, probeDonor);
            std::cerr << "[difftest] probe: " << target << " on " << donor << "'s inputs" << std::endl;
        }
    }
#endif

    void install(PS2Runtime &runtime)
    {
        if (const char *on = std::getenv("SOTC_DECOMP"); on && std::atoi(on) == 0)
            return;
#if !defined(__vita__)
        g_callGuestStats = callStatsPeriod() != 0u;
#endif
        // Round 106's full checks: SOTC_GRP106_CHECK=2 / SOTC_OAN106_CHECK=2 /
        // SOTC_AI106_CHECK=2 put that switch's natives in the difftest's list
        // (the native leg against the recompiled code on the same input:
        // registers and every RAM byte compared; SOTC_DIFFTEST_FIRST / _EVERY
        // set how many calls: each check copies 32 MB twice, ~0.7 s in
        // Vita3K). =1 is each switch's own per-call check (its file).
        {
            static const struct
            {
                const char *env, *names;
            } kChecks106[] = {
                {"SOTC_GRP106_CHECK", "gamecore_sub_013448C0"},
                {"SOTC_OAN106_CHECK", "manager_sub_010AF2A0,manager_sub_010AF4D8,manager_sub_010B06A8"},
                {"SOTC_AI106_CHECK", "gamecore_sub_013E1520"},
            };
            std::string names = ps2_diag::env("SOTC_DIFFTEST") ? ps2_diag::env("SOTC_DIFFTEST") : "";
            bool any = false;
            for (const auto &c : kChecks106)
                if (const char *e = std::getenv(c.env); e && e[0] == '2')
                {
                    names += (names.empty() ? "" : ",") + std::string(c.names);
                    any = true;
                }
            if (any)
            {
                setenv("SOTC_DIFFTEST", names.c_str(), 1);
                std::fprintf(stderr, "[r106] difftest list: %s\n", names.c_str());
            }
        }
        const auto &checkers = sotc_decomp::checkers();
        const char *skip = std::getenv("SOTC_DECOMP_SKIP");
        const char *test = ps2_diag::env("SOTC_DIFFTEST");
        if (const char *max = ps2_diag::env("SOTC_DIFFTEST_MAX"))
            g_maxChecks = std::strtoull(max, nullptr, 0);
        else if (test && std::strcmp(test, "all") == 0)
            g_maxChecks = 1000u;
        int replaced = 0, checked = g_checkedUsed, unchecked = 0;
        for (const Function &f : registry())
        {
            if (!runtime.hasFunction(f.address))
                continue;
            // A hook calls it (behindHook): its table entry stays the hook's.
            if (std::find(g_behindHook.begin(), g_behindHook.end(), f.address) != g_behindHook.end())
            {
                ++replaced;
                continue;
            }
            // Kept for a skipped one too: other natives reach the recompiled
            // code through recompiled() (a skipped boundary crashed them).
            g_recompiled.emplace(f.address, runtime.lookupFunction(f.address));
            if (listed(skip, f.name))
                continue;
            // Round 48: a coroutine's entries are never checked by themselves
            // (isysWakeupSubThread's check with SOTC_DIFFTEST_SWITCH covers them).
            if (f.coroutine)
            {
                if (listed(test, f.name) && !f.resume && std::strcmp(test, "all") != 0)
                    std::cerr << "[difftest] " << f.name << ": a coroutine or unchecked native, not checked by itself (coroutines: isysWakeupSubThread with SOTC_DIFFTEST_SWITCH)" << std::endl;
                runtime.registerFunction(f.address, f.native);
                if (!f.resume)
                    ++replaced;
                continue;
            }
            if (listed(test, f.name) && checked >= kMaxChecked)
                ++unchecked;
            if (listed(test, f.name) && checked < kMaxChecked)
            {
                g_checked[checked].function = &f;
                // Decomp D6: a lifted function is checked against the generated function it was lifted from
                // (f.original). The slot's holder was used before: for a lifted copy whose slot a native holds (D5's
                // fallback copies, `<name>_fb`, put in the table by SOTC_DIFFTEST) that was the native, so D5's
                // "register mismatches" of the GetGeometryPointer* trio, getLayoutClassCollisionIndepSurfBoundary_fb
                // and manager_sub_01117340_fb compared the lifted copy with geo126's / decomp_collision's ports.
                g_checked[checked].recompiled = f.lift && f.original ? f.original : runtime.lookupFunction(f.address);
                runtime.registerFunction(f.address, checkers[checked]);
                ++checked;
            }
            else
            {
                if (f.lift && runtime.lookupFunction(f.address) != f.original)
                {
                    ++g_liftHooked; // a hook or a runtime override holds the slot: it stays
                    continue;
                }
                if (f.lift)
                {
                    // Decomp D1: in the table while SOTC_LIFTD1 is on (read here, after the env file); kept for the
                    // A/B's swaps either way.
                    const int sw = f.liftSwitch < 0 ? kLiftD1 : f.liftSwitch;
                    // Decomp D2 (test knob): SOTC_LIFTD2_RANGE=<lo>:<hi> (hex guest addresses) installs only D2's
                    // functions in [lo, hi), for bisecting on the Vita (the env file's lines are 4 KB at most).
                    static const std::pair<uint32_t, uint32_t> d2Range = []
                    {
                        const char *r = ps2_diag::env("SOTC_LIFTD2_RANGE");
                        if (!r || !std::strchr(r, ':'))
                            return std::pair<uint32_t, uint32_t>{0u, 0xFFFFFFFFu};
                        return std::pair<uint32_t, uint32_t>{static_cast<uint32_t>(std::strtoul(r, nullptr, 16)),
                                                             static_cast<uint32_t>(std::strtoul(std::strchr(r, ':') + 1, nullptr, 16))};
                    }();
                    if (sw == kLiftD2 && (f.address < d2Range.first || f.address >= d2Range.second))
                        continue;
                    // Decomp D3: SOTC_LIFTD3_RANGE, the same for D3's functions.
                    static const std::pair<uint32_t, uint32_t> d3Range = []
                    {
                        const char *r = ps2_diag::env("SOTC_LIFTD3_RANGE");
                        if (!r || !std::strchr(r, ':'))
                            return std::pair<uint32_t, uint32_t>{0u, 0xFFFFFFFFu};
                        return std::pair<uint32_t, uint32_t>{static_cast<uint32_t>(std::strtoul(r, nullptr, 16)),
                                                             static_cast<uint32_t>(std::strtoul(std::strchr(r, ':') + 1, nullptr, 16))};
                    }();
                    if (sw == kLiftD3 && (f.address < d3Range.first || f.address >= d3Range.second))
                        continue;
                    // Decomp D4: SOTC_LIFTD4_RANGE, the same for D4's functions.
                    static const std::pair<uint32_t, uint32_t> d4Range = []
                    {
                        const char *r = ps2_diag::env("SOTC_LIFTD4_RANGE");
                        if (!r || !std::strchr(r, ':'))
                            return std::pair<uint32_t, uint32_t>{0u, 0xFFFFFFFFu};
                        return std::pair<uint32_t, uint32_t>{static_cast<uint32_t>(std::strtoul(r, nullptr, 16)),
                                                             static_cast<uint32_t>(std::strtoul(std::strchr(r, ':') + 1, nullptr, 16))};
                    }();
                    if (sw == kLiftD4 && (f.address < d4Range.first || f.address >= d4Range.second))
                        continue;
                    // Decomp D5: SOTC_LIFTD5_RANGE, the same for D5's functions.
                    static const std::pair<uint32_t, uint32_t> d5Range = []
                    {
                        const char *r = ps2_diag::env("SOTC_LIFTD5_RANGE");
                        if (!r || !std::strchr(r, ':'))
                            return std::pair<uint32_t, uint32_t>{0u, 0xFFFFFFFFu};
                        return std::pair<uint32_t, uint32_t>{static_cast<uint32_t>(std::strtoul(r, nullptr, 16)),
                                                             static_cast<uint32_t>(std::strtoul(std::strchr(r, ':') + 1, nullptr, 16))};
                    }();
                    if (sw == kLiftD5 && (f.address < d5Range.first || f.address >= d5Range.second))
                        continue;
                    // Decomp D6: SOTC_LIFTD6_RANGE, the same for D6's functions.
                    static const std::pair<uint32_t, uint32_t> d6Range = []
                    {
                        const char *r = ps2_diag::env("SOTC_LIFTD6_RANGE");
                        if (!r || !std::strchr(r, ':'))
                            return std::pair<uint32_t, uint32_t>{0u, 0xFFFFFFFFu};
                        return std::pair<uint32_t, uint32_t>{static_cast<uint32_t>(std::strtoul(r, nullptr, 16)),
                                                             static_cast<uint32_t>(std::strtoul(std::strchr(r, ':') + 1, nullptr, 16))};
                    }();
                    if (sw == kLiftD6 && (f.address < d6Range.first || f.address >= d6Range.second))
                        continue;
                    (sw == kLiftD1 ? g_lift : sw == kLiftD2 ? g_lift2 : sw == kLiftD3 ? g_lift3 : sw == kLiftD4 ? g_lift4 : sw == kLiftD5 ? g_lift5 : g_lift6).push_back({f.address, f.native, runtime.lookupFunction(f.address)});
                    if (!switchOn(sw))
                        continue;
                }
                runtime.registerFunction(f.address, f.native);
                if (f.ab)
                    g_ab.push_back({f.address, f.native, runtime.lookupFunction(f.address) == f.native ? g_recompiled[f.address] : nullptr});
            }
            ++replaced;
        }
        if (const char *ab = ps2_diag::env("SOTC_AB"))
        {
            g_abPeriod = std::max<uint64_t>(2u, std::strtoull(ab, nullptr, 10));
            g_abRuntime = &runtime;
            g_abSpr = ps2_fast_paths::g_sprFast;
            g_abPrefetch = ps2_fast_paths::g_dmaPrefetch;
            g_abFastPaths = ps2_diag::env("SOTC_AB_FASTPATHS") && std::atoi(ps2_diag::env("SOTC_AB_FASTPATHS")) != 0;
            sotc_prof::noteAb(true);
            std::cerr << "[ab] SOTC_AB: " << g_ab.size() << " functions" << (g_abFastPaths ? " and the runtime's fast paths" : "") << " alternate every " << g_abPeriod << " frames" << std::endl;
        }
        // Round 43: SignalSema's uncontended case (decomp_sema.cpp; not a
        // registry native: a difftest leg would signal twice).
        if (!listed(skip, "SignalSema"))
            installSemaFast(runtime);
        g_checkedUsed = checked;
        if (unchecked)
            std::cerr << "[difftest] " << unchecked << " functions past the " << kMaxChecked << " slots left unchecked" << std::endl;
        if (!g_lift.empty() || !g_lift2.empty() || !g_lift3.empty() || !g_lift4.empty() || !g_lift5.empty() || !g_lift6.empty())
        {
            g_liftRuntime = &runtime;
            g_switchChanged = liftSwitchChanged;
            std::cerr << "[liftd1] " << g_lift.size() << " lifted functions, D2 " << g_lift2.size() << ", D3 " << g_lift3.size() << ", D4 " << g_lift4.size() << ", D5 " << g_lift5.size() << ", D6 " << g_lift6.size()
                      << " (" << g_liftHooked << " left to a hook), d1 "
                      << (switchOn(kLiftD1) ? "on" : "off (SOTC_LIFTD1=0)") << ", d2 " << (switchOn(kLiftD2) ? "on" : "off (SOTC_LIFTD2=0)")
                      << ", d3 " << (switchOn(kLiftD3) ? "on" : "off (SOTC_LIFTD3=0)") << ", d4 " << (switchOn(kLiftD4) ? "on" : "off (SOTC_LIFTD4=0)") << ", d5 " << (switchOn(kLiftD5) ? "on" : "off (SOTC_LIFTD5=0)") << ", d6 " << (switchOn(kLiftD6) ? "on" : "off (SOTC_LIFTD6=0)")
                      << (liftAlts().empty() ? "" : ", alternative copies ") << (liftAlts().empty() ? "" : std::to_string(liftAlts().size())) << std::endl;
            // Decomp D4 (test): SOTC_LIFTALT=0 starts with the alternative copies in place.
            if (!liftAlts().empty() && !switchOn(kLiftAlt))
                liftAltChanged(false);
        }
        // Decomp D5: lifted functions whose slot a native or a hook holds: their copies serve recompiled()
        for (const Function &f : registry())
        {
            if (!f.lift || !f.original || !runtime.hasFunction(f.address) || runtime.lookupFunction(f.address) == f.native)
                continue;
            const auto it = g_recompiled.find(f.address);
            if (it == g_recompiled.end() || it->second != f.original || listed(skip, f.name))
                continue;
            PS2Runtime::RecompiledFunction lifted = f.native;
            if (listed(test, f.name) && checked < kMaxChecked)
            {
                // SOTC_DIFFTEST=<name>_fb: the natives' fallback calls go through a checker (the copy against the
                // generated function, on the inputs the natives give it)
                g_checked[checked].function = &f;
                g_checked[checked].recompiled = f.original;
                lifted = checkers[checked];
                ++checked;
            }
            g_liftFallback.push_back({f.address, lifted, f.original, f.liftSwitch < 0 ? kLiftD1 : f.liftSwitch});
        }
        g_checkedUsed = checked;
        if (!g_liftFallback.empty())
        {
            g_liftRuntime = &runtime;
            g_switchChanged = liftSwitchChanged;
            std::cerr << "[liftd5] " << g_liftFallback.size() << " lifted copies serve recompiled() (natives' fallbacks)" << std::endl;
        }
        buildRecompiledTable();
#if !defined(__vita__)
        installProbe(runtime);
#endif
        std::cerr << "[sotc:decomp] " << replaced << " decompiled functions in place" << (checked ? ", " : "")
                  << (checked ? std::to_string(checked) + " checked against the recompiled code" : std::string()) << std::endl;
    }
}
