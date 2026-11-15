"""Capture a loaded XFF module from a guest RAM dump.

After the game's loader runs, the in-RAM header has absolute section addresses
(memPt) and the symbol table holds final symbol addresses.
"""
from __future__ import annotations  # `bytes | None` on Python 3.9 (macOS's own python3)

import struct
from dataclasses import dataclass
from xff import Xff

RAM_MASK = 0x1FFFFFF
STT_FUNC = 2


@dataclass
class ModSection:
    name: str
    addr: int
    size: int
    data: bytes | None   # None for NOBITS
    exec: bool


def find_header(xff_path, ram):
    """Locate a module's loaded xff2 header: the 16-byte aligned copy that best
    matches the file's own header (the loader only patches a few pointers)."""
    head = open(xff_path, "rb").read()[:0x80]
    best, best_score = None, 0
    pos = ram.find(b"xff2")
    while pos >= 0:
        if pos % 16 == 0:
            score = sum(head[i:i + 4] == ram[pos + i:pos + i + 4] for i in range(0, len(head), 4))
            if score > best_score:
                best, best_score = pos, score
        pos = ram.find(b"xff2", pos + 4)
    if best is None or best_score < len(head) // 8:
        raise ValueError(f"{xff_path}: module header not found in RAM dump")
    return best


class LoadedModule:
    def __init__(self, xff_path, ram, header_addr):
        self.xff = x = Xff(xff_path)
        self.name = xff_path.split("/")[-1]
        self.base = header_addr
        u32 = lambda a: struct.unpack_from("<I", ram, a & RAM_MASK)[0]
        if ram[header_addr:header_addr + 4] != b"xff2":
            raise ValueError(f"no xff2 header at {header_addr:#x}")
        self.sections = []
        self.sect_addr = {}
        for s in x.sections:
            mem = u32(header_addr + x.sect_off + s.index * 0x20)
            self.sect_addr[s.index] = mem
            if not s.size or s.type not in (1, 8, 0x7FFFF420) or s.name.startswith(".DVP") or s.name == ".reginfo":
                continue
            nobits = s.type == 8
            data = None if nobits else bytes(ram[mem & RAM_MASK:(mem & RAM_MASK) + s.size])
            self.sections.append(ModSection(s.name, mem, s.size, data, s.name == ".text"))
        self.symbols = []
        for s in x.symbols:
            addr = u32(header_addr + x.sym_off + s.index * 0x10 + 4)
            self.symbols.append((s, addr))

    def text(self):
        return next(s for s in self.sections if s.exec)

    def functions(self):
        """(addr, size, name) for every function symbol defined in .text."""
        t = self.text()
        out = {}
        for s, addr in self.symbols:
            if s.type == STT_FUNC and s.name and t.addr <= addr < t.addr + t.size:
                if addr not in out or (s.bind and not out[addr][2]):
                    out[addr] = (s.size, s.name, s.bind)
        return {a: (sz, n) for a, (sz, n, b) in out.items()}

    def data_symbols(self):
        return {addr: s.name for s, addr in self.symbols
                if s.name and s.type == 1 and s.shndx not in (0, 0xFFF1)}
