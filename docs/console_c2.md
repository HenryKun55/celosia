# Console C2: the display group as a host scene list (c2scene)

Round C2 of the console work (2026-11-20). Source of the task: docs/console_profile_2026-11-20.md. On the console the
game thread is CPU-bound at ~40 ms a frame riding; the display cluster (the display walk, the model hook, the light
pass, the layout) is 7.5-8.2 ms of it, and the profile says the cost is memory latency, not instructions.

Scripts, runs, logs: `build/agent-scratch/c2/` (NOTES.md has the exact commands; walkpcs.py the console profile by pc
inside the walk).

## 1. Where the display walk's own time goes on the console

prof129 sets the sampled `pc` at every guest call and return, so inside a native that keeps the guest frame (the
walk, the light pass) the pc is the return site of its last call: the samples by pc split the walk into stretches
between calls (`walkpcs.py run1/err.txt`, windows 1-2, % of the game thread's running samples, run1):

| pc (the stretch after) | % | ms a frame | what runs there |
|---|---|---|---|
| 0x10f550c (a layout object's update / clip140's apply) | 0.92 | 0.39 | the group pass per layout object: the shown test, AddSortObj, +0x260, the next entry, the next object's class lines |
| 0x10f5334 (isysGroupForFirst) | 0.60 | 0.25 | **round 140's clip helper list: a full walk of the group chain on the game thread** (each entry, each object, a layout object's class +0x140 / +0x1b6 and record +0xd8 lines), then the first object |
| 0x10f55e8 (a geodisp's update) | 0.48 | 0.20 | the group pass after a particle geodisp |
| 0x10f5648 / 0x10f5684 (group exit, the copy loop's guest class call) | 0.47 | 0.20 | the second sort list's copy loop |
| 0x10f4fec / 507c / 5108 / 5178 / 50c0 / 504c (light pass) | 0.92 | 0.39 | the light pass's three walks over the sort list between their callbacks |
| 0x10f575c / 5794 / 56f0 (draw loop) | 0.46 | 0.20 | the draw loop between draws |

Two things stand out. (1) 0.25 ms a frame is a pointer chase that exists only to build round 140's helper list: the
group chain is walked entry by entry (each step needs the previous entry's next index), and every object's lines are
fetched cold, one dependent miss after the other, ~1 050 32-byte lines a walk riding. (2) The light pass's walks,
~0.4 ms, have no prefetch at all (round 67 gave the display walk's own passes one, not the light pass), and
InitLayoutObjMatrix (0.26%, 0.11 ms) walks the same chain at the frame's start the same way.

(ptcl91 is off by default, so the walk's worker join, which is not marked as a wait for prof129, does not inflate
wrap<10> in this profile.)

## 2. What changed (`SOTC_C2SCENE`, A/B `c2scene`, kC2Scene, off by default)

`game/overrides/c2scene.h`, `decomp_walk.cpp` (namespace `c2`), hooks in `decomp_light.cpp` and `decomp_group.cpp`.

- **The scene list.** The display walk's group pass records, in the walk's order, what it read anyway: per group entry
  the entry, the object, its layout class, the class's geometry record (+0x140, the sort node) and geometry (+0x144),
  and whether round 140's helper may clip it (no billboard / camera link, +0x1b8 set, update ClipLayoutObjModel, no
  clip callback). No extra guest read. When the pass ends normally the record becomes the scene (a walk that goes to
  the recompiled code midway keeps the previous one); with it a node -> record hash for the light pass.
- **Round 140's helper list from the scene.** clip140::post takes the last walk's objects the helper may clip from the
  host scene instead of walking the guest chain. It is a guess, and a wrong guess cannot change a result: the helper
  still computes each clip from live memory during this walk; the walk still takes a result only for the object it
  was computed for, at its own turn, only where round 140 takes one (and, for a list from the scene, only with no
  clip callback at that moment). An object the list misses or misplaces is clipped by the walk as before. With no
  scene for this group (the first walk after the switch turns on, a new group) post walks the chain as before.
- **Prefetches from the scene** (PLD only; every read, write and decision is the old path's), by level
  `SOTC_C2SCENE_PF` (default **0: none**, 1: the group pass, 2: also InitLayoutObjMatrix and the light pass's first
  walk; see section 4 for why the default is 0):
  - the group pass (level 1): at each object's turn the scene's records 1-4 on: the entry, the object's type line,
    the class's +0x140 and +0x1b6 lines, the record's callbacks (+0xd8), the geometry's +0x260 word; independent addresses, all in flight together, in place of round 67's chain runner
    (which chases the entries one dependent load at a time and asks only for the entries). The runner comes back
    when the scene stops following the walk;
  - InitLayoutObjMatrix (level 2): the entry, the type line, the class's +0x144 line, the geometry's +0x260;
  - the light pass (level 2): the sort list's nodes are read once into a host array (the bucket heads' lines asked
    for together first), each with its scene record (a node -> record hash built at the group pass's end); the first
    walk asks, 4 nodes on, for the object's type line, the record's light callback (+0xe0), the class's +0x140 line
    and the geometry's +0x260.
- **Run-time flips.** The switch is latched per walk; the scene is only recorded while on and is rebuilt by the first
  walk after it turns on (that walk uses the chain as before), so abSwitches can flip it per A/B window.
- `SOTC_C2SCENE_CHECK=1`: every clip the walk takes from a list that came from the scene runs the old way too and is
  compared (round 140's check: the record and geometry words, v0, f0); `SOTC_C2SCENE_LOG=<walks>` (600 with the check):
  `[c2scene]` counts (scene size, helper lists from the scene / the chain, the chain lines not read, how often each
  walk found its object in the scene, lines asked for).

## 3. Exactness

Prefetches cannot change a value; the scene changes only which objects round 140's helper precomputes, and every
result the walk takes is checked by `SOTC_C2SCENE_CHECK=1` (the clip run the old way, round 140's compare). Build p1
(02570ff), Vita3K 222 MHz, `SOTC_C2SCENE=1 SOTC_C2SCENE_CHECK=1`:

| run | clips checked (all from the scene's list) | differ | scene a walk (layout) | helper list from the scene | group pass found its object in the scene |
|---|---|---|---|---|---|
| chkR, ride7 600 s (p1, mounted at 3 931) | 1 520 000 | 0 | 234 (189) | 100% of walks, 177 objects | 233.3 of 234.2 |
| chkL, lake6 400 s | 345 148 | 0 | 148 (81) | 100%, 53 objects | 147.1 of 147.5 |
| chkA, attract 400 s | 660 000 | 0 | 169 (111) | 100%, 81 objects | 168.7 of 169.3 |

InitLayoutObjMatrix found every object in the scene (234.0 / 147.5 / 169.3 a frame, 0 missed); the light pass's walks
found every node (0 missed). The scene follows the group almost exactly from walk to walk (<1 object a walk missed:
objects streaming in / out).

## 4. Vita3K A/B

In-run A/B (`SOTC_AB_FLAG=c2scene SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`, `SOTC_C2SCENE=1`), Vita3K 222 MHz, off - on
(> 0: on faster), 2 SE:

| build | prefetch level | ride7 800 s seed 7 | ride7 800 s seed 13 | lake6 400 s | pooled (2 SE), z |
|---|---|---|---|---|---|
| p1 (02570ff) | group pass, init, light pass x3 walks, a counter per line | -0.416 +- 0.127 | -0.416 +- 0.128 | -0.272 +- 0.107 | -0.37 +- 0.07, z -10 |
| p2 (834494d) | 1 (group pass, 6 lines an object) | -0.054 +- 0.161 | -0.145 +- 0.159 | -0.063 +- 0.123 | -0.09 +- 0.09, z -2.0 |
| **p3 (c0a900e)** | **0 (the helper list only)** | **+0.042 +- 0.123** | **+0.017 +- 0.130** | **+0.024 +- 0.160** | **+0.03 +- 0.08, z +0.7** |

The prefetches cost instructions in Vita3K and buy nothing there (no cache model): p1's sampler had the bookkeeping
(lightAhead 0.23%, groupAhead 0.16%, initAhead 0.14%, lightBegin + hash 0.13% of the game thread, pooled over on and
off frames) at ~0.4 ms an on-frame; p2's group-pass prefetch alone ~0.12% pooled, the whole of its -0.09. So the
switch's default is level 0, which is even in Vita3K (the chain walk it removes is cheap there: Mac caches), and its
console gain is the helper list's ~1 000 serial misses. Levels 1 / 2 are a separate console question: a DRAM miss
costs ~250 ns there and a PLD ~a cycle, so the console may well want them; they are exact either way.

## 5. Boot

bootR3 (p3, ride7 600 s, `SOTC_VITA_EAT_USER_MB=89 SOTC_C2SCENE=1`): booted first try, mounted Agro at frame 3 356 and
rode to the end; shots t0060-t0540 normal: Wander on Agro, the ground, the sky, the cliffs and ruins, no missing or
flickering objects. The helper list came from the scene on every walk (229 of 270 objects, ~1 260 chain lines =
40 KB a walk not read). p1's boot (bootR, all prefetch levels) also rode normally. A p3 check ride (chkR3, 500 s,
`SOTC_C2SCENE_CHECK=1`): 1 260 000 clips checked, 0 differences. spalign (p3 = c2.elf): 0 sp-based :128/:256 NEON
accesses (of 1 026 with an alignment hint).

## 6. Memory traffic

Vita3K's timing has no cache, so this is counted, not timed (32-byte lines, the A9's L1 line; `[c2scene]` counts,
riding = chkR, ~1 walk a frame):

| | before | after |
|---|---|---|
| round 140's helper list (game thread, at the walk's start) | the group chain walked: ~1 033 lines = **33 KB** of guest reads a walk riding (17 KB at the lake, 21 KB attract), each a dependent miss (entry -> next index -> entry -> object -> class -> record) | 0 guest bytes; the host scene (24 bytes x 234 = 5.6 KB, contiguous, written by the pass that reads those words anyway) read once |
| group pass, InitLayoutObjMatrix, light pass (bytes touched) | unchanged | unchanged (same reads) |
| ... of them demand misses | the chain runner asked only for the entries 4 on; the objects' class / record / geometry lines, InitLayoutObjMatrix's and the light pass's lines all demand-fetched | ~4 800 line requests a walk riding issued 1-4 objects ahead (group pass ~9 lines x 233 objects, InitLayoutObjMatrix ~6 x 234, the light pass ~7 x 325 node visits; some repeat lines already in L1) |

At the profile's ~250 ns a DRAM miss, the helper list's ~1 030 serial misses are ~0.26 ms a walk, which is what the
console's 0x10f5334 stretch measured (0.25 ms). The prefetched lines are not fewer bytes; they overlap instead of
coming one after the other, so the gain there depends on how many were misses (an upper bound ~0.3 ms if every one
was, nothing if all hit L2).

## 7. For the console

The verdict is the console's A/B (Vita3K models no cache): `SOTC_C2SCENE=1 SOTC_AB_FLAG=c2scene SOTC_AB_PERIOD=300`
(the A/B only turns switches off, so the env needs the switch on). Build: `build/agent-scratch/c2/c2.vpk` / `c2.elf`
(= p3). What the console should show if the latency reading of the profile is right: wrap<10>'s 0x10f5334 stretch
(~0.25 ms a frame, prof129) mostly gone. A second console A/B with `SOTC_C2SCENE_PF=2` added to the env (both halves;
the prefetches act only in the on half) tells whether the prefetches pay there (expected at most another 0.1-0.3 ms).

## 8. What is left

- The group pass itself still reads ~7 lines per layout object live (now asked for ahead); a cut needs the lines
  themselves fewer (a host copy of per-object state would have to be checked against the guest words, which reads the
  same lines) or the game's writes to them known.
- The draw loop keeps round 67's stage prefetch (dependent reads 3 and 1 nodes on); the scene's records could give it
  independent addresses too.
- The model hook (wrap<12>, 0.8 ms), solidDisp (0.6), the light callbacks (execGlobalLightMan 0.33, calcLightLayoutObj,
  getAmbientColorC, objectLight) are untouched.
