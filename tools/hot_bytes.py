#!/usr/bin/env python3
"""hot_bytes.py <elf> <prof> [--objs DIR ...] [--top N] [--target ELF2]: the game thread's hot code in bytes (round I1).

<prof>: a Vita3K host profile (thread pc lr weight, tools/vita3k.sh PROF=) taken with <elf>. The game thread = the thread
with the most weight. Prints: the functions covering 50/75/90/95/99/100% of the thread's samples and their summed sizes
(the hot set); distinct 32-byte lines / 4 KB pages sampled; shares by kind (recompiled guest code / overrides / runtime /
libraries) and, with --objs, by object file (nm of every .o / .obj below DIR, e.g. the Vita build's ps2EntryRunner.dir).
--target: the same names' sizes in another ELF (the same code built with other flags): the hot set's size there, by kind,
and per function. Names missing from the target keep their size.
"""
import bisect, os, re, subprocess, sys
from collections import Counter
NM = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')


def syms(elf):
    out = subprocess.run([NM, '-n', '-S', '--defined-only', elf], capture_output=True, text=True).stdout
    a, n, s = [], [], []
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in 'tTwW':
            a.append(int(p[0], 16) & ~1); s.append(int(p[1], 16)); n.append(p[3])
    return a, n, s


def kind(name, objof):
    o = objof.get(name, '')
    if o:
        if '/Unity/' in o: return 'recompiled'
        if '/overrides/decomp_liftd' in o: return 'lifted D'
        if '/overrides/' in o: return 'overrides'
        if '/vu1rec' in o: return 'vu1rec'
        return 'runtime'
    if re.search(r'_0x[0-9a-f]+$', name): return 'recompiled'
    if 'veneer' in name: return 'veneer'
    return 'lib/other'


def main():
    args = sys.argv[1:]

    def opt(k, d=None, multi=False):
        vals = []
        while k in args:
            i = args.index(k); vals.append(args[i + 1]); del args[i:i + 2]
        return vals if multi else (vals[-1] if vals else d)
    objdirs = opt('--objs', multi=True); top = int(opt('--top', '40')); target = opt('--target')
    elf, prof = args[0], args[1]
    A, N, S = syms(elf)
    objof = {}
    for d in objdirs:
        for root, _, files in os.walk(d):
            for f in files:
                if f.endswith('.o') or f.endswith('.obj'):
                    p = os.path.join(root, f)
                    out = subprocess.run([NM, '--defined-only', p], capture_output=True, text=True).stdout
                    for line in out.splitlines():
                        q = line.split()
                        if len(q) == 3 and q[1] in 'tTwW':
                            objof.setdefault(q[2], p)
    rows = []; per = Counter()
    for line in open(prof):
        p = line.split()
        if len(p) != 4: continue
        w = int(p[3]); rows.append((p[0], int(p[1], 16) & ~1, w)); per[p[0]] += w
    game = per.most_common(1)[0][0]
    fw = Counter(); lines = set(); pages = set(); tot = 0; miss = 0
    for t, pc, w in rows:
        if t != game: continue
        tot += w
        i = bisect.bisect_right(A, pc) - 1
        if i < 0 or pc >= A[i] + max(S[i], 2): miss += w; continue
        fw[i] += w; lines.add(pc >> 5); pages.add(pc >> 12)
    print(f'{prof}: game thread {game}: {100*tot/sum(per.values()):.1f}% of all weight, unmapped {100*miss/tot:.2f}%')
    print(f'  sampled: {len(fw)} functions, {len(lines)} distinct 32 B lines ({len(lines)*32/1024:.0f} KB), {len(pages)} 4 KB pages')
    order = sorted(fw, key=lambda i: -fw[i])
    T = None
    if target:
        _, TN, TS = syms(target); T = {}
        for n, s in zip(TN, TS): T.setdefault(n, s)
    for cov in (0.5, 0.75, 0.9, 0.95, 0.99, 1.0):
        acc = 0; size = 0; tsz = 0; k = 0; byk = Counter(); tbyk = Counter(); tmiss = 0
        for i in order:
            if acc >= cov * tot: break
            acc += fw[i]; size += S[i]; k += 1; kk = kind(N[i], objof); byk[kk] += S[i]
            if T is not None:
                v = T.get(N[i]); tmiss += v is None; v = S[i] if v is None else v
                tsz += v; tbyk[kk] += v
        ks = ', '.join(f'{kk} {v/1024:.0f}' for kk, v in byk.most_common())
        line = f'  {int(cov*100):3d}%: {k:5d} functions, {size/1024:7.0f} KB ({ks})'
        if T is not None:
            line += (f'  | target {tsz/1024:7.0f} KB ({100*(tsz-size)/max(size,1):+.1f}%; '
                     + ', '.join(f'{kk} {v/1024:.0f}' for kk, v in tbyk.most_common()) + f'; {tmiss} not found)')
        print(line)
    kw = Counter(); ksz = Counter()
    for i in fw: kk = kind(N[i], objof); kw[kk] += fw[i]; ksz[kk] += S[i]
    print('  by kind (% of samples / sampled functions KB): ' + ', '.join(f'{k} {100*v/tot:.1f}% / {ksz[k]/1024:.0f}' for k, v in kw.most_common()))
    if objof:
        ow = Counter(); osz = Counter()
        for i in fw: o = os.path.basename(objof.get(N[i], '?')); ow[o] += fw[i]; osz[o] += S[i]
        print('  top objects (% samples, sampled functions KB):')
        for o, v in ow.most_common(30): print(f'    {100*v/tot:6.2f}%  {osz[o]/1024:7.0f}  {o}')
    print(f'  top {top} functions (% samples, size, % per KB):')
    for i in order[:top]:
        extra = ''
        if T is not None and N[i] in T: extra = f'  -> {T[N[i]]} ({100*(T[N[i]]-S[i])/max(S[i],1):+.0f}%)'
        print(f'    {100*fw[i]/tot:6.2f}%  {S[i]:7d}  {100*fw[i]/tot/max(S[i],1)*1024:7.3f}  {kind(N[i], objof):10s} {N[i][:100]}{extra}')
    big = sorted(order[:3000], key=lambda i: -S[i])[:30]
    print('  largest functions among the 3000 hottest (size, % samples):')
    for i in big:
        extra = ''
        if T is not None and N[i] in T: extra = f'  -> {T[N[i]]}'
        print(f'    {S[i]:8d}  {100*fw[i]/tot:6.3f}%  {kind(N[i], objof):10s} {N[i][:110]}{extra}')


if __name__ == '__main__':
    main()
