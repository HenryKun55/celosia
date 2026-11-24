// typed_a14_adapters.cpp: round A14's adapters for the never-lifted functions stage 14 typed (tools/typed_lift2.py;
// docs/decomp_a14.md). Generated: do not edit. Each takes the recompiled function's slot at install (a14InstallAdapters, from
// sotc_decomp::install after A13's, only while the slot still holds the recompiled function): the typed function while the
// switch a14typed is on and every slot it reaches holds what its typed callee stands for (A14_GO / A14_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a14.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a14ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

namespace a14ad
{
// sub_00101AC0 (0x101ac0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_00101AC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x101AC0u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_28 = Ps2GprU64(ctx, 28); A14_CHK(sub_00101AC0, a2_4, a2_5, a2_28); A14_IN(0); const uint64_t a2_r = typed::sub_00101AC0(rdram, runtime, a2_4, a2_5, a2_28); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_00101AC0_0x101ac0(rdram, ctx, runtime);
}
// QueryIntrContext (0x107460): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void QueryIntrContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107460u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { A14_CHK(QueryIntrContext); uint32_t a4_c0st = ctx->cop0_status; A14_IN(2); const uint64_t a2_r = typed::QueryIntrContext(rdram, runtime, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::QueryIntrContext_0x107460(rdram, ctx, runtime);
}
// sub_00109288 (0x109288): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_00109288(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109288u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(sub_00109288, a2_4); A14_IN(3); const uint64_t a2_r = typed::sub_00109288(rdram, runtime, a2_4); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_00109288_0x109288(rdram, ctx, runtime);
}
// ps2___moddi3 (0x123a18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2___moddi3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x123A18u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint64_t a2_24 = Ps2GprU64(ctx, 24); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(ps2___moddi3, a2_4, a2_5, a2_6, a2_7, a2_11, a2_24, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[8] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11), Ps2GprU64(ctx, 14), Ps2GprU64(ctx, 24)}; A14_IN(6); const uint64_t a2_r = typed::ps2___moddi3(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_11, a2_24, a2_29, a4_h3, a4_h4, a4_h5, a4_h6, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); Ps2SetGpr64(ctx, 14, a3_xo[6]); Ps2SetGpr64(ctx, 24, a3_xo[7]); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::ps2___moddi3_0x123a18(rdram, ctx, runtime);
}
// sub_0012B1D8 (0x12b1d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_0012B1D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12B1D8u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(sub_0012B1D8, a2_4, a2_5, a2_6, a2_29); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A14_IN(10); const uint64_t a2_r = typed::sub_0012B1D8(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_0012B1D8_0x12b1d8(rdram, ctx, runtime);
}
// Shock_SetMotor (0x1b3f68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Shock_SetMotor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B3F68u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(Shock_SetMotor, a2_4, a2_5, a2_6); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A14_IN(12); const uint64_t a2_r = typed::Shock_SetMotor(rdram, runtime, a2_4, a2_5, a2_6, a4_h7, a4_h8, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Shock_SetMotor_0x1b3f68(rdram, ctx, runtime);
}
// iosGetDRandomVector (0x1be858): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetDRandomVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BE858u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A14_CHK(iosGetDRandomVector, a2_4); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vrr = ctx->vu0_r; A14_IN(14); typed::iosGetDRandomVector(rdram, runtime, a2_4, a4_vf0, a4_vf5, a4_vrr); A14_OUT(); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_r = a4_vrr; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::iosGetDRandomVector_0x1be858(rdram, ctx, runtime);
}
// getTextureAreaAddressTBP (0x1c2198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getTextureAreaAddressTBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C2198u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { A14_CHK(getTextureAreaAddressTBP); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(20); const uint32_t a2_r = typed::getTextureAreaAddressTBP(rdram, runtime, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getTextureAreaAddressTBP_0x1c2198(rdram, ctx, runtime);
}
// getTextureAreaAddressFBP (0x1c21d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getTextureAreaAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C21D0u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { A14_CHK(getTextureAreaAddressFBP); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(21); const uint32_t a2_r = typed::getTextureAreaAddressFBP(rdram, runtime, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getTextureAreaAddressFBP_0x1c21d0(rdram, ctx, runtime);
}
// iosGetFontDataWithID (0x1c90e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetFontDataWithID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C90E8u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(iosGetFontDataWithID, a2_4, a2_5, a2_6); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(22); const uint64_t a2_r = typed::iosGetFontDataWithID(rdram, runtime, a2_4, a2_5, a2_6, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::iosGetFontDataWithID_0x1c90e8(rdram, ctx, runtime);
}
// Sg2SlotSetPitch (0x1d6578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetPitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D6578u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); A14_CHK(Sg2SlotSetPitch, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11), Ps2GprU64(ctx, 14)}; A14_IN(23); const uint32_t a2_r = typed::Sg2SlotSetPitch(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 10, a3_xo[0]); Ps2SetGpr64(ctx, 11, a3_xo[1]); Ps2SetGpr64(ctx, 14, a3_xo[2]); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetPitch_0x1d6578(rdram, ctx, runtime);
}
// kernel_sub_001DA790 (0x1da790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DA790(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA790u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A14_CHK(kernel_sub_001DA790, a2_4, a2_5, a2_6, a2_7); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a3_xo[6] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(25); const uint64_t a2_r = typed::kernel_sub_001DA790(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_hi1, a4_lo1, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DA790_0x1da790(rdram, ctx, runtime);
}
// kernel_sub_001DA850 (0x1da850): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DA850(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA850u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); A14_CHK(kernel_sub_001DA850, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h10 = a4::hi64(ctx->r[10]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(26); typed::kernel_sub_001DA850(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a4_hi1, a4_lo1, a4_h8, a4_h10, a3_xo); A14_OUT(); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 10, a3_xo[2]); Ps2SetGpr64(ctx, 11, a3_xo[3]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[10] = a4::mk128(a4_h10, Ps2GprU64(ctx, 10)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DA850_0x1da850(rdram, ctx, runtime);
}
// kernel_sub_001DA8F8 (0x1da8f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DA8F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA8F8u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A14_CHK(kernel_sub_001DA8F8, a2_4, a2_5, a2_6, a2_7); uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(27); const uint64_t a2_r = typed::kernel_sub_001DA8F8(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DA8F8_0x1da8f8(rdram, ctx, runtime);
}
// kernel_sub_001DA9C8 (0x1da9c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DA9C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA9C8u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(kernel_sub_001DA9C8, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h10 = a4::hi64(ctx->r[10]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(28); typed::kernel_sub_001DA9C8(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_29, a4_h3, a4_h8, a4_h10, a3_xo); A14_OUT(); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 10, a3_xo[2]); Ps2SetGpr64(ctx, 11, a3_xo[3]); ctx->r[10] = a4::mk128(a4_h10, Ps2GprU64(ctx, 10)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DA9C8_0x1da9c8(rdram, ctx, runtime);
}
// Sg2ExPcmInputGetRealPlayFrameCount (0x1dba10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmInputGetRealPlayFrameCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DBA10u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(Sg2ExPcmInputGetRealPlayFrameCount, a2_4); A14_IN(29); const uint64_t a2_r = typed::Sg2ExPcmInputGetRealPlayFrameCount(rdram, runtime, a2_4); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmInputGetRealPlayFrameCount_0x1dba10(rdram, ctx, runtime);
}
// objectmanager_mallocerrorfunc (0x11023f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void objectmanager_mallocerrorfunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11023F0u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { A14_CHK(objectmanager_mallocerrorfunc); A14_IN(33); const uint32_t a2_r = typed::objectmanager_mallocerrorfunc(rdram, runtime); A14_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::objectmanager_mallocerrorfunc_0x11023f0(rdram, ctx, runtime);
}
// getStageAreaIndexByPos (0x1358958): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageAreaIndexByPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1358958u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(getStageAreaIndexByPos, a2_4); A14_IN(38); const a1::RetVF a2_r = typed::getStageAreaIndexByPos(rdram, runtime, a2_4); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getStageAreaIndexByPos_0x1358958(rdram, ctx, runtime);
}
// getStageAreaID (0x135d760): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageAreaID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135D760u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(getStageAreaID, a2_4, a2_5, a2_6); A14_IN(40); const a1::RetVF a2_r = typed::getStageAreaID(rdram, runtime, a2_4, a2_5, a2_6); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getStageAreaID_0x135d760(rdram, ctx, runtime);
}
// getStageAreaName (0x135d840): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageAreaName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135D840u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(getStageAreaName, a2_4, a2_5); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A14_IN(41); const a1::RetVF a2_r = typed::getStageAreaName(rdram, runtime, a2_4, a2_5, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getStageAreaName_0x135d840(rdram, ctx, runtime);
}
// getStageAreaNameUV (0x135d930): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageAreaNameUV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135D930u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(getStageAreaNameUV, a2_4, a2_5, a2_6); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(42); const uint64_t a2_r = typed::getStageAreaNameUV(rdram, runtime, a2_4, a2_5, a2_6, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getStageAreaNameUV_0x135d930(rdram, ctx, runtime);
}
// stagemanager_getCenterFromAreaId (0x13667b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void stagemanager_getCenterFromAreaId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13667B8u, 1))
    {
        if (PS2_EXPECT(A14_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(stagemanager_getCenterFromAreaId, a2_4, a2_5); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A14_IN(43); const a1::RetVF a2_r = typed::stagemanager_getCenterFromAreaId(rdram, runtime, a2_4, a2_5, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::stagemanager_getCenterFromAreaId_0x13667b8(rdram, ctx, runtime);
}
// sub_001091C0 (0x1091c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001091C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091C0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(sub_001091C0, a2_4); uint32_t a4_c0st = ctx->cop0_status; A14_IN(46); const uint64_t a2_r = typed::sub_001091C0(rdram, runtime, a2_4, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_001091C0_0x1091c0(rdram, ctx, runtime);
}
// sub_001092B8 (0x1092b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001092B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092B8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x109288u, ::a14ad::sub_00109288), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(sub_001092B8, a2_4, a2_5); A14_IN(47); const uint64_t a2_r = typed::sub_001092B8(rdram, runtime, a2_4, a2_5); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_001092B8_0x1092b8(rdram, ctx, runtime);
}
// GetTimerSystemTime (0x110cc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimerSystemTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110CC8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(GetTimerSystemTime, a2_2, a2_29); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h9 = a4::hi64(ctx->r[9]); A14_IN(48); const uint64_t a2_r = typed::GetTimerSystemTime(rdram, runtime, a2_2, a2_29, a4_c0st, a4_h9); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetTimerSystemTime_0x110cc8(rdram, ctx, runtime);
}
// AllocTimerCounter (0x110db8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AllocTimerCounter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110DB8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x110D48u, lift_d1::iAllocTimerCounter), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); A14_CHK(AllocTimerCounter, a2_2); uint32_t a4_c0st = ctx->cop0_status; A14_IN(49); const uint64_t a2_r = typed::AllocTimerCounter(rdram, runtime, a2_2, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::AllocTimerCounter_0x110db8(rdram, ctx, runtime);
}
// GetTimerUsedUnusedCounters (0x110f40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimerUsedUnusedCounters(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110F40u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x110EF0u, lift_d1::iGetTimerUsedUnusedCounters), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(GetTimerUsedUnusedCounters, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(50); const uint64_t a2_r = typed::GetTimerUsedUnusedCounters(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetTimerUsedUnusedCounters_0x110f40(rdram, ctx, runtime);
}
// SetTimerCount (0x1111b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetTimerCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1111B8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x110C78u, lift_d1::iGetTimerSystemTime), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SetTimerCount, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint32_t a4_c0st = ctx->cop0_status; A14_IN(51); const uint64_t a2_r = typed::SetTimerCount(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SetTimerCount_0x1111b8(rdram, ctx, runtime);
}
// GetTimerBaseTime (0x1112d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimerBaseTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112D0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x111288u, lift_d1::iGetTimerBaseTime), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(GetTimerBaseTime, a2_2, a2_4); uint32_t a4_c0st = ctx->cop0_status; A14_IN(52); const uint64_t a2_r = typed::GetTimerBaseTime(rdram, runtime, a2_2, a2_4, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetTimerBaseTime_0x1112d0(rdram, ctx, runtime);
}
// TimerBusClock2USec (0x111558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void TimerBusClock2USec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x111558u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1240A8u, lift_d1::ps2___udivdi3), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_14 = Ps2GprU64(ctx, 14); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(TimerBusClock2USec, a2_2, a2_4, a2_5, a2_6, a2_14, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); A14_IN(53); const uint64_t a2_r = typed::TimerBusClock2USec(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_14, a2_29, a4_h3, a4_h4, a4_h6); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::TimerBusClock2USec_0x111558(rdram, ctx, runtime);
}
// TimerUSec2BusClock (0x111600): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void TimerUSec2BusClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x111600u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1240A8u, lift_d1::ps2___udivdi3), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_14 = Ps2GprU64(ctx, 14); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(TimerUSec2BusClock, a2_4, a2_5, a2_7, a2_14, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(54); const uint64_t a2_r = typed::TimerUSec2BusClock(rdram, runtime, a2_4, a2_5, a2_7, a2_14, a2_29, a4_hi1, a4_lo1, a4_h3, a4_h4, a4_h6, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::TimerUSec2BusClock_0x111600(rdram, ctx, runtime);
}
// sceSifSetRebootNotifyBuffer (0x111b18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sceSifSetRebootNotifyBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x111B18u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1283D0u, ::memset_0x1283d0), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(sceSifSetRebootNotifyBuffer, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A14_IN(55); const uint64_t a2_r = typed::sceSifSetRebootNotifyBuffer(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_c0st, a4_h2, a4_h4); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sceSifSetRebootNotifyBuffer_0x111b18(rdram, ctx, runtime);
}
// sub_001199F8 (0x1199f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001199F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1199F8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(sub_001199F8, a2_4); uint32_t a4_c0st = ctx->cop0_status; uint64_t a3_xo[1] = {Ps2GprU64(ctx, 5)}; A14_IN(56); const uint64_t a2_r = typed::sub_001199F8(rdram, runtime, a2_4, a4_c0st, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_001199F8_0x1199f8(rdram, ctx, runtime);
}
// sub_0012C240 (0x12c240): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_0012C240(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12C240u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x12F0D8u, lift_d1::ps2___mcmp), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(sub_0012C240, a2_4, a2_5, a2_7, a2_8, a2_29); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(58); const uint64_t a2_r = typed::sub_0012C240(rdram, runtime, a2_4, a2_5, a2_7, a2_8, a2_29, a4_h2, a4_h6, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::sub_0012C240_0x12c240(rdram, ctx, runtime);
}
// kernel_sub_001B7590 (0x1b7590): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001B7590(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B7590u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128FB0u, ::strcpy_0x128fb0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(kernel_sub_001B7590, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[6] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(61); const uint64_t a2_r = typed::kernel_sub_001B7590(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001B7590_0x1b7590(rdram, ctx, runtime);
}
// Sg2CtrlEntryTickProc (0x1cc030): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2CtrlEntryTickProc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC030u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC0C8u, lift_d1::Sg2CtrlEntryTickProcById), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(Sg2CtrlEntryTickProc, a2_2, a2_4); uint32_t a4_c0st = ctx->cop0_status; A14_IN(62); const uint64_t a2_r = typed::Sg2CtrlEntryTickProc(rdram, runtime, a2_2, a2_4, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2CtrlEntryTickProc_0x1cc030(rdram, ctx, runtime);
}
// Sg2CtrlRemoveTickProc (0x1cc108): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2CtrlRemoveTickProc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC108u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A14_CHK(Sg2CtrlRemoveTickProc, a2_2, a2_4); uint32_t a4_c0st = ctx->cop0_status; A14_IN(63); const uint64_t a2_r = typed::Sg2CtrlRemoveTickProc(rdram, runtime, a2_2, a2_4, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2CtrlRemoveTickProc_0x1cc108(rdram, ctx, runtime);
}
// Sg2DspEffectSetDepth2 (0x1cc458): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2DspEffectSetDepth2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC458u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A14_CHK(Sg2DspEffectSetDepth2, a2_2, a2_4, a2_5, a2_6); uint32_t a4_c0st = ctx->cop0_status; A14_IN(67); const uint64_t a2_r = typed::Sg2DspEffectSetDepth2(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2DspEffectSetDepth2_0x1cc458(rdram, ctx, runtime);
}
// Sg2DspEffectSetDelay (0x1cc4d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2DspEffectSetDelay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC4D0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2DspEffectSetDelay, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(68); const uint64_t a2_r = typed::Sg2DspEffectSetDelay(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2DspEffectSetDelay_0x1cc4d0(rdram, ctx, runtime);
}
// Sg2DspEffectSetFeedBack (0x1cc530): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2DspEffectSetFeedBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC530u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2DspEffectSetFeedBack, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(69); const uint64_t a2_r = typed::Sg2DspEffectSetFeedBack(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2DspEffectSetFeedBack_0x1cc530(rdram, ctx, runtime);
}
// Sg2ModMidiout1Exclusive (0x1ce350): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ModMidiout1Exclusive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CE350u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2ModMidiout1Exclusive, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(71); const uint64_t a2_r = typed::Sg2ModMidiout1Exclusive(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ModMidiout1Exclusive_0x1ce350(rdram, ctx, runtime);
}
// Sg2ModMidiout (0x1ce3f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ModMidiout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CE3F0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x12821Cu, ::memcpy_0x12821c), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(Sg2ModMidiout, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A14_IN(72); const uint64_t a2_r = typed::Sg2ModMidiout(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_c0st, a4_h2, a4_h4, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); ctx->cop0_status = a4_c0st; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ModMidiout_0x1ce3f0(rdram, ctx, runtime);
}
// Sg2ModOpenBgm (0x1ce898): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ModOpenBgm(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CE898u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CDDE8u, lift_d1::kernel_sub_001CDDE8) && A14_HOLDS(0x1D3B08u, lift_d1::Sg2SlotGetSlotClassIdx) && A14_HOLDS(0x1DA0A0u, lift_d1::vab_check_alive) && A14_HOLDS(0x1DA0E0u, lift_d1::vab_get_info) && A14_HOLDS(0x1DA348u, lift_d1::vab_get_veltbl) && A14_HOLDS(0x1DA380u, lift_d1::vab_get_lfotbl_param), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2ModOpenBgm, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[6] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(73); const uint64_t a2_r = typed::Sg2ModOpenBgm(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st, a4_h2, a4_h3, a4_h4, a4_h5, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); ctx->cop0_status = a4_c0st; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ModOpenBgm_0x1ce898(rdram, ctx, runtime);
}
// Sg2ModOpenSe (0x1cea68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ModOpenSe(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CEA68u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CDF38u, lift_d1::kernel_sub_001CDF38) && A14_HOLDS(0x1D3B08u, lift_d1::Sg2SlotGetSlotClassIdx) && A14_HOLDS(0x1DA0A0u, lift_d1::vab_check_alive) && A14_HOLDS(0x1DA0E0u, lift_d1::vab_get_info) && A14_HOLDS(0x1DA348u, lift_d1::vab_get_veltbl) && A14_HOLDS(0x1DA380u, lift_d1::vab_get_lfotbl_param), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2ModOpenSe, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(74); const uint64_t a2_r = typed::Sg2ModOpenSe(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st, a4_h2, a4_h4, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); Ps2SetGpr64(ctx, 9, a3_xo[4]); Ps2SetGpr64(ctx, 10, a3_xo[5]); Ps2SetGpr64(ctx, 11, a3_xo[6]); ctx->cop0_status = a4_c0st; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ModOpenSe_0x1cea68(rdram, ctx, runtime);
}
// kernel_sub_001D0E50 (0x1d0e50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D0E50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D0E50u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1D5FD8u, lift_d1::Sg2SlotFindSlotClassCh) && A14_HOLDS(0x1D67F0u, lift_d1::Sg2SlotSetLfoPitchSpeed), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(kernel_sub_001D0E50, a2_29); A14_IN(75); const uint64_t a2_r = typed::kernel_sub_001D0E50(rdram, runtime, a2_29); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001D0E50_0x1d0e50(rdram, ctx, runtime);
}
// Sg2SlotSetDumperOff (0x1d6d90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetDumperOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D6D90u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2SlotSetDumperOff, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(77); const uint64_t a2_r = typed::Sg2SlotSetDumperOff(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetDumperOff_0x1d6d90(rdram, ctx, runtime);
}
// Sg2SlotModKeyOff (0x1d6e70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotModKeyOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D6E70u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(Sg2SlotModKeyOff, a2_2, a2_4, a2_5, a2_6); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(78); const uint64_t a2_r = typed::Sg2SlotModKeyOff(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a4_c0st, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotModKeyOff_0x1d6e70(rdram, ctx, runtime);
}
// Sg2SlotSetReverbOnBit (0x1d72a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetReverbOnBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D72A8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2SlotSetReverbOnBit, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(82); const uint64_t a2_r = typed::Sg2SlotSetReverbOnBit(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetReverbOnBit_0x1d72a8(rdram, ctx, runtime);
}
// Sg2SlotSetReverbOffBit (0x1d7310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetReverbOffBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D7310u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2SlotSetReverbOffBit, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(83); const uint64_t a2_r = typed::Sg2SlotSetReverbOffBit(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetReverbOffBit_0x1d7310(rdram, ctx, runtime);
}
// Sg2SlotSetNoiseOnBit (0x1d7380): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetNoiseOnBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D7380u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2SlotSetNoiseOnBit, a2_2, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; A14_IN(84); const uint64_t a2_r = typed::Sg2SlotSetNoiseOnBit(rdram, runtime, a2_2, a2_4, a2_5, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetNoiseOnBit_0x1d7380(rdram, ctx, runtime);
}
// kernel_sub_001DB088 (0x1db088): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DB088(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DB088u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(kernel_sub_001DB088, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; uint64_t a3_xo[8] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), (uint64_t)a1_bits(ctx->f[2]), (uint64_t)a1_bits(ctx->f[3]), (uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23])}; A14_IN(86); const uint64_t a2_r = typed::kernel_sub_001DB088(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); ctx->f[2] = a1_flt((uint32_t)a3_xo[2]); ctx->f[3] = a1_flt((uint32_t)a3_xo[3]); ctx->f[20] = a1_flt((uint32_t)a3_xo[4]); ctx->f[21] = a1_flt((uint32_t)a3_xo[5]); ctx->f[22] = a1_flt((uint32_t)a3_xo[6]); ctx->f[23] = a1_flt((uint32_t)a3_xo[7]); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DB088_0x1db088(rdram, ctx, runtime);
}
// Sg2ExPcmInputCtrl (0x1db950): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmInputCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DB950u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(Sg2ExPcmInputCtrl, a2_4, a2_5); uint32_t a4_c0st = ctx->cop0_status; uint64_t a3_xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A14_IN(87); const uint64_t a2_r = typed::Sg2ExPcmInputCtrl(rdram, runtime, a2_4, a2_5, a4_c0st, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmInputCtrl_0x1db950(rdram, ctx, runtime);
}
// gamecore_sub_01346090 (0x1346090): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01346090(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1346090u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A5_JAL(0x13460D8u, 0x126F28u), 1)) { const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(gamecore_sub_01346090, a2_9, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(94); const a1::RetVF a2_r = typed::gamecore_sub_01346090(rdram, runtime, a2_9, a2_29, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::gamecore_sub_01346090_0x1346090(rdram, ctx, runtime);
}
// gamecore_sub_01361E88 (0x1361e88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01361E88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1361E88u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDSX(0x135FF08u, lift_d1::getstagenumber), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(gamecore_sub_01361E88, a2_5, a2_6, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(96); const a1::RetVF a2_r = typed::gamecore_sub_01361E88(rdram, runtime, a2_5, a2_6, a2_29, a4_hi1, a4_lo1, a4_h2, a4_h3, a4_h5, a4_h6, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 9, a3_xo[0]); Ps2SetGpr64(ctx, 10, a3_xo[1]); Ps2SetGpr64(ctx, 11, a3_xo[2]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::gamecore_sub_01361E88_0x1361e88(rdram, ctx, runtime);
}
// SpriteInitStatusClut (0x13997f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteInitStatusClut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13997F0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10AAC78u, ::a6ad::texGetCSM1Idx) && A5_JAL(0x1399818u, 0x10AAC78u) && A5_JAL(0x139982Cu, 0x10AAC78u) && A5_JAL(0x1399838u, 0x10AAC78u) && A5_JAL(0x1399844u, 0x10AAC78u) && A5_JAL(0x1399850u, 0x10AAC78u) && A5_JAL(0x1399890u, 0x10AAC78u) && A5_JAL(0x139989Cu, 0x10AAC78u) && A5_JAL(0x13998A8u, 0x10AAC78u) && A5_JAL(0x13998B4u, 0x10AAC78u) && A5_JAL(0x1399908u, 0x10AAC78u) && A5_JAL(0x1399914u, 0x10AAC78u) && A5_JAL(0x1399920u, 0x10AAC78u) && A5_JAL(0x139992Cu, 0x10AAC78u) && A5_JAL(0x1399980u, 0x10AAC78u) && A5_JAL(0x139998Cu, 0x10AAC78u) && A5_JAL(0x1399998u, 0x10AAC78u) && A5_JAL(0x13999A4u, 0x10AAC78u) && A5_JAL(0x13999F8u, 0x10AAC78u) && A5_JAL(0x1399A04u, 0x10AAC78u) && A5_JAL(0x1399A10u, 0x10AAC78u) && A5_JAL(0x1399A1Cu, 0x10AAC78u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(SpriteInitStatusClut, a2_2, a2_4, a2_29); A14_IN(100); const uint64_t a2_r = typed::SpriteInitStatusClut(rdram, runtime, a2_2, a2_4, a2_29); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SpriteInitStatusClut_0x13997f0(rdram, ctx, runtime);
}
// SpriteSetStatusClut (0x1399aa0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteSetStatusClut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1399AA0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10AAC78u, ::a6ad::texGetCSM1Idx) && A5_JAL(0x1399ADCu, 0x10AAC78u) && A5_JAL(0x1399BA0u, 0x10AAC78u) && A5_JAL(0x1399BACu, 0x10AAC78u) && A5_JAL(0x1399BCCu, 0x10AAC78u) && A5_JAL(0x1399BD8u, 0x10AAC78u) && A5_JAL(0x1399BF8u, 0x10AAC78u) && A5_JAL(0x1399C04u, 0x10AAC78u) && A5_JAL(0x1399C28u, 0x10AAC78u) && A5_JAL(0x1399C34u, 0x10AAC78u) && A5_JAL(0x1399C68u, 0x10AAC78u) && A5_JAL(0x1399C74u, 0x10AAC78u) && A5_JAL(0x1399C80u, 0x10AAC78u) && A5_JAL(0x1399C8Cu, 0x10AAC78u) && A5_JAL(0x1399CE4u, 0x10AAC78u) && A5_JAL(0x1399CF0u, 0x10AAC78u) && A5_JAL(0x1399CFCu, 0x10AAC78u) && A5_JAL(0x1399D08u, 0x10AAC78u) && A5_JAL(0x1399D60u, 0x10AAC78u) && A5_JAL(0x1399D6Cu, 0x10AAC78u) && A5_JAL(0x1399D78u, 0x10AAC78u) && A5_JAL(0x1399D84u, 0x10AAC78u) && A5_JAL(0x1399DDCu, 0x10AAC78u) && A5_JAL(0x1399DE8u, 0x10AAC78u) && A5_JAL(0x1399DF4u, 0x10AAC78u) && A5_JAL(0x1399E00u, 0x10AAC78u) && A5_JAL(0x1399E70u, 0x10AAC78u) && A5_JAL(0x1399E7Cu, 0x10AAC78u) && A5_JAL(0x1399E88u, 0x10AAC78u) && A5_JAL(0x1399E94u, 0x10AAC78u) && A5_JAL(0x1399EE8u, 0x10AAC78u) && A5_JAL(0x1399EF4u, 0x10AAC78u) && A5_JAL(0x1399F00u, 0x10AAC78u) && A5_JAL(0x1399F0Cu, 0x10AAC78u) && A5_JAL(0x1399F60u, 0x10AAC78u) && A5_JAL(0x1399F6Cu, 0x10AAC78u) && A5_JAL(0x1399F78u, 0x10AAC78u) && A5_JAL(0x1399F84u, 0x10AAC78u) && A5_JAL(0x1399FD8u, 0x10AAC78u) && A5_JAL(0x1399FE4u, 0x10AAC78u) && A5_JAL(0x1399FF0u, 0x10AAC78u) && A5_JAL(0x1399FFCu, 0x10AAC78u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(SpriteSetStatusClut, a2_2, a2_4, a2_5, a2_6, a2_29); A14_IN(101); const a1::RetVF a2_r = typed::SpriteSetStatusClut(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SpriteSetStatusClut_0x1399aa0(rdram, ctx, runtime);
}
// resetEventLineAttr (0x1432b30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void resetEventLineAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1432B30u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1169DE0u, lift_d1::getLineData) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A5_JAL(0x1432B94u, 0x1169DE0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A14_CHK(resetEventLineAttr, a2_2, a2_4, a2_5); A14_IN(104); const uint64_t a2_r = typed::resetEventLineAttr(rdram, runtime, a2_2, a2_4, a2_5); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::resetEventLineAttr_0x1432b30(rdram, ctx, runtime);
}
// setEventLinePrio (0x1432cc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setEventLinePrio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1432CC0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1169DE0u, lift_d1::getLineData) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A5_JAL(0x1432D2Cu, 0x1169DE0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(setEventLinePrio, a2_2, a2_4, a2_5, a2_6); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 4)}; A14_IN(106); const uint64_t a2_r = typed::setEventLinePrio(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::setEventLinePrio_0x1432cc0(rdram, ctx, runtime);
}
// getPointDataFromEvent (0x14338e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPointDataFromEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14338E8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A5_JAL(0x1433958u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(getPointDataFromEvent, a2_2, a2_4, a2_5, a2_6); A14_IN(107); const uint64_t a2_r = typed::getPointDataFromEvent(rdram, runtime, a2_2, a2_4, a2_5, a2_6); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getPointDataFromEvent_0x14338e8(rdram, ctx, runtime);
}
// getPointPosFromEvent (0x14339d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPointPosFromEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14339D8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A5_JAL(0x1433A44u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A14_CHK(getPointPosFromEvent, a2_2, a2_4, a2_5, a2_6); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(108); const uint64_t a2_r = typed::getPointPosFromEvent(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::getPointPosFromEvent_0x14339d8(rdram, ctx, runtime);
}
// Sg2ExStAdpcmTrkDspSw (0x1cd2b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExStAdpcmTrkDspSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CD2B8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1D72A8u, ::a14ad::Sg2SlotSetReverbOnBit) && A14_HOLDS(0x1D7310u, ::a14ad::Sg2SlotSetReverbOffBit), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A14_CHK(Sg2ExStAdpcmTrkDspSw, a2_4, a2_5, a2_6, a2_29); uint32_t a4_c0st = ctx->cop0_status; A14_IN(113); const uint64_t a2_r = typed::Sg2ExStAdpcmTrkDspSw(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExStAdpcmTrkDspSw_0x1cd2b8(rdram, ctx, runtime);
}
// kernel_sub_001CFB28 (0x1cfb28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001CFB28(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CFB28u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1D6E70u, ::a14ad::Sg2SlotModKeyOff), 1)) { A14_CHK(kernel_sub_001CFB28); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(115); const uint64_t a2_r = typed::kernel_sub_001CFB28(rdram, runtime, a4_c0st, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001CFB28_0x1cfb28(rdram, ctx, runtime);
}
// kernel_sub_001D1360 (0x1d1360): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D1360(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D1360u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1D6D90u, ::a14ad::Sg2SlotSetDumperOff), 1)) { A14_CHK(kernel_sub_001D1360); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(116); const uint64_t a2_r = typed::kernel_sub_001D1360(rdram, runtime, a4_c0st, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001D1360_0x1d1360(rdram, ctx, runtime);
}
// kernel_sub_001D1778 (0x1d1778): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D1778(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D1778u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1D5FD8u, lift_d1::Sg2SlotFindSlotClassCh) && A14_HOLDS(0x1D7240u, lift_d1::Sg2SlotSetKeyOffBit), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(kernel_sub_001D1778, a2_29); uint32_t a4_c0st = ctx->cop0_status; A14_IN(117); const uint64_t a2_r = typed::kernel_sub_001D1778(rdram, runtime, a2_29, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001D1778_0x1d1778(rdram, ctx, runtime);
}
// Sg2SlotSetBasicParam (0x1d6118): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SlotSetBasicParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D6118u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1D7310u, ::a14ad::Sg2SlotSetReverbOffBit) && A14_HOLDS(0x1D73E8u, lift_d1::Sg2SlotSetNoiseOffBit), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); A14_CHK(Sg2SlotSetBasicParam, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9); uint32_t a4_c0st = ctx->cop0_status; uint64_t a3_xo[3] = {Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11), Ps2GprU64(ctx, 14)}; A14_IN(119); const uint64_t a2_r = typed::Sg2SlotSetBasicParam(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a4_c0st, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 10, a3_xo[0]); Ps2SetGpr64(ctx, 11, a3_xo[1]); Ps2SetGpr64(ctx, 14, a3_xo[2]); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SlotSetBasicParam_0x1d6118(rdram, ctx, runtime);
}
// kernel_sub_001DAC50 (0x1dac50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DAC50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DAC50u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1DA790u, ::a14ad::kernel_sub_001DA790) && A14_HOLDS(0x1DA850u, ::a14ad::kernel_sub_001DA850) && A14_HOLDS(0x1DA8F8u, ::a14ad::kernel_sub_001DA8F8) && A14_HOLDS(0x1DA9C8u, ::a14ad::kernel_sub_001DA9C8) && A14_HOLDS(0x1DAAB0u, lift_d1::kernel_sub_001DAAB0) && A14_HOLDS(0x1DAB10u, ::a7ad::kernel_sub_001DAB10) && A14_HOLDS(0x1DCB48u, lift_d1::IirFilter), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A14_CHK(kernel_sub_001DAC50, a2_4, a2_5, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h10 = a4::hi64(ctx->r[10]); uint64_t a3_xo[20] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), (uint64_t)a1_bits(ctx->f[2]), (uint64_t)a1_bits(ctx->f[3]), (uint64_t)a1_bits(ctx->f[4]), (uint64_t)a1_bits(ctx->f[5]), (uint64_t)a1_bits(ctx->f[6]), (uint64_t)a1_bits(ctx->f[7]), (uint64_t)a1_bits(ctx->f[8]), (uint64_t)a1_bits(ctx->f[9]), (uint64_t)a1_bits(ctx->f[10]), (uint64_t)a1_bits(ctx->f[11]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(120); const uint64_t a2_r = typed::kernel_sub_001DAC50(rdram, runtime, a2_4, a2_5, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a4_hi1, a4_lo1, a4_h2, a4_h3, a4_h5, a4_h6, a4_h8, a4_h10, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); ctx->f[2] = a1_flt((uint32_t)a3_xo[2]); ctx->f[3] = a1_flt((uint32_t)a3_xo[3]); ctx->f[4] = a1_flt((uint32_t)a3_xo[4]); ctx->f[5] = a1_flt((uint32_t)a3_xo[5]); ctx->f[6] = a1_flt((uint32_t)a3_xo[6]); ctx->f[7] = a1_flt((uint32_t)a3_xo[7]); ctx->f[8] = a1_flt((uint32_t)a3_xo[8]); ctx->f[9] = a1_flt((uint32_t)a3_xo[9]); ctx->f[10] = a1_flt((uint32_t)a3_xo[10]); ctx->f[11] = a1_flt((uint32_t)a3_xo[11]); ctx->f[12] = a1_flt((uint32_t)a3_xo[12]); ctx->f[13] = a1_flt((uint32_t)a3_xo[13]); ctx->f[14] = a1_flt((uint32_t)a3_xo[14]); Ps2SetGpr64(ctx, 7, a3_xo[15]); Ps2SetGpr64(ctx, 8, a3_xo[16]); Ps2SetGpr64(ctx, 9, a3_xo[17]); Ps2SetGpr64(ctx, 10, a3_xo[18]); Ps2SetGpr64(ctx, 11, a3_xo[19]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[10] = a4::mk128(a4_h10, Ps2GprU64(ctx, 10)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DAC50_0x1dac50(rdram, ctx, runtime);
}
// kernel_sub_001DB238 (0x1db238): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DB238(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DB238u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode) && A14_HOLDS(0x1DB088u, ::a14ad::kernel_sub_001DB088) && A14_HOLDS(0x1DB238u, ::a14ad::kernel_sub_001DB238), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(kernel_sub_001DB238, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; uint64_t a3_xo[13] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[1]), (uint64_t)a1_bits(ctx->f[2]), (uint64_t)a1_bits(ctx->f[3]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23]), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A14_IN(121); const uint64_t a2_r = typed::kernel_sub_001DB238(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[1] = a1_flt((uint32_t)a3_xo[1]); ctx->f[2] = a1_flt((uint32_t)a3_xo[2]); ctx->f[3] = a1_flt((uint32_t)a3_xo[3]); ctx->f[12] = a1_flt((uint32_t)a3_xo[4]); ctx->f[13] = a1_flt((uint32_t)a3_xo[5]); ctx->f[20] = a1_flt((uint32_t)a3_xo[6]); ctx->f[21] = a1_flt((uint32_t)a3_xo[7]); ctx->f[22] = a1_flt((uint32_t)a3_xo[8]); ctx->f[23] = a1_flt((uint32_t)a3_xo[9]); Ps2SetGpr64(ctx, 6, a3_xo[10]); Ps2SetGpr64(ctx, 7, a3_xo[11]); Ps2SetGpr64(ctx, 8, a3_xo[12]); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001DB238_0x1db238(rdram, ctx, runtime);
}
// Sg2ExPcmInputSetAutoFilterDirect (0x1dbc10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmInputSetAutoFilterDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DBC10u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode) && A14_HOLDS(0x1DB088u, ::a14ad::kernel_sub_001DB088), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(Sg2ExPcmInputSetAutoFilterDirect, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h19 = a4::hi64(ctx->r[19]); A14_IN(122); const uint64_t a2_r = typed::Sg2ExPcmInputSetAutoFilterDirect(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st, a4_h7, a4_h19); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmInputSetAutoFilterDirect_0x1dbc10(rdram, ctx, runtime);
}
// Sg2ExPcmOutputSetAutoFilterDirect (0x1dc098): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmOutputSetAutoFilterDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DC098u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode) && A14_HOLDS(0x1DB088u, ::a14ad::kernel_sub_001DB088), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(Sg2ExPcmOutputSetAutoFilterDirect, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h18 = a4::hi64(ctx->r[18]); A14_IN(123); const uint64_t a2_r = typed::Sg2ExPcmOutputSetAutoFilterDirect(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st, a4_h7, a4_h18); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmOutputSetAutoFilterDirect_0x1dc098(rdram, ctx, runtime);
}
// shadowReInit (0x109df58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void shadowReInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109DF58u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1BEC18u, lift_d1::kernel_sub_001BEC18) && A14_HOLDS(0x1BF048u, lift_d1::iosSetDCParam) && A14_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A14_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A14_HOLDS(0x1C2058u, lift_d1::getFrameBufferSize) && A14_HOLDS(0x1C2178u, lift_d1::getZBufferAddressFBP) && A5_JAL(0x109DF80u, 0x1C2038u) && A5_JAL(0x109DF88u, 0x1C2048u) && A5_JAL(0x109DF98u, 0x1C2058u) && A5_JAL(0x109DFACu, 0x1C2178u) && A5_JAL(0x109DFD0u, 0x1BF048u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(shadowReInit, a2_2, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(125); const uint64_t a2_r = typed::shadowReInit(rdram, runtime, a2_2, a2_29, a4_hi1, a4_lo1, a4_h2, a4_h3, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); Ps2SetGpr64(ctx, 9, a3_xo[4]); Ps2SetGpr64(ctx, 10, a3_xo[5]); Ps2SetGpr64(ctx, 11, a3_xo[6]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::shadowReInit_0x109df58(rdram, ctx, runtime);
}
// manager_sub_0110EF60 (0x110ef60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0110EF60(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110EF60u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A14_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A14_HOLDS(0x110EB10u, lift_d1::manager_sub_0110EB10) && A14_HOLDS(0x110EDC8u, lift_d1::manager_sub_0110EDC8) && A14_HOLDS(0x116AD80u, lift_d1::rintf) && A5_JAL(0x110EBA8u, 0x1C2048u) && A5_JAL(0x110EBB8u, 0x1C2038u) && A5_JAL(0x110EBC8u, 0x1C2038u) && A5_JAL(0x110EBD8u, 0x1C2048u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A14_CHK(manager_sub_0110EF60, a2_4, a2_6, a2_7, a2_8, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); uint64_t a3_xo[9] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A14_IN(127); typed::manager_sub_0110EF60(rdram, runtime, a2_4, a2_6, a2_7, a2_8, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a3_xo); A14_OUT(); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[12] = a1_flt((uint32_t)a3_xo[1]); ctx->f[20] = a1_flt((uint32_t)a3_xo[2]); ctx->f[21] = a1_flt((uint32_t)a3_xo[3]); Ps2SetGpr64(ctx, 5, a3_xo[4]); Ps2SetGpr64(ctx, 6, a3_xo[5]); Ps2SetGpr64(ctx, 7, a3_xo[6]); Ps2SetGpr64(ctx, 8, a3_xo[7]); Ps2SetGpr64(ctx, 9, a3_xo[8]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::manager_sub_0110EF60_0x110ef60(rdram, ctx, runtime);
}
// ChangeGameTimeScale (0x1345d00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ChangeGameTimeScale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345D00u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A14_HOLDS(0x1345980u, lift_d1::gamecore_sub_01345980) && A5_JAL(0x13459E4u, 0x126F28u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(ChangeGameTimeScale, a2_4, a2_9, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(129); const a1::RetVF a2_r = typed::ChangeGameTimeScale(rdram, runtime, a2_4, a2_9, a2_29, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::ChangeGameTimeScale_0x1345d00(rdram, ctx, runtime);
}
// setGameAnimeTimeScaleRatio (0x1345ea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345EA0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A14_HOLDS(0x1345DB8u, lift_d1::ChangeGameAnimeTimeScaleRatio) && A5_JAL(0x1345E14u, 0x126F28u), 1)) { const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(setGameAnimeTimeScaleRatio, a2_9, a2_29); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(131); const a1::RetVF a2_r = typed::setGameAnimeTimeScaleRatio(rdram, runtime, a2_9, a2_29, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::setGameAnimeTimeScaleRatio_0x1345ea0(rdram, ctx, runtime);
}
// SCRCerberusEventLineCut (0x137b488): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCerberusEventLineCut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137B488u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x1169DE0u, lift_d1::getLineData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x1432950u, lift_d1::setEventLineAttr) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14329BCu, 0x1169DE0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRCerberusEventLineCut, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(137); const uint64_t a2_r = typed::SCRCerberusEventLineCut(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRCerberusEventLineCut_0x137b488(rdram, ctx, runtime);
}
// kernel_sub_001D24B0 (0x1d24b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D24B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D24B0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CE2C8u, lift_d1::Sg2ModMidiout1) && A14_HOLDS(0x1D28A0u, lift_d1::Sg2SepStopSndOff), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A14_CHK(kernel_sub_001D24B0, a2_4, a2_5, a2_6, a2_7, a2_29); uint32_t a4_c0st = ctx->cop0_status; A14_IN(138); const uint64_t a2_r = typed::kernel_sub_001D24B0(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::kernel_sub_001D24B0_0x1d24b0(rdram, ctx, runtime);
}
// Sg2SepAllStopSndOff (0x1d2b58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SepAllStopSndOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D2B58u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CE2C8u, lift_d1::Sg2ModMidiout1) && A14_HOLDS(0x1D28A0u, lift_d1::Sg2SepStopSndOff), 1)) { A14_CHK(Sg2SepAllStopSndOff); uint32_t a4_c0st = ctx->cop0_status; A14_IN(140); const uint64_t a2_r = typed::Sg2SepAllStopSndOff(rdram, runtime, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2SepAllStopSndOff_0x1d2b58(rdram, ctx, runtime);
}
// Sg2ExPcmInputSetAutoFilter (0x1dbd00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmInputSetAutoFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DBD00u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode) && A14_HOLDS(0x1DB088u, ::a14ad::kernel_sub_001DB088) && A14_HOLDS(0x1DB238u, ::a14ad::kernel_sub_001DB238) && A14_HOLDS(0x1DB3C8u, lift_d1::kernel_sub_001DB3C8) && A14_HOLDS(0x1DB418u, lift_d1::kernel_sub_001DB418), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(Sg2ExPcmInputSetAutoFilter, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; A14_IN(141); const uint64_t a2_r = typed::Sg2ExPcmInputSetAutoFilter(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmInputSetAutoFilter_0x1dbd00(rdram, ctx, runtime);
}
// Sg2ExPcmOutputSetAutoFilter (0x1dc160): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmOutputSetAutoFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DC160u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x10FB20u, lift_d1::DIntr) && A14_HOLDS(0x10FB70u, lift_d1::EIntr) && A14_HOLDS(0x1CC240u, lift_d1::Sg2CtrlGetTickMode) && A14_HOLDS(0x1DB088u, ::a14ad::kernel_sub_001DB088) && A14_HOLDS(0x1DB238u, ::a14ad::kernel_sub_001DB238) && A14_HOLDS(0x1DB3C8u, lift_d1::kernel_sub_001DB3C8) && A14_HOLDS(0x1DB418u, lift_d1::kernel_sub_001DB418), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A14_CHK(Sg2ExPcmOutputSetAutoFilter, a2_2, a2_4, a2_5, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23); uint32_t a4_c0st = ctx->cop0_status; A14_IN(142); const uint64_t a2_r = typed::Sg2ExPcmOutputSetAutoFilter(rdram, runtime, a2_2, a2_4, a2_5, a2_7, a2_8, a2_f1, a2_f2, a2_f3, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a2_f23, a4_c0st); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->cop0_status = a4_c0st; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::Sg2ExPcmOutputSetAutoFilter_0x1dc160(rdram, ctx, runtime);
}
// gamecore_sub_013524D8 (0x13524d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013524D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13524D8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A14_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A14_HOLDS(0x1345980u, lift_d1::gamecore_sub_01345980) && A14_HOLDS(0x1345CB8u, lift_d1::PauseGame) && A14_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x13459E4u, 0x126F28u) && A5_JAL(0x13524E0u, 0x1CA1B0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(gamecore_sub_013524D8, a2_2, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(144); const a1::RetVF a2_r = typed::gamecore_sub_013524D8(rdram, runtime, a2_2, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::gamecore_sub_013524D8_0x13524d8(rdram, ctx, runtime);
}
// gamecore_sub_01352528 (0x1352528): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352528(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352528u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A14_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A14_HOLDS(0x1345980u, lift_d1::gamecore_sub_01345980) && A14_HOLDS(0x1345CB8u, lift_d1::PauseGame) && A14_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x13459E4u, 0x126F28u) && A5_JAL(0x1352530u, 0x1CA1B0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(gamecore_sub_01352528, a2_2, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; A14_IN(145); const a1::RetVF a2_r = typed::gamecore_sub_01352528(rdram, runtime, a2_2, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_hi1, a4_lo1); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::gamecore_sub_01352528_0x1352528(rdram, ctx, runtime);
}
// GetStageCenter (0x1365f98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetStageCenter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1365F98u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x12C048u, ::vsprintf_0x12c048) && A14_HOLDS(0x1B2920u, lift_d1::iosSPrintf) && A14_HOLDS(0x1365230u, lift_d1::GetStageContext) && A5_JAL(0x1365430u, 0x1B2920u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(GetStageCenter, a2_4, a2_5, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f2, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a3_xo[10] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[2]), (uint64_t)a1_bits(ctx->f[3]), (uint64_t)a1_bits(ctx->f[4]), (uint64_t)a1_bits(ctx->f[5]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(147); const uint64_t a2_r = typed::GetStageCenter(rdram, runtime, a2_4, a2_5, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f2, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5, a4_h6, a4_h7, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[2] = a1_flt((uint32_t)a3_xo[1]); ctx->f[3] = a1_flt((uint32_t)a3_xo[2]); ctx->f[4] = a1_flt((uint32_t)a3_xo[3]); ctx->f[5] = a1_flt((uint32_t)a3_xo[4]); Ps2SetGpr64(ctx, 7, a3_xo[5]); Ps2SetGpr64(ctx, 8, a3_xo[6]); Ps2SetGpr64(ctx, 9, a3_xo[7]); Ps2SetGpr64(ctx, 10, a3_xo[8]); Ps2SetGpr64(ctx, 11, a3_xo[9]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetStageCenter_0x1365f98(rdram, ctx, runtime);
}
// GetStageName (0x1365fd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetStageName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1365FD0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128FB0u, ::strcpy_0x128fb0) && A14_HOLDS(0x12C048u, ::vsprintf_0x12c048) && A14_HOLDS(0x1B2920u, lift_d1::iosSPrintf) && A14_HOLDS(0x1365230u, lift_d1::GetStageContext) && A5_JAL(0x1365430u, 0x1B2920u) && A5_JAL(0x1365FF8u, 0x128FB0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(GetStageName, a2_4, a2_7, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(148); const uint64_t a2_r = typed::GetStageName(rdram, runtime, a2_4, a2_7, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a4_h5, a4_h6, a4_h7, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetStageName_0x1365fd0(rdram, ctx, runtime);
}
// GetStageAreaNumber (0x1366010): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetStageAreaNumber(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366010u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x12C048u, ::vsprintf_0x12c048) && A14_HOLDS(0x1B2920u, lift_d1::iosSPrintf) && A14_HOLDS(0x1365230u, lift_d1::GetStageContext) && A5_JAL(0x1365430u, 0x1B2920u), 1)) { const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(GetStageAreaNumber, a2_10, a2_11, a2_29, a2_f0, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a3_xo[8] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[3]), (uint64_t)a1_bits(ctx->f[4]), (uint64_t)a1_bits(ctx->f[5]), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(149); const uint64_t a2_r = typed::GetStageAreaNumber(rdram, runtime, a2_10, a2_11, a2_29, a2_f0, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a4_h5, a4_h6, a4_h7, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[3] = a1_flt((uint32_t)a3_xo[1]); ctx->f[4] = a1_flt((uint32_t)a3_xo[2]); ctx->f[5] = a1_flt((uint32_t)a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); Ps2SetGpr64(ctx, 10, a3_xo[6]); Ps2SetGpr64(ctx, 11, a3_xo[7]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetStageAreaNumber_0x1366010(rdram, ctx, runtime);
}
// GetStageNumberByPos (0x13660a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetStageNumberByPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13660A0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x12C048u, ::vsprintf_0x12c048) && A14_HOLDS(0x1B2920u, lift_d1::iosSPrintf) && A14_HOLDSX(0x135FF08u, lift_d1::getstagenumber) && A14_HOLDS(0x1365230u, lift_d1::GetStageContext) && A5_JAL(0x1365430u, 0x1B2920u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(GetStageNumberByPos, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); A14_IN(151); const uint64_t a2_r = typed::GetStageNumberByPos(rdram, runtime, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5, a4_h6, a4_h7); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GetStageNumberByPos_0x13660a0(rdram, ctx, runtime);
}
// SCRFuncBirdSetAttainPosForPathData (0x136e8c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBirdSetAttainPosForPathData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136E8C8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128188u, ::memcmp_0x128188) && A14_HOLDSX(0x10F1A20u, lift_d1::GetLayoutClass) && A14_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A14_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A14_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A14_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A14_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x1169D88u, lift_d1::getConnectLineArray) && A14_HOLDS(0x1169DE0u, lift_d1::getLineData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x14338E8u, ::a14ad::getPointDataFromEvent) && A5_JAL(0x136E908u, 0x10F1A20u) && A5_JAL(0x136E97Cu, 0x1169D88u) && A5_JAL(0x136E988u, 0x1169DE0u) && A5_JAL(0x136E9ACu, 0x1169D48u) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433958u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; const float a2_f30 = ctx->f[30]; const float a2_f31 = ctx->f[31]; A14_CHK(SCRFuncBirdSetAttainPosForPathData, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(153); const uint64_t a2_r = typed::SCRFuncBirdSetAttainPosForPathData(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncBirdSetAttainPosForPathData_0x136e8c8(rdram, ctx, runtime);
}
// SCRFuncBirdSetAttainNearPosForPathData (0x136efb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBirdSetAttainNearPosForPathData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136EFB8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128188u, ::memcmp_0x128188) && A14_HOLDSX(0x10F1A20u, lift_d1::GetLayoutClass) && A14_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A14_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A14_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A14_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A14_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x14338E8u, ::a14ad::getPointDataFromEvent) && A5_JAL(0x136EFE4u, 0x10F1A20u) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433958u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; const float a2_f30 = ctx->f[30]; const float a2_f31 = ctx->f[31]; A14_CHK(SCRFuncBirdSetAttainNearPosForPathData, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(154); const uint64_t a2_r = typed::SCRFuncBirdSetAttainNearPosForPathData(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a4_vf5, a4_vf6, a4_vf7, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncBirdSetAttainNearPosForPathData_0x136efb8(rdram, ctx, runtime);
}
// SCRFuncBirdSetAttainFarPosForPathData (0x136f130): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBirdSetAttainFarPosForPathData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136F130u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128188u, ::memcmp_0x128188) && A14_HOLDSX(0x10F1A20u, lift_d1::GetLayoutClass) && A14_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A14_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A14_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A14_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A14_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x14338E8u, ::a14ad::getPointDataFromEvent) && A5_JAL(0x136F15Cu, 0x10F1A20u) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433958u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; const float a2_f30 = ctx->f[30]; const float a2_f31 = ctx->f[31]; A14_CHK(SCRFuncBirdSetAttainFarPosForPathData, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(155); const uint64_t a2_r = typed::SCRFuncBirdSetAttainFarPosForPathData(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a4_vf5, a4_vf6, a4_vf7, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncBirdSetAttainFarPosForPathData_0x136f130(rdram, ctx, runtime);
}
// SCRConditionKirinIntervalOfMovement (0x139cdb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionKirinIntervalOfMovement(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139CDB8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRConditionKirinIntervalOfMovement, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(158); const a1::RetVF a2_r = typed::SCRConditionKirinIntervalOfMovement(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRConditionKirinIntervalOfMovement_0x139cdb8(rdram, ctx, runtime);
}
// SCRConditionKameNextTime (0x1404ff0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionKameNextTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1404FF0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRConditionKameNextTime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(159); const a1::RetVF a2_r = typed::SCRConditionKameNextTime(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRConditionKameNextTime_0x1404ff0(rdram, ctx, runtime);
}
// gamecore_sub_01426370 (0x1426370): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01426370(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1426370u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x126F28u, lift_d1::ps2___muldi3) && A14_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x1161C08u, lift_d1::constructor_camera_lws_anime_data) && A14_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A14_HOLDS(0x1345DB8u, lift_d1::ChangeGameAnimeTimeScaleRatio) && A14_HOLDS(0x1345E48u, lift_d1::GetGameAnimeTimeScaleRatio) && A14_HOLDS(0x1345E78u, lift_d1::RestoreDefaultGameAnimeTimeScaleRatio) && A14_HOLDS(0x1345E90u, lift_d1::SetDefaultGameAnimeTimeScaleRatio) && A14_HOLDS(0x1346240u, lift_d1::setDemoBar) && A14_HOLDS(0x134DB68u, lift_d1::setFadeParam) && A14_HOLDS(0x1352B30u, lift_d1::gameModeSet) && A14_HOLDS(0x1352B88u, lift_d1::gameModeCls) && A14_HOLDS(0x13B4DD8u, lift_d1::nico_playerSetAttentionHeadActFlag) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x141A140u, lift_d1::CAM_set_all_camera_base_data) && A14_HOLDS(0x141A7B8u, lift_d1::CAM_get_target_obj) && A14_HOLDS(0x1423410u, lift_d1::gamecore_sub_01423410) && A5_JAL(0x1343474u, 0x10C0A70u) && A5_JAL(0x1345E14u, 0x126F28u) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x142356Cu, 0x1161C08u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A14_CHK(gamecore_sub_01426370, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_29, a2_f0, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22); uint64_t a4_hi1 = ctx->hi1; uint64_t a4_lo1 = ctx->lo1; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[10] = {(uint64_t)a1_bits(ctx->f[2]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), (uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A14_IN(160); const a1::RetVF a2_r = typed::gamecore_sub_01426370(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_29, a2_f0, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a4_hi1, a4_lo1, a4_h2, a4_h3, a4_h5, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[2] = a1_flt((uint32_t)a3_xo[0]); ctx->f[12] = a1_flt((uint32_t)a3_xo[1]); ctx->f[13] = a1_flt((uint32_t)a3_xo[2]); ctx->f[14] = a1_flt((uint32_t)a3_xo[3]); ctx->f[20] = a1_flt((uint32_t)a3_xo[4]); ctx->f[21] = a1_flt((uint32_t)a3_xo[5]); ctx->f[22] = a1_flt((uint32_t)a3_xo[6]); Ps2SetGpr64(ctx, 7, a3_xo[7]); Ps2SetGpr64(ctx, 8, a3_xo[8]); Ps2SetGpr64(ctx, 9, a3_xo[9]); ctx->hi1 = a4_hi1; ctx->lo1 = a4_lo1; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::gamecore_sub_01426370_0x1426370(rdram, ctx, runtime);
}
// SCRFuncLineSetForcePrio (0x144f9d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncLineSetForcePrio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144F9D8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x1169DE0u, lift_d1::getLineData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x1432CC0u, ::a14ad::setEventLinePrio) && A14_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432D2Cu, 0x1169DE0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRFuncLineSetForcePrio, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A14_IN(166); const uint64_t a2_r = typed::SCRFuncLineSetForcePrio(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncLineSetForcePrio_0x144f9d8(rdram, ctx, runtime);
}
// SCRFuncSetAttainPointPosForLanding (0x144fe48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainPointPosForLanding(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FE48u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128188u, ::memcmp_0x128188) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x14339D8u, ::a14ad::getPointPosFromEvent) && A14_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433A44u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRFuncSetAttainPointPosForLanding, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(168); const uint64_t a2_r = typed::SCRFuncSetAttainPointPosForLanding(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncSetAttainPointPosForLanding_0x144fe48(rdram, ctx, runtime);
}
// SCRFuncSetAttainPointPosForEventID (0x1452198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainPointPosForEventID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452198u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x128188u, ::memcmp_0x128188) && A14_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A14_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A14_HOLDS(0x1169D48u, lift_d1::getPointData) && A14_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A14_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A14_HOLDS(0x1432710u, lift_d1::getPathInfo) && A14_HOLDS(0x14339D8u, ::a14ad::getPointPosFromEvent) && A14_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433A44u, 0x1169D48u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(SCRFuncSetAttainPointPosForEventID, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A14_IN(169); const uint64_t a2_r = typed::SCRFuncSetAttainPointPosForEventID(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::SCRFuncSetAttainPointPosForEventID_0x1452198(rdram, ctx, runtime);
}
// GAMEUTIL_get_add_cur_stage_ofst_position (0x13433b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_add_cur_stage_ofst_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13433B8u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x12C048u, ::vsprintf_0x12c048) && A14_HOLDS(0x1B2920u, lift_d1::iosSPrintf) && A14_HOLDS(0x1162EF0u, lift_d1::get_camera_position) && A14_HOLDS(0x1365230u, lift_d1::GetStageContext) && A14_HOLDS(0x1365F98u, ::a14ad::GetStageCenter) && A14_HOLDS(0x141CD68u, lift_d1::get_camera_obj) && A14_HOLDS(0x141D390u, lift_d1::get_main_cameraman_camera_obj) && A5_JAL(0x13433D4u, 0x1162EF0u) && A5_JAL(0x1365430u, 0x1B2920u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A14_CHK(GAMEUTIL_get_add_cur_stage_ofst_position, a2_2, a2_4, a2_5, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f2, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a3_xo[6] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A14_IN(170); const uint64_t a2_r = typed::GAMEUTIL_get_add_cur_stage_ofst_position(rdram, runtime, a2_2, a2_4, a2_5, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f2, a2_f3, a2_f4, a2_f5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf5, a4_vf6, a4_h2, a4_h3, a4_h4, a4_h5, a4_h6, a4_h7, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::GAMEUTIL_get_add_cur_stage_ofst_position_0x13433b8(rdram, ctx, runtime);
}
// get_apply_camera_region (0x1163ef0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_apply_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1163EF0u, 1))
    {
        if (PS2_EXPECT(A14_GO() && A14_HOLDS(0x1B9238u, lift_d1::iosInverseMatrix) && A14_HOLDS(0x1B9A58u, lift_d1::iosRotMatrixY) && A14_HOLDS(0x1B9AC8u, lift_d1::iosRotMatrixZ) && A14_HOLDS(0x1BDD78u, lift_d1::iosGetAlignSinCosf) && A14_HOLDS(0x1BDDB0u, ::decomp_math::getSinCos) && A14_HOLDS(0x1BE898u, ::decomp_quat::absVector) && A14_HOLDS(0x107EC70u, lift_d1::GAMEUTIL_is_nearly_equal_zero3D) && A14_HOLDSX(0x10F1A20u, lift_d1::GetLayoutClass) && A14_HOLDS(0x10F3420u, lift_d1::GetLayoutParentObj) && A14_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A14_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A14_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A14_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A14_HOLDS(0x1135DD8u, lift_d1::IsInsideOfRegion) && A14_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A14_HOLDS(0x1162EF8u, lift_d1::get_camera_direction) && A14_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A14_HOLDS(0x1162FD8u, ::a11ad::manager_sub_01162FD8) && A14_HOLDS(0x1163618u, ::a11ad::is_clear_camera_region_condition) && A14_HOLDS(0x1163788u, lift_d1::is_parent_obj_boss_camera_region) && A14_HOLDSX(0x11638B8u, lift_d1::is_have_region_camera_region) && A14_HOLDS(0x11641E0u, lift_d1::manager_sub_011641E0) && A14_HOLDS(0x1164260u, lift_d1::manager_sub_01164260) && A5_JAL(0x1135EDCu, 0x1B9238u) && A5_JAL(0x1135FECu, 0x1B9AC8u) && A5_JAL(0x1136034u, 0x1B9A58u) && A5_JAL(0x11360B0u, 0x1B9238u) && A5_JAL(0x1136174u, 0x1B9238u) && A5_JAL(0x11361ACu, 0x1BE898u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; const float a2_f30 = ctx->f[30]; const float a2_f31 = ctx->f[31]; A14_CHK(get_apply_camera_region, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf1 = ctx->vu0_vf[1]; a4::vec4 a4_vf2 = ctx->vu0_vf[2]; a4::vec4 a4_vf3 = ctx->vu0_vf[3]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf9 = ctx->vu0_vf[9]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf24 = ctx->vu0_vf[24]; a4::vec4 a4_vf25 = ctx->vu0_vf[25]; a4::vec4 a4_vf26 = ctx->vu0_vf[26]; a4::vec4 a4_vf27 = ctx->vu0_vf[27]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h9 = a4::hi64(ctx->r[9]); uint64_t a4_h10 = a4::hi64(ctx->r[10]); uint64_t a4_h11 = a4::hi64(ctx->r[11]); uint64_t a4_h12 = a4::hi64(ctx->r[12]); uint64_t a4_h13 = a4::hi64(ctx->r[13]); uint64_t a4_h14 = a4::hi64(ctx->r[14]); uint64_t a4_h15 = a4::hi64(ctx->r[15]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23])}; A14_IN(172); const uint64_t a2_r = typed::get_apply_camera_region(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a4_vf0, a4_vf1, a4_vf2, a4_vf3, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf9, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf24, a4_vf25, a4_vf26, a4_vf27, a4_vacc, a4_h2, a4_h3, a4_h6, a4_h7, a4_h8, a4_h9, a4_h10, a4_h11, a4_h12, a4_h13, a4_h14, a4_h15, a3_xo); A14_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); ctx->f[22] = a1_flt((uint32_t)a3_xo[2]); ctx->f[23] = a1_flt((uint32_t)a3_xo[3]); ctx->vu0_vf[1] = a4_vf1; ctx->vu0_vf[2] = a4_vf2; ctx->vu0_vf[3] = a4_vf3; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[9] = a4_vf9; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[24] = a4_vf24; ctx->vu0_vf[25] = a4_vf25; ctx->vu0_vf[26] = a4_vf26; ctx->vu0_vf[27] = a4_vf27; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->r[8] = a4::mk128(a4_h8, Ps2GprU64(ctx, 8)); ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->r[10] = a4::mk128(a4_h10, Ps2GprU64(ctx, 10)); ctx->r[11] = a4::mk128(a4_h11, Ps2GprU64(ctx, 11)); ctx->r[12] = a4::mk128(a4_h12, Ps2GprU64(ctx, 12)); ctx->r[13] = a4::mk128(a4_h13, Ps2GprU64(ctx, 13)); ctx->r[14] = a4::mk128(a4_h14, Ps2GprU64(ctx, 14)); ctx->r[15] = a4::mk128(a4_h15, Ps2GprU64(ctx, 15)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a14
    }
    ::get_apply_camera_region_0x1163ef0(rdram, ctx, runtime);
}
}

