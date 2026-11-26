// Round W3 (SOTC_W3=1, off by default; docs/decomp_w3.md): the script objects' pass (manager_sub_01102638) as a
// native walk.
//
// What the pass's console "self" is (P3's brackets: 1.98 M cycles a frame, 16 passes, ~130 objects): a sub-thread
// woken by isysWakeupSubThread resumes at the label after its last sleep (the frame end, scrRunM ...). Those
// resume entries return to the script's own labels, which have no host frame below the wake: the wake's call
// comes back with pc on the script and the wake returns (its bracket closes). The pass's callGuest then runs the
// script body in its run-to-return loop (sotc_decomp::finishGuestCall) until the body sleeps again and the switch
// lands on the pass's return address. So the bodies of almost every script (the AI, the frame end's per-type work:
// ExecObjectAnimation, the characters' geometry, the view clip ...) run in the pass's self, not in the wake's.
//
// The W3 pass, the same calls in the same order with the same registers and stores (game logic: script order and
// timing are the game's):
// - that run-to-return loop in a bracket of its own, "scripts:subThreadRun" (SOTC_P2BRACKET >= 2), so the
//   console splits the walk from the script bodies (SOTC_P2BRACKET=4: one bracket per object type,
//   "scripts:run_tNN");
// - GetScriptClass's plain cases inline (an object's class at +0x20 / +0x1c or none, by its type's jump table
//   entry, read live), the registers and the frame word GetScriptClass leaves; its call-out cases (the window /
//   sprite / layout / character class getters) as before;
// - isysGroupForNext called directly (decomp_group::nextAt, the native the table holds) with the registers its
//   native leaves; anything else in the slot (a difftest checker, a profiler wrapper, SOTC_DECOMP_SKIP): the table;
// - the next object's link, object and script-class lines asked for (PLD) at the start of each object: the body
//   of the current one runs meanwhile (on the console each object is a cold line; nothing is read for it).
// isysGroupForFirst / ForExit (the group lock: semaphore calls), the wake, the sub-thread start / init, the
// letters' discard, the +0x4c callback and the delete request stay calls through the table.
//
// SOTC_W3_CHECK=1: every inline GetScriptClass also runs the native on a copy of the context and compares v0 / v1 /
// sp / ra / pc ("[w3c]" lines). SOTC_W3_LOG=<n>: a "[w3]" line every n frames (passes, objects, wakes, how many
// went through each path).
#include "sotc_decomp.h"
#include "decomp_group.h"
#include "sotc_p2.h"
#include "sotc_pf141.h"
#include "runtime/ps2_diag.h"
#include "runtime/ps2_thread_load.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace obj96 // decomp_obj96.cpp (round 96)
{
    bool skipWake(uint8_t *rdram, uint32_t obj, uint32_t scriptClass);
    bool timing();
    uint64_t now();
    void wakeBegin();
    void wakeTime(uint32_t obj, uint64_t us);
}

namespace char104 // round 104 (decomp_char104.cpp)
{
    void wake();
    void beforeWake();
    void groupEnd();
}

