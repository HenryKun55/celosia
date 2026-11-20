# Console C1: the character chain's memory traffic (2026-11-20)

Round C1 follows the console profile of the same day (docs/console_profile_2026-11-20.md). On the Vita the game
thread takes ~40 ms a frame riding. The profile is flat, and the slowdown against Vita3K is spread over hundreds of
functions. That points at memory latency, which Vita3K does not model. The characters (motion, pose, cloth, IK) are the
largest subsystem at 8.4-9.1 ms a frame. C1 cuts what the character chain reads from guest memory on each call, with
results that are the same bits. It adds two switches, both **off by default**. The verdict is a console A/B.

Scripts, runs and logs: `build/agent-scratch/c1/` (NOTES.md, chk.sh the exactness run, q.sh / abn1.sh the A/B,
after.sh the ride check and the boot). Vita3K trees: ../vita3k-celosia-c1, -c1b, -c1c. The VPK / ELF of the tip:
`build/agent-scratch/c1/c1.vpk`, `c1.elf` (switches off unless set).

## 1. The chain's hot functions and what they read

From the profile (run1 / run2, ms a frame on the console): motionMatrix (S3's pose) 0.5-0.7, execCharaGeometry
0.3-0.5, the motion keys (floatFrame + intFrame + the root key) ~0.7, rootBlend 0.3-0.4, the cloths ~1.2, charaLinks,
gaze, ctrl* and the quaternion helpers 0.1-0.3 each.

- **The motion keys** (GetMotionFloatFrameNoCaribration 0x10AD1B0 = keys122, GetMotionIntFrame 0x10B3E98 = key136).
  For every channel, every call reads the format byte, the channel pointer, the channel's 0x24-byte curve record, and
  the key word(s) of frame k (and k + 1). Records sit 0x24 apart. Each channel's keys are a separate array, so a key
  read costs one cache line per channel. Riding: ~12 calls a frame (6 float, 6 int) of ~26 channels, over ~9 motions;
  the lake: ~6 calls of ~38 channels. Sampled per call, the decode touches 40-48 distinct 32-byte lines riding and
  ~70 at the lake.
- **The pose** (S3's rig). Each of the ~14 poses a frame compares the rig with the skeleton's bone records: each bone's
  offset (+0x10) and links (+0x30), two lines per 0x40-byte record, ~25 bones a pose riding. Only ~4 of those poses a
  frame are the first pose of their skeleton in that frame.
- **Cloth**: S8's queue (`s8cloth`) moved the cloth calls to the guest worker, and it was slower in every run: the
  context / VU0 / scratchpad copies at each hand-over and merge cost more than the cloth, and the joins come ~0.1-0.4 ms
  after the hand-over, so the game thread waits anyway (docs/characters_s8.md). It stays off. C1 does not move work to
  a worker (cores 1-2 are already ~60% busy).

## 2. What changed

### c1keys (`SOTC_C1KEYS=1`, A/B `c1keys`, `kC1Keys`)

`decomp_char122.cpp`, `char122::keys::c1`; hooked into keys122's floatFrame and into GetMotionIntFrame
(`intKeysC1`). For each motion resource the game thread keeps a host mirror:

- a copy of what the decode reads besides the keys: the formats, the channel pointers, 0x30 bytes from each record's
  qword, and the header word (frame count). Every game frame, on a motion's first call, the header, formats and channel
  pointers are compared (they are contiguous), plus the first and last records and every 8th record by the frame's phase.
  So every record is compared at least once every 8 frames (`SOTC_C1KEYS_ROT`; 1 = every record every frame). Any
  difference drops the mirror's rows and makes it again. Motion data are resources: the game does not write them after
  load. A motion freed and another loaded at the same address with the same channel pointers (the case round 122 met)
  differs in its records' scales, offsets and key pointers.
- rows by frame: frame f's keys as `decodeKey` makes them (the same function on the same words), decoded the first
  time a call needs that frame. After that, every call reads the row: n x 16 contiguous host bytes.

A call then reads one row (integer frame) or two rows (float frame) and blends them with keys122's own blend
(`keys::blend`, now shared by both paths: the same NEON operations). The gallop and walk cycles return to the same
frames every second, so **~99% of the rows needed are already decoded** (riding 17.8 hit / 0.1 decoded a frame; the
lake 10.2 / 0.0).

Memory: one arena (`SOTC_C1KEYS_KB`, 1024 KB) allocated at first use. 300-580 KB were in use in the runs. A full
arena drops everything and starts again (counted; 0-1 flushes a run). Only the game thread (lane 0) uses it; the
worker lane and S3's split jobs keep the decode.

### c1rig (`SOTC_C1RIG=1`, A/B `c1rig`, `kC1Rig`)

`s3_chara.cpp`, `rigFor`: a rig compared with the guest's bone records in this game frame is trusted for the frame's
later poses of that skeleton. The first pose of each frame still compares. Skeletons are resource data, so their links
and offsets are not written while they live.

### Runtime switching

Both switches can be flipped per window by abSwitches. While off, nothing is read or kept current. When switched on
again, a mirror or rig is compared before its first use in a frame, because its frame stamp is old. So stale state can
never be served after an off window.

## 3. Exactness (in-run, bit for bit)

- `SOTC_C1KEYS_CHECK=1`: every served call also runs the decode from the guest's words, and the results are compared
  with memcmp (`[c1keys] ... check A compared B differ`, `[c1chk]` for each difference).
- `SOTC_C1RIG_CHECK=1`: every trusted rig is compared anyway (`[c1rig] ... stale`), and `SOTC_S3CHARA_CHECK=1` compares
  every pose with round 82's pose read straight from guest memory.

| run (build) | where | c1keys calls compared | differ | rigs trusted (all checked) | stale | poses compared (bones) | differ |
|---|---|---:|---:|---:|---:|---:|---:|
| chkR (p1, full compare each frame) | ride7, 800 s, mounted at 3 208 | 332 936 | **0** | 567 078 | **0** | 333 381 (8 235 736) | **0** |
| chkL (p1) | lake6, 400 s | 73 106 | **0** | 189 443 | **0** | 116 970 (3 864 996) | **0** |
| chkL2 (p2, records in turns) | lake6, 400 s | 72 058 | **0** | 187 271 | **0** | 115 828 (3 845 114) | **0** |
| chkR2 (p2) | ride7, 800 s, mounted at 5 267 | 372 299 | **0** | 660 474 | **0** | 384 058 (9 245 791) | **0** |

## 4. Speed in Vita3K (222 MHz, interleaved A/B, `SOTC_C1KEYS=1 SOTC_C1RIG=1 SOTC_AB_FLAG=c1keys,c1rig`, p2)

| run | frame off - on | 2 SE |
|---|---:|---:|
| mixR7 (ride7, seed 7, 800 s) | +0.149 ms | 0.143 |
| mixR13 (ride7, seed 13, 800 s) | -0.012 ms | 0.146 |
| mixL7 (lake6, seed 7, 400 s) | +0.054 ms | 0.115 |
| **pooled** | **+0.064 ms** | **0.078** (z +1.6) |

Not slower in Vita3K (slightly faster, below z 2). Vita3K does not model cache misses, so this only says the
bookkeeping costs no more than the decode arithmetic it saves. The memory effect needs the console.

## 5. Memory traffic (estimate)

c1keys samples one call in 32 (`SOTC_C1KEYS_LOG`). For each sampled call it counts the distinct 32-byte lines (and
bytes) the decode would read: formats, channel pointers, records, key words and the curve table. It also counts what C1
read: the header line, the host rows, the validation's guest words when that call validated, and the decode's reads for
rows made then.

| route | lines a call: decode -> C1 | bytes a call | lines a frame (calls x lines) |
|---|---|---|---|
| ride7 (p2 A/B runs, on frames) | 40-43 -> 30.5 (-25..-29%) | 970-1 060 -> 840-870 | ~12 calls: ~500 -> ~365 |
| lake6 (p2) | 70 -> 43.5 (-38%) | 1 420 -> 1 260 | ~6 calls: ~430 -> ~270 |
| (p1, every record compared every frame) | ride 44 -> 48, lake 70 -> 82 | | worse: the reason for the turns |

Most of what C1 still reads is its own rows, which are 16 bytes a key against the 4-byte packed key. But they are
contiguous host lines that the next calls of the frame reuse, against scattered guest lines (one per channel), and the
decode's arithmetic (int-to-float, the square root) is gone for 99% of the keys.

c1rig: riding, ~24 of the ~28 rig lookups a frame are trusted (chkR: 567 078 trusted, 99 663 compared). That skips
~24 x 25 bones x 2 lines ≈ 1 200 line reads a frame (~14 KB of loads: 8 link bytes and 16 offset bytes a bone). Most of
those lines were warm from the frame's first pose (L1 / L2 hits), so the saving is load and compare work more than
misses.

## 6. Boot and pictures

bootR (p2, ride7, 600 s, `SOTC_VITA_EAT_USER_MB=89` taken, both switches on from the start) mounted at frame 2 506
and rode to frame 18 575 (game 32.8 fps at the end) with no fault, the arena at 425 KB. Window shots every 120 s, and
those of chkR (p1): Wander riding Agro at the gallop, the mantle, the hair and the reins moving as usual, the ground,
the sky, the shrine bridge and the shadows normal. spalign on p1 and p2: 0 sp-based NEON accesses (of 1 014 with an
alignment hint).

## 7. For the console A/B

Build `build/agent-scratch/c1/c1.vpk`. Env: `SOTC_C1KEYS=1 SOTC_C1RIG=1 SOTC_AB_FLAG=c1keys,c1rig SOTC_AB_PERIOD=300`
(both together). To tell them apart, use `SOTC_AB_FLAG=c1keys` or `c1rig` alone with only that switch's env. The
switch names for the A/B are **c1keys** and **c1rig**. With S20's rule, a listed switch that starts off is turned on in
its on windows anyway. Add `SOTC_C1KEYS_LOG=600` for the `[c1keys]` counts.

## 8. What C1 did not do

The rest of the chain (rootBlend, execCharaGeometry's body, charaLinks / findRecord's handle walks, the gaze and the IK
helpers, the cloths) still reads guest records through the guest context on every call. The same pattern (a host
mirror checked in turns, contiguous host rows) fits gaze's per-node curveEval keys and the cloth grids. The display walk
and modelDispHook were left to the display round.
