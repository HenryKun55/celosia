#!/usr/bin/env python3
"""n4_abi.py: round N4's ABI proof over the whole program (docs/native_n4.md).

For every function in the recompiled function table (build/runner, through tools/lift_d1.py's Lifter: the same CFG,
delay slots and per-instruction register uses / defs the lifter works from) it states, from the MIPS alone:

  - inputs: the GPRs read before any write on some path from the entry (a call reads what its callee reads: the
    callee's own inputs when it is proven, every register otherwise);
  - callee-saved: whether s0-s7 / fp / gp / sp come back unchanged at every return (written only between a prologue
    save and its restore, sp back to its entry value: lift_d1's find_saves / restores), given the callees' results;
  - outputs: the GPRs that may be changed at a return (function_gpr_writes with the callees' results);
  - what can make it unwind (a thread switch or a checkpoint resuming through the context): calls (every call's dispatch
    has a clock checkpoint), loop checkpoints, runtime calls (syscall / break / COP0), indirect calls / jumps.

ABI-conforming ("proven") = inputs within a0-a3, t0-t3 (EABI's eight argument registers), sp, ra, gp and callee-saved
registers it only saves; outputs within v0, v1, the temporaries and ra; callee-saved preserved; every callee proven.

It also counts the call-boundary traffic of the lifted files (game/overrides/decomp_lift*.cpp): per guest call, the
context write-backs before it and the read-backs after it, by register class.

Usage: n4_abi.py [--out dir] [--profile syms.txt] [--jobs 2]
"""
import argparse
import glob
import os
import re
import sys
from multiprocessing import Pool

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lift_d1 as L  # noqa: E402

ARGS = set(range(4, 12))
TEMPS = {1, 2, 3} | set(range(4, 16)) | {24, 25}
SAVED = set(range(16, 24)) | {28, 30}
ALL = set(range(1, 32))
NATIVES_ABI = [False]


def word_uses(w):
    """The GPRs a MIPS / R5900 instruction reads, from its word (the lifter's per-statement uses over-approximate: a
    narrow destination counts as read). Unknown encodings read rs and rt (safe: more inputs, never fewer)."""
    op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
    if w == 0:
        return set()
    if op == 0:
        if fn in (0, 2, 3, 0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F):
            r = {rt}
        elif fn in (0x10, 0x12, 0x0C, 0x0D, 0x0F):
            r = set()
        elif fn in (8, 9, 0x11, 0x13):
            r = {rs}
        elif fn in (0x0A, 0x0B):
            r = {rs, rt, rd}
        else:
            r = {rs, rt}
    elif op in (2, 3, 0x0F):
        r = set()
    elif op in (1, 6, 7, 0x16, 0x17) or op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x18, 0x19) or \
            op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x1A, 0x1B, 0x37, 0x1E, 0x31, 0x36, 0x2F, 0x33, 0x39, 0x3E):
        r = {rs}
    elif op in (0x10, 0x11, 0x12):
        r = {rt} if rs in (4, 5, 6) else set()
    else:
        r = {rs, rt}
    r.discard(0)
    return r


def facts(a):
    try:
        lf = L.Lifter(a, {})
        lf.build()
        nodes = lf.node_effects()
    except Exception as e:  # Refuse / parse limits: unproven
        return a, None, f'{type(e).__name__}: {str(e)[:60]}'
    return facts_from(a, lf, nodes)


