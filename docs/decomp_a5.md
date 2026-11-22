# Round A5: the typed ABI proof, relocated calls, guest frames, jump tables (a5typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5; the wall A4 left: n4_abi's unproven, relocated calls, the frame's address
escaping, 64-bit pass-through arguments, indirect jumps, and the 2 973 functions waiting on an untyped callee). Base: A4
(b29cd5e). Scratch: `build/agent-scratch/a5/` in the main checkout (NOTES.md has every command). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. What blocks what: the unlock table

`tools/typed_lift2.py --unlock FILE` (new) writes, for each refusal reason, how many *waiting* functions (untyped only
because some callee is untyped) it blocks. For each waiting function w, roots(w) = the untyped, non-waiting functions
reachable from w through untyped callees (a fixpoint over the call graph). "Blocks" = waiting functions with at least one
root of that reason; "alone" = waiting functions whose every root has that reason (all of them would be tried if the
reason went away). `build/a5w/whatif.py` splits the blockers further and simulates fixes.

At A4 (2 973 waiting, 5 of them only on a recursion cycle):

| reason | refused | waiting it blocks | waiting it alone blocks |
|---|---:|---:|---:|
| not lifted once: a hand native holds the slot (524) / never lifted, recompiled only (3 024) | 3 548 | 2 465 (natives 2 404, never lifted 741) | 795 (natives 740) |
| callee outside the IR (WaitSema / SignalSema / GetThreadId syscalls, iosJumpRecoverPoint, memset / memcpy / strcmp / vsprintf ...) | 123 | 1 369 | 278 |
| **not proven by tools/n4_abi.py** | 177 | 1 325 | 92 |
| **an indirect call / jump** | 22 | 456 | 4 |
| **the frame's address escapes** | 38 | 411 | 14 |
| **a relocated call** | 63 | 309 | 15 |
| reference text form | 28 | 273 | 5 |
| docs/a3_skip.txt (the five long loops) | 5 | 211 | 0 |
| a hi / lo input | 5 | 208 | 3 |
| a loop whose exit may wait on memory | 25 | 195 | 3 |
| a 64-bit return into a narrow local | 12 | 184 | 1 |
| **a 64-bit argument from a 32-bit local** | 34 | 139 | 3 |
| a scratch register a caller reads | 27 | 128 | 0 |
| ctx use | 2 | 128 | 0 |
| a callee's scratch read after the call (typed caller) | 4 | 89 | 0 |
| a restored register read | 5 | 71 | 14 |
| sp step form | 12 | 54 | 2 |
| others (a typed callee refused, FPU spin loops, caller's frame, frame forms, libc names) | 25 | 38 or fewer each | 6 |

The single biggest blockers: GetScriptLayoutClass (a native, 1 027 waiting behind it), GetLightClass (native, 995),
dynamic_cast_to_camera_obj (not proven, 995), WaitSema / SignalSema (757 / 753), GetCharacterClass (native, 678),
GetThreadId (675), iosJumpRecoverPoint (651), memset (570).

What the table says, honestly: the five classes this round was asked to remove (bold) refuse 334 functions, but the
waiting functions above them are mostly also behind a hand native or a syscall. Fixing all five completely could unlock
at most **132** waiting functions; with the natives also callable it would be 1 273, with the outside-the-IR callees
433. The 2 973 do not come in by widening the proof alone: the natives (phase D) and the syscalls / host threads
(phase B) are the real wall.

## 2. The widenings (stage 5, `Emit(..., a5=True)`), and why each is exact

Stage 5 runs on every function stages 2-4 did not type (3 445 candidates), depth by depth to a fixpoint (a function is
tried once all its callees are typed), with the caller check and A3's re-emit for extra outputs, VU0 allowed (A4's
interface). Stages 2-4 give A2's / A3's / A4's output byte for byte (checked on every run: typed_a2/a3/a4.inc and their
check files unchanged).

