# Round R1: the characters' per-frame data walks for the A9's caches (r1char, off by default) (2026-11-25)

First round of the "rewrite in order of console weight" plan: the characters (8.4-9.1 ms of the ~44 ms console game thread,
docs/console_profile_2026-11-20.md section 3). The console's own counters (docs/vita_perf_guide.md section 0: ~106 k L1 D
refills a frame, ~46 stall cycles each, **no hardware prefetcher**, 32-byte lines) say a walk over guest records costs one
serial miss per line it touches first. R1 measures the character walks, then lays the hottest out for that machine.

Switch: `SOTC_R1CHAR` (A/B name `r1char`, `kR1Char`), **off by default**. It acts inside d1native's D bodies, so it needs
`SOTC_D1NATIVE=1` (with d1native off the old bodies run and r1char changes nothing). Census `SOTC_R1CHAR_CENSUS=1`
(+ `SOTC_R1CHAR_LOG=<frames>`: `[r1char]` lines). Code: game/overrides/r1_chara.h (notes, prefetch helpers, census),
dc2_chara.cpp (cloth step / flip), decomp_chara.cpp (link list), s3_chara.cpp + decomp_skeleton.cpp (pose),
decomp_hot46c.cpp (control definitions). Scratch: build/agent-scratch/r1 (chain.sh, runs/, tools/poscmp.py).

## 1. What the character walks touch (census, Vita3K ride7, riding, per frame)

`SOTC_R1CHAR_CENSUS=1` replays each walk's guest reads / writes record by record before the walk runs and stamps every 32-byte
guest line with the frame it was first touched in (run `census`, 600 s, mounted 7 314, `SOTC_AB_FLAG=r1char` so the windows
alternate; the cloth / link numbers do not depend on the state). "first" = lines no walk had touched earlier in the frame
(the cold ones: the misses a cache that keeps nothing across frames would take).

