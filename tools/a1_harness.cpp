// a1_harness.cpp: round A1's offline equivalence harness (docs/decomp_a1.md section 4, the plan's T5).
//
// Every typed function (game/overrides/a1_typed_check.cpp's table) against its lifted text on random states, without
// the game: a 32 MB main RAM of random bytes, registers drawn from small integers, main-RAM addresses (aligned and
// not), random 64-bit values, and floats (ordinary, tiny, huge, zero, negative zero, denormal, Inf / NaN patterns).
// Both legs run on logged memory (typed_a1_check_rt.h); an address outside main RAM reads bytes derived from the
// address (the same for both legs). The check is the in-run one: return value, stores, ra / sp, undeclared slots.
//
// Build and run (Mac, host clang; tools/a1_harness.sh):
//   clang++ -std=c++20 -O1 -DA1_OFFLINE -DPS2X_A1_CHECK=0 -I PS2Recomp/ps2xRuntime/include -I game/overrides \
//       tools/a1_harness.cpp game/overrides/a1_typed_check.cpp -o build/a1_harness && build/a1_harness [trials] [seed]
#include "typed_a1_check_rt.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

// what the runtime's headers reference and the harness does not link
bool g_ps2xFpuZeroFast = false;
bool g_ps2FpuSetcsrNearest = false;
bool g_a1Typed = true;

namespace
{
    uint64_t g_rng = 0x9E3779B97F4A7C15ull;
    uint64_t rnd()
    {
        g_rng ^= g_rng << 13;
        g_rng ^= g_rng >> 7;
        g_rng ^= g_rng << 17;
        return g_rng;
    }
    uint64_t hashAddr(uint32_t a)
    {
        uint64_t x = a * 0x9E3779B97F4A7C15ull;
        x ^= x >> 29;
        x *= 0xBF58476D1CE4E5B9ull;
        return x ^ (x >> 32);
    }
    uint64_t slowRead(uint8_t *, PS2Runtime *, uint32_t a, uint32_t n)
    {
        uint64_t v = 0;
        for (uint32_t i = 0; i < n; ++i)
            v |= (hashAddr(a + i) & 0xFFu) << (8u * i);
        return v;
    }
    uint64_t gprValue()
    {
        switch (rnd() % 8u)
        {
        case 0: return rnd() % 16u;
        case 1: return static_cast<uint64_t>(-static_cast<int64_t>(rnd() % 16u));
        case 2: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % 0x1E00000u)) & ~3u);
        case 3: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % 0x1E00000u)) & ~15u);
        case 4: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % 0x1E00000u)));
        case 5: return a1_sx64(static_cast<uint32_t>(rnd()));
        case 6: return rnd();
        default: return rnd() % 4096u;
        }
    }
    float fprValue()
    {
        static const uint32_t special[] = {0x00000000u, 0x80000000u, 0x3F800000u, 0xBF800000u, 0x7F7FFFFFu, 0xFF7FFFFFu, 0x00000001u,
                                           0x807FFFFFu, 0x7F800000u, 0xFF800000u, 0x7FC00000u, 0x00800000u, 0x34000000u, 0x4B000000u};
        uint32_t b;
        switch (rnd() % 6u)
        {
        case 0: b = special[rnd() % (sizeof(special) / sizeof(special[0]))]; break;
        case 1: b = static_cast<uint32_t>(rnd()); break;
        default:
        {
            const float f = static_cast<float>(static_cast<int64_t>(rnd() % 2000001u) - 1000000) / static_cast<float>(1u + rnd() % 1000u);
            std::memcpy(&b, &f, 4);
        }
        }
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
}

int main(int argc, char **argv)
{
    const int trials = argc > 1 ? std::atoi(argv[1]) : 200;
    g_rng ^= argc > 2 ? static_cast<uint64_t>(std::atoll(argv[2])) * 0x2545F4914F6CDD1Dull : 0u;
    ps2SetEeFloatingPointMode(); // the game thread's FPU mode (round toward zero, flush to zero)
    // two RAM images, alternated by trial: random bytes, and small words (0-15, +-1.0f, main-RAM pointers) so that the
    // comparisons against small constants and pointer chases take both ways
    std::vector<uint8_t> ram(PS2_RAM_SIZE + 64), ram2(PS2_RAM_SIZE + 64);
    for (size_t i = 0; i < ram.size(); i += 8)
    {
        const uint64_t v = rnd();
        std::memcpy(&ram[i], &v, 8);
    }
    for (size_t i = 0; i < ram2.size(); i += 4)
    {
        const uint64_t r = rnd();
        uint32_t v = static_cast<uint32_t>(r % 16u);
        if ((r >> 8) % 4u == 0u)
            v = (r >> 16) % 2u ? 0x3F800000u : 0xBF800000u;
        else if ((r >> 8) % 4u == 1u)
            v = static_cast<uint32_t>(0x100000u + ((r >> 20) % 0x1E00000u)) & ~15u;
        std::memcpy(&ram2[i], &v, 4);
    }
    a1ck::g_slowRead = &slowRead;
    void *m = std::malloc(sizeof(R5900Context) + 16);
    R5900Context *ctx = reinterpret_cast<R5900Context *>((reinterpret_cast<uintptr_t>(m) + 15u) & ~uintptr_t(15));
    uint32_t bad = 0;
    for (uint32_t i = 0; i < a1ck::kEntryCount; ++i)
    {
        const uint64_t before = a1ck::g_stats.differ;
        for (int t = 0; t < trials; ++t)
        {
            std::memset(static_cast<void *>(ctx), 0, sizeof(R5900Context));
            for (int r = 1; r < 32; ++r)
            {
                const uint64_t lo = gprValue(), hi = rnd();
                std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[r]), &lo, 8);
                std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[r]) + 8, &hi, 8);
            }
            const uint64_t sp = 0x1F00000u - 16u * (rnd() % 4096u);
            std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[29]), &sp, 8);
            for (int r = 0; r < 32; ++r)
                ctx->f[r] = fprValue();
            ctx->hi = rnd();
            ctx->lo = rnd();
            ctx->fcr31 = static_cast<uint32_t>(rnd()) & 0x0083F07Fu;
            ctx->f_acc = fprValue();
            ctx->pc = a1ck::kEntries[i].addr;
            a1ck::kEntries[i].run((t & 1) ? ram2.data() : ram.data(), ctx, nullptr);
        }
        if (a1ck::g_stats.differ != before)
        {
            ++bad;
            std::printf("DIFFER %08x %s: %llu of %d\n", a1ck::kEntries[i].addr, a1ck::kEntries[i].name,
                        (unsigned long long)(a1ck::g_stats.differ - before), trials);
        }
    }
    const a1ck::CkStats &g = a1ck::g_stats;
    std::printf("a1_harness: %u functions x %d random states: %llu checks, %llu differ (ret %llu mem %llu pc %llu sp %llu other %llu) in %u "
                "functions; slow-path loads %llu, log overflow %llu; scratch slots changed by the reference %llu%s%s\n",
                a1ck::kEntryCount, trials, (unsigned long long)g.calls, (unsigned long long)g.differ, (unsigned long long)g.ret,
                (unsigned long long)g.mem, (unsigned long long)g.pc, (unsigned long long)g.sp, (unsigned long long)g.other, bad,
                (unsigned long long)g.io, (unsigned long long)g.ovf, (unsigned long long)g.droppedChanged, g.firstId >= 0 ? "; first: " : "",
                g.first);
    return g.differ ? 1 : 0;
}