namespace decomp_scriptvm3
{
    void getScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_group
{
    void groupForNextSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace w3
{
    using sotc_decomp::jalTarget;

    // The pass's other guest calls (the walk's rare ones and the lock / letters / callback / delete calls): one
    // out-of-line copy of sotc_decomp::callGuest instead of one inline copy a site (code bytes are the console's cost).
    __attribute__((noinline)) void callGuest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
    }

    namespace
    {
        constexpr uint32_t kGetScriptClass = 0x01102438u, kForNext = 0x001C76D0u, kLinks = 0x0021B900u;

        int g_on = -1, g_check = 0;
        uint64_t g_logEvery = 0;
        void init()
        {
            const char *e = ps2_diag::env("SOTC_W3");
            g_on = e && std::atoi(e) != 0;
            const char *c = ps2_diag::env("SOTC_W3_CHECK");
            g_check = c && std::atoi(c) != 0;
            const char *l = ps2_diag::env("SOTC_W3_LOG");
            g_logEvery = l ? std::strtoull(l, nullptr, 10) : 0u;
            if (g_on)
                std::fprintf(stderr, "[w3] on: the script pass as a native walk (check %d, log every %llu)\n", g_check,
                             static_cast<unsigned long long>(g_logEvery));
        }

        inline uint32_t ru32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        inline uint16_t ru16(const uint8_t *rdram, uint32_t a)
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

        // Counters (SOTC_W3_LOG).
        struct Stats
        {
            uint64_t passes, objects, wakes, runs, classInline, classGuest, nextDirect, nextGuest, checks, bad;
        } g_st{};
        uint64_t g_lastLog = 0;

        // The brackets (SOTC_P2BRACKET >= 2: one for the run-to-return loop; = 4: one per object type).
        int g_runId = -2;
        int g_typeId[256];
        int g_level = 0;
        __attribute__((cold, noinline)) void bracketInit()
        {
            g_runId = sotc_p2::named("scripts:subThreadRun", 2);
            const char *b = ps2_diag::env("SOTC_P2BRACKET");
            g_level = b ? std::atoi(b) : 0;
            for (int &t : g_typeId)
                t = -2;
        }
        __attribute__((cold, noinline)) int typeBracket(uint8_t type)
        {
            if (g_typeId[type] == -2)
            {
                static char names[256][24];
                std::snprintf(names[type], sizeof(names[type]), "scripts:run_t%02x", type);
                g_typeId[type] = sotc_p2::named(names[type], 4);
            }
            return g_typeId[type];
        }

        // Round X2: the run-to-return loop split by the guest function it enters (keyed by the table's function: a script
        // body's labels share one; up to 63, the rest under index 63).
        // - SOTC_X2_RANK=1 with SOTC_P2BRACKET >= 2: one bracket per function ("scripts:fn_<first pc>"), for Vita3K's clock
        //   brackets / the console's counters (bit 0 of g_rank);
        // - SOTC_X2_ZONE=1 with SOTC_PROF129=1: prof129's samples carry zone 0x4000 | function index << 6 | (type & 0x3f)
        //   while that function runs (the level-1 brackets inside set 0x5000 | slot, sotc_p2.cpp), the wake 0x6100, the
        //   rest of the pass 0x6200: the console's samples of execScript's self by function and object type (bit 1). The
        //   index -> pc map goes to stderr ("[x2z] fn <index> <pc>").
        int g_rank = -1, g_zone = 0;
        // A prof129 zone while g_zone is on (nothing touched otherwise: no thread-local read).
        struct ZoneIf
        {
            bool on;
            uint32_t prev = 0;
            ZoneIf(uint32_t zone) : on(g_zone != 0)
            {
                if (__builtin_expect(on, 0))
                {
                    prev = ps2_load::gameZone.load(std::memory_order_relaxed);
                    ps2_load::gameZone.store(zone, std::memory_order_relaxed);
                }
            }
            ~ZoneIf()
            {
                if (__builtin_expect(on, 0))
                    ps2_load::gameZone.store(prev, std::memory_order_relaxed);
            }
        };
        __attribute__((cold, noinline)) void rankedFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp, uint8_t type)
        {
            constexpr int kFns = 63;
            static PS2Runtime::RecompiledFunction keys[kFns];
            static int ids[kFns];
            static char names[kFns][24];
            static int used = 0;
            while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
            {
                PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                if (!next)
                    break;
                int id = -1, i = 0;
                for (; i < used; ++i)
                    if (keys[i] == next)
                        break;
                if (i < used)
                    id = ids[i];
                else if (used < kFns)
                {
                    std::snprintf(names[used], sizeof(names[used]), "scripts:fn_%08x", ctx->pc);
                    keys[used] = next;
                    ids[used] = id = (g_rank & 1) ? sotc_p2::named(names[used], 2) : -1;
                    if (g_zone)
                        std::fprintf(stderr, "[x2z] fn %d %08x\n", used, ctx->pc);
                    ++used;
                }
                {
                    const ZoneIf z(0x4000u | static_cast<uint32_t>(i) << 6 | (type & 0x3Fu));
                    const sotc_p2::Scope s(id);
                    next(rdram, ctx, runtime);
                }
                runtime->checkTransfer();
            }
        }

        __attribute__((cold, noinline)) void rankInit()
        {
            const char *r = ps2_diag::env("SOTC_X2_RANK");
            const char *z = ps2_diag::env("SOTC_X2_ZONE");
            g_zone = z && std::atoi(z) != 0 && ps2_load::zonesWanted();
            if (g_runId == -2)
                bracketInit();
            g_rank = ((r && std::atoi(r) != 0 && g_level >= 2) ? 1 : 0) | (g_zone ? 2 : 0);
            if (g_rank)
                std::fprintf(stderr, "[x2z] rank %d (brackets %d, zones %d)\n", g_rank, g_rank & 1, g_zone);
        }

        __attribute__((cold, noinline)) void logLine()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_lastLog + g_logEvery)
                return;
            const double n = g_lastLog ? static_cast<double>(frame - g_lastLog) : 1.0;
            std::fprintf(stderr,
                         "[w3] frames %llu-%llu: passes/f %.1f objects/f %.1f wakes/f %.1f runs/f %.1f | class inline %llu guest %llu | next direct "
                         "%llu guest %llu | checks %llu bad %llu\n",
                         static_cast<unsigned long long>(g_lastLog), static_cast<unsigned long long>(frame), g_st.passes / n, g_st.objects / n,
                         g_st.wakes / n, g_st.runs / n, static_cast<unsigned long long>(g_st.classInline), static_cast<unsigned long long>(g_st.classGuest),
                         static_cast<unsigned long long>(g_st.nextDirect), static_cast<unsigned long long>(g_st.nextGuest),
                         static_cast<unsigned long long>(g_st.checks), static_cast<unsigned long long>(g_st.bad));
            const uint64_t checks = g_st.checks, bad = g_st.bad;
            g_st = Stats{};
            g_st.checks = checks;
            g_st.bad = bad;
            g_lastLog = frame;
        }

        // GetScriptClass(a0) from the pass (ra 0x1102678, sp the pass's frame). The plain cases as the native
        // (decomp_scriptvm3::getScriptClass) leaves them: its frame word (ra at sp - 0x10), v0 the class, v1 the jump
        // table entry (or the "in table" flag), sp / ra back, pc on ra. false: the table's call is needed (a call-out
        // case, an unknown entry, another function in the slot, the script VM switch off); nothing written then.
        inline bool scriptClassInline(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj, uint32_t sp)
        {
            if (__builtin_expect(sotc_decomp::guestFunction(runtime, kGetScriptClass) != &decomp_scriptvm3::getScriptClass || !sotc_decomp::scriptVmOn(), 0))
                return false;
            if (__builtin_expect(!inRam(obj + 8u, 1u) || !inRam(sp - 0x10u, 8u), 0))
                return false; // READ8 / WRITE64's special addresses: the native through the table
            const uint32_t idx = static_cast<uint32_t>(rdram[(obj + 8u) & PS2_RAM_MASK]) - 6u;
            const bool inTable = idx < 28u;
            uint32_t target = 0u, v0 = 0u;
            if (inTable)
            {
                target = ru32(rdram, 0x01190000u + (idx << 2) + 0x4CC0u);
                switch (target)
                {
                case 0x011024D0u: v0 = obj + 0x20u; break;
                case 0x011024D8u: v0 = obj + 0x1Cu; break;
                case 0x011024E0u: v0 = 0u; break;
                default: return false; // a getter's tail jump or an entry the native does not know
                }
            }
            constexpr uint32_t ra = 0x01102678u;
            SET_GPR_U32(ctx, 31, ra);
            const uint64_t ra64 = GPR_U64(ctx, 31);
            std::memcpy(rdram + ((sp - 0x10u) & PS2_RAM_MASK), &ra64, 8); // sd $ra, 0($sp) of its 0x10 frame
            if (inTable)
            {
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(target));
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            }
            else
            {
                SET_GPR_U64(ctx, 3, 0u);
                SET_GPR_U64(ctx, 2, 0u);
            }
            SET_GPR_U64(ctx, 31, ra64);
            SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
            ctx->pc = ra;
            return true;
        }

        __attribute__((cold, noinline)) void checkScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const R5900Context &before, uint32_t sp)
        {
            R5900Context ref = before;
            SET_GPR_U32(&ref, 31, 0x01102678u);
            SET_GPR_U32(&ref, 29, sp);
            ref.pc = kGetScriptClass;
            decomp_scriptvm3::getScriptClass(rdram, &ref, runtime);
            ++g_st.checks;
            const bool same = GPR_U64(&ref, 2) == GPR_U64(ctx, 2) && GPR_U64(&ref, 3) == GPR_U64(ctx, 3) && GPR_U64(&ref, 29) == GPR_U64(ctx, 29) &&
                              GPR_U64(&ref, 31) == GPR_U64(ctx, 31) && ref.pc == ctx->pc;
            if (!same)
                ++g_st.bad;
            if ((!same && g_st.bad <= 20u) || (g_st.checks % 200000u) == 0u)
                std::fprintf(stderr, "[w3c] GetScriptClass checks %llu differ %llu%s (obj %08x: v0 %08x/%08x v1 %08x/%08x pc %08x/%08x)\n",
                             static_cast<unsigned long long>(g_st.checks), static_cast<unsigned long long>(g_st.bad), same ? "" : " DIFFERS",
                             GPR_U32(&before, 4), GPR_U32(ctx, 2), GPR_U32(&ref, 2), GPR_U32(ctx, 3), GPR_U32(&ref, 3), ctx->pc, ref.pc);
        }

        // SOTC_W3_CHECK: the inline answer, then the native's on a copy of the context, compared.
        __attribute__((cold, noinline)) void scriptClassChecked(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj, uint32_t sp)
        {
            const R5900Context before = *ctx;
            if (scriptClassInline(rdram, ctx, runtime, obj, sp))
            {
                ++g_st.classInline;
                checkScriptClass(rdram, ctx, runtime, before, sp);
                return;
            }
            ++g_st.classGuest;
            callGuest(rdram, ctx, runtime, kGetScriptClass, 0x01102678u, sp);
        }

        // isysGroupForNext(a0, a1 = 0) from the pass (ra 0x1102730): decomp_group's native directly, its registers as
        // groupForNext leaves them. false: the table's call (another function in the slot, an entry outside main RAM).
        inline bool nextDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t entry, uint32_t sp)
        {
            if (__builtin_expect(sotc_decomp::guestFunction(runtime, kForNext) != &decomp_group::groupForNextSw, 0))
                return false;
            decomp_group::NextRegs r;
            if (!decomp_group::nextAt(rdram, entry, r))
                return false;
            constexpr uint32_t ra = 0x01102730u;
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.a0));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.a1));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(kLinks));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(r.a3));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
            ctx->pc = ra;
            return true;
        }

        // The next object's lines (its link, the object's +8 / +9 / +0xa line, the script class's +0x24..+0x50 at
        // +0x20 / +0x1c: two lines) asked for while this one runs.
        inline void prefetchNext(const uint8_t *rdram, uint32_t link)
        {
            const uint32_t index = ru16(rdram, link + 0xEu);
            if (index == 0u)
                return;
            const uint32_t next = kLinks + index * 0x18u;
            if (!inRam(next, 0x18u))
                return;
            const uint32_t obj = ru32(rdram, next);
            if (!inRam(obj, 0x80u))
                return;
            const uint8_t *o = rdram + obj;
            __builtin_prefetch(o);
            __builtin_prefetch(o + 0x40u);
            __builtin_prefetch(o + 0x60u);
        }

        // The wake (jal isysWakeupSubThread at 0x11026ac, ra 0x11026b4) as sotc_decomp::callGuest makes it, with the
        // run-to-return loop that runs the woken script's body in its own bracket.
        inline void wake(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj, uint32_t sp)
        {
            const uint32_t target = jalTarget(rdram, 0x011026ACu), ra = 0x011026B4u;
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, sp);
            ctx->pc = target;
            const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, target);
            if (__builtin_expect(!fn, 0))
                return sotc_decomp::callGuestSlow(rdram, ctx, runtime, target, ra, sp);
            {
                const ZoneIf z(0x6100u); // round X2: the wake's own samples (SOTC_X2_ZONE)
                fn(rdram, ctx, runtime);
            }
            runtime->checkTransfer(); // a thread switch below: off the stack (round 52)
            if (__builtin_expect(ctx->pc != ra || GPR_U32(ctx, 29) != sp, 1))
            {
                ++g_st.runs;
                if (g_runId == -2)
                    bracketInit();
                const sotc_p2::Scope run(g_level >= 4 ? typeBracket(rdram[(obj + 8u) & PS2_RAM_MASK]) : g_runId);
                if (__builtin_expect(g_rank != 0, 0))
                {
                    if (g_rank < 0)
                        rankInit();
                    if (g_rank)
                    {
                        rankedFinish(rdram, ctx, runtime, ra, sp, rdram[(obj + 8u) & PS2_RAM_MASK]);
                        return;
                    }
                }
                sotc_decomp::finishGuestCall(rdram, ctx, runtime, ra, sp);
            }
        }
    }

    bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_on != 0;
    }

    // manager_sub_01102638 (decomp_hot44d's scriptPass, which calls this when SOTC_W3=1): see the top of the file.
    void scriptPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++g_st.passes;
        const ZoneIf passZone(0x6200u); // round X2: the walk's own samples (SOTC_X2_ZONE)
        char104::wake();
        pf141::passBegin(); // round 141: a new frame's ranges; the last frame's first segments asked for
        const uint32_t sp = GPR_U32(ctx, 29) - 0x30u;
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 5, 0u);
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 31));
        uint64_t s2 = 0;
        SET_GPR_U64(ctx, 18, s2);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102658u), 0x01102660u, sp); // isysGroupForFirst
        uint64_t s3 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 19, s3);
        SET_GPR_U64(ctx, 20, 2u);
        if (s3 != 0u)
        {
            s2 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(static_cast<uint32_t>(s3)))));
            SET_GPR_U64(ctx, 18, s2);
            for (;;)
            {
                ++g_st.objects;
                const uint32_t obj = static_cast<uint32_t>(s2);
                if (inRam(static_cast<uint32_t>(s3), 0x10u))
                    prefetchNext(rdram, static_cast<uint32_t>(s3));
                // 0x1102670: GetScriptClass(s2)
                SET_GPR_U64(ctx, 4, s2);
                if (__builtin_expect(g_check != 0, 0))
                    scriptClassChecked(rdram, ctx, runtime, obj, sp);
                else if (scriptClassInline(rdram, ctx, runtime, obj, sp))
                    ++g_st.classInline;
                else
                {
                    ++g_st.classGuest;
                    callGuest(rdram, ctx, runtime, kGetScriptClass, 0x01102678u, sp);
                }
                const uint64_t s1 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 17, s1);
                const uint32_t c = static_cast<uint32_t>(s1);
                uint32_t v0 = READ32(c + 0x38u);
                SET_GPR_U32(ctx, 2, v0);
                SET_GPR_U64(ctx, 4, s2);
                if (v0 != 0u)
                {
                    WRITE8(c + 0x28u, 0u);
                    v0 = READ32(c + 0x3Cu);
                    SET_GPR_U32(ctx, 2, v0);
                    SET_GPR_U32(ctx, 5, READ32(v0 + 4u));
                    callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102690u), 0x01102698u, sp); // isysInitSubThreadExecEnv
                    WRITE32(c + 0x38u, 0u);
                }
                // 0x110269c
                v0 = READ8(c + 0x28u);
                SET_GPR_U32(ctx, 2, v0);
                SET_GPR_U64(ctx, 3, 1u);
                uint64_t s0;
                if (v0 == 1u && obj96::skipWake(rdram, obj, c))
                {
                    // Round 96: an unseen small creature left asleep this frame (as a sub-thread that did not end).
                    s0 = 0u;
                    SET_GPR_U64(ctx, 2, 0u);
                }
                else if (v0 == 1u)
                {
                    ++g_st.wakes;
                    char104::beforeWake();
                    SET_GPR_U64(ctx, 4, s2);
                    const bool timed = obj96::timing();
                    uint64_t t0 = 0u;
                    if (__builtin_expect(timed, 0))
                    {
                        obj96::wakeBegin();
                        t0 = obj96::now();
                    }
                    wake(rdram, ctx, runtime, obj, sp);
                    if (__builtin_expect(timed, 0))
                        obj96::wakeTime(obj, obj96::now() - t0);
                    s0 = GPR_U64(ctx, 2);
                }
                else
                {
                    SET_GPR_U64(ctx, 4, s2);
                    if (v0 == 0u)
                    {
                        WRITE8(c + 0x28u, static_cast<uint8_t>(GPR_U32(ctx, 3)));
                        SET_GPR_U32(ctx, 5, READ32(c + 0x24u));
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026CCu), 0x011026D4u, sp); // isysStartSubThread
                        s0 = GPR_U64(ctx, 2);
                    }
                    else
                    {
                        s0 = 0u;
                        SET_GPR_U64(ctx, 16, s0);
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026E0u), 0x011026E8u, sp); // isysLetterObjDiscardLetterAll
                    }
                }
                SET_GPR_U64(ctx, 16, s0);
                // 0x11026e8
                SET_GPR_U64(ctx, 4, s3);
                if (s0 != 0u)
                {
                    const uint32_t cb = READ32(c + 0x4Cu);
                    SET_GPR_U32(ctx, 2, cb);
                    if (cb != 0u)
                    {
                        SET_GPR_U32(ctx, 5, READ32(c + 0x50u));
                        SET_GPR_U64(ctx, 4, s2);
                        callGuest(rdram, ctx, runtime, cb, 0x01102708u, sp);
                    }
                    v0 = READ8(c + 0x29u);
                    // 0x110270c
                    v0 &= 1u;
                    SET_GPR_U32(ctx, 2, v0);
                    WRITE8(c + 0x28u, static_cast<uint8_t>(GPR_U32(ctx, 20)));
                    if (v0 == 0u)
                    {
                        SET_GPR_U64(ctx, 4, s2);
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0110271Cu), 0x01102724u, sp); // isysRequestDeleteObj
                    }
                    SET_GPR_U64(ctx, 4, s3);
                }
                // 0x1102728: isysGroupForNext(s3, 0)
                SET_GPR_U64(ctx, 5, 0u);
                if (jalTarget(rdram, 0x01102728u) == kForNext && nextDirect(rdram, ctx, runtime, static_cast<uint32_t>(GPR_U32(ctx, 4)), sp))
                    ++g_st.nextDirect;
                else
                {
                    ++g_st.nextGuest;
                    callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102728u), 0x01102730u, sp);
                }
                s3 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 19, s3);
                if (s3 == 0u)
                    break;
                s2 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(static_cast<uint32_t>(s3)))));
                SET_GPR_U64(ctx, 18, s2);
            }
        }
        // 0x110273c
        char104::groupEnd(); // round 104: a character's job joined by the group's end
        if (__builtin_expect(g_logEvery != 0u, 0))
            logLine();
        SET_GPR_U64(ctx, 4, s2);
        if (s2 != 0u)
        {
            // isysGroupForExit(s2, 0) as a tail jump: it returns to our caller.
            SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
            SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0x20u));
            const uint64_t ra = READ64(sp + 0x28u);
            const uint32_t target = jalTarget(rdram, 0x01102760u);
            callGuest(rdram, ctx, runtime, target, static_cast<uint32_t>(ra), sp + 0x30u);
            ctx->pc = static_cast<uint32_t>(ra);
            return;
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x28u));
        SET_GPR_U32(ctx, 29, sp + 0x30u);
        ctx->pc = GPR_U32(ctx, 31);
    }
}
