#!/usr/bin/env python3
"""Round A1: sum the recorder's [a1] blocks (SOTC_A1_RECORD=1, game/overrides/a1_record.cpp) over a walk range.

usage: a1_census.py <log> [--from WALK] [--to WALK] [--bin ps2EntryRunner] [--top N]

Prints per block range: W_disp words a walk, the readers summed (reads a cycle, cycles, same / next / later frame), the
W_disp and R_disp x W_logic object offsets summed. With --bin, "pc 0x..." readers are named with atos.
"""
import re
import subprocess
import sys
from collections import defaultdict

args = sys.argv[1:]
log = args[0]
opt = {}
i = 1
while i < len(args):
    opt[args[i]] = args[i + 1]
    i += 2
lo = int(opt.get('--from', 0))
hi = int(opt.get('--to', 1 << 60))
top = int(opt.get('--top', 40))

head = re.compile(r'\[a1\] ==== walk (\d+) \(scope [0-9a-f]+\): (\d+) cycles; W_disp (\d+) words a walk \(stack (\d+), scratchpad (\d+), '
                  r'group objects (\d+); (\d+) objects\); still the walk\'s at the next walk (\d+), rewritten (\d+); R_disp (\d+) lines.*?'
                  r'W_logic (\d+) words; R_disp x W_logic (\d+) words \(stack (\d+)\)')
reader = re.compile(r'\[a1\] reader (.+?)\s+([\d.]+) reads a cycle in (\d+) cycles; same frame (\d+) next (\d+) later (\d+); stack (\d+); '
                    r'other thread (\d+); earliest (\d+) us \(([0-9a-f]+)\);(.*)')
offs = re.compile(r'\[a1\] (W|RxWlogic) object offsets \(words a walk\):(.*)')
wline = re.compile(r'\[a1\] (W|RxWlogic) ([0-9a-f]+)\.\.([0-9a-f]+)\s+([\d.]+) bytes a cycle\s+in (\d+) of (\d+) cycles(.*)')

blocks = 0
cyc = 0
tot = defaultdict(float)
readers = defaultdict(lambda: defaultdict(float))
offsum = {'W': defaultdict(float), 'RxWlogic': defaultdict(float)}
ranges = {'W': defaultdict(float), 'RxWlogic': defaultdict(float)}
blk = re.compile(r'\[a1\] (W|R|RxWlogic) by 64 KB \(bytes a walk\):(.*)')
blocks64 = {'W': defaultdict(float), 'R': defaultdict(float), 'RxWlogic': defaultdict(float)}
inblock = False
for line in open(log, errors='replace'):
    m = head.search(line)
    if m:
        walk, n = int(m.group(1)), int(m.group(2))
        inblock = lo <= walk <= hi and n > 0
        if inblock:
            blocks += 1
            cyc += n
            keys = ['W', 'stack', 'scr', 'obj', 'objects', 'live', 'rewritten', 'R', 'Wlogic', 'RxW', 'RxWstack']
            for k, v in zip(keys, m.groups()[2:]):
                tot[k] += float(v) * n
            curn = n
        continue
    if not inblock:
        continue
    m = reader.search(line)
    if m:
        r = readers[m.group(1)]
        r['reads'] += float(m.group(2)) * curn
        r['cycles'] += int(m.group(3))
        r['same'] += int(m.group(4))
        r['next'] += int(m.group(5))
        r['later'] += int(m.group(6))
        r['stack'] += int(m.group(7))
        r['other'] += int(m.group(8))
        if 'ex' not in r:
            r['ex'] = m.group(11).strip()
        continue
    m = offs.search(line)
    if m:
        for tok in m.group(2).split():
            o, v = tok.split(':')
            offsum[m.group(1)][o] += float(v) * curn
        continue
    m = wline.search(line)
    if m:
        a = int(m.group(2), 16)
        ranges[m.group(1)][(a >> 12) << 12] += float(m.group(4)) * curn
        continue
    m = blk.search(line)
    if m:
        for tok in m.group(2).split():
            a, v = tok.split(':')
            blocks64[m.group(1)][int(a, 16)] += float(v) * curn

if not cyc:
    print('no blocks')
    sys.exit(0)
print(f'{blocks} blocks, {cyc} cycles (walks {lo}..{hi})')
print('per walk: ' + ', '.join(f'{k} {v / cyc:.0f}' for k, v in tot.items()))
names = {}
if '--bin' in opt:
    pcs = [k for k in readers if k.startswith('pc 0x')]
    if pcs:
        out = subprocess.run(['atos', '-o', opt['--bin']] + [k[3:] for k in pcs], capture_output=True, text=True).stdout.split('\n')
        names = dict(zip(pcs, out))
print(f'\nreaders (reads a walk, cycles of {cyc}, same / next / later frame):')
for k, r in sorted(readers.items(), key=lambda kv: -kv[1]['reads'])[:top]:
    nm = names.get(k, k)
    nm = re.sub(r'^_Z\d+(\w+?)_0x([0-9a-f]+)PhP12R5900ContextP10PS2Runtime', r'\1 (0x\2)', nm)
    print(f"  {nm:60s} {r['reads'] / cyc:9.1f}  {int(r['cycles']):5d}  {int(r['same'])}/{int(r['next'])}/{int(r['later'])}"
          f"  stack {int(r['stack'])} other-thread {int(r['other'])}  {r.get('ex', '')[:90]}")
for what in ('W', 'RxWlogic'):
    print(f'\n{what} object offsets (words a walk):')
    items = sorted(offsum[what].items(), key=lambda kv: -kv[1])[:top]
    print('  ' + ' '.join(f'{o}:{v / cyc:.1f}' for o, v in items))
    print(f'{what} top 4 KB pages of the listed ranges (bytes a walk):')
    items = sorted(ranges[what].items(), key=lambda kv: -kv[1])[:top // 2]
    print('  ' + ' '.join(f'{a:08x}:{v / cyc:.0f}' for a, v in items))
for what, b in blocks64.items():
    if b:
        print(f'\n{what} by 64 KB block (bytes a walk; the 48 largest blocks of each report; sum {sum(b.values()) / cyc:.0f}):')
        items = sorted(b.items(), key=lambda kv: -kv[1])[:top]
        print('  ' + ' '.join(f'{a:08x}:{v / cyc:.0f}' for a, v in items))
