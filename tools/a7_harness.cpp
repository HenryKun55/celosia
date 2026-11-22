// a7_harness.cpp: round A7's offline equivalence harness (docs/decomp_a7.md, the plan's T5).
//
// Every A7 typed function (game/overrides/a7_typed_check.cpp's table; its callees' references and typed copies with it) against its lifted text on random states, without
// the game: a 32 MB main RAM of random bytes, registers drawn from small integers, main-RAM addresses (aligned and
// not), random 64-bit values, and floats (ordinary, tiny, huge, zero, negative zero, denormal, Inf / NaN patterns).
// Both legs run on logged memory (typed_a7_check_rt.h: a per-byte overlay, the stores compared as a hash and a count);
// an address outside main RAM reads bytes derived from the address (the same for both legs). The check is the in-run
// one: return value, extra outputs, stores, ra / sp, undeclared slots, and the VU0 state (every VU0 register, ACC, Q,
// status, every GPR's upper half: drawn at random too, vf0 included). Stores below the entry sp (the dead stack: the
// reference's frames) are left out. A leg past 2^16 back edges (a loop count drawn at random) gives the check up
// ("capped"); a leg whose overlay fills is not compared ("unverified"). Both are reported, never counted as equal.
// A5: the relocated sites' jal / j words (a7_typed_check.cpp's kRelocs: the static targets the adapters' A5_JAL tests)
// are written into both RAM images, as the game's code is in RAM; the guest frames live below the drawn sp (the dead
// stack, left out of the store comparison as the reference's frames are); a jump table's random out-of-list value gives
// the check up ("capped", counted apart as bad jumps). A7H_HI0=1: the GPRs' upper halves drawn as 0 (as the game has
// them almost always): the A1-A3 callees' movz $zero upper halves (counted apart, see vuDifferMask) then agree.
// A7H_ELF=<SOTC.elf>: the game's code and data segments over the random images (jump tables, constants, code words).
//
// Modes (environment): A7H_FINITE=1 (no Inf / NaN float input, see g_finite), A7H_DN=1 (default-NaN mode on),
// A7H_TRACE=<name> (that function's loads / stores per leg to stderr). Build and run (Mac, host clang;
// -ffp-contract=off: the Vita's Cortex-A9 (VFPv3) has no fused multiply-add, so the Mac must not fuse either):
//   clang++ -std=c++20 -O1 -ffp-contract=off -DA3_OFFLINE -DA4_OFFLINE -DA5_OFFLINE -DA7_OFFLINE -DUSE_SSE2NEON -I <sse2neon> -I PS2Recomp/ps2xRuntime/include -I PS2Recomp/ps2xIOP/include \
//       -I game/overrides tools/a7_harness.cpp game/overrides/a7_typed_check.cpp -o build/a7_harness && build/a7_harness [trials] [seed]
#include "typed_a7_check_rt.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

// what the runtime's headers reference and the harness does not link
bool g_ps2xFpuZeroFast = false;
bool g_ps2FpuSetcsrNearest = false;
bool g_a1Typed = true;
bool g_a2Typed = true;
bool g_a3Typed = true;
bool g_a4Typed = true;
bool g_a5Typed = true;
bool g_a6Typed = true;
bool g_a7Typed = true;
A6Brk g_a6Brk = {0u, 0u, 0u};
bool g_ps2DirectCalls = true;
// A6: a jump table's random out-of-list value gives the check up (typed_a5_rt.h's offline A5_BADJUMP), counted as a bad jump
namespace a5ck
{
    uint64_t g_badJumps = 0;
    void legBadJump()
    {
        ++g_badJumps;
        throw a7ck::Capped{};
    }
}

