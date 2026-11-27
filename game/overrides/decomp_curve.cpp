// The motion curves' key decoder (manager_sub_010AC6C8, "curveEval"): one
// key of a channel into a qword, ~360 calls a frame in the attract demo
// (ExecCharaGeometryPreprocess -> GetMotionFloatFrameNoCaribration ->
// 010B81F8, and the IK / expression nodes), ~1 ms of the console's frame.
//
// a0 the qword out, a1 the channel's format (0..14), a2 the channel, a3 the
// key's index; v0 1 for a format with no data (out = vf0), else 0. The
// formats (by the jump table at 0x1193230, read live: the module's data):
//   0      the channel's own qword
//   1      a float xyz key (+4..+0xc; +1's sign byte), w = sqrt(1 - |xyz|^2) (|xyz|^2 at most 1)
//   2      an axis (the channel's qword) x cos, sin of a float angle table (+0xc)
//   3..6   a 32-bit key (table +0x20): three 9-bit components with
//          offset / scale (channel +0 .. +0x14) and the fourth from a 4-bit
//          one plus w = +-sqrt(1 - sum) (VU0 sqrt), each format one lane
//   7..9   none (out = vf0, v0 = 1)
//   10     a float xyz key (12 bytes), w from |xyz|^2 (VU0), the sign in x's low bit
//   11     an s16 xyz key x 2^-15, likewise
//   12     an s16 xyz key (table +0x18) with offset / scale
//   13     an s16 angle (table +0x14) with offset / scale, halved: sin / cos
//          of it, the channel's +8..+0x10 axis x cos
//   14     a 32-bit key (table +0x18): 11 / 10 / 10 bits with offset / scale
// Done natively: 0, 3..9, 12 and 13 (the attract demo's and the route's;
// SOTC_CURVE_STATS=1 counts the calls by format). 1, 2, 10, 11 and 14 were
// never seen there: the recompiled code. iosGetSinCosf (format 13) is
// decomp_math's, called directly.
//
// The recompiled code's operations in its order (FPU_* / PS2_V* macros,
// the constants by their bits); the callee's frame below sp is dead after
// the return. f0, v0, fcr31 and the VU0 registers it writes (vf1, vf5-vf7,
// Q, the status) as the original leaves them.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "d1_native.h"
#include "decomp_curve.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_math
{
    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_curve
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kCurveEval = 0x010AC6C8u, kMotionKeys = 0x010B81F8u, kTable = 0x01193230u, kSinCos = 0x001BDDB0u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        void put(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        // A word copied as the lwc1 / swc1 pair does (the bits).
        void copy(uint32_t to, uint32_t from) const { put32(to, u32(from)); }
    };

    inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    const __m128 kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)), kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1));
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    float mul(float a, float b) { return FPU_MUL_S(a, b); }
    float add(float a, float b) { return FPU_ADD_S(a, b); }
    float sub(float a, float b) { return FPU_SUB_S(a, b); }
    float cvt(int32_t w) { return FPU_CVT_S_W(w); }

    struct Eval
    {
        Ram ram;
        R5900Context *ctx;
        uint32_t fcr31;
        bool vu7 = false, vu56 = false, vu1 = false;
        __m128 vf1, vf5, vf6, vf7;
        float q;
        uint16_t status;

        void cond(bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }

        // lqc2 vf7, out; vmul.xyz vf7, vf7, vf7; vaddy.x; vaddz.x: |xyz|^2 in x.
        float squared(uint32_t out)
        {
            vf7 = ram.q(out);
            vf7 = PS2_VBLEND(vf7, PS2_VMUL(vf7, vf7), kXyz);
            vf7 = _mm_blendv_ps(vf7, PS2_VADD(vf7, lane<1>(vf7)), kX);
            vf7 = _mm_blendv_ps(vf7, PS2_VADD(vf7, lane<2>(vf7)), kX);
            vu7 = true;
            return _mm_cvtss_f32(vf7);
        }
        // mfc1 v0, f; qmtc2 v0, vf6 (the GPR's upper words as they are); lqc2
        // vf5, out; vmulx.xyz vf5, vf5, vf6x; sqc2 vf5, out.
        void scaleOut(uint32_t out, float f)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(bitsOf(f)));
            vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            vf5 = ram.q(out);
            vf5 = _mm_blendv_ps(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
            ram.putq(out, vf5);
            vu56 = true;
        }
        // The w of formats 10 / 11 / 12: sqrt(1 - |xyz|^2), negated by the flag.
        float wFromSquared(uint32_t out, bool negative)
        {
            float f0 = sub(1.0f, squared(out));
            f0 = ps2FpuSqrt(fcr31, f0);
            if (negative)
                f0 = FPU_NEG_S(f0);
            ram.put(out + 0xCu, f0);
            return f0;
        }
        // Formats 3..6: three 9-bit fields (shifted left by s0..s2, then sra 23)
        // into lanes l0..l2, the 4-bit one (shift s3, sra 28) into lane l3,
        // its sign from the key's bit sb (31: the word's sign).
        float packed9(uint32_t out, uint32_t chan, uint32_t key, int s0, int s1, int s2, int s3, uint32_t l0, uint32_t l1, uint32_t l2, uint32_t l3, int sb)
        {
            const float k = bits(0x3B808080u);
            auto field = [&](int shift, uint32_t scale, uint32_t offset)
            {
                const int32_t v = static_cast<int32_t>(ram.u32(key) << shift) >> 23;
                return add(mul(mul(cvt(v), ram.f32(chan + scale)), k), ram.f32(chan + offset));
            };
            float f2 = field(s0, 4u, 0u);
            ram.put(out + l0 * 4u, f2);
            f2 = mul(f2, f2);
            float f1 = field(s1, 0xCu, 8u);
            ram.put(out + l1 * 4u, f1);
            f1 = mul(f1, f1);
            const float c = field(s2, 0x14u, 0x10u);
            f2 = add(f2, f1);
            f1 = mul(c, c);
            ram.put(out + l2 * 4u, c);
            f2 = add(f2, f1);
            const float f5 = sub(1.0f, f2);
            vu1 = true;
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(bitsOf(f5))); // mfc1 a2 / qmtc2 a2, vf1 (x read)
            vf1 = _mm_castsi128_ps(GPR_VEC(ctx, 6));
            q = ps2VuSqrt(status, _mm_cvtss_f32(vf1));
            const uint32_t w = ram.u32(key);
            const int32_t v = static_cast<int32_t>(w << s3) >> 28;
            const float off = add(mul(mul(cvt(v), ram.f32(chan + 0x1Cu)), bits(0x3E124924u)), ram.f32(chan + 0x18u));
            const bool neg = cond5(f5);
            float f0;
            if (neg)
                f0 = add(off, 0.0f);
            else
            {
                f0 = q;
                if (sb == 31 ? static_cast<int32_t>(w) < 0 : ((w >> sb) & 1u) != 0u)
                    f0 = FPU_NEG_S(f0);
                f0 = add(f0, off);
            }
            ram.put(out + l3 * 4u, f0);
            return f0;
        }
        bool cond5(float f5)
        {
            const bool c = FPU_C_OLT_S(f5, 0.0f);
            cond(c);
            return c;
        }
    };

    // SOTC_CURVE_STATS=1: the calls by format, every 20 000.
    void count(uint64_t format)
    {
        static const bool on = ps2_diag::env("SOTC_CURVE_STATS") != nullptr;
        if (!on)
            return;
        static uint64_t n[16], total = 0;
        ++n[format < 15u ? format : 15u];
        if (++total % 20000u == 0u)
        {
            std::fprintf(stderr, "[curve] %llu calls:", static_cast<unsigned long long>(total));
            for (int i = 0; i < 16; ++i)
                if (n[i])
                    std::fprintf(stderr, " %d:%llu", i, static_cast<unsigned long long>(n[i]));
            std::fprintf(stderr, "\n");
        }
    }

    // The key into a0 natively; false (nothing written) for the cases the
    // recompiled code takes. Round 48's code (counted: the fast path called
    // count() already).
    bool evalKeyR48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool counted)
    {
        const Ram ram{rdram};
        const uint32_t out = GPR_U32(ctx, 4), chan = GPR_U32(ctx, 6), index = GPR_U32(ctx, 7);
        const uint64_t format = GPR_U64(ctx, 5);
        if (!counted)
            count(format);
        const uint32_t target = format < 15u ? ram.u32(kTable + static_cast<uint32_t>(format) * 4u) : 0x010AC700u;
        if (!inRam(out, 16u) || !inRam(chan, 0x20u))
            return false;

        Eval e{ram, ctx, ctx->fcr31};
        e.q = ctx->vu0_q;
        e.status = ctx->vu0_status;
        uint32_t v0 = 0u;
        bool setF0 = true;
        float f0 = 0.0f;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x40u;

        // iosGetSinCosf(a0 = frame + at, f12): sin, cos at a0.
        auto sinCos = [&](uint32_t at, float angle, uint32_t site)
        {
            SET_GPR_U32(ctx, 4, frame + at);
            ctx->f[12] = angle;
            const uint32_t fn = sotc_decomp::jalTarget(rdram, site);
            if (fn == kSinCos)
            {
                SET_GPR_U32(ctx, 29, frame);
                SET_GPR_U32(ctx, 31, site + 8u);
                ctx->fcr31 = e.fcr31;
                decomp_math::getSinCos(rdram, ctx, runtime);
            }
            else
            {
                ctx->fcr31 = e.fcr31;
                sotc_decomp::callGuest(rdram, ctx, runtime, fn, site + 8u, frame);
            }
            e.fcr31 = ctx->fcr31;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
        };

        switch (target)
        {
        case 0x010AD028u: // the channel's qword
            ram.putq(out, ram.q(chan));
            setF0 = false;
            break;
        case 0x010AC700u: // none
            ram.putq(out, ctx->vu0_vf[0]);
            v0 = 1u;
            setF0 = false;
            break;
        case 0x010AC8A8u: // s16 xyz with offset / scale (table +0x18)
        {
            const uint32_t rec = ram.u32(chan + 0x18u) + index * 6u;
            if (!inRam(rec, 6u))
                return false;
            const float k = bits(0x38000100u);
            ram.put(out, add(mul(mul(cvt(ram.s16(rec)), ram.f32(chan + 4u)), k), ram.f32(chan)));
            ram.put(out + 4u, add(mul(mul(cvt(ram.s16(rec + 2u)), ram.f32(chan + 0xCu)), k), ram.f32(chan + 8u)));
            ram.put(out + 8u, add(mul(mul(cvt(ram.s16(rec + 4u)), ram.f32(chan + 0x14u)), k), ram.f32(chan + 0x10u)));
            f0 = e.wFromSquared(out, (static_cast<uint16_t>(ram.s16(rec)) & 1u) != 0u);
            break;
        }
        case 0x010AC978u: // packed: y z w, x from the 4-bit field
        case 0x010ACAB0u: // x z w, y
        case 0x010ACBE8u: // x y w, z
        case 0x010ACD20u: // x y z, w
        {
            const uint32_t key = ram.u32(chan + 0x20u) + (index << 2);
            if (!inRam(key, 4u))
                return false;
            if (target == 0x010AC978u)
                f0 = e.packed9(out, chan, key, 5, 14, 23, 1, 1u, 2u, 3u, 0u, 31);
            else if (target == 0x010ACAB0u)
                f0 = e.packed9(out, chan, key, 0, 14, 23, 10, 0u, 2u, 3u, 1u, 22);
            else if (target == 0x010ACBE8u)
                f0 = e.packed9(out, chan, key, 0, 9, 23, 19, 0u, 1u, 3u, 2u, 13);
            else
                f0 = e.packed9(out, chan, key, 0, 9, 18, 28, 0u, 1u, 2u, 3u, 4);
            break;
        }
        case 0x010ACFA0u: // s16 angle with offset / scale, halved (table +0x14)
        {
            const uint32_t rec = ram.u32(chan + 0x14u) + (index << 1);
            if (!inRam(rec, 2u) || !inRam(frame, 0x20u))
                return false;
            float f12 = mul(cvt(ram.s16(rec)), ram.f32(chan + 4u));
            f12 = mul(f12, bits(0x38000100u));
            f12 = add(f12, ram.f32(chan));
            f12 = mul(f12, 0.5f);
            sinCos(0x10u, f12, 0x010ACFE4u);
            const float cosine = ram.f32(frame + 0x14u);
            ram.copy(out, chan + 8u);
            ram.copy(out + 4u, chan + 0xCu);
            ram.copy(out + 8u, chan + 0x10u);
            e.scaleOut(out, cosine);
            f0 = ram.f32(frame + 0x10u);
            ram.put(out + 0xCu, f0);
            break;
        }
        default:
            return false;
        }

        if (setF0)
            ctx->f[0] = f0;
        ctx->fcr31 = e.fcr31;
        if (e.vu7)
            ctx->vu0_vf[7] = e.vf7;
        if (e.vu56)
        {
            ctx->vu0_vf[5] = e.vf5;
            ctx->vu0_vf[6] = e.vf6;
        }
        if (e.vu1)
        {
            ctx->vu0_vf[1] = e.vf1;
            ctx->vu0_q = e.q;
            ctx->vu0_status = e.status;
        }
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
        return true;
    }

    // ---- Round 49 (SOTC_FASTNATIVE): formats 0, 3..9 without the Eval
    // struct (no 76-byte memset a call), the stats flag cached, and the packed
    // formats' FPU arithmetic without the clamps that provably change nothing:
    //  - the game thread rounds toward zero (ps2SetEeFloatingPointMode, on the
    //    Vita and the Mac alike: the generated code's FPU_*_SF rely on it,
    //    round 29), so a product / sum of two finite floats is finite (an
    //    overflow gives +-FLT_MAX) and FPU_MUL_S / ADD_S / SUB_S's result clamp
    //    is the identity on finite operands;
    //  - the channel's eight scales / offsets (+0 .. +0x1c, the only operands
    //    read from memory) are tested finite once (else round 48's code);
    //    cvt.s.w, the products, sums and the clamped VU0 sqrt are finite;
    //  - an add's operand alignment only runs when the signs differ
    //    (ps2FpuAddFinite, as FPU_ADD_S): a key field's product
    //    cvt(v) x scale x k has the sign (v < 0) ^ sign(scale) (k > 0; the
    //    IEEE product's sign, zeros and flushed results included), known from
    //    the integers, so the same-sign case is a plain add (round 29: the
    //    alignment changes nothing then), the other ps2FpuAddFinite itself;
    //    the squares are >= +0, so their sums are plain adds too.
    // The output qword must not overlap the channel's 0x20 bytes or the key
    // word (the original re-reads them after its stores; else round 48's
    // code). Bit-exact on every host.
    inline signed char g_statsOn = -1;
    inline void countFast(uint64_t format)
    {
        if (__builtin_expect(g_statsOn != 0, 0))
        {
            if (g_statsOn < 0)
                g_statsOn = ps2_diag::env("SOTC_CURVE_STATS") != nullptr ? 1 : 0;
            if (g_statsOn)
                count(format);
        }
    }
    inline bool overlaps(uint32_t a, uint32_t an, uint32_t b, uint32_t bn) { return a < b + bn && b < a + an; }
    // a + b where a's sign bit (bit 31 of signA) is known without moving a to
    // a core register; the same bits as ps2FpuAddFinite(a, b).
    inline __attribute__((always_inline)) float addSigned(float a, uint32_t signA, float b, uint32_t bBits)
    {
        if (static_cast<int32_t>(signA ^ bBits) >= 0)
            return a + b;
        return ps2FpuAddFinite(a, b);
    }
    // Any of the eight words' exponent 255 (Inf / NaN).
    inline bool anyNonFinite8(const uint8_t *p)
    {
#if defined(__ARM_NEON)
        const uint32x4_t e = vdupq_n_u32(0x7F800000u);
        const uint32x4_t a = vreinterpretq_u32_u8(vld1q_u8(p)), b = vreinterpretq_u32_u8(vld1q_u8(p + 16));
        const uint32x4_t t = vorrq_u32(vceqq_u32(vandq_u32(a, e), e), vceqq_u32(vandq_u32(b, e), e));
        const uint32x2_t h = vorr_u32(vget_low_u32(t), vget_high_u32(t));
        return vget_lane_u32(vpmax_u32(h, h), 0) != 0u;
#else
        uint32_t w[8];
        std::memcpy(w, p, 32);
        uint32_t bad = 0u;
        for (int i = 0; i < 8; ++i)
            bad |= static_cast<uint32_t>((w[i] & 0x7F800000u) == 0x7F800000u);
        return bad != 0u;
#endif
    }

    template <int S0, int S1, int S2, int S3, int L0, int L1, int L2, int L3, int SB>
    inline __attribute__((always_inline)) bool packedFast(uint8_t *rdram, R5900Context *ctx, uint32_t out, uint32_t chan, uint32_t key)
    {
        const uint8_t *cp = rdram + chan; // inRam(chan, 0x20)
        if (overlaps(out, 16u, chan, 0x20u) || overlaps(out, 16u, key, 4u) || anyNonFinite8(cp))
            return false;
        // Everything read before the first store (the stores cannot reach
        // these words: checked above), kept in registers.
        auto word = [cp](int i) __attribute__((always_inline))
        {
            uint32_t v;
            std::memcpy(&v, cp + i * 4, 4);
            return v;
        };
        auto flt = [cp](int i) __attribute__((always_inline))
        {
            float v;
            std::memcpy(&v, cp + i * 4, 4);
            return v;
        };
        uint32_t w;
        std::memcpy(&w, rdram + key, 4);
        uint8_t *op = rdram + out;
        const float k = bits(0x3B808080u);
        // A field: cvt(v) x scale x k + offset (scale / offset words si / oi).
        auto field = [&](int32_t v, int si, int oi) __attribute__((always_inline))
        {
            float p = static_cast<float>(v) * flt(si);
            p = p * k;
            return addSigned(p, static_cast<uint32_t>(v) ^ word(si), flt(oi), word(oi));
        };
        const float a = field(static_cast<int32_t>(w << S0) >> 23, 1, 0);
        const float b = field(static_cast<int32_t>(w << S1) >> 23, 3, 2);
        const float cc = field(static_cast<int32_t>(w << S2) >> 23, 5, 4);
        const int32_t v4 = static_cast<int32_t>(w << S3) >> 28;
        float p4 = static_cast<float>(v4) * flt(7);
        p4 = p4 * bits(0x3E124924u);
        const float off = addSigned(p4, static_cast<uint32_t>(v4) ^ word(7), flt(6), word(6));
        // One operation a statement (no a * b + c expression a compiler may
        // contract into a fused multiply-add: clang's -ffp-contract=on).
        float f2 = a * a;
        const float f1 = b * b;
        f2 = f2 + f1;
        const float f3 = cc * cc;
        f2 = f2 + f3;
        const float f5 = ps2FpuSubFinite(1.0f, f2);
        const uint32_t f5b = bitsOf(f5);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(f5b)); // mfc1 a2 / qmtc2 a2, vf1 (x read)
        const __m128 vf1 = _mm_castsi128_ps(GPR_VEC(ctx, 6));
        uint16_t status = ctx->vu0_status;
        const float q = ps2VuSqrt(status, _mm_cvtss_f32(vf1));
        // FPU_C_OLT_S(f5, 0): negative with a nonzero exponent.
        const bool neg = static_cast<int32_t>(f5b) < 0 && (f5b & 0x7F800000u) != 0u;
        float f0;
        if (neg)
            f0 = ps2FpuAddFinite(off, 0.0f);
        else
        {
            const bool minus = SB == 31 ? static_cast<int32_t>(w) < 0 : ((w >> SB) & 1u) != 0u;
            f0 = minus ? -q : q;
            f0 = ps2FpuAddFinite(f0, off);
        }
        std::memcpy(op + L0 * 4, &a, 4);
        std::memcpy(op + L1 * 4, &b, 4);
        std::memcpy(op + L2 * 4, &cc, 4);
        std::memcpy(op + L3 * 4, &f0, 4);
        ctx->f[0] = f0;
        ctx->fcr31 = neg ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        ctx->vu0_vf[1] = vf1;
        ctx->vu0_q = q;
        ctx->vu0_status = status;
        return true;
    }

    bool evalKeyFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t out = GPR_U32(ctx, 4), chan = GPR_U32(ctx, 6), index = GPR_U32(ctx, 7);
        const uint64_t format = GPR_U64(ctx, 5);
        countFast(format);
        const uint32_t target = format < 15u ? ram.u32(kTable + static_cast<uint32_t>(format) * 4u) : 0x010AC700u;
        if (!inRam(out, 16u) || !inRam(chan, 0x20u))
            return false;
        uint64_t v0 = 0u;
        switch (target)
        {
        case 0x010AD028u: // the channel's qword
            ram.putq(out, ram.q(chan));
            break;
        case 0x010AC700u: // none
            ram.putq(out, ctx->vu0_vf[0]);
            v0 = 1u;
            break;
        case 0x010AC978u:
        case 0x010ACAB0u:
        case 0x010ACBE8u:
        case 0x010ACD20u:
        {
            const uint32_t key = ram.u32(chan + 0x20u) + (index << 2);
            if (!inRam(key, 4u))
                return false;
            bool done;
            if (target == 0x010AC978u)
                done = packedFast<5, 14, 23, 1, 1, 2, 3, 0, 31>(rdram, ctx, out, chan, key);
            else if (target == 0x010ACAB0u)
                done = packedFast<0, 14, 23, 10, 0, 2, 3, 1, 22>(rdram, ctx, out, chan, key);
            else if (target == 0x010ACBE8u)
                done = packedFast<0, 9, 23, 19, 0, 1, 3, 2, 13>(rdram, ctx, out, chan, key);
            else
                done = packedFast<0, 9, 18, 28, 0, 1, 2, 3, 4>(rdram, ctx, out, chan, key);
            if (!done)
                return evalKeyR48(rdram, ctx, runtime, true);
            break;
        }
        default: // 12, 13 and the recompiled code's formats: round 48's code
            return evalKeyR48(rdram, ctx, runtime, true);
        }
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
        return true;
    }

    bool evalKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("evalKey");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return evalKeyFast(rdram, ctx, runtime);
        return evalKeyR48(rdram, ctx, runtime, false);
    }

    void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!evalKey(rdram, ctx, runtime))
            sotc_decomp::recompiled(kCurveEval)(rdram, ctx, runtime);
    }

    // manager_sub_010B81F8(a0 out, a1 the motion, a2 the key, a3 count):
    // min(count, motion +0x14) channels' keys (formats at *(motion +8), the
    // channels at *(motion +0x10)) into consecutive qwords, curveEval each
    // (natively, or through the function table for the formats it leaves to
    // the recompiled code). A motion without the format table (+8 == 0: the
    // other path, 010B1458 / 010B59C0) goes to the recompiled code.
    void motionKeys(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t out = GPR_U32(ctx, 4), motion = GPR_U32(ctx, 5);
        if (!inRam(motion, 0x18u) || ram.u32(motion + 8u) == 0u)
            return sotc_decomp::recompiled(kMotionKeys)(rdram, ctx, runtime);
        const int32_t channels = static_cast<int32_t>(ram.u32(motion + 0x14u));
        int32_t n = GPR_S32(ctx, 7);
        const bool fewer = channels < n;
        if (n != channels && fewer)
            n = channels;
        const uint64_t key64 = GPR_U64(ctx, 6); // s2 (daddu), each call's a3
        if (n <= 0)
        {
            SET_GPR_U64(ctx, 2, fewer ? 1u : 0u);
            return returnTo(ctx);
        }
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0xF0u;
        SET_GPR_U32(ctx, 29, frame);
        for (int32_t i = 0; i < n; ++i)
        {
            const uint32_t formats = ram.u32(motion + 8u), chans = ram.u32(motion + 0x10u);
            SET_GPR_U32(ctx, 4, out + static_cast<uint32_t>(i) * 16u);
            SET_GPR_U64(ctx, 5, rdram[(formats + static_cast<uint32_t>(i)) & PS2_RAM_MASK]);
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(ram.u32(chans + static_cast<uint32_t>(i) * 4u)));
            SET_GPR_U64(ctx, 7, key64);
            SET_GPR_U32(ctx, 31, 0x010B8290u);
            if (!evalKey(rdram, ctx, runtime))
                sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, 0x010B8290u, frame);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

