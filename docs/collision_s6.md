# S6: collision, second pass: the water probes over a host index made on a worker (2026-11-17)

Round S6 is the second collision round (S2 was the first, docs/collision_s2.md). Its brief: move collision queries whose
inputs are known ahead, or which are independent of each other, to a worker in batches with a deterministic hand-off,
as S5 did for the particles; gameplay and picture the same.

Switch: `SOTC_S6COLL` (A/B name `s6coll`). Check mode `SOTC_S6COLL_CHECK=1`, counts and per-call timing
`SOTC_S6COLL_LOG=1`. Code: game/overrides/s6_coll.h / s6_coll.cpp (the index, the worker, the statistics), the two water
natives in decomp_collision.cpp (waterProbeBody, waterSurfaceBody), sotc_native.cpp (the frame end), sotc_decomp.h.
Scripts, runs and logs: build/agent-scratch/s6/ (STATE.md; abs6.sh the A/B, chk6.sh the check, prof.sh the sampler,
macrun.sh the Mac runner's hidden-window pictures, run.sh the scenes attract / lake6 / ride6).

## 0. The measuring routes

S5 found that the "lake" route's pictures showed the shrine's altar, and took it for the camera staying behind while
Wander was moved. The logs say otherwise:

- the route (in_tbz2.txt with `SOTC_TELEPORT=2000:boss4;7600:300,11.3,278.45,0 SOTC_TELEPORT_HOLD=600`) is at Avion's
  lake from frame 2 000 only until its **second entry at frame 7 600, which teleports Wander and Agro back to the
  shrine**: the teleport's numbers (300, 11.3, 278.45) are the console card's own load place on the altar (world
  -300, 11.31, -278.4, the `[pos]` line's numbers, which are the teleport's with x and z negated). The A/Bs analysed
  frames from 3 000 on, so most of every "lake" A/B since S2 measured the altar;
- in a run without the 7 600 entry the Mac pictures (runs/e1, shots every 150 frames) show Wander swimming in the lake
  from ~2 850 to the run's end with the camera on him: the teleport aid's camera follows. The `[pos]` line's "eye" is
  not the camera's position (its formula does not invert the view matrix), and its "player" stops updating while
  Wander swims (the player lookup reads a stale object), so neither says where the picture is;
- the r141 "ride" route (in_ride.txt) never mounts: Wander stands on the altar the whole run and Agro ambles nearby (the
  `[pos]` lines of every ride run since r141 show Wander at -300, 11.31, -278).

The fixed routes (build/agent-scratch/s6/, scenes in run.sh):

| scene | input | teleport | what it shows |
|---|---|---|---|
| `lake6` | in_lake6.txt: in_tbz2's inputs to frame 7 300, then a swim loop (stick up / left / up / right / down / right, 300 / 150 frames) to frame 80 000 | `SOTC_TELEPORT=2000:boss4 SOTC_TELEPORT_HOLD=600` (no return entry) | Avion's lake: the intro view of the pillars, then Wander swimming |
| `ride6` | in_ride6.txt: CROSS (call Agro) at 2 300, stick up 40 at 2 900, TRIANGLE at 3 000, CROSS + stick up at 3 600 (Wander runs to Agro; he mounts when the run brings him to Agro's side: three Vita3K runs mounted at frames ~4 100, ~5 000 and ~14 400, the Mac picture run not in 23 000 frames); from 4 300 blocks of 260 frames: CROSS twice (spur), stick up 200, stick left 50 | `SOTC_TELEPORT=2000:182.26,0,333.6,0h120` | the plain on the attract demo's gallop path (world -182, -334, south-east of the shrine): Wander runs in wide left turns and rides Agro once mounted; **not a fixed ride** |

**The ride route is not reliable**: mounting depends on where Agro's AI stands when Wander reaches him. The Vita3K runs mounted (at different frames); the Mac pictures (runs/mR1, 67 shots to ~23 000) show Wander running on the plain and along the shrine's walls, never mounted. Its A/Bs measure the plain with Wander on foot for part of the run and riding for the rest. The attract demo remains the steady gallop (Agro with Wander on the plain and through the canyon). A pad-script condition on the ridden state (the horse class's +0x950, read through the horse's handle) would make the mount certain.

While Wander rides, `[tpos]` / `[pos]` stop moving: the objects' +0x170 is not updated for a ridden pair (the Mac
pictures of a gallop show it), so a frozen position there means "mounted", not "stuck".

A new teleport entry `<frame>:ride` (sotc_replay.cpp) calls the game's nico_ForceChangePlayerHorseRidingControl
(0x13B6F88, a0 the player; the horse is the game's own, handle 0x1465904). It did not mount Wander in Vita3K (he ran on,
Agro stayed apart): the function switches the controls of a pair already placed by the mount animation. It is left as
a test aid but the route does not use it.

## 1. What the profile says

profL0: celosia-s5 (the base), the fixed lake route, Vita3K 222 MHz, the sampler from 120 to 600 s with round 63's
census on (`SOTC_COLL_CENSUS=1`: every collision entry wrapped, each outermost query timed by its function and return
address; its own cost is ~2% of the game thread and inflates many short calls). 11 881 frames, game thread 40.4 ms a
frame.

Collision and physics: **16.7% of the game thread (6.8 ms a frame)**. The queries by caller (census, ms a frame, at
frames 6 000 / 12 000, Wander in the water):

| query (return address) | calls a frame | ms a frame | what |
|---|---:|---:|---|
| waterProbe (0x1127530) | 12 | 2.11 / 1.26 | the under-end water probe (Wander's swim) |
| waterSurface (0x112778C) | 9-10 | 1.43 / 0.80 | the crossing water probe (Wander's swim) |
| SCRGetCollisionCallBack (0x13CB0AC) | 5 | 0.72 / 0.68 | gamecore_sub_013CAEE0's loop: a table of segments in an object's frame, one segment test each |
| SCRGetCollisionCallBack (0x145C8C0) | 0.9 | 0.67 / 0.65 | perceptionExec (an AI's sight line) |
| GetCollisionDBCallBack (0x10D4668) | 2 | 0.61 / 0.46 | the ground fill (Wander, Agro) |
| camera_collision_check_direct (5 sites) | 1-3 each | ~0.8 together | the camera's checks |
| FlushModifiedManager (4 sites) | 274 each | ~0.35 each | the per-part cache flushes (mostly empty lists; the census's wrapper is most of it) |

The sampler agrees on the water: waterProbeBody 1.40% and waterSurfaceBody 0.88% of the game thread (~0.9 ms a frame)
after S2's pre-tests. The segment tests' host code (coll115 walkT 1.85%, directCallBackBody 1.60%, leafTA9 1.12%,
clipDBT 0.86%, walk101Run 0.70%) is the larger part of the area but is spread over ~20 callers of 1-5 calls each.
Cores 1-2 (Vita3K): the render thread 46%, the draw-prep hook thread 17%, the rest under 5% each.

## 2. The redesign: a host index of the water mesh, made and vouched for on a worker

### The PS2's way after S2

A water mesh is a flat list in the collision DB (+0x38 the count, +0x3C a table of 0x20-byte triangles: +4 the height,
+8..+0x1C three xz vertices). Both water natives walk it on every call:

- **the under-end probe** (manager_sub_01117EA0, `waterProbeBody`: is the segment's end over a water triangle with a
  point under it): S2 runs its xz box test (outside(): a dozen loads and compares) on every triangle and the native's
  body (the barycentric test) on the few the box cannot rule out, then on the last one;
- **the crossing probe** (manager_sub_01117340, `waterSurfaceBody`: the water surface a segment crosses): S2 finds the
  last triangle whose height lies between the ends, then walks every triangle up to it with the height test and, for
  those between, the box test against the ends' xz box. On a flat lake every triangle has the same height, so a probe
  that crosses the surface walks the whole mesh through both tests.

Avion's lake mesh has ~1 500 triangles and Wander's swim asks ~12 under-end and ~10 crossing probes a frame.

### The index (s6_coll.cpp `make`)

Per mesh, from a copy of its table:

- **a uniform xz grid** (about count / 2 cells, at most 128 x 128): each cell lists, ascending, every triangle whose xz
  box widened by a margin M reaches the cell. M bounds S2's own margin (1e-3 of the box + 1e-3 + 2e-6 of the largest
  coordinate) for any query whose coordinates are at most `qb` (twice the mesh's largest coordinate + 4 096, at most
  9e4), with 1% + 1e-3 to spare;
- **the "always" list**: the triangles S2's box tests never pass over whatever the point (a coordinate not finite or
  huge, or thin), with wider thresholds than S2's (1e5 instead of 1e6, twice the thinness bound), so it is a superset;
- **the heights** sorted by the FPU compare key (FPU_C_OLT_S's own key: a zero exponent reads as +0), with a sparse
  table of the range maximum of their indices: the last triangle with p1y < h < p0y is two binary searches and two
  lookups.

### The probes with the index (decomp_collision.cpp)

- under-end: the cell under the end's xz, merged with the always list (ascending); each candidate goes through S2's own
  box test and, if kept, the native's body, in index order; then the last triangle. A triangle no candidate list holds
  has its box (with S2's margin) away from the end, so S2's test would pass it over: the bodies run are exactly S2's,
  in the same order, so the outcome and f0 are S2's.
- crossing: the last-between triangle from the heights; the candidates of the cells under the ends' xz box (at most 16
  cells, else S2's loop) and the always list; each candidate up to the last-between one goes through S2's height test
  and box test, the last-between one always runs (as in S2), then the last triangle. The same bodies in the same order:
  the same outcome, f0 and work / crossing blocks.
- Any coordinate over `qb` (or no index): S2's loop, which gives the same answer.

### The worker and the hand-off (deterministic)

The index lives on the host and is made off the game thread. At each frame's end (sotc_native, beside S5's join) the
game thread hands a job to a host worker (its own thread, cores 1-2, the game thread's float mode):

- every index probed in the last 120 frames: its table compared byte for byte with guest memory (equal: vouched for the
  next frame; changed: made again);
- every mesh asked for without an index (a probe that found none): made.

The game thread joins at its first water probe of the next frame (or the next frame's end): an index is used only in a
frame after the worker vouched for it. On every probe it also compares the table's first, middle and last triangles
with the copy. A probe in a frame with no vouched index (the first frames of a new mesh, a changed table) runs S2's
loop, which gives the same answer, so when the worker finishes changes nothing the game sees.

What the game thread no longer does per probe: the box test of every triangle (the under-end probe) and the height
test of every triangle plus the box test of every one at the water's height (the crossing probe); it visits the cell's
~10-20 candidates instead. What the worker does per frame: one memcmp of each live mesh's table (~48 KB for Avion's
lake), and a build (a few ms) when a mesh first appears or changes.

## 3. Equivalence

`SOTC_S6COLL_CHECK=1` (chk6.sh), in-run, from the same state both ways, no tolerance:

- every probe the index answers is also run through S2's loop: for the crossing probe the work and crossing blocks are
  saved before the index's run, put back, S2's loop run, and its blocks kept; compared bit for bit: the outcome (to the
  recompiled code or not), f0, the 0x40-byte work block and the 0x10-byte crossing block;
- "missing": every triangle S2's loop would run the body for (its box test keeps it; for the crossing probe, with its
  height between the ends) is looked up in the index's candidate list, and S2's last-between index is compared with the
  one from the sorted heights.

| run (build) | where | frames | under-end probes compared | crossing probes compared | differ | missing |
|---|---|---:|---:|---:|---:|---:|
| chkLp1 (p1) | `lake6`, 600 s | 16 398 | 121 661 | 101 036 | **0** | **0** |

In that run 75% of the probes went through the index (the rest: meshes under 32 triangles, which are not indexed, and
the first frames of a mesh); a probe by the index ran the body for 1.6 (under-end) / 1.4 (crossing) triangles, where
S2's loop box-tested ~74 / ~76 (S2_LOG: 888 / 758 triangles a frame over 12 / 10 probes). The worker's job (verifying
three tables) took 0.009-0.044 ms; the game thread never waited at a join (waits 0-1 in every run); no table changed
while in use (the spot checks never failed; "changed" only when a mesh streamed in). spalign on p1.elf: 846 NEON
:128 / :256 accesses, 0 sp-based.

## 4. Speed

`SOTC_AB_FLAG=s6coll SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs6.sh, 1 000 s each, frames from 3 000): S6 on or off at
random each frame. Frame times by tools/mixframes.py (`--shift -1`, blocks of 300 frames, +- 2 SE); the water probes'
host time per call by state (`SOTC_S6COLL_LOG=1`, [s6time], Welch z). Build p1.

| run | route | frame off - on | busy off - on | under-end probe: us a call off -> on (z); ms a frame | crossing probe: us (z); ms a frame |
|---|---|---|---|---|---|
| mixL7p1b | `lake6`, seed 7 | **+0.817 +- 0.109 ms** | +0.814 +- 0.108 | 90.4 -> 56.5 (**42.8**); 1.62 -> 1.02 | 119.8 -> 97.3 (**13.6**); 1.04 -> 0.85 |
| mixL11p1b | `lake6`, seed 11 | **+0.933 +- 0.128 ms** | +0.924 +- 0.128 | 95.1 -> 63.4 (**34.6**); 1.78 -> 1.19 | 124.6 -> 104.7 (**13.2**); 1.14 -> 0.96 |
| mixL7p1 | `lake6`, seed 7, the Mac at load ~33 (another job's fuzz tests), 15 fps | +1.218 +- 1.215 ms | +1.122 +- 1.172 | 105.1 -> 72.6 (9.0) | 166.8 -> 139.8 (4.7) |
| mixA7p1 | attract, seed 7 | +0.132 +- 0.265 ms | +0.120 +- 0.233 | 14.1 -> 13.7 (0.2) | 53.6 -> 54.4 (-0.3) |
| mixA11p1 | attract, seed 11 | -0.125 +- 0.193 ms | | 9.6 -> 10.1 (-1.5) | 30.1 -> 32.6 (-1.3) |
| mixR7p1 | `ride6`, seed 7 | -0.001 +- 0.116 ms | | 8.9 -> 9.3 (-1.1) | 12.2 -> 12.5 (-0.6) |
| mixR11p1 | `ride6`, seed 11 | -0.001 +- 0.117 ms | -0.013 +- 0.069 | 9.1 -> 10.4 (-1.0) | 11.1 -> 11.4 (-0.8) |

(mixL11p1, the first seed-11 lake run, never left the title screen: the boot menu's input was lost while the Mac was
loaded; run again as mixL11p1b. mixL7p1 is listed but not used for the decision: its noise is ten times the others'.)

**At the lake: about 0.8-0.9 ms a frame of game thread** (z ~15 and ~15 per seed by frame time, z 13-43 per probe kind),
2.1-2.4% of the ~38.6 ms game thread there; the probes' own time accounts for ~0.8 of it. The game thread at the lake:
busy 38.59 -> 37.83 ms (seed 7), 39.00 -> 38.09 ms (seed 11). **Away from big water nothing changes**: the attract demo
and the ride see only small meshes (under 32 triangles, not indexed; 9-54 us a probe), and their frame A/Bs are 0 within
+- 0.12-0.27 ms; the probes there are ~1 us slower with S6 on (the index lookup that finds nothing; ~0.001 ms a frame).

Other threads: the render thread unchanged (off - on -0.03..-0.08 +- 0.10 ms at the lake), the hook jobs unchanged; the S6
worker runs 0.009-0.027 ms a frame (one memcmp of each live table) and does not appear among the sampler's threads
(under 1% of a core).

**Decision: `SOTC_S6COLL` on by default.** Both lake seeds clear z > 2 by a wide margin in frame time and in per-call
time; the attract demo and the ride are neutral (no large water mesh), as expected from what was changed; the check is
exact.

## 5. What was not moved, and why

The brief named view clips, keep tests, water probes and ground probes. What the profile and the code show:

- **Batches of independent segment tests exist.** gamecore_sub_013CAEE0 (0x13CAEE0, recompiled) walks a table of
  segments (+0 start, +0xC end, 0x18 apart) in an object's frame and asks SCRGetCollisionCallBack for each (5 a frame at
  the lake, ~0.7 ms census): the segments are known before the first test. The camera's five checks
  (CAM_doit_post_update_new_camera_data_target_offset's two, CAM_act_ray_check_for_camera, CAM_doit_objective_target_
  visibility, gamecore_sub_01416FF8) come one per function, each used at once. Running a segment test on another core
  is not safe yet: the test makes planes, boundaries and vertices on demand into the per-part caches in guest memory
  and appends their indices to the modified managers' lists (EntryModifiedManager: a count read, a store, a count
  written), and coll115 / fin125 / seg80 keep host caches that only lane 0 may touch (round 108). Two tests at once
  would race on those lists. Making the caches safe for two writers (a host-side "made" bitmap per part, appends replayed
  on the game thread in the original order at the join) is the prerequisite; with it, gamecore_sub_013CAEE0's loop
  (natively: all its segments handed over, the first run here) is the first candidate, then the camera's two checks in
  one function.
- **Finder clips and keep tests** (the "view clips"): each clip's segment comes from the previous clip's answer
  (the slide, the landing, the IK feet), as S2 found (collision_s2.md, section 6); nothing to batch.
- **The ground fill** (GetCollisionDBCallBack, 2 a frame, ~0.5 ms census): one fill per character, each followed at
  once by that character's clips; Agro's fill could only run beside Wander's step if the fill's writes (the finder
  list, the plane caches) were kept apart as above.
- **The flushes**: four list flushes per part per object a frame (274 x 4 at the lake), almost all empty; round 119's
  flush119 (skip the empty ones) is exact and stays a console decision (off by default).

So S6 moved the part of collision that is data, not a query: the water meshes' index and its upkeep, which is what a
modern engine does with static collision (an acceleration structure built and validated off the main thread).

## 6. Pictures, boot and the console card

Pictures (the Mac runner of this tree, `PS2X_HIDDEN_WINDOW=1`, muted, a copy of the console card; looked at, then the
frames deleted, composites kept in build/agent-scratch/s6/runs/):

- **the lake, S6 on and off** (mL1 / mL0, `lake6`, shots every 600 frames from 3 000; mLcmp.jpg): Avion's pillars and
  the lake, Wander swimming with his ripples, once on a pillar's foot; the two states show the same water, ripples and
  swim. (Mac runs drift in time against each other, so later shots differ in where Wander is, not in how anything is
  drawn.)
- **the route fix** (e1, S5's Mac build): with the return teleport left out, Wander swims in the lake to the run's end
  and the camera follows him (e1a-d.jpg).
- **the ride** (mR1, `ride6`, 67 shots): Wander runs on the plain and along the shrine's walls; he did not mount in this
  run (see section 0). Earlier tries (e2: mounted, galloping on the plain) show what the mount looks like.

Boot: celosia-s6.vpk on `lake6` with `SOTC_VITA_EAT_USER_MB=89` in Vita3K (92 MB user memory left before vitaGL): to
frame 10 818, the index in use (12.5 of 16.6 under-end probes a frame), no fault.

**Console A/B:** build/agent-scratch/celosia-s6.vpk (= p1, commit 709a523; later commits change comments and docs only)
with build/agent-scratch/s6/env-console.txt (`SOTC_AB_FLAG=s6coll,s5ptcl,liftd12,a9sweep,s4disp,a9cmp`,
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=600`). The console gain is expected only where Wander is in or near a large water mesh
(the lakes, the rivers); riding the plains it is neutral.