namespace
{
    // A7H_FINITE=1: no float input with an all-ones exponent (Inf / NaN): FPR inputs and the GPRs' low words get exponent
    // 254 instead, and the RAM image and the slow path have no byte 0x7F / 0xFF (0x7E / 0xFE instead), so no 32-bit
    // window at any alignment holds an all-ones exponent (the same draws otherwise). The PS2 FPU has no Inf / NaN (an all-ones
    // exponent is an ordinary huge number there) and the game's float data never holds one; IEEE leaves a NaN result's
    // sign to the implementation, and clang picks operand order / where a negation goes per inlining context, so an
    // Inf / NaN input can give the two legs (the same C++ semantics) NaNs of different sign, clamped to +-FLT_MAX.
    bool g_finite = false;
    bool g_hi0 = false; // A7H_HI0=1
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
    // A5: drawn main-RAM pointers stay below the stack region: the entry sp is drawn in [0x1EF0010, 0x1F00000] and the
    // dead stack below it ([sp - 0x40000, sp): the lifted text's own frames, left out of the store comparison) is not a
    // valid place for an argument to point (a pointer there aliases the reference's save slots, which the typed function
    // keeps in registers: the first 500-state run hit one such draw, a1 = sp - 0x38, and the restored s2 / s3 differed)
    constexpr uint32_t kPtrSpan = 0x1D80000u; // pointers in [0x100000, 0x1E80000)
    uint64_t gprValue()
    {
        switch (rnd() % 8u)
        {
        case 0: return rnd() % 16u;
        case 1: return static_cast<uint64_t>(-static_cast<int64_t>(rnd() % 16u));
        case 2: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % kPtrSpan)) & ~3u);
        case 3: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % kPtrSpan)) & ~15u);
        case 4: return a1_sx64(static_cast<uint32_t>(0x100000u + (rnd() % kPtrSpan)));
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
    if (const char *e = std::getenv("A7H_FINITE"); e && *e == '1')
        g_finite = true;
    if (const char *e = std::getenv("A7H_HI0"); e && *e == '1')
        g_hi0 = true;
    const int trials = argc > 1 ? std::atoi(argv[1]) : 200;
    g_rng ^= argc > 2 ? static_cast<uint64_t>(std::atoll(argv[2])) * 0x2545F4914F6CDD1Dull : 0u;
    ps2SetEeFloatingPointMode(); // the game thread's FPU mode (round toward zero, flush to zero)
