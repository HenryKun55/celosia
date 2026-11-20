#!/usr/bin/env python3
"""n4_native.py [--list docs/n4b_native.txt] [--undo] [--check] <lifted .cpp ...>: round N4b, native entries with
register arguments for loop-free, non-blocking, ABI-proven leaves, and direct native calls into them
(docs/native_n4b.md).

The set (docs/n4b_native.txt, written by --make-list): the functions in docs/n5_noloop.txt (no loop, no thread switch
anywhere below, not a native / host slot) that tools/n4_abi.py proves (inputs within the argument registers, sp, ra,
gp; callee-saved registers preserved; no runtime call, break or indirect jump) and that are leaves (depth=0: no call).

For every lifted function F of the set whose text qualifies (it reads and writes the context only as GPR / FPR /
VU0 / named-field loads and stores, makes no runtime call, has no mutable static), each lifted batch with a call into
F gets a copy of its native entry, `static inline ... F_n4(rdram, ctx, runtime, <GPR / FPR inputs as values>)`, in
one block before its first function (between N4 marker comments): F's own text with its input loads replaced by the
parameters, its resume check (ctx->pc not on the entry: the generated function) left out, and, when every return of F
stores v0, v0 returned as a value too. Everything else is F's text unchanged: the outputs are still stored into the
context, ctx->pc is still set to the return address.

Every NOCHK call site (tools/n6_nochk.py) into such an F

    ctx->pc = <F>;
    if (!PS2_CALL_GUEST[_DIRECT]_NOCHK(<F>, lift_d1::F, <ret>, ...)) { return; }
    }
    <read back every slot from the context>

becomes (one line; the read back moves into the call's block, its old line keeps a marker comment)

    if (PS2_N4_FAST(<the call's slot expression>, lift_d1::F)) { <charge the dispatch cycles>; [v0 =] F_n4(<arguments
        from the caller's locals: the values the write-back just stored, the clean locals, or the context>); <the
        call's own post-test, then read back only the slots F's text stores> } else { <the call as it was> <read back
        everything> } // n4

PS2_N4_FAST (game/overrides/n4_native.h) holds exactly when the NOCHK call's fast form would run (the slot holds F:
N3's guard for the DIRECT form, the slot itself otherwise; the host stack above runtime->m_noCheckFloor, which is the
top of the address space unless NOCHK is on) and the switch n4native is on. Then the plain call would have run F's
body with the context the write-back left; the native entry runs the same body with the same values as parameters.
The slots F does not store keep their locals: each equals the context (written back just before, or clean).

Idempotent: the N4 blocks and sites are first put back, then the list applied. --undo: back to the NOCHK forms only.
--check: count, write nothing (and verify that applying twice gives the same text). --make-list writes the list.
tools/lift_d1.py runs this tool after n3_direct.py and n6_nochk.py.
"""
import argparse
import collections
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BLOCK_BEGIN = '// N4 native entries (tools/n4_native.py; docs/native_n4b.md)\n'
BLOCK_END = '// N4 native entries end\n'
SITE_MARK = ' // n4'
RB_MARK = '// n4: read back in the call\'s block above'
FUNC = re.compile(r'^// (\w+) \((0x[0-9a-f]+)-(0x[0-9a-f]+)\)\nvoid (\w+)\(uint8_t \*rdram, R5900Context \*ctx, PS2Runtime \*runtime\)\n\{\n(.*?)^\}\n',
                  re.M | re.S)
REG = re.compile(r'^SOTC_DECOMP_LIFT_FUNCTION(?:_D\d+)?\((0x[0-9A-Fa-f]+)u, (\w+), lift_d1::(\w+), (\w+)\);$', re.M)
CALL = re.compile(r'^(\s*)if \(!PS2_CALL_GUEST_(NOCHK|DIRECT_NOCHK)\((0x[0-9A-Fa-f]+)u, lift_d1::(\w+), (0x[0-9A-Fa-f]+)u, (0x[0-9A-Fa-f]+)u, (\w+), "(\w+)"\)\) \{ return; \}$')
SITE = re.compile(r'^(\s*)if \(PS2_N4_FAST\(.*\} else \{ (if \(!PS2_CALL_GUEST_\w+\(.*?\)\) \{ return; \}) (.*) \}' + re.escape(SITE_MARK) + r'$')
GUARD = re.compile(r'^\s*if \(__builtin_expect\(ctx->pc != 0x[0-9A-Fa-f]+u, 0\)\) \{ \w+\(rdram, ctx, runtime\); return; \}$')
GDECL = re.compile(r'^\s*(uint32_t|uint64_t) (\w+ = Ps2GprU(?:32|64)\(ctx, \d+\)(?:, \w+ = Ps2GprU(?:32|64)\(ctx, \d+\))*);$')
FDECL = re.compile(r'^\s*float (\w+ = ctx->f\[\d+\](?:, \w+ = ctx->f\[\d+\])*);$')
# context uses a qualifying text may make (anything else: the function keeps the plain call)
CTX_OK = re.compile(r'Ps2GprU(?:32|64)\(ctx, \d+\)|Ps2SetGpr64\(ctx, \d+, |ctx->f\[\d+\]|ctx->vu0_vf\[\d+\]|ctx->(?:hi|lo|fcr31|vu0_q|vu0_acc|vu0_status|f_acc)\b|ctx->pc = target;')
WIDE = {'uint64_t'}


