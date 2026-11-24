#!/usr/bin/env python3
"""typed_lift2.py: round A2, the typed lifter's second stage (docs/decomp_a2.md; docs/decomp_100_plan.md T1 / T4 / T5).

Round A1 (tools/typed_lift.py) rewrote lift_d1's text of 979 loop-free leaves without a stack frame as real C++
functions. This stage writes typed functions from lift_d1's IR directly (tools/lift_d1.py's Lifter: the CFG, delay
slots, each instruction's statement, the register widths) for what A1 left out:

  - frames: the guest stack frame becomes a host array `a2fr` (constant offsets: the compiler keeps the slots in
    registers); the prologue's saves of callee-saved registers and ra and their restores go (a C++ function keeps its
    caller's registers); sp is no longer a value. Refused when the frame's address escapes (any use of sp other than
    `addiu sp, sp, imm` and loads / stores at a constant offset), when a load reads frame bytes not written on every
    path before it, or when it touches the caller's frame;
  - calls: a `jal` / tail `j` to a typed function (A1's or this stage's) is a C++ call with the callee's typed
    signature (its parameters from the caller's locals, its return value into v0 / f0), after charging the EE clock the
    call's dispatch cycles as the lifted call does (A2_CHARGE); a tail jump returns the callee's value. Iterated upward:
    depth 1 (callees all A1), 2, 3 ... while the proof holds;
  - the liveness proof (A1's caller check) refined: inside a typed caller the registers live at its return are only the
    ones it returns (everything else it or its callees change is its own scratch set, checked at its callers), and a
    tail call's effects are the caller's; so leaves A1 refused can come in once their callers are typed.

Only loop-free (docs/n5_noloop.txt), ABI-proven (tools/n4_abi.py), lifted-once functions; no VU0 / 128-bit values, no
indirect call or jump, no runtime call. Emission: one label per branch target, `goto` for branches, locals for every
register, no R5900 context, no guest sp, no function table.

Guest entries (as A1): the lifted function's body starts with
    if (A2_GO()) { inputs from ctx; typed call; v0 / f0 into ctx; ctx->pc = ra; return; } // a2
A2_GO (typed_a2_rt.h): the switch a2typed, N3's direct-call flag (every direct target's slot holds its lifted function,
so the typed callees are what the calls would run) and the NOCHK fast-path condition (the lifted calls would charge,
not test, the checkpoint). Exactness: the in-run check (PS2X_A2_CHECK=1: each adapter call compared with the lifted
text, its calls dispatched to the lifted texts of the callees, all on logged memory; stores below the entry sp, the
dead stack, left out) and the offline harness (tools/a2_harness.cpp).

Usage: typed_lift2.py [--cache facts.pkl] [--abi abi_all.txt (tools/n4_abi.py --all)] [--undo] [--check] [--log FILE] [--check-tu] files...
Run after typed_lift.py (lift_d1.py does); --undo before typed_lift.py --undo. Idempotent.
"""
import argparse
import collections
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lift_d1 as L  # noqa: E402
import n4_native as N4  # noqa: E402
import n4_abi as A  # noqa: E402
import n1_basemem  # noqa: E402
import typed_lift as T1  # noqa: E402
import n3_direct as N3  # noqa: E402  (LIBC: names a typed function must not take)

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OV = os.path.join(REPO, 'game/overrides')
INC_LINE = '#include "typed_a2.h" // a2\n'
ADAPT_MARK = ' // a2'
INC_LINE3 = '#include "typed_a3.h" // a3\n'
ADAPT_MARK3 = ' // a3'
INC_LINE4 = '#include "typed_a4.h" // a4\n'
ADAPT_MARK4 = ' // a4'
INC_LINE5 = '#include "typed_a5.h" // a5\n'
ADAPT_MARK5 = ' // a5'
INC_LINE6 = '#include "typed_a6.h" // a6\n'
ADAPT_MARK6 = ' // a6'
INC_LINE7 = '#include "typed_a7.h" // a7\n'
ADAPT_MARK7 = ' // a7'
INC_LINE8 = '#include "typed_a8.h" // a8\n'
ADAPT_MARK8 = ' // a8'
INC_LINE9 = '#include "typed_a9.h" // a9\n'
ADAPT_MARK9 = ' // a9'
INC_LINE10 = '#include "typed_a10.h" // a10\n'
ADAPT_MARK10 = ' // a10'
INC_LINE11 = '#include "typed_a11.h" // a11\n'
ADAPT_MARK11 = ' // a11'
INC_LINE12 = '#include "typed_a12.h" // a12\n'
ADAPT_MARK12 = ' // a12'
INC_LINE13 = '#include "typed_a13.h" // a13\n'
ADAPT_MARK13 = ' // a13'
INC_LINE14 = '#include "typed_a14.h" // a14\n'
ADAPT_MARK14 = ' // a14'
SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31}
LOADS = {0x20: 1, 0x24: 1, 0x21: 2, 0x25: 2, 0x23: 4, 0x27: 4, 0x31: 4, 0x37: 8}
STORES = {0x28: 1, 0x29: 2, 0x2B: 4, 0x39: 4, 0x3F: 8}
BADMEM = {0x22, 0x26, 0x2A, 0x2E, 0x1A, 0x1B, 0x2C, 0x2D, 0x1E, 0x1F, 0x36, 0x3E}  # lwl / lwr / swl / swr / ldl ... lq / sq
RET_SLOTS = {'void': set(), 'uint32_t': {('g', 2)}, 'uint64_t': {('g', 2)}, 'float': {('f', 0)}, 'a1::RetVF': {('g', 2), ('f', 0)}}


class Refuse(Exception):
    pass


class A7Restart(Exception):
    """A7: the function is emitted again with more inputs / wide registers (its callees' extra outputs it consumes)."""
    pass


def xo_rd(k, q, ck=False):
    """An extra output's value in the context (the adapter's / check's xo array start)."""
    if k == 'g':
        return f"Ps2GprU64(ctx, {q})"
    if k == 'f':
        return f"(uint64_t)a1ck::ckBits(ctx->f[{q}])" if ck else f"(uint64_t)a1_bits(ctx->f[{q}])"
    assert (k, q) in HL13, (k, q)
    return f"ctx->{q}"  # A13: hi / lo (uint64_t)


def xo_wr(k, q, i):
    """The adapter's write-back of extra output i."""
    if k == 'g':
        return f'Ps2SetGpr64(ctx, {q}, a3_xo[{i}]); '
    if k == 'f':
        return f'ctx->f[{q}] = a1_flt((uint32_t)a3_xo[{i}]); '
    assert (k, q) in HL13, (k, q)
    return f'ctx->{q} = a3_xo[{i}]; '


def xo_ck(k, q, j):
    """The check TU's comparison of extra output j with the reference context c."""
    if k == 'g':
        return f'Ps2GprU64(&c, {q}) != xo[{j}]'
    if k == 'f':
        return f'a1ck::ckBits(c.f[{q}]) != (uint32_t)xo[{j}]'
    assert (k, q) in HL13, (k, q)
    return f'c.{q} != xo[{j}]'


def xo_nm(k, r):
    return L.GPR[r] if k == 'g' else ('f%d' % r if k == 'f' else str(r))


class NeedXout(Exception):
    """A7: a scratch slot of a typed callee is read after the call (live in this function) and the callee does not give it
    back as an extra output: {callee address: slots}. The generator asks the callee for them (stage 7's want / variants)."""
    def __init__(self, need):
        Exception.__init__(self, 'a7: a callee extra output needed')
        self.need = need


# A5: the host frame's refusals the guest frame takes (the frame in guest RAM below the guest sp, as the lifted text has it)
GF_REASONS = {'frame address escapes', 'frame access form', "caller's frame", 'frame read before write', 'frame quadword alignment'}


class Sig:
    """A typed function's interface: parameters (('g', r, ctype) / ('f', r)), return type, v0 width, scratch slots."""

    def __init__(self, name, addr, rtype, params, dropped, v0w=32, tier='a1'):
        self.name, self.addr, self.rtype, self.params, self.dropped, self.v0w, self.tier = name, addr, rtype, params, dropped, v0w, tier
        self.guards = []  # A2: (address, lifted name) of callees whose slot the adapter tests (not N3 direct targets)
        self.holds = set()  # A2: (address, lifted name) of every typed function it reaches through calls (transitively):
        #                     the adapter tests that each slot holds its lifted function (what the lifted calls would run)
        # A4: the VU0 interface (tools/typed_lift2.py stage 4, the VU0 ABI): the VU0 / 128-bit state slots it reads on
        # entry (vin), may write (vout), writes on every path (vmust); vio: the reference parameters, in order
        self.vin, self.vout, self.vmust, self.vio = frozenset(), frozenset(), frozenset(), ()
        self.brk = False  # A6: reaches a `break` (the adapter applies its cop0 effects after the call: A6_BRKS)

    def inputs(self):
        return {('g', p[1]) for p in self.params if p[0] == 'g'} | {('f', p[1]) for p in self.params if p[0] == 'f'} | {('x', p[1]) for p in self.params if p[0] == 'x'}


# A6: the symbol a slot must hold for a typed caller to call the typed callee (A6_HOLDS): the lifted function unless listed
# here (a never-lifted function's recompiled function, a hand native typed from its MIPS, a runtime stub's native)
GUARD_SYM = {}
NO_BRK6 = [False]  # --no-brk6 (measuring)
A13_DIAG = [False]  # --a13-diag: the slots in the field refusal's text
REC13_OFF = [False]  # --no-rec13 (measuring): no self-recursive variants
HL13_OFF = [False]  # --no-hl13 (measuring): stage 13 without hi / lo as extra outputs
HL13 = {('x', 'hi'), ('x', 'lo')}  # A13: the field slots an extra output may be (whole 64-bit registers: mult / div / mthi / mtlo write them whole)
RT14_OFF = [False]  # --no-rt14 (measuring): stage 14 without the runtime state as VU0-interface slots
BRK14_OFF = [False]  # --no-brk14 (measuring): stage 14 without a likely branch's break slot
SRCH14_OFF = [False]  # --no-srch14 (measuring): stage 14 without the bounded-search exit rule
CSR14_OFF = [False]  # --no-csr14 (measuring): stage 14 without the callee-saved read-before-write refusal
# A14 (RT14): context state lift_d1 leaves to the runtime (an opaque statement) that is plain CPU state, read and written by
# the instructions alone (no runtime call, no scheduler reads it between two checkpoints): the second multiply pipeline's
# hi1 / lo1 (mult1 / div1 / mflo1 ...), the shift-amount register (mtsab / qfsrv), COP0 Status (mfc0 / ei / di: EIntr, DIntr;
# the runtime reads it only for BEV / EXL in an exception's vector), VU0's clip flags (vclipw / cfc2 vi18) and R register (vrinit /
# vrnext / vrxor). lift_d1 names them as locals only in stage 14's lifters (RT14_LIFT, set around each stage-14 Emit), so stages
# 2-13 never see these slots; in the typed function they are slots of the VU0 interface (VU_FIELDS: reference parameters, the
# adapter loads and stores the context field: nothing dropped).
RT14_LIFT = {'lo1': ('lo1', 'uint64_t'), 'hi1': ('hi1', 'uint64_t'), 'sa': ('rsa', 'uint32_t'), 'cop0_status': ('c0st', 'uint32_t'),
             'vu0_clip_flags': ('vclip', 'uint32_t'), 'vu0_r': ('vrr', '__m128')}
A14_KNOWN = {'a4m::qfsrv'}  # A14: typed_a14_rt.h's helpers (QFSRV over the SA local)
XO7_OFF = [False]  # --no-xo7 (measuring): stage 7 without callee extra outputs consumed by typed callers
CNT12_OFF = [False]  # --no-cnt12 (measuring): stage 12 without the counted-loop exit rule
LIK12_OFF = [False]  # --no-lik12 (measuring): stage 12 counts a likely branch's restore live on both edges
PT12_OFF = [False]  # --no-pt12 (measuring): stage 12 refuses a 64-bit pass-through of a callee-written register at once
SPIN12_OFF = [False]  # --no-spin12 (measuring): stage 12 with A8's spin_flow (the base's taint flow-insensitive)
SPIN8_OFF = [False]  # --no-spin8 (measuring): stage 8 with spin_filter's flow-insensitive taint
UH10_OFF = [False]  # --no-uh10 (measuring): stage 10 without the upper-half variants
MUT10_OFF = [False]  # --no-mut10 (measuring): stage 10 with self recursion only
# A10: the functions docs/a3_skip.txt left out for A6's upper-half class (a callee's 128-bit move not in its typed interface):
# stage 10 takes them again, with the callees' exact variants (Stage7.uh_*)
UH10 = ('IsParentCharOrLwsOrient', 'AddLayoutObjSubLink', 'SelectOmakeItem', 'gamecore_sub_013828A8')
# A10: the camera note: the typed function of iosgluGetCameraContext returns through A10_CAMNOTE (typed_a10_note.h: the note
# sotc_native's hook makes, while the hook holds the slot), so its typed callers leave the renderer's camera context as the hook does
NOTE10 = {0x108F518: 'A10_CAMNOTE'}


