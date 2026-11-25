#!/bin/bash
# vita_hot_order.sh <elf> <profile> [<profile> ...]: regenerates game/overrides/vita_hot_order.ld (round M1; the procedure of
# rounds 66 / 121 / 137 in one command). <elf>: the ps2EntryRunner the profiles were taken with; <profile>: Vita3K profiles of it
# (tools/vita3k.sh PROF=<file>; scratch scripts: PROF=1 run.sh <scene> ...). Take them with the D rounds' switch on
# (SOTC_D1NATIVE=1) on the ride / lake / attract routes, plus one with the default switches, so both bodies of each D native
# are sampled. The D rounds' adapters / dispatchers / cores that no profile sampled go right after the sampled set
# (--natives: docs/d1_natives.txt, docs/d2_natives.txt and any later docs/d<N>_natives.txt).
# Then relink (only the link changes) and compare the layout: tools/hot_layout.py <new elf> <profile>:<elf> ... --order <ld>.
set -e
cd "$(dirname "$0")/.."
ELF=$1; shift
SPECS=()
for p in "$@"; do SPECS+=("$p:$ELF"); done
NAT=()
for f in docs/d*_natives.txt; do NAT+=(--natives "$f"); done
python3 tools/vita_hot_order.py game/overrides/vita_hot_order.ld "${SPECS[@]}" "${NAT[@]}" --cover 0.995 --size-elf "$ELF"
