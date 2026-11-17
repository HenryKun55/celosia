# S5: particles, second pass: the step off the game thread (2026-11-16)

Round S5 is the second particle round after S1 (docs/particles_s1.md). S1 made the particles' draw records host code
and batched the per-particle step per emitter. S5 takes the per-particle step, and the draws' per-particle values, off
the game thread onto a host worker, with the result identical to the old path.

Switch: `SOTC_S5PTCL` (default on; `=0` gives S1's path back). A/B name `s5ptcl`. Sub-switches: `SOTC_S5PTCL_DRAW=0`
(the worker steps only, the draws compute their values as S1), `SOTC_S5PTCL_THREAD=0` (no worker thread: the game
thread runs the batch at the join; a test of the batch alone), `SOTC_S5PTCL_LOG=1` (`[s5ptcl]` lines every 600
frames), `SOTC_S5PTCL_CHECK=1` (the in-run equivalence check, below).
Code: game/overrides/s5_particles.cpp / .h; small hooks in decomp_particles.cpp (the host step), decomp_ptclupdate.cpp
(the emitter's particle loop), decomp_hot47e.cpp (the manager's window), sotc_worker.cpp (the module gates' join),
s1_particles.cpp (the draws take the worker's values), decomp_ptcl117.cpp (joins), sotc_native.cpp (the frame's end).
Scripts, runs and logs: build/agent-scratch/s5/ (STATE.md; abs5.sh the interleaved A/B, chk5.sh the check; authored in
the worktree's build/agent-scratch/s5src/).

## 1. What was left on the game thread after S1, and what tied it there

PLACEHOLDER_PROFILE

The per-particle step (S1's `step123`) is plain float arithmetic on the 0xB0-byte record and the emitter's per-frame
values (`Emitter123`, read out of guest memory once per emitter). Three things tied it to the game thread:

- **the floor / ceiling reflection**: a guest call to iosGetReflectVector (boot ELF 0x1BE8B0) per bounce;
- **the damage test**: a guest call per particle (0113C8C8), only while the damage target is set (a colossus fight)
  and the effect has a damage handle;
- **the host random stream** (`g_rndState`, round 123's xorshift for the jitter and shape 4's random axis): the draws
  must come in the order the particles are stepped. The game's own generator (VU0's R register) is not touched by the
  step: the emitters' rates and the new particles (0113C1A8) draw from it on the game thread, which is why the emitter
  frame itself cannot move without changing the game's random sequence.

## 2. The redesign

### The step as host code (`decomp_particles::stepT<true>`)

S1's step and S5's are one template: `stepT<false>` is S1's `step123` unchanged (guest reflection, damage test), and
`stepT<true>` is the same statements on any host copy of the record, with the reflection as `reflectHost` (the
recompiled function's seven VU0 operations on locals, with the same PS2_V* macros: the same bits) and no damage block.
The random state is an argument (`rndOf(state)`), so the worker and the check can each own one.

### The batch (structure of arrays)

Inside doit_particle_manager's native (decomp_hot47e: `managerBegin` / `managerEnd`), the emitter's particle loop
(decomp_ptclupdate's `particles`) asks `wantDefer`: yes unless the window is closed (not the manager's native, round 91's
job path, the gates not installed) or the emitter's damage test is on. Then `deferRun` walks the live list exactly as
S1's `stepRun` does, but where `step123` would step a particle (its own test: in RAM and aligned, not dying this
frame, a shape, not waiting for a collision) it appends the record's address to the batch instead and counts it as
stepped. Every other node (dying this frame, the collision path, shape 0, outside RAM) goes to the original in its
place in the walk, as before. The loop's count of stepped particles is unchanged, so the emitter's "finished" answer,
its spawns, its end and the manager's layers are what they were. The batch is three host arrays: the record addresses
(in step order), the emitter segments (each a range of records plus its `Emitter123`), and the draw values below.

An emitter stepped on the game thread after part of the batch was built (its damage test on, or update123's path)
consumes the same random stream, so the batch so far is run first (`beforeInlineStep`): the stream is consumed in the
original order.

### The worker and the joins

At the manager's end the batch goes to a host thread (`native_renderer::threads::kOffload`: cores 1-2), woken by a
condition variable, which steps the segments in order with `g_rndState` and then, per segment, makes the draws'
per-particle values (below). It needs no guest context: it reads and writes the records through the rdram pointer the
game thread handed over and calls nothing.

The game thread joins before anything reads a record again:
- **every module entry** through sotc_worker's gates (installed by default since round 91), except entries checked to
  read no particle record (`s5ptcl::allowed`): disp_particle_manager (the screen matrix), clip_particle_geodisp and its
  emitter walk / is_emitter_need_disp / 0113B1D8 (emitter and record fields, the live count), the light pass
  (calc_light_particle_geodisp, 01140C80, 01140E60), set_npe_blend_ratio_particle_emitter (E +0x16C),
  get_particle_emitter_from_emitter_id and get_top_emitter_list_from_particle_geodisp. Each was read in the
  recompiled code (build/runner): no load from a Particle (+0x128 the first, +0xA8 the links);
- **the draw natives** that a native may call straight (disp_particle_geodisp, disp_particle_emitter, s1ptcl's entry);
- **the frame's end** (before the A/B switches flip) and the next manager call.
In the display walk the first reader is the first particle draw, after the group pass, the sort and the light pass.

### The draws' values from the worker

S1's `records()` computes per live particle the sin / cos pair of angle + pi/4 (the PS2 polynomial, a range reduction
with a division, a square root) and the fade. The worker now makes those three values for every deferred particle
right after stepping it (`s1ptcl_api::drawPrep`, the same functions), into SoA arrays in batch order. `records()` walks
the emitter's list as before; a node equal to the next deferred record of that emitter's segment takes the three
values, any other node (new this frame, stepped on the game thread) computes them. The segment is found by the emitter
(a sorted vector made at the hand-over).

### Not as before

The step's f0 (a scratch FPU register the loop leaves as the last particle's result) is that particle's life when it
was deferred. Nothing reads it: the next callee that receives it, 0113C1A8, starts with iosGetRandom, and the step's
callers treat it as scratch. The step's other guest-visible effects (the reflection's VU0 vf4-vf7, the call frames)
were already undone by the step's epilogue (it puts the caller's VU0 file back).

PLACEHOLDER_REST
