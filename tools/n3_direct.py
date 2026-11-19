#!/usr/bin/env python3
"""n3_direct.py [--overrides DIR] [--exclude FILE] [--undo] <lifted .cpp ...>: round N3, direct calls between lifted
functions (docs/native_n3.md).

Every PS2_CALL_GUEST_KNOWN(T, <recompiled>, ...) / PS2_TAIL_GUEST_KNOWN(T, <recompiled>) in the given lifted batches
whose target T is a lifted function (a SOTC_DECOMP_LIFT_FUNCTION* registration in any of the given files) becomes
PS2_CALL_GUEST_DIRECT(T, lift_d1::<name>, ...) / PS2_TAIL_GUEST_DIRECT(T, lift_d1::<name>): the call reaches the lifted
function without reading the function table while the runtime's g_ps2DirectCalls is set (ps2_runtime_macros.h), which
the runtime keeps only while every direct target's slot holds its lifted function, so the result is the same either way.
Each file gets the declarations of its targets in other batches and a registrar of its targets (ps2DirectTarget), both
between N3 marker comments (rerunning the tool rewrites them: idempotent; run it over ALL lifted batches at once, as
tools/lift_d1.py does after writing a batch: the targets come from every file's registrations).

Left out (they keep the table call): targets whose address appears in another override file (a hand-written native, a
hook or a switch may hold that slot: --overrides, default game/overrides), targets listed in --exclude (hex addresses,
one a line; '#' comments), targets named like a C library function (a declaration in namespace lift_d1 would hide it).
--undo: back to the KNOWN forms (the recompiled function's name from the registration), markers removed.
"""
import argparse
import os
import re
import sys

REG = re.compile(r'^SOTC_DECOMP_LIFT_FUNCTION(?:_D\d+)?\((0x[0-9A-Fa-f]+)u, (\w+), lift_d1::(\w+), (\w+)\);$', re.M)
BODY = re.compile(r'^void (\w+)\(uint8_t \*rdram, R5900Context \*ctx, PS2Runtime \*runtime\)$', re.M)
CALL = re.compile(r'PS2_CALL_GUEST_KNOWN\((0x[0-9A-Fa-f]+)u, (\w+), ')
TAIL = re.compile(r'PS2_TAIL_GUEST_KNOWN\((0x[0-9A-Fa-f]+)u, (\w+)\)')
DCALL = re.compile(r'PS2_CALL_GUEST_DIRECT\((0x[0-9A-Fa-f]+)u, lift_d1::(\w+), ')
DTAIL = re.compile(r'PS2_TAIL_GUEST_DIRECT\((0x[0-9A-Fa-f]+)u, lift_d1::(\w+)\)')
DECL_BEGIN = '// N3 direct-call declarations (tools/n3_direct.py)\n'
DECL_END = '// N3 direct-call declarations end\n'
REG_BEGIN = '// N3 direct-call targets (tools/n3_direct.py)\n'
REG_END = '// N3 direct-call targets end\n'
# Names a declaration in namespace lift_d1 would hide from the batch's own code (<cmath>, <cstring>, <cstdlib> ...).
LIBC = set('''abs fabs fabsf sqrt sqrtf sin sinf cos cosf tan tanf asin asinf acos acosf atan atanf atan2 atan2f exp expf
log logf log10 log10f pow powf floor floorf ceil ceilf fmod fmodf modf modff frexp ldexp round roundf trunc truncf
memcpy memmove memset memcmp memchr strlen strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strtol
strtoul strtod atoi atol atof malloc calloc realloc free qsort bsearch rand srand printf sprintf snprintf vsprintf
vsnprintf puts putchar abort exit isdigit isalpha isspace toupper tolower div labs fmin fmax fminf fmaxf hypot hypotf
copysign copysignf nan isnan isinf signbit'''.split())


