# Round W3: what the script pass's console "self" is, and the pass as a native walk (SOTC_W3=1, off by default) (2026-11-25)

Why: the game thread is at ~40 ms a frame on the console and has to reach 33 ms. P3's console brackets
(`build/agent-scratch/console-logs/run-20261125-p3/report.txt`, cycles a frame, 444 k = 1 ms) put 7.6 ms of self time in the
script / task system and 2.1 ms in the camera:

| bracket | calls/f | self cycles | (icache / dcache stall) |
|---|---|---|---|
| scriptPass (manager_sub_01102638) | 15.9 | 1.98 M | 0.40 M / 0.46 M |
| isysWakeupSubThread | 56.4 | 0.89 M | 0.19 M / 0.29 M |
| GetLayoutClass | 443 | 0.10 M | |
| ExecObjectAnimation | 5.9 | 0.34 M | |
| doit_cameraman_manager | 1 | 0.95 M | 0.29 M / 0.20 M |

Base: main 063f62c (P3). Scratch, packages, runs: `build/agent-scratch/w3/` (M1's Vita build dir copied and re-pointed, as
P2 / P3 did; own Vita3K tree ../vita3k-celosia-w3, deleted at the end).

## 1. What scriptPass's self is (read from the code, then counted)

**The mechanism.** A script object's sub-thread is a coroutine (decomp_subthread.cpp). The pass wakes it with
`callGuest(isysWakeupSubThread)`; the wake loads the sub-thread's registers and calls its resume address, the label after
its last sleep (the frame end gamecore_sub_013CF258's labels, scrRunM's 0x13cf9f8, the animal script's ...). Those resume
entries finish their own function and return to *its* caller's label, a frame that exists only on the guest stack: the
wake's inner `runtime->callGuest` sees pc off its return address and gives up (returns false), the wake returns, and **its
bracket closes there**. The pass's `sotc_decomp::callGuest` then sees pc != 0x11026b4 and runs its run-to-return loop
(`finishGuestCall`: table lookup at pc, call, repeat) until the body calls scrRun / scrRunM -> the frame end -> the sleep,
which switches back with pc on 0x11026b4. So:

- the **script bodies** (the AI, execSCRBaseFunc's data-driven functions, the animal / colossus scripts), **the frame
  end's per-type work** (letters, view clip, the character's geometry and post-process, ExecObjectAnimation) and every
  unbracketed native they call are in the pass's self, not in the wake's. That is why P3's tree shows
  frameEndCharaGeometry, ExecObjectAnimation, ClipCollisionDB and GetLayoutClass (443 calls) with `par=scriptPass`;
- the wake's own self (0.89 M, 56 calls) is the switch (two lean register frames, cold lines) plus the resumed frame end's
  tail up to its return, plus what runs before the first return to a host-less frame (the camera, the frame start's
  graph work: they show `par=isysWakeupSubThread`);
- the **walk** itself per object: GetScriptClass (a jump table), the class's +0x38 / +0x28 / +0x4c / +0x29, and
  isysGroupForNext; per pass isysGroupForFirst / ForExit (the group lock: a PollSema / SignalSema pair). ~130 objects
  a frame (P3: 56 wakes, ~72 letter discards for objects past their script).

**Counted** (Vita3K clock brackets, `SOTC_P2BRACKET=3 SOTC_P2BRACKET_CLOCK=1`, 89 MB ride7, windows 8+, us a frame at 222 MHz:
the shape and the call counts, not console weights):

| bracket | off (main's pass) | on (W3: the bodies in their own bracket) |
|---|---|---|
| scriptPass self | **4 038** | 580 |
| scripts:subThreadRun (the woken bodies' run-to-return loops, 36 a frame) self | (inside scriptPass) | **3 335** (incl 13 177) |
| isysGroupForNext (all callers) | 305 calls, 372 | 250 calls, 326 |
| isysGroupForFirst / ForExit (all callers) | 144 / 117 calls, 338 / 245 | 143 / 117 calls, 309 / 247 |
| GetScriptClass (the pass) | (not wrapped) | 65 calls, 98 |
| LetterObjDiscardAll | 69 calls, 152 | 60 calls, 134 |
| ExecObjectAnimation | 1 444 (par scriptPass) | 1 002 (par subThreadRun) |
| isysWakeupSubThread self | 957 (72 wakes) | 997 (63 wakes) |
| doit_cameraman_manager self / doit_cameraman self | - / 794 | 18 / 808 |
| ClipCollisionDirectCallBack (11 calls, par the camera's target visibility) | 1 372 | 1 239 |
| GetCollisionDBCallBack incl (finder 37 calls 570) | | 1 111 |

So **~83% of scriptPass's self is the script bodies** (3.3 of 4.0 ms here), not the walk; the walk's own code plus its
callees is ~0.6 ms + the group calls (shared with the AI's other group walks). On the console that is ~1.6 of P3's 1.98 M
cycles in the bodies, ~0.35 M in the walk (an estimate by this split; the console run of the package measures it). The
camera: doit_cameraman_manager's 0.95 M console self is doit_cameraman's own lifted code (~0.8 ms Vita3K, its unbracketed
helpers) plus the collision callbacks under the target-visibility steps (ClipCollisionDirectCallBack 11 calls,
GetCollisionDBCallBack 4 calls with 37 finder calls); every other CAM_ step is < 0.12 ms.

W3_LOG on the rides (on runs): 16 passes, 67-80 objects, 50-80 wakes and 24-53 run-to-return loops a frame (ride7 / lake6);
GetScriptClass inline for 37-56% of the objects (the others are the getter tail cases: character / layout / animation /
sprite / window classes, through the table), isysGroupForNext direct for 100%.

## 2. The W3 pass (game/overrides/decomp_w3.cpp)

`SOTC_W3=1` (read once; off by default): decomp_hot44d's scriptPass hands the pass to `w3::scriptPass` (game thread only).
The same calls in the same order, the same registers before each call and at the end, the same stores (the 0x30 frame,
the class's +0x28 / +0x38, ...): script order and timing are game logic and do not move.

- **The script bodies in a bracket of their own.** The wake is called as sotc_decomp::callGuest calls it; when it comes
  back with pc off the return address (the woken script's body has to run), the run-to-return loop
  (`sotc_decomp::finishGuestCall`, unchanged) runs inside `sotc_p2::Scope("scripts:subThreadRun")`
  (SOTC_P2BRACKET >= 2; =4: one bracket per object type, `scripts:run_tNN`). Without brackets the scope is a compare.
- **GetScriptClass inline** for its plain cases: the type byte (+8) - 6 < 28 picks the jump table entry at 0x1194cc0, read
  live; entries 0x11024d0 / 0x11024d8 / 0x11024e0 are "a0 + 0x20", "a0 + 0x1c", "none"; a type outside the table is "none".
  As the native leaves them: its frame word (ra at sp - 0x10), v0 the class, v1 the entry (0 outside the table), sp / ra
  back, pc on ra. The getter tail jumps (window / animation / sprite / layout / character class) and unknown entries go
  through the table as before; so does everything when the slot does not hold decomp_scriptvm3::getScriptClass (a checker,
  a profiler wrapper, SOTC_DECOMP_SKIP) or the script VM switch is off.
- **isysGroupForNext directly**: decomp_group::nextAt (the native the table holds, groupForNextSw) with the registers
  groupForNext leaves (v0 / v1 / a0-a3, a2 the link table). Another function in the slot or a link outside main RAM: the
  table.
- **The next object asked for** (PLD, `__builtin_prefetch`): at the start of each object, the next link's record, its
  object's first line and the two lines its script class's +0x24..+0x50 sit on (+0x20 / +0x1c classes). The current
  object's body runs meanwhile; on the console each object of the walk is a cold line (P3: 0.46 M D-stall cycles in the
  pass's self). Nothing is read for the game; a prefetch changes no state.
- Unchanged, through the table: isysGroupForFirst / ForExit (the group lock: semaphore calls), the wake itself, the
  sub-thread start / init, the letters' discard, the +0x4c callback, the delete request; obj96 / char104 / pf141 hooks
  where they were.

`SOTC_W3_CHECK=1`: each inline GetScriptClass also runs the native on a copy of the context (it only reads guest memory and
writes its own frame word, the same value) and compares v0 / v1 / sp / ra / pc (`[w3c]` lines, the first 20 differences
and every 200 000th check). `SOTC_W3_LOG=<n>`: `[w3]` every n frames (passes, objects, wakes, run-to-return loops a frame;
how many class / next calls took each path; checks and differences).

## 3. Brackets added for the console (game/overrides/sotc_p2.cpp, small)

- `sotc_p2::named(name, level)` / `open` / `close` / `Scope`: a bracket around a stretch of native code (registered when
  SOTC_P2BRACKET >= level; else -1 and nothing happens). Used by the W3 pass (above).
- `SOTC_P2BRACKET=3`: P3's 79 plus 22 more: the walk's callees (GetScriptClass, isysGroupForFirst / ForNext / ForExit,
  isysLetterObjDiscardLetterAll, the character frame-end tail 0x13cf4e0) and the camera's steps under
  doit_cameraman_manager (doit_cameraman, the region / LWS animation managers, obstacle / view-cut avoidance, slope,
  subjective / objective target visibility, CAM_doit_target, follow rotation, post update, final update, default /
  view / aim angles, target offset). The lifted camera calls these as known natives (PS2_CALL_GUEST_KNOWN); wrapped, the
  slot no longer holds the known function and the call goes through the table, so they can be bracketed. With W3 on,
  wrapping GetScriptClass / isysGroupForNext sends the walk's calls back through the table (by design: the slot test).
- Level 1 (the console A/B env files) is unchanged: the W3 brackets are not registered there, so the A/B has the same 22
  brackets on both sides.

## 4. Checks

**Build** (`build/agent-scratch/w3/`, M1's dir re-pointed; one build at a time, nice 20, -j4): 0 errors.
spalign `0 sp-based :128/:256 NEON accesses (of 3008 with an alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880; no
"segment 1 overlaps". The first build did overlap (segment 0 ended 3 980 bytes below the 64 KB boundary; vita-elf-create needs
4 828): decomp_w3.cpp was 12.7 KB of text, with one inline copy of sotc_decomp::callGuest per call site and the check path
duplicated; one out-of-line callGuest for the rare calls, one wake site and the check in a cold helper brought it to 8.5 KB
(the pass itself 5.9 KB) and the gap to 8 080 bytes. No alignas(16) by-value structs, no %zu.

**Sameness** (Vita3K 222 MHz, 3 cores, muted, 89 MB `SOTC_VITA_EAT_USER_MB=89`, 600 s, SOTC_D1NATIVE=1 as on the console). **No
frozen-picture and no difftest mode was used**: the pass wakes sub-threads (coroutine switches), which the difftest cannot run
twice. The proof is by construction plus a state check:
- the same calls in the same order with the same registers and stores; the direct isysGroupForNext is the very native the
  table holds (taken only while the slot holds it), so its stores are those of the old path;
- `SOTC_W3_CHECK=1` on both routes: every inline GetScriptClass compared with the native on a context copy (v0 / v1 / sp / ra /
  pc; the frame word is the same value): **ride7 387 194 checks, 0 differ; lake6 428 611 checks, 0 differ**;
- the routes play out: ride7 on mounted Agro and rode to the end (frame 13 398); lake6 on ran to frame 15 498 in the lake.

| run | switches | result |
|---|---|---|
| onRA | ride7, SOTC_W3=1 SOTC_W3_CHECK=1 SOTC_W3_LOG=600 | mounted 9 309 (the mount frame varies 2 200-9 300 between boots of the same build), 0 differ |
| offRA | ride7, none | no `[w3]` lines (main's pass); mounted 3 180 |
| onLA / offLA | lake6, as onRA / offRA | 0 differ / no `[w3]` |
| clkOnA / clkOffA | ride7, SOTC_P2BRACKET=3 SOTC_P2BRACKET_CLOCK=1, W3 on / off | 101 functions wrapped, level 3; the table in section 1; mounted 2 748 / 4 828 |

Vita3K's frame times are not a measure here (shared host, one CHECK run, mount frames apart): the console A/B decides.

**Pictures** (looked at): onRA t0300 Wander on foot with the sword on the grass plain, the ruined arches and the long bridge
behind; t0540 galloping on Agro beside a tall dark stone wall with hanging roots, pillars and cloudy sky on the right, Agro's
shadow. offRA t0540 Wander on Agro in the shrine's stone corridor. onLA t0300 / t0540 Wander swimming in the misty lake by
the stone wall and the iron fence, the HUD's stamina circle and sword. Nothing missing or garbled.

**What W3 gains** (estimate, not measured on the console yet): the walk's share. GetScriptClass inline for ~half the objects
(no table call, no extra frame round trip), isysGroupForNext without the table and callGuest's register traffic for every
object, the next object's lines prefetched (P3: 0.46 M D-stall in the pass's self; the walk's part of it), the rare calls out
of line (fewer I-lines). Expected on the order of 0.1-0.3 ms of the ~0.8 ms the walk is on the console. **The script bodies
(~1.6 M cycles, ~3.6 ms) are untouched**: they are the AI's long tail (execSCRBaseFunc's functions, the animal and colossus
scripts, the frame end's per-type work), and are where the script system's time is; the next rounds should take them by
weight with the new `scripts:run_tNN` brackets (SOTC_P2BRACKET=4) or prof129 under `scripts:subThreadRun`. The camera was not
rewritten: its cost is doit_cameraman's lifted helpers and the collision callbacks, measured by the level-3 brackets.

## 5. Console steps

`build/agent-scratch/w3/` (README.txt): `celosia-w3.vpk` (ELF `celosia-w3.elf`), `env-w3-off.txt` / `env-w3-on.txt` (P3's
env-p3-bracket.txt with SOTC_P2BRACKET=1 and no SOTC_P2PMUSAMPLE; on adds SOTC_W3=1 SOTC_W3_LOG=600). Two rides of the usual
route, off then on, same VPK; send both err.txt. Compare execScript's inclusive and `[frame] game` between them
(`python3 tools/p2_report.py err-w3-{off,on}.txt --elf celosia-w3.elf`). Optional third ride with SOTC_P2BRACKET=3: the split of
section 1 in console cycles (bodies vs walk, the camera's steps).
