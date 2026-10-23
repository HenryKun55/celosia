# Decompiling the game, function by function

The game runs as recompiled code: every MIPS function of the boot ELF and
the XFF modules turned into C++ mechanically (recomp/game/output, ~16 000
functions, 2.5 MB of MIPS). That code works but cannot be read or changed.
Decompiling here means rewriting a function (or a whole system) by hand in
readable C++ and putting it in place of the recompiled one, proven equal.
Not a matching decomp: nothing has to rebuild a PS2 binary.

## The loop

1. **Find what matters.** `PS2X_PROFILE=<file>` samples the running EE
   context's pc 1000 times a second (`PS2X_PROFILE_START=<s>` skips the
   boot); `tools/guest_profile.py <file> --module` names the functions and
   their callers. Run it on the route: `PS2X_PROFILE=... tools/native_run.sh <dir>`.
2. **Read it.** `tools/ghidra.sh import` builds the Ghidra project once
   (build/ghidra, from recomp/game/SOTC.elf with the recomp names applied);
   then `tools/ghidra.sh DecompileTo.java $PWD/build/ghidra/out <name|0xaddr> ...`,
   `Disasm.java 0xaddr`, `Callers.java name`, `RefsTo.java 0xaddr`,
   `Containing.java 0xaddr`. Decompiler output is a guide, not the truth:
   check the disassembly where it matters (it splits functions at stray
   branches, see FUN_00125448 inside dpmul).
3. **Write it.** In `game/overrides/decomp_<system>.cpp`, a function with the
   recompiled signature, registered with `SOTC_DECOMP_FUNCTION(address, name, fn)`
   (`SOTC_DECOMP_FLOAT_FUNCTION` when it returns in f0). Arguments in a0-a3
   (64-bit GPRs), f12/f13; result in v0 or f0; end with `returnTo(ctx)`.
4. **Prove it.** `SOTC_DIFFTEST=name,name` (or `all`) runs each call twice on
   the same state, the native one and the recompiled one, and compares v0,
   f0, the callee-saved registers and every byte of RAM and scratchpad
   (the dead stack below sp excepted). `SOTC_DIFFTEST_FIRST` / `_EVERY` set
   how many calls are checked (each check copies 64 MB: the game stutters).
   A function goes in with 0 mismatches over the route.
5. **Measure it** with the profiler again (Mac, then the Vita).

`SOTC_DECOMP=0` puts every recompiled function back; `SOTC_DECOMP_SKIP=name`
one of them.

A function the recompiled code reaches by a tail jump (`j` at the end of
its caller) is called directly as C++, not through the function table, so
its registration is never seen there: replace the caller as well (see
decomp_keyboard.cpp). Check with SOTC_DIFFTEST that the checks count up.
Difftest runs are slow enough that the scripted route's inputs (timed in
VBlanks) land elsewhere: compare pictures from a run without it.

What a native function has to keep, learned the hard way:

- **Relocated code in the XFF modules.** The modules' references to the
  boot ELF (and to each other) are patched into the code when the game
  links them: `lui` / `lw` pairs and `jal`s whose values differ from the
  disassembly (the recompiled code reads them at run time: `_dynInsn`,
  `jumpTarget = READ32(site)`). Read them the same way:
  `sotc_decomp::relocated(rdram, luiSite, loSite)` and
  `sotc_decomp::jalTarget(rdram, site)`. charaLinks first compared against
  the disassembly's 0x1b7b60 and got another pointer.
- **Guest calls** from a native function: `sotc_decomp::callGuest(rdram,
  ctx, runtime, target, ra, sp)` with the original's ra (the jal site + 8)
  and frame sp; set the argument registers the callee reads; whatever the
  callee reads from the caller's frame has to be written there first.
