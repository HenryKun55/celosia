# Release configuration (no env file)

Round F1, 2026-11-27. What the game does when `ux0:data/celosia/env.txt` is absent or empty, which options the player has,
the escape hatches, and how to turn the debug output on. The per-switch record is `docs/switches.json` (field `f1`) and the
classification is `docs/switches.md`.

## With no env file

- **Every proven round is on by its code default.** That is the 278 class-A switches that were on already plus the 15 that
  round F1 turned on: `D1NATIVE`, `DT1`, `W1`, `W2`, `W3`, `X1`, `X2`, `X3`, `Y1`, `Y2`, `Y3`, `Z1`, `Z2`, `Z3`, `Z4`.
  Their sub-knobs are now constants with their built-in values: W1 all four parts, X3 part 2 (the batched wake-ups), Y1 / Z1
  parts 7, Z2 0x1F, Z3 0x7F, DT1's worker pinned to core 1 at priority 140 with the palette copied at the hook and no
  wake-ups from the hook, C2's scene prefetch level 2 (C2's scene is part of W1). `SOTC_NATIVE` is set to 1 by
  `ps2_vita_runtime.cpp` at start.
- **Nothing debug runs.** The debug / profiling / check switches (class D) default off. `[w1]`, `[x3]` and `[dt1]` lines
  are off by default since F1. In a release build (`PS2X_VITA_RELEASE=ON`) every class-D read goes through `ps2_diag::env`,
  which is a constant `nullptr` there, so none of them can be turned on and their code is compiled out. The boot-time
  profilers (`SOTC_K2`, `SOTC_K2REMAP_DRY`, `SOTC_K2_PATH`, `SOTC_C1CORE3_EXPECT`) are behind `!ps2_diag::kRelease`.
- **Test-only, unproven and dead paths are off.** Class C (`CHARLOD78`, `OBJ96`) and class F (A12-A14 typed stages, the B1-B10
  carrier rounds, C1 / K2REMAP / K3 / M1 / P1-P4 / R1 / DC2 / PTCL79 / G1 and the rest, now including K1, SEG80's
  every-caller reuse and CHAR104 on the Vita) stay off with no env. Round F1 deleted the class-E paths (see
  `switches.md`): they are no longer in the code or behind any variable.
- **Player options** come from `ux0:data/celosia/settings.cfg`, else from the menu defaults below.

## Menu options (class B)

Graphics page of the native renderer's settings (Select twice on the Vita) and the System tab of the host menu. Defaults
with no `settings.cfg`:

| option | default | variable (dev build only) |
|---|---|---|
| Preset | High (3) | `SOTC_PRESET` |
| Render scale | 1.0 | `SOTC_RENDER_SCALE` |
| Bloom (on / strength) | on, 1.0 | `SOTC_BLOOM_SCALE` |
| Bloom rounds / spread | 1 / 1 | `SOTC_BLOOM_ROUNDS`, `SOTC_BLOOM_SPREAD` |
| Glow mode | 1 | `SOTC_GLOW_VITA` |
| Glow, depth of field, shadows, particles, screen blur, feedback, reflections, refraction, heat haze, vertex fog | on | `SOTC_NATIVE_NO*` |
| Depth fog | on | `SOTC_NATIVE_ZFOG` |
| Fog amount | 1.0 | `SOTC_FOG_AMOUNT` |
| Body shadows | on | `SOTC_BODY_SHADOWS` |
| Alpha test | on | `SOTC_ALPHA_TEST` |
| Texture filter | 2 (trilinear) | `SOTC_TEXTURE_FILTER` |
| Model detail | 0 | `SOTC_MODEL_DETAIL` |
| Frame rate | Auto (`Steady 20` the other choice) | `SOTC_CAP20` |
| 30 fps cap | off | `SOTC_CAP30` |
| Performance cuts (camera collision, draw distance, far characters, effects, LOD, particles) and "Max performance" | off | `SOTC_SAC_*`, `SOTC_SACRIFICES` |
| CPU at 500 MHz (System tab) | off | `SOTC_CPU500` |
| MSAA, effect detail | 0 (Metal only, nothing on the Vita) | `SOTC_MSAA`, `SOTC_EFFECT_DETAIL` |

