# S12: the immediate quads as host vertex records, not GS A+D pairs (2026-11-18)

Round S12's brief: S10's section 6, the display steps still shaped for the GS / VIF / DMA, biggest first, each as a
part of one switch. Measured again on S10's build (riding), the largest of them on the game thread is the immediate
quads: the heat-haze strips behind Wander, Agro and the colossi (decomp_walk::strips, two quads a segment) and
01142350's quads, which S10's table counted as "immediate quads / 2D" plus the strips under "Other game code".

Switch: `SOTC_S12DRAW` (A/B name `s12draw`; sotc_decomp switch `kS12Draw`, the last of the tables, off by default: on with `SOTC_S12DRAW=1`, see
section 4). Parts: `SOTC_S12DRAW_PARTS=<mask>` (bit 0 `strips`, bit 1 `reuse`; default 3). Check mode
`SOTC_S12DRAW_CHECK=1`, counts `SOTC_S12DRAW_LOG=<frames>` (`[s12draw]` lines, default every 600 frames). Code:
game/overrides/s12_draw.h (the record, Quad12), decomp_walk.cpp (quad12Body, imm12, prim128's check, clipFlags12),
sotc_native.cpp (s12Feed, s12Record, namespace `s12draw`: config, counts). Scripts, runs and logs:
build/agent-scratch/s12/ (q*.sh the queues, abs12.sh the A/B, chk12.sh the check, chain_ab.sh, vq.out / chain.log).

## 1. The profile (ride7, Vita3K 222 MHz, base = celosia-s10)

profB0 (base celosia-s10, ride7 120-840 s, 18 593 frames, game thread 29.55 ms a frame), self time of the game thread's
symbols, x 29.55 ms. "Display and models" is 23.7% (7.0 ms); the strips are counted under "Other game code".

| step | share | ms a frame | tied to the PS2 by |
|---|---:|---:|---|
| immediate quads: quad117BodyA9 1.79, vertex2D 0.36, immFeed128 0.27, glColor4f 0.21, feed2D 0.08, emit2D 0.07, imm128 0.07, prim128 0.03 | 2.88% | 0.85 | per vertex the gl layer's colour / ST / ring slot / state words written to the gl block and read back, then the vertex as GS A+D pairs (ST, RGBAQ, XYZF2 / XYZF3) parsed back by feed2D's register switch |
| the strips' own segment math (strips, its lambdas and their long-branch veneers) | 0.85% | 0.25 | (the game's own logic over guest data) |
| model hook and its reads (modelDispHook 1.49, readChainPaletteLean 0.42, readChainPalette 0.19, queueNextDraw 0.24, hookScrolls127 0.21, planChecked 0.18, meshDispHook 0.17) | 2.90% | 0.86 | the palette parsed out of the instance chain's VIF UNPACKs; the plans checked against the guest packet lists |
| glVertexKick (other gl callers) | 0.56% | 0.16 | the guest gl ring and GIF packet |
| lights (lightPass 0.71, getAmbientColorC 0.52, execGlobalLightMan 0.37, objectLight 0.29, calcLightLayoutObj 0.27, lightMatrices 0.16, setWorkImpl 0.13) | 2.45% | 0.72 | objectLight's camera-space lights written into the VU1 packet |
| texture lists (resetFast 0.32, texResetFast 0.15) | 0.46% | 0.14 | the GS transfer state of the texture records |
| layer stack / gsReg (gsRegLean 0.14, popLayerLean 0.14, pushLayerLean 0.11, leaves' push / pop 0.20) | 0.59% | 0.17 | the display list's layers and GS register writes |

The modelDispState lane after S10: the game thread's joins ran 12-20 ms of lane jobs per 600 frames riding (bootRp3
of S10: ~0.03 ms a frame); the lane itself is ~6% of another core. Its mirror (S10's step 1) costs the game thread
almost nothing in Vita3K any more, so the quads came first.

## 2. What changed (`SOTC_S12DRAW`)

### Part `strips` (bit 0): one host pass per quad, its record straight to the 2D assembler

Before (rounds 117 / 128 / 140): prim128 -> imm128: glBegin's words (begin128), quad117 (per vertex glColor4f writing
the gl block's colour words and clamping them there, glTexCoord2f's four words, glVertex3fv's state read-modify-writes
and the ring slot's colour / ST / position written, then the kick reading them back, three VU0 products, Q, the CLIP
history, the primitive's decision, the Z and fog, and the result as GS A+D pairs into a host array), then immFeed128
fed the pairs one by one through feed2D's register switch into the 2D assembler (vertex2D).

Now (imm12 / quad12Body): the same operations in the same order on values in registers: the colour clamp of
glColor4f, the ST, the state words, the pending count, the Z scale and fog byte read once; the ring slots written as
they are made (glEnd's software clip and the next gl call read them), the gl block's words (colour, ST, state, pending
count, ring index, ADC bytes) written once at the end with quad117's final values; the VU0 registers, Q, f0 - f2 and
the GPRs left as the fourth kick leaves them. The output is a record (Quad12: the PRIM value, per vertex the ST / UV,
RGBAQ and XYZF2 / XYZF3 words and which of them) that s12Feed hands to the 2D assembler as feed2D would (the layer, the
screen-copy note for the haze, PRIM's strip clear, vertex2D per vertex), without the pairs and the switch. The CLIP
judgement is computed on NEON (clipFlags12: ps2VuClipFlags' integer compares, max(|w| bits, 0x7fffff) as the limit,
without its store and lane loads).

### Part `reuse` (bit 1): a segment's two shared vertices transformed once

A strip segment's second quad starts with the first quad's last two vertices (sp+0x120, sp+0x130). The kick's three
products depend on the position and the three matrices only (pure VU0 arithmetic), so when the matrices are the same
bits as the quad before's and the position too, its products (view, screen, clip, the accumulator) are taken from the
quad before (two buffers, game thread only). Riding, 33% of the vertices are taken this way (chkRp2: 753 660 of
2 277 124).

## 3. Equivalence

`SOTC_S12DRAW_CHECK=1` (chk12.sh): every quad both ways from the same state (prim128): the new way dry (its record,
the gl block it leaves, VU0 / Q / CLIP / f0 - f2), the gl block, the context and the CLIP record put back, then the old
way for the picture (imm128, else the calls), the old feed's record taken at immFeed128 (or glEndHook). Compared: the 2D
assembler's input (the layer, the texture header, every (value, register) pair the record stands for), the whole gl
block (0x5300 bytes but the old packet pointers +4 / +8), VU0's registers, ACC, Q, the CLIP register, f0 - f2. With a
clip flag set (glEnd's software clip, 0108CD80, runs in the old way only) the record alone.

| run | build | route | frames | quads checked | clipped | vertices (reused) | differ |
|---|---|---|---:|---:|---:|---:|---:|
| chkRp1 | p1 (strips) | ride7 (900 s) | 18 198 | 610 465 | 0 | | **0** |
| chkRp2 | p2 (+ reuse) | ride7 (900 s) | 17 718 | 569 281 | 0 | 2 277 124 (753 660) | **0** |
| chkRp4 | p4 (shipped code) | ride7 (900 s) | 18 858 | 632 231 | 0 | 2 528 924 (839 352) | **0** |
| chkLp4 | p4 | lake6 (700 s) | 14 298 | 173 695 | 0 | 694 780 (217 577) | **0** |
| chkAp4 | p4 | attract (600 s) | 17 418 | 366 733 | 27 | 1 466 932 (450 474) | **0** |

p4's three routes: 1 172 659 quads, 0 differences. No quad was declined (every quad the old path took, the new one took).
spalign: 968 / 964 NEON :128 / :256 accesses, 0 sp-based (p1-p5). No function-table wrapper: prim128 is reached from
decomp_walk's own natives (strips, 01142350's quads).

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s12draw SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

abs12.sh, 800 s a run; frames from the mount + 600 (ride7), from frame 3 000 otherwise; tools/mixframes.py (300-frame
blocks, +- 2 SE). Positive = on is faster. "Sampler" = s4groups' NEW (quad12Body, s12Feed, imm12: code only the on
state runs) per on-frame against OLD (quad117BodyA9, immFeed128, imm128: only the off state) per off-frame. (abs12.sh's
first OLD pattern also took feed2D and glColor4f, which other 2D / gl callers run in both states: it read 0.22-0.32 ms
"saved" in every run, an artefact; the table has the corrected patterns.)

| run (build, seed) | route (window) | frames | **frame off - on** | game busy off - on | sampler NEW / OLD (z) |
|---|---|---:|---|---|---|
| mixR7p2 (p2, 7) | ride7 (mount 9 992 + 600) | 6 098 | **-0.374 +- 0.324** | -0.521 +- 0.380 | (flattened: slower) |
| mixR7p3 (p3, 7) | ride7 (mount 2 273 + 600) | 9 707 | **+0.303 +- 0.289** | +0.245 +- 0.249 | 100.6% (-0.8) |
| mixR11p3 (p3, 11) | ride7 (mount 2 635 + 600) | 18 891 | **+0.149 +- 0.163** | +0.141 +- 0.165 | 99.7% (0.5) |
| mixR7p4 (p4, 7) | ride7 (mount 2 855 + 600) | 14 723 | **+0.111 +- 0.238** | +0.108 +- 0.245 | 101.1% (-1.8) |
| mixR11p4 (p4, 11) | ride7 (mount 11 314 + 600) | 8 037 | **-0.077 +- 0.299** | -0.001 +- 0.289 | 99.4% (0.9) |
| mixL7p4 (p4, 7) | lake6 | 16 001 | **+0.018 +- 0.116** | +0.015 +- 0.116 | 96.8% (3.3) |
| mixL11p4 (p4, 11) | lake6 | 14 007 | **+0.053 +- 0.153** | +0.073 +- 0.136 | 98.3% (1.8) |
| mixA7p4 (p4, 7) | attract | 25 639 | **+0.093 +- 0.117** | +0.059 +- 0.125 | 97.3% (4.6) |

(attract seed 11 not run: the round was closed early at the coordinator's request, the result already clear: even.)

(p2 = strips + reuse with quad12Body `flatten`ed: GCC inlined round 116's diagnostic caller map into it, 14.4 KB of
code, slower than quad117; p3 = without flatten, the ring slots written as made, reuse double-buffered; p4 = p3 + the
CLIP judgement on NEON. p5 = p4 with the switch off by default, the shipped build.)

**Decision: `SOTC_S12DRAW` off by default (on with `SOTC_S12DRAW=1`).** The rule is z > 2 in both seeds riding; the
frame time's A/B gives +0.30 (z 2.1) and +0.15 (z 1.8) for p3, +0.11 / -0.08 for p4, ~0 at the lake. The sampler
agrees that it is about even: quad12Body + s12Feed + imm12 cost what quad117BodyA9 + immFeed128 + imm128 cost (99-101%);
what goes on top (glColor4f's per-vertex gl block writes, feed2D's register switch for the quads' pairs) is small and
shares its symbols with other callers. The VU-exact arithmetic (three clamped 4x4 products, the VU divide, the fixed-point
encodes) is the quad's cost, not the gl block's memory round trips that the part removed. Its parts stay exact and
ready (`SOTC_S12DRAW=1`; with `s12draw` in SOTC_AB_FLAG the A/B's off state is the old path).

## 5. Pictures

Vita3K 222 MHz, muted, window shots looked at (Wander, Agro, the ground, the cloths, the shadows, the haze strips behind
Agro and Wander), then the A/B and check runs' shots and sampler snapshots deleted.
- **bootRp5** (p5, the shipped build, with `SOTC_S12DRAW=1`, ride7, `SOTC_VITA_EAT_USER_MB=89`, 600 s): mounted at frame
  9 866, 12 798 frames, 3 game frames over 300 ms (the loads), No exact 0, 291 231 quads the new way. Wander running to
  Agro, the gallop on the plain: the haze strips behind Agro's tail and Wander's mantle, the cloths, the shadows on the
  grass, the ground, the sky: normal.
- **The A/Bs' shots** (each frame on or off at random): the gallop with the haze behind Agro (mixR7p3), the shrine's
  stairs with Agro in the attract demo (mixA7p4): normal, as the base's (profB0).
- **The checks** (the old way drawing): chkRp1, chkLp4 (the lake shore with Wander and Agro): normal.

(The ride A/B windows are short when the mount comes late: mixR7p2 mounted at 9 992, mixR11p4 at 11 314.)

## 6. What still ties the draw to the PS2

1. **The immediate quads' math is still the VU0's** (quad12Body ~1.6% of the game thread's on-frames riding): three
   4x4 products with the PS2's operand clamps per vertex, Q by the VU's divide, the CLIP history shifted through the
   context's register, the GS fixed-point XY / Z encoded and decoded again by vertex2D. A host strip vertex (screen
   x / y / w as floats, one view-screen product) would not be bit-exact with the PS2's rounding; that is the next step
   once exactness is relaxed for the haze (its output is a screen-copy distortion).
2. **The strips' segment walk** (strips, its lambdas and their long-branch veneers, ~0.85%): guest stack frames and
   guest float registers kept as the recompiled code had them.
3. **The model hook's VIF / VU1 reads**: readChainPaletteLean / readChainPalette (0.6%), the lights copied out of the
   VU1 packet that objectLight writes, the plans checked against the guest packet lists (planChecked 0.18%).
4. **The lane's mirror of modelDisp** (+0x2c, the CLIP chain): ~6% of another core, ~0.03 ms of game-thread joins a
   frame after S10.
5. **The texture records' GS transfer state** (resetFast / texResetFast 0.46%) and **the layer stack / gsReg hooks**
   (0.59%).
