// The player's grip ring (round 11): gamecore_sub_013F8958 (the two
// flash fades, 0x11f8604 / 0x11f861c) and gamecore_sub_013F8668 (the ring
// itself), called every frame by playerlifebar_script's sub-thread, also
// while the ring is not shown.
//
// 013F8668 reads the grip ratio (getPlayerMaxGripRatio), derives the ring's
// two scales (0x1466b3c / 0x1466b40) and its pulse phase (0x11f8620), then
// draws: four rings of 44 points (013F6B20, 013F7178, 013F78A8, 013F75D0:
// each point a guest iosGetCosf + iosGetSinf + getDrawScreenHeight +
// iosFTOI4Vector, ~200 gsReg calls in all) on display list layer 0x19,
// with texSearchMopByName / texTransRequest for its texture, and the two
// flashes (013F7F38, 013F8380) when their fades are above 0. 013F78A8 also
// keeps the ring's shrink history (0x11f8530, 16 pairs; 0x1466b68 / b6c)
// and its scale 0x1466b44. The point arrays are in 8668's own frame.
//
// Game-specific shortcut: with the ring's alpha (*0x11f85b4, the ring's
// fade entry) at 0 and neither flash up, every primitive it draws has
// alpha 0 (ABE on, ALPHA (Cs - Cd) As + Cd / Cs As + Cd, so nothing
// shows; the GS's frame-buffer alpha of those pixels is the one thing a
// GS picture could keep of them). Then only the state is made, natively:
// b30 / b3c / b40, the pulse phase, 013F78A8's history and b44, in the
// original's operations and order; the draws, the texture request and the
// layer push / pop are left out (the display list's packets for that
// frame differ, nothing else). Otherwise the recompiled 013F8668 runs.
// SOTC_LIFEBAR_SHORTCUT=0: always the recompiled one.
//
// SOTC_LIFEBAR_CHECK=1 (checking): when the shortcut applies, its writes
// are kept aside, the recompiled function runs as usual, and the two state
// blocks (0x1466b28..b80, 0x11f8520..8630) are compared with what the
// shortcut would have left; the first calls also list every other RAM
// range the recompiled function changed (outside its frame).
//
// 013F8958 tail-jumps (j) into 013F8668: the recompiler makes that a
// direct C++ call past the function table, so 8958 is here too (its own
// part as the recompiled code, then 8668's native, or the recompiled 8668
// when the table does not hold the native: a difftest check, SKIP).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace decomp_lifebar
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kFade = 0x013F8958u, kRing = 0x013F8668u;
    constexpr uint32_t kGripRatio = 0x013586D0u, kFlashSound = 0x013F67B8u;
    constexpr uint32_t kB30 = 0x01466B30u, kB3C = 0x01466B3Cu, kB40 = 0x01466B40u, kB44 = 0x01466B44u, kB50 = 0x01466B50u, kB60 = 0x01466B60u,
                       kB68 = 0x01466B68u, kB6C = 0x01466B6Cu;
    constexpr uint32_t kHistory = 0x011F8530u, kAlpha = 0x011F85B4u, kPhase = 0x011F8620u, kFlash1 = 0x011F8604u, kFlash2 = 0x011F861Cu;
    constexpr uint32_t kPi = 0x40490FDAu; // the code's 3.1415925 (lui 0x4049 / ori 0xfda)

    inline float bitsF(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t fBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline uint32_t ramWord(const uint8_t *rdram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
        return v;
    }

    bool shortcutOn()
    {
        static const bool on = !(std::getenv("SOTC_LIFEBAR_SHORTCUT") && std::atoi(std::getenv("SOTC_LIFEBAR_SHORTCUT")) == 0);
        return on;
    }
    bool checkOn()
    {
        static const bool on = std::getenv("SOTC_LIFEBAR_CHECK") != nullptr;
        return on;
    }

    // The shortcut's memory: words written straight, or (the check) kept in
    // a log that later reads see.
    struct Mem
    {
        uint8_t *rdram;
        bool logOnly;
        std::vector<std::pair<uint32_t, uint32_t>> log;
        uint32_t w(uint32_t a) const
        {
            for (auto it = log.rbegin(); it != log.rend(); ++it)
                if (it->first == a)
                    return it->second;
            return ramWord(rdram, a);
        }
        float f(uint32_t a) const { return bitsF(w(a)); }
        void put(uint32_t a, uint32_t v)
        {
            if (logOnly)
                log.emplace_back(a, v);
            else
                std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4);
        }
        void putF(uint32_t a, float v) { put(a, fBits(v)); }
    };

    // -(x - 1) / 6 + 1 between 1 and 7 (0 at 7 and over, 1 at 1 and under),
    // as the code has it three times.
    float scale(R5900Context *ctx, float x)
    {
        const float seven = bitsF(0x40E00000u), one = bitsF(0x3F800000u);
        if (FPU_C_OLE_S(seven, x))
            return bitsF(0u);
        if (FPU_C_OLE_S(x, one))
            return one;
        float t = FPU_SUB_S(x, one);
        t = FPU_NEG_S(t);
        t = ps2FpuDivide(ctx->fcr31, t, bitsF(0x40C00000u));
        return FPU_ADD_S(t, one);
    }

    // 8668's state with the draws left out (see the header); ratio: what
    // getPlayerMaxGripRatio returned.
    void ringState(R5900Context *ctx, Mem &m, float dt, float ratio)
    {
        const float zero = bitsF(0u), one = bitsF(0x3F800000u);
        m.putF(kB30, ratio);
        m.putF(kB3C, scale(ctx, ratio));
        m.putF(kB40, scale(ctx, FPU_MUL_S(m.f(kB50), m.f(kB30))));
        // The pulse: (0.5 - b50) * 2 above 0 advances the phase.
        float half = FPU_SUB_S(bitsF(0x3F000000u), m.f(kB50));
        const float p = FPU_ADD_S(half, half);
        if (!FPU_C_OLE_S(p, zero))
        {
            float f1 = FPU_SUB_S(one, p);
            float f3 = FPU_MUL_S(dt, bitsF(kPi));
            const float f2 = FPU_MUL_S(p, bitsF(0x40400000u));
            const float phase = m.f(kPhase);
            f1 = FPU_MUL_S(f1, bitsF(0x41200000u));
            f3 = FPU_MUL_S(f3, bitsF(0x41F00000u));
            f1 = FPU_ADD_S(f1, f2);
            const float step = ps2FpuDivide(ctx->fcr31, f3, f1);
            m.putF(kPhase, FPU_ADD_S(phase, step));
        }
        // 013F78A8: the shrink history.
        const float sixteenth = bitsF(0x3D800000u);
        bool scaled = true;
        if (FPU_C_OLT_S(zero, m.f(kB60)))
        {
            if (m.w(kB6C) != 0u)
            {
                m.put(kB6C, 0u);
                m.put(kB68, 0u);
            }
            for (uint32_t v = kHistory + 0x78u, n = 0; n < 15u; ++n, v -= 8u)
            {
                const float a = FPU_SUB_S(m.f(v - 4u), sixteenth);
                m.put(v, m.w(v - 8u));
                m.putF(v + 4u, a);
            }
            const int32_t count = static_cast<int32_t>(m.w(kB68)) + 1;
            m.put(kHistory, m.w(kB60));
            m.putF(kHistory + 4u, one);
            m.put(kB68, static_cast<uint32_t>(count));
            if (!(count < 17))
                m.put(kB68, 16u);
        }
        else
        {
            const int32_t count = static_cast<int32_t>(m.w(kB68));
            if (count <= 0)
            {
                m.put(kB6C, 0u);
                for (uint32_t k = 0; k < 16u; ++k)
                {
                    m.put(kHistory + k * 8u, 0u);
                    m.put(kHistory + k * 8u + 4u, 0u);
                }
                scaled = false; // returns before the points
            }
            else
            {
                m.put(kB68, static_cast<uint32_t>(count - 1));
                m.put(kB6C, 1u);
                if (count - 1 == 0)
                {
                    for (uint32_t k = 0; k < 16u; ++k)
                    {
                        m.put(kHistory + k * 8u, 0u);
                        m.put(kHistory + k * 8u + 4u, 0u);
                    }
                    m.put(kB6C, 0u);
                }
                else
                    for (uint32_t k = 0; k < 16u; ++k)
                        m.putF(kHistory + 4u + k * 8u, FPU_SUB_S(m.f(kHistory + 4u + k * 8u), sixteenth));
            }
        }
        if (scaled)
            m.putF(kB44, scale(ctx, m.f(kHistory)));
    }

    bool ringHidden(const uint8_t *rdram)
    {
        const float zero = bitsF(0u);
        return FPU_C_EQ_S(bitsF(ramWord(rdram, kAlpha)), zero) && !FPU_C_OLT_S(zero, bitsF(ramWord(rdram, kFlash1))) &&
               !FPU_C_OLT_S(zero, bitsF(ramWord(rdram, kFlash2)));
    }

    // The check: the shortcut's writes against what the recompiled code left.
    void check(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float dt)
    {
        static uint64_t calls = 0, bad = 0;
        static std::vector<uint8_t> before;
        const bool listOthers = calls < 4u;
        constexpr uint32_t kBlockA = 0x01466B28u, kBlockAEnd = 0x01466B80u, kBlockB = 0x011F8520u, kBlockBEnd = 0x011F8630u;
        const R5900Context input = *ctx;
        const uint32_t sp = GPR_U32(ctx, 29);
        Mem m{rdram, true, {}};
        sotc_decomp::callGuest(rdram, ctx, runtime, kGripRatio, 0x013F8690u, sp - 0x5C0u);
        ringState(ctx, m, dt, ctx->f[0]);
        std::vector<uint8_t> expectA(rdram + kBlockA, rdram + kBlockAEnd), expectB(rdram + kBlockB, rdram + kBlockBEnd);
        for (const auto &e : m.log)
        {
            if (e.first >= kBlockA && e.first + 4u <= kBlockAEnd)
                std::memcpy(expectA.data() + (e.first - kBlockA), &e.second, 4);
            else if (e.first >= kBlockB && e.first + 4u <= kBlockBEnd)
                std::memcpy(expectB.data() + (e.first - kBlockB), &e.second, 4);
        }
        if (listOthers)
            before.assign(rdram, rdram + PS2_RAM_SIZE);
        *ctx = input;
        sotc_decomp::callRecompiled(rdram, ctx, runtime, kRing, GPR_U32(&input, 31), sp);
        ++calls;
        const bool same = std::memcmp(expectA.data(), rdram + kBlockA, expectA.size()) == 0 && std::memcmp(expectB.data(), rdram + kBlockB, expectB.size()) == 0;
        if (!same)
        {
            ++bad;
            for (uint32_t a = kBlockA; a < kBlockAEnd; a += 4u)
                if (std::memcmp(expectA.data() + (a - kBlockA), rdram + a, 4) != 0 && bad < 10u)
                    std::fprintf(stderr, "[lifebar-check] %08x: shortcut %08x recompiled %08x\n", a, ramWord(expectA.data(), a - kBlockA), ramWord(rdram, a));
            for (uint32_t a = kBlockB; a < kBlockBEnd; a += 4u)
                if (std::memcmp(expectB.data() + (a - kBlockB), rdram + a, 4) != 0 && bad < 10u)
                    std::fprintf(stderr, "[lifebar-check] %08x: shortcut %08x recompiled %08x\n", a, ramWord(expectB.data(), a - kBlockB), ramWord(rdram, a));
        }
        if (listOthers)
        {
            // Every other range the recompiled code wrote (its frame and the
            // two blocks left out).
            const uint32_t frameLo = (sp - 0x800u) & PS2_RAM_MASK, frameHi = sp & PS2_RAM_MASK;
            for (uint32_t a = 0; a < PS2_RAM_SIZE;)
            {
                if (before[a] == rdram[a] || (a >= frameLo && a < frameHi) || (a >= kBlockA && a < kBlockAEnd) || (a >= kBlockB && a < kBlockBEnd))
                {
                    ++a;
                    continue;
                }
                uint32_t end = a + 1u;
                while (end < PS2_RAM_SIZE && (end - a) < 0x100000u)
                {
                    uint32_t gap = 0;
                    while (end + gap < PS2_RAM_SIZE && gap < 64u && before[end + gap] == rdram[end + gap])
                        ++gap;
                    if (gap >= 64u || end + gap >= PS2_RAM_SIZE)
                        break;
                    end += gap + 1u;
                }
                std::fprintf(stderr, "[lifebar-check] call %llu: also written %08x..%08x", (unsigned long long)calls, a, end);
                if (end - a <= 32u)
                    for (uint32_t k = a & ~3u; k < end; k += 4u)
                        std::fprintf(stderr, " %08x:%08x->%08x", k, ramWord(before.data(), k), ramWord(rdram, k));
                std::fprintf(stderr, "\n");
                a = end;
            }
        }
        if (calls == 1u || calls % 200u == 0u || !same)
            std::fprintf(stderr, "[lifebar-check] %llu hidden calls checked, %llu differ\n", (unsigned long long)calls, (unsigned long long)bad);
    }

    void ring(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = kRing;
        if (!shortcutOn() || !ringHidden(rdram))
        {
            if (checkOn())
            {
                static uint64_t shown = 0;
                if (++shown % 30u == 1u)
                    std::fprintf(stderr, "[lifebar-check] ring shown (recompiled): %llu calls, alpha %08x\n", (unsigned long long)shown, ramWord(rdram, kAlpha));
            }
            return sotc_decomp::recompiled(kRing)(rdram, ctx, runtime);
        }
        const float dt = ctx->f[12];
        if (checkOn())
            return check(rdram, ctx, runtime, dt);
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        sotc_decomp::callGuest(rdram, ctx, runtime, kGripRatio, 0x013F8690u, sp - 0x5C0u);
        Mem m{rdram, false, {}};
        ringState(ctx, m, dt, ctx->f[0]);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    // 013F8958: the two flash fades (a sound and a sine pulse while each
    // runs), then 8668.
    void fade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp = GPR_U32(ctx, 29), frame = sp - 0x30u, ra = GPR_U32(ctx, 31);
        const float dt = ctx->f[12], zero = bitsF(0u);
        const uint32_t now = 0x014657C8u;
        auto flash = [&](uint32_t entry, uint32_t soundSite, uint32_t sinSite)
        {
            float out = zero;
            if (ramWord(rdram, entry) == 0u)
                return out;
            const float t = FPU_SUB_S(bitsF(ramWord(rdram, now)), bitsF(ramWord(rdram, entry + 0x10u)));
            if (FPU_C_OLE_S(bitsF(ramWord(rdram, entry + 4u)), t))
            {
                const uint32_t z = 0u;
                std::memcpy(rdram + entry, &z, 4);
                return out;
            }
            SET_GPR_S32(ctx, 4, 4);
            sotc_decomp::callGuest(rdram, ctx, runtime, kFlashSound, soundSite + 8u, frame);
            float a = FPU_MUL_S(t, bitsF(kPi));
            a = ps2FpuDivide(ctx->fcr31, a, bitsF(ramWord(rdram, entry + 4u)));
            ctx->f[12] = a;
            sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, sinSite), sinSite + 8u, frame);
            out = FPU_MUL_S(bitsF(ramWord(rdram, entry + 8u)), ctx->f[0]);
            return out;
        };
        const float first = flash(0x011F85F0u, 0x013F89B8u, 0x013F89E0u);
        {
            const uint32_t b = fBits(first);
            std::memcpy(rdram + kFlash1, &b, 4);
        }
        const float second = flash(0x011F8608u, 0x013F8A38u, 0x013F8A60u);
        {
            const uint32_t b = fBits(second);
            std::memcpy(rdram + kFlash2, &b, 4);
        }
        // As the original leaves them at its j: the scratch registers it set.
        SET_GPR_S32(ctx, 2, 0x011F8608);
        SET_GPR_S32(ctx, 3, 0x011F85F0);
        ctx->f[3] = second;
        ctx->f[4] = first;
        ctx->f[12] = dt;
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        ctx->pc = kRing;
        if (runtime->guestFunctionAt(kRing) == &ring)
            return ring(rdram, ctx, runtime);
        // Through the table (a check of 8668, SOTC_DECOMP_SKIP): ra / sp as the j leaves them.
        sotc_decomp::callGuest(rdram, ctx, runtime, kRing, ra, sp);
    }

    SOTC_DECOMP_FUNCTION(kFade, gamecore_sub_013F8958, fade);
    SOTC_DECOMP_FUNCTION(kRing, gamecore_sub_013F8668, ring);
}
