# Round S17: the EE scheduler's dispatch nested at the checkpoint (SOTC_S17IOP, off by default)

Redesign round S17 (2026-11-19). Scripts, runs and logs: `build/agent-scratch/s17/` (clone.sh the build dir cloned from
S16's, b2.sh the builds, chk17.sh the in-run check / the profile, abs17.sh the s17iop interleaved A/B, qc.sh short
sanity runs, q1 / q2b / q5 the queues, tools/pcname.py the profile's pcs named). Own Vita3K trees ../vita3k-celosia-s17
and -s17b (222 MHz, 3 cores, muted), deleted at the end. Builds p2 (the A/Bs, the boot test) and p3 (the same with the
check's turns at random, the checks); p1 was a broken clone (below).

## 1. What the game thread's unwinds were, counted

S15 (not merged, branch wip/s15-sndtick) saw the game thread unwinding at ~25 "pending" checkpoints a frame riding and
thought the IOP's replies waking the streaming / loader threads were the cause. S17 records every *episode* in which the
game thread (thread 9, the one that runs the frame hook) is taken off by a checkpoint: from the checkpoint to the game
thread's next dispatch at its own context, with the threads run, the handlers queued and every kernel call in order
(`SOTC_S17IOP_LOG=<frames>`: `[s17]` lines; also with the switch off).

Riding (ride7, Vita3K 222 MHz, offRp2: S17 off, mounted at 2 425):

| per frame | S17 off (offRp2) | S17 on (onRp2) |
|---|---:|---:|
| episodes (the game thread taken off by a checkpoint) | 1.02 | 1.03 (all nested) |
| host time of an episode | 394-2 184 µs (576 at frame 7 200) | 46-76 µs |
| checkpoints that found the pending flag ("unwinds", `[s11]` pending) | 72-131 (91 at frame 7 200) | 0.2-0.6 |
| slow checkpoints | 85-146 | 14.6-17.5 |
| the game thread's dispatches at its own context | 4.2 | 2.3 |

- **There is about one episode a frame, not seven or twenty-five**: the GS FINISH interrupt handler (kernel_sub_001C37F0:
  the finish counter at 0x1e1158, the callback at *0x1e116c, CSR.FINISH cleared) arriving while the game thread polls
  for it (iosWaitGPFinish, 0x1c3788: the counter compared, isysSleepSubThread between polls). 90-94% of the episodes are
  that handler alone; the rest add a woken thread: 11 (kernel_sub_001AC9C8+0x124, the CD streaming loop), 8 (the VBlank /
  sound thread's loop at kernel_sub_001C80A0+0x68 and iosRecvMsg), 4 (kernel_sub_001B2CA0+0x68), 10 (iosSendMsg+0x11c),
  the VBlank handler 0x1c8180 (~0.06 a frame). The IOP replies (`flags 6` / `flags 7`, ~3 / ~3.3 a frame) and the
  threads they wake (4 / 8 / 10 twice a frame, 5 / 9 once, 11 0.2-0.3) are taken almost entirely while the game thread
  waits for the render thread (~15 ms a frame): they do not reach its checkpoints.
- **What made the unwinds expensive is the old path itself**: each one-handler episode left the game thread's host stack
  unwound and the game thread re-entered through the dispatcher in the middle of its sub-thread coroutine (the game's
  isysWake/SleepSubThread, decomp_subthread.cpp) and of execScript (thread 9 resumed at execScript+0x2c ~0.07 a frame):
  the pending flag was met again by ~70-130 checkpoints a frame on the way, and an episode took 0.4-2.2 ms of host time.
  Nested, the same episode is the handler's run (~45-75 µs) and the game thread's frames go on.

## 2. What S17 changes (`SOTC_S17IOP`, A/B `s17iop`)

`EeScheduler::s17Checkpoint` / `s17Nest` (EeScheduler.cpp): a checkpoint of the game thread that would unwind (the
pending flag, a due deadline, a slice end with a ready thread) runs run()'s dispatch loop right there, on top of the game
thread's host frames: the events taken where run() takes them, the interrupt handlers queued on the game thread as run()
queues them, the woken threads picked by priority until they park, the game thread put back at the front of its queue
(behind it at a slice end); when the loop would pick the game thread at its own context again, the checkpoint returns
false and its frames go on. The shared VU0 registers are handed over as run() hands them; a nested thread's own
checkpoints and parks unwind to the nested loop as they unwind to run(); the frames below that cannot return early
(m_xferThrowDepth) and the guest jump serial are the game thread's own and restored after.

Not nested (the old unwind): any thread but the game thread (`g_ps2xS17Home`, set each frame by the frame hook; nothing
before the first frame), inside an interrupt handler or invocation, a posted external wake (a CD read's completion)
whose waiter has not parked yet (S15's boot hang), less than 192 KB of host stack above the floor. When the game thread
stops being ready inside the nested loop (suspended, terminated) or the emulator stops, the loop hands over to run() by
unwinding then. Differs from the old path only in the dispatcher's hops on the way back up the game thread's guest call
chain, which are not taken (8 EE cycles each are not charged).

Switch: `SOTC_S17IOP=1` (kS17Iop, after kS13Mot; self-gated in s17iop.h like s13mot.h); `SOTC_S17IOP_CHECK=1` the check;
`SOTC_S17IOP_LOG=<frames>` the `[s17]` lines; `SOTC_S17IOP_PARTS` (debug): 4 events only (no handler or thread nested),
8 handlers but no thread.

## 3. Equivalence (`SOTC_S17IOP_CHECK=1`, p3)

Each game-thread episode goes the old way or nested at random (xorshift: p2's alternation aliased with the events' own
order, see below) and both paths record its kernel calls in order as an order hash: threads made running, parks (reason,
thread), semaphore signals, wakeups, threads made ready, interrupt handlers queued (handler pc), invocations run, threads
made dormant. The same kinds of episode must come out with the same orders in both paths, in the same proportions.

| run (p3) | where | episodes | in orders both paths produced | only old / only nested (orders seen 1-3 times) | the commonest orders, old / nested |
|---|---|---:|---:|---:|---|
| chkRp3 | ride7, mounted, to frame 10 800 | 11 451 | 11 401 (99.6%) | 22 / 28 | 5 265 / 5 152 (the handler alone), 169 / 174, 71 / 76, 71 / 66, 58 / 70 |
| chkLp3 | lake6, to frame 8 400 | 8 997 | 8 946 (99.4%) | 31 / 20 | 3 995 / 3 956, 114 / 126, 103 / 103, 100 / 88, 66 / 79 |
| chkAp3 | attract, to frame 9 600 | 10 204 | 10 173 (99.7%) | 14 / 17 | 4 787 / 4 736, 219 / 207, 40 / 39, 37 / 32 |

Every order that occurs more than a few times occurs on both turns in the same proportion (within the counts' noise);
the ones seen only on one side are seen 1-3 times in the whole run. The per-frame kernel calls inside the episodes match
too (ride7: runs 0.07 / 0.05, parks 0.04 / 0.03, wakes 0.06 / 0.04, readies 0.04 / 0.03, handlers 0.04 / 0.03, invocations
0.54 / 0.51 a frame for ~0.52 / 0.50 episodes). No bail-out (the game thread never stopped being ready inside a nested
loop), no posted wake found unparked, the host stack never short.

p2's check (the old / nested turns alternating): 12 132 episodes riding, 99.2% of them in orders both paths produced,
but the multi-thread orders mostly on the old turn (e.g. 211 / 41): the alternation lined up with the events' periodic
order (a handler-only episode, then one with a thread). Hence p3's random turns.

## 4. Speed (interleaved A/B, Vita3K 222 MHz, off - on, p2)

Per-frame interleaved (SOTC_AB_PERIOD=1, mixframes.py, 300-frame blocks, ± 2 SE; ride7 from the mount + 600):

| run | where | off - on, frame | busy |
|---|---|---:|---:|
| mixR7p2 (400 s) | ride7 seed 7, 6 blocks | +0.210 ± 0.423 ms | |
| mixR7p2 (800 s) | ride7 seed 7, mounted at 5 060, 32 blocks | +0.156 ± 0.168 ms | +0.174 ± 0.176 |
| mixR11p2 (800 s) | ride7 seed 11, mounted at 3 414, 37 blocks | -0.005 ± 0.250 ms | +0.072 ± 0.244 |
| mixL7p2 (400 s) | lake6 seed 7, 18 blocks | -0.142 ± 0.191 ms | |
| mixA7p2 (400 s) | attract seed 7, 35 blocks | -0.008 ± 0.313 ms | |

Not z > 2 riding in either seed: **off by default**.

A caution on what the per-frame A/B can see: the old path's long episodes (0.4-2.2 ms, ~90 pending checkpoints a frame)
appear only in whole runs with S17 off (offRp2); in the check runs, where half of the episodes are nested, the old turn's
episodes take 52-67 µs and the pending checkpoints are ~1 a frame on both turns. The expensive state seems to need several
old-path episodes in a row, so a per-frame A/B may hide it. A block A/B (SOTC_AB_PERIOD=150: on or off for 150 frames at a time, blkR7p2, ride7 seed 7, 800 s,
1 200-frame blocks) did not settle it: +0.407 ± 1.063 ms, the off windows drew more (1 381 vs 1 280 draws a frame), so
the route's scenery, not S17, dominates at that granularity.

## 5. Boot, pictures, alignment

bootRp2 (p2, S17 on, `SOTC_VITA_EAT_USER_MB=89` taken): booted, mounted at frame 5 119 and rode to 11 178 in 600 s, no
fault; the window shots (riding Agro past the bridge, the plain) as before. spalign105.py: 0 sp-based :128 / :256 NEON
accesses (of 975) in p2 and p3.

The first build (p1) did not get past the title in any run, S17 on or off: the build dir cloned from S16's kept S16's
objects of every file that includes sotc_decomp.h / ee_scheduler.h, because S16 had compiled them after this worktree's
edits (ninja saw the headers as older). p2 was built after touching the changed files; a clone must touch what differs
to the clone's time.

## 6. What it leaves

- Whether the old path's expensive state (offRp2's ~90 pending checkpoints and 0.4-2.2 ms episodes a frame) costs the
  game thread frame time riding: neither A/B could show it, the whole runs are too noisy to compare. A longer block A/B
  (period >= 600, 1 600 s, both seeds) or the console's A/B (env-console: s17iop) is the next measurement; the switch
  is exact by its check, so it can be tried there as it is.
- Why the old path re-enters ~90 times: the game thread resumed through the dispatcher in the middle of its sub-thread
  coroutines (decomp_subthread.cpp) and execScript, polling iosWaitGPFinish; not traced.
- The IOP replies and the woken streaming / loader / sound threads (4, 5, 8, 9, 10, 11) are not on the game thread's
  path riding (they run while it waits for the render thread): making them host threads or host code is about the
  console's single core time, not the game thread's unwinds.
