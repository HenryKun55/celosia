// manager_sub_010C6238 (round 41): a character's gaze added to its pose,
// node by node (called by GetCharacterClassGazeAddMotions).
//
// a0 the pose's quaternions (16 bytes a node), a1 the gaze record (+4 the
// node count), a2 the gaze: +0 x, +4 y (each clamped to [-1, 1]), +8 the
// gaze motion's resource. The direction's angle (iosGetATan2fExact(x, y),
// + 2 pi when negative) picks two of the motion's eight direction keys
// (k = angle x 8 / 2 pi: keys (k & 7) + 1 and (k + 1 & 7) + 1, the fraction
// between them), the length sqrt(x^2 + y^2) how much of each. For every node
// from the last down to 0: the motion's key 0 (base), key A and key B
// (GetMotionNodeIntFrameDirectResource -> curveEval), A and B taken relative
// to the base (conjugate x key), each blended from the identity quaternion
// (0x1e0f40, relocated) by the length (vmulax / vmaddw, the identity negated
// when its dot with the key is negative), normalised; then B towards A by the
// fraction, normalised twice; the node's quaternion = node x that.
//
// The recompiled code's VU0 / FPU statements in its order (PS2_V* / FPU_*
// macros, Q from ps2VuRsqrt with the status as it does, the constants by
// their bits), on locals; the frame (0x110 bytes, all dead after the return)
// is not written but where curveEval writes its keys (sp, sp + 0x10,
// sp + 0x20), read back from there. GetMotionNodeIntFrameDirectResource's
// own loads are done here (its frame is dead stack) and curveEval is called
// as its tail jump would: ra the jal's return, sp this frame's.
// iosGetATan2fExact is called through the guest (native, decomp_quat.cpp).
// v0 / f0 as the original leaves them (the last dot product's bits, or the
// key index / its float when there are no nodes); f20-f22 and the
// callee-saved GPRs are never touched; the VU0 registers, Q and the status
// written back. A jal site naming another function: the recompiled code.
// SOTC_MOTION=0 puts it back; alone with SOTC_DECOMP_SKIP=manager_sub_010C6238.
//
// Round P1 (p1chara, p1_chara.h): the keys from a host table per gaze motion. A key is a function of the motion's
// words alone (format byte, channel record, key words, the curve table), and the gaze motions are resources the game
// never writes after load, so each (motion, node, key) is evaluated once by the real curveEval and kept; the table is
// validated once a game frame per motion as C1 validates a motion (the format and channel words every frame, the
// channels' 0x30-byte records in turns, the curve table). The last key of a call is always the real call (the
// registers, fcr31 and VU0's vf1 it leaves are then the original's), and the last node's two other keys are stored where
// curveEval would have put them. SOTC_P1CHARA_CHECK=1 runs every call both ways from the same state and compares.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "decomp_curve.h"
#include "p1_chara.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace decomp_gaze
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kGaze = 0x010C6238u, kAtan2 = 0x001BE458u, kWrapper = 0x010B64C0u, kCurveEval = 0x010AC6C8u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
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
    };

    inline float fromBits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline uint32_t laneX(__m128 v) { return bitsOf(_mm_cvtss_f32(v)); }
    // A GPR's 128 bits with the low 64 replaced (qmtc2 of a register just
    // set by a 64-bit write: its upper half is what the register held).
    inline __m128 vecWithLow(R5900Context *ctx, int r, uint64_t low)
    {
        uint64_t w[2];
        const __m128i v = GPR_VEC(ctx, r);
        std::memcpy(w, &v, 16);
        w[0] = low;
        __m128 out;
        std::memcpy(&out, w, 16);
        return out;
    }

    const __m128 kAll = _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kW = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));

    struct Vu
    {
        __m128 v0, v5, v6, v7, v8, v9, v11, v12, v13, v14, v15, v16, acc;
        float q;
        uint16_t status;

        // v13 = v11 x v12 (the quaternion product: vmul, vaddy.x, vaddz.x,
        // vsubx.w, vmulw.xyz x2, vopmula / vopmsub, vadd.xyz x2).
        void qmul()
        {
            v13 = PS2_VBLEND(v13, PS2_VMUL(v11, v12), kAll);
            v13 = _mm_blendv_ps(v13, PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v13 = _mm_blendv_ps(v13, PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v13 = _mm_blendv_ps(v13, PS2_VSUB(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(0, 0, 0, 0))), kW);
            v14 = _mm_blendv_ps(v14, PS2_VMUL(v12, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 3, 3, 3))), kXyz);
            v15 = _mm_blendv_ps(v15, PS2_VMUL(v11, _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 3, 3, 3))), kXyz);
            {
                const __m128 fs = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 0, 2, 1)), ft = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 1, 0, 2));
                acc = _mm_blendv_ps(acc, PS2_VMUL(fs, ft), kXyz);
            }
            {
                const __m128 fs = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 0, 2, 1)), ft = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 1, 0, 2));
                v16 = _mm_blendv_ps(v16, PS2_VSUB(acc, PS2_VMUL(fs, ft)), kXyz);
            }
            v13 = PS2_VBLEND(v13, PS2_VADD(v14, v15), kXyz);
            v13 = PS2_VBLEND(v13, PS2_VADD(v13, v16), kXyz);
        }
        // v15.x = v14 . v15 (vmul.xyzw, vaddy.x, vaddz.x, vaddw.x).
        void dot()
        {
            v15 = PS2_VBLEND(v15, PS2_VMUL(v14, v15), kAll);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3, 3, 3, 3))), kX);
        }
        // vmulx.xyzw vf5, vf5, vf6x (vf6 the -1.0 of s5).
        void negate5() { v5 = _mm_blendv_ps(v5, PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0, 0, 0, 0))), kAll); }
        // vsubx.w vf8, vf0, vf7x; vmulax.xyzw ACC, vf5, vf7x; vmaddw.xyzw vf9, vf6, vf8w.
        void blend()
        {
            v8 = _mm_blendv_ps(v8, PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0, 0, 0, 0))), kW);
            acc = _mm_blendv_ps(acc, PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0, 0, 0, 0))), kAll);
            v9 = _mm_blendv_ps(v9, PS2_VADD(acc, PS2_VMUL(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3, 3, 3, 3)))), kAll);
        }
        // v15 = v14 normalised (vmul, the sums into x, vrsqrt Q, vf0w, vf15x, vmulq).
        void normalise()
        {
            v15 = PS2_VBLEND(v15, PS2_VMUL(v14, v14), kAll);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3, 3, 3, 3))), kX);
            const float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 0, 0, 3)));
            const float ft = _mm_cvtss_f32(_mm_shuffle_ps(v15, v15, _MM_SHUFFLE(0, 0, 0, 0)));
            q = ps2VuRsqrt(status, fs, ft);
            v15 = _mm_blendv_ps(v15, PS2_VMUL(v14, _mm_set1_ps(q)), kAll);
        }
    };

    inline void setCond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    inline bool inRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

    // ---- Round P1: the gaze motions' keys, kept ----
    namespace p1
    {
        constexpr uint32_t kKeys = 9u, kRecWords = 12u, kMaxWidth = 256u, kCurveTable = 0x01193230u;
        constexpr int kTables = 8;
        constexpr uint8_t kFxPacked = 1u, kFxC = 2u;
        // What a format's evaluator leaves besides the qword (decomp_curve.cpp's fast path): the channel's qword and the
        // no-data formats nothing; the packed formats vf1 (low 64 bits their 1 - |xyz|^2, high 64 a2's), Q, the status,
        // f0, a2 and fcr31's C (set or cleared). 0 none, 1 packed, 2 anything else: those keys are always the real call.
        inline int formatClass(uint32_t target)
        {
            switch (target)
            {
            case 0x010AD028u: // the channel's qword
            case 0x010AC700u: // none
                return 0;
            case 0x010AC978u:
            case 0x010ACAB0u:
            case 0x010ACBE8u:
            case 0x010ACD20u: // packed
                return 1;
            default:
                return 2;
            }
        }
        struct Table
        {
            uint32_t res = 0, formats = 0, chans = 0, width = 0, cap = 0;
            uint64_t checked = ~0ull, used = 0;
            uint8_t *fmt = nullptr;   // cap
            uint32_t *chan = nullptr; // cap
            uint32_t *rec = nullptr;  // cap x kRecWords
            uint16_t *have = nullptr; // cap: bit k = key k kept
            float *key = nullptr;     // cap x kKeys x 4 (16-byte aligned)
            uint64_t *vlo = nullptr;  // cap x kKeys: the low 64 bits of the vf1 the call leaves (the packed formats write it)
            uint8_t *vw = nullptr;    // cap x kKeys: the call's effects (kFxPacked: it wrote vf1 and fcr31's C; kFxC: C set)
            void *block = nullptr;
        };
        struct Cache
        {
            Table t[kTables];
            uint32_t curve[15] = {};
            uint64_t curveChecked = ~0ull, tick = 0;
        };
        Cache *g_c = nullptr;

        uint32_t rot()
        {
            static const int e = p1chara::envI("SOTC_P1CHARA_ROT", 8);
            static const uint32_t v = static_cast<uint32_t>(e < 1 ? 1 : (e > 64 ? 64 : e));
            return v;
        }
        inline const uint32_t *words(const uint8_t *ram, uint32_t a) { return reinterpret_cast<const uint32_t *>(ram + (a & PS2_RAM_MASK)); }
        inline uint32_t rd32(const uint8_t *ram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, ram + (a & PS2_RAM_MASK), 4);
            return v;
        }

        // Make t the table of res (width nodes): the words the keys depend on besides the key arrays; no keys yet.
        bool fill(Table &t, const uint8_t *ram, uint32_t res, uint32_t formats, uint32_t chans, uint32_t width)
        {
            if (!inRam(formats, width) || !inRam(chans, width * 4u))
                return false;
            for (uint32_t i = 0; i < width; ++i)
            {
                const uint32_t ch = rd32(ram, chans + i * 4u);
                if ((ch & 3u) != 0u || !inRam(ch & ~0xFu, kRecWords * 4u))
                    return false;
            }
            if (t.cap < width)
            {
                std::free(t.block);
                const uint64_t used = t.used;
                t = Table{};
                t.used = used;
                const uint32_t cap = width < 32u ? 32u : width;
                const size_t bytes = static_cast<size_t>(cap) * (kKeys * 16u + kKeys * 8u + kKeys + kRecWords * 4u + 4u + 2u + 1u) + 64u;
                t.block = std::malloc(bytes);
                if (!t.block)
                    return false;
                uint8_t *b = reinterpret_cast<uint8_t *>((reinterpret_cast<uintptr_t>(t.block) + 15u) & ~static_cast<uintptr_t>(15u));
                t.key = reinterpret_cast<float *>(b);
                b += cap * kKeys * 16u;
                t.vlo = reinterpret_cast<uint64_t *>(b);
                b += cap * kKeys * 8u;
                t.vw = b;
                b += cap * kKeys;
                t.rec = reinterpret_cast<uint32_t *>(b);
                b += cap * kRecWords * 4u;
                t.chan = reinterpret_cast<uint32_t *>(b);
                b += cap * 4u;
                t.have = reinterpret_cast<uint16_t *>(b);
                b += cap * 2u;
                t.fmt = b;
                t.cap = cap;
            }
            std::memcpy(t.fmt, ram + (formats & PS2_RAM_MASK), width);
            std::memcpy(t.chan, ram + (chans & PS2_RAM_MASK), width * 4u);
            for (uint32_t i = 0; i < width; ++i)
                std::memcpy(t.rec + i * kRecWords, words(ram, t.chan[i] & ~0xFu), kRecWords * 4u);
            std::memset(t.have, 0, width * 2u);
            t.res = res;
            t.formats = formats;
            t.chans = chans;
            t.width = width;
            t.checked = sotc_decomp::g_gameFrame;
            ++p1chara::g_st.tableMade;
            p1chara::g_st.gazeNewLd += static_cast<uint64_t>(width) * (1u + 4u + kRecWords * 4u);
            return true;
        }

        // The guest's words still the table's (once a frame per motion): the format and channel words every frame, the
        // records in turns (the first, the last and every rot()-th from the frame's phase).
        bool same(const Table &t, const uint8_t *ram)
        {
            if (std::memcmp(t.fmt, ram + (t.formats & PS2_RAM_MASK), t.width) != 0 || std::memcmp(t.chan, ram + (t.chans & PS2_RAM_MASK), t.width * 4u) != 0)
                return false;
            const uint64_t fr = sotc_decomp::g_gameFrame;
            const uint32_t rt = rot();
            uint32_t diff = 0, n = 0;
            for (uint32_t i = 0; i < t.width; ++i)
            {
                if (!(i == 0u || i + 1u == t.width || (i % rt) == static_cast<uint32_t>(fr % rt)))
                    continue;
                const uint32_t *g = words(ram, t.chan[i] & ~0xFu), *h = t.rec + i * kRecWords;
                for (uint32_t w = 0; w < kRecWords; ++w)
                    diff |= g[w] ^ h[w];
                ++n;
            }
            p1chara::g_st.gazeNewLd += static_cast<uint64_t>(t.width) * 5u + n * kRecWords * 4u;
            return diff == 0u;
        }

        // The table of res for nodes [0, width), validated for this frame (nullptr: the real calls).
        Table *tableFor(const uint8_t *ram, uint32_t res, uint32_t width)
        {
            if (width == 0u || width > kMaxWidth || !inRam(res, 0x14u))
                return nullptr;
            Cache *c = g_c;
            if (!c)
            {
                c = g_c = new (std::nothrow) Cache();
                if (!c)
                    return nullptr;
            }
            const uint64_t fr = sotc_decomp::g_gameFrame;
            if (c->curveChecked != fr)
            {
                // the curve table (the module's data): a change voids every key
                if (std::memcmp(c->curve, words(ram, kCurveTable), sizeof(c->curve)) != 0)
                {
                    std::memcpy(c->curve, words(ram, kCurveTable), sizeof(c->curve));
                    for (Table &t : c->t)
                        t.res = 0, t.checked = ~0ull;
                }
                c->curveChecked = fr;
                p1chara::g_st.gazeNewLd += sizeof(c->curve);
            }
            const uint32_t formats = rd32(ram, res + 8u), chans = rd32(ram, res + 0x10u);
            p1chara::g_st.gazeNewLd += 8u;
            ++c->tick;
            Table *lru = &c->t[0];
            for (Table &t : c->t)
            {
                if (t.res == res && t.block)
                {
                    t.used = c->tick;
                    if (t.formats != formats || t.chans != chans || t.width < width)
                        return fill(t, ram, res, formats, chans, width > t.width ? width : t.width) ? &t : (t.res = 0, nullptr);
                    if (t.checked != fr)
                    {
                        t.checked = fr;
                        ++p1chara::g_st.tableValid;
                        if (!same(t, ram) && !fill(t, ram, res, formats, chans, t.width))
                            return t.res = 0, nullptr;
                    }
                    return &t;
                }
                if (t.used < lru->used)
                    lru = &t;
            }
            lru->used = c->tick;
            lru->res = 0;
            return fill(*lru, ram, res, formats, chans, width) ? lru : nullptr;
        }

        // ---- the check: both ways from the same state ----
        struct alignas(16) Snap
        {
            uint8_t ctx[sizeof(R5900Context)];
            uint8_t quats[512 * 16];
            uint8_t stack[0x400];
        };
        Snap *g_snap[3] = {nullptr, nullptr, nullptr}; // 0: before, 1: P1's results, 2: the original's
        alignas(16) uint8_t g_ta[sizeof(R5900Context)], g_tb[sizeof(R5900Context)];
    }

    void gazeImpl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool useTable);

    void gaze(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || jalTarget(rdram, 0x010C62F0u) != kAtan2 || jalTarget(rdram, 0x010C63C4u) != kWrapper ||
            jalTarget(rdram, 0x010C63D8u) != kWrapper || jalTarget(rdram, 0x010C63ECu) != kWrapper || jalTarget(rdram, 0x010B64E8u) != kCurveEval)
            return sotc_decomp::recompiled(kGaze)(rdram, ctx, runtime);
        const bool p1 = sotc_decomp::p1charaOn() && sotc_decomp::lane(runtime) == 0;
        if (p1)
            p1chara::logMaybe();
        if (!p1 || !p1chara::checkOn())
            return gazeImpl(rdram, ctx, runtime, p1);
        // SOTC_P1CHARA_CHECK: P1's way first, its results kept, the state put back, then the original's way (whose results
        // the game goes on with), compared.
        const uint32_t quats = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), res = Ram{rdram}.u32(GPR_U32(ctx, 6) + 8u);
        const int32_t n = static_cast<int32_t>(Ram{rdram}.u32(GPR_U32(ctx, 5) + 4u));
        const uint32_t qBytes = n > 0 ? static_cast<uint32_t>(n) * 16u : 0u;
        if (n < 0 || n > 512 || !inRam(quats, qBytes ? qBytes : 16u) || !inRam(sp0 - 0x400u, 0x400u))
            return gazeImpl(rdram, ctx, runtime, true);
        for (auto *&sn : p1::g_snap)
            if (!sn && !(sn = new (std::nothrow) p1::Snap()))
                return gazeImpl(rdram, ctx, runtime, true);
        uint8_t *const qp = rdram + (quats & PS2_RAM_MASK), *const stp = rdram + ((sp0 - 0x400u) & PS2_RAM_MASK);
        auto take = [&](p1::Snap *sn)
        {
            std::memcpy(sn->ctx, ctx, sizeof(R5900Context));
            std::memcpy(sn->quats, qp, qBytes);
            std::memcpy(sn->stack, stp, 0x400u);
        };
        take(p1::g_snap[0]);
        gazeImpl(rdram, ctx, runtime, true);
        take(p1::g_snap[1]);
        std::memcpy(ctx, p1::g_snap[0]->ctx, sizeof(R5900Context));
        std::memcpy(qp, p1::g_snap[0]->quats, qBytes);
        std::memcpy(stp, p1::g_snap[0]->stack, 0x400u);
        gazeImpl(rdram, ctx, runtime, false);
        take(p1::g_snap[2]);
        p1chara::Stats &st = p1chara::g_st;
        ++st.gazeChecks;
        const bool outDiff = std::memcmp(p1::g_snap[1]->quats, p1::g_snap[2]->quats, qBytes) != 0;
        // The registers the gaze itself leaves (v0, v1, f0, VU0 vf5-vf16, ACC, Q, status, fcr31, sp, ra, pc) and the rest.
        const R5900Context *a = reinterpret_cast<const R5900Context *>(p1::g_snap[1]->ctx), *b = reinterpret_cast<const R5900Context *>(p1::g_snap[2]->ctx);
        const bool regDiff = std::memcmp(&a->r[2], &b->r[2], 32) != 0 || std::memcmp(&a->r[29], &b->r[29], 16) != 0 || std::memcmp(&a->r[31], &b->r[31], 16) != 0 ||
                             a->pc != b->pc || std::memcmp(&a->f[0], &b->f[0], 4) != 0 || std::memcmp(&a->vu0_vf[5], &b->vu0_vf[5], 12 * 16) != 0 ||
                             std::memcmp(&a->vu0_acc, &b->vu0_acc, 16) != 0 || std::memcmp(&a->vu0_q, &b->vu0_q, 4) != 0 || a->vu0_status != b->vu0_status ||
                             a->fcr31 != b->fcr31;
        // Everything else in the context (the scratch GPRs, the other FPU / VU0 registers, ...), compared as bytes with the
        // fields above made equal.
        std::memcpy(p1::g_ta, a, sizeof(R5900Context));
        std::memcpy(p1::g_tb, b, sizeof(R5900Context));
        {
            R5900Context *x = reinterpret_cast<R5900Context *>(p1::g_ta);
            const R5900Context *y = reinterpret_cast<const R5900Context *>(p1::g_tb);
            std::memcpy(&x->r[2], &y->r[2], 32);
            std::memcpy(&x->r[29], &y->r[29], 16);
            std::memcpy(&x->r[31], &y->r[31], 16);
            x->pc = y->pc;
            std::memcpy(&x->f[0], &y->f[0], 4);
            std::memcpy(&x->vu0_vf[5], &y->vu0_vf[5], 12 * 16);
            std::memcpy(&x->vu0_acc, &y->vu0_acc, 16);
            std::memcpy(&x->vu0_q, &y->vu0_q, 4);
            x->vu0_status = y->vu0_status;
            x->fcr31 = y->fcr31;
        }
        const bool scratchDiff = std::memcmp(p1::g_ta, p1::g_tb, sizeof(R5900Context)) != 0;
        const bool stackDiff = std::memcmp(p1::g_snap[1]->stack, p1::g_snap[2]->stack, 0x400u) != 0;
        st.gazeDiffOut += outDiff;
        st.gazeDiffRegs += regDiff;
        st.gazeDiffScratch += scratchDiff;
        st.gazeDiffStack += stackDiff;
        static int shownRegs = 0;
        if (regDiff && shownRegs < 30)
        {
            ++shownRegs;
            std::fprintf(stderr, "[p1chk] frame %llu: gaze regs differ: v0 %d v1 %d sp %d ra %d pc %d f0 %d vf5-16 %d acc %d q %d status %04x/%04x fcr31 %08x/%08x\n",
                         (unsigned long long)sotc_decomp::g_gameFrame, std::memcmp(&a->r[2], &b->r[2], 16) != 0, std::memcmp(&a->r[3], &b->r[3], 16) != 0,
                         std::memcmp(&a->r[29], &b->r[29], 16) != 0, std::memcmp(&a->r[31], &b->r[31], 16) != 0, a->pc != b->pc,
                         std::memcmp(&a->f[0], &b->f[0], 4) != 0, std::memcmp(&a->vu0_vf[5], &b->vu0_vf[5], 12 * 16) != 0,
                         std::memcmp(&a->vu0_acc, &b->vu0_acc, 16) != 0, std::memcmp(&a->vu0_q, &b->vu0_q, 4) != 0, a->vu0_status, b->vu0_status, a->fcr31,
                         b->fcr31);
        }
        static int shown = 0;
        if ((outDiff || regDiff || scratchDiff) && shown < 30)
        {
            ++shown;
            int firstR = -1, firstF = -1, firstV = -1;
            for (int i = 0; i < 32 && firstR < 0; ++i)
                if (std::memcmp(&a->r[i], &b->r[i], 16) != 0)
                    firstR = i;
            for (int i = 0; i < 32 && firstF < 0; ++i)
                if (std::memcmp(&a->f[i], &b->f[i], 4) != 0)
                    firstF = i;
            for (int i = 0; i < 32 && firstV < 0; ++i)
                if (std::memcmp(&a->vu0_vf[i], &b->vu0_vf[i], 16) != 0)
                    firstV = i;
            std::fprintf(stderr, "[p1chk] frame %llu: gaze res %08x nodes %d differ: out %d regs %d scratch %d (first GPR %d FPR %d VF %d fcr31 %08x/%08x)\n",
                         (unsigned long long)sotc_decomp::g_gameFrame, res, n, outDiff ? 1 : 0, regDiff ? 1 : 0, scratchDiff ? 1 : 0, firstR, firstF, firstV,
                         a->fcr31, b->fcr31);
        }
    }

    void gazeImpl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool useTable)
    {
        const Ram r{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x110u;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);

        // The direction, each component clamped to [-1, 1] (c.lt.s as the original).
        const float minus1 = fromBits(0xBF800000u), one = fromBits(0x3F800000u);
        const float f1 = r.f32(a2 + 4u), f2 = r.f32(a2 + 0u);
        int32_t s1 = static_cast<int32_t>(r.u32(a1 + 4u));
        const uint32_t res = r.u32(a2 + 8u);
        uint32_t s4 = a0 + (static_cast<uint32_t>(s1) << 4) - 0x10u;
        float f21, f20;
        bool c = FPU_C_OLT_S(f1, minus1);
        setCond(ctx, c);
        if (c)
            f21 = minus1;
        else
        {
            f21 = one;
            c = FPU_C_OLT_S(f21, f1);
            setCond(ctx, c);
            if (!c)
                f21 = f1;
        }
        c = FPU_C_OLT_S(f2, minus1);
        setCond(ctx, c);
        if (c)
            f20 = minus1;
        else
        {
            f20 = one;
            c = FPU_C_OLT_S(f20, f2);
            setCond(ctx, c);
            if (!c)
                f20 = f2;
        }
        ctx->f[12] = f20;
        ctx->f[13] = f21;
        sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x010C62F0u), 0x010C62F8u, sp);

        float f3;
        {
            const float a = ctx->f[0];
            c = FPU_C_OLT_S(a, 0.0f);
            setCond(ctx, c);
            f3 = c ? FPU_ADD_S(a, fromBits(0x40C90FDAu)) : FPU_ADD_S(a, fromBits(0u));
        }
        float f0 = FPU_ADD_S(FPU_MUL_S(f21, f21), FPU_MUL_S(f20, f20));
        f0 = ps2FpuSqrt(ctx->fcr31, f0);
        const float len = f0; // f20 = f22
        float fk = FPU_MUL_S(f3, fromBits(0x41000000u));
        fk = ps2FpuDivide(ctx->fcr31, fk, fromBits(0x40C90FDAu));
        const int32_t k = FPU_CVT_W_S(fk);
        f0 = FPU_CVT_S_W(k);
        const float frac = FPU_SUB_S(fk, f0); // f21
        s1 -= 1;
        if (s1 == -1)
        {
            SET_GPR_S32(ctx, 2, k);
            ctx->f[0] = f0;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
            return;
        }
        const uint32_t keyA = (static_cast<uint32_t>(k) & 7u) + 1u, keyB = ((static_cast<uint32_t>(k) + 1u) & 7u) + 1u;
        const uint32_t identity = sotc_decomp::relocated(rdram, 0x010C646Cu, 0x010C6470u);
        const uint32_t identity2 = sotc_decomp::relocated(rdram, 0x010C6508u, 0x010C650Cu);

        Vu u;
        u.v0 = ctx->vu0_vf[0];
        u.v5 = ctx->vu0_vf[5];
        u.v6 = ctx->vu0_vf[6];
        u.v7 = ctx->vu0_vf[7];
        u.v8 = ctx->vu0_vf[8];
        u.v9 = ctx->vu0_vf[9];
        u.v11 = ctx->vu0_vf[11];
        u.v12 = ctx->vu0_vf[12];
        u.v13 = ctx->vu0_vf[13];
        u.v14 = ctx->vu0_vf[14];
        u.v15 = ctx->vu0_vf[15];
        u.v16 = ctx->vu0_vf[16];
        u.acc = ctx->vu0_acc;
        u.q = ctx->vu0_q;
        u.status = ctx->vu0_status;
        // s5 = 0xbf800000 (lui): vf6 for the negations is its 128 bits.
        const uint64_t s5low = 0xFFFFFFFFBF800000ull;
        uint32_t lastDot = 0u;
        __m128 lastDotVec = _mm_setzero_ps();

        // Round P1: the motion's key table (nullptr: every key the real call).
        p1::Table *const tb = useTable ? p1::tableFor(rdram, res, static_cast<uint32_t>(s1 + 1)) : nullptr;
        p1chara::Stats &pst = p1chara::g_st;
        const bool count = useTable || (sotc_decomp::lane(runtime) == 0 && p1chara::logEvery() > 0);
        if (count)
        {
            ++pst.gazeCalls;
            pst.gazeNodes += static_cast<uint64_t>(s1 + 1);
            if (useTable && !tb)
                ++pst.gazeFallback;
        }
        // The proxy, per key by the real call (curveEval's reads for the 32-bit key formats: the format byte, the channel
        // word, the motion's two table words, the 0x24-byte record, the key word, the curve table's word, then the gaze's
        // read back of the qword; its write of the qword).
        constexpr uint32_t kKeyLd = 1u + 4u + 8u + 0x24u + 4u + 4u + 16u, kKeySt = 16u;

        // GetMotionNodeIntFrameDirectResource(out, res, node, key): curveEval(out,
        // byte res[+8][node], word res[+0x10][node], key), sp this frame's.
        // Round P1: with the table, a key kept is a host load (the call's last key is always the real call, and the last
        // node's keys are stored where the call puts them); a key not kept yet is the real call, then kept.
        // What a kept key's call would have left that the gaze does not overwrite later (formatClass): a packed key's vf1
        // (its low 64 bits kept, the high 64 a2's as they are) and fcr31's C are put in the context when the key is
        // served, as the call would; Q, the status, f0 and v0 the gaze overwrites, a2 the next real call's setup does.
        // Keys of other formats are not kept.
        auto key = [&](uint32_t out, uint32_t site, uint32_t keyIndex) -> __m128
        {
            const uint32_t node = static_cast<uint32_t>(s1);
            const bool last = node == 0u && site == 0x010C63ECu;
            if (count)
                pst.gazeOldLd += kKeyLd, pst.gazeOldSt += kKeySt, ++pst.gazeOldCalls;
            if (tb && !last && keyIndex < p1::kKeys && (tb->have[node] & (1u << keyIndex)) != 0u)
            {
                const __m128 v = _mm_load_ps(tb->key + (node * p1::kKeys + keyIndex) * 4u);
                const uint8_t fx = tb->vw[node * p1::kKeys + keyIndex];
                if (fx & p1::kFxPacked)
                {
                    uint64_t w[2];
                    const __m128i a2 = GPR_VEC(ctx, 6);
                    std::memcpy(w, &a2, 16);
                    w[0] = tb->vlo[node * p1::kKeys + keyIndex];
                    std::memcpy(&ctx->vu0_vf[1], w, 16);
                    ctx->fcr31 = (fx & p1::kFxC) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                }
                ++pst.keysServed;
                if (node == 0u)
                {
                    r.putq(out, v);
                    pst.gazeNewSt += 16u;
                }
                return v;
            }
            if (count)
            {
                pst.gazeNewLd += kKeyLd, pst.gazeNewSt += kKeySt, ++pst.gazeNewCalls;
                if (last)
                    ++pst.keysReal;
            }
            const uint32_t t0 = r.u32(res + 0x10u), v1 = r.u32(res + 8u) + node, v0 = (node << 2) + t0;
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(t0));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(out));
            SET_GPR_U64(ctx, 5, r.u8(v1));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(r.u32(v0)));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(keyIndex));
            sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, site + 8u, sp);
            const __m128 v = r.q(out);
            const int cls = tb ? p1::formatClass(p1::g_c->curve[tb->fmt[node] < 15u ? tb->fmt[node] : 0u]) : 2;
            if (tb && keyIndex < p1::kKeys && tb->fmt[node] < 15u && cls != 2)
            {
                uint64_t lo[2];
                std::memcpy(lo, &ctx->vu0_vf[1], 16);
                tb->vw[node * p1::kKeys + keyIndex] =
                    cls == 1 ? static_cast<uint8_t>(p1::kFxPacked | ((ctx->fcr31 & 0x800000u) ? p1::kFxC : 0u)) : static_cast<uint8_t>(0u);
                tb->vlo[node * p1::kKeys + keyIndex] = lo[0];
                _mm_store_ps(tb->key + (node * p1::kKeys + keyIndex) * 4u, v);
                tb->have[node] = static_cast<uint16_t>(tb->have[node] | (1u << keyIndex));
                ++pst.keysFilled;
            }
            return v;
        };
        // The dot's sign through mfc1 / swc1 / lw / bgez (its x lane's bits).
        auto dotSign = [&]() -> bool
        {
            lastDotVec = u.v15;
            lastDot = laneX(u.v15);
            return static_cast<int32_t>(lastDot) < 0;
        };
        auto t0Vec = [&](float f)
        {
            SET_GPR_U32(ctx, 8, bitsOf(f));
            return _mm_castsi128_ps(GPR_VEC(ctx, 8));
        };

        do
        {
            const __m128 k0 = key(sp, 0x010C63C4u, 0u);
            const __m128 kA = key(sp + 0x10u, 0x010C63D8u, keyA);
            const __m128 kB = key(sp + 0x20u, 0x010C63ECu, keyB);
            // the base's conjugate (x, y, z negated by vf0x - vf0w)
            u.v11 = k0;
            u.v5 = _mm_blendv_ps(u.v5, PS2_VSUB(u.v0, _mm_shuffle_ps(u.v0, u.v0, _MM_SHUFFLE(3, 3, 3, 3))), kX);
            u.v11 = _mm_blendv_ps(u.v11, PS2_VMUL(u.v11, _mm_shuffle_ps(u.v5, u.v5, _MM_SHUFFLE(0, 0, 0, 0))), kXyz);
            const __m128 conj = u.v11;
            // A and B relative to the base
            u.v12 = kA;
            u.qmul();
            const __m128 relA = u.v13;
            u.v11 = conj;
            u.v12 = kB;
            u.qmul();
            const __m128 relB = u.v13;

            // A from the identity by the length
            __m128 id = r.q(identity);
            u.v14 = relA;
            u.v15 = id;
            u.dot();
            if (dotSign())
            {
                u.v5 = id;
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                id = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relA;
            u.v6 = id;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            const __m128 nA = u.v15;

            // B from the identity by the length
            __m128 id2 = r.q(identity2);
            u.v14 = relB;
            u.v15 = id2;
            u.dot();
            u.v5 = id2; // the branch's delay slot
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                id2 = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relB;
            u.v6 = id2;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            const __m128 nB = u.v15;

            // B towards A by the fraction, normalised twice
            __m128 a = nA;
            u.v14 = nB;
            u.v15 = a;
            u.dot();
            u.v5 = a; // the branch's delay slot
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                a = u.v5;
            }
            u.v7 = t0Vec(frac);
            u.v5 = nB;
            u.v6 = a;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            u.v14 = u.v15;
            u.normalise();

            // the node's quaternion x the gaze
            u.v11 = r.q(s4);
            u.v12 = u.v15;
            u.qmul();
            r.putq(s4, u.v13);
            s1 -= 1;
            s4 -= 0x10u;
        } while (s1 != -1);

        // v0: the last dot's qword (qmfc2), its low word reloaded (lw); f0 its bits.
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(lastDotVec));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(lastDot));
        SET_GPR_S32(ctx, 3, -1);
        ctx->f[0] = fromBits(lastDot);
        ctx->vu0_vf[5] = u.v5;
        ctx->vu0_vf[6] = u.v6;
        ctx->vu0_vf[7] = u.v7;
        ctx->vu0_vf[8] = u.v8;
        ctx->vu0_vf[9] = u.v9;
        ctx->vu0_vf[11] = u.v11;
        ctx->vu0_vf[12] = u.v12;
        ctx->vu0_vf[13] = u.v13;
        ctx->vu0_vf[14] = u.v14;
        ctx->vu0_vf[15] = u.v15;
        ctx->vu0_vf[16] = u.v16;
        ctx->vu0_acc = u.acc;
        ctx->vu0_q = u.q;
        ctx->vu0_status = u.status;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

