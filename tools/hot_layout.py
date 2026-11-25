#!/usr/bin/env python3
"""The game thread's hot code in a Vita ELF's layout (round M1; round 121 / 137 measured the same with a scratch script).

Usage: tools/hot_layout.py <target.elf> <profile>:<elf> [<profile>:<elf> ...] [--order <vita_hot_order.ld>] [--thread all]
  <profile>: a VITA3K_PROFILE file ("thread pc lr weight" lines, tools/vita3k.sh PROF=), <elf> the build it was taken with
  (samples are named there, then the names looked up in <target.elf>: the same code relinked in another order compares).
  The game thread is the profile's thread with the most samples (--thread all: every thread).
For each profile: the functions holding 50% / 90% of the thread's samples, and the distinct 4 KB / 64 KB / 1 MB pages
their code covers in <target.elf>, and the span (lowest to highest byte). Veneers are left out (named alike, they
cannot be found by name; their share in the profile's own build is printed). --order: also the size of the order file's
cluster in <target.elf> (the listed functions found there, their sizes summed, and the address range they occupy).
"""
import bisect
import os
import re
import subprocess
import sys
from collections import Counter

NM = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')


def symbols(elf):
    out = subprocess.run([NM, '-n', '-S', '--defined-only', elf], capture_output=True, text=True).stdout
    addrs, names, sizes = [], [], []
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in 'tTwW':
            addrs.append(int(p[0], 16) & ~1)
            sizes.append(int(p[1], 16))
            names.append(p[3])
    return addrs, names, sizes


def pages(spans, shift):
    s = set()
    for a, n in spans:
        for p in range(a >> shift, ((a + max(n, 1) - 1) >> shift) + 1):
            s.add(p)
    return len(s)


def main():
    args = sys.argv[1:]
    order = None
    allthreads = False
    if '--order' in args:
        i = args.index('--order')
        order = args[i + 1]
        del args[i:i + 2]
    if '--thread' in args:
        i = args.index('--thread')
        allthreads = args[i + 1] == 'all'
        del args[i:i + 2]
    target, specs = args[0], args[1:]
    _, tn, ts = symbols(target)
    taddr = {}
    tsize = {}
    ta, _, _ = symbols(target)
    for a, n, s in zip(ta, tn, ts):
        taddr.setdefault(n, a)
        tsize.setdefault(n, s)
    cache = {}
    for spec in specs:
        prof, elf = spec.rsplit(':', 1)
        if elf not in cache:
            cache[elf] = symbols(elf)
        addrs, names, sizes = cache[elf]
        rows = []
        per_thread = Counter()
        for line in open(prof):
            p = line.split()
            if len(p) != 4:
                continue
            rows.append((p[0], int(p[1], 16) & ~1, int(p[3])))
            per_thread[p[0]] += int(p[3])
        game = per_thread.most_common(1)[0][0] if per_thread else None
        w = Counter()
        for t, pc, wt in rows:
            if not allthreads and t != game:
                continue
            i = bisect.bisect_right(addrs, pc) - 1
            if i < 0 or pc >= addrs[i] + max(sizes[i], 2):
                continue
            w[names[i]] += wt
        tot = sum(w.values()) or 1
        print(f'{os.path.basename(os.path.dirname(prof)) or prof}: thread {"all" if allthreads else game}, {len(w)} functions')
        for mark in (0.5, 0.9):
            cw = 0
            spans = []
            missing = 0.0
            veneers = 0.0
            for k, v in w.most_common():
                if cw / tot >= mark:
                    break
                cw += v
                if k.endswith('_veneer'):
                    veneers += v / tot  # several veneers share a name: not placed by name (where they sit: the target's own)
                elif k in taddr:
                    spans.append((taddr[k], tsize[k]))
                else:
                    missing += v / tot
            lo = min(a for a, _ in spans)
            hi = max(a + n for a, n in spans)
            print(f'  {int(mark * 100)}%: {len(spans)} functions, {pages(spans, 12)} 4 KB / {pages(spans, 16)} 64 KB / '
                  f'{pages(spans, 20)} 1 MB pages, span {(hi - lo) / 1048576:.1f} MB'
                  + (f' ({missing * 100:.1f}% of the samples in functions not in the target)' if missing else '')
                  + (f' (veneers {veneers * 100:.2f}% of the samples in the profile\'s build, left out)' if veneers else ''))
    if order:
        listed = re.findall(r'\*\(\.text\.(\S+) ', open(order).read())
        spans = sorted((taddr[k], tsize[k]) for k in listed if k in taddr)
        # The cluster: from the first listed function on, until a gap of more than 1 MB (a name the order lists can also
        # be a same-named local function elsewhere, or a part GCC split out to .text.unlikely).
        lo = spans[0][0]
        hi = spans[0][0] + spans[0][1]
        inside = 0
        for a, n in spans:
            if a > hi + (1 << 20):
                break
            hi = max(hi, a + n)
            inside += 1
        print(f'order {order}: {len(listed)} listed, {len(spans)} in the target, {sum(n for _, n in spans) / 1024:.0f} KB of code; '
              f'cluster 0x{lo:08x}-0x{hi:08x} ({(hi - lo) / 1048576:.2f} MB, {inside} of them; the other {len(spans) - inside} beyond)')


if __name__ == '__main__':
    main()
