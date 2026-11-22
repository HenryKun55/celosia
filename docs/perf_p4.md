# Perf P4: a garbage move waits only for the jobs that read what it moves (p4move) (2026-11-22)

Round P4's brief: round P3 (docs/perf_p3.md) found the one parallelism loss left on the console's game thread: every heap
garbage move (copyViaSpr -> `g_beforeGuestMove`) joins the whole hook-worker queue. The console n6 logs give `joins move`
0.86 / 1.03 ms a frame riding (Vita3K 0.2-0.6). P3's census: the moves that wait come late in the frame with the whole
display queued (100-500 model, cloth, shadow, particle and 2D jobs). Let the moves go on without the full join, exactly,
behind an off-by-default switch with a check mode and a console A/B package.

Switch: `SOTC_P4MOVE` (A/B name `p4move`, `kP4Move`), **off by default**. Check mode `SOTC_P4MOVE_CHECK=1` in a build with
`-DPS2X_P4_CHECK=1` (p4_move.h's define set to 1). Counts: `SOTC_P4MOVE_LOG=<frames>` (`[p4move]` lines; default 600 when
the switch or the check is on). Code: game/overrides/p4_move.h (notes, stats), sotc_native.cpp (hookjobs: p4Partial,
p4Touches, p4Texture, p4MakePlan / p4MakeFrame / p4Forget, the check p4CheckMove / p4AfterMove / p4NoteReadImpl; push()
and the worker loop: each job's view of the moves), sotc_decomp.h (the switch; moveSerialSeen / moveCountSeen /
moveIndexSeen). Scripts, runs and logs: `build/agent-scratch/p4/` (NOTES.md has every command). Vita3K trees
../vita3k-celosia-c1, -c1b, -c1c.

## 1. The design (design 1 of perf_p3.md section 7: read sets, not copy-on-move)

Design 2 (every guest read of the hook jobs through one accessor with a copy of the moved span) would touch every read
site of the model / texture / packet code (raw `g_ram` reads, `word()`, memcpy of pixels and packets) and add a branch to
each; design 1 needs only what the jobs will read, and most of it is already known on the game thread. So P4 is design 1.

At a move with jobs queued (copyViaSpr -> `g_beforeGuestMove` -> `join(k_move)`; the lane and offload joins before it stay
as they were), the game thread takes the move's two spans, the destination `[dst, dst + bytes)` (what the memmove writes)
and the source `[src, src + bytes)` (where the block was: the bytes the game writes next, the freed tail, its relocation),
and walks the queue **from the newest job back**, asking each job whether what it will read may meet either span
(`p4Touches`). The first job that may is the last one the move waits for: the game thread waits until the worker is past
it (the worker signals when a move waits for it) and lets the move go on with the newer jobs still queued. No job meets
the spans: the move goes on at once. The newest job meets them: the whole join as before. Jobs still run in their order
and draw in it.

What a job reads (the spans; "everything" makes the move wait for it):