namespace decomp_curve // round D2
{
    void curveEvalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void motionKeysSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_AB_FUNCTION(0x010AC6C8u, curveEval, decomp_curve::curveEvalSw);
D1_ALIAS(0x010AC6C8u, curveEval, decomp_curve::curveEvalSw, decomp_curve::curveEval);
SOTC_DECOMP_AB_FUNCTION(0x010B81F8u, motionKeys, decomp_curve::motionKeysSw);
D1_ALIAS(0x010B81F8u, motionKeys, decomp_curve::motionKeysSw, decomp_curve::motionKeys);

// ---- Round D2 (switch d1native, d1_native.h): manager_sub_010AC6C8 (curveEval) as native C++ --------------------------------
// The default (fastnative) path as a typed core on d1::Mem: formats 0 (the channel's qword), 7..9 (none: vf0, v0 = 1),
// 3..6 (round 49's packedFast: the same host ops in the same order, d1::add / d1::sub for ps2FpuAddFinite / SubFinite,
// ps2VuSqrt kept, the channel's eight words checked finite first) and 12 (round 48's s16 xyz key: FPU_MUL_S / ADD_S /
// SUB_S as d1::mul / add / sub on the channel's words checked finite, the |xyz|^2 VU0 block as d1:: NEON vec4, ps2FpuSqrt
// kept). Format 13 (iosGetSinCosf), the recompiled code's formats, an overlap of the out qword with the channel or the
// key, a non-finite channel word, pointers outside main RAM, fastnative off or SOTC_CURVE_STATS: the old body, before
// anything is written. The adapter writes f0, fcr31, vf1 / a2 (128 bits), vf7, Q, the status and v0 as the old body.
// motionKeys (0x010B81F8, round 49's caller that reaches the decoder by a direct C++ call, not through the table): its D2
// body calls the D2 decoder for each channel (else the old evalKey, else the guest call, as before).
namespace decomp_curve
{
    D1_COUNTER(c_d2CurveEval, "manager_sub_010AC6C8");
    D1_COUNTER(c_d2MotionKeys, "motionKeys");

