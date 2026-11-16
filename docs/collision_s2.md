# S2: the collision subsystem's redesign, part 1 (2026-11-15)

Round S2 is the second subsystem round (S1 was the particles, docs/particles_s1.md). Its subject is collision and physics:
the collision library (GAMECORE 0x01111000-0x01136F00) and the characters' root dynamics (0x010D4000-0x010EBA00),
mapped by D9 in docs/collision_map.md. The aim, as in S1: code that runs the way a modern simple machine wants it (the
PS Vita first), not the PS2's way, with the same gameplay and picture.

Switch: `SOTC_S2COLL` (default on, see section 5; `SOTC_S2COLL=0` gives the round-138 path back). A/B name `s2coll`.
Check mode `SOTC_S2COLL_CHECK=1`, counts and per-clip timing `SOTC_S2COLL_LOG=1`.
Code: game/overrides/coll_types.h (the semantic layer), game/overrides/s2_coll.h (the host list views, included by
decomp_collision.cpp after coll125.h), small hooks in decomp_collision.cpp (finderClip / finderClipBody, keepEarlyFrom)
and coll125.h (a slot generation counter).
Scripts, runs and logs: build/agent-scratch/s2/ (STATE.md, abs2.sh, tools/).

## 1. Semantic layer: coll_types.h

game/overrides/coll_types.h names the collision data with C++ types and offsets, pinned with static_asserts, as
ptcl_types.h did for the particles. They are views on guest memory (guest RAM is host memory, little-endian), with
may_alias members.

| type | what |
|---|---|
| `SurfaceId` (8) | how every client names a surface: the layout object, then a word (bit 31 independent / mesh, bits 24-30 the part, 0-23 the surface); helpers `independent`, `partOf`, `surfaceOf`, `key` |
| `object::` / `layout::` | the object's type byte and the layout class's collision fields (scale, position, the part matrices, the collision work, the record, the enable bytes) |
| `DB` | the collision DB (kind, parts, faces, tree, bound radius, attributes, water triangles) |
| `Part` (0x90), `SurfaceShort` (0x14), `SurfaceLong` (0x34) | a part record and its two surface record forms |
| `Tree`, `Node` (8) | the split tree and its node word (axis, children, leaf bits) with decoders |
| `Hit` (0x50) | the segment test's result record |
| `ModifiedManager` (0x18) | the game's per-frame cache of one kind (count, flags, index size, list, values) |
| `work::` | the collision work's layout (finder links, per-part caches, the skinned caches) |
| `Finder` (0x14), `FinderQuery` | a contact list and the finder leaf's query |
| `GroupLink` (24) | an isys group link (0x21B900) |

Who outside the library reads these (why they stay in guest RAM): the root dynamics and every gameplay client hold
surfaces by id and read the hit record and the finder lists; the modified managers are the game's own caches. S2's host
structures copy from them into host memory (section 2) and check against them; they do not replace them.

## 2. The redesign: finder clips on host list views

### What the profile said

D9's sampler had collision at 13.7% (attract) / 21.6% (lake) of the Vita3K game thread, almost all host code already.
S2's own counts (`SOTC_S2COLL_LOG`, Vita3K 222 MHz) show where the per-frame work is repeated: a character's contact
list (the ground fill's finder) is clipped **15-23 times a frame** (the landing ray, the IK feet, the slide's probes),
and the keep pass tests the same list's entries again (**19-213 keep tests a frame**). Each clip walked every entry
through round 125's surface cache (fin125): a hash probe over 190-byte slots (up to 8), its frame / epoch / work-epoch
check (3-5 dependent loads), its LRU stamp store, then the clip, one entry at a time. Riding: ~255 entries a frame through
that, the lake ~1 100.

### The view

After a clip that went through fin125 (coll125::run) with every entry current in its cache, the list gets a host view
(s2coll::build): the id words copied, each entry's slot pointer, and the entries' planes as structure of arrays
(nx[], ny[], nz[], nw[]). A later clip of the same list uses the view while nothing it rests on has moved:

