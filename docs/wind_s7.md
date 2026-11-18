# S7: a ride route that mounts, the ride's profile, and the wind query without its slot scans (2026-11-17)

Round S7's brief: the goal is a stable 30 fps on the Vita **while riding Agro across the plains**; the Vita3K game thread
sits ~38-42 ms on the heavy scenes and needs 33. Step 0 a fixed ride route, step 1 the ride's profile, step 2 the top
hotspots that can be moved or rewritten safely, each with an in-run check and an interleaved A/B.

Switch: `SOTC_S7WIND` (A/B name `s7wind`). Check mode `SOTC_S7WIND_CHECK=1`, counts and per-call timing
`SOTC_S7WIND_LOG=1`. Code: game/overrides/decomp_wind.cpp (the query and its host list, the sphere weight), sotc_native.cpp
(the A/B window), sotc_replay.cpp and ps2_pad.cpp (the route's aids). Scripts, runs and logs: build/agent-scratch/s7/
(STATE.md; run.sh the scenes, abs7.sh the A/B, chk7.sh the check, prof.sh the sampler, chain_ab.sh the queue, macrun.sh the
Mac runner).

## 0. The ride route (`ride7`)

### Why S6's ride did not mount, and what mounting needs

- **TRIANGLE beside a standing Agro does not mount.** Ten placements of Agro around a standing Wander (1.1 to 3 units, four
  sides, four yaws), with CROSS, CIRCLE then TRIANGLE, R1 held, a drop from above: none mounted (the player class's
  +0x950, the game's own "riding horse" field, stayed the player itself). R1 grabs (Wander hangs on Agro's side or on a
  wall and is dragged).
- **Wander mounts when he runs at Agro and TRIANGLE comes as he reaches him**: he leaps on (the Mac runs r11 / r15 / r16:
  mounted at frame 2 478 twice with the same inputs).
- **Fixed stick timings do not carry from the Mac to Vita3K**: the stick is camera-relative, and after the teleport
  Vita3K's camera sits so that "up" runs Wander *away* from Agro (7.6 units the other way in every try), the Mac's toward
  him.
- **S6's ride6 script was cut on Vita3K**: the Vita reads env.txt lines into a 4 KB buffer (ps2_vita_runtime.cpp), and
  ride6's PS2X_INPUT_SCRIPT is 23 KB: Vita3K ran its first ~200 entries only. Now 64 KB (static); ride7's script is < 1 KB.
- `nico_ForceChangePlayerHorseRidingControl` (S6's `ride` teleport) sets the riding fields but leaves Wander floating
  beside an invisible Agro with the reins stretched: not a mount.

### The aids (sotc_replay.cpp, ps2_pad.cpp; test aids, off unless set)

- **Host flags in the input script**: `@f<k>=<v>` waits on host flag k (any time, not in script order), `r<n>@f<k>=<v>`
  repeats a press until it holds, `<start>?f<k>=<v>` presses only if it holds when due (else skipped).
- **`SOTC_RIDE_FLAG`** (on with any `SOTC_TELEPORT`): flag 0 = Wander rides, read as the game reads it
  (nico_GetPlayerCharacterRidingHorseObject: the player class +0x950 is the horse, else the player) and the AI riding
  flag (nico_returnPlayerRidingFlag, asked every 10 steps); `[ride] frame N: mounted / on foot`. Teleports are skipped
  while Wander rides.
- **`SOTC_RIDE_STEER=<frame>`**: from that frame, while Wander is on foot, the left stick is steered toward Agro in closed
  loop on Wander's measured motion (so the camera's angle does not matter; a mirrored map is detected and flipped), and
  TRIANGLE is pressed in short beats within 2.2 units. Off as soon as he rides.
- Teleport entries take Agro's offset and yaw (`x,y,z,yaw,adx,adz,ayaw`); `ahead,<d>,<yaw>` puts Agro ahead of a running
  Wander (tried, not used).

### The route

`ride7` (run.sh): the console card; in_ride7.txt (the boot menu as ride6, then `@f0=1:WAIT` and the ride as a 520-vblank
block repeated for the whole run anchored to the mount: CROSS twice, stick up 400 vblanks, stick left 100);
tp_ride7.txt (`2000:182.26,0,333.6,0,6,0h120`: Wander on the plain on the attract demo's path, Agro 6 units off; then the
same every 900 frames while Wander is on foot); `SOTC_RIDE_STEER=2130`.

| run | where | mounted at frame | then |
|---|---|---:|---|
| m21 | Mac runner | 2 479 | riding to the run's end |
| m22 | Mac runner (loaded) | 9 858 | riding |
| mc1 | Mac runner, s7wind check | 4 945 | riding |
| profR0 | Vita3K 222 MHz (p0) | 6 606 | riding to the run's end (16 278) |
| chkRp1 | Vita3K (p1) | 3 302 | riding to 16 518 |
| mixR7p1 / mixR7p2 / mixR11p2 | Vita3K A/Bs | 4 071 / 2 551 / 3 942 | riding to ~18 000 |
| bootR3 | Vita3K (p3, 89 MB taken) | 4 219 | riding to 12 438 |

The mount frame varies (Agro trots off when chased; the steer retries until he is caught), but once mounted Wander rides
for the rest of the run, and the ride's inputs start from the mount: every A/B analyses the frames from the mount + 600.
Pictures: runs/ride7_mac_r16.jpg (Mac: the mount by the shrine wall, Wander on Agro, the gallop on the plain to frame
16 000), runs/ride7_vita3k_profR0.jpg (Vita3K window shots at 240-840 s: riding past the cliffs and the shrine's arches,
the gallop on the plain).

## 1. The ride's profile

profR0 (p0 = 0f3ba7a: celosia-s6's game code plus the route aids; ride7, Vita3K 222 MHz, the sampler every 60 s). The
window analysed is 360-840 s, all of it riding (mounted at frame 6 606, ~330 s): 9 111 frames, 19 fps under the
sampler, **game thread 34.9 ms a frame** (66% of a core; the render thread 44% of core 2, the draw-prep hook thread 18%).

| area (tools/decomp_tree.py) | share of the game thread | ms a frame |
|---|---:|---:|
| Display and models | 24.1% | 8.4 |
| Other game code (scripts, control, motion managers, wind, strips) | 22.8% | 7.9 |
| Characters and motion | 17.2% | 6.0 |
| Runtime (DMA walk, memory, scheduler) | 11.3% | 4.0 |
| Collision and physics | 9.4% | 3.3 |
| System and memory | 3.9% | 1.4 |
| Particles and effects | 2.9% | 1.0 |
| Boot ELF, SDK and libc | 2.8% | 1.0 |

The profile is flat: no symbol reaches 2%. The top ones (share of the game thread; x 34.9 ms):

| symbol | share | ms a frame | area |
|---|---:|---:|---|
| decomp_collision::coll115::walkT | 1.61% | 0.56 | collision (the segment walk, rounds 115-138) |
| decomp_walk::quad117BodyA9 | 1.45% | 0.51 | the strips' quads (grass, round 117) |
| sotc_native::modelDispHook | 1.39% | 0.48 | display |
| memcpy (+ sceClibMemcpy 0.66, memmove 0.50) | 1.28% | 0.45 | runtime |
| decomp_prefetch::note (+ objectLines, aheadWalk*, pf141 notes: ~2.2% together) | 1.09% | 0.38 | display / characters (prefetch for the console's memory latency) |
| dispDispObjGroupModel | 1.06% | 0.37 | display |
| **decomp_wind::windZones** | **1.04%** | **0.36** | the wind query |
| char122::decodeKey | 1.04% | 0.36 | characters |
| s3chara::pose | 0.93% | 0.32 | characters |
| obj125Run | 0.87% | 0.30 | collision |
| decomp_finder::finder | 0.78% | 0.27 | other |
| sceKernelGetProcessTimeWide | 0.63% | 0.22 | runtime (timers) |

**The wind query is the largest single game subsystem left that is self-contained**: windZones 1.04% + GetWindVector
(decomp_hot48c::windVector) 0.33% + the zones' weight functions (the sphere's 0.11%, the box's 0.15%) + the wind managers
~0.17%: **~1.8% of the game thread, ~0.63 ms a frame riding**. Riding asks it ~59 times a frame (Wander's cloth and hair,
Agro's mane and tail, the grass), each over the 80 zone slots. The rest of the top list is code that earlier rounds already
made native and tuned (the segment walk, the strips, the model hook, the pose, the key decoder), or memory latency hiding
that only the console can judge (the prefetch notes). Step 2 takes the wind.

## 2. The redesign: the wind query over a host list of its zones, the sphere zones weighed as host code

### The PS2's way (after round 6 / round 54)

The wind is a set of zones in 80 slots (a pointer table at 0x1216EF0); a zone has a kind (+0: 0 a sphere, 1 a box with an
inverse matrix, else a script's judge volume), an active flag (+0xA0), a priority (through +4) and its shape (+0xB0 the
centre, +0x100 / +0x104 the inner and outer radii for a sphere). GetWindVector (0x1146A50, native since round 48) asks
manager_sub_01145F78 (`windZones`, native since round 6) for the zones acting on a point: every active zone is weighed by
its kind's function (guest call: manager_sub_01146EB8 / 01146FB8 / 01147070), the ones with weight > 0 go into a result
list sorted by priority (a priority of -100 000 is the only result). Riding it runs ~30-60 times a frame. Each call:
- an up-front pass over the 80 slots checking every non-empty zone's pointers (so a bad zone goes to the recompiled code
  before anything is written);
- the loop over the 80 slots again (round 54 skips the empty ones in a tight loop);
- a guest call per active zone (callGuest's set-up, the table lookup, the lifted function, the return check).

### What S7 does (decomp_wind.cpp, `SOTC_S7WIND`, lane 0 only)

- **The host list of the zones**: the non-empty slots' indices in ascending order, made from a copy of the 320-byte slot
  table. Each call compares the live table with the copy (40 64-bit words OR-reduced; the Vita's `memcmp` goes a byte at a
  time and made the first build, p1, 0.7 us a call *slower*), and makes the list again when they differ (zones come and go
  as Wander rides: 24-32 rebuilds in a 16 000-frame ride). Both passes then visit only the listed slots, in the same
  order, reading each zone live as before: the same zones, the same checks, the same order.
- **The sphere zones weighed as host code** (`s7::weighSphere`): manager_sub_01146EB8's operations in its order, with its
  delay slots (the offset to the centre with PS2_VSUB, its squared length with PS2_VMUL / PS2_VADD_K as the VU0 macro code
  computes it, the falloff between the radii with FPU_C_OLE_S / ps2FpuDivide / FPU_SUB_SF), writing the same two outputs in
  the caller's frame and leaving at / v0 / v1, f0..f4, vf5..vf7, the C bit, ra, sp and pc as the guest call leaves them.
  Used only when the call site's jal still targets 0x1146EB8.
- Box and judge-volume zones stay guest calls (the box's GetRateInOutCubeVolumeWithInverseMatrix is decomp_light's native
  already).
- The worker shell (round 91's lane 1) runs the original; the A/B window is `s7wind` (sotc_native's abSwitches sets
  decomp_wind::g_s7AbOff; no sotc_decomp.h switch, so no header change).

Riding, every live zone in Vita3K's ride was a sphere or a box (chkRp1: 1 719 435 spheres weighed as host code, 838 920
boxes through the guest, no judge volume), ~3 live zones a call on the Mac's ride.

## 3. Equivalence

`SOTC_S7WIND_CHECK=1` (chk7.sh), in-run, no tolerance: every call is run the S7 way and then, from the same saved state
(the whole R5900 context, the result list's 2 592 bytes, the call's 0x70-byte frame), the original way; compared: the
result list, the frame, every GPR, every FPR, fcr31, pc and vf5..vf7. The original's outcome is kept.

| run (build) | where | frames | calls compared | live zones (sphere by host / box by guest) | differ |
|---|---|---:|---:|---|---:|
| mc1 (Mac runner, p1 code) | `ride7` | 8 028 | 448 180 | 1 294 574 / 0 | **0** |
| chkRp1 (p1) | `ride7`, Vita3K 222 MHz | 16 518 | 704 957 | 1 719 435 / 838 920 | **0** |
| bootR3 (p3, `SOTC_S7WIND=1`, `SOTC_VITA_EAT_USER_MB=89`) | `ride7`, Vita3K (mounted 4 219) | 12 438 | 641 455 | 1 730 448 / 706 241 | **0** |

(p2 / p3 differ from p1 only in how the slot table is compared with its copy, and p3 in the default.) spalign: p1.elf
850 NEON :128 / :256 accesses, 0 sp-based; p3.elf 850, 0 sp-based.

## 4. Speed

`SOTC_AB_FLAG=s7wind SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs7.sh, 1 000 s each): S7 on or off at random each frame.
Frame times by tools/mixframes.py (`--shift -1`, blocks of 300 frames, +- 2 SE), from the mount + 600 on `ride7`, from
3 000 elsewhere; the query's host time a call by state (`SOTC_S7WIND_LOG=1`, [s7time]); the sampler's NEW
(decomp_wind::s7) against OLD (the sphere weight's lifted code). Build p2 (7235443).

| run | route | frames | frame off - on | busy off - on | windZones a call off -> on | sampler NEW / OLD per 1 000 frames |
|---|---|---:|---|---|---|---|
| mixR7p2 | `ride7`, seed 7 (mounted 2 551) | 14 569 | **+0.146 +- 0.140 ms** | +0.141 +- 0.140 | 13.37 -> 10.84 us | 271 / 313 (z 4.9) |
| mixR11p2 | `ride7`, seed 11 (mounted 3 942) | 13 941 | **-0.101 +- 0.146 ms** | -0.068 +- 0.158 | 12.98 -> 10.33 us | 386 / 427 (z 4.0) |
| mixL7p2 | `lake6`, seed 7 | 15 026 | +0.124 +- 0.128 ms | +0.107 +- 0.122 | 22.68 -> 19.97 us | 138 / 158 (z 3.2) |
| mixL11p2 | `lake6`, seed 11 | 15 107 | **+0.222 +- 0.113 ms** | +0.197 +- 0.107 | 22.16 -> 19.80 us | 130 / 155 (z 4.0) |
| mixA7p2 | attract, seed 7 | 22 312 | **+0.262 +- 0.150 ms** | +0.262 +- 0.130 | 19.20 -> 17.53 us | 142 / 162 (z 3.7) |
| mixA11p2 | attract, seed 11 | 23 094 | +0.063 +- 0.154 ms | +0.037 +- 0.143 | 11.02 -> 8.65 us | 220 / 269 (z 7.2) |
| mixR7p1 | `ride7`, seed 7, p1 (byte-wise memcmp) | 13 367 | +0.107 +- 0.228 ms | +0.133 +- 0.243 | 12.13 -> 12.85 us (slower) | |

Riding the query runs ~48 times a frame (~730 000 calls over the ~15 000 ridden frames of a run): 2.5-2.7 us saved a call
is **~0.12-0.13 ms a frame** of game thread (~0.4% of the ride's ~33.5 ms). The render thread and the hook jobs do not
move (render off - on -0.17..+0.02 +- 0.11..0.18 ms).

## 5. Decision: `SOTC_S7WIND` off by default, on in the console A/B

The rule (keep a switch on only when its frame A/B clears z > 2 in both seeds) is not met on any route: the ride +0.146
(z 2.1) / -0.101 (z -1.4), the lake +0.124 (z 1.9) / +0.222 (z 3.9), the attract demo +0.262 (z 3.5) / +0.063 (z 0.8) ms.
The six runs pooled read +0.12 +- 0.06 ms (z ~4), five of six positive, and the per-call time and the sampler agree in every
run (2.4-2.7 us a call, z 3-7): the change is a real but small gain, below what one 1 000-s frame A/B resolves (+- 0.11-0.15
ms). So `SOTC_S7WIND` ships **off** (exact, checked, `SOTC_S7WIND=1` turns it on), and env-console.txt turns it on with
`s7wind` first in the rotation so the console's own A/B (where a guest call's round trip costs more than in Vita3K)
decides.

**What S7 does not do**: it does not close the gap to 33 ms. The ride's game thread is ~33.5 ms in these runs (34.9 under
the profile's sampler), and the wind was the largest self-contained subsystem left; the profile is flat. The remaining
single items over 0.3 ms are code earlier rounds already made native (the segment walk 0.56 ms, the strips 0.51, the model
hook 0.48, the pose and the key decoder ~0.35 each) or the console's memory-latency aids (the prefetch notes ~0.77 ms
together in Vita3K, where they cannot pay off).

## 6. Pictures, boot and the console card

- runs/ride7_mac_r16.jpg (Mac runner, window shown): the mount by the shrine wall, Wander on Agro, the gallop on the plain
  to frame 16 000.
- runs/ride7_vita3k_profR0.jpg (Vita3K window shots, p0): riding past the cliffs and the shrine's arches, the gallop.
- runs/ride7_vita3k_chkRp1_s7on.jpg (Vita3K, p1, S7 on and checked): the gallop on the plain, Agro and Wander, the
  ground and the dust as in the S7-off shots.
- runs/ride7_vita3k_bootR3_eat89.jpg (Vita3K, p3 with `SOTC_S7WIND=1` and `SOTC_VITA_EAT_USER_MB=89`): Wander steered to
  Agro on the plain, then the gallop with its dust, under the colonnade, on the plain.

Boot: celosia-s7.vpk (p3) on `ride7` with `SOTC_VITA_EAT_USER_MB=89` ("taken") and S7 on in check mode: to frame 12 438,
mounted at 4 219, no fault, 0 differences.

**Console A/B:** build/agent-scratch/celosia-s7.vpk (= p3, commit ef0f3b9) with build/agent-scratch/s7/env-console.txt
(`SOTC_S7WIND=1`, `SOTC_AB_FLAG=s7wind,s6coll,s5ptcl,liftd12,a9sweep,s4disp,a9cmp`, `SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=600`).
For a riding test on the console: the ride7 aids are in the build (`SOTC_RIDE_STEER=2130` and the teleport list in
tp_ride7.txt, with in_ride7.txt), but they are test aids; a player riding by hand measures the same thing.

## 7. Next

- The wind's second half: GetWindVector's per-source call (manager_sub_01146DA8, 0.33% + its callee) and the box zones'
  weight (manager_sub_01146FB8 through decomp_light's rateCube native) as host code in the same way would take the
  query's remaining guest round trips (~0.84 M box weighings in a 16 000-frame ride).
- The ride's next targets are not single functions: the display walk's per-object work (modelDispHook +
  dispDispObjGroupModel + mdsPlan / mdsPart ~1.2 ms) and the characters' per-frame chain (~6 ms, already split by S3 /
  round 141's analysis: no independent unit riding) need structural designs, not a native each.
- A console A/B of the prefetch aids riding (pf141 / pfc141 / walk67: ~0.8 ms of game thread in Vita3K) would say whether
  they still pay on the A9.