    namespace d2c
    {
        PS2_FORCE_INLINE bool d2Overlaps(uint32_t a, uint32_t an, uint32_t b, uint32_t bn) { return a < b + bn && b < a + an; }
        PS2_FORCE_INLINE uint32_t d2fBits(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        PS2_FORCE_INLINE float d2bitsF(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        PS2_FORCE_INLINE bool d2InRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

        // What the decoder leaves (CurveRegs): decomp_curve.h.

        // Formats 3..6: round 49's packedFast on typed memory (false: nothing written).
        template <int S0, int S1, int S2, int S3, int L0, int L1, int L2, int L3, int SB>
        bool d2Packed(const d1::Mem &m, uint32_t out, uint32_t chan, uint32_t key, CurveRegs &r)
        {
            if (d2Overlaps(out, 16u, chan, 0x20u) || d2Overlaps(out, 16u, key, 4u) || anyNonFinite8(m.at<uint8_t>(chan)))
                return false;
            const uint32_t w = m.u32(key);
            const float k = d2bitsF(0x3B808080u);
            // A field: cvt(v) x scale x k + offset (scale / offset words si / oi).
            auto field = [&](int32_t v, uint32_t si, uint32_t oi)
            {
                float p = static_cast<float>(v) * m.f32(chan + si * 4u);
                p = p * k;
                return d1::add(p, m.f32(chan + oi * 4u));
            };
            const float a = field(static_cast<int32_t>(w << S0) >> 23, 1u, 0u);
            const float b = field(static_cast<int32_t>(w << S1) >> 23, 3u, 2u);
            const float cc = field(static_cast<int32_t>(w << S2) >> 23, 5u, 4u);
            const int32_t v4 = static_cast<int32_t>(w << S3) >> 28;
            float p4 = static_cast<float>(v4) * m.f32(chan + 7u * 4u);
            p4 = p4 * d2bitsF(0x3E124924u);
            const float off = d1::add(p4, m.f32(chan + 6u * 4u));
            // One operation a statement (no contraction into a fused multiply-add).
            float f2 = a * a;
            const float f1 = b * b;
            f2 = f2 + f1;
            const float f3 = cc * cc;
            f2 = f2 + f3;
            const float f5 = d1::sub(1.0f, f2);
            const uint32_t f5b = d2fBits(f5);
            // mfc1 a2, f5 (the low 64 bits sign-extended, the upper kept) / qmtc2 a2, vf1.
            __m128 vf1 = r.a2q;
            const uint64_t low = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(f5b)));
            std::memcpy(&vf1, &low, 8);
            uint16_t status = r.status;
            const float q = ps2VuSqrt(status, d1::lane<0>(vf1));
            // FPU_C_OLT_S(f5, 0): negative with a nonzero exponent.
            const bool neg = static_cast<int32_t>(f5b) < 0 && (f5b & 0x7F800000u) != 0u;
            float f0;
            if (neg)
                f0 = d1::add(off, 0.0f);
            else
            {
                const bool minus = SB == 31 ? static_cast<int32_t>(w) < 0 : ((w >> SB) & 1u) != 0u;
                f0 = minus ? -q : q;
                f0 = d1::add(f0, off);
            }
            m.set<float>(out + L0 * 4, a);
            m.set<float>(out + L1 * 4, b);
            m.set<float>(out + L2 * 4, cc);
            m.set<float>(out + L3 * 4, f0);
            r.f0 = f0;
            r.setF0 = true;
            r.fcr31 = neg ? (r.fcr31 | 0x800000u) : (r.fcr31 & ~0x800000u);
            r.vf1 = vf1;
            r.q = q;
            r.status = status;
            r.vu1 = true;
            return true;
        }

