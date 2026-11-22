// typed_a4.h: round A4's typed functions (tools/typed_lift2.py, stage 4; docs/decomp_a4.md). Generated: do not edit.
// Real C++ functions that use VU0 / 128-bit values: the VU0 registers, ACC, Q, status and the GPRs' upper halves a
// function reads or writes are reference parameters (its VU0 interface, the VU0 ABI), vec4 / q128 locals, the VU0
// macro operations and MMI as named helpers (typed_a4_rt.h); no R5900 context, no VU0 emulation state.
#pragma once
#include "typed_a3.h"
#include "typed_a4_rt.h"

namespace typed
{
#include "typed_a4.inc"
}

// the lifted functions whose slots the adapters test (A4_HOLDS / A2_SLOT)
namespace lift_d1
{
void AI_GetAccelA3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelC3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetPosition3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_GetVelocity3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_InitAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void AI_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_convert_from_lw_to_game_coord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_parameter_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _resetBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01411AF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01440FE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01444568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_0144CEA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle_camera_view_angle_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void resetVelocityWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
