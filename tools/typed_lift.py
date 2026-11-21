#!/usr/bin/env python3
"""typed_lift.py: round A1, the typed lifter's first stage (docs/decomp_a1.md, docs/decomp_100_plan.md T1 / T4 / T5).

For a loop-free, non-blocking leaf that tools/n4_abi.py proves (docs/n4b_native.txt) and that tools/lift_d1.py lifted,
it writes a real C++ function: no R5900 context, no function table, no guest calling convention.

    inline uint32_t sceGsSetHalfOffset(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, ...)

  - parameters: the leaf's proven inputs (n4_abi's GPR inputs; the FPR inputs from lift_d1's IR, read before any write
    on some path), typed as lift_d1 declares the register (uint32_t for a 32-bit use, uint64_t for a 64-bit one, float);
  - return: v0 when every return of the leaf stores it, f0 when every return stores f0, both (a1::RetVF) when both;
    nothing else leaves the function (the other registers it writes are its scratch: the caller-side check below);
  - the other MIPS registers are C locals (lift_d1's locals, now never loaded from or stored to a context);
  - guest memory through `ram` (main RAM) with N1's base test only (A1_R32B / A1_W32B: a base in main RAM is one host
    load / store; any other base goes to the runtime's slow path with `rt`, which is only the memory environment:
    scratchpad and I/O); constant addresses fold the test away;
  - FPU: the PS2's own rounding / clamping kept as named helpers (a1f::add / sub / mul / div / cvtw / lt ...: PS2
    operand alignment, the result clamp, the compare key, divide by zero), plain C float where lift_d1's macro is the
    plain C operation (mov, neg, abs, cvt.s.w, the finite multiply): docs/decomp_a1.md section 3 lists which;
  - no guest stack frame (leaves that use sp are left out this round), no checkpoints, no ra.

Each converted function gets two guest-side entries, generated into the lifted batch that defines / calls it:
  - the adapter: the lifted function's own body starts with `if (A1_ON) { inputs from ctx -> typed call -> v0 / f0 into
    ctx, ctx->pc = ra; return; }` (so the function table, the recompiled callers and the natives reach the typed body);
  - the N4 sites (tools/n4_native.py's native calls from lifted callers): `if (A1_ON && PS2_N4_FAST(...)) { typed call
    with the caller's values; v0 / f0 into the caller's local and ctx } else <the N4 site as it was>`.
A1_ON is the switch a1typed (sotc_decomp::kA1Typed, SOTC_A1TYPED), g_a1Typed set once a frame.

Exactness (why dropping the scratch registers is safe): a slot the leaf writes but does not return is checked at every
direct call site of the leaf in the whole program (lift_d1's IR of every parsed function): live after the call
(backward liveness; at the caller's own return the EE ABI's live set v0, s0-s7, gp, sp, fp, ra, f0, f20-f31; another
callee reads its n4_abi inputs, a jalr the ABI's argument registers) means the leaf is refused. --check-tu also writes
the in-run differential check (game/overrides/a1_typed_check.cpp, PS2X_A1_CHECK=1: every typed call is compared with a
copy of the lifted text on the same state, stores logged, not made) and the offline harness's input (T5).

Usage:
  typed_lift.py [--list docs/n4b_native.txt] [--abi abi.txt] [--cache facts.pkl] [--undo] [--check] [--log FILE]
                [--check-tu] files...
  (files: the lifted batches, game/overrides/decomp_lift*_NN.cpp). Idempotent (undo first, then apply). --undo: back to
  N4b's text byte for byte. tools/lift_d1.py runs `typed_lift.py --undo` before n4_native.py's undo.
"""
import argparse
import collections
import os
import pickle
import re
import sys
from multiprocessing import Pool

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lift_d1 as L  # noqa: E402
import n4_native as N4  # noqa: E402
import n4_abi as A  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OV = os.path.join(REPO, 'game/overrides')
INC_LINE = '#include "typed_a1.h" // a1\n'
INC_AFTER = '#include "sotc_decomp.h"\n'
ADAPT_MARK = ' // a1'
SITE_PRE = '/*a1*/ '
SITE_END = ' /*a1e*/ '

ARGS = set(range(4, 12))
ABI_LIVE_G = {2, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31}
ABI_LIVE_F = {0} | set(range(20, 32))
ABI_ARGS_F = set(range(12, 20))
ALL_G = set(range(1, 32))
ALL_F = set(range(0, 32))
XFIELDS = {'fcr31', 'hi', 'lo', 'f_acc'}


# ----------------------------------------------------------------------------------------------- undo / text helpers
def undo_text(t):
    t = t.replace(INC_LINE, '')
    out = []
    for line in t.split('\n'):
        if line.endswith(ADAPT_MARK) and 'A1_ON' in line:
            continue
        s = line.lstrip()
        if s.startswith(SITE_PRE):
            j = s.index(SITE_END)
            line = line[:len(line) - len(s)] + s[j + len(SITE_END):]
        out.append(line)
    return '\n'.join(out)


def balanced_args(text, i):
    """text[i] just after '(' -> (args, index after ')')"""
    return L.split_args(text, i)


def msub(text, name, fn):
    return L.macro_sub(text, re.escape(name), fn)


