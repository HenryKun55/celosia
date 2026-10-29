// The generated code's VU0 / FPU macros (ps2_runtime_macros.h) against their
// old definitions, bit for bit, on the machine that runs the game: the
// ARMv7 forms rely on how the Vita's NEON treats denormals, so they are
// checked there (PS2X_MACRO_SELFTEST=<millions>[x], on the game thread,
// before the game starts; 'x' adds every 32-bit pattern through the
// normalisation). Results and a small timing of old / new go to stderr as
// "[selftest]" lines. Also built on the host by a standalone test.
#include "ps2_runtime_macros.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace ps2_macro_selftest
{
    static double nowSeconds()
    {
#if defined(__vita__)
        return static_cast<double>(sceKernelGetProcessTimeWide()) * 1e-6;
#else
        return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
    }

    struct Rng
    {
        uint64_t s;
        uint32_t next()
        {
            s ^= s >> 12;
            s ^= s << 25;
            s ^= s >> 27;
            return static_cast<uint32_t>((s * 0x2545F4914F6CDD1Dull) >> 32);
        }
    };

    static uint32_t bitsOf(__m128 v, int lane)
    {
        alignas(16) uint32_t b[4];
        std::memcpy(b, &v, 16);
        return b[lane];
    }
    static __m128 fromBits(const uint32_t *b)
    {
        __m128 v;
        std::memcpy(&v, b, 16);
        return v;
    }
    static bool same(__m128 x, __m128 y) { return std::memcmp(&x, &y, 16) == 0; }

    // Edge patterns: every exponent with mantissas 0, 1, half, all ones, both
    // signs, and the values around the denormal / overflow / NaN borders.
    static int edgeValues(uint32_t *out)
    {
        int n = 0;
        static const uint32_t mant[] = {0u, 1u, 2u, 0x400000u, 0x7FFFFEu, 0x7FFFFFu, 0x2AAAAAu, 0x555555u};
        for (uint32_t sgn = 0; sgn < 2; ++sgn)
            for (uint32_t e = 0; e < 256; ++e)
                for (uint32_t m : mant)
                    out[n++] = (sgn << 31) | (e << 23) | m;
        return n;
    }

    // An operand pattern: uniform bits, an edge exponent, or near another
    // exponent (alignment / cancellation), or so that a product is at the
    // denormal or the overflow border.
    static uint32_t pick(Rng &r, uint32_t other)
    {
        const uint32_t k = r.next();
        const uint32_t m = r.next() & 0x7FFFFFu;
        const uint32_t sgn = r.next() & 0x80000000u;
        const uint32_t eo = (other >> 23) & 0xFFu;
        uint32_t e;
        switch (k & 7u)
        {
        case 0:
        case 1:
            return r.next();
        case 2:
        {
            static const uint8_t ex[] = {0, 1, 2, 3, 23, 24, 25, 26, 102, 103, 126, 127, 128, 152, 153, 228, 229, 252, 253, 254, 255};
            e = ex[(k >> 3) % sizeof(ex)];
            break;
        }
        case 3:
        case 4:
            e = static_cast<uint32_t>(static_cast<int32_t>(eo) + static_cast<int32_t>((k >> 3) % 61u) - 30) & 0xFFu;
            break;
        case 5:
            e = (254u + 127u - eo + ((k >> 3) % 5u) - 2u) & 0xFFu; // product near FLT_MAX
            break;
        case 6:
            e = (127u + 1u - eo + ((k >> 3) % 5u) - 2u) & 0xFFu; // product near the smallest normal
            break;
        default:
            e = 100u + ((k >> 3) % 56u);
            break;
        }
        return sgn | (e << 23) | m;
    }

    struct Counter
    {
        const char *name;
        uint64_t checked = 0;
        uint64_t bad = 0;
        uint64_t nanOrder = 0;
        void report(uint32_t a, uint32_t b, uint32_t want, uint32_t got)
        {
            if (bad < 8)
                std::fprintf(stderr, "[selftest] %s MISMATCH a=%08x b=%08x old=%08x new=%08x\n", name, a, b, want, got);
            ++bad;
        }
        void print() const
        {
            std::fprintf(stderr, "[selftest] %-22s %14llu checked, %llu mismatches", name, (unsigned long long)checked, (unsigned long long)bad);
            if (nanOrder)
                std::fprintf(stderr, " (%llu NaN + NaN of two signs: the other NaN's sign, see checkFpu)", (unsigned long long)nanOrder);
            std::fprintf(stderr, "\n");
        }
    };

    using VuOp = __m128 (*)(__m128, __m128);
    static __m128 addNew(__m128 a, __m128 b) { return PS2_VADD(a, b); }
    static __m128 subNew(__m128 a, __m128 b) { return PS2_VSUB(a, b); }
    static __m128 mulNew(__m128 a, __m128 b) { return PS2_VMUL(a, b); }
    static __m128 addRef(__m128 a, __m128 b) { return PS2_VADD_REF(a, b); }
    static __m128 subRef(__m128 a, __m128 b) { return PS2_VSUB_REF(a, b); }
    static __m128 mulRef(__m128 a, __m128 b) { return PS2_VMUL_REF(a, b); }
    // The natives' clamp-once form: operands normalised by the caller.
    static __m128 mulNNew(__m128 a, __m128 b) { return ps2VuResult(_mm_mul_ps(ps2VuNormalize(a), ps2VuNormalize(b))); }
    static __m128 addNNew(__m128 a, __m128 b) { return ps2VuResult(_mm_add_ps(ps2VuNormalize(a), ps2VuNormalize(b))); }
    static __m128 subNNew(__m128 a, __m128 b) { return ps2VuResult(_mm_sub_ps(ps2VuNormalize(a), ps2VuNormalize(b))); }

    static void checkVu(Counter &c, VuOp fNew, VuOp fRef, const uint32_t *a, const uint32_t *b)
    {
        const __m128 va = fromBits(a), vb = fromBits(b);
        const __m128 n = fNew(va, vb), o = fRef(va, vb);
        c.checked += 4;
        if (!same(n, o))
            for (int l = 0; l < 4; ++l)
                if (bitsOf(n, l) != bitsOf(o, l))
                    c.report(a[l], b[l], bitsOf(o, l), bitsOf(n, l));
    }

    using FpuOp = float (*)(float, float);
    // The table alignment (FPU_ADD_S / FPU_SUB_S on the Vita), on every host.
    static float fAddNew(float a, float b)
    {
        ps2FpuAlignOperandsFast(a, b);
        return ps2FpuClamp(a + b);
    }
    static float fSubNew(float a, float b)
    {
        ps2FpuAlignOperandsFast(a, b);
        return ps2FpuClamp(a - b);
    }
    static float fAddShift(float a, float b)
    {
        ps2FpuAlignOperandsShift(a, b);
        return ps2FpuClamp(a + b);
    }
    static float fSubShift(float a, float b)
    {
        ps2FpuAlignOperandsShift(a, b);
        return ps2FpuClamp(a - b);
    }
    static float fAddMacro(float a, float b) { return FPU_ADD_S(a, b); }
    static float fSubMacro(float a, float b) { return FPU_SUB_S(a, b); }
    static float fAddRef(float a, float b)
    {
        ps2FpuAlignOperandsRef(a, b);
        return ps2FpuClamp(a + b);
    }
    static float fSubRef(float a, float b)
    {
        ps2FpuAlignOperandsRef(a, b);
        return ps2FpuClamp(a - b);
    }
    static void checkFpu(Counter &c, FpuOp fNew, FpuOp fRef, uint32_t a, uint32_t b)
    {
        float fa, fb;
        std::memcpy(&fa, &a, 4);
        std::memcpy(&fb, &b, 4);
        const float n = fNew(fa, fb), o = fRef(fa, fb);
        uint32_t un, uo;
        std::memcpy(&un, &n, 4);
        std::memcpy(&uo, &o, 4);
        ++c.checked;
        if (un != uo)
        {
            // NaN + NaN: which operand's NaN the hardware passes on (without
            // default-NaN mode) follows the register order the compiler
            // picked for a + b, which C++ leaves open (old and new alike);
            // the clamp keeps that NaN's sign. Either sign is the old code.
            const bool nanA = (a & 0x7F800000u) == 0x7F800000u && (a & 0x7FFFFFu) != 0u;
            const bool nanB = (b & 0x7F800000u) == 0x7F800000u && (b & 0x7FFFFFu) != 0u;
            const uint32_t sa = (a & 0x80000000u) | 0x7F7FFFFFu, sb = (b & 0x80000000u) | 0x7F7FFFFFu;
            if (nanA && nanB && (un == sa || un == sb) && (uo == sa || uo == sb))
                ++c.nanOrder;
            else
                c.report(a, b, uo, un);
        }
    }

#if PS2X_VU_NEON_A32 && !PS2X_VU_MACROS_OLD && defined(__GNUC__) && !defined(__clang__)
    // The overridden sse2neon intrinsics against the lane selections they
    // stand for (and the parenthesised names reach sse2neon's own).
    template <int Imm>
    static bool shuffleOk(__m128 a, __m128 b)
    {
        const __m128 got = _mm_shuffle_ps(a, b, Imm);
        const __m128 got2 = _mm_shuffle_ps(a, a, Imm);
        const uint32_t want[4] = {bitsOf(a, Imm & 3), bitsOf(a, (Imm >> 2) & 3), bitsOf(b, (Imm >> 4) & 3), bitsOf(b, (Imm >> 6) & 3)};
        const uint32_t want2[4] = {bitsOf(a, Imm & 3), bitsOf(a, (Imm >> 2) & 3), bitsOf(a, (Imm >> 4) & 3), bitsOf(a, (Imm >> 6) & 3)};
        return same(got, fromBits(want)) && same(got2, fromBits(want2));
    }
    template <int... I>
    static uint64_t shuffleAll(__m128 a, __m128 b, std::integer_sequence<int, I...>)
    {
        return (static_cast<uint64_t>(!shuffleOk<I>(a, b)) + ...);
    }
    static void checkIntrinsics(Rng &r, uint64_t rounds)
    {
        Counter shuf{"shuffle (256 imm)"}, blend{"blendv"}, set{"set_epi32"};
        for (uint64_t i = 0; i < rounds; ++i)
        {
            uint32_t a[4], b[4], m[4];
            for (int l = 0; l < 4; ++l)
            {
                a[l] = r.next();
                b[l] = r.next();
                m[l] = r.next();
            }
            const __m128 va = fromBits(a), vb = fromBits(b), vm = fromBits(m);
            const uint64_t bad = shuffleAll(va, vb, std::make_integer_sequence<int, 256>{});
            shuf.checked += 512;
            if (bad)
                shuf.report(a[0], b[0], 0, static_cast<uint32_t>(bad));
            uint32_t want[4];
            for (int l = 0; l < 4; ++l)
                want[l] = (m[l] & 0x80000000u) ? b[l] : a[l];
            blend.checked += 4;
            if (!same(_mm_blendv_ps(va, vb, vm), fromBits(want)) || !same(_mm_blendv_ps(va, vb, vm), (_mm_blendv_ps)(va, vb, vm)))
                blend.report(a[0], b[0], want[0], bitsOf(_mm_blendv_ps(va, vb, vm), 0));
            // constant masks, as the generated code writes them
            const __m128 c1 = _mm_blendv_ps(va, vb, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            const __m128 c2 = _mm_blendv_ps(va, vb, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0)));
            const __m128 c3 = _mm_blendv_ps(va, vb, _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, -1)));
            const uint32_t w3[4] = {b[0], a[1], b[2], a[3]};
            blend.checked += 12;
            if (!same(c1, vb) || !same(c2, va) || !same(c3, fromBits(w3)))
                blend.report(a[0], b[0], 0, 1);
            const int s3 = static_cast<int>(a[3]), s2 = static_cast<int>(a[2]), s1 = static_cast<int>(a[1]), s0 = static_cast<int>(a[0]);
            const __m128i e = _mm_set_epi32(s3, s2, s1, s0);
            const __m128i e0 = (_mm_set_epi32)(s3, s2, s1, s0);
            set.checked += 4;
            if (std::memcmp(&e, &e0, 16) != 0 || std::memcmp(&e, a, 16) != 0)
                set.report(a[0], a[1], 0, 1);
        }
        shuf.print();
        blend.print();
        set.print();
    }
