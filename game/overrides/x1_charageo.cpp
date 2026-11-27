// Round X1 (SOTC_X1=1, off by default; docs/decomp_x1.md, x1_chara.h): ExecCharaGeometry (0x10bf8f0) as native C++.
//
// What the body is (phases as decomp_charageo.cpp numbers them): a character's frame geometry. c = the character class
// (a0 + 0x90 for object types 9 / 0x16). The buffer pair flips (+0x1130); with a motion (+0x1c0): the collision sphere
// (the motion resource's own sphere, GetMotionColSphereInfo through the root, or +0x3b0 turned by +0x490) blended into
// +0xe10 by +0x73c and given to the override callback or +0xe00; the option nodes and the root dynamics; +0x460 = the
// position (+0x134) minus the root (+0x4e0) turned by the root quaternion (+0x13c); CalcNodeInterp, CalcRootFitting,
// the sub geometries; the node quaternions (+0x2e8, +0x1b4 of them) normalised; IK, the root nodes, the sub geometries;
// +0x470 = the root quaternion (turned by +0x490 and slerped into +0x480 by +0x724 when the class's +0x38 bit 3 is set);
// the gaze into +0x710 / +0x590; the root dynamics' second half, the gaze motions, the pose (GetMotionMatrix), the matrix
// pair (+0x344 / +0x348), UpdateLayoutClassGeometry and the bone matrices +0x160 = +0x15c x +0x2d8, +0x450, the child
// nodes, again for the past geometry (+0x10a0); the queued callbacks (+0x550); then the cloth and the anime tables.
//
// X1 writes it as host code over the class: fields read through a host pointer once, VU0 math on host vectors (the same
// NEON ops, operand / result clamps and order as the recompiled code: the bits are the same), the VU0 / FPU registers
// written to the context only where the original leaves them at a guest call or at the exit, the node quaternions four
// dot products at a time, the bone products with the next bones' lines prefetched. The guest calls, their arguments,
// the frame slots and the callee-saved registers at each call are the original's.
//
// SOTC_X1_CHECK=1: X1's run is recorded segment by segment (a segment: from one guest call to the next; the bytes it
// read, the bytes it wrote, the context at each call), then the old body (a copy of decomp_charageo's, unchanged
// statements) replays the same call on a copy of the entry context against the record, its writes kept in a shadow,
// never in RAM; at each guest call and at the exit the two are compared. [x1chk] every 600 frames.
#include "x1_chara.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "d1_native.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/native_tuning.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>

namespace dc1mot
{
    void optionNodesCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t chara, uint32_t sp);
}
namespace s8cloth
{
    bool lastCloth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t lc, uint32_t ra, uint32_t sp);
}

namespace x1geo
{
    using sotc_decomp::relocated;

    D1_COUNTER(c_calls, "x1.ExecCharaGeometry");
    D1_COUNTER(c_nodes, "x1.node quaternions");
    D1_COUNTER(c_bones, "x1.bone products");
    D1_COUNTER(c_old, "x1.old body (rare state)");

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

