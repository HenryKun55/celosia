#!/usr/bin/env python3
"""Round DC1: the character chain's functions and how far each is from real C++.

The chain: every guest function reachable by direct jal from the per-frame character entries (charaGeometry,
charaTail, ExecCharaGeometryPreprocess / Postprocess, charaCtrl and its control variants, Agro's horseStep) plus the
calls through pointers the area is known to make (the IK callbacks, the gaze workers), walking only through the
character / math / "other game code" areas (a jal into collision, camera, display, sound, script or system code is
counted as a boundary, not walked). The jal graph comes from the recompiled code's MIPS comments.

Classes:
  recompiled  generated code only (no hand native, no lift)
  lifted      a lift_d* body (C++ on the guest context, made by tools/lift_d1.py)
  ctx-native  hand native that still runs on R5900Context registers / guest frames / callGuest
  real        hand native whose work is a ctx-free C++ function on typed views and host pointers (the guest-signature
              wrapper only unpacks the arguments and returns), listed in REAL below with the round that made it

Usage: tools/dc1_chain.py [--build DIR] [--list] [--json out.json]
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from decomp_status import area_of  # noqa: E402

ROOTS = [0x013CFBE0, 0x013CF4E0, 0x010C0B30, 0x010C0BE0, 0x010CABA0, 0x010CBF08, 0x010CBF40, 0x010CBF78, 0x01391E30]
# Known pointer calls of the area (IK callbacks, gaze worker, cloth through the layout class).
POINTER = [0x010C6238, 0x013703F0, 0x01374498, 0x010B98E0, 0x010BA5D8, 0x01137A68, 0x011399E0, 0x013B8DC8, 0x013978E8,
           0x013976B8, 0x010C2F88, 0x010C2ED0, 0x010E1EF8, 0x010C1018, 0x010C3328]
WALK_AREAS = {'Characters and motion', 'Math', 'Other game code', 'Boot ELF, SDK and libc'}
BOOT_MATH = re.compile(r'quat|matrix|euler|atan|asin|acos|sin|cos|sqrt|vector|ios(Get|Set|Push|Pop|Mul|Trans|Unit|Rot|Inv)', re.I)

# Hand natives whose work is real C++ (ctx-free core on typed views; wrapper unpacks only). Address -> round.
REAL = {
    0x010B1D60: 'S3 (s3chara pose over the host rig; pose82_math)',
    0x010AD1B0: 'C1 (keys122 decodeKey + c1keys rows)',
    # DC1
    0x010B28E0: 'DC1', 0x010B3810: 'DC1', 0x010B39B0: 'DC1', 0x010B36C8: 'DC1', 0x010C6710: 'DC1',
    0x010B6E80: 'DC1', 0x010B6F60: 'DC1', 0x010C6798: 'DC1', 0x010BC4F0: 'DC1',
}
DC1_BEFORE = {}  # filled from --before if given


def functions(build):
    rows = [l.strip().split(',') for l in open(os.path.join(build, 'recomp/game/functions.csv')).readlines()[1:]]
    return {int(r[1], 16): (r[0], int(r[3])) for r in rows}


def jal_graph(build):
    g = {}
    for path in glob.glob(os.path.join(build, 'recomp/game/output/*_0x*.cpp')):
        m = re.search(r'_0x([0-9a-f]+)\.cpp$', path)
        if not m:
            continue
        a = int(m.group(1), 16)
        s = g.setdefault(a, set())
        with open(path, errors='replace') as f:
            for line in f:
                j = re.search(r'\bjal\s+func_([0-9A-Fa-f]+)', line)
                if j:
                    s.add(int(j.group(1), 16))
    return g


def registrations():
    """Hand natives: SOTC_DECOMP_*FUNCTION / COROUTINE / UNCHECKED / registerFunction with a literal or a
    constexpr name as the address. Lifted: SOTC_DECOMP_LIFT_FUNCTION* or a lift_d* body comment."""
    hand, lifted = {}, {}
    reg = re.compile(r'(SOTC_DECOMP_(?:AB_|FLOAT_|SWITCH_|SWITCH_ONCE_|LIFT_)?(?:FUNCTION(?:_D\d+)?|COROUTINE|UNCHECKED)|registerFunction)'
                     r'\(\s*([A-Za-z_0-9:]+?)u?\s*,\s*([A-Za-z_0-9:<>]+)(?:\s*,\s*([A-Za-z_0-9:<>]+))?')
    for path in sorted(glob.glob(os.path.join(ROOT, 'game/overrides/*.cpp'))):
        base = os.path.basename(path)
        text = open(path, errors='replace').read()
        consts = {m.group(1): int(m.group(2), 16) for m in re.finditer(r'\b(k[A-Za-z_0-9]+)\s*=\s*0x([0-9A-Fa-f]+)u?\b', text)}
        if base.startswith('decomp_liftd'):
            for m in re.finditer(r'^// ([A-Za-z_0-9]+) \(0x([0-9a-f]+)-0x[0-9a-f]+\)', text, re.M):
                lifted.setdefault(int(m.group(2), 16), base)
        for m in reg.finditer(text):
            tok = m.group(2).split('::')[-1]
            if re.fullmatch(r'0x[0-9A-Fa-f]+', tok):
                addr = int(tok, 16)
            elif tok in consts:
                addr = consts[tok]
            else:
                continue
            if 'LIFT' in m.group(1):
                lifted.setdefault(addr, base)
            elif not base.startswith('decomp_liftd'):
                hand.setdefault(addr, (base, m.group(4) or m.group(3)))
    return hand, lifted


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', default=os.environ.get('CELOSIA_BUILD', os.path.join(ROOT, 'build')))
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--json')
    ap.add_argument('--no-dc1', action='store_true', help='count as before DC1 (DC1 rows as ctx-native)')
    a = ap.parse_args()
    fns = functions(a.build)
    g = jal_graph(a.build)
    hand, lifted = registrations()
    seen, boundary = set(), set()
    stack = [r for r in ROOTS + POINTER if r in fns]
    while stack:
        f = stack.pop()
        if f in seen:
            continue
        name = fns[f][0]
        area = area_of(f, name)
        if f not in ROOTS and f not in POINTER:
            if area not in WALK_AREAS or (area == 'Boot ELF, SDK and libc' and not BOOT_MATH.search(name)):
                boundary.add(f)
                continue
        seen.add(f)
        for t in g.get(f, ()):
            if t in fns and t not in seen:
                stack.append(t)
    rows = []
    for f in sorted(seen):
        name, size = fns[f]
        if f in REAL and not (a.no_dc1 and REAL[f] == 'DC1'):
            cls = 'real'
        elif f in hand:
            cls = 'ctx-native'
        elif f in lifted:
            cls = 'lifted'
        else:
            cls = 'recompiled'
        rows.append({'address': f'0x{f:08x}', 'name': name, 'bytes': size, 'class': cls,
                     'source': hand.get(f, (lifted.get(f, ''), ''))[0]})
    counts = {}
    for r in rows:
        c = counts.setdefault(r['class'], [0, 0])
        c[0] += 1
        c[1] += r['bytes']
    total = sum(c[0] for c in counts.values())
    print(f'chain: {total} functions ({len(boundary)} boundary calls into other areas not walked)')
    for k in ('real', 'ctx-native', 'lifted', 'recompiled'):
        n, b = counts.get(k, [0, 0])
        print(f'  {k:11s} {n:5d}  {b:7d} bytes')
    if a.list:
        for r in rows:
            print(f"{r['address']} {r['class']:11s} {r['bytes']:6d} {r['name']} {r['source']}")
    if a.json:
        json.dump({'counts': counts, 'rows': rows}, open(a.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
