// The particle step from its top (round 8): doit_particle_geodisp
// (0x113f500, a geodisp's emitters each frame, from the display walk's
// class callback), 0113AAE0 (the emitter list: each emitter's step, the
// dead ones unlinked and freed), 0113B610 (an emitter's step: the frame
// step pointer by the effect's name, its distance and depth to the camera,
// the blend parameters) and 0113B928 (its particles: the local matrix of
// types 2 / 3, the wind, each live particle's step (particleUpdate,
// decomp_particles.cpp, called straight), new particles from the free
// list). Each calls the next straight when the function table holds it
// (else through the table: a difftest check, SOTC_DECOMP_SKIP, a profiler
// probe); the rest stay guest calls with the original's frame and
// callee-saved registers (strcmp, 01141090 / 011410D0 / 01141150 /
// 01141230 / 01141400 / 01141080, the parameter blend, the wind, the
// camera context, iosGetRandom, iosUnitMatrix, 0113C1A8 (a new
// particle), the collision finder and iosFree for a dead emitter).
//
// The recompiler's operations in the original order (FPU_* / PS2_V*
// macros); the matrix block of 0113B928 is the recompiled code's own
// statements on locals (lifted from build/runner). Pointers outside main
// RAM go to the recompiled code before anything is written.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/native_tuning.h"

#include <cstring>

namespace decomp_particles
{
    void particleUpdate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_ptclupdate
{
    constexpr uint32_t kGeodisp = 0x0113F500u, kList = 0x0113AAE0u, kEmitter = 0x0113B610u, kStep = 0x0113B928u, kUpdate = 0x0113CA58u;

    inline float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    // SOTC_PTCL_STATS=1 (debugging): which paths ran, every 600 calls of the list.
    struct Stats
    {
        bool on = std::getenv("SOTC_PTCL_STATS") != nullptr;
        uint64_t lists = 0, emitters = 0, steps = 0, matrix = 0, unit = 0, plain = 0, done = 0, spawned = 0, dead = 0, fallbacks = 0;
    };
    Stats *g_stats = nullptr;
    inline Stats &stats()
    {
        if (!g_stats)
            g_stats = new Stats;
        return *g_stats;
    }
    inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    struct Mem
    {
        uint8_t *rdram;
        uint8_t *p(uint32_t a) const { return rdram + (a & PS2_RAM_MASK); }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        float f32(uint32_t a) const { return bits(u32(a)); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a), &v, 4); }
        void putf(uint32_t a, float v) const { std::memcpy(p(a), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(p(a), &v, 8); }
        __m128i q128(uint32_t a) const
        {
            __m128i v;
            std::memcpy(&v, p(a & ~0xFu), 16);
            return v;
        }
        void put128(uint32_t a, __m128i v) const { std::memcpy(p(a & ~0xFu), &v, 16); }
        __m128 q(uint32_t a) const { return _mm_castsi128_ps(q128(a)); }
        void putq(uint32_t a, __m128 v) const { put128(a, _mm_castps_si128(v)); }
    };

