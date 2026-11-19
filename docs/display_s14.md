# S14: the model hook's bone palette as a host record, not read back out of the VIF chain (2026-11-19)

Round S14's brief: S10 / S12's section 6, item "the model hook's VIF / VU1 reads". The hook (modelDispHook's lean path)
and the skinned shadow (shadowCapture) read each skinned model's bone palette back from the instance chain's second
packet: readChainPaletteLean walked the packet's VIF codes twice (once for the range, once for the copy), then copied
every qword with a bounds check, a read note and a prefetch. A small, fast round: one part.

Switch: `SOTC_S14HOOK` (A/B name `s14hook`; sotc_decomp switch `kS14Hook`, the last of the tables, **off by default**:
on with `SOTC_S14HOOK=1`, see section 4). Parts: `SOTC_S14HOOK_PARTS=<mask>` (bit 0 `palette`; default 1). Check mode
`SOTC_S14HOOK_CHECK=1`, counts `SOTC_S14HOOK_LOG=<frames>` (`[s14hook]` lines, default every 600 frames with the check).
Code: game/overrides/s14hook.h (the record, its layout, begin / finish / take / learn), decomp_skinpal.cpp (dispSkinned
writes the rows), sotc_native.cpp (the hook's and the shadow's take, s14Check). Scripts, runs and logs:
build/agent-scratch/s14/ (q1-q4.sh the queues, abs14.sh the A/B, chk14.sh the check, vq.out).

## 1. The profile (ride7, Vita3K 222 MHz)

Base: S13's sampler run profR0 (ride7, the S12 build plus S13's WIP, game thread ~30 ms a frame) and S12's profB0. The
model hook and its reads riding: modelDispHook 1.44-1.49%, readChainPaletteLean 0.42-0.57%, readChainPalette 0.19%
(profB0), planChecked 0.17-0.18%, queueNextDraw 0.24%, hookScrolls127 0.21%, mdsPlanPackets (lane work joined) up to
0.56%. dispSkinned (decomp_skinpal::skinned, the palette's writer) 0.29%. The lights are a 128-byte block copy
(guestBlock, inlined into the hook): nothing to win there, so S14 is the palette only.

What a skinned palette costs riding: ~10-12 dispSkinned calls a frame (Wander, Agro, their parts), each followed by its
model hook and its shadow reading the same rows back (chkRp1: 72 200 palettes written, 110 107 hook takes, 59 934 shadow
takes over 21 000 frames).

## 2. What changed (`SOTC_S14HOOK`, part `palette`)

Before: dispSkinned's native loop wrote each bone's four rows (camera x bone matrix) into the palette block (model
+0x104); the chain's second packet carries that block to VU1 with UNPACKs; the hook and the shadow then parsed those
UNPACKs out of guest memory into the job's palette (qwords lo..hi of the VU picture, zero where no UNPACK wrote, later
UNPACKs over earlier ones).

