# Round B1: the VBlank / sound thread on a host thread (switch b1sound) (2026-11-22)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B1. Base: main f643b44 (P3); round B1b (the freeze fix, section 7) on main 3c663b6 (P4). Switch `SOTC_B1SOUND` (A/B
name `b1sound`, sotc_decomp `kB1Sound`, **off by default**; the freeze and its fix, round B1b: section 7). Code: game/overrides/b1_sound.cpp (the thread's body and the host
tick functions), PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (the host carrier
of a guest thread). Scripts, runs and logs: `build/agent-scratch/b1/` (NOTES.md has every command). Vita3K trees
../vita3k-celosia-b1 / -b1b / -b1c (copies of d17, 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. What the guest thread does

There is no separate "sound thread" in SotC's EE code: the SG2 engine's per-tick work runs on the game's VBlank thread,
which also reads the pads and paces the GP thread. It is the scheduler's t8 in docs/native_n5.md.

- **Creation.** `bootSchedulerThread` (0x1c7ec0) creates it with `iosCreateThread(0x1c80a0, stack 0x26d9d0, 0x1000,
  priority 0x28 = 40)` (its id at 0x26d9c4), creates and starts a second one (0x1c7ae0, priority 45), and installs the
  VBlank-start interrupt handler 0x1c8180 (`iosAddIntcHandler(2, ...)`). Priorities of the other threads: the thread
  system's own thread 0x1b2ca0 2, the GP thread 0x1c38d8 42, the main thread and the loader 45, the memory-card
  manager 47, the idle thread 125. So t8 preempts the game thread whenever it is woken, and only the kernel-side
  thread at priority 2 can preempt it.
- **Entry 0x1c80a0.** Saves s0-s6 / ra, creates the message box 0x26e9d0 (`iosCreateMsgQueue(0x26e9d0, 0, 0)`), sets
  s0 0x1e133c, s1 0x1e1314, s2 0x26d9a0, s3 / s6 0x270000, s4 / s5 0x1e0000 and loops forever:

  | pc | what | guest memory |
  |---|---|---|
  | 0x1c8100 | `iosSleepThread()` | parks until woken |
  | 0x1c8108 | `*(u64 *)0x26d990 = iosGetTCountExtension()` | writes the tick's start time |
  | 0x1c8110 | `if (*0x26d9a0 == 0) ++*0x1e1314` | reads the pause flag, writes the VBlank count |
  | 0x1c8128 | `if (*0x1e1340) ++*0x1e133c` | a second count, enabled by a flag |
  | 0x1c8140 | `tickProcSound(*0x26d9a0)` (0x1cb2c0) | the SG2 engine's tick, below |
  | 0x1c814c | `if (*0x1e1318) (*0x1e1318)()` | an optional callback (no writer found in the code: null) |
  | 0x1c8160 | `iosPadTickProc()` (0x1b1e70) | the pads' state |
  | 0x1c8168 | `iosRecvMsg(0x26e9d0, 0, 0)` | a poll of the GP thread's box: the GP thread blocks in `iosSendMsg` on it (docs/native_n5b.md) and is let through once a VBlank |

- **tickProcSound (0x1cb2c0).** On a change of the pause flag (0x1e1568 holds the last one): `Sg2DspSetMasterVolume`
  (0, 0, 0) and (1, 0, 0) when pausing, `Sg2DspSetMasterVolumeMax` when resuming; while paused, nothing more. Else
  `Sg2RemoteSyncStatus` (0x1d21c8: is the driver's reply in?), and when it returns 0 `Sg2CtrlTickProc` (0x1cc1c0: the 16
  tick procs of the table at 0x273d40, each with its count; the tick count 0x273dc0); then ++0x1e1564 (the sound tick
  count), `Sg2ExPcmTickProc` (0x1dc210), the PCM tick proc *0x271c0c and the ADPCM tick proc *0x271c08 when set, and
  the pause flag stored. The tick procs in the table are the SG2 engine's (Sg2SlotTickProc, Sg2RemoteTickProc, the
  sequencers Sg2SepTickProc / Sg2SmfpTickProc / Sg2ModTickProc, the ADPCM stream Sg2ExStAdpcmTickProc and the dummy
  0x1cc028 for the empty entries); N2 made the remote layer and the slot tick host code (docs/sound_n2.md).
