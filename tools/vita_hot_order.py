#!/usr/bin/env python3
"""Writes the Vita build's hot-code order (round 66) from Vita3K profiles.

Every function a profile sampled (any thread) gets the sum over the profiles
of its share of that profile's samples (each profile weighs the same); the
functions are written hottest first as ld section-ordering lines
(`*(.text.<mangled name>)`), then whole libraries (vitaGL, libgcc, libc...),
so the linker puts them at the start of .text, contiguous, and every
function no profile saw after them. Needs the build's
-ffunction-sections (CMakeLists.txt, PS2X_VITA_FUNCTION_SECTIONS).

Usage: tools/vita_hot_order.py <out.ld> <profile>:<elf> [<profile>:<elf> ...] [--size-elf <elf>] [--natives <list.txt> ...] [--cover <f>]
  <profile>: a VITA3K_PROFILE file ("thread pc lr weight" lines, tools/vita3k.sh
  PROF=); <elf>: the ps2EntryRunner it was taken with (names are looked up
  there, so profiles of older builds still count). --size-elf: print the hot
  set's size in that ELF (its functions' sizes) at 50 / 90 / 99 / 100%.
  --natives (round M1, repeatable): a docs/d1_natives.txt-style list; its adapters (<fn>D1), their dispatchers (<fn>Sw)
  and cores that no profile sampled are written right after the sampled set (the D rounds' bodies run only with
  d1native on, rare paths of them in few profiles), looked up by their C++ names in the first profile's ELF.
  --cover (round M1): keep only the hottest functions holding that share of the samples (0.995 in vita_hot_order.sh).
  The current order: tools/vita_hot_order.sh (docs/decomp_m1.md).
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


def lambda_variants(name):
    """The names with some of the numbered lambda discriminators (`Ul..E<n>_`) un-numbered (`Ul..E_`), name excluded."""
    parts = re.split(r'(Ul[0-9A-Za-z_]*?E)([0-9]+)_', name)
    n = (len(parts) - 1) // 3
    if not n or n > 4:
        return []
    out = []
    for mask in range(1, 1 << n):
        s = parts[0]
        for i in range(n):
            pre, num, rest = parts[1 + 3 * i], parts[2 + 3 * i], parts[3 + 3 * i]
            s += pre + ('' if mask >> i & 1 else num) + '_' + rest
        out.append(s)
    return out


def natives_names(lists, elf):
    """Mangled names of the adapters / dispatchers / cores a natives list names (demangled lookup in elf)."""
    want = []
    for path in lists:
        for line in open(path):
            if line.startswith('#') or '|' not in line:
                continue
            col = [c.strip() for c in line.split('|')]
            if len(col) < 5:
                continue
            names = [col[3]]
            if col[3].endswith('D1'):
                names.append(col[3][:-2] + 'Sw')
            names += [c.strip() for c in col[4].split(',') if c.strip() and c.strip() != '-']
            for n in names:
                if n not in want:
                    want.append(n)
    out = subprocess.run([NM, '--defined-only', elf], capture_output=True, text=True).stdout
    mangled = [l.split()[2] for l in out.splitlines() if len(l.split()) == 3 and l.split()[1] in 'tTwW']
    dem = subprocess.run([os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-c++filt')], input='\n'.join(mangled),
                         capture_output=True, text=True).stdout.splitlines()
    found = []
    for n in want:
        rx = re.compile(r'(^|\s)' + re.escape(n) + r'\(')
        hits = [m for m, d in zip(mangled, dem) if rx.search(d) and 'veneer' not in m]
        for h in hits:
            if h not in found:
                found.append(h)
    print(f'natives: {len(want)} names, {len(found)} functions in {elf}', file=sys.stderr)
    return found


def main():
    args = sys.argv[1:]
    size_elf = None
    lists = []
    cover = 1.0
    if '--cover' in args:
        i = args.index('--cover')
        cover = float(args[i + 1])
        del args[i:i + 2]
    while '--natives' in args:
        i = args.index('--natives')
        lists.append(args[i + 1])
        del args[i:i + 2]
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
    if cover < 1.0:
        # Round M1: only the functions holding the first `cover` of the samples. The long tail (once-a-minute code
        # sampled once) made the cluster ~21 MB, past Thumb's +-16 MB bl reach: the hot code's calls into the libraries
        # after the cluster then went through veneers.
        T0 = sum(total[k] for k in order) or 1
        cw, keep = 0.0, []
        for k in order:
            if cw / T0 >= cover:
                break
            cw += total[k]
            keep.append(k)
        order = keep
    sampled = len(order)
    if lists:
        seen = set(order)
        order += [k for k in natives_names(lists, specs[0].rsplit(':', 1)[1]) if k not in seen]
    with open(out, 'w') as f:
        f.write('/* The functions Vita3K profiles sampled, hottest first (tools/vita_hot_order.py, round 66;\n'
                '   regenerate after big native changes with tools/vita_hot_order.sh: round 121 found the round 66 order stale,\n'
                '   round M1 the D rounds\' bodies outside it). */\n'
                '.text : {\n')
        # Round I2: the lifted code's shared call helpers (game/overrides/lift_outline.h, PS2X_VITA_LIFTED_OUTLINE=ON;
        # every guest call of the lifted D files goes through them) first; with the option off nothing matches.
        f.write('  *(.text.ps2lift_* .text.hot.ps2lift_*)\n')
        # (.text.hot.<name>: GCC's section for a function a PGO profile found hot, round 121)
        for k in order:
            # Round M1: GCC names a lambda's section after the first lambda of its scope (`..UljE_clEj`) while the
            # symbol carries its own number (`..UljE0_clEj`): obj125Run's 30 KB inner lambda (~1% of the game thread)
            # was never matched and sat 42 MB from the rest. The un-numbered name too (it can also pull the sibling).
            # (Each numbered lambda in the name kept or un-numbered: nested lambdas keep their own number.)
            extra = ''.join(f' .text.{a} .text.hot.{a}' for a in lambda_variants(k))
            f.write(f'  *(.text.{k} .text.hot.{k}{extra})\n')
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
    print(f'{out}: {len(order)} functions ({sampled} sampled, {len(order) - sampled} from the natives lists)', file=sys.stderr)
    if size_elf:
        _, names, sizes = symbols(size_elf)
        size = dict(zip(names, sizes))
        T = sum(total[k] for k in order) or 1
        cw = cs = 0.0
        marks = [0.5, 0.9, 0.99, 1.0]
        for k in order:
            cw += total[k]
            cs += size.get(k, 0)
            while marks and cw / T >= marks[0] - 1e-9:
                print(f'{marks.pop(0) * 100:5.1f}% of the samples: {cs / 1024:8.0f} KB', file=sys.stderr)


if __name__ == '__main__':
    main()
