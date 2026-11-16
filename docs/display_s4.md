# S4: display / draw prep without the PS2 packets nobody reads (2026-11-16)

Round S4 redesigns the game side of the draw path for the Vita. With `SOTC_NATIVE=1` (always on the Vita) the GS
rasterises nothing and no VU1 program runs (docs/display_map.md 5a): the native renderer draws from host records its
hooks make, and redoes every screen effect as its own pass from guest globals it reads at endFrame. Yet the game still
built PS2 packets whose only reader was the emulated GS / VU1, and host code read guest packets back. S4 removes three
pieces of that, all behind `SOTC_S4DISP` (default on; A/B name `s4disp`), each with its own knob:

| part | knob (default) | what changed | where |
|---|---|---|---|
| **passes** | `SOTC_S4DISP_PASSES=<mask>` (all) | Ten screen passes of graphFrameFinish / the walk (the water reflection set-up, zFog with the background blend, the shadow reset and reduce, bgEnd, the depth of field, the refraction copy, the bloom chain and its second part, the solar flare reset) replaced, while the native renderer draws, by host-only variants: the guest words the rest of the game reads (the ripple clock, the shadow handshake, the bloom's eased values) and the host notes (camera context, bloom flag, zFog's parameters) without a single GS packet. | s4_disp.cpp |
| **hooks** | `SOTC_S4DISP_HOOKS` (1) | gsReg, iosDlPushLayer and iosDlPopLayer hooks without their per-call debugging-switch reads: each function-local `static const` costs a load and a `dmb ish` per call on the A9. | sotc_native.cpp (gsRegLean, pushLayerLean, popLayerLean) |
| **palette** | `SOTC_S4DISP_PALETTE` (1) | The kick no longer reads back the skinned palettes dispSkinned has not written since their hook read them: dispSkinned counts its writes per chain, the worker keeps the hook's copy. | s4_palette.cpp, decomp_skinpal.cpp, sotc_native.cpp (kickCopy / kickRun / runJob / the hook) |

Check mode `SOTC_S4DISP_CHECK=<n>` (section 4) runs the original every n-th call of each pass with a snapshot of guest
RAM and classifies every word it wrote; the palette part reads at the kick anyway and compares.

## 1. Semantic layer: disp_types.h

`game/overrides/disp_types.h` names the display's guest records as the native path uses them, as views on guest memory
(may_alias members, static_asserts), like coll_types.h / chara_types.h: DMA tags and the VIF codes the instance chains use
(`disp::dma`, `disp::vif`: the V4-32 UNPACK test readChainPalette makes), the model header (+0x14 load packets, +0x18
flags, +0x1C LOD count, +0x30 part tables, +0x40 textures, +0x50 surfaces, +0x60 draw packets), the part node (+0x2C the
clip's visibility), the surface (0x120 bytes: flags, texture indices, the GS register packet at +0x60, the second pass
at +0xC0, alpha at +0x10C), the draw packet (8 words), the instance (+0xC the chain, +0x244 / +0x248 fades, +0x250 the
LOD word with `lodLevel` the signed read D11 fixed, +0x260 the draw flags), the skinned palette layout
(`disp::skinned`: object +0x144 -> instance, +0xC -> the chain, +0x104 -> the palette; bone i at
((4i + ((i + 1) >> 6)) << 4)), the instance lights, the display list record (0x2132D8), its layer packets, gsReg's
packet state (*0x1E11A4), the NTO2 header, the cloth block and the camera context offsets. S4 found that the instance
the walk passes to modelDisp is object +0x144 and that its chain is the "model" dispSkinned writes through: the hook's
`own` is dispSkinned's model (the key s4_palette uses).

## 2. The profile first (Vita3K 222 MHz, base = celosia-d11.vpk)

Sampler runs of 600 s (snapshots 120-540 s), no A/B: tools/s4rank.py groups the game thread's symbols.

