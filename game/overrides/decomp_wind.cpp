// manager_sub_01145F78 (round 6): the wind zones that act on a point (a0),
// for GetWindVector and the wind manager, ~6 calls a frame over the 80
// zone slots at 0x1216ef0.
//
// A zone (non-null, +0xa0 == 1) is weighed by the function of its kind
// (+0: 0 -> 01146EB8, 1 -> 01146FB8, else 01147070; each gets a0 = a
// 16-byte out at sp+0x10, a1 = one at sp, a2 the point, a3 the zone, and
// returns the weight in f0). A zone with weight > 0 goes into the result
// list at 0x1217100 (32-byte records: +0 the slot, +4 the weight, +8 the
// first word of the sp+0x10 out, +0x10 the sp out), kept in descending
// order of the zone's priority (*(+4)); a zone of priority -100000 is the
// only result (the walk stops). Returns the count.
//
// The zone loop and the sorted insert natively; the three weight functions
// stay guest calls from the original's frame. A slot or zone outside main
// RAM goes to the recompiled code before anything is done (checked up front).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_stats.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_wind
{
    constexpr uint32_t kFn = 0x01145F78u, kSlots = 0x01216EF0u, kResults = 0x01217100u;
    constexpr int kCount = 0x50;

    // ---- Round S7 (SOTC_S7WIND, A/B `s7wind`): the wind zones' query without its slot scans and its sphere zones' round
    // trips ---------------------------------------------------------------------------------------------------------------
    // Riding, the wind query runs ~40 times a frame (Wander's cloth and hair, Agro's mane and tail, the grass) and each call
    // scanned the 80 zone slots twice (the up-front check and the loop) and weighed every live zone through a guest call.
    // S7 keeps, host side, the list of the non-empty slots (made again whenever the 320-byte slot table differs from the
    // copy it was made from: one memcmp a call), and weighs the sphere zones (kind 0, manager_sub_01146EB8: the distance
    // falloff between the zone's inner and outer radii) as host code with the original's own operations, leaving the
    // registers, the frame and the outputs as the guest function leaves them. Kinds 1 / 2 (a box volume, a script's judge
    // volume) stay guest calls. Lane 0 only (the worker shell runs the original). SOTC_S7WIND_CHECK=1: every call run both
    // ways from the same state and compared (results, count, registers, the frame); SOTC_S7WIND_LOG=1: [s7wind] counts
    // and [s7time] the host time a call by state, every 600 frames.
    bool g_s7AbOff = false; // set by sotc_native's A/B windows (`s7wind`)
    bool g_s7AbOn = false;  // round S20: listed in SOTC_AB_FLAG, its on windows turn it on even without SOTC_S7WIND=1
    namespace s7
    {
        int g_on = -1, g_check = 0, g_log = 0;
        inline bool envOn(const char *name, bool def)
        {
            const char *e = std::getenv(name);
            return e ? std::atoi(e) != 0 : def;
        }
        inline bool on()
        {
            if (__builtin_expect(g_on < 0, 0))
            {
                // On by default since round S20 (S7 left it off: z > 2 was asked of each seed): exact (0 differ), and all its
                // A/Bs pooled with S20's ride7 seed 13 (+0.22 +- 0.39) read +0.12 +- 0.06 ms (2 SE), z about 4 (docs/ab_s20.md).
                // SOTC_S7WIND=0 turns it off.
                g_on = envOn("SOTC_S7WIND", true) ? 1 : 0;
                g_check = envOn("SOTC_S7WIND_CHECK", false) ? 1 : 0;
                g_log = envOn("SOTC_S7WIND_LOG", false) ? 1 : 0;
                std::fprintf(stderr, "[s7wind] init: on %d, check %d, log %d\n", g_on, g_check, g_log);
            }
            return (g_on || g_s7AbOn) && !g_s7AbOff;
        }
        uint32_t g_copy[kCount];
        uint8_t g_list[kCount];
        int g_listN = -1; // -1: never made
        // Statistics (lane 0).
        uint64_t g_calls[2] = {0, 0}, g_us[2] = {0, 0}, g_rebuilds = 0, g_live = 0, g_kind[3] = {0, 0, 0}, g_checked = 0, g_differ = 0, g_lastLog = 0;
        inline uint64_t nowUs()
        {
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        }
        // The non-empty slots in ascending order, from a copy of the table equal to guest memory.
        inline void refresh(const uint8_t *rdram)
        {
            if (g_listN >= 0)
            {
                // 64-bit words, OR-reduced (the Vita's memcmp goes a byte at a time: 320 compares).
                uint64_t diff = 0;
                const uint8_t *live = rdram + kSlots;
                const uint8_t *copy = reinterpret_cast<const uint8_t *>(g_copy);
                for (uint32_t o = 0; o < sizeof(g_copy); o += 16u)
                {
                    uint64_t a0, a1, b0, b1;
                    std::memcpy(&a0, live + o, 8);
                    std::memcpy(&a1, live + o + 8u, 8);
                    std::memcpy(&b0, copy + o, 8);
                    std::memcpy(&b1, copy + o + 8u, 8);
                    diff |= (a0 ^ b0) | (a1 ^ b1);
                }
                if (diff == 0u)
                    return;
            }
            std::memcpy(g_copy, rdram + kSlots, sizeof(g_copy));
            g_listN = 0;
            for (int i = 0; i < kCount; ++i)
                if (g_copy[i] != 0u)
                    g_list[g_listN++] = static_cast<uint8_t>(i);
            ++g_rebuilds;
        }
        inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
        inline float lwc1(const uint8_t *rdram, uint32_t a)
        {
            float f;
            std::memcpy(&f, rdram + (a & PS2_RAM_MASK), 4);
            return f;
        }
        inline void swc1(uint8_t *rdram, uint32_t a, float f) { std::memcpy(rdram + (a & PS2_RAM_MASK), &f, 4); }
        inline uint32_t bitsOf(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        // manager_sub_01146EB8 (the sphere zone's weight) as host code: a0 the distance out, a1 the 16-byte offset out,
        // a2 the point, a3 the zone; returns in f0. The same operations in the same order as the recompiled function (its
        // delay slots included), so f0..f4, at / v0 / v1, vf5 / vf6 / vf7, the C bit and both outputs end as it leaves them.
        void weighSphere(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
        {
            const uint32_t p = READ32(a3 + 4u);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(p));
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x3F800000u));
            float f0 = ctx->f[0], f1 = 1.0f, f2, f3, f4;
            ctx->f[1] = f1;
            const uint32_t r2 = READ32(p + 4u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(r2));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(a3 + 176u));
            if (r2 != 0u)
            {
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a2));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(a3 + 176u));
                {
                    __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]);
                    __m128i mask = _mm_set_epi32(0, -1, -1, -1);
                    ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
                }
                WRITE128(a1, _mm_castps_si128(ctx->vu0_vf[5]));
                ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(a1));
                {
                    __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]);
                    __m128i mask = _mm_set_epi32(0, -1, -1, -1);
                    ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
                }
                {
                    __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1, 1, 1, 1)));
                    __m128i mask = _mm_set_epi32(0, 0, 0, -1);
                    ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
                }
                {
                    __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2, 2, 2, 2)));
                    __m128i mask = _mm_set_epi32(0, 0, 0, -1);
                    ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask));
                }
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[7]));
                {
                    const uint32_t bits = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(ctx->vu0_vf[7])));
                    std::memcpy(&f0, &bits, 4);
                }
                ctx->f[0] = f0;
                SET_GPR_S32(ctx, 1, static_cast<int32_t>(0xBF800000u));
                f1 = -1.0f;
                ctx->f[1] = f1;
                swc1(rdram, a0, f0);
            }
            // 0x1146F04
            f2 = 0.0f;
            ctx->f[2] = f2;
            setC(ctx, FPU_C_OLT_S(f1, f2));
            f0 = FPU_MOV_S(f1);
            ctx->f[0] = f0;
            if (!(ctx->fcr31 & 0x800000u))
                return;
            f0 = lwc1(rdram, a3 + 260u);
            ctx->f[0] = f0;
            f3 = lwc1(rdram, a3 + 256u);
            ctx->f[3] = f3;
            setC(ctx, FPU_C_OLE_S(f3, f0));
            f4 = lwc1(rdram, a0);
            ctx->f[4] = f4;
            if (!(ctx->fcr31 & 0x800000u))
            {
                // 0x1146F70
                setC(ctx, FPU_C_OLE_S(f3, f4));
                f1 = FPU_MOV_S(f2);
                ctx->f[1] = f1;
                if (ctx->fcr31 & 0x800000u)
                    goto fa8;
                setC(ctx, FPU_C_OLE_S(f4, f0));
                if (ctx->fcr31 & 0x800000u)
                {
                    f0 = FPU_MOV_S(f1);
                    ctx->f[0] = f0;
                    return;
                }
                f0 = FPU_SUB_S(f3, f0);
                ctx->f[0] = f0;
                SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x3F800000u));
                f2 = 1.0f;
                ctx->f[2] = f2;
                f1 = FPU_SUB_S(f3, f4);
                f1 = ps2FpuDivide(ctx->fcr31, f1, f0);
                f1 = FPU_SUB_SF(f2, f1);
                ctx->f[1] = f1;
                goto fa8;
            }
            setC(ctx, FPU_C_OLE_S(f0, f4));
            f1 = FPU_MOV_S(f2);
            ctx->f[1] = f1;
            if (ctx->fcr31 & 0x800000u)
                goto fa8;
            setC(ctx, FPU_C_OLE_S(f4, f3));
            if (ctx->fcr31 & 0x800000u)
            {
                // 0x1146F60
                SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x3F800000u));
                f1 = 1.0f;
                ctx->f[1] = f1;
                f0 = FPU_MOV_S(f1);
                ctx->f[0] = f0;
                return;
            }
            f1 = FPU_SUB_S(f0, f3);
            f0 = FPU_SUB_S(f0, f4);
            ctx->f[0] = f0;
            f1 = ps2FpuDivide(ctx->fcr31, f0, f1);
            ctx->f[1] = f1;
            f0 = FPU_MOV_S(f1);
            ctx->f[0] = f0;
            return;
        fa8:
            f1 = ctx->f[1];
            f0 = FPU_MOV_S(f1);
            ctx->f[0] = f0;
        }
    }

    void windZonesBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool s7on);
    void windZones(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool lane0 = !sotc_decomp::onWorkerLane(runtime);
        const bool s7 = lane0 && s7::on();
        if (lane0 && __builtin_expect(s7::g_check == 1 && s7, 0))
        {
            // The check: the S7 path and the original from the same state; the original's outcome is kept.
            const uint32_t sp0 = GPR_U32(ctx, 29), frame = (sp0 - 0x70u) & PS2_RAM_MASK;
            static uint8_t res0[kCount * 32 + 32], fr0[0x70], resS[kCount * 32 + 32], frS[0x70];
            static R5900Context start, afterS;
            start = *ctx;
            std::memcpy(res0, rdram + kResults, sizeof(res0));
            std::memcpy(fr0, rdram + frame, sizeof(fr0));
            windZonesBody(rdram, ctx, runtime, true);
            afterS = *ctx;
            std::memcpy(resS, rdram + kResults, sizeof(resS));
            std::memcpy(frS, rdram + frame, sizeof(frS));
            *ctx = start;
            std::memcpy(rdram + kResults, res0, sizeof(res0));
            std::memcpy(rdram + frame, fr0, sizeof(fr0));
            windZonesBody(rdram, ctx, runtime, false);
            bool same = std::memcmp(resS, rdram + kResults, sizeof(resS)) == 0 && std::memcmp(frS, rdram + frame, sizeof(frS)) == 0 &&
                        ctx->pc == afterS.pc && ctx->fcr31 == afterS.fcr31 && std::memcmp(ctx->r, afterS.r, sizeof(ctx->r)) == 0 &&
                        std::memcmp(ctx->f, afterS.f, sizeof(ctx->f)) == 0;
            for (int v = 5; v <= 7; ++v)
                same = same && std::memcmp(&ctx->vu0_vf[v], &afterS.vu0_vf[v], sizeof(ctx->vu0_vf[v])) == 0;
            ++s7::g_checked;
            if (!same && ++s7::g_differ <= 20u)
                std::fprintf(stderr, "[s7wind] frame %llu: call DIFFERS (n %d / %d, f0 %08x / %08x)\n", (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed),
                             GPR_S32(&afterS, 2), GPR_S32(ctx, 2), s7::bitsOf(afterS.f[0]), s7::bitsOf(ctx->f[0]));
        }
        else if (lane0 && __builtin_expect(s7::g_log == 1, 0))
        {
            const uint64_t t0 = s7::nowUs();
            windZonesBody(rdram, ctx, runtime, s7);
            s7::g_us[s7 ? 1 : 0] += s7::nowUs() - t0;
            ++s7::g_calls[s7 ? 1 : 0];
        }
        else
            windZonesBody(rdram, ctx, runtime, s7);
        const uint64_t frameNow = ps2_stats::vif1Chains.load(std::memory_order_relaxed); // the game frame (as the input script's clock)
        if (lane0 && __builtin_expect(s7::g_log == 1 || s7::g_check == 1, 0) && frameNow >= s7::g_lastLog + 600u)
        {
            s7::g_lastLog = frameNow;
            const double c0 = static_cast<double>(s7::g_calls[0]), c1 = static_cast<double>(s7::g_calls[1]);
            std::fprintf(stderr, "[s7wind] frame %llu: list rebuilt %llu, live zones %llu, weighed by kind %llu / %llu / %llu, checked %llu, differ %llu\n",
                         (unsigned long long)frameNow, (unsigned long long)s7::g_rebuilds, (unsigned long long)s7::g_live,
                         (unsigned long long)s7::g_kind[0], (unsigned long long)s7::g_kind[1], (unsigned long long)s7::g_kind[2], (unsigned long long)s7::g_checked,
                         (unsigned long long)s7::g_differ);
            if (s7::g_log == 1)
                std::fprintf(stderr, "[s7time] frame %llu: off %llu calls %.2f us, on %llu calls %.2f us\n", (unsigned long long)frameNow,
                             (unsigned long long)s7::g_calls[0], c0 > 0 ? static_cast<double>(s7::g_us[0]) / c0 : 0.0, (unsigned long long)s7::g_calls[1],
                             c1 > 0 ? static_cast<double>(s7::g_us[1]) / c1 : 0.0);
        }
    }

    void windZonesBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool s7on)
    {
        auto ok = [](uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE && (a & 3u) == 0u; };
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x70u;
        const uint32_t point = GPR_U32(ctx, 4);
        if (!ok(frame, 0x70u))
            return sotc_decomp::recompiled(kFn)(rdram, ctx, runtime);
        if (s7on)
        {
            // Round S7: only the non-empty slots (the host list, made from a copy equal to the table).
            s7::refresh(rdram);
            for (int j = 0; j < s7::g_listN; ++j)
            {
                const uint32_t zone = s7::g_copy[s7::g_list[j]];
                if (!ok(zone, 0xA4u) || !ok(u32(zone + 4u), 4u))
                    return sotc_decomp::recompiled(kFn)(rdram, ctx, runtime);
            }
        }
        else
            for (int i = 0; i < kCount; ++i)
            {
                const uint32_t zone = u32(kSlots + static_cast<uint32_t>(i) * 4u);
                if (zone == 0u)
                    continue;
                if (!ok(zone, 0xA4u) || !ok(u32(zone + 4u), 4u))
                    return sotc_decomp::recompiled(kFn)(rdram, ctx, runtime);
            }
        const uint32_t weigh[3] = {sotc_decomp::jalTarget(rdram, 0x01145FF4u), sotc_decomp::jalTarget(rdram, 0x01146014u),
                                   sotc_decomp::jalTarget(rdram, 0x0114602Cu)};
        const uint32_t sites[3] = {0x01145FF4u, 0x01146014u, 0x0114602Cu};
        // The callee-saved registers the original uses (restored at its end).
        const uint64_t saved[8] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19),
                                   GPR_U64(ctx, 20), GPR_U64(ctx, 21), GPR_U64(ctx, 22), GPR_U64(ctx, 23)};
        int32_t n = 0;
        // Round 54 (SOTC_FASTNATIVE2): the empty slots skipped in a loop of
        // their own, the slot array through a host pointer (the zone loop
        // kept every counter and the frame's values on the stack across the
        // skip: ~40 calls a frame, 80 slots each, most of them empty). The
        // same slots read in the same order.
        const bool fastSkip = sotc_decomp::fastNative2On();
        const uint8_t *slotWords = rdram + kSlots;
        const bool sphereHost = s7on && weigh[0] == 0x01146EB8u;
        int listAt = 0;
        for (int i = 0; i < kCount; ++i)
        {
            if (s7on)
            {
                // Round S7: the next non-empty slot from the host list (the same slots in the same order).
                if (listAt == s7::g_listN)
                    break;
                i = s7::g_list[listAt++];
            }
            else if (fastSkip)
            {
                uint32_t w;
                while (true)
                {
                    std::memcpy(&w, slotWords + static_cast<uint32_t>(i) * 4u, 4);
                    if (w != 0u || ++i == kCount)
                        break;
                }
                if (i == kCount)
                    break;
            }
            const uint32_t zone = u32(kSlots + static_cast<uint32_t>(i) * 4u);
            if (zone == 0u || u32(zone + 0xA0u) != 1u)
                continue;
            const uint32_t kind = u32(zone);
            const int k = kind == 0u ? 0 : (kind == 1u ? 1 : 2);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(frame + 0x10u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(frame));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(point));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(zone));
            // What the original holds in s0-s7 across the call (the callee may look; it preserves them).
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(zone));
            SET_GPR_S32(ctx, 17, n);
            SET_GPR_S32(ctx, 18, i);
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(kResults));
            SET_GPR_S32(ctx, 20, static_cast<int32_t>(point));
            SET_GPR_S32(ctx, 21, static_cast<int32_t>(kSlots + static_cast<uint32_t>(i) * 4u));
            SET_GPR_S32(ctx, 22, static_cast<int32_t>(kSlots));
            SET_GPR_S32(ctx, 23, 0x01210000);
            if (s7on && s7::g_log == 1)
            {
                ++s7::g_live;
                ++s7::g_kind[k];
            }
            if (k == 0 && sphereHost)
            {
                // Round S7: the sphere zone's weight as host code; ra, sp and pc as the call leaves them.
                SET_GPR_U32(ctx, 31, sites[0] + 8u);
                SET_GPR_U32(ctx, 29, frame);
                s7::weighSphere(rdram, ctx, runtime, frame + 0x10u, frame, point, zone);
                ctx->pc = sites[0] + 8u;
            }
            else
                sotc_decomp::callGuest(rdram, ctx, runtime, weigh[k], sites[k] + 8u, frame);
            const float weight = ctx->f[0];
            ctx->f[1] = weight;
            ctx->f[0] = 0.0f;
            const bool c = FPU_C_OLT_S(0.0f, weight);
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (!c)
                continue;
            const int32_t prio = static_cast<int32_t>(u32(u32(zone + 4u)));
            uint8_t out[16];
            std::memcpy(out, rdram + frame, 16);
            float first;
            std::memcpy(&first, rdram + frame + 0x10u, 4);
            auto put = [&](uint32_t rec)
            {
                std::memcpy(rdram + rec + 4u, &weight, 4);
                std::memcpy(rdram + rec + 0x10u, out, 16);
                std::memcpy(rdram + rec + 8u, &first, 4);
                const uint32_t slot = static_cast<uint32_t>(i);
                std::memcpy(rdram + rec, &slot, 4);
            };
            if (prio == -100000)
            {
                // Order as the original: the slot, the out, the weight, the first word.
                const uint32_t slot = static_cast<uint32_t>(i);
                std::memcpy(rdram + kResults, &slot, 4);
                std::memcpy(rdram + kResults + 0x10u, out, 16);
                std::memcpy(rdram + kResults + 4u, &weight, 4);
                std::memcpy(rdram + kResults + 8u, &first, 4);
                ctx->f[0] = first;
                n = 1;
                break;
            }
            auto prioOf = [&](int32_t r) { return static_cast<int32_t>(u32(u32(u32(kSlots + u32(kResults + static_cast<uint32_t>(r) * 32u) * 4u) + 4u))); };
            int32_t at = -1;
            for (int32_t t = 0; t < n; ++t)
                if (prioOf(t) < prio)
                {
                    at = t;
                    break;
                }
            if (at == -1)
                at = n;
            else if (fastSkip)
            {
                // Round 54: the same move a record at a time from the top, in
                // place (the Vita's memmove is a library call through a veneer
                // for these few records).
                uint8_t *base = rdram + kResults;
                for (int32_t r = n - 1; r >= at; --r)
                {
                    uint8_t rec[32];
                    std::memcpy(rec, base + static_cast<uint32_t>(r) * 32u, 32);
                    std::memcpy(base + static_cast<uint32_t>(r + 1) * 32u, rec, 32);
                }
            }
            else // records at..n-1 up one, as the original's loop from the top leaves them (one move: round 11)
                std::memmove(rdram + kResults + static_cast<uint32_t>(at + 1) * 32u, rdram + kResults + static_cast<uint32_t>(at) * 32u,
                             static_cast<size_t>(n - at) * 32u);
            ++n;
            put(kResults + static_cast<uint32_t>(at) * 32u);
            ctx->f[0] = first;
        }
        for (int r = 0; r < 8; ++r)
            SET_GPR_U64(ctx, 16 + r, saved[r]);
        SET_GPR_S32(ctx, 2, n);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x01145F78u, windZones, decomp_wind::windZones);
