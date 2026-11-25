# Round P3: K2 without a reboot between launches, brackets one level down, the probe's PPI / CPU-interface checks (SOTC_P2BRACKET=2, SOTC_P2BRACKET_CLOCK, SOTC_P2PMUSAMPLE=probe; off by default) (2026-11-25)

Why: the game thread is at ~41 ms a frame on the console and has to reach 33 ms. P2's console runs (2026-11-25,
`build/agent-scratch/console-logs/run-20261125-p2dmiss`, `run-20261125-p2bracket2`) said two things and left one bug:

- **IRQ sampling is not available**: the probe found no GIC SPI line rising on a PMU overflow (`candidates 0` in 3 trials,
  `noise 0`): P2 turned itself off.
- **Brackets** (frame ~18.4 M cycles at 444 MHz): execScript inclusive 14.4 M (78%). Self cycles: execScript 4.09 M,
  ExecCharaGeometry 2.76 M (9.9 calls), DispDispObjGroupModel 2.76 M, camera 0.96 M, modelDisp 0.91 M, particle manager
  0.77 M. D-stall self: execScript 1.06 M, DispDispObjGroupModel 0.65 M, ExecCharaGeometry 0.51 M. Those three selfs (9.6 M,
  52% of the frame) say "somewhere below here"; P3 splits them.
