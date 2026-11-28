#!/usr/bin/env python3
"""r1_split.py: Round R1 (docs/r1_ps2_layers.md): the console game thread split into (a) the game's own logic, (b) PS2
emulation / PS2-SDK layers, (c) our runtime / native work, and the waits.

  tools/r1_split.py --err <console err.txt> --cls <decomp100_count.py --json output> [--windows 1-3] [--elf <shipped ELF>]
                    [--host <Vita3K prof.txt> --host-elf <that build's ELF> --host-tid <game thread id>] [--json out.json]

Console part (SOTC_PROF129 logs): every sample is put in one bucket: the wait state first (ps2_load sites: blocked, render,
hookjoin), then round 134's zones (the hook phases 0x2001-0x2005 and the 2D feed 0x2013 -> (c), the frame change 0x2020 ->
(c), SIF RPC 0x2030 and the EE dispatcher 0x2040 -> (b)), then the guest function the pc falls in (the runner's table and
functions.csv, through the --cls JSON's address / name / class / area / native file), classified by the address and name
rules below (KERNEL_ADDR, NAME_RULES), else by its code area (AREA). --elf only names the table's host function per pc.

Host part (optional, a Vita3K host sampler profile of a build of the same era, tools/vita3k.sh PROF=): the game thread's
host functions grouped into runtime layers the console sampler cannot see, because they run under whatever guest pc is
current: the guest call ABI (table slot tests, callGuest), the scheduler's checkpoints, IOP batches, host syscall stubs,
guest memory helpers, the renderer hooks a native calls (not a guest slot), the offload / prefetch plumbing, diagnostics.
Their Vita3K shares times the console's running ms are the estimate of what moves from (a) to (b) / (c) (an assumption:
the runtime's share of the game thread is the same on the console as in Vita3K).
"""
import argparse
import bisect
import collections
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import prof129 as P  # noqa: E402

B = {
    'A.chars': '(a) characters / motion / cloth / IK',
    'A.disp': '(a) display, game side: the walk, clip, lights, layout, texture reset, sprites',
    'A.coll': '(a) collision / physics',
    'A.ptcl': '(a) particles / wind (incl. the particle module run behind the worker gates)',
    'A.script': '(a) scripts / AI / other game code',
    'A.cam': '(a) camera',
    'A.engine': '(a) engine services: isys objects / groups / letters, heap alloc, engine math, frame loop',
    'B.kernel': '(b) EE kernel: thread switches (dispatcher), semaphores, message boxes, interrupt handlers, system threads t4 / t5 / t8 / t10, timers',
    'B.subthr': '(b) guest sub-thread switching (script coroutines on PS2 register frames)',
    'B.sound': '(b) SG2 sound library, EE side (sequencer ticks, SIF RPC to the IOP driver)',
    'B.gs': '(b) GS / DMA / display-list plumbing (gsReg, iosDl*, DMA chain send, frame-buffer copy, VRAM texture requests, SPR copies)',
    'B.load': '(b) CD / loader / XFF module relocation and run-time symbol resolution',
    'B.heap': '(b) PS2 heap compaction (the garbage pass: block moves through the scratchpad)',
    'B.pad': '(b) pad / vibration / memory-card SDK',
    'B.libc': '(b) PS2 libc / libm / softfloat as guest code',
    'C.hook': '(c) native renderer front end on the game thread (modelDispHook phases, 2D feed)',
    'C.sync': '(c) frame hand-over to the render thread (syncFrame / endFrame), running',
    'C.join': '(c) offload join wrappers running on the game thread',
    'W.blocked': 'wait (b): every guest thread blocked, the EE scheduler idles to a VBlank / deadline',
    'W.render': 'wait (c): the render thread hand-over',
    'W.hookjoin': 'wait (c): the hook jobs joined',
    'W.other': 'wait: other sites',
    'X.none': 'outside the table (no guest pc)',
}

