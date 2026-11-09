#!/usr/bin/env python3
"""prof129.py err.txt --elf <elf> [options]: round 129's console sampler (SOTC_PROF129=1) as function-level shares of the
game thread, grouped by subsystem (round 113's budget areas).

  --elf ELF          the shipped ELF of the same build (build/agent-scratch/<round>/*.elf)
  --windows A-B      only windows A..B (the [prof129] w numbers; default all)
  --t A-B            only windows ending between A and B seconds after the sampler started
  --frames A-B       only windows whose frame range lies inside A..B
  --top N            functions listed (default 40)
  --group PATTERN    list the functions of the groups matching PATTERN (regex)
  --host PROF        also group a Vita3K host sampler profile (tools/vita3k.sh PROF=, busiest thread) for comparison
  --base PROF        the host profile's earlier snapshot (its window = PROF - BASE)
  --raw              keys by sampled pc (no function grouping): checks the dump itself

Shares: '% busy' = of the samples where the game thread ran guest or native code (not waiting at a marked site,
not without an EE thread); '% all' = of every sample. ms/frame = share of all x the windows' wall time / their frames.

Round 132: a native is named by the table entry of the guest address it was entered at, so a wrapper in front of a native
takes its whole time (decomp_tex64::wrap<12> = the tex64 join check in front of modelDisp's hook, sotc_native::modelDispHook;
the r130 console's 46% was the hook, not tex64). Builds from r132 on mark the render hand-over ('wait render': the game
thread waiting for the render thread at the frame's first modelDisp, syncFrame -> endFrame) and the hook ring ('wait
hookring'), and give the hook's running samples by phase (gameZone 0x2000 | phase).
"""
import argparse, bisect, re, subprocess, sys
from collections import Counter, defaultdict

NM = '/Users/flaviohenrique/vitasdk/bin/arm-vita-eabi-nm'
SITES = ['other', 'pace', 'event', 'deadline', 'blocked', 'vu1q', 'vu1sync', 'vif1', 'hookjoin', 'tex', 'render', 'hookring']
# Round 132: modelDispHook's phases (gameZone 0x2000 | phase)
HOOK_ZONES = {1: 'sync (syncFrame)', 2: 'setup (camera, plan)', 3: 'state (lane / modelDispState)', 4: 'decision (parts)', 5: 'job (projection, lights, palette, push)'}