#if defined(__aarch64__)
    // A7H_DN=1: the FPU's default-NaN mode (FPCR.DN) on, so that an operation on two NaNs gives the same NaN whatever
    // operand order the compiler picked for a commutative add / multiply (off by default: the host's own mode)
    if (const char *dn = std::getenv("A7H_DN"); dn && *dn == '1')
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
            v = static_cast<uint32_t>(0x100000u + ((r >> 20) % kPtrSpan)) & ~15u;
        std::memcpy(&ram2[i], &v, 4);
    }
    // A7H_ELF=<SOTC.elf> (build/recomp/game/SOTC.elf: the boot ELF and the XFF modules linked at their load addresses):
    // its loadable segments over both RAM images, so jump tables, constants and code words are the game's
    if (const char *ep = std::getenv("A7H_ELF"); ep && *ep)
    {
        FILE *f = std::fopen(ep, "rb");
        if (!f)
        {
            std::printf("a7_harness: cannot open %s\n", ep);
            return 2;
        }
        std::vector<uint8_t> elf;
        uint8_t buf[65536];
        size_t n;
        while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0)
            elf.insert(elf.end(), buf, buf + n);
        std::fclose(f);
        uint32_t phoff;
        uint16_t phentsize, phnum;
        std::memcpy(&phoff, &elf[28], 4);
        std::memcpy(&phentsize, &elf[42], 2);
        std::memcpy(&phnum, &elf[44], 2);
        uint32_t loaded = 0;
        for (uint32_t i = 0; i < phnum; ++i)
        {
            uint32_t ph[8];
            std::memcpy(ph, &elf[phoff + i * phentsize], 32);
            if (ph[0] != 1u || ph[2] + ph[4] > PS2_RAM_SIZE || ph[1] + ph[4] > elf.size())
                continue;
            for (uint32_t k = 0; k < ph[4]; ++k)
            {
                uint8_t b = elf[ph[1] + k];
                if (g_finite && (b & 0x7Fu) == 0x7Fu) // A7H_FINITE: no all-ones exponent anywhere (as the random image)
                    b ^= 1u;
                ram[ph[2] + k] = b;
                ram2[ph[2] + k] = b;
            }
            loaded += ph[4];
        }
        std::printf("a7_harness: %s: %u bytes loaded\n", ep, loaded);
    }
    for (uint32_t k = 0; k < a7ck::kRelocCount; ++k)
    {
        std::memcpy(&ram[a7ck::kRelocs[k][0]], &a7ck::kRelocs[k][1], 4);
        std::memcpy(&ram2[a7ck::kRelocs[k][0]], &a7ck::kRelocs[k][1], 4);
    }
    a1ck::g_slowRead = &slowRead;
    void *m = std::malloc(sizeof(R5900Context) + 16);
    R5900Context *ctx = reinterpret_cast<R5900Context *>((reinterpret_cast<uintptr_t>(m) + 15u) & ~uintptr_t(15));
    uint32_t bad = 0;
    const char *only = std::getenv("A7H_ONLY"); // A7: one function by name (investigating)
    for (uint32_t i = 0; i < a7ck::kEntryCount; ++i)
    {
        if (only && *only && std::strcmp(only, a7ck::kEntries[i].name) != 0)
            continue;
        const uint64_t before = a7ck::g_stats.differ;
        for (int t = 0; t < trials; ++t)
        {
            std::memset(static_cast<void *>(ctx), 0, sizeof(R5900Context));
            for (int r = 1; r < 32; ++r)
            {
                const uint64_t lo0 = gprValue(), hi = g_hi0 ? 0u : rnd();
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
            // A4: the VU0 state (lanes drawn as the FPRs are), Q, the status (its sticky bits and flags)
            for (int r = 0; r < 32; ++r)
            {
                float l[4] = {fprValue(), fprValue(), fprValue(), fprValue()};
                std::memcpy(&ctx->vu0_vf[r], l, 16);
            }
            {
                float l[4] = {fprValue(), fprValue(), fprValue(), fprValue()};
                std::memcpy(&ctx->vu0_acc, l, 16);
            }
            ctx->vu0_q = fprValue();
            ctx->vu0_status = static_cast<uint16_t>(rnd() & 0x0FFFu);
            ctx->pc = a7ck::kEntries[i].addr;
            a7ck::kEntries[i].run((t & 1) ? ram2.data() : ram.data(), ctx, nullptr);
        }
        if (a7ck::g_stats.differ != before)
        {
            ++bad;
            std::printf("DIFFER %08x %s: %llu of %d\n", a7ck::kEntries[i].addr, a7ck::kEntries[i].name,
                        (unsigned long long)(a7ck::g_stats.differ - before), trials);
        }
    }
    const a1ck::CkStats &g = a7ck::g_stats;
    std::printf("a7_harness: %u functions x %d random states: %llu checks, %llu differ (ret %llu mem %llu pc %llu sp %llu other %llu; "
                "extra outputs %llu, VU0 state %llu) in %u functions; capped %llu, unverified %llu, compared %llu; slow-path loads %llu; back edges ref %llu typed %llu; "
                "scratch slots changed by the reference %llu; bad jumps (capped) %llu, relocated sites %u; breaks typed %llu, break differences %llu, typed stub calls %llu (given up %llu); upper halves outside the interface that differ %llu; guest-frame save slots stored into by the reference (given up) %llu%s%s\n",
                a7ck::kEntryCount, trials, (unsigned long long)g.calls, (unsigned long long)g.differ, (unsigned long long)g.ret,
                (unsigned long long)g.mem, (unsigned long long)g.pc, (unsigned long long)g.sp, (unsigned long long)g.other,
                (unsigned long long)a7ck::g_xo, (unsigned long long)a7ck::g_vu, bad, (unsigned long long)a7ck::g_capped, (unsigned long long)a7ck::g_unverified,
                (unsigned long long)(g.calls - a7ck::g_capped - a7ck::g_unverified), (unsigned long long)g.io,
                (unsigned long long)a7ck::g_backs[0], (unsigned long long)a7ck::g_backs[1], (unsigned long long)g.droppedChanged,
                (unsigned long long)a5ck::g_badJumps, a7ck::kRelocCount, (unsigned long long)a7ck::g_brks, (unsigned long long)a7ck::g_brkDiffer, (unsigned long long)a7ck::g_extCalls, (unsigned long long)a7ck::g_extGiveUp, (unsigned long long)a7ck::g_hOut, (unsigned long long)a7ck::g_saveClob, g.firstId >= 0 ? "; first: " : "", g.first);
    return g.differ ? 1 : 0;
}
