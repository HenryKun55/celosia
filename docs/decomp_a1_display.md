# Round A1 (display census; not the typed lifter's A1 of 2026-11-21): the display walk's write / read census, phase 0 of "display as an off-thread consumer" (2026-11-27)

Why: docs/arch_plan.md item 1 moves the display walk's processing (draw loop, modelDisp's job setup, the light pass) to
core 1, overlapping the next frame's logic. Its one real risk: gameplay reading something the walk writes, in the same
frame or the next. Phase 0 settles that with a recorder, before any of the 12-18 days of phase 1-2 are spent.

Base: main d30914a, rebased onto 0d923a6 (Z4). Code: `game/overrides/a1_record.cpp` (the recorder, `SOTC_A1_RECORD=1`, off
by default, the Mac only: an empty `install` on the Vita), a source-range hook in the libc stubs (`LibC.cpp`,
`g_a1RangeReadHook`, Mac only), `tools/a1_census.py` (sums the `[a1]` blocks over a walk range). Scratch, Mac build, runs:
`build/agent-scratch/a1/`.

## 1. The recorder (SOTC_A1_RECORD=1)

DispDispObjGroupModel (0x10f52c8) is wrapped in the function table. For every `SOTC_A1_EVERY`-th walk (default 15; the
runs below used 3) from walk `SOTC_A1_FROM` (default 300):

