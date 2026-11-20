# Round N5: the EE threads and the scheduler's checkpoints (layers 6 and 7) — profile, the blocking set, n5sched (2026-11-19)

Plan: docs/native_all_plan.md, step N5. Base: N1's tip (ca9c344). Scratch: `build/agent-scratch/n5/` in the main
checkout (NOTES.md has the exact next commands). Own Vita3K trees ../vita3k-celosia-n5 / -n5b (copies of d17, 222 MHz,
3 cores, muted). Status: **profile and blocking set done; n5sched built into the code, off by default; its build and
A/Bs were queued but had not run when the round was cut** (results go below when they come in).

## 1. What the EE threads do riding (profR7: N1's p1, ride7 500 s, mounted at frame 4 838)

The scheduler's own counters (`SOTC_STAT139=1`, `SOTC_S11RT_LOG=600`, `SOTC_S17IOP_LOG=600`), frames 13 200-14 400, a frame:

| what | a frame |
|---|---:|
| dispatches by run() (thread switches, invocations, re-entries) | 13.5-16.1 |
| the dispatcher's host time between two guest runs (on the EE host thread) | 0.33-0.94 ms |
| guest thread runs: t4 / t5 / t8 / t9 (the game thread) / t10 / t11 | 2.0 / 1.0 / 2.1 / 1.1-1.3 / 2.1 / 0.05-0.4 |
| where they leave (park): 0x1b2d08 (t4), 0x1c8108 (t8, the VBlank / sound loop), 0x1b10dc (t10), 0x1acaec (t11, CD streaming) | ~2 / ~1 / ~2 / 0.04-0.35 |
| slow checkpoints | 14-17 |
| ... pending flag / deadline / slice not over ("go on") / **preempt at a slice end** / renew | 0.2-0.4 / 0.1 / 11.5-13.5 / **0.00** / 1.3-2.0 |
| what set the gate: the IOP batch / the slice end / a deadline | 43-50 / 13-16 / 0.2-0.4 |
| S17 episodes (the game thread's checkpoint finding work: all nested, none unwound) | ~1 (the GS FINISH handler 0x1c37f0 in 90%) |
| the isys lock's signals (fast path / slow) | 247-274 / 0 |
| SG2 RPCs (the sound driver, host side) | 1.8-2.3 (40-63 µs) |

What it says:

- **No thread is ever preempted at a slice end riding** (preempt 0.00). Every switch is the threads' own: a park
  (semaphore wait, sleep, the VBlank wait), a wake by an interrupt handler or by an IOP reply (`flags 6 / 7`, ~3 a
  frame each), or S17's nested run of the GS FINISH handler. The time slice only costs gate hits (13-16 a frame) that
  renew it.
- **The game thread does not unwind at its checkpoints any more** (S17 / S18, on by default): every episode is nested
  on its host stack. So, on the game thread, a guest call unwinds through the context only when its callee reaches a
  real thread switch — a blocking kernel call or a host stub that waits — or in S17's rare not-nested cases (inside a
  handler / invocation, a posted reply whose waiter has not parked, less than 192 KB of host stack).
- The service threads (4, 5, 8, 10, 11) run ~7 times a frame; S17 found their runs taken almost entirely while the
  game thread waits for the render thread. Their cost on the EE host thread is the dispatcher (0.3-0.9 ms a frame,
  which includes the game thread's own re-dispatches) plus their bodies (the sampler's per-thread split was lost: N1's
  p1.elf was deleted during the run; the `[n5]` log below measures it per guest thread).

So the ~1.5 ms the plan priced for layer 6 is mostly the dispatcher and the isys lock's fast path (the lock: 1.0% in
S21's profile), not preemption. Moving the service threads to host threads (the plan's step) removes the dispatcher's
share only if their kernel objects (the semaphores and event flags they share with the game thread, the guest heap,
the SG2 status blocks) become host-locked objects first; that is design-sized and was not started this round.

## 2. The blocking set: docs/n5_blocking.txt (tools/n5_blocking.py)

The set N4 needs: for each of the 9 024 table functions, whether a call into it can reach a thread switch.

- Seeds: a `syscall` whose number (from the `addiu / ori v1, zero, N` before it) is not one of the non-blocking EE
  calls (queries, the i* forms, setup, cache, handler registration, SIF / GS registers; `NONBLOCK` in the tool), or
  whose number cannot be found; a hand-written native whose C++ body names a waiting primitive (callMaySleep,
  runToReturn, WaitSema, SleepThread, waitSemaphore, isysSleep*, iosWait* ...), is a coroutine / resume entry, or calls
  guest code at a computed address; a slot the runtime / config fills with host code the tool cannot read (stubs,
  hooks: `lift_d1.native_addresses` less the lifted copies); an indirect call (`jalr`) or jump; a function the lifter
  does not parse.
