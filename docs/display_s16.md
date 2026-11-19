# S16: the model draw plans vouched for by the game's own model setup calls, not checked against the guest list every frame (2026-11-19)

Round S16's brief: S14's section 6, item 1, "the plans": build the model draw plans from host model data filled when the
model is set up, so planChecked's per-frame checks against the guest packet lists (and packetListHash, one frame in 16
per header) go away. A small round: one part. Item 2 (the lights as a host record) was not started: the hook's light
copy is one 128-byte block (S14 section 1), below anything an A/B resolves.

Switch: `SOTC_S16PLAN` (A/B name `s16plan`; sotc_decomp switch `kS16Plan`, the last of the tables, **off by default**:
on with `SOTC_S16PLAN=1`, see section 4). Check mode `SOTC_S16PLAN_CHECK=1`, counts `SOTC_S16PLAN_LOG=<frames>`
(`[s16plan]` lines, default every 600 frames with the check), `SOTC_S16PLAN_MOVES=1` the stricter rule of the first
builds (every garbage move withdraws every stamp; section 2). Code: game/overrides/s16plan.h (the state, the lane queue,
the counts), sotc_native.cpp (planChecked; s16Vouch / s16Taken / s16Drain / s16Mark; the three setup hooks). Scripts,
runs and logs: build/agent-scratch/s16/ (q1.sh / q3.sh the queues, chain1.sh / chain2.sh build-then-queue, abs16.sh the
A/B, chk16.sh the check, prof.sh the sampler, vq.out). Builds p1-p4 (p4 shipped; the A/B runs need `SOTC_S16PLAN=1`
besides `SOTC_AB_FLAG=s16plan`: the A/B only turns a switch off in its windows, so an off-by-default switch stays off).

## 1. The profile (ride7, Vita3K 222 MHz)

profB0 (main's build celosia-s13, 700 s, frames 120-660 s, game thread 31.67 ms a frame): planChecked **0.169%** of
the game thread (~0.054 ms a frame; packetListHash is inlined into it), modelDispHook 1.37%, objectLight 0.29%,
hookScrolls127 0.18%. Riding, ~40 plans are looked at a frame (one planChecked each past the frame's first use), each
reading the header's packet list and surface table pointers and the list's first five words (two cache lines of guest
memory, ~1.3 us a plan in Vita3K), and one frame in 16 per header the whole list again. The part is small: at best
~0.05 ms a frame, below what a Vita3K A/B resolves; S16 does it because it removes a per-frame round trip into the
PS2-shaped model data and gives the plans a host-side owner (the setup hooks) that later rounds can fill further.

## 2. What changed (`SOTC_S16PLAN`)

Before (round 8, planChecked): each frame a plan is used, header +0x60 (packets) and +0x50 (surfaces) and the first five
words of the list are compared with the plan; one frame in 16 by header the whole list (size, LOD, surface, part, data
of every packet) is hashed again; a mismatch rebuilds the plan.

Now: three of the game's own model setup calls are hooked (each runs the table's function, lifted or not, then notes
its header):
- **modelInitialize** (0x1099680, a1): loadCdNmoFile's setup of a model file just read (the model load);
- **modelReInitialize** (0x1096f60, a1): garbageNmoCallBack's, after the heap's garbage pass moved a model file and
  relocated it (the model at its new address);
- **modelInitDispModelClass** (0x1096b88, a3): an instance's display class.
Each call notes the header as set up (a direct-mapped table of headers) and withdraws the stamps of its plans (every
LOD); a call from the guest worker lane is queued (a mutex-guarded ring) and marked by the game thread before it next
looks at a plan (none came on any route).

A plan is stamped (s16Vouch) when it is built, or when round 8's checks held **in full** (pointers, first words and the
whole list's hash and count). A stamped plan of a header seen set up is **taken as it is**, with no guest read; a plan
whose stamp was withdrawn is checked in full once and stamped again (rebuilt when the check fails); a plan of a header
never seen set up keeps round 8's checks at their cadence. No guest memory is written; the plans (and everything the
jobs read from them) are the same.