In a release build these variables are ignored: the menu and `settings.cfg` are the only source. In a dev build a variable
still overrides the saved value for one run (tests).

## Kill switches (escape hatches)

One variable per round that F1 turned on. `SOTC_<NAME>=0` in `env.txt` turns that round off and brings back the code that
ran before it. They are read with `std::getenv`, so they work in a release build too. They exist because 11 of the 15 rounds
never ran on the real console: their proof is Vita3K checks only (0 differences in each round's own check).

| kill switch | what goes back | ran on the console |
|---|---|---|
| `SOTC_D1NATIVE=0` | the hand natives' old bodies (D1-D3) | yes (A/B 2026-11-24, base of every console run since) |
| `SOTC_W1=0` | the display walk's guest calls, C2's scene list off | yes (run-20261126-wxon) |
| `SOTC_W2=0` | the character pipeline's math one bone / node at a time | yes (run-20261126-wxon) |
| `SOTC_W3=0` | the script objects' pass as guest code | yes (run-20261126-wxon) |
| `SOTC_X1=0` | ExecCharaGeometry's old body | no |
| `SOTC_X2=0` | the animal scripts / AI_OptFloatFrameSample as guest code | no |
| `SOTC_X3=0` | the display walk's draw side as before X3 | no |
| `SOTC_Y1=0` | the small animals' AI library as guest code | no |
| `SOTC_Y2=0` | Wander's mantle solver as guest code | no |
| `SOTC_Y3=0` | the object animation library as guest code | no |
| `SOTC_Z1=0` | the characters' collision library as guest code | no |
| `SOTC_Z2=0` | the cloth driver / fringe as guest code | no |
| `SOTC_Z3=0` | the root-dynamics helpers as guest code | no |
| `SOTC_Z4=0` | the frame change's renderer bookkeeping as before | no |
| `SOTC_DT1=0` | modelDisp's host part on the game thread (no core-1 worker) | no |

The switches are independent in the code. X1, W2 and X3 were only ever checked with D1NATIVE on (W2 acts inside D1's bodies):
if D1NATIVE has to go, turn X1, W2 and X3 off with it. Round F1 checked the all-off set (every kill switch `=0`) in Vita3K.

The older class-A rounds keep their own `SOTC_<NAME>=0` in the switch table (`sotc_decomp.h`), as before F1.

## Debug output (dev builds)

A dev build (`PS2X_VITA_RELEASE=OFF`, what the scratch scripts build) reads every class-D variable from `env.txt` as before:

- frame summary: `SOTC_FRAME_LOG=1` (`[frame]` lines), `SOTC_FRAME_SLOW_MS=<ms>`;
- a round's own check / log: `SOTC_<ROUND>_CHECK=1`, `SOTC_<ROUND>_LOG=<frames>` (for example `SOTC_W1_LOG=1`,
  `SOTC_X3_LOG=1`, `SOTC_DT1_LOG=600`, which F1 turned off by default);
- profilers: `SOTC_P2BRACKET=1`, `SOTC_PROF129=1`, `SOTC_K2=1` + `SOTC_M1PMU=<n>` (console), `SOTC_SUBSYS_PROF=1`;
- recorder / replay / route aids: `SOTC_INPUT_*`, `SOTC_REC_*`, `SOTC_TELEPORT`, `SOTC_RIDE_STEER`, `SOTC_VITA_EAT_USER_MB`.

A release build (`cmake -DPS2X_VITA_RELEASE=ON`) has none of these: `ps2_diag::env` is a constant `nullptr` there.
