# Round Y1: the small animals' AI library as typed native C++ (SOTC_Y1=1, off by default) (2026-11-26)

Why: X2's ranking (docs/decomp_x2.md, section 1.3) put the small animals' script at 0.76 ms a frame on the console and
Agro's AI at 0.28 ms, nearly all of it in the ~40 AI functions they call; the body itself is a thin sequencer. The heaviest
of those functions were still round 43 / 44 natives written register for register: every float and VU0 register lives in
R5900Context, every guest read and write goes through READ32 / WRITE128, and every leaf helper (AI_UpdateVelocity,
AI_GetRotVelocity, AI_GetAreaDistScale, AI_GetInterRatio, the table sines, the getters, AI_GetSmallAnimalAIParam) is a guest
call through the function table, with the context traffic around it.

Base: main c34d573 (X1 + X2 + X3), rebased onto 04986d4 (Y2, Y3) for the package. Scratch, packages, runs: `build/agent-scratch/y1/`, using M1's
Vita build dir with the same byte-length path repoint and the runner at `x1/runner`, as X1-X3 did. Own Vita3K tree
`../vita3k-celosia-y1` (a copy of the D2 `x2` template).

## 1. The ranking

Vita3K, 222 MHz, ride7 600 s with Y1 off (`runs/rankRB`), clock brackets (`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1`) on
32 AI library entries through `SOTC_P2BRACKET_EXTRA` (`extra.txt`). Microseconds a frame, the 25 windows of 600 frames after
frame 3000 (mounted at 2223); ~6.75 small animals stepped a frame:

| function | calls a frame | inclusive us a frame | self us a frame | inclusive us a call |
|---|---|---|---|---|
| animalStep (013E1E48) | 6.75 | 637 | 163 | 94.4 |
| Agro's AI (01390A90) | 1.00 | 516 | 14 | 516 |
| updateScriptDataSystem | 1.00 | 501 | 189 | 501 |
| updateUserPathWork | 2.07 | 375 | 100 | 181 |
| animalMove (013E30D0) | 6.75 | 273 | 114 | 40.5 |
| 013E2A78 (calls steer) | 6.75 | 253 | 47 | 37.4 |
| AI_GetSmallAnimalAIParam | 33.75 | 211 | 211 | 6.25 |
| 013E1190 | 6.75 | 172 | 117 | 25.5 |
| steer (013DD228) | 4.41 | 167 | 97 | 37.9 |
| 013D89A8 | 6.75 | 138 | 41 | 20.4 |
| pointsort | 2.07 | 134 | 134 | 64.7 |
| 013D8B38 | 6.75 | 104 | 59 | 15.4 |
| yRot | 13.48 | 67 | 67 | 5.0 |
| AI_GetRotVelocity / AI_UpdateVelocity | 9.43 / 21.30 | 36 / 24 | | 3.8 / 1.1 |

The three small-animal functions with the most self and callee weight that are still register-for-register natives
are animalStep, animalMove and steer: together ~1.08 ms a frame in Vita3K. Their leaf calls (AI_GetSmallAnimalAIParam
x5, AI_UpdateVelocity, AI_GetRotVelocity, the getters) are a large part of that. Agro's AI and updateScriptDataSystem are
heavier, but they belong to Agro's script and are left for a later round (section 7).

## 2. What Y1 rewrites

`SOTC_Y1=1` (read once; off by default). The old natives hand over at their entry when Y1's entry test passes: the
records are inside main RAM, the stack window is inside main RAM, and the relocated jal sites of the inlined boot-ELF
helpers still name them. If the test fails, the old native runs and the call is counted as `fallback` (0 in every run).
`SOTC_Y1_PARTS=<mask>` (testing only): 1 = steer, 2 = animalMove, 4 = animalStep (default 7).