namespace p1chara
{
    // The [p1chara] line every SOTC_P1CHARA_LOG game frames (called from the gaze and the pose on the game thread).
    void logMaybe()
    {
        const int every = logEvery();
        Stats &s = g_st;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (s.lastFrame != fr)
        {
            s.lastFrame = fr;
            ++s.frames;
            if (sotc_decomp::p1charaOn())
                ++s.onFrames;
        }
        if (every <= 0)
            return;
        if (g_logStart == 0)
            g_logStart = fr;
        if (fr - g_logStart < static_cast<uint64_t>(every))
            return;
        const double f = static_cast<double>(s.frames ? s.frames : 1);
        std::fprintf(stderr,
                     "[p1chara] frame %llu: %llu frames (%llu on); a frame: gaze %.2f calls %.1f nodes, keys served %.1f filled %.2f real %.2f, fallback %.3f, "
                     "tables valid %.2f made %llu; proxy gaze old ld %.0f B st %.0f B (%.1f calls) new ld %.0f B st %.0f B (%.1f calls); "
                     "pose %.2f calls %.1f bones (host offsets %.2f) ld old %.0f B new %.0f B; palettes %.2f (%.1f bones); "
                     "checks gaze %llu differ out %llu regs %llu scratch %llu (dead stack %llu)\n",
                     (unsigned long long)fr, (unsigned long long)s.frames, (unsigned long long)s.onFrames, s.gazeCalls / f, s.gazeNodes / f, s.keysServed / f,
                     s.keysFilled / f, s.keysReal / f, s.gazeFallback / f, s.tableValid / f, (unsigned long long)s.tableMade, s.gazeOldLd / f, s.gazeOldSt / f,
                     s.gazeOldCalls / f, s.gazeNewLd / f, s.gazeNewSt / f, s.gazeNewCalls / f, s.poses / f, s.poseBones / f, s.poseHostOff / f, s.poseOldLd / f,
                     s.poseNewLd / f, s.palettes / f, s.paletteBones / f, (unsigned long long)s.gazeChecks, (unsigned long long)s.gazeDiffOut,
                     (unsigned long long)s.gazeDiffRegs, (unsigned long long)s.gazeDiffScratch, (unsigned long long)s.gazeDiffStack);
        const Stats keep = s;
        s = Stats{};
        s.lastFrame = fr;
        s.gazeChecks = keep.gazeChecks, s.gazeDiffOut = keep.gazeDiffOut, s.gazeDiffRegs = keep.gazeDiffRegs, s.gazeDiffScratch = keep.gazeDiffScratch;
        s.gazeDiffStack = keep.gazeDiffStack, s.tableMade = keep.tableMade;
        g_logStart = fr;
    }
}

