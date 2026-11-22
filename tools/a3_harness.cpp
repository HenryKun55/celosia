// a3_harness.cpp: round A3's offline equivalence harness (docs/decomp_a3.md, the plan's T5).
//
// Every A3 typed function (game/overrides/a3_typed_check.cpp's table; its callees' references and typed copies with it) against its lifted text on random states, without
// the game: a 32 MB main RAM of random bytes, registers drawn from small integers, main-RAM addresses (aligned and
// not), random 64-bit values, and floats (ordinary, tiny, huge, zero, negative zero, denormal, Inf / NaN patterns).
// Both legs run on logged memory (typed_a3_check_rt.h: a per-byte overlay, the stores compared as a hash and a count);
// an address outside main RAM reads bytes derived from the address (the same for both legs). The check is the in-run
// one: return value, extra outputs, stores, ra / sp, undeclared slots. Stores below the entry sp (the dead stack: the
// reference's frames) are left out. A leg past 2^16 back edges (a loop count drawn at random) gives the check up
// ("capped"); a leg whose overlay fills is not compared ("unverified"). Both are reported, never counted as equal.
//
// Modes (environment): A3H_FINITE=1 (no Inf / NaN float input, see g_finite), A3H_DN=1 (default-NaN mode on),
// A3H_TRACE=<name> (that function's loads / stores per leg to stderr). Build and run (Mac, host clang;
// -ffp-contract=off: the Vita's Cortex-A9 (VFPv3) has no fused multiply-add, so the Mac must not fuse either):
//   clang++ -std=c++20 -O1 -ffp-contract=off -DA3_OFFLINE -DUSE_SSE2NEON -I <sse2neon> -I PS2Recomp/ps2xRuntime/include -I PS2Recomp/ps2xIOP/include \
//       -I game/overrides tools/a3_harness.cpp game/overrides/a3_typed_check.cpp -o build/a3_harness && build/a3_harness [trials] [seed]
#include "typed_a3_check_rt.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

// what the runtime's headers reference and the harness does not link
bool g_ps2xFpuZeroFast = false;
bool g_ps2FpuSetcsrNearest = false;
bool g_a1Typed = true;
bool g_a2Typed = true;
bool g_a3Typed = true;
bool g_ps2DirectCalls = true;

namespace
{
    // A3H_FINITE=1: no float input with an all-ones exponent (Inf / NaN): FPR inputs and the GPRs' low words get exponent
    // 254 instead, and the RAM image and the slow path have no byte 0x7F / 0xFF (0x7E / 0xFE instead), so no 32-bit
    // window at any alignment holds an all-ones exponent (the same draws otherwise). The PS2 FPU has no Inf / NaN (an all-ones
    // exponent is an ordinary huge number there) and the game's float data never holds one; IEEE leaves a NaN result's
    // sign to the implementation, and clang picks operand order / where a negation goes per inlining context, so an
    // Inf / NaN input can give the two legs (the same C++ semantics) NaNs of different sign, clamped to +-FLT_MAX.
    bool g_finite = false;
    uint32_t fin32(uint32_t b) { return (g_finite && ((b >> 23) & 0xFFu) == 0xFFu) ? (b ^ 0x00800000u) : b; }
    uint64_t fin64(uint64_t v)
    {
        if (!g_finite)
            return v;
        for (int k = 0; k < 8; ++k)
            if (((v >> (8 * k)) & 0x7Fu) == 0x7Fu)
                v ^= uint64_t(1) << (8 * k);
        return v;
    }
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
        return fin64(v);
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
        b = fin32(b);
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
}

int main(int argc, char **argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    if (const char *e = std::getenv("A3H_FINITE"); e && *e == '1')
        g_finite = true;
    const int trials = argc > 1 ? std::atoi(argv[1]) : 200;
    g_rng ^= argc > 2 ? static_cast<uint64_t>(std::atoll(argv[2])) * 0x2545F4914F6CDD1Dull : 0u;
    ps2SetEeFloatingPointMode(); // the game thread's FPU mode (round toward zero, flush to zero)
#if defined(__aarch64__)
    // A3H_DN=1: the FPU's default-NaN mode (FPCR.DN) on, so that an operation on two NaNs gives the same NaN whatever
    // operand order the compiler picked for a commutative add / multiply (off by default: the host's own mode)
    if (const char *dn = std::getenv("A3H_DN"); dn && *dn == '1')
    {
        uint64_t fpcr;
        __asm__ volatile("mrs %0, fpcr" : "=r"(fpcr));
        fpcr |= uint64_t(1) << 25;
        __asm__ volatile("msr fpcr, %0" : : "r"(fpcr));
    }
#endif
    // two RAM images, alternated by trial: random bytes, and small words (0-15, +-1.0f, main-RAM pointers) so that the
    // comparisons against small constants and pointer chases take both ways
    std::vector<uint8_t> ram(PS2_RAM_SIZE + 64), ram2(PS2_RAM_SIZE + 64);
    for (size_t i = 0; i < ram.size(); i += 8)
    {
        const uint64_t v = fin64(rnd());
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
    for (uint32_t i = 0; i < a3ck::kEntryCount; ++i)
    {
        const uint64_t before = a3ck::g_stats.differ;
        for (int t = 0; t < trials; ++t)
        {
            std::memset(static_cast<void *>(ctx), 0, sizeof(R5900Context));
            for (int r = 1; r < 32; ++r)
            {
                const uint64_t lo0 = gprValue(), hi = rnd();
                const uint64_t lo = (lo0 & ~uint64_t(0xFFFFFFFFu)) | fin32(static_cast<uint32_t>(lo0));
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
            ctx->pc = a3ck::kEntries[i].addr;
            a3ck::kEntries[i].run((t & 1) ? ram2.data() : ram.data(), ctx, nullptr);
        }
        if (a3ck::g_stats.differ != before)
        {
            ++bad;
            std::printf("DIFFER %08x %s: %llu of %d\n", a3ck::kEntries[i].addr, a3ck::kEntries[i].name,
                        (unsigned long long)(a3ck::g_stats.differ - before), trials);
        }
    }
    const a1ck::CkStats &g = a3ck::g_stats;
    std::printf("a3_harness: %u functions x %d random states: %llu checks, %llu differ (ret %llu mem %llu pc %llu sp %llu other %llu; "
                "extra outputs %llu) in %u functions; capped %llu, unverified %llu, compared %llu; slow-path loads %llu; back edges ref %llu typed %llu; "
                "scratch slots changed by the reference %llu%s%s\n",
                a3ck::kEntryCount, trials, (unsigned long long)g.calls, (unsigned long long)g.differ, (unsigned long long)g.ret,
                (unsigned long long)g.mem, (unsigned long long)g.pc, (unsigned long long)g.sp, (unsigned long long)g.other,
                (unsigned long long)a3ck::g_xo, bad, (unsigned long long)a3ck::g_capped, (unsigned long long)a3ck::g_unverified,
                (unsigned long long)(g.calls - a3ck::g_capped - a3ck::g_unverified), (unsigned long long)g.io,
                (unsigned long long)a3ck::g_backs[0], (unsigned long long)a3ck::g_backs[1], (unsigned long long)g.droppedChanged,
                g.firstId >= 0 ? "; first: " : "", g.first);
    return g.differ ? 1 : 0;
}
