# Round B2: the loader / CD streaming thread on a host thread (switch b2load) (2026-11-22)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B2. Base: main 414a182 (B1 + B1b on A7). Switch `SOTC_B2LOAD`
(A/B name `b2load`, sotc_decomp `kB2Load`, **off by default**). Code: game/overrides/b2_load.cpp (the thread's body),
PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (the carrier table, the waits in
place), Kernel/Stubs/CD.cpp, MPEG.cpp, ps2_runtime.cpp (the waits' callers). Scripts, runs and logs:
`build/agent-scratch/b2/` (NOTES.md has every command). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (B1's, now pointed
at b2; 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. Which threads load

The brief named two threads, "the loader t6 (entry 0x1b5a00)" and "the CD streaming thread t11". A thread census of main's
build riding (`SOTC_N5_LOG`, `[n5t]`: every guest thread's entry, priority, state) says they are not two live threads:

| t | entry | prio | what | alive riding |
|---|---|---:|---|---|
| t1 / t2 | 0x100010 / 0x104ff0 | 1 | the loader system's threads (LoaderSys) | parked for good |
| t4 | 0x1b2ca0 | 2 | the thread system's own thread | yes, ~2 runs a frame |
| t5 | 0x1ad270 | 125 | the idle thread (polls the idle queue 0x1fc870) | yes |
| t6 | 0x1b5a00 | 45 | **IosCdvdManagerSimulation: the boot loader** | **no: dead after GAMECORE's entry** |
| t7 | 0x11b458 | 1 | (waits on a semaphore for good) | parked |
| t8 | 0x1c80a0 | 40 | the VBlank / sound thread (B1) | yes |
| t9 | 0x1c7ae0 | 45 | the game thread | yes |
| t10 | 0x1c38d8 | 42 | the GP thread | yes |
| t11 | 0x115e4c0 | 43 | **the sheet manager: every load and stream job** | yes, 0.1-10 runs a frame |
| t12 | 0x11490f8 | 47 | the memory-card manager | yes, rarely |

- **t6, IosCdvdManagerSimulation (0x1b5a00)** is not a loop. It finds the XFF modules on the disc (sceCdSearchFile),
  loads and relocates them (0x1b5510, 0x1b57f8, FlushCache) and jumps to each module's entry (`jalr s0` at 0x1b5b04 /
  0x1b5bcc / 0x1b5ca0; 0x1b5bcc is GAMECORE's entry, the "GAMECORE entered" point), then returns: its thread ends. It runs
  only during the boot (0.65 runs a frame in the first 300 frames, never after; diag1).
- **t11, manager_sub_0115E4C0** (GAMECORE's sheet manager, created by sheetMgrInit with priority 43) is the loader and the
  streamer at once: it sleeps until a job is queued, then runs the job (segment index load / unload, the map, the stage
  textures, the ADPCM stream, a direct file, the special file system ...; 13 types through a jump table), all of whose disc
  reads go through sceCdRead + sceCdSync (iosCdStRead's helper 0x1ac9c8: its park pc 0x1acaec is "t11, CD streaming" in
  docs/native_n5.md) or LoaderSysFRead; then it takes the next job from five priority lists under the queue's semaphore.
  Riding it runs 0.1-1.8 times a frame and 10-300 us of host time; at a load up to 10 runs a frame.

So B2 carries t11. t6 cannot be carried exactly (section 8): it lives only before GAMECORE's entry, where the EE clock is
executed cycles and the time slice decides the heap order that places the modules, and a carrier changes the dispatcher's
cycle charges.

## 2. The thread's loop (b2_load.cpp's `body`)

From the take-over pc 0x115e5a8 (SleepThread just returned):

| pc | what |
|---|---|
| 0x115e5a0 | `iosSleepThread()` (a JAL read from the code: the ELF's functions are relocated into GAMECORE's code at load) |
| 0x115e5a8 | `job = *0x118fe34` |
| 0x115e5b0 | `if (job->status) *job->status = 0xdeae`; `if (job->flags & 1)` cancelled: status 0xdeb0 |
| 0x115e5e0 | `switch (job->type)` through the table at 0x1196a10 (type >= 13: iosJumpRecoverPoint) |
| 0x115e608 | type "segment index": unless the queue is held (*0x118fe1c), unless the segment is on the load list 0x12228b8 (a search that marks the entry it finds), `++use[seg]`; on the first use sheetFileLoadSegmentIndex, then the segment's loaded bit |
| 0x115e750 | type "segment unload": the same search, `--use[seg]`; on the last one the loaded bit cleared, manager_sub_0115DF58, manager_sub_0115D968 |
| 0x115e878 | sheetFileLoadMap (7 arguments from the job) |
| 0x115e8d0 | a call through the file system's table (*(*(0x1b7b60 + 0x1c) + 8)), then manager_sub_0115D968 |
| 0x115e910 | sheetFileLoadStageTex, sheetFsSetStageTexSegTexSegDef (NOCHK) |
| 0x115e968 / 988 / 9a0 / 9c8 / 9e8 / a00 | manager_sub_0115E268, sheetFsExistTexSegIndexCallBack, sheetFsLoadAdpcmInt, sheetFsLoadDirectFile, sheetFsSetSpecialFs (NOCHK, twice) |
| 0x115ea20 | `*job->status = 0xdeaf` |
| 0x115ea30 | `WaitSema(*0x1220ca0)` (the queue's lock), the job onto the done list 0x12228a8 |
| 0x115ea58 | unless the game holds the queue (*0x118fe30, *0x1e1324) or the finished job was not of type 1 / 3 / 5 / 6: the next job from the five priority lists 0x12228b0 |
| 0x115eae8 | `*0x118fe34 = next; SignalSema(lock)`; a next job: back to 0x115e5b0; none: back to SleepThread |

The body is written over the guest registers in the context, instruction for instruction (lw / addiu / lui / addu / sll
/ sra results sign-extended from 32 bits, ori / andi / and / nor / daddu / lbu 64-bit, movn copying the whole 128-bit
register, every branch's delay slot where the original executes it, the likely branches' only when taken), with the
callees called as the generated code calls them (DirectCall for GAMECORE's own, the ELF's as a JAL read from the code,
IndirectCall for the jalr, `callGuestKnownNoCheck` with the generated function for the two NOCHK callees). Its labels are
the generated function's resume entries, so a hand-back anywhere leaves ctx->pc on a label the generated code resumes at.

## 3. The carrier table and the waits in place

- **A table.** B1's EeScheduler took one carried thread (setHostCarry). B2 makes it a table of up to four
  (`addHostCarry(entry, claimPc, body, gate, stats)`): each entry has its own switch flag (B1: g_ps2xB1Carry, B2:
  g_ps2xB2Carry, both set once a frame from their switches and only once GAMECORE runs) and its own counts (g_eeB1,
  g_eeB2), a spare host thread per entry, and its own pair of baton semaphores (B1b's kernel semaphore per side on the
  Vita; `SOTC_B1_BATON=cv` still selects the condition variable). setHostCarry is entry 0 with B1's flag, so t8 works as
  before. One carrier holds the baton at a time (m_activeCarrier): the dispatcher resumes whichever carried thread it
  would have dispatched; the others sit in their yields. The watchdog ([b1dog]) and the executor audit cover all of them.
- **The waits that used to leave only by the exception** now park in place on a carrier too: `waitExternal` (sceCdSync's
  wait for an asynchronous read, the MPEG picture wait, DelayThread through delayCurrent) and `waitVSync` (sceCdStRead's
  wait for stream data, WaitVSyncTick). On a carrier they park the thread, hand the baton back, and return when the
  thread runs again; the dispatcher has run the wait's completion before re-dispatching, exactly as it does before
  re-dispatching an unwound thread, and the stub then returns to its caller with ctx->pc = ra (where the generated code
  would resume). Each caller returns at once after the call (CD.cpp twice, MPEG.cpp twice; WaitVSyncTick and DelayThread
  end there). `eeWaitVSyncTicks` (a completion that sends the thread to a pc other than its caller's: the boot's letter
  post, the replay tool) keeps the exception (`inPlace = false`). Off a carrier nothing changed.
- **Guest message boxes: none became host queues.** The plan allowed a host queue only where every sender and receiver
  is a carrier. The check runs counted every iosSendMsg / iosRecvMsg by box and by thread (`[b2box]`): there are four
  live boxes, and each has an uncarried thread at one end:

  | box | senders | receivers | ride7 600 s sends / receives |
  |---|---|---|---:|
  | 0x26e9d0 (the GP box) | t9, t10 (and t6 / t13-t16 at the boot) | t8 (B1, carried; a poll) | 20 954 / 35 499 |
  | 0x26d9a8 (the frame box) | t9 | t10 | 19 167 / 20 904 |
  | 0x1fc870 (the idle queue) | **t11** (carried: its callees queue idle work, ~0.6 a job) | t5, the idle thread (polls) | 724 / 10 718 |
  | 0x121af50 | t9 | t12 (memory card) | 25 / 26 |

  t11's own synchronisation is SleepThread / WakeupThread and the queue's semaphore; the one box it uses is received by
  the idle thread. They stay guest boxes until B3 carries the game, GP and idle threads.

## 4. Why gameplay stays exact

- **The same operations in the same order.** Section 2's body issues the generated loop's guest-visible operations one
  for one and charges the same EE cycles: 8 a call through the dispatch checkpoint (the NOCHK ones charged untested,
  as the generated code does), 32 a back edge, at the same 13 back edges (to 0x115e5d4, 0x115e638, 0x115e770, 0x115eab0,
  0x115e5a0, 0x115e5b0).
- **The same scheduling decisions.** Every decision is the dispatcher's: the carrier only runs while the dispatcher
  would have run t11's own context and hands the baton back exactly where the generated code would have unwound (a
  checkpoint that must reach the scheduler, a park: SleepThread, WaitSema, sceCdSync / sceCdStRead's waits). The
  dispatcher then does what it does after an unwind (events, interrupt handlers, the reschedule, other threads, the
  wait's completion) and resumes the carrier where it would have dispatched t11 again.
- **What is not the same: the dispatcher's own charge after a park.** After an unwind the generated code goes on by
  dispatches: the resumed function runs from its label, returns to run() (its caller's host frame is gone), and run()
  dispatches the caller at its return label, each dispatch charging the dispatch checkpoint's cycles. On a carrier the
  host frames are still there and none of those re-dispatches happen (B1's t8 is the same: one fewer a park). After
  GAMECORE's entry EE time follows the host clock (host clock sync), so this is the same order of change as host timing
  jitter and changes no operation; before GAMECORE (executed cycles, the time slice) it would move the heap order,
  which is why both switches take effect only once GAMECORE runs and why t6 is not carried (section 8).
- **Handing back is exact.** At any yield with the switch off (the A/B window), and at an in-place wait the
  dispatcher answers "unwind" to, the carrier unwinds there (the exception, or the generated code's return path) with
  ctx->pc on a resume label of manager_sub_0115E4C0 or of the callee it was in, the registers the generated code expects,
  and t11 goes on as generated code until its next park at 0x115e5a0, after which the dispatch at 0x115e5a8 takes it
  over again.

## 5. Checks

Build p2, Vita3K 222 MHz, muted, `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B1SOUND_CHECK=1 SOTC_B2LOAD_CHECK=1` (the check turns the
watchdog on at 10 s; both carriers on, so the table runs two carried threads at once):

| run | frames | t11 jobs | in-place waits | executor audit (both carriers) | kept registers | hand-backs | B1: carrier ticks = VBlanks, VBlank count = sound ticks | watchdog |
|---|---:|---:|---:|---|---|---|---|---|
| ride7 600 s (chkR) | 19 392 | 1 192 | 3 996 | 0 bad of 333 521 | 0 bad | 0 | 31 of 32 windows; 35 499 = 35 499 | silent |
| attract 400 s (chkA) | 19 027 | 478 | 2 031 | 0 bad of 305 051 | 0 bad | 0 | 30 of 31; 22 946 = 22 946 | silent |
| lake6 400 s (chkL) | 12 663 | 378 | 1 799 | 0 bad of 249 758 | 0 bad | 0 | 20 of 21; 24 000 = 24 000 | silent |

One take-over a run (claims 1), the body never unwound (0 hand-backs of any kind), every job type but two seen (ride7: by
type 170 151 145 465 33 29 0 21 0 152 14 5 5), no missing target, no `[b1dog]`. Every run went to its end: Wander mounted
Agro on ride7 (frame 2 217), lake6's teleport (frame 2 000) and the boss arena, the attract demo past the title and its
loads. (The window that differs in B1's column is the first one, as in B1.) The carrier's time is 80-135 us a frame on
average, up to 1.4 ms in a 600-frame window of loads (the whole job, CD waits' hand-overs included).

The check found one bug before these runs (smoke run smR on p1): case 8d0's `lui v1, 0x1b; addiu v0, v1, 0x7b60` carry a
relocation, and the generated code reads their immediates from the code at run time (`_dynInsn`); the body had the
captured image's constants, called address 0 and the thread died (`[guest-branch:missing-target] source=0x115e8e4`). The
body now reads them from the code too (the other two such instructions, 0x115e4f0 / 0x115e59c, set s6 / s5 before the
loop, which the kept-register check covers).

**Per-thread work counters** (`[b2work]`: calls by guest thread; on = the check runs / ldOn*, off = ldOff*, b1sound on in
both; t11's columns):

| | iosSendMsg | iosRecvMsg | sceCdRead | sceCdSync | reads per sync |
|---|---:|---:|---:|---:|---:|
| ride7 360 s on (ldOnR) | 1 265 | 0 | 2 608 | 2 625 | 0.994 |
| ride7 360 s off (ldOffR) | 1 039 | 0 | 2 053 | 2 070 | 0.992 |
| lake6 400 s on (ldOnL) | 216 | 0 | 1 817 | 1 835 | 0.990 |
| lake6 400 s off (ldOffL) | 438 | 0 | 1 831 | 1 849 | 0.990 |

The same shape (17-18 syncs a run without a read: the polls before the first read); lake6's read counts on / off are
within 1%, ride7's follow the route (the mount came at frame 3 105 on, 7 015 off: the ride covers different ground in the
same 360 s). No other thread does a CD read.

## 6. Loads and hitches

`SOTC_FRAME_LOG_ALL` runs on / off (b1sound on both ways, SOTC_B2_COUNT on both ways), tools/loads.py: the boot, the title,
Load Game and the first area are frames 0-1999; the teleport (an area load) is at frame 2 000.

| run | window | longest frame | frames over 100 ms | over 200 ms |
|---|---|---:|---:|---:|
| ride7 on | 0-1999 | 404.5 ms (155, the boot) | 10 | 6 |
| ride7 off | 0-1999 | 403.6 ms (147) | 12 | 7 |
| ride7 on | 2000-2599 (teleport) | 170.5 ms (2 000) | 1 | 0 |
| ride7 off | 2000-2599 | 173.1 ms (2 000) | 1 | 0 |
| ride7 on | whole run | 404.5 | 11 | 6 |
| ride7 off | whole run | 403.6 | 14 | 7 |
| lake6 on | 0-1999 | 408.1 (155) | 9 | 6 |
| lake6 off | 0-1999 | 407.6 (154) | 10 | 5 |
| lake6 on | 2000-2599 | 85.6 | 0 | 0 |
| lake6 off | 2000-2599 | 78.6 | 0 | 0 |
| lake6 on | whole run | 408.1 | 9 | 6 |
| lake6 off | whole run | 407.6 | 10 | 5 |

No hitch added: the longest frames are the same frames within 1 ms, the counts over 100 ms are equal or one fewer with
b2load on. The sums of the first 2 000 frames differ (ride7 51.3 / 49.5 s, lake6 49.5 / 46.5 s): in both the extra is in
frames 1 750-1 850, where the game enters a phase of ~50 ms frames (the Load Game load behind a fade) about 25 (ride7) /
100 (lake6) frames earlier with the switch on; the frames themselves are the same length. Single runs on a loaded
machine (A8's three Vita3K runs alongside); the console decides the load times.

## 7. Measurements

Vita3K 222 MHz, 3 cores, interleaved A/B of `b2load` (`SOTC_AB_FLAG=b2load SOTC_AB_PERIOD=1`, `SOTC_B2LOAD=1 SOTC_B1SOUND=1`
in the env: b1sound on in both halves), off - on (positive: on faster), 2 SE:

| run | frame | busy | take-overs / hand-backs |
|---|---:|---:|---|
| ride7 800 s seed 7 (mixR7) | +0.049 +- 0.129 | +0.065 +- 0.120 | 385 / 384 by the switch + 84 at an in-place wait |
| ride7 800 s seed 13 (mixR13) | +0.053 +- 0.129 | +0.074 +- 0.116 | 479 / 479 + 104 |
| lake6 400 s (mixL7) | -0.073 +- 0.141 | -0.074 +- 0.143 | 106 / 105 + 38 |
| **pooled** (mean of 3, SE = sqrt(sum se^2) / 3) | **+0.010 +- 0.038 ms (1 SE), z 0.3: even** | +0.022 +- 0.037 | |

Even, as expected: t11 runs 10-300 us a frame riding; what the carrier saves is its unwinds and re-dispatches at each
park (~0.1-0.4 a frame riding), which the A/B cannot resolve. The A/Bs exercised the hand-back at both kinds of yield
(the switch flipped at a park or a checkpoint: 968 hand-backs; and at an in-place CD wait, where the dispatcher answers
"unwind" and the carrier throws after the wait: 226) with no freeze, no missing target and t11's job count going on.

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, both switches on, ride7 600 s, shots every 120 s, build p2): booted, rode the 600
s (frames 3 766 / 6 930 / 10 237 / 13 427 / 16 979 at 120 / 240 / 360 / 480 / 600 s, 24-37 game fps; mounted at frame 12 460
this time, the route's timing), system free 67.2 MB at the end, 512 jobs, 2 214 in-place waits, 0 hand-backs. Shots at 60 /
180 / 300 / 420 / 540 s looked at: Wander and Agro by the shrine's steps, then riding the plain; ground texture, Wander,
Agro and their shadows normal. **89 MB attract** (attract 300 s, both on, shots every 60 s): past the title into the demo and its area loads (225 jobs, 1 456 in-place waits, 0 hand-backs), 15 541 frames, system free 67.2 MB; the shots (the title's fly-over of the bridge in letterbox, Agro galloping past the cliffs) normal.

**spalign**: `python3 ../tools/spalign.py p2.elf`: 0 sp-based :128/:256 NEON accesses (of 1 038 with a hint).

**Default.** Off (`SOTC_B2LOAD` must be 1-3): exact by the checks, even in Vita3K, and it carries a thread whose waits are
the disc; the console's loads decide. Console A/B package: `build/agent-scratch/b2/celosia-b2.vpk` (= p2) + `env-ab.txt`
(N6's console env without its A/B lines + `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_AB_FLAG=b2load SOTC_AB_PERIOD=300 SOTC_B1_LOG=600
SOTC_B2_LOG=600`). It needs b1sound's audio heard first (B1's package), since b1sound is on in both halves.

## 8. Why t6 is not carried, and what B3 needs

- **t6.** The boot loader runs only before GAMECORE's entry. There the EE clock moves by executed cycles alone (host clock
  sync starts at GAMECORE's entry) and the time slice between t6 and the main thread (both priority 45) decides the order
  of the heap allocations that place the XFF modules (docs/native_n5b.md). A carrier removes the dispatcher's
  re-dispatches after each park (section 4), so the slice would end at other checkpoints, the heap order could change
  and GAMECORE's modules would move (the recompiled code has them where the capture put them). So t6 stays generated code;
  its body (120 instructions, straight-line, no loop) is a small job once the boot's clock or the module placement no
  longer depends on dispatch charges (B3: the dispatcher's charges themselves go away, or the boot keeps them as charged
  cycles).
- **B3 (the game, GP and idle threads; the kernel objects).** With t8 and t11 carried, the parks that still unwind are the
  game thread's (t9), the GP thread's (t10), the idle thread's (t5) and t4's. Carrying them makes every box a host-queue
  candidate (0x26e9d0, 0x26d9a8 and 0x1fc870 then have carriers at both ends), and once every live thread is carried the
  EE semaphores / sleeps / wakes and the message boxes can be host primitives used by the carriers directly, the
  dispatcher reduced to choosing which carrier holds the baton, and the generated checkpoints removed (a carrier only
  needs to yield at a block). The carrier table takes four entries today (kMaxCarry): B3 needs it bigger, and the
  in-place waits already cover the CD / stream / VBlank / delay waits.
- **The callees.** t11's jobs (sheetFileLoadSegmentIndex, sheetFileLoadMap, the sheetFs* loaders, the CD helpers
  0x1ac9c8 / iosCdStRead / LoaderSysFRead) are still generated code called from the body: phase C's loader area.