def strip_block(text, begin, end):
    i = text.find(begin)
    if i < 0:
        return text
    j = text.index(end, i) + len(end)
    if text[j:j + 1] == '\n':
        j += 1
    return text[:i] + text[j:]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='+')
    ap.add_argument('--overrides', default=None, help='override dir scanned for other files naming an address')
    ap.add_argument('--exclude', default=None)
    ap.add_argument('--undo', action='store_true')
    args = ap.parse_args()
    files = sorted(set(args.files))
    texts = {f: open(f).read() for f in files}
    lifted = {}  # address -> (lift name, recompiled name)
    for t in texts.values():
        for m in REG.finditer(t):
            lifted[int(m.group(1), 16)] = (m.group(3), m.group(4))
    excluded = set()
    odir = args.overrides or os.path.join(os.path.dirname(os.path.abspath(files[0])))
    mine = {os.path.abspath(f) for f in files}
    hexre = re.compile(r'0x0*([0-9A-Fa-f]{5,8})\b')
    for name in sorted(os.listdir(odir)):
        p = os.path.join(odir, name)
        if os.path.abspath(p) in mine or not name.endswith(('.cpp', '.h', '.inc')):
            continue
        if re.match(r'decomp_lift(d\d+|alt)_\d+\.cpp$', name) or name.startswith('lean_'):
            continue
        for m in hexre.finditer(open(p, errors='replace').read()):
            a = int(m.group(1), 16)
            if a in lifted:
                excluded.add(a)
    if args.exclude:
        for line in open(args.exclude):
            line = line.split('#')[0].strip()
            if line:
                excluded.add(int(line, 16))
    for a, (n, _) in lifted.items():
        if n in LIBC:
            excluded.add(a)
    total_sites = 0
    total_targets = set()
    for f in files:
        t = texts[f]
        t = strip_block(strip_block(t, DECL_BEGIN, DECL_END), REG_BEGIN, REG_END)
        # back to the KNOWN forms first (idempotent; the exclusions may have changed)
        t = DCALL.sub(lambda m: f'PS2_CALL_GUEST_KNOWN({m.group(1)}u, {lifted[int(m.group(1), 16)][1]}, ', t)
        t = DTAIL.sub(lambda m: f'PS2_TAIL_GUEST_KNOWN({m.group(1)}u, {lifted[int(m.group(1), 16)][1]})', t)
        if args.undo:
            texts[f] = t
            continue
        used = {}

        def direct(m, tail):
            a = int(m.group(1), 16)
            if a not in lifted or a in excluded:
                return m.group(0)
            name = lifted[a][0]
            used[a] = name
            return f'PS2_TAIL_GUEST_DIRECT({m.group(1)}u, lift_d1::{name})' if tail else f'PS2_CALL_GUEST_DIRECT({m.group(1)}u, lift_d1::{name}, '

        t, n1 = CALL.subn(lambda m: direct(m, False), t)
        t, n2 = TAIL.subn(lambda m: direct(m, True), t)
        sites = len(DCALL.findall(t)) + len(DTAIL.findall(t))
        total_sites += sites
        total_targets.update(used)
        if used:
            defined = set(BODY.findall(t))
            ext = sorted({n for n in used.values() if n not in defined})
            if ext:
                decl = DECL_BEGIN + 'namespace lift_d1\n{\n' + ''.join(
                    f'    void {n}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for n in ext) + '}\n' + DECL_END
                k = t.index('\nnamespace lift_d1\n{\n    void ')
                t = t[:k + 1] + decl + t[k + 1:]
            reg = (REG_BEGIN + 'namespace\n{\n    const struct N3DirectTargets\n    {\n        N3DirectTargets()\n        {\n'
                   '            static const struct\n            {\n                uint32_t address;\n'
                   '                PS2Runtime::RecompiledFunction fn;\n            } kTargets[] = {\n' +
                   ''.join(f'                {{0x{a:08X}u, &lift_d1::{n}}},\n' for a, n in sorted(used.items())) +
                   '            };\n            for (const auto &e : kTargets)\n                ps2DirectTarget(e.address, e.fn);\n'
                   '        }\n    } n3DirectTargets;\n}\n' + REG_END)
            if not t.endswith('\n'):
                t += '\n'
            t += reg
        texts[f] = t
    for f in files:
        if open(f).read() != texts[f]:
            open(f, 'w').write(texts[f])
    print(f'n3_direct: {len(lifted)} lifted functions, {len(excluded)} left out, {total_sites} direct call sites to '
          f'{len(total_targets)} targets in {len(files)} files{" (undone)" if args.undo else ""}', file=sys.stderr)


if __name__ == '__main__':
    main()
