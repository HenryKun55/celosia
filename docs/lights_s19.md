# S19: the ambient light lookups without guest calls (2026-11-19)

Round S19's brief: profile the ride on the current tip (main + S17 + S16, every default-on switch), list the game
thread's top items, and redesign the biggest one no earlier S round took, as host code over host reads. A small round:
one part.

Switch: `SOTC_S19AMB` (A/B name `s19amb`; sotc_decomp switch `kS19Amb`, after kS16Plan, the last of the tables,
**off by default**: on with `SOTC_S19AMB=1`, see section 4; the A/B runs need `SOTC_S19AMB=1` besides `SOTC_AB_FLAG=s19amb`, as S16's). Check mode `SOTC_S19AMB_CHECK=1`, counts `SOTC_S19AMB_LOG=<frames>` (`[s19amb]` lines, default every 600
frames with the check). Code: game/overrides/decomp_ambient.cpp (impl's `host` path, G::hostWalk / pick / floorAt,
hostOk, the check). Scripts, runs and logs: build/agent-scratch/s19/ (clone.sh the build dir cloned from S16's,
prof.sh the sampler, chk19.sh the check, abs19.sh the s19amb interleaved A/B, chainA.sh / chainB.sh the build-then-queue
chains started with nohup, vq.out). Own Vita3K trees ../vita3k-celosia-s19 / -s19b (222 MHz, 3 cores, muted), deleted
at the end.

## 1. The profile (ride7, Vita3K 222 MHz, p0 = 1f8bb16, every default-on switch)

profR0: 120-660 s, 9 988 frames, the game thread 34.3 ms a frame (a loaded machine: two other rounds' Vita3K runs and
builds beside it). The areas: display and models 23.0%, other game code 22.2%, characters and motion 18.6%, collision
10.4%, runtime 10.0%, system and memory 4.0%, particles 3.4%, math 2.6%. The top symbols (self time):

