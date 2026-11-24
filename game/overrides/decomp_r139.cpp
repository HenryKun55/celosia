// Round 139: the leftover guest code around the scripts on the console's game thread.
//
// rnd139 (SOTC_RND139, A/B `rnd139`): iosGetRandom (0x1be7e8), the boot ELF's random float in
// [0, 1): seven vrnext, a vrinit from vf5.x, seven more vrnext, vf5.x - vf0.w, qmfc2 a3, f0 =
// a3's word (~600 calls a frame riding, most from the particle emitters' recompiled code). The
// recompiled code moves R and vf5 through the context at every step (a shuffle and a blend each);
// natively the 23-bit register is stepped in a host register and written once. The same results
// bit for bit: R = 0x3f800000 | the 23 bits (all four lanes, as vrnext / vrinit leave it), vf5.x =
// R's float - 1 (exact: R's float is in [1, 2)), its y / z / w kept, a3 = vf5's 128 bits, f0 = its
// x word. SOTC_RND139_CHECK=1: every call also runs the recompiled code on a copy of the context
// and compares R, vf5, a3, f0 ([rnd139] lines).
#include "sotc_decomp.h"
#include "d1_native.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_r139
{
    using sotc_decomp::returnTo;
    constexpr uint32_t kGetRandom = 0x001BE7E8u;

    __attribute__((always_inline)) inline uint32_t rnext(uint32_t r)
    {
        return (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u;
    }

    int g_check = -1;
    uint64_t g_checks = 0, g_bad = 0;

    void getRandom(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::rnd139On())
        {
            ctx->pc = kGetRandom;
            return sotc_decomp::recompiled(kGetRandom)(rdram, ctx, runtime);
        }
        if (__builtin_expect(g_check < 0, 0))
        {
            const char *e = ps2_diag::env("SOTC_RND139_CHECK");
            g_check = e && std::atoi(e) != 0;
        }
        R5900Context *copy = nullptr;
        if (__builtin_expect(g_check != 0, 0))
        {
            copy = new R5900Context(*ctx);
            copy->pc = kGetRandom;
            sotc_decomp::recompiled(kGetRandom)(rdram, copy, runtime);
        }
        uint32_t r;
        std::memcpy(&r, &ctx->vu0_r, 4);
        for (int i = 0; i < 7; ++i)
            r = rnext(r);
        // vrinit R, vf5x: vf5.x is the last vrnext's R
        r = 0x3F800000u | (r & 0x007FFFFFu);
        for (int i = 0; i < 7; ++i)
            r = rnext(r);
        const uint32_t lanes[4] = {r, r, r, r};
        std::memcpy(&ctx->vu0_r, lanes, 16);
        float x;
        std::memcpy(&x, &r, 4);
        x -= 1.0f;
        uint32_t xb;
        std::memcpy(&xb, &x, 4);
        std::memcpy(&ctx->vu0_vf[5], &xb, 4); // y / z / w kept
        std::memcpy(&ctx->r[7], &ctx->vu0_vf[5], 16); // qmfc2 a3, vf5
        std::memcpy(&ctx->f[0], &xb, 4);
        if (__builtin_expect(copy != nullptr, 0))
        {
            const bool same = std::memcmp(&copy->vu0_r, &ctx->vu0_r, 16) == 0 && std::memcmp(&copy->vu0_vf[5], &ctx->vu0_vf[5], 16) == 0 &&
                              std::memcmp(&copy->r[7], &ctx->r[7], 16) == 0 && std::memcmp(&copy->f[0], &ctx->f[0], 4) == 0 && copy->pc == GPR_U32(ctx, 31);
            ++g_checks;
            g_bad += !same;
            if ((!same && g_bad <= 20u) || g_checks % 200000u == 0u)
                std::fprintf(stderr, "[rnd139] checks %llu differ %llu%s\n", (unsigned long long)g_checks, (unsigned long long)g_bad, same ? "" : " (this one)");
            delete copy;
        }
        returnTo(ctx);
    }
}

namespace decomp_r139
{
    void getRandomSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x001BE7E8u, iosGetRandom, decomp_r139::getRandomSw);
D1_ALIAS(0x001BE7E8u, iosGetRandom, decomp_r139::getRandomSw, decomp_r139::getRandom);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): iosGetRandom as a typed core ------------------------------------------------
// The 23-bit generator stepped on a host word (vrnext x7, vrinit from the last R, vrnext x7) and the float in [1, 2)
// minus 1 (exact) in the core; the adapter reads R once and writes R (four lanes), vf5.x (y / z / w kept), a3 (vf5's
// 128 bits) and f0. rnd139 off or SOTC_RND139_CHECK set (or not read yet): the old body.
namespace decomp_r139
{
    D1_COUNTER(c_d1GetRandom, "iosGetRandom");

    PS2_FORCE_INLINE uint32_t d1Rnext(uint32_t r) { return (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; }

    struct RandomOut
    {
        uint32_t r;  // R's new word (every lane)
        uint32_t xb; // vf5.x / f0: R's float - 1
    };
    // R in; the new R and the result's bits out.
    PS2_FORCE_INLINE RandomOut getRandomCore(uint32_t r)
    {
        for (int i = 0; i < 7; ++i)
            r = d1Rnext(r);
        r = 0x3F800000u | (r & 0x007FFFFFu); // vrinit R, vf5x (the last vrnext's R)
        for (int i = 0; i < 7; ++i)
            r = d1Rnext(r);
        float x;
        std::memcpy(&x, &r, 4);
        x -= 1.0f; // exact: x in [1, 2)
        uint32_t xb;
        std::memcpy(&xb, &x, 4);
        return {r, xb};
    }

    void getRandomD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::rnd139On() || g_check != 0)
            return getRandom(rdram, ctx, runtime);
        ++c_d1GetRandom.n;
        uint32_t r;
        std::memcpy(&r, &ctx->vu0_r, 4);
        const RandomOut o = getRandomCore(r);
        const uint32_t lanes[4] = {o.r, o.r, o.r, o.r};
        std::memcpy(&ctx->vu0_r, lanes, 16);
        std::memcpy(&ctx->vu0_vf[5], &o.xb, 4);       // y / z / w kept
        std::memcpy(&ctx->r[7], &ctx->vu0_vf[5], 16); // qmfc2 a3, vf5
        std::memcpy(&ctx->f[0], &o.xb, 4);
        returnTo(ctx);
    }

    void getRandomSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            getRandomD1(rdram, ctx, runtime);
        else
            getRandom(rdram, ctx, runtime);
    }
}
