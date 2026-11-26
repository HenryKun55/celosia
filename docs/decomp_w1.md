# Round W1: the display walk Vita-style, first cut (SOTC_W1=1, off by default) (2026-11-25/26)

Why: the display walk (DispDispObjGroupModel's tree) is the game thread's heaviest subsystem on the console. P3's brackets
(`build/agent-scratch/console-logs/run-20261125-p3/report.txt`) put it at 3.9 M cycles a frame (8.8 ms at 444 MHz):
DispDispObjGroupModel self 0.86 M, DispLayoutObjModel self 0.66 M (54 calls), modelDisp 0.96 M (54 calls, 0.30 M of it D-stall),
the light chain ~0.57 M inclusive, the sword blur 0.34 M, the particle emitters 0.27 M. The game thread has to lose ~7.7 ms.

Base: main 063f62c (P3). Scratch, packages, runs: `build/agent-scratch/w1/` (M1's Vita build dir copied and re-pointed, as P3
did; own Vita3K tree ../vita3k-celosia-w1, deleted at the end). Other agents (W2 characters, W3 scripts) worked at the same time
on other files.

## 1. Where the walk's time goes, before writing anything

- The console's numbers above, plus the Vita3K PC profile of main (I2's `pR0`, ride7, game thread) split by 64-byte buckets inside
  the two biggest natives:
  - `dispDispObjGroupModel` (1.93% of the game thread in Vita3K): about half its samples are in the group pass's per-object loop
    (between the walk's prefetch and AddSortObj's host version).
  - `modelDispHook` (1.75%): 15% of its samples are one inner loop, readChainPaletteLean's per-qword palette copy (a masked
    16-byte memcpy, a bounds test and a PLD per qword, after a zero fill of the whole range).
- The draw loop calls iosDlLockObj once per drawn object. On the console that is ~150 calls a frame through the table, and each
  one is three nested guest calls: lockObjSw -> lockObjD3 -> isysAddGroupWithLinkParam (addGroupSw -> addGroupD1 -> acquire,
  GetThreadId). While the walk holds the isys lock (it does: isysObjWaitSema at its start), all that ends in a depth that goes up
  and back down and a few link writes.
- CalcLightLayoutObj (20 calls a frame on the console) makes 2 + 4 x (attachment steps) guest calls to three getters
  (GetLayoutClass, GetCharacterClass, GetGeometryPointerObject). Each getter is a few loads.
- The group's chain is walked from scratch three times a frame (InitLayoutObjMatrix, the group pass, the light pass's three
  walks over the sort list), one dependent miss after the other, with the A9's own prefetcher off. Round C2 already built a host
  scene list and PLDs from it, but left it off by default and never had a console A/B.

## 2. What changed (`SOTC_W1=1`; `game/overrides/w1_disp.h`)

One switch for the console's A/B, read at first use (getenv; the Vita's env.txt comes after static init). With it off the game
is main's. Every part is exact: the same guest values read, the same writes, the same draws.

1. **The group as a host scene list** (`decomp_walk.cpp`: `c2scene::on()` and `c2::pfLevel()`; `decomp_group.cpp`,
   `decomp_light.cpp` call `c2scene::on()`). Round C2's scene comes on with W1, at prefetch level 2 unless `SOTC_C2SCENE_PF` says
   otherwise. Per group entry, the group pass keeps the entry, the object, its layout class, the sort node and the geometry in a
   dense host array, in the walk's order. The next frame's group pass, InitLayoutObjMatrix and the light pass's first walk then
   ask (PLD) for the lines of the next 1-4 objects from that array: independent addresses, all in flight together, instead of
   chasing the chain. Like C2, W1 makes lightPass and InitLayoutObjMatrix take their old (non-D1) bodies, which is where C2's
   prefetches are.
   **Found on the way:** round 140's clip helper never runs with `SOTC_D1NATIVE=1`. `clip140::post` declines when the table's
   ClipLayoutObjModel slot is not `clipLayoutObjModel`, and with d1native on the slot holds the D1 dispatcher. So on the console
   env C2's other half (the helper list from the scene) does nothing; only its prefetches act (chkR: "helper list from the scene
   0.00 / chain 0.00").