How it got there (the in-run counts of the first builds):
- p1 / p2 noted only modelInitDispModelClass and also withdrew every stamp at any garbage move (the move serial, two
  frames' margin): most headers (the world's models) never get a display class through it, so only ~21% of the uses
  were taken unchecked riding (chkRp2: 32 181 of 169 053, 0 differences). (p1's log line also printed `%zu`, which the
  Vita's printf does not know: its counts were garbage; p2 prints every count as `%llu`.)
- p3 added modelInitialize / modelReInitialize (found through their callers loadCdNmoFile / garbageNmoCallBack): 94-98%
  taken, 0 differences in 543 500 checked takes (chkRp3 / chkLp3 / chkAp3: 200 894 / 177 722 / 164 884). But riding the
  garbage pass keeps moving blocks (the move serial climbs all ride), and every plan not taken was hashed in full every
  frame: the sampler had planChecked at 0.24% of the game thread in p3's half-on A/B against 0.23% all off. Slower, not
  faster.
- p4 (shipped): a garbage move no longer withdraws stamps (a moved model file is relocated and set up again by
  garbageNmoCallBack -> modelReInitialize, which the hook sees; `SOTC_S16PLAN_MOVES=1` restores p3's rule), and only a
  plan that can be stamped is checked in full. 99.8% taken riding.

## 3. Equivalence

`SOTC_S16PLAN_CHECK=1` (chk16.sh, switch on): every plan taken unchecked is also checked the old way in full (the
pointers, the first five words, the whole list's hash and count); a difference would be counted, printed and the plan
rebuilt.

| run | build | route | frames | plans looked at | taken unchecked (checked) | stamped after a full check | moves (serial) | setup calls (init / reinit / disp class) | differ |
|---|---|---|---:|---:|---:|---:|---:|---:|---:|
| chkRp4 | p4 | ride7 (800 s, mounted at 3 568) | 15 000 | 522 880 | 522 046 | 47 | 11 785 | 789 / 4 503 / 1 730 | **0** |
| chkLp4 | p4 | lake6 (450 s) | 8 400 | 177 297 | 168 548 | 38 | 3 962 | 310 / 904 / 496 | **0** |
| chkAp4 | p4 | attract (450 s) | 14 400 | 168 699 | 168 664 | 35 | 1 713 | 161 / 507 / 621 | **0** |
| chkRp3 | p3 | ride7 (700 s, on foot) | 6 600 | 210 099 | 200 894 | 9 205 | 2 503 | 275 / 633 / 400 | **0** |
| chkLp3 | p3 | lake6 (450 s) | 9 000 | 187 484 | 177 722 | 9 762 | 3 771 | 304 / 869 / 490 | **0** |
| chkAp3 | p3 | attract (450 s) | 14 400 | 168 408 | 164 884 | 3 524 | 1 979 | 181 / 551 / 641 | **0** |

p4: 859 258 unchecked takes over the three routes, through 17 460 garbage moves and 10 021 setup calls, 0 differences,
0 failed full checks (the lake's 8 711 uses of headers never seen set up kept round 8's checks). spalign: 0 sp-based
NEON :128 / :256 accesses (of 975 with an alignment hint, p1 and p4).

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_S16PLAN=1 SOTC_AB_FLAG=s16plan SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

abs16.sh, 400 s a run, p4; frames from the mount + 600 (ride7), from frame 3 000 otherwise (mixR11p4's route did not
mount in time: its window is on foot); tools/mixframes.py (300-frame blocks, +- 2 SE). Positive = on is faster. The machine
was loaded by other rounds' builds and Vita3K runs throughout (ride7 ~36 ms a frame against ~30-32 normally).

| run (build, seed) | route (window) | frames | **frame off - on** | game busy off - on | plans taken (on-frames) |
|---|---|---:|---|---|---:|
| mixR7p4 (p4, 7) | ride7 (mount 2 215 + 600) | 5 108 | **-0.085 +- 0.371** | -0.085 +- 0.355 | 119 144 of 125 521 |
| mixR11p4 (p4, 11) | ride7 (on foot, from 3 000) | 4 923 | **+0.058 +- 0.247** | +0.040 +- 0.246 | 125 473 of 125 601 |
| mixL7p4 (p4, 7) | lake6 | 4 973 | **+0.106 +- 0.258** | +0.083 +- 0.263 | 82 667 of 82 833 |
| mixA7p4 (p4, 7) | attract | 10 184 | **-0.033 +- 0.252** | -0.149 +- 0.204 | 70 706 of 70 741 |

The sampler cannot split planChecked by state (it runs in both, cheaper in one), and the planned all-on / all-off
sampler pair and ride7's 800 s A/Bs were not run: the machine's three heavy slots were held by other rounds for over
40 minutes at the round's end. p3's half-on run (above) is the only sampler reading of the function under the switch.

**Decision: `SOTC_S16PLAN` off by default (on with `SOTC_S16PLAN=1`).** The rule is z > 2 in both ride7 seeds; every
A/B is even (all within +- 0.11 ms, 2 SE 0.25-0.37 ms), as the ~0.05 ms at stake is far below what these windows
resolve on a loaded machine. The part is exact (0 differences in 859 258 checked takes) and ready for the console's A/B
(s16plan in SOTC_AB_FLAG, with SOTC_S16PLAN=1), where the two guest cache lines a plan reads cost real memory latency.

## 5. Pictures

Vita3K 222 MHz, muted, window shots looked at (Wander, Agro, the ground, the shadows), then the runs' shots and sampler
snapshots deleted.
- **bootRp4** (p4, the shipped build, with `SOTC_S16PLAN=1`, ride7, `SOTC_VITA_EAT_USER_MB=89`, 600 s): mounted at frame
  4 208, 11 478 frames, 2 game-thread frames over 300 ms (frames 156 / 185, the boot), No exact 0; 365 938 of 385 338
  plan uses taken unchecked, 1 full check failed (rebuilt, as round 8 would). Wander galloping on Agro across the plain
  past the broken pillar and along the cliff, the shadows on the ground: normal.
- **The checks**: the gallop through the bridge's arches with the hawk (chkRp4), Wander swimming in the lake's arena
  (chkLp3), the attract demo's canyon ride (chkAp3), Wander walking beside Agro on the plain (chkRp3): normal.

## 6. What still ties the draw to the PS2

1. **The lights**: objectLight writes the camera-space lights into the instance's own packet (+0x10..+0x90), the hook
   copies 128 bytes; a host record would remove the guest round trip (cheap either way).
2. **The plans' per-packet questions**: the packet ids (packetId's memo by address, sampled), the skinned / lit lookups
   after mesh evictions, the per-frame surface memo. With the setup hooks in place these can become per-model host data
   too (filled at modelInitialize, dropped at the next setup call of that header), and the plan itself could be built at
   modelInitialize instead of at the first draw.
3. The headers never seen set up (the lake's arena models: 8 711 uses in chkLp4) come from another setup path; finding
   it would let them be taken too.
4. S14's items 2 and 4: the palette's guest copy (the kick reads it), the immediate quads' VU-exact arithmetic, the
   strips' segment walk, the lane's mirror of modelDisp, the texture records' GS transfer state, the layer stack / gsReg
   hooks.
