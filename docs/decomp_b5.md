# Round B5: the memory-card load stall (2026-11-23)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B5. Base: main 64d0d0c (B4 rebased on A9). No new switch: the
fix acts with `SOTC_B3THREADS` (b3threads, off by default) and has an env override `SOTC_B5_DEFER=0 / 1` for comparisons.
Code: PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp (`parkCurrent`, `syncCyclesToHostClock`, `processPendingEvents`, `idleWait`,
the debug ring and `b5Dump`), include/runtime/ee_scheduler.h (`g_ps2xB5Defer`, `EeB5Stats`, `g_ps2xB5Trace`), game/overrides/
b3_threads.cpp (the switch per frame, the `[b5]` line, the idle-thread watchdog `[b5dog]`), sotc_native.cpp (sotc_vita_elf_room
112 KB). Scripts, runs and logs: `build/agent-scratch/b5/` (NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed
at b5; 222 MHz, 3 cores, audio-volume 0; every run muted).

## 1. The bug as found at the merge

One 89 MB ride7 boot on the merge build (bootMon, b1sound + b2load + b3threads + b4fast) sat on the memory card's "Loading
data" screen for the whole 600 s (28 800 frames, menu animating); `[b3threads]` showed the idle thread t5's turns frozen at
2 848 and `[b4fast]`'s picks by thread showed, in every 600-frame window from frame 1 800 to the end, **t12 (the memory-card
manager) picked exactly 1.00 times a frame, t5 0.00, t11 (the loader) 0.00**, and "generated 1.000" (a carrier's loop
giving the baton back to the executor once a frame to run t12).

## 2. Instruments

- `SOTC_B5_DOG=<s>` (debug, off unless set): once a frame, from the frame's end on the host thread holding the baton (so the
  records are consistent), when t5 has had no turn of its loop for that long (its picks when it is not carried): a `[b5dog]`
  dump: every thread's record (status, wait, wakeups, invocations, pcs, carrier and its site / pin / turn / the Vita
  semaphores' counts), the ready queues, the baton's owners (active carrier, the carrier the executor waits on, the returner),
  every semaphore with a waiter plus the kernel message semaphore and the callback slot's, the idle queue's box and its
  senders. `SOTC_B5_DUMP_AT=<f,...>` / `SOTC_B5_DUMP_EVERY=<from>,<to>,<step>` force dumps.
- `SOTC_B5_TRACE=1` (debug): a 2 048-entry ring of the scheduler's decisions, written by whichever host thread holds the
  baton and printed with each dump (`[b5trace]`): picks, enqueues (front / back), parks (reason), made-ready (by whom),
  invocations pushed (handler pc) / popped, generated dispatches (pc), carrier loops opened / given back (reason) / gone on,
  direct hand-overs, yields to the executor, events (type), the executor's idle waits; each tagged with the host thread
  (`x` the executor, `cN` the carrier of table entry N). `tr.py` (b5/) merges the rings into a per-frame table.

## 3. What the dumps and the trace show

The first dumps (p1, the 5 s watchdog, all four on) caught the starvation as it happened: **t5 and t12 both Ready**, both with a
completed interrupt invocation on top (pc 0: a handler had run on them and they had been preempted right after), t9 running,
`ready: t12@47 t5@125`, the baton consistent (one owner, every carrier's Vita semaphore at 0, nothing pinned wrong), no
semaphore with a lost waiter, the idle box empty. Between frame 1 007 and 1 977 of one run t12 was picked 972 times and t5 four
times. No lost wake, no lost turn: both threads sat in the ready queue and were picked.

The trace says what each pick did (p2, b4fast, the memory-card phase; one frame, every frame alike):

```
c5K9:1 c5N9:1 c5V0:1000000 c5V0:2000000 c5E12:1 c5B9:5 c5X9:1        t9 parks (frame box), its loop opens: VBlank start + end
                                                                       taken there; t12 next: generated, back to the executor
xP12:1 xO12 xI12:1c8180 xG12:1c8180 xY8:c xE8 xE12:1 xP8 xR8          executor picks t12, pops its old handler, pushes the
                                                                       VBlank handler 0x1c8180 onto it; the handler readies t8;
                                                                       t12 preempted (front of its queue) before any own code
c0Y4:8 ... c2Y10:4 ... c4Y4:a ... c2Y9:4 ... c4P9 c4H9:a                t8 -> t4 -> t10 -> t4 -> t9 released at once (its frame
                                                                       box message was already there at t10's VBlank turn)
```

- **Why the VBlank is always due exactly at t9's park.** After GAMECORE the EE clock follows the host clock, but the catch-up
  (`syncCyclesToHostClock`) is done only at parks and in the idle thread's idle wait; between them EE time moves by the cycles
  the guest code charges, far slower than host time. While t9 runs its frame alone no thread parks, so a VBlank that falls due
  mid-frame is seen only at t9's park, when the frame box message is already sent: the VBlank chain (t8, t4, t10's VBlank
  turn, t4) releases t9 at once. Then the one thread that runs between t9's park and the VBlank chain is the next ready one,
  and the pending VBlank handler is pushed onto it at its first dispatch (as run() does for any pending invocation and any
  picked thread); the handler readies t8 (priority 40), which preempts it.
