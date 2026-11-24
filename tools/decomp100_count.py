#!/usr/bin/env python3
"""Where the game's code stands on the way to 100% native C++ (docs/decomp_100_plan.md).

Every guest function the recompiler knows (build/runner/register_functions.cpp: the boot ELF plus the KERNEL,
MANAGER and GAMECORE XFF modules linked by tools/build_elf.py) is put in one class:

  runtime   the runtime implements it (build/runner/ps2_recompiled_stubs.h: EE kernel calls, SDK, libc)
  recomp    only the ps2xRecomp output runs (MIPS-shaped generated C++ on the guest context)
  lifted    tools/lift_d1.py's output holds the slot (game/overrides/decomp_lift*.cpp: host locals, still the
            guest context at calls, guest RAM addressing, PS2 FPU / VU0 macros)
  native    a hand-written override holds the slot. All of them still have the guest signature
            (rdram, ctx, runtime): no table entry can be anything else today. They are split by what the body does:
              native-ps2   the body still emulates PS2 semantics (FPU_* / PS2_V* / vu:: macros, ctx registers past the
                           arguments) or calls guest code (callGuest, recompiled(), PS2_CALL_GUEST)
              native-host  host code behind a guest adapter (arguments from ctx, result to v0; no PS2 math macros, no
                           guest calls): what becomes real native C++ once layer 10 (the signature) goes
  typed     (d) real C++ (rounds A1 / A2 / A3 / A4 / A5, tools/typed_lift.py / typed_lift2.py): the slot's lifted function is a guest adapter in front of a
            typed function (game/overrides/typed_a1.inc: parameters and return values, no ctx, no guest calls, no PS2
            FPU / VU0 macros, no READ / WRITE macros), and typed callers call it directly. Counted when the function is
            listed in docs/a1_typed.txt, docs/a2_typed.txt, docs/a3_typed.txt, docs/a4_typed.txt or docs/a5_typed.txt, its lifted function holds the slot and its typed body has none of those markers.
            (The table entry itself, the adapter, goes when every caller calls the typed function: the plan's E1.)

Which slots are held by overrides comes from a PS2X_REPLACED_DUMP run (decomp_status.py's input). Without --replaced,
the native list docs/decomp_status.json keeps from the last dump is used, plus the natives registered statically since.
Lifted / native are told apart statically: the lift tables (SOTC_DECOMP_LIFT_FUNCTION*) against the hand registrars
(SOTC_DECOMP_FUNCTION family, registerFunction with a constant address). A lifted function whose slot a native holds is
native (sotc_decomp::install never puts a lift over another holder).

Weights: function count, MIPS bytes (build/recomp/game/functions.csv) and, with --prof, the game thread's running
samples on the console (SOTC_PROF129 logs, tools/prof129.py's parser): each sample's pc is charged to the guest function
that contains it (a native's return sites are inside its own guest function, so they are charged to the native).

Usage:
  tools/decomp100_count.py [--replaced replaced.txt] [--prof err.txt:1-2 ...] [--json out.json]
"""
import argparse
import bisect
import collections
import csv
import glob
import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))


def build_dir():
    """build/ of this checkout, else the main checkout's (a worktree has none)."""
    b = os.path.join(ROOT, 'build')
    if os.path.isdir(os.path.join(b, 'runner')):
        return b
    m = re.match(r'(.*)/\.claude/worktrees/[^/]+$', ROOT)
    if m and os.path.isdir(os.path.join(m.group(1), 'build/runner')):
        return os.path.join(m.group(1), 'build')
    sys.exit('no build/runner (the recompiled runner) found')


BUILD = build_dir()
OVR = os.path.join(ROOT, 'game/overrides')

