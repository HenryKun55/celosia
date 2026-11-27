# Round Z1: the collision library the characters call as typed native C++ (SOTC_Z1=1, off by default) (2026-11-26)

Why: X2's console ranking (docs/decomp_x2.md, section 1.3) put two of the script bodies' weight in the collision library:
nico_sheathScript (0.31 ms a frame: CalcCollisionMultiDepSurfBoundary 0.12, ClipCollisionDirectCallBackOneObj 0.06) and,
through the cloth, st_collisionalias (0.71). The library's hot path is host code since rounds 10-125 (ClipCollisionDirectCallBack's
obj125 loop, the tree walk, ClipCollisionDB, the finder), but three of its entries were still lifts written register for
register (D5 / D7: every float and VU0 register in R5900Context, the 0x260-byte frame in guest RAM through the READ / WRITE
macros, each callee a guest call through the table), and the skinned surfaces' boundary a guest call with the A8 adapter's
context hand-over (some 30 registers read and written) for a dozen VU0 operations.

Base: main d783114 (Y1 + Y2 + Y3), rebased onto f6e2a5d (Z2) for the package. Code: `game/overrides/z1_coll.h` / `z1_coll.cpp` (switch, check, counts),
`game/overrides/z1_oneobj.inc` (the functions, included at the end of decomp_collision.cpp for its helpers), the hooks in
`decomp_liftd5_00.cpp` (OneObj's lift), `decomp_liftd7_04.cpp` (ClipCollisionDirect's lift) and decomp_collision.cpp's D1
loop (obj125RunD1's skinned boundary). Scratch, runs, package: `build/agent-scratch/z1/` (M1's Vita build dir copied and
re-pointed, the runner at `x1/runner`; own Vita3K tree `../vita3k-celosia-z1`, a copy of D2's `x2` template, deleted at the end).

## 1. The ranking

Vita3K, 222 MHz, Z1 off (main's code: the Y1 package's VPK), clock brackets (`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1`) on 32
collision entries through `SOTC_P2BRACKET_EXTRA` (`build/agent-scratch/z1/extra.txt`). Microseconds a frame, means of the
600-frame windows after frame 3000 on ride7 (`runs/rankRA`, 600 s, mounted at 2221, 28 windows) and after 2400 on lake6
(`runs/rankLA`, 480 s, in the lake after the teleport at 2000, 20 windows). Self is what no other bracket covers (the
natives a function calls directly stay in its self). The bracket's own reads (~900 a frame) inflate the absolute numbers.

| function | ride7 calls/f | ride7 incl / self | lake6 calls/f | lake6 incl / self | what it is now |
|---|---|---|---|---|---|
| ClipCollisionDirectCallBack (112D590) | 11.1 | 1 067 / 932 | 14.9 | 1 520 / 1 299 | native (obj125's D1 loop); the camera's and the scripts' segment tests |
| GetCollisionDBCallBack (1124C88) | 4.1 | 1 025 / 305 | 3.1 | 650 / 351 | native (round 47) + the finder |
| finder (1116958) | 32.0 | 678 / 303 | 9.7 | 253 / 107 | native (guest-exact transcription) |
| **ClipCollisionDirectCallBackOneObj (112CA30)** | 1.2 | **613 / 292 (508 a call)** | 3.6 | **486 / 434** | **D5 lift, register for register** |
| CalcRootFitting (10E21C0) | 7.2 | 593 / 464 | 4.0 | 584 / 456 | D4 lift of a 68-instruction wrapper; its self is the IK it tail-jumps to, not collision |
| **ClipCollisionDirect (112ED58)** | 0 | - | 1.8 | **484 / 430 (272 a call)** | **D7 lift, register for register** |
| finderWalk / finderLeaf | 9.5 / 21.3 | 358 / 237 | 4.2 / 7.7 | 140 / 92 | native |
| ObjAry (112E180) | 2.9 | 242 / 199 | - | - | native (obj125's D1 loop) |
| invMatrix (1133548) | 14.9 | 182 | 19.5 | 238 | D1 native |
| **CalcCollisionMultiDepSurfBoundary (1124368)** | **35.8** | **177 (4.9 a call)** | - | - | A8 typed lift behind a context adapter; called as a guest call by OneObj and by the D1 loops' skinned surfaces |
| FinderWithKeep / Keep4DSlopeCB / FinderCB / Finder / clipKeep | 2 / 1.6 / 6.1 / 6.7 / 43 | 153 / 129 / 115 / 97 / 106 | | 167 / 232 / 187 / 110 / 163 | native |
| ClipCollisionDB (111B9F0) | 10.2 | 81 | | | D3 native |
| the rest (ikcolsearch, groundProbe, the edge links, EntryFinder, the caches' getters) | | < 65 each | | | |

A second run (`runs/clkOffB`, ride7 600 s, mounted at 6448; `extra2.txt`: the callers and the callees as brackets)
splits it further, windows after frame 7000:

| bracket | calls/f | incl / self us a frame | parent bracket |
|---|---|---|---|
| OneObj | 1.34 | 641 / 286 (479 a call) | scripts:execScript (the sheath's script body) |
| CalcCollisionMultiDepSurfBoundary | 35.9 | 174 (4.9 a call) | OneObj (every call) |
| the ground probe (01118340, also calls OneObj) | 2.54 | 50 / 14 (20 a call) | |
| the water probes 01117340 / 01117EA0 | 1.7 / 1.6 | 72 / 21 | |
| ClipCollisionDB | 10.5 | 83 | |
| the old tree walkers 0112ACD0 / 0112B890 | 0 | 0 | never called on the ride or the lake |

So the ride's OneObj weight is the sheath's probe against Agro's skinned body: some 25-35 skinned surfaces a call whose
boundary cache is clear (the caches are flushed every frame), each a guest call into CalcCollisionMultiDepSurfBoundary,
plus the lifted body's own work around them (286 us a frame of self). On the lake OneObj is the water probes' and
ClipCollisionDirect the swim's / the falls' group probes (no skinned surfaces there).

ClipCollisionDirectCallBack's self is large but already host code (obj125's D1 loop with the tree walk, the leaves and
ClipCollisionDB as direct C++ calls; the old tree walkers it could still call never run); its skinned surfaces are few.
Z1 takes their boundary out of the guest call too (section 2) but leaves the loop alone.

## 2. What Z1 rewrites

`SOTC_Z1=1` (read once; off by default; requires d1native as the console env files set it). The lifts hand over at their
entry (after their resume-pc test) when Z1's entry test passes (the frame inside main RAM, the game thread); otherwise,
or when a function declines before writing anything, the lift runs (counted as `fallback`). `SOTC_Z1_PARTS=<mask>`
(testing): 1 OneObj, 2 MultiDep, 4 Direct (default 7).

| function | what | before | Z1 | where |
|---|---|---|---|---|
| ClipCollisionDirectCallBackOneObj (112CA30) | a segment against one object: the bound cylinder, each part's cylinder and part filter, the part's tree or its surfaces one by one, the skinned surfaces (boundary made, capsule test, filters, ClipCollisionDB) | D5 lift: R5900Context registers, the 0x260 frame through READ / WRITE, ~30 guest calls a sheath call (MultiDep x ~27, GetLayoutClass, the filters, ClipCollisionDB, EntryModifiedManager, iosAbsVector) | host function: floats and VU0 values as locals, the frame written only where a callee reads it, MultiDep / EntryModifiedManager / iosAbsVector / GetLayoutClass's plain cases / the boundary cache as host code; guest calls left: GetArbitraryDirectionInverseMatrix, the filters, the tree walks, ClipCollisionDB | z1_oneobj.inc `z1OneObj`, hook in decomp_liftd5_00.cpp |
| ps2__CalcCollisionMultiDepSurfBoundary (1124368) | a skinned triangle's boundary: its three corners (01116108: the bone / blend vertex caches made, EntryModifiedManager), the centre (sum x 1/3), the largest squared distance | A8 typed lift behind an adapter that copies ~30 registers in and out, called as a guest call | host code on round D3's corner core (`decomp_skinfinder::cornersD3Core`), the centre / radius in NEON with D1's operations in the old order; called directly from OneObj / Direct, and from obj125's D1 loop (ClipCollisionDirectCallBack / ObjAry) through a table-shaped adapter | z1_oneobj.inc `z1MultiDepCore` / `z1MultiDepCall`, decomp_collision.cpp obj125RunD1 |
| ClipCollisionDirect (112ED58) | OneObj's tests without filters for every object of an isys group | D7 lift, as OneObj's | as OneObj; isysGroupForNext by the native group walk's host case (decomp_collision's `groupNext`) | z1_oneobj.inc `z1Direct`, hook in decomp_liftd7_04.cpp |

How:

- **Exact values.** Every compare and product is the original's: `mul.s` / `add.s` / `sub.s` as D1's `d1::mul / add / sub`,
  the compares as `d1::lt / le / eq`, the VU0 transforms (`vmulax / vmadday / vmaddaz / vmaddw`) and the matrix product as
  D1's NEON chains, the three cylinder forms the original uses (the sum or y^2 left in f0, by call site) and its capsule
  test, the order of every early exit. The skinned group's and the skinned surfaces' forms differ between OneObj and
  ClipCollisionDirect in the original; Z1 keeps each.
- **What the caller and the callees see.** A host shadow (`Z1Vu`) keeps the VU0 registers, ACC and f0 as the original
  leaves them after each operation; it is stored into the context before every call that can run guest code or read
  them and at the exit, so the callees see the context the original gives them and the caller gets the original's
  f0 / VU0 registers. MultiDep's leftovers include vf8's high half from t0's 128-bit register (`qmtc2` of t0), read from
  the context as it stands at the call. The frame words the callees read are written before each call: the part
  filter's centre, the walk's record (+0xc0 / +0xd0) and segment, the boundary and id records, the ends ClipCollisionDB
  reads. Each guest call keeps the original's ra and frame sp (natives that look at the call site see the same one).
- **Not as the original, dead after the return:** the frame words no callee reads (the saves, the loop's spill slots,
  the intermediate transforms), MultiDep's / the corner core's frames are written as the D1 adapters write them; the
  scratch GPRs (at, v1, a0-a3, t*), their high halves, f1-f19, the fcr31 condition bit.

## 3. The check (SOTC_Z1_CHECK=1)

Y1's check (docs/decomp_y1.md section 3), for each of the three functions: same thread, no helper thread; for a call
checked, guest RAM (32 MB, one buffer allocated at the first check), the scratchpad and the context are copied; Z1's
function runs; the RAM words it changed are listed with their values and **only those words** are put back, with the
scratchpad and the context; the old function (the generated code, the ground truth; every Z1 hook declines while that leg
runs, so its callees are the table's as in a game without Z1) runs to its return; then every word either leg changed,
the scratchpad, and the registers a caller can read are compared: s0-s7, gp, sp, fp, ra, pc, v0, f0, f20-f31, fcr31
without the condition bit, VU0 vf1-vf31 / ACC / Q / status. The dead stack window (8 KB below the entry sp) is counted
apart (`deadStack`). The game goes on from the old leg. A check is taken only while nothing is posted to the EE
scheduler and its gate is not reached (`clock-skipped` otherwise), and a check in which the VBlank counts moved in either
leg is `void` (the old leg's checkpoints ran the VBlank handler and the threads it wakes). MultiDep is checked where it is
a call of its own (obj125's D1 loop through the adapter); inside OneObj and Direct it is checked as part of their calls.

Switches: `SOTC_Z1_CHECK_FROM` / `_UNTIL=<game frame>`, `SOTC_Z1_CHECK_EVERY=<n>` (one call in n per function, default 4),
`SOTC_Z1_CHECK_EVERY_MD=<n>` (MultiDep's own), `SOTC_Z1_CHECK_QUIET=<cycles>`, `SOTC_Z1_CHECK_CONTROL=1` (the old function in
both legs: the check's own noise), `SOTC_Z1_LOG=<frames>` (the `[z1]` counts every n frames: calls a frame, MultiDep's
inline calls a frame, fallbacks, guest calls a call, checks, bad, void, clock-skipped, dead-stack bytes).

**The noise the first runs met, and the repeat probes.** chkRC (p1, every 6 / 12 from frame 600) took ~1 check a frame at
~2.7 s each and was stopped; MultiDep's check 85 there (frame 1840), and on p2 chkLWE's MultiDep check 2 (frame 2483) and
OneObj check 138 (frame 4147), each listed 175-280 differing words, all at the same addresses (0x29126c, 0x2916bc,
0x2938fc, 0x293f3c, 0x29618c, 0x2983dc, 0x2a061c, 0x2c084c ...: w lanes of a static block), holding time-like floats
(655 at frame 1840, 1456 at 2483, 3784 at 4147) that the first leg had a little ahead of the old leg's. Nothing in the
three functions writes there. The control run ctlLWF (p3, `SOTC_Z1_CHECK_CONTROL=1`: the old function in both legs, Z1 not
run at all) showed the same words in two of its 79 MultiDep checks (frames 2482 and 2571), so they are the check's: a
host thread writes that block while a ~1 s leg runs. p3's probe (the old function again) could not see it, because the
words change only in the first leg; p4's probes run both again from the check's start (every word that differs from the
copy put back): a word the old function or the first leg's function does not repeat is another thread's write, counted
as `unrepeatable` and not compared; a word both repeat and that differs is a mismatch.

**Results** (Vita3K, 222 MHz, 89 MB, d1native + W1-W3 + X1-X3 + Y1-Y3 on):

| run | build | checked | OneObj | MultiDep (its own calls) | Direct |
|---|---|---|---|---|---|
| **chkRG**: ride7 900 s, from frame 2300, OneObj one call in 30, MultiDep one in 400 | p4 | **mounted at 3156**, to frame 11 478; MultiDep 36 a frame inline in OneObj after the mount | **508, 0 bad**, 3 void, 1 clock-skipped | 26, 0 bad | - (not called) |
| **chkLWG**: lake6 720 s, from frame 2400 (in the lake), one in 40 / 400 | p4 | to frame 5 298 | **255, 0 bad** (1 check with 240 unrepeatable words: the block above) | **145, 0 bad** | **58, 0 bad** |
| chkRD: ride7 900 s, from frame 2300, one in 30 / 400 | p1 | mounted at 9 891 (the checks' slowdown), to frame 13 278 | 218, 0 bad, 1 void (~185 after the mount, the sheath's) | 244, 0 bad | 14, 0 bad |
| chkLWE: lake6 720 s, from frame 2400 | p2 | | 250, 1 "bad" | 146, 1 "bad" | 58, 0 bad |
| ctlLWF: lake6 300 s, control (old in both legs) | p3 | | | 79, 2 "bad" | |
| ctlLWG: lake6 300 s, control (old in both legs) | p4 | | | ~70, 0 bad; 5 checks with 48-330 unrepeatable words (the block above) | |
| **pkgRH**: ride7 600 s, the package with Z2 on too (SOTC_Z2=1), from frame 2300, one in 30 / 400 | **p5** | mounted at 3368, to frame 7 998 | **291, 0 bad**, 1 void | 62, 0 bad | - |

chkLWE's and ctlLWF's "bad" checks are the block above (the same words, also in the control run); chkRG / chkLWG on p4 are
the final code's checks. All other words, every register, f0, the VU0 file / ACC / Q / status matched in every compared
check: the VU0 shadow, the cylinder forms' f0 and MultiDep's vf8 high half included.

Pictures, looked at: chkRD t0300 / t0540 Wander on foot on the grass plain, sword drawn (the ride script had lost Agro
under the checks' slowdown), t0900 Wander galloping on Agro along the shrine's wall, sheath on his back. chkRG t0300 Wander
on Agro at the foot of the shrine's stone stairs, t0540 on Agro by the tower wall with the plain and the far cliffs
behind, the reins drawn. pkgRH (p5 + Z2) t0300 Wander riding Agro up the shrine's stone stairs between the pillars, t0540 galloping on the plain, a hawk overhead. chkLWG t0300 Wander swimming in the lake by the stone wall and the iron fence, the HUD's circle,
sword and red bar; t0540 under water by a tower's base, the HUD. Ground, Agro, Wander, the water look normal.

## 4. Build

`build/agent-scratch/z1/` (M1's Vita build dir copied and re-pointed by `setup.sh`, nice 20, -j4; the first build p0
compiled the overrides into this tree's object directory and failed at the link only because z1_coll.cpp was created
after its configure step; p1 = p0 + the re-glob):

| build | what | used by |
|---|---|---|
| p1 | Z1 as committed, the check's scan with memcmp | chkRD |
| p2 | p1 + the check's scan in 64-bit words | chkLWE, clkOffE / clkOnE |
| p3 | p2 + a one-way repeat probe (the old function only) | ctlLWF |
| p4 | p2 + the repeat probes both ways (section 3), as committed | chkLWG, chkRG, ctlLWG |
| p5 | p4 rebased onto main f6e2a5d (Z2) | pkgRH; the package |

p5: `vita exit 0`, 0 errors. spalign: `0 sp-based :128/:256 NEON accesses (of 3417 with an alignment hint)`.
`sotc_vita_elf_room` 0x1e000 (122 880). No "segment 1 overlaps" (W1's pad unchanged at 8 KB). No alignas(16) by-value
structs (the VU0 shadow is a local passed by reference), no %zu, no GPU pool reads; every guest pointer goes through
decomp_collision's `G` (main RAM directly, anything else through the runtime) or `d1::Mem`.

## 5. Speed

Vita3K, 222 MHz, ride7 600 s, clock brackets (`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1`, `extra2.txt`: the same table
entries in both runs, so OneObj's bracket holds the lift in one and Z1's body in the other), build p2, Z1 off (`runs/clkOffE`,
mounted at 2224, windows after frame 3000) against Z1 on (`runs/clkOnE`, mounted at 5780, windows after frame 6400):

| bracket | off: calls/f, us a frame (a call) | on: calls/f, us a frame (a call) |
|---|---|---|
| OneObj (table entry) | 1.51, 711 (472) | 1.02, 311 (305) |
| collision:z1OneObj (Z1's body inside it) | - | 1.02, 306 incl / 192 self |
| CalcCollisionMultiDepSurfBoundary (table calls) | 36.1, 187 (5.2) | 0 (all ~35 a frame inline: `[z1]` inline/f 35.4 on chkRD's ride) |
| the ground probe | 2.74, 85 | 1.97, 61 |
| ClipCollisionDirectCallBack | 10.9, 1 265 (116) | 13.2, 1 567 (119) |

OneObj's calls are the sheath's (1 a frame while riding) and the ground / water probes' cheap ones (~20-30 us each; the
off run had ~0.5 a frame more of those). Taking those out, **the sheath's probe costs ~690 us a frame with Z1 off and ~300
with Z1 on: ~0.39 ms a frame less (-56%) in Vita3K**, of which MultiDep's 36 guest calls (187 us) are a half and the
lifted body's own register traffic the other half. ClipCollisionDirectCallBack is unchanged per call (its skinned
surfaces are rare: MultiDep from its loop ~10-20 a frame, through the adapter). ClipCollisionDirect runs only at the lake
(1.8 a frame there, 272 us a call off); no clock pair was run there (lake6's check run is in section 3).

The whole frame is not comparable between the two runs (the mounts came at 2224 and 5780, so the windows cover different
places). Vita3K has no caches and runs NEON through its JIT; on the A9 the removed context traffic (each MultiDep guest
call copied ~30 registers in and out of R5900Context and walked the table) and the lift's guest-RAM frame should count
for more, not less.

**Expected on the console**: X2's estimate put the sheath's script at 0.31 ms a frame, 0.12 of it
CalcCollisionMultiDepSurfBoundary and 0.06 OneObj's own; Z1 removes the 36 guest calls and most of the lift's register
and frame traffic around them: **~0.1-0.15 ms a frame of the game thread while Wander rides Agro** (about nothing on foot
without Agro near), plus the lake's ClipCollisionDirect probes while swimming.

## 6. Console steps

`build/agent-scratch/z1/` (README.txt): `celosia-z1.vpk` (ELF `celosia-z1.elf`, build p5 = main f6e2a5d + Z1),
`env-z1-off.txt` (= `build/agent-scratch/xy/env-xy-on.txt` + `SOTC_Z2=1`: d1native, W1-W3, X1-X3, Y1-Y3, Z2 on, P2 brackets,
K2, M1PMU, prof129) and `env-z1-on.txt` (off + `SOTC_Z1=1 SOTC_Z1_LOG=600`). Two rides of the usual route (most of it on
Agro: the sheath's probe runs while riding), off then on; send both err.txt. `python3 tools/p2_report.py err-z1-on.txt --elf
build/agent-scratch/z1/celosia-z1.elf`: `collision:z1OneObj` (on), scripts:execScript's self and inclusive in both, the
`[m1pmu]` frame cycles. Expected ~0.1-0.15 ms a frame less while riding.

## 7. Next

- The finder (manager_sub_01116958, 32 calls / ~300 us a frame of self in Vita3K): still the round-38 guest-exact
  transcription (its frame and VU0 registers through the context); typed the same way as OneObj, with
  GetCollisionDBCallBack's per-object loop around it.
- ClipCollisionDirectCallBack's obj125 D1 loop (11-15 calls / ~1 ms a frame of self in Vita3K, the camera's and the
  scripts' probes): already host code; what is left is its guest calls (GetArbitraryDirectionInverseMatrix, the isys
  group walk, the filters) and a per-object host index (S2's plan) rather than another transcription.
- The check costs ~1 s of Vita3K time a call (32 MB copied and scanned twice in the emulated CPU); a round that checks
  many calls should track the regions its function may write instead of scanning the whole RAM.
