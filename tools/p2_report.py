#!/usr/bin/env python3
"""p2_report.py err.txt [err2.txt ...] --elf <elf> [options]: round P2's console profiler (docs/decomp_p2.md) as a ranked
table of functions and subsystems by PMU event: which code of the game thread takes the L1 D / I refills and the stall
cycles.

Input: the console's err.txt of one or more runs with SOTC_P2PMUSAMPLE=<event>:<period> (one event per run: give the four
runs together for the four columns) and / or SOTC_P2BRACKET=1, plus the shipped ELF of the same build.

  --elf ELF        the ELF of the VPK the console ran (build/agent-scratch/p2/celosia-p2.elf)
  --windows A-B    only [p2] windows A..B of each run (default: all)
  --top N          functions listed (default 50)
  --callers N      for the top functions, their N most frequent callers (the user lr at the sample; meaningful for leaf
                   functions and for samples outside the eboot: system modules, the kernel) (default 3)
  --sort EVENT     rank functions by this event's share (0x03, 0x01, 0x61, 0x60...; default: the first run's)
  --group PATTERN  list the functions of the subsystems matching PATTERN (regex)
  --raw            key by pc bucket (no symbols)

Columns: '% <event>' = the function's share of that event's samples on the game thread; '/frame' = samples x period /
frames (events a frame). Subsystem grouping: tools/prof129.py's (round 113's areas). Samples whose pc lies outside the
eboot's text (the kernel during a syscall, SceLibKernel / SceGxm / libc modules) are charged to the function of their lr
(the caller) when the pair table has it, else listed as '(outside text)'.
"""
import argparse, bisect, os, re, subprocess, sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from prof129 import NM, classify, short  # noqa: E402

EVNAME = {0x01: 'icache miss', 0x03: 'dcache miss', 0x60: 'icache stall cyc', 0x61: 'dcache stall cyc', 0x11: 'cycles', 0x68: 'instructions'}


def load_elf(elf):
    out = subprocess.run([NM, '-n', '-C', '--defined-only', elf], capture_output=True, text=True).stdout
    A, N = [], []
    anchor = None
    for l in out.splitlines():
        p = l.split(' ', 2)
        if len(p) < 3 or p[1] not in 'tTwW':
            continue
        a = int(p[0], 16) & ~1
        A.append(a); N.append(p[2])
        if anchor is None and re.match(r'p2::anchor\(', p[2]):
            anchor = a
    return A, N, anchor


def rng(s):
    a, b = s.split('-')
    return int(a), int(b)


