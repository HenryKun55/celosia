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

**Passes.** `SOTC_S4DISP_CHECK=<n>` (s4_disp.cpp checkCall): every n-th call of each pass runs the *original* with the
guest RAM (32 MB) snapshotted at entry and the renderer's emissions counted (`g_s4Emits`: draw jobs, 2D writes, zFog's
hook, the bloom flag, glEnd / screenBlur / mesh / shadow / particle hooks) and its host state hashed (layer mirror,
requested texture, camera note, bloom flag, fog). After the call every changed word is classified:

- the stack below the entry sp (16 KB: its frames and its callees');
- packet memory: the current display-list buffer from its start (0x2132D8 +0 / +4 by buffer) to its next free word
  (+0x10, read after the call): the new packets and the link words written into earlier ones; the layer packets
  (+8 / +0xC, count +0x14); gsReg's state; the layer stack (0x1E10B0, 0x213308); the list record itself;
- list bookkeeping: a word whose new value points into the call's new packets (the root draw context's per-layer
  "last packet" words, two arrays 0xB0 bytes apart from 0x1657E90 on these runs: a layer the pass alone uses keeps its
  earlier value, cleared at the buffer's reset, as in the frames where the pass does not run);
- a word the variant writes: compared with the value the variant computes from the entry state;
- anything else: **unknown** (a write the variant would miss).

Host side: the original's emissions must equal the variant's own (zFog: its hook, 1; bloom: one or more flag sets), and
the host digest after the original must equal the digest after the variant's notes.

The first check builds found what each variant must keep: p1 listed the shadow handshake 0x118C080 (shadowReset sets it,
shadowDraw clears it) and words at 0x122xxxx / 0x12Axxxx / 0x1657Exx; p2-p3's value dumps placed those as the two
display-list buffers (0x122B8C0 / 0x12A80F0) and the per-layer last-packet words, and showed the bloom's eased values
(0x118C12C .. 0x118C14C, 68 -> 67 ...) as real state the renderer reads; the haze copy 0x10F37C8 left two words at
0x1658004 (values 0x463820 -> 0x41efc0, not packet pointers) S4 could not place: it is not replaced. Shipped (p4 / p5),
`SOTC_S4DISP_CHECK=150`, 720 s each:

| run | route | pass calls checked (incomplete: a thread switch inside) | unknown words | effects differ | host state differ | emissions differ |
|---|---|---|---|---|---|---|
| chkAp4 | attract demo | 573 (0) | 0 | 0 | 0 | 0 |
| chkLp4 | Avion's lake (teleport) | 377 (1) | 0 | 0 | 0 | 0 |
| chkRp4 | the ride (console card) | 347 (1) | 0 | 0 | 0 | 0 |
| chkRp5 (shipped) | the ride | 414 (2) | 0 | 0 | 0 | 0 |
| chkLp5 (shipped) | Avion's lake | 471 (3) | 0 | 0 | 0 | 0 |

(Per pass on the attract run: reflectionBegin 81, zFog 15, shadowDraw 83, shadowReset 81, bgEnd 83, depthOfField 81,
waterRefractionBegin 81, bloomDisplay 67, the bloom's second part 1; words classified: stack ~39 000, packet ~10 600, list
bookkeeping ~435. p1's longer runs (CHECK=150, the attract demo and the lake, 13 200 / 9 000 frames) had 0 host-state and
0 emission differences over ~1 100 checked calls before the dead ranges were placed.)

**Tolerated difference.** The display list sent to the (idle) GS path is shorter: it has no packets from these passes, and
the per-layer last-packet words keep their earlier value. The DMA thread's walk still finds the frame's FINISH (in the
list's end packet, untouched); the game paced as before in every run (no hang, the [frame] counts of on and off frames
the same within 1%).

**Hooks.** The lean hooks are the old hooks' default paths with the switches read once (section 3b); no check beyond
the runs themselves (same draws on and off: draws on 1107 / off 1107 a frame in mixA7p4).

**Palettes.** With the check mode on, every elidable chain is read at the kick anyway (and used, as before), and compared
with the worker's kept read: chkAp4 57 854, chkLp4 34 445, chkRp4 54 679, and on the shipped p5 (the read moved, not copied) chkRp5 64 460, chkLp5 43 467 compared, **0 differ** (p1's runs: 57 555 and
46 827, 0 differ).