| function | what | before (guest calls a call) | Y1 (guest calls a call, ride7) | file |
|---|---|---|---|---|
| gamecore_sub_013DD228 (steer) | an animal's steering: two direction lerps, the heading's turn rate, the pitch, the velocity's parts, +0x30c | 14 (AreaDistScale x2, acos, SideSignal, RotVelocity x2, asin, UpdateVelocity x3, TableSin x2, TableCos) | **2.00** (acos, asin) | y1_steer.cpp |
| gamecore_sub_013E30D0 (animalMove) | the movement step: the motion speed, the turn, the velocity, the heading, the layout position moved by the turned offsets | 10 (thread x2, layout x2, character, SetSpeedRatio, UpdateVelocity, 013D8B38, yRot) | **2.95** (SetSpeedRatio, 013D8B38, yRot except in states 14 / 15 / 20) | y1_move.cpp |
| gamecore_sub_013E1E48 (animalStep) | the per-frame AI step: the classes, +0x260, the rotation frame, the state's targets, the five AI parameter records | 12-17 (thread x2-3, layout x2-4, character x1-2, AI_GetSmallAnimalAIParam x5, UpdateVelocity, InterRatio, 013E1190, the state's callees) | **2.59** (013E1190 and the state's own callees: 013DA6B8 / 013DA970 / 013DAEB0, CheckPathPoint, iosGetRandom, the collision probe, HitPerp) | decomp_hot43d.cpp (Y1 mode), decomp_aiparam.cpp |

How:

- **steer and animalMove are new functions** (y1_steer.cpp, y1_move.cpp). The arguments are read once, then floats, the
  frame's qwords (the lerp results, the frame copy of t0's qword, the turned offsets) and the VU0 registers are C++
  locals. Guest memory goes through `d1::Mem`. The leaf helpers are inline host functions in y1_leaves.h, each written
  from its recompiled code: the generator's `FPU_*` / `PS2_V*` operations in its order, the same clamps, and the
  condition bit and divide flags in a local fcr31. The helpers are AI_GetAreaDistScale, AI_GetInterRatio,
  AI_GetRotVelocity, AI_UpdateVelocity, AI_GetSideSignalFromDirLev, iosGetTableSin / Cos, and the 16-bit angle
  conversion. The getters are host reads while their jal sites name them, X2's plain cases otherwise the guest call.
  Guest calls are kept for callees with side effects or with natives that the table may switch (quat122's acos, the
  lifted asinf, the motion speed, the heading, yRot). Around each one, fcr31 and the VU0 locals are stored before the
  call and loaded after it.
- **animalStep keeps round 43's body** (its 25 paths and the frame structs that the collision probe reads) **in a Y1
  mode.** The getters, AI_UpdateVelocity and AI_GetInterRatio are done on the host inside its call helper. The five
  AI_GetSmallAnimalAIParam calls run on round D2's typed core (`aiParamCore`) in one batch: the registers are loaded
  from the context once and stored once (decomp_aiparam.cpp `aiParamBatchY1`), where before there were five table calls
  that each loaded and stored the whole set.
- **The context is written only where a caller or a guest callee can read it**: a call's arguments, fcr31, and at the
  exit f0, v0, fcr31, sp / ra and the VU0 registers the original leaves. The saved registers are never changed in the
  context, so the epilogue's reloads are not needed.
