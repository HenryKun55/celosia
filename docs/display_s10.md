# S10: the model draw path without the GS's state on the game thread (2026-11-18)

Round S10's brief: Display and models is the largest block of the game thread riding (24%, ~8 ms a frame in Vita3K at
222 MHz); the console is ~6 ms a frame short of 30 fps. The aim is the phase-2 redesign of the draw prep (host draw
records from host data, nothing shaped for the GS / VIF / DMA built or read on the game thread), starting with the
biggest measurable piece, the code laid out for that end state.

Switch: `SOTC_S10DRAW` (A/B name `s10draw`; sotc_decomp switch `kS10Draw`, default on). Parts:
`SOTC_S10DRAW_PARTS=<mask>` (bit 0 `surface`, bit 1 `texture`, bit 2 `lockonce`; default 7). Check mode
`SOTC_S10DRAW_CHECK=1`, counts `SOTC_S10DRAW_LOG=<frames>` (`[s10draw]` lines, default every 600 frames). Code:
game/overrides/sotc_native.cpp (namespace `sotc_native::s10draw`, mdsjoin::wrap, the model hook's lane hand-over,
mdsPlanPackets, meshDispHook's check), decomp_isys48.cpp (texTransRequest, dlReset's epoch). Scripts, runs and logs:
build/agent-scratch/s10/ (STATE.md; abs10.sh the A/B, chk10.sh the check, chain_ab.sh / queue1-3.sh the queues).

## 1. The profile: where the 8 ms go (ride7, Vita3K 222 MHz)

Base celosia-s8; S9's sampler run profR0 (ride7, 120-960 s, 21 361 frames, game thread 32.2 ms a frame) re-read with
tools/s4groups.py (`--area 'Display and models' --top 600`) and grouped by step. Self time of the game thread's symbols;
x 32.2 ms.

| step | share | ms a frame | what it is | tied to the PS2 by |
|---|---:|---:|---|---|
| walk / clip / LOD / sort (dispDispObjGroupModel 1.29, clip83 0.77, addSortObj 0.58, getLayoutClass 0.56, updateSimple 0.54, initLayoutObjMatrix 0.32, sort / next / clip140 ~1.1, solidDisp / dispLayoutObjModel 0.44 ...) | 8.35% | 2.69 | real work: culling, LOD, sort, the class callbacks | guest objects and callbacks through the function table with exact registers; the guest sort list; the LOD bits in the instance; the VU0 CLIP register's chain |
| model hook + modelDispState lane (modelDispHook 1.50, lane jobs run by the game thread's joins: mdsPartWalk / mdsPlanPackets 1.31, readChainPalette* 0.46, hookScrolls / scroll127Valid 0.36, mdslane join / push / take 0.35, queueNextDraw 0.23, packetListHash 0.11, modelPlan 0.09 ...) | 3.29% | 1.06 | host draw records (the job) and the mirror of modelDisp's guest writes | the palette parsed out of the instance chain's VIF UNPACKs; the plans built from and validated against the guest packet lists; the lane redoing modelDisp's packet loop for the GS layer cache, the part tree's +0x2c and the CLIP chain |
| lights (lightPass 0.69, getAmbientColorC 0.47, execGlobalLightMan 0.33, objectLight 0.27, calcLightLayoutObj 0.27, light matrices ~0.33) | 2.74% | 0.88 | real work | objectLight writes the camera-space lights into the instance's VU1 packet (own +0x10..+0x90), the hook copies them back |
| immediate quads / 2D (glVertexKick 0.48, immFeed128 0.29, vertex2D 0.25, glColor4f 0.35, emit2D, glEndHook, sprites ...) | 2.31% | 0.74 | the haze / grass strips' screen-space quads, sprites, HUD | the guest gl ring and GS A+D pairs built, then parsed back into the 2D assembler (plus quad117BodyA9 1.54% and the strips 0.9% under "Other game code") |
| prefetch notes (note 1.14, objectLines 0.31, aheadWalk* 0.55) | 1.99% | 0.64 | the console's DRAM latency hidden (next frame's lines asked for) | follows the guest walk's object order |
| textures (texTransRequest 0.42, texReset 0.42, vramAlloc / textureRecord / textrans / upload ~0.33) | 1.39% | 0.45 | GS VRAM upload requests and their resolve | entirely GS: VRAM placement, upload packets, request lists |
| particles' draw side (ptcl117, s1ptcl, ptclS1Commit, ptcl62 ...) | 1.18% | 0.38 | real work (S1 / S5 already host records) | the particle display lists |
| layer stack / gsReg / other hooks (pushLayerLean, popLayerLean, gsRegLean, meshDispHook ...) | 0.88% | 0.28 | host layer mirror, 2D feed | the display list's layers and GS register writes |
| other (sword blur, camera blur, fog ...) | 1.02% | 0.33 | | |