- **Registers compared:** v0 (also for void functions), f0, s0-s7, fp, gp,
  sp, ra. Keep v0 / f0 as the original leaves them (the last value it
  wrote, often a callee's). VU0 registers, fcr31 and the other FPU
  registers are not compared: the native code writes back what the
  original leaves there anyway where it is cheap (vf / ACC / Q / R,
  status).
- **Same operations, same order:** FPU_* and PS2_V* macros (EE rounding,
  clamping, operand alignment), the original's constants by their bits
  (0.001 is 0x3a83126e, not 0x3a83126f).
- **Rare cases go back to the recompiled code** before anything is written
  (`sotc_decomp::recompiled(address)`): a dying particle, a collision test,
  a negative count. A function can also do its hot part natively and then
  enter the recompiled one at one of its labels with the registers and
  frame it expects there (decomp_cloth.cpp, at 0x1138200).
- **Unity build:** every decomp_*.cpp gets a namespace of its own.
- The difftest is re-entrant: a checked function reached while a check
  runs (recursion) runs recompiled, unchecked.

## Done

| Module | Functions | Checks on the route | Notes |
|---|---|---|---|
| decomp_softfloat.cpp | dpadd, dpsub, dpmul, dpdiv, dpcmp, litodp, dptoli, fptodp, dptofp, __floatdisf | ~93 000, 0 mismatches (dptoli not called on the route) | the SDK's software doubles: no rounding (truncation), denormals as zero; ~3% of the EE's busy time |
| decomp_sortlist.cpp | GetNextSortObj, ClearSortList | 19 000 + 3 000, 0 mismatches | the display's sort buckets; 4% of the Vita's game thread recompiled (throttled attract demo) |
| decomp_keyboard.cpp | kbGetStatus, usbdevFrameInit | 1 500, 0 mismatches | 256 keys copied every frame (1.2%). usbdevFrameInit tail-jumps to kbGetStatus and the recompiler makes that a direct C++ call past the function table: replaced in the caller too |
| decomp_time.cpp | mktime (newlib, with validate_structure) | 900, 0 mismatches | our console clock reads zeros, so the year is -1900 and mktime's year loop ran ~1 900 times a frame (0.8%); days counted in closed form, checked against the loop for every year in +-10000 |
| decomp_softfloat.cpp | litodp, __floatdisf (faster) | 160 000, 0 mismatches | litodp normalised bit by bit (up to 60 steps): one clz; __floatdisf within +-2^24 is the plain conversion (every step exact) |
| decomp_particles.cpp | manager_sub_0113CA58 (particle step) with 0113D280, 01141918, 01141968, 01141B70 | 37 000 (attract demo + route), 0 mismatches | ~9% of the Vita's game thread. Dying particles, the collision test (0113D5F0) and shape 0 go to the recompiled code; iosGetRandom / iosGetReflectVector / the damage test (0113C8C8) stay guest calls. In the attract demo 99.8% of the calls are native; the damage test runs for almost every particle (guest) |
| decomp_chara.cpp | manager_sub_010CE558 (charaLinks), manager_sub_010CDD10 (findRecord) | 2 000 + 1 200, 0 mismatches | the character's handle-list searches (3.4% + 0.9%) |
| decomp_layout.cpp | UpdateLayoutClassGeometry | 1 800, 0 mismatches | bone matrices: inverses, scaled, products (3.6%); iosScaleMatrixV / iosGetScaleMatrixV / iosGetScaleInverseVector stay guest calls |
| decomp_cloth.cpp | manager_sub_01137F78 (cloth mesh) | 8 500, 0 mismatches | the grid normals and the mesh vertices natively (2.1%), then the recompiled function from 0x1138200 (display list layer, camera matrix, meshDisp / meshFlip) |
| decomp_clothstep.cpp | manager_sub_011370E8 (cloth step) | 8 500, 0 mismatches | gravity, wind, VRNEXT noise, the pinned row ends (1.3%) |
| decomp_skeleton.cpp | manager_sub_010B8590 (skeleton pose, recursive) | 37 000, 0 mismatches | the quaternion -> matrix VU0 block natively (2.8%), the matrix / quaternion stack helpers as guest calls |
| decomp_math.cpp | iosGetSinCosf | 30 000, 0 mismatches | VU0 polynomial (1.2%) |

| decomp_texanim.cpp | manager_sub_010A9EA8 (texAnim: texture scroll + palette animation) | 40 000 attract + 35 000 route, 0 mismatches | ~300 calls a frame (3.4%); the waves' iosGetCosf / iosGetSinf stay guest calls, iosDlGetWorkBufId read directly. Not seen on the routes: the wobble terms and the CSM1-swizzled formats 0x13 / 0x1b (written from the disassembly, unchecked) |
| decomp_skeleton.cpp | the pose's stack helpers (PushQuaternion, iosPush/PopCurrentMatrix, MultiCurrentQuaternion, iosTransCurrentMatrix, iosMulCurrentMatrixR, iosGetCurrentMatrix, Get/PopQuaternion) inline | 19 000 + 10 000 (skeletonPose), 0 mismatches | 9 guest calls a bone were ~2.4% in call overhead alone (runToReturn + callGuest); on the context's VU0 registers and VU0 memory (vi15) as their recompiled code |
| decomp_layout.cpp | iosScaleMatrixV inline (UpdateLayoutClassGeometry), GetLayoutClass | 700 + 400; 90 000 + 145 000, 0 mismatches | GetLayoutClass: ~1 700 calls a frame (1%); the cases that call out go to the recompiled code |
| decomp_subthread.cpp | isysWakeupSubThread, isysSleepSubThread | sleep 6 900 + 3 600, 0 mismatches; wake: see below | the sub-threads' coroutine switch (1.85%): 0x2c0 bytes of registers saved / loaded as blocks. The game's own load gives f5..f8 f4's word (no lw for them): kept |
| decomp_gctick.cpp | iosMallocGarbageTickProc with its time check kernel_sub_001C81B0 | 300 + 200, 0 mismatches | the heap's garbage walk, ~385 blocks a frame, the time check before each (1.7%) |
| decomp_modelclip.cpp | manager_sub_01099C50 (modelClipPoint) | 5 500 + 2 600, 0 mismatches | a model point through the camera, CLIP test, distance (0.9%) |
| decomp_ptcldraw.cpp | manager_sub_0113E940 (particleDraw), its particle loop | 600 + 700, 0 mismatches | then the recompiled code from its label 0x113eafc (the matrix, the ptclman packets) |
| decomp_collision.cpp | the segment test: ClipCollisionDirectCallBack (0x112d590), the tree's node visitor and sides (01131C80, 0112C610, 0112C498), the leaf (0112BE58), ClipCollisionDB (the plane, the crossing, the triangle), GetCollisionDBSurfBoundary, getLayoutClassCollisionIndepSurfBoundary, GetCollisionDBAttributeAdr; ClipCollisionDB's keep variant (0111C078, its first test); the sphere finder's leaf (01115798 / 01115A50) and CheckCollisionFinderExistData; the id filters (CameraVisibility, CameraLeave, 010D41E0, 011292B0, pathJudgeFunc, 01434C00) | route (EVERY=100-200, parents left recompiled for the children): ClipCollisionDB 21 700, leaf 2 400, node / sides 10 500, DirectCallBack 600, keep 5 900, finder leaf 1 300, filters 300-3 500 each; attract demo: see below; 0 mismatches | the shrine altar's collision family, ~29 ms of the throttled Vita3K frame: see "Collision" below |
| decomp_pointsort.cpp | execPointListSort (0x142da48) | 300 (route), 0 mismatches | the path points within a radius, selection-sorted (1.3% at the altar); getPointDataByAreaIdx stays a guest call |
| decomp_skeleton.cpp | skeletonPose: the bone's VU0 block, the quaternion normalisation, MultiCurrentQuaternion, iosTransCurrentMatrix / iosMulCurrentMatrixR with each operand clamped once (vu::*N, the rows once for the four columns) | 87 300 (route, EVERY=20) + 17 900 (attract demo), 0 mismatches | ~210 -> ~90 clamps (ps2VuNormalize) a bone |
| decomp_clothdyn.cpp | 01137858 (the rows' length constraint), 01137640 (the velocities; type 1 cloths go on in the recompiled code from 0x1137728 with the prologue's frame) | attract 1 100 + 5 600, route 2 700 + 6 900, 0 mismatches | under GetClothDynamics (in ExecCharaGeometry). 01137858's type 1 path stays recompiled. RSQRT as build/runner has it (ps2VuRsqrt with the status flags, not recomp/game/output's older ps2VuRsqrtQ) |
| decomp_prefetch.cpp | ClipLayoutObjModel, DispLayoutObjModel: the recompiled functions, with the next object's lines prefetched | route 23 900 + 9 600, attract 4 700 + 2 600 (EVERY=20), 0 mismatches | see "The display walk's memory misses" below |
| decomp_light.cpp | the light pass: manager_sub_010F4FB8 (its three walks of the sort list), 010F4A18, ExecGlobalLightMan, 010F8360, 010F5F60, SetWorkFromGlobalLight, GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix | attract 800-6 500 each, route 300-6 700 each, 0 mismatches | see "The light pass" below |
| decomp_curve.cpp | manager_sub_010AC6C8 (curveEval: a motion channel's key), manager_sub_010B81F8 (a motion's keys, curveEval called natively) | attract 80 800 + 2 100, route 86 300 + 1 600, 0 mismatches | formats 0, 3-9, 12, 13 natively (all the attract demo and the route use); 1, 2, 10, 11, 14 recompiled (SOTC_CURVE_STATS=1 counts them). See "Round 3" below |
| decomp_quat.cpp | GetQuaternionByAxisDirZY, GetDifferencialQuaternionWithNoRegularize, SetQuaternionByAxisRotateVWithNoRegularize, GetEulerYXZFromQuaternion, iosGetATan2fExact, atan2f / __ieee754_atan2f, atanf | attract 4 500-32 900 each, route 2 500-28 600 each, 0 mismatches | each natively into the next (newlib's atanf / atan2f ported with the FPU_* ops); asinf stays a guest call |
| decomp_bridle.cpp | gamecore_sub_013978E8, gamecore_sub_013976B8 (the horse's rein cloths against its body: three cylinders / the head's) | attract 600 + 600, route 400 + 400, 0 mismatches | iosTransMatrixF / 01B96D0 stay guest calls |
| decomp_skinpal.cpp | manager_sub_010F3CF0 (a skinned model's matrix palette) | attract 1 500, route 800, 0 mismatches | the palette loop natively, then the recompiled code from 0x10f3df0 |
| decomp_texreset.cpp | manager_sub_010AB618 (the texture list's transfer reset) | attract 1 300, route 1 100, 0 mismatches | ~1 000 nodes a frame |

The wake is a round trip: the check runs the sub-thread until it sleeps,
and that code writes values that differ between two runs of the same code
(object state at 0x14656xx, 0x15b68c4, 0x15b8bd0 ...). A control run
(SOTC_DIFFTEST_CONTROL=1, the recompiled code in both legs) gives the same
mismatches at the same addresses at the same rate (17 000 checks, ~700
each); every register (all 128 bits, FPU, hi / lo / sa) matched in all.

## The light pass and the scratchpad (2026-10-22)

The console's "dispSort_010F4FB8" (2.75 ms self in the attract demo) is
not a sort: it is DispDispObjGroupModel's light pass. LoopExecPreLightColMan
copies the light work area into the scratchpad, then every lit object's
+0xe0 callback (CalcLightLayoutObj -> SetGeoDispClassLightColMan) runs
ExecGlobalLightMan (the global lights in range, their volume, sorted and
blended by 010F8360), SetWorkFromGlobalLight, ExecGameLight and 010F5F60
(the object's normal and colour light matrices); then 010F4A18 per layout
object (its lights in camera space). Every access to the work area went
through the generated code's out-of-line path to PS2Runtime::LoadN /
PS2Memory::readN (the scratchpad is not main RAM).

- **decomp_light.cpp** does that chain natively; leaf helpers (GetLayoutClass,
  GetNextSortObj, iosCalcFColorToBrightness, iosCopyMatrix,
  iosTransposeMatrix, iosInverseMatrix, iosMakeNormal/LightColorMatrix,
  GetDirColorFromWorkLight, getSpecularity, GetCharacterClass, memset)
  inline; getAmbientColorC, GetRateInOutElliptical..., ExecGameLight, the
  camera context and the +0xe0 callbacks stay guest calls from the
  original's frame. A native parent calls a native child through the table
  (callGuest) when that child can fall back to the recompiled code: a
  recompiled function called directly can return early at a checkpoint.
- **ps2_memory.cpp ps2_guest_mem**: aligned scratchpad loads / stores are done
  right there (PS2X_SPR_FAST=0: the long way); for all generated code.
- **The VIF1 chain walk** (iosDmaSendSChainPath1, 1.45 ms on the console,
  one DRAM miss per tag and VIFcode, no hardware prefetch) reads the next
  tag before walking the current tag's data and prefetches that data's
  first two lines (PS2X_DMA_PREFETCH=0: off).
- **SOTC_AB=<frames>** alternated that round's set (SOTC_DECOMP_AB_FUNCTION
  and the two fast paths) with the old code (now the next round's, below); [prof] lines end in "ab: new |
  old | mixed". 480 leaves three clean 120-frame windows out of four.
  SOTC_SUBSYS_PROF_MIN=<ms> lowers the report's cut (Mac: 0.0005).

Throttled Vita3K (444 MHz, 3 cores), attract demo, SOTC_AB=480, 36 windows
each: dispSort 1.69 -> 1.06 ms, ExecCharaGeometry 5.49 -> 5.08 (cloth),
execScript 23.8 -> 22.7 ms. Mac: ExecGlobalLightMan 2x, the pass -25%.
Mac route: the plain, game clock 1.002x (last 12 windows; 0.998 before).

Recompiler levers seen on the way (not done): build/runner (what is
compiled) differs from recomp/game/output (older: pc writes, other RSQRT
macro), always read build/runner; GPRs / VU registers live in ctx memory,
so every guest store (char aliasing) forces reloads.

## The display walk's memory misses (2026-10-22, evening)

The console's heavy-scene profile (build/agent-scratch/console-light):
hook.packets ~10 ms self, hook.state 3-4 ms, hook.packetId 3 ms,
modelClipSolid 2.4 ms self for ~200 calls (12 us a call; 0.17 us on the
Mac): 20-70x the Mac's time where the rest of the game is ~20x. That is
DRAM misses (~250 ns, no useful hardware prefetch on the A9): the walk
touches every object's instance, header, packet list, part tree and
surfaces once a frame, one line after the other. SOTC_HOOK_STATS=1 counts
what modelDispHook walks (the plain, per frame: ~100 instances, ~4 000
packets, ~3 200 draws over ~440 surfaces and ~200 textures).

- **Textures** (sotc_native.cpp decodedTexture): every texture's 64 + 16
  word sample (~80 misses, ~20 us on the console) ran once a frame for
  each of ~200 textures. Now a texture drawn the frame before and never
  seen rewritten is taken as it was; the full sample one frame in 16 (by
  address), as packetId does (SOTC_TEX_QUICK=0: off).
- **Packets** (packetId): the 32-word sample (32 misses) one frame in
  48-63 instead of 16, a 4-word check on the 16-frame beat (a packet
  rewritten in place is still caught within 16 frames). The memo's tags
  apart from its entries (one line looked at, not 3-4), prefetched for
  the next packet.
- **Surfaces** (SurfaceInfo): a surface's state (texture keys, blends,
  clamps, layers, LOD, scroll row address) worked out once a frame, not
  per draw; the flags tests against the game's globals, the row's values
  and the LOD's K product stay per draw (SOTC_SURFACE_MEMO=0: off).
- **Prefetch** (g_prefetch, SOTC_HOOK_PREFETCH=0: off): the order of the
  last frame's modelDisp calls and each model's shape (packet count, part
  tree size, first surfaces) say what the next draw reads; queued and
  asked for a few lines per packet while the current one runs; the part
  tree and the packet list ahead of modelDispState's walk.
- **decomp_prefetch.cpp**: ClipLayoutObjModel / DispLayoutObjModel
  wrapped: the lines the last frame's next call read (object, layout
  class, instance, header, LOD record, matrix, part root) asked for, then
  the recompiled function (SOTC_OBJ_PREFETCH=0: off).

Checks: SOTC_R2_CHECK=1 compares the kept surface state with the reads it
replaces at every draw and the quick texture / packet answers with their
full samples: route 4.9 M surface draws, 0 mismatches, 0 stale textures;
packets kept while changed 28 (the old policy: 41, the same check);
attract demo 0 / 0 / 0. SOTC_MODELDISP_CHECK=1: 190 000 checks, 0.
SOTC_DRAW_DUMP_FRAMES=f,... (SOTC_DRAW_DUMP_DIR) writes those frames'
draw lists (now with the matrix and light hashes); two runs are not
frame-identical, so the in-process checks are the proof.

SOTC_AB alternated that round's set: the two wrappers and the hooks' new
paths (sotc_decomp::abNew, read once a frame; plain since round 3). The
light pass and the cloth passes are plain decompiled functions again; the
scratchpad / DMA fast paths stay on (SOTC_AB_FASTPATHS=1 alternates them
as before).

Calibrated Vita3K (240 MHz, below), attract demo frames 600-4800, the
same VPK with the new paths on / off: hook.packets 2.49 -> 2.18 ms,
modelDisp 3.37 -> 3.21, hook.state 0.24 -> 0.36 (the prefetch
bookkeeping; Vita3K's PLD does nothing), the frame unchanged (48.4 / 48.4).
The misses these remove are the console's: read the console's AB.
Mac route clock: 1.000x.

## Round 3: the characters' math (2026-10-22, night)

Measured first (calibrated Vita3K, SOTC_SUBSYS_PROF with probes on the
children and a VITA3K_PROFILE sample of the game thread): with the r2
probes in place ExecCharaGeometry's own time is ~1.4 ms, not ~4.5 (that was
its unprobed children): GetCharaRootDynamics is 0.4 ms with all its physics
(xzfit, swim, pendulum: under 0.05 ms each in the attract demo). The time
is spread over the motion keys (curveEval and its callers ~1.6 ms), the
quaternion helpers of the IK / expression nodes (GetQuaternionByAxisDirZY,
GetEulerYXZFromQuaternion and newlib's atan2f / atanf under them, ~1.5 ms),
the rein cloths, the matrix palettes and a texture list walk. The camera
(doit_game_camera ~1.9 ms) is ~30 functions of 0.05-0.15 ms each: left.

- **decomp_curve.cpp**: curveEval decodes one key of a channel (packed
  9-bit quaternions with a VU0 sqrt for the fourth component, s16 keys,
  axis-angle with iosGetSinCosf); 010B81F8 calls it for every channel of a
  motion, now natively (no table call / runToReturn a key).
- **decomp_quat.cpp**: the rotation helpers natively into each other;
  atan2f / atanf transcribed from the recompiled newlib code (their pi is
  0x40490fda, not 0x...db: the first difftest caught 2 900 mismatches in
  the -x -y quadrant). Every VU0 operand clamped once and vu::mulN / addN
  / subN after (each PS2_V* op clamps both operands and the result): the
  native helpers went from ~30% faster than the recompiled code to
  1.6-1.9x.
- **decomp_bridle.cpp**, **decomp_skinpal.cpp**, **decomp_texreset.cpp**:
  loops natively (matrix rows clamped once, vu::apply).
- The r2 display-walk paths (the hooks' quick textures / packets, surface
  memo, prefetch; decomp_prefetch's wrappers) are plain now, their
  switches kept; SOTC_AB alternates this round's 14 functions.

Native functions here call a recompiled fallback only as their last act
(the generated code can return at a checkpoint before its end; a native
parent that goes on after a child calls it through callGuest, as 010B81F8
does for the curve formats it does not do itself).

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, one run
per side (HEAD e28f9de's VPK, then this one), the same probes:

| ms / frame | before | after |
|---|---|---|
| frame (wall) | 48.4 | 45.8 |
| execScript | 42.7 | 40.4 |
| ExecCharaGeometry | 10.6 | 9.8 |
| DispDispObjGroupModel (self) | 14.7 (3.85) | 14.4 (3.49) |
| motion keys 010B81F8 (curveEval in it) | 1.57 | 0.52 |
| GetQuaternionByAxisDirZY | 0.74 | 0.40 |
| GetEulerYXZFromQuaternion | 0.71 | 0.44 |
| texture list reset | 0.33 | 0.14 |
| rein cloths (both) | 0.55 | 0.37 |

Mac route: the plain, game clock 1.003x (last 12 windows).

What is left is the emulation itself: the recompiled game spends its time
in PS2_V* / FPU_* (three clamps a VU0 op, operand alignment an FPU add),
and natively the same ops cost nearly the same; the clamp-once trick is
the lever left inside the rules. A note from the way (from the ARMv7
architecture, not checked on the console): Advanced SIMD ignores FPSCR's
rounding mode (its standard FPSCR value rounds to nearest), so on the Vita
PS2_V* round to nearest while the scalar FPU_* truncate as the EE does; the
Mac's NEON truncates both. Native and recompiled agree on each platform
(the same intrinsics), which is what the difftest proves.

## Vita3K calibration (2026-10-22)

Vita3K at the Vita's own clock (MHZ=444) runs the game ~1.85x faster
than the console (its host has caches and a memory system the Cortex-A9
does not). Throttled to **MHZ=240 CORES=3** (`MHZ=240 CORES=3
tools/vita3k.sh <s>`) it matches the console as a whole:

| attract demo, frames 600-4800, 18 windows | console | Vita3K 240 MHz |
|---|---|---|
| frame (wall) | 49.4 ms | 49.2 ms |
| execScript | 43.9 | 44.1 |
| DispDispObjGroupModel (self) | 15.7 (4.5) | 16.4 (5.3) |
| ExecCharaGeometry (self) | 10.4 (5.1) | 10.5 (5.2) |
| modelDisp / hook.packets (self) | 4.5 / 3.4 (2.8) | 3.1 / 2.3 (1.8) |
| doit_game_camera (self) | 2.5 (1.75) | 1.9 (1.05) |
| doit_particle_manager / particleUpdate | 3.4 / 2.1 | 4.2 / 2.7 |
| dispSort_010F4FB8 (the light pass, old code) | 2.4 | 3.4 |
| iosDmaSendSChainPath1 | 1.36 | 1.97 |

Procedure: the same VPK on both (build/agent-scratch/celosia-allean.vpk,
the console log build/agent-scratch/console-allean/err.txt), env.txt
`SOTC_LEAN=2 SOTC_LEAN_PERIOD=240 SOTC_SUBSYS_PROF=1`, the attract demo
left to run (~260 s of Vita3K reach frame 4800), the [prof] windows of a
frame range averaged (build/agent-scratch/lean_range.py err.txt 600 4800
any). 240 MHz matched at the first try (frames 600-3000: execScript 45.8
vs 45.7, frame 50.7 vs 50.5), so no bisection was needed.

Where it still disagrees: our native code that walks guest memory
(hook.packets, modelDisp: the packet / surface / texture reads, one DRAM
miss each on the console) and the camera run ~1.4x slower on the console
than in throttled Vita3K; float-heavy code (particles, the old light pass,
the DMA chain walk) runs ~1.3x slower in Vita3K (its JIT's FPU / NEON).
A memory-latency fix shows less in Vita3K than on the console; a compute
fix shows more.

## Collision (2026-10-21)

The shrine altar spent ~29 ms of the throttled Vita3K frame (94 ms) in the
collision database: the camera's and the character's segment and sphere
probes against each object's collision tree. decomp_collision.cpp does the
segment test from the object loop down (ClipCollisionDirectCallBack ->
the node visitor and its two sides -> the leaf -> ClipCollisionDB) as
native calls into each other, so a traversal is native from its top; the
recompiled callers of any of them still reach the native one through the
function table.

- **Guest calls from native code** keep the original's ra and frame sp
  (callGuest), with whatever the callee reads from that frame written
  there first (the filters get pointers into it). The vertex fetchers
  (01114C70 / 01116108), EntryModifiedManager, the isys group walk,
  GetArbitraryDirectionInverseMatrix and the filters the file does not
  know stay guest calls; 01114C70 is native when its three vertices are
  cached (the common case).
- **Recompiled fallbacks from native code**: sotc_decomp::callRecompiled
  (address, ra, sp), run to its return. Handing ctx straight to a
  recompiled function after guest calls needs ra, sp *and pc* put back
  (the generated code switches on ctx->pc): clipCollisionKeep returned 0
  for 2 until that was fixed (28 mismatches on the route).
- **Lookups kept between calls**: a few pointer chains (a class's plane and
  boundary tables, its part records) are kept in static caches tagged with
  an epoch that every entry from guest code and every guest call from the
  native code bumps (decomp_collision's G constructor, guestCall,
  recompiledCall): within one native traversal they are read once, and
  nothing but guest code could change them.
- **Fewer clamps**: an operand clamped once and then summed / multiplied
  with vu::mulN / addN / subN gives the PS2_V* result bit for bit (the
  clamp is idempotent; x 1.0 of a clamped value is that value); used for
  the plane distances and the barycentric test.
- **Checking a child the native parent calls directly**: the parent is
  left recompiled for that run (SOTC_DECOMP_SKIP=parent), so the child is
  reached through the function table and checked.

Throttled Vita3K (444 MHz, 3 cores), HEAD 461fd49 and the new build
alternated, twice each: shrine altar 94.2 / 93.9 -> 68.6 / 68.6 ms a frame
(game thread 76.7 -> 57.6, hooks 17.3 -> 10.9: the hooks' share is the
vectorised part test and the packet check in sotc_native.cpp); attract demo
34.4 / 34.5 -> 34.0 / 34.0. Left in the altar profile (v9): the native
clipDB / leaf ~1.9% each, the finder's traversal (01128F38, 01116958),
ClipCollisionDirectCallBackObjAry and 0111C078's crossing path ~0.5% each.

SOTC_COLLISION_STATS=1 counts the paths (native / recompiled, crossings,
planes made, the filters run as guest code by address).

Two bugs the checks caught, worth remembering:

- **The time check read the hardware T0.** iosGetTCountExtension is an
  override (sotc_overrides.cpp: the EE clock / 2, T0 is stopped): reading
  T0 natively gave 0, the check said "out of time" at once and the garbage
  walk did nothing, with 1 mismatch in 1 100 checks (only frames where the
  recompiled walk merged a block). Call what the function table has.
- **A leftover of removed debug code** (`if (csm)` left in front of the
  palette copy) skipped the copy of the entries outside the rotated range:
  0 mismatches on the attract demo (its textures rotate the whole palette),
  830 on the route. Run the route too after any edit.

Framework additions: SOTC_DECOMP_SWITCH_FUNCTION /
SOTC_DECOMP_SWITCH_ONCE_FUNCTION (context switches: every register and the
stack below sp compared; `once` runs the recompiled code once instead of
to its return, for a sleep that returns into the other context);
SOTC_DIFFTEST_CONTROL=1 (both legs recompiled: the noise floor of a check);
SOTC_DIFFTEST_IGNORE=lo-hi,... (RAM ranges left out); SOTC_DIFFTEST_DUMP=1
(the next 16 differing bytes too).

Native loops charge no EE cycles: where the game measures its own time
(the garbage walk's budget) the native loop does more within one budget.
On the attract demo the recompiled walk never ran out of budget either
(~385 blocks a frame, 0 stopped by the clock), so the work is the same.

With these (throttled Vita3K attract demo, frames 1100-1460, alternating
the HEAD VPK and the new one, the host shared with another Vita3K):
37.6 -> 34.7 ms a frame (game 34.0 -> 31.6, hooks 3.54 -> 3.0); unthrottled
17.4 -> 16.9 ms.

Together (throttled Vita3K attract demo, frames 1100-1460, the same VPK
with SOTC_DECOMP_SKIP / PS2X_FPU_SETCSR=1 for the old code): 45.7 -> 38.0
ms a frame, game thread 42.1 -> 34.5 ms.

## Compiler level and the hooks (2026-10-21, afternoon)

The altar profile after the collision work (throttled Vita3K, HEAD
5464e5a: 68.6 ms a frame, game thread 57.6 + hooks 10.9) is flat: the
recompiled game code is 57% of the game thread (33 ms) over ~1 750
functions, nothing above 1.1%; ours (hooks and decomp_*) ~29%. Grouped by
caller (macOS `sample` of the same scene, inclusive, waits left out): the
display walk DispDispObjGroupModel ~28% (modelDispHook half of it, the
particle emitters 4%), the character control / AI (st_controltarget,
010CABA0) 8%, the animation (ExecCharaGeometry through the script VM, IK,
GetMotion*) ~10%, the camera (doit_cameraman) 5%, the display list DMA
(iosDlTrans) 5%, the script data / life bar 6%.

**The recompiled code's GPR access** (ps2_runtime_macros.h). GPR_U32 /
SET_GPR_* went through the context's __m128i registers: every read moved
the value from a NEON register to an ARM one (a stall on the Cortex-A9)
and every 32-bit write loaded the whole 128-bit register, merged its low
half and stored it back. They are now plain 32 / 64-bit loads and stores
of the register's low half (memcpy: no aliasing question). With them the
guest RAM test leads each READ / WRITE macro with a hint (PS2_EXPECT:
main RAM on the straight path; GCC had laid the RAM path out of line).
isysGroupForNext: 1 001 -> 864 instructions, 80 NEON loads / stores -> 2,
11 NEON -> ARM moves -> 0. Measured on the 40 unity batches holding 69% of
the recompiled time (rebuilt by hand, the rest left as it was): altar
67.2 -> 61.2 ms, attract demo 33.7 -> 31.8. Every recompiled file includes
the header: the change needs the full game rebuild (Mac 11 min, Vita ~70
min, ccache has nothing for it).

**The hooks:** modelDispHook read seven function-local statics per packet
(debug switches; each a guard check with a barrier on ARM): read once per
call now (ModelDispEnv); asyncBuilds() likewise, per draw. The part test
in modelDispState makes the eight corners' CLIP flags four at a time in
vectors (no move to ARM registers per corner); SOTC_MODELDISP_CHECK=1:
160 000 checks on the route, 30 000 on the attract demo, 0 mismatches.
IdSet (the renderer's id sets, mostly asked for ids they do not hold)
stays at most half full: a miss walked ~8 slots at 3/4. With the skeleton
change above: altar 68.4 -> 67.2 ms.

**PGO** works with the VitaSDK, with care: its libgcov is built without
libc (no file I/O: __gcov_init / __gcov_dump are empty, the value
profilers missing). Instrument with `-fprofile-generate=ux0:data/celosia/pgo
-fprofile-update=single -fno-profile-values -fno-profile-reorder-functions
-fprofile-info-section=gcov_info`, link with `-fprofile-generate
-Wl,--undefined=__gcov_info_to_gcda`; SOTC_GCOV_DUMP_FRAME=<n> writes the
.gcda files through __gcov_info_to_gcda (sotc_native.cpp gcovDump). The
dump leaves runs / sum_max at 0: set them (runs 1, sum_max the largest
arc count of all files) before `-fprofile-use=<dir> -fprofile-partial-training
-fprofile-correction -fno-profile-values -fno-profile-reorder-functions
-fno-vpt -Wno-missing-profile -Wno-coverage-mismatch`. Compile gen and use
from the same directory with the same relative -o (the .gcda name is the
object's mangled path). The instrumented build runs at 87 ms at the altar
(throttled), so training in Vita3K is fine. Trained on the altar route and
the attract demo, applied to the same 40 batches and the overrides: altar
60.3 / 60.6 -> 59.4 / 59.0 ms, attract demo 31.7 / 31.7 -> 31.1 / 31.2
(~2%). Not worth two full rebuilds and a stale-profile risk at every game
code change now; the render thread is the next limit anyway (below).

**-O3 / LTO on the runtime and the overrides** (no game rebuild): -O3
(CMakeLists: ps2_runtime and game/overrides; vu1rec stays -O2) took the
render thread 57.5 -> 52.8 ms at the altar and the attract demo 30.1 ->
29.5 ms; LTO of the two (-flto, gcc-ar archive) gave nothing (58.8 / 30.0).

**All together** (full rebuild, throttled Vita3K, alternating with the HEAD
5464e5a VPK): shrine altar 67.8 / 70.2 -> 57.0 / 57.1 ms a frame (game
thread 57-59 -> 47, hooks 10.8 -> 9.7), attract demo 33.9 / 34.0 -> 29.5 /
29.5 ms. Mac: the route reaches the plain, game clock 1.000x;
SOTC_DIFFTEST=all over the route and the attract demo: 0 mismatches but
isysWakeupSubThread's known noise (above). ELF 124 -> 113 MB on the Vita.

**What limits the altar now: the render thread** (~52-57 ms, the game
thread ~57). Its profile (thread 246): sceGxmDraw 26%, sceGxmSetVertexStream
9.5%, sceGxmSetFragmentTexture 9.5%, vitaGL's _glDrawArrays 7.8%, our
Renderer::render loop 10% and uniform4 7.7%, memcmp in vglCreateVertexProgram
4.7% (per draw), the unordered_map modulo 4.3%. The sceGxm* part is Vita3K's
HLE and may cost less on the hardware; fewer draws (~2 000 at the altar) and
fewer uniform uploads per draw are the levers there.


## Next candidates (profile 2026-10-20 evening, throttled attract demo, after the above)

The profile is flat: nothing above 2.5%.

- decomp_skeleton::bone 2.45% (ours: the VU0 block; PS2_V* clamps every
  operand), decomp_layout::mul 1.5% (ours), particleUpdate 1.6% (ours).
- ExecCharaGeometry 1.8%, manager_sub_010AC6C8 1.7% (a 15-way jump table
  of curve evaluations), ClipCollisionDirectCallBack 1.5% (the particles'
  damage test), manager_sub_0108D730 1.45%.
- texTransResolve 1.0%, manager_sub_010F3CF0 1.0%, iosDlChainTailCurrent
  0.95%, manager_sub_010AB618 0.9%, 010F3FB0 0.9%.
- Host: ptclVtxHook 1.9%, Vif1Walker::feed 1.4%, sceClibMemcpy 0.8%
  (copies over 512 bytes: the scheduler's context copies).

## Older candidates (profile 2026-10-17, native route)

- The PS2 display lists the game still builds for models the native
  renderer draws itself (manager_sub_01097xxx, iosDlChainTailCurrent,
  transMicroCode, and the VIF1 chain walk in iosDmaSendSChainPath1, ~6%).
- modelDisp's 25% is mostly host code (native renderer, skinning): profile
  it on the host side.
- Collision: ClipCollisionDB, GetLayoutClass, GetCollisionDBAttributeAdr.
