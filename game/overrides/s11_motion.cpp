// Round S11 (SOTC_S11RT part kMotion, s11rt.h): the motion root's key at a float frame as host code.
//
// GetMotionRootFloatFrame (0x10b4478, manager) is the hottest recompiled function left riding (S11's profile: 0.12% of
// the game thread) and calls manager_sub_010AD048 (the root track's key at an integer frame) twice a call through the
// table: round 126's wrapper (cam126::rootKeyEntry, ctl126 off by default) -> a hash lookup for the recompiled function
// (sotc_decomp::recompiled, 0.09%) -> D5's lifted copy (0.12%). Here both are one host routine over named views of the
// motion data, plain arguments and values; only the guest's visible results are written back (the 16 bytes out, v0 /
// f0, the VU0 registers the original's lerp leaves).
//
// The data, as the code reads it (names from the code's use):
//   MotionRef   a u32 word in a motion user: low 18 bits = the byte distance back to the owner pointer, bits 18..29
//               the motion's index.
//   MotionSet   (the owner): +0x08 its descriptor (the motion resource kind: GetMotionRootFloatFrame works on one kind,
//               a relocated constant, 0x1b7b60 in the disassembly), +0x10 the motions; descriptor +0x0C the stride.
//   Motion      +0x0C its root info (0: the original prints an error), root info +0x20 the root track.
//   RootTrack   +0x00 s16 kind. Kind < 6: +0x0C -> float3 per frame. Else +0x06 low 4 bits the encoding and +0x0C its
//               data D: 0 a constant (D +0..+8), 2 int16x3 per frame at *(D + 0x18), each times its scale (D +4 / +0xC /
//               +0x14) x 2^-15 (1 + 2^-15) plus its base (D +0 / +8 / +0x10), other a float3 per frame at D.
//
// Anything else (an address outside main RAM, a motion of another kind, the error print, the blend base's recursion)
// runs the recompiled function from its start: nothing is written before those tests.
//
// SOTC_S11RT_CHECK=1: each call computes the host answer without writing, then runs the recompiled function and
// compares the 16 bytes out, v0, f0 and VU0 vf5..vf8 / ACC with it ([s11] log: motion checks / differ).
#include "s11_motion.h"

namespace s11motion
{
    using sotc_decomp::returnTo;

