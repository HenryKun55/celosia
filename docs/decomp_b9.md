# Round B9: a delivery in one turn of the dispatcher's loop, the ready queues as a host bitmap (switch b9loop) (2026-11-24)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B9 (decomp_b8.md section 6: "the loop itself is the next thing
to make host-shaped"). Base: main 3061496 (B8 + A13). Switch `SOTC_B9LOOP` (A/B name `b9loop`, sotc_decomp `kB9Loop`, **off by
default**; it takes effect only while b8sched is in effect, so with b7irq and the carrier switches once GAMECORE runs). Code:
PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (`b9AfterPush`, `b9Dispatch`,
`b9SkipTop`, `b9Quiet`, the bitmap `m_b9Ready` with `b9MaskSet` / `b9MaskClear` / `b9MaskRebuild` / `b9FirstReady`, the check
`b9Digest` / `b9TopCheck`, `EeB9Stats`; the three loops' push points and slot dispatches), game/overrides/b9_loop.cpp (the
switch, the `[b9loop]` line), sotc_native.cpp / sotc_decomp.h (install, frame call, the switch tables). Scripts, runs and logs:
`build/agent-scratch/b9/` (b9chain.sh, pfre.sh, b9ab.sh; vq.out is the run log). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed at b9; 222 MHz, 3 cores,
audio-volume 0; every run muted).

**Host load during this round**: another round's Vita3K jobs ran on the same Mac the whole time (load average up to 32). The
boots got 3 000-4 800 frames in 300 s (B8's: 7 800-9 000) and game fps of 6-25; every timing below (loads, frames to mount)
is from a loaded host and says little on its own. The checks do not depend on it; the A/B (on and off alternating in the same
run) is noisier for it.

## 1. What B8 left in loop form

With b8sched a host-form interrupt (VBlank start 0x1c8180, GS 0x1c37f0) lives in its thread's slot, but the dispatcher still took
the record form's four turns for each delivery, in all three loops (run(), B4's carrierNest, S17's nest):

1. the push (the slot filled), then `continue`;
2. the loop's top (the stop test, processPendingEvents, the pick), then the run (B8's `b8Dispatch`, state 1);
3. the top again, then the pop (state 2);
4. the top again, then the thread's own path.

Each top is a few tests and calls that almost always do nothing at that instant: nothing posted, no deadline due, no reschedule.

## 2. The design

**(1) A delivery in one turn.** At the push point (`b8Push` returned true) `b9AfterPush` checks that the run turn would not turn
the slot into its record (B8's refusals: b7irq off, another handler at the pc, the GS callback refused; else the loop goes round
as before, counted "refused") and calls `b9Dispatch`. That walks the slot's steps (B8's own `b8Dispatch` for the run and the
pop) and, before each step, looks at the top the record form would run there (`b9SkipTop` -> `b9Quiet`). The top is skipped only
when it would do nothing:

- no stop requested; nothing flagged (`m_checkpointPending`: an event, an invocation queued from elsewhere); no timer interrupt
  latched; no deadline due by cycles (`m_nextDeadlineCycle` <= the EE clock); no reschedule asked;
- the thread is still the running one (the pick would pick it) and has no completion to run.

When a top would do something the loop goes round as before and the slot's state (B8's 1 / 2) carries the delivery on at the
thread's next dispatch, again through `b9Dispatch` (with b9loop the loops call it in place of `b8Dispatch`). When push, run and
pop all pass, the caller jumps to the thread's own path in the same turn (`b9Own0` / `b9Own1` / `b9Own2`). So the steps, their
order, the picks and the EE cycles stay the record form's: only tops that do nothing are left out.

