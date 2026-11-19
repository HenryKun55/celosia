#!/usr/bin/env python3
"""n1_basemem.py: round N1's base + offset guest memory form (docs/native_all_plan.md, docs/decomp.md "N1").

Rewrites generated code's guest loads / stores whose address is a register plus a MIPS immediate into the base form
of ps2_runtime_macros.h (READ32_B(base, off), WRITE32_B(base, off, val); 8 to 64 bits; 128-bit accesses are left):

    READ32(ADD32((uint32_t)v0, 32))          -> READ32_B((uint32_t)v0, 32)
    WRITE64(ADD32((uint32_t)sp, 4294966688), x) -> WRITE64_B((uint32_t)sp, -608, x)
    READ32(sp + 0x1Cu)                       -> READ32_B(sp, 28)
    READ32(ADD32(RC_U32(rc_29), 16))         -> READ32_B(RC_U32(rc_29), 16)   (ps2xRecomp's register cache)

Only a simple base (a name, (uint32_t)name, RC_U32(rc_N)) with an offset in -0x8000..0x7FFF is rewritten; anything
else is left as it was. With PS2X_N1_BASEMEM=0 the _B macros are the old macros on ADD32(base, off), so the same text
builds the old code (the A/B build).

Used by tools/lift_d1.py (its output) and standalone over files: n1_basemem.py [--check] file.cpp ...
(in place; --check only counts)."""
import re
import sys

_BASE = r'(\(uint32_t\)[A-Za-z_]\w*|RC_U32\(rc_\d+\)|[A-Za-z_]\w*)'
_K = r'(0x[0-9A-Fa-f]+u?|\d+u?|-\d+)'
_RD_ADD = re.compile(r'\bREAD(8|16|32|64)\(ADD32\(' + _BASE + r', ' + _K + r'\)\)')
_WR_ADD = re.compile(r'\bWRITE(8|16|32|64)\(ADD32\(' + _BASE + r', ' + _K + r'\), ')
_RD_PLUS = re.compile(r'\bREAD(8|16|32|64)\(([A-Za-z_]\w*) \+ ' + _K + r'\)')
_WR_PLUS = re.compile(r'\bWRITE(8|16|32|64)\(([A-Za-z_]\w*) \+ ' + _K + r', ')
_KEYWORDS = {'uint32_t', 'int32_t', 'uint64_t', 'int64_t', 'return', 'sizeof'}


def _off(k):
    """The offset as a signed 32-bit value, or None when it is not a MIPS immediate."""
    v = int(k.rstrip('u'), 0) if not k.startswith('-') else int(k)
    v &= 0xFFFFFFFF
    if v >= 0x80000000:
        v -= 0x100000000
    return v if -0x8000 <= v <= 0x7FFF else None


def rewrite(text, counts=None):
    c = counts if counts is not None else {}

    def sub(rx, fmt):
        def f(m):
            size, base, k = m.group(1), m.group(2), m.group(3)
            o = _off(k)
            if o is None or base in _KEYWORDS:
                c['left'] = c.get('left', 0) + 1
                return m.group(0)
            c['rewritten'] = c.get('rewritten', 0) + 1
            return fmt.format(size=size, base=base, off=o)
        return f

    text = _RD_ADD.sub(sub(_RD_ADD, 'READ{size}_B({base}, {off})'), text)
    text = _WR_ADD.sub(sub(_WR_ADD, 'WRITE{size}_B({base}, {off}, '), text)
    text = _RD_PLUS.sub(sub(_RD_PLUS, 'READ{size}_B({base}, {off})'), text)
    text = _WR_PLUS.sub(sub(_WR_PLUS, 'WRITE{size}_B({base}, {off}, '), text)
    return text


def main():
    check = '--check' in sys.argv
    files = [a for a in sys.argv[1:] if a != '--check']
    tot = {}
    for f in files:
        t = open(f).read()
        c = {}
        n = rewrite(t, c)
        for k, v in c.items():
            tot[k] = tot.get(k, 0) + v
        if not check and n != t:
            open(f, 'w').write(n)
    print(f"n1_basemem: {len(files)} files, {tot.get('rewritten', 0)} accesses rewritten, {tot.get('left', 0)} left")


if __name__ == '__main__':
    main()