def load_list(path):
    out = {}
    for line in open(path):
        if line.startswith('#') or not line.strip():
            continue
        p = line.split()
        out[int(p[0], 16)] = p[1]
    return out


def balanced(s, i):
    """index after the balanced (...) starting at s[i] == '('"""
    d = 0
    for j in range(i, len(s)):
        if s[j] == '(':
            d += 1
        elif s[j] == ')':
            d -= 1
            if d == 0:
                return j + 1
    raise ValueError('unbalanced')


def split_stmts(line):
    """top-level statements of a one-line run of simple statements (no braces inside a statement but `if (x) S;`)"""
    out, d, cur = [], 0, ''
    for ch in line.strip():
        cur += ch
        if ch == '(':
            d += 1
        elif ch == ')':
            d -= 1
        elif ch == ';' and d == 0:
            out.append(cur.strip())
            cur = ''
    if cur.strip():
        return None
    return out


class Callee:
    """A qualifying lifted leaf: its native entry's text and interface."""

    def __init__(self, name, addr, body):
        self.name, self.addr = name, addr
        self.ok, self.why = False, ''
        lines = body.rstrip('\n').split('\n')
        if lines and GUARD.match(lines[0]):
            lines = lines[1:]
        self.gin = []  # (regno, ctype, local name)
        self.fin = []  # (fprno, local name)
        rest = []
        head = True  # the leading declarations: GPR / FPR inputs become parameters
        for line in lines:
            m, mf = GDECL.match(line), FDECL.match(line)
            if head and m:
                ty = m.group(1)
                parts = []
                for mm in re.finditer(r'(\w+) = Ps2GprU(32|64)\(ctx, (\d+)\)', m.group(2)):
                    r = int(mm.group(3))
                    self.gin.append((r, ty, mm.group(1)))
                    parts.append(f'{mm.group(1)} = n4_r{r}')
                rest.append(f'    {ty} {", ".join(parts)};')
            elif head and mf:
                parts = []
                for mm in re.finditer(r'(\w+) = ctx->f\[(\d+)\]', mf.group(1)):
                    self.fin.append((int(mm.group(2)), mm.group(1)))
                    parts.append(f'{mm.group(1)} = n4_f{mm.group(2)}')
                rest.append(f'    float {", ".join(parts)};')
            else:
                if not re.match(r'^\s*(?:uint32_t|uint64_t|float|__m128|bool|uint8_t|uint16_t|int32_t) \w+ = [^;]*;$', line):
                    head = False
                rest.append(line)
        text = '\n'.join(rest)
        if re.search(r'PS2_(?:CALL|TAIL|JUMP|CHECKPOINT)|runtime->|eeCheckpoint', text):
            self.why = 'call / runtime'
            return
        if re.search(r'\bstatic\b(?!\s+(?:const|constexpr)\b)', text):
            self.why = 'static'
            return
        if any(GUARD.match(x) for x in rest):
            self.why = 'guard'
            return
        # every remaining ctx use must be a recognised slot load / store
        if re.search(r'\bctx\b', re.sub(CTX_OK, '', text)):
            self.why = 'ctx use'
            return
        # the slots the text stores (what a caller must read back)
        self.wg = {int(x) for x in re.findall(r'Ps2SetGpr64\(ctx, (\d+), ', text)}
        self.wf = {int(x) for x in re.findall(r'ctx->f\[(\d+)\] = ', text)}
        self.wv = {int(x) for x in re.findall(r'ctx->vu0_vf\[(\d+)\] = ', text)}
        self.wx = set(re.findall(r'ctx->(\w+) = ', text)) - {'f', 'vu0_vf', 'pc'}
        # v0 as a return value when every return stores it (each return's exit block: since the last `target`)
        rets = [m.start() for m in re.finditer(r'\breturn;', text)]
        self.ret_v0 = bool(rets) and 2 in self.wg
        last = 0
        for r in rets:
            seg = text[last:r]
            k = seg.rfind('const uint32_t target')
            if k < 0 or 'Ps2SetGpr64(ctx, 2, ' not in seg[k:]:
                self.ret_v0 = False
            last = r
        if self.ret_v0:
            out, pos = [], 0
            for m in re.finditer(r'Ps2SetGpr64\(ctx, 2, ', text):
                j = balanced(text, m.start() + len('Ps2SetGpr64'))
                inner = text[m.end():j - 1]
                out.append(text[pos:m.start()] + f'Ps2SetGpr64(ctx, 2, (n4_v0 = ({inner})))')
                pos = j
            text = ''.join(out) + text[pos:]
            text = re.sub(r'\breturn;', 'return n4_v0;', text)
            text = '    uint64_t n4_v0;\n' + text
        params = ''.join(f', {ty} n4_r{r}' for r, ty, _ in self.gin) + ''.join(f', float n4_f{r}' for r, _ in self.fin)
        self.text = (f'// {name}: native entry (0x{addr:x})\n'
                     f'static inline {"uint64_t" if self.ret_v0 else "void"} {name}_n4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime{params})\n'
                     f'{{\n{text}\n}}\n')
        self.ok = True


