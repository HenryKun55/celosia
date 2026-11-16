#!/usr/bin/env python3
"""mixframes.py <err.txt> [--seed N] [--period N] [--from F] [--to F] [--block N] [--stall MS] [--shift S]:
an SOTC_AB_MIX run (the A/B flag on or off at random each window, the same hash as sotc_native's abSwitches) with
SOTC_FRAME_LOG=1 SOTC_FRAME_LOG_ALL=1 ("[frame] each/slow N: X ms: hooks H ... wait W ..."). Per frame: its state
(off = the flag's windows), frame ms, busy = frame - wait (the game thread's CPU incl. its hook work).
Paired estimate: frames grouped in blocks of --block consecutive frames (default 300: the same content), in each block
mean(off) - mean(on); the blocks' differences averaged (weight min(n_on, n_off)), +- 2 standard errors over blocks.
Positive = the flag ON saves that much per frame. Frames >= --stall ms (loads, host stalls) left out.
--shift (default -1): frame f's state is the hash of window (f + shift) // period. abSwitches() runs in endFrame and sets
the switches for the next frame from the number of the frame that just ended, so the work logged as frame f ran in the
state hashed from f - 1 (round S3's finding; D7-S2 read --shift 0, which pairs each time with an unrelated state: their
frame-time A/Bs read ~0 by construction). Kept as an option for a check: tools/frametime.py --scan prints shifts -3..3."""
import re, sys, math
args = sys.argv[1:]
def opt(name, d):
    if name in args:
        i = args.index(name); v = args[i + 1]; del args[i:i + 2]; return float(v)
    return d
seed, period = int(opt('--seed', 7)), int(opt('--period', 1))
lo, hi, block, stall = opt('--from', 0), opt('--to', 1e18), int(opt('--block', 300)), opt('--stall', 200)
# S3: the switch for frame f's work is set by abSwitches() at the END of the frame before (sotc_native endFrame, from
# that frame's number), and the [frame] line of a frame carries the next number: frame f ran in the state hashed from
# f - 1. Default --shift -1 (S1 / S2 used 0, which compares unrelated states: their frame A/Bs read ~0 by construction).
shift = int(opt('--shift', -1))
M = (1 << 64) - 1
def off(frame):
    w = frame // period
    z = (w * 0x9E3779B97F4A7C15 + seed * 0xBF58476D1CE4E5B9 + 0x94D049BB133111EB) & M
    z = ((z ^ (z >> 30)) * 0xBF58476D1CE4E5B9) & M
    z = ((z ^ (z >> 27)) * 0x94D049BB133111EB) & M
    z ^= z >> 31
    return (z >> 63) != 0
fr = re.compile(r'^\[frame\] (?:each|slow) (\d+): ([\d.]+) ms: hooks ([\d.]+).*? wait ([\d.]+) draws (\d+)')
rows = []
skipped = 0
for line in open(args[0], errors='replace'):
    m = fr.match(line)
    if not m:
        continue
    f = int(m.group(1))
    if f < lo or f >= hi or f < period:
        continue
    ms, hooks, wait, draws = float(m.group(2)), float(m.group(3)), float(m.group(4)), int(m.group(5))
    if ms >= stall:
        skipped += 1
        continue
    rows.append((f, off(f + shift), ms, ms - wait, draws))
if not rows:
    sys.exit('no frames')
def mean(v):
    return sum(v) / len(v) if v else float('nan')
on = [r for r in rows if not r[1]]; of = [r for r in rows if r[1]]
print(f'{len(rows)} frames ({len(on)} on, {len(of)} off), {skipped} at >= {stall:.0f} ms left out; frames {rows[0][0]}-{rows[-1][0]}')
print(f'means: frame on {mean([r[2] for r in on]):.2f} off {mean([r[2] for r in of]):.2f} ms; busy on {mean([r[3] for r in on]):.2f} off {mean([r[3] for r in of]):.2f}; draws on {mean([r[4] for r in on]):.0f} off {mean([r[4] for r in of]):.0f}')
for col, name in ((2, 'frame'), (3, 'busy')):
    diffs = []
    b0 = rows[0][0]
    groups = {}
    for r in rows:
        groups.setdefault((r[0] - b0) // block, []).append(r)
    for g in groups.values():
        a = [r[col] for r in g if not r[1]]; b = [r[col] for r in g if r[1]]
        if len(a) >= 5 and len(b) >= 5:
            diffs.append((mean(b) - mean(a), min(len(a), len(b))))
    if len(diffs) < 3:
        print(f'{name}: too few blocks'); continue
    W = sum(w for _, w in diffs); d = sum(x * w for x, w in diffs) / W
    var = sum(w * (x - d) ** 2 for x, w in diffs) / W
    se = math.sqrt(var / len(diffs))
    print(f'{name}: off - on = {d:+.3f} ms +- {2 * se:.3f} (2 SE over {len(diffs)} blocks of {block} frames)')
