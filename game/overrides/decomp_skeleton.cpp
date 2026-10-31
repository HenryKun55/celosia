// manager_sub_010B8590: a skeleton's pose, bone by bone (recursive: first
// child, then next sibling). 2.8% of the Vita's game thread recompiled
// (throttled attract demo). For bone i (a0) of the skeleton a3 (+8 the
// bones, 0x40 each: +0x10 offset, +0x30 first child, +0x34 next sibling),
// with the local rotations t0 (quaternions), the scale t1, it pushes the
// matrix and quaternion stacks, turns the current quaternion by bone i's
// (normalised in place), builds that quaternion's rotation matrix and the
// offset x scale translation on the frame, applies them to the current
// matrix and stores it (a1 + 0x40 i) with the current quaternion,
// normalised (a2 + 0x10 i), then does the children and pops.
//
// The matrix / quaternion stack helpers (boot ELF: PushQuaternion,
// iosPushCurrentMatrix, MultiCurrentQuaternion, iosTransCurrentMatrix,
// iosMulCurrentMatrixR, iosGetCurrentMatrix, GetCurrentQuaternion,
// iosPopCurrentMatrix, PopQuaternion) are native too, on the context's VU0
// registers and VU0 memory (vi15, the matrix stack) as their recompiled
// code: 9 guest calls a bone were ~2.4% of the Vita's game thread in call
// overhead alone. Their jal sites are relocated: the targets are read from
// the code, and a site naming another function is called through the guest.
// The VU0 arithmetic of the pose itself is the recompiled code's own, on
// locals, written back to the context. Recursion is native.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace decomp_skeleton
{
    using sotc_decomp::returnTo;
    namespace vu = sotc_decomp::vu;

    struct Ctx
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;

        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
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
        void call(uint32_t site, uint32_t sp) const { sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp); }
        uint8_t *vu0(uint32_t vi) const { return runtime->memory().getVU0Data() + ((vi & 0xFFu) << 4); }

        // The boot ELF's matrix / quaternion stack helpers, natively (each
        // as its recompiled code: the same VU0 operations, the registers it
        // leaves). Called through the guest when the jal site does not name
        // the expected function, or for PushQuaternion's reset case.
        bool is(uint32_t site, uint32_t target) const { return sotc_decomp::jalTarget(rdram, site) == target; }
        static constexpr uint32_t kQuatTop = 0x001E0F60u, kQuatStack = 0x00202AC0u;
        void pushQuaternion(uint32_t site, uint32_t sp) const
        {
            const int32_t n0 = static_cast<int32_t>(u32(kQuatTop));
            if (!is(site, 0x001BBB48u) || n0 < 0)
                return call(site, sp);
            int32_t n = n0 + 1;
            put32(kQuatTop, static_cast<uint32_t>(n));
            if (!(n < 0x40))
            {
                n = 0x3F;
                put32(kQuatTop, 0x3Fu);
            }
            const uint32_t a0 = (static_cast<uint32_t>(n) << 4) + kQuatStack;
            const __m128i v = _mm_castps_si128(q(a0 - 0x10u));
            putq(a0, _mm_castsi128_ps(v));
            SET_GPR_VEC(ctx, 2, v);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(kQuatStack));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(a0));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(a0));
        }
        void pushMatrix(uint32_t site, uint32_t sp) const
        {
            if (!is(site, 0x001B8638u))
                return call(site, sp);
            for (int r = 1; r <= 4; ++r)
            {
                std::memcpy(vu0(ctx->vi[15]), &ctx->vu0_vf[r], 16);
                ctx->vi[15] = static_cast<uint16_t>(ctx->vi[15] + 1u);
            }
        }
        void popMatrix(uint32_t site, uint32_t sp) const
        {
            if (!is(site, 0x001B8650u))
                return call(site, sp);
            for (int r = 4; r >= 1; --r)
            {
                ctx->vi[15] = static_cast<uint16_t>(ctx->vi[15] - 1u);
                std::memcpy(&ctx->vu0_vf[r], vu0(ctx->vi[15]), 16);
            }
        }
        void popQuaternion(uint32_t site, uint32_t sp) const
        {
            if (!is(site, 0x001BBC08u))
                return call(site, sp);
            int32_t n = static_cast<int32_t>(u32(kQuatTop)) - 1;
            const bool negative = n < 0;
            if (negative)
                n = 0;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(kQuatTop));
            SET_GPR_S32(ctx, 3, n);
            SET_GPR_U64(ctx, 4, negative ? 1u : 0u);
            put32(kQuatTop, static_cast<uint32_t>(n));
        }
        uint32_t currentQuaternion() const
        {
            const uint32_t v0 = kQuatStack + (u32(kQuatTop) << 4);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(kQuatStack));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            SET_GPR_S32(ctx, 3, 0x001E0000);
            return v0;
        }
        void getCurrentQuaternion(uint32_t site, uint32_t sp) const
        {
            if (!is(site, 0x001BB868u))
                return call(site, sp);
            currentQuaternion();
        }
        void multiCurrentQuaternion(uint32_t site, uint32_t sp, uint32_t a0) const
        {
            if (!is(site, 0x001BB8A8u))
            {
                SET_GPR_U32(ctx, 4, a0);
                return call(site, sp);
            }
            const uint32_t v0 = currentQuaternion();
            __m128 *vf = ctx->vu0_vf;
            const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)), x = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)),
                         w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
            auto ln = [](__m128 v, int l)
            {
                switch (l)
                {
                case 0: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
                case 1: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
                case 2: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
                default: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
                }
            };
            auto yzx_ = [](__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); };
            auto zxy_ = [](__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); };
            // Clamped once (sotc_decomp::vu): every PS2_V* operand below is
            // a clamped input or a clamped result in the lanes kept.
            vf[11] = q(v0);
            vf[12] = q(a0);
            const __m128 c11 = vu::clamp(vf[11]), c12 = vu::clamp(vf[12]);
            vf[13] = vu::mulN(c11, c12);
            vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], ln(vf[13], 1)), x);
            vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], ln(vf[13], 2)), x);
            vf[13] = _mm_blendv_ps(vf[13], vu::subN(vf[13], ln(vf[13], 0)), w);
            vf[14] = _mm_blendv_ps(vf[14], vu::mulN(c12, ln(c11, 3)), xyz);
            vf[15] = _mm_blendv_ps(vf[15], vu::mulN(c11, ln(c12, 3)), xyz);
            ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, vu::mulN(yzx_(c12), zxy_(c11)), xyz);
            vf[16] = _mm_blendv_ps(vf[16], vu::subN(ctx->vu0_acc, vu::mulN(yzx_(c11), zxy_(c12))), xyz);
            vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[14], vf[15]), xyz);
            vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], vf[16]), xyz);
            putq(v0, vf[13]);
            SET_GPR_U32(ctx, 4, a0);
        }
        // ACC = m1 x v.x + m2 x v.y + m3 x v.z; returns ACC + m4 x v.w (vmulax / vmadday / vmaddaz / vmaddw),
        // the rows m1..m4 clamped (vu::clampRows), v and w4 raw.
        __m128 column(const vu::Rows &m, __m128 v, __m128 w4) const
        {
            const __m128 c = vu::clamp(v);
            __m128 acc = vu::mulN(m.r[0], vu::lane<0>(c));
            acc = vu::addN(acc, vu::mulN(m.r[1], vu::lane<1>(c)));
            acc = vu::addN(acc, vu::mulN(m.r[2], vu::lane<2>(c)));
            ctx->vu0_acc = acc;
            return vu::addN(acc, vu::mulN(m.r[3], vu::clamp(w4)));
        }
        void transCurrentMatrix(uint32_t site, uint32_t sp, uint32_t a0) const
        {
            SET_GPR_U32(ctx, 4, a0);
            if (!is(site, 0x001B8878u))
                return call(site, sp);
            __m128 *vf = ctx->vu0_vf;
            vf[8] = q(a0);
            vf[4] = column(vu::clampRows(vf + 1), vf[8], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3, 3, 3, 3)));
        }
        void mulCurrentMatrixR(uint32_t site, uint32_t sp, uint32_t a0) const
        {
            SET_GPR_U32(ctx, 4, a0);
            if (!is(site, 0x001B8CD0u))
                return call(site, sp);
            __m128 *vf = ctx->vu0_vf;
            for (int c = 0; c < 4; ++c)
                vf[14 + c] = q(a0 + 0x10u * c);
            const vu::Rows rows = vu::clampRows(vf + 1); // vf1..vf4 stay as they are until the copy below
            for (int c = 0; c < 4; ++c)
                vf[14 + c] = column(rows, vf[14 + c], _mm_shuffle_ps(vf[14 + c], vf[14 + c], _MM_SHUFFLE(3, 3, 3, 3)));
            for (int c = 0; c < 4; ++c)
                vf[1 + c] = vf[14 + c];
        }
        void getCurrentMatrix(uint32_t site, uint32_t sp, uint32_t a0) const
        {
            SET_GPR_U32(ctx, 4, a0);
            if (!is(site, 0x001B8C90u))
                return call(site, sp);
            for (int c = 0; c < 4; ++c)
                putq(a0 + 0x10u * c, ctx->vu0_vf[1 + c]);
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
    };

    const __m128 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kY = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                 kZ = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)), kW = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)),
                 kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)), kYz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, 0));

    template <int L>
    inline __attribute__((always_inline)) __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline __attribute__((always_inline)) __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    inline __attribute__((always_inline)) __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }

    // vmul vf15, vf14, vf14; vaddy / vaddz / vaddw .x; vrsqrt Q, vf0w, vf15x; vmulq vf15, vf14, Q.
    // v clamped once (sotc_decomp::vu); the sums are of clamped results.
    __m128 normalised(R5900Context *ctx, __m128 v, __m128 &vf15)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 c = vu::clamp(v);
        vf15 = vu::mulN(c, c);
        vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<1>(vf15)), kX);
        vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<2>(vf15)), kX);
        vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<3>(vf15)), kX);
        ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(vf15));
        vf15 = vu::mulN(c, vu::clamp(_mm_set1_ps(ctx->vu0_q)));
        return vf15;
    }

    void bone(const Ctx g, uint32_t i, uint32_t matrices, uint32_t quats, uint32_t skeleton, uint32_t locals, uint32_t scale, uint32_t sp0)
    {
        R5900Context *ctx = g.ctx;
        const uint32_t sp = sp0 - 0xB0u;
        const uint32_t b = g.u32(skeleton + 8u) + i * 0x40u;
        const uint32_t local = locals + i * 16u;
        const uint32_t sibling = g.u32(b + 0x34u);
        g.pushQuaternion(0x010B85F0u, sp);
        g.pushMatrix(0x010B85F8u, sp);
        const uint32_t child = g.u32(b + 0x30u);
        g.multiCurrentQuaternion(0x010B8604u, sp, local);

        __m128 vf15;
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 vf0c = vu::clamp(vf0);
        // normalised's result: clamped in every lane, read back below as vf11.
        const __m128 quat = normalised(ctx, g.q(local), vf15);
        g.putq(local, quat);
        // The rotation matrix of the quaternion (rows vf14..vf16, w 0).
        // Clamped once (sotc_decomp::vu): each operand of the products and
        // sums is a clamped value in the lanes the blend keeps (x, y, z;
        // vf11's w is the quaternion's), so the results are PS2_V*'s bit for bit.
        __m128 vf12 = _mm_blendv_ps(ctx->vu0_vf[12], vu::addN(vf0c, lane<3>(vf0c)), kXyz);
        __m128 vf11 = quat;
        const __m128 mr32w = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0, 3, 2, 1));
        __m128 vf14 = _mm_blendv_ps(ctx->vu0_vf[14], mr32w, kW);
        vf15 = _mm_blendv_ps(vf15, mr32w, kW);
        __m128 vf16 = _mm_blendv_ps(ctx->vu0_vf[16], mr32w, kW);
        const __m128 vf0sq = vu::mulN(vf0c, vf0c);
        __m128 acc = _mm_blendv_ps(ctx->vu0_acc, vu::mulN(yzx(vf11), zxy(vf12)), kXyz);
        vf11 = _mm_blendv_ps(vf11, vu::addN(acc, vf0sq), kXyz);
        const __m128 vf10 = _mm_blendv_ps(ctx->vu0_vf[10], vu::addN(acc, vu::mulN(vf11, vf12)), kXyz);
        const __m128 vf13 = PS2_VBLEND(ctx->vu0_vf[13], vu::mulN(vf11, vf10), kXyz);
        acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf11), zxy(vf10)), kXyz);
        const __m128 byW = vu::mulN(vf10, lane<3>(vf11));
        vf15 = _mm_blendv_ps(vf15, vu::addN(acc, byW), kXyz);
        vf16 = _mm_blendv_ps(vf16, vu::subN(acc, byW), kXyz);
        acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf13), zxy(vf12)), kXyz);
        __m128 vf17 = _mm_blendv_ps(ctx->vu0_vf[17], vu::addN(acc, vu::mulN(vf13, vf12)), kXyz);
        acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf15), zxy(vf12)), kXyz);
        vf15 = _mm_blendv_ps(vf15, vu::addN(acc, vf0sq), kXyz);
        vf14 = PS2_VBLEND(vf14, vu::subN(vf12, vf17), kXyz);
        vf17 = _mm_blendv_ps(vf17, vf14, kYz);
        vf14 = _mm_blendv_ps(vf14, vf16, kY);
        vf16 = _mm_blendv_ps(vf16, vf15, kY);
        vf15 = _mm_blendv_ps(vf15, vf17, kY);
        vf14 = _mm_blendv_ps(vf14, vf15, kZ);
        vf15 = _mm_blendv_ps(vf15, vf16, kZ);
        vf16 = _mm_blendv_ps(vf16, vf17, kZ);
        g.putq(sp, vf14);
        g.putq(sp + 0x10u, vf15);
        g.putq(sp + 0x20u, vf16);
        g.putq(sp + 0x30u, vf0);
        __m128 vf5 = g.q(b + 0x10u);
        const __m128 vf6 = g.q(scale);
        vf5 = PS2_VBLEND(vf5, PS2_VMUL(vf5, vf6), kXyz);
        g.putq(sp + 0x40u, vf5);
        const float one = 1.0f;
        std::memcpy(g.rdram + ((sp + 0x4Cu) & PS2_RAM_MASK), &one, 4);
        ctx->f[0] = one;
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[6] = vf6;
        ctx->vu0_vf[10] = vf10;
        ctx->vu0_vf[11] = vf11;
        ctx->vu0_vf[12] = vf12;
        ctx->vu0_vf[13] = vf13;
        ctx->vu0_vf[14] = vf14;
        ctx->vu0_vf[15] = vf15;
        ctx->vu0_vf[16] = vf16;
        ctx->vu0_vf[17] = vf17;
        ctx->vu0_acc = acc;

        g.transCurrentMatrix(0x010B86C0u, sp, sp + 0x40u); // offset x scale
        g.mulCurrentMatrixR(0x010B86C8u, sp, sp);           // rotation
        g.getCurrentMatrix(0x010B86D8u, sp, matrices + i * 0x40u);
        g.getCurrentQuaternion(0x010B86E0u, sp);
        __m128 n15;
        g.putq(quats + i * 16u, normalised(ctx, g.q(GPR_U32(ctx, 2)), n15));
        ctx->vu0_vf[14] = g.q(GPR_U32(ctx, 2));
        ctx->vu0_vf[15] = n15;

        auto recurse = [&](uint32_t j, uint32_t ra)
        {
            SET_GPR_U32(ctx, 4, j);
            SET_GPR_U32(ctx, 5, matrices);
            SET_GPR_U32(ctx, 6, quats);
            SET_GPR_U32(ctx, 7, skeleton);
            SET_GPR_U32(ctx, 8, locals);
            SET_GPR_U32(ctx, 9, scale);
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, sp);
            bone(g, j, matrices, quats, skeleton, locals, scale, sp);
        };
        if (child != 0xFFFFFFFFu)
            recurse(child, 0x010B8734u);
        g.popMatrix(0x010B8734u, sp);
        g.popQuaternion(0x010B873Cu, sp);
        if (sibling != 0xFFFFFFFFu)
            recurse(sibling, 0x010B8768u);
    }

    // ---- Round 26: the whole pose on host locals ----
    //
    // The same operations as bone() above, in the same order, on a copy of
    // the context's VU0 registers and of the GPRs the pose writes, kept in
    // locals (the recursion as a loop over a frame stack, so the copy never
    // has its address taken and GCC keeps it out of memory: a guest store
    // through rdram no longer makes it reload every register). The context
    // gets them once at the end, as the last bone left them. Guest memory
    // (the quaternion stack, the frames, the results) and VU0 memory (the
    // matrix stack) are written as before, in the same order. Taken when the
    // nine helper call sites name the expected functions (checked once a
    // call); otherwise, and with SOTC_SKELETON_FAST=0, bone() above.
    struct Gprs
    {
        uint64_t lo[32];
        uint64_t hi2; // v0's upper half (pushQuaternion's 128-bit write)
        uint32_t dirty = 0u;
        bool hi2Dirty = false;
        void s32(int r, uint32_t v)
        {
            lo[r] = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v)));
            dirty |= 1u << r;
        }
        void u64(int r, uint64_t v)
        {
            lo[r] = v;
            dirty |= 1u << r;
        }
        void vec2(__m128i v)
        {
            uint64_t w[2];
            std::memcpy(w, &v, 16);
            lo[2] = w[0];
            hi2 = w[1];
            dirty |= 1u << 2;
            hi2Dirty = true;
        }
        void flush(R5900Context *ctx)
        {
            // The dirty ones by their bits (round 35), not a test of all 31.
            for (uint32_t m = dirty & ~1u; m; m &= m - 1u)
            {
                const int r = __builtin_ctz(m);
                std::memcpy(&ctx->r[r], &lo[r], 8);
            }
            if (hi2Dirty)
                std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[2]) + 8, &hi2, 8);
            dirty = 0u;
            hi2Dirty = false;
        }
    };
    struct Vu
    {
        __m128 vf[18];
        __m128 acc;
        float q;
        uint16_t status, vi15;
        bool f0 = false; // ctx->f[0] = 1.0f done
        static constexpr int kRegs[] = {1, 2, 3, 4, 5, 6, 8, 10, 11, 12, 13, 14, 15, 16, 17};
        void load(const R5900Context *ctx)
        {
            for (int r : kRegs)
                vf[r] = ctx->vu0_vf[r];
            acc = ctx->vu0_acc;
            q = ctx->vu0_q;
            status = ctx->vu0_status;
            vi15 = ctx->vi[15];
        }
        void store(R5900Context *ctx) const
        {
            for (int r : kRegs)
                ctx->vu0_vf[r] = vf[r];
            ctx->vu0_acc = acc;
            ctx->vu0_q = q;
            ctx->vu0_status = status;
            ctx->vi[15] = vi15;
            if (f0)
                ctx->f[0] = 1.0f;
        }
    };
    struct Frame
    {
        uint32_t sibling, sp, phase;
        bool rowsClean; // the rows pushed by this bone were clamped values
    };
    thread_local std::vector<Frame> t_frames;

    struct Fast
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint8_t *vu0mem;
        uint32_t matrices, quats, skeleton, locals, scale;
        __m128 vf0, vf0c, vf0sq, vf0w, mr32w, vf12xyz;
        Vu v;
        Gprs gp;
        // Clamps left out where the value is already a clamped one (the
        // clamp is then the identity: a VU0 result is finite and, NEON
        // flushing, never denormal): vf0 is (when vf0Clean), the current
        // matrix rows are after a bone's iosMulCurrentMatrixR (rowsClean,
        // and back at the pop that restores rows pushed clean, unless the
        // VU0 matrix stack went deep enough to wrap: noClean).
        bool vf0Clean = false, rowsClean = false, noClean = false, pushedClean = false;

        uint32_t u32(uint32_t a) const
        {
            uint32_t x;
            std::memcpy(&x, rdram + (a & PS2_RAM_MASK), 4);
            return x;
        }
        void put32(uint32_t a, uint32_t x) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &x, 4); }
        __m128 q(uint32_t a) const
        {
            __m128 x;
            std::memcpy(&x, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return x;
        }
        void putq(uint32_t a, __m128 x) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &x, 16); }
        uint8_t *vu0(uint32_t vi) const { return vu0mem + ((vi & 0xFFu) << 4); }

        __attribute__((always_inline)) __m128 normalised(__m128 x, __m128 &vf15)
        {
            const __m128 c = vu::clamp(x);
            vf15 = vu::mulN(c, c);
            vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<1>(vf15)), kX);
            vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<2>(vf15)), kX);
            vf15 = _mm_blendv_ps(vf15, vu::addN(vf15, lane<3>(vf15)), kX);
            v.q = ps2VuRsqrt(v.status, _mm_cvtss_f32(vf0w), _mm_cvtss_f32(vf15));
            vf15 = vu::mulN(c, vu::clamp(_mm_set1_ps(v.q)));
            return vf15;
        }
        __m128 column(const vu::Rows &m, __m128 x, __m128 w4)
        {
            const __m128 c = vu::clamp(x);
            __m128 a = vu::mulN(m.r[0], vu::lane<0>(c));
            a = vu::addN(a, vu::mulN(m.r[1], vu::lane<1>(c)));
            a = vu::addN(a, vu::mulN(m.r[2], vu::lane<2>(c)));
            v.acc = a;
            return vu::addN(a, vu::mulN(m.r[3], vu::clamp(w4)));
        }
        // column() of clamped operands (x and w4 clamped values).
        __m128 columnClean(const vu::Rows &m, __m128 c, __m128 w4)
        {
            __m128 a = vu::mulN(m.r[0], vu::lane<0>(c));
            a = vu::addN(a, vu::mulN(m.r[1], vu::lane<1>(c)));
            a = vu::addN(a, vu::mulN(m.r[2], vu::lane<2>(c)));
            v.acc = a;
            return vu::addN(a, vu::mulN(m.r[3], w4));
        }
        uint32_t currentQuaternion()
        {
            const uint32_t v0 = Ctx::kQuatStack + (u32(Ctx::kQuatTop) << 4);
            gp.s32(1, Ctx::kQuatStack);
            gp.s32(2, v0);
            gp.s32(3, 0x001E0000u);
            return v0;
        }
        void setArgs(uint32_t j, uint32_t ra, uint32_t sp)
        {
            gp.s32(4, j);
            gp.s32(5, matrices);
            gp.s32(6, quats);
            gp.s32(7, skeleton);
            gp.s32(8, locals);
            gp.s32(9, scale);
            gp.s32(31, ra);
            gp.s32(29, sp);
        }
        // PushQuaternion's reset case (a negative top): the guest's, with
        // the context as the bones so far left it.
        __attribute__((always_inline)) void pushQuaternionGuest(uint32_t sp)
        {
            gp.flush(ctx);
            v.store(ctx);
            sotc_decomp::callGuest(rdram, ctx, runtime, 0x001BBB48u, 0x010B85F0u + 8u, sp);
            v.load(ctx);
            v.f0 = false;
            for (int r = 1; r < 32; ++r)
                gp.lo[r] = GPR_U64(ctx, r);
            noClean = true;
            rowsClean = false;
        }

        // bone() up to its children: returns the child, the sibling in *sibling.
        __attribute__((always_inline)) uint32_t enter(uint32_t i, uint32_t sp, uint32_t *sibling)
        {
            const uint32_t b = u32(skeleton + 8u) + i * 0x40u;
            const uint32_t local = locals + i * 16u;
            *sibling = u32(b + 0x34u);
            // PushQuaternion
            {
                const int32_t n0 = static_cast<int32_t>(u32(Ctx::kQuatTop));
                if (n0 < 0)
                    pushQuaternionGuest(sp);
                else
                {
                    int32_t n = n0 + 1;
                    put32(Ctx::kQuatTop, static_cast<uint32_t>(n));
                    if (!(n < 0x40))
                    {
                        n = 0x3F;
                        put32(Ctx::kQuatTop, 0x3Fu);
                    }
                    const uint32_t a0 = (static_cast<uint32_t>(n) << 4) + Ctx::kQuatStack;
                    const __m128i x = _mm_castps_si128(q(a0 - 0x10u));
                    putq(a0, _mm_castsi128_ps(x));
                    gp.vec2(x);
                    gp.s32(3, Ctx::kQuatStack);
                    gp.s32(4, a0);
                    gp.s32(5, a0);
                }
            }
            // iosPushCurrentMatrix (round 35: stores of the values, not a
            // memcpy from the registers' addresses)
            for (int r = 1; r <= 4; ++r)
            {
                _mm_storeu_ps(reinterpret_cast<float *>(vu0(v.vi15)), v.vf[r]);
                v.vi15 = static_cast<uint16_t>(v.vi15 + 1u);
            }
            pushedClean = rowsClean && !noClean;
            const uint32_t child = u32(b + 0x30u);
            __m128 *vf = v.vf;
            // MultiCurrentQuaternion(local)
            {
                const uint32_t v0 = currentQuaternion();
                const __m128 xyz = kXyz;
                vf[11] = q(v0);
                vf[12] = q(local);
                const __m128 c11 = vu::clamp(vf[11]), c12 = vu::clamp(vf[12]);
                vf[13] = vu::mulN(c11, c12);
                vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], lane<1>(vf[13])), kX);
                vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], lane<2>(vf[13])), kX);
                vf[13] = _mm_blendv_ps(vf[13], vu::subN(vf[13], lane<0>(vf[13])), kW);
                vf[14] = _mm_blendv_ps(vf[14], vu::mulN(c12, lane<3>(c11)), xyz);
                vf[15] = _mm_blendv_ps(vf[15], vu::mulN(c11, lane<3>(c12)), xyz);
                v.acc = _mm_blendv_ps(v.acc, vu::mulN(yzx(c12), zxy(c11)), xyz);
                vf[16] = _mm_blendv_ps(vf[16], vu::subN(v.acc, vu::mulN(yzx(c11), zxy(c12))), xyz);
                vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[14], vf[15]), xyz);
                vf[13] = _mm_blendv_ps(vf[13], vu::addN(vf[13], vf[16]), xyz);
                putq(v0, vf[13]);
                gp.s32(4, local);
            }
            // The bone's own VU0 block (bone() above, line for line).
            __m128 vf15;
            const __m128 quat = normalised(q(local), vf15);
            putq(local, quat);
            __m128 vf12 = _mm_blendv_ps(vf[12], vf12xyz, kXyz);
            __m128 vf11 = quat;
            __m128 vf14 = _mm_blendv_ps(vf[14], mr32w, kW);
            vf15 = _mm_blendv_ps(vf15, mr32w, kW);
            __m128 vf16 = _mm_blendv_ps(vf[16], mr32w, kW);
            __m128 acc = _mm_blendv_ps(v.acc, vu::mulN(yzx(vf11), zxy(vf12)), kXyz);
            vf11 = _mm_blendv_ps(vf11, vu::addN(acc, vf0sq), kXyz);
            const __m128 vf10 = _mm_blendv_ps(vf[10], vu::addN(acc, vu::mulN(vf11, vf12)), kXyz);
            const __m128 vf13 = PS2_VBLEND(vf[13], vu::mulN(vf11, vf10), kXyz);
            acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf11), zxy(vf10)), kXyz);
            const __m128 byW = vu::mulN(vf10, lane<3>(vf11));
            vf15 = _mm_blendv_ps(vf15, vu::addN(acc, byW), kXyz);
            vf16 = _mm_blendv_ps(vf16, vu::subN(acc, byW), kXyz);
            acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf13), zxy(vf12)), kXyz);
            __m128 vf17 = _mm_blendv_ps(vf[17], vu::addN(acc, vu::mulN(vf13, vf12)), kXyz);
            acc = _mm_blendv_ps(acc, vu::mulN(yzx(vf15), zxy(vf12)), kXyz);
            vf15 = _mm_blendv_ps(vf15, vu::addN(acc, vf0sq), kXyz);
            vf14 = PS2_VBLEND(vf14, vu::subN(vf12, vf17), kXyz);
            vf17 = _mm_blendv_ps(vf17, vf14, kYz);
            vf14 = _mm_blendv_ps(vf14, vf16, kY);
            vf16 = _mm_blendv_ps(vf16, vf15, kY);
            vf15 = _mm_blendv_ps(vf15, vf17, kY);
            vf14 = _mm_blendv_ps(vf14, vf15, kZ);
            vf15 = _mm_blendv_ps(vf15, vf16, kZ);
            vf16 = _mm_blendv_ps(vf16, vf17, kZ);
            putq(sp, vf14);
            putq(sp + 0x10u, vf15);
            putq(sp + 0x20u, vf16);
            putq(sp + 0x30u, vf0);
            __m128 vf5 = q(b + 0x10u);
            const __m128 vf6 = q(scale);
            vf5 = PS2_VBLEND(vf5, PS2_VMUL(vf5, vf6), kXyz);
            putq(sp + 0x40u, vf5);
            const float one = 1.0f;
            std::memcpy(rdram + ((sp + 0x4Cu) & PS2_RAM_MASK), &one, 4);
            v.f0 = true;
            vf[5] = vf5;
            vf[6] = vf6;
            vf[10] = vf10;
            vf[11] = vf11;
            vf[12] = vf12;
            vf[13] = vf13;
            vf[14] = vf14;
            vf[15] = vf15;
            vf[16] = vf16;
            vf[17] = vf17;
            v.acc = acc;
            // iosTransCurrentMatrix(sp + 0x40): offset x scale
            gp.s32(4, sp + 0x40u);
            vf[8] = q(sp + 0x40u);
            if (vf0Clean)
            {
                // vf8: vf5's xyz (a product) and 1.0; vf14..vf16 (blends of
                // results and of vf0's lanes) and vf0 as stored at sp: all
                // clamped already, as are their w lanes.
                const bool clean = rowsClean && !noClean;
                vu::Rows rows = clean ? vu::Rows{{vf[1], vf[2], vf[3], vf[4]}} : vu::clampRows(vf + 1);
                vf[4] = columnClean(rows, vf[8], lane<3>(vf0));
                rows.r[3] = vf[4]; // vf1..vf3 as they were (clamped above), vf4 a result
                gp.s32(4, sp);
                vf[14] = q(sp);
                vf[15] = q(sp + 0x10u);
                vf[16] = q(sp + 0x20u);
                vf[17] = q(sp + 0x30u);
                for (int c = 0; c < 4; ++c)
                    vf[14 + c] = columnClean(rows, vf[14 + c], lane<3>(vf[14 + c]));
                for (int c = 0; c < 4; ++c)
                    vf[1 + c] = vf[14 + c];
                rowsClean = true;
            }
            else
            {
                vf[4] = column(vu::clampRows(vf + 1), vf[8], lane<3>(vf0));
                // iosMulCurrentMatrixR(sp): rotation
                gp.s32(4, sp);
                for (int c = 0; c < 4; ++c)
                    vf[14 + c] = q(sp + 0x10u * c);
                const vu::Rows rows = vu::clampRows(vf + 1);
                for (int c = 0; c < 4; ++c)
                    vf[14 + c] = column(rows, vf[14 + c], lane<3>(vf[14 + c]));
                for (int c = 0; c < 4; ++c)
                    vf[1 + c] = vf[14 + c];
            }
            // iosGetCurrentMatrix
            gp.s32(4, matrices + i * 0x40u);
            for (int c = 0; c < 4; ++c)
                putq(matrices + i * 0x40u + 0x10u * c, vf[1 + c]);
            // GetCurrentQuaternion, normalised into the results
            {
                const uint32_t v0 = currentQuaternion();
                __m128 n15;
                putq(quats + i * 16u, normalised(q(v0), n15));
                vf[14] = q(v0);
                vf[15] = n15;
            }
            return child;
        }
        __attribute__((always_inline)) void leave(bool restoredClean)
        {
            rowsClean = restoredClean && !noClean;
            // iosPopCurrentMatrix
            for (int r = 4; r >= 1; --r)
            {
                v.vi15 = static_cast<uint16_t>(v.vi15 - 1u);
                v.vf[r] = _mm_loadu_ps(reinterpret_cast<const float *>(vu0(v.vi15)));
            }
            // PopQuaternion
            int32_t n = static_cast<int32_t>(u32(Ctx::kQuatTop)) - 1;
            const bool negative = n < 0;
            if (negative)
                n = 0;
            gp.s32(2, Ctx::kQuatTop);
            gp.s32(3, static_cast<uint32_t>(n));
            gp.u64(4, negative ? 1u : 0u);
            put32(Ctx::kQuatTop, static_cast<uint32_t>(n));
        }
        __attribute__((always_inline)) void run(uint32_t root, uint32_t sp0)
        {
            std::vector<Frame> &st = t_frames;
            const size_t base = st.size();
            uint32_t cur = root, curSp0 = sp0;
            for (;;)
            {
                // into bone cur
                const uint32_t sp = curSp0 - 0xB0u;
                uint32_t sibling;
                const uint32_t child = enter(cur, sp, &sibling);
                st.push_back({sibling, sp, 0u, pushedClean});
                // 60 matrices pushed (240 of VU0's 256 quadwords): deeper, a
                // push could wrap onto a pushed matrix; no clean rows after.
                if (st.size() - base > 60u)
                    noClean = true;
                if (child != 0xFFFFFFFFu)
                {
                    setArgs(child, 0x010B8734u, sp);
                    cur = child;
                    curSp0 = sp;
                    continue;
                }
                // back up to a frame with a sibling still to do
                bool next = false;
                while (st.size() > base)
                {
                    Frame &f = st.back();
                    if (f.phase == 0u)
                    {
                        leave(f.rowsClean);
                        if (f.sibling != 0xFFFFFFFFu)
                        {
                            f.phase = 1u;
                            setArgs(f.sibling, 0x010B8768u, f.sp);
                            cur = f.sibling;
                            curSp0 = f.sp;
                            next = true;
                            break;
                        }
                    }
                    st.pop_back();
                }
                if (!next)
                    break;
            }
        }
    };

    bool fastSites(uint8_t *rdram)
    {
        static constexpr uint32_t kSites[9][2] = {{0x010B85F0u, 0x001BBB48u}, {0x010B85F8u, 0x001B8638u}, {0x010B8604u, 0x001BB8A8u},
                                                  {0x010B86C0u, 0x001B8878u}, {0x010B86C8u, 0x001B8CD0u}, {0x010B86D8u, 0x001B8C90u},
                                                  {0x010B86E0u, 0x001BB868u}, {0x010B8734u, 0x001B8650u}, {0x010B873Cu, 0x001BBC08u}};
        for (const auto &s : kSites)
            if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                return false;
        return true;
    }

    void skeleton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // SOTC_SKELETON_FAST=0: bone() always; SOTC_SKELETON_FAST_AB=<n>: the two in turns of n game frames.
        static const bool fastOn = !std::getenv("SOTC_SKELETON_FAST") || std::atoi(std::getenv("SOTC_SKELETON_FAST")) != 0;
        static const uint64_t ab = std::getenv("SOTC_SKELETON_FAST_AB") ? std::strtoull(std::getenv("SOTC_SKELETON_FAST_AB"), nullptr, 10) : 0u;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        if (fastOn && (!ab || (sotc_decomp::g_gameFrame / ab) % 2u == 0u) && fastSites(rdram))
        {
            Fast f;
            f.rdram = rdram;
            f.ctx = ctx;
            f.runtime = runtime;
            f.vu0mem = runtime->memory().getVU0Data();
            f.matrices = GPR_U32(ctx, 5);
            f.quats = GPR_U32(ctx, 6);
            f.skeleton = GPR_U32(ctx, 7);
            f.locals = GPR_U32(ctx, 8);
            f.scale = GPR_U32(ctx, 9);
            f.vf0 = ctx->vu0_vf[0];
            f.vf0c = vu::clamp(f.vf0);
            f.vf0sq = vu::mulN(f.vf0c, f.vf0c);
            f.vf0w = lane<3>(f.vf0);
            f.mr32w = _mm_shuffle_ps(f.vf0, f.vf0, _MM_SHUFFLE(0, 3, 2, 1));
            f.vf12xyz = vu::addN(f.vf0c, lane<3>(f.vf0c));
            f.vf0Clean = std::memcmp(&f.vf0, &f.vf0c, 16) == 0;
            f.v.load(ctx);
            f.run(GPR_U32(ctx, 4), sp0);
            f.gp.s32(29, sp0);
            f.gp.s32(31, ra0);
            f.gp.flush(ctx);
            f.v.store(ctx);
            returnTo(ctx);
            return;
        }
        const Ctx g{rdram, ctx, runtime};
        bone(g, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9), sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010B8590u, skeletonPose, decomp_skeleton::skeleton);
