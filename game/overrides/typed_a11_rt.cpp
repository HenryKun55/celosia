// typed_a11_rt.cpp: round A11's runtime pieces (docs/decomp_a11.md): the switch a11typed's flag, the matrix stack's cores (the one
// compiled copy of iosPushCurrentMatrix / iosPopCurrentMatrix as functions of values, typed_a11_ext.h) and, in a check build
// (PS2X_A11_CHECK=1), A11_NCHK: the two slots wrapped so each call first runs the core on a copy of the entry state (vi15, vf1..vf4,
// VU0's whole data memory), then the recompiled function itself, and compares vi15, vf1..vf4 and all 4 KB of the data memory.
#include "typed_a11_rt.h"
#include "typed_a11_ext.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

#include <cstdio>
#include <cstring>

bool g_a11Typed = true; // round A11 (typed_a11_rt.h): a11typed and A10's flag, set once a frame
#if PS2X_A11_CHECK
A3Stats g_a11s; // round A11's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif

namespace a11x
{
    uint8_t *vu0Data(PS2Runtime *rt) { return rt->memory().getVU0Data(); }

    __attribute__((noinline)) void pushCore(uint8_t *vu0, uint16_t &vi15, const a4::vec4 &v1, const a4::vec4 &v2, const a4::vec4 &v3,
                                            const a4::vec4 &v4)
    {
        // vsqi.xyzw vfN, (vi15++): the quadword at vi15 & 0xFF, all four lanes, then vi15 + 1 (16 bits)
        std::memcpy(vu0 + vuOff(vi15), &v1, 16);
        vi15 = static_cast<uint16_t>(vi15 + 1u);
        std::memcpy(vu0 + vuOff(vi15), &v2, 16);
        vi15 = static_cast<uint16_t>(vi15 + 1u);
        std::memcpy(vu0 + vuOff(vi15), &v3, 16);
        vi15 = static_cast<uint16_t>(vi15 + 1u);
        std::memcpy(vu0 + vuOff(vi15), &v4, 16);
        vi15 = static_cast<uint16_t>(vi15 + 1u);
    }

    __attribute__((noinline)) void popCore(const uint8_t *vu0, uint16_t &vi15, a4::vec4 &v1, a4::vec4 &v2, a4::vec4 &v3, a4::vec4 &v4)
    {
        // vlqd.xyzw vfN, (--vi15): vi15 - 1 (16 bits), then the quadword at vi15 & 0xFF, all four lanes
        vi15 = static_cast<uint16_t>(vi15 - 1u);
        std::memcpy(&v4, vu0 + vuOff(vi15), 16);
        vi15 = static_cast<uint16_t>(vi15 - 1u);
        std::memcpy(&v3, vu0 + vuOff(vi15), 16);
        vi15 = static_cast<uint16_t>(vi15 - 1u);
        std::memcpy(&v2, vu0 + vuOff(vi15), 16);
        vi15 = static_cast<uint16_t>(vi15 - 1u);
        std::memcpy(&v1, vu0 + vuOff(vi15), 16);
    }
}

#if PS2X_A11_CHECK
// ---- A11_NCHK: the cores against the recompiled functions, every call (game thread and workers: each on its own runtime's data)
namespace a11nchk
{
    uint64_t g_checks[2] = {0, 0}, g_differ[2] = {0, 0}, g_vi[2] = {0, 0}, g_vf[2] = {0, 0}, g_mem[2] = {0, 0};
    namespace
    {
        PS2Runtime::RecompiledFunction g_rec[2] = {nullptr, nullptr};
        thread_local uint8_t t_vu[PS2_VU0_DATA_SIZE];

        void check(int k, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            uint8_t *vu0 = runtime->memory().getVU0Data();
            std::memcpy(t_vu, vu0, PS2_VU0_DATA_SIZE);
            uint16_t vi = ctx->vi[15];
            a4::vec4 v1 = ctx->vu0_vf[1], v2 = ctx->vu0_vf[2], v3 = ctx->vu0_vf[3], v4 = ctx->vu0_vf[4];
            if (k == 0)
                a11x::pushCore(t_vu, vi, v1, v2, v3, v4);
            else
                a11x::popCore(t_vu, vi, v1, v2, v3, v4);
            g_rec[k](rdram, ctx, runtime);
            const bool dvi = vi != ctx->vi[15];
            const bool dvf = std::memcmp(&v1, &ctx->vu0_vf[1], 16) || std::memcmp(&v2, &ctx->vu0_vf[2], 16) || std::memcmp(&v3, &ctx->vu0_vf[3], 16) ||
                             std::memcmp(&v4, &ctx->vu0_vf[4], 16);
            const bool dmem = std::memcmp(t_vu, vu0, PS2_VU0_DATA_SIZE) != 0;
            __atomic_fetch_add(&g_checks[k], 1u, __ATOMIC_RELAXED);
            if (dvi || dvf || dmem)
            {
                __atomic_fetch_add(&g_differ[k], 1u, __ATOMIC_RELAXED);
                __atomic_fetch_add(&g_vi[k], dvi ? 1u : 0u, __ATOMIC_RELAXED);
                __atomic_fetch_add(&g_vf[k], dvf ? 1u : 0u, __ATOMIC_RELAXED);
                __atomic_fetch_add(&g_mem[k], dmem ? 1u : 0u, __ATOMIC_RELAXED);
            }
        }
        void push(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { check(0, rdram, ctx, runtime); }
        void pop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { check(1, rdram, ctx, runtime); }
    }
    // over each slot while it still holds the recompiled function (sotc_decomp::install, a check build only)
    uint32_t install(PS2Runtime &runtime)
    {
        uint32_t n = 0;
        if (runtime.lookupFunction(0x1B8638u) == &::iosPushCurrentMatrix_0x1b8638)
            g_rec[0] = &::iosPushCurrentMatrix_0x1b8638, runtime.registerFunction(0x1B8638u, &push), ++n;
        if (runtime.lookupFunction(0x1B8650u) == &::iosPopCurrentMatrix_0x1b8650)
            g_rec[1] = &::iosPopCurrentMatrix_0x1b8650, runtime.registerFunction(0x1B8650u, &pop), ++n;
        return n;
    }
    bool wraps(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known)
    {
        return (got == &push && known == &::iosPushCurrentMatrix_0x1b8638) || (got == &pop && known == &::iosPopCurrentMatrix_0x1b8650);
    }
    void report(unsigned long long frame)
    {
        static const char *const nm[2] = {"iosPushCurrentMatrix", "iosPopCurrentMatrix"};
        for (int k = 0; k < 2; ++k)
            std::fprintf(stderr, "[a11nchk] frame %llu: %s: checks %llu differ %llu (vi15 %llu, vf1-vf4 %llu, VU0 data %llu)\n", frame, nm[k],
                         (unsigned long long)g_checks[k], (unsigned long long)g_differ[k], (unsigned long long)g_vi[k], (unsigned long long)g_vf[k],
                         (unsigned long long)g_mem[k]);
    }
}
#endif
