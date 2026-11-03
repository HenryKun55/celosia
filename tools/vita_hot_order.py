#!/usr/bin/env python3
"""Writes the Vita build's hot-code order (round 66) from Vita3K profiles.

Every function a profile sampled (any thread) gets the sum over the profiles
of its share of that profile's samples (each profile weighs the same); the
functions are written hottest first as ld section-ordering lines
(`*(.text.<mangled name>)`), then whole libraries (vitaGL, libgcc, libc...),
so the linker puts them at the start of .text, contiguous, and every
function no profile saw after them. Needs the build's
-ffunction-sections (CMakeLists.txt, PS2X_VITA_FUNCTION_SECTIONS).

Usage: tools/vita_hot_order.py <out.ld> <profile>:<elf> [<profile>:<elf> ...] [--size-elf <elf>]
  <profile>: a VITA3K_PROFILE file ("thread pc lr weight" lines, tools/vita3k.sh
  PROF=); <elf>: the ps2EntryRunner it was taken with (names are looked up
  there, so profiles of older builds still count). --size-elf: print the hot
  set's size in that ELF (its functions' sizes) at 50 / 90 / 99 / 100%.
"""
import bisect
import os
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


def main():
    args = sys.argv[1:]
    size_elf = None
    if '--size-elf' in args:
        i = args.index('--size-elf')
        size_elf = args[i + 1]
        del args[i:i + 2]
    out, specs = args[0], args[1:]
    total = Counter()
    for spec in specs:
        prof, elf = spec.rsplit(':', 1)
        addrs, names, sizes = symbols(elf)
        w = Counter()
        for line in open(prof):
            p = line.split()
            if len(p) != 4:
                continue
            pc = int(p[1], 16) & ~1
            i = bisect.bisect_right(addrs, pc) - 1
            if i < 0 or pc >= addrs[i] + max(sizes[i], 2):
                continue  # a stub, a veneer's padding, a system module
            w[names[i]] += int(p[3])
        tw = sum(w.values()) or 1
        for k, v in w.items():
            total[k] += v / tw
        print(f'{prof}: {len(w)} functions', file=sys.stderr)
    order = [k for k, _ in total.most_common() if 'veneer' not in k]
    with open(out, 'w') as f:
        f.write('/* Round 66: the functions Vita3K profiles sampled, hottest first\n'
                '   (tools/vita_hot_order.py). ld --section-ordering-file. */\n'
                '.text : {\n')
        for k in order:
            f.write(f'  *(.text.{k})\n')
        # Libraries built without function sections (or whose sampled parts
        # are hand-written: strlen, the division helpers), whole, right after
        # the sampled set: vitaGL (the render thread's GL calls, ~400 KB),
        # its NEON matrix helpers, the raylib shim, libgcc / the unwinder (the thread switch's
        # exception), pthread, libm, libc, the C++ runtime. Left at the end
        # of .text they are >16 MB from the hot code, whose calls into them
        # would go through long-branch veneers.
        for lib in ('libvitaGL.a', 'libmathneon.a', 'libps2x_vita_raylib.a', 'libgcc.a', 'libgcc_eh.a', 'libpthread.a',
                    'libm.a', 'libc.a', 'libsupc++.a', 'libstdc++.a'):
            f.write(f'  *{lib}:*(.text .text.*)\n')
        f.write('}\n')
    print(f'{out}: {len(order)} functions', file=sys.stderr)
    if size_elf:
        _, names, sizes = symbols(size_elf)
        size = dict(zip(names, sizes))
        T = sum(total[k] for k in order)
        cw = cs = 0.0
        marks = [0.5, 0.9, 0.99, 1.0]
        for k in order:
            cw += total[k]
            cs += size.get(k, 0)
            while marks and cw / T >= marks[0] - 1e-9:
                print(f'{marks.pop(0) * 100:5.1f}% of the samples: {cs / 1024:8.0f} KB', file=sys.stderr)


if __name__ == '__main__':
    main()
