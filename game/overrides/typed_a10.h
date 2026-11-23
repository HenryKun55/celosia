// typed_a10.h: round A10's typed functions (tools/typed_lift2.py, stage 10; docs/decomp_a10.md). Generated: do not edit.
// What A1-A9 left behind a callee they could not type and A10 types: recursive functions (a function that calls itself, or a
// cycle of functions calling each other, typed against its own interface to a fixpoint: each call a C++ call with its own host
// frame, or its guest frame in guest RAM below the guest sp as the MIPS keeps it), the upper-half class (a callee whose lifted
// text writes a GPR's whole 128 bits typed with that upper half where its caller's VU0 interface holds it), and every caller
// that then had all its callees typed (variants <name>_x10 of earlier functions as A7's).
#pragma once
#include "typed_a9.h"
#include "typed_a10_rt.h"

namespace typed
{
#include "typed_a10.inc"
}

// what the adapters test the slots against (A10_HOLDS): lifted functions, never-lifted functions' recompiled functions / A6 / A7 / A9 / A10 adapters, hand natives, the runtime stubs' holders
namespace lift_d1
{
void AddTwoWayLinkNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetMeshClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSubLinkClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void SpriteMenuGetMenuIdByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void dynamic_cast_to_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getSelectWeapon(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void manager_sub_010C68F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace a6ad { void GetLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void SpriteMenuSetCheckBoxByHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace a6ad { void _omakeSelectFlagCls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_copymatrix { void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_leaves { void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_scriptvm3 { void getScriptLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_sym120 { void makeHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
void strlen_0x1290c4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