namespace decomp_gaze
{
    void gazeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D1 (below)
}

SOTC_DECOMP_FUNCTION(0x010C6238u, manager_sub_010C6238, decomp_gaze::gazeSw);
D1_ALIAS(0x010C6238u, manager_sub_010C6238, decomp_gaze::gazeSw, decomp_gaze::gaze);

// ---- Round D1 (d1native, d1_native.h): manager_sub_010C6238 (the gaze) with native math ------------------------------
// Partial: gazeImpl's default path (P1 off: every key the real curveEval call) with its FPU statements as d1:: scalar
// ops (the direction's clamps, the angle, the length's ps2FpuSqrt and the key's ps2FpuDivide kept as the old body
// calls them: they round to nearest), the VU0 blocks (quaternion product, dot, negation, blend, normalisation) as d1
// vec4 ops on a local register set, Q by the old body's ps2VuRsqrt with its status; guest memory through d1::Mem. The
// guest calls (iosGetATan2fExact, curveEval per key) and the registers they read stay as the old body makes them; the
// VU0 registers, Q, the status, v0 / v1 / f0 written at the exit as the old body does. P1 on: the old body.
namespace decomp_gaze
{
    D1_COUNTER(c_d1Gaze, "manager_sub_010C6238");

    template <int A, int B, int C, int D>
    PS2_FORCE_INLINE __m128 d1perm(__m128 v)
    {
#if defined(__clang__)
        return __builtin_shufflevector(v, v, A, B, C, D);
#else
        typedef int d1v4i __attribute__((vector_size(16)));
        return __builtin_shuffle(v, d1v4i{A, B, C, D});
#endif
    }

