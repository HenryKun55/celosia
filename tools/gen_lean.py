#!/usr/bin/env python3
"""Lean copies of the hot recompiled functions (game/overrides/lean_hot.cpp).

The hot set is game/lean_hot.txt (guest addresses, hottest first: made from
SOTC_CALL_COUNT_DUMP files with --from). This writes, under build/runner/lean/
(next to the recompiled code; ./celosia recompile wipes it, run this again):
  decls.inc     the lean functions' declarations
  renames.inc   #define <name> lean_<name> for each (direct calls between
                lean functions stay lean)
  table.inc     LEAN_ENTRY(address, lean_<name>, <name>, "<name>", checkable)
                lines (checkable: no syscall inside, for SOTC_LEAN_CHECK)
  partN.inc     the #includes of the recompiled sources, split over the
                game/overrides/lean_partN.cpp files (compiled in parallel)

After a run, rebuild the lean parts with CCACHE_RECACHE=1 (touch
game/overrides/lean_part*.cpp first): ccache's direct mode does not see a
__has_include that turned true and hands back the old, empty objects (a
link error "undefined reference to lean_...").

Usage:
  tools/gen_lean.py                       # from game/lean_hot.txt
  tools/gen_lean.py --from calls.txt [calls2.txt ...] [--min 1000]
                                          # rewrites game/lean_hot.txt first
"""
import glob
import os
import re
import sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
RUNNER = os.path.join(ROOT, 'build', 'runner')
LIST = os.path.join(ROOT, 'game', 'lean_hot.txt')
OUT = os.path.join(RUNNER, 'lean')
PARTS = 12  # game/overrides/lean_part0.cpp .. lean_part11.cpp


def runner_functions():
    by_address = {}
    for path in glob.glob(os.path.join(RUNNER, '*_0x*.cpp')):
        name = os.path.basename(path)[:-4]
        m = re.match(r'.*_0x([0-9a-f]+)$', name)
        if m:
            by_address[int(m.group(1), 16)] = name
    return by_address


def write_list(dumps, minimum):
    counts = {}
    for path in dumps:
        for line in open(path):
            if line.startswith('#'):
                continue
            a, c = line.split()
            counts[int(a, 16)] = max(counts.get(int(a, 16), 0), int(c))
    hot = sorted((a for a, c in counts.items() if c >= minimum), key=lambda a: -counts[a])
    with open(LIST, 'w') as out:
        out.write('# Hot recompiled functions for the lean copies (tools/gen_lean.py), hottest first.\n')
        out.write('# From %s, at least %d calls.\n' % (', '.join(os.path.basename(d) for d in dumps), minimum))
        for a in hot:
            out.write('%08x %d\n' % (a, counts[a]))
    print('%s: %d functions' % (LIST, len(hot)))


def main():
    args = sys.argv[1:]
    if args and args[0] == '--from':
        minimum = 1000
        dumps = []
        rest = args[1:]
        while rest:
            if rest[0] == '--min':
                minimum = int(rest[1])
                rest = rest[2:]
            else:
                dumps.append(rest[0])
                rest = rest[1:]
        write_list(dumps, minimum)
    funcs = runner_functions()
    names = []
    for line in open(LIST):
        if line.startswith('#') or not line.strip():
            continue
        a = int(line.split()[0], 16)
        if a not in funcs:  # not in build/runner: nothing to copy
            continue
        src = open(os.path.join(RUNNER, funcs[a] + '.cpp')).read()
        if '// Function:' not in src:  # a stub into the host kernel (WaitSema ...): no guest code
            continue
        # A syscall inside: left out of the difftest (it can switch threads).
        names.append((a, funcs[a], 'handleSyscall' in src))
    os.makedirs(OUT, exist_ok=True)
    for f in glob.glob(os.path.join(OUT, '*.inc')):
        os.remove(f)
    sig = '(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)'
    with open(os.path.join(OUT, 'decls.inc'), 'w') as out:
        for _, n, _ in names:
            out.write('void lean_%s%s;\n' % (n, sig))
    with open(os.path.join(OUT, 'renames.inc'), 'w') as out:
        for _, n, _ in names:
            out.write('#define %s lean_%s\n' % (n, n))
    with open(os.path.join(OUT, 'table.inc'), 'w') as out:
        for a, n, sys_ in names:
            out.write('LEAN_ENTRY(0x%08xu, lean_%s, %s, "%s", %s)\n' % (a, n, n, n, 'false' if sys_ else 'true'))
    # Spread by size so the parts take about as long to compile.
    sizes = sorted(((os.path.getsize(os.path.join(RUNNER, n + '.cpp')), n) for _, n, _ in names), reverse=True)
    parts = [[] for _ in range(PARTS)]
    load = [0] * PARTS
    for size, n in sizes:
        i = load.index(min(load))
        parts[i].append(n)
        load[i] += size
    for i, part in enumerate(parts):
        with open(os.path.join(OUT, 'part%d.inc' % i), 'w') as out:
            for n in sorted(part):
                out.write('#include "%s.cpp"\n' % n)
    print('%s: %d lean functions in %d parts (%.1f MB of recompiled C++)' % (OUT, len(names), PARTS, sum(load) / 1e6))


if __name__ == '__main__':
    main()
