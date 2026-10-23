"""Combine the boot ELF and loaded XFF modules into one ELF for PS2Recomp,
and emit the matching function map CSV + symbol list.

Usage: build_elf.py out_dir boot_elf boot_syms XFF@hdr@ram_dump [...]

hdr is the module header address, or "auto" to locate it in the dump.

Each module is captured from a RAM dump taken when the game jumps to its entry.
"""
import bisect, pathlib, re, sys, struct
from elfmini import Elf
from modimage import LoadedModule, find_header
import funcmap_elf

SHF_WRITE, SHF_ALLOC, SHF_EXEC = 1, 2, 4


def write_elf(path, entry, sections):
    """sections: list of (name, addr, size, data|None, flags)."""
    shstr = b"\0" + b"".join(n.encode() + b"\0" for n, *_ in sections) + b".shstrtab\0"
    progs = [s for s in sections if s[3] is not None]
    ehsize, phsize, shsize = 52, 32, 40
    off = ehsize + phsize * len(progs)
    blobs, phdrs, shdrs = [], [], [(0,) * 10]
    name_off = 1
    for name, addr, size, data, flags in sections:
        if data is not None:
            off = (off + 15) & ~15
            pflags = 4 | (1 if flags & SHF_EXEC else 0) | (2 if flags & SHF_WRITE else 0)
            phdrs.append((1, off, addr, addr, len(data), len(data), pflags, 16))
            blobs.append((off, data))
            shdrs.append((name_off, 1, flags, addr, off, size, 0, 0, 16, 0))
            off += len(data)
        else:
            shdrs.append((name_off, 8, flags, addr, off, size, 0, 0, 16, 0))
        name_off += len(name) + 1
    shstr_off = off
    off += len(shstr)
    shdrs.append((name_off, 3, 0, 0, shstr_off, len(shstr), 0, 0, 1, 0))
    shoff = (off + 3) & ~3
    out = bytearray(shoff + shsize * len(shdrs))
    out[0:16] = b"\x7fELF\x01\x01\x01" + bytes(9)
    struct.pack_into("<HHIIIIIHHHHHH", out, 16, 2, 8, 1, entry, ehsize, shoff, 0x20924001,
                     ehsize, phsize, len(phdrs), shsize, len(shdrs), len(shdrs) - 1)
    for i, p in enumerate(phdrs):
        struct.pack_into("<8I", out, ehsize + i * phsize, *p)
    for o, d in blobs:
        out[o:o + len(d)] = d
    out[shstr_off:shstr_off + len(shstr)] = shstr
    for i, s in enumerate(shdrs):
        struct.pack_into("<10I", out, shoff + i * shsize, *s)
    open(path, "wb").write(out)


def load_pinned(path):
    """Addresses (hex, one a line, # comments) kept as function starts, and
    the ones marked !hex: registered but left merged on purpose (no warning)."""
    out, quiet = set(), set()
    if path.exists():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line.startswith("!"):
                quiet.add(int(line[1:], 16))
            elif line:
                out.add(int(line, 16))
    return out, quiet


def registered_addresses(root):
    """Addresses the overrides register a function at (SOTC_DECOMP_*FUNCTION,
    registerFunction): each should be a function start, else calls through
    the function table reach it but gotos inside its owner do not."""
    pat = re.compile(r"\b(?:SOTC_DECOMP_\w*FUNCTION|registerFunction)\s*\(\s*0x([0-9A-Fa-f]{6,8})u?\b")
    out = {}
    for src in sorted((root / "game/overrides").glob("*.cpp")):
        for h in pat.findall(src.read_text(errors="ignore")):
            out[int(h, 16)] = src.name
    return out