| # | symbol | % | what it is | redesigned before |
|---|---|---|---|---|
| 1 | memcpy | 1.41 | libc copies from ~80 sites; the samples' LR mostly past the ELF's text (another module's code) | S9 took the long ones |
| 2 | sotc_native::modelDispHook | 1.36 | the model draw hook: the instance's matrices, plan, job for the render thread | S14 (palette), S16 (plans), S10 |
| 3 | decomp_walk::quad117BodyA9 | 1.22 | the heat-haze strips' immediate quads (VU-exact) | S12 (off) |
| 4 | char122::decodeKey | 1.15 | the characters' motion keys decoded (packed quaternions, s16 keys) each frame | host code since round 122; no S round |
| 5 | decomp_prefetch::objectLines67 | 1.13 | the display walk's learned guest-line prefetch (+ window 0.41) | a console latency aid, free on the A9 only |
| 6 | coll115::walkT | 1.09 | the collision segment's tree walk over host leaf blocks | host data since round 115 |
| 7 | decomp_walk::dispDispObjGroupModel | 1.04 | the display group walk (clip, draw calls per object) | S4 / S10 parts |
| 8 | decomp_wind::windZonesBody | 1.02 | the wind zones | S7 (off) |
| 9 | s3chara::pose | 0.99 | the characters' pose on a host rig | S3 |
| 10 | obj125Run (object loop) | 0.91 | collision against the objects' parts | round 125 host loop |

Then callGuest 0.77% (0.53 of it from the particle draw list, decomp_ptcl117::listImpl), finder 0.77, the particle
emitter 0.75, addSortObj 0.73, lightPass 0.70, clothStep 0.69, finderLeaf 0.68, ... execGlobalLightMan 0.51,
getAmbientColorC 0.41.

The flat profile has no unredesigned item past 1% that is guest-shaped: decodeKey, coll115 and the display walk are
host code already (a decoded-key cache was tried in round 122 and dropped: the heap reuses a freed motion's address),
the prefetch is the console's, the rest were S rounds. The light pass is the largest area no S round touched
(lightPass 0.70, execGlobalLightMan 0.51, getAmbientColorC 0.41, objectLight, calcLightLayoutObj, lightMatrices,
setWorkImpl: ~2.6% together), and its most PS2-shaped piece is getAmbientColorC: the per-object ambient colour, which
asks the kernel's group API for the ambient light objects through guest calls. That is what S19 took.

## 2. What getAmbientColorC did, and what S19 changes

getAmbientColorC (0x1091e30, native since round 41) gives the ambient colour at a point from the ambient light grids
(4-bit samples, a palette per grid, bilinear over the four samples around the point; a sample past the cell's edge
comes from the neighbouring cell's grid). ExecGlobalLightMan calls it once per lit object (the global light query).
Per call, as guest calls through the table: GetAmbientLightObjGroup; the cell call (a relocated jal into the boot
ELF); a group walk to find the cell's grid (isysGroupForFirst, then GetAmbientLightClass and isysGroupForNext per
light object until the cell matches, isysGroupForExit); per sample (four) two floorf; per sample in a neighbouring
cell another whole group walk.

The walks' guest calls change nothing the walk leaves behind: ForFirst / ForNext / ForExit move each visited object's
walk count (+9) up and back down, ForFirst nests the isys lock (depth up and down, no semaphore call when this thread
holds it) - net zero. S19 (`SOTC_S19AMB`, A/B `s19amb`):

- the walk as host reads of the isys link table (0x21b900, 0x18-byte links): the group's first link, each link's next
  (+0xe), links marked removed (+0xc) and objects flagged (+0xa bit 0) passed over exactly as ForFirst / ForNext pick
  them; the class as GetAmbientLightClass answers it (object + 0x1c when its byte +8 is 0x14, else 0); the grid at the
  class's +0 -> +0x20 as before;
- GetAmbientLightObjGroup as its three instructions (*0x118cb20 + 0x14), v0 / v1 / ra / sp as its jal leaves them;
- floorf called as the native it is (decomp_leaves::floorF), not through the table (the same registers it leaves);
- the cell call stays a guest call (one a call).

The host path runs only when every relocated site names the function it stands in for (ForFirst / ForNext / ForExit
at both walks, floorf at both sites), on the game's runtime (not the guest worker's shell), and while this thread
already holds the isys lock (owner = GetThreadId, depth >= 1: ForFirst would only nest it). Otherwise, and when a link
or object is outside what the walk can read, the guest calls as before.

Counted (`[s19amb]`, p1 with S19 on): riding (chkRp1, 11 577 frames) 164 835 calls, **about 14 a frame** - one per
lit object's global light query, not hundreds - all of them on the host path (the lock was this thread's at every call:
0 not nested, 0 sites renamed); 157 221 host walks found a light object to compare (about one link a walk: the
ambient group holds the cells around the player), 1 257 720 floorf calls made straight (7.6 a call). Lake6 (chkLp1,
7 800 frames) 111 469 calls, 14 a frame. Attract (chkAp1, 13 200 frames) 97 213 calls, 7 a frame, almost all of them
with the group empty (1 046 links in all: the default colour path). So a call is ~11 us (0.41% self, its callees ~0.05%) of guest-call machinery and
arithmetic on Vita3K and S19 takes ~13 table calls out of each: ~180 a frame riding.

## 3. Equivalence (`SOTC_S19AMB_CHECK=1`)

Each call taken the host way is first run the old way, its results kept, the state put back, then run the host way
and compared: the colour qword, the cell call's four outputs, v0 (64 bits), f0 / f1, fcr31, vf5..vf7, pc, sp, ra. A
difference keeps the old way's results and is printed (the first 8).

| run (p1) | calls checked | differences |
|---|---|---|
| chkRp1, ride7, 600 s (mounted at 2 599, to frame 11 577) | 164 834 | **0** |
| chkLp1, lake6, 420 s (to frame 7 800) | 111 468 | **0** |
| chkAp1, attract, 420 s (to frame 13 200) | 97 212 | **0** |

