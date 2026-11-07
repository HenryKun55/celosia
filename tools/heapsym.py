#!/usr/bin/env python3
# heapsym.py <elf> <err.txt> [last-n-reports]: names the call sites of the
# SOTC_HEAP_PROF [heap] lines (round mem99). Each report's "base" field is
# operator new's run-time address: nm's _Znwj gives the load delta (the
# console loads the code elsewhere than the ELF's addresses).
import os, re, subprocess, sys

elf, log = sys.argv[1], sys.argv[2]
lastn = int(sys.argv[3]) if len(sys.argv) > 3 else 1
tools = os.path.expanduser('~/vitasdk/bin/')
nm = subprocess.run([tools + 'arm-vita-eabi-nm', elf], capture_output=True, text=True).stdout
znwj = int(next(l.split()[0] for l in nm.splitlines() if l.endswith(' _Znwj')), 16)
reports, cur = [], None
for line in open(log, errors='replace'):
    if line.startswith('[heap] t='):
        m = re.search(r'base (0x[0-9a-f]+)', line)
        cur = {'head': line.strip(), 'delta': int(m.group(1), 16) - znwj, 'rows': []}
        reports.append(cur)
    elif line.startswith('[heap] ') and cur is not None and ' at ' in line:
        cur['rows'].append(line.strip())
addrs = set()
for r in reports[-lastn:]:
    for row in r['rows']:
        for a in row.split(' at ')[-1].split():
            v = int(a, 16)
            if v > 1:
                addrs.add(v - r['delta'] - 1)
addrs = sorted(addrs)
names = {}
if addrs:
    out = subprocess.run([tools + 'arm-vita-eabi-addr2line', '-f', '-C', '-e', elf] + ['0x%x' % a for a in addrs],
                         capture_output=True, text=True).stdout.splitlines()
    for i, a in enumerate(addrs):
        names[a] = '%s (%s)' % (out[2 * i][:120], os.path.basename(out[2 * i + 1]))
for r in reports[-lastn:]:
    print(r['head'])
    for row in r['rows']:
        head, _, pcs = row.partition(' at ')
        fns = [names.get(int(a, 16) - r['delta'] - 1, '?') for a in pcs.split() if int(a, 16) > 1]
        print(head[7:], '|', ' <- '.join(f[:90] for f in fns))
