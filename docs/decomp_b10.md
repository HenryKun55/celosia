# Round B10: the thread switch without the dispatcher's loop top (switch b10switch) (2026-11-24)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B10 (decomp_b9.md section 6, first item: "the switch itself: a
carrier handing over to the next thread directly instead of through run()'s top"). Base: main b189f29 (B9). Switch
`SOTC_B10SWITCH` (A/B name `b10switch`, sotc_decomp `kB10Switch`, **off by default**; it takes effect only while b9loop is in
effect, so with b8sched, b7irq and the carrier switches once GAMECORE runs). Code: PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp
+ include/runtime/ee_scheduler.h (`B10_ROUND`, `b10Skip`, `b10Quiet`, the labels `b10Pick0` / `b10Pick1` / `b10Pick2` at the three
loops' picks, the check `b10Top` / `b10Pick` / `b10Front`, `EeB10Stats`), game/overrides/b10_switch.cpp (the switch, the
`[b10switch]` line), sotc_native.cpp / sotc_decomp.h (install, frame call, the switch tables). Scripts, runs and logs:
`build/agent-scratch/b10/` (smoke10.sh, b10chain.sh, b10ab.sh; vq.out is the run log). Vita3K trees ../vita3k-celosia-b1 / -b1b /
-b1c (pointed at b10; 222 MHz, 3 cores, audio-volume 0; every run muted).

**Builds**: p1 covered every `continue` with no thread running except the push points (a slot filled whose delivery then made
another thread the one to run went round there); its smoke showed it (sm1On: "after a delivery" 0 while B9's "not running" was
1.1 a frame). p2 adds the push points (`B10_ROUND` after `b9AfterPush` in the three loops) and is the round's build: every result
below is p2's unless marked p1. p1's checks (sm1R, chkR / chkA / chkL) were also 0 bad (93 563 tops run here, 93 566
predicted, 0 bad; B3-B9 checks 0).

**Host load**: round A14's Vita3K jobs ran next to these (load average 10-22); the boots got 7 200-14 300 frames in 450 s
(game fps 7-36). Timings below are from a loaded host.

## 1. What B9 left in loop form

