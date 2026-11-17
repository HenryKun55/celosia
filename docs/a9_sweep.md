# Round A9 sweep: A9 codegen costs in hot code (SOTC_A9SWEEP, A/B a9sweep)

A codebase-wide sweep for code the Vita's GCC (arm-vita-eabi 15, `-O3 -mfpu=neon`) compiles into something expensive on the
Cortex-A9, ranked with the Vita3K sampler and fixed without a change of behaviour. Everything is behind `SOTC_A9SWEEP`
(default on; A/B name `a9sweep`); the off state runs the code as it was.

Scripts, runs and logs: `build/agent-scratch/a9s/` (STATE.md). Own Vita3K trees `../vita3k-celosia-a9s` / `-a9sb` (222 MHz,
3 cores, muted).

**Result:** frame time -0.30 / -0.44 ms on the attract demo and -0.71 / -0.75 ms at the lake (seeds 7 / 11, z 5.5-11.4),
the game thread 0.31-0.73 ms a frame less busy; memcmp and the scheduler parts measured on their own (z 4.0-5.6), the
hoisted statics kept on exactness (Vita3K cannot price their barrier); every check 0 differences.

## 1. How the costs were found

- **a9rank.py**: the sampler's top 400 game-thread symbols (attract, lake, ride, mean share) and, from each one's
  disassembly, its `dmb`, `vmrs`, `ldrex` counts and its calls to library helpers (`__aeabi_*div*`, `__cxa_guard_*`,
  `__emutls_get_address`, `memcmp`, `fmaxf` ...).
- **guards.py**: the guard variables a function tests (`movw/movt` of the guard, `ldr`, `dmb ish`), by name.
- **callers.py / pcrange.py**: the samples inside a helper grouped by the sample's LR (the helper's caller): the only way
  to attribute a library helper's time, since the helpers are shared.
- **divsites.py / neonscan.py**: each division helper call with its divisor; scalarised NEON compare masks.
- **The sampler counts guest instructions** (PCs at block granularity, a weight of 20 000 instructions a sample), and
  Vita3K's time is instruction-count based: a `dmb ish` costs one instruction there. The A9's real barrier cost (a store
  buffer drain, tens of cycles) is invisible in Vita3K; only fewer instructions show. That ranked the work below.

## 2. What the sampler found (s4's p5 runs, the tip's code)

| pattern | share of the game thread (attract / lake) | where |
|---|---|---|
| software division (the A9 has no divide instruction) | `__udivmoddi4` 0.33 / 0.26%, `__udivsi3` 0.29 / 0.20% | 64-bit: `EeScheduler::syncCyclesToHostClock` 81% (ns -> us `/ 1000`, us -> cycles `/ 10^6`), syncFrame, `s1ptcl::draw` (`n % 4000`); 32-bit: `invocationStackTop` 75%, `wakeupThread` 25% |
| `memcmp` (newlib: a call, 6 instructions a word aligned, 5 a byte otherwise) | 1.00% | cache validations (`memcmp(rdram + x, kept, 16) != 0`): coll125, coll115, seg80, s2coll, the particles, modelDispHook |
| emulated TLS (`__emutls_get_address`, `pthread_getspecific`, `__getreent`, `vitasdk_get_tls_data`) | ~0.3% | `ps2_load::t_slot` at every hook (GameZone) and twice per guest dispatch; malloc's reent; `syncAccounting`'s `this_thread::get_id` |
| function-local static guards (`ldr; dmb ish; lsls; bpl`) | not visible (1 instruction each in Vita3K) | modelDispHook 19 `dmb`, obj125Run's lambda 30, leafTA9 23, mdslane::join 11, the switch helpers (`checkOn()`, `params()`, `enabled()` ...) inlined everywhere |
| scalarised NEON float compares | clip83::clip 41 masks, glVertexKick 18, char122 8 + 8, quad117Body 6 | all D11's sites, measured there and reverted (z < 2): nothing new |
| soft-float conversions (`__floatdisf` ...) | 0.03% | not worth a change |

Most guard `dmb`s counted statically sit in check-only branches (FastCheck statics, `SOTC_*_CHECK` blocks) and never run.

## 3. The changes (all behind `SOTC_A9SWEEP`)

