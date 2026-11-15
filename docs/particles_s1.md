# S1: the particle subsystem's first redesign (2026-11-15)

Round S1 is the first subsystem round after the decomp rounds D1-D8. Its pilot is the particle subsystem (the particle /
wind module 0x01139CA0-0x01147530 and ptclman 0x01099E00-0x0109A4D0, docs/particles_map.md). The aim is code that runs
the way a modern simple machine wants it (the PS Vita above all), not the PS2's way, with the same gameplay and picture.
Functional equivalence is enough where nothing observable depends on bit-exact guest state.

Switch: `SOTC_S1PTCL` (default on; `SOTC_S1PTCL=0` gives the original path back). A/B name `s1ptcl`.
Code: game/overrides/s1_particles.cpp, s1_particles.h, ptcl_types.h, and small hooks in decomp_ptcldraw.cpp,
decomp_hot46j.cpp, decomp_ptclupdate.cpp, decomp_particles.cpp and sotc_native.cpp.
Scripts, runs and logs: build/agent-scratch/s1/ (STATE.md, tools/mixgroups.py, tools/mixframes.py).

## 1. Semantic decomp: what the data is

game/overrides/ptcl_types.h gives the module's guest data C++ names and layouts, pinned with static_asserts. These are
views on guest memory: guest RAM is host memory and little-endian, so a `const ptcl::Particle *` at rdram + address reads
the record in place.

| structure | where | what |
|---|---|---|
| `Particle` (0xB0) | the module's particle pool; free ones on the manager's free list (0x1216DF0: count, first, last), live ones on their emitter's list, linked by +0xA8 | position, velocity, acceleration, a 4th vector (shape 2's end / shape 4's random axis), life (0 = dead) and age, size and growth, alpha and its fade-in / fade-out, three colour channels with speeds, spin / drift / sink factors, the animation frame, the sprite angle and its speed, the "has hit" flag |
| node / `emitter` | a particle geodisp's class +0xE8 heads a list of nodes: node +0 the sheet record, E = node + 0x10, next node at node + 0x1B0 | the emitter's position / orientation / direction and last frame's copies, colour scales (fog-blended once a frame), the view direction (the lit draw's light), done / LOD / carry / spawned, the live list (+0x124), distance and depth to the camera, alpha and size scales, the bounce planes, the end callback, the clip result, the generate flag and blend ratio set from outside, the display flag |
| `effect` set Mr | the sheet record's +0x48 (main) / +0x4C (sub, the blend partner) | the LOD mode and thresholds, the live cap (+0x1C, also the display-list room asked for), fade distances, the sprite's texture extent, the lit draw's brightness / shade, the planes switch, the LOD records (+0xD0 count, +0xD4 array of 0x1BC), the info record (name, damage handle), animation frames and cell sizes |
| `lod` record R (0x1BC) | Mr +0xD4 [E +0x118] | space, total, shape (1 sprites, 2 streaks, 4 lit puffs), generation mode, the blend mode (+0x14), the blend sources, spin, collision, jitter, aim, the drawer variants |
| blended parameters P (0x160) | on the emitter step's stack, rebuilt each frame by get_particle_parameter_blend_doit from R of Mr and of the sub set | emission rate, the acceleration / velocity / growth / turn phases and decays, the channel speeds, jitter, bounce factors, wind factors |
| `DrawRecord` (48) | what every sprite drawer writes per live particle | position + size x scale; colour x scales and alpha x scale x fade, clamped to 0..1; sin / cos of angle + pi/4, or (type 7) the texture cell's offset |
| manager globals | 0x118CFD0-0x118D054, 0x1216DF0-0x1216EF4 | the clocks (0x1216E08 / 18 / 28: dt, dt2, rest, VBlanks) and the step pointer 0x118CFE8 that picks one; the renderer switch 0x118CFF0; the free list; the view rows 0x1216E40; the layer switch 0x1216ED0 |

