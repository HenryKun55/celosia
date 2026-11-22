// typed_a3.h: round A3's typed functions (tools/typed_lift2.py, stage 3; docs/decomp_a3.md). Generated: do not edit.
// Real C++ functions with loops (goto-based, locals, no context: a back edge charges the EE clock and the checkpoint is
// deferred to the first one after the call, A3_BACK), frames and calls; the A1 / A2 functions they call are in
// typed_a1.h / typed_a2.h.
#pragma once
#include "typed_a2.h"
#include "typed_a3_rt.h"

namespace typed
{
#include "typed_a3.inc"
}

// the lifted functions whose slots the adapters test (A3_HOLDS / A2_SLOT)
namespace lift_d1
{
void AI_UpdateVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_interp_value_interp_value_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_push_or_down_1p_button(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2CtrlSetTickTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _gameFWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeGetFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _omakeSelectFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _optionWorkSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bgmStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void cancel_fadeout_weakpoint_effect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void fadeout_weakpoint_effect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gStatSetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_014144D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_014144F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void initWeaponSpriteStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void kernel_sub_001B06C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void set_param_double_spring_interp_common(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
