#!/usr/bin/env python3
"""n5_blocking.py: round N5's set of game functions that can block or switch EE threads (docs/native_n5.md).

N4 (docs/native_n4.md, branch commit 4af1bf0) needs to know which guest calls can unwind through the context. With the
dispatch checkpoint off the call path (and S17's nesting on the game thread), a call unwinds only when its callee can
reach an explicit thread switch: a blocking or rescheduling kernel call, a host stub / native that can wait, or code
the analysis cannot see (an indirect call / jump). This tool computes that set statically, for every function of the
recompiled function table, from the MIPS (tools/lift_d1.py's Lifter: the same CFG and call sites the lifter uses) plus
the hand-written natives' C++ (game/overrides):

  seeds (the function itself may switch threads):
    syscall   a `syscall` whose number (the `addiu/ori v1, zero, N` before it) is not in NONBLOCK below
              (WaitSema, SleepThread, SignalSema / WakeupThread / StartThread / ChangeThreadPriority /
              RotateThreadReadyQueue ... can switch to a higher-priority thread at once); an unknown number blocks
    native    a slot that holds a hand-written native (SOTC_DECOMP_FUNCTION & co.) whose C++ body names a waiting
              primitive (callMaySleep, runToReturn, WaitSema, SleepThread, waitSemaphore, isysSleep, Wakeup ...),
              a coroutine / resume entry (SOTC_DECOMP_COROUTINE / RESUME / UNCHECKED), or calls guest code at a
              computed address
    stub      a slot the runtime / the recompiler's config fills with host code this tool cannot read (ps2_stubs,
              syscall wrappers by name, hooks): conservative
    jalr / ijump / notparsed   an indirect call or jump, or a function the lifter refuses: conservative
  transitive: a function that calls (jal / tail jump) a function of the set is in the set. A native's guest callees
  are the function starts its body names as hex constants.

Output (default docs/n5_blocking.txt): one line per table function
  <addr> <name> BLOCK|NOBLOCK <reason> loops=<0|1> [via=<callee that brings the block>]
reason: the seed kind for a seed, `calls` for a transitive member, `-` for NOBLOCK. loops=1: the function has a loop
back edge (a checkpoint: it can still be preempted there on a thread other than the game thread, or when S17 does not
nest). The header counts the set and, with --profile, its share of the riding profile.

Usage: n5_blocking.py [--out docs/n5_blocking.txt] [--profile syms.txt] [--jobs 2]
"""
import argparse
import os
import re
import sys
from multiprocessing import Pool

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lift_d1 as L  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# EE kernel syscalls that never block nor make another thread run at once (the i* forms, queries, setup, cache,
# handlers' registration, SIF / GS register access). Every other number may switch threads.
NONBLOCK = {
    0x01, 0x02, 0x04, 0x05, 0x06, 0x07,                     # ResetEE, SetGsCrt, Exit-family host side (counted below), ...
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,         # Add/RemoveIntcHandler, Add/RemoveDmacHandler, _Enable/_Disable Intc/Dmac
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,         # SetAlarm, ReleaseAlarm, iSetAlarm, iReleaseAlarm, ...
    0x20, 0x21,                                             # CreateThread, DeleteThread
    0x2F, 0x30, 0x31,                                       # GetThreadId, ReferThreadStatus, iReferThreadStatus
    0x34, 0x35, 0x36, 0x38, 0x3A,                           # iWakeupThread, CancelWakeupThread, iCancelWakeupThread, iSuspendThread, iResumeThread
    0x3C, 0x3D, 0x3E,                                       # SetupThread, SetupHeap, EndOfHeap
    0x40, 0x43, 0x45, 0x46, 0x47, 0x48,                     # CreateSema, iSignalSema, PollSema, iPollSema, ReferSemaStatus, iReferSemaStatus
    0x64, 0x68,                                             # FlushCache, iFlushCache
    0x70, 0x71, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,   # GsGetIMR, GsPutIMR, SetVSyncFlag, SetSyscall, SifDmaStat, SifSetDma, SifSetDChain, SifSetReg, SifGetReg
    0x7C, 0x7D, 0x7E, 0x7F,                                 # Deci2Call, PSMode, MachineType, GetMemorySize
}
# 0x04 Exit / 0x05..0x07 (Exit / LoadExecPS2 / ExecPS2) end the program: harmless for the call protocol (never return).