**(2) The ready queues as a host bitmap.** `m_b9Ready` (4 x 32 bits, bit p set while priority p's queue is not empty), kept
always by `enqueueReady` / `removeReady` / `selectReady` (and rebuilt at reset and at B7's check restore). With b9loop the pick
(`selectReady`) and the preemption test (`hasReadyAtOrAbovePriority`) take the highest non-empty queue with a count-trailing-zeros
instead of scanning 128 queues.

Switch: `SOTC_B9LOOP=1` (A/B `b9loop`), off by default; b9_loop.cpp turns it on for the next frame only while b8sched is in
effect (so with b7irq and the carrier switches once GAMECORE runs).

## 3. Why it stays exact

- A top's work is decided by state only the executor changes (between two of a delivery's steps nothing else runs on it) and by
  flags other host threads set (flagCheckpoint). `b9Quiet` tests exactly the conditions processPendingEvents' fast path, its
  preemption step, the loops' stop tests and the pick look at; when all are clear the top is a no-op. A flag set by another host
  thread after the test is seen at the next top that runs, as one set a moment later would be in the record form.
- The bitmap is a cache of "queue p is not empty", updated at every place a queue changes; the pick returns the same queue's
  front as the scan.
- The check (section 4) runs the skipped tops for real and compares, and compares every bitmap answer with the scan.

## 4. Checks

`SOTC_B9_CHECK=1` with every earlier check on (B1 sound, B2 load, B3 audit, B6 shadow, B7 handlers, B8 steps), all eight
switches on (b1sound b2load b3threads b4fast b6sync b7irq b8sched b9loop), p1 build. At each skip point the check alternates: one
runs processPendingEvents right there and compares a digest of the scheduler's state (running thread, reschedule / pending / stop
flags, latched timer interrupts, queued invocations, deadlines and events, VBlank count, EE clock, ready queues, every thread
record's state) before and after ("tops run here"); the next goes round as the record form does with its prediction armed, and the
loop's own top compares the state it finds ("predicted"). A difference with a flag / post from another thread in between counts
as a race, without one as bad. (Because every other skip point goes round, a check run shows 0 deliveries in one turn: expected.)

| run | tops run here (bad / raced) | predicted tops (bad / raced) | bitmap answers (bad) | B8 steps (bad) | B7 handler checks (bad) | B6 shadow checks (bad) | B3 audits (bad) |
|---|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (chkR, mounted 3 345) | 47 602 (0 / 0) | 47 603 (0 / 1) | 92 123 (0) | 163 089 (0) | 35 029 (0) | 2 924 537 (0) | 154 747 (0) |
| attract 400 s (chkA) | 40 394 (0 / 0) | 40 395 (0 / 1) | 90 120 (0) | 148 796 (0) | 30 259 (0) | 2 015 882 (0) | 138 592 (0) |
| lake6 400 s (chkL) | 31 480 (0 / 0) | 31 480 (0 / 1) | 56 082 (0) | 104 255 (0) | 22 934 (0) | 1 597 247 (0) | 72 454 (0) |
| ride7 240 s smoke (sm1R) | 18 308 (0 / 2) | 18 309 (0 / 0) | 40 946 (0) | 67 013 (0) | 13 551 (0) | - | 56 077 (0) |

**0 bad of 275 571 top comparisons (137 784 run here, 137 787 predicted) and 279 271 bitmap comparisons**; 5 races (a post from another host thread landed between the
test and the compare: what the design allows). B8's step shadow, B8's charge and regate checks, B7, B6, B3 and the B1 / B2 audits still 0 (check-mode generated runs that unwound turned ~0.002 slots a frame back into records, the fallback B8's check has). No
`[b9chk]` line in any run.

## 5. Counts, boots, loads, A/B, spalign, console package

**Counts** (`[b9loop]` / `[b8sched]` / `[s11]` lines, a frame, b9loop on, no check):

| run | slots (deliveries) | delivered in one turn | loop turns skipped | went round: not running | picks / ready tests from the bitmap |
|---|---:|---:|---:|---:|---:|
| bootR (ride7 600 s, to frame 13 200) | 3.07 | 1.92 | 8.07 | 1.15 | 5.22 / 4.87 |
| sm1On (ride7 240 s) | 5.32 | 4.17 | 14.81 | 1.14 | 4.44 / 4.43 |
| pf3 (ride7 300 s) | 5.69 | 4.51 | 15.89 | 1.17 | 5.12 / 4.84 |
| pr2 (ride7 450 s) | 6.33 | 5.02 | 17.69 | 1.31 | 7.36 / 6.15 |

So 63-80 % of the deliveries now take one loop turn instead of four; the rest (~1.1-1.3 a frame) go round because the handler
made another thread the one to run (a handler's wakeup, e.g. the VBlank handler's): there the pick has real work, and the
loop does it as before. No top was ever skipped past a flag, a timer interrupt, a deadline or a reschedule (those "went round"
reasons stay ~0: the only one seen is 0.002 flagged in pr2). Deliveries turned into records: 0.

What B9 does **not** change: the slow checkpoints. bootR has 5.6 a frame ([s11]: pending 2.3, deadline 0.1, go on 3.2), sm1Off
(b9loop off, same build) 5.3, sm1On 5.5; B8's counts the same order (carriers ~3.0 + elsewhere ~2.2). B9 removes loop turns, not
checkpoints: each skipped turn is a few tests and a processPendingEvents call, well under a microsecond each on the Mac.
The regates at timer reads (B8 part 2) stay at 3.1-3.3 a frame.

**Boots** (89 MB, `SOTC_VITA_EAT_USER_MB=89`, ride7, all eight switches on): **pf1-pf10 (300 s): 10 of 10 booted on the first
try and reached the field (frame 2 000); 5 mounted Agro** (pf3 at frame 2 285, pf5 3 389, pf6 2 275, pf8 2 447, pf10 2 825).
The other five were still on their way when the 300 s ended: on the loaded host they got 3 600-4 300 frames (game fps 6-19), and
B8's mounts came as late as frame 6 091. Their reruns at 450 s (pr1-pr5): **pr2-pr5 mounted** (2 582, 2 803, 2 411, 2 597);
pr1 reached frame 5 400 without mounting (no freeze or watchdog line, the frame count still moving). So 9 of the 10 boot slots
mounted, 15 of 15 boots reached the field; 0 dismounts, 0 watchdog lines (b1dog / b5dog) in all 15.

**Loads** (on vs off, `SOTC_FRAME_LOG=1`, 89 MB; `python3 lg.py runs/<r>/err.txt`; on a loaded host, see above):

| | Load Game pair | frames 200-1999: longest / >100 ms / >200 ms | teleport load |
|---|---|---|---:|
| ride7 on (ldOnR) | 366 + 89 ms | 1 397 ms / 102 / 43 | 465 ms |
| ride7 off (ldOffR) | 326 + 86 ms | 326 ms / 74 / 18 | 246 ms |
| lake6 on (ldOnL) | 918 + 449 ms | 918 ms / 290 / 188 | 353 ms |
| lake6 off (ldOffL) | 451 + 318 ms | 531 ms / 131 / 58 | 466 ms |

All four are far slower than B8's (Load Game ~160 + 200 ms, 6-7 frames over 100 ms) and differ in both directions (lake6's
teleport faster on, ride7's slower on), so these runs show the host's load, not b9loop (they ran next to the boot series and the other
round's jobs). b9loop touches only the dispatcher's loop turns, not the loader's path (B2 / B5), and the routes loaded
fine on and off. ldOnA / ldOffA (attract) never entered the field (classify.py "STALL", field-at 0) on and off alike, as in B6 /
B8: not a B9 failure. A re-measure of the loads on an idle host (or on the console) is still to do.

**Vita3K A/B** (222 MHz, `SOTC_AB_FLAG=b9loop`, off - on +- 2 SE over blocks of 300 frames): mixR7 **-0.012 +- 0.228 ms** (63
blocks), mixR13 **-0.721 +- 0.613 ms** (51 blocks), mixL7 **-0.014 +- 0.314 ms** (29 blocks): no measurable difference (mixR13's
-0.7 is just past 2 SE in the "on slower" direction, the other two are 0; the host was loaded), as expected for removing a few
no-op loop turns a frame.

**89 MB boot with pictures** (bootR, ride7 600 s, all eight switches, shots every 120 s): booted on the first try, field at
frame 2 000, **mounted at frame 4 294 and rode the full 600 s** (13 800 frames; 0 dismounts, 0 watchdog lines; slots 3.07,
deliveries in one turn 1.92, 0 records). Pictures at 300 s (galloping across the grass plain, Agro and Wander with their ground
shadow, ruins and trees on the horizon, sky) and 540 s (galloping past the cliffs, ground texture, shadow) normal.

**spalign**: `python3 ../tools/spalign.py p1.elf`: 0 sp-based :128/:256 NEON accesses (of 1 064 with a hint). ELF room
unchanged (122 880 bytes).

**Default**: off (`SOTC_B9LOOP` must be 1-3; it acts only while b8sched and b7irq are in effect). **Console A/B package**:
`build/agent-scratch/b9/celosia-b9.vpk` (= p1, byte-identical; built from this round's code commit's sources) + `env-ab.txt`
(B8's console env + `SOTC_B9LOOP=1 SOTC_AB_FLAG=b9loop SOTC_AB_PERIOD=300` and `SOTC_B9_LOG=600`). Not installed.

## 6. What is left of the PS2 scheduler

With all eight switches: every live thread after GAMECORE runs as host code on a carrier, the boxes are host wait lists, the two
live handlers are host functions delivered without invocation records and, when nothing else is due, in one loop turn; the
ready queues are found through a host bitmap. What EeScheduler still does in guest shape:

- **The thread records and run()'s decisions** (priorities, ready queues as 128 deques, wait states, wakeup counts; every switch
  still decided by run()'s loop body, on the executor or in carrierNest), one baton between carriers. B9 shortened the loop around
  a delivery but kept its steps: a delivery that wakes another thread (~1.2 a frame) still goes round for the pick, and every
  other switch (a wait, a wakeup, a sleep) is still a loop turn with its top. The next step is the switch itself: a carrier
  handing over to the next thread directly instead of through run()'s top.
- **The EE clock and checkpoints**: guest code still charges cycles (8 a call, 32 a back edge) and the clock decides when a
  deadline is seen between parks; slow checkpoints ~5.5 a frame (pending ~2.3, go on ~3.2, deadline ~0.1).
  B9 does not touch them. The charges cannot go while EE time between parks is the charged cycles.
- **The non-host interrupt paths**: any interrupt / alarm / callback without a host form still uses records (none live after
  GAMECORE: 0 materialized in every run), and the boot-time interrupts before GAMECORE.
- **The semaphore / event-flag records keyed by guest id**, the hand-back / resume labels, the bodies as C++ over R5900
  registers, the boot threads (t1 / t2 / t3 / t6 / t7 / t13-t16) as generated code: unchanged.
- The timer model in PS2Memory and the IOP's clock (kept for the readers' syncs).
