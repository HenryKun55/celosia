# Round B7: the interrupt handlers as host functions, the carriers' checkpoints reduced to delivery points (switch b7irq) (2026-11-23)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B7. Base: main 80ccbd2 (B6 on A11). Switch `SOTC_B7IRQ` (A/B name
`b7irq`, sotc_decomp `kB7Irq`, **off by default**; it takes effect once GAMECORE runs and only with b3threads on, so it needs the
carrier switches, but toggles on its own). Code: game/overrides/b7_irq.cpp (the host handlers, the switch, the `[b7irq]` line),
PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (`EeHostIrq`, `addHostIrq`, `b7Find` /
`b7RunIrq` / `b7CheckIrq`, the hook in B1_INVOKE at run() / S17's nest / B4's carrierNest, the gate part in `updateGate`, the
counts `EeB7Stats`), sotc_native.cpp (install, the frame call, the switch name). Scripts, runs and logs: `build/agent-scratch/b7/`
(NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed at b7; 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. Which interrupts are live after GAMECORE

From the `[irq] add` / `[irq] dispatch` lines and B7's counts of queued invocations by kind (`[b7irq]`, every run):

| cause | handler | fires after GAMECORE | what it is |
|---|---|---|---|
| INTC 2 (VBlank start) | 0x1c8180 | ~2-3 a frame (one a VBlank) | wakes the VBlank thread t8 |
| INTC 0 (GS) | 0x1c37f0 | 1 a frame (the FINISH event) | IMR / CSR acknowledge + GAMECORE's GP finish callback |
| INTC 3 (VBlank end) | none registered | dispatched, nothing runs | |
| INTC 9 (T0) | 0x1ad850 (sysnative's tcountOverflow) | never (T0 stopped by sysnative) | |
| INTC 11 (T2) | registered at boot | never (T2 stopped by sysnative) | |
| DMAC 1 / 2 / 5 | none registered | dispatched, nothing runs | |
| alarms, GS VSync callback, RPC / SIF / other invocations | | **0** queued after GAMECORE | |

So two handlers are live, and both now have host forms.

The two handlers, from the lifted code (decomp_liftd6_00.cpp, decomp_liftd4_00.cpp) and the generated isceGsPutIMR (0x1e05b8) /
GP finish callback (0x1345fb0, set by initgc 0x1345b70 through iosSetGpFinishFunc 0x1c3730):

    0x1c8180 (VBlank start):  iosiWakeupThread(*0x26d9c4) -> iWakeupThread(record +4)    (t8)
                              ei; return 0
    0x1c37f0 (GS):            s0 = isceGsPutIMR(0xff00)      (old = iGsGetIMR(); iGsPutIMR(0xff00); return old)
                              ++*(u64 *)0x1e1158
                              if (*(u32 *)0x1e116c) (*0x1e116c)()    (0x1345fb0 from the title on)
                              *(u64 *)0x12001000 |= 2          (GS CSR: the FINISH event acknowledged)
                              isceGsPutIMR(s0); ei; return 0
    0x1345fb0 (GP finish):    *(u64 *)0x1465670 = *(u64 *)0x1465a00
                              *(u64 *)0x1465678 = iosGetTCountExtension()   (sysnative: the EE clock's bus ticks)
                              ++*(u64 *)0x1465648

## 2. The design

**Where a handler runs today.** An interrupt is queued by the dispatcher (`dispatchIrq`: a `GuestInvocation` with the handler's
pc, a0 = cause, a1 = its argument, ra = 0). At the dispatcher's next step the invocation is pushed onto the thread it is running
(or picks), the dispatch checkpoint is charged, the shared VU0 registers are handed over to the invocation's context, and the
handler's generated code runs (`B1_INVOKE`). On return (pc 0) the dispatcher processes events and, if the handler readied a
higher-priority thread, re-queues the interrupted thread at the front; at that thread's next dispatch the invocation is popped
(VU0 flushed back). With b4fast this all happens in `carrierNest`, on the host thread of the carrier that holds the baton at the
delivery point (a checkpoint yield or a park); else in run() on the executor or in S17's nest.

**B7.** At exactly that point (`B1_INVOKE`, all three dispatchers), when the invocation on top of the running thread is an
interrupt at its entry (pc = the handler, ra still 0: not a handler resumed after an unwind) and the handler has a host form
(`EeHostIrq`, registered by b7_irq.cpp), the host function runs instead of the generated code. Nothing else changes: the same
invocation record is pushed, charged, popped; the same event processing and reschedule follow; the next pick is made by the same
code from the same records. The host forms (b7_irq.cpp):

- do the same operations in the same order: guest memory through the same READ / WRITE paths (the GS CSR through PS2Memory's
  GS register path, its write-one-to-clear semantics), iosGetTCountExtension through its slot (on the Vita sysnative's EE-clock
  count);
- make the i* calls straight to the kernel records: `EeScheduler::wakeupThread(id, true)` (what iWakeupThread's syscall does;
  its `transferIfRequested(true)` returns at once inside a handler), the GS IMR read / written as GsGetIMR / GsPutIMR do;
- charge the EE cycles the generated code charges on the same path: 8 a JAL / JALR (callGuestKnown's dispatch checkpoint; the
  returns charge nothing): VBlank 8, GS 48, GS with the GP finish callback 64 (the check found 0 cycle differences);
- leave out: the VU0 hand-over and the pop's flush around the invocation (no handler touches a VU0 register, so they move the
  same values back and forth), the handlers' frames on the invocation stack (written, never read), the invocation context's
  scratch registers (discarded at the pop).
- A handler whose charges cross the EE clock's gate runs whole; the generated code would have unwound at that JAL and finished
  the handler after the dispatcher's next turn (events / deadlines processed in between). On the console a handler runs with
  interrupts off, whole: the host form is the console's order; the crossing is counted (`crossed`, 0.000 a frame outside check
  mode) and seen at the next checkpoint.
- A GS run with another callback than 0x1345fb0 takes the generated code (`accept`; 0 such runs).

**Checkpoints (part 1, `SOTC_B7_CP`, on with b7irq).** With all live threads on carriers, what a generated checkpoint does on a
carrier is: charge cycles inline (`run += n; run < limit`), and past the gate (`checkpointSlow`) one of: deliver (a posted event /
the pending flag, a deadline due: yield to the dispatcher's loop, which runs the events and handlers), preempt (the time slice:
never after GAMECORE, n5sched makes the slice endless and the live priorities are distinct), or only re-sync the accounting and
the gate ("below the slice": the EE timers' budget, the IOP batch, a gate closed by a timer read). The IOP batch is the one term
with nothing to deliver: the native IOP's CPU is halted after its boot (IopEmulator::runCycles only advances its clock; the
sound mixer is in pull mode on the Vita), and every reader of the IOP (an RPC, a module call, the SIF) brings it up to date
first (`PS2Runtime::catchUpIopNow`: syncAccounting, then catchUpIop). So on a carrier, under b7irq, `updateGate` leaves the IOP
batch out of the gate: the cycles reach the IOP all the same (the same total, in fewer batches) at the next slow checkpoint, park
or dispatch. The EE timers' term stays (it is the maximum anyway: sysnative stopped T0 / T2, no timer interrupt is set up), the
deadline (VBlank) and the pending flag stay, the cycle charges stay (the EE clock decides when a deadline is due).

What could not be removed, and why:

- The cycle charges: the EE clock between parks is the charged cycles; a deadline due by cycles is delivered mid-frame from them.
  Dropping them would move when a VBlank is seen (an ordering change, not a delivery-point reproduction).
- Pending / deadline slow checkpoints (~2.2 a frame on carriers): they are the deliveries themselves.
- "Below the slice" after a gate closed by a timer read (~4 a frame): the game reads the EE clock (iosGetTCountExtension /
  sysnative, GetTimerSystemTime) and PS2Memory's timer sync closes the gate (`flushEeTimers`); the next checkpoint only re-sets it.
  Cheap, and removing it needs the timer model out of PS2Memory.

## 3. Why it stays exact

(The check in section 4 compares all of this after both forms.)

- **Same invocation, same dispatcher.** The host function replaces only the generated code's run, at the point where it would
  start; the push, the dispatch charge, the event processing, the reschedule and the pop are the dispatcher's own code, unchanged.
  The pick that follows is made from the records the handler left: B7's check compares them after both forms.
- **Same effects.** VBlank: one `wakeupThread(t8, true)` (a sleeping t8 readied, else its wakeup counted; a self-wake when the
  invocation sits on t8 itself, as PS2X_SELF_WAKE does), the preemption flag when t8 outranks the interrupted thread. GS: IMR to
  0xff00 and back, the counter, the GP finish stamp (the previous clock, the EE-clock count at the same charged cycle, the count),
  the CSR's FINISH event acknowledged through the same register path.
- **Same EE time.** The same charges at the same points relative to the reads of the clock (the stamp reads the clock after the
  callback's two charges, as the guest path does).
- **The checkpoint part** changes only when the IOP's clock is advanced between delivery points, and nothing reads the IOP without
  advancing it first; the EE timers, the deadlines, the pending flag and the slice still set the gate.

## 4. Checks

`SOTC_B7_CHECK=1`: each host handler run becomes a shadow (`EeScheduler::b7CheckIrq`). The generated handler runs first, for real
(so the game goes on exactly as without b7irq); then the state from before it is put back, the host form runs on a copy of the
invocation's context, what it left is recorded, and the guest path's result is put back. Compared: every thread's status,
wakeup count, wait reason, suspend count, pending resume completion, v0 and invocation depth; the 128 ready queues; the
reschedule / time-slice-expired / slice flags; the next pick (front of the highest non-empty queue); the GS IMR; the CSR's event
bits; the handler's guest doublewords (GS: the counter 0x1e1158 and the GP finish stamp's three words); the EE cycles charged;
the jump serial; that the host form returned (pc = ra = 0). One allowance: the T0 stamp (0x1465678) is read by the shadow after
the guest run and its own charges, so it must be later by at most those cycles' bus ticks (counted: `timer stamps read later`).
Guest runs that unwound inside (a checkpoint went to the scheduler mid-handler) are not compared (`unwound`).

Build p5, Vita3K 222 MHz, muted, all six switches (`SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_B6SYNC=1
SOTC_B7IRQ=1`) with every check (`SOTC_B1SOUND_CHECK SOTC_B2LOAD_CHECK SOTC_B3_CHECK SOTC_B6_CHECK SOTC_B7_CHECK`, the B1 watchdog,
`SOTC_B5_DOG=10`) and the ride probe:

| run | frames | B7 checks (bad, unwound, cycle diff., stamps later) | B6 shadow checks | executor audit | watchdogs | outcome |
|---|---:|---|---|---|---|---|
| ride7 600 s (chkR) | 10 800 | **40 798 (0, 9, 0, 10 728)** | 0 bad of 3 667 013 | 0 bad of 146 613 | silent | field 2 000, mounted 3 664, 0 dismounts |
| attract 400 s (chkA) | 9 445 | **26 842 (0, 5, 0, 8 923)** | 0 bad of 1 823 430 | 0 bad of 116 691 | silent | past the title, the demo |
| lake6 400 s (chkL) | 5 400 | **23 614 (0, 4, 0, 5 327)** | 0 bad of 1 725 816 | 0 bad of 133 556 | silent | field 2 000, the teleport, the boss arena |

**0 bad of 91 254 handler comparisons** (VBlank start and GS with the GP finish callback; 0 cycle differences: the charges are the
generated code's), 0 of 7.2 M B6 shadow checks, 0 of 0.40 M audits, 0 hand-backs, 0 kept-register faults, 0 `[b1dog]` / `[b5dog]`
lines. (Frame counts are low: the host ran at load 20-50, other jobs, including a 12-thread test process of another project.)

Development finds, fixed before p5 (each caught by the check): the GS handler's GP finish callback is set from the title on (the
first form covered only "no callback"); iosGetTCountExtension is sysnative's EE-clock count on the Vita, not the T0 register (the
first form read T0: 0); the callback path's ra must be restored (the first form returned to 0x1345fd4); and the shadow must run
after the guest path (run first, its wake closed the gate and the guest path unwound at its first JAL: an artifact of the check).

