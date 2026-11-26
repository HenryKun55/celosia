#!/usr/bin/env python3
"""x2_zone.py err.txt --elf ELF [--top N] [--windows A-B]: round X2's console split of execScript's self (SOTC_X2_ZONE=1 with
SOTC_PROF129=1 and SOTC_P2BRACKET>=1). prof129's running samples by zone:
  0x4000 | fn << 6 | type : a woken script's run-to-return loop, in the guest function with index fn ([x2z] fn lines), object
                            type `type` (low 6 bits), outside every level-1 bracket;
  0x6100                  : the wake (isysWakeupSubThread) outside the body loop;
  0x6200                  : the rest of the script pass (the walk);
  0x5000 | slot           : inside a level-1 bracket (any caller).
Prints ms/frame by body function, by object type, and the functions (sampled pc) inside each zone class."""
import argparse, bisect, re, sys
from collections import Counter, defaultdict
sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import prof129 as P

ap = argparse.ArgumentParser()
ap.add_argument('err'); ap.add_argument('--elf', required=True); ap.add_argument('--top', type=int, default=30)
ap.add_argument('--windows'); ap.add_argument('--fnTop', type=int, default=8)
args = ap.parse_args()
args.t = None; args.frames = None
A, N, gstart, gname, anchor = P.load_elf(args.elf)
start, wins, sel = P.parse(args.err, args)
fnpc = {}
for l in open(args.err, errors='replace'):
    m = re.search(r'\[x2z\] fn (\d+) ([0-9a-f]+)', l)
    if m:
        fnpc[int(m.group(1))] = int(m.group(2), 16)


def guest_of(pc):
    i = bisect.bisect_right(gstart, pc) - 1
    return (gstart[i], gname[i]) if i >= 0 else (0, '?')


native_of = {}
for w in sel:
    for pc, off in w['h'].items():
        n = P.host_name(A, N, anchor + off) if off else ''
        if n and '_0x' not in n:
            native_of[pc] = n


def fname(pc):
    if pc in native_of:
        return P.short(native_of[pc])
    s, g = guest_of(pc)
    if s in native_of:
        return P.short(native_of[s])
    return f'{g}@{s:x}'


total = 0; wall = 0.0; frames = 0
cls = Counter(); byfn = Counter(); bytype = Counter(); funcs = defaultdict(Counter); fnfuncs = defaultdict(Counter)
for i, w in enumerate(sel):
    wall += w['t'] - (wins[wins.index(w) - 1]['t'] if wins.index(w) > 0 else 0.0)
    frames += w['f1'] - w['f0']
    for pc, ra, n, s, z in w['e']:
        total += n
        if 1 <= (s & 15) <= 14:
            continue
        z &= 0xFFFF
        if 0x4000 <= z < 0x5000:
            c = 'body'
        elif z == 0x6100:
            c = 'wake'
        elif z == 0x6200:
            c = 'walk'
        elif 0x5000 <= z < 0x5100:
            c = 'bracket'
        else:
            c = 'other'
        cls[c] += n
        if c in ('body', 'wake', 'walk'):
            funcs[c][fname(pc)] += n
        if c == 'body':
            fi = (z >> 6) & 0x3F; ty = z & 0x3F
            byfn[fi] += n; bytype[ty] += n; fnfuncs[fi][fname(pc)] += n
msf = wall * 1000.0 / frames if frames else 0
k = msf / total if total else 0
print(f'windows {len(sel)}, frames {frames}, {msf:.1f} ms/frame; samples {total}')
print('zone classes (ms/frame): ' + ', '.join(f'{c} {n * k:.3f}' for c, n in cls.most_common()))
print('\nscript bodies by object type (ms/frame):')
for t, n in bytype.most_common():
    print(f'  type {t:2d} (0x{t:02x}) {n * k:7.3f}')
print('\nscript bodies by entered function (ms/frame), with the sampled functions inside:')
for fi, n in byfn.most_common(args.top):
    pc = fnpc.get(fi, 0)
    print(f'  fn {fi:2d} {fname(pc) if pc else "?":50s} {n * k:7.3f}   ' + ', '.join(f'{f[:40]} {m * k:.3f}' for f, m in fnfuncs[fi].most_common(args.fnTop)))
for c in ('body', 'wake', 'walk'):
    print(f'\n{c}: sampled functions (ms/frame)')
    for f, n in funcs[c].most_common(args.top):
        print(f'  {n * k:7.3f} {f}')
