# S11: the PS2 machinery still on the game thread riding: the isys lock, the IOP's batches, the motion root as host code (2026-11-18)

Round S11's brief: the port should run like a native game, so strip the PS2-shaped layers instead of tuning them; the goal
is a stable 30 fps on the Vita riding, ~6 ms a frame short on the console. Target: the PS2 machinery S9 priced on the game
thread (docs/runtime_s9.md §5, ride7, Vita3K 222 MHz, game thread 31.8 ms): recompiled guest code still running 3.58%
(1.14 ms), the EE scheduler's checkpoints 0.78% (0.25 ms), the natives' guest calls through the table 0.66% (0.21 ms), the
isys lock's thread-id / semaphore calls 0.49% (0.16 ms).

Switch: `SOTC_S11RT` (A/B name `s11rt`, **on by default**). `SOTC_S11RT_PARTS=<mask>` limits it to some parts,
`SOTC_S11RT_CHECK=1` runs kMotion both ways, `SOTC_S11RT_LOG=<frames>` prints `[s11]` counts, `SOTC_S11RT_IOPBATCH=<cycles>`
sets kIop's batch (16384), `SOTC_S11RT_PACKED=1` keeps kMotion's packed keys as host arithmetic (off: see §3). Code:
game/overrides/s11rt.h (the parts, the lock's inline calls, the counts), s11_motion.cpp (the motion root), decomp_isys.cpp /
decomp_isys48.cpp (the lock's users), decomp_sema.cpp (the table's SignalSema for the inline signal), decomp_cam126.cpp
(the root key's wrapper), sotc_native.cpp (the per-frame IOP batch, the log); the runtime: EeScheduler.cpp / ee_scheduler.h
(the IOP batch as a global, the `[s11]` counts: slow checkpoints by outcome, the gate by term, the checkpoint flag by call
site, thread runs), ps2_runtime.cpp. Scripts, runs and logs: build/agent-scratch/s11/ (STATE.md; prof.sh the sampler,
chk11.sh the check, abs11.sh / chain_ab.sh the A/B, dis.sh a function's MIPS listing from the recompiled source).

## 1. Where the machinery is riding (profR0: celosia-s9, ride7, sampler)

profR0 (celosia-s9 = S9's p3; ride7 mounted at frame 2 700, window 120-960 s: 20 245 frames, **game thread 32.8 ms**):

| item | share | ms a frame | by caller / what |
|---|---:|---:|---|
| recompiled `_0x` functions | 3.54% | 1.16 | flat: ~400 functions, the largest GetMotionRootFloatFrame 0.12%, manager_sub_01137F78 0.10% (a cloth-mesh normal pass), Sg2SlotTickProc 0.09% (the sound thread), modelReInitialize 0.09%, the motion curves 010B00A8 / 010AEE18 0.09 / 0.07% |
| D-rounds' lifted copies (`lift_d1::`) | — | — | the hottest: manager_sub_010AD048 0.12% (the motion root's key), GetMotionOptionNodeParentId 0.05%, GetMotionRootInfo 0.04% |
| sotc_decomp::recompiled | 0.11% | 0.03 | 0.09% from cam126::rootKeyEntry: with ctl126 off (its default) the root key's wrapper looks its recompiled function up in a hash table on every call |
| the isys lock: getThreadId139 (+ veneer) 0.20%, pollSema139 0.14%, PS2Runtime::eeScheduler() 0.08% (all from getThreadId139) | 0.42% | 0.14 | decomp_isys::forFirst / addGroup / forExit: GetThreadId, PollSema and the release's SignalSema through out-of-line helpers, the scheduler's out-of-line accessor and the table |
| the scheduler: checkpointSlow 0.20%, updateGate 0.11%, EeScheduler::run 0.06%, advanceIopEeCycles 0.04%, the IOP's runCycles 0.03% ... | ~0.5% | ~0.17 | below |
| callGuest's out-of-line copy | 0.70% | 0.23 | decomp_ptcl117::listImpl 0.29%, dispEmitterImpl 0.15%: the particles' display walk (S10's area, not touched) |
| finishGuestCall | 0.14% | 0.05 | spread |

**The scheduler, counted** (`[s11]`, chkRp2 / chkRp3 riding, S11 on): the game thread reaches the scheduler's slow
checkpoint 40-150 times a frame. The gate (where the next checkpoint goes slow) is set by the IOP's batch most of the
time (~80 IOP runs a frame with the old 2 048 cycles, ~11-13 with S11's 16 384), by the end of the 65 536-cycle time slice ~11-16, by a
deadline < 1, by an EE timer never. The checkpoints that unwind to the scheduler ("pending": 25-140 a frame) come from only
~7 events a frame: ~3.5 interrupt handlers (VBlank start / end ~2.2 each at 25 fps, GS ~1, the VIF1 / GIF DMA ~1) and ~3.1
wake-ups of a higher-priority thread (the flag's call sites, `flags 6` / `flags 7`); the rest are the same flags seen again
by the checkpoints on the way out. Threads run a frame: ids 4 (2.0), 5 (1.0), 8 (2.1), 9 (1.0-1.5), 10 (2.0), 11 (0-0.7):
~8-9 thread switches a frame (the sound, streaming and loader threads), no time-slice preemption (0.00).

## 2. What S11 changes (`SOTC_S11RT`, parts)

| part | what | exact because |
|---|---|---|
| kLock (1) | the isys lock's GetThreadId and PollSema from the scheduler's state inline (EeScheduler::currentThreadIdFast / pollSemaphoreFast through a pointer kept at install; no out-of-line helper, no accessor call), and its release's SignalSema inline (signalSemaphoreUncontended) instead of a table call, when the table holds decomp_sema's SignalSema | the same calls round 139 / round 43 make, with the same results in v0 and the same registers a jal leaves (ra, sp, pc = ra); anything else (no thread, a waiter, the count at its maximum, a pending reschedule, another table entry, the worker's runtime) as before |
| kIop (2) | the IOP runs behind the EE in batches of 16 384 EE cycles instead of 2 048 (the batch is a runtime global the game sets each frame, `g_ps2xIopBatchEeCycles`) | not bit-exact by design, the gameplay the same: every EE -> IOP call (RPCs, SIF, module loads) brings the IOP up to date first as before (catchUpIopNow), so the game sees the same IOP state at each call; only the IOP's own progress between calls is coarser (56 µs of EE time, 0.3% of a frame) |
| kMotion (4) | GetMotionRootFloatFrame (0x10b4478) and the root track's key it calls twice (manager_sub_010AD048) as one host routine (s11_motion.cpp) over named views of the motion data (MotionRef / MotionSet / Motion / RootTrack), plain arguments and values: no table call, no wrapper, no hash lookup, no lifted code. Riding ~16 calls a frame | the same 16 bytes out, v0 and f0, and the VU0 registers the original's lerp leaves (vf5-vf8, ACC, t0), with the runtime's own FPU / VU0 operations; checked in-run (§3). Packed tracks (int16 keys) go to the recompiled function |
| kRootKey (8) | cam126::rootKeyEntry (the table's manager_sub_010AD048, a pass-through while ctl126 is off) keeps the recompiled function in a pointer instead of a hash lookup per call | the same function |

## 3. Equivalence

- **kMotion**: `SOTC_S11RT_CHECK=1` (chk11.sh) computes the host answer without writing, runs the recompiled function on the
  live state and compares the 16 bytes out, v0, f0 and, after a lerp, vf5-vf8 / ACC / t0. Legs that a checkpoint unwound
  inside the recompiled function (it resumes later from the scheduler) are counted apart, not compared; the check runs on
  the game thread only (on round 91's worker a guest call can park the job).

| run (build) | where | checked | unwound (skipped) | differ |
|---|---|---:|---:|---:|
| chkRp2 (p2) | ride7 | 337 194 | (not separated yet: the 3 380 "differ" were the unwound legs: out not yet written) | — |
| chkRp3 (p3) | ride7, mounted at 2 258, to frame 21 000 | 385 434 | 5 676 | **675** (0.18%): all packed keys, z one or two units apart |
| chkRp4k (p4, `SOTC_S11RT_PACKED=1`) | ride7, mounted at 3 475 | 165 845 | 2 943 | 321: packed keys again (x or z) |
| **chkRp4 (p4)** | ride7, mounted at 6 160, to frame 13 000 | **106 573** | 2 075 | **0** |

  The packed keys (three int16 per frame, each times its scale x 2^-15 (1 + 2^-15) plus its base) use the same FPU macros in
  the same order as D5's lifted copy; ~0.2% of them land one or two units of the last place apart in z, and S11 did not
  find why in the time it had: worked out by hand with exact rationals for one of chkRp4k's printed cases (raw 16 383, its
  scale and base), the original's z equals "the product raw x scale rounded toward zero, then x k + base fused, rounded
  toward zero" and none of the separately-rounded orders, so the reference path seems to fuse that multiply-add (the
  D5 copy's own code shows separate vmul / vadd; which leg runs it was not settled). ~9 of kMotion's ~16 calls a frame are
  packed. p4 sends packed tracks to the recompiled function (exact by construction) unless
  `SOTC_S11RT_PACKED=1`.
- **kLock** and **kRootKey** call the same scheduler functions / the same function the old paths call (argued, not run
  twice); the log counts the inline signals and the fallbacks: riding ~180-260 inline signals a frame, 0 fallbacks.
- **kIop** changes when the IOP's own work runs between EE -> IOP calls, not what any call returns (above).

## 4. Speed

`SOTC_AB_FLAG=s11rt SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs11.sh): S11 on or off at random each frame, Vita3K 222 MHz, the
sampler on, 800 s a run; tools/mixframes.py (300-frame blocks, ± 2 SE, shift -1; ride7 from the mount + 600, the others
from frame 3 000). Off - on: positive = S11 saves that much a frame.

| run (build, seed) | route | frame off - on (± 2 SE) | z | busy off - on |
|---|---|---|---:|---|
| **mixR7p3 (p3, 7)** | ride7 (mounted 2 219) | **+0.346 ± 0.199 ms** | **3.5** | +0.320 ± 0.166 |
| **mixR11p3 (p3, 11)** | ride7 (mounted 2 429) | **+0.249 ± 0.174 ms** | **2.9** | +0.289 ± 0.156 |
| mixL7p3 (p3, 7) | lake6 | +0.372 ± 0.143 ms | 5.2 | +0.381 ± 0.140 |
| mixL11p3 (p3, 11) | lake6 | +0.410 ± 0.233 ms | 3.5 | +0.419 ± 0.236 |
| mixA7p3 (p3, 7) | attract | +0.015 ± 0.106 ms | 0.3 | +0.079 ± 0.098 |
| mixA11p3 (p3, 11) | attract | +0.058 ± 0.130 ms | 0.9 | +0.113 ± 0.136 |
| mixR7p4 (p4, 7) | ride7 (mounted late, 5 474: 31 blocks) | +0.194 ± 0.293 ms | 1.3 | +0.238 ± 0.303 |
| **mixR7p4b (p4, 7)** | ride7 (mounted 2 350) | **+0.400 ± 0.338 ms** | **2.4** | +0.403 ± 0.325 |
| **mixR11p4 (p4, 11)** | ride7 (mounted 4 131) | **+0.173 ± 0.156 ms** | **2.2** | +0.221 ± 0.150 |

p3 has kMotion's packed keys as host arithmetic, p4 (the shipped build) sends them to the recompiled function, so p4's
kMotion does about half the calls; the other parts are the same in both.

The sampler cannot split a symbol both states run by state (the A/B's NEW / OLD split needs code only one state runs):
kMotion's own code (s11motion::) against what it replaces (the lifted GetMotionRootFloatFrame, D5's root key, the hash
lookup) reads 0.47 against 1.50 samples per 1 000 frames of each state (z 66): ~0.09 ms a frame of the ~0.3 ms.

**Decision (z > 2 in both ride seeds, p3 and p4; the lake a gain; attract even): `SOTC_S11RT` stays on** (default on):
ride7 +0.25-0.35 ms a frame (p3), +0.17-0.40 ms (p4), lake6 +0.37 / +0.41 ms. The ~1.8 ms of PS2 machinery priced is
not removed by these parts: S11 took ~0.2-0.35 ms of it riding (§5 says what the rest needs).

### Boot with the console's memory

bootR4: celosia-s11.vpk (p4, S11 on) with `SOTC_VITA_EAT_USER_MB=89` taken (the console's free memory): mounted at
3 186, riding to frame 16 235 (720 s), no fault. Pictures looked at: Wander on Agro by the cliff temple, the ground, the
far hills and the shadow as usual (build/agent-scratch/s11/runs/ride7_vita3k_bootR4_s11on_eat89.png; the check run's:
runs/ride7_vita3k_chkRp2_s11check.png, Wander galloping on the plain). spalign (p4): 959 NEON :128 / :256 accesses, 0
sp-based. Final VPK build/agent-scratch/celosia-s11.vpk / .elf (p4); console env build/agent-scratch/s11/env-console.txt
(console-env-latest.txt with s11rt first in SOTC_AB_FLAG; S11 is on by default).

## 5. What this leaves (item by item, priced)

- **Recompiled guest code (3.5%)** is flat: after the motion root the next ones are 0.06-0.10% each (a cloth-mesh normal
  pass with VU0 rsqrt, the sound thread's tick, modelReInitialize and vramAlloc in the display chain, two motion-curve
  evaluators that call three key fetchers each). Each is a host routine like kMotion (~0.02-0.04 ms each); the curve
  evaluators and the root info getters (GetMotionRootInfo / IntFrame / DirInfo, D5's lifted copies, 0.07% together) are the
  natural next batch, since they share kMotion's views. A packed-key mismatch has to be understood first (§3).
- **The scheduler**: with kIop the IOP's batches (and their slow checkpoints) are ~11-13 a frame (from ~80); the rest are the time slice's
  end (~11-16 a frame: no thread ever needs the preemption riding, a slice of 4x the length would cut them to ~3-4 with no
  change while one thread runs) and the unwinds for ~7 events a frame (interrupts, wake-ups). Making the sound / streaming /
  loader threads host threads would take the 8-9 thread switches a frame and the wake-up unwinds away, but they run guest
  code (Sg2*TickProc, sndSepTickProc: ~0.2% of the game thread) that would need to become host code first.
- **The isys lock**: kLock leaves its users' own guest frames and register traffic (the natives keep the lock in guest RAM
  as the guest code shares it); a host lock needs every isys user native, including the kernel's lifted ones.
- **callGuest from the particles' display walk (0.44%)**: decomp_ptcl117 calls guest functions per emitter / particle
  through the table; S10's area.
- **ctl126's root key (found on the way)**: cam126::rootKey reads the packed and constant encodings' data from the track
  itself, the original from *(track + 0xC) (0x10ad078: `lw $a1, 0xC($a1)` before the branch): with SOTC_CTL126=1 the root
  motion of those tracks is wrong. That may be the round-131 route-6 bug (Wander's position jumping) that turned ctl126 off.
  Not fixed here (S11 does not use it); worth a look before ctl126 is turned on again.
