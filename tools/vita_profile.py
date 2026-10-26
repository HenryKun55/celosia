#!/usr/bin/env python3
"""Names a Vita3K profile (VITA3K_PROFILE, "thread pc lr weight" lines).

pc / lr are Vita addresses of build/vita/ps2xRuntime/ps2EntryRunner (loaded
at its own addresses, 0x81000000): each is put in the function that holds it
(symbols from arm-vita-eabi-nm, demangled). Prints the top functions by
self weight, with their main callers, and the split by thread.

Usage: tools/vita_profile.py <profile> [--top 40] [--elf path] [--thread N | --busiest] [--minus <profile>]
  --thread N: only that thread's samples; --busiest: only the busiest
  thread's (the game thread when it is the bottleneck).
  --minus <profile>: an earlier copy of the same run's profile (tools/vita3k.sh
  PROF_SNAPS) subtracted, sample by sample: the window between the two.
"""
import bisect
import os
import subprocess
import sys
from collections import Counter, defaultdict

ROOT = os.path.join(os.path.dirname(__file__), '..')


def symbols(elf):
    nm = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')
    out = subprocess.run([nm, '-n', '-C', '--defined-only', elf], capture_output=True, text=True).stdout
    addrs, names = [], []
    for line in out.splitlines():
        p = line.split(' ', 2)
        if len(p) < 3 or p[1] not in 'tTwW':
            continue
        addrs.append(int(p[0], 16) & ~1)
        names.append(p[2])
    return addrs, names


def main():
    path = sys.argv[1]
    top = int(sys.argv[sys.argv.index('--top') + 1]) if '--top' in sys.argv else 40
    elf = sys.argv[sys.argv.index('--elf') + 1] if '--elf' in sys.argv else os.path.join(ROOT, 'build/vita/ps2xRuntime/ps2EntryRunner')
    addrs, names = symbols(elf)
    only = int(sys.argv[sys.argv.index('--thread') + 1]) if '--thread' in sys.argv else None
    base = {}
    if '--minus' in sys.argv:
        for line in open(sys.argv[sys.argv.index('--minus') + 1]):
            p = line.split()
            if len(p) == 4:
                base[(p[0], p[1])] = int(p[3])

    def rows():
        for line in open(path):
            p = line.split()
            if len(p) == 4:
                w = int(p[3]) - base.get((p[0], p[1]), 0)
                if w > 0:
                    yield int(p[0]), int(p[1], 16), int(p[2], 16), w

    if '--busiest' in sys.argv:
        weights = Counter()
        for thread, _, _, w in rows():
            weights[thread] += w
        only = weights.most_common(1)[0][0]

    def name(a):
        i = bisect.bisect_right(addrs, a & ~1) - 1
        return names[i] if i >= 0 else '?%08x' % a

    total = 0
    self_w = Counter()
    callers = defaultdict(Counter)
    threads = Counter()
    for thread, pc, lr, w in rows():
        if only is not None and thread != only:
            continue
        fn = name(pc)
        self_w[fn] += w
        callers[fn][name(lr) if lr else '-'] += w
        threads[thread] += w
        total += w
    print(f'total weight {total}')
    for t, w in threads.most_common(8):
        print(f'  thread {t}: {100.0 * w / total:.1f}%')
    for fn, w in self_w.most_common(top):
        by = ', '.join(f'{c[:40]} {100.0 * k / w:.0f}%' for c, k in callers[fn].most_common(2))
        print(f'{100.0 * w / total:6.2f}%  {fn[:90]}  ({by})')


if __name__ == '__main__':
    main()
