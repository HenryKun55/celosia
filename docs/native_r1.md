# Native R1: the display walk's sort-list inserts as host code (r1sort)

Round R1 of the "everything native" order (docs/native_all_plan.md): the hand-written natives that still have the
guest signature take 52.5% of the game thread riding (17.2 ms of ~33 at Vita3K 222 MHz), and N1-N5 showed the time
is in them, not in the generated code. R1 takes the display / model cluster of that profile
(build/agent-scratch/n3/mixR7e.share.txt: "Display and models" 23.3% of the game thread, 7.05 ms a frame).

Scripts, runs, logs: `build/agent-scratch/r1/` (NOTES.md has the exact commands).

## 1. What the cluster is in the code

The symbols the brief named are not one path:

| symbol | share riding | what it is | called from |
|---|---|---|---|
| sotc_native::modelDispHook | 1.43% | the model draw hook (the renderer's per-model plan / draw) | the game's model display, through the hook table |
| decomp_walk::quad117BodyA9 | 1.41% | the immediate-mode quad builder, already host-shaped (G, Vert117 arrays) | particle / haze quads |
| decomp_prefetch::objectLines67 / window | 1.15% / 0.43% | the guest-line prefetch planner (host code over guest RAM) | the walk and the hook |
| decomp_walk::dispDispObjGroupModel | 1.14% | the display walk (DispDispObjGroupModel, 0x10f52c8): the group's layout objects, their classes, the sort lists | once a frame per camera |
| decomp_dl::addSortObj | 0.77% | AddSortObj (0x110dc20): one node into a bucketed, key-ordered sort list | the walk (5 sites) and other display code |
| decomp_walk::clip83 / nextSortObj | 0.57% | host clip / sort-list walk (already host code) | the walk |
| decomp_pointsort::sortFast | 0.51% | the point sort (its own module) | particles |
| decomp_layoutclass::getLayoutClass | 0.61% | GetLayoutClass (0x10f1a20): a type lookup | ~40 call sites across collision, characters, camera; the walk already inlines it |
| sotc_native::vertex2D / immFeed128 | | the 2D / immediate-mode feeders of the renderer | HUD, particles |

The walk (dispDispObjGroupModel) is the one place where several of them meet: per layout object it inlines
GetLayoutClass (round 49), walks the sort lists with host code (nextSortObj, round 140's host bucket words), and
called AddSortObj through the guest function table (`callGuest`: arguments into the context, the table lookup, the
native's own context reads and its register write-backs) at five sites.

## 2. This round: r1sort

`SOTC_R1SORT` (A/B name `r1sort`, `kR1Sort`, default on). AddSortObj for the walk's calls as plain host code:

- **Structs** (game/overrides/r1sort.h), overlaid on guest RAM with one host pointer each, resolved once:
  `SortList { uint32_t buckets; int32_t count; float scale; int32_t mode; }` and
  `SortNode { uint32_t word0; float key; uint32_t toward; uint32_t back; }` (may_alias, 4-byte aligned, no
  by-value vector members).
- **The function** (decomp_dl.cpp `addSortHost`): the same reads, the same compares in the same order through the EE
  FPU helpers (FPU_MUL_S, FPU_CVT_W_S, FPU_C_OLT_S), the same stores; no context register is read or written, no
  table call, no fcr31 condition bit. It declines (nothing written, the guest call made instead) exactly where the
  guest native falls back to the recompiled code (an address out of RAM or misaligned).
- **The walk** (decomp_walk.cpp `r1sort::add`): the five sites (0x10f559c, 0x10f5614, 0x10f56a0, 0x10f56b0 twice: the
  layout objects, the geodisp objects, the second list's copy loop, round 95's late particle geodisps) call it
  directly. The registers AddSortObj clobbered (v0, v1, a0, a2-t1, f0-f2, the condition bit) are dead after each site
  (the walk's own code sets everything it uses next; MIPS caller-saved).
- **The guest entry point** (decomp_dl::addSortObj, 0x110dc20) is unchanged for every other caller.

## 3. Check

`SOTC_R1SORT_CHECK=1`: at every walk site the host version runs in record mode (its stores noted, not applied,
together with every link word it read), the guest call runs, and every one of those words is compared with what the
guest call left (`[r1sort] checked N, differ D, host declined F` every 20 000 calls; the first 20 differences printed
with addresses). The picture under the check is the guest's.

Results (build p1, Vita3K 222 MHz): **0 differences, 0 declined** in 2 480 000 calls riding (chkR, ride7 400 s),
1 520 000 at the lake (chkL, lake6 300 s) and 1 240 000 in the attract demo (chkA, 300 s). chkR's shots: Wander on
Agro, ground, sky, buildings normal.

## 4. A/B

In-run A/B (SOTC_AB_FLAG=r1sort, SOTC_AB_MIX per-frame random flip, mixframes), Vita3K 222 MHz: PENDING
(mixR7p1 ride7 800 s seed 7, mixR13p1 seed 13, mixL7p1 lake6 400 s).

## 5. Boot

600 s ride7 with SOTC_VITA_EAT_USER_MB=89 (bootRp1): PENDING. spalign (p1.elf): 0 sp-based :128/:256 NEON accesses (of 1 013 with an alignment hint).

## 6. What is left of the cluster

- modelDispHook (1.43%) and the walk's own body: the walk still keeps the guest frame (the prologue's saves at sp,
  `resume()` into the recompiled code at its loop heads when an address is bad) and calls the classes' update /
  draw callbacks (guest function pointers at +0xd8 etc.) through `callGuest`; those callbacks are where the
  remaining guest traffic is. The next step is the callbacks the walk reaches most riding (their targets from the
  sampler) as host functions called directly, then the walk without its guest frame.
- GetLayoutClass's other ~40 callers (collision, characters, camera) still call it through the table: a host
  `layoutClassOf(rdram, obj)` (the walk's inline version) at those sites is mechanical.
- quad117BodyA9, clip83, nextSortObj, objectLines67 are already host code over guest RAM: what they cost is their
  work, not guest traffic.