- **The livelock.** In the menu, at frames just over one VBlank, that repeats every frame: t12 (priority 47) is dispatched
  once a frame, runs only the VBlank handler, and never executes an instruction of its own; t5 (125) never gets the CPU at all;
  the loader t11 waits for work that t12's memory-card result would start. A frame escapes only when t9's frame ends before
  the next VBlank (no catch-up due at its park): then t12 runs a step. The load needs a number of t12 steps; the stalled run had
  no escaping frame in 28 000.
- **It is not b3threads' or b4fast's.** The same per-frame analysis of the memory-card phase (ride7, 89 MB, dumps every 12
  frames from 600 to 1 200, the rings merged):

| run | switches | frames with a VBlank taken at t9's park | t12's phase (first..last pick) | frames t12 picked | of them handler only | frames t12 ran own code |
|---|---|---:|---|---:|---:|---:|
| oD1 | none (the generated path) | 323 of 610 | 683..888 | 200 | 187 | 13 |
| oB1 | b1 + b2 + b3threads | 319 of 600 | 675..894 | 205 | 192 | 13 |
| oA1 | b1 + b2 + b3threads + b4fast | 351 of 606 | 671..928 | 235 | 225 | 10 |

  The default path livelocks the same way and escapes the same way (13 frames of own code in ~200); with the carriers the
  frames are a little longer and the escapes a little rarer, which is all the switches change. The stall (1 in 7 boots with
  b3threads on at the merge) is the tail of this distribution: a run whose menu frames never end before the next VBlank.
- The decisions are run()'s in all three: each step above (the event taken at the loop's top, the pending invocation pushed onto
  the picked thread, the preemption, the queue order) is what run() does at the same point; b4fast's `bail 5` hands t12 to the
  executor with the scheduler's state exactly as run() would have it. What makes the livelock is when the VBlank is seen.

## 4. The fix: the park's catch-up after the lower-priority thread's turn

`EeScheduler::parkCurrent`, with `g_ps2xB5Defer` (set once a frame by b3_threads.cpp: on with b3threads once GAMECORE runs;
`SOTC_B5_DEFER=0` off, `=1` on whatever the switches):

- A thread that parks while **no other thread of its priority or higher is ready** and whose **catch-up would make a deadline
  due** (host-clock cycles >= the next deadline's cycle, which the cycles run had not reached) leaves the catch-up pending
  instead of doing it there.
- The pending catch-up is done by the next `syncCyclesToHostClock`: the next park that does not defer (the lower-priority
  thread's own park when a higher thread is ready again, or the chain's end), the idle thread's idle wait (always the floor: t5
  syncs after one poll; `idleWait` then returns at once and the next checkpoint takes the deadline), or after 4 ms by the first
  event processing (`processPendingEvents`' slow part: a lower thread that runs on without parking).
- (Build p10.) In p8 / p9 the idle wait did the catch-up only after its wait, and its wait looks only at host deadlines still
  ahead, so an overdue VBlank made it wait the full 1 ms: a frame of gameplay paid ~1.5 ms (the lake6 A/B of b3threads, section
  7). p10's `idleWait` does a pending catch-up first and returns.
- So at t9's park the next thread (t12, else t5) runs its turn first, and the VBlank is taken on the next sync point, after it:
  on the PS2 the VBlank would have hit t9 mid-frame and t12 would have had the time up to the next one.

**Why it stays exact in the sense of decomp_b1-b4**: every scheduling decision is still run()'s, in run()'s order, given the
events; what moves is the EE time at which a host-paced deadline (VBlank) is seen as due: later by the lower-priority thread's
turn (a park or an idle-wait poll: microseconds to a few ms, 4 ms at most), the same kind of change as host timing jitter, which
after GAMECORE's entry already decides when every VBlank is seen. No guest operation changes. The deferral never applies when a
thread of the parking thread's priority or higher is ready (the VBlank chain's own parks, t8 / t10 / t4, sync as before) or when
no deadline would fall due. It acts only with b3threads on, so the default path is untouched (it has the same livelock exposure:
oD1 above; `SOTC_B5_DEFER=1` applies the rule there for a later decision).