- **Who wakes it.** Only the VBlank-start handler 0x1c8180: `iWakeupThread(*0x26d9c4)` once a VBlank (a wake that
  comes while the thread runs is counted by the kernel and makes the next SleepThread return at once: the runtime's
  self-wake, PS2X_SELF_WAKE).
- **Who reads what it writes.** The game thread and the other threads read the VBlank count 0x1e1314 (frame pacing,
  0x1c81b0's time check with 0x26d990), the pads' state, the sound engine's state (the Sg2 API the game calls writes
  the same slot tables / the remote queue; the status block 0x28b540 the driver's reply fills); the GP thread waits on
  the box. Nothing in the loop waits on anything but its SleepThread; its callees can block in principle (n5_blocking:
  every one of them is BLOCK through jalr / stubs), and the runtime counts what they actually do (section 4).

## 2. The design: a host carrier for the guest thread

The order of everything the other threads observe is decided by the EE scheduler (priority 40, woken by the VBlank
handler, preempted only by priority 2). B1 keeps that order exactly by keeping the scheduler's decisions and changing
only how the thread's code runs:

- **The body.** b1_sound.cpp's `body` is the loop above as host C++ (the same loads, stores and register values, the
  same EE cycles charged: 8 a call, 32 the back edge), called from 0x1c8108 (just woken). `tickProcSound` and
  `Sg2CtrlTickProc` are host functions in their slots, written in the generated code's guest-frame form (the same frame
  stores and callee-saved registers), so anything below them that resumes into them resumes into the generated code
  exactly.
