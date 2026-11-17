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

The Vita3K sampler (222 MHz, 3 cores, muted; `tools/s4groups.py` on the four interleaved A/B runs of section 3, both
states mixed, 120-1 260 s of each run) puts the particle area at **4.4-4.5% of the game thread on the attract demo
(1.06-1.10 ms a frame)** and **3.9-4.0% at the lake (1.64-1.68 ms a frame)**. It is flat; the largest symbols (share of
the whole game thread, attract seed 7 / lake seed 7):

| symbol | attract | lake | what |
|---|---:|---:|---|
| decomp_ptclupdate::emitter | 1.03% | 0.91% | the emitter frame (rates, spawns, the guest random calls): stays on the game thread |
| decomp_ptclupdate::step | 0.59% | 0.51% | the emitter's particle loop around the step |
| decomp_ptcl117::listImpl | 0.41% | 0.40% | the manager's emitter list walk |
| decomp_particles::prep123 | 0.32% | 0.29% | `Emitter123` read out of guest memory once per emitter |
| decomp_particles::stepRun (OLD) | 0.29% | 0.24% | **the per-particle step: what S5 moves** (off frames only, so ~0.5-0.6% of an off frame) |
| decomp_ptclupdate::list | 0.27% | 0.24% | the list pass |
| s1ptcl::records | 0.22% | 0.21% | the draws' records (the sin / cos / fade S5 also moves) |

So the part of the area that can move without changing the game's random sequence is the per-particle step and the
draws' per-particle values: about 0.15 ms of an off frame at 222 MHz. The rest (the emitter frame, the list walks, the
reads of the emitter) touches guest state other modules read in the same frame, or the game's own random generator.

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

## 3. Equivalence

`SOTC_S5PTCL_CHECK=1` copies every deferred record before the hand-over, steps the copies on the game thread with its
own copy of the random state (`stepT<true>` on the copy, the same order), and at the join compares every record byte,
the random state, and the draw values the worker made against s1ptcl's own computation of them. It also counts records
written by the game between the hand-over and the join (none may be), records deferred but not stepped, and a damage
test reached inside a deferred emitter (0113C8C8 must never run for one).

| run (Vita3K 222 MHz, 600 s, p5) | particles checked | differ | draw values checked | differ | random state / written before join / damage in deferred |
|---|---:|---:|---:|---:|---|
| attract demo (chkAp5) | 2 132 765 | **0** | 1 730 026 | **0** | 0 / 0 / 0 |
| Avion's lake (chkLp5, teleport + hold) | 1 789 581 | **0** | 1 625 092 | **0** | 0 / 0 / 0 |

A job: 95-99 particles in 68-79 emitters, 0.084-0.088 ms on the worker, started 0.020 ms after the hand-over; the game
thread never waited at a join (joins waited 0). The only join site in practice is the frame's end (site 2): the draws
come after it and before the next manager call, which is why the draw values had to be kept past the frame's end (p5;
p4 computed them and never used them). One module-gate join in an A/B run (01147430, once in 50 000 frames).

Debugging on the way (STATE.md): p2's first check deferred nothing (the raw damage test, `kDamageOn` and a handle,
refused every emitter even in normal frames); p4 uses 0113C8C8's own early test (`damageWanted`: the handle resolved),
which is what decides whether the damage call does anything.

spalign (build/agent-scratch/r110/spalign.py) on the shipped ELF: 0 sp-based NEON :128 / :256 accesses.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, both seeds)

`abs5.sh`: `SOTC_AB_FLAG=s5ptcl SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (the state flips per frame in a seeded order),
1 300 s a run, frames from 3 000 on, mixframes.py (blocks of 300 frames, `--shift -1`), sampler snapshots every 60 s.

| route | seed 7: frame off - on | seed 11: frame off - on | game busy off - on (7 / 11) | sampler NEW / OLD (7 / 11) |
|---|---|---|---|---|
| attract | +0.104 +- 0.098 ms (z 2.1) | +0.107 +- 0.093 ms (z 2.3) | +0.120 / +0.105 ms | 60% / 63%, saved 0.057 / 0.048 ms |
| lake | +0.103 +- 0.119 ms (z 1.7) | +0.092 +- 0.100 ms (z 1.8) | +0.098 / +0.080 ms | 67% / 66%, saved 0.067 / 0.067 ms |

(+- is 2 SE.) The render thread does not move (-0.07..+0.03 ms, all within its 2 SE).

**Decision: `SOTC_S5PTCL` stays on (default).** The gain is small, about 0.1 ms a frame (0.4% of the attract demo's
frame, 0.2% of the lake's), and only the attract runs clear z > 2 per seed; **the lake runs alone fall short of z > 2
in each seed (1.7 and 1.8)**. It is kept because all four runs agree in sign and size (+0.09..+0.11 ms), the four
together give +0.10 +- 0.05 ms (inverse-variance pooled, 2 SE; z about 4; attract pooled z 3.1, lake pooled z 2.5),
the sampler shows the moved code's cost leaving the game thread in every run (NEW 60-67% of OLD, z 45-58), and the
in-run check is exact. The worker's cost (0.07-0.09 ms a frame on cores 1-2) does not show on the render thread.
On the console the step costs more cycles than in Vita3K, so the share should be at least as large there; the console
A/B (`build/agent-scratch/celosia-s5.vpk`, `s5/env-console.txt`: s5ptcl, liftd12, a9sweep, s4disp, a9cmp rotated every
600 frames) will say.

## 5. Pictures

Vita3K cannot take window shots with its window hidden (the user's rule), so the pictures come from the Mac runner with
the same code (`PS2X_HIDDEN_WINDOW=1`, frame-locked shots with `PS2X_SNAPSHOT_FRAMES`), S5 on and off
(`build/agent-scratch/s5/macrun.sh`, the rebased build):

- **Gallop dust**: the attract demo (the opening and the title's demo, 480 s, a shot every 300 game frames). The soft
  dust puffs under and behind Agro's hooves on grass and sand look the same with S5 on and off, as do the opening's
  rain and the forest. The two Mac runs drift apart in time after the opening (the runner paces by the wall clock), so a
  pixel RMSE across the runs measures that drift, not S5; the shots were compared by eye. The ride script
  (r141/in_ride.txt) does not work on the Mac runner (Wander stays in the shrine), so the attract demo stood in for it.
- **The lake**: the in_tbz2.txt route with the 7600 teleport is the shrine altar's view in both states. Wander is put
  at the teleport target (the `[pos]` line prints x / z negated), but the camera stays at the shrine (eye -1855, 50,
  -347), as in the Vita3K A/B runs. So **the water itself was not on screen**: the "lake" A/B and check runs measure
  that scene (the shrine view with the lake area loaded), not a swim. The shrine view is identical by eye in both states.

The shots were looked at and deleted.


## 6. Next

- The emitter frame itself (decomp_ptclupdate::emitter, ~1% of the game thread) is the larger part of the area; it
  draws from the game's own random generator (VU0 R) and writes guest state other modules read, so moving it needs the
  generator's sequence replayed in order (a per-frame log of the draws) rather than a deferral.
- The list walks (listImpl, list) could run over a host view of the emitter list built once a frame.
- The off path (S1's inline step) can go once the console A/B agrees.