| job | spans |
|---|---|
| model with a plan | the header `[h, h + 0x70)`; the packet list `[packets, packets + 32 (count + 1))`; every plan packet's data `[data & ~3, data + size + 64)` (packetId's samples and hashes, the builds' copies and their 64 bytes after); every surface record `[s, s + 0x120)`. Per frame: the owner word (`h + 8` -> `+0x38`, surfaceLayer); for each record's texture fields (`+0x2c`, `+0x3c`) the object table entry's word, the object's nto2 word, the NTO2 texture, its scroll row `[row, row + 8)`; for each reflective surface the VU constants record (`h + 0x14` -> `+4` -> rec, `rec + 4` -> `data + 0x3c`); the globals 0x1186290 / 0x1186328 / 0x118c180 |
| NTO2 texture `t` | `[t, t + 0x100)` (format, swizzle bits, the LOD words), the pixels `[t + *(t + 0x14), + every level's bytes + 64)`, the CLUT `[t + *(t + 0x18), + 1024)` (decodedTexture samples it whatever the format) |
| 2D | untextured: nothing (the batch is the job's); textured: its texture |
| cloth | its texture, `[object + 0x20, + 0x10)` (the grid is the job's copy) |
| particles | their texture (the particles and the environment are the job's copies) |
| shadow | the chain word, the model's tag word, the tag, the batch `[addr, addr + size + 64)` (the palette / matrix are the job's) |
| model without a plan (the packet walk), the kick pass | everything |

The plan part is what the job itself uses: the draw takes the packets' addresses and sizes from the plan (`job.plan->pk`),
not from guest memory, so those spans are exact for the job and need no guest read (`p4MakePlan`, made once after each
buildPlan). The per-frame part follows pointers (surface -> table -> object -> nto2 -> pixels); it is made at the first
move that needs it in a frame with every guest word it read logged, and taken again (a job handed over after it was made,
a move that met it, the next frame) when those words read the same now (`p4FrameHolds`: the spans are a function of those
words read in that order), else made again. A move that meets a plan's spans marks them to be asked again (`p4Forget`):
what they point at may have moved, and the move's handler relocates pointers to the moved block.

## 2. Why it is exact

1. **Guest memory.** A job the move does not wait for reads no byte of either span. The move writes only the destination
   span, so what that job reads is what it would have read had it run before the move (the full join's order). What the
   game writes after the move because of it (the block at its new place, the freed tail, the next allocations there) is
   inside the spans too; a pointer the handler relocates points into the source span, which the job does not read.
2. **Host state the move changes.** The move serial and the ring of recorded moves (`g_guestMoveSerial`,
   `g_guestMoves` / count, the index): the memos by address read them on the worker (decodedTexture's quick path,
   packetId, rekeyPacketMemo, walkGuestMoves). Every job now carries the serial and the count as they were at its
   hand-over and the worker reads those (`t_moveView`, `moveSerialSeen` / `moveCountSeen`, its own index of the ring
   `g_guestMoveIndexView`). Before P4 a job handed over before a move always ran before it (the move joined the queue) and
   one handed over after, after it: so each job sees exactly what it saw before. The ring keeps 4 096 moves; a move whose
   oldest queued job is 2 048 moves back joins as before (ring guard). With p4move off the view equals the globals.
3. **Order.** The worker runs the jobs in their order; draws reach the renderer in it; the worker's memos change in it.
4. **Nothing else of the move is read by the jobs**: the GC byte count and s16plan's serial are the game thread's; the
   plans' spans are the game thread's (the worker never touches them).

The premise kept from rounds 23 / 32 / 34: what a job reads is left as it is by the game from its hook until it runs (the
PS2's DMA reads it at the kick). P4 makes that window longer by the moves it lets go on; the check below measures it.
Debug modes that read more (SOTC_HOOK_ASYNC_VERIFY, SOTC_STALE_CHECK) keep the whole join.

## 3. How many moves still wait

`[p4move]` lines (p2, the shipped build; Vita3K 222 MHz; in the A/Bs only the on windows count). "Jobs queued" = moves that
found hook jobs still queued (the ones that joined the whole queue before P4).

| run | moves | jobs queued | went on at once | after a partial wait | whole join | queue ran out | jobs left queued per move that went on | the job waited for |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| mixR7 (ride7 seed 7, on windows) | 6 557 | 3 526 | 3 431 (97.3%) | 58 (1.6%; 157.5 ms) | **0** | 37 | 77 | model plan 24, model frame 27, particles 7 |
| mixR13 (ride7 seed 13) | 10 042 | 6 445 | 6 267 (97.2%) | 95 (1.5%; 271.9 ms) | **0** | 83 | 82 | model plan 47, frame 41, particles 7, shadow 2, cloth 1 |
| mixL7 (lake6) | 1 993 | 542 | 514 (94.8%) | 22 (4.1%; 69.0 ms) | **0** | 6 | 56 | plan 3, frame 19 |
| dR (p1, ride7 420 s, all on) | 7 479 | 3 263 | 3 154 (96.7%) | 68 (2.1%) | 0 | 41 | 84 | plan 26, frame 29, particles 16, shadow 1, cloth 1 |

So 95-97% of the moves that used to join the whole queue now go on at once, 1.5-4% wait for part of it (the jobs up to
the newest one whose reads meet the move: a model whose file or texture the move moves, a particle texture), and **none**
needs the whole join (no planless model or kick pass was ever the newest job meeting a move; the ring guard never fired).
What the game thread now spends at a move is mostly the walk: ride7 0.045 ms a frame (550 ms of walk over 12 270 on
frames; 1.9 us a job asked in Vita3K, ~80 jobs a move, half of them particle jobs), the partial waits 0.013 ms a frame.

## 4. Exactness (the check)

`SOTC_P4MOVE_CHECK=1` on build pc2 (= p2 with `-DPS2X_P4_CHECK=1`: SOTC_READ_TRACK on, every guest read of the hook worker
noted): at each move that goes on with jobs queued, main RAM is copied before the move (a copy serves the jobs queued when
it was made; every later move that goes on past them adds its spans to it); each guest read those jobs then make is checked
(a) against the spans of every move that went on past them and (b) byte for byte against the copy (the bytes the job would
have read in the full join's path, where it ran before the move); (c) right after the move, main RAM against the copy
word for word (the destination span against the copy's source). No room for a copy: the move joins (counted; never
happened).

| run | build | frames | moves that went on under check (copies) | jobs checked | guest reads checked | (a) reads in a moved span | (b) differ from the copy | (c) RAM checks / differ |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| chkR (ride7 **900 s**, mounted at frame 5 862) | pc2 | 21 600 | 35 | 4 725 | 455 914 (9.0 MB) | **0** | **0** | 35 / **0** |
| chkA (attract 400 s) | pc2 | 9 600 | 129 | 8 494 | 829 883 (10.9 MB) | **0** | **0** | 129 / **0** |
| chkL (lake6 400 s) | pc2 | 7 200 | 107 | 7 035 | 977 466 (13.1 MB) | **0** | **0** | 107 / **0** |
| c0R (ride7 420 s, mounted at 2 473) | pc (p1's) | 5 100 | 311 | 38 412 | 4 425 059 (36.8 MB) | 0 | 0 | 311 / 0 |
| c0A (attract 300 s) | pc (p1's) | 9 900 | 24 | 1 270 | 267 206 (6.8 MB) | 0 | 0 | 24 / 0 |

On the shipped build: 271 moves went on past 20 254 queued jobs under check, 2.26 M guest reads of those jobs checked,
**0** inside a moved span, **0** bytes different from the full join's memory, 271 whole-RAM compares with 0 words
different. With p1's check build (the same spans, the frame lists made again instead of re-read) 335 more moves, 39 682
jobs, 4.7 M reads, 0 / 0 / 0. Not vacuous; but note the check runs slow the game down, so fewer moves find jobs queued than
in the A/B runs (chkR 35 against mixR13's 6 445).

What the check does not see: a guest read the worker makes without noteRead (the read tracking of round 34; the texture,
packet, surface, palette, shadow and 2D paths have it; raw `g_ram` reads in those paths were audited: each follows a
noteRead of the same bytes). Outputs (draws) are not hashed: with every word read equal to the full join's and the
worker's host state the same (section 2), the draws are a function of the same inputs.

## 5. Speed (Vita3K 222 MHz)

Interleaved A/B, `SOTC_P4MOVE=1 SOTC_AB_FLAG=p4move SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (q.sh / abn1.sh, mixframes.py,
300-frame blocks; ride7 from the mount + 600). Off - on, +- 2 SE.

| run | frame | busy (game thread) | render thread | move joins a frame (off -> on) |
|---|---:|---:|---:|---|
| mixR7 (ride7 seed 7, 800 s) | +0.074 +- 0.141 | +0.116 +- 0.137 | -0.105 +- 0.115 | 0.186 -> 0.059 ms |
| mixR13 (ride7 seed 13, 800 s) | +0.100 +- 0.159 | +0.147 +- 0.150 | -0.064 +- 0.140 | 0.333 -> 0.109 ms |
| mixL7 (lake6 seed 7, 400 s) | +0.067 +- 0.108 | +0.062 +- 0.114 | -0.015 +- 0.106 | 0.049 -> 0.025 ms |
| **pooled (mean of 3, SE = sqrt(sum se^2) / 3)** | **+0.080 +- 0.040 ms (1 SE), z 2.0** | **+0.108 +- 0.039, z 2.8** | | **0.189 -> 0.064 ms (-0.125)** |

The move joins (the [p4move] line's join time by window state: the game thread inside join(k_move), the walk and the
partial waits included) fall by two thirds, and the frame / busy gain matches that size. It is small in Vita3K because
Vita3K's hook worker keeps up (its move joins were 0.05-0.33 ms a frame); the console's were 0.86 / 1.03 ms a frame riding.

## 6. Boot and pictures

bootR (p2, ride7 600 s, `SOTC_P4MOVE=1 SOTC_VITA_EAT_USER_MB=89`): `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, booted first
try, mounted at frame 5 279, rode to frame 18 243 (605 s) with no fault; its `[p4move]` line: 7 066 moves with jobs queued,
6 861 went on at once, 163 after a partial wait, 0 whole joins. Window shots every 120 s, looked at: Wander riding Agro
at the gallop across the plain under the cliffs (t0300, t0540) and along the stone wall (t0420), the reins, the mane and
the tail, Agro's shadow on the ground, the sky normal; dR's shots (p1) the same (the gallop past the tower, t0420).
spalign on p2: **0 sp-based** NEON accesses (of 1 038 with an alignment hint). No `%zu`, no by-value aligned structs,
nothing reads GPU pools; the worker side reads only the job's view of the moves (two words) and, in the check build only,
the copies; it never touches the R5900 context or guest code, and never reads guest memory with a null `g_ram`.

## 7. The console A/B package

- VPK: `build/agent-scratch/p4/celosia-p4.vpk` (= p2.vpk; ELF `celosia-p4.elf`), P4 off unless set.
- Env: `build/agent-scratch/p4/env-ab.txt`: the N6 console env (frame log, PROF129, body shadows, muted logs) with
  `SOTC_AB_FLAG=p4move SOTC_AB_PERIOD=300 SOTC_P4MOVE=1 SOTC_P4MOVE_LOG=600`.
- What to look at: the frame log's `joins move` by window (on windows should drop from ~0.9-1.0 ms a frame riding), the
  `[p4move]` lines (moves with jobs queued, went on at once / partial / whole join, the walk's ms, the move joins' ms by
  state), and frame / busy by window.

## 8. Honest size, and what is left

- In Vita3K the move joins fall by two thirds (0.19 -> 0.06 ms a frame pooled) and the frame by **+0.08 +- 0.04 ms**
  (busy +0.11 +- 0.04). Vita3K's worker keeps up, so its wait was small to begin with.
- On the console the wait was 0.86 / 1.03 ms a frame riding. If the console's moves look like Vita3K's (95-97% go on at
  once, the rest wait for part of the queue), most of that should go; what replaces it is the walk (Vita3K 0.045 ms a
  frame riding; the console's memory is slower, maybe 2-3x) and the partial waits. The A/B package measures it.
- The walk is now the cost: ~80 jobs asked a move, half of them particle jobs whose texture is looked up for each. A per-walk
  memo of a texture's answer (the move's spans are fixed during a walk, the texture's words are too) would cut most of it;
  not done here (it would need the check runs again).
- The 1.5-4% partial waits are moves that really move what a queued job reads (a model file or texture being compacted
  while a model of it is queued): they must wait.

