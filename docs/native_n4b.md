# Round N4b: native entries with register inputs for loop-free, ABI-proven leaves (2026-11-20)

Plan: docs/native_all_plan.md, step N4 (layers 2 and 10), the step docs/native_n6.md section 6 left next. Base: main
244ea6d. Scratch: `build/agent-scratch/n4b/` in the main checkout (NOTES.md has the exact commands). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c (N6's), 222 MHz, 3 cores, muted.

## 1. The set: docs/n4b_native.txt

`tools/n4_native.py --make-list --abi <tools/n4_abi.py strict output>`: the functions of docs/n5_noloop.txt with
`depth=0` (no call, no loop, no thread switch, not a native / host slot) that tools/n4_abi.py proves in strict mode
(inputs within a0-a3 / t0-t3, sp, ra, gp; callee-saved registers preserved; no runtime call, break or indirect jump).
**2 052 leaves**, 1.60% of the game thread riding (S21's profile; n4_abi.py was rerun on main, same counts as N4).

Of these, **1 199** are lifted functions whose text qualifies (the tool checks the text, not only the proof): they
touch the context only as GPR / FPR / VU0 / named-field loads and stores, make no runtime call and keep no mutable
static. Left out: 215 that write a whole 128-bit register into the context (`ctx->r[n] = ...`, the lq / quadword
moves), 12 with a runtime call or a guest jump in their text. **684** of the 1 199 are called from lifted code
through a NOCHK call (the others are called only from recompiled code, natives or not at all riding): 0.54% of the
game thread riding is those 684 leaves' own time.

## 2. The native entry and the call (tools/n4_native.py, sotc_decomp.h)

For each such leaf F, every lifted batch that calls F gets a copy of F's native entry (`static inline`, in one block
before the batch's first function, between N4 marker comments):

    static inline uint64_t F_n4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n4_r4, ..., float n4_f12, ...)

It is F's own lifted text with three changes: the GPR / FPR loads of its leading declarations come from parameters
(the register's C++ type as F declares it: `uint32_t` for a narrow one, `uint64_t` for a wide one), the resume check
(ctx->pc not on the entry: the generated function) is left out, and when every return stores v0 (479 of the 684) v0
is also returned as a value. Everything else is F's text unchanged: its outputs are still stored into the context, its
guest stack frame is the same, ctx->pc is still set to the return address.

Every NOCHK call site into F in the 52 lifted batches (1 937 sites)

    <write back the dirty locals>
    ctx->pc = <F>;
    if (!PS2_CALL_GUEST[_DIRECT]_NOCHK(<F>, lift_d1::F, <ret>, ...)) { return; }
    }
    <read every slot back from the context>

becomes

    if (PS2_N4_FAST(<the call's own slot expression>, lift_d1::F)) {
        n4_s = PS2_N4_ENTER();                    // the dispatch cycles charged, as the NOCHK fast form does
        [n4_r =] lift_d1::F_n4(rdram, ctx, runtime, <arguments>);
        if (!PS2_N4_DONE(<ret>))                  // the NOCHK fast form's own post-test
            { noCheckFinish(...) or return; read everything back }
        else
            { read back only the slots F's text stores; v0 from n4_r }
    } else { <the call as it was> <read everything back> }

- **Arguments**: the value the write-back just stored for that register (`a0`, `sx64(s1)` ...), else the caller's
  local when it holds the register at least as wide as F declares it (a clean local equals the context), else a load
  from the context. 9 090 arguments over the 1 937 sites.
- **Read back on the native path**: 6 416 of the 47 153 read-back statements of these sites (86% fewer; many were dead
  loads the compiler already removed, but the FPR ones (`f0 = ctx->f[0]; ...`, at 1 873 of the sites) and the
  callee-saved ones were live).
- **PS2_N4_FAST** holds exactly when the NOCHK call would take its fast form (the slot holds F: N3's guard
  `g_ps2DirectCalls` for the DIRECT form, the slot itself otherwise; the host stack above `runtime->m_noCheckFloor`,
  which is the top of the address space unless NOCHK is on, and is never set on the worker's shell runtime) and the
  switch is on (`g_ps2N4Mask`, 0 on / UINTPTR_MAX off, ORed into the floor: one more load). So a native call only ever
  replaces a call that would have run F's body directly, and F's body then runs on the same values.

Why it is exact: the context the native entry sees is the one the plain call would have given F (the write-back is
unchanged and comes first); its inputs are the same values (the stored expression, or a clean local, which equals the
context, or the context itself); its body is F's text; its stores into the context and memory are F's. After it,
every slot F's text does not store still holds in the context what the caller's local holds, so not reading it back
leaves the caller's locals as the read-back would. The EE clock reads the same (the dispatch charge is made), and the
call's post-test and unwind handling are the NOCHK fast form's. A replaced slot (a hook, an A/B swap, a difftest
checker, a lift switch turned off) fails the guard and takes the old call.

What it removes per native call: the callee's input loads (the arguments come in registers or fold away when the
compiler inlines the entry), the read-backs of the slots F does not write, the table / guard load and indirect call
of the NOCHK macro, and the resume check at F's entry; inlined, F's body is optimised with the caller's values. What
it keeps: the caller's write-back before the call and F's stores of its outputs (removing them needs the lifter's
dirty-slot analysis to treat the native call as a non-sync point: the next step, section 6).

## 3. Generators, switch, check build

- `tools/n4_native.py [--list docs/n4b_native.txt] [--undo] [--check] [--log FILE] files...`: idempotent (blocks and
  sites put back first, then the list applied; `--check` also verifies that undo gives the input back); `--undo` back
  to N6's text byte for byte. `--make-list` writes the list.
- `tools/lift_d1.py` runs `n4_native.py --undo` before n3_direct.py (its sites hold NOCHK calls the next passes
  rewrite), then n3_direct.py, n6_nochk.py, then n4_native.py (`--no-n4native` to skip).
- ps2xRecomp: not changed. The recompiled code's NOCHK calls name the recompiled function as the known one, while the
  slots of the leaves this round covers hold their lifted copies, so those calls never take the fast form anyway; a
  recompiler-side native entry needs the recompiler to know the lifted targets first (N3's open item). The runner copy
  (`build/agent-scratch/n4b/runnr`, N6's rewritten copy) is therefore used unchanged.
- Switch `n4native` (`kN4Native`, `SOTC_N4NATIVE`, A/B flag `n4native`), all four tables (addswitch.py); set once a
  frame in sotc_native.cpp's endFrame, on only while NOCHK is (n5sched and n6nochk). `SOTC_N4_LOG=<frames>`: the
  `[n4]` line (frames off / on; in a check build the native calls and the differences).
- `PS2X_N4_CHECK=1` (define): the check build. Before each native call every argument that is not a context load is
  compared with the context (`PS2_N4_ARG`), after it every local not read back is compared with the context
  (`PS2_N4_KEEP`), bytewise. Both counts 0 means the two premises of the exactness argument held for every native call
  of the run.

## 4. Checks and measurements (Vita3K 222 MHz; off - on, ms a frame, ± 2 SE)

Builds: p1 (the A/B build: n4native off unless `SOTC_N4NATIVE=1`), pc (the check build: p1 with `PS2X_N4_CHECK=1`),
p2 (the default on). Only the overrides rebuilt (the build dir was N6's, re-pointed: NOTES.md).

- **Exactness, check build** (pc, ride7 500 s, `SOTC_N4NATIVE=1`; chkR): by frame 13 800 (mounted at frame 9 352,
  then riding) **5 438 666 native calls, 0 values passed that differ from the context, 0 kept locals that differ**.
  So both premises held for every native call; with the body being the leaf's own text, every native call computed
  what the plain call computes. About 390 native calls a frame (376 a frame riding).
- **In-run A/B** (`SOTC_AB_FLAG=n4native SOTC_N4NATIVE=1`, `SOTC_AB_MIX`, mixframes; on = native entries):

| run | off - on (ms a frame) | ± 2 SE | z |
|---|---:|---:|---:|
| ride7 800 s, seed 7 (mixR7) | +0.096 | 0.122 | +1.6 |
| ride7 800 s, seed 13 (mixR13) | +0.070 | 0.142 | +1.0 |
| lake6 400 s (mixL7) | +0.048 | 0.101 | +1.0 |
| **pooled** (mean of three, SE = sqrt(sum SE^2) / 3) | **+0.071** | **0.071** | **+2.0** |

  On is faster in all three, by ~0.07 ms a frame (0.2% of the game thread): at the edge of what the A/B resolves.
- Rides: mounted at frames 3 072 / 2 900 and rode the whole 800 s; `[n6]` unwinds after a skipped test 0 in every run
  (the native path's fallback, `noCheckFinish`, counts there too).
- **Share of the game thread**: the 684 leaves are 0.54% of the game thread riding in S21's profile (their own time);
  in this round's mixed runs the leaves' plain entries are 0.37-0.40% and the native copies the compiler did not inline
  0.07% (most native entries are inlined into their callers, so their time shows as the callers'). On frames, the
  calls from lifted code into them (~390 a frame) all run through native entries; calls from recompiled code and
  natives still use the plain entries. Of all the generated code's time (19.3% of the game thread, docs/native_all_plan.md),
  roughly 0.5 points is now reached through native entries.
- spalign (p1.elf): 0 sp-based :128 / :256 NEON accesses (of 1 012).

## 5. Boot

bootR (p1, ride7 600 s, `SOTC_VITA_EAT_USER_MB=89` taken, `SOTC_N4NATIVE=1`, shots every 120 s): booted on the first try,
`[n4]` native on every frame from GAMECORE's NOCHK on (17 982 on frames by frame 18 000), mounted Agro at frame 8 481
(the route's teleport tries), 32.8-33.8 game fps at the end, no fault or missing target. Pictures normal: Wander running
on the plain (ground, shadow, sky, clouds, ruins), Agro at the shrine's walls, the cliff passage.

## Default

**On** (`SOTC_N4NATIVE=0` turns it off; sotc_decomp.h switchOn): exact (the check build found 0 differing arguments
and 0 differing kept locals in 5.4 M native calls; the body is the leaf's own text), not slower (pooled +0.071 ± 0.071
ms, z +2.0, on faster in all three runs), the rides and the boot normal. It only takes effect while NOCHK does (n5sched
and n6nochk, from GAMECORE's entry on). Default build p2 (no env; ride7 300 s, d4def): `[n4] ... native 1` on every
frame after GAMECORE, mounted at frame 3 130, rode at ~33 game fps, 0 unwinds after a skipped test, pictures normal
(Agro and Wander on the plain, sky, ruins); 0 sp-based NEON accesses on p2.elf.

What it buys is small, as expected for ~390 calls a frame into leaves that are ~0.5% of the game thread: ~0.07 ms a
frame. Its value is structural: for these calls the guest calling convention is gone at the call itself (no table
call, no context round trip for the inputs, no read-back of what the callee does not write), which is the shape the
next steps extend.

No sound compare was run: the change is exact by the check (the same values computed, the same stores), and sound
does not depend on when a call is made.

**What reaches main's build**: everything this round changes is in the worktree's tracked files (tools/n4_native.py,
tools/lift_d1.py, docs/n4b_native.txt, sotc_decomp.h / sotc_native.cpp, the 52 lifted batches), so main's build gets
the native entries and calls as soon as it is merged. Nothing in the runner (build/runner, generated, gitignored)
changes this round; the N6 NOCHK rewrite of the runner copy that this round's builds used
(build/agent-scratch/n4b/runnr = N6's n6/runner) still reaches main's build only when the runner is regenerated with
`PS2X_RECOMP_NOCHK_LIST` (docs/native_n6.md section 3).

## 6. What is next

- Remove the write-back before a native call and the output stores after it: the lifter's dirty analysis
  (lift_d1.py `analyse_dirty`) has to treat a native call as a non-sync point (dirty slots stay dirty across it; the
  callee's outputs become dirty locals), which needs the lifter, not a text pass: regenerate the batches (D5's
  regen.sh reproduces D1-D4 byte for byte; the later batches need their own commands).
- Depth-1 and deeper functions of n5_noloop.txt: a native entry whose own calls are native can still fall back to the
  plain call when a slot below it is replaced, so its callers must read back everything unless the fallback is
  tracked; the leaf-only set has no such case.
- The 215 leaves with 128-bit register stores (`ctx->r[n]`): add the quadword slot to the written set.
- The recompiled code's calls into lifted leaves: give ps2xRecomp the lifted target list (N3's next step).
