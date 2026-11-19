# S13: the motion root's packed keys exact, the root getters as host code, the FPU's zero divides, the time slice (2026-11-18)

Round S13's brief: carry on S11's list ("what's left", docs/runtime_s11.md §5) under the directive to strip PS2-shaped
layers: (1) kMotion's packed (int16) root keys, which S11 left to the recompiled function because ~0.2% of them came
out one or two units apart; (2) the next motion getters on the same views; (3) the scheduler's time-slice checkpoints.
Closed early at the coordinator's request (speed): the motion-curve evaluators 0x010B00A8 / 0x010AEE18 were not started.

Switch: `SOTC_S13MOT` (A/B name `s13mot`, **off by default**: §5). `SOTC_S13MOT_PARTS=<mask>` limits it to some parts, `SOTC_S13MOT_CHECK=1` runs
the host routines and the recompiled legs both ways on the game thread and compares, `SOTC_S13MOT_LOG=<frames>` prints
`[s13]` counts, `SOTC_S13MOT_STRETCH=<n>` the slice stretch (8). Code: game/overrides/s13mot.h (the parts, the counts),
s11_motion.h (S11's views and root key, now shared), s11_motion.cpp (kMotion, its check), s13_motion.cpp (the root
getters), decomp_cam126.cpp (the root key's table entry), sotc_native.cpp (the per-frame globals, the log); the runtime:
ps2_runtime_macros.h / ps2_runtime.cpp (`g_ps2xFpuZeroFast`), EeScheduler.cpp / ee_scheduler.h (`g_ps2xSliceStretch`,
the counts). Scripts, runs and logs: build/agent-scratch/s13/ (prof.sh, chk13.sh the check, abs13.sh / chain_ab.sh the
A/B, dis.sh a function's MIPS listing, waitslot.sh now atomic: a mkdir lock held 20 s after a slot is granted).

## 1. Where it stood (profR0: celosia-s11, ride7, sampler, 16 334 frames, game thread 33.1 ms)

The pieces S13 replaces, as shares of the game thread: D5's lifted root key (lift_d1::manager_sub_010AD048) 0.09%, the
recompiled GetMotionRootFloatFrame (kMotion's packed fallback, ~8 calls a frame) 0.07%, sotc_decomp::recompiled 0.09%
(from cam126::localQPEntry: ctl126 off, a hash lookup a call, the same pattern S11 fixed for the root key),
GetMotionRootInfo 0.03%, GetMotionRootIntFrame 0.02%, fesetround (+ veneer, fegetround) 0.02%: ~0.3% (~0.1 ms). The
curve evaluators 0x010B00A8 / 0x010AEE18 0.10 / 0.08% (with their callees ~1 500 MIPS instructions of float / VU0
code: left). The scheduler counted (`[s11]` riding): 80-140 slow checkpoints a frame, almost all "pending" (the unwind of
~7 events a frame through nested frames); the time slice's end sets the gate ~13 times a frame but only ~1.1-1.4 of
those become a slow checkpoint that renews the slice (S11's "~11-16 a frame" counted the gate, not the checkpoints).

## 2. The packed keys: what S11 saw was Vita3K, not the arithmetic

S13's check prints, for each difference, the host answer, the recompiled leg's, the FPSCR before and after the leg, and
the host answer computed again from the same inputs after the leg. chkRp1 (p1, ride7): every difference was a packed
key; the FPSCR read round-toward-zero before and after; **the host answer computed again after the leg equalled the
leg**. Worked out with exact rationals (build/agent-scratch/s13 notes; e.g. raw 16383, scale 0x3e166019, base
0xbc67dba3): the leg's z (0x3d72c6ee) is the PS2's arithmetic, both products and the aligned add rounded toward zero;
the first host answer (0x3d72c6f0) is the same arithmetic with the products rounded **to nearest**. Same code (both
legs run the same out-of-line routine), same inputs, same FPSCR value: the first answer ran while the emulator was
rounding to nearest.

The cause: the FPU divide / root by a non-normal operand (ps2FpuDivideNearestCsr / SqrtNearestCsr: 0 / x is the
common case, e.g. a motion at frame 0) switches the rounding mode with two FPSCR writes (sse2neon's _mm_setcsr ->
fesetround). Vita3K's A32 recompiler (dynarmic, arm64 host) keeps the rounding mode per translated block; after those
writes, linked blocks can go on running the host's float unit in round-to-nearest although the guest FPSCR reads
toward zero, until the next exit to the host (a kernel call). In chkRp1 every differing call was the second
GetMotionRootFloatFrame of PredictMotionGeometryPotentialWithScaleElem, which follows GetMotionRootDirInfo's `div.s`
of the frame (0 / x at a whole frame); writing the FPSCR back unchanged before the host answer did not help (p2's
experiment), a kernel call does (p3). On the console the FPSCR is what rounds: there the host code and the recompiled code give the
same bits by construction (same instructions, same mode).

Two changes follow:
- **kZeroDiv** (a speed change that also removes most of the trigger): 0 (or a flushed denormal) divided by a normal
  divisor, and the root of a zero, are the signed zero in every rounding mode (IEEE; with flush-to-zero a denormal
  operand is a zero), so `g_ps2xFpuZeroFast` answers them without the mode switch (two FPSCR writes and two libc calls
  each). With it, chkRp2 (p2, ride7) compared 1 173 652 calls with 0 differences.
- **the check's guard** (p3): one kernel call (a clock read) before each compared call, so both legs run in the mode the
  FPSCR says. Without it chkLp2 (p2, lake6) found 1 difference in 611 399 calls: again a packed key whose host answer
  was the round-to-nearest one, the leg's the round-toward-zero one (other divides by a non-normal operand still switch
  the mode).

## 3. What S13 changes (`SOTC_S13MOT`, parts)

| part | what | exact because |
|---|---|---|
| kPacked (1) | kMotion's packed root keys as host arithmetic (the same FPU macros in the same order as the original: `raw x scale x 2^-15 (1 + 2^-15) + base`, operand alignment, round toward zero) instead of the recompiled GetMotionRootFloatFrame | §2; checked |
| kRoot (2) | GetMotionRootIntFrame (0x10b4098) and GetMotionRootInfo (0x10b67a8, the non-looping path: f13 <= f12) as host routines over S11's views (s13_motion.cpp); the root key's table entry (cam126::rootKeyEntry, 0x10ad048) answers from the same views for every caller (natives' callGuest from geometryStep / rootBlend, the recompiled callers), not only with ctl126 on; GetMotionOptionNodeParentId / GetCharacterClassOptionNodeLocalQuaternionPos keep their recompiled functions in a pointer (no hash lookup a call) | the outputs, v0 (all 128 bits where the original's `lq` sets them), f0, the FPU condition bit and the VU0 registers (vf5-vf8, ACC, t0) the originals leave; checked |
| kSlice (4) | the EE time slice renewed 8 slices long when it ends with nothing ready at or above the running thread's priority (then a slice end only renews it); a thread made ready at or above that priority during the stretch cuts it back to the plain slices' next boundary | the thread that runs is the same unless an equal-priority thread becomes ready mid-stretch (counted: 2 a run, both at boot, none riding) |
| kZeroDiv (8) | 0 / x and sqrt(0) as the signed zero without the rounding-mode switch (`g_ps2xFpuZeroFast`, ps2_runtime_macros.h) | IEEE: the same bits in every rounding mode (§2) |

Also (always on, from S13's check): kMotion's apply now leaves v0's upper 64 bits as the original's `lq` does (z and
1.0 of a kind < 6 key) and the FPU condition bit of its `c.eq.s`; S11 left both.

**ctl126's root key** (S11's finding): cam126::rootKey read the constant and packed encodings' data from the track
itself instead of `*(track + 0xC)` (0x10ad078: `lw $a1, 0xC($a1)`). rootKeyEntry's host path now uses S11's views
(s11motion::rootKey: the right data, the original's v0 too) and the check compares it (counted as "root key"); the old
routine is left unused. With ctl126 off (its default) that path runs under kRoot.

## 4. Equivalence (`SOTC_S13MOT_CHECK=1`, chk13.sh)

Each call answered by host code is answered again by the recompiled function on the live state; legs that a checkpoint
unwound inside are counted apart. All four routines: GetMotionRootFloatFrame (kMotion incl. packed keys), IntFrame,
RootInfo, the root key's table entry.

| run (build) | route | compared | unwound | differ |
|---|---|---:|---:|---:|
| chkRp1 (p1, no kZeroDiv in the run's code) | ride7 | 1 347 529 | 8 339 | 20 260: packed keys rounded to nearest (§2) |
| chkRp2 (p2) | ride7 | 1 173 652 | 6 752 | **0** |
| chkLp2 (p2) | lake6 | 611 399 | 4 441 | 1 (the emulator's mode, §2) |
| chkAp2 (p2) | attract | 621 609 | 2 313 | **0** |
| **chkRp3 (p3, the check's guard)** | ride7 | 1 056 633 | 6 466 | **0** |
| **chkLp3 (p3)** | lake6 | 609 759 | 4 331 | **0** |
| **chkAp3 (p3)** | attract | 656 901 | 3 237 | **0** |

The slice: in every run 2 stretched slices were cut by a thread of the running one's priority, both before the game
loop (frame < 600); riding none (the other cuts are higher-priority wake-ups, which preempt at once either way).

## 5. Speed

`SOTC_AB_FLAG=s13mot SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs13.sh): S13 on or off at random each frame, Vita3K 222
MHz, the sampler on; tools/mixframes.py (300-frame blocks, ± 2 SE, shift -1; ride7 from the mount + 600). Off - on:
positive = S13 saves that much a frame. At the coordinator's request (close fast) the second seed ran 400 s and lake6 /
attract were not A/B'd.

| run (build, seed, length) | route | frame off - on (± 2 SE) | z | busy off - on |
|---|---|---|---:|---|
| mixR7p2 (p2, 7, 800 s) | ride7 (mounted 3 032; 55 blocks) | +0.123 ± 0.192 ms | 1.3 | +0.080 ± 0.159 |
| mixR11p3 (p3, 11, 400 s) | ride7 (mounted 3 547; 20 blocks) | -0.099 ± 0.228 ms | -0.9 | +0.076 ± 0.208 |

(p2 and p3 differ only in the check's code.) The pieces replaced were priced at ~0.3% of the game thread (~0.1 ms);
the host routines and the per-call switch tests take part of that back, and the slice stretch saves ~1 slow checkpoint a
frame (§1): the whole is within the A/B's noise.

**Decision (z > 2 in both ride seeds required): `SOTC_S13MOT` off by default** (`SOTC_S13MOT=1` turns it on; the A/B's
turns flip it). Exact and checked, so it can be turned on for the console's A/B; the fixes to S11's kMotion (v0's upper
half, the condition bit) are on with S11.

### Boot with the console's memory, pictures

`SOTC_VITA_EAT_USER_MB=89` taken: bootRp3 (p3, S13 on) mounted at 6 897 and rode to frame 10 952; bootRp4b (p4, the
shipped build, S13 off by default) mounted at 4 337 and rode to 18 438; no fault (bootRp4, p4's first try, never
mounted: the route script's timing; it ran clean on foot beside Agro). Pictures looked at: Wander galloping on Agro by
the bridge and by the cliffs, the ground, Agro's shadow as usual (runs/chkRp1/t0720.png, runs/bootRp3/t0480.png,
runs/bootRp4b/t0600.png). spalign (p2-p4): 964 NEON :128 / :256 accesses, 0 sp-based. Final VPK
build/agent-scratch/celosia-s13.vpk / .elf (p4); console env build/agent-scratch/s13/env-console.txt (console-env-latest.txt
unchanged: S13 is off by default).

## 6. What this leaves

- The motion-curve evaluators 0x010B00A8 / 0x010AEE18 (0.10 / 0.08%) and their callees (manager_sub_010AC6C8,
  010AE658, 010AEAF8, 010ADE08: ~1 500 MIPS instructions of float / VU0 code, a sine / cosine call): the next batch on
  the same views, bigger than a round's tail.
- GetMotionRootInfo's looping path (f13 > f12: GetMotionResourceFrames and two integer keys) and GetMotionRootDirInfo
  (0.014%) still run the recompiled / lifted code.
- The scheduler: the slow checkpoints riding are the unwinds (~85-130 a frame for ~7 events), not the slice; only host
  threads for the sound / streaming / loader code would take them away (S11 §5).
- The remaining divides / roots by a non-normal operand (results outside the plain exponents, denormal divisors) still
  switch the rounding mode; on Vita3K that leaves the emulator rounding to nearest for a while (a fidelity issue of the
  emulator, not of the console).
