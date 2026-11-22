# Perf P2: the particle frame for the Vita (p2ptcl) (2026-11-22)

Round P2's brief: on the console (docs/console_profile_2026-11-20.md) the game thread takes ~40 ms a frame riding against a
33 ms budget, and the particle / wind module holds 3.5-4.1 ms of it. Move as much of that as possible to the worker cores,
or make it cheaper, with the same picture and gameplay; target 2 ms or more off the console's game thread. P2 ships one
switch, **off by default**, and a console A/B package.

Switch: `SOTC_P2PTCL` (A/B name `p2ptcl`, `kP2Ptcl`). Check mode `SOTC_P2PTCL_CHECK=1` (with `SOTC_P2PTCL_RAMCHECK=<n>`,
default 120), counts `SOTC_P2PTCL_LOG=<frames>` (`[p2ptcl]` lines), sub-switches `SOTC_P2PTCL_LAYERS=0` (the original
layers pass), `SOTC_P2PTCL_PF=0` (no prefetch helper), `SOTC_P2PTCL_PFCODE=0` (the helper asks for no code lines).
Code: game/overrides/p2_particles.h / .cpp (the switch, the helper, the statistics), decomp_hot47e.cpp (`p2layers`: the
layers pass; the helper's kick at the camera manager's entry), sotc_native.cpp (the frame end's kick and log).
Scripts, runs and logs: `build/agent-scratch/p2/` (NOTES.md has every command). Vita3K trees ../vita3k-celosia-c1, -c1b, -c1c.

## 1. Where the particle time goes riding

**The console** (prof129 `e` lines of the two 2026-11-20 N6 runs, windows 1-2, running samples whose guest pc is in the
module 0x1139CA0-0x1147530; `build/agent-scratch/p2/modpcs.py`): 4.08 ms a frame (run1) / 3.47 (run2). By pc (entries
and the return sites after a guest call, which charge the native's own code after that call):

| pc | what | run1 | run2 |
|---|---|---:|---:|
| 0x113B610 | the emitter frame (decomp_ptclupdate::emitter) | 0.37 | 0.33 |
| 0x113B928 / 0x113BD50 / 0x113BDF4 | the emitter's particles (step): its entry, after the wind, after iosGetRandom (spawns) | 0.26 / 0.24 / 0.27 | 0.27 / 0.16 / 0.20 |
| 0x1144F68 | the parameter blend | 0.23 | 0.18 |
| 0x113AF94 | 0113AEF8 (the emitter's position) after GetGeometryPointerQuaternion | 0.22 | 0.17 |
| 0x1145F78 / 0x1145FFC / 0x1146B0C / 0x1146DA8 / 0x1146A50 | the wind zones (also asked by cloth, hair, grass) | 0.50 | 0.33 |
| 0x113A4F8 / 0x113A53C / 0x113A574 / 0x11408E0 | the manager's layers pass (five group walks, 011408E0) | 0.30 | 0.24 |
| 0x113A4AC | the manager between geodisps (the auto-end check) | 0.14 | 0.10 |
| 0x113AAE0 / 0x113AB2C | the emitter list | 0.14 | 0.11 |
| 0x113E940 / E994 / E6C8 / E714 / DBF8 / ACB8 / 1140ED0 / D988 | the draws (from the display walk) | ~0.5 | ~0.45 |
| 0x113B1D8 / 1140C10 / 1140C80 / F530 / FDC8 | the clip and the light-need walk (display walk) | ~0.25 | ~0.22 |

**Vita3K** (census cenR: base build = main's code, ride7 420 s, 222 MHz, sampler 120-360 s; `SOTC_WALK79_LOG=1
SOTC_PTCL_STATS=1 SOTC_S5PTCL_LOG=1`): the step side (manager, emitter, step, blend, prep123, list, layers, S5's
deferral, 0113C1A8, 01141EA8) ~3.1% of the game thread (~1.0 ms of 32.9), the draw / clip side ~1.9% (~0.6 ms).
`[ptcl79]`: riding the manager steps **~40-60 geodisps a frame, one emitter each, 1-3 particles each** (by name: Agro's
HORSE_GRASS_CHIP / BROKEN / DUST, HORSE_SAND_SPOUT, HORSE_MUD_*, BIRD_FEATHER, KITE_FEATHER, the shrine's HOME_IN_FOG);
`[s5ptcl]`: 75-140 particles in 57-105 emitters a job, the job 0.06-0.12 ms on the worker, joined at the frame's end
only, never waited for.

So the particle cost riding is **per geodisp**, not per particle: ~50-60 us a geodisp on the console against ~20 us in
Vita3K (the step side is ~2.8x its Vita3K share on the console, the draw side ~1x). Per geodisp the game thread runs
~45-55 KB of native code (emitter 10 KB, step 15 KB, the manager 9 KB, list, blend, prep123, the leaves, the lifted
0113AEF8) and touches ~50 cache lines of guest data (the geodisp and class, the 0x1C0-byte node, the sheet record, two
effect sets of 0x140, their LOD records of 0x1BC, the particles). The per-particle step that S5 moved is ~2 us a particle.

## 2. What the earlier rounds tell (and why P2 does not move the step to a thread)

- Round 91 (`ptcl91`, the whole step on the guest worker beside the display walk's group pass) and round 95 (the join
  later / the hand-over a camera earlier) lost on the console: with the job on, the 3D frames were 2.5-3 ms **slower** on
  the game thread. The job took 2.6-5.6 ms of wall time on the worker (cores 1-2 70-85% busy: it waited for a core), the
  walk joined 1.3-1.7 ms after the kick, and the hand-over's own costs (the attached emitters' answers, the deferred
  calls) came on top. It was also not exact: the worker drew its own random numbers, sub-effects stepped a frame late.
- S5 moved what is cleanly independent (the per-particle step, host code, the host random stream in order): exact, a
  job of ~0.1 ms, never waited for. What remains on the game thread draws from the game's VU0 random generator in emitter
  order (rates, spawns: 0113C1A8), frees and allocates (dead emitters, end callbacks that kick sub-effects), reads the
  characters' nodes (attached emitters), and is all guest-shaped code on the R5900 context. Running any part of it beside
  the game thread changes the order of the random draws or the heap's, i.e. gameplay-visible state: not allowed here.
- The window after the manager is short: `[s5ptcl]`'s only join site is the frame's end, the display walk (the draws)
  comes after it. There is no long stretch of independent game-thread work to overlap a big particle job with.

So P2 keeps the step where it is and attacks the two things the console pays for that Vita3K does not show, with no
change to what the game computes: the guest-call scaffolding of a pass that need not be a guest pass (the layers), and
the cache lines of the particle data, asked for on a worker core before the game thread needs them.

## 3. What p2ptcl does

| part | before | P2 | exact? |
|---|---|---|---|
| the layers pass (doit_particle_manager's tail) | five passes (layer 4 .. 0), each: isysGroupForFirst (guest call: the isys lock, the iteration count), the group's entries (ptcl83::groupNext), get_top_emitter_list per geodisp, every emitter node's record, and for the emitters of that layer 011408E0 through callGuest (lifted code), then isysGroupForExit | one host pass: the group's entries walked once read-only (the same filters as forFirst / groupNext), the emitters collected with their layer, 011408E0's rule applied in the original order (layer 4 .. 0, group order, list order) on host reads, the budget count kept in a local; every address checked before anything is written (else layers() runs as before) | the same writes (E +0x198 of every emitter of layers 0-4, 0x0118CFF4); not the same: the iteration counts and the isys lock (taken and given back: net zero), the scratch registers (section 4), the calls' frames below sp |
| the particle data's cache lines | the game thread misses them on first touch in the manager and again in the display walk's draws | a helper thread (cores 1-2, offload class, no guest context) walks last frame's particle geodisps (the layers pass's list) and issues PLDs: the geodisp and class, each emitter node (0x1C0), its sheet record, both effect sets (0x140) and their LOD record in use (0x1BC), the live particles (0xB0, the +0xA8 links); at the camera manager's entry (~1 ms of camera work on the console before the particle task) also the code lines of the manager's hottest natives (emitter, step, list, geodisp, manager, blend: ~40 KB); again at the frame's end for the display walk's draws | nothing written: the same bits by construction. Pointers read while the game thread may write them only change which line is asked for; every address is checked against main RAM, the walk is capped (256 geodisps, 32 nodes, 48 particles a node), a newer request cuts a walk short |

Not done (and why): the clip and light-need passes precomputed on the S5 worker (0113B1D8 is 140 VU0 instructions with
clipBySphere / box-clip guest calls to port exactly; ~0.25 ms on the console); a memo of the parameter blend (already
host code, and riding it is mostly a straight copy); the emitter frame / step as plain C++ (the real lever for their
~2 ms: their PS2 shape, VU0 registers carried between them, the guest stack frames used by their callees; a DC-style
rewrite, not a round).

## 4. Design notes

- **Where the layers' outputs are read.** E +0x198 is read only by disp_particle_emitter (the draw) and written by the
  emitter frame (cleared, next frame) and 01140CE0; 0x0118CFF4 only by 011408E0 and the manager (the recompiled code,
  build/runner, grepped for both). P2 writes them at the same moment as the original (the manager's tail), so no join is
  involved.
- **The registers.** The original's layers leave v0 / v1 / a1-a3 / t0 / t1 as the last 011408E0 / isysGroupForExit left
  them; the manager then sets s4 / a0, restores s0-s5 and returns to the particle task (01345110), whose next instruction
  is `jal disp_particle_manager`: that function sets a0 in its first delay slot, calls iosUnitMatrix (a0 only) and
  getDrawScreenWidth, and reads no scratch register before writing it. The check counts which registers differ.
- **The helper and the cores.** One more thread on cores 1-2, asleep except for two short walks a frame. It competes with
  the render thread for nothing but L2 room (~100 KB of lines a walk, 2 MB L2).
- **Spalign / threads.** No aligned by-value structs (the check's context copies are static); the helper never reads a
  null g_ram (the rdram handed with the request is checked) and touches no GPU pool.

## 5. Exactness

In-run, every manager call, no tolerance. `SOTC_P2PTCL_CHECK=1` runs the original layers pass first (its writes and the
context it leaves kept), puts the emitters' E +0x198 and the budget count back (or, every `SOTC_P2PTCL_RAMCHECK` passes,
all 32 MB of main RAM from a copy taken before), runs the host pass from that state and compares: every emitter's
E +0x198, the count at 0x0118CFF4, and on the RAM-check passes every word of main RAM (the dead stack below sp counted
apart). The run then goes on with the original's state. The prefetch helper writes nothing: nothing to compare (by
construction; the runs below have it on).

| run (Vita3K 222, p1 = the shipped build) | where | frames | passes compared | emitters compared | differ | budget count differs | whole-RAM checks | words differing outside the dead stack |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| chkA | attract demo (the world, Agro's gallop), 400 s | 15 862 | 15 550 | 208 347 | **0** | **0** | 130 (32 MB each) | **0** (1 003 in the dead stack below sp) |
| chkR2 | ride7, 600 s, mounted at 6 037 | 16 789 | 16 147 | 916 387 | **0** | **0** | off | - |
| chkL2 | lake6, 400 s | 9 163 | 8 949 | 434 245 | **0** | **0** | off | - |
| chkR / chkL (first try, RAM checks on) | ride7 / lake6 | 2 323 / 1 989 | 1 750 / 1 750 | 17 013 / 15 299 | 0 / 0 | 0 / 0 | 15 / 15 | 0 / 0 (99 / 97 dead stack) |

chkR and chkL with the RAM check stopped early: the check's two 32 MB copies of main RAM ran the game's 176 MB arena out
of memory (`oom.txt`, then a fault) once the field and the lake were loaded; the attract demo had the room. They were run
again with `SOTC_P2PTCL_RAMCHECK=0` (node and count checks only: chkR2 / chkL2). Check mode only; the switch itself
allocates nothing of the kind.

**Registers** (counted by the check: registers the original layers leave that the host pass, which leaves the context as
it found it, does not): v0, v1, a2, a3, t0, t1 (the last 011408E0 / isysGroupForExit's scratch), and s0-s4, a0 and ra,
which the manager sets or restores right after the pass (s4 = -1, a0 = the group, s0-s5 / ra from its frame), so they do
not survive its return. The particle task's next call, disp_particle_manager, writes a0 in its first delay slot and reads
no scratch register before writing it (section 4). **Gameplay state**: the same writes in guest memory (130 whole-RAM
compares: 0 words outside the dead stack), so nothing the game reads differs.

## 6. Speed (Vita3K 222 MHz)

Interleaved A/B, `SOTC_P2PTCL=1 SOTC_AB_FLAG=p2ptcl SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (q.sh / abn1.sh, mixframes.py,
300-frame blocks; ride7 from the mount + 600). Off - on, +- 2 SE.

| run | frames from | frame | busy (game thread) | render thread |
|---|---|---:|---:|---:|
| mixR7 (ride7 seed 7, 800 s) | 6 625 | +0.256 +- 0.716 | +0.206 +- 0.716 | -0.280 +- 0.382 |
| mixR13 (ride7 seed 13, 800 s) | 5 554 | -0.387 +- 0.884 | -0.340 +- 0.893 | -0.064 +- 0.362 |
| mixL7 (lake6 seed 7, 400 s) | 3 000 | **+0.164 +- 0.109** | +0.151 +- 0.111 | +0.033 +- 0.150 |
| mixR7b (ride7 seed 7 again) | 6 729 | +0.085 +- 0.390 | +0.154 +- 0.382 | +0.126 +- 0.181 |
| mixR13b (ride7 seed 13 again) | 4 357 | +0.011 +- 0.410 | +0.115 +- 0.421 | +0.069 +- 0.171 |
| **pooled (mixR7b, mixR13b, mixL7; mean of 3, SE = sqrt(sum se^2) / 3)** | | **+0.087 +- 0.096 (1 SE), z 0.9** | **+0.140 +- 0.097, z 1.5** | +0.076 +- 0.048 |
| pooled (the first three: mixR7, mixR13, mixL7) | | +0.011 +- 0.190, z 0.1 | +0.006 +- 0.192 | -0.104 +- 0.091 |

The two ride7 pairs ran while another project's build (~10 cc1 processes outside the slot script) held the Mac at a
load average of 35-45: frame times 46-47 ms instead of ~33, per-block noise ~4x the lake run's. The ride runs were
repeated (mixR7b / mixR13b) when the load had dropped, and it rose again during them; the pool reported first uses the
repeats. Neither ride run resolves anything; the lake (run at load ~5) does, at z 3.

**The sampler** (the game thread's samples in the old layers code, decomp_hot47e::layers + 011408E0, against the new
p2layers code, per 1 000 frames of each state; tools/s4groups.py `--new p2layers --old 'decomp_hot47e::layers|manager_sub_011408E0'`):

| run | new / old | saved a frame (z) |
|---|---:|---:|
| mixL7 | 31.0% | 0.115 ms (z 52) |
| mixR7 / mixR7b | 31.0% / 31.0% | 0.158 / 0.096 ms (z 83 / 73) |
| mixR13 / mixR13b | 31.3% / 30.8% | 0.138 / 0.093 ms (z 77 / 72) |

That counts only the two functions' own code; the five passes' isysGroupForFirst / Exit, ptcl83::groupNext and the
callGuest dispatch around them go too. So in Vita3K P2 takes **~0.1-0.16 ms a frame** off the game thread, the size the
lake's frame A/B sees.

**The helper** (`[p2ptcl]`, on-frames): 28-120 geodisps a walk, 2 800-10 000 lines asked for, 0.10-0.23 ms of helper CPU
a walk, two walks a frame: **0.20-0.46 ms a frame of a core 1-2 thread** in Vita3K (no memory latency there; on the
console the walk itself misses, so expect 2-4x). Never cut short by a newer request. The host layers pass: 0.017-0.12 ms
on the game thread (28-120 geodisps). The S5 job is unchanged (0.06-0.12 ms, joined at the frame's end, not waited for).

## 7. Boot and pictures

bootR (p1, ride7 600 s, `SOTC_P2PTCL=1 SOTC_VITA_EAT_USER_MB=89`): `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, mounted at
frame 2 527, rode to frame 14 078 with no fault (`[p2ptcl]` 120 geodisps a pass at the end, fallbacks 0). Window shots
every 120 s, looked at: Wander riding Agro at the gallop across the plain under the cliffs (t0180), the shadows under
both, then through the ruins' colonnade in the fog (t0420, t0540): the fog / mist and the stone normal, nothing missing
or flickering. The hoof dust is small at this size; the check runs (chkA's gallop, chkR2's ride, chkL2's lake with the
shrine) were the frame-exact proof for the particles (their outputs are the original's, section 5). spalign on p1:
**0 sp-based** NEON accesses (of 1 038 with an alignment hint). No `%zu`, no aligned by-value structs (the check's
context copies are static), the helper never touches GPU pools and checks its rdram pointer.

## 8. The console A/B package

- VPK: `build/agent-scratch/p2/celosia-p2.vpk` (= p1.vpk; ELF `celosia-p2.elf`), P2 off unless set.
- Env: `build/agent-scratch/p2/env-ab.txt`: the P1 console env (frame log, PROF129, body shadows, muted logs) with
  `SOTC_P2PTCL=1 SOTC_AB_FLAG=p2ptcl SOTC_AB_PERIOD=300 SOTC_P2PTCL_LOG=600`: 300-frame windows alternate P2 on / off.
- What to look at: game-thread ms in on vs off windows riding; PROF129's module pcs (`build/agent-scratch/p2/modpcs.py`
  style: 0x113A4F8 / 0x113A53C / 0x113A574 / 0x11408E0 should vanish in on windows; the emitter frame / step / blend pcs
  shrink if the prefetch helps); `[p2ptcl]` the helper's ms a walk on the console (its own misses) and `cpu offload`.

## 9. What P2 did not reach, and why

The target was 2 ms off the console's game thread. What this round can claim:

- **Exact, measured**: the layers pass, ~0.25-0.3 ms of console time riding (its pcs), now ~30% of its old cost in
  Vita3K: **~0.2 ms on the console** expected from that part.
- **Exact by construction, unmeasurable here**: the prefetch helper. If the console's ~2.8x step-side excess is data
  misses that the helper turns into L2 hits, up to ~0.5-1 ms; if it is instruction-cache thrash inside the ~50 KB of
  native code run per geodisp (likely part of it), the data prefetch does little and the code prefetch only helps if L2
  lost that code between frames. **A realistic console estimate for P2 is 0.2-1 ms, not 2 ms.** Only the console A/B
  can say.
- **Threads**: the step itself was not moved (section 2): every exact split runs into the game's VU0 random stream, the
  heap and the guest-shaped code, and round 91/95's whole-step job lost 2.5-3 ms on the console.
- **What would reach 2 ms**: rewriting the per-geodisp chain (emitter frame, step, list, the leaves) as plain C++ over
  host emitter records, so a geodisp costs ~2-5 KB of code and a few lines instead of ~50 KB and ~50 lines; then the
  steady emitters (done generating, no spawn, no random) could be batched by phase or moved to a worker with no ordering
  issue. Next candidates in the same spirit and smaller: the clip / light-need passes precomputed on the S5 worker
  (0113B1D8 ported exactly), ptclDisplayListGet native.