- **Relaunch bug**: the second launch without a console reboot got `taiStartKernelModule 0x8002d013` (the module is still
  resident: the load hands back the resident module's id, the start fails) and k2::boot never tried the bridge then: K2 off.

Base: main c553cbb (P2). Scratch, packages, runs: `build/agent-scratch/p3/` (M1's Vita build dir copied and re-pointed, as P2 /
I2 did; own Vita3K trees ../vita3k-celosia-p3 / -p3b, deleted at the end).

## 1. The relaunch: K2 through the resident module (no reboot between runs)

What the console logs show: the first launch after a reboot loads K2 (`taiLoadKernelModule 0x4003003f, taiStartKernelModule 0`,
module_start 0, answer yes); the next launch gets the **same module id** back from the load (`0x4003003f`) and
`taiStartKernelModule 0x8002d013` (SCE_KERNEL_ERROR_MODULEMGR_OLD_LIB: the module is resident and started). k2::boot only
tried the bridge when the *load* failed, so K2 stayed off.

Now (`PS2Recomp/ps2xRuntime/src/lib/ps2_vita_runtime.cpp`, k2::boot): when the start fails (`< 0`) the bridge is tried too
(`g_resident`); the bridge binds to the resident module's syscalls and its hello decides (magic ^ nonce, version). Vita3K's
start returns 0 without running the module, so nothing changes there. The `[k2] load` line says
`K2 on (the module resident from an earlier launch, bridged: no reboot needed)`, or why not (an older version resident:
reboot once; no answer), plus a `[k2] resident module: hello rc ... version ... instance ... held ...` line.

State left by the previous process (kernel, `vita/k2/celosia_k2.c` / `celosia_p2.c`):

- **Normal exit or kill**: K2's process-event handler (`onGone`) already releases everything of that pid: C1's hook pid
  cleared and the hook released (`c3Release`), P2's sampling stopped and its interrupt handler released (`p2Gone`), K2's
  re-maps put back, owner = -1. Checked by reading the code: every path that P2 / C1 registered is released there.
  New in P3: `p2Forget` also clears P2's probe results (thread, context, line of the dead process), so a START needs the new
  process's own probe (START already compared them with the calling thread; now they are gone).
- **The handler never ran** (should not happen, but the hello allowed a new owner whenever nothing was held): the hello now
  releases the previous owner's state itself before taking ownership (C1 pid + hook, P2 handler + probe results).
- **Per-thread PMU state** is re-armed by the new game's first frame (K2_ARM_RESET); a PMINTENSET bit P2 left on a core is
  cleared by the next arm there (P2's leftover logic, unchanged).
- **Version**: K2_VERSION 1 -> 2 (and P2_VERSION 2: the probe struct grew). The first P3 run after a P2 run must reboot once
  (the resident module is P2's: hello answers version 1, K2 off with "reboot"); from then on no reboot is needed.

## 2. Task 3: the probe's PPI / CPU-interface checks (SOTC_P2PMUSAMPLE=probe)

The A9 MPCore's private interrupts are IDs 27-31 (global timer, legacy nFIQ, private timer, watchdog, legacy nIRQ); the
PMU's nPMUIRQ[3:0] are outputs of the MPCore that the SoC wires somewhere (ARM's TRM: not to a PPI inside the MPCore). P2's
probe looked at the distributor's SPI words (IDs 32-255, TYPER 0xfc67 = 8 words, all of them) and found nothing rising.
P3's probe also records:

- **word 0 of GICD_ISPENDR** (banked per core: SGIs 0-15, PPIs 16-31) before / during / after each trial, which bits rose in
  every trial and in some (always: reading a pending word changes nothing). Never chosen as the sampling line.
- **the CPU interface** (PERIPHBASE + 0x100, through the distributor's kernel VA offset or K2's search; device /
  strongly-ordered memory checked): IIDR, PMR, and GICC_HPPIR (the highest pending interrupt id: read-only, the acknowledge
  register is never read) + RPR before and while the overflow line is up in each trial. HPPIR only shows enabled interrupts,
  so it answers "does a PMU interrupt reach this core under some id that someone enabled".

`SOTC_P2PMUSAMPLE=probe`: the probe alone with these extended checks, at the first frame, never started; K2's events are
left as they are. One `[p2] probe ...` line (as P2's) + `[p2] probe ppi: word 0 rose in every trial ..., in some ...
| cpu interface found: ... HPPIR before ... during ... (ids ...) | RPR ...` + `[p2] probe only ...: rc N, not started`.
Also fixed: the console returned the probe's -4 as `-1073741828` (0xbffffffc: the syscall return path clears bit 30 of a
negative result), so P2 printed "refused (?)"; the code is now read with bit 30 set again (-4 = "no single PMU interrupt line").

## 3. Brackets one level down (SOTC_P2BRACKET=2)

**How the children were chosen.** The P2 console run had SOTC_PROF129=1 on: its `[prof129] e <pc>:<ra>:<n>` pairs give, for every
guest function the game thread was in, the guest function of its return address. Following each function's most frequent
caller up to the nearest P2 bracket attributes every running sample to one bracket and to the unbracketed functions on the way
(a scratch script on top of tools/prof129.py's ELF / window parser). The console's own samples, 5 942 frames, 124 916 running:

| bracket (P2 self) | under it, not bracketed (console prof129 samples inside the bracket's self) |
|---|---|
| execScript (15 507) | manager_sub_01102638 = scriptPass 98%, isysWakeupSubThread 91% (the script bodies run in the wakes), st_controltarget 37% -> charaCtrl (decomp_hot46c) 35%, frameEnd 25%, frameEnd's charaGeometry 11% and objectAnimation 10% (ExecObjectAnimation), GetMainAnimation 8%, ctrlTurn 6%, walk 5%, graphFrameInit 5%, horseStep 4%, texReset 3%, isysGroupForExit / ForNext 2% each |
| ExecCharaGeometry (30 440) | GetCharaRootDynamics 27%, GetClothDynamics 19% (nico_execPlayerFringe -> getClothDynamics 17%), CalcNodeQuaternionWithIK 11%, norootdynamics 10% -> charrootdyn_getCollisionDB 10% -> GetCollisionDBCallBack 9%, cloth velocity 9%, xz 9%, manager_sub_010E1808 7%, ExecAnimeTableInfo 7%, finder 6%, AddCharacterClassSubGeometries 6%, rootBlend 5%, SequenceKickParticle 4%, playerExpression 4%, gaze 3% |
| DispDispObjGroupModel (20 575) | DispLayoutObjModel 25% (manager_sub_010F3FB0 11%, solidDisp 10%, skinned 3%), lightPass 21% (CalcLightLayoutObj 14%, SetGeoDispClassLightColMan 8%, ExecGlobalLightMan 4%), ClipLayoutObjModel 13%, the particle-display gate 7% (disp_particle_emitter 5%), DispWithSwordBlur 6%, clip_particle_geodisp 4%, DispCloth 3%, glVertexKick 3%, graphFsEffectReset 3% |

67 candidates were wrapped in a first build and counted in Vita3K with the new clock brackets (SOTC_P2BRACKET_CLOCK=1, ride7,
windows 10-14). Left out: isysGroupForNext 323 calls a frame, iosDlLockObj 171, ClipLayoutObjModel 168 (no guest callees: it
is most of what stays in DispDispObjGroupModel's self), isysGroupForExit 122, clip_particle_geodisp 75,
isysLetterObjDiscardLetterAll 72 (0 us), the particle-display gate 0113ACB8 71 (its emitter is kept); never entered through the
table on the ride: manager_sub_010BC4F0, manager_sub_010F3FB0 (DispLayoutObjModel calls the recompiled code directly), DispCloth.
**57 kept** (game/overrides/sotc_p2.cpp `kTargetsP3`; with P2's 22: 79 wrapped). Only functions entered through the function
table can be bracketed (jal / jalr, callGuest, callNative: a wrapper in the table makes callNative fall back to callGuest); a
native's direct C++ helpers ("...Sw") stay in their caller's self.

**What changed in the brackets** (`src/lib/ps2_vita_p2.cpp`):
- the game thread only: TPIDRURO (the thread's TLS pointer, user-readable, no syscall) recorded at start; a call from another
  thread (the guest worker) passes through untimed (`foreign` count; 0 on the rides);
- a closed child charges its parent with its count **+ one read** (the read that opened it): a parent's self now holds one read
  per call of its own, as a leaf's (P2's parents held one more read per child call: execScript's 4.09 M had ~0.18 M of reads);
- `nest` (bracket calls inside, transitive) on every line: the report subtracts (calls + 2 x nested) reads from inclusive;
- `par=<name>`: the most frequent caller bracket (3 slots, halved every window): the report prints a tree;
- 128 slots (was 48 / 64); `[p2b] w ... bracket calls/f N = 2N reads/f (~C cycles/f); foreign F` per window;
- `SOTC_P2BRACKET_CLOCK=1`: without K2's counters (Vita3K) the brackets run on sceKernelGetProcessTimeLow (microseconds in the
  cycles column, events 0) for call counts and the tree; never needed on the console.

**Cost.** One bracket call = two K2 reads (P2's console calibration: ~1 097 cycles a read, ~20 itlb and ~31 0x05 events).
The final set makes 740-1 250 bracket calls a frame on the Vita3K ride (clockC windows 7-14; 916 averaged over windows 6+),
P2's set ~180 on the console: **~2.0 M cycles a frame (~4.5 ms at 444 MHz)** of reads in this run, subtracted by the report
from the numbers (not from the frame time: the run itself is slower, and the reads' cache footprint is not subtracted; for the
big brackets it is noise, for the 50-75-call ones read their self as "+-0.1 M"). The busiest: DispLayoutObjModel / modelDisp
74, isysWakeupSubThread 68, solidDisp 67, finder 49, disp_particle_emitter 49, glVertexKick 31, clothStep / cloth velocity 27.

**Vita3K's tree** (clock brackets, us a frame, ride7 windows 6+, 222 MHz; for the shape and the call counts, not console
weights; a child called from several brackets is listed under its most frequent caller with its whole inclusive, so a share
over 100% means "mostly called from elsewhere"):

```
execScript                          incl 42307   self    16   calls  1
  scriptPass                        incl 42287   self  4426   calls 16
    isysWakeupSubThread             incl 24140   self  1835   calls 68   (the script bodies)
      DispDispObjGroupModel         incl 13389   self  3135              (ClipLayoutObjModel + the particle clip stay here)
        DispLayoutObjModel          incl  7430   self  2566   calls 74
          solidDisp                 incl  4094   self   451   calls 67   (-> modelDisp 3936)
          skinned                   incl   741   self   292
        lightPass                   incl  1368   self   413   (CalcLightLayoutObj 834 -> SetGeoDisp.. 648 -> ExecGlobalLightMan 484)
        disp_particle_emitter       incl   608   self   425   calls 49
        DispWithSwordBlur           incl   576   self   303   (glVertexKick 185)
      doit_cameraman_manager        incl  4227   self  2587   (ClipCollisionDirectCallBack 1630, GetCollisionDBCallBack 908 -> finder 642)
      doit_particle_manager         incl  1586   self  1577
      CharaCtrlPlayable / AIControl -> charaCtrl 1945 (horseStep 804, GetMainAnimation 263, ctrlTurn 258, playerStep 172)
      graphFrameFinish 360, graphFrameInit 182, InitLayoutObjMatrix 92, ExecFogColMan 76, updateSpecialEffectManager 75
    frameEndCharaGeometry           incl 11177   self   171   calls 11
      ExecCharaGeometry             incl 10962   self  1725   calls 11
        GetCharaRootDynamics        incl  2313   self   658   (xz 1382: slidecollision 190, landingSurface 158)
        GetClothDynamics            incl  1927   self   393   (getClothDynamics 1467: clothStep 405, cloth velocity 398)
        CalcNodeQuaternionWithIK    incl  1827   self   791   (playerExpression 327)
        GetMotionMatrix             incl  1770                calls 17
        UpdateLayoutClassGeometry   incl  1069                calls 11
        sub_010E1808                incl  1010   (sub_010E0558 342, aimIk 272)
        AddCharacterClassSubGeometries 439 (rootBlend 581), GazeAddMotions 274, ExecAnimeTableInfo 192
    ExecObjectAnimation             incl  1366   self  1345   calls 17
```

What it already says before the console run: execScript's 4.1 M self was not execScript (16 us): it is scriptPass's own
walk (~10% of execScript in Vita3K), the sub-thread wakes' script bodies (~4%), ExecObjectAnimation (3%), charaCtrl (5%), and
the frame-start / frame-end graph work; ExecCharaGeometry's self splits into root dynamics, cloth, IK, motion matrices; the
display's into the model walk, the light pass and what ClipLayoutObjModel leaves. The console run gives the cycles and the
D / I stalls of each.

## 4. Report (tools/p2_report.py)

Reads the new `nest` / `par=` fields (P2's lines still parse: then one read a call is subtracted, as before, with a note);
inclusive minus (calls + 2 x nested) reads, self minus one read a call; a `bracket cost` line (calls a frame x 2 reads x the
calibrated read, and one call's cycles); a `tree (by caller bracket ...)` with each bracket's inclusive, its share of the
caller's inclusive, self and calls; clock runs label the column "us (clock)". Checked on P2's console err.txt (same table as
P2's report plus `bracket cost: 180.6 bracket calls a frame x 2 reads x 1097 cycles = 396 213 cycles a frame (~0.89 ms)`) and
on the Vita3K clock runs (the tree above).

## 5. Checks (Vita3K: correctness only)

Build: rebased on main 8d340e7 (I2's hot order), packaged as `build/agent-scratch/p3/celosia-p3.vpk` (ELF `celosia-p3.elf`).
spalign `0 sp-based :128/:256 NEON accesses (of 3008 with an alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880; no
"segment 1 overlaps"; 0 errors. celosia_k2.skprx 13 352 B (12 855 in P2); it and its bridges build warning-free with -Wall
-Wextra. No frozen-picture or difftest mode was used.

89 MB ride7 boots (`SOTC_VITA_EAT_USER_MB=89`, 600 s, Vita3K 222 MHz, 3 cores, muted):

| run | build | switches | result |
|---|---|---|---|
| bootOffA / bootOffB | first / final | none | no `[p2]` / `[p2b]` / `[k2]` lines (main's behaviour); mounted at 6 271 / 6 175; heap 105.7 MB at t=606 s (B) |
| bootOnA | first (89 wrapped) | SOTC_P2BRACKET=2 SOTC_P2BRACKET_CLOCK=1 SOTC_P2PMUSAMPLE=probe SOTC_K2=1 SOTC_M1PMU=300 SOTC_PROF129=1 SOTC_D1NATIVE=1 | the candidate count (section 3); `early 0` on every bracket, `foreign 0`; mounted 3 737 |
| bootOnB | final | env-p3-bracket.txt (no clock) | `[p2b] 79 functions wrapped, level 2`, `[p2] off: K2 is off`, `[p2b] off: K2's counters are not on the game thread` (wrappers pass through); mounted 3 057; heap 106.9 MB |
| clockC | final | as bootOnA | 79 wrapped, the tree above, 740-1 250 bracket calls a frame; mounted 3 155 |

(The mount frame varies 2 200-6 700 between runs of the same build, I2's boots too.)

Pictures (looked at, t0300 / t0540): bootOffB t0300 Wander on Agro galloping beside the shrine's mossy stone wall with two
trees, Agro's shadow on the sand; t0540 riding across the open plain towards a grassy ridge under a cloudy sky. bootOnB t0300
riding past the shrine's stone stairs and walls, cliffs behind; t0540 galloping along a tall dark stone wall, mountains and
clouds on the left, shadow under Agro. Nothing missing or garbled.

**Not verifiable here (the console decides):** the relaunch path (Vita3K never runs the module), the hello's release of a
previous owner, the probe's PPI / CPU-interface lines, every bracket's counters.

## 6. Console steps

`build/agent-scratch/p3/` (README.txt): `celosia-p3.vpk` + `env-p3-bracket.txt` (P2's env-p2-bracket.txt with
SOTC_P2BRACKET=2 and SOTC_P2PMUSAMPLE=probe). **Reboot once first** (a new K2 module, version 2), then one 4-5 min ride; send
err.txt. Optionally launch again without a reboot and ride a minute: its `[k2] load` line must say "bridged: no reboot needed".
Report: `python3 tools/p2_report.py err-p3-bracket.txt --elf build/agent-scratch/p3/celosia-p3.elf`.