- **The carrier.** EeScheduler gains host-carried guest threads: once GAMECORE runs and the switch is on, the first
  time the dispatcher (run() or S17's nested dispatch) would run t8's own context at 0x1c8108 it starts the body on a
  host thread of its own (`claimCarrier`), and from then on, wherever it would have dispatched t8, it hands that host
  thread the baton and waits (`resumeCarrier`). The two host threads pass one baton (a mutex hand-over): exactly one of
  them runs guest-visible code at any time. The executor identity, the host stack floors, the frames-that-cannot-return
  count and the guest jumps' serial are the carrier's while it holds the baton.
- **Blocking just blocks.** Where the generated code would unwind to the dispatcher, the carrier hands the baton back
  and goes on in place when it gets it again: a checkpoint that must reach the scheduler (an interrupt or event due, a
  preemption: `checkpointSlow`), a park (`SleepThread`, a semaphore / event-flag wait) or a thread switch the game asked
  for (`SignalSema` waking a higher priority: `leaveThread`). The dispatcher does exactly what it did after the unwind
  (the events, the interrupt handlers as invocations on t8, the reschedule, other threads) and resumes the carrier
  where it would have dispatched t8 again. No unwinding, no resume entries for the loop's frames, no re-dispatch of
  the loop's pcs.
- **Handing back.** Anything that still unwinds on the carrier (an exception-based switch: blockCurrent / exitCurrent /
  invokeCurrent; the host stack's floor; the stop) or a yield while the switch has been turned off (the A/B window)
  unwinds as before; the body leaves the registers and frames the generated loop expects at every call, so t8 goes on
  as generated code from its resume pc until its next wake, when it is taken over again.

What is still PS2-shaped: the thread's kernel record (its priority, its sleep / wake and its place in the ready queues
are the EE kernel's), the VBlank handler's `iWakeupThread`, the GP thread's box (a guest message queue), and the
callees (iosGetTCountExtension, iosPadTickProc, iosRecvMsg, iosSleepThread, the SG2 tick procs other than N2's, the
PCM / ADPCM procs) are the generated code. They need B2 / B3 (section 6).

## 3. Why gameplay stays exact

- **The same operations in the same order.** The body issues the generated loop's guest-visible operations one for one
  (the same loads and stores, the same calls with the same ra / argument registers, the same delay-slot effects), and
  charges the same EE cycles (8 a call through the dispatch checkpoint, 32 the back edge at 0x1c8178). tickProcSound
  and Sg2CtrlTickProc are written the same way, in the generated code's guest-frame form (the frame stores at the same
  sp offsets, the callee-saved registers kept as the original does), so a callee that unwinds through them resumes in
  the generated code exactly where the generated function would have.
- **The same scheduling decisions.** Every decision is still the dispatcher's: the carrier only runs while the
  dispatcher would have been running t8's own context, and hands the baton back exactly where the generated code would
  have unwound (a checkpoint that must reach the scheduler, a park, a thread switch). The dispatcher then does exactly
  what it does after an unwind (events, interrupt handlers as invocations, the reschedule, other threads) and resumes
  the carrier where it would have dispatched t8 again. The VBlank handler's `iWakeupThread` meets the same kernel
  record (a wake while t8 is the running thread is counted, a wake while it sleeps readies it), so no tick is lost or
  added.
- **One runner at a time.** The baton makes the carrier and the executor strictly alternate: whatever one side wrote is
  seen by the other at the hand-over (a semaphore signal / wait pair, or the mutex in the cv form). The executor's
  identity, its host stack floors, the frames-that-cannot-return count and the guest jumps' serial are the carrier's
  while it runs, so the runtime's checks behave as on the executor.
- **Handing back is exact too.** At any yield with the switch off (the A/B window) the carrier is told to unwind there:
  the body leaves the registers and frames the generated loop expects, and t8 goes on as generated code from its resume
  pc until the next park at 0x1c8100, after which the next dispatch at 0x1c8108 takes it over again.
- **Measured** (section 4): the kept registers checked every tick, the executor audit (guest code reaching the scheduler
  from a host thread that does not hold the baton), the carrier's ticks against the VBlanks and the game's own VBlank
  count against its sound tick count.

## 4. Checks

Build p3 (B1 + B1b's baton, the fix of section 7), Vita3K 222 MHz, muted. `SOTC_B1SOUND=1 SOTC_B1SOUND_CHECK=1
SOTC_B1_LOG=600 SOTC_AUDIO_LOG=1` (the check also turns the watchdog on at 10 s):

| run | frames | carrier ticks vs VBlanks (600-frame windows, after the first) | executor audit | kept registers | game VBlank count = sound ticks = ctrl ticks | watchdog |
|---|---:|---|---|---|---|---|
| ride7 600 s (chkR4) | 18 493 | equal in 30 of 30 | 0 bad of 325 304 | 0 bad | 35 259 = 35 259 = 35 259 | silent |
| attract 400 s (chkA4) | 19 327 | equal in 32 of 32 | 0 bad of 342 272 | 0 bad | 23 588 = 23 588 = 23 588 | silent |
| lake6 400 s (chkL4) | 12 448 | equal in 20 of 20 | 0 bad of 273 642 | 0 bad | 23 201 = 23 201 = 23 201 | silent |

One take-over a run (claims 1), 0 hand-backs (switch / unwind / throw), ~2.3 baton round trips a frame (park 2.3, a
checkpoint yield 0.01). Every run went to its end: Wander mounted Agro in ride7 (frame 3 678; the off reference 4 518,
the mount frame varies run to run), lake6's teleport at frame 2 000 and the boss arena.

**The A/B windows** (switch toggled every frame at run time, `SOTC_AB_PERIOD=1`): ride7 800 s seed 7 / seed 13, lake6
400 s: 6 213 / 5 841 / 3 169 take-overs and as many hand-backs (by the switch, at a yield: the carrier unwinds, t8 goes on
as generated code, the next park at 0x1c8100 and dispatch at 0x1c8108 take it over again), 0 by exception, no freeze;
the game's VBlank and sound tick counts still equal (48 136, 48 418, 24 362).

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89 SOTC_B1SOUND=1`, ride7 600 s, shots every 120 s, build p3): booted, rode the 600 s (mounted at frame 3 326; frames 3 670 / 7 245 / 10 762 / 14 927 / 18 810 at 120 / 240 / 360 / 480 / 600 s, 29-36 game fps), system free 67.3 MB at the end, carrier ticks = VBlanks to the end, no hand-back. The shots at 60 / 180 / 300 / 540 s (looked at): Wander kneeling by Agro, then riding across the plain and past the ruins and the cliff; ground texture, Agro, Wander and the body shadows normal.

**Audio counters** (`[audio]` lines summed; on = the check runs above, off = `SOTC_B1SOUND` unset, same build and route):

| | attract on | attract off | ride7 on | ride7 off | lake6 on |
|---|---:|---:|---:|---:|---:|
| VBlanks / sound status replies | 24 024 / 24 024 | 24 012 / 24 012 | 36 098 / 36 098 | 36 096 / 36 096 | 24 046 / 24 046 |
| status gaps over 50 ms | 28 | 25 | 296 | 583 | 139 |
| streams started / stopped | 4 / 2 | 4 / 3 | 18 / 17 | 21 / 19 | 8 / 7 |
| stream refills (max wait ms) | 8 (25.4) | 8 (18.6) | 36 (54.4) | 42 (34.7) | 16 (34.3) |
| key on | 2 729 | 2 550 | 11 379 | 13 192 | 1 602 |
| PCM underruns, stale halves, ADPCM waits | 0, 0, 0 | 0, 0, 0 | 0, 0, 0 | 0, 0, 0 | 0, 0, 0 |

The same shape on and off (the key-on and stream counts follow the route's timing, which varies run to run: the mount
frame alone moved by ~800 frames). The runs are muted (PS2X_NO_SPU2_AUDIO=1: no output callback, so output underruns
cannot show here): **the user must listen on the console before B1's audio can be trusted.**

## 5. Measurements

Vita3K 222 MHz, 3 cores, interleaved A/B of `b1sound` (`SOTC_AB_FLAG=b1sound SOTC_AB_PERIOD=1`, `SOTC_B1SOUND=1` in the
env), off - on (positive: on faster), 2 SE:

| run | frame | busy |
|---|---:|---:|
| ride7 800 s seed 7 (mixR7b) | +0.030 +- 0.147 | +0.091 +- 0.128 |
| ride7 800 s seed 13 (mixR13b) | +0.126 +- 0.130 | +0.135 +- 0.114 |
| lake6 400 s (mixL7b) | +0.310 +- 0.099 | +0.310 +- 0.099 |
| **pooled** (mean of 3, SE = sqrt(sum se^2) / 3) | **+0.155 +- 0.037 ms (1 SE), z 4.2** | +0.179 +- 0.033 (z 5.4) |

The carrier's own time is 230-260 us a frame in the check runs (the whole tick, both hand-overs included). What it saves
is the generated loop's dispatches and unwinds of t8 (two parks a tick: each was an unwind to run() and a re-dispatch of
the loop's resume pc) and the host tick functions' calls; the A/B pays a take-over and a hand-back every frame and still
comes out ahead. The cv baton re-checked every 1 ms (SOTC_B1_BATON=cv) cost ~70 us a frame more than the semaphores
(306 vs 233 us a frame in the 300 s ride7 runs of section 7).

**Default.** Off (`SOTC_B1SOUND` must be 1-3): B1's audio has not been heard on the console yet, and the semaphore baton
has only run in Vita3K. Console A/B package: `build/agent-scratch/b1/celosia-b1.vpk` + `env-ab.txt` (N6's env +
`SOTC_B1SOUND=1 SOTC_AB_FLAG=b1sound SOTC_AB_PERIOD=300 SOTC_B1_LOG=600`).

## 6. What B2 / B3 need

- **B2 (the loader / streaming threads).** The same carrier serves any guest thread whose loop B2 writes as host code
  (setHostCarry takes one thread today: a list is a small change). The GP thread's box 0x26e9d0 and the ios message
  boxes (iosSendMsg / iosRecvMsg) become host queues there; t8's `iosRecvMsg` poll then stops being a guest call.
- **B3 (the main thread, the kernel objects).** Once the threads that block (main, loader, t8) are all carried, a park
  no longer needs the dispatcher's unwind at all, and the EE semaphores / sleeps / wakes can be host primitives on
  the carriers directly; the dispatcher (run(), S17's nest) and the generated checkpoints go away with it.
- **The baton.** Any further host thread that passes control with the executor must use the semaphore baton (or any
  primitive that counts), never a bare notify: section 7.
- **The callees.** iosGetTCountExtension, iosPadTickProc, iosRecvMsg, iosSleepThread, the SG2 tick procs other than
  N2's, the PCM / ADPCM procs are still generated code called from the body; the tick could call them as host functions
  once their own callees are (phase C5's sound area).

## 7. The freeze (round B1b) and its fix

**Symptom.** With SOTC_B1SOUND=1 B1's build froze in Vita3K: frames stopped (gameFps 0) while the process lived, at a
random point (ride7 89 MB boot frame 85, ride7 check ~379, lake6 check ~3, attract check ~1259; every A/B). With the
switch off everything ran to the end.

**The watchdog.** `SOTC_B1_WATCHDOG=<seconds>` (default 10 with SOTC_B1SOUND_CHECK; EeScheduler::startB1Watchdog): a host
thread that, once no game frame has advanced for that long, prints `[b1dog]` lines: the executor's site (in
resumeCarrier's hand-over / its unwind hand-over / the idle wait / generated code), the carrier's site (body / yielded at
a checkpoint / yielded at a park or switch / waiting for a body / ended), whose turn the baton says it is, each side's
"inside the wait" flag and both hand-over counts, every guest thread's kernel record (status, wait reason, wakeup count,
pc / ra), the pending events, deadlines and invocations and the ready queues.

**What it showed** (build p2 / p3, all three scenes, every dump identical for 48 s):

- executor: "waits for the carrier", the dispatcher's current thread 0; carrier: "yielded (park / switch)", baton turn
  **dispatcher**; hand-overs executor 994 = carrier 994 (ride7 frame 462; attract 2 443 at frame 1 188, lake6 1 128 at
  frame 529);
- with p3's flags: **inside the condition variable's wait: executor 1, carrier 1**;
- t8 Ready or Sleeping at 0x1c8108 / 0x1b1278 with 0 wakeups, the other threads in their usual waits, one VBlank
  deadline pending, no pending invocations: the guest state was fine and nothing guest-side was waited for.

So the carrier had handed the baton back (turn = dispatcher, written under the mutex, notify_all after it) and gone to
sleep in its own wait, and the executor was still asleep in `cv.wait(lock, turn != carrier)` with its predicate true:
the hand-over's notify never woke it. B1's baton was the classic mutex + condition variable, which is correct C++; the
wake was lost below it (vitasdk's pthread condition variable on the Vita's kernel primitives as Vita3K emulates them;
which of the two layers loses it was not isolated). Neither side waited on anything else: it was not a lock cycle,
not a lost guest wakeup, not an interrupt or the GP thread's box.

**Proof by switching the baton** (p3, ride7 300 s each, on + check):

| baton | result |
|---|---|
| `SOTC_B1_BATON=cv SOTC_B1_BATON_US=0` (B1's exact form) | froze at frame 462, executor inside its wait with the baton back (above) |
| `SOTC_B1_BATON=cv` (the wait re-checks the baton every 1 ms) | ran 300 s (9 496 frames); re-checks that found the baton already back: executor 156, carrier 3 851 (an upper bound of the lost wakes: a notify that lands just after the timeout counts too); carrier 306 us a frame |
| semaphores (default) | ran 300 s (8 905 frames), carrier 233 us a frame |

**The fix.** On the Vita the baton is one kernel semaphore per side (`sceKernelCreateSema`, max 1): the side that hands
over signals the other's semaphore and waits on its own; a signal is a count, so a hand-over can never be missed, and
it costs one kernel call each way instead of a mutex, a broadcast and a wait. The first hand-over to a new body and the
body's end use the same semaphores. `SOTC_B1_BATON=cv` keeps the condition variable (its wait re-checked every
`SOTC_B1_BATON_US`, default 1000 us; 0 = B1's untimed form, which reproduces the freeze) for comparison and for hosts
without the Vita's semaphores. The `[b1sound]` line counts the cv form's rescues.