## 5. Counts, boots, loads, A/B, 89 MB boot, spalign, console package

**Counts before / after** (`[b7irq]`, ride7 240 s on the same build p1, all carrier switches + b6sync; sm1Off: b7irq off,
sm1On: on; per frame riding):

| | b7irq off | b7irq on |
|---|---:|---:|
| handler invocations (VBlank start / GS) | 2.0-2.9 / 1.0, generated code | the same, host functions (2.06-2.65 / 1.00 host, 0 generated) |
| slow checkpoints on a carrier: pending / deadline / below the slice / preempt / renew | 2.16 / 0.11 / **11.34** / 0 / 0 | 2.3 / 0.12 / **4.3** / 0 / 0 |
| slow checkpoints elsewhere (the executor, the nests) | 2.14 (below the slice) | 2.3-2.5 |
| carrier gates set by: timer / IOP / deadline / slice | 0 / 14.5 / 2.3 / 0 | 0 / 0 / 10.1-10.8 / 0 |
| IOP batches run (s11's iop runs) | 12.2 | 7.0 |
| all slow checkpoints (s11) | 15.8 | 9.2 |
| EE cycles charged by guest code | 155-210 k (~6.5-26 k inline checkpoints, 8 / 32 cycles each) | the same |
| EE cycles from the host clock's catch-up | 9-13 M | the same |

So on a carrier the slow path drops from ~13.6 to ~6.7 a frame: what is left is the deliveries (~2.3) and the gate re-sets after
a timer read (~4). Each removed one was syncAccounting + the IOP clock + updateGate: about a microsecond on the console, so the
expected gain is a few microseconds a frame, below what Vita3K's A/B can resolve. The handler side saves, per invocation, the
VU0 hand-over and flush (two copies of the VU0 block), the generated code's call chain and the syscall layer (handleSyscall,
the binding call): again microseconds a frame.