        // Format 12: an s16 xyz key (table +0x18) with offset / scale, w = +-sqrt(1 - |xyz|^2) (round 48's statements).
        bool d2S16Key(const d1::Mem &m, uint32_t out, uint32_t chan, uint32_t index, CurveRegs &r)
        {
            const uint32_t rec = m.u32(chan + 0x18u) + index * 6u;
            if (!d2InRam(rec, 6u) || anyNonFinite8(m.at<uint8_t>(chan)))
                return false;
            const float k = d2bitsF(0x38000100u);
            auto cvt16 = [&](uint32_t a) { return static_cast<float>(static_cast<int32_t>(m.get<int16_t>(a))); };
            m.set<float>(out, d1::add(d1::mul(d1::mul(cvt16(rec), m.f32(chan + 4u)), k), m.f32(chan)));
            m.set<float>(out + 4u, d1::add(d1::mul(d1::mul(cvt16(rec + 2u), m.f32(chan + 0xCu)), k), m.f32(chan + 8u)));
            m.set<float>(out + 8u, d1::add(d1::mul(d1::mul(cvt16(rec + 4u), m.f32(chan + 0x14u)), k), m.f32(chan + 0x10u)));
            const bool negative = (static_cast<uint16_t>(m.get<int16_t>(rec)) & 1u) != 0u;
            // lqc2 vf7, out; vmul.xyz vf7, vf7, vf7; vaddy.x; vaddz.x: |xyz|^2 in x.
            __m128 vf7 = m.q(out & ~0xFu);
            vf7 = d1::vsel(vf7, d1::vmul(vf7, vf7), d1::mask(1, 1, 1, 0));
            vf7 = d1::vsel(vf7, d1::vadd(vf7, d1::bc<1>(vf7)), d1::mask(1, 0, 0, 0));
            vf7 = d1::vsel(vf7, d1::vadd(vf7, d1::bc<2>(vf7)), d1::mask(1, 0, 0, 0));
            float f0 = d1::sub(1.0f, d1::lane<0>(vf7));
            f0 = ps2FpuSqrt(r.fcr31, f0);
            if (negative)
                f0 = -f0;
            m.set<float>(out + 0xCu, f0);
            r.vf7 = vf7;
            r.vu7 = true;
            r.f0 = f0;
            r.setF0 = true;
            return true;
        }

