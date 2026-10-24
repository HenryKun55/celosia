#!/usr/bin/env python3
"""How much of the game's code is native C++ (decompiled) instead of
recompiled MIPS: docs/decomp_status.json and docs/decomp_status.svg.

The functions are the recompiler's (build/runner/register_functions.cpp: every
table entry whose slot is the function's own address), sized from
build/recomp/game/functions.csv. A function counts as native when
  - the runtime implements it (build/runner/ps2_recompiled_stubs.h: kernel
    calls, SDK and libc pieces), or
  - a game override replaces its table entry at run time: a run with
    PS2X_REPLACED_DUMP=<file> and PS2X_EXIT_AT_FRAME=<n> writes those
    addresses (default env, so no profiler / difftest wrappers).
A replaced address inside a function (a native that resumes the recompiled
code at a label) makes that function "partial".

Three figures: by function count, by MIPS code size, and by calls (each
function weighted by its call count in game/lean_hot.txt's sources, the
SOTC_CALL_COUNT dumps given with --calls; without them that figure is left
out).

Usage:
  tools/decomp_status.py --replaced replaced.txt [--calls calls.txt ...]
"""
import argparse
import csv
import datetime
import glob
import html
import json
import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from status_treemap import squarify, wrap  # noqa: E402

# Areas, by the first matching pattern of the function's name (game code at
# 0x1000000 and up: GAMECORE; below: the boot ELF, kernel, SDK and libc).
AREAS = [
    ('Characters and motion', r'chara|motion|skeleton|bone|joint|pose|ik|gaze|quat|euler|curve|anim|cloth|bridle|wander|horse|agro|player|enemy|boss|colossus|grip|climb'),
    ('Display and models', r'disp|model|mesh|draw|layout|lod|clip|sort|shadow|texture|tex|sprite|gl|vram|light|fog|blur|bloom|render|screen|color|colour'),
    ('Particles and effects', r'ptcl|particle|effect|eff|splash|dust|smoke|fire|water|rain|wave'),
    ('Camera', r'camera|cam'),
    ('Collision and physics', r'collision|coll|hit|rigid|phys|dyn|ground|fit|swim|pendulum|volume'),
    ('Script and events', r'scr|script|event|cond|mission|demo|stage|area'),
    ('Sound', r'snd|sound|sd|spu|voice|bgm|se_|adpcm|audio|seq'),
    ('System and memory', r'ios|sys|heap|malloc|free|thread|task|dma|vif|gif|gs|pad|cd|file|load|mc|memcard|sif|iop|rpc|intc|alarm'),
    ('Math', r'math|vec|mat|vector|matrix|sin|cos|tan|sqrt|rand|interp|lerp|rate'),
]
LIB_AREA = 'Boot ELF, SDK and libc'
# A native's area from the file it lives in (its symbol is often a bare
# manager_sub_<address>).
SOURCE_AREAS = [
    (r'decomp_(bridle|chara|cloth|clothdyn|clothstep|curve|quat|skeleton|skinpal)\.cpp', 'Characters and motion'),
    (r'decomp_(layout|light|modelclip|pointsort|prefetch|sortlist|texanim|texreset)\.cpp|sotc_native\.cpp', 'Display and models'),
    (r'decomp_(particles|ptcldraw|ptclupdate)\.cpp', 'Particles and effects'),
    (r'decomp_collision\.cpp', 'Collision and physics'),
    (r'decomp_(math|softfloat)\.cpp', 'Math'),
    (r'decomp_(gctick|subthread|time|keyboard)\.cpp', 'System and memory'),
]


def area_of(address, name, source=''):
    for pattern, area in SOURCE_AREAS:
        if source and re.search(pattern, source):
            return area
    if address < 0x1000000:
        return LIB_AREA
    low = name.lower()
    for area, pattern in AREAS:
        if re.search(pattern, low):
            return area
    return 'Other game code'


