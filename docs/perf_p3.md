# Perf P3: work beside the game thread within the frame (p3par) (2026-11-22)

Round P3's brief: the console's game thread takes ~40 ms a frame riding against a 33 ms budget (docs/console_profile_2026-11-20.md)
and is CPU-bound, while cores 1-2 are ~60% busy. P1, P2 and DC2 showed that patching single hot functions gives ~0.1 ms
each. Take the largest block(s) of game-thread work that can run **in parallel on cores 1-2 within the same frame**,
exactly (same RAM effects, same order of observable writes, same picture), behind an off-by-default switch with a sync
point, a check mode and a console A/B package. Target: 2 ms or more off the game thread.

Switch: `SOTC_P3PAR` (A/B name `p3par`, `kP3Par`), **off by default**. Check mode `SOTC_P3PAR_CHECK=1` (with
`SOTC_P3PAR_RAMCHECK=<n>`, default 16), counts `SOTC_P3PAR_LOG=<frames>` (`[p3par]` and `[p3q]` lines; default 600 when
the switch or the check is on). Code: game/overrides/p3_par.h (notes, stats), sotc_native.cpp (hookjobs: p3Partial,
p3KickStart / End, p3Verify, p3AfterMove, the [p3q] census; skinAtKickCore with its live and dry sinks; kickFetch,
kickReadsNoGuest), sotc_overrides.cpp (copyViaSpr: the move's arguments and the after-move call), sotc_decomp.h (the
switch, g_afterGuestMove). Scripts, runs and logs: `build/agent-scratch/p3/` (NOTES.md has every command). Vita3K trees
../vita3k-celosia-c1, -c1b, -c1c.

## 1. Looking for a block that can run beside the game thread

The rules of the round: the worker code may not call guest code or touch the R5900 context, and the result must be
exact. So a candidate is host-only work whose inputs are fixed at some point of the frame and whose outputs nobody reads
until a later point. Going through the console profile's big items with that filter:

| block (console ms riding) | host-only? | verdict |
|---|---|---|
| characters: motionMatrix 0.5-0.7, the keys, the cloths ~1.2, gaze, ctrl* | no: guest-context natives / lifted code, results read at once by the next guest call (IK, the cloth callbacks) | S3's pose jobs, S8's cloth queue and char104 already lost on the console (P1 section 1) |
| display walk 1.0-1.4, lightPass + light manager ~1.1, solidDisp 0.5-0.7, clip 0.2-0.35 | no: guest-context code with guest calls (getAmbientColorC, the volumes, CalcLightLayoutObj), registers and dead stack kept | not movable under the rules |
| the haze strips 0.35-0.5 | no: the guest gl* state, registers, the stack frame | idem |
| collision: obj125's loop 1.2-1.3, finder ~1.3 | queries asked by game logic, answers used at once; sequential (the segment shortens at each hit) | no window |
| particles 3.5-4.1 | the step is guest-shaped and draws the game's VU0 random numbers in order (P2 section 2) | S5 / P2 moved what is free of that |
| modelDispHook 0.8-0.9 | host, but already on the lane and the hook worker (rounds 23 / 61) | done |
| copyViaSpr 0.4-0.85 (the heap's garbage moves) | host memmove | its consumers (the block's handler) read the moved block right after: no window; splitting the copy over two cores would be ~0.1-0.2 ms if the bus has room |

What the round found instead is a wait, not a block of work: **the game thread waiting for work that already runs on
cores 1-2**. The frame logs' `joins` part (the game thread's join waits by site, a 60-frame window's per-frame mean):

| run | `joins move` (heap moves) | `joins screenblur` |
|---|---:|---:|
| console run1 (2026-11-20 n6, riding windows from frame 2 600) | 0.86 ms a frame | 0.71 |
| console run2 | 1.03 | 0.46 |
| Vita3K dRp2 (ride7, riding windows) | 0.64 | |
| Vita3K chkR (ride7, the whole run) | 0.23 | |

