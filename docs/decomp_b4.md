# Round B4: the dismount, and the carriers' baton cost (switch b4fast) (2026-11-22)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B4. Base: main 1a27e8d (B3). Switch `SOTC_B4FAST` (A/B name
`b4fast`, sotc_decomp `kB4Fast`, **off by default**; it acts on the carriers of b1sound / b2load / b3threads and does nothing
without them). Code: PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp (`EeScheduler::carrierNest`, the hook in
`carrierYield`), include/runtime/ee_scheduler.h (`g_ps2xB4Fast`, `EeB4Stats`), game/overrides/b3_threads.cpp (the switch, the
`[b4fast]` line), game/overrides/sotc_replay.cpp (`SOTC_RIDE_PROBE`, `SOTC_REC_ROUTE`: test aids for the dismount question).
Scripts, runs and logs: `build/agent-scratch/b4/` (NOTES.md has every command). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c
(now pointed at b4; 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. The dismount (B3's bootR, frame 9 206)

**Verdict: a game outcome of the route's timing, not a divergence.** Not reproduced in 13 further mounted rides this round
(0 dismounts; b3threads on in all, b4fast on in most); with the same recorded input and frame steps, b3threads on and off
follow the same trajectory as closely as two replays of the same configuration follow each other.

**How the ride route drives Wander** (game/overrides/sotc_replay.cpp, PS2X_INPUT_SCRIPT in ps2_pad.cpp):

- The input script runs on game frames and guest VBlanks (`PS2X_INPUT_CLOCK=frames`, "v" durations): after the mount
  (`@f0=1:WAIT`, the host flag the horse hook sets when Wander rides) it repeats, every 520 VBlanks, two CROSS kicks, the
  left stick up for 400 VBlanks and left for 100: a gallop in a polygon on the plain, forever.
- `SOTC_TELEPORT` (tp_ride7.txt): every 900 game frames from 2 000, Wander and Agro are put back near the shrine, **only while
  Wander is on foot** (skipped once he rides). `SOTC_RIDE_STEER=2130`: while on foot, a closed loop on Wander's own motion
  steers the stick toward Agro and taps TRIANGLE within 2.2 units (the mount). Off once he rides.
- SotC is a variable-step game: each frame's step is the last frame's time (gamecore_sub_013455F8 reads the EE timer), and
  after GAMECORE's entry the EE clock follows the host clock. So the same script yields a different path every run: the
  mount moved from frame 2 657 to 9 418 across this round's rides (B3: 2 668-4 998), and the gallop polygon drifts across the
  plain differently each time.

**What made Wander get off at 9 206** (B3's bootR log): at frame 9 201 the player's class +0x950 (the horse ridden) went to 0
while the AI riding flag was still set, five frames later both were 0 (`[ride] on foot`). No teleport was due (9 200's was
skipped: he was riding), no TRIANGLE (the steering only presses it on foot), and the script had been holding the stick up for
a gallop since frame 9 025 (two CROSS kicks at 9 025 / 9 042). The 300 s shot (frame ~9 330) shows Wander running on foot
toward the shrine's ruins with the cliffs to the right, Agro out of view: most likely the game's own ride-off path after a
gallop into the rocks, reached because that run's polygon happened to cross them (not proven: that run had no probe; this
round's bootR1 shows Agro galloping along a cliff face at 300 s, still mounted, so the route does brush the cliffs).
`SOTC_RIDE_PROBE=1` (new) logs the game's ride-off calls (nico_ResetPlayerHorseRidingControl 0x13B50B8, its
...AndContinueMotionCurrentSpeed 0x13B65D0, nico_resetHorseRidingStatusForHorse 0x1396F68) and the mount calls with their
caller and both places; in this round's rides it saw every mount (nico_ChangePlayerHorseRidingControlWithVCol from
manager_sub_010D1100's 0x10D1610) and no ride-off (the attract demo's resetHorseRidingStatus from 0x134AAAC shows the probe
works for those too). Next time a ride dismounts, its caller says which game path it was.

**Rides this round** (ride7, `[ride]` lines; all with b1sound + b2load + b3threads on):

| run | A/B | b4fast | mounted at | frames | dismounts |
|---|---|---|---:|---:|---:|
| chkR (checks) | — | on | 2 657 | 18 329 | 0 |
| mixR7 / mixR13 | b4fast | A/B | 4 651 / 3 195 | 25 488 / 24 904 | 0 / 0 |
| mixR7t / mixR13t | b3threads | on | 2 852 / 2 228 | 24 173 / 22 286 | 0 / 0 |
| bootR1 / bootR2 / bootR3 (89 MB) | — | on | 3 355 / 3 635 / 9 418 | 19 113 / 18 586 / 17 911 | 0 / 0 / 0 |
| p2smR / p3smR (smoke 240 s) | — | on | 5 841 / 7 281 | 7 310 / 7 300 | 0 / 0 |
| mtNo / mtPr (300 s, probe off / on) | — | off | 6 094 / 5 384 | 8 861 / 8 884 | 0 / 0 |
| mtB3 (B3's own build p1, 300 s) | — | off | 7 536 | 8 963 | 0 |

13 mounted rides, ~148 000 frames ridden after the mount, 0 dismounts (B3 had 1 in 52 recorded rides). The rides without a
mount (ldOnR / ldOffR 360 s, recR and its replays) are not counted.

**The same input, carrier on / off.** An input recording fixes what a replay can fix: each frame's timer count (the step), the
pad state each frame, the VBlank count four readers see, the main loop's frame count and garbage choice, the loader's reads
(sotc_replay.cpp's header lists what it does not cover: inline VBlank reads, the sound driver, render completions). The ride
route's aids were turned off under a recording; `SOTC_REC_ROUTE=1` (new) keeps them (teleports, steering, flags), so a ride7
run can be recorded and replayed. recR (ride7 600 s, b1sound + b2load + b3threads on) was recorded, then replayed twice with
b3threads on (repOn, repOn2), twice off (repOff, repOff2) and once with b3threads + b4fast (repB4). Wander's place against
the recording / each other, at replay ticks (units; ~548 ticks of 30 compared per pair):

| pair | tick 2 400 | 4 800 | 9 600 | 14 400 |
|---|---:|---:|---:|---:|
| repOn vs recording | 0.02 | 2.09 | 14.19 | 22.85 |
| repOff vs recording | 0.02 | 2.09 | 14.21 | 19.05 |
| **repOn vs repOn2 (A/A)** | 0.00 | 3.91 | 9.85 | 25.47 |
| **repOff vs repOff2 (A/A)** | 0.00 | 2.09 | 13.95 | 21.65 |
| repOn vs repOff | 0.00 | 0.00 | 0.07 | 4.19 |
| repOn2 vs repOff2 | 0.00 | 1.83 | 10.53 | 36.00 |
| repOn vs repB4 | 0.02 | 5.04 | 12.52 | 35.86 |

Every replay leaves the recording the same way, and the first thing that moves is a route aid the recording does not key to
ticks: the first teleport fires at the first Agro step after frame 2 000 once the area is loaded, which landed on frame 2 062 /
2 067 / 2 069 / 2 070 depending on the load's timing. Two replays that happened to put it on the same frame (repOn and repOff,
both 2 070) stay within 0.07 units of each other for 9 600 ticks with b3threads on in one and off in the other; two replays of
the *same* configuration whose teleport landed differently drift apart by 4-25 units, as much as any on / off pair. So the
carrier adds no divergence of its own on top of the replay's own timing noise. (None of the six mounted Agro: the recorded
steering did not reach him in recR, and the replays replay that.)

**Gameplay stays exact** in the sense of decomp_b1/b2/b3: the same guest operations in the same order, the dispatcher's
decisions; which frame the route's ride ends on is chosen by timing, as the mount frame is.

## 2. B3's cost, as B3 read it (the hand-overs)

B3's carriers pass one baton with the executor (the EE dispatcher's host thread): every yield of a carrier (a checkpoint that
must reach the scheduler, a park, a thread switch) is a hand-over to the executor (a kernel semaphore signal + wait, a host
thread switch), the executor runs run()'s loop (the events, the interrupt handlers, the next thread), and resuming the next
carried thread is a second hand-over. Riding, a frame had ~8.2 yields of B3's four carriers plus B1 / B2's ~2.3 (t4 2.0 parks,
t10 2.05 parks, t9 1.0 checkpoint + 1.0 park, t8 2.06 parks, t5 0.1-0.5 checkpoints): ~20 hand-overs a frame. t9's checkpoint
yield is the one S17 had made cheap on the executor (the VBlank / GS handler nested on the game thread's own stack): carried,
it became two hand-overs.

## 3. The design: the dispatcher's loop on the carrier that yields (b4fast)

`EeScheduler::carrierNest`, called from `carrierYield` while b4fast is on (once GAMECORE runs, as the carriers):

- The carrier that yields runs **run()'s loop itself**, on its own host thread, while it holds the baton. It is the executor
  while it does (resumeCarrier had installed its executor identity, host stack floor, guest jumps' serial; m_activeCarrier is
  cleared for the loop, so a checkpoint inside a handler run there unwinds to this loop as it would to run()). The loop is
  S17's nested dispatch (s17Nest) for carriers, which is run()'s own loop body: the events taken where run() takes them (after
  the function returned, then at the loop's top), the reschedule, the interrupt handlers queued on the thread run() would
  queue them on and run there as generated code, the woken threads picked by priority (selectReady / makeRunning), each
  thread's resume completion, the dispatch checkpoint's 8 cycles before a dispatch, the shared VU0 registers handed over as
  run() hands them, updateGate before a dispatch.
- **Its own thread picked again** (run() would resume this carrier there): the thread goes on in place. No hand-over.
- **Another carried thread picked** (run() would resume that carrier now): the **baton goes straight to that carrier**
  (`SOTC_B4_MODE=2`, the default with the Vita's semaphore baton). The yielding carrier keeps its state as resumeCarrier keeps
  it after a yield (its frames that cannot return, its jumps' serial), installs the next carrier's executor identity as
  resumeCarrier installs it, signals that carrier's semaphore and waits on its own. It wakes when some loop (another
  carrier's, or the executor's run()) picks its thread again and installs its identity the same way; the yield then returns
  and its thread goes on. One hand-over a thread switch instead of two.
- **Back to the executor** (the yield's B3 hand-over, with the scheduler's state exactly as run() has it at that point of its
  loop; run() goes on from there): no thread ready (run() would wait for an event or start a pending invocation's owner), the
  next thread runs as generated code (t12, a carrier handed back; run() runs it on the executor, where S17 nests the game
  thread's checkpoints), the yielding thread no longer ready or waiting (suspended / deleted), the stop, the switch off or a
  hand-back pending, less than 192 KB of host stack. The executor waits on whichever carrier gives the baton back
  (`g_b4ExecWait`: the carrier it handed the baton to; `g_b4Returner`: the one that gives it back, whose state it saves and
  which it hands back / tells to unwind exactly as B3 did for the carrier it resumed).
- `SOTC_B4_MODE=1`, the nested form (builds p1 / p2, kept for comparison): the other carrier is resumed from the yielding
  carrier's loop and yields back to it (two hand-overs, as through the executor); resumed so, its own yields run the loop
  only until it would run another thread (the "local" loop: t9's checkpoint yields go on in place).
- `[b4fast]` (with `SOTC_B3_LOG`, or every 600 frames when b4fast is on): loops opened at a checkpoint / a park, went on in
  place, given back to the executor by reason, direct hand-overs, what ran in the loops, and the scheduling decisions a frame
  by thread (makeRunning, g_eeS11.runs) with the invocations queued: the guest-level counts that must keep their shape on /
  off.

## 4. Why it stays exact

- **The same decisions in the same order.** The loop is run()'s loop body (S17's, already exact and on by default for the
  game thread on the executor); it runs exactly where run() would have run it (the yield is where the generated code would
  have unwound and the carrier would have handed the baton back), on another host thread that holds the baton and the
  executor's identity. Everything guest-visible it does (events, handlers, completions, picks, the 8-cycle charges, the VU0
  hand-over) is run()'s, in run()'s order. The direct hand-over does what resumeCarrier does (the identity installed, the
  carrier's turn) without the executor's round trip in between; the executor's own steps between two dispatches that it
  skips are its debug publication (publishDebugContext, the snapshot every N dispatches) and its statistics.
- **One runner at a time.** Every hand-over is a semaphore signal followed by a wait on the signaller's own semaphore: the
  baton is in exactly one host thread, and whatever one side wrote is seen by the next after the signal / wait pair (seq_cst
  fences on both sides, as B1b).
- **Handing back is unchanged**: a carrier whose switch went off gives the baton to the executor at its next yield, and the
  executor tells it to unwind (B3's path, now for the carrier that gave it back).
- **Checked** (section 5): the executor audit (guest code reaching the scheduler from a host thread that is not the current
  executor), every loop's kept registers, B1's ticks = VBlanks and VBlank count = sound ticks, B2's jobs, the watchdog, the
  per-thread pick counts on / off.

## 5. Checks

Build p3 (= p4 for the default mode), Vita3K 222 MHz, muted, `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1`
with all three checks (`SOTC_B1SOUND_CHECK=1 SOTC_B2LOAD_CHECK=1 SOTC_B3_CHECK=1`: the executor audit for every carrier and
for every host thread that runs a loop, the kept registers of the six loops once a turn, the per-box / per-semaphore counts,
the watchdog at 10 s) and `SOTC_RIDE_PROBE=1`:

| run | frames | executor audit | kept registers | hand-backs | B1: windows ticks = VBlanks; VBlank count = sound ticks | B2 jobs / in-place waits | a frame: loops cp / park, went on in place, direct hand-overs, handlers in loops | watchdog |
|---|---:|---|---|---|---|---|---|---|
| ride7 600 s (chkR) | 18 329 | 0 bad of 423 682 | 0 bad | 0 | 30 of 30; 35 452 = 35 452 | 1 170 / 3 890 | 1.86 / 8.46, 1.41, 8.90, 3.02 | silent |
| attract 400 s (chkA) | 19 362 | 0 bad of 420 985 | 0 bad | 0 | 32 of 32; 23 567 = 23 567 | 507 / 2 107 | 1.53 / 8.23, 1.15, 8.60, 2.71 | silent |
| lake6 400 s (chkL) | 12 887 | 0 bad of 345 595 | 0 bad | 0 | 21 of 21; 23 524 = 23 524 | 378 / 1 798 | 1.08 / 7.20, 1.01, 7.26, 2.96 | silent |

**0 bad of 1 190 262**, 1 claim per carrier, 0 hand-backs of any kind, 0 loops given back to the executor (no thread ever
needed it: no generated thread ran, the idle thread is always ready), every run to its end (chkR mounted at 2 657 and rode to
the end; lake6's teleport and the boss arena; the attract demo past the title). In-place syscalls in chkR: 859 103 WaitSema,
113 045 SleepThread, 77 666 WakeupThread, 539 SignalSema. The smoke runs of each build (p1 / p2 / p3, ride7 and attract 240 s,
all checks) were clean as well.

**Hand-overs a frame, riding** (last 600-frame window of chkR): B3 passed the baton ~20 times a frame (each of ~10 yields
twice); b4fast passes it ~9 times (8.90 direct hand-overs; the executor's own round trips are gone: 0 given back), and 1.41
checkpoint yields a frame go on with none (t9's VBlank / GS handler run in place, S17's case).

**The scheduling decisions on / off** (`[b4fast]`'s picks by thread, makeRunning a frame, ldOnR (b3threads + b4fast) / ldOffR
(both off), ride7 360 s, b1sound + b2load on both): t4 2.13 / 2.05, t5 1.04 / 1.01, t8 2.29 / 2.11, t9 1.39 / 1.13, t10 2.15 /
2.06, t11 0.33 / 0.10; lake6 (ldOnL / ldOffL): t4 2.05 / 2.04, t5 1.00 / 1.00, t8 2.11 / 2.08, t9 1.05 / 1.04, t10 2.07 / 2.05.
The same shape (neither ride mounted in 360 s; the ride's small differences follow its route). The box
counts (ldOnR / ldOffR): GP box 12 034 / 11 664 sends, frame box 10 767 / 10 767, the idle queue 62 / 983 (B3's sndBDOpen poll,
section 3 of decomp_b3.md: fewer, longer rounds with carriers).

## 6. Measurements

Vita3K 222 MHz, 3 cores, interleaved A/Bs (`SOTC_AB_PERIOD=1`, the switch toggled every frame), off - on (positive: on
faster), 2 SE; pooled = mean of 3, SE = sqrt(sum se^2) / 3:

**b4fast** (`SOTC_AB_FLAG=b4fast`, b1sound + b2load + b3threads on in both halves; the default direct form, build p3):

| run | frame | busy |
|---|---:|---:|
| ride7 800 s seed 7 (mixR7) | -0.153 +- 0.125 | -0.152 +- 0.105 |
| ride7 800 s seed 13 (mixR13) | -0.157 +- 0.121 | -0.138 +- 0.114 |
| lake6 400 s (mixL7) | -0.139 +- 0.087 | -0.139 +- 0.092 |
| **pooled** | **-0.150 +- 0.032 ms (1 SE), z -4.6: on slower** | -0.143 +- 0.030 |

**b3threads with b4fast on in both halves** (`SOTC_AB_FLAG=b3threads`, b1sound + b2load + b4fast on; B3's A/B was -0.174 +-
0.055):

| run | frame | busy |
|---|---:|---:|
| ride7 800 s seed 7 (mixR7t) | -0.205 +- 0.168 | -0.215 +- 0.164 |
| ride7 800 s seed 13 (mixR13t) | -0.077 +- 0.143 | -0.041 +- 0.136 |
| lake6 400 s (mixL7t) | -0.168 +- 0.098 | -0.180 +- 0.099 |
| **pooled** | **-0.150 +- 0.040 ms (1 SE), z -3.7: on slower** | -0.145 +- 0.039 |

**Where the cost is not.** The direct form cuts the baton's hand-overs from ~20 to ~9 a frame and Vita3K says it is ~0.15 ms
*slower*; so the hand-overs' count is not what costs in Vita3K. The two other forms, same A/B:

| form | ride7 800 s seed 7 | lake6 400 s | mean |
|---|---:|---:|---:|
| 2, direct hand-overs (default; above) | -0.153 +- 0.125 | -0.139 +- 0.087 | -0.146 +- 0.038 |
| 1, nested (a carrier's loop resumes the others: two hand-overs each, t9's checkpoints in place) (mixR7m / mixL7m) | -0.092 +- 0.118 | -0.182 +- 0.123 | -0.137 +- 0.043 |
| 3, a checkpoint's own loop only (only the in-place checkpoints; every switch through the executor) (mixR7s / mixL7s) | +0.022 +- 0.179 | -0.104 +- 0.105 | -0.041 +- 0.052 |

The more of the dispatcher's work moves from the executor's host thread onto the carriers, the slower Vita3K gets, even
with fewer hand-overs; the form that keeps every switch on the executor and only saves t9's handler-only checkpoints is even.
A likely reading (not proven): in Vita3K every guest thread is a host thread, and the scheduler's state and the handlers'
data then move between host cores with each loop (the executor keeps them in one core's caches; on the console the game's
threads all share core 0). The console's A/B decides (section 9).

**Period 300** (b3threads A/B with the switch toggled every 300 frames instead of every frame: B3's A/B paid a take-over and a
hand-back of four threads every frame; ride7 800 s seed 7): with b4fast (mixR7p) -0.149 +- 0.749 ms, without (mixR7q) +0.562 +- 1.308 ms (2 SE): with 300-frame halves the route's own
phases swamp the difference; this tells nothing either way.

## 7. Loads and hitches

`SOTC_FRAME_LOG_ALL` runs, on = b3threads + b4fast, off = both off (b1sound + b2load on both ways); lg.py (the Load Game
pair: the two consecutive frames with the largest sum in 600-1599) and tools/loads.py:

| run | Load Game pair | 200-1999: longest, over 100 / 200 ms | 0-1999 longest (the boot) | teleport load (frame ~2 000) |
|---|---|---|---|---|
| ride7 360 s on (ldOnR) | 210 + 268 ms | 268 ms, 8 / 2 | 539.7 ms (frame 3) | 175 ms |
| ride7 360 s off (ldOffR) | 234 + 256 ms | 256 ms, 5 / 2 | 409.6 ms (frame 156) | 151 ms (frame 2 157) |
| lake6 400 s on (ldOnL) | 168 + 199 ms | 320 ms (the teleport, 1 999), 6 / 1 | 460.6 ms (frame 3) | 320 ms |
| lake6 400 s off (ldOffL) | 204 + 211 ms | 327 ms (the teleport, 1 999), 5 / 3 | 407.2 ms (frame 155) | 327 ms |

The Load Game pair is no longer longer with the carriers on (478 vs 490 ms riding, 367 vs 415 ms on lake6; B3 had 482 vs 406
and 592 vs 454 in the same kind of single runs). The same hitches in the same places; the longest frame both ways is in the
boot before GAMECORE (frame 3 on, 155-156 off: host noise at the boot, where none of the switches acts yet).

## 8. 89 MB boots

`SOTC_VITA_EAT_USER_MB=89`, b1sound + b2load + b3threads + b4fast on, ride7 600 s, shots every 120 s (looked at), the ride probe:

| run | mounted | frames at 120 / 240 / 360 / 480 / 600 s | dismounts | system free at the end | hand-backs, watchdog |
|---|---:|---|---:|---:|---|
| bootR1 | 3 355 | 3 779 / 7 448 / 11 126 / 15 085 / 18 959 | 0 | 67.2 MB | 0, silent |
| bootR2 | 3 635 | 3 973 / 7 477 / 11 106 / 14 674 / 18 423 | 0 | 67.2 MB | 0, silent |
| bootR3 | 9 418 | 3 867 / 7 181 / 10 621 / 14 146 / 17 911 | 0 | 67.2 MB | 0, silent |

The shots: Wander running on the plain to Agro, then on Agro galloping across the grass, past the stone pillars and along the
shrine's walls and cliffs (bootR1's 300 s shot: Agro brushing a cliff face, still mounted); ground texture, Wander, Agro and
the body shadow under Agro normal, the frame counter advancing in every shot. **89 MB attract** (bootA, 300 s, shots every 60
s): the title's menu and the demo, normal (15 441 frames).

## 9. spalign, the console package, the default

- **spalign**: `python3 ../tools/spalign.py p4.elf`: 0 sp-based :128/:256 NEON accesses (of 1 038 with a hint).
- **Default: off** (`SOTC_B4FAST` must be 1-3; `SOTC_B4_MODE` 2 by default, 1 / 3 the other forms). Exact by the checks, but
  ~0.15 ms slower in Vita3K in its direct form, and even only in form 3.
- **Console A/B package**: `build/agent-scratch/b4/celosia-b4.vpk` (= p4) + `env-ab.txt`: N6's console env without its A/B
  lines + `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_AB_FLAG=b4fast SOTC_AB_PERIOD=300 SOTC_B1_LOG=600
  SOTC_B2_LOG=600 SOTC_B3_LOG=600` (all switches on, the A/B on b4fast: the hand-overs' cost on the real kernel and core 0,
  where Vita3K's host threads do not apply). `SOTC_B4_MODE=3` in the env would measure the form Vita3K calls even.

## 10. What is still open

- **The carriers' cost is not the hand-overs** (in Vita3K). B3's -0.17 ms and this round's b3threads -0.15 ms with b4fast on
  remain; the period-300 A/Bs (section 6) say how much of it was the per-frame take-over / hand-back of the A/B itself. The
  console's A/B (b3threads, B3's package; b4fast, this one) is the measure that counts.
- **Message boxes as host queues (goal 3): not done** this round (the A/Bs took it). decomp_b3.md section 8 has what it needs.
- **The replay's ride**: `SOTC_REC_ROUTE` replays drift from the first teleport because the route's teleports are keyed to
  game frames, not to the recording's ticks; recording the teleports' ticks (and making the steering reach Agro under a
  recording) would make a ride replay lockstep enough to compare a dismount itself on / off.
- With b4fast the executor's host thread is idle while the carriers run the dispatcher: a step toward dropping run() for the
  carriers altogether (decomp_b3.md's "checkpoints as pure delivery points").

## Open bug found at the merge (2026-11-23): an intermittent memory-card load stall

On the merge build (B4 rebased on A9, main 496cf72 + B4), one 89 MB ride7 boot with SOTC_B1SOUND=1 SOTC_B2LOAD=1
SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_RIDE_PROBE=1 sat on the memory card's "Loading data" screen for the whole 600 s
(frames kept advancing, 28 800 frames, menu animating). The [b3threads] line shows the idle thread t5's turns frozen at
2 848 from early on while t4 kept turning; the load needs t5 (iosWaitIdleThread polls). Two repeats right after
(all four on, and b1+b2+b3 without b4fast, 300 s each) got through the load to the field, and B4's own six boots passed:
1 stall in 7 boots with b3threads on, not yet attributed to b3threads or b4fast. Logs: build/agent-scratch/b4/runs/bootMon
(stalled), repAll / repB3 (passed). Both switches stay off by default; this must be found before either goes on.