1. **The walk's real start and end.** The walk is suspended at thread switches: its host function returns early and the
   scheduler resumes it later at a label, not through the table. So the recording starts at graphFsEffectReset called from
   0x10f5318 (right after the walk's isysObjWaitSema, which can block) and ends at isysObjSignalSema's return to
   0x10f57b4 (the walk's last call). On the Mac every recorded walk was suspended at least once.
2. **W_disp** (what the walk writes): guest RAM (32 MB) copied at the start, compared word by word at the end, with a byte
   mask per changed word; the scratchpad too. Natives included: it is a memory diff. The 64 KB under the walk's entry sp
   (its dead frames) are counted apart and not watched.
3. **Other guest threads inside the walk.** While the walk is suspended, other EE threads and handlers run on the same host
   thread (the VBlank's work, ~1 600 loads in 1 segment a walk on the ride). The recorder sees the switch at the first load
   whose `currentThreadId()` is not the walk's, copies RAM there, and at the walk's next load (or its end) diffs that
   segment: those words (~270 a walk) are kept out of W_disp.
4. **R_disp** (what the walk reads): during the walk the read watch (`g_ps2ReadWatchMap`, all lines on; compiled into the
   Mac's generated code, `PS2X_GUEST_READ_WATCH`) marks every 16-byte line the walk's own guest loads touch, plus memcpy /
   memmove source ranges (`g_a1RangeReadHook`). Natives' plain loads are not seen, so the runs use `SOTC_DECOMP=0` and no
   `SOTC_NATIVE`: the whole walk and the whole game logic are recompiled code, every guest load is watched.
5. **Readers of W_disp** until the next walk: the lines holding W_disp bytes are watched. A load that covers a byte the walk
   changed and that still holds the walk's value is a dependency, logged with the reader (host pc: the generated function's
   symbol, `name_0xADDR+offset`), the time since the walk, and the frame: "same" (before the next execScript entry; one
   execScript per walk was counted in every block) or "next" (in the next frame's logic). A byte that no longer holds the
   walk's value was rewritten and stops being watched (a rewrite with the same value still counts: an upper bound).
6. **W_logic and R_disp x W_logic**: at the next walk's start RAM is compared with the copy taken at the walk's end. The
   R_disp lines that hold a W_logic word are the walk's inputs that the next frame rewrites: what an extraction must copy.

Output: `[a1]` lines every `SOTC_A1_REPORT` (20) recorded walks: the W_disp runs, object offsets and 64 KB blocks, the
readers, R and R x W_logic by 64 KB and object offsets. "Object" = the display group's objects at the walk's start
(isysGroupForFirst's chain), offset < 0x1000 above the nearest one.

## 2. Runs (Mac, Release + ThinLTO with an LTO cache, muted, 222-free: the Mac runs as fast as it can)

`SOTC_DECOMP=0 SOTC_A1_RECORD=1 SOTC_A1_EVERY=3`, the console memory card, the round's usual inputs:

| run | route | recorded walks used | scene (shots looked at) |
|---|---|---|---|
| R6 | ride7 (teleport to the plain, Agro steered) | 10 940 from walk 3 800 | on foot towards Agro on the plain (frame 4 201), riding along the tall wall (9 301) and the plain (12 301) |
| S5 | the shrine: the save's load with no teleport | 5 580 from walk 1 300 | the altar with Mono and the doves, Wander standing in front (frames 3 300 - 18 600: a static camera) |
| L6 | lake6 | 5 740 from walk 3 500 | Wander swimming in the lake beside the iron fence and the stone tower, ripples, HUD normal (frames 3 600 and 12 000) |

A console-env run (`SOTC_NATIVE=1`, D1 natives, W1-Y3 on) was tried on the Mac too (N6): it stayed on the memory card's
"Checking memory card" screen (frame 21 000) and was stopped. Its W_disp before that was 3-4 K words a walk on the attract
demo against 15-18 K for the recompiled walk (the native renderer builds no guest packets). Natives are exact rewrites of
the same guest functions, so the guest-level census below holds for them; the native-only consumers (hook jobs, render
thread) are display-side by construction.

## 3. The census

Per walk, all runs: (R6 ride / S5 altar / L6 lake)

| | ride | altar | lake |
|---|---:|---:|---:|
| W_disp words (not the dead stack, not other threads) | 22 978 | 14 855 | 17 664 |
| of which still the walk's value at the next walk | 22 468 | 14 389 | 17 283 |
| dead stack words | 183 | 113 | 144 |
| R_disp lines (16 B) | 20 604 | 20 265 | 15 836 |
| W_logic words (everything else between two walks) | 67 141 | 58 536 | 39 524 |
| **R_disp x W_logic words (extraction inputs)** | 4 399 | 4 330 | 3 853 |

### 3.1 What the walk writes, and who reads it

| structure / range | bytes a walk (ride / altar) | readers outside the walk | when | verdict |
|---|---|---|---|---|
| display-list packet buffers 0x1230000-0x12d0000 (two halves, alternate walks: iosDl chains, the recompiled modelDisp's DMA packets) | ~50 K / ~23 K | iosDlChainTailCurrent, iosGetPacketPtrPath1, gsRegEnd (the frame-end 2D / effect passes appending to the same chains), texTransResolve; the DMA / GS (host) | same frame | display-private. With the native renderer (the console) modelDisp writes none of this |
| isys link entries 0x22e000-0x24a000 (24-byte links at 0x21b900 + i x 24: iosDlLockObj's links into the display list's group) | ~10 K / ~8 K | isysDeleteObjManager (kernel_sub_001C6408), isysObjDeleteUnLock, iosDlSwap's isysGroupForNextForce | same frame (frame end) | **game thread**: the links come from the isys free list that gameplay's isysAddGroupWithLinkParam allocates from next frame (it reads the free-list words the walk changed: 0x26d9xx, object +0xb00 fields) |
| object delete-locks: the u16 at object +0xA (bits 1-15 the lock count, bit 0 the delete flag); object +0x8 / +0xC | 127 + 127 words / 118 + 118 | isysObjDeleteUnLock (same frame); **isysGroupForNext / isysGroupForFirst in gameplay** (collision, lights, AI group walks) read the halfword for bit 0 | same + **next** (ride: 41 071 of 85 233 reads over 10 940 walks, ~3.8 a walk; altar: 12 391 of 49 911) | **game thread** (iosDlLockObj, already W1's host code). The bit-0 reads do not depend on the count, but the count defers deletions (isysRequestDeleteObj) until the display list is done: it must be raised in order before the frame's logic runs |
| group pass object writes: object +0x4f0..+0x4f8, +0x690..+0x698, +0x5b0..+0x5c8, +0x620..+0x628, +0x674, +0x680..+0x688, +0x750..+0x758 (the billboard / camera-linked quaternions and points into the layout class's +0x134 / +0x13c, UpdateLayoutObj simple, the clip key and +0x260's bit 8) | ~2.5 K words / ~0.9 K | **InitLayoutObjMatrix** (0x10f5820, called by 0x1346318 with graphFrameInit at the next frame's start): its matrices are the ones gameplay reads | **next** (925 reads over 10 940 walks, ride; none at the altar) | **game thread**: the group pass stays on core 0 (the plan already keeps w1GroupPass) |
| clip results, sort keys, the two sort lists (kSortList) | in the object rows above / the sort buckets | AddSortObj / GetNextSortObj only (the walk's own light pass and draw loop) | inside the walk | display-private: moves with the draw loop |
| skinning palettes / bone blocks written by dispSkinned (heap runs 0x1aa0000, 0x1ab0000, 0x1c00000 ... 1-2.4 KB each) | ~12-16 K / ~10 K | none in guest code (the DMA / native jobs) | — | display-private |
| texture transfer queue 0x1657e94.. (texTransRequest from modelDisp) and VRAM requests | ~150 B | texTransResolve / manager_sub_010AB618 / texResetVRAMRequest at the frame end (0x1345F60) | same frame | **game thread or move both**: the requests stay in order on the game thread (with the native renderer they are texTransRequestHook's), or texTransResolve moves with them |
| iosDl / gsReg state 0x213200.., 0x21b8e0.. | ~100 B | iosDlChainTailCurrent, iosGetPacketPtrPath1, gsRegEnd, iosDlLockObj (later display passes) | same frame | display-side; stays with whatever still builds packets on the game thread |
| isys lock state 0x1e1300.. (owner, depth) | ~40 B | every isys call after the walk | same frame | game thread (isysObjWaitSema / SignalSema at the walk's ends) |
| timer extension 0x1fd8a0 (iosGetTCountExtension's last count) | 8 B | iosGetTCountExtension, kernel_sub_001AD850 | next (ride 427, altar 3 969 over the run) | harmless: any caller extends the same hardware count; the worker calls no guest code |
| display globals 0x118c000.., 0x1186294.., 0x118ab10.. | ~350 B | shadowDraw, bossDeathEffectEnd, reflectionBegin (frame-end passes) | same frame | display-side |

The lake (L6) has the same reader set (plus manager_sub_0109C610, a same-frame display pass): next-frame reads only from
isysGroupForNext / First (+0xA halfword: 16 751 + 88 over 5 740 walks), the timer extension (582) and nothing else; no
InitLayoutObjMatrix read there (no billboard / camera-linked object in that scene).

Nothing else outside the walk read a byte the walk wrote and had not rewritten. In particular **no gameplay function read
the light state, the palettes, the sort lists, the clip results or any modelDisp output**, in either frame.

### 3.2 What the walk reads that the next frame's logic writes (the extraction inputs)

R_disp is ~20-21 K lines a walk (~330 KB, most of it model / texture / packet data nobody writes). The lines the next
frame rewrites, R_disp x W_logic, are ~17.6 KB a walk on the ride and ~17 KB at the altar:

- object +0x8 (the isys header words: not an input of the drawing, skip),
- object +0x520, +0x2cc, +0x5e0, +0x38c, +0x4dc, +0x730 (per-object state the scripts set every frame: visibility / LOD /
  colour words),
- object +0x250..+0x288 (a 64-byte block: the world matrix the logic writes),
- +0xb4..+0xbc (characters),
- the characters' bone blocks (runs of 1-2 KB in 0x1a70000 / 0x1aa0000 / 0x1c00000: ExecCharaGeometry's matrices, the +0x160
  skinning input),
- 0x11fb760.. (camera / light globals: ~220 B).

That is the packet the extraction must copy; the plan's estimate (~56 KB with palettes, 273 objects x 128 B + 21 KB) is the
right order and an upper bound: the measured logic-written input is ~17 KB a walk.

## 4. Verdict

**Item 1 is safe.** No gameplay code reads the light pass's, the draw loop's or modelDisp's output. Three groups of the
walk's writes must stay game-thread writes, in the original order, before the next frame's logic:

1. **The group pass** (w1GroupPass, ~0.58 M cycles light): its billboard / camera-linked transforms feed the next frame's
   InitLayoutObjMatrix, whose matrices gameplay uses. The plan already keeps it on core 0.
2. **iosDlLockObj** (W1's w1LockHost, 104-186 a walk): the display-list links come from isys's shared free list (gameplay
   allocates from it next frame) and the delete-lock counts defer deletions until the list is done.
3. **The texture-transfer requests** (texTransRequest from modelDisp, a handful a walk) feed the frame-end texTransResolve on
   the game thread: keep the requests (or their guest-visible queue writes) in order on core 0, or move texTransResolve
   with them.

Plus the walk's isysObjWaitSema / SignalSema around all of it.

## 5. Phase-1 design

**Game thread, at the walk's place in the frame (part i):**

- isysObjWaitSema; graphFsEffectReset; the sort lists cleared; the group pass exactly as today (callbacks, clip,
  AddSortObj, billboards, +0x260 bit 8); isysGroupForExit; the copy list (w1CopyList, it only builds the second sort list).
- The draw loop's guest-visible parts, in the loop's order, without drawing: for each drawn object w1LockHost's lock
  (links, delete lock), and modelDisp's texTransRequest calls (from the plan's per-(header, LOD) records: which textures a
  model asks for is known at extraction).
- The extraction: the packet below, written into one of two host buffers.
- isysObjSignalSema; kStripsOff = 0.

**The packet (host memory, no guest pointers kept for the worker; one per frame, double-buffered):**

| part | layout | size (ride / altar) |
|---|---|---|
| header | frame number, camera context (0x200 B: view, projection, the +0x140 position), global light / fog / environment (ExecGlobalLightMan's inputs, ~0.4 KB), screen / LOD scales | ~1 KB |
| per drawn object, in sort order (both lists) | instance id, model header address (a key, not dereferenced), LOD, flags (+0x260, layer, opacity), world matrix (64 B), the light inputs CalcLightLayoutObj reads (class colour, ambient, the object's light-group words), the per-object words of section 3.2 | ~160 B x 120-270 = 20-43 KB |
| per skinned character | the bone block (ExecCharaGeometry's matrices, the +0x160 palette input), its length | 1-2.4 KB x 3-25 = 3-30 KB |
| static data the worker needs | model / mesh / texture data: not copied; the native renderer's plans and mesh caches already hold host copies keyed by header (draw plans, round 8) | 0 |

Total ~25-75 KB a frame. At ~40 stall cycles a miss (arch_plan §2) that is 0.05-0.1 M cycles plus the walk logic.

**Core 1 (part ii), from the packet only (no guest memory):** the light pass (CalcLightLayoutObj, SetGeoDispClassLightColMan,
ExecGlobalLightMan's per-object part), the draw loop's drawing (DispLayoutObjModel, modelDisp's job setup: the frame change,
matrices, part decisions, palettes, lights, the hand-over to the hook worker / lane), dispSkinned's palette (into host
memory: nothing outside reads the guest palette). The render thread consumes the jobs as now.

**Expected:** the plan's arithmetic stands, with the three kept parts added on core 0:

| | light (32 ms) | heavy (46.6 ms) |
|---|---|---|
| leaves core 0 (plan §2) | 4.27 M | 2.70 M |
| stays: group pass | 0.58 M | 0.40 M |
| new on core 0: extraction (packet copy and walk) | 0.20-0.35 M | 0.25-0.40 M |
| new on core 0: locks in the loop's order + texture requests (W1's host lock ~0.3 us x 104-186; a handful of requests) | ~0.05-0.08 M | ~0.05-0.08 M |
| **net** | **-3.84..-4.02 M = -8.6..-9.1 ms** (conservative, DispLayoutObjModel's lifted self staying: **-7.1 ms**) | **-2.22..-2.40 M = -5.0..-5.4 ms** (conservative **-3.6 ms**; needs the core-3 enabler) |

**The check that the moved part stays exact** (`SOTC_A1_CHECK`, phase 2's first deliverable):

1. *Job stream:* in the check build each frame runs the old walk (as today, on the game thread) into a recorded job
   stream, puts back the guest words that only the moved part writes (the display-private set of section 3.1: packets,
   palettes, sort lists, light words; known per walk from the A1 diff), then runs the new game-thread part plus the worker
   on the packet, and compares the two hook job streams job by job (kind, draw state, matrices, palette, lights, the order:
   SOTC_HOOK_ASYNC_VERIFY's hash per job, extended to every field), and the GL hand-over's draw list.
2. *Game-visible words:* the new game-thread part's guest diff (A1's W_disp machinery) must equal the old walk's W_disp
   restricted to the kept set (links, delete locks, group pass rows, texture queue, isys state), word for word, and the
   display-private set must have no reader: `SOTC_A1_RECORD=1` on the new build re-runs this census and must list no reader
   of anything the worker produces.
3. Pictures on ride7 / lake6 / the altar, and the job-stream check on Vita3K at 222 MHz with the worker on core 1.

## 6. Build and checks

- Mac: `build/agent-scratch/a1/mac` (Release; the Mac target's ThinLTO link through `-Wl,-cache_path_lto`, so a relink after a
  small change takes ~1 min; the build at background QoS). The Mac target did not build on main: unity batches that differ
  from the Vita's put colliding files together, and five files included `a9sweep.h` only on the Vita while using A9S_HOIST
  everywhere. Fixed: `PS2X_RUNNER_UNITY_SKIP` (CMake cache list, empty by default; the Mac config here passes
  `decomp_math.cpp;decomp_hot44d.cpp;decomp_gaze.cpp;decomp_hot46h.cpp;a[0-9]+_typed_check.cpp;decomp_ikquat.cpp`), the
  include moved out of the Vita guard in decomp_ikquat / char122 / charageo / pose82 / pf141 / s3_chara (same include on
  the Vita), a1_record.cpp built as its own translation unit (the Vita's unity batches stay main's).
- Vita (builds p1 and p2, the second at this tree's head; the recorder is an empty install there): 0 errors; spalign `0 sp-based :128/:256 NEON accesses (of 3417 with an alignment hint)`; `sotc_vita_elf_room` 0x1e000; no "segment 1 overlaps". No Vita3K run: nothing changes on the Vita.

## 7. Next

Phase 1 (6-8 days): the extraction of the model path and the packet, behind a switch, with the job-stream check of section 5
from its first day. Re-run this census (`SOTC_A1_RECORD=1`, Mac) on the new build: the reader list must lose nothing but
gain nothing either.