# ----------------------------------------------------------------------------------------------- IR facts (lift_d1)
def nongpr_word_uses(w):
    """The FPR / hi / lo / FPU accumulator reads of an instruction, from its word (lift_d1's per-statement uses count a
    destination as read). Unknown COP1 / MMI encodings read fs and ft / hi and lo (more uses: safe)."""
    op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
    u = set()
    if op == 0x11:  # COP1
        fs, ft = rd, rt
        if rs == 0:  # mfc1
            u.add(('f', fs))
        elif rs in (2, 4, 6, 8):  # cfc1 (fcr31: lift_d1's IR), mtc1, ctc1, bc1
            pass
        elif rs == 16:  # S
            if fn in (4,):  # sqrt.s fd, ft
                u.add(('f', ft))
            elif fn in (5, 6, 7, 0x24):  # abs, mov, neg, cvt.w.s
                u.add(('f', fs))
            else:
                u |= {('f', fs), ('f', ft)}
                if fn in (0x1c, 0x1d, 0x1e, 0x1f):  # madd / msub / madda / msuba
                    u.add(('x', 'f_acc'))
        elif rs == 20:  # W: cvt.s.w
            u.add(('f', fs))
        else:
            u |= {('f', fs), ('f', ft)}
    elif op == 0x39:  # swc1
        u.add(('f', rt))
    elif op == 0 and fn in (0x10, 0x12):  # mfhi / mflo
        u.add(('x', 'hi' if fn == 0x10 else 'lo'))
    elif op == 0x1C:  # MMI: madd / maddu / madd1 ..., mfhi1 / mflo1, the pmfhi / pmflo family
        if fn not in (0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13) and fn not in (0x04, 0x30, 0x34, 0x36):
            u |= {('x', 'hi'), ('x', 'lo')}
    return u


def ir_facts(a):
    """Per node: kind, successors, GPR uses (exact, from the words), all-kind defs, non-GPR uses; per function: calls."""
    try:
        lf = L.Lifter(a, {})
        lf.build()
        nodes = lf.node_effects()
    except Exception as e:
        return a, None, f'{type(e).__name__}: {str(e)[:60]}'
    W = lf.g.words
    out = {}
    for x in lf.order:
        n = nodes[x]
        k = n['kind']
        gu = set(A.word_uses(W[x]))
        dgu = set(A.word_uses(W[x + 4])) if x + 4 in lf.delay else set()
        defs = {s for s in n['defs'] if s != ('g', 0)}
        ddefs = {s for s in n.get('ddefs', set()) if s != ('g', 0)}
        # non-GPR uses: FPR / hi / lo / accumulator from the words; fcr31 and VU0 from lift_d1's IR
        ou = {s for s in n['uses'] if s[0] == 'v' or s == ('x', 'fcr31') or (s[0] == 'x' and s[1] not in XFIELDS)}
        dou = {s for s in n.get('duses', set()) if s[0] == 'v' or s == ('x', 'fcr31') or (s[0] == 'x' and s[1] not in XFIELDS)}
        ou |= nongpr_word_uses(W[x])
        if x + 4 in lf.delay:
            dou |= nongpr_word_uses(W[x + 4])
        # an FPU compare writes the condition bit (lift_d1 reads fcr31 to keep its other bits): a def, not a use; any
        # other non-branch use of fcr31 (cfc1) is a whole-register read
        cmp_ = ('x', 'fcr31') in defs
        dcmp = ('x', 'fcr31') in ddefs
        if cmp_:
            ou.discard(('x', 'fcr31'))
        if dcmp:
            dou.discard(('x', 'fcr31'))
        kk = k[0]
        info = None
        if kk in ('jal', 'tail', 'j'):
            info = k[1]
        elif kk == 'jr':
            info = (k[1], tuple(k[2]))
        elif kk == 'jalr':
            info = (k[1], k[2])
        elif kk == 'br':
            info = (k[4], bool(k[5]))
        out[x] = (kk, info, tuple(lf.succ[x]), frozenset(gu), frozenset(dgu), frozenset(defs), frozenset(ddefs),
                  frozenset(ou), frozenset(dou), bool(kk == 'op' and n['opaque']))
    return a, {'name': lf.g.name, 'order': list(lf.order), 'nodes': out}, None


def load_facts(cache, jobs):
    if cache and os.path.exists(cache):
        return pickle.load(open(cache, 'rb'))
    tab = L.table()
    funcs = sorted(a for a, c in tab.items() if re.search(r'_0x([0-9a-f]+)$', c) and int(re.search(r'_0x([0-9a-f]+)$', c).group(1), 16) == a)
    with Pool(jobs) as p:
        res = p.map(ir_facts, funcs, chunksize=32)
    F = {a: f for a, f, e in res if f is not None}
    bad = {a: e for a, f, e in res if f is None}
    d = {'facts': F, 'bad': bad, 'funcs': funcs}
    if cache:
        pickle.dump(d, open(cache, 'wb'))
    return d


def load_abi(path):
    """address -> (proven, inputs set, outputs set)"""
    inv = {n: i for i, n in enumerate(L.GPR)}
    out = {}
    for line in open(path):
        if line.startswith('#'):
            continue
        p = line.split()
        if len(p) < 5 or p[1] == 'NOTPARSED' or not p[3].startswith('in='):
            continue
        ins = {inv[x] for x in p[3][3:].split(',') if x}
        outs = {inv[x] for x in p[4][4:].split(',') if x}
        out[int(p[0], 16)] = (p[2] == 'PROVEN', ins, outs)
    return out


def fpr_x_inputs(f, fin=None):
    """Forward must-written analysis over non-GPR slots: the FPR / field slots read before any write on some path.
    fin (callee -> its FPR inputs; None: a leaf's own analysis): a call reads its callee's FPR inputs (unknown callee,
    jalr: the ABI's f12-f19), an opaque op every FPR."""
    nodes, order = f['nodes'], f['order']
    wr = {order[0]: frozenset()}
    work = [order[0]]
    reads = set()
    while work:
        x = work.pop()
        kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq = nodes[x]
        w = wr[x]
        if opq:
            reads |= {('f', r) for r in ALL_F} - w
        reads |= set(ou) - w
        mid = w | {s for s in defs if s[0] != 'g'}
        reads |= set(dou) - mid
        after = mid | {s for s in ddefs if s[0] != 'g'}
        if fin is not None and kk in ('jal', 'tail', 'jalr'):
            ci = fin.get(info) if kk != 'jalr' else None
            reads |= (ci if ci is not None else {('f', r) for r in ABI_ARGS_F}) - after
        for y in succ:
            e = mid if (kk == 'br' and info[1] and y != info[0]) else after
            old = wr.get(y)
            new = frozenset(e) if old is None else (old & frozenset(e))
            if old is None or new != old:
                wr[y] = new
                work.append(y)
    return reads