# Native (host C++) names by namespace / function, first match wins.
NATIVE_GROUPS = [
    ('kernel/syscalls', r'ps2_syscalls|sceKernel|Syscall|syscall'),
    ('worker gates (guest worker jobs run or joined here)', r'sotc_worker::gate'),
    ('collision', r'decomp_collision|coll1\d\d|hot47j|decomp_finder|collcensus|camcoll|ground85|probe84|seg80|Collision'),
    ('camera', r'decomp_cam126|decomp_camera|[Cc]amera'),
    ('particles', r'hot47e|ptcl|particle|decomp_wind|hot48c|hot46j|isys48'),
    ('characters', r'quat|pose82|decomp_chara|decomp_char1|curve|motion|cloth|ctrl|gaze|bridle|skeleton|nodeinterp|charageo|hot44d|lifebar|gc13b8dc8|ikquat|charlod'),
    ('display', r'sotc_native|decomp_walk|decomp_light|decomp_dl\b|decomp_dl::|prefetch|modelclip|skinpal|sortlist|layoutclass|texreset|hot43g|decomp_leaves|decomp_tex|native_renderer|decomp_reloc|hookjobs|\bgl[A-Z]'),
    ('scripts/AI', r'decomp_ai|aiparam|decomp_isys\b|decomp_isys::|subthread|pointsort|hot46h|decomp_scr|decomp_obj96|decomp_group|ambient|decomp_hot4'),
    ('runtime/libc', r'mem(cpy|set|move|cmp)|sceClib|str(cmp|len|cpy|ncpy|chr)|malloc|free\b|heap120|sym120|EeScheduler|PS2Runtime|ps2_|sotc_worker|printf|fmt102|gctick|sotc_decomp|sotc_prof|finishGuestCall|callGuest|__aeabi|_veneer|lean_'),
]
# Recompiled guest functions by their guest name, first match wins.
GUEST_GROUPS = [
    ('kernel/syscalls', r'^(kernel|sce(Sif|Dma|Gs|Vif|Ipu|Cd|Mc|Pad|Kernel)|_?i?(Sleep|Wakeup|Wait|Signal|Poll|Refer|Create|Delete|Start|Exit|Change|Rotate|Suspend|Resume|Release|Cancel|Set|Get)\w*(Sema|Thread|EventFlag|Alarm|Intc|Dmac)|FlushCache|SifCallRpc|syscall|Sync(DCache|Data)|DI$|EI$)'),
    ('runtime/libc', r'^(mem(cpy|set|move|cmp|chr)|str\w+|_\w+_r$|malloc|free$|calloc|realloc|v?s?n?printf|atoi|atof|strtol|qsort|iosSPrintf|__\w+|_d2b|_b2d|_dtoa|_cleanup|_free|_malloc|sqrt|sin|cos|atan|pow|fmod|floor|ceil)'),
    ('collision', r'[Cc]oll|Clip|Finder|finder|[Ss]egment|HitCheck|Ground|Col[A-Z]'),
    ('camera', r'[Cc]am(era)?[A-Z_]|^cam'),
    ('particles', r'[Pp]tcl|[Pp]article|[Ee]ffect|[Ww]ind|[Ee]mitter'),
    ('characters', r'[Mm]otion|[Mm]ot[A-Z]|[Cc]hara|[Qq]uaternion|[Qq]uat|Geometry|[Cc]loth|IK|[Bb]one|[Pp]ose|^player|[Hh]orse|charroot|motroot|norootdyn|Wander|Node|[Aa]nime|[Ss]keleton|[Bb]ridle'),
    ('display', r'[Dd]isp|[Dd]raw|^gl[A-Z]|[Mm]odel|[Mm]esh|^tex|Tex[A-Z]|[Ss]prite|[Ll]ight|[Ll]ayout|Dl[A-Z]|iosDl|[Ff]ont|[Bb]loom|[Ff]og|[Ss]hadow|[Rr]ender|iosRot|iosPush|iosPop|Matrix'),
    ('scripts/AI', r'^(nico|AI|SCR|scr|exec|st_|Kite|lizard|demo|stagemanager|isys|act|doit|MVHIST|object|Obj)'),
]


def classify(name, native):
    for g, pat in (NATIVE_GROUPS if native else GUEST_GROUPS):
        if re.search(pat, name):
            return g
    if native:
        return 'other natives'
    return 'guest code (unnamed)' if re.search(r'_sub_[0-9A-Fa-f]+$|^sub_', name) else 'guest code (named, other)'


def load_elf(elf):
    out = subprocess.run([NM, '-n', '-C', '--defined-only', elf], capture_output=True, text=True).stdout
    A, N, gstart, gname = [], [], [], []
    anchor = None
    for l in out.splitlines():
        p = l.split(' ', 2)
        if len(p) < 3 or p[1] not in 'tTwW':
            continue
        a = int(p[0], 16) & ~1
        n = p[2]
        A.append(a); N.append(n)
        if anchor is None and re.match(r'prof129::.*anchor\(', n):
            anchor = a
        m = re.match(r'(.+?)_0x([0-9a-f]+)(\(|$)', n)
        if m:
            gstart.append(int(m.group(2), 16)); gname.append(m.group(1))
    order = sorted(range(len(gstart)), key=lambda i: gstart[i])
    gstart = [gstart[i] for i in order]; gname = [gname[i] for i in order]
    return A, N, gstart, gname, anchor


def host_name(A, N, addr):
    i = bisect.bisect_right(A, addr & ~1) - 1
    return N[i] if i >= 0 else '?'


def short(n):
    if 'introsort' in n:
        return n[:160]
    n = n.replace('(anonymous namespace)::', '').replace('{lambda', '<lambda')
    i = n.find('(', 1)
    return (n[:i] if i > 0 else n)[:160]


def rng(s):
    a, b = s.split('-')
    return float(a), float(b)