WAIT_TOKENS = re.compile(r'\b(callMaySleep|runToReturn|WaitSema|waitSemaphore|SleepThread|sleepCurrent|isysSleep\w*|'
                         r'WakeupThread|SignalSema|signalSemaphore|RotateThreadReadyQueue|ChangeThreadPriority|'
                         r'StartThread|blockCurrentOrReturn|leaveThread|checkTransfer|EeDispatcherTransfer|'
                         r'WaitEventFlag|waitEventFlag|waitVSync|iosWait\w*|sceSifCallRpc|syscall)\b')
GUEST_CALL = re.compile(r'\b(callGuest\w*|PS2_CALL_GUEST\w*|PS2_JUMP_GUEST\w*|PS2_TAIL_GUEST\w*|guestFunctionAt|'
                        r'lookupFunction|jumpGuest\w*|finishGuestCall)\b')
REG = re.compile(r'SOTC_DECOMP_(FUNCTION|AB_FUNCTION|FLOAT_FUNCTION|COROUTINE|RESUME|UNCHECKED|LIFT_FUNCTION\w*)\(\s*'
                 r'0x([0-9A-Fa-f]+)u?\s*,\s*(\w+)\s*,\s*(?:(\w+)\s*,\s*)?([\w:]+)')


def facts(a):
    try:
        lf = L.Lifter(a, {})
        lf.build()
        nodes = lf.node_effects()
    except Exception as e:
        return a, None, f'{type(e).__name__}: {str(e)[:60]}'
    calls, why, sysnums = [], set(), set()
    W = lf.g.words
    for x in lf.order:
        k = nodes[x]['kind']
        if k[0] == 'jalr':
            why.add('jalr')
        if k[0] == 'jr' and not k[2]:
            why.add('ijump')
        if k[0] in ('jal', 'tail'):
            calls.append(k[1])
    for x, w in W.items():
        if (w & 0xFC00003F) == 0x0000000C:  # syscall
            num = None
            for back in range(4, 24, 4):
                p = W.get(x - back)
                if p is None:
                    break
                if (p >> 16) in (0x2403, 0x3403):  # addiu / ori v1, zero, imm
                    num = p & 0xFFFF
                    if (p >> 16) == 0x2403 and num & 0x8000:
                        num -= 0x10000
                    break
            sysnums.add(num)
    loops = any(s <= x for x in lf.order for s in lf.succ[x])
    return a, {'name': lf.g.name, 'calls': calls, 'why': why, 'sys': sysnums, 'loops': loops}, None


def body_of(text, fname):
    """The C++ body of the function named fname (its last :: component) in text, by brace matching."""
    short = fname.split('::')[-1]
    for m in re.finditer(r'(?m)^[^\n;{}]*\b' + re.escape(short) + r'\s*\([^;{]*\)\s*(?:noexcept\s*)?\{', text):
        i, depth = m.end() - 1, 0
        for j in range(i, len(text)):
            c = text[j]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    return text[i:j + 1]
    return None