def written_at_returns(f):
    """The slots written on every path from the entry to every return (lift_d1's exits store a slot the function may
    write unconditionally: on a path that does not write it, the store passes the caller's value through)."""
    nodes, order = f['nodes'], f['order']
    wr = {order[0]: frozenset()}
    work = [order[0]]
    at_ret = None
    while work:
        x = work.pop()
        kk, info, succ, gu, dgu, defs, ddefs, ou, dou, opq = nodes[x]
        w = wr[x]
        mid = w | defs
        after = mid | ddefs
        if kk == 'ret':
            at_ret = after if at_ret is None else (at_ret & after)
        for y in succ:
            e = mid if (kk == 'br' and info[1] and y != info[0]) else after
            old = wr.get(y)
            new = frozenset(e) if old is None else (old & frozenset(e))
            if old is None or new != old:
                wr[y] = new
                work.append(y)
    # a return reached first through one path and later through another: recompute at the fixpoint
    at_ret = None
    for x in order:
        kk, info, succ, gu, dgu, defs, ddefs = nodes[x][:7]
        if kk == 'ret' and x in wr:
            after = wr[x] | defs | ddefs
            at_ret = after if at_ret is None else (at_ret & after)
    return at_ret or frozenset()


def fpr_inputs_all(F, natives):
    """Whole program: every parsed function's FPR inputs (least fixpoint, callees first; a native / unparsed callee
    reads the ABI's f12-f19)."""
    fin = {a: set() for a in F if a not in natives}
    changed, passes = True, 0
    while changed and passes < 30:
        changed, passes = False, passes + 1
        for a in fin:
            r = {s for s in fpr_x_inputs(F[a], fin) if s[0] == 'f'}
            if r != fin[a]:
                fin[a] = r
                changed = True
    return fin


def live_after_sites(f, callee_inputs, targets, callee_fin=None):
    """Backward liveness over a caller's nodes; returns {site address: live slot set after the call} for the jal / tail
    nodes whose target is in `targets` (a tail jump's 'after' is the ABI's live set at a return)."""
    nodes, order = f['nodes'], f['order']
    ABI0 = {('g', r) for r in ABI_LIVE_G} | {('f', r) for r in ABI_LIVE_F}
    ALL = {('g', r) for r in ALL_G} | {('f', r) for r in ALL_F} | {('x', k) for k in XFIELDS}
    # a whole-register read of fcr31 anywhere (cfc1): fcr31 live everywhere (its sticky bits pass through compares)
    fcr_whole = any(('x', 'fcr31') in n[7] or ('x', 'fcr31') in n[8] for n in nodes.values() if n[0] == 'op')

    def g(s):
        return {('g', r) for r in s}

    def cin(t):
        ins = callee_inputs.get(t)
        gi = ins if ins is not None else (ARGS | {28, 29})
        fi = callee_fin.get(t) if callee_fin is not None and ins is not None else None
        return g(gi) | (fi if fi is not None else {('f', r) for r in ABI_ARGS_F})

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
                li = (ALL if opq else uses) | (out - defs)
            elif kk == 'br':
                if info[1]:  # likely: the delay slot only on the taken edge
                    lt = live.get(info[0], set())
                    lnt = set()
                    for y in succ:
                        if y != info[0]:
                            lnt |= live.get(y, set())
                    li = uses | duses | (lt - ddefs) | lnt
                else:
                    li = uses | duses | (out - ddefs)
            elif kk == 'jal':
                li = duses | ((cin(info) | (out - {('g', 31)})) - ddefs)
            elif kk == 'tail':
                li = duses | ((cin(info) | ABI0) - ddefs)
            elif kk == 'ret':
                li = {('g', 31)} | duses | (ABI0 - ddefs)
            elif kk == 'jr':
                if info[1]:
                    li = uses | duses | g({info[0]}) | (out - ddefs)
                else:
                    li = set(ALL)
            elif kk == 'jalr':
                li = duses | ((g(ARGS | {28, 29, info[0]}) | {('f', r) for r in ABI_ARGS_F} | (out - {('g', info[1])})) - ddefs)
            else:  # j and anything else: falls / jumps to succ
                li = uses | duses | (out - ddefs)
            if fcr_whole:
                li = li | {('x', 'fcr31')}
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
            res[x] = (info, set(ABI0))
    return res


# ----------------------------------------------------------------------------------------------- the typed function
GDECL = re.compile(r'^\s*(uint32_t|uint64_t) (\w+ = Ps2GprU(?:32|64)\(ctx, \d+\)(?:, \w+ = Ps2GprU(?:32|64)\(ctx, \d+\))*);$')
FDECL = re.compile(r'^\s*float (\w+ = ctx->f\[\d+\](?:, \w+ = ctx->f\[\d+\])*);$')
XDECL = re.compile(r'^\s*(uint32_t|uint64_t|float|uint16_t) (\w+) = ctx->(fcr31|hi|lo|f_acc);$')
OTHER_DECL = re.compile(r'^\s*(?:bool) \w+ = [^;]*;$')
REFUSE_PAT = [
    ('vu0 / 128-bit', re.compile(r'__m128|READ128|WRITE128|_mm_|PS2_V|PS2_P[A-Z]|ps2Vu|vu0_')),
    ('call / runtime', re.compile(r'PS2_(?:CALL|TAIL|JUMP|CHECKPOINT|DIFFTRACE)|runtime->|eeCheckpoint')),
    ('stack (sp)', re.compile(r'\bsp\b')),
    ('static', re.compile(r'\bstatic\b')),
]
# lift_d1's FPU macros: the plain C ones (exactly the macro's text) and the PS2-specific ones (named helpers)
FPU_PLAIN = {
    'FPU_MOV_S': lambda a: f'((float)({a[0]}))',
    'FPU_NEG_S': lambda a: f'(-(float)({a[0]}))',
    'FPU_ABS_S': lambda a: f'std::fabs((float)({a[0]}))',
    'FPU_CVT_S_W': lambda a: f'((float)(int32_t)({a[0]}))',
    'FPU_MUL_SF': lambda a: f'((float)({a[0]}) * (float)({a[1]}))',
}
FPU_PS2 = {
    'FPU_ADD_S': 'a1f::add', 'FPU_SUB_S': 'a1f::sub', 'FPU_ADD_SF': 'a1f::addFinite', 'FPU_SUB_SF': 'a1f::subFinite',
    'FPU_MUL_S': 'a1f::mul', 'FPU_DIV_S': 'a1f::divClamp', 'FPU_SQRT_S': 'a1f::sqrtClamp', 'FPU_CVT_W_S': 'a1f::cvtw',
    'FPU_C_OLT_S': 'a1f::lt', 'FPU_C_OLE_S': 'a1f::le', 'FPU_C_EQ_S': 'a1f::eq',
    'ps2FpuDivide': 'a1f::div', 'ps2FpuSqrt': 'a1f::sqrt',
}
OK_CALLS = {'if', 'sizeof', 'std::memcpy', 'std::fabs', 'a1_sx64', 'a1::RetVF', 'a1_bits', 'a1_flt', 'ADD32', 'SUB32', 'SLL32', 'SRL32',
            'SRA32', 'SLT32', 'SLTU32', 'RC_U64X', 'RC_S64X', 'static_cast', 'uint32_t', 'uint64_t', 'int32_t',
            'int64_t', 'float', 'uint8_t', 'uint16_t', 'int16_t', 'int8_t', '__builtin_expect', 'return'} | \
    {f'A1_{k}{n}{b}' for k in 'RW' for n in (8, 16, 32, 64) for b in ('', 'B')} | set(FPU_PS2.values())