def caller_locals(body):
    """register number -> (ctype, name) of the caller's GPR locals, fpr number -> name"""
    g, f = {}, {}
    for line in body.split('\n')[:12]:
        m = GDECL.match(line)
        if m:
            for mm in re.finditer(r'(\w+) = Ps2GprU(32|64)\(ctx, (\d+)\)', m.group(2)):
                g[int(mm.group(3))] = (m.group(1), mm.group(1))
        mf = FDECL.match(line)
        if mf:
            for mm in re.finditer(r'(\w+) = ctx->f\[(\d+)\]', mf.group(1)):
                f[int(mm.group(2))] = mm.group(1)
    return g, f


def parse_writeback(line):
    """{gpr: expr} unconditional GPR write-backs, {fpr: expr}; None when the line is not a write-back run"""
    st = split_stmts(line)
    if st is None:
        return None
    wg, wf = {}, {}
    for s in st:
        m = re.match(r'^Ps2SetGpr64\(ctx, (\d+), (.*)\);$', s)
        if m:
            wg[int(m.group(1))] = m.group(2)
            continue
        m = re.match(r'^ctx->f\[(\d+)\] = (\w+);$', s)
        if m:
            wf[int(m.group(1))] = m.group(2)
            continue
        if re.match(r'^if \(df_\w+\) Ps2SetGpr64\(ctx, \d+, .*\);$', s) or re.match(r'^ctx->[\w\[\]]+ = \w+;$', s):
            continue  # conditional or other slots: the callee reads them from the context
        return None
    return wg, wf


def parse_readback(line):
    """[(slot, stmt)] with slot ('g', n) / ('f', n) / ('v', n) / ('x', field) / ('df', name); None when unknown"""
    st = split_stmts(line)
    if st is None or not st:
        return None
    out = []
    for s in st:
        m = re.match(r'^\w+ = Ps2GprU(?:32|64)\(ctx, (\d+)\);$', s)
        if m:
            out.append((('g', int(m.group(1))), s))
            continue
        m = re.match(r'^\w+ = ctx->f\[(\d+)\];$', s)
        if m:
            out.append((('f', int(m.group(1))), s))
            continue
        m = re.match(r'^\w+ = ctx->vu0_vf\[(\d+)\];$', s)
        if m:
            out.append((('v', int(m.group(1))), s))
            continue
        m = re.match(r'^\w+ = ctx->(\w+);$', s)
        if m:
            out.append((('x', m.group(1)), s))
            continue
        m = re.match(r'^(df_\w+) = false;$', s)
        if m:
            out.append((('df', m.group(1)), s))
            continue
        return None
    return out