# Boot ELF / KERNEL functions known by address (docs/decomp_b2.md / b3.md thread table, decomp.md's garbage pass, the
# recompiled sources' callees).
KERNEL_ADDR = {
    0x1b2ca0: 'B.kernel',  # t4: the thread system's callback thread
    0x1ad270: 'B.kernel',  # t5: the idle thread (polls the idle queue)
    0x1c80a0: 'B.kernel',  # t8: the VBlank / sound thread's loop
    0x1c38d8: 'B.kernel',  # t10: the GP thread (VBlank turn, VU0 push / pop, draw finish)
    0x1c8180: 'B.kernel',  # VBlank-start interrupt handler
    0x1c37f0: 'B.kernel',  # GS interrupt handler
    0x1c8078: 'B.kernel',  # the frame box's release (gpReleaseFrameHook)
    0x1ad958: 'B.heap',    # the garbage pass's copy through the scratchpad (sysnative::copyViaSpr)
    0x1ae270: 'B.heap',    # heapMove
    0x1c81b0: 'B.heap',    # the garbage pass's time check
    0x1af418: 'A.engine', 0x1addc0: 'A.engine', 0x1ae9b0: 'A.engine', 0x1ba690: 'A.engine',  # heap alloc / helpers
    0x1c7ae0: 'A.engine',  # t9: the game's frame loop
    0x1b4760: 'B.load',    # import resolution by string hash (decomp_sym120::resolveImports)
    0x1b4170: 'B.load', 0x1b44d8: 'B.load',  # relocation helpers (RelocateCode / LoaderSys*)
    0x1ac9c8: 'B.load',    # iosCdStRead's helper (sceCdRead / sceCdSync polling)
    0x115d170: 'B.load',   # sheetMgrResolve (sheet symbol resolution, decomp_sym120)
    0x1d0730: 'B.sound', 0x1db440: 'B.sound', 0x1ca5b8: 'B.sound', 0x1cdf38: 'B.sound',  # Sg2 / vab helpers
}
NAME_RULES = [
    ('B.subthr', r'SubThread'),
    ('B.kernel', r'^(ios(Send|Recv|Poll)Msg|iosWakeupThread|iosSleepThread|iosCallBackThreadMgr|iosGetThread|iosCreateThread|iosWaitIdleThread|'
                 r'iosStartThread|iosDeleteThread|iosChangeThreadPriority|isysObjWaitSema|isysObjSignalSema|DIntr|EIntr|iStart|iFinish|getIntrGpCount|'
                 r'iosGPFrameInit|iosGetTCount|iosGetSecondFromTCount|iosResetCpuRapCounter|iosGetCpuRapCount|iosAddIntcHandler|iosCreateMsgQueue|'
                 r'(i?)(Signal|Wait|Poll|Create|Delete|Refer)Sema|(i?)(Wakeup|Sleep|Start|Exit|Delete|Create|Rotate|Suspend|Resume|Release|Change)Thread\w*|'
                 r'GetThreadId|ReferThreadStatus|DelayThread|SetAlarm|ReleaseAlarm|FlushCache|SyncDCache|InvalidDCache|isce\w+|iosCreateIopMgr)'),
    ('B.sound', r'^(Sg2|sg2|snd|tickProcSound|vab_|Snd|iosSnd|IirParam)'),
    ('B.gs', r'^(gsReg|iosGs|iosDl[A-Z]|iosDma|iosChkGpDlTransExec|iosCheckDrawFinish|iosCopyFrameBuffer|iosMemToSpr|iosSprToMem|iosPushVu0|iosPopVu0|'
             r'getDrawScreen|getFrameBufferSize|sceGs|sceDma|sceVif|texTrans|texResetVRAMRequest)'),
    ('B.load', r'^(iosCd|sceCd|LoaderSys|Relocate(Code|ElfInfo|SelfSymbol)|ResolveRelocation|DecodeSection|iosIopRead|iosEE2IopRequest|isysMoveModuleObj|'
               r'SearchStrHashKey|sceSif|SifCallRpc|SifBindRpc|_sceSif)'),
    ('B.heap', r'^(iosMallocGarbage|garbage)'),
    ('B.pad', r'^(padSys|scePad|iosPad|sceDbc|sceVib|Shock_|iosVib|_?sceMc)'),
    ('B.libc', r'^(mem(cpy|set|move|cmp|chr)|str[a-z]+|floorf?|ceilf?|sqrtf?|sinf?|cosf?|tanf?|atan2?f?|acosf?|asinf?|powf?|expf?|logf?|fmodf?|'
               r'dp(add|sub|mul|div|cmp)|fp(add|sub|mul|div|cmp)|fptodp|dptofp|_d2b|_b2d|_dtoa|mktime|iosSPrintf|v?s?n?printf|qsort|atoi|atof|'
               r'strtol|rand|srand|isinf|isnan|raise|_\w+_r)$|^ps2___'),
]
AREA = {'Characters and motion': 'A.chars', 'Display and models': 'A.disp', 'Collision and physics': 'A.coll',
        'Particles and effects': 'A.ptcl', 'Script and events': 'A.script', 'Other game code': 'A.script', 'Camera': 'A.cam',
        'System and memory': 'A.engine', 'Math': 'A.engine', 'Sound': 'B.sound', 'Boot ELF, SDK and libc': 'A.engine'}
