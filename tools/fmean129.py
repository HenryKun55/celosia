#!/usr/bin/env python3
"""fmean129.py err.txt from to: mean frame ms and sched ms of [frame] each / slow lines in game frames from..to (round 129's overhead A/B)."""
import re, sys
f, lo, hi = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
ms = []; sched = []
for l in open(f, errors='replace'):
    m = re.match(r'\[frame\] (?:each|slow) (\d+): ([\d.]+) ms', l)
    if not m or not lo <= int(m.group(1)) < hi:
        continue
    ms.append(float(m.group(2)))
    s = re.search(r'\| sched ([\d.]+)', l)
    if s:
        sched.append(float(s.group(1)))
print(f'{f}: {len(ms)} frames, mean {sum(ms) / max(len(ms), 1):.2f} ms, sched {sum(sched) / max(len(sched), 1):.2f} ms, busy {sum(ms) / max(len(ms), 1) - sum(sched) / max(len(sched), 1):.2f}')