def undo_text(t):
    i = t.find(BLOCK_BEGIN)
    if i >= 0:
        j = t.index(BLOCK_END, i) + len(BLOCK_END)
        t = t[:i] + t[j:]
    lines = t.split('\n')
    for k, line in enumerate(lines):
        if line.endswith(SITE_MARK) and 'PS2_N4_FAST(' in line:
            m = SITE.match(line)
            if not m:
                raise SystemExit(f'n4_native: cannot undo site: {line[:120]}')
            lines[k] = m.group(1) + m.group(2)
            j = k + 1
            while lines[j].strip() != RB_MARK:
                j += 1
            ind = lines[j][:len(lines[j]) - len(lines[j].lstrip())]
            lines[j] = ind + m.group(3)
    return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='*')
    ap.add_argument('--list', default=os.path.join(REPO, 'docs/n4b_native.txt'))
    ap.add_argument('--undo', action='store_true')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--make-list', action='store_true', help='write --list from --abi and --noloop (leaves: depth=0)')
    ap.add_argument('--abi', default=None, help="tools/n4_abi.py's abi.txt (strict mode)")
    ap.add_argument('--noloop', default=os.path.join(REPO, 'docs/n5_noloop.txt'))
    ap.add_argument('--log', default=None, help='write per-callee / per-site details here')
    args = ap.parse_args()
    if args.make_list:
        proven, share = set(), {}
        for line in open(args.abi):
            p = line.split()
            if len(p) > 2 and p[2] == 'PROVEN':
                proven.add(int(p[0], 16))
                share[int(p[0], 16)] = float(line.rsplit('w=', 1)[1])
        rows = []
        for line in open(args.noloop):
            if line.startswith('#') or not line.strip():
                continue
            p = line.split()
            a = int(p[0], 16)
            if a in proven and 'depth=0' in p:
                rows.append((a, p[1], share.get(a, 0.0)))
        with open(args.list, 'w') as fo:
            fo.write('# n4b_native.txt: loop-free NOBLOCK leaves (docs/n5_noloop.txt depth=0) proven by tools/n4_abi.py (strict);\n'
                     f'# tools/n4_native.py --make-list; docs/native_n4b.md. {len(rows)} functions, profile share (S21 riding, % of the game thread) '
                     f'{sum(r[2] for r in rows):.2f}\n')
            for a, n, w in rows:
                fo.write(f'{a:08x} {n} w={w:.4f}\n')
        print(f'n4_native: {len(rows)} functions in {args.list}', file=sys.stderr)
        return
    files = sorted(set(args.files))
    texts = {f: open(f, errors='surrogateescape').read() for f in files}
    base = {f: undo_text(t) for f, t in texts.items()}
    if args.undo:
        new = base
        stats = 'undone'
    else:
        want = load_list(args.list)
        # the lifted functions of all files: name -> (addr, body); a name defined twice is left out
        defs, count = {}, collections.Counter()
        for f, t in base.items():
            for m in FUNC.finditer(t):
                if m.group(1) != m.group(4):
                    continue
                count[m.group(1)] += 1
                defs[m.group(1)] = (int(m.group(2), 16), m.group(5))
        callees, why = {}, collections.Counter()
        for name, (a, body) in defs.items():
            if a not in want or count[name] != 1:
                continue
            c = Callee(name, a, body)
            if c.ok:
                callees[name] = c
            else:
                why[c.why] += 1
        new, nsites, skipped, used = {}, 0, collections.Counter(), set()
        log = []
        for f, t in base.items():
            lines = t.split('\n')
            # caller bodies: line index -> caller locals
            spans = []
            for m in FUNC.finditer(t):
                s = t.count('\n', 0, m.start())
                spans.append((s, s + m.group(0).count('\n'), m.group(5), m.group(1)))
            here = set()
            si = 0
            for k, line in enumerate(lines):
                m = CALL.match(line)
                if not m or m.group(4) not in callees:
                    continue
                c = callees[m.group(4)]
                while si < len(spans) and spans[si][1] < k:
                    si += 1
                if si >= len(spans) or not (spans[si][0] <= k <= spans[si][1]):
                    skipped['outside a function'] += 1
                    continue
                if int(m.group(3), 16) != c.addr:
                    skipped['target'] += 1
                    continue
                if lines[k - 1].strip() != f'ctx->pc = {m.group(3)}u;' or lines[k + 1].strip() != '}':
                    skipped['shape'] += 1
                    continue
                rb = parse_readback(lines[k + 2])
                if rb is None:
                    skipped['read back'] += 1
                    continue
                wb = parse_writeback(lines[k - 2]) if 'Ps2SetGpr64(ctx' in lines[k - 2] or 'ctx->f[' in lines[k - 2] else ({}, {})
                if wb is None:
                    skipped['write back'] += 1
                    continue
                wg, wf = wb
                gl, fl = caller_locals(spans[si][2])
                argv, chk = [], []
                ret = m.group(5) + 'u'
                for r, ty, _ in c.gin:
                    cx = f'Ps2GprU{"64" if ty in WIDE else "32"}(ctx, {r})'
                    if r in wg:
                        argv.append(wg[r])
                    elif r in gl and (gl[r][0] in WIDE or ty not in WIDE):
                        argv.append(gl[r][1])
                    else:
                        argv.append(cx)
                        continue
                    chk.append(f'PS2_N4_ARG({ty}, {argv[-1]}, {cx}, {ret});')
                for r, _ in c.fin:
                    argv.append(wf[r] if r in wf else fl.get(r, f'ctx->f[{r}]'))
                    if r in wf or r in fl:
                        chk.append(f'PS2_N4_ARG(float, {argv[-1]}, ctx->f[{r}], {ret});')
                full = lines[k + 2].strip()
                part, keep = [], []
                for (kind, x), s in rb:
                    if kind == 'df' or (kind == 'g' and x in c.wg) or (kind == 'f' and x in c.wf) or \
                            (kind == 'v' and x in c.wv) or (kind == 'x' and x in c.wx):
                        if c.ret_v0 and kind == 'g' and x == 2:
                            mm = re.match(r'^(\w+) = Ps2GprU(32|64)\(ctx, 2\);$', s)
                            s = f'{mm.group(1)} = {"(uint32_t)" if mm.group(2) == "32" else ""}n4_r;'
                        part.append(s)
                    else:
                        mm = re.match(r'^(\w+) = (.*);$', s)
                        keep.append(f'PS2_N4_KEEP({mm.group(1)}, {mm.group(2)}, {ret});')
                ind = m.group(1)
                fnexpr = f'PS2_GUEST_FN({m.group(3)}u)' if m.group(2) == 'NOCHK' else f'PS2_N3_FN({m.group(3)}u, lift_d1::{c.name})'
                call = f'lift_d1::{c.name}_n4(rdram, ctx, runtime{"".join(", " + a for a in argv)})'
                lines[k] = (f'{ind}if (PS2_N4_FAST({fnexpr}, lift_d1::{c.name})) {{ {" ".join(chk) + " " if chk else ""}const uint32_t n4_s = PS2_N4_ENTER(); '
                            f'{"const uint64_t n4_r = " if c.ret_v0 else ""}{call}; '
                            f'if (PS2_EXPECT(!PS2_N4_DONE({ret}), 0)) {{ if (!runtime->noCheckFinish(ctx, {m.group(3)}u, {ret}, n4_s)) {{ return; }} {full} }} '
                            f'else {{ PS2_N4_COUNT(); {" ".join(part + keep)} }} }} else {{ {line.strip()} {full} }}{SITE_MARK}')
                lines[k + 2] = lines[k + 2][:len(lines[k + 2]) - len(lines[k + 2].lstrip())] + RB_MARK
                nsites += 1
                here.add(c.name)
                used.add(c.name)
                log.append(f'site {os.path.basename(f)}:{k + 1} {spans[si][3]} -> {c.name} args {len(argv)} read back {len(part)} of {len(rb)}')
            t2 = '\n'.join(lines)
            if here:
                mf = FUNC.search(t2)
                blk = BLOCK_BEGIN + ''.join(callees[n].text for n in sorted(here, key=lambda n: callees[n].addr)) + BLOCK_END
                t2 = t2[:mf.start()] + blk + t2[mf.start():]
            new[f] = t2
        stats = (f'{len(want)} listed, {len(callees)} lifted with a qualifying text (not: {dict(why)}), {nsites} sites to '
                 f'{len(used)} callees{", skipped " + str(dict(skipped)) if skipped else ""}')
        if args.log:
            with open(args.log, 'w') as fo:
                for n in sorted(callees, key=lambda n: callees[n].addr):
                    c = callees[n]
                    fo.write(f'callee {c.addr:08x} {n} in {len(c.gin)}+{len(c.fin)}f stores g{sorted(c.wg)} f{sorted(c.wf)} '
                             f'x{sorted(c.wx)} ret_v0 {int(c.ret_v0)} used {int(n in used)}\n')
                fo.write('\n'.join(log) + '\n')
    changed = 0
    for f in files:
        if new[f] != texts[f]:
            changed += 1
            if not args.check:
                with open(f, 'w', errors='surrogateescape') as fo:
                    fo.write(new[f])
    if args.check and not args.undo:
        again = {f: undo_text(new[f]) for f in files}
        bad = [f for f in files if again[f] != base[f]]
        if bad:
            print(f'n4_native: undo does not give the input back for {bad[:3]}', file=sys.stderr)
    print(f'n4_native: {stats}; {len(files)} files ({changed} {"would change" if args.check else "changed"})', file=sys.stderr)


if __name__ == '__main__':
    main()