#endif

    // Timing: four columns weighted by a vector (vmulax / vmadday / vmaddaz /
    // vmaddw, the most common VU0 sequence) and a chain of FPU adds.
    struct NewOps
    {
        static __m128 mul(__m128 a, __m128 b) { return PS2_VMUL(a, b); }
        static __m128 add(__m128 a, __m128 b) { return PS2_VADD(a, b); }
        static float fadd(float a, float b) { return fAddNew(a, b); }
    };
    struct RefOps
    {
        static __m128 mul(__m128 a, __m128 b) { return PS2_VMUL_REF(a, b); }
        static __m128 add(__m128 a, __m128 b) { return PS2_VADD_REF(a, b); }
        static float fadd(float a, float b) { return fAddRef(a, b); }
    };
    template <class Ops>
    __attribute__((noinline)) static __m128 benchVu(const __m128 *cols, const __m128 *vs, int n, int reps)
    {
        __m128 sum = _mm_setzero_ps();
        for (int r = 0; r < reps; ++r)
            for (int i = 0; i < n; ++i)
            {
                const __m128 v = vs[i];
                __m128 acc = Ops::mul(cols[0], _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
                acc = Ops::add(acc, Ops::mul(cols[1], _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))));
                acc = Ops::add(acc, Ops::mul(cols[2], _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))));
                acc = Ops::add(acc, Ops::mul(cols[3], _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3))));
                sum = _mm_blendv_ps(sum, _mm_xor_ps(sum, acc), _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            }
        return sum;
    }
    struct ShiftOps
    {
        static float fadd(float a, float b) { return fAddShift(a, b); }
    };
    // independent adds (throughput) and compares
    template <class Ops>
    __attribute__((noinline)) static float benchFpuWide(const float *xs, int n, int reps)
    {
        float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f, s3 = 0.0f;
        for (int r = 0; r < reps; ++r)
            for (int i = 0; i + 3 < n; i += 4)
            {
                s0 = Ops::fadd(xs[i], xs[i + 1]) + s0 * 0.0f;
                s1 = Ops::fadd(xs[i + 1], xs[i + 2]) + s1 * 0.0f;
                s2 = Ops::fadd(xs[i + 2], xs[i + 3]) + s2 * 0.0f;
                s3 = Ops::fadd(xs[i + 3], xs[i]) + s3 * 0.0f;
            }
        return s0 + s1 + s2 + s3;
    }
    template <bool Fast>
    __attribute__((noinline)) static uint32_t benchCmp(const float *xs, int n, int reps)
    {
        uint32_t c = 0;
        for (int r = 0; r < reps; ++r)
            for (int i = 0; i + 1 < n; ++i)
            {
                const bool lt = Fast ? ps2FpuCompareKeyFast(xs[i]) < ps2FpuCompareKeyFast(xs[i + 1]) : ps2FpuCompareKeyRef(xs[i]) < ps2FpuCompareKeyRef(xs[i + 1]);
                c = lt ? (c | 0x800000u) + 1u : (c & ~0x800000u) + 1u;
            }
        return c;
    }
    template <class Ops>
    __attribute__((noinline)) static float benchFpu(const float *xs, int n, int reps)
    {
        float s = 0.0f;
        for (int r = 0; r < reps; ++r)
            for (int i = 0; i + 1 < n; i += 2)
                s = Ops::fadd(Ops::fadd(s, xs[i]), xs[i + 1]) * 0.5f;
        return s;
    }

    static int run(const char *spec)
    {
        const uint64_t millions = std::strtoull(spec, nullptr, 10);
        const bool exhaustive = std::strchr(spec, 'x') != nullptr;
        std::fprintf(stderr, "[selftest] macros %s, NEON A32 forms %s; %llu M random per op%s\n",
                     PS2X_VU_MACROS_OLD ? "OLD" : "new", PS2X_VU_NEON_A32 ? "on" : "off", (unsigned long long)millions,
                     exhaustive ? ", every 32-bit pattern through ps2VuNormalize" : "");
#if defined(__arm__) && !defined(__aarch64__)
        uint32_t fpscr;
        __asm__ __volatile__("vmrs %0, FPSCR" : "=r"(fpscr));
        std::fprintf(stderr, "[selftest] FPSCR %08x (RMode %u, FZ %u, DN %u)\n", fpscr, (fpscr >> 22) & 3u, (fpscr >> 24) & 1u, (fpscr >> 25) & 1u);
#endif
        const double t0 = nowSeconds();
        uint64_t bad = 0;

        // 1. ps2VuNormalizeFast == ps2VuNormalizeRef
        {
            Counter c{"normalize"};
            uint32_t b[4];
            if (exhaustive)
            {
                for (uint64_t x = 0; x < (1ull << 32); x += 4)
                {
                    for (int l = 0; l < 4; ++l)
                        b[l] = static_cast<uint32_t>(x + l);
                    const __m128 v = fromBits(b);
                    const __m128 n = ps2VuNormalizeFast(v), o = ps2VuNormalizeRef(v);
                    c.checked += 4;
                    if (!same(n, o))
                        for (int l = 0; l < 4; ++l)
                            if (bitsOf(n, l) != bitsOf(o, l))
                                c.report(b[l], 0, bitsOf(o, l), bitsOf(n, l));
                }
            }
            else
            {
                Rng r{0x9E3779B97F4A7C15ull};
                for (uint64_t i = 0; i < millions * 1000000ull; ++i)
                {
                    for (int l = 0; l < 4; ++l)
                        b[l] = pick(r, r.next());
                    const __m128 v = fromBits(b);
                    c.checked += 4;
                    if (!same(ps2VuNormalizeFast(v), ps2VuNormalizeRef(v)))
                        c.report(b[0], 0, 0, 1);
                }
            }
            c.print();
            bad += c.bad;
        }

        // 1b. the FPU compare key (C.EQ / C.LT / C.LE): a function of one
        // operand, so equal keys for every pattern make every compare equal.
        {
            Counter c{"compare key"};
            Rng r{0xA0761D6478BD642Full};
            const uint64_t n = exhaustive ? (1ull << 32) : millions * 4000000ull;
            for (uint64_t x = 0; x < n; ++x)
            {
                const uint32_t b = exhaustive ? static_cast<uint32_t>(x) : pick(r, r.next());
                float f;
                std::memcpy(&f, &b, 4);
                ++c.checked;
                if (ps2FpuCompareKeyFast(f) != ps2FpuCompareKeyRef(f))
                    c.report(b, 0, ps2FpuCompareKeyRef(f), ps2FpuCompareKeyFast(f));
            }
            c.print();
            bad += c.bad;
        }

        // 2. the VU0 ops: every pair of edge patterns, then random pairs.
        static uint32_t edge[4096];
        const int ne = edgeValues(edge);
        struct
        {
            Counter c;
            VuOp fNew, fRef;
        } vu[] = {{{"PS2_VADD"}, addNew, addRef}, {{"PS2_VSUB"}, subNew, subRef}, {{"PS2_VMUL"}, mulNew, mulRef},
                  {{"vu::addN (natives)"}, addNNew, addRef}, {{"vu::subN (natives)"}, subNNew, subRef}, {{"vu::mulN (natives)"}, mulNNew, mulRef}};
        for (auto &t : vu)
        {
            for (int i = 0; i < ne; ++i)
                for (int j = 0; j < ne; j += 4)
                {
                    const uint32_t a[4] = {edge[i], edge[i], edge[i], edge[i]};
                    checkVu(t.c, t.fNew, t.fRef, a, &edge[j]);
                }
            Rng r{0xD1B54A32D192ED03ull ^ reinterpret_cast<uintptr_t>(t.c.name)};
            for (uint64_t i = 0; i < millions * 1000000ull; ++i)
            {
                uint32_t a[4], b[4];
                for (int l = 0; l < 4; ++l)
                {
                    a[l] = pick(r, r.next());
                    b[l] = pick(r, a[l]);
                }
                checkVu(t.c, t.fNew, t.fRef, a, b);
            }
            t.c.print();
            bad += t.c.bad;
        }

        // 3. the FPU's operand alignment: every exponent pair (the masks
        // depend on nothing else: all-ones mantissa and sign show them), then
        // FPU_ADD_S / FPU_SUB_S whole on edge pairs and random pairs.
        {
            Counter c{"align (all exp pairs)"};
            for (uint32_t ea = 0; ea < 256; ++ea)
                for (uint32_t eb = 0; eb < 256; ++eb)
                {
                    const uint32_t a = 0x807FFFFFu | (ea << 23), b = 0x807FFFFFu | (eb << 23);
                    float fa, fb, ra, rb;
                    std::memcpy(&fa, &a, 4);
                    std::memcpy(&fb, &b, 4);
                    ra = fa;
                    rb = fb;
                    float sa = fa, sb = fb;
                    ps2FpuAlignOperandsRef(ra, rb);
                    ps2FpuAlignOperandsFast(fa, fb);
                    ps2FpuAlignOperandsShift(sa, sb);
                    c.checked += 2;
                    if (std::memcmp(&fa, &ra, 4) != 0 || std::memcmp(&fb, &rb, 4) != 0)
                        c.report(a, b, 0, 1);
                    if (std::memcmp(&sa, &ra, 4) != 0 || std::memcmp(&sb, &rb, 4) != 0)
                        c.report(a, b, 0, 2);
                }
            c.print();
            bad += c.bad;
        }
        struct
        {
            Counter c;
            FpuOp fNew, fRef;
        } fpu[] = {{{"add, table alignment"}, fAddNew, fAddRef}, {{"sub, table alignment"}, fSubNew, fSubRef},
                   {{"add, shift alignment"}, fAddShift, fAddRef}, {{"sub, shift alignment"}, fSubShift, fSubRef},
                   {{"FPU_ADD_S"}, fAddMacro, fAddRef}, {{"FPU_SUB_S"}, fSubMacro, fSubRef}};
        for (auto &t : fpu)
        {
            for (int i = 0; i < ne; ++i)
                for (int j = 0; j < ne; ++j)
                    checkFpu(t.c, t.fNew, t.fRef, edge[i], edge[j]);
            Rng r{0x94D049BB133111EBull ^ reinterpret_cast<uintptr_t>(t.c.name)};
            for (uint64_t i = 0; i < millions * 4000000ull; ++i)
            {
                const uint32_t a = pick(r, r.next());
                checkFpu(t.c, t.fNew, t.fRef, a, pick(r, a));
            }
            t.c.print();
            bad += t.c.bad;
        }

        // 3c. round 29's forms. FPU_*_SF (no result clamp) on finite operands
        // against the old FPU_ADD_S / FPU_SUB_S / FPU_MUL_S (aligned, clamped):
        // they hold in round toward zero only (this thread's mode).
        {
            struct
            {
                Counter c;
                FpuOp fNew, fRef;
            } fin[] = {{{"FPU_ADD_SF (finite)"}, [](float a, float b) { return FPU_ADD_SF(a, b); }, fAddRef},
                       {{"FPU_SUB_SF (finite)"}, [](float a, float b) { return FPU_SUB_SF(a, b); }, fSubRef},
                       {{"FPU_MUL_SF (finite)"}, [](float a, float b) { return FPU_MUL_SF(a, b); }, [](float a, float b) { return ps2FpuClamp(a * b); }}};
            auto finite = [](uint32_t x) { return (x & 0x7F800000u) != 0x7F800000u; };
            for (auto &t : fin)
            {
                for (int i = 0; i < ne; ++i)
                    for (int j = 0; j < ne; ++j)
                        if (finite(edge[i]) && finite(edge[j]))
                            checkFpu(t.c, t.fNew, t.fRef, edge[i], edge[j]);
                Rng r{0x5851F42D4C957F2Dull ^ reinterpret_cast<uintptr_t>(t.c.name)};
                for (uint64_t i = 0; i < millions * 4000000ull; ++i)
                {
                    const uint32_t a = pick(r, r.next());
                    const uint32_t b = pick(r, a);
                    if (finite(a) && finite(b))
                        checkFpu(t.c, t.fNew, t.fRef, a, b);
                }
                t.c.print();
                bad += t.c.bad;
            }
        }
        // 3d. PS2_V*_K: an operand that is a VU0 result already (what
        // ps2VuOperand makes of anything) skips the normalisation; the same
        // bits as PS2_V* and the old macros.
        {
            struct
            {
                Counter c;
                VuOp fNew, fRef;
            } kn[] = {{{"PS2_VADD_K(1, 0)"}, [](__m128 a, __m128 b) { return PS2_VADD_K(1, 0, ps2VuOperand(a), b); }, [](__m128 a, __m128 b) { return PS2_VADD_REF(ps2VuOperand(a), b); }},
                      {{"PS2_VADD_K(0, 1)"}, [](__m128 a, __m128 b) { return PS2_VADD_K(0, 1, a, ps2VuOperand(b)); }, [](__m128 a, __m128 b) { return PS2_VADD_REF(a, ps2VuOperand(b)); }},
                      {{"PS2_VSUB_K(1, 1)"}, [](__m128 a, __m128 b) { return PS2_VSUB_K(1, 1, ps2VuOperand(a), ps2VuOperand(b)); }, [](__m128 a, __m128 b) { return PS2_VSUB_REF(ps2VuOperand(a), ps2VuOperand(b)); }},
                      {{"PS2_VMUL_K(1, 1)"}, [](__m128 a, __m128 b) { return PS2_VMUL_K(1, 1, ps2VuOperand(a), ps2VuOperand(b)); }, [](__m128 a, __m128 b) { return PS2_VMUL_REF(ps2VuOperand(a), ps2VuOperand(b)); }},
                      {{"PS2_VMUL_K(0, 1) of a result"}, [](__m128 a, __m128 b) { return PS2_VMUL_K(0, 1, a, PS2_VADD(b, b)); }, [](__m128 a, __m128 b) { return PS2_VMUL_REF(a, PS2_VADD_REF(b, b)); }}};
            for (auto &t : kn)
            {
                for (int i = 0; i < ne; ++i)
                    for (int j = 0; j < ne; j += 4)
                    {
                        const uint32_t a[4] = {edge[i], edge[i], edge[i], edge[i]};
                        checkVu(t.c, t.fNew, t.fRef, a, &edge[j]);
                    }
                Rng r{0x2545F4914F6CDD1Dull ^ reinterpret_cast<uintptr_t>(t.c.name)};
                for (uint64_t i = 0; i < millions * 1000000ull; ++i)
                {
                    uint32_t a[4], b[4];
                    for (int l = 0; l < 4; ++l)
                    {
                        a[l] = pick(r, r.next());
                        b[l] = pick(r, a[l]);
                    }
                    checkVu(t.c, t.fNew, t.fRef, a, b);
                }
                t.c.print();
                bad += t.c.bad;
            }
        }

#if PS2X_VU_NEON_A32 && !PS2X_VU_MACROS_OLD && defined(__GNUC__) && !defined(__clang__)
        {
            Rng r{0x123456789ull};
            checkIntrinsics(r, 2000 + millions * 2000ull);
        }
#endif
        const double t1 = nowSeconds();

        // 3b. rounding (a report, not a check): PS2_VMUL / PS2_VADD against
        // the same lanes done by the scalar FPU in this thread's float
        // environment (round toward zero on the game thread, like the EE and
        // the Mac's NEON). On ARMv7 NEON always rounds to nearest.
        {
            Rng r{77};
            uint64_t n = 0, diffMul = 0, diffAdd = 0;
            for (uint64_t i = 0; i < 1000000ull; ++i)
            {
                alignas(16) float a[4], b[4], m[4], s[4];
                for (int l = 0; l < 4; ++l)
                {
                    const uint32_t ua = (r.next() & 0x807FFFFFu) | ((100u + r.next() % 56u) << 23);
                    const uint32_t ub = (r.next() & 0x807FFFFFu) | ((100u + r.next() % 56u) << 23);
                    std::memcpy(&a[l], &ua, 4);
                    std::memcpy(&b[l], &ub, 4);
                }
                const __m128 va = _mm_load_ps(a), vb = _mm_load_ps(b);
                const __m128 vm = PS2_VMUL(va, vb), vs = PS2_VADD(va, vb);
                for (int l = 0; l < 4; ++l)
                {
                    volatile float x = a[l], y = b[l];
                    m[l] = x * y;
                    s[l] = x + y;
                }
                for (int l = 0; l < 4; ++l)
                {
                    uint32_t u;
                    std::memcpy(&u, &m[l], 4);
                    diffMul += bitsOf(vm, l) != u;
                    std::memcpy(&u, &s[l], 4);
                    diffAdd += bitsOf(vs, l) != u;
                }
                n += 4;
            }
            std::fprintf(stderr, "[selftest] rounding: VU0 lanes != scalar FPU (this thread's mode) on random normal operands: mul %.1f%%, add %.1f%%\n",
                         100.0 * static_cast<double>(diffMul) / static_cast<double>(n), 100.0 * static_cast<double>(diffAdd) / static_cast<double>(n));
        }

        // 4. timing, finite data (what the game mostly feeds them)
        {
            static __m128 cols[4], vs[256];
            static float xs[512];
            Rng r{42};
            auto finite = [&r]() {
                const uint32_t b = (r.next() & 0x807FFFFFu) | ((100u + r.next() % 56u) << 23);
                float f;
                std::memcpy(&f, &b, 4);
                return f;
            };
            for (auto &c : cols)
                c = _mm_set_ps(finite(), finite(), finite(), finite());
            for (auto &v : vs)
                v = _mm_set_ps(finite(), finite(), finite(), finite());
            for (auto &x : xs)
                x = finite();
            const int reps = 2000;
            double s = nowSeconds();
            const __m128 a = benchVu<RefOps>(cols, vs, 256, reps);
            const double tVuRef = nowSeconds() - s;
            s = nowSeconds();
            const __m128 b = benchVu<NewOps>(cols, vs, 256, reps);
            const double tVuNew = nowSeconds() - s;
            s = nowSeconds();
            const float fa = benchFpu<RefOps>(xs, 512, reps);
            const double tFRef = nowSeconds() - s;
            s = nowSeconds();
            const float fb = benchFpu<NewOps>(xs, 512, reps);
            const double tFNew = nowSeconds() - s;
            s = nowSeconds();
            const float fc = benchFpu<ShiftOps>(xs, 512, reps);
            const double tFShift = nowSeconds() - s;
            s = nowSeconds();
            const float w0 = benchFpuWide<RefOps>(xs, 512, reps);
            const double tW0 = nowSeconds() - s;
            s = nowSeconds();
            const float w1 = benchFpuWide<NewOps>(xs, 512, reps);
            const double tW1 = nowSeconds() - s;
            s = nowSeconds();
            const float w2 = benchFpuWide<ShiftOps>(xs, 512, reps);
            const double tW2 = nowSeconds() - s;
            // compares on signed data
            static float ys[512];
            for (auto &y : ys)
            {
                const uint32_t u = (r.next() & 0x807FFFFFu) | ((100u + r.next() % 56u) << 23);
                std::memcpy(&y, &u, 4);
            }
            s = nowSeconds();
            const uint32_t c0 = benchCmp<false>(ys, 512, reps);
            const double tC0 = nowSeconds() - s;
            s = nowSeconds();
            const uint32_t c1 = benchCmp<true>(ys, 512, reps);
            const double tC1 = nowSeconds() - s;
            const double nv = 256.0 * reps, nf = 512.0 * reps;
            std::fprintf(stderr, "[selftest] timing: FPU add chain shift %.2f ns (%s); independent adds old %.2f / table %.2f / shift %.2f ns (%s); compare old %.2f / new %.2f ns (%s)\n",
                         tFShift / nf * 1e9, std::memcmp(&fa, &fc, 4) == 0 ? "same" : "DIFFERENT", tW0 / nf * 1e9, tW1 / nf * 1e9, tW2 / nf * 1e9,
                         (std::memcmp(&w0, &w1, 4) == 0 && std::memcmp(&w0, &w2, 4) == 0) ? "same" : "DIFFERENT", tC0 / nf * 1e9, tC1 / nf * 1e9, c0 == c1 ? "same" : "DIFFERENT");
            std::fprintf(stderr, "[selftest] timing: matrix x vector (4 mul + 3 add) old %.1f ns, new %.1f ns (%s); FPU add old %.2f ns, new %.2f ns (%s)\n",
                         tVuRef / nv * 1e9, tVuNew / nv * 1e9, same(a, b) ? "same" : "DIFFERENT", tFRef / nf * 1e9, tFNew / nf * 1e9,
                         std::memcmp(&fa, &fb, 4) == 0 ? "same" : "DIFFERENT");
            if (!same(a, b) || std::memcmp(&fa, &fb, 4) != 0)
                ++bad;
        }
        std::fprintf(stderr, "[selftest] %s: %llu mismatches (%.1f s)\n", bad ? "FAILED" : "passed", (unsigned long long)bad, t1 - t0);
        return bad ? 1 : 0;
    }
}

int ps2MacroSelfTest(const char *spec)
{
    return ps2_macro_selftest::run(spec);
}