def parse(path, args):
    wins = []; cur = None; start = None
    for l in open(path, errors='replace'):
        i = l.find('[prof129] ')
        if i < 0:
            continue
        p = l[i + 10:].split()
        if not p:
            continue
        k = p[0]
        if k == 'start':
            start = l[i:].strip()
        elif k == 'w':
            d = dict(zip(p[2::2], p[3::2]))
            f0, f1 = d['f'].split('-')
            cur = {'w': int(d['w']) if 'w' in d else int(p[1]), 't': float(d['t']), 'f0': int(f0), 'f1': int(f1), 'n': int(d['n']),
                   'gap': int(d.get('gap', 0)), 'self': float(d.get('self', 0)), 'int': float(d.get('int', 0)), 'drop': int(d.get('drop', 0)),
                   'h': {}, 'e': []}
            cur['w'] = int(p[1])
        elif k == 'h' and cur is not None:
            for it in p[1:]:
                pc, _, off = it.partition(':')
                cur['h'][int(pc, 16)] = int(off, 16) if not off.startswith('-') else -int(off[1:], 16)
        elif k == 'e' and cur is not None:
            for it in p[1:]:
                f = it.split(':')
                if len(f) < 3:
                    continue
                cur['e'].append((int(f[0], 16), int(f[1], 16), int(f[2]), int(f[3]) if len(f) > 3 else 0, int(f[4], 16) if len(f) > 4 else 0))
        elif k == 'end' and cur is not None:
            wins.append(cur); cur = None
    sel = []
    for w in wins:
        if args.windows and not (rng(args.windows)[0] <= w['w'] <= rng(args.windows)[1]):
            continue
        if args.t and not (rng(args.t)[0] <= w['t'] <= rng(args.t)[1]):
            continue
        if args.frames and not (rng(args.frames)[0] <= w['f0'] and w['f1'] <= rng(args.frames)[1]):
            continue
        sel.append(w)
    return start, wins, sel


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('err'); ap.add_argument('--elf', required=True)
    ap.add_argument('--windows'); ap.add_argument('--t'); ap.add_argument('--frames')
    ap.add_argument('--top', type=int, default=40); ap.add_argument('--group')
    ap.add_argument('--host'); ap.add_argument('--base'); ap.add_argument('--raw', action='store_true')
    args = ap.parse_args()
    A, N, gstart, gname, anchor = load_elf(args.elf)
    if anchor is None:
        sys.exit('no prof129 anchor symbol in the ELF (a build without round 129?)')
    start, wins, sel = parse(args.err, args)
    if not sel:
        sys.exit(f'no [prof129] windows selected ({len(wins)} in the file)')

    def guest_of(pc):
        i = bisect.bisect_right(gstart, pc) - 1
        return (gstart[i], gname[i]) if i >= 0 else (0, '?')

    # Pass 1: each guest function's native (a sample at its entry whose table entry is host code).
    native_of = {}
    hostcache = {}

    def hname(off):
        if off not in hostcache:
            hostcache[off] = host_name(A, N, anchor + off) if off else ''
        return hostcache[off]
    for w in sel:
        for pc, off in w['h'].items():
            n = hname(off)
            if n and '_0x' not in n:
                native_of[pc] = n

    def func(pc):
        """(function key, native?) for a sampled pc."""
        if pc == 0:
            return None, False
        if pc in native_of:
            return short(native_of[pc]), True
        s, gn = guest_of(pc)
        if s in native_of:
            return short(native_of[s]), True
        return f'{gn} @{s:x}', False

    total = 0; busy = 0; wall = 0.0; frames = 0
    states = Counter(); fn = Counter(); fnnat = {}; grp = Counter(); gfn = defaultdict(Counter); zones = Counter(); hzones = Counter(); via = Counter()
    for w in sel:
        wall += w['t'] - (wins[wins.index(w) - 1]['t'] if wins.index(w) > 0 else 0.0)
        frames += w['f1'] - w['f0']
        for pc, ra, n, s, z in w['e']:
            total += n
            if z:
                if z & 0x2000:
                    if (s & 15) == 0:
                        hzones[HOOK_ZONES.get(z & 0xFF, f'{z:#x}')] += n
                else:
                    zid = z & 0xFFF
                    zones[f'syscall {zid - 0x1000 if zid >= 0x800 else zid:#x}'] += n
            off_core = s & 16
            base = s & 15
            if 1 <= base <= 13:
                states['wait ' + (SITES[base - 1] if base - 1 < len(SITES) else str(base - 1))] += n
                continue
            if base == 14:
                states['no EE thread (scheduler)' + (' off-core' if off_core else '')] += n
                continue
            states['running' + (' (off a core)' if off_core else '')] += n
            busy += n
            if args.raw:
                key, nat = f'{pc:08x}', False
            else:
                key, nat = func(pc)
                if key is None:
                    key, nat = '(pc 0: no guest pc yet)', False
            fn[key] += n; fnnat[key] = nat
            g = classify(key.split(' @')[0], nat)
            if g.startswith('guest code') and ra:
                ck, cn = func(ra)
                if ck:
                    cg = classify(ck.split(' @')[0], cn)
                    if not cg.startswith('guest code') and cg != 'other natives':
                        g = cg; via[cg] += n
            grp[g] += n; gfn[g][key] += n
    msf = (wall * 1000.0 / frames) if frames else 0.0
    nsamp = sum(w['n'] for w in sel)
    print(f'{start or "[prof129] (no start line)"}')
    print(f'windows {sel[0]["w"]}-{sel[-1]["w"]} ({len(sel)}): {wall:.0f} s, frames {sel[0]["f0"]}-{sel[-1]["f1"]} ({frames}, {msf:.1f} ms/frame), '
          f'samples {total} (interval {sum(w["int"] * w["n"] for w in sel) / max(nsamp, 1):.0f} us, gaps {sum(w["gap"] for w in sel)}, '
          f'dropped {sum(w["drop"] for w in sel)}), sampler cpu {sum(w["self"] for w in sel):.0f} ms = {100 * sum(w["self"] for w in sel) / max(wall * 1000, 1):.2f}% of one core')
    print('\nthe game thread by state             % all   ms/frame')
    for k, v in states.most_common():
        print(f'  {k:34s} {100 * v / total:6.2f} {v / total * msf:8.2f}')
    if zones:
        print('  (in a syscall, any state: ' + ', '.join(f'{k} {100 * v / total:.2f}%' for k, v in zones.most_common(6)) + ')')
    if hzones:
        print('  (modelDispHook running, by phase: ' + ', '.join(f'{k} {100 * v / total:.2f}% = {v / total * msf:.2f} ms' for k, v in hzones.most_common()) + ')')
    print('\nthe running samples by subsystem     % busy  % all   ms/frame')
    for k, v in sorted(grp.items(), key=lambda x: -x[1]):
        print(f'  {k:34s} {100 * v / busy:6.2f} {100 * v / total:6.2f} {v / total * msf:8.2f}' + (f'  (of it {100 * via[k] / busy:.2f} unnamed guest code by its caller)' if via[k] else ''))
    if args.group:
        for g in sorted(gfn):
            if re.search(args.group, g):
                print(f'\n{g}:')
                for k, v in gfn[g].most_common(args.top):
                    print(f'  {100 * v / busy:6.2f} {k}{" [native]" if fnnat[k] else ""}')
    print(f'\ntop functions (% busy, {busy} samples)')
    for k, v in fn.most_common(args.top):
        g = classify(k.split(' @')[0], fnnat[k])
        print(f'  {100 * v / busy:6.2f} {k[:90]:90s} {g}{" [native]" if fnnat[k] else ""}')
    if args.host:
        host_compare(args, A, N, grp, busy)


