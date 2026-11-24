// typed_a13.h: round A13's typed functions (tools/typed_lift2.py, stage 13; docs/decomp_a13.md). Generated: do not edit.
// What A1-A12 left behind a callee whose hi / lo its callers read after the call: hi / lo taken back as extra outputs (HL13: the
// typed callee returns its hi / lo locals in the xo array, a typed caller takes them into its own, the adapter writes them to the
// context); every caller that then had all its callees typed (variants <name>_x13 of earlier functions).
#pragma once
#include "typed_a12.h"
#include "typed_a13_rt.h"

namespace typed
{
#include "typed_a13.inc"
}

// what the adapters test the slots against (A13_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 / A11 / A12 / A13 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AddCharacterClassOptionNodeSubGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionNodeQuaternionPosToolCoord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetStabYRotQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void InitQuaternionDrive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MultiCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void PopQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01386308(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosFTOI15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosITOF15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010B8798(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a12ad { void GetMotionOptionNodeFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void PushQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace cam126 { void ccNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryBlendLast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryOptionNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryPhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entrySubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace dc1mot { void entryVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_curve { void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot44d { void nodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot46h { void keyVec3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot46h { void optionNodeSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot47i { void optionNodeGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_math { void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_quat { void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skeleton { void motionMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skeleton { void skeleton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosJumpRecoverPoint_0x1b3410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosPopCurrentMatrix_0x1b8650(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosPushCurrentMatrix_0x1b8638(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