    Result compute(const uint8_t *ram, R5900Context *ctx)
    {
        Result r;
        const uint32_t out = GPR_U32(ctx, 4), ref = GPR_U32(ctx, 5);
        if (!okAddr(out) || !okAddr(ref) || (ref & 3u) != 0u || (out & 0xFu) != 0u)
            return r;
        const uint32_t word = rd32(ram, ref);
        // The resource kind test (0x10b44bc..0x10b44e0): the owner, not the global at 0x1e0ed4, its descriptor the kind.
        const uint32_t g1 = sotc_decomp::relocated(ram, 0x010B44C0u, 0x010B44C4u);
        if (!okAddr(g1 & ~3u))
            return r;
        const uint32_t owner1 = ownerOf(ram, ref, word, rd32(ram, g1));
        if (!okAddr(owner1))
            return r; // (a null owner reads address 8: the recompiled code)
        if (rd32(ram, owner1 + layout::kSetDescriptor) != sotc_decomp::relocated(ram, 0x010B44D4u, 0x010B44DCu))
            return r; // another kind: the blend base's path (recompiled)
        // The motion (0x10b4504..0x10b4530).
        const uint32_t g2 = sotc_decomp::relocated(ram, 0x010B4508u, 0x010B450Cu);
        if (!okAddr(g2 & ~3u))
            return r;
        const uint32_t owner = ownerOf(ram, ref, word, rd32(ram, g2));
        if (!okAddr(owner))
            return r;
        const uint32_t desc = rd32(ram, owner + layout::kSetDescriptor);
        if (!okAddr(desc))
            return r;
        const uint32_t index = (word >> layout::kRefIndexShift) & layout::kRefIndexMask;
        const uint32_t motion = rd32(ram, owner + layout::kSetMotions) + index * rd32(ram, desc + layout::kDescStride);
        if (!okAddr(motion))
            return r;
        const uint32_t root = rd32(ram, motion + layout::kMotionRoot);
        if (root == 0u || !okAddr(root))
            return r; // the error print (recompiled)
        const uint32_t track = rd32(ram, root + layout::kRootTrack);
        // The frame (0x10b4558..0x10b457c).
        const float frame = ctx->f[12];
        const int32_t i = FPU_CVT_W_S(frame);
        const float frac = FPU_SUB_S(frame, FPU_CVT_S_W(i));
        if (FPU_C_EQ_S(frac, 0.0f))
        {
            const Key k = rootKey(ram, track, static_cast<uint32_t>(i));
            if (!k.ok)
            {
                s11rt::g_st.motionPacked += k.packed;
                return r;
            }
            r.key = k;
            r.packed = k.packedHost;
            r.out = _mm_setr_ps(k.v[0], k.v[1], k.v[2], 1.0f);
            r.v0 = k.v0;
            r.v0Full = k.full;
            r.v0Upper = keyUpper(k.v);
            r.cEq = true;
            r.ok = true;
            return r;
        }
        const Key k1 = rootKey(ram, track, static_cast<uint32_t>(i));
        const Key k2 = rootKey(ram, track, static_cast<uint32_t>(i) + 1u);
        if (!k1.ok || !k2.ok)
        {
            s11rt::g_st.motionPacked += k1.packed || k2.packed;
            return r;
        }
        r.key = k1.packedHost ? k1 : k2;
        r.packed = k1.packedHost || k2.packedHost;
        r.v0Full = k2.full; // (both keys come from one track: one kind)
        r.v0Upper = keyUpper(k2.v);
        r.cEq = false;
        // The lerp as the original's VU0 code (0x10b45b4..0x10b45d4), the runtime's own VU0 operations.
        const uint32_t sp = GPR_U32(ctx, 29) - 0x50u;
        const uint64_t t0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(bitsf(frac))));
        __m128i t0v = ctx->r[8];
        {
            uint64_t lanes[2];
            std::memcpy(lanes, &t0v, 16);
            lanes[0] = t0;
            std::memcpy(&t0v, lanes, 16);
        }
        const __m128 vf0 = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
        const __m128 vf7 = _mm_castsi128_ps(t0v);
        const __m128 vf5in = _mm_setr_ps(k2.v[0], k2.v[1], k2.v[2], k2.v[3]);
        const __m128 vf6 = _mm_setr_ps(k1.v[0], k1.v[1], k1.v[2], k1.v[3]);
        const __m128 x = _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 vf8 = PS2_VSUB_K(1, 0, vf0, x);
        vf8 = _mm_blendv_ps(ctx->vu0_vf[8], vf8, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
        const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        const __m128 acc = _mm_blendv_ps(ctx->vu0_acc, PS2_VMUL(vf5in, x), xyz);
        const __m128 mul = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3, 3, 3, 3)));
        const __m128 vf5 = _mm_blendv_ps(vf5in, PS2_VADD_K(1, 1, acc, mul), xyz);
        r.vf5 = vf5;
        r.vf6 = vf6;
        r.vf7 = vf7;
        r.vf8 = vf8;
        r.acc = acc;
        r.t0 = t0;
        r.lerp = true;
        float o[4];
        _mm_storeu_ps(o, vf5);
        o[3] = 1.0f;
        r.out = _mm_loadu_ps(o);
        r.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp + 0x10u)));
        r.ok = true;
        return r;
    }

    PS2Runtime::RecompiledFunction g_rec = nullptr;
    PS2Runtime::RecompiledFunction rec()
    {
        if (__builtin_expect(g_rec == nullptr, 0))
            g_rec = sotc_decomp::recompiled(kSelf);
        return g_rec;
    }

    void rootFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!s11rt::on(s11rt::kMotion))
            return rec()(rdram, ctx, runtime);
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
            s13mot::emuSync();
        const Result r = compute(rdram, ctx);
        if (!r.ok)
        {
            ++s11rt::g_st.motionFallback;
            return rec()(rdram, ctx, runtime);
        }
        // (the check runs on the game thread only: on round 91's guest worker a guest call can park the job and return
        // early, so the recompiled leg is not a whole run there)
        const bool s13check = s13mot::g_check; // (all kMotion calls: kPacked's keys and the v0 / condition bit S13 adds)
        if (__builtin_expect(s11rt::g_check || s13check, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            // the recompiled function on the live state, then its results against the host answer
            const uint32_t out = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31);
            // (round S13: the float mode before and after the recompiled leg, and the call's inputs kept to answer it
            // again after the leg)
            const uint32_t fpscrBefore = s13mot::fpscr();
            static R5900Context saved;
            saved = *ctx;
            // (the experiment on a packed key: the answer again before the leg, then again after the control word is
            // written back unchanged)
            Result pre{}, rewritten{};
            if (s13check && r.packed)
            {
                pre = compute(rdram, &saved);
                s13mot::fpscrRewrite();
                rewritten = compute(rdram, &saved);
            }
            rec()(rdram, ctx, runtime);
            const uint32_t fpscrAfter = s13mot::fpscr();
            if (ctx->pc != ra)
            {
                // a checkpoint inside the recompiled leg unwound it to the scheduler (it resumes there later): not a
                // whole run, nothing to compare
                ++s11rt::g_st.motionUnwound;
                s13mot::g_st.unwound += s13check;
                return;
            }
            if (s13check && fpscrBefore != fpscrAfter)
                ++s13mot::g_st.fpscrMoved;
            float got[4];
            std::memcpy(got, rdram + out, 16);
            const __m128 g = _mm_loadu_ps(got);
            bool ok = same(g, r.out) && GPR_U64(ctx, 2) == r.v0 && bitsf(ctx->f[0]) == bitsf(1.0f);
            if (r.lerp)
                ok = ok && same(ctx->vu0_vf[5], r.vf5) && same(ctx->vu0_vf[6], r.vf6) && same(ctx->vu0_vf[7], r.vf7) && same(ctx->vu0_vf[8], r.vf8) &&
                     same(ctx->vu0_acc, r.acc) && GPR_U64(ctx, 8) == r.t0;
            if (s13check)
            {
                // round S13: v0's upper half (when the key path sets it) and the FPU condition bit too
                uint64_t upper;
                std::memcpy(&upper, reinterpret_cast<const uint8_t *>(&ctx->r[2]) + 8, 8);
                ok = ok && (!r.v0Full || upper == r.v0Upper) && ((ctx->fcr31 & 0x800000u) != 0u) == r.cEq;
            }
            ++s11rt::g_st.motionChecks;
            s13mot::g_st.checks += s13check;
            if (!ok && s13check)
            {
                // the host answer again, after the recompiled leg, from the call's inputs (the input data unchanged)
                const Result again = compute(rdram, &saved);
                const bool againSame = again.ok && same(again.out, g);
                if (r.packed)
                    ++s13mot::g_st.packedDiffer[(pre.ok && same(pre.out, g) ? 1 : 0) | (rewritten.ok && same(rewritten.out, g) ? 2 : 0) | (againSame ? 4 : 0)];
                if (s13mot::differ(0))
                {
                    float h[4];
                    _mm_storeu_ps(h, r.out);
                    std::fprintf(stderr, "[s13] motion differ (packed %d): out %08x %08x %08x %08x host %08x %08x %08x %08x lerp %d fpscr %08x -> %08x, host again %s; raw %d %d %d data %08x: %08x %08x %08x %08x %08x %08x ra %08x\n",
                                 r.packed ? 1 : 0, bitsf(got[0]), bitsf(got[1]), bitsf(got[2]), bitsf(got[3]), bitsf(h[0]), bitsf(h[1]), bitsf(h[2]), bitsf(h[3]), r.lerp ? 1 : 0,
                                 fpscrBefore, fpscrAfter, againSame ? "same as the leg" : "differs too", r.key.raw[0], r.key.raw[1], r.key.raw[2], r.key.data,
                                 rd32(rdram, r.key.data), rd32(rdram, r.key.data + 4u), rd32(rdram, r.key.data + 8u), rd32(rdram, r.key.data + 0xCu),
                                 rd32(rdram, r.key.data + 0x10u), rd32(rdram, r.key.data + 0x14u), ra);
                }
            }
            if (!ok)
            {
                if (s11rt::g_st.motionDiffer++ < 16u)
                {
                    float h[4];
                    _mm_storeu_ps(h, r.out);
                    std::fprintf(stderr, "[s11] motion differ: out %08x %08x %08x %08x host %08x %08x %08x %08x v0 %llx host %llx lerp %d f0 %08x pc %08x ra %08x data %08x: %08x %08x %08x %08x %08x %08x\n",
                                 bitsf(got[0]), bitsf(got[1]), bitsf(got[2]), bitsf(got[3]), bitsf(h[0]), bitsf(h[1]), bitsf(h[2]), bitsf(h[3]),
                                 (unsigned long long)GPR_U64(ctx, 2), (unsigned long long)r.v0, r.lerp ? 1 : 0, bitsf(ctx->f[0]), ctx->pc, GPR_U32(ctx, 31),
                                 r.key.data, r.key.data ? rd32(rdram, r.key.data) : 0u, r.key.data ? rd32(rdram, r.key.data + 4u) : 0u,
                                 r.key.data ? rd32(rdram, r.key.data + 8u) : 0u, r.key.data ? rd32(rdram, r.key.data + 0xCu) : 0u,
                                 r.key.data ? rd32(rdram, r.key.data + 0x10u) : 0u, r.key.data ? rd32(rdram, r.key.data + 0x14u) : 0u);
                }
            }
            return;
        }
        apply(rdram, ctx, r);
        ++s11rt::g_st.motionHost;
        s13mot::g_st.packedHost += r.packed;
        if (sotc_decomp::onWorkerLane(runtime))
            ++s11rt::g_st.motionWorker;
        returnTo(ctx);
    }
}

