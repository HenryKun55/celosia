// typed_a7.h: round A7's typed functions (tools/typed_lift2.py, stage 7; docs/decomp_a7.md). Generated: do not edit.
// What A1-A6 left behind a callee they could not type and A7 types: callers that read a callee's scratch register after the
// call take it back from the callee's extra outputs (a typed out-parameter, written where the callee changes it; earlier
// functions get a variant <name>_x7 typed with them), the printf-family stubs as typed calls to their cores (typed_a7_ext.h),
// more hand natives typed from their MIPS, and every caller that then had all its callees typed.
#pragma once
#include "typed_a6.h"
#include "typed_a7_rt.h"
#include "typed_a7_ext.h"

namespace typed
{
#include "typed_a7.inc"
}

// what the adapters test the slots against (A7_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_CheckInAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_CheckNormZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAreaDistScale2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetCurMotionKind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetHitPosPerpendicular(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetInterRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetInterRatioOutCut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetNegaVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetSideSignalFromDirLev(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetTimeFromAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetTimeOfCurrMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetYRotQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_InitAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_InitAnimalOptimize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_InitGexCommonWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_InitSurpriseArrow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_SetAnimalWorkParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_SetSurprisedArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_ground_contact_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_motion_kind_interp_ratio_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_init_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_reset_aim_angle_rot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_reset_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_set_all_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ChangeCharacterObjControlTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ChangeCharacterObjControlledTarget(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CharaCtrlSetOutsideScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CheckClothChainExist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CheckLegalCollisionGP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ColCont2_IsColStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ColCont2_IsParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ConstructorCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DecreaseDamageLockTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DivQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ForceParentLayoutObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ForceUnparentLayoutObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_begin_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_check_parent_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_constructor_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_horse_move_status(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_increase_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_local_directionX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_local_directionY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_local_directionZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_motion_kind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2_extend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_offset_position_by_option_space(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_parent_collision_attribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_parent_object(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_riding_horse_object(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_segment_num_and_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_sight_on_angle_vel_ratio_float(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_sight_on_reverse_cntrlLR_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_sight_on_reverse_cntrlUD_bool(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_pitch_limit_clip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_increase_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_init_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_boss_booting(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_boss_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_horse_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_horse_obj_contact_ground(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_player_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_reach_aim_yaw_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_mul_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_near_to_aim_pitch_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_reset_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_reset_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_custom_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_parameter_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_parameter_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_parameter_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCharacterClassInitParamNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttributeAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetDamagerClassDamageSheetType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerCollisionDB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionFrameDirectResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionMatrixOfTargetFromRoot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionResourceFrames(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionResourceOriginalFrames(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionStretchRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetOffsetPositionAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetParentCharColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneFittingPointForUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptSpriteClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptSpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSizeOfModifiedManagerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSubLinkClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetTRS_Onake_From_ObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void IirFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Init_Controler(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Init_Player(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Init_Shock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_CmpPathPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_InitKitePathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_InitSpecialPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Kite_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LinkSecureParentInfoByGP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LoaderSysFWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ReflectIK_initWormDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ResetAnimeFrameTableInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SCRGetBossWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCharacterClassMotionNormalSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCharacterObjMotionShiftCallBackEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetDisplayModelNearClippingSwitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerGlobalState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerNodePosState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetJudgeVolumeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetVSMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2CtrlEntryTickProcById(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotFindSlotClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotFindSlotClassCh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetBendValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetLfoPitchDepth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetMute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2VabHdClose(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Shock_SetShockVoiceSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetLayerByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetLayerNoByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetMenuIdByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetDispFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetDispFlagByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetPartsDefValueByHashStr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UnlinkSecureParentInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameIWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _resetBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _setStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void arrowFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void begin_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bossStatBootChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bossStatGetActiveBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void calcAngleTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void calcMoveAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void calcProgRotRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void charrootdyn_getRegularColFinderPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void checkFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void chkNearStickArrow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_camera_context(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_camera_lws_anime_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void deleteUserPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void doit_aim_angle_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void doit_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_object_interest_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void entry_new_letter_type(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void equipmentInvisibilityCloak(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fruitsFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fruitsFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fsEffectDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fsEffectManagerInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fsEffectSetParamSheetSeg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gStatGetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gStatGetPlayerAccessoriesDamgeRaitoToBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gStatGobjDispGflagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameFlagCheckPointSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01347920(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01347F88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01347FC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01348218(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013772B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013BBE20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013EBE10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013EF210(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013F35C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01411AD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01411AF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01412BF0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01413708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01413720(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141B480(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141B6C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141C4A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01423410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01425700(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01427408(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01434610(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01444370(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01444760(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_014498B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0144CDF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0144CEA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getArrowObjParent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBlendBaseMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBossPartsLifeRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBossWeakCurDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBossWeakLifeRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildSubPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDemoBarFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDispScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDispScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getEditWorkInitData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFadeValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getInterestExec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getNowPowerRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPathExFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPathStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerColCont2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerHp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerHpRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerLastAttrStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerMaxGripRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerMaxHp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerWeaponHitAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSObjNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSScriptWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getTorchInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getWayFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_base_yaw_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_fixed_point_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_cameraman_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_type_camera_lws_animation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_scissor_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_screen_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_sight_on_cntrl_param_player_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void graphSetFogParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void initInterestWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void init_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCdSetMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetScaleInverseVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTCountExtension(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableSin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMakeLightColorMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMakeNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetBGColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_doing_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_enable_base_pitch_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_parent_obj_boss_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_pass_camera_etc_condition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_playing_lws_movie_now_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_predict_to_acting_base_yaw_rot_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void isysInitGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001C1538(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001CDDE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01084A70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01084C30(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010BA140(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01129258(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_011601C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_011629E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164AC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164BC0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void modelGetDlLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void mouseSetCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_GetCharacterAttackedFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_IK_getAddSubPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_playerCheckNippingWithHorseAndWall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___unpack_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void resetAttackWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void resetPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void resetVelocityWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_camera_collision_data_except_slide_move_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_double_spring_interp_cntrl3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_time_interp_param_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void seSetKeyOnOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void seStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainRadPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainRangeStruct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainSpeedWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDirAndLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDirPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDirPosAndLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setCreateTempAttain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setLoadMapOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setObstacleWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setOffsetParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setPassageRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setPerceptionParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setRotColCheckWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSCRAttainRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSearchRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setUserPathMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setWallStopHorseCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_base_pitch_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_base_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_max_min_pitch_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_node_data_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_npe_blend_ratio_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_offset_pitch_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_offset_yaw_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_option_parameter_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_param_double_spring_interp_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_parameter_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_parameter_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_reverse_input_pitch_flag_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_reverse_input_yaw_flag_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_screen_size(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_yaw_pitch_input_border_parameter_nico_yaw_pitch_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sheetMgrChkStageIndexResolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sheetMgrSetNoWaitMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepCont(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepStopTypeAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stagemanager_checkstageAlreadyLoadedByStrptr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stop_particle_emitter_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stop_particle_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sub_00125998(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sub_001259E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sub_0012D7F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void updateAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void updateAttackWaistIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a6ad { void CalcDistancePointAnd2Point(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void ColCont2_EventAttrReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void DEBUG_get_base_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GAMEUTIL_get_limit_clip_yaw_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GAMEUTIL_is_between_yaw_and_yaw_extend_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetMotionFrameWithOutsideLimit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetWindowClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SetCharacterClassMotionAddSyncMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SpriteMenuGetPartsDefByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SpriteMenuSetCheckBoxByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void _isysRecvSubMsg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void _omakeSelectFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void bgmAutoStart(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void forceRenewLastPosPerceptionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void forceSetPerceptionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gStatGobjDispGflagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void getBuryingWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void getPPointPasasgeNum(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setAcceleParamWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setBossIndexTimeAttackBestTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setGpCalcDist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setMiddleTargetWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void sndSepStopReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void windowCloseRequest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void windowGetCursorPosOnWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void windowSetCursorPosOnWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void GAMEUTIL_is_swimming_now(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void IsHorseCalled_withFlgOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void SendCharaNodeParentSetMsg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void SpriteMenuGetPartsDefValueByHashStr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void doit_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void getBossPartsCurDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void player_SetGripDamage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void setAttainmentObjAndOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void setGameFlagForGameObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_heap120 { void allocFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_heap120 { void insertFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRCharaAIWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRDataCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void setScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_sym120 { void makeHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosJumpRecoverPoint_0x1b3410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcmp_0x128188(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcpy_0x12821c(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void printf_0x1285c0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sceWrite_0x10b678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strlen_0x1290c4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