**Record round trips versus real work.** Real work (walk / clip / sort / LOD, lights, particles): ~3.95 ms. Work that
exists only because the draw is shaped for the GS / VIF / DMA: the hook's reads of VIF / VU1 records and the lane's
mirror of modelDisp (~1.06), the GS pairs of the immediate quads and 2D (~0.74, plus part of the strips' ~0.8 under
"Other"), the GS texture requests (~0.45), the layer / gsReg hooks (~0.28): **~2.5 ms**, plus 0.64 of console latency
hiding. The **largest single PS2-only item** was the lane joins: the GS layer cache *0x118bfc8 made every
modelSetSurface / modelResetSurface (the cloth grid, the particle geodisps: riding ~10 modelSetSurface and ~47 modelResetSurface a
frame, chkRp2) join the modelDispState lane, and each join ran the lane's pending jobs on the game thread
(`[mdslane] help`: 13 058 jobs in 3 482 joins per 600 frames riding, 0.52 ms a frame; console round 105: "lane help"
1.04 ms a frame).

## 2. What changed (`SOTC_S10DRAW`)

### Part `surface` (bit 0): the GS layer cache left out

*0x118bfc8 is "the surface whose GS registers each display-list layer was last set to": modelDisp's packet loop writes
it, modelSetSurface skips the surface's texture request and packet when it already matches, modelResetSurface clears an
entry, modelSetEnviroment clears the whole cache at every walk. With the native renderer drawing, guest modelDisp does
not run, and the cache's only readers left are modelSetSurface (called by the cloth grid, manager_sub_01137F78) and
modelResetSurface (the particle geodisps, gamecore_sub_0136D4B0 / 01382FC8 / 0138D480 / 0138D910). What the renderer takes
of them is one thing: the texture note texTransRequest's hook leaves for the cloth's draw record (meshDispHook reads
`g_requested`).

- modelSetSurface (in mdsjoin::wrap, the wrapper round 61 already installed: no new table entry) becomes its host
  effect: the surface record (header +0x50 + index x 0x120); one texture (+0x1c == 1, bit 5 of +0x18 clear) -> the note
  = the texture resource's +0x20, as texTransRequestHook makes it; two textures (texTransRequestMulti, which has no
  hook) or none -> no note. No join, no cache test, no request packets, no GS packet chained.
- modelResetSurface runs without the join (its store of 0 is harmless: nothing reads the cache).
- The lane's jobs no longer write the cache (MdsIn::cache 0 -> mdsPlanPackets skips the store).

Result: the game thread's cache joins are gone in the on frames; the lane's jobs run on the lane (or the hook worker,
which helps the lane for its own models: hook jobs +0.05-0.17 ms a frame).

### Part `texture` (bit 1): texTransRequest without the GS upload request

