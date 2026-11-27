// Round X2 (SOTC_X2=1, off by default; docs/decomp_x2.md): the script bodies by weight, the heaviest as native C++.
//
// Ranking (round X2's zone split of execScript's self, decomp_w3.cpp's SOTC_X2_ZONE / SOTC_X2_RANK): the script bodies are
// thin sequencers; their cost is the libraries they call. By the console's own samples per function (Vita3K's zone split at
// the console's bracket level applied to the 2026-11-26 console run, docs/decomp_x2.md): the small animals' script
// (gamecore_sub_013E3840) ~0.76 ms a frame, st_collisionalias ~0.71, st_playObjAnim ~0.57, the sheath script ~0.31, Agro's AI
// (gamecore_sub_01395E20) ~0.28, the player's script ~0.18, the sound / small-object manager ~0.18; ~3.5 ms in all.
//
// What X2 makes native (each a C++ function on a typed view of the guest structs; no lifted code, no guest call for a getter):
//  - the animal script's body (decomp_scriptvm4's animalScript hands over while SOTC_X2=1): the animal's work as a struct view
//    (Animal), the frame-step wait, the state reset, the collision decision, the position / rotation copies and the timers as
//    plain C++ floats; isysGetThisSubThread / GetLayoutClass / GetCharacterClass as host reads (their plain cases);
//    AI_OptFloatFrameSample (the per-frame motion sample) as a native called directly; the AI steps and the sleeps as calls.
//  - AI_OptFloatFrameSample (0x13d38c0) itself (also through the table: x2::optSample).
//
// Coroutine semantics, kept: the body sleeps in scrRun / scrRunM (callMaySleep: false = it slept, return at once) and is
// resumed at the labels after them (0x13e38b0 / 0x13e38f8 / 0x13e3908 / 0x13e3a7c, decomp_scriptvm4's resume entries, which
// hand over here). The callee-saved registers the original keeps for the whole body (s0 the animal, s1 this sub-thread, s2
// its character class, s3 0x1465650, s4 +0x7d0, s5 +0x890, s6 6, s7 0x1460000, fp +0x14, f20 1.0) and its frame (0x60
// bytes, the saves at the original's offsets) are the original's at every call and sleep, so a sub-thread that slept in the
// old body resumes in this one and the other way round; every guest store is the original's, in the original's order where
// two could alias. Not as the original: the scratch registers (v0 / v1 / a* / at / f0-f7 / the condition bit) at a sleep,
// which the sleep stores into the sub-thread's save area and nothing reads back (each is written before it is read after the
// resume label); the back edges' EE checkpoints (as every native loop).
//
// SOTC_X2_CHECK=1: each native step is compared against the old statements on the same input (the state reset, the collision
// decision, the copies, the timers on a copy of the animal's work; each inline getter against the guest call), "[x2c]" lines.
// SOTC_X2_LOG=<n>: "[x2]" counts every n frames.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace lift_d1
{
    void AI_OptFloatFrameSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace x2
{
    namespace
    {
        signed char g_on = -1;
        int g_check = 0;
        uint64_t g_logEvery = 0, g_lastLog = 0;
        struct Stats
        {
            uint64_t frames, resumes, getters, gettersGuest, samples, samplesGuest, checks, bad;
        } g_st{};

        __attribute__((cold, noinline)) void init()
        {
            const char *e = std::getenv("SOTC_X2");
            g_on = (e && std::atoi(e) != 0) ? 1 : 0;
            const char *c = std::getenv("SOTC_X2_CHECK");
            g_check = c && std::atoi(c) != 0;
            const char *l = std::getenv("SOTC_X2_LOG");
            g_logEvery = l ? std::strtoull(l, nullptr, 10) : 0u;
            if (g_on)
                std::fprintf(stderr, "[x2] on: the animal script and AI_OptFloatFrameSample as native C++ (check %d, log every %llu)\n", g_check,
                             static_cast<unsigned long long>(g_logEvery));
        }

        __attribute__((cold, noinline)) void logLine()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_lastLog + g_logEvery)
                return;
            const double n = g_lastLog ? static_cast<double>(frame - g_lastLog) : 1.0;
            std::fprintf(stderr,
                         "[x2] frames %llu-%llu: animal frames/f %.2f resumes/f %.2f | getters host %llu guest %llu | samples native %llu guest %llu | "
                         "checks %llu bad %llu\n",
                         static_cast<unsigned long long>(g_lastLog), static_cast<unsigned long long>(frame), g_st.frames / n, g_st.resumes / n,
                         static_cast<unsigned long long>(g_st.getters), static_cast<unsigned long long>(g_st.gettersGuest),
                         static_cast<unsigned long long>(g_st.samples), static_cast<unsigned long long>(g_st.samplesGuest),
                         static_cast<unsigned long long>(g_st.checks), static_cast<unsigned long long>(g_st.bad));
            const uint64_t checks = g_st.checks, bad = g_st.bad;
            g_st = Stats{};
            g_st.checks = checks;
            g_st.bad = bad;
            g_lastLog = frame;
        }

        __attribute__((cold, noinline)) void note(const char *what, bool same, uint32_t obj, uint32_t a, uint32_t b)
        {
            ++g_st.checks;
            if (!same)
                ++g_st.bad;
            if ((!same && g_st.bad <= 20u) || (g_st.checks % 100000u) == 0u)
                std::fprintf(stderr, "[x2c] %s checks %llu differ %llu%s (obj %08x: %08x / %08x)\n", what, static_cast<unsigned long long>(g_st.checks),
                             static_cast<unsigned long long>(g_st.bad), same ? "" : " DIFFERS", obj, a, b);
        }

        inline uint32_t bitsOf(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        inline float floatOf(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }

        // ---- the guest structs ---------------------------------------------------------------------------------------
        constexpr uint32_t kGlobals = 0x01465650u;      // the script system's globals
        constexpr uint32_t kFrameStep = kGlobals + 0x190u; // float: this frame's step
        constexpr uint32_t kThisSubThread = 0x001E1550u; // the running sub-thread's object
        constexpr uint32_t kGetThisSubThread = 0x001CA1B0u, kGetLayoutClass = 0x010F1A20u, kGetCharacterClass = 0x010C0A70u;
        constexpr uint32_t kScrRun = 0x013CF9A8u, kScrRunM = 0x013CF9C8u, kSCRGetFirstObj = 0x0137C348u, kOptSample = 0x013D38C0u;
        constexpr uint32_t kBody = 0x013E3840u;

        // A small animal's work (the script's object, s0), by its guest offsets.
        struct Animal
        {
            enum : uint32_t
            {
                kKind = 0x00,       // int: 6 = no collision handling
                kOpt = 0x14,        // the motion sample's record (fp)
                kLayout = 0x24,     // its layout object (SCRGetFirstObj)
                kFlag44 = 0x44,     // int
                kSpeed = 0x90,      // float
                kAction = 0x260,    // int: the current action; +0x264 the last frame's
                kState = 0x26C,     // int: the current state; +0x268 the last frame's
                kSub = 0x274,       // int: +0x270 the last frame's
                kNext = 0x27C,      // int: +0x278 the last frame's
                kTimer0 = 0x290,    // floats advanced by the frame step: +0x290, +0x298, +0x2a0, +0x2c0..+0x2cc, +0x7b4
                kPos = 0x330,       // qword: the layout's position, copied each frame
                kRot = 0x350,       // qword: the layout's rotation
                kTimer7b4 = 0x7B4,
                kHits = 0x7BC,      // int
                kBlockA = 0x7D0,    // s4: +0 cur, +4 last, +0xc / +0x14 timers, +0x18 = 1.0
                kBlockB = 0x890,    // s5: +0x28 / +0x2c timers
            };
        };
        // The layout class: +0x134 its position's pointer, +0x13c its rotation's.
        // The character class: +0x53b the "far" byte, +0x52b / +0x53a the sample's two bytes.

        inline bool plain(uint32_t a, uint32_t size) { return a < PS2_RAM_SIZE && a + size <= PS2_RAM_SIZE; }

        // ---- the getters as host reads (their plain cases; the jal's live target must be the getter) -------------------
        // GetLayoutClass(obj): by the type byte (+8): 9 + index through the table at 0x1194070: obj + 0x20, obj + 0x90 or 0;
        // a type outside the table 0. false: another case (the guest call).
        inline bool layoutClassHost(const d1::Mem m, uint32_t obj, uint32_t &cls)
        {
            if (!plain(obj + 8u, 1u))
                return false;
            const uint32_t index = static_cast<uint32_t>(m.get<uint8_t>(obj + 8u)) - 9u;
            if (index >= 0xEu)
            {
                cls = 0u;
                return true;
            }
            const uint32_t t = m.u32(0x01194070u + index * 4u);
            if (t == 0x010F1A58u)
                cls = obj + 0x20u;
            else if (t == 0x010F1A98u)
                cls = obj + 0x90u;
            else if (t == 0x010F1AA0u)
                cls = 0u;
            else
                return false;
            return true;
        }
        // GetCharacterClass(obj): type 9 or 0x16: obj + 0x90, else 0.
        inline uint32_t characterClassHost(const d1::Mem m, uint32_t obj)
        {
            const uint32_t type = m.get<uint8_t>(obj + 8u);
            return (type == 9u || type == 0x16u) ? obj + 0x90u : 0u;
        }

        // A call through the table with the frame's sp and ra (the jal's + 8); a0 / a1 set by the caller.
        inline void call(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
        {
            sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
        }

        // A getter at a relocated jal site: the host read when the site still names it, else the guest call (v0 back).
        __attribute__((noinline)) uint32_t layoutClassAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t site, uint32_t obj, uint32_t sp)
        {
            const d1::Mem m{rdram};
            uint32_t cls;
            if (sotc_decomp::jalTarget(rdram, site) == kGetLayoutClass && sotc_decomp::guestFunction(runtime, kGetLayoutClass) &&
                layoutClassHost(m, obj, cls))
            {
                ++g_st.getters;
                if (__builtin_expect(g_check != 0, 0))
                {
                    SET_GPR_U32(ctx, 4, obj);
                    call(rdram, ctx, runtime, kGetLayoutClass, site + 8u, sp);
                    note("GetLayoutClass", GPR_U32(ctx, 2) == cls, obj, cls, GPR_U32(ctx, 2));
                }
                return cls;
            }
            ++g_st.gettersGuest;
            SET_GPR_U32(ctx, 4, obj);
            call(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp);
            return GPR_U32(ctx, 2);
        }
        __attribute__((noinline)) uint32_t characterClassAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t site, uint32_t obj, uint32_t sp)
        {
            const d1::Mem m{rdram};
            if (sotc_decomp::jalTarget(rdram, site) == kGetCharacterClass && plain(obj + 8u, 1u))
            {
                ++g_st.getters;
                const uint32_t cls = characterClassHost(m, obj);
                if (__builtin_expect(g_check != 0, 0))
                {
                    SET_GPR_U32(ctx, 4, obj);
                    call(rdram, ctx, runtime, kGetCharacterClass, site + 8u, sp);
                    note("GetCharacterClass", GPR_U32(ctx, 2) == cls, obj, cls, GPR_U32(ctx, 2));
                }
                return cls;
            }
            ++g_st.gettersGuest;
            SET_GPR_U32(ctx, 4, obj);
            call(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp);
            return GPR_U32(ctx, 2);
        }
        // isysGetThisSubThread at a relocated jal site: *0x1e1550 while the site still names it.
        inline uint32_t thisSubThreadAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t site, uint32_t sp)
        {
            const uint32_t target = sotc_decomp::jalTarget(rdram, site);
            if (target == kGetThisSubThread)
                return d1::Mem{rdram}.u32(kThisSubThread);
            call(rdram, ctx, runtime, target, site + 8u, sp);
            return GPR_U32(ctx, 2);
        }
    }

    bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_on != 0;
    }

    // ---- AI_OptFloatFrameSample (0x13d38c0): a character's motion sample this frame --------------------------------------
    // a0 the character object, a1 its sample record (+0 the motion, +4 the limit, +8 the distance out). The motion of the
    // object at its layout position (013D5600(frame, position): v0 the motion, 0 none) into +0; with one, the distance
    // (0108F518(), then 01160B00(it, position, f12 = 1.0): f0) into +8. The character class's bytes: distance below the limit
    // or no motion: +0x52b = 0, +0x53a = 1, v0 0; else +0x53a = 0, +0x52b = 1, v0 1 (v1 1 on every path).
    // Frame 0x30: s0 +0x10, s1 +0x18, s2 +0x20, ra +0x28 (sp + 0 the 013D5600 out area); the callee-saved registers as the
    // original sets them before each call (s0 the object, then its layout class; s1 the record; s2 the character class).
    void optSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on() || ctx->pc != kOptSample)
            return lift_d1::AI_OptFloatFrameSample(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        const uint32_t obj = GPR_U32(ctx, 4), rec = GPR_U32(ctx, 5);
        if (!plain(sp, 0x30u) || !plain(rec, 12u))
            return lift_d1::AI_OptFloatFrameSample(rdram, ctx, runtime);
        ++g_st.samples;
        // The prologue's saves (s0 / s1 / s2 / ra) at the original's offsets.
        m.set<uint64_t>(sp + 0x10u, GPR_U64(ctx, 16));
        m.set<uint64_t>(sp + 0x18u, GPR_U64(ctx, 17));
        m.set<uint64_t>(sp + 0x20u, GPR_U64(ctx, 18));
        m.set<uint64_t>(sp + 0x28u, ra);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4)); // s0 = the object
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5)); // s1 = the record
        const uint32_t cls = characterClassAt(rdram, ctx, runtime, 0x013D38D8u, obj, sp);
        SET_GPR_U32(ctx, 18, cls); // s2 = the character class
        const uint32_t lay = layoutClassAt(rdram, ctx, runtime, 0x013D38E4u, obj, sp);
        SET_GPR_U32(ctx, 16, lay); // s0 = the layout class
        const uint32_t pos = m.u32(lay + 0x134u);
        SET_GPR_U32(ctx, 4, sp);
        SET_GPR_U32(ctx, 5, pos);
        call(rdram, ctx, runtime, 0x013D5600u, 0x013D38FCu, sp);
        const uint32_t motion = GPR_U32(ctx, 2);
        m.set<uint32_t>(rec, motion);
        uint32_t v0 = 0u;
        if (motion != 0u)
        {
            call(rdram, ctx, runtime, 0x0108F518u, 0x013D3910u, sp);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
            SET_GPR_U32(ctx, 5, m.u32(lay + 0x134u));
            SET_GPR_S32(ctx, 1, 0x3F800000);
            ctx->f[12] = 1.0f;
            call(rdram, ctx, runtime, 0x01160B00u, 0x013D3924u, sp);
            const float dist = ctx->f[0], limit = m.f32(rec + 4u);
            m.set<float>(rec + 8u, dist);
            const bool near = ps2FpuCompareKey(dist) < ps2FpuCompareKey(limit); // c.lt.s $f0, $f1
            if (near)
            {
                m.set<uint8_t>(cls + 0x52Bu, 0u);
                m.set<uint8_t>(cls + 0x53Au, 1u);
            }
            else
            {
                m.set<uint8_t>(cls + 0x53Au, 0u);
                m.set<uint8_t>(cls + 0x52Bu, 1u);
                v0 = 1u;
            }
        }
        else
        {
            m.set<uint8_t>(cls + 0x52Bu, 0u);
            m.set<uint8_t>(cls + 0x53Au, 1u);
        }
        SET_GPR_U64(ctx, 3, 1u);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x10u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x18u));
        SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x20u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    namespace
    {
        // ---- the animal's per-frame steps as plain C++ (each one also has its check) ----------------------------------

        // State 4 / 5 / 6 with action 0x1c: back to state 0, the next state 2.
        inline void stateReset(const d1::Mem m, uint32_t a)
        {
            const int32_t state = m.s32(a + Animal::kState);
            if ((state == 4 || state == 5 || state == 6) && m.s32(a + Animal::kAction) == 0x1C)
            {
                m.set<int32_t>(a + Animal::kState, 0);
                m.set<int32_t>(a + Animal::kNext, 2);
            }
        }
        // The old statements' branch order (decomp_scriptvm4 0x13e3964..0x13e399c) on the same two words.
        __attribute__((cold, noinline)) void stateResetRef(int32_t state, int32_t action, int32_t &stateOut, int32_t &nextOut, bool &wrote)
        {
            wrote = false;
            int32_t v1 = state;
            if (v1 == 4)
                v1 = action;
            else if (v1 == 6 || v1 == 5)
                v1 = action;
            else
                return;
            if (v1 != 28)
                return;
            stateOut = 0;
            nextOut = 2;
            wrote = true;
        }

        // The collision handling's on / off (unless the kind is 6): on when (+0x44 set and the state below 0x12), or the
        // state is 0x12, or (the state is not 0x13 and +0x7bc > 0).
        inline bool collisionOn(const d1::Mem m, uint32_t a)
        {
            const int32_t state = m.s32(a + Animal::kState);
            if (m.s32(a + Animal::kFlag44) != 0 && state < 0x12)
                return true;
            if (state == 0x12)
                return true;
            if (state == 0x13)
                return false;
            return m.s32(a + Animal::kHits) > 0;
        }
        __attribute__((cold, noinline)) bool collisionOnRef(int32_t flag44, int32_t state, int32_t hits)
        {
            // 0x13e39d8..0x13e3a28 in the old body's order
            if (flag44 != 0)
            {
                if (state < 18)
                    return true;
            }
            if (state == 18)
                return true;
            if (state == 19)
                return false;
            return hits > 0;
        }

        // The timers by the frame step (the original's store order), the last frame's state words.
        inline void advance(const d1::Mem m, uint32_t a)
        {
            const float step = m.f32(kFrameStep);
            const uint32_t timers[] = {0x290u, 0x298u, 0x2A0u, 0x2C0u, 0x2C4u, 0x2C8u, 0x2CCu};
            for (uint32_t off : timers)
                m.set<float>(a + off, d1::add(m.f32(a + off), step));
            const int32_t state = m.s32(a + Animal::kState), sub = m.s32(a + Animal::kSub), next = m.s32(a + Animal::kNext),
                          action = m.s32(a + Animal::kAction);
            m.set<int32_t>(a + 0x268u, state);
            m.set<int32_t>(a + 0x270u, sub);
            m.set<int32_t>(a + 0x278u, next);
            m.set<int32_t>(a + 0x264u, action);
            m.set<float>(a + Animal::kTimer7b4, d1::add(m.f32(a + Animal::kTimer7b4), step));
            const uint32_t b = a + Animal::kBlockA;
            m.set<uint32_t>(b + 4u, m.u32(b));
            m.set<float>(b + 0xCu, d1::add(m.f32(b + 0xCu), step));
            m.set<float>(b + 0x18u, 1.0f);
            m.set<float>(b + 0x14u, d1::add(m.f32(b + 0x14u), step));
            const uint32_t c = a + Animal::kBlockB;
            m.set<float>(c + 0x28u, d1::add(m.f32(c + 0x28u), step));
            m.set<float>(c + 0x2Cu, d1::add(m.f32(c + 0x2Cu), step));
        }
        // The check: the work's bytes before, the old statements (FPU_ADD_S on the same words, the same stores) on a copy,
        // compared with what advance() wrote.
        struct AdvanceRef
        {
            uint8_t lo[0x80], a[0x20], b[0x30];
        };
        __attribute__((cold, noinline)) void advanceRef(const d1::Mem m, uint32_t a, AdvanceRef &r)
        {
            std::memcpy(r.lo, m.base + ((a + 0x260u) & PS2_RAM_MASK), sizeof(r.lo)); // +0x260..+0x2e0
            std::memcpy(r.a, m.base + ((a + 0x7B0u) & PS2_RAM_MASK), sizeof(r.a));   // +0x7b0..+0x7d0
            std::memcpy(r.b, m.base + ((a + 0x7D0u) & PS2_RAM_MASK), sizeof(r.b));   // block A +0..+0x30 (and +0x7d0..)
            const float step = m.f32(kFrameStep);
            auto f = [](uint8_t *p, uint32_t off) { float v; std::memcpy(&v, p + off, 4); return v; };
            auto put = [](uint8_t *p, uint32_t off, float v) { std::memcpy(p + off, &v, 4); };
            auto w = [](uint8_t *p, uint32_t off) { uint32_t v; std::memcpy(&v, p + off, 4); return v; };
            auto putw = [](uint8_t *p, uint32_t off, uint32_t v) { std::memcpy(p + off, &v, 4); };
            for (uint32_t off : {0x30u, 0x38u, 0x40u, 0x60u, 0x64u, 0x68u, 0x6Cu})
                put(r.lo, off, FPU_ADD_S(f(r.lo, off), step));
            const uint32_t st = w(r.lo, 0xC), sub = w(r.lo, 0x14), nx = w(r.lo, 0x1C), ac = w(r.lo, 0);
            putw(r.lo, 0x8, st);
            putw(r.lo, 0x10, sub);
            putw(r.lo, 0x18, nx);
            putw(r.lo, 0x4, ac);
            put(r.a, 4, FPU_ADD_S(f(r.a, 4), step));
            putw(r.b, 4, w(r.b, 0));
            put(r.b, 0xC, FPU_ADD_S(f(r.b, 0xC), step));
            put(r.b, 0x18, 1.0f);
            put(r.b, 0x14, FPU_ADD_S(f(r.b, 0x14), step));
        }
        __attribute__((cold, noinline)) void advanceChecked(const d1::Mem m, uint32_t a)
        {
            AdvanceRef r;
            advanceRef(m, a, r);
            const uint32_t c = a + Animal::kBlockB;
            const float step = m.f32(kFrameStep);
            const uint32_t b28 = bitsOf(FPU_ADD_S(m.f32(c + 0x28u), step)), b2c = bitsOf(FPU_ADD_S(m.f32(c + 0x2Cu), step));
            advance(m, a);
            const bool same = std::memcmp(r.lo, m.base + ((a + 0x260u) & PS2_RAM_MASK), sizeof(r.lo)) == 0 &&
                              std::memcmp(r.a, m.base + ((a + 0x7B0u) & PS2_RAM_MASK), sizeof(r.a)) == 0 &&
                              std::memcmp(r.b, m.base + ((a + 0x7D0u) & PS2_RAM_MASK), sizeof(r.b)) == 0 && m.u32(c + 0x28u) == b28 &&
                              m.u32(c + 0x2Cu) == b2c;
            note("timers", same, a, m.u32(a + 0x290u), 0u);
        }
    }

    // ---- gamecore_sub_013E3840: a small animal's script body (decomp_scriptvm4's animalScript hands over) ----------------
    // false: not handled here (a pc this body does not know; the work outside main RAM): the old body runs.
    bool animalScript(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        uint32_t sp = GPR_U32(ctx, 29);
        uint32_t a = GPR_U32(ctx, 16); // the animal (s0) on a resume
        auto sleepRunM = [&](uint32_t ra)
        {
            SET_GPR_U64(ctx, 4, 1u);
            return sotc_decomp::callMaySleep(rdram, ctx, runtime, kScrRunM, ra, sp);
        };
        switch (ctx->pc)
        {
        case kBody:
            a = GPR_U32(ctx, 4);
            if (!plain(a, 0x900u) || !plain(sp - 0x60u, 0x60u))
                return false;
            break;
        case 0x013E38B0u:
        case 0x013E38F8u:
        case 0x013E3908u:
        case 0x013E3A7Cu:
            if (!plain(a, 0x900u) || !plain(sp, 0x60u))
                return false;
            ++g_st.resumes;
            break;
        default:
            return false;
        }
        // Where the body goes on: its entry (the prologue, then a frame's top) or the label after the sleep it resumes from.
        enum Phase
        {
            kTop,      // s1 = this sub-thread, then the layout object's wait
            kFirstObj, // 0x13e38b0: SCRGetFirstObj (after scrRun: none yet)
            kGone,     // 0x13e38f8: below the world, frame ends for good
            kWait,     // 0x13e3908: the frame-step wait
            kAdvance,  // 0x13e3a7c: the frame ended: the timers, then the next frame's top
        } phase = kTop;
        switch (ctx->pc)
        {
        case 0x013E38B0u: phase = kFirstObj; break;
        case 0x013E38F8u: phase = kGone; break;
        case 0x013E3908u: phase = kWait; break;
        case 0x013E3A7Cu: phase = kAdvance; break;
        default:
            // The prologue: the frame's saves at the original's offsets, the body's constants in s3..s7 / fp / f20.
            sp -= 0x60u;
            m.set<uint64_t>(sp + 0x00u, GPR_U64(ctx, 16));
            m.set<uint64_t>(sp + 0x08u, GPR_U64(ctx, 17));
            m.set<uint64_t>(sp + 0x10u, GPR_U64(ctx, 18));
            m.set<uint64_t>(sp + 0x18u, GPR_U64(ctx, 19));
            m.set<uint64_t>(sp + 0x20u, GPR_U64(ctx, 20));
            m.set<uint64_t>(sp + 0x28u, GPR_U64(ctx, 21));
            m.set<uint64_t>(sp + 0x30u, GPR_U64(ctx, 22));
            m.set<uint64_t>(sp + 0x38u, GPR_U64(ctx, 23));
            m.set<uint64_t>(sp + 0x40u, GPR_U64(ctx, 30));
            m.set<uint64_t>(sp + 0x48u, GPR_U64(ctx, 31));
            m.set<uint32_t>(sp + 0x50u, bitsOf(ctx->f[20]));
            SET_GPR_U32(ctx, 29, sp);
            SET_GPR_U32(ctx, 16, a);
            SET_GPR_U32(ctx, 19, kGlobals);
            SET_GPR_U32(ctx, 20, a + Animal::kBlockA);
            SET_GPR_U32(ctx, 21, a + Animal::kBlockB);
            SET_GPR_U64(ctx, 22, 6u);
            SET_GPR_U32(ctx, 23, 0x01460000u);
            SET_GPR_U32(ctx, 30, a + Animal::kOpt);
            ctx->f[20] = 1.0f;
            break;
        }

        for (;;)
        {
            switch (phase)
            {
            case kTop:
                SET_GPR_U32(ctx, 17, thisSubThreadAt(rdram, ctx, runtime, 0x013E3898u, sp));
                phase = kFirstObj;
                continue;

            case kFirstObj:
            {
                SET_GPR_U32(ctx, 4, 0x01465900u);
                call(rdram, ctx, runtime, kSCRGetFirstObj, 0x013E38B8u, sp);
                const uint32_t layoutObj = GPR_U32(ctx, 2);
                m.set<uint32_t>(a + Animal::kLayout, layoutObj);
                if (layoutObj == 0u)
                {
                    // No layout yet: end this frame (scrRun), then ask again.
                    if (!sotc_decomp::callMaySleep(rdram, ctx, runtime, kScrRun, 0x013E38B0u, sp))
                        return true;
                    continue;
                }
                // Gone below the world (y < -1000): the layout's access off, frame ends for good.
                const uint32_t self = GPR_U32(ctx, 17);
                const uint32_t lay = layoutClassAt(rdram, ctx, runtime, 0x013E38C0u, self, sp);
                const float y = m.f32(m.u32(lay + 0x134u) + 4u);
                if (ps2FpuCompareKey(y) < ps2FpuCompareKey(-1000.0f))
                {
                    SET_GPR_U32(ctx, 4, self);
                    SET_GPR_U64(ctx, 5, 0u);
                    call(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x013E38E8u), 0x013E38F0u, sp);
                    phase = kGone;
                }
                else
                    phase = kWait;
                continue;
            }

            case kGone:
                if (!sleepRunM(0x013E38F8u))
                    return true;
                continue;

            case kWait:
            {
                // Wait (a frame end a frame) while the frame step is >= 1.0 or 0.
                const float step = m.f32(kFrameStep);
                if ((ps2FpuCompareKey(1.0f) <= ps2FpuCompareKey(step)) || (ps2FpuCompareKey(step) == ps2FpuCompareKey(0.0f)))
                {
                    if (!sleepRunM(0x013E3908u))
                        return true;
                    continue;
                }
                break; // the AI frame below
            }

            case kAdvance:
                if (__builtin_expect(g_check != 0, 0))
                    advanceChecked(m, a);
                else
                    advance(m, a);
                if (__builtin_expect(g_logEvery != 0u, 0))
                    logLine();
                phase = kTop;
                continue;
            }

            // ---- the AI frame ----
            ++g_st.frames;
            static constexpr uint32_t kSteps[][2] = {{0x013D89A8u, 0x013E3930u}, {0x013E1520u, 0x013E3938u}, {0x013E1738u, 0x013E3940u},
                                                     {0x013E1E48u, 0x013E3948u}, {0x013DC5F8u, 0x013E3950u}, {0x013DCB80u, 0x013E3958u}};
            for (const auto &s : kSteps)
            {
                SET_GPR_U32(ctx, 4, a);
                call(rdram, ctx, runtime, s[0], s[1], sp);
            }
            SET_GPR_U32(ctx, 4, a);
            SET_GPR_U64(ctx, 5, 0u);
            call(rdram, ctx, runtime, 0x013DCD78u, 0x013E3964u, sp);
            if (__builtin_expect(g_check != 0, 0))
            {
                int32_t so = m.s32(a + Animal::kState), no = m.s32(a + Animal::kNext);
                bool wrote;
                stateResetRef(so, m.s32(a + Animal::kAction), so, no, wrote);
                stateReset(m, a);
                note("stateReset", m.s32(a + Animal::kState) == so && m.s32(a + Animal::kNext) == no, a, static_cast<uint32_t>(so),
                     static_cast<uint32_t>(m.s32(a + Animal::kState)));
            }
            else
                stateReset(m, a);
            SET_GPR_U32(ctx, 4, a);
            call(rdram, ctx, runtime, 0x013E2A78u, 0x013E39A4u, sp);
            SET_GPR_U32(ctx, 4, a);
            call(rdram, ctx, runtime, 0x013E30D0u, 0x013E39ACu, sp);

            // s1 = this sub-thread, s2 = its character class; the motion sample (fp = +0x14 its record).
            const uint32_t self = thisSubThreadAt(rdram, ctx, runtime, 0x013E39ACu, sp);
            SET_GPR_U32(ctx, 17, self);
            const uint32_t cls = characterClassAt(rdram, ctx, runtime, 0x013E39B8u, self, sp);
            SET_GPR_U32(ctx, 18, cls);
            SET_GPR_U32(ctx, 4, self);
            SET_GPR_U32(ctx, 5, a + Animal::kOpt);
            if (sotc_decomp::guestFunction(runtime, kOptSample) == &lift_d1::AI_OptFloatFrameSample)
            {
                SET_GPR_U32(ctx, 31, 0x013E39D0u);
                ctx->pc = kOptSample;
                optSample(rdram, ctx, runtime);
            }
            else
            {
                ++g_st.samplesGuest;
                call(rdram, ctx, runtime, kOptSample, 0x013E39D0u, sp);
            }

            // The collision handling and the class's +0x53b byte, unless the kind is 6.
            if (m.s32(a + Animal::kKind) != 6)
            {
                const bool on = collisionOn(m, a);
                if (__builtin_expect(g_check != 0, 0))
                {
                    const bool ref = collisionOnRef(m.s32(a + Animal::kFlag44), m.s32(a + Animal::kState), m.s32(a + Animal::kHits));
                    note("collision", ref == on, a, on, ref);
                }
                SET_GPR_U32(ctx, 4, self);
                if (on)
                {
                    SET_GPR_U64(ctx, 5, 1u);
                    call(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x013E39F8u), 0x013E3A00u, sp);
                    m.set<uint8_t>(cls + 0x53Bu, 0u);
                }
                else
                {
                    SET_GPR_U64(ctx, 5, 0u);
                    call(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x013E3A28u), 0x013E3A30u, sp);
                    const bool fast = ps2FpuCompareKey(80.0f) < ps2FpuCompareKey(m.f32(a + Animal::kSpeed));
                    m.set<uint8_t>(cls + 0x53Bu, fast ? 1u : 0u);
                }
            }
            {
                // The layout's position / rotation quadwords into +0x330 / +0x350.
                const uint32_t lay = layoutClassAt(rdram, ctx, runtime, 0x013E3A50u, m.u32(a + Animal::kLayout), sp);
                WRITE128(a + Animal::kPos, READ128(m.u32(lay + 0x134u)));
                WRITE128(a + Animal::kRot, READ128(m.u32(lay + 0x13Cu)));
            }
            (void)thisSubThreadAt(rdram, ctx, runtime, 0x013E3A6Cu, sp); // its result unused
            if (!sleepRunM(0x013E3A7Cu))
                return true;
            phase = kAdvance;
        }
    }
}