MODEL_DISP = 0x1097118  # sotc_native::modelDispHook holds modelDisp's slot (the VU1 path's replacement)


def classify(fun, a, z, s):
    base = s & 15
    if base:
        site = P.SITES[base - 1] if base - 1 < len(P.SITES) else str(base - 1)
        return {'blocked': 'W.blocked', 'render': 'W.render', 'hookjoin': 'W.hookjoin'}.get(site, 'W.other')
    if z == 0x2020:
        return 'C.sync'
    if z in (0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2013):
        return 'C.hook'
    if z == 0x2030:
        return 'B.sound'
    if z == 0x2040:
        return 'B.kernel'
    if a is None:
        return 'X.none'
    x = fun[a]
    name, cls, area = x['name'], x['class'], x['area']
    if a == MODEL_DISP:
        return 'C.hook'
    if a in KERNEL_ADDR:
        return KERNEL_ADDR[a]
    if cls == 'runtime':
        return 'B.libc' if re.search(NAME_RULES[-1][1], name) else 'B.kernel'
    for k, pat in NAME_RULES:
        if re.search(pat, name):
            return k
    if x.get('native', {}).get('file', '') == 'sotc_native.cpp':
        return 'C.hook'
    return AREA.get(area, 'A.script')


# ---- the Vita3K host sampler's groups (first match wins) ----
HOST_GROUPS = [
    ('B.abi', 'guest call ABI: typed slot tests (a6Is / a10Holds / aliasOf), callGuest, recompiled() lookups, switchOn',
     r'a6Is|a10Holds|aliasOf|sotc_decomp::callGuest|sotc_decomp::recompiled|sotc_decomp::switchOn|finishGuestCall|lookupFunction|::callNative|w3::callGuest|hasFunction'),
    ('B.sched', 'EE scheduler: checkpoints, deadlines, events (EeScheduler::run itself is the console\'s zone 0x2040)', r'EeScheduler::(?!run$)|eeCheckpoint|n5stat|b5trace'),
    ('B.sched_run', 'EE scheduler: EeScheduler::run (seen on the console as zone 0x2040)', r'EeScheduler::run$'),
    ('B.iop', 'IOP batches / SG2 driver host side', r'ps2x::iop|Sg2Driver|advanceIopEeCycles|catchUpIop|sound_n2::'),
    ('B.sys', 'host syscall / libc stubs reached from natives (decomp_sema, ps2_stubs)', r'^decomp_sema::|^ps2_stubs::|getThreadId139|pollSema139'),
    ('B.sys_guest', 'host syscall stubs behind a guest slot (b6sync: seen on the console)', r'^b6sync::'),
    ('B.mem', 'guest memory helpers / IO registers', r'^ps2_guest_mem::|^PS2Memory::'),
    ('C.hook_slot', 'renderer hooks that hold a guest slot (seen on the console)',
     r'sotc_native::(modelDispHook|gsRegHook|pushLayerHook|popLayerHook|glEndHook|meshDispHook|screenBlurHook|getCameraContextHook|s16InitHook|immFeed128|syncFrame)'),
    ('C.hook_hidden', 'renderer work a native calls (not a guest slot): particle / 2D / mesh submission, scroll, LOD, texture requests', r'^sotc_native::|^native_renderer::'),
    ('C.offload', 'offload / prefetch plumbing (decomp_prefetch notes, pf141, worker gates, lane joins)', r'^decomp_prefetch::|^pf141::|^sotc_worker::|^mdsjoin::|^decomp_tex64::wrap|^s5ptcl::join'),
    ('C.diag', 'diagnostics guards and profilers', r'wprof::|::logOn|^sotc_p2::|^prof129::|^sotc_prof|fmt102|printf'),
    ('X.veneer', 'veneers (long-branch stubs between far sections)', r'veneer'),
    ('X.mem', 'memcpy / memset / memcmp (spread over every caller)', r'^memcpy$|^memset$|^memmove$|^memcmp$'),
    ('X.div', 'integer division helpers', r'udivsi3|divsi3|udivmoddi4|__aeabi_'),
    ('X.libc', 'newlib / Vita imports', r'^str[a-z]+$|^sce|^__sce|pthread|_malloc_r|^_?free$|^sqrtf?$|^std::|^operator'),
]