**Effect** (the same dense-dump analysis, build p8/p9):

| run | switches | frames with a VBlank at t9's park | t12's phase | frames t12 picked | handler only | own code | menu fps (5 s windows) |
|---|---|---:|---|---:|---:|---:|---|
| oA1 (before) | all four | 351 of 606 | 671..928 (258) | 235 | 225 | 10 | 51 47 46.6 46 44 |
| wA1 (after) | all four | **0 of 600** | 661..790 (130) | 82 | 3 | **79** | 48 44.8 43.8 48.4 44.4 |
| oB1 (before) | b1 + b2 + b3 | 319 of 600 | 675..894 (220) | 205 | 192 | 13 | 51.6 48.6 42.8 49.4 44.6 |
| wB1 (after) | b1 + b2 + b3 | **0 of 601** | 681..807 (127) | 73 | 1 | **73** | 50.4 44.4 45 48 44.2 |

`[b5]` (wA1): 1.1-1.7 deferred park catch-ups a frame, 0.07-0.17 a frame done by the 4 ms limit. A first form (p7) deferred at
every park with only lower-priority threads ready, without the "a deadline would fall due" test: it also removed the livelock
but deferred ~8 catch-ups a frame (t10's park before t9's frame among them), 1/frame by the 4 ms limit, and cost menu fps (36-41
vs 46-49); the narrow form keeps the menu's frame rate.

## 5. The rate before and after

**Before** (no fix): the stall itself, 1 in 7 boots with b3threads on at B4's merge (bootMon). This round's 600-1 200 windows
(the memory-card phase) show the livelock regime directly (t11 0 picks, t12 ~0.9 a frame, t5 ~0.15; the field reached late):

| build / switches | runs | livelock regime in 600-1 200 | field reached at (first teleport) |
|---|---:|---:|---|
| pm / p1 / p2 / p6, all four on | 9 | **6** (dA2 dB2 tA1 tA2 tA3 fA1) | 2 000-2 265 (2 179 / 2 265 / 2 210 / 2 062 in the livelocked ones) |
| p2 / p6, b1 + b2 + b3threads | 5 | **1** (sB2) | 2 000 |
| p3-p6, all off (default) | 4 | 0 | 2 000 |

None of these 18 stalled for good (the stall is the tail: every frame over the next VBlank for the whole run); all of them
livelocked or not depending on the menu's frame times.

**After** (p9, the fix): 18 boots, 89 MB, ride7 300 s, b1sound + b2load + b3threads + b4fast on (pf1-pf18, three trees):

