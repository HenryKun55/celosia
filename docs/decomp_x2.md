# Round X2: the script bodies ranked by weight; the heaviest as native C++ (SOTC_X2=1, off by default) (2026-11-26)

Why: the console's biggest single self on the game thread is `scripts:execScript` (3.17 M cycles a frame = 7.1 ms at
444 MHz in the W1+W2+W3 run, `build/agent-scratch/console-logs/run-20261126-wxon/report.txt`; 3.62 M with them off). W3 showed
it is the woken script bodies, not the walk. X2 measures which bodies, then rewrites the heaviest.

Base: main 8e39115. Scratch, packages, runs: `build/agent-scratch/x2/` (M1's Vita build dir copied and re-pointed, as W3 did;
own Vita3K tree `../vita3k-celosia-x2r`, a copy of the D2 `x2` template tree, deleted at the end: the template itself is the
tree other rounds copy from, so it was not used directly).

## 1. The ranking

### 1.1 New measuring tools

- **Per object type** (W3's `SOTC_P2BRACKET=4`): one bracket per type around the woken body's run-to-return loop.
- **Per body function** (`SOTC_X2_RANK=1`, decomp_w3.cpp, level >= 2): the run-to-return loop split by the guest function
  each step enters (`scripts:fn_<first pc>`, keyed by the table's function so a body's labels share one; 63 at most).
- **Per sampled function inside each body, on the console** (`SOTC_X2_ZONE=1` with `SOTC_PROF129=1`): prof129's samples
  carry a zone: `0x4000 | fn << 6 | type` while a body's step runs, `0x6100` in the wake, `0x6200` in the rest of the walk,
  and `0x5000 | slot` inside any P2 bracket (sotc_p2.cpp's wrappers set it). So the samples in a body zone are exactly that
  body's self as the brackets define it, by function. `[x2z] fn <index> <pc>` lines map the indices.
  `tools/x2_zone.py err.txt --elf <elf>` prints the split (by type, by body with its top functions, by function).
  One relaxed store per bracket and per step; only when the switch is set.

### 1.2 Vita3K: per object type (ride7, 89 MB, level-4 clock brackets, us a frame, windows 6+; shape, not console weight)

| type | ride7 self (calls/f) | lake6 self (calls/f) | what |
|---|---|---|---|
| 0x09 | 2 106 (12.2) + frameEndCharaGeometry 210 | 1 873 (4.0) | characters: animals, Agro, Wander, the sword / sheath scripts |
| 0x0b | 601 (6.0) | 377 (8.0) | layout objects (collision aliases, the box) |
| 0x0d | 111 (12.5) + ExecObjectAnimation 888 | 27 (3.0) + 570 | animated objects (st_playObjAnim) |
| 0x06 | 320 (2.0) | 131 (2.0) | system tasks (sound / small objects) |
| 0x21 | 203 (3.0) | **2 597 (4.0)** | the lake's type-0x21 objects |
| 0x16 / 0x0e | 98 / 74 | - / 285 | |

### 1.3 Console weight per body (the ride)

The console run (`run-20261126-wxon`, level-1 brackets, prof129) cannot split execScript's self by itself: a native keeps
the guest pc of the last call, and the return addresses do not chain past a native. So the split is Vita3K's zone run at the
console's bracket level (`fn1RC`: SOTC_P2BRACKET=1, SOTC_X2_ZONE=1, prof129), applied to the console's own samples: for every
function, the share of its Vita3K samples that fall in each body zone, times the console's samples of that function
(scratch script `consest.py`, kept in the docs' numbers only). ms a frame on the console:

| # | body (entered function) | console est. ms | where its time goes (console est.) |
|---|---|---|---|
| 1 | small animals' script (gamecore_sub_013E3840) | **0.76** | its frame end's character geometry wrapper 0.12, aiParam 0.05, animalStep 0.03, pointsort 0.02, animalMove 0.02, steer 0.02, yRot 0.015, ... (a tail of ~40 AI functions) |
| 2 | st_collisionalias | **0.71** | the strand solver (gamecore_sub_013B8DC8) 0.25, cloth tension 0.09, UpdateLayoutClassSimpleGeometry 0.07 |
| 3 | st_playObjAnim | **0.57** | the frame end's object animation: optionNodeSample 0.10, simple geometry 0.08, kick 0.03 |
| 4 | nico_sheathScript | 0.31 | CalcCollisionMultiDepSurfBoundary 0.12, ClipCollisionDirectCallBackOneObj 0.06 |
| 5 | Agro's AI (gamecore_sub_01395E20) | 0.28 | pointsort 0.05, perception / script data system |
| 6 | nico_player1 | 0.18 | its own code 0.02, MVHIST 0.01, powerWork 0.01 |
| 7 | sound / small-object manager (gamecore_sub_013F6080) | 0.18 | smallManager 0.04 |
| 8 | playerlifebar_script | 0.11 | |
| 9 | nico_swordstart / nico_boxtest | 0.10 / 0.10 | |
| | all bodies | **~3.5 ms (~1.57 M cycles)** | of execScript's 7.1 ms self; the rest is the wake (~0.2 ms console: the switch, the resumed frame ends), the walk, and the non-body callees under execScript (graphFrame*, charaTail ...) |

**The finding**: the bodies are thin sequencers (st_collisionalias is 30 instructions, Agro's AI body 52, the animal's 204):
their own code is under 2% of their weight on every body measured. The weight is the libraries they call: the AI (aiParam,
pointsort, perception, the SCR data system), the strand / cloth solvers, the object animation samplers, the sheath's collision
test. "Rewriting the body" saves its glue (the guest calls, the getters, the register traffic); the next rounds must take the
libraries, by the table above.

## 2. What X2 makes native (game/overrides/decomp_x2.cpp)

`SOTC_X2=1` (read once; off by default; assumes SOTC_D1NATIVE=1 / SOTC_W3=1 as the console env files set them):

- **The small animals' script body** (gamecore_sub_013E3840, #1): decomp_scriptvm4's `animalScript` hands over at its entry and
  at its four resume labels (0x13e38b0 / 0x13e38f8 / 0x13e3908 / 0x13e3a7c). A C++ function on a typed view of the animal's
  work (`Animal`: kind, the layout object, the state / action / next words, the timers, the +0x7d0 / +0x890 blocks), a phase
  state machine for the coroutine (top, first object, gone, frame-step wait, frame done), plain C++ for the state reset, the
  collision decision, the position / rotation copies and the timers (d1::add: the PS2 adder for finite operands).
  isysGetThisSubThread, GetLayoutClass, GetCharacterClass as host reads of their plain cases (the relocated jal site must still
  name the getter; else the guest call). The AI steps, SCRGetFirstObj, SetLayoutObjAccessSwitch, SetCharacterObjCollisionProcFlag
  and the sleeps stay calls.
- **AI_OptFloatFrameSample** (0x13d38c0, the animal's per-frame motion sample), native, called directly from the animal body
  while the table holds its lift_d1 body: the getters as host reads, the two bytes and v0 as the code decides them.

**Coroutine semantics, kept**: the body sleeps in scrRun / scrRunM through `callMaySleep` (false: it slept, return at once) and
resumes at the same labels; its callee-saved registers (s0 the animal, s1 this sub-thread, s2 its character class, s3
0x1465650, s4 +0x7d0, s5 +0x890, s6 6, s7 0x1460000, fp +0x14, f20 1.0) and its 0x60-byte frame (the saves at the original's
offsets) are the original's at every call and sleep, so a sub-thread that slept in the old body resumes in the new one and the
other way round. Every guest store is the original's. **Not as the original**: the scratch registers (v0 / v1 / a* / at /
f0-f7 / the condition bit) at a sleep; the sleep stores them into the sub-thread's save area and nothing reads them back (each
is written before it is read after every resume label). The back edges' EE checkpoints are left out, as in every native loop.

**Not rewritten, and why**: bodies 2-9 are 10-50 instruction sequencers over library calls (section 1.3); their bodies'
rewrite would save a few guest calls each, the libraries are where their time is. The sheath script (2 985 lifted lines) is
the exception in size, but its weight is two collision functions.

## 3. Checks

**Build** (`build/agent-scratch/x2/`, M1's dir re-pointed; nice 20, -j4): 0 errors. spalign `0 sp-based :128/:256 NEON
accesses (of 3026 with an alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps" (W1's pad
unchanged). No alignas(16) by-value structs, no %zu.

**Exactness**: the body wakes and sleeps (a coroutine), so neither the difftest nor a run on a copy is possible for the whole
body. By construction (above) plus `SOTC_X2_CHECK=1`, which compares, on every animal frame, each native step with the old
statements on the same input: the state reset (the old branch order), the collision decision, the timers (the old statements'
FPU_ADD_S and stores on a copy of the work's bytes, compared byte for byte with what the native wrote), and every host getter
against the guest call (GetLayoutClass / GetCharacterClass, v0).

| run | switches | result |
|---|---|---|
| onRD | ride7, SOTC_X2=1 SOTC_X2_CHECK=1 SOTC_X2_LOG=600 | **662 856 checks, 0 differ**; up to 10.4 animal frames a game frame; getters 100% host, samples 100% native; mounted, rode to the end |
| onLD | lake6, the same | 4 073 checks, 0 differ (one animal before the lake); swam to the end |
| clkOnD / clkOffD | ride7, SOTC_P2BRACKET=4 SOTC_P2BRACKET_CLOCK=1, X2 on / off | see 3.1 |
| fn1OnE vs fn1RC | ride7, level-1 zones, X2 on / off | see 3.1 |

**Pictures** (looked at): onRD t0300 Wander galloping on Agro across the grass plain, the hazy ruins and the cloud bank ahead;
t0540 Agro walking in the shrine's pale stone corridor, the saddle and the reins drawn. onLD t0300 / t0540 Wander swimming
in the misty lake by the stone wall and the iron fence, the HUD's stamina circle, sword icon and the red bar. Nothing missing.

### 3.1 Speed (Vita3K, for the shape)

- The clock-bracket pair (clkOnD / clkOffD, level 4) is not comparable: clkOnD ran under twice the host load (brackets X2 does
  not touch were 2x slower too: ExecCharaGeometry self 2 545 vs 1 370 us a frame), so no number is taken from it.
- The zone pair at level 1 (fn1RC off / fn1OnE on, ride7, prof129 in Vita3K) normalised per animal frame by the AI library's
  aiParam (called once per animal frame, untouched): the animal body's samples / aiParam's are 9.03 off and 9.09 on (within
  the run-to-run noise; the animal count changes along the ride, 1-10 a frame); the glue the rewrite removes (the old body's
  own code, the getters, the motion sample's frame) 0.50 -> 0.40 of aiParam. So in Vita3K the change is below what the runs
  resolve; the body's own code was a small part of its weight (section 1.3), as expected.

## 4. Console steps

`build/agent-scratch/x2/` (README.txt): `celosia-x2.vpk` (ELF `celosia-x2.elf`), `env-x2-off.txt` / `env-x2-on.txt`
(`build/agent-scratch/wx/env-wx-on.txt`: W1 + W2 + W3 on, SOTC_P2BRACKET=1; on adds SOTC_X2=1 SOTC_X2_LOG=600) and
`env-x2-rank.txt` (the same base, SOTC_P2BRACKET=4 and SOTC_X2_ZONE=1: the per-type brackets plus the console's own split of
execScript's self by body and function; `python3 tools/x2_zone.py err-x2-rank.txt --elf celosia-x2.elf`). Three rides of the
usual route: off, on, rank; send the three err.txt.

**Expected gain** of SOTC_X2=1 on the console: small, ~0.02-0.05 ms (the animal body's glue: ~15 guest calls, 5 getters and
the register traffic per animal frame, 1-10 animals a frame on the ride). The rank ride is the main result for the next rounds:
it measures the table of section 1.3 on the console itself.
