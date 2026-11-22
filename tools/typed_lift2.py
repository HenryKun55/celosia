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
SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31}
LOADS = {0x20: 1, 0x24: 1, 0x21: 2, 0x25: 2, 0x23: 4, 0x27: 4, 0x31: 4, 0x37: 8}
STORES = {0x28: 1, 0x29: 2, 0x2B: 4, 0x39: 4, 0x3F: 8}
BADMEM = {0x22, 0x26, 0x2A, 0x2E, 0x1A, 0x1B, 0x2C, 0x2D, 0x1E, 0x1F, 0x36, 0x3E}  # lwl / lwr / swl / swr / ldl ... lq / sq
RET_SLOTS = {'void': set(), 'uint32_t': {('g', 2)}, 'uint64_t': {('g', 2)}, 'float': {('f', 0)}, 'a1::RetVF': {('g', 2), ('f', 0)}}


class Refuse(Exception):
    pass


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

    def inputs(self):
        return {('g', p[1]) for p in self.params if p[0] == 'g'} | {('f', p[1]) for p in self.params if p[0] == 'f'}


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

# ----------------------------------------------------------------------------------------------- A4: VU0 / 128-bit
# The VU0 state a function may carry across calls (the macro-mode registers are global: the game keeps the current
# matrix in vf1-vf4, iosPushVu0RegistersToWork saves all 32 ...) and the GPRs' upper 64 bits: slot -> (local, C type,
# context lvalue). ('h', r): the upper half of GPR r (lq / qmfc2 / pcpyld ... write it; sq reads it).
VU_FIELDS = {'vu0_acc': ('vacc', 'a4::vec4'), 'vu0_q': ('vq', 'float'), 'vu0_status': ('vstat', 'uint16_t')}


def vu_local(s):
    k, r = s
    if k == 'v':
        return f'vf{r}', 'a4::vec4', f'ctx->vu0_vf[{r}]'
    if k == 'h':
        return f'h{r}', 'uint64_t', None
    loc, ty = VU_FIELDS[r]
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
VU_NAME = re.compile(r'\b(vf(\d+)|vacc|vq|vstat|h(\d+))\b')


def vu_slot_of(m):
    if m.group(2) is not None:
        return ('v', int(m.group(2)))
    if m.group(3) is not None:
        return ('h', int(m.group(3)))
    return ('x', {'vacc': 'vu0_acc', 'vq': 'vu0_q', 'vstat': 'vu0_status'}[m.group(1)])


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
    text = re.sub(r'(?<![\w:])_mm_(\w+)\(', r'a4v::\1(', text)
    text = re.sub(r'(?<![\w:])READ128\(', 'A4_R128(', text)
    text = re.sub(r'(?<![\w:])WRITE128\(', 'A4_W128(', text)
    text = re.sub(r'\b__m128i\b', 'a4::q128', text)
    text = re.sub(r'\b__m128\b', 'a4::vec4', text)
    return text