**Boots** (89 MB, ride7 300 s, all six switches on, three trees; pf1-pf10 in b7chain.sh at host load 20-50 with A12's runs,
pf4 and pf11-pf15 in b7chain2.sh after the 12:25 stop): **15 boots, 14 reached the field (frame 2 000), 10 of them mounted Agro
and rode** (pf1 3 546, pf2 3 331, pf5 3 402, pf7 2 297, pf9 3 792, pf11 2 831, pf12 4 806, pf13 2 393, pf14 4 894, pf15 3 718),
0 dismounts, 0 watchdog lines, system free 65.0-67.2 MB. Four reached the field and did not mount within 300 s (pf3, pf4, pf6,
pf10: the closed-loop steer did not bring Wander to Agro; the route's mount is timing-sensitive, B4 section 1, and B6's ldOnR
also rode 7 200 frames unmounted). One did not reach the field: **pf8, Vita3K closed the game at frame ~1 360 (t = 32 s, the
Load Game screen) after "Unhandled write protected region" access violations** (vita3k.log; err.txt simply stops). The same
violations, and runs dying the same way, appear in A12's runs of the same afternoon (a12/runs/d1, chkA: no b7irq there); I
could not tie it to B7, but it is one crash in 15 boots and is reported as such. With the checks, smoke and load runs, b7irq ran
in ~40 boots; this was the only one that died.

