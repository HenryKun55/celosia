#!/usr/bin/env python3
"""Decomp progress over time: docs/decomp_history.json -> docs/decomp_history.svg.

Each entry is one measurement:
  code      % of the MIPS code (by size) that is native (tools/decomp_status.py)
  calls     % of guest calls that land in native code (same tool)
  time      % of the Vita game thread's instructions in native code (decomp
            and overrides) rather than recompiled MIPS or the runtime, from a
            Vita3K profile (VITA3K_PROFILE snapshots, 222/240 MHz route,
            180-300 s): see --profile below
  generated % of the game thread's instructions still in recompiled MIPS

Usage:
  tools/decomp_history.py                         redraw the SVG
  tools/decomp_history.py --add --label r36 [--status docs/decomp_status.json]
                          [--profile LATE EARLY ELF THREAD]
                          append an entry (status figures from the JSON,
                          time figures from a profile window), then redraw
"""
import argparse
import bisect
import datetime
import html
import json
import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
HISTORY = os.path.join(ROOT, 'docs/decomp_history.json')
SVG = os.path.join(ROOT, 'docs/decomp_history.svg')
GUEST = '(unsigned char*, R5900Context*, PS2Runtime*)'

# (key, label, light colour, dark colour): categorical slots 1-3.
SERIES = [
    ('time', 'Game-thread time native', '#2a78d6', '#3987e5'),
    ('calls', 'Guest calls native', '#eb6834', '#d95926'),
    ('code', 'MIPS code native (size)', '#1baf7a', '#199e70'),
]


def profile_share(late, early, elf, thread):
    """Game-thread instructions by kind between two Vita3K profile snapshots."""
    nm = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')
    out = subprocess.run([nm, '-n', '-C', '-S', '--defined-only', elf], capture_output=True, text=True).stdout
    syms = []
    for line in out.splitlines():
        p = line.split(' ', 3)
        if len(p) == 4 and p[2] in 'tTwW':
            syms.append((int(p[0], 16) & ~1, int(p[1], 16), p[3]))
    syms.sort()
    starts = [s[0] for s in syms]

    def name(a):
        i = bisect.bisect_right(starts, a & ~1) - 1
        if i >= 0 and a < syms[i][0] + max(syms[i][1], 2):
            return syms[i][2]
        return '?'

    def load(path):
        d = {}
        for line in open(path):
            q = line.split()
            if len(q) == 4 and q[0] == thread:
                d[q[1]] = d.get(q[1], 0) + int(q[3])
        return d

    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from decomp_tree import classify  # same split as the dashboard's treemaps
    a, b = load(late), load(early)
    kinds = {'native': 0, 'recompiled': 0, 'runtime': 0}
    for pc, v in a.items():
        d = v - b.get(pc, 0)
        if d <= 0:
            continue
        n = name(int(pc, 16))
        kinds[classify(n)[0] if n != '?' else 'runtime'] += d
    total = sum(kinds.values()) or 1
    return {k: round(100.0 * v / total, 1) for k, v in kinds.items()}


