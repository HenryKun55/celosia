// typed_a13_adapters.cpp: round A13's adapters for the never-lifted functions stage 13 typed (tools/typed_lift2.py;
// docs/decomp_a13.md). Generated: do not edit. Each takes the recompiled function's slot at install (a13InstallAdapters, from
// sotc_decomp::install after A12's, only while the slot still holds the recompiled function): the typed function while the
// switch a13typed is on and every slot it reaches holds what its typed callee stands for (A13_GO / A13_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a13.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a13ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

namespace a13ad
{
// iosFvecToMmColMvec (0x1ba2e0): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosFvecToMmColMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BA2E0u, 1))
    {
        if (PS2_EXPECT(A13_GO() && A13_HOLDS(0x1BE6D0u, lift_d1::iosFTOI15Vector), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const uint64_t a2_hi = ctx->hi; const uint64_t a2_lo = ctx->lo; A13_CHK(iosFvecToMmColMvec, a2_4, a2_5, a2_29, a2_hi, a2_lo); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A13_IN(6); const uint64_t a2_r = typed::iosFvecToMmColMvec(rdram, runtime, a2_4, a2_5, a2_29, a2_hi, a2_lo, a4_vf5, a4_vf6, a4_h2); A13_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a13
    }
    ::iosFvecToMmColMvec_0x1ba2e0(rdram, ctx, runtime);
}
// iosMmColToFvecMvec (0x1ba320): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void iosMmColToFvecMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x1BA320u, 1))
    {
        if (PS2_EXPECT(A13_GO() && A13_HOLDS(0x1BE730u, lift_d1::iosITOF15Vector), 1)) { const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const uint64_t a2_hi = ctx->hi; const uint64_t a2_lo = ctx->lo; A13_CHK(iosMmColToFvecMvec, a2_4, a2_5, a2_29, a2_hi, a2_lo); a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; uint64_t a4_h2 = a4::hi64(ctx->r[2]); A13_IN(7); const uint64_t a2_r = typed::iosMmColToFvecMvec(rdram, runtime, a2_4, a2_5, a2_29, a2_hi, a2_lo, a4_vf5, a4_vf6, a4_h2); A13_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a13
    }
    ::iosMmColToFvecMvec_0x1ba320(rdram, ctx, runtime);
}
// GetMorphAnimation (0x10b7eb8): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void GetMorphAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x10B7EB8u, 1))
    {
        if (PS2_EXPECT(A13_GO() && A13_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A13_HOLDS(0x10B28E0u, ::dc1mot::entryBlendLast) && A13_HOLDS(0x10B2DE8u, lift_d1::GetAnimObjResource) && A5_JAL(0x10B2F48u, 0x1B3410u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_8 = Ps2GprU64(ctx, 8); const uint64_t a2_9 = Ps2GprU64(ctx, 9); const uint32_t a2_10 = Ps2GprU32(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint32_t a2_29 = Ps2GprU32(ctx, 29); const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const uint64_t a2_hi = ctx->hi; const uint64_t a2_lo = ctx->lo; A13_CHK(GetMorphAnimation, a2_2, a2_4, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_hi, a2_lo); uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a3_xo[4] = {(uint64_t)a1_bits(ctx->f[20]), Ps2GprU64(ctx, 7), Ps2GprU64(ctx, 8), Ps2GprU64(ctx, 9)}; A13_IN(10); const a1::RetVF a2_r = typed::GetMorphAnimation(rdram, runtime, a2_2, a2_4, a2_8, a2_9, a2_10, a2_11, a2_29, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_hi, a2_lo, a4_h2, a3_xo); A13_OUT(); Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; ctx->f[20] = a1_flt((uint32_t)a3_xo[0]); Ps2SetGpr64(ctx, 7, a3_xo[1]); Ps2SetGpr64(ctx, 8, a3_xo[2]); Ps2SetGpr64(ctx, 9, a3_xo[3]); ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->pc = Ps2GprU32(ctx, 31); return; } // a13
    }
    ::GetMorphAnimation_0x10b7eb8(rdram, ctx, runtime);
}
// updateLightGeo (0x110b038): the slot of a never-lifted function; the typed function while the adapter's test holds, else the recompiled function
void updateLightGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    if (PS2_EXPECT(ctx->pc == 0x110B038u, 1))
    {
        if (PS2_EXPECT(A13_GO() && A13_HOLDS(0x1B3410u, ::iosJumpRecoverPoint_0x1b3410) && A13_HOLDS(0x1B8620u, lift_d1::iosUnitCurrentMatrix) && A13_HOLDS(0x1B8638u, ::iosPushCurrentMatrix_0x1b8638) && A13_HOLDS(0x1B8650u, ::iosPopCurrentMatrix_0x1b8650) && A13_HOLDS(0x1B8878u, lift_d1::iosTransCurrentMatrix) && A13_HOLDS(0x1B8C90u, lift_d1::iosGetCurrentMatrix) && A13_HOLDS(0x1B8CD0u, lift_d1::iosMulCurrentMatrixR) && A13_HOLDS(0x1BB868u, lift_d1::GetCurrentQuaternion) && A13_HOLDS(0x1BB8A8u, lift_d1::MultiCurrentQuaternion) && A13_HOLDS(0x1BB988u, lift_d1::SetCurrentQuaternion) && A13_HOLDS(0x1BBB48u, ::a6ad::PushQuaternion) && A13_HOLDS(0x1BBC08u, lift_d1::PopQuaternion) && A13_HOLDSX(0x1BBC08u, lift_d1::PopQuaternion) && A13_HOLDS(0x1BBC28u, lift_d1::InitQuaternionDrive) && A13_HOLDS(0x1BBDA8u, ::decomp_quat::axisRotate) && A13_HOLDS(0x1BDDB0u, ::decomp_math::getSinCos) && A13_HOLDS(0x10AC4A0u, ::decomp_hot44d::nodeQuatPos) && A13_HOLDS(0x10AC6C8u, ::decomp_curve::curveEval) && A13_HOLDS(0x10ADE08u, ::decomp_hot46h::keyVec3) && A13_HOLDS(0x10AE128u, ::decomp_hot46h::optionNodeSample) && A13_HOLDSX(0x10B1D60u, ::decomp_skeleton::motionMatrix) && A13_HOLDS(0x10B28E0u, ::dc1mot::entryBlendLast) && A13_HOLDS(0x10B36C8u, ::dc1mot::entryNbOptionNodes) && A13_HOLDS(0x10B3810u, ::dc1mot::entryPhysicalId) && A13_HOLDS(0x10B39B0u, ::dc1mot::entryVersion) && A13_HOLDS(0x10B4218u, lift_d1::GetMotionOptionNodeIntFrame) && A13_HOLDS(0x10B63C0u, lift_d1::GetMotionOptionNodeParentId) && A13_HOLDS(0x10B63F8u, lift_d1::GetMotionNodeQuaternionPosToolCoord) && A13_HOLDS(0x10B6650u, ::a12ad::GetMotionOptionNodeFloatFrame) && A13_HOLDS(0x10B70B8u, lift_d1::getAnimObjResource) && A13_HOLDS(0x10B8590u, ::decomp_skeleton::skeleton) && A13_HOLDSX(0x10B8590u, ::decomp_skeleton::skeleton) && A13_HOLDS(0x10B8798u, lift_d1::manager_sub_010B8798) && A13_HOLDS(0x10BD3F8u, ::decomp_hot47i::optionNodeGeo) && A13_HOLDS(0x10BD3F8u, ::decomp_hot47i::optionNodeGeo) && A13_HOLDS(0x10C0A70u, ::decomp_characlass::getCharacterClass) && A13_HOLDSX(0x10C1580u, ::cam126::ccNodeQP) && A13_HOLDS(0x10C3650u, lift_d1::AddCharacterClassOptionNodeSubGeometry) && A13_HOLDS(0x10C6710u, ::dc1mot::entryOptionNodeId) && A13_HOLDS(0x10C6798u, ::dc1mot::entrySubGeometries) && A13_HOLDSX(0x10F1A20u, lift_d1::GetLayoutClass) && A13_HOLDS(0x10F93F0u, ::a6ad::GetLightClass) && A13_HOLDS(0x110A838u, lift_d1::GetScriptAnimationClass) && A13_HOLDS(0x110D058u, ::decomp_scriptvm3::getScriptLayoutClass) && A13_HOLDS(0x1162E08u, lift_d1::dynamic_cast_to_camera_obj) && A13_HOLDS(0x1162F08u, lift_d1::get_camera_obj_layout_class) && A5_JAL(0x10AC5F0u, 0x1BBDA8u) && A5_JAL(0x10ACF70u, 0x1BDDB0u) && A5_JAL(0x10ACFE4u, 0x1BDDB0u) && A5_JAL(0x10B1DA8u, 0x1BB988u) && A5_JAL(0x10B1DB0u, 0x1B8620u) && A5_JAL(0x10B1DB8u, 0x1B8878u) && A5_JAL(0x10B1DD8u, 0x1BBB48u) && A5_JAL(0x10B1DE0u, 0x1B8638u) && A5_JAL(0x10B1DF0u, 0x1BB8A8u) && A5_JAL(0x10B1EB4u, 0x1B8878u) && A5_JAL(0x10B1EBCu, 0x1B8CD0u) && A5_JAL(0x10B1ECCu, 0x1B8C90u) && A5_JAL(0x10B1ED4u, 0x1BB868u) && A5_JAL(0x10B1F2Cu, 0x1B8650u) && A5_JAL(0x10B1F34u, 0x1BBC08u) && A5_JAL(0x10B1F7Cu, 0x1B8638u) && A5_JAL(0x10B1F88u, 0x1BB8A8u) && A5_JAL(0x10B2058u, 0x1B8878u) && A5_JAL(0x10B2060u, 0x1B8CD0u) && A5_JAL(0x10B2068u, 0x1B8C90u) && A5_JAL(0x10B208Cu, 0x1B8650u) && A5_JAL(0x10B3790u, 0x1B3410u) && A5_JAL(0x10B38E8u, 0x1B3410u) && A5_JAL(0x10B3A78u, 0x1B3410u) && A5_JAL(0x10B4310u, 0x1B3410u) && A5_JAL(0x10B7138u, 0x1B3410u) && A5_JAL(0x10B85F0u, 0x1BBB48u) && A5_JAL(0x10B85F8u, 0x1B8638u) && A5_JAL(0x10B8604u, 0x1BB8A8u) && A5_JAL(0x10B86C0u, 0x1B8878u) && A5_JAL(0x10B86C8u, 0x1B8CD0u) && A5_JAL(0x10B86D8u, 0x1B8C90u) && A5_JAL(0x10B86E0u, 0x1BB868u) && A5_JAL(0x10B8734u, 0x1B8650u) && A5_JAL(0x10B873Cu, 0x1BBC08u) && A5_JAL(0x10B87F4u, 0x1B8638u) && A5_JAL(0x10B8800u, 0x1BB8A8u) && A5_JAL(0x10B88C4u, 0x1B8878u) && A5_JAL(0x10B88CCu, 0x1B8CD0u) && A5_JAL(0x10B88D4u, 0x1B8C90u) && A5_JAL(0x10B88FCu, 0x1B8650u), 1)) { const uint64_t a2_2 = Ps2GprU64(ctx, 2); const uint64_t a2_4 = Ps2GprU64(ctx, 4); const uint64_t a2_5 = Ps2GprU64(ctx, 5); const uint64_t a2_6 = Ps2GprU64(ctx, 6); const uint64_t a2_7 = Ps2GprU64(ctx, 7); const uint64_t a2_10 = Ps2GprU64(ctx, 10); const uint32_t a2_11 = Ps2GprU32(ctx, 11); const uint64_t a2_29 = Ps2GprU64(ctx, 29); const float a2_f0 = ctx->f[0]; const float a2_f1 = ctx->f[1]; const float a2_f2 = ctx->f[2]; const float a2_f3 = ctx->f[3]; const float a2_f4 = ctx->f[4]; const float a2_f5 = ctx->f[5]; const float a2_f6 = ctx->f[6]; const float a2_f7 = ctx->f[7]; const float a2_f8 = ctx->f[8]; const float a2_f9 = ctx->f[9]; const float a2_f10 = ctx->f[10]; const float a2_f11 = ctx->f[11]; const float a2_f12 = ctx->f[12]; const float a2_f13 = ctx->f[13]; const float a2_f14 = ctx->f[14]; const float a2_f15 = ctx->f[15]; const float a2_f16 = ctx->f[16]; const float a2_f17 = ctx->f[17]; const float a2_f18 = ctx->f[18]; const float a2_f19 = ctx->f[19]; const float a2_f20 = ctx->f[20]; const float a2_f21 = ctx->f[21]; const float a2_f22 = ctx->f[22]; const float a2_f23 = ctx->f[23]; const float a2_f24 = ctx->f[24]; const float a2_f25 = ctx->f[25]; const float a2_f26 = ctx->f[26]; const float a2_f27 = ctx->f[27]; const float a2_f28 = ctx->f[28]; const float a2_f29 = ctx->f[29]; const float a2_f30 = ctx->f[30]; const float a2_f31 = ctx->f[31]; const uint64_t a2_hi = ctx->hi; const uint64_t a2_lo = ctx->lo; A13_CHK(updateLightGeo, a2_2, a2_4, a2_5, a2_6, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a2_hi, a2_lo); a4::vec4 a4_vf0 = ctx->vu0_vf[0]; a4::vec4 a4_vf1 = ctx->vu0_vf[1]; a4::vec4 a4_vf2 = ctx->vu0_vf[2]; a4::vec4 a4_vf3 = ctx->vu0_vf[3]; a4::vec4 a4_vf4 = ctx->vu0_vf[4]; a4::vec4 a4_vf5 = ctx->vu0_vf[5]; a4::vec4 a4_vf6 = ctx->vu0_vf[6]; a4::vec4 a4_vf7 = ctx->vu0_vf[7]; a4::vec4 a4_vf8 = ctx->vu0_vf[8]; a4::vec4 a4_vf9 = ctx->vu0_vf[9]; a4::vec4 a4_vf10 = ctx->vu0_vf[10]; a4::vec4 a4_vf11 = ctx->vu0_vf[11]; a4::vec4 a4_vf12 = ctx->vu0_vf[12]; a4::vec4 a4_vf13 = ctx->vu0_vf[13]; a4::vec4 a4_vf14 = ctx->vu0_vf[14]; a4::vec4 a4_vf15 = ctx->vu0_vf[15]; a4::vec4 a4_vf16 = ctx->vu0_vf[16]; a4::vec4 a4_vf17 = ctx->vu0_vf[17]; a4::vec4 a4_vf18 = ctx->vu0_vf[18]; uint16_t a4_vi15 = ctx->vi[15]; a4::vec4 a4_vacc = ctx->vu0_acc; float a4_vq = ctx->vu0_q; uint16_t a4_vstat = ctx->vu0_status; uint64_t a4_h2 = a4::hi64(ctx->r[2]); uint64_t a4_h3 = a4::hi64(ctx->r[3]); uint64_t a4_h4 = a4::hi64(ctx->r[4]); uint64_t a4_h5 = a4::hi64(ctx->r[5]); uint64_t a4_h6 = a4::hi64(ctx->r[6]); uint64_t a4_h8 = a4::hi64(ctx->r[8]); uint64_t a3_xo[2] = {(uint64_t)a1_bits(ctx->f[0]), (uint64_t)a1_bits(ctx->f[20])}; A13_IN(13); const uint64_t a2_r = typed::updateLightGeo(rdram, runtime, a2_2, a2_4, a2_5, a2_6, a2_7, a2_10, a2_11, a2_29, a2_f0, a2_f1, a2_f2, a2_f3, a2_f4, a2_f5, a2_f6, a2_f7, a2_f8, a2_f9, a2_f10, a2_f11, a2_f12, a2_f13, a2_f14, a2_f15, a2_f16, a2_f17, a2_f18, a2_f19, a2_f20, a2_f21, a2_f22, a2_f23, a2_f24, a2_f25, a2_f26, a2_f27, a2_f28, a2_f29, a2_f30, a2_f31, a2_hi, a2_lo, a4_vf0, a4_vf1, a4_vf2, a4_vf3, a4_vf4, a4_vf5, a4_vf6, a4_vf7, a4_vf8, a4_vf9, a4_vf10, a4_vf11, a4_vf12, a4_vf13, a4_vf14, a4_vf15, a4_vf16, a4_vf17, a4_vf18, a4_vi15, a4_vacc, a4_vq, a4_vstat, a4_h2, a4_h3, a4_h4, a4_h5, a4_h6, a4_h8, a3_xo); A13_OUT(); Ps2SetGpr64(ctx, 2, a2_r); ctx->f[0] = a1_flt((uint32_t)a3_xo[0]); ctx->f[20] = a1_flt((uint32_t)a3_xo[1]); ctx->vu0_vf[1] = a4_vf1; ctx->vu0_vf[2] = a4_vf2; ctx->vu0_vf[3] = a4_vf3; ctx->vu0_vf[4] = a4_vf4; ctx->vu0_vf[5] = a4_vf5; ctx->vu0_vf[6] = a4_vf6; ctx->vu0_vf[7] = a4_vf7; ctx->vu0_vf[8] = a4_vf8; ctx->vu0_vf[9] = a4_vf9; ctx->vu0_vf[10] = a4_vf10; ctx->vu0_vf[11] = a4_vf11; ctx->vu0_vf[12] = a4_vf12; ctx->vu0_vf[13] = a4_vf13; ctx->vu0_vf[14] = a4_vf14; ctx->vu0_vf[15] = a4_vf15; ctx->vu0_vf[16] = a4_vf16; ctx->vu0_vf[17] = a4_vf17; ctx->vu0_vf[18] = a4_vf18; ctx->vi[15] = a4_vi15; ctx->vu0_acc = a4_vacc; ctx->vu0_q = a4_vq; ctx->vu0_status = a4_vstat; ctx->r[2] = a4::mk128(a4_h2, Ps2GprU64(ctx, 2)); ctx->r[3] = a4::mk128(a4_h3, Ps2GprU64(ctx, 3)); ctx->r[4] = a4::mk128(a4_h4, Ps2GprU64(ctx, 4)); ctx->r[5] = a4::mk128(a4_h5, Ps2GprU64(ctx, 5)); A6_BRKS(); ctx->pc = Ps2GprU32(ctx, 31); return; } // a13
    }
    ::updateLightGeo_0x110b038(rdram, ctx, runtime);
}
}

namespace
{
    struct A13Ad
    {
        uint32_t addr;
        PS2Runtime::RecompiledFunction rec, ad;
    };
    const A13Ad kA13Ad[4] = {
        {0x1BA2E0u, &::iosFvecToMmColMvec_0x1ba2e0, &a13ad::iosFvecToMmColMvec},
        {0x1BA320u, &::iosMmColToFvecMvec_0x1ba320, &a13ad::iosMmColToFvecMvec},
        {0x10B7EB8u, &::GetMorphAnimation_0x10b7eb8, &a13ad::GetMorphAnimation},
        {0x110B038u, &::updateLightGeo_0x110b038, &a13ad::updateLightGeo},
    };
}

// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function
uint32_t a13InstallAdapters(PS2Runtime &runtime)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < 4u; ++i)
        if (runtime.lookupFunction(kA13Ad[i].addr) == kA13Ad[i].rec)
        {
            runtime.registerFunction(kA13Ad[i].addr, kA13Ad[i].ad);
            ++n;
        }
    return n;
}