class Emit:
    """One function written as typed C++ from lift_d1's IR."""

    def __init__(self, addr, name, abi, sigs, fin, ffacts, loops=False, xout=(), vu=False, a5=False):
        self.addr, self.name = addr, name
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
        try:
            self.run(abi, sigs, fin, ffacts)
            self.ok = True
        except Refuse as e:
            self.why = str(e)
        except L.Refuse as e:
            self.why = 'lifter: ' + str(e)[:50]

    def run(self, abi, sigs, fin, ffacts):
        if not self.a5 and (abi is None or not abi[0]):
            raise Refuse('not proven')
        lf = L.Lifter(self.addr, {})
        lf.build()
        nodes = lf.node_effects()
        lf.nodes = nodes
        self.lf = lf
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
        if lf.brk:
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
        try:
            acc, save_at, restores = host_frame()
        except Refuse as e:
            if not self.a5 or str(e) not in GF_REASONS:
                raise
            acc, save_at, restores = self.guest_frame(order, W)
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
        # ---------------------------------------------------------------- parameters, locals
        gin = set(abi[1]) - {0, 29, 31}
        if self.a5 and any(p[0] == 'g' and p[1] == 29 for c in callees for p in sigs[c].params):
            self.needs_sp = True  # A5: a callee keeps its frame in guest RAM: it gets the guest sp at the call
        if self.needs_sp:
            gin.add(29)
        fx = T1.fpr_x_inputs(ffacts, fin)
        if self.vu:  # A4: the VU0 state is the VU0 interface's (vu_scan), not a plain input
            fx = {s for s in fx if s[0] != 'v' and not (s[0] == 'x' and s[1] in VU_FIELDS)}
        if any(k == 'x' for k, r in fx):
            raise Refuse('field input ' + ','.join(sorted(r for k, r in fx if k == 'x')))
        if any(k == 'v' for k, r in fx):
            raise Refuse('vu0 / 128-bit')
        fins = {r for k, r in fx if k == 'f'}
        slots = {s for s in lf.allslots if s != ('g', 0)}
        for x in order:
            k = lf.kind[x]
            if k[0] in ('jal', 'tail'):
                for p in sigs[k[1]].params:
                    slots.add((p[0], p[1]))
                if k[0] == 'jal':
                    slots |= RET_SLOTS[sigs[k[1]].rtype]
        slots |= {('g', r) for r in gin} | {('f', r) for r in fins}
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
        saves = lf.find_saves()
        save_at = {sx: (r, off, size) for r, (off, size, sx) in saves.items()}
        restores = dict(lf.restores)
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
        if bad:
            raise Refuse('not proven: ' + ','.join(sorted(bad)))
        for c in fa['calls']:
            if c not in sigs:
                raise Refuse('untyped callee')
        inp = {c: {p[1] for p in sigs[c].params if p[0] == 'g'} for c in fa['calls']}
        ins = A.inputs_of(fa, inp) - fa['saves']
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
        if any(q[0] not in ('g', 'f') for q in xs) or not xs <= self.dropped:
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
            if xs & unk.get(x, frozenset()):
                raise Refuse('extra output changed by a callee')
            if k[0] == 'tail':
                cs = sigs[k[1]]
                if xs & (cs.dropped | RET_SLOTS[cs.rtype]):
                    raise Refuse('extra output changed by a tail callee')
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
            for z in body:
                k = lf.kind[z]
                if k[0] != 'br' or all(y in body for y in lf.succ[z]):
                    continue
                if k[1] in ('c1t', 'c1f'):
                    if ftaint:
                        raise Refuse('loop: exit may wait on memory (FPU)')
                elif {r for r in (k[2], k[3]) if r} & taint:
                    raise Refuse('loop: exit may wait on memory')

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
                else:
                    after = set()
                    for s in nodes[x][2]:
                        after |= live.get(s, set())
            if r in after:
                raise Refuse('restored register read')

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
                L_.append(f'{I}{{ {d}{" " if d else ""}{"A3_CHARGE" if self.loops else "A2_CHARGE"}(); {assign(s, self.call_expr(x, s))}{self.xo_flagset(RET_SLOTS[s.rtype])} }}')
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
        if re.search(r'\bctx\b', text):
            raise Refuse('ctx use')
        if re.search(r'\b(rdram|runtime|target)\b', text):
            raise Refuse('rdram / runtime use')
        if re.search(r'\bra\b', text):
            raise Refuse('ra use')
        if 'LIFT_SET_FACC' in text:
            text = text.replace('LIFT_SET_FACC(', 'A2_SET_FACC(')
        self.fpu_ps2 = sorted({h for h in T1.FPU_PS2.values() if h + '(' in text})
        ok = T1.OK_CALLS | {s.name for s in sigs.values()} | {'A2_CHARGE', 'a2::frLd', 'a2::frSt', 'A2_SET_FACC', 'A3_BACK', 'A3_CHARGE', 'A5_BADJUMP'}
        if self.vu:
            ok = ok | A4_KNOWN
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
        params = [f'{ty} {n}' for r, ty, n in self.gparams] + [f'float {n}' for r, n in self.fparams] + vparams
        if self.xout:
            params = params + ['uint64_t *xo = nullptr']
        sig = ', '.join(['uint8_t *ram', '[[maybe_unused]] PS2Runtime *rt'] + params)
        self.decl = f'{self.rtype} {self.name}({sig})'
        self.sig = Sig(self.name, self.addr, self.rtype, [('g', r, ty) for r, ty, _ in self.gparams] + [('f', r) for r, _ in self.fparams],
                       self.dropped, self.v0w, 'a5' if self.a5 else 'a4' if self.vu else 'a2')
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
            xo = ', extra outputs ' + ' '.join(f'xo[{i}]={L.GPR[r] if k == "g" else "f%d" % r}' for i, (k, r) in enumerate(self.xout))
        if self.vu:
            def vn(ss):
                return ' '.join(vu_local(q)[0] for q in sorted(ss, key=vu_sortkey)) or '-'
            xo += f'; VU0 in {vn(self.vin)}, out {vn(self.vout)}'
        if self.a5:
            xo += ''.join([', jump tables %d' % len(self.jtabs) if self.jtabs else '', ', relocated calls %d' % len(self.relocs) if self.relocs else '',
                           ', 64-bit inputs ' + ' '.join(L.GPR[r] for r in sorted(self.wide_in)) if self.wide_in else '',
                           ', guest sp' if self.needs_sp and not self.gf else ''])
        out = (f'// {self.name} (0x{self.addr:x}): typed by tools/typed_lift2.py (round {"A5" if self.a5 else "A4" if self.vu else "A3" if self.loops else "A2"}; {self.kind}{xo}'
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
            xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(f"Ps2GprU64(ctx, {q})" if k == "g" else f"(uint64_t)a1_bits(ctx->f[{q}])" for k, q in self.xout)}}}; '
            for i, (k, q) in enumerate(self.xout):
                xw += f'Ps2SetGpr64(ctx, {q}, a3_xo[{i}]); ' if k == 'g' else f'ctx->f[{q}] = a1_flt((uint32_t)a3_xo[{i}]); '
        leaf = go.startswith('A5_GO_LEAF()')
        return (f'    if (PS2_EXPECT({go}, 1)) {{ {" ".join(ld)}{" " if ld else ""}A5_CHK({self.name}{args}); {" ".join(vl)}{" " if vl else ""}{xd}'
                f'{"" if leaf else f"A5_IN({self.idx}); "}{r}{call}; {"" if leaf else "A5_OUT(); "}{st}{xw}{" ".join(vw)}{" " if vw else ""}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK5}')

    # the guest-signature adapter, one line at the top of the lifted function's body
    def adapter(self):
        ld, av = [], []
        for r, ty, n in self.gparams:
            ld.append(f'const {ty} a2_{r} = Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r});')
            av.append(f'a2_{r}')
        for r, n in self.fparams:
            ld.append(f'const float a2_f{r} = ctx->f[{r}];')
            av.append(f'a2_f{r}')
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
                xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(f"Ps2GprU64(ctx, {q})" if k == "g" else f"(uint64_t)a1_bits(ctx->f[{q}])" for k, q in self.xout)}}}; '
                for i, (k, q) in enumerate(self.xout):
                    xw += f'Ps2SetGpr64(ctx, {q}, a3_xo[{i}]); ' if k == 'g' else f'ctx->f[{q}] = a1_flt((uint32_t)a3_xo[{i}]); '
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
                xd = f'uint64_t a3_xo[{len(self.xout)}] = {{{", ".join(f"Ps2GprU64(ctx, {q})" if k == "g" else f"(uint64_t)a1_bits(ctx->f[{q}])" for k, q in self.xout)}}}; '
                for i, (k, q) in enumerate(self.xout):
                    xw += f'Ps2SetGpr64(ctx, {q}, a3_xo[{i}]); ' if k == 'g' else f'ctx->f[{q}] = a1_flt((uint32_t)a3_xo[{i}]); '
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
    t = t.replace(INC_LINE5, '').replace(INC_LINE4, '').replace(INC_LINE3, '').replace(INC_LINE, '')
    return '\n'.join(line for line in t.split('\n') if not ((line.endswith(ADAPT_MARK) and 'A2_GO' in line) or
                                                           (line.endswith(ADAPT_MARK3) and 'A3_GO' in line) or
                                                           (line.endswith(ADAPT_MARK4) and 'A4_GO' in line) or
                                                           (line.endswith(ADAPT_MARK5) and 'A5_GO' in line)))


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


