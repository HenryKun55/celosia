# S23: the light pass's later walks over a host list (2026-11-19)

Round S23's brief: finish the light pass as host code. S21 left the light pass's three walks (lightPass 0.70% of the
game thread riding), SetWorkFromGlobalLight / ExecGameLight / the light matrices (~0.3%) and the global light query's
own time (~0.5%). Profile riding, rewrite the biggest as host code / data, exact.

Switch: `SOTC_S23LIGHT` (A/B name `s23light`; sotc_decomp switch `kS23Light`, after kS21Light; **off by default**: on
with `SOTC_S23LIGHT=1`, see section 4; the A/B runs need `SOTC_S23LIGHT=1` besides `SOTC_AB_FLAG=s23light`). Check mode
`SOTC_S23LIGHT_CHECK=1`, counts `SOTC_S23LIGHT_LOG=<frames>` (`[s23light]` lines, every 600 frames with the check).
Code: game/overrides/decomp_light.cpp (lightPass: inlineClass, s23Nodes, s23Walk, s23EndIndex, the check's s23See /
s23Verify). Scripts, runs and logs: build/agent-scratch/s23/ (clone.sh the build dir cloned from S21's, b.sh the
build, prof.sh / profA.sh the profile, tools/pclines.py the per-function sample split, chk23.sh the check, abs23.sh /
q.sh the interleaved A/B, chainA.sh / chainB.sh / chainC.sh started with nohup, vq.out, NOTES.txt). Own Vita3K trees
../vita3k-celosia-s23 / -s23b (222 MHz, 3 cores, muted), deleted at the end.

## 1. The profile (ride7, Vita3K 222 MHz, S21's p3: the same code as 166b4ed without D20's lifts)

profR0: 120-660 s, 11 873 frames, the game thread 34.92 ms a frame (a loaded machine: D21's Mac runners and builds
beside it). The light pass's functions (self time, share of the game thread):

| function | % | what it is |
|---|---|---|
| decomp_light::lightPass | 0.724 | the pass's three walks of the sort list (native since round 41; 19 KB, samples flat over it) |
| decomp_light::execImpl | 0.516 | the global light query (S21) |
| decomp_ambient::impl | 0.405 | getAmbientColorC (S19's host walk is off) |
| decomp_light::objectLight | 0.277 | a layout object's light matrices (010F4A18) |
| decomp_hot43b::calcLightLayoutObj | 0.248 | the layout objects' light callback (round 43 / 83's cache) |
| decomp_light::lightMatrices | 0.125 | the normal and colour light matrices (010F5F60) |
| decomp_light::blendLights | 0.107 | the in-range lights' blend |
| decomp_light::setWorkImpl | 0.106 | SetWorkFromGlobalLight |
| decomp_light::normalLightMatrix | 0.089 | iosMakeNormalLightMatrix |
| decomp_hot43b::colMan83 | 0.081 | round 83's colour cache |
| lift_d1::SetGeoDispClassLightColMan | 0.079 | the per-object colour manager (D7 lift: four guest calls) |
| sotc_decomp::walk83ParticleLight | 0.076 | the particles' light through round 83's cache |
| lift_d1::ExecGameLight | 0.059 | the game lights (D4 lift) |

All of them: ~3.2%. The biggest single item is lightPass. Its samples are spread evenly over the whole function (no
64-byte block above 3.2% of its samples): the cost is the walks themselves, node by node.

## 2. What lightPass did per frame, and what S23 changes

The pass (manager_sub_010F4FB8) walks the frame's sort list - every object the display sorted, **~130 nodes a frame
riding** (the check runs' counts) through ~1 000 buckets - three times:

1. each node's light callback (CalcLightLayoutObj for the layout objects, the particles' through round 83's cache);
2. each layout object's own lights (010F4A18) when it has no light parent, then its scale on its matrices' +0x80;
3. each layout object with a light parent (+0x198): the parent's lights copied over.

Walks 2 and 3 act only on nodes whose GetLayoutClass is nonzero, **~72 of the ~130 riding** (38 of 90 at the lake,
30 of 44 in the attract demo); for every other node they read
the node, its id and type byte and the class table, and go on, touching no memory and no register (the class is 0
from the inline cases). Both also scan the bucket array again.

S23 (`SOTC_S23LIGHT`, A/B `s23light`):

- the first walk keeps, in the list's order, every node the later walks do not pass over at once: a nonzero class
  from the inline cases, or a class the guest GetLayoutClass gives (the script / camera / light objects' cases: those
  nodes make the guest call again in walks 2 and 3, as in the original);
- walks 2 and 3 run over that host list, each node's id, class and fields read live as before (the same bodies: the
  same calls, stores and registers in the same order);
- the sort list's index word (+16), which a full walk leaves at its end (the count when a bucket holds a node, count - 1
  when none does, untouched for a count <= 0), is stored after each walk as the full walk leaves it;
- round 95's join adds the held-back particle geodisps to the list after the first walk: when one of them is not a
  plain class-0 node, the pass runs the full walks (never seen: they are geodisps, type 0x24).

## 3. Equivalence (`SOTC_S23LIGHT_CHECK=1`)