texTransRequest (decomp_isys48.cpp) built, per call (~300 a frame), two packets, bumped the texture owner's count and
appended the request to the layer's list: texTransResolve's input, which places the texture in GS VRAM and builds its
upload. With the native renderer the GS has no VRAM in use; the renderer decodes textures from their NTO2 headers in
guest RAM and takes the texture of a 2D / mesh / particle draw from the hook's note. Now: the hook's note (unchanged,
before the native), the lock (iosDlLockObj: the texture's delete held past the frame, the renderer may read it later),
v0 the layer's tail slot as before. The request lists stay empty, so the resolve, vramAlloc and the uploads do nothing.

### Part `lockonce` (bit 2): the texture lock once per buffer build

Of texTransRequest's remaining cost, iosDlLockObj was most (a lock count, a link added to the buffer's lock group, a
counter: 0.36% of the game thread). A locked object's delete waits for the buffer's release, so a texture already locked
in the current buffer build (dlReset's epoch) is the same object still held: a second lock only adds a link released at
the same moment. A 512-slot table (object, epoch), game thread only (not on the guest worker's lane), and the object's
lock count read as well. 93% of the locks are skipped riding (437 928 of 472 317 in mixR7p3).

## 3. Equivalence

`SOTC_S10DRAW_CHECK=1` (chk10.sh): every modelSetSurface runs the old way (the join, the recompiled function with the
cache, the request) so the picture is the old one, and the texture the new way computes from the same entry state is
compared **at the cloth's draw record** (meshDispHook's `header`, s10CheckMesh). p1's first check compared at the
call's return instead and counted 650 "differences" in 176 953 calls: the old way's request landing after the call had
returned (the recompiled function resumed past a checkpoint), one call late; at the record they agree.

| run (p2) | route | frames | modelSetSurface checked | cloth records compared | cache hits (old way) | records differ |
|---|---|---:|---:|---:|---:|---:|
| chkRp2 | ride7 (900 s) | 23 958 | 245 399 | 245 399 | 0 | **0** |
| chkLp2 | lake6 (700 s) | 15 738 | 117 404 | 117 404 | 0 | **0** |
| chkAp2 | attract (600 s) | 26 118 | 153 908 | 153 908 | 0 | **0** |

The cache never hit in the old way (modelResetSurface clears the layer before each modelSetSurface), so the old way
always requested and the new note is always its note. Parts `texture` / `lockonce` change no host record by construction
(the hook's note is taken before the native; the lock still holds every texture for its buffer); draws a frame on / off
in the A/Bs: the same within 0.5%. Tolerated (as S4): the display list is shorter (no surface / request packets), the
GS request lists and VRAM bookkeeping stay empty, texture owner counts are not bumped, and a texture is linked into the
buffer's lock group once. spalign: 952 NEON :128 / :256 accesses, 0 sp-based (p1, p2, p3). No new function-table wrapper (S8's crash): the
two cache sites go through the wrapper round 61 installed.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s10draw SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

abs10.sh, 800 s a run, frames from the mount + 600 riding (ride7), from frame 3 000 otherwise; tools/mixframes.py
(`--shift -1`, 300-frame blocks, +- 2 SE). Positive = on is faster.

| run (build, seed) | route (window) | frames on / off | **frame off - on** | game busy off - on | render thread | hook jobs |
|---|---|---|---|---|---|---|
| mixR7p1 (p1, 7) | ride7 (mount 6 132 + 600) | 6 363 / 6 311 | **+0.741 +- 0.173** | +0.896 +- 0.141 | +0.020 +- 0.052 | -0.170 +- 0.083 |
| mixR11p1 (p1, 11) | ride7 (mount 8 483 + 600) | 5 355 / 5 426 | **+0.908 +- 0.224** | +1.037 +- 0.208 | -0.187 +- 0.217 | -0.067 +- 0.130 |
| mixL7p1 (p1, 7) | lake6 | 7 795 / 7 768 | **+1.514 +- 0.153** | +1.515 +- 0.155 | -0.045 +- 0.158 | -0.147 +- 0.063 |
| mixL11p1 (p1, 11) | lake6 | 7 283 / 7 271 | **+1.555 +- 0.201** | +1.556 +- 0.184 | +0.023 +- 0.112 | -0.159 +- 0.089 |
| mixA7p1 (p1, 7) | attract | 12 658 / 12 657 | **+0.247 +- 0.129** | +0.487 +- 0.132 | +0.015 +- 0.100 | -0.053 +- 0.045 |
| mixA11p1 (p1, 11) | attract | 13 055 / 13 176 | **+0.437 +- 0.205** | +0.547 +- 0.201 | -0.019 +- 0.087 | -0.076 +- 0.062 |
| mixR7p3 (p3, 7) | ride7 (mount 7 776 + 600) | 2 866 / 2 777 | **+0.414 +- 0.354** | +0.563 +- 0.362 | +0.114 +- 0.442 | -0.114 +- 0.131 |
| mixR7p3, from frame 3 000 | ride7 (on foot + riding) | | +0.961 +- 0.346 | +1.094 +- 0.383 | | |
| mixR11p3 (p3, 11) | ride7 (mount 8 463 + 600) | 2 975 / 3 104 | **+0.577 +- 0.238** | +1.276 +- 0.159 | +0.035 +- 0.076 | +0.022 +- 0.069 |
| mixR11p3, from frame 3 000 | ride7 (on foot + riding) | | +0.983 +- 0.237 | +1.330 +- 0.158 | | |
| mixL7p3 (p3, 7) | lake6 | 5 964 / 5 912 | **+1.368 +- 0.147** | +1.375 +- 0.148 | | -0.164 +- 0.069 |
| mixL11p3 (p3, 11) | lake6 | 5 629 / 5 663 | **+1.682 +- 0.176** | +1.726 +- 0.201 | | -0.146 +- 0.075 |
| mixA7p3 (p3, 7) | attract | 9 400 / 9 449 | **+0.486 +- 0.162** | +0.682 +- 0.143 | | -0.015 +- 0.054 |
| mixA11p3 (p3, 11) | attract | 9 839 / 9 923 | **+0.370 +- 0.150** | +0.575 +- 0.132 | | -0.016 +- 0.064 |

(p1 = parts surface + texture; p3 = + lockonce, the shipped build. The p3 ride runs mounted late (7 776 / 8 463 of
~15 000 frames), so their riding windows are short (~5 700 frames) and their errors twice p1's; from frame 3 000 they
read +0.96 / +0.98 ms. The sampler's NEW / OLD split says little here: what S10 removes is mostly the lane jobs the
game thread no longer runs (mdsPartWalk / mdsPlanPackets 1.31% of the game thread in the base, 0.43-0.49% mixed in
mixR7p1 / mixR11p1, i.e. ~0 in the on frames) and joins, not code of its own.)

**Decision (z > 2 in both seeds on every route): `SOTC_S10DRAW` on, parts 7.** Riding (the mount + 600) -0.41..-0.91
ms a frame (z 2.3-8.6), the game thread's busy time -0.56..-1.28 ms; the lake -1.37..-1.68 ms (z 9-19); the attract
demo -0.25..-0.49 ms (z 3.8-6.0). The hook worker pays 0.05-0.17 ms a frame more (it now helps the lane for its own
models); the render thread is unchanged. Console: build/agent-scratch/celosia-s10.vpk with
build/agent-scratch/s10/env-console.txt (`s10draw` first in SOTC_AB_FLAG's rotation); round 105 measured the console's
lane help at ~1 ms a frame, so the console may gain more than Vita3K's ride shows (its lane does not get a core while
the render thread draws).

## 5. Pictures

Vita3K 222 MHz, muted, window shots (looked at, then the runs' shots and sampler snapshots deleted but the boot run's).

- **bootRp3** (p3, ride7, `SOTC_VITA_EAT_USER_MB=89`: 89 MB taken as on the console, 720 s): mounted at frame 3 907,
  15 738 frames, 4 game frames over 300 ms (the loads), No exact 0. Wander running to Agro, the gallop on the plain and
  by the cliffs, the shrine's stairs and walls: Wander's mantle and Agro's mane / tail (the cloths whose texture part
  `surface` now notes), the shadows on the grass, the grass and ground, the sky, the bloom: normal.
