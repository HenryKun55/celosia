// typed_a6.h: round A6's typed functions (tools/typed_lift2.py, stage 6; docs/decomp_a6.md). Generated: do not edit.
// What A1-A5 left behind a callee they could not type and A6 types: never-lifted functions and hand natives typed from
// their MIPS (no adapter: a typed caller calls them while the slot holds the recompiled function / the native, A6_HOLDS),
// calls to the runtime's libc / thread-id stubs as typed calls to the stubs' own cores (typed_a6_ext.h), `break` (the
// cop0 effects applied by the adapter after the call), and every caller that then had all its callees typed.
#pragma once
#include "typed_a5.h"
#include "typed_a6_rt.h"
#include "typed_a6_ext.h"

namespace typed
{
#include "typed_a6.inc"
}

// what the adapters test the slots against (A6_HOLDS): lifted functions, never-lifted functions' recompiled functions, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_GetMotionBasicVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ChangeCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CheckLegalCollisionGP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ConstructorCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DivQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_cur_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_dist_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_divide_vector_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_timer_interp_ratio_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_pitch1_to_pitch2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_new_velocity_by_brake_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_inc_or_dec_for_near_to_aim_pitch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_between_yaw_and_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetClothWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPointDistanceFromPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetTwoWayLinkData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetWorldLayoutSheetSegHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitQuaternionDrive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LoaderSysCheckCDBootMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ReleaseGame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void RequestCharacterMotionShiftFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ResetCharacterClassFallDownInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SeemlessInitFogTargetParamWorldLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetInitFogColClassFromStageData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetLayoutObjCollisionEnableFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2ModCheckSndOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2RemoteGetAdpStmAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2RemoteGetPcmStmTransOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotFree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotTbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UnlinkSecureParentInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UpdateGlobalLightOutRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _hi0bits(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _localeconv_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _resetBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _setStopCtrlModeBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void adpcmGamePause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void backStageCheckPointSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bgmPlay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bgmStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void calcMoveAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void checkFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void deleteUserPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void endBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void existSoundBow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameFlagCheckPointSave(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0141D4F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0143D660(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0143D718(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameflowSetPauseEnable(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getEffectBloodWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGameClearCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGoalPointDataByAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getHorseStandByMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getOptionDataAdrsAndSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPlayerGrip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRatioFromInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSprLogStatusAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSprLogStatusSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSprLogWorkAddress(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSprLogWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_clip_screen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_cur_camera_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosAllocPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlChainTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlGetWorkBufId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDumpFVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosFTOI0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetBGColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetSyncNoReturnFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosWaitDMA(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void isysGetSubThreadWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001C8B98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001DB3C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_0110EB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01113BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01141080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void mcGetCompleteTimeAttack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void mouseSetCursorPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___errno(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___pack_f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void resetPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_camera_collision_data_except_slide_move_ratio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void reset_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void scrInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void seStopLoopType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainRadPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setBossDeathEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFadeColor(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sheetMgrChkStageIndexResolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepPauseAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void spriteMenuObjPadChangeHorse(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stagemanager_checkstageAlreadyLoadRequestedByStrptr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void texScaleClut(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a6ad { void ChangeLayoutObjCollisionResourceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GAMEUTIL_is_between_yaw_and_yaw_extend_abs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void MotSpdCtrl_SetSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SCRSetCharaMotionPlaySpeedRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SetCaptionPauseFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SetTimeAttackTimeFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void Sg2StAdpcmIopReadAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void _setlocale_r(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void activeDemoCancel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void bgmScriptRecvSoundSignal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void forceAddPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gStatGobjDispGflagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gamecore_sub_014454B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gamecore_sub_014454E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void gameflowSetCaptionPause(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void get_cur_base_mode_condition_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void get_cur_base_mode_manager_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void manager_sub_010BA688(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void modelGetName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setLoadingCheckFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void sndSepStopReq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void windowCloseRequest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_sema { void getThreadIdNative(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void memcmp_0x128188(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcpy_0x12821c(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strcpy_0x128fb0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strlen_0x1290c4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