#include "d1_native.h"
namespace s11motion
{
    void rootFloatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D2 (d1native): below
}

SOTC_DECOMP_FUNCTION(0x010B4478u, GetMotionRootFloatFrame, s11motion::rootFloatFrameSw);
D1_ALIAS(0x010B4478u, GetMotionRootFloatFrame, s11motion::rootFloatFrameSw, s11motion::rootFloatFrame);

// ---- Round D2 (switch d1native, d1_native.h): GetMotionRootFloatFrame as native C++ (full) ------------------------------
// The S11 host path (compute + apply) with the arguments read once by the adapter: the core takes the call's inputs as
// C++ values (the three relocated words decoded by the adapter, the registers the lerp keeps lanes of) and typed guest
// memory, returns the registers the old body leaves, and writes the 16 bytes out itself. The frame's FPU ops as d1::
// (cvt.w.s by the runtime's ps2FpuConvertToWord, sub / c.eq as d1::sub / d1::eq); the lerp's three VU0 ops as d1 vec4 ops
// (operand clamps dropped: the keys are motion data, the fraction a finite FPU result; the only stale lanes, vf7's y..w
// and ACC's w, are never computed into a kept lane), dest masks as d1::vsel. Refusals (a packed key while neither
// SOTC_S11RT_PACKED nor s13mot's kPacked is on, any address test S11 makes) run the old body, which refuses the same way.
namespace s11motion
{
    D1_COUNTER(c_d2RootFloat, "GetMotionRootFloatFrame");