def host_groups(prof, elf, tid):
    out = subprocess.run([P.NM, '-n', '-C', '--defined-only', elf], capture_output=True, text=True).stdout
    A, N = [], []
    for line in out.splitlines():
        p = line.split(' ', 2)
        if len(p) < 3 or p[1] not in 'tTwW':
            continue
        A.append(int(p[0], 16) & ~1)
        N.append(re.sub(r'\(.*', '', p[2]).replace('(anonymous namespace)::', ''))
    fn = collections.Counter()
    for line in open(prof):
        p = line.split()
        if len(p) != 4 or p[0] != tid:
            continue
        i = bisect.bisect_right(A, int(p[1], 16) & ~1) - 1
        fn[N[i] if i >= 0 else '?'] += int(p[3])
    s = sum(fn.values()) or 1
    g = collections.Counter()
    ex = collections.defaultdict(list)
    for k, v in fn.items():
        for key, _, pat in HOST_GROUPS:
            if re.search(pat, k):
                g[key] += v
                ex[key].append((v, k))
                break
        else:
            g['A.rest'] += v
    return {k: {'share': g[k] / s, 'top': [[n, round(c / s, 5)] for c, n in sorted(ex[k], reverse=True)[:8]]} for k in g}, s


# What the hidden host groups move: (from bucket, to bucket). Only groups the console cannot see are moved.
MOVES = {'B.abi': 'B.abi', 'B.sched': 'B.kernel', 'B.iop': 'B.sound', 'B.sys': 'B.kernel', 'B.mem': 'B.abi',
         'C.hook_hidden': 'C.hook', 'C.offload': 'C.offload', 'C.diag': 'C.diag'}