namespace decomp_skeleton
{
    // GetMotionMatrix (0x10b1d60, round 41): a whole pose from its root. a0
    // the matrices, a1 the quaternions out, a2 the skeleton, a3 the local
    // rotations, t0 the root's position, t1 the scale. The current
    // quaternion set from 0x1e0f40 (relocated), the current matrix the unit
    // one translated by t0, then node 0 as manager_sub_010B8590 does a node
    // (the same frame offsets, its quaternion into a1 + 0), its children and
    // siblings by it: Fast above from node 0, on GetMotionMatrix's frame. v0:
    // the root's sibling (-1) when it has none (the original's last load).
    // a1 == 0 (the pose without quaternions, 010B8798) and helper sites that
    // name other functions: the recompiled code.
    bool motionMatrixSites(uint8_t *rdram)
    {
        static constexpr uint32_t kSites[14][2] = {{0x010B1DA8u, 0x001BB988u}, {0x010B1DB0u, 0x001B8620u}, {0x010B1DB8u, 0x001B8878u},
                                                   {0x010B1DD8u, 0x001BBB48u}, {0x010B1DE0u, 0x001B8638u}, {0x010B1DF0u, 0x001BB8A8u},
                                                   {0x010B1EB4u, 0x001B8878u}, {0x010B1EBCu, 0x001B8CD0u}, {0x010B1ECCu, 0x001B8C90u},
                                                   {0x010B1ED4u, 0x001BB868u}, {0x010B1F24u, 0x010B8590u}, {0x010B1F2Cu, 0x001B8650u},
                                                   {0x010B1F34u, 0x001BBC08u}, {0x010B1F58u, 0x010B8590u}};
        for (const auto &s : kSites)
            if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                return false;
        return true;
    }
    void motionMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool fastOn = !std::getenv("SOTC_SKELETON_FAST") || std::atoi(std::getenv("SOTC_SKELETON_FAST")) != 0;
        if (!sotc_decomp::motionOn() || !fastOn || GPR_U64(ctx, 5) == 0u || !fastSites(rdram) || !motionMatrixSites(rdram))
            return sotc_decomp::recompiled(0x010B1D60u)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x100u;
        Fast f;
        f.rdram = rdram;
        f.ctx = ctx;
        f.runtime = runtime;
        f.vu0mem = runtime->memory().getVU0Data();
        f.matrices = GPR_U32(ctx, 4);
        f.quats = GPR_U32(ctx, 5);
        f.skeleton = GPR_U32(ctx, 6);
        f.locals = GPR_U32(ctx, 7);
        f.scale = GPR_U32(ctx, 9);
        const uint32_t pos = GPR_U32(ctx, 8);
        f.vf0 = ctx->vu0_vf[0];
        f.vf0c = vu::clamp(f.vf0);
        f.vf0sq = vu::mulN(f.vf0c, f.vf0c);
        f.vf0w = lane<3>(f.vf0);
        f.mr32w = _mm_shuffle_ps(f.vf0, f.vf0, _MM_SHUFFLE(0, 3, 2, 1));
        f.vf12xyz = vu::addN(f.vf0c, lane<3>(f.vf0c));
        f.vf0Clean = std::memcmp(&f.vf0, &f.vf0c, 16) == 0;
        f.v.load(ctx);
        // SetCurrentQuaternion(0x1e0f40)
        {
            const uint32_t src = sotc_decomp::relocated(rdram, 0x010B1D64u, 0x010B1D78u);
            const uint32_t top = Ctx::kQuatStack + (f.u32(Ctx::kQuatTop) << 4);
            f.putq(top, f.q(src));
        }
        // iosUnitCurrentMatrix: vf4 = vf0, vf3..vf1 each the one above rotated (vmr32)
        __m128 *vf = f.v.vf;
        vf[4] = f.vf0;
        vf[3] = _mm_shuffle_ps(vf[4], vf[4], _MM_SHUFFLE(0, 3, 2, 1));
        vf[2] = _mm_shuffle_ps(vf[3], vf[3], _MM_SHUFFLE(0, 3, 2, 1));
        vf[1] = _mm_shuffle_ps(vf[2], vf[2], _MM_SHUFFLE(0, 3, 2, 1));
        // iosTransCurrentMatrix(t0)
        vf[8] = f.q(pos);
        vf[4] = f.column(vu::clampRows(vf + 1), vf[8], f.vf0w);
        const uint32_t rootSibling = f.u32(f.u32(f.skeleton + 8u) + 0x34u);
        f.run(0u, sp + 0xB0u);
        if (rootSibling == 0xFFFFFFFFu)
            f.gp.s32(2, 0xFFFFFFFFu);
        f.gp.s32(29, sp0);
        f.gp.s32(31, ra0);
        f.gp.flush(ctx);
        f.v.store(ctx);
        returnTo(ctx);
    }
}
SOTC_DECOMP_FUNCTION(0x010B1D60u, GetMotionMatrix, decomp_skeleton::motionMatrix);