    // The root track's key as rootKey (S11's view): xyz, v0, whether v0 is all 128 bits; false: the old body's refusal.
    struct KeyD2
    {
        float v[4];
        uint64_t v0;
        bool full;
    };
    bool keyD2(const d1::Mem &m, uint32_t track, uint32_t frame, bool packedHost, KeyD2 &k)
    {
        k.v[3] = 1.0f;
        k.full = false;
        if (!okAddr(track))
            return false;
        const int16_t kind = m.get<int16_t>(track + layout::kTrackKind);
        const uint32_t data = m.u32(track + layout::kTrackData);
        if (kind < 6)
        {
            const uint32_t p = data + frame * 12u;
            if (!okAddr(p))
                return false;
            k.v[0] = m.f32(p);
            k.v[1] = m.f32(p + 4u);
            k.v[2] = m.f32(p + 8u);
            k.v0 = static_cast<uint64_t>(m.u32(p)) | (static_cast<uint64_t>(m.u32(p + 4u)) << 32);
            k.full = true;
            return true;
        }
        const uint16_t enc16 = m.get<uint16_t>(track + layout::kTrackEncoding);
        const uint32_t enc = enc16 & 0xFu;
        if (!okAddr(data))
            return false;
        if (enc == 0u)
        {
            k.v[0] = m.f32(data);
            k.v[1] = m.f32(data + 4u);
            k.v[2] = m.f32(data + 8u);
            k.v0 = enc16;
            return true;
        }
        if (enc == 2u)
        {
            if (!packedHost)
                return false;
            const uint32_t p = m.u32(data + layout::kPackedTable) + frame * 6u;
            if (!okAddr(p))
                return false;
            const float scale = fbits(0x38000100u); // 2^-15 (1 + 2^-15)
            int16_t last = 0;
            for (uint32_t i = 0; i < 3u; ++i)
            {
                last = m.get<int16_t>(p + 2u * i);
                const float raw = static_cast<float>(static_cast<int32_t>(last));
                k.v[i] = d1::add(d1::mul(d1::mul(raw, m.f32(data + 4u + 8u * i)), scale), m.f32(data + 8u * i));
            }
            k.v0 = static_cast<uint64_t>(static_cast<int64_t>(last));
            return true;
        }
        const uint32_t p = data + frame * 12u;
        if (!okAddr(p))
            return false;
        k.v[0] = m.f32(p);
        k.v[1] = m.f32(p + 4u);
        k.v[2] = m.f32(p + 8u);
        k.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(p)));
        return true;
    }

    // The call's inputs (a0 out, a1 the MotionRef, f12 the frame; sp, t0's 128 bits, vf8 / ACC whose lanes the lerp keeps)
    // and the three relocated words of the original's code (g1 / g2 the two globals' addresses, kind the resource kind).
    struct RootFrameD2In
    {
        __m128 t0v, vf8, acc;
        uint32_t out, ref, g1, kindWord, g2, sp;
        float frame;
        bool packedHost;
    };
    // The registers the old body leaves (apply): v0 (and its upper half when full), the c.eq condition, the lerp's VU0
    // registers and t0.
    struct RootFrameD2Out
    {
        __m128 vf5, vf6, vf7, vf8, acc;
        uint64_t v0, v0Upper, t0;
        bool v0Full, cEq, lerp;
    };
    bool rootFrameD2Core(const d1::Mem &m, const RootFrameD2In &in, RootFrameD2Out &o)
    {
        if (!okAddr(in.out) || !okAddr(in.ref) || (in.ref & 3u) != 0u || (in.out & 0xFu) != 0u)
            return false;
        const uint32_t word = m.u32(in.ref);
        if (!okAddr(in.g1 & ~3u))
            return false;
        const uint32_t off = word & layout::kRefOffsetMask;
        const uint32_t owner1 = off != 0u ? m.u32(in.ref - off) : 0u;
        if (owner1 == 0u || owner1 == m.u32(in.g1) || !okAddr(owner1))
            return false;
        if (m.u32(owner1 + layout::kSetDescriptor) != in.kindWord)
            return false;
        if (!okAddr(in.g2 & ~3u))
            return false;
        const uint32_t owner = off != 0u ? m.u32(in.ref - off) : 0u;
        if (owner == 0u || owner == m.u32(in.g2) || !okAddr(owner))
            return false;
        const uint32_t desc = m.u32(owner + layout::kSetDescriptor);
        if (!okAddr(desc))
            return false;
        const uint32_t index = (word >> layout::kRefIndexShift) & layout::kRefIndexMask;
        const uint32_t motion = m.u32(owner + layout::kSetMotions) + index * m.u32(desc + layout::kDescStride);
        if (!okAddr(motion))
            return false;
        const uint32_t root = m.u32(motion + layout::kMotionRoot);
        if (root == 0u || !okAddr(root))
            return false;
        const uint32_t track = m.u32(root + layout::kRootTrack);
        // The frame: cvt.w.s, the fraction (sub.s), c.eq.s against 0.
        const int32_t i = ps2FpuConvertToWord(in.frame);
        const float frac = d1::sub(in.frame, static_cast<float>(i));
        float key[4];
        if (d1::eq(frac, 0.0f))
        {
            KeyD2 k;
            if (!keyD2(m, track, static_cast<uint32_t>(i), in.packedHost, k))
                return false;
            key[0] = k.v[0];
            key[1] = k.v[1];
            key[2] = k.v[2];
            key[3] = 1.0f;
            o.v0 = k.v0;
            o.v0Full = k.full;
            o.v0Upper = keyUpper(k.v);
            o.cEq = true;
            o.lerp = false;
        }
        else
        {
            KeyD2 k1, k2;
            if (!keyD2(m, track, static_cast<uint32_t>(i), in.packedHost, k1) || !keyD2(m, track, static_cast<uint32_t>(i) + 1u, in.packedHost, k2))
                return false;
            o.v0Full = k2.full;
            o.v0Upper = keyUpper(k2.v);
            o.cEq = false;
            // The lerp (0x10b45b4..0x10b45d4): vf7 = t0's 128 bits (the fraction's bits, sign-extended, in the low word),
            // vf8.w = 1 - x, ACC.xyz = k2 x, vf5.xyz = ACC + k1 vf8.w.
            const uint64_t t0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(bitsf(frac))));
            __m128 vf7 = in.t0v;
            std::memcpy(&vf7, &t0, 8);
            const __m128 vf5in = d1::make(k2.v[0], k2.v[1], k2.v[2], k2.v[3]);
            const __m128 vf6 = d1::make(k1.v[0], k1.v[1], k1.v[2], k1.v[3]);
            const __m128 x = d1::bc<0>(vf7);
            const __m128 xyz = d1::mask(1, 1, 1, 0);
            const __m128 vf8 = d1::vsel(in.vf8, d1::vsub(d1::vf0(), x), d1::mask(0, 0, 0, 1));
            const __m128 acc = d1::vsel(in.acc, d1::vmul(vf5in, x), xyz);
            const __m128 vf5 = d1::vsel(vf5in, d1::vadd(acc, d1::vmul(vf6, d1::bc<3>(vf8))), xyz);
            o.vf5 = vf5;
            o.vf6 = vf6;
            o.vf7 = vf7;
            o.vf8 = vf8;
            o.acc = acc;
            o.t0 = t0;
            o.lerp = true;
            key[0] = d1::lane<0>(vf5);
            key[1] = d1::lane<1>(vf5);
            key[2] = d1::lane<2>(vf5);
            key[3] = 1.0f;
            o.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(in.sp + 0x10u)));
        }
        m.putq(in.out, d1::make(key[0], key[1], key[2], key[3]));
        return true;
    }

    void rootFloatFrameD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!s11rt::on(s11rt::kMotion) || s11rt::g_check || s13mot::g_check)
            return rootFloatFrame(rdram, ctx, runtime);
        RootFrameD2In in;
        in.out = GPR_U32(ctx, 4);
        in.ref = GPR_U32(ctx, 5);
        in.frame = ctx->f[12];
        in.sp = GPR_U32(ctx, 29) - 0x50u;
        in.t0v = _mm_castsi128_ps(ctx->r[8]);
        in.vf8 = ctx->vu0_vf[8];
        in.acc = ctx->vu0_acc;
        in.g1 = sotc_decomp::relocated(rdram, 0x010B44C0u, 0x010B44C4u);
        in.kindWord = sotc_decomp::relocated(rdram, 0x010B44D4u, 0x010B44DCu);
        in.g2 = sotc_decomp::relocated(rdram, 0x010B4508u, 0x010B450Cu);
        in.packedHost = s11rt::g_packed || s13mot::on(s13mot::kPacked);
        RootFrameD2Out o;
        if (!rootFrameD2Core(d1::Mem{rdram}, in, o))
            return rootFloatFrame(rdram, ctx, runtime); // (nothing written: the old body refuses the same way)
        ++c_d2RootFloat.n;
        SET_GPR_U64(ctx, 2, o.v0);
        if (o.v0Full)
            std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[2]) + 8, &o.v0Upper, 8);
        ctx->fcr31 = o.cEq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        ctx->f[0] = 1.0f;
        if (o.lerp)
        {
            ctx->vu0_vf[5] = o.vf5;
            ctx->vu0_vf[6] = o.vf6;
            ctx->vu0_vf[7] = o.vf7;
            ctx->vu0_vf[8] = o.vf8;
            ctx->vu0_acc = o.acc;
            SET_GPR_U64(ctx, 8, o.t0);
        }
        returnTo(ctx);
    }
    void rootFloatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            rootFloatFrameD1(rdram, ctx, runtime);
        else
            rootFloatFrame(rdram, ctx, runtime);
    }
} // namespace s11motion
