# Round S18: why the old path met the pending flag ~90 times a frame, and S17 measured for real

Redesign round S18 (2026-11-19). Scripts, runs and logs: `build/agent-scratch/s18/` (in this worktree; the Vita build dir
cloned from S17's lives at the main checkout's `build/agent-scratch/s18/vita` so its paths keep S17's length): clone.sh
the build dir and p1, lab.sh the long-block A/B, q1.sh the root-cause run and the per-frame A/B, abs17.sh / chk17.sh /
run.sh / v3k.sh as S17's, tools/winab.py the window-by-window A/B, tools/pcname18.py the `[s18]` pcs named. Own Vita3K
trees ../vita3k-celosia-s18 / -s18b / -s18c (222 MHz, 3 cores, muted), deleted at the end. Build p1: S17 plus the `[s18]`
counters (no behaviour change).

## 1. S17's A/Bs never turned S17 on

The A/B (`SOTC_AB_FLAG=<name>`, sotc_native abSwitches) turns a listed switch **off** in its off windows and back to its
*starting* value (`switchStart[i] = switchOn(i)` at the first frame) in the others. s17iop is off by default and S17's A/B
runs did not set `SOTC_S17IOP=1`, so both turns were off: mixR7p2 / mixR11p2 / mixL7p2 / mixA7p2 / blkR7p2 compared the
old path with itself (their +0.156 ± 0.168, -0.005 ± 0.250, ... are A/A noise). Seen in S18's first long-block runs
(aaR7 / aaR11, p3, without `SOTC_S17IOP=1`): every `[s17]` line `nested 0`, the old path's episodes in every window.
**An A/B of a default-off switch needs the switch's env set to 1** (as S11 / S13 did for theirs). S18's A/Bs below set
`SOTC_S17IOP=1`; the `[s17]` lines show nested episodes in the on windows only.

## 2. Why the old path re-enters ~90 times a frame (`[s18]`, rcOff: ride7 600 s, S17 off)

`[s18]` (with `SOTC_S17IOP_LOG`): every pending checkpoint of the game thread at its own context on the old path, whether
`unwinding` was already set when it ran, how many came between two of run()'s dispatches, and their pcs.

| rcOff, a frame over 600 | pending at home | with `unwinding` already set | runs (between two dispatches): length 1 | length 256+ |
|---|---:|---:|---:|---:|
| frame 1 200 (the load) | 588.6 | 587.4 | 0.93 | 0.212 |
| frames 2 400-11 400 (riding) | 35-99 (60 typical) | 34-98 | 0.97-0.98 | 0.025-0.048 |

- About one checkpoint a frame is the normal unwind (the GS FINISH handler, S17's episode; first pc
  iosGetTCountExtension, the game thread's poll: 10 963 of 11 400 runs).
- **Everything else (98%) is met with `unwinding` already set**: a host frame went on running guest code after its callee
  had unwound. These come in rare, very long runs: ~0.03-0.05 a frame (one every 20-40 frames), each ~1 500-2 000
  pending checkpoints long, with no dispatch of run() in between.
- Who goes on: the natives. A native (decomp C++) calls guest code through sotc_decomp::callGuest / callMaySleep /
  runToReturn, whose loop runs the callee *until it returns to the native* (`while (pc != ra) guestFunctionAt(pc)(...)`):
  a native has no continuation, it cannot return early. When the episode's checkpoint fires inside such a callee, the
  generated frames return with `unwinding` set and pc at the resume point, and the native's loop runs from there. With
  `unwinding` set and the pending flag still up (only run() clears them), every guest call below the native now returns
  at its first checkpoint (callGuestFnBody's checkpoint, pending) and every guest return finds no host frame to return to
  (each generated caller returns early on `unwinding`): each call and each return is one turn of the native's loop, a
  table lookup and a re-entry at a label in the middle of a function. That lasts until the native itself returns to
  generated code, which then unwinds to run(), where the handler finally runs.
- Which natives: the repeats' pcs (rcOff, totals): scrRun 6 877 (the script frame end: decomp_hot44d's scriptPass, the
  native pass over the script objects that wakes each sub-thread), iosDlPopLayer 4 828 / iosDlPushLayer 4 763 /
  iosDlChainTailCurrent 1 412 (the native draw code's display-list calls), isysGroupForFirst 4 502 (scriptPass),
  cameraBlur+0x8c 796, resetMicroCodeManager+0x18 698, glReset+0x60 369, get_cur_camera_context 203, the light matrices.
  So: when the GS FINISH interrupt lands while the game thread is inside the script pass or the native draw code (about one
  frame in 20-40), the rest of that native's work runs chopped at every call and return, and the handler waits until the
  native is done. That is S15's / S17's "re-entering through the dispatcher in the middle of the sub-thread coroutines
  and execScript" (execScript+0x2c is where the game thread resumes after the script pass's long run).
- Why S17's check runs did not show it: in the check, every pending checkpoint draws old / nested again, so a chopped run
  ends at its first nested draw (two checkpoints on average); the old turn's episodes looked short (52-67 µs).
- With S17 on, the first pending checkpoint nests (the handler runs there, pending cleared, `unwinding` never set): the
  chopped runs disappear (pending 0.0-0.8 a frame). S17 is the fix for it; a separate old-path fix (the natives'
  loops nesting or deferring the episode) would be S17 again in another place, so no kS18Chk.

## 3. What it costs: the A/B with S17 really on (Vita3K 222 MHz, off - on, p3 / p1, `SOTC_S17IOP=1`)

Per-frame interleaved (SOTC_AB_PERIOD=1, mixframes.py, 300-frame blocks, ± 2 SE; ride7 from the mount + 600; p1):

| run | where | off - on, frame | busy | pending checkpoints a frame (both turns; the off turn's ~2x) | old / nested episode |
|---|---|---:|---:|---:|---:|
| mixR7p1 (800 s) | ride7 seed 7, mounted at 2 622, 42 blocks | +0.262 ± 0.292 ms | +0.374 ± 0.268 | 41.7 | 701 / 75 µs |
| mixR13p1 (800 s) | ride7 seed 13, mounted at 2 230, 43 blocks | **+0.507 ± 0.234 ms** | +0.517 ± 0.242 | 64.7 | 1 337 / 99 µs |
| mixR11p1 (800 s) | ride7 seed 11, mounted at 3 138 (an emptier stretch: 25 ms frames), 52 blocks | +0.047 ± 0.101 ms | +0.046 ± 0.088 | 12.7 | 230 / 52 µs |
| mixL7p1 (400 s) | lake6 seed 7, 14 blocks | **+0.447 ± 0.258 ms** | | 42.4 | 1 205 / 62 µs |

Long blocks (SOTC_AB_PERIOD=600: on or off for 600 frames at a time, 1 600 s; p3; mixframes 1 200-frame blocks, and
tools/winab.py: each window against its opposite neighbours):

| run | where | off - on (blocks) | (windows vs neighbours) |
|---|---|---:|---:|
| lbR7 | ride7 seed 7, 19 blocks / 40 windows | +0.448 ± 0.859 ms | +0.895 ± 0.838 |
| lbR11 | ride7 seed 11, 20 blocks / 41 windows | -0.147 ± 0.889 ms | -0.023 ± 0.689 |
| aaR7 / aaR11 (A/A: S17's mistake, both turns off) | ride7, cut at ~900 s | -0.668 ± 2.988 / -4.200 ± 3.694 | |

- The gain follows the chopped runs: ~2-5 µs per pending checkpoint the off turn meets (R11's 25 a frame +0.05 ms, R7's
  83 +0.26, L7's 85 +0.45, R13's 129 +0.51). Every per-frame run's sign is S17's.
- The ride7 per-frame runs together (inverse-variance): **+0.132 ± 0.088 ms (2 SE), z 3.0**; with the lake run
  +0.165 ± 0.084, z 3.9.
- The long blocks see the same sign in seed 7 but are an order noisier (the scenery changes over 600 frames): they settle
  nothing alone, and there is no build-up to find. The expensive state is per episode (a chopped run ends when its native
  returns, within the frame), so the per-frame A/B sees it whole; S17's worry that it needed old episodes in a row came
  from the check's re-draw (section 2) and from its A/Bs being A/A (section 1).
- Nested, an episode is 52-99 µs of host time; the old one 230-1 337 µs (that includes the native's own remaining work run
  chopped).

## 4. Decision

**S17 on by default** (`SOTC_S17IOP=0` turns it off; s17iop.h init: the A/B's off turns still flip it, so `SOTC_AB_FLAG=s17iop`
now compares on with off without any env). Exact by S17's check (99.4-99.7% of episodes in orders both paths produce,
the common ones in the same proportions); it takes ~60-130 chopped checkpoints a frame off the game thread riding,
+0.13-0.17 ms a frame on Vita3K 222 MHz on average (+0.5 ms where the script pass and the native draw code are busiest).
Not measured on the console (no console runs); its A/B (`s17iop`, off turns = the old path) can confirm.

## 5. Boot, alignment

bootRp2 (p2: S17 on by default, no S17 env; `SOTC_VITA_EAT_USER_MB=89` taken; ride7 600 s): booted, mounted at frame
5 978 and rode to 11 337, no fault; `[s17]` nested 1.00 a frame, pending 0.5, `[s18]` none met with `unwinding` set; the
window shots normal (the plain's ground, Wander on Agro galloping past the cliff, the sky). spalign105.py: 0 sp-based
:128 / :256 NEON accesses (of 975) in p2. Code commit ae8ef1f.

## 6. What it leaves

- The console A/B (`SOTC_AB_FLAG=s17iop`: now on against off without any env).
- S17's not-nested cases (inside a handler / invocation, a posted wake whose waiter has not parked, < 192 KB of host
  stack: 0.2-0.8 a frame) still unwind the old way; one landing inside a native would still go chopped. Not seen riding
  (`[s18]` 0 with `unwinding` set in bootRp2).
- Any A/B of a default-off switch must set its env to 1 (check the switch's own log that the on turns really are on).
