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


def facts(a):
    try:
        lf = L.Lifter(a, {})
        lf.build()
        nodes = lf.node_effects()
    except Exception as e:  # Refuse / parse limits: unproven
        return a, None, f'{type(e).__name__}: {str(e)[:60]}'
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
        gu = {r for kk, r in (n['uses'] | n.get('duses', set())) if kk == 'g' and r}
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
    return a, {'name': lf.g.name, 'order': order, 'cfg': cfg, 'calls': calls, 'why': why, 'saves': psaved,
               'restores': rest, 'loops': loops}, None


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
            reads |= (ci if ci is not None else ALL) - w - ({31} if kind == 'jal' else set())
        if kind == 'jalr' or (kind == 'jr' and tgt is None and x in cfg and cfg[x][0] == 'jr'):
            pass
        after = w | gd | dd
        if kind == 'jal':
            after |= {31}
        for y in succ:
            e = (w | gd) if (kind == 'br' and likely and y != btgt) else after
            old = wr.get(y)
            new = frozenset(e) if old is None else (old & frozenset(e))
            if old is None or new != old:
                wr[y] = new
                work.append(y)
    if 'jalr' in f['why'] or 'ijump' in f['why'] or 'runtime' in f['why']:
        reads |= ALL  # an unknown callee / runtime may read anything not written
    reads.discard(0)
    return reads


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='.')
    ap.add_argument('--profile', default=None)
    ap.add_argument('--jobs', type=int, default=2)
    args = ap.parse_args()
    tab = L.table()
    funcs = sorted(a for a, c in tab.items() if re.search(r'_0x([0-9a-f]+)$', c) and int(re.search(r'_0x([0-9a-f]+)$', c).group(1), 16) == a)
    with Pool(args.jobs) as p:
        res = p.map(facts, funcs, chunksize=32)
    F = {a: f for a, f, e in res if f is not None}
    bad = {a: e for a, f, e in res if f is None}
    natives = L.native_addresses(set(funcs))
    # whole-program fixpoints: inputs (union, grows) and outputs (function_gpr_writes)
    inp = {a: set() for a in F}
    outs = {a: set() for a in F}
    for a in natives:
        inp.pop(a, None)
    changed, passes = True, 0
    while changed and passes < 50:
        changed, passes = False, passes + 1
        for a, f in F.items():
            if a in natives:
                continue
            r = inputs_of(f, inp)
            if r != inp[a]:
                inp[a] = r
                changed = True
    wmap = {}
    for a in F:
        wmap[a] = set(ALL)
    changed, passes2 = True, 0
    while changed and passes2 < 50:
        changed, passes2 = False, passes2 + 1
        for a, f in F.items():
            ff = {'order': f['order'], 'restores': f['restores'],
                  'cfg': {x: (c[0], c[1], c[6], c[7], c[5], tuple(sorted(c[3])), tuple(sorted(c[4]))) for x, c in f['cfg'].items()}}
            wm = {t: (wmap.get(t, ALL) if t not in natives else ALL) for t in f['calls']}
            g = L.function_gpr_writes(ff, wm)
            if 'jalr' in f['why'] or 'runtime' in f['why'] or 'ijump' in f['why']:
                g = set(ALL)
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
        if a in natives:
            continue
        i = inp[a]
        ins_ok = i <= (ARGS | {28, 29, 31} | f['saves'] | (SAVED & set()))
        # a callee-saved register only read to be saved counts as saved (its prologue sd reads it)
        ins_ok = (i - f['saves']) <= (ARGS | {28, 29, 31})
        o = wmap[a]
        saved_ok = not (o & (SAVED | {29}))
        callees_ok = all(t in F and t not in natives for t in f['calls'])
        unknown = bool(f['why'])
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
        rows.append((a, f['name'], proven, sorted(i), sorted(o), sorted(f['why']), len(f['calls']), f['loops'], w))
    os.makedirs(args.out, exist_ok=True)
    with open(os.path.join(args.out, 'abi.txt'), 'w') as fo:
        fo.write(f'# {len(funcs)} table functions: {len(F)} analysed, {len(bad)} not parsed, {len(natives)} may hold a native; '
                 f'inputs {passes} passes, outputs {passes2} passes\n')
        fo.write(f'# counts: {cnt}\n# profile share (ride7, % of the game thread): ' +
                 ', '.join(f'{k} {v:.2f}' for k, v in wsum.items()) + f', all analysed {total_w:.2f}\n')
        for r in sorted(rows):
            fo.write(f'{r[0]:08x} {r[1]} {"PROVEN" if r[2] else "no"} in={",".join(L.GPR[x] for x in r[3])} '
                     f'out={",".join(L.GPR[x] for x in r[4])} why={",".join(r[5])} calls={r[6]} loops={int(r[7])} w={r[8]:.4f}\n')
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
    with open(os.path.join(args.out, 'abi.txt'), 'a') as fo:
        fo.write(f'# lifted call-boundary traffic (static): {tr}\n')
    print(open(os.path.join(args.out, 'abi.txt')).readline(), cnt, wsum, total_w, tr)


if __name__ == '__main__':
    main()