2. **iosDlLockObj as host code in the draw loop** (`decomp_isys::w1LockHost`, called from the walk's draw loop and its geodisp
   branches). When the table holds the natives, the walk owns the lock (owner = this thread's id from s11rt, depth >= 1) and the
   add's plan is native (addPlanCore), the host code does everything the three calls did, in their order:
   - the object's delete lock (+8);
   - the frame saves (s0, s1, ra);
   - the depth up;
   - the link into the display list's current group (addDoCore, word 0);
   - the depth back down;
   - the group's count up;
   - the registers the calls leave: a0 to a3, v0 the count, ra, sp, pc.
   Otherwise it makes the guest call as before.
3. **CalcLightLayoutObj's getters as host reads** (D1 body, `decomp_hot43b.cpp`; the console env runs d1native).
   - GetLayoutClass: its three plain table targets as host code, with its v0 / v1; index >= 14 gives 0.
   - GetCharacterClass: type 9 or 0x16 gives the object + 0x90; a0 is left as the type byte and a1 as the object + 0x90, as the
     MIPS code leaves them.
   - GetGeometryPointerObject: the jump table at 0x1195700, its two targets (`*a0` or 0).
   - Any other case makes the guest call.
4. **The bone palette in one pass** (`sotc_native::readChainPaletteW1`, behind readChainPaletteLean). One scan of the chain's
   UNPACKs (up to 32 kept). Then one memcpy per UNPACK when its block does not wrap at the 32 MB end (the old per-qword masked
   copy when it does). There is no zero fill when the UNPACKs cover lo..hi; later UNPACKs still overwrite earlier ones, as before.
5. **Direct brackets** (`sotc_p2::DirectScope`, `sotc_p2.h` / `.cpp`), active in both halves with `SOTC_P2BRACKET >= 1`:
   `display:w1GroupPass`, `display:w1CopyList`, `display:w1LightPass`, `display:w1DrawLoop`, `display:w1Palette` (only models
   that have a palette chain) and `display:w1CalcLight`. They read the same K2 counters and print the same `[p2b]` lines as the
   table's brackets (`tools/p2_report.py` reads them unchanged). With BRACKET=1 they add 5 + ~6 + ~20 bracket calls a frame.
6. **Checks, counts, parts.** `SOTC_W1_PARTS` (debugging, default 15: all) turns parts off one at a time: bit 0 the scene, 1
   the lock, 2 the getters, 3 the palette.
   - `SOTC_W1_CHECK=1` checks every lock, every getter and every palette against the old code:
     - lock: the guest call runs first and the words it wrote are kept; those words are put back, the host code runs, and the
       words and registers are compared;
     - getters: the guest call follows the host code and v0 / v1 are compared;
     - palette: the old copy is compared.
   - A `[w1]` line every 600 walks (`SOTC_W1_LOG=0`: none).
7. **Link room.** The W1 build's text ended 0x690 bytes below a 64 KB boundary ("Cannot allocate 4828 bytes for SCE data ...;
   segment 1 overlaps"). `sotc_vita_elf_room` stays at 122 880; a separate 4 KB `sotc_w1_elf_pad` (decomp_walk.cpp) moves the
   text end past the boundary. Drop it or resize it when a merge moves the text end again. The two W1 functions are in
   `vita_hot_order.ld` (next to lockObjSw).

What it does **not** do yet (the next W rounds): the group pass's per-object loop, DispLayoutObjModel, modelDisp's job setup and
the light colour manager (SetGeoDispClassLightColMan, still lifted code) are unchanged.

## 3. Checks

Builds p1 and p2 (main 063f62c + W1; p2 packaged): 0 errors; spalign `0 sp-based :128/:256 NEON accesses (of 3008 with an alignment hint)` (both);
`sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps".

**Exactness: every W1 decision compared with the old code, live** (Vita3K 222 MHz, 3 cores, muted, 89 MB
`SOTC_VITA_EAT_USER_MB=89`, `SOTC_D1NATIVE=1 SOTC_W1=1 SOTC_W1_CHECK=1 SOTC_C2SCENE_CHECK=1`). This is not a frozen-picture or
difftest mode: the game runs normally and each W1 path is checked in place against the old one in the same frame (section 2,
item 6). Compared: the guest words and the registers it leaves; the picture is the guest's whenever they differ. The runs:

| run | route | locks checked / differ | getters checked / differ | palettes checked / differ | C2 scene |
|---|---|---|---|---|---|
| chkR | ride7 600 s, mounted at 10 336 | 1 992 292 / 0 | 1 393 768 / 0 | 128 600 / 0 | 272.7 objects a walk, group pass synced 271.2, light synced every node |
| chkL | lake6 400 s (teleport to the lake at 2 081) | 815 575 / 0 | 713 398 / 0 | 68 717 / 0 | 150.7 objects, synced 150.0 |
| chkR2 (p2, the packaged build) | ride7 420 s, mounted at 2 221 | 1 347 493 / 0 | 1 427 326 / 0 | 114 448 / 0 | (C2 check off) |

chkR and chkL ran on build p1. p2 is p1 plus the `SOTC_W1_PARTS` mask; it is the packaged build, and chkR2 checked it again
(t0300: galloping up a sandy slope towards a rocky outcrop, Agro's shadow).

Per walk (ride7): lock 104-186 host, 0 guest; light getters 93-131 host, 8.5-10 guest (the GetLayoutClass cases that call out);
palette ~9-11 UNPACK blocks, ~18-21 KB copied in bulk.

**Draws and pictures.** Every write the W1 paths make is checked equal to the old code's, so the draw stream cannot change. The
pictures were looked at (t0300 / t0540; t0300 / t0360 on lake6), all normal:
- chkR: Wander on foot crossing the plain towards the stone arches (t0300), shadow under him; galloping on Agro along the tall
  mossy stone wall (t0540).
- chkL: Wander swimming in the lake, ripples around him, HUD (stamina and sword) normal.
- bootOffR: galloping past the ruined wall with trees on the plain, cloudy sky, Agro's shadow (both shots).
- bootOnR: on Agro in the shrine's stone courtyard, lit carved walls (t0300); on the stairs beside the dark wall (t0540).
- bootOffL: Wander swimming beside the lake's stone tower, his reflection under him, HUD normal (t0300).
- bootOnL (twice): stuck on the memory card's "Loading data" screen, see the next paragraph.

**The lake6 load flake is not W1.** bootOnL and its repeat bootOnL2 never left the memory card's "Loading data" screen (the input
script's CROSS repeats kept going). To find out why, the build got `SOTC_W1_PARTS` (bit 0 scene, 1 lock, 2 getters, 3 palette;
debugging, default all) and short lake6 runs (170 s, p2):

| W1 | runs | result |
|---|---|---|
| on, all parts | onE, onF | onE: stuck on the "Loading data" screen; onF: same script failure (104 CROSS presses), screen not checked |
| on, one part at a time | bis1, bis2, bis4, bis8 | scene: stuck on "Loading data"; lock: stuck on "Loading data"; getters: loaded; palette: past the load, at its radial blur at t0120 |
| on, `SOTC_W1_PARTS=0` (none of W1's code runs) | pz0, pz1 | pz0: at the post-load radial blur at t0150; pz1: loaded |
| **off** | ctlA, ctlB, ctlC, ctlD | ctlA, ctlB: loaded; **ctlC: stuck on the same "Loading data" screen**; ctlD: at the radial blur at t0150 |

With W1 off and with W1 on but none of its code running, the runs fail the same ways, and chkL (W1 + all checks) loaded the
lake. So the frame-timed input script on the save load is fragile in this tree and on this day (D3's runs met the same
radial-blur hang). It is not a W1 change. chkR / bootOnR loaded the same save on ride7 normally.

(The route drifts between runs: the mount frame was 4 012 off and 4 398 on, so the off and on pictures are of different places.)

**Vita3K clock brackets** (bootOffR / bootOnR, `SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1`, us a frame at 222 MHz, all windows).
These only show that the direct brackets work and nest. They are **not a speed result**: Vita3K models no caches, which is what
W1 is for, and the two runs rode through different places (one window's light pass alone varies 350-3 100 us with the place).

| bracket | off | on |
|---|---:|---:|
| DispDispObjGroupModel | 11 638 | 12 280 |
| w1GroupPass | 1 824 | 2 085 (the PLD bookkeeping, as C2 measured in Vita3K) |
| w1CopyList | 243 | 301 |
| w1LightPass (incl.) | 1 168 | 1 935 |
| w1CalcLight (21.8 / 22.4 calls) | 534 | 1 049 |
| w1DrawLoop (incl.) | 8 169 | 7 744 |
| modelDisp (60 / 66 calls) | 3 519 | 2 960 |
| w1Palette (6 calls) | 131 | 87 |

The console A/B (P2BRACKET=1 counters, section 4) decides.

## 4. Console steps

`build/agent-scratch/w1/` (README.txt): `celosia-w1.vpk` (ELF `celosia-w1.elf`), `env-w1-off.txt` and `env-w1-on.txt`.
Both are P3's env-p3-bracket.txt with `SOTC_P2BRACKET=1` (P2's 22 brackets + the 6 W1 direct ones, lower overhead than level 2)
and without `SOTC_P2PMUSAMPLE`; the on file adds `SOTC_W1=1`. Two rides of 4-5 min each (off, then on), same route; send both
err.txt. Report: `python3 tools/p2_report.py err-w1-off.txt --elf build/agent-scratch/w1/celosia-w1.elf`, and the same for the on
run. Compare DispDispObjGroupModel / modelDisp inclusive, the w1* brackets, and `[m1pmu]`'s busy cycles a frame.