PS2_MATH = re.compile(r'\bFPU_[A-Z_]+\(|\bPS2_V[A-Z0-9_]+\(|\bvu::|ps2VuClamp|ps2FpuCompareKey|\bvf\[|_mm_blendv_ps')
GUEST_CALL = re.compile(r'callGuest|PS2_CALL_GUEST|\brecompiled\(|finishGuestCall|lookupFunction')
CTX_REG = re.compile(r'ctx->(r|f|vu0|hi|lo|sa|fcr31|pc)\b|\bGPR_[A-Z0-9]+\(|\bSET_GPR|\bFPR_')
GUEST_MEM = re.compile(r'\bREAD(8|16|32|64|128)\w*\(|\bWRITE(8|16|32|64|128)\w*\(|rdram\s*\+|rdram\[|PS2_RAM_MASK')


def table_functions():
    text = open(os.path.join(BUILD, 'runner/register_functions.cpp')).read()
    out = {}
    for m in re.finditer(r'= (\w+)_0x([0-9a-f]+); // 0x([0-9a-f]+)', text):
        if m.group(2) == m.group(3):
            out[int(m.group(2), 16)] = m.group(1)
    return out


def stub_functions():
    text = open(os.path.join(BUILD, 'runner/ps2_recompiled_stubs.h')).read()
    return {int(a, 16) for _, a in re.findall(r'void (\w+)_0x([0-9a-f]+)\(', text)}


def sizes():
    return {int(r['Start'], 16): int(r['Size']) for r in csv.DictReader(open(os.path.join(BUILD, 'recomp/game/functions.csv')))}


def lift_files():
    return sorted(glob.glob(os.path.join(OVR, 'decomp_lift*.cpp')))


def native_files():
    out = []
    for f in sorted(glob.glob(os.path.join(OVR, '*.cpp'))):
        b = os.path.basename(f)
        if b.startswith(('decomp_lift', 'lean_part')):
            continue
        out.append(f)
    return out


TYPED_BAD = re.compile(r'\bctx\b|\b_mm_\w+\(|\bFPU_[A-Z_]+\(|\bPS2_V[A-Z0-9_]+\(|\bREAD(8|16|32|64|128)\w*\(|\bWRITE(8|16|32|64|128)\w*\(|'
                       r'callGuest|PS2_CALL_GUEST|PS2_TAIL|PS2_JUMP|PS2_CHECKPOINT|runtime->|\brdram\b')


def typed_functions():
    """Rounds A1 / A2 / A3: address -> name of the functions docs/a1_typed.txt ... a5_typed.txt list whose
    typed body (typed_a1.inc ... typed_a5.inc) has no ctx, no guest call / checkpoint, no PS2 FPU / VU0 macro and no guest READ /
    WRITE macro (A2's frames are host arrays, its calls C++ calls to typed functions)."""
    out, bad = {}, 0
    for lst, inc, tool in (('docs/a1_typed.txt', 'typed_a1.inc', 'typed_lift.py'), ('docs/a2_typed.txt', 'typed_a2.inc', 'typed_lift2.py'),
                           ('docs/a3_typed.txt', 'typed_a3.inc', 'typed_lift2.py'), ('docs/a4_typed.txt', 'typed_a4.inc', 'typed_lift2.py'),
                           ('docs/a5_typed.txt', 'typed_a5.inc', 'typed_lift2.py'), ('docs/a6_typed.txt', 'typed_a6.inc', 'typed_lift2.py'),
                           ('docs/a7_typed.txt', 'typed_a7.inc', 'typed_lift2.py'), ('docs/a8_typed.txt', 'typed_a8.inc', 'typed_lift2.py'),
                           ('docs/a9_typed.txt', 'typed_a9.inc', 'typed_lift2.py'), ('docs/a10_typed.txt', 'typed_a10.inc', 'typed_lift2.py'), ('docs/a11_typed.txt', 'typed_a11.inc', 'typed_lift2.py'), ('docs/a12_typed.txt', 'typed_a12.inc', 'typed_lift2.py'), ('docs/a13_typed.txt', 'typed_a13.inc', 'typed_lift2.py'), ('docs/a14_typed.txt', 'typed_a14.inc', 'typed_lift2.py')):
        lst = os.path.join(ROOT, lst)
        inc = os.path.join(OVR, inc)
        if not (os.path.exists(lst) and os.path.exists(inc)):
            continue
        bodies = {}
        for m in re.finditer(r'^// (\w+) \(0x([0-9a-f]+)\): typed by tools/' + re.escape(tool) + r'.*?\n(.*?)(?=^// \w+ \(0x[0-9a-f]+\): typed|\Z)',
                             open(inc).read(), re.M | re.S):
            bodies[int(m.group(2), 16)] = (m.group(1), m.group(3))
        for line in open(lst):
            if line.startswith('#') or not line.strip():
                continue
            if ' | variant' in line or '| variant ' in line:
                continue  # rounds A7 / A8: a variant (<name>_x7 / _x8) of a function an earlier round typed: counted there
            a = int(line.split()[0], 16)
            if a not in bodies:
                continue
            name, body = bodies[a]
            if TYPED_BAD.search(body):
                bad += 1
                continue
            out[a] = name
    return out, bad