The parts (`ps2_a9.h`: `SOTC_A9SWEEP_AB_PARTS=<mask>` makes only those parts follow the A/B, the others stay at
`SOTC_A9SWEEP`'s value; default all):

| part (mask bit) | what | where |
|---|---|---|
| hoisting (1) | function-local statics with a dynamic initialiser (`static const bool x = getenv(...)` ...) become plain globals set once at install (`A9S_HOIST`, read with one load); the original static is the off path (`A9S_VAL`) | 48 switch helpers (`checkOn()`, `params()`, `enabled()` ...), 35 in-body statics of hot functions, modelDispHook's nine, gameLodRaw, dispDispObjGroupModel's vectors, s1ptcl's counters; in the runtime, wakeupThread's selfWake and syncCyclesToHostClock's max lag as members |
| memcmp (2) | `memcmp(a, b, N) == 0` with a constant N as word compares (`a9eq::eq<N>`: unaligned word loads folded with xor / or, `A9_MEMEQ`) | 82 sites: the cache validations in coll125, coll115, seg80, s2coll, the particles, gctick, texreset, the walk, modelDispHook |
| divisions (4) | syncCyclesToHostClock's two 64-bit divisions (ns -> us `/ 1000`, us -> cycles `/ 10^6`) by long division in base 2^16 (`ps2_a9::udivSmall`: four 32-bit divisions by a constant, i.e. multiplies); s1ptcl's `n % 4000` the same way | EeScheduler.cpp, s1_particles.cpp |
| scheduler caches (8) | invocationStackTop's answers direct-mapped (16 ways) in front of its `unordered_map<uint64_t>` (whose 32-bit hash is the depth only: a division per node); thread() keeps the last 8 found by `id & 7` (wakeupThread of another thread no longer evicts the running one) | EeScheduler.cpp, ee_scheduler.h |
| zones / TLS (16) | GameZone and the dispatch's zone stores (an emulated-TLS call each: `t_slot` is a thread_local) skipped while nothing reads them (prof129 off) | ps2_thread_load.h, EeScheduler.cpp, decomp_walk.cpp, sotc_native.cpp |

Exactness:

- `src/divtest.cpp`: udiv1e6 / sdiv1000 / udivSmall against the compiler's division, every 32-bit low word under a set of
  high words plus random 64-bit values: **1.1e11 cases, 0 differ** (INT64_MIN included).
- `src/eqtest.cpp`: `a9eq::eq<N>` against `memcmp(...) == 0` on random buffers, equal and with one bit flipped, every pair
  of offsets 0-3: **1.6e7 cases, 0 differ**.
- The hoisted values: `SOTC_A9SWEEP_LOG=1` prints every hoisted value at install and, in the interleaved A/B (where the
  off path's original statics initialise too), each original value beside it, `DIFFERS` if not equal. Initialisers run in
  registration order (a value may read one hoisted before it in the same file).
- The caches: the map never forgets or changes an invocation's stack top while the thread lives; thread()'s cache is
  forgotten wherever the thread map is erased or cleared.

## 4. Speed (Vita3K 222 MHz, interleaved A/B `SOTC_AB_FLAG=a9sweep SOTC_AB_PERIOD=1`, both seeds, p1 = commit 171fcea)

`aba9s.sh`: 1 260 s runs, tools/mixframes.py `--shift -1` (D11's method) from frame 3000, blocks of 300 frames, +- 2 SE.
Game busy = the game thread's own time a frame from the frame log, per state.

| run | frames on / off | **frame off - on** | game busy on / off (ms) | busy off - on | render thread off - on | hook jobs off - on |
|---|---|---|---|---|---|---|
| mixA7p1 (attract, 7) | 18 872 / 18 890 | **+0.437 +- 0.131** | 25.50 / 25.99 | +0.473 +- 0.119 | -0.022 +- 0.066 | -0.004 +- 0.047 |
| mixA11p1 (attract, 11) | 17 954 / 18 342 | **+0.302 +- 0.109** | 23.79 / 24.14 | +0.310 +- 0.091 | +0.026 +- 0.080 | +0.040 +- 0.036 |
| mixL7p1 (lake, 7) | 10 514 / 10 526 | **+0.748 +- 0.152** | 42.42 / 43.11 | +0.732 +- 0.148 | -0.224 +- 0.130 | +0.076 +- 0.049 |
| mixL11p1 (lake, 11) | 10 615 / 10 710 | **+0.711 +- 0.125** | 41.82 / 42.51 | +0.688 +- 0.126 | -0.206 +- 0.160 | +0.079 +- 0.054 |

The whole sweep: frame time -0.30..-0.44 ms on the attract demo (z 5.5 / 6.7) and -0.71..-0.75 ms at the lake (z 9.8 /
11.4), 1.2-1.7% of the frame; the game thread 0.31-0.73 ms a frame less busy. The render thread at the lake +0.21-0.22 ms
a frame with the sweep on (z 2.6 / 3.4; S4 saw the same sign there: the faster game thread hands it the next frame
sooner); the hook worker unchanged within 2 SE.

**The frame gain is larger than the removed helper calls alone** (the division helpers and memcmp were ~1.3% of the game
thread at most). Vita3K's time is instruction-count based and a `dmb` is one instruction there; the per-part A/Bs below
say which part carries the rest. On the console the `dmb ish` itself (a store-buffer drain on the A9) comes on top of
whatever Vita3K shows: a console-only gain.

### Per part

**Why not a sampler z per part.** The sampler's samples carry no A/B state, so a part can be priced from it only through
code that runs in one state alone. Here the old code is shared library helpers (`memcmp`, `__udivmoddi4`, emulated TLS)
that unconverted callers run in both states, and the sampler records LR = 0 inside those leaf helpers (callers by LR are
not recoverable). The `aba9s.sh` NEW / OLD line (ps2_a9's helpers vs the division helpers) proves it: it "saves" 0.12-0.18 ms
with z > 100 even in the runs where the divisions do not flip (m1, m2), and comparing helper rates between runs fails a
null test (s4's two seeds of one build: memcmp z -55). So each part (or group) got its own interleaved A/B instead:
`SOTC_A9SWEEP_AB_PARTS=<mask>` flips only those parts, the rest stay on; the lake (the largest effect; one heavy process at
a time on the Mac by then, so the attract demo was not repeated per part), p2, seeds 7 and 11, 1 260 s.

| part (mask) | seed 7: frame off - on | seed 11 | busy off - on (7 / 11) | decision |
|---|---|---|---|---|
| hoisting (1) | +0.046 +- 0.137 | +0.091 +- 0.140 | +0.034 / +0.067 | **kept** on exactness and no regression (z 0.7 / 1.3): Vita3K cannot price a `dmb`; the gain is console-only |
| memcmp (2) | +0.249 +- 0.126 (z 4.0) | +0.372 +- 0.132 (z 5.6) | +0.243 / +0.368 | **kept** |
| scheduler: divisions + caches + zones (4+8+16 = 28) | +0.408 +- 0.174 (z 4.7) | +0.275 +- 0.112 (z 4.9) | +0.403 / +0.260 | **kept** |

The three add up to the whole sweep at the lake (+0.05 + 0.25..0.37 + 0.28..0.41 = 0.58..0.83 against 0.71..0.75 ms). The
scheduler's three parts were measured together (each alone is below what a 1 260 s A/B resolves: the sampler put the
division helpers at ~0.6% and the TLS helpers at ~0.3% of the game thread before); all three are exact.

## 5. Equivalence and checks (p2, sweep on, `SOTC_A9SWEEP_LOG=1`)

| run | check | result |
|---|---|---|
| chk2_att (attract, 720 s) | `SOTC_S4DISP_CHECK=5` (+ S3) | 1 338 screen-pass checks: 0 unknown words, 0 effect / host-state / emission differences; 5 744 kick palettes vs the worker's copy, 0 differ (the check runs the demo at 1.4 fps: 1 234 frames) |
| chk3_att (attract, 720 s) | `SOTC_S3CHARA_CHECK=1` alone | 340 001 poses, 7 408 963 bones, 0 differ (29 118 frames) |
| chk2_lake (lake, ~700 s*) | `SOTC_S2COLL_CHECK=1` | 183 167 view clips 0 differ, 1 065 990 keep tests vs fin125 0 differ; s2water 135 754 under-end + 79 480 crossing probes 0 differ |
| all of the above + the four A/Bs | `SOTC_A9SWEEP_LOG=1` | 90 hoisted values printed at install (85 initialisers); in the A/Bs 73-76 of the off path's original statics initialised, each beside its hoisted value: **0 DIFFERS** |

\* chk2_lake was stopped at ~700 of 720 s when the Mac's load had to drop to one heavy process; its err.txt was taken from
the tree.

## 6. Pictures

Window shots of the A/B runs (both states interleaved frame by frame) and the check runs, looked at: Wander and Agro on the
ground at the attract demo's gallop (shadows under Agro), Wander swimming at the lake, the shrine; nothing changed.

## 7. Memory

`arm-vita-eabi-size`: celosia-s4.elf text 54 499 450, data 1 975 286, bss 29 906 128; the a9sweep build (p2) text
54 684 942 (+185 492: both paths of every converted site, the off path kept for the A/B), data 1 975 290 (+4), bss
29 907 264 (+1 136: the hoisted globals, the scheduler's caches). +186 632 bytes in all (0.18 MB).

## 8. Console

`build/agent-scratch/celosia-a9s.vpk` with `build/agent-scratch/a9s/env-console.txt` (`SOTC_AB_FLAG=a9sweep,s4disp,a9cmp`,
`SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=600`): the console prices the barriers Vita3K cannot. Note that the console env keeps
`SOTC_PROF129=1`, which reads the zones: the zones part (16) then does nothing there.