def rec_sccs(F, self_only=False):
    """A10: {function: frozenset(its cycle)} for the call graph's cycles (Tarjan): a self-recursive function alone, or the members
    of a strongly connected component of several functions (not with self_only)."""
    import sys as _s
    G = {a: sorted({n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail') and n[1] in F}) for a, f in F.items()}
    idx, low, on, st, out = {}, {}, set(), [], {}
    cnt = [0]
    lim = _s.getrecursionlimit()
    _s.setrecursionlimit(max(lim, 50000))

    def sc(v):
        idx[v] = low[v] = cnt[0]
        cnt[0] += 1
        st.append(v)
        on.add(v)
        for w in G[v]:
            if w not in idx:
                sc(w)
                low[v] = min(low[v], low[w])
            elif w in on:
                low[v] = min(low[v], idx[w])
        if low[v] == idx[v]:
            comp = []
            while True:
                w = st.pop()
                on.discard(w)
                comp.append(w)
                if w == v:
                    break
            if len(comp) > 1 and not self_only:
                fz = frozenset(comp)
                for w in comp:
                    out[w] = fz
            elif len(comp) == 1 and v in G[v]:
                out[v] = frozenset(comp)
    for v in sorted(G):
        if v not in idx:
            sc(v)
    _s.setrecursionlimit(lim)
    return out


SP8_OFF = [False]  # --no-sp8 (measuring): stage 8 without the argument-register spills' entry values (the varargs functions)


def guard_sym(addr, name):
    return GUARD_SYM.get(addr, f'lift_d1::{name}')


def load_a1_sigs(path, inc):
    """docs/a1_typed.txt -> address -> Sig (the v0 width of a RetVF from typed_a1.inc's return statements)."""
    inv = {n: i for i, n in enumerate(L.GPR)}
    wide_retvf = set()
    cur = None
    for line in open(inc):
        m = re.match(r'^inline a1::RetVF (\w+)\(', line)
        if m:
            cur = m.group(1)
        elif cur and 'a1::RetVF{(uint64_t)' in line and 'a1_sx64(' not in line:
            wide_retvf.add(cur)
    out = {}
    for line in open(path):
        if line.startswith('#'):
            continue
        p = [x.strip() for x in line.split('|')]
        a, name = p[0].split()
        decl = p[1]
        m = re.match(r'^(\S+) (\w+)\((.*)\)$', decl)
        rtype = m.group(1)
        params = []
        for x in m.group(3).split(', ')[2:]:
            ty, n = x.rsplit(' ', 1)
            if ty == 'float':
                params.append(('f', int(n[1:])))
            else:
                params.append(('g', inv[n], ty))
        dropped = set()
        for d in p[2].split(','):
            if not d:
                continue
            if d[0] == 'x':
                dropped.add(('x', d[1:]))
            else:
                dropped.add((d[0], int(d[1:])))
        v0w = 64 if rtype == 'uint64_t' or (rtype == 'a1::RetVF' and name in wide_retvf) else 32
        out[int(a, 16)] = Sig(name, int(a, 16), rtype, params, dropped, v0w, 'a1')
    return out


def word_mem(w, vu=False):
    op = w >> 26
    if op in LOADS:
        return ('ld', LOADS[op])
    if op in STORES:
        return ('st', STORES[op])
    if vu and op in (0x1E, 0x36):  # A4: lq / lqc2 (a 16-byte frame slot)
        return ('ld', 16)
    if vu and op in (0x1F, 0x3E):  # sq / sqc2
        return ('st', 16)
    if op in BADMEM:
        return ('bad', 0)
    return None


FR_RD = re.compile(r'\bREAD(8|16|32|64)_B\((?:\(uint32_t\))?sp, (-?\d+)\)')
FR_WR = re.compile(r'\bWRITE(8|16|32|64)_B\((?:\(uint32_t\))?sp, (-?\d+), ')
FR_RD0 = re.compile(r'\bREAD(8|16|32|64)\((?:\(uint32_t\))?sp\)')
FR_WR0 = re.compile(r'\bWRITE(8|16|32|64)\((?:\(uint32_t\))?sp, ')
FR_RDM = re.compile(r'\bREAD(8|16|32|64)\(sp - (0x[0-9A-Fa-f]+u|\d+u)\)')
FR_WRM = re.compile(r'\bWRITE(8|16|32|64)\(sp - (0x[0-9A-Fa-f]+u|\d+u), ')
VEC_W = re.compile(r'do \{ const __m128i _v = \((.*?)\); ctx->r\[(\d+)\] = _v; std::memcpy\(&(\w+), &_v, 8\); \} while \(0\)')
VEC_N = re.compile(r'do \{ const __m128i _v = \((.*?)\); ctx->r\[(\d+)\] = _v; (\w+) = \(uint32_t\)_mm_cvtsi128_si32\(_v\); \} while \(0\)')


def vec_low(e):
    """The low 64 bits of a movz / movn's 128-bit source expression (the upper half stays in the context only)."""
    if e == '_mm_setzero_si128()':
        return '(uint64_t)0'
    m = re.fullmatch(r'Ps2GprVecWithLow\(ctx->r\[\d+\], (.*)\)', e)
    if m:
        return '(uint64_t)(' + m.group(1) + ')'
    raise Refuse('vu0 / 128-bit')


SP_STEP = re.compile(r'^sp = sp [+-] (?:0x[0-9A-Fa-f]+u|\d+u);$')

# ----------------------------------------------------------------------------------------------- A11: MMI and hi / lo
# The IR's facts (typed_lift.nongpr_word_uses) count every MMI0-MMI3 instruction as a reader of hi and lo ("unknown MMI
# encodings: more uses, safe"). Most read neither: MMI0 / MMI1 (fn 0x08 / 0x28: padd / psub / pext / ppac / pcgt ...) none at
# all; in MMI2 (fn 0x09) only pmaddw / pmsubw / pmfhi / pmflo / pmaddh / phmadh / pmsubh / phmsbh read them, in MMI3 (fn
# 0x29) pmadduw. Stage 11 reads the field inputs with those exact sets (a function whose only hi / lo "reads" are pextlw /
# pcpyld ... was refused as "field input hi,lo": manager_sub_010B8AB8, 010C38E8, the matrix-stack walkers' transposes).
_MMI2_HILO = {0x00, 0x04, 0x08, 0x09, 0x10, 0x11, 0x14, 0x15}
_MMI3_HILO = {0x00}


def mmi_hilo_free(w):
    op, fn, sa = w >> 26, w & 63, (w >> 6) & 31
    if op != 0x1C:
        return False
    if fn in (0x08, 0x28):
        return True
    if fn == 0x09:
        return sa not in _MMI2_HILO
    if fn == 0x29:
        return sa not in _MMI3_HILO
    return False


def mmi_exact(ffacts, words):
    """A11: the facts with hi / lo taken out of the non-GPR uses of the MMI instructions that read neither."""
    HL = {('x', 'hi'), ('x', 'lo')}
    nodes = {}
    for x, n in ffacts['nodes'].items():
        kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq = n
        if mmi_hilo_free(words.get(x, 0)) and HL & set(ou):
            ou = type(ou)(s for s in ou if s not in HL)
        if mmi_hilo_free(words.get(x + 4, 0)) and HL & set(dou):
            dou = type(dou)(s for s in dou if s not in HL)
        nodes[x] = (kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq)
    f = dict(ffacts)
    f['nodes'] = nodes
    return f


# ----------------------------------------------------------------------------------------------- A4: VU0 / 128-bit
# The VU0 state a function may carry across calls (the macro-mode registers are global: the game keeps the current
# matrix in vf1-vf4, iosPushVu0RegistersToWork saves all 32 ...) and the GPRs' upper 64 bits: slot -> (local, C type,
# context lvalue). ('h', r): the upper half of GPR r (lq / qmfc2 / pcpyld ... write it; sq reads it).
VU_FIELDS = {'vu0_acc': ('vacc', 'a4::vec4'), 'vu0_q': ('vq', 'float'), 'vu0_status': ('vstat', 'uint16_t'),
             # A11: VU0's vi15, the matrix stack's pointer into the VU0 data memory (iosPushCurrentMatrix / iosPopCurrentMatrix,
             # A11_EXT): only typed calls of those two name it, so stages 2-10 never see the slot
             'vi15': ('vi15', 'uint16_t'),
             # A14 (RT14): runtime state as VU0-interface slots (only stage 14's lifters name them: RT14_LIFT)
             'lo1': ('lo1', 'uint64_t'), 'hi1': ('hi1', 'uint64_t'), 'sa': ('rsa', 'uint32_t'), 'cop0_status': ('c0st', 'uint32_t'),
             'vu0_clip_flags': ('vclip', 'uint32_t'), 'vu0_r': ('vrr', 'a4::vec4')}


def vu_local(s):
    k, r = s
    if k == 'v':
        return f'vf{r}', 'a4::vec4', f'ctx->vu0_vf[{r}]'
    if k == 'h':
        return f'h{r}', 'uint64_t', None
    loc, ty = VU_FIELDS[r]
    if r == 'vi15':
        return loc, ty, 'ctx->vi[15]'
    return loc, ty, f'ctx->{r}'


def vu_sortkey(s):
    k, r = s
    return ({'v': 0, 'x': 1, 'h': 2}[k], r if isinstance(r, int) else 0, str(r))


# the helpers typed_a4_rt.h defines (anything else a VU0 text names refuses the function: an unknown call)
A4_KNOWN = {'A4_R128', 'A4_W128', 'a4::mk128', 'a4::hi64', 'a4::lo64'} | {'a4v::' + n for n in (
    'castsi128_ps', 'castps_si128', 'mul', 'add', 'sub', 'blend', 'blendv_ps', 'rsqrt', 'sqrt', 'div', 'set_epi32', 'set1_ps',
    'cvtss_f32', 'setzero_si128', 'mul_ps', 'max_ps', 'min_ps', 'cvtepi32_ps', 'cvttps_epi32', 'xor_si128', 'andnot_si128',
    'cmpnlt_ps', 'unpackhi_epi64', 'cvtsi128_si32', 'cvtsi128_si64', 'set1_epi32')} | {'a4m::' + n for n in (
    'pextlw', 'pextuw', 'pcpyld', 'pceqw', 'por', 'pxor', 'pand', 'paddw', 'psubw', 'pextlh', 'pextlb')}
# A7: more lane helpers (typed_a7_rt.h's a4v additions), allowed in stage 7 only (stages 4-6 stay as they were)
A7_KNOWN = {'a4v::' + n for n in ('and_si128', 'cmpeq_epi32', 'or_si128', 'and_ps', 'andnot_ps', 'or_ps', 'xor_ps', 'add_epi32', 'sub_epi32',
                                  'cmpgt_epi32', 'cmplt_epi32', 'max_epi32', 'min_epi32')}
VU_NAME = re.compile(r'\b(vf(\d+)|vacc|vq|vstat|vi15|lo1|hi1|rsa|c0st|vclip|vrr|h(\d+))\b')


def vu_slot_of(m):
    if m.group(2) is not None:
        return ('v', int(m.group(2)))
    if m.group(3) is not None:
        return ('h', int(m.group(3)))
    return ('x', {'vacc': 'vu0_acc', 'vq': 'vu0_q', 'vstat': 'vu0_status', 'vi15': 'vi15', 'lo1': 'lo1', 'hi1': 'hi1', 'rsa': 'sa',
                  'c0st': 'cop0_status', 'vclip': 'vu0_clip_flags', 'vrr': 'vu0_r'}[m.group(1)])


def vu_rw(c):
    """VU0 / upper-half slots a statement reads and writes (from its text; a partial write such as a dest-field blend
    names the register on the right too, so it counts as read)."""
    names = {m.group(1): vu_slot_of(m) for m in VU_NAME.finditer(c)}
    r, w = set(), set()
    for nm, s in names.items():
        # written: an assignment, a memcpy into it, or the status passed by reference to Q's divide / square root
        if re.search(r'\b' + nm + r'\s*(=(?!=)|\|=|&=)', c) or re.search(r'memcpy\(&' + nm + r'\b', c) or \
                (nm == 'vstat' and re.search(r'\bps2Vu(?:Divide|Sqrt|Rsqrt)\(\s*vstat\s*,', c)):
            w.add(s)
        elif nm == 'vstat' and re.search(r'\(\s*vstat\s*,', c):
            raise Refuse('vu0 status form')
        # its address only as a memcpy destination (a write) or source (a read: `memcpy(&bits, &vq, 4)`)
        if re.search(r'&' + nm + r'\b', re.sub(r'memcpy\(&\w+\s*,\s*&\w+', 'memcpy(', re.sub(r'memcpy\(&' + nm + r'\b', 'memcpy(', c))):
            raise Refuse('vu0 address taken')
        rest = re.sub(r'\b' + nm + r'\s*=(?!=)', '', c)
        if re.search(r'\b' + nm + r'\b', rest):
            r.add(s)
    return r, w


VEC_SET = re.compile(r'do \{ const __m128i _v = \((.*?)\); ctx->r\[(\d+)\] = _v; (?:std::memcpy\(&(\w+), &_v, 8\);|(\w+) = \(uint32_t\)_mm_cvtsi128_si32\(_v\);) \} while \(0\)')
FR128_RD = re.compile(r'\bREAD128\(ADD32\((?:\(uint32_t\))?sp, (-?\d+)\)\)')
FR128_WR = re.compile(r'\bWRITE128\(ADD32\((?:\(uint32_t\))?sp, (-?\d+)\), ')


def mm_shuffle(a):
    m = re.fullmatch(r'_MM_SHUFFLE\((\d),\s*(\d),\s*(\d),\s*(\d)\)', a.strip())
    if not m:
        raise Refuse('vu0 shuffle form')
    return (int(m.group(1)) << 6) | (int(m.group(2)) << 4) | (int(m.group(3)) << 2) | int(m.group(4))


def vu_text(text):
    """A4: lift_d1's VU0 / 128-bit forms over typed locals as named helpers (typed_a4_rt.h): a4v:: the VU0 macro
    operations with the PS2's own float rules (the runtime's functions lift_d1's macros call), the lane / field helpers;
    a4m:: the MMI operations in plain C; A4_R128 / A4_W128 the quadword loads / stores (N1's base test, the slow path)."""
    def sub(t, name, fn):
        while True:  # nested calls of the same name: the outer one first, then again for the inner ones
            t2 = T1.msub(t, name, fn)
            if t2 == t:
                return t
            t = t2
    # the dest-field write: _mm_blendv_ps(old, new, _mm_castsi128_ps(_mm_set_epi32(w, z, y, x))), constant lanes
    def dest(a):
        m = re.fullmatch(r'_mm_castsi128_ps\(_mm_set_epi32\((-1|0), (-1|0), (-1|0), (-1|0)\)\)', a[2].strip())
        if not m:
            return f'a4v::blendv_ps({a[0]}, {a[1]}, {a[2]})'
        w, z, y, x = (1 if v == '-1' else 0 for v in m.groups())
        return f'a4v::dest<{x}, {y}, {z}, {w}>({a[0]}, {a[1]})'
    text = sub(text, '_mm_blendv_ps', dest)
    # the broadcast of one field: _mm_shuffle_ps(v, v, _MM_SHUFFLE(i, i, i, i))
    def shuf(a):
        imm = mm_shuffle(a[2])
        if a[0].strip() == a[1].strip() and imm in (0x00, 0x55, 0xAA, 0xFF):
            return f'a4v::bcast<{imm & 3}>({a[0]})'
        return f'a4v::shuffle_ps<0x{imm:02X}>({a[0]}, {a[1]})'
    text = sub(text, '_mm_shuffle_ps', shuf)
    for nm in ('_mm_srai_epi32', '_mm_srli_epi32', '_mm_slli_epi32', '_mm_srli_si128', '_mm_slli_si128'):
        text = sub(text, nm, lambda a, nm=nm: f'a4v::{nm[4:]}<{a[1].strip()}>({a[0]})')
    for k, h in (('PS2_VMUL_K', 'mulK'), ('PS2_VADD_K', 'addK'), ('PS2_VSUB_K', 'subK')):
        text = sub(text, k, lambda a, h=h: f'a4v::{h}<{a[0].strip()}, {a[1].strip()}>({a[2]}, {a[3]})')
    for k, h in (('PS2_VMUL', 'mul'), ('PS2_VADD', 'add'), ('PS2_VSUB', 'sub'), ('PS2_VBLEND', 'blend'),
                 ('ps2VuRsqrt', 'rsqrt'), ('ps2VuSqrt', 'sqrt'), ('ps2VuDivide', 'div')):
        text = re.sub(r'(?<![\w:])' + k + r'\(', f'a4v::{h}(', text)
    for k in ('PEXTLW', 'PEXTUW', 'PCPYLD', 'PCEQW', 'POR', 'PXOR', 'PAND', 'PADDW', 'PSUBW', 'PEXTLH', 'PEXTLB'):
        text = re.sub(r'(?<![\w:])PS2_' + k + r'\(', f'a4m::{k.lower()}(', text)
    text = re.sub(r'(?<![\w:])PS2_QFSRV\(', 'a4m::qfsrv(', text)  # A14
    text = re.sub(r'(?<![\w:])_mm_(\w+)\(', r'a4v::\1(', text)
    text = re.sub(r'(?<![\w:])READ128\(', 'A4_R128(', text)
    text = re.sub(r'(?<![\w:])WRITE128\(', 'A4_W128(', text)
    text = re.sub(r'\b__m128i\b', 'a4::q128', text)
    text = re.sub(r'\b__m128\b', 'a4::vec4', text)
    return text


class Emit:
    """One function written as typed C++ from lift_d1's IR."""

    def __init__(self, addr, name, abi, sigs, fin, ffacts, loops=False, xout=(), vu=False, a5=False, a6=False, a7=False, a8=False, a9=False, a10=False, a11=False, a12=False, a13=False, a14=False):
        self.addr, self.name = addr, name
        self.a14 = a14  # A14: stage 14 (stage 13's form; the runtime state as VU0-interface slots: RT14; a likely break slot: BRK14)
        a13 = a13 or a14
        self.a13 = a13  # A13: stage 13 (stage 12's form; hi / lo as extra outputs: HL13)
        a12 = a12 or a13
        self.a12 = a12  # A12: stage 12 (stage 11's form; the spin rule's base taint flow-sensitive: SPIN12)
        a11 = a11 or a12
        # A10: stage 10 (stage 9's form, recursion; the adapters test the switch a10typed: A10_GO / A10_HOLDS)
        self.a11 = a11  # A11: stage 11 (stage 10's form; the adapters test the switch a11typed: A11_GO / A11_HOLDS)
        a10 = a10 or a11
        self.a10 = a10
        a9 = a9 or a10
        # A9: stage 9 (stage 8's form; the adapters test the switch a9typed: A9_GO / A9_HOLDS)
        self.a9 = a9
        a8 = a8 or a9
        # A8: stage 8 (stage 7's form; a kept spill of an argument register in a guest frame stores the register's entry
        # value: the varargs functions LoaderSysPrintf / iosSPrintf ...)
        self.a8 = a8
        a7 = a7 or a8
        # A7: a typed callee's extra outputs consumed at the call (the caller reads the scratch register after it): the call
        # passes `a7c` (the slots' values before the call), the callee writes the ones it changes, the caller takes them back
        self.a7 = a7
        self.need = None
        self.extra_in, self.extra_fin, self.force_wide = set(), set(), set()
        self.extra_xin = set()  # A13 (XIN13): hi / lo as inputs
        self.cons = {}
        self.stk = False
        # A6: calls to typed callees that are not lifted functions (a recompiled-only function, a hand native typed from
        # its MIPS, a typed runtime stub: their slot tested against what holds it), `break` (A6_BREAK: its cop0 effects
        # applied by the adapter after the call, A6_BRKS)
        self.a6 = a6
        self.brks = set()
        self.ok, self.why = False, ''
        # A5: the typed ABI proof (callees' own typed interfaces), relocated calls with a slot test, the guest frame
        # (a frame whose address escapes lives in guest RAM below the guest sp), 64-bit input pass-through, jump tables
        self.a5 = a5
        self.gf = False  # A5: the frame in guest RAM (sp a local guest address)
        self.needs_sp = False  # A5: sp is a parameter (the guest frame here or in a callee)
        self.relocs = set()  # A5: (site, static target) of relocated jal / j: the adapter tests the word in RAM
        self.wide_in = set()  # A5: 32-bit inputs passed whole (64-bit) to a callee: a uint64_t parameter w64_<r>
        self.jtabs = {}  # A5: jump tables: node -> case targets
        self.spcall = {}  # A5: entry-relative sp offset at each call (after its delay slot)
        self.loops = loops  # A3: back edges allowed (A3_BACK: the clock charged, the checkpoint deferred)
        self.vu = vu  # A4: VU0 registers, ACC, Q, status, GPR upper halves and quadword accesses allowed (the VU0 ABI)
        self.vio, self.vin, self.vout, self.vmust = (), frozenset(), frozenset(), frozenset()
        self.has128 = False
        self.backs = set()
        # A3: extra outputs, scratch registers a lifted caller reads after the call (the caller check): the adapter
        # writes them back from `xo` (a typed caller passes no xo and never reads them)
        self.xout = tuple(sorted(xout, key=lambda q: (q[0], q[1])))
        rt14 = a14 and not RT14_OFF[0]
        if rt14:
            saved14 = dict(L.FIELDS)
            L.FIELDS.update(RT14_LIFT)
        try:
            self.run6(abi, sigs, fin, ffacts)
        finally:
            if rt14:
                L.FIELDS.clear()
                L.FIELDS.update(saved14)

    def run6(self, abi, sigs, fin, ffacts):
        for _ in range(6):
            try:
                self.run(abi, sigs, fin, ffacts)
                self.ok = True
            except A7Restart:
                self.brks, self.gf, self.needs_sp, self.relocs, self.wide_in, self.jtabs, self.spcall = set(), False, False, set(), set(), {}, {}
                self.vio, self.vin, self.vout, self.vmust = (), frozenset(), frozenset(), frozenset()
                self.has128, self.backs, self.cons, self.stk = False, set(), {}, False
                continue
            except NeedXout as e:
                self.why, self.need = str(e), e.need
            except Refuse as e:
                self.why = str(e)
            except L.Refuse as e:
                self.why = 'lifter: ' + str(e)[:50]
            break
        else:
            self.why = 'a7: restart limit'

    def run(self, abi, sigs, fin, ffacts):
        if not self.a5 and (abi is None or not abi[0]):
            raise Refuse('not proven')
        lf = L.Lifter(self.addr, {})
        lf.build()
        if self.a7 and self.force_wide and not lf.mode64:
            lf.wide = set(lf.wide) | self.force_wide  # A7: a consumed extra output carries the callee's whole 64-bit value
        nodes = lf.node_effects()
        lf.nodes = nodes
        self.lf = lf
        if self.a6 and not NO_BRK6[0]:
            # A6: `break` (dynamic_cast's failed type check ...): the runtime's handleBreak changes only the cop0 state (EPC,
            # Cause, Status.EXL) and pc, never a GPR / FPR; the typed function records the site (A6_BREAK) and the adapter
            # runs handleBreak on the context after the call (A6_BRKS: the first and the last site, which leave the same
            # state as every one in order). No typed code reads cop0 (mfc0 refuses), so the context ends the same.
            W0 = lf.g.words
            for x in lf.order:
                n = nodes[x]
                if lf.kind[x][0] == 'op' and n['opaque'] and (W0[x] & 0xFC00003F) == 0x0000000D:
                    n['opaque'] = False
                    n['code'] = f'A6_BREAK(0x{x:X}u);'
                    n['uses'], n['defs'] = set(), set()
                    self.brks.add(x)
        self.brk14 = set()
        if getattr(self, 'a14', False) and not BRK14_OFF[0] and lf.brk:
            # A14 (BRK14): a likely branch's `break` slot (`beql $x, $zero` / `break 7`: the divide-by-zero check) runs only when the
            # branch is taken; lift_d1's text there sets pc to the slot, in_delay_slot and branch_pc to the branch, calls handleBreak
            # (cop0 EPC = the branch, Cause.BD, Status.EXL; no GPR / FPR) and goes on to the target. The typed function records the
            # site with the slot flag (A6_BREAK(slot | 1)) in the taken path; the adapter applies it after the call (A6_BRKS: a14's
            # applyBrks takes the flag as the delay slot and leaves branch_pc as the last slot break did).
            for b in sorted(lf.brk):
                x = b - 4
                k = lf.kind.get(x)
                if not k or k[0] != 'br' or not k[5] or nodes[x].get('dcode'):
                    raise Refuse('break slot form')
                nodes[x]['dcode'] = f'A6_BREAK(0x{b | 1:X}u);'
                self.brk14.add(b)
                self.brks.add(b | 1)
        if self.a5:
            abi = self.typed_abi(lf, nodes, sigs)
            self.abi5 = abi
        W = lf.g.words
        reach, work = {lf.order[0]}, [lf.order[0]]
        while work:
            x = work.pop()
            for y in lf.succ[x]:
                if y not in reach:
                    reach.add(y)
                    work.append(y)
        order = [x for x in lf.order if x in reach]
        self.order = order
        if lf.brk and set(lf.brk) != self.brk14:
            raise Refuse('break slot')
        callees = []
        for x in order:
            k = lf.kind[x]
            if self.a5 and k[0] == 'jr' and k[2]:
                # A5: a jump table (the recompiler's case list): a switch over the same targets
                self.jtabs[x] = list(k[2])
            elif k[0] in ('jalr', 'jr', 'ijal', 'iret'):
                raise Refuse('indirect call / jump')
            if k[0] == 'op' and nodes[x]['opaque']:
                raise Refuse('runtime call')
            if k[0] in ('jal', 'tail'):
                dyn = x < L.EXIT_BASE and ('READ32(0x%Xu)' % x).lower() in lf.g.block_text(x).split('// 0x')[0].lower()
                if x in lf.dyncall or dyn:
                    if not self.a5:
                        if x in lf.dyncall:
                            raise Refuse('relocated call')
                    else:
                        # A5: the relocated jal / j's word as the build links it gives the static target; the adapter tests
                        # that the word in RAM still encodes it (A5_JAL), so the typed call is the one the lifted text makes
                        self.relocs.add((x, k[1]))
                if k[1] not in sigs:
                    raise Refuse('untyped callee')
                callees.append(k[1])
            for y in lf.succ[x]:
                if y <= x:
                    if not self.loops:
                        raise Refuse('loop')
                    self.backs.add((x, y))
        if any(s[0] == 'v' for s in lf.allslots) and not self.vu:
            raise Refuse('vu0 / 128-bit')
        if self.vu and any(s[0] == 'x' and s[1] not in VU_FIELDS and s[1] not in ('fcr31', 'f_acc', 'hi', 'lo') for s in lf.allslots):
            raise Refuse('vu0 field ' + ','.join(sorted(str(s[1]) for s in lf.allslots if s[0] == 'x' and s[1] not in VU_FIELDS)))
        if self.backs:
            self.spin_filter(order)
        self.callees = callees
        # ---------------------------------------------------------------- the frame
        spoff = {order[0]: 0}
        sp_changed = True
        while sp_changed:  # A3: to a fixpoint (a back edge's target may come before some of its predecessors)
          sp_changed = False
          for x in order:
            if x not in spoff:
                continue
            o = spoff[x]
            omid = o
            for y in ((x, x + 4) if x + 4 in lf.delay else (x,)):
                w = W[y]
                if (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29:
                    o += L.sx16(w & 0xFFFF)
                elif 29 in lf.gpr_defs_raw(lf.raw.get(y, '')):
                    raise Refuse('sp write')
                if y == x:
                    omid = o
            k = lf.kind[x]
            self.spcall[x] = o
            for s in lf.succ[x]:
                e = omid if (k[0] == 'br' and k[5] and s != k[4]) else o
                if s in spoff and spoff[s] != e:
                    raise Refuse('sp differs between paths')
                if s not in spoff:
                    sp_changed = True
                spoff[s] = e
            if k[0] in ('ret', 'tail') and o != 0:
                raise Refuse('sp not back at an exit')
        self.spoff = spoff
        def host_frame():
            acc = {}  # instruction address -> (kind, entry-relative offset, size)
            for x in order:
                for y in ((x, x + 4) if x + 4 in lf.delay else (x,)):
                    w = W[y]
                    if w == 0:
                        continue
                    uses = A.word_uses(w)
                    if 29 not in uses:
                        continue
                    op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                    if op == 9 and rs == 29 and rt == 29:
                        continue
                    mk = word_mem(w, self.vu)
                    if mk is None or rs != 29 or (mk[0] == 'st' and rt == 29 and op not in (0x3E,)):
                        raise Refuse('frame address escapes')
                    if mk[0] == 'bad':
                        raise Refuse('frame access form')
                    off = spoff[x] + L.sx16(w & 0xFFFF)
                    if off + mk[1] > 0:
                        raise Refuse("caller's frame")
                    if mk[1] == 16:
                        # A4: a quadword frame slot; lq / sq clear the address's low 4 bits, so with sp 16-aligned (the EE ABI; the
                        # adapter tests it) the slot is at this offset only when the offset is a multiple of 16
                        if off % 16:
                            raise Refuse('frame quadword alignment')
                        self.has128 = True
                    acc[y] = (mk[0], off, mk[1])
            saves = lf.find_saves() if acc else {}
            save_at = {sx: (r, off, size) for r, (off, size, sx) in saves.items()}
            restores = dict(lf.restores)
            self.frame = -min([o for _, o, _ in acc.values()] + [0])
            # the save slots: read only by their restores
            for y, (kd, off, size) in acc.items():
                if kd == 'ld' and y not in restores:
                    for sy, (r, so, ss) in save_at.items():
                        if off < so + ss and so < off + size:
                            raise Refuse('save slot read')
                if y in restores and y not in acc:
                    raise Refuse('restore form')
            # frame bytes written on every path before each load (saves dropped: not written). A must-analysis to a
            # fixpoint (A3: loops; the sets only shrink), then one pass that checks each load against its final set
            def frame_pass(wr, check):
                changed = False
                for x in order:
                    if x not in wr:
                        continue
                    cur = set(wr[x])

                    def visit(y, cur):
                        if y not in acc:
                            return
                        kd, off, size = acc[y]
                        if kd == 'ld':
                            if y in restores:
                                return
                            if check and not set(range(off, off + size)) <= cur:
                                raise Refuse('frame read before write')
                        elif y not in save_at:
                            cur |= set(range(off, off + size))
                    k = lf.kind[x]
                    visit(x, cur)
                    mid = frozenset(cur)
                    if x + 4 in lf.delay:
                        visit(x + 4, cur)
                    after = frozenset(cur)
                    for s in lf.succ[x]:
                        e = mid if (k[0] == 'br' and k[5] and s != k[4]) else after
                        ne = e if s not in wr else (wr[s] & e)
                        if wr.get(s) != ne:
                            wr[s] = ne
                            changed = True
                return changed
            wr = {order[0]: frozenset()}
            while frame_pass(wr, False):
                pass
            frame_pass(wr, True)
            return acc, save_at, restores
        # A7: the call sites of callees that read or write their caller's frame (printf's varargs past the registers from sp + 0x10
        # on; a guest-frame function reading its stack arguments above its entry sp), by the sp offset there: at an offset other
        # than 0 the accesses land in this function's frame, which is then kept in guest RAM (a host frame's slots are not there)
        stk_sites = [x for x in order if self.a7 and lf.kind[x][0] in ('jal', 'tail') and getattr(sigs[lf.kind[x][1]], 'stk', False)]
        if any(self.spcall.get(x, 0) != 0 for x in stk_sites):
            # A7: a callee reads varargs from the guest stack above its sp (printf's cursor): this function's frame (its outgoing
            # argument slots) is kept in guest RAM at the lifted text's addresses (A5's guest frame)
            acc, save_at, restores = self.guest_frame(order, W)
        else:
            try:
                acc, save_at, restores = host_frame()
            except Refuse as e:
                if not self.a5 or str(e) not in GF_REASONS:
                    raise
                acc, save_at, restores = self.guest_frame(order, W)
        # A7: such a callee called at the entry sp (a tail call, or no frame), or a guest frame reaching above its entry sp: this
        # function touches its caller's frame, which its typed callers then keep in guest RAM too (the property passes up)
        self.stk = any(self.spcall.get(x, 0) == 0 for x in stk_sites) or (self.a7 and self.gf and getattr(self, 'gf_cfr', False))
        self.acc, self.save_at, self.restores = acc, save_at, restores
        # ---------------------------------------------------------------- statements
        def statements():
            code = {}
            for x in order:
                n = nodes[x]
                if lf.kind[x][0] == 'op':
                    code[x] = self.stmt(x, n['code'])
                else:
                    code[x + 4] = self.stmt(x + 4, n['dcode'])
            return code
        try:
            code = statements()
        except Refuse as e:
            # A5: a 64-bit sp (its step sign-extends) has no host-frame form: the guest frame keeps the lifted step
            if not self.a5 or self.gf or str(e) != 'sp step form':
                raise
            self.acc, self.save_at, self.restores = self.guest_frame(order, W)
            code = statements()
        self.code = code
        # restored registers must be dead after their restores (the typed function never had their caller's values)
        self.check_restored_dead(ffacts, sigs)
        if self.vu:
            self.vu_scan(sigs)
            if getattr(self, 'a14', False) and ('x', 'cop0_status') in (self.vin | self.vout) and \
                    (self.brks or any(getattr(sigs[c], 'brk', False) for c in callees)):
                # A14 (RT14): the adapter applies a break's cop0 effects (Status.EXL) after the call: a typed call tree that also
                # reads or writes Status would see / leave it in another order than the lifted text
                raise Refuse('break and COP0 Status')
        # ---------------------------------------------------------------- parameters, locals
        gin = set(abi[1]) - {0, 29, 31}
        if self.a5 and any(p[0] == 'g' and p[1] == 29 for c in callees for p in sigs[c].params):
            self.needs_sp = True  # A5: a callee keeps its frame in guest RAM: it gets the guest sp at the call
        if self.needs_sp:
            gin.add(29)
        fx = T1.fpr_x_inputs(mmi_exact(ffacts, self.lf.g.words) if self.a11 and not MMI11_OFF[0] else ffacts, fin)
        if self.vu:  # A4: the VU0 state is the VU0 interface's (vu_scan), not a plain input
            fx = {s for s in fx if s[0] != 'v' and not (s[0] == 'x' and s[1] in VU_FIELDS)}
        if self.a13 and not HL13_OFF[0]:  # A13 (XIN13): hi / lo read before written: inputs
            self.extra_xin |= {r for k, r in fx if (k, r) in HL13}
            fx = {q for q in fx if q not in HL13}
        if any(k == 'x' for k, r in fx):
            raise Refuse('field input ' + ','.join(sorted(r for k, r in fx if k == 'x')))
        if any(k == 'v' for k, r in fx):
            raise Refuse('vu0 / 128-bit')
        fins = {r for k, r in fx if k == 'f'} | (self.extra_fin if self.a7 else set())
        slots = {s for s in lf.allslots if s != ('g', 0)}
        for x in order:
            k = lf.kind[x]
            if k[0] in ('jal', 'tail'):
                for p in sigs[k[1]].params:
                    slots.add((p[0], p[1]))
                if k[0] == 'jal':
                    slots |= RET_SLOTS[sigs[k[1]].rtype]
        slots |= {('g', r) for r in gin} | {('f', r) for r in fins}
        slots |= {('x', r) for r in self.extra_xin}  # A13 (XIN13)
        slots -= {('g', 29), ('g', 31)}
        if self.needs_sp:
            slots.add(('g', 29))
        self.gty = {}
        for k, r in slots:
            if k == 'g':
                self.gty[r] = 'uint64_t' if lf.is_wide(r) else 'uint32_t'
        self.slots = slots
        self.gin, self.fins = gin, fins
        # ---------------------------------------------------------------- must-written (v0 / f0 at the exits, 64-bit args)
        self.mw_in = self.must_written(sigs)
        if self.a5:
            # A5: a 32-bit input passed to a callee's 64-bit parameter while still the input on every path (never written
            # before the call): the lifted caller leaves it in the context untouched, so the callee reads the whole 64-bit
            # value; the typed function takes that value too (w64_<r>, the 32-bit local its low half)
            may = self.may_written(sigs)
            pt12 = set()
            for x in order:
                k = lf.kind[x]
                if k[0] not in ('jal', 'tail'):
                    continue
                dd = set(lf.nodes[x].get('ddefs', set()))
                before = self.mw_in.get(x, frozenset()) | dd
                mbefore = may.get(x, frozenset()) | dd
                for p in sigs[k[1]].params:
                    if p[0] != 'g' or p[2] != 'uint64_t' or p[1] == 29 or self.gty[p[1]] == 'uint64_t' or ('g', p[1]) in before:
                        continue
                    if p[1] in gin and ('g', p[1]) not in mbefore:
                        self.wide_in.add(p[1])
                    elif self.a12 and not PT12_OFF[0]:
                        # A12 (PT12): a register an earlier callee may have written (its scratch, e.g. t0 / t1 after a call
                        # into the printf core) passed to a 64-bit parameter: left to a7_consume, which takes it back as
                        # that callee's extra output, a wide local holding the whole 64-bit value (an input where no callee
                        # wrote it); refused after a7_consume if it is still a narrow local
                        pt12.add(p[1])
                    else:
                        raise Refuse('64-bit pass-through argument')
        # ---------------------------------------------------------------- return value
        prov = []
        for x in order:
            k = lf.kind[x]
            if k[0] == 'ret':
                w = self.mw_after(x)
                prov.append(({('g', 2), ('f', 0)} & w))
            elif k[0] == 'tail':
                prov.append(set(RET_SLOTS[sigs[k[1]].rtype]))
        if not prov:
            raise Refuse('no return')
        ret = set.intersection(*prov)
        v0w = 32
        if ('g', 2) in ret:
            if self.gty.get(2) == 'uint64_t':
                v0w = 64
            for x in order:
                k = lf.kind[x]
                if k[0] == 'tail' and sigs[k[1]].v0w == 64:
                    v0w = 64
        if ret == {('g', 2), ('f', 0)}:
            self.rtype = 'a1::RetVF'
        elif ret == {('g', 2)}:
            self.rtype = 'uint64_t' if v0w == 64 else 'uint32_t'
        elif ret == {('f', 0)}:
            self.rtype = 'float'
        else:
            self.rtype = 'void'
        self.v0w = v0w
        self.ret = ret
        if self.a7:
            self.a7_consume(sigs, ffacts)
        if self.a5 and pt12 - set(lf.wide):
            raise Refuse('64-bit pass-through argument')
        # ---------------------------------------------------------------- scratch slots (what the lifted text may change)
        wrote = set()
        for x in order:
            n = nodes[x]
            wrote |= set(n['defs']) | set(n.get('ddefs', set()))
            k = lf.kind[x]
            if k[0] in ('jal', 'tail'):
                s = sigs[k[1]]
                wrote |= s.dropped | RET_SLOTS[s.rtype]
        wrote = {s for s in wrote if s[0] != 'v' and not (s[0] == 'g' and (s[1] == 0 or s[1] in SAVED))}
        if self.vu:  # A4: the VU0 state is never dropped (every slot written is an output of the VU0 interface)
            wrote = {s for s in wrote if not (s[0] == 'x' and s[1] in VU_FIELDS)}
        self.dropped = wrote - ret
        if self.xout:
            self.check_xout(sigs)
        # ---------------------------------------------------------------- the text
        self.text = self.body(sigs)
        if self.addr in NOTE10 and self.rtype == 'uint32_t' and not self.xout:
            # A10: the camera note (NOTE10): every return's value through the note macro
            t0 = self.text
            self.text = re.sub(r'\breturn ([^;]+);', lambda m: f'return {NOTE10[self.addr]}({m.group(1)});', self.text)
            assert self.text != t0

    def guest_frame(self, order, W):
        """A5: the frame in guest RAM. When the frame's address escapes (a pointer to a vector or a struct on the stack passed
        to a callee or kept in a register), it is read before a write, it reaches into the caller's frame or has an lwl /
        lwr form, the function keeps `sp` as a local guest address (a parameter: the guest sp at entry, as the adapter
        reads it from the context or the typed caller passes its own), moves it by the same steps and reads / writes the
        frame in guest RAM at the same addresses as the lifted text. Only the prologue's saves of callee-saved registers
        and ra go (and their restores, as for a host frame): their values are the caller's, which a typed function keeps
        in its own registers; a load from a save slot other than its restore refuses the function."""
        lf = self.lf
        self.gf, self.needs_sp, self.has128, self.frame = True, True, False, 0
        acc = {}
        for x in order:
            for y in ((x, x + 4) if x + 4 in lf.delay else (x,)):
                w = W[y]
                if w == 0 or 29 not in A.word_uses(w):
                    continue
                op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                if op == 9 and rs == 29 and rt == 29:
                    continue
                mk = word_mem(w, self.vu)
                if mk is None or rs != 29 or mk[0] == 'bad' or (mk[0] == 'st' and rt == 29 and op not in (0x3E,)):
                    continue
                acc[y] = (mk[0], self.spoff[x] + L.sx16(w & 0xFFFF), mk[1])
        self.gf_cfr = any(off + size > 0 for kd, off, size in acc.values())  # A7: an access above the entry sp (the caller's frame)
        saves = lf.find_saves()
        restores = dict(lf.restores)
        if self.a7:
            # A7: only the callee-saved registers' saves go (their values are the caller's, which a typed function keeps in its
            # own registers); a spill of any other register (a varargs function's a1..t3 into its va area, read through the
            # escaped frame address) stays a store of the guest frame, and the register an input
            if self.a8 and not SP8_OFF[0]:
                # A8: such a kept spill stores the register's whole 64-bit entry value (lift_d1's text names it from the
                # context, Ps2GprU64(ctx, r): the local here, a wide input, read before anything writes it: a prologue store)
                self.a7_spills = {v[2]: r for r, v in saves.items() if r not in SAVED}
                sp_regs = set(self.a7_spills.values())
                if sp_regs and not lf.mode64 and not sp_regs <= set(lf.wide):
                    self.force_wide |= sp_regs
                    raise A7Restart()
            saves = {r: v for r, v in saves.items() if r in SAVED}
            restores = {y: v for y, v in restores.items() if v[0] in saves}
        save_at = {sx: (r, off, size) for r, (off, size, sx) in saves.items()}
        for y, (kd, off, size) in acc.items():
            if kd == 'ld' and y not in restores:
                for sy, (r, so, ss) in save_at.items():
                    if off < so + ss and so < off + size:
                        raise Refuse('save slot read')
        for y in restores:
            if y not in acc:
                raise Refuse('restore form')
        return {}, save_at, restores

    def typed_abi(self, lf, nodes, sigs):
        """A5: tools/n4_abi.py's proof over this function with its callees' typed interfaces (every callee is typed when a
        function is tried: its inputs are its parameters, its changes its scratch set and return slots). N4's proof takes a
        callee whose slot may hold a native as reading and writing every register; a typed caller calls the typed callee
        directly (the adapter tests every reached slot, A5_HOLDS), so the callee's own interface is the one that holds.
        Inputs need not be within the argument registers: each becomes a parameter (the adapter reads it from the context,
        a typed caller passes its own value, which N4's input analysis makes one of its inputs or a register written on
        every path before the call). Still required: callee-saved registers and sp back at every exit, no runtime call,
        break, jalr or jump through a register without a case list."""
        fa = A.facts_from(self.addr, lf, nodes)[1]
        bad = set(fa['why'])
        if getattr(self, 'brk14', None) and set(lf.brk) == self.brk14:
            bad.discard('break')  # A14 (BRK14): every break slot recorded (A6_BREAK), no other effect on the registers
        if bad:
            raise Refuse('not proven: ' + ','.join(sorted(bad)))
        for c in fa['calls']:
            if c not in sigs:
                raise Refuse('untyped callee')
        inp = {c: {p[1] for p in sigs[c].params if p[0] == 'g'} for c in fa['calls']}
        if getattr(self, 'a14', False) and not CSR14_OFF[0]:
            # A14 (CSR14): the input set below drops the callee-saved registers the prologue saves (the save reads them); a path
            # that reads one before writing it other than by its save reads the caller's value, which a typed function does not
            # have (Sg2SlotFreeSearchTerm's s0 / s1 for an a0 outside 0..2, found by the offline harness: A8's class). The same
            # forward analysis without the saves' own reads: any such register left is refused.
            saves = lf.find_saves()
            W = lf.g.words
            sv = {sx: r for r, (o_, sz_, sx) in saves.items() if r in SAVED}
            cfg2 = {}
            for x, c in fa['cfg'].items():
                rm = set()
                for y in ((x, x + 4) if x + 4 in lf.delay else (x,)):
                    if y in sv:
                        other = [z for z in ((x, x + 4) if x + 4 in lf.delay else (x,)) if z != y]
                        if not any(sv[y] in A.word_uses(W[z]) for z in other):
                            rm.add(sv[y])
                cfg2[x] = (c[0], c[1], frozenset(set(c[2]) - rm)) + tuple(c[3:])
            rb = (A.inputs_of(dict(fa, cfg=cfg2), inp) & SAVED & set(saves)) - {29, 31}
            if rb:
                raise Refuse('callee-saved register read before written')
        ins = A.inputs_of(fa, inp) - (fa['saves'] & SAVED if self.a7 else fa['saves'])
        if self.a7:
            ins = ins | self.extra_in  # A7: a consumed extra output's value before the call (it passes through when unchanged)
        wm = {c: {r for k, r in sigs[c].dropped if k == 'g'} | {r for k, r in RET_SLOTS[sigs[c].rtype] if k == 'g'} for c in fa['calls']}
        ff = {'order': fa['order'], 'restores': fa['restores'],
              'cfg': {x: (c[0], c[1], c[6], c[7], c[5], tuple(sorted(c[3])), tuple(sorted(c[4]))) for x, c in fa['cfg'].items()}}
        outs = L.function_gpr_writes(ff, wm)
        if outs & A.SAVED:
            raise Refuse('not proven: callee-saved register changed')
        if not fa['sp_ok']:
            raise Refuse('not proven: sp')
        return (True, ins, outs)

    def may_written(self, sigs):
        """A5: slots written on some path before each node (a typed callee's return slots and scratch set count)."""
        lf = self.lf
        nodes = lf.nodes
        order = self.order
        mw = {order[0]: frozenset()}
        changed = True
        while changed:
            changed = False
            for x in order:
                if x not in mw:
                    continue
                n = nodes[x]
                k = lf.kind[x]
                cur = set(mw[x])
                if k[0] == 'op':
                    cur |= set(n['defs'])
                    mid = after = frozenset(cur)
                else:
                    mid = frozenset(cur)
                    cur |= set(n.get('ddefs', set()))
                    if k[0] == 'jal':
                        cur |= RET_SLOTS[sigs[k[1]].rtype] | set(sigs[k[1]].dropped)
                    after = frozenset(cur)
                for s_ in lf.succ[x]:
                    e = mid if (k[0] == 'br' and k[5] and s_ != k[4]) else after
                    ne = e if s_ not in mw else (mw[s_] | e)
                    if mw.get(s_) != ne:
                        mw[s_] = ne
                        changed = True
        return mw

    # ------------------------------------------------------------------------------------------------ helpers
    def word_defs(self, w):
        op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
        if op == 0:
            if fn in (8, 0x0C, 0x0D, 0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13):
                return set()
            return {rd}
        if op == 0x1C:  # MMI
            return {rd}
        if op in (2, 4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17, 1) or op in STORES or op in (0x2A, 0x2E, 0x2C, 0x2D, 0x1F, 0x3E, 0x39):
            return set() if op != 3 else {31}
        if op == 3:
            return {31}
        if op == 0x11:
            return {rt} if rs in (0, 2) else set()
        if op in (0x10, 0x12):
            return {rt} if rs in (0, 2) else set()
        return {rt}

    def stmt(self, y, c):
        """One instruction's statement text in the typed form (frame slots, no sp)."""
        lf = self.lf
        w = lf.g.words[y]
        if y in self.restores or y in self.save_at:
            return ''
        if w != 0 and (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29:
            if self.gf and re.fullmatch(r'\(?sp = [^;]*;', c.strip()):
                return c.strip()  # A5: the guest frame keeps the lifted step (sp a local guest address)
            if not SP_STEP.match(c.strip()):
                raise Refuse('sp step form')
            return c.strip() if self.gf else ''
        if self.vu:
            c = self.stmt_vu(y, c)
        c = VEC_W.sub(lambda m: f'{m.group(3)} = {vec_low(m.group(1))};', c)
        c = VEC_N.sub(lambda m: f'{m.group(3)} = (uint32_t)({vec_low(m.group(1))});', c)
        c = n1_basemem.rewrite(c)
        if self.gf:
            if self.a8 and y in getattr(self, 'a7_spills', {}):
                r = self.a7_spills[y]  # A8: a kept spill of an argument register: its entry value is the (wide) local
                c = c.replace(f'Ps2GprU64(ctx, {r})', L.GPR[r])
            return c  # A5: the guest frame: sp a local guest address, the frame's loads / stores guest memory as lifted
        node = y - 4 if y in lf.delay else y
        nfr = [0]

        def off_of(imm):
            o = self.spoff[node] + imm + self.frame
            if y not in self.acc or self.acc[y][1] + self.frame != o:
                raise Refuse('frame form')
            nfr[0] += 1
            return o
        c = FR_RD.sub(lambda m: f'a2::frLd<uint{m.group(1)}_t>(a2fr, {off_of(int(m.group(2)))})', c)
        c = FR_WR.sub(lambda m: f'a2::frSt<uint{m.group(1)}_t>(a2fr, {off_of(int(m.group(2)))}, ', c)
        c = FR_RD0.sub(lambda m: f'a2::frLd<uint{m.group(1)}_t>(a2fr, {off_of(0)})', c)
        c = FR_WR0.sub(lambda m: f'a2::frSt<uint{m.group(1)}_t>(a2fr, {off_of(0)}, ', c)
        c = FR_RDM.sub(lambda m: f'a2::frLd<uint{m.group(1)}_t>(a2fr, {off_of(-int(m.group(2).rstrip("u"), 0))})', c)
        c = FR_WRM.sub(lambda m: f'a2::frSt<uint{m.group(1)}_t>(a2fr, {off_of(-int(m.group(2).rstrip("u"), 0))}, ', c)
        if self.vu:  # A4: quadword frame slots
            c = FR128_RD.sub(lambda m: f'a2::frLd<a4::q128>(a2fr, {off_of(int(m.group(1)))})', c)
            c = FR128_WR.sub(lambda m: f'a2::frSt<a4::q128>(a2fr, {off_of(int(m.group(1)))}, ', c)
        if (y in self.acc) != (nfr[0] == 1) or nfr[0] > 1:
            raise Refuse('frame form')
        if re.search(r'\bsp\b', c):
            raise Refuse('sp use')
        return c

    def vu_out(self):
        """A4: the VU0 outputs' locals back into their reference parameters (at an exit)."""
        return ''.join(f'io_{vu_local(q)[0]} = {vu_local(q)[0]}; ' for q, isout in self.vio if isout)

    def stmt_vu(self, y, c):
        """A4: a GPR's 128-bit value is its local (the low 64 bits) and h<r> (the upper 64 bits, a slot of the VU0
        interface): lift_d1's context forms for the upper half become those locals."""
        lf = self.lf

        def setv(m):
            r = int(m.group(2))
            if m.group(3):
                lo = f'std::memcpy(&{m.group(3)}, &_v, 8);'
            else:
                lo = f'{m.group(4)} = (uint32_t)a4::lo64(_v);'
            return f'do {{ const __m128i _v = ({m.group(1)}); h{r} = a4::hi64(_v); {lo} }} while (0)'
        c = VEC_SET.sub(setv, c)
        c = re.sub(r'Ps2GprVecWithLow\(ctx->r\[(\d+)\], ', lambda m: f'a4::mk128(h{m.group(1)}, ', c)

        def raw(m):
            r = int(m.group(1))
            if r == 0:
                return '_mm_setzero_si128()'
            if not lf.is_wide(r):
                raise Refuse('128-bit read of a narrow register')
            return f'a4::mk128(h{r}, {L.GPR[r]})'
        c = re.sub(r'ctx->r\[(\d+)\]', raw, c)
        return c

    def vu_scan(self, sigs):
        """A4: the VU0 ABI of this function (an n4_abi for VU0): the VU0 registers, ACC, Q, status and GPR upper halves it
        reads before writing them on some path (vin, its own reads and its typed callees' vin), may write (vout: its own
        writes and its callees' vout) and writes on every path to every exit (vmust); a forward must-written analysis to
        a fixpoint over the CFG (loops). Every slot in vin | vout is a reference parameter; the body copies each into a
        local at entry and the outputs back at every exit, so a slot written on some paths only passes the caller's value
        through, as the lifted write-back did (exact by construction: nothing is dropped)."""
        lf = self.lf
        order = self.order
        rw = {}
        for y, c in self.code.items():
            rw[y] = vu_rw(c)
        mw = {order[0]: frozenset()}
        vin, vout = set(), set()
        exits = []
        changed = True
        while changed:
            changed = False
            for x in order:
                if x not in mw:
                    continue
                cur = set(mw[x])
                k = lf.kind[x]
                if k[0] == 'op':
                    r, w = rw.get(x, (set(), set()))
                    vin |= r - cur
                    cur |= w
                    vout |= w
                    mid = after = frozenset(cur)
                else:
                    if k[0] == 'br':
                        r, _ = vu_rw(lf.cond(k))
                        vin |= r - cur
                    mid = frozenset(cur)
                    r, w = rw.get(x + 4, (set(), set()))
                    vin |= r - cur
                    cur |= w
                    vout |= w
                    if k[0] in ('jal', 'tail'):
                        s = sigs[k[1]]
                        vin |= set(s.vin) - cur
                        vout |= set(s.vout)
                        cur |= set(s.vmust)
                    after = frozenset(cur)
                for s_ in lf.succ[x]:
                    e = mid if (k[0] == 'br' and k[5] and s_ != k[4]) else after
                    ne = e if s_ not in mw else (mw[s_] & e)
                    if mw.get(s_) != ne:
                        mw[s_] = ne
                        changed = True
                if k[0] in ('ret', 'tail'):
                    exits.append((x, after))
        vmust = None
        for x, a in exits:
            vmust = set(a) if vmust is None else (vmust & a)
        self.vin, self.vout, self.vmust = frozenset(vin), frozenset(vout), frozenset(vmust or ())
        bad = [r for k, r in self.vout if k == 'h' and r in SAVED]
        if bad:
            raise Refuse('upper half of a callee-saved register written')
        if any(k == 'h' and r == 0 for k, r in self.vin | self.vout):
            raise Refuse('upper half of zero')
        self.vio = tuple((s, s in self.vout) for s in sorted(self.vin | self.vout, key=vu_sortkey))

    def check_xout(self, sigs):
        """A3: every extra output holds the lifted text's value at every exit: written on every path (a forward
        must-analysis; a typed callee's scratch set kills, its return slots write), and at a tail exit not touched by
        the tail callee."""
        xs = set(self.xout)
        if any(q[0] not in ('g', 'f') and not (self.a13 and not HL13_OFF[0] and q in HL13) for q in xs) or not xs <= self.dropped:
            raise Refuse('extra output form')
        lf = self.lf
        order = self.order
        nodes = lf.nodes
        def flow(mode):
            # 'must': written by this function on every path (a typed callee's scratch set kills); 'may': on some path
            # (a callee's scratch set counts); 'unk': last changed by a callee on some path (its value unknown here)
            must = mode == 'must'
            st = {order[0]: frozenset()}
            after = {}
            changed = True
            while changed:
                changed = False
                for x in order:
                    if x not in st:
                        continue
                    n = nodes[x]
                    k = lf.kind[x]
                    cur = set(st[x])
                    if k[0] == 'op':
                        if mode == 'unk':
                            cur -= set(n['defs'])
                        else:
                            cur |= set(n['defs']) & xs
                        mid = frozenset(cur)
                    else:
                        mid = frozenset(cur)
                        if mode == 'unk':
                            cur -= set(n.get('ddefs', set()))
                        else:
                            cur |= set(n.get('ddefs', set())) & xs
                        if k[0] == 'jal':
                            cs = sigs[k[1]]
                            if must:
                                cur -= cs.dropped
                            else:
                                cur |= cs.dropped & xs
                            if mode == 'unk':
                                cur -= RET_SLOTS[cs.rtype]
                            else:
                                cur |= RET_SLOTS[cs.rtype] & xs
                            if self.a7:  # A7: the callee's extra outputs taken back: written by the call
                                cc = self.cons.get(x, set()) & xs
                                if mode == 'unk':
                                    cur -= cc
                                else:
                                    cur |= cc
                    after[x] = frozenset(cur)
                    for y in lf.succ[x]:
                        e = mid if (k[0] == 'br' and k[5] and y != k[4]) else after[x]
                        ne = e if y not in st else ((st[y] & e) if must else (st[y] | e))
                        if st.get(y) != ne:
                            st[y] = ne
                            changed = True
            return after
        mustw, mayw, unk = flow('must'), flow('may'), flow('unk')
        # per exit: an extra output written on every path to it is stored; one written on some paths only is stored
        # when its dirty flag says so (lift_d1's own write-back rule); one written on no path is left alone (the
        # adapter's xo starts as the context's value, which the lifted text would not have changed either)
        self.xo_at, self.xo_flag = {}, set()
        for x in order:
            k = lf.kind[x]
            if k[0] not in ('ret', 'tail'):
                continue
            mu, ma = mustw.get(x, frozenset()), mayw.get(x, frozenset())
            uk = unk.get(x, frozenset()) - (self.cons.get(x, set()) if self.a7 and k[0] == 'tail' else set())  # A7: a tail callee's outputs taken back
            if xs & uk:
                raise Refuse('extra output changed by a callee' + (f': {sorted(xs & unk.get(x, frozenset()))} at {x:x}; ' + ' '.join(
                    f'{y:x}->{lf.kind[y][1]:x}:{sorted(sigs[lf.kind[y][1]].dropped & xs)}/{sorted(self.cons.get(y, set()))}'
                    for y in order if lf.kind[y][0] == 'jal' and sigs[lf.kind[y][1]].dropped & xs) if self.a7 else ''))
            if k[0] == 'tail':
                cs = sigs[k[1]]
                cc = self.cons.get(x, set()) if self.a7 else set()
                if (xs & RET_SLOTS[cs.rtype]) or ((xs & cs.dropped) - cc):
                    raise Refuse('extra output changed by a tail callee')
                mu = frozenset(mu | (xs & cc))
            self.xo_at[x] = (xs & mu, (xs & ma) - mu)
            self.xo_flag |= (xs & ma) - mu

    def xo_flagset(self, slots):
        """`a3xwN = true;` for the flagged extra outputs among slots (written here)."""
        if not self.xout:
            return ''
        return ''.join(f' a3xw{i} = true;' for i, q in enumerate(self.xout) if q in self.xo_flag and q in slots)

    def xo_stores(self, x):
        if not self.xout or x not in self.xo_at:
            return ''
        mu, fl = self.xo_at[x]
        parts = []
        for i, (k, r) in enumerate(self.xout):
            if (k, r) not in mu and (k, r) not in fl:
                continue
            if k == 'g':
                n = L.GPR[r]
                st = f'xo[{i}] = {n};' if self.gty.get(r) == 'uint64_t' else f'xo[{i}] = a1_sx64({n});'
            elif k == 'x':
                st = f'xo[{i}] = {L.FIELDS[r][0]};'  # A13: hi / lo, the uint64_t locals
            else:
                st = f'xo[{i}] = a1_bits(f{r});'
            parts.append(st if (k, r) in mu else f'if (a3xw{i}) {st}')
        if not parts:
            return ''
        return 'if (xo) { ' + ' '.join(parts) + ' } '

    def spin_filter(self, order):
        """A3: a loop whose exit could wait on memory something else writes (another guest thread, an interrupt handler,
        the IOP, a timer) is refused. In the typed function the back edge's checkpoint is deferred to after the call, so
        nothing else runs during it: such a loop would spin (or take its timeout path) where the lifted one would have
        let the writer run. Rule (per natural loop, flow-insensitive, conservative): a value is tainted when it is loaded
        at an address the loop does not change (a base register not written in the loop, or a tainted one), except a
        frame slot (sp: private) and a slot the loop itself stores (same base and offset: a counter kept in memory);
        taint flows through every instruction of the loop; an exit branch whose condition reads a tainted register
        (or an FPU condition while an FPR is tainted) refuses the function."""
        lf = self.lf
        W = lf.g.words
        pred = collections.defaultdict(set)
        for x in order:
            for y in lf.succ[x]:
                pred[y].add(x)
        for (x, t) in sorted(self.backs):
            body, work = {t, x}, [x]
            while work:
                z = work.pop()
                if z == t:
                    continue
                for p in pred[z]:
                    if p not in body:
                        body.add(p)
                        work.append(p)
            ins = []
            for z in sorted(body):
                ins.append(z)
                if z + 4 in lf.delay:
                    ins.append(z + 4)
            gdefs = set()
            stores = set()
            for y in ins:
                w = W[y]
                gdefs |= self.word_defs(w) if w else set()
                op = w >> 26
                if op in STORES or op == 0x39:
                    stores.add(((w >> 21) & 31, L.sx16(w & 0xFFFF)))
            gdefs.discard(0)
            taint, ftaint = set(), False
            changed = True
            while changed:
                changed = False
                for y in ins:
                    w = W[y]
                    if not w:
                        continue
                    op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                    if op in LOADS:
                        base = rs
                        inv = base not in gdefs or base in taint
                        if base == 29 or (base, L.sx16(w & 0xFFFF)) in stores:
                            inv = base in taint
                        if inv:
                            if op == 0x31:
                                if not ftaint:
                                    ftaint = changed = True
                            elif rt and rt not in taint:
                                taint.add(rt)
                                changed = True
                        continue
                    uses = A.word_uses(w)
                    if op == 0x11 and rs == 0 and ftaint:  # mfc1 of a possibly tainted FPR
                        uses = uses | {-1}
                    if (uses & taint) or -1 in uses:
                        for d in self.word_defs(w):
                            if d and d not in taint:
                                taint.add(d)
                                changed = True
                        if op == 0x11 and rs == 4 and not ftaint:  # mtc1 of a tainted GPR
                            ftaint = changed = True
            fs = self.spin_flow(body, pred, gdefs, stores, taint) if (self.a8 and not SPIN8_OFF[0]) else None
            for z in body:
                k = lf.kind[z]
                if k[0] != 'br' or all(y in body for y in lf.succ[z]):
                    continue
                if k[1] in ('c1t', 'c1f'):
                    if ftaint:
                        raise Refuse('loop: exit may wait on memory (FPU)')
                elif {r for r in (k[2], k[3]) if r} & (taint if fs is None else fs[z]):
                    if self.a12 and not CNT12_OFF[0] and self.counted_exit(z, body, ins, pred, x, t):
                        self.counted12 = getattr(self, 'counted12', 0) + 1
                        continue
                    if getattr(self, 'a14', False) and not SRCH14_OFF[0] and self.bounded_search(z, body, ins, pred, x, t):
                        self.search14 = getattr(self, 'search14', 0) + 1
                        continue
                    raise Refuse('loop: exit may wait on memory')

    def bounded_search(self, z, body, ins, pred, latch, head):
        """A14 (SRCH14): an exit of a bounded search is not a wait. The loop has a counted exit that every iteration passes (it
        dominates the back edge's source): a test of an induction variable, a register the loop writes only by `addiu r, r, k`
        with k of one sign (at one place or several: a likely slot's step counts on its taken edge) and steps on every path
        around the loop; the test is A12's (a sign test, eq / ne with |k| = 1, or the slt / sltu / slti / sltiu just before
        the branch). So the loop ends by its own progress whatever memory holds. And the value the exit z tests depends on an
        element the loop walks: a load whose address depends on an induction variable, flow-insensitively through every
        instruction of the loop. A poll (a flag at an address the loop does not step, with or without a timeout counter)
        stays refused; a search's hit test compares the element it is on. With the typed call's checkpoints deferred (A3),
        the lifted and the typed loop take the same iterations unless another thread writes the walked data within the
        call: the deferral A3 already takes for every typed loop (an event taken at most one call late)."""
        lf = self.lf
        W = lf.g.words
        calls = any(lf.kind[y][0] in ('jal', 'jalr') for y in body if y in lf.kind)
        # induction variables: every write in the loop is addiu r, r, k (k != 0, one sign), and every path around the loop steps r
        ivs, steps = set(), {}
        for r in range(1, 32):
            defs = [y for y in ins if W[y] and r in self.word_defs(W[y])]
            if not defs:
                continue
            ks = []
            for d in defs:
                w = W[d]
                if w >> 26 != 9 or (w >> 21) & 31 != r or (w >> 16) & 31 != r or not (w & 0xFFFF):
                    break
                ks.append(L.sx16(w & 0xFFFF))
            else:
                if not (all(k_ > 0 for k_ in ks) or all(k_ < 0 for k_ in ks)):
                    continue
                if calls and r not in (16, 17, 18, 19, 20, 21, 22, 23, 30):
                    continue
                # every path head -> latch passes a step: a node whose own instruction (or a plain branch's slot) steps blocks
                # the walk; a likely branch whose slot steps covers its taken edge only
                block, cover = set(), set()
                for d in defs:
                    if d in lf.delay:
                        n = d - 4
                        if lf.kind[n][0] == 'br' and lf.kind[n][5]:
                            cover.add((n, lf.kind[n][4]))
                        else:
                            block.add(n)
                    else:
                        block.add(d)
                if head in block:
                    ok_ = True
                else:
                    seen, work, ok_ = {head}, [head], True
                    while work and ok_:
                        y = work.pop()
                        if y == latch:
                            ok_ = False
                            break
                        for q in lf.succ[y]:
                            if q in body and q != head and (y, q) not in cover and q not in block and q not in seen:
                                seen.add(q)
                                work.append(q)
                if ok_:
                    ivs.add(r)
                    steps[r] = set(abs(k_) for k_ in ks)
        if not ivs:
            return False

        def dominates(n):
            if n == head:
                return True
            seen, work = {head}, [head]
            while work:
                y = work.pop()
                for q in lf.succ[y]:
                    if q in body and q != n and q not in seen:
                        seen.add(q)
                        work.append(q)
            return latch not in seen

        def counted(c):
            k = lf.kind[c]
            cond, rs, rt = k[1], k[2], k[3]
            if cond in ('lez', 'gtz', 'ltz', 'gez'):
                return rs in ivs
            if cond not in ('eq', 'ne'):
                return False
            for a_ in (rs, rt):
                if a_ in ivs and steps[a_] == {1}:
                    return True
            v = rs if rt == 0 else rt if rs == 0 else 0
            if not v or c - 4 not in body or c - 4 in lf.delay or pred[c] != {c - 4} or lf.kind[c - 4][0] != 'op':
                return False
            w = W[c - 4]
            op, ws, wt, wd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
            if op == 0 and fn in (0x2A, 0x2B) and wd == v:
                return ws in ivs or wt in ivs
            if op in (0x0A, 0x0B) and wt == v:
                return ws in ivs
            return False
        if not any(lf.kind.get(c, ('op',))[0] == 'br' and not all(y in body for y in lf.succ[c]) and counted(c) and dominates(c) for c in body):
            return False
        # registers that hold an element of the walk (a load through an induction-derived address) or a value made from one
        varying, elem = set(ivs), set()
        changed = True
        while changed:
            changed = False
            for y in ins:
                w = W[y]
                if not w:
                    continue
                op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                if op in LOADS:
                    if rs in varying and op != 0x31 and rt:
                        for s_ in (varying, elem):
                            if rt not in s_:
                                s_.add(rt)
                                changed = True
                    continue
                uses = A.word_uses(w)
                for d in self.word_defs(w):
                    if not d:
                        continue
                    if uses & varying and d not in varying:
                        varying.add(d)
                        changed = True
                    if uses & elem and d not in elem:
                        elem.add(d)
                        changed = True
        k = lf.kind[z]
        return bool({r for r in (k[2], k[3]) if r} & elem)

    def counted_exit(self, z, body, ins, pred, latch, head):
        """A12 (CNT12): an exit of a counted loop is not a wait. The exit branch z tests an induction variable: a register the loop
        writes once, `addiu iv, iv, k` (k != 0), at an instruction every iteration runs (it dominates the back edge's source; not a
        likely branch's slot), not clobbered by a call in the loop unless callee-saved; the branch reads iv itself (a sign test, or
        eq / ne against anything with k = +-1) or the result of the instruction just before it (z's only predecessor), an
        slt / sltu / slti / sltiu with iv as an operand. Whatever the other operand (a bound reloaded from memory each iteration),
        the counter advances on every iteration: the exit comes from the loop's own progress, not from another thread's write
        (with no other thread running, the lifted and the typed loop take the same iterations)."""
        lf = self.lf
        W = lf.g.words
        k = lf.kind[z]
        calls = any(lf.kind[y][0] in ('jal', 'jalr') for y in body if y in lf.kind)

        def iv_step(r):
            if not r:
                return None
            defs = [y for y in ins if W[y] and r in self.word_defs(W[y])]
            if len(defs) != 1:
                return None
            d = defs[0]
            w = W[d]
            if w >> 26 != 9 or (w >> 21) & 31 != r or (w >> 16) & 31 != r or not (w & 0xFFFF):
                return None
            if calls and r not in (16, 17, 18, 19, 20, 21, 22, 23, 30):
                return None
            n = d
            if d in lf.delay:
                n = d - 4
                if lf.kind[n][0] == 'br' and lf.kind[n][5]:
                    return None
            if n != head:
                seen, work = {head}, [head]
                while work:
                    y = work.pop()
                    for q in lf.succ[y]:
                        if q in body and q != n and q not in seen:
                            seen.add(q)
                            work.append(q)
                if latch in seen:
                    return None
            return L.sx16(w & 0xFFFF)
        cond, rs, rt = k[1], k[2], k[3]
        if cond in ('lez', 'gtz', 'ltz', 'gez'):
            return iv_step(rs) is not None
        if cond in ('eq', 'ne'):
            for a_, b_ in ((rs, rt), (rt, rs)):
                st_ = iv_step(a_)
                if st_ in (1, -1):
                    return True
            c = rs if rt == 0 else rt if rs == 0 else 0
            if not c or z - 4 not in body or z - 4 in lf.delay or pred[z] != {z - 4} or lf.kind[z - 4][0] != 'op':
                return False
            w = W[z - 4]
            op, ws, wt, wd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
            if op == 0 and fn in (0x2A, 0x2B) and wd == c:
                return iv_step(ws) is not None or iv_step(wt) is not None
            if op in (0x0A, 0x0B) and wt == c:
                return iv_step(ws) is not None
        return False

    def spin_flow(self, body, pred, gdefs, stores, taint0):
        """A8: the GPR taint of spin_filter flow-sensitive: per instruction of the loop, the registers tainted where the exit
        branch reads them (a register written by an untainted instruction is clean after it: a counter compared with slti
        after the loop loaded something else into the same register is not a wait). The loaded-value rule is spin_filter's
        (an address the loop does not change, its flow-insensitive taint for the base); the loop is entered clean (a value
        loaded before the loop is read once, not waited on). Returns {branch: tainted registers before it}."""
        lf = self.lf
        W = lf.g.words
        spin12 = getattr(self, 'a12', False) and not SPIN12_OFF[0]

        def xfer(z, st):
            st = set(st)
            for y in ((z, z + 4) if z + 4 in lf.delay else (z,)):
                w = W[y]
                if not w:
                    continue
                op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                if op in LOADS:
                    base = rs
                    # A12 (SPIN12): the base's taint flow-sensitive too (st, the registers tainted before this instruction),
                    # not the loop's flow-insensitive set: a list walk's pointer register, loaded from the node it walks, is
                    # clean where the walk reaches it even if the same register holds a loaded global elsewhere in the loop
                    bt = st if spin12 else taint0
                    inv = base not in gdefs or base in bt
                    if base == 29 or (base, L.sx16(w & 0xFFFF)) in stores:
                        inv = base in bt
                    if op != 0x31 and rt:
                        if inv or base in st:
                            st.add(rt)
                        else:
                            st.discard(rt)
                    continue
                uses = A.word_uses(w)
                t = bool(uses & st)
                for d in self.word_defs(w):
                    if d:
                        if t:
                            st.add(d)
                        else:
                            st.discard(d)
            return st
        IN = {z: set() for z in body}
        OUT = {z: set() for z in body}
        changed = True
        while changed:
            changed = False
            for z in sorted(body):
                i = set()
                for p in pred[z]:
                    if p in body:
                        i |= OUT[p]
                o = xfer(z, i)
                if i != IN[z] or o != OUT[z]:
                    IN[z], OUT[z] = i, o
                    changed = True
        return IN

    def check_restored_dead(self, ff, sigs):
        if not self.restores:
            return
        lf = self.lf
        nodes = ff['nodes']
        order = self.order
        live = {x: set() for x in order}
        changed = True

        def uses_call(t):
            return {p[1] for p in sigs[t].params if p[0] == 'g'}
        while changed:
            changed = False
            for x in reversed(order):
                kk, info, succ, gu, dgu, defs, ddefs = nodes[x][:7]
                gdefs = {r for k, r in defs if k == 'g'}
                gdd = {r for k, r in ddefs if k == 'g'}
                out = set()
                for y in succ:
                    out |= live.get(y, set())
                if kk == 'ret':
                    li = set(dgu)
                elif kk == 'tail':
                    li = set(dgu) | (uses_call(info) - gdd)
                elif kk == 'jal':
                    li = set(dgu) | ((uses_call(info) | (out - {2, 3})) - gdd)
                else:
                    li = set(gu) | set(dgu) | (out - gdefs - gdd)
                if li != live[x]:
                    live[x] = li
                    changed = True
        for y, (r, size) in self.restores.items():
            x = y - 4 if y in lf.delay else y
            kk = nodes[x][0]
            if y == x:  # an op node: live after it
                after = set()
                for s in nodes[x][2]:
                    after |= live.get(s, set())
            else:
                if kk == 'ret':
                    after = set()
                elif kk == 'tail':
                    after = uses_call(nodes[x][1])
                elif kk == 'jal':
                    after = uses_call(nodes[x][1])
                    for s in nodes[x][2]:
                        after |= live.get(s, set())
                elif getattr(self, 'a12', False) and not LIK12_OFF[0] and lf.kind[x][0] == 'br' and lf.kind[x][5]:
                    # A12 (LIK12): a likely branch's delay slot runs only on the taken edge (lift_d1's D2 rule): the
                    # restore is live after it only where the branch goes; the fall-through never restored the register
                    after = set(live.get(lf.kind[x][4], set()))
                else:
                    after = set()
                    for s in nodes[x][2]:
                        after |= live.get(s, set())
            if r in after:
                raise Refuse('restored register read')

    def a7_live(self, sigs, ffacts, ret_live):
        """A7: backward liveness over this function (as live_typed_caller): its exits read what it returns and its extra
        outputs (a caller of it may take them back); a call reads its callee's parameters and the callee's extra outputs it
        may leave unchanged (the value before the call passes through them); {node: live after}."""
        nodes, order = ffacts['nodes'], ffacts['order']

        def g(s_):
            return {('g', r) for r in s_}
        live = {x: set() for x in order}
        changed = True
        while changed:
            changed = False
            for x in reversed(order):
                kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq = nodes[x]
                out = set()
                for y in succ:
                    out |= live.get(y, set())
                uses, duses = g(gu) | set(ou), g(dgu) | set(dou)
                if kk == 'op':
                    li = uses | (out - defs)
                elif kk == 'br':
                    if info[1]:
                        lt = live.get(info[0], set())
                        lnt = set()
                        for y in succ:
                            if y != info[0]:
                                lnt |= live.get(y, set())
                        li = uses | duses | (lt - ddefs) | lnt
                    else:
                        li = uses | duses | (out - ddefs)
                elif kk in ('jal', 'tail'):
                    s_ = sigs[info]
                    xm = set(getattr(s_, 'xmust', ())) & set(getattr(s_, 'xout', ()))
                    after = (out - RET_SLOTS[s_.rtype] - {('g', 31)}) if kk == 'jal' else (ret_live - RET_SLOTS[s_.rtype])
                    li = duses | ((s_.inputs() | (after - xm)) - ddefs)
                elif kk == 'ret':
                    li = duses | (ret_live - ddefs)
                else:
                    li = uses | duses | (out - ddefs)
                li.discard(('g', 0))
                li.discard(('g', 31))
                li.discard(('g', 29))
                if li != live[x]:
                    live[x] = li
                    changed = True
        res = {}
        for x in order:
            kk = nodes[x][0]
            if kk == 'jal':
                a = set()
                for y in nodes[x][2]:
                    a |= live.get(y, set())
                res[x] = a
        return res

    def a7_consume(self, sigs, ffacts):
        """A7: the typed callees' scratch slots this function reads after a call (or passes on as its own extra outputs past
        a tail call) are taken back from the callee's extra outputs (self.cons: node -> slots); a slot the callee does not
        give back raises NeedXout (the generator asks the callee for it). A consumed slot is a wide local (the callee's whole
        64-bit value) and, unless the callee writes it on every path, its value before the call is passed in (an input when
        not written on every path before the call): both found here, then the function is emitted again (A7Restart)."""
        lf = self.lf
        # a callee's 64-bit return into v0 here: v0 a wide local (the lifter made it narrow: its own uses are 32-bit; a wide
        # local is exact by construction, it holds the whole value as the context would)
        if not lf.mode64 and 2 not in lf.wide and 2 not in self.force_wide:
            for x in self.order:
                k = lf.kind[x]
                if k[0] == 'jal' and sigs[k[1]].v0w == 64 and sigs[k[1]].rtype in ('uint64_t', 'a1::RetVF') and ('g', 2) in self.slots:
                    self.force_wide.add(2)
                    raise A7Restart()
        # a callee's float inputs stage 7 added (its consumed extra outputs' values before its own calls: xfin) are not in the
        # global float-input facts this function's inputs come from (fpr_x_inputs): one not written here on every path before
        # the call is this function's entry value, an input too (the property passes up the typed callers)
        efin0 = set()
        for x in self.order:
            k = lf.kind[x]
            if k[0] in ('jal', 'tail'):
                before = self.mw_in.get(x, frozenset()) | set(lf.nodes[x].get('ddefs', set()))
                efin0 |= {r for r in getattr(sigs[k[1]], 'xfin', ()) if ('f', r) not in before}
                if self.a8:
                    # A8: any float parameter of a typed callee (a native typed from its MIPS: its row in the global facts is
                    # the native's, argument registers only; a kept FPR save such as f20's is an input of the typed function)
                    efin0 |= {p[1] for p in sigs[k[1]].params if p[0] == 'f' and ('f', p[1]) not in before}
        if not efin0 <= self.extra_fin | self.fins:
            self.extra_fin |= efin0
            raise A7Restart()
        if self.a13:  # A13 (XIN13): a typed callee's hi / lo parameter not written here before the call: this function's input
            exin0 = set()
            for x in self.order:
                k = lf.kind[x]
                if k[0] in ('jal', 'tail'):
                    before = self.mw_in.get(x, frozenset()) | set(lf.nodes[x].get('ddefs', set()))
                    exin0 |= {p[1] for p in sigs[k[1]].params if p[0] == 'x' and ('x', p[1]) not in before}
            if not exin0 <= self.extra_xin:
                self.extra_xin |= exin0
                raise A7Restart()
        ret_live = set(RET_SLOTS[self.rtype]) | set(self.xout)
        live_after = self.a7_live(sigs, ffacts, ret_live)
        need, cons = collections.defaultdict(set), {}
        for x in self.order:
            k = lf.kind[x]
            if k[0] not in ('jal', 'tail'):
                continue
            s_ = sigs[k[1]]
            if k[0] == 'jal':
                want = set(live_after.get(x, set())) & set(s_.dropped)
            else:
                want = set(self.xout) & set(s_.dropped)
            if not want:
                continue
            if A13_DIAG[0] and any(q[0] not in ('g', 'f') for q in want):
                rd = sorted(f'{y:x}' for y, n in ffacts['nodes'].items() if {('x', 'hi'), ('x', 'lo'), ('x', 'fcr31')} & (set(n[7]) | set(n[8])))
                print(f'[a13diag] typed {self.name} {k[0]} @{x:x} -> {k[1]:x} wants {sorted(map(str, want))} (xout {sorted(map(str, self.xout))}); field readers {rd[:12]}', file=sys.stderr)
            have = set(getattr(s_, 'xout', ()))
            miss = want - have
            if miss:
                if XO7_OFF[0]:
                    raise Refuse('a callee scratch / extra output read after the call')
                if any(q[0] not in ('g', 'f') and not (self.a13 and not HL13_OFF[0] and q in HL13) for q in miss):
                    raise Refuse('a callee scratch read after the call (field)' + (': ' + ' '.join(f'{k[1]:x}:{sorted(map(str, miss))}' for _ in [0]) if A13_DIAG[0] else ''))
                need[k[1]] |= miss
            cons[x] = want & have
        if need:
            raise NeedXout(dict(need))
        self.cons = {x: c for x, c in cons.items() if c}
        if not self.cons:
            return
        fw = {q[1] for c in self.cons.values() for q in c if q[0] == 'g'}
        self.mw_in = self.must_written(sigs)
        ein, efin, exin = set(), set(), set()
        for x, c in self.cons.items():
            s_ = sigs[lf.kind[x][1]]
            xm = set(getattr(s_, 'xmust', ()))
            before = self.mw_in.get(x, frozenset()) | set(lf.nodes[x].get('ddefs', set()))
            for q in c - xm:
                if q not in before:
                    if q[0] == 'x':  # A13 (XIN13): its value before the call, an input of this function
                        exin.add(q[1])
                        continue
                    (ein if q[0] == 'g' else efin).add(q[1])
        if (not lf.mode64 and not fw <= set(lf.wide)) or not ein <= self.extra_in or not efin <= self.extra_fin or not exin <= self.extra_xin:
            self.force_wide |= fw
            self.extra_in |= ein
            self.extra_fin |= efin
            self.extra_xin |= exin
            raise A7Restart()
        for c in self.cons.values():
            for q in c:
                self.slots.add(q)
                if q[0] == 'g':
                    assert self.gty.get(q[1], 'uint64_t') == 'uint64_t' or lf.mode64, (self.name, q)
                    self.gty[q[1]] = 'uint64_t'

    def a7_cx(self, x, s_):
        """A7: the consumed extra outputs at call x: the array passed (each slot's value before the call where the callee
        may leave it unchanged, else 0) and the locals taken back after the call."""
        c = self.cons.get(x)
        if not c:
            return '', ''
        xm = set(getattr(s_, 'xmust', ()))
        init, post = [], []
        for i, q in enumerate(s_.xout):
            if q in c and q not in xm:
                init.append(L.GPR[q[1]] if q[0] == 'g' else L.FIELDS[q[1]][0] if q[0] == 'x' else f'(uint64_t)a1_bits(f{q[1]})')
            else:
                init.append('0u')
            if q in c:
                post.append(f'{L.GPR[q[1]]} = a7c[{i}];' if q[0] == 'g' else f'{L.FIELDS[q[1]][0]} = a7c[{i}];' if q[0] == 'x' else f'f{q[1]} = a1_flt((uint32_t)a7c[{i}]);')
        return f'uint64_t a7c[{len(s_.xout)}] = {{{", ".join(init)}}}; ', ' ' + ' '.join(post)

    def must_written(self, sigs):
        """Slots written on every path before each node (A3: to a fixpoint over loops; the sets only shrink)."""
        lf = self.lf
        nodes = lf.nodes
        order = self.order
        mw = {order[0]: frozenset()}
        changed = True
        while changed:
            changed = False
            self.after = {}
            for x in order:
                if x not in mw:
                    continue
                n = nodes[x]
                k = lf.kind[x]
                cur = set(mw[x])
                if k[0] == 'op':
                    cur |= set(n['defs'])
                    mid = after = frozenset(cur)
                else:
                    mid = frozenset(cur)
                    cur |= set(n.get('ddefs', set()))
                    if k[0] == 'jal':
                        cur |= RET_SLOTS[sigs[k[1]].rtype]
                        if self.a7:
                            cur |= self.cons.get(x, set())
                    after = frozenset(cur)
                self.after[x] = after
                for s in lf.succ[x]:
                    e = mid if (k[0] == 'br' and k[5] and s != k[4]) else after
                    ne = e if s not in mw else (mw[s] & e)
                    if mw.get(s) != ne:
                        mw[s] = ne
                        changed = True
        return mw

    def mw_after(self, x):
        return set(self.after.get(x, frozenset()))

    # ------------------------------------------------------------------------------------------------ emission
    def call_expr(self, x, s):
        """The typed call of callee `s` at node x (its arguments from the caller's locals)."""
        args = []
        before = self.mw_in.get(x, frozenset()) | set(self.lf.nodes[x].get('ddefs', set()))
        for p in s.params:
            if p[0] == 'f':
                args.append(f'f{p[1]}')
                continue
            if p[0] == 'x':  # A13 (XIN13): the caller's hi / lo local
                args.append(L.FIELDS[p[1]][0])
                continue
            r, ty = p[1], p[2]
            n = L.GPR[r]
            mine = self.gty[r]
            if r == 29:
                # A5: the guest sp at the call (after its delay slot): the local in a guest frame, else the entry sp moved by
                # the host frame's offset there (the callee's guest frame at the lifted text's addresses)
                e = 'sp' if self.gf else f'(uint32_t)(sp + 0x{self.spcall[x] & 0xFFFFFFFF:X}u)'
                if self.gf and mine == 'uint64_t':
                    e = 'sp' if ty == 'uint64_t' else '(uint32_t)sp'
                elif ty == 'uint64_t':
                    e = f'a1_sx64({e})'
                args.append(e)
                continue
            if r in self.wide_in and ty == 'uint64_t' and ('g', r) not in before:
                args.append(f'w64_{n}')
                continue
            if ty == 'uint32_t':
                args.append(n if mine == 'uint32_t' else f'(uint32_t){n}')
            elif mine == 'uint64_t':
                args.append(n)
            elif ('g', r) in before:
                args.append(f'a1_sx64({n})')
            else:
                raise Refuse('64-bit pass-through argument')
        for q, _ in s.vio:  # A4: the callee's VU0 interface, the caller's locals by reference
            args.append(vu_local(q)[0])
        if self.a7 and self.cons.get(x):
            args.append('a7c')  # A7: the callee's extra outputs this caller takes back
        return f'{s.name}(ram, rt{"".join(", " + a for a in args)})'

    def body(self, sigs):
        lf = self.lf
        order = self.order
        nodes = lf.nodes
        nxt = {order[i]: order[i + 1] for i in range(len(order) - 1)}
        targets = set()
        for x in order:
            k = lf.kind[x]
            for s in lf.succ[x]:
                if s != nxt.get(x) or k[0] in ('j',) or (k[0] == 'br' and s == k[4]):
                    targets.add(s)
            targets.update(self.jtabs.get(x, ()))
        L_ = []
        I = '    '

        def ret_expr():
            if self.rtype == 'void':
                return 'return;'
            v0 = 'v0' if self.gty.get(2) == 'uint64_t' else ('a1_sx64(v0)' if self.v0w == 64 else 'v0')
            if self.rtype == 'a1::RetVF':
                v0 = 'v0' if self.gty.get(2) == 'uint64_t' else 'a1_sx64(v0)'
                return f'return a1::RetVF{{(uint64_t){v0}, f0}};'
            if self.rtype == 'float':
                return 'return f0;'
            return f'return {v0};'

        def tail_expr(s, call):
            R, C = self.rtype, s.rtype
            if R == 'void':
                return f'(void){call}; return;'
            if R == C:
                return f'return {call};'
            if R == 'uint64_t' and C == 'uint32_t':
                return f'return a1_sx64({call});'
            if R == 'uint64_t' and C == 'a1::RetVF':
                return f'return {call}.v0;'
            if R == 'uint32_t' and C == 'a1::RetVF' and s.v0w == 32:
                return f'return (uint32_t){call}.v0;'
            if R == 'float' and C == 'a1::RetVF':
                return f'return {call}.f0;'
            raise Refuse(f'tail return {C} -> {R}')

        def assign(s, call):
            C = s.rtype
            v0n = self.gty.get(2)
            if C == 'void':
                return f'{call};'
            if C == 'float':
                return f'f0 = {call};' if ('f', 0) in self.slots else f'(void){call};'
            if C in ('uint32_t', 'uint64_t'):
                if v0n is None:
                    return f'(void){call};'
                if C == 'uint32_t':
                    return f'v0 = {call};' if v0n == 'uint32_t' else f'v0 = a1_sx64({call});'
                if v0n == 'uint64_t':
                    return f'v0 = {call};'
                raise Refuse('64-bit return into a narrow local')
            # RetVF
            parts = ['const a1::RetVF a2_r = ' + call + ';']
            if v0n == 'uint64_t':
                parts.append('v0 = a2_r.v0;')
            elif v0n == 'uint32_t':
                if s.v0w == 64:
                    raise Refuse('64-bit return into a narrow local')
                parts.append('v0 = (uint32_t)a2_r.v0;')
            if ('f', 0) in self.slots:
                parts.append('f0 = a2_r.f0;')
            return '{ ' + ' '.join(parts) + ' }'
        for x in order:
            k = lf.kind[x]
            if x in targets:
                L_.append(f'L_{x:x}:;')
            if k[0] == 'op':
                if self.code[x]:
                    L_.append(I + self.code[x] + self.xo_flagset(nodes[x]['defs']))
                continue
            d = self.code.get(x + 4, '')
            if d and self.xout:
                d = d + self.xo_flagset(nodes[x].get('ddefs', set()))
            if k[0] == 'br':
                c = lf.cond(k)
                t = k[4]
                # A3: a back edge (target at or before the branch, lift_d1's rule) charges the clock as the lifted
                # checkpoint does; the test is deferred to the first checkpoint after the typed call (A3_BACK)
                gt = f'{{ A3_BACK(); goto L_{t:x}; }}' if t <= x else f'goto L_{t:x};'
                if k[5]:
                    L_.append(f'{I}if ({c}) {{ {d}{" " if d else ""}{gt} }}')
                elif c == 'true':
                    if d:
                        L_.append(I + d)
                    L_.append(f'{I}{gt}')
                elif d:
                    L_.append(f'{I}{{ const bool c = {c}; {d} if (c) {gt} }}')
                else:
                    L_.append(f'{I}if ({c}) {gt}')
            elif k[0] == 'j':
                if d:
                    L_.append(I + d)
                L_.append(f'{I}{{ A3_BACK(); goto L_{k[1]:x}; }}' if k[1] <= x else f'{I}goto L_{k[1]:x};')
            elif k[0] == 'ret':
                L_.append(f'{I}{{ {d}{" " if d else ""}{self.xo_stores(x)}{self.vu_out()}{ret_expr()} }}')
            elif k[0] == 'jal':
                s = sigs[k[1]]
                pre, post = self.a7_cx(x, s) if self.a7 else ('', '')
                L_.append(f'{I}{{ {d}{" " if d else ""}{"A3_CHARGE" if self.loops else "A2_CHARGE"}(); {pre}{assign(s, self.call_expr(x, s))}{post}'
                          f'{self.xo_flagset(RET_SLOTS[s.rtype] | (self.cons.get(x, set()) if self.a7 else set()))} }}')
            elif k[0] == 'tail' and self.a7 and self.cons.get(x):
                # A7: a tail call whose callee gives back this function's extra outputs: the call, the outputs taken back,
                # then this function's extra outputs stored and its VU0 outputs back, then the return
                s = sigs[k[1]]
                pre, post = self.a7_cx(x, s)
                call = self.call_expr(x, s)
                if self.rtype == 'void':
                    tb = f'{pre}{call};{post} {self.xo_stores(x)}{self.vu_out()}return;'
                else:
                    tb = f'{pre}const {s.rtype} a7_t = {call};{post} {self.xo_stores(x)}{self.vu_out()}{tail_expr(s, "a7_t")}'
                L_.append(f'{I}{{ {d}{" " if d else ""}{tb} }}')
            elif k[0] == 'tail':
                s = sigs[k[1]]
                if self.vu_out():
                    # A4: the VU0 outputs go back after the tail callee wrote them
                    call = self.call_expr(x, s)
                    if self.rtype == 'void':
                        tb = f'{call}; {self.vu_out()}return;'
                    else:
                        te = tail_expr(s, 'a4_t')
                        tb = f'const {s.rtype} a4_t = {call}; {self.vu_out()}{te}'
                    L_.append(f'{I}{{ {d}{" " if d else ""}{self.xo_stores(x)}{tb} }}')
                else:
                    L_.append(f'{I}{{ {d}{" " if d else ""}{self.xo_stores(x)}{tail_expr(s, self.call_expr(x, s))} }}')
            elif k[0] == 'jr' and x in self.jtabs:
                # A5: a jump table: the register read before the delay slot runs (MIPS), then a switch over the recompiler's
                # case list, as lift_d1's text (`switch (jumpTarget) { case ...: goto ...; }`, no checkpoint charged); a
                # value outside the list (lift_d1 jumps out to it: never in this game) stops in A5_BADJUMP
                rs = k[1]
                jt = L.GPR[rs] if self.gty.get(rs) == 'uint32_t' else f'(uint32_t){L.GPR[rs]}'
                cases = ' '.join(f'case 0x{t:X}u: goto L_{t:x};' for t in self.jtabs[x])
                L_.append(f'{I}{{ const uint32_t a5_jt = {jt}; {d}{" " if d else ""}switch (a5_jt) {{ {cases} default: A5_BADJUMP(0x{x:X}u, a5_jt); }} }}')
            else:
                raise Refuse(f'kind {k[0]}')
        text = '\n'.join(L_)
        # A1's text forms: memory through ram / rt, PS2 float helpers by name
        text = T1.MEM_RE.sub(lambda m: f'A1_{"R" if m.group(1) == "READ" else "W"}{m.group(2)}{"B" if m.group(3) else ""}(', text)
        text = text.replace('lift_d1::flt(', 'a1_flt(').replace('lift_d1::bits(', 'a1_bits(').replace('lift_d1::sx64(', 'sx64(')
        text = re.sub(r'(?<![\w:])sx64\(', 'a1_sx64(', text)
        try:
            for kk, fn in T1.FPU_PLAIN.items():
                text = T1.msub(text, kk, fn)
            for kk, h in T1.FPU_PS2.items():
                text = T1.msub(text, kk, lambda a, h=h: f'{h}({", ".join(a)})')
        except L.Refuse:
            raise Refuse('parse')
        if self.vu:
            try:
                text = vu_text(text)
            except L.Refuse:
                raise Refuse('parse')
        if self.a7:
            # A7: a GPR's word moved into an FPR (mtc1: the float constants lui / ori build) through a7_opq, an empty asm the
            # compiler cannot see through: no float operation of this function (or of a callee inlined into it) is evaluated
            # at compile time in the host's default rounding; the PS2 / the game thread round toward zero at run time (the
            # harness caught -89.0f * (pi / 180) folded one ulp away from the run-time product)
            gn = '|'.join(L.GPR[1:])
            text = re.sub(r'\{ uint32_t bits = (' + gn + r'); std::memcpy\(&f(\d+), &bits, sizeof\(bits\)\); \}',
                          lambda m: f'{{ uint32_t bits = a7_opq({m.group(1)}); std::memcpy(&f{m.group(2)}, &bits, sizeof(bits)); }}', text)
            text = re.sub(r'\ba1_flt\((' + gn + r')\)', r'a1_flt(a7_opq(\1))', text)
        if re.search(r'\bctx\b', text):
            raise Refuse('ctx use')
        if re.search(r'\b(rdram|runtime|target)\b', text):
            raise Refuse('rdram / runtime use')
        if re.search(r'\bra\b', text):
            raise Refuse('ra use')
        if 'LIFT_SET_FACC' in text:
            text = text.replace('LIFT_SET_FACC(', 'A2_SET_FACC(')
        self.fpu_ps2 = sorted({h for h in T1.FPU_PS2.values() if h + '(' in text})
        ok = T1.OK_CALLS | {s.name for s in sigs.values()} | {'A2_CHARGE', 'a2::frLd', 'a2::frSt', 'A2_SET_FACC', 'A3_BACK', 'A3_CHARGE', 'A5_BADJUMP'} | ({'A6_BREAK'} if self.a6 else set())
        if self.vu:
            ok = ok | A4_KNOWN
        if self.a7:
            ok = ok | A7_KNOWN
        if getattr(self, 'a14', False):
            ok = ok | A14_KNOWN
        bad = set()
        for m in re.finditer(r'([A-Za-z_][\w:]*)\s*\(', text):
            g = m.group(1)
            if g in ok or re.match(r'^a2::fr(Ld|St)<', text[m.start():m.start() + 12]) or re.match(r'^[a-z]\w*$', g):
                continue
            bad.add(g)
        bad -= {'a2::frLd', 'a2::frSt'}
        if bad:
            raise Refuse('unknown call ' + ','.join(sorted(bad))[:60])
        # declarations
        params, locs = [], []
        self.gparams, self.fparams = [], []
        self.xparams = []  # A13 (XIN13): (field, local)
        for k, r in sorted(self.slots, key=lambda s: (s[0], str(s[1]).zfill(3))):
            if k == 'g':
                ty, n = self.gty[r], L.GPR[r]
                if r in self.gin and r in self.wide_in:
                    # A5: the whole 64-bit input (passed on to a callee's 64-bit parameter), its low half the local
                    params.append(f'uint64_t w64_{n}')
                    self.gparams.append((r, 'uint64_t', f'w64_{n}'))
                    locs.append(f'{ty} {n} = (uint32_t)w64_{n};')
                elif r in self.gin:
                    params.append(f'{ty} {n}')
                    self.gparams.append((r, ty, n))
                else:
                    locs.append(f'{ty} {n} = 0;')
            elif k == 'f':
                if r in self.fins:
                    params.append(f'float f{r}')
                    self.fparams.append((r, f'f{r}'))
                else:
                    locs.append(f'float f{r} = 0.0f;')
            elif k == 'x':
                if self.vu and r in VU_FIELDS:
                    continue
                loc, ty = L.FIELDS[r]
                if r in self.extra_xin:
                    self.xparams.append((r, loc))
                    continue
                locs.append(f'{ty} {loc} = 0;')
        vparams = []
        for q, isout in self.vio:  # A4: the VU0 interface: a local copy of each reference parameter
            nm, ty, _ = vu_local(q)
            locs.append(f'{ty} {nm} = io_{nm};')
            if isout:
                vparams.append(f'{ty} &io_{nm}')
            elif ty == 'a4::vec4':
                vparams.append(f'const {ty} &io_{nm}')
            else:
                vparams.append(f'{ty} io_{nm}')
        if self.frame and any(1 for y in self.acc if y not in self.save_at and y not in self.restores):
            locs.append(f'uint8_t a2fr[{self.frame}];')
        for i, q in enumerate(self.xout):
            if q in self.xo_flag:
                locs.append(f'bool a3xw{i} = false;')
        else:
            text = text  # no frame slots left
        params = [f'{ty} {n}' for r, ty, n in self.gparams] + [f'float {n}' for r, n in self.fparams] + [f'uint64_t {n}' for r, n in self.xparams] + vparams
        if self.xout:
            params = params + ['uint64_t *xo = nullptr']
        sig = ', '.join(['uint8_t *ram', '[[maybe_unused]] PS2Runtime *rt'] + params)
        self.decl = f'{self.rtype} {self.name}({sig})'
        self.sig = Sig(self.name, self.addr, self.rtype, [('g', r, ty) for r, ty, _ in self.gparams] + [('f', r) for r, _ in self.fparams] + [('x', r) for r, _ in self.xparams],
                       self.dropped, self.v0w, 'a6' if self.a6 else 'a5' if self.a5 else 'a4' if self.vu else 'a2')
        self.sig.brk = bool(self.brks) or any(getattr(sigs[c], 'brk', False) for c in self.callees)
        if self.a7:
            # A7: the extra outputs (a typed caller takes them back) and the ones written on every path to every exit
            self.sig.xout = self.xout
            xm = None
            for x_, (mu, fl) in getattr(self, 'xo_at', {}).items():
                xm = set(mu) if xm is None else (xm & set(mu))
            self.sig.xmust = frozenset(xm or ()) if self.xout else frozenset()
            self.sig.stk = self.stk
            self.sig.xfin = frozenset(self.extra_fin)
        # A5: the relocated sites this function reaches (its own and its typed callees'): the adapter tests each word
        rel = set(self.relocs)
        for c in self.callees:
            rel |= getattr(sigs[c], 'relocs', set())
        self.sig.relocs = self.relocs_all = rel
        self.reloc_words = {site: self.lf.g.words[site] for site, _ in self.relocs}
        self.sig.vin, self.sig.vout, self.sig.vmust, self.sig.vio = self.vin, self.vout, self.vmust, self.vio
        nfr = sum(1 for y in self.acc if y not in self.save_at and y not in self.restores)
        self.kind = ('guest frame ' if self.gf else 'frame ' if self.acc else '') + ('calls' if self.callees else 'leaf')
        if self.loops:
            self.kind = ('loop ' if self.backs else '') + self.kind
        xo = ''
        if self.xout:
            xo = ', extra outputs ' + ' '.join(f'xo[{i}]={xo_nm(k, r)}' for i, (k, r) in enumerate(self.xout))
        if self.vu:
            def vn(ss):
                return ' '.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey)) or '-'
            xo += f'; VU0 in {vn(self.vin)}, out {vn(self.vout)}'
        if self.a5:
            xo += ''.join([', jump tables %d' % len(self.jtabs) if self.jtabs else '', ', relocated calls %d' % len(self.relocs) if self.relocs else '',
                           ', 64-bit inputs ' + ' '.join(L.GPR[r] for r in sorted(self.wide_in)) if self.wide_in else '',
                           ', guest sp' if self.needs_sp and not self.gf else ''])
        if self.a6:
            xo += ', break' if self.brks else ''
        out = (f'// {self.name} (0x{self.addr:x}): typed by tools/typed_lift2.py (round {"A6" if self.a6 else "A5" if self.a5 else "A4" if self.vu else "A3" if self.loops else "A2"}; {self.kind}{xo}'
               f'{", frame %d bytes, %d slot accesses" % (self.frame, nfr) if self.acc else ""})\n'
               f'inline {self.decl}\n{{\n' + ''.join(f'    {x}\n' for x in locs) + text + '\n    __builtin_unreachable();\n}\n')
        return out

    def adapter5(self, ld, args, st, r):
        """A5: A4's adapter with the switch a5typed (A5_GO / A5_GO_LEAF / A5_HOLDS / A5_SP16) and, for every relocated site
        the typed function reaches, the test that the jal / j word in RAM still encodes the static target (A5_JAL)."""
        if not self.callees and not self.backs:
            go = 'A5_GO_LEAF()'
        else:
            go = 'A5_GO()' + ''.join(f' && A5_HOLDS(0x{c:X}u, lift_d1::{nm})' for c, nm in sorted(self.holds)) + \
                ''.join(f' && A2_SLOT(0x{c:X}u, lift_d1::{nm})' for c, nm in getattr(self, 'guards', []))
        go += ''.join(f' && A5_JAL(0x{site:X}u, 0x{t:X}u)' for site, t in sorted(self.relocs_all))
        if self.has128:
            go += ' && A5_SP16()'
        vl, vw, va = [], [], ''
        for q, isout in self.vio:
            nm, ty, cx = vu_local(q)
            if q[0] == 'h':
                vl.append(f'uint64_t a4_{nm} = a4::hi64(ctx->r[{q[1]}]);')
                if isout:
                    vw.append(f'ctx->r[{q[1]}] = a4::mk128(a4_{nm}, Ps2GprU64(ctx, {q[1]}));')
            else:
                vl.append(f'{ty} a4_{nm} = {cx};')
                if isout:
                    vw.append(f'{cx} = a4_{nm};')
            va += f', a4_{nm}'
        call = f'typed::{self.name}(rdram, runtime{args}{va}{", a3_xo" if self.xout else ""})'
        xd, xw = '', ''
        if self.xout:
            xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(xo_rd(k, q) for k, q in self.xout)}}}; '
            for i, (k, q) in enumerate(self.xout):
                xw += xo_wr(k, q, i)
        leaf = go.startswith('A5_GO_LEAF()')
        return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A5_CHK({self.name}{args}); {" ".join(vl)}{" " if vl else ""}{xd}'
                f'{"" if leaf else f"A5_IN({self.idx}); "}{r}{call}; {"" if leaf else "A5_OUT(); "}{st}{xw}{" ".join(vw)}{" " if vw else ""}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK5}')

    def adapter6(self, ld, args, st, r):
        """A6: A5's adapter with the switch a6typed (A6_GO for every function, leaves too: the typed path only on the game
        thread's runtime above the NOCHK floor, never on a worker shell), each reached slot tested against what holds it
        (A6_HOLDS: the lifted function, the recompiled function of a never-lifted one, the hand native, the runtime stub's
        native), and a break's cop0 effects applied after the call (A6_BRKS)."""
        go = 'A6_GO()' + ''.join(f' && A6_HOLDS(0x{c:X}u, {guard_sym(c, nm)})' for c, nm in sorted(self.holds)) + \
            ''.join(f' && A2_SLOT(0x{c:X}u, lift_d1::{nm})' for c, nm in getattr(self, 'guards', []))
        go += ''.join(f' && A5_JAL(0x{site:X}u, 0x{t:X}u)' for site, t in sorted(self.relocs_all))
        if self.has128:
            go += ' && A5_SP16()'
        vl, vw, va = [], [], ''
        for q, isout in self.vio:
            nm, ty, cx = vu_local(q)
            if q[0] == 'h':
                vl.append(f'uint64_t a4_{nm} = a4::hi64(ctx->r[{q[1]}]);')
                if isout:
                    vw.append(f'ctx->r[{q[1]}] = a4::mk128(a4_{nm}, Ps2GprU64(ctx, {q[1]}));')
            else:
                vl.append(f'{ty} a4_{nm} = {cx};')
                if isout:
                    vw.append(f'{cx} = a4_{nm};')
            va += f', a4_{nm}'
        call = f'typed::{self.name}(rdram, runtime{args}{va}{", a3_xo" if self.xout else ""})'
        xd, xw = '', ''
        if self.xout:
            xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(xo_rd(k, q) for k, q in self.xout)}}}; '
            for i, (k, q) in enumerate(self.xout):
                xw += xo_wr(k, q, i)
        brk = 'A6_BRKS(); ' if self.sig.brk else ''
        return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A6_CHK({self.name}{args}); {" ".join(vl)}{" " if vl else ""}{xd}'
                f'A6_IN({self.idx}); {r}{call}; A6_OUT(); {st}{xw}{" ".join(vw)}{" " if vw else ""}{brk}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK6}')

    def adapter7(self, ld, args, st, r):
        """A7: A6's adapter with the switch a7typed (A7_GO / A7_HOLDS / A7_IN / A7_OUT / A7_CHK); a typed callee's extra outputs
        a lifted caller reads come back through the adapter's xo as A3's do."""
        go = 'A7_GO()' + ''.join((f' && A7_HOLDSX(0x{c:X}u, {guard_sym(c, nm[:-2])})' if nm.endswith('@x') else f' && A7_HOLDS(0x{c:X}u, {guard_sym(c, nm)})') for c, nm in sorted(self.holds))
        go += ''.join(f' && A5_JAL(0x{site:X}u, 0x{t:X}u)' for site, t in sorted(self.relocs_all))
        if self.has128:
            go += ' && A5_SP16()'
        vl, vw, va = [], [], ''
        for q, isout in self.vio:
            nm, ty, cx = vu_local(q)
            if q[0] == 'h':
                vl.append(f'uint64_t a4_{nm} = a4::hi64(ctx->r[{q[1]}]);')
                if isout:
                    vw.append(f'ctx->r[{q[1]}] = a4::mk128(a4_{nm}, Ps2GprU64(ctx, {q[1]}));')
            else:
                vl.append(f'{ty} a4_{nm} = {cx};')
                if isout:
                    vw.append(f'{cx} = a4_{nm};')
            va += f', a4_{nm}'
        call = f'typed::{self.name}(rdram, runtime{args}{va}{", a3_xo" if self.xout else ""})'
        xd, xw = '', ''
        if self.xout:
            xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(xo_rd(k, q) for k, q in self.xout)}}}; '
            for i, (k, q) in enumerate(self.xout):
                xw += xo_wr(k, q, i)
        brk = 'A6_BRKS(); ' if self.sig.brk else ''
        return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A7_CHK({self.name}{args}); {" ".join(vl)}{" " if vl else ""}{xd}'
                f'A7_IN({self.idx}); {r}{call}; A7_OUT(); {st}{xw}{" ".join(vw)}{" " if vw else ""}{brk}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK7}')

    def adapter8(self, ld, args, st, r):
        """A8: A7's adapter with the switch a8typed (A8_GO / A8_HOLDS / A8_IN / A8_OUT / A8_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A8_GO()').replace('A7_HOLDS(', 'A8_HOLDS(').replace(
            'A7_CHK(', 'A8_CHK(').replace('A7_IN(', 'A8_IN(').replace('A7_OUT()', 'A8_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK8

    def adapter9(self, ld, args, st, r):
        """A9: A8's adapter with the switch a9typed (A9_GO / A9_HOLDS / A9_IN / A9_OUT / A9_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A9_GO()').replace('A7_HOLDS(', 'A9_HOLDS(').replace(
            'A7_CHK(', 'A9_CHK(').replace('A7_IN(', 'A9_IN(').replace('A7_OUT()', 'A9_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK9

    def adapter10(self, ld, args, st, r):
        """A10: A9's adapter with the switch a10typed (A10_GO / A10_HOLDS / A10_IN / A10_OUT / A10_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A10_GO()').replace('A7_HOLDS(', 'A10_HOLDS(').replace('A7_HOLDSX(', 'A10_HOLDSX(').replace(
            'A7_CHK(', 'A10_CHK(').replace('A7_IN(', 'A10_IN(').replace('A7_OUT()', 'A10_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK10

    def adapter14(self, ld, args, st, r):
        """A14: A13's adapter with the switch a14typed (A14_GO / A14_HOLDS / A14_IN / A14_OUT / A14_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A14_GO()').replace('A7_HOLDS(', 'A14_HOLDS(').replace('A7_HOLDSX(', 'A14_HOLDSX(').replace(
            'A7_CHK(', 'A14_CHK(').replace('A7_IN(', 'A14_IN(').replace('A7_OUT()', 'A14_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK14

    def adapter13(self, ld, args, st, r):
        """A13: A12's adapter with the switch a13typed (A13_GO / A13_HOLDS / A13_IN / A13_OUT / A13_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A13_GO()').replace('A7_HOLDS(', 'A13_HOLDS(').replace('A7_HOLDSX(', 'A13_HOLDSX(').replace(
            'A7_CHK(', 'A13_CHK(').replace('A7_IN(', 'A13_IN(').replace('A7_OUT()', 'A13_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK13

    def adapter12(self, ld, args, st, r):
        """A12: A11's adapter with the switch a12typed (A12_GO / A12_HOLDS / A12_IN / A12_OUT / A12_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A12_GO()').replace('A7_HOLDS(', 'A12_HOLDS(').replace('A7_HOLDSX(', 'A12_HOLDSX(').replace(
            'A7_CHK(', 'A12_CHK(').replace('A7_IN(', 'A12_IN(').replace('A7_OUT()', 'A12_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK12

    def adapter11(self, ld, args, st, r):
        """A11: A10's adapter with the switch a11typed (A11_GO / A11_HOLDS / A11_IN / A11_OUT / A11_CHK)."""
        return self.adapter7(ld, args, st, r).replace('A7_GO()', 'A11_GO()').replace('A7_HOLDS(', 'A11_HOLDS(').replace('A7_HOLDSX(', 'A11_HOLDSX(').replace(
            'A7_CHK(', 'A11_CHK(').replace('A7_IN(', 'A11_IN(').replace('A7_OUT()', 'A11_OUT()')[:-len(ADAPT_MARK7)] + ADAPT_MARK11

    # the guest-signature adapter, one line at the top of the lifted function's body
    def adapter(self):
        ld, av = [], []
        for r, ty, n in self.gparams:
            ld.append(f'const {ty} a2_{r} = Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r});')
            av.append(f'a2_{r}')
        for r, n in self.fparams:
            ld.append(f'const float a2_f{r} = ctx->f[{r}];')
            av.append(f'a2_f{r}')
        for r, n in getattr(self, 'xparams', ()):  # A13 (XIN13)
            ld.append(f'const uint64_t a2_{n} = ctx->{r};')
            av.append(f'a2_{n}')
        args = ''.join(', ' + x for x in av)
        call = f'typed::{self.name}(rdram, runtime{args}{", a3_xo" if self.xout else ""})'
        rt = self.rtype
        st = ''
        if rt == 'a1::RetVF':
            st = 'Ps2SetGpr64(ctx, 2, a2_r.v0); ctx->f[0] = a2_r.f0; '
        elif rt == 'float':
            st = 'ctx->f[0] = a2_r; '
        elif rt == 'uint32_t':
            st = 'Ps2SetGpr64(ctx, 2, sx64(a2_r)); '
        elif rt == 'uint64_t':
            st = 'Ps2SetGpr64(ctx, 2, a2_r); '
        r = '' if rt == 'void' else f'const {rt} a2_r = '
        if getattr(self, 'a14', False):
            return self.adapter14(ld, args, st, r)
        if self.a13:
            return self.adapter13(ld, args, st, r)
        if self.a12:
            return self.adapter12(ld, args, st, r)
        if self.a11:
            return self.adapter11(ld, args, st, r)
        if self.a10:
            return self.adapter10(ld, args, st, r)
        if self.a9:
            return self.adapter9(ld, args, st, r)
        if self.a8:
            return self.adapter8(ld, args, st, r)
        if self.a7:
            return self.adapter7(ld, args, st, r)
        if self.vu and self.a6:
            return self.adapter6(ld, args, st, r)
        if self.vu and self.a5:
            return self.adapter5(ld, args, st, r)
        if self.vu:
            # A4: as A3's adapter, with the VU0 interface: each slot loaded from the context into a local the typed function
            # gets by reference, the outputs stored back (a GPR's upper half beside the low half the context keeps)
            if not self.callees and not self.backs:
                go = 'A4_GO_LEAF()'
            else:
                go = 'A4_GO()' + ''.join(f' && A4_HOLDS(0x{c:X}u, lift_d1::{nm})' for c, nm in sorted(self.holds)) + \
                    ''.join(f' && A2_SLOT(0x{c:X}u, lift_d1::{nm})' for c, nm in getattr(self, 'guards', []))
            if self.has128:
                go += ' && A4_SP16()'
            vl, vw, va = [], [], ''
            for q, isout in self.vio:
                nm, ty, cx = vu_local(q)
                if q[0] == 'h':
                    vl.append(f'uint64_t a4_{nm} = a4::hi64(ctx->r[{q[1]}]);')
                    if isout:
                        vw.append(f'ctx->r[{q[1]}] = a4::mk128(a4_{nm}, Ps2GprU64(ctx, {q[1]}));')
                else:
                    vl.append(f'{ty} a4_{nm} = {cx};')
                    if isout:
                        vw.append(f'{cx} = a4_{nm};')
                va += f', a4_{nm}'
            call = f'typed::{self.name}(rdram, runtime{args}{va}{", a3_xo" if self.xout else ""})'
            xd, xw = '', ''
            if self.xout:
                xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(xo_rd(k, q) for k, q in self.xout)}}}; '
                for i, (k, q) in enumerate(self.xout):
                    xw += xo_wr(k, q, i)
            leaf = go.startswith('A4_GO_LEAF()')
            return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A4_CHK({self.name}{args}); {" ".join(vl)}{" " if vl else ""}{xd}'
                    f'{"" if leaf else f"A4_IN({self.idx}); "}{r}{call}; {"" if leaf else "A4_OUT(); "}{st}{xw}{" ".join(vw)}{" " if vw else ""}'
                    f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK4}')
        if self.loops:
            # A3: a function with a loop (or calls) needs n5sched in effect and the NOCHK fast path (A3_GO: the host
            # stack above m_noCheckFloor, which is UINTPTR_MAX unless NOCHK is on): the deferred checkpoints are then
            # taken at the next checkpoint after the call; a loop-free leaf needs only the switch (A3_GO_LEAF)
            if not self.callees and not self.backs:
                go = 'A3_GO_LEAF()'
            else:
                go = 'A3_GO()' + ''.join(f' && A3_HOLDS(0x{c:X}u, lift_d1::{nm})' for c, nm in sorted(self.holds)) + \
                    ''.join(f' && A2_SLOT(0x{c:X}u, lift_d1::{nm})' for c, nm in getattr(self, 'guards', []))
            xd, xw = '', ''
            if self.xout:
                xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(xo_rd(k, q) for k, q in self.xout)}}}; '
                for i, (k, q) in enumerate(self.xout):
                    xw += xo_wr(k, q, i)
            return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A3_CHK({self.name}{args}); {xd}{"" if go == "A3_GO_LEAF()" else f"A3_IN({self.idx}); "}{r}{call}; {"" if go == "A3_GO_LEAF()" else "A3_OUT(); "}{st}{xw}'
                    f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK3}')
        if not self.callees:
            go = 'A2_GO_LEAF()'
        else:
            go = 'A2_GO()' + ''.join(f' && A2_HOLDS(0x{c:X}u, lift_d1::{nm})' for c, nm in sorted(self.holds)) + \
                ''.join(f' && A2_SLOT(0x{c:X}u, lift_d1::{nm})' for c, nm in getattr(self, 'guards', []))
        return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A2_CHK({self.name}{args}); {r}{call}; {st}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK}')


