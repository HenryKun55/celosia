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
| decomp_prefetch.cpp | (round 2: ClipLayoutObjModel / DispLayoutObjModel wrapped, the next object's lines prefetched; since round 7 the lines are asked for from decomp_walk.cpp's natives) | route 23 900 + 9 600, attract 4 700 + 2 600 (EVERY=20), 0 mismatches | see "The display walk's memory misses" below |
| decomp_light.cpp | the light pass: manager_sub_010F4FB8 (its three walks of the sort list), 010F4A18, ExecGlobalLightMan, 010F8360, 010F5F60, SetWorkFromGlobalLight, GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix | attract 800-6 500 each, route 300-6 700 each, 0 mismatches | see "The light pass" below |
| decomp_curve.cpp | manager_sub_010AC6C8 (curveEval: a motion channel's key), manager_sub_010B81F8 (a motion's keys, curveEval called natively) | attract 80 800 + 2 100, route 86 300 + 1 600, 0 mismatches | formats 0, 3-9, 12, 13 natively (all the attract demo and the route use); 1, 2, 10, 11, 14 recompiled (SOTC_CURVE_STATS=1 counts them). See "Round 3" below |
| decomp_quat.cpp | GetQuaternionByAxisDirZY, GetDifferencialQuaternionWithNoRegularize, SetQuaternionByAxisRotateVWithNoRegularize, GetEulerYXZFromQuaternion, iosGetATan2fExact, atan2f / __ieee754_atan2f, atanf | attract 4 500-32 900 each, route 2 500-28 600 each, 0 mismatches | each natively into the next (newlib's atanf / atan2f ported with the FPU_* ops); asinf stays a guest call |
| decomp_bridle.cpp | gamecore_sub_013978E8, gamecore_sub_013976B8 (the horse's rein cloths against its body: three cylinders / the head's) | attract 600 + 600, route 400 + 400, 0 mismatches | iosTransMatrixF / 01B96D0 stay guest calls |
| decomp_skinpal.cpp | manager_sub_010F3CF0 (a skinned model's matrix palette) | attract 1 500, route 800, 0 mismatches | the palette loop natively, then the recompiled code from 0x10f3df0 |
| decomp_texreset.cpp | manager_sub_010AB618 (the texture list's transfer reset) | attract 1 300, route 1 100, 0 mismatches | ~1 000 nodes a frame |
| decomp_texframe.cpp | texReset (the frame start's texture reset: LOD scale, slot arrays, the list walk with texAnim) | attract 400, route 400, 0 mismatches | see "Round 6" |
| decomp_group.cpp | isysGroupForNext, isysGroupForNextForce, InitLayoutObjMatrix | attract 13 000 / 1 900 / 400, route 62 300 / 45 800 / 400, 0 mismatches | the group iterator inline in the frame start's matrix flip |
| decomp_dl.cpp | AddSortObj, iosDlChainTailCurrent | attract 22 800 / 121 500, route 3 600 / 89 200 (SOTC_NATIVE=1, EVERY=5), 0 mismatches | the sort buckets' insert, the display list's tail packet |
| decomp_wind.cpp | manager_sub_01145F78 (the wind zones at a point, sorted by priority) | attract 3 300, route 1 300 (SOTC_NATIVE=1), 0 mismatches | the three weight functions stay guest calls |
| decomp_walk.cpp | DispDispObjGroupModel (the display walk from its top), ClipLayoutObjModel, modelClipSolid (clipByMinMaxCube inline), DispLayoutObjModel (DispCloth inline), 010F3A08 (solidDisp), 010F3EB8 (the heat haze strips, glColor4f / glTexCoord2f / glVertex3fv / iosgluGetViewCoordPosition inline), 0108D730 (glVertex's kick), glVertex3fv | walk: attract 1 200, route 1 100 (SOTC_NATIVE=1, EVERY=5, to frame 4800), with the native renderer's draws compared too; children (walk recompiled): attract 2 900-41 500 each, route 2 200-43 900 each; 0 mismatches | see "Round 7" below |
| decomp_ptcldraw.cpp | particleDraw's loop: 01142030 (the fade) inline, iosGetSinCosf called straight (round 7) | attract 4 200, route 17 700 (SOTC_NATIVE=1), 0 mismatches | no guest call a particle |
| decomp_ptclupdate.cpp | the particle step from its top: doit_particle_geodisp, 0113AAE0 (emitter list, dead emitters freed), 0113B610 (an emitter's frame), 0113B928 (its particles: types 2 / 3's local matrix, wind, particleUpdate called straight, new particles from the free list) | attract (EVERY=5) 5 500 / 1 400 / 6 600 / 8 000, route 27 000 / 27 000 / 32 400 / 38 900; also SOTC_NATIVE=2 (route, EVERY=10) 14 400 / 1 600 / 16 000 / 17 600; 0 mismatches | round 8; the VU0 blocks are the recompiled statements lifted onto locals |
| decomp_math.cpp | iosGetSinf, iosGetCosf (round 10) | route (EVERY=200, SOTC_NATIVE=1 / 2) 3 400 + 2 900 / 3 700 + 3 200 checks (640 000+ / 700 000+ calls), attract (=1 / 2) 1 100 + 400, 0 mismatches | the VU0 polynomial iosGetSinCosf shares; the player's life bar ~400 calls a frame, texAnim's waves |
| decomp_texsearch.cpp | texSearchMopByName (round 10) | route 200, attract 1, 0 mismatches | the texture list walk with the names compared in place (was one guest strcmp a node) |
| decomp_collision.cpp | the sphere finder's tree walk, manager_sub_01128F38 (round 10) | route SOTC_NATIVE=1 3 000 checks (297 500 calls), =2 1 200 (234 400), 0 mismatches; not called by the attract demo | low child by recursion, high by the loop; the finder's native leaves called straight |

The wake is a round trip: the check runs the sub-thread until it sleeps,
and that code writes values that differ between two runs of the same code
(object state at 0x14656xx, 0x15b68c4, 0x15b8bd0 ...). A control run
(SOTC_DIFFTEST_CONTROL=1, the recompiled code in both legs) gives the same
mismatches at the same addresses at the same rate (17 000 checks, ~700
each); every register (all 128 bits, FPU, hi / lo / sa) matched in all.

## Progress count (2026-10-23)

`tools/decomp_status.py` counts what is native, from 0 to 100%, like
docs/port_status.json does for the hardware. The functions are the
recompiler's (build/runner/register_functions.cpp), sized from
build/recomp/game/functions.csv. Native means the runtime implements it
(ps2_recompiled_stubs.h) or an override replaces its table entry: a Mac run
with `PS2X_REPLACED_DUMP=<file> PS2X_EXIT_AT_FRAME=<n>` (default env, no
profiler or difftest wrappers) writes those addresses at exit.

    PS2X_REPLACED_DUMP=$PWD/build/agent-scratch/replaced.txt \
      build/agent-scratch/r5/attract.sh build/agent-scratch/ds 1200
    tools/decomp_status.py --replaced build/agent-scratch/replaced.txt \
      --calls build/calls_attract.txt build/calls_route.txt

It writes docs/decomp_status.json (summary, areas, every native function
with its source file) and docs/decomp_status.svg. Areas come from the
native's source file, else from keywords in the symbol name, so the many
bare `manager_sub_*` functions still recompiled fall in "Other". First
count, HEAD 14b3e13 + round 5 (off): 4.24% by code size, 4.42% by function
count, 34.1% by calls (SOTC_CALL_COUNT dumps of the attract demo and route).
Round 6: 4.31% by code size, 4.50% by function count (406), 39.6% by calls
(the same call dumps), 2.15% of the game's own code. Round 7: 4.51% by code
size, 4.57% by function count (412), 40.3% by calls, 2.39% of the game's
own code (010F3FB0, the haze strips' second half, is native too but counts
as recompiled: only 010F3EB8's entry is in the table). Round 8: 4.65% by code
size, 4.61% by function count (416), 40.6% by calls, 2.55% of the game's own
code. Run it again after
every round and commit both files. (A function an override hooks counts as
native too: iosDlPushLayer / iosDlPopLayer are sotc_native.cpp's hooks
around the recompiled code.)

**Relink warning (fixed in round 6).** `./celosia recompile` relinks
(tools/build_elf.py) before recompiling. On 2026-10-23 a relink from the same
SOTC.elf gave a different split: curveEval's jump-table cases (0x10AC700...)
and others became separate functions, and the attract demo hung at game frame
~370 in the game's own part-tree walk (manager_sub_01099A08). The cause: the
merge of false splits kept as function starts ("pinned") every 6-8 digit hex
literal found anywhere in game/overrides/*.cpp, comments and profiler tables
included. Round 3's decomp_curve.cpp names curveEval's cases in a `switch`
(0x010AC700, 0x010AC8A8 ...), sotc_prof.cpp names 0x010F3B58 / 0x010F3DE8 /
0x013C9180, decomp_clothdyn.cpp's comment 0x1137960: each pinned a start
that the merge had folded away before, so the split followed the override
sources, not the ELF. The inputs were otherwise identical (SOTC.elf,
scus_symbols.txt, the captures, dynamic_imm.txt, boot_functions.csv). Now
build_elf.py pins only game/pinned_starts.txt: the four starts the good split
depends on (sotc_native.cpp's return-address ranges), found by relinking with
nothing pinned and diffing against the good split; `!addr` lines are
registered addresses merged on purpose (isysSleepSubThread). It warns when an
override registers (SOTC_DECOMP_*FUNCTION, registerFunction) an address the
merge folded away. Proof: build_elf.py + gen_config.py into a scratch
directory gave SOTC.elf, functions.csv, merged_entries.txt, symbols.txt,
boot_functions.csv and dynamic_imm.txt identical to
build/agent-scratch/r5/stage-off; config.toml differs only by gen_config's
explicit `register_cache = false` (the recompiler's default); the recompiler's
output from it is identical file for file (diff -r empty) to stage-off's and
to build/recomp/game/output. Still run the attract demo past frame 900 after
any recompile.

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

## Round 4: the macros themselves (2026-10-22, late night)

A global lever: the generated code's VU0 ops for all ~16 000 recompiled
functions (and the natives' vu::*N), in ps2_runtime_macros.h. Each PS2_V* op
ran ps2VuNormalize (11 NEON ops) on both operands and the result; on the
Vita sse2neon also built every `_mm_set_epi32` mask through a stack array
(spilled, reloaded, then `vshr` in `_mm_blendv_ps`) and every lane broadcast
`_mm_shuffle_ps(v, v, 0x55)` as a two-register vtbl from the literal pool.
iosApplyRotMatrix's VU0 block: 213 -> 84 NEON instructions.

- **ps2VuNormalizeFast** (every platform): Inf / NaN -> signed FLT_MAX as two
  integer minimums on the pattern (signed min 0x7F7FFFFF clamps the
  positives, unsigned min 0xFF7FFFFF the negatives), then a zero exponent
  keeps only the sign: 5 ops. The same function: all 2^32 patterns.
- **ARMv7 (PS2X_VU_NEON_A32)**: Advanced SIMD always runs with the standard
  FPSCR value (flush-to-zero in and out, a zero of the same sign, default
  NaN, round to nearest). A denormal operand is read as the signed zero the
  normalisation would have made, and a result is never denormal; operands
  clamped and finite give no NaN. So PS2_VADD / VSUB / VMUL (and
  ps2VuResult, vu::mulN / addN / subN) only clamp Inf / NaN: 2 ops an
  operand and 2 for the result. AArch64 / x86 keep the full
  normalisation (FZ there is the thread's FPCR / MXCSR).
- **sse2neon on GCC ARMv7**: `_mm_set_epi32` a vector literal (constant masks
  fold), `_mm_blendv_ps` from the mask's sign bits with constant masks folded
  (all ones is b), broadcast shuffles as `vdup` per operand combined so that
  `x ^ x` folds when both are the same register (the generated code always
  shuffles a register with itself). GCC 15 miscompiles
  `vcombine_f32(vdup_lane_f32(..), ..)` (the high half dropped): avoided.
- **FPU** (kept off, switches only): the add's operand alignment without
  branches (PS2X_FPU_ALIGN=1 a 64-entry mask table, =2 shifts) and a
  branchless compare key (PS2X_FPU_FAST_CMP=1) are exact (every exponent
  pair; all 2^32 keys) but ran slower in calibrated Vita3K (adds 104.8 ->
  114.9 / 135.6 ns, compares 78.7 -> 96.1 ns): the branches stay.
- **PS2X_VU_MACROS_OLD=1** (compile time) builds the old macros.

**Checks**: ps2_macro_selftest.cpp (`PS2X_MACRO_SELFTEST=<millions>[x]`, on
the game thread before the game; build/agent-scratch/r4/host_test.cpp on the
Mac) compares every changed form with the old one: every pair of 4 096 edge
patterns (all exponents x mantissas 0 / 1 / 2 / half / all ones ..., both
signs) then random pairs (uniform, edge exponents, exponents near the other
operand, products at the overflow / denormal borders). Mac: normalisation
and compare key all 2^32, VADD / VSUB / VMUL / vu::*N 4.0 G lanes each (RZ +
FZ) and 1.2 G (default mode), alignment every exponent pair, FPU add / sub
4.0 G each: 0 mismatches. Calibrated Vita3K (the ARMv7 forms): 4 M random
+ the edge pairs per op (33 M lanes each), the shuffles (all 256 imm), the
blends, set_epi32: 0 mismatches. NaN + NaN of two signs in an FPU add can
come out with either NaN's sign: which one the hardware passes on follows
the register order the compiler picked for `a + b` (FPSCR DN is 0 on the
Vita), free for old and new code alike. SOTC_DIFFTEST=all over the Mac
attract demo (EVERY=200): 50 functions, 39 200 checks, 0 mismatches but
isysWakeupSubThread's known noise. Mac route: game clock 1.000x (last 12
windows). The attract demo's draw dumps differ as much between two runs of
the old binary as between old and new (not deterministic): no evidence.

**Rounding** (the self-test's report): the game thread's FPSCR on the Vita
is 0x01c00000 (RZ, FZ, DN 0): the scalar FPU_* ops truncate as the EE does,
but PS2_V* (NEON) round to nearest: on random normal operands 50% of the
VU0 products and 47% of the sums differ in the last bit from the scalar RZ
result (0% on the Mac, whose NEON follows FPCR). Native and recompiled code
agree on each platform. Truncating on the Vita would take a TwoSum-style
error term an add and a Dekker product a mul (no FMA on the A9's VFPv3),
~10-17 extra ops an op: not done.

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, one run
per side (HEAD 11f953c's VPK, then this one), env SOTC_SUBSYS_PROF=1:

| ms / frame | before | after |
|---|---|---|
| frame (wall) | 45.4 | 39.3 |
| execScript | 40.1 | 34.1 |
| ExecCharaGeometry (self) | 9.47 (4.60) | 6.41 (3.29) |
| DispDispObjGroupModel (self) | 14.4 (5.08) | 12.7 (4.50) |
| doit_particle_geodisp | 4.14 | 3.89 |
| skeletonPose | 1.43 | 0.98 |
| dispSort_010F4FB8 | 1.42 | 1.19 |

Vita3K runs float code ~1.3x slower than the console (above): the console
may gain less.

## Round 5: guest registers in host locals (2026-10-23)

The recompiler lever the earlier rounds pointed at: the generated code kept
every GPR / FPR in the R5900Context (GPR_U32(ctx, n), SET_GPR_S32(ctx, n,
v), ctx->f[n]); ctx escapes and guest memory is char data, so the host
compiler reloaded registers after guest stores and stored every result.

**ps2xRecomp register_cache.cpp** rewrites each generated function body (a
pass after FunctionEmitter, on the emitted text) so the registers live in
host locals for the whole function: rc_<n> (the low 64 bits; ctx keeps the
high half, GPR_VEC / SET_GPR_VEC merge) and rf_<n>.

- A *barrier* is any line that hands ctx to something else: callGuest /
  jumpGuest / dispatchGuestBranch, direct C++ calls, syscalls and stubs,
  runtime helpers taking ctx (VU0 microprograms, SignalException, getRegU32
  ...), throw. Before it the dirty locals are stored, the line itself stays
  the old code, after it every local is stale. Guest memory accesses are not
  barriers (their slow path never touches GPRs / FPRs; thread switches are
  thrown only from syscalls).
- Loads are lazy (where a register is first read, nothing on entry; an entry
  at a mid-function label finds every local stale); stores happen at
  barriers, returns and on the edges that need them.
- Two forward analyses over the body's labels / gotos / blocks, to a fixed
  point: *dirty* (may differ from ctx; over-approximated, storing an in-sync
  local is harmless) and *stale* (not loaded: a valid local may be dropped,
  a dirty one never). A loop head keeps dirty what its back edges bring, its
  entry edge stores the rest once (the prologue's sp / s-registers no longer
  stored at every call in the loop); another label keeps what every edge
  into it holds dirty, an edge holding more stores it. Resume entries (the
  switch on ctx->pc) go through trampolines at the end of the function that
  load what the label needs. Checkpoint returns share one cold exit.
- *Sign extension*: a must analysis of the GPRs whose local is a
  sign-extended 32-bit value (32-bit results, byte / half loads, moves,
  andi / ori / and / or / xor of such, slt); their 64-bit reads and stores
  come from the low half (RC_U64X), so GCC can drop the high half (a second
  ARMv7 register per guest register). Nothing is known at a resume entry.
- Anything unexpected (a statement in a call's failure block, a label in a
  block, a multi-line statement ...) leaves the function as the old code: 2
  of 8 790 functions (PMULTUW's two-line translation).

Switch: `[general] register_cache = false` in the stage's config.toml
(tools/gen_config.py writes `true`) or `PS2X_RECOMP_REGCACHE=0` at recompile
time gives the old generator's output byte for byte (checked: diff -r empty);
`PS2X_RECOMP_REGCACHE_FPR=0` keeps the FPRs on ctx,
`PS2X_RECOMP_REGCACHE_SEXT=0` drops the sign tracking,
`PS2X_RECOMP_REGCACHE_DEBUG=1` names the functions kept old. The recompiler
log ends with the static counts. After `./celosia recompile`, run
tools/gen_lean.py again (CCACHE_RECACHE=1 for the lean parts).

Static counts (all functions): the old code's ctx register accesses 728 697
reads + 409 253 writes; with the cache 230 840 loads + 443 743 stores
(stores are sunk to barriers / returns / edges and so duplicated per path:
fewer are executed, more are written down), sign extension known at 59 198
64-bit reads and 218 364 stores. Vita text 48.1 -> 50.5 MB (+5%), Mac
__text 56.3 -> 61.2 MB.

Two bugs found on the way, worth knowing for any change here:

- **A conditional write** (MOVN / MOVZ: `if (rt != 0) SET_GPR_VEC(...)` in
  one line) was taken as a write: with lazy loads the local could stay
  unloaded and the next store wrote host stack garbage into v0. The game
  still ran; it hung on the memory card format at boot only under
  SOTC_DIFFTEST=isysSleepSubThread (the garbage was what the native sleep had
  left on the host stack; SOTC_DIFFTEST_CONTROL=1 passed). Found by bisecting
  the generated files between old and new output (iosMcFormat), then
  `clang -Wconditional-uninitialized` over all output: 74 such reads, now 0.
  Keep that clang check in the loop.
- **Resume entries are not this function's checkpoints only**:
  isysSleepSubThread is a label inside isysWakeupSubThread's function and
  returns into the other coroutine with whole 64-bit registers; taking them
  as sign extended there broke the sleep's difftest. Nothing is assumed at a
  resume entry.

Checks (Mac): SOTC_DIFFTEST=all, EVERY=200: attract demo 50 functions,
87 200 checks, route 50 functions, 85 100 checks, 0 mismatches but
isysWakeupSubThread's known noise. The old generator against the new one:
the lean copies built from the old output (build/agent-scratch/r5/lean_old.sh)
checked against the new originals with SOTC_LEAN_CHECK (64 a run) and every
register compared (SOTC_DIFFTEST_ALLREGS=1, new; SOTC_LEAN_CHECK_ONLY=1 keeps
the lean copies out of the rest of the run, new), attract demo to frame 1800:
1 692 of the 1 847 hot functions in 27 runs. Every mismatch is either a check
whose "recompiled" leg is a hook (sotc_native overrides: the DMA copies, the
CPU rap counter, gsReg ...), a kernel wrapper that cannot run twice
(isceGsPutIMR), or the state noise SOTC_DIFFTEST_CONTROL=1 reproduces with
the new code in both legs (0x14656xx / 0x15b8bc0, pad and frame timers: the
same functions, the same addresses). Two runs (entries 548-640: iosWakeupThread,
iosCallBackThreadMgr ...) hang because the check runs thread calls twice;
they hang in CONTROL mode too. Mac route
with the same input script: the same places at the same frames as HEAD
(altar, shrine stairs, the plain with Agro), game clock 0.994x (HEAD 0.993x
in the same conditions, last 12 windows). Vita3K boots into the attract demo
with the same picture.

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, env
SOTC_SUBSYS_PROF=1, one run per side (HEAD's VPK twice: 39.3 / 39.3):

| ms / frame | HEAD 14b3e13 | register cache |
|---|---|---|
| frame (wall) | 39.3 | 39.0 |
| execScript | 34.1 | 33.9 |
| ExecCharaGeometry (self) | 6.44 (3.30) | 6.38 (3.27) |
| DispDispObjGroupModel (self) | 12.67 (4.49) | 12.48 (4.35) |

Small. The intermediate builds say why: with every register loaded on entry
and reloaded after each call (first version) the game ran 3.5% *slower*
(40.7 ms): the reloads lengthen live ranges, GCC spills, and a loop with a
call re-stored the prologue's registers at every call. Lazy loads brought it
to 39.6, the loop-entry stores and sign tracking to 39.0. Profile
(VITA3K_PROFILE, game thread, the whole run): the recompiled functions
41.2% -> 40.2% of HEAD's samples (-2.4% of their time); the rest unchanged. On the Cortex-A9 an L1 load
or store to ctx is cheap and GCC already forwarded stores to loads where no
guest store came between (most of the old code's ctx traffic was stores,
which the store buffer absorbs); what the cache removes is mostly that.
Vita3K's JIT pays for each guest memory access, so the console should not
gain more than this; possibly less (5% more text).

Build: a full Vita rebuild with the cache took 22 min wall (11 900 s CPU,
10 cores); its critical path is unity_244 (~19 min), which holds the 5 MB
register_functions.cpp: the function table, the same file byte for byte
with or without the cache (the next slowest batch ~2.5 min). The recompiler
pass itself adds ~1 s.

What could still pay at this level: fewer stores at calls for the
callee-saved registers (s0-s7, sp: the callee preserves them; needs a proof
that every callee does, coroutine switches do not), keeping sign-extended
registers as 32-bit locals outright, and the FPU / VU0 state (fcr31,
vu0_vf / ACC / Q) in locals the same way.

## Round 6: the relink, the frame's waits, small hot leaves (2026-10-23)

**The relink** (above, "Relink warning"): build_elf.py pinned every hex
literal of the override sources; now game/pinned_starts.txt. A relink from the
same inputs gives the known-good split byte for byte, and the recompiler's
output from it is identical to build/recomp/game/output.

**Waiting on the vblank.** The [prof] line now ends its header with the game
thread's waits (ps2_load's count: the scheduler's idle and pacing waits, a
guest thread blocked until the next VBlank or timer) and the guest VBlanks a
frame: `wait W ms, V vblanks/frame`. Calibrated Vita3K, attract demo frames
600-4800: wait 0.1-0.2 ms a frame (0.45 averaged over the whole run, the
loads included), 2.17 VBlanks a frame at 39 ms; the route (Vita3K,
the same input script as the Mac's, the save copied onto the card for the
run): 0.0-0.3 ms in 3D. The frame is all work. The game does not lock to
VBlanks either: windows at 28.9 ms run at 1.73 VBlanks a frame (game time
steps by the clock: the T0 overflow accumulator), so any cut shows directly.
To hold 2 VBlanks (33.3 ms) the work has to drop by ~5.8 ms on the attract
demo (39.1 -> 33.3) and ~12.7 ms on the route's walk from the altar (46.0).

**The leaves.** A VITA3K_PROFILE sample of the attract demo (game thread,
before this round; build/agent-scratch/r6/v3k-before-prof-named.txt) ranks
the recompiled functions by their own time: manager_sub_0108D730 (glVertex:
immediate-mode vertices with VU0 clipping, 1.41%), ExecCharaGeometry 1.10%,
DispDispObjGroupModel 0.86%, 010F3FB0 (strips) 0.76%, 010C7B28 0.70%,
reflectionBegin 0.64%, 013B8DC8 0.63%, modelClipSolid 0.60%, texReset 0.59%,
010C6238 0.58%, isysAddGroupWithLinkParam / UpdateLayoutClassSimpleGeometry
0.57%, 01145F78 / iosDlChainTailCurrent 0.55%, 0113B928 0.51%,
isysGroupForFirst 0.50%, isysGroupForNext 0.48%, iosDlPopLayer 0.41%,
iosDlPushLayer 0.37%, AddSortObj 0.35%. Flat: nothing above 1.5%.

- **decomp_texframe.cpp** texReset (graphFrameInit, once a frame): the
  slot arrays cleared with memset, the ~330-node texture list walked natively
  (four dependent loads a node), texAnim (native) called per node from it.
- **decomp_group.cpp** isysGroupForNext / isysGroupForNextForce (every group
  walk: ~260 + ~190 calls a frame on the route) and InitLayoutObjMatrix (the
  layout objects' double-buffered matrix flip, the iterator and
  GetLayoutClass's plain cases inline).
- **decomp_dl.cpp** AddSortObj (the display sort's bucket insert, ~25-60
  calls a frame) and iosDlChainTailCurrent (a DMA tag behind the current
  layer's tail, ~75-130 calls a frame).
- **decomp_wind.cpp** manager_sub_01145F78: the wind zones at a point (80
  slots, the zones sorted by priority); the zones' weight functions stay
  guest calls.
- Not done: iosDlPushLayer / iosDlPopLayer. sotc_native.cpp hooks them for the
  native renderer's layer stack, and sotc_decomp::install runs after the
  hooks, so a registered native replaces the hook (the first build of this
  round did, unnoticed by the difftest: the hook's state is host-side). Any
  address an override hooks needs the hook to call the native instead.
- Left for the next round: glVertex (the biggest leaf, a state machine over
  the primitive type with VU0 clip flags and Q divisions), the strips
  (010F3FB0), 010C7B28 / 010C6238 (character), reflectionBegin.

Checks (Mac, EVERY=20; AddSortObj / the tail / the wind also with
SOTC_NATIVE=1 as on the Vita, EVERY=5, where AddSortObj runs ~25-60 times a
frame): attract demo and route to frame 4800, 0 mismatches (counts in the
table above). Mac route: game clock 0.998x and, final binary, 0.997x (last 12 windows; a run while the host was loaded gave 0.76x and is not counted). Attract demo past frame 1500 (Mac), to 7900 (Vita3K).

Calibrated Vita3K (MHZ=240 CORES=3), one run per side (this round's prof
change in both VPKs; before: HEAD ed10257's code), env SOTC_SUBSYS_PROF=1:

| ms / frame | before | after |
|---|---|---|
| attract 600-4800: frame (wall) | 39.4 | 39.1 |
| execScript | 34.18 | 33.87 |
| task 0x1346318 (frame start) self | 1.38 | 1.28 |
| DispDispObjGroupModel (self) | 12.60 (4.47) | 12.60 (4.37) |
| ExecCharaGeometry (self) | 6.37 (3.34) | 6.38 (3.22) |
| doit_game_camera (self) | 2.05 (1.46) | 1.63 (0.98) |
| route 1900-3400 (the walk from the altar): frame | 46.3 | 46.0 |
| route 600-4800: frame | 38.7 | 38.4 |

Under 1%, about the run-to-run spread of one run per side (particleUpdate
alone moved 2.35 -> 2.69 between the runs with the same code): these are
small leaves, ~0.2 ms each before, and a native leaf called from recompiled
code still pays the table call. The frame's 6 ms to 2 VBlanks will not come
from leaves this size; the next lever is a whole subsystem natively from its
top (the display walk's DispDispObjGroupModel loop with its callees, the
character's ExecCharaGeometry body), as the collision work (2026-10-21) did for the segment test.

## Round 7: the display walk natively from its top (2026-10-23, evening)

Round 6 showed a flat profile, so this round takes a whole subsystem natively
from its top, as the collision work did: the display walk,
DispDispObjGroupModel (12.6 ms of the calibrated attract frame, 4.4 ms of it
in unprobed callees). A Vita3K run with probes on its callees (SOTC_SUBSYS_PROF_EXTRA,
build/agent-scratch/r7/r7-head-probe-err.txt) split it: modelDisp (the native
hook) 3.2, the particle display (doit_particle_geodisp) 3.9, the light pass
1.2, the heat haze strips (010F3EB8) 1.3 for 2.3 calls a frame,
CalcLightLayoutObj 0.8, cloth 1.0, modelClipSolid 0.5, the walk's own
recompiled code and the clip / draw callbacks ~0.6.

**decomp_walk.cpp** (see its header):

- DispDispObjGroupModel: the group loop (the billboard and camera-link VU0
  blocks, the clip / visibility / sort-key logic, the copy into the second
  sort list, the draw loop) natively. GetLayoutClass's plain cases,
  GetCharacterClass, GetGeoDispClass, GetLayoutDispObjGroup, isysGroupForNext
  and the sort lists (ClearSortList, GetFirstSortObj, GetNextSortObj) inline;
  the class callbacks (+0xd8 / +0xdc / +0xd4 / +0xd0 / +0xcc0) are guest
  calls, or straight C++ calls when they are this file's natives
  (G::callNative: only when the table holds the native, so a difftest
  check, SOTC_DECOMP_SKIP, SOTC_AB's old half or a profiler probe on the
  child still sees every call).
- ClipLayoutObjModel, modelClipSolid (the far cull, the LOD level with its
  hysteresis, clipByMinMaxCube's VU0 CLIP of the root box inline),
  DispLayoutObjModel (DispCloth inline), 010F3A08 (the parts' matrices).
- The heat haze (010F3EB8 / 010F3FB0): the segments' bone inverses and
  screen tests natively, glColor4f, glTexCoord2f, glVertex3fv's own part
  (0108D358) and iosgluGetViewCoordPosition inline; 0108D730 (glVertex's
  kick: the vertex through view / screen / clip matrices, CLIP flags, RGBAQ,
  ST / UV, XYZ2 / XYZ3 by the primitive's clip state into the GIF packet)
  native and registered, with glVertex3fv, for every other gl* user.
- decomp_ptcldraw.cpp: the particle loop's two guest calls a particle
  (01142030, the fade, inline; iosGetSinCosf called straight).
- decomp_prefetch.cpp no longer registers anything: the natives ask it for
  the next object's lines (aheadClip / aheadDisp).

Guest calls keep the context as the recompiled code has it at that call:
the callee-saved registers, the prologue's saves in the frame, f20-f28 where
the original holds values there. A guest callee that blocks (isysObjWaitSema
is a real semaphore) throws to the scheduler; the thread then goes on in
the recompiled function at that call's return label, which finds what it
expects. The relocated jals into the boot ELF read their target from the
code. Rare cases (a pointer outside main RAM, a jump-table entry the code
does not know) go to the recompiled code at a loop head or a return label
with its registers set, or from the entry before anything is written.

One bug the difftest caught: a haze segment skipped by its distance test
leaves f0 = r_b^2 (the dividend), not the quotient (1 581 mismatches in
3 900 checks, all on f0).

**A picture check that does not need two identical runs.** Two runs are not
frame-identical (the draw dumps of two runs of the same code differ), so
SOTC_DIFFTEST_DRAWS=1 compares the native renderer's output inside each
check: native_renderer::setDrawTap hashes every 3D draw (mesh, the whole
DrawState, the matrix, the lights) of each leg and the two lists are
compared (per-frame mesh ids, a tag and a counter, by their tag; 2D draws
are batched and flushed wherever the next draw falls, so they are left out:
their content is guest packets, which the memory compare covers; =2 adds
them, the per-frame meshes' vertices and the skin palettes, with the
expected noise of once-a-frame uploads). The control (SOTC_DIFFTEST_CONTROL=1)
gives 0 of 100; the walk, SOTC_NATIVE=1, SOTC_ASYNC_BUILDS=0, EVERY=5 to
frame 4800: attract 1 200 checks / 239 667 draws, route 1 100 checks /
1 029 614 draws, 0 differ.

Checks (Mac, 0 mismatches each): the walk whole (its children native inside
it), SOTC_NATIVE=1, attract and route to frame 4800, 1 200 / 1 100 checks;
the children with the walk recompiled (EVERY=10, to 4800): attract
ClipLayoutObjModel 23 300, DispLayoutObjModel 17 100, modelClipSolid 21 200,
solidDisp 15 700, the haze 2 900, glVertex's kick 41 500, particleDraw 3 600;
route (SOTC_NATIVE=1) 43 900 / 19 000 / 42 400 / 18 800 / 2 200 / 39 700 /
9 300; glVertex3fv 26 300 (the haze left recompiled, its caller); the haze
and particleDraw also in both renderer modes. Mac route game clock 1.005x
(last 12 windows). Attract demo to frame 4800.

Calibrated Vita3K (MHZ=240 CORES=3), env SOTC_SUBSYS_PROF=1, one run per
side, HEAD fce6b0c's VPK (build/agent-scratch/celosia-r6.vpk) then this one
(celosia-r7.vpk), the host otherwise idle; the route with the Mac's save on
the card and the same input script (build/agent-scratch/r7/v3k_ab.sh). The
first HEAD route run took another path through the game from frame ~1440
(100 ms windows from 4440: another scene) and was run again; the second
follows the same timeline as this round's run window for window (and round
6's log: the load at 2280-2520, the walk from 3000).

| ms / frame | before | after |
|---|---|---|
| attract 600-4800: frame (wall) | 39.16 | 38.33 |
| execScript | 33.98 | 33.07 |
| DispDispObjGroupModel (self) | 12.60 (4.44) | 11.74 (3.69) |
| particleDraw (self) | 2.20 (1.18) | 2.08 (1.03) |
| route 1900-3400 (the walk from the altar, three load windows in it): frame | 46.03 | 45.48 |
| DispDispObjGroupModel (self) | 14.05 (4.04) | 13.35 (3.34) |
| route 3000-4800 (3D only): frame | 39.78 | 38.99 |
| DispDispObjGroupModel (self) | 13.06 (4.51) | 12.12 (3.76) |
| route 600-4800: frame | 38.31 | 37.88 |

The steps on the way (single attract runs, the same procedure): the walk,
the callbacks and modelClipSolid 39.1 -> 38.87 (DispDispObjGroupModel
12.60 -> 12.28); the haze strips natively 38.65 (12.05); the particle
loop's inlines and glVertex's kick 38.37 (11.75).

About 0.8 ms (2%) on the attract demo and 0.6-0.8 ms on the route: the
walk's own recompiled code, its clip / draw callbacks and the haze were ~2
ms of the 12.6 and run ~2x faster natively, which is what native code gave
in every earlier round. The frame is still ~5 ms (attract) and ~12 ms
(route walk) from 2 VBlanks: the time under the walk is in the native hook
(modelDisp 3.1 / 6.8 ms on the route), the particle display and the light
pass, already native or host code, and spread over the rest of the frame
(ExecCharaGeometry 6.4-9.2, particle update 2.7, camera 1.7, the DMA chain
walk 2.0).

Where the walk's time is now (wip VITA3K_PROFILE, build/agent-scratch/r7/v3k-wip2-top.txt):
the native hook (modelDispHook 3.5% of the samples, modelDispState, packetId,
surfaceInfo), the particle display (ptclVtxHook 1.2%, particleDraw's
recompiled tail, the per-emitter 0113B610 / 0113AAE0 recompiled), the light
pass natives, iosDlLockObj -> isysAddGroupWithLinkParam (a group insert with
the thread-id lock, 0.5%, once per drawn object), UpdateLayoutClassSimpleGeometry
(0.5%, 42 calls), CalcLightLayoutObj (0.35%), iosDlPushLayer / PopLayer
(sotc_native's hooks, 0.6%), reflectionBegin (0.5%). The profiler itself:
SOTC_SUBSYS_PROF's clock reads (sceKernelGetProcessTimeWide 6.3% of the
samples, closeAt / Timed / the TLS lookups ~3% more) are in every number of
these tables, before and after alike.

**The rest of the walk (plan):**

1. The particle display from doit_particle_geodisp down: 0113AAE0 (the
   emitter list), 0113B610 (per emitter: camera distance, LOD, 0113B928),
   particleDraw's tail from 0x113eafc (the matrix and the ptclman packet
   builders, which sotc_native hooks: call the hooks, never replace them) and
   ptclVtxHook's host side: ~3.9 ms of the attract frame, the biggest piece
   left under the walk.
2. modelDispHook's packet loop (host code, 1.6 ms attract / 4.1 ms route of
   self time): a per-instance list of the packets it drew last frame (LOD,
   part, surface, id), rebuilt only when the header / LOD / part visibility
   change, instead of the packet walk, packetId and the memo lookups each
   frame; this is the route's largest single item.
3. iosDlLockObj with isysAddGroupWithLinkParam natively (its thread-id lock
   is held by the walk already: the owner check is a counter there), and
   CalcLightLayoutObj's matrix work (SetGeoDispClassLightColMan's callees are
   native already).
4. UpdateLayoutClassSimpleGeometry (the billboard / link objects' geometry,
   42 calls a frame).
5. (This round's second target, not started.) ExecCharaGeometry's body
   (0x10bf8f0, 3 944 bytes, ~28 callees): its own recompiled code is ~1% of
   the samples (0.4 ms); its 3.3 ms of probe "self" time are its unprobed
   callees (010C7B28, 010C6238, 010C4CA8, 010C6A88 ...), each 0.5% or less:
   the body natively pays only with those callees natively under it.

The hook hazard (round 6): none of this round's addresses is one
sotc_native.cpp hooks; what it hooks on the way (modelDisp,
iosgluGetCameraContext, shadowDisp, meshDisp, glEnd, iosDlPushLayer /
PopLayer, the ptclman builders) the natives reach through the function
table with the original's return address (glEnd's hook tells the haze from
other gl* users by it). SOTC_STRIPS_DEBUG=1 checks the inline
iosgluGetViewCoordPosition against the guest one at every haze point
(debugging; it calls both).

## Round 8: draw plans, one draw a run, the DMA walk, the particle step (2026-10-24)

**modelDispHook's packet loop as a cached plan** (sotc_native.cpp, "Draw
plans"). For each (model header, LOD) a plan keeps that LOD's packets in the
list's order (part, surface record, data, size) and what the hook found out
about each: its id, skinned or not, its meshes there, lit. modelDispState and
the packet loop go through it instead of walking the whole packet list (every
LOD) twice a draw; the per-frame reads stay (the part tree's results, the
surfaces' flags / texture / layer / scroll through the per-frame surface memo,
the instance's matrix, opacity, lights, palette). Invalidation: each frame a
plan is used its header's packet list and surface table pointers and the
list's first packet are compared, one frame in 16 (by header) the whole list
is hashed again; a moved header (an area's heap compaction) or a LOD switch is
another plan; a packet's id is asked from packetId whenever packetId itself
would look at the packet (not drawn the frame before, its own stagger frame,
or seen rewritten in place: every frame), so the ids are packetId's; a mesh's
presence is asked again after the backend lets meshes go
(native_renderer::meshEvictions, new) and a skinned packet's after their
sweep. Plans unused for 600 frames go. The instance's lights are read once
for its draws (were 32 guest reads a lit draw). r2Check / hookStats were
function-local statics (a guard with a barrier per call on the Vita, twice a
draw): now plain pointers.

**One draw a run** (native_renderer::drawMulti, new). Consecutive packets of
one instance with the same surface record, lit alike, a plain mesh already
there and one draw a packet (no second texture, no reflection pass) are drawn
as one draw: the state made and copied once, the GL backend records one Draw
with the other meshes in a side list and the render thread expands them in
place before it plans the frame (its own merges then run as before; Metal and
the check backend loop over draw()). Taps, the draw dump and the held /
hidden tests see each mesh as its own draw. Route (Mac, to frame 4800):
4.94 M packet draws, 3.49 M fewer draw records (~70%); attract 0.87 M, 0.29 M
fewer. SOTC_PLAN_RUNS=0: one draw a packet.

**Loads.** A plan is built the first time its model is drawn; an area load
brings hundreds at once (639 plans at the route's load). New plans have a
budget of 3000 packets a frame (SOTC_PLAN_BUDGET); past it the model is drawn
by the packet walk as before and its plan made at a later frame (313 deferred
over the route), so a load frame does at most HEAD's work plus the budget.

**Checks of the cache.** SOTC_PLAN_VERIFY=<n>: every n-th frame each draw's
packets go through the plan (drawn) and through the old walk (dry: hashed by
the draw tap, native_renderer::setDrawTapOnly, not drawn) and the two lists of
draw hashes (mesh, the whole DrawState, matrix, lights) are compared. Every
frame (n = 1), SOTC_NATIVE=1, to frame 4800, with runs and the build budget
on: route 185 634 instance checks / 5 366 931 draws, attract 141 857 /
911 893, 0 differ. The GL multi-draw against single draws: SOTC_NATIVE_GL=check
(Metal one draw a packet, GL drawMulti, pictures compared every 60 frames) on
the route gives the same means with SOTC_PLAN_RUNS=1 and 0 (~0.003 per
channel, one fade frame at 15). The walk's draw compare (SOTC_DIFFTEST=
DispDispObjGroupModel SOTC_DIFFTEST_DRAWS=1 SOTC_ASYNC_BUILDS=0 EVERY=5, to
frame 4800): attract 1 000 checks / 212 928 draws, route 1 000 / 1 251 602,
0 differ, 0 mismatches; control (SOTC_DIFFTEST_CONTROL=1, attract to 2400)
500 checks / 91 214 draws, 0 differ.

**The DMA chain walk** (PS2X_DMA_WALK_FAST=0: off). With SOTC_NATIVE the VIF1
chain is still walked every frame for its GS events: ~1 200 tags and ~3 500
VIFcodes a frame in the attract demo, and the only event in it is one FINISH
a frame (PS2X_GIF_EVENT_LOG=1: 2 998 FINISH in 3 000 chains, no SIGNAL or
LABEL, with the fast paths on and off). ~680 of the tags are NEXT / CALL
links with two NOP VIFcodes: Vif1Walker::feedTagWords takes those without the
walker's loop (CODE / NUM set as the two NOPs leave them), and the GIF event
scan reads an A+D-only PACKED block's register addresses in a tight loop.
Vif1Walker::feed 3.7% -> 2.4% of the game thread (clean attract profile).
PS2X_DMA_DUMP=<n> prints the n-th chain's tags (debugging).

**The particle step from its top** (decomp_ptclupdate.cpp): doit_particle_geodisp,
0113AAE0, 0113B610, 0113B928, each calling the next straight; particleUpdate
called straight for each particle. The VU0 blocks (0113B928's matrices for
types 2 / 3, 0113B610's camera distance and depth) are the recompiled
statements lifted onto locals (build/agent-scratch/r8/tools/lift.py, from
build/runner). All paths ran in the checks (SOTC_PTCL_STATS=1, attract:
32 394 steps, 8 618 matrix blocks, 1 667 unit matrices, 34 dead emitters,
224 new particles, 0 fallbacks). It gave almost nothing: in the clean attract
profile the recompiled particle code went 2.67% -> 2.12% and the new natives
are 0.51% (the guest calls under it stay: strcmp, the blend, the wind, the
camera, 0113C1A8); under SOTC_SUBSYS_PROF doit_particle_geodisp's self time
reads 1.24 -> 1.34 ms (each child goes through the profiler's wrapper).

**Measuring.** SOTC_SUBSYS_PROF itself costs ~5 ms a frame in Vita3K (attract
600-4800: 37.5 ms with it, 32.7 without, same VPK): the clock reads around
every probed call. The [stats] lines (5 s windows, frames counted) give the
frame time without it (build/agent-scratch/r8/tools/statfps.py); the console
calibration of 2026-10-22 was made with the profiler on both sides. Without
the profiler the scripted route is not reliable in Vita3K: the menu presses are
timed in VBlanks and a faster game misses one (the attract demo starts, or
another path); giving the presses as game frames, or repeating the last CROSS
(r250), led elsewhere too. The route A/B below is with the profiler, as in
round 7 (both runs followed the same timeline: the 3D after the load at frame
2220 / 2231, window for window).

Calibrated Vita3K (MHZ=240 CORES=3), one run per side, HEAD e51649c's VPK
(celosia-r7.vpk) then this round's (celosia-r8.vpk), env SOTC_SUBSYS_PROF=1
SOTC_FRAME_LOG=1 (frames over 50 ms logged), the route with the Mac's save and
input script (build/agent-scratch/r8/v3k.sh), host idle (load 1.5-2.4):

| ms / frame | HEAD | round 8 |
|---|---|---|
| attract 600-4800: frame (wall) | 38.8 | 37.5 |
| execScript | 33.43 | 32.65 |
| DispDispObjGroupModel (self) | 12.15 (3.70) | 11.25 (3.70) |
| modelDisp | 3.41 | 2.51 |
| iosDmaSendSChainPath1 | 2.04 | 1.63 |
| attract 600-4800 without the profiler ([stats]) | 33.01 | 32.25 |
| attract 4800-9600 without the profiler | 34.71 | 33.37 |
| route 1900-3400 (the walk, three load windows): frame | 45.8 | 41.6 |
| DispDispObjGroupModel (self) | 13.57 (3.32) | 10.02 (3.31) |
| modelDisp (hook.packets self, packetId) | 6.94 (4.02, 1.15) | 3.52 (1.92, 0.11) |
| route 3000-4800 (3D only): frame | 39.4 | 37.9 |
| route 600-4800: frame | 38.2 | 35.0 |

Loads (frames over 50 ms, SOTC_FRAME_LOG): route frames 2200-2800 (the load
after the shrine) HEAD 299 such frames, 233 over 85 ms, 27.7 s in all, the
worst 372.4 ms (frame 2522); round 8 297, 24 over 85 ms, 24.4 s, the worst
340.0 ms (2529) (an earlier round 8 run: 301 / 29 / 24.8 s / 370.9). Route
2800-4800: HEAD 30 frames over 50 ms, worst 66.9; round 8 32, worst 64.0. The
whole runs' worst frames are the boot's (~580 ms) on both. Attract 600-4800:
HEAD 75 over 50 ms (worst 224.2), round 8 69 (222.0). On the Mac (route,
SOTC_FRAME_LOG_ALL=1, the same binary with the round's paths off:
SOTC_PLAN=0 PS2X_DMA_WALK_FAST=0 SOTC_DECOMP_SKIP=ptcl*): frames 2100-2700
mean 16.75 / 16.73 ms, worst 37.0 / 33.7 ms; 600-4800 worst 39.1 / 37.8 ms;
the Mac runs at the display's 60 Hz, loads barely show there.

Other checks: the particle natives' difftests above; Mac route game clock
1.000x (last 12 windows); attract to frame 6000; pictures (route shots, native
renderer, against the same binary with the round's paths off) the same scenes
(the poses differ by a frame); consecutive frames: draw dumps of frames
2440-2450 at the shrine in a normal run (no difftest; every other frame: a dump
takes the next whole frame) keep their layer counts and light sets from dump
to dump (1072-1100 draws, 7-8 distinct light sets); the flicker the user saw
was with difftest windows open, and none shows here (the dumps do not hold the
bloom's parameters).

What is still slow on the route's walk (allprobes, round 8): ExecCharaGeometry
9.0 (4.6 self), DispDispObjGroupModel's own 3.3, modelDisp 3.5, collision 1.5,
particles 2.0 + 1.3, camera 2.3, the AI 1.6, the DMA walk 1.2; the frame is
~8 ms from 2 VBlanks with the profiler, the attract demo is at 32.3 ms without
it.

**Not done this round:** iosDlLockObj -> isysAddGroupWithLinkParam,
CalcLightLayoutObj, UpdateLayoutClassSimpleGeometry (target 3);
the particle display side (0113ACB8 and the per-type writers), ptclVtxHook.

**Round 9 (plan):**

1. The load hitch (route 2200-2800: ~300 frames over 50 ms, ~240 of them over
   70): what runs in those frames (a clean VITA3K_PROFILE of a run held in the
   load; the frame log's hooks / upload columns) before any change.
2. ExecCharaGeometry's body with its callees (010C7B28, 010C6238, 010C4CA8,
   010C6A88...): the route walk's largest item (9 ms).
3. The DMA walk without walking: the chain carries one FINISH a frame and the
   VIF registers; the producers that remain (cloth meshDisp, shadows, particle
   packets, microcode uploads, 2D) could leave their packets out as modelDisp
   does (each checked for other side effects first).
4. Target 3 of this round.
5. A route script for Vita3K that does not depend on the game's speed (the
   menu presses on game-state anchors, "@addr=value"), so clean runs can time
   the route.

## Round 9: loads and area changes (2026-10-25)

**First: the Vita3K "route" of rounds 7 and 8 was not the route.** Its menu
presses (timed in VBlanks) were never taken in Vita3K: the game stayed on the
title menu (930 draws, frames 678-2178), played a scene of its own, then the
attract demo. From frame ~2600 on, the "route" log is the attract demo's
timeline shifted by ~2220 frames (r8/ab-r8-route-err.txt against
ab-r8-attract-err.txt, window for window), and "route 2200-2800, the load
after the shrine" was that scene and the demo's first load. The profiler
made no difference to it. The round 8 route numbers above are attract-demo
numbers.

**The route input, keyed to game state** (build/agent-scratch/r9/route2.sh,
route_in2.txt): 0x270bd4 counts the menus' confirms (1 START at the title
menu, 2 and 3 the two CROSSes of the load menus; it goes to 4 by itself
before the load dialog). PS2X_INPUT_SCRIPT's new "r<n>@<addr>=<v>" presses
again every n VBlanks until the word holds v (ps2_pad.cpp; "r<n>@<addr>"
until it changes), so each press repeats until the game took it:
`300v:START:12:r120@270bd4=1;@270bd4=1:CROSS:8:r150@270bd4=2;@270bd4=2:CROSS:8:r150@270bd4=3;@270bd4=4:CROSS:8:r150;+90v@3:WAIT;@3d:WAIT;`
then the round 5 walk, anchored on the @3d entry (5). The WAIT matters:
"@3d" counts the quiet VBlanks from before its previous entry, and the
long 2D menu let the load dialog's first VU1 program pass for 3D. On the
Mac it follows round 5's route (@3d at VBlank 1288, 1286 before; the same
scenes frame for frame); in calibrated Vita3K it takes 14 STARTs and 6
CROSSes (the menus take presses much later there) and then plays the real
route, with and without the profiler (5 runs). Compare runs from the @3d
entry's frame (build/agent-scratch/r9/tools/slowcmp.py err.txt:<frame>).
The round 8 VPK cannot play it (its pad script lacks the new keys): the
route A/B below is this round's VPK with this round's changes switched off
by env against on. PS2X_INPUT_WATCH=<addr>,... logs each change of guest
words at the pad reads (finding anchors; 0x213300, the previous round 9
attempt's anchor, is a counter, not a menu state).

**What the real route costs in calibrated Vita3K** (no profiler, the
frames from the @3d entry): -400..0 the load of the save (~60 frames over
50 ms, worst ~300 ms); 0-400 the shrine (48 ms); 400-1600 the walk out and
the area change to the outside, ~87 ms a frame for 1 200 frames (game
thread ~82 ms: ExecCharaGeometry, the walk, the hooks; not loading); the
outside's first frames (frames 3204-3230 of the first run) 140-225 ms;
1600-2400 58 ms; then ~50 ms.

**Where the load frames go** (SOTC_SUBSYS_PROF_SLOW=<ms>: each slow game
frame's probes on a line; SOTC_FRAME_LOG's slow lines now also give the
game thread's scheduler wait, the disc reads (time, count, KB), the
VBlanks and the scratchpad DMA; build/agent-scratch/r9/tools/slowprobes.py
and loadsum.py sum them):

1. Disc reads on the game thread. The loader (guest thread 11, priority 43,
   above the game loop's 45) streams an area in 32 KB reads (iosCdStRead:
   sceCdRead + sceCdSync(0)). sceCdRead read the host file inline, so the
   loader never blocked and the game loop waited for the whole burst: 146
   reads, 4.6 MB, 245 ms in one frame of the title load (Vita3K models the
   Vita's storage at 25 MB/s + 150 us an operation, io.cpp, the calling
   thread blocked). The audio stream's 150-350 KB reads (sceCdReadIOPm,
   every ~5 s) are 6-15 ms each, on the same thread (now in the background too, below).
2. The heap's garbage pass (iosMallocGarbageTickProc, decomp_gctick.cpp) at
   every area change: ~250 blocks moved a frame for 10-20 frames, 50-150 ms
   a frame. Each move is a chunked copy through the scratchpad (0x1ad958,
   already a host copy with SOTC_NATIVE) and, for the model / texture
   modules, a relocation of the moved module (garbageNmoCallBack /
   garbageNtoCallBack -> isysMoveModuleObj -> LoaderSysRelocateOnlineElfInfo,
   isysResolveOneModuleObj -> RelocateCode -> ResolveRelocation: ~8 000
   relocations a frame). Over 56 slow frames of the route: 4.2 s, of it the
   copy 0.86, the relocation loops ~2.5 (with the probes' own cost on
   468 000 ResolveRelocation calls). Its time check (2 123 463 T0 ticks of EE
   time since the frame's start, 0x26d990) never stopped it: SOTC_GC_LOG=1
   now prints each call's blocks, moves, EE cycles and how it ended (46 calls
   with moves on the Mac route to frame 4800, 3 on the attract demo, all "to
   the end").
3. The native renderer's first draws of moved and new models: packetId hashes
   a packet at an address it has not seen (each GC move gives ~2 400 packets
   new addresses): hook.packetId 15-38 ms and hook.packets 30-60 ms a frame
   at the area change.
4. The render thread in the outside's first four frames (194-210 ms, with
   110 ms of GPU wait in the first): no shader is made at run time (all at
   init); Vita3K's own pipeline creation is the likely cause (not checked on
   a console).
5. The rest is the scenes' usual work (ExecCharaGeometry 9-24 ms, the walk,
   CalcNodeQuaternionWithIK 40 ms in a character's first frame).

**Changes:**

- **sceCdRead in the background** (CD.cpp, PS2X_CD_ASYNC=0: off). A host
  worker reads; sceCdSync(0) blocks the calling guest thread (an external
  wait the worker's ExternalWake ends), sceCdSync(1) answers busy; the data
  is copied into guest memory on the EE executor when the wait ends, or by
  any CD call that could see it (the next read, sceCdGetError, sceCdStatus,
  sceCdDiskReady, the stream calls...), never by the worker. As on the
  console, the loader waits in sceCdSync while the game loop runs, and the
  woken loader (higher priority) preempts it at the next checkpoint. Check:
  PS2X_CD_LOG=2 hashes each read's data as it lands in guest memory; Mac
  route to frame 3400, sync and async: the same 1 509 reads (LBN, size,
  hash) in the same order. The load takes about the same time; the frames
  around it run.
- **The relocation natively** (decomp_reloc.cpp): ResolveRelocation,
  RelocateCode, LoaderSysRelocateOnlineElfInfo (its two guest calls,
  RelocateElfInfoHeader and DecodeSection, stay guest calls). The printing
  cases (types 1, 3, 7.., a HI16 without LO16), a chained module and anything
  outside main RAM go to the recompiled code (from the entry, at one
  relocation's call, or at the module loop's head with its registers and
  frame). No EE cycles charged (the garbage pass's clock never stopped it).
- **The garbage pass's copy as one memmove** (sotc_overrides.cpp
  copyViaSpr, SOTC_SPR_COPY_CHUNKED=1: the chunks): when the destination is
  below the source or apart, the chunked copy through the scratchpad comes to
  a memmove; the scratchpad is left as the chunks leave it (the last chunk,
  the one before past it). SOTC_SPR_COPY_CHECK=1 runs the chunks on a copy
  and compares: Mac route 3 072+ checks, 0 differ; attract demo none differ.
- Measuring: PS2X_CD_LOG (=1 every read with frame, VBlank, guest thread;
  =2 the data hashes), the frame log's new columns, SOTC_SUBSYS_PROF_SLOW,
  SOTC_GC_LOG, PS2X_INPUT_WATCH, the pad script's RAMDUMP entry, and (from
  the first round 9 attempt) SOTC_HOST_PROF=<file>, a Mac sampler of every
  named host thread with the game frame (r9/tools/hostprof.py).

- **The sound streams' reads in the background too** (sceCdReadIOPm, from
  iosIopRead, then sceCdSync(0) as for sceCdRead): the data goes into IOP
  memory on the executor when the wait ends. The streamed sound's reads vary
  from run to run in both modes (they follow the sound's real-time
  position); the other reads are the same (Mac route, PS2X_CD_LOG=2, two
  sync runs and an async one: 1 470 non-sound reads identical).

**Where the transition frames go after the changes** (calibrated Vita3K,
profiler on, the outside's first frames): the garbage pass ~50 ms a frame
(was 80-150), of it the copy (the game's own heap traffic, several MB a
frame) ~12; hook.packets + packetId 45-60 ms (the moved models' packets at
new addresses, hashed again: contentHash is a serial 64-bit multiply chain,
slow on the A9); the scene's usual ~100 ms. Over the route's slow frames the
garbage pass went 2 473 -> 1 352 ms (round 9 off / on, one run each); over
56 / 47 slow frames with the callees probed, 4.2 s -> 1.4 s.

**Results.** Calibrated Vita3K (MHZ=240 CORES=3), host on AC power, one run
a side, back to back; the route with r9/route2.sh's input (5 runs of it
followed the route; a 6th went a longer way through the altar room, the
walk's presses being VBlank-timed, and was run again), frames counted from
the @3d entry; attract and intro without input (the intro: New Game on a
fresh card, frames from its first 3D frame). "off" is this round's VPK with
PS2X_CD_ASYNC=0 SOTC_DECOMP_SKIP=ResolveRelocation,RelocateCode,LoaderSysRelocateOnlineElfInfo
SOTC_SPR_COPY_CHUNKED=1 (the round 8 VPK cannot play the route); HEAD is
celosia-r8.vpk. Frames over 50 / 85 / 120 ms, their total, the worst; no
profiler unless said.

| window | off / HEAD | round 9 |
|---|---|---|
| route, the save's load (-400..0) | 70 / 16 / 9, 6.1 s, worst 348 | 44 / 11 / 3, 3.7 s, worst 291 |
| route, shrine to the outside (0..2400) | 1850 / 664 / 46, 146 s, worst 272 | 1848 / 620 / 25, 145 s, worst 214 |
| route, the area change (740..1000) | 260 / 231 / 21, worst 272 | 260 / 216 / 21, worst 214 |
| route -400..4400 | 2452 / 681 / 55, 181 s, worst 348 | 2599 / 631 / 28, 186 s, worst 291 |
| route (profiler) -400..4400 | 4218 / 1147 / 69, worst 353 | 4230 / 1167 / 49, worst 285 |
| intro, the load into it (-250..0) HEAD | 24 / 10 / 5, 2.4 s, worst 313 | 18 / 4 / 2, 1.5 s, worst 231 |
| intro, the ride (0..5000) HEAD | 3558 / 21 / 2, worst 137 | 3630 / 10 / 0, worst 117 |
| attract 600-4800 HEAD | 28 / 1 / 1, worst 219 | 36 / 0 / 0, worst 83 |
| attract 4800-9600 HEAD | 340 / 8 / 2, worst 323 | 343 / 6 / 1, worst 157 |
| attract (profiler) 600-4800 HEAD | 63 / 1 / 1, worst 237 | 68 / 0 / 0, worst 81 |

Averages (round 8's): attract 600-4800 with the profiler, frame 37.62 ->
37.67 ms, execScript 32.68 -> 32.75, DispDispObjGroupModel 11.28 -> 11.29;
without it ([stats]) 32.56 -> 32.59, 4800-9600 33.68 -> 33.60; the route's
walk (400..1600, profiler) 97.4 -> 95.7; the intro ride 53.5 -> 53.6. The
averages do not move: what changed are the load and area-change frames
(over 120 ms: route 55 -> 28, intro 2 -> 0; the worst frames 20-60% lower).
The frames just over 50 ms count a little higher on some windows: the ~50 ms
frames sit on the threshold (noise of one run a side).

Checks: SOTC_DIFFTEST=ResolveRelocation,RelocateCode,LoaderSysRelocateOnlineElfInfo
(EVERY=200, 100 on the intro) to frame 4800, 0 mismatches: route
SOTC_NATIVE=2 3 400 / 200+ / 200+ checks (640 001 ResolveRelocation calls),
route SOTC_NATIVE=1 3 200 / 200+ / 200+, attract both modes 1 300 / 200+ /
200+, intro SOTC_NATIVE=1 4 400 / 200+ / 200+. SOTC_SPR_COPY_CHECK on the
route and the attract demo: 0 differ. The disc data (PS2X_CD_LOG=2) sync
against async: route (1 509 reads, then 1 470 non-sound with the sound
streams async too), attract (728) and intro (1 256), identical. The garbage
pass never stopped by its clock (SOTC_GC_LOG). Mac route game clock 1.001x /
0.9998x (last 12 windows; with the first and the final binary); attract demo
to frame 6000; pictures: route shots 2600-3500 (the walk out, the stairs, the
temple cutscene) and the intro (bridge, cliff, tunnel, stairs) right, no
missing or late meshes or textures (r9/sheet-mac-clock.png,
sheet-intro-async.png).

**Sound and the picture on the Vita** (the user's report: around
transitions and cutscenes the sound runs ahead and never comes back):

- (a) The sound runs on the host's clock: with SOTC_NATIVE=1 the host audio
  thread pulls the mixer at 48 kHz (sound_mixer pull mode), and the streamed
  voices keep up in real time (PS2X_SG2_STREAM_LOG=1: ~16 refills and as
  many half changes per 5 s of host time in calibrated Vita3K, as on the
  Mac at 60 fps, 0 halves played stale). The game runs on its own clock:
  the guest VBlanks. A VBlank needs its EE-cycle deadline, and the host
  clock's catch-up (EeScheduler::syncCyclesToHostClock, after GAMECORE) is
  capped at 2 VBlank periods ("never replay more than two frames at once;
  forget older lag"): a frame slower than that loses the rest. So the game
  sees fewer VBlanks than real time and SotC, which moves by VBlank count,
  plays slower than real time: game time / wall time (r9/tools/drift.py, the
  frame log's "vb") 0.65-0.71 over the eagle ride (the sound ahead by 15-20 s
  per 1 000 frames, 52 s over the first 3 000), 0.47-0.50 on the route's
  walk and area change (51 s over 1 200 frames), 0.65 in the save's load.
  It never comes back because nothing ties the two clocks.
- (b) The streamed sound's reads (sceCdReadIOPm, 16-352 KB: 6-15 ms each in
  Vita3K) and the area loads (sceCdRead, 32 KB) are both issued by guest
  thread 11, the loader, one after the other, through the same host read
  (readHostRange; on the Vita one mutex held across sceIoPread). Before this
  round both held every guest thread; now both wait in sceCdSync on the
  loader only. A load still waits behind a sound read and the reverse (same
  thread), ~15 ms at most.
- (c) The game reads the driver's status every frame (voice envelopes, the
  streams' positions at +0xc0, the PCM position at +0x180); the streams'
  feeding follows those. Nothing found that times a cutscene by the sound,
  and the drift itself says the picture does not wait for it; a voice line
  that must end before the game goes on would make the game wait less, not
  skip. Not established beyond that (no wait on a sound semaphore was
  looked for).
- (d) The overlay's "game N fps" counts display lists (game frames) a
  second of host time; "host M fps" is the raylib loop's rate, which with
  SOTC_NATIVE runs apart from the game thread (the native renderer presents
  each game frame itself): at a transition only the first drops.

The fix is the clock, not the sound: PS2X_HOST_CLOCK_MAX_LAG=<VBlanks>
(new, default 2 = as before) caps the catch-up. With 8, the intro in
calibrated Vita3K keeps game time = real time (1.00 over 2 000 frames,
against 0.69) at the same cost (53.5 ms a frame, worst 118 vs 113): the
game steps ~3.2 VBlanks a frame, as the console does at that frame rate,
and the sound stays with the picture. Not on by default: it changes the
time step the game logic sees on slow frames (larger steps for movement,
physics, AI). Plan below.

**Round 10 (plan):**

1. The clock: run the route and the attract demo with
   PS2X_HOST_CLOCK_MAX_LAG=4 / 8 in calibrated Vita3K and look for what the
   larger steps change (the route's path, collisions at the stairs, the
   cutscenes' lengths in host time, the sound); the game was written for a
   VBlank clock that never stops, so a cap of a few frames should be the
   console's behaviour. Then the default for the Vita.
2. packetId after a garbage move: a faster content hash (parallel lanes,
   32-bit multiplies for the A9), or the moves told to the packet memo
   (the bytes of a moved packet are the same unless a relocation wrote into
   it: ResolveRelocation is native now and can say where it wrote): 45-60 ms
   of each area-change frame.
3. The garbage pass's rest natively: heapMove (0x1ae270), isysMoveModuleObj,
   kernel_sub_001B4170 / 4760 (symbol search), DecodeSection; ~15 ms a frame
   at an area change.
4. Round 8's plan items 2-4 (ExecCharaGeometry's body, the DMA walk without
   walking, iosDlLockObj / CalcLightLayoutObj): the route's walk is 87 ms a
   frame of real game work, the largest cost left.
5. The render thread's first frames in a new area (194-210 ms, 110 ms of it
   waiting for the GPU in Vita3K): check on the console whether it is
   Vita3K's own pipeline creation.

## Round 10: the clock, and the route's walk outside (2026-10-25)

**The game's time step.** GAMECORE's frame time (0x1465650, gamecore_sub_01345638
each frame) comes from the EE timer, not from a VBlank count: iosGetTCount
(0x1fd8a0, T0 overflows at the bus clock) minus the last frame's, through
iosGetSecondFromTCount, scaled by +0x1a0 (the game speed): +0x180 is the
world's step in seconds (doit_particle_manager, doit_wind_manager,
doit_cameraman_manager and ~120 other readers take it; 0x14657e0 = +0x190
another ~180). No clamp is applied where it is made; frameRateMode
(0x14659e8, set only through a pointer, a debug switch) replaces it with a
fixed 1/30 s. VBlanks themselves count only at 0x1e1314 (getVIntrCnt: the
cutscenes' demo clock, getVIntrDiffTime, and the play-time counter
0x1e133c). So SotC is a variable-step game on real (EE) time, as on the
console when it drops frames; the frame log now prints the step ("dt", ms)
beside "vb".

The host clock's catch-up cap (EeScheduler::syncCyclesToHostClock) was 2
VBlanks: on a 90 ms frame the game saw ~40 ms of time, so the world ran at
0.4-0.7 of real time while the streamed sound runs at real time.

**Checks of a larger cap** (all on this round's binary, cap by
PS2X_HOST_CLOCK_MAX_LAG):

- Mac, frames made slow on purpose (SOTC_FRAME_STALL_MS=70 from frame 1300,
  ~86 ms frames, like the Vita's walk), the route (r10/route.sh with
  SOTC_POS_LOG: the player's position by game time): cap 8 gives game/wall
  1.00 and follows the fixed-step references (SOTC_FRAME_INTERVAL=2, a
  steady 30 fps, and =5, a steady 12 fps: both real time) within 1-3 units
  over the whole walk out of the shrine and across the plain to Agro; cap 2
  gives 0.44 and another path (into a pillar of the shrine's gate, as the
  unthrottled 60 fps Mac run also does); cap 4 0.82, its path leaves the
  references at the plain. The path is chaotic in frame timing anyway: two
  runs of the same configuration can differ (round 9's runs took a longer
  way once in six). Nothing broke: no falls through the ground, no stuck
  animation, the cutscene ("If thou should findst an altar...") at the same
  game time in all the real-time runs (sheets r10/sheet-p1.png, sheet-p2.png).
  The stall stops every guest thread (the loader too), so it does not
  model cutscene streaming: the attract demo's cutscenes ran late under it,
  on both caps; that is the stall, not the cap (the Vita3K runs below).
- Calibrated Vita3K, the attract demo: the same demo, scene for scene, to
  frame 12 000 (the draw-count profile on the game's clock,
  r10/tools/gametime.py; the scene changes come at the same wall time and
  frame, frame 4 360 / 6 038 / 6 838 against 4 390 / 6 065 / 6 854, and later
  in VBlanks by up to ~600: HEAD's VBlanks ran at 0.94 of real time).
- Calibrated Vita3K, the route: the real route plays (5 runs), game/wall
  1.00 in every window.
- Mac: route clock 0.9996-0.9999 (last 12 windows; the cap does not act at
  60 fps), attract demo past frame 4800 (to 5 700).

**Change:** the cap is 8 VBlanks (133 ms) by default, everywhere;
PS2X_HOST_CLOCK_MAX_LAG=2 gives the old behaviour. Frames longer than 133
ms (loads) still lose the rest. In a slow scene the game now takes fewer,
bigger steps and keeps real-time speed; the sound stays with the picture.

**The walk outside, ranked** (calibrated Vita3K, profiler on, the real
route, frames 400-1600 after the @3d entry; cap 8, so ~4.9 VBlanks a
frame; two runs with callee probes, SOTC_SUBSYS_PROF_EXTRA,
r10/env-route-xprof*.txt; ms a frame, self / inclusive):

| subsystem | self | incl |
|---|---|---|
| ExecCharaGeometry (12 characters a frame) | 1.2 (5.9 without its callees probed) | 21.4-21.8 |
| .. GetCharaRootDynamics (its walking-mode callback xz and charrootdyn_*: ground, slide, landing, water) | 4.5 | 6.5 |
| .. CalcNodeQuaternionWithIK / skeletonPose | 2.7 / 2.9 | 4.6 |
| .. GetMotionMatrix, CalcRootFitting, Preprocess, 010C7B28 (x380), cloth | 0.4-1.5 each | 3.3 / 2.3 / 2.0 / 1.5 / 1.4 |
| DispDispObjGroupModel (the display walk, native since round 7) | 6.3-6.5 | 19.3-19.8 |
| .. modelDisp (hook.packets 3.0, hook.state 1.6, packetId 0.5) | 1.0 | 7.3-7.6 |
| collision: ClipCollisionDirectCallBack (native), ClipCollisionDB x600, finders | 3.5-4.0 / 1.7-1.8 / 0.9+0.5 | ~9 |
| the horse's script task (nico_horseRStart) | 1.6-1.8 | 10.9 |
| the player's life bar task (013F8668's rings: 013F6B20 1.5, 7178 0.75, 75D0 0.7, 400 sin / cos) | 2.1-2.4 | 2.4-3.5 |
| camera (doit_game_camera) | 2.0-2.2 | 4.5-4.7 |
| the AI (CharaCtrlAIControl) | 1.8-2.4 | 2.6-3.1 |
| the DMA chain walk, the light pass, particles | 2.0, 1.7-1.9, 2.0+3.0 | |

What the outside adds to the attract demo's profile: the horse, the
characters' root dynamics on uneven ground (the sphere finder and its
skinned variant), the HUD's life bar, more collision.

**Changes (natives, each with SOTC_DIFFTEST, 0 mismatches):**

- iosGetSinf / iosGetCosf (decomp_math.cpp): the life bar's ~400 calls a
  frame (0.95 ms of the frame in the probe run) and texAnim's waves.
- texSearchMopByName (decomp_texsearch.cpp): the texture list walk without a
  guest strcmp a node (rare: ~200 calls over the route).
- The sphere finder's tree walk, manager_sub_01128F38 (decomp_collision.cpp:
  1.0 ms self at 65 calls a frame in the probe run, recursive, a guest
  call per leaf), its leaves (round 4's natives) called straight.

They are small: the walk's cost is spread over whole subsystems (the
characters' root dynamics, the horse's script, the HUD) that this round
only mapped. The route A/B with the old cap (PS2X_HOST_CLOCK_MAX_LAG=2,
the same frames as HEAD) is within noise (below).

**Results.** Calibrated Vita3K (MHZ=240 CORES=3), host on AC power, one
run a side, HEAD = celosia-r9.vpk (d44f871), round 10 = celosia-r10.vpk;
the route with r9/route2.sh's input (r10/v3k.sh), windows on the game's
clock in VBlanks after the @3d entry (r10/tools/vbwin.py: with the new cap
a frame covers more game time, so frame windows would compare different
places); frames over 50 / 85 / 120 ms, the worst; no profiler unless said.
"r10 cap 2" is this round's VPK with PS2X_HOST_CLOCK_MAX_LAG=2 (the
natives alone).

| route window (VBlanks) | HEAD | r10 cap 2 | r10 |
|---|---|---|---|
| the save's load (-1000..0): mean ms, >50/85/120, worst | 35.2, 54/8/3, 293.7 | 34.1, 49/8/4, 293.4 | 34.0, 34/10/3, 297.2 |
| the shrine (0..1000) | 52.9, 181/71/0, 119.1 | 53.1, 184/69/0, 119.3 | 47.8, 109/43/0, 113.2 |
| the walk out and the area change (1000..4000) | 85.7, 1172/519/22, 211.7 | 85.2, 1182/502/22, 209.8 | 87.4, 573/279/24, 207.1 |
| the plain (4000..6000) | 59.4, 846/0/0, 82.6 | 58.7, 845/1/0, 85.8 | 59.3, 563/0/0, 75.0 |
| frames in 0..8000 (133 s of game time) | 3304 | 3319 | 2057 |
| game time / wall time, 1000..4000 | 0.50 | 0.50 | 1.00 |
| sound ahead of the picture over 0..8000 | 90.4 s | 89.7 s | 0.0 s |
| VBlanks (game step) a frame, 1000..4000 | 2.56 (~42 ms) | 2.54 (41.6 ms) | 5.24 (85.8 ms) |

The same with the profiler: the walk (1000..4000) 89.6 ms a frame HEAD,
93.5 round 10 (>85 ms: 797 / 378, >120: 33 / 39, worst 214.6 / 228.1);
game/wall 0.48 / 1.00. The frame indexes 400..1600 (round 9's window):
HEAD 94.4 ms (profiler), 85.9 (clean); round 10 79.3 / 71.3, but those
frames are further along the route with the new cap.

With the larger step a frame of the walk costs about the same or slightly
more (+0-4%: some work grows with the step), but the game covers the walk
in about half the frames and half the wall time: the route's 133 s of game
time take 2 057 frames instead of 3 304. Two runs of the route do not walk
the same path (the stick presses are VBlank-timed and the world is
chaotic in its frame timing), so a window's content differs a little from
run to run; per-subsystem differences between the two caps are not
attributable (one run had more characters on screen than the other).

| attract demo | HEAD | round 10 |
|---|---|---|
| 600-4800, no profiler: mean ms (>50 / >85, worst) | 32.55 (18 / 0, 75.5) | 32.55 (24 / 0, 75.2) |
| 4800-9600, no profiler (later scenes at those frames: the demo runs later in frames) | 34.09 (397 / 7, 190.9) | 32.23 (222 / 3, 159.5) |
| 600-4800, profiler: frame / execScript-level probes | 37.57 / 34.87 | 37.53 / 34.77 |
| DispDispObjGroupModel / ExecCharaGeometry (profiler) | 11.28 / 6.37 | 11.21 / 6.38 |
| 4800-9600, profiler | 37.32 | 36.38 |
| game / wall over 12 000 frames, sound ahead | 0.94, 21.2 s | 1.00, 0.8 s (1.1 s of it in frames 0-600, the boot; 600-12000: -0.3 s) |
| plays to | frame 12 000+ | frame 12 000+ (the same demo) |

**The dust report (not reproduced).** During this round's Vita3K run
w2-route-xprof2 the user saw the dust kicked up by Wander running and by
Agro galloping drawn as hard-edged red / grey translucent quads, fanned
behind them. Looked for, without finding it: three calibrated Vita3K runs of
the same VPK with screenshots of our window (tools/vita3k.sh SHOTS=, 1-2 s
apart from the walk out to Agro's arrival: two clean, one with that run's
probe env), and Mac runs with GS and native pictures of the same moments
(PS2X_SNAPSHOT_FRAMES pairs, the native renderer through Metal and through
its GL backend, SOTC_NATIVE_GL=1, which the Vita uses, at 60 fps and
throttled with cap 8): no such quads, and no dust visible in the emulated
picture at those moments either (the dust is brief; 1-2 s sampling can miss
it). Not bisected (nothing to bisect without a reproduction). The quick
texture path (decodedTexture: a texture drawn the frame before is not
resampled for up to 15 frames) could show another texture's pixels after a
garbage move puts a different texture at the same address: SOTC_R2_CHECK=1
on the Mac route counted 0 stale quick textures (536 336 checks) but 2
packets rewritten in place that kept their id. Open; a screenshot or the
moment (scene, wall time) would let it be bisected with the switches listed
in the request.

**Measuring tools (new):** SOTC_FRAME_STALL_MS / _FROM (Mac: slow frames),
SOTC_POS_LOG=<n> (player and camera position, VBlank and step every n
frames), PS2X_EXIT_AT_VBLANK, SNAP entries of the input script saved by the
Mac loop with the native renderer (snap_<n>.png / native_snap_<n>.png),
tools/vita3k.sh SHOTS=<dir> SHOTS_FROM / SHOTS_EVERY (screenshots of our
Vita3K window only, tools/winid.swift), r10/tools/ (poscmp.py, gametime.py,
vbwin.py: windows on the game's clock, profrank.py: every probe of a frame
range ranked, hostunder.py: Mac host samples under a function).

**Round 11 (plan):**

1. The character root dynamics from its top: GetCharaRootDynamics's
   callback xz (0x10e7610, the walking mode) and its charrootdyn_* callees
   (getCollisionDB, slidecollision, getLandingSurface, getWaterStatus,
   getCompleteFitting...), with the sphere finder's top (01115F88) and the
   skinned finder (01116958) under it: GetCharaRootDynamics was 6.5 ms of
   the calibrated walk frame (4.5 of it unprobed callees), xz 6.7 with its
   collision. The finder walk (this round) is its first piece.
2. ExecCharaGeometry's other callees (CalcRootFitting 2.3, GetMotionMatrix
   3.0-3.3, CalcNodeInterp, AddCharacterClassSubGeometries, 010C7B28 1.5 ms
   at 380 calls a frame), then its body.
3. The player's life bar (playerlifebar_script, ~2.1-2.4 ms a frame of
   the walk even while the bar is invisible): 013F6B20 / 7178 / 75D0 draw
   two 44-point rings with ~200 gsReg calls; natively, with the gsReg calls
   kept (the native 2D hook needs them) or, as a game-specific shortcut,
   the draws left out while the bar's alpha (*0x11f85b4) is 0 (the point
   arrays they write must still be made: 78A8 / 7F38 read them).
4. The horse's script task (nico_horseRStart, t09 @01390f48: 10-12 ms
   inclusive on the plain; gamecore_sub_01390A90 1.8 ms self) and the
   kites (t09 @013e4350, 1.9 ms self).
5. Round 9's leftovers: packetId after a garbage move, heapMove and the
   symbol search.
6. The quick packet ids: SOTC_R2_CHECK=1 on the Mac route found 2 packets
   rewritten in place that kept their id (frames 3043-3044, the area
   change): one frame in 16 recovers them; check what they draw meanwhile.

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
