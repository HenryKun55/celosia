#!/usr/bin/env python3
"""a9groups.py <elf> <run dir> [--sym RX]: D11 part A in a Vita3K sampler run (first and last prof snapshots).
Without an A/B: the game thread's share of each symbol matching --sym (default: the a9cmp sites' functions).
With an SOTC_AB_MIX run of a9cmp: per site the two copies (name...A9<true> = the integer compares, run only in the on
state; A9<false> = the float compares, off state), samples per 1000 frames of their own state, Poisson z (positive: the
integer copy cheaper), and all sites together; plus the game thread's ms a frame."""
import bisect, collections, glob, math, os, re, subprocess, sys
elf, run = sys.argv[1], sys.argv[2]
arg = lambda k, d: sys.argv[sys.argv.index(k) + 1] if k in sys.argv else d
SYM = re.compile(arg('--sym', r'g85::select|clip83::clip\(|clip140::compute\(|glVertexKick|quad117Body|clothMesh|ftoi4|diffQuat|axisDirZYQ|leafT<|A9<'))
mhz = 222.0
nm = os.path.expanduser('~/vitasdk/bin/arm-vita-eabi-nm')
out = subprocess.run([nm, '-n', '-C', '--defined-only', elf], capture_output=True, text=True).stdout
addrs, syms = [], []
for line in out.splitlines():
    p = line.split(' ', 2)
    if len(p) == 3 and p[1] in 'tTwW':
        addrs.append(int(p[0], 16) & ~1)
        syms.append(p[2])
snaps = sorted((int(m.group(1)), p) for p in glob.glob(f'{run}/prof.txt.*') if (m := re.search(r'prof\.txt\.(\d+)$', p)))
(s0, early), (s1, late) = snaps[0], snaps[-1]


def frames_at(err):
    last, fr = (0, 0), 0
    try:
        for line in open(err, errors='replace'):
            m = re.match(r'\[abmix\] frame (\d+): (\d+) frames, (\d+) off', line)
            if m:
                last = (int(m.group(2)), int(m.group(3)))
            m = re.match(r'\[frame\] each (\d+):', line)
            if m:
                fr = int(m.group(1))
    except OSError:
        pass
    return last, fr


def load(path):
    c = collections.Counter()
    for line in open(path):
        p = line.split()
        if len(p) >= 4:
            try:
                c[(p[0], int(p[1], 16))] += int(p[3])
            except ValueError:
                pass
    return c


(fa, ga), (fb, gb) = frames_at(late + '.err'), frames_at(early + '.err')
N, M = fa[0] - fb[0], fa[1] - fb[1]
onf, offf = N - M, M
frames = N if N > 0 else ga - gb
W = load(late)
W.subtract(load(early))
th = collections.Counter()
for (t, pc), w in W.items():
    if w > 0:
        th[t] += w
gt = th.most_common(1)[0][0]
per = collections.Counter()
for (t, pc), w in W.items():
    if t != gt or w <= 0:
        continue
    i = bisect.bisect_right(addrs, pc) - 1
    per[syms[i] if i >= 0 else '?'] += w
tot = sum(per.values())
secs = max(s1 - s0, 1)
print(f'{run}: {s0}-{s1} s, {frames} frames; game thread {tot / max(frames, 1) / (mhz * 1e3):.2f} ms a frame; on {onf} off {offf}')
site = collections.defaultdict(lambda: [0, 0])
for s, w in per.items():
    if not SYM.search(s):
        continue
    m9 = re.search(r'A9<((?:\w+, )*)(true|false)>', s)
    if m9:  # the last template argument is the A9 switch (leafTA9<Fast, A9>: Fast kept in the key)
        key = s.split('(')[0].replace(m9.group(0), 'A9' + (f'<{m9.group(1)[:-2]}>' if m9.group(1) else ''))
        site[key][0 if m9.group(2) == 'true' else 1] += w
    else:
        print(f'  {100 * w / tot:6.3f}%  {s[:110]}')
if N > 0 and site:
    A = B = 0
    for k, (a, b) in sorted(site.items(), key=lambda kv: -sum(kv[1])):
        a /= 20000.0; b /= 20000.0
        A += a; B += b
        ra, rb = a / max(onf, 1), b / max(offf, 1)
        var = a / max(onf, 1) ** 2 + b / max(offf, 1) ** 2
        z = (rb - ra) / math.sqrt(var) if var > 0 else 0
        print(f'  {k[:70]:70s} int {ra * 1000:8.2f} float {rb * 1000:8.2f} /1000 frames  {100 * ra / max(rb, 1e-12):6.1f}%  z {z:6.1f}')
    ra, rb = A / max(onf, 1), B / max(offf, 1)
    var = A / max(onf, 1) ** 2 + B / max(offf, 1) ** 2
    z = (rb - ra) / math.sqrt(var) if var > 0 else 0
    print(f'  ALL sites: int {ra * 1000:.2f} float {rb * 1000:.2f} samples/1000 frames ({100 * ra / max(rb, 1e-12):.1f}%), z {z:.1f}; '
          f'saved {(rb - ra) * 20000 / (mhz * 1e3):.4f} ms a frame')