| walk | calls | records | bytes | line touches | first in frame | stride / shape |
|---|---:|---:|---:|---:|---:|---|
| pose (GetMotionMatrix: locals 16 B read+write, matrix 64 B, quaternion 16 B per bone; the rig compare 2 lines a bone) | 10-23 | 1 050-2 300 | 30-64 KB | 1 460-3 250 | **540-1 480** | per bone 3 arrays indexed by bone number (visit order jumps), 96 B of outputs a bone |
| cloth step (row records 24 B, 3 point buffers of cols x 16 B per row) | 23-31 | 290-350 | 15.8-17.4 KB | 630-720 | **460-525** | contiguous per row buffer, rows in separate allocations |
| cloth pins (row params 36 B, 2 lists of 32 B entries, bone matrices 64 B) | 23-31 | 280-315 | 11.7-13.3 KB | 545-620 | **228-246** | list entries in their own blocks, matrices by bone index |
| cloth flip (row records) | 22 | 88 | 1.6 KB | 109 | **94** | contiguous |
| charaLinks (28 B entries + each handle's table word + table header) | 3.1-6.8 | 2 600-9 700 entries | 18-66 KB | 2 750-9 900 | **291-333** | one ~10 KB list re-scanned 2x a call, 3-7 calls a frame: warm after the first scan |
| **all five** | | | | | **~1 600-2 700** | |

Reading: the character walks R1 can reach put ~2 k cold lines a frame through L1 (2% of the console's ~106 k refills). The
link list is touched ~8 k-10 k lines a frame but only ~330 of them cold: its 0.25-0.3 ms on the console is instruction work over
2 600-9 700 entries a frame (two dependent handle resolutions an entry), not misses. The pose is the largest cold set (outputs
written to guest buffers indexed by bone, 96 B a bone), then the cloths (~800 cold lines: buffers, pin entries, row records).
Round 141's helper thread (pf141, on) already asks for these lines one character segment ahead into the shared L2; what is
left is the L1 miss to L2 (~20 cycles) or to DRAM when pf141 was late, in series, because nothing else prefetches.

## 2. What R1 does (exact: the same values, the same order of every guest read / write and float op)

- **Pose** (`s3chara::poseR1`, called from GetMotionMatrix's D3 core when r1char is on): the dense host mirror is S3's rig
  (bones in visit order, parent visit indices, the bone offsets in one contiguous host array, 16 B a visit). Coherence: the
  rig is compared with the guest's bone records (links + offset, 2 lines a bone) on the frame's **first** pose of that skeleton
  only and trusted for the frame's later poses (round C1's rule; skeletons are resource data, never written while loaded; C1 /
  P1's check runs found 0 stale rigs in 1.4 M trusted). The bone offsets come from the host array, not from the 64-byte guest
  bone records. The outputs' lines (matrix 2, quaternion 1, local 1) are asked for (PLD) two visits ahead and the first two
  visits' up front. Who reads what: the guest buffers (matrices, quaternions, normalised locals) stay the only output, written
  by the same stores in the same order; every reader (skin palette, display, IK, gaze, cloth pins, lifted code) reads them as
  before. The rig is host-only. Keep mode (SOTC_S3CHARA_KEEP) still goes to S3's pose; the D3 core's gates (S3 off, its check /
  log / jobs) are unchanged.
- **Cloth step** (`clothStepD1Core<true>`): row records asked for up front and two rows ahead; the first row's three buffers
  up front; while a row's points run, the next row's velocity / positions-read / positions-written lines one of each every
  second point (one row of lead, ~cols x 20 cycles); the pin pass's row parameters up front and the next row's two pin-entry
  lists ahead. Nothing new is written; the extra guest words read (the next row's buffer pointers) are reads the loop makes
  one iteration later anyway.
- **Cloth flip** (getClothDynamics' D1 adapter, `clothFlipAhead`): the grid and all row records asked for before the flip, the
  first two rows' buffers after it, while the step's guest call is being dispatched.
- **Link list** (`charaLinksD3Core<true>`): entries asked for four ahead (28 B stride: every line once); each handle column keeps
  its own table cache (the two columns name different tables, so the shared one-slot cache reloaded both tables' header words
  on every entry). The scan only reads memory; the cached words are the same words.
- **Control definitions** (CheckControlDef D3 core): the next 0x30-byte condition's two lines asked for before the current
  condition runs.

Not done as mirrors, and why: the cloth buffers and link entries are already dense guest arrays (the missing piece on the A9 is
the prefetch, not the layout); a host copy of them would have to be compared with the guest's words on every use (they are
written by guest code between frames), which reads the same lines. The per-character class (0x1210 bytes, read by dozens of
lifted functions) stays the guest's until those functions are native.

## 3. Checks

- **Build** (p1 = celosia-r1): 0 errors, no "segment 1 overlaps" (ELF room 122 880 unchanged), spalign **0 sp-based of 2 888**
  hinted NEON accesses. No by-value aligned struct added (M4 locals only, as poseRange), no `%zu`, no GPU pool read, the census
  and prefetches are game-thread only where they keep state (`!isShell`).
- **Difftest** (dtR: ride7 1 200 s, `SOTC_D1NATIVE=1 SOTC_R1CHAR=1 SOTC_DIFFTEST=clothStep,getClothDynamics,charaLinks,GetMotionMatrix
  SOTC_DIFFTEST_SELF=1 SOTC_DIFFTEST_SWITCH=r1char SOTC_DIFFTEST_VU0=1 DEPTH=1`: each checked call runs with r1char on, then off on
  the same state; all RAM / scratchpad, v0 / v1 / f0, callee-saved registers and the VU0 file compared): **clothStep 100 checks,
  getClothDynamics 100, GetMotionMatrix 100: 0 mismatches**; charaLinks: checked (first line at 1 check, 0 mismatches; it did not
  reach the 100-check line, most of its calls take the old body's guest-call path, identical in both legs). Loaded, mounted at
  frame 2 512 and rode to 15 987 under the difftest (D3's full-list hang is not hit with this list).
- **Deterministic ride7 off vs on** (detOff / detOn: `SOTC_FIXED_STEP=1 SOTC_POS_LOG=30 SOTC_D1NATIVE=1`, r1char 0 / 1): the runs
  are not frame-deterministic before any R1 code runs: the title camera already differs at frame 330 (a load moment, dt 0 vs 33),
  the field appears at frame 1 230 (off) vs 1 110 (on) and Agro is mounted at 2 671 vs 2 264: Vita3K's asynchronous loads land
  at host-time-dependent frames. So the positions cannot be compared frame by frame; the exactness evidence is the difftest
  (same state, both ways, byte for byte) and the construction (prefetches and same-bit mirrors only).
- **89 MB boots** (`SOTC_VITA_EAT_USER_MB=89` `taken`, ride7 600 s, muted):

| run | switches | mounted | end frame | pictures (looked at) |
|---|---|---:|---:|---|
| bootOff | default (r1char off, d1native off) | 6 448 | 15 947 | 300 s: galloping up the sand slope toward the misty cliffs, Agro's shadow, cloudy sky; 540 s: galloping between two tall stone towers, mane / tail cloth moving, grass ground |
| bootOn | SOTC_D1NATIVE=1 SOTC_R1CHAR=1 | 3 431 | 13 814 | 300 s: Agro standing by a carved stone wall, Wander on his back, plain behind; 540 s: galloping under a large mossy rock overhang, cliffs and sky normal |
| census / detOn / dtR | r1char on (census also alternating) | 7 314 / 2 264 / 2 512 | 15 261 / 16 210 / 15 987 | census 540 s: galloping across the plain, tail cloth and cape flowing; dtR 1 140 s: Agro on temple stairs (difftest speed) |

  No fault, no FREEZE / watchdog line in any run. The A/B rotation works with the switch (`[ab] r1char off in odd windows of 300
  frames`; the census lines alternate r1char 1 / 0).

## 4. Speed

Not measured in Vita3K on purpose: Vita3K has no cache model (PLD is a no-op there; the mirror's saved reads are L1 hits there),
so its A/B could only show R1's few added instructions. The console decides: **celosia-r1.vpk** (+ celosia-r1.elf) and
`env-r1-ab.txt` (one run, r1char alternating every 300 frames, aligned with `SOTC_M1PMU=300`'s counter windows: compare dcache
miss / dcache stall cyc / cycles of the on and off windows), `env-r1-off.txt` / `env-r1-on.txt` (whole runs), `env-r1p1-on.txt`
(+ round P1's key rows / gaze tables / skin prefetch, never run on the console). All with the K2 run's base env (SOTC_K2=1,
SOTC_M1PMU=300, frame log, prof129, body shadows, logs muted) and SOTC_D1NATIVE=1. README.txt in build/agent-scratch/r1.

Estimate: ~1.6-2.7 k cold lines a frame reached; if R1 hid all of them at ~46 cycles each that is ~0.2 ms; realistically
**0.05-0.2 ms, 1-2 k fewer dcache refills a frame** out of ~106 k. That is the honest size of the "dense layout + prefetch" lever
for the characters as they stand: the census shows the characters' cold data are small; their console cost is mostly the
instruction work of PS2-shaped code (charaLinks' 2 600-9 700 entry resolutions a frame, the lifted chain) and the I-side.

## 5. Next

- charaLinks: a per-list host index (the resolved pairs and the target's entry positions) would remove the 2 600-9 700 entry
  resolutions a frame; it needs a write hook on the list (isysAddGroupWithLinkParam / the delete path) to stay exact.
- The pose's 96 B of guest outputs a bone: once the skin palette / IK / gaze readers are native, a host pose buffer (SoA, one
  matrix array per character) can replace the guest writes for everything but the lifted readers.
- The cloths: the mesh builder (pose82::clothMesh, 01137F78) re-reads the positions; fusing step + constrain + velocity + mesh
  per row would read each row once.
