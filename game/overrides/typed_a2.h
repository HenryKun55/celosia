// typed_a2.h: round A2's typed functions (tools/typed_lift2.py; docs/decomp_a2.md). Generated: do not edit.
// Real C++ functions with frames (host arrays, no guest sp) and calls (C++ calls to typed callees), written from
// lift_d1's IR; the A1 functions they call are in typed_a1.h.
#pragma once
#include "typed_a1.h"
#include "typed_a2_rt.h"

namespace typed
{
#include "typed_a2.inc"
}

// the lifted functions whose slots the adapters test (A2_HOLDS / A2_SLOT)
namespace lift_d1
{
void CAM_init_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void CAM_reset_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_cur_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_get_pre_changed_status_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_reset_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_reset_status_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GAMEUTIL_set_auto_get_flag_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetCollisionFinderWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void GetSwordFlareHitPlaneAttribute(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void MotSpdCtrl_Reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2RemoteDmaTransStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void Sg2SlotSetOutputMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void _optionWorkChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void bgmStop(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void checkFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void gamecore_sub_01438448(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getGameHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void getPathColiGroupByType(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void get_view_angle_lws_anime_mode_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void is_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void isysInitGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void request_end_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFadeParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void setFullPower(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void vab_check_alive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