- the same game frame;
- no flush since: fin125's global epoch, its worker epoch and its count of work-epoch bumps unchanged;
- no fin125 slot taken over or made again (coll125::g_gen, new in S2, moved by claimSurf's eviction and by make);
- the list's id words byte for byte the view's (n x 8 bytes, one memcmp).

Under those conditions every entry is current in fin125's cache, which is exactly when coll125::run uses its kept
surface without a check. The view path calls the same `coll125::clip` on the same `Entry` objects in the same order,
asks the id filter at the same entries (only on a hit), and writes the hit record the same way: the same answer, the
same record, the same guest writes. A view is built only when all entries are kept host surfaces with small planes
(finite, below 2^61), and used only when the segment's ends and vf0 are small, so clip() can never answer "ask the guest's
ClipCollisionDB" for any entry and no guest call can come between entries. A flush on the worker's lane during a view
clip makes the caller run coll125::run instead (nothing has been written but the segment's copy at sp, which run writes
first too).

### Four entries at a time (the Vita)

clip()'s first step is round 49's NEON plane distances: ((vf0.x + n.x a.x) + n.y a.y) + n.z a.z + n.w for both ends.
The view computes the same lane operations in the same order with four entries in the lanes (s2coll::blockMask), so
the bits are the same, and applies clip()'s own rejection on them (start behind the plane or end in front). A rejected
entry is passed over without touching its slot. Entries that pass, and every entry after a hit in the same block of
four (the segment is shorter then), go through clip() itself. On x86 (the Mac) the view calls clip() for every entry
(no NEON block test; the same answers).

### The sphere pre-test

For an entry that passes the plane test, clip() computes the crossing point (a PS2 division and a VU lerp) and
rejects it when its squared distance to the surface's boundary sphere's centre is over r^2. The crossing point lies on
the segment, so a surface whose sphere is out of the segment's reach cannot hit. s2coll::sphereFar tests that in plain
floats with a margin (3e-5 of the largest coordinate + 1e-3 on the distance, 1e-5 on the squared limit), which covers
the crossing point's rounding and the test's own; anything not finite goes to clip(). It only ever skips entries clip()
rejects (the check mode below compares every view clip with the original).

### The keep pass

ClipCollisionFinderWithKeep's keep test (decomp_hot44b, through decomp_collision's keepEarlyFrom) asks first whether
clipKeep answers at its first return (the point at least `keep` in front of the plane), which round 125 answers from
fin125's cache (a hash probe and the epoch checks per entry). With S2 the entry's slot comes from the list's view when
the view holds and the entry's id words are the view's: the same test on the same Entry.

### What did not change

The finder fill (GetCollisionDBCallBack), the segment tests (ClipCollisionDirectCallBack), the tree walks and the
guest's caches are as before. fin125 keeps its role (making and validating the kept surfaces); the views sit on top of
it, so `SOTC_FIN125=0` also turns S2's views off.

## 3. The redesign: the water probes

### What the profile said

S2's first A/B on Avion's lake route (Vita3K 222 MHz, the sampler from 120 to 720 s, both states mixed) put the
collision family at 20.6% of the game thread, and its two largest items were not the segment tests or the finder but
the round-40 water natives: **waterProbe (manager_sub_01117EA0) 2.63% and waterSurface (manager_sub_01117340) 1.46%**,
4.1% together, while Wander swims. Then the segment test's host walk (coll115 walkT 1.48%), the object loop (obj125Run
1.14% + 0.21%), the ground fill (collisionDB 0.93%), fin125's clip and run (0.76% + 0.65% + 0.64%).

### Why, and the change (s2_water.h)

A water mesh is a flat list of triangles in the collision DB (+0x38 count, +0x3C table of 0x20 bytes: +4 the height,
+8..+0x1C three xz vertices), and both natives run every triangle through the full barycentric test (three VU0 outer
products, four dots, two PS2 divisions) on every call. Their outcome is decided by few triangles:

