// typed_a7_adapters.cpp: round A7's adapters for the never-lifted functions stage 7 typed (tools/typed_lift2.py;
// docs/decomp_a7.md). Generated: do not edit. Each takes the recompiled function's slot at install (a7InstallAdapters, from
// sotc_decomp::install after A6's, only while the slot still holds the recompiled function): the typed function while the
// switch a7typed is on and every slot it reaches holds what its typed callee stands for (A7_GO / A7_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a7.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a7ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

namespace a7ad
{
// LoaderSysPutString (0x102fe8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void LoaderSysPutString(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x102FE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1285C0u, ::printf_0x1285c0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(LoaderSysPutString, a2_4, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(5); const uint32_t a2_r = typed::LoaderSysPutString(rdram, runtime, a2_4, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::LoaderSysPutString_0x102fe8(rdram, ctx, runtime);
}
// sub_001155D8 (0x1155d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001155D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155D8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1283D0u, ::memset_0x1283d0), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sub_001155D8, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(7); const uint64_t a2_r = typed::sub_001155D8(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sub_001155D8_0x1155d8(rdram, ctx, runtime);
}
// sub_00118438 (0x118438): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_00118438(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x118438u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x12821Cu, ::memcpy_0x12821c), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sub_00118438, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(8); const uint64_t a2_r = typed::sub_00118438(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sub_00118438_0x118438(rdram, ctx, runtime);
}
// sub_00118490 (0x118490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_00118490(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x118490u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x12821Cu, ::memcpy_0x12821c), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sub_00118490, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(9); const uint64_t a2_r = typed::sub_00118490(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sub_00118490_0x118490(rdram, ctx, runtime);
}
// sub_001266F0 (0x1266f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001266F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1266F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x125998u, lift_d1::sub_00125998) && A7_HOLDS(0x1259E0u, lift_d1::sub_001259E0) && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sub_001266F0, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A7_IN(11); const uint64_t a2_r = typed::sub_001266F0(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sub_001266F0_0x1266f0(rdram, ctx, runtime);
}
// _iosAssert (0x1ac998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _iosAssert(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1AC998u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(_iosAssert, a2_4, a2_5, a2_6, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(13); const uint64_t a2_r = typed::_iosAssert(rdram, runtime, a2_4, a2_5, a2_6, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::_iosAssert_0x1ac998(rdram, ctx, runtime);
}
// kernel_sub_001B6BF8 (0x1b6bf8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001B6BF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B6BF8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(kernel_sub_001B6BF8, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(18); const uint64_t a2_r = typed::kernel_sub_001B6BF8(rdram, runtime, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::kernel_sub_001B6BF8_0x1b6bf8(rdram, ctx, runtime);
}
// SetQuaternionByAxisRotateWithNoRegularize (0x1bbca8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetQuaternionByAxisRotateWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BBCA8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1BE520u, lift_d1::iosGetTableSin) && A7_HOLDS(0x1BE570u, lift_d1::iosGetTableCos), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A7_CHK(SetQuaternionByAxisRotateWithNoRegularize, a2_4, a2_5, a2_29, a2_f0, a2_f12, a2_f13, a2_f14); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(22); const a1::RetVF a2_r = typed::SetQuaternionByAxisRotateWithNoRegularize(rdram, runtime, a2_4, a2_5, a2_29, a2_f0, a2_f12, a2_f13, a2_f14, a4_vf5, a4_vf6, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SetQuaternionByAxisRotateWithNoRegularize_0x1bbca8(rdram, ctx, runtime);
}
// gsRegBegin (0x1c39e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gsRegBegin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C39E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1BFBA8u, lift_d1::iosGetPacketPtrPath1), 1)) { A7_CHK(gsRegBegin); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A7_IN(24); const uint32_t a2_r = typed::gsRegBegin(rdram, runtime, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gsRegBegin_0x1c39e8(rdram, ctx, runtime);
}
// all_soundpause (0x1ce1b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void all_soundpause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CE1B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1D5F88u, lift_d1::Sg2SlotFindSlotClass) && A7_HOLDS(0x1D65F0u, lift_d1::Sg2SlotSetPause), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(all_soundpause, a2_4, a2_5, a2_29); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A7_IN(30); const uint64_t a2_r = typed::all_soundpause(rdram, runtime, a2_4, a2_5, a2_29, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::all_soundpause_0x1ce1b8(rdram, ctx, runtime);
}
// all_soundmute (0x1ce240): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void all_soundmute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CE240u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1D5F88u, lift_d1::Sg2SlotFindSlotClass) && A7_HOLDS(0x1D6270u, lift_d1::Sg2SlotSetMute), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(all_soundmute, a2_4, a2_5, a2_29); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A7_IN(31); const uint64_t a2_r = typed::all_soundmute(rdram, runtime, a2_4, a2_5, a2_29, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::all_soundmute_0x1ce240(rdram, ctx, runtime);
}
// kernel_sub_001D0DB0 (0x1d0db0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D0DB0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D0DB0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1D5FD8u, lift_d1::Sg2SlotFindSlotClassCh) && A7_HOLDS(0x1D68A8u, lift_d1::Sg2SlotSetLfoPitchDepth), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(kernel_sub_001D0DB0, a2_29); A7_IN(33); const uint64_t a2_r = typed::kernel_sub_001D0DB0(rdram, runtime, a2_29); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::kernel_sub_001D0DB0_0x1d0db0(rdram, ctx, runtime);
}
// kernel_sub_001D18E0 (0x1d18e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001D18E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D18E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1D5FD8u, lift_d1::Sg2SlotFindSlotClassCh) && A7_HOLDS(0x1D6548u, lift_d1::Sg2SlotSetBendValue), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(kernel_sub_001D18E0, a2_29); A7_IN(34); const uint64_t a2_r = typed::kernel_sub_001D18E0(rdram, runtime, a2_29); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::kernel_sub_001D18E0_0x1d18e0(rdram, ctx, runtime);
}
// kernel_sub_001DAB10 (0x1dab10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DAB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DAB10u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1DCB48u, lift_d1::IirFilter), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(kernel_sub_001DAB10, a2_4, a2_5, a2_29); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), Ps2GprU64(ctx, 6)}; A7_IN(38); const uint64_t a2_r = typed::kernel_sub_001DAB10(rdram, runtime, a2_4, a2_5, a2_29, a4_h2, a4_h3, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[12] = a1_flt((uint32_t)a3_xo[1]); ctx->f[13] = a1_flt((uint32_t)a3_xo[2]); ctx->f[14] = a1_flt((uint32_t)a3_xo[3]); Ps2SetGpr64(ctx, 6, a3_xo[4]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::kernel_sub_001DAB10_0x1dab10(rdram, ctx, runtime);
}
// glScalef (0x108eec8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glScalef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108EEC8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1283D0u, ::memset_0x1283d0) && A5_JAL(0x108EEF0u, 0x1283D0u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A7_CHK(glScalef, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22); a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf24 = ctx->vu0_vf[24]; a4::vec4 a4_vf25 = ctx->vu0_vf[25]; a4::vec4 a4_vf26 = ctx->vu0_vf[26]; a4::vec4 a4_vf27 = ctx->vu0_vf[27]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(45); const a1::RetVF a2_r = typed::glScalef(rdram, runtime, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf24, a4_vf25, a4_vf26, a4_vf27, a4_vacc, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[24] = a4_vf24; ctx->vu0_vf[25] = a4_vf25; ctx->vu0_vf[26] = a4_vf26; ctx->vu0_vf[27] = a4_vf27; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::glScalef_0x108eec8(rdram, ctx, runtime);
}
// GetAnimationClassCurrentInfoWithOutsideLimit (0x10ba970): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetAnimationClassCurrentInfoWithOutsideLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA970u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10B6C78u, ::a6ad::GetMotionFrameWithOutsideLimit), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A7_CHK(GetAnimationClassCurrentInfoWithOutsideLimit, a2_2, a2_4, a2_5, a2_f12, a2_f13); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A7_IN(56); const a1::RetVF a2_r = typed::GetAnimationClassCurrentInfoWithOutsideLimit(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetAnimationClassCurrentInfoWithOutsideLimit_0x10ba970(rdram, ctx, runtime);
}
// SetAnimationClassLastInfoDirectResource (0x10baa80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetAnimationClassLastInfoDirectResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BAA80u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x11692A8u, lift_d1::ResetAnimeFrameTableInfo), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A7_CHK(SetAnimationClassLastInfoDirectResource, a2_4, a2_5, a2_7); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A7_IN(58); const a1::RetVF a2_r = typed::SetAnimationClassLastInfoDirectResource(rdram, runtime, a2_4, a2_5, a2_7, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SetAnimationClassLastInfoDirectResource_0x10baa80(rdram, ctx, runtime);
}
// GetChildGroupManagerClass (0x10eb918): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetChildGroupManagerClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10EB918u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D868u, lift_d1::GetScrLwsOriClass), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A7_CHK(GetChildGroupManagerClass, a2_4); A7_IN(68); const uint64_t a2_r = typed::GetChildGroupManagerClass(rdram, runtime, a2_4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetChildGroupManagerClass_0x10eb918(rdram, ctx, runtime);
}
// GetCollisionDynamicsProjectionFloatPosition (0x1129a38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCollisionDynamicsProjectionFloatPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129A38u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1134440u, lift_d1::GetPlaneFittingPointForUnnormalizedPlane), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; A7_CHK(GetCollisionDynamicsProjectionFloatPosition, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(82); const a1::RetVF a2_r = typed::GetCollisionDynamicsProjectionFloatPosition(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f20, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetCollisionDynamicsProjectionFloatPosition_0x1129a38(rdram, ctx, runtime);
}
// nicosysdSendDataWrite (0x115a7e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nicosysdSendDataWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115A7E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x12821Cu, ::memcpy_0x12821c) && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A5_JAL(0x115A814u, 0x12821Cu) && A5_JAL(0x115A83Cu, 0x1B3410u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nicosysdSendDataWrite, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(86); const uint64_t a2_r = typed::nicosysdSendDataWrite(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nicosysdSendDataWrite_0x115a7e8(rdram, ctx, runtime);
}
// getEditWorkSize (0x1169bf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getEditWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1169BF0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A5_JAL(0x1169C18u, 0x1B3410u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getEditWorkSize, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(95); const uint64_t a2_r = typed::getEditWorkSize(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getEditWorkSize_0x1169bf0(rdram, ctx, runtime);
}
// DebugDispTargetStatusForce (0x1341180): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugDispTargetStatusForce(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341180u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x1341188u, 0x10C0A70u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(DebugDispTargetStatusForce, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(96); const uint64_t a2_r = typed::DebugDispTargetStatusForce(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::DebugDispTargetStatusForce_0x1341180(rdram, ctx, runtime);
}
// GAMEUTIL_is_player_obj_standing_on_horse (0x13434b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_player_obj_standing_on_horse(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13434B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x13434C4u, 0x10C0A70u) && A5_JAL(0x13434F4u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GAMEUTIL_is_player_obj_standing_on_horse, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 5)}; A7_IN(98); const uint64_t a2_r = typed::GAMEUTIL_is_player_obj_standing_on_horse(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GAMEUTIL_is_player_obj_standing_on_horse_0x13434b0(rdram, ctx, runtime);
}
// nico_setFruitsFlag (0x134e418): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setFruitsFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134E418u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110D868u, lift_d1::GetScrLwsOriClass) && A7_HOLDS(0x134F150u, lift_d1::fruitsFlagSet) && A5_JAL(0x134E420u, 0x110D868u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A7_CHK(nico_setFruitsFlag, a2_2, a2_4, a2_7); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A7_IN(114); const uint64_t a2_r = typed::nico_setFruitsFlag(rdram, runtime, a2_2, a2_4, a2_7, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_setFruitsFlag_0x134e418(rdram, ctx, runtime);
}
// nico_checkTreeAllFruitsAlreadyTaken (0x134e4d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_checkTreeAllFruitsAlreadyTaken(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134E4D8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134F1B0u, lift_d1::fruitsFlagChk), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A7_CHK(nico_checkTreeAllFruitsAlreadyTaken, a2_2, a2_4, a2_7); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A7_IN(115); const uint64_t a2_r = typed::nico_checkTreeAllFruitsAlreadyTaken(rdram, runtime, a2_2, a2_4, a2_7, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_checkTreeAllFruitsAlreadyTaken_0x134e4d8(rdram, ctx, runtime);
}
// gStatOmakeSave (0x1353410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatOmakeSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353410u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134E8A8u, lift_d1::gameFlagCheckPointSave), 1)) { const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(gStatOmakeSave, a2_10, a2_11, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[7] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A7_IN(119); const uint64_t a2_r = typed::gStatOmakeSave(rdram, runtime, a2_10, a2_11, a2_29, a4_h3, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); Ps2SetGpr64(ctx, 9, a3_xo[4]); Ps2SetGpr64(ctx, 10, a3_xo[5]); Ps2SetGpr64(ctx, 11, a3_xo[6]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gStatOmakeSave_0x1353410(rdram, ctx, runtime);
}
// gStatLoadAdvertiseHorseChara (0x1353bf8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatLoadAdvertiseHorseChara(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353BF8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134E530u, lift_d1::_omakeSelectFlagSet) && A7_HOLDS(0x134FB28u, lift_d1::_omakeSelectFlagChk), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(gStatLoadAdvertiseHorseChara, a2_4, a2_9, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[6] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A7_IN(121); const uint64_t a2_r = typed::gStatLoadAdvertiseHorseChara(rdram, runtime, a2_4, a2_9, a2_29, a4_h3, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gStatLoadAdvertiseHorseChara_0x1353bf8(rdram, ctx, runtime);
}
// gStatUnloadAdvertiseHorseChara (0x1353c80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatUnloadAdvertiseHorseChara(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353C80u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134E530u, lift_d1::_omakeSelectFlagSet) && A7_HOLDS(0x134FA48u, ::a6ad::_omakeSelectFlagCls) && A7_HOLDS(0x134FB28u, lift_d1::_omakeSelectFlagChk), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(gStatUnloadAdvertiseHorseChara, a2_4, a2_9, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[6] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A7_IN(122); const uint64_t a2_r = typed::gStatUnloadAdvertiseHorseChara(rdram, runtime, a2_4, a2_9, a2_29, a4_h3, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gStatUnloadAdvertiseHorseChara_0x1353c80(rdram, ctx, runtime);
}
// time_attack_set_best_time (0x1367e40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void time_attack_set_best_time(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367E40u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134F260u, ::a6ad::setBossIndexTimeAttackBestTime), 1)) { const float a2_f12 = ctx->f[12]; A7_CHK(time_attack_set_best_time, a2_f12); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 1), Ps2GprU64(ctx, 2)}; A7_IN(128); typed::time_attack_set_best_time(rdram, runtime, a2_f12, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 1, a3_xo[0]); Ps2SetGpr64(ctx, 2, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::time_attack_set_best_time_0x1367e40(rdram, ctx, runtime);
}
// SCRCharaTurn (0x137c728): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCharaTurn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C728u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x137C740u, 0x10C0A70u) && A5_JAL(0x137C76Cu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(SCRCharaTurn, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21])}; A7_IN(129); const a1::RetVF a2_r = typed::SCRCharaTurn(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRCharaTurn_0x137c728(rdram, ctx, runtime);
}
// SCRPlayableCharaTurnDirectInput (0x137c7c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayableCharaTurnDirectInput(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C7C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x137C7CCu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRPlayableCharaTurnDirectInput, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A7_IN(130); const uint64_t a2_r = typed::SCRPlayableCharaTurnDirectInput(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRPlayableCharaTurnDirectInput_0x137c7c0(rdram, ctx, runtime);
}
// SCRCharaAccel (0x137c7f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCharaAccel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C7F8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x137C80Cu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRCharaAccel, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[20]), Ps2GprU64(ctx, 5)}; A7_IN(131); const a1::RetVF a2_r = typed::SCRCharaAccel(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRCharaAccel_0x137c7f8(rdram, ctx, runtime);
}
// SCRCharaStand (0x137c8b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCharaStand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C8B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x137C8C0u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRCharaStand, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(133); const uint64_t a2_r = typed::SCRCharaStand(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRCharaStand_0x137c8b8(rdram, ctx, runtime);
}
// SCRGetDir (0x137c950): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C950u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x137C960u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRGetDir, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(134); const uint64_t a2_r = typed::SCRGetDir(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRGetDir_0x137c950(rdram, ctx, runtime);
}
// gamecore_sub_0138C1C8 (0x138c1c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138C1C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138C1C8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110D868u, lift_d1::GetScrLwsOriClass) && A5_JAL(0x138C1D0u, 0x110D868u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A7_CHK(gamecore_sub_0138C1C8, a2_2, a2_4); A7_IN(139); const uint64_t a2_r = typed::gamecore_sub_0138C1C8(rdram, runtime, a2_2, a2_4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_0138C1C8_0x138c1c8(rdram, ctx, runtime);
}
// gamecore_sub_0138C1F0 (0x138c1f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138C1F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138C1F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110D868u, lift_d1::GetScrLwsOriClass) && A5_JAL(0x138C1F8u, 0x110D868u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A7_CHK(gamecore_sub_0138C1F0, a2_2, a2_4); A7_IN(140); const uint64_t a2_r = typed::gamecore_sub_0138C1F0(rdram, runtime, a2_2, a2_4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_0138C1F0_0x138c1f0(rdram, ctx, runtime);
}
// gamecore_sub_0138CB48 (0x138cb48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138CB48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138CB48u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x13D3DA0u, lift_d1::AI_UpdateVelocity) && A7_HOLDS(0x13D4A28u, lift_d1::AI_GetInterRatio), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f20 = ctx->f[20]; A7_CHK(gamecore_sub_0138CB48, a2_4, a2_5, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a3_xo[7] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), (uint64_t)a1_bits(ctx->f[15]), (uint64_t)a1_bits(ctx->f[16]), (uint64_t)a1_bits(ctx->f[20])}; A7_IN(141); const uint32_t a2_r = typed::gamecore_sub_0138CB48(rdram, runtime, a2_4, a2_5, a2_f20, a4_vf0, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[12] = a1_flt((uint32_t)a3_xo[1]); ctx->f[13] = a1_flt((uint32_t)a3_xo[2]); ctx->f[14] = a1_flt((uint32_t)a3_xo[3]); ctx->f[15] = a1_flt((uint32_t)a3_xo[4]); ctx->f[16] = a1_flt((uint32_t)a3_xo[5]); ctx->f[20] = a1_flt((uint32_t)a3_xo[6]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_0138CB48_0x138cb48(rdram, ctx, runtime);
}
// player_IsHorseHang (0x13b51b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_IsHorseHang(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B51B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x13B51B8u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(player_IsHorseHang, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(144); const uint64_t a2_r = typed::player_IsHorseHang(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::player_IsHorseHang_0x13b51b0(rdram, ctx, runtime);
}
// gamecore_sub_013C8748 (0x13c8748): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013C8748(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13C8748u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x138BE30u, lift_d1::set_node_data_expression), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A7_CHK(gamecore_sub_013C8748, a2_4, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 2)}; A7_IN(145); typed::gamecore_sub_013C8748(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a3_xo); A7_OUT(); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 2, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013C8748_0x13c8748(rdram, ctx, runtime);
}
// GexCutTail (0x13eca60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GexCutTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13ECA60u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A5_JAL(0x13ECA68u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GexCutTail, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(150); const uint64_t a2_r = typed::GexCutTail(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GexCutTail_0x13eca60(rdram, ctx, runtime);
}
// gamecore_sub_01434440 (0x1434440): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01434440(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1434440u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x13A7318u, ::a6ad::CalcDistancePointAnd2Point), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; A7_CHK(gamecore_sub_01434440, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f12, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A7_IN(161); const a1::RetVF a2_r = typed::gamecore_sub_01434440(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a2_f0, a2_f12, a2_f20, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2, a4_h8, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01434440_0x1434440(rdram, ctx, runtime);
}
// calcAttainPosForDirExec (0x1445bb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void calcAttainPosForDirExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1445BB0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x1445BCCu, 0x10C0A70u) && A5_SP16(), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(calcAttainPosForDirExec, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(162); const a1::RetVF a2_r = typed::calcAttainPosForDirExec(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::calcAttainPosForDirExec_0x1445bb0(rdram, ctx, runtime);
}
// SCRConditionMyselfKeepHit (0x1457868): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMyselfKeepHit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1457868u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x1457870u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionMyselfKeepHit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(163); const uint64_t a2_r = typed::SCRConditionMyselfKeepHit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMyselfKeepHit_0x1457868(rdram, ctx, runtime);
}
// getAimIKWork (0x145d668): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145D668u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x145D670u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getAimIKWork, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(166); const uint64_t a2_r = typed::getAimIKWork(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getAimIKWork_0x145d668(rdram, ctx, runtime);
}
// DEBUG_get_cur_using_anb_file_name (0x107b8d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_cur_using_anb_file_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107B8D0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x107C938u, ::a6ad::DEBUG_get_base_name) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x107C944u, 0x1290C4u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(DEBUG_get_cur_using_anb_file_name, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A7_IN(170); const uint64_t a2_r = typed::DEBUG_get_cur_using_anb_file_name(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::DEBUG_get_cur_using_anb_file_name_0x107b8d0(rdram, ctx, runtime);
}
// GAMEUTIL_get_limit_clip_yaw_rel (0x107d1b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_limit_clip_yaw_rel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107D1B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1080610u, ::a6ad::GAMEUTIL_get_limit_clip_yaw_abs) && A7_HOLDS(0x1080B28u, lift_d1::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2) && A7_HOLDS(0x1080C28u, ::a6ad::GAMEUTIL_is_between_yaw_and_yaw_extend_abs) && A7_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A7_CHK(GAMEUTIL_get_limit_clip_yaw_rel, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); A7_IN(171); const a1::RetVF a2_r = typed::GAMEUTIL_get_limit_clip_yaw_rel(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GAMEUTIL_get_limit_clip_yaw_rel_0x107d1b8(rdram, ctx, runtime);
}
// GetLwsOrientInitMotionPtr (0x10fc250): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLwsOrientInitMotionPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10FC250u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A7_HOLDS(0x10B70B8u, lift_d1::getAnimObjResource) && A5_JAL(0x10B7138u, 0x1B3410u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GetLwsOrientInitMotionPtr, a2_4, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(184); const uint32_t a2_r = typed::GetLwsOrientInitMotionPtr(rdram, runtime, a2_4, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetLwsOrientInitMotionPtr_0x10fc250(rdram, ctx, runtime);
}
// SpriteMenuGetPartsDefValueByHashStr (0x1112530): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetPartsDefValueByHashStr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112530u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A5_JAL(0x111254Cu, 0x1CBDB8u) && A5_JAL(0x1112558u, 0x1CBDB8u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SpriteMenuGetPartsDefValueByHashStr, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A7_IN(190); const uint64_t a2_r = typed::SpriteMenuGetPartsDefValueByHashStr(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SpriteMenuGetPartsDefValueByHashStr_0x1112530(rdram, ctx, runtime);
}
// SpriteMenuSetSpriteMenuSoundSwitchByHashStr (0x1112a78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetSpriteMenuSoundSwitchByHashStr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112A78u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A5_JAL(0x1112A9Cu, 0x1CBDB8u) && A5_JAL(0x1112AA8u, 0x1CBDB8u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SpriteMenuSetSpriteMenuSoundSwitchByHashStr, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(192); const uint64_t a2_r = typed::SpriteMenuSetSpriteMenuSoundSwitchByHashStr(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SpriteMenuSetSpriteMenuSoundSwitchByHashStr_0x1112a78(rdram, ctx, runtime);
}
// manager_sub_0114DDC0 (0x114ddc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0114DDC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114DDC0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1283D0u, ::memset_0x1283d0) && A7_HOLDS(0x114AD38u, lift_d1::mouseSetCursorPosition) && A7_HOLDS(0x1156018u, ::a6ad::windowGetCursorPosOnWindow) && A7_HOLDS(0x1156050u, ::a6ad::windowSetCursorPosOnWindow) && A7_HOLDS(0x11561B0u, ::a6ad::windowCloseRequest) && A5_JAL(0x115607Cu, 0x1283D0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(manager_sub_0114DDC0, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(195); const a1::RetVF a2_r = typed::manager_sub_0114DDC0(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::manager_sub_0114DDC0_0x114ddc0(rdram, ctx, runtime);
}
// dsysWrite (0x115a018): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dsysWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115A018u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x103080u, lift_d1::LoaderSysFWrite) && A7_HOLDS(0x10B678u, ::sceWrite_0x10b678) && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A5_JAL(0x115A030u, 0x103080u) && A5_JAL(0x115A044u, 0x1B3410u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(dsysWrite, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(196); const uint64_t a2_r = typed::dsysWrite(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::dsysWrite_0x115a018(rdram, ctx, runtime);
}
// manager_sub_011648D0 (0x11648d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_011648D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11648D0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B91A0u, lift_d1::iosUnitMatrix) && A7_HOLDS(0x1164AC0u, lift_d1::manager_sub_01164AC0) && A5_JAL(0x1164B48u, 0x1B91A0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; A7_CHK(manager_sub_011648D0, a2_4, a2_f12, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23]), (uint64_t)a1_bits(ctx->f[24])}; A7_IN(201); const a1::RetVF a2_r = typed::manager_sub_011648D0(rdram, runtime, a2_4, a2_f12, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a4_vf0, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); ctx->f[22] = a1_flt((uint32_t)a3_xo[2]); ctx->f[23] = a1_flt((uint32_t)a3_xo[3]); ctx->f[24] = a1_flt((uint32_t)a3_xo[4]); ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::manager_sub_011648D0_0x11648d0(rdram, ctx, runtime);
}
// GAMEUTIL_is_swimming_now (0x1343290): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_swimming_now(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1343290u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A5_JAL(0x13432B0u, 0x10C0A70u) && A5_JAL(0x1343474u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GAMEUTIL_is_swimming_now, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 5)}; A7_IN(208); const uint64_t a2_r = typed::GAMEUTIL_is_swimming_now(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GAMEUTIL_is_swimming_now_0x1343290(rdram, ctx, runtime);
}
// set_max_min_yaw_parameter_nico_yaw_pitch_cntrl (0x1347068): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_max_min_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347068u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1080310u, lift_d1::GAMEUTIL_increase_yaw) && A7_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8) && A5_JAL(0x13470D4u, 0x1080310u) && A5_JAL(0x13470E8u, 0x1080310u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const float a2_f3 = ctx->f[3]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(set_max_min_yaw_parameter_nico_yaw_pitch_cntrl, a2_4, a2_5, a2_6, a2_7, a2_f3, a2_f12, a2_f13, a2_f14, a2_f20, a2_f21); A7_IN(213); const a1::RetVF a2_r = typed::set_max_min_yaw_parameter_nico_yaw_pitch_cntrl(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f3, a2_f12, a2_f13, a2_f14, a2_f20, a2_f21); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::set_max_min_yaw_parameter_nico_yaw_pitch_cntrl_0x1347068(rdram, ctx, runtime);
}
// set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl (0x1347110): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347110u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1080310u, lift_d1::GAMEUTIL_increase_yaw) && A7_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8) && A5_JAL(0x1347194u, 0x1080310u) && A5_JAL(0x13471A8u, 0x1080310u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const float a2_f3 = ctx->f[3]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; A7_CHK(set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl, a2_4, a2_5, a2_6, a2_7, a2_f3, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); A7_IN(214); const a1::RetVF a2_r = typed::set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f3, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl_0x1347110(rdram, ctx, runtime);
}
// get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl (0x13477c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13477C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10807E0u, lift_d1::GAMEUTIL_increase_pitch_limit_clip) && A7_HOLDS(0x1347F88u, lift_d1::gamecore_sub_01347F88) && A7_HOLDS(0x1347FC8u, lift_d1::gamecore_sub_01347FC8) && A5_JAL(0x1347808u, 0x10807E0u) && A5_JAL(0x1347FBCu, 0x10807E0u) && A5_JAL(0x1347FFCu, 0x10807E0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl, a2_4, a2_5, a2_29, a2_f0, a2_f12, a2_f20, a2_f21); A7_IN(217); const a1::RetVF a2_r = typed::get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl(rdram, runtime, a2_4, a2_5, a2_29, a2_f0, a2_f12, a2_f20, a2_f21); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl_0x13477c0(rdram, ctx, runtime);
}
// endingflowSetTimePlayerHpMax (0x134d8d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void endingflowSetTimePlayerHpMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134D8D0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134F120u, lift_d1::_gameFWorkChk) && A7_HOLDS(0x1358248u, lift_d1::getPlayerHp) && A7_HOLDS(0x13583F8u, lift_d1::getPlayerMaxHp), 1)) { const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(endingflowSetTimePlayerHpMax, a2_f0, a2_f12, a2_f20, a2_f21); A7_IN(222); const a1::RetVF a2_r = typed::endingflowSetTimePlayerHpMax(rdram, runtime, a2_f0, a2_f12, a2_f20, a2_f21); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::endingflowSetTimePlayerHpMax_0x134d8d0(rdram, ctx, runtime);
}
// SCRConditionBirdPassingEachOther (0x136f0c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionBirdPassingEachOther(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136F0C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionBirdPassingEachOther, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(227); const a1::RetVF a2_r = typed::SCRConditionBirdPassingEachOther(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionBirdPassingEachOther_0x136f0c0(rdram, ctx, runtime);
}
// SCRConditionBirdBeginningApproaching (0x136f0f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionBirdBeginningApproaching(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136F0F8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionBirdBeginningApproaching, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(228); const a1::RetVF a2_r = typed::SCRConditionBirdBeginningApproaching(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionBirdBeginningApproaching_0x136f0f8(rdram, ctx, runtime);
}
// SCRGetBossPartsDamageNum (0x1378b68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetBossPartsDamageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1378B68u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRGetBossPartsDamageNum, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(235); const uint64_t a2_r = typed::SCRGetBossPartsDamageNum(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRGetBossPartsDamageNum_0x1378b68(rdram, ctx, runtime);
}
// SCRGetBossWeakPointAngleFromIndex (0x1378d90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetBossWeakPointAngleFromIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1378D90u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRGetBossWeakPointAngleFromIndex, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(240); const a1::RetVF a2_r = typed::SCRGetBossWeakPointAngleFromIndex(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRGetBossWeakPointAngleFromIndex_0x1378d90(rdram, ctx, runtime);
}
// getBossPartsCurDamage (0x1378dd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBossPartsCurDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1378DD8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getBossPartsCurDamage, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(241); const a1::RetVF a2_r = typed::getBossPartsCurDamage(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getBossPartsCurDamage_0x1378dd8(rdram, ctx, runtime);
}
// SCRGetBossPartsDamageFromWeak (0x13790f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetBossPartsDamageFromWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13790F8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRGetBossPartsDamageFromWeak, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(243); const uint64_t a2_r = typed::SCRGetBossPartsDamageFromWeak(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRGetBossPartsDamageFromWeak_0x13790f8(rdram, ctx, runtime);
}
// SCRGetBackStageBossPartsDamageFromWeak (0x1379188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetBackStageBossPartsDamageFromWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1379188u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRGetBackStageBossPartsDamageFromWeak, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(244); const uint64_t a2_r = typed::SCRGetBackStageBossPartsDamageFromWeak(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRGetBackStageBossPartsDamageFromWeak_0x1379188(rdram, ctx, runtime);
}
// SCRFuncBuddhaMissileInit2 (0x1379738): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBuddhaMissileInit2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1379738u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncBuddhaMissileInit2, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(246); const a1::RetVF a2_r = typed::SCRFuncBuddhaMissileInit2(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncBuddhaMissileInit2_0x1379738(rdram, ctx, runtime);
}
// NICO_is_hit_except_underwater (0x137e928): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void NICO_is_hit_except_underwater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E928u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A5_JAL(0x1343474u, 0x10C0A70u) && A5_JAL(0x137E948u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(NICO_is_hit_except_underwater, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(247); const uint64_t a2_r = typed::NICO_is_hit_except_underwater(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::NICO_is_hit_except_underwater_0x137e928(rdram, ctx, runtime);
}
// NICO_is_hit_except_underwater_or_boss (0x137e978): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void NICO_is_hit_except_underwater_or_boss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E978u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A7_HOLDS(0x13B53B0u, lift_d1::getPlayerLastAttrStatus) && A5_JAL(0x1343474u, 0x10C0A70u) && A5_JAL(0x137E998u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(NICO_is_hit_except_underwater_or_boss, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(248); const uint64_t a2_r = typed::NICO_is_hit_except_underwater_or_boss(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::NICO_is_hit_except_underwater_or_boss_0x137e978(rdram, ctx, runtime);
}
// gamecore_sub_0138F730 (0x138f730): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138F730(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138F730u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431FE0u, lift_d1::deleteUserPathWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432000u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_0138F730, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(249); const uint64_t a2_r = typed::gamecore_sub_0138F730(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_0138F730_0x138f730(rdram, ctx, runtime);
}
// gamecore_sub_01390218 (0x1390218): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01390218(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1390218u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431FE0u, lift_d1::deleteUserPathWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432000u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01390218, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(250); const uint64_t a2_r = typed::gamecore_sub_01390218(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01390218_0x1390218(rdram, ctx, runtime);
}
// gamecore_sub_01390380 (0x1390380): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01390380(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1390380u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431FE0u, lift_d1::deleteUserPathWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432000u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01390380, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(251); const uint64_t a2_r = typed::gamecore_sub_01390380(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01390380_0x1390380(rdram, ctx, runtime);
}
// SCRFuncHorseAdvertisFlag (0x1396300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncHorseAdvertisFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1396300u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncHorseAdvertisFlag, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(258); const uint64_t a2_r = typed::SCRFuncHorseAdvertisFlag(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncHorseAdvertisFlag_0x1396300(rdram, ctx, runtime);
}
// SCRFuncHorseEscapeCounterInit (0x1396398): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncHorseEscapeCounterInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1396398u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncHorseEscapeCounterInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(260); const uint64_t a2_r = typed::SCRFuncHorseEscapeCounterInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncHorseEscapeCounterInit_0x1396398(rdram, ctx, runtime);
}
// SCRFuncHorseEscapeCounterMain (0x13963c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncHorseEscapeCounterMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13963C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncHorseEscapeCounterMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(261); const a1::RetVF a2_r = typed::SCRFuncHorseEscapeCounterMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncHorseEscapeCounterMain_0x13963c0(rdram, ctx, runtime);
}
// SCRConditionHorseEscapeCounterChack (0x1396400): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionHorseEscapeCounterChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1396400u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionHorseEscapeCounterChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(262); const a1::RetVF a2_r = typed::SCRConditionHorseEscapeCounterChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionHorseEscapeCounterChack_0x1396400(rdram, ctx, runtime);
}
// setHorseForceScriptKey (0x1396920): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setHorseForceScriptKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1396920u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(setHorseForceScriptKey, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(266); const uint64_t a2_r = typed::setHorseForceScriptKey(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::setHorseForceScriptKey_0x1396920(rdram, ctx, runtime);
}
// SCRConditionFallRockChack (0x139cd38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionFallRockChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139CD38u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionFallRockChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(269); const a1::RetVF a2_r = typed::SCRConditionFallRockChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionFallRockChack_0x139cd38(rdram, ctx, runtime);
}
// SCRFuncKirinFallRockTimerSetFlagOn (0x139cd70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncKirinFallRockTimerSetFlagOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139CD70u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncKirinFallRockTimerSetFlagOn, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(270); const uint64_t a2_r = typed::SCRFuncKirinFallRockTimerSetFlagOn(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncKirinFallRockTimerSetFlagOn_0x139cd70(rdram, ctx, runtime);
}
// SCRConditionMinoCTargetWallChangeChack (0x13a5ac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMinoCTargetWallChangeChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5AC8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionMinoCTargetWallChangeChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(279); const uint64_t a2_r = typed::SCRConditionMinoCTargetWallChangeChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMinoCTargetWallChangeChack_0x13a5ac8(rdram, ctx, runtime);
}
// SCRFuncMinoCTargetWallReset (0x13a5ae8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCTargetWallReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5AE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCTargetWallReset, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(280); const uint64_t a2_r = typed::SCRFuncMinoCTargetWallReset(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCTargetWallReset_0x13a5ae8(rdram, ctx, runtime);
}
// SCRFuncMinoCAtkCountInit (0x13a5b08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCAtkCountInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5B08u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCAtkCountInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(281); const uint64_t a2_r = typed::SCRFuncMinoCAtkCountInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCAtkCountInit_0x13a5b08(rdram, ctx, runtime);
}
// SCRFuncMinoCAddAtkCount (0x13a5b28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCAddAtkCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5B28u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCAddAtkCount, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(282); const uint64_t a2_r = typed::SCRFuncMinoCAddAtkCount(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCAddAtkCount_0x13a5b28(rdram, ctx, runtime);
}
// SCRConditionMinoCAtkCountChack (0x13a5b50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMinoCAtkCountChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5B50u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionMinoCAtkCountChack, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(283); const uint64_t a2_r = typed::SCRConditionMinoCAtkCountChack(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMinoCAtkCountChack_0x13a5b50(rdram, ctx, runtime);
}
// SCRConditionMinoCWallCaseFlagChack (0x13a5be0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMinoCWallCaseFlagChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5BE0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionMinoCWallCaseFlagChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(285); const uint64_t a2_r = typed::SCRConditionMinoCWallCaseFlagChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMinoCWallCaseFlagChack_0x13a5be0(rdram, ctx, runtime);
}
// SCRFuncMinoCSetPlayerInCorridorFlag (0x13a5c00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCSetPlayerInCorridorFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5C00u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCSetPlayerInCorridorFlag, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(286); const uint64_t a2_r = typed::SCRFuncMinoCSetPlayerInCorridorFlag(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCSetPlayerInCorridorFlag_0x13a5c00(rdram, ctx, runtime);
}
// SCRFuncMinoCResetPlayerInCorridorFlag (0x13a5c28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCResetPlayerInCorridorFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5C28u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCResetPlayerInCorridorFlag, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(287); const uint64_t a2_r = typed::SCRFuncMinoCResetPlayerInCorridorFlag(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCResetPlayerInCorridorFlag_0x13a5c28(rdram, ctx, runtime);
}
// SCRFuncMinoCPlayerInCorridorFlagChack (0x13a5c48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncMinoCPlayerInCorridorFlagChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5C48u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncMinoCPlayerInCorridorFlagChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(288); const uint64_t a2_r = typed::SCRFuncMinoCPlayerInCorridorFlagChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncMinoCPlayerInCorridorFlagChack_0x13a5c48(rdram, ctx, runtime);
}
// SCRFuncNargaSetPlayerDamage (0x13a8538): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncNargaSetPlayerDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A8538u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncNargaSetPlayerDamage, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(289); const uint64_t a2_r = typed::SCRFuncNargaSetPlayerDamage(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncNargaSetPlayerDamage_0x13a8538(rdram, ctx, runtime);
}
// SCRConditionNargaWallCheck (0x13a8668): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionNargaWallCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A8668u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionNargaWallCheck, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(290); const uint64_t a2_r = typed::SCRConditionNargaWallCheck(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionNargaWallCheck_0x13a8668(rdram, ctx, runtime);
}
// SCRConditionNargaPlayerDamage (0x13a8698): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionNargaPlayerDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A8698u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionNargaPlayerDamage, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(291); const uint64_t a2_r = typed::SCRConditionNargaPlayerDamage(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionNargaPlayerDamage_0x13a8698(rdram, ctx, runtime);
}
// SCRConditionNargaWallNear (0x13a8788): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionNargaWallNear(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A8788u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionNargaWallNear, a2_2, a2_4, a2_8, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(292); const uint64_t a2_r = typed::SCRConditionNargaWallNear(rdram, runtime, a2_2, a2_4, a2_8, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionNargaWallNear_0x13a8788(rdram, ctx, runtime);
}
// get_merge_yaw_nico_sight_on (0x13aa410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_merge_yaw_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA410u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1080310u, lift_d1::GAMEUTIL_increase_yaw) && A7_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8) && A5_JAL(0x13AA42Cu, 0x1080310u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f3 = ctx->f[3]; A7_CHK(get_merge_yaw_nico_sight_on, a2_4, a2_f3); A7_IN(294); const a1::RetVF a2_r = typed::get_merge_yaw_nico_sight_on(rdram, runtime, a2_4, a2_f3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::get_merge_yaw_nico_sight_on_0x13aa410(rdram, ctx, runtime);
}
// nico_playerIsSwimmingNow (0x13b4e20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_playerIsSwimmingNow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B4E20u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A5_JAL(0x1343474u, 0x10C0A70u) && A5_JAL(0x13B4E40u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nico_playerIsSwimmingNow, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(298); const uint64_t a2_r = typed::nico_playerIsSwimmingNow(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_playerIsSwimmingNow_0x13b4e20(rdram, ctx, runtime);
}
// getCurWeapon (0x13b5078): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCurWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5078u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getCurWeapon, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(302); const uint64_t a2_r = typed::getCurWeapon(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getCurWeapon_0x13b5078(rdram, ctx, runtime);
}
// getPlayerMVHIST (0x13b5098): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerMVHIST(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5098u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getPlayerMVHIST, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(303); const uint64_t a2_r = typed::getPlayerMVHIST(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getPlayerMVHIST_0x13b5098(rdram, ctx, runtime);
}
// IsHorseCalled_withFlgOff (0x13b5158): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsHorseCalled_withFlgOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5158u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(IsHorseCalled_withFlgOff, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(305); const uint64_t a2_r = typed::IsHorseCalled_withFlgOff(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsHorseCalled_withFlgOff_0x13b5158(rdram, ctx, runtime);
}
// IsHorseAutoControlled (0x13b5190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsHorseAutoControlled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5190u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(IsHorseAutoControlled, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(306); const uint64_t a2_r = typed::IsHorseAutoControlled(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsHorseAutoControlled_0x13b5190(rdram, ctx, runtime);
}
// player_getWeaponType (0x13b5340): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_getWeaponType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5340u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(player_getWeaponType, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(309); const uint64_t a2_r = typed::player_getWeaponType(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::player_getWeaponType_0x13b5340(rdram, ctx, runtime);
}
// getPlayerColCont (0x13b5370): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerColCont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5370u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getPlayerColCont, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(310); const uint64_t a2_r = typed::getPlayerColCont(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getPlayerColCont_0x13b5370(rdram, ctx, runtime);
}
// player_SetGripDamage (0x13b5678): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_SetGripDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5678u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(player_SetGripDamage, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(314); const uint64_t a2_r = typed::player_SetGripDamage(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::player_SetGripDamage_0x13b5678(rdram, ctx, runtime);
}
// player_GetBoundDamageLevel (0x13b5a88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_GetBoundDamageLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5A88u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(player_GetBoundDamageLevel, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(315); const a1::RetVF a2_r = typed::player_GetBoundDamageLevel(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::player_GetBoundDamageLevel_0x13b5a88(rdram, ctx, runtime);
}
// isWeaponTargetMarker (0x13b5be0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void isWeaponTargetMarker(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5BE0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(isWeaponTargetMarker, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(316); const uint64_t a2_r = typed::isWeaponTargetMarker(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::isWeaponTargetMarker_0x13b5be0(rdram, ctx, runtime);
}
// torchAttached (0x13b6538): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void torchAttached(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6538u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(torchAttached, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A7_IN(318); const uint64_t a2_r = typed::torchAttached(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::torchAttached_0x13b6538(rdram, ctx, runtime);
}
// sendArrowPos (0x13b6580): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sendArrowPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6580u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sendArrowPos, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(319); const uint64_t a2_r = typed::sendArrowPos(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sendArrowPos_0x13b6580(rdram, ctx, runtime);
}
// sendArrowInfo (0x13b65b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sendArrowInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B65B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(sendArrowInfo, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(320); const uint64_t a2_r = typed::sendArrowInfo(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::sendArrowInfo_0x13b65b0(rdram, ctx, runtime);
}
// nico_playeGripholdOn (0x13b6a60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_playeGripholdOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6A60u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nico_playeGripholdOn, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(321); const uint64_t a2_r = typed::nico_playeGripholdOn(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_playeGripholdOn_0x13b6a60(rdram, ctx, runtime);
}
// nico_playeGripholdOff (0x13b6a88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_playeGripholdOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6A88u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nico_playeGripholdOff, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(322); const uint64_t a2_r = typed::nico_playeGripholdOff(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_playeGripholdOff_0x13b6a88(rdram, ctx, runtime);
}
// gamecore_sub_013B73B8 (0x13b73b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B73B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B73B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013B73B8, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(323); const uint64_t a2_r = typed::gamecore_sub_013B73B8(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013B73B8_0x13b73b8(rdram, ctx, runtime);
}
// gamecore_sub_013B7678 (0x13b7678): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B7678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B7678u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013B7678, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(324); const uint64_t a2_r = typed::gamecore_sub_013B7678(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013B7678_0x13b7678(rdram, ctx, runtime);
}
// gamecore_sub_013B76A0 (0x13b76a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B76A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B76A0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013B76A0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(325); const uint64_t a2_r = typed::gamecore_sub_013B76A0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013B76A0_0x13b76a0(rdram, ctx, runtime);
}
// SCRFuncPoseidonResetRevolutionDirection (0x13ce680): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonResetRevolutionDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE680u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonResetRevolutionDirection, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(327); const uint64_t a2_r = typed::SCRFuncPoseidonResetRevolutionDirection(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonResetRevolutionDirection_0x13ce680(rdram, ctx, runtime);
}
// SCRConditionPoseidonGetRevolutionRayChack (0x13ce6a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonGetRevolutionRayChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE6A0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonGetRevolutionRayChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(328); const uint64_t a2_r = typed::SCRConditionPoseidonGetRevolutionRayChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonGetRevolutionRayChack_0x13ce6a0(rdram, ctx, runtime);
}
// SCRFuncPoseidonMissAttackInit (0x13ce6c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonMissAttackInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE6C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonMissAttackInit, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(329); const uint64_t a2_r = typed::SCRFuncPoseidonMissAttackInit(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonMissAttackInit_0x13ce6c0(rdram, ctx, runtime);
}
// SCRFuncPoseidonMissAttackAdd (0x13ce6e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonMissAttackAdd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE6E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonMissAttackAdd, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(330); const uint64_t a2_r = typed::SCRFuncPoseidonMissAttackAdd(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonMissAttackAdd_0x13ce6e8(rdram, ctx, runtime);
}
// SCRConditionPoseidonMissAttackCheck (0x13ce710): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonMissAttackCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE710u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonMissAttackCheck, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(331); const uint64_t a2_r = typed::SCRConditionPoseidonMissAttackCheck(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonMissAttackCheck_0x13ce710(rdram, ctx, runtime);
}
// SCRFuncPoseidonSetPlayerTerrLog (0x13ce738): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonSetPlayerTerrLog(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE738u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonSetPlayerTerrLog, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(332); const uint64_t a2_r = typed::SCRFuncPoseidonSetPlayerTerrLog(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonSetPlayerTerrLog_0x13ce738(rdram, ctx, runtime);
}
// SCRFuncPoseidonResetPlayerTerrLog (0x13ce770): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonResetPlayerTerrLog(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE770u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonResetPlayerTerrLog, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(333); const uint64_t a2_r = typed::SCRFuncPoseidonResetPlayerTerrLog(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonResetPlayerTerrLog_0x13ce770(rdram, ctx, runtime);
}
// SCRConditionPoseidonGetPlayerTerrLog (0x13ce790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonGetPlayerTerrLog(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE790u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonGetPlayerTerrLog, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(334); const uint64_t a2_r = typed::SCRConditionPoseidonGetPlayerTerrLog(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonGetPlayerTerrLog_0x13ce790(rdram, ctx, runtime);
}
// SCRFuncPoseidonAngryCountAdd (0x13ce7f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonAngryCountAdd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE7F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonAngryCountAdd, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(335); const uint64_t a2_r = typed::SCRFuncPoseidonAngryCountAdd(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonAngryCountAdd_0x13ce7f0(rdram, ctx, runtime);
}
// SCRFuncPoseidonAngryCountInit (0x13ce818): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonAngryCountInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE818u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncPoseidonAngryCountInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(336); const uint64_t a2_r = typed::SCRFuncPoseidonAngryCountInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonAngryCountInit_0x13ce818(rdram, ctx, runtime);
}
// SCRConditionPoseidonAngryCountChack (0x13ce838): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonAngryCountChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE838u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonAngryCountChack, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(337); const uint64_t a2_r = typed::SCRConditionPoseidonAngryCountChack(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonAngryCountChack_0x13ce838(rdram, ctx, runtime);
}
// SCRConditionPoseidonDamageCoreTime (0x13ce868): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonDamageCoreTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE868u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionPoseidonDamageCoreTime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(338); const a1::RetVF a2_r = typed::SCRConditionPoseidonDamageCoreTime(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonDamageCoreTime_0x13ce868(rdram, ctx, runtime);
}
// SCRConditionPoseidonDamageFlagSet (0x13ce8a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonDamageFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE8A0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonDamageFlagSet, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(339); const uint64_t a2_r = typed::SCRConditionPoseidonDamageFlagSet(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonDamageFlagSet_0x13ce8a0(rdram, ctx, runtime);
}
// SCRConditionPoseidonDamageFlagReset (0x13ce8c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonDamageFlagReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE8C8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonDamageFlagReset, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(340); const uint64_t a2_r = typed::SCRConditionPoseidonDamageFlagReset(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonDamageFlagReset_0x13ce8c8(rdram, ctx, runtime);
}
// SCRConditionPoseidonDamageCoreTimeMain (0x13ce8f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPoseidonDamageCoreTimeMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE8F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPoseidonDamageCoreTimeMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(341); const uint64_t a2_r = typed::SCRConditionPoseidonDamageCoreTimeMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPoseidonDamageCoreTimeMain_0x13ce8f0(rdram, ctx, runtime);
}
// SCRFuncPoseidonAngryMain (0x13cea30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncPoseidonAngryMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CEA30u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncPoseidonAngryMain, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(342); const uint64_t a2_r = typed::SCRFuncPoseidonAngryMain(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncPoseidonAngryMain_0x13cea30(rdram, ctx, runtime);
}
// gamecore_sub_013D15B0 (0x13d15b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013D15B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D15B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431FE0u, lift_d1::deleteUserPathWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432000u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013D15B0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(343); const uint64_t a2_r = typed::gamecore_sub_013D15B0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013D15B0_0x13d15b0(rdram, ctx, runtime);
}
// SCRFuncSnakeCUpGroundInit (0x13f5d98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSnakeCUpGroundInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F5D98u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSnakeCUpGroundInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(346); const uint64_t a2_r = typed::SCRFuncSnakeCUpGroundInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSnakeCUpGroundInit_0x13f5d98(rdram, ctx, runtime);
}
// gamecore_sub_013FCA10 (0x13fca10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FCA10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FCA10u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x11327B0u, lift_d1::CopyGeometryPointer) && A7_HOLDS(0x113F878u, lift_d1::set_npe_blend_ratio_particle_emitter) && A7_HOLDS(0x113FA70u, lift_d1::get_particle_emitter_from_emitter_id) && A5_JAL(0x13FCAFCu, 0x11327B0u) && A5_JAL(0x13FCB10u, 0x113F878u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(gamecore_sub_013FCA10, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[20]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A7_IN(347); const a1::RetVF a2_r = typed::gamecore_sub_013FCA10(rdram, runtime, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[20] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 4, a3_xo[2]); Ps2SetGpr64(ctx, 5, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013FCA10_0x13fca10(rdram, ctx, runtime);
}
// gamecore_sub_013FF580 (0x13ff580): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FF580(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF580u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x13FF5B8u, 0x1CBDB8u) && A5_JAL(0x13FF5C8u, 0x1CBDB8u) && A5_JAL(0x13FF5D8u, 0x1112C88u) && A5_JAL(0x13FF5E8u, 0x1CBDB8u) && A5_JAL(0x13FF5F8u, 0x1CBDB8u) && A5_JAL(0x13FF608u, 0x1112C88u) && A5_JAL(0x13FF618u, 0x1CBDB8u) && A5_JAL(0x13FF628u, 0x1CBDB8u) && A5_JAL(0x13FF638u, 0x1112C88u) && A5_JAL(0x13FF648u, 0x1CBDB8u) && A5_JAL(0x13FF658u, 0x1CBDB8u) && A5_JAL(0x13FF668u, 0x1112C88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013FF580, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(350); const uint64_t a2_r = typed::gamecore_sub_013FF580(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013FF580_0x13ff580(rdram, ctx, runtime);
}
// gamecore_sub_013FF9A8 (0x13ff9a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FF9A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF9A8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1111A38u, lift_d1::SpriteMenuSetDispFlag) && A7_HOLDS(0x1112008u, lift_d1::SpriteMenuGetLayerNoByHash) && A7_HOLDS(0x1112260u, ::a6ad::SpriteMenuGetPartsDefByHash) && A7_HOLDS(0x1346210u, lift_d1::getFadeValue) && A7_HOLDS(0x1346300u, lift_d1::getDemoBarFade) && A5_JAL(0x13FFA4Cu, 0x1CBDB8u) && A5_JAL(0x13FFA5Cu, 0x1CBDB8u) && A5_JAL(0x13FFA6Cu, 0x1112260u) && A5_JAL(0x13FFA7Cu, 0x1CBDB8u) && A5_JAL(0x13FFA8Cu, 0x1CBDB8u) && A5_JAL(0x13FFA9Cu, 0x1112260u) && A5_JAL(0x13FFAC4u, 0x1CBDB8u) && A5_JAL(0x13FFAD0u, 0x1112008u) && A5_JAL(0x13FFAE0u, 0x1111A38u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013FF9A8, a2_4, a2_5, a2_6, a2_10, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[7] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A7_IN(351); const uint64_t a2_r = typed::gamecore_sub_013FF9A8(rdram, runtime, a2_4, a2_5, a2_6, a2_10, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); Ps2SetGpr64(ctx, 10, a3_xo[6]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013FF9A8_0x13ff9a8(rdram, ctx, runtime);
}
// SelectOmakeItem (0x1400a20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SelectOmakeItem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1400A20u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A7_HOLDS(0x11134B0u, ::a6ad::SpriteMenuSetCheckBoxByHash) && A7_HOLDS(0x134E530u, lift_d1::_omakeSelectFlagSet) && A7_HOLDS(0x134F2A8u, lift_d1::getGameHardMode) && A7_HOLDS(0x134FA48u, ::a6ad::_omakeSelectFlagCls) && A7_HOLDS(0x134FB28u, lift_d1::_omakeSelectFlagChk) && A7_HOLDS(0x1407310u, lift_d1::getSelectWeapon) && A5_JAL(0x1400A78u, 0x1CBDB8u) && A5_JAL(0x1400AB4u, 0x1CBDB8u) && A5_JAL(0x1400ACCu, 0x1112C88u) && A5_JAL(0x1400B50u, 0x1CBDB8u) && A5_JAL(0x1400B8Cu, 0x1CBDB8u) && A5_JAL(0x1400BB0u, 0x11134B0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SelectOmakeItem, a2_4, a2_5, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(352); const uint64_t a2_r = typed::SelectOmakeItem(rdram, runtime, a2_4, a2_5, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SelectOmakeItem_0x1400a20(rdram, ctx, runtime);
}
// gamecore_sub_01402458 (0x1402458): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402458(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402458u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1111A38u, lift_d1::SpriteMenuSetDispFlag) && A7_HOLDS(0x1112008u, lift_d1::SpriteMenuGetLayerNoByHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402494u, 0x1CBDB8u) && A5_JAL(0x14024A4u, 0x1CBDB8u) && A5_JAL(0x14024B4u, 0x1112C88u) && A5_JAL(0x14024D0u, 0x1CBDB8u) && A5_JAL(0x14024E0u, 0x1CBDB8u) && A5_JAL(0x14024F0u, 0x1112C88u) && A5_JAL(0x1402510u, 0x1CBDB8u) && A5_JAL(0x140251Cu, 0x1112008u) && A5_JAL(0x140252Cu, 0x1111A38u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402458, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(353); const uint64_t a2_r = typed::gamecore_sub_01402458(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402458_0x1402458(rdram, ctx, runtime);
}
// gamecore_sub_01402560 (0x1402560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402560(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402560u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x140259Cu, 0x1CBDB8u) && A5_JAL(0x14025ACu, 0x1CBDB8u) && A5_JAL(0x14025BCu, 0x1112C88u) && A5_JAL(0x14025D8u, 0x1CBDB8u) && A5_JAL(0x14025E8u, 0x1CBDB8u) && A5_JAL(0x14025F8u, 0x1112C88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402560, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(354); const uint64_t a2_r = typed::gamecore_sub_01402560(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402560_0x1402560(rdram, ctx, runtime);
}
// gamecore_sub_01402640 (0x1402640): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402640(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402640u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402678u, 0x1CBDB8u) && A5_JAL(0x1402688u, 0x1CBDB8u) && A5_JAL(0x1402698u, 0x1112C88u) && A5_JAL(0x14026A8u, 0x1CBDB8u) && A5_JAL(0x14026B8u, 0x1CBDB8u) && A5_JAL(0x14026C8u, 0x1112C88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402640, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(355); const uint64_t a2_r = typed::gamecore_sub_01402640(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402640_0x1402640(rdram, ctx, runtime);
}
// gamecore_sub_01402730 (0x1402730): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402730(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402730u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402768u, 0x1CBDB8u) && A5_JAL(0x1402778u, 0x1CBDB8u) && A5_JAL(0x1402788u, 0x1112C88u) && A5_JAL(0x14027A0u, 0x1CBDB8u) && A5_JAL(0x14027B0u, 0x1CBDB8u) && A5_JAL(0x14027C0u, 0x1112C88u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402730, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(356); typed::gamecore_sub_01402730(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402730_0x1402730(rdram, ctx, runtime);
}
// CheckSelectDisp (0x14028b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CheckSelectDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14028B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x11134B0u, ::a6ad::SpriteMenuSetCheckBoxByHash) && A7_HOLDS(0x134F2A8u, lift_d1::getGameHardMode) && A7_HOLDS(0x134FB28u, lift_d1::_omakeSelectFlagChk) && A5_JAL(0x1402900u, 0x1CBDB8u) && A5_JAL(0x140293Cu, 0x1CBDB8u) && A5_JAL(0x1402960u, 0x11134B0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CheckSelectDisp, a2_4, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(357); const uint64_t a2_r = typed::CheckSelectDisp(rdram, runtime, a2_4, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CheckSelectDisp_0x14028b8(rdram, ctx, runtime);
}
// gamecore_sub_01402B08 (0x1402b08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402B08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402B08u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1111A38u, lift_d1::SpriteMenuSetDispFlag) && A7_HOLDS(0x1112008u, lift_d1::SpriteMenuGetLayerNoByHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402B44u, 0x1CBDB8u) && A5_JAL(0x1402B54u, 0x1CBDB8u) && A5_JAL(0x1402B64u, 0x1112C88u) && A5_JAL(0x1402B80u, 0x1CBDB8u) && A5_JAL(0x1402B90u, 0x1CBDB8u) && A5_JAL(0x1402BA0u, 0x1112C88u) && A5_JAL(0x1402BC0u, 0x1CBDB8u) && A5_JAL(0x1402BCCu, 0x1112008u) && A5_JAL(0x1402BDCu, 0x1111A38u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402B08, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(358); const uint64_t a2_r = typed::gamecore_sub_01402B08(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402B08_0x1402b08(rdram, ctx, runtime);
}
// gamecore_sub_01402C10 (0x1402c10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402C10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402C10u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402C4Cu, 0x1CBDB8u) && A5_JAL(0x1402C5Cu, 0x1CBDB8u) && A5_JAL(0x1402C6Cu, 0x1112C88u) && A5_JAL(0x1402C88u, 0x1CBDB8u) && A5_JAL(0x1402C98u, 0x1CBDB8u) && A5_JAL(0x1402CA8u, 0x1112C88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402C10, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(359); const uint64_t a2_r = typed::gamecore_sub_01402C10(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402C10_0x1402c10(rdram, ctx, runtime);
}
// gamecore_sub_01402CF0 (0x1402cf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402CF0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402CF0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402D28u, 0x1CBDB8u) && A5_JAL(0x1402D38u, 0x1CBDB8u) && A5_JAL(0x1402D48u, 0x1112C88u) && A5_JAL(0x1402D58u, 0x1CBDB8u) && A5_JAL(0x1402D68u, 0x1CBDB8u) && A5_JAL(0x1402D78u, 0x1112C88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402CF0, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(360); const uint64_t a2_r = typed::gamecore_sub_01402CF0(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402CF0_0x1402cf0(rdram, ctx, runtime);
}
// gamecore_sub_01402DE0 (0x1402de0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01402DE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402DE0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1112C88u, lift_d1::SpriteMenuGetMenuIdByHash) && A5_JAL(0x1402E18u, 0x1CBDB8u) && A5_JAL(0x1402E28u, 0x1CBDB8u) && A5_JAL(0x1402E38u, 0x1112C88u) && A5_JAL(0x1402E50u, 0x1CBDB8u) && A5_JAL(0x1402E60u, 0x1CBDB8u) && A5_JAL(0x1402E70u, 0x1112C88u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01402DE0, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(361); typed::gamecore_sub_01402DE0(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01402DE0_0x1402de0(rdram, ctx, runtime);
}
// SCRFuncKameFallPatienceCunterMain (0x1404ae8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncKameFallPatienceCunterMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1404AE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncKameFallPatienceCunterMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(364); const a1::RetVF a2_r = typed::SCRFuncKameFallPatienceCunterMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncKameFallPatienceCunterMain_0x1404ae8(rdram, ctx, runtime);
}
// SCRConditionKameCrackAnimChack (0x1404d30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionKameCrackAnimChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1404D30u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionKameCrackAnimChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(370); const a1::RetVF a2_r = typed::SCRConditionKameCrackAnimChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionKameCrackAnimChack_0x1404d30(rdram, ctx, runtime);
}
// SCRConditionKameWallCaseFlagChack (0x1405038): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionKameWallCaseFlagChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1405038u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionKameWallCaseFlagChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(372); const uint64_t a2_r = typed::SCRConditionKameWallCaseFlagChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionKameWallCaseFlagChack_0x1405038(rdram, ctx, runtime);
}
// SCRConditionKameMoveTimeChack (0x1405058): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionKameMoveTimeChack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1405058u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionKameMoveTimeChack, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(373); const a1::RetVF a2_r = typed::SCRConditionKameMoveTimeChack(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionKameMoveTimeChack_0x1405058(rdram, ctx, runtime);
}
// SCRFuncKameMoveTimeSet (0x1405090): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncKameMoveTimeSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1405090u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncKameMoveTimeSet, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(374); const a1::RetVF a2_r = typed::SCRFuncKameMoveTimeSet(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncKameMoveTimeSet_0x1405090(rdram, ctx, runtime);
}
// SCRFuncKameMoveTimeMain (0x14050e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncKameMoveTimeMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14050E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncKameMoveTimeMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(376); const a1::RetVF a2_r = typed::SCRFuncKameMoveTimeMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncKameMoveTimeMain_0x14050e8(rdram, ctx, runtime);
}
// SCRFuncUnagiSetPlayerGazePos (0x1406760): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncUnagiSetPlayerGazePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1406760u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1343B28u, lift_d1::GAMEUTIL_set_auto_get_flag_boss_interest_position) && A7_HOLDS(0x1343B38u, lift_d1::GAMEUTIL_set_custom_boss_interest_position) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncUnagiSetPlayerGazePos, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(380); const uint64_t a2_r = typed::SCRFuncUnagiSetPlayerGazePos(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncUnagiSetPlayerGazePos_0x1406760(rdram, ctx, runtime);
}
// SCRFuncUnageIncTblIdx (0x1406a70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncUnageIncTblIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1406A70u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncUnageIncTblIdx, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(384); const uint64_t a2_r = typed::SCRFuncUnageIncTblIdx(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncUnageIncTblIdx_0x1406a70(rdram, ctx, runtime);
}
// SCRFuncUnageInitBossNotParentTime (0x1406aa0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncUnageInitBossNotParentTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1406AA0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncUnageInitBossNotParentTime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(385); const uint64_t a2_r = typed::SCRFuncUnageInitBossNotParentTime(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncUnageInitBossNotParentTime_0x1406aa0(rdram, ctx, runtime);
}
// SCRConditionUnageBossNotParentTime (0x1406b38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionUnageBossNotParentTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1406B38u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionUnageBossNotParentTime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(386); const a1::RetVF a2_r = typed::SCRConditionUnageBossNotParentTime(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionUnageBossNotParentTime_0x1406b38(rdram, ctx, runtime);
}
// SCRFuncYamoriBDamageCheckInit (0x140c880): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBDamageCheckInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140C880u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBDamageCheckInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(387); const a1::RetVF a2_r = typed::SCRFuncYamoriBDamageCheckInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBDamageCheckInit_0x140c880(rdram, ctx, runtime);
}
// SCRConditionYamoriBDamageCheckFrontR (0x140ca40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBDamageCheckFrontR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140CA40u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionYamoriBDamageCheckFrontR, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(388); const a1::RetVF a2_r = typed::SCRConditionYamoriBDamageCheckFrontR(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBDamageCheckFrontR_0x140ca40(rdram, ctx, runtime);
}
// SCRConditionYamoriBDamageCheckBackR (0x140ca78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBDamageCheckBackR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140CA78u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionYamoriBDamageCheckBackR, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(389); const a1::RetVF a2_r = typed::SCRConditionYamoriBDamageCheckBackR(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBDamageCheckBackR_0x140ca78(rdram, ctx, runtime);
}
// SCRConditionYamoriBDamageCheckFrontL (0x140cab0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBDamageCheckFrontL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140CAB0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionYamoriBDamageCheckFrontL, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(390); const a1::RetVF a2_r = typed::SCRConditionYamoriBDamageCheckFrontL(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBDamageCheckFrontL_0x140cab0(rdram, ctx, runtime);
}
// SCRConditionYamoriBDamageCheckBackL (0x140cae8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBDamageCheckBackL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140CAE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionYamoriBDamageCheckBackL, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(391); const a1::RetVF a2_r = typed::SCRConditionYamoriBDamageCheckBackL(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBDamageCheckBackL_0x140cae8(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetCalcPosOffset (0x140cd28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetCalcPosOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140CD28u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncYamoriBSetCalcPosOffset, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(392); const uint64_t a2_r = typed::SCRFuncYamoriBSetCalcPosOffset(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetCalcPosOffset_0x140cd28(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetBreathVolume (0x140d0b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetBreathVolume(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D0B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBSetBreathVolume, a2_2, a2_4, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(393); const uint64_t a2_r = typed::SCRFuncYamoriBSetBreathVolume(rdram, runtime, a2_2, a2_4, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetBreathVolume_0x140d0b0(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetRandomRadius (0x140d150): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetRandomRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D150u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(SCRFuncYamoriBSetRandomRadius, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(394); const uint64_t a2_r = typed::SCRFuncYamoriBSetRandomRadius(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetRandomRadius_0x140d150(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetRandomRadius2 (0x140d198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetRandomRadius2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D198u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(SCRFuncYamoriBSetRandomRadius2, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(395); const uint64_t a2_r = typed::SCRFuncYamoriBSetRandomRadius2(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetRandomRadius2_0x140d198(rdram, ctx, runtime);
}
// SCRFuncYamoriBUnsetRandom (0x140d1e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBUnsetRandom(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D1E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBUnsetRandom, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(396); const uint64_t a2_r = typed::SCRFuncYamoriBUnsetRandom(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBUnsetRandom_0x140d1e0(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetAttackPosAndGazePosInit (0x140d208): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetAttackPosAndGazePosInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D208u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBSetAttackPosAndGazePosInit, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(397); const uint64_t a2_r = typed::SCRFuncYamoriBSetAttackPosAndGazePosInit(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetAttackPosAndGazePosInit_0x140d208(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetBellyDamageParam (0x140d4b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetBellyDamageParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D4B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncYamoriBSetBellyDamageParam, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(398); const uint64_t a2_r = typed::SCRFuncYamoriBSetBellyDamageParam(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetBellyDamageParam_0x140d4b8(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetBellyDamageInitWork (0x140d4e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetBellyDamageInitWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D4E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBSetBellyDamageInitWork, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(399); const a1::RetVF a2_r = typed::SCRFuncYamoriBSetBellyDamageInitWork(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetBellyDamageInitWork_0x140d4e0(rdram, ctx, runtime);
}
// SCRConditionYamoriBLessBellyDamageProgress (0x140d510): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBLessBellyDamageProgress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D510u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionYamoriBLessBellyDamageProgress, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(400); const a1::RetVF a2_r = typed::SCRConditionYamoriBLessBellyDamageProgress(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBLessBellyDamageProgress_0x140d510(rdram, ctx, runtime);
}
// SCRConditionYamoriBBellyDamageCheck (0x140d548): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionYamoriBBellyDamageCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D548u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionYamoriBBellyDamageCheck, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(401); const uint64_t a2_r = typed::SCRConditionYamoriBBellyDamageCheck(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionYamoriBBellyDamageCheck_0x140d548(rdram, ctx, runtime);
}
// SCRFuncYamoriBAimInit (0x140d6f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBAimInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D6F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x145D638u, lift_d1::updateAimIKWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x145D644u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncYamoriBAimInit, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(402); const a1::RetVF a2_r = typed::SCRFuncYamoriBAimInit(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBAimInit_0x140d6f0(rdram, ctx, runtime);
}
// demoManChkLoadFinish (0x140e468): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void demoManChkLoadFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140E468u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A5_JAL(0x140E470u, 0x110D058u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(demoManChkLoadFinish, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A7_IN(408); const uint64_t a2_r = typed::demoManChkLoadFinish(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::demoManChkLoadFinish_0x140e468(rdram, ctx, runtime);
}
// IsForwardTime__DemoMan (0x140e4e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsForwardTime__DemoMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140E4E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A5_JAL(0x140E4F4u, 0x110D058u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(IsForwardTime__DemoMan, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A7_IN(409); const uint64_t a2_r = typed::IsForwardTime__DemoMan(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsForwardTime__DemoMan_0x140e4e8(rdram, ctx, runtime);
}
// ForwardTime__DemoMan (0x140e540): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ForwardTime__DemoMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140E540u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A5_JAL(0x140E54Cu, 0x110D058u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(ForwardTime__DemoMan, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A7_IN(410); const a1::RetVF a2_r = typed::ForwardTime__DemoMan(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::ForwardTime__DemoMan_0x140e540(rdram, ctx, runtime);
}
// CAM_get_motion_kind_follow_rot_axis_attr_target (0x141a698): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_get_motion_kind_follow_rot_axis_attr_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A698u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x107FFB8u, lift_d1::GAMEUTIL_get_motion_kind) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x141A7C8u, lift_d1::CAM_get_ground_contact_obj) && A5_JAL(0x141A6B0u, 0x107FFB8u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CAM_get_motion_kind_follow_rot_axis_attr_target, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(419); const uint64_t a2_r = typed::CAM_get_motion_kind_follow_rot_axis_attr_target(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CAM_get_motion_kind_follow_rot_axis_attr_target_0x141a698(rdram, ctx, runtime);
}
// is_lws_fixed_point_mode (0x1425728): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_lws_fixed_point_mode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425728u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x11621C0u, lift_d1::get_fixed_point_type_camera_lws_animation) && A7_HOLDS(0x11629E8u, lift_d1::manager_sub_011629E8) && A7_HOLDS(0x1425700u, lift_d1::gamecore_sub_01425700) && A5_JAL(0x1425734u, 0x11621C0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A7_CHK(is_lws_fixed_point_mode, a2_2, a2_4); A7_IN(426); const uint64_t a2_r = typed::is_lws_fixed_point_mode(rdram, runtime, a2_2, a2_4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::is_lws_fixed_point_mode_0x1425728(rdram, ctx, runtime);
}
// is_lws_movie_mode_game_camera (0x1425758): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_lws_movie_mode_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425758u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1162190u, lift_d1::get_main_type_camera_lws_animation) && A7_HOLDS(0x11629E8u, lift_d1::manager_sub_011629E8) && A7_HOLDS(0x1425700u, lift_d1::gamecore_sub_01425700) && A5_JAL(0x1425764u, 0x1162190u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); A7_CHK(is_lws_movie_mode_game_camera, a2_2, a2_4); A7_IN(427); const uint64_t a2_r = typed::is_lws_movie_mode_game_camera(rdram, runtime, a2_2, a2_4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::is_lws_movie_mode_game_camera_0x1425758(rdram, ctx, runtime);
}
// gamecore_sub_01435EA0 (0x1435ea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01435EA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1435EA0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1169EE8u, lift_d1::getSubPointData) && A7_HOLDS(0x1432018u, lift_d1::setUserPathMode) && A7_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A7_HOLDS(0x1433D80u, lift_d1::getSubPointDataByAreaIdx) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x1433DFCu, 0x1169EE8u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01435EA0, a2_2, a2_4, a2_6, a2_7, a2_8, a2_10, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A7_IN(432); const uint64_t a2_r = typed::gamecore_sub_01435EA0(rdram, runtime, a2_2, a2_4, a2_6, a2_7, a2_8, a2_10, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01435EA0_0x1435ea0(rdram, ctx, runtime);
}
// gamecore_sub_01435F00 (0x1435f00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01435F00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1435F00u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1169EA8u, lift_d1::getGoalPointData) && A7_HOLDS(0x1432018u, lift_d1::setUserPathMode) && A7_HOLDS(0x1432448u, lift_d1::setAttainmentPosAndDir) && A7_HOLDS(0x1433CE0u, lift_d1::getGoalPointDataByAreaIdx) && A5_JAL(0x1432474u, 0x128188u) && A5_JAL(0x14324ACu, 0x1147AB0u) && A5_JAL(0x1433D5Cu, 0x1169EA8u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01435F00, a2_2, a2_4, a2_5, a2_7, a2_8, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A7_IN(433); const uint64_t a2_r = typed::gamecore_sub_01435F00(rdram, runtime, a2_2, a2_4, a2_5, a2_7, a2_8, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01435F00_0x1435f00(rdram, ctx, runtime);
}
// SCRConditionSegmentLoad (0x1459638): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionSegmentLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459638u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x115EE68u, lift_d1::sheetMgrChkStageIndexResolve) && A7_HOLDS(0x13668F0u, lift_d1::stagemanager_checkstageAlreadyLoadedByStrptr) && A5_JAL(0x136699Cu, 0x115EE68u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); A7_CHK(SCRConditionSegmentLoad, a2_29); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); A7_IN(439); const uint64_t a2_r = typed::SCRConditionSegmentLoad(rdram, runtime, a2_29, a4_h2, a4_h3, a4_h7); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionSegmentLoad_0x1459638(rdram, ctx, runtime);
}
// SCRFuncSoundPlay (0x1464e08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSoundPlay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464E08u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1283D0u, ::memset_0x1283d0) && A7_HOLDS(0x136A758u, ::a6ad::bgmAutoStart) && A5_JAL(0x136A7D4u, 0x1283D0u), 1)) { const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSoundPlay, a2_7, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(445); const uint64_t a2_r = typed::SCRFuncSoundPlay(rdram, runtime, a2_7, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSoundPlay_0x1464e08(rdram, ctx, runtime);
}
// DEBUG_get_parent_layout_name (0x107ca20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_parent_layout_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107CA20u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F3420u, lift_d1::GetLayoutParentObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(DEBUG_get_parent_layout_name, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(446); const uint64_t a2_r = typed::DEBUG_get_parent_layout_name(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::DEBUG_get_parent_layout_name_0x107ca20(rdram, ctx, runtime);
}
// GetCharacterClassCurAnimationTotalFrame (0x10c1a20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCharacterClassCurAnimationTotalFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C1A20u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A7_HOLDS(0x10B2790u, lift_d1::getBlendBaseMotion) && A7_HOLDS(0x10B3AF8u, lift_d1::GetMotionResourceOriginalFrames) && A7_HOLDS(0x10B63D8u, lift_d1::GetMotionResourceFrames) && A5_JAL(0x10B3BC0u, 0x1B3410u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GetCharacterClassCurAnimationTotalFrame, a2_4, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A7_IN(458); const uint64_t a2_r = typed::GetCharacterClassCurAnimationTotalFrame(rdram, runtime, a2_4, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetCharacterClassCurAnimationTotalFrame_0x10c1a20(rdram, ctx, runtime);
}
// CharaCtrlSetScriptCtrlStatusCallBack (0x10cc408): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CharaCtrlSetScriptCtrlStatusCallBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC408u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CharaCtrlSetScriptCtrlStatusCallBack, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(465); const uint64_t a2_r = typed::CharaCtrlSetScriptCtrlStatusCallBack(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CharaCtrlSetScriptCtrlStatusCallBack_0x10cc408(rdram, ctx, runtime);
}
// checkShift (0x10d32f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void checkShift(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D32F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A7_HOLDS(0x10B2790u, lift_d1::getBlendBaseMotion) && A7_HOLDS(0x10B3AF8u, lift_d1::GetMotionResourceOriginalFrames) && A7_HOLDS(0x10B63D8u, lift_d1::GetMotionResourceFrames) && A5_JAL(0x10B3BC0u, 0x1B3410u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(checkShift, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(469); const a1::RetVF a2_r = typed::checkShift(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::checkShift_0x10d32f0(rdram, ctx, runtime);
}
// manager_sub_010DF590 (0x10df590): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010DF590(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DF590u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(manager_sub_010DF590, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(471); const uint64_t a2_r = typed::manager_sub_010DF590(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::manager_sub_010DF590_0x10df590(rdram, ctx, runtime);
}
// SetScriptObjFinishCallBackFunc (0x1102ac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetScriptObjFinishCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1102AC8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1102438u, ::decomp_scriptvm3::getScriptClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11119B8u, lift_d1::GetScriptSpriteMenuClass) && A7_HOLDS(0x1114238u, lift_d1::GetScriptSpriteClass) && A7_HOLDS(0x1156628u, ::a6ad::GetWindowClass), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SetScriptObjFinishCallBackFunc, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(478); const uint64_t a2_r = typed::SetScriptObjFinishCallBackFunc(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SetScriptObjFinishCallBackFunc_0x1102ac8(rdram, ctx, runtime);
}
// manager_sub_01123380 (0x1123380): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01123380(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1123380u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(manager_sub_01123380, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(487); const uint64_t a2_r = typed::manager_sub_01123380(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::manager_sub_01123380_0x1123380(rdram, ctx, runtime);
}
// CollisionPlaneJudgeCallbackParticle (0x1123520): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CollisionPlaneJudgeCallbackParticle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1123520u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CollisionPlaneJudgeCallbackParticle, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(488); const uint64_t a2_r = typed::CollisionPlaneJudgeCallbackParticle(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CollisionPlaneJudgeCallbackParticle_0x1123520(rdram, ctx, runtime);
}
// MakeGeometryPointerCollisionInfo (0x1128300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MakeGeometryPointerCollisionInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128300u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(MakeGeometryPointerCollisionInfo, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(494); const uint64_t a2_r = typed::MakeGeometryPointerCollisionInfo(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf5, a4_vf6, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::MakeGeometryPointerCollisionInfo_0x1128300(rdram, ctx, runtime);
}
// CollisionObjectJudgeCallbackExceptDescendOneObj (0x1128828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CollisionObjectJudgeCallbackExceptDescendOneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128828u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F3420u, lift_d1::GetLayoutParentObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CollisionObjectJudgeCallbackExceptDescendOneObj, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(495); const uint64_t a2_r = typed::CollisionObjectJudgeCallbackExceptDescendOneObj(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CollisionObjectJudgeCallbackExceptDescendOneObj_0x1128828(rdram, ctx, runtime);
}
// GetCollisionDBNextSurfLinkComp (0x1129410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCollisionDBNextSurfLinkComp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129410u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GetCollisionDBNextSurfLinkComp, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(498); const uint64_t a2_r = typed::GetCollisionDBNextSurfLinkComp(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetCollisionDBNextSurfLinkComp_0x1129410(rdram, ctx, runtime);
}
// GetCollisionDBNextSurfLink (0x1129488): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCollisionDBNextSurfLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129488u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GetCollisionDBNextSurfLink, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(499); const uint64_t a2_r = typed::GetCollisionDBNextSurfLink(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GetCollisionDBNextSurfLink_0x1129488(rdram, ctx, runtime);
}
// MakeCollisionPointCollisionInfo (0x1129550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MakeCollisionPointCollisionInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129550u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(MakeCollisionPointCollisionInfo, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(500); const uint64_t a2_r = typed::MakeCollisionPointCollisionInfo(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf5, a4_vf6, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::MakeCollisionPointCollisionInfo_0x1129550(rdram, ctx, runtime);
}
// pop_camera_context (0x1160f18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void pop_camera_context(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160F18u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B91A0u, lift_d1::iosUnitMatrix) && A7_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A7_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A7_HOLDS(0x11601C8u, lift_d1::manager_sub_011601C8) && A7_HOLDS(0x1160C20u, lift_d1::SetVSMatrix) && A5_JAL(0x11601F4u, 0x1C2038u) && A5_JAL(0x1160210u, 0x1C2048u) && A5_JAL(0x11602A0u, 0x1B91A0u) && A5_JAL(0x11602D0u, 0x1B91A0u) && A5_JAL(0x11603B4u, 0x1B91A0u) && A5_JAL(0x1160420u, 0x1B91A0u) && A5_JAL(0x11604D8u, 0x1B91A0u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A7_CHK(pop_camera_context, a2_29, a2_f20, a2_f21, a2_f22, a2_f23); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf24 = ctx->vu0_vf[24]; a4::vec4 a4_vf25 = ctx->vu0_vf[25]; a4::vec4 a4_vf26 = ctx->vu0_vf[26]; a4::vec4 a4_vf27 = ctx->vu0_vf[27]; a4::vec4 a4_vacc = ctx->vu0_acc; A7_IN(503); const uint64_t a2_r = typed::pop_camera_context(rdram, runtime, a2_29, a2_f20, a2_f21, a2_f22, a2_f23, a4_vf0, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf24, a4_vf25, a4_vf26, a4_vf27, a4_vacc); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[24] = a4_vf24; ctx->vu0_vf[25] = a4_vf25; ctx->vu0_vf[26] = a4_vf26; ctx->vu0_vf[27] = a4_vf27; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::pop_camera_context_0x1160f18(rdram, ctx, runtime);
}
// doit_camera_region (0x1163538): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void doit_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1163538u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1BBE10u, lift_d1::DivQuaternion) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x1163788u, lift_d1::is_parent_obj_boss_camera_region) && A5_JAL(0x11635B8u, 0x1BBE10u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(doit_camera_region, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A7_IN(504); const uint64_t a2_r = typed::doit_camera_region(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf5, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::doit_camera_region_0x1163538(rdram, ctx, runtime);
}
// getBootParent (0x1169648): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBootParent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1169648u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getBootParent, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(506); const uint64_t a2_r = typed::getBootParent(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getBootParent_0x1169648(rdram, ctx, runtime);
}
// DebugIsPlayer (0x1341570): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugIsPlayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341570u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1341578u, 0x10F1A20u) && A5_JAL(0x134158Cu, 0x128E6Cu), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(DebugIsPlayer, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(507); const uint64_t a2_r = typed::DebugIsPlayer(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::DebugIsPlayer_0x1341570(rdram, ctx, runtime);
}
// getSkeletonGlobalGeo (0x1376128): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSkeletonGlobalGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1376128u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1376164u, 0x10F1A20u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(getSkeletonGlobalGeo, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(520); const uint64_t a2_r = typed::getSkeletonGlobalGeo(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getSkeletonGlobalGeo_0x1376128(rdram, ctx, runtime);
}
// SCRFuncCerberusSetPlayerParentObj (0x137b748): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncCerberusSetPlayerParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137B748u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13B5390u, lift_d1::getPlayerColCont2) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncCerberusSetPlayerParentObj, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(522); const uint64_t a2_r = typed::SCRFuncCerberusSetPlayerParentObj(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncCerberusSetPlayerParentObj_0x137b748(rdram, ctx, runtime);
}
// setGameFlagForGameObj (0x137df08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setGameFlagForGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137DF08u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A7_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls) && A7_HOLDS(0x1353638u, ::a6ad::gStatGobjDispGflagSet) && A5_JAL(0x137DF14u, 0x10F1A20u) && A5_JAL(0x137DF24u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(setGameFlagForGameObj, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(524); const uint64_t a2_r = typed::setGameFlagForGameObj(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::setGameFlagForGameObj_0x137df08(rdram, ctx, runtime);
}
// chkGameFlagForGameObj (0x137e020): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkGameFlagForGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E020u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x134F030u, lift_d1::_gameFlagChk) && A7_HOLDS(0x13535E8u, lift_d1::gStatGobjDispGflagChk) && A5_JAL(0x137E02Cu, 0x10F1A20u) && A5_JAL(0x137E03Cu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(chkGameFlagForGameObj, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(525); const uint64_t a2_r = typed::chkGameFlagForGameObj(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::chkGameFlagForGameObj_0x137e020(rdram, ctx, runtime);
}
// SCRSetMyOwnGameFlag (0x137e4d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRSetMyOwnGameFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E4D0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A5_JAL(0x137E4D8u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRSetMyOwnGameFlag, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(526); const uint64_t a2_r = typed::SCRSetMyOwnGameFlag(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRSetMyOwnGameFlag_0x137e4d0(rdram, ctx, runtime);
}
// SCRResetMyOwnGameFlag (0x137e558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRResetMyOwnGameFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E558u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls) && A5_JAL(0x137E560u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRResetMyOwnGameFlag, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(527); const uint64_t a2_r = typed::SCRResetMyOwnGameFlag(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRResetMyOwnGameFlag_0x137e558(rdram, ctx, runtime);
}
// SCRConditionHorseCalled (0x13961d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionHorseCalled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13961D8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13B5158u, ::a7ad::IsHorseCalled_withFlgOff) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionHorseCalled, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(534); const uint64_t a2_r = typed::SCRConditionHorseCalled(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionHorseCalled_0x13961d8(rdram, ctx, runtime);
}
// IsDamaged (0x139a808): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsDamaged(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A808u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1166C10u, lift_d1::GetDamageClass) && A7_HOLDS(0x1166E98u, lift_d1::GetDamagerClassDamageSheetType) && A5_JAL(0x139A818u, 0x1166C10u) && A5_JAL(0x139A83Cu, 0x1166E98u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(IsDamaged, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(537); const uint64_t a2_r = typed::IsDamaged(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsDamaged_0x139a808(rdram, ctx, runtime);
}
// IsColStatus (0x139ac68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139AC68u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x139AC90u, 0x10F1A20u) && A5_JAL(0x139AC9Cu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(IsColStatus, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(540); const uint64_t a2_r = typed::IsColStatus(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsColStatus_0x139ac68(rdram, ctx, runtime);
}
// IsColStatusCUP (0x139ace8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsColStatusCUP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139ACE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x139AD10u, 0x10F1A20u) && A5_JAL(0x139AD1Cu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(IsColStatusCUP, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(541); const uint64_t a2_r = typed::IsColStatusCUP(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::IsColStatusCUP_0x139ace8(rdram, ctx, runtime);
}
// SCRFuncKirinCameraNeckDownIdReset (0x139cbd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncKirinCameraNeckDownIdReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139CBD0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x139CBD8u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncKirinCameraNeckDownIdReset, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(543); const uint64_t a2_r = typed::SCRFuncKirinCameraNeckDownIdReset(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncKirinCameraNeckDownIdReset_0x139cbd0(rdram, ctx, runtime);
}
// gamecore_sub_013AD738 (0x13ad738): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013AD738(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AD738u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A7_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A7_HOLDS(0x1CBDB8u, ::decomp_sym120::makeHash) && A7_HOLDS(0x1111F90u, lift_d1::SpriteMenuGetLayerByHash) && A7_HOLDS(0x1112530u, ::a7ad::SpriteMenuGetPartsDefValueByHashStr) && A7_HOLDS(0x1112728u, lift_d1::SpriteMenuSetPartsDefValueByHashStr) && A5_JAL(0x111254Cu, 0x1CBDB8u) && A5_JAL(0x1112558u, 0x1CBDB8u) && A5_JAL(0x111274Cu, 0x1CBDB8u) && A5_JAL(0x1112758u, 0x1CBDB8u) && A5_JAL(0x13AD77Cu, 0x1CBDB8u) && A5_JAL(0x13AD788u, 0x1111F90u) && A5_JAL(0x13AD7B8u, 0x1112530u) && A5_JAL(0x13AD7E0u, 0x1112530u) && A5_JAL(0x13AD7FCu, 0x1112728u) && A5_JAL(0x13AD854u, 0x1112728u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013AD738, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(549); const uint64_t a2_r = typed::gamecore_sub_013AD738(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013AD738_0x13ad738(rdram, ctx, runtime);
}
// player_IsSwiming (0x13b5208): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_IsSwiming(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5208u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1343290u, ::a7ad::GAMEUTIL_is_swimming_now) && A7_HOLDS(0x1343468u, lift_d1::GAMEUTIL_is_player_obj) && A5_JAL(0x13432B0u, 0x10C0A70u) && A5_JAL(0x1343474u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(player_IsSwiming, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(550); const uint64_t a2_r = typed::player_IsSwiming(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::player_IsSwiming_0x13b5208(rdram, ctx, runtime);
}
// getPlayerPosAfterXSeconds (0x13b5220): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerPosAfterXSeconds(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5220u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13B525Cu, 0x10F1A20u) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(getPlayerPosAfterXSeconds, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); A7_IN(551); const uint64_t a2_r = typed::getPlayerPosAfterXSeconds(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_h2, a4_h8); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::getPlayerPosAfterXSeconds_0x13b5220(rdram, ctx, runtime);
}
// SCRPlayerRotateStop (0x13b6040): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerRotateStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6040u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13B6048u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRPlayerRotateStop, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(552); const a1::RetVF a2_r = typed::SCRPlayerRotateStop(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRPlayerRotateStop_0x13b6040(rdram, ctx, runtime);
}
// gamecore_sub_013D5FC0 (0x13d5fc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013D5FC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5FC0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1102438u, ::decomp_scriptvm3::getScriptClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11119B8u, lift_d1::GetScriptSpriteMenuClass) && A7_HOLDS(0x1114238u, lift_d1::GetScriptSpriteClass) && A7_HOLDS(0x1156628u, ::a6ad::GetWindowClass) && A7_HOLDS(0x13D49E8u, lift_d1::AI_GetCurMotionKind) && A5_JAL(0x13D49F0u, 0x10C0A70u) && A5_JAL(0x13D5FD0u, 0x1CA1B0u) && A5_JAL(0x13D5FDCu, 0x1102438u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013D5FC0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(561); const uint64_t a2_r = typed::gamecore_sub_013D5FC0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013D5FC0_0x13d5fc0(rdram, ctx, runtime);
}
// gamecore_sub_013D84F0 (0x13d84f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013D84F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D84F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13E7C78u, lift_d1::Kite_SetAnimalLayout) && A7_HOLDS(0x13F0D60u, lift_d1::getSObjNode) && A5_JAL(0x13D8504u, 0x1CA1B0u) && A5_JAL(0x13D8588u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013D84F0, a2_2, a2_4, a2_6, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[11] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A7_IN(562); const a1::RetVF a2_r = typed::gamecore_sub_013D84F0(rdram, runtime, a2_2, a2_4, a2_6, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->f[14] = a1_flt((uint32_t)a3_xo[2]); Ps2SetGpr64(ctx, 4, a3_xo[3]); Ps2SetGpr64(ctx, 5, a3_xo[4]); Ps2SetGpr64(ctx, 6, a3_xo[5]); Ps2SetGpr64(ctx, 7, a3_xo[6]); Ps2SetGpr64(ctx, 8, a3_xo[7]); Ps2SetGpr64(ctx, 9, a3_xo[8]); Ps2SetGpr64(ctx, 10, a3_xo[9]); Ps2SetGpr64(ctx, 11, a3_xo[10]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013D84F0_0x13d84f0(rdram, ctx, runtime);
}
// gamecore_sub_013D9138 (0x13d9138): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013D9138(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D9138u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1102438u, ::decomp_scriptvm3::getScriptClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11119B8u, lift_d1::GetScriptSpriteMenuClass) && A7_HOLDS(0x1114238u, lift_d1::GetScriptSpriteClass) && A7_HOLDS(0x1156628u, ::a6ad::GetWindowClass) && A7_HOLDS(0x13D49E8u, lift_d1::AI_GetCurMotionKind) && A5_JAL(0x13D49F0u, 0x10C0A70u) && A5_JAL(0x13D9148u, 0x1CA1B0u) && A5_JAL(0x13D9154u, 0x1102438u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013D9138, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(563); const uint64_t a2_r = typed::gamecore_sub_013D9138(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013D9138_0x13d9138(rdram, ctx, runtime);
}
// gamecore_sub_013DA430 (0x13da430): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013DA430(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13DA430u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13D4A60u, lift_d1::AI_GetInterRatioOutCut) && A5_JAL(0x13DA490u, 0x1CA1B0u) && A5_JAL(0x13DA49Cu, 0x10F1A20u) && A5_JAL(0x13DA4A8u, 0x10F1A20u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; A7_CHK(gamecore_sub_013DA430, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[6] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), (uint64_t)a1_bits(ctx->f[15]), (uint64_t)a1_bits(ctx->f[16]), Ps2GprU64(ctx, 5)}; A7_IN(564); const a1::RetVF a2_r = typed::gamecore_sub_013DA430(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_vq, a4_vstat, a4_h2, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->f[14] = a1_flt((uint32_t)a3_xo[2]); ctx->f[15] = a1_flt((uint32_t)a3_xo[3]); ctx->f[16] = a1_flt((uint32_t)a3_xo[4]); Ps2SetGpr64(ctx, 5, a3_xo[5]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013DA430_0x13da430(rdram, ctx, runtime);
}
// Kite_InitKiteAdvertisePathWork (0x13e7bd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Kite_InitKiteAdvertisePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13E7BD8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x13D3980u, lift_d1::AI_InitAnimalLayout) && A7_HOLDS(0x13E6A58u, lift_d1::Kite_InitKitePathWork) && A7_HOLDS(0x13E7900u, lift_d1::Kite_InitSpecialPathWork) && A7_HOLDS(0x13F0D60u, lift_d1::getSObjNode), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(Kite_InitKiteAdvertisePathWork, a2_2, a2_4, a2_5, a2_8, a2_f12, a2_f13, a2_f20, a2_f21); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[8] = {(uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A7_IN(571); const uint64_t a2_r = typed::Kite_InitKiteAdvertisePathWork(rdram, runtime, a2_2, a2_4, a2_5, a2_8, a2_f12, a2_f13, a2_f20, a2_f21, a4_vf0, a4_h2, a4_h3, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); Ps2SetGpr64(ctx, 10, a3_xo[6]); Ps2SetGpr64(ctx, 11, a3_xo[7]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::Kite_InitKiteAdvertisePathWork_0x13e7bd8(rdram, ctx, runtime);
}
// gamecore_sub_013E8588 (0x13e8588): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013E8588(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13E8588u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1102438u, ::decomp_scriptvm3::getScriptClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11119B8u, lift_d1::GetScriptSpriteMenuClass) && A7_HOLDS(0x1114238u, lift_d1::GetScriptSpriteClass) && A7_HOLDS(0x1156628u, ::a6ad::GetWindowClass) && A7_HOLDS(0x13D49E8u, lift_d1::AI_GetCurMotionKind) && A5_JAL(0x13D49F0u, 0x10C0A70u) && A5_JAL(0x13E8598u, 0x1CA1B0u) && A5_JAL(0x13E85A4u, 0x1102438u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013E8588, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(573); const uint64_t a2_r = typed::gamecore_sub_013E8588(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013E8588_0x13e8588(rdram, ctx, runtime);
}
// gamecore_sub_013E86F0 (0x13e86f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013E86F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13E86F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x1102438u, ::decomp_scriptvm3::getScriptClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11119B8u, lift_d1::GetScriptSpriteMenuClass) && A7_HOLDS(0x1114238u, lift_d1::GetScriptSpriteClass) && A7_HOLDS(0x1156628u, ::a6ad::GetWindowClass) && A7_HOLDS(0x13D49E8u, lift_d1::AI_GetCurMotionKind) && A5_JAL(0x13D49F0u, 0x10C0A70u) && A5_JAL(0x13E8704u, 0x1CA1B0u) && A5_JAL(0x13E8710u, 0x1102438u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013E86F0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(574); const uint64_t a2_r = typed::gamecore_sub_013E86F0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013E86F0_0x13e86f0(rdram, ctx, runtime);
}
// SCRFuncUnageMainBossNotParentTime (0x1406ac0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncUnageMainBossNotParentTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1406AC0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x139AA00u, lift_d1::ColCont2_IsColStatus) && A7_HOLDS(0x13B5390u, lift_d1::getPlayerColCont2) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncUnageMainBossNotParentTime, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(578); const uint64_t a2_r = typed::SCRFuncUnageMainBossNotParentTime(rdram, runtime, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncUnageMainBossNotParentTime_0x1406ac0(rdram, ctx, runtime);
}
// SCRFuncYamoriBGounrdAttackMain (0x140d0d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBGounrdAttackMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D0D8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x140D0E4u, 0x10F1A20u) && A5_JAL(0x140D0F8u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBGounrdAttackMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(579); const uint64_t a2_r = typed::SCRFuncYamoriBGounrdAttackMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBGounrdAttackMain_0x140d0d8(rdram, ctx, runtime);
}
// SCRFuncYamoriBSetAttainPosForGroundAttack (0x140d640): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncYamoriBSetAttainPosForGroundAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140D640u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14324F8u, lift_d1::setAttainmentPosAndDirAndLock) && A7_HOLDS(0x14418F0u, lift_d1::setPosGazeCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1432524u, 0x128188u) && A5_JAL(0x143255Cu, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncYamoriBSetAttainPosForGroundAttack, a2_2, a2_4, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(580); const a1::RetVF a2_r = typed::SCRFuncYamoriBSetAttainPosForGroundAttack(rdram, runtime, a2_2, a2_4, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncYamoriBSetAttainPosForGroundAttack_0x140d640(rdram, ctx, runtime);
}
// CAM_set_target_display_flag (0x141a760): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_set_target_display_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A760u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x141A77Cu, 0x10F1A20u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(CAM_set_target_display_flag, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(586); const uint64_t a2_r = typed::CAM_set_target_display_flag(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::CAM_set_target_display_flag_0x141a760(rdram, ctx, runtime);
}
// setAttainmentObjAndDir (0x14325b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setAttainmentObjAndDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14325B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x14325F0u, 0x1147AB0u) && A5_JAL(0x1432600u, 0x10F1A20u) && A5_JAL(0x143260Cu, 0x1147A88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(setAttainmentObjAndDir, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(591); const a1::RetVF a2_r = typed::setAttainmentObjAndDir(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::setAttainmentObjAndDir_0x14325b0(rdram, ctx, runtime);
}
// setAttainmentObjAndOffset (0x1432670): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setAttainmentObjAndOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1432670u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x14326A8u, 0x1147AB0u) && A5_JAL(0x14326B8u, 0x10F1A20u) && A5_JAL(0x14326C4u, 0x1147A88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(setAttainmentObjAndOffset, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(592); const uint64_t a2_r = typed::setAttainmentObjAndOffset(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::setAttainmentObjAndOffset_0x1432670(rdram, ctx, runtime);
}
// SCRConditionLessAngryValue (0x144d1e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionLessAngryValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144D1E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionLessAngryValue, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(602); const a1::RetVF a2_r = typed::SCRConditionLessAngryValue(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionLessAngryValue_0x144d1e8(rdram, ctx, runtime);
}
// SCRFuncSetAttainLostPlayerPos (0x144fb40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainLostPlayerPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FB40u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetAttainLostPlayerPos, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(610); const a1::RetVF a2_r = typed::SCRFuncSetAttainLostPlayerPos(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAttainLostPlayerPos_0x144fb40(rdram, ctx, runtime);
}
// SCRFuncSetAttainLostPlayerPosGlobalY (0x144fb88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainLostPlayerPosGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FB88u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetAttainLostPlayerPosGlobalY, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(611); const uint64_t a2_r = typed::SCRFuncSetAttainLostPlayerPosGlobalY(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAttainLostPlayerPosGlobalY_0x144fb88(rdram, ctx, runtime);
}
// SCRFuncSetAttainMyselfPosAndGlobalY (0x144fbc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainMyselfPosAndGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FBC8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u) && A5_JAL(0x144FBECu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetAttainMyselfPosAndGlobalY, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(612); const uint64_t a2_r = typed::SCRFuncSetAttainMyselfPosAndGlobalY(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAttainMyselfPosAndGlobalY_0x144fbc8(rdram, ctx, runtime);
}
// SCRFuncSetAttainPlayerPosAndGlobalY (0x144fc28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainPlayerPosAndGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FC28u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14320B8u, lift_d1::setAttainmentPos) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14320DCu, 0x128188u) && A5_JAL(0x1432114u, 0x1147AB0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetAttainPlayerPosAndGlobalY, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(613); const uint64_t a2_r = typed::SCRFuncSetAttainPlayerPosAndGlobalY(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAttainPlayerPosAndGlobalY_0x144fc28(rdram, ctx, runtime);
}
// SCRFuncSetAttainMyselfFrontPos (0x144fd28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAttainMyselfFrontPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144FD28u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x128188u, ::memcmp_0x128188) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14321D8u, lift_d1::setAttainmentPosAndLock) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x14321FCu, 0x128188u) && A5_JAL(0x1432234u, 0x1147AB0u) && A5_JAL(0x144FD4Cu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetAttainMyselfFrontPos, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(614); const a1::RetVF a2_r = typed::SCRFuncSetAttainMyselfFrontPos(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAttainMyselfFrontPos_0x144fd28(rdram, ctx, runtime);
}
// SCRFuncResetVolumeTime0 (0x14506e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncResetVolumeTime0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14506E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncResetVolumeTime0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(616); const uint64_t a2_r = typed::SCRFuncResetVolumeTime0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncResetVolumeTime0_0x14506e8(rdram, ctx, runtime);
}
// SCRFuncSetGpCalcDist (0x1450bd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetGpCalcDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1450BD0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1435B50u, ::a6ad::setGpCalcDist) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetGpCalcDist, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(617); typed::SCRFuncSetGpCalcDist(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetGpCalcDist_0x1450bd0(rdram, ctx, runtime);
}
// SCRFuncSetOrientCheckWork (0x1452248): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetOrientCheckWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452248u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetOrientCheckWork, a2_2, a2_4, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(622); const uint64_t a2_r = typed::SCRFuncSetOrientCheckWork(rdram, runtime, a2_2, a2_4, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetOrientCheckWork_0x1452248(rdram, ctx, runtime);
}
// SCRFuncForceSetPerceptionWork (0x1452358): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncForceSetPerceptionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452358u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A7_HOLDS(0x145CA68u, ::a6ad::forceSetPerceptionWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncForceSetPerceptionWork, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(623); typed::SCRFuncForceSetPerceptionWork(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncForceSetPerceptionWork_0x1452358(rdram, ctx, runtime);
}
// SCRFuncForceRenewLastPos (0x1452388): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncForceRenewLastPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452388u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A7_HOLDS(0x145CAB0u, ::a6ad::forceRenewLastPosPerceptionWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncForceRenewLastPos, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(624); const uint64_t a2_r = typed::SCRFuncForceRenewLastPos(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncForceRenewLastPos_0x1452388(rdram, ctx, runtime);
}
// SCRFuncResetWormCtrlParam (0x14523b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncResetWormCtrlParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14523B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CDB8u, lift_d1::resetVelocityWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncResetWormCtrlParam, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(625); const uint64_t a2_r = typed::SCRFuncResetWormCtrlParam(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncResetWormCtrlParam_0x14523b8(rdram, ctx, runtime);
}
// SCRFuncSetRotColCheckWormCtrlParam (0x1452400): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetRotColCheckWormCtrlParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452400u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CD40u, lift_d1::setRotColCheckWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetRotColCheckWormCtrlParam, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(626); typed::SCRFuncSetRotColCheckWormCtrlParam(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetRotColCheckWormCtrlParam_0x1452400(rdram, ctx, runtime);
}
// SCRFuncSetMiddleTargetWormCtrlParam (0x1452448): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetMiddleTargetWormCtrlParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452448u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CD48u, ::a6ad::setMiddleTargetWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetMiddleTargetWormCtrlParam, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(627); typed::SCRFuncSetMiddleTargetWormCtrlParam(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetMiddleTargetWormCtrlParam_0x1452448(rdram, ctx, runtime);
}
// SCRFuncSetAcceleParamWormCtrlParam (0x1452490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAcceleParamWormCtrlParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452490u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CC60u, lift_d1::setAttainSpeedWormCtrlWork) && A7_HOLDS(0x144CDD8u, ::a6ad::setAcceleParamWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A7_CHK(SCRFuncSetAcceleParamWormCtrlParam, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(628); typed::SCRFuncSetAcceleParamWormCtrlParam(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetAcceleParamWormCtrlParam_0x1452490(rdram, ctx, runtime);
}
// SCRFuncSetSearchRadius (0x14525c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetSearchRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14525C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14328A8u, lift_d1::setSearchRadius) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetSearchRadius, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(629); const a1::RetVF a2_r = typed::SCRFuncSetSearchRadius(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetSearchRadius_0x14525c0(rdram, ctx, runtime);
}
// SCRFuncResetSearchRadius (0x14525f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncResetSearchRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14525F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14328A8u, lift_d1::setSearchRadius) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncResetSearchRadius, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(630); const a1::RetVF a2_r = typed::SCRFuncResetSearchRadius(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncResetSearchRadius_0x14525f0(rdram, ctx, runtime);
}
// SCRFuncSetObstacleWormCtrl (0x14526f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetObstacleWormCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14526F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CD50u, lift_d1::setObstacleWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetObstacleWormCtrl, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(631); typed::SCRFuncSetObstacleWormCtrl(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetObstacleWormCtrl_0x14526f0(rdram, ctx, runtime);
}
// SCRFuncSetPerceptionParam (0x1452db8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetPerceptionParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452DB8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A7_HOLDS(0x145C990u, lift_d1::setPerceptionParam) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetPerceptionParam, a2_2, a2_4, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(632); typed::SCRFuncSetPerceptionParam(rdram, runtime, a2_2, a2_4, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetPerceptionParam_0x1452db8(rdram, ctx, runtime);
}
// SCRFuncSetGripDamage (0x1452e88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetGripDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452E88u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13B5678u, ::a7ad::player_SetGripDamage) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRFuncSetGripDamage, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(633); const uint64_t a2_r = typed::SCRFuncSetGripDamage(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetGripDamage_0x1452e88(rdram, ctx, runtime);
}
// SCRFuncSetPassageRangeData (0x1452ec0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetPassageRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452EC0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1432020u, lift_d1::setPassageRangeData) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(SCRFuncSetPassageRangeData, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(634); typed::SCRFuncSetPassageRangeData(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetPassageRangeData_0x1452ec0(rdram, ctx, runtime);
}
// SCRFuncSetCreateTempAttain (0x1453138): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetCreateTempAttain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1453138u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14320B0u, lift_d1::setCreateTempAttain) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSetCreateTempAttain, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(637); typed::SCRFuncSetCreateTempAttain(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSetCreateTempAttain_0x1453138(rdram, ctx, runtime);
}
// SCRConditionDamagedType (0x14563f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionDamagedType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14563F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1166C10u, lift_d1::GetDamageClass) && A7_HOLDS(0x1166E98u, lift_d1::GetDamagerClassDamageSheetType) && A5_JAL(0x14563FCu, 0x1166C10u) && A5_JAL(0x1456418u, 0x1166E98u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionDamagedType, a2_2, a2_4, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(638); const uint64_t a2_r = typed::SCRConditionDamagedType(rdram, runtime, a2_2, a2_4, a2_8, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionDamagedType_0x14563f0(rdram, ctx, runtime);
}
// SCRConditionDamagedWeaponType (0x1456440): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionDamagedWeaponType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1456440u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1166C10u, lift_d1::GetDamageClass) && A5_JAL(0x145644Cu, 0x1166C10u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionDamagedWeaponType, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(639); const uint64_t a2_r = typed::SCRConditionDamagedWeaponType(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionDamagedWeaponType_0x1456440(rdram, ctx, runtime);
}
// SCRConditionPlayerParentRangeColAttrAndEquipmentCheck (0x1456d48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPlayerParentRangeColAttrAndEquipmentCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1456D48u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x134FB28u, lift_d1::_omakeSelectFlagChk) && A7_HOLDS(0x139AA00u, lift_d1::ColCont2_IsColStatus) && A7_HOLDS(0x13B5390u, lift_d1::getPlayerColCont2) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A7_HOLDS(0x145CAD0u, lift_d1::equipmentInvisibilityCloak) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPlayerParentRangeColAttrAndEquipmentCheck, a2_2, a2_4, a2_8, a2_9, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(641); const uint64_t a2_r = typed::SCRConditionPlayerParentRangeColAttrAndEquipmentCheck(rdram, runtime, a2_2, a2_4, a2_8, a2_9, a2_10, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPlayerParentRangeColAttrAndEquipmentCheck_0x1456d48(rdram, ctx, runtime);
}
// SCRConditionMyselfOverGlobalY (0x14577a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMyselfOverGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14577A0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x14577ACu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionMyselfOverGlobalY, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(647); const a1::RetVF a2_r = typed::SCRConditionMyselfOverGlobalY(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMyselfOverGlobalY_0x14577a0(rdram, ctx, runtime);
}
// SCRConditionMyselfLessGlobalY (0x14577e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMyselfLessGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14577E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x14577ECu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionMyselfLessGlobalY, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(648); const a1::RetVF a2_r = typed::SCRConditionMyselfLessGlobalY(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMyselfLessGlobalY_0x14577e0(rdram, ctx, runtime);
}
// SCRConditionPlayerLessGlobalY (0x1457820): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPlayerLessGlobalY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1457820u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionPlayerLessGlobalY, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(649); const a1::RetVF a2_r = typed::SCRConditionPlayerLessGlobalY(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPlayerLessGlobalY_0x1457820(rdram, ctx, runtime);
}
// SCRConditionPPFinish (0x14578f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPPFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14578F8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x14359E8u, lift_d1::getPathExFinish) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPPFinish, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(653); const uint64_t a2_r = typed::SCRConditionPPFinish(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPPFinish_0x14578f8(rdram, ctx, runtime);
}
// SCRConditionPPPassageNum (0x1457920): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPPPassageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1457920u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1435A30u, ::a6ad::getPPointPasasgeNum) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPPPassageNum, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(654); const uint64_t a2_r = typed::SCRConditionPPPassageNum(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPPPassageNum_0x1457920(rdram, ctx, runtime);
}
// SCRConditionPathPassageNum (0x1457ae8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPathPassageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1457AE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431D18u, lift_d1::getPassageNum) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPathPassageNum, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(657); const uint64_t a2_r = typed::SCRConditionPathPassageNum(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPathPassageNum_0x1457ae8(rdram, ctx, runtime);
}
// SCRConditionPathPassageNumOver (0x1457b20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionPathPassageNumOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1457B20u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431D18u, lift_d1::getPassageNum) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionPathPassageNumOver, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(658); const uint64_t a2_r = typed::SCRConditionPathPassageNumOver(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionPathPassageNumOver_0x1457b20(rdram, ctx, runtime);
}
// SCRConditionVolumeTime0 (0x14580a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionVolumeTime0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14580A0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionVolumeTime0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(669); const a1::RetVF a2_r = typed::SCRConditionVolumeTime0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionVolumeTime0_0x14580a0(rdram, ctx, runtime);
}
// SCRConditionOverMyselfPartsLife (0x1458d40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOverMyselfPartsLife(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458D40u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13784B8u, lift_d1::getBossPartsLifeRatio) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionOverMyselfPartsLife, a2_2, a2_4, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(673); const a1::RetVF a2_r = typed::SCRConditionOverMyselfPartsLife(rdram, runtime, a2_2, a2_4, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionOverMyselfPartsLife_0x1458d40(rdram, ctx, runtime);
}
// SCRConditionOverMyselfWeakLife (0x1458dd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOverMyselfWeakLife(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458DD0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1378608u, lift_d1::getBossWeakLifeRatio) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionOverMyselfWeakLife, a2_2, a2_4, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(675); const a1::RetVF a2_r = typed::SCRConditionOverMyselfWeakLife(rdram, runtime, a2_2, a2_4, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionOverMyselfWeakLife_0x1458dd0(rdram, ctx, runtime);
}
// SCRConditionMyselfPartsDamaged (0x1458e60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionMyselfPartsDamaged(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458E60u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x1378DD8u, ::a7ad::getBossPartsCurDamage) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionMyselfPartsDamaged, a2_2, a2_4, a2_5, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(677); const a1::RetVF a2_r = typed::SCRConditionMyselfPartsDamaged(rdram, runtime, a2_2, a2_4, a2_5, a2_9, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionMyselfPartsDamaged_0x1458e60(rdram, ctx, runtime);
}
// SCRConditionExecGazeToPlayer (0x1458ec0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionExecGazeToPlayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458EC0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1431D18u, lift_d1::getPassageNum) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1458EE4u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionExecGazeToPlayer, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(679); const uint64_t a2_r = typed::SCRConditionExecGazeToPlayer(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionExecGazeToPlayer_0x1458ec0(rdram, ctx, runtime);
}
// SCRConditionLessPlayerLife (0x1458f98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionLessPlayerLife(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458F98u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x134F120u, lift_d1::_gameFWorkChk) && A7_HOLDS(0x1358248u, lift_d1::getPlayerHp) && A7_HOLDS(0x13583B8u, lift_d1::getPlayerHpRatio) && A7_HOLDS(0x13583F8u, lift_d1::getPlayerMaxHp), 1)) { const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionLessPlayerLife, a2_f0, a2_f12, a2_f20); A7_IN(681); const a1::RetVF a2_r = typed::SCRConditionLessPlayerLife(rdram, runtime, a2_f0, a2_f12, a2_f20); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionLessPlayerLife_0x1458f98(rdram, ctx, runtime);
}
// SCRConditionOverRandomVal (0x14590e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOverRandomVal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14590E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionOverRandomVal, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(682); const a1::RetVF a2_r = typed::SCRConditionOverRandomVal(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionOverRandomVal_0x14590e0(rdram, ctx, runtime);
}
// SCRConditionLessRandomVal (0x1459118): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionLessRandomVal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459118u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionLessRandomVal, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(683); const a1::RetVF a2_r = typed::SCRConditionLessRandomVal(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionLessRandomVal_0x1459118(rdram, ctx, runtime);
}
// SCRConditionOverLengthXZPlayerFromGp (0x1459230): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOverLengthXZPlayerFromGp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459230u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x1169EA8u, lift_d1::getGoalPointData) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1433CE0u, lift_d1::getGoalPointDataByAreaIdx) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1433D5Cu, 0x1169EA8u) && A5_JAL(0x145926Cu, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionOverLengthXZPlayerFromGp, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(684); const uint64_t a2_r = typed::SCRConditionOverLengthXZPlayerFromGp(rdram, runtime, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionOverLengthXZPlayerFromGp_0x1459230(rdram, ctx, runtime);
}
// SCRConditionLessLengthXZPlayerFromGp (0x14592c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionLessLengthXZPlayerFromGp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14592C8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x1169EA8u, lift_d1::getGoalPointData) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1433CE0u, lift_d1::getGoalPointDataByAreaIdx) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x1433D5Cu, 0x1169EA8u) && A5_JAL(0x1459304u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionLessLengthXZPlayerFromGp, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(685); const uint64_t a2_r = typed::SCRConditionLessLengthXZPlayerFromGp(rdram, runtime, a2_2, a2_4, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionLessLengthXZPlayerFromGp_0x14592c8(rdram, ctx, runtime);
}
// SCRConditionEyeModeLapsedTime (0x1459670): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionEyeModeLapsedTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459670u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCRConditionEyeModeLapsedTime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(687); const a1::RetVF a2_r = typed::SCRConditionEyeModeLapsedTime(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionEyeModeLapsedTime_0x1459670(rdram, ctx, runtime);
}
// SCRConditionOldModeStatusByMySelf (0x1459858): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOldModeStatusByMySelf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459858u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionOldModeStatusByMySelf, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(689); const uint64_t a2_r = typed::SCRConditionOldModeStatusByMySelf(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionOldModeStatusByMySelf_0x1459858(rdram, ctx, runtime);
}
// SCRConditionSpecifiedModeByMySelf (0x14598e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionSpecifiedModeByMySelf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14598E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionSpecifiedModeByMySelf, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(690); const uint64_t a2_r = typed::SCRConditionSpecifiedModeByMySelf(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionSpecifiedModeByMySelf_0x14598e0(rdram, ctx, runtime);
}
// SCRConditionCheckBuryingWormCtrl (0x1459c38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionCheckBuryingWormCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459C38u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CDF0u, ::a6ad::getBuryingWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionCheckBuryingWormCtrl, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(696); const uint64_t a2_r = typed::SCRConditionCheckBuryingWormCtrl(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionCheckBuryingWormCtrl_0x1459c38(rdram, ctx, runtime);
}
// SCRConditionHintOverPlayCount (0x145bbe8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionHintOverPlayCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145BBE8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRConditionHintOverPlayCount, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(700); const uint64_t a2_r = typed::SCRConditionHintOverPlayCount(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRConditionHintOverPlayCount_0x145bbe8(rdram, ctx, runtime);
}
// SCRFuncSoundResetPlayerAttrInfo (0x1464c88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSoundResetPlayerAttrInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464C88u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x139A9F8u, ::a6ad::ColCont2_EventAttrReset) && A7_HOLDS(0x13B5390u, lift_d1::getPlayerColCont2) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1465308u, lift_d1::getSScriptWork) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSoundResetPlayerAttrInfo, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(702); typed::SCRFuncSoundResetPlayerAttrInfo(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSoundResetPlayerAttrInfo_0x1464c88(rdram, ctx, runtime);
}
// SCREventConditionOverDistanceToPlayerWorldXZ (0x1465020): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCREventConditionOverDistanceToPlayerWorldXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1465020u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x1465308u, lift_d1::getSScriptWork) && A5_JAL(0x14650A0u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_11 = Ps2GprU64(ctx, 11); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A7_CHK(SCREventConditionOverDistanceToPlayerWorldXZ, a2_2, a2_4, a2_11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(703); const uint64_t a2_r = typed::SCREventConditionOverDistanceToPlayerWorldXZ(rdram, runtime, a2_2, a2_4, a2_11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCREventConditionOverDistanceToPlayerWorldXZ_0x1465020(rdram, ctx, runtime);
}
// DebugGetParentCharColAttr (0x10c2010): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugGetParentCharColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2010u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1132D58u, lift_d1::CheckLegalCollisionGP) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(DebugGetParentCharColAttr, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A7_IN(707); const uint64_t a2_r = typed::DebugGetParentCharColAttr(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h6, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::DebugGetParentCharColAttr_0x10c2010(rdram, ctx, runtime);
}
// SendCharaNodeParentSetMsg (0x10ccb38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SendCharaNodeParentSetMsg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CCB38u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F20C0u, lift_d1::ForceParentLayoutObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11327B0u, lift_d1::CopyGeometryPointer) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1132C60u, lift_d1::SetGeometryPointerNodePosState) && A7_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SendCharaNodeParentSetMsg, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 8)}; A7_IN(709); const uint64_t a2_r = typed::SendCharaNodeParentSetMsg(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 8, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SendCharaNodeParentSetMsg_0x10ccb38(rdram, ctx, runtime);
}
// charrootdyn_setChainFindStatus (0x10dfb00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void charrootdyn_setChainFindStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DFB00u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C2BE8u, lift_d1::UnlinkSecureParentInfo) && A7_HOLDS(0x10C2C20u, lift_d1::LinkSecureParentInfoByGP) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11327B0u, lift_d1::CopyGeometryPointer) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(charrootdyn_setChainFindStatus, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(712); const uint64_t a2_r = typed::charrootdyn_setChainFindStatus(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::charrootdyn_setChainFindStatus_0x10dfb00(rdram, ctx, runtime);
}
// directplay (0x10e0368): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void directplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E0368u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10DE1A0u, lift_d1::charrootdyn_getRegularColFinderPosition) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F1FC0u, lift_d1::ForceUnparentLayoutObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11328F8u, lift_d1::SetGeometryPointerGlobalState) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(directplay, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf9 = ctx->vu0_vf[9]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); A7_IN(714); const a1::RetVF a2_r = typed::directplay(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf9, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2, a4_h3, a4_h4, a4_h5, a4_h8); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[9] = a4_vf9; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::directplay_0x10e0368(rdram, ctx, runtime);
}
// gamecore_sub_013417B8 (0x13417b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013417B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13417B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13417C0u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013417B8, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(719); const uint64_t a2_r = typed::gamecore_sub_013417B8(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013417B8_0x13417b8(rdram, ctx, runtime);
}
// gamecore_sub_01341CA8 (0x1341ca8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01341CA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341CA8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1341CB0u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01341CA8, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(720); const uint64_t a2_r = typed::gamecore_sub_01341CA8(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01341CA8_0x1341ca8(rdram, ctx, runtime);
}
// nico_UnparentObject (0x134c0f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_UnparentObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C0F8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F1FC0u, lift_d1::ForceUnparentLayoutObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11328F8u, lift_d1::SetGeometryPointerGlobalState) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x134C104u, 0x10F1FC0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nico_UnparentObject, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A7_IN(723); const uint64_t a2_r = typed::nico_UnparentObject(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_UnparentObject_0x134c0f8(rdram, ctx, runtime);
}
// gamecore_sub_01375160 (0x1375160): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01375160(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1375160u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1375168u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01375160, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(725); const uint64_t a2_r = typed::gamecore_sub_01375160(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01375160_0x1375160(rdram, ctx, runtime);
}
// SCRFuncBuddhaMissileMain (0x13795c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBuddhaMissileMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13795C0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC178u, ::decomp_scriptvm3::setScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13795F4u, 0x10F1A20u) && A5_JAL(0x13796F0u, 0x10CC178u) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncBuddhaMissileMain, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(727); const a1::RetVF a2_r = typed::SCRFuncBuddhaMissileMain(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncBuddhaMissileMain_0x13795c0(rdram, ctx, runtime);
}
// SCRFuncBuddhaMissileMain2 (0x1379770): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncBuddhaMissileMain2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1379770u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC178u, ::decomp_scriptvm3::setScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A5_JAL(0x1379848u, 0x10CC178u) && A5_JAL(0x13CFAF0u, 0x110CB28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A7_CHK(SCRFuncBuddhaMissileMain2, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(728); const a1::RetVF a2_r = typed::SCRFuncBuddhaMissileMain2(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncBuddhaMissileMain2_0x1379770(rdram, ctx, runtime);
}
// ps2__SCRRequestDamageReaction (0x137d520): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2__SCRRequestDamageReaction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137D520u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC178u, ::decomp_scriptvm3::setScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x137D558u, 0x10F1A20u) && A5_JAL(0x137D57Cu, 0x10C0A70u) && A5_JAL(0x137D60Cu, 0x10CC178u) && A5_SP16(), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A7_CHK(ps2__SCRRequestDamageReaction, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(730); const a1::RetVF a2_r = typed::ps2__SCRRequestDamageReaction(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::ps2__SCRRequestDamageReaction_0x137d520(rdram, ctx, runtime);
}
// gamecore_sub_013A47F0 (0x13a47f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A47F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A47F0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13A47FCu, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013A47F0, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(732); const uint64_t a2_r = typed::gamecore_sub_013A47F0(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013A47F0_0x13a47f0(rdram, ctx, runtime);
}
// gamecore_sub_013A48B8 (0x13a48b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A48B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A48B8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A7_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls) && A7_HOLDS(0x1353638u, ::a6ad::gStatGobjDispGflagSet) && A7_HOLDS(0x137DF08u, ::a7ad::setGameFlagForGameObj) && A5_JAL(0x137DF14u, 0x10F1A20u) && A5_JAL(0x137DF24u, 0x10F1A20u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013A48B8, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(733); const uint64_t a2_r = typed::gamecore_sub_013A48B8(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013A48B8_0x13a48b8(rdram, ctx, runtime);
}
// gamecore_sub_013A4EA0 (0x13a4ea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A4EA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A4EA0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13A4EA8u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013A4EA0, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(734); const uint64_t a2_r = typed::gamecore_sub_013A4EA0(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013A4EA0_0x13a4ea0(rdram, ctx, runtime);
}
// gamecore_sub_013A6FE0 (0x13a6fe0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A6FE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A6FE0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13A6FE8u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013A6FE0, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(736); const uint64_t a2_r = typed::gamecore_sub_013A6FE0(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013A6FE0_0x13a6fe0(rdram, ctx, runtime);
}
// SCRFuncNargaFloatMain (0x13a84b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncNargaFloatMain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A84B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110CB28u, ::decomp_scriptvm3::getScriptCharacterClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A7_HOLDS(0x13CFAE8u, ::decomp_scriptvm2::getSCRCharaAIWorkPtr) && A7_HOLDS(0x144CCA0u, lift_d1::setOffsetParamWormCtrlWork) && A7_HOLDS(0x1455F80u, ::decomp_scriptvm2::getSCRDataCtrlWork) && A5_JAL(0x13CFAF0u, 0x110CB28u) && A5_JAL(0x144CCD4u, 0x10F1A20u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A7_CHK(SCRFuncNargaFloatMain, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(738); const uint64_t a2_r = typed::SCRFuncNargaFloatMain(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncNargaFloatMain_0x13a84b0(rdram, ctx, runtime);
}
// gamecore_sub_013CE4D8 (0x13ce4d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013CE4D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CE4D8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13CE4E0u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013CE4D8, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(746); const uint64_t a2_r = typed::gamecore_sub_013CE4D8(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013CE4D8_0x13ce4d8(rdram, ctx, runtime);
}
// gamecore_sub_013FF3B0 (0x13ff3b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FF3B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF3B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13FF3B8u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_013FF3B0, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(752); const uint64_t a2_r = typed::gamecore_sub_013FF3B0(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013FF3B0_0x13ff3b0(rdram, ctx, runtime);
}
// gamecore_sub_01405300 (0x1405300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01405300(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1405300u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1405308u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_01405300, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(754); const uint64_t a2_r = typed::gamecore_sub_01405300(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_01405300_0x1405300(rdram, ctx, runtime);
}
// gamecore_sub_014454E8 (0x14454e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_014454E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14454E8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC178u, ::decomp_scriptvm3::setScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1445504u, 0x10CC178u) && A5_JAL(0x1445530u, 0x10CC178u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_014454E8, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(764); const uint64_t a2_r = typed::gamecore_sub_014454E8(rdram, runtime, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_014454E8_0x14454e8(rdram, ctx, runtime);
}
// SCRFuncSendKeyToGameObj (0x14508b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSendKeyToGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14508B0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC178u, ::decomp_scriptvm3::setScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1450984u, 0x10CC178u), 1)) { const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncSendKeyToGameObj, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(767); const uint64_t a2_r = typed::SCRFuncSendKeyToGameObj(rdram, runtime, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncSendKeyToGameObj_0x14508b0(rdram, ctx, runtime);
}
// SCRFuncOutsideSendKeyToGameObj (0x1450a08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncOutsideSendKeyToGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1450A08u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CC370u, lift_d1::CharaCtrlSetOutsideScriptCtrlStatus) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1450ADCu, 0x10CC370u), 1)) { const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(SCRFuncOutsideSendKeyToGameObj, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(768); const uint64_t a2_r = typed::SCRFuncOutsideSendKeyToGameObj(rdram, runtime, a2_9, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::SCRFuncOutsideSendKeyToGameObj_0x1450a08(rdram, ctx, runtime);
}
// gamecore_sub_014531A8 (0x14531a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_014531A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14531A8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1125B30u, lift_d1::GetCollisionDBAttributeAdr) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x14531B4u, 0x1125B30u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(gamecore_sub_014531A8, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A7_IN(770); const uint64_t a2_r = typed::gamecore_sub_014531A8(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_014531A8_0x14531a8(rdram, ctx, runtime);
}
// GAMEUTIL_is_parent_collision_attribute_boss (0x1343ac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_parent_collision_attribute_boss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1343AC8u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x107F150u, lift_d1::GAMEUTIL_get_parent_collision_attribute) && A7_HOLDS(0x10C1F90u, lift_d1::GetParentCharColAttr) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x1343AD4u, 0x107F150u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(GAMEUTIL_is_parent_collision_attribute_boss, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(779); const uint64_t a2_r = typed::GAMEUTIL_is_parent_collision_attribute_boss(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::GAMEUTIL_is_parent_collision_attribute_boss_0x1343ac8(rdram, ctx, runtime);
}
// NICO_is_quake_attack_hit_only_boss (0x137e8e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void NICO_is_quake_attack_hit_only_boss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E8E0u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x107F150u, lift_d1::GAMEUTIL_get_parent_collision_attribute) && A7_HOLDS(0x10C1F90u, lift_d1::GetParentCharColAttr) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x137E8F0u, 0x107F150u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(NICO_is_quake_attack_hit_only_boss, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A7_IN(782); const uint64_t a2_r = typed::NICO_is_quake_attack_hit_only_boss(rdram, runtime, a2_2, a2_4, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::NICO_is_quake_attack_hit_only_boss_0x137e8e0(rdram, ctx, runtime);
}
// nico_ChangePlayerHorseRidingParentWithVCol (0x13b7078): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_ChangePlayerHorseRidingParentWithVCol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B7078u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x10CCB38u, ::a7ad::SendCharaNodeParentSetMsg) && A7_HOLDS(0x10F1A20u, lift_d1::GetLayoutClass) && A7_HOLDS(0x10F20C0u, lift_d1::ForceParentLayoutObj) && A7_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A7_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A7_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A7_HOLDS(0x11327B0u, lift_d1::CopyGeometryPointer) && A7_HOLDS(0x11329C0u, lift_d1::GetGeometryPointerObject) && A7_HOLDS(0x1132C60u, lift_d1::SetGeometryPointerNodePosState) && A7_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A7_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A7_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A7_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x13B7084u, 0x10C0A70u) && A5_JAL(0x13B7098u, 0x11329C0u) && A5_JAL(0x13B70A8u, 0x10CCB38u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A7_CHK(nico_ChangePlayerHorseRidingParentWithVCol, a2_2, a2_4, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A7_IN(784); const uint64_t a2_r = typed::nico_ChangePlayerHorseRidingParentWithVCol(rdram, runtime, a2_2, a2_4, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a4_h5); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::nico_ChangePlayerHorseRidingParentWithVCol_0x13b7078(rdram, ctx, runtime);
}
// gamecore_sub_013F3D48 (0x13f3d48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013F3D48(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F3D48u, 1))
    {
        if (PS2_EXPECT(A7_GO() && A7_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A7_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A7_HOLDS(0x10B2790u, lift_d1::getBlendBaseMotion) && A7_HOLDS(0x10B3AF8u, lift_d1::GetMotionResourceOriginalFrames) && A7_HOLDS(0x10B63D8u, lift_d1::GetMotionResourceFrames) && A7_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A7_HOLDS(0x13D3DA0u, lift_d1::AI_UpdateVelocity) && A7_HOLDS(0x13D4958u, lift_d1::AI_GetTimeOfCurrMotion) && A7_HOLDS(0x13F35C8u, lift_d1::gamecore_sub_013F35C8) && A5_JAL(0x10B3BC0u, 0x1B3410u) && A5_JAL(0x13D498Cu, 0x10B63D8u) && A5_JAL(0x13F35E0u, 0x1CA1B0u) && A5_JAL(0x13F3734u, 0x10C0A70u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A7_CHK(gamecore_sub_013F3D48, a2_4, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[7] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[21]), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A7_IN(785); const a1::RetVF a2_r = typed::gamecore_sub_013F3D48(rdram, runtime, a2_4, a2_6, a2_7, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2, a4_h5, a3_xo); A7_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->f[21] = a1_flt((uint32_t)a3_xo[2]); Ps2SetGpr64(ctx, 6, a3_xo[3]); Ps2SetGpr64(ctx, 7, a3_xo[4]); Ps2SetGpr64(ctx, 8, a3_xo[5]); Ps2SetGpr64(ctx, 9, a3_xo[6]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a7
    }
    ::gamecore_sub_013F3D48_0x13f3d48(rdram, ctx, runtime);
}
}

namespace
{
    struct A7Ad
    {
        uint32_t addr;
        PS2Runtime::RecompiledFunction rec, ad;
    };
    const A7Ad kA7Ad[297] = {
        {0x102FE8u, &::LoaderSysPutString_0x102fe8, &a7ad::LoaderSysPutString},
        {0x1155D8u, &::sub_001155D8_0x1155d8, &a7ad::sub_001155D8},
        {0x118438u, &::sub_00118438_0x118438, &a7ad::sub_00118438},
        {0x118490u, &::sub_00118490_0x118490, &a7ad::sub_00118490},
        {0x1266F0u, &::sub_001266F0_0x1266f0, &a7ad::sub_001266F0},
        {0x1AC998u, &::_iosAssert_0x1ac998, &a7ad::_iosAssert},
        {0x1B6BF8u, &::kernel_sub_001B6BF8_0x1b6bf8, &a7ad::kernel_sub_001B6BF8},
        {0x1BBCA8u, &::SetQuaternionByAxisRotateWithNoRegularize_0x1bbca8, &a7ad::SetQuaternionByAxisRotateWithNoRegularize},
        {0x1C39E8u, &::gsRegBegin_0x1c39e8, &a7ad::gsRegBegin},
        {0x1CE1B8u, &::all_soundpause_0x1ce1b8, &a7ad::all_soundpause},
        {0x1CE240u, &::all_soundmute_0x1ce240, &a7ad::all_soundmute},
        {0x1D0DB0u, &::kernel_sub_001D0DB0_0x1d0db0, &a7ad::kernel_sub_001D0DB0},
        {0x1D18E0u, &::kernel_sub_001D18E0_0x1d18e0, &a7ad::kernel_sub_001D18E0},
        {0x1DAB10u, &::kernel_sub_001DAB10_0x1dab10, &a7ad::kernel_sub_001DAB10},
        {0x108EEC8u, &::glScalef_0x108eec8, &a7ad::glScalef},
        {0x10BA970u, &::GetAnimationClassCurrentInfoWithOutsideLimit_0x10ba970, &a7ad::GetAnimationClassCurrentInfoWithOutsideLimit},
        {0x10BAA80u, &::SetAnimationClassLastInfoDirectResource_0x10baa80, &a7ad::SetAnimationClassLastInfoDirectResource},
        {0x10EB918u, &::GetChildGroupManagerClass_0x10eb918, &a7ad::GetChildGroupManagerClass},
        {0x1129A38u, &::GetCollisionDynamicsProjectionFloatPosition_0x1129a38, &a7ad::GetCollisionDynamicsProjectionFloatPosition},
        {0x115A7E8u, &::nicosysdSendDataWrite_0x115a7e8, &a7ad::nicosysdSendDataWrite},
        {0x1169BF0u, &::getEditWorkSize_0x1169bf0, &a7ad::getEditWorkSize},
        {0x1341180u, &::DebugDispTargetStatusForce_0x1341180, &a7ad::DebugDispTargetStatusForce},
        {0x13434B0u, &::GAMEUTIL_is_player_obj_standing_on_horse_0x13434b0, &a7ad::GAMEUTIL_is_player_obj_standing_on_horse},
        {0x134E418u, &::nico_setFruitsFlag_0x134e418, &a7ad::nico_setFruitsFlag},
        {0x134E4D8u, &::nico_checkTreeAllFruitsAlreadyTaken_0x134e4d8, &a7ad::nico_checkTreeAllFruitsAlreadyTaken},
        {0x1353410u, &::gStatOmakeSave_0x1353410, &a7ad::gStatOmakeSave},
        {0x1353BF8u, &::gStatLoadAdvertiseHorseChara_0x1353bf8, &a7ad::gStatLoadAdvertiseHorseChara},
        {0x1353C80u, &::gStatUnloadAdvertiseHorseChara_0x1353c80, &a7ad::gStatUnloadAdvertiseHorseChara},
        {0x1367E40u, &::time_attack_set_best_time_0x1367e40, &a7ad::time_attack_set_best_time},
        {0x137C728u, &::SCRCharaTurn_0x137c728, &a7ad::SCRCharaTurn},
        {0x137C7C0u, &::SCRPlayableCharaTurnDirectInput_0x137c7c0, &a7ad::SCRPlayableCharaTurnDirectInput},
        {0x137C7F8u, &::SCRCharaAccel_0x137c7f8, &a7ad::SCRCharaAccel},
        {0x137C8B8u, &::SCRCharaStand_0x137c8b8, &a7ad::SCRCharaStand},
        {0x137C950u, &::SCRGetDir_0x137c950, &a7ad::SCRGetDir},
        {0x138C1C8u, &::gamecore_sub_0138C1C8_0x138c1c8, &a7ad::gamecore_sub_0138C1C8},
        {0x138C1F0u, &::gamecore_sub_0138C1F0_0x138c1f0, &a7ad::gamecore_sub_0138C1F0},
        {0x138CB48u, &::gamecore_sub_0138CB48_0x138cb48, &a7ad::gamecore_sub_0138CB48},
        {0x13B51B0u, &::player_IsHorseHang_0x13b51b0, &a7ad::player_IsHorseHang},
        {0x13C8748u, &::gamecore_sub_013C8748_0x13c8748, &a7ad::gamecore_sub_013C8748},
        {0x13ECA60u, &::GexCutTail_0x13eca60, &a7ad::GexCutTail},
        {0x1434440u, &::gamecore_sub_01434440_0x1434440, &a7ad::gamecore_sub_01434440},
        {0x1445BB0u, &::calcAttainPosForDirExec_0x1445bb0, &a7ad::calcAttainPosForDirExec},
        {0x1457868u, &::SCRConditionMyselfKeepHit_0x1457868, &a7ad::SCRConditionMyselfKeepHit},
        {0x145D668u, &::getAimIKWork_0x145d668, &a7ad::getAimIKWork},
        {0x107B8D0u, &::DEBUG_get_cur_using_anb_file_name_0x107b8d0, &a7ad::DEBUG_get_cur_using_anb_file_name},
        {0x107D1B8u, &::GAMEUTIL_get_limit_clip_yaw_rel_0x107d1b8, &a7ad::GAMEUTIL_get_limit_clip_yaw_rel},
        {0x10FC250u, &::GetLwsOrientInitMotionPtr_0x10fc250, &a7ad::GetLwsOrientInitMotionPtr},
        {0x1112530u, &::SpriteMenuGetPartsDefValueByHashStr_0x1112530, &a7ad::SpriteMenuGetPartsDefValueByHashStr},
        {0x1112A78u, &::SpriteMenuSetSpriteMenuSoundSwitchByHashStr_0x1112a78, &a7ad::SpriteMenuSetSpriteMenuSoundSwitchByHashStr},
        {0x114DDC0u, &::manager_sub_0114DDC0_0x114ddc0, &a7ad::manager_sub_0114DDC0},
        {0x115A018u, &::dsysWrite_0x115a018, &a7ad::dsysWrite},
        {0x11648D0u, &::manager_sub_011648D0_0x11648d0, &a7ad::manager_sub_011648D0},
        {0x1343290u, &::GAMEUTIL_is_swimming_now_0x1343290, &a7ad::GAMEUTIL_is_swimming_now},
        {0x1347068u, &::set_max_min_yaw_parameter_nico_yaw_pitch_cntrl_0x1347068, &a7ad::set_max_min_yaw_parameter_nico_yaw_pitch_cntrl},
        {0x1347110u, &::set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl_0x1347110, &a7ad::set_max_min_yaw_parameter_extend_nico_yaw_pitch_cntrl},
        {0x13477C0u, &::get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl_0x13477c0, &a7ad::get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl},
        {0x134D8D0u, &::endingflowSetTimePlayerHpMax_0x134d8d0, &a7ad::endingflowSetTimePlayerHpMax},
        {0x136F0C0u, &::SCRConditionBirdPassingEachOther_0x136f0c0, &a7ad::SCRConditionBirdPassingEachOther},
        {0x136F0F8u, &::SCRConditionBirdBeginningApproaching_0x136f0f8, &a7ad::SCRConditionBirdBeginningApproaching},
        {0x1378B68u, &::SCRGetBossPartsDamageNum_0x1378b68, &a7ad::SCRGetBossPartsDamageNum},
        {0x1378D90u, &::SCRGetBossWeakPointAngleFromIndex_0x1378d90, &a7ad::SCRGetBossWeakPointAngleFromIndex},
        {0x1378DD8u, &::getBossPartsCurDamage_0x1378dd8, &a7ad::getBossPartsCurDamage},
        {0x13790F8u, &::SCRGetBossPartsDamageFromWeak_0x13790f8, &a7ad::SCRGetBossPartsDamageFromWeak},
        {0x1379188u, &::SCRGetBackStageBossPartsDamageFromWeak_0x1379188, &a7ad::SCRGetBackStageBossPartsDamageFromWeak},
        {0x1379738u, &::SCRFuncBuddhaMissileInit2_0x1379738, &a7ad::SCRFuncBuddhaMissileInit2},
        {0x137E928u, &::NICO_is_hit_except_underwater_0x137e928, &a7ad::NICO_is_hit_except_underwater},
        {0x137E978u, &::NICO_is_hit_except_underwater_or_boss_0x137e978, &a7ad::NICO_is_hit_except_underwater_or_boss},
        {0x138F730u, &::gamecore_sub_0138F730_0x138f730, &a7ad::gamecore_sub_0138F730},
        {0x1390218u, &::gamecore_sub_01390218_0x1390218, &a7ad::gamecore_sub_01390218},
        {0x1390380u, &::gamecore_sub_01390380_0x1390380, &a7ad::gamecore_sub_01390380},
        {0x1396300u, &::SCRFuncHorseAdvertisFlag_0x1396300, &a7ad::SCRFuncHorseAdvertisFlag},
        {0x1396398u, &::SCRFuncHorseEscapeCounterInit_0x1396398, &a7ad::SCRFuncHorseEscapeCounterInit},
        {0x13963C0u, &::SCRFuncHorseEscapeCounterMain_0x13963c0, &a7ad::SCRFuncHorseEscapeCounterMain},
        {0x1396400u, &::SCRConditionHorseEscapeCounterChack_0x1396400, &a7ad::SCRConditionHorseEscapeCounterChack},
        {0x1396920u, &::setHorseForceScriptKey_0x1396920, &a7ad::setHorseForceScriptKey},
        {0x139CD38u, &::SCRConditionFallRockChack_0x139cd38, &a7ad::SCRConditionFallRockChack},
        {0x139CD70u, &::SCRFuncKirinFallRockTimerSetFlagOn_0x139cd70, &a7ad::SCRFuncKirinFallRockTimerSetFlagOn},
        {0x13A5AC8u, &::SCRConditionMinoCTargetWallChangeChack_0x13a5ac8, &a7ad::SCRConditionMinoCTargetWallChangeChack},
        {0x13A5AE8u, &::SCRFuncMinoCTargetWallReset_0x13a5ae8, &a7ad::SCRFuncMinoCTargetWallReset},
        {0x13A5B08u, &::SCRFuncMinoCAtkCountInit_0x13a5b08, &a7ad::SCRFuncMinoCAtkCountInit},
        {0x13A5B28u, &::SCRFuncMinoCAddAtkCount_0x13a5b28, &a7ad::SCRFuncMinoCAddAtkCount},
        {0x13A5B50u, &::SCRConditionMinoCAtkCountChack_0x13a5b50, &a7ad::SCRConditionMinoCAtkCountChack},
        {0x13A5BE0u, &::SCRConditionMinoCWallCaseFlagChack_0x13a5be0, &a7ad::SCRConditionMinoCWallCaseFlagChack},
        {0x13A5C00u, &::SCRFuncMinoCSetPlayerInCorridorFlag_0x13a5c00, &a7ad::SCRFuncMinoCSetPlayerInCorridorFlag},
        {0x13A5C28u, &::SCRFuncMinoCResetPlayerInCorridorFlag_0x13a5c28, &a7ad::SCRFuncMinoCResetPlayerInCorridorFlag},
        {0x13A5C48u, &::SCRFuncMinoCPlayerInCorridorFlagChack_0x13a5c48, &a7ad::SCRFuncMinoCPlayerInCorridorFlagChack},
        {0x13A8538u, &::SCRFuncNargaSetPlayerDamage_0x13a8538, &a7ad::SCRFuncNargaSetPlayerDamage},
        {0x13A8668u, &::SCRConditionNargaWallCheck_0x13a8668, &a7ad::SCRConditionNargaWallCheck},
        {0x13A8698u, &::SCRConditionNargaPlayerDamage_0x13a8698, &a7ad::SCRConditionNargaPlayerDamage},
        {0x13A8788u, &::SCRConditionNargaWallNear_0x13a8788, &a7ad::SCRConditionNargaWallNear},
        {0x13AA410u, &::get_merge_yaw_nico_sight_on_0x13aa410, &a7ad::get_merge_yaw_nico_sight_on},
        {0x13B4E20u, &::nico_playerIsSwimmingNow_0x13b4e20, &a7ad::nico_playerIsSwimmingNow},
        {0x13B5078u, &::getCurWeapon_0x13b5078, &a7ad::getCurWeapon},
        {0x13B5098u, &::getPlayerMVHIST_0x13b5098, &a7ad::getPlayerMVHIST},
        {0x13B5158u, &::IsHorseCalled_withFlgOff_0x13b5158, &a7ad::IsHorseCalled_withFlgOff},
        {0x13B5190u, &::IsHorseAutoControlled_0x13b5190, &a7ad::IsHorseAutoControlled},
        {0x13B5340u, &::player_getWeaponType_0x13b5340, &a7ad::player_getWeaponType},
        {0x13B5370u, &::getPlayerColCont_0x13b5370, &a7ad::getPlayerColCont},
        {0x13B5678u, &::player_SetGripDamage_0x13b5678, &a7ad::player_SetGripDamage},
        {0x13B5A88u, &::player_GetBoundDamageLevel_0x13b5a88, &a7ad::player_GetBoundDamageLevel},
        {0x13B5BE0u, &::isWeaponTargetMarker_0x13b5be0, &a7ad::isWeaponTargetMarker},
        {0x13B6538u, &::torchAttached_0x13b6538, &a7ad::torchAttached},
        {0x13B6580u, &::sendArrowPos_0x13b6580, &a7ad::sendArrowPos},
        {0x13B65B0u, &::sendArrowInfo_0x13b65b0, &a7ad::sendArrowInfo},
        {0x13B6A60u, &::nico_playeGripholdOn_0x13b6a60, &a7ad::nico_playeGripholdOn},
        {0x13B6A88u, &::nico_playeGripholdOff_0x13b6a88, &a7ad::nico_playeGripholdOff},
        {0x13B73B8u, &::gamecore_sub_013B73B8_0x13b73b8, &a7ad::gamecore_sub_013B73B8},
        {0x13B7678u, &::gamecore_sub_013B7678_0x13b7678, &a7ad::gamecore_sub_013B7678},
        {0x13B76A0u, &::gamecore_sub_013B76A0_0x13b76a0, &a7ad::gamecore_sub_013B76A0},
        {0x13CE680u, &::SCRFuncPoseidonResetRevolutionDirection_0x13ce680, &a7ad::SCRFuncPoseidonResetRevolutionDirection},
        {0x13CE6A0u, &::SCRConditionPoseidonGetRevolutionRayChack_0x13ce6a0, &a7ad::SCRConditionPoseidonGetRevolutionRayChack},
        {0x13CE6C0u, &::SCRFuncPoseidonMissAttackInit_0x13ce6c0, &a7ad::SCRFuncPoseidonMissAttackInit},
        {0x13CE6E8u, &::SCRFuncPoseidonMissAttackAdd_0x13ce6e8, &a7ad::SCRFuncPoseidonMissAttackAdd},
        {0x13CE710u, &::SCRConditionPoseidonMissAttackCheck_0x13ce710, &a7ad::SCRConditionPoseidonMissAttackCheck},
        {0x13CE738u, &::SCRFuncPoseidonSetPlayerTerrLog_0x13ce738, &a7ad::SCRFuncPoseidonSetPlayerTerrLog},
        {0x13CE770u, &::SCRFuncPoseidonResetPlayerTerrLog_0x13ce770, &a7ad::SCRFuncPoseidonResetPlayerTerrLog},
        {0x13CE790u, &::SCRConditionPoseidonGetPlayerTerrLog_0x13ce790, &a7ad::SCRConditionPoseidonGetPlayerTerrLog},
        {0x13CE7F0u, &::SCRFuncPoseidonAngryCountAdd_0x13ce7f0, &a7ad::SCRFuncPoseidonAngryCountAdd},
        {0x13CE818u, &::SCRFuncPoseidonAngryCountInit_0x13ce818, &a7ad::SCRFuncPoseidonAngryCountInit},
        {0x13CE838u, &::SCRConditionPoseidonAngryCountChack_0x13ce838, &a7ad::SCRConditionPoseidonAngryCountChack},
        {0x13CE868u, &::SCRConditionPoseidonDamageCoreTime_0x13ce868, &a7ad::SCRConditionPoseidonDamageCoreTime},
        {0x13CE8A0u, &::SCRConditionPoseidonDamageFlagSet_0x13ce8a0, &a7ad::SCRConditionPoseidonDamageFlagSet},
        {0x13CE8C8u, &::SCRConditionPoseidonDamageFlagReset_0x13ce8c8, &a7ad::SCRConditionPoseidonDamageFlagReset},
        {0x13CE8F0u, &::SCRConditionPoseidonDamageCoreTimeMain_0x13ce8f0, &a7ad::SCRConditionPoseidonDamageCoreTimeMain},
        {0x13CEA30u, &::SCRFuncPoseidonAngryMain_0x13cea30, &a7ad::SCRFuncPoseidonAngryMain},
        {0x13D15B0u, &::gamecore_sub_013D15B0_0x13d15b0, &a7ad::gamecore_sub_013D15B0},
        {0x13F5D98u, &::SCRFuncSnakeCUpGroundInit_0x13f5d98, &a7ad::SCRFuncSnakeCUpGroundInit},
        {0x13FCA10u, &::gamecore_sub_013FCA10_0x13fca10, &a7ad::gamecore_sub_013FCA10},
        {0x13FF580u, &::gamecore_sub_013FF580_0x13ff580, &a7ad::gamecore_sub_013FF580},
        {0x13FF9A8u, &::gamecore_sub_013FF9A8_0x13ff9a8, &a7ad::gamecore_sub_013FF9A8},
        {0x1400A20u, &::SelectOmakeItem_0x1400a20, &a7ad::SelectOmakeItem},
        {0x1402458u, &::gamecore_sub_01402458_0x1402458, &a7ad::gamecore_sub_01402458},
        {0x1402560u, &::gamecore_sub_01402560_0x1402560, &a7ad::gamecore_sub_01402560},
        {0x1402640u, &::gamecore_sub_01402640_0x1402640, &a7ad::gamecore_sub_01402640},
        {0x1402730u, &::gamecore_sub_01402730_0x1402730, &a7ad::gamecore_sub_01402730},
        {0x14028B8u, &::CheckSelectDisp_0x14028b8, &a7ad::CheckSelectDisp},
        {0x1402B08u, &::gamecore_sub_01402B08_0x1402b08, &a7ad::gamecore_sub_01402B08},
        {0x1402C10u, &::gamecore_sub_01402C10_0x1402c10, &a7ad::gamecore_sub_01402C10},
        {0x1402CF0u, &::gamecore_sub_01402CF0_0x1402cf0, &a7ad::gamecore_sub_01402CF0},
        {0x1402DE0u, &::gamecore_sub_01402DE0_0x1402de0, &a7ad::gamecore_sub_01402DE0},
        {0x1404AE8u, &::SCRFuncKameFallPatienceCunterMain_0x1404ae8, &a7ad::SCRFuncKameFallPatienceCunterMain},
        {0x1404D30u, &::SCRConditionKameCrackAnimChack_0x1404d30, &a7ad::SCRConditionKameCrackAnimChack},
        {0x1405038u, &::SCRConditionKameWallCaseFlagChack_0x1405038, &a7ad::SCRConditionKameWallCaseFlagChack},
        {0x1405058u, &::SCRConditionKameMoveTimeChack_0x1405058, &a7ad::SCRConditionKameMoveTimeChack},
        {0x1405090u, &::SCRFuncKameMoveTimeSet_0x1405090, &a7ad::SCRFuncKameMoveTimeSet},
        {0x14050E8u, &::SCRFuncKameMoveTimeMain_0x14050e8, &a7ad::SCRFuncKameMoveTimeMain},
        {0x1406760u, &::SCRFuncUnagiSetPlayerGazePos_0x1406760, &a7ad::SCRFuncUnagiSetPlayerGazePos},
        {0x1406A70u, &::SCRFuncUnageIncTblIdx_0x1406a70, &a7ad::SCRFuncUnageIncTblIdx},
        {0x1406AA0u, &::SCRFuncUnageInitBossNotParentTime_0x1406aa0, &a7ad::SCRFuncUnageInitBossNotParentTime},
        {0x1406B38u, &::SCRConditionUnageBossNotParentTime_0x1406b38, &a7ad::SCRConditionUnageBossNotParentTime},
        {0x140C880u, &::SCRFuncYamoriBDamageCheckInit_0x140c880, &a7ad::SCRFuncYamoriBDamageCheckInit},
        {0x140CA40u, &::SCRConditionYamoriBDamageCheckFrontR_0x140ca40, &a7ad::SCRConditionYamoriBDamageCheckFrontR},
        {0x140CA78u, &::SCRConditionYamoriBDamageCheckBackR_0x140ca78, &a7ad::SCRConditionYamoriBDamageCheckBackR},
        {0x140CAB0u, &::SCRConditionYamoriBDamageCheckFrontL_0x140cab0, &a7ad::SCRConditionYamoriBDamageCheckFrontL},
        {0x140CAE8u, &::SCRConditionYamoriBDamageCheckBackL_0x140cae8, &a7ad::SCRConditionYamoriBDamageCheckBackL},
        {0x140CD28u, &::SCRFuncYamoriBSetCalcPosOffset_0x140cd28, &a7ad::SCRFuncYamoriBSetCalcPosOffset},
        {0x140D0B0u, &::SCRFuncYamoriBSetBreathVolume_0x140d0b0, &a7ad::SCRFuncYamoriBSetBreathVolume},
        {0x140D150u, &::SCRFuncYamoriBSetRandomRadius_0x140d150, &a7ad::SCRFuncYamoriBSetRandomRadius},
        {0x140D198u, &::SCRFuncYamoriBSetRandomRadius2_0x140d198, &a7ad::SCRFuncYamoriBSetRandomRadius2},
        {0x140D1E0u, &::SCRFuncYamoriBUnsetRandom_0x140d1e0, &a7ad::SCRFuncYamoriBUnsetRandom},
        {0x140D208u, &::SCRFuncYamoriBSetAttackPosAndGazePosInit_0x140d208, &a7ad::SCRFuncYamoriBSetAttackPosAndGazePosInit},
        {0x140D4B8u, &::SCRFuncYamoriBSetBellyDamageParam_0x140d4b8, &a7ad::SCRFuncYamoriBSetBellyDamageParam},
        {0x140D4E0u, &::SCRFuncYamoriBSetBellyDamageInitWork_0x140d4e0, &a7ad::SCRFuncYamoriBSetBellyDamageInitWork},
        {0x140D510u, &::SCRConditionYamoriBLessBellyDamageProgress_0x140d510, &a7ad::SCRConditionYamoriBLessBellyDamageProgress},
        {0x140D548u, &::SCRConditionYamoriBBellyDamageCheck_0x140d548, &a7ad::SCRConditionYamoriBBellyDamageCheck},
        {0x140D6F0u, &::SCRFuncYamoriBAimInit_0x140d6f0, &a7ad::SCRFuncYamoriBAimInit},
        {0x140E468u, &::demoManChkLoadFinish_0x140e468, &a7ad::demoManChkLoadFinish},
        {0x140E4E8u, &::IsForwardTime__DemoMan_0x140e4e8, &a7ad::IsForwardTime__DemoMan},
        {0x140E540u, &::ForwardTime__DemoMan_0x140e540, &a7ad::ForwardTime__DemoMan},
        {0x141A698u, &::CAM_get_motion_kind_follow_rot_axis_attr_target_0x141a698, &a7ad::CAM_get_motion_kind_follow_rot_axis_attr_target},
        {0x1425728u, &::is_lws_fixed_point_mode_0x1425728, &a7ad::is_lws_fixed_point_mode},
        {0x1425758u, &::is_lws_movie_mode_game_camera_0x1425758, &a7ad::is_lws_movie_mode_game_camera},
        {0x1435EA0u, &::gamecore_sub_01435EA0_0x1435ea0, &a7ad::gamecore_sub_01435EA0},
        {0x1435F00u, &::gamecore_sub_01435F00_0x1435f00, &a7ad::gamecore_sub_01435F00},
        {0x1459638u, &::SCRConditionSegmentLoad_0x1459638, &a7ad::SCRConditionSegmentLoad},
        {0x1464E08u, &::SCRFuncSoundPlay_0x1464e08, &a7ad::SCRFuncSoundPlay},
        {0x107CA20u, &::DEBUG_get_parent_layout_name_0x107ca20, &a7ad::DEBUG_get_parent_layout_name},
        {0x10C1A20u, &::GetCharacterClassCurAnimationTotalFrame_0x10c1a20, &a7ad::GetCharacterClassCurAnimationTotalFrame},
        {0x10CC408u, &::CharaCtrlSetScriptCtrlStatusCallBack_0x10cc408, &a7ad::CharaCtrlSetScriptCtrlStatusCallBack},
        {0x10D32F0u, &::checkShift_0x10d32f0, &a7ad::checkShift},
        {0x10DF590u, &::manager_sub_010DF590_0x10df590, &a7ad::manager_sub_010DF590},
        {0x1102AC8u, &::SetScriptObjFinishCallBackFunc_0x1102ac8, &a7ad::SetScriptObjFinishCallBackFunc},
        {0x1123380u, &::manager_sub_01123380_0x1123380, &a7ad::manager_sub_01123380},
        {0x1123520u, &::CollisionPlaneJudgeCallbackParticle_0x1123520, &a7ad::CollisionPlaneJudgeCallbackParticle},
        {0x1128300u, &::MakeGeometryPointerCollisionInfo_0x1128300, &a7ad::MakeGeometryPointerCollisionInfo},
        {0x1128828u, &::CollisionObjectJudgeCallbackExceptDescendOneObj_0x1128828, &a7ad::CollisionObjectJudgeCallbackExceptDescendOneObj},
        {0x1129410u, &::GetCollisionDBNextSurfLinkComp_0x1129410, &a7ad::GetCollisionDBNextSurfLinkComp},
        {0x1129488u, &::GetCollisionDBNextSurfLink_0x1129488, &a7ad::GetCollisionDBNextSurfLink},
        {0x1129550u, &::MakeCollisionPointCollisionInfo_0x1129550, &a7ad::MakeCollisionPointCollisionInfo},
        {0x1160F18u, &::pop_camera_context_0x1160f18, &a7ad::pop_camera_context},
        {0x1163538u, &::doit_camera_region_0x1163538, &a7ad::doit_camera_region},
        {0x1169648u, &::getBootParent_0x1169648, &a7ad::getBootParent},
        {0x1341570u, &::DebugIsPlayer_0x1341570, &a7ad::DebugIsPlayer},
        {0x1376128u, &::getSkeletonGlobalGeo_0x1376128, &a7ad::getSkeletonGlobalGeo},
        {0x137B748u, &::SCRFuncCerberusSetPlayerParentObj_0x137b748, &a7ad::SCRFuncCerberusSetPlayerParentObj},
        {0x137DF08u, &::setGameFlagForGameObj_0x137df08, &a7ad::setGameFlagForGameObj},
        {0x137E020u, &::chkGameFlagForGameObj_0x137e020, &a7ad::chkGameFlagForGameObj},
        {0x137E4D0u, &::SCRSetMyOwnGameFlag_0x137e4d0, &a7ad::SCRSetMyOwnGameFlag},
        {0x137E558u, &::SCRResetMyOwnGameFlag_0x137e558, &a7ad::SCRResetMyOwnGameFlag},
        {0x13961D8u, &::SCRConditionHorseCalled_0x13961d8, &a7ad::SCRConditionHorseCalled},
        {0x139A808u, &::IsDamaged_0x139a808, &a7ad::IsDamaged},
        {0x139AC68u, &::IsColStatus_0x139ac68, &a7ad::IsColStatus},
        {0x139ACE8u, &::IsColStatusCUP_0x139ace8, &a7ad::IsColStatusCUP},
        {0x139CBD0u, &::SCRFuncKirinCameraNeckDownIdReset_0x139cbd0, &a7ad::SCRFuncKirinCameraNeckDownIdReset},
        {0x13AD738u, &::gamecore_sub_013AD738_0x13ad738, &a7ad::gamecore_sub_013AD738},
        {0x13B5208u, &::player_IsSwiming_0x13b5208, &a7ad::player_IsSwiming},
        {0x13B5220u, &::getPlayerPosAfterXSeconds_0x13b5220, &a7ad::getPlayerPosAfterXSeconds},
        {0x13B6040u, &::SCRPlayerRotateStop_0x13b6040, &a7ad::SCRPlayerRotateStop},
        {0x13D5FC0u, &::gamecore_sub_013D5FC0_0x13d5fc0, &a7ad::gamecore_sub_013D5FC0},
        {0x13D84F0u, &::gamecore_sub_013D84F0_0x13d84f0, &a7ad::gamecore_sub_013D84F0},
        {0x13D9138u, &::gamecore_sub_013D9138_0x13d9138, &a7ad::gamecore_sub_013D9138},
        {0x13DA430u, &::gamecore_sub_013DA430_0x13da430, &a7ad::gamecore_sub_013DA430},
        {0x13E7BD8u, &::Kite_InitKiteAdvertisePathWork_0x13e7bd8, &a7ad::Kite_InitKiteAdvertisePathWork},
        {0x13E8588u, &::gamecore_sub_013E8588_0x13e8588, &a7ad::gamecore_sub_013E8588},
        {0x13E86F0u, &::gamecore_sub_013E86F0_0x13e86f0, &a7ad::gamecore_sub_013E86F0},
        {0x1406AC0u, &::SCRFuncUnageMainBossNotParentTime_0x1406ac0, &a7ad::SCRFuncUnageMainBossNotParentTime},
        {0x140D0D8u, &::SCRFuncYamoriBGounrdAttackMain_0x140d0d8, &a7ad::SCRFuncYamoriBGounrdAttackMain},
        {0x140D640u, &::SCRFuncYamoriBSetAttainPosForGroundAttack_0x140d640, &a7ad::SCRFuncYamoriBSetAttainPosForGroundAttack},
        {0x141A760u, &::CAM_set_target_display_flag_0x141a760, &a7ad::CAM_set_target_display_flag},
        {0x14325B0u, &::setAttainmentObjAndDir_0x14325b0, &a7ad::setAttainmentObjAndDir},
        {0x1432670u, &::setAttainmentObjAndOffset_0x1432670, &a7ad::setAttainmentObjAndOffset},
        {0x144D1E8u, &::SCRConditionLessAngryValue_0x144d1e8, &a7ad::SCRConditionLessAngryValue},
        {0x144FB40u, &::SCRFuncSetAttainLostPlayerPos_0x144fb40, &a7ad::SCRFuncSetAttainLostPlayerPos},
        {0x144FB88u, &::SCRFuncSetAttainLostPlayerPosGlobalY_0x144fb88, &a7ad::SCRFuncSetAttainLostPlayerPosGlobalY},
        {0x144FBC8u, &::SCRFuncSetAttainMyselfPosAndGlobalY_0x144fbc8, &a7ad::SCRFuncSetAttainMyselfPosAndGlobalY},
        {0x144FC28u, &::SCRFuncSetAttainPlayerPosAndGlobalY_0x144fc28, &a7ad::SCRFuncSetAttainPlayerPosAndGlobalY},
        {0x144FD28u, &::SCRFuncSetAttainMyselfFrontPos_0x144fd28, &a7ad::SCRFuncSetAttainMyselfFrontPos},
        {0x14506E8u, &::SCRFuncResetVolumeTime0_0x14506e8, &a7ad::SCRFuncResetVolumeTime0},
        {0x1450BD0u, &::SCRFuncSetGpCalcDist_0x1450bd0, &a7ad::SCRFuncSetGpCalcDist},
        {0x1452248u, &::SCRFuncSetOrientCheckWork_0x1452248, &a7ad::SCRFuncSetOrientCheckWork},
        {0x1452358u, &::SCRFuncForceSetPerceptionWork_0x1452358, &a7ad::SCRFuncForceSetPerceptionWork},
        {0x1452388u, &::SCRFuncForceRenewLastPos_0x1452388, &a7ad::SCRFuncForceRenewLastPos},
        {0x14523B8u, &::SCRFuncResetWormCtrlParam_0x14523b8, &a7ad::SCRFuncResetWormCtrlParam},
        {0x1452400u, &::SCRFuncSetRotColCheckWormCtrlParam_0x1452400, &a7ad::SCRFuncSetRotColCheckWormCtrlParam},
        {0x1452448u, &::SCRFuncSetMiddleTargetWormCtrlParam_0x1452448, &a7ad::SCRFuncSetMiddleTargetWormCtrlParam},
        {0x1452490u, &::SCRFuncSetAcceleParamWormCtrlParam_0x1452490, &a7ad::SCRFuncSetAcceleParamWormCtrlParam},
        {0x14525C0u, &::SCRFuncSetSearchRadius_0x14525c0, &a7ad::SCRFuncSetSearchRadius},
        {0x14525F0u, &::SCRFuncResetSearchRadius_0x14525f0, &a7ad::SCRFuncResetSearchRadius},
        {0x14526F0u, &::SCRFuncSetObstacleWormCtrl_0x14526f0, &a7ad::SCRFuncSetObstacleWormCtrl},
        {0x1452DB8u, &::SCRFuncSetPerceptionParam_0x1452db8, &a7ad::SCRFuncSetPerceptionParam},
        {0x1452E88u, &::SCRFuncSetGripDamage_0x1452e88, &a7ad::SCRFuncSetGripDamage},
        {0x1452EC0u, &::SCRFuncSetPassageRangeData_0x1452ec0, &a7ad::SCRFuncSetPassageRangeData},
        {0x1453138u, &::SCRFuncSetCreateTempAttain_0x1453138, &a7ad::SCRFuncSetCreateTempAttain},
        {0x14563F0u, &::SCRConditionDamagedType_0x14563f0, &a7ad::SCRConditionDamagedType},
        {0x1456440u, &::SCRConditionDamagedWeaponType_0x1456440, &a7ad::SCRConditionDamagedWeaponType},
        {0x1456D48u, &::SCRConditionPlayerParentRangeColAttrAndEquipmentCheck_0x1456d48, &a7ad::SCRConditionPlayerParentRangeColAttrAndEquipmentCheck},
        {0x14577A0u, &::SCRConditionMyselfOverGlobalY_0x14577a0, &a7ad::SCRConditionMyselfOverGlobalY},
        {0x14577E0u, &::SCRConditionMyselfLessGlobalY_0x14577e0, &a7ad::SCRConditionMyselfLessGlobalY},
        {0x1457820u, &::SCRConditionPlayerLessGlobalY_0x1457820, &a7ad::SCRConditionPlayerLessGlobalY},
        {0x14578F8u, &::SCRConditionPPFinish_0x14578f8, &a7ad::SCRConditionPPFinish},
        {0x1457920u, &::SCRConditionPPPassageNum_0x1457920, &a7ad::SCRConditionPPPassageNum},
        {0x1457AE8u, &::SCRConditionPathPassageNum_0x1457ae8, &a7ad::SCRConditionPathPassageNum},
        {0x1457B20u, &::SCRConditionPathPassageNumOver_0x1457b20, &a7ad::SCRConditionPathPassageNumOver},
        {0x14580A0u, &::SCRConditionVolumeTime0_0x14580a0, &a7ad::SCRConditionVolumeTime0},
        {0x1458D40u, &::SCRConditionOverMyselfPartsLife_0x1458d40, &a7ad::SCRConditionOverMyselfPartsLife},
        {0x1458DD0u, &::SCRConditionOverMyselfWeakLife_0x1458dd0, &a7ad::SCRConditionOverMyselfWeakLife},
        {0x1458E60u, &::SCRConditionMyselfPartsDamaged_0x1458e60, &a7ad::SCRConditionMyselfPartsDamaged},
        {0x1458EC0u, &::SCRConditionExecGazeToPlayer_0x1458ec0, &a7ad::SCRConditionExecGazeToPlayer},
        {0x1458F98u, &::SCRConditionLessPlayerLife_0x1458f98, &a7ad::SCRConditionLessPlayerLife},
        {0x14590E0u, &::SCRConditionOverRandomVal_0x14590e0, &a7ad::SCRConditionOverRandomVal},
        {0x1459118u, &::SCRConditionLessRandomVal_0x1459118, &a7ad::SCRConditionLessRandomVal},
        {0x1459230u, &::SCRConditionOverLengthXZPlayerFromGp_0x1459230, &a7ad::SCRConditionOverLengthXZPlayerFromGp},
        {0x14592C8u, &::SCRConditionLessLengthXZPlayerFromGp_0x14592c8, &a7ad::SCRConditionLessLengthXZPlayerFromGp},
        {0x1459670u, &::SCRConditionEyeModeLapsedTime_0x1459670, &a7ad::SCRConditionEyeModeLapsedTime},
        {0x1459858u, &::SCRConditionOldModeStatusByMySelf_0x1459858, &a7ad::SCRConditionOldModeStatusByMySelf},
        {0x14598E0u, &::SCRConditionSpecifiedModeByMySelf_0x14598e0, &a7ad::SCRConditionSpecifiedModeByMySelf},
        {0x1459C38u, &::SCRConditionCheckBuryingWormCtrl_0x1459c38, &a7ad::SCRConditionCheckBuryingWormCtrl},
        {0x145BBE8u, &::SCRConditionHintOverPlayCount_0x145bbe8, &a7ad::SCRConditionHintOverPlayCount},
        {0x1464C88u, &::SCRFuncSoundResetPlayerAttrInfo_0x1464c88, &a7ad::SCRFuncSoundResetPlayerAttrInfo},
        {0x1465020u, &::SCREventConditionOverDistanceToPlayerWorldXZ_0x1465020, &a7ad::SCREventConditionOverDistanceToPlayerWorldXZ},
        {0x10C2010u, &::DebugGetParentCharColAttr_0x10c2010, &a7ad::DebugGetParentCharColAttr},
        {0x10CCB38u, &::SendCharaNodeParentSetMsg_0x10ccb38, &a7ad::SendCharaNodeParentSetMsg},
        {0x10DFB00u, &::charrootdyn_setChainFindStatus_0x10dfb00, &a7ad::charrootdyn_setChainFindStatus},
        {0x10E0368u, &::directplay_0x10e0368, &a7ad::directplay},
        {0x13417B8u, &::gamecore_sub_013417B8_0x13417b8, &a7ad::gamecore_sub_013417B8},
        {0x1341CA8u, &::gamecore_sub_01341CA8_0x1341ca8, &a7ad::gamecore_sub_01341CA8},
        {0x134C0F8u, &::nico_UnparentObject_0x134c0f8, &a7ad::nico_UnparentObject},
        {0x1375160u, &::gamecore_sub_01375160_0x1375160, &a7ad::gamecore_sub_01375160},
        {0x13795C0u, &::SCRFuncBuddhaMissileMain_0x13795c0, &a7ad::SCRFuncBuddhaMissileMain},
        {0x1379770u, &::SCRFuncBuddhaMissileMain2_0x1379770, &a7ad::SCRFuncBuddhaMissileMain2},
        {0x137D520u, &::ps2__SCRRequestDamageReaction_0x137d520, &a7ad::ps2__SCRRequestDamageReaction},
        {0x13A47F0u, &::gamecore_sub_013A47F0_0x13a47f0, &a7ad::gamecore_sub_013A47F0},
        {0x13A48B8u, &::gamecore_sub_013A48B8_0x13a48b8, &a7ad::gamecore_sub_013A48B8},
        {0x13A4EA0u, &::gamecore_sub_013A4EA0_0x13a4ea0, &a7ad::gamecore_sub_013A4EA0},
        {0x13A6FE0u, &::gamecore_sub_013A6FE0_0x13a6fe0, &a7ad::gamecore_sub_013A6FE0},
        {0x13A84B0u, &::SCRFuncNargaFloatMain_0x13a84b0, &a7ad::SCRFuncNargaFloatMain},
        {0x13CE4D8u, &::gamecore_sub_013CE4D8_0x13ce4d8, &a7ad::gamecore_sub_013CE4D8},
        {0x13FF3B0u, &::gamecore_sub_013FF3B0_0x13ff3b0, &a7ad::gamecore_sub_013FF3B0},
        {0x1405300u, &::gamecore_sub_01405300_0x1405300, &a7ad::gamecore_sub_01405300},
        {0x14454E8u, &::gamecore_sub_014454E8_0x14454e8, &a7ad::gamecore_sub_014454E8},
        {0x14508B0u, &::SCRFuncSendKeyToGameObj_0x14508b0, &a7ad::SCRFuncSendKeyToGameObj},
        {0x1450A08u, &::SCRFuncOutsideSendKeyToGameObj_0x1450a08, &a7ad::SCRFuncOutsideSendKeyToGameObj},
        {0x14531A8u, &::gamecore_sub_014531A8_0x14531a8, &a7ad::gamecore_sub_014531A8},
        {0x1343AC8u, &::GAMEUTIL_is_parent_collision_attribute_boss_0x1343ac8, &a7ad::GAMEUTIL_is_parent_collision_attribute_boss},
        {0x137E8E0u, &::NICO_is_quake_attack_hit_only_boss_0x137e8e0, &a7ad::NICO_is_quake_attack_hit_only_boss},
        {0x13B7078u, &::nico_ChangePlayerHorseRidingParentWithVCol_0x13b7078, &a7ad::nico_ChangePlayerHorseRidingParentWithVCol},
        {0x13F3D48u, &::gamecore_sub_013F3D48_0x13f3d48, &a7ad::gamecore_sub_013F3D48},
    };
}

// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function
uint32_t a7InstallAdapters(PS2Runtime &runtime)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < 297u; ++i)
        if (runtime.lookupFunction(kA7Ad[i].addr) == kA7Ad[i].rec)
        {
            runtime.registerFunction(kA7Ad[i].addr, kA7Ad[i].ad);
            ++n;
        }
    return n;
}