| route | game thread | display area (decomp_tree) |
|---|---|---|
| attract (profA0) | 20.70 ms a frame, 36.6 fps | 20.7% (4.28 ms) |
| lake (profL0, Avion's lake by teleport) | 40.85 ms, 20.0 fps | 21.8% (8.91 ms) |
| ride (profR0, the console card's ride: the shrine) | 44.67 ms, 18.1 fps | 23.2% (10.34 ms) |

Draw prep by item (share of the game thread, ms a frame):

| item | attract | lake | ride |
|---|---|---|---|
| walk / clip / sort / light natives | 10.13% 2.10 | 10.05% 4.10 | 11.53% 5.14 |
| model hook + mds lane (game-thread side) | 3.27% 0.68 | 3.53% 1.44 | 4.33% 1.93 |
| 2D assembler (feed2D, vertex2D, emit2D, sprite family, immediate quads) | 2.69% 0.56 | 1.01% 0.41 | 0.18% 0.08 |
| textures (requests, reset, animation, VRAM) | 2.04% 0.42 | 1.91% 0.78 | 1.83% 0.82 |
| **gsReg packets** (gsReg, its hook and census, gsRegBegin / End, spriteNoTexFast, the spriteTexFast hook) | 1.73% 0.36 | 1.63% 0.67 | 1.04% 0.46 |
| **screen passes' own code** | 1.57% 0.33 | 0.85% 0.35 | 0.76% 0.34 |
| **layer stack** (push / pop hooks + natives) | 0.77% 0.16 | 0.56% 0.23 | 0.53% 0.24 |
| **palettes read back** (hook + kick) | 0.41% 0.09 | 0.48% 0.20 | 0.63% 0.28 |
| callGuest dispatch (all callers) | 0.26% | 0.28% | 0.33% |

(Bold: what S4 cuts into. The walk / clip / light group is the game's own logic, already native and exact from
rounds 7-140; the model hook is round 8 / 23 / 61's host side.)

Per thread (share of one core at the throttle): game 75.6 / 81.7 / 80.6%, render thread 54.8 / 39.2 / 60.5%, hook worker
23.1 / 19.7 / 28.8% (modelDispDraw 6.5, shadowDispDraw 6.5, surfaceInfoFull 1.0 on attract), VIF walker 4.5 / 4.1 /
4.2%, mds lane 3.6 / 2.7 / 3.6%, texture animation 2.6 / 1.7 / 1.6%, pf141 1.5 / 1.5 / 1.9%, clip140 0.9 / 0.9 / 1.0%. The
render thread's top symbols are vitaGL and sceGxm (sceGxmDraw 13%, memcpy 3.4%, state setters ~1% each): it no longer
translates guest packets (round 8-116 moved that to the hooks and the worker); its cost is per draw (round 116).

**The A9 finding: function-local statics.** gsRegCensus, a function that only tests an environment switch, was 0.13-0.15%
of the game thread. GCC's guard for a function-local `static const` is a load and a `dmb ish` on every call on ARMv7
(`ldr; dmb ish; lsls; bpl`), and gsRegCensus also built its full prologue (nine registers, 84 bytes of frame) before the
test. In celosia-d11.elf: gsRegHook 5 dmb, pushLayerHook 7, popLayerHook 1, gsRegCensus 2, modelDispHook 19,
mdslane::join 11, obj125Run 30 (s4/dmbscan.py lists the dmb count of the hottest symbols). The `hooks` part removes them
from the three packet-level hooks (hundreds of calls a frame); the rest is left to a later round.

## 3. The redesign

### 3a. Screen passes (part "passes")

Every pass below runs once a frame (or per scene) under graphFrameFinish or the walk, builds its GS register packets with
gsRegBegin / gsReg / gsRegEnd, spriteNoTexFast / spriteTexFast and layer pushes, ~20-140 guest calls through the function
table, each through its hook. With the native renderer drawing, none of those packets has a reader (the GS rasterises
nothing; the DMA thread only looks for the frame's FINISH, which is in the list's end packet, not in these), and the
hooks take nothing from them (no k2D / feedback / refraction range lies in them; spriteTexFast's hook returns at once).
What the rest of the game and the renderer do read, found by the check (section 4) and kept by each variant:

| pass (address) | calls / frame (attract, lake) | the variant keeps |
|---|---|---|
| reflectionBegin 0x109AD58 (native, round 43) | 0.96, 0.90 | the ripple clock 0x118C030 += f12 x 0.1 (the renderer's water ripple) |
| zFog 0x10AC0A8 (+ bgDraw it calls) | 0.22, 0.92 | its hook (zFog's parameters into the renderer's fog) without the guest zFog; the camera note |
| shadowDraw 0x109D320 | 0.99, 0.99 | the shadow handshake: 0x118C080 cleared when set; the camera note |
| shadowReset 0x109D158 | 0.96, 0.90 | 0x118C080 = 1 when shadows are on (0x118C07C) |
| bgEnd 0x1084DE8 | 0.99, 0.99 | nothing (packets only) |
| depthOfField 0x10874E8 | 0.96, 0.90 | the camera note |
| waterRefractionBegin 0x109C108 | 0.96, 0.90 | nothing |
| bloomDisplay 0x109E2A0 (native, round 48) | 0.82, 0.96 | its six eased values (0x118C140 / 144 / 148 / 14C / 13C / 12C toward 0x118C0A8 / AC / B0 / A4 / A0 / 9C, (t - v) / 12, at least one step: the renderer's bloom), the bloom flag |
| manager_sub_0109FB08 (the bloom's second part) | title only | nothing |
| solarFlareReset 0x10A1518 | rare | nothing |

Not replaced: anything whose writes the renderer reads through a hook (the 2D in k2D, feedbackBlur, refractionEnd,
bossDeathEffectBegin's fan through glEnd, screenBlur / cameraBlur through the screenBlur hook, the shadow volumes at
shadowDisp's pops), and the haze's screen copy 0x10F37C8 (it writes two words of a structure S4 could not place; left as
it was). Tolerated difference (section 4): the display list is shorter (the passes' packets are not in it) and the
draw context's per-layer "last packet" words keep their earlier value for the layers these passes alone use, as in the
frames where a pass does not run (zFog runs in ~22% of the attract demo's frames).

### 3b. Packet-level hooks without per-call env reads (part "hooks")

gsRegLean / pushLayerLean / popLayerLean do what gsRegHook / pushLayerHook / popLayerHook do for the default switches
(the bloom flag, feedbackBlur / refractionEnd through the old hook, the k2D feed, the D11 layer mirror, the shadow capture
at shadowDisp's pops), with the switches read once into a plain struct at the first call with S4 on. Any of the hooks'
debugging switches set (SOTC_LOG_GSREG, SOTC_LOG_LAYER, SOTC_FORCE_UNDERWATER / FEEDBACK / BOSSFAN, SOTC_LAYER_MIRROR=0)
keeps the old hooks. Same work by construction.

### 3c. Palettes at the kick (part "palette")

Round 34's kick copies, on the game thread, every chain's palette its hooks saw (kickCopy -> readChainPaletteLean), so
the worker's kick pass can repose the draws of a chain the game rewrote after their hook (a chain shared by several
instances). Now dispSkinned (decomp_skinpal.cpp, the only writer of a skinned palette) bumps a per-chain serial
(s4disp::paletteWritten, keyed by the chain address = instance +0xC); the hook notes the serial it read
(paletteRead); at the kick a chain dispSkinned wrote this frame and has not written since its last hook read is not read
again (k.fromLast): the worker, which runs the jobs in order, kept a copy of each model job's palette (paletteDrawn, in
runJob before the lane's out-of-view skip) and the kick's pass takes that copy. Rigid chains (no dispSkinned write this
frame) and chains written after their last hook read are read at the kick as before.

## 4. Equivalence

(filled in below)

## 5. Speed (Vita3K 222 MHz, interleaved A/B)

(filled in below)

## 6. Per-core load and another core

(filled in below)

## 7. Pictures

(filled in below)

## 8. What S4 leaves for the next round

(filled in below)