namespace
{
    struct A14Ad
    {
        uint32_t addr;
        PS2Runtime::RecompiledFunction rec, ad;
    };
    const A14Ad kA14Ad[95] = {
        {0x101AC0u, &::sub_00101AC0_0x101ac0, &a14ad::sub_00101AC0},
        {0x107460u, &::QueryIntrContext_0x107460, &a14ad::QueryIntrContext},
        {0x109288u, &::sub_00109288_0x109288, &a14ad::sub_00109288},
        {0x123A18u, &::ps2___moddi3_0x123a18, &a14ad::ps2___moddi3},
        {0x12B1D8u, &::sub_0012B1D8_0x12b1d8, &a14ad::sub_0012B1D8},
        {0x1B3F68u, &::Shock_SetMotor_0x1b3f68, &a14ad::Shock_SetMotor},
        {0x1BE858u, &::iosGetDRandomVector_0x1be858, &a14ad::iosGetDRandomVector},
        {0x1C2198u, &::getTextureAreaAddressTBP_0x1c2198, &a14ad::getTextureAreaAddressTBP},
        {0x1C21D0u, &::getTextureAreaAddressFBP_0x1c21d0, &a14ad::getTextureAreaAddressFBP},
        {0x1C90E8u, &::iosGetFontDataWithID_0x1c90e8, &a14ad::iosGetFontDataWithID},
        {0x1D6578u, &::Sg2SlotSetPitch_0x1d6578, &a14ad::Sg2SlotSetPitch},
        {0x1DA790u, &::kernel_sub_001DA790_0x1da790, &a14ad::kernel_sub_001DA790},
        {0x1DA850u, &::kernel_sub_001DA850_0x1da850, &a14ad::kernel_sub_001DA850},
        {0x1DA8F8u, &::kernel_sub_001DA8F8_0x1da8f8, &a14ad::kernel_sub_001DA8F8},
        {0x1DA9C8u, &::kernel_sub_001DA9C8_0x1da9c8, &a14ad::kernel_sub_001DA9C8},
        {0x1DBA10u, &::Sg2ExPcmInputGetRealPlayFrameCount_0x1dba10, &a14ad::Sg2ExPcmInputGetRealPlayFrameCount},
        {0x11023F0u, &::objectmanager_mallocerrorfunc_0x11023f0, &a14ad::objectmanager_mallocerrorfunc},
        {0x1358958u, &::getStageAreaIndexByPos_0x1358958, &a14ad::getStageAreaIndexByPos},
        {0x135D760u, &::getStageAreaID_0x135d760, &a14ad::getStageAreaID},
        {0x135D840u, &::getStageAreaName_0x135d840, &a14ad::getStageAreaName},
        {0x135D930u, &::getStageAreaNameUV_0x135d930, &a14ad::getStageAreaNameUV},
        {0x13667B8u, &::stagemanager_getCenterFromAreaId_0x13667b8, &a14ad::stagemanager_getCenterFromAreaId},
        {0x1091C0u, &::sub_001091C0_0x1091c0, &a14ad::sub_001091C0},
        {0x1092B8u, &::sub_001092B8_0x1092b8, &a14ad::sub_001092B8},
        {0x110CC8u, &::GetTimerSystemTime_0x110cc8, &a14ad::GetTimerSystemTime},
        {0x110DB8u, &::AllocTimerCounter_0x110db8, &a14ad::AllocTimerCounter},
        {0x110F40u, &::GetTimerUsedUnusedCounters_0x110f40, &a14ad::GetTimerUsedUnusedCounters},
        {0x1111B8u, &::SetTimerCount_0x1111b8, &a14ad::SetTimerCount},
        {0x1112D0u, &::GetTimerBaseTime_0x1112d0, &a14ad::GetTimerBaseTime},
        {0x111558u, &::TimerBusClock2USec_0x111558, &a14ad::TimerBusClock2USec},
        {0x111600u, &::TimerUSec2BusClock_0x111600, &a14ad::TimerUSec2BusClock},
        {0x111B18u, &::sceSifSetRebootNotifyBuffer_0x111b18, &a14ad::sceSifSetRebootNotifyBuffer},
        {0x1199F8u, &::sub_001199F8_0x1199f8, &a14ad::sub_001199F8},
        {0x12C240u, &::sub_0012C240_0x12c240, &a14ad::sub_0012C240},
        {0x1B7590u, &::kernel_sub_001B7590_0x1b7590, &a14ad::kernel_sub_001B7590},
        {0x1CC030u, &::Sg2CtrlEntryTickProc_0x1cc030, &a14ad::Sg2CtrlEntryTickProc},
        {0x1CC108u, &::Sg2CtrlRemoveTickProc_0x1cc108, &a14ad::Sg2CtrlRemoveTickProc},
        {0x1CC458u, &::Sg2DspEffectSetDepth2_0x1cc458, &a14ad::Sg2DspEffectSetDepth2},
        {0x1CC4D0u, &::Sg2DspEffectSetDelay_0x1cc4d0, &a14ad::Sg2DspEffectSetDelay},
        {0x1CC530u, &::Sg2DspEffectSetFeedBack_0x1cc530, &a14ad::Sg2DspEffectSetFeedBack},
        {0x1CE350u, &::Sg2ModMidiout1Exclusive_0x1ce350, &a14ad::Sg2ModMidiout1Exclusive},
        {0x1CE3F0u, &::Sg2ModMidiout_0x1ce3f0, &a14ad::Sg2ModMidiout},
        {0x1CE898u, &::Sg2ModOpenBgm_0x1ce898, &a14ad::Sg2ModOpenBgm},
        {0x1CEA68u, &::Sg2ModOpenSe_0x1cea68, &a14ad::Sg2ModOpenSe},
        {0x1D0E50u, &::kernel_sub_001D0E50_0x1d0e50, &a14ad::kernel_sub_001D0E50},
        {0x1D6D90u, &::Sg2SlotSetDumperOff_0x1d6d90, &a14ad::Sg2SlotSetDumperOff},
        {0x1D6E70u, &::Sg2SlotModKeyOff_0x1d6e70, &a14ad::Sg2SlotModKeyOff},
        {0x1D72A8u, &::Sg2SlotSetReverbOnBit_0x1d72a8, &a14ad::Sg2SlotSetReverbOnBit},
        {0x1D7310u, &::Sg2SlotSetReverbOffBit_0x1d7310, &a14ad::Sg2SlotSetReverbOffBit},
        {0x1D7380u, &::Sg2SlotSetNoiseOnBit_0x1d7380, &a14ad::Sg2SlotSetNoiseOnBit},
        {0x1DB088u, &::kernel_sub_001DB088_0x1db088, &a14ad::kernel_sub_001DB088},
        {0x1DB950u, &::Sg2ExPcmInputCtrl_0x1db950, &a14ad::Sg2ExPcmInputCtrl},
        {0x1346090u, &::gamecore_sub_01346090_0x1346090, &a14ad::gamecore_sub_01346090},
        {0x1361E88u, &::gamecore_sub_01361E88_0x1361e88, &a14ad::gamecore_sub_01361E88},
        {0x13997F0u, &::SpriteInitStatusClut_0x13997f0, &a14ad::SpriteInitStatusClut},
        {0x1399AA0u, &::SpriteSetStatusClut_0x1399aa0, &a14ad::SpriteSetStatusClut},
        {0x1432B30u, &::resetEventLineAttr_0x1432b30, &a14ad::resetEventLineAttr},
        {0x1432CC0u, &::setEventLinePrio_0x1432cc0, &a14ad::setEventLinePrio},
        {0x14338E8u, &::getPointDataFromEvent_0x14338e8, &a14ad::getPointDataFromEvent},
        {0x14339D8u, &::getPointPosFromEvent_0x14339d8, &a14ad::getPointPosFromEvent},
        {0x1CD2B8u, &::Sg2ExStAdpcmTrkDspSw_0x1cd2b8, &a14ad::Sg2ExStAdpcmTrkDspSw},
        {0x1CFB28u, &::kernel_sub_001CFB28_0x1cfb28, &a14ad::kernel_sub_001CFB28},
        {0x1D1360u, &::kernel_sub_001D1360_0x1d1360, &a14ad::kernel_sub_001D1360},
        {0x1D1778u, &::kernel_sub_001D1778_0x1d1778, &a14ad::kernel_sub_001D1778},
        {0x1D6118u, &::Sg2SlotSetBasicParam_0x1d6118, &a14ad::Sg2SlotSetBasicParam},
        {0x1DAC50u, &::kernel_sub_001DAC50_0x1dac50, &a14ad::kernel_sub_001DAC50},
        {0x1DB238u, &::kernel_sub_001DB238_0x1db238, &a14ad::kernel_sub_001DB238},
        {0x1DBC10u, &::Sg2ExPcmInputSetAutoFilterDirect_0x1dbc10, &a14ad::Sg2ExPcmInputSetAutoFilterDirect},
        {0x1DC098u, &::Sg2ExPcmOutputSetAutoFilterDirect_0x1dc098, &a14ad::Sg2ExPcmOutputSetAutoFilterDirect},
        {0x109DF58u, &::shadowReInit_0x109df58, &a14ad::shadowReInit},
        {0x110EF60u, &::manager_sub_0110EF60_0x110ef60, &a14ad::manager_sub_0110EF60},
        {0x1345D00u, &::ChangeGameTimeScale_0x1345d00, &a14ad::ChangeGameTimeScale},
        {0x1345EA0u, &::setGameAnimeTimeScaleRatio_0x1345ea0, &a14ad::setGameAnimeTimeScaleRatio},
        {0x137B488u, &::SCRCerberusEventLineCut_0x137b488, &a14ad::SCRCerberusEventLineCut},
        {0x1D24B0u, &::kernel_sub_001D24B0_0x1d24b0, &a14ad::kernel_sub_001D24B0},
        {0x1D2B58u, &::Sg2SepAllStopSndOff_0x1d2b58, &a14ad::Sg2SepAllStopSndOff},
        {0x1DBD00u, &::Sg2ExPcmInputSetAutoFilter_0x1dbd00, &a14ad::Sg2ExPcmInputSetAutoFilter},
        {0x1DC160u, &::Sg2ExPcmOutputSetAutoFilter_0x1dc160, &a14ad::Sg2ExPcmOutputSetAutoFilter},
        {0x13524D8u, &::gamecore_sub_013524D8_0x13524d8, &a14ad::gamecore_sub_013524D8},
        {0x1352528u, &::gamecore_sub_01352528_0x1352528, &a14ad::gamecore_sub_01352528},
        {0x1365F98u, &::GetStageCenter_0x1365f98, &a14ad::GetStageCenter},
        {0x1365FD0u, &::GetStageName_0x1365fd0, &a14ad::GetStageName},
        {0x1366010u, &::GetStageAreaNumber_0x1366010, &a14ad::GetStageAreaNumber},
        {0x13660A0u, &::GetStageNumberByPos_0x13660a0, &a14ad::GetStageNumberByPos},
        {0x136E8C8u, &::SCRFuncBirdSetAttainPosForPathData_0x136e8c8, &a14ad::SCRFuncBirdSetAttainPosForPathData},
        {0x136EFB8u, &::SCRFuncBirdSetAttainNearPosForPathData_0x136efb8, &a14ad::SCRFuncBirdSetAttainNearPosForPathData},
        {0x136F130u, &::SCRFuncBirdSetAttainFarPosForPathData_0x136f130, &a14ad::SCRFuncBirdSetAttainFarPosForPathData},
        {0x139CDB8u, &::SCRConditionKirinIntervalOfMovement_0x139cdb8, &a14ad::SCRConditionKirinIntervalOfMovement},
        {0x1404FF0u, &::SCRConditionKameNextTime_0x1404ff0, &a14ad::SCRConditionKameNextTime},
        {0x1426370u, &::gamecore_sub_01426370_0x1426370, &a14ad::gamecore_sub_01426370},
        {0x144F9D8u, &::SCRFuncLineSetForcePrio_0x144f9d8, &a14ad::SCRFuncLineSetForcePrio},
        {0x144FE48u, &::SCRFuncSetAttainPointPosForLanding_0x144fe48, &a14ad::SCRFuncSetAttainPointPosForLanding},
        {0x1452198u, &::SCRFuncSetAttainPointPosForEventID_0x1452198, &a14ad::SCRFuncSetAttainPointPosForEventID},
        {0x13433B8u, &::GAMEUTIL_get_add_cur_stage_ofst_position_0x13433b8, &a14ad::GAMEUTIL_get_add_cur_stage_ofst_position},
        {0x1163EF0u, &::get_apply_camera_region_0x1163ef0, &a14ad::get_apply_camera_region},
    };
}

// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function
uint32_t a14InstallAdapters(PS2Runtime &runtime)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < 95u; ++i)
        if (runtime.lookupFunction(kA14Ad[i].addr) == kA14Ad[i].rec)
        {
            runtime.registerFunction(kA14Ad[i].addr, kA14Ad[i].ad);
            ++n;
        }
    return n;
}