def recompiled_functions():
    text = open(os.path.join(ROOT, 'build/runner/register_functions.cpp')).read()
    out = {}
    for m in re.finditer(r'= (\w+)_0x([0-9a-f]+); // 0x([0-9a-f]+)', text):
        if m.group(2) == m.group(3):
            out[int(m.group(2), 16)] = m.group(1)
    return out


def stub_functions():
    text = open(os.path.join(ROOT, 'build/runner/ps2_recompiled_stubs.h')).read()
    return {int(a, 16) for _, a in re.findall(r'void (\w+)_0x([0-9a-f]+)\(', text)}


def sizes():
    out = {}
    for row in csv.DictReader(open(os.path.join(ROOT, 'build/recomp/game/functions.csv'))):
        out[int(row['Start'], 16)] = int(row['Size'])
    return out


def call_counts(paths):
    counts = {}
    for path in paths:
        for line in open(path):
            m = re.match(r'\s*(?:0x)?([0-9a-fA-F]{6,8})\s+(\d+)', line)
            if m:
                a = int(m.group(1), 16)
                counts[a] = counts.get(a, 0) + int(m.group(2))
    return counts


def source_files(addresses):
    """address -> the override / runtime file that names it (first match)."""
    files = glob.glob(os.path.join(ROOT, 'game/overrides/*.cpp')) + glob.glob(os.path.join(ROOT, 'PS2Recomp/ps2xRuntime/src/lib/**/*.cpp'), recursive=True)
    texts = {}
    for f in files:
        if '/lean_part' in f:
            continue
        try:
            texts[os.path.relpath(f, ROOT)] = open(f, errors='replace').read().lower()
        except OSError:
            pass
    out = {}
    for a in addresses:
        keys = ['0x%08x' % a, '0x%x' % a]
        for path, text in texts.items():
            if any(re.search(re.escape(k) + r'(?![0-9a-f])', text) for k in keys):
                out[a] = path
                break
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--replaced', required=True, help='PS2X_REPLACED_DUMP output')
    ap.add_argument('--calls', nargs='*', default=[], help='SOTC_CALL_COUNT dumps')
    ap.add_argument('--json', default=os.path.join(ROOT, 'docs/decomp_status.json'))
    ap.add_argument('--svg', default=os.path.join(ROOT, 'docs/decomp_status.svg'))
    args = ap.parse_args()

    funcs = recompiled_functions()
    stubs = stub_functions()
    size = sizes()
    replaced = {int(l.split()[0], 16) for l in open(args.replaced) if l.strip()}
    calls = call_counts(args.calls)

    starts = sorted(funcs)
    import bisect

    def containing(a):
        i = bisect.bisect_right(starts, a) - 1
        return starts[i] if i >= 0 else None

    status = {a: 'recompiled' for a in funcs}
    for a in stubs:
        if a in status:
            status[a] = 'runtime'
    for a in replaced:
        if a in status:
            status[a] = 'native'
        else:
            c = containing(a)
            if c is not None and status[c] == 'recompiled':
                status[c] = 'partial'

    native_like = ('native', 'runtime')

    def figure(weight):
        total = sum(weight(a) for a in funcs)
        done = sum(weight(a) for a in funcs if status[a] in native_like)
        part = sum(weight(a) for a in funcs if status[a] == 'partial')
        return {'native': done, 'partial': part, 'total': total, 'percent': round(100.0 * (done + 0.5 * part) / total, 2) if total else 0.0}

    summary = {
        'by_functions': figure(lambda a: 1),
        'by_code_size': figure(lambda a: size.get(a, 4)),
    }
    if calls:
        summary['by_calls'] = figure(lambda a: calls.get(a, 0))
    game = [a for a in funcs if a >= 0x1000000]
    summary['game_code_by_size'] = {
        'native': sum(size.get(a, 4) for a in game if status[a] in native_like),
        'total': sum(size.get(a, 4) for a in game),
    }
    summary['game_code_by_size']['percent'] = round(100.0 * summary['game_code_by_size']['native'] / summary['game_code_by_size']['total'], 2)

    files = source_files([a for a, s in status.items() if s in ('native', 'partial')])
    areas = {}
    for a, name in funcs.items():
        ar = areas.setdefault(area_of(a, name, files.get(a, '')), {'functions': 0, 'native_functions': 0, 'bytes': 0, 'native_bytes': 0, 'partial_functions': 0})
        ar['functions'] += 1
        ar['bytes'] += size.get(a, 4)
        if status[a] in native_like:
            ar['native_functions'] += 1
            ar['native_bytes'] += size.get(a, 4)
        elif status[a] == 'partial':
            ar['partial_functions'] += 1
    for ar in areas.values():
        ar['percent'] = round(100.0 * ar['native_bytes'] / ar['bytes'], 2) if ar['bytes'] else 0.0

    try:
        commit = subprocess.check_output(['git', '-C', ROOT, 'rev-parse', '--short', 'HEAD'], text=True).strip()
    except (OSError, subprocess.CalledProcessError):
        commit = ''
    natives = [{'address': '0x%08x' % a, 'name': funcs[a], 'bytes': size.get(a, 4), 'status': status[a],
                'area': area_of(a, funcs[a], files.get(a, '')), 'source': files.get(a, '')}
               for a in sorted(funcs) if status[a] in ('native', 'partial')]
    out = {'updated': datetime.date.today().isoformat(), 'commit': commit, 'summary': summary,
           'areas': areas, 'native_functions': natives, 'runtime_functions': len([a for a in funcs if status[a] == 'runtime'])}
    with open(args.json, 'w') as f:
        json.dump(out, f, indent=1)
        f.write('\n')
    draw_svg(out, args.svg)

    for k, v in summary.items():
        print('%-18s %6.2f%%  (%s of %s)' % (k, v['percent'], v['native'], v['total']))
    for name, ar in sorted(areas.items(), key=lambda t: -t[1]['bytes']):
        print('  %-26s %6.2f%%  %4d/%-5d functions' % (name, ar['percent'], ar['native_functions'], ar['functions']))


