#!/usr/bin/env python3
"""Round 100: classify 3D frame spikes in a console err.txt by cause.
usage: spike100.py err.txt [--min-draws 200] [--k 1.4] [-v]
Each [frame] each/slow line -> a frame. 3D frames: draws >= min-draws.
Spike: period > k * median(period of 3D frames). Excess = period - median,
attributed to the columns that rose above their own 3D median."""
import re, sys, statistics, collections


def num(pat, s, d=0.0):
    m = re.search(pat, s)
    return float(m.group(1)) if m else d


def parse(path):
    frames = []
    for line in open(path, errors='replace'):
        if not line.startswith('[frame] each') and not line.startswith('[frame] slow'):
            continue
        f = {}
        f['frame'] = int(re.search(r'(?:each|slow) (\d+)', line).group(1))
        f['period'] = num(r': ([\d.]+) ms:', line)
        f['hooks'] = num(r'hooks ([\d.]+)', line)
        f['render'] = num(r'render ([\d.]+)', line)
        f['draw'] = num(r'draw ([\d.]+)', line, f['render'])
        f['lock'] = num(r'lock ([\d.]+)', line)
        f['present'] = num(r'present ([\d.]+)', line)
        f['wait'] = num(r' wait ([\d.]+)', line)
        f['draws'] = num(r'draws (\d+)', line)
        f['sched'] = num(r'sched ([\d.]+)', line)
        f['cd'] = num(r' cd ([\d.]+)', line)
        f['vb'] = num(r' vb (\d+)', line)
        f['jobs'] = num(r'jobs ([\d.]+) \(', line)
        f['join'] = num(r'\) join ([\d.]+)', line)
        f['sync'] = num(r'sync ([\d.]+)', line)
        f['syncjoin'] = num(r'sync [\d.]+ \(join ([\d.]+)', line)
        f['syncend'] = num(r' end ([\d.]+)', line)
        f['synclog'] = num(r' log ([\d.]+) audio', line)
        f['syncaudio'] = num(r' audio ([\d.]+) idle', line)
        f['cpugame'] = num(r'cpu game ([\d.]+)', line)
        f['cpurender'] = num(r', render ([\d.]+) \(', line)
        f['cpuworker'] = num(r', worker ([\d.]+) \(', line)
        f['dt'] = num(r' dt ([\d.]+)', line)
        js = re.search(r'joins (.*?) \| cpu', line)
        f['joins'] = {}
        if js and js.group(1) != 'none':
            for n, v in re.findall(r'(\S+) ([\d.]+)', js.group(1)):
                f['joins'][n] = float(v)
        w = re.search(r'\| waits(.*?)\|', line)
        f['waits'] = {}
        if w:
            for n, v in re.findall(r'(\S+) ([\d.]+)', w.group(1)):
                f['waits'][n] = float(v)
        f['line'] = line
        frames.append(f)
    return frames


COLS = ['wait', 'sched', 'join', 'hooks', 'synclog', 'syncaudio', 'syncend', 'cpugame', 'present', 'draw']


def attribute(f, cm, med):
    """Excess over the median split by column. The game thread's CPU (cpu game)
    holds its hooks and its frame-end work; the waits (render thread, sched,
    joins) are off-CPU; what is left of the period is preemption / other."""
    ex = f['period'] - med
    parts = {}
    endOther = lambda g: max(0.0, g['syncend'] - g['wait'])
    hk = lambda g: max(0.0, g['hooks'] - g['join'])  # hookUs holds the joins' time
    own = lambda g: max(0.0, g['cpugame'] - hk(g) - endOther(g) - g['synclog'])
    dwait = max(0, f['wait'] - cm['wait'])
    if dwait > 0:
        if f['present'] > f['draw']:
            parts['wait: render thread present/swap (GPU)'] = dwait
        else:
            parts['wait: render thread draw (CPU)'] = dwait
    ds = max(0, f['sched'] - cm['sched'])
    if ds > 0:
        tw = sum(f['waits'].values()) or 1
        for n, v in f['waits'].items():
            parts[f'sched: {n}'] = ds * v / tw
        if not f['waits']:
            parts['sched: ?'] = ds
    jsum = sum(f['joins'].values())
    dj = max(max(0, f['join'] - cm['join']), jsum)
    if dj > 0:
        if f['joins']:
            for n, v in f['joins'].items():
                parts[f'join: {n}'] = dj * v / (jsum or 1)
        else:
            parts['join: ?'] = dj
    for lab, v, m in (('hooks on game thread (decode/upload/build)', hk(f), cm['hk']),
                      ('frame end (flush2D, hand-over prep)', endOther(f), cm['endother']),
                      ('sync log', f['synclog'], cm['synclog']),
                      ('sync audio', f['syncaudio'], cm['syncaudio']),
                      ('game thread own CPU', own(f), cm['own'])):
        if v - m > 0:
            parts[lab] = v - m
    s = sum(parts.values())
    if s > ex:
        for n in parts:
            parts[n] *= ex / s
    else:
        parts['off-CPU, not a logged wait (preempt/other)'] = ex - s
    return parts