    struct D1Vu
    {
        __m128 v0, v5, v6, v7, v8, v9, v11, v12, v13, v14, v15, v16, acc;
        float q;
        uint16_t status;
    };
    // v13 = v11 x v12 (Vu::qmul's ops in its order).
    PS2_FORCE_INLINE void d1Qmul(D1Vu &u)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0), mW = d1::mask(0, 0, 0, 1), mXyz = d1::mask(1, 1, 1, 0);
        u.v13 = d1::vmul(u.v11, u.v12);
        u.v13 = d1::vsel(u.v13, d1::vadd(u.v13, d1::bc<1>(u.v13)), mX);
        u.v13 = d1::vsel(u.v13, d1::vadd(u.v13, d1::bc<2>(u.v13)), mX);
        u.v13 = d1::vsel(u.v13, d1::vsub(u.v13, d1::bc<0>(u.v13)), mW);
        u.v14 = d1::vsel(u.v14, d1::vmul(u.v12, d1::bc<3>(u.v11)), mXyz);
        u.v15 = d1::vsel(u.v15, d1::vmul(u.v11, d1::bc<3>(u.v12)), mXyz);
        u.acc = d1::vsel(u.acc, d1::vmul(d1perm<1, 2, 0, 3>(u.v12), d1perm<2, 0, 1, 3>(u.v11)), mXyz);
        u.v16 = d1::vsel(u.v16, d1::vsub(u.acc, d1::vmul(d1perm<1, 2, 0, 3>(u.v11), d1perm<2, 0, 1, 3>(u.v12))), mXyz);
        u.v13 = d1::vsel(u.v13, d1::vadd(u.v14, u.v15), mXyz);
        u.v13 = d1::vsel(u.v13, d1::vadd(u.v13, u.v16), mXyz);
    }
    // v15.x = v14 . v15.
    PS2_FORCE_INLINE void d1Dot(D1Vu &u)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0);
        u.v15 = d1::vmul(u.v14, u.v15);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<1>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<2>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<3>(u.v15)), mX);
    }
    PS2_FORCE_INLINE void d1Negate5(D1Vu &u) { u.v5 = d1::vmul(u.v5, d1::bc<0>(u.v6)); }
    PS2_FORCE_INLINE void d1Blend(D1Vu &u)
    {
        u.v8 = d1::vsel(u.v8, d1::vsub(u.v0, d1::bc<0>(u.v7)), d1::mask(0, 0, 0, 1));
        u.acc = d1::vmul(u.v5, d1::bc<0>(u.v7));
        u.v9 = d1::vmadd(u.acc, u.v6, d1::bc<3>(u.v8));
    }
    PS2_FORCE_INLINE void d1Normalise(D1Vu &u)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0);
        u.v15 = d1::vmul(u.v14, u.v14);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<1>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<2>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<3>(u.v15)), mX);
        u.q = ps2VuRsqrt(u.status, d1::lane<3>(u.v0), d1::lane<0>(u.v15));
        u.v15 = d1::vmul(u.v14, d1::splat(u.q));
    }
    // A component clamped to [-1, 1] (c.lt.s twice as the original; c: the last compare's condition).
    PS2_FORCE_INLINE float d1Clamp1(float v, bool &c)
    {
        const float minus1 = fromBits(0xBF800000u), one = fromBits(0x3F800000u);
        c = d1::lt(v, minus1);
        if (c)
            return minus1;
        c = d1::lt(one, v);
        return c ? one : v;
    }

    void gazeD1Body(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x110u;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);

        const float f1 = m.f32(a2 + 4u), f2 = m.f32(a2 + 0u);
        int32_t s1 = m.s32(a1 + 4u);
        const uint32_t res = m.u32(a2 + 8u);
        uint32_t s4 = a0 + (static_cast<uint32_t>(s1) << 4) - 0x10u;
        bool c;
        const float f21 = d1Clamp1(f1, c);
        const float f20 = d1Clamp1(f2, c);
        setCond(ctx, c);
        ctx->f[12] = f20;
        ctx->f[13] = f21;
        sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x010C62F0u), 0x010C62F8u, sp);

        const float ang = ctx->f[0];
        c = d1::lt(ang, 0.0f);
        setCond(ctx, c);
        const float f3 = c ? d1::add(ang, fromBits(0x40C90FDAu)) : d1::add(ang, fromBits(0u));
        uint32_t fcr = 0u; // (the flags these two can raise need a negative root / a zero divisor: neither happens)
        const float len = ps2FpuSqrt(fcr, d1::add(d1::mul(f21, f21), d1::mul(f20, f20)));
        const float fk = ps2FpuDivide(fcr, d1::mul(f3, fromBits(0x41000000u)), fromBits(0x40C90FDAu));
        const int32_t k = ps2FpuConvertToWord(fk);
        const float fkw = static_cast<float>(k);
        const float frac = d1::sub(fk, fkw);
        s1 -= 1;
        if (s1 == -1)
        {
            SET_GPR_S32(ctx, 2, k);
            ctx->f[0] = fkw;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
            return;
        }
        const uint32_t keyA = (static_cast<uint32_t>(k) & 7u) + 1u, keyB = ((static_cast<uint32_t>(k) + 1u) & 7u) + 1u;
        const uint32_t identity = sotc_decomp::relocated(rdram, 0x010C646Cu, 0x010C6470u);
        const uint32_t identity2 = sotc_decomp::relocated(rdram, 0x010C6508u, 0x010C650Cu);

        D1Vu u;
        u.v0 = ctx->vu0_vf[0];
        u.v5 = ctx->vu0_vf[5];
        u.v6 = ctx->vu0_vf[6];
        u.v7 = ctx->vu0_vf[7];
        u.v8 = ctx->vu0_vf[8];
        u.v9 = ctx->vu0_vf[9];
        u.v11 = ctx->vu0_vf[11];
        u.v12 = ctx->vu0_vf[12];
        u.v13 = ctx->vu0_vf[13];
        u.v14 = ctx->vu0_vf[14];
        u.v15 = ctx->vu0_vf[15];
        u.v16 = ctx->vu0_vf[16];
        u.acc = ctx->vu0_acc;
        u.q = ctx->vu0_q;
        u.status = ctx->vu0_status;
        const uint64_t s5low = 0xFFFFFFFFBF800000ull;
        uint32_t lastDot = 0u;
        __m128 lastDotVec = d1::zero4();

        // GetMotionNodeIntFrameDirectResource's loads, then curveEval as its tail jump (the guest call).
        auto key = [&](uint32_t out, uint32_t site, uint32_t keyIndex) -> __m128
        {
            const uint32_t node = static_cast<uint32_t>(s1);
            const uint32_t t0 = m.u32(res + 0x10u), v1 = m.u32(res + 8u) + node, v0 = (node << 2) + t0;
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(t0));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(out));
            SET_GPR_U64(ctx, 5, *m.at<uint8_t>(v1));
            SET_GPR_S32(ctx, 6, m.s32(v0));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(keyIndex));
            sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, site + 8u, sp);
            return m.q(out & ~0xFu);
        };
        auto dotSign = [&]() -> bool
        {
            lastDotVec = u.v15;
            lastDot = bitsOf(d1::lane<0>(u.v15));
            return static_cast<int32_t>(lastDot) < 0;
        };
        auto t0Vec = [&](float f)
        {
            SET_GPR_U32(ctx, 8, bitsOf(f));
            return _mm_castsi128_ps(GPR_VEC(ctx, 8));
        };

        do
        {
            const __m128 k0 = key(sp, 0x010C63C4u, 0u);
            const __m128 kA = key(sp + 0x10u, 0x010C63D8u, keyA);
            const __m128 kB = key(sp + 0x20u, 0x010C63ECu, keyB);
            u.v11 = k0;
            u.v5 = d1::vsel(u.v5, d1::vsub(u.v0, d1::bc<3>(u.v0)), d1::mask(1, 0, 0, 0));
            u.v11 = d1::vsel(u.v11, d1::vmul(u.v11, d1::bc<0>(u.v5)), d1::mask(1, 1, 1, 0));
            const __m128 conj = u.v11;
            u.v12 = kA;
            d1Qmul(u);
            const __m128 relA = u.v13;
            u.v11 = conj;
            u.v12 = kB;
            d1Qmul(u);
            const __m128 relB = u.v13;

            __m128 id = m.q(identity & ~0xFu);
            u.v14 = relA;
            u.v15 = id;
            d1Dot(u);
            if (dotSign())
            {
                u.v5 = id;
                u.v6 = vecWithLow(ctx, 21, s5low);
                d1Negate5(u);
                id = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relA;
            u.v6 = id;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            const __m128 nA = u.v15;

            __m128 id2 = m.q(identity2 & ~0xFu);
            u.v14 = relB;
            u.v15 = id2;
            d1Dot(u);
            u.v5 = id2;
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                d1Negate5(u);
                id2 = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relB;
            u.v6 = id2;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            const __m128 nB = u.v15;

            __m128 a = nA;
            u.v14 = nB;
            u.v15 = a;
            d1Dot(u);
            u.v5 = a;
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                d1Negate5(u);
                a = u.v5;
            }
            u.v7 = t0Vec(frac);
            u.v5 = nB;
            u.v6 = a;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            u.v14 = u.v15;
            d1Normalise(u);

            u.v11 = m.q(s4 & ~0xFu);
            u.v12 = u.v15;
            d1Qmul(u);
            m.putq(s4 & ~0xFu, u.v13);
            s1 -= 1;
            s4 -= 0x10u;
        } while (s1 != -1);

        SET_GPR_VEC(ctx, 2, _mm_castps_si128(lastDotVec));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(lastDot));
        SET_GPR_S32(ctx, 3, -1);
        ctx->f[0] = fromBits(lastDot);
        ctx->vu0_vf[5] = u.v5;
        ctx->vu0_vf[6] = u.v6;
        ctx->vu0_vf[7] = u.v7;
        ctx->vu0_vf[8] = u.v8;
        ctx->vu0_vf[9] = u.v9;
        ctx->vu0_vf[11] = u.v11;
        ctx->vu0_vf[12] = u.v12;
        ctx->vu0_vf[13] = u.v13;
        ctx->vu0_vf[14] = u.v14;
        ctx->vu0_vf[15] = u.v15;
        ctx->vu0_vf[16] = u.v16;
        ctx->vu0_acc = u.acc;
        ctx->vu0_q = u.q;
        ctx->vu0_status = u.status;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void gazeD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!sotc_decomp::motionOn() || sotc_decomp::p1charaOn() || jalTarget(rdram, 0x010C62F0u) != kAtan2 ||
                                 jalTarget(rdram, 0x010C63C4u) != kWrapper || jalTarget(rdram, 0x010C63D8u) != kWrapper ||
                                 jalTarget(rdram, 0x010C63ECu) != kWrapper || jalTarget(rdram, 0x010B64E8u) != kCurveEval,
                             0))
            return gaze(rdram, ctx, runtime);
        ++c_d1Gaze.n;
        gazeD1Body(rdram, ctx, runtime);
    }
    void gazeD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
    void gazeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            gazeD3(rdram, ctx, runtime);
        else
            gaze(rdram, ctx, runtime);
    }
} // namespace decomp_gaze

