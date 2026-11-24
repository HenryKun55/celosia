// typed_a14.h: round A14's typed functions (tools/typed_lift2.py, stage 14; docs/decomp_a14.md). Generated: do not edit.
// What A1-A13 left refused for runtime state lift_d1 did not name: the second multiply pipeline's hi1 / lo1, the shift-amount
// register, COP0 Status (EIntr / DIntr), VU0's clip flags and R register as slots of the VU0 interface (RT14); a likely branch's
// break slot recorded for the adapter (BRK14); every caller that then had all its callees typed (variants <name>_x14).
#pragma once
#include "typed_a13.h"
#include "typed_a14_rt.h"

namespace typed
{
#include "typed_a14.inc"
}

// what the adapters test the slots against (A14_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 / A11 / A12 / A13 / A14 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_CalcIntersectionCircleXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_set_all_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ChangeGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DIntr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void EIntr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_player_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetStageAreaNumberByPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetStageContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void IirFilter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void IsInsideOfRegion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void PauseCamFinalizer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void PauseGame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void RestoreDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCamFinalizerGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetDefaultGameAnimeTimeScaleRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2CtrlEntryTickProcById(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2CtrlGetTickMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2ModMidiout1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2RemoteSendDmaCom(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SepStopSndOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotFindSlotClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotFindSlotClassCh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotGetSlotClassIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetKeyOffBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetLfoPitchSpeed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetNoiseOffBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _sceMcpIsSpeckOutBlock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void allResetLineAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void clipByMinMaxCube(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void clipBySphere(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void constructor_camera_lws_anime_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameModeCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gameModeSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01345980(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0137EA00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0137EB38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0137F508(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0137F850(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0137FD70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01423410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getConnectLineArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFrameBufferSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLineData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPathDataAddr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPathInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getRootPointData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getZBufferAddressFBP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_main_cameraman_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getstagenumber(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iAllocTimerCounter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iGetTimerBaseTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iGetTimerSystemTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iGetTimerUsedUnusedCounters(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosEI(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetAlignSinCosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosInverseMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetCameraMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetDCParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_have_region_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_parent_obj_boss_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001BEC18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001CDDE8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001CDF38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001DAAB0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001DB3C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001DB418(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01099A08(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_0110EB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_0110EDC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_011641E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_01164260(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_playerSetAttentionHeadActFlag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___mcmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___muldi3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___udivdi3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void rintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void scrInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainmentPosAndDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setDataHeaderFromAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setDemoBar(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setEventLineAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void unsetEventLineAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_get_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_get_lfotbl_param(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_get_veltbl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a11ad { void is_clear_camera_region_condition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a11ad { void manager_sub_01162FD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void GetStageCenter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void Sg2SlotModKeyOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void Sg2SlotSetDumperOff(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void Sg2SlotSetReverbOffBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void Sg2SlotSetReverbOnBit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void gamecore_sub_01346090(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void gamecore_sub_01361E88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void getPointDataFromEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void getPointPosFromEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void iosGetDRandomVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DA790(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DA850(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DA8F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DA9C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DB088(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void kernel_sub_001DB238(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void setEventLinePrio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a14ad { void sub_00109288(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void texGetCSM1Idx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a7ad { void kernel_sub_001DAB10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryBlendLast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_libm { void acosExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_math { void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void absVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRCharaAIWorkPtr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm2 { void getSCRDataCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosJumpRecoverPoint_0x1b3410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcmp_0x128188(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcpy_0x12821c(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strcpy_0x128fb0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vsprintf_0x12c048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