Who outside the module touches them (why the records stay in guest RAM for now): the effect users hold emitters by id
(the node address) and write only through module entries: set_npe_blend_ratio_particle_emitter (E +0x16C; the dissolve
geodisps, the sword's light, the boss effects, the geometry-pointer kick), set_generate_particle_flag (E +0x168),
end / stop, and the sheet readers (get_main / sub_particle_effect_lod_from_sheet). The end callback (SequenceKickParticle's
0x011697E8) and the collision finder (E +0x170) are the module calling out at an emitter's death. Nothing outside the module
reads a `Particle`; the drawers' records are read only by the renderer's job. So the particle records could move to host
memory once every module function that touches them is host code; today 0113C1A8 (a new particle, lifted code),
0113D280 (the collision path) and the damage test 0113C8C8 still read them as guest records.

## 2. The redesign

### Draws straight into the renderer's job (`s1ptcl::draw`)

The PS2's way: walk the live list of 0xB0-byte records, write one 48-byte record per live particle into the GS display
list (ptclDisplayListGet's room), then three packets for VU1 microcode 7. The earlier rounds kept that shape in host
code: the three sprite drawers (particleDraw 0113E940, 0113DBF8, 0113E6C8; decomp_ptcldraw / decomp_hot46j) emulated the
guest's registers, stack frame and FPU / VU0 operations record by record, called iosGetSinCosf per particle through the
guest ABI and wrote the records to guest RAM, and the renderer's hook copied them back out (guestBlock) into its job.
Rounds 103 / 117 had removed the packets only.

S1 builds a system's records the way the renderer consumes them. The records are plain host floats, computed from the
typed views four lanes at a time (colour x scales, then the clamp; SSE intrinsics, NEON on the Vita through sse2neon),
straight into the hook job's data (`sotc_native::ptclS1Begin` / `ptclS1Commit`). There is no guest display list, no copy
and no guest call per particle. The sin / cos pair uses the PS2 polynomial (round 122's plain-float form, the
coefficients read from the boot ELF), inline. Two guest calls are left per system, and both answers belong to other
subsystems: ptclDisplayListGet (the original draws nothing when the display list has no room for the effect's cap, so
S1 asks and does the same) and gluGetBrightness (the lit draw's shade). The blend and the environment are set as the
direct path set them, only when something is drawn, so the renderer's carried-over state is the same.

Not as the original: the floats are plain products, while the original's FPU macros round toward zero and clamp at the
PS2 maximum, so the last bits differ. The display list's room is no longer written; only the renderer's hook read it.
The drawers' scratch registers, VU0 registers and stack frame are not left as the original left them; their callers
reload what they use, and the RAM check below shows nothing reads them.

### The particles' step as one batch per emitter (`decomp_particles::prep123` / `step123`)

Round 123 had already put the per-particle step (particleUpdate) into plain host floats (update123). But it re-read and
re-derived, for every particle, what is the same for all of an emitter's particles in a frame: seven pointer checks,
dt / dt2 / VBlanks, the space, shape, spin and jitter modes, the parameters' phase limits and values, and nine decay
factors raised to the frame's VBlanks by repeated products. S1 derives them once per emitter (`Emitter123`) and runs
each particle's step on them. The float operations are the same and come in the same order on the same values (a decay
raised to n is a function of its two arguments), so the result is **the same bits as update123**, with the same random
draws in the same order. Particles that update123 hands to the original code (dying this frame, collision mode without a
hit, shape 0) still go there. The damage test is asked per particle, as before.

### What did not move off the game thread, and why

Round 91's worker can run the module's step as a job (`SOTC_PTCL91`), but it stays off, and S1 leaves it off. The
console's verdict (round 95, celosia-r94): with the job on, the 3D frames were **2.5-3 ms slower** on the game thread.
Cores 1-2 are 70-85% busy with the render thread, the hook worker, audio and the helpers. The job (2.6-5.6 ms of wall
time there) waited for a core, and the display walk's join waited for the job. Round 104 / 111's character jobs reached
the same conclusion (exact, but no overlap to win). The way to take particle work off core 0 is to make it host code
that needs no guest context. Then it can go wherever there is room (the hook worker already expands the sprites, and
records built from host data could be built there too). Moving the guest-shaped step around does not do that. S1 does
the first half: the draws no longer touch guest state per particle.

## 3. Equivalence: how it was proven

Two in-run checks compare the original path and S1 on the same frames from the same state. A run-to-run comparison was
not used: two runs of the same build do not repeat frame for frame (timing and host threads), so per-frame state from
two runs cannot be lined up.

**Per draw** (`SOTC_S1PTCL_CHECK=<n>`): every n-th drawer call builds S1's records into a side buffer and then runs the
original drawer. When the original's job reaches the renderer's hook, the two are compared (sotc_native::s1CheckAgainst):
the record count, texture, display layer, blend, environment (type, matrix, extent, fade, light / shade) and every word.
The records' last two words are compared only for type 7, the only type the renderer reads them for.
Tolerance: 1e-5 absolute or 1e-4 relative. The S1 records are plain-float products of the same inputs, and the original's
FPU macros differ from IEEE round-to-nearest by at most an ulp or two per operation. A word fails only when it is over
both bounds.

**Whole RAM** (`SOTC_S1PTCL_RAMCHECK=<n>`, Mac): every n-th drawer call copies main RAM and the context, runs the
original drawer, keeps the resulting RAM, puts RAM and the context back, runs S1 (its records kept aside), and compares
all 32 MB. A word may differ only in the dead stack below the caller's sp or in the display list's room (the original's
records). Anything else would be state game code could read, and is counted and named. The run then continues as the
original.

**Step** (`SOTC_S1PTCL_CHECK=<n>`, the same switch): every n-th batched particle is stepped by step123, the record and
the random state are put back, update123 runs, and the two 0xB0-byte records, the random state and the returned value
are compared **bit for bit**. Particles whose damage test calls the guest are left out of the check, since the call would
be made twice.

| run | where | checks | result |
|---|---|---|---|
| chk2 (Mac, native renderer) | attract demo, 300 s | 104 000 draws (lit 40k, sin/cos 54k, animated 10k), 28.4 M words | count / env / state 0 differ; 25 words over the bound, all the cos lane of the sin/cos pair where sin is about +-1 (sqrt(1 - s^2) amplifies the last bit of the angle: max 2.0e-4, an angle error under 0.012 deg) |
| ram1 (Mac) | attract demo, 300 s | 1 050 whole-RAM checks | 0 words differ outside the dead stack (39 443) and the display list's room (317 098) |
| lakeC (Vita3K 222 MHz) | Avion's lake: console card, teleport to the lake, the boss's set-up, arrows into the water, swimming, back to the shrine (in_tbz2.txt), 716 s | 648 000 draws, 13.1 M words | count / env / state 0 differ; 5 words over the bound (the same cos lane, max 2.9e-5) |
| chk4 (Mac, p2's step) | attract demo, 300 s | 3.36 M particle steps; 104 000 draws, 28.2 M words | steps: **0 differ** (bit-exact); draws as chk2 (17 cos-lane words, max 9.3e-5) |
| chk5 (Mac, p3's stepRun) | attract demo, 300 s | 3.16 M particle steps; 100 000 draws, 25.9 M words | steps: **0 differ**; draws: 18 cos-lane words (max 9.6e-5) |
| lakeOn (Vita3K, p3, S1 on) | Avion's lake route, 716 s | 648 000 S1 draws, no fallback | No exact recompiled function 0; Wander y 3.8..11.3 as the original's run |

Gameplay state: the draws write nothing game code reads (the RAM check), and the step is bit-exact. So nothing the game
logic reads back differs.

## 4. Speed (Vita3K 222 MHz, interleaved A/B)

`SOTC_AB_FLAG=s1ptcl SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`: S1 is on or off at random each frame, so both states see the
same content. Two measures:
- **The sampler** (tools/mixgroups.py): the game thread's samples in the code only one state runs, per 1 000 frames of
  that state. S1's draws and batch (s1ptcl::*, ptclS1*, step123 / prep123) are set against the original draws' own code
  and update123 (decomp_ptcldraw, hot46j drawSinCos / drawAnimated / record / fade / sinCos, getSinCos,
  ptclDirect103 / 117, ptclVtxTake, ptclEnvTake, guestBlock).
- **Frame times** (tools/mixframes.py): per-frame "[frame] each" lines (`SOTC_FRAME_LOG_ALL=1`). The state of each
  frame comes from the same hash. Frames are paired in blocks of 300 consecutive frames (the same content), and the block
  differences are averaged with +- 2 standard errors. "busy" = frame - wait (the game thread's CPU).

Builds: p1 = the draws only; p2 = + the first step batch (slower, see below); p3 = the shipped code (the draws + stepRun).

| run (build, seconds) | where | S1's code (per its own frames) | the code it replaces | saved, share of the game thread | frame time off - on | busy off - on |
|---|---|---|---|---|---|---|
| mixA (p1, 120-1440 s) | attract demo | 0.56% | 1.22% | 0.66% (the draws: -54%) | -0.005 +- 0.120 ms | +0.055 +- 0.133 |
| mixE (p3, 120-1440 s) | attract demo | 0.81% | 1.23% | 0.42% (draws + step: -34%) | -0.091 +- 0.122 | -0.105 +- 0.116 |
| mixE, riding (frames 13 000+) | Agro at a gallop | | | | -0.109 +- 0.130 | -0.126 +- 0.142 |
| mixD (p3, 120-720 s) | Avion's lake route | 1.49% | 2.34% | 0.84% (-36%); ~0.37 ms of a 43.7 ms frame | -0.024 +- 0.170 | -0.033 +- 0.169 |
| mixC (p2, 120-720 s) | Avion's lake route | 2.49% | 3.35% | 0.86%, but step123 alone **1.7x** update123 | -0.125 +- 0.215 | -0.105 +- 0.212 |
| mixB (p2, 120-1440 s) | attract demo | 2.36% | 3.07% | 0.71%, step123 1.7x update123 | -0.026 +- 0.102 | +0.016 +- 0.092 |

Inside p3's set at the lake: stepRun 0.55% of its frames against update123's 0.70% (the batch -21%); the draws
(s1ptcl::records and the rest) against decomp_ptcldraw / hot46j / getSinCos / ptclDirect / ptclVtxTake / guestBlock.
The sampler's z scores are 50-100: S1's code is clearly cheaper than the code it replaces, by 0.4-0.85% of the game
thread, or about 0.1-0.37 ms a frame at Vita3K 222 MHz. That is all the code-level evidence shows.

**Honest reading of the frame times.** None of the six frame-time A/Bs resolves a difference. Each is within 2 SE of zero,
and all six lean slightly negative (pooled about -0.05 +- 0.06 ms). The sampler's expected gain (+0.1 to +0.37 ms) sits
at the edge of what these runs can resolve: per-frame noise is ~5 ms, so 4 500-16 000 frames per state give 2 SE of
0.1-0.2 ms. A method check did not help. mixV toggled round 36's "fewer particles" (`sacptcl`) the same way, and came
out at +0.056 +- 0.141 ms (riding +0.040 +- 0.172). That switch halves emission rates and caps, so its effect builds up
over a particle's lifetime and is diluted when the state flips every frame. It cannot calibrate the method. Shifting the
per-frame state by +-1 or 2 frames (a mis-assigned frame number) gives the same noise (+-0.17 ms), so no offset hides a
clear effect either. Until the console's A/B (env-console.txt) says otherwise, the claim is: **S1's particle code costs 34-54% less than
the code it replaces (0.4-0.85% of the game thread); no frame-time change is measurable in Vita3K.**

Game-thread profile, before / after (mixD, the lake, the particle module's top rows, share over all frames, half of them
in each state): emitter 0.73%, layers 0.52%, step 0.50%, update123 0.35% (off-frames only) / stepRun 0.28% (on-frames
only), blend 0.33%, the draw chain's walk (ptcl117 list / dispEmitter) 0.33% + 0.33%, wind 0.32%, 01146FB8 0.26%,
0113C1A8 0.22%, decomp_ptcldraw::draw 0.21% (off only).

## 5. Pictures

Vita3K 222 MHz pairs, run one after the other or side by side in two trees: S1 on (`SOTC_S1PTCL=1`) and off (`=0`),
the same input and window shots at the same seconds. Runs drift apart, so a pair shows the same stretch of the route,
not the same frame. That is why the per-draw check above is the frame-exact proof. What was looked at:
- **Agro's gallop dust** (attract demo, attOn / attOff, shots every 30 s from 600 s): with S1 the hoof dust trails Agro
  and puffs settle on the ground behind him (attOn t0660, t0810), as in the original (attOff t0840). Same size, colour
  and density.
- **The shrine's light and mist** (the console card's walk / ride input, rideOn / rideOff t0200-t0420): the light shafts
  and the drifting mist at the shrine's entrance are drawn in both. The mist's shape differs between the pair's shots
  because it is animated and the runs drift; nothing is missing or broken.
- **Avion's lake** (in_tbz2.txt with the teleport): the splashes around Wander while he swims (lakeC t0420, original
  path) and S1 on (lakeOn t0240, t0420): the droplets by the wall and the ring of splashes around the swimming Wander
  look the same. lakeOn drew all of its 648 000 sprite draws through S1 (`[s1ptcl]`: lit 312 577, sin/cos 294 078,
  animated 41 344, no fallback). Not seen: the boss's blood and the weak point's glow. They need a hit on a colossus,
  which no route reaches (docs/particles_map.md, the D8 table). Their draws go through the same three drawers.
- Every run: No exact recompiled function 0, Wander's and Agro's [pos] y in the usual range (attract Agro -6..19, lake
  Wander 3.8..11.3), no stall over the watchdog. The shots were deleted after the check.

## 6. What the next subsystem round should take from this

- **Check in-run and from the same state.** Two runs of one build do not repeat frame for frame, so "compare the
  per-frame state of two runs" does not work. Two checks did: the per-call compare of outputs (the renderer's job here)
  and the whole-RAM compare (run A, keep its RAM, put the old state back, run B, diff 32 MB with a list of the places
  allowed to differ). The second one is generic: it proves "nothing the game reads changed" for any rewrite that replaces
  a call with something whose only intended effects are elsewhere (here, the renderer's job). Use it for every
  subsystem round.
- **The tolerance falls out of the arithmetic; write down why.** Plain floats against the PS2's round-toward-zero FPU
  macros differ by an ulp or two per operation. The only words outside 1e-5 / 1e-4 were the cos lane of a sin / cos
  pair computed as sqrt(1 - s^2), which turns a 1-ulp angle difference into 1e-4 near s = +-1 (an angle error under
  0.012 deg). Where the arithmetic can stay the same (the step), keep it bit-exact and check it bit for bit.
- **Hoisting per-emitter work is a real win only if the values stay in registers.** The first step batch (p2) read the
  per-emitter values through a reference while it stored the particle through `may_alias` float pointers. The compiler
  then had to reload every value after every store, and the batch ran 1.7x slower than the code it replaced (mixC /
  mixB). With a local copy inside the batch function (p3, `stepRun`) it is 21% faster than update123. The guest-memory
  views every native uses are `may_alias`, so any host-side redesign has to keep its hot state in locals, or in host
  memory the compiler can see is separate.
- **The threading lesson came before this round, from the console.** Round 91's particle job and round 104's
  character jobs ran guest-shaped code on cores 1-2, which the render thread and the hook worker already keep busy. On the
  console they cost the game thread time. Work can leave core 0 only after it is host code that needs no guest context.
  Then it can go where there is room (the hook worker already expands the sprites).
- **What S1 leaves for S2 on particles:** the emitter frame (decomp_ptclupdate::emitter / step: 0.7% + 0.5% of the game
  thread at the lake, guest-register emulation around five camera reads, the VU0 distance block and the leaf calls), the
  layers pass (decomp_hot47e::layers 0.5%), the parameter blend (0.3%, rebuilt every frame for every emitter although its
  inputs rarely change: a memo per emitter keyed by the ratio and the LOD index), the wind (0.3%) and a new particle
  (0113C1A8, lifted code: 0.2%). Once those are host code, the particle records can move to host memory (SoA pools), and
  the whole frame can become one job with no guest context, worth placing on another core. The order: the per-emitter
  host frame with its blend memo first (exact, checkable bit for bit or with the RAM check), then 0113C1A8 and the
  collision path, then the pools.