        // The key into out (false: nothing written, the old body's case).
        bool curveCore(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index, CurveRegs &r)
        {
            const uint32_t target = format < 15u ? m.u32(kTable + static_cast<uint32_t>(format) * 4u) : 0x010AC700u;
            r.v0 = 0u;
            switch (target)
            {
            case 0x010AD028u: // the channel's qword
                m.putq(out & ~0xFu, m.q(chan & ~0xFu));
                return true;
            case 0x010AC700u: // none
                m.putq(out & ~0xFu, d1::vf0());
                r.v0 = 1u;
                return true;
            case 0x010AC978u:
            case 0x010ACAB0u:
            case 0x010ACBE8u:
            case 0x010ACD20u:
            {
                const uint32_t key = m.u32(chan + 0x20u) + (index << 2);
                if (!d2InRam(key, 4u))
                    return false;
                if (target == 0x010AC978u)
                    return d2Packed<5, 14, 23, 1, 1, 2, 3, 0, 31>(m, out, chan, key, r);
                if (target == 0x010ACAB0u)
                    return d2Packed<0, 14, 23, 10, 0, 2, 3, 1, 22>(m, out, chan, key, r);
                if (target == 0x010ACBE8u)
                    return d2Packed<0, 9, 23, 19, 0, 1, 3, 2, 13>(m, out, chan, key, r);
                return d2Packed<0, 9, 18, 28, 0, 1, 2, 3, 4>(m, out, chan, key, r);
            }
            case 0x010AC8A8u: // s16 xyz with offset / scale (table +0x18)
                return d2S16Key(m, out, chan, index, r);
            default: // 13 (iosGetSinCosf) and the recompiled code's formats
                return false;
            }
        }
    } // namespace d2c

