# Round Y3: the object animation library as native C++ (SOTC_Y3=1, off by default) (2026-11-26)

Why: X2's console ranking put `st_playObjAnim` third among the script bodies (~0.57 ms a frame on the console), its
weight in the frame end's object animation: `ExecObjectAnimation` (0x110a130), its sampler
`GetObjectAnimationFloatFrame` (0x10b1458) with the evaluators, and the geometry pointer kick (manager_sub_01108960) with
the updates it hands the samples to. Y3 ranks the library's functions, then rewrites the heaviest piece whole.

Base: local branch `xm` (X1 + X2 + X3 on main 8e39115). Code: `game/overrides/y3_objanim.cpp` / `y3_objanim.h`, the hook
in `decomp_hot46i.cpp` (ExecObjectAnimation's native), two thin entries in `decomp_hot46h.cpp` (the D2 / D3 sampler cores
callable directly), `sotc_w1_elf_pad` 4 -> 8 KB (`decomp_walk.cpp`). Scratch, package, runs: `build/agent-scratch/y3/`.

## 1. The ranking

### 1.1 Vita3K, function by function

`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1` with `SOTC_P2BRACKET_EXTRA` = 31 library functions (ExecObjectAnimation,
st_playObjAnim, the sampler and every evaluator, the kick, updateObjectGeo, the simple geometry, the bookkeeping callees).
Clock brackets, us a frame at 222 MHz, windows 6+ (~1 100 brackets a frame inflate the absolute numbers; the shape is
what counts). rankA = ride7 480 s, lrankA = lake6 400 s (x3's package; the library is the same code in X1-X3).

| function | ride7 incl / self (calls/f) | lake6 incl / self (calls/f) |
|---|---|---|
| ExecObjectAnimation | 1 953 / 316 (23.0) | 1 511 / 202 (3.0) |
| GetObjectAnimationFloatFrame (the sampler) | 1 241 / 426 (70.0) | 1 038 / 356 (38.0) |
| the kick (manager_sub_01108960) | 390 / 322 (69.1) | 259 / 255 (38.0) |
| objectAnimSample (010AF070) | 320 / 161 (16.0) | 358 / 200 (14.0) |
| keyVec3 (010ADE08) | 157 (141) | 119 (78) |
| posKeyLerp (010AEAF8) | 118 (55) | 94 (37) |
| CheckLetter | 100 (45) | (in ExecScriptLayoutObj) |
| updateObjectGeo | 79 / 31 (2.9) | - |
| ResetDamageClass / the time step / ExecChildGroup / the part clock | 28 / 37 / 10 / 9 | 9 / 12 / - / 3 |

The sampler's self (426 us) is mostly its evaluators that no bracket covers: the alpha / vector / colour samplers
(010AF2A0 / 010AF4D8 / 010B06A8) run as recompiled MIPS (round 106's natives are off by default); the kick's self (322
us) is its cases' own work and their unbracketed callees.

### 1.2 What the channels are (`SOTC_Y3_LOG`: every channel counted by type, the two tables' words with it)

The two jump tables (sampler 0x1193270, kick 0x1195030; 48 words each, decoded from `build/captures/manager.bin` and
read live in the runs) decide what a channel costs. On ride7:

| where | channels / frame | types: sampler -> kick |
|---|---|---|
| the shrine and its steps (23 objects) | 62 | 23: object sample -> global light in range (10.8); 24: alpha -> out of range (12); 25: vector -> ambient (12); 26: colour -> distant (16.5); 22, 4, 5, 9, 16, 17, 28, 32 (1-2 each) |
| riding out of the shrine area (11-16 objects) | 44-87 | 5: option node + alpha -> caseObjectAttach -> updateObjectGeo (60-82); 1: option node + flags -> caseObject -> updateObjectGeo (8-10); 48: with position -> wind box (6-8) |
| riding the open plain | 1-3 | |
| lake6 (3 objects) | 37 | the light channels (20 a frame) and type 2 (particle kick) |

So the library's weight is ~60-90 channels a frame, each the same guest-call ladder: ExecObjectAnimation -> the sampler
(a 0xd0-byte prologue / epilogue, the jump table, the plain-case table walk) -> one evaluator -> the kick (a 0x480-byte
prologue / epilogue, its jump table, nine callee-saved registers copied into the context before each of its guest calls)
-> GetLayoutClass, the light type test, the light's update (or updateObjectGeo). The useful work per channel is one
evaluator and one update; the rest is glue. **Rewritten whole: ExecObjectAnimation with its channel walk, the sampler's
dispatch and the kick's cases the ride runs, as one native pipeline.** (The damage channels, types 3 / 10 / 13 ->
updateDamageGeo, still recompiled MIPS, are the colossi's weak points; none ran on ride7 / lake6, Y3 takes them too.)

## 2. What Y3 makes native

`SOTC_Y3=1` (read once; off by default; requires `SOTC_D1NATIVE=1` as the console env files set it):

- **ExecObjectAnimation** as host code over typed views (`AnimPart`: object + 0x20, the key frame / step, the walk and
  sampled animations, the targets, the cloth list, the callback, the state bytes; `WalkAnim` / `WalkChannel`, `Target`,
  `ResAnim` / `ResChannel`, `Blend3Data`). Its bookkeeping guest calls (iosResetCpuRapCounter at its relocated site, the
  damage class reset, CheckLetter, the time step, the part clock, the callback and its simple geometry,
  ExecChildGroupManagerObj, the cloth layouts' wind and GetClothDynamics) stay calls with the original's arguments,
  s0-s7 / fp and frame slots (round 48's rule).
- **The sample**, per channel, by the sampler's word (read live):
  - a plain case (12 of them: the object / alpha / vector / colour samplers ...): its evaluator called directly with the
    registers the sampler's prologue and case leave for it, no sampler frame, no table walk;
  - the option-node cases (flags, alpha): optionNodeSample (010AE128) called directly, then the case's tail natively
    (+0x30: 0 or the frame's alpha byte / 255; +0x34 / +0x38 the data's flags);
  - the vec3 blend (type 3): direct C++ (the frame's cycle wrap with the divide's sticky flags, D2's `posKeyLerpCore` and
    D3's `d3Vec3Core` called directly through `decomp_hot46h::y3PosKeyLerp` / `y3KeyVec3`, the blend's VU0 statements on
    host vectors);
  - anything else: the old sampler call.
- **The kick**, by the kick's word (read live):
  - the four global light cases (in range, out of range, ambient, distant): the kick's registers from its prologue,
    GetLayoutClass's plain cases and the light type tests (IsEnvelopeGlobalLightType / IsEnvelopeDistantLightType) as
    host reads, the out-of-range case's VU0 placing (the pointer's rotation by the parent's, its position through the
    parent's matrix), then the light's update as the one guest call left, with s0-s7 / fp / sp / ra as the kick holds
    them; then the kick's epilogue;
  - caseObject / caseObjectAttach (when the record's id is not negative: nothing follows the call): updateObjectGeo
    called with the kick's arguments, registers, frame and ra;
  - the damage case: updateDamageGeo's size test natively; below 0.001 nothing runs, above it the kick is called;
  - anything else: the old kick call.
- Gates (the old body for the whole call, nothing written): hotlist46 off, d1native off, the worker lane, a table slot
  that holds something else than the functions Y3 stands in for (a profiler wrapper, a skip), a frame outside RAM. Per
  channel: posKeyLerpD1's / keyVec3D3's own gates and the blend of the frame's stale contents.

**Kept exactly**: every byte the old chain writes outside the dead stack below the frame; the VU0 registers (the lerps'
partial-lane writes carry vf5-vf8 / ACC lanes from channel to channel, vf7 takes t0's high half); fcr31 (the divide's
sticky flags, the condition bit); f12-f31; s0-s7 / gp / sp / fp; at every guest call left, the callee's arguments and
the callee-saved registers as the original holds them. **Not kept** (dead at every later read): the scratch GPRs' low
halves (each set before its next read; the callees read only their arguments), f0-f11, LO / HI, ra, s1's high half (only
updateDamageGeo's `movn s1, v1, v0` + `ld s1` changed it), the skipped callees' frames below sp (as every direct-call D3
native). The contract is X1's (low 64 bits of the GPRs, f12-f31, VU0, fcr31, every written byte).

## 3. Checks

`SOTC_Y3_CHECK=1` (same thread, no host thread; `SOTC_Y3_CHECK_FROM=<frame>`), per channel Y3 takes:

- **sample**: the old sampler runs first on the real state (the context and the frame's 0xf0 bytes copied), its result
  is kept, the copy is put back, Y3's sample runs; compared: s0-s7 / gp / sp / fp low 64 bits, f14-f31, fcr31, VU0
  vf1-vf31 / ACC / Q / status / MAC / clip flags, the frame's bytes (the record). On a difference the old sampler runs
  a second time on the same state: if that run equals Y3's, the old sampler is not repeatable there (counted apart).
- **light kick**: the same, old kick first (its result and the light object's 0x200 bytes kept, everything put back),
  then Y3's; compared with f12-f31 and the light's bytes.
- **object kick**: a spy in updateObjectGeo's slot (it keeps the call's context and returns) while the old kick and Y3's
  run on the same state; compared at the call (a0-a3 / t0-t3 / ra too) and after; then the old kick runs for real.
- **damage kick skipped**: the old kick runs for real on the same state and is compared with what Y3 leaves.

| run | build | result |
|---|---|---|
| chkH | p7 (light kicks, plain samples) | ride7 600 s, mounted at 2 217: 192 249 samples, 162 358 light kicks, **1 difference**: ACC after a vector sample (type 25) at frame 2 990, right after a teleport (see below) |
| lchkH | p7 | lake6 400 s (in the lake): 370 003 samples, 229 997 light kicks, 0 differ |
| **chkI** | p8 (+ object kicks, option-node samples, the repeat probe) | ride7 600 s, mounted at 7 360: **564 659 samples, 301 159 light kicks, 225 084 object kicks, 0 differ** |
| **lchkI** | p8 | lake6 400 s, swimming in the lake: **372 595 samples, 226 338 light kicks, 8 860 object kicks, 0 differ**; 1 sample where the old sampler is not repeatable |

The ACC case: lchkI's repeat probe caught the same kind (type 25, ACC, frame 2 009, again the first frames after the
route's teleport at 2 000): the old sampler run twice on one state left two different ACCs, and its second run equals
Y3's. The vector sampler (010AF4D8) runs as recompiled MIPS there; Y3 calls the very same function with the same
registers. So the difference is in the old evaluator's repeatability (state the generated code keeps between calls),
not in Y3; it shows only because the check runs it twice. chkH's single difference is that case (the same type, ACC,
after the teleport; p7 had no probe).

No damage channel ran on either route (0 damage kicks); that path is checked by construction only.

**Build** (`build/agent-scratch/y3/`, X1's Vita dir re-pointed: same-length paths replaced in place, the object
directory moved to this tree's name, the ninja log's command hashes recomputed for the new paths (rapidhash, ninja
1.12), the unchanged sources' times set back: an incremental build of ~20 steps): 0 errors. spalign `0 sp-based
:128/:256 NEON accesses (of 3320 with an alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1
overlaps" after `sotc_w1_elf_pad` 4 -> 8 KB (the text ended 0x88c bytes below the next 64 KB boundary, the same
vita-elf-create message W1 had). No alignas(16) by-value structs (the check's contexts are static), no %zu.

**Pictures** (looked at): rankA t0300 Wander running on foot across the grass by the ruined shrine walls; lrankA t0300
the lake's dark water by a stone wall, the HUD. chkH t0300 Wander galloping on Agro across the plain, a hawk overhead,
the shrine's tower far right; t0540 galloping along the shrine's stone wall. chkI t0300 Wander on Agro walking out of
the shrine's stone passage; t0540 galloping on the plain, cliffs and the far wall ahead. lchkI t0300 Wander swimming in
the gorge lake, the cliffs both sides, the HUD's circle, sword and red bar. brkJ (off) t0300 Agro galloping by the
shrine's sunlit pillars (bright yellow-green stone, birds), t0420 galloping on the plain; brkonJ (on) t0300 Wander running
on the grass toward the rock arches, t0420 on Agro by a sunlit shrine wall, the same yellow-green light as off's. The
light channels Y3 runs are the shrine's light animation: its colours look the same on and off. Nothing missing or
misplaced.

## 4. Speed

Vita3K (brkJ off / brkonJ on, p8, ride7 480 s, `SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1 SOTC_Y3_BRACKET=1`: the
`scripts:y3ObjAnim` bracket around every ExecObjectAnimation call in both halves, us a frame at 222 MHz; Vita3K is the
shape, not the console):

| windows (600 frames) | off: y3ObjAnim incl (calls/f) | on: y3ObjAnim incl (calls/f) | on: y3Walk (3 walks a frame) |
|---|---|---|---|
| 3-13 (the shrine: 23 objects, ~62 channels) | 837-1 041, mean **918** (23) | 770-847, mean **803** (23) | 591-678 |
| open plain (6 objects, 1-3 channels) | 66-77 (6) | (not reached in the window: the on run mounted later) | |

So **-115 us a frame (-12.5%)** of ExecObjectAnimation's 918 in Vita3K where it is heavy. Y3's own walk is ~600-670 us of
the 803: what is left is the evaluators themselves (the alpha / vector / colour samplers, still recompiled MIPS, and
optionNodeSample) and the light / object updates (updateObjectGeo and its ~10 guest calls). Those are the next step: the
evaluators as typed host code (round 106 found its context-exact natives no faster: the gain needs the keys decoded in
host locals) and updateObjectGeo's callees.

**Expected on the console**: the glue Y3 removes is guest-call dispatch, two prologues / epilogues and ~27 register
stores per channel (the sampler's 0xd0 and the kick's 0x480 frames, the kick's sync before each call): ~0.8-1.5 us a
channel at 444 MHz, ~60-90 channels a frame in the shrine area and while riding out of it: **~0.05-0.15 ms a frame**
there (of st_playObjAnim's ~0.57 ms, X2), about nothing on the open plain.

## 5. Console steps

`build/agent-scratch/y3/` (README.txt): `celosia-y3.vpk` (ELF `celosia-y3.elf`, build p8), `env-y3-off.txt`
(`build/agent-scratch/wx/env-wx-on.txt` + `SOTC_X1=1 SOTC_X2=1 SOTC_X3=1`), `env-y3-on.txt` (+ `SOTC_Y3=1`),
`env-y3-brk.txt` (on + `SOTC_Y3_BRACKET=1`). Two rides of the usual route (off, on), send both err.txt; optionally a
third with the brk file (`scripts:y3ObjAnim` around every ExecObjectAnimation call, `scripts:y3Walk` around Y3's channel
walk; for an A/B of the bracket itself add `SOTC_Y3_BRACKET=1` to the off file: y3ObjAnim is in both halves).
`python3 tools/p2_report.py err-y3-on.txt --elf build/agent-scratch/y3/celosia-y3.elf`.

Expected: ~0.05-0.15 ms a frame less while the shrine's light animation and the riding area's objects run; about nothing
on the open plain.
