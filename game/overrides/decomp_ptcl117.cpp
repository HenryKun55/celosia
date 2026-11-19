// Round 117 (SOTC_DRAW117, default on; A/B `draw117`, `r117all`): the
// particle geodisps' draw chain as host code, from the display walk's class
// callback down to each emitter's drawer.
//
// - disp_particle_geodisp (0x113f6a8; a1 the geodisp): its class
//   (GetGeoDispClass, inline: geodisp + 0x20 when its byte +8 is 0x24), the
//   emitter list's head (class + 0xe8) and a tail jump to 0113ACB8 with the
//   class's +0x80 / +0x40 (the view's two vectors).
// - 0113ACB8 (a0 the first list node, a1 / a2 those vectors): the display
//   list layer by the LOD record's kind (+0xc: 4 -> 0x13) and the record's
//   +0xc4 with the global at 0x1216ed0 (0x12 with modelResetSurface first,
//   0x16, else 0xb; *0x1216ed0 and the record's +0xc0 can skip the whole
//   list), iosDlPushLayer, then the nodes in runs of the same draw key
//   (record +0x34): a run of one straight to 01140ED0, longer runs as an
//   array of node pointers on the stack (sp lowered by its 16-aligned
//   size) qsorted with 01140EA8 (guest calls) and drawn in that order;
//   iosDlPopLayer.
// - 01140ED0 (a0 the node): 0113B408 (the emitter's view direction, its
//   colour blend toward the class's fog colour, once a frame: +0x110),
//   then, when the clip pass kept it (+0x164 == 1), disp_particle_emitter.
// - disp_particle_emitter (0x113d988; a0 the emitter, a1 its record, a2
//   the parameters): only with +0x198 == 1; texTransRequest for the LOD
//   record's texture (kind != 4); then the drawer by the kind (+0xc) and
//   the record's flags: particleDraw, 0113E6C8, 0113DBF8 (mode 01141E80),
//   01141D20 (the quads), 0113EC40, 0113DEA0, 0113E218 (kind 4, with
//   get_particle_parameter_blend_disp), with get_world_matrixSR into the
//   frame first where the original does it. Every callee a guest call
//   (straight to its native when the table holds one).
//
// The recompiled code's operations in its order (0113B408's VU0 block with
// its macros), its frames and the saved registers' slots written as it
// writes them, s0-s7 / fp holding the code's values at every guest call and
// reloaded at the end as its epilogues do; v0 / f0 as the last callee
// leaves them. Not as the original: the scratch GPRs (at, v1, a0-t4 past
// the calls' arguments), the run loop's EE checkpoint. A `break` (a kind 0
// LOD record) or memory outside main RAM goes to the recompiled code
// before anything is written.
#include "sotc_decomp.h"
#include "s5_particles.h"
#include "sotc_worker.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <vector>
#include "a9sweep.h"