- **bootRp1** (p1, the same route, did not mount): the shrine with Agro, the plain, the cliffs: normal.
- **The A/Bs' shots** (each frame on or off at random): the lake swim with the HUD's grip ring and sword icon (2D
  textures through the lean texTransRequest), the title screen and its menu text, the attract demo's gallop: normal.
- **The checks** (the old way drawing): chkRp1 / chkRp2 / chkLp2 / chkAp2 normal.

## 6. What still ties the draw to the PS2 (the next display rounds)

The end state: per object a host record (model, matrix, material, palette, lights) made from host data and drawn into
vitaGL, nothing shaped for the GS / VIF / DMA on the game thread. After S10, in the order of what they cost riding:

1. **The lane's mirror of modelDisp** (~0.5 ms of lane work a frame, now mostly off the game thread): the part tree's
   results written to +0x2c (read by nobody but modelDisp's own internals, which do not run, and the lane / hook
   themselves), and the VU0 CLIP register's chain (modelDisp's root test ORs the three judgements before it; the game's
   clip tests read it). Next: the part visibility as a host array, and the CLIP chain reduced to the judgements its
   readers actually use (a census of the 9 readers' masks), after which the lane is a pure host job with no join.
2. **The instance chain's VIF records**: the hook parses the palette out of the chain's UNPACKs (readChainPalette*
   0.46%), copies the lights out of the VU1 packet (objectLight writes them there), the kick re-reads palettes. Next:
   dispSkinned and objectLight write host records (palette rows, camera-space lights) that the hook takes by pointer.
3. **The plans** built from and hashed against the guest packet lists every 16 frames (packetListHash, modelPlan):
   next, a host model cache filled at model load (modelInitDispModelClass), keyed by header, with no per-frame checks.
4. **The immediate quads and 2D** (~0.74 ms + the strips' ~0.8): screen-space vertices written to the guest gl ring and
   as GS A+D pairs, parsed back into the 2D assembler. Next: the strips emit host vertices directly (the ring only
   when glEnd's software clip needs it).
5. **The layer stack and gsReg hooks** (~0.28 ms): the display list's layers mirrored on the host.
6. **The walk itself** (~2.7 ms) stays the game's own logic over guest objects; the console's prefetch notes
   (~0.64 ms in Vita3K) follow it and are for the console's A/B, not Vita3K's, to judge.