// ---- Round D3 (d1native): manager_sub_010C6238 partial -> full -----------------------------------------------------------
// The D1 body's guest calls as direct core calls: iosGetATan2fExact as decomp_quat's D2 core (round 122's atan2ExactCore;
// the callee leaves f0 only), each key's curveEval as decomp_curve's D2 core (d2c::curveCore) with the leftovers it chains
// from one call to the next as the old calls leave them (fcr31, a2's 128 bits, the status of the context, vf1). Every key
// of every node is tested first with decomp_curve's d3CurveTakes (curveCore's own refusals: format 13, the recompiled
// formats, an overlap, a non-finite channel word) and d3CurveReadsMiss (no word a decode reads lies in the frame's key
// slots or the pose the loop writes), so the core runs with no fallback; any key it would refuse, curveEval's slot held by
// something else, fastnative off / SOTC_CURVE_STATS, quat122 off or its check mode, P1 on, a worker lane: the D1 body.
// The registers the old body leaves (with its callees' leftovers): v0 (the last dot's 128 bits, low word sign-extended),
// v1 -1, a0-a3 / t0 as the last key's call set them (t0's low word the blend weight's bits), f0 the last dot, f12 / f13,
// fcr31, vf1 (the last packed key's), vf5-vf16, ACC, Q and the status of the gaze's own normalisations.
namespace decomp_curve
{
    void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void curveEvalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool d3CurveDefault();
    namespace d2c
    {
        // (CurveRegs: decomp_curve.h)
        bool curveCore(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index, CurveRegs &r);
        bool d3CurveTakes(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index);
        bool d3CurveReadsMiss(const d1::Mem &m, uint32_t chan, uint32_t index, uint32_t lo, uint32_t hi);
    }
}
namespace decomp_quat
{
    void atan2Exact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void atan2ExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d2q
    {
        float atan2ExactD1Core(float y, float x);
    }
}
namespace char122
{
    int checkEvery(); // decomp_char122.cpp: SOTC_CHAR122_CHECK's period (0: off)
    namespace quat
    {
        bool on();
    }
}