    // The D2 decoder on ctx's arguments: false (nothing written, nothing set) for the old body's cases.
    bool curveD2(uint8_t *rdram, R5900Context *ctx)
    {
        const uint32_t out = GPR_U32(ctx, 4), chan = GPR_U32(ctx, 6), index = GPR_U32(ctx, 7);
        if (!d2c::d2InRam(out, 16u) || !d2c::d2InRam(chan, 0x20u))
            return false;
        d2c::CurveRegs r;
        r.fcr31 = ctx->fcr31;
        r.a2q = _mm_castsi128_ps(ctx->r[6]);
        r.status = ctx->vu0_status;
        r.setF0 = r.vu1 = r.vu7 = false;
        if (!d2c::curveCore(d1::Mem{rdram}, out, GPR_U64(ctx, 5), chan, index, r))
            return false;
        ++c_d2CurveEval.n;
        if (r.setF0)
            ctx->f[0] = r.f0;
        ctx->fcr31 = r.fcr31;
        if (r.vu7)
            ctx->vu0_vf[7] = r.vf7;
        if (r.vu1)
        {
            ctx->r[6] = _mm_castps_si128(r.vf1);
            ctx->vu0_vf[1] = r.vf1;
            ctx->vu0_q = r.q;
            ctx->vu0_status = r.status;
        }
        SET_GPR_U64(ctx, 2, r.v0);
        return true;
    }

