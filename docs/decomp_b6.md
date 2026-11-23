# Round B6: the carriers' message boxes and semaphore calls as host primitives, t12 on a carrier (switch b6sync) (2026-11-23)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B6. Base: main 5b8074f (B5 on A10). Switch `SOTC_B6SYNC` (A/B name
`b6sync`, sotc_decomp `kB6Sync`, **off by default**; it acts only on carried threads, so it needs the carrier switches b1sound /
b2load / b3threads, but toggles on its own). Code: game/overrides/b6_sync.cpp (the boxes, the semaphore stubs, t12's body, the
check), PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (`EeScheduler::carrierWakeList`),
sotc_overrides.cpp (`sotc_decomp::swapIosRecvInner`: the idle-turn hook's inner call), sotc_native.cpp (install, the frame call,
the switch name). Scripts, runs and logs: `build/agent-scratch/b6/` (NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c
(pointed at b6; 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. What the boxes really are

decomp_b3.md section 8 described the boxes as queues. Reading iosCreateMsgQueue's callers (iosCreateIdleThread 0x1ad198, the
VBlank thread 0x1c80a0, the main thread's start 0x1c7ae0, initMcManager 0x114a368): the three boxes with carriers at both ends
are created **with no buffer** (`iosCreateMsgQueue(box, 0, 0)`: slots 0, capacity 0). They are rendezvous points, not queues:

| box | senders (iosSendMsg, a2 = 1) | receiver (iosRecvMsg) | what a receive does |
|---|---|---|---|
| 0x26e9d0 (GP box) | t10 (waitVscheduler), t9 | t8, once a VBlank (a poll) | lets the GP thread's VBlank turn through |
| 0x26d9a8 (frame box) | t9 at its frame's end | t10 when the GS is done (0x1c8078, the frame cap's hook) | starts the next frame |
| 0x1fc870 (idle queue) | t11 / t9 (iosWaitIdleThread) | t5 (the idle thread's poll) | wakes whoever waits for idle |
| 0x121af50 (memory-card box) | t9 (mcLoadGameData etc., a2 = 1) | t12 (blocking, a2 = 1) | **one 8-byte slot** (capacity 1): a real mailbox |

The guest code on a rendezvous box (iosSendMsg 0x1b0fc0 / iosRecvMsg 0x1b1138):

- **send, a2 = 1**: WaitSema(kernel message semaphore *0x1e0be8); count 0 < capacity 0 fails, so: the caller's ios record's +0x28
  = 0, the record appended at the tail of the box's sender list (+0x18, linked through +0x28), SignalSema, iosSleepThread
  (SleepThread); when it runs again the box's +0 is still 0: return -1. (a2 != 1: -1 at once.)
- **receive**: WaitSema; +0x14 = the receiver's record; no buffer, so the whole sender list is taken (+0x18 = 0); +0x14 = 0;
  `iosCallBackThreadMgr(0x1b1340, head)`: WaitSema(the callback slot's semaphore *0x201dc4), the slot written (0x201dc0 = 1,
  0x201db8 = 0x1b1340, 0x201dbc = head), WakeupThread(t4). t4 (priority 2) preempts the receiver, runs 0x1b1340 (iosWakeupThread
  of each record from the head through +0x28: WakeupThread(record +4)), SignalSema(slot), SleepThread. The dispatcher picks the
  highest ready thread; the receiver, when it runs again, SignalSema(message semaphore) and returns 0 (*a1 not written).
- The memory-card box (capacity 1) does the queue part: send puts the message in the slot at the write index (+0xc, wrapping),
  count + 1, and wakes a receiver waiting in +0x14 through the same slot with `0x1b1328` (iosWakeupThread of one record);
  receive takes the oldest message (write index - count), count - 1, or (none, a2 = 1) leaves its record in +0x14 and sleeps,
  then loops from its WaitSema. A full box queues the sender in +0x18 and the receive then pops it but wakes the *next* sender
  (a quirk of 0x1b1210..0x1b125c; with no next one t4 would wake itself and hit iosJumpRecoverPoint); this never happens in
  the runs (the full-box path: 0 calls) and B6 leaves it to the guest code.

## 2. The design

`b6_sync.cpp`, on a carried thread's own code (`EeScheduler::carrierCallable`), b6sync on, GAMECORE entered:

- **Rendezvous boxes as host wait lists** (`HostBox::senders`, the ios records' thread ids in order). A blocking send appends
  the caller (guest memory written as the guest code writes it: +0x28 = 0, linked at the tail) and parks in place
  (`EeScheduler::carrierSleep`: SleepThread's records, a counted wakeup returns at once); it returns -1. A receive takes the list
  (+0x18 = 0), writes t4's callback slot as iosCallBackThreadMgr would, and wakes the list itself, in order, then makes the one
  switch the dispatcher would make after t4 parked (`EeScheduler::carrierWakeList`: wakeupThread of each id, then
  transferIfRequested's switch as a pinned yield if a woken thread outranks the receiver); it returns 0. No kernel message
  semaphore traffic, no t4 turn.
- **The memory-card box as a host message ring** (`HostBox::msgs`, mirrored into the guest slot / count / write index): send
  with room, receive (take / none / wait in +0x14 and loop), the receiver's wake by the sender through carrierWakeList.
- **The guard**: the host path is taken only while the kernel message semaphore and (for a call that wakes) the callback
  slot's semaphore are free (count > 0), +0x14 is 0 on a receive, the box has its shape (no buffer for the rendezvous boxes; a
  buffered box not full and with no waiting sender), and the caller's ios record names it. Anything else runs the guest code
  (which then blocks on the semaphores exactly as before). A buffered receive that wakes while the message semaphore is held
  takes it in place (carrierWaitSema) and gives it back at the end, as the guest code's loop does.
- **Where it sits**: the send in iosSendMsg's slot below the count wrappers (b2load / b3threads install after it), above the
  lifted iosSendMsg (liftd12); the receive inside the idle-turn hook on iosRecvMsg (`sotc_decomp::swapIosRecvInner`), so its
  poll count and idle wait stay around it.
- **The semaphores.** The EE kernel's semaphores are already host records (`EeSemaphore`: count, max, a FIFO `std::deque` of
  waiting thread ids in EeScheduler); there is no guest-memory kernel record to replace, and ReferSemaStatus reads that record.
  B3 made the stubs' calls in place on a carrier. B6 puts host functions in the stubs' slots (WaitSema 0x107020, SignalSema
  0x107000, PollSema 0x107030): on a carried thread's own code they call the record directly (`carrierWaitSema /
  carrierSignalSema / carrierPollSema`: the count, FIFO waiters, the preemption test), without the syscall layer (Sync.cpp's
  binding call, the SOTC_B3_SYNC gate); anything else (the boot, a handler's i* call, an uncarried thread) takes the stub. One
  record for every path (the isys fast paths, iSignalSema from handlers, ReferSemaStatus): nothing to keep in step.
- **t12 on a carrier** (`bodyT12`, B3's form): mcManager's loop (0x1149138..0x114934c: the blocking receive on 0x121af50, the
  request's type through the jump table 0x1195a70 to one of 14 ios memory-card calls / manager functions, the result in the
  request's +0x2c, the request's +0 = 0) as host C++ over the guest registers, instruction for instruction from the generated
  function (the ELF's JALs read from the code as the generated code reads them: IndirectCall; the back edges' checkpoints;
  33 claim labels). Its gate follows b6sync (`SOTC_B6_T12=0`: not carried); entry 7 of the carrier table, added after B3's.
  With it, every live thread after GAMECORE is carried, and 0x121af50 and semaphores 13 / 14 / 15 have carriers at their ends.

## 3. Why it stays exact

- **Messages and wake order.** The rendezvous boxes carry no message (no buffer): what is ordered is the release of the senders,
  which B6 keeps (the list order, walked from the guest memory's head through +0x28 exactly as 0x1b1340 walks it). The
  memory-card box's messages keep their order (one slot; the host ring mirrors it).
- **The t4 hop.** t4 runs at priority 2: none of its wakes preempts it, and after it parks the dispatcher picks the highest
  ready thread. The live threads after GAMECORE have distinct priorities (t4 2, t8 40, t10 42, t11 43, t9 45, t12 47, t5 125),
  so that pick is the receiver (back at the front of its queue) unless a woken sender outranks it: the decision carrierWakeList
  makes after waking the same threads in the same order (same ready-queue contents, same front). What the hop adds and B6
  removes is t4's own turn (its SleepThread / WakeupThread leave its record as they found it), the dispatches around it (where
  the EE clock is seen: the same order of change as host timing after GAMECORE, decomp_b1-b5), and nothing else in guest memory:
  t4's slot is written by B6 as by the guest path, and only t4 reads it, after a wake.
- **The kernel message semaphore and the slot's semaphore.** In the guest path each is taken and given back around the call.
  On the host path no other thread can run inside the call (the carrier holds the baton and does not yield until the final
  park / switch), and the guard takes the host path only while both are free, so the guest path's WaitSema would not have
  blocked; their counts end where the guest path leaves them. When one is held (someone in the guest path is inside a message
  call) the guest code runs and blocks on it as before.
- **Guest memory.** The box's +0x18 list, the records' +0x28 links, +0x14, the memory-card box's slot / count / write index and
  t4's slot end as the guest code leaves them; the stack below sp (the callees' dead frames) is not written. So every reader that
  still reads the box (the idle-turn hook, the frame cap's release hook, [b5dog], the loader log) sees the same box.
- **Registers.** The host calls return v0 as the guest functions do (-1 / 0) and pc = ra; s-registers and sp untouched. The
  guest functions also leave caller-saved temporaries (a0, v1 ...) that the callers (compiled code, the B-series bodies written
  from it) do not read after the call.
- **The semaphore calls** do what the syscall does on the same record (carrierWaitSema etc. are B3's, checked there and here).

## 4. Checks

Build p3, Vita3K 222 MHz, muted, `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_B6SYNC=1` with every check
(`SOTC_B1SOUND_CHECK=1 SOTC_B2LOAD_CHECK=1 SOTC_B3_CHECK=1 SOTC_B6_CHECK=1`: the executor audit, kept registers, B1-B3's counts,
the B1 watchdog at 10 s; `SOTC_B5_DOG=10`: B5's idle-thread watchdog) and the ride probe. `SOTC_B6_CHECK` runs the shadow of the
guest path's semantics on every host operation: per box the sender list the guest code would hold against the host list and
the guest memory's list, the released list and the switch the dispatcher would make after t4; for the memory-card box the
message ring (each message taken = the shadow's oldest); per semaphore the record before / after each call against the
syscall's rules (result, FIFO waiter woken, block / switch exactly when the syscall's would).

| run | frames | B6 shadow checks | executor audit | t12 carrier (claims / turns / parks / hand-backs) | watchdogs | outcome |
|---|---:|---|---|---|---|---|
| ride7 600 s (chkR) | 13 800 | **0 bad of 4 836 554** | 0 bad of 316 192 | 1 / 109 / 272 / 0 | silent | field 2 000, mounted 2 771, 0 dismounts |
| attract 400 s (chkA) | 16 200 | **0 bad of 3 549 291** | 0 bad of 292 961 | 1 / 15 / 70 / 0 | silent | past the title, the demo |
| lake6 400 s (chkL) | 8 400 | **0 bad of 2 620 859** | 0 bad of 199 554 | 1 / 109 / 272 / 0 | silent | field 2 000, the teleport, the boss arena |

**0 bad of 11.0 M shadow checks, 0 of 0.81 M audit**, 0 kept-register faults (t12's included), 0 hand-backs of any carrier,
0 `[b1dog]` / `[b5dog]` lines. (Frame counts are lower than B5's: these runs had the host at load 16-24 from other jobs.)

**The boxes on the host path** (chkR, `[b6box]`, 13 800 frames):

| box | host sends (blocking) | receives (with senders) | released | switches after a receive | guest path (why) |
|---|---:|---:|---:|---:|---|
| GP 0x26e9d0 | 15 372 (t10) | 34 095 (15 372) (t8) | 15 412 | 0 | 39 not carried (the first frames before the claims) |
| frame 0x26d9a8 | 13 764 (t9) | 15 372 (13 764) (t10) | 13 764 | 0 | 0 |
| idle 0x1fc870 | 65 (t11) | 18 684 (65) (t5) | 65 | 65 (t11 outranks t5) | 0 |
| memory card 0x121af50 | 109 (t9; one slot) | 109 (t12: 109 waited, 108 took, 109 woken by the send) | | 0 | 1 not carried |

The kernel message semaphore and t4's slot semaphore were never found held at a host call (0 `msgsema` / `cbsema` refusals
in all three); the full-box path 0. **t4 is out of the frame**: B3's `[b3threads]` showed t4 resuming 2.0 times a frame (its
box hops); with b6sync t4 had 5 turns in the whole ride (`resumes 0.00` a frame), and the dispatcher's picks a frame (`[b4fast]`)
fell to t8 1.10 / t10 1.09 / t9 1.12 (B4: t8 2.29, t10 2.15, t9 1.39, t4 2.13).

**The semaphores** (`[b6sema]`, chkR; host-path calls / guest-path calls after GAMECORE, by thread mask): used only by carried
threads on the host path: 18, 19 (t8 t9), 4 (t8), 17 (t9 t11), 12 (t9), 11 (t11), 13 (t12). Also used through the stub after
GAMECORE by threads that are not carried at that call: 14 / 15 (t12's, also t7: 163 calls each), 10 / 7 (34-361 / 3 024 calls by
t6 / t11 off the carrier), 16 (t6), 6 / 9 (t6, t13-t16 at the boot's end). 10 had 3.47 M host signals (the isys locks).

## 5. Boots, loads, A/B, 89 MB boot, spalign, console package

**Boots** (pf1-pf10: 89 MB, ride7 300 s, all five switches on, three trees): **10 of 10 reached the field at frame 2 000 and
mounted Agro** (frames 2 211-4 355), 0 stalls, 0 dismounts, 0 watchdog lines, the memory-card load through t12's carrier in all
of them. With the smoke runs, the checks, bootR / bootR2 and the load runs: 20 boots with b6sync on, every one through the
memory-card load. B5's livelock did not come back (`[b5]` deferral on, 1.17 park catch-ups deferred a frame, as in B5).

**Loads** (`SOTC_FRAME_LOG=1 SOTC_FRAME_LOG_ALL=1`; on = all five, off = the four carrier switches without b6sync; lg.py and
tools/loads.py; single runs at host load ~20):

| run | Load Game pair | 200-1999 longest, over 100 / 200 ms | teleport load | 0-1999 longest (the boot) |
|---|---|---|---|---|
| ride7 360 s on / off | 169 + 192 / 350 + 382 ms | 192 ms, 7 / 0 / 413 ms, 73 / 22 | 202 / 200 ms | 446 (157) / 430 (3) |
| attract 300 s on / off | 26 + 24 / 18 + 26 ms | 146, 2 / 0 / 149, 3 / 0 | (no load) | 445 (157) / 421 (3) |
| lake6 400 s on / off | 177 + 196 / 164 + 207 ms | 350 (the teleport), 12 / 2 / 346, 24 / 4 | 350 / 346 ms | 445 (157) / 494 (3) |

No new hitch, the same hitches in the same places; the ride7 "off" run's menu was much slower (73 frames over 100 ms against 7):
host noise of a loaded machine, not a b6sync effect to claim.

**Vita3K A/B** (222 MHz, `SOTC_AB_FLAG=b6sync SOTC_AB_PERIOD=1`, the four carrier switches + `SOTC_B6SYNC=1` in the env), off -
on (positive: on faster), 2 SE; pooled = mean of 3, SE = sqrt(sum se^2) / 3 (1 SE). The machine was at load 20-27 for every run
(other agents' jobs), so the per-run SEs are 3-7x B5's:

| run | first set (mixR7 / mixR13 / mixL7) | second set (mixR7b / mixR13b / mixL7b) |
|---|---:|---:|
| ride7 800 s seed 7 | -0.172 +- 1.187 | +0.197 +- 0.993 |
| ride7 800 s seed 13 | -0.533 +- 0.618 | -0.342 +- 0.476 |
| lake6 400 s | -0.326 +- 1.077 | +0.325 +- 2.316 |
| **pooled** | **-0.344 +- 0.286 (z -1.2)** | +0.060 +- 0.428 |
| six runs | **-0.14 +- 0.26 ms (1 SE), z -0.5: no measurable difference** | |

What the counts say instead: per frame riding, t4's two turns are gone and the dispatcher's picks a frame are t4 ~0, t8 1.10,
t10 1.09, t9 1.12 (chkR) against B4's ride (ldOnR, another run) t4 2.13, t8 2.29, t10 2.15, t9 1.39: about 4.5 picks a frame
fewer; Vita3K at this host load cannot resolve what that is worth. The console's A/B decides.

**89 MB boots**: bootR (all five on, ride7 600 s): field 2 000, mounted 3 773, rode to the end, frames 3 483 / 6 488 / 9 642 /
11 508 / 14 416 at 120 / 240 / 360 / 480 / 600 s, system free 65.0 MB, 0 dismounts; **no screenshots: the Mac was locked**
(the capture tool found the window and wrote nothing). bootR2 (the same, later): mounted 3 086, frames 3 351 / 6 635 / 10 283 /
13 569 / 17 359, system free 67.2 MB; two shots landed (420 s and 540 s), looked at: Wander on Agro at a gallop on the plain with
Agro's body shadow on the grass and the ruins behind; Wander on Agro along a cliff wall with the stone tower ahead; ground
texture, Wander, Agro, shadows normal, the frame counter advancing (game 21 / 32 fps in the overlay).

**spalign**: `python3 ../tools/spalign.py p3.elf`: 0 sp-based :128/:256 NEON accesses (of 1 055 with a hint).

**Default**: off (`SOTC_B6SYNC` must be 1-3). **Console A/B package**: `build/agent-scratch/b6/celosia-b6.vpk` (= p3) +
`env-ab.txt` (N6's console env without its A/B lines + `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_B6SYNC=1
SOTC_AB_FLAG=b6sync SOTC_AB_PERIOD=300 SOTC_B1_LOG=600 SOTC_B2_LOG=600 SOTC_B3_LOG=600 SOTC_B6_LOG=600`). Not installed.

## 6. What is left of the PS2 scheduler

With b1sound + b2load + b3threads + b4fast + b6sync every live thread after GAMECORE (t4 t5 t8 t9 t10 t11 t12) runs as host
code on a carrier, the boxes are host wait lists, and t4 is idle. What EeScheduler still does in guest shape:

- **The kernel's thread records and the dispatcher**: priorities, ready queues, Running / Waiting / Suspended, wakeup counts,
  sleep; every switch is still decided by run()'s loop body (on the executor, or on the yielding carrier with b4fast), and the
  carriers pass one baton. This is the EE kernel's scheduling model, kept on purpose (exactness); it is host data, but the
  decisions are made at the guest's points (checkpoints, parks, signals).
- **The EE clock and checkpoints**: guest code still charges cycles (8 a call, 32 a back edge) and checks the time slice /
  pending events at checkpoints; the host catch-up at parks and B5's deferral decide when a VBlank is seen.
- **Interrupts as guest invocations**: the VBlank / GS / DMA handlers run as generated code pushed onto the thread run() picks
  (the handlers' iWakeupThread / iSignalSema go through the syscalls).
- **The semaphore and event-flag records keyed by guest ids**, with the guest's syscall entry points still used by everything
  that is not a carried thread's own code (the boot threads, handlers, t6 / t7 / t13-t16), and the isys fast paths.
- **Hand-back / resume labels**: every carrier can still be handed back to the generated code (the A/B, the switches, the host
  stack floor), so the bodies keep the guest registers and the generated functions' labels; the bodies themselves are host
  C++ over R5900 registers, not typed code.
- **The boot**: t1 / t2 / t3 / t6 / t7 / t13-t16 stay generated code dispatched by run() (t7 still shares semaphores 14 / 15
  with t12).

Next steps that this round makes possible: delete t4 from the frame for good (it now only serves thread deletion and recover
jumps); write the boxes' callers (t8 / t9 / t10 / t5 / t11 / t12 bodies) against `HostBox` directly instead of through the
iosSendMsg / iosRecvMsg slots; make the boot threads independent of dispatch charges so the hand-back path (and the resume
labels) can go.