def a6_adapted():
    """Round A6: the never-lifted functions stage 6 typed whose slot its adapter takes (docs/a6_typed.txt's slot holder ::a6ad::):
    the table runs the typed function (the recompiled one only as the adapter's fallback), as a lifted function's adapter does."""
    out = set()
    for f, mark in (('docs/a6_typed.txt', '::a6ad::'), ('docs/a7_typed.txt', '::a7ad::'), ('docs/a8_typed.txt', '::a8ad::'), ('docs/a9_typed.txt', '::a9ad::'), ('docs/a10_typed.txt', '::a10ad::'), ('docs/a11_typed.txt', '::a11ad::'), ('docs/a12_typed.txt', '::a12ad::'), ('docs/a13_typed.txt', '::a13ad::'), ('docs/a14_typed.txt', '::a14ad::')):  # round A7: its adapters likewise
        p = os.path.join(ROOT, f)
        if os.path.exists(p):
            out |= {int(l.split()[0], 16) for l in open(p) if not l.startswith('#') and mark in l}
    return out


def d1_natives():
    """Round D1 (docs/decomp_d1.md): the hand natives with a D1 body (switch d1native), docs/d1_natives.txt:
    `<address> | <registration name> | <file> | <adapter fn> | <core fn> | full|partial | <what changed>`.
    A `full` one counts as (d) when its core (the function the adapter calls with C++ parameters, and its same-file
    callees, three levels) has none of TYPED_BAD's markers (no ctx, no guest call, no PS2 FPU / VU0 macro, no READ /
    WRITE macro, no rdram, no runtime->, no _mm_ intrinsic): the typed bar of rounds A1-A14 applied to a hand native.
    Returns address -> (file, adapter, core, kind, clean)."""
    p = os.path.join(ROOT, 'docs/d1_natives.txt')
    out = {}
    if not os.path.exists(p):
        return out
    for line in open(p):
        if line.startswith('#') or not line.strip():
            continue
        f = [x.strip() for x in line.split('|')]
        a, name, file, adapter, core, kind = int(f[0], 16), f[1], f[2], f[3], f[4], f[5]
        clean = False
        if kind == 'full':
            text = ''
            for c in core.split(','):
                t, found = body_closure(os.path.join(OVR, file), c.strip())
                if not found:
                    text = None
                    break
                text += t
            clean = text is not None and not TYPED_BAD.search(text)
        out[a] = (file, adapter, core, kind, clean)
    return out


def lifted_addresses():
    out = {}
    pat = re.compile(r'SOTC_DECOMP_LIFT_FUNCTION(?:_D\d+)?\(\s*(0x[0-9A-Fa-f]+)u?\s*,\s*(\w+)')
    for f in lift_files():
        for m in pat.finditer(open(f, errors='replace').read()):
            out[int(m.group(1), 16)] = (os.path.basename(f), m.group(2))
    return out