def natives_info(funcs):
    """address -> (kind, guest callees, seed reason or None) for the hand-written natives; the set of lifted slots."""
    od = os.path.join(REPO, 'game/overrides')
    texts = {}
    for f in sorted(os.listdir(od)):
        if f.endswith(('.cpp', '.h', '.inc')):
            texts[f] = open(os.path.join(od, f), errors='replace').read()
    alltext = '\n'.join(t for f, t in texts.items() if not f.startswith('decomp_lift'))
    info, lifted = {}, set()
    for f, t in texts.items():
        for m in REG.finditer(t):
            kind, addr, fn = m.group(1), int(m.group(2), 16), m.group(5)
            if kind.startswith('LIFT'):
                lifted.add(addr)
                continue
            if addr not in funcs:
                continue
            if kind in ('COROUTINE', 'RESUME', 'UNCHECKED'):
                info[addr] = (kind, [], 'native:' + kind.lower())
                continue
            body = body_of(t, fn) or body_of(alltext, fn)
            if body is None:
                info.setdefault(addr, (kind, [], 'native:unread'))
                continue
            callees = [int(h, 16) for h in re.findall(r'0x([0-9A-Fa-f]{5,8})', body) if int(h, 16) in funcs and int(h, 16) != addr]
            seed = None
            wm = WAIT_TOKENS.search(body)
            if wm:
                seed = 'native:' + wm.group(1)
            elif GUEST_CALL.search(body) and not callees:
                seed = 'native:computed-call'
            info[addr] = (kind, callees, seed)
    return info, lifted


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default=os.path.join(REPO, 'docs/n5_blocking.txt'))
    ap.add_argument('--profile', default=None)
    ap.add_argument('--jobs', type=int, default=2)
    args = ap.parse_args()
    tab = L.table()
    funcs = sorted(a for a, c in tab.items() if re.search(r'_0x([0-9a-f]+)$', c) and int(re.search(r'_0x([0-9a-f]+)$', c).group(1), 16) == a)
    fset = set(funcs)
    with Pool(args.jobs) as p:
        res = p.map(facts, funcs, chunksize=32)
    F = {a: f for a, f, e in res if f is not None}
    bad = {a: e for a, f, e in res if f is None}
    nat, lifted = natives_info(fset)
    slots = L.native_addresses(fset) - lifted  # natives, hooks, stubs (the lifted copies are the same code)
    seed, callees = {}, {}
    for a in funcs:
        if a in nat:
            kind, cs, s = nat[a]
            callees[a] = cs
            if s:
                seed[a] = s
            continue
        if a in slots:
            seed[a] = 'stub'
            callees[a] = []
            continue
        f = F.get(a)
        if f is None:
            seed[a] = 'notparsed'
            callees[a] = []
            continue
        callees[a] = f['calls']
        blk = sorted(n for n in f['sys'] if n is not None and n >= 0 and n not in NONBLOCK)  # negative: the i* forms
        if any(n is None for n in f['sys']):
            seed[a] = 'syscall:?'
        elif blk:
            seed[a] = 'syscall:' + '/'.join(f'{n:#x}' for n in blk)
        elif f['why'] & {'jalr', 'ijump'}:
            seed[a] = sorted(f['why'] & {'jalr', 'ijump'})[0]
    # transitive closure over the callers
    callers = {}
    for a, cs in callees.items():
        for c in cs:
            callers.setdefault(c, set()).add(a)
    blocked = dict(seed)
    via = {}
    work = list(seed)
    while work:
        c = work.pop()
        for a in callers.get(c, ()):
            if a not in blocked:
                blocked[a] = 'calls'
                via[a] = c
                work.append(a)
    # unknown callees (not a table function: a jal into the middle of something / data) count as blocking
    for a, cs in callees.items():
        if a not in blocked and any(c not in fset for c in cs):
            blocked[a] = 'unknown-callee'
    changed = True
    while changed:
        changed = False
        for a, cs in callees.items():
            if a not in blocked:
                for c in cs:
                    if c in blocked:
                        blocked[a], via[a], changed = 'calls', c, True
                        break
    prof = {}
    if args.profile:
        for line in open(args.profile, errors='replace'):
            m = re.match(r'\s*([\d.]+)%\s+(.*)', line)
            if m:
                prof[m.group(2).strip()] = prof.get(m.group(2).strip(), 0.0) + float(m.group(1))
    names = {a: (F[a]['name'] if a in F else tab[a]) for a in funcs}

    def share(a):
        n = names[a]
        return sum(v for k, v in prof.items() if k.startswith(f'lift_d1::{n}(') or k.startswith(f'{tab[a]}(') or k.startswith(f'{n}('))
    kinds = {}
    for a, r in blocked.items():
        k = r.split(':')[0]
        kinds[k] = kinds.get(k, 0) + 1
    wb = sum(share(a) for a in blocked) if prof else 0.0
    wn = sum(share(a) for a in funcs if a not in blocked) if prof else 0.0
    nob_loopfree = sum(1 for a in funcs if a not in blocked and not (F.get(a) or {}).get('loops', True))
    with open(args.out, 'w') as fo:
        fo.write('# n5_blocking.txt: game functions that can block or switch EE threads (tools/n5_blocking.py; docs/native_n5.md)\n')
        fo.write(f'# {len(funcs)} table functions, {len(F)} parsed, {len(bad)} not parsed, {len(nat)} hand-written natives read, '
                 f'{len(slots - set(nat))} other host slots (stubs / hooks)\n')
        fo.write(f'# BLOCK {len(blocked)} (seeds {len(seed)}; by reason {dict(sorted(kinds.items()))}), NOBLOCK {len(funcs) - len(blocked)} '
                 f'(of them without a loop: {nob_loopfree})\n')
        if prof:
            fo.write(f'# profile share (% of the game thread): BLOCK {wb:.2f}, NOBLOCK {wn:.2f}\n')
        for a in funcs:
            f = F.get(a)
            lp = int(f['loops']) if f else 1
            if a in blocked:
                fo.write(f'{a:08x} {names[a]} BLOCK {blocked[a]} loops={lp}' + (f' via={via[a]:08x}' if a in via else '') + '\n')
            else:
                fo.write(f'{a:08x} {names[a]} NOBLOCK - loops={lp}\n')
    print(open(args.out).read().split('\n')[1:4])


if __name__ == '__main__':
    main()