def draw(entries, path):
    W, H = 760, 340
    L, R, T, B = 52, 230, 40, 58
    pw, ph = W - L - R, H - T - B
    n = len(entries)

    def x(i):
        return L + (pw * i / (n - 1) if n > 1 else pw / 2)

    def y(v):
        return T + ph * (1 - v / 100.0)

    css = ['svg{background:#fcfcfb}', '.t1{fill:#0b0b0b}', '.t2{fill:#52514e}', '.g{stroke:#e4e3df}', '.ax{stroke:#a3a29c}',
           '.ring{stroke:#fcfcfb}']
    dark = ['svg{background:#1a1a19}', '.t1{fill:#f0efec}', '.t2{fill:#c3c2b7}', '.g{stroke:#383835}', '.ax{stroke:#6b6a65}',
            '.ring{stroke:#1a1a19}']
    for i, (_, _, light, dk) in enumerate(SERIES):
        css += ['.s%d{stroke:%s;fill:%s}' % (i, light, light)]
        dark += ['.s%d{stroke:%s;fill:%s}' % (i, dk, dk)]
    p = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d" '
         'font-family="-apple-system, Helvetica, Arial, sans-serif" font-size="12">' % (W, H, W, H),
         '<title>Decomp progress over time</title>',
         '<style>%s @media (prefers-color-scheme: dark){%s}</style>' % (' '.join(css), ' '.join(dark)),
         '<rect width="%d" height="%d" style="fill:none"/>' % (W, H),
         '<text class="t1" x="%d" y="22" font-size="15" font-weight="600">Decomp progress over time (%%)</text>' % L]
    for v in (0, 25, 50, 75, 100):
        p.append('<line class="g" x1="%d" x2="%d" y1="%.1f" y2="%.1f" stroke-width="1"/>' % (L, L + pw, y(v), y(v)))
        p.append('<text class="t2" x="%d" y="%.1f" text-anchor="end" dy="4">%d%%</text>' % (L - 8, y(v), v))
    p.append('<line class="ax" x1="%d" x2="%d" y1="%.1f" y2="%.1f" stroke-width="1"/>' % (L, L + pw, y(0), y(0)))
    # x labels: first, last, and every entry with a round label, thinned to fit
    step = max(1, -(-n // 6))
    for i, e in enumerate(entries):
        if i == n - 1 or (i % step == 0 and n - 1 - i >= step / 2):
            p.append('<text class="t2" x="%.1f" y="%d" text-anchor="middle">%s</text>' % (x(i), T + ph + 18, html.escape(e.get('label', ''))))
            p.append('<text class="t2" x="%.1f" y="%d" text-anchor="middle" font-size="10">%s</text>' % (x(i), T + ph + 32, e['date'][5:]))
    ends = []
    for si, (key, label, _, _) in enumerate(SERIES):
        pts = [(x(i), y(e[key])) for i, e in enumerate(entries) if e.get(key) is not None]
        if not pts:
            continue
        if len(pts) > 1:
            p.append('<polyline class="s%d" fill="none" style="fill:none" stroke-width="2" stroke-linejoin="round" points="%s"/>'
                     % (si, ' '.join('%.1f,%.1f' % q for q in pts)))
        for qx, qy in pts:
            p.append('<circle class="s%d ring" cx="%.1f" cy="%.1f" r="4" stroke-width="2"/>' % (si, qx, qy))
        last = [e for e in entries if e.get(key) is not None][-1]
        ends.append([pts[-1][1], si, '%s: %.1f%%' % (label, last[key])])
    # direct labels at the right, nudged apart
    ends.sort()
    for i in range(1, len(ends)):
        if ends[i][0] - ends[i - 1][0] < 16:
            ends[i][0] = ends[i - 1][0] + 16
    for ly, si, text in ends:
        p.append('<rect class="s%d" x="%d" y="%.1f" width="10" height="3" rx="1.5"/>' % (si, L + pw + 12, ly - 1.5))
        p.append('<text class="t1" x="%d" y="%.1f" dy="4">%s</text>' % (L + pw + 28, ly, html.escape(text)))
    p.append('<text class="t2" x="%d" y="%d" font-size="10">Calls: a larger call sample from 2026-10-27 on. Time: Vita3K profile of the game thread.</text>'
             % (L, H - 6))
    p.append('</svg>')
    with open(path, 'w') as f:
        f.write('\n'.join(p) + '\n')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--add', action='store_true')
    ap.add_argument('--label', default='')
    ap.add_argument('--status', default=os.path.join(ROOT, 'docs/decomp_status.json'))
    ap.add_argument('--profile', nargs=4, metavar=('LATE', 'EARLY', 'ELF', 'THREAD'))
    args = ap.parse_args()
    data = json.load(open(HISTORY))
    if args.add:
        s = json.load(open(args.status))
        commit = subprocess.run(['git', '-C', ROOT, 'rev-parse', '--short', 'HEAD'], capture_output=True, text=True).stdout.strip()
        e = {'date': datetime.date.today().isoformat(), 'label': args.label, 'commit': commit,
             'code': s['summary']['by_code_size']['percent'], 'calls': s['summary'].get('by_calls', {}).get('percent')}
        if args.profile:
            share = profile_share(*args.profile)
            e['time'] = share['native']
            e['generated'] = share['recompiled']
            e['runtime'] = share['runtime']
        data['entries'].append(e)
        with open(HISTORY, 'w') as f:
            json.dump(data, f, indent=1)
            f.write('\n')
    draw(data['entries'], SVG)
    last = data['entries'][-1]
    print('entries %d, last %s' % (len(data['entries']), json.dumps(last)))


if __name__ == '__main__':
    main()
