#!/usr/bin/env python3
"""frametime.py <err.txt> --seed N [--from F] [--shift S]: the frame-time side of an SOTC_AB_MIX run (period 1).
Per state (off / on, the state of frame f taken from the hash of f + shift): frames, mean frame ms, the share of frames
by VBlank count (the [frame] line's "vb N"), the mean game-thread wait ("wait") and render-thread ms ("render").
With --scan: the paired off - on frame difference (300-frame blocks) for shifts -3..3 (only the right alignment shows the
saving: S3's finding)."""
import re, sys, math, collections
a = sys.argv[1:]
def opt(name, d):
    if name in a:
        i = a.index(name); v = a[i + 1]; del a[i:i + 2]; return v
    return d
seed, lo, shift = int(opt('--seed', 7)), int(opt('--from', 3000)), int(opt('--shift', -1))
scan = '--scan' in a
M = (1 << 64) - 1
def off(f):
    z = (f * 0x9E3779B97F4A7C15 + seed * 0xBF58476D1CE4E5B9 + 0x94D049BB133111EB) & M
    z = ((z ^ (z >> 30)) * 0xBF58476D1CE4E5B9) & M
    z = ((z ^ (z >> 27)) * 0x94D049BB133111EB) & M
    z ^= z >> 31
    return (z >> 63) != 0
rx = re.compile(r'^\[frame\] (?:each|slow) (\d+): ([\d.]+) ms: .*? render ([\d.]+) present ([\d.]+) wait ([\d.]+) .*? vb (\d+) dt ([\d.]+)')
rows = []
for line in open([x for x in a if not x.startswith('--')][0], errors='replace'):
    m = rx.match(line)
    if m and int(m.group(1)) >= lo and float(m.group(2)) < 200:
        rows.append((int(m.group(1)), float(m.group(2)), float(m.group(3)), float(m.group(5)), int(m.group(6)), float(m.group(7))))


def paired(sh):
    blocks = collections.defaultdict(lambda: [[], []])
    for f, ms, *_ in rows:
        blocks[f // 300][0 if off(f + sh) else 1].append(ms)
    d, w = [], []
    for b, (x, y) in blocks.items():
        if x and y:
            d.append(sum(x) / len(x) - sum(y) / len(y)); w.append(min(len(x), len(y)))
    W = sum(w)
    mu = sum(di * wi for di, wi in zip(d, w)) / W
    var = sum(wi * (di - mu) ** 2 for di, wi in zip(d, w)) / W
    return mu, 2 * math.sqrt(var / max(len(d) - 1, 1))


if scan:
    for sh in range(-3, 4):
        mu, se2 = paired(sh)
        print(f'shift {sh:+d}: frame off - on = {mu:+.3f} ms +- {se2:.3f} (2 SE)')
for name, st in (('off', True), ('on', False)):
    sel = [r for r in rows if off(r[0] + shift) == st]
    n = len(sel)
    vb = collections.Counter(r[4] for r in sel)
    print(f'{name}: {n} frames, frame {sum(r[1] for r in sel) / n:.2f} ms, render {sum(r[2] for r in sel) / n:.2f}, wait {sum(r[3] for r in sel) / n:.2f}, '
          f'dt {sum(r[5] for r in sel) / n:.2f}; vb ' + ', '.join(f'{k}: {100 * v / n:.1f}%' for k, v in sorted(vb.items())))
mu, se2 = paired(shift)
print(f'paired (shift {shift:+d}): frame off - on = {mu:+.3f} ms +- {se2:.3f} (2 SE)')