namespace decomp_ptcl117
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    constexpr uint32_t kGeodispDraw = 0x0113F6A8u, kList = 0x0113ACB8u, kNode = 0x01140ED0u, kEmitter = 0x0113D988u;
    constexpr uint32_t kResetSurface = 0x010993C8u, kQsort = 0x0116A310u, kCompare = 0x01140EA8u;
    constexpr uint32_t kParticleDraw = 0x0113E940u, kDrawAnimated = 0x0113E6C8u, kDrawSinCos = 0x0113DBF8u, kQuads = 0x01141D20u, kDrawEC40 = 0x0113EC40u,
                       kDrawSpan = 0x0113DEA0u, kDrawLit = 0x0113E218u, kMode = 0x01141E80u, kWorldMatrix = 0x01140418u, kBlendDisp = 0x01145BA8u,
                       kTexTransRequest = 0x010AAD48u;
    constexpr uint32_t kGlobal = 0x01216ED0u;

    inline bool on() { return sotc_decomp::draw117On(); }
    inline bool ram(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && a + n <= PS2_RAM_SIZE; }
    inline uint32_t rd(const uint8_t *rdram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline void wr64(uint8_t *rdram, uint32_t a, uint64_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
    inline void wr32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
    inline uint64_t rd64(const uint8_t *rdram, uint32_t a)
    {
        uint64_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
        return v;
    }
    inline __m128i rd128(const uint8_t *rdram, uint32_t a)
    {
        __m128i v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
        return v;
    }
    inline void wr128(uint8_t *rdram, uint32_t a, __m128i v) { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline uint32_t x1bc(uint32_t i)
    {
        uint32_t v = i << 3;
        v -= i;
        v <<= 4;
        v -= i;
        return v << 2;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // ---- 0113B408 (a0 E, a1 the record, a2 its LOD records, a3 the class's
    // +0x80 vectors, t0 the class's +0x40 rows): once a frame (+0x110) the
    // emitter's fog-blended colour (+0x70), its position copy (+0x80) and,
    // for a view-facing kind, the unit direction (+0x90) from the rows.
    void viewOnce(uint8_t *rdram, R5900Context *ctx, uint32_t E, uint32_t Mr, uint32_t recs, uint32_t a3, uint32_t t4, uint32_t sp)
    {
        if (rd(rdram, E + 0x110u) != 0u)
            return;
        __m128 *vf = ctx->vu0_vf;
        const uint32_t t0 = rd(rdram, Mr + 0x64u);
        wr64(rdram, sp + 8u, rd64(rdram, 0x011958B8u));
        wr64(rdram, sp, rd64(rdram, 0x011958B0u));
        bool have = false; // t2
        auto q = [&](uint32_t a) { return _mm_castsi128_ps(rd128(rdram, a)); };
        auto put = [&](uint32_t a, __m128 v) { wr128(rdram, a, _mm_castps_si128(v)); };
        const __m128i xyz = _mm_set_epi32(0, -1, -1, -1);
        if (t0 == 1u)
        {
            put(sp, _mm_castsi128_ps(rd128(rdram, a3 + 0x30u)));
            have = true;
        }
        else if (t0 == 0u || t0 >= 6u)
        {
            // The default colour at 0x1e0fe0 (the lui in the taken branch's delay slot, the addiu: relocated, read live).
            const uint32_t hi = (sotc_decomp::codeWord(rdram, t0 == 0u ? 0x0113B44Cu : 0x0113B458u) & 0xFFFFu) << 16;
            const uint32_t at = hi + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(sotc_decomp::codeWord(rdram, 0x0113B4D0u) & 0xFFFFu)));
            SET_GPR_VEC(ctx, 3, rd128(rdram, at)); // lq v1 (the whole register)
            wr128(rdram, E + 0x70u, GPR_VEC(ctx, 3));
        }
        else
        {
            vf[5] = q(a3);
            vf[6] = q(a3 + 0x10u);
            vf[5] = PS2_VBLEND(vf[5], PS2_VADD(vf[5], vf[6]), _mm_castsi128_ps(xyz));
            put(sp, vf[5]);
            vf[5] = q(sp);
            vf[6] = q(a3 + 0x20u);
            vf[5] = PS2_VBLEND(vf[5], PS2_VADD(vf[5], vf[6]), _mm_castsi128_ps(xyz));
            put(sp, vf[5]);
            ctx->f[0] = fb(0x3EAAAAAAu);
            vf[5] = q(sp);
            SET_GPR_U32(ctx, 2, 0x3EAAAAAAu); // qmtc2.ni of the 32-bit v0
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            vf[5] = _mm_blendv_ps(vf[5], PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(xyz));
            put(sp, vf[5]);
            vf[5] = q(sp);
            vf[6] = q(a3 + 0x30u);
            vf[5] = PS2_VBLEND(vf[5], PS2_VADD(vf[5], vf[6]), _mm_castsi128_ps(xyz));
            put(sp, vf[5]);
            have = true;
        }
        __m128i pos;
        if (have)
        {
            // The colour x the record's +0x6c, each of x / y / z clamped to 0..1,
            // blended with the default colour by +0x68 into +0x70.
            float f0 = fb(rd(rdram, Mr + 0x6Cu));
            ctx->f[0] = f0;
            SET_GPR_U32(ctx, 3, bf(f0));
            vf[5] = q(sp);
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
            vf[5] = _mm_blendv_ps(vf[5], PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(xyz));
            put(sp, vf[5]);
            const float f2 = 0.0f;
            float f1 = ctx->f[1];
            for (uint32_t i = 0; i < 3u; ++i)
            {
                const uint32_t at = sp + i * 4u;
                f0 = fb(rd(rdram, at));
                bool c = FPU_C_OLT_S(f0, f2);
                setC(ctx, c);
                if (c)
                    wr32(rdram, at, 0u);
                else
                {
                    f1 = fb(0x3F800000u);
                    c = FPU_C_OLT_S(f1, f0);
                    setC(ctx, c);
                    if (c)
                        wr32(rdram, at, bf(f1));
                }
            }
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            const uint32_t hi = (sotc_decomp::codeWord(rdram, 0x0113B548u) & 0xFFFFu) << 16;
            const uint32_t def = hi + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(sotc_decomp::codeWord(rdram, 0x0113B550u) & 0xFFFFu)));
            f0 = fb(rd(rdram, Mr + 0x68u));
            ctx->f[0] = f0;
            SET_GPR_U32(ctx, 8, bf(f0));
            vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            vf[5] = q(sp);
            vf[6] = q(def);
            vf[8] = _mm_blendv_ps(vf[8], PS2_VSUB_K(1, 0, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
            ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, PS2_VMUL(vf[5], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(xyz));
            {
                const __m128 mul = PS2_VMUL_K(0, 1, vf[6], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3, 3, 3, 3)));
                vf[5] = _mm_blendv_ps(vf[5], PS2_VADD_K(1, 1, ctx->vu0_acc, mul), _mm_castsi128_ps(xyz));
            }
            put(E + 0x70u, vf[5]);
        }
        pos = rd128(rdram, a3 + 0x30u);
        SET_GPR_VEC(ctx, 2, pos);
        wr128(rdram, E + 0x80u, pos);
        const uint32_t flag = 1u;
        if (rd(rdram, Mr + 0xB0u) == 1u || rd(rdram, recs + 0x164u) == 2u)
        {
            // The direction: the rows' z column, negated, normalised (RSQRT).
            float f1 = fb(rd(rdram, t4));
            const float f2 = fb(0xBF800000u);
            ctx->f[2] = f2;
            wr32(rdram, E + 0x90u, bf(f1));
            const float f0 = fb(rd(rdram, t4 + 0x10u));
            ctx->f[0] = f0;
            wr32(rdram, E + 0x94u, bf(f0));
            f1 = fb(rd(rdram, t4 + 0x20u));
            ctx->f[1] = f1;
            wr32(rdram, E + 0x9Cu, 0u);
            wr32(rdram, E + 0x98u, bf(f1));
            const uint32_t d = E + 0x90u;
            vf[5] = q(d);
            SET_GPR_U32(ctx, 3, bf(f2));
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
            vf[5] = _mm_blendv_ps(vf[5], PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(xyz));
            put(d, vf[5]);
            vf[5] = q(d);
            vf[7] = PS2_VBLEND(vf[7], PS2_VMUL(vf[5], vf[5]), _mm_castsi128_ps(xyz));
            const __m128i w = _mm_set_epi32(-1, 0, 0, 0);
            __m128 acc = _mm_blendv_ps(ctx->vu0_acc, PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0))), _mm_castsi128_ps(w));
            acc = _mm_blendv_ps(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1)))), _mm_castsi128_ps(w));
            ctx->vu0_acc = acc;
            vf[7] = _mm_blendv_ps(vf[7], PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2)))), _mm_castsi128_ps(w));
            {
                const float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 0, 0, 3)));
                const float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 3)));
                ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft);
            }
            vf[5] = _mm_blendv_ps(vf[5], PS2_VMUL(vf[5], _mm_set1_ps(ctx->vu0_q)), _mm_castsi128_ps(xyz));
            put(d, vf[5]);
        }
        wr32(rdram, E + 0x110u, flag);
    }

    // ---- SOTC_DRAW117_CHECK=<n> -----------------------------------------
    // Every n-th call of each native the recompiled function and the native
    // are run from the same state with their callees replaced by a recorder
    // (the target, a0 - a3, t0, ra, sp, s0 - s7, fp and f12 at each call; the
    // callee not run: 01141E80, a pure leaf, still runs), the two call lists
    // and the registers / frame each leaves compared; then state put back and
    // the native run for real. 0113B408 (viewOnce) against the recompiled
    // one directly: the emitter's 0x200 bytes, vf5 - vf8, ACC, Q, f0 - f2.
    // [draw117c] every 1000 checks and at each difference (first 20).
    struct Rec
    {
        uint32_t target, a[5], ra, sp;
        uint64_t sreg[9];
        uint32_t f12;
        bool operator==(const Rec &o) const
        {
            if (target != o.target || ra != o.ra || sp != o.sp || !A9_MEMEQ(sreg, o.sreg, sizeof sreg))
                return false;
            const int n = argsRead(target);
            for (int i = 0; i < n; ++i)
                if (a[i] != o.a[i])
                    return false;
            return target != 0x01145BA8u || f12 == o.f12;
        }
        // The argument registers each callee reads (a0 .. a3, t0).
        static int argsRead(uint32_t t)
        {
            switch (t)
            {
            case 0x0113E940u: case 0x0113EC40u: case 0x0113DEA0u: case 0x010AAD48u: case 0x01140ED0u: return 3;
            case 0x0113E6C8u: case 0x01141D20u: case 0x01140418u: case 0x01145BA8u: case 0x0116A310u: return 4;
            case 0x0113DBF8u: case 0x0113E218u: return 5;
            case 0x010993C8u: case 0x001C0210u: return 1;
            case 0x001BF8C0u: return 0;
            default: return 1; // the layer push (a0); the pop reads none, its a0 the push's
            }
        }
    };
    std::vector<Rec> *g_recs = nullptr;
    void recorder(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        Rec r{};
        r.target = ctx->pc;
        for (int i = 0; i < 4; ++i)
            r.a[i] = GPR_U32(ctx, 4 + i);
        r.a[4] = GPR_U32(ctx, 8);
        r.ra = GPR_U32(ctx, 31);
        r.sp = GPR_U32(ctx, 29);
        for (int i = 0; i < 8; ++i)
            r.sreg[i] = GPR_U64(ctx, 16 + i);
        r.sreg[8] = GPR_U64(ctx, 30);
        std::memcpy(&r.f12, &ctx->f[12], 4);
        if (g_recs)
            g_recs->push_back(r);
        ctx->pc = GPR_U32(ctx, 31);
    }
    struct Spy
    {
        uint32_t at[16];
        PS2Runtime::RecompiledFunction was[16];
        int n = 0;
        void on(uint32_t target)
        {
            const uint32_t slot = (target - g_ps2RecompiledFunctionTableBase) >> 2;
            if ((target & 3u) || slot >= g_ps2RecompiledFunctionTableSlotCount || n >= 16)
                return;
            at[n] = slot;
            was[n] = g_ps2RecompiledFunctionTable[slot];
            g_ps2RecompiledFunctionTable[slot] = &recorder;
            ++n;
        }
        ~Spy()
        {
            for (int i = n - 1; i >= 0; --i)
                g_ps2RecompiledFunctionTable[at[i]] = was[i];
        }
    };
    struct Check117
    {
        int every = -1;
        uint64_t calls[3] = {}, checks[3] = {}, differ[3] = {};
    };
    Check117 g_check;
    inline bool checkDue(int which)
    {
        Check117 &c = g_check;
        if (c.every < 0)
            c.every = ps2_diag::env("SOTC_DRAW117_CHECK") ? std::atoi(ps2_diag::env("SOTC_DRAW117_CHECK")) : 0;
        return c.every > 0 && ++c.calls[which] % static_cast<uint64_t>(c.every) == 0u;
    }
    void noteCheck(int which, bool same, const char *what)
    {
        static const char *const names[3] = {"dispEmitter", "list", "viewOnce"};
        Check117 &c = g_check;
        ++c.checks[which];
        if (!same && c.differ[which]++ < 20u)
            std::fprintf(stderr, "[draw117c] DIFFERS %s: %s\n", names[which], what);
        if ((c.checks[0] + c.checks[1] + c.checks[2]) % 1000u == 0u)
            std::fprintf(stderr, "[draw117c] checks: dispEmitter %llu (%llu differ), list %llu (%llu), viewOnce %llu (%llu)\n", (unsigned long long)c.checks[0],
                         (unsigned long long)c.differ[0], (unsigned long long)c.checks[1], (unsigned long long)c.differ[1], (unsigned long long)c.checks[2],
                         (unsigned long long)c.differ[2]);
    }
    // Both runs of `fn` (recompiled, then native) from the same context and stack window, spied.
    template <class Run>
    void spyCompare(int which, uint8_t *rdram, R5900Context *ctx, const uint32_t *targets, int nt, uint32_t lo, uint32_t hi, Run recompiledRun, Run nativeRun)
    {
        static std::vector<uint8_t> ctx0, ctxA, stack0, stackA;
        ctx0.resize(sizeof(R5900Context));
        ctxA.resize(sizeof(R5900Context));
        std::memcpy(ctx0.data(), ctx, sizeof(R5900Context));
        stack0.assign(rdram + lo, rdram + hi);
        std::vector<Rec> a, b;
        {
            Spy spy;
            for (int i = 0; i < nt; ++i)
                spy.on(targets[i]);
            g_recs = &a;
            recompiledRun();
            std::memcpy(ctxA.data(), ctx, sizeof(R5900Context));
            stackA.assign(rdram + lo, rdram + hi);
            std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
            std::memcpy(rdram + lo, stack0.data(), hi - lo);
            g_recs = &b;
            nativeRun();
            g_recs = nullptr;
        }
        if (reinterpret_cast<const R5900Context *>(ctxA.data())->pc != GPR_U32(reinterpret_cast<const R5900Context *>(ctx0.data()), 31))
        {
            // The recompiled leg stopped at a checkpoint: not compared.
            std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
            std::memcpy(rdram + lo, stack0.data(), hi - lo);
            return;
        }
        bool same = a.size() == b.size();
        for (size_t i = 0; same && i < a.size(); ++i)
            same = a[i] == b[i];
        char what[400];
        if (!same)
        {
            size_t i = 0;
            while (i < a.size() && i < b.size() && a[i] == b[i])
                ++i;
            int n = std::snprintf(what, sizeof what, "calls %u / %u, first differing #%u", static_cast<unsigned>(a.size()), static_cast<unsigned>(b.size()),
                                  static_cast<unsigned>(i));
            auto dump = [&](const char *tag, const std::vector<Rec> &v)
            {
                if (i >= v.size() || n >= static_cast<int>(sizeof what) - 8)
                    return;
                const Rec &r = v[i];
                n += std::snprintf(what + n, sizeof what - n, " %s{t %08x a %08x %08x %08x %08x t0 %08x ra %08x sp %08x s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x s6 %08x s7 %08x fp %08x}", tag,
                                   r.target, r.a[0], r.a[1], r.a[2], r.a[3], r.a[4], r.ra, r.sp, (unsigned)r.sreg[0], (unsigned)r.sreg[1], (unsigned)r.sreg[2],
                                   (unsigned)r.sreg[3], (unsigned)r.sreg[4], (unsigned)r.sreg[5], (unsigned)r.sreg[6], (unsigned)r.sreg[7], (unsigned)r.sreg[8]);
            };
            dump("rec", a);
            dump("nat", b);
        }
        else
        {
            const R5900Context *ra = reinterpret_cast<const R5900Context *>(ctxA.data());
            bool regs = ra->pc == ctx->pc && GPR_U64(ra, 29) == GPR_U64(ctx, 29) && GPR_U64(ra, 31) == GPR_U64(ctx, 31) && GPR_U64(ra, 30) == GPR_U64(ctx, 30);
            for (int r = 16; r < 24; ++r)
                regs = regs && GPR_U64(ra, r) == GPR_U64(ctx, r);
            const bool stack = std::memcmp(stackA.data(), rdram + lo, hi - lo) == 0;
            same = regs && stack;
            uint32_t firstByte = hi - lo;
            for (uint32_t k = 0; k < hi - lo; ++k)
                if (stackA[k] != rdram[lo + k])
                {
                    firstByte = k;
                    break;
                }
            std::snprintf(what, sizeof what, "same calls (%u); saved registers %d, frame %d (first byte sp0-%x), pc %08x/%08x", static_cast<unsigned>(a.size()),
                          regs ? 1 : 0, stack ? 1 : 0, (hi - lo) - firstByte, ra->pc, ctx->pc);
        }
        noteCheck(which, same, what);
        std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
        std::memcpy(rdram + lo, stack0.data(), hi - lo);
    }

    struct Frame
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        void call(uint32_t target, uint32_t site, uint32_t sp) const { callGuest(rdram, ctx, runtime, target, site + 8u, sp); }
        void callRel(uint32_t site, uint32_t sp) const { call(sotc_decomp::jalTarget(rdram, site), site, sp); }
    };

    // ---- disp_particle_emitter (0x113d988) -----------------------------
    void dispEmitterImpl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t E = GPR_U32(ctx, 4), Mr = GPR_U32(ctx, 5), P = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x90u, ra = GPR_U32(ctx, 31);
        if (!on() || (sp & 0xFu) != 0u || !ram(sp, 0x90u) || !ram(E, 0x200u) || !ram(Mr, 0x130u) || !ram(rd(rdram, Mr + 0xD4u), 4u))
            return sotc_decomp::recompiled(kEmitter)(rdram, ctx, runtime);
        const uint32_t recs = rd(rdram, Mr + 0xD4u);
        const uint32_t R = recs + x1bc(rd(rdram, E + 0x118u));
        if (!ram(R, 0x1BCu) || (rd(rdram, E + 0x198u) == 1u && rd(rdram, R + 0xCu) == 0u))
            return sotc_decomp::recompiled(kEmitter)(rdram, ctx, runtime); // (a kind 0 record: the original's break)
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20),
                       e21 = GPR_U64(ctx, 21), e22 = GPR_U64(ctx, 22), e31 = GPR_U64(ctx, 31);
        wr64(rdram, sp + 0x58u, e17);
        wr64(rdram, sp + 0x60u, e18);
        wr64(rdram, sp + 0x78u, e21);
        wr64(rdram, sp + 0x50u, e16);
        wr64(rdram, sp + 0x68u, e19);
        wr64(rdram, sp + 0x70u, e20);
        wr64(rdram, sp + 0x80u, e22);
        wr64(rdram, sp + 0x88u, e31);
        SET_GPR_U32(ctx, 29, sp);
        const Frame g{rdram, ctx, runtime};
        uint64_t s0 = e16, s3 = e19, s4 = rd(rdram, E + 0x198u), s6 = e22;
        auto regs = [&]
        {
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U32(ctx, 17, E);
            SET_GPR_U32(ctx, 18, Mr);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_S32(ctx, 20, static_cast<int32_t>(s4));
            SET_GPR_U32(ctx, 21, P);
            SET_GPR_U64(ctx, 22, s6);
        };
        auto args = [&](uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
        {
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            SET_GPR_U32(ctx, 6, a2);
            SET_GPR_U32(ctx, 7, a3);
        };
        auto world = [&](uint32_t site, uint32_t flag)
        {
            regs();
            args(sp, E + 0x30u, Mr, flag);
            g.call(kWorldMatrix, site, sp);
        };
        if (s4 == 1u)
        {
            s6 = 4u;
            s3 = R;
            s0 = rd(rdram, R + 0xCu);
            if (s0 != 4u)
            {
                regs();
                const uint32_t tex = rd(rdram, 0x0118D050u + rd(rdram, recs + 8u) * 12u);
                args(tex, 0u, 0u, GPR_U32(ctx, 7));
                g.call(kTexTransRequest, 0x0113DA10u, sp);
                s0 = rd(rdram, R + 0xCu);
            }
            if (s0 == 1u)
            {
                if (rd(rdram, R + 0x18Cu) != 0u)
                {
                    world(0x0113DB10u, 0u);
                    regs();
                    args(E, R, Mr, sp);
                    g.call(kQuads, 0x0113DB24u, sp);
                }
                else if (rd(rdram, Mr + 0xB0u) == 1u)
                {
                    regs();
                    args(E, R, Mr, GPR_U32(ctx, 7));
                    g.call(kParticleDraw, 0x0113DA58u, sp);
                }
                else if (static_cast<int32_t>(rd(rdram, Mr + 0x120u)) >= 2)
                {
                    world(0x0113DA80u, 1u);
                    regs();
                    args(E, R, Mr, sp);
                    g.call(kDrawAnimated, 0x0113DA94u, sp);
                }
                else
                {
                    regs();
                    args(Mr, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7));
                    g.call(kMode, 0x0113DAA8u, sp);
                    s4 = GPR_U32(ctx, 2);
                    world(s4 == 1u ? 0x0113DAC4u : 0x0113DAE0u, s4 == 1u ? 0u : 1u);
                    regs();
                    args(E, R, Mr, static_cast<uint32_t>(s4));
                    SET_GPR_U32(ctx, 8, sp);
                    g.call(kDrawSinCos, 0x0113DAF8u, sp);
                }
            }
            else if (s0 == 2u)
            {
                regs();
                args(E, R, Mr, GPR_U32(ctx, 7));
                g.call(rd(rdram, Mr + 0xB0u) == 1u ? kDrawEC40 : kDrawSpan, rd(rdram, Mr + 0xB0u) == 1u ? 0x0113DB50u : 0x0113DB64u, sp);
            }
            else if (s0 == 4u)
            {
                ctx->f[12] = fb(rd(rdram, E + 0x16Cu));
                regs();
                args(sp + 0x40u, Mr, P, rd(rdram, E + 0x118u));
                g.call(kBlendDisp, 0x0113DB98u, sp);
                world(0x0113DBACu, 1u);
                regs();
                args(E, R, Mr, sp + 0x40u);
                SET_GPR_U32(ctx, 8, sp);
                g.call(kDrawLit, 0x0113DBC4u, sp);
            }
            else
                SET_GPR_U64(ctx, 2, 3u); // kind 3 / others: v0 as the delay slot leaves it
        }
        else
            SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 16, e16);
        SET_GPR_U64(ctx, 17, e17);
        SET_GPR_U64(ctx, 18, e18);
        SET_GPR_U64(ctx, 19, e19);
        SET_GPR_U64(ctx, 20, e20);
        SET_GPR_U64(ctx, 21, e21);
        SET_GPR_U64(ctx, 22, e22);
        SET_GPR_U64(ctx, 31, e31);
        SET_GPR_U32(ctx, 29, sp0);
        ctx->pc = ra;
    }

    void dispEmitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        s5ptcl::joinIfPending(0x0113D988u); // round S5
        if (on() && checkDue(0))
        {
            const uint32_t sp0 = GPR_U32(ctx, 29);
            if (ram(sp0 - 0x100u, 0x100u))
            {
                static const uint32_t targets[] = {kTexTransRequest, kWorldMatrix, kBlendDisp, kParticleDraw, kDrawAnimated, kDrawSinCos, kQuads, kDrawEC40, kDrawSpan, kDrawLit};
                auto rec = [&] { sotc_decomp::recompiled(kEmitter)(rdram, ctx, runtime); };
                auto nat = [&] { dispEmitterImpl(rdram, ctx, runtime); };
                spyCompare(0, rdram, ctx, targets, 10, sp0 - 0x100u, sp0, std::function<void()>(rec), std::function<void()>(nat));
            }
        }
        dispEmitterImpl(rdram, ctx, runtime);
    }

    void nodeBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n, uint32_t a1, uint32_t a2, uint32_t sp0, bool emitterDirect = false);
    // ---- 01140ED0 (a0 the node, a1 / a2 the class's +0x80 / +0x40) -------
    void node(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t n = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x20u;
        if (!on() || (sp & 0xFu) != 0u || !ram(sp - 0x10u, 0x30u) || !ram(n, 0x200u) || !ram(rd(rdram, n), 0x50u))
            return sotc_decomp::recompiled(kNode)(rdram, ctx, runtime);
        const uint32_t Mr = rd(rdram, rd(rdram, n) + 0x48u);
        if (!ram(Mr, 0x130u) || !ram(rd(rdram, Mr + 0xD4u), 0x170u) || !ram(a1, 0x40u) || !ram(a2, 0x30u))
            return sotc_decomp::recompiled(kNode)(rdram, ctx, runtime);
        nodeBody(rdram, ctx, runtime, n, a1, a2, sp0);
    }

    // node's body past its checks (round S22's list calls it straight for nodes its pass checked the same way).
    void nodeBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n, uint32_t a1, uint32_t a2, uint32_t sp0, bool emitterDirect)
    {
        const uint32_t sp = sp0 - 0x20u;
        const uint32_t rec = rd(rdram, n), Mr = rd(rdram, rec + 0x48u), E = n + 0x10u;
        // The frame (s0, s1, ra), 0113B408's own below it (it writes sp - 0x10 .. sp).
        wr64(rdram, sp, GPR_U64(ctx, 16));
        wr64(rdram, sp + 8u, GPR_U64(ctx, 17));
        wr64(rdram, sp + 0x10u, GPR_U64(ctx, 31));
        if (checkDue(2))
        {
            // viewOnce against the recompiled 0113B408 from the same state.
            static std::vector<uint8_t> ctx0, ctxA;
            ctx0.resize(sizeof(R5900Context));
            ctxA.resize(sizeof(R5900Context));
            std::memcpy(ctx0.data(), ctx, sizeof(R5900Context));
            uint8_t e0[0x200], eA[0x200], s0[0x10], sA[0x10];
            std::memcpy(e0, rdram + E, 0x200);
            std::memcpy(s0, rdram + sp - 0x10u, 0x10);
            viewOnce(rdram, ctx, E, Mr, rd(rdram, Mr + 0xD4u), a1, a2, sp - 0x10u);
            std::memcpy(eA, rdram + E, 0x200);
            std::memcpy(sA, rdram + sp - 0x10u, 0x10);
            std::memcpy(ctxA.data(), ctx, sizeof(R5900Context));
            std::memcpy(ctx, ctx0.data(), sizeof(R5900Context));
            std::memcpy(rdram + E, e0, 0x200);
            std::memcpy(rdram + sp - 0x10u, s0, 0x10);
            SET_GPR_U32(ctx, 4, E);
            SET_GPR_U32(ctx, 5, Mr);
            SET_GPR_U32(ctx, 6, rd(rdram, Mr + 0xD4u));
            SET_GPR_U32(ctx, 7, a1);
            SET_GPR_U32(ctx, 8, a2);
            callGuest(rdram, ctx, runtime, 0x0113B408u, 0x01140F04u, sp);
            const R5900Context *A = reinterpret_cast<const R5900Context *>(ctxA.data());
            const bool mem = A9_MEMEQ(eA, rdram + E, 0x200) && A9_MEMEQ(sA, rdram + sp - 0x10u, 0x10);
            bool vu = A9_MEMEQ(&A->vu0_acc, &ctx->vu0_acc, 16) && A9_MEMEQ(&A->vu0_q, &ctx->vu0_q, 4);
            for (int i = 5; i <= 8; ++i)
                vu = vu && A9_MEMEQ(&A->vu0_vf[i], &ctx->vu0_vf[i], 16);
            const bool fl = std::memcmp(A->f, ctx->f, 3 * sizeof(float)) == 0;
            char what[300];
            int wn = std::snprintf(what, sizeof what, "emitter / frame %d, vu0 %d, f0-f2 %d", mem ? 1 : 0, vu ? 1 : 0, fl ? 1 : 0);
            if (!vu)
            {
                auto lanes = [&](const char *tag, const __m128 &x, const __m128 &y)
                {
                    uint32_t p[4], q[4];
                    std::memcpy(p, &x, 16);
                    std::memcpy(q, &y, 16);
                    if (!A9_MEMEQ(p, q, 16) && wn < static_cast<int>(sizeof what) - 80)
                        wn += std::snprintf(what + wn, sizeof what - wn, " %s nat %08x %08x %08x %08x rec %08x %08x %08x %08x", tag, p[0], p[1], p[2], p[3], q[0], q[1], q[2], q[3]);
                };
                lanes("acc", A->vu0_acc, ctx->vu0_acc);
                lanes("vf5", A->vu0_vf[5], ctx->vu0_vf[5]);
                lanes("vf6", A->vu0_vf[6], ctx->vu0_vf[6]);
                lanes("vf7", A->vu0_vf[7], ctx->vu0_vf[7]);
                lanes("vf8", A->vu0_vf[8], ctx->vu0_vf[8]);
                if (!A9_MEMEQ(&A->vu0_q, &ctx->vu0_q, 4))
                    wn += std::snprintf(what + wn, sizeof what - wn, " q differs");
            }
            noteCheck(2, mem && vu && fl, what);
            // The guest's result goes on (the same, when it matches); sp / ra as at entry.
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U64(ctx, 31, GPR_U64(A, 31));
        }
        else
            viewOnce(rdram, ctx, E, Mr, rd(rdram, Mr + 0xD4u), a1, a2, sp - 0x10u);
        SET_GPR_U32(ctx, 7, a1);
        SET_GPR_U32(ctx, 8, a2);
        if (rd(rdram, E + 0x164u) != 1u)
        {
            SET_GPR_U64(ctx, 2, 1u); // li v0, 1 before the test
            returnTo(ctx);
            return;
        }
        // The tail jump: disp_particle_emitter(E, the record's +0x48 / +0x4c), ra and sp as at entry.
        SET_GPR_U32(ctx, 4, E);
        SET_GPR_U32(ctx, 5, Mr);
        SET_GPR_U32(ctx, 6, rd(rdram, rec + 0x4Cu));
        ctx->pc = kEmitter;
        if (emitterDirect || runtime->guestFunctionAt(kEmitter) == &dispEmitter)
            dispEmitter(rdram, ctx, runtime);
        else
            runtime->guestFunctionAt(kEmitter)(rdram, ctx, runtime);
    }

    // ---- 0113ACB8 (a0 the first node, a1 / a2 the class's +0x80 / +0x40) ---
    void listImpl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t first = GPR_U32(ctx, 4), s6 = GPR_U32(ctx, 5), s5 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x70u;
        if (!on() || (sp & 0xFu) != 0u || !ram(sp, 0x70u) || (first != 0u && (!ram(first, 0x200u) || !ram(rd(rdram, first), 0x50u))))
            return sotc_decomp::recompiled(kList)(rdram, ctx, runtime);
        const uint64_t e[10] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19), GPR_U64(ctx, 20),
                                GPR_U64(ctx, 21), GPR_U64(ctx, 22), GPR_U64(ctx, 23), GPR_U64(ctx, 30), GPR_U64(ctx, 31)};
        static const uint32_t slot[10] = {0x20u, 0x28u, 0x30u, 0x38u, 0x40u, 0x48u, 0x50u, 0x58u, 0x60u, 0x68u};
        uint64_t s[8] = {e[0], e[1], e[2], first, e[4], s5, s6, e[7]}; // s0 .. s7
        const Frame g{rdram, ctx, runtime};
        auto regs = [&](uint32_t curSp)
        {
            for (int i = 0; i < 8; ++i)
                SET_GPR_U64(ctx, 16 + i, s[i]);
            SET_GPR_U32(ctx, 30, sp);
            SET_GPR_U32(ctx, 29, curSp);
        };
        auto finish = [&]
        {
            for (int i = 0; i < 8; ++i)
                SET_GPR_U64(ctx, 16 + i, e[i]);
            SET_GPR_U64(ctx, 30, e[8]);
            SET_GPR_U64(ctx, 31, e[9]);
            SET_GPR_U32(ctx, 29, sp0);
            ctx->pc = static_cast<uint32_t>(e[9]);
        };
        if (first == 0u)
        {
            for (int i = 0; i < 10; ++i)
                wr64(rdram, sp + slot[i], e[i]);
            return finish();
        }
        // Check every node of the list first: anything outside main RAM goes to the recompiled code.
        for (uint32_t n = first, guard = 0; n != 0u; n = rd(rdram, n + 0x1B0u))
            if (!ram(n, 0x200u) || !ram(rd(rdram, n), 0x50u) || ++guard > 4096u)
                return sotc_decomp::recompiled(kList)(rdram, ctx, runtime);
        for (int i = 0; i < 10; ++i)
            wr64(rdram, sp + slot[i], e[i]);
        // The layer.
        const uint32_t Mr = rd(rdram, rd(rdram, first) + 0x48u);
        const uint32_t R = rd(rdram, Mr + 0xD4u) + x1bc(rd(rdram, first + 0x128u));
        const uint32_t kind = rd(rdram, Mr + 0xC0u);
        if (!ram(R, 0x10u))
            return sotc_decomp::recompiled(kList)(rdram, ctx, runtime);
        const uint32_t global = rd(rdram, kGlobal);
        if ((kind == 1u && global == 0u) || (kind == 2u && global == 1u))
        {
            // Skipped: no layer pushed or popped.
            return finish();
        }
        uint32_t layer = 0xBu;
        if (rd(rdram, R + 0xCu) == 4u)
            layer = 0x13u;
        else
        {
            const uint32_t c4 = rd(rdram, Mr + 0xC4u);
            if (global == 0u)
                layer = c4 == 0u ? 0x12u : 0xBu;
            else
                layer = c4 == 1u ? 0x12u : c4 == 2u ? 0x16u : 0xBu;
        }
        s[0] = layer;
        if (layer == 0x12u)
        {
            regs(sp);
            SET_GPR_U32(ctx, 4, 0x12u);
            g.call(kResetSurface, 0x0113ADBCu, sp);
        }
        regs(sp);
        SET_GPR_U32(ctx, 4, layer);
        g.callRel(0x0113ADC4u, sp); // iosDlPushLayer
        s[7] = 0x01140000u;
        uint32_t a2 = first;
        for (;;)
        {
            // The run: the nodes after a2 with the same draw key (record +0x34).
            uint32_t count = 0u, n = rd(rdram, a2 + 0x1B0u);
            const uint32_t key = rd(rdram, rd(rdram, a2) + 0x34u);
            for (;;)
            {
                ++count;
                if (n == 0u || rd(rdram, rd(rdram, n) + 0x34u) != key)
                    break;
                n = rd(rdram, n + 0x1B0u);
            }
            s[3] = n;
            s[2] = count;
            if (count < 2u)
            {
                regs(sp);
                SET_GPR_U32(ctx, 4, a2);
                SET_GPR_U32(ctx, 5, s6);
                SET_GPR_U32(ctx, 6, s5);
                g.call(kNode, 0x0113AE18u, sp);
            }
            else
            {
                s[4] = sp;
                const uint32_t lowered = sp - (((count << 2) + 0xFu) >> 4 << 4);
                s[1] = lowered;
                uint32_t a0 = a2;
                for (uint32_t i = 0; i < count; ++i)
                {
                    wr32(rdram, lowered + i * 4u, a0);
                    a0 = rd(rdram, a0 + 0x1B0u);
                }
                s[0] = 0u;
                regs(lowered);
                SET_GPR_U32(ctx, 4, lowered);
                SET_GPR_U32(ctx, 5, count);
                SET_GPR_U32(ctx, 6, 4u);
                SET_GPR_U32(ctx, 7, s[7] + 0xEA8u);
                g.call(kQsort, 0x0113AE78u, lowered);
                for (uint32_t i = 0; i < count; ++i)
                {
                    s[1] = lowered + (i + 1u) * 4u;
                    s[0] = count - i - 1u;
                    regs(lowered);
                    SET_GPR_U32(ctx, 4, rd(rdram, lowered + i * 4u));
                    SET_GPR_U32(ctx, 5, s6);
                    SET_GPR_U32(ctx, 6, s5);
                    g.call(kNode, 0x0113AE9Cu, lowered);
                }
            }
            if (n == 0u)
                break;
            a2 = n;
        }
        regs(sp);
        g.callRel(0x0113AEB8u, sp); // iosDlPopLayer
        finish();
    }

    // ---- Round S22 (SOTC_S22PTCL, A/B s22ptcl) ---------------------------
    // The list without a guest call per node. listImpl above is the
    // original's shape: each node through callGuest (the table, the run-to-
    // return check), a run of more than one node sorted by the guest qsort
    // (the lifted libc one, 0116A310) with 01140EA8 as its comparator, a
    // guest call per comparison. listHost: one pass over the nodes (node's
    // checks made once for each), each run's order from a host copy of that
    // qsort (the BSD one the game links: insertion sort below 7, med3 / the
    // ninther, the swap_cnt fallback) over the nodes' depths (node +0x144,
    // the comparator's c.lt.s as the runtime's compare key; it never answers
    // 0), then each node's body (0113B408, the tail into
    // disp_particle_emitter) called straight with the registers and sp the
    // original call had. The drawers, the layer push / pop and
    // modelResetSurface stay calls of the table (once a list, or the
    // emitter's own). Not as the original: the list's and qsort's save
    // slots and the sorted pointer array on the stack (dead below the
    // caller's sp), the scratch registers qsort / the comparator leave
    // (f0 / f1, fcr31's C, a0-t3).
    // SOTC_S22PTCL_CHECK=<n>: every n-th list (1 with any value below 1) run
    // both ways from the same state with the drawers / layer calls recorded,
    // not run (the calls with their registers and sp, the nodes' 0x200 bytes,
    // the saved registers, VU0, f20-f31 compared); SOTC_S22PTCL_LOG=<frames>
    // [s22ptcl] counts (default 600 with the check).
    namespace s22
    {
        struct Item
        {
            uint32_t node, key;
        };
        struct Stats
        {
            uint64_t lists = 0, host = 0, nodes = 0, runs = 0, sorted = 0, sortedNodes = 0, old = 0, checks = 0, differ = 0, skipped = 0, gateCalls = 0;
            uint64_t why[8] = {}; // listHost's declines: empty, frame / view, worker, table, node, record, model, R
            uint32_t lastFirst = 0;
        };
        Stats g_st;
        int g_check = -1;
        uint64_t g_log = 0, g_logFrame = 0, g_checkN = 0;
        std::vector<Item> g_items;
        std::vector<uint32_t> g_runEnd;

        void init()
        {
            const char *c = ps2_diag::env("SOTC_S22PTCL_CHECK");
            g_check = (c && c[0] != '0') ? std::max(1, std::atoi(c)) : 0;
            const char *l = ps2_diag::env("SOTC_S22PTCL_LOG");
            g_log = l ? static_cast<uint64_t>(std::atoll(l)) : (g_check > 0 ? 600u : 0u);
        }
        void log()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (g_log == 0u || frame < g_logFrame + g_log)
                return;
            g_logFrame = frame;
            const Stats &t = g_st;
            std::fprintf(stderr, "[s22ptcl] frame %llu: lists %llu host %llu old %llu nodes %llu runs %llu sorted %llu (%llu nodes); check %llu differ %llu skipped %llu gate calls %llu\n",
                         (unsigned long long)frame, (unsigned long long)t.lists, (unsigned long long)t.host, (unsigned long long)t.old, (unsigned long long)t.nodes,
                         (unsigned long long)t.runs, (unsigned long long)t.sorted, (unsigned long long)t.sortedNodes, (unsigned long long)t.checks,
                         (unsigned long long)t.differ, (unsigned long long)t.skipped, (unsigned long long)t.gateCalls);
            std::fprintf(stderr, "[s22ptcl] declined: empty %llu frame %llu worker %llu table %llu node %llu record %llu model %llu R %llu (last first %08x)\n",
                         (unsigned long long)t.why[0], (unsigned long long)t.why[1], (unsigned long long)t.why[2], (unsigned long long)t.why[3],
                         (unsigned long long)t.why[4], (unsigned long long)t.why[5], (unsigned long long)t.why[6], (unsigned long long)t.why[7], t.lastFirst);
        }

        // 01140EA8: (*a)->+0x144 < (*b)->+0x144 ? 1 : -1 (c.lt.s on the runtime's compare keys).
        inline int cmp(const Item *a, const Item *b) { return a->key < b->key ? 1 : -1; }
        inline void swap(Item *a, Item *b)
        {
            const Item t = *a;
            *a = *b;
            *b = t;
        }
        inline void vecswap(Item *a, Item *b, ptrdiff_t n)
        {
            for (ptrdiff_t i = 0; i < n; ++i)
                swap(a + i, b + i);
        }
        inline Item *med3(Item *a, Item *b, Item *c)
        {
            return cmp(a, b) < 0 ? (cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a)) : (cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c));
        }
        inline void insertion(Item *a, size_t n)
        {
            for (Item *pm = a + 1; pm < a + n; ++pm)
                for (Item *pl = pm; pl > a && cmp(pl - 1, pl) > 0; --pl)
                    swap(pl, pl - 1);
        }
        // The game's qsort (BSD 4.4 / newlib), its element the 4-byte node pointer.
        void qsortBsd(Item *a, size_t n)
        {
        loop:
            int swapCnt = 0;
            if (n < 7u)
            {
                insertion(a, n);
                return;
            }
            Item *pm = a + n / 2u;
            if (n > 7u)
            {
                Item *pl = a, *pn = a + (n - 1u);
                if (n > 40u)
                {
                    const size_t d = n / 8u;
                    pl = med3(pl, pl + d, pl + 2u * d);
                    pm = med3(pm - d, pm, pm + d);
                    pn = med3(pn - 2u * d, pn - d, pn);
                }
                pm = med3(pl, pm, pn);
            }
            swap(a, pm);
            Item *pa = a + 1, *pb = a + 1, *pc = a + (n - 1u), *pd = a + (n - 1u);
            for (;;)
            {
                int r;
                while (pb <= pc && (r = cmp(pb, a)) <= 0)
                {
                    if (r == 0)
                    {
                        swapCnt = 1;
                        swap(pa, pb);
                        ++pa;
                    }
                    ++pb;
                }
                while (pb <= pc && (r = cmp(pc, a)) >= 0)
                {
                    if (r == 0)
                    {
                        swapCnt = 1;
                        swap(pc, pd);
                        --pd;
                    }
                    --pc;
                }
                if (pb > pc)
                    break;
                swap(pb, pc);
                swapCnt = 1;
                ++pb;
                --pc;
            }
            if (swapCnt == 0)
            {
                insertion(a, n);
                return;
            }
            Item *const pn = a + n;
            ptrdiff_t r = std::min(pa - a, pb - pa);
            vecswap(a, pb - r, r);
            r = std::min(pd - pc, pn - pd - 1);
            vecswap(pb, pn - r, r);
            if ((r = pb - pa) > 1)
                qsortBsd(a, static_cast<size_t>(r));
            if ((r = pd - pc) > 1)
            {
                a = pn - r;
                n = static_cast<size_t>(r);
                goto loop;
            }
        }
    }

    // The list the S22 way. Returns false (nothing done) when a node or the list's state is outside what it handles:
    // the caller runs listImpl.
    bool listHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t first = GPR_U32(ctx, 4), s6 = GPR_U32(ctx, 5), s5 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x70u;
        s22::Stats &st = s22::g_st;
        st.lastFirst = first;
        if (first == 0u)
            return ++st.why[0], false;
        if ((sp & 0xFu) != 0u || !ram(sp, 0x70u) || !ram(s6, 0x40u) || !ram(s5, 0x30u))
            return ++st.why[1], false;
        if (sotc_decomp::onWorkerLane(runtime))
            return ++st.why[2], false;
        // The table holds round 117's natives, or round 95's gates around them (sotc_worker.cpp: the S5 batch's join, the
        // guest worker's join while a job runs, early95's skip); a gated node is called straight only while the gate
        // would just join S5's batch and call it (else through the table, as before).
        const PS2Runtime::RecompiledFunction nodeFn = sotc_decomp::guestFunction(runtime, kNode), emFn = sotc_decomp::guestFunction(runtime, kEmitter);
        if (sotc_worker::behindGate(kNode, nodeFn) != &node || sotc_worker::behindGate(kEmitter, emFn) != &dispEmitter)
            return ++st.why[3], false;
        const bool gated = nodeFn != &node || emFn != &dispEmitter;
        // The nodes, node's checks made here once each; the runs (draw key, record +0x34) marked by their ends.
        std::vector<s22::Item> &items = s22::g_items;
        std::vector<uint32_t> &ends = s22::g_runEnd;
        items.clear();
        ends.clear();
        uint32_t lastKey = 0u;
        for (uint32_t n = first; n != 0u; n = rd(rdram, n + 0x1B0u))
        {
            if (!ram(n, 0x200u) || items.size() >= 4096u)
                return ++st.why[4], false;
            const uint32_t rec = rd(rdram, n);
            if (!ram(rec, 0x50u))
                return ++st.why[5], false;
            const uint32_t Mr = rd(rdram, rec + 0x48u);
            if (!ram(Mr, 0x130u) || !ram(rd(rdram, Mr + 0xD4u), 0x170u))
                return ++st.why[6], false;
            const uint32_t key = rd(rdram, rec + 0x34u);
            if (!items.empty() && key != lastKey)
                ends.push_back(static_cast<uint32_t>(items.size()));
            lastKey = key;
            items.push_back({n, 0u});
        }
        ends.push_back(static_cast<uint32_t>(items.size()));
        // The layer, as listImpl picks it.
        const uint32_t Mr0 = rd(rdram, rd(rdram, first) + 0x48u);
        const uint32_t R = rd(rdram, Mr0 + 0xD4u) + x1bc(rd(rdram, first + 0x128u));
        if (!ram(R, 0x10u))
            return ++st.why[7], false;
        const uint64_t e[10] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19), GPR_U64(ctx, 20),
                                GPR_U64(ctx, 21), GPR_U64(ctx, 22), GPR_U64(ctx, 23), GPR_U64(ctx, 30), GPR_U64(ctx, 31)};
        auto finish = [&]
        {
            for (int i = 0; i < 8; ++i)
                SET_GPR_U64(ctx, 16 + i, e[i]);
            SET_GPR_U64(ctx, 30, e[8]);
            SET_GPR_U64(ctx, 31, e[9]);
            SET_GPR_U32(ctx, 29, sp0);
            ctx->pc = static_cast<uint32_t>(e[9]);
        };
        const uint32_t kind = rd(rdram, Mr0 + 0xC0u), global = rd(rdram, kGlobal);
        ++s22::g_st.host;
        if ((kind == 1u && global == 0u) || (kind == 2u && global == 1u))
        {
            finish(); // skipped: no layer pushed or popped
            return true;
        }
        uint32_t layer = 0xBu;
        if (rd(rdram, R + 0xCu) == 4u)
            layer = 0x13u;
        else
        {
            const uint32_t c4 = rd(rdram, Mr0 + 0xC4u);
            if (global == 0u)
                layer = c4 == 0u ? 0x12u : 0xBu;
            else
                layer = c4 == 1u ? 0x12u : c4 == 2u ? 0x16u : 0xBu;
        }
        uint64_t s[8] = {layer, e[1], e[2], first, e[4], s5, s6, e[7]}; // s0 .. s7 as listImpl keeps them
        const Frame g{rdram, ctx, runtime};
        auto regs = [&](uint32_t curSp)
        {
            for (int i = 0; i < 8; ++i)
                SET_GPR_U64(ctx, 16 + i, s[i]);
            SET_GPR_U32(ctx, 30, sp);
            SET_GPR_U32(ctx, 29, curSp);
        };
        if (layer == 0x12u)
        {
            regs(sp);
            SET_GPR_U32(ctx, 4, 0x12u);
            g.call(kResetSurface, 0x0113ADBCu, sp);
        }
        regs(sp);
        SET_GPR_U32(ctx, 4, layer);
        g.callRel(0x0113ADC4u, sp); // iosDlPushLayer
        s[7] = 0x01140000u;
        // A node: what callGuest(kNode) did, node's body straight (its checks made above).
        auto drawNode = [&](uint32_t n, uint32_t curSp, uint32_t site)
        {
            regs(curSp);
            SET_GPR_U32(ctx, 4, n);
            SET_GPR_U32(ctx, 5, s6);
            SET_GPR_U32(ctx, 6, s5);
            if (gated && (sotc_worker::busy() || sotc_worker::skipPending()))
            {
                ++s22::g_st.gateCalls;
                g.call(kNode, site, curSp); // the gate would join the worker or skip: through the table
                return;
            }
            if (gated)
                s5ptcl::joinIfPending(kNode); // the gate's first step (0113D988's own join is in dispEmitter)
            const uint32_t ra = site + 8u;
            SET_GPR_U32(ctx, 31, ra);
            ctx->pc = kNode;
            nodeBody(rdram, ctx, runtime, n, s6, s5, curSp, true);
            runtime->checkTransfer();
            if (__builtin_expect(ctx->pc != ra || GPR_U32(ctx, 29) != curSp, 0))
                sotc_decomp::finishGuestCall(rdram, ctx, runtime, ra, curSp);
        };
        uint32_t begin = 0u;
        for (const uint32_t end : ends)
        {
            const uint32_t count = end - begin;
            s[3] = end < items.size() ? items[end].node : 0u;
            s[2] = count;
            ++s22::g_st.runs;
            if (count < 2u)
                drawNode(items[begin].node, sp, 0x0113AE18u);
            else
            {
                ++s22::g_st.sorted;
                s22::g_st.sortedNodes += count;
                s[4] = sp;
                const uint32_t lowered = sp - (((count << 2) + 0xFu) >> 4 << 4);
                s22::Item *const a = items.data() + begin;
                for (uint32_t i = 0; i < count; ++i)
                    a[i].key = ps2FpuCompareKey(fb(rd(rdram, a[i].node + 0x144u)));
                s22::qsortBsd(a, count);
                for (uint32_t i = 0; i < count; ++i)
                {
                    s[1] = lowered + (i + 1u) * 4u;
                    s[0] = count - i - 1u;
                    drawNode(a[i].node, lowered, 0x0113AE9Cu);
                }
            }
            begin = end;
        }
        s22::g_st.nodes += items.size();
        regs(sp);
        g.callRel(0x0113AEB8u, sp); // iosDlPopLayer
        finish();
        return true;
    }

    // SOTC_S22PTCL_CHECK: the old list and the host one from the same state, the leaf calls recorded, not run.
    void s22Check(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t first = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29);
        std::vector<uint32_t> nodes;
        for (uint32_t n = first; n != 0u; n = rd(rdram, n + 0x1B0u))
        {
            if (!ram(n, 0x200u) || nodes.size() >= 1024u)
            {
                ++s22::g_st.skipped;
                return;
            }
            nodes.push_back(n);
        }
        const uint32_t window = 0x70u + static_cast<uint32_t>(nodes.size()) * 4u + 0x1000u;
        if (nodes.empty() || sp0 < window || !ram(sp0 - window, window))
        {
            ++s22::g_st.skipped;
            return;
        }
        const uint32_t lo = sp0 - window;
        static std::vector<uint8_t> nodes0, nodesA, stack0;
        nodes0.resize(nodes.size() * 0x200u);
        nodesA.resize(nodes.size() * 0x200u);
        for (size_t i = 0; i < nodes.size(); ++i)
            std::memcpy(nodes0.data() + i * 0x200u, rdram + nodes[i], 0x200u);
        stack0.assign(rdram + lo, rdram + sp0);
        static R5900Context c0, cA;
        c0 = *ctx;
        std::vector<Rec> a, b;
        bool hostRan = false;
        {
            Spy spy;
            const uint32_t targets[] = {kTexTransRequest, kWorldMatrix, kBlendDisp, kParticleDraw, kDrawAnimated, kDrawSinCos, kQuads, kDrawEC40,
                                        kDrawSpan, kDrawLit, kResetSurface, sotc_decomp::jalTarget(rdram, 0x0113ADC4u), sotc_decomp::jalTarget(rdram, 0x0113AEB8u)};
            for (const uint32_t t : targets)
                spy.on(t);
            g_recs = &a;
            listImpl(rdram, ctx, runtime);
            cA = *ctx;
            for (size_t i = 0; i < nodes.size(); ++i)
                std::memcpy(nodesA.data() + i * 0x200u, rdram + nodes[i], 0x200u);
            *ctx = c0;
            for (size_t i = 0; i < nodes.size(); ++i)
                std::memcpy(rdram + nodes[i], nodes0.data() + i * 0x200u, 0x200u);
            std::memcpy(rdram + lo, stack0.data(), window);
            g_recs = &b;
            hostRan = listHost(rdram, ctx, runtime);
            g_recs = nullptr;
        }
        if (!hostRan || cA.pc != GPR_U32(&c0, 31))
            ++s22::g_st.skipped; // the host path declined, or the old one stopped at a checkpoint
        else
        {
            ++s22::g_st.checks;
            char what[512];
            int wn = 0;
            what[0] = 0;
            size_t i = 0;
            while (i < a.size() && i < b.size() && a[i] == b[i])
                ++i;
            bool same = a.size() == b.size() && i == a.size();
            if (!same)
            {
                wn += std::snprintf(what + wn, sizeof what - wn, "calls %u / %u, first differing #%u", static_cast<unsigned>(a.size()), static_cast<unsigned>(b.size()),
                                    static_cast<unsigned>(i));
                auto dump = [&](const char *tag, const std::vector<Rec> &v)
                {
                    if (i >= v.size() || wn >= static_cast<int>(sizeof what) - 8)
                        return;
                    const Rec &r = v[i];
                    wn += std::snprintf(what + wn, sizeof what - wn, " %s{t %08x a %08x %08x %08x %08x t0 %08x ra %08x sp %08x s0 %08x s1 %08x s2 %08x s3 %08x}", tag, r.target,
                                        r.a[0], r.a[1], r.a[2], r.a[3], r.a[4], r.ra, r.sp, (unsigned)r.sreg[0], (unsigned)r.sreg[1], (unsigned)r.sreg[2], (unsigned)r.sreg[3]);
                };
                dump("old", a);
                dump("new", b);
            }
            bool regs = cA.pc == ctx->pc && GPR_U64(&cA, 29) == GPR_U64(ctx, 29) && GPR_U64(&cA, 31) == GPR_U64(ctx, 31) && GPR_U64(&cA, 30) == GPR_U64(ctx, 30);
            for (int r = 16; r < 24; ++r)
                regs = regs && GPR_U64(&cA, r) == GPR_U64(ctx, r);
            regs = regs && std::memcmp(&cA.f[20], &ctx->f[20], 12 * sizeof(float)) == 0;
            bool vu = A9_MEMEQ(&cA.vu0_acc, &ctx->vu0_acc, 16) && A9_MEMEQ(&cA.vu0_q, &ctx->vu0_q, 4);
            for (int r = 0; r < 32; ++r)
                vu = vu && A9_MEMEQ(&cA.vu0_vf[r], &ctx->vu0_vf[r], 16);
            size_t badNode = nodes.size();
            for (size_t k = 0; k < nodes.size() && badNode == nodes.size(); ++k)
                if (std::memcmp(nodesA.data() + k * 0x200u, rdram + nodes[k], 0x200u) != 0)
                    badNode = k;
            if (!regs || !vu || badNode != nodes.size())
            {
                same = false;
                wn += std::snprintf(what + wn, sizeof what - wn, " regs %d vu0 %d node %d/%u", regs ? 1 : 0, vu ? 1 : 0, static_cast<int>(badNode),
                                    static_cast<unsigned>(nodes.size()));
            }
            if (!same && s22::g_st.differ++ < 20u)
                std::fprintf(stderr, "[s22ptcl] differ: frame %llu list %08x nodes %u: %s\n", (unsigned long long)sotc_decomp::g_gameFrame, first,
                             static_cast<unsigned>(nodes.size()), what);
        }
        *ctx = c0;
        for (size_t i = 0; i < nodes.size(); ++i)
            std::memcpy(rdram + nodes[i], nodes0.data() + i * 0x200u, 0x200u);
        std::memcpy(rdram + lo, stack0.data(), window);
    }

    void list(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (sotc_decomp::switchOn(sotc_decomp::kS22Ptcl) && on())
        {
            if (__builtin_expect(s22::g_check < 0, 0))
                s22::init();
            ++s22::g_st.lists;
            if (__builtin_expect(s22::g_check > 0, 0) && ++s22::g_checkN % static_cast<uint64_t>(s22::g_check) == 0u)
                s22Check(rdram, ctx, runtime);
            if (__builtin_expect(s22::g_log != 0u, 0))
                s22::log();
            if (listHost(rdram, ctx, runtime))
                return;
            ++s22::g_st.old;
        }
        if (on() && checkDue(1))
        {
            const uint32_t sp0 = GPR_U32(ctx, 29);
            if (ram(sp0 - 0x800u, 0x800u))
            {
                const uint32_t targets[] = {kNode, kQsort, kResetSurface, sotc_decomp::jalTarget(rdram, 0x0113ADC4u), sotc_decomp::jalTarget(rdram, 0x0113AEB8u)};
                auto rec = [&] { sotc_decomp::recompiled(kList)(rdram, ctx, runtime); };
                auto nat = [&] { listImpl(rdram, ctx, runtime); };
                spyCompare(1, rdram, ctx, targets, 5, sp0 - 0x800u, sp0, std::function<void()>(rec), std::function<void()>(nat));
            }
        }
        listImpl(rdram, ctx, runtime);
    }

    // ---- disp_particle_geodisp (0x113f6a8; a1 the geodisp) -------------
    void geodispDraw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        s5ptcl::joinIfPending(0x0113F6A8u); // round S5: the draws read the records
        const uint32_t gd = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x20u;
        if (!on() || (sp & 0xFu) != 0u || !ram(sp, 0x20u) || (gd != 0u && !ram(gd, 0x110u)))
            return sotc_decomp::recompiled(kGeodispDraw)(rdram, ctx, runtime);
        // GetGeoDispClass, inline (decomp_ptcl62's): geodisp + 0x20 when its byte +8 is 0x24.
        auto cls = [&](uint32_t a) -> uint32_t { return (a != 0u && rdram[(a + 8u) & PS2_RAM_MASK] == 0x24u) ? a + 0x20u : 0u; };
        const uint32_t c = cls(gd);
        const uint32_t head = gd != 0u ? c + 0xE8u : 0u;
        if (!ram(head, 4u) || (c != 0u && !ram(c, 0xF0u)))
            return sotc_decomp::recompiled(kGeodispDraw)(rdram, ctx, runtime);
        wr64(rdram, sp, GPR_U64(ctx, 16));
        wr64(rdram, sp + 8u, GPR_U64(ctx, 17));
        wr64(rdram, sp + 0x10u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 2, head);
        SET_GPR_U32(ctx, 4, rd(rdram, head));
        SET_GPR_U32(ctx, 5, c + 0x80u);
        SET_GPR_U32(ctx, 6, c + 0x40u);
        ctx->pc = kList;
        if (runtime->guestFunctionAt(kList) == &list)
            list(rdram, ctx, runtime);
        else
            runtime->guestFunctionAt(kList)(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x0113F6A8u, disp_particle_geodisp, decomp_ptcl117::geodispDraw);
SOTC_DECOMP_FUNCTION(0x0113ACB8u, manager_sub_0113ACB8, decomp_ptcl117::list);
SOTC_DECOMP_FUNCTION(0x01140ED0u, manager_sub_01140ED0, decomp_ptcl117::node);
SOTC_DECOMP_FUNCTION(0x0113D988u, disp_particle_emitter, decomp_ptcl117::dispEmitter);
