# Round B8: the dispatcher's deliveries without invocation records, the gate re-set at timer reads (switch b8sched) (2026-11-23)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B8. Base: main 0394454 (B7 on A12). Switch `SOTC_B8SCHED` (A/B
name `b8sched`, sotc_decomp `kB8Sched`, **off by default**; it takes effect only while b7irq is in effect, so with b3threads and
the other carrier switches once GAMECORE runs). Code: PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp +
include/runtime/ee_scheduler.h (`b8Push`, `b8Dispatch`, `b8Materialize`, the hooks in run() / carrierNest / s17Nest, `b8Step` /
`b8ShadowPush` / `b8ShadowRecord` (the check), `b8Regate` / `b8Compare`, `gateNow` split out of `updateGate`, `EeB8Stats`),
ps2_memory.cpp / .h (the regate hook after a timer read's flush), game/overrides/b8_sched.cpp (the switch, the `[b8sched]` line),
b7_irq.cpp (each host handler's charge formula for the check), sotc_native.cpp / sotc_decomp.h (install, frame call, the switch
tables). Scripts, runs and logs: `build/agent-scratch/b8/` (NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed
at b8; 222 MHz, 3 cores, audio-volume 0; every run muted).

**State of this document**: the round is paused twice for the user's meetings (the second time all Vita3K runs were stopped,
22:2x, before the boot series and the A/B finished). The code, the checks and the first boots are done; the rest of the run plan
is queued as `build/agent-scratch/b8/b8chain3.sh p2.vpk` (not started). Items marked **pending** below wait for it.

## 1. What B7 left in record form

With b7irq the two live handlers (VBlank start 0x1c8180, GS 0x1c37f0) are host functions, but each delivery still went through the
dispatcher's record form, in all three places it runs (run(), B4's carrierNest, S17's nest):

1. the queued interrupt became a `GuestInvocation` (a whole guest context) pushed on the thread the dispatcher picked;
2. at that thread's next dispatch the generated dispatch path looked the record up, B7 searched its host-handler table for the pc,
   the host function ran on the record's context, then the events and the reschedule;
3. at the dispatch after, the completed record was popped (the VU0 owner's flush test) and the loop went round again.

And on a carrier the slow checkpoint was still reached ~4 times a frame only because a timer register read (PS2Memory's timer
sync) had closed the EE clock's gate: B7's "below the slice" count.

## 2. The design

**(1) Deliveries without records.** `EeScheduler::b8Push`: when the queued invocation is an interrupt whose pc has a host form
(B7's `EeHostIrq`), has no completion, and the owner thread has no record of its own, the invocation moves into the thread's
slot (`m_b8Slot[id]`, `GuestThread::b8State` 1) instead of its record vector. The thread's next dispatch (`b8Dispatch`, state 1)
does the record's dispatch steps itself, in the record's order: the dispatch checkpoint (a checkpoint due: the loop goes round, as
before), the interrupt flag, the gate, the host function as a plain call on the slot's context, the events, the reschedule. The
dispatch after that (state 2) is the pop alone: the VU0 owner's flush test, then the loop goes round as after a pop. Every step
stays at its place in the loop, so the picks, thread records, ready queues and EE cycles are the record form's; only the record
itself, the generated-dispatch lookup and B7's record search are gone. The slot is the record for every reader that matters (the
loops' "has an invocation" tests, the snapshot depth, a thread made dormant clears it). Any path the host form does not cover (B7
off, the GS callback refused, a completion to run on the record, a check-mode generated run that unwound) turns the slot back
into the record it stands for (`b8Materialize`) and the record path goes on from the same point.

**(2) The gate at timer reads.** After a timer register read's sync and flush (which closes the gate), PS2Memory calls the
scheduler's regate hook; on a carrier, with B7's checkpoint part, `b8Regate` sets the same absolute gate in place
(`gateNow`, split out of `updateGate`), and the timers' / IOP's catch-up is left to the next checkpoint that delivers something
or the next park (every reader of either syncs first). `SOTC_B8_REGATE=0` turns part (2) off.

## 3. Why it stays exact

- Part (1) changes where the invocation lives, not which steps the dispatcher takes or when: the same push point, the same
  checkpoint charge at the dispatch, the same run, the same pop dispatch. The check (section 4) runs the record form's shadow
  next to it and compares every dispatcher step.
- The host function's charges (B7: 8 cycles a JAL / JALR on the generated code's path) are recomputed by a formula per handler
  (b7_irq.cpp) and compared with the run's actual charge.
- Part (2) sets the gate to the value the next slow checkpoint would set; the check runs the old path and compares the gate it
  predicted with what the next slow checkpoint does (`b8Compare`).

## 4. Checks

`SOTC_B8_CHECK=1` with every earlier check on (B1 sound, B2 load, B3 audit, B6 shadow, B7 handlers), all seven switches on
(b1sound b2load b3threads b4fast b6sync b7irq b8sched), p2 build:

| run | B8 dispatcher steps (bad) | B8 host runs' charges (bad) | B8 regate predictions compared (bad) | B7 handler checks (bad) | B6 shadow checks (bad) | B3 audits (bad) |
|---|---:|---:|---:|---:|---:|---:|
| ride7 600 s (chkR) | 262 086 (0) | 52 765 (0) | 53 711 (0) | 52 765 (0) | 5 359 280 (0) | 219 911 (0) |
| attract 400 s (chkA) | 247 107 (0) | 43 045 (0) | 58 272 (0) | 43 045 (0) | 4 185 160 (0) | 262 600 (0) |
| lake6 400 s (chkL) | 172 502 (0) | 34 576 (0) | 35 289 (0) | 34 576 (0) | 3 475 670 (0) | 140 192 (0) |