REG = re.compile(r'SOTC_DECOMP_(?:FUNCTION|AB_FUNCTION|FLOAT_FUNCTION|COROUTINE|UNCHECKED|SWITCH_FUNCTION|SWITCH_ONCE_FUNCTION)'
                 r'\(\s*([\w:]+)\s*,\s*(\w+)\s*,\s*([\w:<>]+)\s*\)')
RUNREG = re.compile(r'registerFunction\(\s*([\w:]+)\s*,\s*([\w:<>&]+)')
CONST = re.compile(r'\b(k\w+)\s*=\s*(0x[0-9A-Fa-f]{5,8})u?\b')
# Diagnostic tools: their hooks are off in the default env (replay, profiler wrappers).
DEBUG_FILES = ('sotc_replay.cpp', 'sotc_prof.cpp', 'sotc_prof129.cpp')
# Wrappers in front of a function (join gates, census / dump hooks): the body behind them is the generated code.
WRAPPER_FN = ('wrap', 'gate', 'tracers', 'hooks', 'wrappers', 'checkers', 'heapHook', 'meshDumpHook', 'modelDispDumpHook',
              'loaderGate', 'probe', 'to', 'f.native', 'h.fn', 'p.', 'e.')


def _addr(tok, consts):
    tok = tok.rstrip('u')
    if tok.startswith('0x'):
        return int(tok, 16)
    return consts.get(tok.split('::')[-1])


def native_registrations():
    """address -> (file, name, fn, how) for every hand registrar whose address is a constant. how: 'macro' (the
    SOTC_DECOMP_* registrars, installed by sotc_decomp::install) or 'runtime' (a registerFunction call)."""
    out = {}
    for f in native_files():
        text = open(f, errors='replace').read()
        b = os.path.basename(f)
        consts = {k: int(v, 16) for k, v in CONST.findall(text)}
        for m in REG.finditer(text):
            a = _addr(m.group(1), consts)
            if a is not None:
                out.setdefault(a, (b, m.group(2), m.group(3), 'macro'))
        if b in DEBUG_FILES:
            continue
        for m in RUNREG.finditer(text):
            a = _addr(m.group(1), consts)
            fn = m.group(2)
            if a is None or fn.startswith(WRAPPER_FN):
                continue
            if a not in out:
                out[a] = (b, fn, fn, 'runtime')
    return out


# ---- the bodies of the natives -------------------------------------------------------------------------------------

_defs_cache = {}


def definitions(path):
    """name -> body text for every brace-delimited function definition in a file (rough C++ scan: good enough to read
    the markers of a body; templates / overloads of one name are concatenated)."""
    if path in _defs_cache:
        return _defs_cache[path]
    text = open(path, errors='replace').read()
    # strip comments and strings
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    text = re.sub(r'"(\\.|[^"\\\n])*"', '""', text)
    defs = collections.defaultdict(str)
    head = re.compile(r'([A-Za-z_]\w*)\s*\(([^;{}()]|\([^;{}()]*\))*\)\s*(const\s*)?(noexcept\s*)?(->\s*[\w:<>* ]+)?\s*\{')
    pos = 0
    while True:
        m = head.search(text, pos)
        if not m:
            break
        name = m.group(1)
        if name in ('if', 'for', 'while', 'switch', 'catch', 'return', 'sizeof', 'defined'):
            pos = m.end()
            continue
        depth, i = 1, m.end()
        while i < len(text) and depth:
            c = text[i]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
            i += 1
        defs[name] += m.group(0) + text[m.end():i] + '\n'
        pos = i
    _defs_cache[path] = defs
    return defs