- Then the transitive closure over the callers (MIPS `jal` / tail jumps; a native's guest callees are the function
  starts its body names).

Result (ca9c344): **BLOCK 4 305** (seeds 999: stubs 372, jalr 284, notparsed 164, natives 121, syscall 55, ijump 3,
unknown callee 11; reached through calls 3 295), **NOBLOCK 4 719** (4 067 of them without a loop). Weighted with S21's
riding profile: the BLOCK functions are 10.64% of the game thread, the NOBLOCK ones 8.58%. Each line:
`<addr> <name> BLOCK|NOBLOCK <reason> loops=<0|1> [via=<callee>]` (`via` names the callee that brings the block in,
so a chain can be followed down to its seed).

Rerun: `python3 tools/n5_blocking.py --jobs 2 --profile build/agent-scratch/s21/mixR7p1.syms.txt` (~3 min).

Limits: conservative by construction (a stub or a `jalr` blocks); the natives are read by regex (a native that waits
through a helper whose name is not in the list would be missed: the D4 difftest found three such, iosWaitIopSema,
sceMc2ReadFileAsync, sub_0011B458, which this list catches by name or through their syscalls). For N4 the useful
column is NOBLOCK with loops=0 (cannot unwind at all) and NOBLOCK with loops (can unwind only at a back-edge
checkpoint: on the game thread S17 nests those, elsewhere a higher-priority wake can preempt there).

## 3. n5sched (SOTC_N5SCHED, A/B name n5sched, off by default)

The PS2 kernel never time-slices threads of equal priority: a thread runs until it blocks, yields
(RotateThreadReadyQueue) or a higher-priority thread is made ready (which preempts at once). The runtime's scheduler
adds a 65 536-cycle slice whose end hands the CPU to a ready thread of equal priority. n5sched restores the kernel's
rule (EeScheduler.cpp): the slice never ends (renewed as 2^40 cycles), a checkpoint preempts only for a strictly
higher-priority ready thread, S13's stretch and cut do nothing. With it, threads switch only at explicit points
(blocking, yielding, waking, interrupts), which is the property N4's calling convention needs; the dispatch
checkpoint at calls then only charges cycles and hands over events (nested by S17 on the game thread).

Riding, the slice never preempted anyway (section 1), so n5sched is expected to be even in speed; its value is the
guarantee. Off -> on flips at a frame boundary; on -> off pulls a long slice back to a plain one at the next slow
checkpoint (O(1)). `SOTC_N5_LOG=<frames>`: `[n5]` lines, each guest thread's host time a frame (exclusive of S17's
nested runs) and its runs.

Not done (why): taking the dispatch checkpoint out of `callGuestKnownBody` (ps2_runtime.h) is a change to the inline
call path every generated file includes, i.e. a rebuild of all 9 000 recompiled files (~1 h on this machine), and
round N3 was editing the same call macros; with S17 nesting on the game thread it removes only an add and a compare a
call. Host threads for the service threads: section 1.

## 4. Checks and measurements

Queued (`build/agent-scratch/n5/chainN5.sh`, detached): clone N3's build dir once N3's build is over, build p1, then
`offR` (ride7 420 s, n5sched off, `[n5]` / `[s11]` / `[s17]` logs: the per-thread baseline), A/Bs `mixR7p1` (ride7
800 s seed 7), `mixR13p1` (seed 13), `mixL7p1` (lake6 400 s), all `SOTC_AB_FLAG=n5sched SOTC_N5SCHED=1`; `recR1` /
`recR0` (ride7 360 s with `PS2X_SPU2_WAV`, on / off, for `n2/tools/wavcmp`); `bootRp1` (ride7 600 s, n5sched on, 89 MB
taken, shots every 60 s); spalign. Results: `vq.out`, `mix*.frames.txt` (off - on > 0: on faster).

**Results (p1, 2026-11-19 20:36-21:17).** n5sched on does not boot: every run with `SOTC_N5SCHED=1` from the start
stays in the boot sequence at 2.8-4.4 game fps with no draws and never reaches the ride (bootRp1: 610 s, 2 614 frames,
no mount; recR1: 360 s, 1 015 frames, 3.3 MB of sound against 69 MB with it off; the three A/B runs `mixR7p1`,
`mixR13p1`, `mixL7p1` give "no frames" past frame 3 000 for the same reason). So the game does rely on the time-slice
switch between same-priority threads during the boot / loads (a thread polling for another one's work), although
riding shows 0 preemptions a frame. With n5sched off (the default) the same build boots and rides normally (offR);
spalign on p1: 0 sp-based :128 / :256 NEON accesses (of 1 013).

What it means for the next step: the time slice cannot simply go. A version that keeps it only for a thread that has
run a long time without blocking (the poller), or that turns the boot's polling loop into a wait, is the next try;
the blocking set (section 2) stands on its own and is what N4 needs. n5sched stays off by default.