# ----------------------------------------------------------------------------------------------- liveness at call sites
def live_typed_caller(f, csig, sigs, targets):
    """Backward liveness over a typed caller (its calls all typed): live at its return only what it returns; a call
    reads its callee's parameters; tail sites are the caller's own effects (not checked). {site: (callee, live after)}"""
    nodes, order = f['nodes'], f['order']
    ret_live = set(RET_SLOTS[csig.rtype])

    def g(s):
        return {('g', r) for r in s}
    live = {x: set() for x in order}
    changed = True
    while changed:
        changed = False
        for x in reversed(order):
            kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq = nodes[x]
            out = set()
            for y in succ:
                out |= live.get(y, set())
            uses, duses = g(gu) | set(ou), g(dgu) | set(dou)
            if kk == 'op':
                li = uses | (out - defs)
            elif kk == 'br':
                if info[1]:
                    lt = live.get(info[0], set())
                    lnt = set()
                    for y in succ:
                        if y != info[0]:
                            lnt |= live.get(y, set())
                    li = uses | duses | (lt - ddefs) | lnt
                else:
                    li = uses | duses | (out - ddefs)
            elif kk == 'jal':
                s = sigs[info]
                li = duses | ((s.inputs() | (out - RET_SLOTS[s.rtype] - {('g', 31)})) - ddefs)
            elif kk == 'tail':
                s = sigs[info]
                li = duses | ((s.inputs() | (ret_live - RET_SLOTS[s.rtype])) - ddefs)
            elif kk == 'ret':
                li = duses | (ret_live - ddefs)
            else:
                li = uses | duses | (out - ddefs)
            li.discard(('g', 0))
            li.discard(('g', 31))
            li.discard(('g', 29))
            if li != live[x]:
                live[x] = li
                changed = True
    res = {}
    for x in order:
        kk, info = nodes[x][0], nodes[x][1]
        if kk == 'jal' and info in targets:
            after = set()
            for y in nodes[x][2]:
                after |= live.get(y, set())
            res[x] = (info, after)
        elif kk == 'tail' and info in targets:
            # the callee's scratch slots beyond what the caller returns: the caller's own (in its scratch set)
            res[x] = (info, set())
    return res


