# Round N5b: n5sched boots — what stalled the boot, the fix, the measurements (2026-11-20)

Plan: docs/native_all_plan.md, step N5 (follows docs/native_n5.md). Base: main 478a8df. Scratch:
`build/agent-scratch/n5b/` in the main checkout (NOTES.md has the exact next commands; scripts were written in the
worktree's `build/n5bw/` and copied there, because the worktree sandbox refuses direct writes outside it). Vita3K
trees ../vita3k-celosia-n5b (N5's, reused) and ../vita3k-celosia-n5c (a copy of d17), 222 MHz, 3 cores, muted.

## 1. What spun, and why it was not a spin

N5 saw n5sched (no time slice between same-priority threads) never leave the boot: ~4 game fps, no draws, no mount.

- **Who ran** (spin1: main's build, ride7 200 s, `SOTC_N5SCHED=1 SOTC_N5_LOG=30`, the sampler): guest thread t5 took
  237-443 ms of host time a game frame, every other guest thread under 1 ms. The sampler put 44% of the EE host thread
  in `iosRecvMsg` called by `iosRecvMsgHook` called by `kernel_sub_001AD270`: t5 is SotC's idle thread (KERNEL
  0x1ad270, priority 125, the lowest), a loop that polls the idle queue 0x1fc870 forever. That loop is normal: it runs
  whenever every other thread waits.
- **Why its polls were slow to end** (diag2, a new `[n5]` field: why `idleWait` returned at once): every one of 4.2 M
  polls in 60 frames returned because **host clock sync was off**, i.e. GAMECORE had never been entered. Before
  GAMECORE, EE time moves only by executed cycles (so the boot's heap layout stays reproducible), and the idle loop
  charges few cycles a poll: one VBlank per ~240-440 ms of host time.
- **Why GAMECORE was never entered** (diag1's log, the new `[n5t]` thread states): the loader thread t6
  (IosCdvdManagerSimulation, 0x1b5a00, priority 45) went dormant at the jump to a module entry:
  `[guest-branch:missing-target] kind=IndirectCall op=JALR source=0x1b5bcc target=0x124a448` — the module was not where
  the recompiled code expects it. The other threads then waited on each other (t9 and t10 asleep in iosSendMsg,
  0x1b10dc, on the frame box 0x26d9a8 and 0x26e9d0; t8 in its VBlank wait) and only the idle thread ran.
- **Why the module moved** (h1on / h1off, `SOTC_LOG_MALLOC=1`, the boot's 33 heap calls on / off): with the time slice,
  the main thread t9 (also priority 45) gets turns while the loader t6 runs, and its allocations (ra 0x1bfa08 2 x
  0x7c800, 0x1bfa4c 2 x 0x630, 0x1c74e8, 0x1bef98, 0x1c3144) land before GAMECORE's image buffer (0x20aea8 bytes, ra
  0x1b5558: at 0x1333500). With n5sched t6 runs until it blocks, its allocations come first, GAMECORE's image lands at
  0x12391c0 and every module after it moves, so the jump at 0x1b5bcc hits an address with no recompiled code. The
  same class of problem as round 2026-10-16's letter post (heapHook in sotc_overrides.cpp): the XFF modules were
  captured with the boot order the time slice gives.

So the boot did not rely on a polling loop that needs the time slice; it relies on the slice's **order of heap
allocations** between two priority-45 threads, which fixes where the code modules are loaded.

## 2. The fix (exact)

n5sched takes effect only once GAMECORE runs (sotc_native.cpp, the per-frame switch line:
`switchOn(kN5Sched) && gamecoreEntered()`). The boot up to GAMECORE's entry (~3.4 s, all by executed cycles) keeps
the time slice and is the same as with n5sched off — the same heap calls, the same layout (GAMECORE entered at 3.45 s
on vs 3.48 s off). From GAMECORE's entry on, threads of equal priority switch only at explicit points.

Kept as guards (both counted on the `[n5]` line, both 0 in every run): `EeScheduler::n5IdleYield` (after an idle-thread
poll, a ready thread of the idle thread's priority gets the turn: no other thread has priority 125 in SotC) and
`SOTC_N5_SPIN=<slices>` (off by default: equal-priority preemption after that many slices without a switch).
Diagnostics added: `[n5]` idle-wait reasons, `[n5t]` every guest thread's priority, state, wait reason, pc / ra / a0 /
s0 / entry (with `SOTC_N5_LOG`).

## 3. Measurements (p4 = N5b's build; Vita3K 222 MHz; off - on, ms a frame, ± 2 SE)

- In-run A/B (`SOTC_AB_FLAG=n5sched SOTC_N5SCHED=1`, `SOTC_AB_MIX`, mixframes; on = n5sched): ride7 800 s seed 7
  **-0.081 ± 0.139 ms**, seed 13 **+0.101 ± 0.122 ms**; lake6 400 s LAKE_PENDING.
- Boot: d4on (n5sched on from the start, ride7 300 s): GAMECORE at 3.45 s (off: 3.48 s), mounted (frame 2 848), rode at
  ~33 fps, pictures normal; spin preempts 0, idle yields 0. 600 s boot with 89 MB taken: BOOT_PENDING.
- Load: the boot to GAMECORE is the same code path as off (3.45 vs 3.48 s); the mount time on ride7 depends on how many
  teleport tries the route needs (off1 needed 4, d4on 1), so it is not a load-time measure. SOUND_PENDING
- spalign (p4.elf): 0 sp-based :128 / :256 NEON accesses (of 951).

## 4. The next step for N4: the dispatch checkpoint off the call path (not done: needs a full runner rebuild)

With n5sched on, a call into a function that cannot block and has no loop anywhere below it can never unwind: the only
ways out are the dispatch checkpoint at the call itself (`callGuestKnownBody`: `eeCheckpointDue(kEeDispatchCycles)`)
and a back edge's checkpoint. Exactly how:

1. **The set**: from docs/n5_blocking.txt, the NOBLOCK functions whose whole callee tree has `loops=0` (a closure over
   the `via` / call edges tools/n5_blocking.py already builds: a function qualifies when it has no loop and every
   callee qualifies). To do: a `--noloop` output in n5_blocking.py writing `docs/n5_noloop.txt`.
2. **The call form**: `PS2_CALL_GUEST_NOCHK(target, fn, returnPc, sourcePc, kind, name)` in ps2_runtime_macros.h ->
   `PS2Runtime::callGuestKnownNoCheck`: the same as `callGuestKnownBody` with the checkpoint test replaced by the charge
   alone (`m_eeClock.cycle += kEeDispatchCycles`; the next back edge's or dispatch's checkpoint sees the cycles and
   anything pending), behind the same slot guard as N3's direct calls (`g_ps2DirectCalls`: the slot must still hold the
   static target, else the plain call), and only while `g_ps2xN5Sched` (else the plain call: the slice's end can be
   due at the call). After it, `ctx->pc == returnPc` holds by construction: no `finishGuestCall`, no resume label.
3. **The generators**: tools/lift_d1.py (and a pass like tools/n3_direct.py over the 52 lifted batches) write
   `PS2_CALL_GUEST_NOCHK` for calls into the set; ps2xRecomp's function emitter the same from the list.
4. **Cost**: ps2_runtime.h / ps2_runtime_macros.h change -> every one of the ~9 000 recompiled runner objects
   rebuilds (~1 h at -j4), or N3's mtime hack (headers edited, mtimes set back, only the rewritten lifted batches
   rebuilt). Measure with an in-run A/B (a runtime flag `n5nochk` read in `callGuestKnownNoCheck`), then N4's register
   passing between such functions (docs/native_n4.md "What N4 needs", step 2).