| | runs | stalled | livelock regime in 600-1 200 | field reached | mounted Agro | dismounts | watchdog lines |
|---|---:|---:|---:|---|---:|---:|---:|
| pf1-pf18 | 18 | **0** | **0** | frame 2 000 in all 18 | 17 (frames 2 222-6 967; pf8 not in 300 s: the route's steering) | 0 | 0 |

The 600-1 200 window in every one of the 18: t5 1.28-1.36 picks a frame, t11 1.40-1.51, t12 0.30-0.33 (t12's phase over inside
the window), against t5 ~0.15, t11 0.00, t12 ~0.89 in the livelocked runs before.

## 6. Checks

Build p9, Vita3K 222 MHz, muted, all four on with all checks (`SOTC_B1SOUND_CHECK=1 SOTC_B2LOAD_CHECK=1 SOTC_B3_CHECK=1`: the
executor audit for every carrier and every host thread running a loop, kept registers, per-box / per-semaphore counts, the B1
watchdog at 10 s) and `SOTC_RIDE_PROBE=1`:

| run | frames | executor audit | kept registers | hand-backs | B1: windows ticks = VBlanks; VBlank count = sound ticks | B2 jobs / in-place waits | `[b5]` a frame: deferred, done, by the 4 ms limit | watchdog |
|---|---:|---|---|---|---|---|---|---|
| ride7 600 s (chkR) | 17 400 | 0 bad of 438 015 | 0 bad | 0 | 28 of 29; 35 396 = 35 396 | 890 / 3 055 | 1.27, 1.02, 0.037 | silent |
| attract 400 s (chkA) | 19 200 | 0 bad of 444 713 | 0 bad | 0 | 31 of 32; 23 946 = 23 946 | 516 / 2 184 | 1.48, 1.04, 0.072 | silent |
| lake6 400 s (chkL) | 12 000 | 0 bad of 361 556 | 0 bad | 0 | 19 of 20; 23 605 = 23 605 | 380 / 1 801 | 1.24, 1.02, 0.045 | silent |

The same on p10 (the final build, + the idle wait's prompt catch-up):

| run | frames | executor audit | kept registers | hand-backs | B1 windows; VBlank count = sound ticks | B2 jobs / waits | `[b5]` deferred, done, 4 ms limit, idle wait | watchdog |
|---|---:|---|---|---|---|---|---|---|
| ride7 600 s (chkR2) | 18 600 | 0 bad of 499 080 | 0 bad | 0 | 30 of 31; 35 978 = 35 978 | 1 025 / 3 316 | 1.17, 1.02, 0.032, 0.98 | silent |
| attract 400 s (chkA2) | 19 200 | 0 bad of 451 281 | 0 bad | 0 | 31 of 32; 23 800 = 23 800 | 467 / 2 034 | 1.37, 1.03, 0.047, 0.97 | silent |
| lake6 400 s (chkL2) | 12 600 | 0 bad of 364 079 | 0 bad | 0 | 20 of 21; 24 244 = 24 244 | 365 / 1 782 | 1.20, 1.02, 0.037, 0.98 | silent |

p9 **0 bad of 1 244 284**, p10 **0 bad of 1 314 440**; 0 hand-backs; the first B1 window differs as in B1-B4 (the take-over's frame). chkR mounted at 5 828 and
rode to the end (0 dismounts; chkR2 at 3 796); lake6's teleport and the boss arena; the attract demo past the title (19 200 frames).

## 7. Measurements (the fix is in `parkCurrent`: on the hot path)

Vita3K 222 MHz, 3 cores, interleaved A/Bs (`SOTC_AB_PERIOD=1`), off - on (positive: on faster), 2 SE; pooled = mean of 3,
SE = sqrt(sum se^2) / 3. b5's deferral follows b3threads frame by frame, so the b3threads A/B carries the fix's cost too.

**b3threads, b4fast on in both halves** (`SOTC_AB_FLAG=b3threads`; B3 -0.174 +- 0.055, B4 -0.150 +- 0.040):

| run | p9 (idle wait still waited 1 ms) | p10 (final) |
|---|---:|---:|
| ride7 800 s seed 7 (mixR7t / mixR7tb) | (stopped) | +0.354 +- 0.163 |
| ride7 800 s seed 13 (mixR13t / mixR13tb) | (stopped) | +0.210 +- 0.187 |
| lake6 400 s (mixL7t / mixL7tb) | **-1.560 +- 0.179** | -0.053 +- 0.174 |
| **pooled (p10)** | | **+0.170 +- 0.050 ms (1 SE), z +3.4: on faster** |

**b4fast, b1 + b2 + b3threads on in both halves** (`SOTC_AB_FLAG=b4fast`; B4 -0.150 +- 0.032):

| run | p9 | p10 |
|---|---:|---:|
| ride7 800 s seed 7 (mixR7 / mixR7b) | +0.158 +- 0.124 | not run (below) |
| ride7 800 s seed 13 (mixR13 / mixR13b) | +0.064 +- 0.132 | +0.071 +- 0.138 |
| lake6 400 s (mixL7 / mixL7b) | +0.079 +- 0.143 | +0.028 +- 0.087 |
| **pooled** | **+0.100 +- 0.038 (z +2.6)** | p10's two: +0.050 +- 0.041; with p9's seed 7: **+0.086 +- 0.034 (z +2.5)** |

mixR7b (p10) could not run: from 05:37 every Vita3K launch on this Mac (any build, p9 included, any tree) crashed at the app's
start ("Unhandled access to 0x0" right after `_sceKernelGetThreadInfo`), and exactly those launches log Vita3K's "Failed to
initialize audio! Audio will not work.": the host's audio device went away (the Mac's state, not the build; pg1-pg6, bootR2 and
five A/Bs of p10 had booted before it). The b4fast pool therefore takes p9's seed-7 run (the same b4fast code and deferral; p10
only changes the idle wait's 1 ms, which the seed-13 and lake6 b4fast A/Bs show does not move b4fast: +0.064 / +0.079 on p9,
+0.071 / +0.028 on p10).

- p9's lake6 A/B found the idle wait's 1 ms (section 4): fixed in p10, which the other runs then measured.
- With the deferral, b3threads is no longer a cost in Vita3K (+0.17 ms on faster where B3 / B4 measured -0.17 / -0.15), and
  b4fast neither (B4 -0.150). A likely reading (not proven): part of the carriers' measured cost was the same mechanism at
  gameplay frame rates, a lower thread dispatched only to take the overdue VBlank handler and be preempted (an extra
  invocation and a round of hand-overs a frame); with the deferral that thread runs its turn and the VBlank comes once.
  The console's A/B decides (section 9).

## 8. 89 MB boot

bootR2 (p10, `SOTC_VITA_EAT_USER_MB=89`, all four on, ride7 600 s, shots every 120 s, the ride probe): field at frame 2 000,
mounted at 2 603, rode to the end with 0 dismounts; frames 3 902 / 7 562 / 11 304 / 14 959 / 18 705 at 120 / 240 / 360 / 480 /
600 s; system free 67.2 MB at the end; 0 watchdog lines. The screenshot tool captured only the 60 s and 180 s shots this time
(the window list was still right; the later captures did not land), so the riding pictures come from the check run on the
same build: bootR2 60 s Wander on foot on the grass with his sword up, Agro's shadow at the edge; 180 s Wander on Agro at a
gallop along the shrine's wall with the bridge behind, Agro's body shadow on the ground; chkR2 (p10, 89 MB off) 510 s Wander
on Agro at a gallop on the plain by a stone pillar. Ground texture, Wander, Agro and the shadows normal, the frame counter
advancing in every shot.

**p10's own boots** (pg1-pg6, 89 MB ride7 300 s, all four on): 5 reached the field at 2 000 and mounted (2 435-5 318); **pg3's
Vita3K died at 85 s** (frame ~2 550, after the field: err.txt ends mid-line, Vita3K logged only its shutdown and was killed at
the run's end), so no guest-side reason is in the log; not counted as a stall, and not seen in any of the other ~40 runs with the fix. (Tree b1 then failed three launches of one A/B at Vita3K's start, "Unhandled access to 0x0" before any guest output; that A/B was rerun on b1b.)

## 9. spalign, the console package, the default

- **spalign**: `python3 ../tools/spalign.py p10.elf` (and p9.elf): 0 sp-based :128/:256 NEON accesses (of 1 046 with a hint).
- **Default**: nothing changes with every switch off (`g_ps2xB5Defer` follows b3threads); b3threads and b4fast stay off by
  default (their Vita3K cost, B3 / B4). The default path has the same livelock exposure (section 3, oD1: 13 escaping frames
  in ~200) and has not been seen to stall; `SOTC_B5_DEFER=1` applies the rule there if a console or Vita3K run ever does.
- **Console A/B package**: `build/agent-scratch/b5/celosia-b5.vpk` (= p10) + `env-ab.txt`: N6's console env without its A/B
  lines + `SOTC_B1SOUND=1 SOTC_B2LOAD=1 SOTC_B3THREADS=1 SOTC_B4FAST=1 SOTC_AB_FLAG=b3threads SOTC_AB_PERIOD=300 SOTC_B1_LOG=600
  SOTC_B2_LOG=600 SOTC_B3_LOG=600` (the `[b5]` line comes with the B3 log). Not installed.

## 10. What is still open

- The default path's exposure: the same livelock happens with every switch off (oD1); it has not stalled there in these runs,
  but nothing but frame timing keeps it out. Making the deferral the default (`SOTC_B5_DEFER=1` everywhere) is a decision for
  a round that can A/B the default path and look at its loads.
- The idle-turn hook (round 25, sotc_overrides.cpp) rescues the idle queue's senders from the same family of starvation; with
  the deferral on, t5 gets ~1.3 turns a frame in the menu; the hook still fired once (one sender woken) in 9 of the 21 runs
  of sections 5 and 6 with the fix (a single heavy frame each).
- The broader cause, EE time seen only at parks / the idle wait, also decides when every VBlank lands relative to the main
  thread's frame; a host-paced VBlank delivered mid-frame would quantize frames to VBlanks like the PS2 (and cost the
  port's back-to-back frames), which is why the fix moves the catch-up only past a lower thread's turn.
