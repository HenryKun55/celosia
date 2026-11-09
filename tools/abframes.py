#!/usr/bin/env python3
"""Per-frame rotating A/B table from an err.txt (SOTC_FRAME_LOG=1 SOTC_FRAME_LOG_ALL=1
SOTC_AB_FLAG=.. SOTC_AB_ROTATE=1). Round 56: Vita3K on this Mac stalls ~1 s every few
seconds while the host screen is locked (the host's present, not the guest): frames
of STALL ms or more are left out and counted apart. Uses "[frame] each" (<= 50 ms) and
"[frame] slow" (> 50 ms) lines; game = frame - hooks - wait (the game thread's own time).
Usage: abframes.py err.txt [--from F] [--to F] [--stall MS]
       abframes.py err.txt --summary [--bin DRAWS] [--from F] [--to F] [--stall MS] [--min N]
Round 135, --summary (also chosen when the log has no "[frame] each" lines: the console env, SOTC_FRAME_LOG=1 only): a
draw-matched A/B on the 60-frame "[frame] N: .. ms" summaries. A summary counts for a state when its 60 frames lie in
one A/B window. The summaries are put in bins by their draws a frame (--bin, default 50: a bin is a scene load), each
state compared with "all on" inside a bin, and the bins' differences averaged with weight min(n_state, n_on) (bins
where one side has fewer than --min summaries, default 2, left out), so windows that saw other scenes do not count
as a difference. +- is 2 standard errors from the bins' spread. Summaries with a slowest frame >= --stall (a load,
a host stall) are counted apart.
"""
import re, sys, statistics as st
args = sys.argv[1:]
def opt(name, d):
    if name in args:
        i = args.index(name); v = args[i + 1]; del args[i:i + 2]; return float(v)
    return d
summary = '--summary' in args
if summary: args.remove('--summary')
binw, minn = opt('--bin', 50), int(opt('--min', 2))
lo, hi, stall = opt('--from', 0), opt('--to', 1e18), opt('--stall', 700)
ab_re = re.compile(r'\[ab\] frame (\d+): (all on|(\S+) off)')
fr_re = re.compile(r'^\[frame\] (?:each|slow) (\d+): ([\d.]+) ms')
def num(line, key):
    m = re.search(r'\b' + key + r' ([\d.]+)', line)
    return float(m.group(1)) if m else 0.0
wins, rows, stalls = [], {}, {}
sum_re = re.compile(r'^\[frame\] (\d+): ([\d.]+) ms \([\d.]+ fps, slowest ([\d.]+)\)')
lines = open(args[0], errors='replace').read().splitlines()
if not summary and not any(l.startswith('[frame] each ') for l in lines):
    summary = True
def stateAt(f):
    st_ = 'all on' if not wins else None
    for s0, s in wins:
        if s0 <= f: st_ = s
        else: break
    return st_
