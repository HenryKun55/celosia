// typed_a8.h: round A8's typed functions (tools/typed_lift2.py, stage 8; docs/decomp_a8.md). Generated: do not edit.
// What A1-A7 left behind a callee they could not type and A8 types: the varargs functions whose argument-register spills
// (their va area in a guest frame) store the registers' entry values, more hand natives typed from their MIPS
// (docs/a8_natives.txt), and every caller that then had all its callees typed (variants <name>_x8 of earlier functions as A7's).
#pragma once
#include "typed_a7.h"
#include "typed_a8_rt.h"

namespace typed
{
#include "typed_a8.inc"
}

// what the adapters test the slots against (A8_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A8 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ClipCollisionDB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CompletePlaneD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void DeleteTwoWayLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBAttributeAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionDBSurfBoundary(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionEdgePointerGlobalVerticalPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerSurfacePlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetGeometryPointerWithCollisionDBDataGlobalPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionResourceFrames(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMotionResourceOriginalFrames(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneFittingPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetPlaneFittingPointForUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScreenRadius(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetYRotQuaternionFromDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LoaderSysFWrite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void LoaderSysPrintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SetGeometryPointerGlobalState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetLayerNoByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuSetNumPartsFormatByPartsParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UnlinkSecureParentInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void UpdateLayoutClassPastGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void charrootdyn_getInterpPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void end_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01386B98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01386EF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01462628(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getBlendBaseMotion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenHeight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getDrawScreenWidth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getFogFactor1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getLayoutClassCollisionIndepSurfBoundary(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getZFogNear(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getZFogScissorMargin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_effect_id_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void glColor4f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void glLoadMatrixf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gluGetLookAtPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosAllocPacketPtrPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosDlChainTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosScaleMatrixV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void loaderSetResetCallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void ps2___pack_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_npe_blend_ratio_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void stop_particle_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void GetScreenRadiusParticle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void setv3f(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a8ad { void LinkSecureParentInfoByDBPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a8ad { void get_particle_lod_base_size(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a8ad { void manager_sub_010B2318(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_collision { void verticesIndepEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMulti(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_ptcl62 { void geoDispClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getThisSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skinfinder { void corners(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_sym120 { void makeHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void iosJumpRecoverPoint_0x1b3410(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memcpy_0x12821c(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void memset_0x1283d0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void printf_0x1285c0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sceWrite_0x10b678(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void strlen_0x1290c4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vsprintf_0x12c048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
