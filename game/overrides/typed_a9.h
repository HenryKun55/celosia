// typed_a9.h: round A9's typed functions (tools/typed_lift2.py, stage 9; docs/decomp_a9.md). Generated: do not edit.
// What A1-A8 left behind a callee they could not type and A9 types: the quat122 / geo126 hand natives (the quaternion
// helpers, iosGetSinCosf, the arc cosine / tangent, GetMatrixFromQuatPos) as typed calls to the natives' own host-float
// cores (typed_a9_ext.h: the same code the native runs on its default path), and every caller that then had all its callees
// typed (variants <name>_x9 of earlier functions as A7's).
#pragma once
#include "typed_a8.h"
#include "typed_a9_rt.h"
#include "typed_a9_ext.h"

namespace typed
{
#include "typed_a9.inc"
}

// what the adapters test the slots against (A9_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_CheckNormZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetInterRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetNegaVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetSideSignalFromDirLev(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetYRotQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_camera_position_by_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_camera_space_axisXY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_convert_from_cos_to_linear_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_look_at_vs_offset_vec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_direction_by_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_direction_by_yaw_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_local_directionZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_yaw_pitch_by_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_yaw_pitch_get_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttributeAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObjectParentNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneFittingVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneSlopeDegree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneTopDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneZTopYPlaneCoordQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetQuaternionAxisRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetQuaternionByEulerYXZExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_CmpPathPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MultiGlobalQuaternionToRootSkeleton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void RotQuaternionXExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerGlobalState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerNodePosState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerNodePosStateGlobalRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByAxisRotateV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByAxisRotateVWithNoRegularizeRadian(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _setRootXYZHPB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void adjustAnglePI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fogSetFogColorMulRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01415158(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01415388(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLayoutClassCollisionIndepSurfBoundary(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getTurnAndPitchFromDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getWormIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_aspect_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_base_pitch_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_boss_interest_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_matrix_fovignore(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_dist_from_eye_to_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_far_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_near_clip_z_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_scissor_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_height(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_width(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_sub_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_top_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle_camera_view_angle_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_z_buf_max_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_z_buf_min_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getexpressiondatapoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetAlignSinCosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTanf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotCurrentMatrixX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotCurrentMatrixY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotCurrentMatrixZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_enable_base_yaw_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01115158(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01116698(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01119568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164858(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_011649C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164A10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164AC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164BC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_neckFitToParentInvDirIK(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDirAndLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_parameter_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_npe_blend_ratio_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a6ad { void AI_CheckNoAngleVec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void AI_GetVelocity2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GAMEUTIL_is_yaw_dir_sandwich(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void get_look_at_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setWormIKAttainXVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void manager_sub_011648D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void AI_GetQuaternionNodeDirTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_act_pitch_rot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_get_difference_radi_yaw_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_get_rot_dir_for_near_to_aim_pitch_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_get_rot_dir_for_near_to_aim_yaw_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_is_yaw_dir_inside_of_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GetQuaternionByEulerZXYExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void gamecore_sub_013CDED0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void iosGetSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void verticesIndepEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_libm { void acosExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_math { void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void absVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void atan2Exact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void axisDirZY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRCharaAIWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRDataCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skinfinder { void corners(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void memcmp_0x128188(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
