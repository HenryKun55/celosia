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
| decomp_texframe.cpp | texReset (the frame start's texture reset: LOD scale, slot arrays, the list walk with texAnim) | attract 400, route 400, 0 mismatches | see "Round 6" |
| decomp_group.cpp | isysGroupForNext, isysGroupForNextForce, InitLayoutObjMatrix | attract 13 000 / 1 900 / 400, route 62 300 / 45 800 / 400, 0 mismatches | the group iterator inline in the frame start's matrix flip |
| decomp_dl.cpp | AddSortObj, iosDlChainTailCurrent | attract 22 800 / 121 500, route 3 600 / 89 200 (SOTC_NATIVE=1, EVERY=5), 0 mismatches | the sort buckets' insert, the display list's tail packet |
| decomp_wind.cpp | manager_sub_01145F78 (the wind zones at a point, sorted by priority) | attract 3 300, route 1 300 (SOTC_NATIVE=1), 0 mismatches | the three weight functions stay guest calls |

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
(the same call dumps), 2.15% of the game's own code. Run it again after
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