def write_check(emits, a1sigs, defs_n4, path_tu, path_ref, tag='a2', prev=(), prev_incs=()):
    """The check TU (tag a2: round A2's; a3: round A3's, whose references may call A2's: prev holds A2's emits and
    prev_incs the extra ref / typed includes)."""
    ns = tag + 'ck'
    TAG = tag.upper()
    ref = []
    for E in emits:
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
    out = head + [f'#include "typed_{tag}_check_rt.h"',
                  f'#if PS2X_{TAG}_CHECK || defined({TAG}_OFFLINE)',
                  f'namespace {ns}',
                  '{',
                  'bool refCall(uint32_t t, uint32_t ret, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);',
                  '}',
                  REF_MACROS.replace('a2ck::', ns + '::') + (REF_JUMP.replace('a5ck::', ns + '::') if tag == 'a5' else ''),
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
    out.append('    switch (t)')
    out.append('    {')
    for a, s in sorted(a1sigs.items()):
        out.append(f'    case 0x{a:x}u: ref::{s.name}(rdram, ctx, runtime); break;')
    for E in list(prev) + list(emits):
        out.append(f'    case 0x{E.addr:x}u: ref::{E.name}(rdram, ctx, runtime); break;')
    out.append('    default: ckUnknownCall(t); return true;')
    out.append('    }')
    out.append('    (void)ret;')
    out.append('    return true;')
    out.append('}')
    for i, E in enumerate(emits):
        ps = ''.join(f', {ty} p{r}' for r, ty, _ in E.gparams) + ''.join(f', float pf{r}' for r, _ in E.fparams)
        av = ''.join(f', p{r}' for r, _, _ in E.gparams) + ''.join(f', pf{r}' for r, _ in E.fparams)
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
            sv = sorted((off, size) for (r, off, size) in getattr(E, 'save_at', {}).values()) if getattr(E, 'gf', False) else []
            if sv:  # A5: a guest frame's save slots (a reference store into one other than its save gives the check up)
                out.append('    static const int32_t kSv[][2] = {' + ', '.join(f'{{{o}, {z}}}' for o, z in sv) + '};')
                out.append(f'    ckSaves(kSv, {len(sv)}u);')
            out.append(f'    if (!ckLeg([&] {{ ref::{E.name}(rdram, &c, runtime); }})) return;')
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
                out.append(f'    uint64_t xo[{len(E.xout)}] = {{{", ".join(f"Ps2GprU64(ctx, {q})" if k == "g" else f"(uint64_t)a1ck::ckBits(ctx->f[{q}])" for k, q in E.xout)}}};')
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
            xc = []
            for j, (k, q) in enumerate(E.xout):
                xc.append(f'Ps2GprU64(&c, {q}) != xo[{j}]' if k == 'g' else f'a1ck::ckBits(c.f[{q}]) != (uint32_t)xo[{j}]')
            vb = ''
            if tag in ('a4', 'a5'):
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
            ''.join(f', ctx->f[{r}]' for r, _ in E.fparams)
        out.append(f'    {{"{E.name}", 0x{E.addr:x}u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) {{ {E.name}(rdram, ctx, runtime{av}); }}}},')
    if not emits:
        out.append('    {"none", 0u, nullptr},')
    out.append('};')
    out.append(f'const uint32_t kEntryCount = {len(emits)}u;')
    if tag == 'a5':
        # the relocated sites' words (the static target each A5_JAL tests): the offline harness writes them into its RAM
        rw = sorted({(site, w) for E in emits for site, w in getattr(E, 'reloc_words', {}).items()})
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
            ps = ''.join(f', {ty}' for r, ty, _ in E.gparams) + ''.join(', float' for r, _ in E.fparams)
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


def caller_check(emitted, sigs, F, abi_path, abi, cfin, reemit=None):
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
    for pre, k in (('unknown call', 'unknown call'), ('a callee scratch', 'a callee scratch / extra output read after the call'),
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
        elif a not in byaddr:
            reason[a] = 'not lifted once (recompiled only / native / lifted twice)'
        elif a in skipped:
            reason[a] = skipped[a]
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
        if args.unlock and not args.no_a3:
            why_of = {}
            for d in (tried_fail, live_refused, tried3, live3, tried4, live4, tried5, live5):
                for a, w in d.items():
                    if a not in sigs:
                        why_of[a] = w
            skipped = {}
            for a, n in byaddr.items():
                if n in N3.LIBC:
                    skipped[a] = 'named like a libc function'
                elif n in skip3:
                    skipped[a] = 'docs/a3_skip.txt'
            unlock_table(args.unlock, F, byaddr, sigs, why_of, skipped)
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

        def wlog(path, tf, lr, em):
            with open(path, 'w') as fo:
                for a in sorted(set(tf) | set(lr) | set(em)):
                    nm = byaddr.get(a, '?')
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
