#!/usr/bin/env python3
"""Round 121: readies the Vita's PGO profiles for -fprofile-use (CMake PS2X_VITA_PGO=<dir>).

The VitaSDK's libgcov has no file I/O; the instrumented build (PS2X_VITA_PGO=gen) writes its .gcda files through
__gcov_info_to_gcda (sotc_native.cpp gcovDump, SOTC_GCOV_DUMP_FRAME=<n>), which leaves the object summary's runs and
sum_max at 0: GCC then takes every function as never run. This copies the .gcda files of one or more runs (each a
pulled ux0:data/celosia/pgo directory) to <out>, summing the arc counters of the same object over the runs, and
writes the object summary (runs = 1, sum_max = the largest arc counter of all files; GCC 15's __gcov_info_to_gcda
writes none). With --root <path>, that tree's path in the names (the overrides are outside the CMake source
directory: their objects are named by their absolute paths) becomes @ROOT@, which CMake (PS2X_VITA_PGO=<dir>)
replaces with the building tree's path.

Usage: tools/pgo_fix.py [--root <tree>] <out-dir> <run-dir> [<run-dir> ...]
  The files' names are the objects' paths below the build directory with '/' as '#' (-fprofile-prefix-path); they
  are kept. Runs whose stamp / checksum differ from the first one's for an object (another build) are refused.
"""
import os
import struct
import sys

MAGIC = 0x67636461
TAG_FUNCTION = 0x01000000
TAG_COUNTER_ARCS = 0x01a10000
TAG_SUMMARY = 0xa1000000


def parse(data):
    """-> (header words, [(tag, payload words or ('zeros', n))])"""
    words = struct.unpack_from(f'<{len(data) // 4}I', data)
    if words[0] != MAGIC:
        raise ValueError('not a gcda file')
    head = list(words[:4])
    recs = []
    i = 4
    while i + 1 < len(words):
        tag, length = words[i], words[i + 1]
        i += 2
        slen = struct.unpack('<i', struct.pack('<I', length))[0]
        if slen < 0:  # GCC 12+: a counter record of -length bytes of zeros, no payload
            recs.append((tag, ('zeros', -slen // 4)))
            continue
        n = length // 4
        recs.append((tag, list(words[i:i + n])))
        i += n
    return head, recs


def counters(payload):
    if isinstance(payload, tuple):
        return [0] * (payload[1] // 2)
    return [payload[k] | (payload[k + 1] << 32) for k in range(0, len(payload), 2)]


def encode(head, recs):
    out = list(head)
    for tag, payload in recs:
        if isinstance(payload, tuple):
            out += [tag, (-payload[1] * 4) & 0xffffffff]
        else:
            out += [tag, len(payload) * 4] + payload
    return struct.pack(f'<{len(out)}I', *out)


def main():
    args = sys.argv[1:]
    root = None
    if args[0] == '--root':
        root = os.path.abspath(args[1]).strip('/').replace('/', '#')
        args = args[2:]
    out, runs = args[0], args[1:]
    merged = {}
    for run in runs:
        for name in sorted(os.listdir(run)):
            if not name.endswith('.gcda'):
                continue
            head, recs = parse(open(os.path.join(run, name), 'rb').read())
            if name not in merged:
                merged[name] = (head, recs)
                continue
            h0, r0 = merged[name]
            if h0[1:4] != head[1:4] or len(r0) != len(recs):
                sys.exit(f'{run}/{name}: another build than the first run\'s')
            new = []
            for (t0, p0), (t1, p1) in zip(r0, recs):
                if t0 != t1:
                    sys.exit(f'{run}/{name}: records differ')
                if t0 == TAG_COUNTER_ARCS:
                    c = [a + b for a, b in zip(counters(p0), counters(p1))]
                    if any(c):
                        w = []
                        for v in c:
                            w += [v & 0xffffffff, (v >> 32) & 0xffffffff]
                        new.append((t0, w))
                    else:
                        new.append((t0, ('zeros', len(c) * 2)))
                else:
                    new.append((t0, p0))
            merged[name] = (h0, new)
    sum_max = 0
    for head, recs in merged.values():
        for tag, payload in recs:
            if tag == TAG_COUNTER_ARCS:
                sum_max = max([sum_max] + counters(payload))
    os.makedirs(out, exist_ok=True)
    nfun = 0
    for name, (head, recs) in merged.items():
        recs = [(t, p) for t, p in recs if t != TAG_SUMMARY]
        recs.insert(0, (TAG_SUMMARY, [1, min(sum_max, 0xffffffff)]))
        nfun += sum(1 for t, _ in recs if t == TAG_FUNCTION)
        dest = name.replace(root, '@ROOT@') if root else name
        open(os.path.join(out, dest), 'wb').write(encode(head, recs))
    print(f'{out}: {len(merged)} files, {nfun} functions, sum_max {sum_max}, from {len(runs)} run(s)')


if __name__ == '__main__':
    main()
