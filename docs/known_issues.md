# Known issues

## Raised sword: the arm sweeps back and forth (emulated and native) — gone

**2026-10-19: no longer seen** (reported by the user while playing natively).
The fix is not pinned down. Likely candidates, both in the game code's
execution rather than a renderer: the 841 hidden entry points (c81db86,
code packed after a jump and called through pointers) and the exact SDK
soft-double (decomp_softfloat.cpp: truncation, not IEEE rounding). The notes
below are kept for reference.


Seen 2026-10-15 in the shrine and on the plain: with the sword raised and the
stick untouched, Wander's arm swings widely within a second (10 screenshots
in one second all differ). Same in `./celosia run` and with SOTC_NATIVE=1, so
it is in the recompiled game code or the runtime, not in a renderer.

Ruled out so far:
- Stick drift: a radial dead zone (PS2X_STICK_DEADZONE, f683eae) did not
  change it.
- GAMEUTIL_get_yaw_pitch_by_direction (0x10816c0): checked against atan2 on
  15,000 calls, no mismatch (SOTC_MATH_CHECK=1).
- The aim interpolation's time step: doit_aim_angle_interp_cntrl (0x10848b8)
  gets dt ~ 0.033 (1/30 s), rate 4, max 1.57 rad: a stable step
  (step = -diff x rate x dt, 0.13 of the gap per frame).

Not yet checked:
- the aiming IK (nico_IK_targetAiming 0x13758d8, GetTargetLookAtQuaternionOnBase
  0x1134bd0: quaternions with VOPMULA/VOPMSUB, VMR32, VRSQRT in VU0 macro mode);
- the sight controller (doit_nico_sight_on 0x13a9508) and what it aims at
  (the light beam seeks the colossus: its target may jump);
- the VU0 macro translations in ps2xRecomp (vu_translation_helpers.cpp)
  against an interpreter on the same inputs.

Note: a hook around a recompiled function that calls others sees its result
too early (the function runs in pieces the dispatcher continues), so the
SOTC_MATH_CHECK results for sinf/cosf/atanf/acos* are not evidence.

## Native: brighter than the emulated picture, most in the opening

Measured 2026-10-15 at the same game frame: emulated / native mean
brightness about 0.68 in the opening, 0.84 in the shrine, 0.95 on the plain.
In the shrine it looks like an offset (native mid tones about 16 higher,
equal near white); in the opening like a factor. Ruled out (each switched off
on its side, the gap stayed): vertex fog, bloom, depth of field, the game's
screenBlur and shadows, the display merge (PMODE 0x8023 blends the frame with
itself). The opening's models are mostly unlit (vertex colour x texture).
Also matching in the opening: the fog colour (FOGCOL 30 40 45, the same
in the GS and natively) and F per vertex (z 3650 -> w 245 -> F 132.6
natively, 132 in the GS stream); switching the native lighting off changes
nothing; the unlit models' instance factor (VU 991) is 1. Not yet checked:
the GS texture function and CLUT path against the native decode on one
surface, per draw.

### Findings of 2026-10-16 (paired route, tools/native_run.sh)

- Native about 15 brighter in mean luminance through the route; with the
  native bloom off (SOTC_BLOOM_SCALE=0) the gap on the plain goes to ~0
  (from +20) and drops from ~15 to ~11 at the pillars: most of it is the
  bloom (see below). The shrine cutscenes kept +17 with the bloom off: the
  bossDeathEffect composite, native since 2026-10-16 (layer 8 drawn apart,
  as the GS does; the cutscene now matches).
- Ruled out by measurement: textures (the GS's decoded texture and the
  native one are byte-identical, PS2X_GS_TEXDUMP / SOTC_TEXDUMP_DIR),
  vertex colours (the same VIF data on both paths), vertex fog (GS F = 255
  near the camera, native base + scale / w clamps to 255; FOGCOL is light
  here), coplanar layers (depth test now less-or-equal like the GS's
  GEQUAL: no change), zFog (the game's zFog skipped on both sides,
  SOTC_SKIP_ZFOG=1: no change).
- zFog is now native by the game's own algorithm (a pass at layer 4 over
  the picture: slice i at depth far - margin - width + width x i / N,
  alpha i x 128 / N, the background copy blended by it), on by default.
- PS2X_PROBE on the software GS (PS2X_GS_GPU=0): at a floor pixel the
  bloom sprites arrive with colour (0, 0, 0, 0): the emulated bloom adds
  nothing there, the native one does. Which one the PS2 does needs a real
  reference (PCSX2 with a BIOS, or the console).

## Native vs emulated: bloom over the sky and windows (reference doubtful)

On the new route (tools/native_run.sh) the native sky outdoors and the
shrine window blow out to white, while the emulated picture shows clouds
and the landscape through a soft glow. The game's own chain says the
native is right: the mask (Z <= 0x118c094 + 1) covers the sky also on the
PS2 (bgEnd clears Z to 1 after layer 1), the 12 shifted copies are a
normalized lerp, and the final add is colour 0x118c140 (0xaf/0xaf/0xa5
inside, 0x51/0x5e/0x5b outdoors) x alpha 0x80, which saturates a bright sky.
An earlier finding (memory: GS-exact blend gives a bright shrine window; the
software path sampled sprites half a texel off and dimmed the bloom) points
at the emulated GS here. Needs a real reference (PCSX2 software or the
console) before tuning either side.

## Native zFog (opt-in): layer 1 kept out of the main pass

bgEnd copies the main frame (layer 1 drawn into it) to the 256 x 256
background and then sets Z to 1 with a full-screen Z-only sprite: layer 1 is
in the main picture, sharp, and the zFog/bgDraw blend replaces it only by
the fog alpha. The native renderer, with SOTC_NATIVE_ZFOG=1, skips layer 1
in the main pass ("shown through the backdrop"), which leaves flat, blurred
patches where layer 1 surfaces come close to the camera (shrine floor near
the stairs). To fix before turning zFog on.

## Native: Wander's shadow on the plain, Agro up close

On the plain the emulated picture shows Wander's soft shadow on the ground;
the native one does not (seen 2026-10-16, with or without the glow and the
native system: older than those). Agro right next to the camera (route
picture 33, game frame ~2040) is missing natively: likely the near plane
(the GS does not clip at it).

