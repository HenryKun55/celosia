// typed_a5.h: round A5's typed functions (tools/typed_lift2.py, stage 5; docs/decomp_a5.md). Generated: do not edit.
// What A1-A4 refused for their form and A5 types: functions proven against their callees' typed interfaces (the typed
// ABI proof), relocated calls (the adapter tests the jal word in RAM, A5_JAL), frames whose address escapes kept in
// guest RAM below the guest sp (sp a local guest address, no context), 32-bit inputs passed whole to a 64-bit
// parameter, jump tables as switch statements; and every caller that then had all its callees typed.
#pragma once
#include "typed_a4.h"
#include "typed_a5_rt.h"

namespace typed
{
#include "typed_a5.inc"
}

// the lifted functions whose slots the adapters test (A5_HOLDS / A2_SLOT)
namespace lift_d1
{
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_camera_position_by_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_is_visible_now_target_visibility(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_set_all_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ChangeCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ExchangeLWVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_doit_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_answer_of_2jihouteishiki(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_cur_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_data_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_pitch_and_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_ratio_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_new_velocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_new_velocity_by_brake_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_status_change_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_mul_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_reset_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCharacterClassInitParamNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCrossPositionPlaneLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetOffsetPositionAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void IsFullFixFormat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void RequestCharacterMotionShiftFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ResetCharacterClassWaterStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCharacterClassGazeAdditionChannelInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetDamageClassDamageTableType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetAnimParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UnlinkSecureParentInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameIWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameIWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _optionWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void act_camera_cutout_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void adpcmOpenChkComplete(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bossStatGetActiveBoss(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_camera_lws_anime_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_wind_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gStatGobjDispGflagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_013481A0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0140FE00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0140FE20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0140FE50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0140FE70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141BCF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141D4F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameflowGetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameflowSetJumpSekiban(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getChildGoalPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getConnectLineArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDispScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDispScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getEditWorkInitData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFreeModeRevAngleVelRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFreeModeRevControlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFreeModeRevControlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerMaxGripRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getScreenOffsetX(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getScreenOffsetY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSightOnRevCntrlLR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSightOnRevCntrlUD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSpecialEffectObjWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSubPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_cameraman(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_mode_common_data_filter_base_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_effect_id_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_top_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void glSetFogFactor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void glSetFogFactorDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void graphSetFogParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCdStSectorAlignSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCdStSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosFloatModulo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetReflectVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTCountExtension(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosIColToFvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosITOF0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetBGColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSprToMemDmaAdrCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_camera_target_debug_move(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_leave_camera_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_loop_particle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void isysGetSubThreadWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void isysInitGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B53F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_0110EB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01140740(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void mcGetCompleteTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void padSysGet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_game_camera_standby_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainRangeStruct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFogImmediate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSoundMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setZFogFar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepSetDefaultParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stop_wind_emitter_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stop_wind_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void usbdevMouseGetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