if summary:
    import math
    sums = {}
    for line in lines:
        m = ab_re.search(line)
        if m:
            wins.append((int(m.group(1)), m.group(3) or 'all on')); continue
        m = sum_re.search(line)
        if not m: continue
        f, ms, slowest = int(m.group(1)), float(m.group(2)), float(m.group(3))
        if f < lo or f > hi: continue
        s0, s1 = stateAt(f - 59), stateAt(f)
        if s0 is None or s0 != s1: continue
        if slowest >= stall:
            stalls[s1] = stalls.get(s1, 0) + 1; continue
        body = line.split(' ms', 1)[1]
        cpu = re.search(r'cpu game ([\d.]+)', body)
        sums.setdefault(s1, []).append(dict(frame=ms, game=num(body, 'game'), cpu=float(cpu.group(1)) if cpu else 0.0, render=num(body, 'render'),
                                             wait=num(body, 'wait'), draws=num(body, 'draws'), jobs=num(body, 'jobs')))
    keys = ['frame', 'game', 'cpu', 'render', 'wait', 'draws', 'jobs']
    def bins(r):
        b = {}
        for x in r: b.setdefault(int(x['draws'] // binw), []).append(x)
        return b
    on = sums.get('all on', [])
    bon = bins(on)
    print('draw-matched A/B on the 60-frame summaries (bins of %d draws; %d summaries all on)' % (binw, len(on)))
    print('%-12s %5s %5s %6s | %s' % ('state', 'n', 'used', 'stall', ' '.join('%13s' % k for k in keys)))
    def mean(v): return sum(v) / len(v)
    def var(v):
        if len(v) < 2: return 0.0
        m_ = mean(v); return sum((x - m_) ** 2 for x in v) / (len(v) - 1)
    for s in ['all on'] + sorted(k for k in sums if k != 'all on'):
        r = sums[s]
        print('%-12s %5d %5s %6d | %s   (plain means)' % (s, len(r), '', stalls.get(s, 0), ' '.join('%13.2f' % mean([x[k] for x in r]) for k in keys)))
        if s == 'all on': continue
        bs = bins(r); W = 0.0; acc = {k: 0.0 for k in keys}; vv = {k: 0.0 for k in keys}; used = 0; per = []
        for b in sorted(set(bs) & set(bon)):
            A, B = bs[b], bon[b]
            if len(A) < minn or len(B) < minn: continue
            w = min(len(A), len(B)); W += w; used += len(A)
            for k in keys:
                a_, b_ = [x[k] for x in A], [x[k] for x in B]
                acc[k] += w * (mean(a_) - mean(b_)); vv[k] += w * w * (var(a_) / len(a_) + var(b_) / len(b_))
            per.append((b, len(A), len(B), mean([x['game'] for x in A]) - mean([x['game'] for x in B]), mean([x['frame'] for x in A]) - mean([x['frame'] for x in B])))
        if W == 0:
            print('%-12s no bin with both states' % ''); continue
        print('%-12s %5s %5d %6s | %s   (matched: off minus on)' % ('', '', used, '', ' '.join('%+7.2f+-%-5.2f' % (acc[k] / W, 2 * math.sqrt(vv[k]) / W) for k in keys)))
        if '--bins' in args:
            for b, na, nb, dg, dfr in per:
                print('%-12s   draws %4d-%4d: n %3d / %3d  game %+6.2f frame %+6.2f' % ('', b * binw, b * binw + binw - 1, na, nb, dg, dfr))
    sys.exit(0)
for line in lines:
    m = ab_re.search(line)
    if m:
        wins.append((int(m.group(1)), m.group(3) or 'all on')); continue
    m = fr_re.search(line)
    if not m:
        continue
    f, ms = int(m.group(1)), float(m.group(2))
    if f < lo or f > hi:
        continue
    state = 'all on' if not wins else None
    for s0, s in wins:
        if s0 <= f: state = s
        else: break
    if state is None:
        continue
    if ms >= stall:
        stalls[state] = stalls.get(state, 0) + 1; continue
    body = line.split(' ms', 1)[1]
    hooks, wait, render, draws = num(body, 'hooks'), num(body, 'wait'), num(body, 'render'), num(body, 'draws')
    rows.setdefault(state, []).append((ms, ms - hooks - wait, render, wait, draws, num(body, 'vb')))
print('%-12s %5s %6s %7s %7s %7s %7s %7s %6s %6s %6s %5s %6s' % ('state', 'n', 'stall', 'frame', 'game', 'render', 'wait', 'draws', '<=34', '<=50', 'p95', 'vb', 'vb<=2'))
base = None
for s in ['all on'] + sorted(k for k in rows if k != 'all on'):
    r = rows.get(s)
    if not r: continue
    c = list(zip(*r)); fr = sorted(c[0])
    vals = [st.mean(x) for x in c]
    if s == 'all on': base = vals
    print('%-12s %5d %6d %7.2f %7.2f %7.2f %7.2f %7.0f %5.1f%% %5.1f%% %6.1f %5.2f %5.1f%%' % (s, len(r), stalls.get(s, 0), vals[0], vals[1], vals[2], vals[3], vals[4],
          100 * sum(1 for x in fr if x <= 34.0) / len(fr), 100 * sum(1 for x in fr if x <= 50.0) / len(fr), fr[int(0.95 * (len(fr) - 1))], vals[5], 100 * sum(1 for x in c[5] if x <= 2) / len(fr)))
    if base and s != 'all on':
        print('%-12s %5s %6s %+7.2f %+7.2f %+7.2f %+7.2f %+7.0f   (off minus on)' % ('', '', '', *[vals[i] - base[i] for i in range(5)]))