def direct_targets(texts):
    out = set()
    for t in texts.values():
        for m in re.finditer(r'\{(0x[0-9A-Fa-f]+)u, &lift_d1::\w+\},', t):
            out.add(int(m.group(1), 16))
    return out


# ----------------------------------------------------------------------------------------------- text / undo
def undo_text(t):
    t = t.replace(INC_LINE14, '').replace(INC_LINE13, '').replace(INC_LINE12, '').replace(INC_LINE11, '').replace(INC_LINE10, '').replace(INC_LINE9, '').replace(INC_LINE8, '').replace(INC_LINE7, '').replace(INC_LINE6, '').replace(INC_LINE5, '').replace(INC_LINE4, '').replace(INC_LINE3, '').replace(INC_LINE, '')
    return '\n'.join(line for line in t.split('\n') if not ((line.endswith(ADAPT_MARK) and 'A2_GO' in line) or
                                                           (line.endswith(ADAPT_MARK3) and 'A3_GO' in line) or
                                                           (line.endswith(ADAPT_MARK4) and 'A4_GO' in line) or
                                                           (line.endswith(ADAPT_MARK5) and 'A5_GO' in line) or
                                                           (line.endswith(ADAPT_MARK6) and 'A6_GO' in line) or
                                                           (line.endswith(ADAPT_MARK7) and 'A7_GO' in line) or
                                                           (line.endswith(ADAPT_MARK8) and 'A8_GO' in line) or
                                                           (line.endswith(ADAPT_MARK9) and 'A9_GO' in line) or
                                                           (line.endswith(ADAPT_MARK10) and 'A10_GO' in line) or
                                                           (line.endswith(ADAPT_MARK11) and 'A11_GO' in line) or
                                                           (line.endswith(ADAPT_MARK12) and 'A12_GO' in line) or
                                                           (line.endswith(ADAPT_MARK13) and 'A13_GO' in line) or
                                                           (line.endswith(ADAPT_MARK14) and 'A14_GO' in line)))


HDR = '''// typed_a2.h: round A2's typed functions (tools/typed_lift2.py; docs/decomp_a2.md). Generated: do not edit.
// Real C++ functions with frames (host arrays, no guest sp) and calls (C++ calls to typed callees), written from
// lift_d1's IR; the A1 functions they call are in typed_a1.h.
#pragma once
#include "typed_a1.h"
#include "typed_a2_rt.h"

namespace typed
{
#include "typed_a2.inc"
}
'''

REF_MACROS = '''// the reference legs' calls: dispatched to the callees' lifted texts on the same logged state (no clock, no table)
#undef PS2_CALL_GUEST_DIRECT_NOCHK
#undef PS2_CALL_GUEST_NOCHK
#undef PS2_CALL_GUEST_DIRECT
#undef PS2_CALL_GUEST_KNOWN
#undef PS2_TAIL_GUEST_DIRECT
#undef PS2_TAIL_GUEST_KNOWN
#define PS2_CALL_GUEST_DIRECT_NOCHK(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_NOCHK(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_DIRECT(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_CALL_GUEST_KNOWN(t, f, r, s, k, n) a2ck::refCall((t), (r), rdram, ctx, runtime)
#define PS2_TAIL_GUEST_DIRECT(t, f) (void)a2ck::refCall((t), 0u, rdram, ctx, runtime)
#define PS2_TAIL_GUEST_KNOWN(t, f) (void)a2ck::refCall((t), 0u, rdram, ctx, runtime)
'''
REF_JUMP = '''// A5: a relocated tail jump (PS2_JUMP_GUEST(jumpTarget)) dispatched as a tail call; a jump table's out-of-list value
// reaches refCall as an unknown target (counted), the typed leg's A5_BADJUMP gives the check up
#undef PS2_JUMP_GUEST
#define PS2_JUMP_GUEST(t) (void)a5ck::refCall((t), 0u, rdram, ctx, runtime)
'''
CALL_FORMS = re.compile(r'PS2_(?:CALL_GUEST_DIRECT_NOCHK|CALL_GUEST_NOCHK|CALL_GUEST_DIRECT|CALL_GUEST_KNOWN|TAIL_GUEST_DIRECT|TAIL_GUEST_KNOWN)\(')


REEMIT_WHY = {}
BACK_RE = re.compile(r'^(\s*)if \(__builtin_expect\(!runtime->m_eeClock\.charge\(32u\), 0\)\) \{.*\}$')


def ref_body(name, addr, body, a5=False):
    """The lifted text (NOCHK forms, the resume guard out) for the reference; None when it holds something the check TU
    cannot compile or run without the game (a recompiled function's name, the runtime)."""
    lines = body.rstrip('\n').split('\n')
    if lines and N4.GUARD.match(lines[0]):
        lines = lines[1:]
    # A3: a back edge's checkpoint: the reference runs as the typed function does, without the test (nothing else runs
    # during the check); A3_REF_BACK only counts it (the offline harness caps a leg's iterations on random states)
    lines = [BACK_RE.sub(r'\1A3_REF_BACK();', ln) for ln in lines]
    text = '\n'.join(lines)
    probe = text
    while True:
        m = CALL_FORMS.search(probe)
        if not m:
            break
        args, j = L.split_args(probe, m.end())
        probe = probe[:m.start()] + 'CALL' + probe[j:]
    if re.search(r'\bruntime->|lift_d1::(?!flt\b|bits\b|sx64\b)\w+|\b\w+_0x[0-9a-f]+\b|PS2_GUEST_FN|PS2_CHECKPOINT', probe):
        return None
    if not a5 and 'PS2_JUMP_GUEST' in probe:
        return None
    return (f'// {name} (0x{addr:x}): lift_d1\'s text (the reference)\n'
            f'void {name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n' + text + '\n}\n')


