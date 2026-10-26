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

#include <cstring>

namespace decomp_wind
{
    constexpr uint32_t kFn = 0x01145F78u, kSlots = 0x01216EF0u, kResults = 0x01217100u;
    constexpr int kCount = 0x50;

    void windZones(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
        for (int i = 0; i < kCount; ++i)
        {
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