namespace decomp_gaze
{
    D1_COUNTER(c_d3Gaze, "manager_sub_010C6238.d3");

    struct GazeD3
    {
        // in
        uint32_t a0, a1, a2, sp, identity, identity2;
        uint64_t r8hi, r21hi;
        // in / out: what the curveEval calls chain (a2's 128 bits, fcr31, the context's status, vf1)
        __m128 r6, vf1;
        uint32_t fcr31;
        uint16_t cstatus;
        // out
        __m128 r2;
        float f0, f12, f13;
        uint64_t r4, r5, r7, r8;
        bool early;
    };
    PS2_FORCE_INLINE uint64_t d3sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    PS2_FORCE_INLINE __m128 d3vec(uint64_t lo, uint64_t hi)
    {
        const uint64_t w[2] = {lo, hi};
        __m128 v;
        std::memcpy(&v, w, 16);
        return v;
    }
    PS2_FORCE_INLINE bool d3inRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }

    // The direction's key index k, as gazeD3Core computes it (the same statements; no side effect).
    int32_t gazeD3Key(const d1::Mem &m, uint32_t a2)
    {
        bool c;
        const float f21 = d1Clamp1(m.f32(a2 + 4u), c);
        const float f20 = d1Clamp1(m.f32(a2 + 0u), c);
        const float ang = decomp_quat::d2q::atan2ExactD1Core(f20, f21);
        const float f3 = d1::lt(ang, 0.0f) ? d1::add(ang, fromBits(0x40C90FDAu)) : d1::add(ang, fromBits(0u));
        uint32_t fcr = 0u;
        return ps2FpuConvertToWord(ps2FpuDivide(fcr, d1::mul(f3, fromBits(0x41000000u)), fromBits(0x40C90FDAu)));
    }
    // Every key the loop decodes is one curveCore takes (tested on the words as they stand; the decodes' words lie outside
    // what the loop writes: the frame's three key slots and the pose). false: the D1 body.
    bool gazeD3Plan(const d1::Mem &m, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t sp)
    {
        if (!d3inRam(a1, 8u) || !d3inRam(a2, 12u) || !d3inRam(sp, 0x110u))
            return false;
        const int32_t n = m.s32(a1 + 4u);
        if (n < 0 || n > 0x1000)
            return false;
        if (n == 0)
            return true;
        const uint32_t poseLo = a0 & ~0xFu, poseHi = a0 + (static_cast<uint32_t>(n) << 4);
        if (!d3inRam(poseLo, poseHi - poseLo))
            return false;
        const int32_t k = gazeD3Key(m, a2);
        const uint32_t keys[3] = {0u, (static_cast<uint32_t>(k) & 7u) + 1u, ((static_cast<uint32_t>(k) + 1u) & 7u) + 1u};
        const uint32_t res = m.u32(a2 + 8u);
        if (!d3inRam(res + 8u, 12u))
            return false;
        for (int32_t node = 0; node < n; ++node)
        {
            const uint32_t t0 = m.u32(res + 0x10u), v1 = m.u32(res + 8u) + static_cast<uint32_t>(node), v0 = (static_cast<uint32_t>(node) << 2) + t0;
            if (!d3inRam(v1, 1u) || !d3inRam(v0, 4u))
                return false;
            const uint64_t fmt = m.get<uint8_t>(v1);
            const uint32_t chan = static_cast<uint32_t>(m.s32(v0));
            for (uint32_t i = 0; i < 3u; ++i)
                if (!decomp_curve::d2c::d3CurveTakes(m, sp + i * 0x10u, fmt, chan, keys[i]) ||
                    !decomp_curve::d2c::d3CurveReadsMiss(m, chan, keys[i], sp, sp + 0x30u) ||
                    !decomp_curve::d2c::d3CurveReadsMiss(m, chan, keys[i], poseLo, poseHi))
                    return false;
        }
        return true;
    }

    // gazeD1Body's statements with the atan2 and the keys as core calls.
    void gazeD3Core(const d1::Mem &m, GazeD3 &g, D1Vu &u)
    {
        const uint32_t sp = g.sp, a0 = g.a0, a1 = g.a1, a2 = g.a2;
        const float f1 = m.f32(a2 + 4u), f2 = m.f32(a2 + 0u);
        int32_t s1 = m.s32(a1 + 4u);
        const uint32_t res = m.u32(a2 + 8u);
        uint32_t s4 = a0 + (static_cast<uint32_t>(s1) << 4) - 0x10u;
        bool c;
        const float f21 = d1Clamp1(f1, c);
        const float f20 = d1Clamp1(f2, c);
        g.f12 = f20;
        g.f13 = f21;
        // iosGetATan2fExact(f12, f13): its D2 core (f0 only)
        const float ang = decomp_quat::d2q::atan2ExactD1Core(f20, f21);
        c = d1::lt(ang, 0.0f);
        g.fcr31 = c ? (g.fcr31 | 0x800000u) : (g.fcr31 & ~0x800000u);
        const float f3 = c ? d1::add(ang, fromBits(0x40C90FDAu)) : d1::add(ang, fromBits(0u));
        uint32_t fcr = 0u; // (as the D1 body: the flags these two can raise need a negative root / a zero divisor)
        const float len = ps2FpuSqrt(fcr, d1::add(d1::mul(f21, f21), d1::mul(f20, f20)));
        const float fk = ps2FpuDivide(fcr, d1::mul(f3, fromBits(0x41000000u)), fromBits(0x40C90FDAu));
        const int32_t k = ps2FpuConvertToWord(fk);
        const float fkw = static_cast<float>(k);
        const float frac = d1::sub(fk, fkw);
        s1 -= 1;
        if (s1 == -1)
        {
            g.early = true;
            g.r2 = d3vec(d3sx(static_cast<uint32_t>(k)), 0u);
            g.f0 = fkw;
            return;
        }
        g.early = false;
        const uint32_t keyA = (static_cast<uint32_t>(k) & 7u) + 1u, keyB = ((static_cast<uint32_t>(k) + 1u) & 7u) + 1u;
        const uint64_t s5low = 0xFFFFFFFFBF800000ull;
        uint32_t lastDot = 0u;
        __m128 lastDotVec = d1::zero4();
        float weight = 0.0f;

        // GetMotionNodeIntFrameDirectResource's loads, then curveEval's D2 core (as the tail jump: the chained leftovers).
        auto key = [&](uint32_t out, uint32_t keyIndex) -> __m128
        {
            const uint32_t node = static_cast<uint32_t>(s1);
            const uint32_t t0 = m.u32(res + 0x10u), v1 = m.u32(res + 8u) + node, v0 = (node << 2) + t0;
            const uint64_t fmt = m.get<uint8_t>(v1);
            const uint32_t chan = static_cast<uint32_t>(m.s32(v0));
            decomp_curve::d2c::CurveRegs r;
            r.fcr31 = g.fcr31;
            r.status = g.cstatus;
            r.a2q = g.r6;
            const uint64_t low = d3sx(chan);
            std::memcpy(&r.a2q, &low, 8);
            r.setF0 = r.vu1 = r.vu7 = false;
            decomp_curve::d2c::curveCore(m, out, fmt, chan, keyIndex, r); // (taken: gazeD3Plan)
            g.fcr31 = r.fcr31;
            g.r6 = r.a2q;
            if (r.vu1)
            {
                g.r6 = r.vf1;
                g.vf1 = r.vf1;
                g.cstatus = r.status;
            }
            g.r4 = d3sx(out);
            g.r5 = fmt;
            g.r7 = d3sx(keyIndex);
            return m.q(out & ~0xFu);
        };
        auto dotSign = [&]() -> bool
        {
            lastDotVec = u.v15;
            lastDot = bitsOf(d1::lane<0>(u.v15));
            return static_cast<int32_t>(lastDot) < 0;
        };
        // t0 = the weight's bits (mfc1: sign-extended low 64, t0's upper half as it was) / qmtc2 t0, vf7.
        auto t0Vec = [&](float f)
        {
            weight = f;
            return d3vec(d3sx(bitsOf(f)), g.r8hi);
        };
        const __m128 minus1 = d3vec(s5low, g.r21hi);

        do
        {
            const __m128 k0 = key(sp, 0u);
            const __m128 kA = key(sp + 0x10u, keyA);
            const __m128 kB = key(sp + 0x20u, keyB);
            u.v11 = k0;
            u.v5 = d1::vsel(u.v5, d1::vsub(u.v0, d1::bc<3>(u.v0)), d1::mask(1, 0, 0, 0));
            u.v11 = d1::vsel(u.v11, d1::vmul(u.v11, d1::bc<0>(u.v5)), d1::mask(1, 1, 1, 0));
            const __m128 conj = u.v11;
            u.v12 = kA;
            d1Qmul(u);
            const __m128 relA = u.v13;
            u.v11 = conj;
            u.v12 = kB;
            d1Qmul(u);
            const __m128 relB = u.v13;

            __m128 id = m.q(g.identity & ~0xFu);
            u.v14 = relA;
            u.v15 = id;
            d1Dot(u);
            if (dotSign())
            {
                u.v5 = id;
                u.v6 = minus1;
                d1Negate5(u);
                id = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relA;
            u.v6 = id;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            const __m128 nA = u.v15;

            __m128 id2 = m.q(g.identity2 & ~0xFu);
            u.v14 = relB;
            u.v15 = id2;
            d1Dot(u);
            u.v5 = id2;
            if (dotSign())
            {
                u.v6 = minus1;
                d1Negate5(u);
                id2 = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relB;
            u.v6 = id2;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            const __m128 nB = u.v15;

            __m128 a = nA;
            u.v14 = nB;
            u.v15 = a;
            d1Dot(u);
            u.v5 = a;
            if (dotSign())
            {
                u.v6 = minus1;
                d1Negate5(u);
                a = u.v5;
            }
            u.v7 = t0Vec(frac);
            u.v5 = nB;
            u.v6 = a;
            d1Blend(u);
            u.v14 = u.v9;
            d1Normalise(u);
            u.v14 = u.v15;
            d1Normalise(u);

            u.v11 = m.q(s4 & ~0xFu);
            u.v12 = u.v15;
            d1Qmul(u);
            m.putq(s4 & ~0xFu, u.v13);
            s1 -= 1;
            s4 -= 0x10u;
        } while (s1 != -1);

        uint64_t dotLow = d3sx(lastDot);
        g.r2 = lastDotVec;
        std::memcpy(&g.r2, &dotLow, 8);
        g.f0 = fromBits(lastDot);
        g.r8 = d3sx(bitsOf(weight));
    }

    void gazeD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || sotc_decomp::p1charaOn() || sotc_decomp::onWorkerLane(runtime) || !decomp_curve::d3CurveDefault() ||
            !char122::quat::on() || char122::checkEvery() > 0 || jalTarget(rdram, 0x010C62F0u) != kAtan2 || jalTarget(rdram, 0x010C63C4u) != kWrapper ||
            jalTarget(rdram, 0x010C63D8u) != kWrapper || jalTarget(rdram, 0x010C63ECu) != kWrapper || jalTarget(rdram, 0x010B64E8u) != kCurveEval)
            return gazeD1(rdram, ctx, runtime);
        const PS2Runtime::RecompiledFunction at = sotc_decomp::guestFunction(runtime, kAtan2), ce = sotc_decomp::guestFunction(runtime, kCurveEval);
        if (!(at == &decomp_quat::atan2ExactSw || at == &decomp_quat::atan2Exact) || !(ce == &decomp_curve::curveEvalSw || ce == &decomp_curve::curveEval))
            return gazeD1(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29);
        GazeD3 g;
        g.a0 = GPR_U32(ctx, 4);
        g.a1 = GPR_U32(ctx, 5);
        g.a2 = GPR_U32(ctx, 6);
        g.sp = sp0 - 0x110u;
        if (sp0 < 0x1110u || !gazeD3Plan(m, g.a0, g.a1, g.a2, g.sp))
            return gazeD1(rdram, ctx, runtime);
        ++c_d3Gaze.n;
        g.identity = sotc_decomp::relocated(rdram, 0x010C646Cu, 0x010C6470u);
        g.identity2 = sotc_decomp::relocated(rdram, 0x010C6508u, 0x010C650Cu);
        {
            uint64_t w[2];
            std::memcpy(w, &ctx->r[8], 16);
            g.r8hi = w[1];
            std::memcpy(w, &ctx->r[21], 16);
            g.r21hi = w[1];
        }
        std::memcpy(&g.r6, &ctx->r[6], 16);
        g.vf1 = ctx->vu0_vf[1];
        g.fcr31 = ctx->fcr31;
        g.cstatus = ctx->vu0_status;
        D1Vu u;
        u.v0 = ctx->vu0_vf[0];
        u.v5 = ctx->vu0_vf[5];
        u.v6 = ctx->vu0_vf[6];
        u.v7 = ctx->vu0_vf[7];
        u.v8 = ctx->vu0_vf[8];
        u.v9 = ctx->vu0_vf[9];
        u.v11 = ctx->vu0_vf[11];
        u.v12 = ctx->vu0_vf[12];
        u.v13 = ctx->vu0_vf[13];
        u.v14 = ctx->vu0_vf[14];
        u.v15 = ctx->vu0_vf[15];
        u.v16 = ctx->vu0_vf[16];
        u.acc = ctx->vu0_acc;
        u.q = ctx->vu0_q;
        u.status = ctx->vu0_status;
        gazeD3Core(m, g, u);
        ctx->f[12] = g.f12;
        ctx->f[13] = g.f13;
        ctx->f[0] = g.f0;
        ctx->fcr31 = g.fcr31;
        if (g.early)
        {
            SET_GPR_U64(ctx, 2, d3sx(static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(g.r2)))));
            return returnTo(ctx);
        }
        std::memcpy(&ctx->r[2], &g.r2, 16);
        SET_GPR_S32(ctx, 3, -1);
        SET_GPR_U64(ctx, 4, g.r4);
        SET_GPR_U64(ctx, 5, g.r5);
        std::memcpy(&ctx->r[6], &g.r6, 16);
        SET_GPR_U64(ctx, 7, g.r7);
        SET_GPR_U64(ctx, 8, g.r8);
        ctx->vu0_vf[1] = g.vf1;
        ctx->vu0_vf[5] = u.v5;
        ctx->vu0_vf[6] = u.v6;
        ctx->vu0_vf[7] = u.v7;
        ctx->vu0_vf[8] = u.v8;
        ctx->vu0_vf[9] = u.v9;
        ctx->vu0_vf[11] = u.v11;
        ctx->vu0_vf[12] = u.v12;
        ctx->vu0_vf[13] = u.v13;
        ctx->vu0_vf[14] = u.v14;
        ctx->vu0_vf[15] = u.v15;
        ctx->vu0_vf[16] = u.v16;
        ctx->vu0_acc = u.acc;
        ctx->vu0_q = u.q;
        ctx->vu0_status = u.status;
        returnTo(ctx);
    }
} // namespace decomp_gaze