def body_closure(path, fn, depth=3):
    """The text of fn's body and of the same-file functions it calls, `depth` levels down."""
    defs = definitions(path)
    name = re.sub(r'<.*', '', fn.split('::')[-1])
    seen, todo, text = set(), [(name, 0)], []
    while todo:
        n, d = todo.pop()
        if n in seen or n not in defs:
            continue
        seen.add(n)
        body = defs[n]
        text.append(body)
        if d < depth:
            for callee in set(re.findall(r'\b([A-Za-z_]\w*)\s*(?:<[^<>;]*>)?\s*\(', body)):
                if callee in defs and callee not in seen:
                    todo.append((callee, d + 1))
    return '\n'.join(text), name in defs


def native_kind(path, fn):
    text, found = body_closure(path, fn)
    if not found:
        return 'native-ps2', {'found': False}
    marks = {
        'ps2math': len(PS2_MATH.findall(text)),
        'guestcall': len(GUEST_CALL.findall(text)),
        'ctxreg': len(CTX_REG.findall(text)),
        'guestmem': len(GUEST_MEM.findall(text)),
        'found': True,
    }
    if marks['ps2math'] or marks['guestcall']:
        return 'native-ps2', marks
    return 'native-host', marks


# ---- areas (decomp_status.py's, so the numbers line up with the dashboard) ----------------------------------------

