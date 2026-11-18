# S9: the game thread's runtime costs on the ride, by caller (2026-11-18)

Round S9's brief: a stable 30 fps on the Vita **while riding**; the console still needs ~6 ms a frame. Target: the
**Runtime bucket** of the game thread (S7's ride profile: 11.3%, 4.0 ms a frame at Vita3K 222 MHz): the DMA walk, guest
memory helpers, the scheduler, memcpy / memmove / sceClibMemcpy, the timers, the dispatch. Aim: ≥ 1.5 ms riding, with
the same picture and gameplay, preferring cheaper work on the game thread over offloading (S8's lesson).

Switch: `SOTC_S9RT` (A/B name `s9rt`, **on by default**). `SOTC_S9RT_PARTS=<mask>` limits it to some parts,
`SOTC_S9RT_CHECK=1` runs the checkable parts both ways, `SOTC_S9RT_LOG=<frames>` prints `[s9rt]` counts,
`SOTC_S9RT_HOOKTIME=1` keeps the hooks' clock reads. Code: game/overrides/s9rt.h (the parts, the counts, the copy),
native_renderer_gl.cpp (the eviction pass), s5_particles.cpp (the clock reads), decomp_leaves.cpp (the camera copy),
sotc_native.cpp (the shadow palette, the hook timer, the flag for the renderer, the log), decomp_ctrlturn.cpp (the
registers' zeroing). Measuring build: `PS2X_VITA_DIAGCOUNT=ON` now also counts memcpy (32-512 bytes, > 512),
memmove, memset, malloc and free by caller (ps2_vita_diagcount.cpp; never shipped). Scripts, runs and logs:
build/agent-scratch/s9/ (STATE.md; prof.sh the sampler, tools/callers9.py the sampler's samples by caller, diag.sh +
tools/diagsum.py the measuring build's calls by caller, chk9.sh the check, abs9.sh / chain_ab.sh the A/B).

## 1. The bucket by caller

profR0r (celosia-s8 = p13, S8 off; ride7, Vita3K 222 MHz, the sampler; mounted at frame 3 472, window 240-960 s:
18 455 riding frames, **game thread 31.8 ms a frame**): Runtime 11.0%, **3.51 ms**. The sampler keeps each sample's
LR, so for the leaf helpers (the libc's and the SceLibKernel imports' stubs) the caller is known: tools/callers9.py.
(newlib's memcpy keeps the source pointer in LR in its 32-byte loop, so its LR is the caller only for < 32 bytes; the
measuring build counted the rest.)

| symbol (share of the game thread) | ms a frame | who, how often (the measuring build's counts, a frame riding) |
|---|---:|---|
| memcpy 1.21% | 0.39 | 32-512-byte copies: the particle step's / emitter's / S5's guest blocks (~80 a call site), the hook paths' records; the render thread's own are not here. Spread thin: no one site |
| **GlBackend::evictKept 1.14% + IdSet::insert 0.16%** | **0.41** | the renderer's cache eviction at the hand-over: riding the heap sits over its high mark, so a "heap-only" pressure pass runs every 16 frames, walks every kept mesh (~7 600) and puts every draw's mesh / texture in two hash sets, and evicts nothing (below) |
| **sceKernelGetProcessTimeWide 0.64%** | **0.20** | 0.75% of it from **s5ptcl::managerEnd**: one read a frame, the hand-over stamp of S5's particle batch, used only by the [s5ptcl] log. The display hooks' timer (HookTimer: modelDispHook 152, ptclS1Commit 98, meshDispHook 23 reads a frame with SOTC_FRAME_LOG=1) reads cheap in Vita3K's sampler (0.02%) but each is a syscall on the console |
| **sceClibMemcpy 0.63%** | **0.20** | 0.56% from **decomp_leaves::cameraParamCopy** (gamecore 0x14274A8: the camera parameters, 0x654 bytes, about once a frame): newlib's memcpy sends > 512 bytes to sceClibMemcpy |
| **sceClibMemmove 0.43%** | **0.14** | **sotc_native::shadowCapture**: a skinned shadow's bone palette read into a 16 KB picture, then its rows copied into the job (`assign`, ~3 a frame) |
| callGuest 0.36% | 0.11 | the natives' guest calls (inline already, round 42) |
| strips' lambda veneers 0.34% (all veneers 0.92%) | 0.29 | long-branch stubs between far functions (55 MB of text): the strips' per-quad lambdas live away from strips |
| udivsi3 0.27% + udivmoddi4 0.12% | 0.12 | divisions (callers not visible: __aeabi_uidivmod's LR) |
| memset 0.29% + **sceClibMemset 0.22%** | 0.16 | sceClibMemset from **decomp_ctrlturn::ctrlTurn** (0.20%: a zero-initialised block of 32 VU0 registers, 512 bytes, ~12 calls a frame) |
| sceKernelGetTLSAddr 0.20% (+ getreent 0.07) | 0.09 | newlib's reentrancy struct (errno, stdio) |
| checkpointSlow 0.19% + updateGate 0.11% | 0.10 | the scheduler's checkpoints |
| getThreadId139 / pollSema139 0.26% | 0.08 | round 139's fast syscalls (isys locks) |

The SceLibKernel imports (sceClib*, the clocks, the TLS, semaphores, lightweight mutexes) are 2.6% of the game thread
in Vita3K (0.83 ms): Vita3K runs them as HLE calls, so one call can weigh much more there than on the console (one
sceClibMemcpy a frame: 0.56%; one clock read a frame: 0.75%). What S9 takes out of them is real work avoided on both,
but the Vita3K gain of those parts can be larger than the console's.

## 2. What S9 changes (`SOTC_S9RT`, parts)

| part | what | exact because |
|---|---|---|
| kEvict (1) | evictKept's heap-only pressure pass with no aged cut (every 256th frame) and no budget cut lists the textures alone (and builds the kept-texture set only when a texture in the newlib heap is old enough) | in that pass the loop spares every mesh (not aged, no budget) and every texture outside the heap; the listed entries are the same, and the loop's order does not matter without a budget cut (round 127) |
| kClock (2) | S5's hand-over stamp and its join's job / wait times read the clock only with the [s5ptcl] log on | log-only values |
| kCopy (4) | cameraParamCopy's 0x654 bytes copied with NEON loads / stores, no libc call | the same bytes (non-overlapping, checked before as before) |
| kShadow (8) | shadowCapture reads a skinned shadow's palette rows straight into the job (readChainPaletteLean, round 32's lean read) | the same rows: zero where no UNPACK wrote, later UNPACKs over earlier ones |
| kZero (16) | ctrlTurn zeroes the 13 VU0 registers its load() leaves alone, one by one (no 512-byte memset) | load() fills the other 19: the same 32 values as the zero-initialised block |
| kHookTime (32) | the display hooks' timer (the frame log's "hooks" time) reads no clock with S9 on (SOTC_S9RT_HOOKTIME=1 keeps it) | log-only: the [frame] lines' hooks field reads 0 on S9 frames |

The eviction's [gl:mem] "kept in pools" count no longer counts the meshes in S9's textures-only passes (it counted the
meshes that pass spared).

## 3. Equivalence

`SOTC_S9RT_CHECK=1` (chk9.sh) runs the checkable parts both ways in the same call, no tolerance:

- **kEvict**: in every textures-only pass the full pass's list is built too (the kept-mesh and kept-texture sets, every
  mesh and texture) and the entries its loop would evict (its own conditions: aged / pressed / budget, spared outside
  the heap) are compared as a set with S9's list.
- **kShadow**: the 16 KB picture path is run as well (readChainPalette) and its rows lo..hi compared with the job's,
  with lo / hi and the empty case.
- **kCopy**: the destination compared with the source after the copy.
- kZero (the same 32 values by construction: load() fills 19 registers, S9 zeroes the other 13), kClock and kHookTime
  (log-only values) are argued, not run twice.

| run (build) | where | frames | checked | differ |
|---|---|---:|---|---:|
| **chkRp1 (p1)** | ride7: mounted at 2 318, riding to the end | 17 400 | 846 eviction passes (textures-only; 60 full passes beside them, 9 549 entries evicted in those), 50 928 shadow palettes, 17 378 camera copies (68 306 checks in all) | **0** |

The counts also give the call rates riding: one camera copy a frame, ~3 skinned shadow palettes, ~9-12 ctrlTurn calls,
one S5 hand-over; a textures-only eviction pass every ~16-20 frames (the heap over its high mark the whole ride), a full
pass (aged) every ~256. Picture: build/agent-scratch/s9/runs/ride7_vita3k_chkRp1_s9check.jpg (S9 on with its check:
Wander on Agro on the plain, the reins, the shadow on the grass, the ground and the sky as before).

## 4. Speed

`SOTC_AB_FLAG=s9rt SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs9.sh): S9 on or off at random each frame, Vita3K 222 MHz,
the sampler on, 800 s a run; tools/mixframes.py (300-frame blocks, ± 2 SE, default shift -1; ride7 from the mount +
600, the others from frame 3 000). Off - on: positive = S9 saves that much a frame.

| run (build, seed) | route | frames used | frame off - on (± 2 SE) | z |
|---|---|---:|---|---:|
| mixR7p2 (p2, 7) | ride7 (mounted late, 9 746) | 4 647 riding | +0.303 ± 0.373 ms (from 3 000: +0.076 ± 0.201) | 1.6 |
| mixR11p2 (p2, 11) | ride7 (mounted 5 416) | 9 021 riding | +0.023 ± 0.224 ms | 0.2 |
| **mixR7p3 (p3, 7)** | **ride7 (mounted 6 963)** | **9 127 riding** | **+0.440 ± 0.347 ms** (from 3 000: +0.404 ± 0.280) | **2.5** |
| **mixR11p3 (p3, 11)** | **ride7 (mounted 2 650)** | **14 553 riding** | **+0.384 ± 0.191 ms** (from 3 000: +0.407 ± 0.181) | **4.0** |
| mixL7p2 (p2, 7) | lake6 | 13 921 | +0.045 ± 0.162 ms | 0.6 |
| mixL11p2 (p2, 11) | lake6 | 13 377 | +0.200 ± 0.345 ms | 1.2 |
| mixA7p2 (p2, 7) | attract | 23 510 | +0.028 ± 0.123 ms | 0.5 |
| mixA11p2 (p2, 11) | attract | 21 695 | -0.001 ± 0.123 ms | 0.0 |

**p2's ride A/Bs read ~0 because the eviction pass ran in the wrong frame's state**: sotc_native set the renderer's flag
after abSwitches() had already chosen the next frame's state, so the pass (once every ~16 frames, ~4-5 ms) landed on
on- and off-frames alike. Re-read aligned (--shift 0) the same p2 runs gave +0.341 ± 0.219 / +0.262 ± 0.226 ms. p3 takes
the flag before abSwitches(); p3's ride A/Bs are the decision runs. Lake and attract (p2; few eviction passes there: 51
in 16 800 frames at the lake, the heap stays under its mark) show no loss.

The sampler, by part (ride, both p3 runs): the eviction pass and its hash sets cost **0.24 / 0.36 ms a frame** in the
off state and ~0 in the on state (evictKept + IdSet::insert, per off-frame). The HLE parts do not show as a saving: in
the S9 runs the samples that sat on the camera copy's sceClibMemcpy and on S5's clock read moved to the next HLE call
of the frame (sceClibMemcpy from decomp_tex64::post 0.59%, the clock from hookjobs::push 0.66%, which only the A/B's
SOTC_FRAME_LOG_ALL stamps): **Vita3K charges a per-frame wait to whatever import call sits at that point of the frame**,
it is not the call's own cost. So kCopy / kClock / kShadow / kZero / kHookTime remove real work (a 1.6 KB copy, ~275
clock reads, a 1-2 KB copy and 16 KB of zeroing, a 512-byte memset a call), worth a few hundredths of a ms each on the
console (kHookTime the most: ~275 syscalls a frame with the console's SOTC_FRAME_LOG=1), but Vita3K cannot price them.

**Decision (z > 2 in both seeds riding): `SOTC_S9RT` stays on** (default on): ride7 +0.44 / +0.38 ms a frame (z 2.5 /
4.0), no loss at the lake or in the attract demo. The ≥ 1.5 ms aimed at is **not** reached: ~0.4 ms of the bucket's 3.5
ms was work S9 could remove without offloading.

### Boot with the console's memory

bootR3: celosia-s9.vpk (p3, S9 on) with `SOTC_VITA_EAT_USER_MB=89` taken (the console's free memory): mounted at
2 339, riding to frame 14 455 (t=740 s), no fault; 711 textures-only eviction passes and 49 full ones (the heap tighter:
12 831 entries evicted, all in full passes, as before S9). Pictures looked at: Wander on Agro galloping by the canyon's
cliffs and the river, the shadow on the grass, the ground and the far walls as usual
(build/agent-scratch/s9/runs/ride7_vita3k_bootR3_s9on_eat89.jpg). spalign (p3): 952 NEON :128 / :256 accesses, 0
sp-based. Final VPK build/agent-scratch/celosia-s9.vpk / .elf (p3); console env build/agent-scratch/s9/env-console.txt
(console-env-latest.txt with s9rt first in SOTC_AB_FLAG; S9 is on by default).

## 5. What this leaves

### The PS2-shaped layers that are left, priced (profR0r, ride7, game thread 31.8 ms)

The user's direction (2026-11-18): the port should run like a native game; take the guest CPU context, the dispatch and
the scheduler off the hot path (direct C++ calls, values in host locals, no checkpoint polling per call). What those
layers cost riding, by the sampler, and what stands in the way of removing each:

| layer | share | ms a frame | what stands in the way |
|---|---:|---:|---|
| guest calls / dispatch: callGuest 0.36%, recompiled() 0.10%, finishGuestCall 0.08%, G::callNative 0.06% | 0.66% | 0.21 | the natives still call guest functions through the table (ctx args, ra / sp, the run-to-return check), because the callee is either recompiled code (3.6% of the thread, 1.1 ms, still runs on R5900Context) or a native whose switch / A/B / difftest / profiler wrapper lives in the table; a direct C++ call needs the callee native and its entry exempt from the table's wrappers (S8: wrapping crashed Vita3K) |
| the scheduler: checkpointSlow 0.19%, updateGate 0.11%, eeScheduler() 0.07% (+ veneer 0.03%), EeScheduler::run 0.06% | 0.78%* | 0.25 | the game still runs several EE threads (sound / streaming / loader / sub-threads) that the frame waits on with semaphores, and VBlank / alarm interrupts are delivered at checkpoints; the checks cannot go before those threads become host threads or host callbacks |
| syscalls: getThreadId139 0.15%, signalSema 0.13%, pollSema139 0.12% | 0.49% | 0.16 | the isys group / object locks ask the EE thread id and poll semaphores a few hundred times a frame; a host lock-free path needs the isys lock users known (the guest threads above) |
| guest memory helpers (write32, the guest memcpy / memset stubs, IO registers) | 0.23% | 0.07 | the recompiled callers |
| R5900Context traffic inside the natives (ctx loads / stores of GPRs, VU0 registers, FPU) | not separable | — | the sampler charges it to each native; natives that take and return values in host locals need their guest callers native too (a chain, function by function) |
| SceLibKernel imports on the game thread (sceClib*, clocks, TLS, semaphores, lightweight mutexes) | 2.6% in Vita3K | 0.83 in Vita3K | mostly Vita3K's per-frame wait charged to whichever import sits there (above); on the console a fraction of that |

(*) the explicit symbols; the inline gate compare in every recompiled / native block (eeCheckpointDue) is inside those
functions and not separable either.

So even all four explicit layers together are ~1.2% / 0.7 ms riding in Vita3K; the bigger PS2 shape is that the
natives keep guest state in R5900Context and guest RAM and talk to each other through it, which is not a runtime
symbol but spread over the 85% of the thread the natives are. The next structural step that can buy whole
milliseconds is not in the runtime layer but in turning a chain of natives into one host routine with host data (as S3
did for the pose and S7 for the wind), and turning the remaining EE threads into host threads so the scheduler and its
checkpoints can go.

### Other leads found

- **Veneers: 0.92% of the game thread (0.29 ms)**: long-branch stubs between far functions (55 MB of text); the strips'
  per-quad lambdas alone 0.37% (their sections are not in game/overrides/vita_hot_order.ld, which round 121 already
  called stale). Regenerating the hot order (tools/vita_hot_order.py) is exact and cheap, but a link-order change cannot
  run in an interleaved A/B: it needs two builds measured apart.
- Divisions (__aeabi_uidivmod 0.27%, __udivmoddi4 0.12%) and newlib's 32-512-byte memcpy (1.2%, spread over ~80-a-frame
  sites in the particle step, emitters and S5) have no single caller worth a switch.
- The console: SOTC_S9RT's kHookTime takes ~275 clock syscalls a frame off the game thread with the console's
  SOTC_FRAME_LOG=1 (and kClock one more); the hook worker still reads the clock ~290 times a frame for the log's job
  times (FRAME_LOG_ALL only for the lag, but jobUs with FRAME_LOG): a candidate on the worker side.