    void curveEvalD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // fastnative off, the stats on (or not read yet): the old body.
        if (!sotc_decomp::fastNativeOn() || g_statsOn != 0 || !curveD2(rdram, ctx))
            return curveEval(rdram, ctx, runtime);
        returnTo(ctx);
    }
    void curveEvalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            curveEvalD1(rdram, ctx, runtime);
        else
            curveEval(rdram, ctx, runtime);
    }

    // motionKeys (partial): the old body's loop on d1::Mem, each channel by the D2 decoder (then the old evalKey, then the
    // guest call: the formats neither decodes).
    void motionKeysD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), motion = GPR_U32(ctx, 5);
        const d1::Mem m{rdram};
        if (!sotc_decomp::fastNativeOn() || g_statsOn != 0 || !inRam(motion, 0x18u) || m.u32(motion + 8u) == 0u)
            return motionKeys(rdram, ctx, runtime);
        ++c_d2MotionKeys.n;
        const int32_t channels = m.s32(motion + 0x14u);
        int32_t n = GPR_S32(ctx, 7);
        const bool fewer = channels < n;
        if (n != channels && fewer)
            n = channels;
        const uint64_t key64 = GPR_U64(ctx, 6);
        if (n <= 0)
        {
            SET_GPR_U64(ctx, 2, fewer ? 1u : 0u);
            return returnTo(ctx);
        }
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0xF0u;
        SET_GPR_U32(ctx, 29, frame);
        for (int32_t i = 0; i < n; ++i)
        {
            const uint32_t formats = m.u32(motion + 8u), chans = m.u32(motion + 0x10u);
            SET_GPR_U32(ctx, 4, out + static_cast<uint32_t>(i) * 16u);
            SET_GPR_U64(ctx, 5, m.get<uint8_t>(formats + static_cast<uint32_t>(i)));
            SET_GPR_S32(ctx, 6, m.s32(chans + static_cast<uint32_t>(i) * 4u));
            SET_GPR_U64(ctx, 7, key64);
            SET_GPR_U32(ctx, 31, 0x010B8290u);
            if (curveD2(rdram, ctx))
                returnTo(ctx);
            else if (!evalKey(rdram, ctx, runtime))
                sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, 0x010B8290u, frame);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
    void motionKeysD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (below): the full body
    void motionKeysSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            motionKeysD3(rdram, ctx, runtime);
        else
            motionKeys(rdram, ctx, runtime);
    }
} // namespace decomp_curve

// ---- Round D3 (d1native): motionKeys (0x010B81F8) partial -> full; curveCore's refusals as a test for callers ----------
// d3CurveTakes: curveCore's own refusals (pointers, the format's decoder, the packed formats' overlap / finite tests, the s16
// key's record) evaluated without writing; d3CurveReadsMiss: none of the words a decode reads (the channel's 0x24 bytes,
// the packed key word, the s16 record) lies in a range the caller writes first. A caller that checks both for every
// decode it makes before its first store knows curveCore will take each one (the same tests on the same words), so it
// calls curveCore directly with no guest fallback (decomp_hot46h's D3 bodies; motionKeys below).
// motionKeys: every channel tested first (formats, channels, the motion's header and the decodes' words outside the out
// array); then the old body's loop with each channel's key by curveCore (calls direct n), its leftovers chained from one
// channel to the next as the old calls leave them (fcr31, the status, a2's upper half; f0 / vf7 / vf1 / Q as the last
// decode that sets them). The adapter writes a0..a3, v0, f0, fcr31, vf1 / a2 (128 bits), vf7, Q, the status, sp / ra as
// the old body leaves them. Any channel curveCore would refuse (format 13, a recompiled format, an overlap, a non-finite
// word), fastnative off, SOTC_CURVE_STATS, a motion without its formats: the old body, before anything is written.
namespace decomp_curve
{
    D1_COUNTER(c_d3MotionKeys, "motionKeys");

    namespace d2c
    {
        bool d3CurveTakes(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index)
        {
            if (!d2InRam(out, 16u) || !d2InRam(chan, 0x20u))
                return false;
            const uint32_t target = format < 15u ? m.u32(kTable + static_cast<uint32_t>(format) * 4u) : 0x010AC700u;
            switch (target)
            {
            case 0x010AD028u:
            case 0x010AC700u:
                return true;
            case 0x010AC978u:
            case 0x010ACAB0u:
            case 0x010ACBE8u:
            case 0x010ACD20u:
            {
                const uint32_t key = m.u32(chan + 0x20u) + (index << 2);
                return d2InRam(key, 4u) && !d2Overlaps(out, 16u, chan, 0x20u) && !d2Overlaps(out, 16u, key, 4u) && !anyNonFinite8(m.at<uint8_t>(chan));
            }
            case 0x010AC8A8u:
            {
                const uint32_t rec = m.u32(chan + 0x18u) + index * 6u;
                return d2InRam(rec, 6u) && !anyNonFinite8(m.at<uint8_t>(chan));
            }
            default:
                return false;
            }
        }
        bool d3CurveReadsMiss(const d1::Mem &m, uint32_t chan, uint32_t index, uint32_t lo, uint32_t hi)
        {
            const uint32_t n = hi - lo;
            if (d2Overlaps(chan & ~0xFu, (chan - (chan & ~0xFu)) + 0x24u, lo, n))
                return false;
            const uint32_t key = m.u32(chan + 0x20u) + (index << 2), rec = m.u32(chan + 0x18u) + index * 6u;
            return !(d2InRam(key, 4u) && d2Overlaps(key, 4u, lo, n)) && !(d2InRam(rec, 6u) && d2Overlaps(rec, 6u, lo, n));
        }