**Loads** (`SOTC_FRAME_LOG=1 SOTC_FRAME_LOG_ALL=1`; on = all six, off = without b7irq; lg.py):

| run | Load Game pair | 200-1999 longest, over 100 / 200 ms | teleport load |
|---|---|---|---|
| attract 300 s on / off (ldOnA / ldOffA, load 8-26) | 21 + 27 / 20 + 26 ms | 150, 2 / 0 / 156, 2 / 0 | (no load) |
| ride7 360 s on / off (ldOnR2 / ldOffR2, one at a time, load ~8-10) | 155 + 200 / 180 + 194 ms | 200, 7 / 1 / 306, 13 / 3 | 166 / 306 ms |
| lake6 400 s on / off (ldOnL2 / ldOffL2, one at a time) | 150 + 194 / 156 + 187 ms | 326 (the teleport), 5 / 1 / 372, 8 / 2 | 326 / 372 ms |

The first ride7 / lake6 pairs (b7chain2, three Vita3K at once beside A12's, load 20-36) had 300-420 frames over 100 ms on both
sides (ride7 on 391 / off 419, lake6 on 307 / off 414): host noise, not used. No new hitch with b7irq; the same hitches in the same
places.

**Vita3K A/B** (222 MHz, `SOTC_AB_FLAG=b7irq SOTC_AB_PERIOD=1`, all six switches in the env), off - on (positive: on faster), 2 SE:

| run | first set (side by side, load 20-36) | second set (one at a time on b1) |
|---|---:|---:|
| ride7 800 s seed 7 | -0.894 +- 1.633 (26 blocks) | +0.079 +- 0.264 (64 blocks) |
| ride7 800 s seed 13 | -0.199 +- 3.454 (6 blocks: mounted late) | -0.010 +- 0.317 (52 blocks) |
| lake6 400 s | +0.159 +- 0.617 (23 blocks) | -0.334 +- 0.747 (27 blocks) |
| **pooled** (mean of 3, SE = sqrt(sum se^2) / 3, 1 SE) | -0.31 +- 0.65 | **-0.09 +- 0.14 ms (z -0.6)** |

No measurable difference, as the counts predict (microseconds a frame). Host load during the second set: median 12.8 / 8.2 /
12.6 (max 27 / 14 / 21), with A12's Vita3K runs and B7's own load reruns beside it part of the time.

**89 MB boot** (bootR: `SOTC_VITA_EAT_USER_MB=89`, all six on, ride7 600 s, shots every 120 s): field 2 000, mounted 3 621, rode to
the end, frames 3 663 / 5 678 / 6 824 / 10 192 / 13 712 at 120 / 240 / 360 / 480 / 600 s (the slow middle: host load), system free
67.2 MB, 0 dismounts, handlers host-run to the end (VBlank 2.06, GS 1.00 a frame, 0 generated). Shots at 60 / 180 / 300 / 420 /
540 s landed (the Mac was not locked) and I looked at them: at 300 s Wander on Agro galloping along the stone wall (game 9 fps
then); at 540 s Wander on Agro at a gallop past the shrine's arches, the ground textured, Agro's body shadow on the grass, the
overlay at game 31 / host 32 fps. Wander, Agro, ground, shadows normal; the frame counter advancing.

**spalign**: `python3 ../tools/spalign.py p5.elf`: 0 sp-based :128/:256 NEON accesses (of 1 058 with a hint).

**Default**: off (`SOTC_B7IRQ` must be 1-3; and it acts only with b3threads on, once GAMECORE runs). **Console A/B package**:
`build/agent-scratch/b7/celosia-b7.vpk` (= p5) + `env-ab.txt` (N6's console env without its A/B lines + `SOTC_B1SOUND=1
SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_B6SYNC=1 SOTC_B7IRQ=1 SOTC_AB_FLAG=b7irq SOTC_AB_PERIOD=300 SOTC_B1_LOG=600
SOTC_B2_LOG=600 SOTC_B3_LOG=600 SOTC_B6_LOG=600 SOTC_B7_LOG=600`). Not installed.

## 6. What is left of the PS2 scheduler

With b1sound + b2load + b3threads + b4fast + b6sync + b7irq: every live thread after GAMECORE runs as host code on a carrier, the
boxes are host wait lists, and the two live interrupt handlers are host functions. What EeScheduler still does in guest shape:

- **The thread records and run()'s decisions** (priorities, ready queues, wait states, wakeup counts; every switch decided by
  run()'s loop body, on the executor or in carrierNest), one baton between carriers. Unchanged by B7.
- **The interrupt plumbing around the handlers**: an interrupt is still a `GuestInvocation` record pushed on the thread the
  dispatcher picks, charged, popped at its next dispatch; only the run is host code. Making the delivery a plain host call at
  the delivery point (no record, no push / pop) changes the dispatcher's own steps around it (the pop's dispatch, the order of
  the reschedule against the pop), so it needs the dispatcher itself in host form first.
- **The EE clock and checkpoints**: guest code still charges cycles (8 a call, 32 a back edge: ~170-250 k cycles a frame riding,
  so ~10-25 k inline checkpoints a frame) and the clock still decides when a deadline is seen between parks. On a carrier the
  slow path is now reached only to deliver (~2.2 a frame) or after a timer read closed the gate (~4 a frame). The charges cannot
  go while EE time between parks is the charged cycles (section 2).
- **The semaphore / event-flag records keyed by guest id** (step 3 of the plan not done in B7: the boot threads, t7 and the
  handlers' syscalls still use the guest ids, and B6's host calls already go straight to the records; a separate host object
  for the carrier-only semaphores 4 / 11 / 12 / 13 / 17 / 18 / 19 would duplicate the record that ReferSemaStatus reads, for no
  gain over B6's direct calls).
- **Hand-back / resume labels**, the bodies as C++ over R5900 registers, and the boot threads (t1 / t2 / t3 / t6 / t7 /
  t13-t16) as generated code: unchanged.
- The timer model in PS2Memory (the gate closures on timer reads) and the IOP's clock (kept only for the readers' syncs).