## 5. Speed (Vita3K 222 MHz, interleaved A/B)

`SOTC_AB_FLAG=s4disp SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (s4/abs4.sh): all of S4 on or off at random each frame, 1 260 s
a run (from frame 3 000), own trees ../vita3k-celosia-s4 / -s4b. Frame times: tools/mixframes.py (`--shift -1`, 300-frame
blocks, +- 2 SE), now also the frame log's render-thread and hook-job columns. Sampler split (s4/tools/s4groups.py): NEW =
S4's code (s4disp::, the lean hooks), OLD = the code only the off state runs (the ten passes' own code, gsRegCensus,
spriteNoTexFast, the old hooks, kickCopy), samples per 1 000 frames of each state, Poisson z.

| run (build, seed) | route | frames on / off | sampler NEW / OLD, z | saved (sampler) | **frame off - on** | game busy off - on | render thread off - on | hook jobs off - on |
|---|---|---|---|---|---|---|---|---|
| mixA7p4 (p4, 7) | attract | 14 298 / 14 351 | 17.1%, z 240 | 0.543 ms | +0.543 +- 0.131 | +0.805 +- 0.104 | +0.039 +- 0.128 | +0.003 +- 0.051 |
| mixA11p4 (p4, 11) | attract | 13 194 / 13 351 | 17.5%, z 232 | 0.551 ms | +0.513 +- 0.151 | +0.720 +- 0.128 | +0.041 +- 0.094 | -0.008 +- 0.051 |
| mixL7p4 (p4, 7) | lake | 8 422 / 8 386 | 22.6%, z 203 | 0.693 ms | +0.945 +- 0.128 | +0.932 +- 0.127 | -0.370 +- 0.253 | -0.115 +- 0.048 |
| mixL11p4 (p4, 11) | lake | 9 395 / 9 431 | 22.4%, z 212 | 0.709 ms | +1.208 +- 0.298 | +1.137 +- 0.282 | -0.443 +- 0.266 | +0.009 +- 0.108 |
| **mixA7p5 (shipped, 7)** | attract | 16 505 / 16 495 (sampler) | **17.0%, z 272** | 0.569 ms | **+0.504 +- 0.199** | +0.753 +- 0.169 | -0.024 +- 0.111 | +0.014 +- 0.072 |
| **mixA11p5 (shipped, 11)** | attract | 18 286 / 18 714 (sampler) | **18.0%, z 283** | 0.555 ms | **+0.693 +- 0.128** | +0.867 +- 0.106 | -0.007 +- 0.091 | +0.039 +- 0.060 |
| **mixL7p5 (shipped, 7)** | lake | 9 958 / 10 042 (sampler) | **22.4%, z 219** | 0.679 ms | **+1.173 +- 0.525** | +1.169 +- 0.535 | -0.431 +- 0.251 | -0.018 +- 0.101 |
| **mixL11p5 (shipped, 11)** | lake | 10 466 / 10 534 (sampler) | **22.3%, z 222** | 0.666 ms | **+0.885 +- 0.475** | +0.877 +- 0.470 | -0.279 +- 0.186 | -0.087 +- 0.162 |

(p5 = p4 + the palette swap: p4's lake seed 7 showed the worker +0.115 ms a frame copying palettes; p5's lake runs show
-0.018 / -0.087, within noise. mixL11p4 ran through a ~80-minute host stall (the Mac asleep): frames >= 200 ms are left
out, the blocks around it are fewer.)

**Decision (z > 2 in both seeds, sampler and frame time):** `SOTC_S4DISP` on. Frame time -0.50..-0.69 ms on the attract
demo (z 5.1-10.8) and -0.89..-1.21 ms at the lake (z 3.7-14.8): 2.0-2.6% of the frame. The sampler's NEW / OLD sets cover
the passes' and hooks' own code (0.55-0.71 ms a frame); the frame times include their callees (gsReg, spriteNoTexFast,
callGuest, the layer natives) and the kick's reads.

**Per-thread cost before / after** (frame log means, the shipped runs, on vs off): game thread busy 25.30 vs 26.04 /
22.58 vs 23.42 ms (attract), 45.35 vs 46.45 / 45.51 vs 46.34 ms (lake); render thread 14.45 vs 14.41 / 13.13 vs 13.11
(attract), 17.44 vs 17.02 / 17.45 vs 17.22 (lake); hook jobs 5.33 vs 5.36 / 4.85 vs 4.87 (attract), 10.98 vs 10.91 /
11.04 vs 10.97 (lake). The render thread's slightly higher figure at the lake with S4 on (0.28-0.44 ms, z 1.5-3.0 over
the four lake runs) is discussed in section 6.

## 6. Per-core load and another core

From the sampler (guest instructions / (222 MHz x seconds), both states mixed) in the A/Bs: attract mixA7p4 game 57.4%,
render thread 40.4%, hook worker 15.6%, mds lane 3.6%, VIF walker 3.1%, texture animation 1.9%; lake mixL7p4 game
66.1%, render 30.9%, worker 17.4%, VIF walker 3.3%, lane 2.7%, pf141 1.4%. By state, from the frame log (section 5):
the game thread's busy time falls 0.7-1.1 ms a frame with S4 on, the render thread's own time does not fall (attract
+0.04 ms, lake -0.37 / -0.44 ms: slightly *more* render time a frame with S4 on at the lake, z 1.7-2.9; S4 changes no
draw record (section 4) and the draws a frame are the same, so this is the render thread's share of shorter game frames
in Vita3K's scheduling (guess), and p4's hook worker +0.115 ms a frame on the lake (the palette copies it then made;
p5 swaps instead of copying).

**Part 4 (draw prep off the game thread): not done.** What S4 removed is work, not moved work: the passes' variants are a
few guest words each (nothing to hand over), the lean hooks are cheaper in place, and the palette part already moved the
kick's read to the worker. The per-object host-side draw prep that remains on the game thread is the model hook's half
(round 23 / 61 already put the draw building on the worker and modelDispState on the lane: what is left is the copies
the jobs need, which must happen at the hook because the game changes them after it, display_map.md 5d) and the walk's
natives, which write guest state the next frame reads. Cores 1-2 on the console were 70-85% busy (round 89) and the
render thread (core 2) is the console's critical path in heavy frames: moving more onto them is not clean, so S4 leaves
the split as it is.

## 7. Pictures

(filled in below)

## 8. What S4 leaves for the next round

- **Static guards in hot code** (section 2): modelDispHook (19 `dmb` a call, ~50-100 calls a frame), mdslane::join (11),
  obj125Run (30), dispDispObjGroupModel (5), the particle emitter (6): the same fix as the lean hooks (switches read once
  into plain globals at install), worth a sweep over every function the sampler shows (s4/dmbscan.py).
- **The hook's own palette read** (readChainPaletteLean at the model hook, 0.26-0.38% of the game thread): dispSkinned
  could write the palette into a host arena too and the hook take that (with the chain's UNPACK layout cached and
  validated); S4 only removed the kick's second read.
- **The rest of the gsReg / 2D path**: the 2D (k2D) writes still go through gsReg A+D pairs that the hook parses back
  into the 2D assembler (feed2D / vertex2D, 0.6-2.7%); the sprite functions could hand their quads to the renderer
  directly. The haze screen copy (0x10F37C8) needs its two unplaced words understood first.
- **The display list itself**: with the passes gone, the list's remaining packets are the 2D, shadows and the models'
  leftovers; a host FINISH without the DMA walk (display_map.md 5e step 2) needs every remaining producer proven
  event-free.
- **The render thread** is unchanged by S4 (its cost is per draw, round 116); the console's GPU-bound heavy frames are not
  touched by any game-thread saving.