MEM_RE = re.compile(r'\b(READ|WRITE)(8|16|32|64)(_B)?\(')
STORE_STMT = re.compile(r'^(?:Ps2SetGpr64\(ctx, (\d+), (sx64\((\w+)\)|(\w+))\)|ctx->f\[(\d+)\] = (\w+)|ctx->(fcr31|hi|lo|f_acc) = (\w+))$')


class Typed:
    def __init__(self, name, addr, body, abi, fx, mw=frozenset()):
        self.name, self.addr = name, addr
        self.ok, self.why = False, ''
        self.n4 = N4.Callee(name, addr, body)  # the N4 entry's interface (argument order at the sites)
        lines = body.rstrip('\n').split('\n')
        if lines and N4.GUARD.match(lines[0]):
            lines = lines[1:]
        gdecl, fdecl, xdecl, rest, head = [], [], [], [], True
        for line in lines:
            m, mf, mx = GDECL.match(line), FDECL.match(line), XDECL.match(line)
            if head and m:
                for mm in re.finditer(r'(\w+) = Ps2GprU(32|64)\(ctx, (\d+)\)', m.group(2)):
                    gdecl.append((int(mm.group(3)), m.group(1), mm.group(1)))
            elif head and mf:
                for mm in re.finditer(r'(\w+) = ctx->f\[(\d+)\]', mf.group(1)):
                    fdecl.append((int(mm.group(2)), mm.group(1)))
            elif head and mx:
                xdecl.append((mx.group(3), mx.group(1), mx.group(2)))
            elif head and OTHER_DECL.match(line):
                rest.append(line)
            else:
                head = False
                rest.append(line)
        text = '\n'.join(rest)
        for why, pat in REFUSE_PAT:
            if pat.search(text) or (why == 'vu0 / 128-bit' and pat.search(body)):
                self.why = why
                return
        if abi is None or not abi[0]:
            self.why = 'not proven'
            return
        gin = set(abi[1]) - {0, 31}
        if 29 in gin:
            self.why = 'stack (sp)'
            return
        fin = {r for k, r in fx if k == 'f'}
        xin = {r for k, r in fx if k == 'x'}
        if xin:
            self.why = 'field input ' + ','.join(sorted(xin))
            return
        if any(k == 'v' for k, r in fx):
            self.why = 'vu0 / 128-bit'
            return
        # ---- the exits: `const uint32_t target = ra;` ... stores ... `ctx->pc = target;` `return;`
        lines = text.split('\n')
        new, rets, stored_all = [], [], collections.Counter()
        cur = None  # the stores of the exit being read
        for line in lines:
            s = line.strip()
            if re.match(r'^const uint32_t target = (?:\(uint32_t\))?ra;$', s):
                cur = {}
                continue
            if cur is not None and ('Ps2SetGpr64(ctx' in s or s.startswith('ctx->f[') or re.match(r'^ctx->(fcr31|hi|lo|f_acc) = ', s)) \
                    and s != 'ctx->pc = target;':
                st = N4.split_stmts(s)
                if st is None:
                    self.why = 'exit shape'
                    return
                keep = []
                for x in st:
                    cond = re.match(r'^if \(df_\w+\) (.*)$', x)
                    mm = STORE_STMT.match((cond.group(1) if cond else x)[:-1].strip())
                    if not mm:
                        keep.append(x)
                        continue
                    if cond:  # stored only when written on the path: never a return value, a scratch slot
                        slot = (('g', int(mm.group(1))) if mm.group(1) else ('f', int(mm.group(5))) if mm.group(5)
                                else ('x', mm.group(7)))
                        cur[slot] = None
                        continue
                    if mm.group(1):
                        r = int(mm.group(1))
                        cur[('g', r)] = (mm.group(3) or mm.group(4), bool(mm.group(3)))
                    elif mm.group(5):
                        cur[('f', int(mm.group(5)))] = (mm.group(6), False)
                    else:
                        cur[('x', mm.group(7))] = (mm.group(8), False)
                if keep:
                    new.append(line[:len(line) - len(line.lstrip())] + ' '.join(keep))
                continue
            if s == 'ctx->pc = target;':
                if cur is None:
                    self.why = 'exit shape'
                    return
                continue
            if s == 'return;':
                if cur is None:
                    self.why = 'exit shape'
                    return
                rets.append(cur)
                for k in cur:
                    stored_all[k] += 1
                new.append(line.replace('return;', 'return A1_RET;'))
                cur = None
                continue
            new.append(line)
        if not rets:
            self.why = 'no return'
            return
        text = '\n'.join(new)
        if re.search(r'\bctx\b', text):
            self.why = 'ctx use'
            return
        if re.search(r'\b(rdram|runtime|target)\b', text):
            self.why = 'rdram / runtime use'
            return
        if re.search(r'\bra\b', text):
            self.why = 'ra use'
            return
        # ---- return value: v0 / f0 stored at every return, the same local and form each time
        def every(slot):  # stored at every return, the same local each time, and written on every path to them
            vals = {r.get(slot) for r in rets}
            return len(vals) == 1 and None not in vals and slot in mw
        self.ret_v0 = every(('g', 2))
        self.ret_f0 = every(('f', 0))
        gty = {r: ty for r, ty, _ in gdecl}
        gname = {r: n for r, _, n in gdecl}
        if self.ret_v0:
            loc, sx = rets[0][('g', 2)]
            if loc != gname.get(2):
                self.why = 'return shape'
                return
            self.v0_ty = 'uint32_t' if sx else 'uint64_t'
        if self.ret_f0 and rets[0][('f', 0)][0] != dict((r, n) for r, n in fdecl).get(0):
            self.why = 'return shape'
            return
        if self.ret_v0 and self.ret_f0:
            self.rtype = 'a1::RetVF'
            rexpr = 'a1::RetVF{(uint64_t)%s, %s}' % ('a1_sx64(%s)' % gname[2] if self.v0_ty == 'uint32_t' else gname[2],
                                                    dict((r, n) for r, n in fdecl)[0])
        elif self.ret_v0:
            self.rtype, rexpr = self.v0_ty, gname[2]
        elif self.ret_f0:
            self.rtype, rexpr = 'float', dict((r, n) for r, n in fdecl)[0]
        else:
            self.rtype, rexpr = 'void', ''
        text = text.replace('return A1_RET;', f'return {rexpr};' if rexpr else 'return;')
        # the slots it writes but does not return: checked at the call sites
        ret_slots = ({('g', 2)} if self.ret_v0 else set()) | ({('f', 0)} if self.ret_f0 else set())
        self.dropped = set(stored_all) - ret_slots
        # ---- memory, FPU, helpers
        def mem(m):
            return f'A1_{"R" if m.group(1) == "READ" else "W"}{m.group(2)}{"B" if m.group(3) else ""}('
        text = MEM_RE.sub(mem, text)
        text = text.replace('lift_d1::flt(', 'a1_flt(').replace('lift_d1::bits(', 'a1_bits(').replace('lift_d1::sx64(', 'sx64(')
        text = re.sub(r'\bsx64\(', 'a1_sx64(', text)
        try:
            for k, fn in FPU_PLAIN.items():
                text = msub(text, k, fn)
            for k, h in FPU_PS2.items():
                text = msub(text, k, lambda a, h=h: f'{h}({", ".join(a)})')
        except L.Refuse:
            self.why = 'parse'
            return
        self.fpu_ps2 = sorted({h for h in FPU_PS2.values() if h + '(' in text})
        self.fpu_plain = sorted(k for k in FPU_PLAIN if k in body)
        bad = set()
        for m in re.finditer(r'([A-Za-z_][\w:]*)\s*\(', text):
            if m.group(1) not in OK_CALLS and not re.match(r'^L_[0-9a-f]+:?$', m.group(1)) and not re.match(r'^[a-z]\w*$', m.group(1)):
                bad.add(m.group(1))
        # lower-case words before '(' are casts / locals in expressions like `(v0 = ...)`: a call of a lower-case
        # function would still need a declaration, which the header does not give, so the compiler refuses it
        if bad:
            self.why = 'unknown call ' + ','.join(sorted(bad))[:60]
            return
        # ---- parameters and locals
        params, locs = [], []
        self.gparams = []
        for r, ty, n in gdecl:
            if r == 31:
                continue
            if r in gin:
                params.append(f'{ty} {n}')
                self.gparams.append((r, ty, n))
            else:
                locs.append(f'{ty} {n} = 0;')
        missing = gin - {r for r, _, _ in gdecl}
        if missing:
            # an input the text never declares cannot be read by it (n4_abi's input sets are by word, lift_d1 by use):
            # nothing to pass
            pass
        self.fparams = []
        for r, n in fdecl:
            if r in fin:
                params.append(f'float {n}')
                self.fparams.append((r, n))
            else:
                locs.append(f'float {n} = 0.0f;')
        for fld, ty, n in xdecl:
            locs.append(f'{ty} {n} = 0;')
        self.uses_mem_rt = bool(re.search(r'A1_[RW]\d+B?\(', text))
        sig = ', '.join(['uint8_t *ram', '[[maybe_unused]] PS2Runtime *rt'] + params)
        self.decl = f'{self.rtype} {name}({sig})'
        body_lines = [ln for ln in text.split('\n')]
        self.text = (f'// {name} (0x{addr:x}): typed by tools/typed_lift.py (round A1)\n'
                     f'inline {self.decl}\n{{\n' + ''.join(f'    {x}\n' for x in locs) + '\n'.join(body_lines) + '\n}\n')
        self.ok = True

    # the guest-signature adapter, one line at the top of the lifted function's body
    def adapter(self):
        ld, av = [], []
        for r, ty, n in self.gparams:
            ld.append(f'const {ty} a1_{r} = Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r});')
            av.append(f'a1_{r}')
        for r, n in self.fparams:
            ld.append(f'const float a1_f{r} = ctx->f[{r}];')
            av.append(f'a1_f{r}')
        args = ''.join(', ' + x for x in av)
        call = f'typed::{self.name}(rdram, runtime{args})'
        st = self.store_ctx('a1_r')
        chk = f'A1_CHK({self.name}{args}); '
        r = '' if self.rtype == 'void' else f'const {self.rtype} a1_r = '
        return (f'    if (PS2_EXPECT(A1_ON, 1)) {{ {" ".join(ld)}{" " if ld else ""}{chk}{r}{call}; {st}'
                f'ctx->pc = Ps2GprU32(ctx, 31); return; }}{ADAPT_MARK}')

    def store_ctx(self, v):
        if self.rtype == 'void':
            return ''
        if self.rtype == 'a1::RetVF':
            return f'Ps2SetGpr64(ctx, 2, {v}.v0); ctx->f[0] = {v}.f0; '
        if self.rtype == 'float':
            return f'ctx->f[0] = {v}; '
        return f'Ps2SetGpr64(ctx, 2, {"sx64(" + v + ")" if self.rtype == "uint32_t" else v}); '


