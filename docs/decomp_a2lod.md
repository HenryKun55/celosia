# Round A2-lod: charlod78 made safe to turn back on (SOTC_CHARLOD78=1, still off by default) (2026-11-27)

Item 2 of docs/arch_plan.md: characters nobody sees skip geometry updates. Round 78's charlod78 did that and was turned off on
2026-11-07 (ebe018f, together with ptcl79, seg80, probe84 and ground85) because Avion's intro on the console "only showed part
(no full reveal)". A later console bisect cleared seg80, probe84 and ground85. This round finds what was wrong with
charlod78, fixes it, audits every put-off against the picture, and measures it. (The name "A2" belongs to an older round; this
one is "A2-lod" everywhere.)

Code: `game/overrides/decomp_charlod78.cpp` (and W1's elf pad in `decomp_walk.cpp`). Base: main c6fbbf5 (the runs up to
p9 were on 0d923a6; p13 / p14 on c6fbbf5). Scratch, runs, package: `build/agent-scratch/a2/` (chain2.sh, win.py,
tcheck.py, p2sum.py; own Vita3K tree `../vita3k-celosia-a2`, deleted at the end). Vita3K 222 MHz, 3 cores, muted,
`SOTC_VITA_EAT_USER_MB=89`, the console card, all rounds through Z2 on (the env of `all/env-all-on.txt` plus brackets).

## 1. What was wrong with round 78's rule

Round 78 put a character's update off (its time kept in the class's +0x1164, as the game's own in-between frames do) until
0.25 s (radius < 0.3) or 0.12 s had gathered, when its position with a margin of 2r+1 was outside the noted camera's clip
volume this frame and the last. Never: the player, within 12 of the player, radius > 2, the active colossus.

Four faults, each seen in the logs (`SOTC_CHARLOD78_LEGACY=1` = round 78's rule exactly):

1. **The active colossus was never excluded.** The test compared with 0x1465AD4, which holds bossStatGetActiveBoss's boss
   *table record*, never an object. Avion's collision sphere radius is **0.05** (`[charlod]` line: `r 0.05`), so Avion was
   treated like a bird: in avLegF (the lake, legacy rule) the boss object is `lod` and **put off 247-262 of 300 frames**
   (updated 38-53 times in 300 frames, ~1 update in 6 frames), and drawn stale. The real handle is 0x1465908 (as the player's
   0x1465900): the new rule reads it, and Avion is `full`, `held 300` (avOnB).
2. **The view test was too tight.** A margin of 2r+1 is 1.1 units for a bird (r 0.05) or a lizard (r 0.00), at the exact screen
   edges, with a 1-frame hold: wings, tails and Avion's whole body reach far past it. Legacy at the shrine (wkLegF): **4 433
   stale** (put off on a frame whose drawn camera saw them within 6 units), only 7 of them at camera cuts, e.g. `stale frame
   1206: r 0.00 put off 3 in a row; the drawn camera moved 0.03, turned cos 1.0000` (a steady camera). That is the "frozen
   animal coming back into view".
3. **The player's place was wrong while riding or swimming.** The "within 12 of the player" rule used the player object's
   +0x170, which stops following Wander while he rides (ride7: 460 units off) or swims (lake: 1 353): Agro and the small
   character that rides along (r 0.00, 0.3 from Wander) became "far from the player" and eligible. A2-lod keeps the place
   Wander's own character tail gives (+0x134 of his class, the same source as everyone's) and uses the nearer of the two; if
   it is not known this frame, nothing is far.