The callbacks of walks 2 and 3 change state that cannot be put back and run twice (the scale is multiplied in place),
so the check runs the walks the old way (the frame's results are the original's) and compares what S23 relies on:
the nodes the old walks do not pass over at once, in order, against the host list, and the index word each walk leaves
against S23's end value. Equal, the S23 walks call the same bodies on the same nodes in the same order (the nodes
passed over touch nothing).

| run (p2) | passes | walks checked | nodes a pass | kept a pass | of them guest class | differences |
|---|---|---|---|---|---|---|
| chkRp2, ride7, 600 s (mounted at 5 223, to frame 12 728) | 11 656 | 23 310 | 130 | 72 (55%) | 4.7 | **0** |
| chkLp2, lake6, 420 s (to frame 9 159) | 8 091 | 16 180 | 90 | 38 (42%) | 3.9 | **0** |
| chkAp2, attract, 420 s (to frame 16 200) | 15 719 | 31 436 | 44 | 30 (68%) | 2.3 | **0** |

71 926 walks in all, 0 differences; round 95's join never added a node that is not class 0 (0 fallbacks). The first
build (p1) fell back to the full walks whenever a node's class took the guest call, which is every frame (a script /
camera / light object is always in the list): p2 keeps those nodes. So walks 2 and 3 now visit 55% of the nodes riding
(42% at the lake, 68% in the attract demo): ~116 node visits a frame saved riding, and two scans of the bucket array.
The check's shots (ride7 at the gallop, the lake, the attract demo's bridge) were looked at: normal.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_AB_FLAG=s23light SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>`)

p2, `SOTC_S23LIGHT=1` (on by p2's default then too; the `[ab]` lines say "s23light off in odd windows"); frames
>= 200 ms left out (the locked screen's ~1 s host stalls); ride7 from the mount + 600 frames.

| run | route, length | from frame | off - on, frame (2 SE) | busy | blocks |
|---|---|---|---|---|---|
| mixR7p2 | ride7, 800 s, seed 7 | 2 805 | +0.144 +- 0.322 ms | +0.220 +- 0.328 | 65 |
| mixR13p2 | ride7, 800 s, seed 13 | 2 814 | -0.041 +- 0.163 | +0.063 +- 0.147 | 57 |
| mixR11p2 | ride7, 800 s, seed 11 | 6 382 | -0.167 +- 0.432 | +0.042 +- 0.318 | 44 |
| mixL7p2 | lake6, 400 s, seed 7 | 3 000 | +0.605 +- 1.005 | +0.648 +- 1.327 | 22 |

Pooled (inverse variance): **-0.008 +- 0.068 ms (1 SE), z -0.1**; the three rides -0.020 +- 0.069. Even.

What there is to see, honestly: the walks S23 shortens are small work. Riding it saves ~116 node visits a frame (a
node's word, its id's type byte and the class table) and two scans of the ~1 000-word bucket array: a few thousand
guest-free host instructions, ~0.02-0.04 ms a frame on Vita3K's A9 at 222 MHz, below what the frame A/B resolves
(+-0.07 ms pooled). The sampler cannot split it either: lightPass is one function in both states (0.789% of the game
thread in mixR7p2, 0.195 ms a frame, against 0.724% / 0.253 ms in profR0, a different machine load). **Off by default**
(sotc_decomp.h's off list beside S12 / S14 / S16 / S19): exact, so the console's A/B can try it (`SOTC_S23LIGHT=1`).

The default was set after the A/Bs, in sotc_decomp.h's off list only (no rebuild: the same p2 code with the switch read
the other way; a syntax check of decomp_light.cpp, which includes it, passed).

## 5. Boot, pictures, alignment

bootRp2 (p2, S23 on, ride7 route, 600 s, `SOTC_VITA_EAT_USER_MB=89` taken): booted on the first try, mounted at frame
3 415, rode to frame 13 894, no fault; 12 845 passes the S23 way, 0 fallbacks. Its shots (every 60 s: Wander on Agro at
the gallop over the plain, the ravine and the ruins) normal: ground, Wander, Agro, shadows, lighting. The lightPass
code stays in the hot text (0x81060c68, its place in vita_hot_order.ld); the two small helpers the compiler kept out of
line (the end-index lambda, called twice a pass, and the check's verify) landed outside it, which costs nothing
measurable. spalign: 0 sp-based :128 / :256 NEON accesses on p1 and p2 (of 1 001 with an alignment hint).

## 6. What it leaves

1. **The light pass's first walk** (all ~130 nodes riding, each node's callback: CalcLightLayoutObj for the layout
   objects, the particles' through round 83's cache) and the callbacks themselves: calcLightLayoutObj 0.25%, objectLight
   0.28%, colMan83 0.08%, walk83ParticleLight 0.08%. lightPass's own ~0.7% is spread evenly over the function: the guest
   calls' machinery (11 inlined callGuest sites, the table, the register traffic), not the walks' reads. A host pass
   that calls the native callbacks directly (no table, no ctx round trips) is the next step there.
2. **The global light query** (execImpl 0.52%) and getAmbientColorC (0.41%, S19's host walk off): per lit object,
   ~10-14 a frame riding.
3. **The work records** (SetGeoDispClassLightColMan's D7 lift calling ExecGlobalLightMan, SetWorkFromGlobalLight,
   ExecGameLight's D4 lift and the light matrices through the table: ~0.55% with normalLightMatrix and blendLights): one
   host function for the whole per-object colour manager would take the four guest calls and their register traffic out.