- **waterProbe** (is the segment's end over a water triangle with a point under it): a triangle can send the call to
  the recompiled code only when the end's xz is in it (u, v >= -2e-5, u + v <= 1 + 2e-5). Otherwise it only sets f0,
  and f0 after the loop is the last triangle's. So the native's own body runs only for the triangles whose xz box (with
  a margin: 1e-3 of the box + 1e-3 + 2e-6 of the coordinates) holds the end, for every triangle the box cannot vouch for
  (a coordinate not finite or over 1e6, or a thin one: twice its area under 1e-3 of |e1|1 |e2|1), and for the last one.
- **waterSurface** (the water surface a segment crosses), with no triangle filter: a triangle without p1y < h < p0y
  (its height between the ends, the code's own compares on the same bits) only writes its words into the work block and
  sets f0, both overwritten by the next triangle. So the body (its writes included) runs for the triangles with the
  height between the ends and for the last one, which leaves the work block and f0 as the loop does. The crossing
  block is written only by triangles with the height between the ends, so it ends the same. With a triangle filter (a
  guest call per triangle) the loop is as before.

No host copy of the mesh is needed: the box and height tests read the guest's table as the loop does (one line a
triangle, no arithmetic beyond a few compares), so there is nothing to validate or go stale.

## 4. Equivalence: how it was checked

As in S1, every check runs in-run, from the same state, both ways (two runs of one build do not repeat frame for frame).

- **Finder clips** (`SOTC_S2COLL_CHECK=1`): every clip a view can answer is first run the original way (fin125 off, so
  the guest's own ClipCollisionDB loop, entry by entry), its record kept and put back; then the view answers. Compared
  **bit for bit**: hit / no hit, the hit's id words, u, v, the kind, the point (and an independent surface's local
  point). The original's answer and record are kept.
- **Keep tests**: every keep test the view answers is compared with fin125's own answer (keepEarly125) for the same
  entry.
- **Water probes**: every call runs the full loop and the pre-tested one from the same state (for the crossing probe the
  work and crossing blocks are saved, the original run, its blocks kept, the blocks put back, the S2 run); compared: the
  outcome (to the recompiled code or not), f0's bits and the 0x40-byte work block and 0x10-byte crossing block.

No tolerance is used anywhere: every comparison is exact, and every one had 0 differences.

| run (build) | where | finder clips by a view compared | keep tests compared | water probes compared | differ |
|---|---|---:|---:|---:|---:|
| chkA (p1) | attract demo, 31 038 frames (the long gallop, the canyon) | 272 262 | 775 426 | - | **0** |
| chkL (p1) | Avion's lake: console card, teleport to the lake, swim, back to the shrine (in_tbz2.txt), 12 858 frames | 204 680 | 1 245 714 | - | **0** |
| chkL3 (p3) | the lake route, 15 078 frames | 232 329 | 1 528 680 | under-end 150 035, crossing 86 373 | **0** |
| **fin_lake (p6, shipped)** | the lake route, 14 898 frames | 223 762 | 1 505 500 | under-end 151 236, crossing 86 678 | **0** |
| **fin_att (p6, shipped)** | attract demo, 33 738 frames | 529 884 | 639 902 | under-end 7 339, crossing 9 080 | **0** |

Gameplay state: the views and the water pre-tests give the original's answers, records and guest writes, so nothing the
game reads differs. The runs' positions stay in the usual ranges (Wander's y 0..11.3 on the lake route, Agro's -98..7.6
in the attract demo, where the low values are the gallop through the canyon: the shots show Agro on the ground).

## 5. Speed (Vita3K 222 MHz, interleaved A/B)

