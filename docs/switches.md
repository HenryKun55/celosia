# Runtime switches (SOTC_*): the definitive classification

Round F0, 2026-11-27, on main 3edd3a1. Inventory only: no build, no Vita3K run, no console run. The machine-readable copy is
`docs/switches.json` (`{name: {class, default_now, proposed_default, files, round, evidence, parent, role, console, ...}}`).
A follow-up round makes the code match this file. The goal: a release package that works with **no env.txt**.

**Round F1 (2026-11-27) made the code match it.** The 15 switches are on with no env (one kill switch each), the class-E
paths are deleted (34 names moved to F instead: see the E section), every class-D and class-B read goes through
`ps2_diag::env`, and the switch table agrees with the real defaults. Every entry of `docs/switches.json` has an `f1` field
(what F1 did) and its `default_now` is the state after F1. The no-env behaviour, the menu and the kill switches are in
`docs/release_config.md`.

## Method

- Names: `git grep -ohE '"SOTC_[A-Z0-9_]+"'` over `*.cpp *.h *.c` gives 1 156 names. 16 more are read at boot from env.txt by
  `bootenv` in `ps2_vita_runtime.cpp` with an `=` inside the literal (`"SOTC_K2="`), so the grep misses them: K2, K2REMAP, K3, C1CORE3,
  M1RAM, the K1 family and their knobs. Total: **1 172 names**.
- Default now: read from the read site (`e && e[0]=='1'`, `!(e && e[0]=='0')`, `e ? atoi(e) : N`), from the `sotc_decomp.h`
  switch table (171 entries, `switchOn`), from `native_tuning.h` (the Vita branch) and `defaultTuning()`, and from code comments
  ("default on", "=0: as before"). Overrides of the table were checked by hand (A12TYPED, S13MOT, SEG80, S17IOP, N5SCHED).
- Evidence: `docs/rounds.json`, the `docs/decomp_*.md` round docs, the `## Decomp X` sections of `docs/decomp.md`, `docs/ab_s20.md`,
  `docs/arch_plan.md`, and the console logs in `build/agent-scratch/console-logs/*` (env.txt and err.txt of each run).
- Parents: a name is a sub-knob of the longest prefix that is itself a switch (`SOTC_DT1_PRIO` under `SOTC_DT1`), with hand fixes
  (the A*AD adapters under their A*TYPED stage, the B round knobs under their B switch, the char104 family under CHAR104).
  A sub-knob takes its parent's class unless its name marks a log / check / profiler (then D).
- When the docs give no verdict, the switch is in F. Nothing was guessed into A.

## Totals

| class | meaning | all names | top-level switches | sub-knobs |
|---|---|---|---|---|
| A | proven / already default: on in release | 383 | 293 | 90 |
| B | player option (settings menu) | 38 | 35 | 3 |
| C | test only, off, never in packages | 23 | 2 | 21 |
| D | debug / profiling / check | 572 | 218 | 354 |
| E | dead or superseded: delete the code | 89 | 46 | 43 |
| F | unknown: evidence missing | 67 | 35 | 32 |
| | **total** | **1172** | **629** | **543** |

After round F1 (`switches.json`, 1 176 names: F1 found 4 debug names F0's grep missed, `FOG_NO_OVERLAY`, `NATIVE_ADDZ`,
`NATIVE_SHOWFOG` in the Metal renderer and `Y2_RAMCHECK` in `y2_dynamics.inc`):

| class | names after F1 | of them deleted from the code |
|---|---|---|
| A | 383 | 10 knobs (now constants: W1 / X3 / Y1 / Z1 / Z2 / Z3 parts, DT1 palette / pin / prio / wake) |
| B | 38 | 0 (ignored in a release build) |
| C | 23 | 0 |
| D | 576 | 27 (the checks / logs of deleted E switches) |
| E | 55 | 55 (all of them; 34 E names moved to F) |
| F | 101 | 0 |

Of the 293 top-level A switches, **15 change default** (off today, on in release) and 278 are on already (the switch only
brings back an older path, or sets a constant that already has its built-in value).

## Release config (no env file; done in round F1, see `docs/release_config.md`)

