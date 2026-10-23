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

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

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
        static const bool on = std::getenv("SOTC_CURVE_STATS") != nullptr;
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
    // recompiled code takes.
    bool evalKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t out = GPR_U32(ctx, 4), chan = GPR_U32(ctx, 6), index = GPR_U32(ctx, 7);
        const uint64_t format = GPR_U64(ctx, 5);
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

SOTC_DECOMP_AB_FUNCTION(0x010AC6C8u, curveEval, decomp_curve::curveEval);
SOTC_DECOMP_AB_FUNCTION(0x010B81F8u, motionKeys, decomp_curve::motionKeys);
