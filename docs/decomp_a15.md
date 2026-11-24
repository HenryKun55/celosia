# Round A15: the ride7 freeze hunt (the stage manager's subcell gate; typed switches stay off) (2026-11-24)

Goal: find and fix the intermittent ride7 freeze (A12-A14: after the mount Agro stops at x = -147 .. -149 for good while its
motion state machine runs) so that a12typed / a13typed / a14typed could be turned on. Result: the freeze is **not caused by the
typed code** (it is older than A12), the game mechanism that can hold the pair still is identified, a guarded fix exists as an
opt-in switch, but the freeze **did not recur in 14 A15 rides**, so the fix is unproven and **no default changed**. A separate,
new failure class was seen once: a whole-process hang (section 4).

Commits: bf2429a (diagnostic + fix behind a switch), 3e10a2d (ELF room 128 KB, reverted at the merge: 5d17847), and this round's close (fix made opt-in, docs). Rebased on D1 main de2fc67 as 665c8a1.
Scratch: build/agent-scratch/a15 (pf.vpk = diagnostic + fix on by default, pr.vpk = the commit's build, fix opt-in).

## 1. The freeze is older than A12

A scan of every kept ride7 run with a position log (`[pos]`, SOTC_RIDE_STEER) for Agro standing still east of x = -150 after the
mount (scratchpad script cross.py over build/agent-scratch/*/runs):

| run | round | typed code | froze at |
|---|---|---|---|
| s10/mixR7p1 | S10 (2026-11-18) | none (A12 did not exist) | -149.16 -331.65, frame 6 900 to the end |
| s10/mixR11p3 | S10 | none | -148.81 -313.79 |
| s11/chkRp2 | S11 | none | -149.42 -273.13 |
| a12/mixR7 | A12 | a12typed A/B | -148.93 -318.07 |
| a13/fA7b, a13/xoff7 | A13 | A/B, off all run | -149.05 -348.91, -149.29 -305.94 |
| a14/boot14d | A14 | a12 + a14 on | -148.76 -381.80 |

Many other rides of the same rounds crossed x = -150 eastwards and rode on (c11 -145.8, ctlR7 -129.9, d1 -143.4, b6/b7 mixR*,
s10 bootR12m -118.5, s13 profR0 -128.6 ...). So the A12 / A13 / A14 "rates" (3 of 9, 2 of 5) are host-load luck, not the typed
code: the same freeze happened in S10 / S11 builds with none of it.

## 2. What holds the pair (the game's stage manager)

Read from the ELF (capstone in a scratchpad venv over tools/disasm.py) and confirmed with A15's `[a15s]` lines:

- The stage manager (0x14663a8) cuts the field into a 10 x 10 grid of 600-unit cells, each 4 x 4 subcells of **150 units**
  (`[a15s] ... grid 600.0 x 600.0, 10 x 10 cells, 4 x 4 sub`). Cell (5, 4) spans x 0 .. -600; its subcell edge at **x = -150**
  is the freeze line. The grid (0x14663ac) has 156-byte cells; +8 is the loaded stage's handle word; the stage's subcell
  records (60 bytes) have flags at +4: bit 2 = loaded (set by the load script 0x13608E8 after sheetMgrLoadSegmentIndex, cleared
  by the unload 0x1360AD8), bit 1 = wanted this turn.
- Its coroutine gamecore_sub_013650E0 calls gamecore_sub_01362208 every turn: the **load centre is the camera's map place**
  (0x1465990, written by st_camfinalizer from the camera context). 01362208 marks the subcell under the camera and, per axis, the
  neighbour on the side the camera is nearer to (none inside a band of 1/64 of a subcell around the middle); new loads are issued
  only while no load script is in flight (group 0x11e4010).
- Agro's coroutine gamecore_sub_01395E20 (the horse's +0x20) and Wander's nico_player1: while horse +0x4204 is set (always while
  ridden, `+4204 2147484...` in the lines) they **do not step** as long as StageManagerCheckAreaExist (0x1365450) says their
  layout place (+0x90 +0x134 -> +0xA0) is in a subcell with neither bit 1 nor bit 2. execObjectWithNonTaskSwitch skips their
  geometry the same way, which is why the dumps showed the motion words changing while the place never did.

So a pair that reaches an unloaded, unwanted subcell stops; if the camera, which follows the pair, is then still outside the
half of its own subcell nearest that edge, nothing ever marks the subcell ahead: a deadlock that looks exactly like the freeze
(Agro galloping on the spot just past x = -150, the camera still behind it). This is the best-supported mechanism; it was **not
caught in the act** in A15 (section 3), so it stays a hypothesis.

## 3. A15's runs (Vita3K 222 MHz, muted, ride7 900 s, a12 + a13 + a14 typed on, watchdog report-only)

Build pf (diagnostic + fix). `[a15s]` lines every 150 frames and at every change of the gate's answer for Agro's place.

| set | runs | fix (SOTC_A15_LOADCENTER) | extra | mounted | crossed x = -150 east | ride freezes |
|---|---|---|---|---|---|---|
| sOff1-3 | 3 | off | SOTC_FRAME_STALL_MS=40 from 2 200 (a slow host) | 3 | 0 | 0 |
| sOn1-3 | 3 | on | same stall | 3 | 1 | 0 |
| hA1-4, hB1-4 | 8 | off | none | 8 | 4 (hA1, hA4, hB1, hB2) | 0 |

- 14 of 14 mounted rides with all three typed stages on: **0 ride freezes** (1 whole-process hang, section 4).
- The camera's map place stayed within ~5-15 units of Agro in every line (e.g. horse -183.30 -305.84, camera -186.50 -313.00);
  every crossing of the -150 edge found the subcell already wanted / loaded (`flags 00000005 exist 1`).
- The fix never acted (no `[a15] frame ... load centre on the pair` line in the 3 runs with it on): the trailing-camera state did
  not occur, even with 40 ms added to every frame. So neither the cause nor the fix could be shown on a freeze in this round.

**The fix (opt-in, SOTC_A15_LOADCENTER=1; a13_watch.cpp `installLoadCenter`)**: 01362208's table slot is wrapped; when the ridden
horse's (or Wander's) subcell is outside the camera's window and the camera's subcell is inside the window centred on the pair,
that call is given the pair's x / z instead of the camera's (both subcells kept, the gate opens), then 0x1465990 is restored;
every other call is the game's own. It is off by default: unproven on a real freeze.

## 4. New failure class: a whole-process hang (hA1)

hA1 (pf, a12 + a13 + a14 on, fix off, n6 tree): at 16:34:40, t = 705 s, frame 18 738, mounted and galloping at -372 -287 (cell
5,4 sub 2,2, area loaded), err.txt stopped growing; the window stayed at ~4 fps with nothing moving (picture t0840: Agro mid-gallop
by the tall wall, normal ground / shadows). Kept in build/agent-scratch/a15/hang-hA1 (err.txt, vita3k.log, pictures, `sample`
of the Vita3K process). The sample: every guest thread parked in Vita3K's semaphore_wait / condition waits (CELO00001 main,
the game / render / worker / loader pthreads), a few in timed polls; no thread running guest code. vita3k.log has only the usual
`handle_access_violation ... write protected region` lines (also present in healthy runs). No `[b5dog]` report was triggered.
Seen once in 14 rides; cause not found. Counted apart from the ride freeze.

## 5. What was not done

- No Vita3K A/B of the typed stages in this round (A12-A14's own A/Bs stand: pooled about +0.0 .. +0.7 ms, not slower).
- 89 MB boot of the merged build (pmA15 = A15 rebased on D1 main de2fc67 as 665c8a1, default switches, ELF room back to
  120 KB): mounted at frame 3 491, rode to frame 8 213 where Wander came off Agro (the route's steering; d1/chkC dismounted the
  same way), then on foot to frame 9 793 (605 s); 0 FREEZE / watchdog lines, pictures at 300 / 540 s normal (Agro by the wall;
  Wander running on the plain). The console memory card was restored from the vita3k-celosia-avion tree's identical copy.
- Defaults: a12typed / a13typed / a14typed stay **off**; SOTC_A15_LOADCENTER stays off.

## 6. Next

- Catch a freeze with `[a15s]` on (SOTC_A15_STAGE=150, SOTC_A13_WATCH=90): the FREEZE dump now prints the gate's answer, the
  subcell, the flags and the camera's place; if it shows `exist 0` with the camera outside the near half, rerun the same with
  SOTC_A15_LOADCENTER=1 and turn the fix on by default; if it shows `exist 1`, the hold is elsewhere (horse +0x4204, the motion
  state) and the stage manager is cleared.
- The hA1 hang: a guest-thread state dump on a host-side no-frame timeout (the b5dog path only fires from the ride watchdog).