def main(out_dir, boot_path, boot_syms, mods):
    boot = Elf(boot_path)
    sections = []
    for s in boot.sections:
        if s["flags"] & SHF_ALLOC and s["size"]:
            data = None if s["type"] == 8 else boot.data[s["offset"]:s["offset"] + s["size"]]
            sections.append((s["name"], s["addr"], s["size"], data, s["flags"]))

    # Addresses the hand-written overrides hook or replace stay function
    # starts: a goto inside a merged function would go past the hook. They
    # come from a fixed list (game/pinned_starts.txt), not from the override
    # sources: every hex literal there (comments, switch cases on jump-table
    # targets, profiler tables) used to pin, so editing an override changed
    # the split (2026-10-23: curveEval's cases became functions and the
    # attract demo hung). Changing the list changes the split on purpose.
    root = pathlib.Path(__file__).resolve().parent.parent
    pinned, quiet = load_pinned(root / "game/pinned_starts.txt")
    merged = []

    # boot ELF function map from heuristics
    boot_csv = f"{out_dir}/boot_functions.csv"
    merged += funcmap_elf.main(boot_path, boot_syms, boot_csv, merge=True, pinned=pinned)
    rows = [l.strip().split(",") for l in open(boot_csv).readlines()[1:]]
    funcs = [(int(r[1], 16), int(r[2], 16), r[0]) for r in rows]
    syms = [l.split() for l in open(boot_syms)]
    dynamic = set()

    for spec in mods:
        path, hdr, ram_path = spec.split("@")
        ram = open(ram_path, "rb").read()
        m = LoadedModule(path, ram, find_header(path, ram) if hdr == "auto" else int(hdr, 0))
        tag = m.name.split(".")[0]
        for s in m.sections:
            flags = SHF_ALLOC | (SHF_EXEC if s.exec else SHF_WRITE)
            sections.append((f"{s.name}.{tag}", s.addr, s.size, s.data, flags))
        t = m.text()
        fmap = m.functions()
        # unnamed (static) functions: jal targets, code pointers, prologues
        words = {t.addr + o: struct.unpack_from("<I", t.data, o)[0] for o in range(0, t.size, 4)}
        in_text = lambda a, t=t: t.addr <= a < t.addr + t.size
        blobs = [s.data for s in m.sections if s.data is not None and not s.exec]
        # The section's first word starts a function even when nothing names
        # or calls it (MANAGER's first, a debug routine, was left out).
        if t.addr not in fmap:
            fmap[t.addr] = (0, f"{tag.lower()}_sub_{t.addr:08X}")
        named = set(fmap)
        for tgt in funcmap_elf.find_starts(words, in_text, blobs):
            if tgt not in fmap:
                fmap[tgt] = (0, f"{tag.lower()}_sub_{tgt:08X}")
        # Pieces of one function the heuristics split (switch cases, aligned
        # labels): merged back, still callable through entry points.
        # (A sized symbol's extent is authoritative: an override address
        # inside one, such as the SOTC_CENSUS counters in modelDisp's body or
        # a range bound, does not split it.)
        sizes = {a: sz for a, (sz, n) in fmap.items() if a in named}
        extents = sorted((a, a + sz) for a, sz in sizes.items() if sz)
        def inside_named(p):
            i = bisect.bisect_right(extents, (p, 0xFFFFFFFF)) - 1
            return i >= 0 and extents[i][0] < p < extents[i][1]
        keep = {p for p in pinned & set(fmap) if not inside_named(p)}
        kept, dropped = funcmap_elf.merge_split_starts(words, fmap, named | keep, sizes)
        for a in dropped:
            del fmap[a]
        merged += dropped
        starts = sorted(fmap)
        for i, a in enumerate(starts):
            nxt = starts[i + 1] if i + 1 < len(starts) else t.addr + t.size
            funcs.append((a, nxt, fmap[a][1]))
        for a, (sz, n) in fmap.items():
            syms.append([f"{a:08x}", n])
        for a, n in m.data_symbols().items():
            syms.append([f"{a:08x}", n])
        print(f"{m.name}: header {m.base:#x}, {len(fmap)} functions ({len(dropped)} false splits merged), sections:",
              ", ".join(f"{s.name}@{s.addr:#x}+{s.size:#x}" for s in m.sections))

        # import relocation sites in .text: the guest linker rewrites these at run time
        text_index = next(i for i, sec in enumerate(m.xff.sections) if sec.name == ".text")
        for tab in m.xff.reloc_tables:
            if tab.sect == text_index:
                for r in tab.relocs:
                    if m.xff.symbols[r.sym].shndx == 0:
                        dynamic.add(t.addr + r.offset)

    sections.sort(key=lambda s: s[1])
    with open(f"{out_dir}/dynamic_imm.txt", "w") as f:
        f.write("# instruction addresses relinked by the game's dynamic linker\n")
        for a in sorted(dynamic):
            f.write(f"{a:08x}\n")
    print(f"{len(dynamic)} dynamic immediate sites")
    write_elf(f"{out_dir}/SOTC.elf", boot.entry, sections)
    seen = set()
    with open(f"{out_dir}/functions.csv", "w") as f:
        f.write("Name,Start,End,Size\n")
        for a, e, n in sorted(funcs):
            if a in seen:
                continue
            seen.add(a)
            f.write(f"{n},0x{a:08X},0x{e:08X},{e - a}\n")
    # Starts merged into a function: entry points (tools/gen_config.py).
    with open(f"{out_dir}/merged_entries.txt", "w") as f:
        f.write("# function starts merged into the function around them\n")
        for a in sorted(set(merged)):
            f.write(f"{a:08x}\n")
    print(f"{len(set(merged))} false splits merged")
    for a, src in sorted(registered_addresses(root).items()):
        if a in set(merged) and a not in seen and a not in quiet:
            print(f"warning: {src} registers 0x{a:08x}, merged into the function around it: "
                  f"add it to game/pinned_starts.txt if gotos must reach the override")
    with open(f"{out_dir}/symbols.txt", "w") as f:
        for a, n in sorted(syms):
            f.write(f"{a} {n}\n")
    print(f"wrote {out_dir}/SOTC.elf with {len(seen)} functions")


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4:])