def write_check(emits, a1sigs, defs_n4, path_tu, path_ref, tag='a2', prev=(), prev_incs=(), natives6=None):
    """The check TU (tag a2: round A2's; a3: round A3's, whose references may call A2's: prev holds A2's emits and
    prev_incs the extra ref / typed includes)."""
    ns = tag + 'ck'
    TAG = tag.upper()
    ref = []
    for E in emits:
        if not getattr(E, 'variant', False):
            ref.append(E.ref)
    with open(path_ref, 'w') as fo:
        fo.write(f"// typed_{tag}_ref.inc: round {TAG}'s references for the check (tools/typed_lift2.py --check-tu). Generated.\n")
        fo.write('\n'.join(ref))
    if tag == 'a2':
        head = ["// a2_typed_check.cpp: round A2's differential check of the typed functions (tools/typed_lift2.py --check-tu;",
                '// docs/decomp_a2.md). Generated: do not edit. Compiled to code only with PS2X_A2_CHECK=1 (a check build) or',
                '// A2_OFFLINE (the offline harness, tools/a2_harness.cpp).']
    else:
        head = [f"// {tag}_typed_check.cpp: round {TAG}'s differential check of the typed functions (tools/typed_lift2.py --check-tu;",
                f'// docs/decomp_{tag}.md). Generated: do not edit. Compiled to code only with PS2X_{TAG}_CHECK=1 (a check build) or',
                f'// {TAG}_OFFLINE (the offline harness, tools/{tag}_harness.cpp).']
    if tag in ('a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
        # A7: both legs' float operations at run time, in the game thread's rounding (toward zero): no compile-time folding in
        # the default rounding (GCC: the TU's functions with -frounding-math; the offline harness is built with it)
        head = head + ['#if defined(__GNUC__) && !defined(__clang__)', '#pragma GCC optimize("rounding-math")', '#endif']
    if tag in ('a10', 'a11', 'a12', 'a13', 'a14'):
        # A10: the camera note in this TU's typed legs is captured, not made (typed_a10_check_rt.h camCap: A10_NCHK compares it)
        head = head + [f'#if !defined({TAG}_OFFLINE)', f'#define A10_CAMNOTE(v) ::{tag}ck::camCap(v)', '#endif']
    if tag == 'a14':
        head = head + ['#define A14_CHECK_TU 1']
    out = head + [f'#include "typed_{tag}_check_rt.h"',
                  f'#if PS2X_{TAG}_CHECK || defined({TAG}_OFFLINE)',
                  f'namespace {ns}',
                  '{',
                  'bool refCall(uint32_t t, uint32_t ret, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);',
                  '}',
                  REF_MACROS.replace('a2ck::', ns + '::') + (REF_JUMP.replace('a5ck::', ns + '::') if tag in ('a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14') else ''),
                  f'namespace {ns}',
                  '{',
                  'namespace ref',
                  '{',
                  'using lift_d1::sx64;',
                  '#include "typed_a1_ref.inc"'] + [f'#include "typed_{p}_ref.inc"' for p in prev_incs] + [
                  f'#include "typed_{tag}_ref.inc"',
                  '}',
                  'namespace ty',
                  '{',
                  '#include "typed_a1.inc"'] + [f'#include "typed_{p}.inc"' for p in prev_incs] + [
                  f'#include "typed_{tag}.inc"',
                  '}',
                  '']
    out.append('bool refCall(uint32_t t, uint32_t ret, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)')
    out.append('{')
    out.append('    ctx->pc = t;')
    if tag in ('a10', 'a11', 'a12', 'a13', 'a14'):
        # A10: the reference legs recurse through here; on random states (offline) a drawn cycle of pointers would not end
        out.append(f'#if defined({TAG}_OFFLINE)')
        out.append('    legBack(); // each reference call counts against the leg cap as a back edge does (a binary recursion on a drawn tree)')
        out.append('    static int depth = 0; // and the depth is bounded (a drawn cycle of pointers: the host stack)')
        out.append('    if (depth > 2000) throw Capped{};')
        out.append('    struct Depth { Depth() { ++depth; } ~Depth() { --depth; } } depth_;')
        out.append('#endif')
    out.append('    switch (t)')
    out.append('    {')
    for a, s in sorted(a1sigs.items()):
        out.append(f'    case 0x{a:x}u: ref::{s.name}(rdram, ctx, runtime); break;')
    for E in list(prev) + list(emits):
        if getattr(E, 'variant', False):
            continue
        out.append(f'    case 0x{E.addr:x}u: ref::{E.name}(rdram, ctx, runtime); break;')
    if tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
        for x in A6_EXT + (A7_EXT if tag in ('a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14') else []) + (A8_EXT if tag in ('a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14') else []) + (A9_EXT if tag in ('a9', 'a10', 'a11', 'a12', 'a13', 'a14') else []) + (A10_EXT if tag in ('a10', 'a11', 'a12', 'a13', 'a14') else []) + (A11_EXT if tag in ('a11', 'a12', 'a13', 'a14') else []):  # the typed runtime stubs: the reference's call runs the stub's core on the leg's memory (refExt)
            out.append(f'    case 0x{x[0]:x}u: refExt(0x{x[0]:x}u, rdram, ctx, runtime); break;')
    out.append('    default: ckUnknownCall(t); return true;')
    out.append('    }')
    out.append('    (void)ret;')
    out.append('    return true;')
    out.append('}')
    for i, E in enumerate(emits):
        ps = ''.join(f', {ty} p{r}' for r, ty, _ in E.gparams) + ''.join(f', float pf{r}' for r, _ in E.fparams) + ''.join(f', uint64_t px{n}' for r, n in getattr(E, 'xparams', ()))
        av = ''.join(f', p{r}' for r, _, _ in E.gparams) + ''.join(f', pf{r}' for r, _ in E.fparams) + ''.join(f', px{n}' for r, n in getattr(E, 'xparams', ()))
        kind = {'void': 0, 'uint32_t': 1, 'uint64_t': 2, 'float': 3, 'a1::RetVF': 4}[E.rtype]
        dg = sum(1 << r for k, r in E.dropped if k == 'g')
        df = sum(1 << r for k, r in E.dropped if k == 'f')
        out.append(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime{ps})')
        out.append('{')
        out.append(f'    if (!ckBegin({i}, rdram, runtime)) return;')
        if tag == 'a2':
            out.append('    R5900Context &c = a1ck::ckCtx(ctx);')
            out.append(f'    ref::{E.name}(rdram, &c, runtime);')
            out.append('    a1ck::ckMid();')
        else:  # A3: a leg that runs past the iteration cap (offline, random states) or the log is given up (ckLeg false)
            out.append('    R5900Context &c = ckCtx(ctx);')
            sv = sorted((off, size) for (r, off, size) in getattr(E, 'save_at', {}).values()) if (getattr(E, 'gf', False) or tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14')) else []
            if sv:  # A5: a guest frame's save slots (a reference store into one other than its save gives the check up)
                out.append('    static const int32_t kSv[][2] = {' + ', '.join(f'{{{o}, {z}}}' for o, z in sv) + '};')
                out.append(f'    ckSaves(kSv, {len(sv)}u);')
            if tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
                out.append('    brkReset();')
            out.append(f'    if (!ckLeg([&] {{ ref::{getattr(E, "refname", E.name)}(rdram, &c, runtime); }})) return;')
            if sv:
                out.append('    if (ckSavesClobbered()) return;')
            out.append('    ckMid();')
        if tag == 'a2':
            if E.rtype == 'void':
                out.append(f'    ty::{E.name}(rdram, runtime{av});')
                out.append(f'    ckEnd({i}, ctx, c, {kind}, 0u, 0.0f, 0x{dg:x}u, 0x{df:x}u);')
            elif E.rtype == 'a1::RetVF':
                out.append(f'    const a1::RetVF r = ty::{E.name}(rdram, runtime{av});')
                out.append(f'    ckEnd({i}, ctx, c, {kind}, r.v0, r.f0, 0x{dg:x}u, 0x{df:x}u);')
            elif E.rtype == 'float':
                out.append(f'    const float r = ty::{E.name}(rdram, runtime{av});')
                out.append(f'    ckEnd({i}, ctx, c, {kind}, 0u, r, 0x{dg:x}u, 0x{df:x}u);')
            else:
                out.append(f'    const uint64_t r = {"a1_sx64" if E.rtype == "uint32_t" else ""}(ty::{E.name}(rdram, runtime{av}));')
                out.append(f'    ckEnd({i}, ctx, c, {kind}, r, 0.0f, 0x{dg:x}u, 0x{df:x}u);')
        else:
            out.append('    uint64_t v0 = 0u;')
            out.append('    float f0 = 0.0f;')
            xa = ''
            vio = getattr(E, 'vio', ())
            for q, _ in vio:  # A4: the VU0 interface from the input context
                nm, ty, cx = vu_local(q)
                out.append(f'    uint64_t v_{nm} = a4::hi64(ctx->r[{q[1]}]);' if q[0] == 'h' else f'    {ty} v_{nm} = {cx};')
            av = av + ''.join(f', v_{vu_local(q)[0]}' for q, _ in vio)
            if E.xout:
                out.append(f'    uint64_t xo[{len(E.xout)}] = {{{", ".join(xo_rd(k, q, ck=True) for k, q in E.xout)}}};')
                xa = ', xo'
            if E.rtype == 'void':
                body = f'ty::{E.name}(rdram, runtime{av}{xa});'
            elif E.rtype == 'a1::RetVF':
                body = f'const a1::RetVF r = ty::{E.name}(rdram, runtime{av}{xa}); v0 = r.v0; f0 = r.f0;'
            elif E.rtype == 'float':
                body = f'f0 = ty::{E.name}(rdram, runtime{av}{xa});'
            else:
                body = f'v0 = {"a1_sx64" if E.rtype == "uint32_t" else ""}(ty::{E.name}(rdram, runtime{av}{xa}));'
            out.append(f'    if (!ckLeg([&] {{ {body} }})) return;')
            xc = ['brkDiffer()'] if tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14') else []
            for j, (k, q) in enumerate(E.xout):
                xc.append(xo_ck(k, q, j))
            vb = ''
            if tag in ('a4', 'a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
                # the typed leg's VU0 state: the input context with the VU0 outputs written in, against the reference's
                out.append('    R5900Context &t = ckTyCtx(ctx);')
                for q, isout in vio:
                    if not isout:
                        continue
                    nm, ty, cx = vu_local(q)
                    out.append(f'    t.r[{q[1]}] = a4::mk128(v_{nm}, Ps2GprU64(&t, {q[1]}));' if q[0] == 'h' else f'    {cx.replace("ctx->", "t.")} = v_{nm};')
                vb = ', vuDiffer(t, c)' if tag == 'a4' else f', vuDifferMask(t, c, 0x{sum(1 << q[1] for q, _ in vio if q[0] == "h"):X}u)'
            out.append(f'    ckEnd({i}, ctx, c, {kind}, v0, f0, 0x{dg:x}u, 0x{df:x}u, {" || ".join(xc) if xc else "false"}{vb});')
        out.append('}')
    out.append(f'const a1ck::CkEntry kEntries[{max(1, len(emits))}] = {{')
    for E in emits:
        av = ''.join(f', ({ty})Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r})' for r, ty, _ in E.gparams) + \
            ''.join(f', ctx->f[{r}]' for r, _ in E.fparams) + ''.join(f', ctx->{r}' for r, _ in getattr(E, 'xparams', ()))
        out.append(f'    {{"{E.name}", 0x{E.addr:x}u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) {{ {E.name}(rdram, ctx, runtime{av}); }}}},')
    if not emits:
        out.append('    {"none", 0u, nullptr},')
    out.append('};')
    out.append(f'const uint32_t kEntryCount = {len(emits)}u;')
    if tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
        # A6_NCHK: a hand native typed from its MIPS, its typed function run on the native's entry state (leg 1 of the overlay)
        out.append('bool nativeLeg(uint32_t addr, uint8_t *rdram, const R5900Context *ctx, PS2Runtime *runtime, uint64_t &v0, float &f0, int &kind)')
        out.append('{')
        out.append('    switch (addr)')
        out.append('    {')
        for E in emits:
            if not natives6 or E.addr not in natives6 or getattr(E, 'variant', False):
                continue
            kind = {'void': 0, 'uint32_t': 1, 'uint64_t': 2, 'float': 3, 'a1::RetVF': 4}[E.rtype]
            av = ''.join(f', ({ty})Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r})' for r, ty, _ in E.gparams) + \
                ''.join(f', ctx->f[{r}]' for r, _ in E.fparams) + ''.join(f', ctx->{r}' for r, _ in getattr(E, 'xparams', ()))
            pre = []
            for q, _ in getattr(E, 'vio', ()):
                nm, ty, cx = vu_local(q)
                pre.append(f'uint64_t v_{nm} = a4::hi64(ctx->r[{q[1]}]);' if q[0] == 'h' else f'{ty} v_{nm} = {cx};')
            av += ''.join(f', v_{vu_local(q)[0]}' for q, _ in getattr(E, 'vio', ()))
            if E.rtype == 'void':
                body = f'ty::{E.name}(rdram, runtime{av});'
            elif E.rtype == 'a1::RetVF':
                body = f'const a1::RetVF r = ty::{E.name}(rdram, runtime{av}); v0 = r.v0; f0 = r.f0;'
            elif E.rtype == 'float':
                body = f'f0 = ty::{E.name}(rdram, runtime{av});'
            else:
                body = f'v0 = {"a1_sx64" if E.rtype == "uint32_t" else ""}(ty::{E.name}(rdram, runtime{av}));'
            out.append(f'    case 0x{E.addr:x}u: {{ {" ".join(pre)} kind = {kind}; return ckLeg([&] {{ {body} }}); }}')
        out.append('    default: return false;')
        out.append('    }')
        out.append('}')
    if tag in ('a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):
        # the relocated sites' words (the static target each A5_JAL tests): the offline harness writes them into its RAM
        rw = sorted({(site, w) for E in emits for site, w in getattr(E, 'reloc_words', {}).items()})
        if tag in ('a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13', 'a14'):  # A6: every relocated site a function reaches through its typed callees too (A5_JAL tests them all)
            allw = {}
            for E in list(prev) + list(emits):
                allw.update(getattr(E, 'reloc_words', {}))
            rw = sorted(set(rw) | {(site, allw[site]) for E in emits for site, _ in getattr(E, 'relocs_all', set()) if site in allw})
        out.append(f'const uint32_t kRelocs[{max(1, len(rw))}][2] = {{' + ', '.join(f'{{0x{a:X}u, 0x{w:08X}u}}' for a, w in rw) + ('{0u, 0u}' if not rw else '') + '};')
        out.append(f'const uint32_t kRelocCount = {len(rw)}u;')
    out.append('}')
    out.append(f'#if !defined({TAG}_OFFLINE)')
    out.append(f'void {ns}Report(unsigned long long frame) {{ {ns}::ckReport(frame); }}')
    out.append('#endif')
    out.append('#endif')
    with open(path_tu, 'w') as fo:
        fo.write('\n'.join(out) + '\n')
    with open(os.path.join(os.path.dirname(path_tu), f'typed_{tag}_check.inc'), 'w') as fo:
        fo.write(f"// typed_{tag}_check.inc: round {TAG}'s check entries (tools/typed_lift2.py --check-tu). Generated.\n")
        for E in emits:
            ps = ''.join(f', {ty}' for r, ty, _ in E.gparams) + ''.join(', float' for r, _ in E.fparams) + ''.join(', uint64_t' for r, _ in getattr(E, 'xparams', ()))
            fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime{ps});\n')


HDR3 = '''// typed_a3.h: round A3's typed functions (tools/typed_lift2.py, stage 3; docs/decomp_a3.md). Generated: do not edit.
// Real C++ functions with loops (goto-based, locals, no context: a back edge charges the EE clock and the checkpoint is
// deferred to the first one after the call, A3_BACK), frames and calls; the A1 / A2 functions they call are in
// typed_a1.h / typed_a2.h.
#pragma once
#include "typed_a2.h"
#include "typed_a3_rt.h"

namespace typed
{
#include "typed_a3.inc"
}
'''


HDR4 = '''// typed_a4.h: round A4's typed functions (tools/typed_lift2.py, stage 4; docs/decomp_a4.md). Generated: do not edit.
// Real C++ functions that use VU0 / 128-bit values: the VU0 registers, ACC, Q, status and the GPRs' upper halves a
// function reads or writes are reference parameters (its VU0 interface, the VU0 ABI), vec4 / q128 locals, the VU0
// macro operations and MMI as named helpers (typed_a4_rt.h); no R5900 context, no VU0 emulation state.
#pragma once
#include "typed_a3.h"
#include "typed_a4_rt.h"

namespace typed
{
#include "typed_a4.inc"
}
'''


HDR5 = '''// typed_a5.h: round A5's typed functions (tools/typed_lift2.py, stage 5; docs/decomp_a5.md). Generated: do not edit.
// What A1-A4 refused for their form and A5 types: functions proven against their callees' typed interfaces (the typed
// ABI proof), relocated calls (the adapter tests the jal word in RAM, A5_JAL), frames whose address escapes kept in
// guest RAM below the guest sp (sp a local guest address, no context), 32-bit inputs passed whole to a 64-bit
// parameter, jump tables as switch statements; and every caller that then had all its callees typed.
#pragma once
#include "typed_a4.h"
#include "typed_a5_rt.h"

namespace typed
{
#include "typed_a5.inc"
}
'''


HDR6 = '''// typed_a6.h: round A6's typed functions (tools/typed_lift2.py, stage 6; docs/decomp_a6.md). Generated: do not edit.
// What A1-A5 left behind a callee they could not type and A6 types: never-lifted functions and hand natives typed from
// their MIPS (no adapter: a typed caller calls them while the slot holds the recompiled function / the native, A6_HOLDS),
// calls to the runtime's libc / thread-id stubs as typed calls to the stubs' own cores (typed_a6_ext.h), `break` (the
// cop0 effects applied by the adapter after the call), and every caller that then had all its callees typed.
#pragma once
#include "typed_a5.h"
#include "typed_a6_rt.h"
#include "typed_a6_ext.h"

namespace typed
{
#include "typed_a6.inc"
}
'''


HDR7 = '''// typed_a7.h: round A7's typed functions (tools/typed_lift2.py, stage 7; docs/decomp_a7.md). Generated: do not edit.
// What A1-A6 left behind a callee they could not type and A7 types: callers that read a callee's scratch register after the
// call take it back from the callee's extra outputs (a typed out-parameter, written where the callee changes it; earlier
// functions get a variant <name>_x7 typed with them), the printf-family stubs as typed calls to their cores (typed_a7_ext.h),
// more hand natives typed from their MIPS, and every caller that then had all its callees typed.
#pragma once
#include "typed_a6.h"
#include "typed_a7_rt.h"
#include "typed_a7_ext.h"

namespace typed
{
#include "typed_a7.inc"
}
'''


HDR8 = '''// typed_a8.h: round A8's typed functions (tools/typed_lift2.py, stage 8; docs/decomp_a8.md). Generated: do not edit.
// What A1-A7 left behind a callee they could not type and A8 types: the varargs functions whose argument-register spills
// (their va area in a guest frame) store the registers' entry values, more hand natives typed from their MIPS
// (docs/a8_natives.txt), and every caller that then had all its callees typed (variants <name>_x8 of earlier functions as A7's).
#pragma once
#include "typed_a7.h"
#include "typed_a8_rt.h"

namespace typed
{
#include "typed_a8.inc"
}
'''


HDR9 = '''// typed_a9.h: round A9's typed functions (tools/typed_lift2.py, stage 9; docs/decomp_a9.md). Generated: do not edit.
// What A1-A8 left behind a callee they could not type and A9 types: the quat122 / geo126 hand natives (the quaternion
// helpers, iosGetSinCosf, the arc cosine / tangent, GetMatrixFromQuatPos) as typed calls to the natives' own host-float
// cores (typed_a9_ext.h: the same code the native runs on its default path), and every caller that then had all its callees
// typed (variants <name>_x9 of earlier functions as A7's).
#pragma once
#include "typed_a8.h"
#include "typed_a9_rt.h"
#include "typed_a9_ext.h"

namespace typed
{
#include "typed_a9.inc"
}
'''


ADAPT9_HEAD = '''// typed_a9_adapters.cpp: round A9's adapters for the never-lifted functions stage 9 typed (tools/typed_lift2.py;
// docs/decomp_a9.md). Generated: do not edit. Each takes the recompiled function's slot at install (a9InstallAdapters, from
// sotc_decomp::install after A8's, only while the slot still holds the recompiled function): the typed function while the
// switch a9typed is on and every slot it reaches holds what its typed callee stands for (A9_GO / A9_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a9.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a9ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


HDR10 = '''// typed_a10.h: round A10's typed functions (tools/typed_lift2.py, stage 10; docs/decomp_a10.md). Generated: do not edit.
// What A1-A9 left behind a callee they could not type and A10 types: recursive functions (a function that calls itself, or a
// cycle of functions calling each other, typed against its own interface to a fixpoint: each call a C++ call with its own host
// frame, or its guest frame in guest RAM below the guest sp as the MIPS keeps it), the upper-half class (a callee whose lifted
// text writes a GPR's whole 128 bits typed with that upper half where its caller's VU0 interface holds it), and every caller
// that then had all its callees typed (variants <name>_x10 of earlier functions as A7's).
#pragma once
#include "typed_a9.h"
#include "typed_a10_rt.h"

namespace typed
{
#include "typed_a10.inc"
}
'''


HDR11 = '''// typed_a11.h: round A11's typed functions (tools/typed_lift2.py, stage 11; docs/decomp_a11.md). Generated: do not edit.
// What A1-A10 left behind a callee they could not type and A11 types: the matrix stack's users (iosPushCurrentMatrix /
// iosPopCurrentMatrix as typed calls on VU0's data memory, vi15 a slot of the VU0 interface: typed_a11_ext.h), recursion over
// it (stage 10's fixpoint), and every caller that then had all its callees typed (variants <name>_x11 of earlier functions).
#pragma once
#include "typed_a10.h"
#include "typed_a11_rt.h"
#include "typed_a11_ext.h"

namespace typed
{
#include "typed_a11.inc"
}
'''


HDR12 = '''// typed_a12.h: round A12's typed functions (tools/typed_lift2.py, stage 12; docs/decomp_a12.md). Generated: do not edit.
// What A1-A11 left behind a callee they could not type and A12 types: the motion subsystem's natives typed from their MIPS
// (docs/a12_natives.txt: a typed caller calls the typed function while the slot holds the native and A12_HOLDS says the native's
// path is the proven one), the spin rule's base taint flow-sensitive (a list walk), a likely branch's restore on the taken edge, a
// callee-written register passed to a 64-bit parameter taken back as an extra output; every caller that then had all its callees
// typed (variants <name>_x12 of earlier functions).
#pragma once
#include "typed_a11.h"
#include "typed_a12_rt.h"

namespace typed
{
#include "typed_a12.inc"
}
'''


HDR13 = '''// typed_a13.h: round A13's typed functions (tools/typed_lift2.py, stage 13; docs/decomp_a13.md). Generated: do not edit.
// What A1-A12 left behind a callee whose hi / lo its callers read after the call: hi / lo taken back as extra outputs (HL13: the
// typed callee returns its hi / lo locals in the xo array, a typed caller takes them into its own, the adapter writes them to the
// context); every caller that then had all its callees typed (variants <name>_x13 of earlier functions).
#pragma once
#include "typed_a12.h"
#include "typed_a13_rt.h"

namespace typed
{
#include "typed_a13.inc"
}
'''


HDR14 = '''// typed_a14.h: round A14's typed functions (tools/typed_lift2.py, stage 14; docs/decomp_a14.md). Generated: do not edit.
// What A1-A13 left refused for runtime state lift_d1 did not name: the second multiply pipeline's hi1 / lo1, the shift-amount
// register, COP0 Status (EIntr / DIntr), VU0's clip flags and R register as slots of the VU0 interface (RT14); a likely branch's
// break slot recorded for the adapter (BRK14); every caller that then had all its callees typed (variants <name>_x14).
#pragma once
#include "typed_a13.h"
#include "typed_a14_rt.h"

namespace typed
{
#include "typed_a14.inc"
}
'''


ADAPT14_HEAD = '''// typed_a14_adapters.cpp: round A14's adapters for the never-lifted functions stage 14 typed (tools/typed_lift2.py;
// docs/decomp_a14.md). Generated: do not edit. Each takes the recompiled function's slot at install (a14InstallAdapters, from
// sotc_decomp::install after A13's, only while the slot still holds the recompiled function): the typed function while the
// switch a14typed is on and every slot it reaches holds what its typed callee stands for (A14_GO / A14_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a14.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a14ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT13_HEAD = '''// typed_a13_adapters.cpp: round A13's adapters for the never-lifted functions stage 13 typed (tools/typed_lift2.py;
// docs/decomp_a13.md). Generated: do not edit. Each takes the recompiled function's slot at install (a13InstallAdapters, from
// sotc_decomp::install after A12's, only while the slot still holds the recompiled function): the typed function while the
// switch a13typed is on and every slot it reaches holds what its typed callee stands for (A13_GO / A13_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a13.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a13ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT12_HEAD = '''// typed_a12_adapters.cpp: round A12's adapters for the never-lifted functions stage 12 typed (tools/typed_lift2.py;
// docs/decomp_a12.md). Generated: do not edit. Each takes the recompiled function's slot at install (a12InstallAdapters, from
// sotc_decomp::install after A11's, only while the slot still holds the recompiled function): the typed function while the
// switch a12typed is on and every slot it reaches holds what its typed callee stands for (A12_GO / A12_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a12.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a12ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT11_HEAD = '''// typed_a11_adapters.cpp: round A11's adapters for the never-lifted functions stage 11 typed (tools/typed_lift2.py;
// docs/decomp_a11.md). Generated: do not edit. Each takes the recompiled function's slot at install (a11InstallAdapters, from
// sotc_decomp::install after A10's, only while the slot still holds the recompiled function): the typed function while the
// switch a11typed is on and every slot it reaches holds what its typed callee stands for (A11_GO / A11_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a11.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a11ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT10_HEAD = '''// typed_a10_adapters.cpp: round A10's adapters for the never-lifted functions stage 10 typed (tools/typed_lift2.py;
// docs/decomp_a10.md). Generated: do not edit. Each takes the recompiled function's slot at install (a10InstallAdapters, from
// sotc_decomp::install after A9's, only while the slot still holds the recompiled function): the typed function while the
// switch a10typed is on and every slot it reaches holds what its typed callee stands for (A10_GO / A10_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a10.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a10ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT8_HEAD = '''// typed_a8_adapters.cpp: round A8's adapters for the never-lifted functions stage 8 typed (tools/typed_lift2.py;
// docs/decomp_a8.md). Generated: do not edit. Each takes the recompiled function's slot at install (a8InstallAdapters, from
// sotc_decomp::install after A7's, only while the slot still holds the recompiled function): the typed function while the
// switch a8typed is on and every slot it reaches holds what its typed callee stands for (A8_GO / A8_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a8.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a8ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT7_HEAD = '''// typed_a7_adapters.cpp: round A7's adapters for the never-lifted functions stage 7 typed (tools/typed_lift2.py;
// docs/decomp_a7.md). Generated: do not edit. Each takes the recompiled function's slot at install (a7InstallAdapters, from
// sotc_decomp::install after A6's, only while the slot still holds the recompiled function): the typed function while the
// switch a7typed is on and every slot it reaches holds what its typed callee stands for (A7_GO / A7_HOLDS), else the
// recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a7.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a7ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


ADAPT6_HEAD = '''// typed_a6_adapters.cpp: round A6's adapters for the never-lifted functions stage 6 typed (tools/typed_lift2.py;
// docs/decomp_a6.md). Generated: do not edit. Each takes the recompiled function's slot at install (a6InstallAdapters, from
// sotc_decomp::install, only while the slot still holds the recompiled function): the typed function while the switch a6typed
// is on and every slot it reaches holds what its typed callee stands for (A6_GO / A6_HOLDS, as the lifted functions'
// adapters), else the recompiled function as before.
#include "sotc_decomp.h"
#include "typed_a6.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>

namespace a6ad
{
inline uint64_t sx64(uint32_t v) { return a1_sx64(v); } // the adapters' v0 (as lift_d1::sx64 in the lifted batches)
}

'''


def tier5(E):
    t = ('loop ' if E.backs else '') + ('gframe ' if E.gf else 'frame ' if E.acc else '')
    t += 'vu0 ' if any(q[0] != 'h' for q, _ in E.vio) else '128 ' if (E.vio or E.has128) else ''
    return t + ('depth %d' % E.depth if E.callees else 'leaf')


def run_stage(cand, sigs, depth, loops, F, defs, byaddr, abi, cfin, dtargets, a1sigs, refdefs, max_depth, vu=False, a5=False):
    """One stage of the typed lifter: depth by depth while functions convert (a function is tried once all its callees
    are typed). sigs / depth are extended in place. Returns emitted {addr: Emit}, tried_fail {addr: why}."""
    emitted = {}
    tried_fail = {}
    level = 0
    while level < max_depth:
        level += 1
        added = 0
        for a in cand:
            if a in emitted:
                continue
            f = F[a]
            calls = [n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail')]
            if any(c not in sigs for c in calls):
                continue
            if a in tried_fail and tried_fail[a] != 'untyped callee':
                continue
            # a callee that is not an N3 direct target (a native or a hook may hold its slot): only an A1 leaf whose
            # every call here is an N4 site (the slot compared with its lifted function, the native entry called
            # with the checkpoint charged, not tested, while n4native is on); the adapter then tests the same (guards)
            guards, gbad = [], None
            body = defs[byaddr[a]][1]
            for c in sorted(set(calls)):
                if c in dtargets:
                    continue
                if loops and not sigs[c].guards:
                    # A3: any callee; the adapter tests that its slot holds its lifted function (A3_HOLDS), so the typed
                    # callee is what the lifted call would run; a plain (tested) lifted call's checkpoint is deferred as
                    # a back edge's is (the clock charged the same)
                    continue
                if c not in a1sigs or sigs[c].guards:
                    gbad = 'callee not a direct target'
                    break
                nm = a1sigs[c].name
                pat = 'PS2_N4_FAST(PS2_GUEST_FN(0x%Xu), lift_d1::%s)' % (c, nm)
                sites = sum(1 for ln in body.split('\n') if ln.endswith(N4.SITE_MARK) and pat in ln)
                njal = sum(1 for n in f['nodes'].values() if n[0] == 'jal' and n[1] == c)
                ntail = sum(1 for n in f['nodes'].values() if n[0] == 'tail' and n[1] == c)
                if ntail or sites != njal:
                    gbad = 'callee not a direct target'
                    break
                guards.append((c, nm))
            if any(c in sigs and sigs[c].guards for c in calls):
                gbad = 'callee needs a slot test'
            if gbad:
                tried_fail[a] = gbad
                continue
            E = Emit(a, byaddr[a], abi.get(a), sigs, cfin, f, loops=loops, vu=vu, a5=a5)
            if E.ok:
                E.guards = guards
                E.sig.guards = guards
                gset = {c for c, _ in guards}
                holds = set()
                for c in set(calls):
                    if c not in gset:
                        holds.add((c, sigs[c].name))
                    holds |= sigs[c].holds
                if any(nm in N3.LIBC for _, nm in holds):
                    E.ok, E.why = False, 'callee named like a libc function'
                E.holds = E.sig.holds = holds
            if E.ok:
                E.ref = ref_body(E.name, a, refdefs[E.name], a5=a5)
                if E.ref is None:
                    E.ok, E.why = False, 'reference text form'
            if not E.ok:
                tried_fail[a] = E.why
                continue
            E.depth = 1 + max([depth[c] for c in calls] + [-1])
            depth[a] = E.depth
            emitted[a] = E
            sigs[a] = E.sig
            if a5:  # A5: its proven interface for the caller checks (the lifted callers' liveness at other sites)
                abi[a] = (True, set(E.abi5[1]) | ({29} if E.needs_sp else set()), set(E.abi5[2]))
            added += 1
        if not added:
            break
    return emitted, tried_fail


# ----------------------------------------------------------------------------------------------- stage 6 (round A6)
# The typed runtime stubs (game/overrides/typed_a6_ext.h, namespace a6x): the calls outside the IR that have an exact typed
# form. Each calls the stub's own core (the same code, no context); the slot must hold the stub (or the native that holds
# it) for a typed caller to call it (A6_HOLDS). Fields: address, the table's name, the typed name, its parameters
# (('g', register, C type)), return type, v0 width, scratch slots, VU0 interface (GPR upper halves: memset / memcpy / strcpy
# copy the whole 128-bit $a0 into $v0, setReturnS32 / U32 clear $v0's upper half), the symbol the slot holds.
A6_EXT = [
    (0x1283D0, 'memset', 'a6x::memset', [('g', 4, 'uint64_t'), ('g', 5, 'uint32_t'), ('g', 6, 'uint32_t')], 'uint64_t', 64,
     {('h', 4)}, {('h', 2)}, {('h', 2)}, '::memset_0x1283d0'),
    (0x12821C, 'memcpy', 'a6x::memcpy', [('g', 4, 'uint64_t'), ('g', 5, 'uint32_t'), ('g', 6, 'uint32_t')], 'uint64_t', 64,
     {('h', 4)}, {('h', 2)}, {('h', 2)}, '::memcpy_0x12821c'),
    (0x128FB0, 'strcpy', 'a6x::strcpy', [('g', 4, 'uint64_t'), ('g', 5, 'uint32_t')], 'uint64_t', 64,
     {('h', 4)}, {('h', 2)}, {('h', 2)}, '::strcpy_0x128fb0'),
    # strcmp's slot holds decomp_leaves::strCmp (a byte loop while the switch leaves is on, $v0's upper half kept; else the stub)
    (0x128E6C, 'strcmp', 'a6x::strcmp', [('g', 4, 'uint32_t'), ('g', 5, 'uint32_t')], 'uint32_t', 32, {('h', 2)}, {('h', 2)}, set(),
     '::decomp_leaves::strCmp'),
    (0x128188, 'memcmp', 'a6x::memcmp', [('g', 4, 'uint32_t'), ('g', 5, 'uint32_t'), ('g', 6, 'uint32_t')], 'uint32_t', 32,
     set(), {('h', 2)}, {('h', 2)}, '::memcmp_0x128188'),
    (0x1290C4, 'strlen', 'a6x::strlen', [('g', 4, 'uint32_t')], 'uint32_t', 32, set(), {('h', 2)}, {('h', 2)}, '::strlen_0x1290c4'),
    # GetThreadId's slot holds round 139's native (decomp_sema.cpp: the scheduler's fast path, else the stub)
    (0x106ED0, 'GetThreadId', 'a6x::getThreadId', [], 'uint32_t', 32, set(), {('h', 2)}, {('h', 2)}, '::decomp_sema::getThreadIdNative'),
]


def a6_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in A6_EXT:
        s = Sig(nm, a, rt, params, set(), v0w, 'a6x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        out[a] = s
        GUARD_SYM[a] = sym
    return out


# A7: the printf-family stubs (game/overrides/typed_a7_ext.h, namespace a7x), A6_EXT's fields; stack readers (their varargs
# past the eight argument registers come from the guest stack above sp) are listed in A7_STK.
_A7_ARGS8 = [('g', r, 'uint32_t') for r in range(4, 12)]
A7_EXT = [
    # printf: the eight argument registers' low words and the guest sp (varargs past the registers read from sp + 0x10 on,
    # as the stub's cursor does); v0 through setReturnS32 (the upper half cleared)
    (0x1285C0, 'printf', 'a7x::printf', _A7_ARGS8 + [('g', 29, 'uint32_t')], 'uint32_t', 32, set(), {('h', 2)}, {('h', 2)}, '::printf_0x1285c0'),
    # iosJumpRecoverPoint: the recompiler stubbed it as ps2_stubs::printf
    (0x1B3410, 'iosJumpRecoverPoint', 'a7x::iosJumpRecoverPoint', _A7_ARGS8 + [('g', 29, 'uint32_t')], 'uint32_t', 32, set(), {('h', 2)}, {('h', 2)},
     '::iosJumpRecoverPoint_0x1b3410'),
    # vsprintf(str, format, va_list): the va_list in guest memory
    (0x12C048, 'vsprintf', 'a7x::vsprintf', [('g', 4, 'uint32_t'), ('g', 5, 'uint32_t'), ('g', 6, 'uint32_t')], 'uint32_t', 32, set(), {('h', 2)},
     {('h', 2)}, '::vsprintf_0x12c048'),
    # sceWrite(fd, buf, size): ps2_syscalls::fioWrite's core (the VFS write)
    (0x10B678, 'sceWrite', 'a7x::sceWrite', [('g', 4, 'uint32_t'), ('g', 5, 'uint32_t'), ('g', 6, 'uint32_t')], 'uint32_t', 32, set(), {('h', 2)},
     {('h', 2)}, '::sceWrite_0x10b678'),
]
A7_STK = {0x1285C0, 0x1B3410}


def a7_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in A7_EXT:
        s = Sig(nm, a, rt, params, set(), v0w, 'a7x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        s.stk = a in A7_STK
        out[a] = s
        GUARD_SYM[a] = sym
    return out


# A8: hand natives whose default path is not their MIPS step by step, as typed calls to the native's own core (the same
# code the native runs: exact by construction); A6_EXT's fields (game/overrides/typed_a8_ext.h, namespace a8x)
A8_EXT = []


def a8_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in A8_EXT:
        s = Sig(nm, a, rt, params, set(), v0w, 'a8x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        out[a] = s
        GUARD_SYM[a] = sym
    return out



# A9: the quat122 / geo126 hand natives as typed calls to the natives' own host-float cores (game/overrides/typed_a9_ext.h,
# namespace a9x; the cores in decomp_char122.cpp / decomp_cam126.cpp, shared with the natives: the same compiled code on the
# same values). Each native's default path (quat122 / geo126 on, their checks off: A9_GO) writes only its memory results and
# f0 / v0 as listed; every other register is left as it was (the cores' sigs drop nothing). A6_EXT's fields.
_U = 'uint32_t'
A9_EXT = [
    # SetQuaternionByAxisRotateVWithNoRegularize(out, angle, axis): the quaternion at out, f0 its w
    (0x1BBDA8, 'SetQuaternionByAxisRotateVWithNoRegularize', 'a9x::axisRotate', [('g', 4, _U), ('g', 5, _U), ('g', 6, _U)], 'float', 32,
     set(), set(), set(), '::decomp_quat::axisRotate'),
    # GetDifferencialQuaternionWithNoRegularize(out, from, to, axis): the quaternion at out
    (0x1BA4D0, 'GetDifferencialQuaternionWithNoRegularize', 'a9x::diffQuat', [('g', 4, _U), ('g', 5, _U), ('g', 6, _U), ('g', 7, _U)], 'void', 32,
     set(), set(), set(), '::decomp_quat::diffQuat'),
    # GetQuaternionByAxisDirZY(q, dir, v): the quaternion at q, f0 the turn's cosine
    (0x1BC308, 'GetQuaternionByAxisDirZY', 'a9x::axisDirZY', [('g', 4, _U), ('g', 5, _U), ('g', 6, _U)], 'float', 32,
     set(), set(), set(), '::decomp_quat::axisDirZY'),
    # iosGetSinCosf(out, x): (sin, cos, 0, 0) at out (masked to 16)
    (0x1BDDB0, 'iosGetSinCosf', 'a9x::sinCos', [('g', 4, _U), ('f', 12)], 'void', 32, set(), set(), set(), '::decomp_math::getSinCos'),
    # iosGetACosfExact(x): f0
    (0x1BE388, 'iosGetACosfExact', 'a9x::acosExact', [('f', 12)], 'float', 32, set(), set(), set(), '::decomp_libm::acosExact'),
    # iosGetATan2fExact(y, x): f0
    (0x1BE458, 'iosGetATan2fExact', 'a9x::atan2Exact', [('f', 12), ('f', 13)], 'float', 32, set(), set(), set(), '::decomp_quat::atan2Exact'),
    # GetMatrixFromQuatPos(out, qp): the 4x4 matrix at out, v0 = qp + 0x10 (sign-extended, the upper half kept)
    (0x1135720, 'GetMatrixFromQuatPos', 'a9x::matFromQuatPos', [('g', 4, _U), ('g', 5, _U)], 'uint32_t', 32, set(), set(), set(),
     '::cam126::matFromQuatPos'),
]


def a9_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in A9_EXT:
        s = Sig(nm, a, rt, params, set(), v0w, 'a9x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        out[a] = s
        GUARD_SYM[a] = sym
    return out


# A10: no new cores (the camera note is in the typed iosgluGetCameraContext itself: NOTE10)
A10_EXT = []


def a10_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in A10_EXT:
        s = Sig(nm, a, rt, params, set(), v0w, 'a10x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        out[a] = s
        GUARD_SYM[a] = sym
    return out


# A11: the matrix stack (VU0 macro mode: the current matrix in vf1-vf4, pushed into / popped from VU0's data memory at vi15,
# 16 bytes a register, vi15 & 0xFF the quadword index): iosPushCurrentMatrix / iosPopCurrentMatrix as typed calls
# (game/overrides/typed_a11_ext.h, namespace a11x) on the runtime's VU0 data memory (the worker shell's own on a worker, as the
# recompiled text's runtime->memory().getVU0Data()), vi15 a slot of the VU0 interface (('x', 'vi15'), ctx->vi[15] at the
# adapters): the same memory and the same vi15 the recompiled functions use, so typed and untyped callers interleave exactly.
# A6_EXT's fields; the slots hold the recompiled functions.
_VF14 = {('v', 1), ('v', 2), ('v', 3), ('v', 4)}
_VI15 = {('x', 'vi15')}
A11_EXT = [
    (0x1B8638, 'iosPushCurrentMatrix', 'a11x::pushMatrix', [], 'void', 32, _VF14 | _VI15, set(_VI15), set(_VI15), '::iosPushCurrentMatrix_0x1b8638'),
    (0x1B8650, 'iosPopCurrentMatrix', 'a11x::popMatrix', [], 'void', 32, set(_VI15), _VF14 | _VI15, _VF14 | _VI15, '::iosPopCurrentMatrix_0x1b8650'),
]
A11_EXT_OFF = [False]  # --no-ext11 (measuring)
MMI11_OFF = [False]  # --no-mmi11 (measuring): stage 11 without the MMI instructions' exact hi / lo uses


def a11_ext_sigs():
    out = {}
    for a, tname, nm, params, rt, v0w, vin, vout, vmust, sym in ([] if A11_EXT_OFF[0] else A11_EXT):
        s = Sig(nm, a, rt, params, set(), v0w, 'a11x')
        s.vin, s.vout, s.vmust = frozenset(vin), frozenset(vout), frozenset(vmust)
        s.vio = tuple((q, q in vout) for q in sorted(set(vin) | set(vout), key=vu_sortkey))
        s.ext = tname
        out[a] = s
        GUARD_SYM[a] = sym
    return out


def rt14_facts(F):
    """A14 (RT14): F with the functions whose facts have an opaque statement lifted again with RT14_LIFT's fields named."""
    out, n = dict(F), 0
    saved = dict(L.FIELDS)
    L.FIELDS.update(RT14_LIFT)
    try:
        for a, f in F.items():
            if any(nd[9] for nd in f['nodes'].values()):
                r = T1.ir_facts(a)
                if r[1] is not None:
                    out[a] = r[1]
                    n += 1
    finally:
        L.FIELDS.clear()
        L.FIELDS.update(saved)
    return out, n


def run_stage6(cand, sigs, depth, F, names, abi, cfin, refdefs, max_depth, nolift):
    """Stage 6: stage 5's form (loops, VU0, the typed ABI proof, relocations, guest frames, jump tables) plus `break`, over the
    lifted functions stage 5 left and the never-lifted / native-held functions in `nolift` (typed from their MIPS, no adapter:
    their typed callers call them while the slot holds what GUARD_SYM names). Depth by depth to a fixpoint."""
    emitted, tried_fail = {}, {}
    level = 0
    while level < max_depth:
        level += 1
        added = 0
        for a in cand:
            if a in emitted:
                continue
            f = F[a]
            calls = [n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail')]
            if any(c not in sigs for c in calls):
                continue
            if a in tried_fail and tried_fail[a] != 'untyped callee':
                continue
            if any(c in sigs and sigs[c].guards for c in calls):
                tried_fail[a] = 'callee needs a slot test'
                continue
            name = names[a]
            E = Emit(a, name, abi.get(a), sigs, cfin, f, loops=True, vu=True, a5=True, a6=True)
            if E.ok:
                E.guards = E.sig.guards = []
                holds = set()
                for c in set(calls):
                    holds.add((c, sigs[c].name))
                    holds |= sigs[c].holds
                if any(nm in N3.LIBC for c, nm in holds if c not in GUARD_SYM):
                    E.ok, E.why = False, 'callee named like a libc function'
                E.holds = E.sig.holds = holds
            if E.ok:
                body = refdefs.get(name)
                if body is not None:
                    body = body.replace('runtime->handleBreak(rdram, ctx);', 'A6_REF_BREAK();')
                E.ref = None if body is None else ref_body(name, a, body, a5=True)
                if E.ref is None:
                    E.ok, E.why = False, 'reference text form'
            if not E.ok:
                tried_fail[a] = E.why
                continue
            E.lifted = a not in nolift
            E.depth = 1 + max([depth.get(c, 0) for c in calls] + [-1])
            depth[a] = E.depth
            emitted[a] = E
            sigs[a] = E.sig
            abi[a] = (True, set(E.abi5[1]) | ({29} if E.needs_sp else set()), set(E.abi5[2]))
            added += 1
        if not added:
            break
    return emitted, tried_fail


# ----------------------------------------------------------------------------------------------- stage 7 (round A7)
class Stage7:
    """Stage 7: stage 6's form plus the typed callees' extra outputs consumed by typed callers (Emit a7=True): a caller that
    reads a callee's scratch register after the call takes it back from the callee's extra outputs. A stage-7 function asked
    for an extra output is emitted again with it (want: from typed callers; want_req: from lifted / recompiled callers, the
    adapter writes it back as A3's); a function typed in an earlier stage gets a variant (<name>_x7: the same MIPS typed with
    the extra outputs, no adapter, called by stage-7 functions only, under the same slot test). Round by round until no new
    extra output is asked for (each round emits everything again from the accumulated requests)."""

    def __init__(self, cand, base_sigs, base_depth, F, names, abi, cfin, refs, nolift, noadapter, lifted_names, sfx='_x7', a8=False, a9=False, a10=False, rec=False, a11=False, a12=False, a13=False, a14=False):
        self.sfx, self.a8 = sfx, a8  # A8: stage 8 runs this class again (variants <name>_x8, Emit a8)
        self.a9 = a9  # A9: stage 9 likewise (variants <name>_x9, Emit a9)
        self.a11 = a11  # A11: stage 11 (Emit a11, variants <name>_x11 / <name>_u11)
        self.a12 = a12  # A12: stage 12 (Emit a12, variants <name>_x12 / <name>_u12)
        self.a13 = a13  # A13: stage 13 (Emit a13, variants <name>_x13 / <name>_u13)
        self.a14 = a14  # A14: stage 14 (Emit a14, variants <name>_x14 / <name>_u14)
        self.a10 = a10  # A10: stage 10 likewise (variants <name>_x10, Emit a10)
        self.rec = rec  # A10: recursive functions (a self call / a cycle typed against its own interface to a fixpoint)
        self.cand, self.cset = cand, set(cand)
        self.base_sigs, self.base_depth = base_sigs, base_depth
        self.F, self.names, self.abi, self.cfin, self.refs = F, names, abi, cfin, refs
        self.nolift, self.noadapter = nolift, noadapter
        self.lifted_names = lifted_names  # earlier-stage function address -> its name (for the variants)
        self.want = collections.defaultdict(set)
        self.want_req = collections.defaultdict(set)
        self.want_var = collections.defaultdict(set)
        self.bad = collections.defaultdict(set)  # slots a function / variant could not give back (callers needing them refused)
        self.changed = False
        # A10: the call graph's cycles: a self-recursive function {itself}, a cycle of several functions (unless --no-mut10)
        self.scc = rec_sccs(F, MUT10_OFF[0]) if rec else {}

    def need(self, E):
        """A caller's NeedXout: record the requests; True when the caller can be retried now with variants (overlay)."""
        retry = False
        for c, sl in E.need.items():
            sl = set(sl)
            if sl & self.bad[c]:
                continue
            if c in self.cset:
                if not sl <= self.want[c] | self.want_req[c]:
                    self.want[c] |= sl
                    self.changed = True
            else:
                v = self.variants.get(c)
                if v is not None and sl <= set(v.xout):
                    if c not in self.overlay:
                        self.overlay[c] = v.sig
                        retry = True
                elif not sl <= self.want_var[c]:
                    self.want_var[c] |= sl
                    self.changed = True
        return retry

    def emit(self, a, name, xout, sigs, extra=None):
        self.overlay = {}
        for _ in range(12):
            sg = collections.ChainMap(self.overlay, extra, sigs) if extra else collections.ChainMap(self.overlay, sigs)
            sg = self.uh_overlay(sg)
            E = Emit(a, name, self.abi.get(a), sg, self.cfin, self.F[a], loops=True, xout=xout, vu=True, a5=True, a6=True, a7=True, a8=self.a8, a9=self.a9, a10=self.a10, a11=self.a11, a12=self.a12, a13=self.a13, a14=self.a14)
            if E.ok and self.a10 and not UH10_OFF[0]:
                # A10: the upper-half class: a callee whose lifted text writes a GPR's whole 128 bits (movz / movn's 128-bit
                # move) typed without that upper half, called where this function's VU0 interface holds the half: emitted
                # again with the callee's exact variant (typed in the VU0 form, the half an output), until nothing changes
                need = self.uh_need(E, sg)
                if need and self.uh_add(need, sigs):
                    continue
                miss = frozenset()
                for c in set(E.callees):
                    if c != a:
                        miss |= self.uh_missing(c, sg)
                E.sig.uh_missing = miss
            if E.ok or E.need is None:
                E.sg = sg
                return E
            if not self.need(E):
                E.why = 'a7: a callee extra output pending: ' + ' '.join(f'{c:x}:{sorted(sl)}' for c, sl in E.need.items())[:120]
                return E
        E.why = 'a7: overlay limit'
        return E

    # ---------------------------------------------------------------- A10: the upper-half class
    def uh_ir(self, c, seen=()):
        """A10: the GPRs whose upper half the lifted text of c or of anything it calls may write with a 128-bit move (movz / movn:
        lift_d1's `ctx->r[N] = _v`, VEC_W / VEC_N), from the IR. Memoised."""
        if c in self.uh_irm:
            return self.uh_irm[c]
        if c not in self.F or c in seen:
            return frozenset()
        out = set()
        try:
            lf = L.Lifter(c, {})
            lf.build()
            for x, n in lf.node_effects().items():
                for k in ('code', 'dcode'):
                    t = n.get(k) or ''
                    out |= {int(m.group(2)) for m in VEC_W.finditer(t)} | {int(m.group(2)) for m in VEC_N.finditer(t)}
        except Exception:
            pass
        for d in {n[1] for n in self.F[c]['nodes'].values() if n[0] in ('jal', 'tail')}:
            if d != c:
                out |= self.uh_ir(d, seen + (c,))
        r = frozenset(out - {0})
        self.uh_irm[c] = r
        return r

    def uh_missing(self, c, sg):
        """A10: the upper halves typed callee c's interface does not carry although its lifted text may write them: everything
        uh_ir finds for a function outside the VU0 form (A1-A3), what its callees miss for one in it (its own 128-bit moves are
        modelled there), what an exact variant still misses past its registers, nothing for a core / stub."""
        s_ = sg.get(c)
        if s_ is None or getattr(s_, 'ext', None):
            return frozenset()
        if getattr(s_, 'uh_missing', None) is not None:
            return s_.uh_missing
        if c in self.uh_mm:
            return self.uh_mm[c]
        if getattr(s_, 'tier', '') in ('a1', 'a2'):
            r = self.uh_ir(c)
        else:
            self.uh_mm[c] = frozenset()  # a cycle: its members' own halves are found on the way
            r = frozenset()
            if c in self.F:
                for d in {n[1] for n in self.F[c]['nodes'].values() if n[0] in ('jal', 'tail')}:
                    if d != c:
                        r |= self.uh_missing(d, self.base_sigs)
        self.uh_mm[c] = r
        return r

    def uh_need(self, E, sg):
        """A10: {callee: registers} whose exact variant E needs (the callee's unmodelled upper halves within E's interface)."""
        hs = {q[1] for q, _ in E.vio if q[0] == 'h'} | set(self.uh_force)
        if not hs:
            return {}
        need = {}
        for c in set(E.callees):
            if c == E.addr:
                continue
            u = self.uh_missing(c, sg) & hs
            if u:
                need[c] = u
        return need

    def uh_add(self, need, sigs):
        """A10: the exact variants asked for (callee, registers); True when one is new (the caller is emitted again)."""
        new = False
        for c, rs in need.items():
            req = frozenset(rs) | self.uh_req.get(c, frozenset())
            if c in self.uh_bad or (c in self.uh_var and self.uh_req.get(c) == req):
                continue
            self.uh_req[c] = req
            orig = sigs[c]
            oname = getattr(orig, 'orig', orig.name)
            calls = [n[1] for n in self.F[c]['nodes'].values() if n[0] in ('jal', 'tail')] if c in self.F else []
            if c not in self.F or any(cc not in sigs and cc != c for cc in calls):
                self.uh_bad.add(c)
                self.uh_why[c] = 'not in the IR / a callee untyped'
                continue
            save, force = self.overlay, self.uh_force
            self.uh_force = req
            E = self.emit(c, oname + ('_u14' if self.a14 else '_u13' if self.a13 else '_u12' if self.a12 else '_u11' if self.a11 else '_u10'), tuple(getattr(orig, 'xout', ())), sigs)
            self.overlay, self.uh_force = save, force
            if not E.ok or (getattr(E.sig, 'uh_missing', frozenset()) & req) or not ({('h', r) for r in req} <= set(E.vout)):
                self.uh_bad.add(c)
                self.uh_why[c] = E.why if not E.ok else 'the upper halves not outputs of the VU0 form'
                continue
            E.lifted = False
            E.holds = E.sig.holds = self.holds_of(E, E.sg, calls)
            E.guards = E.sig.guards = []
            E.sig.orig = oname
            E.sig.strict = True  # its callers' slot test: the slot holds exactly what it was typed from (no a6Is alternative: a native
            #                     taken for the lifted function was proven against the form without the upper half)
            E.depth = self.base_depth.get(c, 0)
            E.variant = True
            self.uh_var[c] = E
            new = True
        return new

    def uh_overlay(self, sg):
        """A10: the exact variants made so far in front of the signatures."""
        if not getattr(self, 'uh_var', None):
            return sg
        return collections.ChainMap({c: E.sig for c, E in self.uh_var.items()}, sg)

    # ---------------------------------------------------------------- A10: recursion
    def prov_sig(self, a, name, xo):
        """A10: a recursive function's provisional interface (the fixpoint's start): no parameters, both return slots, nothing
        dropped, no VU0 state; its extra outputs the ones it is emitted with."""
        s_ = Sig(name, a, 'a1::RetVF', [], set(), 32, 'a10')
        s_.xout = tuple(sorted(xo, key=lambda q: (q[0], q[1])))
        s_.xmust = frozenset()
        s_.stk = False
        s_.xfin = frozenset()
        s_.relocs = set()
        return s_

    @staticmethod
    def sig_key(s_):
        return (tuple(s_.params), s_.rtype, frozenset(s_.dropped), s_.v0w, frozenset(s_.vin), frozenset(s_.vout), frozenset(s_.vmust),
                tuple(s_.vio), tuple(getattr(s_, 'xout', ())), frozenset(getattr(s_, 'xmust', ())), bool(getattr(s_, 'stk', False)),
                frozenset(getattr(s_, 'xfin', ())), bool(getattr(s_, 'brk', False)), frozenset(getattr(s_, 'relocs', set())))

    def emit_rec(self, members, sigs):
        """A10: a recursive function (members = {itself}) or a cycle of functions calling each other, typed together against
        their own interfaces: each member emitted with the others' (and its own) current interface, the interfaces replaced by
        what the emission gives, until they agree (a fixpoint: the typed text then calls functions whose interfaces are exactly
        the ones written). ({addr: Emit}, None), or (None, why)."""
        xos = {a: (self.want[a] | self.want_req[a]) - self.bad[a] for a in members}
        prov = {a: self.prov_sig(a, self.names[a], xos[a]) for a in members}
        for it in range(10):
            Es = {}
            for a in sorted(members):
                E = self.emit(a, self.names[a], xos[a], sigs, extra=prov)
                if not E.ok and E.need is None and (self.want[a] - self.bad[a]) - self.want_req[a]:
                    self.bad[a] |= (self.want[a] - self.want_req[a])
                    self.changed = True
                    return None, 'recursion: a typed caller\'s extra output cannot be met (retried next round)'
                if not E.ok:
                    return None, E.why
                Es[a] = E
            new = {a: Es[a].sig for a in members}
            if all(self.sig_key(new[a]) == self.sig_key(prov[a]) for a in members):
                self.rec_iters[min(members)] = it + 1
                return Es, None
            prov = {}
            for a in members:
                s_ = Sig(self.names[a], a, new[a].rtype, list(new[a].params), set(new[a].dropped), new[a].v0w, 'a10')
                for k in ('xout', 'xmust', 'stk', 'xfin', 'relocs', 'brk', 'vin', 'vout', 'vmust', 'vio', 'uh_missing'):
                    if hasattr(new[a], k):
                        setattr(s_, k, getattr(new[a], k))
                prov[a] = s_
        return None, 'recursion: no fixpoint'

    def holds_of(self, E, sg, calls):
        holds = set()
        for c in set(calls):
            holds.add((c, getattr(sg[c], 'orig', sg[c].name) + ('@x' if getattr(sg[c], 'strict', False) else '')))
            holds |= sg[c].holds
        return holds

    def variant_rec(self, c, name, want, sigs):
        """A13 (REC13): the variant of a self-recursive earlier-stage function (its self call made against the variant's own
        interface, A10's fixpoint: emitted against a provisional interface, the interface replaced by what the emission gives,
        until they agree). The last Emit (refused with its reason when no fixpoint)."""
        prov = {c: self.prov_sig(c, name, want)}
        E = None
        for it in range(10):
            E = self.emit(c, name, want, sigs, extra=prov)
            if not E.ok:
                return E
            if self.sig_key(E.sig) == self.sig_key(prov[c]):
                self.rec_iters[c] = it + 1
                return E
            s_ = Sig(name, c, E.sig.rtype, list(E.sig.params), set(E.sig.dropped), E.sig.v0w, 'a10')
            for k in ('xout', 'xmust', 'stk', 'xfin', 'relocs', 'brk', 'vin', 'vout', 'vmust', 'vio', 'uh_missing'):
                if hasattr(E.sig, k):
                    setattr(s_, k, getattr(E.sig, k))
            prov = {c: s_}
        E.ok, E.why, E.need = False, 'recursion: no fixpoint (variant)', None
        return E

    def build_variants(self, sigs):
        """The variants of earlier-stage functions asked for extra outputs (callees first: by depth)."""
        self.variants = {}
        order = sorted(self.want_var, key=lambda c: (self.base_depth.get(c, 0), c))
        for _ in range(3):  # a variant's own callees' variants come first (depth order); a second pass for what that missed
            for c in order:
                if c in self.variants:
                    continue
                if c not in self.lifted_names or c not in self.F:
                    self.bad[c] |= self.want_var[c]
                    continue
                f = self.F[c]
                calls = [n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail')]
                if any(cc not in sigs for cc in calls):
                    self.bad[c] |= self.want_var[c]
                    continue
                orig = sigs[c]
                if getattr(self, 'a13', False) and c in calls and not REC13_OFF[0]:
                    E = self.variant_rec(c, getattr(orig, 'orig', orig.name) + self.sfx, self.want_var[c], sigs)
                else:
                    E = self.emit(c, getattr(orig, 'orig', orig.name) + self.sfx, self.want_var[c], sigs)
                E.lifted = False
                if not E.ok and A13_DIAG[0] and getattr(self, 'a13', False):
                    print(f'[a13diag] variant {c:x} {E.name} want {sorted(map(str, self.want_var[c]))}: {E.why} need {E.need}', file=sys.stderr)
                if not E.ok:
                    if E.need is None:
                        self.bad[c] |= self.want_var[c]
                        self.vwhy[c] = E.why
                    continue
                E.holds = E.sig.holds = self.holds_of(E, E.sg, calls)
                E.guards = E.sig.guards = []
                E.sig.orig = getattr(orig, 'orig', orig.name)
                E.depth = self.base_depth.get(c, 0)
                E.variant = True
                self.variants[c] = E

    def finish(self, a, E, calls, sigs, depth, emitted, holds=None):
        """One emitted function into the round's result (A7's tail of run_round); False with E.why when refused."""
        name = self.names[a]
        E.guards = E.sig.guards = []
        if holds is None:
            holds = self.holds_of(E, E.sg, calls)
        if any(nm in N3.LIBC for c, nm in holds if c not in GUARD_SYM):
            E.ok, E.why = False, 'callee named like a libc function'
            return False
        E.holds = E.sig.holds = holds
        body = self.refs.get(name)
        if body is not None:
            body = body.replace('runtime->handleBreak(rdram, ctx);', 'A6_REF_BREAK();')
            body = ref_reloc_guards(body, E)
        E.ref = None if body is None else ref_body(name, a, body, a5=True)
        if E.ref is None:
            E.ok, E.why = False, 'reference text form'
            return False
        E.lifted = a not in self.nolift
        E.depth = 1 + max([depth.get(c, 0) for c in calls] + [-1])
        E.variant = False
        return True

    def run_round(self, max_depth):
        sigs = dict(self.base_sigs)
        depth = dict(self.base_depth)
        self.vwhy = {}
        self.rec_iters = {}
        self.uh_irm, self.uh_mm, self.uh_var, self.uh_req, self.uh_bad, self.uh_why = {}, {}, {}, {}, set(), {}
        self.uh_force = frozenset()
        self.build_variants(sigs)
        emitted, tried = {}, {}
        level = 0
        while level < max_depth:
            level += 1
            added = 0
            for a in self.cand:
                if a in emitted:
                    continue
                f = self.F[a]
                calls = [n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail')]
                own = self.scc.get(a, frozenset()) if self.rec else frozenset()
                if any(c not in sigs and c not in own for c in calls):
                    continue
                if a in tried and tried[a] != 'untyped callee':
                    continue
                if any(c in sigs and sigs[c].guards for c in calls):
                    tried[a] = 'callee needs a slot test'
                    continue
                if own:
                    # A10: a recursive function (or a cycle): typed together once every callee outside the cycle is typed
                    if not own <= self.cset:
                        tried[a] = 'recursion: a member of the cycle is not a candidate'
                        continue
                    mcalls = {b: [n[1] for n in self.F[b]['nodes'].values() if n[0] in ('jal', 'tail')] for b in own}
                    if any(any(c not in sigs and c not in own for c in mcalls[b]) for b in own):
                        continue
                    Es, why = self.emit_rec(own, sigs)
                    if Es is None:
                        for b in own:
                            tried[b] = why
                        continue
                    holds = {b: set() for b in own}
                    ch = True
                    while ch:  # the members' slot tests: their callees' and each other's, to a fixpoint
                        ch = False
                        for b in own:
                            h = set()
                            for c in set(mcalls[b]):
                                sgc = Es[c].sig if c in own else Es[b].sg[c]
                                h.add((c, getattr(sgc, 'orig', sgc.name) + ('@x' if getattr(sgc, 'strict', False) else '')))
                                h |= holds[c] if c in own else sgc.holds
                            if h != holds[b]:
                                holds[b] = h
                                ch = True
                    ok = True
                    for b in own:
                        if not self.finish(b, Es[b], [c for c in mcalls[b] if c not in own], sigs, depth, emitted, holds[b]):
                            ok = False
                            tried[b] = Es[b].why
                    if not ok:
                        for b in own:
                            tried.setdefault(b, 'recursion: a member refused')
                        continue
                    dmax = max(Es[b].depth for b in own)
                    for b in own:
                        E = Es[b]
                        E.depth = dmax
                        E.rec = len(own)
                        depth[b] = E.depth
                        emitted[b] = E
                        sigs[b] = E.sig
                        self.abi[b] = (True, set(E.abi5[1]) | ({29} if E.needs_sp else set()), set(E.abi5[2]))
                        added += 1
                    continue
                name = self.names[a]
                xo = self.want[a] | self.want_req[a]
                E = self.emit(a, name, xo - self.bad[a], sigs)
                if not E.ok and E.need is None and (self.want[a] - self.bad[a]) - self.want_req[a]:
                    # the typed callers' requests cannot be met: emitted with the lifted callers' only, those callers refused
                    self.bad[a] |= (self.want[a] - self.want_req[a])
                    self.changed = True
                    E = self.emit(a, name, (self.want_req[a]) - self.bad[a], sigs)
                if E.ok:
                    self.finish(a, E, calls, sigs, depth, emitted)
                if not E.ok:
                    tried[a] = E.why
                    continue
                depth[a] = E.depth
                emitted[a] = E
                sigs[a] = E.sig
                self.abi[a] = (True, set(E.abi5[1]) | ({29} if E.needs_sp else set()), set(E.abi5[2]))
                added += 1
            if not added:
                break
        return emitted, tried, sigs, depth


RELOC_GUARD = re.compile(r'^[ \t]*if \(__builtin_expect\(jumpTarget != 0x([0-9A-F]+)u, 0\)\) \{ if \(PS2_CALL_GUEST_\w+\(jumpTarget, \w+, 0x[0-9A-F]+u, '
                         r'0x[0-9A-F]+u, \w+, "\w+"\)\) \{ ctx->pc = 0x[0-9A-F]+u; \w+_0x[0-9a-f]+\(rdram, ctx, runtime\); \} return; \}$', re.M)


def ref_reloc_guards(body, E):
    """A7: a relocated jal's guard in the lifted text (the word in RAM giving another target: a call and the resume through the
    recompiled function) is dead in the reference: the adapter runs the typed function only while every relocated site's word
    encodes its static target (A5_JAL), so jumpTarget is that target. Removed when its target is one of E's relocated targets."""
    tg = {t for _, t in E.relocs}

    def sub(m):
        return '' if int(m.group(1), 16) in tg else m.group(0)
    return RELOC_GUARD.sub(sub, body)


def caller_check7(emitted, F, abi_path, abi, cfin, noadapter):
    """A7: the lifted / recompiled callers of the stage-7 functions with an adapter (A3's rule): a scratch slot read after the
    call must be an extra output. {function: slots asked for}. (Typed callers are proven by their own Emit: a7_consume.)"""
    nat_rows = {int(l.split()[0], 16) for l in open(abi_path) if l.rstrip().endswith(' native=1')}
    callee_inputs = {a: (v[1] if v[0] else v[1] & (T1.ARGS | {28, 29})) for a, v in abi.items() if a not in nat_rows}
    callers = collections.defaultdict(set)
    for g, f in F.items():
        if g in emitted:
            continue
        for x, n in f['nodes'].items():
            if n[0] in ('jal', 'tail') and n[1] in emitted and n[1] not in noadapter:
                callers[g].add(n[1])
    viol = collections.defaultdict(set)
    for g, tg in callers.items():
        res = T1.live_after_sites(F[g], callee_inputs, tg, cfin)
        for x, (t, after) in res.items():
            bad = (emitted[t].dropped - set(emitted[t].xout)) & after
            if bad:
                viol[t] |= bad
                if A13_DIAG[0] and any(q[0] not in ('g', 'f') for q in bad):
                    rd = sorted(f'{y:x}' for y, n in F[g]['nodes'].items() if {('x', 'hi'), ('x', 'lo'), ('x', 'fcr31')} & (set(n[7]) | set(n[8])))
                    op = sorted(f'{y:x}' for y, n in F[g]['nodes'].items() if n[0] == 'op' and n[9])
                    print(f'[a13diag] lifted {g:x} @{x:x} -> {t:x} bad {sorted(map(str, bad))}; field readers {rd[:12]}; opaque {op[:8]}', file=sys.stderr)
    return viol


def caller_check(emitted, sigs, F, abi_path, abi, cfin, reemit=None, noadapter=frozenset()):
    """The caller check, to a fixpoint: a refused function takes its typed callers with it. A callee's inputs at the
    sites: n4_abi's input set (sound with or without the full proof: a union fixpoint where an unknown callee, a jalr or
    a runtime call reads everything) unless its slot may hold a native (then the ABI's argument registers, as A1).
    Removes the refused from emitted / sigs; returns {addr: why}."""
    nat_rows = {int(l.split()[0], 16) for l in open(abi_path) if l.rstrip().endswith(' native=1')}
    # (unproven: within the argument registers, A1's assumption for a callee it does not know, narrowed by the analysis)
    callee_inputs = {a: (v[1] if v[0] else v[1] & (T1.ARGS | {28, 29})) for a, v in abi.items() if a not in nat_rows}
    callers = collections.defaultdict(set)
    for g, f in F.items():
        for x, n in f['nodes'].items():
            if n[0] in ('jal', 'tail') and n[1] in emitted:
                callers[g].add(n[1])
    live_refused = {}
    while True:
        viol = collections.defaultdict(list)
        for g, tg in callers.items():
            tg = tg & set(emitted)
            if not tg:
                continue
            if g in emitted:
                res = live_typed_caller(F[g], emitted[g].sig, sigs, tg)
            else:
                # A6: a function without an adapter (never lifted, a native) is called typed only by typed callers
                tg = tg - noadapter
                if not tg:
                    continue
                res = T1.live_after_sites(F[g], callee_inputs, tg, cfin)
            for x, (t, after) in res.items():
                if g in emitted:
                    bad = emitted[t].dropped & after
                else:  # a lifted caller reads what the adapter writes back (A3's extra outputs) too
                    bad = (emitted[t].dropped - set(emitted[t].xout)) & after
                if bad:
                    viol[t].append((g, x, sorted(bad), g in emitted))
        if not viol:
            break
        if reemit:
            # A3: a function whose violations are all at lifted callers is written again with those registers as extra
            # outputs (the adapter writes them back); refused only when that fails or a typed caller reads them
            drop = set()
            for t, vs in viol.items():
                if any(v[3] for v in vs):
                    drop.add(t)
                    continue
                want = set(emitted[t].xout) | {b for v in vs for b in v[2]}
                E2 = reemit(emitted[t], want)
                if E2 is None:
                    drop.add(t)
                else:
                    emitted[t] = E2
                    sigs[t] = E2.sig
            if not drop:
                continue
        else:
            drop = set(viol)
        # and every typed caller of a dropped one (its text calls the typed function)
        changed = True
        while changed:
            changed = False
            for a, E in emitted.items():
                if a not in drop and any(c in drop for c in E.callees):
                    drop.add(a)
                    changed = True
        for t in drop:
            if t in viol:
                live_refused[t] = 'live scratch at a caller: ' + '; '.join(f'{g:x}@{x:x} {b}{" typed" if ty else ""}' for g, x, b, ty in viol[t][:3])
            else:
                live_refused[t] = 'a typed callee refused (live scratch)'
            del emitted[t]
            del sigs[t]
    return live_refused


def why_counts(tried_fail, live_refused):
    why = collections.Counter()
    for a, w in tried_fail.items():
        if w.startswith('unknown call'):
            w = 'unknown call'
        why[w] += 1
    for a, w in live_refused.items():
        why['live scratch at a caller' if w.startswith('live') else 'callee refused (live scratch)'] += 1
    return why


def norm_why(w):
    """A refusal reason without its per-site details (for the unlock table)."""
    for pre, k in (('unknown call', 'unknown call'), ('extra output changed by a callee', 'extra output changed by a callee'),
                   ('a7: a callee extra output pending', 'a7: a callee extra output pending'), ('a callee scratch', 'a callee scratch / extra output read after the call'),
                   ('live scratch', 'live scratch at a caller'), ('field input', 'field input'), ('lifter:', 'lifter'),
                   ('vu0 field', 'vu0 field'), ('tail return', 'tail return form'), ('kind ', 'node kind')):
        if w.startswith(pre):
            return k
    return w


def unlock_table(path, F, byaddr, sigs, why_of, skipped):
    """A5: for each refusal reason, how many waiting functions (untyped only because some callee is untyped) it blocks
    transitively. roots(w) = the untyped, non-waiting functions reachable from w through untyped callees. Columns: refused
    with that reason; waiting with at least one root of that reason ('blocks'); waiting whose every root has that reason
    ('sole': all of them would be tried if the reason went away)."""
    calls = {a: sorted({n[1] for n in f['nodes'].values() if n[0] in ('jal', 'tail')}) for a, f in F.items()}
    reason = {}
    for a in F:
        if a in sigs:
            continue
        if a in why_of:
            reason[a] = norm_why(why_of[a])
        elif a in skipped:
            reason[a] = skipped[a]
        elif a not in byaddr:
            reason[a] = 'not lifted once (recompiled only / native / lifted twice)'
        elif all(c in sigs for c in calls[a]):
            reason[a] = 'not tried (?)'
    waiting = [a for a in F if a not in sigs and a not in reason]
    for a in waiting:
        for c in calls[a]:
            if c not in F and c not in sigs:
                reason.setdefault(c, 'callee outside the IR')
    roots = {a: set() for a in waiting}
    wset = set(waiting)
    changed = True
    while changed:
        changed = False
        for a in waiting:
            r = set(roots[a])
            for c in calls[a]:
                if c in sigs:
                    continue
                if c in wset:
                    r |= roots[c]
                else:
                    r.add(c)
            if r != roots[a]:
                roots[a] = r
                changed = True
    direct, blocks, sole = collections.Counter(), collections.Counter(), collections.Counter()
    for a, w in reason.items():
        if a in F or w == 'callee outside the IR':
            direct[w] += 1
    cyc = 0
    for a in waiting:
        rs = {reason.get(c, '?') for c in roots[a]}
        if not rs:
            cyc += 1
            continue
        for w in rs:
            blocks[w] += 1
        if len(rs) == 1:
            sole[next(iter(rs))] += 1
    # per refused function: how many waiting functions it blocks (the biggest single unlocks)
    per = collections.Counter()
    for a in waiting:
        for c in roots[a]:
            per[c] += 1
    with open(path, 'w') as fo:
        fo.write(f'# unlock table: {len(waiting)} waiting functions ({cyc} only on a recursion cycle); '
                 f'{sum(1 for a in F if a in sigs)} typed in the IR\n')
        fo.write('# reason | refused | waiting it blocks (some root) | waiting it alone blocks (every root)\n')
        for w in sorted(set(direct) | set(blocks), key=lambda w: (-blocks[w], -direct[w])):
            fo.write(f'{w} | {direct[w]} | {blocks[w]} | {sole[w]}\n')
        fo.write('# top single blockers: address name reason | waiting functions behind it\n')
        for c, n in per.most_common(60):
            fo.write(f'{c:08x} {byaddr.get(c, F[c]["name"] if c in F and "name" in F[c] else "?")} {reason.get(c, "?")} | {n}\n')
    import pickle
    pickle.dump({'roots': roots, 'reason': reason, 'calls': calls}, open(path + '.pkl', 'wb'))
    return roots, reason


def insert_adapters(base, tset, inc_line, after_lines):
    """Each listed function's adapter at the top of its lifted body (after the resume guard); inc_line once a file,
    after the first of after_lines found."""
    new, nadapt = {}, 0
    for f, t in base.items():
        lines = t.split('\n')
        ins = {}
        for m in N4.FUNC.finditer(t):
            if m.group(1) == m.group(4) and m.group(1) in tset:
                s = t.count('\n', 0, m.start())
                k = s + 3
                assert lines[s + 2] == '{', (f, m.group(1))
                if N4.GUARD.match(lines[k]):
                    k += 1
                ins[k] = tset[m.group(1)].adapter()
        if not ins:
            new[f] = t
            continue
        out = []
        for k, line in enumerate(lines):
            if k in ins:
                out.append(ins[k])
                nadapt += 1
            out.append(line)
        t2 = '\n'.join(out)
        anchor = next(a for a in after_lines if a in t2)
        t2 = t2.replace(anchor, anchor + inc_line, 1)
        new[f] = t2
    return new, nadapt


# ----------------------------------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='*')
    ap.add_argument('--abi', default=os.path.join(os.path.dirname(L.RUNNER), 'agent-scratch/a2/abi_all.txt'),
                    help='tools/n4_abi.py --all output (the natives\' own proofs too, marked native=1)')
    ap.add_argument('--cache', default=None)
    ap.add_argument('--jobs', type=int, default=3)
    ap.add_argument('--noloop', default=os.path.join(REPO, 'docs/n5_noloop.txt'))
    ap.add_argument('--a1', default=os.path.join(REPO, 'docs/a1_typed.txt'))
    ap.add_argument('--undo', action='store_true')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--check-tu', action='store_true')
    ap.add_argument('--skip', default=None)
    ap.add_argument('--max-depth', type=int, default=99)
    ap.add_argument('--out-list', default=os.path.join(REPO, 'docs/a2_typed.txt'))
    ap.add_argument('--out-list3', default=os.path.join(REPO, 'docs/a3_typed.txt'))
    ap.add_argument('--no-a3', action='store_true', help='stage 2 only (A2), no loops stage')
    ap.add_argument('--skip3', default=os.path.join(REPO, 'docs/a3_skip.txt'),
                    help='A3: names left out of stage 3 (first word of a line; # comments): docs/a3_skip.txt, the long deferrals')
    ap.add_argument('--out-list4', default=os.path.join(REPO, 'docs/a4_typed.txt'))
    ap.add_argument('--no-a4', action='store_true', help='no VU0 stage (A4)')
    ap.add_argument('--out-list5', default=os.path.join(REPO, 'docs/a5_typed.txt'))
    ap.add_argument('--no-a5', action='store_true', help='no A5 stage (typed ABI proof, relocated calls, guest frames, jump tables)')
    ap.add_argument('--log5', default=None)
    ap.add_argument('--no-a6', action='store_true', help='no A6 stage (never-lifted / native-held functions, typed runtime stubs, break)')
    ap.add_argument('--out-list6', default=os.path.join(REPO, 'docs/a6_typed.txt'))
    ap.add_argument('--log6', default=None)
    ap.add_argument('--no-ext6', action='store_true', help='A6 (measuring): without the typed runtime stubs')
    ap.add_argument('--no-brk6', action='store_true', help='A6 (measuring): without break')
    ap.add_argument('--no-nolift6', action='store_true', help='A6 (measuring): without the never-lifted functions (natives6 still)')
    ap.add_argument('--natives6', default=os.path.join(REPO, 'docs/a6_natives.txt'),
                    help='A6: hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a7', action='store_true', help='no A7 stage (callee extra outputs consumed by typed callers, printf-family cores, more natives)')
    ap.add_argument('--out-list7', default=os.path.join(REPO, 'docs/a7_typed.txt'))
    ap.add_argument('--log7', default=None)
    ap.add_argument('--rounds7', type=int, default=14, help='A7: rounds of extra-output requests')
    ap.add_argument('--no-xo7', action='store_true', help='A7 (measuring): no extra outputs consumed by typed callers')
    ap.add_argument('--a13-diag', action='store_true', help='A13 (measuring): name the slots in the field refusal')
    ap.add_argument('--no-ext7', action='store_true', help='A7 (measuring): without the printf-family cores')
    ap.add_argument('--no-nat7', action='store_true', help='A7 (measuring): without docs/a7_natives.txt')
    ap.add_argument('--natives7', default=os.path.join(REPO, 'docs/a7_natives.txt'),
                    help='A7: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a8', action='store_true', help='no A8 stage (varargs spills, more natives)')
    ap.add_argument('--out-list8', default=os.path.join(REPO, 'docs/a8_typed.txt'))
    ap.add_argument('--log8', default=None)
    ap.add_argument('--rounds8', type=int, default=14, help='A8: rounds of extra-output requests')
    ap.add_argument('--no-sp8', action='store_true', help='A8 (measuring): without the argument-register spills\' entry values')
    ap.add_argument('--no-spin8', action='store_true', help='A8 (measuring): spin_filter flow-insensitive as stages 3-7')
    ap.add_argument('--no-nat8', action='store_true', help='A8 (measuring): without docs/a8_natives.txt')
    ap.add_argument('--no-ext8', action='store_true', help='A8 (measuring): without the natives as cores (A8_EXT)')
    ap.add_argument('--natives8', default=os.path.join(REPO, 'docs/a8_natives.txt'),
                    help='A8: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a9', action='store_true', help='no A9 stage (the quat122 / geo126 natives as cores)')
    ap.add_argument('--out-list9', default=os.path.join(REPO, 'docs/a9_typed.txt'))
    ap.add_argument('--log9', default=None)
    ap.add_argument('--rounds9', type=int, default=14, help='A9: rounds of extra-output requests')
    ap.add_argument('--no-nat9', action='store_true', help='A9 (measuring): without docs/a9_natives.txt')
    ap.add_argument('--no-ext9', action='store_true', help='A9 (measuring): without the natives as cores (A9_EXT)')
    ap.add_argument('--ext9', default=None, help='A9 (measuring): only these A9_EXT addresses (comma-separated hex)')
    ap.add_argument('--natives9', default=os.path.join(REPO, 'docs/a9_natives.txt'),
                    help='A9: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a11', action='store_true', help='no A11 stage (the matrix stack)')
    ap.add_argument('--out-list11', default=os.path.join(REPO, 'docs/a11_typed.txt'))
    ap.add_argument('--log11', default=None)
    ap.add_argument('--rounds11', type=int, default=14, help='A11: rounds of extra-output requests')
    ap.add_argument('--no-nat11', action='store_true', help='A11 (measuring): without docs/a11_natives.txt')
    ap.add_argument('--no-ext11', action='store_true', help='A11 (measuring): without the matrix stack (A11_EXT)')
    ap.add_argument('--no-mmi11', action='store_true', help='A11 (measuring): without the MMI instructions\' exact hi / lo uses')
    ap.add_argument('--natives11', default=os.path.join(REPO, 'docs/a11_natives.txt'),
                    help='A11: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a14', action='store_true', help='no A14 stage (runtime state as VU0-interface slots, break slots)')
    ap.add_argument('--out-list14', default=os.path.join(REPO, 'docs/a14_typed.txt'))
    ap.add_argument('--log14', default=None)
    ap.add_argument('--rounds14', type=int, default=40, help='A14: rounds of extra-output requests')
    ap.add_argument('--no-rt14', action='store_true', help='A14 (measuring): without the runtime state as VU0-interface slots')
    ap.add_argument('--no-brk14', action='store_true', help='A14 (measuring): without a likely branch\'s break slot')
    ap.add_argument('--no-srch14', action='store_true', help='A14 (measuring): without the bounded-search exit rule')
    ap.add_argument('--no-csr14', action='store_true', help='A14 (measuring): without the callee-saved read-before-write refusal')
    ap.add_argument('--no-a13', action='store_true', help='no A13 stage (hi / lo as extra outputs)')
    ap.add_argument('--out-list13', default=os.path.join(REPO, 'docs/a13_typed.txt'))
    ap.add_argument('--log13', default=None)
    ap.add_argument('--rounds13', type=int, default=40, help='A13: rounds of extra-output requests')
    ap.add_argument('--no-hl13', action='store_true', help='A13 (measuring): without hi / lo as extra outputs')
    ap.add_argument('--no-rec13', action='store_true', help='A13 (measuring): without self-recursive variants')
    ap.add_argument('--no-a12', action='store_true', help='no A12 stage (the motion natives, the list walk)')
    ap.add_argument('--out-list12', default=os.path.join(REPO, 'docs/a12_typed.txt'))
    ap.add_argument('--log12', default=None)
    ap.add_argument('--rounds12', type=int, default=40, help='A12: rounds of extra-output requests (the motion group settles in 36)')
    ap.add_argument('--no-nat12', action='store_true', help='A12 (measuring): without docs/a12_natives.txt')
    ap.add_argument('--no-spin12', action='store_true', help='A12 (measuring): A8\'s spin rule (the base taint flow-insensitive)')
    ap.add_argument('--no-pt12', action='store_true', help='A12 (measuring): a callee-written register passed to a 64-bit parameter refused')
    ap.add_argument('--no-cnt12', action='store_true', help='A12 (measuring): without the counted-loop exit rule')
    ap.add_argument('--no-lik12', action='store_true', help='A12 (measuring): a likely branch\'s restore counted live on both edges')
    ap.add_argument('--natives12', default=os.path.join(REPO, 'docs/a12_natives.txt'),
                    help='A12: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--no-a10', action='store_true', help='no A10 stage (recursion, the upper-half class)')
    ap.add_argument('--out-list10', default=os.path.join(REPO, 'docs/a10_typed.txt'))
    ap.add_argument('--log10', default=None)
    ap.add_argument('--rounds10', type=int, default=14, help='A10: rounds of extra-output requests')
    ap.add_argument('--no-nat10', action='store_true', help='A10 (measuring): without docs/a10_natives.txt')
    ap.add_argument('--no-rec10', action='store_true', help='A10 (measuring): without recursion')
    ap.add_argument('--no-mut10', action='store_true', help='A10 (measuring): self recursion only (no cycles of several functions)')
    ap.add_argument('--no-uh10', action='store_true', help='A10 (measuring): without the upper-half variants (docs/a3_skip.txt upper-half lines stay out)')
    ap.add_argument('--natives10', default=os.path.join(REPO, 'docs/a10_natives.txt'),
                    help='A10: more hand natives typed from their MIPS (address, the native symbol the slot holds)')
    ap.add_argument('--ov', default=None, help='write the generated typed files here instead of game/overrides (development)')
    ap.add_argument('--n3-exclude', default=os.path.join(REPO, 'tools/n3_direct_exclude.txt'),
                    help="A4: N3's excluded targets count as direct targets here (their adapters test every typed callee's slot)")
    ap.add_argument('--log', default=None)
    ap.add_argument('--log3', default=None)
    ap.add_argument('--log4', default=None)
    ap.add_argument('--unlock', default=None, help='A5: write the unlock table (refusal reason -> waiting functions it blocks)')
    args = ap.parse_args()
    global OV
    if args.ov:
        OV = args.ov
    files = sorted(set(args.files))
    texts = {f: open(f, errors='surrogateescape').read() for f in files}
    base = {f: undo_text(t) for f, t in texts.items()}
    if args.undo:
        new = base
        stats = 'undone'
    else:
        a1sigs = load_a1_sigs(args.a1, os.path.join(OV, 'typed_a1.inc'))
        skip = set(x.strip() for x in open(args.skip)) if args.skip else set()
        noloop = set()
        for line in open(args.noloop):
            if not line.startswith('#') and line.strip():
                noloop.add(int(line.split()[0], 16))
        defs, count, where = {}, collections.Counter(), {}
        for f, t in base.items():
            for m in N4.FUNC.finditer(t):
                if m.group(1) != m.group(4):
                    continue
                count[m.group(1)] += 1
                defs[m.group(1)] = (int(m.group(2), 16), m.group(5))
                where[m.group(1)] = f
        byaddr = {a: n for n, (a, b) in defs.items() if count[n] == 1}
        abi = T1.load_abi(args.abi)
        abi5_n4 = dict(abi)
        facts = T1.load_facts(args.cache, args.jobs)
        F = facts['facts']
        natives = L.native_addresses(set(facts['funcs']))
        cfin = T1.fpr_inputs_all(F, natives)
        dtargets = direct_targets(base)
        # A4: N3's targets taken out of the direct set (tools/n3_direct_exclude.txt: GAMECORE's hooks hold their slots in game)
        # keep the A2 / A3 sets as they were: A2_HOLDS / A3_HOLDS test every typed callee's slot anyway
        if args.n3_exclude and os.path.exists(args.n3_exclude):
            for line in open(args.n3_exclude):
                line = line.split('#')[0].strip()
                if line:
                    dtargets.add(int(line, 16))
        # the reference texts (NOCHK forms) of the lifted functions
        undone = {f: N4.undo_text(T1.undo_text(t)) for f, t in base.items()}
        refdefs = {}
        for f, t in undone.items():
            for m in N4.FUNC.finditer(t):
                if m.group(1) == m.group(4):
                    refdefs[m.group(1)] = m.group(5)
        # ---- stage 2 (round A2): loop-free functions
        cand = sorted(a for a in byaddr if a in noloop and a not in a1sigs and a in F and byaddr[a] not in skip and byaddr[a] not in N3.LIBC)
        sigs = dict(a1sigs)
        depth = {a: 0 for a in a1sigs}
        emitted, tried_fail = run_stage(cand, sigs, depth, False, F, defs, byaddr, abi, cfin, dtargets, a1sigs, refdefs, args.max_depth)
        live_refused = caller_check(emitted, sigs, F, args.abi, abi, cfin)
        why = why_counts(tried_fail, live_refused)
        emits = [emitted[a] for a in sorted(emitted, key=lambda a: (emitted[a].depth, a))]
        tset = {E.name: E for E in emits}
        new, nadapt = insert_adapters(base, tset, INC_LINE, (T1.INC_LINE, T1.INC_AFTER))
        tiers = collections.Counter()
        for E in emits:
            tiers[('frame ' if E.acc else '') + ('depth %d' % E.depth if E.callees else 'leaf')] += 1
        stats = (f'{len(cand)} candidates, {len(emits)} converted {dict(sorted(tiers.items()))} (refused: {dict(why)}); {nadapt} adapters; '
                 f'{len(F)} functions in the IR')
        # ---- stage 3 (round A3): loops (A3_BACK), and every function stage 2 did not take
        emits3, why3, tiers3, cand3 = [], collections.Counter(), collections.Counter(), []
        emitted3, tried3, live3 = {}, {}, {}
        if not args.no_a3:
            for a in list(depth):
                if a not in a1sigs and a not in emitted:
                    del depth[a]
            skip3 = set()
            if args.skip3 and os.path.exists(args.skip3):
                for x in open(args.skip3):
                    x = x.split('#')[0].split()
                    if x:
                        skip3.add(x[0])
            cand3 = sorted(a for a in byaddr if a not in a1sigs and a not in emitted and a in F and byaddr[a] not in skip
                           and byaddr[a] not in skip3 and byaddr[a] not in N3.LIBC)
            emitted3, tried3 = run_stage(cand3, sigs, depth, True, F, defs, byaddr, abi, cfin, dtargets, a1sigs, refdefs, args.max_depth)
            def reemit(E, want):
                E2 = Emit(E.addr, E.name, abi.get(E.addr), sigs, cfin, F[E.addr], loops=True, xout=want)
                if not E2.ok:
                    REEMIT_WHY[E.addr] = E2.why
                    return None
                E2.guards, E2.sig.guards = E.guards, E.guards
                E2.holds = E2.sig.holds = E.holds
                E2.ref, E2.depth = E.ref, E.depth
                return E2
            live3 = caller_check(emitted3, sigs, F, args.abi, abi, cfin, reemit=reemit)
            why3 = why_counts(tried3, live3)
            emits3 = [emitted3[a] for a in sorted(emitted3, key=lambda a: (emitted3[a].depth, a))]
            for i, E in enumerate(emits3):
                E.idx = i  # the check entry's index and docs/a3_typed.txt's order (A3_IN's per-function counts)
            new, nadapt3 = insert_adapters(new, {E.name: E for E in emits3}, INC_LINE3, (INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits3:
                tiers3[('loop ' if E.backs else '') + ('frame ' if E.acc else '') + ('depth %d' % E.depth if E.callees else 'leaf')] += 1
            stats += (f'\ntyped_lift2 stage 3 (A3): {len(cand3)} candidates, {len(emits3)} converted {dict(sorted(tiers3.items()))} '
                      f'(refused: {dict(why3.most_common())}); {nadapt3} adapters')
        # ---- stage 4 (round A4): the VU0 / 128-bit functions (the VU0 ABI: vu_scan) and, depth by depth, their callers
        emits4, why4, tiers4, cand4 = [], collections.Counter(), collections.Counter(), []
        emitted4, tried4, live4 = {}, {}, {}
        if not args.no_a3 and not args.no_a4:
            cand4 = [a for a in cand3 if a not in emitted3]
            emitted4, tried4 = run_stage(cand4, sigs, depth, True, F, defs, byaddr, abi, cfin, dtargets, a1sigs, refdefs, args.max_depth, vu=True)
            # a typed A4 caller passes no extra outputs (A3's xo) and drops its callees' scratch slots: none may be read after
            # the call (A3's caller check proved it for the lifted text of these callers, which read xo through the adapter)
            changed4 = True
            while changed4:
                changed4 = False
                for a in sorted(emitted4):
                    E = emitted4[a]
                    if any(c not in sigs for c in E.callees):
                        why = 'a typed callee refused'
                    else:
                        res = live_typed_caller(F[a], E.sig, sigs, set(E.callees))
                        bad = [(x, t, sorted(sigs[t].dropped & after)) for x, (t, after) in res.items() if sigs[t].dropped & after]
                        why = ('a callee scratch / extra output read after the call: ' + '; '.join(f'{x:x} {t:x} {b}' for x, t, b in bad[:2])) if bad else None
                    if why:
                        tried4[a] = why
                        del emitted4[a]
                        del sigs[a]
                        changed4 = True
            def reemit4(E, want):
                E2 = Emit(E.addr, E.name, abi.get(E.addr), sigs, cfin, F[E.addr], loops=True, xout=want, vu=True)
                if not E2.ok:
                    REEMIT_WHY[E.addr] = E2.why
                    return None
                E2.guards, E2.sig.guards = E.guards, E.guards
                E2.holds = E2.sig.holds = E.holds
                E2.ref, E2.depth = E.ref, E.depth
                return E2
            live4 = caller_check(emitted4, sigs, F, args.abi, abi, cfin, reemit=reemit4)
            why4 = why_counts(tried4, live4)
            emits4 = [emitted4[a] for a in sorted(emitted4, key=lambda a: (emitted4[a].depth, a))]
            for i, E in enumerate(emits4):
                E.idx = i
            new, nadapt4 = insert_adapters(new, {E.name: E for E in emits4}, INC_LINE4, (INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits4:
                tiers4[('loop ' if E.backs else '') + ('frame ' if E.acc else '') + ('vu0 ' if any(q[0] != 'h' for q, _ in E.vio) else '128 ' if (E.vio or E.has128) else '')
                       + ('depth %d' % E.depth if E.callees else 'leaf')] += 1
            stats += (f'\ntyped_lift2 stage 4 (A4): {len(cand4)} candidates, {len(emits4)} converted {dict(sorted(tiers4.items()))} '
                      f'(refused: {dict(why4.most_common())}); {nadapt4} adapters')
        # ---- stage 5 (round A5): the typed ABI proof, relocated calls (A5_JAL), guest frames, 64-bit inputs, jump tables
        emits5, why5, tiers5, cand5 = [], collections.Counter(), collections.Counter(), []
        emitted5, tried5, live5 = {}, {}, {}
        do5 = not args.no_a3 and not args.no_a4 and not args.no_a5
        if do5:
            cand5 = [a for a in cand4 if a not in emitted4]
            emitted5, tried5 = run_stage(cand5, sigs, depth, True, F, defs, byaddr, abi, cfin, dtargets, a1sigs, refdefs, args.max_depth,
                                         vu=True, a5=True)
            changed5 = True
            while changed5:
                changed5 = False
                for a in sorted(emitted5):
                    E = emitted5[a]
                    if any(c not in sigs for c in E.callees):
                        why = 'a typed callee refused'
                    else:
                        res = live_typed_caller(F[a], E.sig, sigs, set(E.callees))
                        bad = [(x, t, sorted(sigs[t].dropped & after)) for x, (t, after) in res.items() if sigs[t].dropped & after]
                        why = ('a callee scratch / extra output read after the call: ' + '; '.join(f'{x:x} {t:x} {b}' for x, t, b in bad[:2])) if bad else None
                    if why:
                        tried5[a] = why
                        del emitted5[a]
                        del sigs[a]
                        changed5 = True

            def reemit5(E, want):
                E2 = Emit(E.addr, E.name, abi.get(E.addr), sigs, cfin, F[E.addr], loops=True, xout=want, vu=True, a5=True)
                if not E2.ok:
                    REEMIT_WHY[E.addr] = E2.why
                    return None
                E2.guards, E2.sig.guards = E.guards, E.guards
                E2.holds = E2.sig.holds = E.holds
                E2.ref, E2.depth = E.ref, E.depth
                return E2
            live5 = caller_check(emitted5, sigs, F, args.abi, abi, cfin, reemit=reemit5)
            why5 = why_counts(tried5, live5)
            emits5 = [emitted5[a] for a in sorted(emitted5, key=lambda a: (emitted5[a].depth, a))]
            for i, E in enumerate(emits5):
                E.idx = i
            new, nadapt5 = insert_adapters(new, {E.name: E for E in emits5}, INC_LINE5, (INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits5:
                tiers5[tier5(E)] += 1
            stats += (f'\ntyped_lift2 stage 5 (A5): {len(cand5)} candidates, {len(emits5)} converted {dict(sorted(tiers5.items()))} '
                      f'(refused: {dict(why5.most_common())}); {nadapt5} adapters')
        # ---- stage 6 (round A6): never-lifted functions and hand natives typed from their MIPS (no adapter: their typed callers
        # call them while the slot holds what GUARD_SYM names), the typed runtime stubs (A6_EXT), break (A6_BREAK)
        emits6, why6, tiers6, cand6 = [], collections.Counter(), collections.Counter(), []
        emitted6, tried6, live6 = {}, {}, {}
        nolift, natives6 = set(), {}
        do6 = do5 and not args.no_a6
        if do6:
            tab = L.table()
            stubs = set()
            sp = os.path.join(L.RUNNER, 'ps2_recompiled_stubs.h')
            if os.path.exists(sp):
                stubs = {int(x, 16) for x in re.findall(r'void \w+_0x([0-9a-f]+)\(', open(sp).read())}
            reg_lifted = set()
            for t in base.values():
                for m in N4.REG.finditer(t):
                    reg_lifted.add(int(m.group(1), 16))
            sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
            import decomp100_count as DC
            natreg = DC.native_registrations()
            if args.natives6 and os.path.exists(args.natives6):
                for line in open(args.natives6):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives6[int(line[0], 16)] = line[1]
            names6 = dict(byaddr)
            used = {sg.name for sg in sigs.values()} | set(byaddr.values()) | set(defs)
            for a in sorted(F):
                if a in sigs or a in byaddr or a in reg_lifted or a in stubs:
                    continue
                if a in natreg and a not in natives6:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in N3.LIBC or nm in skip3:  # docs/a3_skip.txt: the long loops stay as they are
                    continue
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = ('::' + natives6[a]) if a in natives6 else ('::' + sym)
                nolift.add(a)

            class Refs(dict):
                """the reference texts: the lifted functions' (undone), the never-lifted ones' from lift_d1's Lifter on demand"""
                def get(self, name, default=None):
                    if name in refdefs:
                        return refdefs[name]
                    a = by_name6.get(name)
                    if a is None:
                        return default
                    try:
                        txt = L.Lifter(a, {}).text('kLiftD1') + '\n'
                    except Exception:
                        return default
                    m = N4.FUNC.search(txt)
                    return m.group(5) if m else default
            by_name6 = {names6[a]: a for a in nolift}
            if args.no_ext6:
                a6_ext_sigs()  # GUARD_SYM only
            else:
                sigs.update(a6_ext_sigs())
            if args.no_nolift6:
                nolift = {a for a in nolift if a in natives6}
                by_name6 = {names6[a]: a for a in nolift}
            NO_BRK6[0] = args.no_brk6
            cand6 = [a for a in cand5 if a not in emitted5] + sorted(nolift)
            emitted6, tried6 = run_stage6(cand6, sigs, depth, F, names6, abi, cfin, Refs(), args.max_depth, nolift)
            changed6 = True
            while changed6:
                changed6 = False
                for a in sorted(emitted6):
                    E = emitted6[a]
                    if any(c not in sigs for c in E.callees):
                        why = 'a typed callee refused'
                    else:
                        res = live_typed_caller(F[a], E.sig, sigs, set(E.callees))
                        bad = [(x, t, sorted(sigs[t].dropped & after)) for x, (t, after) in res.items() if sigs[t].dropped & after]
                        why = ('a callee scratch / extra output read after the call: ' + '; '.join(f'{x:x} {t:x} {b}' for x, t, b in bad[:2])) if bad else None
                    if why:
                        tried6[a] = why
                        del emitted6[a]
                        del sigs[a]
                        changed6 = True

            def reemit6(E, want):
                if E.addr in natives6:
                    return None
                E2 = Emit(E.addr, E.name, abi.get(E.addr), sigs, cfin, F[E.addr], loops=True, xout=want, vu=True, a5=True, a6=True)
                if not E2.ok:
                    REEMIT_WHY[E.addr] = E2.why
                    return None
                E2.guards, E2.sig.guards = E.guards, E.guards
                E2.holds = E2.sig.holds = E.holds
                E2.ref, E2.depth, E2.lifted = E.ref, E.depth, E.lifted
                return E2
            # a never-lifted function's adapter takes its slot: every caller in the IR reaches it (checked as the lifted functions'
            # callers are); a native keeps its slot, so only typed callers call its typed function
            live6 = caller_check(emitted6, sigs, F, args.abi, abi, cfin, reemit=reemit6, noadapter=frozenset(a for a in nolift if a in natives6))
            why6 = why_counts(tried6, live6)
            emits6 = [emitted6[a] for a in sorted(emitted6, key=lambda a: (emitted6[a].depth, a))]
            for i, E in enumerate(emits6):
                E.idx = i
            # a never-lifted function's slot holds its A6 adapter (typed_a6_adapters.cpp, installed over the recompiled function at
            # install), so the table's callers run the typed function too; its typed callers test the slot against the adapter
            rec6 = {}
            for E in emits6:
                if not E.lifted and E.addr not in natives6:
                    rec6[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a6ad::{E.name}'
            new, nadapt6 = insert_adapters(new, {E.name: E for E in emits6 if E.lifted}, INC_LINE6,
                                           (INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits6:
                tiers6[('native ' if E.addr in natives6 else 'nolift ' if not E.lifted else '') + tier5(E) + (' break' if E.brks else '')] += 1
            stats += (f'\ntyped_lift2 stage 6 (A6): {len(cand6)} candidates ({len(nolift)} never lifted / native-held), {len(emits6)} converted '
                      f'({sum(1 for E in emits6 if not E.lifted)} without an adapter) {dict(sorted(tiers6.items()))} '
                      f'(refused: {dict(why6.most_common(30))}); {nadapt6} adapters')
        # ---- stage 7 (round A7): typed callers take their callees' extra outputs back (a7_consume; variants of earlier-stage
        # callees), the printf-family cores (A7_EXT), more MIPS-step natives (docs/a7_natives.txt)
        emits7, vars7, why7, tiers7, cand7 = [], [], collections.Counter(), collections.Counter(), []
        emitted7, tried7, live7 = {}, {}, {}
        natives7, nolift7 = {}, set()
        do7 = do6 and not args.no_a7
        if do7:
            if args.natives7 and os.path.exists(args.natives7) and not args.no_nat7:
                for line in open(args.natives7):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives7[int(line[0], 16)] = line[1]
            for a in sorted(natives7):
                if a in sigs or a not in F or a in nolift:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives7[a]
                nolift7.add(a)
            by_name6.update({names6[a]: a for a in nolift7})
            base7 = dict(sigs)
            if args.no_ext7:
                a7_ext_sigs()
            else:
                base7.update(a7_ext_sigs())
            XO7_OFF[0] = args.no_xo7
            A13_DIAG[0] = args.a13_diag
            cand7 = [a for a in cand6 if a not in emitted6] + sorted(nolift7)
            noadapter7 = frozenset(set(natives6) | set(natives7))
            S7 = Stage7(cand7, base7, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7, noadapter7,
                        {a: sg.name for a, sg in base7.items() if a in F})
            for rnd in range(args.rounds7):
                S7.changed = False
                emitted7, tried7, sigs7, depth7 = S7.run_round(args.max_depth)
                viol = caller_check7(emitted7, F, args.abi, abi, cfin, noadapter7)
                for t, sl in viol.items():
                    if not sl <= S7.want_req[t]:
                        S7.want_req[t] |= sl
                        S7.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted7.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live7 = {}
                for t in drop:
                    live7[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted7[t]
                    del sigs7[t]
                print(f'typed_lift2 stage 7 round {rnd + 1}: {len(emitted7)} typed, {len(S7.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S7.want.values())} want_req {sum(len(v) for v in S7.want_req.values())} '
                      f'variants {sum(len(v) for v in S7.want_var.values())}; changed {S7.changed}', file=sys.stderr)
                if not S7.changed:
                    break
            sigs = sigs7
            depth = depth7
            why7 = why_counts(tried7, live7)
            emits7 = [emitted7[a] for a in sorted(emitted7, key=lambda a: (emitted7[a].depth, a))]
            for i, E in enumerate(emits7):
                E.idx = i
            # the variants some stage-7 function (or a used variant) calls
            usedv, work = set(), [E for E in emits7]
            while work:
                E = work.pop()
                for c in getattr(E, 'sg', {}).maps[0] if hasattr(E, 'sg') else ():
                    if c not in usedv and c in S7.variants:
                        usedv.add(c)
                        work.append(S7.variants[c])
            vars7 = sorted((S7.variants[c] for c in usedv), key=lambda E: (E.depth, E.addr))
            rec7 = {}
            for E in emits7:
                if not E.lifted and E.addr not in natives6 and E.addr not in natives7:
                    rec7[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a7ad::{E.name}'
            new, nadapt7 = insert_adapters(new, {E.name: E for E in emits7 if E.lifted}, INC_LINE7,
                                           (INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits7:
                tiers7[('native ' if E.addr in natives6 or E.addr in natives7 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '')] += 1
            stats += (f'\ntyped_lift2 stage 7 (A7): {len(cand7)} candidates ({len(nolift7)} more natives), {len(emits7)} converted '
                      f'({sum(1 for E in emits7 if not E.lifted)} without an adapter in the lifted text; {len(vars7)} variants of earlier functions) '
                      f'{dict(sorted(tiers7.items()))} (refused: {dict(why7.most_common(30))}); {nadapt7} adapters')
        # ---- stage 8 (round A8): stage 7's form again over what it left, with the argument-register spills' entry values
        # (Emit a8: the varargs functions), more MIPS-step natives (docs/a8_natives.txt), the natives as cores (A8_EXT)
        emits8, vars8, why8, tiers8, cand8 = [], [], collections.Counter(), collections.Counter(), []
        emitted8, tried8, live8 = {}, {}, {}
        natives8, nolift8 = {}, set()
        do8 = do7 and not args.no_a8
        if do8:
            if args.natives8 and os.path.exists(args.natives8) and not args.no_nat8:
                for line in open(args.natives8):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives8[int(line[0], 16)] = line[1]
            for a in sorted(natives8):
                if a in sigs or a not in F or a in nolift or a in nolift7:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives8[a]
                nolift8.add(a)
            by_name6.update({names6[a]: a for a in nolift8})
            base8 = dict(sigs)
            if args.no_ext8:
                a8_ext_sigs()
            else:
                base8.update(a8_ext_sigs())
            SP8_OFF[0] = args.no_sp8
            SPIN8_OFF[0] = args.no_spin8
            cand8 = [a for a in cand7 if a not in emitted7 and a not in base8] + sorted(nolift8)
            noadapter8 = frozenset(set(natives6) | set(natives7) | set(natives8))
            S8 = Stage7(cand8, base8, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8, noadapter8,
                        {a: sg.name for a, sg in base8.items() if a in F}, sfx='_x8', a8=True)
            for rnd in range(args.rounds8):
                S8.changed = False
                emitted8, tried8, sigs8, depth8 = S8.run_round(args.max_depth)
                viol = caller_check7(emitted8, F, args.abi, abi, cfin, noadapter8)
                for t, sl in viol.items():
                    if not sl <= S8.want_req[t]:
                        S8.want_req[t] |= sl
                        S8.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted8.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live8 = {}
                for t in drop:
                    live8[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted8[t]
                    del sigs8[t]
                print(f'typed_lift2 stage 8 round {rnd + 1}: {len(emitted8)} typed, {len(S8.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S8.want.values())} want_req {sum(len(v) for v in S8.want_req.values())} '
                      f'variants {sum(len(v) for v in S8.want_var.values())}; changed {S8.changed}', file=sys.stderr)
                if not S8.changed:
                    break
            sigs = sigs8
            depth = depth8
            why8 = why_counts(tried8, live8)
            emits8 = [emitted8[a] for a in sorted(emitted8, key=lambda a: (emitted8[a].depth, a))]
            for i, E in enumerate(emits8):
                E.idx = i
            usedv, work = set(), [E for E in emits8]
            while work:
                E = work.pop()
                for c in getattr(E, 'sg', {}).maps[0] if hasattr(E, 'sg') else ():
                    if c not in usedv and c in S8.variants:
                        usedv.add(c)
                        work.append(S8.variants[c])
            vars8 = sorted((S8.variants[c] for c in usedv), key=lambda E: (E.depth, E.addr))
            rec8 = {}
            nat8 = set(natives6) | set(natives7) | set(natives8)
            for E in emits8:
                if not E.lifted and E.addr not in nat8:
                    rec8[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a8ad::{E.name}'
            new, nadapt8 = insert_adapters(new, {E.name: E for E in emits8 if E.lifted}, INC_LINE8,
                                           (INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits8:
                tiers8[('native ' if E.addr in nat8 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 8 (A8): {len(cand8)} candidates ({len(nolift8)} more natives), {len(emits8)} converted '
                      f'({sum(1 for E in emits8 if not E.lifted)} without an adapter in the lifted text; {len(vars8)} variants of earlier functions) '
                      f'{dict(sorted(tiers8.items()))} (refused: {dict(why8.most_common(30))}); {nadapt8} adapters')
        # ---- stage 9 (round A9): stage 8's form again over what it left, the quat122 / geo126 natives as cores (A9_EXT),
        # more MIPS-step natives (docs/a9_natives.txt)
        emits9, vars9, why9, tiers9, cand9 = [], [], collections.Counter(), collections.Counter(), []
        emitted9, tried9, live9 = {}, {}, {}
        natives9, nolift9 = {}, set()
        do9 = do8 and not args.no_a9
        if do9:
            if args.natives9 and os.path.exists(args.natives9) and not args.no_nat9:
                for line in open(args.natives9):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives9[int(line[0], 16)] = line[1]
            for a in sorted(natives9):
                if a in sigs or a not in F or a in nolift or a in nolift7 or a in nolift8:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives9[a]
                nolift9.add(a)
            by_name6.update({names6[a]: a for a in nolift9})
            base9 = dict(sigs)
            if args.no_ext9:
                a9_ext_sigs()
            else:
                x9 = a9_ext_sigs()
                if args.ext9:
                    keep = {int(x, 16) for x in args.ext9.split(',')}
                    x9 = {a: v for a, v in x9.items() if a in keep}
                base9.update(x9)
            cand9 = [a for a in cand8 if a not in emitted8 and a not in base9] + sorted(nolift9)
            noadapter9 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9))
            S9 = Stage7(cand9, base9, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8 | nolift9, noadapter9,
                        {a: sg.name for a, sg in base9.items() if a in F}, sfx='_x9', a9=True)
            for rnd in range(args.rounds9):
                S9.changed = False
                emitted9, tried9, sigs9, depth9 = S9.run_round(args.max_depth)
                viol = caller_check7(emitted9, F, args.abi, abi, cfin, noadapter9)
                for t, sl in viol.items():
                    if not sl <= S9.want_req[t]:
                        S9.want_req[t] |= sl
                        S9.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted9.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live9 = {}
                for t in drop:
                    live9[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted9[t]
                    del sigs9[t]
                print(f'typed_lift2 stage 9 round {rnd + 1}: {len(emitted9)} typed, {len(S9.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S9.want.values())} want_req {sum(len(v) for v in S9.want_req.values())} '
                      f'variants {sum(len(v) for v in S9.want_var.values())}; changed {S9.changed}', file=sys.stderr)
                if not S9.changed:
                    break
            sigs = sigs9
            depth = depth9
            why9 = why_counts(tried9, live9)
            emits9 = [emitted9[a] for a in sorted(emitted9, key=lambda a: (emitted9[a].depth, a))]
            for i, E in enumerate(emits9):
                E.idx = i
            usedv, work = set(), [E for E in emits9]
            while work:
                E = work.pop()
                for c in getattr(E, 'sg', {}).maps[0] if hasattr(E, 'sg') else ():
                    if c not in usedv and c in S9.variants:
                        usedv.add(c)
                        work.append(S9.variants[c])
            vars9 = sorted((S9.variants[c] for c in usedv), key=lambda E: (E.depth, E.addr))
            rec9 = {}
            nat9 = set(natives6) | set(natives7) | set(natives8) | set(natives9)
            for E in emits9:
                if not E.lifted and E.addr not in nat9:
                    rec9[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a9ad::{E.name}'
            new, nadapt9 = insert_adapters(new, {E.name: E for E in emits9 if E.lifted}, INC_LINE9,
                                           (INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits9:
                tiers9[('native ' if E.addr in nat9 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 9 (A9): {len(cand9)} candidates ({len(nolift9)} more natives), {len(emits9)} converted '
                      f'({sum(1 for E in emits9 if not E.lifted)} without an adapter in the lifted text; {len(vars9)} variants of earlier functions) '
                      f'{dict(sorted(tiers9.items()))} (refused: {dict(why9.most_common(30))}); {nadapt9} adapters')
        # ---- stage 10 (round A10): stage 9's form again over what it left, with recursion (a self call or a cycle typed
        # against its own interface to a fixpoint), the upper-half class, more MIPS-step natives (docs/a10_natives.txt)
        emits10, vars10, why10, tiers10, cand10 = [], [], collections.Counter(), collections.Counter(), []
        emitted10, tried10, live10 = {}, {}, {}
        natives10, nolift10 = {}, set()
        do10 = do9 and not args.no_a10
        if do10:
            if args.natives10 and os.path.exists(args.natives10) and not args.no_nat10:
                for line in open(args.natives10):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives10[int(line[0], 16)] = line[1]
            for a in sorted(natives10):
                if a in sigs or a not in F or a in nolift or a in nolift7 or a in nolift8 or a in nolift9:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives10[a]
                nolift10.add(a)
            by_name6.update({names6[a]: a for a in nolift10})
            base10 = dict(sigs)
            base10.update(a10_ext_sigs())
            UH10_OFF[0] = args.no_uh10
            MUT10_OFF[0] = args.no_mut10
            if not args.no_uh10:
                # the upper-half functions that are never lifted (stage 6 left them out for their docs/a3_skip.txt line): named here
                for a in sorted(F):
                    if a in sigs or a in byaddr or a in reg_lifted or a in stubs or a in natreg or a in names6:
                        continue
                    sym = tab.get(a, '')
                    m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                    if not m or int(m.group(2), 16) != a or L.cxx(m.group(1)) not in UH10:
                        continue
                    nm = L.cxx(m.group(1))
                    if nm in used:
                        nm = f'{nm}_{a:x}'
                    used.add(nm)
                    names6[a] = nm
                    by_name6[nm] = a
                    GUARD_SYM[a] = '::' + sym
                    nolift10.add(a)
                    natives10.pop(a, None)
            uh10 = [] if args.no_uh10 else sorted({a for a, n in list(byaddr.items()) + [(b, m) for m, b in by_name6.items()] if n in UH10 and a in F and a not in base10})
            cand10 = [a for a in cand9 if a not in emitted9 and a not in base10] + sorted(nolift10) + uh10
            noadapter10 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10))
            S10 = Stage7(cand10, base10, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8 | nolift9 | nolift10, noadapter10,
                        {a: sg.name for a, sg in base10.items() if a in F}, sfx='_x10', a10=True, rec=not args.no_rec10)
            for rnd in range(args.rounds10):
                S10.changed = False
                emitted10, tried10, sigs10, depth10 = S10.run_round(args.max_depth)
                viol = caller_check7(emitted10, F, args.abi, abi, cfin, noadapter10)
                for t, sl in viol.items():
                    if not sl <= S10.want_req[t]:
                        S10.want_req[t] |= sl
                        S10.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted10.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live10 = {}
                for t in drop:
                    live10[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted10[t]
                    del sigs10[t]
                print(f'typed_lift2 stage 10 round {rnd + 1}: {len(emitted10)} typed, {len(S10.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S10.want.values())} want_req {sum(len(v) for v in S10.want_req.values())} '
                      f'variants {sum(len(v) for v in S10.want_var.values())}; changed {S10.changed}', file=sys.stderr)
                if not S10.changed:
                    break
            sigs = sigs10
            depth = depth10
            why10 = why_counts(tried10, live10)
            emits10 = [emitted10[a] for a in sorted(emitted10, key=lambda a: (emitted10[a].depth, a))]
            for i, E in enumerate(emits10):
                E.idx = i
            # the variants the emitted functions call (extra-output variants <name>_x10, upper-half variants <name>_u10), transitively
            usedv, usedu, work = set(), set(), [E for E in emits10]
            while work:
                E = work.pop()
                for c in set(getattr(E, 'callees', ())):
                    s_ = E.sg[c] if hasattr(E, 'sg') else None
                    if c in S10.variants and s_ is S10.variants[c].sig and c not in usedv:
                        usedv.add(c)
                        work.append(S10.variants[c])
                    if c in S10.uh_var and s_ is S10.uh_var[c].sig and c not in usedu:
                        usedu.add(c)
                        work.append(S10.uh_var[c])
            vars10 = sorted([S10.variants[c] for c in usedv] + [S10.uh_var[c] for c in usedu], key=lambda E: (E.depth, E.addr, E.name))
            # A10_REC: the recursive functions' depth counters (a check build's [a10] line), by index
            recs10 = [E for E in emits10 if getattr(E, 'rec', 0)]
            for k, E in enumerate(recs10):
                E.recidx = k
                E.text = E.text.replace('\n{\n', f'\n{{\n    A10_REC({k});\n', 1)
            rec10 = {}
            nat10 = set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10)
            for E in emits10:
                if not E.lifted and E.addr not in nat10:
                    rec10[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a10ad::{E.name}'
            new, nadapt10 = insert_adapters(new, {E.name: E for E in emits10 if E.lifted}, INC_LINE10,
                                           (INC_LINE9, INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits10:
                tiers10[('native ' if E.addr in nat10 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 10 (A10): {len(cand10)} candidates ({len(nolift10)} more natives), {len(emits10)} converted '
                      f'({sum(1 for E in emits10 if not E.lifted)} without an adapter in the lifted text; {len(vars10)} variants of earlier functions) '
                      f'{dict(sorted(tiers10.items()))} (refused: {dict(why10.most_common(30))}); {nadapt10} adapters; recursion fixpoint rounds {dict((f'{a:x}', n) for a, n in S10.rec_iters.items())}; cycles {len(set(S10.scc.values()))}, upper-half variants {len(S10.uh_var)} (not made: {dict((f'{a:x}', w[:40]) for a, w in S10.uh_why.items())})')
        # ---- stage 11 (round A11): stage 9's form again over what it left, with recursion (a self call or a cycle typed
        # against its own interface to a fixpoint), the matrix stack (iosPush / PopCurrentMatrix as typed calls over VU0's
        # data memory with vi15 an interface slot: A11_EXT), more MIPS-step natives (docs/a11_natives.txt)
        emits11, vars11, why11, tiers11, cand11 = [], [], collections.Counter(), collections.Counter(), []
        emitted11, tried11, live11 = {}, {}, {}
        natives11, nolift11 = {}, set()
        do11 = do10 and not args.no_a11
        if do11:
            if args.natives11 and os.path.exists(args.natives11) and not args.no_nat11:
                for line in open(args.natives11):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives11[int(line[0], 16)] = line[1]
            for a in sorted(natives11):
                if a in sigs or a not in F or a in nolift or a in nolift7 or a in nolift8 or a in nolift9 or a in nolift10:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives11[a]
                nolift11.add(a)
            by_name6.update({names6[a]: a for a in nolift11})
            base11 = dict(sigs)
            A11_EXT_OFF[0] = args.no_ext11
            MMI11_OFF[0] = args.no_mmi11
            base11.update(a11_ext_sigs())
            uh11 = []
            cand11 = [a for a in cand10 if a not in emitted10 and a not in base11] + sorted(nolift11) + uh11
            noadapter11 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11))
            S11 = Stage7(cand11, base11, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8 | nolift9 | nolift10 | nolift11, noadapter11,
                        {a: sg.name for a, sg in base11.items() if a in F}, sfx='_x11', a10=True, a11=True, rec=True)
            for rnd in range(args.rounds11):
                S11.changed = False
                emitted11, tried11, sigs11, depth11 = S11.run_round(args.max_depth)
                viol = caller_check7(emitted11, F, args.abi, abi, cfin, noadapter11)
                for t, sl in viol.items():
                    if not sl <= S11.want_req[t]:
                        S11.want_req[t] |= sl
                        S11.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted11.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live11 = {}
                for t in drop:
                    live11[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted11[t]
                    del sigs11[t]
                print(f'typed_lift2 stage 11 round {rnd + 1}: {len(emitted11)} typed, {len(S11.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S11.want.values())} want_req {sum(len(v) for v in S11.want_req.values())} '
                      f'variants {sum(len(v) for v in S11.want_var.values())}; changed {S11.changed}', file=sys.stderr)
                if not S11.changed:
                    break
            sigs = sigs11
            depth = depth11
            why11 = why_counts(tried11, live11)
            emits11 = [emitted11[a] for a in sorted(emitted11, key=lambda a: (emitted11[a].depth, a))]
            for i, E in enumerate(emits11):
                E.idx = i
            # the variants the emitted functions call (extra-output variants <name>_x11, upper-half variants <name>_u11), transitively
            usedv, usedu, work = set(), set(), [E for E in emits11]
            while work:
                E = work.pop()
                for c in set(getattr(E, 'callees', ())):
                    s_ = E.sg[c] if hasattr(E, 'sg') else None
                    if c in S11.variants and s_ is S11.variants[c].sig and c not in usedv:
                        usedv.add(c)
                        work.append(S11.variants[c])
                    if c in S11.uh_var and s_ is S11.uh_var[c].sig and c not in usedu:
                        usedu.add(c)
                        work.append(S11.uh_var[c])
            vars11 = sorted([S11.variants[c] for c in usedv] + [S11.uh_var[c] for c in usedu], key=lambda E: (E.depth, E.addr, E.name))
            # A11_REC: the recursive functions' depth counters (a check build's [a11] line), by index
            recs11 = [E for E in emits11 if getattr(E, 'rec', 0)]
            for k, E in enumerate(recs11):
                E.recidx = k
                E.text = E.text.replace('\n{\n', f'\n{{\n    A11_REC({k});\n', 1)
            rec11 = {}
            nat11 = set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11)
            for E in emits11:
                if not E.lifted and E.addr not in nat11:
                    rec11[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a11ad::{E.name}'
            new, nadapt11 = insert_adapters(new, {E.name: E for E in emits11 if E.lifted}, INC_LINE11,
                                           (INC_LINE10, INC_LINE9, INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits11:
                tiers11[('native ' if E.addr in nat11 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 11 (A11): {len(cand11)} candidates ({len(nolift11)} more natives), {len(emits11)} converted '
                      f'({sum(1 for E in emits11 if not E.lifted)} without an adapter in the lifted text; {len(vars11)} variants of earlier functions) '
                      f'{dict(sorted(tiers11.items()))} (refused: {dict(why11.most_common(30))}); {nadapt11} adapters; recursion fixpoint rounds {dict((f'{a:x}', n) for a, n in S11.rec_iters.items())}; cycles {len(set(S11.scc.values()))}, upper-half variants {len(S11.uh_var)} (not made: {dict((f'{a:x}', w[:40]) for a, w in S11.uh_why.items())})')
        # ---- stage 12 (round A12): stage 11's form again over what it left, with the motion subsystem's natives typed from
        # their MIPS (docs/a12_natives.txt), the spin rule's base taint flow-sensitive (SPIN12), a likely branch's restore on its
        # taken edge (LIK12), a callee-written register passed to a 64-bit parameter taken back as an extra output (PT12)
        emits12, vars12, why12, tiers12, cand12 = [], [], collections.Counter(), collections.Counter(), []
        emitted12, tried12, live12 = {}, {}, {}
        natives12, nolift12 = {}, set()
        do12 = do11 and not args.no_a12
        if do12:
            if args.natives12 and os.path.exists(args.natives12) and not args.no_nat12:
                for line in open(args.natives12):
                    line = line.split('#')[0].split()
                    if len(line) >= 2:
                        natives12[int(line[0], 16)] = line[1]
            for a in sorted(natives12):
                if a in sigs or a not in F or a in nolift or a in nolift7 or a in nolift8 or a in nolift9 or a in nolift10 or a in nolift11:
                    continue
                sym = tab.get(a, '')
                m = re.match(r'^(\w+)_0x([0-9a-f]+)$', sym)
                if not m or int(m.group(2), 16) != a:
                    continue
                nm = L.cxx(m.group(1))
                if nm in used:
                    nm = f'{nm}_{a:x}'
                used.add(nm)
                names6[a] = nm
                GUARD_SYM[a] = '::' + natives12[a]
                nolift12.add(a)
            by_name6.update({names6[a]: a for a in nolift12})
            base12 = dict(sigs)
            SPIN12_OFF[0] = args.no_spin12
            PT12_OFF[0] = args.no_pt12
            LIK12_OFF[0] = args.no_lik12
            CNT12_OFF[0] = args.no_cnt12
            uh12 = []
            cand12 = [a for a in cand11 if a not in emitted11 and a not in base12] + sorted(nolift12) + uh12
            noadapter12 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12))
            S12 = Stage7(cand12, base12, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8 | nolift9 | nolift10 | nolift11 | nolift12, noadapter12,
                        {a: sg.name for a, sg in base12.items() if a in F}, sfx='_x12', a10=True, a11=True, a12=True, rec=True)
            for rnd in range(args.rounds12):
                S12.changed = False
                emitted12, tried12, sigs12, depth12 = S12.run_round(args.max_depth)
                viol = caller_check7(emitted12, F, args.abi, abi, cfin, noadapter12)
                for t, sl in viol.items():
                    if not sl <= S12.want_req[t]:
                        S12.want_req[t] |= sl
                        S12.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted12.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live12 = {}
                for t in drop:
                    live12[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted12[t]
                    del sigs12[t]
                print(f'typed_lift2 stage 12 round {rnd + 1}: {len(emitted12)} typed, {len(S12.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S12.want.values())} want_req {sum(len(v) for v in S12.want_req.values())} '
                      f'variants {sum(len(v) for v in S12.want_var.values())}; changed {S12.changed}', file=sys.stderr)
                if not S12.changed:
                    break
            sigs = sigs12
            depth = depth12
            why12 = why_counts(tried12, live12)
            emits12 = [emitted12[a] for a in sorted(emitted12, key=lambda a: (emitted12[a].depth, a))]
            for i, E in enumerate(emits12):
                E.idx = i
            # the variants the emitted functions call (extra-output variants <name>_x12, upper-half variants <name>_u12), transitively
            usedv, usedu, work = set(), set(), [E for E in emits12]
            while work:
                E = work.pop()
                for c in set(getattr(E, 'callees', ())):
                    s_ = E.sg[c] if hasattr(E, 'sg') else None
                    if c in S12.variants and s_ is S12.variants[c].sig and c not in usedv:
                        usedv.add(c)
                        work.append(S12.variants[c])
                    if c in S12.uh_var and s_ is S12.uh_var[c].sig and c not in usedu:
                        usedu.add(c)
                        work.append(S12.uh_var[c])
            vars12 = sorted([S12.variants[c] for c in usedv] + [S12.uh_var[c] for c in usedu], key=lambda E: (E.depth, E.addr, E.name))
            # A12_REC: the recursive functions' depth counters (a check build's [a12] line), by index
            recs12 = [E for E in emits12 if getattr(E, 'rec', 0)]
            for k, E in enumerate(recs12):
                E.recidx = k
                E.text = E.text.replace('\n{\n', f'\n{{\n    A12_REC({k});\n', 1)
            rec12 = {}
            nat12 = set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12)
            for E in emits12:
                if not E.lifted and E.addr not in nat12:
                    rec12[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a12ad::{E.name}'
            new, nadapt12 = insert_adapters(new, {E.name: E for E in emits12 if E.lifted}, INC_LINE12,
                                           (INC_LINE10, INC_LINE9, INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits12:
                tiers12[('native ' if E.addr in nat12 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 12 (A12): {len(cand12)} candidates ({len(nolift12)} more natives), {len(emits12)} converted '
                      f'({sum(1 for E in emits12 if not E.lifted)} without an adapter in the lifted text; {len(vars12)} variants of earlier functions) '
                      f'{dict(sorted(tiers12.items()))} (refused: {dict(why12.most_common(30))}); {nadapt12} adapters; recursion fixpoint rounds {dict((f'{a:x}', n) for a, n in S12.rec_iters.items())}; cycles {len(set(S12.scc.values()))}, upper-half variants {len(S12.uh_var)} (not made: {dict((f'{a:x}', w[:40]) for a, w in S12.uh_why.items())})')
        # ---- stage 13 (round A13): stage 12's form again over what it left, with hi / lo taken back as a callee's extra outputs
        # (HL13: the callers that read a callee's hi / lo after the call, GetMotionNodeQuaternionPosToolCoord's class)
        emits13, vars13, why13, tiers13, cand13 = [], [], collections.Counter(), collections.Counter(), []
        emitted13, tried13, live13 = {}, {}, {}
        natives13, nolift13 = {}, set()
        do13 = do12 and not args.no_a13
        if do13:
            by_name6.update({names6[a]: a for a in nolift13})
            base13 = dict(sigs)
            HL13_OFF[0] = args.no_hl13
            REC13_OFF[0] = args.no_rec13
            uh13 = []
            cand13 = [a for a in cand12 if a not in emitted12 and a not in base13] + sorted(nolift13) + uh13
            noadapter13 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12) | set(natives13))
            S13 = Stage7(cand13, base13, dict(depth), F, names6, abi, cfin, Refs(), nolift | nolift7 | nolift8 | nolift9 | nolift10 | nolift11 | nolift12 | nolift13, noadapter13,
                        {a: sg.name for a, sg in base13.items() if a in F}, sfx='_x13', a10=True, a11=True, a12=True, a13=True, rec=True)
            for rnd in range(args.rounds13):
                S13.changed = False
                emitted13, tried13, sigs13, depth13 = S13.run_round(args.max_depth)
                viol = caller_check7(emitted13, F, args.abi, abi, cfin, noadapter13)
                for t, sl in viol.items():
                    if not sl <= S13.want_req[t]:
                        S13.want_req[t] |= sl
                        S13.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted13.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live13 = {}
                for t in drop:
                    live13[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted13[t]
                    del sigs13[t]
                print(f'typed_lift2 stage 13 round {rnd + 1}: {len(emitted13)} typed, {len(S13.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S13.want.values())} want_req {sum(len(v) for v in S13.want_req.values())} '
                      f'variants {sum(len(v) for v in S13.want_var.values())}; changed {S13.changed}', file=sys.stderr)
                if not S13.changed:
                    break
            sigs = sigs13
            depth = depth13
            why13 = why_counts(tried13, live13)
            emits13 = [emitted13[a] for a in sorted(emitted13, key=lambda a: (emitted13[a].depth, a))]
            for i, E in enumerate(emits13):
                E.idx = i
            # the variants the emitted functions call (extra-output variants <name>_x13, upper-half variants <name>_u13), transitively
            usedv, usedu, work = set(), set(), [E for E in emits13]
            while work:
                E = work.pop()
                for c in set(getattr(E, 'callees', ())):
                    s_ = E.sg[c] if hasattr(E, 'sg') else None
                    if c in S13.variants and s_ is S13.variants[c].sig and c not in usedv:
                        usedv.add(c)
                        work.append(S13.variants[c])
                    if c in S13.uh_var and s_ is S13.uh_var[c].sig and c not in usedu:
                        usedu.add(c)
                        work.append(S13.uh_var[c])
            vars13 = sorted([S13.variants[c] for c in usedv] + [S13.uh_var[c] for c in usedu], key=lambda E: (E.depth, E.addr, E.name))
            # A12_REC: the recursive functions' depth counters (a check build's [a12] line), by index
            recs13 = [E for E in emits13 if getattr(E, 'rec', 0)]
            for k, E in enumerate(recs13):
                E.recidx = k
                E.text = E.text.replace('\n{\n', f'\n{{\n    A13_REC({k});\n', 1)
            rec13 = {}
            nat13 = set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12) | set(natives13)
            for E in emits13:
                if not E.lifted and E.addr not in nat13:
                    rec13[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a13ad::{E.name}'
            new, nadapt13 = insert_adapters(new, {E.name: E for E in emits13 if E.lifted}, INC_LINE13,
                                           (INC_LINE12, INC_LINE10, INC_LINE9, INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits13:
                tiers13[('native ' if E.addr in nat13 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 13 (A13): {len(cand13)} candidates ({len(nolift13)} more natives), {len(emits13)} converted '
                      f'({sum(1 for E in emits13 if not E.lifted)} without an adapter in the lifted text; {len(vars13)} variants of earlier functions) '
                      f'{dict(sorted(tiers13.items()))} (refused: {dict(why13.most_common(30))}); {nadapt13} adapters; recursion fixpoint rounds {dict((f'{a:x}', n) for a, n in S13.rec_iters.items())}; cycles {len(set(S13.scc.values()))}, upper-half variants {len(S13.uh_var)} (not made: {dict((f'{a:x}', w[:40]) for a, w in S13.uh_why.items())})')
        # ---- stage 14 (round A14): stage 13's form again over what it left, with the runtime state lift_d1 left opaque as VU0-interface
        # slots (RT14: hi1 / lo1, SA, COP0 Status, VU0's clip flags and R) and a likely branch's break slot (BRK14)
        emits14, vars14, why14, tiers14, cand14 = [], [], collections.Counter(), collections.Counter(), []
        emitted14, tried14, live14 = {}, {}, {}
        natives14, nolift14 = {}, set()
        do14 = do13 and not args.no_a14
        if do14:
            by_name6.update({names6[a]: a for a in nolift14})
            base14 = dict(sigs)
            RT14_OFF[0] = args.no_rt14
            BRK14_OFF[0] = args.no_brk14
            SRCH14_OFF[0] = args.no_srch14
            CSR14_OFF[0] = args.no_csr14
            uh14 = []
            cand14 = [a for a in cand13 if a not in emitted13 and a not in base14] + sorted(nolift14) + uh14
            noadapter14 = frozenset(set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12) | set(natives13) | set(natives14))
            # A14 (RT14): the stage's facts with RT14's fields named (the functions whose cached facts have an opaque statement:
            # their FPR / field inputs as the stage-14 lifter sees them), and the FPR inputs over them (a typed EIntr reads no
            # FPR: its callers need not pass all 32); the lifted callers' checks (caller_check7) keep the cached facts
            F14, cfin14, nrt14 = F, cfin, 0
            if not args.no_rt14:
                F14, nrt14 = rt14_facts(F)
                cfin14 = T1.fpr_inputs_all(F14, natives)
            print(f'typed_lift2 stage 14: {nrt14} functions\' facts with the RT14 fields', file=sys.stderr)
            S14 = Stage7(cand14, base14, dict(depth), F14, names6, abi, cfin14, Refs(), nolift | nolift7 | nolift8 | nolift9 | nolift10 | nolift11 | nolift12 | nolift13 | nolift14, noadapter14,
                        {a: sg.name for a, sg in base14.items() if a in F}, sfx='_x14', a10=True, a11=True, a12=True, a13=True, a14=True, rec=True)
            for rnd in range(args.rounds14):
                S14.changed = False
                emitted14, tried14, sigs14, depth14 = S14.run_round(args.max_depth)
                viol = caller_check7(emitted14, F, args.abi, abi, cfin, noadapter14)
                for t, sl in viol.items():
                    if not sl <= S14.want_req[t]:
                        S14.want_req[t] |= sl
                        S14.changed = True
                drop = set(viol)
                ch = True
                while ch:
                    ch = False
                    for a, E in emitted14.items():
                        if a not in drop and any(c in drop for c in E.callees):
                            drop.add(a)
                            ch = True
                live14 = {}
                for t in drop:
                    live14[t] = ('live scratch at a caller (asked as an extra output: ' + ','.join(sorted(f'{k}{r}' for k, r in viol[t])) + ')') if t in viol \
                        else 'a typed callee refused (live scratch)'
                    del emitted14[t]
                    del sigs14[t]
                print(f'typed_lift2 stage 14 round {rnd + 1}: {len(emitted14)} typed, {len(S14.variants)} variants, {len(drop)} dropped at lifted callers; '
                      f'requests: want {sum(len(v) for v in S14.want.values())} want_req {sum(len(v) for v in S14.want_req.values())} '
                      f'variants {sum(len(v) for v in S14.want_var.values())}; changed {S14.changed}', file=sys.stderr)
                if not S14.changed:
                    break
            sigs = sigs14
            depth = depth14
            why14 = why_counts(tried14, live14)
            emits14 = [emitted14[a] for a in sorted(emitted14, key=lambda a: (emitted14[a].depth, a))]
            for i, E in enumerate(emits14):
                E.idx = i
            # the variants the emitted functions call (extra-output variants <name>_x14, upper-half variants <name>_u14), transitively
            usedv, usedu, work = set(), set(), [E for E in emits14]
            while work:
                E = work.pop()
                for c in set(getattr(E, 'callees', ())):
                    s_ = E.sg[c] if hasattr(E, 'sg') else None
                    if c in S14.variants and s_ is S14.variants[c].sig and c not in usedv:
                        usedv.add(c)
                        work.append(S14.variants[c])
                    if c in S14.uh_var and s_ is S14.uh_var[c].sig and c not in usedu:
                        usedu.add(c)
                        work.append(S14.uh_var[c])
            vars14 = sorted([S14.variants[c] for c in usedv] + [S14.uh_var[c] for c in usedu], key=lambda E: (E.depth, E.addr, E.name))
            # A12_REC: the recursive functions' depth counters (a check build's [a12] line), by index
            recs14 = [E for E in emits14 if getattr(E, 'rec', 0)]
            for k, E in enumerate(recs14):
                E.recidx = k
                E.text = E.text.replace('\n{\n', f'\n{{\n    A14_REC({k});\n', 1)
            rec14 = {}
            nat14 = set(natives6) | set(natives7) | set(natives8) | set(natives9) | set(natives10) | set(natives11) | set(natives12) | set(natives13) | set(natives14)
            for E in emits14:
                if not E.lifted and E.addr not in nat14:
                    rec14[E.addr] = GUARD_SYM[E.addr]
                    GUARD_SYM[E.addr] = f'::a14ad::{E.name}'
            new, nadapt14 = insert_adapters(new, {E.name: E for E in emits14 if E.lifted}, INC_LINE14,
                                           (INC_LINE13, INC_LINE12, INC_LINE10, INC_LINE9, INC_LINE8, INC_LINE7, INC_LINE6, INC_LINE5, INC_LINE4, INC_LINE3, INC_LINE, T1.INC_LINE, T1.INC_AFTER))
            for E in emits14:
                tiers14[('native ' if E.addr in nat14 else 'nolift ' if not E.lifted else '') + tier5(E) +
                       (' break' if E.brks else '') + (' xo' if E.xout else '') + (' cons' if E.cons else '') +
                       (' spill' if getattr(E, 'a7_spills', None) else '')] += 1
            stats += (f'\ntyped_lift2 stage 14 (A14): {len(cand14)} candidates ({len(nolift14)} more natives), {len(emits14)} converted '
                      f'({sum(1 for E in emits14 if not E.lifted)} without an adapter in the lifted text; {len(vars14)} variants of earlier functions) '
                      f'{dict(sorted(tiers14.items()))} (refused: {dict(why14.most_common(30))}); {nadapt14} adapters; recursion fixpoint rounds {dict((f'{a:x}', n) for a, n in S14.rec_iters.items())}; cycles {len(set(S14.scc.values()))}, upper-half variants {len(S14.uh_var)} (not made: {dict((f'{a:x}', w[:40]) for a, w in S14.uh_why.items())})')
        if args.unlock and not args.no_a3:
            why_of = {}
            for d in (tried_fail, live_refused, tried3, live3, tried4, live4, tried5, live5, tried6, live6, tried7, live7, tried8, live8, tried9, live9, tried10, live10, tried11, live11, tried12, live12, tried13, live13, tried14, live14):
                for a, w in d.items():
                    if a not in sigs:
                        why_of[a] = w
            skipped = {}
            for a, n in byaddr.items():
                if n in N3.LIBC:
                    skipped[a] = 'named like a libc function'
                elif n in skip3:
                    skipped[a] = 'docs/a3_skip.txt'
            if do6:
                for a in F:
                    if a not in sigs and a not in why_of and a not in byaddr and a not in nolift and a not in nolift7 and a not in nolift8 and a not in nolift9 and a not in nolift10 and a not in nolift11 and a not in nolift12 and a not in nolift13 and a not in nolift14:
                        skipped[a] = 'a hand native holds the slot' if a in natreg else 'not lifted once (lifted twice / a stub / unnamed)'
            unlock_table(args.unlock, F, ({**byaddr, **{a: names6[a] for a in nolift | nolift7 | nolift8 | nolift9 | nolift10 | nolift11 | nolift12 | nolift13 | nolift14}} if do6 else byaddr), sigs, why_of, skipped)
        if not args.check:
            with open(os.path.join(OV, 'typed_a2.inc'), 'w') as fo:
                fo.write("// typed_a2.inc: round A2's typed functions (tools/typed_lift2.py). Generated: do not edit. "
                         "Included inside a namespace (typed_a2.h: namespace typed), after typed_a1.inc.\n")
                fo.write('\n'.join(E.text for E in emits))
            with open(os.path.join(OV, 'typed_a2.h'), 'w') as fo:
                fo.write(HDR)
                names = sorted({nm for E in emits for _, nm in (set(E.holds) | set(E.guards))})
                fo.write('\n// the lifted functions whose slots the adapters test (A2_HOLDS / A2_SLOT)\nnamespace lift_d1\n{\n')
                fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in names))
                fo.write('}\n')
            rk = collections.Counter(E.rtype for E in emits)
            with open(args.out_list, 'w') as fo:
                fo.write(f"# a2_typed.txt: round A2's typed functions (tools/typed_lift2.py; docs/decomp_a2.md). {len(emits)} converted; "
                         f"tiers {dict(sorted(tiers.items()))}; returns {dict(rk)}\n")
                fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers\n')
                for E in emits:
                    fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                             f'{("frame%d " % E.frame) if E.acc else ""}depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)}\n')
            if args.check_tu:
                write_check(emits, a1sigs, None, os.path.join(OV, 'a2_typed_check.cpp'), os.path.join(OV, 'typed_a2_ref.inc'))
            if not args.no_a3:
                with open(os.path.join(OV, 'typed_a3.inc'), 'w') as fo:
                    fo.write("// typed_a3.inc: round A3's typed functions (tools/typed_lift2.py, stage 3). Generated: do not edit. "
                             "Included inside a namespace (typed_a3.h: namespace typed), after typed_a2.inc.\n")
                    fo.write('\n'.join(E.text for E in emits3))
                with open(os.path.join(OV, 'typed_a3.h'), 'w') as fo:
                    fo.write(HDR3)
                    names = sorted({nm for E in emits3 for _, nm in (set(E.holds) | set(E.guards))})
                    fo.write('\n// the lifted functions whose slots the adapters test (A3_HOLDS / A2_SLOT)\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in names))
                    fo.write('}\n')
                rk = collections.Counter(E.rtype for E in emits3)
                with open(args.out_list3, 'w') as fo:
                    fo.write(f"# a3_typed.txt: round A3's typed functions (tools/typed_lift2.py stage 3; docs/decomp_a3.md). {len(emits3)} converted; "
                             f"tiers {dict(sorted(tiers3.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers\n')
                    for E in emits3:
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)}\n')
                if args.check_tu:
                    write_check(emits3, a1sigs, None, os.path.join(OV, 'a3_typed_check.cpp'), os.path.join(OV, 'typed_a3_ref.inc'),
                                tag='a3', prev=emits, prev_incs=('a2',))
            if not args.no_a3 and not args.no_a4:
                with open(os.path.join(OV, 'typed_a4.inc'), 'w') as fo:
                    fo.write("// typed_a4.inc: round A4's typed functions (tools/typed_lift2.py, stage 4: VU0 / 128-bit). Generated: do not edit. "
                             "Included inside a namespace (typed_a4.h: namespace typed), after typed_a3.inc.\n")
                    fo.write('\n'.join(E.text for E in emits4))
                with open(os.path.join(OV, 'typed_a4.h'), 'w') as fo:
                    fo.write(HDR4)
                    names = sorted({nm for E in emits4 for _, nm in (set(E.holds) | set(E.guards))})
                    fo.write('\n// the lifted functions whose slots the adapters test (A4_HOLDS / A2_SLOT)\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in names))
                    fo.write('}\n')
                rk = collections.Counter(E.rtype for E in emits4)

                def vn(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))
                with open(args.out_list4, 'w') as fo:
                    fo.write(f"# a4_typed.txt: round A4's typed functions (tools/typed_lift2.py stage 4; docs/decomp_a4.md). {len(emits4)} converted; "
                             f"tiers {dict(sorted(tiers4.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out\n')
                    for E in emits4:
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn(E.vin)} | {vn(E.vout)}\n')
                if args.check_tu:
                    write_check(emits4, a1sigs, None, os.path.join(OV, 'a4_typed_check.cpp'), os.path.join(OV, 'typed_a4_ref.inc'),
                                tag='a4', prev=emits + emits3, prev_incs=('a2', 'a3'))
            if do5:
                with open(os.path.join(OV, 'typed_a5.inc'), 'w') as fo:
                    fo.write("// typed_a5.inc: round A5's typed functions (tools/typed_lift2.py, stage 5: the typed ABI proof, relocated calls, "
                             "guest frames, 64-bit inputs, jump tables). Generated: do not edit. Included inside a namespace (typed_a5.h: "
                             "namespace typed), after typed_a4.inc.\n")
                    fo.write('\n'.join(E.text for E in emits5))
                with open(os.path.join(OV, 'typed_a5.h'), 'w') as fo:
                    fo.write(HDR5)
                    names = sorted({nm for E in emits5 for _, nm in (set(E.holds) | set(E.guards))})
                    fo.write('\n// the lifted functions whose slots the adapters test (A5_HOLDS / A2_SLOT)\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in names))
                    fo.write('}\n')
                rk = collections.Counter(E.rtype for E in emits5)

                def vn5(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))
                with open(args.out_list5, 'w') as fo:
                    fo.write(f"# a5_typed.txt: round A5's typed functions (tools/typed_lift2.py stage 5; docs/decomp_a5.md). {len(emits5)} converted; "
                             f"tiers {dict(sorted(tiers5.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | A5 forms\n')
                    for E in emits5:
                        forms = ' '.join(x for x in ('gframe' if E.gf else '', 'gsp' if E.needs_sp and not E.gf else '',
                                                     'jtab%d' % len(E.jtabs) if E.jtabs else '', 'reloc%d' % len(E.relocs) if E.relocs else '',
                                                     'w64' if E.wide_in else '', 'abi5' if not (abi5_n4.get(E.addr) or (False,))[0] else '') if x)
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn5(E.vin)} | {vn5(E.vout)} | {forms}\n')
                if args.check_tu:
                    write_check(emits5, a1sigs, None, os.path.join(OV, 'a5_typed_check.cpp'), os.path.join(OV, 'typed_a5_ref.inc'),
                                tag='a5', prev=emits + emits3 + emits4, prev_incs=('a2', 'a3', 'a4'))

            if do6:
                with open(os.path.join(OV, 'typed_a6.inc'), 'w') as fo:
                    fo.write("// typed_a6.inc: round A6's typed functions (tools/typed_lift2.py, stage 6: never-lifted functions and hand natives "
                             "typed from their MIPS, the typed runtime stubs, break). Generated: do not edit. Included inside a namespace "
                             "(typed_a6.h: namespace typed), after typed_a5.inc.\n")
                    fo.write('\n'.join(E.text for E in emits6))
                with open(os.path.join(OV, 'typed_a6.h'), 'w') as fo:
                    fo.write(HDR6)
                    held = sorted({(c, nm) for E in emits6 for c, nm in E.holds})
                    lnames = sorted({nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A6_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits6)

                def vn6(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))
                with open(args.out_list6, 'w') as fo:
                    fo.write(f"# a6_typed.txt: round A6's typed functions (tools/typed_lift2.py stage 6; docs/decomp_a6.md). {len(emits6)} converted "
                             f"({sum(1 for E in emits6 if not E.lifted and E.addr not in natives6)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits6 if E.addr in natives6)} MIPS-step natives: no adapter); tiers {dict(sorted(tiers6.items()))}; "
                             f"returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder\n')
                    for E in emits6:
                        forms = ' '.join(x for x in ('nolift' if not E.lifted else '', 'native' if E.addr in natives6 else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'ext:' + ','.join(sorted(sigs[c].ext for c in set(E.callees) if getattr(sigs[c], 'ext', None)))
                                                     if any(getattr(sigs[c], 'ext', None) for c in E.callees) else '') if x)
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn6(E.vin)} | {vn6(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, E.name)}\n')
                with open(os.path.join(OV, 'typed_a6_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT6_HEAD)
                    ad = [E for E in emits6 if E.addr in rec6]
                    fo.write('namespace a6ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec6[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A6Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A6Ad kA6Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec6[E.addr]}, &a6ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a6InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA6Ad[i].addr) == kA6Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA6Ad[i].addr, kA6Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    write_check(emits6, a1sigs, None, os.path.join(OV, 'a6_typed_check.cpp'), os.path.join(OV, 'typed_a6_ref.inc'),
                                tag='a6', prev=emits + emits3 + emits4 + emits5, prev_incs=('a2', 'a3', 'a4', 'a5'), natives6=natives6)

            if do7:
                with open(os.path.join(OV, 'typed_a7.inc'), 'w') as fo:
                    fo.write("// typed_a7.inc: round A7's typed functions (tools/typed_lift2.py, stage 7: typed callers take their callees' extra "
                             "outputs back, the printf-family cores, more MIPS-step natives). Generated: do not edit. Included inside a namespace "
                             "(typed_a7.h: namespace typed), after typed_a6.inc. First the variants of earlier functions (<name>_x7: the same MIPS "
                             "typed with extra outputs, called by stage-7 functions only), then stage 7's functions.\n")
                    fo.write('\n'.join(E.text for E in vars7 + emits7))
                with open(os.path.join(OV, 'typed_a7.h'), 'w') as fo:
                    fo.write(HDR7)
                    held = sorted({(c, nm) for E in vars7 + emits7 for c, nm in E.holds})
                    lnames = sorted({nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A7_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits7)

                def vn7(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn7(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                nat7 = set(natives6) | set(natives7)
                with open(args.out_list7, 'w') as fo:
                    fo.write(f"# a7_typed.txt: round A7's typed functions (tools/typed_lift2.py stage 7; docs/decomp_a7.md). {len(emits7)} converted "
                             f"({sum(1 for E in emits7 if not E.lifted and E.addr not in nat7)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits7 if E.addr in nat7)} MIPS-step natives: no adapter); {len(vars7)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers7.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars7 + emits7:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat7 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn7(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn7(E.vin)} | {vn7(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn7(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a7_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT7_HEAD)
                    ad = [E for E in emits7 if E.addr in rec7]
                    fo.write('namespace a7ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec7[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A7Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A7Ad kA7Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec7[E.addr]}, &a7ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a7InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA7Ad[i].addr) == kA7Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA7Ad[i].addr, kA7Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars7:
                        E.refname = E.sig.orig
                    write_check(emits7 + vars7, a1sigs, None, os.path.join(OV, 'a7_typed_check.cpp'), os.path.join(OV, 'typed_a7_ref.inc'),
                                tag='a7', prev=emits + emits3 + emits4 + emits5 + emits6, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6'),
                                natives6={**natives6, **natives7})

            if do8:
                with open(os.path.join(OV, 'typed_a8.inc'), 'w') as fo:
                    fo.write("// typed_a8.inc: round A8's typed functions (tools/typed_lift2.py, stage 8: the varargs functions' spills, more "
                             "MIPS-step natives). Generated: do not edit. Included inside a namespace (typed_a8.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x8: the same MIPS typed with extra outputs, called "
                             "by stage-8 functions only), then stage 8's functions.\n")
                    fo.write('\n'.join(E.text for E in vars8 + emits8))
                with open(os.path.join(OV, 'typed_a8.h'), 'w') as fo:
                    fo.write(HDR8)
                    held = sorted({(c, nm) for E in vars8 + emits8 for c, nm in E.holds})
                    lnames = sorted({nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A8_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A8 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits8)

                def vn8(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn8(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list8, 'w') as fo:
                    fo.write(f"# a8_typed.txt: round A8's typed functions (tools/typed_lift2.py stage 8; docs/decomp_a8.md). {len(emits8)} converted "
                             f"({sum(1 for E in emits8 if not E.lifted and E.addr not in nat8)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits8 if E.addr in nat8)} MIPS-step natives: no adapter); {len(vars8)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers8.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars8 + emits8:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat8 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn8(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn8(E.vin)} | {vn8(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn8(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a8_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT8_HEAD)
                    ad = [E for E in emits8 if E.addr in rec8]
                    fo.write('namespace a8ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec8[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A8Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A8Ad kA8Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec8[E.addr]}, &a8ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a8InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA8Ad[i].addr) == kA8Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA8Ad[i].addr, kA8Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars8:
                        E.refname = E.sig.orig
                    write_check(emits8 + vars8, a1sigs, None, os.path.join(OV, 'a8_typed_check.cpp'), os.path.join(OV, 'typed_a8_ref.inc'),
                                tag='a8', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7'),
                                natives6={**natives6, **natives7, **natives8})

            if do9:
                with open(os.path.join(OV, 'typed_a9.inc'), 'w') as fo:
                    fo.write("// typed_a9.inc: round A9's typed functions (tools/typed_lift2.py, stage 9: the quat122 / geo126 natives as "
                             "cores). Generated: do not edit. Included inside a namespace (typed_a9.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x9: the same MIPS typed with extra outputs, called "
                             "by stage-9 functions only), then stage 9's functions.\n")
                    fo.write('\n'.join(E.text for E in vars9 + emits9))
                with open(os.path.join(OV, 'typed_a9.h'), 'w') as fo:
                    fo.write(HDR9)
                    held = sorted({(c, nm) for E in vars9 + emits9 for c, nm in E.holds})
                    lnames = sorted({nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A9_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits9)

                def vn9(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn9(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list9, 'w') as fo:
                    fo.write(f"# a9_typed.txt: round A9's typed functions (tools/typed_lift2.py stage 9; docs/decomp_a9.md). {len(emits9)} converted "
                             f"({sum(1 for E in emits9 if not E.lifted and E.addr not in nat9)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits9 if E.addr in nat9)} MIPS-step natives: no adapter); {len(vars9)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers9.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars9 + emits9:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat9 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn9(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn9(E.vin)} | {vn9(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn9(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a9_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT9_HEAD)
                    ad = [E for E in emits9 if E.addr in rec9]
                    fo.write('namespace a9ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec9[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A9Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A9Ad kA9Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec9[E.addr]}, &a9ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a9InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA9Ad[i].addr) == kA9Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA9Ad[i].addr, kA9Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars9:
                        E.refname = E.sig.orig
                    write_check(emits9 + vars9, a1sigs, None, os.path.join(OV, 'a9_typed_check.cpp'), os.path.join(OV, 'typed_a9_ref.inc'),
                                tag='a9', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8'),
                                natives6={**natives6, **natives7, **natives8, **natives9})
            if do10:
                with open(os.path.join(OV, 'typed_a10.inc'), 'w') as fo:
                    fo.write("// typed_a10.inc: round A10's typed functions (tools/typed_lift2.py, stage 10: recursion, the upper-half class). Generated: do not edit. Included inside a namespace (typed_a10.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x10: the same MIPS typed with extra outputs, called "
                             "by stage-10 functions only), then stage 10's functions.\n")
                    fo.write('// the recursive functions (A10_REC index): ' + ', '.join(f'{E.recidx} {E.name}' for E in emits10 if getattr(E, 'rec', 0)) + '\n')
                    fwd = [E for E in emits10 if getattr(E, 'rec', 0) > 1]
                    if fwd:
                        fo.write('// the cycles\' members (functions calling each other), declared first\n')
                        fo.write(''.join(f'inline {E.decl};\n' for E in fwd) + '\n')
                        for E in fwd:
                            E.text = E.text.replace(f'inline {E.decl}', 'inline ' + E.decl.replace(' = nullptr', ''), 1)
                    fo.write('\n'.join(E.text for E in vars10 + emits10))
                with open(os.path.join(OV, 'typed_a10.h'), 'w') as fo:
                    fo.write(HDR10)
                    held = sorted({(c, nm) for E in vars10 + emits10 for c, nm in E.holds})
                    lnames = sorted({nm[:-2] if nm.endswith('@x') else nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A10_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 / A10 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits10)

                def vn10(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn10(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list10, 'w') as fo:
                    fo.write(f"# a10_typed.txt: round A10's typed functions (tools/typed_lift2.py stage 10; docs/decomp_a10.md). {len(emits10)} converted "
                             f"({sum(1 for E in emits10 if not E.lifted and E.addr not in nat10)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits10 if E.addr in nat10)} MIPS-step natives: no adapter); {len(vars10)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers10.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars10 + emits10:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'uh' if E.name.endswith('_u10') else '', ('rec%d' % E.rec) if getattr(E, 'rec', 0) else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat10 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn10(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn10(E.vin)} | {vn10(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn10(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a10_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT10_HEAD)
                    ad = [E for E in emits10 if E.addr in rec10]
                    fo.write('namespace a10ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec10[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A10Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A10Ad kA10Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec10[E.addr]}, &a10ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a10InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA10Ad[i].addr) == kA10Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA10Ad[i].addr, kA10Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars10:
                        E.refname = E.sig.orig
                    write_check(emits10 + vars10, a1sigs, None, os.path.join(OV, 'a10_typed_check.cpp'), os.path.join(OV, 'typed_a10_ref.inc'),
                                tag='a10', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8 + emits9, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8', 'a9'),
                                natives6={**natives6, **natives7, **natives8, **natives9, **natives10})
            if do11:
                with open(os.path.join(OV, 'typed_a11.inc'), 'w') as fo:
                    fo.write("// typed_a11.inc: round A11's typed functions (tools/typed_lift2.py, stage 11: the matrix stack, recursion over it). Generated: do not edit. Included inside a namespace (typed_a11.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x11: the same MIPS typed with extra outputs, called "
                             "by stage-11 functions only), then stage 11's functions.\n")
                    fo.write('// the recursive functions (A11_REC index): ' + ', '.join(f'{E.recidx} {E.name}' for E in emits11 if getattr(E, 'rec', 0)) + '\n')
                    fwd = [E for E in emits11 if getattr(E, 'rec', 0) > 1]
                    if fwd:
                        fo.write('// the cycles\' members (functions calling each other), declared first\n')
                        fo.write(''.join(f'inline {E.decl};\n' for E in fwd) + '\n')
                        for E in fwd:
                            E.text = E.text.replace(f'inline {E.decl}', 'inline ' + E.decl.replace(' = nullptr', ''), 1)
                    fo.write('\n'.join(E.text for E in vars11 + emits11))
                with open(os.path.join(OV, 'typed_a11.h'), 'w') as fo:
                    fo.write(HDR11)
                    held = sorted({(c, nm) for E in vars11 + emits11 for c, nm in E.holds})
                    lnames = sorted({nm[:-2] if nm.endswith('@x') else nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A11_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 / A11 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits11)

                def vn11(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn11(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list11, 'w') as fo:
                    fo.write(f"# a11_typed.txt: round A11's typed functions (tools/typed_lift2.py stage 11; docs/decomp_a11.md). {len(emits11)} converted "
                             f"({sum(1 for E in emits11 if not E.lifted and E.addr not in nat11)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits11 if E.addr in nat11)} MIPS-step natives: no adapter); {len(vars11)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers11.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars11 + emits11:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'uh' if E.name.endswith('_u11') else '', ('rec%d' % E.rec) if getattr(E, 'rec', 0) else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat11 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn11(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn11(E.vin)} | {vn11(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn11(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a11_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT11_HEAD)
                    ad = [E for E in emits11 if E.addr in rec11]
                    fo.write('namespace a11ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec11[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A11Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A11Ad kA11Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec11[E.addr]}, &a11ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a11InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA11Ad[i].addr) == kA11Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA11Ad[i].addr, kA11Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars11:
                        E.refname = E.sig.orig
                    write_check(emits11 + vars11, a1sigs, None, os.path.join(OV, 'a11_typed_check.cpp'), os.path.join(OV, 'typed_a11_ref.inc'),
                                tag='a11', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8 + emits9 + emits10 + vars10, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8', 'a9', 'a10'),
                                natives6={**natives6, **natives7, **natives8, **natives9, **natives10, **natives11})
            if do12:
                with open(os.path.join(OV, 'typed_a12.inc'), 'w') as fo:
                    fo.write("// typed_a12.inc: round A12's typed functions (tools/typed_lift2.py, stage 12: the motion natives, the list walk). Generated: do not edit. Included inside a namespace (typed_a12.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x12: the same MIPS typed with extra outputs, called "
                             "by stage-12 functions only), then stage 12's functions; every one declared first.\n")
                    fo.write('// the recursive functions (A12_REC index): ' + ', '.join(f'{E.recidx} {E.name}' for E in emits12 if getattr(E, 'rec', 0)) + '\n')
                    # A12: variants of stage-12 functions themselves (a recursive walker's _u12 / _x12 calling its stage-12 form, an
                    # extra-output variant of a callee typed later in the order): every function and variant declared first
                    fwd = list(vars12) + list(emits12)
                    if fwd:
                        fo.write('// every function of the stage declared first (variants of stage-12 functions call stage-12 functions)\n')
                        fo.write(''.join(f'inline {E.decl};\n' for E in fwd) + '\n')
                        for E in fwd:
                            E.text = E.text.replace(f'inline {E.decl}', 'inline ' + E.decl.replace(' = nullptr', ''), 1)
                    fo.write('\n'.join(E.text for E in vars12 + emits12))
                with open(os.path.join(OV, 'typed_a12.h'), 'w') as fo:
                    fo.write(HDR12)
                    held = sorted({(c, nm) for E in vars12 + emits12 for c, nm in E.holds})
                    lnames = sorted({nm[:-2] if nm.endswith('@x') else nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A12_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 / A11 / A12 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits12)

                def vn12(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn12(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list12, 'w') as fo:
                    fo.write(f"# a12_typed.txt: round A12's typed functions (tools/typed_lift2.py stage 12; docs/decomp_a12.md). {len(emits12)} converted "
                             f"({sum(1 for E in emits12 if not E.lifted and E.addr not in nat12)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits12 if E.addr in nat12)} MIPS-step natives: no adapter); {len(vars12)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers12.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars12 + emits12:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'uh' if E.name.endswith('_u12') else '', ('rec%d' % E.rec) if getattr(E, 'rec', 0) else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat12 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn12(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn12(E.vin)} | {vn12(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn12(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a12_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT12_HEAD)
                    ad = [E for E in emits12 if E.addr in rec12]
                    fo.write('namespace a12ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec12[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A12Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A12Ad kA12Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec12[E.addr]}, &a12ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a12InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA12Ad[i].addr) == kA12Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA12Ad[i].addr, kA12Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars12:
                        E.refname = E.sig.orig
                    write_check(emits12 + vars12, a1sigs, None, os.path.join(OV, 'a12_typed_check.cpp'), os.path.join(OV, 'typed_a12_ref.inc'),
                                tag='a12', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8 + emits9 + emits10 + vars10 + emits11 + vars11, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11'),
                                natives6={**natives6, **natives7, **natives8, **natives9, **natives10, **natives11, **natives12})
            if do13:
                with open(os.path.join(OV, 'typed_a13.inc'), 'w') as fo:
                    fo.write("// typed_a13.inc: round A13's typed functions (tools/typed_lift2.py, stage 13: hi / lo as extra outputs). Generated: do not edit. Included inside a namespace (typed_a13.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x13: the same MIPS typed with extra outputs, called "
                             "by stage-13 functions only), then stage 13's functions; every one declared first.\n")
                    fo.write('// the recursive functions (A13_REC index): ' + ', '.join(f'{E.recidx} {E.name}' for E in emits13 if getattr(E, 'rec', 0)) + '\n')
                    # A12: variants of stage-13 functions themselves (a recursive walker's _u13 / _x13 calling its stage-13 form, an
                    # extra-output variant of a callee typed later in the order): every function and variant declared first
                    fwd = list(vars13) + list(emits13)
                    if fwd:
                        fo.write('// every function of the stage declared first (variants of stage-13 functions call stage-13 functions)\n')
                        fo.write(''.join(f'inline {E.decl};\n' for E in fwd) + '\n')
                        for E in fwd:
                            E.text = E.text.replace(f'inline {E.decl}', 'inline ' + E.decl.replace(' = nullptr', ''), 1)
                    fo.write('\n'.join(E.text for E in vars13 + emits13))
                with open(os.path.join(OV, 'typed_a13.h'), 'w') as fo:
                    fo.write(HDR13)
                    held = sorted({(c, nm) for E in vars13 + emits13 for c, nm in E.holds})
                    lnames = sorted({nm[:-2] if nm.endswith('@x') else nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A13_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 / A11 / A12 / A13 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits13)

                def vn13(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn13(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list13, 'w') as fo:
                    fo.write(f"# a13_typed.txt: round A13's typed functions (tools/typed_lift2.py stage 13; docs/decomp_a13.md). {len(emits13)} converted "
                             f"({sum(1 for E in emits13 if not E.lifted and E.addr not in nat13)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits13 if E.addr in nat13)} MIPS-step natives: no adapter); {len(vars13)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers13.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars13 + emits13:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'uh' if E.name.endswith('_u13') else '', ('rec%d' % E.rec) if getattr(E, 'rec', 0) else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat13 and not E.variant else '',
                                                     'break' if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn13(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn13(E.vin)} | {vn13(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn13(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a13_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT13_HEAD)
                    ad = [E for E in emits13 if E.addr in rec13]
                    fo.write('namespace a13ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec13[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A13Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A13Ad kA13Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec13[E.addr]}, &a13ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a13InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA13Ad[i].addr) == kA13Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA13Ad[i].addr, kA13Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars13:
                        E.refname = E.sig.orig
                    write_check(emits13 + vars13, a1sigs, None, os.path.join(OV, 'a13_typed_check.cpp'), os.path.join(OV, 'typed_a13_ref.inc'),
                                tag='a13', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8 + emits9 + emits10 + vars10 + emits11 + vars11 + emits12 + vars12, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12'),
                                natives6={**natives6, **natives7, **natives8, **natives9, **natives10, **natives11, **natives12, **natives13})
            if do14:
                with open(os.path.join(OV, 'typed_a14.inc'), 'w') as fo:
                    fo.write("// typed_a14.inc: round A14's typed functions (tools/typed_lift2.py, stage 14: runtime state as VU0-interface slots, break slots). Generated: do not edit. Included inside a namespace (typed_a14.h: namespace typed), after "
                             "typed_a7.inc. First the variants of earlier functions (<name>_x14: the same MIPS typed with extra outputs, called "
                             "by stage-14 functions only), then stage 14's functions; every one declared first.\n")
                    fo.write('// the recursive functions (A14_REC index): ' + ', '.join(f'{E.recidx} {E.name}' for E in emits14 if getattr(E, 'rec', 0)) + '\n')
                    # A12: variants of stage-14 functions themselves (a recursive walker's _u14 / _x14 calling its stage-14 form, an
                    # extra-output variant of a callee typed later in the order): every function and variant declared first
                    fwd = list(vars14) + list(emits14)
                    if fwd:
                        fo.write('// every function of the stage declared first (variants of stage-14 functions call stage-14 functions)\n')
                        fo.write(''.join(f'inline {E.decl};\n' for E in fwd) + '\n')
                        for E in fwd:
                            E.text = E.text.replace(f'inline {E.decl}', 'inline ' + E.decl.replace(' = nullptr', ''), 1)
                    fo.write('\n'.join(E.text for E in vars14 + emits14))
                with open(os.path.join(OV, 'typed_a14.h'), 'w') as fo:
                    fo.write(HDR14)
                    held = sorted({(c, nm) for E in vars14 + emits14 for c, nm in E.holds})
                    lnames = sorted({nm[:-2] if nm.endswith('@x') else nm for c, nm in held if c not in GUARD_SYM})
                    fo.write('\n// what the adapters test the slots against (A14_HOLDS): lifted functions, never-lifted functions\' recompiled '
                             'functions / A6 / A7 / A9 / A11 / A12 / A13 / A14 adapters, hand natives, the runtime stubs\' holders\nnamespace lift_d1\n{\n')
                    fo.write(''.join(f'void {nm}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for nm in lnames))
                    fo.write('}\n')
                    for sym in sorted({GUARD_SYM[c] for c, nm in held if c in GUARD_SYM}):
                        parts = sym.lstrip(':').split('::')
                        fo.write(''.join(f'namespace {x} {{ ' for x in parts[:-1]) + f'void {parts[-1]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);'
                                 + ' }' * (len(parts) - 1) + '\n')
                rk = collections.Counter(E.rtype for E in emits14)

                def vn14(ss):
                    return ','.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey))

                def xn14(xs):
                    return ','.join(f'{k}{r}' for k, r in xs)
                with open(args.out_list14, 'w') as fo:
                    fo.write(f"# a14_typed.txt: round A14's typed functions (tools/typed_lift2.py stage 14; docs/decomp_a14.md). {len(emits14)} converted "
                             f"({sum(1 for E in emits14 if not E.lifted and E.addr not in nat14)} never lifted: their adapter over the recompiled function's slot; "
                             f"{sum(1 for E in emits14 if E.addr in nat14)} MIPS-step natives: no adapter); {len(vars14)} variants of earlier functions "
                             f"(lines marked variant: not counted); tiers {dict(sorted(tiers14.items()))}; returns {dict(rk)}\n")
                    fo.write('# address name | C signature | dropped scratch slots | tier | PS2 float helpers | VU0 in | VU0 out | forms | slot holder | '
                             'extra outputs | consumed from callees\n')
                    for E in vars14 + emits14:
                        forms = ' '.join(x for x in ('variant' if E.variant else '', 'uh' if E.name.endswith('_u14') else '', ('rec%d' % E.rec) if getattr(E, 'rec', 0) else '', 'nolift' if not E.lifted and not E.variant else '',
                                                     'native' if E.addr in nat14 and not E.variant else '',
                                                     ('brkslot' if getattr(E, 'brk14', None) else 'break') if E.brks else '', 'gframe' if E.gf else '',
                                                     'gsp' if E.needs_sp and not E.gf else '', 'jtab%d' % len(E.jtabs) if E.jtabs else '',
                                                     'reloc%d' % len(E.relocs) if E.relocs else '', 'w64' if E.wide_in else '',
                                                     'spill' if getattr(E, 'a7_spills', None) else '',
                                                     'ext:' + ','.join(sorted({E.sg[c].ext for c in set(E.callees) if getattr(E.sg[c], 'ext', None)}))
                                                     if any(getattr(E.sg[c], 'ext', None) for c in E.callees) else '') if x)
                        consn = ' '.join(sorted({f'{getattr(E.sg[E.lf.kind[x][1]], "orig", E.sg[E.lf.kind[x][1]].name)}:{xn14(sorted(c))}' for x, c in E.cons.items()}))
                        fo.write(f'{E.addr:08x} {E.name} | {E.decl} | {",".join(k + str(r) for k, r in sorted(E.dropped, key=str))} | '
                                 f'{"loop%d " % len(E.backs) if E.backs else ""}{("frame%d " % E.frame) if E.acc else ""}'
                                 f'depth{E.depth if E.callees else 0} | {",".join(E.fpu_ps2)} | {vn14(E.vin)} | {vn14(E.vout)} | {forms} | '
                                 f'{guard_sym(E.addr, getattr(E.sig, "orig", E.name))} | {xn14(E.xout)} | {consn}\n')
                with open(os.path.join(OV, 'typed_a14_adapters.cpp'), 'w') as fo:
                    fo.write(ADAPT14_HEAD)
                    ad = [E for E in emits14 if E.addr in rec14]
                    fo.write('namespace a14ad\n{\n')
                    for E in ad:
                        fo.write(f'// {E.name} (0x{E.addr:x}): the slot of a never-lifted function; the typed function while the adapter\'s '
                                 f'test holds, else the recompiled function\n')
                        fo.write(f'void {E.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n')
                        fo.write(f'    if (PS2_EXPECT(ctx->pc == 0x{E.addr:X}u, 1))\n    {{\n    {E.adapter()}\n    }}\n')
                        fo.write(f'    {rec14[E.addr]}(rdram, ctx, runtime);\n}}\n')
                    fo.write('}\n\n')
                    fo.write('namespace\n{\n    struct A14Ad\n    {\n        uint32_t addr;\n        PS2Runtime::RecompiledFunction rec, ad;\n    };\n')
                    fo.write(f'    const A14Ad kA14Ad[{max(1, len(ad))}] = {{\n')
                    for E in ad:
                        fo.write(f'        {{0x{E.addr:X}u, &{rec14[E.addr]}, &a14ad::{E.name}}},\n')
                    if not ad:
                        fo.write('        {0u, nullptr, nullptr},\n')
                    fo.write('    };\n}\n\n')
                    fo.write('// sotc_decomp::install: each adapter over its slot while the slot still holds the recompiled function\n')
                    fo.write('uint32_t a14InstallAdapters(PS2Runtime &runtime)\n{\n    uint32_t n = 0;\n')
                    fo.write(f'    for (uint32_t i = 0; i < {len(ad)}u; ++i)\n')
                    fo.write('        if (runtime.lookupFunction(kA14Ad[i].addr) == kA14Ad[i].rec)\n')
                    fo.write('        {\n            runtime.registerFunction(kA14Ad[i].addr, kA14Ad[i].ad);\n            ++n;\n        }\n    return n;\n}\n')
                if args.check_tu:
                    for E in vars14:
                        E.refname = E.sig.orig
                    write_check(emits14 + vars14, a1sigs, None, os.path.join(OV, 'a14_typed_check.cpp'), os.path.join(OV, 'typed_a14_ref.inc'),
                                tag='a14', prev=emits + emits3 + emits4 + emits5 + emits6 + emits7 + emits8 + emits9 + emits10 + vars10 + emits11 + vars11 + emits12 + vars12 + emits13 + vars13, prev_incs=('a2', 'a3', 'a4', 'a5', 'a6', 'a7', 'a8', 'a9', 'a10', 'a11', 'a12', 'a13'),
                                natives6={**natives6, **natives7, **natives8, **natives9, **natives10, **natives11, **natives12, **natives13, **natives14})
        def wlog(path, tf, lr, em):
            with open(path, 'w') as fo:
                for a in sorted(set(tf) | set(lr) | set(em)):
                    nm = byaddr.get(a) or (names6.get(a, '?') if do6 else '?')
                    if a in em:
                        E = em[a]
                        fo.write(f'{a:08x} {nm} OK depth={E.depth} frame={E.frame} loops={len(E.backs)} {E.rtype}\n')
                    else:
                        fo.write(f'{a:08x} {nm} REFUSED {lr.get(a) or tf.get(a)} {REEMIT_WHY.get(a, "")}\n')
        if args.log:
            wlog(args.log, tried_fail, live_refused, emitted)
        if args.log3 and not args.no_a3:
            wlog(args.log3, tried3, live3, emitted3)
        if args.log4 and not args.no_a3 and not args.no_a4:
            wlog(args.log4, tried4, live4, emitted4)
        if args.log5 and do5:
            wlog(args.log5, tried5, live5, emitted5)
        if args.log6 and do6:
            wlog(args.log6, tried6, live6, emitted6)
        if args.log7 and do7:
            wlog(args.log7, tried7, live7, emitted7)
        if args.log8 and do8:
            wlog(args.log8, tried8, live8, emitted8)
        if args.log9 and do9:
            wlog(args.log9, tried9, live9, emitted9)
        if args.log10 and do10:
            wlog(args.log10, tried10, live10, emitted10)
        if args.log11 and do11:
            wlog(args.log11, tried11, live11, emitted11)
        if args.log12 and do12:
            wlog(args.log12, tried12, live12, emitted12)
        if args.log13 and do13:
            wlog(args.log13, tried13, live13, emitted13)
        if args.log14 and do14:
            wlog(args.log14, tried14, live14, emitted14)
    changed = 0
    for f in files:
        if new[f] != texts[f]:
            changed += 1
            if not args.check:
                with open(f, 'w', errors='surrogateescape') as fo:
                    fo.write(new[f])
    if not args.undo:
        badu = [f for f in files if undo_text(new[f]) != base[f]]
        if badu:
            print(f'typed_lift2: undo does not give the input back for {badu[:3]}', file=sys.stderr)
    print(f'typed_lift2: {stats}; {len(files)} files ({changed} {"would change" if args.check else "changed"})', file=sys.stderr)


if __name__ == '__main__':
    main()
