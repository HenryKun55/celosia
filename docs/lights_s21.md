# S21: the global light query's lights as host records (2026-11-19)

Round S21's brief: the light pass is the largest area no S round redesigned (~2.6% of the game thread riding over
its functions: lightPass 0.70%, ExecGlobalLightMan 0.51% self at ~12 us a call, getAmbientColorC 0.41%, objectLight
0.29%, the light matrices). Redesign the global light query as host records read by its users instead of guest-shaped
work repeated per object, the picture identical.

Switch: `SOTC_S21LIGHT` (A/B name `s21light`; sotc_decomp switch `kS21Light`, after kS19Amb). Check mode
`SOTC_S21LIGHT_CHECK=1`, counts `SOTC_S21LIGHT_LOG=<frames>` (`[s21light]` lines, every 600 frames with the check).
Code: game/overrides/decomp_light.cpp (execGlobalLightMan / execImpl, S21Inv, s21InvQuatPos). Scripts, runs and logs:
build/agent-scratch/s21/ (clone.sh the build dir cloned from S18's, b.sh the build, chk21.sh the check, abs21.sh /
q.sh the interleaved A/B, chainA.sh / chainB.sh started with nohup, vq.out). Own Vita3K trees ../vita3k-celosia-s21 /
-s21b (222 MHz, 3 cores, muted), deleted at the end.

## 1. What the query did per object

ExecGlobalLightMan (0x10f8728, native since round 41) is called once per lit object whose colours round 83's cache
(walk83) cannot give back - the moving ones: Wander, Agro, the colossus parts, ~14 a frame riding - plus once a
frame for the camera (SetCameraReverbFlag). Per call: the three directional records kept aside, the ambient colour
(getAmbientColorC, a guest call), then for every global light of the work area's list (+0x5c4, +0x510 of them):

- enabled (+0x144)? its squared distance to the object (+0x128 written) against its range (+0x118);
- in range: the light's volume matrix built - GetInverseMatrixFromQuaternionPos of the light's quaternion (+0xd0)
  and position (+0xc0) into the work area's +0xb0 - then the cube (native) or ellipsoid (guest call) rate of the
  object's point against the inner / outer boxes, the weight at +0x11c, the light kept in the +0x540 list;
- then the list sorted and blended (blendLights).

Every light was read through Mem::p (a scratchpad test per word), and the inverse matrix was built again for every
object in range of the light, although it depends only on the light's 32 bytes, which the game sets once a frame.

## 2. What S21 changes

