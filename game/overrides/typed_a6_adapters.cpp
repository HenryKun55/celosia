// typed_a6_adapters.cpp: round A6's adapters for the never-lifted functions stage 6 typed (tools/typed_lift2.py;
// docs/decomp_a6.md). Generated: do not edit. Each takes the recompiled function's slot at install (a6InstallAdapters, from
// sotc_decomp::install, only while the slot still holds the recompiled function): the typed function while the switch a6typed
// is on and every slot it reaches holds what its typed callee stands for (A6_GO / A6_HOLDS, as the lifted functions'
// adapters), else the recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a6.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a6ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

namespace a6ad
{
// ps2___sccl (0x12bf48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2___sccl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12BF48u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(ps2___sccl, a2_4, a2_5); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A6_IN(0); const uint32_t a2_r = typed::ps2___sccl(rdram, runtime, a2_4, a2_5, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ps2___sccl_0x12bf48(rdram, ctx, runtime);
}
// sndSepStopReq (0x1cac80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sndSepStopReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CAC80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(sndSepStopReq, a2_4); A6_IN(4); const uint64_t a2_r = typed::sndSepStopReq(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sndSepStopReq_0x1cac80(rdram, ctx, runtime);
}
// doit_debug_utility (0x107c6c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void doit_debug_utility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C6C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; A6_CHK(doit_debug_utility, a2_f12); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 4)}; A6_IN(5); const a1::RetVF a2_r = typed::doit_debug_utility(rdram, runtime, a2_f12, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 4, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::doit_debug_utility_0x107c6c0(rdram, ctx, runtime);
}
// DEBUG_set_update_position_method_pointer (0x107c750): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_set_update_position_method_pointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C750u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(DEBUG_set_update_position_method_pointer, a2_4); A6_IN(6); const uint32_t a2_r = typed::DEBUG_set_update_position_method_pointer(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_set_update_position_method_pointer_0x107c750(rdram, ctx, runtime);
}
// DEBUG_is_locking_debug_camera (0x107c760): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_is_locking_debug_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C760u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(DEBUG_is_locking_debug_camera); A6_IN(7); const uint32_t a2_r = typed::DEBUG_is_locking_debug_camera(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_is_locking_debug_camera_0x107c760(rdram, ctx, runtime);
}
// GAMEUTIL_get_velocity_by_diff_move3D (0x107fba0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_velocity_by_diff_move3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FBA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GAMEUTIL_get_velocity_by_diff_move3D, a2_4, a2_5, a2_f12, a2_f13); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(8); const float a2_r = typed::GAMEUTIL_get_velocity_by_diff_move3D(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat); A6_OUT(); ctx->f[0] = a2_r; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_velocity_by_diff_move3D_0x107fba0(rdram, ctx, runtime);
}
// GAMEUTIL_get_move_limit_position (0x107fc50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_move_limit_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FC50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(GAMEUTIL_get_move_limit_position, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(9); const float a2_r = typed::GAMEUTIL_get_move_limit_position(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_move_limit_position_0x107fc50(rdram, ctx, runtime);
}
// GAMEUTIL_get_move_limit_position3D (0x107fcf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_move_limit_position3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FCF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GAMEUTIL_get_move_limit_position3D, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); A6_IN(10); const a1::RetVF a2_r = typed::GAMEUTIL_get_move_limit_position3D(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_move_limit_position3D_0x107fcf0(rdram, ctx, runtime);
}
// GAMEUTIL_get_vel_limit_position (0x107fdf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_vel_limit_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FDF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(GAMEUTIL_get_vel_limit_position, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(11); const float a2_r = typed::GAMEUTIL_get_vel_limit_position(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_vel_limit_position_0x107fdf0(rdram, ctx, runtime);
}
// GAMEUTIL_is_inverse_move_direction (0x107fe68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_inverse_move_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FE68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_inverse_move_direction, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(12); const a1::RetVF a2_r = typed::GAMEUTIL_is_inverse_move_direction(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_inverse_move_direction_0x107fe68(rdram, ctx, runtime);
}
// GAMEUTIL_is_max_input (0x10802c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_max_input(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10802C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GAMEUTIL_is_max_input, a2_f12, a2_f13); A6_IN(13); const a1::RetVF a2_r = typed::GAMEUTIL_is_max_input(rdram, runtime, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_max_input_0x10802c0(rdram, ctx, runtime);
}
// GAMEUTIL_is_between_yaw_and_yaw_extend_abs (0x1080c28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_between_yaw_and_yaw_extend_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1080C28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(GAMEUTIL_is_between_yaw_and_yaw_extend_abs, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(14); const a1::RetVF a2_r = typed::GAMEUTIL_is_between_yaw_and_yaw_extend_abs(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_between_yaw_and_yaw_extend_abs_0x1080c28(rdram, ctx, runtime);
}
// GAMEUTIL_is_yaw_dir_sandwich (0x1081dc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_yaw_dir_sandwich(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1081DC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_yaw_dir_sandwich, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(15); const a1::RetVF a2_r = typed::GAMEUTIL_is_yaw_dir_sandwich(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_yaw_dir_sandwich_0x1081dc8(rdram, ctx, runtime);
}
// GAMEUTIL_is_pitch_dir_sandwich (0x1081e58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_pitch_dir_sandwich(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1081E58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_pitch_dir_sandwich, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(16); const a1::RetVF a2_r = typed::GAMEUTIL_is_pitch_dir_sandwich(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_pitch_dir_sandwich_0x1081e58(rdram, ctx, runtime);
}
// GAMEUTIL_is_reach_aim_yaw_dir (0x10826c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_reach_aim_yaw_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10826C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_reach_aim_yaw_dir, a2_4, a2_5, a2_6, a2_7, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(17); const a1::RetVF a2_r = typed::GAMEUTIL_is_reach_aim_yaw_dir(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_reach_aim_yaw_dir_0x10826c0(rdram, ctx, runtime);
}
// GAMEUTIL_is_reach_aim_pitch_dir (0x1082790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_reach_aim_pitch_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1082790u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_reach_aim_pitch_dir, a2_4, a2_5, a2_6, a2_7, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(18); const a1::RetVF a2_r = typed::GAMEUTIL_is_reach_aim_pitch_dir(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_reach_aim_pitch_dir_0x1082790(rdram, ctx, runtime);
}
// GAMEUTIL_set_inside_check_method (0x1082a80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_inside_check_method(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1082A80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(GAMEUTIL_set_inside_check_method, a2_4, a2_5, a2_6); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(19); const uint64_t a2_r = typed::GAMEUTIL_set_inside_check_method(rdram, runtime, a2_4, a2_5, a2_6, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_inside_check_method_0x1082a80(rdram, ctx, runtime);
}
// GAMEUTIL_get_dist_ratio_by_dist_squ (0x1083700): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_dist_ratio_by_dist_squ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1083700u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(GAMEUTIL_get_dist_ratio_by_dist_squ, a2_f12, a2_f13, a2_f14); A6_IN(20); const float a2_r = typed::GAMEUTIL_get_dist_ratio_by_dist_squ(rdram, runtime, a2_f12, a2_f13, a2_f14); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_dist_ratio_by_dist_squ_0x1083700(rdram, ctx, runtime);
}
// getBgAntiLevel (0x10853e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBgAntiLevel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10853E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBgAntiLevel); A6_IN(21); const uint32_t a2_r = typed::getBgAntiLevel(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBgAntiLevel_0x10853e8(rdram, ctx, runtime);
}
// bgInit (0x10853f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10853F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(bgInit); A6_IN(22); typed::bgInit(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgInit_0x10853f8(rdram, ctx, runtime);
}
// fogEffectSetParam (0x1088b10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void fogEffectSetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1088B10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(fogEffectSetParam, a2_4); A6_IN(23); const uint32_t a2_r = typed::fogEffectSetParam(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::fogEffectSetParam_0x1088b10(rdram, ctx, runtime);
}
// getCurrentFsEffect (0x1089278): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCurrentFsEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1089278u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCurrentFsEffect); A6_IN(24); const uint32_t a2_r = typed::getCurrentFsEffect(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCurrentFsEffect_0x1089278(rdram, ctx, runtime);
}
// GetNowFogColorR (0x10898c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetNowFogColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10898C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetNowFogColorR); A6_IN(25); const uint32_t a2_r = typed::GetNowFogColorR(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetNowFogColorR_0x10898c8(rdram, ctx, runtime);
}
// GetNowFogColorG (0x10898d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetNowFogColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10898D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetNowFogColorG); A6_IN(26); const uint32_t a2_r = typed::GetNowFogColorG(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetNowFogColorG_0x10898d8(rdram, ctx, runtime);
}
// GetNowFogColorB (0x10898e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetNowFogColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10898E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetNowFogColorB); A6_IN(27); const uint32_t a2_r = typed::GetNowFogColorB(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetNowFogColorB_0x10898e8(rdram, ctx, runtime);
}
// getFogRate (0x1089948): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFogRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1089948u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFogRate); A6_IN(28); const uint32_t a2_r = typed::getFogRate(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFogRate_0x1089948(rdram, ctx, runtime);
}
// UnlockBgColor (0x1089a28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void UnlockBgColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1089A28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(UnlockBgColor); A6_IN(29); const uint32_t a2_r = typed::UnlockBgColor(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::UnlockBgColor_0x1089a28(rdram, ctx, runtime);
}
// skipBlur (0x108b960): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void skipBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108B960u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(skipBlur, a2_4); A6_IN(30); const uint32_t a2_r = typed::skipBlur(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::skipBlur_0x108b960(rdram, ctx, runtime);
}
// glMatrixMode (0x108e528): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glMatrixMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E528u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(glMatrixMode); A6_IN(31); typed::glMatrixMode(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glMatrixMode_0x108e528(rdram, ctx, runtime);
}
// glColor3f (0x108e7b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glColor3f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E7B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(glColor3f, a2_f12, a2_f13, a2_f14); A6_IN(32); const a1::RetVF a2_r = typed::glColor3f(rdram, runtime, a2_f12, a2_f13, a2_f14); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glColor3f_0x108e7b0(rdram, ctx, runtime);
}
// glColor3fv (0x108e830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glColor3fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E830u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(glColor3fv, a2_4); A6_IN(33); const a1::RetVF a2_r = typed::glColor3fv(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glColor3fv_0x108e830(rdram, ctx, runtime);
}
// glGetColor (0x108ece8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108ECE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(glGetColor, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(34); const a1::RetVF a2_r = typed::glGetColor(rdram, runtime, a2_4, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetColor_0x108ece8(rdram, ctx, runtime);
}
// glGetZTest (0x108ed40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetZTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108ED40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(glGetZTest); A6_IN(35); const uint32_t a2_r = typed::glGetZTest(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetZTest_0x108ed40(rdram, ctx, runtime);
}
// glPopMatrix (0x108ee38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glPopMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108EE38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(glPopMatrix); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(36); const uint32_t a2_r = typed::glPopMatrix(rdram, runtime, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glPopMatrix_0x108ee38(rdram, ctx, runtime);
}
// glTranslatef (0x108ee58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glTranslatef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108EE58u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(glTranslatef, a2_f12, a2_f13, a2_f14); a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(37); const a1::RetVF a2_r = typed::glTranslatef(rdram, runtime, a2_f12, a2_f13, a2_f14, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glTranslatef_0x108ee58(rdram, ctx, runtime);
}
// glLightVector (0x108f420): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glLightVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F420u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(glLightVector, a2_4, a2_5); A6_IN(38); const a1::RetVF a2_r = typed::glLightVector(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glLightVector_0x108f420(rdram, ctx, runtime);
}
// printGifTag (0x108f828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void printGifTag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F828u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(printGifTag); A6_IN(39); typed::printGifTag(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::printGifTag_0x108f828(rdram, ctx, runtime);
}
// printPrimReg (0x108f830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void printPrimReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F830u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(printPrimReg); A6_IN(40); typed::printPrimReg(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::printPrimReg_0x108f830(rdram, ctx, runtime);
}
// glGetZWrite (0x108f838): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetZWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F838u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(glGetZWrite); A6_IN(41); const uint32_t a2_r = typed::glGetZWrite(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetZWrite_0x108f838(rdram, ctx, runtime);
}
// glGetLineSmooth (0x108f848): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetLineSmooth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F848u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(glGetLineSmooth); A6_IN(42); const uint32_t a2_r = typed::glGetLineSmooth(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetLineSmooth_0x108f848(rdram, ctx, runtime);
}
// setv3f (0x108f8a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setv3f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F8A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(setv3f); A6_IN(43); typed::setv3f(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setv3f_0x108f8a8(rdram, ctx, runtime);
}
// LockFullScreenEffect (0x108faf8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void LockFullScreenEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FAF8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(LockFullScreenEffect); A6_IN(44); const uint32_t a2_r = typed::LockFullScreenEffect(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::LockFullScreenEffect_0x108faf8(rdram, ctx, runtime);
}
// UnlockFullScreenEffect (0x108fb08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void UnlockFullScreenEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FB08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(UnlockFullScreenEffect); A6_IN(45); const uint32_t a2_r = typed::UnlockFullScreenEffect(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::UnlockFullScreenEffect_0x108fb08(rdram, ctx, runtime);
}
// setMBlur (0x108fbb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setMBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FBB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setMBlur, a2_4); A6_IN(46); const uint32_t a2_r = typed::setMBlur(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setMBlur_0x108fbb8(rdram, ctx, runtime);
}
// graphFrameBufferReInitializeRequest (0x108fbd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void graphFrameBufferReInitializeRequest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FBD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(graphFrameBufferReInitializeRequest); A6_IN(47); const uint32_t a2_r = typed::graphFrameBufferReInitializeRequest(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::graphFrameBufferReInitializeRequest_0x108fbd8(rdram, ctx, runtime);
}
// getFullScreenEffect (0x108fe40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFullScreenEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FE40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFullScreenEffect); A6_IN(48); const uint32_t a2_r = typed::getFullScreenEffect(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFullScreenEffect_0x108fe40(rdram, ctx, runtime);
}
// getFBOfsU (0x108fe50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFBOfsU(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FE50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFBOfsU); A6_IN(49); const uint32_t a2_r = typed::getFBOfsU(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFBOfsU_0x108fe50(rdram, ctx, runtime);
}
// getFBOfsV (0x108fe60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFBOfsV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FE60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFBOfsV); A6_IN(50); const uint32_t a2_r = typed::getFBOfsV(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFBOfsV_0x108fe60(rdram, ctx, runtime);
}
// getFBOfsX (0x108fe70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFBOfsX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FE70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFBOfsX); A6_IN(51); const uint32_t a2_r = typed::getFBOfsX(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFBOfsX_0x108fe70(rdram, ctx, runtime);
}
// getFBOfsY (0x108fe80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFBOfsY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108FE80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFBOfsY); A6_IN(52); const uint32_t a2_r = typed::getFBOfsY(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFBOfsY_0x108fe80(rdram, ctx, runtime);
}
// kColor (0x1091708): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091708u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(kColor, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(53); const uint32_t a2_r = typed::kColor(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::kColor_0x1091708(rdram, ctx, runtime);
}
// iosSetKPrintfColorContext (0x1091910): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfColorContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091910u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(iosSetKPrintfColorContext, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(54); typed::iosSetKPrintfColorContext(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfColorContext_0x1091910(rdram, ctx, runtime);
}
// iosSetKPrintfLocateContext (0x1091928): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfLocateContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091928u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(iosSetKPrintfLocateContext, a2_4, a2_f12, a2_f13); A6_IN(55); typed::iosSetKPrintfLocateContext(rdram, runtime, a2_4, a2_f12, a2_f13); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfLocateContext_0x1091928(rdram, ctx, runtime);
}
// iosSetKPrintfLocateContextRelativePosition (0x1091940): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfLocateContextRelativePosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091940u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(iosSetKPrintfLocateContextRelativePosition, a2_4, a2_f12, a2_f13); A6_IN(56); const float a2_r = typed::iosSetKPrintfLocateContextRelativePosition(rdram, runtime, a2_4, a2_f12, a2_f13); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfLocateContextRelativePosition_0x1091940(rdram, ctx, runtime);
}
// iosSetKPrintfDynamicsContext (0x1091978): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfDynamicsContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091978u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(iosSetKPrintfDynamicsContext, a2_4, a2_5); A6_IN(57); typed::iosSetKPrintfDynamicsContext(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfDynamicsContext_0x1091978(rdram, ctx, runtime);
}
// iosSetKPrintfDlContext (0x1091980): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfDlContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091980u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(iosSetKPrintfDlContext, a2_4, a2_5); A6_IN(58); typed::iosSetKPrintfDlContext(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfDlContext_0x1091980(rdram, ctx, runtime);
}
// iosSetKPrintfScaleContext (0x1091988): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSetKPrintfScaleContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091988u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(iosSetKPrintfScaleContext, a2_4, a2_f12); A6_IN(59); typed::iosSetKPrintfScaleContext(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSetKPrintfScaleContext_0x1091988(rdram, ctx, runtime);
}
// getFontBoard (0x1091990): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFontBoard(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1091990u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFontBoard); A6_IN(60); const uint32_t a2_r = typed::getFontBoard(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFontBoard_0x1091990(rdram, ctx, runtime);
}
// setSpecularity (0x1092480): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setSpecularity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092480u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; A6_CHK(setSpecularity, a2_f12); A6_IN(61); const uint32_t a2_r = typed::setSpecularity(rdram, runtime, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setSpecularity_0x1092480(rdram, ctx, runtime);
}
// lightsw0 (0x10924d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lightsw0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10924D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lightsw0); A6_IN(62); const uint32_t a2_r = typed::lightsw0(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lightsw0_0x10924d8(rdram, ctx, runtime);
}
// getlc0_r (0x10924e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc0_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10924E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc0_r); A6_IN(63); const uint32_t a2_r = typed::getlc0_r(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc0_r_0x10924e8(rdram, ctx, runtime);
}
// getlc0_g (0x10924f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc0_g(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10924F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc0_g); A6_IN(64); const uint32_t a2_r = typed::getlc0_g(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc0_g_0x10924f8(rdram, ctx, runtime);
}
// getlc0_b (0x1092508): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc0_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092508u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc0_b); A6_IN(65); const uint32_t a2_r = typed::getlc0_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc0_b_0x1092508(rdram, ctx, runtime);
}
// getlv0_h (0x1092518): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv0_h(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092518u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv0_h); A6_IN(66); const uint32_t a2_r = typed::getlv0_h(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv0_h_0x1092518(rdram, ctx, runtime);
}
// getlv0_p (0x1092528): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv0_p(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092528u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv0_p); A6_IN(67); const uint32_t a2_r = typed::getlv0_p(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv0_p_0x1092528(rdram, ctx, runtime);
}
// getlv0_b (0x1092538): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv0_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092538u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv0_b); A6_IN(68); const uint32_t a2_r = typed::getlv0_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv0_b_0x1092538(rdram, ctx, runtime);
}
// lightsw1 (0x1092548): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lightsw1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092548u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lightsw1); A6_IN(69); const uint32_t a2_r = typed::lightsw1(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lightsw1_0x1092548(rdram, ctx, runtime);
}
// getlc1_r (0x1092558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc1_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092558u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc1_r); A6_IN(70); const uint32_t a2_r = typed::getlc1_r(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc1_r_0x1092558(rdram, ctx, runtime);
}
// getlc1_g (0x1092568): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc1_g(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092568u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc1_g); A6_IN(71); const uint32_t a2_r = typed::getlc1_g(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc1_g_0x1092568(rdram, ctx, runtime);
}
// getlc1_b (0x1092578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc1_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092578u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc1_b); A6_IN(72); const uint32_t a2_r = typed::getlc1_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc1_b_0x1092578(rdram, ctx, runtime);
}
// getlv1_h (0x1092588): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv1_h(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092588u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv1_h); A6_IN(73); const uint32_t a2_r = typed::getlv1_h(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv1_h_0x1092588(rdram, ctx, runtime);
}
// getlv1_p (0x1092598): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv1_p(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092598u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv1_p); A6_IN(74); const uint32_t a2_r = typed::getlv1_p(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv1_p_0x1092598(rdram, ctx, runtime);
}
// getlv1_b (0x10925a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv1_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv1_b); A6_IN(75); const uint32_t a2_r = typed::getlv1_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv1_b_0x10925a8(rdram, ctx, runtime);
}
// lightsw2 (0x10925b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lightsw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lightsw2); A6_IN(76); const uint32_t a2_r = typed::lightsw2(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lightsw2_0x10925b8(rdram, ctx, runtime);
}
// getlc2_r (0x10925c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc2_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc2_r); A6_IN(77); const uint32_t a2_r = typed::getlc2_r(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc2_r_0x10925c8(rdram, ctx, runtime);
}
// getlc2_g (0x10925d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc2_g(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc2_g); A6_IN(78); const uint32_t a2_r = typed::getlc2_g(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc2_g_0x10925d8(rdram, ctx, runtime);
}
// getlc2_b (0x10925e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlc2_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlc2_b); A6_IN(79); const uint32_t a2_r = typed::getlc2_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlc2_b_0x10925e8(rdram, ctx, runtime);
}
// getlv2_h (0x10925f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv2_h(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10925F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv2_h); A6_IN(80); const uint32_t a2_r = typed::getlv2_h(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv2_h_0x10925f8(rdram, ctx, runtime);
}
// getlv2_p (0x1092608): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv2_p(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092608u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv2_p); A6_IN(81); const uint32_t a2_r = typed::getlv2_p(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv2_p_0x1092608(rdram, ctx, runtime);
}
// getlv2_b (0x1092618): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getlv2_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092618u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getlv2_b); A6_IN(82); const uint32_t a2_r = typed::getlv2_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getlv2_b_0x1092618(rdram, ctx, runtime);
}
// lightsw3 (0x1092628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lightsw3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lightsw3); A6_IN(83); const uint32_t a2_r = typed::lightsw3(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lightsw3_0x1092628(rdram, ctx, runtime);
}
// getamb_r (0x1092638): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getamb_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092638u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getamb_r); A6_IN(84); const uint32_t a2_r = typed::getamb_r(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getamb_r_0x1092638(rdram, ctx, runtime);
}
// getamb_g (0x1092648): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getamb_g(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092648u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getamb_g); A6_IN(85); const uint32_t a2_r = typed::getamb_g(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getamb_g_0x1092648(rdram, ctx, runtime);
}
// getamb_b (0x1092658): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getamb_b(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092658u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getamb_b); A6_IN(86); const uint32_t a2_r = typed::getamb_b(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getamb_b_0x1092658(rdram, ctx, runtime);
}
// CopyAmbFromAmbdef (0x1092790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CopyAmbFromAmbdef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1092790u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(CopyAmbFromAmbdef); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(87); const uint32_t a2_r = typed::CopyAmbFromAmbdef(rdram, runtime, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CopyAmbFromAmbdef_0x1092790(rdram, ctx, runtime);
}
// getLightVector (0x10927b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLightVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10927B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getLightVector, a2_4); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(88); const uint32_t a2_r = typed::getLightVector(rdram, runtime, a2_4, a4_vf0, a4_vf5, a4_vf7, a4_vacc, a4_vq, a4_vstat); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLightVector_0x10927b0(rdram, ctx, runtime);
}
// manager_sub_010965C8 (0x10965c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010965C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10965C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_010965C8); A6_IN(89); typed::manager_sub_010965C8(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010965C8_0x10965c8(rdram, ctx, runtime);
}
// modelNameByObj (0x1099420): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelNameByObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099420u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(modelNameByObj, a2_4); A6_IN(90); const uint32_t a2_r = typed::modelNameByObj(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelNameByObj_0x1099420(rdram, ctx, runtime);
}
// modelNameByHeader (0x1099430): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelNameByHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099430u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(modelNameByHeader, a2_4); A6_IN(91); const uint32_t a2_r = typed::modelNameByHeader(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelNameByHeader_0x1099430(rdram, ctx, runtime);
}
// modelGetTextureObjByIndex (0x1099560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelGetTextureObjByIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099560u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(modelGetTextureObjByIndex, a2_4, a2_5); A6_IN(92); const uint32_t a2_r = typed::modelGetTextureObjByIndex(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelGetTextureObjByIndex_0x1099560(rdram, ctx, runtime);
}
// modelGetTextureResourceByIndex (0x1099578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelGetTextureResourceByIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099578u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(modelGetTextureResourceByIndex, a2_4, a2_5); A6_IN(93); const uint32_t a2_r = typed::modelGetTextureResourceByIndex(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelGetTextureResourceByIndex_0x1099578(rdram, ctx, runtime);
}
// modelGetLinkCount (0x10995d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelGetLinkCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10995D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(modelGetLinkCount, a2_4); A6_IN(94); const uint64_t a2_r = typed::modelGetLinkCount(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelGetLinkCount_0x10995d0(rdram, ctx, runtime);
}
// modelIncLinkCount (0x10995e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelIncLinkCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10995E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(modelIncLinkCount, a2_4); A6_IN(95); const uint64_t a2_r = typed::modelIncLinkCount(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelIncLinkCount_0x10995e8(rdram, ctx, runtime);
}
// modelDecLinkCount (0x1099620): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelDecLinkCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099620u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(modelDecLinkCount, a2_4); A6_IN(96); const uint64_t a2_r = typed::modelDecLinkCount(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelDecLinkCount_0x1099620(rdram, ctx, runtime);
}
// modelGetName (0x1099750): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelGetName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1099750u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(modelGetName, a2_4); A6_IN(97); const uint32_t a2_r = typed::modelGetName(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelGetName_0x1099750(rdram, ctx, runtime);
}
// meshGetWorkSize (0x109acc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void meshGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109ACC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(meshGetWorkSize, a2_4, a2_5); A6_IN(98); const uint32_t a2_r = typed::meshGetWorkSize(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::meshGetWorkSize_0x109acc8(rdram, ctx, runtime);
}
// reflectionInit (0x109be98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void reflectionInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109BE98u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(reflectionInit); A6_IN(99); typed::reflectionInit(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::reflectionInit_0x109be98(rdram, ctx, runtime);
}
// getShadowDebug (0x109de88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getShadowDebug(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109DE88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getShadowDebug); A6_IN(100); const uint32_t a2_r = typed::getShadowDebug(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getShadowDebug_0x109de88(rdram, ctx, runtime);
}
// setShadowIntensity (0x109de98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setShadowIntensity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109DE98u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setShadowIntensity, a2_4); A6_IN(101); const uint32_t a2_r = typed::setShadowIntensity(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setShadowIntensity_0x109de98(rdram, ctx, runtime);
}
// getShadowDispFlag (0x109e290): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getShadowDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x109E290u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getShadowDispFlag); A6_IN(102); const uint32_t a2_r = typed::getShadowDispFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getShadowDispFlag_0x109e290(rdram, ctx, runtime);
}
// getSolarFlarePanelZ (0x10a17a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSolarFlarePanelZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A17A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSolarFlarePanelZ); A6_IN(103); const uint32_t a2_r = typed::getSolarFlarePanelZ(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSolarFlarePanelZ_0x10a17a8(rdram, ctx, runtime);
}
// getSolarFlareReduceOfsU (0x10a17b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSolarFlareReduceOfsU(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A17B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSolarFlareReduceOfsU); A6_IN(104); const uint32_t a2_r = typed::getSolarFlareReduceOfsU(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSolarFlareReduceOfsU_0x10a17b8(rdram, ctx, runtime);
}
// getSolarFlareReduceOfsV (0x10a17c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSolarFlareReduceOfsV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A17C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSolarFlareReduceOfsV); A6_IN(105); const uint32_t a2_r = typed::getSolarFlareReduceOfsV(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSolarFlareReduceOfsV_0x10a17c8(rdram, ctx, runtime);
}
// getSolarFlareZoomOfsU (0x10a17d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSolarFlareZoomOfsU(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A17D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSolarFlareZoomOfsU); A6_IN(106); const uint32_t a2_r = typed::getSolarFlareZoomOfsU(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSolarFlareZoomOfsU_0x10a17d8(rdram, ctx, runtime);
}
// getSolarFlareZoomOfsV (0x10a17e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSolarFlareZoomOfsV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A17E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSolarFlareZoomOfsV); A6_IN(107); const uint32_t a2_r = typed::getSolarFlareZoomOfsV(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSolarFlareZoomOfsV_0x10a17e8(rdram, ctx, runtime);
}
// getSpriteDispFlag (0x10a54e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSpriteDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A54E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSpriteDispFlag); A6_IN(108); const uint32_t a2_r = typed::getSpriteDispFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSpriteDispFlag_0x10a54e0(rdram, ctx, runtime);
}
// texName (0x10aabd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AABD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(texName, a2_4); A6_IN(109); const uint32_t a2_r = typed::texName(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texName_0x10aabd0(rdram, ctx, runtime);
}
// texNameRes (0x10aac28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texNameRes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AAC28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(texNameRes, a2_4); A6_IN(110); const uint32_t a2_r = typed::texNameRes(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texNameRes_0x10aac28(rdram, ctx, runtime);
}
// texGetCSM1Idx (0x10aac78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texGetCSM1Idx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AAC78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(texGetCSM1Idx, a2_4); A6_IN(111); const uint32_t a2_r = typed::texGetCSM1Idx(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texGetCSM1Idx_0x10aac78(rdram, ctx, runtime);
}
// texGetTexByteSize (0x10aac90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texGetTexByteSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AAC90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(texGetTexByteSize, a2_4, a2_5, a2_6); A6_IN(112); const uint32_t a2_r = typed::texGetTexByteSize(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texGetTexByteSize_0x10aac90(rdram, ctx, runtime);
}
// texGetSize (0x10aacf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texGetSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AACF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(texGetSize, a2_4, a2_5, a2_6); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 7)}; A6_IN(113); const uint32_t a2_r = typed::texGetSize(rdram, runtime, a2_4, a2_5, a2_6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 7, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texGetSize_0x10aacf0(rdram, ctx, runtime);
}
// texGetTextureModuleGroup (0x10ab088): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texGetTextureModuleGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB088u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(texGetTextureModuleGroup); A6_IN(114); const uint32_t a2_r = typed::texGetTextureModuleGroup(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texGetTextureModuleGroup_0x10ab088(rdram, ctx, runtime);
}
// texDelete (0x10ab190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB190u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(texDelete, a2_4); A6_IN(115); const uint32_t a2_r = typed::texDelete(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texDelete_0x10ab190(rdram, ctx, runtime);
}
// texNameResErrorCheck (0x10ab660): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texNameResErrorCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB660u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(texNameResErrorCheck, a2_4); A6_IN(116); const uint32_t a2_r = typed::texNameResErrorCheck(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texNameResErrorCheck_0x10ab660(rdram, ctx, runtime);
}
// getTexTransEnableFlag (0x10ab6b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getTexTransEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB6B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getTexTransEnableFlag); A6_IN(117); const uint32_t a2_r = typed::getTexTransEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getTexTransEnableFlag_0x10ab6b0(rdram, ctx, runtime);
}
// texSetModelName (0x10ab6c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texSetModelName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB6C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(texSetModelName, a2_4); A6_IN(118); const uint32_t a2_r = typed::texSetModelName(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texSetModelName_0x10ab6c0(rdram, ctx, runtime);
}
// getZFogWidth (0x10ac410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getZFogWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AC410u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getZFogWidth); A6_IN(119); const uint32_t a2_r = typed::getZFogWidth(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getZFogWidth_0x10ac410(rdram, ctx, runtime);
}
// getZFogFarMargin (0x10ac420): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getZFogFarMargin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AC420u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getZFogFarMargin); A6_IN(120); const uint32_t a2_r = typed::getZFogFarMargin(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getZFogFarMargin_0x10ac420(rdram, ctx, runtime);
}
// getZFogFar (0x10ac430): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getZFogFar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AC430u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getZFogFar); A6_IN(121); const uint32_t a2_r = typed::getZFogFar(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getZFogFar_0x10ac430(rdram, ctx, runtime);
}
// getBgFogOffset (0x10ac450): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBgFogOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AC450u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBgFogOffset); A6_IN(122); const uint32_t a2_r = typed::getBgFogOffset(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBgFogOffset_0x10ac450(rdram, ctx, runtime);
}
// SetTRS_Onake_From_ObjectAnimation (0x10b61b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetTRS_Onake_From_ObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B61B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); A6_CHK(SetTRS_Onake_From_ObjectAnimation, a2_4, a2_5, a2_6, a2_7, a2_8); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(123); const uint64_t a2_r = typed::SetTRS_Onake_From_ObjectAnimation(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetTRS_Onake_From_ObjectAnimation_0x10b61b0(rdram, ctx, runtime);
}
// GetMotionFrameWithOutsideLimit (0x10b6c78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMotionFrameWithOutsideLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B6C78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GetMotionFrameWithOutsideLimit, a2_4, a2_5, a2_6, a2_f12, a2_f13); A6_IN(124); const a1::RetVF a2_r = typed::GetMotionFrameWithOutsideLimit(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetMotionFrameWithOutsideLimit_0x10b6c78(rdram, ctx, runtime);
}
// GetMotionNodeParentBaseQuaternion (0x10b6fd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMotionNodeParentBaseQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B6FD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(GetMotionNodeParentBaseQuaternion, a2_4, a2_5, a2_6, a2_7); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(125); const uint32_t a2_r = typed::GetMotionNodeParentBaseQuaternion(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_vf0, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetMotionNodeParentBaseQuaternion_0x10b6fd0(rdram, ctx, runtime);
}
// DebugDumpMotion (0x10b74b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugDumpMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B74B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(DebugDumpMotion); A6_IN(126); typed::DebugDumpMotion(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DebugDumpMotion_0x10b74b8(rdram, ctx, runtime);
}
// getPosPQSAnimIntFrame (0x10b74d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPosPQSAnimIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B74D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(getPosPQSAnimIntFrame, a2_4, a2_5, a2_6); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 8)}; A6_IN(127); typed::getPosPQSAnimIntFrame(rdram, runtime, a2_4, a2_5, a2_6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 8, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPosPQSAnimIntFrame_0x10b74d8(rdram, ctx, runtime);
}
// SetMotionDynamicCallFunc (0x10b7640): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetMotionDynamicCallFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B7640u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(SetMotionDynamicCallFunc, a2_4, a2_5, a2_6); A6_IN(128); typed::SetMotionDynamicCallFunc(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetMotionDynamicCallFunc_0x10b7640(rdram, ctx, runtime);
}
// GetMotionMatrixOfChildsFromTarget (0x10b7cf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMotionMatrixOfChildsFromTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B7CF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GetMotionMatrixOfChildsFromTarget, a2_4, a2_5, a2_6, a2_7, a2_9, a2_10, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf24 = ctx->vu0_vf[24]; a4::vec4 a4_vf25 = ctx->vu0_vf[25]; a4::vec4 a4_vf26 = ctx->vu0_vf[26]; a4::vec4 a4_vf27 = ctx->vu0_vf[27]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(129); const uint64_t a2_r = typed::GetMotionMatrixOfChildsFromTarget(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_9, a2_10, a2_29, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf24, a4_vf25, a4_vf26, a4_vf27, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[24] = a4_vf24; ctx->vu0_vf[25] = a4_vf25; ctx->vu0_vf[26] = a4_vf26; ctx->vu0_vf[27] = a4_vf27; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetMotionMatrixOfChildsFromTarget_0x10b7cf0(rdram, ctx, runtime);
}
// GetBlendedMotion (0x10b83d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetBlendedMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B83D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; A6_CHK(GetBlendedMotion, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf9 = ctx->vu0_vf[9]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h12 = a4::hi64(ctx->r[12]); A6_IN(130); typed::GetBlendedMotion(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf9, a4_vf14, a4_vf15, a4_vacc, a4_h2, a4_h3, a4_h8, a4_h12); A6_OUT(); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[9] = a4_vf9; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetBlendedMotion_0x10b83d8(rdram, ctx, runtime);
}
// GetBlendedMotionRoot (0x10b8488): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetBlendedMotionRoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B8488u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; A6_CHK(GetBlendedMotionRoot, a2_4, a2_5, a2_6, a2_f12); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h8 = a4::hi64(ctx->r[8]); A6_IN(131); typed::GetBlendedMotionRoot(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vacc, a4_h8); A6_OUT(); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetBlendedMotionRoot_0x10b8488(rdram, ctx, runtime);
}
// CopyMultiQuaternionFromMotionRotElem (0x10b8cd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CopyMultiQuaternionFromMotionRotElem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B8CD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(CopyMultiQuaternionFromMotionRotElem, a2_4, a2_5, a2_6); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(132); typed::CopyMultiQuaternionFromMotionRotElem(rdram, runtime, a2_4, a2_5, a2_6, a4_vf0, a4_vf14, a4_vf15, a4_vq, a4_vstat); A6_OUT(); ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CopyMultiQuaternionFromMotionRotElem_0x10b8cd0(rdram, ctx, runtime);
}
// InitAnimationData (0x10b8f50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void InitAnimationData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B8F50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(InitAnimationData); A6_IN(133); typed::InitAnimationData(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::InitAnimationData_0x10b8f50(rdram, ctx, runtime);
}
// getBlendFirstMotion (0x10b8f58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBlendFirstMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B8F58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getBlendFirstMotion, a2_4); A6_IN(134); const uint32_t a2_r = typed::getBlendFirstMotion(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBlendFirstMotion_0x10b8f58(rdram, ctx, runtime);
}
// update_node_by_quater_motion_ik_base (0x10b9328): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void update_node_by_quater_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B9328u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(update_node_by_quater_motion_ik_base, a2_4, a2_5); a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(135); const uint32_t a2_r = typed::update_node_by_quater_motion_ik_base(rdram, runtime, a2_4, a2_5, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::update_node_by_quater_motion_ik_base_0x10b9328(rdram, ctx, runtime);
}
// is_doing_motion_ik_interest (0x10ba138): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_doing_motion_ik_interest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA138u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_doing_motion_ik_interest, a2_4); A6_IN(136); const uint32_t a2_r = typed::is_doing_motion_ik_interest(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_doing_motion_ik_interest_0x10ba138(rdram, ctx, runtime);
}
// constructor_neck_turn (0x10ba550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void constructor_neck_turn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA550u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(constructor_neck_turn, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(137); const uint64_t a2_r = typed::constructor_neck_turn(rdram, runtime, a2_4, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::constructor_neck_turn_0x10ba550(rdram, ctx, runtime);
}
// begin_neck_turn (0x10ba578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void begin_neck_turn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA578u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(begin_neck_turn, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(138); const uint64_t a2_r = typed::begin_neck_turn(rdram, runtime, a2_4, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::begin_neck_turn_0x10ba578(rdram, ctx, runtime);
}
// request_end_neck_turn (0x10ba5b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void request_end_neck_turn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA5B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(request_end_neck_turn, a2_4); A6_IN(139); const uint32_t a2_r = typed::request_end_neck_turn(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::request_end_neck_turn_0x10ba5b0(rdram, ctx, runtime);
}
// is_doit_neck_turn_need (0x10ba5d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_doit_neck_turn_need(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA5D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_doit_neck_turn_need, a2_4); A6_IN(140); const uint32_t a2_r = typed::is_doit_neck_turn_need(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_doit_neck_turn_need_0x10ba5d0(rdram, ctx, runtime);
}
// manager_sub_010BA688 (0x10ba688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010BA688(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA688u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(manager_sub_010BA688, a2_4, a2_f12, a2_f13, a2_f14); A6_IN(141); const float a2_r = typed::manager_sub_010BA688(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010BA688_0x10ba688(rdram, ctx, runtime);
}
// GetAnimationDefObjLeaveType (0x10bb050): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetAnimationDefObjLeaveType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BB050u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetAnimationDefObjLeaveType, a2_4); A6_IN(142); const uint64_t a2_r = typed::GetAnimationDefObjLeaveType(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetAnimationDefObjLeaveType_0x10bb050(rdram, ctx, runtime);
}
// GetAnimationDefObjWorkSize (0x10bb238): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetAnimationDefObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BB238u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetAnimationDefObjWorkSize, a2_4); A6_IN(143); const uint32_t a2_r = typed::GetAnimationDefObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetAnimationDefObjWorkSize_0x10bb238(rdram, ctx, runtime);
}
// GetCharacterClassInitParamNbNodes (0x10c0c58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCharacterClassInitParamNbNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C0C58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetCharacterClassInitParamNbNodes, a2_4); A6_IN(145); const uint32_t a2_r = typed::GetCharacterClassInitParamNbNodes(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCharacterClassInitParamNbNodes_0x10c0c58(rdram, ctx, runtime);
}
// GetCharacterClassControlDirection (0x10c0f90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCharacterClassControlDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C0F90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(GetCharacterClassControlDirection, a2_4, a2_5, a2_6); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(146); const uint32_t a2_r = typed::GetCharacterClassControlDirection(rdram, runtime, a2_4, a2_5, a2_6, a4_vf0, a4_vf8, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCharacterClassControlDirection_0x10c0f90(rdram, ctx, runtime);
}
// SetCharacterObjColSphereOverrideFunc (0x10c2970): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetCharacterObjColSphereOverrideFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2970u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(SetCharacterObjColSphereOverrideFunc, a2_4, a2_5, a2_6); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 7)}; A6_IN(147); const uint32_t a2_r = typed::SetCharacterObjColSphereOverrideFunc(rdram, runtime, a2_4, a2_5, a2_6, a4_h4, a4_h7, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 7, a3_xo[0]); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetCharacterObjColSphereOverrideFunc_0x10c2970(rdram, ctx, runtime);
}
// SetCharacterObjScriptKeyEnableFlagOnDemoOrient (0x10c2bb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetCharacterObjScriptKeyEnableFlagOnDemoOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2BB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(SetCharacterObjScriptKeyEnableFlagOnDemoOrient, a2_4, a2_5); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6)}; A6_IN(148); const uint32_t a2_r = typed::SetCharacterObjScriptKeyEnableFlagOnDemoOrient(rdram, runtime, a2_4, a2_5, a4_h4, a4_h6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetCharacterObjScriptKeyEnableFlagOnDemoOrient_0x10c2bb8(rdram, ctx, runtime);
}
// getMotionDebugPrintMode (0x10c2e60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMotionDebugPrintMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2E60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMotionDebugPrintMode); A6_IN(149); const uint32_t a2_r = typed::getMotionDebugPrintMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMotionDebugPrintMode_0x10c2e60(rdram, ctx, runtime);
}
// getAddMotionEnableFlag (0x10c2e70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getAddMotionEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2E70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getAddMotionEnableFlag); A6_IN(150); const uint32_t a2_r = typed::getAddMotionEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getAddMotionEnableFlag_0x10c2e70(rdram, ctx, runtime);
}
// getIkControlEnableFlag (0x10c2e80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getIkControlEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2E80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getIkControlEnableFlag); A6_IN(151); const uint32_t a2_r = typed::getIkControlEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getIkControlEnableFlag_0x10c2e80(rdram, ctx, runtime);
}
// getRootMoveDisableFlag (0x10c2e90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getRootMoveDisableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2E90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getRootMoveDisableFlag); A6_IN(152); const uint32_t a2_r = typed::getRootMoveDisableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getRootMoveDisableFlag_0x10c2e90(rdram, ctx, runtime);
}
// dispCollisionFinderFlag (0x10c2ea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dispCollisionFinderFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2EA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(dispCollisionFinderFlag); A6_IN(153); const uint32_t a2_r = typed::dispCollisionFinderFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dispCollisionFinderFlag_0x10c2ea0(rdram, ctx, runtime);
}
// dispCollisionKeepAreaFlag (0x10c2eb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dispCollisionKeepAreaFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2EB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(dispCollisionKeepAreaFlag); A6_IN(154); const uint32_t a2_r = typed::dispCollisionKeepAreaFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dispCollisionKeepAreaFlag_0x10c2eb0(rdram, ctx, runtime);
}
// getCollisionDisableAtDirectPlayFlag (0x10c2ec0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionDisableAtDirectPlayFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2EC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionDisableAtDirectPlayFlag); A6_IN(155); const uint32_t a2_r = typed::getCollisionDisableAtDirectPlayFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionDisableAtDirectPlayFlag_0x10c2ec0(rdram, ctx, runtime);
}
// ResetCharacterClassIKNodeInterpInfo (0x10c3298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ResetCharacterClassIKNodeInterpInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C3298u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(ResetCharacterClassIKNodeInterpInfo, a2_4); A6_IN(156); const uint32_t a2_r = typed::ResetCharacterClassIKNodeInterpInfo(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ResetCharacterClassIKNodeInterpInfo_0x10c3298(rdram, ctx, runtime);
}
// EnableCharacterObjAdditionMotion (0x10c3448): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void EnableCharacterObjAdditionMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C3448u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(EnableCharacterObjAdditionMotion, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(157); const uint32_t a2_r = typed::EnableCharacterObjAdditionMotion(rdram, runtime, a2_4, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::EnableCharacterObjAdditionMotion_0x10c3448(rdram, ctx, runtime);
}
// DisableCharacterObjAdditionMotion (0x10c3478): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DisableCharacterObjAdditionMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C3478u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(DisableCharacterObjAdditionMotion, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(158); const uint32_t a2_r = typed::DisableCharacterObjAdditionMotion(rdram, runtime, a2_4, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DisableCharacterObjAdditionMotion_0x10c3478(rdram, ctx, runtime);
}
// DisableCharacterObjAddParentVelocity (0x10c34a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DisableCharacterObjAddParentVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C34A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(DisableCharacterObjAddParentVelocity, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(159); const uint32_t a2_r = typed::DisableCharacterObjAddParentVelocity(rdram, runtime, a2_4, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DisableCharacterObjAddParentVelocity_0x10c34a8(rdram, ctx, runtime);
}
// EnableCharacterObjAddParentVelocity (0x10c34d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void EnableCharacterObjAddParentVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C34D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(EnableCharacterObjAddParentVelocity, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(160); const uint32_t a2_r = typed::EnableCharacterObjAddParentVelocity(rdram, runtime, a2_4, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::EnableCharacterObjAddParentVelocity_0x10c34d8(rdram, ctx, runtime);
}
// getMotionPrintTarget (0x10cc6e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMotionPrintTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC6E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMotionPrintTarget); A6_IN(161); const uint32_t a2_r = typed::getMotionPrintTarget(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMotionPrintTarget_0x10cc6e8(rdram, ctx, runtime);
}
// getLocalAccelDispFlag (0x10cc6f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLocalAccelDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC6F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLocalAccelDispFlag); A6_IN(162); const uint32_t a2_r = typed::getLocalAccelDispFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLocalAccelDispFlag_0x10cc6f8(rdram, ctx, runtime);
}
// getScriptSwitchDispSw (0x10cc708): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getScriptSwitchDispSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC708u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getScriptSwitchDispSw); A6_IN(163); const uint32_t a2_r = typed::getScriptSwitchDispSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getScriptSwitchDispSw_0x10cc708(rdram, ctx, runtime);
}
// resetOutsideScriptCtrlStatus (0x10cc718): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void resetOutsideScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC718u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(resetOutsideScriptCtrlStatus, a2_4); A6_IN(164); typed::resetOutsideScriptCtrlStatus(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::resetOutsideScriptCtrlStatus_0x10cc718(rdram, ctx, runtime);
}
// GetPadControlFlag (0x10cc720): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetPadControlFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10CC720u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetPadControlFlag); A6_IN(165); const uint32_t a2_r = typed::GetPadControlFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetPadControlFlag_0x10cc720(rdram, ctx, runtime);
}
// SearchSubAnimation (0x10d2a38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SearchSubAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2A38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SearchSubAnimation, a2_4, a2_5); A6_IN(166); const uint64_t a2_r = typed::SearchSubAnimation(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SearchSubAnimation_0x10d2a38(rdram, ctx, runtime);
}
// RequestCharacterClassMotionChange (0x10d2ab0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void RequestCharacterClassMotionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2AB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(RequestCharacterClassMotionChange, a2_4, a2_5, a2_6); A6_IN(167); const uint32_t a2_r = typed::RequestCharacterClassMotionChange(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::RequestCharacterClassMotionChange_0x10d2ab0(rdram, ctx, runtime);
}
// RequestCharacterClassMotionDirectionByGPAirBalanceDir (0x10d2b80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void RequestCharacterClassMotionDirectionByGPAirBalanceDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2B80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(RequestCharacterClassMotionDirectionByGPAirBalanceDir, a2_4); A6_IN(168); const uint32_t a2_r = typed::RequestCharacterClassMotionDirectionByGPAirBalanceDir(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::RequestCharacterClassMotionDirectionByGPAirBalanceDir_0x10d2b80(rdram, ctx, runtime);
}
// CancelCharacterClassMotionChange (0x10d2cd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CancelCharacterClassMotionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2CD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CancelCharacterClassMotionChange, a2_4); A6_IN(169); typed::CancelCharacterClassMotionChange(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CancelCharacterClassMotionChange_0x10d2cd8(rdram, ctx, runtime);
}
// getMotionInterpMode (0x10d2fc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMotionInterpMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2FC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMotionInterpMode); A6_IN(170); const uint32_t a2_r = typed::getMotionInterpMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMotionInterpMode_0x10d2fc8(rdram, ctx, runtime);
}
// getJumpSpeedLimitEnableFlag (0x10d2fd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getJumpSpeedLimitEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10D2FD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getJumpSpeedLimitEnableFlag); A6_IN(171); const uint32_t a2_r = typed::getJumpSpeedLimitEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getJumpSpeedLimitEnableFlag_0x10d2fd8(rdram, ctx, runtime);
}
// charrootdyn_setEdgeUpperPlaneInfoFlag (0x10dde08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void charrootdyn_setEdgeUpperPlaneInfoFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DDE08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(charrootdyn_setEdgeUpperPlaneInfoFlag, a2_4); A6_IN(172); const uint32_t a2_r = typed::charrootdyn_setEdgeUpperPlaneInfoFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::charrootdyn_setEdgeUpperPlaneInfoFlag_0x10dde08(rdram, ctx, runtime);
}
// charrootdyn_resetEdgeUpperPlaneInfoFlag (0x10dde18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void charrootdyn_resetEdgeUpperPlaneInfoFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DDE18u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(charrootdyn_resetEdgeUpperPlaneInfoFlag, a2_4); A6_IN(173); typed::charrootdyn_resetEdgeUpperPlaneInfoFlag(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::charrootdyn_resetEdgeUpperPlaneInfoFlag_0x10dde18(rdram, ctx, runtime);
}
// charrootdyn_getForceLimitMode (0x10de340): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void charrootdyn_getForceLimitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DE340u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(charrootdyn_getForceLimitMode, a2_4, a2_5); A6_IN(174); const a1::RetVF a2_r = typed::charrootdyn_getForceLimitMode(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::charrootdyn_getForceLimitMode_0x10de340(rdram, ctx, runtime);
}
// getEdgeHangDebugFlag (0x10df560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getEdgeHangDebugFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DF560u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getEdgeHangDebugFlag); A6_IN(175); const uint32_t a2_r = typed::getEdgeHangDebugFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getEdgeHangDebugFlag_0x10df560(rdram, ctx, runtime);
}
// getAirResistEnableFlag (0x10df570): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getAirResistEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DF570u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getAirResistEnableFlag); A6_IN(176); const uint32_t a2_r = typed::getAirResistEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getAirResistEnableFlag_0x10df570(rdram, ctx, runtime);
}
// getDisableExtendWallFlag (0x10df580): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDisableExtendWallFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10DF580u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDisableExtendWallFlag); A6_IN(177); const uint32_t a2_r = typed::getDisableExtendWallFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDisableExtendWallFlag_0x10df580(rdram, ctx, runtime);
}
// manager_sub_010E0010 (0x10e0010): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010E0010(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E0010u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(manager_sub_010E0010, a2_4, a2_5); A6_IN(178); const uint32_t a2_r = typed::manager_sub_010E0010(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010E0010_0x10e0010(rdram, ctx, runtime);
}
// getIkExpressionSwBool (0x10e1eb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getIkExpressionSwBool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E1EB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getIkExpressionSwBool); A6_IN(179); const uint32_t a2_r = typed::getIkExpressionSwBool(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getIkExpressionSwBool_0x10e1eb8(rdram, ctx, runtime);
}
// getRootFittingEnableFlag (0x10e1ec8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getRootFittingEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E1EC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getRootFittingEnableFlag); A6_IN(180); const uint32_t a2_r = typed::getRootFittingEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getRootFittingEnableFlag_0x10e1ec8(rdram, ctx, runtime);
}
// getRootFittingMode (0x10e1ed8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getRootFittingMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E1ED8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getRootFittingMode); A6_IN(181); const uint32_t a2_r = typed::getRootFittingMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getRootFittingMode_0x10e1ed8(rdram, ctx, runtime);
}
// getIkExpressionSw (0x10e1ee8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getIkExpressionSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E1EE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getIkExpressionSw); A6_IN(182); const uint32_t a2_r = typed::getIkExpressionSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getIkExpressionSw_0x10e1ee8(rdram, ctx, runtime);
}
// GetFogColMan (0x10ed538): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetFogColMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10ED538u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetFogColMan); A6_IN(183); const uint32_t a2_r = typed::GetFogColMan(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetFogColMan_0x10ed538(rdram, ctx, runtime);
}
// GetFogColClass (0x10ed548): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetFogColClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10ED548u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetFogColClass, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(184); const uint64_t a2_r = typed::GetFogColClass(rdram, runtime, a2_4, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetFogColClass_0x10ed548(rdram, ctx, runtime);
}
// ForceFogColFlagEnvelope (0x10ed568): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ForceFogColFlagEnvelope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10ED568u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(ForceFogColFlagEnvelope, a2_4); A6_IN(185); const uint32_t a2_r = typed::ForceFogColFlagEnvelope(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ForceFogColFlagEnvelope_0x10ed568(rdram, ctx, runtime);
}
// manager_sub_010ED5D8 (0x10ed5d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010ED5D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10ED5D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_010ED5D8); A6_IN(186); typed::manager_sub_010ED5D8(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010ED5D8_0x10ed5d8(rdram, ctx, runtime);
}
// GetCenterPositionOfJudgeVolumeInfo (0x10eead0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCenterPositionOfJudgeVolumeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10EEAD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GetCenterPositionOfJudgeVolumeInfo, a2_4, a2_5, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(187); const uint64_t a2_r = typed::GetCenterPositionOfJudgeVolumeInfo(rdram, runtime, a2_4, a2_5, a2_29, a4_vf0, a4_vf4, a4_vf8, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCenterPositionOfJudgeVolumeInfo_0x10eead0(rdram, ctx, runtime);
}
// GetLayoutClassCollisionBoundaryInfo (0x10f3030): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLayoutClassCollisionBoundaryInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F3030u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(GetLayoutClassCollisionBoundaryInfo, a2_4, a2_5); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(188); const uint64_t a2_r = typed::GetLayoutClassCollisionBoundaryInfo(rdram, runtime, a2_4, a2_5, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLayoutClassCollisionBoundaryInfo_0x10f3030(rdram, ctx, runtime);
}
// GetLayoutRootMatrix (0x10f3398): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLayoutRootMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F3398u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(GetLayoutRootMatrix, a2_4, a2_5); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(189); const uint32_t a2_r = typed::GetLayoutRootMatrix(rdram, runtime, a2_4, a2_5, a4_vf0, a4_vf4, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLayoutRootMatrix_0x10f3398(rdram, ctx, runtime);
}
// getSeqNo (0x10f3450): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSeqNo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F3450u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSeqNo); A6_IN(190); const uint32_t a2_r = typed::getSeqNo(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSeqNo_0x10f3450(rdram, ctx, runtime);
}
// getSubObjDispSw (0x10f5b08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSubObjDispSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F5B08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSubObjDispSw); A6_IN(191); const uint32_t a2_r = typed::getSubObjDispSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSubObjDispSw_0x10f5b08(rdram, ctx, runtime);
}
// getMotionBlurSw (0x10f5b18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMotionBlurSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F5B18u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMotionBlurSw); A6_IN(192); const uint32_t a2_r = typed::getMotionBlurSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMotionBlurSw_0x10f5b18(rdram, ctx, runtime);
}
// motionBlurTestType (0x10f5b28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void motionBlurTestType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F5B28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(motionBlurTestType); A6_IN(193); const uint32_t a2_r = typed::motionBlurTestType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::motionBlurTestType_0x10f5b28(rdram, ctx, runtime);
}
// manager_sub_010F5BC0 (0x10f5bc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010F5BC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F5BC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_010F5BC0); A6_IN(194); typed::manager_sub_010F5BC0(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010F5BC0_0x10f5bc0(rdram, ctx, runtime);
}
// getColLightDebugSts (0x10f6570): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getColLightDebugSts(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F6570u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getColLightDebugSts); A6_IN(195); const uint32_t a2_r = typed::getColLightDebugSts(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getColLightDebugSts_0x10f6570(rdram, ctx, runtime);
}
// GetColLightDebugSts (0x10f6580): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetColLightDebugSts(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F6580u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetColLightDebugSts); A6_IN(196); const uint32_t a2_r = typed::GetColLightDebugSts(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetColLightDebugSts_0x10f6580(rdram, ctx, runtime);
}
// deleteGameLightObj (0x10f74e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteGameLightObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F74E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteGameLightObj); A6_IN(197); typed::deleteGameLightObj(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteGameLightObj_0x10f74e0(rdram, ctx, runtime);
}
// GameLightCalcOutSideParam (0x10f7830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GameLightCalcOutSideParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F7830u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GameLightCalcOutSideParam, a2_4); A6_IN(198); const float a2_r = typed::GameLightCalcOutSideParam(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GameLightCalcOutSideParam_0x10f7830(rdram, ctx, runtime);
}
// InitGameLightClass (0x10f78b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void InitGameLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F78B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(InitGameLightClass); A6_IN(199); typed::InitGameLightClass(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::InitGameLightClass_0x10f78b8(rdram, ctx, runtime);
}
// GameLightCalcImpact (0x10f78c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GameLightCalcImpact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F78C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(GameLightCalcImpact, a2_4, a2_f12); A6_IN(200); const float a2_r = typed::GameLightCalcImpact(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GameLightCalcImpact_0x10f78c0(rdram, ctx, runtime);
}
// deleteGlobalLightObj (0x10f8b50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteGlobalLightObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F8B50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteGlobalLightObj); A6_IN(201); typed::deleteGlobalLightObj(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteGlobalLightObj_0x10f8b50(rdram, ctx, runtime);
}
// GlobalLightCalcOutSideParam (0x10f8be0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GlobalLightCalcOutSideParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F8BE0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GlobalLightCalcOutSideParam, a2_4, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(202); const a1::RetVF a2_r = typed::GlobalLightCalcOutSideParam(rdram, runtime, a2_4, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GlobalLightCalcOutSideParam_0x10f8be0(rdram, ctx, runtime);
}
// GetNextWorkLightClass (0x10f9270): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetNextWorkLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F9270u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetNextWorkLightClass, a2_4); A6_IN(203); const uint64_t a2_r = typed::GetNextWorkLightClass(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetNextWorkLightClass_0x10f9270(rdram, ctx, runtime);
}
// GetDirColorFromWorkLight (0x10f9300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDirColorFromWorkLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F9300u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(GetDirColorFromWorkLight, a2_4, a2_5, a2_6); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(204); const a1::RetVF a2_r = typed::GetDirColorFromWorkLight(rdram, runtime, a2_4, a2_5, a2_6, a4_vf5, a4_vf6, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDirColorFromWorkLight_0x10f9300(rdram, ctx, runtime);
}
// GetLightClass (0x10f93f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F93F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetLightClass, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 4)}; A6_IN(205); const uint64_t a2_r = typed::GetLightClass(rdram, runtime, a2_4, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLightClass_0x10f93f0(rdram, ctx, runtime);
}
// InitAmbientLightObj (0x10f9530): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void InitAmbientLightObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F9530u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(InitAmbientLightObj, a2_4, a2_5); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(206); const uint32_t a2_r = typed::InitAmbientLightObj(rdram, runtime, a2_4, a2_5, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::InitAmbientLightObj_0x10f9530(rdram, ctx, runtime);
}
// manager_sub_010FAC78 (0x10fac78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010FAC78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10FAC78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; A6_CHK(manager_sub_010FAC78, a2_4, a2_29, a2_f12); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf9 = ctx->vu0_vf[9]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A6_IN(207); const uint64_t a2_r = typed::manager_sub_010FAC78(rdram, runtime, a2_4, a2_29, a2_f12, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf9, a4_vf14, a4_vf15, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h8, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[9] = a4_vf9; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010FAC78_0x10fac78(rdram, ctx, runtime);
}
// IsLwsOrientRootNode (0x10fc190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsLwsOrientRootNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10FC190u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(IsLwsOrientRootNode, a2_4); A6_IN(208); const uint64_t a2_r = typed::IsLwsOrientRootNode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::IsLwsOrientRootNode_0x10fc190(rdram, ctx, runtime);
}
// initSkelObjResource (0x1100938): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initSkelObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1100938u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(initSkelObjResource, a2_4); A6_IN(209); const uint64_t a2_r = typed::initSkelObjResource(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initSkelObjResource_0x1100938(rdram, ctx, runtime);
}
// initDispObjResource (0x11009a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initDispObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11009A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(initDispObjResource, a2_4); A6_IN(210); const uint32_t a2_r = typed::initDispObjResource(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initDispObjResource_0x11009a8(rdram, ctx, runtime);
}
// initShadowObjResource (0x1100a18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initShadowObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1100A18u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(initShadowObjResource, a2_4); A6_IN(211); const uint32_t a2_r = typed::initShadowObjResource(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initShadowObjResource_0x1100a18(rdram, ctx, runtime);
}
// initColObjResource (0x1100a88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initColObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1100A88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(initColObjResource, a2_4); A6_IN(212); const uint32_t a2_r = typed::initColObjResource(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initColObjResource_0x1100a88(rdram, ctx, runtime);
}
// initClothObjResource (0x1100b68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initClothObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1100B68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(initClothObjResource, a2_4); A6_IN(213); const uint32_t a2_r = typed::initClothObjResource(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initClothObjResource_0x1100b68(rdram, ctx, runtime);
}
// AddObjectManagerCollisionWorkSizeBoss (0x1101690): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AddObjectManagerCollisionWorkSizeBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1101690u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(AddObjectManagerCollisionWorkSizeBoss, a2_4); A6_IN(214); const uint32_t a2_r = typed::AddObjectManagerCollisionWorkSizeBoss(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AddObjectManagerCollisionWorkSizeBoss_0x1101690(rdram, ctx, runtime);
}
// AddObjectManagerCollisionWorkSizeStage (0x11016a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AddObjectManagerCollisionWorkSizeStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11016A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(AddObjectManagerCollisionWorkSizeStage, a2_4); A6_IN(215); const uint32_t a2_r = typed::AddObjectManagerCollisionWorkSizeStage(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AddObjectManagerCollisionWorkSizeStage_0x11016a8(rdram, ctx, runtime);
}
// AddObjectManagerGameObjectSize (0x11016d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AddObjectManagerGameObjectSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11016D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(AddObjectManagerGameObjectSize, a2_4); A6_IN(216); const uint32_t a2_r = typed::AddObjectManagerGameObjectSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AddObjectManagerGameObjectSize_0x11016d8(rdram, ctx, runtime);
}
// AddObjectManagerSeamlessWorkSize (0x11016f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AddObjectManagerSeamlessWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11016F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(AddObjectManagerSeamlessWorkSize, a2_4); A6_IN(217); const uint32_t a2_r = typed::AddObjectManagerSeamlessWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AddObjectManagerSeamlessWorkSize_0x11016f0(rdram, ctx, runtime);
}
// getObjInitMessageDebugSw (0x1101708): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getObjInitMessageDebugSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1101708u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getObjInitMessageDebugSw); A6_IN(218); const uint32_t a2_r = typed::getObjInitMessageDebugSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getObjInitMessageDebugSw_0x1101708(rdram, ctx, runtime);
}
// GetModelObjGroup (0x1101728): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetModelObjGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1101728u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetModelObjGroup); A6_IN(219); const uint32_t a2_r = typed::GetModelObjGroup(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetModelObjGroup_0x1101728(rdram, ctx, runtime);
}
// deleteAmbientLightObj (0x11023c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteAmbientLightObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11023C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteAmbientLightObj); A6_IN(220); typed::deleteAmbientLightObj(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteAmbientLightObj_0x11023c8(rdram, ctx, runtime);
}
// getMemoryInfoMode (0x11023d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMemoryInfoMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11023D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMemoryInfoMode); A6_IN(221); const uint32_t a2_r = typed::getMemoryInfoMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMemoryInfoMode_0x11023d0(rdram, ctx, runtime);
}
// getMemoryInfoLightSw (0x11023e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMemoryInfoLightSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11023E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMemoryInfoLightSw); A6_IN(222); const uint32_t a2_r = typed::getMemoryInfoLightSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMemoryInfoLightSw_0x11023e0(rdram, ctx, runtime);
}
// getStackViewFlag (0x1102c78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStackViewFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1102C78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getStackViewFlag); A6_IN(223); const uint32_t a2_r = typed::getStackViewFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStackViewFlag_0x1102c78(rdram, ctx, runtime);
}
// manager_sub_011030A8 (0x11030a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_011030A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11030A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(manager_sub_011030A8, a2_4); A6_IN(224); const uint64_t a2_r = typed::manager_sub_011030A8(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_011030A8_0x11030a8(rdram, ctx, runtime);
}
// getLWSDebugMode (0x110a828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLWSDebugMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110A828u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLWSDebugMode); A6_IN(225); const uint32_t a2_r = typed::getLWSDebugMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLWSDebugMode_0x110a828(rdram, ctx, runtime);
}
// SetScriptAnimationObjGeometryOverrideCallBackFunc (0x110a8a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetScriptAnimationObjGeometryOverrideCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110A8A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(SetScriptAnimationObjGeometryOverrideCallBackFunc, a2_4, a2_5, a2_6); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(226); const uint32_t a2_r = typed::SetScriptAnimationObjGeometryOverrideCallBackFunc(rdram, runtime, a2_4, a2_5, a2_6, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetScriptAnimationObjGeometryOverrideCallBackFunc_0x110a8a8(rdram, ctx, runtime);
}
// GetScriptAnimationObjResourcePlayTime (0x110be10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetScriptAnimationObjResourcePlayTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110BE10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetScriptAnimationObjResourcePlayTime, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(227); const a1::RetVF a2_r = typed::GetScriptAnimationObjResourcePlayTime(rdram, runtime, a2_4, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetScriptAnimationObjResourcePlayTime_0x110be10(rdram, ctx, runtime);
}
// getLWSDebugMode2 (0x110c130): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLWSDebugMode2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110C130u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLWSDebugMode2); A6_IN(228); const uint32_t a2_r = typed::getLWSDebugMode2(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLWSDebugMode2_0x110c130(rdram, ctx, runtime);
}
// getExternalLWSFlag (0x110c140): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getExternalLWSFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110C140u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getExternalLWSFlag); A6_IN(229); const uint32_t a2_r = typed::getExternalLWSFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getExternalLWSFlag_0x110c140(rdram, ctx, runtime);
}
// GetMotionResourceTime (0x110c7e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMotionResourceTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110C7E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(GetMotionResourceTime, a2_4, a2_f12); A6_IN(230); const a1::RetVF a2_r = typed::GetMotionResourceTime(rdram, runtime, a2_4, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetMotionResourceTime_0x110c7e8(rdram, ctx, runtime);
}
// SetScrLwsOriObjReleaseCallBackFunc (0x110d918): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetScrLwsOriObjReleaseCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110D918u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(SetScrLwsOriObjReleaseCallBackFunc, a2_4, a2_5, a2_6); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(231); const uint32_t a2_r = typed::SetScrLwsOriObjReleaseCallBackFunc(rdram, runtime, a2_4, a2_5, a2_6, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetScrLwsOriObjReleaseCallBackFunc_0x110d918(rdram, ctx, runtime);
}
// SetScriptLwsOrientObjDefaultScriptFunc (0x110d9f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetScriptLwsOrientObjDefaultScriptFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110D9F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetScriptLwsOrientObjDefaultScriptFunc, a2_4); A6_IN(232); const uint32_t a2_r = typed::SetScriptLwsOrientObjDefaultScriptFunc(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetScriptLwsOrientObjDefaultScriptFunc_0x110d9f0(rdram, ctx, runtime);
}
// SpriteMenuGetSpriteMenuClassParam (0x1111e08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetSpriteMenuClassParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1111E08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SpriteMenuGetSpriteMenuClassParam, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(233); const uint32_t a2_r = typed::SpriteMenuGetSpriteMenuClassParam(rdram, runtime, a2_4, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetSpriteMenuClassParam_0x1111e08(rdram, ctx, runtime);
}
// SpriteMenuGetPartsParamByHash (0x1112088): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetPartsParamByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112088u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SpriteMenuGetPartsParamByHash, a2_4, a2_5, a2_6); A6_IN(234); const uint64_t a2_r = typed::SpriteMenuGetPartsParamByHash(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetPartsParamByHash_0x1112088(rdram, ctx, runtime);
}
// SpriteMenuGetPartsDefByHash (0x1112260): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetPartsDefByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112260u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SpriteMenuGetPartsDefByHash, a2_4, a2_5, a2_6); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10)}; A6_IN(235); const uint32_t a2_r = typed::SpriteMenuGetPartsDefByHash(rdram, runtime, a2_4, a2_5, a2_6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetPartsDefByHash_0x1112260(rdram, ctx, runtime);
}
// SpriteMenuGetPartsDefValueByHash (0x1112458): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetPartsDefValueByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112458u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SpriteMenuGetPartsDefValueByHash, a2_4, a2_5, a2_6); A6_IN(236); const uint64_t a2_r = typed::SpriteMenuGetPartsDefValueByHash(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetPartsDefValueByHash_0x1112458(rdram, ctx, runtime);
}
// SpriteMenuSetPartsDefValueByHash (0x1112650): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetPartsDefValueByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112650u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(SpriteMenuSetPartsDefValueByHash, a2_4, a2_5, a2_6, a2_7); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(237); const uint64_t a2_r = typed::SpriteMenuSetPartsDefValueByHash(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetPartsDefValueByHash_0x1112650(rdram, ctx, runtime);
}
// SpriteMenuGetMenuIdByPartsId (0x1112c10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetMenuIdByPartsId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112C10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SpriteMenuGetMenuIdByPartsId, a2_4, a2_5, a2_6); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(238); const uint32_t a2_r = typed::SpriteMenuGetMenuIdByPartsId(rdram, runtime, a2_4, a2_5, a2_6, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetMenuIdByPartsId_0x1112c10(rdram, ctx, runtime);
}
// SpriteMenuGetSelectMenuId (0x1112ea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuGetSelectMenuId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1112EA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SpriteMenuGetSelectMenuId, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(239); const uint32_t a2_r = typed::SpriteMenuGetSelectMenuId(rdram, runtime, a2_4, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuGetSelectMenuId_0x1112ea0(rdram, ctx, runtime);
}
// SpriteMenuSetSelectMenuMode (0x11130d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetSelectMenuMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11130D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(SpriteMenuSetSelectMenuMode, a2_4, a2_5); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(240); const uint32_t a2_r = typed::SpriteMenuSetSelectMenuMode(rdram, runtime, a2_4, a2_5, a4_h4, a4_h5, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetSelectMenuMode_0x11130d8(rdram, ctx, runtime);
}
// SpriteMenuSetSliderValueByHash (0x1113260): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetSliderValueByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1113260u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(SpriteMenuSetSliderValueByHash, a2_4, a2_5, a2_6, a2_7); A6_IN(241); const uint64_t a2_r = typed::SpriteMenuSetSliderValueByHash(rdram, runtime, a2_4, a2_5, a2_6, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetSliderValueByHash_0x1113260(rdram, ctx, runtime);
}
// SpriteMenuSetCheckBoxByPartsId (0x1113448): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetCheckBoxByPartsId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1113448u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(SpriteMenuSetCheckBoxByPartsId, a2_4, a2_5, a2_6, a2_7); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(242); const uint32_t a2_r = typed::SpriteMenuSetCheckBoxByPartsId(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetCheckBoxByPartsId_0x1113448(rdram, ctx, runtime);
}
// SpriteMenuSetCheckBoxByHash (0x11134b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetCheckBoxByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11134B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(SpriteMenuSetCheckBoxByHash, a2_4, a2_5, a2_6, a2_7); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(243); const uint64_t a2_r = typed::SpriteMenuSetCheckBoxByHash(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetCheckBoxByHash_0x11134b0(rdram, ctx, runtime);
}
// spriteMenuCheckSelectPushPad (0x1113b00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void spriteMenuCheckSelectPushPad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1113B00u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(spriteMenuCheckSelectPushPad); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(244); const uint32_t a2_r = typed::spriteMenuCheckSelectPushPad(rdram, runtime, a4_h3, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::spriteMenuCheckSelectPushPad_0x1113b00(rdram, ctx, runtime);
}
// FlushSpriteObj (0x1114190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void FlushSpriteObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1114190u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(FlushSpriteObj, a2_4); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(245); const uint32_t a2_r = typed::FlushSpriteObj(rdram, runtime, a2_4, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::FlushSpriteObj_0x1114190(rdram, ctx, runtime);
}
// GetSpriteObjWorkSize (0x1114258): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetSpriteObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1114258u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetSpriteObjWorkSize, a2_4); A6_IN(246); const uint32_t a2_r = typed::GetSpriteObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetSpriteObjWorkSize_0x1114258(rdram, ctx, runtime);
}
// ExecScriptSpriteObj (0x11143a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ExecScriptSpriteObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11143A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(ExecScriptSpriteObj); A6_IN(247); typed::ExecScriptSpriteObj(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ExecScriptSpriteObj_0x11143a0(rdram, ctx, runtime);
}
// getMicroCodeAddress (0x1114b38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMicroCodeAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1114B38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getMicroCodeAddress, a2_4); A6_IN(248); const uint32_t a2_r = typed::getMicroCodeAddress(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMicroCodeAddress_0x1114b38(rdram, ctx, runtime);
}
// resetMicroCodeManager (0x1114c30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void resetMicroCodeManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1114C30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(resetMicroCodeManager); A6_IN(249); const uint32_t a2_r = typed::resetMicroCodeManager(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::resetMicroCodeManager_0x1114c30(rdram, ctx, runtime);
}
// ResetCollisionDebug (0x11240a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ResetCollisionDebug(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11240A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(ResetCollisionDebug); A6_IN(250); const uint32_t a2_r = typed::ResetCollisionDebug(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ResetCollisionDebug_0x11240a0(rdram, ctx, runtime);
}
// CopyCollisionEdgePointer (0x11280c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CopyCollisionEdgePointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11280C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(CopyCollisionEdgePointer, a2_4, a2_5); A6_IN(251); const uint32_t a2_r = typed::CopyCollisionEdgePointer(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CopyCollisionEdgePointer_0x11280c0(rdram, ctx, runtime);
}
// getVolumeCollisionDebugMode (0x1128cb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getVolumeCollisionDebugMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128CB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getVolumeCollisionDebugMode); A6_IN(252); const uint32_t a2_r = typed::getVolumeCollisionDebugMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getVolumeCollisionDebugMode_0x1128cb0(rdram, ctx, runtime);
}
// getCollisionDebugGraphMode (0x1128cc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionDebugGraphMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128CC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionDebugGraphMode); A6_IN(253); const uint32_t a2_r = typed::getCollisionDebugGraphMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionDebugGraphMode_0x1128cc0(rdram, ctx, runtime);
}
// getCollisionVirtualWallDispFlag (0x1128cd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionVirtualWallDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128CD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionVirtualWallDispFlag); A6_IN(254); const uint32_t a2_r = typed::getCollisionVirtualWallDispFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionVirtualWallDispFlag_0x1128cd0(rdram, ctx, runtime);
}
// GetColMultiDepVtxInfoVertexNormal (0x1129068): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetColMultiDepVtxInfoVertexNormal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129068u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(GetColMultiDepVtxInfoVertexNormal, a2_4, a2_5); A6_IN(255); const uint32_t a2_r = typed::GetColMultiDepVtxInfoVertexNormal(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetColMultiDepVtxInfoVertexNormal_0x1129068(rdram, ctx, runtime);
}
// GetColMultiDepVtxInfoVertexNormalWork (0x11290b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetColMultiDepVtxInfoVertexNormalWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11290B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(GetColMultiDepVtxInfoVertexNormalWork, a2_4, a2_5, a2_6); A6_IN(256); const uint32_t a2_r = typed::GetColMultiDepVtxInfoVertexNormalWork(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetColMultiDepVtxInfoVertexNormalWork_0x11290b8(rdram, ctx, runtime);
}
// iasmCalcCollisionMultiDepSurfBoundarySub (0x1129108): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iasmCalcCollisionMultiDepSurfBoundarySub(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129108u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const float a2_f12 = ctx->f[12]; A6_CHK(iasmCalcCollisionMultiDepSurfBoundarySub, a2_4, a2_5, a2_6, a2_7, a2_f12); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; uint64_t a4_h8 = a4::hi64(ctx->r[8]); A6_IN(257); typed::iasmCalcCollisionMultiDepSurfBoundarySub(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_h8); A6_OUT(); ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iasmCalcCollisionMultiDepSurfBoundarySub_0x1129108(rdram, ctx, runtime);
}
// SetColDebug (0x1129248): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetColDebug(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129248u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetColDebug, a2_4); A6_IN(258); const uint32_t a2_r = typed::SetColDebug(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetColDebug_0x1129248(rdram, ctx, runtime);
}
// manager_sub_01129400 (0x1129400): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01129400(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129400u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(manager_sub_01129400, a2_4, a2_5); A6_IN(259); const uint64_t a2_r = typed::manager_sub_01129400(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01129400_0x1129400(rdram, ctx, runtime);
}
// MakeGeometryPointerGlobalInfo (0x1129628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MakeGeometryPointerGlobalInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(MakeGeometryPointerGlobalInfo, a2_4); A6_IN(260); typed::MakeGeometryPointerGlobalInfo(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::MakeGeometryPointerGlobalInfo_0x1129628(rdram, ctx, runtime);
}
// MakeCollisionPointGlobalInfo (0x1129898): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MakeCollisionPointGlobalInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129898u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(MakeCollisionPointGlobalInfo, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(261); const uint64_t a2_r = typed::MakeCollisionPointGlobalInfo(rdram, runtime, a2_4, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::MakeCollisionPointGlobalInfo_0x1129898(rdram, ctx, runtime);
}
// objfilter (0x1129af0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void objfilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1129AF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(objfilter, a2_4, a2_5); A6_IN(262); const uint64_t a2_r = typed::objfilter(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::objfilter_0x1129af0(rdram, ctx, runtime);
}
// DebugSetCollisionWideAreaSearchGroup (0x112a4e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugSetCollisionWideAreaSearchGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x112A4E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(DebugSetCollisionWideAreaSearchGroup, a2_4); A6_IN(263); const uint32_t a2_r = typed::DebugSetCollisionWideAreaSearchGroup(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DebugSetCollisionWideAreaSearchGroup_0x112a4e0(rdram, ctx, runtime);
}
// InitCollisionWideAreaSearchInfo (0x112a4f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void InitCollisionWideAreaSearchInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x112A4F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(InitCollisionWideAreaSearchInfo, a2_4); A6_IN(264); const uint32_t a2_r = typed::InitCollisionWideAreaSearchInfo(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::InitCollisionWideAreaSearchInfo_0x112a4f0(rdram, ctx, runtime);
}
// CancelRequestCollisionWideAreaSearch (0x112a628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CancelRequestCollisionWideAreaSearch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x112A628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(CancelRequestCollisionWideAreaSearch); A6_IN(265); typed::CancelRequestCollisionWideAreaSearch(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CancelRequestCollisionWideAreaSearch_0x112a628(rdram, ctx, runtime);
}
// manager_sub_01131D70 (0x1131d70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01131D70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1131D70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(manager_sub_01131D70, a2_4, a2_5); A6_IN(266); const uint64_t a2_r = typed::manager_sub_01131D70(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01131D70_0x1131d70(rdram, ctx, runtime);
}
// SetGeometryPointerGlobalStatePosPointer (0x1132998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetGeometryPointerGlobalStatePosPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1132998u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); A6_CHK(SetGeometryPointerGlobalStatePosPointer, a2_4, a2_5, a2_6, a2_7, a2_8); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(267); const uint64_t a2_r = typed::SetGeometryPointerGlobalStatePosPointer(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetGeometryPointerGlobalStatePosPointer_0x1132998(rdram, ctx, runtime);
}
// GetPlaneGlobalYFittingPoint (0x11340f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetPlaneGlobalYFittingPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11340F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(GetPlaneGlobalYFittingPoint, a2_4, a2_5, a2_6); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(268); const a1::RetVF a2_r = typed::GetPlaneGlobalYFittingPoint(rdram, runtime, a2_4, a2_5, a2_6, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetPlaneGlobalYFittingPoint_0x11340f8(rdram, ctx, runtime);
}
// GetYDifferenceOfPlaneXZProjection (0x1134348): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetYDifferenceOfPlaneXZProjection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1134348u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(GetYDifferenceOfPlaneXZProjection, a2_4, a2_5); A6_IN(269); const float a2_r = typed::GetYDifferenceOfPlaneXZProjection(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetYDifferenceOfPlaneXZProjection_0x1134348(rdram, ctx, runtime);
}
// IsOppositePlaneLine (0x11349e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void IsOppositePlaneLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11349E8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(IsOppositePlaneLine, a2_4, a2_5, a2_6); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(270); const a1::RetVF a2_r = typed::IsOppositePlaneLine(rdram, runtime, a2_4, a2_5, a2_6, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::IsOppositePlaneLine_0x11349e8(rdram, ctx, runtime);
}
// GetPlaneFrom3Position (0x1134a40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetPlaneFrom3Position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1134A40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GetPlaneFrom3Position, a2_4, a2_5, a2_6, a2_7, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(271); const a1::RetVF a2_r = typed::GetPlaneFrom3Position(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetPlaneFrom3Position_0x1134a40(rdram, ctx, runtime);
}
// GetCrossPositionOf3Plane (0x1134ab8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCrossPositionOf3Plane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1134AB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(GetCrossPositionOf3Plane, a2_4, a2_5, a2_6, a2_7); A6_IN(272); const float a2_r = typed::GetCrossPositionOf3Plane(rdram, runtime, a2_4, a2_5, a2_6, a2_7); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCrossPositionOf3Plane_0x1134ab8(rdram, ctx, runtime);
}
// CopyMultiQuatPos (0x11351b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CopyMultiQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11351B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(CopyMultiQuatPos, a2_4, a2_5, a2_6); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(273); typed::CopyMultiQuatPos(rdram, runtime, a2_4, a2_5, a2_6, a4_h2, a4_h3); A6_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CopyMultiQuatPos_0x11351b0(rdram, ctx, runtime);
}
// CopyQuatPos (0x1135b20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CopyQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1135B20u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CopyQuatPos, a2_4); A6_IN(274); const uint64_t a2_r = typed::CopyQuatPos(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CopyQuatPos_0x1135b20(rdram, ctx, runtime);
}
// CheckEllipticalSphereVolumeWithInverseMatrix (0x1136380): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CheckEllipticalSphereVolumeWithInverseMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1136380u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(CheckEllipticalSphereVolumeWithInverseMatrix, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(275); const a1::RetVF a2_r = typed::CheckEllipticalSphereVolumeWithInverseMatrix(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CheckEllipticalSphereVolumeWithInverseMatrix_0x1136380(rdram, ctx, runtime);
}
// CheckCylinderVolumeWithInverseMatrix (0x1136be0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CheckCylinderVolumeWithInverseMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1136BE0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(CheckCylinderVolumeWithInverseMatrix, a2_4, a2_5, a2_6); a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(276); const a1::RetVF a2_r = typed::CheckCylinderVolumeWithInverseMatrix(rdram, runtime, a2_4, a2_5, a2_6, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CheckCylinderVolumeWithInverseMatrix_0x1136be0(rdram, ctx, runtime);
}
// manager_sub_0113B408 (0x113b408): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0113B408(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113B408u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(manager_sub_0113B408, a2_4, a2_5, a2_6, a2_7, a2_8, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a3_xo[5] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(277); const uint64_t a2_r = typed::manager_sub_0113B408(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h8, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); Ps2SetGpr64(ctx, 9, a3_xo[2]); Ps2SetGpr64(ctx, 10, a3_xo[3]); Ps2SetGpr64(ctx, 11, a3_xo[4]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_0113B408_0x113b408(rdram, ctx, runtime);
}
// get_particle_name_from_effect_id (0x113fa28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_particle_name_from_effect_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FA28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(get_particle_name_from_effect_id, a2_4); A6_IN(278); const uint64_t a2_r = typed::get_particle_name_from_effect_id(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_particle_name_from_effect_id_0x113fa28(rdram, ctx, runtime);
}
// get_particle_effect_from_lod (0x113faa0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FAA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_particle_effect_from_lod, a2_4, a2_5); A6_IN(279); const uint32_t a2_r = typed::get_particle_effect_from_lod(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_particle_effect_from_lod_0x113faa0(rdram, ctx, runtime);
}
// quit_particle_texture_user_by_id (0x113fad8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void quit_particle_texture_user_by_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FAD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(quit_particle_texture_user_by_id, a2_4); A6_IN(280); const uint32_t a2_r = typed::quit_particle_texture_user_by_id(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::quit_particle_texture_user_by_id_0x113fad8(rdram, ctx, runtime);
}
// getPtclDisp (0x113fb88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPtclDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FB88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPtclDisp); A6_IN(281); const uint32_t a2_r = typed::getPtclDisp(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPtclDisp_0x113fb88(rdram, ctx, runtime);
}
// get_particle_printf_disp_level (0x113fb98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_particle_printf_disp_level(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FB98u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_particle_printf_disp_level); A6_IN(282); const uint32_t a2_r = typed::get_particle_printf_disp_level(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_particle_printf_disp_level_0x113fb98(rdram, ctx, runtime);
}
// get_particle_camera_data (0x113fba8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_particle_camera_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FBA8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_particle_camera_data); A6_IN(283); const uint32_t a2_r = typed::get_particle_camera_data(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_particle_camera_data_0x113fba8(rdram, ctx, runtime);
}
// is_proper_particle_texture_id (0x113fbb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_proper_particle_texture_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FBB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(is_proper_particle_texture_id, a2_4); A6_IN(284); const uint32_t a2_r = typed::is_proper_particle_texture_id(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_proper_particle_texture_id_0x113fbb8(rdram, ctx, runtime);
}
// particleGWorkPut (0x113ff90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void particleGWorkPut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x113FF90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(particleGWorkPut, a2_4, a2_5); A6_IN(285); const uint64_t a2_r = typed::particleGWorkPut(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::particleGWorkPut_0x113ff90(rdram, ctx, runtime);
}
// add_particle_texture_user (0x1140228): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void add_particle_texture_user(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1140228u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(add_particle_texture_user, a2_4); A6_IN(286); const uint32_t a2_r = typed::add_particle_texture_user(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::add_particle_texture_user_0x1140228(rdram, ctx, runtime);
}
// set_act_particle_lod_select_flag (0x11403f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_act_particle_lod_select_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11403F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(set_act_particle_lod_select_flag, a2_4); A6_IN(287); const uint32_t a2_r = typed::set_act_particle_lod_select_flag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_act_particle_lod_select_flag_0x11403f8(rdram, ctx, runtime);
}
// set_disp_particle_collisin_debug_flag (0x1140408): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_disp_particle_collisin_debug_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1140408u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(set_disp_particle_collisin_debug_flag, a2_4); A6_IN(288); const uint32_t a2_r = typed::set_disp_particle_collisin_debug_flag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_disp_particle_collisin_debug_flag_0x1140408(rdram, ctx, runtime);
}
// init_texture_animation_parameter (0x11404c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void init_texture_animation_parameter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11404C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(init_texture_animation_parameter, a2_4); A6_IN(289); const a1::RetVF a2_r = typed::init_texture_animation_parameter(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::init_texture_animation_parameter_0x11404c0(rdram, ctx, runtime);
}
// get_particle_geodispobj_group (0x1140548): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_particle_geodispobj_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1140548u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_particle_geodispobj_group); A6_IN(290); const uint32_t a2_r = typed::get_particle_geodispobj_group(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_particle_geodispobj_group_0x1140548(rdram, ctx, runtime);
}
// get_set_limit_gl_draw_particle_flag (0x1140558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_set_limit_gl_draw_particle_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1140558u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_set_limit_gl_draw_particle_flag); A6_IN(291); const uint32_t a2_r = typed::get_set_limit_gl_draw_particle_flag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_set_limit_gl_draw_particle_flag_0x1140558(rdram, ctx, runtime);
}
// create_particle_texture (0x1140568): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void create_particle_texture(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1140568u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(create_particle_texture, a2_4); A6_IN(292); const uint32_t a2_r = typed::create_particle_texture(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::create_particle_texture_0x1140568(rdram, ctx, runtime);
}
// manager_sub_011410D0 (0x11410d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_011410D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11410D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(manager_sub_011410D0, a2_4, a2_5); A6_IN(293); const float a2_r = typed::manager_sub_011410D0(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_011410D0_0x11410d0(rdram, ctx, runtime);
}
// manager_sub_01145C70 (0x1145c70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01145C70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1145C70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(manager_sub_01145C70, a2_5, a2_7); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A6_IN(294); const uint32_t a2_r = typed::manager_sub_01145C70(rdram, runtime, a2_5, a2_7, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01145C70_0x1145c70(rdram, ctx, runtime);
}
// manager_sub_01145CD0 (0x1145cd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01145CD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1145CD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const float a2_f12 = ctx->f[12]; A6_CHK(manager_sub_01145CD0, a2_4, a2_5, a2_f12); A6_IN(295); const float a2_r = typed::manager_sub_01145CD0(rdram, runtime, a2_4, a2_5, a2_f12); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01145CD0_0x1145cd0(rdram, ctx, runtime);
}
// dynamic_cast_to_wind_emitter_sphere (0x11461c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dynamic_cast_to_wind_emitter_sphere(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11461C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(dynamic_cast_to_wind_emitter_sphere, a2_4); A6_IN(296); const uint64_t a2_r = typed::dynamic_cast_to_wind_emitter_sphere(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dynamic_cast_to_wind_emitter_sphere_0x11461c0(rdram, ctx, runtime);
}
// dynamic_cast_to_wind_emitter_box (0x11461e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dynamic_cast_to_wind_emitter_box(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11461E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(dynamic_cast_to_wind_emitter_box, a2_4); A6_IN(297); const uint64_t a2_r = typed::dynamic_cast_to_wind_emitter_box(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dynamic_cast_to_wind_emitter_box_0x11461e0(rdram, ctx, runtime);
}
// dynamic_cast_to_wind_emitter_judge_volume (0x1146208): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dynamic_cast_to_wind_emitter_judge_volume(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1146208u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(dynamic_cast_to_wind_emitter_judge_volume, a2_4); A6_IN(298); const uint64_t a2_r = typed::dynamic_cast_to_wind_emitter_judge_volume(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dynamic_cast_to_wind_emitter_judge_volume_0x1146208(rdram, ctx, runtime);
}
// constructor_wind_effect_data (0x1146c08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void constructor_wind_effect_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1146C08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(constructor_wind_effect_data, a2_4); A6_IN(299); const uint32_t a2_r = typed::constructor_wind_effect_data(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::constructor_wind_effect_data_0x1146c08(rdram, ctx, runtime);
}
// is_ambient_directional_wind_emitter (0x1146c30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_ambient_directional_wind_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1146C30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_ambient_directional_wind_emitter, a2_4); A6_IN(300); const uint32_t a2_r = typed::is_ambient_directional_wind_emitter(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_ambient_directional_wind_emitter_0x1146c30(rdram, ctx, runtime);
}
// get_wind_emitter_direct (0x1146c40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_wind_emitter_direct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1146C40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_wind_emitter_direct, a2_4); A6_IN(301); const uint32_t a2_r = typed::get_wind_emitter_direct(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_wind_emitter_direct_0x1146c40(rdram, ctx, runtime);
}
// get_wind_effect_data_direct (0x1146c58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_wind_effect_data_direct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1146C58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_wind_effect_data_direct, a2_4); A6_IN(302); const uint32_t a2_r = typed::get_wind_effect_data_direct(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_wind_effect_data_direct_0x1146c58(rdram, ctx, runtime);
}
// DeleteModifiedManager (0x1147a20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DeleteModifiedManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1147A20u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(DeleteModifiedManager); A6_IN(303); typed::DeleteModifiedManager(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DeleteModifiedManager_0x1147a20(rdram, ctx, runtime);
}
// mcSetGameClearFlag (0x114a310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void mcSetGameClearFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114A310u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(mcSetGameClearFlag); A6_IN(304); const uint32_t a2_r = typed::mcSetGameClearFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::mcSetGameClearFlag_0x114a310(rdram, ctx, runtime);
}
// mcSetCompleteTimeAttack (0x114a340): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void mcSetCompleteTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114A340u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(mcSetCompleteTimeAttack, a2_4, a2_5); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 2)}; A6_IN(305); typed::mcSetCompleteTimeAttack(rdram, runtime, a2_4, a2_5, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::mcSetCompleteTimeAttack_0x114a340(rdram, ctx, runtime);
}
// mcReqSync (0x114a5d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void mcReqSync(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114A5D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(mcReqSync, a2_4); A6_IN(306); const uint32_t a2_r = typed::mcReqSync(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::mcReqSync_0x114a5d8(rdram, ctx, runtime);
}
// mouseClearStatus (0x114ad50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void mouseClearStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114AD50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(mouseClearStatus, a2_4); A6_IN(307); const float a2_r = typed::mouseClearStatus(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::mouseClearStatus_0x114ad50(rdram, ctx, runtime);
}
// usbdevKeybdGetStatus (0x114b6f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void usbdevKeybdGetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114B6F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(usbdevKeybdGetStatus, a2_4, a2_5, a2_6); A6_IN(308); const uint64_t a2_r = typed::usbdevKeybdGetStatus(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::usbdevKeybdGetStatus_0x114b6f0(rdram, ctx, runtime);
}
// cursorLockParamMenu (0x114f298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void cursorLockParamMenu(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114F298u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(cursorLockParamMenu, a2_4, a2_5); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 2)}; A6_IN(309); typed::cursorLockParamMenu(rdram, runtime, a2_4, a2_5, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::cursorLockParamMenu_0x114f298(rdram, ctx, runtime);
}
// manager_sub_0114F990 (0x114f990): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0114F990(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114F990u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_0114F990); A6_IN(310); typed::manager_sub_0114F990(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_0114F990_0x114f990(rdram, ctx, runtime);
}
// manager_sub_0114FB10 (0x114fb10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0114FB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114FB10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(manager_sub_0114FB10, a2_5); A6_IN(311); const uint32_t a2_r = typed::manager_sub_0114FB10(rdram, runtime, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_0114FB10_0x114fb10(rdram, ctx, runtime);
}
// manager_sub_01150C78 (0x1150c78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01150C78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1150C78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01150C78); uint64_t a3_xo[3] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(312); const uint64_t a2_r = typed::manager_sub_01150C78(rdram, runtime, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01150C78_0x1150c78(rdram, ctx, runtime);
}
// manager_sub_01152D80 (0x1152d80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01152D80(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1152D80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; A6_CHK(manager_sub_01152D80, a2_4, a2_29, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29); uint64_t a3_xo[19] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[14]), (uint64_t)a1_bits(ctx->f[15]), (uint64_t)a1_bits(ctx->f[16]), (uint64_t)a1_bits(ctx->f[17]), (uint64_t)a1_bits(ctx->f[18]), (uint64_t)a1_bits(ctx->f[19]), (uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23]), (uint64_t)a1_bits(ctx->f[24]), (uint64_t)a1_bits(ctx->f[25]), (uint64_t)a1_bits(ctx->f[26]), (uint64_t)a1_bits(ctx->f[27]), (uint64_t)a1_bits(ctx->f[28]), (uint64_t)a1_bits(ctx->f[29]), Ps2GprU64(ctx, 4)}; A6_IN(313); const a1::RetVF a2_r = typed::manager_sub_01152D80(rdram, runtime, a2_4, a2_29, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->f[14] = a1_flt((uint32_t)a3_xo[2]); ctx->f[15] = a1_flt((uint32_t)a3_xo[3]); ctx->f[16] = a1_flt((uint32_t)a3_xo[4]); ctx->f[17] = a1_flt((uint32_t)a3_xo[5]); ctx->f[18] = a1_flt((uint32_t)a3_xo[6]); ctx->f[19] = a1_flt((uint32_t)a3_xo[7]); ctx->f[20] = a1_flt((uint32_t)a3_xo[8]); ctx->f[21] = a1_flt((uint32_t)a3_xo[9]); ctx->f[22] = a1_flt((uint32_t)a3_xo[10]); ctx->f[23] = a1_flt((uint32_t)a3_xo[11]); ctx->f[24] = a1_flt((uint32_t)a3_xo[12]); ctx->f[25] = a1_flt((uint32_t)a3_xo[13]); ctx->f[26] = a1_flt((uint32_t)a3_xo[14]); ctx->f[27] = a1_flt((uint32_t)a3_xo[15]); ctx->f[28] = a1_flt((uint32_t)a3_xo[16]); ctx->f[29] = a1_flt((uint32_t)a3_xo[17]); Ps2SetGpr64(ctx, 4, a3_xo[18]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01152D80_0x1152d80(rdram, ctx, runtime);
}
// manager_sub_01154080 (0x1154080): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01154080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1154080u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(manager_sub_01154080, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h12 = a4::hi64(ctx->r[12]); uint64_t a3_xo[8] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(314); const uint64_t a2_r = typed::manager_sub_01154080(rdram, runtime, a2_4, a4_h2, a4_h3, a4_h4, a4_h5, a4_h6, a4_h12, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); Ps2SetGpr64(ctx, 9, a3_xo[5]); Ps2SetGpr64(ctx, 10, a3_xo[6]); Ps2SetGpr64(ctx, 11, a3_xo[7]); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[12] = a4::mk128(a4_h12, Ps2GprU64(ctx, 12)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01154080_0x1154080(rdram, ctx, runtime);
}
// windowDisplayMode (0x1155b60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowDisplayMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155B60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(windowDisplayMode); A6_IN(315); const uint32_t a2_r = typed::windowDisplayMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowDisplayMode_0x1155b60(rdram, ctx, runtime);
}
// windowFontColor (0x1155b70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowFontColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155B70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(windowFontColor, a2_4, a2_5); A6_IN(316); const uint64_t a2_r = typed::windowFontColor(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowFontColor_0x1155b70(rdram, ctx, runtime);
}
// windowCheckCursorInRect (0x1155ed8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowCheckCursorInRect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155ED8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(windowCheckCursorInRect, a2_4, a2_5, a2_6); A6_IN(317); const uint64_t a2_r = typed::windowCheckCursorInRect(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowCheckCursorInRect_0x1155ed8(rdram, ctx, runtime);
}
// windowGetCursorPos (0x1155fa0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowGetCursorPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155FA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(windowGetCursorPos, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(318); const uint32_t a2_r = typed::windowGetCursorPos(rdram, runtime, a2_4, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowGetCursorPos_0x1155fa0(rdram, ctx, runtime);
}
// windowGetCursorPosOnWindow (0x1156018): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowGetCursorPosOnWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1156018u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(windowGetCursorPosOnWindow, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A6_IN(319); const a1::RetVF a2_r = typed::windowGetCursorPosOnWindow(rdram, runtime, a2_4, a2_5, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowGetCursorPosOnWindow_0x1156018(rdram, ctx, runtime);
}
// windowGetMouseStatus (0x11560e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowGetMouseStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11560E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(windowGetMouseStatus); A6_IN(320); const uint32_t a2_r = typed::windowGetMouseStatus(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowGetMouseStatus_0x11560e8(rdram, ctx, runtime);
}
// windowGetTargetWindowHandle (0x1156108): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowGetTargetWindowHandle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1156108u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(windowGetTargetWindowHandle); A6_IN(321); const uint64_t a2_r = typed::windowGetTargetWindowHandle(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowGetTargetWindowHandle_0x1156108(rdram, ctx, runtime);
}
// windowCloseRequest (0x11561b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowCloseRequest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11561B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(windowCloseRequest, a2_4); A6_IN(322); const uint32_t a2_r = typed::windowCloseRequest(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowCloseRequest_0x11561b0(rdram, ctx, runtime);
}
// GetWindowClass (0x1156628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetWindowClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1156628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetWindowClass, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(323); const uint64_t a2_r = typed::GetWindowClass(rdram, runtime, a2_4, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetWindowClass_0x1156628(rdram, ctx, runtime);
}
// manager_sub_01157CC0 (0x1157cc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157CC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157CC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01157CC0); A6_IN(324); const uint32_t a2_r = typed::manager_sub_01157CC0(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157CC0_0x1157cc0(rdram, ctx, runtime);
}
// manager_sub_01157CD0 (0x1157cd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157CD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157CD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01157CD0); A6_IN(325); const uint32_t a2_r = typed::manager_sub_01157CD0(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157CD0_0x1157cd0(rdram, ctx, runtime);
}
// manager_sub_01157CE0 (0x1157ce0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157CE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157CE0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01157CE0); A6_IN(326); const uint32_t a2_r = typed::manager_sub_01157CE0(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157CE0_0x1157ce0(rdram, ctx, runtime);
}
// manager_sub_01157CF0 (0x1157cf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157CF0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157CF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01157CF0); A6_IN(327); const uint32_t a2_r = typed::manager_sub_01157CF0(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157CF0_0x1157cf0(rdram, ctx, runtime);
}
// manager_sub_01157E90 (0x1157e90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157E90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157E90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01157E90); A6_IN(328); const uint64_t a2_r = typed::manager_sub_01157E90(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157E90_0x1157e90(rdram, ctx, runtime);
}
// manager_sub_01157F38 (0x1157f38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01157F38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1157F38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(manager_sub_01157F38, a2_7); A6_IN(329); const uint32_t a2_r = typed::manager_sub_01157F38(rdram, runtime, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01157F38_0x1157f38(rdram, ctx, runtime);
}
// seSetDebugTrigFunc (0x1158848): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void seSetDebugTrigFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1158848u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(seSetDebugTrigFunc, a2_4); A6_IN(330); const uint32_t a2_r = typed::seSetDebugTrigFunc(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::seSetDebugTrigFunc_0x1158848(rdram, ctx, runtime);
}
// CreateEndAutoModFile (0x115a0b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CreateEndAutoModFile(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115A0B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(CreateEndAutoModFile); A6_IN(331); typed::CreateEndAutoModFile(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CreateEndAutoModFile_0x115a0b8(rdram, ctx, runtime);
}
// sheetIdToName (0x115ec28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sheetIdToName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115EC28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(sheetIdToName, a2_4); A6_IN(332); const uint32_t a2_r = typed::sheetIdToName(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sheetIdToName_0x115ec28(rdram, ctx, runtime);
}
// getSheetDebugNoLoadMode (0x115fe30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSheetDebugNoLoadMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115FE30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSheetDebugNoLoadMode); A6_IN(333); const uint32_t a2_r = typed::getSheetDebugNoLoadMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSheetDebugNoLoadMode_0x115fe30(rdram, ctx, runtime);
}
// getDebPcWinSw (0x1160160): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebPcWinSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160160u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebPcWinSw); A6_IN(334); const uint32_t a2_r = typed::getDebPcWinSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebPcWinSw_0x1160160(rdram, ctx, runtime);
}
// GetDebPcWinSw (0x1160170): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebPcWinSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160170u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebPcWinSw); A6_IN(335); const uint32_t a2_r = typed::GetDebPcWinSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebPcWinSw_0x1160170(rdram, ctx, runtime);
}
// getDebPcWinType (0x1160180): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebPcWinType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160180u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebPcWinType); A6_IN(336); const uint32_t a2_r = typed::getDebPcWinType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebPcWinType_0x1160180(rdram, ctx, runtime);
}
// GetDebPcWinType (0x1160190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebPcWinType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160190u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebPcWinType); A6_IN(337); const uint32_t a2_r = typed::GetDebPcWinType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebPcWinType_0x1160190(rdram, ctx, runtime);
}
// getDebPcWinCategory (0x11601a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebPcWinCategory(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11601A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebPcWinCategory); A6_IN(338); const uint32_t a2_r = typed::getDebPcWinCategory(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebPcWinCategory_0x11601a0(rdram, ctx, runtime);
}
// GetDebPcWinCategory (0x11601b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebPcWinCategory(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11601B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebPcWinCategory); A6_IN(339); const uint32_t a2_r = typed::GetDebPcWinCategory(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebPcWinCategory_0x11601b0(rdram, ctx, runtime);
}
// toggleDebPcWinWindow (0x11601c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void toggleDebPcWinWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11601C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(toggleDebPcWinWindow); A6_IN(340); typed::toggleDebPcWinWindow(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::toggleDebPcWinWindow_0x11601c0(rdram, ctx, runtime);
}
// SetDefaultViewContext (0x1160c58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetDefaultViewContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160C58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(SetDefaultViewContext, a2_4, a2_5, a2_6); A6_IN(341); const float a2_r = typed::SetDefaultViewContext(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetDefaultViewContext_0x1160c58(rdram, ctx, runtime);
}
// SetViewContext (0x1160cc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetViewContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160CC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; A6_CHK(SetViewContext, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16); A6_IN(342); const float a2_r = typed::SetViewContext(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetViewContext_0x1160cc8(rdram, ctx, runtime);
}
// GetScreenRadiusParticle (0x1160e08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetScreenRadiusParticle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1160E08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GetScreenRadiusParticle, a2_4, a2_29, a2_f12, a2_f13); a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(343); const a1::RetVF a2_r = typed::GetScreenRadiusParticle(rdram, runtime, a2_4, a2_29, a2_f12, a2_f13, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetScreenRadiusParticle_0x1160e08(rdram, ctx, runtime);
}
// is_camera_lws_animation_using (0x1161c88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_camera_lws_animation_using(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161C88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_camera_lws_animation_using, a2_4); A6_IN(344); const uint32_t a2_r = typed::is_camera_lws_animation_using(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_camera_lws_animation_using_0x1161c88(rdram, ctx, runtime);
}
// get_motion_data_camera_lws_animation (0x1161f50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_motion_data_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161F50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(get_motion_data_camera_lws_animation, a2_4); A6_IN(345); const uint64_t a2_r = typed::get_motion_data_camera_lws_animation(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_motion_data_camera_lws_animation_0x1161f50(rdram, ctx, runtime);
}
// get_animation_class_camera_lws_animation (0x1161f68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_animation_class_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161F68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_animation_class_camera_lws_animation, a2_4); A6_IN(346); const uint32_t a2_r = typed::get_animation_class_camera_lws_animation(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_animation_class_camera_lws_animation_0x1161f68(rdram, ctx, runtime);
}
// is_main_type_camera_lws_animation (0x1161f88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_main_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161F88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_main_type_camera_lws_animation, a2_4); A6_IN(347); const uint32_t a2_r = typed::is_main_type_camera_lws_animation(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_main_type_camera_lws_animation_0x1161f88(rdram, ctx, runtime);
}
// is_interest_type_camera_lws_animation (0x1161fb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_interest_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161FB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_interest_type_camera_lws_animation, a2_4); A6_IN(348); const uint32_t a2_r = typed::is_interest_type_camera_lws_animation(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_interest_type_camera_lws_animation_0x1161fb8(rdram, ctx, runtime);
}
// is_fixed_point_type_camera_lws_animation (0x1161fd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_fixed_point_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1161FD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_fixed_point_type_camera_lws_animation, a2_4); A6_IN(349); const uint32_t a2_r = typed::is_fixed_point_type_camera_lws_animation(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_fixed_point_type_camera_lws_animation_0x1161fd0(rdram, ctx, runtime);
}
// manager_sub_01162A38 (0x1162a38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01162A38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1162A38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(manager_sub_01162A38); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(350); const uint64_t a2_r = typed::manager_sub_01162A38(rdram, runtime, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01162A38_0x1162a38(rdram, ctx, runtime);
}
// constructor_camera_region (0x1163448): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void constructor_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1163448u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(constructor_camera_region, a2_4); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; A6_IN(352); const uint32_t a2_r = typed::constructor_camera_region(rdram, runtime, a2_4, a4_vf0); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::constructor_camera_region_0x1163448(rdram, ctx, runtime);
}
// get_max_object_animation_camera_region (0x1164180): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_max_object_animation_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1164180u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_max_object_animation_camera_region); A6_IN(353); const uint32_t a2_r = typed::get_max_object_animation_camera_region(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_max_object_animation_camera_region_0x1164180(rdram, ctx, runtime);
}
// get_cur_total_object_animation_camera_region (0x1164188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_total_object_animation_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1164188u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_cur_total_object_animation_camera_region); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(354); const uint64_t a2_r = typed::get_cur_total_object_animation_camera_region(rdram, runtime, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_total_object_animation_camera_region_0x1164188(rdram, ctx, runtime);
}
// get_col_attr_cam_region_select_flag (0x11641c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_col_attr_cam_region_select_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11641C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_col_attr_cam_region_select_flag); A6_IN(355); const uint32_t a2_r = typed::get_col_attr_cam_region_select_flag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_col_attr_cam_region_select_flag_0x11641c0(rdram, ctx, runtime);
}
// get_col_attr_cam_region_select_index (0x11641d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_col_attr_cam_region_select_index(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11641D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_col_attr_cam_region_select_index); A6_IN(356); const uint32_t a2_r = typed::get_col_attr_cam_region_select_index(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_col_attr_cam_region_select_index_0x11641d0(rdram, ctx, runtime);
}
// get_screen_size_scale_screen_matrix (0x1164848): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_screen_size_scale_screen_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1164848u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_screen_size_scale_screen_matrix); A6_IN(357); const uint32_t a2_r = typed::get_screen_size_scale_screen_matrix(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_screen_size_scale_screen_matrix_0x1164848(rdram, ctx, runtime);
}
// SetDamageOffsetStatus (0x1166e10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetDamageOffsetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1166E10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; A6_CHK(SetDamageOffsetStatus, a2_4, a2_5, a2_6, a2_f12); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(358); const uint64_t a2_r = typed::SetDamageOffsetStatus(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetDamageOffsetStatus_0x1166e10(rdram, ctx, runtime);
}
// SetDamageClassJudgeExternalFunc (0x1166e40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetDamageClassJudgeExternalFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1166E40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(SetDamageClassJudgeExternalFunc, a2_4, a2_5); A6_IN(359); const uint32_t a2_r = typed::SetDamageClassJudgeExternalFunc(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetDamageClassJudgeExternalFunc_0x1166e40(rdram, ctx, runtime);
}
// GetLetterKindVariableData (0x1167098): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLetterKindVariableData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1167098u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(GetLetterKindVariableData, a2_4); A6_IN(360); const uint64_t a2_r = typed::GetLetterKindVariableData(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLetterKindVariableData_0x1167098(rdram, ctx, runtime);
}
// getSETriggerTestMode (0x11694d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSETriggerTestMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11694D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSETriggerTestMode); A6_IN(361); const uint32_t a2_r = typed::getSETriggerTestMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSETriggerTestMode_0x11694d8(rdram, ctx, runtime);
}
// AnimeTableInfoDebugMode (0x11694e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AnimeTableInfoDebugMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11694E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(AnimeTableInfoDebugMode, a2_4); A6_IN(362); const uint32_t a2_r = typed::AnimeTableInfoDebugMode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AnimeTableInfoDebugMode_0x11694e8(rdram, ctx, runtime);
}
// getEditWorkDebugHeader (0x1169c50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getEditWorkDebugHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1169C50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getEditWorkDebugHeader, a2_4); A6_IN(363); const uint64_t a2_r = typed::getEditWorkDebugHeader(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getEditWorkDebugHeader_0x1169c50(rdram, ctx, runtime);
}
// getEditWorkDebugData (0x1169c70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getEditWorkDebugData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1169C70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getEditWorkDebugData, a2_4); A6_IN(364); const uint32_t a2_r = typed::getEditWorkDebugData(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getEditWorkDebugData_0x1169c70(rdram, ctx, runtime);
}
// DebugGetMutekiMode (0x1341310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DebugGetMutekiMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341310u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(DebugGetMutekiMode); A6_IN(365); const uint32_t a2_r = typed::DebugGetMutekiMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DebugGetMutekiMode_0x1341310(rdram, ctx, runtime);
}
// getForceMemoryAllocSize (0x1341650): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getForceMemoryAllocSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341650u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getForceMemoryAllocSize); A6_IN(366); const uint32_t a2_r = typed::getForceMemoryAllocSize(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getForceMemoryAllocSize_0x1341650(rdram, ctx, runtime);
}
// getStatusMode (0x13416f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStatusMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13416F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getStatusMode); A6_IN(367); const uint32_t a2_r = typed::getStatusMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStatusMode_0x13416f8(rdram, ctx, runtime);
}
// getDispCollisionPerfFlag (0x1341708): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDispCollisionPerfFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341708u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDispCollisionPerfFlag); A6_IN(368); const uint32_t a2_r = typed::getDispCollisionPerfFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDispCollisionPerfFlag_0x1341708(rdram, ctx, runtime);
}
// getPrintfPerfWarning (0x1341718): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPrintfPerfWarning(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341718u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPrintfPerfWarning); A6_IN(369); const uint32_t a2_r = typed::getPrintfPerfWarning(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPrintfPerfWarning_0x1341718(rdram, ctx, runtime);
}
// is_disp_debug_object_info (0x1341758): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_disp_debug_object_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341758u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(is_disp_debug_object_info); A6_IN(370); const uint32_t a2_r = typed::is_disp_debug_object_info(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_disp_debug_object_info_0x1341758(rdram, ctx, runtime);
}
// getCollisionDispMode (0x1341768): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionDispMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341768u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionDispMode); A6_IN(371); const uint32_t a2_r = typed::getCollisionDispMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionDispMode_0x1341768(rdram, ctx, runtime);
}
// getGetMutekiMode (0x1341778): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGetMutekiMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341778u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGetMutekiMode); A6_IN(372); const uint32_t a2_r = typed::getGetMutekiMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGetMutekiMode_0x1341778(rdram, ctx, runtime);
}
// getPlayerPowerDisp (0x1341788): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerPowerDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341788u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPlayerPowerDisp); A6_IN(373); const uint32_t a2_r = typed::getPlayerPowerDisp(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPlayerPowerDisp_0x1341788(rdram, ctx, runtime);
}
// getPlayerPowerMax (0x1341798): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerPowerMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341798u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPlayerPowerMax); A6_IN(374); const uint32_t a2_r = typed::getPlayerPowerMax(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPlayerPowerMax_0x1341798(rdram, ctx, runtime);
}
// getGripCalcCancelMode (0x13417a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGripCalcCancelMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13417A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGripCalcCancelMode); A6_IN(375); const uint32_t a2_r = typed::getGripCalcCancelMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGripCalcCancelMode_0x13417a8(rdram, ctx, runtime);
}
// getSnapShotSize (0x1341910): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSnapShotSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341910u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSnapShotSize); A6_IN(376); const uint32_t a2_r = typed::getSnapShotSize(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSnapShotSize_0x1341910(rdram, ctx, runtime);
}
// getSnapShotNum (0x1341930): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSnapShotNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341930u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSnapShotNum); A6_IN(377); const uint32_t a2_r = typed::getSnapShotNum(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSnapShotNum_0x1341930(rdram, ctx, runtime);
}
// getSnapShotRotation (0x1341940): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSnapShotRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341940u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSnapShotRotation); A6_IN(378); const uint32_t a2_r = typed::getSnapShotRotation(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSnapShotRotation_0x1341940(rdram, ctx, runtime);
}
// getSnapShotAspect (0x1341950): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSnapShotAspect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341950u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSnapShotAspect); A6_IN(379); const uint32_t a2_r = typed::getSnapShotAspect(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSnapShotAspect_0x1341950(rdram, ctx, runtime);
}
// getDebugSnapShotSizeX (0x1341960): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugSnapShotSizeX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341960u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugSnapShotSizeX); A6_IN(380); const uint32_t a2_r = typed::getDebugSnapShotSizeX(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugSnapShotSizeX_0x1341960(rdram, ctx, runtime);
}
// getFreezeBossFlag (0x1341a48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFreezeBossFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341A48u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFreezeBossFlag); A6_IN(381); const uint32_t a2_r = typed::getFreezeBossFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFreezeBossFlag_0x1341a48(rdram, ctx, runtime);
}
// getDispGpsMarker (0x1341c08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDispGpsMarker(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341C08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDispGpsMarker); A6_IN(382); const uint32_t a2_r = typed::getDispGpsMarker(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDispGpsMarker_0x1341c08(rdram, ctx, runtime);
}
// getGpsGridWidth (0x1341c18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGpsGridWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1341C18u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGpsGridWidth); A6_IN(383); const uint32_t a2_r = typed::getGpsGridWidth(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGpsGridWidth_0x1341c18(rdram, ctx, runtime);
}
// convert_from_button_value_to_bool (0x1342a40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void convert_from_button_value_to_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342A40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(convert_from_button_value_to_bool, a2_4); A6_IN(384); const uint64_t a2_r = typed::convert_from_button_value_to_bool(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::convert_from_button_value_to_bool_0x1342a40(rdram, ctx, runtime);
}
// GAMEUTIL_set_sight_on_reverse_cntrlLR (0x1342b20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_sight_on_reverse_cntrlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B20u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_sight_on_reverse_cntrlLR, a2_4); A6_IN(385); const uint32_t a2_r = typed::GAMEUTIL_set_sight_on_reverse_cntrlLR(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_sight_on_reverse_cntrlLR_0x1342b20(rdram, ctx, runtime);
}
// GAMEUTIL_get_sight_on_reverse_cntrlLR_int (0x1342b30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_sight_on_reverse_cntrlLR_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_sight_on_reverse_cntrlLR_int); A6_IN(386); const uint32_t a2_r = typed::GAMEUTIL_get_sight_on_reverse_cntrlLR_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_sight_on_reverse_cntrlLR_int_0x1342b30(rdram, ctx, runtime);
}
// GAMEUTIL_set_sight_on_reverse_cntrlUD (0x1342b50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_sight_on_reverse_cntrlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_sight_on_reverse_cntrlUD, a2_4); A6_IN(387); const uint32_t a2_r = typed::GAMEUTIL_set_sight_on_reverse_cntrlUD(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_sight_on_reverse_cntrlUD_0x1342b50(rdram, ctx, runtime);
}
// GAMEUTIL_get_sight_on_reverse_cntrlUD_int (0x1342b60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_sight_on_reverse_cntrlUD_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_sight_on_reverse_cntrlUD_int); A6_IN(388); const uint32_t a2_r = typed::GAMEUTIL_get_sight_on_reverse_cntrlUD_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_sight_on_reverse_cntrlUD_int_0x1342b60(rdram, ctx, runtime);
}
// GAMEUTIL_set_sight_on_angle_vel_ratio (0x1342b80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_sight_on_angle_vel_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_sight_on_angle_vel_ratio, a2_4); A6_IN(389); const uint32_t a2_r = typed::GAMEUTIL_set_sight_on_angle_vel_ratio(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_sight_on_angle_vel_ratio_0x1342b80(rdram, ctx, runtime);
}
// GAMEUTIL_get_sight_on_angle_vel_ratio_int (0x1342b90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_sight_on_angle_vel_ratio_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342B90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_sight_on_angle_vel_ratio_int); A6_IN(390); const uint32_t a2_r = typed::GAMEUTIL_get_sight_on_angle_vel_ratio_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_sight_on_angle_vel_ratio_int_0x1342b90(rdram, ctx, runtime);
}
// GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR (0x1342c50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342C50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR, a2_4); A6_IN(391); const uint32_t a2_r = typed::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR_0x1342c50(rdram, ctx, runtime);
}
// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int (0x1342c60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342C60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int); A6_IN(392); const uint32_t a2_r = typed::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int_0x1342c60(rdram, ctx, runtime);
}
// GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD (0x1342c80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342C80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD, a2_4); A6_IN(393); const uint32_t a2_r = typed::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD_0x1342c80(rdram, ctx, runtime);
}
// GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int (0x1342c90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342C90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int); A6_IN(394); const uint32_t a2_r = typed::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int_0x1342c90(rdram, ctx, runtime);
}
// GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio (0x1342cb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342CB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio, a2_4); A6_IN(395); const uint32_t a2_r = typed::GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio_0x1342cb0(rdram, ctx, runtime);
}
// GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int (0x1342cc0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342CC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int); A6_IN(396); const uint32_t a2_r = typed::GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int_0x1342cc0(rdram, ctx, runtime);
}
// GAMEUTIL_get_obj_from_stagelayout_sheet (0x1342e40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_obj_from_stagelayout_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342E40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_get_obj_from_stagelayout_sheet, a2_4); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A6_IN(397); const uint32_t a2_r = typed::GAMEUTIL_get_obj_from_stagelayout_sheet(rdram, runtime, a2_4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_obj_from_stagelayout_sheet_0x1342e40(rdram, ctx, runtime);
}
// GAMEUTIL_is_overtake_aim_position (0x1343060): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_overtake_aim_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1343060u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_overtake_aim_position, a2_4, a2_5, a2_6, a2_7, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(398); const a1::RetVF a2_r = typed::GAMEUTIL_is_overtake_aim_position(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_overtake_aim_position_0x1343060(rdram, ctx, runtime);
}
// GAMEUTIL_is_advance_near_dir_to_aim_position (0x13430e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_advance_near_dir_to_aim_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13430E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(GAMEUTIL_is_advance_near_dir_to_aim_position, a2_4, a2_5, a2_6, a2_7, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(399); const a1::RetVF a2_r = typed::GAMEUTIL_is_advance_near_dir_to_aim_position(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_advance_near_dir_to_aim_position_0x13430e0(rdram, ctx, runtime);
}
// GAMEUTIL_is_sign_change (0x1343388): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_sign_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1343388u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(GAMEUTIL_is_sign_change, a2_f12, a2_f13); A6_IN(400); const uint64_t a2_r = typed::GAMEUTIL_is_sign_change(rdram, runtime, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_sign_change_0x1343388(rdram, ctx, runtime);
}
// GAMEUTIL_constructor_record_info (0x13443b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_constructor_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13443B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_constructor_record_info, a2_4); A6_IN(401); typed::GAMEUTIL_constructor_record_info(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_constructor_record_info_0x13443b0(rdram, ctx, runtime);
}
// GAMEUTIL_reset_record_info (0x13444a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_reset_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13444A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_reset_record_info, a2_4); A6_IN(402); typed::GAMEUTIL_reset_record_info(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_reset_record_info_0x13444a8(rdram, ctx, runtime);
}
// GAMEUTIL_reset_status_continue_time_status_manager (0x1344620): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_reset_status_continue_time_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1344620u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GAMEUTIL_reset_status_continue_time_status_manager, a2_4); A6_IN(403); typed::GAMEUTIL_reset_status_continue_time_status_manager(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_reset_status_continue_time_status_manager_0x1344620(rdram, ctx, runtime);
}
// getSightOnAngleVelRatio (0x1344688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSightOnAngleVelRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1344688u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSightOnAngleVelRatio); A6_IN(404); const uint32_t a2_r = typed::getSightOnAngleVelRatio(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSightOnAngleVelRatio_0x1344688(rdram, ctx, runtime);
}
// getCullingScreenSize (0x1345190): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCullingScreenSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345190u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCullingScreenSize); A6_IN(405); const uint32_t a2_r = typed::getCullingScreenSize(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCullingScreenSize_0x1345190(rdram, ctx, runtime);
}
// getScissoringScreenSize (0x13451a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getScissoringScreenSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13451A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getScissoringScreenSize); A6_IN(406); const uint32_t a2_r = typed::getScissoringScreenSize(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getScissoringScreenSize_0x13451a0(rdram, ctx, runtime);
}
// getPlayerAliveInPauseFlag (0x13451b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerAliveInPauseFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13451B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPlayerAliveInPauseFlag); A6_IN(407); const uint32_t a2_r = typed::getPlayerAliveInPauseFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPlayerAliveInPauseFlag_0x13451b0(rdram, ctx, runtime);
}
// PauseAI (0x1345d50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void PauseAI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345D50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(PauseAI); A6_IN(408); const uint32_t a2_r = typed::PauseAI(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::PauseAI_0x1345d50(rdram, ctx, runtime);
}
// ReleaseAI (0x1345d60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ReleaseAI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345D60u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(ReleaseAI); A6_IN(409); const uint32_t a2_r = typed::ReleaseAI(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ReleaseAI_0x1345d60(rdram, ctx, runtime);
}
// SwitchPadAct (0x1345d70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SwitchPadAct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345D70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(SwitchPadAct); A6_IN(410); typed::SwitchPadAct(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SwitchPadAct_0x1345d70(rdram, ctx, runtime);
}
// getGameAnimeTimeScaleRatio (0x1345e68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345E68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGameAnimeTimeScaleRatio); A6_IN(411); const uint32_t a2_r = typed::getGameAnimeTimeScaleRatio(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGameAnimeTimeScaleRatio_0x1345e68(rdram, ctx, runtime);
}
// getDummyCount (0x1345ec0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDummyCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1345EC0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDummyCount); A6_IN(412); const uint64_t a2_r = typed::getDummyCount(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDummyCount_0x1345ec0(rdram, ctx, runtime);
}
// setLayerFade (0x13461c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setLayerFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13461C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setLayerFade, a2_4); A6_IN(413); const uint32_t a2_r = typed::setLayerFade(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setLayerFade_0x13461c0(rdram, ctx, runtime);
}
// setFadeValue (0x1346220): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setFadeValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1346220u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; A6_CHK(setFadeValue, a2_f12); A6_IN(414); const uint32_t a2_r = typed::setFadeValue(rdram, runtime, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setFadeValue_0x1346220(rdram, ctx, runtime);
}
// getDemoBarSw (0x1346230): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDemoBarSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1346230u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDemoBarSw); A6_IN(415); const uint32_t a2_r = typed::getDemoBarSw(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDemoBarSw_0x1346230(rdram, ctx, runtime);
}
// set_base_vel_nico_yaw_pitch_cntrl (0x1347368): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_base_vel_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347368u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(set_base_vel_nico_yaw_pitch_cntrl, a2_4, a2_f12, a2_f13); A6_IN(416); typed::set_base_vel_nico_yaw_pitch_cntrl(rdram, runtime, a2_4, a2_f12, a2_f13); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_base_vel_nico_yaw_pitch_cntrl_0x1347368(rdram, ctx, runtime);
}
// is_doing_nico_yaw_pitch_cntrl (0x1347590): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_doing_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347590u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_doing_nico_yaw_pitch_cntrl, a2_4); A6_IN(417); const uint32_t a2_r = typed::is_doing_nico_yaw_pitch_cntrl(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_doing_nico_yaw_pitch_cntrl_0x1347590(rdram, ctx, runtime);
}
// get_offset_yaw_nico_yaw_pitch_cntrl (0x1347828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_offset_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347828u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_offset_yaw_nico_yaw_pitch_cntrl, a2_4); A6_IN(418); const float a2_r = typed::get_offset_yaw_nico_yaw_pitch_cntrl(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_offset_yaw_nico_yaw_pitch_cntrl_0x1347828(rdram, ctx, runtime);
}
// get_offset_pitch_nico_yaw_pitch_cntrl (0x1347830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_offset_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347830u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_offset_pitch_nico_yaw_pitch_cntrl, a2_4); A6_IN(419); const float a2_r = typed::get_offset_pitch_nico_yaw_pitch_cntrl(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_offset_pitch_nico_yaw_pitch_cntrl_0x1347830(rdram, ctx, runtime);
}
// get_base_pitch_move_nico_yaw_pitch_cntrl (0x1347850): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_base_pitch_move_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1347850u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_base_pitch_move_nico_yaw_pitch_cntrl, a2_4); A6_IN(420); const float a2_r = typed::get_base_pitch_move_nico_yaw_pitch_cntrl(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_base_pitch_move_nico_yaw_pitch_cntrl_0x1347850(rdram, ctx, runtime);
}
// activedemoflowDemoEnd (0x13487b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void activedemoflowDemoEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13487B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(activedemoflowDemoEnd); A6_IN(421); const uint32_t a2_r = typed::activedemoflowDemoEnd(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::activedemoflowDemoEnd_0x13487b0(rdram, ctx, runtime);
}
// getActivedemoStage (0x13487c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getActivedemoStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13487C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getActivedemoStage); A6_IN(422); const uint32_t a2_r = typed::getActivedemoStage(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getActivedemoStage_0x13487c8(rdram, ctx, runtime);
}
// chkActiveDemoStart (0x13487d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkActiveDemoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13487D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(chkActiveDemoStart); A6_IN(423); const uint32_t a2_r = typed::chkActiveDemoStart(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::chkActiveDemoStart_0x13487d8(rdram, ctx, runtime);
}
// activeDemoCancel (0x1348958): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void activeDemoCancel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1348958u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(activeDemoCancel, a2_4); A6_IN(424); const uint32_t a2_r = typed::activeDemoCancel(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::activeDemoCancel_0x1348958(rdram, ctx, runtime);
}
// SkipCutDemoCameraOrient (0x134b250): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SkipCutDemoCameraOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134B250u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SkipCutDemoCameraOrient, a2_4); A6_IN(425); const uint32_t a2_r = typed::SkipCutDemoCameraOrient(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SkipCutDemoCameraOrient_0x134b250(rdram, ctx, runtime);
}
// SetCaptionPauseFlag (0x134baf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetCaptionPauseFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134BAF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetCaptionPauseFlag, a2_4); A6_IN(426); const uint32_t a2_r = typed::SetCaptionPauseFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetCaptionPauseFlag_0x134baf0(rdram, ctx, runtime);
}
// nico_InitScriptCharObjDefaultValue (0x134c348): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_InitScriptCharObjDefaultValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C348u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_InitScriptCharObjDefaultValue); A6_IN(427); typed::nico_InitScriptCharObjDefaultValue(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_InitScriptCharObjDefaultValue_0x134c348(rdram, ctx, runtime);
}
// SetDefaultStageLoad (0x134c6d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetDefaultStageLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C6D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetDefaultStageLoad, a2_4); A6_IN(428); const uint32_t a2_r = typed::SetDefaultStageLoad(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetDefaultStageLoad_0x134c6d8(rdram, ctx, runtime);
}
// endingflowDemoEnd (0x134d850): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void endingflowDemoEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134D850u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(endingflowDemoEnd); A6_IN(429); const uint32_t a2_r = typed::endingflowDemoEnd(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::endingflowDemoEnd_0x134d850(rdram, ctx, runtime);
}
// fadeCheck (0x134dc10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void fadeCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134DC10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(fadeCheck); A6_IN(430); const uint32_t a2_r = typed::fadeCheck(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::fadeCheck_0x134dc10(rdram, ctx, runtime);
}
// _optionWorkCls (0x134ef68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _optionWorkCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134EF68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(_optionWorkCls, a2_4); A6_IN(431); typed::_optionWorkCls(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_optionWorkCls_0x134ef68(rdram, ctx, runtime);
}
// _gameFlagXor (0x134f060): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _gameFlagXor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F060u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(_gameFlagXor, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); A6_IN(432); const uint64_t a2_r = typed::_gameFlagXor(rdram, runtime, a2_4, a4_h3, a4_h6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_gameFlagXor_0x134f060(rdram, ctx, runtime);
}
// _gameWorkCls (0x134f0f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _gameWorkCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F0F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(_gameWorkCls, a2_4); A6_IN(433); typed::_gameWorkCls(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_gameWorkCls_0x134f0f0(rdram, ctx, runtime);
}
// getGameDataExistIcoData (0x134f218): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGameDataExistIcoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F218u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGameDataExistIcoData); A6_IN(434); const uint32_t a2_r = typed::getGameDataExistIcoData(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGameDataExistIcoData_0x134f218(rdram, ctx, runtime);
}
// getGameDataExistIcoDataAdrs (0x134f228): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGameDataExistIcoDataAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F228u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGameDataExistIcoDataAdrs); A6_IN(435); const uint32_t a2_r = typed::getGameDataExistIcoDataAdrs(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGameDataExistIcoDataAdrs_0x134f228(rdram, ctx, runtime);
}
// getBossIndexTimeAttackBestTime (0x134f248): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBossIndexTimeAttackBestTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F248u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getBossIndexTimeAttackBestTime, a2_4); A6_IN(436); const float a2_r = typed::getBossIndexTimeAttackBestTime(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBossIndexTimeAttackBestTime_0x134f248(rdram, ctx, runtime);
}
// setBossIndexTimeAttackBestTime (0x134f260): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setBossIndexTimeAttackBestTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F260u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(setBossIndexTimeAttackBestTime, a2_4, a2_f12); A6_IN(437); typed::setBossIndexTimeAttackBestTime(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setBossIndexTimeAttackBestTime_0x134f260(rdram, ctx, runtime);
}
// getGameClearCntAdrs (0x134f288): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGameClearCntAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F288u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGameClearCntAdrs); A6_IN(438); const uint32_t a2_r = typed::getGameClearCntAdrs(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGameClearCntAdrs_0x134f288(rdram, ctx, runtime);
}
// setGmaeClearCnt (0x134f298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setGmaeClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F298u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setGmaeClearCnt, a2_4); A6_IN(439); const uint32_t a2_r = typed::setGmaeClearCnt(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setGmaeClearCnt_0x134f298(rdram, ctx, runtime);
}
// getGameHardModeAdrs (0x134f2b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGameHardModeAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F2B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getGameHardModeAdrs); A6_IN(440); const uint32_t a2_r = typed::getGameHardModeAdrs(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGameHardModeAdrs_0x134f2b8(rdram, ctx, runtime);
}
// getSavedGameTime (0x134f2f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSavedGameTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F2F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSavedGameTime); A6_IN(441); const uint32_t a2_r = typed::getSavedGameTime(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSavedGameTime_0x134f2f0(rdram, ctx, runtime);
}
// setSavedGameTime (0x134f300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setSavedGameTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F300u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setSavedGameTime, a2_4); A6_IN(442); const uint32_t a2_r = typed::setSavedGameTime(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setSavedGameTime_0x134f300(rdram, ctx, runtime);
}
// getSavedClock (0x134f310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSavedClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F310u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getSavedClock, a2_4); A6_IN(443); const uint32_t a2_r = typed::getSavedClock(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSavedClock_0x134f310(rdram, ctx, runtime);
}
// setSavedPlaceId (0x134f340): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setSavedPlaceId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F340u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setSavedPlaceId, a2_4); A6_IN(444); const uint32_t a2_r = typed::setSavedPlaceId(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setSavedPlaceId_0x134f340(rdram, ctx, runtime);
}
// tailsFlagSet (0x134f740): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void tailsFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F740u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(tailsFlagSet, a2_4, a2_5, a2_7); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 4)}; A6_IN(445); const uint32_t a2_r = typed::tailsFlagSet(rdram, runtime, a2_4, a2_5, a2_7, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::tailsFlagSet_0x134f740(rdram, ctx, runtime);
}
// tailsFlagClr (0x134f798): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void tailsFlagClr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F798u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(tailsFlagClr, a2_4, a2_5, a2_7); A6_IN(446); const uint32_t a2_r = typed::tailsFlagClr(rdram, runtime, a2_4, a2_5, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::tailsFlagClr_0x134f798(rdram, ctx, runtime);
}
// gexsFlagSet (0x134f880): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gexsFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F880u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(gexsFlagSet, a2_4, a2_5, a2_7); A6_IN(447); const uint32_t a2_r = typed::gexsFlagSet(rdram, runtime, a2_4, a2_5, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gexsFlagSet_0x134f880(rdram, ctx, runtime);
}
// gexsFlagClr (0x134f8d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gexsFlagClr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F8D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(gexsFlagClr, a2_4, a2_5, a2_7); A6_IN(448); const uint32_t a2_r = typed::gexsFlagClr(rdram, runtime, a2_4, a2_5, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gexsFlagClr_0x134f8d8(rdram, ctx, runtime);
}
// _omakeGetFlagSet (0x134f998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _omakeGetFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F998u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(_omakeGetFlagSet, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(449); const uint64_t a2_r = typed::_omakeGetFlagSet(rdram, runtime, a2_4, a4_h3, a4_h6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_omakeGetFlagSet_0x134f998(rdram, ctx, runtime);
}
// _omakeGetFlagCls (0x134f9d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _omakeGetFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134F9D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(_omakeGetFlagCls, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(450); const uint64_t a2_r = typed::_omakeGetFlagCls(rdram, runtime, a2_4, a4_h3, a4_h6, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_omakeGetFlagCls_0x134f9d8(rdram, ctx, runtime);
}
// _omakeSelectFlagCls (0x134fa48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _omakeSelectFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134FA48u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(_omakeSelectFlagCls, a2_4, a2_29); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A6_IN(451); const uint64_t a2_r = typed::_omakeSelectFlagCls(rdram, runtime, a2_4, a2_29, a4_h3, a4_h5, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_omakeSelectFlagCls_0x134fa48(rdram, ctx, runtime);
}
// setClearGameHotStartFlag (0x134fba8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setClearGameHotStartFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134FBA8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(setClearGameHotStartFlag, a2_4); A6_IN(452); const uint64_t a2_r = typed::setClearGameHotStartFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setClearGameHotStartFlag_0x134fba8(rdram, ctx, runtime);
}
// getClearGameHotStartFlag (0x134fbd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getClearGameHotStartFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134FBD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getClearGameHotStartFlag); A6_IN(453); const uint64_t a2_r = typed::getClearGameHotStartFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getClearGameHotStartFlag_0x134fbd0(rdram, ctx, runtime);
}
// gameflowMpegCancelChk (0x1352070): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gameflowMpegCancelChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352070u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gameflowMpegCancelChk); A6_IN(454); const uint32_t a2_r = typed::gameflowMpegCancelChk(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gameflowMpegCancelChk_0x1352070(rdram, ctx, runtime);
}
// gamecore_sub_01352760 (0x1352760): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352760(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352760u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01352760); A6_IN(455); const uint32_t a2_r = typed::gamecore_sub_01352760(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352760_0x1352760(rdram, ctx, runtime);
}
// gamecore_sub_01352780 (0x1352780): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352780(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352780u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01352780); A6_IN(456); const uint32_t a2_r = typed::gamecore_sub_01352780(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352780_0x1352780(rdram, ctx, runtime);
}
// gamecore_sub_01352790 (0x1352790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352790(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352790u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01352790); A6_IN(457); const uint32_t a2_r = typed::gamecore_sub_01352790(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352790_0x1352790(rdram, ctx, runtime);
}
// gamecore_sub_01352878 (0x1352878): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352878(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352878u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01352878); A6_IN(458); const uint32_t a2_r = typed::gamecore_sub_01352878(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352878_0x1352878(rdram, ctx, runtime);
}
// gStatGObjDispOn (0x13535b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatGObjDispOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13535B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gStatGObjDispOn, a2_4); A6_IN(459); const uint32_t a2_r = typed::gStatGObjDispOn(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatGObjDispOn_0x13535b0(rdram, ctx, runtime);
}
// gStatGObjDispOff (0x13535c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatGObjDispOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13535C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gStatGObjDispOff, a2_4); A6_IN(460); const uint32_t a2_r = typed::gStatGObjDispOff(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatGObjDispOff_0x13535c0(rdram, ctx, runtime);
}
// gstatBootMcChkExistNicoData (0x1353dd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gstatBootMcChkExistNicoData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353DD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gstatBootMcChkExistNicoData, a2_4); A6_IN(461); const uint32_t a2_r = typed::gstatBootMcChkExistNicoData(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gstatBootMcChkExistNicoData_0x1353dd8(rdram, ctx, runtime);
}
// checkBoot (0x1356678): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void checkBoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1356678u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(checkBoot); A6_IN(462); const uint32_t a2_r = typed::checkBoot(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::checkBoot_0x1356678(rdram, ctx, runtime);
}
// reloadStage (0x1356688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void reloadStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1356688u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(reloadStage, a2_4, a2_5, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(463); const uint32_t a2_r = typed::reloadStage(rdram, runtime, a2_4, a2_5, a2_29, a4_vf0, a4_h3, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::reloadStage_0x1356688(rdram, ctx, runtime);
}
// reloadStageWithPlayerPos (0x13566e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void reloadStageWithPlayerPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13566E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(reloadStageWithPlayerPos, a2_4, a2_5, a2_6, a2_7); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(464); typed::reloadStageWithPlayerPos(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_h2, a4_h3); A6_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::reloadStageWithPlayerPos_0x13566e8(rdram, ctx, runtime);
}
// getPlayerO2 (0x1358700): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPlayerO2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1358700u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPlayerO2); A6_IN(465); const a1::RetVF a2_r = typed::getPlayerO2(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPlayerO2_0x1358700(rdram, ctx, runtime);
}
// setPlayerO2 (0x1358710): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setPlayerO2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1358710u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; A6_CHK(setPlayerO2, a2_f12); A6_IN(466); const a1::RetVF a2_r = typed::setPlayerO2(rdram, runtime, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setPlayerO2_0x1358710(rdram, ctx, runtime);
}
// addPlayerO2 (0x1358740): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void addPlayerO2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1358740u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; A6_CHK(addPlayerO2, a2_f12); A6_IN(467); const a1::RetVF a2_r = typed::addPlayerO2(rdram, runtime, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::addPlayerO2_0x1358740(rdram, ctx, runtime);
}
// SetSeamlessZFogDist (0x135da20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetSeamlessZFogDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DA20u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SetSeamlessZFogDist, a2_4, a2_5); A6_IN(468); const uint32_t a2_r = typed::SetSeamlessZFogDist(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetSeamlessZFogDist_0x135da20(rdram, ctx, runtime);
}
// getSeamlessStageHighModelDispDist (0x135dac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSeamlessStageHighModelDispDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DAC8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSeamlessStageHighModelDispDist); A6_IN(469); const uint32_t a2_r = typed::getSeamlessStageHighModelDispDist(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSeamlessStageHighModelDispDist_0x135dac8(rdram, ctx, runtime);
}
// getSeamlessStageMiddleModelDispDist (0x135dad8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSeamlessStageMiddleModelDispDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DAD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSeamlessStageMiddleModelDispDist); A6_IN(470); const uint32_t a2_r = typed::getSeamlessStageMiddleModelDispDist(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSeamlessStageMiddleModelDispDist_0x135dad8(rdram, ctx, runtime);
}
// getStageChipIDReal (0x135db08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageChipIDReal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DB08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(getStageChipIDReal, a2_4, a2_5, a2_6); A6_IN(471); const a1::RetVF a2_r = typed::getStageChipIDReal(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageChipIDReal_0x135db08(rdram, ctx, runtime);
}
// getStageChipIDWithOffset (0x135db68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageChipIDWithOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DB68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); A6_CHK(getStageChipIDWithOffset, a2_4, a2_5, a2_6, a2_7, a2_8); A6_IN(472); const a1::RetVF a2_r = typed::getStageChipIDWithOffset(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageChipIDWithOffset_0x135db68(rdram, ctx, runtime);
}
// getStageNormalizePosition (0x135dbe8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageNormalizePosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DBE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(getStageNormalizePosition, a2_4, a2_5, a2_6); A6_IN(473); const a1::RetVF a2_r = typed::getStageNormalizePosition(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageNormalizePosition_0x135dbe8(rdram, ctx, runtime);
}
// getCameraPosOnMap (0x135dc40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCameraPosOnMap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DC40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(getCameraPosOnMap, a2_4, a2_5); A6_IN(474); const a1::RetVF a2_r = typed::getCameraPosOnMap(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCameraPosOnMap_0x135dc40(rdram, ctx, runtime);
}
// getSeamlessStageCenter (0x135dca8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSeamlessStageCenter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DCA8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(getSeamlessStageCenter, a2_4, a2_5); A6_IN(475); const a1::RetVF a2_r = typed::getSeamlessStageCenter(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSeamlessStageCenter_0x135dca8(rdram, ctx, runtime);
}
// getStageLayout (0x135dd68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DD68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(getStageLayout, a2_4, a2_5, a2_6); A6_IN(476); const uint64_t a2_r = typed::getStageLayout(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageLayout_0x135dd68(rdram, ctx, runtime);
}
// getStageStat (0x135de38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageStat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DE38u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(getStageStat, a2_4, a2_5, a2_6); A6_IN(477); const uint32_t a2_r = typed::getStageStat(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageStat_0x135de38(rdram, ctx, runtime);
}
// GetLoadAreaGroupData (0x135deb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLoadAreaGroupData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DEB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetLoadAreaGroupData); A6_IN(478); const uint32_t a2_r = typed::GetLoadAreaGroupData(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLoadAreaGroupData_0x135deb0(rdram, ctx, runtime);
}
// UpdateSeamlessStageLoad (0x135dfa8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void UpdateSeamlessStageLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135DFA8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(UpdateSeamlessStageLoad); A6_IN(479); const uint32_t a2_r = typed::UpdateSeamlessStageLoad(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::UpdateSeamlessStageLoad_0x135dfa8(rdram, ctx, runtime);
}
// GetStageChipNum (0x135e068): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetStageChipNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E068u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetStageChipNum); A6_IN(480); const uint64_t a2_r = typed::GetStageChipNum(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetStageChipNum_0x135e068(rdram, ctx, runtime);
}
// DeleteSeamlessStageObject (0x135e088): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DeleteSeamlessStageObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E088u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(DeleteSeamlessStageObject, a2_4, a2_5, a2_6); A6_IN(481); const uint64_t a2_r = typed::DeleteSeamlessStageObject(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DeleteSeamlessStageObject_0x135e088(rdram, ctx, runtime);
}
// GetSeamlessStageObjectGroup (0x135e128): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetSeamlessStageObjectGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E128u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetSeamlessStageObjectGroup); A6_IN(482); const uint32_t a2_r = typed::GetSeamlessStageObjectGroup(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetSeamlessStageObjectGroup_0x135e128(rdram, ctx, runtime);
}
// getChipNo (0x135e238): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getChipNo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E238u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(getChipNo, a2_4, a2_5); A6_IN(483); const uint32_t a2_r = typed::getChipNo(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getChipNo_0x135e238(rdram, ctx, runtime);
}
// getStageStatByChipNo (0x135e310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageStatByChipNo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E310u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(getStageStatByChipNo, a2_4, a2_5); A6_IN(484); const uint32_t a2_r = typed::getStageStatByChipNo(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageStatByChipNo_0x135e310(rdram, ctx, runtime);
}
// getStageChipIDFloat (0x135e410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getStageChipIDFloat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E410u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(getStageChipIDFloat, a2_4, a2_5, a2_6); A6_IN(485); const a1::RetVF a2_r = typed::getStageChipIDFloat(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getStageChipIDFloat_0x135e410(rdram, ctx, runtime);
}
// RestartSeamlessStage (0x135e940): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void RestartSeamlessStage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x135E940u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(RestartSeamlessStage); A6_IN(486); const uint32_t a2_r = typed::RestartSeamlessStage(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::RestartSeamlessStage_0x135e940(rdram, ctx, runtime);
}
// gcGetNbStages (0x1365d58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gcGetNbStages(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1365D58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gcGetNbStages); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(487); const uint32_t a2_r = typed::gcGetNbStages(rdram, runtime, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gcGetNbStages_0x1365d58(rdram, ctx, runtime);
}
// CheckStageManagerLoadArea (0x1365eb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CheckStageManagerLoadArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1365EB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(CheckStageManagerLoadArea, a2_4, a2_5, a2_6); A6_IN(488); const uint64_t a2_r = typed::CheckStageManagerLoadArea(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CheckStageManagerLoadArea_0x1365eb8(rdram, ctx, runtime);
}
// stagemanager_GetStageAlignParam (0x1366660): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void stagemanager_GetStageAlignParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366660u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(stagemanager_GetStageAlignParam); A6_IN(489); const uint32_t a2_r = typed::stagemanager_GetStageAlignParam(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::stagemanager_GetStageAlignParam_0x1366660(rdram, ctx, runtime);
}
// stagemanager_getNbWorldLayoutDatas (0x1366858): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void stagemanager_getNbWorldLayoutDatas(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366858u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(stagemanager_getNbWorldLayoutDatas); A6_IN(490); const uint32_t a2_r = typed::stagemanager_getNbWorldLayoutDatas(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::stagemanager_getNbWorldLayoutDatas_0x1366858(rdram, ctx, runtime);
}
// stagemanager_checkstageAlreadyLoadRequested (0x1366890): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void stagemanager_checkstageAlreadyLoadRequested(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366890u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(stagemanager_checkstageAlreadyLoadRequested, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(491); const uint32_t a2_r = typed::stagemanager_checkstageAlreadyLoadRequested(rdram, runtime, a2_4, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::stagemanager_checkstageAlreadyLoadRequested_0x1366890(rdram, ctx, runtime);
}
// getWorldLayoutHead (0x1366ce0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getWorldLayoutHead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366CE0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getWorldLayoutHead); A6_IN(492); const uint32_t a2_r = typed::getWorldLayoutHead(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getWorldLayoutHead_0x1366ce0(rdram, ctx, runtime);
}
// deleteWorldLayout (0x1366d10): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1366D10u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteWorldLayout); A6_IN(493); const uint32_t a2_r = typed::deleteWorldLayout(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteWorldLayout_0x1366d10(rdram, ctx, runtime);
}
// getBossExtControl (0x1367b78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBossExtControl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367B78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBossExtControl); A6_IN(494); const uint32_t a2_r = typed::getBossExtControl(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBossExtControl_0x1367b78(rdram, ctx, runtime);
}
// getTargetAIControlMode (0x1367b88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getTargetAIControlMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367B88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getTargetAIControlMode); A6_IN(495); const uint32_t a2_r = typed::getTargetAIControlMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getTargetAIControlMode_0x1367b88(rdram, ctx, runtime);
}
// time_attack_get_best_time (0x1367e30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void time_attack_get_best_time(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367E30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(time_attack_get_best_time); A6_IN(496); const a1::RetVF a2_r = typed::time_attack_get_best_time(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::time_attack_get_best_time_0x1367e30(rdram, ctx, runtime);
}
// time_attack_get_boss_status (0x1367e68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void time_attack_get_boss_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367E68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(time_attack_get_boss_status); A6_IN(497); const uint32_t a2_r = typed::time_attack_get_boss_status(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::time_attack_get_boss_status_0x1367e68(rdram, ctx, runtime);
}
// time_attack_end (0x1367e78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void time_attack_end(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1367E78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(time_attack_end); A6_IN(498); const uint32_t a2_r = typed::time_attack_end(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::time_attack_end_0x1367e78(rdram, ctx, runtime);
}
// bgmScriptSendSoundSignal (0x136a198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmScriptSendSoundSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A198u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(bgmScriptSendSoundSignal, a2_4); A6_IN(501); const uint32_t a2_r = typed::bgmScriptSendSoundSignal(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmScriptSendSoundSignal_0x136a198(rdram, ctx, runtime);
}
// bgmScriptRecvSoundSignal (0x136a1b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmScriptRecvSoundSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A1B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(bgmScriptRecvSoundSignal, a2_4); A6_IN(502); const uint32_t a2_r = typed::bgmScriptRecvSoundSignal(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmScriptRecvSoundSignal_0x136a1b8(rdram, ctx, runtime);
}
// bgmDemoPackagePlay (0x136a1f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmDemoPackagePlay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A1F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(bgmDemoPackagePlay, a2_4); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7)}; A6_IN(503); const uint32_t a2_r = typed::bgmDemoPackagePlay(rdram, runtime, a2_4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 5, a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); Ps2SetGpr64(ctx, 7, a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmDemoPackagePlay_0x136a1f0(rdram, ctx, runtime);
}
// gamecore_sub_0136A3A8 (0x136a3a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0136A3A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A3A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(gamecore_sub_0136A3A8, a2_4); A6_IN(504); const uint32_t a2_r = typed::gamecore_sub_0136A3A8(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0136A3A8_0x136a3a8(rdram, ctx, runtime);
}
// scriptNormalHook (0x136a3f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void scriptNormalHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A3F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_8 = Ps2GprU32(ctx, 8); A6_CHK(scriptNormalHook, a2_8); A6_IN(505); const uint32_t a2_r = typed::scriptNormalHook(rdram, runtime, a2_8); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::scriptNormalHook_0x136a3f8(rdram, ctx, runtime);
}
// gamecore_sub_0136A408 (0x136a408): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0136A408(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A408u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0136A408); A6_IN(506); typed::gamecore_sub_0136A408(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0136A408_0x136a408(rdram, ctx, runtime);
}
// demoNormalHook (0x136a410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void demoNormalHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A410u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_8 = Ps2GprU32(ctx, 8); A6_CHK(demoNormalHook, a2_8); A6_IN(507); const uint32_t a2_r = typed::demoNormalHook(rdram, runtime, a2_8); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::demoNormalHook_0x136a410(rdram, ctx, runtime);
}
// bgmStopAllAttr (0x136aa78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmStopAllAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136AA78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(bgmStopAllAttr, a2_4, a2_5); A6_IN(508); const uint64_t a2_r = typed::bgmStopAllAttr(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmStopAllAttr_0x136aa78(rdram, ctx, runtime);
}
// bgmEndGameOver (0x136aaf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmEndGameOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136AAF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(bgmEndGameOver); A6_IN(509); typed::bgmEndGameOver(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmEndGameOver_0x136aaf0(rdram, ctx, runtime);
}
// gamecore_sub_0136AB80 (0x136ab80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0136AB80(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136AB80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0136AB80); A6_IN(510); typed::gamecore_sub_0136AB80(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0136AB80_0x136ab80(rdram, ctx, runtime);
}
// gamecore_sub_0136F298 (0x136f298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0136F298(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136F298u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0136F298); A6_IN(511); typed::gamecore_sub_0136F298(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0136F298_0x136f298(rdram, ctx, runtime);
}
// bootmccheckscriptNoBoot (0x136fd68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bootmccheckscriptNoBoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136FD68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(bootmccheckscriptNoBoot); A6_IN(512); const uint32_t a2_r = typed::bootmccheckscriptNoBoot(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bootmccheckscriptNoBoot_0x136fd68(rdram, ctx, runtime);
}
// nico_IK_leafSetWithMotion (0x13757c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_IK_leafSetWithMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13757C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_IK_leafSetWithMotion); A6_IN(513); typed::nico_IK_leafSetWithMotion(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_IK_leafSetWithMotion_0x13757c0(rdram, ctx, runtime);
}
// SCRGetBossPartsDamage (0x1378868): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetBossPartsDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1378868u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SCRGetBossPartsDamage, a2_4, a2_5); A6_IN(514); const float a2_r = typed::SCRGetBossPartsDamage(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRGetBossPartsDamage_0x1378868(rdram, ctx, runtime);
}
// GetBossPartsDamage (0x1379220): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetBossPartsDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1379220u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(GetBossPartsDamage, a2_4, a2_5); A6_IN(515); const a1::RetVF a2_r = typed::GetBossPartsDamage(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetBossPartsDamage_0x1379220(rdram, ctx, runtime);
}
// gamecore_sub_0137B790 (0x137b790): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0137B790(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137B790u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0137B790); A6_IN(516); typed::gamecore_sub_0137B790(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0137B790_0x137b790(rdram, ctx, runtime);
}
// getDebugDispType (0x137c2c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugDispType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugDispType); A6_IN(517); const uint64_t a2_r = typed::getDebugDispType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugDispType_0x137c2c0(rdram, ctx, runtime);
}
// getDebugMotionStop (0x137c2c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugMotionStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugMotionStop); A6_IN(518); const uint64_t a2_r = typed::getDebugMotionStop(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugMotionStop_0x137c2c8(rdram, ctx, runtime);
}
// getDebugScrDataPrintf (0x137c2d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugScrDataPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugScrDataPrintf); A6_IN(519); const uint64_t a2_r = typed::getDebugScrDataPrintf(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugScrDataPrintf_0x137c2d0(rdram, ctx, runtime);
}
// getDebugHorseRoute (0x137c2d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugHorseRoute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugHorseRoute); A6_IN(520); const uint64_t a2_r = typed::getDebugHorseRoute(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugHorseRoute_0x137c2d8(rdram, ctx, runtime);
}
// getDebugHorseBikko (0x137c2e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugHorseBikko(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugHorseBikko); A6_IN(521); const uint64_t a2_r = typed::getDebugHorseBikko(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugHorseBikko_0x137c2e0(rdram, ctx, runtime);
}
// getDebugParentAttrReset (0x137c2e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugParentAttrReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugParentAttrReset); A6_IN(522); const uint64_t a2_r = typed::getDebugParentAttrReset(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugParentAttrReset_0x137c2e8(rdram, ctx, runtime);
}
// getDebugTrigerPrintf (0x137c2f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugTrigerPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugTrigerPrintf); A6_IN(523); const uint64_t a2_r = typed::getDebugTrigerPrintf(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugTrigerPrintf_0x137c2f0(rdram, ctx, runtime);
}
// getDebugHorseInsurance (0x137c2f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugHorseInsurance(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C2F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugHorseInsurance); A6_IN(524); const uint64_t a2_r = typed::getDebugHorseInsurance(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugHorseInsurance_0x137c2f8(rdram, ctx, runtime);
}
// getDebugHorsePathVirtual (0x137c300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugHorsePathVirtual(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C300u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugHorsePathVirtual); A6_IN(525); const uint64_t a2_r = typed::getDebugHorsePathVirtual(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugHorsePathVirtual_0x137c300(rdram, ctx, runtime);
}
// getDebugViewDisp (0x137c308): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugViewDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C308u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugViewDisp); A6_IN(526); const uint64_t a2_r = typed::getDebugViewDisp(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugViewDisp_0x137c308(rdram, ctx, runtime);
}
// GetDebugMotionStop (0x137c318): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebugMotionStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C318u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebugMotionStop); A6_IN(527); const uint64_t a2_r = typed::GetDebugMotionStop(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebugMotionStop_0x137c318(rdram, ctx, runtime);
}
// GetDebugScrDataPrintf (0x137c320): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebugScrDataPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C320u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebugScrDataPrintf); A6_IN(528); const uint64_t a2_r = typed::GetDebugScrDataPrintf(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebugScrDataPrintf_0x137c320(rdram, ctx, runtime);
}
// GetDebugViewDisp (0x137c328): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetDebugViewDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137C328u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetDebugViewDisp); A6_IN(529); const uint64_t a2_r = typed::GetDebugViewDisp(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetDebugViewDisp_0x137c328(rdram, ctx, runtime);
}
// getBosstatusDispMode (0x137e238): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBosstatusDispMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E238u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBosstatusDispMode); A6_IN(530); const uint32_t a2_r = typed::getBosstatusDispMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBosstatusDispMode_0x137e238(rdram, ctx, runtime);
}
// getBoneCollisionDebugMode (0x137e2a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBoneCollisionDebugMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E2A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBoneCollisionDebugMode); A6_IN(531); const uint32_t a2_r = typed::getBoneCollisionDebugMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBoneCollisionDebugMode_0x137e2a8(rdram, ctx, runtime);
}
// AI_GetDeathTentWorkSize (0x1384e98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_GetDeathTentWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1384E98u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(AI_GetDeathTentWorkSize, a2_4, a2_5); A6_IN(532); const uint32_t a2_r = typed::AI_GetDeathTentWorkSize(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_GetDeathTentWorkSize_0x1384e98(rdram, ctx, runtime);
}
// GetMeshObjInfo (0x1384ef8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMeshObjInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1384EF8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(GetMeshObjInfo, a2_4, a2_5, a2_6); A6_IN(533); const uint64_t a2_r = typed::GetMeshObjInfo(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetMeshObjInfo_0x1384ef8(rdram, ctx, runtime);
}
// AI_GetBaseWorkSize (0x1384f30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_GetBaseWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1384F30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(AI_GetBaseWorkSize, a2_4, a2_5); A6_IN(534); const uint32_t a2_r = typed::AI_GetBaseWorkSize(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_GetBaseWorkSize_0x1384f30(rdram, ctx, runtime);
}
// getDynamicsTestType (0x13862c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDynamicsTestType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13862C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDynamicsTestType); A6_IN(535); const uint32_t a2_r = typed::getDynamicsTestType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDynamicsTestType_0x13862c0(rdram, ctx, runtime);
}
// getPendulumGravityRatio (0x13862d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPendulumGravityRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13862D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPendulumGravityRatio); A6_IN(536); const uint32_t a2_r = typed::getPendulumGravityRatio(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPendulumGravityRatio_0x13862d0(rdram, ctx, runtime);
}
// getPendulumAirResistRatio (0x13862e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPendulumAirResistRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13862E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPendulumAirResistRatio); A6_IN(537); const uint32_t a2_r = typed::getPendulumAirResistRatio(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPendulumAirResistRatio_0x13862e0(rdram, ctx, runtime);
}
// getPendulumSideFriction (0x13862f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPendulumSideFriction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13862F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getPendulumSideFriction); A6_IN(538); const uint32_t a2_r = typed::getPendulumSideFriction(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPendulumSideFriction_0x13862f0(rdram, ctx, runtime);
}
// dynamicstest (0x1386300): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void dynamicstest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1386300u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(dynamicstest); A6_IN(539); typed::dynamicstest(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::dynamicstest_0x1386300(rdram, ctx, runtime);
}
// getBloodEffectEnableFlag (0x1388618): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBloodEffectEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388618u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBloodEffectEnableFlag); A6_IN(540); const uint32_t a2_r = typed::getBloodEffectEnableFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBloodEffectEnableFlag_0x1388618(rdram, ctx, runtime);
}
// gamecore_sub_01388628 (0x1388628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388628(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388628); A6_IN(541); const uint32_t a2_r = typed::gamecore_sub_01388628(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388628_0x1388628(rdram, ctx, runtime);
}
// gamecore_sub_01388638 (0x1388638): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388638(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388638u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388638); A6_IN(542); const uint32_t a2_r = typed::gamecore_sub_01388638(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388638_0x1388638(rdram, ctx, runtime);
}
// gamecore_sub_01388648 (0x1388648): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388648(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388648u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388648); A6_IN(543); const uint32_t a2_r = typed::gamecore_sub_01388648(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388648_0x1388648(rdram, ctx, runtime);
}
// gamecore_sub_01388658 (0x1388658): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388658(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388658u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388658); A6_IN(544); const uint32_t a2_r = typed::gamecore_sub_01388658(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388658_0x1388658(rdram, ctx, runtime);
}
// gamecore_sub_01388668 (0x1388668): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388668(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388668u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388668); A6_IN(545); const uint32_t a2_r = typed::gamecore_sub_01388668(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388668_0x1388668(rdram, ctx, runtime);
}
// gamecore_sub_01388678 (0x1388678): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01388678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388678u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01388678); A6_IN(546); const uint32_t a2_r = typed::gamecore_sub_01388678(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01388678_0x1388678(rdram, ctx, runtime);
}
// getWeakGlowSpeed (0x1388688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getWeakGlowSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388688u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getWeakGlowSpeed); A6_IN(547); const uint32_t a2_r = typed::getWeakGlowSpeed(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getWeakGlowSpeed_0x1388688(rdram, ctx, runtime);
}
// getWeakGlowTime (0x1388698): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getWeakGlowTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1388698u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getWeakGlowTime); A6_IN(548); const uint32_t a2_r = typed::getWeakGlowTime(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getWeakGlowTime_0x1388698(rdram, ctx, runtime);
}
// getTrigGlowTime (0x13886a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getTrigGlowTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13886A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getTrigGlowTime); A6_IN(549); const uint32_t a2_r = typed::getTrigGlowTime(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getTrigGlowTime_0x13886a8(rdram, ctx, runtime);
}
// gamecore_sub_013888E0 (0x13888e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013888E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13888E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gamecore_sub_013888E0, a2_4); A6_IN(550); const uint32_t a2_r = typed::gamecore_sub_013888E0(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013888E0_0x13888e0(rdram, ctx, runtime);
}
// gamecore_sub_0138B8E8 (0x138b8e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138B8E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138B8E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(gamecore_sub_0138B8E8, a2_5, a2_6); A6_IN(551); typed::gamecore_sub_0138B8E8(rdram, runtime, a2_5, a2_6); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0138B8E8_0x138b8e8(rdram, ctx, runtime);
}
// set_root_node_data_expression (0x138be08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_root_node_data_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138BE08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; A6_CHK(set_root_node_data_expression, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 4)}; A6_IN(552); typed::set_root_node_data_expression(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 4, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_root_node_data_expression_0x138be08(rdram, ctx, runtime);
}
// add_element_expression_change (0x138bf08): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void add_element_expression_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138BF08u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(add_element_expression_change, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 8)}; A6_IN(553); const uint32_t a2_r = typed::add_element_expression_change(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 8, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::add_element_expression_change_0x138bf08(rdram, ctx, runtime);
}
// gamecore_sub_0138F180 (0x138f180): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138F180(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138F180u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(gamecore_sub_0138F180, a2_6, a2_7); A6_IN(554); typed::gamecore_sub_0138F180(rdram, runtime, a2_6, a2_7); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0138F180_0x138f180(rdram, ctx, runtime);
}
// gamecore_sub_0138F978 (0x138f978): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0138F978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138F978u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0138F978); A6_IN(555); const uint32_t a2_r = typed::gamecore_sub_0138F978(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0138F978_0x138f978(rdram, ctx, runtime);
}
// gamecore_sub_01394F18 (0x1394f18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01394F18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1394F18u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_01394F18); uint64_t a3_xo[3] = {(uint64_t)a1_bits(ctx->f[12]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6)}; A6_IN(556); const a1::RetVF a2_r = typed::gamecore_sub_01394F18(rdram, runtime, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 4, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01394F18_0x1394f18(rdram, ctx, runtime);
}
// gamecore_sub_01398678 (0x1398678): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01398678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1398678u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(gamecore_sub_01398678, a2_4, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); A6_IN(557); const uint64_t a2_r = typed::gamecore_sub_01398678(rdram, runtime, a2_4, a2_6, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h8); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01398678_0x1398678(rdram, ctx, runtime);
}
// initbar (0x139a090): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void initbar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A090u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(initbar, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(558); const a1::RetVF a2_r = typed::initbar(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f12, a2_f13, a2_f14, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::initbar_0x139a090(rdram, ctx, runtime);
}
// setbar (0x139a0f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setbar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A0F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(setbar, a2_4, a2_f12); A6_IN(559); const float a2_r = typed::setbar(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setbar_0x139a0f0(rdram, ctx, runtime);
}
// SpriteSetStatus (0x139a198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteSetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A198u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(SpriteSetStatus, a2_4, a2_5); A6_IN(560); const uint32_t a2_r = typed::SpriteSetStatus(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteSetStatus_0x139a198(rdram, ctx, runtime);
}
// Init_TimeFloatInter (0x139a218): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Init_TimeFloatInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A218u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(Init_TimeFloatInter, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(561); typed::Init_TimeFloatInter(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Init_TimeFloatInter_0x139a218(rdram, ctx, runtime);
}
// Update_TimeFloatInter (0x139a230): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Update_TimeFloatInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A230u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(Update_TimeFloatInter, a2_4); A6_IN(562); const a1::RetVF a2_r = typed::Update_TimeFloatInter(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Update_TimeFloatInter_0x139a230(rdram, ctx, runtime);
}
// Init_TimeFloatSpeed (0x139a348): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Init_TimeFloatSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A348u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(Init_TimeFloatSpeed, a2_4, a2_f12, a2_f13, a2_f14); A6_IN(563); typed::Init_TimeFloatSpeed(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Init_TimeFloatSpeed_0x139a348(rdram, ctx, runtime);
}
// Update_TimeFloatSpeed (0x139a360): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Update_TimeFloatSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A360u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(Update_TimeFloatSpeed, a2_4); A6_IN(564); const a1::RetVF a2_r = typed::Update_TimeFloatSpeed(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Update_TimeFloatSpeed_0x139a360(rdram, ctx, runtime);
}
// ColCont_IsColStatus (0x139a928): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ColCont_IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A928u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(ColCont_IsColStatus, a2_4, a2_5, a2_6); A6_IN(565); const uint64_t a2_r = typed::ColCont_IsColStatus(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ColCont_IsColStatus_0x139a928(rdram, ctx, runtime);
}
// ColCont2_EventAttrReset (0x139a9f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ColCont2_EventAttrReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139A9F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(ColCont2_EventAttrReset, a2_4); A6_IN(566); typed::ColCont2_EventAttrReset(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ColCont2_EventAttrReset_0x139a9f8(rdram, ctx, runtime);
}
// gamecore_sub_0139BB70 (0x139bb70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0139BB70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139BB70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(gamecore_sub_0139BB70, a2_4, a2_5, a2_6, a2_7); uint64_t a3_xo[4] = {Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(567); const uint64_t a2_r = typed::gamecore_sub_0139BB70(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 8, a3_xo[0]); Ps2SetGpr64(ctx, 9, a3_xo[1]); Ps2SetGpr64(ctx, 10, a3_xo[2]); Ps2SetGpr64(ctx, 11, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0139BB70_0x139bb70(rdram, ctx, runtime);
}
// gamecore_sub_0139E490 (0x139e490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0139E490(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E490u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0139E490); A6_IN(568); typed::gamecore_sub_0139E490(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0139E490_0x139e490(rdram, ctx, runtime);
}
// gamecore_sub_0139E498 (0x139e498): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0139E498(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E498u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0139E498); A6_IN(569); typed::gamecore_sub_0139E498(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0139E498_0x139e498(rdram, ctx, runtime);
}
// gamecore_sub_0139E4A0 (0x139e4a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0139E4A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E4A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0139E4A0); A6_IN(570); typed::gamecore_sub_0139E4A0(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0139E4A0_0x139e4a0(rdram, ctx, runtime);
}
// gamecore_sub_0139E4A8 (0x139e4a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0139E4A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E4A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0139E4A8); A6_IN(571); typed::gamecore_sub_0139E4A8(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0139E4A8_0x139e4a8(rdram, ctx, runtime);
}
// gamecore_sub_013A6FB0 (0x13a6fb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A6FB0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A6FB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(gamecore_sub_013A6FB0, a2_4); A6_IN(572); const uint32_t a2_r = typed::gamecore_sub_013A6FB0(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013A6FB0_0x13a6fb0(rdram, ctx, runtime);
}
// CalcFootPos (0x13a7298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CalcFootPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A7298u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); A6_CHK(CalcFootPos, a2_4, a2_5, a2_6, a2_7); a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); A6_IN(573); const a1::RetVF a2_r = typed::CalcFootPos(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_h2, a4_h8); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CalcFootPos_0x13a7298(rdram, ctx, runtime);
}
// CalcDistancePointAnd2Point (0x13a7318): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CalcDistancePointAnd2Point(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A7318u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(CalcDistancePointAnd2Point, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); A6_IN(574); const a1::RetVF a2_r = typed::CalcDistancePointAnd2Point(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf0, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vq, a4_vstat, a4_h2, a4_h8); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CalcDistancePointAnd2Point_0x13a7318(rdram, ctx, runtime);
}
// SCRFuncNargaPlayerStop (0x13a8970): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncNargaPlayerStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A8970u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(SCRFuncNargaPlayerStop); A6_IN(575); typed::SCRFuncNargaPlayerStop(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncNargaPlayerStop_0x13a8970(rdram, ctx, runtime);
}
// get_ofst_yaw_nico_sight_on (0x13aa390): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_ofst_yaw_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA390u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_ofst_yaw_nico_sight_on, a2_4); A6_IN(576); const float a2_r = typed::get_ofst_yaw_nico_sight_on(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_ofst_yaw_nico_sight_on_0x13aa390(rdram, ctx, runtime);
}
// get_ofst_pitch_nico_sight_on (0x13aa398): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_ofst_pitch_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA398u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_ofst_pitch_nico_sight_on); A6_IN(577); const float a2_r = typed::get_ofst_pitch_nico_sight_on(rdram, runtime); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_ofst_pitch_nico_sight_on_0x13aa398(rdram, ctx, runtime);
}
// get_base_yaw_nico_sight_on (0x13aa3a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_base_yaw_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA3A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_base_yaw_nico_sight_on, a2_4); A6_IN(578); const float a2_r = typed::get_base_yaw_nico_sight_on(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_base_yaw_nico_sight_on_0x13aa3a8(rdram, ctx, runtime);
}
// get_base_pitch_nico_sight_on (0x13aa408): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_base_pitch_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA408u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_base_pitch_nico_sight_on, a2_4); A6_IN(579); const float a2_r = typed::get_base_pitch_nico_sight_on(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_base_pitch_nico_sight_on_0x13aa408(rdram, ctx, runtime);
}
// get_hope_yaw_nico_sight_on (0x13aa478): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_hope_yaw_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA478u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_hope_yaw_nico_sight_on, a2_4); A6_IN(580); const float a2_r = typed::get_hope_yaw_nico_sight_on(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_hope_yaw_nico_sight_on_0x13aa478(rdram, ctx, runtime);
}
// get_hope_pitch_nico_sight_on (0x13aa480): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_hope_pitch_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA480u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_hope_pitch_nico_sight_on, a2_4); A6_IN(581); const float a2_r = typed::get_hope_pitch_nico_sight_on(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_hope_pitch_nico_sight_on_0x13aa480(rdram, ctx, runtime);
}
// get_snipe_position_nico_sight_on (0x13aa488): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_snipe_position_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13AA488u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_snipe_position_nico_sight_on, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(582); const uint64_t a2_r = typed::get_snipe_position_nico_sight_on(rdram, runtime, a2_4, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_snipe_position_nico_sight_on_0x13aa488(rdram, ctx, runtime);
}
// player_AutoHorseCtrlOff (0x13b5360): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_AutoHorseCtrlOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5360u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(player_AutoHorseCtrlOff, a2_4); A6_IN(583); const uint32_t a2_r = typed::player_AutoHorseCtrlOff(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::player_AutoHorseCtrlOff_0x13b5360(rdram, ctx, runtime);
}
// player_IsHorseCtrlOff (0x13b5a78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void player_IsHorseCtrlOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B5A78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(player_IsHorseCtrlOff); A6_IN(584); const uint32_t a2_r = typed::player_IsHorseCtrlOff(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::player_IsHorseCtrlOff_0x13b5a78(rdram, ctx, runtime);
}
// gamecore_sub_013B7AE0 (0x13b7ae0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B7AE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B7AE0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_013B7AE0); A6_IN(585); typed::gamecore_sub_013B7AE0(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013B7AE0_0x13b7ae0(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionFloorTable (0x13ba560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionFloorTable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA560u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_setPlayerDemoAdditionClothCollisionFloorTable); A6_IN(586); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionFloorTable(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionFloorTable_0x13ba560(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor (0x13ba5a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA5A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor); A6_IN(587); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor_0x13ba5a0(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor (0x13ba5e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA5E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor, a2_4); A6_IN(588); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor_0x13ba5e0(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionEvisFloor (0x13ba628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionEvisFloor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA628u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_setPlayerDemoAdditionClothCollisionEvisFloor); A6_IN(589); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionEvisFloor(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionEvisFloor_0x13ba628(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionRightHand (0x13ba668): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionRightHand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA668u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(nico_setPlayerDemoAdditionClothCollisionRightHand, a2_4); A6_IN(590); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionRightHand(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionRightHand_0x13ba668(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionRightHandY (0x13ba6a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionRightHandY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA6A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(nico_setPlayerDemoAdditionClothCollisionRightHandY, a2_4); A6_IN(591); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionRightHandY(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionRightHandY_0x13ba6a8(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionHand (0x13ba6e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionHand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA6E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(nico_setPlayerDemoAdditionClothCollisionHand, a2_4); A6_IN(592); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionHand(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionHand_0x13ba6e8(rdram, ctx, runtime);
}
// nico_setPlayerDemoAdditionClothCollisionLeftHandY (0x13ba728): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_setPlayerDemoAdditionClothCollisionLeftHandY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BA728u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(nico_setPlayerDemoAdditionClothCollisionLeftHandY, a2_4); A6_IN(593); const uint32_t a2_r = typed::nico_setPlayerDemoAdditionClothCollisionLeftHandY(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_setPlayerDemoAdditionClothCollisionLeftHandY_0x13ba728(rdram, ctx, runtime);
}
// gamecore_sub_013BB470 (0x13bb470): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013BB470(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13BB470u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gamecore_sub_013BB470, a2_4); A6_IN(594); const a1::RetVF a2_r = typed::gamecore_sub_013BB470(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013BB470_0x13bb470(rdram, ctx, runtime);
}
// getBossCloudObj (0x13d0aa8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBossCloudObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D0AA8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBossCloudObj); A6_IN(595); const uint32_t a2_r = typed::getBossCloudObj(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBossCloudObj_0x13d0aa8(rdram, ctx, runtime);
}
// AI_UpdateVelocityXYZ (0x13d2580): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_UpdateVelocityXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D2580u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(AI_UpdateVelocityXYZ, a2_4, a2_5, a2_6); A6_IN(596); const a1::RetVF a2_r = typed::AI_UpdateVelocityXYZ(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_UpdateVelocityXYZ_0x13d2580(rdram, ctx, runtime);
}
// AI_CheckNoAngleVec (0x13d2de8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_CheckNoAngleVec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D2DE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(AI_CheckNoAngleVec, a2_4, a2_5, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(597); const a1::RetVF a2_r = typed::AI_CheckNoAngleVec(rdram, runtime, a2_4, a2_5, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_CheckNoAngleVec_0x13d2de8(rdram, ctx, runtime);
}
// AI_UpdateVelocity2 (0x13d3e00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_UpdateVelocity2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D3E00u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(AI_UpdateVelocity2, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(598); const a1::RetVF a2_r = typed::AI_UpdateVelocity2(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_UpdateVelocity2_0x13d3e00(rdram, ctx, runtime);
}
// AI_GetVelocity2 (0x13d45c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_GetVelocity2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D45C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(AI_GetVelocity2, a2_f12, a2_f13, a2_f14, a2_f15); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13])}; A6_IN(599); const float a2_r = typed::AI_GetVelocity2(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15, a3_xo); A6_OUT(); ctx->f[0] = a2_r; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_GetVelocity2_0x13d45c8(rdram, ctx, runtime);
}
// AI_GetLengthSquarePerpendicular (0x13d4c48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_GetLengthSquarePerpendicular(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D4C48u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(AI_GetLengthSquarePerpendicular, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(600); const a1::RetVF a2_r = typed::AI_GetLengthSquarePerpendicular(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_GetLengthSquarePerpendicular_0x13d4c48(rdram, ctx, runtime);
}
// AI_DebugDir (0x13d5940): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_DebugDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5940u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AI_DebugDir); A6_IN(601); typed::AI_DebugDir(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_DebugDir_0x13d5940(rdram, ctx, runtime);
}
// AI_DebugArrow (0x13d5948): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_DebugArrow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5948u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AI_DebugArrow); A6_IN(602); typed::AI_DebugArrow(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_DebugArrow_0x13d5948(rdram, ctx, runtime);
}
// AI_DebugCylinder (0x13d5950): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_DebugCylinder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5950u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AI_DebugCylinder); A6_IN(603); typed::AI_DebugCylinder(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_DebugCylinder_0x13d5950(rdram, ctx, runtime);
}
// AIDebug_GetFirstObjOfMyGroup (0x13d5958): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetFirstObjOfMyGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5958u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetFirstObjOfMyGroup); A6_IN(604); const uint64_t a2_r = typed::AIDebug_GetFirstObjOfMyGroup(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetFirstObjOfMyGroup_0x13d5958(rdram, ctx, runtime);
}
// AIDebug_GetAnimalMode (0x13d5960): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetAnimalMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5960u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetAnimalMode); A6_IN(605); const uint64_t a2_r = typed::AIDebug_GetAnimalMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetAnimalMode_0x13d5960(rdram, ctx, runtime);
}
// AIDebug_CheckPlayerParent (0x13d5968): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_CheckPlayerParent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5968u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_CheckPlayerParent); A6_IN(606); const uint64_t a2_r = typed::AIDebug_CheckPlayerParent(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_CheckPlayerParent_0x13d5968(rdram, ctx, runtime);
}
// AIDebug_SetID (0x13d5970): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_SetID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5970u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_SetID); A6_IN(607); const uint64_t a2_r = typed::AIDebug_SetID(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_SetID_0x13d5970(rdram, ctx, runtime);
}
// AIDebug_GetAnimalDispType (0x13d5978): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetAnimalDispType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5978u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetAnimalDispType); A6_IN(608); const uint64_t a2_r = typed::AIDebug_GetAnimalDispType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetAnimalDispType_0x13d5978(rdram, ctx, runtime);
}
// AIDebug_GetAnimalDispID (0x13d5980): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetAnimalDispID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5980u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetAnimalDispID); A6_IN(609); const uint64_t a2_r = typed::AIDebug_GetAnimalDispID(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetAnimalDispID_0x13d5980(rdram, ctx, runtime);
}
// AIDebug_GetAdvertiseLoopFlag (0x13d5988): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetAdvertiseLoopFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5988u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetAdvertiseLoopFlag); A6_IN(610); const uint64_t a2_r = typed::AIDebug_GetAdvertiseLoopFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetAdvertiseLoopFlag_0x13d5988(rdram, ctx, runtime);
}
// AIDebug_GetAdvertisePauseFlag (0x13d5990): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_GetAdvertisePauseFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5990u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_GetAdvertisePauseFlag); A6_IN(611); const uint64_t a2_r = typed::AIDebug_GetAdvertisePauseFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_GetAdvertisePauseFlag_0x13d5990(rdram, ctx, runtime);
}
// AIDebug_SetAdvertisePauseFlag (0x13d5998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_SetAdvertisePauseFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D5998u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_SetAdvertisePauseFlag); A6_IN(612); typed::AIDebug_SetAdvertisePauseFlag(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_SetAdvertisePauseFlag_0x13d5998(rdram, ctx, runtime);
}
// getDebugAdvertiseLoopFlag (0x13d59a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugAdvertiseLoopFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugAdvertiseLoopFlag); A6_IN(613); const uint64_t a2_r = typed::getDebugAdvertiseLoopFlag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugAdvertiseLoopFlag_0x13d59a0(rdram, ctx, runtime);
}
// getDebugAnimalMode (0x13d59a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugAnimalMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugAnimalMode); A6_IN(614); const uint64_t a2_r = typed::getDebugAnimalMode(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugAnimalMode_0x13d59a8(rdram, ctx, runtime);
}
// getDebugAnimalDispType (0x13d59b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugAnimalDispType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugAnimalDispType); A6_IN(615); const uint64_t a2_r = typed::getDebugAnimalDispType(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugAnimalDispType_0x13d59b0(rdram, ctx, runtime);
}
// getDebugAnimalDispID (0x13d59b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDebugAnimalDispID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getDebugAnimalDispID); A6_IN(616); const uint64_t a2_r = typed::getDebugAnimalDispID(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDebugAnimalDispID_0x13d59b8(rdram, ctx, runtime);
}
// AIDebug_AdvertisePauseKey (0x13d59c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AIDebug_AdvertisePauseKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(AIDebug_AdvertisePauseKey); A6_IN(617); typed::AIDebug_AdvertisePauseKey(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AIDebug_AdvertisePauseKey_0x13d59c0(rdram, ctx, runtime);
}
// gamecore_sub_013D7870 (0x13d7870): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013D7870(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D7870u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(gamecore_sub_013D7870, a2_4, a2_5); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[0])}; A6_IN(618); const uint64_t a2_r = typed::gamecore_sub_013D7870(rdram, runtime, a2_4, a2_5, a4_vf5, a4_vf6, a4_vf7, a4_h2, a4_h3, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013D7870_0x13d7870(rdram, ctx, runtime);
}
// Kite_SetKiteObj (0x13e7cb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Kite_SetKiteObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13E7CB8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(Kite_SetKiteObj, a2_4, a2_5); A6_IN(619); typed::Kite_SetKiteObj(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Kite_SetKiteObj_0x13e7cb8(rdram, ctx, runtime);
}
// lizard_head (0x13ee468): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lizard_head(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13EE468u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lizard_head); A6_IN(620); typed::lizard_head(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lizard_head_0x13ee468(rdram, ctx, runtime);
}
// lizard_waist (0x13ee490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lizard_waist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13EE490u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lizard_waist); A6_IN(621); typed::lizard_waist(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lizard_waist_0x13ee490(rdram, ctx, runtime);
}
// lizard_berry (0x13ee4b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void lizard_berry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13EE4B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(lizard_berry); A6_IN(622); typed::lizard_berry(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::lizard_berry_0x13ee4b8(rdram, ctx, runtime);
}
// getSObjNodeBySubObj (0x13f0dd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSObjNodeBySubObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F0DD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getSObjNodeBySubObj, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a3_xo[6] = {Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9), Ps2GprU64(ctx, 10), Ps2GprU64(ctx, 11)}; A6_IN(623); const uint32_t a2_r = typed::getSObjNodeBySubObj(rdram, runtime, a2_4, a4_h2, a4_h3, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 6, a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); Ps2SetGpr64(ctx, 10, a3_xo[4]); Ps2SetGpr64(ctx, 11, a3_xo[5]); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSObjNodeBySubObj_0x13f0dd8(rdram, ctx, runtime);
}
// setSObjNodeLock (0x13f0e50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setSObjNodeLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F0E50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setSObjNodeLock, a2_4, a2_5); A6_IN(624); typed::setSObjNodeLock(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setSObjNodeLock_0x13f0e50(rdram, ctx, runtime);
}
// setLoadingCheckFunc (0x13f62d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setLoadingCheckFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F62D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setLoadingCheckFunc, a2_4, a2_5); A6_IN(625); const uint32_t a2_r = typed::setLoadingCheckFunc(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setLoadingCheckFunc_0x13f62d8(rdram, ctx, runtime);
}
// setPlayerStatusDisableFadeCompareSwitch (0x13fa540): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setPlayerStatusDisableFadeCompareSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FA540u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setPlayerStatusDisableFadeCompareSwitch, a2_4); A6_IN(626); const uint32_t a2_r = typed::setPlayerStatusDisableFadeCompareSwitch(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setPlayerStatusDisableFadeCompareSwitch_0x13fa540(rdram, ctx, runtime);
}
// getMinAmbient (0x13ff158): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAmbient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF158u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAmbient); A6_IN(627); const uint32_t a2_r = typed::getMinAmbient(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAmbient_0x13ff158(rdram, ctx, runtime);
}
// getMaxAmbient (0x13ff168): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAmbient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF168u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAmbient); A6_IN(628); const uint32_t a2_r = typed::getMaxAmbient(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAmbient_0x13ff168(rdram, ctx, runtime);
}
// getMinAmbientLine (0x13ff178): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAmbientLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF178u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAmbientLine); A6_IN(629); const uint32_t a2_r = typed::getMinAmbientLine(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAmbientLine_0x13ff178(rdram, ctx, runtime);
}
// getMaxAmbientLine (0x13ff188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAmbientLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF188u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAmbientLine); A6_IN(630); const uint32_t a2_r = typed::getMaxAmbientLine(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAmbientLine_0x13ff188(rdram, ctx, runtime);
}
// getMinAngle (0x13ff198): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF198u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAngle); A6_IN(631); const uint32_t a2_r = typed::getMinAngle(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAngle_0x13ff198(rdram, ctx, runtime);
}
// getMaxAngle (0x13ff1a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAngle); A6_IN(632); const uint32_t a2_r = typed::getMaxAngle(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAngle_0x13ff1a8(rdram, ctx, runtime);
}
// getMaxLineAngle (0x13ff1b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxLineAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxLineAngle); A6_IN(633); const uint32_t a2_r = typed::getMaxLineAngle(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxLineAngle_0x13ff1b8(rdram, ctx, runtime);
}
// getMinAmbientWeak (0x13ff1c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAmbientWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAmbientWeak); A6_IN(634); const uint32_t a2_r = typed::getMinAmbientWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAmbientWeak_0x13ff1c8(rdram, ctx, runtime);
}
// getMaxAmbientWeak (0x13ff1d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAmbientWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAmbientWeak); A6_IN(635); const uint32_t a2_r = typed::getMaxAmbientWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAmbientWeak_0x13ff1d8(rdram, ctx, runtime);
}
// getMinAmbientLineWeak (0x13ff1e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAmbientLineWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAmbientLineWeak); A6_IN(636); const uint32_t a2_r = typed::getMinAmbientLineWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAmbientLineWeak_0x13ff1e8(rdram, ctx, runtime);
}
// getMaxAmbientLineWeak (0x13ff1f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAmbientLineWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF1F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAmbientLineWeak); A6_IN(637); const uint32_t a2_r = typed::getMaxAmbientLineWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAmbientLineWeak_0x13ff1f8(rdram, ctx, runtime);
}
// getMinAngleWeak (0x13ff208): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMinAngleWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF208u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMinAngleWeak); A6_IN(638); const uint32_t a2_r = typed::getMinAngleWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMinAngleWeak_0x13ff208(rdram, ctx, runtime);
}
// getMaxAngleWeak (0x13ff218): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxAngleWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF218u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxAngleWeak); A6_IN(639); const uint32_t a2_r = typed::getMaxAngleWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxAngleWeak_0x13ff218(rdram, ctx, runtime);
}
// getMaxLineAngleWeak (0x13ff228): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxLineAngleWeak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF228u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxLineAngleWeak); A6_IN(640); const uint32_t a2_r = typed::getMaxLineAngleWeak(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxLineAngleWeak_0x13ff228(rdram, ctx, runtime);
}
// getMaxFogMulVal (0x13ff238): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getMaxFogMulVal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF238u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getMaxFogMulVal); A6_IN(641); const uint32_t a2_r = typed::getMaxFogMulVal(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getMaxFogMulVal_0x13ff238(rdram, ctx, runtime);
}
// getFogChangeSpeed (0x13ff248): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getFogChangeSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF248u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getFogChangeSpeed); A6_IN(642); const uint32_t a2_r = typed::getFogChangeSpeed(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getFogChangeSpeed_0x13ff248(rdram, ctx, runtime);
}
// getSwordLineWidthMax (0x13ff258): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSwordLineWidthMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF258u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSwordLineWidthMax); A6_IN(643); const uint32_t a2_r = typed::getSwordLineWidthMax(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSwordLineWidthMax_0x13ff258(rdram, ctx, runtime);
}
// getSwordLineChangeSpeed (0x13ff268): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSwordLineChangeSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF268u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSwordLineChangeSpeed); A6_IN(644); const uint32_t a2_r = typed::getSwordLineChangeSpeed(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSwordLineChangeSpeed_0x13ff268(rdram, ctx, runtime);
}
// getSwordLlineChangeThreshold (0x13ff278): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSwordLlineChangeThreshold(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF278u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSwordLlineChangeThreshold); A6_IN(645); const uint32_t a2_r = typed::getSwordLlineChangeThreshold(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSwordLlineChangeThreshold_0x13ff278(rdram, ctx, runtime);
}
// getBossHPRatioBase (0x13ff288): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBossHPRatioBase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF288u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getBossHPRatioBase); A6_IN(646); const uint32_t a2_r = typed::getBossHPRatioBase(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBossHPRatioBase_0x13ff288(rdram, ctx, runtime);
}
// getSwordGlowDistMin (0x13ff348): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSwordGlowDistMin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF348u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSwordGlowDistMin); A6_IN(647); const uint32_t a2_r = typed::getSwordGlowDistMin(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSwordGlowDistMin_0x13ff348(rdram, ctx, runtime);
}
// getSwordGlowDistMax (0x13ff358): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSwordGlowDistMax(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF358u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getSwordGlowDistMax); A6_IN(648); const uint32_t a2_r = typed::getSwordGlowDistMax(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSwordGlowDistMax_0x13ff358(rdram, ctx, runtime);
}
// getCollisionGlowSpeed (0x13ff368): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionGlowSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF368u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionGlowSpeed); A6_IN(649); const uint32_t a2_r = typed::getCollisionGlowSpeed(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionGlowSpeed_0x13ff368(rdram, ctx, runtime);
}
// getCollisionGlowTime (0x13ff378): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionGlowTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF378u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionGlowTime); A6_IN(650); const uint32_t a2_r = typed::getCollisionGlowTime(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionGlowTime_0x13ff378(rdram, ctx, runtime);
}
// getCollisionPosOffset (0x13ff388): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getCollisionPosOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF388u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getCollisionPosOffset); A6_IN(651); const uint32_t a2_r = typed::getCollisionPosOffset(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getCollisionPosOffset_0x13ff388(rdram, ctx, runtime);
}
// gamecore_sub_013FF398 (0x13ff398): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FF398(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF398u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(gamecore_sub_013FF398, a2_4); A6_IN(652); typed::gamecore_sub_013FF398(rdram, runtime, a2_4); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013FF398_0x13ff398(rdram, ctx, runtime);
}
// gamecore_sub_013FF3A0 (0x13ff3a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013FF3A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF3A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(gamecore_sub_013FF3A0, a2_4, a2_5); A6_IN(653); const uint64_t a2_r = typed::gamecore_sub_013FF3A0(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013FF3A0_0x13ff3a0(rdram, ctx, runtime);
}
// timeAttackTimeScriptEnd (0x13ff560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void timeAttackTimeScriptEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13FF560u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(timeAttackTimeScriptEnd); A6_IN(654); const a1::RetVF a2_r = typed::timeAttackTimeScriptEnd(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::timeAttackTimeScriptEnd_0x13ff560(rdram, ctx, runtime);
}
// GetTimeAttackNextPhase (0x1402078): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimeAttackNextPhase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402078u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetTimeAttackNextPhase); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A6_IN(655); const uint32_t a2_r = typed::GetTimeAttackNextPhase(rdram, runtime, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetTimeAttackNextPhase_0x1402078(rdram, ctx, runtime);
}
// GetTimeAttackTimer (0x14020c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimeAttackTimer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14020C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetTimeAttackTimer); A6_IN(656); const a1::RetVF a2_r = typed::GetTimeAttackTimer(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetTimeAttackTimer_0x14020c8(rdram, ctx, runtime);
}
// SetTimeAttackTimeFlag (0x14020d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetTimeAttackTimeFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14020D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetTimeAttackTimeFlag, a2_4); A6_IN(657); const uint32_t a2_r = typed::SetTimeAttackTimeFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetTimeAttackTimeFlag_0x14020d8(rdram, ctx, runtime);
}
// GetTimeAttackTimeLimit (0x14020e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetTimeAttackTimeLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14020E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetTimeAttackTimeLimit); A6_IN(658); const a1::RetVF a2_r = typed::GetTimeAttackTimeLimit(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetTimeAttackTimeLimit_0x14020e8(rdram, ctx, runtime);
}
// GetReplayNextPhase (0x1402178): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetReplayNextPhase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402178u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(GetReplayNextPhase); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A6_IN(659); const uint32_t a2_r = typed::GetReplayNextPhase(rdram, runtime, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetReplayNextPhase_0x1402178(rdram, ctx, runtime);
}
// CloseTimeAttackMenu (0x1402420): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CloseTimeAttackMenu(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1402420u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(CloseTimeAttackMenu, a2_4); A6_IN(660); const uint64_t a2_r = typed::CloseTimeAttackMenu(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CloseTimeAttackMenu_0x1402420(rdram, ctx, runtime);
}
// getWeaponName (0x1407320): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getWeaponName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1407320u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getWeaponName, a2_4); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(661); const uint32_t a2_r = typed::getWeaponName(rdram, runtime, a2_4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getWeaponName_0x1407320(rdram, ctx, runtime);
}
// deleteSavePoint (0x140ae20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteSavePoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE20u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteSavePoint); A6_IN(662); typed::deleteSavePoint(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteSavePoint_0x140ae20(rdram, ctx, runtime);
}
// SetMapLogDispMode (0x140ae28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetMapLogDispMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(SetMapLogDispMode, a2_4); A6_IN(663); const uint32_t a2_r = typed::SetMapLogDispMode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetMapLogDispMode_0x140ae28(rdram, ctx, runtime);
}
// getLogLineColorR (0x140ae68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLogLineColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLogLineColorR); A6_IN(664); const uint32_t a2_r = typed::getLogLineColorR(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLogLineColorR_0x140ae68(rdram, ctx, runtime);
}
// getLogLineColorG (0x140ae78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLogLineColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLogLineColorG); A6_IN(665); const uint32_t a2_r = typed::getLogLineColorG(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLogLineColorG_0x140ae78(rdram, ctx, runtime);
}
// getLogLineColorB (0x140ae88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLogLineColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE88u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLogLineColorB); A6_IN(666); const uint32_t a2_r = typed::getLogLineColorB(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLogLineColorB_0x140ae88(rdram, ctx, runtime);
}
// getLogLineColorA (0x140ae98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLogLineColorA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140AE98u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(getLogLineColorA); A6_IN(667); const uint32_t a2_r = typed::getLogLineColorA(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLogLineColorA_0x140ae98(rdram, ctx, runtime);
}
// gamecore_sub_0140DAD8 (0x140dad8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0140DAD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140DAD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_0140DAD8); A6_IN(668); typed::gamecore_sub_0140DAD8(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0140DAD8_0x140dad8(rdram, ctx, runtime);
}
// begin_time_interp_camera_dist_to_look_at (0x1411460): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void begin_time_interp_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1411460u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(begin_time_interp_camera_dist_to_look_at, a2_4, a2_f12, a2_f13); A6_IN(669); typed::begin_time_interp_camera_dist_to_look_at(rdram, runtime, a2_4, a2_f12, a2_f13); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::begin_time_interp_camera_dist_to_look_at_0x1411460(rdram, ctx, runtime);
}
// set_stretch_phase_ratio_camera_dist_to_look_at (0x1411470): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_stretch_phase_ratio_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1411470u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(set_stretch_phase_ratio_camera_dist_to_look_at, a2_4, a2_f12); A6_IN(670); typed::set_stretch_phase_ratio_camera_dist_to_look_at(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_stretch_phase_ratio_camera_dist_to_look_at_0x1411470(rdram, ctx, runtime);
}
// get_stretch_phase_ratio_camera_dist_to_look_at (0x1411478): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_stretch_phase_ratio_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1411478u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_stretch_phase_ratio_camera_dist_to_look_at, a2_4); A6_IN(671); const float a2_r = typed::get_stretch_phase_ratio_camera_dist_to_look_at(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_stretch_phase_ratio_camera_dist_to_look_at_0x1411478(rdram, ctx, runtime);
}
// get_end_interp_finish_time_camera_pause_rise (0x1412a90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_end_interp_finish_time_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1412A90u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_end_interp_finish_time_camera_pause_rise); A6_IN(674); const a1::RetVF a2_r = typed::get_end_interp_finish_time_camera_pause_rise(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_end_interp_finish_time_camera_pause_rise_0x1412a90(rdram, ctx, runtime);
}
// get_cam_region_col_attr_camera_place_status (0x14134a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cam_region_col_attr_camera_place_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14134A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_cam_region_col_attr_camera_place_status, a2_4, a2_5); A6_IN(675); const uint64_t a2_r = typed::get_cam_region_col_attr_camera_place_status(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cam_region_col_attr_camera_place_status_0x14134a0(rdram, ctx, runtime);
}
// is_exist_camera_target_lost_angle (0x1414e68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_exist_camera_target_lost_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1414E68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_exist_camera_target_lost_angle, a2_4); A6_IN(676); const uint32_t a2_r = typed::is_exist_camera_target_lost_angle(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_exist_camera_target_lost_angle_0x1414e68(rdram, ctx, runtime);
}
// is_exist_camera_target_visible_angle (0x14158b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_exist_camera_target_visible_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14158B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_exist_camera_target_visible_angle, a2_4); A6_IN(677); const uint32_t a2_r = typed::is_exist_camera_target_visible_angle(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_exist_camera_target_visible_angle_0x14158b8(rdram, ctx, runtime);
}
// CAM_is_reach_aim_yaw (0x1419898): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_is_reach_aim_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1419898u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(CAM_is_reach_aim_yaw, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15); A6_IN(678); const uint64_t a2_r = typed::CAM_is_reach_aim_yaw(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_is_reach_aim_yaw_0x1419898(rdram, ctx, runtime);
}
// CAM_is_reach_aim_pitch (0x1419938): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_is_reach_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1419938u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(CAM_is_reach_aim_pitch, a2_4, a2_5, a2_f12, a2_f14, a2_f15); A6_IN(679); const uint64_t a2_r = typed::CAM_is_reach_aim_pitch(rdram, runtime, a2_4, a2_5, a2_f12, a2_f14, a2_f15); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_is_reach_aim_pitch_0x1419938(rdram, ctx, runtime);
}
// CAM_set_clip_max_rot_vel_aim_rot_common (0x141a170): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_set_clip_max_rot_vel_aim_rot_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A170u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(CAM_set_clip_max_rot_vel_aim_rot_common, a2_4, a2_f12); A6_IN(680); const float a2_r = typed::CAM_set_clip_max_rot_vel_aim_rot_common(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_set_clip_max_rot_vel_aim_rot_common_0x141a170(rdram, ctx, runtime);
}
// CAM_set_yaw_shortest_rot_flag_aim_rot_common (0x141a188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_set_yaw_shortest_rot_flag_aim_rot_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A188u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(CAM_set_yaw_shortest_rot_flag_aim_rot_common, a2_4, a2_5); A6_IN(681); typed::CAM_set_yaw_shortest_rot_flag_aim_rot_common(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_set_yaw_shortest_rot_flag_aim_rot_common_0x141a188(rdram, ctx, runtime);
}
// CAM_is_inside_of_stop_region_aim_rot_common (0x141a1f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_is_inside_of_stop_region_aim_rot_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A1F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_is_inside_of_stop_region_aim_rot_common, a2_4); A6_IN(682); const uint32_t a2_r = typed::CAM_is_inside_of_stop_region_aim_rot_common(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_is_inside_of_stop_region_aim_rot_common_0x141a1f0(rdram, ctx, runtime);
}
// CAM_is_motion_kind_change_target (0x141a620): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_is_motion_kind_change_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A620u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_is_motion_kind_change_target, a2_4); A6_IN(683); const uint32_t a2_r = typed::CAM_is_motion_kind_change_target(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_is_motion_kind_change_target_0x141a620(rdram, ctx, runtime);
}
// CAM_motion_kind_act_auto_yaw_rot_target (0x141a668): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_motion_kind_act_auto_yaw_rot_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A668u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_motion_kind_act_auto_yaw_rot_target, a2_4); A6_IN(684); const uint32_t a2_r = typed::CAM_motion_kind_act_auto_yaw_rot_target(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_motion_kind_act_auto_yaw_rot_target_0x141a668(rdram, ctx, runtime);
}
// CAM_motion_kind_is_accept_follow_rot_motion_target (0x141a688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_motion_kind_is_accept_follow_rot_motion_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A688u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_motion_kind_is_accept_follow_rot_motion_target, a2_4); A6_IN(685); const uint32_t a2_r = typed::CAM_motion_kind_is_accept_follow_rot_motion_target(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_motion_kind_is_accept_follow_rot_motion_target_0x141a688(rdram, ctx, runtime);
}
// CAM_motion_kind_is_mode_lock_target (0x141a6e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_motion_kind_is_mode_lock_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A6E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_motion_kind_is_mode_lock_target, a2_4); A6_IN(686); const uint32_t a2_r = typed::CAM_motion_kind_is_mode_lock_target(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_motion_kind_is_mode_lock_target_0x141a6e0(rdram, ctx, runtime);
}
// CAM_is_use_obstacle_target_ofst_interp_coe (0x141b0f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_is_use_obstacle_target_ofst_interp_coe(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141B0F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(CAM_is_use_obstacle_target_ofst_interp_coe, a2_f12, a2_f13); A6_IN(687); const uint32_t a2_r = typed::CAM_is_use_obstacle_target_ofst_interp_coe(rdram, runtime, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_is_use_obstacle_target_ofst_interp_coe_0x141b0f8(rdram, ctx, runtime);
}
// set_camera_mode (0x141cd70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_camera_mode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CD70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(set_camera_mode, a2_4, a2_5); A6_IN(688); typed::set_camera_mode(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_camera_mode_0x141cd70(rdram, ctx, runtime);
}
// get_cur_camera_mode (0x141cd78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_camera_mode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CD78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_camera_mode, a2_4); A6_IN(689); const uint32_t a2_r = typed::get_cur_camera_mode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_camera_mode_0x141cd78(rdram, ctx, runtime);
}
// get_camera_mode_name (0x141cd80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_camera_mode_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CD80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(get_camera_mode_name, a2_4); A6_IN(690); const uint32_t a2_r = typed::get_camera_mode_name(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_camera_mode_name_0x141cd80(rdram, ctx, runtime);
}
// get_camera_target_group (0x141ce30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_camera_target_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CE30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_camera_target_group, a2_4); A6_IN(691); const uint32_t a2_r = typed::get_camera_target_group(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_camera_target_group_0x141ce30(rdram, ctx, runtime);
}
// get_camera_visible_collision_group (0x141ce40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_camera_visible_collision_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CE40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_camera_visible_collision_group, a2_4); A6_IN(692); const uint32_t a2_r = typed::get_camera_visible_collision_group(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_camera_visible_collision_group_0x141ce40(rdram, ctx, runtime);
}
// get_lws_animation_collision_group_cameraman (0x141ce50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_lws_animation_collision_group_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CE50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_lws_animation_collision_group_cameraman, a2_4); A6_IN(693); const uint32_t a2_r = typed::get_lws_animation_collision_group_cameraman(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_lws_animation_collision_group_cameraman_0x141ce50(rdram, ctx, runtime);
}
// get_game_camera_WV_matrix (0x141ce68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_game_camera_WV_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CE68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_game_camera_WV_matrix, a2_4); A6_IN(694); const uint32_t a2_r = typed::get_game_camera_WV_matrix(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_game_camera_WV_matrix_0x141ce68(rdram, ctx, runtime);
}
// is_game_camera_mode_now (0x141ce70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_game_camera_mode_now(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CE70u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_game_camera_mode_now, a2_4); A6_IN(695); const uint32_t a2_r = typed::is_game_camera_mode_now(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_game_camera_mode_now_0x141ce70(rdram, ctx, runtime);
}
// change_camera_target (0x141cea0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void change_camera_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CEA0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(change_camera_target, a2_4, a2_5); A6_IN(696); const uint64_t a2_r = typed::change_camera_target(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::change_camera_target_0x141cea0(rdram, ctx, runtime);
}
// get_main_cameraman_id (0x141d4b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_main_cameraman_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D4B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_main_cameraman_id); A6_IN(698); const uint32_t a2_r = typed::get_main_cameraman_id(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_main_cameraman_id_0x141d4b0(rdram, ctx, runtime);
}
// is_main_cameraman_exist (0x141d4c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_main_cameraman_exist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D4C0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(is_main_cameraman_exist); A6_IN(699); const uint32_t a2_r = typed::is_main_cameraman_exist(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_main_cameraman_exist_0x141d4c0(rdram, ctx, runtime);
}
// get_total_cameraman (0x141d4d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_total_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D4D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_total_cameraman); A6_IN(700); const uint32_t a2_r = typed::get_total_cameraman(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_total_cameraman_0x141d4d8(rdram, ctx, runtime);
}
// get_max_cameraman (0x141d4e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_max_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D4E8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_max_cameraman); A6_IN(701); const uint32_t a2_r = typed::get_max_cameraman(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_max_cameraman_0x141d4e8(rdram, ctx, runtime);
}
// get_debug_camera_view_angle (0x141e630): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_debug_camera_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E630u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_debug_camera_view_angle); A6_IN(702); const a1::RetVF a2_r = typed::get_debug_camera_view_angle(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_debug_camera_view_angle_0x141e630(rdram, ctx, runtime);
}
// get_debug_camera_view_angle_pointer (0x141e640): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_debug_camera_view_angle_pointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E640u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_debug_camera_view_angle_pointer); A6_IN(703); const uint32_t a2_r = typed::get_debug_camera_view_angle_pointer(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_debug_camera_view_angle_pointer_0x141e640(rdram, ctx, runtime);
}
// is_disp_game_camera_model_ok (0x141e650): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_disp_game_camera_model_ok(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E650u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(is_disp_game_camera_model_ok); A6_IN(704); const uint32_t a2_r = typed::is_disp_game_camera_model_ok(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_disp_game_camera_model_ok_0x141e650(rdram, ctx, runtime);
}
// is_disp_game_camera_model_ok_pointer (0x141e660): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_disp_game_camera_model_ok_pointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E660u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(is_disp_game_camera_model_ok_pointer); A6_IN(705); const uint32_t a2_r = typed::is_disp_game_camera_model_ok_pointer(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_disp_game_camera_model_ok_pointer_0x141e660(rdram, ctx, runtime);
}
// get_debug_camera_dist_to_look_at_pointer (0x141e670): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_debug_camera_dist_to_look_at_pointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E670u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_debug_camera_dist_to_look_at_pointer); A6_IN(706); const uint32_t a2_r = typed::get_debug_camera_dist_to_look_at_pointer(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_debug_camera_dist_to_look_at_pointer_0x141e670(rdram, ctx, runtime);
}
// set_debug_camera_move_diff (0x141e680): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void set_debug_camera_move_diff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141E680u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(set_debug_camera_move_diff, a2_f12, a2_f13); A6_IN(707); const uint32_t a2_r = typed::set_debug_camera_move_diff(rdram, runtime, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::set_debug_camera_move_diff_0x141e680(rdram, ctx, runtime);
}
// constructor_mode_common_camera_data (0x1424be8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void constructor_mode_common_camera_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1424BE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(constructor_mode_common_camera_data, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(708); const a1::RetVF a2_r = typed::constructor_mode_common_camera_data(rdram, runtime, a2_4, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::constructor_mode_common_camera_data_0x1424be8(rdram, ctx, runtime);
}
// get_look_at_position_game_camera (0x1424f68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_look_at_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1424F68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_look_at_position_game_camera, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(709); const uint64_t a2_r = typed::get_look_at_position_game_camera(rdram, runtime, a2_4, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_look_at_position_game_camera_0x1424f68(rdram, ctx, runtime);
}
// jack_game_camera (0x1424f78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void jack_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1424F78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(jack_game_camera, a2_4); A6_IN(710); const uint32_t a2_r = typed::jack_game_camera(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::jack_game_camera_0x1424f78(rdram, ctx, runtime);
}
// is_game_camera_standby_ok (0x1425020): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_game_camera_standby_ok(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425020u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(is_game_camera_standby_ok, a2_4); A6_IN(711); const uint32_t a2_r = typed::is_game_camera_standby_ok(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_game_camera_standby_ok_0x1425020(rdram, ctx, runtime);
}
// get_look_lws_anime_camera_flag (0x14250f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_look_lws_anime_camera_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14250F0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(get_look_lws_anime_camera_flag); A6_IN(712); const uint32_t a2_r = typed::get_look_lws_anime_camera_flag(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_look_lws_anime_camera_flag_0x14250f0(rdram, ctx, runtime);
}
// begin_check_and_boot_boss_subjective_camera_lws_animation (0x1425298): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void begin_check_and_boot_boss_subjective_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425298u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(begin_check_and_boot_boss_subjective_camera_lws_animation); A6_IN(713); typed::begin_check_and_boot_boss_subjective_camera_lws_animation(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::begin_check_and_boot_boss_subjective_camera_lws_animation_0x1425298(rdram, ctx, runtime);
}
// get_object_interest_position_game_camera (0x14253b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_object_interest_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14253B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_object_interest_position_game_camera, a2_4, a2_5); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(714); const uint32_t a2_r = typed::get_object_interest_position_game_camera(rdram, runtime, a2_4, a2_5, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_object_interest_position_game_camera_0x14253b0(rdram, ctx, runtime);
}
// get_cur_base_mode_condition_game_camera_param_manager (0x142c108): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_base_mode_condition_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x142C108u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_base_mode_condition_game_camera_param_manager, a2_4); A6_IN(715); const uint32_t a2_r = typed::get_cur_base_mode_condition_game_camera_param_manager(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_base_mode_condition_game_camera_param_manager_0x142c108(rdram, ctx, runtime);
}
// get_cur_base_mode_manager_game_camera_param_manager (0x142c110): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_base_mode_manager_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x142C110u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_base_mode_manager_game_camera_param_manager, a2_4); A6_IN(716); const uint32_t a2_r = typed::get_cur_base_mode_manager_game_camera_param_manager(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_base_mode_manager_game_camera_param_manager_0x142c110(rdram, ctx, runtime);
}
// gamecore_sub_0142C8B0 (0x142c8b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0142C8B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x142C8B0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint32_t a2_9 = Ps2GprU32(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const float a2_f12 = ctx->f[12]; A6_CHK(gamecore_sub_0142C8B0, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12); A6_IN(717); const a1::RetVF a2_r = typed::gamecore_sub_0142C8B0(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0142C8B0_0x142c8b0(rdram, ctx, runtime);
}
// setPassageCallBack (0x1431fd0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setPassageCallBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1431FD0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(setPassageCallBack, a2_4, a2_5, a2_6); A6_IN(718); typed::setPassageCallBack(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setPassageCallBack_0x1431fd0(rdram, ctx, runtime);
}
// useTempAttain (0x14327d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void useTempAttain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14327D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(useTempAttain, a2_4); A6_IN(719); const uint32_t a2_r = typed::useTempAttain(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::useTempAttain_0x14327d0(rdram, ctx, runtime);
}
// getWayForRoute (0x14327f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getWayForRoute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14327F8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getWayForRoute, a2_4); A6_IN(720); const uint32_t a2_r = typed::getWayForRoute(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getWayForRoute_0x14327f8(rdram, ctx, runtime);
}
// getLastAttainUpdate (0x1432800): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLastAttainUpdate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1432800u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getLastAttainUpdate, a2_4); A6_IN(721); const uint32_t a2_r = typed::getLastAttainUpdate(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLastAttainUpdate_0x1432800(rdram, ctx, runtime);
}
// forceAddPassageWork (0x14338a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void forceAddPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14338A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_8 = Ps2GprU32(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); A6_CHK(forceAddPassageWork, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(722); const uint32_t a2_r = typed::forceAddPassageWork(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::forceAddPassageWork_0x14338a0(rdram, ctx, runtime);
}
// getUserPathExMode (0x14359e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getUserPathExMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14359E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getUserPathExMode, a2_4); A6_IN(723); const uint32_t a2_r = typed::getUserPathExMode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getUserPathExMode_0x14359e0(rdram, ctx, runtime);
}
// getPPointPasasgeNum (0x1435a30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getPPointPasasgeNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1435A30u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getPPointPasasgeNum, a2_4); A6_IN(724); const uint64_t a2_r = typed::getPPointPasasgeNum(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getPPointPasasgeNum_0x1435a30(rdram, ctx, runtime);
}
// changedDPGroupID (0x1435b28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void changedDPGroupID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1435B28u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); A6_CHK(changedDPGroupID, a2_5, a2_6, a2_7); A6_IN(725); const uint64_t a2_r = typed::changedDPGroupID(rdram, runtime, a2_5, a2_6, a2_7); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::changedDPGroupID_0x1435b28(rdram, ctx, runtime);
}
// setGpCalcDist (0x1435b50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setGpCalcDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1435B50u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(setGpCalcDist, a2_4, a2_f12); A6_IN(726); typed::setGpCalcDist(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setGpCalcDist_0x1435b50(rdram, ctx, runtime);
}
// _getExecBaseMoveWork (0x1437b80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _getExecBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1437B80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(_getExecBaseMoveWork, a2_4); A6_IN(727); const uint32_t a2_r = typed::_getExecBaseMoveWork(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_getExecBaseMoveWork_0x1437b80(rdram, ctx, runtime);
}
// deleteBirdCtrlWork (0x143d550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x143D550u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteBirdCtrlWork); A6_IN(728); typed::deleteBirdCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteBirdCtrlWork_0x143d550(rdram, ctx, runtime);
}
// getLandMotionOffset (0x143d558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getLandMotionOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x143D558u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(getLandMotionOffset, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(729); const uint64_t a2_r = typed::getLandMotionOffset(rdram, runtime, a2_4, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getLandMotionOffset_0x143d558(rdram, ctx, runtime);
}
// setRouteAttackBirdCtrlWork (0x143d5b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setRouteAttackBirdCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x143D5B8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setRouteAttackBirdCtrlWork, a2_4); A6_IN(730); const uint32_t a2_r = typed::setRouteAttackBirdCtrlWork(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setRouteAttackBirdCtrlWork_0x143d5b8(rdram, ctx, runtime);
}
// gamecore_sub_0143D690 (0x143d690): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_0143D690(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x143D690u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A5_SP16(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); A6_CHK(gamecore_sub_0143D690, a2_4, a2_5, a2_6); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(731); const a1::RetVF a2_r = typed::gamecore_sub_0143D690(rdram, runtime, a2_4, a2_5, a2_6, a4_vf5, a4_vf6, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_0143D690_0x143d690(rdram, ctx, runtime);
}
// deleteFastLegCtrlWork (0x1440ee8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteFastLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1440EE8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteFastLegCtrlWork); A6_IN(732); typed::deleteFastLegCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteFastLegCtrlWork_0x1440ee8(rdram, ctx, runtime);
}
// getChkColWidth (0x1440f40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getChkColWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1440F40u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getChkColWidth, a2_4); A6_IN(733); const float a2_r = typed::getChkColWidth(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getChkColWidth_0x1440f40(rdram, ctx, runtime);
}
// chkGazeTool (0x1441a68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkGazeTool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1441A68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(chkGazeTool); A6_IN(734); const uint32_t a2_r = typed::chkGazeTool(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::chkGazeTool_0x1441a68(rdram, ctx, runtime);
}
// getGazeToolPos (0x1441a78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getGazeToolPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1441A78u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getGazeToolPos, a2_4); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(735); const uint32_t a2_r = typed::getGazeToolPos(rdram, runtime, a2_4, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getGazeToolPos_0x1441a78(rdram, ctx, runtime);
}
// deleteHorseCtrlWork (0x14444c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14444C8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteHorseCtrlWork); A6_IN(736); typed::deleteHorseCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteHorseCtrlWork_0x14444c8(rdram, ctx, runtime);
}
// deleteHumanCtrlWork (0x14454a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteHumanCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14454A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteHumanCtrlWork); A6_IN(737); typed::deleteHumanCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteHumanCtrlWork_0x14454a0(rdram, ctx, runtime);
}
// setDashHumanCtrlWork (0x14454a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setDashHumanCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14454A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setDashHumanCtrlWork, a2_4, a2_5); A6_IN(738); typed::setDashHumanCtrlWork(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setDashHumanCtrlWork_0x14454a8(rdram, ctx, runtime);
}
// gamecore_sub_014454E0 (0x14454e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_014454E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14454E0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(gamecore_sub_014454E0); A6_IN(739); typed::gamecore_sub_014454E0(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_014454E0_0x14454e0(rdram, ctx, runtime);
}
// chkMoveAngle2 (0x1446120): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkMoveAngle2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1446120u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(chkMoveAngle2, a2_f12, a2_f13); A6_IN(740); const a1::RetVF a2_r = typed::chkMoveAngle2(rdram, runtime, a2_f12, a2_f13); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::chkMoveAngle2_0x1446120(rdram, ctx, runtime);
}
// chkLeftPointForLineXZ (0x14466d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkLeftPointForLineXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14466D8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(chkLeftPointForLineXZ, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); A6_IN(741); const a1::RetVF a2_r = typed::chkLeftPointForLineXZ(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h7); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::chkLeftPointForLineXZ_0x14466d8(rdram, ctx, runtime);
}
// chkUpPointForLineYZ (0x14467a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void chkUpPointForLineYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14467A0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(chkUpPointForLineYZ, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); A6_IN(742); const a1::RetVF a2_r = typed::chkUpPointForLineYZ(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h7); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::chkUpPointForLineYZ_0x14467a0(rdram, ctx, runtime);
}
// setAttainMotScaleSlowLegCtrlWork (0x1449830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setAttainMotScaleSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1449830u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; A6_CHK(setAttainMotScaleSlowLegCtrlWork, a2_4, a2_f12); A6_IN(743); typed::setAttainMotScaleSlowLegCtrlWork(rdram, runtime, a2_4, a2_f12); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setAttainMotScaleSlowLegCtrlWork_0x1449830(rdram, ctx, runtime);
}
// deleteSlowLegCtrlWork (0x1449840): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteSlowLegCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1449840u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteSlowLegCtrlWork); A6_IN(744); typed::deleteSlowLegCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteSlowLegCtrlWork_0x1449840(rdram, ctx, runtime);
}
// deleteWormCtrlWork (0x144cc58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144CC58u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(deleteWormCtrlWork); A6_IN(745); typed::deleteWormCtrlWork(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteWormCtrlWork_0x144cc58(rdram, ctx, runtime);
}
// setMiddleTargetWormCtrlWork (0x144cd48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setMiddleTargetWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144CD48u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setMiddleTargetWormCtrlWork, a2_4, a2_5); A6_IN(746); typed::setMiddleTargetWormCtrlWork(rdram, runtime, a2_4, a2_5); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setMiddleTargetWormCtrlWork_0x144cd48(rdram, ctx, runtime);
}
// setAcceleParamWormCtrlWork (0x144cdd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setAcceleParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144CDD8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(setAcceleParamWormCtrlWork, a2_4, a2_f12, a2_f13); A6_IN(747); typed::setAcceleParamWormCtrlWork(rdram, runtime, a2_4, a2_f12, a2_f13); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setAcceleParamWormCtrlWork_0x144cdd8(rdram, ctx, runtime);
}
// getBuryingWormCtrlWork (0x144cdf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getBuryingWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x144CDF0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(getBuryingWormCtrlWork, a2_4); A6_IN(748); const uint32_t a2_r = typed::getBuryingWormCtrlWork(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getBuryingWormCtrlWork_0x144cdf0(rdram, ctx, runtime);
}
// setConditionResultBoss (0x1456378): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setConditionResultBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1456378u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setConditionResultBoss, a2_4); A6_IN(749); const uint32_t a2_r = typed::setConditionResultBoss(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setConditionResultBoss_0x1456378(rdram, ctx, runtime);
}
// setConditionResultHorse (0x1456388): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setConditionResultHorse(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1456388u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(setConditionResultHorse, a2_4); A6_IN(750); const uint32_t a2_r = typed::setConditionResultHorse(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setConditionResultHorse_0x1456388(rdram, ctx, runtime);
}
// SCRConditionResultBoss (0x1456398): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionResultBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1456398u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(SCRConditionResultBoss); A6_IN(751); const uint32_t a2_r = typed::SCRConditionResultBoss(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionResultBoss_0x1456398(rdram, ctx, runtime);
}
// SCRConditionResultHorse (0x14563a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionResultHorse(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14563A8u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(SCRConditionResultHorse); A6_IN(752); const uint32_t a2_r = typed::SCRConditionResultHorse(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionResultHorse_0x14563a8(rdram, ctx, runtime);
}
// pauseEventDemoManger (0x145ae80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void pauseEventDemoManger(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145AE80u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(pauseEventDemoManger, a2_4); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 5)}; A6_IN(753); const uint32_t a2_r = typed::pauseEventDemoManger(rdram, runtime, a2_4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 5, a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::pauseEventDemoManger_0x145ae80(rdram, ctx, runtime);
}
// forceSetPerceptionWork (0x145ca68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void forceSetPerceptionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145CA68u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(forceSetPerceptionWork, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 2)}; A6_IN(754); typed::forceSetPerceptionWork(rdram, runtime, a2_5, a4_h2, a3_xo); A6_OUT(); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 2, a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::forceSetPerceptionWork_0x145ca68(rdram, ctx, runtime);
}
// forceRenewLastPosPerceptionWork (0x145cab0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void forceRenewLastPosPerceptionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145CAB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(forceRenewLastPosPerceptionWork, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(755); const uint64_t a2_r = typed::forceRenewLastPosPerceptionWork(rdram, runtime, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::forceRenewLastPosPerceptionWork_0x145cab0(rdram, ctx, runtime);
}
// nico_BeardTest (0x145f118): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_BeardTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x145F118u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(nico_BeardTest); A6_IN(756); typed::nico_BeardTest(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_BeardTest_0x145f118(rdram, ctx, runtime);
}
// setGazeDirGazeIKWork (0x14617d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setGazeDirGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14617D0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setGazeDirGazeIKWork, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 2)}; A6_IN(757); typed::setGazeDirGazeIKWork(rdram, runtime, a2_4, a2_5, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setGazeDirGazeIKWork_0x14617d0(rdram, ctx, runtime);
}
// setWormIKAttainXVector (0x1464828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setWormIKAttainXVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464828u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(setWormIKAttainXVector, a2_4, a2_5); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(758); const uint64_t a2_r = typed::setWormIKAttainXVector(rdram, runtime, a2_4, a2_5, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setWormIKAttainXVector_0x1464828(rdram, ctx, runtime);
}
// SCRFuncSoundSetDepth (0x1464cb0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSoundSetDepth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464CB0u, 1))
    {
        if (PS2_EXPECT(A6_GO(), 1)) { A6_CHK(SCRFuncSoundSetDepth); A6_IN(759); typed::SCRFuncSoundSetDepth(rdram, runtime); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSoundSetDepth_0x1464cb0(rdram, ctx, runtime);
}
// _checkExistString (0x101b20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _checkExistString(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x101B20u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(_checkExistString, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(760); const uint64_t a2_r = typed::_checkExistString(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_checkExistString_0x101b20(rdram, ctx, runtime);
}
// sub_0010AAC8 (0x10aac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_0010AAC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AAC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128188u, ::memcmp_0x128188), 1)) { const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(sub_0010AAC8, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(761); const uint64_t a2_r = typed::sub_0010AAC8(rdram, runtime, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sub_0010AAC8_0x10aac8(rdram, ctx, runtime);
}
// sub_0010E338 (0x10e338): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_0010E338(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E338u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128188u, ::memcmp_0x128188), 1)) { const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(sub_0010E338, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(762); const uint64_t a2_r = typed::sub_0010E338(rdram, runtime, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sub_0010E338_0x10e338(rdram, ctx, runtime);
}
// unlink (0x1120f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void unlink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1120F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x127528u, lift_d1::ps2___errno), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); A6_CHK(unlink, a2_2); A6_IN(764); const uint32_t a2_r = typed::unlink(rdram, runtime, a2_2); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::unlink_0x1120f8(rdram, ctx, runtime);
}
// scePad2StateIntToStr (0x115888): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void scePad2StateIntToStr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x115888u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128FB0u, ::strcpy_0x128fb0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(scePad2StateIntToStr, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(765); const uint64_t a2_r = typed::scePad2StateIntToStr(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::scePad2StateIntToStr_0x115888(rdram, ctx, runtime);
}
// sub_001184E8 (0x1184e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_001184E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1184E8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128FB0u, ::strcpy_0x128fb0) && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(sub_001184E8, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(766); const uint64_t a2_r = typed::sub_001184E8(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sub_001184E8_0x1184e8(rdram, ctx, runtime);
}
// sub_00118578 (0x118578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void sub_00118578(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x118578u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128FB0u, ::strcpy_0x128fb0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(sub_00118578, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(767); const uint64_t a2_r = typed::sub_00118578(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::sub_00118578_0x118578(rdram, ctx, runtime);
}
// ps2___make_fp (0x1270a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2___make_fp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1270A8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x127390u, lift_d1::ps2___pack_f), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_7 = Ps2GprU32(ctx, 7); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(ps2___make_fp, a2_4, a2_5, a2_6, a2_7, a2_29); A6_IN(768); const a1::RetVF a2_r = typed::ps2___make_fp(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ps2___make_fp_0x1270a8(rdram, ctx, runtime);
}
// _setlocale_r (0x12e490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _setlocale_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12E490u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(_setlocale_r, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(769); const uint64_t a2_r = typed::_setlocale_r(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_setlocale_r_0x12e490(rdram, ctx, runtime);
}
// localeconv (0x12e558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void localeconv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12E558u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12E520u, lift_d1::_localeconv_r), 1)) { A6_CHK(localeconv); A6_IN(770); const uint32_t a2_r = typed::localeconv(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::localeconv_0x12e558(rdram, ctx, runtime);
}
// _b2d (0x12f3a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _b2d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12F3A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12EAF8u, lift_d1::_hi0bits), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(_b2d, a2_4, a2_5, a2_29); A6_IN(771); const uint64_t a2_r = typed::_b2d(rdram, runtime, a2_4, a2_5, a2_29); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_b2d_0x12f3a0(rdram, ctx, runtime);
}
// iosSrpWaitDMA (0x1b2a00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosSrpWaitDMA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B2A00u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2B98u, lift_d1::iosWaitDMA), 1)) { A6_CHK(iosSrpWaitDMA); A6_IN(772); const uint64_t a2_r = typed::iosSrpWaitDMA(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosSrpWaitDMA_0x1b2a00(rdram, ctx, runtime);
}
// iosChangeThreadPriority (0x1b3140): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosChangeThreadPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B3140u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x106ED0u, ::decomp_sema::getThreadIdNative), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosChangeThreadPriority, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(774); typed::iosChangeThreadPriority(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosChangeThreadPriority_0x1b3140(rdram, ctx, runtime);
}
// iosGetThreadPriority (0x1b3188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetThreadPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B3188u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x106ED0u, ::decomp_sema::getThreadIdNative), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosGetThreadPriority, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(775); const uint32_t a2_r = typed::iosGetThreadPriority(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosGetThreadPriority_0x1b3188(rdram, ctx, runtime);
}
// iosDumpMatrix (0x1b9b38): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosDumpMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1B9B38u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128FB0u, ::strcpy_0x128fb0) && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A6_HOLDS(0x1BE8E0u, lift_d1::iosDumpFVector), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosDumpMatrix, a2_2, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h20 = a4::hi64(ctx->r[20]); uint64_t a4_h21 = a4::hi64(ctx->r[21]); A6_IN(776); const uint64_t a2_r = typed::iosDumpMatrix(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a4_h6, a4_h20, a4_h21); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosDumpMatrix_0x1b9b38(rdram, ctx, runtime);
}
// iosFvecToICol (0x1ba3e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosFvecToICol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BA3E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BE718u, lift_d1::iosFTOI0Vector), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(iosFvecToICol, a2_4, a2_5, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; A6_IN(777); const uint32_t a2_r = typed::iosFvecToICol(rdram, runtime, a2_4, a2_5, a2_29, a4_vf5, a4_vf6); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosFvecToICol_0x1ba3e0(rdram, ctx, runtime);
}
// PushQuaternion (0x1bbb48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void PushQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BBB48u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BBC28u, lift_d1::InitQuaternionDrive), 1)) { A6_CHK(PushQuaternion); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5)}; A6_IN(778); const uint64_t a2_r = typed::PushQuaternion(rdram, runtime, a4_vf0, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::PushQuaternion_0x1bbb48(rdram, ctx, runtime);
}
// PushQuaternionWithNoCopy (0x1bbbb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void PushQuaternionWithNoCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BBBB8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BBC28u, lift_d1::InitQuaternionDrive), 1)) { A6_CHK(PushQuaternionWithNoCopy); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; A6_IN(779); const uint32_t a2_r = typed::PushQuaternionWithNoCopy(rdram, runtime, a4_vf0); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::PushQuaternionWithNoCopy_0x1bbbb8(rdram, ctx, runtime);
}
// MultiMatrixByQuaternion (0x1bbef8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MultiMatrixByQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BBEF8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B8CD0u, lift_d1::iosMulCurrentMatrixR), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(MultiMatrixByQuaternion, a2_4, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf1 = ctx->vu0_vf[1]; a4::vec4 a4_vf2 = ctx->vu0_vf[2]; a4::vec4 a4_vf3 = ctx->vu0_vf[3]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; A6_IN(780); const uint64_t a2_r = typed::MultiMatrixByQuaternion(rdram, runtime, a2_4, a2_29, a4_vf0, a4_vf1, a4_vf2, a4_vf3, a4_vf4, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[1] = a4_vf1; ctx->vu0_vf[2] = a4_vf2; ctx->vu0_vf[3] = a4_vf3; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::MultiMatrixByQuaternion_0x1bbef8(rdram, ctx, runtime);
}
// iosConvertStringSJIStoEUC (0x1c8d88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosConvertStringSJIStoEUC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C8D88u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosConvertStringSJIStoEUC, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h9 = a4::hi64(ctx->r[9]); uint64_t a4_h10 = a4::hi64(ctx->r[10]); A6_IN(781); const uint64_t a2_r = typed::iosConvertStringSJIStoEUC(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5, a4_h6, a4_h7, a4_h8, a4_h9, a4_h10); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[8] = a4::mk128(a4_h8, Ps2GprU64(ctx, 8)); ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->r[10] = a4::mk128(a4_h10, Ps2GprU64(ctx, 10)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosConvertStringSJIStoEUC_0x1c8d88(rdram, ctx, runtime);
}
// iosConvertStringEUCtoSJIS (0x1c8ec8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosConvertStringEUCtoSJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C8EC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A6_HOLDS(0x1C8B98u, lift_d1::kernel_sub_001C8B98), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosConvertStringEUCtoSJIS, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(782); const uint64_t a2_r = typed::iosConvertStringEUCtoSJIS(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosConvertStringEUCtoSJIS_0x1c8ec8(rdram, ctx, runtime);
}
// iosConvertStringEUCtoID (0x1c8f98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosConvertStringEUCtoID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C8F98u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A6_HOLDS(0x1C8B98u, lift_d1::kernel_sub_001C8B98), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(iosConvertStringEUCtoID, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); A6_IN(783); const uint64_t a2_r = typed::iosConvertStringEUCtoID(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h5, a4_h6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosConvertStringEUCtoID_0x1c8f98(rdram, ctx, runtime);
}
// iosGetFontDataWithEUC (0x1c9320): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetFontDataWithEUC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C9320u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C8B98u, lift_d1::kernel_sub_001C8B98), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(iosGetFontDataWithEUC, a2_2, a2_4, a2_5, a2_6, a2_29); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); A6_IN(784); const uint64_t a2_r = typed::iosGetFontDataWithEUC(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a4_h2, a4_h5, a4_h6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosGetFontDataWithEUC_0x1c9320(rdram, ctx, runtime);
}
// iosGetFontIDWithEUC (0x1c9550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetFontIDWithEUC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C9550u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C8B98u, lift_d1::kernel_sub_001C8B98), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(iosGetFontIDWithEUC, a2_2, a2_4, a2_29); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); A6_IN(785); const uint64_t a2_r = typed::iosGetFontIDWithEUC(rdram, runtime, a2_2, a2_4, a2_29, a4_h4, a4_h6, a4_h7); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosGetFontIDWithEUC_0x1c9550(rdram, ctx, runtime);
}
// _isysRecvSubMsg (0x1c9738): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void _isysRecvSubMsg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1C9738u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12821Cu, ::memcpy_0x12821c), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(_isysRecvSubMsg, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[2] = {Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A6_IN(786); const uint64_t a2_r = typed::_isysRecvSubMsg(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 7, a3_xo[0]); Ps2SetGpr64(ctx, 8, a3_xo[1]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::_isysRecvSubMsg_0x1c9738(rdram, ctx, runtime);
}
// getSlotInfo (0x1cb418): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getSlotInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CB418u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D8000u, lift_d1::Sg2SlotTbl), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(getSlotInfo, a2_4, a2_5); A6_IN(787); const uint32_t a2_r = typed::getSlotInfo(rdram, runtime, a2_4, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getSlotInfo_0x1cb418(rdram, ctx, runtime);
}
// iosGetIopTotalMemSize (0x1cb630): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosGetIopTotalMemSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CB630u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x102FE0u, lift_d1::LoaderSysCheckCDBootMode), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); A6_CHK(iosGetIopTotalMemSize, a2_2); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(788); const uint64_t a2_r = typed::iosGetIopTotalMemSize(rdram, runtime, a2_2, a4_h3, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::iosGetIopTotalMemSize_0x1cb630(rdram, ctx, runtime);
}
// Sg2ExStAdpcmSlotFree (0x1cc998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExStAdpcmSlotFree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CC998u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D5D28u, lift_d1::Sg2SlotFree), 1)) { A6_CHK(Sg2ExStAdpcmSlotFree); A6_IN(789); const uint64_t a2_r = typed::Sg2ExStAdpcmSlotFree(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2ExStAdpcmSlotFree_0x1cc998(rdram, ctx, runtime);
}
// Sg2ModSetOutputMode (0x1cf7b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ModSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CF7B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D7458u, lift_d1::Sg2SlotSetOutputMode), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(Sg2ModSetOutputMode, a2_4); A6_IN(790); const uint32_t a2_r = typed::Sg2ModSetOutputMode(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2ModSetOutputMode_0x1cf7b0(rdram, ctx, runtime);
}
// Sg2SepCheckSndOn (0x1d2bc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SepCheckSndOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D2BC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CEC38u, lift_d1::Sg2ModCheckSndOn), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(Sg2SepCheckSndOn, a2_4); A6_IN(791); const uint32_t a2_r = typed::Sg2SepCheckSndOn(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2SepCheckSndOn_0x1d2bc8(rdram, ctx, runtime);
}
// Sg2SmfpCheckSndOn (0x1d8920): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2SmfpCheckSndOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D8920u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CEC38u, lift_d1::Sg2ModCheckSndOn), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(Sg2SmfpCheckSndOn, a2_4); A6_IN(792); const uint32_t a2_r = typed::Sg2SmfpCheckSndOn(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2SmfpCheckSndOn_0x1d8920(rdram, ctx, runtime);
}
// Sg2StAdpcmIopReadAdr (0x1d9e50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2StAdpcmIopReadAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1D9E50u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D2170u, lift_d1::Sg2RemoteGetAdpStmAdr), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(Sg2StAdpcmIopReadAdr, a2_4); A6_IN(793); const uint32_t a2_r = typed::Sg2StAdpcmIopReadAdr(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2StAdpcmIopReadAdr_0x1d9e50(rdram, ctx, runtime);
}
// Sg2VabGetBdSize (0x1da4a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2VabGetBdSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA4A8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1DA0A0u, lift_d1::vab_check_alive), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(Sg2VabGetBdSize, a2_4); A6_IN(794); const uint64_t a2_r = typed::Sg2VabGetBdSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2VabGetBdSize_0x1da4a8(rdram, ctx, runtime);
}
// kernel_sub_001DA6F0 (0x1da6f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void kernel_sub_001DA6F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DA6F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1283D0u, ::memset_0x1283d0), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(kernel_sub_001DA6F0, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[1] = {Ps2GprU64(ctx, 6)}; A6_IN(795); const uint64_t a2_r = typed::kernel_sub_001DA6F0(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 6, a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::kernel_sub_001DA6F0_0x1da6f0(rdram, ctx, runtime);
}
// Sg2ExPcmCheckAutoFilterSeq (0x1dc1f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExPcmCheckAutoFilterSeq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DC1F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1DB3C8u, lift_d1::kernel_sub_001DB3C8), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(Sg2ExPcmCheckAutoFilterSeq, a2_4); A6_IN(796); const uint32_t a2_r = typed::Sg2ExPcmCheckAutoFilterSeq(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2ExPcmCheckAutoFilterSeq_0x1dc1f8(rdram, ctx, runtime);
}
// Sg2StPcmIopTransOffset (0x1dcd48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2StPcmIopTransOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1DCD48u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D2188u, lift_d1::Sg2RemoteGetPcmStmTransOffset), 1)) { A6_CHK(Sg2StPcmIopTransOffset); A6_IN(797); const uint32_t a2_r = typed::Sg2StPcmIopTransOffset(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2StPcmIopTransOffset_0x1dcd48(rdram, ctx, runtime);
}
// DEBUG_get_2d_font_disp_position (0x107c7c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_2d_font_disp_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C7C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A6_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A5_JAL(0x107C7F4u, 0x1C2038u) && A5_JAL(0x107C7FCu, 0x1C2038u) && A5_JAL(0x107C82Cu, 0x1C2048u) && A5_JAL(0x107C834u, 0x1C2048u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; A6_CHK(DEBUG_get_2d_font_disp_position, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22); uint64_t a3_xo[3] = {(uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22])}; A6_IN(798); const a1::RetVF a2_r = typed::DEBUG_get_2d_font_disp_position(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f20, a2_f21, a2_f22, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); ctx->f[22] = a1_flt((uint32_t)a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_get_2d_font_disp_position_0x107c7c8(rdram, ctx, runtime);
}
// DEBUG_get_2d_font_default_height_offset (0x107c888): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_2d_font_default_height_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C888u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A5_JAL(0x107C890u, 0x1C2048u), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); A6_CHK(DEBUG_get_2d_font_default_height_offset, a2_2); A6_IN(799); const a1::RetVF a2_r = typed::DEBUG_get_2d_font_default_height_offset(rdram, runtime, a2_2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_get_2d_font_default_height_offset_0x107c888(rdram, ctx, runtime);
}
// DEBUG_get_cntrl_mode_name (0x107c8c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_cntrl_mode_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C8C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(DEBUG_get_cntrl_mode_name, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(800); const uint64_t a2_r = typed::DEBUG_get_cntrl_mode_name(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_get_cntrl_mode_name_0x107c8c0(rdram, ctx, runtime);
}
// DEBUG_get_latest_motion_name (0x107c8f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_latest_motion_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C8F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(DEBUG_get_latest_motion_name, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(801); const uint64_t a2_r = typed::DEBUG_get_latest_motion_name(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_get_latest_motion_name_0x107c8f8(rdram, ctx, runtime);
}
// DEBUG_get_base_name (0x107c938): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DEBUG_get_base_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107C938u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1290C4u, ::strlen_0x1290c4) && A5_JAL(0x107C944u, 0x1290C4u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(DEBUG_get_base_name, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(802); const uint64_t a2_r = typed::DEBUG_get_base_name(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DEBUG_get_base_name_0x107c938(rdram, ctx, runtime);
}
// GAMEUTIL_is_between_yaw_and_yaw_extend_rel (0x107d4c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_between_yaw_and_yaw_extend_rel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107D4C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1080C28u, ::a6ad::GAMEUTIL_is_between_yaw_and_yaw_extend_abs) && A6_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A6_CHK(GAMEUTIL_is_between_yaw_and_yaw_extend_rel, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); A6_IN(803); const a1::RetVF a2_r = typed::GAMEUTIL_is_between_yaw_and_yaw_extend_rel(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_between_yaw_and_yaw_extend_rel_0x107d4c8(rdram, ctx, runtime);
}
// GAMEUTIL_get_new_velocity_by_brake (0x107fae0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_new_velocity_by_brake(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x107FAE0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x107FB30u, lift_d1::GAMEUTIL_get_new_velocity_by_brake_abs), 1)) { const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A6_CHK(GAMEUTIL_get_new_velocity_by_brake, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21); A6_IN(804); const float a2_r = typed::GAMEUTIL_get_new_velocity_by_brake(rdram, runtime, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_new_velocity_by_brake_0x107fae0(rdram, ctx, runtime);
}
// GAMEUTIL_get_limit_clip_yaw_abs (0x1080610): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_limit_clip_yaw_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1080610u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1080B28u, lift_d1::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2) && A6_HOLDS(0x1080C28u, ::a6ad::GAMEUTIL_is_between_yaw_and_yaw_extend_abs), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A6_CHK(GAMEUTIL_get_limit_clip_yaw_abs, a2_2, a2_4, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[20]), (uint64_t)a1_bits(ctx->f[21]), (uint64_t)a1_bits(ctx->f[22]), (uint64_t)a1_bits(ctx->f[23])}; A6_IN(805); const a1::RetVF a2_r = typed::GAMEUTIL_get_limit_clip_yaw_abs(rdram, runtime, a2_2, a2_4, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->f[21] = a1_flt((uint32_t)a3_xo[1]); ctx->f[22] = a1_flt((uint32_t)a3_xo[2]); ctx->f[23] = a1_flt((uint32_t)a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_limit_clip_yaw_abs_0x1080610(rdram, ctx, runtime);
}
// GAMEUTIL_get_difference_yaw_and_yaw_extend (0x10809b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_get_difference_yaw_and_yaw_extend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10809B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1080B98u, lift_d1::GAMEUTIL_is_between_yaw_and_yaw), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; A6_CHK(GAMEUTIL_get_difference_yaw_and_yaw_extend, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); A6_IN(806); const float a2_r = typed::GAMEUTIL_get_difference_yaw_and_yaw_extend(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_get_difference_yaw_and_yaw_extend_0x10809b0(rdram, ctx, runtime);
}
// GAMEUTIL_is_inside (0x1082a90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_is_inside(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1082A90u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BEB30u, lift_d1::iosGetExceptAxisVector) && A5_JAL(0x1082AD0u, 0x1BEB30u) && A5_JAL(0x1082AE4u, 0x1BEB30u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; A6_CHK(GAMEUTIL_is_inside, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(807); const a1::RetVF a2_r = typed::GAMEUTIL_is_inside(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f20, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_is_inside_0x1082a90(rdram, ctx, runtime);
}
// glNormal3f (0x108e560): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glNormal3f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E560u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9170u, lift_d1::iosApplyRotMatrix) && A5_JAL(0x108E5A0u, 0x1B9170u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; A6_CHK(glNormal3f, a2_29, a2_f12, a2_f13, a2_f14); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a3_xo[3] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6)}; A6_IN(808); const uint64_t a2_r = typed::glNormal3f(rdram, runtime, a2_29, a2_f12, a2_f13, a2_f14, a4_vf0, a4_vf5, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_vq, a4_vstat, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glNormal3f_0x108e560(rdram, ctx, runtime);
}
// glNormal3fv (0x108e5f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glNormal3fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E5F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9170u, lift_d1::iosApplyRotMatrix) && A5_JAL(0x108E64Cu, 0x1B9170u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(glNormal3fv, a2_4, a2_29); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(809); const a1::RetVF a2_r = typed::glNormal3fv(rdram, runtime, a2_4, a2_29, a4_vf0, a4_vf5, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_vq, a4_vstat); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glNormal3fv_0x108e5f0(rdram, ctx, runtime);
}
// glNormal4f (0x108e6a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glNormal4f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E6A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9170u, lift_d1::iosApplyRotMatrix) && A5_JAL(0x108E6E0u, 0x1B9170u), 1)) { const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; A6_CHK(glNormal4f, a2_29, a2_f12, a2_f13, a2_f14, a2_f15); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(810); const uint64_t a2_r = typed::glNormal4f(rdram, runtime, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a4_vf0, a4_vf5, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_vq, a4_vstat); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glNormal4f_0x108e6a0(rdram, ctx, runtime);
}
// glNormal4fv (0x108e730): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glNormal4fv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108E730u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9170u, lift_d1::iosApplyRotMatrix) && A5_JAL(0x108E760u, 0x1B9170u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(glNormal4fv, a2_4); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; A6_IN(811); const uint64_t a2_r = typed::glNormal4fv(rdram, runtime, a2_4, a4_vf0, a4_vf5, a4_vf7, a4_vf8, a4_vf10, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_vq, a4_vstat); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glNormal4fv_0x108e730(rdram, ctx, runtime);
}
// glLoadIdentity (0x108ed50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glLoadIdentity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108ED50u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B91A0u, lift_d1::iosUnitMatrix) && A5_JAL(0x108ED6Cu, 0x1B91A0u), 1)) { A6_CHK(glLoadIdentity); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; uint64_t a3_xo[1] = {Ps2GprU64(ctx, 2)}; A6_IN(812); typed::glLoadIdentity(rdram, runtime, a4_vf0, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a3_xo[0]); ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glLoadIdentity_0x108ed50(rdram, ctx, runtime);
}
// glGetMatrixf (0x108eda8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetMatrixf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108EDA8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9148u, ::decomp_copymatrix::copyMatrix) && A5_JAL(0x108EDCCu, 0x1B9148u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(glGetMatrixf, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h9 = a4::hi64(ctx->r[9]); A6_IN(813); typed::glGetMatrixf(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h6, a4_h7, a4_h8, a4_h9); A6_OUT(); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->r[8] = a4::mk128(a4_h8, Ps2GprU64(ctx, 8)); ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetMatrixf_0x108eda8(rdram, ctx, runtime);
}
// glPushMatrix (0x108edd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glPushMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108EDD8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9148u, ::decomp_copymatrix::copyMatrix) && A5_JAL(0x108EE30u, 0x1B9148u), 1)) { const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(glPushMatrix, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h9 = a4::hi64(ctx->r[9]); A6_IN(814); typed::glPushMatrix(rdram, runtime, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h6, a4_h7, a4_h8, a4_h9); A6_OUT(); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->r[8] = a4::mk128(a4_h8, Ps2GprU64(ctx, 8)); ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glPushMatrix_0x108edd8(rdram, ctx, runtime);
}
// glGetCurrentMatrix (0x108f3f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void glGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x108F3F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1B9148u, ::decomp_copymatrix::copyMatrix) && A5_JAL(0x108F414u, 0x1B9148u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(glGetCurrentMatrix, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h7 = a4::hi64(ctx->r[7]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a4_h9 = a4::hi64(ctx->r[9]); A6_IN(815); typed::glGetCurrentMatrix(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h6, a4_h7, a4_h8, a4_h9); A6_OUT(); ctx->r[6] = a4::mk128(a4_h6, Ps2GprU64(ctx, 6)); ctx->r[7] = a4::mk128(a4_h7, Ps2GprU64(ctx, 7)); ctx->r[8] = a4::mk128(a4_h8, Ps2GprU64(ctx, 8)); ctx->r[9] = a4::mk128(a4_h9, Ps2GprU64(ctx, 9)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::glGetCurrentMatrix_0x108f3f0(rdram, ctx, runtime);
}
// debugLineObjectCreateAttributre (0x1095998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void debugLineObjectCreateAttributre(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1095998u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BFC08u, lift_d1::iosAllocPacketPtrPath1) && A5_JAL(0x10959D8u, 0x1BFC08u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; A6_CHK(debugLineObjectCreateAttributre, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(818); const a1::RetVF a2_r = typed::debugLineObjectCreateAttributre(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f20, a2_f21, a2_f22, a2_f23, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::debugLineObjectCreateAttributre_0x1095998(rdram, ctx, runtime);
}
// modelResetLayer (0x10993e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void modelResetLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10993E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BFCF8u, lift_d1::iosDlChainTail) && A5_JAL(0x1099408u, 0x1BFCF8u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(modelResetLayer, a2_4); A6_IN(819); const uint64_t a2_r = typed::modelResetLayer(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::modelResetLayer_0x10993e0(rdram, ctx, runtime);
}
// setVertexPos (0x10a43c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setVertexPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10A43C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A6_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A5_JAL(0x10A4520u, 0x1C2038u) && A5_JAL(0x10A4528u, 0x1C2048u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; A6_CHK(setVertexPos, a2_4, a2_5, a2_6, a2_f12, a2_f13); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[0])}; A6_IN(820); const uint32_t a2_r = typed::setVertexPos(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setVertexPos_0x10a43c8(rdram, ctx, runtime);
}
// texScaleClutAlphaByName (0x10aa8a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texScaleClutAlphaByName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AA8A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A5_JAL(0x10AA928u, 0x128E6Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A6_CHK(texScaleClutAlphaByName, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(821); const uint64_t a2_r = typed::texScaleClutAlphaByName(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texScaleClutAlphaByName_0x10aa8a0(rdram, ctx, runtime);
}
// texScaleClutByName (0x10ab528): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void texScaleClutByName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10AB528u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A6_HOLDS(0x10AAA20u, lift_d1::texScaleClut) && A5_JAL(0x10AB5B0u, 0x128E6Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(texScaleClutByName, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(822); const uint64_t a2_r = typed::texScaleClutByName(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::texScaleClutByName_0x10ab528(rdram, ctx, runtime);
}
// get_animation_designer_node_quater_motion_ik_base (0x10b9590): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_animation_designer_node_quater_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B9590u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BBE10u, lift_d1::DivQuaternion) && A5_JAL(0x10B9634u, 0x1BBE10u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(get_animation_designer_node_quater_motion_ik_base, a2_4, a2_5); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(823); typed::get_animation_designer_node_quater_motion_ik_base(rdram, runtime, a2_4, a2_5, a4_vf0, a4_vf5, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_h3); A6_OUT(); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_animation_designer_node_quater_motion_ik_base_0x10b9590(rdram, ctx, runtime);
}
// manager_sub_010BA1F0 (0x10ba1f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_010BA1F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BA1F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1283D0u, ::memset_0x1283d0) && A6_HOLDS(0x10BA688u, ::a6ad::manager_sub_010BA688) && A5_JAL(0x10BA23Cu, 0x1283D0u) && A5_JAL(0x10BA250u, 0x1283D0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A6_CHK(manager_sub_010BA1F0, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[20])}; A6_IN(824); const a1::RetVF a2_r = typed::manager_sub_010BA1F0(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h3, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_010BA1F0_0x10ba1f0(rdram, ctx, runtime);
}
// SetCharacterClassMotionAddSyncMode (0x10bc1d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetCharacterClassMotionAddSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10BC1D0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1147A88u, lift_d1::AddTwoWayLinkNext) && A6_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(SetCharacterClassMotionAddSyncMode, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(826); const uint64_t a2_r = typed::SetCharacterClassMotionAddSyncMode(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetCharacterClassMotionAddSyncMode_0x10bc1d0(rdram, ctx, runtime);
}
// GetCharacterClassMotionInterpNodePosition (0x10c23d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCharacterClassMotionInterpNodePosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C23D8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10B63C0u, lift_d1::GetMotionOptionNodeParentId), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(GetCharacterClassMotionInterpNodePosition, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf18 = ctx->vu0_vf[18]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(827); const uint64_t a2_r = typed::GetCharacterClassMotionInterpNodePosition(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf0, a4_vf8, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf18, a4_vacc, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[18] = a4_vf18; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCharacterClassMotionInterpNodePosition_0x10c23d8(rdram, ctx, runtime);
}
// GetCharacterObjMotionShiftCallBackMaxEntry (0x10c27f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetCharacterObjMotionShiftCallBackMaxEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C27F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(GetCharacterObjMotionShiftCallBackMaxEntry, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(828); const uint64_t a2_r = typed::GetCharacterObjMotionShiftCallBackMaxEntry(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetCharacterObjMotionShiftCallBackMaxEntry_0x10c27f8(rdram, ctx, runtime);
}
// ResetCharacterObjMotionShiftCallBackEntryAll (0x10c2828): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ResetCharacterObjMotionShiftCallBackEntryAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2828u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(ResetCharacterObjMotionShiftCallBackEntryAll, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(829); const uint64_t a2_r = typed::ResetCharacterObjMotionShiftCallBackEntryAll(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ResetCharacterObjMotionShiftCallBackEntryAll_0x10c2828(rdram, ctx, runtime);
}
// InitSecureParentInfo (0x10c2e40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void InitSecureParentInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C2E40u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1147A78u, lift_d1::InitTwoWayLink), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(InitSecureParentInfo, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(831); typed::InitSecureParentInfo(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::InitSecureParentInfo_0x10c2e40(rdram, ctx, runtime);
}
// GetPleneFittingPointWithNormal (0x10e3f60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetPleneFittingPointWithNormal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10E3F60u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1134198u, lift_d1::GetPointDistanceFromPlane) && A5_SP16(), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f20 = ctx->f[20]; A6_CHK(GetPleneFittingPointWithNormal, a2_4, a2_5, a2_6, a2_7, a2_f20); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(833); const a1::RetVF a2_r = typed::GetPleneFittingPointWithNormal(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f20, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetPleneFittingPointWithNormal_0x10e3f60(rdram, ctx, runtime);
}
// GetLayoutClassObjectName (0x10f2490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLayoutClassObjectName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F2490u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1099750u, ::a6ad::modelGetName), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetLayoutClassObjectName, a2_4); A6_IN(835); const uint32_t a2_r = typed::GetLayoutClassObjectName(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLayoutClassObjectName_0x10f2490(rdram, ctx, runtime);
}
// GetAliasLayoutObjWorkSize (0x10f34a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetAliasLayoutObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F34A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1139128u, lift_d1::GetClothWorkSize), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetAliasLayoutObjWorkSize, a2_4); A6_IN(836); const uint64_t a2_r = typed::GetAliasLayoutObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetAliasLayoutObjWorkSize_0x10f34a0(rdram, ctx, runtime);
}
// SetVIF1MarkCallbackFunc (0x10f57f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetVIF1MarkCallbackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F57F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2A68u, lift_d1::iosSetSyncNoReturnFunc) && A5_JAL(0x10F5804u, 0x1C2A68u), 1)) { A6_CHK(SetVIF1MarkCallbackFunc); A6_IN(837); const uint32_t a2_r = typed::SetVIF1MarkCallbackFunc(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetVIF1MarkCallbackFunc_0x10f57f0(rdram, ctx, runtime);
}
// SetColorParamAmbientLight (0x10f8cc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetColorParamAmbientLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F8CC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BEB88u, lift_d1::iosCalcFColorToBrightness) && A5_JAL(0x10F8CE8u, 0x1BEB88u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(SetColorParamAmbientLight, a2_4, a2_5); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(838); const a1::RetVF a2_r = typed::SetColorParamAmbientLight(rdram, runtime, a2_4, a2_5, a4_vf5, a4_vf6, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetColorParamAmbientLight_0x10f8cc8(rdram, ctx, runtime);
}
// UpdateOutRangeLightGeo (0x110c878): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void UpdateOutRangeLightGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110C878u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10F8D60u, lift_d1::UpdateGlobalLightOutRange), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(UpdateOutRangeLightGeo, a2_4, a2_5, a2_6, a2_29); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(842); typed::UpdateOutRangeLightGeo(rdram, runtime, a2_4, a2_5, a2_6, a2_29, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vacc, a4_h2, a4_h3); A6_OUT(); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::UpdateOutRangeLightGeo_0x110c878(rdram, ctx, runtime);
}
// DeleteSubLinkObjLink (0x11145f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void DeleteSubLinkObjLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11145F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(DeleteSubLinkObjLink, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(845); const uint32_t a2_r = typed::DeleteSubLinkObjLink(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::DeleteSubLinkObjLink_0x11145f0(rdram, ctx, runtime);
}
// GetGeometryPointerCollisionSurfSTPosition (0x1128400): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetGeometryPointerCollisionSurfSTPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1128400u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1132D58u, lift_d1::CheckLegalCollisionGP), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(GetGeometryPointerCollisionSurfSTPosition, a2_2, a2_4, a2_5, a2_6); A6_IN(848); const uint64_t a2_r = typed::GetGeometryPointerCollisionSurfSTPosition(rdram, runtime, a2_2, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetGeometryPointerCollisionSurfSTPosition_0x1128400(rdram, ctx, runtime);
}
// RequestCollisionWideAreaSearch (0x112a5d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void RequestCollisionWideAreaSearch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x112A5D8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x11327B0u, lift_d1::CopyGeometryPointer), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(RequestCollisionWideAreaSearch, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(849); const uint64_t a2_r = typed::RequestCollisionWideAreaSearch(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::RequestCollisionWideAreaSearch_0x112a5d8(rdram, ctx, runtime);
}
// manager_sub_01141090 (0x1141090): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01141090(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1141090u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1141080u, lift_d1::manager_sub_01141080), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(manager_sub_01141090, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(851); const uint64_t a2_r = typed::manager_sub_01141090(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01141090_0x1141090(rdram, ctx, runtime);
}
// manager_sub_0114F998 (0x114f998): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_0114F998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x114F998u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x11561B0u, ::a6ad::windowCloseRequest), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(manager_sub_0114F998, a2_5); A6_IN(853); const uint32_t a2_r = typed::manager_sub_0114F998(rdram, runtime, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_0114F998_0x114f998(rdram, ctx, runtime);
}
// manager_sub_01153610 (0x1153610): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void manager_sub_01153610(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1153610u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x114AD38u, lift_d1::mouseSetCursorPosition), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(manager_sub_01153610, a2_4, a2_29); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 6)}; A6_IN(854); const uint64_t a2_r = typed::manager_sub_01153610(rdram, runtime, a2_4, a2_29, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 4, a3_xo[2]); Ps2SetGpr64(ctx, 6, a3_xo[3]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::manager_sub_01153610_0x1153610(rdram, ctx, runtime);
}
// windowSetCursorPos (0x1155fb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowSetCursorPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1155FB8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x114AD38u, lift_d1::mouseSetCursorPosition), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(windowSetCursorPos, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(855); const a1::RetVF a2_r = typed::windowSetCursorPos(rdram, runtime, a2_4, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowSetCursorPos_0x1155fb8(rdram, ctx, runtime);
}
// windowSetCursorPosOnWindow (0x1156050): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void windowSetCursorPosOnWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1156050u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1283D0u, ::memset_0x1283d0) && A6_HOLDS(0x114AD38u, lift_d1::mouseSetCursorPosition) && A5_JAL(0x115607Cu, 0x1283D0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(windowSetCursorPosOnWindow, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[3] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), Ps2GprU64(ctx, 6)}; A6_IN(856); const a1::RetVF a2_r = typed::windowSetCursorPosOnWindow(rdram, runtime, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::windowSetCursorPosOnWindow_0x1156050(rdram, ctx, runtime);
}
// ps2__SetMultiCastLetterReceiverInfo (0x1167038): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2__SetMultiCastLetterReceiverInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1167038u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12821Cu, ::memcpy_0x12821c) && A5_JAL(0x1167054u, 0x12821Cu), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(ps2__SetMultiCastLetterReceiverInfo, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(863); const uint64_t a2_r = typed::ps2__SetMultiCastLetterReceiverInfo(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ps2__SetMultiCastLetterReceiverInfo_0x1167038(rdram, ctx, runtime);
}
// ps2__SetLetterKindVariable (0x1167080): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ps2__SetLetterKindVariable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1167080u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12821Cu, ::memcpy_0x12821c) && A5_JAL(0x116708Cu, 0x12821Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(ps2__SetLetterKindVariable, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(865); const uint64_t a2_r = typed::ps2__SetLetterKindVariable(rdram, runtime, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ps2__SetLetterKindVariable_0x1167080(rdram, ctx, runtime);
}
// GetAnimationFogDefByName (0x11675a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetAnimationFogDefByName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x11675A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A5_JAL(0x1167664u, 0x128E6Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(GetAnimationFogDefByName, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(869); const uint64_t a2_r = typed::GetAnimationFogDefByName(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetAnimationFogDefByName_0x11675a0(rdram, ctx, runtime);
}
// convert_from_slide_bar_value_to_ratio (0x1342a48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void convert_from_slide_bar_value_to_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1342A48u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10836C0u, lift_d1::GAMEUTIL_get_dist_ratio) && A6_HOLDS(0x1083828u, lift_d1::GAMEUTIL_get_interp_value) && A5_JAL(0x1342A90u, 0x10836C0u) && A5_JAL(0x1342AB4u, 0x1083828u) && A5_JAL(0x1342AD8u, 0x10836C0u) && A5_JAL(0x1342AFCu, 0x1083828u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A6_CHK(convert_from_slide_bar_value_to_ratio, a2_4, a2_f12, a2_f13, a2_f20, a2_f21); A6_IN(870); const float a2_r = typed::convert_from_slide_bar_value_to_ratio(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f20, a2_f21); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::convert_from_slide_bar_value_to_ratio_0x1342a48(rdram, ctx, runtime);
}
// GAMEUTIL_add_data_record_info (0x13444b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GAMEUTIL_add_data_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13444B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x12821Cu, ::memcpy_0x12821c) && A5_JAL(0x1344540u, 0x12821Cu) && A5_JAL(0x134457Cu, 0x12821Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(GAMEUTIL_add_data_record_info, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(873); const uint64_t a2_r = typed::GAMEUTIL_add_data_record_info(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GAMEUTIL_add_data_record_info_0x13444b0(rdram, ctx, runtime);
}
// nico_deleteMyself (0x13452c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_deleteMyself(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13452C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13452DCu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(nico_deleteMyself, a2_4); A6_IN(874); const uint32_t a2_r = typed::nico_deleteMyself(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_deleteMyself_0x13452c8(rdram, ctx, runtime);
}
// getDemoBar (0x13462e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getDemoBar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13462E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134DBE8u, lift_d1::checkFadeParam), 1)) { A6_CHK(getDemoBar); A6_IN(875); const a1::RetVF a2_r = typed::getDemoBar(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getDemoBar_0x13462e0(rdram, ctx, runtime);
}
// update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl (0x13475a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13475A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1080B28u, lift_d1::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2) && A6_HOLDS(0x1080B90u, lift_d1::GAMEUTIL_get_move_radi_from_pitch1_to_pitch2) && A5_JAL(0x13475C0u, 0x1080B28u) && A5_JAL(0x13475D0u, 0x1080B90u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f20 = ctx->f[20]; A6_CHK(update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl, a2_4, a2_f12, a2_f13, a2_f20); A6_IN(876); const a1::RetVF a2_r = typed::update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f20); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl_0x13475a0(rdram, ctx, runtime);
}
// bossStatLayoutChk (0x134a2b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bossStatLayoutChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134A2B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F030u, lift_d1::_gameFlagChk), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(bossStatLayoutChk, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(879); const uint64_t a2_r = typed::bossStatLayoutChk(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bossStatLayoutChk_0x134a2b0(rdram, ctx, runtime);
}
// bossStatChkDeadBoss (0x134a368): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bossStatChkDeadBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134A368u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F030u, lift_d1::_gameFlagChk), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(bossStatChkDeadBoss, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(880); const uint64_t a2_r = typed::bossStatChkDeadBoss(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bossStatChkDeadBoss_0x134a368(rdram, ctx, runtime);
}
// nico_GetCharacterParentGPAttr (0x134c2c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_GetCharacterParentGPAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C2C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x134C2D4u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(nico_GetCharacterParentGPAttr, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(882); const uint64_t a2_r = typed::nico_GetCharacterParentGPAttr(rdram, runtime, a2_2, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_GetCharacterParentGPAttr_0x134c2c8(rdram, ctx, runtime);
}
// nico_CheckCharacterVColPlaneAttribute (0x134c350): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_CheckCharacterVColPlaneAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C350u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x134C364u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(nico_CheckCharacterVColPlaneAttribute, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(883); const uint64_t a2_r = typed::nico_CheckCharacterVColPlaneAttribute(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_CheckCharacterVColPlaneAttribute_0x134c350(rdram, ctx, runtime);
}
// nico_CheckCharacterGPColPlaneAttribute (0x134c3a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_CheckCharacterGPColPlaneAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134C3A8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x134C3BCu, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(nico_CheckCharacterGPColPlaneAttribute, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(884); const uint64_t a2_r = typed::nico_CheckCharacterGPColPlaneAttribute(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_CheckCharacterGPColPlaneAttribute_0x134c3a8(rdram, ctx, runtime);
}
// scrSetFadeColorBlack (0x134df50): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void scrSetFadeColorBlack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134DF50u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x108B970u, lift_d1::setFadeColor) && A5_JAL(0x134DF60u, 0x108B970u), 1)) { A6_CHK(scrSetFadeColorBlack); A6_IN(885); const uint32_t a2_r = typed::scrSetFadeColorBlack(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::scrSetFadeColorBlack_0x134df50(rdram, ctx, runtime);
}
// scrSetFadeColorWhite (0x134df68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void scrSetFadeColorWhite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x134DF68u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x108B970u, lift_d1::setFadeColor) && A5_JAL(0x134DF78u, 0x108B970u), 1)) { A6_CHK(scrSetFadeColorWhite); A6_IN(886); const uint32_t a2_r = typed::scrSetFadeColorWhite(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::scrSetFadeColorWhite_0x134df68(rdram, ctx, runtime);
}
// gameflowSetCaptionPause (0x1352440): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gameflowSetCaptionPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352440u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134BAF0u, ::a6ad::SetCaptionPauseFlag), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(gameflowSetCaptionPause, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(888); const uint32_t a2_r = typed::gameflowSetCaptionPause(rdram, runtime, a2_4, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gameflowSetCaptionPause_0x1352440(rdram, ctx, runtime);
}
// gamecore_sub_01352500 (0x1352500): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352500(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352500u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A6_HOLDS(0x1345CE0u, lift_d1::ReleaseGame) && A6_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x1352508u, 0x1CA1B0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_01352500, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(889); const uint64_t a2_r = typed::gamecore_sub_01352500(rdram, runtime, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352500_0x1352500(rdram, ctx, runtime);
}
// gamecore_sub_01352550 (0x1352550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01352550(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1352550u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A6_HOLDS(0x1345CE0u, lift_d1::ReleaseGame) && A6_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x1352558u, 0x1CA1B0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_01352550, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(890); const uint64_t a2_r = typed::gamecore_sub_01352550(rdram, runtime, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01352550_0x1352550(rdram, ctx, runtime);
}
// gamecore_sub_013525E0 (0x13525e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013525E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13525E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A6_HOLDS(0x1CAD90u, lift_d1::sndSepPauseAll) && A6_HOLDS(0x1157B78u, lift_d1::adpcmGamePause) && A6_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x13525E8u, 0x1CA1B0u) && A5_JAL(0x13525F8u, 0x1157B78u) && A5_JAL(0x1352600u, 0x1CAD90u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_013525E0, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(891); const uint64_t a2_r = typed::gamecore_sub_013525E0(rdram, runtime, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013525E0_0x13525e0(rdram, ctx, runtime);
}
// gamecore_sub_013527A0 (0x13527a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013527A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13527A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA1B0u, ::decomp_scriptvm3::getThisSubThread) && A6_HOLDS(0x13CFB60u, lift_d1::scrInit) && A5_JAL(0x13527A8u, 0x1CA1B0u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_013527A0, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(892); const uint64_t a2_r = typed::gamecore_sub_013527A0(rdram, runtime, a2_2, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013527A0_0x13527a0(rdram, ctx, runtime);
}
// gStatGobjDispGflagSet (0x1353638): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatGobjDispGflagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353638u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A6_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gStatGobjDispGflagSet, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(895); const uint64_t a2_r = typed::gStatGobjDispGflagSet(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatGobjDispGflagSet_0x1353638(rdram, ctx, runtime);
}
// gStatGobjDispGflagCls (0x1353680): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatGobjDispGflagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353680u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A6_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gStatGobjDispGflagCls, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(896); const uint64_t a2_r = typed::gStatGobjDispGflagCls(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatGobjDispGflagCls_0x1353680(rdram, ctx, runtime);
}
// gStatStartTimeAttack (0x13536e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatStartTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13536E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x14020D8u, ::a6ad::SetTimeAttackTimeFlag), 1)) { A6_CHK(gStatStartTimeAttack); A6_IN(897); const uint32_t a2_r = typed::gStatStartTimeAttack(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatStartTimeAttack_0x13536e0(rdram, ctx, runtime);
}
// gStatStopTimeAttack (0x1353708): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gStatStopTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353708u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x14020D8u, ::a6ad::SetTimeAttackTimeFlag), 1)) { A6_CHK(gStatStopTimeAttack); A6_IN(898); const uint32_t a2_r = typed::gStatStopTimeAttack(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gStatStopTimeAttack_0x1353708(rdram, ctx, runtime);
}
// gstatBootMcCurSetLastSaveOmakeStat (0x1353e70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gstatBootMcCurSetLastSaveOmakeStat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1353E70u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F278u, lift_d1::getGameClearCnt), 1)) { A6_CHK(gstatBootMcCurSetLastSaveOmakeStat); uint64_t a3_xo[5] = {Ps2GprU64(ctx, 4), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8)}; A6_IN(899); const uint32_t a2_r = typed::gstatBootMcCurSetLastSaveOmakeStat(rdram, runtime, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); Ps2SetGpr64(ctx, 4, a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 7, a3_xo[3]); Ps2SetGpr64(ctx, 8, a3_xo[4]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gstatBootMcCurSetLastSaveOmakeStat_0x1353e70(rdram, ctx, runtime);
}
// getNowPower (0x13587b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getNowPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13587B8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F120u, lift_d1::_gameFWorkChk), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f0 = ctx->f[0]; A6_CHK(getNowPower, a2_4, a2_f0); A6_IN(902); const a1::RetVF a2_r = typed::getNowPower(rdram, runtime, a2_4, a2_f0); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getNowPower_0x13587b8(rdram, ctx, runtime);
}
// stagemanager_checkstageAlreadyLoaded (0x13668d8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void stagemanager_checkstageAlreadyLoaded(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13668D8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x115EE68u, lift_d1::sheetMgrChkStageIndexResolve) && A5_JAL(0x13668E4u, 0x115EE68u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(stagemanager_checkstageAlreadyLoaded, a2_4); A6_IN(903); const uint64_t a2_r = typed::stagemanager_checkstageAlreadyLoaded(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::stagemanager_checkstageAlreadyLoaded_0x13668d8(rdram, ctx, runtime);
}
// bgmAutoStart (0x136a758): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void bgmAutoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136A758u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1283D0u, ::memset_0x1283d0) && A5_JAL(0x136A7D4u, 0x1283D0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(bgmAutoStart, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[0]), Ps2GprU64(ctx, 5), Ps2GprU64(ctx, 6), Ps2GprU64(ctx, 8)}; A6_IN(905); const uint64_t a2_r = typed::bgmAutoStart(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h4, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 5, a3_xo[1]); Ps2SetGpr64(ctx, 6, a3_xo[2]); Ps2SetGpr64(ctx, 8, a3_xo[3]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::bgmAutoStart_0x136a758(rdram, ctx, runtime);
}
// SCRBirdStartBreath (0x136ef58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRBirdStartBreath(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136EF58u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x136EF6Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRBirdStartBreath, a2_4); A6_IN(908); const uint32_t a2_r = typed::SCRBirdStartBreath(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRBirdStartBreath_0x136ef58(rdram, ctx, runtime);
}
// SCRBirdStopBreath (0x136ef78): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRBirdStopBreath(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136EF78u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x136EF8Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRBirdStopBreath, a2_4); A6_IN(909); const uint32_t a2_r = typed::SCRBirdStopBreath(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRBirdStopBreath_0x136ef78(rdram, ctx, runtime);
}
// SCRBirdStartFlap (0x136ef98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRBirdStartFlap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136EF98u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x136EFACu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRBirdStartFlap, a2_4); A6_IN(910); const uint32_t a2_r = typed::SCRBirdStartFlap(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRBirdStartFlap_0x136ef98(rdram, ctx, runtime);
}
// SCRBossDebugDeath (0x136ff80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRBossDebugDeath(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136FF80u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x136FF94u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRBossDebugDeath, a2_4); A6_IN(911); const uint32_t a2_r = typed::SCRBossDebugDeath(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRBossDebugDeath_0x136ff80(rdram, ctx, runtime);
}
// SCRMammothBreaksWall (0x136ffa0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMammothBreaksWall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x136FFA0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x136FFB4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMammothBreaksWall, a2_4); A6_IN(912); const uint32_t a2_r = typed::SCRMammothBreaksWall(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMammothBreaksWall_0x136ffa0(rdram, ctx, runtime);
}
// SCRBuddhaShoot (0x1379578): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRBuddhaShoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1379578u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x137958Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRBuddhaShoot, a2_4); A6_IN(914); const uint32_t a2_r = typed::SCRBuddhaShoot(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRBuddhaShoot_0x1379578(rdram, ctx, runtime);
}
// SCRCerberusPollFalldown (0x137b558): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCerberusPollFalldown(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137B558u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x137B56Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRCerberusPollFalldown, a2_4); A6_IN(915); const uint32_t a2_r = typed::SCRCerberusPollFalldown(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRCerberusPollFalldown_0x137b558(rdram, ctx, runtime);
}
// SCRResetCharacterObj4DDynamicsInfo (0x137e5e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRResetCharacterObj4DDynamicsInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x137E5E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x137E5F4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRResetCharacterObj4DDynamicsInfo, a2_4); A6_IN(917); const uint32_t a2_r = typed::SCRResetCharacterObj4DDynamicsInfo(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRResetCharacterObj4DDynamicsInfo_0x137e5e0(rdram, ctx, runtime);
}
// demo_PlayerDeathEffectStart (0x1389a48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void demo_PlayerDeathEffectStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1389A48u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1087118u, lift_d1::setBossDeathEffect) && A5_JAL(0x1389A68u, 0x1087118u), 1)) { A6_CHK(demo_PlayerDeathEffectStart); A6_IN(918); const a1::RetVF a2_r = typed::demo_PlayerDeathEffectStart(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::demo_PlayerDeathEffectStart_0x1389a48(rdram, ctx, runtime);
}
// demo_PlayerDeathEffectEnd (0x1389a70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void demo_PlayerDeathEffectEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1389A70u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10871D8u, lift_d1::endBossDeathEffect) && A5_JAL(0x1389A7Cu, 0x10871D8u), 1)) { A6_CHK(demo_PlayerDeathEffectEnd); A6_IN(919); const uint32_t a2_r = typed::demo_PlayerDeathEffectEnd(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::demo_PlayerDeathEffectEnd_0x1389a70(rdram, ctx, runtime);
}
// get_cur_type_expression (0x138be80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_type_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138BE80u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1344650u, lift_d1::GAMEUTIL_get_cur_status_status_manager), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_type_expression, a2_4); A6_IN(920); const uint32_t a2_r = typed::get_cur_type_expression(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_type_expression_0x138be80(rdram, ctx, runtime);
}
// SCRRockFall (0x138c188): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRRockFall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138C188u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x138C19Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRRockFall, a2_4); A6_IN(921); const uint32_t a2_r = typed::SCRRockFall(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRRockFall_0x138c188(rdram, ctx, runtime);
}
// SCRRockReset (0x138c1a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRRockReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138C1A8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x138C1BCu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRRockReset, a2_4); A6_IN(922); const uint32_t a2_r = typed::SCRRockReset(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRRockReset_0x138c1a8(rdram, ctx, runtime);
}
// SCRGuardSwordPickup (0x138f8d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGuardSwordPickup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138F8D0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x138F8E4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRGuardSwordPickup, a2_4); A6_IN(923); const uint32_t a2_r = typed::SCRGuardSwordPickup(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRGuardSwordPickup_0x138f8d0(rdram, ctx, runtime);
}
// SCRGuardSwordPickupEnd (0x138f8f0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGuardSwordPickupEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x138F8F0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x138F904u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRGuardSwordPickupEnd, a2_4); A6_IN(924); const uint32_t a2_r = typed::SCRGuardSwordPickupEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRGuardSwordPickupEnd_0x138f8f0(rdram, ctx, runtime);
}
// SCRGuardArrowShoot (0x1390550): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGuardArrowShoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1390550u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x1390564u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRGuardArrowShoot, a2_4); A6_IN(925); const uint32_t a2_r = typed::SCRGuardArrowShoot(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRGuardArrowShoot_0x1390550(rdram, ctx, runtime);
}
// SCRHorseUnLoad (0x1396e00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHorseUnLoad(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1396E00u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x1396E14u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHorseUnLoad, a2_4); A6_IN(927); const uint32_t a2_r = typed::SCRHorseUnLoad(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHorseUnLoad_0x1396e00(rdram, ctx, runtime);
}
// nico_horseClothOverSurfDetectFunc (0x13970a0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_horseClothOverSurfDetectFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13970A0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x13970A8u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(nico_horseClothOverSurfDetectFunc, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(928); const uint64_t a2_r = typed::nico_horseClothOverSurfDetectFunc(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_horseClothOverSurfDetectFunc_0x13970a0(rdram, ctx, runtime);
}
// MotSpdCtrl_SetSpeed (0x139ac18): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void MotSpdCtrl_SetSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139AC18u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x139AC40u, 0x10C0A70u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A6_CHK(MotSpdCtrl_SetSpeed, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(929); const uint64_t a2_r = typed::MotSpdCtrl_SetSpeed(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::MotSpdCtrl_SetSpeed_0x139ac18(rdram, ctx, runtime);
}
// SCRKirinFallRockEnd (0x139cbf0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRKirinFallRockEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139CBF0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139CC04u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRKirinFallRockEnd, a2_4); A6_IN(930); const uint32_t a2_r = typed::SCRKirinFallRockEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRKirinFallRockEnd_0x139cbf0(rdram, ctx, runtime);
}
// SCRKnightBossPartsBroken (0x139d6b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRKnightBossPartsBroken(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139D6B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139D6C4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRKnightBossPartsBroken, a2_4); A6_IN(931); const uint32_t a2_r = typed::SCRKnightBossPartsBroken(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRKnightBossPartsBroken_0x139d6b0(rdram, ctx, runtime);
}
// SCRKnightBossPartsDelete (0x139d6d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRKnightBossPartsDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139D6D0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139D6E4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRKnightBossPartsDelete, a2_4); A6_IN(932); const uint32_t a2_r = typed::SCRKnightBossPartsDelete(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRKnightBossPartsDelete_0x139d6d0(rdram, ctx, runtime);
}
// SCRLeoTorch0_Disp (0x139e058): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoTorch0_Disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E058u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E06Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoTorch0_Disp, a2_4); A6_IN(933); const uint32_t a2_r = typed::SCRLeoTorch0_Disp(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoTorch0_Disp_0x139e058(rdram, ctx, runtime);
}
// SCRLeoTorch1_Disp (0x139e078): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoTorch1_Disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E078u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E08Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoTorch1_Disp, a2_4); A6_IN(934); const uint32_t a2_r = typed::SCRLeoTorch1_Disp(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoTorch1_Disp_0x139e078(rdram, ctx, runtime);
}
// SCRLeoTorch2_Disp (0x139e098): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoTorch2_Disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E098u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E0ACu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoTorch2_Disp, a2_4); A6_IN(935); const uint32_t a2_r = typed::SCRLeoTorch2_Disp(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoTorch2_Disp_0x139e098(rdram, ctx, runtime);
}
// SCRLeoTorch3_Disp (0x139e0b8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoTorch3_Disp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E0B8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E0CCu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoTorch3_Disp, a2_4); A6_IN(936); const uint32_t a2_r = typed::SCRLeoTorch3_Disp(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoTorch3_Disp_0x139e0b8(rdram, ctx, runtime);
}
// SCRLeoArmorBroken (0x139e100): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoArmorBroken(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E100u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E114u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoArmorBroken, a2_4); A6_IN(937); const uint32_t a2_r = typed::SCRLeoArmorBroken(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoArmorBroken_0x139e100(rdram, ctx, runtime);
}
// SCRLeoFall (0x139e120): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoFall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E120u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E134u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoFall, a2_4); A6_IN(938); const uint32_t a2_r = typed::SCRLeoFall(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoFall_0x139e120(rdram, ctx, runtime);
}
// SCRLeoDamage (0x139e140): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E140u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E154u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoDamage, a2_4); A6_IN(939); const uint32_t a2_r = typed::SCRLeoDamage(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoDamage_0x139e140(rdram, ctx, runtime);
}
// SCRLeoDashAttackEnd (0x139e160): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoDashAttackEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E160u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E174u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoDashAttackEnd, a2_4); A6_IN(940); const uint32_t a2_r = typed::SCRLeoDashAttackEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoDashAttackEnd_0x139e160(rdram, ctx, runtime);
}
// SCRLeoDashAttack (0x139e180): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRLeoDashAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x139E180u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x139E194u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRLeoDashAttack, a2_4); A6_IN(941); const uint32_t a2_r = typed::SCRLeoDashAttack(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRLeoDashAttack_0x139e180(rdram, ctx, runtime);
}
// SCRCanossaWallBreakEnd (0x13a4830): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCanossaWallBreakEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A4830u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A4844u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRCanossaWallBreakEnd, a2_4); A6_IN(942); const uint32_t a2_r = typed::SCRCanossaWallBreakEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRCanossaWallBreakEnd_0x13a4830(rdram, ctx, runtime);
}
// SCRCanossaWallDelete (0x13a4850): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRCanossaWallDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A4850u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A4864u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRCanossaWallDelete, a2_4); A6_IN(943); const uint32_t a2_r = typed::SCRCanossaWallDelete(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRCanossaWallDelete_0x13a4850(rdram, ctx, runtime);
}
// SCRMinotourCFallWeapon (0x13a5a48): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMinotourCFallWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5A48u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A5A5Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMinotourCFallWeapon, a2_4); A6_IN(944); const uint32_t a2_r = typed::SCRMinotourCFallWeapon(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMinotourCFallWeapon_0x13a5a48(rdram, ctx, runtime);
}
// SCRMinotourCSetFallWeaponFlag (0x13a5a68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMinotourCSetFallWeaponFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5A68u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A5A7Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMinotourCSetFallWeaponFlag, a2_4); A6_IN(945); const uint32_t a2_r = typed::SCRMinotourCSetFallWeaponFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMinotourCSetFallWeaponFlag_0x13a5a68(rdram, ctx, runtime);
}
// SCRMinoCSetAddFlag (0x13a5b80): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMinoCSetAddFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5B80u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A5B94u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMinoCSetAddFlag, a2_4); A6_IN(946); const uint32_t a2_r = typed::SCRMinoCSetAddFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMinoCSetAddFlag_0x13a5b80(rdram, ctx, runtime);
}
// SCRMinoCResetAddFlag (0x13a5ba0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMinoCResetAddFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5BA0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A5BB4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMinoCResetAddFlag, a2_4); A6_IN(947); const uint32_t a2_r = typed::SCRMinoCResetAddFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMinoCResetAddFlag_0x13a5ba0(rdram, ctx, runtime);
}
// SCRMinotourCResetWeaponCameraFlag (0x13a5c68): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRMinotourCResetWeaponCameraFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5C68u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13A5C7Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRMinotourCResetWeaponCameraFlag, a2_4); A6_IN(948); const uint32_t a2_r = typed::SCRMinotourCResetWeaponCameraFlag(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRMinotourCResetWeaponCameraFlag_0x13a5c68(rdram, ctx, runtime);
}
// gamecore_sub_013A5CC8 (0x13a5cc8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013A5CC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13A5CC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A6_HOLDS(0x134F030u, lift_d1::_gameFlagChk), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_013A5CC8, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(949); const uint64_t a2_r = typed::gamecore_sub_013A5CC8(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013A5CC8_0x13a5cc8(rdram, ctx, runtime);
}
// SCRFoodTakeBegin (0x13b61c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFoodTakeBegin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B61C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B61D4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRFoodTakeBegin, a2_4); A6_IN(952); const uint32_t a2_r = typed::SCRFoodTakeBegin(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFoodTakeBegin_0x13b61c0(rdram, ctx, runtime);
}
// SCRFoodTakeEnd (0x13b61e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFoodTakeEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B61E0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B61F4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRFoodTakeEnd, a2_4); A6_IN(953); const uint32_t a2_r = typed::SCRFoodTakeEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFoodTakeEnd_0x13b61e0(rdram, ctx, runtime);
}
// SCRItemTakeBegin (0x13b6200): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRItemTakeBegin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6200u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6214u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRItemTakeBegin, a2_4); A6_IN(954); const uint32_t a2_r = typed::SCRItemTakeBegin(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRItemTakeBegin_0x13b6200(rdram, ctx, runtime);
}
// SCRItemTakeEnd (0x13b6220): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRItemTakeEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6220u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6234u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRItemTakeEnd, a2_4); A6_IN(955); const uint32_t a2_r = typed::SCRItemTakeEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRItemTakeEnd_0x13b6220(rdram, ctx, runtime);
}
// SCRItemTakeDelete (0x13b6240): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRItemTakeDelete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6240u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6254u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRItemTakeDelete, a2_4); A6_IN(956); const uint32_t a2_r = typed::SCRItemTakeDelete(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRItemTakeDelete_0x13b6240(rdram, ctx, runtime);
}
// SCRArrowContinue (0x13b62b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRArrowContinue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B62B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B62C4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRArrowContinue, a2_4); A6_IN(957); const uint32_t a2_r = typed::SCRArrowContinue(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRArrowContinue_0x13b62b0(rdram, ctx, runtime);
}
// SCRHarpoonStart (0x13b6310): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHarpoonStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6310u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6324u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHarpoonStart, a2_4); A6_IN(958); const uint32_t a2_r = typed::SCRHarpoonStart(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHarpoonStart_0x13b6310(rdram, ctx, runtime);
}
// SCRHarpoonContinue (0x13b6330): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHarpoonContinue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6330u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6344u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHarpoonContinue, a2_4); A6_IN(959); const uint32_t a2_r = typed::SCRHarpoonContinue(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHarpoonContinue_0x13b6330(rdram, ctx, runtime);
}
// SCRHarpoonLoopStart (0x13b6350): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHarpoonLoopStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6350u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6364u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHarpoonLoopStart, a2_4); A6_IN(960); const uint32_t a2_r = typed::SCRHarpoonLoopStart(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHarpoonLoopStart_0x13b6350(rdram, ctx, runtime);
}
// SCRHarpoonLoopEnd (0x13b6370): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHarpoonLoopEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6370u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6384u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHarpoonLoopEnd, a2_4); A6_IN(961); const uint32_t a2_r = typed::SCRHarpoonLoopEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHarpoonLoopEnd_0x13b6370(rdram, ctx, runtime);
}
// SCRPlayerSendDamageStart (0x13b6390): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSendDamageStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6390u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B63A4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSendDamageStart, a2_4); A6_IN(962); const uint32_t a2_r = typed::SCRPlayerSendDamageStart(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSendDamageStart_0x13b6390(rdram, ctx, runtime);
}
// SCRPlayerSendDamageEnd (0x13b63b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSendDamageEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B63B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B63C4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSendDamageEnd, a2_4); A6_IN(963); const uint32_t a2_r = typed::SCRPlayerSendDamageEnd(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSendDamageEnd_0x13b63b0(rdram, ctx, runtime);
}
// SCRPlayerSendDamage (0x13b63d0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSendDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B63D0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B63E4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSendDamage, a2_4); A6_IN(964); const uint32_t a2_r = typed::SCRPlayerSendDamage(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSendDamage_0x13b63d0(rdram, ctx, runtime);
}
// SCRPlayerSendDeathDamageLater (0x13b6410): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSendDeathDamageLater(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6410u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6424u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSendDeathDamageLater, a2_4); A6_IN(965); const uint32_t a2_r = typed::SCRPlayerSendDeathDamageLater(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSendDeathDamageLater_0x13b6410(rdram, ctx, runtime);
}
// SCRPlayerSowrdDeathHitSound (0x13b6450): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSowrdDeathHitSound(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6450u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6464u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSowrdDeathHitSound, a2_4); A6_IN(966); const uint32_t a2_r = typed::SCRPlayerSowrdDeathHitSound(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSowrdDeathHitSound_0x13b6450(rdram, ctx, runtime);
}
// SCRPlayerSowrdHitSoundMiddle (0x13b6490): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerSowrdHitSoundMiddle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6490u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B64A4u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerSowrdHitSoundMiddle, a2_4); A6_IN(967); const uint32_t a2_r = typed::SCRPlayerSowrdHitSoundMiddle(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerSowrdHitSoundMiddle_0x13b6490(rdram, ctx, runtime);
}
// SCRPlayerPauseDisable (0x13b67f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerPauseDisable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B67F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B680Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerPauseDisable, a2_4); A6_IN(968); const uint32_t a2_r = typed::SCRPlayerPauseDisable(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerPauseDisable_0x13b67f8(rdram, ctx, runtime);
}
// SCRPlayerPauseEnable (0x13b6818): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRPlayerPauseEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6818u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B682Cu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRPlayerPauseEnable, a2_4); A6_IN(969); const uint32_t a2_r = typed::SCRPlayerPauseEnable(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRPlayerPauseEnable_0x13b6818(rdram, ctx, runtime);
}
// SCRHorseKick (0x13b6898): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRHorseKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6898u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B68ACu, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRHorseKick, a2_4); A6_IN(970); const uint32_t a2_r = typed::SCRHorseKick(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRHorseKick_0x13b6898(rdram, ctx, runtime);
}
// nico_playeGripDamage (0x13b6a40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void nico_playeGripDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B6A40u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x13B6A54u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(nico_playeGripDamage, a2_4); A6_IN(971); const uint32_t a2_r = typed::nico_playeGripDamage(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::nico_playeGripDamage_0x13b6a40(rdram, ctx, runtime);
}
// SCRGetPlayerTiredParam (0x13cd688): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRGetPlayerTiredParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13CD688u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A5_JAL(0x13CD690u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(SCRGetPlayerTiredParam, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(973); const a1::RetVF a2_r = typed::SCRGetPlayerTiredParam(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRGetPlayerTiredParam_0x13cd688(rdram, ctx, runtime);
}
// AI_GetYRotQuaternionFromVector (0x13d59c8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void AI_GetYRotQuaternionFromVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13D59C8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1BC850u, lift_d1::SetQuaternionByCosineAxisRotateVWithNoRegularize) && A5_JAL(0x13D5A48u, 0x1BC850u) && A5_JAL(0x13D5A60u, 0x1BC850u) && A5_SP16(), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); A6_CHK(AI_GetYRotQuaternionFromVector, a2_4, a2_5); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(974); const a1::RetVF a2_r = typed::AI_GetYRotQuaternionFromVector(rdram, runtime, a2_4, a2_5, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::AI_GetYRotQuaternionFromVector_0x13d59c8(rdram, ctx, runtime);
}
// gamecore_sub_013F3EC8 (0x13f3ec8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013F3EC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13F3EC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x13D3DA0u, lift_d1::AI_UpdateVelocity), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A6_CHK(gamecore_sub_013F3EC8, a2_4, a2_f20, a2_f21); uint64_t a3_xo[3] = {(uint64_t)a1_bits(ctx->f[12]), (uint64_t)a1_bits(ctx->f[13]), (uint64_t)a1_bits(ctx->f[21])}; A6_IN(977); const a1::RetVF a2_r = typed::gamecore_sub_013F3EC8(rdram, runtime, a2_4, a2_f20, a2_f21, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); ctx->f[13] = a1_flt((uint32_t)a3_xo[1]); ctx->f[21] = a1_flt((uint32_t)a3_xo[2]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013F3EC8_0x13f3ec8(rdram, ctx, runtime);
}
// SCRTortoisePulseChargeStop (0x1404950): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRTortoisePulseChargeStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1404950u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x1404964u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRTortoisePulseChargeStop, a2_4); A6_IN(978); const uint32_t a2_r = typed::SCRTortoisePulseChargeStop(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRTortoisePulseChargeStop_0x1404950(rdram, ctx, runtime);
}
// SCRYamori2BeginStand (0x140c820): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRYamori2BeginStand(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140C820u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x140C834u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRYamori2BeginStand, a2_4); A6_IN(979); const uint32_t a2_r = typed::SCRYamori2BeginStand(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRYamori2BeginStand_0x140c820(rdram, ctx, runtime);
}
// SCRYamoriBReadyToBreeze (0x140c840): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRYamoriBReadyToBreeze(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140C840u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x140C854u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRYamoriBReadyToBreeze, a2_4); A6_IN(980); const uint32_t a2_r = typed::SCRYamoriBReadyToBreeze(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRYamoriBReadyToBreeze_0x140c840(rdram, ctx, runtime);
}
// SCRYamoriBReadyToBreeze2 (0x140c860): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRYamoriBReadyToBreeze2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x140C860u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C1530u, lift_d1::RequestCharacterMotionShiftFunc) && A5_JAL(0x140C874u, 0x10C1530u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(SCRYamoriBReadyToBreeze2, a2_4); A6_IN(981); const uint32_t a2_r = typed::SCRYamoriBReadyToBreeze2(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRYamoriBReadyToBreeze2_0x140c860(rdram, ctx, runtime);
}
// CAM_near_to_aim_pitch (0x1419a98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_near_to_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1419A98u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x107DA28u, lift_d1::GAMEUTIL_inc_or_dec_for_near_to_aim_pitch) && A5_JAL(0x1419ABCu, 0x107DA28u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; A6_CHK(CAM_near_to_aim_pitch, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f20, a2_f21); A6_IN(983); const a1::RetVF a2_r = typed::CAM_near_to_aim_pitch(rdram, runtime, a2_2, a2_4, a2_5, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f20, a2_f21); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_near_to_aim_pitch_0x1419a98(rdram, ctx, runtime);
}
// CAM_get_mul_ratio_offset_per_axis (0x1419f98): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_get_mul_ratio_offset_per_axis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1419F98u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x107FFE0u, lift_d1::GAMEUTIL_get_divide_vector_value) && A5_JAL(0x141A00Cu, 0x107FFE0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; A6_CHK(CAM_get_mul_ratio_offset_per_axis, a2_4, a2_5, a2_6, a2_7, a2_8, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(984); const a1::RetVF a2_r = typed::CAM_get_mul_ratio_offset_per_axis(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_get_mul_ratio_offset_per_axis_0x1419f98(rdram, ctx, runtime);
}
// CAM_get_motion_kind_interp_timer_target (0x141a628): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void CAM_get_motion_kind_interp_timer_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141A628u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10842F0u, lift_d1::GAMEUTIL_get_interp_timer_interp_ratio_cntrl) && A5_JAL(0x141A638u, 0x10842F0u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(CAM_get_motion_kind_interp_timer_target, a2_4); A6_IN(985); const float a2_r = typed::CAM_get_motion_kind_interp_timer_target(rdram, runtime, a2_4); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::CAM_get_motion_kind_interp_timer_target_0x141a628(rdram, ctx, runtime);
}
// get_camera_parameter_cameraman (0x141cff8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_camera_parameter_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141CFF8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1162EF0u, lift_d1::get_camera_position) && A6_HOLDS(0x1162EF8u, lift_d1::get_camera_direction) && A6_HOLDS(0x1162F00u, lift_d1::get_camera_up_direction) && A6_HOLDS(0x1162F10u, lift_d1::get_clip_screen) && A6_HOLDS(0x1164530u, lift_d1::get_view_angle) && A5_JAL(0x141D028u, 0x1162EF0u) && A5_JAL(0x141D038u, 0x1162EF8u) && A5_JAL(0x141D048u, 0x1162F00u) && A5_JAL(0x141D058u, 0x1162F10u) && A5_JAL(0x141D060u, 0x1164530u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(get_camera_parameter_cameraman, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(987); const a1::RetVF a2_r = typed::get_camera_parameter_cameraman(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a2_f0, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_camera_parameter_cameraman_0x141cff8(rdram, ctx, runtime);
}
// get_cameraman (0x141d3c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D3C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x141D4F0u, lift_d1::gamecore_sub_0141D4F0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(get_cameraman, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(988); const uint32_t a2_r = typed::get_cameraman(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cameraman_0x141d3c0(rdram, ctx, runtime);
}
// get_cameraman_camera_obj (0x141d3f8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cameraman_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x141D3F8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x141CD68u, lift_d1::get_camera_obj) && A6_HOLDS(0x141D4F0u, lift_d1::gamecore_sub_0141D4F0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(get_cameraman_camera_obj, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(989); const uint32_t a2_r = typed::get_cameraman_camera_obj(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cameraman_camera_obj_0x141d3f8(rdram, ctx, runtime);
}
// end_jack_game_camera (0x1424f88): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void end_jack_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1424F88u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1424E18u, lift_d1::reset_game_camera), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(end_jack_game_camera, a2_4); A6_IN(990); const uint32_t a2_r = typed::end_jack_game_camera(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::end_jack_game_camera_0x1424f88(rdram, ctx, runtime);
}
// is_proper_camera_target_game_camera (0x1425058): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_proper_camera_target_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425058u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A6_HOLDS(0x141CE60u, lift_d1::get_cur_camera_target) && A5_JAL(0x1425070u, 0x10C0A70u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(is_proper_camera_target_game_camera, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(991); const uint64_t a2_r = typed::is_proper_camera_target_game_camera(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_proper_camera_target_game_camera_0x1425058(rdram, ctx, runtime);
}
// get_cur_using_camera_mode_condition_param_game_camera (0x1425100): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_using_camera_mode_condition_param_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425100u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x142C108u, ::a6ad::get_cur_base_mode_condition_game_camera_param_manager), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_using_camera_mode_condition_param_game_camera, a2_4); A6_IN(992); const uint32_t a2_r = typed::get_cur_using_camera_mode_condition_param_game_camera(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_using_camera_mode_condition_param_game_camera_0x1425100(rdram, ctx, runtime);
}
// get_cur_using_camera_mode_manager_param_game_camera (0x1425118): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void get_cur_using_camera_mode_manager_param_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1425118u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x142C110u, ::a6ad::get_cur_base_mode_manager_game_camera_param_manager), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(get_cur_using_camera_mode_manager_param_game_camera, a2_4); A6_IN(993); const uint32_t a2_r = typed::get_cur_using_camera_mode_manager_param_game_camera(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::get_cur_using_camera_mode_manager_param_game_camera_0x1425118(rdram, ctx, runtime);
}
// is_mode_change_game_camera (0x14254e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void is_mode_change_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14254E8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x141CE58u, lift_d1::get_game_camera), 1)) { const uint32_t a2_2 = Ps2GprU32(ctx, 2); const uint32_t a2_4 = Ps2GprU32(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(is_mode_change_game_camera, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(994); const uint32_t a2_r = typed::is_mode_change_game_camera(rdram, runtime, a2_2, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::is_mode_change_game_camera_0x14254e8(rdram, ctx, runtime);
}
// setRayCutPassageWork (0x1431f60): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setRayCutPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1431F60u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1431CE0u, lift_d1::getPassageWork), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(setRayCutPassageWork, a2_4, a2_5, a2_6); A6_IN(996); const uint64_t a2_r = typed::setRayCutPassageWork(rdram, runtime, a2_4, a2_5, a2_6); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setRayCutPassageWork_0x1431f60(rdram, ctx, runtime);
}
// addPassageInsertPos (0x1433798): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void addPassageInsertPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1433798u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x14338A0u, ::a6ad::forceAddPassageWork), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(addPassageInsertPos, a2_4, a2_5); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(1003); const uint64_t a2_r = typed::addPassageInsertPos(rdram, runtime, a2_4, a2_5, a4_h3, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::addPassageInsertPos_0x1433798(rdram, ctx, runtime);
}
// forceSetWayFinish (0x1433848): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void forceSetWayFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1433848u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1433120u, lift_d1::resetPassageWork), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(forceSetWayFinish, a2_4); A6_IN(1004); const uint32_t a2_r = typed::forceSetWayFinish(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::forceSetWayFinish_0x1433848(rdram, ctx, runtime);
}
// SCRFuncClsGameFlag (0x14517a8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncClsGameFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14517A8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SCRFuncClsGameFlag, a2_5); A6_IN(1007); const uint64_t a2_r = typed::SCRFuncClsGameFlag(rdram, runtime, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncClsGameFlag_0x14517a8(rdram, ctx, runtime);
}
// SCRFuncSetAutoBossGazePos (0x1452340): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetAutoBossGazePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452340u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1343B28u, lift_d1::GAMEUTIL_set_auto_get_flag_boss_interest_position), 1)) { A6_CHK(SCRFuncSetAutoBossGazePos); A6_IN(1008); const uint32_t a2_r = typed::SCRFuncSetAutoBossGazePos(rdram, runtime); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSetAutoBossGazePos_0x1452340(rdram, ctx, runtime);
}
// SCRFuncSegmentLoadWait (0x1452d70): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSegmentLoadWait(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452D70u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1365AB0u, lift_d1::stagemanager_checkstageAlreadyLoadRequestedByStrptr) && A6_HOLDS(0x13F62D8u, ::a6ad::setLoadingCheckFunc), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_10 = Ps2GprU64(ctx, 10); A6_CHK(SCRFuncSegmentLoadWait, a2_2, a2_4, a2_10); A6_IN(1009); const uint64_t a2_r = typed::SCRFuncSegmentLoadWait(rdram, runtime, a2_2, a2_4, a2_10); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSegmentLoadWait_0x1452d70(rdram, ctx, runtime);
}
// SCRFuncSetGameWork (0x1452f40): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetGameWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1452F40u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F0A8u, lift_d1::_gameWorkSet), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SCRFuncSetGameWork, a2_5, a2_6); A6_IN(1010); typed::SCRFuncSetGameWork(rdram, runtime, a2_5, a2_6); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSetGameWork_0x1452f40(rdram, ctx, runtime);
}
// SCRConditionOverPlayerGrip (0x1458fd8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionOverPlayerGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1458FD8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x13584E8u, lift_d1::getPlayerGrip), 1)) { const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; A6_CHK(SCRConditionOverPlayerGrip, a2_f0, a2_f12, a2_f20); A6_IN(1016); const a1::RetVF a2_r = typed::SCRConditionOverPlayerGrip(rdram, runtime, a2_f0, a2_f12, a2_f20); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionOverPlayerGrip_0x1458fd8(rdram, ctx, runtime);
}
// SCRConditionLessPlayerGrip (0x1459010): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionLessPlayerGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459010u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x13584E8u, lift_d1::getPlayerGrip), 1)) { const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; A6_CHK(SCRConditionLessPlayerGrip, a2_f0, a2_f12, a2_f20); A6_IN(1017); const a1::RetVF a2_r = typed::SCRConditionLessPlayerGrip(rdram, runtime, a2_f0, a2_f12, a2_f20); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionLessPlayerGrip_0x1459010(rdram, ctx, runtime);
}
// SCRConditionCheckGameClearCnt (0x1459ab8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionCheckGameClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1459AB8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134F278u, lift_d1::getGameClearCnt), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SCRConditionCheckGameClearCnt, a2_2, a2_5); A6_IN(1018); const uint64_t a2_r = typed::SCRConditionCheckGameClearCnt(rdram, runtime, a2_2, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionCheckGameClearCnt_0x1459ab8(rdram, ctx, runtime);
}
// SCRFuncActiveDemoCancel (0x1464d28): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncActiveDemoCancel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464D28u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1348958u, ::a6ad::activeDemoCancel), 1)) { const uint32_t a2_10 = Ps2GprU32(ctx, 10); A6_CHK(SCRFuncActiveDemoCancel, a2_10); A6_IN(1022); const uint32_t a2_r = typed::SCRFuncActiveDemoCancel(rdram, runtime, a2_10); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncActiveDemoCancel_0x1464d28(rdram, ctx, runtime);
}
// gamecore_sub_01464E90 (0x1464e90): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_01464E90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464E90u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x136AA50u, lift_d1::bgmStop), 1)) { const uint64_t a2_8 = Ps2GprU64(ctx, 8); A6_CHK(gamecore_sub_01464E90, a2_8); A6_IN(1023); const uint32_t a2_r = typed::gamecore_sub_01464E90(rdram, runtime, a2_8); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_01464E90_0x1464e90(rdram, ctx, runtime);
}
// SCRConditionSoundSignal (0x1464eb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRConditionSoundSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464EB8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x136A1B8u, ::a6ad::bgmScriptRecvSoundSignal), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); A6_CHK(SCRConditionSoundSignal, a2_5); A6_IN(1024); const uint32_t a2_r = typed::SCRConditionSoundSignal(rdram, runtime, a2_5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRConditionSoundSignal_0x1464eb8(rdram, ctx, runtime);
}
// setlocale (0x12e530): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void setlocale(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x12E530u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A6_HOLDS(0x12E490u, ::a6ad::_setlocale_r), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(setlocale, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(1025); const uint64_t a2_r = typed::setlocale(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::setlocale_0x12e530(rdram, ctx, runtime);
}
// Sg2ExStAdpcmIopReadAdr (0x1cd660): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void Sg2ExStAdpcmIopReadAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1CD660u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1D2170u, lift_d1::Sg2RemoteGetAdpStmAdr) && A6_HOLDS(0x1D9E50u, ::a6ad::Sg2StAdpcmIopReadAdr), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); A6_CHK(Sg2ExStAdpcmIopReadAdr, a2_4, a2_5, a2_29); A6_IN(1026); const uint64_t a2_r = typed::Sg2ExStAdpcmIopReadAdr(rdram, runtime, a2_4, a2_5, a2_29); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::Sg2ExStAdpcmIopReadAdr_0x1cd660(rdram, ctx, runtime);
}
// SetAndLockBgColor (0x10899c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetAndLockBgColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10899C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C0138u, lift_d1::iosDlGetWorkBufId) && A6_HOLDS(0x1C2928u, lift_d1::iosSetBGColor) && A5_JAL(0x1089A20u, 0x1C2928u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint32_t a2_6 = Ps2GprU32(ctx, 6); const uint32_t a2_29 = Ps2GprU32(ctx, 29); A6_CHK(SetAndLockBgColor, a2_4, a2_5, a2_6, a2_29); A6_IN(1027); const uint64_t a2_r = typed::SetAndLockBgColor(rdram, runtime, a2_4, a2_5, a2_6, a2_29); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetAndLockBgColor_0x10899c0(rdram, ctx, runtime);
}
// motrootdyn_startxzfit_common (0x10eb568): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void motrootdyn_startxzfit_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10EB568u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C2BE8u, lift_d1::UnlinkSecureParentInfo) && A6_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A5_SP16(), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(motrootdyn_startxzfit_common, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vacc = ctx->vu0_acc; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(1028); const uint64_t a2_r = typed::motrootdyn_startxzfit_common(rdram, runtime, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vacc, a4_h2, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_acc = a4_vacc; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::motrootdyn_startxzfit_common_0x10eb568(rdram, ctx, runtime);
}
// GetLayoutObjChildWithName (0x10f1110): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLayoutObjChildWithName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F1110u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x128E6Cu, ::decomp_leaves::strCmp) && A6_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A6_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A6_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A6_HOLDS(0x1147BA8u, lift_d1::GetTwoWayLinkNext) && A6_HOLDS(0x1147BC8u, lift_d1::GetTwoWayLinkData) && A6_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A6_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x10F1240u, 0x128E6Cu), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(GetLayoutObjChildWithName, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(1029); const uint64_t a2_r = typed::GetLayoutObjChildWithName(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLayoutObjChildWithName_0x10f1110(rdram, ctx, runtime);
}
// SetLayoutUserClipFunc (0x10f1ab0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SetLayoutUserClipFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F1AB0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A6_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A6_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A6_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A6_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(SetLayoutUserClipFunc, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(1030); const uint64_t a2_r = typed::SetLayoutUserClipFunc(rdram, runtime, a2_4, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SetLayoutUserClipFunc_0x10f1ab0(rdram, ctx, runtime);
}
// ChangeLayoutObjCollisionResourceAttribute (0x10f31c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ChangeLayoutObjCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F31C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A6_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A6_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A6_HOLDS(0x1128778u, lift_d1::ChangeCollisionResourceAttribute) && A6_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A6_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(ChangeLayoutObjCollisionResourceAttribute, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); A6_IN(1034); const uint64_t a2_r = typed::ChangeLayoutObjCollisionResourceAttribute(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ChangeLayoutObjCollisionResourceAttribute_0x10f31c0(rdram, ctx, runtime);
}
// GetLwsDemoOrientManagerObjWorkSize (0x10f9c30): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetLwsDemoOrientManagerObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10F9C30u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA4A0u, lift_d1::isysGetSubThreadWorkSize) && A6_HOLDS(0x1102A40u, lift_d1::GetScriptObjWorkSize) && A5_JAL(0x1102A50u, 0x1CA4A0u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetLwsDemoOrientManagerObjWorkSize, a2_4); A6_IN(1035); const uint32_t a2_r = typed::GetLwsDemoOrientManagerObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetLwsDemoOrientManagerObjWorkSize_0x10f9c30(rdram, ctx, runtime);
}
// GetScriptSpriteObjWorkSize (0x1114278): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetScriptSpriteObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1114278u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA4A0u, lift_d1::isysGetSubThreadWorkSize) && A6_HOLDS(0x1102A40u, lift_d1::GetScriptObjWorkSize) && A5_JAL(0x1102A50u, 0x1CA4A0u), 1)) { const uint32_t a2_4 = Ps2GprU32(ctx, 4); A6_CHK(GetScriptSpriteObjWorkSize, a2_4); A6_IN(1036); const uint32_t a2_r = typed::GetScriptSpriteObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::GetScriptSpriteObjWorkSize_0x1114278(rdram, ctx, runtime);
}
// SCRSetCharaMotionPlaySpeedRatio (0x1378758): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRSetCharaMotionPlaySpeedRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1378758u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A6_HOLDS(0x139AC18u, ::a6ad::MotSpdCtrl_SetSpeed) && A5_JAL(0x139AC40u, 0x10C0A70u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A6_CHK(SCRSetCharaMotionPlaySpeedRatio, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(1040); const uint64_t a2_r = typed::SCRSetCharaMotionPlaySpeedRatio(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRSetCharaMotionPlaySpeedRatio_0x1378758(rdram, ctx, runtime);
}
// gamecore_sub_013B7990 (0x13b7990): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B7990(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B7990u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134BAF0u, ::a6ad::SetCaptionPauseFlag) && A6_HOLDS(0x1352418u, lift_d1::gameflowSetPauseEnable) && A6_HOLDS(0x1352440u, ::a6ad::gameflowSetCaptionPause), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(gamecore_sub_013B7990, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(1041); const uint32_t a2_r = typed::gamecore_sub_013B7990(rdram, runtime, a2_4, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013B7990_0x13b7990(rdram, ctx, runtime);
}
// gamecore_sub_013B79B0 (0x13b79b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_013B79B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x13B79B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134BAF0u, ::a6ad::SetCaptionPauseFlag) && A6_HOLDS(0x1352418u, lift_d1::gameflowSetPauseEnable) && A6_HOLDS(0x1352440u, ::a6ad::gameflowSetCaptionPause), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(gamecore_sub_013B79B0, a2_4); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(1042); const uint32_t a2_r = typed::gamecore_sub_013B79B0(rdram, runtime, a2_4, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_013B79B0_0x13b79b0(rdram, ctx, runtime);
}
// deleteUserPathWorkEx (0x14359b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void deleteUserPathWorkEx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14359B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1147AB0u, lift_d1::DeleteTwoWayLink) && A6_HOLDS(0x1431FE0u, lift_d1::deleteUserPathWork) && A5_JAL(0x1432000u, 0x1147AB0u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(deleteUserPathWorkEx, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(1044); const uint32_t a2_r = typed::deleteUserPathWorkEx(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::deleteUserPathWorkEx_0x14359b0(rdram, ctx, runtime);
}
// gamecore_sub_014362E8 (0x14362e8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_014362E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14362E8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1169EA8u, lift_d1::getGoalPointData) && A6_HOLDS(0x1433CE0u, lift_d1::getGoalPointDataByAreaIdx) && A5_JAL(0x1433D5Cu, 0x1169EA8u) && A5_SP16(), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); A6_CHK(gamecore_sub_014362E8, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[1] = {(uint64_t)a1_bits(ctx->f[0])}; A6_IN(1045); const uint64_t a2_r = typed::gamecore_sub_014362E8(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_8, a4_vf0, a4_vf5, a4_vf6, a4_vf7, a4_vacc, a4_vq, a4_vstat, a4_h2, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_014362E8_0x14362e8(rdram, ctx, runtime);
}
// calcMoveTurnAngle2 (0x1437e20): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void calcMoveTurnAngle2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1437E20u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10D2D48u, lift_d1::getRatioFromInterp) && A6_HOLDS(0x1445C90u, lift_d1::calcMoveAngle) && A5_JAL(0x1445CD4u, 0x10D2D48u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f0 = ctx->f[0]; const float a2_f12 = ctx->f[12]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; A6_CHK(calcMoveTurnAngle2, a2_4, a2_f0, a2_f12, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24); A6_IN(1046); const float a2_r = typed::calcMoveTurnAngle2(rdram, runtime, a2_4, a2_f0, a2_f12, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24); A6_OUT(); ctx->f[0] = a2_r; ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::calcMoveTurnAngle2_0x1437e20(rdram, ctx, runtime);
}
// gamecore_sub_014454B0 (0x14454b0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void gamecore_sub_014454B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14454B0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1437A88u, lift_d1::_resetBaseMoveWork) && A6_HOLDS(0x1438448u, lift_d1::gamecore_sub_01438448) && A6_HOLDS(0x14454E0u, ::a6ad::gamecore_sub_014454E0), 1)) { const uint64_t a2_5 = Ps2GprU64(ctx, 5); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(gamecore_sub_014454B0, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(1048); typed::gamecore_sub_014454B0(rdram, runtime, a2_5, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::gamecore_sub_014454B0_0x14454b0(rdram, ctx, runtime);
}
// SCRFuncSetGameFlagByGameObj (0x1451658): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetGameFlagByGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1451658u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x134EFB0u, lift_d1::_gameFlagSet) && A6_HOLDS(0x134EFF0u, lift_d1::_gameFlagCls) && A6_HOLDS(0x1353638u, ::a6ad::gStatGobjDispGflagSet), 1)) { const uint64_t a2_9 = Ps2GprU64(ctx, 9); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(SCRFuncSetGameFlagByGameObj, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_IN(1049); const uint64_t a2_r = typed::SCRFuncSetGameFlagByGameObj(rdram, runtime, a2_9, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSetGameFlagByGameObj_0x1451658(rdram, ctx, runtime);
}
// getEventScriptObjWorkSize (0x1464b58): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void getEventScriptObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1464B58u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1CA4A0u, lift_d1::isysGetSubThreadWorkSize) && A6_HOLDS(0x1102A40u, lift_d1::GetScriptObjWorkSize) && A5_JAL(0x1102A50u, 0x1CA4A0u) && A5_JAL(0x1464B70u, 0x1102A40u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); A6_CHK(getEventScriptObjWorkSize, a2_4); A6_IN(1051); const uint32_t a2_r = typed::getEventScriptObjWorkSize(rdram, runtime, a2_4); A6_OUT(); Ps2SetGpr64(ctx, 2, sx64(a2_r)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::getEventScriptObjWorkSize_0x1464b58(rdram, ctx, runtime);
}
// ChangeCharacterObjCollisionResourceAttribute (0x10c1e00): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void ChangeCharacterObjCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10C1E00u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10F31C0u, ::a6ad::ChangeLayoutObjCollisionResourceAttribute) && A6_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A6_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A6_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A6_HOLDS(0x1128778u, lift_d1::ChangeCollisionResourceAttribute) && A6_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A6_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(ChangeCharacterObjCollisionResourceAttribute, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); A6_IN(1053); const uint64_t a2_r = typed::ChangeCharacterObjCollisionResourceAttribute(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_8, a2_9, a2_10, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a4_h2, a4_h3); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::ChangeCharacterObjCollisionResourceAttribute_0x10c1e00(rdram, ctx, runtime);
}
// SpriteMenuSetDispFlagNoFade (0x1111ac8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SpriteMenuSetDispFlagNoFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1111AC8u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1C2038u, lift_d1::getDrawScreenWidth) && A6_HOLDS(0x1C2048u, lift_d1::getDrawScreenHeight) && A6_HOLDS(0x110EB10u, lift_d1::manager_sub_0110EB10) && A6_HOLDS(0x1113BD8u, lift_d1::manager_sub_01113BD8) && A5_JAL(0x110EBA8u, 0x1C2048u) && A5_JAL(0x110EBB8u, 0x1C2038u) && A5_JAL(0x110EBC8u, 0x1C2038u) && A5_JAL(0x110EBD8u, 0x1C2048u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint32_t a2_5 = Ps2GprU32(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); A6_CHK(SpriteMenuSetDispFlagNoFade, a2_4, a2_5, a2_6); uint64_t a4_h4 = a4::hi64(ctx->r[4]); A6_IN(1054); const uint64_t a2_r = typed::SpriteMenuSetDispFlagNoFade(rdram, runtime, a2_4, a2_5, a2_6, a4_h4); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SpriteMenuSetDispFlagNoFade_0x1111ac8(rdram, ctx, runtime);
}
// createHumanCtrlWork (0x1445388): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void createHumanCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1445388u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x1432708u, lift_d1::setAttainRadPathWork) && A6_HOLDS(0x1437A68u, lift_d1::_setStopCtrlModeBaseMoveWork) && A6_HOLDS(0x1437A88u, lift_d1::_resetBaseMoveWork) && A6_HOLDS(0x1438448u, lift_d1::gamecore_sub_01438448) && A6_HOLDS(0x14454B0u, ::a6ad::gamecore_sub_014454B0) && A6_HOLDS(0x14454E0u, ::a6ad::gamecore_sub_014454E0), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; A6_CHK(createHumanCtrlWork, a2_4, a2_5, a2_6, a2_7, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[12]), Ps2GprU64(ctx, 6)}; A6_IN(1056); const a1::RetVF a2_r = typed::createHumanCtrlWork(rdram, runtime, a2_4, a2_5, a2_6, a2_7, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a3_xo); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[12] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 6, a3_xo[1]); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::createHumanCtrlWork_0x1445388(rdram, ctx, runtime);
}
// SCRFuncSetMyselfMotionSpeedRatio (0x14517c0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void SCRFuncSetMyselfMotionSpeedRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x14517C0u, 1))
    {
        if (PS2_EXPECT(A6_GO() && A6_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A6_HOLDS(0x1378758u, ::a6ad::SCRSetCharaMotionPlaySpeedRatio) && A6_HOLDS(0x139AC18u, ::a6ad::MotSpdCtrl_SetSpeed) && A5_JAL(0x139AC40u, 0x10C0A70u), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; A6_CHK(SCRFuncSetMyselfMotionSpeedRatio, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); A6_IN(1057); const uint64_t a2_r = typed::SCRFuncSetMyselfMotionSpeedRatio(rdram, runtime, a2_4, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a4_h2, a4_h5); A6_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a6
    }
    ::SCRFuncSetMyselfMotionSpeedRatio_0x14517c0(rdram, ctx, runtime);
}
}

namespace
{
    struct A6Ad
    {
        uint32_t addr;
        PS2Runtime::RecompiledFunction rec, ad;
    };
    const A6Ad kA6Ad[970] = {
        {0x12BF48u, &::ps2___sccl_0x12bf48, &a6ad::ps2___sccl},
        {0x1CAC80u, &::sndSepStopReq_0x1cac80, &a6ad::sndSepStopReq},
        {0x107C6C0u, &::doit_debug_utility_0x107c6c0, &a6ad::doit_debug_utility},
        {0x107C750u, &::DEBUG_set_update_position_method_pointer_0x107c750, &a6ad::DEBUG_set_update_position_method_pointer},
        {0x107C760u, &::DEBUG_is_locking_debug_camera_0x107c760, &a6ad::DEBUG_is_locking_debug_camera},
        {0x107FBA0u, &::GAMEUTIL_get_velocity_by_diff_move3D_0x107fba0, &a6ad::GAMEUTIL_get_velocity_by_diff_move3D},
        {0x107FC50u, &::GAMEUTIL_get_move_limit_position_0x107fc50, &a6ad::GAMEUTIL_get_move_limit_position},
        {0x107FCF0u, &::GAMEUTIL_get_move_limit_position3D_0x107fcf0, &a6ad::GAMEUTIL_get_move_limit_position3D},
        {0x107FDF0u, &::GAMEUTIL_get_vel_limit_position_0x107fdf0, &a6ad::GAMEUTIL_get_vel_limit_position},
        {0x107FE68u, &::GAMEUTIL_is_inverse_move_direction_0x107fe68, &a6ad::GAMEUTIL_is_inverse_move_direction},
        {0x10802C0u, &::GAMEUTIL_is_max_input_0x10802c0, &a6ad::GAMEUTIL_is_max_input},
        {0x1080C28u, &::GAMEUTIL_is_between_yaw_and_yaw_extend_abs_0x1080c28, &a6ad::GAMEUTIL_is_between_yaw_and_yaw_extend_abs},
        {0x1081DC8u, &::GAMEUTIL_is_yaw_dir_sandwich_0x1081dc8, &a6ad::GAMEUTIL_is_yaw_dir_sandwich},
        {0x1081E58u, &::GAMEUTIL_is_pitch_dir_sandwich_0x1081e58, &a6ad::GAMEUTIL_is_pitch_dir_sandwich},
        {0x10826C0u, &::GAMEUTIL_is_reach_aim_yaw_dir_0x10826c0, &a6ad::GAMEUTIL_is_reach_aim_yaw_dir},
        {0x1082790u, &::GAMEUTIL_is_reach_aim_pitch_dir_0x1082790, &a6ad::GAMEUTIL_is_reach_aim_pitch_dir},
        {0x1082A80u, &::GAMEUTIL_set_inside_check_method_0x1082a80, &a6ad::GAMEUTIL_set_inside_check_method},
        {0x1083700u, &::GAMEUTIL_get_dist_ratio_by_dist_squ_0x1083700, &a6ad::GAMEUTIL_get_dist_ratio_by_dist_squ},
        {0x10853E8u, &::getBgAntiLevel_0x10853e8, &a6ad::getBgAntiLevel},
        {0x10853F8u, &::bgInit_0x10853f8, &a6ad::bgInit},
        {0x1088B10u, &::fogEffectSetParam_0x1088b10, &a6ad::fogEffectSetParam},
        {0x1089278u, &::getCurrentFsEffect_0x1089278, &a6ad::getCurrentFsEffect},
        {0x10898C8u, &::GetNowFogColorR_0x10898c8, &a6ad::GetNowFogColorR},
        {0x10898D8u, &::GetNowFogColorG_0x10898d8, &a6ad::GetNowFogColorG},
        {0x10898E8u, &::GetNowFogColorB_0x10898e8, &a6ad::GetNowFogColorB},
        {0x1089948u, &::getFogRate_0x1089948, &a6ad::getFogRate},
        {0x1089A28u, &::UnlockBgColor_0x1089a28, &a6ad::UnlockBgColor},
        {0x108B960u, &::skipBlur_0x108b960, &a6ad::skipBlur},
        {0x108E528u, &::glMatrixMode_0x108e528, &a6ad::glMatrixMode},
        {0x108E7B0u, &::glColor3f_0x108e7b0, &a6ad::glColor3f},
        {0x108E830u, &::glColor3fv_0x108e830, &a6ad::glColor3fv},
        {0x108ECE8u, &::glGetColor_0x108ece8, &a6ad::glGetColor},
        {0x108ED40u, &::glGetZTest_0x108ed40, &a6ad::glGetZTest},
        {0x108EE38u, &::glPopMatrix_0x108ee38, &a6ad::glPopMatrix},
        {0x108EE58u, &::glTranslatef_0x108ee58, &a6ad::glTranslatef},
        {0x108F420u, &::glLightVector_0x108f420, &a6ad::glLightVector},
        {0x108F828u, &::printGifTag_0x108f828, &a6ad::printGifTag},
        {0x108F830u, &::printPrimReg_0x108f830, &a6ad::printPrimReg},
        {0x108F838u, &::glGetZWrite_0x108f838, &a6ad::glGetZWrite},
        {0x108F848u, &::glGetLineSmooth_0x108f848, &a6ad::glGetLineSmooth},
        {0x108F8A8u, &::setv3f_0x108f8a8, &a6ad::setv3f},
        {0x108FAF8u, &::LockFullScreenEffect_0x108faf8, &a6ad::LockFullScreenEffect},
        {0x108FB08u, &::UnlockFullScreenEffect_0x108fb08, &a6ad::UnlockFullScreenEffect},
        {0x108FBB8u, &::setMBlur_0x108fbb8, &a6ad::setMBlur},
        {0x108FBD8u, &::graphFrameBufferReInitializeRequest_0x108fbd8, &a6ad::graphFrameBufferReInitializeRequest},
        {0x108FE40u, &::getFullScreenEffect_0x108fe40, &a6ad::getFullScreenEffect},
        {0x108FE50u, &::getFBOfsU_0x108fe50, &a6ad::getFBOfsU},
        {0x108FE60u, &::getFBOfsV_0x108fe60, &a6ad::getFBOfsV},
        {0x108FE70u, &::getFBOfsX_0x108fe70, &a6ad::getFBOfsX},
        {0x108FE80u, &::getFBOfsY_0x108fe80, &a6ad::getFBOfsY},
        {0x1091708u, &::kColor_0x1091708, &a6ad::kColor},
        {0x1091910u, &::iosSetKPrintfColorContext_0x1091910, &a6ad::iosSetKPrintfColorContext},
        {0x1091928u, &::iosSetKPrintfLocateContext_0x1091928, &a6ad::iosSetKPrintfLocateContext},
        {0x1091940u, &::iosSetKPrintfLocateContextRelativePosition_0x1091940, &a6ad::iosSetKPrintfLocateContextRelativePosition},
        {0x1091978u, &::iosSetKPrintfDynamicsContext_0x1091978, &a6ad::iosSetKPrintfDynamicsContext},
        {0x1091980u, &::iosSetKPrintfDlContext_0x1091980, &a6ad::iosSetKPrintfDlContext},
        {0x1091988u, &::iosSetKPrintfScaleContext_0x1091988, &a6ad::iosSetKPrintfScaleContext},
        {0x1091990u, &::getFontBoard_0x1091990, &a6ad::getFontBoard},
        {0x1092480u, &::setSpecularity_0x1092480, &a6ad::setSpecularity},
        {0x10924D8u, &::lightsw0_0x10924d8, &a6ad::lightsw0},
        {0x10924E8u, &::getlc0_r_0x10924e8, &a6ad::getlc0_r},
        {0x10924F8u, &::getlc0_g_0x10924f8, &a6ad::getlc0_g},
        {0x1092508u, &::getlc0_b_0x1092508, &a6ad::getlc0_b},
        {0x1092518u, &::getlv0_h_0x1092518, &a6ad::getlv0_h},
        {0x1092528u, &::getlv0_p_0x1092528, &a6ad::getlv0_p},
        {0x1092538u, &::getlv0_b_0x1092538, &a6ad::getlv0_b},
        {0x1092548u, &::lightsw1_0x1092548, &a6ad::lightsw1},
        {0x1092558u, &::getlc1_r_0x1092558, &a6ad::getlc1_r},
        {0x1092568u, &::getlc1_g_0x1092568, &a6ad::getlc1_g},
        {0x1092578u, &::getlc1_b_0x1092578, &a6ad::getlc1_b},
        {0x1092588u, &::getlv1_h_0x1092588, &a6ad::getlv1_h},
        {0x1092598u, &::getlv1_p_0x1092598, &a6ad::getlv1_p},
        {0x10925A8u, &::getlv1_b_0x10925a8, &a6ad::getlv1_b},
        {0x10925B8u, &::lightsw2_0x10925b8, &a6ad::lightsw2},
        {0x10925C8u, &::getlc2_r_0x10925c8, &a6ad::getlc2_r},
        {0x10925D8u, &::getlc2_g_0x10925d8, &a6ad::getlc2_g},
        {0x10925E8u, &::getlc2_b_0x10925e8, &a6ad::getlc2_b},
        {0x10925F8u, &::getlv2_h_0x10925f8, &a6ad::getlv2_h},
        {0x1092608u, &::getlv2_p_0x1092608, &a6ad::getlv2_p},
        {0x1092618u, &::getlv2_b_0x1092618, &a6ad::getlv2_b},
        {0x1092628u, &::lightsw3_0x1092628, &a6ad::lightsw3},
        {0x1092638u, &::getamb_r_0x1092638, &a6ad::getamb_r},
        {0x1092648u, &::getamb_g_0x1092648, &a6ad::getamb_g},
        {0x1092658u, &::getamb_b_0x1092658, &a6ad::getamb_b},
        {0x1092790u, &::CopyAmbFromAmbdef_0x1092790, &a6ad::CopyAmbFromAmbdef},
        {0x10927B0u, &::getLightVector_0x10927b0, &a6ad::getLightVector},
        {0x10965C8u, &::manager_sub_010965C8_0x10965c8, &a6ad::manager_sub_010965C8},
        {0x1099420u, &::modelNameByObj_0x1099420, &a6ad::modelNameByObj},
        {0x1099430u, &::modelNameByHeader_0x1099430, &a6ad::modelNameByHeader},
        {0x1099560u, &::modelGetTextureObjByIndex_0x1099560, &a6ad::modelGetTextureObjByIndex},
        {0x1099578u, &::modelGetTextureResourceByIndex_0x1099578, &a6ad::modelGetTextureResourceByIndex},
        {0x10995D0u, &::modelGetLinkCount_0x10995d0, &a6ad::modelGetLinkCount},
        {0x10995E8u, &::modelIncLinkCount_0x10995e8, &a6ad::modelIncLinkCount},
        {0x1099620u, &::modelDecLinkCount_0x1099620, &a6ad::modelDecLinkCount},
        {0x1099750u, &::modelGetName_0x1099750, &a6ad::modelGetName},
        {0x109ACC8u, &::meshGetWorkSize_0x109acc8, &a6ad::meshGetWorkSize},
        {0x109BE98u, &::reflectionInit_0x109be98, &a6ad::reflectionInit},
        {0x109DE88u, &::getShadowDebug_0x109de88, &a6ad::getShadowDebug},
        {0x109DE98u, &::setShadowIntensity_0x109de98, &a6ad::setShadowIntensity},
        {0x109E290u, &::getShadowDispFlag_0x109e290, &a6ad::getShadowDispFlag},
        {0x10A17A8u, &::getSolarFlarePanelZ_0x10a17a8, &a6ad::getSolarFlarePanelZ},
        {0x10A17B8u, &::getSolarFlareReduceOfsU_0x10a17b8, &a6ad::getSolarFlareReduceOfsU},
        {0x10A17C8u, &::getSolarFlareReduceOfsV_0x10a17c8, &a6ad::getSolarFlareReduceOfsV},
        {0x10A17D8u, &::getSolarFlareZoomOfsU_0x10a17d8, &a6ad::getSolarFlareZoomOfsU},
        {0x10A17E8u, &::getSolarFlareZoomOfsV_0x10a17e8, &a6ad::getSolarFlareZoomOfsV},
        {0x10A54E0u, &::getSpriteDispFlag_0x10a54e0, &a6ad::getSpriteDispFlag},
        {0x10AABD0u, &::texName_0x10aabd0, &a6ad::texName},
        {0x10AAC28u, &::texNameRes_0x10aac28, &a6ad::texNameRes},
        {0x10AAC78u, &::texGetCSM1Idx_0x10aac78, &a6ad::texGetCSM1Idx},
        {0x10AAC90u, &::texGetTexByteSize_0x10aac90, &a6ad::texGetTexByteSize},
        {0x10AACF0u, &::texGetSize_0x10aacf0, &a6ad::texGetSize},
        {0x10AB088u, &::texGetTextureModuleGroup_0x10ab088, &a6ad::texGetTextureModuleGroup},
        {0x10AB190u, &::texDelete_0x10ab190, &a6ad::texDelete},
        {0x10AB660u, &::texNameResErrorCheck_0x10ab660, &a6ad::texNameResErrorCheck},
        {0x10AB6B0u, &::getTexTransEnableFlag_0x10ab6b0, &a6ad::getTexTransEnableFlag},
        {0x10AB6C0u, &::texSetModelName_0x10ab6c0, &a6ad::texSetModelName},
        {0x10AC410u, &::getZFogWidth_0x10ac410, &a6ad::getZFogWidth},
        {0x10AC420u, &::getZFogFarMargin_0x10ac420, &a6ad::getZFogFarMargin},
        {0x10AC430u, &::getZFogFar_0x10ac430, &a6ad::getZFogFar},
        {0x10AC450u, &::getBgFogOffset_0x10ac450, &a6ad::getBgFogOffset},
        {0x10B61B0u, &::SetTRS_Onake_From_ObjectAnimation_0x10b61b0, &a6ad::SetTRS_Onake_From_ObjectAnimation},
        {0x10B6C78u, &::GetMotionFrameWithOutsideLimit_0x10b6c78, &a6ad::GetMotionFrameWithOutsideLimit},
        {0x10B6FD0u, &::GetMotionNodeParentBaseQuaternion_0x10b6fd0, &a6ad::GetMotionNodeParentBaseQuaternion},
        {0x10B74B8u, &::DebugDumpMotion_0x10b74b8, &a6ad::DebugDumpMotion},
        {0x10B74D8u, &::getPosPQSAnimIntFrame_0x10b74d8, &a6ad::getPosPQSAnimIntFrame},
        {0x10B7640u, &::SetMotionDynamicCallFunc_0x10b7640, &a6ad::SetMotionDynamicCallFunc},
        {0x10B7CF0u, &::GetMotionMatrixOfChildsFromTarget_0x10b7cf0, &a6ad::GetMotionMatrixOfChildsFromTarget},
        {0x10B83D8u, &::GetBlendedMotion_0x10b83d8, &a6ad::GetBlendedMotion},
        {0x10B8488u, &::GetBlendedMotionRoot_0x10b8488, &a6ad::GetBlendedMotionRoot},
        {0x10B8CD0u, &::CopyMultiQuaternionFromMotionRotElem_0x10b8cd0, &a6ad::CopyMultiQuaternionFromMotionRotElem},
        {0x10B8F50u, &::InitAnimationData_0x10b8f50, &a6ad::InitAnimationData},
        {0x10B8F58u, &::getBlendFirstMotion_0x10b8f58, &a6ad::getBlendFirstMotion},
        {0x10B9328u, &::update_node_by_quater_motion_ik_base_0x10b9328, &a6ad::update_node_by_quater_motion_ik_base},
        {0x10BA138u, &::is_doing_motion_ik_interest_0x10ba138, &a6ad::is_doing_motion_ik_interest},
        {0x10BA550u, &::constructor_neck_turn_0x10ba550, &a6ad::constructor_neck_turn},
        {0x10BA578u, &::begin_neck_turn_0x10ba578, &a6ad::begin_neck_turn},
        {0x10BA5B0u, &::request_end_neck_turn_0x10ba5b0, &a6ad::request_end_neck_turn},
        {0x10BA5D0u, &::is_doit_neck_turn_need_0x10ba5d0, &a6ad::is_doit_neck_turn_need},
        {0x10BA688u, &::manager_sub_010BA688_0x10ba688, &a6ad::manager_sub_010BA688},
        {0x10BB050u, &::GetAnimationDefObjLeaveType_0x10bb050, &a6ad::GetAnimationDefObjLeaveType},
        {0x10BB238u, &::GetAnimationDefObjWorkSize_0x10bb238, &a6ad::GetAnimationDefObjWorkSize},
        {0x10C0C58u, &::GetCharacterClassInitParamNbNodes_0x10c0c58, &a6ad::GetCharacterClassInitParamNbNodes},
        {0x10C0F90u, &::GetCharacterClassControlDirection_0x10c0f90, &a6ad::GetCharacterClassControlDirection},
        {0x10C2970u, &::SetCharacterObjColSphereOverrideFunc_0x10c2970, &a6ad::SetCharacterObjColSphereOverrideFunc},
        {0x10C2BB8u, &::SetCharacterObjScriptKeyEnableFlagOnDemoOrient_0x10c2bb8, &a6ad::SetCharacterObjScriptKeyEnableFlagOnDemoOrient},
        {0x10C2E60u, &::getMotionDebugPrintMode_0x10c2e60, &a6ad::getMotionDebugPrintMode},
        {0x10C2E70u, &::getAddMotionEnableFlag_0x10c2e70, &a6ad::getAddMotionEnableFlag},
        {0x10C2E80u, &::getIkControlEnableFlag_0x10c2e80, &a6ad::getIkControlEnableFlag},
        {0x10C2E90u, &::getRootMoveDisableFlag_0x10c2e90, &a6ad::getRootMoveDisableFlag},
        {0x10C2EA0u, &::dispCollisionFinderFlag_0x10c2ea0, &a6ad::dispCollisionFinderFlag},
        {0x10C2EB0u, &::dispCollisionKeepAreaFlag_0x10c2eb0, &a6ad::dispCollisionKeepAreaFlag},
        {0x10C2EC0u, &::getCollisionDisableAtDirectPlayFlag_0x10c2ec0, &a6ad::getCollisionDisableAtDirectPlayFlag},
        {0x10C3298u, &::ResetCharacterClassIKNodeInterpInfo_0x10c3298, &a6ad::ResetCharacterClassIKNodeInterpInfo},
        {0x10C3448u, &::EnableCharacterObjAdditionMotion_0x10c3448, &a6ad::EnableCharacterObjAdditionMotion},
        {0x10C3478u, &::DisableCharacterObjAdditionMotion_0x10c3478, &a6ad::DisableCharacterObjAdditionMotion},
        {0x10C34A8u, &::DisableCharacterObjAddParentVelocity_0x10c34a8, &a6ad::DisableCharacterObjAddParentVelocity},
        {0x10C34D8u, &::EnableCharacterObjAddParentVelocity_0x10c34d8, &a6ad::EnableCharacterObjAddParentVelocity},
        {0x10CC6E8u, &::getMotionPrintTarget_0x10cc6e8, &a6ad::getMotionPrintTarget},
        {0x10CC6F8u, &::getLocalAccelDispFlag_0x10cc6f8, &a6ad::getLocalAccelDispFlag},
        {0x10CC708u, &::getScriptSwitchDispSw_0x10cc708, &a6ad::getScriptSwitchDispSw},
        {0x10CC718u, &::resetOutsideScriptCtrlStatus_0x10cc718, &a6ad::resetOutsideScriptCtrlStatus},
        {0x10CC720u, &::GetPadControlFlag_0x10cc720, &a6ad::GetPadControlFlag},
        {0x10D2A38u, &::SearchSubAnimation_0x10d2a38, &a6ad::SearchSubAnimation},
        {0x10D2AB0u, &::RequestCharacterClassMotionChange_0x10d2ab0, &a6ad::RequestCharacterClassMotionChange},
        {0x10D2B80u, &::RequestCharacterClassMotionDirectionByGPAirBalanceDir_0x10d2b80, &a6ad::RequestCharacterClassMotionDirectionByGPAirBalanceDir},
        {0x10D2CD8u, &::CancelCharacterClassMotionChange_0x10d2cd8, &a6ad::CancelCharacterClassMotionChange},
        {0x10D2FC8u, &::getMotionInterpMode_0x10d2fc8, &a6ad::getMotionInterpMode},
        {0x10D2FD8u, &::getJumpSpeedLimitEnableFlag_0x10d2fd8, &a6ad::getJumpSpeedLimitEnableFlag},
        {0x10DDE08u, &::charrootdyn_setEdgeUpperPlaneInfoFlag_0x10dde08, &a6ad::charrootdyn_setEdgeUpperPlaneInfoFlag},
        {0x10DDE18u, &::charrootdyn_resetEdgeUpperPlaneInfoFlag_0x10dde18, &a6ad::charrootdyn_resetEdgeUpperPlaneInfoFlag},
        {0x10DE340u, &::charrootdyn_getForceLimitMode_0x10de340, &a6ad::charrootdyn_getForceLimitMode},
        {0x10DF560u, &::getEdgeHangDebugFlag_0x10df560, &a6ad::getEdgeHangDebugFlag},
        {0x10DF570u, &::getAirResistEnableFlag_0x10df570, &a6ad::getAirResistEnableFlag},
        {0x10DF580u, &::getDisableExtendWallFlag_0x10df580, &a6ad::getDisableExtendWallFlag},
        {0x10E0010u, &::manager_sub_010E0010_0x10e0010, &a6ad::manager_sub_010E0010},
        {0x10E1EB8u, &::getIkExpressionSwBool_0x10e1eb8, &a6ad::getIkExpressionSwBool},
        {0x10E1EC8u, &::getRootFittingEnableFlag_0x10e1ec8, &a6ad::getRootFittingEnableFlag},
        {0x10E1ED8u, &::getRootFittingMode_0x10e1ed8, &a6ad::getRootFittingMode},
        {0x10E1EE8u, &::getIkExpressionSw_0x10e1ee8, &a6ad::getIkExpressionSw},
        {0x10ED538u, &::GetFogColMan_0x10ed538, &a6ad::GetFogColMan},
        {0x10ED548u, &::GetFogColClass_0x10ed548, &a6ad::GetFogColClass},
        {0x10ED568u, &::ForceFogColFlagEnvelope_0x10ed568, &a6ad::ForceFogColFlagEnvelope},
        {0x10ED5D8u, &::manager_sub_010ED5D8_0x10ed5d8, &a6ad::manager_sub_010ED5D8},
        {0x10EEAD0u, &::GetCenterPositionOfJudgeVolumeInfo_0x10eead0, &a6ad::GetCenterPositionOfJudgeVolumeInfo},
        {0x10F3030u, &::GetLayoutClassCollisionBoundaryInfo_0x10f3030, &a6ad::GetLayoutClassCollisionBoundaryInfo},
        {0x10F3398u, &::GetLayoutRootMatrix_0x10f3398, &a6ad::GetLayoutRootMatrix},
        {0x10F3450u, &::getSeqNo_0x10f3450, &a6ad::getSeqNo},
        {0x10F5B08u, &::getSubObjDispSw_0x10f5b08, &a6ad::getSubObjDispSw},
        {0x10F5B18u, &::getMotionBlurSw_0x10f5b18, &a6ad::getMotionBlurSw},
        {0x10F5B28u, &::motionBlurTestType_0x10f5b28, &a6ad::motionBlurTestType},
        {0x10F5BC0u, &::manager_sub_010F5BC0_0x10f5bc0, &a6ad::manager_sub_010F5BC0},
        {0x10F6570u, &::getColLightDebugSts_0x10f6570, &a6ad::getColLightDebugSts},
        {0x10F6580u, &::GetColLightDebugSts_0x10f6580, &a6ad::GetColLightDebugSts},
        {0x10F74E0u, &::deleteGameLightObj_0x10f74e0, &a6ad::deleteGameLightObj},
        {0x10F7830u, &::GameLightCalcOutSideParam_0x10f7830, &a6ad::GameLightCalcOutSideParam},
        {0x10F78B8u, &::InitGameLightClass_0x10f78b8, &a6ad::InitGameLightClass},
        {0x10F78C0u, &::GameLightCalcImpact_0x10f78c0, &a6ad::GameLightCalcImpact},
        {0x10F8B50u, &::deleteGlobalLightObj_0x10f8b50, &a6ad::deleteGlobalLightObj},
        {0x10F8BE0u, &::GlobalLightCalcOutSideParam_0x10f8be0, &a6ad::GlobalLightCalcOutSideParam},
        {0x10F9270u, &::GetNextWorkLightClass_0x10f9270, &a6ad::GetNextWorkLightClass},
        {0x10F9300u, &::GetDirColorFromWorkLight_0x10f9300, &a6ad::GetDirColorFromWorkLight},
        {0x10F93F0u, &::GetLightClass_0x10f93f0, &a6ad::GetLightClass},
        {0x10F9530u, &::InitAmbientLightObj_0x10f9530, &a6ad::InitAmbientLightObj},
        {0x10FAC78u, &::manager_sub_010FAC78_0x10fac78, &a6ad::manager_sub_010FAC78},
        {0x10FC190u, &::IsLwsOrientRootNode_0x10fc190, &a6ad::IsLwsOrientRootNode},
        {0x1100938u, &::initSkelObjResource_0x1100938, &a6ad::initSkelObjResource},
        {0x11009A8u, &::initDispObjResource_0x11009a8, &a6ad::initDispObjResource},
        {0x1100A18u, &::initShadowObjResource_0x1100a18, &a6ad::initShadowObjResource},
        {0x1100A88u, &::initColObjResource_0x1100a88, &a6ad::initColObjResource},
        {0x1100B68u, &::initClothObjResource_0x1100b68, &a6ad::initClothObjResource},
        {0x1101690u, &::AddObjectManagerCollisionWorkSizeBoss_0x1101690, &a6ad::AddObjectManagerCollisionWorkSizeBoss},
        {0x11016A8u, &::AddObjectManagerCollisionWorkSizeStage_0x11016a8, &a6ad::AddObjectManagerCollisionWorkSizeStage},
        {0x11016D8u, &::AddObjectManagerGameObjectSize_0x11016d8, &a6ad::AddObjectManagerGameObjectSize},
        {0x11016F0u, &::AddObjectManagerSeamlessWorkSize_0x11016f0, &a6ad::AddObjectManagerSeamlessWorkSize},
        {0x1101708u, &::getObjInitMessageDebugSw_0x1101708, &a6ad::getObjInitMessageDebugSw},
        {0x1101728u, &::GetModelObjGroup_0x1101728, &a6ad::GetModelObjGroup},
        {0x11023C8u, &::deleteAmbientLightObj_0x11023c8, &a6ad::deleteAmbientLightObj},
        {0x11023D0u, &::getMemoryInfoMode_0x11023d0, &a6ad::getMemoryInfoMode},
        {0x11023E0u, &::getMemoryInfoLightSw_0x11023e0, &a6ad::getMemoryInfoLightSw},
        {0x1102C78u, &::getStackViewFlag_0x1102c78, &a6ad::getStackViewFlag},
        {0x11030A8u, &::manager_sub_011030A8_0x11030a8, &a6ad::manager_sub_011030A8},
        {0x110A828u, &::getLWSDebugMode_0x110a828, &a6ad::getLWSDebugMode},
        {0x110A8A8u, &::SetScriptAnimationObjGeometryOverrideCallBackFunc_0x110a8a8, &a6ad::SetScriptAnimationObjGeometryOverrideCallBackFunc},
        {0x110BE10u, &::GetScriptAnimationObjResourcePlayTime_0x110be10, &a6ad::GetScriptAnimationObjResourcePlayTime},
        {0x110C130u, &::getLWSDebugMode2_0x110c130, &a6ad::getLWSDebugMode2},
        {0x110C140u, &::getExternalLWSFlag_0x110c140, &a6ad::getExternalLWSFlag},
        {0x110C7E8u, &::GetMotionResourceTime_0x110c7e8, &a6ad::GetMotionResourceTime},
        {0x110D918u, &::SetScrLwsOriObjReleaseCallBackFunc_0x110d918, &a6ad::SetScrLwsOriObjReleaseCallBackFunc},
        {0x110D9F0u, &::SetScriptLwsOrientObjDefaultScriptFunc_0x110d9f0, &a6ad::SetScriptLwsOrientObjDefaultScriptFunc},
        {0x1111E08u, &::SpriteMenuGetSpriteMenuClassParam_0x1111e08, &a6ad::SpriteMenuGetSpriteMenuClassParam},
        {0x1112088u, &::SpriteMenuGetPartsParamByHash_0x1112088, &a6ad::SpriteMenuGetPartsParamByHash},
        {0x1112260u, &::SpriteMenuGetPartsDefByHash_0x1112260, &a6ad::SpriteMenuGetPartsDefByHash},
        {0x1112458u, &::SpriteMenuGetPartsDefValueByHash_0x1112458, &a6ad::SpriteMenuGetPartsDefValueByHash},
        {0x1112650u, &::SpriteMenuSetPartsDefValueByHash_0x1112650, &a6ad::SpriteMenuSetPartsDefValueByHash},
        {0x1112C10u, &::SpriteMenuGetMenuIdByPartsId_0x1112c10, &a6ad::SpriteMenuGetMenuIdByPartsId},
        {0x1112EA0u, &::SpriteMenuGetSelectMenuId_0x1112ea0, &a6ad::SpriteMenuGetSelectMenuId},
        {0x11130D8u, &::SpriteMenuSetSelectMenuMode_0x11130d8, &a6ad::SpriteMenuSetSelectMenuMode},
        {0x1113260u, &::SpriteMenuSetSliderValueByHash_0x1113260, &a6ad::SpriteMenuSetSliderValueByHash},
        {0x1113448u, &::SpriteMenuSetCheckBoxByPartsId_0x1113448, &a6ad::SpriteMenuSetCheckBoxByPartsId},
        {0x11134B0u, &::SpriteMenuSetCheckBoxByHash_0x11134b0, &a6ad::SpriteMenuSetCheckBoxByHash},
        {0x1113B00u, &::spriteMenuCheckSelectPushPad_0x1113b00, &a6ad::spriteMenuCheckSelectPushPad},
        {0x1114190u, &::FlushSpriteObj_0x1114190, &a6ad::FlushSpriteObj},
        {0x1114258u, &::GetSpriteObjWorkSize_0x1114258, &a6ad::GetSpriteObjWorkSize},
        {0x11143A0u, &::ExecScriptSpriteObj_0x11143a0, &a6ad::ExecScriptSpriteObj},
        {0x1114B38u, &::getMicroCodeAddress_0x1114b38, &a6ad::getMicroCodeAddress},
        {0x1114C30u, &::resetMicroCodeManager_0x1114c30, &a6ad::resetMicroCodeManager},
        {0x11240A0u, &::ResetCollisionDebug_0x11240a0, &a6ad::ResetCollisionDebug},
        {0x11280C0u, &::CopyCollisionEdgePointer_0x11280c0, &a6ad::CopyCollisionEdgePointer},
        {0x1128CB0u, &::getVolumeCollisionDebugMode_0x1128cb0, &a6ad::getVolumeCollisionDebugMode},
        {0x1128CC0u, &::getCollisionDebugGraphMode_0x1128cc0, &a6ad::getCollisionDebugGraphMode},
        {0x1128CD0u, &::getCollisionVirtualWallDispFlag_0x1128cd0, &a6ad::getCollisionVirtualWallDispFlag},
        {0x1129068u, &::GetColMultiDepVtxInfoVertexNormal_0x1129068, &a6ad::GetColMultiDepVtxInfoVertexNormal},
        {0x11290B8u, &::GetColMultiDepVtxInfoVertexNormalWork_0x11290b8, &a6ad::GetColMultiDepVtxInfoVertexNormalWork},
        {0x1129108u, &::iasmCalcCollisionMultiDepSurfBoundarySub_0x1129108, &a6ad::iasmCalcCollisionMultiDepSurfBoundarySub},
        {0x1129248u, &::SetColDebug_0x1129248, &a6ad::SetColDebug},
        {0x1129400u, &::manager_sub_01129400_0x1129400, &a6ad::manager_sub_01129400},
        {0x1129628u, &::MakeGeometryPointerGlobalInfo_0x1129628, &a6ad::MakeGeometryPointerGlobalInfo},
        {0x1129898u, &::MakeCollisionPointGlobalInfo_0x1129898, &a6ad::MakeCollisionPointGlobalInfo},
        {0x1129AF0u, &::objfilter_0x1129af0, &a6ad::objfilter},
        {0x112A4E0u, &::DebugSetCollisionWideAreaSearchGroup_0x112a4e0, &a6ad::DebugSetCollisionWideAreaSearchGroup},
        {0x112A4F0u, &::InitCollisionWideAreaSearchInfo_0x112a4f0, &a6ad::InitCollisionWideAreaSearchInfo},
        {0x112A628u, &::CancelRequestCollisionWideAreaSearch_0x112a628, &a6ad::CancelRequestCollisionWideAreaSearch},
        {0x1131D70u, &::manager_sub_01131D70_0x1131d70, &a6ad::manager_sub_01131D70},
        {0x1132998u, &::SetGeometryPointerGlobalStatePosPointer_0x1132998, &a6ad::SetGeometryPointerGlobalStatePosPointer},
        {0x11340F8u, &::GetPlaneGlobalYFittingPoint_0x11340f8, &a6ad::GetPlaneGlobalYFittingPoint},
        {0x1134348u, &::GetYDifferenceOfPlaneXZProjection_0x1134348, &a6ad::GetYDifferenceOfPlaneXZProjection},
        {0x11349E8u, &::IsOppositePlaneLine_0x11349e8, &a6ad::IsOppositePlaneLine},
        {0x1134A40u, &::GetPlaneFrom3Position_0x1134a40, &a6ad::GetPlaneFrom3Position},
        {0x1134AB8u, &::GetCrossPositionOf3Plane_0x1134ab8, &a6ad::GetCrossPositionOf3Plane},
        {0x11351B0u, &::CopyMultiQuatPos_0x11351b0, &a6ad::CopyMultiQuatPos},
        {0x1135B20u, &::CopyQuatPos_0x1135b20, &a6ad::CopyQuatPos},
        {0x1136380u, &::CheckEllipticalSphereVolumeWithInverseMatrix_0x1136380, &a6ad::CheckEllipticalSphereVolumeWithInverseMatrix},
        {0x1136BE0u, &::CheckCylinderVolumeWithInverseMatrix_0x1136be0, &a6ad::CheckCylinderVolumeWithInverseMatrix},
        {0x113B408u, &::manager_sub_0113B408_0x113b408, &a6ad::manager_sub_0113B408},
        {0x113FA28u, &::get_particle_name_from_effect_id_0x113fa28, &a6ad::get_particle_name_from_effect_id},
        {0x113FAA0u, &::get_particle_effect_from_lod_0x113faa0, &a6ad::get_particle_effect_from_lod},
        {0x113FAD8u, &::quit_particle_texture_user_by_id_0x113fad8, &a6ad::quit_particle_texture_user_by_id},
        {0x113FB88u, &::getPtclDisp_0x113fb88, &a6ad::getPtclDisp},
        {0x113FB98u, &::get_particle_printf_disp_level_0x113fb98, &a6ad::get_particle_printf_disp_level},
        {0x113FBA8u, &::get_particle_camera_data_0x113fba8, &a6ad::get_particle_camera_data},
        {0x113FBB8u, &::is_proper_particle_texture_id_0x113fbb8, &a6ad::is_proper_particle_texture_id},
        {0x113FF90u, &::particleGWorkPut_0x113ff90, &a6ad::particleGWorkPut},
        {0x1140228u, &::add_particle_texture_user_0x1140228, &a6ad::add_particle_texture_user},
        {0x11403F8u, &::set_act_particle_lod_select_flag_0x11403f8, &a6ad::set_act_particle_lod_select_flag},
        {0x1140408u, &::set_disp_particle_collisin_debug_flag_0x1140408, &a6ad::set_disp_particle_collisin_debug_flag},
        {0x11404C0u, &::init_texture_animation_parameter_0x11404c0, &a6ad::init_texture_animation_parameter},
        {0x1140548u, &::get_particle_geodispobj_group_0x1140548, &a6ad::get_particle_geodispobj_group},
        {0x1140558u, &::get_set_limit_gl_draw_particle_flag_0x1140558, &a6ad::get_set_limit_gl_draw_particle_flag},
        {0x1140568u, &::create_particle_texture_0x1140568, &a6ad::create_particle_texture},
        {0x11410D0u, &::manager_sub_011410D0_0x11410d0, &a6ad::manager_sub_011410D0},
        {0x1145C70u, &::manager_sub_01145C70_0x1145c70, &a6ad::manager_sub_01145C70},
        {0x1145CD0u, &::manager_sub_01145CD0_0x1145cd0, &a6ad::manager_sub_01145CD0},
        {0x11461C0u, &::dynamic_cast_to_wind_emitter_sphere_0x11461c0, &a6ad::dynamic_cast_to_wind_emitter_sphere},
        {0x11461E0u, &::dynamic_cast_to_wind_emitter_box_0x11461e0, &a6ad::dynamic_cast_to_wind_emitter_box},
        {0x1146208u, &::dynamic_cast_to_wind_emitter_judge_volume_0x1146208, &a6ad::dynamic_cast_to_wind_emitter_judge_volume},
        {0x1146C08u, &::constructor_wind_effect_data_0x1146c08, &a6ad::constructor_wind_effect_data},
        {0x1146C30u, &::is_ambient_directional_wind_emitter_0x1146c30, &a6ad::is_ambient_directional_wind_emitter},
        {0x1146C40u, &::get_wind_emitter_direct_0x1146c40, &a6ad::get_wind_emitter_direct},
        {0x1146C58u, &::get_wind_effect_data_direct_0x1146c58, &a6ad::get_wind_effect_data_direct},
        {0x1147A20u, &::DeleteModifiedManager_0x1147a20, &a6ad::DeleteModifiedManager},
        {0x114A310u, &::mcSetGameClearFlag_0x114a310, &a6ad::mcSetGameClearFlag},
        {0x114A340u, &::mcSetCompleteTimeAttack_0x114a340, &a6ad::mcSetCompleteTimeAttack},
        {0x114A5D8u, &::mcReqSync_0x114a5d8, &a6ad::mcReqSync},
        {0x114AD50u, &::mouseClearStatus_0x114ad50, &a6ad::mouseClearStatus},
        {0x114B6F0u, &::usbdevKeybdGetStatus_0x114b6f0, &a6ad::usbdevKeybdGetStatus},
        {0x114F298u, &::cursorLockParamMenu_0x114f298, &a6ad::cursorLockParamMenu},
        {0x114F990u, &::manager_sub_0114F990_0x114f990, &a6ad::manager_sub_0114F990},
        {0x114FB10u, &::manager_sub_0114FB10_0x114fb10, &a6ad::manager_sub_0114FB10},
        {0x1150C78u, &::manager_sub_01150C78_0x1150c78, &a6ad::manager_sub_01150C78},
        {0x1152D80u, &::manager_sub_01152D80_0x1152d80, &a6ad::manager_sub_01152D80},
        {0x1154080u, &::manager_sub_01154080_0x1154080, &a6ad::manager_sub_01154080},
        {0x1155B60u, &::windowDisplayMode_0x1155b60, &a6ad::windowDisplayMode},
        {0x1155B70u, &::windowFontColor_0x1155b70, &a6ad::windowFontColor},
        {0x1155ED8u, &::windowCheckCursorInRect_0x1155ed8, &a6ad::windowCheckCursorInRect},
        {0x1155FA0u, &::windowGetCursorPos_0x1155fa0, &a6ad::windowGetCursorPos},
        {0x1156018u, &::windowGetCursorPosOnWindow_0x1156018, &a6ad::windowGetCursorPosOnWindow},
        {0x11560E8u, &::windowGetMouseStatus_0x11560e8, &a6ad::windowGetMouseStatus},
        {0x1156108u, &::windowGetTargetWindowHandle_0x1156108, &a6ad::windowGetTargetWindowHandle},
        {0x11561B0u, &::windowCloseRequest_0x11561b0, &a6ad::windowCloseRequest},
        {0x1156628u, &::GetWindowClass_0x1156628, &a6ad::GetWindowClass},
        {0x1157CC0u, &::manager_sub_01157CC0_0x1157cc0, &a6ad::manager_sub_01157CC0},
        {0x1157CD0u, &::manager_sub_01157CD0_0x1157cd0, &a6ad::manager_sub_01157CD0},
        {0x1157CE0u, &::manager_sub_01157CE0_0x1157ce0, &a6ad::manager_sub_01157CE0},
        {0x1157CF0u, &::manager_sub_01157CF0_0x1157cf0, &a6ad::manager_sub_01157CF0},
        {0x1157E90u, &::manager_sub_01157E90_0x1157e90, &a6ad::manager_sub_01157E90},
        {0x1157F38u, &::manager_sub_01157F38_0x1157f38, &a6ad::manager_sub_01157F38},
        {0x1158848u, &::seSetDebugTrigFunc_0x1158848, &a6ad::seSetDebugTrigFunc},
        {0x115A0B8u, &::CreateEndAutoModFile_0x115a0b8, &a6ad::CreateEndAutoModFile},
        {0x115EC28u, &::sheetIdToName_0x115ec28, &a6ad::sheetIdToName},
        {0x115FE30u, &::getSheetDebugNoLoadMode_0x115fe30, &a6ad::getSheetDebugNoLoadMode},
        {0x1160160u, &::getDebPcWinSw_0x1160160, &a6ad::getDebPcWinSw},
        {0x1160170u, &::GetDebPcWinSw_0x1160170, &a6ad::GetDebPcWinSw},
        {0x1160180u, &::getDebPcWinType_0x1160180, &a6ad::getDebPcWinType},
        {0x1160190u, &::GetDebPcWinType_0x1160190, &a6ad::GetDebPcWinType},
        {0x11601A0u, &::getDebPcWinCategory_0x11601a0, &a6ad::getDebPcWinCategory},
        {0x11601B0u, &::GetDebPcWinCategory_0x11601b0, &a6ad::GetDebPcWinCategory},
        {0x11601C0u, &::toggleDebPcWinWindow_0x11601c0, &a6ad::toggleDebPcWinWindow},
        {0x1160C58u, &::SetDefaultViewContext_0x1160c58, &a6ad::SetDefaultViewContext},
        {0x1160CC8u, &::SetViewContext_0x1160cc8, &a6ad::SetViewContext},
        {0x1160E08u, &::GetScreenRadiusParticle_0x1160e08, &a6ad::GetScreenRadiusParticle},
        {0x1161C88u, &::is_camera_lws_animation_using_0x1161c88, &a6ad::is_camera_lws_animation_using},
        {0x1161F50u, &::get_motion_data_camera_lws_animation_0x1161f50, &a6ad::get_motion_data_camera_lws_animation},
        {0x1161F68u, &::get_animation_class_camera_lws_animation_0x1161f68, &a6ad::get_animation_class_camera_lws_animation},
        {0x1161F88u, &::is_main_type_camera_lws_animation_0x1161f88, &a6ad::is_main_type_camera_lws_animation},
        {0x1161FB8u, &::is_interest_type_camera_lws_animation_0x1161fb8, &a6ad::is_interest_type_camera_lws_animation},
        {0x1161FD0u, &::is_fixed_point_type_camera_lws_animation_0x1161fd0, &a6ad::is_fixed_point_type_camera_lws_animation},
        {0x1162A38u, &::manager_sub_01162A38_0x1162a38, &a6ad::manager_sub_01162A38},
        {0x1163448u, &::constructor_camera_region_0x1163448, &a6ad::constructor_camera_region},
        {0x1164180u, &::get_max_object_animation_camera_region_0x1164180, &a6ad::get_max_object_animation_camera_region},
        {0x1164188u, &::get_cur_total_object_animation_camera_region_0x1164188, &a6ad::get_cur_total_object_animation_camera_region},
        {0x11641C0u, &::get_col_attr_cam_region_select_flag_0x11641c0, &a6ad::get_col_attr_cam_region_select_flag},
        {0x11641D0u, &::get_col_attr_cam_region_select_index_0x11641d0, &a6ad::get_col_attr_cam_region_select_index},
        {0x1164848u, &::get_screen_size_scale_screen_matrix_0x1164848, &a6ad::get_screen_size_scale_screen_matrix},
        {0x1166E10u, &::SetDamageOffsetStatus_0x1166e10, &a6ad::SetDamageOffsetStatus},
        {0x1166E40u, &::SetDamageClassJudgeExternalFunc_0x1166e40, &a6ad::SetDamageClassJudgeExternalFunc},
        {0x1167098u, &::GetLetterKindVariableData_0x1167098, &a6ad::GetLetterKindVariableData},
        {0x11694D8u, &::getSETriggerTestMode_0x11694d8, &a6ad::getSETriggerTestMode},
        {0x11694E8u, &::AnimeTableInfoDebugMode_0x11694e8, &a6ad::AnimeTableInfoDebugMode},
        {0x1169C50u, &::getEditWorkDebugHeader_0x1169c50, &a6ad::getEditWorkDebugHeader},
        {0x1169C70u, &::getEditWorkDebugData_0x1169c70, &a6ad::getEditWorkDebugData},
        {0x1341310u, &::DebugGetMutekiMode_0x1341310, &a6ad::DebugGetMutekiMode},
        {0x1341650u, &::getForceMemoryAllocSize_0x1341650, &a6ad::getForceMemoryAllocSize},
        {0x13416F8u, &::getStatusMode_0x13416f8, &a6ad::getStatusMode},
        {0x1341708u, &::getDispCollisionPerfFlag_0x1341708, &a6ad::getDispCollisionPerfFlag},
        {0x1341718u, &::getPrintfPerfWarning_0x1341718, &a6ad::getPrintfPerfWarning},
        {0x1341758u, &::is_disp_debug_object_info_0x1341758, &a6ad::is_disp_debug_object_info},
        {0x1341768u, &::getCollisionDispMode_0x1341768, &a6ad::getCollisionDispMode},
        {0x1341778u, &::getGetMutekiMode_0x1341778, &a6ad::getGetMutekiMode},
        {0x1341788u, &::getPlayerPowerDisp_0x1341788, &a6ad::getPlayerPowerDisp},
        {0x1341798u, &::getPlayerPowerMax_0x1341798, &a6ad::getPlayerPowerMax},
        {0x13417A8u, &::getGripCalcCancelMode_0x13417a8, &a6ad::getGripCalcCancelMode},
        {0x1341910u, &::getSnapShotSize_0x1341910, &a6ad::getSnapShotSize},
        {0x1341930u, &::getSnapShotNum_0x1341930, &a6ad::getSnapShotNum},
        {0x1341940u, &::getSnapShotRotation_0x1341940, &a6ad::getSnapShotRotation},
        {0x1341950u, &::getSnapShotAspect_0x1341950, &a6ad::getSnapShotAspect},
        {0x1341960u, &::getDebugSnapShotSizeX_0x1341960, &a6ad::getDebugSnapShotSizeX},
        {0x1341A48u, &::getFreezeBossFlag_0x1341a48, &a6ad::getFreezeBossFlag},
        {0x1341C08u, &::getDispGpsMarker_0x1341c08, &a6ad::getDispGpsMarker},
        {0x1341C18u, &::getGpsGridWidth_0x1341c18, &a6ad::getGpsGridWidth},
        {0x1342A40u, &::convert_from_button_value_to_bool_0x1342a40, &a6ad::convert_from_button_value_to_bool},
        {0x1342B20u, &::GAMEUTIL_set_sight_on_reverse_cntrlLR_0x1342b20, &a6ad::GAMEUTIL_set_sight_on_reverse_cntrlLR},
        {0x1342B30u, &::GAMEUTIL_get_sight_on_reverse_cntrlLR_int_0x1342b30, &a6ad::GAMEUTIL_get_sight_on_reverse_cntrlLR_int},
        {0x1342B50u, &::GAMEUTIL_set_sight_on_reverse_cntrlUD_0x1342b50, &a6ad::GAMEUTIL_set_sight_on_reverse_cntrlUD},
        {0x1342B60u, &::GAMEUTIL_get_sight_on_reverse_cntrlUD_int_0x1342b60, &a6ad::GAMEUTIL_get_sight_on_reverse_cntrlUD_int},
        {0x1342B80u, &::GAMEUTIL_set_sight_on_angle_vel_ratio_0x1342b80, &a6ad::GAMEUTIL_set_sight_on_angle_vel_ratio},
        {0x1342B90u, &::GAMEUTIL_get_sight_on_angle_vel_ratio_int_0x1342b90, &a6ad::GAMEUTIL_get_sight_on_angle_vel_ratio_int},
        {0x1342C50u, &::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR_0x1342c50, &a6ad::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlLR},
        {0x1342C60u, &::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int_0x1342c60, &a6ad::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlLR_int},
        {0x1342C80u, &::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD_0x1342c80, &a6ad::GAMEUTIL_set_game_camera_free_mode_reverse_cntrlUD},
        {0x1342C90u, &::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int_0x1342c90, &a6ad::GAMEUTIL_get_game_camera_free_mode_reverse_cntrlUD_int},
        {0x1342CB0u, &::GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio_0x1342cb0, &a6ad::GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio},
        {0x1342CC0u, &::GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int_0x1342cc0, &a6ad::GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int},
        {0x1342E40u, &::GAMEUTIL_get_obj_from_stagelayout_sheet_0x1342e40, &a6ad::GAMEUTIL_get_obj_from_stagelayout_sheet},
        {0x1343060u, &::GAMEUTIL_is_overtake_aim_position_0x1343060, &a6ad::GAMEUTIL_is_overtake_aim_position},
        {0x13430E0u, &::GAMEUTIL_is_advance_near_dir_to_aim_position_0x13430e0, &a6ad::GAMEUTIL_is_advance_near_dir_to_aim_position},
        {0x1343388u, &::GAMEUTIL_is_sign_change_0x1343388, &a6ad::GAMEUTIL_is_sign_change},
        {0x13443B0u, &::GAMEUTIL_constructor_record_info_0x13443b0, &a6ad::GAMEUTIL_constructor_record_info},
        {0x13444A8u, &::GAMEUTIL_reset_record_info_0x13444a8, &a6ad::GAMEUTIL_reset_record_info},
        {0x1344620u, &::GAMEUTIL_reset_status_continue_time_status_manager_0x1344620, &a6ad::GAMEUTIL_reset_status_continue_time_status_manager},
        {0x1344688u, &::getSightOnAngleVelRatio_0x1344688, &a6ad::getSightOnAngleVelRatio},
        {0x1345190u, &::getCullingScreenSize_0x1345190, &a6ad::getCullingScreenSize},
        {0x13451A0u, &::getScissoringScreenSize_0x13451a0, &a6ad::getScissoringScreenSize},
        {0x13451B0u, &::getPlayerAliveInPauseFlag_0x13451b0, &a6ad::getPlayerAliveInPauseFlag},
        {0x1345D50u, &::PauseAI_0x1345d50, &a6ad::PauseAI},
        {0x1345D60u, &::ReleaseAI_0x1345d60, &a6ad::ReleaseAI},
        {0x1345D70u, &::SwitchPadAct_0x1345d70, &a6ad::SwitchPadAct},
        {0x1345E68u, &::getGameAnimeTimeScaleRatio_0x1345e68, &a6ad::getGameAnimeTimeScaleRatio},
        {0x1345EC0u, &::getDummyCount_0x1345ec0, &a6ad::getDummyCount},
        {0x13461C0u, &::setLayerFade_0x13461c0, &a6ad::setLayerFade},
        {0x1346220u, &::setFadeValue_0x1346220, &a6ad::setFadeValue},
        {0x1346230u, &::getDemoBarSw_0x1346230, &a6ad::getDemoBarSw},
        {0x1347368u, &::set_base_vel_nico_yaw_pitch_cntrl_0x1347368, &a6ad::set_base_vel_nico_yaw_pitch_cntrl},
        {0x1347590u, &::is_doing_nico_yaw_pitch_cntrl_0x1347590, &a6ad::is_doing_nico_yaw_pitch_cntrl},
        {0x1347828u, &::get_offset_yaw_nico_yaw_pitch_cntrl_0x1347828, &a6ad::get_offset_yaw_nico_yaw_pitch_cntrl},
        {0x1347830u, &::get_offset_pitch_nico_yaw_pitch_cntrl_0x1347830, &a6ad::get_offset_pitch_nico_yaw_pitch_cntrl},
        {0x1347850u, &::get_base_pitch_move_nico_yaw_pitch_cntrl_0x1347850, &a6ad::get_base_pitch_move_nico_yaw_pitch_cntrl},
        {0x13487B0u, &::activedemoflowDemoEnd_0x13487b0, &a6ad::activedemoflowDemoEnd},
        {0x13487C8u, &::getActivedemoStage_0x13487c8, &a6ad::getActivedemoStage},
        {0x13487D8u, &::chkActiveDemoStart_0x13487d8, &a6ad::chkActiveDemoStart},
        {0x1348958u, &::activeDemoCancel_0x1348958, &a6ad::activeDemoCancel},
        {0x134B250u, &::SkipCutDemoCameraOrient_0x134b250, &a6ad::SkipCutDemoCameraOrient},
        {0x134BAF0u, &::SetCaptionPauseFlag_0x134baf0, &a6ad::SetCaptionPauseFlag},
        {0x134C348u, &::nico_InitScriptCharObjDefaultValue_0x134c348, &a6ad::nico_InitScriptCharObjDefaultValue},
        {0x134C6D8u, &::SetDefaultStageLoad_0x134c6d8, &a6ad::SetDefaultStageLoad},
        {0x134D850u, &::endingflowDemoEnd_0x134d850, &a6ad::endingflowDemoEnd},
        {0x134DC10u, &::fadeCheck_0x134dc10, &a6ad::fadeCheck},
        {0x134EF68u, &::_optionWorkCls_0x134ef68, &a6ad::_optionWorkCls},
        {0x134F060u, &::_gameFlagXor_0x134f060, &a6ad::_gameFlagXor},
        {0x134F0F0u, &::_gameWorkCls_0x134f0f0, &a6ad::_gameWorkCls},
        {0x134F218u, &::getGameDataExistIcoData_0x134f218, &a6ad::getGameDataExistIcoData},
        {0x134F228u, &::getGameDataExistIcoDataAdrs_0x134f228, &a6ad::getGameDataExistIcoDataAdrs},
        {0x134F248u, &::getBossIndexTimeAttackBestTime_0x134f248, &a6ad::getBossIndexTimeAttackBestTime},
        {0x134F260u, &::setBossIndexTimeAttackBestTime_0x134f260, &a6ad::setBossIndexTimeAttackBestTime},
        {0x134F288u, &::getGameClearCntAdrs_0x134f288, &a6ad::getGameClearCntAdrs},
        {0x134F298u, &::setGmaeClearCnt_0x134f298, &a6ad::setGmaeClearCnt},
        {0x134F2B8u, &::getGameHardModeAdrs_0x134f2b8, &a6ad::getGameHardModeAdrs},
        {0x134F2F0u, &::getSavedGameTime_0x134f2f0, &a6ad::getSavedGameTime},
        {0x134F300u, &::setSavedGameTime_0x134f300, &a6ad::setSavedGameTime},
        {0x134F310u, &::getSavedClock_0x134f310, &a6ad::getSavedClock},
        {0x134F340u, &::setSavedPlaceId_0x134f340, &a6ad::setSavedPlaceId},
        {0x134F740u, &::tailsFlagSet_0x134f740, &a6ad::tailsFlagSet},
        {0x134F798u, &::tailsFlagClr_0x134f798, &a6ad::tailsFlagClr},
        {0x134F880u, &::gexsFlagSet_0x134f880, &a6ad::gexsFlagSet},
        {0x134F8D8u, &::gexsFlagClr_0x134f8d8, &a6ad::gexsFlagClr},
        {0x134F998u, &::_omakeGetFlagSet_0x134f998, &a6ad::_omakeGetFlagSet},
        {0x134F9D8u, &::_omakeGetFlagCls_0x134f9d8, &a6ad::_omakeGetFlagCls},
        {0x134FA48u, &::_omakeSelectFlagCls_0x134fa48, &a6ad::_omakeSelectFlagCls},
        {0x134FBA8u, &::setClearGameHotStartFlag_0x134fba8, &a6ad::setClearGameHotStartFlag},
        {0x134FBD0u, &::getClearGameHotStartFlag_0x134fbd0, &a6ad::getClearGameHotStartFlag},
        {0x1352070u, &::gameflowMpegCancelChk_0x1352070, &a6ad::gameflowMpegCancelChk},
        {0x1352760u, &::gamecore_sub_01352760_0x1352760, &a6ad::gamecore_sub_01352760},
        {0x1352780u, &::gamecore_sub_01352780_0x1352780, &a6ad::gamecore_sub_01352780},
        {0x1352790u, &::gamecore_sub_01352790_0x1352790, &a6ad::gamecore_sub_01352790},
        {0x1352878u, &::gamecore_sub_01352878_0x1352878, &a6ad::gamecore_sub_01352878},
        {0x13535B0u, &::gStatGObjDispOn_0x13535b0, &a6ad::gStatGObjDispOn},
        {0x13535C0u, &::gStatGObjDispOff_0x13535c0, &a6ad::gStatGObjDispOff},
        {0x1353DD8u, &::gstatBootMcChkExistNicoData_0x1353dd8, &a6ad::gstatBootMcChkExistNicoData},
        {0x1356678u, &::checkBoot_0x1356678, &a6ad::checkBoot},
        {0x1356688u, &::reloadStage_0x1356688, &a6ad::reloadStage},
        {0x13566E8u, &::reloadStageWithPlayerPos_0x13566e8, &a6ad::reloadStageWithPlayerPos},
        {0x1358700u, &::getPlayerO2_0x1358700, &a6ad::getPlayerO2},
        {0x1358710u, &::setPlayerO2_0x1358710, &a6ad::setPlayerO2},
        {0x1358740u, &::addPlayerO2_0x1358740, &a6ad::addPlayerO2},
        {0x135DA20u, &::SetSeamlessZFogDist_0x135da20, &a6ad::SetSeamlessZFogDist},
        {0x135DAC8u, &::getSeamlessStageHighModelDispDist_0x135dac8, &a6ad::getSeamlessStageHighModelDispDist},
        {0x135DAD8u, &::getSeamlessStageMiddleModelDispDist_0x135dad8, &a6ad::getSeamlessStageMiddleModelDispDist},
        {0x135DB08u, &::getStageChipIDReal_0x135db08, &a6ad::getStageChipIDReal},
        {0x135DB68u, &::getStageChipIDWithOffset_0x135db68, &a6ad::getStageChipIDWithOffset},
        {0x135DBE8u, &::getStageNormalizePosition_0x135dbe8, &a6ad::getStageNormalizePosition},
        {0x135DC40u, &::getCameraPosOnMap_0x135dc40, &a6ad::getCameraPosOnMap},
        {0x135DCA8u, &::getSeamlessStageCenter_0x135dca8, &a6ad::getSeamlessStageCenter},
        {0x135DD68u, &::getStageLayout_0x135dd68, &a6ad::getStageLayout},
        {0x135DE38u, &::getStageStat_0x135de38, &a6ad::getStageStat},
        {0x135DEB0u, &::GetLoadAreaGroupData_0x135deb0, &a6ad::GetLoadAreaGroupData},
        {0x135DFA8u, &::UpdateSeamlessStageLoad_0x135dfa8, &a6ad::UpdateSeamlessStageLoad},
        {0x135E068u, &::GetStageChipNum_0x135e068, &a6ad::GetStageChipNum},
        {0x135E088u, &::DeleteSeamlessStageObject_0x135e088, &a6ad::DeleteSeamlessStageObject},
        {0x135E128u, &::GetSeamlessStageObjectGroup_0x135e128, &a6ad::GetSeamlessStageObjectGroup},
        {0x135E238u, &::getChipNo_0x135e238, &a6ad::getChipNo},
        {0x135E310u, &::getStageStatByChipNo_0x135e310, &a6ad::getStageStatByChipNo},
        {0x135E410u, &::getStageChipIDFloat_0x135e410, &a6ad::getStageChipIDFloat},
        {0x135E940u, &::RestartSeamlessStage_0x135e940, &a6ad::RestartSeamlessStage},
        {0x1365D58u, &::gcGetNbStages_0x1365d58, &a6ad::gcGetNbStages},
        {0x1365EB8u, &::CheckStageManagerLoadArea_0x1365eb8, &a6ad::CheckStageManagerLoadArea},
        {0x1366660u, &::stagemanager_GetStageAlignParam_0x1366660, &a6ad::stagemanager_GetStageAlignParam},
        {0x1366858u, &::stagemanager_getNbWorldLayoutDatas_0x1366858, &a6ad::stagemanager_getNbWorldLayoutDatas},
        {0x1366890u, &::stagemanager_checkstageAlreadyLoadRequested_0x1366890, &a6ad::stagemanager_checkstageAlreadyLoadRequested},
        {0x1366CE0u, &::getWorldLayoutHead_0x1366ce0, &a6ad::getWorldLayoutHead},
        {0x1366D10u, &::deleteWorldLayout_0x1366d10, &a6ad::deleteWorldLayout},
        {0x1367B78u, &::getBossExtControl_0x1367b78, &a6ad::getBossExtControl},
        {0x1367B88u, &::getTargetAIControlMode_0x1367b88, &a6ad::getTargetAIControlMode},
        {0x1367E30u, &::time_attack_get_best_time_0x1367e30, &a6ad::time_attack_get_best_time},
        {0x1367E68u, &::time_attack_get_boss_status_0x1367e68, &a6ad::time_attack_get_boss_status},
        {0x1367E78u, &::time_attack_end_0x1367e78, &a6ad::time_attack_end},
        {0x136A198u, &::bgmScriptSendSoundSignal_0x136a198, &a6ad::bgmScriptSendSoundSignal},
        {0x136A1B8u, &::bgmScriptRecvSoundSignal_0x136a1b8, &a6ad::bgmScriptRecvSoundSignal},
        {0x136A1F0u, &::bgmDemoPackagePlay_0x136a1f0, &a6ad::bgmDemoPackagePlay},
        {0x136A3A8u, &::gamecore_sub_0136A3A8_0x136a3a8, &a6ad::gamecore_sub_0136A3A8},
        {0x136A3F8u, &::scriptNormalHook_0x136a3f8, &a6ad::scriptNormalHook},
        {0x136A408u, &::gamecore_sub_0136A408_0x136a408, &a6ad::gamecore_sub_0136A408},
        {0x136A410u, &::demoNormalHook_0x136a410, &a6ad::demoNormalHook},
        {0x136AA78u, &::bgmStopAllAttr_0x136aa78, &a6ad::bgmStopAllAttr},
        {0x136AAF0u, &::bgmEndGameOver_0x136aaf0, &a6ad::bgmEndGameOver},
        {0x136AB80u, &::gamecore_sub_0136AB80_0x136ab80, &a6ad::gamecore_sub_0136AB80},
        {0x136F298u, &::gamecore_sub_0136F298_0x136f298, &a6ad::gamecore_sub_0136F298},
        {0x136FD68u, &::bootmccheckscriptNoBoot_0x136fd68, &a6ad::bootmccheckscriptNoBoot},
        {0x13757C0u, &::nico_IK_leafSetWithMotion_0x13757c0, &a6ad::nico_IK_leafSetWithMotion},
        {0x1378868u, &::SCRGetBossPartsDamage_0x1378868, &a6ad::SCRGetBossPartsDamage},
        {0x1379220u, &::GetBossPartsDamage_0x1379220, &a6ad::GetBossPartsDamage},
        {0x137B790u, &::gamecore_sub_0137B790_0x137b790, &a6ad::gamecore_sub_0137B790},
        {0x137C2C0u, &::getDebugDispType_0x137c2c0, &a6ad::getDebugDispType},
        {0x137C2C8u, &::getDebugMotionStop_0x137c2c8, &a6ad::getDebugMotionStop},
        {0x137C2D0u, &::getDebugScrDataPrintf_0x137c2d0, &a6ad::getDebugScrDataPrintf},
        {0x137C2D8u, &::getDebugHorseRoute_0x137c2d8, &a6ad::getDebugHorseRoute},
        {0x137C2E0u, &::getDebugHorseBikko_0x137c2e0, &a6ad::getDebugHorseBikko},
        {0x137C2E8u, &::getDebugParentAttrReset_0x137c2e8, &a6ad::getDebugParentAttrReset},
        {0x137C2F0u, &::getDebugTrigerPrintf_0x137c2f0, &a6ad::getDebugTrigerPrintf},
        {0x137C2F8u, &::getDebugHorseInsurance_0x137c2f8, &a6ad::getDebugHorseInsurance},
        {0x137C300u, &::getDebugHorsePathVirtual_0x137c300, &a6ad::getDebugHorsePathVirtual},
        {0x137C308u, &::getDebugViewDisp_0x137c308, &a6ad::getDebugViewDisp},
        {0x137C318u, &::GetDebugMotionStop_0x137c318, &a6ad::GetDebugMotionStop},
        {0x137C320u, &::GetDebugScrDataPrintf_0x137c320, &a6ad::GetDebugScrDataPrintf},
        {0x137C328u, &::GetDebugViewDisp_0x137c328, &a6ad::GetDebugViewDisp},
        {0x137E238u, &::getBosstatusDispMode_0x137e238, &a6ad::getBosstatusDispMode},
        {0x137E2A8u, &::getBoneCollisionDebugMode_0x137e2a8, &a6ad::getBoneCollisionDebugMode},
        {0x1384E98u, &::AI_GetDeathTentWorkSize_0x1384e98, &a6ad::AI_GetDeathTentWorkSize},
        {0x1384EF8u, &::GetMeshObjInfo_0x1384ef8, &a6ad::GetMeshObjInfo},
        {0x1384F30u, &::AI_GetBaseWorkSize_0x1384f30, &a6ad::AI_GetBaseWorkSize},
        {0x13862C0u, &::getDynamicsTestType_0x13862c0, &a6ad::getDynamicsTestType},
        {0x13862D0u, &::getPendulumGravityRatio_0x13862d0, &a6ad::getPendulumGravityRatio},
        {0x13862E0u, &::getPendulumAirResistRatio_0x13862e0, &a6ad::getPendulumAirResistRatio},
        {0x13862F0u, &::getPendulumSideFriction_0x13862f0, &a6ad::getPendulumSideFriction},
        {0x1386300u, &::dynamicstest_0x1386300, &a6ad::dynamicstest},
        {0x1388618u, &::getBloodEffectEnableFlag_0x1388618, &a6ad::getBloodEffectEnableFlag},
        {0x1388628u, &::gamecore_sub_01388628_0x1388628, &a6ad::gamecore_sub_01388628},
        {0x1388638u, &::gamecore_sub_01388638_0x1388638, &a6ad::gamecore_sub_01388638},
        {0x1388648u, &::gamecore_sub_01388648_0x1388648, &a6ad::gamecore_sub_01388648},
        {0x1388658u, &::gamecore_sub_01388658_0x1388658, &a6ad::gamecore_sub_01388658},
        {0x1388668u, &::gamecore_sub_01388668_0x1388668, &a6ad::gamecore_sub_01388668},
        {0x1388678u, &::gamecore_sub_01388678_0x1388678, &a6ad::gamecore_sub_01388678},
        {0x1388688u, &::getWeakGlowSpeed_0x1388688, &a6ad::getWeakGlowSpeed},
        {0x1388698u, &::getWeakGlowTime_0x1388698, &a6ad::getWeakGlowTime},
        {0x13886A8u, &::getTrigGlowTime_0x13886a8, &a6ad::getTrigGlowTime},
        {0x13888E0u, &::gamecore_sub_013888E0_0x13888e0, &a6ad::gamecore_sub_013888E0},
        {0x138B8E8u, &::gamecore_sub_0138B8E8_0x138b8e8, &a6ad::gamecore_sub_0138B8E8},
        {0x138BE08u, &::set_root_node_data_expression_0x138be08, &a6ad::set_root_node_data_expression},
        {0x138BF08u, &::add_element_expression_change_0x138bf08, &a6ad::add_element_expression_change},
        {0x138F180u, &::gamecore_sub_0138F180_0x138f180, &a6ad::gamecore_sub_0138F180},
        {0x138F978u, &::gamecore_sub_0138F978_0x138f978, &a6ad::gamecore_sub_0138F978},
        {0x1394F18u, &::gamecore_sub_01394F18_0x1394f18, &a6ad::gamecore_sub_01394F18},
        {0x1398678u, &::gamecore_sub_01398678_0x1398678, &a6ad::gamecore_sub_01398678},
        {0x139A090u, &::initbar_0x139a090, &a6ad::initbar},
        {0x139A0F0u, &::setbar_0x139a0f0, &a6ad::setbar},
        {0x139A198u, &::SpriteSetStatus_0x139a198, &a6ad::SpriteSetStatus},
        {0x139A218u, &::Init_TimeFloatInter_0x139a218, &a6ad::Init_TimeFloatInter},
        {0x139A230u, &::Update_TimeFloatInter_0x139a230, &a6ad::Update_TimeFloatInter},
        {0x139A348u, &::Init_TimeFloatSpeed_0x139a348, &a6ad::Init_TimeFloatSpeed},
        {0x139A360u, &::Update_TimeFloatSpeed_0x139a360, &a6ad::Update_TimeFloatSpeed},
        {0x139A928u, &::ColCont_IsColStatus_0x139a928, &a6ad::ColCont_IsColStatus},
        {0x139A9F8u, &::ColCont2_EventAttrReset_0x139a9f8, &a6ad::ColCont2_EventAttrReset},
        {0x139BB70u, &::gamecore_sub_0139BB70_0x139bb70, &a6ad::gamecore_sub_0139BB70},
        {0x139E490u, &::gamecore_sub_0139E490_0x139e490, &a6ad::gamecore_sub_0139E490},
        {0x139E498u, &::gamecore_sub_0139E498_0x139e498, &a6ad::gamecore_sub_0139E498},
        {0x139E4A0u, &::gamecore_sub_0139E4A0_0x139e4a0, &a6ad::gamecore_sub_0139E4A0},
        {0x139E4A8u, &::gamecore_sub_0139E4A8_0x139e4a8, &a6ad::gamecore_sub_0139E4A8},
        {0x13A6FB0u, &::gamecore_sub_013A6FB0_0x13a6fb0, &a6ad::gamecore_sub_013A6FB0},
        {0x13A7298u, &::CalcFootPos_0x13a7298, &a6ad::CalcFootPos},
        {0x13A7318u, &::CalcDistancePointAnd2Point_0x13a7318, &a6ad::CalcDistancePointAnd2Point},
        {0x13A8970u, &::SCRFuncNargaPlayerStop_0x13a8970, &a6ad::SCRFuncNargaPlayerStop},
        {0x13AA390u, &::get_ofst_yaw_nico_sight_on_0x13aa390, &a6ad::get_ofst_yaw_nico_sight_on},
        {0x13AA398u, &::get_ofst_pitch_nico_sight_on_0x13aa398, &a6ad::get_ofst_pitch_nico_sight_on},
        {0x13AA3A8u, &::get_base_yaw_nico_sight_on_0x13aa3a8, &a6ad::get_base_yaw_nico_sight_on},
        {0x13AA408u, &::get_base_pitch_nico_sight_on_0x13aa408, &a6ad::get_base_pitch_nico_sight_on},
        {0x13AA478u, &::get_hope_yaw_nico_sight_on_0x13aa478, &a6ad::get_hope_yaw_nico_sight_on},
        {0x13AA480u, &::get_hope_pitch_nico_sight_on_0x13aa480, &a6ad::get_hope_pitch_nico_sight_on},
        {0x13AA488u, &::get_snipe_position_nico_sight_on_0x13aa488, &a6ad::get_snipe_position_nico_sight_on},
        {0x13B5360u, &::player_AutoHorseCtrlOff_0x13b5360, &a6ad::player_AutoHorseCtrlOff},
        {0x13B5A78u, &::player_IsHorseCtrlOff_0x13b5a78, &a6ad::player_IsHorseCtrlOff},
        {0x13B7AE0u, &::gamecore_sub_013B7AE0_0x13b7ae0, &a6ad::gamecore_sub_013B7AE0},
        {0x13BA560u, &::nico_setPlayerDemoAdditionClothCollisionFloorTable_0x13ba560, &a6ad::nico_setPlayerDemoAdditionClothCollisionFloorTable},
        {0x13BA5A0u, &::nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor_0x13ba5a0, &a6ad::nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor},
        {0x13BA5E0u, &::nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor_0x13ba5e0, &a6ad::nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor},
        {0x13BA628u, &::nico_setPlayerDemoAdditionClothCollisionEvisFloor_0x13ba628, &a6ad::nico_setPlayerDemoAdditionClothCollisionEvisFloor},
        {0x13BA668u, &::nico_setPlayerDemoAdditionClothCollisionRightHand_0x13ba668, &a6ad::nico_setPlayerDemoAdditionClothCollisionRightHand},
        {0x13BA6A8u, &::nico_setPlayerDemoAdditionClothCollisionRightHandY_0x13ba6a8, &a6ad::nico_setPlayerDemoAdditionClothCollisionRightHandY},
        {0x13BA6E8u, &::nico_setPlayerDemoAdditionClothCollisionHand_0x13ba6e8, &a6ad::nico_setPlayerDemoAdditionClothCollisionHand},
        {0x13BA728u, &::nico_setPlayerDemoAdditionClothCollisionLeftHandY_0x13ba728, &a6ad::nico_setPlayerDemoAdditionClothCollisionLeftHandY},
        {0x13BB470u, &::gamecore_sub_013BB470_0x13bb470, &a6ad::gamecore_sub_013BB470},
        {0x13D0AA8u, &::getBossCloudObj_0x13d0aa8, &a6ad::getBossCloudObj},
        {0x13D2580u, &::AI_UpdateVelocityXYZ_0x13d2580, &a6ad::AI_UpdateVelocityXYZ},
        {0x13D2DE8u, &::AI_CheckNoAngleVec_0x13d2de8, &a6ad::AI_CheckNoAngleVec},
        {0x13D3E00u, &::AI_UpdateVelocity2_0x13d3e00, &a6ad::AI_UpdateVelocity2},
        {0x13D45C8u, &::AI_GetVelocity2_0x13d45c8, &a6ad::AI_GetVelocity2},
        {0x13D4C48u, &::AI_GetLengthSquarePerpendicular_0x13d4c48, &a6ad::AI_GetLengthSquarePerpendicular},
        {0x13D5940u, &::AI_DebugDir_0x13d5940, &a6ad::AI_DebugDir},
        {0x13D5948u, &::AI_DebugArrow_0x13d5948, &a6ad::AI_DebugArrow},
        {0x13D5950u, &::AI_DebugCylinder_0x13d5950, &a6ad::AI_DebugCylinder},
        {0x13D5958u, &::AIDebug_GetFirstObjOfMyGroup_0x13d5958, &a6ad::AIDebug_GetFirstObjOfMyGroup},
        {0x13D5960u, &::AIDebug_GetAnimalMode_0x13d5960, &a6ad::AIDebug_GetAnimalMode},
        {0x13D5968u, &::AIDebug_CheckPlayerParent_0x13d5968, &a6ad::AIDebug_CheckPlayerParent},
        {0x13D5970u, &::AIDebug_SetID_0x13d5970, &a6ad::AIDebug_SetID},
        {0x13D5978u, &::AIDebug_GetAnimalDispType_0x13d5978, &a6ad::AIDebug_GetAnimalDispType},
        {0x13D5980u, &::AIDebug_GetAnimalDispID_0x13d5980, &a6ad::AIDebug_GetAnimalDispID},
        {0x13D5988u, &::AIDebug_GetAdvertiseLoopFlag_0x13d5988, &a6ad::AIDebug_GetAdvertiseLoopFlag},
        {0x13D5990u, &::AIDebug_GetAdvertisePauseFlag_0x13d5990, &a6ad::AIDebug_GetAdvertisePauseFlag},
        {0x13D5998u, &::AIDebug_SetAdvertisePauseFlag_0x13d5998, &a6ad::AIDebug_SetAdvertisePauseFlag},
        {0x13D59A0u, &::getDebugAdvertiseLoopFlag_0x13d59a0, &a6ad::getDebugAdvertiseLoopFlag},
        {0x13D59A8u, &::getDebugAnimalMode_0x13d59a8, &a6ad::getDebugAnimalMode},
        {0x13D59B0u, &::getDebugAnimalDispType_0x13d59b0, &a6ad::getDebugAnimalDispType},
        {0x13D59B8u, &::getDebugAnimalDispID_0x13d59b8, &a6ad::getDebugAnimalDispID},
        {0x13D59C0u, &::AIDebug_AdvertisePauseKey_0x13d59c0, &a6ad::AIDebug_AdvertisePauseKey},
        {0x13D7870u, &::gamecore_sub_013D7870_0x13d7870, &a6ad::gamecore_sub_013D7870},
        {0x13E7CB8u, &::Kite_SetKiteObj_0x13e7cb8, &a6ad::Kite_SetKiteObj},
        {0x13EE468u, &::lizard_head_0x13ee468, &a6ad::lizard_head},
        {0x13EE490u, &::lizard_waist_0x13ee490, &a6ad::lizard_waist},
        {0x13EE4B8u, &::lizard_berry_0x13ee4b8, &a6ad::lizard_berry},
        {0x13F0DD8u, &::getSObjNodeBySubObj_0x13f0dd8, &a6ad::getSObjNodeBySubObj},
        {0x13F0E50u, &::setSObjNodeLock_0x13f0e50, &a6ad::setSObjNodeLock},
        {0x13F62D8u, &::setLoadingCheckFunc_0x13f62d8, &a6ad::setLoadingCheckFunc},
        {0x13FA540u, &::setPlayerStatusDisableFadeCompareSwitch_0x13fa540, &a6ad::setPlayerStatusDisableFadeCompareSwitch},
        {0x13FF158u, &::getMinAmbient_0x13ff158, &a6ad::getMinAmbient},
        {0x13FF168u, &::getMaxAmbient_0x13ff168, &a6ad::getMaxAmbient},
        {0x13FF178u, &::getMinAmbientLine_0x13ff178, &a6ad::getMinAmbientLine},
        {0x13FF188u, &::getMaxAmbientLine_0x13ff188, &a6ad::getMaxAmbientLine},
        {0x13FF198u, &::getMinAngle_0x13ff198, &a6ad::getMinAngle},
        {0x13FF1A8u, &::getMaxAngle_0x13ff1a8, &a6ad::getMaxAngle},
        {0x13FF1B8u, &::getMaxLineAngle_0x13ff1b8, &a6ad::getMaxLineAngle},
        {0x13FF1C8u, &::getMinAmbientWeak_0x13ff1c8, &a6ad::getMinAmbientWeak},
        {0x13FF1D8u, &::getMaxAmbientWeak_0x13ff1d8, &a6ad::getMaxAmbientWeak},
        {0x13FF1E8u, &::getMinAmbientLineWeak_0x13ff1e8, &a6ad::getMinAmbientLineWeak},
        {0x13FF1F8u, &::getMaxAmbientLineWeak_0x13ff1f8, &a6ad::getMaxAmbientLineWeak},
        {0x13FF208u, &::getMinAngleWeak_0x13ff208, &a6ad::getMinAngleWeak},
        {0x13FF218u, &::getMaxAngleWeak_0x13ff218, &a6ad::getMaxAngleWeak},
        {0x13FF228u, &::getMaxLineAngleWeak_0x13ff228, &a6ad::getMaxLineAngleWeak},
        {0x13FF238u, &::getMaxFogMulVal_0x13ff238, &a6ad::getMaxFogMulVal},
        {0x13FF248u, &::getFogChangeSpeed_0x13ff248, &a6ad::getFogChangeSpeed},
        {0x13FF258u, &::getSwordLineWidthMax_0x13ff258, &a6ad::getSwordLineWidthMax},
        {0x13FF268u, &::getSwordLineChangeSpeed_0x13ff268, &a6ad::getSwordLineChangeSpeed},
        {0x13FF278u, &::getSwordLlineChangeThreshold_0x13ff278, &a6ad::getSwordLlineChangeThreshold},
        {0x13FF288u, &::getBossHPRatioBase_0x13ff288, &a6ad::getBossHPRatioBase},
        {0x13FF348u, &::getSwordGlowDistMin_0x13ff348, &a6ad::getSwordGlowDistMin},
        {0x13FF358u, &::getSwordGlowDistMax_0x13ff358, &a6ad::getSwordGlowDistMax},
        {0x13FF368u, &::getCollisionGlowSpeed_0x13ff368, &a6ad::getCollisionGlowSpeed},
        {0x13FF378u, &::getCollisionGlowTime_0x13ff378, &a6ad::getCollisionGlowTime},
        {0x13FF388u, &::getCollisionPosOffset_0x13ff388, &a6ad::getCollisionPosOffset},
        {0x13FF398u, &::gamecore_sub_013FF398_0x13ff398, &a6ad::gamecore_sub_013FF398},
        {0x13FF3A0u, &::gamecore_sub_013FF3A0_0x13ff3a0, &a6ad::gamecore_sub_013FF3A0},
        {0x13FF560u, &::timeAttackTimeScriptEnd_0x13ff560, &a6ad::timeAttackTimeScriptEnd},
        {0x1402078u, &::GetTimeAttackNextPhase_0x1402078, &a6ad::GetTimeAttackNextPhase},
        {0x14020C8u, &::GetTimeAttackTimer_0x14020c8, &a6ad::GetTimeAttackTimer},
        {0x14020D8u, &::SetTimeAttackTimeFlag_0x14020d8, &a6ad::SetTimeAttackTimeFlag},
        {0x14020E8u, &::GetTimeAttackTimeLimit_0x14020e8, &a6ad::GetTimeAttackTimeLimit},
        {0x1402178u, &::GetReplayNextPhase_0x1402178, &a6ad::GetReplayNextPhase},
        {0x1402420u, &::CloseTimeAttackMenu_0x1402420, &a6ad::CloseTimeAttackMenu},
        {0x1407320u, &::getWeaponName_0x1407320, &a6ad::getWeaponName},
        {0x140AE20u, &::deleteSavePoint_0x140ae20, &a6ad::deleteSavePoint},
        {0x140AE28u, &::SetMapLogDispMode_0x140ae28, &a6ad::SetMapLogDispMode},
        {0x140AE68u, &::getLogLineColorR_0x140ae68, &a6ad::getLogLineColorR},
        {0x140AE78u, &::getLogLineColorG_0x140ae78, &a6ad::getLogLineColorG},
        {0x140AE88u, &::getLogLineColorB_0x140ae88, &a6ad::getLogLineColorB},
        {0x140AE98u, &::getLogLineColorA_0x140ae98, &a6ad::getLogLineColorA},
        {0x140DAD8u, &::gamecore_sub_0140DAD8_0x140dad8, &a6ad::gamecore_sub_0140DAD8},
        {0x1411460u, &::begin_time_interp_camera_dist_to_look_at_0x1411460, &a6ad::begin_time_interp_camera_dist_to_look_at},
        {0x1411470u, &::set_stretch_phase_ratio_camera_dist_to_look_at_0x1411470, &a6ad::set_stretch_phase_ratio_camera_dist_to_look_at},
        {0x1411478u, &::get_stretch_phase_ratio_camera_dist_to_look_at_0x1411478, &a6ad::get_stretch_phase_ratio_camera_dist_to_look_at},
        {0x1412A90u, &::get_end_interp_finish_time_camera_pause_rise_0x1412a90, &a6ad::get_end_interp_finish_time_camera_pause_rise},
        {0x14134A0u, &::get_cam_region_col_attr_camera_place_status_0x14134a0, &a6ad::get_cam_region_col_attr_camera_place_status},
        {0x1414E68u, &::is_exist_camera_target_lost_angle_0x1414e68, &a6ad::is_exist_camera_target_lost_angle},
        {0x14158B8u, &::is_exist_camera_target_visible_angle_0x14158b8, &a6ad::is_exist_camera_target_visible_angle},
        {0x1419898u, &::CAM_is_reach_aim_yaw_0x1419898, &a6ad::CAM_is_reach_aim_yaw},
        {0x1419938u, &::CAM_is_reach_aim_pitch_0x1419938, &a6ad::CAM_is_reach_aim_pitch},
        {0x141A170u, &::CAM_set_clip_max_rot_vel_aim_rot_common_0x141a170, &a6ad::CAM_set_clip_max_rot_vel_aim_rot_common},
        {0x141A188u, &::CAM_set_yaw_shortest_rot_flag_aim_rot_common_0x141a188, &a6ad::CAM_set_yaw_shortest_rot_flag_aim_rot_common},
        {0x141A1F0u, &::CAM_is_inside_of_stop_region_aim_rot_common_0x141a1f0, &a6ad::CAM_is_inside_of_stop_region_aim_rot_common},
        {0x141A620u, &::CAM_is_motion_kind_change_target_0x141a620, &a6ad::CAM_is_motion_kind_change_target},
        {0x141A668u, &::CAM_motion_kind_act_auto_yaw_rot_target_0x141a668, &a6ad::CAM_motion_kind_act_auto_yaw_rot_target},
        {0x141A688u, &::CAM_motion_kind_is_accept_follow_rot_motion_target_0x141a688, &a6ad::CAM_motion_kind_is_accept_follow_rot_motion_target},
        {0x141A6E0u, &::CAM_motion_kind_is_mode_lock_target_0x141a6e0, &a6ad::CAM_motion_kind_is_mode_lock_target},
        {0x141B0F8u, &::CAM_is_use_obstacle_target_ofst_interp_coe_0x141b0f8, &a6ad::CAM_is_use_obstacle_target_ofst_interp_coe},
        {0x141CD70u, &::set_camera_mode_0x141cd70, &a6ad::set_camera_mode},
        {0x141CD78u, &::get_cur_camera_mode_0x141cd78, &a6ad::get_cur_camera_mode},
        {0x141CD80u, &::get_camera_mode_name_0x141cd80, &a6ad::get_camera_mode_name},
        {0x141CE30u, &::get_camera_target_group_0x141ce30, &a6ad::get_camera_target_group},
        {0x141CE40u, &::get_camera_visible_collision_group_0x141ce40, &a6ad::get_camera_visible_collision_group},
        {0x141CE50u, &::get_lws_animation_collision_group_cameraman_0x141ce50, &a6ad::get_lws_animation_collision_group_cameraman},
        {0x141CE68u, &::get_game_camera_WV_matrix_0x141ce68, &a6ad::get_game_camera_WV_matrix},
        {0x141CE70u, &::is_game_camera_mode_now_0x141ce70, &a6ad::is_game_camera_mode_now},
        {0x141CEA0u, &::change_camera_target_0x141cea0, &a6ad::change_camera_target},
        {0x141D4B0u, &::get_main_cameraman_id_0x141d4b0, &a6ad::get_main_cameraman_id},
        {0x141D4C0u, &::is_main_cameraman_exist_0x141d4c0, &a6ad::is_main_cameraman_exist},
        {0x141D4D8u, &::get_total_cameraman_0x141d4d8, &a6ad::get_total_cameraman},
        {0x141D4E8u, &::get_max_cameraman_0x141d4e8, &a6ad::get_max_cameraman},
        {0x141E630u, &::get_debug_camera_view_angle_0x141e630, &a6ad::get_debug_camera_view_angle},
        {0x141E640u, &::get_debug_camera_view_angle_pointer_0x141e640, &a6ad::get_debug_camera_view_angle_pointer},
        {0x141E650u, &::is_disp_game_camera_model_ok_0x141e650, &a6ad::is_disp_game_camera_model_ok},
        {0x141E660u, &::is_disp_game_camera_model_ok_pointer_0x141e660, &a6ad::is_disp_game_camera_model_ok_pointer},
        {0x141E670u, &::get_debug_camera_dist_to_look_at_pointer_0x141e670, &a6ad::get_debug_camera_dist_to_look_at_pointer},
        {0x141E680u, &::set_debug_camera_move_diff_0x141e680, &a6ad::set_debug_camera_move_diff},
        {0x1424BE8u, &::constructor_mode_common_camera_data_0x1424be8, &a6ad::constructor_mode_common_camera_data},
        {0x1424F68u, &::get_look_at_position_game_camera_0x1424f68, &a6ad::get_look_at_position_game_camera},
        {0x1424F78u, &::jack_game_camera_0x1424f78, &a6ad::jack_game_camera},
        {0x1425020u, &::is_game_camera_standby_ok_0x1425020, &a6ad::is_game_camera_standby_ok},
        {0x14250F0u, &::get_look_lws_anime_camera_flag_0x14250f0, &a6ad::get_look_lws_anime_camera_flag},
        {0x1425298u, &::begin_check_and_boot_boss_subjective_camera_lws_animation_0x1425298, &a6ad::begin_check_and_boot_boss_subjective_camera_lws_animation},
        {0x14253B0u, &::get_object_interest_position_game_camera_0x14253b0, &a6ad::get_object_interest_position_game_camera},
        {0x142C108u, &::get_cur_base_mode_condition_game_camera_param_manager_0x142c108, &a6ad::get_cur_base_mode_condition_game_camera_param_manager},
        {0x142C110u, &::get_cur_base_mode_manager_game_camera_param_manager_0x142c110, &a6ad::get_cur_base_mode_manager_game_camera_param_manager},
        {0x142C8B0u, &::gamecore_sub_0142C8B0_0x142c8b0, &a6ad::gamecore_sub_0142C8B0},
        {0x1431FD0u, &::setPassageCallBack_0x1431fd0, &a6ad::setPassageCallBack},
        {0x14327D0u, &::useTempAttain_0x14327d0, &a6ad::useTempAttain},
        {0x14327F8u, &::getWayForRoute_0x14327f8, &a6ad::getWayForRoute},
        {0x1432800u, &::getLastAttainUpdate_0x1432800, &a6ad::getLastAttainUpdate},
        {0x14338A0u, &::forceAddPassageWork_0x14338a0, &a6ad::forceAddPassageWork},
        {0x14359E0u, &::getUserPathExMode_0x14359e0, &a6ad::getUserPathExMode},
        {0x1435A30u, &::getPPointPasasgeNum_0x1435a30, &a6ad::getPPointPasasgeNum},
        {0x1435B28u, &::changedDPGroupID_0x1435b28, &a6ad::changedDPGroupID},
        {0x1435B50u, &::setGpCalcDist_0x1435b50, &a6ad::setGpCalcDist},
        {0x1437B80u, &::_getExecBaseMoveWork_0x1437b80, &a6ad::_getExecBaseMoveWork},
        {0x143D550u, &::deleteBirdCtrlWork_0x143d550, &a6ad::deleteBirdCtrlWork},
        {0x143D558u, &::getLandMotionOffset_0x143d558, &a6ad::getLandMotionOffset},
        {0x143D5B8u, &::setRouteAttackBirdCtrlWork_0x143d5b8, &a6ad::setRouteAttackBirdCtrlWork},
        {0x143D690u, &::gamecore_sub_0143D690_0x143d690, &a6ad::gamecore_sub_0143D690},
        {0x1440EE8u, &::deleteFastLegCtrlWork_0x1440ee8, &a6ad::deleteFastLegCtrlWork},
        {0x1440F40u, &::getChkColWidth_0x1440f40, &a6ad::getChkColWidth},
        {0x1441A68u, &::chkGazeTool_0x1441a68, &a6ad::chkGazeTool},
        {0x1441A78u, &::getGazeToolPos_0x1441a78, &a6ad::getGazeToolPos},
        {0x14444C8u, &::deleteHorseCtrlWork_0x14444c8, &a6ad::deleteHorseCtrlWork},
        {0x14454A0u, &::deleteHumanCtrlWork_0x14454a0, &a6ad::deleteHumanCtrlWork},
        {0x14454A8u, &::setDashHumanCtrlWork_0x14454a8, &a6ad::setDashHumanCtrlWork},
        {0x14454E0u, &::gamecore_sub_014454E0_0x14454e0, &a6ad::gamecore_sub_014454E0},
        {0x1446120u, &::chkMoveAngle2_0x1446120, &a6ad::chkMoveAngle2},
        {0x14466D8u, &::chkLeftPointForLineXZ_0x14466d8, &a6ad::chkLeftPointForLineXZ},
        {0x14467A0u, &::chkUpPointForLineYZ_0x14467a0, &a6ad::chkUpPointForLineYZ},
        {0x1449830u, &::setAttainMotScaleSlowLegCtrlWork_0x1449830, &a6ad::setAttainMotScaleSlowLegCtrlWork},
        {0x1449840u, &::deleteSlowLegCtrlWork_0x1449840, &a6ad::deleteSlowLegCtrlWork},
        {0x144CC58u, &::deleteWormCtrlWork_0x144cc58, &a6ad::deleteWormCtrlWork},
        {0x144CD48u, &::setMiddleTargetWormCtrlWork_0x144cd48, &a6ad::setMiddleTargetWormCtrlWork},
        {0x144CDD8u, &::setAcceleParamWormCtrlWork_0x144cdd8, &a6ad::setAcceleParamWormCtrlWork},
        {0x144CDF0u, &::getBuryingWormCtrlWork_0x144cdf0, &a6ad::getBuryingWormCtrlWork},
        {0x1456378u, &::setConditionResultBoss_0x1456378, &a6ad::setConditionResultBoss},
        {0x1456388u, &::setConditionResultHorse_0x1456388, &a6ad::setConditionResultHorse},
        {0x1456398u, &::SCRConditionResultBoss_0x1456398, &a6ad::SCRConditionResultBoss},
        {0x14563A8u, &::SCRConditionResultHorse_0x14563a8, &a6ad::SCRConditionResultHorse},
        {0x145AE80u, &::pauseEventDemoManger_0x145ae80, &a6ad::pauseEventDemoManger},
        {0x145CA68u, &::forceSetPerceptionWork_0x145ca68, &a6ad::forceSetPerceptionWork},
        {0x145CAB0u, &::forceRenewLastPosPerceptionWork_0x145cab0, &a6ad::forceRenewLastPosPerceptionWork},
        {0x145F118u, &::nico_BeardTest_0x145f118, &a6ad::nico_BeardTest},
        {0x14617D0u, &::setGazeDirGazeIKWork_0x14617d0, &a6ad::setGazeDirGazeIKWork},
        {0x1464828u, &::setWormIKAttainXVector_0x1464828, &a6ad::setWormIKAttainXVector},
        {0x1464CB0u, &::SCRFuncSoundSetDepth_0x1464cb0, &a6ad::SCRFuncSoundSetDepth},
        {0x101B20u, &::_checkExistString_0x101b20, &a6ad::_checkExistString},
        {0x10AAC8u, &::sub_0010AAC8_0x10aac8, &a6ad::sub_0010AAC8},
        {0x10E338u, &::sub_0010E338_0x10e338, &a6ad::sub_0010E338},
        {0x1120F8u, &::unlink_0x1120f8, &a6ad::unlink},
        {0x115888u, &::scePad2StateIntToStr_0x115888, &a6ad::scePad2StateIntToStr},
        {0x1184E8u, &::sub_001184E8_0x1184e8, &a6ad::sub_001184E8},
        {0x118578u, &::sub_00118578_0x118578, &a6ad::sub_00118578},
        {0x1270A8u, &::ps2___make_fp_0x1270a8, &a6ad::ps2___make_fp},
        {0x12E490u, &::_setlocale_r_0x12e490, &a6ad::_setlocale_r},
        {0x12E558u, &::localeconv_0x12e558, &a6ad::localeconv},
        {0x12F3A0u, &::_b2d_0x12f3a0, &a6ad::_b2d},
        {0x1B2A00u, &::iosSrpWaitDMA_0x1b2a00, &a6ad::iosSrpWaitDMA},
        {0x1B3140u, &::iosChangeThreadPriority_0x1b3140, &a6ad::iosChangeThreadPriority},
        {0x1B3188u, &::iosGetThreadPriority_0x1b3188, &a6ad::iosGetThreadPriority},
        {0x1B9B38u, &::iosDumpMatrix_0x1b9b38, &a6ad::iosDumpMatrix},
        {0x1BA3E0u, &::iosFvecToICol_0x1ba3e0, &a6ad::iosFvecToICol},
        {0x1BBB48u, &::PushQuaternion_0x1bbb48, &a6ad::PushQuaternion},
        {0x1BBBB8u, &::PushQuaternionWithNoCopy_0x1bbbb8, &a6ad::PushQuaternionWithNoCopy},
        {0x1BBEF8u, &::MultiMatrixByQuaternion_0x1bbef8, &a6ad::MultiMatrixByQuaternion},
        {0x1C8D88u, &::iosConvertStringSJIStoEUC_0x1c8d88, &a6ad::iosConvertStringSJIStoEUC},
        {0x1C8EC8u, &::iosConvertStringEUCtoSJIS_0x1c8ec8, &a6ad::iosConvertStringEUCtoSJIS},
        {0x1C8F98u, &::iosConvertStringEUCtoID_0x1c8f98, &a6ad::iosConvertStringEUCtoID},
        {0x1C9320u, &::iosGetFontDataWithEUC_0x1c9320, &a6ad::iosGetFontDataWithEUC},
        {0x1C9550u, &::iosGetFontIDWithEUC_0x1c9550, &a6ad::iosGetFontIDWithEUC},
        {0x1C9738u, &::_isysRecvSubMsg_0x1c9738, &a6ad::_isysRecvSubMsg},
        {0x1CB418u, &::getSlotInfo_0x1cb418, &a6ad::getSlotInfo},
        {0x1CB630u, &::iosGetIopTotalMemSize_0x1cb630, &a6ad::iosGetIopTotalMemSize},
        {0x1CC998u, &::Sg2ExStAdpcmSlotFree_0x1cc998, &a6ad::Sg2ExStAdpcmSlotFree},
        {0x1CF7B0u, &::Sg2ModSetOutputMode_0x1cf7b0, &a6ad::Sg2ModSetOutputMode},
        {0x1D2BC8u, &::Sg2SepCheckSndOn_0x1d2bc8, &a6ad::Sg2SepCheckSndOn},
        {0x1D8920u, &::Sg2SmfpCheckSndOn_0x1d8920, &a6ad::Sg2SmfpCheckSndOn},
        {0x1D9E50u, &::Sg2StAdpcmIopReadAdr_0x1d9e50, &a6ad::Sg2StAdpcmIopReadAdr},
        {0x1DA4A8u, &::Sg2VabGetBdSize_0x1da4a8, &a6ad::Sg2VabGetBdSize},
        {0x1DA6F0u, &::kernel_sub_001DA6F0_0x1da6f0, &a6ad::kernel_sub_001DA6F0},
        {0x1DC1F8u, &::Sg2ExPcmCheckAutoFilterSeq_0x1dc1f8, &a6ad::Sg2ExPcmCheckAutoFilterSeq},
        {0x1DCD48u, &::Sg2StPcmIopTransOffset_0x1dcd48, &a6ad::Sg2StPcmIopTransOffset},
        {0x107C7C8u, &::DEBUG_get_2d_font_disp_position_0x107c7c8, &a6ad::DEBUG_get_2d_font_disp_position},
        {0x107C888u, &::DEBUG_get_2d_font_default_height_offset_0x107c888, &a6ad::DEBUG_get_2d_font_default_height_offset},
        {0x107C8C0u, &::DEBUG_get_cntrl_mode_name_0x107c8c0, &a6ad::DEBUG_get_cntrl_mode_name},
        {0x107C8F8u, &::DEBUG_get_latest_motion_name_0x107c8f8, &a6ad::DEBUG_get_latest_motion_name},
        {0x107C938u, &::DEBUG_get_base_name_0x107c938, &a6ad::DEBUG_get_base_name},
        {0x107D4C8u, &::GAMEUTIL_is_between_yaw_and_yaw_extend_rel_0x107d4c8, &a6ad::GAMEUTIL_is_between_yaw_and_yaw_extend_rel},
        {0x107FAE0u, &::GAMEUTIL_get_new_velocity_by_brake_0x107fae0, &a6ad::GAMEUTIL_get_new_velocity_by_brake},
        {0x1080610u, &::GAMEUTIL_get_limit_clip_yaw_abs_0x1080610, &a6ad::GAMEUTIL_get_limit_clip_yaw_abs},
        {0x10809B0u, &::GAMEUTIL_get_difference_yaw_and_yaw_extend_0x10809b0, &a6ad::GAMEUTIL_get_difference_yaw_and_yaw_extend},
        {0x1082A90u, &::GAMEUTIL_is_inside_0x1082a90, &a6ad::GAMEUTIL_is_inside},
        {0x108E560u, &::glNormal3f_0x108e560, &a6ad::glNormal3f},
        {0x108E5F0u, &::glNormal3fv_0x108e5f0, &a6ad::glNormal3fv},
        {0x108E6A0u, &::glNormal4f_0x108e6a0, &a6ad::glNormal4f},
        {0x108E730u, &::glNormal4fv_0x108e730, &a6ad::glNormal4fv},
        {0x108ED50u, &::glLoadIdentity_0x108ed50, &a6ad::glLoadIdentity},
        {0x108EDA8u, &::glGetMatrixf_0x108eda8, &a6ad::glGetMatrixf},
        {0x108EDD8u, &::glPushMatrix_0x108edd8, &a6ad::glPushMatrix},
        {0x108F3F0u, &::glGetCurrentMatrix_0x108f3f0, &a6ad::glGetCurrentMatrix},
        {0x1095998u, &::debugLineObjectCreateAttributre_0x1095998, &a6ad::debugLineObjectCreateAttributre},
        {0x10993E0u, &::modelResetLayer_0x10993e0, &a6ad::modelResetLayer},
        {0x10A43C8u, &::setVertexPos_0x10a43c8, &a6ad::setVertexPos},
        {0x10AA8A0u, &::texScaleClutAlphaByName_0x10aa8a0, &a6ad::texScaleClutAlphaByName},
        {0x10AB528u, &::texScaleClutByName_0x10ab528, &a6ad::texScaleClutByName},
        {0x10B9590u, &::get_animation_designer_node_quater_motion_ik_base_0x10b9590, &a6ad::get_animation_designer_node_quater_motion_ik_base},
        {0x10BA1F0u, &::manager_sub_010BA1F0_0x10ba1f0, &a6ad::manager_sub_010BA1F0},
        {0x10BC1D0u, &::SetCharacterClassMotionAddSyncMode_0x10bc1d0, &a6ad::SetCharacterClassMotionAddSyncMode},
        {0x10C23D8u, &::GetCharacterClassMotionInterpNodePosition_0x10c23d8, &a6ad::GetCharacterClassMotionInterpNodePosition},
        {0x10C27F8u, &::GetCharacterObjMotionShiftCallBackMaxEntry_0x10c27f8, &a6ad::GetCharacterObjMotionShiftCallBackMaxEntry},
        {0x10C2828u, &::ResetCharacterObjMotionShiftCallBackEntryAll_0x10c2828, &a6ad::ResetCharacterObjMotionShiftCallBackEntryAll},
        {0x10C2E40u, &::InitSecureParentInfo_0x10c2e40, &a6ad::InitSecureParentInfo},
        {0x10E3F60u, &::GetPleneFittingPointWithNormal_0x10e3f60, &a6ad::GetPleneFittingPointWithNormal},
        {0x10F2490u, &::GetLayoutClassObjectName_0x10f2490, &a6ad::GetLayoutClassObjectName},
        {0x10F34A0u, &::GetAliasLayoutObjWorkSize_0x10f34a0, &a6ad::GetAliasLayoutObjWorkSize},
        {0x10F57F0u, &::SetVIF1MarkCallbackFunc_0x10f57f0, &a6ad::SetVIF1MarkCallbackFunc},
        {0x10F8CC8u, &::SetColorParamAmbientLight_0x10f8cc8, &a6ad::SetColorParamAmbientLight},
        {0x110C878u, &::UpdateOutRangeLightGeo_0x110c878, &a6ad::UpdateOutRangeLightGeo},
        {0x11145F0u, &::DeleteSubLinkObjLink_0x11145f0, &a6ad::DeleteSubLinkObjLink},
        {0x1128400u, &::GetGeometryPointerCollisionSurfSTPosition_0x1128400, &a6ad::GetGeometryPointerCollisionSurfSTPosition},
        {0x112A5D8u, &::RequestCollisionWideAreaSearch_0x112a5d8, &a6ad::RequestCollisionWideAreaSearch},
        {0x1141090u, &::manager_sub_01141090_0x1141090, &a6ad::manager_sub_01141090},
        {0x114F998u, &::manager_sub_0114F998_0x114f998, &a6ad::manager_sub_0114F998},
        {0x1153610u, &::manager_sub_01153610_0x1153610, &a6ad::manager_sub_01153610},
        {0x1155FB8u, &::windowSetCursorPos_0x1155fb8, &a6ad::windowSetCursorPos},
        {0x1156050u, &::windowSetCursorPosOnWindow_0x1156050, &a6ad::windowSetCursorPosOnWindow},
        {0x1167038u, &::ps2__SetMultiCastLetterReceiverInfo_0x1167038, &a6ad::ps2__SetMultiCastLetterReceiverInfo},
        {0x1167080u, &::ps2__SetLetterKindVariable_0x1167080, &a6ad::ps2__SetLetterKindVariable},
        {0x11675A0u, &::GetAnimationFogDefByName_0x11675a0, &a6ad::GetAnimationFogDefByName},
        {0x1342A48u, &::convert_from_slide_bar_value_to_ratio_0x1342a48, &a6ad::convert_from_slide_bar_value_to_ratio},
        {0x13444B0u, &::GAMEUTIL_add_data_record_info_0x13444b0, &a6ad::GAMEUTIL_add_data_record_info},
        {0x13452C8u, &::nico_deleteMyself_0x13452c8, &a6ad::nico_deleteMyself},
        {0x13462E0u, &::getDemoBar_0x13462e0, &a6ad::getDemoBar},
        {0x13475A0u, &::update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl_0x13475a0, &a6ad::update_offset_yaw_pitch_another_factor_nico_yaw_pitch_cntrl},
        {0x134A2B0u, &::bossStatLayoutChk_0x134a2b0, &a6ad::bossStatLayoutChk},
        {0x134A368u, &::bossStatChkDeadBoss_0x134a368, &a6ad::bossStatChkDeadBoss},
        {0x134C2C8u, &::nico_GetCharacterParentGPAttr_0x134c2c8, &a6ad::nico_GetCharacterParentGPAttr},
        {0x134C350u, &::nico_CheckCharacterVColPlaneAttribute_0x134c350, &a6ad::nico_CheckCharacterVColPlaneAttribute},
        {0x134C3A8u, &::nico_CheckCharacterGPColPlaneAttribute_0x134c3a8, &a6ad::nico_CheckCharacterGPColPlaneAttribute},
        {0x134DF50u, &::scrSetFadeColorBlack_0x134df50, &a6ad::scrSetFadeColorBlack},
        {0x134DF68u, &::scrSetFadeColorWhite_0x134df68, &a6ad::scrSetFadeColorWhite},
        {0x1352440u, &::gameflowSetCaptionPause_0x1352440, &a6ad::gameflowSetCaptionPause},
        {0x1352500u, &::gamecore_sub_01352500_0x1352500, &a6ad::gamecore_sub_01352500},
        {0x1352550u, &::gamecore_sub_01352550_0x1352550, &a6ad::gamecore_sub_01352550},
        {0x13525E0u, &::gamecore_sub_013525E0_0x13525e0, &a6ad::gamecore_sub_013525E0},
        {0x13527A0u, &::gamecore_sub_013527A0_0x13527a0, &a6ad::gamecore_sub_013527A0},
        {0x1353638u, &::gStatGobjDispGflagSet_0x1353638, &a6ad::gStatGobjDispGflagSet},
        {0x1353680u, &::gStatGobjDispGflagCls_0x1353680, &a6ad::gStatGobjDispGflagCls},
        {0x13536E0u, &::gStatStartTimeAttack_0x13536e0, &a6ad::gStatStartTimeAttack},
        {0x1353708u, &::gStatStopTimeAttack_0x1353708, &a6ad::gStatStopTimeAttack},
        {0x1353E70u, &::gstatBootMcCurSetLastSaveOmakeStat_0x1353e70, &a6ad::gstatBootMcCurSetLastSaveOmakeStat},
        {0x13587B8u, &::getNowPower_0x13587b8, &a6ad::getNowPower},
        {0x13668D8u, &::stagemanager_checkstageAlreadyLoaded_0x13668d8, &a6ad::stagemanager_checkstageAlreadyLoaded},
        {0x136A758u, &::bgmAutoStart_0x136a758, &a6ad::bgmAutoStart},
        {0x136EF58u, &::SCRBirdStartBreath_0x136ef58, &a6ad::SCRBirdStartBreath},
        {0x136EF78u, &::SCRBirdStopBreath_0x136ef78, &a6ad::SCRBirdStopBreath},
        {0x136EF98u, &::SCRBirdStartFlap_0x136ef98, &a6ad::SCRBirdStartFlap},
        {0x136FF80u, &::SCRBossDebugDeath_0x136ff80, &a6ad::SCRBossDebugDeath},
        {0x136FFA0u, &::SCRMammothBreaksWall_0x136ffa0, &a6ad::SCRMammothBreaksWall},
        {0x1379578u, &::SCRBuddhaShoot_0x1379578, &a6ad::SCRBuddhaShoot},
        {0x137B558u, &::SCRCerberusPollFalldown_0x137b558, &a6ad::SCRCerberusPollFalldown},
        {0x137E5E0u, &::SCRResetCharacterObj4DDynamicsInfo_0x137e5e0, &a6ad::SCRResetCharacterObj4DDynamicsInfo},
        {0x1389A48u, &::demo_PlayerDeathEffectStart_0x1389a48, &a6ad::demo_PlayerDeathEffectStart},
        {0x1389A70u, &::demo_PlayerDeathEffectEnd_0x1389a70, &a6ad::demo_PlayerDeathEffectEnd},
        {0x138BE80u, &::get_cur_type_expression_0x138be80, &a6ad::get_cur_type_expression},
        {0x138C188u, &::SCRRockFall_0x138c188, &a6ad::SCRRockFall},
        {0x138C1A8u, &::SCRRockReset_0x138c1a8, &a6ad::SCRRockReset},
        {0x138F8D0u, &::SCRGuardSwordPickup_0x138f8d0, &a6ad::SCRGuardSwordPickup},
        {0x138F8F0u, &::SCRGuardSwordPickupEnd_0x138f8f0, &a6ad::SCRGuardSwordPickupEnd},
        {0x1390550u, &::SCRGuardArrowShoot_0x1390550, &a6ad::SCRGuardArrowShoot},
        {0x1396E00u, &::SCRHorseUnLoad_0x1396e00, &a6ad::SCRHorseUnLoad},
        {0x13970A0u, &::nico_horseClothOverSurfDetectFunc_0x13970a0, &a6ad::nico_horseClothOverSurfDetectFunc},
        {0x139AC18u, &::MotSpdCtrl_SetSpeed_0x139ac18, &a6ad::MotSpdCtrl_SetSpeed},
        {0x139CBF0u, &::SCRKirinFallRockEnd_0x139cbf0, &a6ad::SCRKirinFallRockEnd},
        {0x139D6B0u, &::SCRKnightBossPartsBroken_0x139d6b0, &a6ad::SCRKnightBossPartsBroken},
        {0x139D6D0u, &::SCRKnightBossPartsDelete_0x139d6d0, &a6ad::SCRKnightBossPartsDelete},
        {0x139E058u, &::SCRLeoTorch0_Disp_0x139e058, &a6ad::SCRLeoTorch0_Disp},
        {0x139E078u, &::SCRLeoTorch1_Disp_0x139e078, &a6ad::SCRLeoTorch1_Disp},
        {0x139E098u, &::SCRLeoTorch2_Disp_0x139e098, &a6ad::SCRLeoTorch2_Disp},
        {0x139E0B8u, &::SCRLeoTorch3_Disp_0x139e0b8, &a6ad::SCRLeoTorch3_Disp},
        {0x139E100u, &::SCRLeoArmorBroken_0x139e100, &a6ad::SCRLeoArmorBroken},
        {0x139E120u, &::SCRLeoFall_0x139e120, &a6ad::SCRLeoFall},
        {0x139E140u, &::SCRLeoDamage_0x139e140, &a6ad::SCRLeoDamage},
        {0x139E160u, &::SCRLeoDashAttackEnd_0x139e160, &a6ad::SCRLeoDashAttackEnd},
        {0x139E180u, &::SCRLeoDashAttack_0x139e180, &a6ad::SCRLeoDashAttack},
        {0x13A4830u, &::SCRCanossaWallBreakEnd_0x13a4830, &a6ad::SCRCanossaWallBreakEnd},
        {0x13A4850u, &::SCRCanossaWallDelete_0x13a4850, &a6ad::SCRCanossaWallDelete},
        {0x13A5A48u, &::SCRMinotourCFallWeapon_0x13a5a48, &a6ad::SCRMinotourCFallWeapon},
        {0x13A5A68u, &::SCRMinotourCSetFallWeaponFlag_0x13a5a68, &a6ad::SCRMinotourCSetFallWeaponFlag},
        {0x13A5B80u, &::SCRMinoCSetAddFlag_0x13a5b80, &a6ad::SCRMinoCSetAddFlag},
        {0x13A5BA0u, &::SCRMinoCResetAddFlag_0x13a5ba0, &a6ad::SCRMinoCResetAddFlag},
        {0x13A5C68u, &::SCRMinotourCResetWeaponCameraFlag_0x13a5c68, &a6ad::SCRMinotourCResetWeaponCameraFlag},
        {0x13A5CC8u, &::gamecore_sub_013A5CC8_0x13a5cc8, &a6ad::gamecore_sub_013A5CC8},
        {0x13B61C0u, &::SCRFoodTakeBegin_0x13b61c0, &a6ad::SCRFoodTakeBegin},
        {0x13B61E0u, &::SCRFoodTakeEnd_0x13b61e0, &a6ad::SCRFoodTakeEnd},
        {0x13B6200u, &::SCRItemTakeBegin_0x13b6200, &a6ad::SCRItemTakeBegin},
        {0x13B6220u, &::SCRItemTakeEnd_0x13b6220, &a6ad::SCRItemTakeEnd},
        {0x13B6240u, &::SCRItemTakeDelete_0x13b6240, &a6ad::SCRItemTakeDelete},
        {0x13B62B0u, &::SCRArrowContinue_0x13b62b0, &a6ad::SCRArrowContinue},
        {0x13B6310u, &::SCRHarpoonStart_0x13b6310, &a6ad::SCRHarpoonStart},
        {0x13B6330u, &::SCRHarpoonContinue_0x13b6330, &a6ad::SCRHarpoonContinue},
        {0x13B6350u, &::SCRHarpoonLoopStart_0x13b6350, &a6ad::SCRHarpoonLoopStart},
        {0x13B6370u, &::SCRHarpoonLoopEnd_0x13b6370, &a6ad::SCRHarpoonLoopEnd},
        {0x13B6390u, &::SCRPlayerSendDamageStart_0x13b6390, &a6ad::SCRPlayerSendDamageStart},
        {0x13B63B0u, &::SCRPlayerSendDamageEnd_0x13b63b0, &a6ad::SCRPlayerSendDamageEnd},
        {0x13B63D0u, &::SCRPlayerSendDamage_0x13b63d0, &a6ad::SCRPlayerSendDamage},
        {0x13B6410u, &::SCRPlayerSendDeathDamageLater_0x13b6410, &a6ad::SCRPlayerSendDeathDamageLater},
        {0x13B6450u, &::SCRPlayerSowrdDeathHitSound_0x13b6450, &a6ad::SCRPlayerSowrdDeathHitSound},
        {0x13B6490u, &::SCRPlayerSowrdHitSoundMiddle_0x13b6490, &a6ad::SCRPlayerSowrdHitSoundMiddle},
        {0x13B67F8u, &::SCRPlayerPauseDisable_0x13b67f8, &a6ad::SCRPlayerPauseDisable},
        {0x13B6818u, &::SCRPlayerPauseEnable_0x13b6818, &a6ad::SCRPlayerPauseEnable},
        {0x13B6898u, &::SCRHorseKick_0x13b6898, &a6ad::SCRHorseKick},
        {0x13B6A40u, &::nico_playeGripDamage_0x13b6a40, &a6ad::nico_playeGripDamage},
        {0x13CD688u, &::SCRGetPlayerTiredParam_0x13cd688, &a6ad::SCRGetPlayerTiredParam},
        {0x13D59C8u, &::AI_GetYRotQuaternionFromVector_0x13d59c8, &a6ad::AI_GetYRotQuaternionFromVector},
        {0x13F3EC8u, &::gamecore_sub_013F3EC8_0x13f3ec8, &a6ad::gamecore_sub_013F3EC8},
        {0x1404950u, &::SCRTortoisePulseChargeStop_0x1404950, &a6ad::SCRTortoisePulseChargeStop},
        {0x140C820u, &::SCRYamori2BeginStand_0x140c820, &a6ad::SCRYamori2BeginStand},
        {0x140C840u, &::SCRYamoriBReadyToBreeze_0x140c840, &a6ad::SCRYamoriBReadyToBreeze},
        {0x140C860u, &::SCRYamoriBReadyToBreeze2_0x140c860, &a6ad::SCRYamoriBReadyToBreeze2},
        {0x1419A98u, &::CAM_near_to_aim_pitch_0x1419a98, &a6ad::CAM_near_to_aim_pitch},
        {0x1419F98u, &::CAM_get_mul_ratio_offset_per_axis_0x1419f98, &a6ad::CAM_get_mul_ratio_offset_per_axis},
        {0x141A628u, &::CAM_get_motion_kind_interp_timer_target_0x141a628, &a6ad::CAM_get_motion_kind_interp_timer_target},
        {0x141CFF8u, &::get_camera_parameter_cameraman_0x141cff8, &a6ad::get_camera_parameter_cameraman},
        {0x141D3C0u, &::get_cameraman_0x141d3c0, &a6ad::get_cameraman},
        {0x141D3F8u, &::get_cameraman_camera_obj_0x141d3f8, &a6ad::get_cameraman_camera_obj},
        {0x1424F88u, &::end_jack_game_camera_0x1424f88, &a6ad::end_jack_game_camera},
        {0x1425058u, &::is_proper_camera_target_game_camera_0x1425058, &a6ad::is_proper_camera_target_game_camera},
        {0x1425100u, &::get_cur_using_camera_mode_condition_param_game_camera_0x1425100, &a6ad::get_cur_using_camera_mode_condition_param_game_camera},
        {0x1425118u, &::get_cur_using_camera_mode_manager_param_game_camera_0x1425118, &a6ad::get_cur_using_camera_mode_manager_param_game_camera},
        {0x14254E8u, &::is_mode_change_game_camera_0x14254e8, &a6ad::is_mode_change_game_camera},
        {0x1431F60u, &::setRayCutPassageWork_0x1431f60, &a6ad::setRayCutPassageWork},
        {0x1433798u, &::addPassageInsertPos_0x1433798, &a6ad::addPassageInsertPos},
        {0x1433848u, &::forceSetWayFinish_0x1433848, &a6ad::forceSetWayFinish},
        {0x14517A8u, &::SCRFuncClsGameFlag_0x14517a8, &a6ad::SCRFuncClsGameFlag},
        {0x1452340u, &::SCRFuncSetAutoBossGazePos_0x1452340, &a6ad::SCRFuncSetAutoBossGazePos},
        {0x1452D70u, &::SCRFuncSegmentLoadWait_0x1452d70, &a6ad::SCRFuncSegmentLoadWait},
        {0x1452F40u, &::SCRFuncSetGameWork_0x1452f40, &a6ad::SCRFuncSetGameWork},
        {0x1458FD8u, &::SCRConditionOverPlayerGrip_0x1458fd8, &a6ad::SCRConditionOverPlayerGrip},
        {0x1459010u, &::SCRConditionLessPlayerGrip_0x1459010, &a6ad::SCRConditionLessPlayerGrip},
        {0x1459AB8u, &::SCRConditionCheckGameClearCnt_0x1459ab8, &a6ad::SCRConditionCheckGameClearCnt},
        {0x1464D28u, &::SCRFuncActiveDemoCancel_0x1464d28, &a6ad::SCRFuncActiveDemoCancel},
        {0x1464E90u, &::gamecore_sub_01464E90_0x1464e90, &a6ad::gamecore_sub_01464E90},
        {0x1464EB8u, &::SCRConditionSoundSignal_0x1464eb8, &a6ad::SCRConditionSoundSignal},
        {0x12E530u, &::setlocale_0x12e530, &a6ad::setlocale},
        {0x1CD660u, &::Sg2ExStAdpcmIopReadAdr_0x1cd660, &a6ad::Sg2ExStAdpcmIopReadAdr},
        {0x10899C0u, &::SetAndLockBgColor_0x10899c0, &a6ad::SetAndLockBgColor},
        {0x10EB568u, &::motrootdyn_startxzfit_common_0x10eb568, &a6ad::motrootdyn_startxzfit_common},
        {0x10F1110u, &::GetLayoutObjChildWithName_0x10f1110, &a6ad::GetLayoutObjChildWithName},
        {0x10F1AB0u, &::SetLayoutUserClipFunc_0x10f1ab0, &a6ad::SetLayoutUserClipFunc},
        {0x10F31C0u, &::ChangeLayoutObjCollisionResourceAttribute_0x10f31c0, &a6ad::ChangeLayoutObjCollisionResourceAttribute},
        {0x10F9C30u, &::GetLwsDemoOrientManagerObjWorkSize_0x10f9c30, &a6ad::GetLwsDemoOrientManagerObjWorkSize},
        {0x1114278u, &::GetScriptSpriteObjWorkSize_0x1114278, &a6ad::GetScriptSpriteObjWorkSize},
        {0x1378758u, &::SCRSetCharaMotionPlaySpeedRatio_0x1378758, &a6ad::SCRSetCharaMotionPlaySpeedRatio},
        {0x13B7990u, &::gamecore_sub_013B7990_0x13b7990, &a6ad::gamecore_sub_013B7990},
        {0x13B79B0u, &::gamecore_sub_013B79B0_0x13b79b0, &a6ad::gamecore_sub_013B79B0},
        {0x14359B0u, &::deleteUserPathWorkEx_0x14359b0, &a6ad::deleteUserPathWorkEx},
        {0x14362E8u, &::gamecore_sub_014362E8_0x14362e8, &a6ad::gamecore_sub_014362E8},
        {0x1437E20u, &::calcMoveTurnAngle2_0x1437e20, &a6ad::calcMoveTurnAngle2},
        {0x14454B0u, &::gamecore_sub_014454B0_0x14454b0, &a6ad::gamecore_sub_014454B0},
        {0x1451658u, &::SCRFuncSetGameFlagByGameObj_0x1451658, &a6ad::SCRFuncSetGameFlagByGameObj},
        {0x1464B58u, &::getEventScriptObjWorkSize_0x1464b58, &a6ad::getEventScriptObjWorkSize},
        {0x10C1E00u, &::ChangeCharacterObjCollisionResourceAttribute_0x10c1e00, &a6ad::ChangeCharacterObjCollisionResourceAttribute},
        {0x1111AC8u, &::SpriteMenuSetDispFlagNoFade_0x1111ac8, &a6ad::SpriteMenuSetDispFlagNoFade},
        {0x1445388u, &::createHumanCtrlWork_0x1445388, &a6ad::createHumanCtrlWork},
        {0x14517C0u, &::SCRFuncSetMyselfMotionSpeedRatio_0x14517c0, &a6ad::SCRFuncSetMyselfMotionSpeedRatio},
    };
}

// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function
uint32_t a6InstallAdapters(PS2Runtime &runtime)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < 970u; ++i)
        if (runtime.lookupFunction(kA6Ad[i].addr) == kA6Ad[i].rec)
        {
            runtime.registerFunction(kA6Ad[i].addr, kA6Ad[i].ad);
            ++n;
        }
    return n;
}
