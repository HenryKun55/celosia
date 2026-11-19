# S22: the particle draw list without a guest call per node (2026-11-19)

Round S22's brief: S19's ride profile put callGuest at 0.77% of the game thread, 0.53 of it from
`decomp_ptcl117::listImpl` (the particle draw list, 0113ACB8: a guest call per node of the list, and the guest qsort
with a guest comparator call per comparison for each run of nodes sharing a draw key). Redesign the list so it runs
host-only, keeping the picture identical.

Switch: `SOTC_S22PTCL` (A/B name `s22ptcl`; sotc_decomp switch `kS22Ptcl`, after kS19Amb, the last of the tables;
round S21 adds kS21Light at the same place, the coordinator orders them at the merge). Check mode
`SOTC_S22PTCL_CHECK=<n>` (every n-th list), counts `SOTC_S22PTCL_LOG=<frames>` (`[s22ptcl]` lines, every 600 frames
with the check). Code: game/overrides/decomp_ptcl117.cpp (`listHost`, `s22::qsortBsd`, `nodeBody`, `s22Check`).
Scripts, runs and logs: build/agent-scratch/s22/ (clone.sh the build dir cloned from S20's, chk22.sh the check,
abs22.sh the interleaved A/B, chainA.sh / chainB.sh the build-then-queue chains, vq.out). Own Vita3K trees
../vita3k-celosia-s22 / -s22b (222 MHz, 3 cores, muted), deleted at the end.

## 1. What the list did

0113ACB8 (round 117's `listImpl`; a0 the emitter list's first node, a1 / a2 the particle class's view vectors):

- the layer from the LOD record's kind and the record's +0xc4 with the global at 0x1216ed0 (or the whole list skipped),
  modelResetSurface for layer 0x12, iosDlPushLayer;
- the nodes in runs of the same draw key (record +0x34): a run of one node straight to 01140ED0 (the node: 0113B408,
  the emitter's once-a-frame view pass, then a tail jump into disp_particle_emitter, which calls the drawer by kind);
  a longer run as an array of node pointers on the stack, qsorted with 01140EA8 (`(*a)->+0x144 < (*b)->+0x144 ? 1 :
  -1`: far first) and drawn in that order;
- iosDlPopLayer.

As host code since round 117, but every node still went through `callGuest` (table lookup, indirect call, the
run-to-return check, the thread-transfer check) into `node`, which redid its bounds checks, and every sorted run went
into the lifted libc qsort (0116A310), whose every comparison is a `PS2_CALL_GUEST` of the comparator's recompiled
code.

## 2. What S22 changes (`listHost`)

- One pass over the nodes: node's checks (the node, its record, the model record, the LOD records in main RAM) made
  once per node; the runs marked by their ends. Anything outside them: the old list (`listImpl`), nothing written yet.
- Each run's order from a host copy of the game's qsort (BSD 4.4 / newlib, as the lifted code reads: swaptype, the
  insertion sort below 7, med3 / the ninther past 40, the swap_cnt fallback to the insertion sort, recursion on the
  left part, the loop on the right) over (node, key) pairs, the key the node's +0x144 as the runtime's FPU compare key
  (`ps2FpuCompareKey`, what `c.lt.s` compares). The comparator never answers 0, so the order of equal depths is the
  algorithm's, which is why the algorithm is copied rather than any host sort used.
- Each node's body (`nodeBody`: 0113B408's view pass, then disp_particle_emitter) called straight, with the registers
  the original call had (s0-s7 and fp as the list keeps them, a0-a2, ra = the call site + 8, sp the list's frame or
  the lowered sort frame), so the drawers see the same registers and sp; then the transfer check and the
  run-to-return fallback as callGuest did.
- The layer push / pop, modelResetSurface and the drawers stay calls of the table (once a list, or the emitter's own).

Not as the original: the list's and qsort's save slots and the sorted pointer array on the stack (dead below the
caller's sp), the scratch registers qsort and the comparator leave (f0 / f1, fcr31's C, a0-t3).

Only on the game thread (not the guest worker's shell) and while the table holds round 117's `node` and
`dispEmitter` natives, or round 95's gates around them (sotc_worker.cpp gate<I>: the table entries of the particle
module are wrapped, so p1's host path never ran; `sotc_worker::behindGate` names the native behind a gate). A gated
node is called straight only while no worker job is out and no early95 skip is pending (after S5's join, the gate's
first step); else through the table as before ("gate calls" in the counts).

## 3. Equivalence (`SOTC_S22PTCL_CHECK=1`)

chkRp3 (ride7 600 s, every list checked): 464 519 lists, **0 differences**, 0 skipped, 0 gate calls. Riding, every
list held exactly one node (929 037 nodes over 464 519 lists x2 with the check's leg): ~85-90 lists a frame, one node
each, so no run was sorted and the host qsort copy was not exercised riding. Lake6 / attract checks were queued when
the round was cut (see 6).

## 4. Speed

Not measured: the interleaved A/Bs (ride7 800 s seeds 7 / 13, attract 400 s) were running when the round was cut.
Expected saving: ~90 callGuest + gate dispatches a frame riding, i.e. well below 0.5% of the game thread.

## 5. Boot and pictures

chkRp3's shots (S22 on): riding, Agro's dust normal. The 600 s boot with 89 MB taken was not run. spalign: 0 sp-based
NEON accesses (p3).

## 6. What it leaves

- The A/Bs, the lake6 / attract checks (the only places a run of 2+ nodes, so the qsort copy, can be checked) and the
  boot. Off by default until then.
- The drawers (particleDraw, the quads, ...) are still table calls from disp_particle_emitter; with one node per list,
  the bigger cost is per list (85-90 lists a frame, one per geodisp), not per node: the geodisp draw callback ->
  list -> node -> emitter chain could become one host walk from the display walk.