`SOTC_AB_FLAG=s2coll SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs2.sh): S2 on or off at random each frame, so both
states see the same content. Three measures, each per state:

- **host time per call** (`SOTC_S2COLL_LOG=1`): every finder clip ([s2time]) and water probe ([s2wtime]) timed by the
  switch's state, cumulative over the run; a Welch z of the per-call means. The timer runs in both states.
- **the sampler split by state** (tools/s2groups.py): the game thread's samples in code only one state runs, per 1 000
  frames of that state. NEW = s2coll's functions and fin125's own copies called with S2 on (`coll125::run<.., 1>`,
  `keepEarly125T<1>`, new in S2 for this purpose); OLD = the same copies called with S2 off (`<.., 0>`). It measures the
  finder views; the water probes' code is one symbol in both states, so the host timing measures them.
- **frame times** (tools/mixframes.py, `SOTC_FRAME_LOG_ALL=1`): frames paired in blocks of 300, +- 2 SE.

Builds: p4 (views + water box pre-tests, first form), p5 (+ knobs and [s2wtime]), p6 (shipped: std::fmax / fmin
replaced by plain compares, see below).

| run (build, seed) | where | finder clips: us a call off -> on (z); ms a frame | views by the sampler (NEW / OLD, z, saved) | water under-end: us (z); ms a frame | water crossing: us (z); ms a frame | busy off - on |
|---|---|---|---|---|---|---|
| mixL4a (p4, 7) | lake route | 80.1 -> 66.4 (15.1); 1.78 -> 1.48 | 54.2%, z 104, 1.17% | | | -0.05 +- 0.14 |
| mixL4b (p4, 11) | lake route | 99.4 -> 80.6 (16.6); 2.18 -> 1.77 | 52.4%, z 116, 1.47% | | | +0.06 +- 0.18 |
| mixA5a (p5, 7) | attract demo | 47.4 -> 44.2 (6.8); 1.19 -> 1.10 | 79.0%, z 48, 0.33% | 14.2 -> 17.3 (-2.6) | 38.4 -> 36.9 (1.2) | +0.12 +- 0.11 |
| mixA5b (p5, 11) | attract demo | 31.6 -> 30.7 (2.6); 0.79 -> 0.76 | 79.2%, z 41, 0.23% | 16.9 -> 21.2 (-3.6) | 33.9 -> 35.9 (-1.2) | -0.00 +- 0.10 |
| mixL5a (p5, 7) | lake route | 96.6 -> 79.9 (15.5); 2.12 -> 1.76 | 55.2%, z 104, 1.30% | 140 -> 173 (**-14.8**) | 206 -> 201 (1.5) | -0.11 +- 0.14 |
| **mixL6a (p6, 7)** | lake route | 76.0 -> 55.1 (**20.4**); 1.64 -> 1.19 | 60.0%, z **76**, 0.92% | 150 -> 60 (**56**); 1.74 -> 0.70 | 213 -> 112 (**37**); 1.49 -> 0.77 | -0.07 +- 0.18 |
| **mixL6c (p6, 11)** | lake route | 75.2 -> 55.1 (**18.1**); 1.59 -> 1.17 | 61.4%, z **65**, 0.79% | 145 -> 55 (**54**); 1.67 -> 0.65 | 221 -> 119 (**32**); 1.54 -> 0.84 | +0.08 +- 0.20 |

**The first water form was slower** (p5, mixL5a: the under-end probe 140 -> 173 us a call). Its box tests used std::fmax
/ std::fmin, which on the A9's VFPv3 (no vmaxnm) are library calls: ten a triangle cost more than the body they skipped.
p6 uses plain compares (every operand is finite where they are used): the under-end probe then costs 40% of the
original's, the crossing probe 53%.

**Frame times do not move** in any run (every busy difference within 2 SE, -0.11..+0.12 ms), as in S1. The game's frame
is paced by VBlanks (43-46 ms frames at 222 MHz, between two and three VBlanks), and with the state flipping every
frame the CPU a frame saves is absorbed by the wait for the next VBlank; per-frame noise is ~5 ms. What the runs do
resolve is the game thread's work: on Avion's lake route **about 2.2 ms a frame less** (mixL6a: finder clips 0.45,
under-end probes 1.04, crossing probes 0.72; mixL6c: 0.43, 1.02, 0.70; host timing, the timer's own cost in both
states), about 5% of the game thread; in the attract demo (no water) 0.03-0.09 ms a frame of finder clips, 0.23-0.33% of the game thread by the
sampler.

**Collision before / after** (the lake route; the collision family's share of the game thread with the two states
mixed, plus / minus half of what S2 saves): mixL6a 19.9% mixed of a ~43.2 ms game thread: with S2 off about **22.4%
(9.7 ms a frame)**, with S2 on about **17.3% (7.5 ms)**; mixL6c 19.3% of ~42.3 ms: off about **21.8% (9.2 ms)**, on
about **16.7% (7.1 ms)**. The attract demo: 15.5% / 13.3% of the game thread mixed in the two runs, S2's part 0.2-0.3% of it.
On the console the views should gain more than Vita3K shows: they remove dependent loads (fin125's hash probes and epoch
checks) whose latency Vita3K does not model (round 128's finding); the water pre-tests remove arithmetic, which Vita3K
does show.

**Decision (the rule: z > 2 in both seeds).** The finder views: faster in all six runs by both measures (sampler z 41-116,
per-clip z 2.6-20.4): on. The water pre-tests (p6): faster in both lake seeds (under-end z 56 and 54, crossing z 37 and 32): on. So `SOTC_S2COLL` is on by
default; `SOTC_S2COLL_VIEWS=0` / `SOTC_S2COLL_WATER=0` leave one part out.

## 6. Another core?

Task 4 asked whether collision queries could run in batches on another core. Not in this round, and not cleanly:

- Every query S2 touches is issued one at a time by guest code (the root dynamics, the swim, the camera) that uses the
  answer at once: the next probe's segment depends on the last one's hit (the slide, the landing, the IK feet), and the
  water probes' answers decide the next call. There is no batch of independent queries to hand over without rewriting
  the callers (the root dynamics' mode functions, still the game's code: lifted, not redesigned).
- What the queries read is guest state that the game thread changes between them (positions, the per-frame caches the
  flushes clear), so a query run ahead on another core would need a snapshot of everything it might touch.
- The cores are not free: S1 and rounds 91 / 104 found cores 1-2 70-85% busy on the console (the render thread, the hook
  worker, audio, the helpers), and round 91's particle job and round 104's character jobs cost the game thread time there.
  The views and the water pre-tests are cheap enough (tens of microseconds a frame) that a hand-over and a join would
  cost more than they save.

The part of collision that could move is the per-frame preparation that does not depend on a query: the views' builds
(about two a frame) and, later, a host copy of the static meshes (water, the trees' leaf blocks of coll115) built off the
game thread when an area loads. That needs the host-side meshes first (section 7).

## 7. What S2 leaves for the next round

From the lake and attract profiles after S2 (both states mixed): the segment tests' host tree walk (coll115 walkT,
1.4-2.2% of the game thread), their object / part loop (obj125Run, 1.2-1.6%), the ground fill (GetCollisionDBCallBack,
0.8-1.1%), the finder leaf (0.3-1.3%), the attribute lookups (0.4-0.8%), the work flushes (flushWorkBody + flushModified,
0.6-0.7%), the wind vector (hot48c, 0.4%) and fin125's remaining clip arithmetic (0.75-0.85%). The order suggested:
1. The segment tests: a per-frame host broadphase of each isys group's objects (bound spheres in SoA, the objects that
   moved since the last frame found by comparing their matrices) so a query visits only the objects near its segment;
   the walk itself is host data already (coll115).
2. The ground fill: most of its objects are static (ground85 keeps their patches); a host index of static surfaces by
   cell would make the fill a lookup.
3. The flushes: only objects whose matrices changed need their caches cleared; with the change detection of (1) the
   flush of a still object could be skipped (its cached planes would be made the same again). That changes guest state
   (the cache flags and lists stay set), so it needs the whole-RAM check (S1's) to show that only the collision code
   reads them.

## 8. Pictures and the console card

Vita3K 222 MHz, muted, window shots every 60-120 s, looked at, then deleted. Every run: "ok" (drew within two minutes),
No exact recompiled function 0, one to four game-thread frames over 300 ms (boot, loads).

| run (build) | frames | Wander y | Agro y | what the shots show |
|---|---:|---|---|---|
| chkL / chkL3 / fin_lake (p1 / p3 / p6, check mode) | 12 858 / 15 078 / 14 898 | 0.00 .. 11.31 | 0.41 .. 12.52 | Wander swimming in Avion's lake with the ripples and splashes, then at the altar in the light shafts |
| chkA / fin_att (p1 / p6, check mode) | 31 038 / 33 738 | (not on the field) | -98.3 .. 7.6 / -6.8 .. 23.7 | Agro galloping on the grass and through the canyon with his shadow (the demo's route differs run to run; the y ranges follow the terrain, no jumps) |
| mixA5a / mixA5b / mixA6 (A/B, S2 flipping every frame) | 42 000-47 000 | | | the gallop, Agro on the ground with his shadow |
| fin_conwalk (p6): the console card's walk under the console env's rotation (`SOTC_AB_FLAG=s2coll,liftd9,s1ptcl SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=600`) | 17 538 | -5.30 .. 11.31 | -5.80 .. 12.47 | Wander running on the plain with his shadow; the lowest y is a dip of the plain |

**Console A/B:** build/agent-scratch/celosia-s2.vpk with build/agent-scratch/s2/env-console.txt
(`SOTC_AB_FLAG=s2coll,liftd9,s1ptcl SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=600`: each switch off in its own 600-frame windows,
rotating). spalign on celosia-s2.elf: 809 NEON :128 accesses, 0 sp-based. The Mac and Vita builds compile.
