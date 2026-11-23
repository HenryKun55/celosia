// typed_a11.h: round A11's typed functions (tools/typed_lift2.py, stage 11; docs/decomp_a11.md). Generated: do not edit.
// What A1-A10 left behind a callee they could not type and A11 types: the matrix stack's users (iosPushCurrentMatrix /
// iosPopCurrentMatrix as typed calls on VU0's data memory, vi15 a slot of the VU0 interface: typed_a11_ext.h), recursion over
// it (stage 10's fixpoint), and every caller that then had all its callees typed (variants <name>_x11 of earlier functions).
#pragma once
#include "typed_a10.h"
#include "typed_a11_rt.h"
#include "typed_a11_ext.h"

namespace typed
{
#include "typed_a11.inc"
}

// what the adapters test the slots against (A11_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 / A11 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AI_CalcIntersectionCircleXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_get_target_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DivQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_parent_object(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerCharColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerObject(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfaceAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfacePlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutParentObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLimitationQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionMatrixOfTargetFromParent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneSlopeDegree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneTopDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneZTopYPlaneCoordQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetQuaternionAxisRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetRateInOutEllipticalSphereVolumeWithInverseMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetTurnYAngleFromGlobalDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void IsInsideOfRegion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MultiCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void PutString(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetQuaternionByAxisRotateV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void StabilizeToYRotQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void charrootdyn_getCollisionAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void chkAttainExecLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void chkAttainExecOffsetData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void create_original_view_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01462340(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01462B30(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getEditWorkInitData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPassageWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosFloatModulo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetAlignSinCosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetTableSin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosInverseMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulRotMatrixXInt16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulRotMatrixZInt16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosRotMatrixZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetCameraMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetDCClearParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetDCParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosSetDCViewPort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransMatrixF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransposeMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_have_region_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_parent_obj_boss_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001BEC18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B8798(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_GetGeometryPointerAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_calcTailGenealogicalNodeNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_calcWormGenealogicalNodeNextBase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void nico_calcWormGenealogicalNodeTailBase(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setAttainRangeStruct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setSCRAttainRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepSetPan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sndSepSetVol(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a11ad { void manager_sub_010B8958(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a11ad { void manager_sub_010C38E8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a11ad { void manager_sub_01162FD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a9ad { void SCRGetXZAngle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void verticesIndepEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
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
namespace decomp_scriptvm3 { void setScriptCtrlStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skinfinder { void corners(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosPopCurrentMatrix_0x1b8650(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosPushCurrentMatrix_0x1b8638(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