- **Not as the original, all dead after the return:**
  - the function's own stack frame and the frames of the inlined leaves below it (steer and animalMove do not write their
    frames: the qwords stay in registers);
  - for kept guest calls, the callee-saved registers the callees save into their own frames (the caller's values instead
    of the original's working values);
  - the scratch registers no caller reads (at, v1, a0-a3, t*, f1-f19);
  - the fcr31 condition bit, which no caller reads before a compare sets it.

  The table's own typed leaves (A1 / A3: AI_UpdateVelocity, AI_GetInterRatio) already leave the condition bit and f1 as
  they find them.

Direct brackets for the console (`SOTC_P2BRACKET>=1`): `ai:y1Steer`, `ai:y1Move`, `ai:y1Step`, around Y1's bodies. In
off runs, the table brackets `extra:ai:*` (`SOTC_P2BRACKET_EXTRA`, below) time the old natives at the same entries.

## 3. The check (SOTC_Y1_CHECK=1)

Same thread, no helper thread (X1's threaded check froze Vita3K). For a checked call, the check:

1. copies guest RAM (32 MB, into one buffer allocated at the first check), the scratchpad and the context;
2. runs Y1's function, keeps its context, and lists the RAM words it changed with its values;
3. puts back **only those words**, the scratchpad and the context;
4. runs the **recompiled function** (the generated code, the ground truth, whose callees resolve through the table as in
   the game) to its return;
5. compares every word either leg changed and the scratchpad, and the registers a caller can read: s0-s7, gp, sp, fp, ra,
   pc, v0, f0, f20-f31, fcr31 without the condition bit, VU0 vf1-vf31 / ACC / Q / status. A RAM mismatch is reported
   with the game frame, the call's a0, the offset in the animal's record and whether both legs or only the recompiled
   one wrote the word (up to 8 a check).

The dead stack window (8 KB below the entry sp) is compared too, and its differing bytes are counted apart as
`deadStack`, not as a mismatch. The game goes on from the recompiled leg.

Two guards keep the check from comparing the kernel's work instead of the function's:

- **Only while the EE clock is quiet.** A call is checked only when nothing is posted to the scheduler and its gate is not
  reached yet (more than `SOTC_Y1_CHECK_QUIET` cycles away, default 0; with 32 768, half a time slice, no ride7 call was
  checked: the gate is nearly always closer; these count as `clock-skipped`). The generated code's checkpoints hand the
  scheduler its due work: the VBlank handler, the threads it wakes, the IOP's completions. Y1's code has checkpoints
  only in its guest calls. A leg that reaches the scheduler does that kernel work, and the two legs then do it twice:
  the queues, the semaphores and the IOP's requests are not in the RAM copy.
- **Void.** A check in which the VBlank counts (`*0x1e1314`, `*0x1e133c`, which the EE's VBlank handler bumps) changed in
  either leg is counted `void`, not compared.

Switches: `SOTC_Y1_CHECK_FROM=<game frame>` / `SOTC_Y1_CHECK_UNTIL=<game frame>` (when the checks start / stop), `SOTC_Y1_CHECK_EVERY=<n>` (one call in n per
function, default 4), `SOTC_Y1_CHECK_QUIET=<cycles>`, `SOTC_Y1_CHECK_CONTROL=1` (the recompiled function in both legs: the
check's own noise), `SOTC_Y1_LOG=<frames>` (the `[y1]` counts: calls a frame, fallbacks, guest calls a call, checks, bad,
void). One check at a time: a Y1 function reached from a checked function's legs runs unchecked.

**Results** (Vita3K, 222 MHz, 89 MB, d1native + W1-W3 + X1-X3 on, the final code: p6 for the ride, p7 for the lake; the package p8 is the same Y1 code on main 04986d4):

| run | checked frames | steer | animalMove | animalStep | void | clock-skipped |
|---|---|---|---|---|---|---|
| ride7 900 s, `chkSG` (from 2300, one call in 150; **mounted at frame 4261**, game frame 7518 at the end) | 2300-7518 | 192, 0 bad | 256, 0 bad | 257, 0 bad | 1 (animalMove, frame 7037) | 0 |
| lake6 900 s, `chkLI` (frames 1075-1995, one call in 24; the lake reached, game frame 15018 at the end) | 1075-1995 | 144, 0 bad | 149, 0 bad | 151, 0 bad | 2 (steer, animalMove, frames 1687-1689) | 0 |

Zero mismatches in every compared check. The lake's Y1 calls are all before the boss teleport (frame 2000): the
small animals by the shrine. After the teleport, at the lake, no Y1 function runs (`chkLG`: no call after frame 1675).

**What the earlier mismatch and crash were** (the handed-over draft's p1 check, which put all 32 MB back):

- lake6 `animalStep MISMATCH (check 16): RAM at 001e0bd4: recompiled 1 y1 2`. Not Y1's: 0x1e0bd4 is the CPU rap counter's
  mode (`iosResetCpuRapCounter`, sotc_overrides.cpp). That p1 check reported only the lowest differing address. With
  every differing word listed (p2), the same kind of check showed 50 000-69 000 differing words: the VBlank counts
  (0x1e1314 / 0x1e133c), the sub-thread state (0x1e11a0), the small heap (0x1e0bf0). The recompiled leg's checkpoints had
  run the VBlank handler and the threads it wakes, during the boss teleport's load (frames 1905-2050, seconds a frame).
  The VBlank guard voids such checks now.
- ride7 crash at frame 2541 (`EXC_BAD_ACCESS`, a corrupted heap in `_malloc_r`, the hook join and the camera blur on the
  stack), during the mount's loads. The p1 check put back all 32 MB of RAM. That also undid whatever other host threads
  had written into guest RAM during the two 32 MB copies: the CD / IOP transfers of a load and the hook workers. Now only
  the words Y1's leg changed are put back. p3 then ran the ride's checks past the mount (2486) to frame 2813, every 40
  calls, without a crash; p6's run above went to 7518.
- Every-4 lake6 runs from frame 0 (p1, p3) went black after the teleport: they checked through its load (one check
  every few frames, seconds each). The lake runs now check only frames 1075-1995.
- One more p2 report: `steer (check 115): 01c8eca4 recompiled ba25c2ed y1 ba25c2f7`, 1 word, at 0.3 fps inside the
  teleport's load. It came before the void guard and the per-word notes (a0 offset, which leg wrote), and it has not
  come back in 192 + 144 steer checks since.


## 4. Build

p8 (the package, rebased onto main 04986d4 with Y2 and Y3): `vita exit 0`, 0 errors. spalign: 0 sp-based of 3 324 alignment
hints. `sotc_vita_elf_room` 0x1e000 (122 880). No "segment 1 overlaps". p6 (the ride's check run, the same code without
`SOTC_Y1_CHECK_UNTIL`, on bc473f3) and p7 (the lake's, on bc473f3): the same three results.

Pictures, looked at: ride7 `chkSG` t0300 shows Agro on the plain with the shrine's ruins behind and Wander climbing on;
t0540 shows Wander on Agro by the shrine's wall. lake6 `chkLI` t0300 shows Wander and Agro at the shrine's altar
with Mono (frame ~1900, checking); `chkLG` / `chkLI` t0540 show Wander swimming in the lake by the towers, with the HUD. Ground, Agro, Wander and the water look normal; nothing is black or broken.

## 5. Speed

Vita3K, 222 MHz, ride7 600 s, clock brackets, Y1 off (`runs/rankRB`, the old natives under `extra:ai:*`) against Y1 on
(`runs/clkOnB`: the same `extra:ai:*` entries, now Y1's bodies, plus `ai:y1*` inside them). The windows after frame 3000:

| function | off: us a call (inclusive / self) | on: us a call (inclusive / self) | change |
|---|---|---|---|
| animalStep | 94.4 / 24.2 | 78.2 / 3.1 (y1Step 74.5 / 39.8) | -17% |
| animalMove | 40.5 / 16.8 | 39.0 / 3.4 (y1Move 35.0 / 14.5) | -4% |
| steer | 37.9 / 22.1 | 22.2 / 3.2 (y1Steer 18.2 / 18.2) | -41% |
| AI_GetSmallAnimalAIParam (table calls) | 33.75 a frame, 211 us | 0 a frame (batched in y1Step) | |
| AI_UpdateVelocity / AI_GetRotVelocity (table calls) | 21.3 / 9.4 a frame | 2.9 / 2.4 a frame | |

At off's call rates (6.75 steps and moves, 4.41 steers a frame), the three functions take **~1.08 ms a frame with Y1 off and
~0.89 ms with Y1 on: ~0.19 ms a frame less (-18%)** in Vita3K. Most of animalStep's remaining time is in its callees
(013E1190, the state's callees), which are not rewritten. animalMove gains little: three of its guest calls stay
(SetSpeedRatio, 013D8B38, yRot), and they hold most of its time.

The whole frame is not comparable between the two runs. clkOnB mounted Agro at frame 7980 and rankRB at 2223 (the ride
script and Vita3K's timing vary), so the windows cover different places. The check runs with Y1 on mounted at 2486
(chkSE), 2800 (chkSF) and 4261 (chkSG), so the late mount is not Y1's doing. Vita3K has no caches and runs NEON through its JIT, so
the A9 should gain more from the removed context traffic; the console A/B (section 6) is the measure.

## 6. Console steps

`build/agent-scratch/y1/` (README.txt): `celosia-y1.vpk` (ELF `celosia-y1.elf`), `env-y1-off.txt` (=
`build/agent-scratch/wx/env-wx-on.txt` + SOTC_X1=1 SOTC_X2=1 SOTC_X3=1) and `env-y1-on.txt` (off + SOTC_Y1=1). Two rides of
the usual route, off and on; send both err.txt files. `python3 tools/p2_report.py err-y1-on.txt --elf
build/agent-scratch/y1/celosia-y1.elf` gives `ai:y1Steer` / `ai:y1Move` / `ai:y1Step` (inclusive, self) and the
`[m1pmu]` frame cycles.

## 7. Next

- Agro's AI (01390A90 -> updateScriptDataSystem, updateUserPathWork, pointsort: ~0.5 ms a frame in Vita3K) is the next
  library by weight, the same way: typed functions, inline leaves, batched getters.
- animalStep's callees: 013E1190 (25 us a call), 013D89A8 / 013D8B38 and yRot (animalMove's three kept guest calls),
  013E2A78 (steer's caller).
- Once those callees are native, animalStep can drop round 43's register-for-register body for a typed one like steer's.