Now: each instance chain has a host record (s14hook::Chain, keyed by the chain's first tag): the VU range lo..hi, the
packet's address and size, the list of the packet's VIF code words (address, value), per bone the row in the record its
four qwords land at, the packet's other qwords (not bone rows) with their guest address, and the rows themselves.
- **dispSkinned** (native path): before the loop, `begin` checks the record against the chain as it is now (the tag's
  id, the packet pointer and size, every code word, the palette block and the bone count); when it holds, the loop
  writes each bone's four rows into the record as well as into the guest block (four unaligned NEON stores), then
  `finish` copies the few non-bone qwords from the guest and stamps the record with the display frame. Every call of
  dispSkinned first invalidates the chain's record (also the recompiled fallback paths), so a record is valid only when
  this frame's native loop wrote it.
- **The hook / the shadow**: `take` gives the record when it was written this frame; the job's palette is one copy of
  host floats, lo / hi / the packet / its size from the record. Otherwise the chain is read as before, and if
  dispSkinned had asked for it, the layout is learned from that read (`learn`: the same walk as readChainPaletteLean;
  a layout where a bone's rows are not four consecutive VU qwords carried once, or data outside main RAM, is marked
  unusable and keeps the old read).

No guest packet list is changed: the guest palette is still written (the kick's copy, S4's elision and anything else
that reads it see the same memory).

## 3. Equivalence

`SOTC_S14HOOK_CHECK=1` (chk14.sh): at every take (the hook and the shadow) the chain is also read the old way
(readChainPaletteLean) and lo, hi, the packet address, its size and every float of the palette are compared; the old
way's values are used.

| run | build | route | frames | palettes written | takes checked (hook + shadow) | layouts learned (unusable) | differ |
|---|---|---|---:|---:|---:|---:|---:|
| chkRp1 | p1 | ride7 (700 s, mounted at 4 297) | 21 000 | 72 200 | 170 041 | 105 (0) | **0** |
| chkLp1 | p1 | lake6 (450 s) | 7 800 | 19 038 | 43 713 | 36 (0) | **0** |
| chkAp1 | p1 | attract (450 s) | 14 400 | 40 123 | 92 821 | 28 (0) | **0** |

306 575 takes over the three routes, 0 differences, no unusable layout. spalign: 0 sp-based NEON :128 / :256 accesses
(of 971 with an alignment hint, p1 and p2). p2 = p1 with the switch off by default and the log's "read the old way"
counting palettes only (the shipped build). No function-table wrapper (dispSkinned is already decomp_skinpal's native).

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s14hook SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

abs14.sh, 400 s a run, ride7 repeated at 800 s (the 400 s windows were unclear); frames from the mount + 600 (ride7), from frame 3 000
otherwise; tools/mixframes.py (300-frame blocks, +- 2 SE). Positive = on is faster. The machine was loaded by other
rounds' builds and runs throughout (ride7 ~37-39 ms a frame against ~30 normally), so the windows are short and noisy.

| run (build, seed) | route (window) | frames | **frame off - on** | game busy off - on |
|---|---|---:|---|---|
| mixR7p1 (p1, 7) | ride7 (mount 2 777 + 600) | 3 697 | **-0.207 +- 0.290** | -0.225 +- 0.291 |
| mixR11p1 (p1, 11) | ride7 (mount 4 286 + 600) | 2 403 | **+0.120 +- 0.320** | +0.194 +- 0.304 |
| mixR7p1L (p1, 7) | ride7, 800 s (mount 3 562 + 600) | 10 754 | **+0.058 +- 0.200** | +0.147 +- 0.161 |
| mixR11p1L (p1, 11) | ride7, 800 s (mount 3 563 + 600) | 11 044 | **+0.137 +- 0.210** | +0.151 +- 0.236 |
| mixL7p1 (p1, 7) | lake6 | 4 645 | **+0.092 +- 0.409** | +0.103 +- 0.402 |
| mixL11p1 (p1, 11) | lake6 | 5 014 | **+0.016 +- 0.177** | +0.007 +- 0.172 |
| mixA7p1 (p1, 7) | attract | 10 284 | **-0.061 +- 0.130** | -0.012 +- 0.123 |
| mixA11p1 (p1, 11) | attract | 10 217 | **+0.063 +- 0.186** | +0.086 +- 0.163 |

The sampler cannot split readChainPaletteLean by state (rigid chains still call it in both states, returning at the
packet size), so its share is compared with the base instead: riding readChainPaletteLean 0.57% (profR0, all frames
off) -> 0.28-0.33% in the mixed runs (half the frames on), i.e. the on-frames' skinned palette reads are gone
(~0.17 ms a frame at 30 ms); dispSkinned 0.29% -> 0.26-0.30% (the extra stores are lost in the noise). At the lake
0.21-0.22%, in attract 0.58-0.59% (attract's skinned characters are fewer and its rigid chains many).

**Decision: `SOTC_S14HOOK` off by default (on with `SOTC_S14HOOK=1`).** The rule is z > 2 in both seeds riding; the
frame time's A/B is even everywhere (ride7 at 800 s +0.06 +- 0.20 / +0.14 +- 0.21 ms, z 0.6 / 1.3; every run within
+- 0.21 ms), as the expected ~0.17 ms is below what a
400-800 s run on a loaded machine resolves. The part is exact and ready; the console's A/B (env-console with s14hook)
can judge it (s14hook in SOTC_AB_FLAG) where the guest reads cost real memory latency.

## 5. Pictures

Vita3K 222 MHz, muted, window shots looked at (Wander, Agro, the cloths, the ground, the shadows), then the A/B and
check runs' shots and sampler snapshots deleted.
- **bootRp2** (p2, the shipped build, with `SOTC_S14HOOK=1`, ride7, `SOTC_VITA_EAT_USER_MB=89`, 600 s): mounted at frame
  3 082, 11 598 frames, 6 game frames over 300 ms (all in the boot / area loads, before the mount), No exact 0; 43 887
  palettes written, 63 783 hook and 32 362 shadow takes, 10 496 palettes read the old way (the first frames of each
  chain and the chains dispSkinned does not write). Wander galloping on Agro past the shrine's wall, the plain, the
  shadows on the ground, the cloths: normal.
- **The A/Bs' shots** (each frame on or off at random): Agro and Wander at the shrine's gate (mixR11p1), Wander
  swimming at the lake (mixL7p1): normal.
- **The checks**: the gallop on the plain past the pillars with Agro's mane and tail, Wander's mantle (chkRp1): normal.

## 6. What still ties the draw to the PS2

1. **The plans** checked against the guest packet lists (planChecked 0.17%, packetListHash one frame in 16 per header):
   a host model cache filled at model load (modelInitDispModelClass), keyed by header, with no per-frame checks.
2. **The palette's guest copy**: dispSkinned still writes the guest block because the kick's copy (kickCopy for the
   chains S4 does not elide) and S4's paletteKeep read it; with the kick taking the host record too, the guest write
   could go (and the skinned shadow's job could share the hook's rows instead of copying them).
3. **The lights**: objectLight writes the camera-space lights into the instance's own packet (+0x10..+0x90), the hook
   copies 128 bytes; cheap, but the same shape: a host record would remove the guest round trip.
4. The rest of S12's list: the immediate quads' VU-exact arithmetic, the strips' segment walk, the lane's mirror of
   modelDisp, the texture records' GS transfer state, the layer stack / gsReg hooks.