        struct D3KeysLast
        {
            uint32_t a0, chan;
            uint64_t a1;
            bool vu1; // the last channel's decode set vf1 / a2
        };
        // false: a channel curveCore would refuse, or a word read inside the out array (nothing written).
        bool d3MotionKeysCore(const d1::Mem &m, uint32_t out, uint32_t motion, int32_t n, uint64_t key64, __m128 a2In, CurveRegs &acc, D3KeysLast &last)
        {
            const uint32_t index = static_cast<uint32_t>(key64);
            const uint32_t lo = out, hi = out + static_cast<uint32_t>(n) * 16u;
            if (n > 0x100000 || !d2InRam(out, static_cast<uint32_t>(n) * 16u) || d2Overlaps(motion + 8u, 0xCu, lo, hi - lo))
                return false;
            {
                const uint32_t formats = m.u32(motion + 8u), chans = m.u32(motion + 0x10u);
                if (!d2InRam(formats, static_cast<uint32_t>(n)) || !d2InRam(chans, static_cast<uint32_t>(n) * 4u) || d2Overlaps(formats, static_cast<uint32_t>(n), lo, hi - lo) ||
                    d2Overlaps(chans, static_cast<uint32_t>(n) * 4u, lo, hi - lo))
                    return false;
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t chan = m.u32(chans + static_cast<uint32_t>(i) * 4u);
                    const uint64_t fmt = m.get<uint8_t>(formats + static_cast<uint32_t>(i));
                    if (!d3CurveTakes(m, out + static_cast<uint32_t>(i) * 16u, fmt, chan, index) || !d3CurveReadsMiss(m, chan, index, lo, hi))
                        return false;
                }
            }
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t formats = m.u32(motion + 8u), chans = m.u32(motion + 0x10u);
                const uint32_t o = out + static_cast<uint32_t>(i) * 16u;
                const uint64_t fmt = m.get<uint8_t>(formats + static_cast<uint32_t>(i));
                const uint32_t chan = m.u32(chans + static_cast<uint32_t>(i) * 4u);
                CurveRegs r;
                r.fcr31 = acc.fcr31;
                r.status = acc.status;
                r.a2q = a2In;
                const uint64_t low = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(chan)));
                std::memcpy(&r.a2q, &low, 8); // SET_GPR_S32(a2, chan): the low 64 bits, the upper kept
                r.setF0 = r.vu1 = r.vu7 = false;
                curveCore(m, o, fmt, chan, index, r); // (taken: tested above)
                if (r.setF0)
                {
                    acc.f0 = r.f0;
                    acc.setF0 = true;
                }
                acc.fcr31 = r.fcr31;
                if (r.vu7)
                {
                    acc.vf7 = r.vf7;
                    acc.vu7 = true;
                }
                if (r.vu1)
                {
                    acc.vf1 = r.vf1;
                    acc.q = r.q;
                    acc.status = r.status;
                    acc.vu1 = true;
                }
                acc.v0 = r.v0;
                last.a0 = o;
                last.a1 = fmt;
                last.chan = chan;
                last.vu1 = r.vu1;
            }
            return true;
        }
    } // namespace d2c

    // curveEval's default state for the D3 callers (decomp_hot46h): fastnative on, SOTC_CURVE_STATS read and off.
    bool d3CurveDefault() { return sotc_decomp::fastNativeOn() && g_statsOn == 0; }

    void motionKeysD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), motion = GPR_U32(ctx, 5);
        const d1::Mem m{rdram};
        if (!sotc_decomp::fastNativeOn() || g_statsOn != 0 || !inRam(motion, 0x18u) || m.u32(motion + 8u) == 0u)
            return motionKeys(rdram, ctx, runtime);
        const int32_t channels = m.s32(motion + 0x14u);
        int32_t n = GPR_S32(ctx, 7);
        const bool fewer = channels < n;
        if (n != channels && fewer)
            n = channels;
        const uint64_t key64 = GPR_U64(ctx, 6);
        if (n <= 0)
        {
            ++c_d3MotionKeys.n;
            SET_GPR_U64(ctx, 2, fewer ? 1u : 0u);
            return returnTo(ctx);
        }
        d2c::CurveRegs acc;
        acc.fcr31 = ctx->fcr31;
        acc.status = ctx->vu0_status;
        acc.setF0 = acc.vu1 = acc.vu7 = false;
        acc.v0 = 0u;
        d2c::D3KeysLast last;
        if (!d2c::d3MotionKeysCore(m, out, motion, n, key64, _mm_castsi128_ps(ctx->r[6]), acc, last))
            return motionKeys(rdram, ctx, runtime); // (nothing written)
        ++c_d3MotionKeys.n;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        SET_GPR_U32(ctx, 4, last.a0);
        SET_GPR_U64(ctx, 5, last.a1);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(last.chan));
        SET_GPR_U64(ctx, 7, key64);
        if (acc.setF0)
            ctx->f[0] = acc.f0;
        ctx->fcr31 = acc.fcr31;
        if (acc.vu7)
            ctx->vu0_vf[7] = acc.vf7;
        if (acc.vu1)
        {
            ctx->vu0_vf[1] = acc.vf1;
            ctx->vu0_q = acc.q;
            ctx->vu0_status = acc.status;
        }
        if (last.vu1)
            ctx->r[6] = _mm_castps_si128(acc.vf1);
        SET_GPR_U64(ctx, 2, acc.v0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
} // namespace decomp_curve