namespace decomp_skeleton
{
    // regularizeMatAry (0x10c1a98, round 11): a1 quaternions at a0 each
    // normalised in place, the VU0 block normalised() does (vf14 the
    // quaternion, vf15 the result, Q). A negative count: the recompiled code.
    void regularizeMatAry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int32_t n = static_cast<int32_t>(GPR_U32(ctx, 5));
        uint32_t a = GPR_U32(ctx, 4);
        if (n < 0 || n > 0x10000 || a >= PS2_RAM_SIZE - 16u * static_cast<uint32_t>(n))
            return sotc_decomp::recompiled(0x010C1A98u)(rdram, ctx, runtime);
        for (int32_t i = 0; i < n; ++i, a += 16u)
        {
            const uint32_t at = a & ~0xFu;
            __m128 v, vf15;
            std::memcpy(&v, rdram + at, 16);
            ctx->vu0_vf[14] = v;
            const __m128 r = normalised(ctx, v, vf15);
            ctx->vu0_vf[15] = r;
            std::memcpy(rdram + at, &r, 16);
        }
        if (n > 0)
            SET_GPR_S32(ctx, 3, -1);
        SET_GPR_U32(ctx, 4, a);
        SET_GPR_S32(ctx, 2, -1);
        returnTo(ctx);
    }
}
SOTC_DECOMP_FUNCTION(0x010C1A98u, regularizeMatAry, decomp_skeleton::regularizeMatAry);