- **One host record per light slot** (S21Inv, 32 slots, by the light's index in the list): the light's address, the
  quaternion / position bytes and vf0 it was made from, the matrix, and every register the routine leaves
  (vf1-vf4, vf8, vf9, vf11, vf14-vf16 whole; vf10 / vf12 / vf13 / vf17 / ACC xyz - their w lanes pass through from
  the caller's registers in the original, so they are taken from the live ones; r8-r15; v0 / a1 = sp - 0x50). A
  record whose light and 32 input bytes are the slot's now is used (the matrix stored to +0xb0, the registers set);
  any other is made again by the native routine. Exact by construction: the routine is a pure function of those
  bytes and the pass-through lanes.
- The light's words through one host pointer (16-aligned, the whole 0x148 bytes in reach: the bytes Mem::p gives).
- Everything else as before: the ambient colour, the cube / ellipsoid rates, the sort, the blend, every store
  (+0x11c, +0x128, the frame's boxes), in the same order.

The original's per-object, per-light writes (+0x11c, +0x128) stay: they are state (an untested light's +0x11c is
multiplied by its +0x13c again on every call) and blendLights reads them.

The check also showed what the query costs riding: ~10-14 calls a frame (one per lit object the walk83 cache misses,
plus the camera's), ~5-9 global lights tested for range per call, ~0.6 in range (riding: chkRp1 146 643 calls, 737 294
lights, 92 689 in range); at the lake 3.7 in range a call. The inverse matrices kept: 99.9% (92 640 of 92 689 riding,
412 360 of 412 390 at the lake, 110 012 of 110 126 in the attract demo): the lights' 32 bytes change only when a light
is set up.

## 3. Equivalence (`SOTC_S21LIGHT_CHECK=1`)

Each call is first run the old way (S21's paths off), its results kept, the state put back (the context, the work
area's 0x644 bytes, the frame and 0x400 bytes of stack below it, every light record of the list), then run the S21 way
and compared: the work area, the frame (0xa0 bytes), every light record (0x148 bytes), every GPR (128-bit, but $zero),
hi / lo, every FPR, fcr31, VU0 vf1..vf31, ACC, Q, pc. A difference keeps the old way's results and is printed (the
first 8).

| run (p1) | calls checked | lights | in range | matrices kept | differences |
|---|---|---|---|---|---|
| chkRp1, ride7, 600 s (mounted at 2 311, to frame 11 562) | 146 643 | 737 294 | 92 689 | 92 640 | **0** |
| chkLp1, lake6, 420 s (to frame 7 807) | 111 618 | 597 934 | 412 390 | 412 360 | **0** |
| chkAp1, attract, 420 s (to frame 13 200) | 97 589 | 152 692 | 110 126 | 110 012 | **0** |

355 850 calls in all, 0 differences in memory or in any register. The check's shots (ride7 / lake6 / attract) were
looked at: normal.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s21light SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

p1, `SOTC_S21LIGHT=1` (the switch on in the on windows: the `[ab]` lines say "s21light off in odd windows");
frames >= 200 ms left out (the locked screen's ~1 s host stalls); ride7 from the mount + 600 frames.

| run | route, length | from frame | off - on, frame (2 SE) | busy | blocks |
|---|---|---|---|---|---|
| mixR7p1 | ride7, 800 s, seed 7 | 6 628 | +0.265 +- 0.227 ms | +0.227 +- 0.181 | 31 |
| mixR11p1 | ride7, 800 s, seed 11 | 9 432 | +0.043 +- 0.339 | -0.052 +- 0.209 | 18 |
| mixR13p1 | ride7, 800 s, seed 13 | 3 629 | +0.117 +- 0.178 | +0.173 +- 0.168 | 45 |
| mixL7p1 | lake6, 400 s, seed 7 | 3 000 | +0.070 +- 0.189 | +0.081 +- 0.187 | 18 |

Pooled (inverse variance): all four **+0.127 +- 0.053 ms (1 SE), z 2.4**; the three rides +0.154 +- 0.065, z 2.4.
All four on the same side. (Seed 11 mounted late, at frame 8 832: only 18 blocks.)

What the sampler says, honestly: the part S21 takes out is small. In mixR7p1 GetInverseMatrixFromQuaternionPos's
routines (invQuatPos 0.016%, inverseMatrix 0.012%, both states mixed) against 0.10 + 0.065% in S19's profile of
the same route: ~0.03% of the game thread, ~0.01 ms a frame; the rest of the query's own time (execImpl 0.53-0.61%,
both states, spread evenly over the whole function, no hot spot) cannot be split by state. So the frame A/B's
+0.13 ms is more than the sampler can account for; the switch is exact either way, so it is on by default by the
round's rule (z > 2 overall), with this caveat written down. `SOTC_S21LIGHT=0` turns it off; the console's A/B can
re-measure it.

The query's code moved out of the hot text with the rename (execImpl, a new static function, landed at 0x8440_8ee8,
outside vita_hot_order.ld's list): p2 adds it to the list next to execGlobalLightMan (0x810afe18 in p2). The A/Bs
above are p1's.

## 5. Boot, pictures, alignment

bootRp1 (p1, S21 on, ride7 route, 600 s, `SOTC_VITA_EAT_USER_MB=89` taken): booted on the first try, no fault, but
Wander did not mount: the load ran later than in the other runs (the player still at 0 at frame 1 200), the teleport
caught Wander mid-climb (other object addresses than in chkRp1), and the frame-clocked input script went on without
him on Agro - the route's timing, not the lights (the query's results are the old ones bit for bit). bootRp1b, the same
again: mounted at 2 289, rode to frame 13 296, no fault, 198 445 calls the S21 way. Its shots (every 60 s: the plain,
Wander on Agro at the gallop, the cliffs) normal: ground, Wander, Agro, shadows, lighting.
bootRp3 (p3: S21 on by default, no SOTC_S21LIGHT in the env, the hot-order line; 89 MB taken): booted on the first
try, mounted at 2 222, rode to frame 13 822, no fault, 195 886 calls the S21 way; shots normal (Wander on Agro by the
cliffs, ground, shadows).
spalign: 0 sp-based :128 / :256 NEON accesses on p1, p2 and p3 (of 983 with an alignment hint).

## 6. What it leaves

1. **The query's own time**: ~0.5% of the game thread, ~12 us a call, flat over the function on Vita3K: the three
   records kept aside and put back, the ambient brightness, the 0x150-byte copy or clear, the sort, the guest calls
   (getAmbientColorC, the ellipsoid rate, the blend) through the table. A host light record per *object*, kept while
   the object's position and the lights' signature stay (round 83 does that for static objects with a tolerance), is
   what is left; riding the ~10-14 calls a frame are moving objects, so a per-object record would be remade each frame.
2. **SetWorkFromGlobalLight / ExecGameLight / the light matrices** after the query (~0.3% together), and
   getAmbientColorC (S19's host walk, still off).
3. The light pass's own walks (lightPass 0.70%: three walks of the sort list).