**On by code default since round F1** (the 15 switches that changed; `SOTC_<NAME>=0` is each one's kill switch):

`D1NATIVE`, `DT1`, `W1`, `W2`, `W3`, `X1`, `X2`, `X3`, `Y1`, `Y2`, `Y3`, `Z1`, `Z2`, `Z3`, `Z4`

Everything else that is on in the release is on already today: the 278 other A switches (the switch table's default-on
rounds, the renderer's default paths, the sound and thread paths, and the built-in constants), with their sub-knobs at their
built-in values. `SOTC_NATIVE` is set to 1 by `ps2_vita_runtime.cpp` at start.

**Player settings (B)**: what `settings.cfg` holds, else the menu default. Preset High (3), render scale 1.0, bloom on, all
effects on, body shadows on (Tuning default true: `SOTC_BODY_SHADOWS=1` in the console env files is redundant), alpha test on,
frame rate Auto, 30 fps cap off, the four performance cuts off, CPU at 500 MHz off. MSAA and effect detail act on Metal only.

**Off**: every C, D and F switch; the E paths are deleted. D is compiled out of a release build (round F1 routed every D
read through `ps2_diag::env`). The release env.txt is empty or absent.

Compared with `all/env-all-on.txt`: the release keeps W1-W3, X1-X3, Y1-Y3, Z1, Z2 and D1NATIVE, adds Z3, Z4 and DT1, and drops
K2, M1PMU, P2BRACKET, PROF129, FRAME_LOG / FRAME_SLOW_MS, AUDIO_LOG, LOG_STDERR, Z1_LOG (all D) and BODY_SHADOWS (already the
default). K3 (in `k3/env-k3-on.txt`) stays off: it is F until a console run proves its path (see below).

## Risks

1. **Most of the switches that change default never ran on the console.** Console-proven: D1NATIVE (A/B 2026-11-24, then the base
   env of every run), W1, W2, W3 (one run each, `run-20261126-wxon`). Never on the console: X1, X2, X3, Y1, Y2, Y3, Z1, Z2, Z3, Z4,
   DT1. The combined `all/celosia-all.vpk` package has no console log either. Their evidence is Vita3K checks (0 bad) only.
2. **Z3, Z4 and DT1 are not in `env-all-on.txt`.** Z3 and Z4 never ran together with the rest. DT1 ran with the full set only in
   K3's Vita3K boots (k3dRA). DT1 adds a worker pinned to core 1 at priority 140.
3. **Dependencies.** W2 and X1 need D1NATIVE. With D1NATIVE on, round 140's clip helper never runs (decomp_w1.md), so CLIP140 is
   dead code. D3's difftest of ride7 with the full D list stops in the save's load (unresolved, no body implicated).
4. **The switch table was not the truth (fixed in F1: A12TYPED is opt-in in the table too; S13MOT is deleted; SEG80's
   table "on" was right, see its F entry).** `sotc_decomp.h` lists A12TYPED and SEG80 as default-on, but A12TYPED is opt-in in
   `sotc_native.cpp` (A13 / A14 ride on it) and SEG80's reuse acts only with `=1` since round 115; S13MOT is default-off through
   `s13mot.h`. The follow-up round should use this file, not the table, as the list of defaults.
5. **K2 is a profiler, not a feature.** `SOTC_K2=1` walks the page tables on the first frame (60.8 ms on the console, wxon) and arms
   the PMU. CPU500 loads K2 lazily and K3 / C1CORE3 load it themselves, so the release needs no `SOTC_K2`.
6. **(Fixed in F1: every D read goes through `ps2_diag::env`; W1_LOG / X3_LOG / DT1_LOG off by default.)** **The release
   build compiled out fewer than half of the debug switches.** `PS2X_VITA_RELEASE=ON` turns `ps2_diag::env()` into
   `nullptr`, but only 242 of the 568 D names are read only through it; the rest use `std::getenv` and stay live. Two debug
   logs are on by default: `SOTC_W1_LOG` (a `[w1]` line every 600 walks) and `SOTC_X3_LOG` (an `[x3]` line every 600 walks). Both
   become release output once W1 / X3 are default. `compiled_out_by_release_build` in the JSON marks each D name.
7. **K3 has never executed.** Vita3K has no kernel module, so every K3 run took the fallback (`[k3] core 3 OFF ... 0 of 17`). It is F,
   not A. The same holds for C1CORE3, C1MAP, K2REMAP, M1RAM and CPU500's 500 MHz path (B, but unproven).
8. **Behavioural switches.** CHARLOD78 and OBJ96 (skip what nobody sees) are C by the user's decision; PTCL79 is the same kind
   and has no decision yet (F). None of them may enter a package.

## A: switches that change default (off now, on in release)

| switch | round | evidence | console | sub-knobs |
|---|---|---|---|---|
| D1NATIVE | D1-D3 | D1/D2/D3 bodies: difftest of each body vs its old body 0 differences on ride7 / attract / lake6 (D1: 39-43 natives a route; D2: only A/A noise bytes; D3 runs through). Console A/B run 2026-11-24 (run-20261124-d1, d1pre) and base env of every console run since (k2pmu, p3, wx). Note: clip140 never runs with it on (decomp_w1.md). | yes | debug: D1_LOG |
| DT1 | DT1 | Checks 0 bad / 0 void: ride7 303, lake6 208, altar 230, control 170 (decomp_dt1.md). Adds a core-1 worker thread (priority 140). NOT in env-all-on (only k3/env-k3-dt1). | no | knobs: PALETTE, PIN, PRIO, WAKE; debug: CHECK, LOG, VERIFY |
| W1 | W1 | W1_CHECK ride7 0 differences in 1.99 M locks, 1.39 M getters, 128 600 palettes; lake6 0 (decomp_w1.md). Console wxon run 2026-11-26 ([w1] lines, differ 0). | yes (run-20261126-wxon) | knobs: PARTS; debug: CHECK, LOG |
| W2 | W2 | W2_CHECK ride7 3.72 M nodes / 1.59 M bones 0 mismatches; GetMotionMatrix difftest 0 of 100 (decomp_w2.md). Console wxon run. Needs D1NATIVE. | yes (run-20261126-wxon) | debug: CHECK |
| W3 | W3 | W3_CHECK 0 of 815 805 differ (decomp_w3.md). Console wxon run ([w3] on). | yes (run-20261126-wxon) | debug: CHECK, LOG |
| X1 | X1 | X1_CHECK ride7 body 38 613 calls / xz 4 956 calls 0 mismatches (decomp_x1.md). In all/env-all-on.txt. Needs D1NATIVE. | no | debug: CHECK |
| X2 | X2 | X2_CHECK ride7 662 856 / lake6 4 073 checks, 0 differ (decomp_x2.md). In env-all-on. | no | debug: CHECK, LOG, RANK, ZONE |
| X3 | X3 | HOOK_ASYNC_VERIFY 2.87 M jobs 0 differ; jobs, contents and order unchanged (decomp_x3.md). In env-all-on. | no | knobs: PARTS; debug: BRACKET, CHECK, LOG |
| Y1 | Y1 | Y1_CHECK ride7 192/256/257, lake6 144/149/151 checks, 0 bad (decomp_y1.md). In env-all-on. | no | knobs: PARTS; debug: CHECK, LOG |
| Y2 | Y2 | Y2_CHECK step 650 104 / strands 59 708 / velocity 174 988 compared, 0 differ; 1 729 RAM scans 0 pages outside (decomp_y2.md). In env-all-on. | no | debug: CHECK, LOG |
| Y3 | Y3 | Y3_CHECK ride7 564 659 samples / 301 159 light kicks / 225 084 object kicks, lake6 0 differ (decomp_y3.md). In env-all-on. | no | debug: BRACKET, CHECK, LOG |
| Z1 | Z1 | Z1_CHECK ride7 OneObj 508 / MultiDep 26, lake6 255/145/58: 0 bad (decomp_z1.md). In env-all-on. | no | knobs: PARTS; debug: CHECK, LOG |
| Z2 | Z2 | Z2_CHECK three 80 / head 80 / pass 80 / keep 556 / type-1 635, registers 81 928, 0 differ (decomp_z2.md; found and fixed a real draft bug). In env-all-on. | no | knobs: PARTS; debug: CHECK, LOG |
| Z3 | Z3 | Z3_CHECK ride7 and lake6 0 bad, 0 void (decomp_z3.md). NOT in env-all-on (round after it). | no | knobs: PARTS; debug: CHECK, LOG |
| Z4 | Z4 | Z4_CHECK ride7 95 lines / 504 passes, lake6 58 lines: 0 differ (decomp_z4.md). NOT in env-all-on. | no | debug: BRACKET, CHECK, LOG |

## A: already default (keep on; the off path can go)

These are on today. Their switch only turns an older path back on, or overrides a built-in constant. The follow-up round can
delete the switch and the old path, or keep the constant without the getenv. Sub-knobs keep their built-in values.

**Switch table (sotc_decomp.h, decomp / redesign / native rounds)** (123):

`A10TYPED`, `A11TYPED`, `A1TYPED`, `A2TYPED`, `A3TYPED`, `A4TYPED`, `A5TYPED`, `A6TYPED`, `A7TYPED`, `A8TYPED`, `A9CMP`, `A9SWEEP`, `A9TYPED`, `AI106`, `BLEND123`, `CAMCOLL`, `CLIP134`, `CLOTH122`, `COLL115`, `COLL63`, `COLL71`, `COLLPROBE`, `DC1MOT`, `DRAW117`, `DRAW95`, `EULER136`, `FASTNATIVE`, `FASTNATIVE2`, `FASTNATIVE3`, `FIN125`, `FIN138`, `GC139`, `GEO126`, `GLV117`, `GROUND85`, `GRP106`, `HEAP120`, `HOTLIST`, `HOTLIST44`, `HOTLIST46`, `HOTLIST47`, `HOTLIST48`, `IMM128`, `ISYS`, `JOIN95`, `KEY136`, `KEYS122`, `LEAF101`, `LEAF117`, `LEAVES`, `LIFTALT`, `LIFTD1`, `LIFTD10`, `LIFTD11`, `LIFTD12`, `LIFTD13`, `LIFTD14`, `LIFTD15`, `LIFTD16`, `LIFTD17`, `LIFTD18`, `LIFTD2`, `LIFTD20`, `LIFTD3`, `LIFTD4`, `LIFTD5`, `LIFTD6`, `LIFTD7`, `LIFTD8`, `LIFTD9`, `LINKS97`, `MOTION`, `N2SND`, `N3DIRECT`, `N4NATIVE`, `N5SCHED`, `N6NOCHK`, `OBJ101`, `OBJ125`, `PATH97`, `PF134`, `PF135`, `PF141`, `PFC141`, `PKT117`, `PLAN97`, `POSE82`, `PREF69`, `PROBE84`, `PTCL103`, `PTCL123`, `PTCL62`, `PTCL83`, `PTCL89`, `QUAD140`, `QUAT122`, `R1SORT`, `RND139`, `S10DRAW`, `S11RT`, `S17IOP`, `S1PTCL`, `S21LIGHT`, `S2COLL`, `S3CHARA`, `S4DISP`, `S5PTCL`, `S6COLL`, `S9RT`, `SCRIPTVM`, `SCROLL127`, `SEL96`, `SEL97`, `SKIN101`, `SOLID103`, `SORT140`, `SUB139`, `SW102`, `SYM120`, `TEX64`, `WALK101`, `WALK67`, `WALK83`

**Renderer (native GL / tuning)** (67):

`ASYNC_BUILDS`, `BG_CLEAR_FOG`, `BG_DEPTH`, `BG_FGE`, `BLOOM_GS_VITA`, `BLOOM_SAMEFRAME`, `BLOOM_TAPS`, `CD_AHEAD100`, `GLOW_PLAIN_FADE`, `GL_AT98`, `GL_ATOP93`, `GL_AT_MERGE`, `GL_BGCOPY94`, `GL_BGSMALL87`, `GL_BLOOM94`, `GL_BLUR93`, `GL_DEPTH_KEEP`, `GL_FAR_GUARD`, `GL_FAT116`, `GL_FEW_COPIES`, `GL_FEW_SCENES`, `GL_FX94`, `GL_GLOW94`, `GL_GLOW98`, `GL_GLOW_PAIR`, `GL_KEEP_LAYOUT`, `GL_KEEP_TARGETS`, `GL_LEAN`, `GL_LOD0_98`, `GL_MERGE`, `GL_MIRROR88`, `GL_MIRROR93`, `GL_NODISCARD`, `GL_NOFOG98`, `GL_NOSCREEN92`, `GL_OPAQUE88`, `GL_OWN_MIPS`, `GL_PLAIN_READS`, `GL_POOLS`, `GL_POOL_COMPACT`, `GL_POST93`, `GL_PRESENT86`, `GL_PRESSURE_POOLS`, `GL_PSOFT98`, `GL_SCRATCH_DEPTH`, `GL_SOFT_CUT`, `GL_SWIZZLE`, `GL_UNTEX98`, `GL_VOL87`, `GL_VOLUME_PROG`, `GL_WARM`, `GPU_SKIN`, `HOLD_INCOMPLETE`, `HOOK_ASYNC`, `NATIVE`, `NATIVE_R2`, `PACKET_QUICK`, `PRIO100`, `RENDER105`, `RENDER_THREAD`, `SBLUR100`, `SBLUR_JOIN`, `SBLUR_PINGPONG`, `SHADOW_INDEXED`, `SHADOW_SOFT_VITA`, `VGL_DEPTH_TILED`, `VGL_UNIFORM_GUARD`

**Runtime, threads, sound, memory** (53):

`2D_TEX_ASYNC`, `ALPHA118`, `BOSSMASK`, `BUILD105`, `DECOMP`, `DMA_ASYNC`, `FAST_HASH`, `FMT102`, `GC_BUDGET`, `GLEND_ASYNC`, `GSREG_ASYNC`, `HOOK_LEAN`, `HOOK_PREFETCH`, `HOOK_SEMA`, `IDLE_TURN`, `KICK_DEFER`, `LAYER_MIRROR`, `LEAN`, `LIFEBAR_SHORTCUT`, `LIFT_SERVE`, `LOD_SIGNED`, `LOG105`, `LOG76`, `LOG_ASYNC`, `MDS_LANE`, `MESHDISP_VIEW`, `MIX90_CORE`, `MIX90_REVFLUSH`, `MIX90_VOICE`, `MOVE_MEMO`, `MOVE_REKEY`, `N2SLOT`, `OBJ_PREFETCH`, `PACE110`, `PACE132`, `PACE143`, `PLAN`, `RPC139`, `RUNDMA112`, `RUNSRC118`, `S7WIND`, `SEM139`, `SEMA_FAST`, `SG2Q139`, `SHADER_FAST`, `SKELETON_FAST`, `SKIN_AT_KICK`, `SURFACE_MEMO`, `SWZ118`, `TEX_QUICK`, `THREADS`, `WORKER105`, `WORKER91`

**Constants (built-in values)** (35):

`ASYNC_BUDGET` (value kDefault), `BG_NEAR` (value -1), `FOG_BASE_SCALE` (value 1.0), `GL_BUDGET_MS` (value 20000), `GL_CACHE_MB` (value 96.0), `GL_HEAP_MB` (value 104.0), `GL_HEAP_RESERVE_MB` (value 12.0), `GL_POOL_LOW_MB` (value 12.0), `GL_PRESSURE_MS` (value 8000), `GL_RUN_FRAMES` (value kRunFrames), `GL_SCALE_GUARD_MB` (value 8), `GL_UPLOAD_FLUSH_MB` (value 8.0), `HOLD_FRAMES` (value 3), `HOLD_TERRAIN_MS` (value 1500), `HOOK_WORKER_PRIO` (value 150), `JOB_KEEP_KB` (value 16), `LANE_HELP` (value 3), `MIX90_CHUNK` (value 32), `N5_SPIN` (value 0), `REFLECT_ALPHA` (value 1.0), `REFRACT_ALPHA` (value 1.0), `RUNBUDGET118_KB` (value kRunBudget0), `RUNCOPY112_KB` (value 1024), `SHADOW_AMOUNT` (value 0.5), `SHADOW_BLUR` (value 1), `TAKEUP_BUDGET` (value 0), `TAKEUP_MS` (value 8), `TERRAIN_BUILD_US` (value 2500), `THREAD_CORES` (built-in per-thread table), `THREAD_PRIO` (built-in per-thread table), `VGL_CIRCULAR_KB` (value 3072), `VGL_IMMEDIATE_KB` (value 64), `VGL_PARAM_KB` (value 16384), `VGL_UNIFORM_KB` (value 2048), `VGL_UNIFORM_MAX_KB` (value 8192)

## B: player options

All already exist as menu items (the Graphics page of the native renderer's tuning list, saved in settings; CPU500 in the System
tab of `host_menu.cpp`). The env variable only overrides the saved value for one run; the follow-up round can keep that for tests
in debug builds and ignore it in release.

| switch | feature default today | menu item / evidence | sub-knobs |
|---|---|---|---|
| ALPHA_TEST | feature: on | Graphics page "alpha test" (default on) (native_renderer.cpp tuning items, saved in settings). |  |
| BLOOM_ROUNDS | feature: 1 | Graphics page (native_renderer.cpp tuning items, saved in settings). |  |
| BLOOM_SCALE | feature: bloom on, strength 1.0 | Graphics page bloom on/strength (native_renderer.cpp tuning items, saved in settings). |  |
| BLOOM_SPREAD | feature: 1 | Graphics page (native_renderer.cpp tuning items, saved in settings). |  |
| BODY_SHADOWS | feature: on | Graphics page "body shadows"; Tuning default true, so SOTC_BODY_SHADOWS=1 in the console env files is redundant (native_renderer.cpp tuning items, saved in settings). |  |
| CAP20 | feature: Auto (off) | Graphics page "Frame rate" (Auto / Steady 20); default Auto (native_renderer.cpp tuning items, saved in settings). | knobs: QUEUE; debug: LOG, TRACE |
| CAP30 | feature: off | Graphics page "30 fps cap"; default off (round 64, user) (native_renderer.cpp tuning items, saved in settings). | knobs: ALWAYS |
| CPU500 | feature: off | Already a saved System-tab item (host_menu.cpp); SOTC_CPU500 only overrides it for one run. Vita3K: CPU500 REFUSED fallback only; console A/B pending (decomp_a3cpu.md). |  |
| EFFECT_DETAIL | feature: 0 (Metal only) | Graphics page; Metal only (native_renderer.cpp tuning items, saved in settings). |  |
| FOG_AMOUNT | feature: 1.0 | Graphics page (native_renderer.cpp tuning items, saved in settings). |  |
| GLOW_VITA | feature: 1 | Graphics page glow mode (group 3); default 1 (native_renderer.cpp tuning items, saved in settings). |  |
| MODEL_DETAIL | feature: 0 | Graphics page; default 0 (native_renderer.cpp tuning items, saved in settings). |  |
| MSAA | feature: 0 (Metal only) | Graphics page; Metal only, nothing on the Vita (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NODOF | feature: depth of field on | Graphics page "depth of field" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOFEEDBACK | feature: feedback on | Graphics page "feedback" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOGLOW | feature: glow on | Graphics page "glow" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOHAZE | feature: heat haze on | Graphics page "heat haze" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOPTCL | feature: particles on | Graphics page "particles" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOREFLECT | feature: reflections on | Graphics page "reflections" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOREFRACT | feature: refraction on | Graphics page "refraction" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOSBLUR | feature: screen blur on | Graphics page "screen blur" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOSHADOWS | feature: shadows on | Graphics page "shadows" (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_NOVFOG | feature: vertex fog on | Graphics page "vertex fog" (env turns it off) (native_renderer.cpp tuning items, saved in settings). |  |
| NATIVE_ZFOG | feature: depth fog on | Graphics page "depth fog" (native_renderer.cpp tuning items, saved in settings). |  |
| PRESET | feature: 3 (High) | Graphics page preset (default High = 3) (native_renderer.cpp tuning items, saved in settings). |  |
| RENDER_SCALE | feature: 1.0 | Graphics page; Vita 1.0 (native_renderer.cpp tuning items, saved in settings). |  |
| SACRIFICES | feature: unset (cuts as saved) | SOTC_SACRIFICES=0 clears every cut at start (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_CAMCOLL | feature: off | Graphics page performance cut, default off (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_DRAWDIST | feature: off | Graphics page performance cut, default off (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_FARCHARA | feature: off | Graphics page performance cut, default off (native_renderer.cpp tuning items, saved in settings). | knobs: DIST |
| SAC_FX | feature: off | Graphics page performance cut, default off (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_LOD | feature: off | Graphics page sacrifice (group 7) (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_MAX | feature: action (no state) | "Max performance" action (all four cuts) (native_renderer.cpp tuning items, saved in settings). |  |
| SAC_PARTICLES | feature: off (applyPreset does not set it; the round 36 comment saying High has it is stale) | Graphics page sacrifice (group 6) (native_renderer.cpp tuning items, saved in settings). |  |
| TEXTURE_FILTER | feature: 2 (trilinear) | Graphics page; default 2 trilinear (native_renderer.cpp tuning items, saved in settings). |  |

## C: test only (off, never in packages)

| switch | round | evidence | sub-knobs |
|---|---|---|---|
| CHARLOD78 | r78 / A2-lod | Characters nobody sees skip geometry updates. User decision: test only. A2-lod made it safe in Vita3K (stale 0) but Avion intro unproven (decomp_a2lod.md). | knobs: CAM_NEAR, CUT_COS, CUT_MOVE, FAR, GUARD, HOLD, K, LEGACY, MARGIN, MAXR, MINVIS, NEAR, OFF; debug: AUDIT_R, LOG |
| OBJ96 | r96 | Small creatures the camera cannot see run their frame less often ("skip unseen animals"). 2026-11-07 console: left Avion frozen at the screen edges; ~1 ms (sotc_decomp.h comment). Behavioural. | knobs: MAXR, NEAR, PERIOD; debug: LOG |

## E: dead or superseded (delete the code)

Round F1: **deleted** (code path and switch): BLOOM_OLD, C2SCENE (its scene stays, part of W1; C2SCENE_PF a constant),
CLIP140, EARLY95, FIND84, FLUSH119, GAZE122, GL_ARENA_CLIB, GL_FLUSH_WAIT, GL_INFLIGHT100, GL_INFLIGHT87, GL_NO_STENCIL,
GL_PRESSURE_POOLFREE, GL_RUN_EVICT_OLD, GL_SHADOWMASK_VITA, GL_SHMASK87, GL_SORT87, IDLE_SPIN, MOVE_WALK, NATIVE_BGFOG,
OAN106, PART119, POSE136, PS2_LISTS, PTCL91, PTCL_DEPTH, S12DRAW (s12_draw.h), S16PLAN (s16plan.h), S19AMB, S22PTCL,
S23LIGHT, SEG123 (coll123.h), SHADOW_HARD, SYSTEM_EMULATED, TEXR128, VIEW119, WALK119, ZFOG_JOIN and their knobs.
**Switch deleted, code compiled out** (its `on()` a constant false; the bodies are left for a cleanup round because they
are woven through live functions): S13MOT, S14HOOK, CTL126 (its two wrappers deleted), S8CLOTH and their knobs.
**Moved to F** (live, not dead):

- **K1 family** (K1, K1PROBE, K1_EVENTS, K1_FORCE, K1_PATH, K1RAM, K1RAM_PICK, K1RAM_TYPES): the `k1` namespace is K2's
  shared boot code (K2's module path from `k1::path()` / SOTC_K1_PATH, the PMU events from SOTC_K1_EVENTS for K2 and M1PMU,
  `k1pmu` / `k1probe` in K2's first frame). Deleting K1's own load means rewriting the profilers' boot path, which no
  Vita3K run can execute (no kernel modules). Off with no env.
- **SEG80, SEG80_AGE**: live by default. `segMemo` still serves round 84's AI path tests (probe84's path callers) under
  coll115; only the reuse for every other caller needs SOTC_SEG80=1. The switch table's "on" is right.
- **CHAR104 and its 23 knobs**: on by default on the Mac runner (`kChar104Default` is 1 off the Vita, 0 on the Vita).

The table below is F0's record.

| switch | default now | round | why | sub-knobs |
|---|---|---|---|---|
| BLOOM_OLD | off |  | Old GL bloom; the GS chain (as Metal) replaced it (native_renderer_gl.cpp). |  |
| C2SCENE | off (switch table) | C2 | W1 turns on C2 host scene list itself at prefetch level 2 (decomp_w1.md); the standalone switch is redundant once W1 is default. | knobs: PF; debug: CHECK, LOG |
| CHAR104 | 0 on the Vita (kChar104Default) | r104-r114 | Character geometry jobs on the worker. Default 0 on the Vita since round 111: the jobs lost 0.7 ms (route 6) and 3.5 on the real Vita (decomp_char104.cpp). | knobs: ANIME108, ANIME109, CALLS, JOIN, NEAR, RECOMP, CHAR111_MIN, CUR109, DELTA111, GAIN114, HOLD111, NAT108, NET114, OBJ109, OPEN109, SNAP108, SPR111; debug: CHECK, LOG, CHAR108_REF, CHAR111_DIFF |
| CLIP140 | on (switch table) | r140 | clip140::post declines whenever SOTC_D1NATIVE=1 (decomp_w1.md): dead once D1NATIVE is the default. | debug: CHECK, LOG |
| CTL126 | off (switch table) | r126 | Route 6 hangs after the shrine cutscene; console r129 Wander loaded under the ground. |  |
| EARLY95 | off (switch table) | r95 | Only acts with ptcl91 mode 1 (rejected). |  |
| FIND84 | off (switch table) | r84 | Measured +0.93 ms (slower) matched (decomp code comment); ab_s20.md: numbers not found. | knobs: AGE; debug: CHECK, EPS, LOG |
| FLUSH119 | off (switch table) | r119 | Exact, each call 2-9% faster but whole-frame A/B even to slower (r119all -0.2..+0.14). |  |
| GAZE122 | off (switch table) | r122 | Checks found calls where the new path turns neck/head nodes by the gaze (0.2-6 deg); never traced. |  |
| GL_ARENA_CLIB | unset (off) |  | "=1: glBufferData copy" (old way). |  |
| GL_FLUSH_WAIT | off |  | "=1: the earlier way" (native_renderer_gl.cpp). |  |
| GL_INFLIGHT100 | off (Tuning default) | r100 | Console 2026-11-07: off was 1.0 ms faster (native_tuning.h). |  |
| GL_INFLIGHT87 | off (Tuning default) | r87 | Console +3.7 ms a frame: stays off. |  |
| GL_NO_STENCIL | off |  | "=1: as before" (native_renderer_gl.cpp). |  |
| GL_PRESSURE_POOLFREE | unset (off) | r33 | "=1: round 33 condition" (old rule). |  |
| GL_RUN_EVICT_OLD | unset (off) |  | "=1: as before". |  |
| GL_SHADOWMASK_VITA | unset (off) |  | "=1: made and cleared, as before". |  |
| GL_SHMASK87 | off (Tuning default) | r87 | Console measured 0 ms: stays off (native_tuning.h). |  |
| GL_SORT87 | off (Tuning default) | r87 | Render thread 0.55-0.90 ms slower (native_tuning.h). |  |
| IDLE_SPIN | off (unset) |  | Set: the iosRecvMsg idle hook is not installed (old behaviour). |  |
| K1 | off | K1 | One-shot kernel module; its PMU counters vanish on the game thread (decomp_k2.md). Superseded by K2. | knobs: K1PROBE, EVENTS, FORCE, PATH |
| K1RAM | off | K1 | Kernel-allocated guest RAM via K1; K1 superseded by K2, M1RAM / K2REMAP cover the same idea; never verified on console. | knobs: PICK, TYPES |
| MOVE_WALK | off |  | Old walk, 13 ms in calibrated Vita3K; only with =1. |  |
| NATIVE_BGFOG | off |  | "=1: on every one" (old behaviour). |  |
| OAN106 | off (switch table) | r106 | Exact but no faster than the recompiled code (sampler 0.30% either way). | debug: CHECK |
| PART119 | off (switch table) | r119 | Exact, each call 2-9% faster but whole-frame A/B even to slower (r119all -0.2..+0.14). | debug: CHECK |
| POSE136 | off (switch table) | r136 / r128 | Exact but no gain in Vita3K (pose136 A/B -0.036 +- 0.325 ms; texr128 sampler cost up); "the console A/B decides" never run with a result (ab_s20.md). | debug: CHECK, LOG |
| PS2_LISTS | off (Tuning false) |  | "=1: built as before" (Tuning ps2Lists false). |  |
| PTCL91 | off (switch table) | r91 | Particle step on the worker; ab_s20.md: "off for the console sake" (+0.9..+1.9 ms). |  |
| PTCL_DEPTH | unset (off) |  | "=1: they write depth, as before". |  |
| S12DRAW | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | knobs: PARTS; debug: CHECK, LOG |
| S13MOT | off (s13mot.h overrides the table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | knobs: PARTS, STRETCH; debug: CHECK, LOG |
| S14HOOK | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | knobs: PARTS; debug: CHECK, LOG |
| S16PLAN | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | knobs: MOVES; debug: CHECK, LOG |
| S19AMB | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | debug: CHECK, LOG |
| S22PTCL | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | debug: CHECK, LOG |
| S23LIGHT | off (switch table) | S12-S23 | Exact (0 differences) but even in the Vita3K frame A/B (z < 2); decisions in display_s12/s14/s16, lights_s19/s23, ptcl_s22, motion_s13, ab_s20.md. | debug: CHECK, LOG |
| S8CLOTH | off | S8 | Decision: stays off (A/B z > 2 against, characters_s8.md); Y2 / Z2 replaced the cloth path. | knobs: GATEMASK, GEOFILTER, INSTALL, LEARN, SPIN_US; debug: CHECK, LOG |
| SEG123 | off (switch table) | r123 | Not exact by design (keeps answers up to 1 cm / 60 frames); Agro dropped out of the gallop on the console (r127). | knobs: AGE; debug: CHECK, EPS |
| SEG80 | off (reuse only with =1 since r115; table says on) | r80 / r115 | Table says on, but since round 115 seg80 reuse only acts with SOTC_SEG80=1 (decomp_collision.cpp seg80Asked); coll115 replaced it. | knobs: AGE; debug: EPS, LOG |
| SHADOW_HARD | off |  | Mac-side hard shadows as before (SHADOW_SOFT_VITA=0 is the Vita twin). |  |
| SYSTEM_EMULATED | off (unset) |  | Set: the native system-call hooks are not installed (the emulated path, as before). |  |
| TEXR128 | off (switch table) | r136 / r128 | Exact but no gain in Vita3K (pose136 A/B -0.036 +- 0.325 ms; texr128 sampler cost up); "the console A/B decides" never run with a result (ab_s20.md). | debug: CHECK, LOG |
| VIEW119 | off (switch table) | r119 | Exact, each call 2-9% faster but whole-frame A/B even to slower (r119all -0.2..+0.14). | debug: CHECK |
| WALK119 | off (switch table) | r119 | Exact, each call 2-9% faster but whole-frame A/B even to slower (r119all -0.2..+0.14). |  |
| ZFOG_JOIN | unset (off) | r32 | "=1: as before" (round 32). |  |

## F: unknown (what is missing to decide)

| switch | default now | round | evidence so far / missing | sub-knobs |
|---|---|---|---|---|
| A12TYPED | off (opt-in in sotc_native.cpp; table says on) | A12-A14 | Exact (A12 422 113 in-run checks 0). Off because of an intermittent ride7 freeze; A15 showed the freeze is older than the typed code (S10/S11 froze too) and 14 of 14 rides with a12+a13+a14 on had 0 freezes. Table says on, but A12TYPED is opt-in in sotc_native.cpp and A13/A14 ride on it. Missing: an explicit decision + console run. | knobs: A12AD, A12HOLD, A12MMPATHS, A12NCHK |
| A13TYPED | off (rides on a12typed) | A12-A14 | Exact (A12 422 113 in-run checks 0). Off because of an intermittent ride7 freeze; A15 showed the freeze is older than the typed code (S10/S11 froze too) and 14 of 14 rides with a12+a13+a14 on had 0 freezes. Table says on, but A12TYPED is opt-in in sotc_native.cpp and A13/A14 ride on it. Missing: an explicit decision + console run. | knobs: A13AD |
| A14TYPED | off (switch table) | A12-A14 | Exact (A12 422 113 in-run checks 0). Off because of an intermittent ride7 freeze; A15 showed the freeze is older than the typed code (S10/S11 froze too) and 14 of 14 rides with a12+a13+a14 on had 0 freezes. Table says on, but A12TYPED is opt-in in sotc_native.cpp and A13/A14 ride on it. Missing: an explicit decision + console run. | knobs: A14AD |
| A15_LOADCENTER | off | A15 | Opt-in fix for the ride7 freeze; never acted in 14 rides, cause and fix unproven. |  |
| B10SWITCH | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | debug: B10_CHECK, B10_LOG |
| B1SOUND | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B1_BATON, B1_BATON_US, B1_HOSTFN; debug: CHECK, B1_LOG, B1_WATCHDOG |
| B2LOAD | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | debug: CHECK, B2_COUNT, B2_LOG, B2_WATCHDOG |
| B3THREADS | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B3_MASK, B3_SYNC, B5_DEFER; debug: B3_CHECK, B3_COUNT, B3_LOG, B3_WATCHDOG, B5_DOG, B5_DUMP_AT, B5_DUMP_EVERY, B5_TRACE |
| B4FAST | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B4_MODE |
| B6SYNC | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B6_T12; debug: B6_CHECK, B6_LOG |
| B7IRQ | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B7_CP, B7_HANDLERS; debug: B7_CHECK, B7_LOG |
| B8SCHED | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | knobs: B8_REGATE; debug: B8_CHECK, B8_LOG |
| B9LOOP | off (switch table) | B1-B10 | Game threads on host carriers, interrupt handlers and scheduler in host form; each round reports its own *_CHECK, all off by default; chain dependencies (B10 needs B9, B8, B7 and the carriers); never on the console; not in env-all-on. Missing: a decision whether the carrier design ships, then a console run. | debug: B9_CHECK, B9_LOG |
| C1CORE3 | off | C1 | Core 3 for the game alone through K2 (isIllegalAffinity hook). Vita3K fallback only; console runs pending (decomp_c1.md). K3 reuses its hook. Missing: console run. | debug: EXPECT |
| C1KEYS | off (switch table) | C1 (console) | Exact by construction; memory-traffic change; console A/B pending (console_c1.md). P1 builds on C1 key rows. | knobs: KB, ROT; debug: CHECK, LOG |
| C1MAP | off (unset: today's map) | C1 | Helper threads re-mapped over cores 1-3; Vita3K fallback only; console pending. | debug: FORCE3 |
| C1RIG | off (switch table) | C1 (console) | Exact by construction; memory-traffic change; console A/B pending (console_c1.md). P1 builds on C1 key rows. | debug: CHECK |
| DC2CHARA | off (switch table) | DC2 | Exact (0 differences in 26 M calls) but ~0.1 ms slower in Vita3K (z -3); console A/B never run. | knobs: DC2_CALLBACK, DC2_RAMAA; debug: DC2_CHECK, DC2_LOG, DC2_RAMCHECK |
| EE_TRUST | off (Tuning trustFinite false) | r26 | Plain host floats for the lean code: not exact (1 ulp), off by default; no speed verdict recorded. Debug menu item. | debug: AB |
| FRAME_INTERVAL | unset |  | Releases a frame only n VBlanks after the last; recorder test knob? no doc verdict. |  |
| GL_G1 | off | G1 | Same GL calls (pixel-identical over 15 toggles); Vita3K speed unchanged; "console decides, expected within noise" (decomp_g1.md). |  |
| GL_GLOW93 | off (Tuning default) | r73 / r93 | Tuning default false on the Vita; no recorded verdict found. |  |
| GL_GLOW_HOIST | off (Tuning default) | r73 / r93 | Tuning default false on the Vita; no recorded verdict found. |  |
| K2REMAP | off | K2 | 1 MB section re-maps of guest RAM / text with verify + put-back. Console unverified. The wx console walk already shows guest RAM in 1 MB sections (47 of 47) and text 67 of 83: gain unclear. Missing: a console A/B. | debug: DRY |
| K3 | off | K3 | Light misc threads (audio, loader, log, main, watch) on core 3 through K2 C1 hook. Vita3K ran only the fallback ([k3] core 3 OFF, 0 of 17 moved): the real path has never executed anywhere. Missing: one console run with the [k3] line showing core 3 ON and no regressions; then A. | knobs: SET, WIDE; debug: EVERY, FIRST |
| M1ALLOC | off | M1 | Removes the scheduler checkpoint per-call deque; console A/B pending. |  |
| M1RAM | off | M1 | Guest RAM in its own 1 MB-aligned 33 MB block before the heap. Vita3K boots normal; console A/B pending (decomp_m1.md). Missing: console A/B. |  |
| P1CHARA | off (switch table) | P1-P4 | Exact (own *_CHECK) but memory-system changes Vita3K does not model: "the console A/B decides", never run (perf_p1-p4.md). | knobs: ROT; debug: CHECK, LOG |
| P2PTCL | off (switch table) | P1-P4 | Exact (own *_CHECK) but memory-system changes Vita3K does not model: "the console A/B decides", never run (perf_p1-p4.md). | knobs: LAYERS, PF, PFCODE; debug: CHECK, LOG, RAMCHECK |
| P3PAR | off (switch table) | P1-P4 | Exact (own *_CHECK) but memory-system changes Vita3K does not model: "the console A/B decides", never run (perf_p1-p4.md). | debug: CHECK, LOG, RAMCHECK |
| P4MOVE | off (switch table) | P1-P4 | Exact (own *_CHECK) but memory-system changes Vita3K does not model: "the console A/B decides", never run (perf_p1-p4.md). | debug: CHECK, LOG |
| PTCL79 | off (switch table) | r79 | Off-screen particle geodisps put off (world time conserved). Off since the 2026-11-07 Avion-reveal bisect; A2-lod left the ptcl79 half unproven. Behavioural like charlod78: needs a user decision (C?) or a console Avion run. | knobs: INVERT, LOCAL, MARGIN, OFF; debug: LOG |
| R1CHAR | off (switch table) | R1 | Difftest ride7 0 mismatches, but Vita3K has no cache model and no console run; estimate 0.05-0.2 ms. Not in env-all-on. Missing: console A/B. | debug: CENSUS, LOG |
| RENDER_PRIO | off (Tuning default) | r74 | Render thread priority 149 (A/B rprio74); default 0; no recorded verdict found. | knobs: VALUE |
| VGL_GC | off (unset) | G1 | Same GL calls (pixel-identical over 15 toggles); Vita3K speed unchanged; "console decides, expected within noise" (decomp_g1.md). |  |

## D: debug, profiling, checks

All off in release. They belong behind one debug build flag (`PS2X_VITA_RELEASE` / `SOTC_RELEASE`: route every one through
`ps2_diag::env`, or delete it). `*` = read only through `ps2_diag::env` today (already compiled out of a release build).
Sub-knobs that are checks / logs of a non-D switch are listed with their parent in the tables above and in the JSON.

**Checks, verifies, difftest** (26): `CENSUS`*, `CHAR122_CHECK`, `COLLISION_FASTCHECK`, `COLL_CENSUS`*, `DIFFTEST`, `DMACACHE_CHECK`*, `EXIDX_CHECK`, `FAT116_CHECK`*, `GL_PROG_CHECK`*, `GL_SELFCHECK`, `GL_TEX_VERIFY`*, `GSREG_CENSUS`*, `HOOK_ORDER_CHECK`*, `LIFEBAR_CHECK`*, `MATH_CHECK`*, `MODELDISP_CHECK`*, `R2_CHECK`*, `RACE_CHECK`*, `READ_CENSUS`*, `SCROLL_CHECK`*, `SHADOW_CHECK`*, `SPR_COPY_CHECK`*, `STALE_CHECK`*, `TEX_DECODE_CHECK`*, `VIEW_CHECK`*, `X_CHECK`*

**Logs and log routing** (42): `A1_LOG`, `AUDIO_LOG`, `BLOOM_LOG`, `CAM126_LOG`, `CHAR122_LOG`, `CHARGEO80_LOG`*, `CPU_LOG`*, `FEEDBACK_LOG`*, `FRAME_LOG`, `GLV116_LOG`*, `GL_ARENA_LOG`*, `GL_CACHE_LOG`*, `GL_DRAW_LOG`*, `GL_EVENT_LOG`*, `GL_LAYER_LOG`*, `GL_MEM_LOG`*, `GL_PHASE_LOG`*, `GL_SCENE_LOG`*, `GL_SPIKE_LOG`*, `GL_TEX_LOG`*, `GPU86_LOG`*, `HITCH112_LOG`*, `IK80_LOG`*, `LOG_2D_ALPHA`, `LOG_GSREG`, `LOG_HEADER`, `LOG_LAYER`*, `LOG_MALLOC`, `LOG_SG2`, `LOG_STDERR`, `LOG_SURFACE_REGS`, `MESHDISP_LOG`*, `NATIVE_LOG`*, `NATIVE_LOGMAT`*, `PASS87_LOG`*, `POS_LOG`*, `PTCL_DRAWLOG`, `SBLUR_LOG`*, `SHADOW_LOG`*, `SKIN_KICK_LOG`*, `SORT87_LOG`*, `WALK79_LOG`*

**Profilers, counters, brackets** (36): `ACOS_STATS`*, `CALLGUEST_STATS`*, `CALL_COUNT`, `COLLISION_STATS`*, `CTRLDEF_STATS`*, `CURVE_STATS`*, `DIAG_TIMERS`*, `FALLBACK_COUNT`, `FINDER_STATS`*, `GL_DIAG92_HALF`, `GL_DIAG92_PX`, `GL_DIAG92_TEX`, `GL_DIAG98`, `GL_PX98`, `GL_SPIKE_MS`, `GS_MEASURE_ROWS`, `HASH_STATS`*, `HEAP_PROF`, `HITCH112_MS`, `HOOK_STATS`*, `HOST_PROF`*, `K2`, `M1PMU`, `MOTIONPREDICT_STATS`*, `P2BRACKET`, `P2PMUSAMPLE`, `PROF129`, `PTCL_STATS`*, `REC_SAMPLER_PRIO`, `REC_SAMPLE_US`, `SAC_STATS`*, `SCRSAMP102`, `SEMA_STATS`*, `SKINFINDER_STATS`*, `STAT139`, `SUBSYS_PROF`

**Recorder, replay, input, test aids** (54): `A13_WATCH`, `A13_WSHELL`, `A15_STAGE`, `A1_EVERY`*, `A1_FROM`*, `A1_RECORD`*, `A1_REPORT`*, `AB`*, `EMULATOR`, `FIXED_STEP`, `FORCE_BOSSFAN`, `FORCE_FEEDBACK`, `FORCE_UNDERWATER`, `GPUREAD118_US_PER_CALL`, `GPUREAD118_US_PER_MB`, `INPUT_RECORD`, `INPUT_REPLAY`, `INPUT_STATE_EVERY`, `PRESENT86_SIM_MS`, `PRESENT86_SIM_VSYNC`, `QUALITY_CYCLE`, `REC_AT`, `REC_DIR`, `REC_FIX_BOSSMASK`, `REC_FIX_MESHDISP`, `REC_FIX_RADIUS`, `REC_FRAMES`, `REC_FULL_DENSE`, `REC_FULL_EVERY`, `REC_FULL_MB`, `REC_MAX_MB`, `REC_MAX_MIN`, `REC_MAX_S`, `REC_MODES`, `REC_OUT`, `REC_REPLAY`, `REC_ROUTE`, `REC_WARM`, `REPLAY_EXIT`, `REPLAY_FB`, `REPLAY_HOLD_MS`, `REPLAY_SHOTS`, `REPLAY_SHOT_TICKS`, `REPLAY_TRACE`*, `REPLAY_WATCH`, `RIDE_FLAG`, `RIDE_PROBE`, `RIDE_STEER`, `SAC_AB`*, `SCALE_CYCLE`, `TELEPORT`, `TEST_SUSPEND`, `VITA_EAT_USER_MB`, `WAIT143_RENDER_US`*

**Renderer debug views** (43): `BLOOM_DUMP`*, `BLOOM_RECT`, `BLOOM_SHOW`, `BLOOM_VITA`, `DRAW_DUMP_DIR`*, `DRAW_DUMP_FRAMES`, `DUMP_SPRITE_PACKETS`, `GLOW_SHOW`*, `GL_DRAWSEQ`, `GL_DRAW_SKIP`, `GL_DYN_TINT`, `GL_FREEZE`, `GL_NOARENA`, `GL_REDRAW`, `GL_SHOTS`, `GL_SHOT_DIR`, `GL_SKIN_PRECISE`, `GL_UNINDEX`, `HAZE_SOURCE`, `HIDE_HEADER`, `HIDE_LAYERS`, `LINES67`, `MODEL_DUMP`*, `NATIVE_GL`, `NATIVE_IDCOLOR`, `NATIVE_NOGL`, `NATIVE_NOMIPS`, `NATIVE_NOROW`, `NATIVE_SKIP`, `NATIVE_SKIPLAYERS`, `NATIVE_SLOWHOOKS`, `RAW_MIRROR`, `REFRACT_SHOW`, `SBLUR_IDENTITY`, `SHADOW_ALLSIDES`, `SHADOW_FLIP`, `SHADOW_SHOW`, `SKIP_ZFOG`, `STRIPS_DEBUG`, `TEXDUMP_DIR`, `VGL_MEMDUMP`, `VGL_POOLS`, `WATER_SOURCE`

**Other** (17): `CK_EVERY`, `DECOMP_SKIP`, `FRAME_SLOW_MS`, `FRAME_STALL_FROM`, `FRAME_STALL_MS`, `GCOV_DUMP_FRAME`, `GPU86_EVERY`, `HOOK_WORKER_DELAY_US`, `MDS_TRACE`*, `PERF_OVERLAY`, `REST81_NAMES`, `REST81_PROBE`, `SPR_COPY_CHUNKED`, `SYSTEM_SKIP`, `TRACE_CALLS`, `TRACE_EVERY`, `X3_RING`

## Sub-knobs by parent

Every sub-knob is in the JSON with `role: sub-knob` and its `parent`. The parents with knobs that are not checks / logs:

| parent | class | knobs (non-debug) |
|---|---|---|
| A10TYPED | A | A10AD, A10HOLD |
| A11TYPED | A | A11AD |
| A12TYPED | F | A12AD, A12HOLD, A12MMPATHS, A12NCHK |
| A13TYPED | F | A13AD |
| A14TYPED | F | A14AD |
| A6TYPED | A | A6AD |
| A7TYPED | A | A7AD |
| A8TYPED | A | A8AD |
| A9TYPED | A | A9AD |
| B1SOUND | F | B1_BATON, B1_BATON_US, B1_HOSTFN |
| B3THREADS | F | B3_MASK, B3_SYNC, B5_DEFER |
| B4FAST | F | B4_MODE |
| B6SYNC | F | B6_T12 |
| B7IRQ | F | B7_CP, B7_HANDLERS |
| B8SCHED | F | B8_REGATE |
| BG_DEPTH | A | BG_DEPTH_RESET |
| C1KEYS | F | C1KEYS_KB, C1KEYS_ROT |
| C2SCENE | E | C2SCENE_PF |
| CAP20 | B | CAP20_QUEUE |
| CAP30 | B | CAP30_ALWAYS |
| CD_AHEAD100 | A | CD_AHEAD100_KB |
| CHAR104 | E | ANIME108, ANIME109, CHAR104_CALLS, CHAR104_JOIN, CHAR104_NEAR, CHAR104_RECOMP, CHAR111_MIN, CUR109, DELTA111, GAIN114, HOLD111, NAT108, NET114, OBJ109, OPEN109, SNAP108, SPR111 |
| CHARLOD78 | C | CHARLOD78_CAM_NEAR, CHARLOD78_CUT_COS, CHARLOD78_CUT_MOVE, CHARLOD78_FAR, CHARLOD78_GUARD, CHARLOD78_HOLD, CHARLOD78_K, CHARLOD78_LEGACY, CHARLOD78_MARGIN, CHARLOD78_MAXR, CHARLOD78_MINVIS, CHARLOD78_NEAR, CHARLOD78_OFF |
| CHARLOD78_FAR | C | CHARLOD78_FAR_S |
| CHARLOD78_OFF | C | CHARLOD78_OFF_BIG |
| COLL115 | A | COLL115_MB |
| DC2CHARA | F | DC2_CALLBACK, DC2_RAMAA |
| DMA_ASYNC | A | DMA_ASYNC_DELAY_US |
| DRAW95 | A | DRAW95_ORDER |
| DT1 | A | DT1_PALETTE, DT1_PIN, DT1_PRIO, DT1_WAKE |
| FIND84 | E | FIND84_AGE |
| GAIN114 | E | GAIN114_PROBE, GAIN114_US |
| GC_BUDGET | A | GC_MAX_KB, GC_MIN_KB |
| GL_GLOW94 | A | GL_GLOW94_UP |
| GL_LEAN | A | GL_LEAN_DRAW, GL_LEAN_PASS |
| GL_MERGE | A | GL_MERGE_MODE |
| GL_SWIZZLE | A | GL_SWIZZLE_BOTH |
| GL_WARM | A | GL_WARM_TARGETS |
| GROUND85 | A | GROUND85_AGE, GROUND85_COOL, GROUND85_MARGIN |
| HOLD111 | E | HOLD111_FRAMES |
| K1 | E | K1PROBE, K1_EVENTS, K1_FORCE, K1_PATH |
| K1RAM | E | K1RAM_PICK, K1RAM_TYPES |
| K3 | F | K3_SET, K3_WIDE |
| LEAN | A | LEAN_PERIOD |
| LIFTD10 | A | LIFTD10_RANGE |
| LIFTD11 | A | LIFTD11_RANGE |
| LIFTD12 | A | LIFTD12_RANGE |
| LIFTD13 | A | LIFTD13_RANGE |
| LIFTD14 | A | LIFTD14_RANGE |
| LIFTD15 | A | LIFTD15_RANGE |
| LIFTD16 | A | LIFTD16_RANGE |
| LIFTD17 | A | LIFTD17_RANGE |
| LIFTD18 | A | LIFTD18_RANGE |
| LIFTD2 | A | LIFTD2_RANGE |
| LIFTD20 | A | LIFTD20_RANGE |
| LIFTD3 | A | LIFTD3_RANGE |
| LIFTD4 | A | LIFTD4_RANGE |
| LIFTD5 | A | LIFTD5_RANGE |
| LIFTD6 | A | LIFTD6_RANGE |
| LIFTD7 | A | LIFTD7_RANGE |
| LIFTD8 | A | LIFTD8_RANGE |
| LIFTD9 | A | LIFTD9_RANGE |
| MDS_LANE | A | MDS_LANE_DELAY_US |
| MIX90_REVFLUSH | A | MIX90_REVFLUSH_MAX |
| NET114 | E | NET114_US, NET114_WIN |
| OBJ109 | E | OBJ109_R |
| OBJ96 | C | OBJ96_MAXR, OBJ96_NEAR, OBJ96_PERIOD |
| OBJ96_MAXR | C | OBJ96_MAXR_BIG |
| OBJ96_NEAR | C | OBJ96_NEAR_SMALL |
| OBJ96_PERIOD | C | OBJ96_PERIOD_BIG |
| P1CHARA | F | P1CHARA_ROT |
| P2PTCL | F | P2PTCL_LAYERS, P2PTCL_PF, P2PTCL_PFCODE |
| PACE110 | A | PACE110_CALM, PACE110_HYST |
| PF141 | A | PF141_AHEAD |
| PLAN | A | PLAN_BUDGET, PLAN_RUNS |
| PRIO100 | A | PRIO100_VALUE |
| PROBE84 | A | PROBE84_AGE, PROBE84_WATER_AGE |
| PTCL79 | F | PTCL79_INVERT, PTCL79_LOCAL, PTCL79_MARGIN, PTCL79_OFF |
| PTCL83 | A | PTCL83_FAR |
| RENDER_PRIO | F | RENDER_PRIO_VALUE |
| RUNSRC118 | A | RUNSRC118_MB |
| S10DRAW | A | S10DRAW_PARTS |
| S11RT | A | S11RT_IOPBATCH, S11RT_PACKED, S11RT_PARTS |
| S12DRAW | E | S12DRAW_PARTS |
| S13MOT | E | S13MOT_PARTS, S13MOT_STRETCH |
| S14HOOK | E | S14HOOK_PARTS |
| S16PLAN | E | S16PLAN_MOVES |
| S17IOP | A | S17IOP_PARTS |
| S2COLL | A | S2COLL_VIEWS, S2COLL_WATER |
| S3CHARA | A | S3CHARA_JOBS, S3CHARA_JOB_MIN, S3CHARA_KEEP, S3CHARA_KEYS_MIN, S3CHARA_SPIN_US |
| S4DISP | A | S4DISP_HOOKS, S4DISP_PALETTE, S4DISP_PASSES |
| S5PTCL | A | S5PTCL_DRAW, S5PTCL_THREAD |
| S6COLL | A | S6COLL_THREAD |
| S8CLOTH | E | S8CLOTH_GATEMASK, S8CLOTH_GEOFILTER, S8CLOTH_INSTALL, S8CLOTH_LEARN, S8CLOTH_SPIN_US |
| S9RT | A | S9RT_HOOKTIME, S9RT_PARTS |
| SAC_FARCHARA | B | SAC_FARCHARA_DIST |
| SEG123 | E | SEG123_AGE |
| SEG80 | E | SEG80_AGE |
| SG2Q139 | A | SG2Q139_ALWAYS |
| W1 | A | W1_PARTS |
| WALK67 | A | WALK67_AHEAD |
| WALK83 | A | WALK83_CLIP, WALK83_LIGHT, WALK83_REFRESH, WALK83_TOL |
| X3 | A | X3_PARTS |
| Y1 | A | Y1_PARTS |
| Z1 | A | Z1_PARTS |
| Z2 | A | Z2_PARTS |
| Z3 | A | Z3_PARTS |

## What the follow-up round should do (round F1 did 1-4 and 6; see the top of this file)

1. Flip the 15 A switches above to on by default (or remove their switch), each with its sub-knobs as constants.
2. Delete the E code paths and their switches.
3. Route every D switch through `ps2_diag::env` (or delete it) so a release build has none; turn `W1_LOG` / `X3_LOG` off by default.
4. Keep B as menu items; in release ignore their env variables.
5. Leave C and F off. Move each F to A, C or E when its missing evidence (console run, user decision) arrives.
6. Fix the switch table so its defaults match the real ones (A12TYPED, SEG80, S13MOT).

Left after F1: the compiled-out bodies of S13MOT / S14HOOK / CTL126 / S8CLOTH; decisions for the 34 E names now in F; the
278 already-default A switches still read their `SOTC_<NAME>=0` (escape hatches); the 15 kill switches can go once the
console has run the F1 package.