SITE = re.compile(r'^(\s*)if \(PS2_N4_FAST\((.*?), lift_d1::(\w+)\)\) \{ (.*?)const uint32_t n4_s = PS2_N4_ENTER\(\); '
                  r'(?:const uint64_t n4_r = )?lift_d1::(\w+)_n4\(rdram, ctx, runtime(.*?)\); '
                  r'if \(PS2_EXPECT\(!PS2_N4_DONE\((0x[0-9A-Fa-f]+u)\), 0\)\) \{ .*? \} else \{ PS2_N4_COUNT\(\); (.*?) \} \} else \{ .* \}'
                  + re.escape(N4.SITE_MARK) + '$')


def rewrite_site(line, T):
    m = SITE.match(line)
    if not m or m.group(3) != T.name or m.group(5) != T.name:
        return None, 'shape'
    ind, fnexpr, chks, argtxt, ret, part = m.group(1), m.group(2), m.group(4), m.group(6), m.group(7), m.group(8)
    if argtxt.strip():
        s = argtxt.strip()
        assert s.startswith(',')
        args, j = L.split_args(s[1:] + ')', 0)
    else:
        args = []
    order = [('g', r) for r, _, _ in T.n4.gin] + [('f', r) for r, _ in T.n4.fin]
    if len(order) != len(args):
        return None, 'args'
    amap = dict(zip(order, args))
    av = [amap[('g', r)] for r, _, _ in T.gparams] + [amap[('f', r)] for r, _ in T.fparams]
    # the caller's locals for v0 / f0 (from N4's read back of the slots F stores) and its dirty-flag resets
    st = N4.split_stmts(part) if part.strip() else []
    if st is None:
        return None, 'read back'
    v0loc = f0loc = None
    v0wide = False
    df = []
    for s in st:
        mm = re.match(r'^(\w+) = (\(uint32_t\))?n4_r;$', s) or re.match(r'^(\w+) = Ps2GprU(32|64)\(ctx, 2\);$', s)
        if mm:
            v0loc = mm.group(1)
            v0wide = mm.group(2) in (None, '64')  # N4's `x = n4_r` (the full stored value) or a 64-bit read back
            continue
        mm = re.match(r'^(\w+) = ctx->f\[0\];$', s)
        if mm:
            f0loc = mm.group(1)
            continue
        if re.match(r'^df_\w+ = false;$', s):
            df.append(s)
    args = ''.join(', ' + x for x in av)
    r = '' if T.rtype == 'void' else f'const {T.rtype} a1_r = '
    loc = []
    if T.rtype in ('uint32_t', 'uint64_t'):
        if v0loc:
            loc.append(f'{v0loc} = {"sx64(a1_r)" if v0wide and T.rtype == "uint32_t" else "a1_r"};')
    elif T.rtype == 'float':
        if f0loc:
            loc.append(f'{f0loc} = a1_r;')
    elif T.rtype == 'a1::RetVF':
        if v0loc:
            loc.append(f'{v0loc} = a1_r.v0;')
        if f0loc:
            loc.append(f'{f0loc} = a1_r.f0;')
    new = (f'{SITE_PRE}if (PS2_EXPECT(A1_ON, 1) && PS2_N4_FAST({fnexpr}, lift_d1::{T.name})) {{ {chks}A1_CHK({T.name}{args}); '
           f'(void)PS2_N4_ENTER(); {r}typed::{T.name}(rdram, runtime{args}); ctx->pc = {ret}; {T.store_ctx("a1_r")}'
           f'{" ".join(loc + df)}{" " if loc or df else ""}PS2_N4_COUNT(); }} else{SITE_END}')
    return ind + new + line[len(ind):], None