def facts_from(a, lf, nodes):
    """facts() on a Lifter already built (A5: tools/typed_lift2.py's typed ABI proof reuses the Emit's own Lifter)."""
    order, succ = lf.order, lf.succ
    calls, why = [], set()
    cfg = {}
    for x in order:
        n = nodes[x]
        k = n['kind']
        if k[0] == 'op' and n['opaque']:
            why.add('runtime')
        if x + 4 in lf.brk:
            why.add('break')
        if k[0] == 'jalr':
            why.add('jalr')
        if k[0] == 'jr' and not k[2]:
            why.add('ijump')
        if k[0] in ('jal', 'tail'):
            calls.append(k[1])
        W = lf.g.words
        gu = set(word_uses(W[x])) | (word_uses(W[x + 4]) if x + 4 in lf.delay else set())
        if k[0] == 'ret':
            gu.add(31)
        gd = {r for kk, r in n['defs'] if kk == 'g' and r}
        dd = {r for kk, r in n.get('ddefs', set()) if kk == 'g' and r}
        cfg[x] = (k[0], k[1] if k[0] in ('jal', 'tail') else None, gu, gd, dd, tuple(succ[x]), bool(k[0] == 'br' and k[5]),
                  k[4] if k[0] == 'br' else None, bool(n['kind'][0] == 'op' and n['opaque']))
    try:
        saves = lf.find_saves()
    except Exception:
        saves = {}
    first_ctl = min([x for x in order if lf.kind[x][0] != 'op'] + [lf.g.end])
    psaved = {r for r, (o, sz, sx) in saves.items() if sx < first_ctl}
    rest = {x: r for x, (r, sz) in lf.restores.items() if r in psaved}
    loops = any(s <= x for x in order for s in succ[x])
    # sp back to its entry value at every return / tail jump: only addiu sp, sp, imm changes it, the same offset on
    # every path (find_saves' pass), offset 0 after the return's delay slot
    W = lf.g.words
    spoff, sp_ok = {order[0]: 0}, True
    work = [order[0]]
    while work and sp_ok:
        x = work.pop()
        o = spoff[x]
        for y in ((x, x + 4) if x + 4 in lf.delay else (x,)):
            w = W[y]
            if (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29:
                o += L.sx16(w & 0xFFFF)
            elif 29 in lf.gpr_defs_raw(lf.raw.get(y, '')):
                sp_ok = False
        if lf.kind[x][0] in ('ret', 'tail') and o != 0:
            sp_ok = False
        for z in succ[x]:
            if z in spoff:
                if spoff[z] != o:
                    sp_ok = False
            else:
                spoff[z] = o
                work.append(z)
    return a, {'name': lf.g.name, 'order': order, 'cfg': cfg, 'calls': calls, 'why': why, 'saves': psaved,
               'restores': rest, 'loops': loops, 'sp_ok': sp_ok}, None


def inputs_of(f, inp):
    """Forward must-written analysis; a use of a register not yet written on some path is an input."""
    cfg, order = f['cfg'], f['order']
    wr = {order[0]: frozenset()}
    work = [order[0]]
    reads = set()
    while work:
        x = work.pop()
        kind, tgt, gu, gd, dd, succ, likely, btgt, opq = cfg[x]
        w = set(wr[x])
        if opq:
            reads |= ALL - w
        reads |= gu - w
        if kind in ('jal', 'tail'):
            ci = inp.get(tgt)
            if ci is None:
                ci = (ARGS | {28, 29, 31}) if NATIVES_ABI[0] else ALL
            reads |= ci - w - ({31} if kind == 'jal' else set())
        if kind == 'jalr' and NATIVES_ABI[0]:
            reads |= (ARGS | {28, 29}) - w
        after = w | gd | dd
        if kind == 'jal':
            after |= {31}
        if kind in ('jal', 'jalr') and NATIVES_ABI[0]:
            after |= TEMPS  # ABI mode: a call leaves the temporaries (v0 / v1 its result) written
        for y in succ:
            e = (w | gd) if (kind == 'br' and likely and y != btgt) else after
            old = wr.get(y)
            new = frozenset(e) if old is None else (old & frozenset(e))
            if old is None or new != old:
                wr[y] = new
                work.append(y)
    if ('jalr' in f['why'] and not NATIVES_ABI[0]) or 'ijump' in f['why'] or 'runtime' in f['why']:
        reads |= ALL  # an unknown callee / runtime may read anything not written
    reads.discard(0)
    return reads


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='.')
    ap.add_argument('--profile', default=None)
    ap.add_argument('--jobs', type=int, default=2)
    ap.add_argument('--natives-abi', action='store_true',
                    help='a native / hook / unparsed callee is taken to follow the ABI (inputs a0-t3, sp, ra, gp; '
                         'outputs the temporaries, v0 / v1, ra) instead of reading / writing everything')
    ap.add_argument('--all', action='store_true',
                    help='A2: also write the functions whose slot may hold a native (their own proof; marked native=1)')
    args = ap.parse_args()
    NATIVES_ABI[0] = args.natives_abi
    tab = L.table()
    funcs = sorted(a for a, c in tab.items() if re.search(r'_0x([0-9a-f]+)$', c) and int(re.search(r'_0x([0-9a-f]+)$', c).group(1), 16) == a)
    with Pool(args.jobs) as p:
        res = p.map(facts, funcs, chunksize=32)
    F = {a: f for a, f, e in res if f is not None}
    bad = {a: e for a, f, e in res if f is None}
    natives = L.native_addresses(set(funcs))
    # whole-program fixpoints: inputs (union, grows) and outputs (function_gpr_writes)
    inp = {a: set() for a in F}
    for a in natives:
        inp.pop(a, None)
    ABI_IN, ABI_OUT = ARGS | {28, 29, 31}, TEMPS | {31}
    if args.natives_abi:
        for a in funcs:
            if a in natives or a not in F:
                inp[a] = set(ABI_IN)
    changed, passes = True, 0
    while changed and passes < 50:
        changed, passes = False, passes + 1
        for a, f in F.items():
            if a in natives:
                continue
            # a callee-saved register read by the prologue's save is not an input of the callers
            r = inputs_of(f, inp) - f['saves']
            if r != inp[a]:
                inp[a] = r
                changed = True
    if args.all:  # A2: the natives' own inputs, from their callees' (they stay unknown to their callers)
        for a in natives:
            if a in F:
                inp[a] = inputs_of(F[a], inp) - F[a]['saves']
    wmap = {}
    for a in F:
        wmap[a] = set(ALL)
    changed, passes2 = True, 0
    while changed and passes2 < 50:
        changed, passes2 = False, passes2 + 1
        for a, f in F.items():
            ff = {'order': f['order'], 'restores': f['restores'],
                  'cfg': {x: (c[0], c[1], c[6], c[7], c[5], tuple(sorted(c[3])), tuple(sorted(c[4]))) for x, c in f['cfg'].items()}}
            if args.natives_abi:
                wm = {t: (wmap[t] if t in wmap and t not in natives else ABI_OUT) for t in f['calls']}
            else:
                wm = {t: (wmap.get(t, ALL) if t not in natives else ALL) for t in f['calls']}
            g = L.function_gpr_writes(ff, wm)
            if 'runtime' in f['why'] or 'ijump' in f['why'] or ('jalr' in f['why'] and not args.natives_abi):
                g = set(ALL)
            elif 'jalr' in f['why']:
                g = set(g) | TEMPS | {31}
            if g != wmap[a]:
                changed = True
                wmap[a] = g
    prof = {}
    if args.profile:
        for line in open(args.profile, errors='replace'):
            m = re.match(r'\s*([\d.]+)%\s+(.*)', line)
            if m:
                prof[m.group(2).strip()] = prof.get(m.group(2).strip(), 0.0) + float(m.group(1))
    def share(a):
        f = F.get(a)
        if not f:
            return 0.0
        n = f['name']
        s = 0.0
        for k, v in prof.items():
            if k.startswith(f'lift_d1::{n}(') or k.startswith(f'{tab[a]}('):
                s += v
        return s
    rows = []
    cnt = {'proven': 0, 'inputs_ok': 0, 'saved_ok': 0, 'leaf': 0, 'leaf_proven': 0, 'no_unwind': 0}
    wsum = {k: 0.0 for k in cnt}
    total_w = 0.0
    for a, f in F.items():
        if a in natives and not args.all:
            continue
        i = inp[a]
        ins_ok = i <= (ARGS | {28, 29, 31} | f['saves'] | (SAVED & set()))
        # a callee-saved register only read to be saved counts as saved (its prologue sd reads it)
        ins_ok = (i - f['saves']) <= (ARGS | {28, 29, 31})
        o = wmap[a]
        saved_ok = not (o & SAVED) and f['sp_ok']
        callees_ok = args.natives_abi or all(t in F and t not in natives for t in f['calls'])
        unknown = bool(f['why'] - ({'jalr'} if args.natives_abi else set()))
        proven = ins_ok and saved_ok and callees_ok and not unknown
        leaf = not f['calls'] and not unknown
        no_unwind = leaf and not f['loops']
        w = share(a)
        total_w += w
        for k, v in (('proven', proven), ('inputs_ok', ins_ok), ('saved_ok', saved_ok), ('leaf', leaf),
                     ('leaf_proven', leaf and proven), ('no_unwind', no_unwind and proven)):
            if v:
                cnt[k] += 1
                wsum[k] += w
        rows.append((a, f['name'], proven, sorted(i), sorted(o), sorted(f['why']), len(f['calls']), f['loops'], w, a in natives))
    os.makedirs(args.out, exist_ok=True)
    tag = ('abi_nat' if args.natives_abi else 'abi') + ('_all' if args.all else '')
    with open(os.path.join(args.out, tag + '.txt'), 'w') as fo:
        fo.write(f'# {len(funcs)} table functions: {len(F)} analysed, {len(bad)} not parsed, {len(natives)} may hold a native; '
                 f'inputs {passes} passes, outputs {passes2} passes\n')
        fo.write(f'# counts: {cnt}\n# profile share (ride7, % of the game thread): ' +
                 ', '.join(f'{k} {v:.2f}' for k, v in wsum.items()) + f', all analysed {total_w:.2f}\n')
        for r in sorted(rows):
            fo.write(f'{r[0]:08x} {r[1]} {"PROVEN" if r[2] else "no"} in={",".join(L.GPR[x] for x in r[3])} '
                     f'out={",".join(L.GPR[x] for x in r[4])} why={",".join(r[5])} calls={r[6]} loops={int(r[7])} w={r[8]:.4f}'
                     + (f' native={int(r[9])}' if args.all else '') + '\n')
        for a, e in sorted(bad.items()):
            fo.write(f'{a:08x} NOTPARSED {e}\n')
    # call-boundary traffic in the lifted files
    tr = {'sites': 0, 'wb': 0, 'wb_temp': 0, 'wb_arg': 0, 'wb_saved': 0, 'wb_sp_ra': 0, 'rb': 0, 'rb_saved': 0}
    for fn in sorted(glob.glob(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'game/overrides/decomp_lift*.cpp'))):
        lines = open(fn, errors='replace').read().split('\n')
        for i, line in enumerate(lines):
            if 'PS2_CALL_GUEST_KNOWN(' not in line and 'PS2_CALL_GUEST(' not in line:
                continue
            tr['sites'] += 1
            pre = lines[i - 2] if i >= 2 else ''
            for m in re.finditer(r'Ps2SetGpr64\(ctx, (\d+),', pre):
                r = int(m.group(1))
                tr['wb'] += 1
                if r in (29, 31):
                    tr['wb_sp_ra'] += 1
                elif r in SAVED:
                    tr['wb_saved'] += 1
                elif r in ARGS:
                    tr['wb_arg'] += 1
                else:
                    tr['wb_temp'] += 1
            post = lines[i + 2] if i + 2 < len(lines) else ''
            for m in re.finditer(r'= Ps2GprU(?:32|64)\(ctx, (\d+)\)', post):
                tr['rb'] += 1
                if int(m.group(1)) in SAVED | {29}:
                    tr['rb_saved'] += 1
    with open(os.path.join(args.out, tag + '.txt'), 'a') as fo:
        fo.write(f'# lifted call-boundary traffic (static): {tr}\n')
    print(open(os.path.join(args.out, tag + '.txt')).readline(), cnt, wsum, total_w, tr)


if __name__ == '__main__':
    main()