4. **Behind the camera a widened test narrows.** (A2-lod's own first version.) Scaling the x / y planes by k > 1 widens the
   frustum in front of the camera but tilts the planes behind its apex: a small character 6 units from the camera failed the
   k test and was drawn stale twice (rdOnE frames 16 769-16 770). Now: within the view radius of the camera = seen, and the
   k = 1 test is made too.

5. **Agro stopped coming back.** (Found in this round's pictures and per-character lines.) At the shrine with no input,
   Agro's own AI walks him out to ~50 units and back to Wander (3-4 units) about every 2 000 frames (wkOffE, wkOffJ). With
   Agro put off at round 78's 0.12 s (any view rule: wkLegF legacy, wkOnE / wkOnH / wkOnI / wkOnJ A2-lod), he walks out to
   ~50 units **and stays there for the rest of the run** (put off ~190-225 of every 300 frames), although the time check
   passes (the time handed to him equals the world's). A gameplay change, the "animal stopped" kind the user remembers.
   A2-lod puts off only characters with a collision radius below 0.3 (birds, lizards, the small animals): radius >= 0.3
   (Agro) is never put off (`SOTC_CHARLOD78_OFF_BIG` now 0 = never; the legacy rule keeps 0.12). With that (p14, wkOnK) Agro
   walks out and back as with the switch off (3.4 -> 53.8 -> 3.4 units, cycling). The small animals keep moving with or
   without the rule (their camera distances vary window to window in wkOnH as in wkOffE).

### The Avion reveal and the bisect

- The intro could not be replayed in Vita3K. `2000:boss4` (the shore of Avion's lake) plus the lake swim never plays it
  (avOnA); `bdemo` (bossStatStartDemo) at 2 700 does nothing visible (avOnB); `bootflag4` shows the boss bar but skips the
  intro (it marks the colossus woken, game flag 67; avOnC); a teleport to Avion's home place (-1 925, 59, -310, another area)
  drops Wander into the void, game over (avOnD). D16's "b4 / ch2 / a4" intro route is not recorded anywhere.
- What the logs do show: under the legacy rule the active colossus Avion is put off ~5 frames in 6 whenever the noted camera
  does not hold its 0.05-radius root (fault 1), and a big body on a tiny root sphere fails the 2r+1 test whenever its root is
  off screen while its wings are not (fault 2). Avion's intro is script-timed on its motion; an Avion updated at ~5 fps with its
  time handed over in 0.25 s lumps is the "only shows part" the console showed. This is the mechanism, not a replay.
- **0x1465AC8 (getActivedemoStage) and 0x1465AC0 (chkActiveDemoStart bit 0) never changed in any run** (`[charlod] watch`
  lines: only the boss handle moves), including the letterboxed scene after Load Game (ldOnG). So "a demo is running" is
  **not** what these words show for the scenes reached; the rule keeps them (harmless) but the protection comes from the
  colossus exclusion, the wide view test and the cut guard, not from them.
- **ptcl79 alone** (off-screen particle systems stepped at a lower rate, not drawn) could not be run against the intro for the
  same reason. It touches particle systems only, never a character's motion or a script's wait, so it cannot stop a colossus'
  reveal; but this round has no replay that clears it.

## 2. The A2-lod rule (decomp_charlod78.cpp)

A character's update is put off only when all of these hold (else it updates as the game decides):

- not the player, not the active colossus (handle 0x1465908), collision radius below 0.3 (fault 5: Agro and anything bigger
  never), more than 12 from Wander (fault 3's place), and the player known;
- more than 15 units from the camera (`SOTC_CHARLOD78_CAM_NEAR`, p13: a small creature near the camera is moved by its AI
  while its pose waits and can cross the screen's edge within a frame or two);
- not seen in the last 8 frames (`SOTC_CHARLOD78_HOLD`), where "seen" is: within the view radius of the camera, or the sphere
  of radius max(2r+1, 6) (`SOTC_CHARLOD78_MINVIS`) inside the clip volume widened to 1.35 x the screen (`SOTC_CHARLOD78_K`) or
  at 1 x, for the drawn camera (`g_cameraContextAddr`) or the camera context the game holds now (0x118FE4C's index into
  0x122A8F0);
- no camera cut and no demo end in the last 30 frames (`SOTC_CHARLOD78_GUARD`): a cut is the drawn camera moving more than 2
  units (`_CUT_MOVE`) or turning more than ~18 degrees (`_CUT_COS` 0.95) between two frames, a context switch, or a frame
  without character tails before this one;
- 0x1465AC8 zero and 0x1465AC0 bit 0 clear.

Unchanged from round 78: the put-off keeps the time in +0x1164 (the next update advances by all of it), up to 0.25 s
(`SOTC_CHARLOD78_OFF`). `SOTC_CHARLOD78_LEGACY=1` restores round 78's rule for comparison.

The audit (`SOTC_CHARLOD78_LOG=<n>`): every n frames a `[charlod] frame` line (stale this window / at a cut; all so far; put
offs; catch-ups; cuts), an `entries` line (characters entering the drawn picture after a put-off in the last 30 frames, and
the least number of frames between the last put-off and the first frame drawn), per character lines (as round 78, plus stale,
held, longest put-off run), and one-off lines: `stale` (with the drawn camera's move), `entry` (A2-lod p9), `watch` (the demo
words and the boss handle when they change).

## 3. Exactness: stale, entries, time

"Stale" = put off on frame N and inside the camera that drew frame N (radius max(2r+1, 6), screen k = 1), i.e. drawn with an
old pose. The target is 0.

| run | build | route | put-offs | stale | of them at a cut | not at a cut |
|---|---|---|---:|---:|---:|---:|
| wkLegF | p6, **LEGACY** | shrine (in_shrine.txt) 600 s | 85 399 | **4 433** | 7 | **4 426** |
| avLegF | p6, **LEGACY** | lake6 (`2000:boss4`) 300 s | 8 982 | **436** | 5 | **431** (Avion among them) |
| wkOnE | p6 | shrine 600 s | 63 637 | 4 | 4 | 0 |
| rdOnE | p6 | ride7 600 s | 74 224 | 6 | 4 | 2 (fault 4, fixed in p9) |
| ldOnG | p6 | lake6 75 s, shots every second | 3 735 | 1 | 1 | 0 |
| enOnH | p9 | shrine 270 s, shots every second | 39 156 | 3 | 3 | 0 |
| wkOnH | p9 | shrine 600 s | 86 591 | 3 | 3 | 0 |
| rdOnH | p9 | ride7 600 s | 73 171 | 5 | 3 | 2 (see below; fixed in p13) |
| rdOnI | p13 (main c6fbbf5) | ride7 600 s | 66 328 | 3 | 3 | **0** |
| wkOnI | p13 | shrine 600 s (load average 21) | 46 176 | 3 | 3 | **0** |
| wkOnJ | p13 | shrine 600 s | 52 366 | 3 | 3 | **0** |
| wkOnK | **p14 (the package)** | shrine 600 s | 44 228 | 3 | 3 | **0** |

rdOnH's two: frame 2 899, Agro, put off 2 frames while far and unseen, then **moved by the test aid** (ride7's
`SOTC_TELEPORT` puts Agro 6 units beside Wander every 900 frames; the game's own warps happen behind a black screen or a
demo); and frame 8 206, a bird (r 0.05) 6.9 units from the camera and 12.7 from Wander, put off 3 frames while its AI kept
moving its place (the AI control runs whether or not the geometry does) until it crossed the screen's edge. p13 adds: within
15 units of the camera (`SOTC_CHARLOD78_CAM_NEAR`) counts as seen.

**The load cut.** Every run's only stale characters before p9 are 1-4 small characters at the altar (r 0.00, 9-23 units from
the camera) on the first frame after Load Game's scene: the drawn camera moved 6.6 units and turned 136 degrees in one frame.
ldOnG's shots every second (t0038-t0055) show the letterboxed scene, then **two full-black shots (t0043, t0044)**, then the
altar: the characters were put off during the black screen and the first frame of the new picture showed them with a pose
<= 0.25 s old, updated on the next frame. The camera changes after the character tails in that frame, so no rule at the tail
can see it coming; nothing was on screen before it to look frozen against. Every later cut is covered by the guard (no
put-off for 30 frames after it).

**Entries.** A character entering the drawn picture after a put-off in the last 30 frames is caught up first: the frames
between its last put-off and the first frame it is drawn (the `entries` / `entry` lines), shrine + ride7 (wkOnE, rdOnE,
rdOnH, enOnH), per 300-frame window the least lead: 1-22 frames, 0 only at the load cut and the cases above. enOnH's 34
`entry` lines: 3 at the load cut (lead 0), the other 31 lead 9-27 frames (the altar's small creatures, r 0.05, 6-10 units from
the camera: updated every frame for at least 9 frames before they reach the picture).

**Time.** Round 78's check per 300-frame window (tcheck.py: time handed to each put-off character's pre-process minus the
player's, who is never put off): wkOnE 342 windows, -0.348 .. +0.244 s, median 0.000; rdOnE 261 windows, -0.249 .. +0.236,
median -0.011 (the time pending at a window's edge, at most one interval plus one frame). Put-off characters keep their own
speed.

**Same-thread replay check: not done.** A put-off character's catch-up runs ExecCharaGeometry once with the gathered time;
a replay of N one-frame updates from the same state would need a snapshot of the character's whole work (motion, IK, cloth,
root collision against the world, the anime tables) and of what it touches, restored on the game thread. One big step is
also not equal to N small ones in the original game: the game's own view interval (ViewIntervalClip) already hands
far-but-visible characters multi-frame steps. The checks above (time conservation, the drawn picture never holding a
put-off pose outside the load cut, the entry lead) are what this round proves.

## 4. Speed (Vita3K 222 MHz)

**With Agro put off (p9, before fault 5 was found; load average ~8).** In-run A/B (`SOTC_CHARLOD78=1 SOTC_AB_FLAG=charlod78
SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, no brackets, no audit; abrot.py from frame 3 000; + = what charlod78 saves):

| run | windows on / off | frame ms on / off | game ms saved | by part |
|---|---|---|---:|---|
| wkABH (shrine) | 103 / 104 | 43.12 / 44.22 | +1.09 | frames 3-6 k +0.35, 6-9 k +1.90, 9-12 k +1.05, 12-16 k +1.05 |
| rdABH (ride7) | 126 / 127 | 34.23 / 34.91 | +0.78 | 3-9 k +1.18, 9 k on +0.54 |

Clock brackets, separate runs, same load (~8-9), frames >= 3 003: ExecCharaGeometry at the shrine 18.0 calls / 14.87 ms
(wkOffE) -> 11.0 / 11.03 (wkOnH); ride7 9.3 / 9.28 (rdOffE) -> 4.9 / 7.28 (rdOnH). Most of the shrine's part was Agro
(3.98 -> 1.59 ms a frame), the part fault 5 takes back.

**The package rule (p13 / p14: Agro never put off, 15 units of the camera kept).** The machine was loaded (load average
15-16, frames 52-70 ms) during the A/B runs: wkABK +2.12 ms game (57 / 58 windows), rdABK -1.60 (81 / 82; its last part -7.9,
noise), i.e. the A/B is below the noise there. The per-character `[charlod]` lines measure the cut directly (smallgeo.py:
ExecCharaGeometry ms a frame of the small characters more than 12 from Wander, all windows from frame 3 000):

| route | off | on | saved |
|---|---:|---:|---:|
| shrine (wkOffJ / wkOnK, p13 / p14) | 2.20 | 1.62 | **0.58** |
| shrine, without the camera-near rule (wkOffE / wkOnH, p6 / p9) | 2.05 | 1.02 | 1.03 |
| ride7 (rdOffE / rdOnI) | 2.72 | 0.73 | **1.99** |

So the safe rule saves **~0.6 ms of geometry at the altar and ~2 ms riding** in Vita3K (plus a share of their
GetMotionMatrix), far below arch_plan's -4.2 ms, which counted Agro and round 78's stale draws. Expected on the console:
~0.5 ms at the altar, ~1-1.5 ms riding past birds and lizards; to be measured with the package.

## 5. Pictures looked at

Read (the png) and described:

- **wkOnI / wkOnJ (p13, Agro still put off at 0.12 s):** at the altar a dark blurred rectangle left of Wander and white
  haze blobs over the light (wkOnJ t0090-t0570), in wkOnI a whole-screen blur of dark and white shapes (t0180-t0420); wkOffJ
  (same build, off) clean in all 19 shots, wkOnK (p14, Agro never put off) clean in all 5. Its cause was not pinned down;
  it went away with fault 5's fix, and Agro was standing 50 units away, unseen, in those runs.

- **ldOnG t0038-t0055 (every second around Load Game):** the letterboxed scene (Wander walking into the light beams), two
  full-black shots (t0043, t0044), then the altar with Wander, Mono and Agro's head at the right. The load cut's stale frame
  is the first frame after that black screen.
- **enOnH t0150-t0270 (every second at the altar):** Wander at the altar, Mono on it, the window's light; a small green shape (a plant or a lizard)
  at the lower right in t0199. The entering creatures (r 0.05, 6-10 units from the camera) are a few pixels at this size; no
  creature is seen in the same pose across consecutive shots while others move, nothing frozen, no jump.
- **t0300 / t0540 of wkOffE, wkOnE, wkOnH, wkABH, wkLegF** (the altar: Wander, Mono, the light; wkLegF Wander raising the sword's
  beam), **rdOffE, rdOnE, rdOnH, rdABH** (Wander riding Agro on the plain, the cliffs, a ruin wall; Agro's legs, mane and tail
  in gallop poses, shadows under them): all normal, no frozen or misplaced character.
- **avOnA / avOnB / avOnC contact sheets (every 5 s at the lake):** Agro on the shore, the bow drawn at the sky, Wander
  swimming past the ruins' pillars and fence, the boss bar from avOnC's `bootflag4`; no intro. **avOnD:** the teleport to
  Avion's home place: a white void, Wander falling, Game Over (the test aid, not the rule). **avLegF t0150 / t0290:** Wander
  swimming by the ruins' wall (legacy rule; Avion not in view).

## 6. Console

`build/agent-scratch/a2/`: celosia-a2lod.vpk / .elf, env-a2lod-off.txt (= all/env-all-on.txt), env-a2lod-on.txt (+
SOTC_CHARLOD78=1), README.txt; build p14 = main c6fbbf5 + A2-lod (W1's elf pad 12 -> 8 KB: with 12 KB the text ended 0x119c
bytes below a 64 KB boundary, vita-elf-create "segment 1 overlaps"; 8 KB leaves 0x219c). Gates: 0 errors, spalign 0 of 3 417,
`sotc_vita_elf_room` 0x1e000, no overlap. One ride with each, starting at the shrine; with SOTC_CHARLOD78_LOG=300 added the
`[charlod]` lines show stale / entries on the console too. Expected: ~0.5 ms less at the altar, ~1-1.5 riding.

**Verdict.** The switch stays **off by default**. What this round proves in Vita3K: with the A2-lod rule no put-off character
is drawn with an old pose (stale 0 outside the first frame after Load Game's black screen, on the shrine, ride7 and the lake),
every character entering the picture was updated 1-27 frames before, time is conserved, Agro and the small animals behave
as with the switch off, the active colossus is never put off. What it does not prove: **Avion's intro** (not replayable
here) and the ptcl79 half of the old bisect. Before it goes on by default: play Avion's intro on the console with
env-a2lod-on.txt and look at Agro and the birds around the shrine and the plain.