373 514 calls in all, 0 differences, no call that left the lock changed. The check's shots (ride7 / lake6 / attract)
were looked at: normal.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s19amb SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

p1 with S19 on by its build default then (`SOTC_S19AMB_LOG=600`); frames >= 200 ms left out (the locked screen's ~1 s
host stalls); ride7 from the mount + 600 frames.

| run | route, length | frames compared | off - on, frame (2 SE) | busy |
|---|---|---|---|---|
| mixR7p1 | ride7, 700 s, seed 7 | 5 716 (7 483-13 308) | +0.394 +- 0.328 ms | +0.146 +- 0.319 |
| mixR11p1 | ride7, 700 s, seed 11 | 5 705 (7 004-12 837) | -0.137 +- 0.257 | -0.166 +- 0.267 |
| mixR13p1 | ride7, 900 s, seed 13 | (8 793-16 959) | +0.012 +- 0.215 | |
| mixL7p1 | lake6, 500 s, seed 7 | 6 662 (3 000-9 804) | -0.130 +- 0.146 | -0.137 +- 0.146 |
| mixA7p1 | attract, 500 s, seed 7 | (3 000-15 000) | -0.006 +- 0.145 | |

Even: one ride seed past 2 SE, the other two and both other routes at or around zero. The sampler (mixR7p1) bounds what
there is to see: the old walk's own samples (decomp_ambient::G::walk, off frames only) 306 per 1 000 off frames against
84 per 1 000 on frames for the host check (hostOk, both states): 0.02 ms a frame; the walk's callees from it
(isysGroupForExit 0.023%, isysGroupForFirst 0.002%) ~0.01 ms; the table calls inlined into getAmbientColorC (its self
time 0.39-0.41% in both states) cannot be split by state. A saving of ~0.05-0.1 ms a frame, a quarter of what the
frame A/B resolves. **Off by default** (in sotc_decomp.h's off list beside S12 / S14 / S16): exact, so the console's
A/B can try it (`SOTC_S19AMB=1`).

## 5. Boot, pictures, alignment

bootRp1 (p1, S19 on by p1's default then, ride7 route, 600 s, `SOTC_VITA_EAT_USER_MB=89` taken): booted on the first
try, mounted at frame 2 339, rode to frame 12 232, no fault; 177 456 calls on the host path. The ten shots (every 60 s:
the plain, Wander on Agro at the gallop, the shrine's arches, the hawk) normal: ground, Wander, Agro, shadows. Every run
of the round (the checks and A/Bs too) has ~220-260 frames of ~1 s: the Mac's locked-screen host stalls, the same with
S19 off (the A/B leaves frames >= 200 ms out). spalign on p1.elf: 0 sp-based :128 / :256 NEON accesses (of 979 with an
alignment hint).

The default (off) was set after the A/Bs, in sotc_decomp.h's off list only (no rebuild: the same p1 code with the
switch read the other way; a syntax check of an includer passed).

## 6. What it leaves

1. **The rest of the light pass**: ExecGlobalLightMan (0.51%, the same ~14 calls a frame riding: ~12 us each) builds
   each object's light record from the three global lights, the ambient colour and up to 32 local lights (inverse
   matrices, in / out boxes); lightPass 0.70%, objectLight, calcLightLayoutObj (round 43's cache), lightMatrices. A
   host light record per object kept while its position and the lights' bits stay the same would take the whole query
   (and getAmbientColorC with it), not just its guest calls.
2. **getAmbientColorC's own arithmetic** (the bilinear blend with the PS2's FPU clamps, the cell call through the
   relocated jal): a per-point memo checked against the four samples' bytes would be exact.
3. The profile's other unredesigned items are host code already (decodeKey, coll115's walk, the display walk) or the
   console's (the prefetch); callGuest from the particle draw list (0.53%, decomp_ptcl117::listImpl's per-node calls)
   is the next PS2-shaped item by size.