# ----------------------------------------------------------------------------------------------- output files
HDR = '''// typed_a1.h: round A1's typed functions (tools/typed_lift.py; docs/decomp_a1.md). Generated: do not edit.
// Real C++ functions for loop-free, ABI-proven leaves: parameters and return values, MIPS registers as locals, guest
// memory through `ram` (rt: the memory environment for scratchpad / I/O only). No R5900 context, no function table.
#pragma once
#include "typed_a1_rt.h"

namespace typed
{
#include "typed_a1.inc"
}
'''


def write_check_tu(typed, defs, path_tu, path_ref):
    """The in-run differential check (PS2X_A1_CHECK) and the offline harness's table: a copy of each converted
    function's lifted text (the reference) and its typed text, both on logged memory."""
    ref = []
    for T in typed:
        body = defs[T.name][1]
        lines = body.rstrip('\n').split('\n')
        if lines and N4.GUARD.match(lines[0]):
            lines = lines[1:]
        ref.append(f'// {T.name} (0x{T.addr:x}): lift_d1\'s text (the reference)\n'
                   f'void {T.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)\n{{\n' + '\n'.join(lines) + '\n}\n')
    with open(path_ref, 'w') as fo:
        fo.write('// typed_a1_ref.inc: round A1\'s references for the check (tools/typed_lift.py --check-tu). Generated.\n')
        fo.write('\n'.join(ref))
    out = ['// a1_typed_check.cpp: round A1\'s differential check of the typed functions (tools/typed_lift.py --check-tu;',
           '// docs/decomp_a1.md section 4). Generated: do not edit. Compiled to code only with PS2X_A1_CHECK=1 (a check build)',
           '// or A1_OFFLINE (the offline harness, tools/a1_harness.cpp).',
           '#include "typed_a1_check_rt.h"',
           '#if PS2X_A1_CHECK || defined(A1_OFFLINE)',
           'namespace a1ck',
           '{',
           'namespace ref',
           '{',
           'using lift_d1::sx64;',
           '#include "typed_a1_ref.inc"',
           '}',
           'namespace ty',
           '{',
           '#include "typed_a1.inc"',
           '}',
           '}',
           '']
    out.append('namespace a1ck\n{')
    for i, T in enumerate(typed):
        ps = ''.join(f', {ty} p{r}' for r, ty, _ in T.gparams) + ''.join(f', float pf{r}' for r, _ in T.fparams)
        av = ''.join(f', p{r}' for r, _, _ in T.gparams) + ''.join(f', pf{r}' for r, _ in T.fparams)
        kind = {'void': 0, 'uint32_t': 1, 'uint64_t': 2, 'float': 3, 'a1::RetVF': 4}[T.rtype]
        dg = sum(1 << r for k, r in T.dropped if k == 'g')
        df = sum(1 << r for k, r in T.dropped if k == 'f')
        out.append(f'void {T.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime{ps})')
        out.append('{')
        out.append(f'    if (!ckBegin({i}, rdram, runtime)) return;')
        out.append(f'    R5900Context &c = ckCtx(ctx);')
        out.append(f'    ref::{T.name}(rdram, &c, runtime);')
        out.append(f'    ckMid();')
        if T.rtype == 'void':
            out.append(f'    ty::{T.name}(rdram, runtime{av});')
            out.append(f'    ckEnd({i}, ctx, c, {kind}, 0u, 0.0f, 0x{dg:x}u, 0x{df:x}u);')
        elif T.rtype == 'a1::RetVF':
            out.append(f'    const a1::RetVF r = ty::{T.name}(rdram, runtime{av});')
            out.append(f'    ckEnd({i}, ctx, c, {kind}, r.v0, r.f0, 0x{dg:x}u, 0x{df:x}u);')
        elif T.rtype == 'float':
            out.append(f'    const float r = ty::{T.name}(rdram, runtime{av});')
            out.append(f'    ckEnd({i}, ctx, c, {kind}, 0u, r, 0x{dg:x}u, 0x{df:x}u);')
        else:
            out.append(f'    const uint64_t r = {"a1_sx64" if T.rtype == "uint32_t" else ""}(ty::{T.name}(rdram, runtime{av}));')
            out.append(f'    ckEnd({i}, ctx, c, {kind}, r, 0.0f, 0x{dg:x}u, 0x{df:x}u);')
        out.append('}')
    # the table for the log and the offline harness: name, address, a runner from a context
    out.append(f'const CkEntry kEntries[{len(typed)}] = {{')
    for T in typed:
        av = ''.join(f', ({ty})Ps2GprU{"64" if ty == "uint64_t" else "32"}(ctx, {r})' for r, ty, _ in T.gparams) + \
            ''.join(f', ctx->f[{r}]' for r, _ in T.fparams)
        out.append(f'    {{"{T.name}", 0x{T.addr:x}u, [](uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) {{ {T.name}(rdram, ctx, runtime{av}); }}}},')
    out.append('};')
    out.append(f'const uint32_t kEntryCount = {len(typed)}u;')
    out.append('}')
    out.append('#if !defined(A1_OFFLINE)')
    out.append('// in-run: an address outside main RAM read through the runtime (the same slow path as the typed code\'s)')
    out.append('static uint64_t a1ckSlowRead(uint8_t *ram, PS2Runtime *rt, uint32_t a, uint32_t n)')
    out.append('{')
    out.append('    return n == 1 ? a1m::slowRead8(ram, rt, a) : n == 2 ? a1m::slowRead16(ram, rt, a) : n == 4 ? a1m::slowRead32(ram, rt, a) : a1m::slowRead64(ram, rt, a);')
    out.append('}')
    out.append('static const bool a1ckInit = (a1ck::g_slowRead = &a1ckSlowRead, true);')
    out.append('void a1ckReport(unsigned long long frame) { a1ck::ckReport(frame); }')
    out.append('#endif')
    out.append('#endif')
    with open(path_tu, 'w') as fo:
        fo.write('\n'.join(out) + '\n')
    with open(os.path.join(os.path.dirname(path_tu), 'typed_a1_check.inc'), 'w') as fo:
        fo.write('// typed_a1_check.inc: round A1\'s check entries (tools/typed_lift.py --check-tu). Generated.\n')
        for T in typed:
            ps = ''.join(f', {ty}' for r, ty, _ in T.gparams) + ''.join(', float' for r, _ in T.fparams)
            fo.write(f'void {T.name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime{ps});\n')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='*')
    ap.add_argument('--list', default=os.path.join(REPO, 'docs/n4b_native.txt'))
    ap.add_argument('--abi', default=os.path.join(os.path.dirname(L.RUNNER), 'agent-scratch/n4b/abi.txt'))
    ap.add_argument('--cache', default=None, help='pickle of the IR facts (made when missing)')
    ap.add_argument('--jobs', type=int, default=3)
    ap.add_argument('--undo', action='store_true')
    ap.add_argument('--check', action='store_true', help='count, write nothing')
    ap.add_argument('--check-tu', action='store_true', help='also write a1_typed_check.cpp / typed_a1_ref.inc')
    ap.add_argument('--skip', default=None, help='file of names never converted (one a line)')
    ap.add_argument('--out-list', default=os.path.join(REPO, 'docs/a1_typed.txt'))
    ap.add_argument('--log', default=None)
    args = ap.parse_args()
    files = sorted(set(args.files))
    texts = {f: open(f, errors='surrogateescape').read() for f in files}
    base = {f: undo_text(t) for f, t in texts.items()}
    if args.undo:
        new = base
        stats = 'undone'
        typed = []
    else:
        want = N4.load_list(args.list)
        skip = set(x.strip() for x in open(args.skip)) if args.skip else set()
        defs, count, where = {}, collections.Counter(), {}
        for f, t in base.items():
            for m in N4.FUNC.finditer(t):
                if m.group(1) != m.group(4):
                    continue
                count[m.group(1)] += 1
                defs[m.group(1)] = (int(m.group(2), 16), m.group(5))
                where[m.group(1)] = f
        abi = load_abi(args.abi)
        facts = load_facts(args.cache, args.jobs)
        F = facts['facts']
        why = collections.Counter()
        cands = {}
        for name, (a, body) in sorted(defs.items(), key=lambda x: x[1][0]):
            if a not in want or count[name] != 1:
                continue
            if name in skip:
                why['skip list'] += 1
                continue
            if a not in F:
                why['no IR'] += 1
                continue
            T = Typed(name, a, body, abi.get(a), fpr_x_inputs(F[a]), written_at_returns(F[a]))
            if T.ok and not T.n4.ok:
                T.ok, T.why = False, 'n4 entry refused'
            if T.ok:
                cands[a] = T
            else:
                why[T.why.split(' ')[0] if T.why.startswith('unknown') else T.why] += 1
        # caller-side check of the dropped slots, whole program
        callee_inputs = {a: v[1] for a, v in abi.items() if v[0]}
        callers = collections.defaultdict(set)
        for g, f in F.items():
            for x, n in f['nodes'].items():
                if n[0] in ('jal', 'tail') and n[1] in cands:
                    callers[g].add(n[1])
        natives = L.native_addresses(set(facts['funcs']))
        cfin = fpr_inputs_all(F, natives)
        viol = collections.defaultdict(list)
        nsites = 0
        for g, tg in callers.items():
            res = live_after_sites(F[g], callee_inputs, tg, cfin)
            for x, (t, after) in res.items():
                nsites += 1
                bad = cands[t].dropped & after
                if bad:
                    viol[t].append((g, x, sorted(bad)))
        for t in list(viol):
            why['live scratch at a caller'] += 1
            cands[t].ok = False
            cands[t].why = 'live scratch at a caller: ' + '; '.join(f'{g:x}@{x:x} {b}' for g, x, b in viol[t][:3])
        typed = [T for a, T in sorted(cands.items()) if T.ok]
        tset = {T.name: T for T in typed}
        # rewrite the batches
        new, nsite, nadapt, skipped = {}, 0, 0, collections.Counter()
        for f, t in base.items():
            lines = t.split('\n')
            here = False
            # adapters: the line after the function's opening brace (after the resume guard when there is one)
            spans = []
            for m in N4.FUNC.finditer(t):
                if m.group(1) == m.group(4) and m.group(1) in tset:
                    spans.append((t.count('\n', 0, m.start()), m.group(1)))
            ins = {}
            for s, name in spans:
                k = s + 3  # comment, signature, '{'
                assert lines[s + 2] == '{', (f, name)
                if N4.GUARD.match(lines[k]):
                    k += 1
                ins[k] = tset[name].adapter()
            for k, line in enumerate(lines):
                if line.endswith(N4.SITE_MARK) and 'PS2_N4_FAST(' in line:
                    mm = re.search(r'PS2_N4_FAST\(.*?, lift_d1::(\w+)\)\)', line)
                    if mm and mm.group(1) in tset:
                        nl, err = rewrite_site(line, tset[mm.group(1)])
                        if nl is None:
                            skipped[err] += 1
                        else:
                            lines[k] = nl
                            nsite += 1
                            here = True
            out = []
            for k, line in enumerate(lines):
                if k in ins:
                    out.append(ins[k])
                    nadapt += 1
                    here = True
                out.append(line)
            t2 = '\n'.join(out)
            if here:
                assert INC_AFTER in t2, f
                t2 = t2.replace(INC_AFTER, INC_AFTER + INC_LINE, 1)
            new[f] = t2
        stats = (f'{len(want)} listed, {len(cands)} typed before the caller check, {len(typed)} converted '
                 f'(refused: {dict(why)}); {nsites} call sites checked; {nadapt} adapters, {nsite} N4 sites rewritten'
                 f'{", skipped " + str(dict(skipped)) if skipped else ""}; IR: {len(F)} functions ({len(facts["bad"])} not parsed)')
        if not args.check:
            with open(os.path.join(OV, 'typed_a1.inc'), 'w') as fo:
                fo.write('// typed_a1.inc: round A1\'s typed functions (tools/typed_lift.py). Generated: do not edit. '
                         'Included inside a namespace (typed_a1.h: namespace typed).\n')
                fo.write('\n'.join(T.text for T in typed))
            with open(os.path.join(OV, 'typed_a1.h'), 'w') as fo:
                fo.write(HDR)
            rt_kinds = collections.Counter(T.rtype for T in typed)
            with open(args.out_list, 'w') as fo:
                fo.write(f'# a1_typed.txt: round A1\'s typed functions (tools/typed_lift.py; docs/decomp_a1.md). {len(typed)} converted of '
                         f'{len(want)} listed in {os.path.relpath(args.list, REPO)}; returns {dict(rt_kinds)}\n')
                fo.write('# address name | C signature | dropped scratch slots | PS2 float helpers\n')
                for T in typed:
                    fo.write(f'{T.addr:08x} {T.name} | {T.decl} | {",".join(k + str(r) for k, r in sorted(T.dropped))} | '
                             f'{",".join(T.fpu_ps2)}\n')
            if args.check_tu:
                write_check_tu(typed, defs, os.path.join(OV, 'a1_typed_check.cpp'), os.path.join(OV, 'typed_a1_ref.inc'))
        if args.log:
            with open(args.log, 'w') as fo:
                for name, (a, body) in sorted(defs.items(), key=lambda x: x[1][0]):
                    if a in cands:
                        T = cands[a]
                        fo.write(f'{a:08x} {name} {"OK" if T.ok else "REFUSED " + T.why}\n')
    changed = 0
    for f in files:
        if new[f] != texts[f]:
            changed += 1
            if not args.check:
                with open(f, 'w', errors='surrogateescape') as fo:
                    fo.write(new[f])
    if not args.undo:
        again = {f: undo_text(new[f]) for f in files}
        badu = [f for f in files if again[f] != base[f]]
        if badu:
            print(f'typed_lift: undo does not give the input back for {badu[:3]}', file=sys.stderr)
    print(f'typed_lift: {stats}; {len(files)} files ({changed} {"would change" if args.check else "changed"})', file=sys.stderr)


if __name__ == '__main__':
    main()