    // Runs recompiled code until it returns to ra with sp: what
    // sotc_decomp::callGuest does, from any pc (a function entered at a label).
    void runToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction fn, uint32_t ra, uint32_t sp)
    {
        fn(rdram, ctx, runtime);
        while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
            if (!next)
                break;
            next(rdram, ctx, runtime);
        }
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        Mem m;
        G(uint8_t *r, R5900Context *c, PS2Runtime *rt) : rdram(r), ctx(c), runtime(rt), m{r} {}
        void a(int reg, uint32_t v) const { SET_GPR_S32(ctx, reg, static_cast<int32_t>(v)); }
        void a64(int reg, uint64_t v) const { SET_GPR_U64(ctx, reg, v); }
        uint64_t v0() const { return GPR_U64(ctx, 2); }
        // A jal at `site` (target given), with sp the original's frame.
        void call(uint32_t target, uint32_t site, uint32_t sp) const { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp); }
        // A relocated jal (into the boot ELF): the target read from the code.
        void callRel(uint32_t site, uint32_t sp) const { call(sotc_decomp::jalTarget(rdram, site), site, sp); }
        // One of the natives straight when the table holds it, else through the table.
        void callNative(PS2Runtime::RecompiledFunction fn, uint32_t target, uint32_t site, uint32_t sp) const
        {
            if (runtime->guestFunctionAt(target) != fn)
                return call(target, site, sp);
            SET_GPR_U32(ctx, 31, site + 8u);
            SET_GPR_U32(ctx, 29, sp);
            ctx->pc = target;
            fn(rdram, ctx, runtime);
            if (!(ctx->pc == site + 8u && GPR_U32(ctx, 29) == sp))
            {
                // Left mid-way (a checkpoint in a recompiled fallback): run on to the return.
                PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                if (next)
                    runToReturn(rdram, ctx, runtime, next, site + 8u, sp);
            }
        }
    };

    // ---- 0113B928: an emitter's particles. a0 the emitter E, a1 its effect
    // record (+0x1c the particle count wanted), a2 its kind K (+0 type, +0x10
    // the generation mode), a3 the blend parameters P (0113B610's frame), t0
    // 1: new particles this frame. v0 1 when the emitter is done.
    void step(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Mem &m = g.m;
        const uint32_t E = GPR_U32(ctx, 4), Mr = GPR_U32(ctx, 5), K = GPR_U32(ctx, 6), P = GPR_U32(ctx, 7);
        const uint64_t gen = GPR_U64(ctx, 8);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x170u;
        if ((sp & 0xFu) != 0u || !inRam(sp, 0x170u) || !inRam(E, 0x200u) || !inRam(Mr, 0x100u) || !inRam(K, 0x20u) || !inRam(P, 0x160u) ||
            !inRam(m.u32(0x0118CFE8u), 0x10u))
        {
            stats().fallbacks += stats().on;
            return sotc_decomp::recompiled(kStep)(rdram, ctx, runtime);
        }
        Stats &st = stats();
        st.steps += st.on;
        // The prologue's frame (the epilogue reloads it).
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20),
                       e21 = GPR_U64(ctx, 21), e22 = GPR_U64(ctx, 22);
        const float f20e = ctx->f[20], f21e = ctx->f[21];
        m.put64(sp + 0x128u, e17);
        m.put64(sp + 0x138u, e19);
        m.put64(sp + 0x140u, e20);
        m.put64(sp + 0x148u, e21);
        m.put64(sp + 0x150u, e22);
        m.put64(sp + 0x120u, e16);
        m.put64(sp + 0x130u, e18);
        m.put64(sp + 0x158u, ra0);
        m.putf(sp + 0x168u, f21e);
        m.putf(sp + 0x160u, f20e);
        SET_GPR_U32(ctx, 29, sp);
        g.a(17, E);
        g.a64(19, gen);
        g.a(20, P);
        g.a(21, K);
        g.a(22, Mr);
        // The registers the guest callees see as the original has them.
        uint64_t s0 = e16, s2 = e18, s3 = gen;
        float f20 = f20e, f21 = f21e;
        auto regs = [&]
        {
            g.a64(16, s0);
            g.a64(18, s2);
            g.a64(19, s3);
            ctx->f[20] = f20;
            ctx->f[21] = f21;
        };
        float f0 = ctx->f[0], f1 = ctx->f[1];
        __m128 vf[32];
        std::memcpy(vf, ctx->vu0_vf, sizeof(vf));
        __m128 acc = ctx->vu0_acc;
        __m128i gq[32]; // 128-bit GPRs (the matrix block's t0..t7)
        uint32_t r[32] = {};
        r[17] = E;
        r[29] = sp;

        // vf5 = E+0x30 x P+4 (x y z), at sp.
        f0 = m.f32(P + 4u);
        {
            __m128 v6 = _mm_set1_ps(f0); // qmtc2 of $v1: only its x is read below
            vf[6] = _mm_blend_ps(vf[6], v6, 1);
            const __m128 res = PS2_VMUL(m.q(E + 0x30u), _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
            vf[5] = _mm_blendv_ps(m.q(E + 0x30u), res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            m.putq(sp, vf[5]);
        }
        uint32_t type = m.u32(K);
        s0 = 1u;
        if (type == 1u || type == 3u)
        {
            const __m128 res = PS2_VSUB(m.q(E + 0x10u), m.q(E + 0x40u));
            vf[5] = _mm_blendv_ps(m.q(E + 0x10u), res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            vf[6] = m.q(E + 0x40u);
            m.putq(sp + 0x10u, vf[5]);
            g.a(4, Mr);
            regs();
            g.call(0x01141080u, 0x0113B9B8u, sp);
            f0 = ctx->f[0];
            f1 = ctx->f[1];
            if (g.v0() == 1u)
                m.put32(sp + 0x14u, 0u);
            type = m.u32(K);
        }
        bool matrixBlock = false;
        if (type - 2u < 2u)
        {
            r[2] = sp + 0x60u;
            bool same = true;
            for (uint32_t i = 0; i < 16u && same; i += 4u)
            {
                f1 = m.f32(E + 0x50u + i);
                f0 = m.f32(E + 0x20u + i);
                same = FPU_C_EQ_S(f1, f0);
            }
            if (same)
            {
                st.unit += st.on;
                // iosUnitMatrix(sp + 0x20).
                g.a(4, sp + 0x20u);
                regs();
                ctx->f[0] = f0;
                ctx->f[1] = f1;
                g.callRel(0x0113BD10u, sp);
                f1 = ctx->f[1];
            }
            else
            {
                r[3] = E + 0x50u;
                matrixBlock = true;
                st.matrix += st.on;
            }
        }
        if (matrixBlock)
        {
            // 0x113ba2c .. 0x113bd00, the recompiled statements on locals.
            // 0x113ba2c  addiu       $v1, $s1, 0x50
            r[3] = (uint32_t)((int32_t)ADD32(r[17], 80));
            // 0x113ba30  addiu       $a0, $s1, 0x40
            r[4] = (uint32_t)((int32_t)ADD32(r[17], 64));
            // 0x113ba34  vaddw.xyz   $vf12, $vf0, $vf0w
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[12] = _mm_blendv_ps(vf[12], res, _mm_castsi128_ps(mask)); }
            // 0x113ba38  lqc2        $vf11, 0x0($v1)
            vf[11] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113ba3c  vmr32.w     $vf14, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[14] = _mm_blendv_ps(vf[14], res, _mm_castsi128_ps(mask)); }
            // 0x113ba40  vmr32.w     $vf15, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113ba44  vmr32.w     $vf16, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
            // 0x113ba48  vopmula.xyz $ACC, $vf11, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113ba4c  vmadd.xyz   $vf11, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = _mm_blendv_ps(vf[11], res, _mm_castsi128_ps(mask)); }
            // 0x113ba50  vmadd.xyz   $vf10, $vf11, $vf12
            { __m128 mul_res = PS2_VMUL(vf[11], vf[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
            // 0x113ba54  vmul.xyz    $vf13, $vf11, $vf10
            { __m128 res = PS2_VMUL(vf[11], vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
            // 0x113ba58  vopmula.xyz $ACC, $vf11, $vf10
            { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113ba5c  vmaddw.xyz  $vf15, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113ba60  vmsubw.xyz  $vf16, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
            // 0x113ba64  vopmula.xyz $ACC, $vf13, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113ba68  vmadd.xyz   $vf17, $vf13, $vf12
            { __m128 mul_res = PS2_VMUL(vf[13], vf[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[17] = _mm_blendv_ps(vf[17], res, _mm_castsi128_ps(mask)); }
            // 0x113ba6c  vopmula.xyz $ACC, $vf15, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113ba70  vmadd.xyz   $vf15, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113ba74  vsub.xyz    $vf14, $vf12, $vf17
            { __m128 res = PS2_VSUB(vf[12], vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[14] = PS2_VBLEND(vf[14], res, _mm_castsi128_ps(mask)); }
            // 0x113ba78  vmove.yz    $vf17, $vf14
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf[17] = _mm_blendv_ps(vf[17], vf[14], _mm_castsi128_ps(mask)); }
            // 0x113ba7c  vmove.y     $vf14, $vf16
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[14] = _mm_blendv_ps(vf[14], vf[16], _mm_castsi128_ps(mask)); }
            // 0x113ba80  vmove.y     $vf16, $vf15
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[16] = _mm_blendv_ps(vf[16], vf[15], _mm_castsi128_ps(mask)); }
            // 0x113ba84  vmove.y     $vf15, $vf17
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[15] = _mm_blendv_ps(vf[15], vf[17], _mm_castsi128_ps(mask)); }
            // 0x113ba88  vmove.z     $vf14, $vf15
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[14] = _mm_blendv_ps(vf[14], vf[15], _mm_castsi128_ps(mask)); }
            // 0x113ba8c  vmove.z     $vf15, $vf16
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[15] = _mm_blendv_ps(vf[15], vf[16], _mm_castsi128_ps(mask)); }
            // 0x113ba90  vmove.z     $vf16, $vf17
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[16] = _mm_blendv_ps(vf[16], vf[17], _mm_castsi128_ps(mask)); }
            // 0x113ba94  sqc2        $vf14, 0x0($v0)
            m.put128(ADD32(r[2], 0), _mm_castps_si128(vf[14]));
            // 0x113ba98  sqc2        $vf15, 0x10($v0)
            m.put128(ADD32(r[2], 16), _mm_castps_si128(vf[15]));
            // 0x113ba9c  sqc2        $vf16, 0x20($v0)
            m.put128(ADD32(r[2], 32), _mm_castps_si128(vf[16]));
            // 0x113baa0  lqc2        $vf4, 0x0($a0)
            vf[4] = _mm_castsi128_ps(m.q128(ADD32(r[4], 0)));
            // 0x113baa4  vmove.w     $vf4, $vf0
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[4] = _mm_blendv_ps(vf[4], vf[0], _mm_castsi128_ps(mask)); }
            // 0x113baa8  sqc2        $vf4, 0x30($v0)
            m.put128(ADD32(r[2], 48), _mm_castps_si128(vf[4]));
            // 0x113baac  addiu       $v1, $sp, 0xA0
            r[3] = (uint32_t)((int32_t)ADD32(r[29], 160));
            // 0x113bab0  addiu       $v0, $s1, 0x20
            r[2] = (uint32_t)((int32_t)ADD32(r[17], 32));
            // 0x113bab4  addiu       $a1, $s1, 0x40
            r[5] = (uint32_t)((int32_t)ADD32(r[17], 64));
            // 0x113bab8  vaddw.xyz   $vf12, $vf0, $vf0w
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[12] = _mm_blendv_ps(vf[12], res, _mm_castsi128_ps(mask)); }
            // 0x113babc  lqc2        $vf11, 0x0($v0)
            vf[11] = _mm_castsi128_ps(m.q128(ADD32(r[2], 0)));
            // 0x113bac0  vmr32.w     $vf14, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[14] = _mm_blendv_ps(vf[14], res, _mm_castsi128_ps(mask)); }
            // 0x113bac4  vmr32.w     $vf15, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113bac8  vmr32.w     $vf16, $vf0
            { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
            // 0x113bacc  vopmula.xyz $ACC, $vf11, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113bad0  vmadd.xyz   $vf11, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = _mm_blendv_ps(vf[11], res, _mm_castsi128_ps(mask)); }
            // 0x113bad4  vmadd.xyz   $vf10, $vf11, $vf12
            { __m128 mul_res = PS2_VMUL(vf[11], vf[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
            // 0x113bad8  vmul.xyz    $vf13, $vf11, $vf10
            { __m128 res = PS2_VMUL(vf[11], vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
            // 0x113badc  vopmula.xyz $ACC, $vf11, $vf10
            { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113bae0  vmaddw.xyz  $vf15, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113bae4  vmsubw.xyz  $vf16, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
            // 0x113bae8  vopmula.xyz $ACC, $vf13, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113baec  vmadd.xyz   $vf17, $vf13, $vf12
            { __m128 mul_res = PS2_VMUL(vf[13], vf[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[17] = _mm_blendv_ps(vf[17], res, _mm_castsi128_ps(mask)); }
            // 0x113baf0  vopmula.xyz $ACC, $vf15, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // 0x113baf4  vmadd.xyz   $vf15, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(vf[0], vf[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            // 0x113baf8  vsub.xyz    $vf14, $vf12, $vf17
            { __m128 res = PS2_VSUB(vf[12], vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[14] = PS2_VBLEND(vf[14], res, _mm_castsi128_ps(mask)); }
            // 0x113bafc  vmove.yz    $vf17, $vf14
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf[17] = _mm_blendv_ps(vf[17], vf[14], _mm_castsi128_ps(mask)); }
            // 0x113bb00  vmove.y     $vf14, $vf16
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[14] = _mm_blendv_ps(vf[14], vf[16], _mm_castsi128_ps(mask)); }
            // 0x113bb04  vmove.y     $vf16, $vf15
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[16] = _mm_blendv_ps(vf[16], vf[15], _mm_castsi128_ps(mask)); }
            // 0x113bb08  vmove.y     $vf15, $vf17
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[15] = _mm_blendv_ps(vf[15], vf[17], _mm_castsi128_ps(mask)); }
            // 0x113bb0c  vmove.z     $vf14, $vf15
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[14] = _mm_blendv_ps(vf[14], vf[15], _mm_castsi128_ps(mask)); }
            // 0x113bb10  vmove.z     $vf15, $vf16
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[15] = _mm_blendv_ps(vf[15], vf[16], _mm_castsi128_ps(mask)); }
            // 0x113bb14  vmove.z     $vf16, $vf17
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[16] = _mm_blendv_ps(vf[16], vf[17], _mm_castsi128_ps(mask)); }
            // 0x113bb18  sqc2        $vf14, 0x0($v1)
            m.put128(ADD32(r[3], 0), _mm_castps_si128(vf[14]));
            // 0x113bb1c  sqc2        $vf15, 0x10($v1)
            m.put128(ADD32(r[3], 16), _mm_castps_si128(vf[15]));
            // 0x113bb20  sqc2        $vf16, 0x20($v1)
            m.put128(ADD32(r[3], 32), _mm_castps_si128(vf[16]));
            // 0x113bb24  lqc2        $vf4, 0x0($a1)
            vf[4] = _mm_castsi128_ps(m.q128(ADD32(r[5], 0)));
            // 0x113bb28  vmove.w     $vf4, $vf0
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[4] = _mm_blendv_ps(vf[4], vf[0], _mm_castsi128_ps(mask)); }
            // 0x113bb2c  sqc2        $vf4, 0x30($v1)
            m.put128(ADD32(r[3], 48), _mm_castps_si128(vf[4]));
            // 0x113bb30  addiu       $a0, $sp, 0xE0
            r[4] = (uint32_t)((int32_t)ADD32(r[29], 224));
            // 0x113bb34  addiu       $a1, $sp, 0x60
            r[5] = (uint32_t)((int32_t)ADD32(r[29], 96));
            // 0x113bb38  lqc2        $vf24, 0x30($a1)
            vf[24] = _mm_castsi128_ps(m.q128(ADD32(r[5], 48)));
            // 0x113bb3c  vsubw.x     $vf25, $vf0, $vf0w
            { __m128 res = PS2_VSUB(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[25] = _mm_blendv_ps(vf[25], res, _mm_castsi128_ps(mask)); }
            // 0x113bb40  vmulx.xyz   $vf28, $vf24, $vf25x
            { __m128 res = PS2_VMUL(vf[24], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[28] = _mm_blendv_ps(vf[28], res, _mm_castsi128_ps(mask)); }
            // 0x113bb44  vsub.w      $vf28, $vf28, $vf28
            { __m128 res = PS2_VSUB(vf[28], vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[28] = PS2_VBLEND(vf[28], res, _mm_castsi128_ps(mask)); }
            // 0x113bb48  lq          $t0, 0x0($a1)
            gq[8] = m.q128(ADD32(r[5], 0)); r[8] = (uint32_t)_mm_cvtsi128_si32(gq[8]);
            // 0x113bb4c  lq          $t1, 0x10($a1)
            gq[9] = m.q128(ADD32(r[5], 16)); r[9] = (uint32_t)_mm_cvtsi128_si32(gq[9]);
            // 0x113bb50  lq          $t2, 0x20($a1)
            gq[10] = m.q128(ADD32(r[5], 32)); r[10] = (uint32_t)_mm_cvtsi128_si32(gq[10]);
            // 0x113bb54  qmfc2.ni    $t3, $vf0
            gq[11] = _mm_castps_si128(vf[0]); r[11] = (uint32_t)_mm_cvtsi128_si32(gq[11]);
            // 0x113bb58  pextlw      $t4, $t1, $t0
            gq[12] = PS2_PEXTLW(gq[9], gq[8]); r[12] = (uint32_t)_mm_cvtsi128_si32(gq[12]);
            // 0x113bb5c  pextuw      $t5, $t1, $t0
            gq[13] = PS2_PEXTUW(gq[9], gq[8]); r[13] = (uint32_t)_mm_cvtsi128_si32(gq[13]);
            // 0x113bb60  pextlw      $t6, $t3, $t2
            gq[14] = PS2_PEXTLW(gq[11], gq[10]); r[14] = (uint32_t)_mm_cvtsi128_si32(gq[14]);
            // 0x113bb64  pextuw      $t7, $t3, $t2
            gq[15] = PS2_PEXTUW(gq[11], gq[10]); r[15] = (uint32_t)_mm_cvtsi128_si32(gq[15]);
            // 0x113bb68  pcpyld      $t0, $t6, $t4
            gq[8] = PS2_PCPYLD(gq[14], gq[12]); r[8] = (uint32_t)_mm_cvtsi128_si32(gq[8]);
            // 0x113bb6c  pcpyud      $t1, $t4, $t6
            gq[9] = _mm_unpackhi_epi64(gq[12], gq[14]); r[9] = (uint32_t)_mm_cvtsi128_si32(gq[9]);
            // 0x113bb70  pcpyld      $t2, $t7, $t5
            gq[10] = PS2_PCPYLD(gq[15], gq[13]); r[10] = (uint32_t)_mm_cvtsi128_si32(gq[10]);
            // 0x113bb74  pcpyud      $t3, $t5, $t7
            gq[11] = _mm_unpackhi_epi64(gq[13], gq[15]); r[11] = (uint32_t)_mm_cvtsi128_si32(gq[11]);
            // 0x113bb78  qmtc2.ni    $t0, $vf24
            vf[24] = _mm_castsi128_ps(gq[8]);
            // 0x113bb7c  qmtc2.ni    $t1, $vf25
            vf[25] = _mm_castsi128_ps(gq[9]);
            // 0x113bb80  qmtc2.ni    $t2, $vf26
            vf[26] = _mm_castsi128_ps(gq[10]);
            // 0x113bb84  vmulax.xyzw $ACC, $vf24, $vf28x
            { __m128 res = PS2_VMUL(vf[24], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bb88  vmadday.xyzw $ACC, $vf25, $vf28y
            { __m128 mul_res = PS2_VMUL(vf[25], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bb8c  vmaddaz.xyzw $ACC, $vf26, $vf28z
            { __m128 mul_res = PS2_VMUL(vf[26], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bb90  vmaddw.xyzw $vf29, $vf0, $vf28w
            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[29] = _mm_blendv_ps(vf[29], res, _mm_castsi128_ps(mask)); }
            // 0x113bb94  vsub.w      $vf29, $vf29, $vf29
            { __m128 res = PS2_VSUB(vf[29], vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[29] = PS2_VBLEND(vf[29], res, _mm_castsi128_ps(mask)); }
            // 0x113bb98  vadd.w      $vf29, $vf29, $vf0
            { __m128 res = PS2_VADD(vf[29], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[29] = PS2_VBLEND(vf[29], res, _mm_castsi128_ps(mask)); }
            // 0x113bb9c  sqc2        $vf24, 0x0($a0)
            m.put128(ADD32(r[4], 0), _mm_castps_si128(vf[24]));
            // 0x113bba0  sqc2        $vf25, 0x10($a0)
            m.put128(ADD32(r[4], 16), _mm_castps_si128(vf[25]));
            // 0x113bba4  sqc2        $vf26, 0x20($a0)
            m.put128(ADD32(r[4], 32), _mm_castps_si128(vf[26]));
            // 0x113bba8  sqc2        $vf29, 0x30($a0)
            m.put128(ADD32(r[4], 48), _mm_castps_si128(vf[29]));
            // 0x113bbac  addiu       $v0, $sp, 0x20
            r[2] = (uint32_t)((int32_t)ADD32(r[29], 32));
            // 0x113bbb0  addiu       $v1, $sp, 0xA0
            r[3] = (uint32_t)((int32_t)ADD32(r[29], 160));
            // 0x113bbb4  lqc2        $vf14, 0x0($v1)
            vf[14] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113bbb8  lqc2        $vf15, 0x10($v1)
            vf[15] = _mm_castsi128_ps(m.q128(ADD32(r[3], 16)));
            // 0x113bbbc  lqc2        $vf16, 0x20($v1)
            vf[16] = _mm_castsi128_ps(m.q128(ADD32(r[3], 32)));
            // 0x113bbc0  lqc2        $vf17, 0x30($v1)
            vf[17] = _mm_castsi128_ps(m.q128(ADD32(r[3], 48)));
            // 0x113bbc4  lqc2        $vf24, 0x0($a0)
            vf[24] = _mm_castsi128_ps(m.q128(ADD32(r[4], 0)));
            // 0x113bbc8  lqc2        $vf25, 0x10($a0)
            vf[25] = _mm_castsi128_ps(m.q128(ADD32(r[4], 16)));
            // 0x113bbcc  lqc2        $vf26, 0x20($a0)
            vf[26] = _mm_castsi128_ps(m.q128(ADD32(r[4], 32)));
            // 0x113bbd0  lqc2        $vf27, 0x30($a0)
            vf[27] = _mm_castsi128_ps(m.q128(ADD32(r[4], 48)));
            // 0x113bbd4  vmulax.xyzw $ACC, $vf14, $vf24x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbd8  vmadday.xyzw $ACC, $vf15, $vf24y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbdc  vmaddaz.xyzw $ACC, $vf16, $vf24z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbe0  vmaddw.xyzw $vf24, $vf17, $vf24w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[24] = _mm_blendv_ps(vf[24], res, _mm_castsi128_ps(mask)); }
            // 0x113bbe4  vmulax.xyzw $ACC, $vf14, $vf25x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbe8  vmadday.xyzw $ACC, $vf15, $vf25y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbec  vmaddaz.xyzw $ACC, $vf16, $vf25z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbf0  vmaddw.xyzw $vf25, $vf17, $vf25w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[25] = _mm_blendv_ps(vf[25], res, _mm_castsi128_ps(mask)); }
            // 0x113bbf4  vmulax.xyzw $ACC, $vf14, $vf26x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbf8  vmadday.xyzw $ACC, $vf15, $vf26y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bbfc  vmaddaz.xyzw $ACC, $vf16, $vf26z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc00  vmaddw.xyzw $vf26, $vf17, $vf26w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[26] = _mm_blendv_ps(vf[26], res, _mm_castsi128_ps(mask)); }
            // 0x113bc04  vmulax.xyzw $ACC, $vf14, $vf27x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc08  vmadday.xyzw $ACC, $vf15, $vf27y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc0c  vmaddaz.xyzw $ACC, $vf16, $vf27z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc10  vmaddw.xyzw $vf27, $vf17, $vf27w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[27] = _mm_blendv_ps(vf[27], res, _mm_castsi128_ps(mask)); }
            // 0x113bc14  sqc2        $vf24, 0x0($v0)
            m.put128(ADD32(r[2], 0), _mm_castps_si128(vf[24]));
            // 0x113bc18  sqc2        $vf25, 0x10($v0)
            m.put128(ADD32(r[2], 16), _mm_castps_si128(vf[25]));
            // 0x113bc1c  sqc2        $vf26, 0x20($v0)
            m.put128(ADD32(r[2], 32), _mm_castps_si128(vf[26]));
            // 0x113bc20  sqc2        $vf27, 0x30($v0)
            m.put128(ADD32(r[2], 48), _mm_castps_si128(vf[27]));
            // 0x113bc24  lqc2        $vf14, 0x0($a0)
            vf[14] = _mm_castsi128_ps(m.q128(ADD32(r[4], 0)));
            // 0x113bc28  lqc2        $vf15, 0x10($a0)
            vf[15] = _mm_castsi128_ps(m.q128(ADD32(r[4], 16)));
            // 0x113bc2c  lqc2        $vf16, 0x20($a0)
            vf[16] = _mm_castsi128_ps(m.q128(ADD32(r[4], 32)));
            // 0x113bc30  lqc2        $vf17, 0x30($a0)
            vf[17] = _mm_castsi128_ps(m.q128(ADD32(r[4], 48)));
            // 0x113bc34  lqc2        $vf24, 0x0($v0)
            vf[24] = _mm_castsi128_ps(m.q128(ADD32(r[2], 0)));
            // 0x113bc38  lqc2        $vf25, 0x10($v0)
            vf[25] = _mm_castsi128_ps(m.q128(ADD32(r[2], 16)));
            // 0x113bc3c  lqc2        $vf26, 0x20($v0)
            vf[26] = _mm_castsi128_ps(m.q128(ADD32(r[2], 32)));
            // 0x113bc40  lqc2        $vf27, 0x30($v0)
            vf[27] = _mm_castsi128_ps(m.q128(ADD32(r[2], 48)));
            // 0x113bc44  vmulax.xyzw $ACC, $vf14, $vf24x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc48  vmadday.xyzw $ACC, $vf15, $vf24y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc4c  vmaddaz.xyzw $ACC, $vf16, $vf24z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc50  vmaddw.xyzw $vf24, $vf17, $vf24w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[24] = _mm_blendv_ps(vf[24], res, _mm_castsi128_ps(mask)); }
            // 0x113bc54  vmulax.xyzw $ACC, $vf14, $vf25x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc58  vmadday.xyzw $ACC, $vf15, $vf25y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc5c  vmaddaz.xyzw $ACC, $vf16, $vf25z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc60  vmaddw.xyzw $vf25, $vf17, $vf25w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[25] = _mm_blendv_ps(vf[25], res, _mm_castsi128_ps(mask)); }
            // 0x113bc64  vmulax.xyzw $ACC, $vf14, $vf26x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc68  vmadday.xyzw $ACC, $vf15, $vf26y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc6c  vmaddaz.xyzw $ACC, $vf16, $vf26z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc70  vmaddw.xyzw $vf26, $vf17, $vf26w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[26] = _mm_blendv_ps(vf[26], res, _mm_castsi128_ps(mask)); }
            // 0x113bc74  vmulax.xyzw $ACC, $vf14, $vf27x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc78  vmadday.xyzw $ACC, $vf15, $vf27y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc7c  vmaddaz.xyzw $ACC, $vf16, $vf27z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bc80  vmaddw.xyzw $vf27, $vf17, $vf27w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[27] = _mm_blendv_ps(vf[27], res, _mm_castsi128_ps(mask)); }
            // 0x113bc84  sqc2        $vf24, 0x0($v0)
            m.put128(ADD32(r[2], 0), _mm_castps_si128(vf[24]));
            // 0x113bc88  sqc2        $vf25, 0x10($v0)
            m.put128(ADD32(r[2], 16), _mm_castps_si128(vf[25]));
            // 0x113bc8c  sqc2        $vf26, 0x20($v0)
            m.put128(ADD32(r[2], 32), _mm_castps_si128(vf[26]));
            // 0x113bc90  sqc2        $vf27, 0x30($v0)
            m.put128(ADD32(r[2], 48), _mm_castps_si128(vf[27]));
            // 0x113bc94  lqc2        $vf14, 0x0($a1)
            vf[14] = _mm_castsi128_ps(m.q128(ADD32(r[5], 0)));
            // 0x113bc98  lqc2        $vf15, 0x10($a1)
            vf[15] = _mm_castsi128_ps(m.q128(ADD32(r[5], 16)));
            // 0x113bc9c  lqc2        $vf16, 0x20($a1)
            vf[16] = _mm_castsi128_ps(m.q128(ADD32(r[5], 32)));
            // 0x113bca0  lqc2        $vf17, 0x30($a1)
            vf[17] = _mm_castsi128_ps(m.q128(ADD32(r[5], 48)));
            // 0x113bca4  lqc2        $vf24, 0x0($v0)
            vf[24] = _mm_castsi128_ps(m.q128(ADD32(r[2], 0)));
            // 0x113bca8  lqc2        $vf25, 0x10($v0)
            vf[25] = _mm_castsi128_ps(m.q128(ADD32(r[2], 16)));
            // 0x113bcac  lqc2        $vf26, 0x20($v0)
            vf[26] = _mm_castsi128_ps(m.q128(ADD32(r[2], 32)));
            // 0x113bcb0  lqc2        $vf27, 0x30($v0)
            vf[27] = _mm_castsi128_ps(m.q128(ADD32(r[2], 48)));
            // 0x113bcb4  vmulax.xyzw $ACC, $vf14, $vf24x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcb8  vmadday.xyzw $ACC, $vf15, $vf24y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcbc  vmaddaz.xyzw $ACC, $vf16, $vf24z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcc0  vmaddw.xyzw $vf24, $vf17, $vf24w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[24], vf[24], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[24] = _mm_blendv_ps(vf[24], res, _mm_castsi128_ps(mask)); }
            // 0x113bcc4  vmulax.xyzw $ACC, $vf14, $vf25x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcc8  vmadday.xyzw $ACC, $vf15, $vf25y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bccc  vmaddaz.xyzw $ACC, $vf16, $vf25z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcd0  vmaddw.xyzw $vf25, $vf17, $vf25w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[25] = _mm_blendv_ps(vf[25], res, _mm_castsi128_ps(mask)); }
            // 0x113bcd4  vmulax.xyzw $ACC, $vf14, $vf26x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcd8  vmadday.xyzw $ACC, $vf15, $vf26y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcdc  vmaddaz.xyzw $ACC, $vf16, $vf26z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bce0  vmaddw.xyzw $vf26, $vf17, $vf26w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[26], vf[26], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[26] = _mm_blendv_ps(vf[26], res, _mm_castsi128_ps(mask)); }
            // 0x113bce4  vmulax.xyzw $ACC, $vf14, $vf27x
            { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bce8  vmadday.xyzw $ACC, $vf15, $vf27y
            { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcec  vmaddaz.xyzw $ACC, $vf16, $vf27z
            { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // 0x113bcf0  vmaddw.xyzw $vf27, $vf17, $vf27w
            { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[27], vf[27], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[27] = _mm_blendv_ps(vf[27], res, _mm_castsi128_ps(mask)); }
            // 0x113bcf4  sqc2        $vf24, 0x0($v0)
            m.put128(ADD32(r[2], 0), _mm_castps_si128(vf[24]));
            // 0x113bcf8  sqc2        $vf25, 0x10($v0)
            m.put128(ADD32(r[2], 16), _mm_castps_si128(vf[25]));
            // 0x113bcfc  sqc2        $vf26, 0x20($v0)
            m.put128(ADD32(r[2], 32), _mm_castps_si128(vf[26]));
            // 0x113bd00  sqc2        $vf27, 0x30($v0)
            m.put128(ADD32(r[2], 48), _mm_castps_si128(vf[27]));
        }
        f0 = m.f32(P + 0x4Cu);
        // The wind at E+0x10 (GetWindVector) unless P+0x4c and P+0x154 are 0: the zero vector.
        f1 = 0.0f;
        uint32_t wind;
        bool calm = false;
        if (FPU_C_EQ_S(f0, f1))
        {
            f0 = m.f32(P + 0x154u);
            calm = FPU_C_EQ_S(f0, f1);
        }
        if (calm)
            wind = sotc_decomp::relocated(rdram, 0x0113BD38u, 0x0113BD40u);
        else
        {
            g.a(4, 0u);
            g.a(5, E + 0x10u);
            regs();
            ctx->f[0] = f0;
            ctx->f[1] = f1;
            g.call(0x01146A50u, 0x0113BD48u, sp);
            f0 = ctx->f[0];
            f1 = ctx->f[1];
            wind = GPR_U32(ctx, 2);
        }
        m.put128(E, m.q128(wind));
        g.a(4, E);
        g.a(5, K);
        regs();
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        g.call(0x01141400u, 0x0113BD5Cu, sp);
        f0 = ctx->f[0];

        // Each live particle's step (+0x40 not 0; next at +0xa8).
        // count: loop 1, s3 counts the particles whose step returned 0.
        auto particles = [&](uint32_t site, bool count)
        {
            const int32_t n = m.s32(E + 0x124u);
            uint32_t a0 = m.u32(E + 0x128u);
            if (n <= 0)
                return;
            const float zero = 0.0f;
            (count ? f20 : f21) = zero;
            s0 = static_cast<uint64_t>(static_cast<int64_t>(n));
            for (int32_t left = n; left != 0; --left)
            {
                f0 = m.f32(a0 + 0x40u);
                s2 = static_cast<uint64_t>(static_cast<int64_t>(m.s32(a0 + 0xA8u)));
                if (!FPU_C_EQ_S(f0, zero))
                {
                    g.a(4, a0);
                    g.a(5, E);
                    g.a(6, Mr);
                    g.a(7, K);
                    g.a(8, P);
                    g.a(9, sp + 0x10u);
                    regs();
                    ctx->f[0] = f0;
                    g.callNative(&decomp_particles::particleUpdate, kUpdate, site, sp);
                    f0 = ctx->f[0];
                    if (count && g.v0() == 0u)
                        ++s3;
                }
                s0 = static_cast<uint64_t>(static_cast<int64_t>(left - 1));
                a0 = static_cast<uint32_t>(s2);
            }
        };
        // A free particle (the free list at 0x1216df0: count, head, tail) to
        // the emitter's list, then 0113C1A8 makes it (a1 sp, a2 E, a3 Mr, t0 K, t1 P).
        auto spawn = [&](uint32_t site)
        {
            const uint32_t fl = 0x01216DF0u, list = E + 0x124u;
            uint32_t a0 = m.u32(fl + 4u);
            if (a0 != 0u)
            {
                // The first free one is the head: unlinked from the free list.
                const uint32_t a1 = 0u;
                uint32_t a2 = a0;
                (void)a2;
                if (m.u32(fl + 4u) == a0)
                    m.put32(fl + 4u, m.u32(a0 + 0xA8u));
                if (m.u32(fl + 8u) == a0)
                    m.put32(fl + 8u, a1);
                m.put32(fl, m.u32(fl) - 1u);
                // Linked at the emitter's tail.
                const uint32_t tail = m.u32(list + 8u);
                if (tail == 0u)
                    m.put32(list + 4u, a0);
                else
                    m.put32(tail + 0xA8u, a0);
                m.put32(a0 + 0xA8u, 0u);
                m.put32(list + 8u, a0);
                m.put32(list, m.u32(list) + 1u);
            }
            if (a0 != 0u)
            {
                g.a(4, a0);
                g.a(5, sp);
                g.a(6, E);
                g.a(7, Mr);
                g.a(8, K);
                g.a(9, P);
                regs();
                ctx->f[0] = f0;
                g.call(0x0113C1A8u, site, sp);
                f0 = ctx->f[0];
            }
        };

        // With sacParticles at most half the emitter's live particles (rounded up).
        const bool sacParticles = native_renderer::tuning().sacParticles;
        auto liveCap = [&]() -> uint32_t
        {
            const uint32_t cap = m.u32(Mr + 0x1Cu);
            return sacParticles && static_cast<int32_t>(cap) > 1 ? (cap + 1u) / 2u : cap;
        };
        uint64_t v0 = 0u;
        if (m.u32(E + 0x114u) == 1u)
        {
            // Done generating: the particles' steps; finished when none failed.
            s3 = 0u;
            particles(0x0113BDB0u, true);
            v0 = s3 != 0u ? 0u : 1u;
            st.done += st.on && v0 == 1u;
        }
        else
        {
            f20 = 0.0f; // how many new particles this frame (with the carried fraction)
            if (s3 != 1u)
                m.putf(E + 0x11Cu, f20);
            else
            {
                regs();
                ctx->f[0] = f0;
                g.callRel(0x0113BDECu, sp); // iosGetRandom -> f0
                f0 = ctx->f[0];
                float f1r = m.f32(P + 0x14u), f2 = m.f32(P + 0x10u);
                const uint32_t stepPtr = m.u32(0x0118CFE8u);
                f1r = FPU_MUL_S(f1r, f0);
                const float f3 = m.f32(E + 0x11Cu);
                f0 = m.f32(stepPtr);
                const float f4 = 1.0f;
                f2 = FPU_ADD_S(f2, f1r);
                f20 = FPU_MUL_S(f2, f0);
                // Round 36, "Partículas reduzidas" (Tuning::sacParticles):
                // half the emission rate (the carried fraction keeps the
                // average exact; an emitter's first frame still makes one).
                if (sacParticles)
                    f20 = FPU_MUL_S(f20, 0.5f);
                f0 = FPU_CVT_S_W(FPU_CVT_W_S(f20));
                f0 = FPU_SUB_S(f20, f0);
                f0 = FPU_ADD_S(f3, f0);
                m.putf(E + 0x11Cu, f0);
                if (FPU_C_LE_S(f4, f0))
                {
                    f0 = FPU_SUB_S(f0, f4);
                    f20 = FPU_ADD_S(f20, f4);
                    m.putf(E + 0x11Cu, f0);
                }
                if (m.u32(E + 0x190u) != static_cast<uint32_t>(s3))
                    m.put32(E + 0x190u, 0u);
                else if (!FPU_C_LT_S(f20, f4))
                    m.put32(E + 0x190u, 0u);
                else
                {
                    m.put32(E + 0x11Cu, 0u);
                    f20 = f4;
                    m.put32(E + 0x190u, 0u);
                }
                f1 = f1r;
            }
            if (m.u32(K + 0x10u) == 0u)
            {
                particles(0x0113BEB0u, false);
                if (s3 == 1u)
                {
                    const int32_t want = static_cast<int32_t>(liveCap() - m.u32(E + 0x124u));
                    s2 = static_cast<uint64_t>(static_cast<int64_t>(want));
                    s0 = 0u;
                    if (want > 0)
                    {
                        f21 = 1.0f;
                        s3 = 0x01210000u;
                        for (int32_t i = 0; i < want; ++i)
                        {
                            if (!FPU_C_LE_S(f21, f20))
                                break;
                            spawn(0x0113BFC8u);
                            st.spawned += st.on;
                            m.put32(E + 0x120u, m.u32(E + 0x120u) + 1u);
                            s0 = static_cast<uint64_t>(i + 1);
                            f20 = FPU_SUB_S(f20, f21);
                        }
                    }
                }
                if (m.s32(E + 0x120u) < m.s32(P + 8u))
                    v0 = 0u;
                else
                {
                    m.put32(E + 0xC4u, 0u);
                    m.put32(E + 0x114u, 1u);
                    v0 = 0u;
                }
            }
            else
            {
                particles(0x0113C040u, false);
                if (s3 == 1u)
                {
                    const int32_t want = static_cast<int32_t>(liveCap() - m.u32(E + 0x124u));
                    s2 = static_cast<uint64_t>(static_cast<int64_t>(want));
                    s0 = 0u;
                    if (want > 0)
                    {
                        f21 = 1.0f;
                        s3 = 0x01210000u;
                        for (int32_t i = 0; i < want; ++i)
                        {
                            if (!FPU_C_LE_S(f21, f20))
                                break;
                            spawn(0x0113C158u);
                            s0 = static_cast<uint64_t>(i + 1);
                            f20 = FPU_SUB_S(f20, f21);
                        }
                    }
                }
                v0 = 0u;
            }
        }
        // The epilogue.
        std::memcpy(ctx->vu0_vf, vf, sizeof(vf));
        ctx->vu0_acc = acc;
        ctx->f[0] = f0;
        ctx->f[20] = f20e;
        ctx->f[21] = f21e;
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, e16);
        SET_GPR_U64(ctx, 17, e17);
        SET_GPR_U64(ctx, 18, e18);
        SET_GPR_U64(ctx, 19, e19);
        SET_GPR_U64(ctx, 20, e20);
        SET_GPR_U64(ctx, 21, e21);
        SET_GPR_U64(ctx, 22, e22);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }

    // ---- 0113B610: an emitter's frame. a0 the emitter E (0 -> 0), a1 its
    // effect record, a2 the effect's parameters. v0: 0113B928's.
    void emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Mem &m = g.m;
        const uint32_t E = GPR_U32(ctx, 4), Mr = GPR_U32(ctx, 5), A2 = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x1A0u;
        if ((sp & 0xFu) != 0u || !inRam(sp, 0x1A0u) || (E != 0u && !inRam(E, 0x200u)) || !inRam(Mr, 0x100u) || (E != 0u && !inRam(m.u32(Mr + 0xD8u), 0x30u)) ||
            (E != 0u && !inRam(m.u32(Mr + 0xD4u), 4u)))
            return sotc_decomp::recompiled(kEmitter)(rdram, ctx, runtime);
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20);
        m.put64(sp + 0x180u, e18);
        m.put64(sp + 0x188u, e19);
        m.put64(sp + 0x190u, e20);
        m.put64(sp + 0x170u, e16);
        m.put64(sp + 0x178u, e17);
        m.put64(sp + 0x198u, ra0);
        SET_GPR_U32(ctx, 29, sp);
        g.a(18, E);
        g.a(19, Mr);
        g.a(20, A2);
        auto epilogue = [&](uint64_t v0)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 16, e16);
            SET_GPR_U64(ctx, 17, e17);
            SET_GPR_U64(ctx, 18, e18);
            SET_GPR_U64(ctx, 19, e19);
            SET_GPR_U64(ctx, 20, e20);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            sotc_decomp::returnTo(ctx);
        };
        if (E == 0u)
            return epilogue(0u);
        uint64_t s0 = e16, s1 = 1u;
        auto regs = [&]
        {
            g.a64(16, s0);
            g.a64(17, s1);
        };
        // The frame step by the effect's name: the two named ones 0x1216e28,
        // else 01141090 says 0x1216e18 or 0x1216e08.
        uint32_t stepPtr;
        g.a(4, m.u32(m.u32(Mr + 0xD8u) + 0x24u));
        g.a(5, 0x011958C0u);
        regs();
        g.callRel(0x0113B64Cu, sp);
        bool named = g.v0() == 0u;
        if (!named)
        {
            g.a(4, m.u32(m.u32(Mr + 0xD8u) + 0x24u));
            g.a(5, 0x011958D0u);
            g.callRel(0x0113B664u, sp);
            named = g.v0() == 0u;
        }
        if (named)
            stepPtr = 0x01216E28u;
        else
        {
            g.a(4, Mr);
            g.call(0x01141090u, 0x0113B688u, sp);
            stepPtr = g.v0() == s1 ? 0x01216E18u : 0x01216E08u;
        }
        m.put32(0x0118CFE8u, stepPtr);
        m.put32(E + 0x198u, 0u);
        g.a(4, E);
        g.a(5, Mr);
        g.a(6, 0u);
        g.call(0x0113AEF8u, 0x0113B6C0u, sp);
        s0 = E + 0x10u;
        regs();
        // The camera context's position (+0x140) and axis (+0x150): the
        // recompiled statements on locals (the VU0 registers, v0 / v1 / s0 / s1).
        __m128 vf[32];
        std::memcpy(vf, ctx->vu0_vf, sizeof(vf));
        __m128 acc = ctx->vu0_acc;
        uint32_t r[32] = {};
        __m128i gq[32];
        __m128i *g_ = gq;
        (void)g_;
        r[16] = static_cast<uint32_t>(s0);
        r[18] = E;
        r[29] = sp;
        auto camera = [&](uint32_t site)
        {
            g.a64(16, s0);
            g.a64(17, s1);
            g.call(0x0108F518u, site, sp);
            r[2] = GPR_U32(ctx, 2);
        };
        camera(0x0113B6C8u);
        {
            // 0x113b6d0  addiu       $v1, $v0, 0x140
            r[3] = (uint32_t)((int32_t)ADD32(r[2], 320));
            // 0x113b6d4  lqc2        $vf5, 0x0($s0)
            vf[5] = _mm_castsi128_ps(m.q128(ADD32(r[16], 0)));
            // 0x113b6d8  lqc2        $vf6, 0x0($v1)
            vf[6] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113b6dc  vsub.xyzw   $vf7, $vf5, $vf6
            { __m128 res = PS2_VSUB(vf[5], vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b6e0  vmul.xyz    $vf7, $vf7, $vf7
            { __m128 res = PS2_VMUL(vf[7], vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b6e4  vmulax.w    $ACC, $vf0, $vf7x
            { __m128 res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            // 0x113b6e8  vmadday.w   $ACC, $vf0, $vf7y
            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            // 0x113b6ec  vmaddz.w    $vf7, $vf0, $vf7z
            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b6f0  .word       0x4BA703BD                   # vsqrt       $Q, $vf7w # 00200000 <InstrIdType: R5900_COP2_SPECIAL2>
            { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
            // 0x113b6f4  vwaitq
            // VWAITQ (Q already resolved in this runtime)
            // 0x113b6f8  cfc2.ni     $v0, $vi22
            { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); r[2] = (uint32_t)(bits); }
            // 0x113b6fc  mtc1        $v0, $f0
            { uint32_t bits = r[2]; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
        }
        m.put32(E + 0x130u, r[2]); // swc1 $f0 (the jal's delay slot)
        camera(0x0113B700u);
        {
            // 0x113b708  addiu       $v0, $v0, 0x140
            r[2] = (uint32_t)((int32_t)ADD32(r[2], 320));
            // 0x113b70c  addiu       $s1, $sp, 0x160
            r[17] = (uint32_t)((int32_t)ADD32(r[29], 352));
            // 0x113b710  lqc2        $vf5, 0x0($s0)
            vf[5] = _mm_castsi128_ps(m.q128(ADD32(r[16], 0)));
            // 0x113b714  lqc2        $vf6, 0x0($v0)
            vf[6] = _mm_castsi128_ps(m.q128(ADD32(r[2], 0)));
            // 0x113b718  vsub.xyz    $vf5, $vf5, $vf6
            { __m128 res = PS2_VSUB(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            // 0x113b71c  sqc2        $vf5, 0x0($s1)
            m.put128(ADD32(r[17], 0), _mm_castps_si128(vf[5]));
        }
        s1 = r[17];
        camera(0x0113B720u);
        {
            // 0x113b728  addiu       $v1, $v0, 0x150
            r[3] = (uint32_t)((int32_t)ADD32(r[2], 336));
            // 0x113b72c  lqc2        $vf5, 0x0($s1)
            vf[5] = _mm_castsi128_ps(m.q128(ADD32(r[17], 0)));
            // 0x113b730  lqc2        $vf6, 0x0($v1)
            vf[6] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113b734  vaddw.x     $vf7, $vf0, $vf0w
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b738  vmul.xyz    $vf6, $vf5, $vf6
            { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b73c  vaddax.x    $ACC, $vf0, $vf6x
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b740  vmadday.x   $ACC, $vf7, $vf6y
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b744  vmaddz.x    $vf6, $vf7, $vf6z
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b748  qmfc2.ni    $v0, $vf6
            gq[2] = _mm_castps_si128(vf[6]); r[2] = (uint32_t)_mm_cvtsi128_si32(gq[2]);
            // 0x113b74c  mtc1        $v0, $f1
            { uint32_t bits = r[2]; std::memcpy(&ctx->f[1], &bits, sizeof(bits)); }
            // 0x113b750  mtc1        $zero, $f0
            { uint32_t bits = r[0]; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
            // 0x113b754  c.lt.s      $f1, $f0
            ctx->fcr31 = (FPU_C_OLT_S(ctx->f[1], ctx->f[0])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
        }
        float f0;
        if (FPU_C_OLT_S(ctx->f[1], ctx->f[0]))
        {
            camera(0x0113B760u);
            {
            // 0x113b768  addiu       $v1, $v0, 0x150
            r[3] = (uint32_t)((int32_t)ADD32(r[2], 336));
            // 0x113b76c  lqc2        $vf5, 0x0($s1)
            vf[5] = _mm_castsi128_ps(m.q128(ADD32(r[17], 0)));
            // 0x113b770  lqc2        $vf6, 0x0($v1)
            vf[6] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113b774  vaddw.x     $vf7, $vf0, $vf0w
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b778  vmul.xyz    $vf6, $vf5, $vf6
            { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b77c  vaddax.x    $ACC, $vf0, $vf6x
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b780  vmadday.x   $ACC, $vf7, $vf6y
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b784  vmaddz.x    $vf6, $vf7, $vf6z
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b788  qmfc2.ni    $v0, $vf6
            gq[2] = _mm_castps_si128(vf[6]); r[2] = (uint32_t)_mm_cvtsi128_si32(gq[2]);
            // 0x113b78c  mtc1        $v0, $f0
            { uint32_t bits = r[2]; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
            }
            f0 = FPU_NEG_S(ctx->f[0]);
        }
        else
        {
            camera(0x0113B798u);
            {
            // 0x113b7a0  addiu       $v1, $v0, 0x150
            r[3] = (uint32_t)((int32_t)ADD32(r[2], 336));
            // 0x113b7a4  lqc2        $vf5, 0x0($s1)
            vf[5] = _mm_castsi128_ps(m.q128(ADD32(r[17], 0)));
            // 0x113b7a8  lqc2        $vf6, 0x0($v1)
            vf[6] = _mm_castsi128_ps(m.q128(ADD32(r[3], 0)));
            // 0x113b7ac  vaddw.x     $vf7, $vf0, $vf0w
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            // 0x113b7b0  vmul.xyz    $vf6, $vf5, $vf6
            { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b7b4  vaddax.x    $ACC, $vf0, $vf6x
            { __m128 res = PS2_VADD(vf[0], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b7b8  vmadday.x   $ACC, $vf7, $vf6y
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            // 0x113b7bc  vmaddz.x    $vf6, $vf7, $vf6z
            { __m128 mul_res = PS2_VMUL(vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            // 0x113b7c0  qmfc2.ni    $v0, $vf6
            gq[2] = _mm_castps_si128(vf[6]); r[2] = (uint32_t)_mm_cvtsi128_si32(gq[2]);
            // 0x113b7c4  mtc1        $v0, $f0
            { uint32_t bits = r[2]; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
            }
            f0 = ctx->f[0];
        }
        m.putf(E + 0x134u, f0);
        std::memcpy(ctx->vu0_vf, vf, sizeof(vf));
        ctx->vu0_acc = acc;
        ctx->f[0] = f0;
        g.a(4, E);
        g.a(5, Mr);
        g.call(0x011410D0u, 0x0113B7D0u, sp);
        g.a(4, E);
        g.a(5, Mr);
        g.call(0x01141150u, 0x0113B7DCu, sp);
        // The effect's LOD record (+0xd4 + 0x1bc x E+0x118): the near / far planes.
        s0 = m.u32(Mr + 0xD4u) + m.u32(E + 0x118u) * 0x1BCu;
        const uint32_t R = static_cast<uint32_t>(s0);
        f0 = ctx->f[0];
        float fl1 = ctx->f[1];
        if (m.u32(Mr + 0xCCu) == 1u)
        {
            auto plane = [&](uint32_t modeAt, uint32_t valueAt, uint32_t outAt, uint32_t none)
            {
                const uint32_t mode = m.u32(R + modeAt);
                if (mode == 1u)
                    f0 = m.f32(R + valueAt);
                else if (mode == 0u)
                    f0 = bits(none);
                else
                {
                    f0 = m.f32(R + valueAt);
                    fl1 = mode == 2u ? m.f32(E + 0x14u) : m.f32(E + 0x14Cu);
                    f0 = FPU_ADD_S(f0, fl1);
                }
                m.putf(E + outAt, f0);
            };
            plane(0x15Cu, 0xB4u, 0x150u, 0x4B189680u);
            plane(0x160u, 0x140u, 0x154u, 0xCB189680u);
        }
        ctx->f[0] = f0;
        ctx->f[1] = fl1;
        s1 = m.u32(E + 0x168u) != 0u ? 1u : 0u;
        regs();
        g.a(4, E);
        g.a(5, Mr);
        g.a(6, R);
        g.call(0x01141230u, 0x0113B8BCu, sp);
        g.a(7, m.u32(E + 0x118u));
        g.a(4, sp);
        ctx->f[12] = m.f32(E + 0x16Cu);
        g.a(5, Mr);
        g.a(6, A2);
        g.call(0x01144F68u, 0x0113B8D4u, sp);
        g.a(4, E);
        g.a(5, Mr);
        g.a(6, R);
        g.a(7, sp);
        g.a64(8, s1);
        g.callNative(&step, kStep, 0x0113B8ECu, sp);
        epilogue(g.v0());
    }

    // ---- 0113AAE0: a geodisp's emitter list (a0 the list's head pointer;
    // node +0 the emitter's record, +0x10 the emitter, next at +0x1b0).
    // An emitter whose step returns 1 is unlinked, its callback run
    // (+0x158 == 1: +0x15c(+0x160)), its particles given back to the free
    // list, its collision finder released, and freed.
    void list(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const Mem &m = g.m;
        const uint32_t head = GPR_U32(ctx, 4);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x40u;
        if ((sp & 0xFu) != 0u || !inRam(sp, 0x40u) || !inRam(head, 4u))
            return sotc_decomp::recompiled(kList)(rdram, ctx, runtime);
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20);
        m.put64(sp + 0x20u, e18);
        m.put64(sp + 0x28u, e19);
        m.put64(sp + 0x10u, e16);
        m.put64(sp + 0x18u, e17);
        m.put64(sp + 0x30u, e20);
        m.put64(sp + 0x38u, ra0);
        SET_GPR_U32(ctx, 29, sp);
        uint64_t s0 = e16, s1 = e17, s3 = 0u, s4 = e20;
        g.a(18, head);
        auto regs = [&]
        {
            g.a64(16, s0);
            g.a64(17, s1);
            g.a64(19, s3);
            g.a64(20, s4);
        };
        uint32_t node = m.u32(head);
        m.put32(sp, node);
        if (Stats &st = stats(); st.on && ++st.lists % 600u == 0u)
            std::fprintf(stderr, "[ptcl] lists %llu: steps %llu (matrix %llu, unit %llu), done %llu, spawned %llu, dead emitters %llu, fallbacks %llu\n",
                         (unsigned long long)st.lists, (unsigned long long)st.steps, (unsigned long long)st.matrix, (unsigned long long)st.unit,
                         (unsigned long long)st.done, (unsigned long long)st.spawned, (unsigned long long)st.dead, (unsigned long long)st.fallbacks);
        if (node != 0u)
        {
            s4 = 1u;
            for (;;)
            {
                if (!inRam(node, 0x200u) || !inRam(m.u32(node), 0x50u))
                {
                    // Not plain RAM: the recompiled loop from its head (0x113ab18), as the registers are.
                    regs();
                    g.a(4, node);
                    ctx->pc = 0x0113AB18u;
                    runToReturn(rdram, ctx, runtime, sotc_decomp::recompiled(kList), ra0, sp0);
                    return;
                }
                const uint32_t rec = m.u32(node);
                g.a(4, node + 0x10u);
                g.a(6, m.u32(rec + 0x4Cu));
                g.a(5, m.u32(rec + 0x48u));
                regs();
                g.callNative(&emitter, kEmitter, 0x0113AB24u, sp);
                const uint64_t r = g.v0();
                uint32_t v0 = m.u32(sp);
                if (r == s4)
                {
                    stats().dead += stats().on;
                    // The emitter is done: unlinked, its particles freed, then the emitter.
                    s1 = sp;
                    if (s3 == 0u)
                        m.put32(head, m.u32(v0 + 0x1B0u));
                    else
                        m.put32(static_cast<uint32_t>(s3) + 0x1B0u, m.u32(m.u32(sp) + 0x1B0u));
                    const uint32_t a0n = m.u32(sp);
                    s0 = a0n + 0x10u;
                    if (s0 != 0u)
                    {
                        const uint32_t e = static_cast<uint32_t>(s0);
                        const uint32_t cb = m.u32(e + 0x158u);
                        m.put32(e + 0x114u, static_cast<uint32_t>(s4));
                        if (cb == s4 && m.u32(e + 0x15Cu) != 0u)
                        {
                            g.a(5, e + 0x124u);
                            g.a(4, m.u32(e + 0x160u));
                            regs();
                            g.call(m.u32(e + 0x15Cu), 0x0113AB80u, sp);
                        }
                        const uint32_t a1 = e + 0x124u;
                        uint32_t v1 = m.u32(a1 + 4u);
                        if (v1 == 0u)
                            s0 = e + 0x170u;
                        else
                        {
                            // Each particle (from the list's head) back to the free list's tail.
                            do
                            {
                                const uint32_t a0 = v1;
                                if (v1 != 0u)
                                {
                                    // Found at once (the head): unlinked from the emitter's list.
                                    uint32_t a2 = 0u;
                                    for (uint32_t v = v1;;)
                                    {
                                        if (v == v1)
                                        {
                                            if (a2 != 0u)
                                            {
                                                m.put32(a2 + 0xA8u, m.u32(v1 + 0xA8u));
                                                v1 = m.u32(a1 + 4u);
                                            }
                                            if (v1 == a0)
                                                m.put32(a1 + 4u, m.u32(v1 + 0xA8u));
                                            if (m.u32(a1 + 8u) == a0)
                                                m.put32(a1 + 8u, a2);
                                            m.put32(a1, m.u32(a1) - 1u);
                                            break;
                                        }
                                        a2 = v;
                                        v = m.u32(v + 0xA8u);
                                        if (v == 0u)
                                            break;
                                    }
                                }
                                if (a0 != 0u)
                                {
                                    const uint32_t fl = 0x01216DF0u;
                                    const uint32_t tail = m.u32(fl + 8u);
                                    if (tail == 0u)
                                        m.put32(fl + 4u, a0);
                                    else
                                        m.put32(tail + 0xA8u, a0);
                                    m.put32(a0 + 0xA8u, 0u);
                                    m.put32(fl + 8u, a0);
                                    m.put32(fl, m.u32(fl) + 1u);
                                }
                                v1 = m.u32(a1 + 4u);
                            } while (v1 != 0u);
                            s0 = e + 0x170u;
                        }
                        g.a(4, static_cast<uint32_t>(s0));
                        regs();
                        g.call(0x0112A450u, 0x0113AC40u, sp); // CheckCollisionFinderAlive
                        if (g.v0() != 0u)
                        {
                            g.a(4, static_cast<uint32_t>(s0));
                            g.call(0x0112A380u, 0x0113AC50u, sp); // ReleaseCollisionFinder
                        }
                    }
                    g.a(4, m.u32(static_cast<uint32_t>(s1)));
                    regs();
                    g.callRel(0x0113AC5Cu, sp); // iosFree
                    m.put32(static_cast<uint32_t>(s1), static_cast<uint32_t>(s3));
                    v0 = m.u32(sp);
                }
                if (v0 != 0u)
                {
                    s3 = v0;
                    node = m.u32(static_cast<uint32_t>(s3) + 0x1B0u);
                }
                else
                {
                    node = m.u32(head);
                    s3 = 0u;
                }
                m.put32(sp, node);
                if (node == 0u)
                    break;
            }
            SET_GPR_U64(ctx, 2, 0u); // lw $v0, 0($sp): the list's end
        }
        SET_GPR_U64(ctx, 16, e16);
        SET_GPR_U64(ctx, 17, e17);
        SET_GPR_U64(ctx, 18, e18);
        SET_GPR_U64(ctx, 19, e19);
        SET_GPR_U64(ctx, 20, e20);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }

    // ---- doit_particle_geodisp (0x113f500): a0 the geodisp (0: none) ->
    // 0113AAE0 on its class's emitter list (GetGeoDispClass + 0xe8), a tail jump.
    void geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        G g(rdram, ctx, runtime);
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x10u;
        if ((sp & 0xFu) != 0u || !inRam(sp, 0x10u))
            return sotc_decomp::recompiled(kGeodisp)(rdram, ctx, runtime);
        g.m.put64(sp, ra0);
        uint32_t list = 0u;
        if (a0 != 0u)
        {
            SET_GPR_U32(ctx, 29, sp);
            g.call(0x010ED688u, 0x0113F50Cu, sp); // GetGeoDispClass
            list = GPR_U32(ctx, 2) + 0xE8u;
        }
        // ld ra; j 0113AAE0 with sp back.
        g.a(4, list);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        ctx->pc = kList;
        if (runtime->guestFunctionAt(kList) == &decomp_ptclupdate::list)
            decomp_ptclupdate::list(rdram, ctx, runtime);
        else
            runtime->guestFunctionAt(kList)(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x0113F500u, ptclGeodisp, decomp_ptclupdate::geodisp);
SOTC_DECOMP_FUNCTION(0x0113AAE0u, ptclEmitterList, decomp_ptclupdate::list);
SOTC_DECOMP_FUNCTION(0x0113B610u, ptclEmitter, decomp_ptclupdate::emitter);
SOTC_DECOMP_FUNCTION(0x0113B928u, ptclParticles, decomp_ptclupdate::step);
