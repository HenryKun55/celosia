# Round B1: the VBlank / sound thread on a host thread (switch b1sound) (2026-11-22)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B1. Base: main f643b44 (P3). Switch `SOTC_B1SOUND` (A/B
name `b1sound`, sotc_decomp `kB1Sound`, @@DEFAULT@@). Code: game/overrides/b1_sound.cpp (the thread's body and the host
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

@@EXACT@@

## 4. Checks

@@CHECKS@@

## 5. Measurements

@@MEAS@@

## 6. What B2 / B3 need

@@NEXT@@