Every garbage move of the heap (copyViaSpr: the merge in the garbage pass and malloc's compaction) calls
`g_beforeGuestMove`, which joins the whole hook-worker queue: the model jobs read guest memory by address (packets,
surfaces, textures, the instance chains) and a move may overwrite it. In the windows where it happens the game thread
sits there 0.3-7 ms.

## 2. What p3par does, and what the census then showed

The idea: at a move, wait only for the jobs that may read guest memory. One kind of job provably reads none: round 34's
deferred kick pass (`kJobKick`: the display list kick's skinned pass on the palettes the game thread copied at the kick).
It is handed over at the frame's end behind the frame's model jobs and is joined at the next frame's first hook, so it
runs beside the next frame's logic, and that logic's moves would be the place where the game thread waits for it.

p3par (switch on):
- **The kick** marks its job (`job.p3`) and keeps its ring index (`g_p3KickIdx`).
- **The hook worker**, at that job's start (every model job before it done): `kickReadsNoGuest` decides whether the pass
  will read guest memory (a skin record whose chain is not among the kick's copies goes to readChainPalette; a copy that
  stands for S4's kept read must find it) and publishes the answer (`g_p3Start`).
- **A move** (copyViaSpr's join, `p3Partial`): when the last job queued is that kick job, it waits until every job before
  it is done and the kick job has published "no guest reads", then goes on while the pass runs; in any other case (another
  job last, a pass that would read guest memory, p3par off) the whole join as before. The next join of any site waits
  for the pass as before.

Why that is exact: the kick pass reads only host memory (its job's copies, the frame's skin records and draw list, the
skinned packets' host data, S4's kept palettes) and writes only host memory (the records, the renderer's frame palettes
and meshes); the move writes guest RAM, the scratchpad and the memo serial, none of which the pass reads; round 34 already
runs the pass beside the next frame's logic whenever there is no move. So guest memory, the registers and the draws are
the same with p3par on or off.

**The census** (`[p3q]`: what is queued when a move has to join, with p3par on). It answers the question the design
rested on, and the answer is no:

| run | moves | with jobs queued | kick job last | queued (count) | frame's draws so far | last job | bytes moved | a queued model job's instance / header / matrix / chain inside the move's span |
|---|---:|---:|---:|---|---|---|---|---:|
| chkR (ride7 600 s) | 15 437 | 1 013 | **0** | <20: 22, <100: 462, more: 529 | >500 in 953 | 2D in all | <4 KB 181, <64 KB 404, <1 MB 428 | 45 (4.4%) |
| chkA (attract 400 s) | 5 499 | 451 | **0** | <100: 423 | >500 in 376 | 2D in all | <1 MB 253, more 1 | 51 (11%) |
| dRp2 (ride7 420 s) | 4 978 | 195 | **0** | more than 100: 146 | >500 in 178 | 2D in all | | |

The moves that wait come **late in the frame, before the kick**, with the whole frame's display queued (100-500 model,
cloth, shadow, particle and 2D jobs, the 2D ones last). That fits the garbage pass's place (the kernel's main loop runs it
after the GS wait, when time is left, before the display list goes out; sotc_replay.cpp's notes) and the streaming's
allocations compacting the heap late in the frame (not traced call by call). The kick pass is
never queued at a move in Vita3K (`kick inside 0` too). So in Vita3K p3par never lets a move go on: it is inert, and the
wait it targets is the frame's model jobs, not the kick pass.

And those cannot be skipped by a simple rule: in 4-11% of the moves that wait, the moved span (the hole and the block)
holds the instance, the model header, the matrix or the chain of a model job still queued: a move that went on would
change what that job draws.

## 3. Exactness (the check)

`SOTC_P3PAR_CHECK=1`: (a) every kick job handed over with p3par on has its inputs copied at its start (the records, the
VU picture, the job's palettes) and its outputs hashed (each rewritten frame palette: handle, count, data; each posed
mesh: id, counts, vertices, indices); at the join that ends it the pass is run again alone on the copies into a dry sink
(hashes only) and the hashes compared; (b) readChainPalette calls made by those passes are counted; (c) for a move that did
not wait, every 16th: main RAM copied before the move, the move applied to the copy, the copy compared with RAM word for
word right after.

In-run, every kick pass with p3par on (build p3 = the shipped one; the RAM compare (c) needs a move that went on):

| run | where | frames | kick passes flagged / read no guest memory | run again alone: compared / differ | guest reads in them | moves / with jobs queued / went on | RAM checks |
|---|---|---:|---|---|---:|---|---:|
| chkR | ride7, 600 s, mounted at 3 519 | 18 600 | 17 339 / 17 339 | 17 339 / **0** | **0** | 15 437 / 1 013 / 0 | 0 |
| chkA | attract demo, 400 s | 18 600 | 14 985 / 14 985 | 14 985 / **0** | **0** | 5 499 / 451 / 0 | 0 |
| chkL | lake6, 400 s | 12 000 | 10 855 / 10 855 | 10 855 / **0** | **0** | 2 950 / 86 / 0 | 0 |

(c) never ran: no move went on in Vita3K (section 2), so the check of the elided path itself is vacuous here; what it does
show is the premise of that path: every kick pass of the three routes read no guest memory (kicks flagged = no guest) and
its outputs are a function of its copies alone (run again alone: 0 differences).

## 4. Speed (Vita3K 222 MHz)

Interleaved A/B, `SOTC_P3PAR=1 SOTC_AB_FLAG=p3par SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (q.sh / abn1.sh, mixframes.py,
300-frame blocks; ride7 from the mount + 600). Off - on, +- 2 SE.

| run | frame | busy (game thread) | render thread | move joins (the [p3par] line), on / off |
|---|---:|---:|---:|---|
| mixR7 (ride7 seed 7, 800 s) | -0.032 +- 0.148 | +0.005 +- 0.154 | -0.070 +- 0.158 | 2 087 ms / 12 584 frames, 1 977 / 12 599 (0.17 / 0.16 ms a frame) |
| mixR13 (ride7 seed 13, 800 s) | +0.043 +- 0.151 | +0.019 +- 0.140 | -0.224 +- 0.215 | |
| mixL7 (lake6 seed 7, 400 s) | +0.031 +- 0.135 | +0.022 +- 0.137 | | |
| **pooled (mean of 3, SE = sqrt(sum se^2) / 3)** | **+0.014 +- 0.042 ms (1 SE), z 0.3** | +0.015 +- 0.042 | | |

An A/A, as section 2 predicts: no move went on in any of the runs (`went on 0`), so the two states run the same code but
for the kick's flag and the worker's check of its records (~100 lower_bound lookups a frame on the worker).
Vita3K is optimistic here twice over: its hook worker keeps up better than the console's (Vita3K's riding move joins are
0.2-0.6 ms a frame against the console's 0.9-1.0), and it does not model the cache cost of anything.

## 5. Boot and pictures

bootR (p3, ride7 600 s, `SOTC_P3PAR=1 SOTC_VITA_EAT_USER_MB=89`): `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, booted first
try, mounted at frame 10 855 (the route's repeated teleports), rode to frame 17 478 with no fault. Window shots every
120 s, looked at: Wander riding Agro at the gallop across the plain under the cliffs and past the stone arch (t0420,
t0540), the reins, the mane and the tail moving, Agro's shadow on the ground, the sky and a bird normal. The check runs'
shots (chkR's gallop past the ruined walls in the fog, chkL's Wander swimming in the lake under the walls with the HUD)
normal too. spalign on p3: **0 sp-based** NEON accesses (of 1 040 with an alignment hint). No `%zu`, no by-value aligned
structs, nothing reads GPU pools; the worker side (kickReadsNoGuest, the check's copies) reads only host memory and never
touches the R5900 context or guest code.

## 6. The console A/B package

- VPK: `build/agent-scratch/p3/celosia-p3.vpk` (= p3.vpk; ELF `celosia-p3.elf`), P3 off unless set.
- Env: `build/agent-scratch/p3/env-ab.txt`: the N6 console env (frame log, PROF129, body shadows, muted logs) with
  `SOTC_P3PAR=1 SOTC_AB_FLAG=p3par SOTC_AB_PERIOD=300 SOTC_P3PAR_LOG=600`.
- What to look at: the `[p3par]` lines (moves, "went on", "whole join: last not a kick", the move joins' ms by state) and
  the `[p3q]` census. If the console's moves also come before the kick (as in Vita3K), "went on" stays 0 and the A/B is
  A/A; if some come in the next frame's logic (the console's frames are slower: its garbage pass's time test differs), the
  move joins' ms in on windows drop. The frame log's `joins move` gives the same per window.

## 7. What P3 did not reach, and why

The target was 2 ms off the console's game thread. **The honest result is ~0 in Vita3K and, very likely, under 0.5 ms on
the console.**

- **No block of game-thread work fits the rules.** What is still on the game thread is guest-context code (natives on the
  R5900 context, lifted code, recompiled code) whose results the next guest call reads; the host-only pieces (model
  draws, the particle step, texture animation, the kick's skinned pass, the layers pass) were moved by rounds 23, 34, 61,
  64, S5 and P2. Every measured move of guest-shaped work to cores 1-2 lost on the console (ptcl91 / 95, S3, S8, char104).
- **The one sizeable parallelism loss left is a wait**: ~0.9-1.0 ms a frame on the console (0.2-0.6 in Vita3K) of the game
  thread joining the hook worker at the heap moves. p3par removes the part that provably needs no join (the kick pass),
  and the census shows that part is not what is queued there.
- **What would get that ~1 ms, exactly**: the moves need to know what the queued model jobs will read. Two ways, both
  bigger than this round: (1) every guest read of the hook jobs through one accessor, so a move that does not wait can
  leave the jobs a copy of the bytes it overwrites (copy-on-move of the span, read by the jobs handed over before it); or
  (2) each job's read set (packets, surfaces, textures, chain) recorded at the hand-over, the move waiting only for jobs
  whose set meets its span (the census says 89-96% of the waiting moves would go on). The check for either is (c) above
  plus round 34's read tracking.
- Cores 1-2 have little room anyway: ~62 ms of CPU a frame there at 45 ms frames; at 33 ms they would be ~95% full.
