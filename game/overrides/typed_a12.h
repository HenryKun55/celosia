// typed_a12.h: round A12's typed functions (tools/typed_lift2.py, stage 12; docs/decomp_a12.md). Generated: do not edit.
// What A1-A11 left behind a callee they could not type and A12 types: the motion subsystem's natives typed from their MIPS
// (docs/a12_natives.txt: a typed caller calls the typed function while the slot holds the native and A12_HOLDS says the native's
// path is the proven one), the spin rule's base taint flow-sensitive (a list walk), a likely branch's restore on the taken edge, a
// callee-written register passed to a 64-bit parameter taken back as an extra output; every caller that then had all its callees
// typed (variants <name>_x12 of earlier functions).
#pragma once
#include "typed_a11.h"
#include "typed_a12_rt.h"

namespace typed
{
#include "typed_a12.inc"
}

// what the adapters test the slots against (A12_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 / A11 / A12 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_CheckDamage_Bow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_CheckDamage_Impact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_CheckDamage_SoundBow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_CheckDamage_Sword(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_CheckInAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetCurControlMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetCurMotionKind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetMotionBasicVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AddCharacterClassOptionNodeSubGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_is_visible_now_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CharaCtrlSetOutsideScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ColCont2_IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void EnableCharacterObjMotionRootRotDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_convert_from_cos_to_linear_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_look_at_vs_offset_vec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_camera_target_look_at_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_direction_by_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_direction_by_yaw_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_horse_move_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_increase_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_local_directionZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_motion_kind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2_extend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_node_posi_by_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_object_move_noise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_yaw_pitch_by_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_diff_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_horse_obj_contact_ground(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_push_or_down_1p_button(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_reach_aim_yaw_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_yaw_pitch_get_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_near_to_aim_pitch_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_need_dir_ratio_flag_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_parameter_look_at_vs_offset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCharacterClassInitParamNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttributeAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetDamagerClassDamageSheetType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerCharColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfaceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfacePlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfaceSlopeDegree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMeshClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionColSphereInfoIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionNodeGeoToolCoord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionNodePos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionStretchRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneSlopeDegree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneTopDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneZTopYPlaneCoordQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetQuaternionAxisRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetRootRotStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptSpriteClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptSpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSkeletonFocusNodeDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSubLinkClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetTurnYAngleFromGlobalDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitQuaternionDrive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void KickParticleGetResourceId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_StartEventCheck(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LoaderSysPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MotSpdCtrl_StartExt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MultiCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void PopQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ResetAnimeFrameTableInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SCRSetCharaMotionPlaySpeedRatioDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ScriptAnimationObjOfJudgeVolumeObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetJudgeVolumeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByAxisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByAxisRotateV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetLayerNoByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetMenuIdByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetAnimParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetNumPartsFormatByPartsParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameIWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeGetFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void acted_camera_warp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void addPlayerMaxGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void addPlayerMaxHp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void adjustAnglePI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void changeFogParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void charrootdyn_resetVColEdgeFindFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void charrootdyn_setVColFindFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void doit_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void doit_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fsEffectSetParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameFlagCheckPointSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameModeCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameModeSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01387270(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013F14D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013F1938(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013F67B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_014143D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01414408(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_014193C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141B978(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141B988(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141B998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BA40(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BA68(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BB20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BB88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BC78(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BCF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141C518(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141C620(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141C9B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141C9E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01440AB0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBlendBaseMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildGoalPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildGoalPointIdxArrayByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildSubPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildSubPointIdxArrayByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getConnectLineArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDeadDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDeadPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDemoBarFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFadeValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogColorB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogColorG(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogColorR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFarDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFarRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogNearDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogNearRate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getInterestDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getInterestPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLayoutDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLayoutPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getNearPointPosUserPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerColCont2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPointDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSObjNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSScriptWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSpecularity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getValueInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_cur_using_lws_anime_mode_base_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle_camera_view_angle_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void graphSetFogParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetAlignSinCosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableSin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMakeLightColorMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMakeNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulRotMatrixYInt16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetBGColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransMatrixF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_camera_target_change(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_leave_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_view_angle_exist_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_view_angle_zoom_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001AE9B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01084A70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01084C30(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010965D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010AD708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010ADA10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010ADBD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B5238(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B53F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B5738(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B84B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B8798(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010C9888(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010F8088(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01140E60(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_011662C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void modelGetDlLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void mouseSetCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_GetCharacterAttackedFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_GetCharacterAttackedGPAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_GetPlayerCharacterRidingHorseObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void omakeGetAndSelectCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void omakeGetAndSelectSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void player_setDangerEdgePoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void playerlifebar_effect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void rintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setBirdModeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setExecGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setExecRootQuatGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFogImmediate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setGazeCoffGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setGoingAngleGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setNextDistGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setObjGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setRatioGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSCRGazeStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSubParamGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setUserPathMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setValidPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setViewJudgeGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_generate_particle_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_param_double_spring_interp_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setgrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void startGaugeMaxAddEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_get_splitchunk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a12ad { void GetCharacterClassMotionPhysicalNodeID(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void GetMotionOptionNodeFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void SpriteMenuSetMenuDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void changedAPIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void changedGPIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void gamecore_sub_01435C28(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void gamecore_sub_01435D68(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void manager_sub_011472A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a12ad { void vab_get_progchunk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GAMEUTIL_is_between_yaw_and_yaw_extend_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetWindowClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void PushQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SpriteMenuSetCheckBoxByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SpriteMenuSetPartsDefValueByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gamecore_sub_014362E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void manager_sub_010BA1F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void manager_sub_010BA688(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setAcceleParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void windowSetCursorPosOnWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void SCRGetDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void gStatOmakeSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void setHorseForceScriptKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void GAMEUTIL_get_rot_dir_for_near_to_aim_yaw_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void SCRGetAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace cam126 { void ccNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryBlendLast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryOptionNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryPhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entrySubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void verticesIndepEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_curve { void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_heap120 { void insertFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot44d { void nodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot46h { void keyVec3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot46h { void optionNodeSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot47i { void optionNodeGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
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
namespace decomp_scriptvm3 { void getScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void setScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skeleton { void motionMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skeleton { void skeleton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skinfinder { void corners(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_sym120 { void makeHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosJumpRecoverPoint_0x1b3410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosPopCurrentMatrix_0x1b8650(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosPushCurrentMatrix_0x1b8638(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcmp_0x128188(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void printf_0x1285c0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strlen_0x1290c4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vsprintf_0x12c048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
