#!/usr/bin/env python3
"""Round 100: a console / Vita3K err.txt with SOTC_AB_ROTATE=1 read per A/B state.
usage: ab100.py err.txt [--min-draws 200] [--k 1.4] [--skip 3] [--causes]

Per state ("all on" and "<flag> off"), over the 3D frames (draws >= min-draws,
the first --skip frames of each window left out):
  n, mean, p50, p90, p99, spike share (frames over k x the run's 3D median,
  their excess ms per 3D frame), and a paired delta: each window's mean and
  p90 against the "all on" windows on either side (the scene drifts slowly,
  so neighbours compare like with like), mean +- standard error.
--causes: the spike causes (spike100.attribute) per state."""
import os
import re
import statistics
import sys
import collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import spike100  # noqa: E402


def windows(path):
    """[(first frame, state)] from the [ab] lines."""
    out = []
    for line in open(path, errors='replace'):
        m = re.match(r'\[ab\] frame (\d+): (.*?)( off)?$', line.strip())
        if m:
            out.append((int(m.group(1)), 'all on' if m.group(2) == 'all on' else m.group(2) + ' off'))
    return out


def q(vals, p):
    v = sorted(vals)
    return v[min(len(v) - 1, int(p * len(v)))] if v else 0.0


def main():
    args = sys.argv[1:]
    k = float(args[args.index('--k') + 1]) if '--k' in args else 1.4
    mind = int(args[args.index('--min-draws') + 1]) if '--min-draws' in args else 200
    skip = int(args[args.index('--skip') + 1]) if '--skip' in args else 3
    causes = '--causes' in args
    path = [a for a in args if a.endswith('.txt')][0]
    frames = spike100.parse(path)
    wins = windows(path)
    if not wins:
        print('no [ab] lines: one state')
        wins = [(0, 'all on')]
    starts = [w[0] for w in wins]
    import bisect
    per = collections.defaultdict(list)
    wframes = collections.defaultdict(list)  # window index -> frames
    for f in frames:
        i = bisect.bisect_right(starts, f['frame']) - 1
        if i < 0 or f['draws'] < mind or f['frame'] < starts[i] + skip:
            continue
        per[wins[i][1]].append(f)
        wframes[i].append(f)
    d3 = [f for L in per.values() for f in L]
    if not d3:
        print('no 3D frames')
        return
    med = statistics.median(f['period'] for f in d3)
    cm = spike100.medians(d3)
    print(f'{path}: {len(d3)} 3D frames in {len(wframes)} windows, run median {med:.1f} ms, spike = over {k * med:.1f} ms')
    print(f'{"state":22s} {"n":>6s} {"mean":>6s} {"p50":>6s} {"p90":>6s} {"p99":>6s} {"spk%":>6s} {"spk ms/f":>8s} | paired vs all-on: {"d mean":>12s} {"d p90":>12s} wins')
    # paired deltas
    def neighbours(i):
        out = []
        for j in (i - 1, i + 1, i - 2, i + 2):
            if 0 <= j < len(wins) and wins[j][1] == 'all on' and wframes.get(j):
                out.append(j)
        return out[:2]
    for state in sorted(per, key=lambda s: (s != 'all on', s)):
        L = per[state]
        ps = [f['period'] for f in L]
        spk = [f for f in L if f['period'] > k * med]
        ex = sum(f['period'] - med for f in spk) / len(L)
        dm, dp = [], []
        if state != 'all on':
            for i, (st, name) in enumerate(wins):
                if name != state or not wframes.get(i):
                    continue
                nb = neighbours(i)
                if not nb:
                    continue
                base = [f['period'] for j in nb for f in wframes[j]]
                mine = [f['period'] for f in wframes[i]]
                if len(base) < 10 or len(mine) < 10:
                    continue
                dm.append(statistics.mean(mine) - statistics.mean(base))
                dp.append(q(mine, .9) - q(base, .9))
        def fmt(d):
            if len(d) < 2:
                return f'{"-":>12s}'
            return f'{statistics.mean(d):+6.1f}+-{statistics.stdev(d) / len(d) ** .5:4.1f}'
        print(f'{state:22s} {len(L):6d} {statistics.mean(ps):6.1f} {q(ps, .5):6.1f} {q(ps, .9):6.1f} {q(ps, .99):6.1f} '
              f'{100 * len(spk) / len(L):5.1f}% {ex:8.2f} | {"":17s} {fmt(dm)} {fmt(dp)} {len(dm)}')
    if causes:
        for state in sorted(per, key=lambda s: (s != 'all on', s)):
            share = collections.Counter()
            tot = 0.0
            for f in per[state]:
                if f['period'] > k * med:
                    tot += f['period'] - med
                    for c, v in spike100.attribute(f, cm, med).items():
                        share[c] += v
            print(f'\n{state}: spike excess {tot:.0f} ms over {len(per[state])} frames')
            for c, v in share.most_common(8):
                print(f'  {c:45s} {v:7.0f} {100 * v / (tot or 1):5.1f}%')


if __name__ == '__main__':
    main()
