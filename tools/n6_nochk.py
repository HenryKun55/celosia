#!/usr/bin/env python3
"""n6_nochk.py [--list docs/n5_noloop.txt] [--undo] [--check] <generated .cpp ...>: round N6, the dispatch checkpoint off
the calls into functions that cannot loop nor switch threads (docs/native_n6.md).

Every JAL with a constant target in the given generated files (the lifted batches game/overrides/decomp_liftd*_NN.cpp,
the recompiler's runner files <name>_0x<addr>.cpp)

    PS2_CALL_GUEST_KNOWN(0x<T>u, <fn>, ...)            -> PS2_CALL_GUEST_NOCHK(0x<T>u, <fn>, ...)
    PS2_CALL_GUEST_DIRECT(0x<T>u, lift_d1::<fn>, ...)  -> PS2_CALL_GUEST_DIRECT_NOCHK(0x<T>u, lift_d1::<fn>, ...)

whose target T is in --list (tools/n5_blocking.py --noloop: NOBLOCK, no loop anywhere below, not a native / host slot).
The NOCHK forms (ps2_runtime_macros.h, PS2Runtime::callGuestKnownNoCheck) charge the dispatch checkpoint's cycles
without testing them while the switch n6nochk is on and n5sched is in effect and the slot holds the known function;
otherwise they are the plain forms. JALRs (a computed target) and tail jumps (no checkpoint anyway) are left.

Idempotent: every NOCHK form is first put back to its plain form, then the list applied (so a changed list is
followed). --undo: back to the plain forms only. --check: count, write nothing. tools/n3_direct.py keeps the NOCHK
suffix when it turns KNOWN into DIRECT (and back); tools/lift_d1.py runs n3_direct.py, then this tool.
"""
import argparse
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLAIN = re.compile(r'PS2_CALL_GUEST_(KNOWN|DIRECT)\((0x[0-9A-Fa-f]+)u, ')
NOCHK = re.compile(r'PS2_CALL_GUEST_(NOCHK|DIRECT_NOCHK)\(')


def load_list(path):
    out = set()
    for line in open(path):
        if line.startswith('#') or not line.strip():
            continue
        out.add(int(line.split()[0], 16))
    return out


def undo_text(t):
    return NOCHK.sub(lambda m: 'PS2_CALL_GUEST_KNOWN(' if m.group(1) == 'NOCHK' else 'PS2_CALL_GUEST_DIRECT(', t)


def apply_text(t, targets):
    """(new text, sites rewritten, targets used)"""
    t = undo_text(t)
    used = set()

    def rw(m):
        a = int(m.group(2), 16)
        if a not in targets:
            return m.group(0)
        used.add(a)
        return f'PS2_CALL_GUEST_{"NOCHK" if m.group(1) == "KNOWN" else "DIRECT_NOCHK"}({m.group(2)}u, '
    t, _ = PLAIN.subn(rw, t)
    return t, len(NOCHK.findall(t)), used


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='+')
    ap.add_argument('--list', default=os.path.join(REPO, 'docs/n5_noloop.txt'))
    ap.add_argument('--undo', action='store_true')
    ap.add_argument('--check', action='store_true')
    args = ap.parse_args()
    targets = set() if args.undo else load_list(args.list)
    sites, used, changed = 0, set(), 0
    for f in sorted(set(args.files)):
        old = open(f, errors='surrogateescape').read()
        if args.undo:
            new, n, u = undo_text(old), 0, set()
        else:
            new, n, u = apply_text(old, targets)
        sites += n
        used |= u
        if new != old:
            changed += 1
            if not args.check:
                with open(f, 'w', errors='surrogateescape') as fo:
                    fo.write(new)
    print(f'n6_nochk: {sites} NOCHK call sites to {len(used)} of {len(targets)} listed targets in {len(args.files)} files '
          f'({changed} {"would change" if args.check else "changed"}){" (undone)" if args.undo else ""}', file=sys.stderr)


if __name__ == '__main__':
    main()