Every thread switch still went through a loop's top. After a run that parked (WaitSema, SleepThread, an event flag), gave way
(a wakeup or a signal of a higher-priority thread), was preempted or ended; at a nest's entry (a carrier yielding for a switch:
`carrierYieldPinned` -> `carrierYield(1)` -> B4's `carrierNest`); after a completion; after a slot-held delivery whose handler
made another thread the one to run (B9's "went round: not running", ~1.1-1.3 a frame) the dispatcher did:

1. the settle after the run: processPendingEvents and the reschedule test (with no thread running: nothing to requeue);
2. `continue`: the loop's top: the stop test, processPendingEvents **again**, the stop test;
3. the pick (B9's bitmap) and the next thread's dispatch (a carrier: B4's direct baton hand-over, or resumeCarrier).

Step 2 runs right after step 1 with nothing in between.

## 2. The design

At each of those `continue`s, in run(), carrierNest and S17's nest (`B10_ROUND(loop, where, label)`), `b10Skip` tests whether the
top would do anything (`b10Quiet`):

- no stop requested; nothing flagged (`m_checkpointPending`: an event posted, an invocation queued, a flag from another host
  thread, a re-flag left by step 1); no timer interrupt latched; no deadline due by cycles (`m_nextDeadlineCycle` <= the EE clock);
  no reschedule pending; no B9 / B10 check prediction armed;
- no thread running (`m_currentThreadId == 0`: it is a switch; a `continue` with the thread still running is left alone).

When all hold, the caller jumps to its loop's pick (`b10Pick0` / `b10Pick1` / `b10Pick2`, just after the top), so the pick and
the next thread's dispatch follow the settle at once; on the Vita with B4's semaphore baton a carrier's park now goes: settle,
pick, the next carrier's dispatch, the baton, in one pass of its own loop. Otherwise the loop goes round as before. The
pick, the dispatch steps, the hand-over and the EE cycles are the record form's; only tops that do nothing are left out.

Switch: `SOTC_B10SWITCH=1` (A/B `b10switch`), off by default; b10_switch.cpp turns it on for the next frame only while b9loop is in
effect.

## 3. Why it stays exact

- With no thread running, processPendingEvents with nothing flagged, no timer interrupt latched and no deadline due by cycles
  is its fast path, `applyPendingPreemption`, which does nothing without a reschedule pending; the stop tests see no stop. So a
  top that `b10Quiet` passes is a no-op, and the loop's next steps (the pick, the dispatch) are what the record form does after it.
- What a top would act on is either state only the executor changes (unchanged between the settle and the test) or a flag other
  host threads set (flagCheckpoint, under or after the event lock); one set after the test is seen at the next top or slow
  checkpoint, as one set a moment later would be in the record form (the same argument as B9's).
- The check (section 4) runs the skipped tops for real, and compares the loop's own top and pick with the prediction.

## 4. Checks

`SOTC_B10_CHECK=1` with every earlier check on (B1 sound, B2 load, B3 audit, B6 shadow, B7 handlers, B8 steps, B9 tops / bitmap),
all nine switches on (b1sound b2load b3threads b4fast b6sync b7irq b8sched b9loop b10switch), p2. At each skip point the check
alternates: one runs processPendingEvents right there and compares B9's digest of the scheduler's state (running thread, flags,
latched timer interrupts, queued invocations, deadlines and events, VBlank count, EE clock, ready queues, every thread record's
state, wakeup count, wait reason, invocations) before and after ("tops run here"; the pick then follows as after that top); the
next goes round as the record form does with the state and the pick predicted (the front of the highest non-empty ready queue
by the queues' own scan, not the bitmap), and the loop's own top compares the state ("predicted") and its pick compares the
thread picked ("picks"). A difference with a flag / post from another host thread in between counts as a race, without one as
bad.

| run | tops run here (bad / raced) | predicted tops (bad / raced) | picks (bad) | B9 tops here / predicted / bitmap (bad) | B8 steps (bad) | B7 (bad) | B6 shadow (bad) | B3 audits (bad) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (chkR, mounted 4 420) | 29 574 (0 / 15) | 29 574 (0 / 24) | 29 550 (0) | 54 381 / 54 382 / 117 301 (0) | 195 618 (0) | 40 572 (0) | 3 833 530 (0) | 153 010 (0) |
| attract 400 s (chkA) | 33 164 (0 / 16) | 33 164 (0 / 21) | 33 143 (0) | 47 651 / 47 651 / 135 514 (0) | 197 701 (0) | 36 896 (0) | 3 106 299 (0) | 182 375 (0) |
| lake6 400 s (chkL) | 17 438 (0 / 20) | 17 439 (0 / 16) | 17 423 (0) | 33 313 / 33 314 / 69 796 (0) | 118 502 (0) | 24 729 (0) | 2 086 465 (0) | 90 758 (0) |
| ride7 240 s smoke (sm2R) | 13 237 (0 / 16) | 13 237 (0 / 15) | 13 222 (0) | 20 479 / 20 479 / 51 638 (0) | 78 780 (0) | 15 278 (0) | 1 522 503 (0) | 73 777 (0) |

**0 bad of 186 827 top comparisons (93 413 run here, 93 414 predicted) and 93 338 pick comparisons** (a predicted top that raced
is not pick-compared); 143 races (a post from another host thread between the test and the compare: what the design allows).
B9 (0 of 311 650 tops, 374 249 bitmap answers), B8 (0 of 590 601 steps; charges and regate predictions 0), B7 (0 of 117 475),
B6 (0 of 10 548 797), B3 / B1 / B2 audits (0 of 499 920; registers 0) still 0. Slots turned into records: 0. No `[b10chk]` /
`[b9chk]` / `[b8chk]` line in any run.

## 5. Counts, boots, loads, A/B, spalign, console package

**Counts** (`[b10switch]` / `[b9loop]` / `[s11]` lines, a frame, all nine on, no check):

| run | tops skipped before a pick | nest entry (a carrier's switch) | after a delivery | went round (any reason) | picked another carrier | slow checkpoints |
|---|---:|---:|---:|---:|---:|---:|
| bootR (ride7 600 s, to frame 12 000) | 6.03 | 4.77 | 1.26 | 0 | 6.03 | 6.6 |
| sm2On (ride7 240 s) | 5.40 | 4.25 | 1.15 | 0 | 5.40 | 5.8 |
| sm2Off (b10switch off, same build) | - | - | - | - | - | 6.1 |
| pf1-pf10 (450 s) | 4.14-6.50 | | 1.04-1.27 | 0 | all | 4.6-6.9 |

Every switch after GAMECORE is now made without a loop top: all of them are in carrierNest (the carriers' own loops; run() and
S17's nest made none riding), and "went round" stayed 0 for every reason in every run without the check: the top before a pick
never had work. "After a delivery" matches B9's "not running" (bootR 1.26 vs 1.257): the deliveries that wake another thread now
go to the pick at once too. Picks after a skipped top were another carried thread every time (own thread 0.00-0.01, generated
0, none 0).

What B10 does **not** change: the slow checkpoints (~5-7 a frame on this host: pending ~2.3-2.5, deadline ~0.1, go on ~3.2-4.0;
B9's bootR 5.6) and the EE cycle charges. It removes one processPendingEvents call and the stop tests per switch (~5-6 a frame).

**Boots** (89 MB, `SOTC_VITA_EAT_USER_MB=89`, ride7, all nine switches on, 450 s): **pf1-pf10: 10 of 10 booted on the first try and
reached the field (frame 2 000); 9 mounted Agro** (pf1 at frame 6 240, pf2 5 201, pf3 3 080, pf4 4 087, pf6 9 724, pf7 2 509, pf8
5 102, pf9 2 481, pf10 3 086). pf5 reached frame 7 237 without mounting (game fps 24 at the end, the frame count moving, no
watchdog line); its rerun **pr5 mounted at frame 3 026**. So **10 of 10 boot slots mounted**, 11 of 11 boots reached the field;
0 dismounts, 0 watchdog lines (b1dog / b5dog) in all 11. (The late mounts, up to 9 724, are the loaded host: B8 saw up to 6 091
on an idle one.)

**Loads** (on vs off, `SOTC_FRAME_LOG=1`, 89 MB; `python3 lg.py runs/<r>/err.txt`; off = b9loop on, b10switch off):

| | Load Game pair | frames 200-1999: longest / >100 ms / >200 ms | teleport load |
|---|---|---|---:|
| ride7 on (ldOnR) | 185 + 222 ms | 222 ms / 6 / 1 | 170 ms |
| ride7 off (ldOffR) | 162 + 194 ms | 194 ms / 6 / 0 | 167 ms |
| lake6 on (ldOnL) | 162 + 193 ms | 316 ms / 5 / 1 | 316 ms |
| lake6 off (ldOffL) | 182 + 194 ms | 326 ms / 5 / 1 | 326 ms |

The same loads on and off (B8's idle-host numbers: Load Game ~160 + 200 ms, 6-7 over 100 ms; teleports 170 / 325 ms). ldOnA /
ldOffA (attract) never entered the field (classify.py "STALL") on and off alike, as in B6 / B8 / B9: not a B10 failure.

**Vita3K A/B** (222 MHz, `SOTC_AB_FLAG=b10switch`, off - on +- 2 SE over blocks of 300 frames): mixR7 **+0.368 +- 0.487 ms** (58
blocks), mixR13 **+0.583 +- 1.039 ms** (11 blocks: it mounted late, from frame 11 128), mixL7 **-0.670 +- 0.830 ms** (13 blocks):
no measurable difference, as expected for a call and two tests a switch.

**89 MB boot with pictures** (bootR, ride7 600 s, all nine switches, shots every 120 s): booted on the first try, field at frame
2 000, **mounted at frame 5 480 and rode to the end** (12 000 frames on the loaded host; 0 dismounts, 0 watchdog lines; 6.03 tops
skipped a frame, 0 records). Pictures at 420 s (galloping across the grass plain, Wander and Agro, cliffs and ruins behind, sky)
and 540 s (riding into the stone colonnade, ground texture, Agro's ground shadow) normal.

**spalign**: `python3 ../tools/spalign.py p2.elf`: 0 sp-based :128/:256 NEON accesses (of 1 064 with a hint). ELF room unchanged
(122 880 bytes).

**Default**: off (`SOTC_B10SWITCH` must be 1-3; it acts only while b9loop is in effect). **Console A/B package**:
`build/agent-scratch/b10/celosia-b10.vpk` (= p2, byte-identical; built from this round's code commit's sources) + `env-ab.txt`
(B9's console env + `SOTC_B10SWITCH=1`, `SOTC_AB_FLAG=b10switch SOTC_AB_PERIOD=300`, `SOTC_B10_LOG=600`). Not installed.

**Unexpected**: in b10chain's first (p1) pass the b1b sub-chain got "Operation not permitted" on vq.out, classify.py and
waitslot.sh for a few minutes (pf7, ldOnA, ldOffA never started); the p2 pass had none. A host-side permission hiccup, not the
game.

## 6. What is left of the PS2 scheduler

With all nine switches: every live thread after GAMECORE runs as host code on a carrier, the boxes are host wait lists, the two
live handlers are host functions delivered without records and in one loop turn, the ready queues are a host bitmap, and every
switch goes from the settle to the pick and the next carrier without a loop top. What EeScheduler still does in guest shape:

- **The thread records keyed by guest id** (`GuestThread`: priorities, PS2 status values, wait states as `EeWaitState`, wakeup /
  suspend counts) and the loops' remaining steps (settle, pick, the dispatch's tests) written as run()'s loop body, three times
  (run(), carrierNest, S17's nest). B10 did not change the records: they are already host structs, but shaped by the PS2 kernel's
  API (ids, KE_ codes, the 128 priorities). The next step would be one host dispatcher function for the three loops and the
  records reduced to what the game's eight live threads use.
- **The EE clock and checkpoints**: guest code still charges cycles; slow checkpoints ~5-6 a frame. Unchanged by B10.
- **The non-host interrupt paths** (none live after GAMECORE), **the semaphore / event-flag records keyed by guest id**, the
  hand-back / resume labels, the bodies as C++ over R5900 registers, the boot threads as generated code, the timer model and the
  IOP's clock: unchanged.
