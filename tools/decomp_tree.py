#!/usr/bin/env python3
"""What is native and what is still recompiled MIPS, by area of the game:
docs/decomp_tree.json, the rows the status dashboard draws as treemaps.

Two weights per area:
  bytes  MIPS code size (docs/decomp_status.json, tools/decomp_status.py's count)
  time   % of the Vita game thread's instructions, from a Vita3K profile window
         (VITA3K_PROFILE snapshots), split native / recompiled / runtime

Usage:
  tools/decomp_tree.py --profile LATE EARLY ELF THREAD [--label rNN]
"""
import argparse
import bisect
import datetime
import json
import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from decomp_status import area_of  # noqa: E402

GUEST = '(unsigned char*, R5900Context*, PS2Runtime*)'
RUNTIME = 'Runtime (DMA walk, memory, scheduler)'


def symbols(elf):
    nm = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')
    out = subprocess.run([nm, '-n', '-C', '-S', '--defined-only', elf], capture_output=True, text=True).stdout
    syms = []
    for line in out.splitlines():
        p = line.split(' ', 3)
        if len(p) == 4 and p[2] in 'tTwW':
            syms.append((int(p[0], 16) & ~1, int(p[1], 16), p[3]))
    syms.sort()
    return syms


def classify(name):
    """(kind, area) of a game-thread symbol."""
    head = name.split('(')[0]
    m = re.match(r'(?:lean_|leant_)?(\w+?)_0x([0-9a-f]+)$', head)
    if m and name.endswith(GUEST) and '::' not in head:
        return 'recompiled', area_of(int(m.group(2), 16), m.group(1))
    ns = re.match(r'(decomp_\w+)::', head)
    if ns:
        area = area_of(0x1000000, head, ns.group(1) + '.cpp')
        return 'native', area if area != 'Other game code' else area_of(0x1000000, head.split('::')[-1])
    if head.startswith('sotc_native::'):
        return 'native', 'Display and models'
    return 'runtime', RUNTIME


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--profile', nargs=4, required=True, metavar=('LATE', 'EARLY', 'ELF', 'THREAD'))
    ap.add_argument('--label', default='')
    ap.add_argument('--status', default=os.path.join(ROOT, 'docs/decomp_status.json'))
    ap.add_argument('--out', default=os.path.join(ROOT, 'docs/decomp_tree.json'))
    args = ap.parse_args()
    late, early, elf, thread = args.profile

    syms = symbols(elf)
    starts = [s[0] for s in syms]

    def name(a):
        i = bisect.bisect_right(starts, a & ~1) - 1
        if i >= 0 and a < syms[i][0] + max(syms[i][1], 2):
            return syms[i][2]
        return None

    def load(path):
        d = {}
        for line in open(path):
            q = line.split()
            if len(q) == 4 and q[0] == thread:
                d[q[1]] = d.get(q[1], 0) + int(q[3])
        return d

    a, b = load(late), load(early)
    time = {}
    for pc, v in a.items():
        d = v - b.get(pc, 0)
        if d <= 0:
            continue
        n = name(int(pc, 16))
        key = classify(n) if n else ('runtime', RUNTIME)
        time[key] = time.get(key, 0) + d
    total = sum(time.values()) or 1

    status = json.load(open(args.status))
    rows = []
    for area, s in status['areas'].items():
        for kind, nbytes, nfuncs in (('native', s['native_bytes'], s['native_functions']),
                                     ('recompiled', s['bytes'] - s['native_bytes'], s['functions'] - s['native_functions'])):
            rows.append({'area': area, 'kind': kind, 'bytes': nbytes, 'functions': nfuncs,
                         'time': round(100.0 * time.pop((kind, area), 0) / total, 2)})
    for (kind, area), v in sorted(time.items()):
        rows.append({'area': area, 'kind': kind, 'bytes': 0, 'functions': 0, 'time': round(100.0 * v / total, 2)})
    out = {'updated': datetime.date.today().isoformat(), 'label': args.label, 'status_commit': status.get('commit'),
           'note': 'bytes: MIPS code size by area (decomp_status.json); time: % of the Vita game thread instructions (Vita3K profile)',
           'rows': rows}
    with open(args.out, 'w') as f:
        json.dump(out, f, indent=1)
        f.write('\n')
    for kind in ('native', 'recompiled', 'runtime'):
        print('%-10s time %5.1f%%' % (kind, sum(r['time'] for r in rows if r['kind'] == kind)))
    for r in sorted(rows, key=lambda r: -r['time'])[:12]:
        print('  %-40s %-10s %5.2f%%  %7d bytes' % (r['area'], r['kind'], r['time'], r['bytes']))


if __name__ == '__main__':
    main()