def host_compare(args, A, N, grp, busy):
    b = {}
    if args.base:
        for l in open(args.base):
            p = l.split()
            if len(p) == 4:
                b[(p[0], p[1])] = int(p[3])
    rows = []; th = Counter()
    for l in open(args.host):
        p = l.split()
        if len(p) != 4:
            continue
        w = int(p[3]) - b.get((p[0], p[1]), 0)
        if w > 0:
            rows.append((p[0], int(p[1], 16), w)); th[p[0]] += w
    t = th.most_common(1)[0][0]
    hg = Counter(); tot = 0
    for tt, pc, w in rows:
        if tt != t:
            continue
        n = host_name(A, N, pc)
        m = re.match(r'(.+?)_0x[0-9a-f]+(\(|$)', n)
        g = classify(m.group(1), False) if m else classify(n, True)
        hg[g] += w; tot += w
    print(f'\nsubsystems: prof129 (% busy) against the Vita3K host sampler (% of thread {t}, {tot} samples)')
    keys = set(k.replace(' (via caller)', '') for k in grp) | set(hg)
    merged = Counter()
    for k, v in grp.items():
        merged[k.replace(' (via caller)', '')] += v
    for k in sorted(keys, key=lambda k: -(merged[k] / busy + hg[k] / tot)):
        print(f'  {k:34s} {100 * merged[k] / busy:6.2f} {100 * hg[k] / tot:6.2f}')


if __name__ == '__main__':
    main()