def medians(d3):
    cm = {c: statistics.median(f[c] for f in d3) for c in COLS}
    cm['endother'] = statistics.median(max(0.0, f['syncend'] - f['wait']) for f in d3)
    cm['hk'] = statistics.median(max(0.0, f['hooks'] - f['join']) for f in d3)
    cm['own'] = statistics.median(max(0.0, f['cpugame'] - max(0.0, f['hooks'] - f['join']) - max(0.0, f['syncend'] - f['wait']) - f['synclog']) for f in d3)
    return cm


def stats(d3):
    ps = sorted(f['period'] for f in d3)
    q = lambda p: ps[min(len(ps) - 1, int(p * len(ps)))]
    return statistics.mean(ps), statistics.median(ps), q(.9), q(.99), ps[-1]


def classify(frames, k=1.4, mind=200, verbose=False, med=None, cm=None, label='', local=0):
    d3 = [f for f in frames if f['draws'] >= mind]
    if local and d3:
        # Local baseline: each frame against the median of its +-local 3D neighbours
        # (a transient spike, not a slow scene).
        print(f'{label}local baseline +-{local} frames')
        share = collections.Counter(); count = collections.Counter(); tot = 0.0; n = 0
        for i, f in enumerate(d3):
            win = d3[max(0, i - local):i + local + 1]
            lm = statistics.median(g['period'] for g in win)
            if f['period'] <= k * lm:
                continue
            n += 1
            tot += f['period'] - lm
            parts = attribute(f, medians(win), lm)
            for c, v in parts.items():
                share[c] += v; count[c] += 1
        print(f'local spikes > {k}x local median: {n} ({100*n/len(d3):.1f}%), excess {tot:.0f} ms ({tot/len(d3):.2f} ms per 3D frame)')
        print(f'{"cause":45s} {"ms":>8s} {"share":>6s} {"frames":>6s}')
        for c, v in share.most_common():
            print(f'{c:45s} {v:8.0f} {100*v/(tot or 1):5.1f}% {count[c]:6d}')
        return None
    if not d3:
        print('no 3D frames')
        return None
    if med is None:
        med = statistics.median(f['period'] for f in d3)
        cm = medians(d3)
    mean, p50, p90, p99, mx = stats(d3)
    print(f'{label}3D frames {len(d3)}: mean {mean:.1f} p50 {p50:.1f} p90 {p90:.1f} p99 {p99:.1f} max {mx:.1f}')
    print('column medians: ' + ' '.join(f'{c} {v:.1f}' for c, v in cm.items()))
    spikes = [f for f in d3 if f['period'] > k * med]
    tot = sum(f['period'] - med for f in spikes)
    allex = sum(max(0, f['period'] - med) for f in d3) or 1
    print(f'spikes > {k}x median ({k*med:.1f} ms): {len(spikes)} ({100*len(spikes)/len(d3):.1f}%), excess {tot:.0f} ms '
          f'({100*tot/allex:.0f}% of all over-median ms; {tot/len(d3):.2f} ms per 3D frame)')
    share = collections.Counter()
    count = collections.Counter()
    for f in spikes:
        parts = attribute(f, cm, med)
        for n, v in parts.items():
            share[n] += v
            count[n] += 1
        if verbose:
            top = sorted(parts.items(), key=lambda x: -x[1])[:4]
            print(f"  f{f['frame']} {f['period']:.1f} " + ' '.join(f'{n}={v:.0f}' for n, v in top))
    print(f'{"cause":45s} {"ms":>8s} {"share":>6s} {"frames":>6s}')
    for n, v in share.most_common():
        print(f'{n:45s} {v:8.0f} {100*v/(tot or 1):5.1f}% {count[n]:6d}')
    return spikes


if __name__ == '__main__':
    args = sys.argv[1:]
    k = 1.4
    mind = 200
    verbose = '-v' in args
    if '--k' in args:
        k = float(args[args.index('--k') + 1])
    if '--min-draws' in args:
        mind = int(args[args.index('--min-draws') + 1])
    local = int(args[args.index('--local') + 1]) if '--local' in args else 0
    path = [a for a in args if a.endswith('.txt')][0]
    classify(parse(path), k, mind, verbose, local=local)
