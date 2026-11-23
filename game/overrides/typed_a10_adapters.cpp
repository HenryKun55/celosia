// typed_a10_adapters.cpp: round A10's adapters for the never-lifted functions stage 10 typed (tools/typed_lift2.py;
// docs/decomp_a10.md). Generated: do not edit. Each takes the recompiled function's slot at install (a10InstallAdapters, from
// sotc_decomp::install after A9's, only while the slot still holds the recompiled function): the typed function while the
// switch a10typed is on and every slot it reaches holds what its typed callee stands for (A10_GO / A10_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a10.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a10ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

namespace a10ad
{
// SelectOmakeItem (0x1400a20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SelectOmakeItem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1400A20u, 1))
    {
        if (PS2_EXPECT(A10_GO() && A10_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A10_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A10_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A10_HOLDSX(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A10_HOLDS(0x11134B0u, ::a6ad::SpriteMenuSetCheckBoxByHash) && A10_HOLDS(0x134E530u, lift_d1::_omakeSelectFlagSet) && A10_HOLDS(0x134F2A8u, lift_d1::getGameHardMode) && A10_HOLDS(0x134FA48u, ::a6ad::_omakeSelectFlagCls) && A10_HOLDSX(0x134FB28u, lift_d1::_omakeSelectFlagChk) && A10_HOLDS(0x1407310u, lift_d1::getSelectWeapon) && A5_JAL(0x1400A78u, 0x1CBDB8u) && A5_JAL(0x1400AB4u, 0x1CBDB8u) && A5_JAL(0x1400ACCu, 0x1112C88u) && A5_JAL(0x1400B50u, 0x1CBDB8u) && A5_JAL(0x1400B8Cu, 0x1CBDB8u) && A5_JAL(0x1400BB0u, 0x11134B0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A10_CHK(SelectOmakeItem, a2_4, a2_5, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A10_IN(2); const uint64_t a2_r = typed::SelectOmakeItem(rdram, runtime, a2_4, a2_5, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5); A10_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a10
    }
    ::SelectOmakeItem_0x1400a20(rdram, ctx, runtime);
}
}

namespace
{
    struct A10Ad
    {
        uint32_t addr;
        PS2Runtime::RecompiledFunction rec, ad;
    };
    const A10Ad kA10Ad[1] = {
        {0x1400A20u, &::SelectOmakeItem_0x1400a20, &a10ad::SelectOmakeItem},
    };
}

// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function
uint32_t a10InstallAdapters(PS2Runtime &runtime)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < 1u; ++i)
        if (runtime.lookupFunction(kA10Ad[i].addr) == kA10Ad[i].rec)
        {
            runtime.registerFunction(kA10Ad[i].addr, kA10Ad[i].ad);
            ++n;
        }
    return n;
}