def areas():
    from decomp_status import area_of
    return area_of


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--replaced', help='PS2X_REPLACED_DUMP output (default: docs/decomp_status.json natives)')
    ap.add_argument('--prof', nargs='*', default=[], help='prof129 console logs as err.txt[:windows]')
    ap.add_argument('--json', help='write the per-function classes here')
    ap.add_argument('--top', type=int, default=25)
    args = ap.parse_args()

    funcs = table_functions()
    size = sizes()
    stubs = stub_functions()
    lifted = lifted_addresses()
    typed, typed_bad = typed_functions()
    adapted6 = a6_adapted()
    nreg = native_registrations()
    d1 = d1_natives()
    if args.replaced:
        replaced = {int(l.split()[0], 16) for l in open(args.replaced) if l.strip()}
        source = args.replaced
    else:
        st = json.load(open(os.path.join(ROOT, 'docs/decomp_status.json')))
        replaced = {int(n['address'], 16) for n in st['native_functions']}
        source = 'docs/decomp_status.json (dump of %s, %s) + static registrars since' % (st['commit'], st['updated'])
        # natives registered since that dump (statically: a registrar whose address the dump does not hold)
        replaced |= {a for a in nreg if a in funcs and a not in replaced and a not in lifted}

    cls = {}
    why = {}
    wrapped = []
    for a in funcs:
        if a in stubs:
            cls[a] = 'runtime'
        elif a in replaced and a in nreg and (nreg[a][3] == 'macro' or a not in lifted):
            f, name, fn, how = nreg[a]
            k, marks = native_kind(os.path.join(OVR, f), fn)
            if a in d1 and d1[a][4]:
                k = 'typed'  # round D1: a full D1 body whose core meets the typed bar
                marks = dict(marks, d1=d1[a][3])
            elif a in d1:
                marks = dict(marks, d1=d1[a][3])
            cls[a] = k
            why[a] = dict(marks, file=f, fn=fn)
        elif a in replaced and a in lifted:
            cls[a] = 'typed' if a in typed else 'lifted'
        elif a in replaced:
            # held by a table-driven wrapper (join gate, census / heap hook) in front of the generated body
            cls[a] = 'recomp'
            wrapped.append(a)
        elif a in adapted6 and a in typed:
            cls[a] = 'typed'  # round A6: a never-lifted function whose slot holds its adapter over the typed function
        else:
            cls[a] = 'recomp'
    unresolved = [a for a in why if not why[a]['found']]

    order = ['recomp', 'lifted', 'native-ps2', 'native-host', 'runtime', 'typed']
    label = {'recomp': '(a) recompiled only', 'lifted': '(b) lifted (lift_d1)', 'native-ps2': '(c1) native, PS2-shaped body',
             'native-host': '(c2) native, host body behind guest adapter', 'runtime': '(r) runtime-implemented (kernel/SDK/libc)',
             'typed': '(d) real C++, no ctx (typed, rounds A1 / A2 / A3 / A4 / A5 / A6 / A7 / A8 / A9 / A10 / A11 / A12 / A13 / A14, D1)'}
    game = lambda a: a >= 0x1000000
    tot_n = len(funcs)
    tot_b = sum(size.get(a, 4) for a in funcs)
    print('source of the slot holders:', source)
    print('functions: %d (game modules >= 0x1000000: %d; boot ELF + KERNEL: %d); MIPS bytes %d' % (tot_n, sum(1 for a in funcs if game(a)), sum(1 for a in funcs if not game(a)), tot_b))
    print('native registrars whose body could not be read statically (counted c1): %d %s' % (
        len(unresolved), ' '.join('%s:%s' % (why[a]['file'], why[a]['fn']) for a in unresolved[:12])))
    print('slots held by a wrapper in front of the generated body (counted recomp, or lifted when lift tables hold it): %d' % len(wrapped))
    print('typed functions (docs/a1_typed.txt ... docs/a14_typed.txt): %d listed with a clean body, %d with a marker left (not counted), %d of them hold their slot' % (
        len(typed), typed_bad, sum(1 for a in funcs if cls[a] == 'typed')))
    if d1:
        print('round D1 natives (docs/d1_natives.txt): %d listed, %d full, %d full with a clean core (counted (d)), %d holding their slot' % (
            len(d1), sum(1 for v in d1.values() if v[3] == 'full'), sum(1 for v in d1.values() if v[4]), sum(1 for a in d1 if a in why)))
        bad_d1 = [a for a, v in d1.items() if v[3] == 'full' and not v[4]]
        if bad_d1:
            print('  full D1 cores with a marker left (not counted): %s' % ' '.join('0x%08x' % a for a in sorted(bad_d1)))
    print()
    print('%-46s %6s %7s %10s %7s' % ('class', 'funcs', '%', 'bytes', '%'))
    for k in order:
        n = sum(1 for a in funcs if cls[a] == k)
        b = sum(size.get(a, 4) for a in funcs if cls[a] == k)
        print('%-46s %6d %6.2f%% %10d %6.2f%%' % (label[k], n, 100.0 * n / tot_n, b, 100.0 * b / tot_b))

    area_of = areas()
    ar = {a: area_of(a, funcs[a], why.get(a, {}).get('file', '')) for a in funcs}

    # ---- time (console prof129) ----
    time = collections.Counter()
    tarea = collections.Counter()
    tfun = collections.Counter()
    tot_running = 0
    other = 0
    frames_ms = []
    if args.prof:
        import prof129 as P
        starts = sorted(funcs)
        for spec in args.prof:
            err, _, wins = spec.partition(':')
            _, allw, sel = P.parse(err, argparse.Namespace(windows=wins or None, t=None, frames=None))
            wall = 0.0
            frames = 0
            total = 0
            run = 0
            for w in sel:
                i = allw.index(w)
                wall += w['t'] - (allw[i - 1]['t'] if i else 0)
                frames += w['f1'] - w['f0']
                for pc, ra, n, s, z in w['e']:
                    total += n
                    if s & 15:
                        continue
                    run += n
                    j = bisect.bisect_right(starts, pc) - 1
                    if j >= 0 and pc < starts[j] + max(size.get(starts[j], 4), 4):
                        a = starts[j]
                        time[cls[a]] += n
                        tarea[ar[a]] += n
                        tfun[a] += n
                    else:
                        other += n
            tot_running += run
            frames_ms.append((err, wall * 1000.0 / frames if frames else 0, run / total if total else 0))
        print()
        print('console running samples (game thread): %d; outside the table (host-only / no pc): %d' % (tot_running, other))
        for err, ms, r in frames_ms:
            print('  %s: %.1f ms/frame wall, %.1f%% running' % (err, ms, 100 * r))
        print('%-46s %8s' % ('class', '% time'))
        for k in order:
            print('%-46s %7.2f%%' % (label[k], 100.0 * time[k] / tot_running))
        print('%-46s %7.2f%%' % ('(outside the table)', 100.0 * other / tot_running))

    print()
    print('areas: functions / MIPS bytes / not-yet-native bytes (recomp + lifted) / %% console time')
    rows = collections.defaultdict(lambda: collections.Counter())
    for a in funcs:
        r = rows[ar[a]]
        r['n'] += 1
        r['b'] += size.get(a, 4)
        r[cls[a]] += 1
        r['b_' + cls[a]] += size.get(a, 4)
    for name, r in sorted(rows.items(), key=lambda t: -t[1]['b']):
        t = 100.0 * tarea[name] / tot_running if tot_running else 0
        print('  %-26s %5d %8d  recomp %4d lifted %4d c1 %4d c2 %4d rt %4d typed %4d  todo-bytes %8d  time %5.2f%%' % (
            name, r['n'], r['b'], r['recomp'], r['lifted'], r['native-ps2'], r['native-host'], r['runtime'], r['typed'],
            r['b_recomp'] + r['b_lifted'], t))

    if tot_running:
        for k in ('recomp', 'lifted', 'native-ps2', 'native-host', 'typed'):
            print()
            print('top %s by console time:' % label[k])
            for a, n in [(a, n) for a, n in tfun.most_common() if cls[a] == k][:args.top]:
                print('  %5.2f%%  0x%08x %-48s %6d B %s' % (100.0 * n / tot_running, a, funcs[a][:48], size.get(a, 4), why.get(a, {}).get('file', '')))
        # concentration
        for k in ('recomp', 'lifted'):
            v = sorted((n for a, n in tfun.items() if cls[a] == k), reverse=True)
            s = sum(v)
            if not s:
                continue
            acc, marks = 0, {}
            for i, n in enumerate(v, 1):
                acc += n
                for p in (50, 80, 95):
                    if p not in marks and acc >= s * p / 100:
                        marks[p] = i
            print('%s: %d functions sampled; %s' % (label[k], len(v), ', '.join('%d%% of its time in %d functions' % (p, marks[p]) for p in sorted(marks))))
            never = sum(1 for a in funcs if cls[a] == k and a not in tfun)
            print('  %d of its functions never sampled riding' % never)

    # native-body markers summary
    print()
    c = collections.Counter()
    for a, w in why.items():
        if w.get('found'):
            c['found'] += 1
            for m in ('ps2math', 'guestcall', 'ctxreg', 'guestmem'):
                if w[m]:
                    c[m] += 1
    print('hand natives with a readable body: %d; with PS2 math macros %d, guest calls %d, ctx registers %d, guest RAM addressing %d' % (
        c['found'], c['ps2math'], c['guestcall'], c['ctxreg'], c['guestmem']))
    lines = collections.Counter()
    for f in lift_files():
        lines['lifted'] += sum(1 for _ in open(f, errors='replace'))
    for f in native_files():
        lines['native'] += sum(1 for _ in open(f, errors='replace'))
    rec = glob.glob(os.path.join(BUILD, 'runner/*_0x*.cpp'))
    print('C++ lines: lifted %d (%d files), hand / override %d (%d files), recompiled output %d files' % (
        lines['lifted'], len(lift_files()), lines['native'], len(native_files()), len(rec)))

    if args.json:
        out = [{'address': '0x%08x' % a, 'name': funcs[a], 'bytes': size.get(a, 4), 'class': cls[a], 'area': ar[a],
                'samples': tfun.get(a, 0), **({'native': why[a]} if a in why else {})} for a in sorted(funcs)]
        with open(args.json, 'w') as f:
            json.dump(out, f, indent=0)


if __name__ == '__main__':
    main()