def parse(path, args):
    run = {'path': path, 'anchor': None, 'event': None, 'period': None, 'text': (0, 0), 'wins': [], 'probe': None, 'start': None,
           'stop': None, 'p2b': defaultdict(lambda: {'calls': 0.0, 'early': 0, 'incl': [0.0] * 7, 'self': [0.0] * 7, 'n': 0}), 'p2bcost': None,
           'p2bwins': set(), 'p2bevents': None}
    cur = None
    for l in open(path, errors='replace'):
        i = l.find('[p2] ')
        if i >= 0:
            s = l[i + 5:].strip()
            p = s.split()
            if not p:
                continue
            if p[0] == 'anchor':
                run['anchor'] = int(p[1], 16)
                m = re.search(r'text ([0-9a-f]+)-([0-9a-f]+)', s)
                if m:
                    run['text'] = (int(m.group(1), 16), int(m.group(2), 16))
                m = re.search(r'every (\d+) events \(0x([0-9a-f]+)\)', s)
                if m:
                    run['period'] = int(m.group(1)); run['event'] = int(m.group(2), 16)
            elif p[0] == 'probe':
                run['probe'] = s
            elif p[0] in ('start:', 'stopped:'):
                run['start' if p[0] == 'start:' else 'stop'] = s
            elif p[0] == 'w':
                m = re.match(r'w (\d+) frames (\d+)-(\d+) \(([\d.]+) ms/frame wall\): event 0x([0-9a-f]+) .*? period (\d+): samples (\d+).*?kernel (\d+), outside text (\d+), irqs (\d+), other thread (\d+), mismatch (\d+), spurious (\d+), dropped (\d+)', s)
                if not m:
                    continue
                g = m.groups()
                cur = {'w': int(g[0]), 'f0': int(g[1]), 'f1': int(g[2]), 'ms': float(g[3]), 'event': int(g[4], 16), 'period': int(g[5]),
                       'samples': int(g[6]), 'kernel': int(g[7]), 'outside': int(g[8]), 'irqs': int(g[9]), 'other': int(g[10]),
                       'mismatch': int(g[11]), 'spurious': int(g[12]), 'dropped': int(g[13]), 'h': Counter(), 'c': Counter()}
                if args.windows and not (rng(args.windows)[0] <= cur['w'] <= rng(args.windows)[1]):
                    cur = None
                    continue
                run['wins'].append(cur)
            elif p[0] == 'h' and cur is not None:
                for it in p[1:]:
                    a, _, n = it.partition(':')
                    cur['h'][int(a, 16)] += int(n)
            elif p[0] == 'c' and cur is not None:
                for it in p[1:]:
                    f = it.split(':')
                    if len(f) == 3:
                        cur['c'][(int(f[0], 16), int(f[1], 16))] += int(f[2])
            continue
        i = l.find('[p2b] ')
        if i >= 0:
            s = l[i + 6:].strip()
            m = re.match(r'read cost .*?: ([\d. ]+) cyc ([\d.]+) \(events ([0-9a-f ]+)\)', s)
            if m:
                v = [float(x) for x in m.group(1).split()] + [float(m.group(2))]
                run['p2bcost'] = v
                run['p2bevents'] = [int(x, 16) for x in m.group(3).split()]
                continue
            m = re.match(r'w (\d+) frames (\d+)-(\d+): (\S+) calls/f ([\d.]+) early (\d+) \| incl ([\d. ]+) \| self ([\d. ]+)', s)
            if m:
                w = int(m.group(1))
                if args.windows and not (rng(args.windows)[0] <= w <= rng(args.windows)[1]):
                    continue
                nf = int(m.group(3)) - int(m.group(2))
                d = run['p2b'][m.group(4)]
                d['calls'] += float(m.group(5)) * nf
                d['early'] += int(m.group(6))
                inc = [float(x) for x in m.group(7).split()]; sel = [float(x) for x in m.group(8).split()[:7]]
                for k in range(7):
                    d['incl'][k] += inc[k] * nf; d['self'][k] += sel[k] * nf
                d['n'] += nf
                run['p2bwins'].add((w, nf))
    return run


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('err', nargs='+'); ap.add_argument('--elf', required=True)
    ap.add_argument('--windows'); ap.add_argument('--top', type=int, default=50); ap.add_argument('--callers', type=int, default=3)
    ap.add_argument('--sort'); ap.add_argument('--group'); ap.add_argument('--raw', action='store_true')
    args = ap.parse_args()
    A, N, eanchor = load_elf(args.elf)
    if eanchor is None:
        sys.exit('no p2::anchor() in the ELF (a build without round P2?)')
    runs = [parse(p, args) for p in args.err]

    namecache = {}

    def fname(addr):
        if args.raw:
            return f'{addr:08x}', True
        if addr in namecache:
            return namecache[addr]
        i = bisect.bisect_right(A, addr) - 1
        n = N[i] if i >= 0 else '?'
        m = re.match(r'(.+?)_0x([0-9a-f]+)(\(|$)', n)
        r = (f'{m.group(1)} @{m.group(2)}', False) if m else (short(n), True)
        namecache[addr] = r
        return r

    def group(key, nat):
        return classify(key.split(' @')[0], nat)

    cols = []  # (event, period, fn Counter, grp Counter, total samples, frames)
    for r in runs:
        print(f'== {r["path"]}')
        if r['probe']:
            print(f'  [p2] {r["probe"][:400]}')
        if r['start']:
            print(f'  [p2] {r["start"][:300]}')
        if r['stop']:
            print(f'  [p2] {r["stop"][:300]}')
        if not r['wins']:
            print('  no [p2] windows' + (' (sampling never started: see the probe / start lines)' if r['anchor'] is None else ''))
            continue
        slide = r['anchor'] - eanchor
        lo, hi = r['text']
        ev, per = r['event'], r['period']
        frames = sum(w['f1'] - w['f0'] for w in r['wins'])
        tot = sum(w['samples'] for w in r['wins'])
        ms = sum(w['ms'] * (w['f1'] - w['f0']) for w in r['wins']) / max(frames, 1)
        print(f'  event 0x{ev:02x} ({EVNAME.get(ev, "event")}) period {per}: windows {len(r["wins"])}, frames {frames} ({ms:.1f} ms/frame wall), samples {tot} '
              f'= {tot * per / max(frames, 1):.0f} events/frame; kernel {sum(w["kernel"] for w in r["wins"])}, outside text {sum(w["outside"] for w in r["wins"])}, '
              f'other-thread irqs {sum(w["other"] for w in r["wins"])}, mismatch {sum(w["mismatch"] for w in r["wins"])}, dropped {sum(w["dropped"] for w in r["wins"])}; '
              f'load slide {slide:+#x}')
        fn = Counter(); nat = {}; grp = Counter(); gfn = defaultdict(Counter); callers = defaultdict(Counter)
        pairs = Counter()
        for w in r['wins']:
            pairs.update(w['c'])
        # outside-text buckets: split by caller (pairs), the rest unattributed
        byc = defaultdict(Counter)
        for (pc, lr), n in pairs.items():
            byc[pc][lr] += n
        for w in r['wins']:
            for pc, n in w['h'].items():
                inside = lo <= pc < hi
                if inside:
                    k, nt = fname(pc - slide)
                    fn[k] += n; nat[k] = nt
                    g = group(k, nt)
                    grp[g] += n; gfn[g][k] += n
                    continue
                where = '(kernel)' if pc < 0x40000000 else '(system module)'
                left = n
                cs = byc.get(pc)
                if cs:
                    tw = sum(cs.values())
                    for lr, c in cs.items():
                        share = n * c / tw
                        if lo <= lr < hi:
                            k, nt = fname((lr & ~1) - slide)
                            kk = f'{where} <- {k}'
                            fn[kk] += share; nat[kk] = nt
                            g = group(k, nt)
                            grp[g] += share; gfn[g][kk] += share
                            left -= share
                if left > 1e-9:
                    fn[where] += left; nat[where] = False
                    grp[where] += left; gfn[where][where] += left
        for (pc, lr), n in pairs.items():
            if lo <= pc < hi and lo <= lr < hi:
                k, _ = fname(pc - slide)
                c, _ = fname((lr & ~1) - slide)
                if c != k:
                    callers[k][c] += n
        cols.append({'ev': ev, 'per': per, 'fn': fn, 'nat': nat, 'grp': grp, 'gfn': gfn, 'tot': tot, 'frames': frames, 'callers': callers})

    if cols:
        evs = [c['ev'] for c in cols]
        hdr = ''.join(f' {"%" + EVNAME.get(e, hex(e)):>18s} {"/frame":>9s}' for e in evs)
        print('\n== subsystems (share of each event\'s samples on the game thread; events a frame)')
        print(f'  {"subsystem":44s}{hdr}')
        keys = set()
        for c in cols:
            keys |= set(c['grp'])
        first = cols[0]
        for k in sorted(keys, key=lambda k: -first['grp'][k] / max(first['tot'], 1)):
            row = ''
            for c in cols:
                v = c['grp'][k]
                row += f' {100 * v / max(c["tot"], 1):18.2f} {v * c["per"] / max(c["frames"], 1):9.0f}'
            print(f'  {k[:44]:44s}{row}')
        if args.sort:
            sk = int(args.sort, 0)
            base = next((c for c in cols if c['ev'] == sk), first)
        else:
            base = first
        allk = Counter()
        for c in cols:
            for k, v in c['fn'].items():
                allk[k] += v / max(c['tot'], 1) * (4.0 if c is base else 1.0)
        print(f'\n== top functions (ranked by {EVNAME.get(base["ev"], hex(base["ev"]))}; % of each event\'s samples)')
        print(f'  {"function":80s} {"subsystem":26s}{hdr}')
        nat = {}
        for c in cols:
            nat.update(c['nat'])
        order = sorted(allk, key=lambda k: -(base['fn'][k] / max(base['tot'], 1) + 1e-6 * allk[k]))
        for k in order[:args.top]:
            row = ''
            for c in cols:
                v = c['fn'][k]
                row += f' {100 * v / max(c["tot"], 1):18.2f} {v * c["per"] / max(c["frames"], 1):9.0f}'
            g = group(k.split(' <- ')[-1], nat.get(k, False)) if not k.startswith('(') or ' <- ' in k else k
            print(f'  {k[:80]:80s} {g[:26]:26s}{row}')
            if args.callers and k in base['callers']:
                cs = base['callers'][k]
                tw = sum(cs.values())
                print('      callers (lr): ' + ', '.join(f'{100 * n / tw:.0f}% {cc[:50]}' for cc, n in cs.most_common(args.callers)))
        if args.group:
            for c in cols:
                for g in sorted(c['gfn']):
                    if re.search(args.group, g):
                        print(f'\n{g} ({EVNAME.get(c["ev"], hex(c["ev"]))}):')
                        for k, v in c['gfn'][g].most_common(args.top):
                            print(f'  {100 * v / max(c["tot"], 1):6.2f} {k}')

    for r in runs:
        if not r['p2b']:
            continue
        cost = r['p2bcost'] or [0.0] * 7
        evn = r['p2bevents'] or []
        names = [EVNAME.get(e, hex(e)) for e in evn] + ['cycles']
        print(f'\n== {r["path"]}: counter brackets (per frame; one K2 read subtracted per call: {", ".join(f"{x:.0f}" for x in cost)})')
        print(f'  {"bracket":44s} {"calls/f":>8s} ' + ' '.join(f'{n[:16]:>16s}' for n in names) + '   (inclusive, then self)')
        rows = []
        for name, d in r['p2b'].items():
            if not d['n']:
                continue
            nf = d['n']
            cpf = d['calls'] / nf
            inc = [max(0.0, d['incl'][k] / nf - cpf * cost[k]) for k in range(7)]
            sel = [max(0.0, d['self'][k] / nf - cpf * cost[k]) for k in range(7)]
            rows.append((name, cpf, inc, sel))
        for name, cpf, inc, sel in sorted(rows, key=lambda x: -x[3][6]):
            print(f'  {name[:44]:44s} {cpf:8.1f} ' + ' '.join(f'{v:16.0f}' for v in inc))
            print(f'  {"":44s} {"self":>8s} ' + ' '.join(f'{v:16.0f}' for v in sel))
        sub = defaultdict(lambda: [0.0] * 7)
        for name, cpf, inc, sel in rows:
            for k in range(7):
                sub[name.split(':')[0]][k] += sel[k]
        print('  by subsystem (sum of self):')
        for s, v in sorted(sub.items(), key=lambda x: -x[1][6]):
            print(f'  {s[:44]:44s} {"":8s} ' + ' '.join(f'{x:16.0f}' for x in v))


if __name__ == '__main__':
    main()
