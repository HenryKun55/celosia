# The particle subsystem: a map for S1

Written in Decomp D8 (2026-11-15) for round S1, the semantic decomp and Vita redesign of particles and effects. It says
what the code is, where its data lives, who calls it, what it costs on the game thread, and which parts are already host
code. Every address is GAMECORE's (it loads at a fixed place); addresses below 0x1000000 inside GAMECORE code are
relocation placeholders (`lui 0x1e` / `0x1b` + offset), so the real target must be read from the live instruction words
(sotc_decomp::relocated).

Sources: the recompiled code's MIPS comments (build/runner), the hand natives' own headers (decomp_particles.cpp,
decomp_ptclupdate.cpp, decomp_ptcldraw.cpp, decomp_ptcl62.cpp, decomp_ptcl117.cpp, sotc_worker.cpp, decomp_hot46j.cpp),
D8's access-pattern pass (build/agent-scratch/d8/access.py: each function's argument offsets and globals, read in
linear order, a heuristic) and call graph (direct `jal`s only: calls through tables and function pointers are not seen).

## What "Particles and effects" is

tools/decomp_status.py puts a function in this area by its name (`ptcl|particle|effect|eff|splash|dust|smoke|fire|water|
rain|wave`) or by its file (decomp_particles / ptcldraw / ptclupdate). That gives 116 functions, but they are four
different things:

| part | address range | functions | what it does |
|---|---|---|---|
| **The particle module** | 0x1139CA0-0x1147530 | 177 | the particle manager, emitters, the per-particle step, parameter blending, LOD, textures, wind. Only 29 of them are named like particles, so most of the module is not in the area |
| **ptclman** | 0x1099E00-0x109A4D0 | 12 | builds the GS packets for particle draws (env, mat, vtx), plus the display list's get / put |
| **Effect users** | scattered | ~60 | the layout objects that emit particles (updateParticleGeo, kickParticle, particleOnOff, kickEffect, updateParticleDissolveGeo), the collision-attribute effects such as footstep dust and splashes (ExecEff, SequenceKickParticle, GetAttributeRelationEffectDefByName), the arrows' water rings (arrowWater*), the boss blood and weak-point glow (effect_blood_*, *_weakpoint_effect, effect*DamageWork), special effect objects |
| **Not particles** | scattered | ~25 | full-screen colour / fog effects (fsEffect*, effectSet: the screen's fade and fog parameters), water queries (GetWaterSurfacePositionY*, IsWaterSurfaceExistAround, charrootdyn_getWaterStatus, checkUnderWater, coliJudgeNotWater), the water refraction screen pass, and names that only contain "eff" (charrootdyn_getUpperEdgeEffectiveHeight, set_*_effective_ratio_*, GAMEUTIL_get_cross_position_coefficient_*) |

For S1, **the particle module and ptclman are the subsystem**. The effect users are its clients, and the rest belongs to
other subsystems: the screen pass, water and collision, and the character root dynamics.

## How a particle gets to the screen (one frame)

1. **Kick.** A client starts an emitter. A layout object does it through updateParticleGeo / kickParticle /
   particleOnOff (the object's emitter id is in its layout class). A collision contact does it through ExecEff →
   manager_sub_011688A8 → SequenceKickParticle → boot_particle_emitter. Script effects use boot_particle_emitter_extend.
   Each emitter is created from an effect record that is looked up by name (get_particle_effect_id_by_name, strcmp
   over the loaded sheet) or by sheet id.
2. **Step.** doit_particle_manager (0x113A340, once a frame) walks the isys group of particle geodisps
   (isysGroupForFirst/Next). For each geodisp, doit_particle_geodisp (0x113F500) runs the emitter list. Each emitter
   step is 0113B610 (the frame step chosen by the effect's name, the distance to the camera, the parameter blend
   get_particle_parameter_blend_doit), then its particles 0113B928 (the local matrix, the wind, each live particle's
   step particleUpdate = manager_sub_0113CA58, and new particles from the free list via 0113C1A8). Dead emitters are
   unlinked and freed (auto_end_check_and_check_free_particle_geodisp). Round 91 can run this step as a **job on a worker
   thread** beside the display walk (sotc_worker.cpp, `SOTC_PTCL91=1`). It is **off by default**, so the step runs on the
   game thread. The worker's `gate<I>` wrappers are still installed on every module entry, so the game thread would join
   before entering the module if a job were running.
3. **Clip.** In the display walk, is_emitter_need_disp / manager_sub_0113B1D8 test each emitter's bounds against the view
   (decomp_ptcl62.cpp).
4. **Draw.** disp_particle_geodisp (0x113F6A8) → 0113ACB8 (the display layer by LOD record kind, the draw-key runs) →
   manager_sub_0113E940, which writes 48 bytes per live particle into the display list (ptclDisplayListGet):
   position, size, clamped colour × scales, alpha × fade (01142030), and sin/cos of angle + π/4. Then ptclman's env /
   mat / vtx packets go out once per run (decomp_ptcldraw.cpp, decomp_ptcl117.cpp, decomp_hot46j.cpp).
   disp_particle_manager (0x113F220) builds the particles' screen matrix from the camera context after the job.

## Data (guessed from access patterns; offsets are bytes)

**Particle** (0xB0 bytes, from decomp_particles.cpp): +0x00 position (vec4), +0x10 velocity, +0x20 / +0x30 more
vectors (acceleration / spin axis), +0x40 life (0 = free), +0x44 age, +0x4C.. size, colour, rotation, +0xA4 "has hit",
+0xA8 next (a singly linked list; free particles on the emitter's free list).

**Emitter / particle system** (decomp_ptcldraw.cpp, decomp_ptclupdate.cpp, sotc_worker.cpp): +0x70.. colour scales,
+0x124 particle count, +0x128 first particle, +0x140 / +0x144 alpha / size scales (0 hides the emitter), +0x150 /
+0x154 floor and ceiling heights, +0x16C blend ratio (set_npe_blend_ratio_particle_emitter), +0x1B0 next emitter in
the geodisp's list.

**Emitter kind / effect record** (a3 of the particle step): +0x0 space (world / local / camera), +0xC shape, +0xC8
spin, +0x104 collision mode, +0x11C jitter. The record that the draw reads: +0x1C display list, +0xB4, +0xB8 brightness
factor. LOD record: +0xC kind (4 = layer 0x13), +0x34 draw key, +0xC0 / +0xC4 skip flags.

**Geodisp**: byte +8 == 0x24 marks a particle geodisp, and its class is at +0x20 (GetGeoDispClass). The class's
emitter list head is at +0xE8, and its view vectors are at +0x40 / +0x80.

**Globals** (main RAM):

| address | what |
|---|---|
| 0x118CFD0-0x118D054, 0x118D228-0x118D238 | manager configuration and counters (init_particle_manager writes them). *0x118CFE8 is the frame step: +0 dt, +4 a second step, +0xC VBlanks this frame |
| 0x1216DF0-0x1216EF4 | the manager's state: isys groups (geodisps, emitters, segments), lists, flags. *0x1216ED0 is the draw skip / layer switch |
| 0x1215BE8-0x1215C00 | ptclman's packet state: +0 the packet's base, +4 qwords used (see below); +0x8 (0x1215BF0) written by ptclmanBuildEnvPacket, read by the vtx packet |
| 0x118BFF0 / 0x118BFF4 | ptclman's frame matrices (ptclmanFrameInit: the camera context copied) |
| 0x1214750-0x1214790 | the full-screen effect manager (fsEffect*) |
| 0x11862F0 / 0x11862F4 | the current full-screen effect parameters (effectSet, fsEffectSetParam) |
| 0x11E6220-0x11E6290, 0x1466510-0x1466528, 0x14666F0 | boss blood / damage work / weak-point state (effect_blood_*, effectSetDamageWork, *_weakpoint_effect) |
| 0x11E6650-0x11E6665 | arrows' water-ring work (arrowWater*, EnableArrowWater) |
| 0x118C034-0x118C058 | in-water screen effect (inWaterEffectInit, getIsInWater) |
| 0x11920B0-0x11920D8 | water refraction pass parameters |
| 0x122B860-0x122B880, 0x118FE88-0x118FE98 | ExecEff's last kick (position / attribute) |

## Facts from RafaelScharf (PRs #4–#6)

PRs #4, #5 and #6 (closed, not merged) carried native ports of ptclman, the fog factors, the geodisp light / display
passes, the emitter display and the module's control / lookup entries. Their code was not taken; the facts below were,
each one checked against the recompiled code (build/runner, the MIPS comments) and our own natives before writing it.
ptcl_types.h has the field names (`ptcl::sheet`, `effect::kSize` / `kLightMode` / `kCameraFollow` / `kMatrix`,
`manager::kPtclman*`).

**Fog state, 0x12147D8** (0x28 bytes; setFogImmediate 0x10896F0 copies it from the target at 0x12147B0,
setFogParamDefault 0x1089AA0 from the defaults at 0x11862F8). With B = +0x4, A = +0x8, C = +0xC, D = +0x10, E = +0x14
(native_renderer_map.md: near / far distances at +0x4 / +0x8, their rates at +0xC / +0x10):

    getFogFactor0 (0x1089B70) = A·B·(C − D) / (A − B)
    getFogFactor1 (0x1089BB0) = ((D − C)·(A + B) / (A − B) + (D + C)) · E

glSetFogFactor (0x108F530) evaluates F = clamp(F1 + F0 / w, 0, 255) for one point (w from the camera matrix, at least
1), the same line the microcode applies per vertex. With E = 0.5 the line goes exactly through (near, C) and (far, D);
native_renderer_map.md's "half of getFogFactor1's base" is the same remark.

**ptclman's packet state, 0x1215BE8.** +0 the packet's base (ptclmanBegin 0x109A1F8: the packet allocator's current
pointer, a relocated `jal`), +4 the qwords used (Begin zeroes it). ptclmanGet (0x109A3D8) = base + used × 16,
ptclmanPut (0x109A3F8) adds a0 qwords. ptclmanEnd (0x109A228) appends the qword 0x60000000, 0, 0, 0x13000000 (a DMA
`ret` tag and VIF FLUSHA), then passes the new end to one packet call and the base to another (both relocated: what they
do is not checked here). ptclDisplayListGet (0x109A1A8, a0 = particles) returns the same allocator's current pointer
(the call ptclmanBegin makes), or 0 when its limit leaves fewer than a0 × 48 bytes.

**The light-need test.** 01140E60 (a0 the emitter E, a1 the effect set Mr, a2 Mr's LOD record 0 — Mr +0xD4 itself, not
indexed by E +0x118): 0 when E +0x110 is set (the light was done this frame by 0113B408), else 1 when Mr +0x64 != 0
(the light colour's source mode, the same word 0113B408 switches on), Mr +0xB0 == 1 (view-facing) or the record's
+0x164 == 2 (aim at the camera); else 0. 01140C80 walks the geodisp's nodes and stops at the first 1;
calc_light_particle_geodisp (0x113F600) then copies the first node's emitter position (node +0x20 = E +0x10) into the
class's +0x10 with w = 1, clears bit 0 of the class's 64-bit +0xC0 and tail-calls SetGeoDispClassLightColMan.

**The depth sort.** In 0113ACB8 the nodes go in runs of equal draw key (sheet +0x34). A run of two or more is copied
as node pointers into a 16-aligned array on the stack and sorted with qsort (0x116A310, 4-byte elements, compare
0x1140EA8). The compare reads node +0x144 (E +0x134, the depth along the camera's axis) and returns 1 when a's depth
is less than b's, else −1. It never returns 0, so the larger depth sorts first: each run is drawn **back to front**.
Runs themselves stay in list order; nothing is sorted across draw keys.

**Sheet records.** An effect id is the address of a 0x54-byte sheet record (get_particle_effect_id_from_sheet returns
its argument). get_particle_effect_id_by_name (0x113F8F8; a0 a resource handle whose word 0 & 0x3FFFF is the distance
back to its owner, a1 the name) walks owner +0x10 for owner +0xC records at stride 0x54 and compares the string that
+0x24 points to. An owner of 0, or equal to the "no owner" word that the code loads with `lui 0x1e` + 0xED4 (a
relocation placeholder, so the real address must be read live), gives 0. get_particle_name_from_effect_id returns
+0x24 (0 for a null id), get_main_particle_effect_lod_from_sheet +0x48, get_top_particle_effect_from_lod Mr +0xD4,
get_particle_emitter_from_emitter_id id + 0x10.

**The effect set.** create_particle_effect_lod (0x113FBF0) allocates **0x140 bytes** (a1 = 0x1195840), zeroes +0xD0..+0xDC and makes +0xE0 a unit matrix. get_particle_camera_follow_attr_from_effect_id (0x113FA40)
returns Mr **+0xBC**, the camera-follow attribute (−1 for a null id), which updateParticleGeo reads.
is_loop_particle (0x113F700) is 1 when LOD record 0's +0x10 is 1.

Not taken: the PRs' natives themselves. Some read relocation placeholders literally (`0x001E0ED4` as the no-owner
word, `0x001B91A0` as a call fallback, both from `lui 0x1e` / `0x1b`), and much of the path they cover is already host
code here (decomp_hot46j, decomp_ptcl117, decomp_lift*); the rest (calc_light_particle_geodisp and the entries that
only have sotc_worker.cpp's gate) is left to S1's redesign rather than a second exact port. The PRs' function names (ptclEmitterNeedLight, ptclEmitterSortCmp, ptclLightListPass,
ptclDispEmitterList, ptclDispEmitterNode, ptclEmitterLightDir, ptclEmitterMatrixMode, ptclWorldMatrixSR) are theirs,
not symbols; the map keeps the manager_sub_ names.

## Host code that already touches particles

The hand natives were written in performance rounds. They are bit-exact ports of the recompiled code's operations and
keep the guest data layout.

| file | what (switch) | functions |
|---|---|---|
| decomp_particles.cpp | the per-particle step, particleUpdate (rounds 8 / 123: ptcl123 runs it in plain floats) | manager_sub_0113CA58 and the helpers only it calls (0113D280, 01141918, 01141968, 01141B70, inline), manager_sub_0113C8C8 (the damage test) |
| decomp_ptclupdate.cpp | the emitter step from its top (round 8) | doit_particle_geodisp 0113F500, 0113AAE0, 0113B610, 0113B928, CheckCollisionFinderAlive |
| decomp_ptcldraw.cpp | one system's draw, 48 bytes per particle (round 46+) | manager_sub_0113E940 (01142030, the fade, inline) |
| decomp_ptcl62.cpp | the clip pass (SOTC_PTCL62) | GetGeoDispClass, get_top_emitter_list_from_particle_geodisp, is_emitter_need_disp, 01140C10 |
| decomp_ptcl117.cpp | the draw chain from the display walk (SOTC_DRAW117) | disp_particle_geodisp 0113F6A8, 0113ACB8, 01140ED0 |
| decomp_hot46j.cpp | ptclman's packets and the layout objects' particle update | ptclmanBuild{Mat,Vtx,Env}Packet, ptclmanBegin / End, updateParticleGeo, kickParticle, particleOnOff, KickParticleGetResourceId |
| decomp_hot47e.cpp | the parameter blend through host pointers (blend123) | get_particle_parameter_blend_doit's body |
| decomp_hot48c.cpp | the wind (round 48) | get_wind_effect_data and the wind vector (01145F78 / 01146DA8 / 01146A50) |
| sotc_worker.cpp | round 91: the module's step as a job on a second thread (`SOTC_PTCL91=1`, **default off**); `gate<I>` on every module entry (installed by default), disp_particle_manager and set_npe_blend_ratio_particle_emitter answered after a job | all module entries (the gate only: it calls whatever the slot held at install, so a gated entry is not a decompiled one) |
| decomp_lift*.cpp (D2-D8) | lifted copies (exact, mechanical) | the rest of the module's reached entries and the effect users (see the table) |

What this means for S1: the hot path (step, blend, draw, clip) is already host code with the guest layout. The worker
can run the step off the game thread, but that job is off by default (round 91 left it to the console's A/B). The work
left is a **redesign**, not more porting. One option is host-side
particle pools (SoA, no 0xB0-byte guest records), the step on the worker with no joins, and a direct GXM draw instead of
building GS packets for the native renderer to translate. The guest-facing surface it must keep is small: the kick /
stop / end / blend-ratio entry points that clients call (boot_particle_emitter*, stop_particle_emitter_*,
end_particle_emitter, set_npe_blend_ratio_particle_emitter, set_generate_particle_flag, get_particle_effect_id_*),
doit_particle_manager once a frame, disp_particle_geodisp from the display walk, and the effect records loaded from the
sheets (the parameter tables that get_particle_parameter_blend_doit reads).

## Cost on the game thread

Sampler (Vita3K 222 MHz, attract demo). Columns: the final build p3 (default config: the gates installed, the job off), runs/p3_att: snapshots 300-690 s; game thread 49133807307 samples, other threads 50435257452; and mixa (p1 with SOTC_WORKER91=0), runs/mixa: snapshots 120-1440 s; game thread 174487889012 samples, other threads 189048999834. Shares are of the game thread's samples; the "other threads" column (worker on) is also scaled to the game thread's total, so it reads as "this much extra work elsewhere".

Totals over the rows below: mixa **8.08%** of the game thread; p3 final **5.06%** on the game thread + **0.00%** on other threads.

| code | mixa (p1, gates off): game thread | p3 final: game thread | p3 final: other threads |
|---|---:|---:|---:|
| [decomp_ptclupdate] | 2.202% | 1.267% | 0.000% |
| [decomp_ptcl117] | 1.200% | 0.693% | 0.000% |
| [decomp_hot46j] | 1.068% | 0.617% | 0.000% |
| [decomp_particles] | 0.831% | 0.446% | 0.000% |
| [decomp_ptcl62] | 0.415% | 0.279% | 0.000% |
| [decomp_ptcldraw] | 0.338% | 0.198% | 0.000% |
| manager_sub_01141EA8 | 0.294% | 0.140% | 0.000% |
| manager_sub_0113B1D8 | 0.214% | 0.131% | 0.000% |
| manager_sub_0113C1A8 | 0.193% | 0.149% | 0.000% |
| arrowWaterMain | 0.141% | 0.135% | 0.000% |
| ptclDisplayListGet | 0.166% | 0.096% | 0.000% |
| manager_sub_01146FB8 | 0.045% | 0.202% | 0.000% |
| manager_sub_01140C80 | 0.152% | 0.088% | 0.000% |
| GetWaterSurfacePositionYExtend | 0.091% | 0.096% | 0.000% |
| waterRefractionBegin | 0.072% | 0.074% | 0.000% |
| manager_sub_01146EB8 | 0.083% | 0.047% | 0.000% |
| doit_wind_manager | 0.057% | 0.045% | 0.000% |
| manager_sub_01146DA8 | 0.057% | 0.039% | 0.000% |
| manager_sub_011408E0 | 0.055% | 0.031% | 0.000% |
| IsWaterSurfaceExistAround | 0.030% | 0.033% | 0.000% |
| get_world_matrixSR | 0.031% | 0.018% | 0.000% |
| manager_sub_011416E0 | 0.028% | 0.020% | 0.000% |
| manager_sub_0113A5A8 | 0.019% | 0.019% | 0.000% |
| manager_sub_01147150 | 0.019% | 0.018% | 0.000% |
| disp_particle_manager | 0.019% | 0.018% | 0.000% |
| manager_sub_01145D28 | 0.007% | 0.027% | 0.000% |
| manager_sub_01142198 | 0.029% | 0.005% | 0.000% |
| manager_sub_01140E60 | 0.021% | 0.012% | 0.000% |
| manager_sub_01141D20 | 0.032% | 0.000% | 0.000% |
| updateParticleGeo | 0.022% | 0.006% | 0.000% |
| manager_sub_0113CA58 | 0.013% | 0.009% | 0.000% |
| graphFsEffectReset | 0.011% | 0.011% | 0.000% |
| ptclmanFrameInit | 0.010% | 0.010% | 0.000% |
| charrootdyn_getWaterStatus | 0.007% | 0.009% | 0.000% |
| waterRefractionEnd | 0.008% | 0.007% | 0.000% |
| effectSet | 0.006% | 0.008% | 0.000% |
| manager_sub_0113AEF8 | 0.007% | 0.006% | 0.000% |
| manager_sub_01141E80 | 0.007% | 0.006% | 0.000% |
| manager_sub_01140CE0 | 0.007% | 0.006% | 0.000% |
| updateSpecialEffectManager | 0.006% | 0.006% | 0.000% |

Read the table this way. The bracketed rows are the hand natives, summed by file namespace. The decomp_hot46j namespace
also holds three sibling particle draws and the layout objects' particle update. The named rows are guest functions:
lifted copies and recompiled code together. The particle code takes **about 5-10% of the game thread in the attract
demo**, which is full of particles. The share follows what the window shows: the p2 build's run of the same window
(28 900 frames) came to 9.9%, and the p3 run (19 200 frames, slower with two emulators running) to 5.1%. Almost all of it is already host code: the step (decomp_ptclupdate +
decomp_particles), the draw chain (decomp_ptcl117 + decomp_hot46j + decomp_ptcldraw) and the clip (decomp_ptcl62). The
biggest guest pieces are manager_sub_01146FB8 (a wind volume's rate, GetRateInOutCubeVolumeWithInverseMatrix), 01141EA8,
0113C1A8 (a new particle), 0113B1D8 (an emitter's bounds against the view), ptclDisplayListGet and 01140C80. The water
ones (arrowWaterMain, GetWaterSurfacePositionYExtend, waterRefractionBegin) are next. The two columns come from different
builds and windows (p3, 300-690 s; p1's mixa, 120-1440 s, half its frames with D8's batch off), so compare their
shapes, not single rows. With round 91's job off by default, nothing particle-related runs on another thread.

## The area's leftovers after D8

D8 counted the area's leftovers twice. First it used decomp_status's own count: 36 recompiled functions plus the
gate-only ones that gated.py finds by address. The second, definitive count is the replaced dump with the worker's gates
off (`SOTC_WORKER91=0`): anything replaced only with the gates on is not decompiled. On that count the area stood at
**about 58 of 116 before D8 and 91 of 116 after** (88.9% of its code by size). decomp_status's gate-counting figure goes
from 80 to 98. The table lists the 57 functions D8 worked on: 33 shipped, 1 was reached but never at top level in a
trace pass (get_particle_effect_id_from_sheet), and 24 were not reached, each with what would reach it.

| function | before D8 | first reached on | D8 |
|---|---|---|---|
| fsEffectCreateSetScriptSheetSeg | recompiled | - (a colossus' death (effect_death, its only caller)) | not reached |
| fsEffectCreateSetScriptWithPlayMode | recompiled | new game (opening) | native (D8) |
| fsEffectReset | recompiled | - (no caller in the recompiled code (no jal, no address constant): a pointer table's entry or dead code) | not reached |
| getCurrentFsEffect | recompiled | - (no caller in the recompiled code (no jal, no address constant): a pointer table's entry or dead code) | not reached |
| charrootdyn_getUpperEdgeEffectiveHeight | recompiled | - (a ledge: edgexz / edgepoint / xzfit at a climbable edge (hanging from a wall's top)) | not reached |
| charrootdyn_checkUnderWater | recompiled | - (Wander under the water surface (a dive; pendulum / slide judge under water): swimming on the surface (tb4w / tb4y) did not) | not reached |
| charrootdyn_getWaterStatus | recompiled | new game (opening) | native (D8) |
| updateParticleDissolveGeo | recompiled | new game (opening) | native (D8) |
| kickEffect | recompiled | new game + input | native (D8) |
| CollisionPlaneJudgeCallbackParticle | recompiled | - (a callback of ClipCollisionDirectForParticle / radiate_particle_collision_check_ray (particles that collide with planes), neither called on any route) | not reached |
| GetWaterSurfacePositionYExtend | recompiled | new game (opening) | native (D8) |
| add_particle_segment | gate only | new game (opening) | native (D8) |
| init_particle_manager | gate only | walk (D7) | native (D8) |
| manager_sub_0113C8C8 | gate only | walk (D7) | native (D8) |
| free_all_emitter_and_particle_particle_manager | gate only | walk (D7) | native (D8) |
| is_loop_particle | gate only | walk (D7) | native (D8) |
| boot_particle_emitter | gate only | walk (D7) | native (D8) |
| boot_particle_emitter_extend | gate only | new game (opening) | native (D8) |
| set_generate_particle_flag | gate only | walk (D7) | native (D8) |
| set_npe_blend_ratio_particle_emitter | gate only | ride (D7) | native (D8) |
| get_particle_effect_id_by_name | gate only | walk (D7) | native (D8) |
| get_particle_effect_id_from_sheet | gate only | ride (D7) | reached, not checked at top level |
| get_particle_name_from_effect_id | gate only | - (no caller in the recompiled code (a debug helper)) | not reached |
| get_particle_emitter_from_emitter_id | gate only | walk (D7) | native (D8) |
| get_particle_camera_data | gate only | - (no caller in the recompiled code) | not reached |
| recreate_particle_effects | gate only | new game (opening) | native (D8) |
| particleGWorkPut | gate only | - (no caller in the recompiled code) | not reached |
| get_radiate_range_coefficient | gate only | new game (opening) | native (D8) |
| radiate_particle_collision_check_ray | gate only | - (manager_sub_01141400 (a particle's radiate collision mode), never called on any route) | not reached |
| free_particle_segment | gate only | new game (opening) | native (D8) |
| get_wind_effect_data | gate only | walk (D7) | native (D8) |
| constructor_wind_effect_data | gate only | - (no caller in the recompiled code) | not reached |
| get_wind_effect_data_direct | gate only | - (no caller in the recompiled code) | not reached |
| NICO_is_hit_except_underwater | recompiled | - (no caller in the recompiled code (no jal, no address constant)) | not reached |
| effectSetDamageWork | recompiled | - (a hit on a colossus: SCRExecBossPreProcess, GetDamageClass kind 3 on a surface with attribute bit 0x8 and a part whose HP drops (a sword stab on a weak point, or an arrow where it hurts)) | not reached |
| effect_blood_init | recompiled | teleport to Avion | native (D8) |
| effect_blood_free | recompiled | Avion, then back to the shrine | native (D8) |
| effect_blood_frame | recompiled | teleport to Avion | native (D8) |
| effect_blood_set | recompiled | - (the same hit (blood from the damaged part)) | not reached |
| getEffectBloodWorkSize | recompiled | teleport to Avion | native (D8) |
| getWeakPointEffectIntensity | recompiled | teleport to Avion | native (D8) |
| getTriggerPointEffectIntensity | recompiled | - (SCRExecBossPreProcess on a colossus with trigger points lit (a fight in progress)) | not reached |
| boot_weakpoint_effect | recompiled | - (the sword's light on a colossus' weak point (nico_swordstart -> 013FC148 / 013FC7E0: R1 held in sunlight, the colossus in the beam)) | not reached |
| boot_weakpoint_hit_effect | recompiled | - (the same, the light's hit on the weak point (013FC148)) | not reached |
| init_weakpoint_effect | recompiled | teleport to Avion | native (D8) |
| fadeout_weakpoint_effect | recompiled | new game (opening) | native (D8) |
| cancel_fadeout_weakpoint_effect | recompiled | new game + input | native (D8) |
| end_weakpoint_effect | recompiled | Avion, then back to the shrine | native (D8) |
| effect_death | recompiled | - (a colossus' death (st_boss_dead, or end_weakpoint_effect when it dies)) | not reached |
| effectClearDamageWork | recompiled | teleport to Avion | native (D8) |
| effectGetDamageWork | recompiled | - (a colossus' death: CreateDeathTentacle (updateScriptDataSystem)) | not reached |
| getBloodEffectEnableFlag | recompiled | - (no caller in the recompiled code (no jal, no address constant)) | not reached |
| EnableArrowWater | recompiled | Avion, bow / swim | native (D8) |
| EndArrowWater | recompiled | Avion, bow / swim | native (D8) |
| inInNowLoadingEffect | recompiled | new game (opening) | native (D8) |
| startGaugeMaxAddEffect | recompiled | - (addPlayerMaxGrip: eating a fruit or a white-tailed lizard's tail (gamecore_sub_013B2BD0), a shrine's gauge reward (0134A1D8), or a colossus' death (st_boss_dead)) | not reached |
| startAttackEffect | recompiled | - (updatePowerWork at a full charge (getAttackPowerRatio): SQUARE held on a colossus (holding it on the ground in ng6 did not)) | not reached |
| coliJudgeNotWater | recompiled | - (a callback of the worm controller (updateWormCtrlWork, 01463C40 / 01463D10: a serpent colossus' script), none of which runs on these routes) | not reached |

## Every function of the map

The particle module (0x1139CA0-0x1147530), ptclman and the decomp_status area. "now" is the status after D8 by
the source file that names the address in code. "gate" means sotc_worker.cpp's wrapper is the only replacement: the
recompiled body still runs. Callers are direct `jal`s only. "touches" lists the argument registers' offsets
(R / W and the access size) and the globals (& is an address taken, R / W a load or a store), read in linear order.

| address | function | now | callers (direct) | touches |
|---|---|---|---|---|
| 0107eec8 | GAMEUTIL_get_cross_position_coefficient_t_by_dist_factor | native: decomp_liftd2_00.cpp | CAM_doit_subjective_target_visibility_and_update_cam_data, gamecore_sub_01415388 | $a0: +0x0:R4/W4; $a1: +0x0:W4 |
| 010888a8 | effectSet | native: decomp_liftd3_00.cpp | graphFsEffectReset | globals: 011862f0:&R 011862f4:R |
| 01088a38 | fsEffectSetParam | native: decomp_liftd4_00.cpp | ExecFogColMan, gamecore_sub_01387BC8, manager_sub_010EBDB0, manager_sub_01162618 | $a0: +0x4:R4; globals: 011862f0:&RW |
| 01088a78 | fsEffectSetParamSheetSeg | native: decomp_liftd2_00.cpp | gamecore_sub_0142C7F0 | $a0: +0x0:R4; globals: 001e0ed4:R 011862f0:&RW |
| 01088b20 | effectReset | native: decomp_hot48b.cpp | graphFrameInit | globals: 011862f0:&W 011862f4:W |
| 01088b48 | fsEffectCreateSetScript | native: decomp_liftd3_00.cpp | gamecore_sub_014098A0, gamecore_sub_0140AAF0 | globals: 001e0c88:& 01089288:& 010892e8:& |
| 01088bc0 | fsEffectCreateSetScriptSheetSeg | recompiled | effect_death | $a2: +0x0:R4; globals: 001e0c88:& 001e0ed4:R 01089288:& 010892e8:& |
| 01088cb0 | fsEffectCreateSetScriptWithPlayMode | native: decomp_liftd6_01.cpp | manager_sub_01105938 | globals: 001e0c88:& 01089288:& 01089340:& |
| 01088d60 | fsEffectManagerSetDemoParam | native: decomp_liftd4_00.cpp | gamecore_sub_0134A8B8, gamecore_sub_0134AAE0 | globals: 001e0ed4:R 011862f0:&RW 011862f4:W |
| 01088e38 | fsEffectReset | recompiled | (table / pointer only) |  |
| 01088e58 | fsEffectDefault | native: decomp_liftd2_00.cpp | ExecFogColMan, effectReset | globals: 001b7b60:& 001e0ed4:R 011862f0:&RW 01214750:& 01214754:R |
| 01088eb8 | fsEffectManagerInit | native: decomp_liftd2_00.cpp | setDefaultFsEffect | globals: 001b7b60:R 01214750:&W 01214757:W 01214758:W 0121475f:W 01214760:W 01214767:W 01214768:W 0121476f:W 01214770:&W 01214778:W 01214780:W 01214788:W 01214790:& |
| 01088f80 | initFsEffect | native: decomp_liftd7_01.cpp | (table / pointer only) | $a0: +0x10:R4; globals: 001b7b60:R 01214750:&W 01214757:W 01214758:W 0121475f:W 01214760:W 01214767:W 01214768:W 0121476f:W 01214770:&W 01214778:W 01214780:W 01214788:W 01214790:& |
| 01089118 | deleteFsEffect | native: sotc_prof.cpp | (table / pointer only) |  |
| 01089278 | getCurrentFsEffect | recompiled | (table / pointer only) | globals: 011862f0:R |
| 0108fcc8 | graphFsEffectReset | native: decomp_walk.cpp | DispDispObjGroupModel | globals: 0118b748:R 0118b74c:R 0118b8c0:&R 0118b8c8:R |
| 01099e00 | ptclmanBuildMatPacket | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 | globals: 01215be8:&R 01215bec:RW |
| 01099f38 | ptclmanBuildVtxPacket | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 | globals: 01215be8:&R 01215bec:RW 01215bf0:R |
| 0109a170 | ptclDisplayListPut | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 |  |
| 0109a1a8 | ptclDisplayListGet | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 |  |
| 0109a1f8 | ptclmanBegin | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 | globals: 01215be8:&W 01215bec:W |
| 0109a228 | ptclmanEnd | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 | globals: 01215be8:R |
| 0109a280 | ptclmanBuildEnvPacket | native: decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E6C8, manager_sub_0 | $a0: +0x0:R16; $a3: +0x0:R16; globals: 0118bfe0:&R 0118bff4:R 01215be8:&R 01215bec:RW 01215bf0:W |
| 0109a3d8 | ptclmanGet | native: decomp_liftd7_02.cpp | ptclmanEnd | globals: 01215be8:&R 01215bec:R |
| 0109a3f8 | ptclmanPut | native: decomp_liftd7_02.cpp | ptclmanEnd | globals: 01215be8:& 01215bec:RW |
| 0109a410 | ptclmanFrameInit | native: decomp_liftd3_01.cpp | gamecore_sub_01345110 | globals: 0118bff0:&RW 0118bff4:&RW 01215c00:& |
| 0109a4c8 | ptclCounterInit | native: decomp_liftd6_01.cpp | gamecore_sub_01345F60 |  |
| 0109a4d0 | ptclCounterGet | native: decomp_liftd4_00.cpp | gamecore_sub_01345F60 |  |
| 0109c108 | waterRefractionBegin | native: decomp_liftd3_00.cpp | graphFsEffectReset | globals: 011920b0:&R 011920b8:R 011920c0:&R 011920c8:R 011920d0:&R 011920d8:R |
| 0109cd28 | inWaterEffectInit | native: decomp_liftd6_00.cpp | graphFrameBufferReInitialize, graphInit | globals: 001e0c88:& 0118c038:&R 0118c03c:&R 0118c040:R 0118c044:R 0118c058:&RW |
| 0109d078 | waterRefractionEnd | native: decomp_liftd3_00.cpp | graphFrameFinish |  |
| 0109d0d8 | getIsInWater | native: decomp_liftd4_00.cpp | manager_sub_010881D0 | globals: 0118c034:& |
| 010dc3f0 | charrootdyn_getUpperEdgeEffectiveHeight | recompiled | manager_sub_010E35B8, manager_sub_010E9A50, manager_sub_010EA718 | $a0: +0x98:W4 |
| 010dc8c8 | charrootdyn_checkUnderWater | recompiled | manager_sub_01167F88, pendulum | $a0: +0x0:W16; $a1: +0x0:R16; globals: 010dc808:& |
| 010dc990 | charrootdyn_getWaterStatus | native: decomp_liftd8_00.cpp | edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, parentpointxzfit, pendulum, swim, xz | $a0: +0x362:R1/W1 +0x370:R2 +0xe38:R4 +0x10c4:W4 +0x10c8:W4 +0x10cc:W4; $a1: +0x0:R16/W16 +0x4:R4; globals: 010dc808:& |
| 010df448 | charrootdyn_getEdgeEffectivityCheckRayPosition | native: decomp_liftd7_03.cpp | gamecore_sub_013B8438 | $a0: +0x0:R16/W16; $a1: +0x0:R16/W16; $a2: +0x0:R16/W16; $a3: +0x0:R16 |
| 01101500 | initParticleObject | native: decomp_liftd6_00.cpp | create_and_boot_particle_geodisp | globals: 001e0c88:& 01194a08:&R 01194a0f:R 01194a10:R 01194a17:R 01194a18:R 01194a1f:R |
| 01102438 | GetScriptClass | native: decomp_liftd7_08.cpp | SCRFuncResetAcceleParamWormCtrlParam, SetScriptObjDeleteCallBackFunc, SetScriptObjDeleteCallBackFunc2, SetScriptObjFinishCallBackFunc, flushScriptObj, flushScri | $a0: +0x8:R1 |
| 01104368 | updateParticleGeo | native: decomp_hot46j.cpp | manager_sub_010FA830, manager_sub_01108960, manager_sub_0110BFB8, updateParticleDissolveGeo | globals: 001e0f40:&R 001e0fa0:& 001e0fd0:&R 001e0fe0:& |
| 0110b210 | updateParticleDissolveGeo | native: decomp_liftd8_00.cpp | manager_sub_010FA830, manager_sub_0110C050 |  |
| 0110b500 | kickEffect | native: decomp_hot46j.cpp | updateSEGeo |  |
| 0110b518 | kickParticle | native: decomp_hot46j.cpp | manager_sub_0110C420 |  |
| 0110b530 | particleOnOff | native: decomp_hot46j.cpp | (table / pointer only) |  |
| 01123520 | CollisionPlaneJudgeCallbackParticle | recompiled | (table / pointer only) | $a0: +0x0:R4 +0x4:R4 |
| 01127670 | GetWaterSurfacePositionY | native: decomp_liftd6_01.cpp | gamecore_sub_013792B8, gamecore_sub_0138E3C8, gamecore_sub_01405488, manager_sub_01140F50, player_collision_check_direct_water_surface_plus |  |
| 01127690 | GetWaterSurfacePositionYExtend | native: decomp_liftd6_01.cpp | GetWaterSurfacePositionY, manager_sub_0113A5A8 | $a1: +0x0:R16; globals: 011293e0:& |
| 011278f0 | IsWaterSurfaceExistAround | native: decomp_probe84.cpp | manager_sub_0113A5A8 | $a0: +0x0:W4; $a1: +0x0:R16; globals: 01118340:& 01195514:R |
| 0112a450 | CheckCollisionFinderAlive | native: decomp_ptclupdate.cpp | manager_sub_0113A7B8, manager_sub_0113A918, manager_sub_0113AAE0, manager_sub_01141690 | $a0: +0xc:R4 +0x10:R4 |
| 01139ca0 | create_particle_texture_and_get_id | native: decomp_walk.cpp | add_particle_segment | $a3: +0x0:W4 +0x4:W4 +0x8:W4; globals: 001e0ed4:R 0118d048:& 0118d050:&R 0118d05c:&R |
| 01139dc8 | add_particle_segment | native: sotc_worker.cpp | (table / pointer only) | $a0: +0x0:R4 +0xc:R4 +0x10:R4; globals: 001e0ed4:R |
| 01139f10 | manager_sub_01139F10 | native: sotc_worker.cpp | free_particle_segment | globals: 0118cfd0:& 0118d048:& 0118d228:& 01216e00:& |
| 0113a1e8 | init_particle_manager | native: sotc_worker.cpp | initmanager | globals: 0118cfd0:& 0118cfdc:&RW 0118d048:&W 0118d04c:W 0118d050:W 0118d054:& 0118d228:&W 0118d22c:W 0118d230:W 0118d234:W 0118d238:& 01195850:& 01216df0:&W 01216df4:W 01216df8:W 01216e08:&W 01216e0c:W 01216e10:W 01216e1 |
| 0113a340 | doit_particle_manager | native: sotc_worker.cpp | gamecore_sub_01345110 | globals: 0118cfe0:W 0118cfe4:W 0118cfec:W 0118cff4:W 01216e00:& 01216e08:&W 01216e0c:W 01216e10:RW 01216e14:W 01216e18:&W 01216e1c:W 01216e20:R 01216e24:W 01216e28:&W 01216e2c:W 01216e30:RW 01216e34:W |
| 0113a5a8 | manager_sub_0113A5A8 | native: sotc_worker.cpp | doit_particle_manager | globals: 001e0fb0:& 001e0fd0:& 0118cfe0:R 01216e40:& 01216e70:W 01216e74:W 01216e78:W 01216e80:& 01216ec0:RW 01216ec4:&RW 01216ec8:RW 01216ecc:W 01216ed0:W |
| 0113a7b8 | manager_sub_0113A7B8 | gate (recompiled body) | check_free_particle_geodisp | $a0: +0x0:R4/W4; globals: 01216df0:&RW 01216df4:W 01216df8:RW |
| 0113a918 | manager_sub_0113A918 | native: sotc_worker.cpp | manager_sub_01139F10 | $a0: +0x0:R4/W4; globals: 01216df0:&RW 01216df4:W 01216df8:RW |
| 0113aae0 | manager_sub_0113AAE0 | native: sotc_worker.cpp | doit_particle_geodisp | $a0: +0x0:R4/W4; globals: 01216df0:&RW 01216df4:W 01216df8:RW |
| 0113acb8 | manager_sub_0113ACB8 | native: sotc_worker.cpp | disp_particle_geodisp | $a0: +0x0:R4 +0x128:R4 +0x1b0:R4; globals: 01140ea8:& 01216ed0:R |
| 0113aef8 | manager_sub_0113AEF8 | native: sotc_worker.cpp | manager_sub_0113B610, manager_sub_01140CE0 | $a0: +0x10:R16 +0x20:R16 +0x30:R16 +0x40:W16 +0x50:W16 +0x60:W16 +0xc0:R4 +0xc4:R4; globals: 001e0fb0:& 001e0fc0:& |
| 0113b1d8 | manager_sub_0113B1D8 | native: sotc_worker.cpp | is_emitter_need_disp | $a1: +0x0:R4 +0x4:R4 +0x20:R4 |
| 0113b408 | manager_sub_0113B408 | gate (recompiled body) | manager_sub_01140ED0 | $a0: +0x70:W16 +0x80:W16 +0x90:W4 +0x94:W4 +0x98:W4 +0x9c:W4 +0x110:R4/W4; $a1: +0x64:R4 +0x68:R4 +0x6c:R4 +0xb0:R4; $a2: +0x164:R4; $a3: +0x0:R16 +0x30:R16; globals: 001e0fe0:&R 011958b0:&R 011958b8:R |
| 0113b610 | manager_sub_0113B610 | native: sotc_worker.cpp | manager_sub_0113AAE0 | $a0: +0x14:R4 +0x118:R4 +0x130:W4 +0x134:W4 +0x14c:R4 +0x150:W4 +0x154:W4 +0x168:R4 +0x16c:R4 +0x198:W4; $a1: +0xcc:R4 +0xd4:R4 +0xd8:R4; globals: 0118cfe8:W 011958c0:& 011958d0:& 01216e18:& 01216e28:& 0121dc20:& |
| 0113b928 | manager_sub_0113B928 | native: sotc_worker.cpp | manager_sub_0113B610 | $a0: +0x0:W16 +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4 +0x50:R4 +0x54:R4 +0x58:R4 +0x5c:R4 +0xc4:W4 +0x114:R4/W4 +0x11c:R4/W4 +0x120:R4/W4 +0x124:R4 +0x128:R4 +0x190:R4/W4; $a1: +0x1c:R4; $a2: +0x0:R4 +0x10:R4; $a3: +0x4:R4 + |
| 0113c1a8 | manager_sub_0113C1A8 | native: sotc_worker.cpp | manager_sub_0113B928 | $a0: +0x0:R16/R4/W16 +0x4:R4/W4 +0x8:R4 +0x10:R16 +0x30:W16/W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:R4/W4 +0x44:W4 +0x48:W4 +0x4c:W4 +0x50:W4 +0x54:W4 +0x58:R4/W4 +0x5c:W4 +0x60:W4 +0x64:W4 +0x68:W4 +0x6c:W4 +0x70:W4 +0x74:W |
| 0113c8c8 | manager_sub_0113C8C8 | native: decomp_particles.cpp | manager_sub_0113C1A8, manager_sub_0113CA58 | $a1: +0xd8:R4; globals: 001e0ed4:R 0118ccc8:R 0118cfe4:R 0118cfec:R |
| 0113ca58 | manager_sub_0113CA58 | native: decomp_particles.cpp | manager_sub_0113B928 | $a0: +0x0:R16/W16 +0x10:R4/W4 +0x14:R4/W4 +0x18:R4/W4 +0x20:R16 +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:R4/W4 +0x44:R4/W4 +0x4c:R4/W4 +0x50:R4/W4 +0x68:R4/W4 +0x6c:R4/W4 +0x70:R4/W4 +0x74:R4/W4 +0x78:R4/W4 +0x7c:R4/W4  |
| 0113d280 | manager_sub_0113D280 | gate (recompiled body) | manager_sub_0113CA58 | $a0: +0x0:R16/R4/W16/W4 +0x4:R4/W4 +0x8:R4/W4 +0x30:W16 +0x40:W4 +0x44:R4 +0xa0:W4 +0xa4:R4/W4 +0xa8:R4/W4; $a3: +0xc:R4 +0x104:R4 +0x11c:R4 +0x16c:R4; globals: 0118cfe8:R 01216df0:&RW 01216df4:W 01216df8:RW |
| 0113d5f0 | manager_sub_0113D5F0 | gate (recompiled body) | manager_sub_0113D280 | $a0: +0x0:W16; $a1: +0x0:W16 +0xc:W4; $a2: +0x188:R4 +0x18c:R4/W4; globals: 01123520:& |
| 0113d7b0 | manager_sub_0113D7B0 | gate (recompiled body) | manager_sub_0113D5F0 | globals: 001e0ed4:R 001e0fc0:& |
| 0113d988 | disp_particle_emitter | native: sotc_worker.cpp | manager_sub_01140ED0 | $a0: +0x118:R4 +0x16c:R4 +0x198:R4; $a1: +0xb0:R4 +0xd4:R4 +0x120:R4 |
| 0113dbf8 | manager_sub_0113DBF8 | native: decomp_hot46j.cpp | disp_particle_emitter | $a0: +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a2: +0x1c:R4 +0x24:R4 +0x58:R4 +0x5c:R4 +0x12c:R4 +0x130:R4; globals: 0118cff0:R |
| 0113dea0 | manager_sub_0113DEA0 | native: decomp_hot46j.cpp | disp_particle_emitter | $a0: +0x10:R16 +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a1: +0x14:R4 +0x1b4:R4; $a2: +0x1c:R4 +0x24:R4 +0x58:R4 +0x5c:R4; globals: 001e0f00:& 0118cff0:R |
| 0113e218 | manager_sub_0113E218 | gate (recompiled body) | disp_particle_emitter | $a0: +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a1: +0x170:R4; globals: 011958f0:&R 011958f7:R 011958f8:R 011958ff:R |
| 0113e6c8 | manager_sub_0113E6C8 | native: decomp_hot46j.cpp | disp_particle_emitter | $a0: +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a1: +0x14:R4; $a2: +0x1c:R4 +0x24:R4 +0x128:R4 +0x12c:R4 +0x130:R4; globals: 0118cff0:R |
| 0113e940 | manager_sub_0113E940 | native: sotc_worker.cpp | disp_particle_emitter | $a0: +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a1: +0x14:R4; $a2: +0x1c:R4 +0x24:R4 +0x50:R4 +0x54:R4 +0x58:R4 +0x5c:R4 +0xb4:R4 +0xb8:R4; globals: 0118cff0:R 01216e40:&R 01216e50:R 01216e60:R  |
| 0113ec40 | manager_sub_0113EC40 | native: sotc_worker.cpp | disp_particle_emitter | $a0: +0x10:R16 +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a1: +0x14:R4 +0x1b4:R4; $a2: +0x1c:R4 +0x24:R4 +0x50:R4 +0x54:R4 +0x58:R4 +0x5c:R4 +0xb4:R4 +0xb8:R4; globals: 0118cff0:R 01216e40:&R 01 |
| 0113f220 | disp_particle_manager | native: sotc_worker.cpp | gamecore_sub_01345110 | globals: 0118d000:&RW 0118d010:RW 0118d014:W 0118d020:RW 0118d030:RW 0118d034:W |
| 0113f370 | free_all_emitter_and_particle_particle_manager | native: sotc_worker.cpp | st_gameflow |  |
| 0113f378 | get_top_emitter_list_from_particle_geodisp | native: sotc_worker.cpp | doit_particle_manager, manager_sub_01139F10, manager_sub_01140748 |  |
| 0113f3a8 | create_and_boot_particle_geodisp | native: sotc_worker.cpp | manager_sub_01140748 |  |
| 0113f458 | check_free_particle_geodisp | gate (recompiled body) | (table / pointer only) | $a0: +0x0:R4/W4 |
| 0113f4a0 | auto_end_check_and_check_free_particle_geodisp | native: sotc_worker.cpp | doit_particle_manager | $a0: +0x0:R4/W4 |
| 0113f500 | doit_particle_geodisp | native: sotc_worker.cpp | doit_particle_manager |  |
| 0113f530 | clip_particle_geodisp | native: sotc_worker.cpp | (table / pointer only) |  |
| 0113f600 | calc_light_particle_geodisp | gate (recompiled body) | (table / pointer only) |  |
| 0113f6a8 | disp_particle_geodisp | native: sotc_worker.cpp | (table / pointer only) |  |
| 0113f700 | is_loop_particle | native: decomp_liftd7_08.cpp | gamecore_sub_013FB7C8, gamecore_sub_013FCB50, manager_sub_010BAAF0, manager_sub_0110D618 |  |
| 0113f748 | boot_particle_emitter | native: decomp_liftd7_08.cpp | SequenceKickParticle, boot_weakpoint_hit_effect, gamecore_sub_01386C60, gamecore_sub_013FB018, gamecore_sub_013FCCE0 |  |
| 0113f760 | boot_particle_emitter_extend | native: sotc_worker.cpp | boot_particle_emitter, manager_sub_0113D7B0 |  |
| 0113f7d8 | stop_particle_emitter_geometry_pointer_reference | native: sotc_worker.cpp | end_weakpoint_effect, gamecore_sub_01386EF8, init_weakpoint_effect, manager_sub_011697A0 |  |
| 0113f808 | stop_particle_emitter_callback | native: sotc_worker.cpp | manager_sub_011697A0 |  |
| 0113f838 | set_generate_particle_flag | native: sotc_worker.cpp | manager_sub_0110C420, particleOnOff |  |
| 0113f878 | set_npe_blend_ratio_particle_emitter | native: decomp_liftd7_08.cpp | gamecore_sub_01387090, gamecore_sub_013FB270, gamecore_sub_013FC7E0, gamecore_sub_013FCA10, manager_sub_01108960, nico_swordstart, updateParticleDissolveGeo |  |
| 0113f8b0 | end_particle_emitter | native: decomp_liftd7_08.cpp | end_weakpoint_effect, gamecore_sub_01386EF8, gamecore_sub_013FB7C8, gamecore_sub_013FCB50, init_weakpoint_effect, manager_sub_011697A0 |  |
| 0113f8f8 | get_particle_effect_id_by_name | native: sotc_worker.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | $a0: +0x0:R4; globals: 001e0ed4:R |
| 0113fa20 | get_particle_effect_id_from_sheet | gate (recompiled body) | boot_weakpoint_hit_effect, end_weakpoint_effect, gamecore_sub_01386C60, gamecore_sub_01386EF8, gamecore_sub_013FB018, gamecore_sub_013FB7C8, gamecore_sub_013FCB |  |
| 0113fa28 | get_particle_name_from_effect_id | gate (recompiled body) | (table / pointer only) | $a0: +0x24:R4 |
| 0113fa40 | get_particle_camera_follow_attr_from_effect_id | native: sotc_worker.cpp | updateParticleGeo |  |
| 0113fa70 | get_particle_emitter_from_emitter_id | native: decomp_liftd7_08.cpp | boot_weakpoint_hit_effect, end_particle_emitter, gamecore_sub_01386C60, gamecore_sub_013FB018, gamecore_sub_013FCCE0, set_generate_particle_flag, set_npe_blend_ |  |
| 0113fa88 | get_main_particle_effect_lod_from_sheet | native: decomp_liftd7_08.cpp | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8, get_particle_camera_follow_attr_from_effect_id, is_loop_particle | $a0: +0x48:R4 |
| 0113fa90 | get_sub_particle_effect_lod_from_sheet | gate (recompiled body) | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8 | $a0: +0x4c:R4 |
| 0113fa98 | get_top_particle_effect_from_lod | native: decomp_liftd7_08.cpp | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8, is_loop_particle | $a0: +0xd4:R4 |
| 0113faa0 | get_particle_effect_from_lod | gate (recompiled body) | (table / pointer only) | $a0: +0xd4:R4 |
| 0113fac0 | reinit_particle_effect_lod | gate (recompiled body) | (table / pointer only) |  |
| 0113fad8 | quit_particle_texture_user_by_id | gate (recompiled body) | (table / pointer only) | globals: 0118cfd0:& 0118d048:& |
| 0113fb50 | set_particle_texture_by_id | gate (recompiled body) | (table / pointer only) |  |
| 0113fb88 | getPtclDisp | gate (recompiled body) | (table / pointer only) | globals: 0118cff0:& |
| 0113fb98 | get_particle_printf_disp_level | gate (recompiled body) | (table / pointer only) | globals: 0118cfd8:& |
| 0113fba8 | get_particle_camera_data | gate (recompiled body) | (table / pointer only) | globals: 01216e40:& |
| 0113fbb8 | is_proper_particle_texture_id | gate (recompiled body) | (table / pointer only) |  |
| 0113fbf0 | create_particle_effect_lod | native: sotc_worker.cpp | create_and_init_particle_effect_lod | globals: 01195840:& |
| 0113fc40 | set_except_npe_file_data_to_particle_effect_lod | native: sotc_worker.cpp | create_and_init_particle_effect_lod | $a0: +0xd0:R4 +0xd4:R4 +0xd8:W4 |
| 0113fc98 | init_particle_effect_lod | native: sotc_worker.cpp | create_and_init_particle_effect_lod, reinit_particle_effect_lod | $a0: +0x50:R4 +0x54:R4 +0x7c:R4 +0x80:R4 +0xd0:R4 +0xd4:R4 +0x120:W4 +0x124:W4 +0x128:W4 +0x12c:W4 +0x130:W4 +0x134:R4/W4 |
| 0113fdc8 | is_emitter_need_disp | native: sotc_worker.cpp | manager_sub_01140C10 | $a0: +0x124:R4 +0x140:R4 |
| 0113fe20 | check_free_particle_effect_lod | gate (recompiled body) | (table / pointer only) | $a0: +0xd0:R4 +0xd4:R4; globals: 0118cfd0:& 0118d048:& |
| 0113ff20 | recreate_particle_effects | native: sotc_worker.cpp | manager_sub_01144E90, set_npe_file_data_to_particle_effect_lod | $a0: +0xd0:W4 +0xd4:R4/W4; globals: 01195830:& |
| 0113ff90 | particleGWorkPut | gate (recompiled body) | (table / pointer only) | $a0: +0x0:R4/W4 +0x4:R4/W4 +0x8:R4/W4; $a1: +0xa8:R4/W4; globals: 01216df0:&RW 01216df4:W 01216df8:RW |
| 01140050 | get_polygon_scale_rotation_matrix | native: sotc_worker.cpp | init_particle_effect_lod | $a0: +0x50:R4 +0x54:R4 +0xdc:W4; globals: 001e0fc0:& |
| 011401d8 | get_radiate_range_coefficient | native: sotc_worker.cpp | init_particle_effect_lod |  |
| 011401f8 | constructor_particle_effect_lod_except_npe | native: sotc_worker.cpp | manager_sub_01144E90 | $a0: +0xd0:W4 +0xd4:W4 +0xd8:W4 +0xdc:W4 |
| 01140228 | add_particle_texture_user | gate (recompiled body) | (table / pointer only) | $a0: +0x4:R4/W4 |
| 01140238 | create_and_init_particle_effect_lod | native: sotc_worker.cpp | manager_sub_011405B8 | globals: 0118cfd0:& 0118d048:& |
| 011403c0 | get_particle_lod_base_size | gate (recompiled body) | manager_sub_01141150 |  |
| 011403f8 | set_act_particle_lod_select_flag | gate (recompiled body) | (table / pointer only) | globals: 0118d040:W |
| 01140408 | set_disp_particle_collisin_debug_flag | gate (recompiled body) | (table / pointer only) | globals: 0118d044:W |
| 01140418 | get_world_matrixSR | native: sotc_worker.cpp | disp_particle_emitter, manager_sub_01141230 |  |
| 01140440 | radiate_particle_collision_check_ray | gate (recompiled body) | manager_sub_01141400 | $a1: +0x0:W16 +0xc:W4; globals: 01123520:& 0118cfe4:R |
| 011404c0 | init_texture_animation_parameter | gate (recompiled body) | (table / pointer only) | $a0: +0x7c:R4 +0x80:R4 +0x120:W4 +0x124:W4 +0x128:W4 +0x12c:W4 +0x130:W4 |
| 01140548 | get_particle_geodispobj_group | gate (recompiled body) | (table / pointer only) | globals: 01216e00:& |
| 01140558 | get_set_limit_gl_draw_particle_flag | gate (recompiled body) | (table / pointer only) | globals: 0118cff8:& |
| 01140568 | create_particle_texture | gate (recompiled body) | (table / pointer only) | globals: 001e0ed4:R |
| 011405b8 | manager_sub_011405B8 | native: sotc_worker.cpp | add_particle_segment | $a0: +0x0:R4/W4 |
| 01140630 | manager_sub_01140630 | native: sotc_worker.cpp | add_particle_segment | globals: 0118d228:& 0118d22c:& |
| 01140678 | free_particle_segment | native: sotc_worker.cpp | (table / pointer only) |  |
| 01140690 | load_npe_file_main | native: sotc_worker.cpp | (table / pointer only) |  |
| 011406e0 | load_npe_file_sub | native: sotc_worker.cpp | (table / pointer only) |  |
| 01140730 | unload_npe_file_main | native: sotc_worker.cpp | (table / pointer only) |  |
| 01140738 | unload_npe_file_sub | native: sotc_worker.cpp | (table / pointer only) |  |
| 01140740 | manager_sub_01140740 | native: sotc_worker.cpp | boot_particle_emitter_extend, is_loop_particle |  |
| 01140748 | manager_sub_01140748 | native: sotc_worker.cpp | boot_particle_emitter_extend | $a0: +0x2c:R4; globals: 01216e00:& |
| 01140898 | manager_sub_01140898 | native: sotc_worker.cpp | init_particle_manager | globals: 01216e40:& 01216e80:& 01216ec0:W 01216ec4:W 01216ec8:W 01216ecc:W 01216ed0:W |
| 011408e0 | manager_sub_011408E0 | native: sotc_worker.cpp | doit_particle_manager | $a0: +0x118:R4 +0x124:R4; $a1: +0xd4:R4; globals: 0118cff0:R 0118cff4:&RW 0118cff8:R |
| 011409a8 | manager_sub_011409A8 | gate (recompiled body) | manager_sub_01140748 | $a0: +0x0:R4/W4; $a1: +0x0:R16; $a2: +0x2c:R4 +0x30:R4 +0x34:R4 +0x48:R4; globals: 01195868:& |
| 01140b70 | manager_sub_01140B70 | native: sotc_worker.cpp | create_and_boot_particle_geodisp | $a1: +0x48:R4; globals: 01195868:& |
| 01140c10 | manager_sub_01140C10 | native: sotc_worker.cpp | clip_particle_geodisp | $a0: +0x0:R4 +0x1b0:R4 |
| 01140c80 | manager_sub_01140C80 | native: sotc_worker.cpp | calc_light_particle_geodisp | $a0: +0x0:R4 |
| 01140ce0 | manager_sub_01140CE0 | native: sotc_worker.cpp | manager_sub_011409A8, manager_sub_01140B70 | $a0: +0x0:W16 +0x70:W16 +0x80:W16 +0x90:W16 +0xc0:W4 +0xc4:W4 +0x110:W4 +0x114:W4 +0x118:W4 +0x11c:W4 +0x120:W4 +0x130:W4 +0x134:W4 +0x138:W4 +0x13c:W4 +0x140:W4 +0x144:W4 +0x148:W4 +0x158:W4 +0x15c:W4 +0x160:W4 +0x164:W |
| 01140e60 | manager_sub_01140E60 | native: sotc_worker.cpp | manager_sub_01140C80 | $a0: +0x0:R4 +0x110:R4; $a1: +0x0:R4 +0x64:R4 +0xb0:R4; $a2: +0x164:R4 |
| 01140ed0 | manager_sub_01140ED0 | native: sotc_worker.cpp | manager_sub_0113ACB8 | $a0: +0x0:R4 |
| 01140f50 | manager_sub_01140F50 | native: sotc_worker.cpp | manager_sub_01140CE0 | $a0: +0x0:W4; $a1: +0x14:R4; $a2: +0x15c:R4 +0x160:R4; $a3: +0xc8:R4 |
| 01140fd8 | manager_sub_01140FD8 | native: sotc_worker.cpp | manager_sub_01140CE0 | $a0: +0x14:R4 +0x14c:R4 +0x150:W4 +0x154:W4; $a1: +0xb4:R4 +0x140:R4 +0x15c:R4 +0x160:R4 |
| 01141080 | manager_sub_01141080 | native: sotc_worker.cpp | manager_sub_0113AEF8, manager_sub_0113B928, manager_sub_01141090 | $a0: +0xc8:R4 |
| 01141090 | manager_sub_01141090 | gate (recompiled body) | manager_sub_0113B610 |  |
| 011410d0 | manager_sub_011410D0 | gate (recompiled body) | manager_sub_0113B610 | $a0: +0x130:R4 +0x138:R4 +0x13c:R4/W4 +0x140:W4; $a1: +0x28:R4 +0x2c:R4 +0x30:R4 |
| 01141150 | manager_sub_01141150 | gate (recompiled body) | manager_sub_0113B610 | $a0: +0x118:R4/W4; $a1: +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 |
| 01141230 | manager_sub_01141230 | native: sotc_worker.cpp | manager_sub_0113B610 | $a0: +0x194:R4/W4; $a2: +0x164:R4; globals: 011958e0:&R 011958e8:R |
| 01141400 | manager_sub_01141400 | gate (recompiled body) | manager_sub_0113B928 | $a0: +0xa0:W16 +0xb0:W16 +0x184:R4/W4 +0x188:W4; $a1: +0x104:R4; globals: 0118cfe8:R |
| 011414e8 | ClipCollisionDirectForParticle | gate (recompiled body) | (table / pointer only) | globals: 01123520:& |
| 01141518 | manager_sub_01141518 | gate (recompiled body) | manager_sub_01141400 | globals: 0118cfe4:R |
| 01141690 | manager_sub_01141690 | gate (recompiled body) | manager_sub_01141518 | globals: 001e0c88:& |
| 011416e0 | manager_sub_011416E0 | native: sotc_worker.cpp | manager_sub_0113C1A8 | $a0: +0x20:W16; $a1: +0x0:R16/W16; $a2: +0x0:W4 |
| 01141840 | manager_sub_01141840 | gate (recompiled body) | manager_sub_0113C1A8 | $a0: +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4; globals: 0118cfe8:R |
| 011418d8 | manager_sub_011418D8 | native: sotc_worker.cpp | manager_sub_0113C1A8 | $a0: +0x4:W4; $a1: +0x14c:R4; $a2: +0xc8:R4; globals: 01216ec4:R |
| 01141918 | manager_sub_01141918 | gate (recompiled body) | manager_sub_0113CA58 | globals: 0118cfe8:R |
| 01141968 | manager_sub_01141968 | gate (recompiled body) | manager_sub_0113D280 | $a0: +0x0:R16/W16 +0x48:R4 +0x80:R4/W4 +0x88:R4; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 0118cfe8:&R |
| 01141b70 | manager_sub_01141B70 | gate (recompiled body) | manager_sub_0113D280 | $a0: +0x4:R4/W4 +0x4c:R4 +0x54:R4/W4; $a1: +0x150:R4 +0x154:R4; $a2: +0x134:R4; $a3: +0x130:R4 +0x134:R4 +0x138:R4 +0x13c:R4 +0x140:R4 +0x144:R4; globals: 001e0fb0:& 001e1000:& |
| 01141ce0 | manager_sub_01141CE0 | gate (recompiled body) | manager_sub_0113D5F0 | $a0: +0x0:W16; $a1: +0x0:W16 +0xc:W4; $a2: +0x40:R16 |
| 01141d20 | manager_sub_01141D20 | native: sotc_worker.cpp | disp_particle_emitter | $a0: +0x70:R4 +0x74:R4 +0x78:R4 +0x124:R4 +0x128:R4 +0x140:R4 +0x144:R4; $a2: +0x58:R4 +0x5c:R4 |
| 01141e80 | manager_sub_01141E80 | native: sotc_worker.cpp | disp_particle_emitter | $a0: +0x34:R4 +0xdc:R4 |
| 01141ea8 | manager_sub_01141EA8 | native: sotc_worker.cpp | get_world_matrixSR | $a0: +0x0:R16/W16 +0x10:R16/W16 +0x20:R16/W16 +0x30:R16/W16; $a3: +0x34:R4 +0x60:R4; globals: 001e0fc0:& 01216e40:& 01216e80:&R 01216e90:R 01216ea0:R 01216eb0:R |
| 01142030 | manager_sub_01142030 | native: sotc_worker.cpp | manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E218, manager_sub_0113E6C8, manager_sub_0113E940, manager_sub_0113EC40, manager_sub_01141D20 | $a0: +0x40:R4 +0x44:R4 +0x58:R4 +0x5c:R4 +0x60:R4 +0x64:R4 |
| 01142080 | manager_sub_01142080 | native: sotc_worker.cpp | manager_sub_01141D20 |  |
| 01142148 | manager_sub_01142148 | native: sotc_worker.cpp | manager_sub_01141D20 |  |
| 01142198 | manager_sub_01142198 | native: decomp_hot46j.cpp | manager_sub_0113DEA0 | $a0: +0x0:W16; $a1: +0x0:R16 +0x44:R4 +0x98:R4 +0xa0:R4 |
| 01142230 | manager_sub_01142230 | gate (recompiled body) | manager_sub_0113E218 | $a0: +0x14:R4 |
| 011422d8 | manager_sub_011422D8 | gate (recompiled body) | manager_sub_0113E218 | globals: 0118d000:&R 0118d010:R 0118d020:R 0118d030:R |
| 01142350 | manager_sub_01142350 | native: decomp_walk.cpp | manager_sub_01141D20 | $a0: +0x0:R16 +0xc:R4 +0x40:R4; $a1: +0x0:R4 +0x4:R4; $a2: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; globals: 01195900:&R 01195907:R 01195908:R 0119590f:R 01195910:R 01195917:R 01195918:R 0119591f:R 01195920:R 01195927:R 0 |
| 011425f0 | manager_sub_011425F0 | native: sotc_worker.cpp | manager_sub_01142080 | $a0: +0x10:R16; $a1: +0x24:R4 |
| 01142648 | manager_sub_01142648 | native: sotc_worker.cpp | load_npe_file_main, load_npe_file_sub | globals: 01195820:& |
| 011426a8 | set_npe_file_data_to_particle_effect_lod | native: sotc_worker.cpp | create_and_init_particle_effect_lod | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 +0x34:W4 +0x40:W16 +0x50:W4 +0x54:W4 +0x58:W4 +0x5c:W4 +0x60:W4 +0x64:W4 +0x68:W4 +0x6c:W4 +0x70:W4 +0 |
| 01144e00 | manager_sub_01144E00 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0x190:W4 +0x194:W4 +0x198:W4 +0x19c:W4 +0x1a0:W4 +0x1a4:W4 +0x1a8:W4 +0x1ac:W4 +0x1b0:W4 |
| 01144e38 | manager_sub_01144E38 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0xa0:W4 +0xa4:W4 +0xa8:W4 |
| 01144e50 | manager_sub_01144E50 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0x1b4:W4 |
| 01144e58 | manager_sub_01144E58 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0xb0:W4 +0xb4:W4 +0xb8:W4 |
| 01144e70 | manager_sub_01144E70 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0xbc:W4 +0xc0:W4 +0xc4:W4 +0xc8:W4 |
| 01144e88 | manager_sub_01144E88 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0xcc:W4 |
| 01144e90 | manager_sub_01144E90 | native: sotc_worker.cpp | set_npe_file_data_to_particle_effect_lod | $a0: +0x8:R4 +0xd4:R4 |
| 01144f68 | get_particle_parameter_blend_doit | native: sotc_worker.cpp | manager_sub_0113B610 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 +0x44:W4 +0x48:W4 +0x4c:W4 +0x50:W4 +0x54:W4 +0x58:W4 +0x5c:W4 +0x |
| 01145ba8 | get_particle_parameter_blend_disp | gate (recompiled body) | disp_particle_emitter | $a0: +0x0:W4; $a1: +0xd4:R4; $a2: +0xd0:R4 +0xd4:R4 |
| 01145c70 | manager_sub_01145C70 | gate (recompiled body) | get_particle_parameter_blend_doit | $a1: +0xd0:R4 +0xd4:R4 |
| 01145cd0 | manager_sub_01145CD0 | gate (recompiled body) | get_particle_parameter_blend_doit | $a0: +0x0:W4; $a1: +0x0:W4 |
| 01145d28 | manager_sub_01145D28 | native: sotc_worker.cpp | boot_wind_emitter_by_box, manager_sub_01146D78 | $a0: +0x4:R4 +0x8:R4 +0x68:R4 +0x6c:R4 +0xa0:R4/W4 +0xa4:R4/W4 +0xb0:W16 +0xd0:W4 +0x160:W4 |
| 01145f78 | manager_sub_01145F78 | native: decomp_hot48c.cpp | GetWindVector | globals: 01216ef0:& 01217100:&W 01217104:W 01217108:W 01217110:W 01217120:& |
| 011461c0 | dynamic_cast_to_wind_emitter_sphere | gate (recompiled body) | (table / pointer only) | $a0: +0x0:R4 |
| 011461e0 | dynamic_cast_to_wind_emitter_box | gate (recompiled body) | (table / pointer only) | $a0: +0x0:R4 |
| 01146208 | dynamic_cast_to_wind_emitter_judge_volume | gate (recompiled body) | (table / pointer only) | $a0: +0x0:R4 |
| 01146230 | init_wind_manager | native: sotc_worker.cpp | initmanager | globals: 0118d548:& 0118d550:& 0118d610:& 0118d614:&W 01216ef0:& 01217030:& |
| 011462a0 | doit_wind_manager | native: sotc_worker.cpp | gamecore_sub_013450C8 | globals: 01216ef0:&R 01216ef4:& |
| 011463b0 | get_wind_effect_data | native: sotc_worker.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | $a0: +0x0:R4; globals: 001e0ed4:R |
| 011464c0 | is_eternal_wind | native: sotc_worker.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | $a0: +0xc:R4 |
| 011464e0 | boot_wind_emitter | native: sotc_worker.cpp | SequenceKickWind | globals: 001e0fc0:&R 001e0fd0:&R 0118ccf0:& 01195940:& 01216ef0:&R 01216ef4:&R |
| 01146668 | boot_wind_emitter_by_box | native: sotc_worker.cpp | SequenceKickWindBox | globals: 001e0f00:& 001e0f90:&R 001e0fc0:&R 001e0fd0:&R 0118ccf0:& 01195948:& 01216ef0:&R 01216ef4:&R |
| 01146818 | boot_wind_emitter_by_judge_volume | gate (recompiled body) | SequenceKickWindVolumeShape | globals: 001e0fc0:&R 001e0fd0:&R 0118ccf0:& 0118cd40:& 01195950:& 01216ef0:&R 01216ef4:&R |
| 01146980 | stop_wind_emitter_geometry_pointer_reference | native: sotc_worker.cpp | manager_sub_01169B20 |  |
| 011469d0 | stop_wind_emitter_callback | native: sotc_worker.cpp | manager_sub_01169B20 |  |
| 01146a10 | end_wind_emitter | native: sotc_worker.cpp | manager_sub_01169B20 |  |
| 01146a50 | GetWindVector | native: decomp_hot48c.cpp | GetLayoutClassWindGeometry, manager_sub_0113B928 | $a0: +0x0:W4; globals: 001e0f90:&R 01216ee0:&RW 01216ef0:& 01217100:&R 01217104:R 01217108:R 01217110:& |
| 01146c08 | constructor_wind_effect_data | gate (recompiled body) | (table / pointer only) | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4; globals: 0118d548:& |
| 01146c30 | is_ambient_directional_wind_emitter | gate (recompiled body) | (table / pointer only) | $a0: +0x4:R4 |
| 01146c40 | get_wind_emitter_direct | gate (recompiled body) | (table / pointer only) |  |
| 01146c58 | get_wind_effect_data_direct | gate (recompiled body) | (table / pointer only) |  |
| 01146c70 | manager_sub_01146C70 | native: sotc_worker.cpp | doit_wind_manager, free_wind_segment | $a0: +0xf0:R4 +0xf4:R4/W4 +0xf8:R4/W4 |
| 01146cc0 | manager_sub_01146CC0 | native: sotc_worker.cpp | doit_wind_manager, free_wind_segment | $a0: +0xf0:R4 +0xf4:R4/W4 +0xf8:R4/W4 |
| 01146d10 | manager_sub_01146D10 | gate (recompiled body) | doit_wind_manager, free_wind_segment | $a0: +0xf0:R4 +0xf4:R4/W4 +0xf8:R4/W4 |
| 01146d60 | manager_sub_01146D60 | native: sotc_worker.cpp | doit_wind_manager |  |
| 01146d78 | manager_sub_01146D78 | native: sotc_worker.cpp | doit_wind_manager |  |
| 01146d90 | manager_sub_01146D90 | gate (recompiled body) | doit_wind_manager |  |
| 01146da8 | manager_sub_01146DA8 | native: decomp_hot48c.cpp | GetWindVector | $a0: +0x0:W16; $a1: +0x4:R4 +0xd0:R4 +0xe0:R16; $a2: +0x0:R16/R4 +0x8:R4; globals: 001e0f90:&R |
| 01146eb8 | manager_sub_01146EB8 | native: sotc_worker.cpp | manager_sub_01145F78 | $a0: +0x0:R4/W4; $a1: +0x0:R16/W16; $a2: +0x0:R16; $a3: +0x4:R4 +0x100:R4 +0x104:R4 |
| 01146fb8 | manager_sub_01146FB8 | native: sotc_worker.cpp | manager_sub_01145F78 | $a0: +0x0:R4/W4; $a1: +0x0:R16/W16; $a2: +0x0:R16; $a3: +0x4:R4 +0x160:R4 |
| 01147070 | manager_sub_01147070 | gate (recompiled body) | manager_sub_01145F78 | $a0: +0x0:W4; $a1: +0x0:R16/W16; $a2: +0x0:R16; $a3: +0x4:R4 +0x100:R4 |
| 01147150 | manager_sub_01147150 | native: sotc_worker.cpp | boot_wind_emitter, manager_sub_01146D60 | $a0: +0x4:R4 +0xa0:R4/W4 +0xa4:R4/W4 |
| 011472a8 | manager_sub_011472A8 | gate (recompiled body) | boot_wind_emitter_by_judge_volume, manager_sub_01146D90 | $a0: +0x4:R4 +0xa0:R4/W4 +0xa4:R4/W4 |
| 01147430 | add_wind_segment | native: sotc_worker.cpp | (table / pointer only) |  |
| 01147438 | free_wind_segment | native: sotc_worker.cpp | (table / pointer only) | $a0: +0xc:R4 +0x10:R4; globals: 01216ef0:& |
| 01167238 | GetAttributeRelationEffectDefByName | native: decomp_liftd6_00.cpp | manager_sub_010BAAF0 | $a0: +0x0:W4; $a1: +0x0:R4; globals: 001e0ed4:R |
| 011674a0 | GetCollisionDetectEffectWaterOnlyDefByName | native: decomp_liftd6_00.cpp | manager_sub_010BAAF0 | $a0: +0x0:R4; globals: 001e0ed4:R |
| 01169388 | ExecEff | native: decomp_liftd7_08.cpp | gamecore_sub_0136B710, gamecore_sub_01370030, gamecore_sub_013B0EB8, gamecore_sub_013B3B98, gamecore_sub_013B3D08, gamecore_sub_013B3E68, gamecore_sub_013B3FC8, | globals: 001e0fe0:&R 0118fe88:W 0118fe8c:W 0118fe90:W 0118fe94:W 0118fe98:W 0122b860:&W 0122b870:&W 0122b880:&W |
| 011696a8 | SequenceKickParticle | native: decomp_liftd6_01.cpp | kickParticle, manager_sub_01167F88 | globals: 011697e8:& |
| 01169778 | KickParticleGetResourceId | native: decomp_hot46j.cpp | manager_sub_01108960, manager_sub_0110C420, particleOnOff, updateParticleDissolveGeo |  |
| 01347358 | set_yaw_parameter_effective_ratio_nico_yaw_pitch_cntrl | native: decomp_liftd7_07.cpp | gamecore_sub_01422E90, set_option_parameter_nico_sight_on | $a0: +0x138:W4 |
| 01347360 | set_pitch_parameter_effective_ratio_nico_yaw_pitch_cntrl | native: decomp_liftd7_07.cpp | gamecore_sub_01422E90, set_option_parameter_nico_sight_on | $a0: +0x13c:W4 |
| 0134c858 | setDefaultFsEffect | native: decomp_liftd6_01.cpp | st_activedemoflow, st_endingflow, st_jumphokoraflow, st_opflow |  |
| 0135ede8 | createSpecialEffectObj | native: decomp_liftd6_01.cpp | loadObj | $a1: +0xc:R4 +0x14:R4; $a2: +0x0:R4 +0x4:R4 |
| 0135ef18 | updateSpecialEffectManager | native: decomp_liftd6_01.cpp | gamecore_sub_01345110 | globals: 01465950:& |
| 0135efb0 | getSpecialEffectObjWork | native: decomp_liftd4_02.cpp | arrowWaterInit, arrowWaterMain, dischargeInit, dischargeMain, dischargeTerm, fallenLeafInit, fallenLeafMain, setDischargeParam | $a0: +0x30:R4 |
| 0137e928 | NICO_is_hit_except_underwater | recompiled | (table / pointer only) | globals: 001b7b60:R |
| 01387a10 | effectSetDamageWork | recompiled | SCRExecBossPreProcess | globals: 011e5fa0:& 011e6220:RW 011e6248:RW |
| 01387db8 | effect_blood_init | native: decomp_liftd8_01.cpp | SCRCharaInitBossWork | globals: 011e6250:& 0145ffff:& 01466510:W 01466514:W 01466518:W 0146651c:W 01466520:&W 01466524:W 01466528:W |
| 01387eb8 | effect_blood_free | native: decomp_liftd8_01.cpp | SCRReleaseBossWork |  |
| 01387ed0 | effect_blood_frame | native: decomp_liftd8_01.cpp | SCRExecBossPreProcess | $a0: +0x0:R16/W16 +0x20:R4/W4 +0x24:R4/W4; globals: 0146651c:&R |
| 01387f70 | effect_blood_set | recompiled | SCRExecBossPreProcess | globals: 011e6250:& 01466510:R |
| 01388048 | getEffectBloodWorkSize | native: decomp_liftd8_01.cpp | SCRGetSizeOfBossWork |  |
| 01388060 | getWeakPointEffectIntensity | native: decomp_liftd8_01.cpp | SCRExecBossPreProcess |  |
| 01388080 | getTriggerPointEffectIntensity | recompiled | SCRExecBossPreProcess |  |
| 013880a8 | boot_weakpoint_effect | recompiled | gamecore_sub_013FC148, gamecore_sub_013FC7E0 | $a0: +0x0:R4 +0x4:R4; globals: 001e0c88:& 011e6290:& 01388790:& 014657c8:R 014666f0:R 014686e8:& |
| 013881a8 | boot_weakpoint_hit_effect | recompiled | gamecore_sub_013FC148 | $a0: +0x0:R4 +0x4:R4; globals: 001b7b60:R 011e6290:& 013888e0:& 014666f0:R |
| 01388288 | init_weakpoint_effect | native: decomp_liftd8_01.cpp | SCRCharaInitBossWork | globals: 001b7b60:R 001e0f40:& 001e0fd0:& 011e6290:&R 011e62ec:R 011e62f0:R 011e62f4:W 011e62f8:W 011e6308:& 014666f0:W |
| 013883b0 | fadeout_weakpoint_effect | native: decomp_liftd7_08.cpp | checkChangeWeaponTypeSwordToOther | globals: 011e6290:& 011e6294:W 011e62a0:W 011e6308:& 014666f0:R |
| 013883f8 | cancel_fadeout_weakpoint_effect | native: decomp_liftd7_08.cpp | checkChangeWeaponTypeSwordToOther | globals: 011e6290:& 011e6294:RW 011e6308:& 014666f0:R |
| 01388440 | end_weakpoint_effect | native: decomp_liftd8_01.cpp | SCRExecBossPreProcess, SCRReleaseBossWork | globals: 001b7b60:R 001e0f40:& 001e0fd0:& 011e6290:&R 011e62ec:R 011e62f0:R 011e62f4:W 011e62f8:W 011e6308:& 014666f0:RW |
| 01388568 | effect_death | recompiled | st_boss_dead | globals: 001e0c88:& |
| 013885f0 | effectClearDamageWork | native: decomp_liftd8_01.cpp | SCRCharaInitBossWork | globals: 011e6248:W |
| 01388600 | effectGetDamageWork | recompiled | CreateDeathTentacle | $a0: +0x0:W4; globals: 011e5fa0:& 011e6248:R |
| 01388618 | getBloodEffectEnableFlag | recompiled | (table / pointer only) | globals: 01466534:& |
| 0138e558 | arrowWaterInit | native: decomp_liftd8_01.cpp | (table / pointer only) | $a0: +0x1c:R4 +0x20:R16; globals: 011e6650:&W 011e6658:W 011e665c:W 011e6660:W 011e6664:& |
| 0138e768 | arrowWaterMain | native: decomp_liftd7_07.cpp | (table / pointer only) | $a0: +0x1c:R4; globals: 011e6650:&R 011e6654:RW 011e665c:RW 011e6664:& 011e6665:& 01465650:& 014657e0:R |
| 0138eab0 | EnableArrowWater | native: decomp_liftd6_02.cpp | gamecore_sub_0136B710 | globals: 011e6650:&RW 011e6658:W 011e665c:W 011e6660:W 011e6664:& |
| 0138eaf8 | EndArrowWater | native: decomp_liftd8_01.cpp | gamecore_sub_0136B5F8 | globals: 011e6650:&W 011e6658:&RW 011e665c:W 011e6660:W 011e666c:& |
| 0138eb48 | getArrowWaterSize | native: decomp_liftd8_01.cpp | (table / pointer only) | globals: 001e0ed4:R |
| 0139e988 | inInNowLoadingEffect | native: decomp_liftd8_01.cpp | demo_op2 | globals: 01466710:R |
| 013fa5c8 | startGaugeMaxAddEffect | recompiled | addPlayerMaxGrip | globals: 011f85f0:&W 011f85f4:W 011f85f8:W 011f8600:W 014657c8:R |
| 013fa628 | startAttackEffect | recompiled | updatePowerWork | globals: 011f8608:&W 011f860c:W 011f8610:W 011f8614:W 011f8618:W 014657c8:R |
| 013fd908 | CancelSwordEffect | native: decomp_liftd7_08.cpp | gamecore_sub_013FF548, nico_playerActForseStandReset, selectWeaponBefore, selectWeaponNext, setIsWeapon, setSelectWeapon | globals: 01465900:& 01466c48:& 01466c4c:RW 01466c50:RW 01466c54:RW 01466c90:R 01466c94:R 01466d2c:W |
| 01464840 | coliJudgeNotWater | recompiled | (table / pointer only) |  |
| 01464aa0 | getEventScriptClass | native: decomp_liftd2_02.cpp | gamecore_sub_013F6080 | $a0: +0x8:R1 |
