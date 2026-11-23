# Round B3: the remaining game threads on host carriers, the EE sync calls in place (switch b3threads) (2026-11-22)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B3. Base: main 2eaede2 (A8 on B2). Switch `SOTC_B3THREADS`
(A/B name `b3threads`, sotc_decomp `kB3Threads`, **off by default**; it needs B1's / B2's infrastructure but is switched on its
own). Code: game/overrides/b3_threads.cpp (the four bodies, the check's counts), game/overrides/b3_sync.h (the in-place
kernel calls for typed code), PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (the
table to 8 entries, claim at any loop label, a stack per entry, `carrier*` calls), Kernel/Syscalls/Sync.cpp and Thread.cpp
(the syscalls through `carrier*` on a carrier), ps2_memory.cpp (the VIF1 join's owner check). Scripts, runs and logs:
`build/agent-scratch/b3/` (NOTES.md has every command). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (now pointed at b3;
222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. Which threads, and "the main thread"

The live threads after GAMECORE's entry (docs/decomp_b2.md section 1) and what they are on now:

| t | entry | prio | what | carried by |
|---|---|---:|---|---|
| t4 | 0x1b2ca0 | 2 | the thread system's callback thread: sleeps, then runs one request of the kernel's request slot (a thread to delete, a callback such as iosWakeupThread's, a recover jump), SignalSema | **B3** |
| t5 | 0x1ad270 | 125 | the idle thread: polls the idle queue 0x1fc870 (the hook's idle wait), the CD clock every 1e6 polls | **B3** |
| t8 | 0x1c80a0 | 40 | the VBlank / sound thread | B1 |
| t9 | 0x1c7ae0 | 45 | **the game's main thread**: the frame loop (below) | **B3** |
| t10 | 0x1c38d8 | 42 | the GP thread: its VBlank turn (a blocking send to the GP box), VU0 push / pop, the draw's finish, the frame callback | **B3** |
| t11 | 0x115e4c0 | 43 | the sheet manager (every load / stream job) | B2 |
| t12 | 0x11490f8 | 47 | the memory-card manager (~25 messages a session) | not carried |
| t1 / t2 / t7 | 0x100010 / 0x104ff0 / 0x11b458 | 1 | the ELF's boot thread (the scheduler's kMainThreadId) and two loader-system threads, parked for good | not carried |

The brief named t9 and "the game's main thread" separately; they are the same thread. t1 is the ELF's entry thread (the
scheduler's `kMainThreadId`), which boots the loader system and parks for good; the game's main thread is t9, created by
`bootSchedulerThread` with priority 45, whose loop **is** the frame: the whole game runs below its three frame callbacks.
B3 carries it.

**t9's loop** (0x1c7ba8 .. 0x1c7d90, 120 instructions): isysDeleteObjManager(*0x1e11a0); iStart(); the callback *0x1e1334
(with *0x1e1324 = 1 around it); sndSepTickProc(); the callback *0x1e1330(0); the frame's three callbacks from the table
0x26e9f0 (function, argument) — the game's frame; ++*0x1e1328; a spin while the pause flag *0x26d9a0 is set;
doit_letter_manager(); unless *0x1e1338 / *0x1e132c: the GP display list's transfer waited (iosChkGpDlTransExec), then the
garbage tick, timed, when *0x1e131c is set or the frame is still in its VBlank with less than 2 125 000 ticks used (an
`iosGetTCountExtension` difference through `__floatdisf`, compared with 0x4a01b31f); iFinish(); then either
iosWaitIdleThread() (*0x1e1320) or the blocking send to the frame box 0x26d9a8 (the GP thread takes it), getIntrGpCount(),
iosGPFrameInit(), and the next frame.

## 2. The design

B1 / B2's carrier, extended:

- **Bodies.** Each loop is host C++ over the guest registers in the context, written instruction for instruction from the
  generated function (b3_threads.cpp): the same loads and stores, the same register values (lw / addiu / lui / addu / sll
  sign-extended, andi / ori / daddu / dsubu / dsrl / lbu / ld 64-bit, the generated code's `bltz` on the low word kept), every
  delay slot where the original executes it (likely branches only when taken), the FPU steps of t9's garbage-tick test with
  the generated code's macros (FPU_MOV_S, FPU_ADD_S, FPU_C_OLT_S, fcr31's condition bit), the calls with the same ra
  through the table (DirectCall / IndirectCall: the dispatch checkpoint's 8 EE cycles), the back edges' checkpoints (32
  cycles) at the same back edges. Callees stay the generated code (or whatever their slots hold: hooks, lifted functions,
  natives).
- **Claim at any loop label.** B1 / B2 took a thread over at one pc (just after its sleep). t9 / t10 park deep in callees
  (the frame box send, the GP box send, isys semaphores, DMA waits); after the unwind the dispatcher walks back up by
  dispatching each caller at its return label. So a B3 body starts at **any resume label of its loop** (a `switch` on
  ctx->pc to the label, as the generated function does), and the carrier table takes a list of claim pcs per entry
  (`CarrySpec::claimPcs`). Only the top function's own frame is on the guest stack when it is dispatched at one of its
  labels, so the body has exactly what the generated function would have. The first such dispatch after GAMECORE's entry
  takes the thread over; from then on its parks are in place and it stays carried (1 claim a run in every check run).
- **The table**: 8 entries (`kMaxCarry`), each with its own gate (b3threads on, GAMECORE entered, its bit of
  `SOTC_B3_MASK`), counts (`g_eeB3[t4 t5 t10 t9]`), host stack and budget: 512 KB / 416 KB as B1 / B2 for t4 / t5 / t10; t9
  gets a 1 MB stack with the executor's budget (`PS2X_HOST_STACK_KB`, 384 KB) so its natives keep the executor's room below
  the host stack floor.
- **The game thread's role.** Two places tied "the game thread" to the executor's host thread and had to follow it onto the
  carrier: `PS2Memory::vif1AsyncJoin` applied the asynchronous VIF1 walk only on the thread that started it (now: or any
  host thread that runs guest code for the scheduler, `t_ps2xEeGuestRunner`, set on the executor and every carrier); and
  NOCHK's floor, which the game's endFrame sets (it runs on t9, now on the carrier) and resumeCarrier used to restore to the
  value from before the carrier ran (now kept, with the executor's floor).
- **Everything else is B1 / B2's**: one baton (a kernel semaphore per side on the Vita), the dispatcher makes every
  decision, a carrier yields where the generated code would have unwound (a checkpoint that must reach the scheduler, a
  park, a switch), and anything that still unwinds on it (the switch turned off at a yield, an exception-based switch, the
  host stack's floor) leaves ctx->pc on a resume label and the thread goes on as generated code until its next claim.

## 3. Why gameplay stays exact

The same three arguments as B1 / B2 (docs/decomp_b1.md section 3, docs/decomp_b2.md section 4):

- **The same operations in the same order**: the bodies issue the generated loops' guest-visible operations one for one, with
  the same EE cycle charges at the same points.
- **The same scheduling decisions**: the carrier runs only where the dispatcher would have run the thread's own context and
  hands the baton back exactly where the generated code would have unwound; the dispatcher then does what it does after an
  unwind (events, interrupt handlers as invocations on the executor, the reschedule, other threads). The in-place kernel
  calls (section 5) make the same changes to the same records as the syscalls.
- **Handing back is exact**: at any yield with the switch off the carrier unwinds there with ctx->pc on a resume label of
  the loop or of the callee it was in.

What is not the same, as in B1 / B2: the dispatcher's own charge after a park (after an unwind the generated code goes on
by one dispatch per frame on the way back up, each charging the dispatch checkpoint; on a carrier the host frames are still
there). After GAMECORE's entry EE time follows the host clock, so this is the same order of change as host timing jitter and
changes no operation; that is why every switch takes effect only once GAMECORE runs. It shows in timing-dependent poll
counts: sndBDOpen's `while (Sg2DmaTransStatus(id)) iosWaitIdleThread();` (the sound bank's DMA to the IOP, on t11) polled
1 275 times off and 66 on in the same ride (section 4's counts): each poll is a round of hand-overs through the idle thread,
and the DMA ends after fewer, longer rounds. The kernel message semaphore's blocked waits follow (1 304 off, 118 on).

## 4. Checks

Build p1, Vita3K 222 MHz, muted, `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1` with all three checks
(`SOTC_B1SOUND_CHECK=1 SOTC_B2LOAD_CHECK=1 SOTC_B3_CHECK=1`: the executor audit for every carrier, every loop's kept
registers once a turn, the per-box / per-semaphore counts, the watchdog at 10 s). Six carriers at once (t4 t5 t8 t9 t10 t11):

| run | frames | executor audit | kept registers (6 loops) | B3 turns t4 / t5 / t10 / t9 | hand-backs | B1: ticks = VBlanks, VBlank count = sound ticks | B2 jobs / in-place waits | watchdog |
|---|---:|---|---|---|---|---|---|---|
| ride7 600 s (chkR) | 19 062 | 0 bad of 457 645 | 0 bad | 38 836 / 8 691 / 20 174 / 18 565 | 0 | 30 of 31 windows; 35 250 = 35 250 | 975 / 3 482 | silent |
| attract 400 s (chkA) | 19 588 | 0 bad of 433 887 | 0 bad | 39 081 / 28 329 / 19 866 / 19 165 | 0 | 31 of 32; 23 602 = 23 602 | 504 / 2 113 | silent |
| lake6 400 s (chkL) | 11 899 | 0 bad of 308 941 | 0 bad | 24 766 / 5 337 / 12 710 / 11 965 | 0 | 19 of 20; 24 021 = 24 021 | 431 / 1 910 | silent |

1 claim per carrier per run, 0 bad entries, 0 hand-backs of any kind, no missing target. Every run went to its end: Wander
mounted Agro on ride7 (frame 4 998), lake6's teleport (frame 2 231) and the boss arena, the attract demo past the title and
its area loads (19 588 frames). (The B1 window that differs is the first one, as in B1 / B2.) A frame, riding: t4 2.0
resumes (its parks), t10 2.05 (its parks), t9 2.0 (1.0 at a checkpoint that must reach the scheduler, 1.0 at a park), t5
0.1-0.5 (preempted at its checkpoints). In-place syscalls in chkR: 862 300 WaitSema (558 blocked), 114 336 SleepThread (99 469
blocked), 79 161 WakeupThread (40 286 yielded to the woken thread), 558 SignalSema through the syscall (all yielded: the
woken waiter preempted); 3 631 refused (made off a carrier: the boot before GAMECORE, interrupt handlers).

**The message boxes** (`[b3box]`, chkR; sends / receives, of them on a carrier, sender / receiver threads):

| box | sends | receives | on a carrier | senders | receivers |
|---|---:|---:|---|---|---|
| 0x26e9d0 (GP box) | 20 228 | 35 250 | 20 174 / 35 240 | t9 t10 (and t6 t13-t16 at the boot) | t8 |
| 0x26d9a8 (frame box) | 18 567 | 20 179 | 18 564 / 20 174 | t9 | t10 |
| 0x1fc870 (idle queue) | 67 | 8 695 | 67 / 8 691 | t11 | t5 |
| 0x121af50 | 27 | 28 | 27 / 0 | t9 | t12 |

The uncarried calls are the boot's (before GAMECORE). So after GAMECORE three of the four boxes have carriers at both ends;
0x121af50's receiver is t12.

**The semaphores** (`[b3sema]`, chkR, the syscalls through the stubs; threads as a mask, bit = thread id): every semaphore
used after GAMECORE is used only by carried threads except t12's three (13, 14, 15) and t7's: 4 (t8 only), 12 (t9), 11
(t11), 18 / 19 (t8 t9), 17 (t9 t11), and 6 / 7 / 9 / 10 / 16 (the kernel message semaphore, the isys locks ...) whose only
uncarried users are the boot threads t3 / t6 / t13-t16 before GAMECORE. (The isys lock's uncontended signals go through
the runtime's fast path, not the stub, and are not counted here: section 5's census is of the syscalls.)

**On / off counts** (ldOnR / ldOffR, ride7 360 s, b1sound + b2load on both ways, `SOTC_B3_COUNT`): GP box 11 311 / 10 649
sends, frame box 10 767 / 10 167, idle queue 66 / 1 275 (section 3: sndBDOpen's poll), 0x121af50 25 / 25; lake6 the same
shape (12 157 / 13 333, 11 367 / 12 567, 33 / 1 272, 25 / 27). The frame counts follow the frames run (10 844 / 10 416).

## 5. The in-place kernel calls (typed callers' WaitSema / SignalSema)

`EeScheduler::carrierWaitSema / carrierSignalSema / carrierPollSema / carrierSleep / carrierWakeup` (game/overrides/
b3_sync.h wraps them as `b3sync::waitSema` ... for typed code). Each does to the EE kernel's records exactly what the guest
syscall does — the count, the waiter queue in order (waiters woken first in, first out), wakeup counts, the priority test
that requests the preemption — and where the syscall would leave the thread (a wait that blocks, a sleep with no wakeup
counted, a signal / wake that readied a higher-priority thread) it parks / re-queues the thread exactly as
blockCurrentOrReturn / transferIfRequested do and hands the baton back in place: the dispatcher picks who runs, and the call
returns, with its result, once the thread runs again. v0 is left as the syscall leaves it. The carrier is **pinned** for the
call: the switch turning off does not hand it back in the middle (typed code has no resume label); it is handed back at its
next plain yield.

**Conditions** (each checked at the call; a call that cannot act returns false and changes nothing, and the caller uses the
guest syscall): the calling host thread holds the baton as a carrier for its thread's **own** code (b1sound / b2load /
b3threads on, the thread taken over — after GAMECORE's entry, from its first dispatch at a loop label), not inside an
interrupt handler's invocation (handlers run on the executor), not an interrupt-safe (i*) call. So **a typed caller can
block in place exactly when every guest thread that can reach it is carried**: with b1sound + b2load + b3threads on that is
every thread that runs after GAMECORE except t12 (the memory-card manager) and the parked t1 / t2 / t7. A typed function
reached from t12 or before GAMECORE (the boot) must keep its guest-call fallback.

**Exercised**: with b3threads on, the guest syscalls WaitSema / SignalSema / PollSema / SleepThread / WakeupThread made on a
carrier go through these calls (`g_ps2xB3Sync`; `SOTC_B3_SYNC=0` turns the routing off). The check runs above made 1.94 M
waits, 1 108 signals, 290 958 sleeps and 208 293 wakes through them, 0 audit faults, every run to its end. Round A9 does not
use them yet.

## 6. Loads and hitches

`SOTC_FRAME_LOG_ALL` runs on / off (b1sound + b2load on both ways), tools/loads.py; frames 0-1999 are the boot, the title,
Load Game and the first area, the teleport (an area load) is at frame 2 000:

| run | window | longest frame | over 100 ms | over 200 ms |
|---|---|---:|---:|---:|
| ride7 on | 0-1999 | 463.6 ms (frame 3, the boot) | 13 | 6 |
| ride7 off | 0-1999 | 401.6 (154) | 13 | 6 |
| ride7 on / off | 2000-2599 | 175.3 / 161.3 (2 000) | 1 / 1 | 0 / 0 |
| lake6 on | 0-1999 | 581.6 (3) | 14 | 8 |
| lake6 off | 0-1999 | 410.4 (157) | 14 | 6 |
| lake6 on / off | 2000-2599 | 83.6 / 71.7 | 0 / 0 | 0 / 0 |

The same hitches in the same places (frames 72, 154-157, 185, 411-419, the Load Game pair, 1999-2000); the longest frame
both ways is in the boot, before GAMECORE, where b3threads is not in effect yet (frame 3: host noise with three Vita3K runs
at once). The Load Game pair was longer with b3threads on in these single runs (ride7 258 + 224 ms on vs 205 + 201 off,
lake6 290 + 302 vs 262 + 192) and the teleport 14 / 12 ms longer: no new hitch, but a possible cost at loads (each job's
CD waits now pass through more carriers' hand-overs) that the console must measure.

## 7. Measurements

Vita3K 222 MHz, 3 cores, interleaved A/B of `b3threads` (`SOTC_AB_FLAG=b3threads SOTC_AB_PERIOD=1`, `SOTC_B1SOUND=1
SOTC_B2LOAD=1 SOTC_B3THREADS=1` in the env: B1 / B2 on in both halves), off - on (positive: on faster), 2 SE:

| run | frame | busy | take-overs / hand-backs |
|---|---:|---:|---|
| ride7 800 s seed 7 (mixR7) | -0.085 +- 0.222 | -0.165 +- 0.197 | t9 5 759 / 5 812 by the switch + 11 by exception; t4 / t10 5 759 each |
| ride7 800 s seed 13 (mixR13) | -0.269 +- 0.168 | -0.318 +- 0.174 | t9 5 985 / 6 035 + 24 |
| lake6 400 s (mixL7) | -0.167 +- 0.174 | -0.200 +- 0.173 | t9 3 009 / 3 084 + 6 |
| **pooled** (mean of 3, SE = sqrt(sum se^2) / 3) | **-0.174 +- 0.055 ms (1 SE), z -3.2: on slower** | -0.228 +- 0.052 (z -4.4) | |

**b3threads costs ~0.17 ms a frame in Vita3K.** Where: a carrier turns every yield into two baton hand-overs (a kernel
semaphore signal + wait each way, a host thread switch), and B3 adds ~6.2 of them a frame riding (t4 2.0, t10 2.05, t9 2.0,
t5 0.1) on top of B1 / B2's ~3.7. The old path's park was an unwind and a re-dispatch, which S17 (on by default) had already
made cheap for t9's checkpoints: S17 runs the interrupt handler nested on the game thread's own stack, while a carried t9
hands the baton to the executor and back for it (1.0 a frame). The A/B also pays a take-over and a hand-back of four threads
every frame. It survived the run-time toggling: ~45 000 take-overs and as many hand-backs over the three runs (four threads) (41 of t9's by exception,
inside an in-place wait the switch had turned off), no freeze, no missing target, frames advancing to the end.

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, all three on, ride7 600 s, shots every 120 s; bootR): booted, rode, frames 3 976 /
7 252 / 11 308 / 15 249 / 19 179 at 120 / 240 / 360 / 480 / 600 s (24-36 game fps), system free 67.2 MB at the end, 1 claim
per carrier, 0 hand-backs. Shots at 60 / 180 / 300 / 420 / 540 s looked at: Wander on foot on the plain by the ruins, Wander on
Agro inside the shrine's colonnade, Wander running toward the shrine, then across the plain; ground texture, Wander, Agro,
the body shadows normal, the frame counter advancing. In this run Wander got off Agro at frame 9 206 (`[ride] on foot`),
which none of the 51 other recorded ride7 runs shows. A second 89 MB ride with all three on (bootR2) mounted at frame 3 108 and rode the whole 600 s (18 505 frames, system free 65.0 MB), and the same with b3threads off (bootR0) mounted at 4 646 and rode (18 183 frames, 67.2 MB): the route's timing moves the mount by thousands of frames run to run, and a dismount after a bump is a game outcome of that timing, not a fault the checks see (no audit fault, no kept-register change, no hand-back in that run). Watch item: if a console or Vita3K ride shows it again with b3threads on, trace it with SOTC_POS_LOG. (System free at the end: bootR 67.2 MB, bootR2 65.0 MB, both with the B3 carriers, whose stacks take 2.5 MB: t9 1 MB, three of 512 KB.) **89 MB attract** (bootA, 300 s, shots every 60 s):
past the title into the demo (15 527 frames), the shots (the title's letterboxed rain sky, the demo) normal.

**spalign**: `python3 ../tools/spalign.py p1.elf`: 0 sp-based :128/:256 NEON accesses (of 1 038 with a hint).

**Default.** Off (`SOTC_B3THREADS` must be 1-3): exact by the checks, but 0.17 ms slower in Vita3K. Console A/B package:
`build/agent-scratch/b3/celosia-b3.vpk` (= p1) + `env-ab.txt` (N6's console env without its A/B lines + `SOTC_B1SOUND=1
SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_AB_FLAG=b3threads SOTC_AB_PERIOD=300 SOTC_B1_LOG=600 SOTC_B2_LOG=600 SOTC_B3_LOG=600`).
It needs B1's audio heard and B2's loads measured first (both on in both halves).

## 8. What is still open

- **Message boxes as host queues: not done.** Three of the four boxes now have carriers at both ends after GAMECORE (the GP
  box, the frame box, the idle queue; 0x121af50 needs t12). What a host queue needs, from iosSendMsg / iosRecvMsg's code:
  the box lives in guest memory ({+0 slots, +4 capacity, +8 count, +0xc write index, +0x14 the waiting receiver, +0x18 the
  waiting senders linked through the ios thread records' +0x28}) and is read by the runtime's idle turn (sotc_decomp::
  idleTurn, which also checks the kernel message semaphore 0x1e0be8); every send / receive takes that semaphore (shared by all
  boxes); and a receive does not wake a sender itself: it queues `iosCallBackThreadMgr(0x1b1328, thread)` to t4, which runs
  at priority 2 and wakes it. An exact host queue keeps the order of messages trivially but must keep the wake order and the
  priority decisions of that hop (t4's turn, its request slot, its SignalSema) or prove them invisible; and the semaphore and
  the senders list must stay consistent for idleTurn. That is a round of its own (iosSendMsg / iosRecvMsg / iosWakeupThread
  as host code over the box first, then the box itself).
- **EE semaphores as host primitives**: the in-place calls keep the kernel's records; a semaphore whose every user is
  carried (section 4: all but t12's / t7's after GAMECORE) could drop its record only once the boot (which uses the same
  semaphores before GAMECORE) runs without the dispatcher's charges mattering, and once the isys fast path and the
  interrupt handlers' iSignalSema use the same object.
- **The cost**: a checkpoint that must reach the scheduler on a carrier is two host thread switches. The next step is to
  deliver interrupts on the carrier itself (S17's nested dispatch, run on the carrier's stack while it holds the baton) so
  only real thread switches hand the baton over; and the idle thread could wait in place without the executor's round trip.
- **Checkpoints as pure delivery points.** With every live thread carried a checkpoint never unwinds any more except to
  hand a thread back. To make the generated `PS2_CHECKPOINT*` pure delivery points: (1) no hand-back (the switches fixed on,
  no A/B, no host-stack floor unwind: the carriers' stacks sized for the deepest chain), so no resume label is ever needed;
  (2) interrupts / handlers delivered on the running carrier (above), so a checkpoint is "run what is pending, then go on";
  (3) t12 and the boot carried or the boot made independent of dispatch charges (the module placement, docs/decomp_b2.md
  section 8), so no generated thread is left that needs the unwinding path; (4) with n5sched (no time slice) and the host
  clock after GAMECORE, the cycle charges only pace events: the checkpoint becomes a flag test (pending event or preemption),
  and its charge can move to the dispatch points that remain. Then the checkpoints can leave the generators (phase B's
  "done when").
- **t12 and the boot threads** stay generated code (t12 runs ~25 times a session, in the save paths: carry it with the save /
  load tests the plan calls for).