**W1. The typed ABI proof** (`Emit.typed_abi`). N4's proof (tools/n4_abi.py) takes a callee whose slot may hold a native
(any function whose address appears as a hex literal in the overrides) as reading and writing every register, so its
callers come out "inputs at, fp, k0, k1, s0-s7, t4-t9, v0, v1; outputs s0-s7": unproven. And it requires the inputs to
be within the eight argument registers. In stage 5 every callee is a typed function, and a typed caller calls it directly;
the adapter tests that every reached slot holds its lifted function (A5_HOLDS), so the callee's own interface is the one
that holds. typed_abi reruns N4's analyses (the same code: `A.inputs_of`, `L.function_gpr_writes`, on the Emit's own
Lifter through the new `n4_abi.facts_from`) with each callee's inputs = its typed parameters and its changes = its
scratch set and return slots. Inputs need not be argument registers: each input becomes a parameter (the adapter loads
it from the context; a typed caller passes its own value, which N4's input analysis makes one of the caller's own inputs
or a register written on every path before the call, the same argument A2-A4 rely on). Still required, as N4 requires:
callee-saved registers and sp back at every exit, no runtime call, break, jalr or jump through a register without a case
list. Exact because nothing about the code changes, only which interface the proof assumes for the callees, and that
interface is enforced by the slot test. (Example: 55 of the 177 read v0 on entry, a value passed through on some path;
they take v0 as a parameter, and the extra-output rule writes it back only where the lifted text would.)