    // ---- the shadow: bytes by 16-byte line ---------------------------------------------------------------------------
    // X1's side keeps the before-image of each byte it writes (check mode); the old body's side keeps its writes (they
    // never reach RAM) and reads through them.
    struct Shadow
    {
        struct Line
        {
            uint8_t b[16];
            uint16_t valid;
        };
        std::unordered_map<uint32_t, Line> lines;
        void clear() { lines.clear(); }
        void before(const uint8_t *ram, uint32_t a, uint32_t n)
        {
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint32_t x = (a + i) & PS2_RAM_MASK;
                Line &l = lines[x >> 4];
                const uint16_t bit = static_cast<uint16_t>(1u << (x & 15u));
                if (!(l.valid & bit))
                {
                    l.b[x & 15u] = ram[x];
                    l.valid |= bit;
                }
            }
        }
        void write(uint32_t a, const void *src, uint32_t n)
        {
            const uint8_t *s = static_cast<const uint8_t *>(src);
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint32_t x = (a + i) & PS2_RAM_MASK;
                Line &l = lines[x >> 4];
                l.b[x & 15u] = s[i];
                l.valid |= static_cast<uint16_t>(1u << (x & 15u));
            }
        }
        void read(const uint8_t *ram, uint32_t a, void *dst, uint32_t n) const
        {
            uint8_t *d = static_cast<uint8_t *>(dst);
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint32_t x = (a + i) & PS2_RAM_MASK;
                d[i] = ram[x];
                if (!lines.empty())
                {
                    const auto it = lines.find(x >> 4);
                    if (it != lines.end() && (it->second.valid & (1u << (x & 15u))))
                        d[i] = it->second.b[x & 15u];
                }
            }
        }
    };

    // ---- guest memory: direct, or recorded (check mode: every byte read before X1 wrote it, every byte written) -----
    struct Check;
    void recRead(Check *k, uint32_t a, uint32_t n);
    void recWrite(Check *k, uint32_t a, uint32_t n);
    template <bool Log>
    struct Mem
    {
        uint8_t *base;
        Check *log;
        PS2_FORCE_INLINE uint8_t *p(uint32_t a) const { return base + (a & PS2_RAM_MASK); }
        PS2_FORCE_INLINE void noteRead(uint32_t a, uint32_t n) const
        {
            if constexpr (Log)
                recRead(log, a, n);
        }
        PS2_FORCE_INLINE void note(uint32_t a, uint32_t n) const
        {
            if constexpr (Log)
                recWrite(log, a, n);
        }
        template <class T>
        PS2_FORCE_INLINE T get(uint32_t a) const
        {
            noteRead(a, sizeof(T));
            T v;
            std::memcpy(&v, p(a), sizeof(T));
            return v;
        }
        PS2_FORCE_INLINE uint32_t u32(uint32_t a) const { return get<uint32_t>(a); }
        PS2_FORCE_INLINE int32_t s32(uint32_t a) const { return get<int32_t>(a); }
        PS2_FORCE_INLINE int16_t s16(uint32_t a) const { return get<int16_t>(a); }
        PS2_FORCE_INLINE uint8_t u8(uint32_t a) const { return get<uint8_t>(a); }
        PS2_FORCE_INLINE int8_t s8(uint32_t a) const { return get<int8_t>(a); }
        PS2_FORCE_INLINE float f(uint32_t a) const { return get<float>(a); }
        PS2_FORCE_INLINE uint64_t u64(uint32_t a) const { return get<uint64_t>(a); }
        // lq / sq: the address's low four bits ignored (as the Ram of decomp_charageo)
        PS2_FORCE_INLINE __m128 q(uint32_t a) const
        {
            noteRead(a & ~0xFu, 16);
            __m128 v;
            std::memcpy(&v, base + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        PS2_FORCE_INLINE void putq(uint32_t a, __m128 v) const
        {
            note(a & ~0xFu, 16);
            std::memcpy(base + (a & PS2_RAM_MASK & ~0xFu), &v, 16);
        }
        PS2_FORCE_INLINE void put32(uint32_t a, uint32_t v) const
        {
            note(a, 4);
            std::memcpy(p(a), &v, 4);
        }
        PS2_FORCE_INLINE void put64(uint32_t a, uint64_t v) const
        {
            note(a, 8);
            std::memcpy(p(a), &v, 8);
        }
        PS2_FORCE_INLINE void putf(uint32_t a, float v) const { put32(a, fBits(v)); }
        PS2_FORCE_INLINE void put8(uint32_t a, uint8_t v) const
        {
            note(a, 1);
            *p(a) = v;
        }
    };

    // ---- VU0 ops on host vectors (the recompiled code's ops and clamps, no context) ----------------------------------
    template <int i>
    PS2_FORCE_INLINE __m128 bc(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(i, i, i, i)); }
    PS2_FORCE_INLINE __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    PS2_FORCE_INLINE __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }
    // the lanes of b where the mask is set, a elsewhere
    PS2_FORCE_INLINE __m128 sel(__m128 a, __m128 b, int x, int y, int z, int w)
    {
        return _mm_blendv_ps(a, b, _mm_castsi128_ps(_mm_setr_epi32(x ? -1 : 0, y ? -1 : 0, z ? -1 : 0, w ? -1 : 0)));
    }
    PS2_FORCE_INLINE __m128 vf0() { return _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f); }
    // qmtc2 of a GPR mfc1 just wrote: lane x the bits, the others zero
    PS2_FORCE_INLINE __m128 laneX(uint32_t bits) { return _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int>(bits))); }
    PS2_FORCE_INLINE uint32_t laneXBits(__m128 v) { return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v))); }

    // The quaternion q's rotation rows built as the original builds them (vf12 .. vf17, ACC), then p turned by them plus
    // add x p.w: rotate0 (add = vf0, ps2VuMulK<1, 0>) / rotate18 (add = vf18, PS2_VMUL). Writes the registers as the
    // original leaves them: vf8, vf10, vf11 whole; vf12 / vf13 / vf17 x y z (their w stays); vf14 - vf16 whole; ACC.
    template <bool Add18>
    PS2_FORCE_INLINE __m128 rotate(R5900Context *ctx, __m128 v8, __m128 v11in, __m128 v18)
    {
        const __m128 V0 = vf0();
        const __m128 v12 = ps2VuAddK<1, 1>(V0, bc<3>(V0)); // x y z 1 (w: 2, not kept)
        __m128 acc = ps2VuMulK<0, 1>(yzx(v11in), zxy(v12));
        const __m128 v11 = sel(v11in, ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(V0, V0)), 1, 1, 1, 0);
        const __m128 v10a = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(v11, v12));
        const __m128 v13 = ps2VuMulK<1, 1>(v11, v10a);
        acc = ps2VuMulK<1, 1>(yzx(v11), zxy(v10a));
        const __m128 w11 = bc<3>(v11);
        const __m128 a15 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(v10a, w11));
        const __m128 a16 = ps2VuSubK<1, 1>(acc, ps2VuMulK<1, 0>(v10a, w11));
        acc = ps2VuMulK<1, 1>(yzx(v13), zxy(v12));
        const __m128 a17 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(v13, v12));
        acc = ps2VuMulK<1, 1>(yzx(a15), zxy(v12));
        const __m128 b15 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 1>(V0, V0));
        const __m128 a14 = ps2VuSubK<1, 1>(v12, a17);
        const __m128 zero = _mm_setzero_ps();
        // the vmove's: v17.yz = v14.yz; v14.y = v16.y; v16.y = v15.y; v15.y = v17.y; v14.z = v15.z; v15.z = v16.z;
        // v16.z = v17.z (w: vf0.x, 0)
        const __m128 n14 = sel(sel(sel(a14, a16, 0, 1, 0, 0), b15, 0, 0, 1, 0), zero, 0, 0, 0, 1);
        const __m128 n15 = sel(sel(sel(b15, a14, 0, 1, 0, 0), a16, 0, 0, 1, 0), zero, 0, 0, 0, 1);
        const __m128 n16 = sel(sel(sel(a16, b15, 0, 1, 0, 0), a14, 0, 0, 1, 0), zero, 0, 0, 0, 1);
        const __m128 n17 = sel(a17, a14, 0, 1, 1, 0);
        acc = ps2VuMulK<1, 0>(n14, bc<0>(v8));
        acc = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(n15, bc<1>(v8)));
        acc = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(n16, bc<2>(v8)));
        __m128 v10;
        if constexpr (Add18)
            v10 = ps2VuAddK<1, 1>(acc, PS2_VMUL(v18, bc<3>(v8)));
        else
            v10 = ps2VuAddK<1, 1>(acc, ps2VuMulK<1, 0>(V0, bc<3>(v8)));
        __m128 *vf = ctx->vu0_vf;
        vf[8] = v8;
        vf[10] = v10;
        vf[11] = v11;
        vf[12] = sel(vf[12], v12, 1, 1, 1, 0);
        vf[13] = sel(vf[13], v13, 1, 1, 1, 0);
        vf[14] = n14;
        vf[15] = n15;
        vf[16] = n16;
        vf[17] = sel(vf[17], n17, 1, 1, 1, 0);
        if constexpr (Add18)
            vf[18] = v18;
        ctx->vu0_acc = acc;
        return v10;
    }

    // vf9 = vf5 x t + vf6 x (1 - t) (t = vf7.x, from laneX): writes vf5 - vf7, vf8.w, vf9, ACC.
    PS2_FORCE_INLINE __m128 blend(R5900Context *ctx, __m128 v5, __m128 v6, uint32_t tBits)
    {
        const __m128 V0 = vf0(), v7 = laneX(tBits);
        const __m128 v8w = ps2VuSubK<1, 0>(V0, bc<0>(v7));
        const __m128 acc = PS2_VMUL(v5, bc<0>(v7));
        const __m128 v9 = ps2VuAddK<1, 1>(acc, ps2VuMulK<0, 1>(v6, bc<3>(v8w)));
        __m128 *vf = ctx->vu0_vf;
        vf[5] = v5;
        vf[6] = v6;
        vf[7] = v7;
        vf[8] = sel(vf[8], v8w, 0, 0, 0, 1);
        vf[9] = v9;
        ctx->vu0_acc = acc;
        return v9;
    }

    // vf13 = vf11 x vf12 (quaternions): writes vf11, vf12, vf13 whole, vf14 - vf16 / ACC x y z.
    PS2_FORCE_INLINE __m128 qmul(R5900Context *ctx, __m128 v11, __m128 v12)
    {
        __m128 t = PS2_VMUL(v11, v12);
        t = sel(t, ps2VuAddK<1, 1>(t, bc<1>(t)), 1, 0, 0, 0);
        t = sel(t, ps2VuAddK<1, 1>(t, bc<2>(t)), 1, 0, 0, 0);
        t = sel(t, ps2VuSubK<1, 1>(t, bc<0>(t)), 0, 0, 0, 1);
        const __m128 v14 = PS2_VMUL(v12, bc<3>(v11));
        const __m128 v15 = PS2_VMUL(v11, bc<3>(v12));
        const __m128 acc = PS2_VMUL(yzx(v12), zxy(v11));
        const __m128 v16 = ps2VuSubK<1, 1>(acc, PS2_VMUL(yzx(v11), zxy(v12)));
        t = sel(t, ps2VuAddK<1, 1>(v14, v15), 1, 1, 1, 0);
        t = sel(t, ps2VuAddK<1, 1>(t, v16), 1, 1, 1, 0);
        __m128 *vf = ctx->vu0_vf;
        vf[11] = v11;
        vf[12] = v12;
        vf[13] = t;
        vf[14] = sel(vf[14], v14, 1, 1, 1, 0);
        vf[15] = sel(vf[15], v15, 1, 1, 1, 0);
        vf[16] = sel(vf[16], v16, 1, 1, 1, 0);
        ctx->vu0_acc = sel(ctx->vu0_acc, acc, 1, 1, 1, 0);
        return t;
    }

    // vf15.x = vf14 . vf15 (the products in y z w): writes vf14, vf15; the dot's bits.
    PS2_FORCE_INLINE uint32_t dot(R5900Context *ctx, __m128 v14, __m128 v15in)
    {
        __m128 v15 = PS2_VMUL(v14, v15in);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<1>(v15)), 1, 0, 0, 0);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<2>(v15)), 1, 0, 0, 0);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<3>(v15)), 1, 0, 0, 0);
        ctx->vu0_vf[14] = v14;
        ctx->vu0_vf[15] = v15;
        return laneXBits(v15);
    }

    // The quaternion at a normalised in place (vrsqrt Q, vf0.w / its squared length): writes vf14, vf15, Q, status.
    template <class M>
    PS2_FORCE_INLINE void norm(R5900Context *ctx, const M &m, uint32_t a)
    {
        const __m128 v14 = m.q(a);
        __m128 v15 = PS2_VMUL(v14, v14);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<1>(v15)), 1, 0, 0, 0);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<2>(v15)), 1, 0, 0, 0);
        v15 = sel(v15, ps2VuAddK<1, 1>(v15, bc<3>(v15)), 1, 0, 0, 0);
        const float q = ps2VuRsqrt(ctx->vu0_status, 1.0f, _mm_cvtss_f32(v15));
        ctx->vu0_q = q;
        v15 = PS2_VMUL(v14, _mm_set1_ps(q));
        ctx->vu0_vf[14] = v14;
        ctx->vu0_vf[15] = v15;
        m.putq(a, v15);
    }

    // The quaternion at a negated in place (f0 = -1, vf6 = laneX(-1), vf5 x vf6.x): writes f0, vf5, vf6.
    template <class M>
    PS2_FORCE_INLINE void negate(R5900Context *ctx, const M &m, uint32_t a)
    {
        ctx->f[0] = bitsF(0xBF800000u);
        const __m128 v6 = laneX(0xBF800000u);
        const __m128 v5 = PS2_VMUL(m.q(a), bc<0>(v6));
        ctx->vu0_vf[5] = v5;
        ctx->vu0_vf[6] = v6;
        m.putq(a, v5);
    }

    // ---- the node quaternions (+0x2e8, n of them): normalised in place ------------------------------------------------
    // Per node: s = x x x + y x y + z x z + w x w (PS2_VMUL, then ps2VuAddK x + y, + z, + w in lane x), Q = VU0's rsqrt of
    // 1 / s (the status in node order), v x Q. Four nodes' dot products at a time on transposed lanes (W2's mapping: the
    // same lane ops, so the same bits); the rsqrt and the scale per node in order. Leaves vf14 (the last node), vf15 (its
    // result), Q, the status as the per-node loop does.
    PS2_FORCE_INLINE void transpose(float32x4_t &a, float32x4_t &b, float32x4_t &c, float32x4_t &d)
    {
        const float32x4x2_t ab = vtrnq_f32(a, b), cd = vtrnq_f32(c, d);
        a = vcombine_f32(vget_low_f32(ab.val[0]), vget_low_f32(cd.val[0]));
        b = vcombine_f32(vget_low_f32(ab.val[1]), vget_low_f32(cd.val[1]));
        c = vcombine_f32(vget_high_f32(ab.val[0]), vget_high_f32(cd.val[0]));
        d = vcombine_f32(vget_high_f32(ab.val[1]), vget_high_f32(cd.val[1]));
    }
    template <bool Log>
    void nodeQuats(R5900Context *ctx, const Mem<Log> &m, uint32_t q, int32_t count)
    {
        __m128 l14 = ctx->vu0_vf[14], l15 = ctx->vu0_vf[15];
        float qq = ctx->vu0_q;
        uint16_t status = ctx->vu0_status;
        const float fs = 1.0f; // vf0.w
        int32_t i = 0;
        const uint32_t at = q & PS2_RAM_MASK & ~0xFu;
        if (count >= 4 && count <= 4096 && at + static_cast<uint32_t>(count) * 16u <= PS2_RAM_SIZE && (q & 0xFu) == 0u)
        {
            m.noteRead(at, static_cast<uint32_t>(count) * 16u);
            m.note(at, static_cast<uint32_t>(count) * 16u);
            uint8_t *const base = m.base + at;
            for (; i + 4 <= count; i += 4)
            {
                uint8_t *const p = base + static_cast<uint32_t>(i) * 16u;
                if (i + 8 <= count)
                    __builtin_prefetch(p + 64);
                __m128 v[4];
                for (int k = 0; k < 4; ++k)
                    std::memcpy(&v[k], p + 16 * k, 16);
                float32x4_t x = v[0], y = v[1], z = v[2], w = v[3];
                transpose(x, y, z, w);
                const __m128 s = ps2VuAddK<1, 1>(ps2VuAddK<1, 1>(ps2VuAddK<1, 1>(PS2_VMUL(x, x), PS2_VMUL(y, y)), PS2_VMUL(z, z)), PS2_VMUL(w, w));
                float sl[4];
                vst1q_f32(sl, s);
                for (int k = 0; k < 4; ++k)
                {
                    qq = ps2VuRsqrt(status, fs, sl[k]);
                    const __m128 r = PS2_VMUL(v[k], _mm_set1_ps(qq));
                    std::memcpy(p + 16 * k, &r, 16);
                    l15 = r;
                }
                l14 = v[3];
            }
        }
        for (; i < count; ++i)
        {
            const uint32_t a = q + static_cast<uint32_t>(i) * 16u;
            l14 = m.q(a);
            __m128 s = PS2_VMUL(l14, l14);
            s = sel(s, ps2VuAddK<1, 1>(s, bc<1>(s)), 1, 0, 0, 0);
            s = sel(s, ps2VuAddK<1, 1>(s, bc<2>(s)), 1, 0, 0, 0);
            s = sel(s, ps2VuAddK<1, 1>(s, bc<3>(s)), 1, 0, 0, 0);
            qq = ps2VuRsqrt(status, fs, _mm_cvtss_f32(s));
            l15 = PS2_VMUL(l14, _mm_set1_ps(qq));
            m.putq(a, l15);
        }
        ctx->vu0_vf[14] = l14;
        ctx->vu0_vf[15] = l15;
        ctx->vu0_q = qq;
        ctx->vu0_status = status;
        c_nodes.n += static_cast<uint64_t>(count);
    }

    // ---- the bone matrices: out[i] = a[i] x b[i] (the posed bone, scaled, times its inverse bind) ------------------------
    // pose82::boneProducts' NEON chain (the same intrinsics in the same order: the same bits), with the lines of the
    // bones three ahead asked for (the A9's hardware prefetcher is off on the Vita). False: a range outside main RAM or
    // overlapping (the old clamped loop then).
    inline bool boneRange(uint32_t a, uint32_t bytes) { return (a & 0xFu) == 0u && a >= 0x1000u && a + bytes <= 0x02000000u && a + bytes > a; }
    template <bool Log>
    bool boneProducts(const Mem<Log> &m, uint32_t out, uint32_t a, uint32_t b, int count)
    {
        const uint32_t bytes = static_cast<uint32_t>(count) * 0x40u;
        if (count <= 0 || count > 1024 || !boneRange(out, bytes) || !boneRange(a, bytes) || !boneRange(b, bytes))
            return false;
        auto overlaps = [bytes](uint32_t x, uint32_t y) { return x < y + bytes && y < x + bytes; };
        if (overlaps(out, a) || overlaps(out, b))
            return false;
        m.noteRead(a, bytes);
        m.noteRead(b, bytes);
        m.note(out, bytes);
        const float *pa = reinterpret_cast<const float *>(m.p(a)), *pb = reinterpret_cast<const float *>(m.p(b));
        float *po = reinterpret_cast<float *>(m.p(out));
        for (int i = 0; i < count; ++i, pa += 16, pb += 16, po += 16)
        {
            if (i + 3 < count)
            {
                __builtin_prefetch(pa + 48);
                __builtin_prefetch(pa + 56);
                __builtin_prefetch(pb + 48);
                __builtin_prefetch(pb + 56);
            }
            const float32x4_t r0 = vld1q_f32(pa), r1 = vld1q_f32(pa + 4), r2 = vld1q_f32(pa + 8), r3 = vld1q_f32(pa + 12);
            for (int k = 0; k < 4; ++k)
            {
                const float32x4_t c = vld1q_f32(pb + 4 * k);
                float32x4_t v = vmulq_lane_f32(r0, vget_low_f32(c), 0);
                v = vmlaq_lane_f32(v, r1, vget_low_f32(c), 1);
                v = vmlaq_lane_f32(v, r2, vget_high_f32(c), 0);
                v = vmlaq_lane_f32(v, r3, vget_high_f32(c), 1);
                vst1q_f32(po + 4 * k, v);
            }
        }
        c_bones.n += static_cast<uint64_t>(count);
        return true;
    }

    // ---- the check (SOTC_X1_CHECK=1): X1 recorded, then the old body replayed against the record ------------------------
    // While X1 runs (for real: its writes and its guest calls are the game's), each segment (from the entry or a guest
    // call's return to the next guest call or the exit) records every byte it reads before writing it (the value then)
    // and every byte it writes (the value before, and at the segment's end the value it left), and the whole context at
    // each guest call (before it, and after it returns). Then the old body (x1_charageo_ref.inc) runs on a copy of the
    // entry context, on the same thread: its reads see its own writes of the segment, then the segment's recorded
    // values; at each of its guest calls its context, the call's site and target and every byte it wrote in the segment
    // are compared with X1's, and its context is set to what X1's call left. No host thread, no second run of a callee.
    // A byte the old body reads that X1 neither read nor wrote in the segment cannot be checked: counted (unrecorded).
    struct Check
    {
        struct Line
        {
            uint8_t rv[16]; // the value before the segment wrote it (read first or written first)
            uint8_t wv[16]; // the value X1 left (at the segment's end)
            uint16_t rmask = 0, wmask = 0;
        };
        struct Seg
        {
            std::unordered_map<uint32_t, Line> lines;
            std::vector<uint8_t> pre, post; // the context before the call (or at the exit), after it
            uint32_t site = 0, target = 0;
            bool exit = false;
        };
        std::vector<Seg> segs;
        size_t cur = 0, used = 0;
        std::vector<uint8_t> entry;
        uint8_t *rdram = nullptr;
        uint64_t calls = 0, boundaries = 0, bytes = 0, bad = 0, from = 0, skipped = 0, unrecorded = 0;
        bool active = false, abiOnly = false;
        const char *name = "body";
        void (*ref)(Check &) = nullptr; // the replayed old code

        static std::vector<uint8_t> bytesOf(const R5900Context *ctx)
        {
            const uint8_t *b = reinterpret_cast<const uint8_t *>(ctx);
            return std::vector<uint8_t>(b, b + sizeof(R5900Context));
        }
        Seg &seg() { return segs[cur]; }
        void read(uint32_t a, uint32_t n)
        {
            Seg &s = seg();
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint32_t x = (a + i) & PS2_RAM_MASK;
                Line &l = s.lines[x >> 4];
                const uint16_t bit = static_cast<uint16_t>(1u << (x & 15u));
                if (!((l.rmask | l.wmask) & bit))
                {
                    l.rv[x & 15u] = rdram[x];
                    l.rmask |= bit;
                }
            }
        }
        void write(uint32_t a, uint32_t n)
        {
            Seg &s = seg();
            for (uint32_t i = 0; i < n; ++i)
            {
                const uint32_t x = (a + i) & PS2_RAM_MASK;
                Line &l = s.lines[x >> 4];
                const uint16_t bit = static_cast<uint16_t>(1u << (x & 15u));
                if (!((l.rmask | l.wmask) & bit))
                {
                    l.rv[x & 15u] = rdram[x];
                    l.rmask |= bit;
                }
                l.wmask |= bit;
            }
        }
        void open()
        {
            if (used == segs.size())
                segs.emplace_back();
            cur = used++;
            Seg &s = segs[cur];
            s.lines.clear();
            s.exit = false;
            s.site = s.target = 0;
        }
        void close(const R5900Context *ctx, uint32_t site, uint32_t target, bool exit)
        {
            Seg &s = seg();
            for (auto &l : s.lines)
                for (uint32_t b = 0; b < 16u; ++b)
                    if (l.second.wmask & (1u << b))
                        l.second.wv[b] = rdram[(l.first << 4) | b];
            s.pre = bytesOf(ctx);
            s.site = site;
            s.target = target;
            s.exit = exit;
        }
        // X1's side
        void begin(uint8_t *ram, const R5900Context *ctx)
        {
            rdram = ram;
            entry = bytesOf(ctx);
            used = 0;
            open();
            ++calls;
        }
        void boundary(const R5900Context *ctx, uint32_t s, uint32_t t) { close(ctx, s, t, false); }
        void resume(const R5900Context *ctx)
        {
            seg().post = bytesOf(ctx);
            open();
        }
        void end(const R5900Context *ctx);

        // ---- the old body's side (the replay) ----
        struct Replay
        {
            Check *k;
            Shadow own; // the old body's writes of the current segment
            size_t at = 0;
            bool off = false; // a difference was found: the rest of this call is not compared
            void read(const uint8_t *ram, uint32_t a, void *dst, uint32_t n)
            {
                uint8_t *d = static_cast<uint8_t *>(dst);
                const Seg *s = at < k->used ? &k->segs[at] : nullptr;
                for (uint32_t i = 0; i < n; ++i)
                {
                    const uint32_t x = (a + i) & PS2_RAM_MASK;
                    const auto oi = own.lines.find(x >> 4);
                    if (oi != own.lines.end() && (oi->second.valid & (1u << (x & 15u))))
                    {
                        d[i] = oi->second.b[x & 15u];
                        continue;
                    }
                    if (s)
                    {
                        const auto li = s->lines.find(x >> 4);
                        if (li != s->lines.end() && (li->second.rmask & (1u << (x & 15u))))
                        {
                            d[i] = li->second.rv[x & 15u];
                            continue;
                        }
                    }
                    d[i] = ram[x];
                    if (k->unrecorded++ < 16)
                        std::fprintf(stderr, "[x1chk] %s: segment %u read %08x (%u bytes from %08x) unrecorded\n", k->name, static_cast<unsigned>(at), x, n, a);
                }
            }
            void write(uint32_t a, const void *src, uint32_t n) { own.write(a, src, n); }
        };
        Replay replay{this};
        R5900Context *rc = nullptr; // the old body's context (static storage)
        R5900Context *xc = nullptr; // X1's recorded context, unpacked for the compare

        bool sameReg(const void *a, const void *b, size_t n) { return std::memcmp(a, b, n) == 0; }
        // the old body at a guest call (exit = false) or at its exit
        void refBoundary(uint32_t s, uint32_t t, bool atExit = false)
        {
            if (replay.off)
                return;
            ++boundaries;
            const char *what = nullptr;
            int idx = -1;
            uint32_t badAt = 0;
            const Seg *g = replay.at < used ? &segs[replay.at] : nullptr;
            if (!g || g->exit != atExit || (!atExit && (g->site != s || g->target != t)))
                what = "the next call";
            if (!what)
            {
                std::memcpy(static_cast<void *>(xc), g->pre.data(), sizeof(R5900Context));
                // abiOnly (xz: the recompiled code's scratch registers are not the callees' or the caller's business):
                // the low 64 bits of a0-a3 / t0-t3 (arguments), s0-s7, sp, fp, ra (v0 / v1 too at the exit); f12-f31
                auto abiGpr = [&](int i)
                {
                    return (i >= 4 && i <= 11) || (i >= 16 && i <= 23) || i >= 29 || (atExit && (i == 2 || i == 3));
                };
                for (int i = 0; !what && i < 32; ++i)
                    if (abiOnly ? (abiGpr(i) && !sameReg(&xc->r[i], &rc->r[i], 8)) : !sameReg(&xc->r[i], &rc->r[i], 16))
                        what = "a GPR", idx = i;
                for (int i = abiOnly ? 12 : 0; !what && i < 32; ++i)
                    if (!sameReg(&xc->f[i], &rc->f[i], 4))
                        what = "an FPR", idx = i;
                for (int i = 1; !what && i < 32; ++i)
                    if (!sameReg(&xc->vu0_vf[i], &rc->vu0_vf[i], 16))
                        what = "a VU0 register", idx = i;
                if (!what && !sameReg(&xc->vu0_acc, &rc->vu0_acc, 16))
                    what = "ACC";
                if (!what && !sameReg(&xc->vu0_q, &rc->vu0_q, 4))
                    what = "Q";
                if (!what && xc->vu0_status != rc->vu0_status)
                    what = "the VU0 status";
                if (!what && xc->fcr31 != rc->fcr31)
                    what = "fcr31";
                if (!what && atExit && xc->pc != rc->pc)
                    what = "pc";
            }
            if (!what)
            {
                // every byte either side wrote in the segment: the old body's value (or the one before the segment)
                // against X1's (or the one before the segment)
                auto valueX1 = [&](uint32_t x, bool &known) -> uint8_t
                {
                    known = true;
                    const auto li = g->lines.find(x >> 4);
                    const uint16_t bit = static_cast<uint16_t>(1u << (x & 15u));
                    if (li != g->lines.end() && (li->second.wmask & bit))
                        return li->second.wv[x & 15u];
                    if (li != g->lines.end() && (li->second.rmask & bit))
                        return li->second.rv[x & 15u];
                    known = false;
                    return 0;
                };
                for (const auto &l : replay.own.lines)
                    for (uint32_t b = 0; !what && b < 16u; ++b)
                        if (l.second.valid & (1u << b))
                        {
                            const uint32_t x = (l.first << 4) | b;
                            bool known;
                            const uint8_t xv = valueX1(x, known);
                            ++bytes;
                            if (!known || xv != l.second.b[b])
                                what = "memory (a byte the old body wrote)", badAt = x;
                        }
                for (const auto &l : g->lines)
                    for (uint32_t b = 0; !what && b < 16u; ++b)
                        if (l.second.wmask & (1u << b))
                        {
                            const uint32_t x = (l.first << 4) | b;
                            const auto oi = replay.own.lines.find(l.first);
                            if (oi != replay.own.lines.end() && (oi->second.valid & (1u << b)))
                                continue; // compared above
                            ++bytes;
                            if (l.second.wv[b] != l.second.rv[b])
                                what = "memory (a byte only X1 changed)", badAt = x;
                        }
            }
            if (what)
            {
                if (bad++ < 12)
                    std::fprintf(stderr, "[x1chk] %s MISMATCH (%s %d, at %08x) at the old body's %s %08x -> %08x (X1's segment %u of %u: %08x -> %08x%s)\n", name, what,
                                 idx, badAt, atExit ? "exit" : "call", s + 8u, t, static_cast<unsigned>(replay.at), static_cast<unsigned>(used),
                                 g ? g->site + 8u : 0u, g ? g->target : 0u, g && g->exit ? ", exit" : "");
                replay.off = true;
                return;
            }
            // the call made: its context as X1's call left it
            if (!atExit)
            {
                std::memcpy(static_cast<void *>(rc), g->post.data(), sizeof(R5900Context));
                replay.own.clear();
                ++replay.at;
            }
        }
    };
    void refBody(Check &k);
    void recRead(Check *k, uint32_t a, uint32_t n) { k->read(a, n); }
    void recWrite(Check *k, uint32_t a, uint32_t n) { k->write(a, n); }
    void Check::end(const R5900Context *ctx)
    {
        close(ctx, 0u, 0u, true);
        static R5900Context s_rc, s_xc; // (static storage: aligned; never by-value temporaries)
        rc = &s_rc;
        xc = &s_xc;
        std::memcpy(static_cast<void *>(rc), entry.data(), sizeof(R5900Context));
        replay.own.clear();
        replay.at = 0;
        replay.off = false;
        ref(*this);
        refBoundary(0u, 0u, true);
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (fr >= from + 600u)
        {
            std::fprintf(stderr,
                         "[x1chk] %s frame %llu: %llu calls checked, %llu boundaries compared, %llu bytes compared, %llu mismatches, %llu bytes read unrecorded (%llu not checked: worker / nested)\n",
                         name, (unsigned long long)fr, (unsigned long long)calls, (unsigned long long)boundaries, (unsigned long long)bytes,
                         (unsigned long long)bad, (unsigned long long)unrecorded, (unsigned long long)skipped);
            from = fr;
        }
    }
    Check g_check;

    // Bracket ids (SOTC_P2BRACKET >= 2: the X1 parts; -1 otherwise).
    int g_idNodes = -2, g_idBones = -2;
    inline void bracketIds()
    {
        if (__builtin_expect(g_idNodes == -2, 0))
        {
            g_idNodes = sotc_p2::named("characters:x1Nodes", 2);
            g_idBones = sotc_p2::named("characters:x1Bones", 2);
        }
    }

    // ---- the body ---------------------------------------------------------------------------------------------------
    template <bool Log>
    void body(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Check *chk)
    {
        const Mem<Log> r{rdram, Log ? chk : nullptr};
        const uint32_t obj = GPR_U32(ctx, 4), point = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x110u;
        const bool game = !sotc_decomp::onWorkerLane(runtime);
        auto boundary = [&](uint32_t site, uint32_t target)
        {
            if constexpr (Log)
                chk->boundary(ctx, site, target);
        };
        auto resume = [&]()
        {
            if constexpr (Log)
                chk->resume(ctx);
        };
        auto call = [&](uint32_t site, uint32_t target)
        {
            boundary(site, target);
            sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp);
            resume();
        };
        auto sreg = [&](int reg, uint32_t v) { SET_GPR_S32(ctx, reg, static_cast<int32_t>(v)); };
        auto cond = [&](bool c)
        {
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        };
        auto classOf = [&](uint32_t o) -> uint32_t
        {
            const uint8_t t = r.u8(o + 8u);
            return t == 9u || t == 0x16u ? o + 0x90u : 0u;
        };

        // The prologue's saves (sp0 - 0x110: s0..s7 at +0xb0, fp +0xf0, ra +0xf8, f21 +0x108, f20 +0x100), a1's spill.
        for (int k = 0; k < 8; ++k)
            r.put64(sp + 0xB0u + 8u * k, GPR_U64(ctx, 16 + k));
        r.put64(sp + 0xF0u, GPR_U64(ctx, 30));
        r.put64(sp + 0xF8u, GPR_U64(ctx, 31));
        r.put32(sp + 0x108u, fBits(ctx->f[21]));
        r.put32(sp + 0x100u, fBits(ctx->f[20]));
        r.put32(sp + 0x70u, point);
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4)); // s3 = a0

        // ---- 1: the class, the buffer pair ----
        const uint32_t c = classOf(obj);
        const uint8_t *const C = r.p(c);
        // the class's lines the first phases read (the A9 has no hardware prefetch on the Vita)
        if (c >= 0x1000u && c < 0x01FFE000u)
            for (uint32_t off : {0x120u, 0x140u, 0x1C0u, 0x2E0u, 0x340u, 0x3E0u, 0x420u, 0x460u, 0x480u, 0x4E0u, 0x500u, 0x520u, 0x560u, 0x580u, 0x720u, 0xCE0u, 0xE00u, 0x1120u})
                __builtin_prefetch(C + off);
        sreg(16, c);
        const float f21 = r.f(c + 0xCE0u), f20 = r.f(c + 0xCE4u);
        ctx->f[21] = f21;
        ctx->f[20] = f20;
        const uint32_t list358 = r.u32(c + 0x358u);
        r.put32(sp + 0x74u, c);
        r.put32(sp + 0x7Cu, c + 0x1C0u);
        r.put32(sp + 0x80u, list358);
        {
            const uint8_t b = static_cast<uint8_t>((r.u8(c + 0x1130u) + 1u) & 1u);
            r.put8(c + 0x1130u, b);
            const uint32_t x = r.u32(c + (b ? 0x2F8u : 0x2F4u)), y = r.u32(c + (b ? 0x2F4u : 0x2F8u));
            r.put32(c + 0x34Cu, x);
            r.put32(c + 0x350u, y);
        }
        const uint32_t m0 = r.u32(c + 0x1C0u);
        uint32_t s2 = 0u, mo = 0u;
        bool run = m0 != 0u;
        if (run)
        {
            r.put32(c + 0x578u, m0);
            s2 = r.u32(c + 0x1C0u);
            sreg(18, s2);
            mo = s2 + 0x50u;
            r.put32(sp + 0x84u, mo);
            run = mo != 0u;
        }
        if (run)
        {
            const bool subgeo = r.u32(c + 0x3FCu) != 0u && r.s8(c + 0x52Eu) != 0;
            const bool dyn = ((r.u32(s2 + 0x68u) >> 26) & 1u) == 0u && r.s8(c + 0x53Bu) == 0;
            const uint32_t cls = r.u32(c + 0x1140u);
            const uint32_t s4 = c + 0x4E0u, s5 = c + 0x4F0u, s7 = c + 0x490u;
            const uint32_t sphereKind = r.u32(c + 0x510u);
            const bool gaze = ((r.u32(cls + 0x38u) >> 4) & 1u) != 0u;
            const uint32_t s1v = sp + 0x20u;
            r.put32(sp + 0x9Cu, subgeo ? 1u : 0u);
            r.put32(sp + 0xA0u, dyn ? 1u : 0u);
            r.put32(sp + 0x88u, cls);
            r.put32(sp + 0x90u, sphereKind);
            r.put32(sp + 0x8Cu, gaze ? 1u : 0u);
            sreg(20, s4);
            sreg(21, s5);
            sreg(23, s7);

            // ---- 2: the collision sphere ----
            if (dyn)
            {
                sreg(17, s1v);
                bool found = false;
                if (sphereKind == r.u32(relocated(rdram, 0x010BFA50u, 0x010BFA54u)))
                {
                    const uint32_t h = s2 + 0xCu, hv = r.u32(h), off = hv & 0x3FFFFu;
                    const uint32_t t = off ? r.u32(h - off) : 0u;
                    uint32_t e = 0u;
                    if (t != 0u && t != r.u32(relocated(rdram, 0x010BFA90u, 0x010BFA94u)))
                        e = r.u32(t + 0x10u) + ((hv >> 18) & 0xFFFu) * r.u32(r.u32(t + 8u) + 0xCu);
                    if (e != 0u && ((r.u32(e) >> 4) & 1u) != 0u)
                    {
                        const float e4 = r.f(e + 4u), e8 = r.f(e + 8u), ec = r.f(e + 0xCu), e10 = r.f(e + 0x10u);
                        r.putf(s1v, e4);
                        r.putf(s1v + 4u, e8);
                        r.putf(s1v + 8u, ec);
                        r.putf(s1v + 0xCu, e10);
                        ctx->f[1] = ec;
                        ctx->f[0] = e10;
                        found = true;
                    }
                }
                if (!found)
                {
                    SET_GPR_U32(ctx, 4, sp + 0x20u);
                    SET_GPR_U32(ctx, 5, mo);
                    ctx->f[12] = r.f(c + 0x1C8u);
                    call(0x010BFB20u, 0x010B4968u); // GetMotionColSphereInfo
                    if (GPR_U64(ctx, 2) != 0u)
                    {
                        SET_GPR_U64(ctx, 4, 0u);
                        SET_GPR_U32(ctx, 7, r.u32(s2 + 0x34u));
                        SET_GPR_U32(ctx, 5, sp + 0x20u);
                        SET_GPR_U64(ctx, 6, 0u);
                        SET_GPR_U64(ctx, 8, 0u);
                        call(0x010BFB3Cu, 0x010B7FB0u);
                        const float scale = r.f(c + 0x94u);
                        ctx->f[0] = scale;
                        const __m128 v6a = laneX(fBits(scale));
                        const __m128 v5a = PS2_VMUL(r.q(s1v), bc<0>(v6a));
                        r.putq(s1v, v5a);
                        const __m128 v6b = r.q(s4);
                        const __m128 v5b = r.q(s1v);
                        const __m128 v5 = sel(v5b, PS2_VSUB(v5b, v6b), 1, 1, 1, 0);
                        r.putq(s1v, v5);
                        ctx->vu0_vf[5] = v5;
                        ctx->vu0_vf[6] = v6b;
                    }
                    else
                        r.putq(s1v, rotate<false>(ctx, r.q(c + 0x3B0u), r.q(s7), _mm_setzero_ps()));
                }
                // blended into +0xe10 by +0x73c
                const float t73c = r.f(c + 0x73Cu);
                ctx->f[0] = t73c;
                r.putq(c + 0xE10u, blend(ctx, r.q(s1v), r.q(c + 0xE10u), fBits(t73c)));
                const uint32_t override = r.u32(c + 0x584u);
                if (override != 0u)
                {
                    SET_GPR_U32(ctx, 4, c + 0xE00u);
                    SET_GPR_U32(ctx, 7, r.u32(c + 0x580u));
                    SET_GPR_U32(ctx, 5, obj);
                    SET_GPR_U32(ctx, 6, c + 0xE10u);
                    ctx->f[12] = f21;
                    call(0x010BFC30u, override);
                    if (GPR_U64(ctx, 2) == 0u)
                    {
                        SET_GPR_U32(ctx, 4, obj);
                        SET_GPR_U64(ctx, 5, 0u);
                        SET_GPR_U64(ctx, 6, 0u);
                        call(0x010BFC44u, 0x010C2970u);
                    }
                }
                else
                    r.putq(c + 0xE00u, r.q(c + 0xE10u));
            }

            // ---- 3: the option nodes, the root dynamics ----
            boundary(0x010BFC60u, 0x010BC4F0u);
            dc1mot::optionNodesCall(rdram, ctx, runtime, c, sp);
            resume();
            if (r.u32(c + 0x438u) == 0u)
            {
                ctx->f[0] = 1.0f;
                r.putf(c + 0x724u, 1.0f);
            }
            else if (dyn)
            {
                const uint32_t flags = r.u32(s2 + 0x68u);
                uint32_t a2 = 0u;
                if ((flags >> 19) & 1u)
                    a2 = r.u32(c + 0xE3Cu) != 0u ? 1u : 0u;
                SET_GPR_U64(ctx, 6, a2);
                SET_GPR_U64(ctx, 7, (flags >> 20) & 1u);
                ctx->f[12] = r.f(s2 + 0x70u);
                ctx->f[13] = r.f(c + 0x724u);
                SET_GPR_U32(ctx, 5, sphereKind);
                SET_GPR_U32(ctx, 9, s5);
                SET_GPR_U32(ctx, 4, obj);
                SET_GPR_U32(ctx, 8, s4);
                ctx->f[14] = f21;
                ctx->f[15] = f20;
                call(0x010BFCBCu, 0x010D9FC0u); // GetCharaRootDynamics
            }

            // ---- 4: +0x460, CalcNodeInterp, CalcRootFitting, the sub geometries, the node quaternions, IK ----
            {
                ctx->f[0] = bitsF(0xBF800000u);
                const __m128 v6 = laneX(0xBF800000u);
                const __m128 l4 = r.q(s4);
                const __m128 v5 = sel(l4, PS2_VMUL(l4, bc<0>(v6)), 1, 1, 1, 0);
                r.putq(sp + 0x30u, v5);
                ctx->vu0_vf[5] = v5;
                ctx->vu0_vf[6] = v6;
                const __m128 v18 = sel(r.q(r.u32(c + 0x134u)), vf0(), 0, 0, 0, 1);
                r.putq(c + 0x460u, rotate<true>(ctx, r.q(sp + 0x30u), r.q(r.u32(c + 0x13Cu)), v18));
            }
            SET_GPR_U32(ctx, 4, obj);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            ctx->f[12] = f21;
            call(0x010BFD94u, 0x010BB258u); // CalcNodeInterp
            const uint64_t interp = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(GPR_U32(ctx, 2))));
            r.put32(sp + 0x78u, GPR_U32(ctx, 2));
            SET_GPR_U32(ctx, 4, obj);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            ctx->f[12] = f21;
            ctx->f[13] = f20;
            call(0x010BFDB0u, 0x010E21C0u); // CalcRootFitting
            const uint32_t s1 = c + 0x40u;
            sreg(21, c + 0x90u);
            if (subgeo)
            {
                const uint32_t fpv = relocated(rdram, 0x010BFDC0u, 0x010BFDD0u);
                const uint32_t s6 = r.u32(relocated(rdram, 0x010BFDC8u, 0x010BFDD4u));
                const uint32_t nodes = r.u32(c + 0x2E8u);
                sreg(30, fpv);
                sreg(22, s6);
                sreg(17, nodes);
                if (r.u8(c + 0xD90u) == 2u)
                {
                    bool add = true;
                    ctx->f[14] = r.f(c + 0x734u);
                    if (r.u32(c + 0xD20u) == 0u)
                    {
                        ctx->f[0] = 1.0f;
                        add = cond(FPU_C_OLT_S(ctx->f[14], ctx->f[0]));
                    }
                    if (add)
                    {
                        SET_GPR_U32(ctx, 8, r.u32(c + 0x358u));
                        r.put32(sp, c + 0xD70u);
                        ctx->f[13] = 1.0f;
                        SET_GPR_U64(ctx, 4, 0u);
                        const uint32_t w318 = r.u32(c + 0x318u);
                        SET_GPR_U64(ctx, 5, 0u);
                        SET_GPR_U32(ctx, 6, nodes);
                        SET_GPR_U32(ctx, 7, c + 0xD20u);
                        SET_GPR_U32(ctx, 9, c + 0x90u);
                        SET_GPR_U32(ctx, 10, fpv);
                        ctx->f[12] = f20;
                        SET_GPR_U32(ctx, 11, c + 0xD60u);
                        r.put32(sp + 8u, w318);
                        r.put32(sp + 0x10u, 0u);
                        call(0x010BFE50u, 0x010C6840u); // AddSubGeometry
                    }
                }
                SET_GPR_U32(ctx, 6, nodes);
                SET_GPR_U32(ctx, 8, fpv);
                SET_GPR_U32(ctx, 9, s6);
                SET_GPR_U64(ctx, 4, 0u);
                SET_GPR_U64(ctx, 5, 0u);
                SET_GPR_U32(ctx, 7, c);
                ctx->f[12] = f20;
                call(0x010BFE70u, 0x010C4CA8u); // AddCharacterClassSubGeometries
            }
            // the node quaternions normalised (a do-while on the count - 1: nothing for 0)
            {
                const int32_t n = static_cast<int32_t>(r.s16(c + 0x1B4u));
                if (n != 0)
                {
                    const sotc_p2::Scope bracket(game ? g_idNodes : -1);
                    nodeQuats<Log>(ctx, r, r.u32(c + 0x2E8u), n);
                }
            }
            SET_GPR_U64(ctx, 7, interp);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            SET_GPR_U32(ctx, 4, obj);
            ctx->f[12] = f21;
            ctx->f[13] = f20;
            call(0x010BFEE8u, 0x010BBA60u); // CalcNodeQuaternionWithIK
            SET_GPR_U32(ctx, 4, c);
            call(0x010BFEF0u, 0x010BC2F8u); // the root nodes
            sreg(17, s1);
            if (subgeo)
            {
                const uint32_t sites[2][3] = {{0x010BFF00u, 0x010BFF08u, 0x010BFF20u}, {0x010BFF28u, 0x010BFF2Cu, 0x010BFF44u}};
                for (const auto &s : sites)
                {
                    SET_GPR_U32(ctx, 9, r.u32(relocated(rdram, s[0], s[1])));
                    SET_GPR_U32(ctx, 4, s1);
                    SET_GPR_U32(ctx, 6, r.u32(c + 0x2E8u));
                    SET_GPR_U32(ctx, 5, sp + 0x40u);
                    SET_GPR_U32(ctx, 8, r.u32(c + 0x13Cu));
                    SET_GPR_U32(ctx, 7, c);
                    ctx->f[12] = f20;
                    call(s[2], 0x010C4CA8u);
                }
            }

            // ---- 5: +0x470 / +0x480 (the root quaternion and its slerp), the gaze into +0x710, +0x590 ----
            r.putq(c + 0x470u, r.q(r.u32(c + 0x13Cu)));
            if ((r.u32(cls + 0x38u) >> 3) & 1u)
            {
                const float t724 = r.f(c + 0x724u);
                ctx->f[1] = t724;
                r.putq(c + 0x470u, qmul(ctx, r.q(c + 0x470u), r.q(s7)));
                ctx->f[0] = 1.0f;
                if (cond(FPU_C_OLE_S(1.0f, t724)))
                    r.putq(c + 0x480u, r.q(c + 0x470u));
                else
                {
                    r.putq(sp + 0x40u, r.q(c + 0x480u));
                    const uint32_t d = dot(ctx, r.q(c + 0x470u), r.q(sp + 0x40u));
                    ctx->f[0] = bitsF(d);
                    r.put32(sp + 0x94u, d);
                    if (static_cast<int32_t>(d) < 0)
                        negate(ctx, r, sp + 0x40u);
                    r.putq(c + 0x480u, blend(ctx, r.q(c + 0x470u), r.q(sp + 0x40u), fBits(t724)));
                    norm(ctx, r, c + 0x480u);
                }
            }
            __m128 q590;
            if (!gaze)
                q590 = r.q(r.u32(c + 0x13Cu));
            else
            {
                SET_GPR_U32(ctx, 4, sp + 0x40u);
                SET_GPR_U32(ctx, 5, mo);
                ctx->f[12] = r.f(c + 0x1C8u);
                call(0x010C0090u, 0x010B47E8u); // GetMotionGazeDirInfo
                if (GPR_U64(ctx, 2) == 0u)
                    r.putq(sp + 0x40u, ctx->vu0_vf[0]);
                else
                {
                    SET_GPR_U32(ctx, 4, sp + 0x50u);
                    SET_GPR_U32(ctx, 5, sp + 0x40u);
                    call(0x010C00A0u, sotc_decomp::jalTarget(rdram, 0x010C00A0u));
                    ctx->f[1] = r.f(c + 0x574u);
                    ctx->f[3] = bitsF(0x47000000u);
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
                    ctx->f[2] = bitsF(0x40490FDAu);
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
                    ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[2]);
                    {
                        int32_t tmp = FPU_CVT_W_S(ctx->f[0]);
                        std::memcpy(&ctx->f[1], &tmp, sizeof(tmp));
                        SET_GPR_S32(ctx, 5, static_cast<int32_t>(static_cast<int16_t>(tmp)));
                    }
                    SET_GPR_U32(ctx, 6, sp + 0x50u);
                    SET_GPR_U32(ctx, 4, sp + 0x40u);
                    call(0x010C00E8u, sotc_decomp::jalTarget(rdram, 0x010C00E8u));
                }
                r.putq(sp + 0x40u, qmul(ctx, r.q(r.u32(c + 0x13Cu)), r.q(sp + 0x40u)));
                const float t744 = r.f(c + 0x744u);
                ctx->f[1] = t744;
                ctx->f[0] = 1.0f;
                const uint32_t a0 = c + 0x710u;
                if (cond(FPU_C_OLT_S(t744, 1.0f)))
                {
                    r.putq(sp + 0x60u, r.q(a0));
                    const uint32_t d = dot(ctx, r.q(sp + 0x40u), r.q(sp + 0x60u));
                    ctx->f[0] = bitsF(d);
                    r.put32(sp + 0x98u, d);
                    if (static_cast<int32_t>(d) < 0)
                        negate(ctx, r, sp + 0x60u);
                    r.putq(a0, blend(ctx, r.q(sp + 0x40u), r.q(sp + 0x60u), fBits(t744)));
                    norm(ctx, r, a0);
                    norm(ctx, r, a0);
                }
                else
                    r.putq(a0, r.q(sp + 0x40u));
                q590 = r.q(a0);
            }
            r.putq(c + 0x590u, q590);

            // ---- 6: the root dynamics' second half, the gaze motions, the pose, the layout, the bone products ----
            if (dyn)
            {
                SET_GPR_U32(ctx, 4, c);
                SET_GPR_U32(ctx, 5, sphereKind);
                ctx->f[12] = f21;
                ctx->f[13] = f20;
                call(0x010C0248u, 0x010DAE30u); // GetCharaRootDynamicsAfter
                r.put32(c + 0x438u, sphereKind);
            }
            if (gaze)
            {
                SET_GPR_U32(ctx, 4, r.u32(c + 0x2E8u));
                SET_GPR_U32(ctx, 5, c);
                call(0x010C0268u, 0x010C6C80u); // GazeAddMotions
            }
            SET_GPR_U32(ctx, 4, r.u32(c + 0x158u));
            SET_GPR_U32(ctx, 9, c + 0x90u);
            SET_GPR_U32(ctx, 5, r.u32(c + 0x154u));
            SET_GPR_U32(ctx, 8, s1);
            SET_GPR_U32(ctx, 6, list358);
            SET_GPR_U32(ctx, 7, r.u32(c + 0x2E8u));
            call(0x010C0284u, 0x010B1D60u); // GetMotionMatrix
            if (subgeo)
            {
                const uint32_t sites[2][3] = {{0x010C0294u, 0x010C029Cu, 0x010C02B4u}, {0x010C02BCu, 0x010C02C0u, 0x010C02DCu}};
                for (const auto &s : sites)
                {
                    SET_GPR_U32(ctx, 4, r.u32(c + 0x158u));
                    SET_GPR_U32(ctx, 10, r.u32(relocated(rdram, s[0], s[1])));
                    SET_GPR_U32(ctx, 6, c);
                    SET_GPR_U32(ctx, 5, r.u32(c + 0x154u));
                    SET_GPR_U32(ctx, 8, s1);
                    SET_GPR_U32(ctx, 7, list358);
                    SET_GPR_U32(ctx, 9, r.u32(c + 0x2E8u));
                    ctx->f[12] = r.f(c + 0x1160u);
                    call(s[2], 0x010C6A88u);
                }
            }
            if (interp != 0u)
            {
                r.put32(c + 0x344u, r.u32(c + 0x310u));
                r.put32(c + 0x348u, r.u32(c + 0x314u));
                r.put8(c + 0x533u, 0u);
            }
            else
            {
                r.put32(c + 0x344u, r.u32(c + 0x158u));
                r.put32(c + 0x348u, r.u32(c + 0x154u));
                r.put8(c + 0x533u, 1u);
            }
            // UpdateLayoutClassGeometry, the bone products, +0x450, the child nodes
            auto layout = [&](uint32_t updateSite, uint32_t bc718Site)
            {
                SET_GPR_U32(ctx, 4, c);
                call(updateSite, 0x010F26D0u);
                uint8_t f1bc = r.u8(c + 0x1BCu);
                const int32_t nb = r.s16(c + 0x1B4u);
                if ((f1bc & 8u) != 0u && nb > 0)
                {
                    const sotc_p2::Scope bracket(game ? g_idBones : -1);
                    if (boneProducts<Log>(r, r.u32(c + 0x160u), r.u32(c + 0x15Cu), r.u32(c + 0x2D8u), nb))
                        f1bc = r.u8(c + 0x1BCu);
                    else
                    {
                        // the clamped VU0 loop (overlapping or out-of-range arrays): decomp_charageo's
                        namespace vu = sotc_decomp::vu;
                        __m128 rows[4], cols[4], lastAcc = ctx->vu0_acc;
                        int32_t a2 = 0;
                        uint32_t out = r.u32(c + 0x160u);
                        for (;;)
                        {
                            const uint32_t o = static_cast<uint32_t>(a2) << 6;
                            const uint32_t ma = r.u32(c + 0x15Cu) + o, mb = r.u32(c + 0x2D8u) + o;
                            out += o;
                            for (int k = 0; k < 4; ++k)
                                rows[k] = r.q(ma + 0x10u * k);
                            for (int k = 0; k < 4; ++k)
                                cols[k] = r.q(mb + 0x10u * k);
                            const vu::Rows cr = vu::clampRows(rows);
                            __m128 res[4];
                            for (int k = 0; k < 4; ++k)
                                res[k] = vu::apply(cr, cols[k], &lastAcc);
                            for (int k = 0; k < 4; ++k)
                                r.putq(out + 0x10u * k, res[k]);
                            for (int k = 0; k < 4; ++k)
                                cols[k] = res[k];
                            const int32_t n = r.s16(c + 0x1B4u);
                            ++a2;
                            if (!(a2 < n))
                                break;
                            out = r.u32(c + 0x160u);
                        }
                        for (int k = 0; k < 4; ++k)
                        {
                            ctx->vu0_vf[14 + k] = rows[k];
                            ctx->vu0_vf[24 + k] = cols[k];
                        }
                        ctx->vu0_acc = lastAcc;
                        f1bc = r.u8(c + 0x1BCu);
                    }
                }
                if (f1bc & 1u)
                    r.putq(c + 0x450u, r.q(r.u32(c + 0x154u)));
                SET_GPR_U32(ctx, 4, c);
                call(bc718Site, 0x010BC718u);
            };
            layout(0x010C031Cu, 0x010C0404u);
            if (r.s8(c + 0x10A0u) != 0)
            {
                r.putq(c + 0x500u, r.q(r.u32(c + 0x138u)));
                SET_GPR_U32(ctx, 4, c);
                call(0x010C0420u, 0x010F2B50u);
                r.putq(c + 0x4D0u, r.q(c + 0x4C0u));
                layout(0x010C0430u, 0x010C0514u);
            }

            // ---- 7: the queued callbacks ----
            const uint32_t c2 = classOf(obj);
            sreg(18, c2);
            const int32_t queued = r.s8(c2 + 0x53Du);
            if (queued > 0)
            {
                uint32_t e = c2 + 0x550u;
                for (int32_t i = queued; i != 0; --i, e += 4u)
                {
                    const uint32_t fn = r.u32(e);
                    if (fn == 0u)
                        continue;
                    sreg(16, e);
                    sreg(17, static_cast<uint32_t>(i));
                    SET_GPR_U32(ctx, 4, obj);
                    call(0x010C0564u, fn);
                    r.put32(e, 0u);
                }
                sreg(16, e);
                sreg(17, 0u);
            }
            r.put8(c2 + 0x53Du, 0u);
        }

        // ---- 8: the cloth ----
        SET_GPR_U32(ctx, 4, obj);
        call(0x010C0580u, 0x010F1A20u); // GetLayoutClass
        const uint32_t lc = GPR_U32(ctx, 2);
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 2));
        if (r.u32(lc + 0x180u) != 0u)
        {
            const uint32_t s1 = classOf(obj);
            sreg(17, s1);
            if (r.s8(s1 + 0x53Cu) == 0)
            {
                sreg(18, s1 + 0x52Au);
                if (r.u32(lc + 0x124u) != 0u)
                {
                    SET_GPR_U32(ctx, 4, lc + 0xC0u);
                    call(0x010C05D0u, 0x011329C0u);
                    const uint32_t other = GPR_U32(ctx, 2);
                    if (GPR_U64(ctx, 2) != 0u)
                    {
                        const uint32_t oc = classOf(other);
                        if (oc != 0u)
                        {
                            const uint8_t b = r.u8(oc + 0x52Au) & 0x7Fu;
                            if (b != 0u)
                                r.put8(s1 + 0x52Au, b);
                        }
                    }
                }
                const uint32_t s2c = s1 + 0x52Au;
                if (r.s8(s2c) != 0)
                {
                    r.putq(lc + 0x30u, _mm_setzero_ps());
                    SET_GPR_U32(ctx, 4, lc);
                    SET_GPR_U32(ctx, 5, relocated(rdram, 0x010C0628u, 0x010C0634u));
                    call(0x010C0630u, 0x010F2C78u);
                    auto cloth = [&](uint32_t site)
                    {
                        SET_GPR_U32(ctx, 4, obj);
                        ctx->f[12] = f21;
                        ctx->f[13] = f20;
                        SET_GPR_U64(ctx, 5, 1u);
                        SET_GPR_U32(ctx, 6, 0u);
                        call(site, 0x01137A68u); // GetClothDynamics
                    };
                    cloth(0x010C0648u);
                    SET_GPR_U32(ctx, 4, obj);
                    call(0x010C0650u, 0x011398F8u);
                    const uint32_t k = r.u8(s2c) & 0x7Fu;
                    sreg(16, k - 1u);
                    if (k != 0u)
                        sreg(21, 0xFFFFFFFFu);
                    for (uint32_t i = 0; i < k; ++i)
                    {
                        sreg(16, k - 2u - i);
                        cloth(0x010C0688u);
                    }
                    if (k != 0u)
                        sreg(16, 0xFFFFFFFEu);
                    r.put8(s2c, static_cast<uint8_t>(r.u8(s2c) | 0x80u));
                }
                const uint32_t a3 = r.u32(lc + 0x178u);
                const uint32_t s0c = r.u32(s1 + 0xE38u) + (static_cast<uint32_t>(static_cast<int32_t>(r.s16(s1 + 0x370u))) << 2);
                sreg(16, s0c);
                uint64_t a1 = 2u;
                if (r.u32(a3 + 0x10u) == 0u)
                {
                    const uint32_t fn = r.u32(a3 + 0xCu);
                    if (fn != 0u)
                    {
                        SET_GPR_U32(ctx, 5, s0c);
                        SET_GPR_U32(ctx, 4, obj);
                        call(0x010C06D0u, fn);
                        a1 = GPR_U64(ctx, 2) != 0u ? 1u : 0u;
                    }
                    else
                        a1 = r.s8(s1 + 0x362u) == 0 ? 1u : 0u;
                }
                SET_GPR_U64(ctx, 5, a1);
                ctx->f[12] = f21;
                ctx->f[13] = f20;
                SET_GPR_U32(ctx, 6, s0c);
                SET_GPR_U32(ctx, 4, obj);
                // Round S8: on the guest worker beside the game thread (s8_cloth.cpp), else here.
                boundary(0x010C06F4u, 0x01137A68u);
                if (!s8cloth::lastCloth(rdram, ctx, runtime, lc, 0x010C06FCu, sp))
                    sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137A68u, 0x010C06FCu, sp);
                resume();
            }
        }
        // ---- 8: the anime tables, near enough ----
        {
            const float lim = r.f(r.u32(c + 0x1140u) + 0x3Cu);
            ctx->f[0] = 0.0f;
            ctx->f[1] = lim;
            if (cond(FPU_C_OLE_S(0.0f, lim)))
            {
                const float lim2 = FPU_MUL_S(lim, lim);
                ctx->f[0] = lim2;
                const __m128 v5 = r.q(point), v6 = r.q(r.u32(c + 0x134u));
                __m128 v7 = PS2_VSUB(v5, v6);
                v7 = sel(v7, ps2VuMulK<1, 1>(v7, v7), 1, 1, 1, 0);
                v7 = sel(v7, ps2VuAddK<1, 1>(v7, bc<1>(v7)), 1, 0, 0, 0);
                v7 = sel(v7, ps2VuAddK<1, 1>(v7, bc<2>(v7)), 1, 0, 0, 0);
                ctx->vu0_vf[5] = v5;
                ctx->vu0_vf[6] = v6;
                ctx->vu0_vf[7] = v7;
                const float d2 = bitsF(laneXBits(v7));
                ctx->f[1] = d2;
                if (cond(FPU_C_OLT_S(d2, lim2)))
                {
                    const uint32_t s0a = classOf(obj);
                    sreg(16, s0a);
                    auto anime = [&](uint32_t site, uint32_t base, uint32_t a0)
                    {
                        const float t = r.f(base + 0x2Cu);
                        ctx->f[0] = t;
                        SET_GPR_U32(ctx, 4, a0);
                        SET_GPR_U32(ctx, 5, obj);
                        ctx->f[12] = FPU_MUL_S(r.f(base + 8u), t);
                        call(site, 0x01169020u); // ExecAnimeTableInfo
                    };
                    anime(0x010C0788u, s0a + 0x1C0u, s0a + 0x1F0u);
                    if (r.u8(s0a + 0xD90u) == 2u && r.s32(r.u32(s0a + 0x750u) + 0xCu) >= 0 && r.u32(s0a + 0xD20u) != 0u && s0a + 0xD20u != 0u)
                        anime(0x010C07D0u, s0a + 0xD20u, s0a + 0xD50u);
                    const int32_t n = r.s32(s0a + 0x3FCu);
                    uint32_t slot = r.u32(s0a + 0x30Cu);
                    sreg(16, slot);
                    for (int32_t i = n; i > 0; --i, slot += 0x80u)
                        if (r.s32(slot + 0x44u) >= 0)
                        {
                            sreg(16, slot);
                            sreg(17, static_cast<uint32_t>(i));
                            anime(0x010C0804u, slot + 4u, slot + 0x34u);
                        }
                }
            }
        }
        // the epilogue: v0 the class (+0x74), the saves back
        SET_GPR_S32(ctx, 2, r.s32(sp + 0x74u));
        for (int k = 0; k < 8; ++k)
            SET_GPR_U64(ctx, 16 + k, r.u64(sp + 0xB0u + 8u * k));
        SET_GPR_U64(ctx, 30, r.u64(sp + 0xF0u));
        SET_GPR_U64(ctx, 31, r.u64(sp + 0xF8u));
        ctx->f[21] = r.f(sp + 0x108u);
        ctx->f[20] = r.f(sp + 0x100u);
        r.put8(c + 0xCECu, 0u);
        SET_GPR_U32(ctx, 29, sp0);
        sotc_decomp::returnTo(ctx);
        (void)C;
    }

    bool run(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // the states the old body keeps: pose82 off (its clamped bone loop is X1's fallback, but the switch's other
        // users decide), the far-character halving (a host table of seen poses)
        if (!sotc_decomp::pose82On() || native_renderer::tuning().sacFarChara)
        {
            ++c_old.n;
            return false;
        }
        ++c_calls.n;
        bracketIds();
        if (__builtin_expect(x1::checkOn(), 0) && x1::checkFromReached())
        {
            if (sotc_decomp::onWorkerLane(runtime) || g_check.active)
            {
                ++g_check.skipped; // (the worker; or a nested call: an EE thread switch inside a callee)
                body<false>(rdram, ctx, runtime, nullptr);
                return true;
            }
            g_check.active = true;
            g_check.ref = &refBody;
            g_check.begin(rdram, ctx);
            body<true>(rdram, ctx, runtime, &g_check);
            g_check.end(ctx);
            g_check.active = false;
            return true;
        }
        body<false>(rdram, ctx, runtime, nullptr);
        return true;
    }
} // namespace x1geo

namespace x1
{
    bool checkFromReached()
    {
        if (__builtin_expect(g_checkFrom < 0, 0))
        {
            const char *e = std::getenv("SOTC_X1_CHECK_FROM");
            g_checkFrom = e ? std::atoll(e) : 0;
        }
        return static_cast<long long>(sotc_decomp::g_gameFrame) >= g_checkFrom;
    }
    bool execCharaGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return x1geo::run(rdram, ctx, runtime); }
}

#include "x1_charageo_ref.inc"
#include "x1_xz.inc"
#include "x1_xz_ref.inc"

SOTC_DECOMP_FUNCTION(0x010E7610u, xz, x1geo::xzSw);