MOVED_LABEL = {'B.abi': '(b) guest call ABI and guest memory helpers inside natives (estimate from Vita3K)',
               'C.offload': '(c) offload / prefetch plumbing inside game natives (estimate from Vita3K)',
               'C.diag': '(c) diagnostics guards (estimate from Vita3K)'}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--err', required=True)
    ap.add_argument('--cls', required=True, help='tools/decomp100_count.py --json output (per-function class / area)')
    ap.add_argument('--windows', default='1-3')
    ap.add_argument('--elf', help='the shipped ELF of the console build (names the table host function per pc)')
    ap.add_argument('--host'), ap.add_argument('--host-elf'), ap.add_argument('--host-tid', default='258')
    ap.add_argument('--top', type=int, default=10)
    ap.add_argument('--json')
    args = ap.parse_args()

    fun = {int(x['address'], 16): x for x in json.load(open(args.cls))}
    starts = sorted(fun)
    _, allw, sel = P.parse(args.err, argparse.Namespace(windows=args.windows, t=None, frames=None))
    if not sel:
        sys.exit('no [prof129] windows selected')
    host = {}
    if args.elf:
        A, N, _, _, anchor = P.load_elf(args.elf)
        for w in sel:
            for pc, off in w['h'].items():
                host[pc] = P.short(P.host_name(A, N, anchor + off)) if off else ''
    cnt = collections.Counter()
    per = collections.defaultdict(collections.Counter)
    tot = 0
    wall = 0.0
    frames = 0
    for w in sel:
        i = allw.index(w)
        wall += w['t'] - (allw[i - 1]['t'] if i else 0)
        frames += w['f1'] - w['f0']
        for pc, ra, n, s, z in w['e']:
            tot += n
            j = bisect.bisect_right(starts, pc) - 1
            a = starts[j] if j >= 0 and pc < starts[j] + max(fun[starts[j]]['bytes'], 4) else None
            k = classify(fun, a, z, s)
            cnt[k] += n
            hn = host.get(pc, '')
            label = (fun[a]['name'] if a is not None else '-') + (' [' + hn[:40] + ']' if hn and '_0x' not in hn else '')
            if z and not (s & 15):
                label += ' {zone %#x}' % z
            per[k][label] += n
    msf = wall * 1000.0 / frames
    ms = {k: cnt[k] / tot * msf for k in cnt}
    running = sum(v for k, v in ms.items() if not k.startswith('W.'))
    run_name = os.path.basename(os.path.dirname(os.path.abspath(args.err)))
    print('%s windows %s: %d frames, %.1f s, %.2f ms/frame wall (running %.2f), %d samples' % (run_name, args.windows, frames, wall, msf, running, tot))
    sums = collections.Counter()
    for k in sorted(B, key=lambda k: (k[0], -cnt[k])):
        if not cnt[k]:
            continue
        sums[k[0]] += ms[k]
        print('%7.2f ms  %5.2f%%  %s' % (ms[k], 100 * cnt[k] / tot, B[k]))
        if args.top:
            print('           ' + '; '.join('%s %.2f' % (l[:64], v / tot * msf) for l, v in per[k].most_common(args.top)))
    print('console buckets:', ', '.join('%s %.2f ms' % (k, v) for k, v in sorted(sums.items())))

    out = {'run': run_name, 'windows': args.windows, 'frames': frames, 'wall_s': round(wall, 1), 'ms_per_frame': round(msf, 2),
           'running_ms': round(running, 2), 'samples': tot,
           'buckets': {k: {'label': B[k], 'ms': round(ms[k], 3), 'share': round(cnt[k] / tot, 5),
                           'top': [[l, round(v / tot * msf, 3)] for l, v in per[k].most_common(20)]} for k in B if cnt[k]}}
    if args.host:
        hg, hs = host_groups(args.host, args.host_elf, args.host_tid)
        print('\nVita3K host sampler, thread %s (%d samples): shares of the game thread' % (args.host_tid, hs))
        for k, v in sorted(hg.items(), key=lambda t: -t[1]['share']):
            lab = next((l for kk, l, _ in HOST_GROUPS if kk == k), 'game natives / lifted / typed / recompiled (the rest)')
            print('  %6.2f%%  %-12s %s  [%s]' % (100 * v['share'], k, lab, ', '.join('%s %.2f' % (n[:34], 100 * c) for n, c in v['top'][:4])))
        adj = dict(ms)
        moved = {}
        for g, to in MOVES.items():
            if g not in hg:
                continue
            m = hg[g]['share'] * running
            moved[g] = round(m, 3)
            adj[to] = adj.get(to, 0.0) + m
            # taken from (a) in proportion to the (a) buckets
        a_total = sum(v for k, v in ms.items() if k.startswith('A.'))
        take = sum(moved.values())
        for k in list(adj):
            if k.startswith('A.'):
                adj[k] -= take * ms[k] / a_total
        print('\nadjusted (hidden host layers moved out of (a), %.2f ms):' % take)
        asum = collections.Counter()
        for k in sorted(adj, key=lambda k: (k[0], -adj[k])):
            asum[k[0]] += adj[k]
            print('  %7.2f ms  %s' % (adj[k], B.get(k) or MOVED_LABEL.get(k, k)))
        print('adjusted:', ', '.join('%s %.2f ms' % (k, v) for k, v in sorted(asum.items())))
        out['host'] = {'profile': args.host, 'tid': args.host_tid, 'samples': hs,
                       'groups': {k: {'share': round(v['share'], 5), 'top': v['top']} for k, v in hg.items()}}
        out['moved_from_a_ms'] = moved
        out['adjusted_ms'] = {k: round(v, 3) for k, v in adj.items()}
    if args.json:
        with open(args.json, 'w') as f:
            json.dump(out, f, indent=1)


if __name__ == '__main__':
    main()