**W2. Relocated calls** (`A5_JAL`). A jal / j whose target lift_d1 reads from the instruction word in RAM
(`jumpTarget = (READ32(site) & 0x3FFFFFF) << 2`): the XFF modules' import sites, which the game's dynamic linker
rewrites at load time (all 179 sites the A5 functions reach are in build/recomp/game/dynamic_imm.txt). The word as
tools/build_elf.py links SOTC.elf gives the static target the lifted text names (`PS2_CALL_GUEST_KNOWN(jumpTarget,
lift_d1::<callee>, ...)`). The typed function calls that target; the adapter tests, for every relocated site the typed
function reaches (its own and its typed callees', transitively), that the word in RAM still encodes it
(`A5_JAL(site, target)`), next to the slot test of that target. Exact: when both hold, the lifted text's jump goes to
the same lifted function the typed call stands for; code words do not change during a typed call (nothing else runs).
When one fails, the lifted text runs. Relocated tail jumps (`PS2_JUMP_GUEST(jumpTarget)`) are handled the same way.

**W3. The guest frame** (`Emit.guest_frame`). When the host-frame analysis refuses because the frame's address escapes
(a vector or a struct built on the stack and passed to a callee by pointer: `addiu a1, sp, 0x10`, `daddu v0, sp, zero`),
a load reads frame bytes not written before, a slot reaches into the caller's frame, a slot has an lwl / lwr form, or sp
is a 64-bit local (its step sign-extends), the function keeps its frame in guest RAM below the guest sp, exactly as the
lifted text does: `sp` is a local guest address (a parameter: the adapter reads it from the context, a typed caller
passes its own sp at the call: a guest-frame caller its local, a host-frame caller its entry sp moved by its own host
frame's offset at the call, so the callee's guest frame is at the lifted text's addresses), moved by the same steps,
and the frame's loads and stores are guest memory accesses at the same addresses. A function that calls a guest-frame
callee gets sp as a parameter too (the "gsp" form), its own frame staying host. Only the prologue's saves of callee-saved
registers and ra go, with their restores, as for a host frame (a typed function keeps its caller's registers); a load
from a save slot other than its restore refuses the function. Still no context: sp is a guest address like any pointer.
Quadword frame accesses go through A4_R128 / A4_W128 on the guest address (low 4 bits cleared as lq / sq do: no
alignment assumption). Exact under one stated condition: nothing stores into the function's own save slots between the
save and the restore (in the game that would be a stack overflow; section 4 shows how the check enforces it). The user's
"keep the rest as host locals" is not done: a guest-frame function keeps its whole frame (minus the saves) in guest RAM,
because a pointer's extent cannot be bounded statically; the frame-local slots of functions whose address does not
escape stay host arrays as before.

**W4. 64-bit inputs passed whole.** A 32-bit local passed to a callee's 64-bit parameter while not written on every
path refused the function. When the register is an input and not written on *any* path before the call (a may-written
analysis), the lifted caller leaves it untouched in the context, so the callee reads the whole 64-bit value; the typed
function now takes that value (`uint64_t w64_<r>`, the 32-bit local its low half) and passes it on. Mixed paths still
refuse.

**W5. Jump tables.** A `jr` whose recompiled code has a case list (`switch (jumpTarget) { case ...: goto ...; }`) becomes
a C++ switch over the same targets, the register read before the delay slot as MIPS does, no checkpoint charged (lift_d1
charges none there either). A value outside the list (lift_d1 jumps out of the function to it; never in this game, the
tables' bounds checks come first) stops in `A5_BADJUMP` (an abort with the site and target). All 22 indirect-jump
refusals were jump tables; jalr (25) stays refused.

## 3. The set

**235 more functions typed** (docs/a5_typed.txt). By the reason A4 refused them: not proven 66, relocated call 42, frame
address escapes 32, 64-bit pass-through 30, indirect jump 22, reference text form 17 (their lifted text has a
PS2_JUMP_GUEST the A5 check dispatches), frame access form 4, caller's frame 3, frame read before write 2, sp step form 1,
and 16 that were waiting on an untyped callee (their callees came in this round).

Forms (one function can have several): typed ABI proof only (N4 did not prove it) 74, guest frame 76, guest sp passed
through 1, jump tables 22 (25 tables), relocated calls 115 functions (179 sites), 64-bit inputs 30.

| tier | functions |
|---|---:|
| no frame: leaf / loop leaf / 128-bit leaf / 128-bit loop leaf / VU0 leaf | 11 / 2 / 1 / 1 / 1 |
| host frame, depth 1 / 2 / 3 | 104 / 8 / 1 |
| host frame with VU0 or 128-bit, depth 1-2 | 20 |
| host frame with a loop (leaf 1, depth 1-2 16; VU0 / 128 included) | 17 |
| guest frame leaf (VU0 / 128 included) | 30 |
| guest frame depth 1-2 (VU0 included) | 23 |
| guest frame with a loop (VU0 / 128 included) | 16 |

Returns: uint64_t 75, a1::RetVF 73, uint32_t 70, float 11, void 6.

**Refused in stage 5 (303; the other 2 907 have a callee that is never typed, transitively):**

| why | functions |
|---|---:|
| a scratch register a lifted caller reads that cannot be an extra output | 74 |
| a loop whose exit may wait on memory (5 through an FPU compare) | 50 |
| not proven: a runtime call (syscall, mfc0 / ei / di, mult1, cfc2 ...) / jalr / break / combinations | 33 / 25 / 11 / 4 |
| a 64-bit return into a narrow local | 30 |
| a hi / lo input | 23 |
| a restored register read | 17 |
| a callee's scratch read after the call (typed caller; mostly f20-f23 a callee saves and restores) | 15 |
| reference text form | 8 |
| a typed callee refused 9, ctx use 2, an upper half of a callee-saved register written 2 | 13 |

The unlock table after A5 (2 907 waiting): natives 2 404 blocked / 765 alone, outside the IR 1 369 / 280, not proven:
runtime 1 143 / 46 (dynamic_cast_to_camera_obj's `break 0` on its failed type check, 995 behind it), live scratch 309 /
0, everything else 267 or fewer. Fixing every remaining refusal of the lifter's own kinds would unlock at most 92 more
waiting functions.

## Measurements

**In-run check** (pc: A3 + A4 + A5 checks, 2026-11-22):

| run | A5 checks | A5 functions reached (of 235) | A4 checks | A3 checks | differences |
|---|---:|---:|---:|---:|---:|
| ride7 | 1 656 646 | 129 | 2 242 055 | 4 464 373 | 0 |
| attract | 1 252 670 | 116 | 1 195 906 | 2 487 744 | 0 |
| lake6 | 1 127 537 | 142 | 1 010 293 | 2 339 190 | 0 |
| **total** | **4.04 M** | | **4.45 M** | **9.29 M** | **0** |

0 in every category (return, memory, pc, sp, extra outputs, VU0 state). The guest-frame functions' dead-stack stores
(the reference's frames below the entry sp) are left out by design (2.4-3.9 M per run).

**Vita3K A/B** (222 MHz, p1, flag a5typed, off - on, 2 SE): ride7 seed 7 +0.137 +- 0.271, ride7 seed 13 +0.066 +- 0.145,
lake6 +0.070 +- 0.102; **pooled +0.091 +- 0.108 ms: even** (not slower). The first attempt's A/Bs (before the hand-back)
were cut short by the hand-back itself, not by a fault.

**89 MB boot** (p1, ride7 600 s): EAT_USER_MB=89 taken, mounted at frame 4 540, rode to the end; pictures normal
(plain, the bridge and its arches, Agro at a gallop, shadows). spalign p1.elf: 0 sp-based NEON accesses.
**On by default** (`SOTC_A5TYPED=0` off).

**Count** (tools/decomp100_count.py): class (d) **1 936 functions (21.45%)**, 6.06% of the MIPS bytes, 6.66% of the
console game thread (A4: 1 709 / 18.94% / 4.12% / 5.75%).