**0 bad of 681 695 B8 step comparisons, 130 386 charge comparisons and 147 272 regate predictions**; B6 / B7 / B3 checks still 0.
Records materialized (the fallback): 0 in every run. ("Slow anyway" 19-22 a run: predictions not compared because the slow path
ran for another reason first; not mismatches.) The first build p1 had two check-only bugs (the B7-check charges wrong in the
shadow, no initial shadow sync), seen in sm1A and fixed in p2; the switch's own path did not change between p1 and p2.

## 5. Counts, boots, loads, A/B, spalign, console package

**Counts** (ride7 240 s, p1, all carrier switches + b7irq; sm1Off without b8sched, sm1On with; per frame riding):

| | b8sched off | b8sched on |
|---|---:|---:|
| interrupt deliveries | 3 a frame, as invocation records | 2.6-3.3 a frame in slots (records made: 0), host runs = pops = slots |
| gates re-set in place at a timer read | 0 | 3.08 |
| slow checkpoints on a carrier: pending / deadline / below the slice | 2.07 / 0.11 / **3.91** (6.1) | 2.06 / 0.09 / **0.81** (3.0) |
| slow checkpoints elsewhere | 2.23 | 2.22 |
| EE cycles charged by guest code | 172 k | 163 k (the same order: the route differs) |

So on a carrier the slow path drops from ~6.1 to ~3.0 a frame (B7: 13.6 -> 6.7; with B8 about 22 % of the pre-B7 count): what is
left is the deliveries themselves (~2.1) and ~0.3-0.9 below-slice crossings that are not timer reads. The boots below show the
same (0.28-0.90 below the slice, 3.06-3.10 regates a frame). Each removed slow checkpoint and each removed record is
microseconds a frame on the console: the expected gain is again below what Vita3K's A/B resolves.

**Boots** (89 MB, `SOTC_VITA_EAT_USER_MB=89`, ride7 300 s, all seven switches on): **3 done, 3 reached the field (frame 2 000)
and 3 mounted Agro and rode**: pf5 mounted at frame 2 880 (9 000 frames in 300 s), pf1 at 3 237 (7 800), pf6 at 2 223 (7 800);
0 dismounts, 0 watchdog lines (b1dog / b5dog), slots 2.7-3.3 a frame, 0 records made. pf7 and pf8 were killed mid-run by the
stop; **pending**: pf2-pf4, pf7-pf10 (b8chain3.sh) for ≥10.

**Loads** (on vs off, `SOTC_FRAME_LOG=1`; ldOn/ldOff R / A / L): **pending** (b8chain3.sh).

**Vita3K A/B** (222 MHz, `SOTC_AB_FLAG=b8sched`, one at a time on b1: mixR7 / mixR13 / mixL7, pooled off - on +- SE):
**pending** (b8ab.sh, run at the end of b8chain3.sh).

**89 MB boot with pictures** (bootR, ride7 600 s, shots every 120 s): **pending** (b8chain3.sh; the shots may fail while
windows are hidden: the picture check is the coordinator's).

**spalign**: `python3 ../tools/spalign.py p2.elf`: 0 sp-based :128/:256 NEON accesses (of 1 058 with a hint).

**Default**: off (`SOTC_B8SCHED` must be 1-3; it acts only while b7irq is in effect). **Console A/B package**:
`build/agent-scratch/b8/celosia-b8.vpk` (= p2, byte-identical; built from this commit's sources) + `env-ab.txt` (N6's console env
without its A/B lines + `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_B6SYNC=1 SOTC_B7IRQ=1 SOTC_B8SCHED=1
SOTC_AB_FLAG=b8sched SOTC_AB_PERIOD=300` and the B1-B8 log lines). Not installed.

## 6. What is left of the PS2 scheduler

With all seven switches: every live thread after GAMECORE runs as host code on a carrier, the boxes are host wait lists, the two
live handlers are host functions delivered without invocation records, and a timer read no longer costs a slow checkpoint.
What EeScheduler still does in guest shape:

- **The thread records and run()'s decisions** (priorities, ready queues, wait states, wakeup counts; every switch decided by
  run()'s loop body, on the executor or in carrierNest), one baton between carriers. B8 kept the dispatcher's steps exactly
  (the slot replaces the record, not the loop): the loop itself is the next thing to make host-shaped, i.e. a delivery that
  runs at the delivery point without the two extra loop turns (push -> run -> pop). That changes the dispatcher's visible order
  (the events and reschedule between them) and needs its own exactness argument.
- **The EE clock and checkpoints**: guest code still charges cycles (8 a call, 32 a back edge; ~160-250 k cycles a frame riding)
  and the clock decides when a deadline is seen between parks. On a carrier the slow path is now ~2 deliveries + <1 other a frame.
  The charges cannot go while EE time between parks is the charged cycles.
- **The non-host interrupt paths**: any interrupt / alarm / callback without a host form still uses records (none is live after
  GAMECORE: 0 materialized in every run), and the boot-time interrupts before GAMECORE.
- **The semaphore / event-flag records keyed by guest id**, the hand-back / resume labels, the bodies as C++ over R5900
  registers, the boot threads (t1 / t2 / t3 / t6 / t7 / t13-t16) as generated code: unchanged.
- The timer model in PS2Memory and the IOP's clock (kept for the readers' syncs).