def draw_svg(data, path):
    W, H, top = 960, 520, 64
    areas = data['areas']
    tiles = squarify([(ar['bytes'], (name, ar)) for name, ar in areas.items()], 0, top, W, H - top)
    s = data['summary']
    parts = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" font-family="Helvetica, Arial, sans-serif">' % (W, H),
             '<rect width="100%" height="100%" fill="#1d1f24"/>',
             '<text x="12" y="28" font-size="20" fill="#f2f2f2">Decompiled: %.1f%% of the code (%.1f%% of the game code), %d of %d functions</text>'
             % (s['by_code_size']['percent'], s['game_code_by_size']['percent'], s['by_functions']['native'], s['by_functions']['total']),
             '<text x="12" y="50" font-size="13" fill="#b9bcc4">Tile area: MIPS code size. Green: native C++; red: still recompiled. %s, %s</text>'
             % (html.escape(data['updated']), html.escape(data['commit']))]
    for x, y, w, h, (name, ar) in tiles:
        frac = ar['native_bytes'] / ar['bytes'] if ar['bytes'] else 0.0
        parts.append('<rect x="%.1f" y="%.1f" width="%.1f" height="%.1f" fill="#c8553d" stroke="#1d1f24" stroke-width="2"/>' % (x, y, w, h))
        parts.append('<rect x="%.1f" y="%.1f" width="%.1f" height="%.1f" fill="#2f9e5b"/>' % (x + 1, y + 1 + (h - 2) * (1 - frac), max(0, w - 2), max(0, (h - 2) * frac)))
        size = 13 if w > 120 else 10
        lines = wrap(name, w - 8, size) + ['%.1f%%  %d/%d' % (100 * frac, ar['native_functions'], ar['functions'])]
        for i, line in enumerate(lines):
            if 16 + i * (size + 3) > h - 4:
                break
            parts.append('<text x="%.1f" y="%.1f" font-size="%d" fill="#ffffff">%s</text>' % (x + 5, y + 16 + i * (size + 3), size, html.escape(line)))
    parts.append('</svg>')
    with open(path, 'w') as f:
        f.write('\n'.join(parts) + '\n')


if __name__ == '__main__':
    main()
