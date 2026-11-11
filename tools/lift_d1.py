#!/usr/bin/env python3
"""lift_d1.py: guest functions lifted to native C++ that is exact and at least as fast as the recompiled code
(decomp round D1, `SOTC_LIFTD1`).

Round 126's lifter (tools/lift126.py) wrote its own instruction semantics with every register as an int64 local and
ran 6-10x slower than the recompiled code. This one starts from the other end: the semantics of every instruction are
the recompiler's own (the generated function in build/runner/<name>_0x<addr>.cpp: its per-instruction C, the FPU's
finite / clamped variants, the VU0 macro ops, the call and tail-jump protocol, the loops' EE clock checkpoints), and
what changes is where the state lives:

  - every GPR is a host local named after the MIPS register (a0, s1, ra ...): `uint32_t` when the function only ever
    gives it 32-bit values (the ABI keeps a 32-bit value sign-extended in its 64-bit register, so the low half is the
    whole value), `uint64_t` when a 64-bit op (ld, dsll32, daddu of two registers, mflo, MMI ...) writes it or a
    value moves into it from such a register. A 64-bit read of a register the function has not written yet (the
    prologue's sd / sq of s0-s7, ra) reads the context's full 128-bit register, and an epilogue's ld / lq of the
    same stack slot writes it back in full, so callers' 64-bit values are kept bit for bit;
  - the FPU registers, fcr31 (the condition bit), the FPU accumulator, VU0's vf0-vf31, ACC, Q and status, lo / hi
    are host locals too (the generated code keeps the VU0 registers, fcr31 and the condition bit in the context:
    a load and a store per instruction, and every guest store through rdram may alias them);
  - the context is written only where something can read it: before a call, a tail jump, a return, a runtime call
    (syscall / break) and in a loop checkpoint's slow path, and then only the registers that can be dirty there (a
    forward data-flow over the function's control-flow graph); after a call every local is read back (the dead ones
    fold away). The generated code writes every register through to the context at each instruction and reloads at
    every label.

Control flow is lifted from the MIPS (delay slots, likely branches, the generator's back-edge checkpoints at the same
edges with the same cycle charge, jump tables from the generated switch), so the EE clock is charged exactly as the
recompiled code charges it and a call that sleeps or yields leaves the context exactly as the recompiled code would:
the scheduler resumes at the return label in the recompiled function. Forward branches that skip a block nobody else
enters become `if`, single-entry backward loops become `do { } while`.

Refused (the function stays recompiled): branches into a delay slot or out of the function, code that runs off its
end, instructions whose generated code reads the context in other ways (COP0, `ctx->pc` readers other than
syscall / break, VU0 integer and control registers other than status / Q ...), and anything the parser does not know.

Registration: SOTC_DECOMP_LIFT_FUNCTION (sotc_decomp.h): sotc_decomp::install puts a lifted function in the table only
while SOTC_LIFTD1 is on and the slot still holds the generated function (a hook keeps its place); an A/B window
(SOTC_AB_FLAG=liftd1) swaps the table entries. docs/decomp.md "Decomp D1".

Usage: lift_d1.py -o out_{n}.cpp [--chunk 160] [--leaf] [--refused refused.txt] addr|@file ...
"""
import argparse
import os
import re
import sys

RUNNER = '/Users/flaviohenrique/Documents/workspace/games/sotc/build/runner'
GPR = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
       's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
# Non-GPR state kept in locals: the context's field -> (local, C type)
FIELDS = {'fcr31': ('fcr31', 'uint32_t'), 'f_acc': ('facc', 'float'), 'vu0_acc': ('vacc', '__m128'),
          'vu0_q': ('vq', 'float'), 'vu0_status': ('vstat', 'uint16_t'), 'lo': ('lo', 'uint64_t'),
          'hi': ('hi', 'uint64_t')}


class Refuse(Exception):
    pass


def sx16(v):
    return v - 0x10000 if v & 0x8000 else v


def split_args(s, i):
    """s[i] is just after '(' : returns (args list, index after the matching ')')."""
    depth, args, cur = 0, [], ''
    while i < len(s):
        c = s[i]
        if c in '([{':
            depth += 1
        elif c in ')]}':
            if depth == 0:
                args.append(cur.strip())
                return args, i + 1
            depth -= 1
        elif c == ',' and depth == 0:
            args.append(cur.strip())
            cur = ''
            i += 1
            continue
        cur += c
        i += 1
    raise Refuse('unbalanced parentheses')


def macro_sub(text, name, fn):
    """Replace every call NAME(args) in text by fn(args) (innermost first is not needed: fn gets raw args)."""
    out, i = '', 0
    pat = re.compile(r'\b' + name + r'\(')
    while True:
        m = pat.search(text, i)
        if not m:
            return out + text[i:]
        args, j = split_args(text, m.end())
        out += text[i:m.start()] + fn(args)
        i = j


CONTROL = re.compile(r'^\s*(\{ (?:Ps2SetGpr64\(ctx, \d+, \w+\); )+(?:goto|return)|\{$|\}$|if \((?:branch_taken|!PS2_|PS2_|runtime->|__builtin_expect\(ctx->pc)|const bool branch_taken|const uint32_t jumpTarget|ctx->pc = |goto |return;|switch \(|case |'
                     r'default:|#|PS2_CALL|PS2_TAIL|PS2_JUMP|runtime->jumpGuest|ctx->in_delay_slot|ctx->branch_pc|\(void\)runtime->dispatch|'
                     r'label_|// NOP|// VWAITQ)')
COMMENT = re.compile(r'^\s*// 0x([0-9a-f]+): 0x([0-9a-f]+)\s+(.*?)\s*$')


_TABLE = None


def table():
    """guest address -> the generated function the table holds at that slot (its own entry)."""
    global _TABLE
    if _TABLE is None:
        _TABLE = {}
        for m in re.finditer(r'= (\w+_0x([0-9a-f]+)); // 0x([0-9a-f]+)', open(f'{RUNNER}/register_functions.cpp').read()):
            _TABLE.setdefault(int(m.group(3), 16), m.group(1))
    return _TABLE


class Gen:
    """The generated function: its words, each instruction's statement text, each control block's lines."""

    def __init__(self, addr):
        import glob
        name = table().get(addr)
        f = [f'{RUNNER}/{name}.cpp'] if name and os.path.exists(f'{RUNNER}/{name}.cpp') else []
        if not f:
            raise Refuse('no generated file for the table entry')
        self.path = f[0]
        self.src = open(f[0], errors='replace').read()
        self.cname = os.path.basename(f[0])[:-4]
        self.name = self.cname[:self.cname.rfind('_0x')]
        m = re.search(r'// Address: 0x([0-9a-f]+) - 0x([0-9a-f]+)', self.src)
        if not m:
            raise Refuse('no address range (a stub?)')
        self.start, self.end = int(m.group(1), 16), int(m.group(2), 16)
        if self.start != addr:
            raise Refuse('the table entry starts elsewhere')
        body = self.src[self.src.find('{', self.src.find(f'void {self.cname}(')) + 1:]
        self.words, self.mnem, self.chunks, self.blocks = {}, {}, {}, {}
        cur = None
        lines = body.split('\n')
        for k, ln in enumerate(lines):
            m = COMMENT.match(ln)
            if m:
                cur = int(m.group(1), 16)
                self.words[cur] = int(m.group(2), 16)
                self.mnem[cur] = m.group(3).replace(' (Delay Slot)', '')
                self.chunks.setdefault(cur, [])
                # the control block that follows a branch / jump's comment: lines up to the matching brace
                self.blocks[cur] = lines[k + 1:k + 60]
                continue
            if cur is not None:
                self.chunks[cur].append(ln)

    def stmt_text(self, a):
        """The instruction's statement lines (control and bookkeeping lines left out)."""
        out = []
        chunk = self.chunks.get(a, [])
        if any('runtime->handle' in ln for ln in chunk):  # syscall / break: their pc stores and the runtime call as they are
            keep = [ln.strip() for ln in chunk if ln.strip() and not ln.strip().startswith(('//', 'label_'))]
            return ' '.join(keep)
        dropped = []
        for ln in chunk:
            if CONTROL.match(ln):
                dropped.append(ln)
                # a control line ends the statement part (a delay slot's chunk goes on with its branch's lines)
                if ln.strip() in ('{', '}') or re.match(r'\{ (?:Ps2SetGpr64\(ctx, \d+, \w+\); )+(?:goto|return)', ln.strip()) or ln.strip().startswith(('if (branch_taken', 'if (!PS2_', 'if (PS2_', 'if (runtime->', 'goto', 'return', 'label_', '#', 'case', 'default', 'switch',
                                                                      'PS2_', 'runtime->jumpGuest', 'const bool', 'const uint32_t jumpTarget')):
                    break
                continue
            out.append(ln.strip())
        # audit: a dropped line must not carry an instruction's effect (only branches, jumps, pc and write-throughs)
        for ln in dropped:
            x = re.sub(r'rc_(\d+) = Ps2GprU64\(ctx, \1\);|Ps2SetGpr64\(ctx, \d+, \w+\);|rf_(\d+) = ctx->f\[\2\];|ctx->f\[(\d+)\] = rf_\3;', '', ln)
            if re.search(r'RC_SET|WRITE|memcpy\(|\brf_\d+ =|ctx->(vu0|fcr31|f\[|lo\b|hi\b|r\[)|SET_GPR|runtime->handle', x):
                raise Refuse(f'a dropped line with an effect at {a:x}: {ln.strip()[:80]}')
        t = ' '.join(out)
        t = re.sub(r'rc_(\d+) = Ps2GprU64\(ctx, \1\);\s*', '', t)
        t = re.sub(r'rf_(\d+) = ctx->f\[\1\];\s*', '', t)
        t = re.sub(r'ctx->f\[(\d+)\] = rf_\1;\s*', '', t)
        t = macro_sub(t, 'Ps2SetGpr64', lambda a: '__DROP__')
        t = t.replace('__DROP__;', '').strip()
        return t

    def block_text(self, a):
        return '\n'.join(self.blocks.get(a, []))


def decode_branch(a, w):
    """(kind, rs, rt, target, likely) for conditional / unconditional PC-relative branches, else None."""
    op = w >> 26
    rs, rt = (w >> 21) & 31, (w >> 16) & 31
    tgt = (a + 4 + (sx16(w & 0xFFFF) << 2)) & 0xFFFFFFFF
    if op in (4, 0x14): return ('eq', rs, rt, tgt, op == 0x14)
    if op in (5, 0x15): return ('ne', rs, rt, tgt, op == 0x15)
    if op in (6, 0x16): return ('lez', rs, 0, tgt, op == 0x16)
    if op in (7, 0x17): return ('gtz', rs, 0, tgt, op == 0x17)
    if op == 1 and rt in (0, 2): return ('ltz', rs, 0, tgt, rt == 2)
    if op == 1 and rt in (1, 3): return ('gez', rs, 0, tgt, rt == 3)
    if op == 0x11 and rs == 8 and rt in (0, 1, 2, 3):
        return ('c1t' if rt & 1 else 'c1f', 0, 0, tgt, bool(rt & 2))
    return None


def negate(c):
    """The negation of a branch condition, as a comparison where it is one."""
    for a, b in ((' == ', ' != '), (' != ', ' == '), (' <= ', ' > '), (' >= ', ' < '), (' < ', ' >= '), (' > ', ' <= ')):
        if c.count(a) == 1 and not any(x in c for x in ('&&', '||', '?')):
            l, r = c.split(a)
            if not any(op in l + r for op in ('==', '!=', '<', '>')) or (a.strip() in ('==', '!=') and '<' not in l + r and '>' not in l + r):
                return l + b + r
    return f'!({c})'


def is_ctl(w):
    op = w >> 26
    if decode_branch(0, w):
        return True
    if op in (2, 3):
        return True
    if op == 0 and (w & 63) in (8, 9):
        return True
    if op == 1 and ((w >> 16) & 31) in (0x10, 0x11, 0x12, 0x13):  # bltzal...
        return True
    if op in (0x10, 0x12) and ((w >> 21) & 31) == 8:  # bc0 / bc2
        return True
    return False


# Mnemonics whose GPR result is a sign-extended 32-bit value of 32-bit sources (a "narrow" producer)
NARROW = {'addu', 'addiu', 'addi', 'add', 'subu', 'sub', 'sll', 'srl', 'sra', 'sllv', 'srlv', 'srav', 'lui', 'slt', 'sltu', 'slti',
          'sltiu', 'lw', 'lh', 'lhu', 'lb', 'lbu', 'mfc1', 'cfc1', 'cfc2.ni', 'cfc2', 'negu', 'neg', 'lwl', 'lwr', 'mult', 'multu', 'mult1',
          'multu1', 'madd', 'maddu', 'jal', 'jalr', 'bltzal', 'bgezal'}
# Whose GPR result takes its sources' width (moves and bitwise ops)
PRESERVE = {'or', 'and', 'xor', 'nor', 'ori', 'andi', 'xori', 'movz', 'movn', 'move'}


class Lifter:
    def __init__(self, addr, lifted_names):
        self.g = Gen(addr)
        self.addr = addr
        self.lifted = lifted_names  # address -> native name of the batch's other lifted functions

    # ---------------------------------------------------------------- analysis
    def analyse(self):
        g = self.g
        # the generator leaves a nop delay slot out of its listing
        for a in range(g.start, g.end, 4):
            if a not in g.words and a - 4 in g.words and is_ctl(g.words[a - 4]):
                g.words[a] = 0
                g.mnem[a] = 'nop'
                g.chunks[a] = []
        addrs = sorted(a for a in g.words if g.start <= a < g.end)
        if not addrs or addrs[0] != g.start or len(addrs) != (g.end - g.start) // 4:
            raise Refuse('listing')
        self.addrs = addrs
        W = g.words
        self.delay = set()
        for a in addrs:
            if is_ctl(W[a]):
                if a + 4 >= g.end:
                    raise Refuse('control at the end')
                if is_ctl(W[a + 4]):
                    raise Refuse(f'control in a delay slot at {a + 4:x}')
                self.delay.add(a + 4)
        # instruction kinds
        self.kind = {}
        self.targets = set()
        for a in addrs:
            if a in self.delay:
                continue
            w = W[a]
            op, fn = w >> 26, w & 63
            br = decode_branch(a, w)
            if br:
                t = br[3]
                if not (g.start <= t < g.end):
                    raise Refuse(f'branch out of the function at {a:x}')
                if t in self.delay:
                    raise Refuse(f'branch into a delay slot at {a:x}')
                self.kind[a] = ('br',) + br
                self.targets.add(t)
            elif op == 2 or op == 3:
                t = ((a + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                dyn = ('READ32(0x%Xu)' % a).lower() in g.block_text(a).split('// 0x')[0].lower()
                if op == 3:
                    self.kind[a] = ('jal', t)
                elif g.start <= t < g.end and not dyn:
                    if t in self.delay:
                        raise Refuse(f'jump into a delay slot at {a:x}')
                    self.kind[a] = ('j', t)
                    self.targets.add(t)
                else:
                    self.kind[a] = ('tail', t)
            elif op == 0 and fn == 8:
                rs = (w >> 21) & 31
                if rs == 31:
                    self.kind[a] = ('ret',)
                else:
                    cases = [int(x, 16) for x in re.findall(r'case 0x([0-9A-Fa-f]+)u: goto label_', g.block_text(a).split('PS2_JUMP_GUEST')[0])]
                    if not cases:
                        self.kind[a] = ('jr', rs, [])
                    else:
                        for t in cases:
                            if not (g.start <= t < g.end) or t in self.delay:
                                raise Refuse(f'jump table target {t:x}')
                        self.targets.update(cases)
                        self.kind[a] = ('jr', rs, cases)
            elif op == 0 and fn == 9:
                self.kind[a] = ('jalr', (w >> 21) & 31, (w >> 11) & 31)
            elif is_ctl(w):
                raise Refuse(f'control op {g.mnem[a]} at {a:x}')
            else:
                self.kind[a] = ('op',)
        # the generator's back-edge checkpoints must be the ones expected
        for a, k in self.kind.items():
            if k[0] in ('br', 'j'):
                t = k[4] if k[0] == 'br' else k[1]
                bt = g.block_text(a)
                has = ('PS2_CHECKPOINT_DUE_AT(0x%Xu)' % t) in bt
                if (t <= a) != has:
                    raise Refuse(f'checkpoint pattern at {a:x}')
        self.stmts()

    # per-instruction statement texts and their slot effects
    def stmts(self):
        g = self.g
        self.raw = {}
        for a in self.addrs:
            k = self.kind.get(a, ('op',))
            if a in self.delay or k[0] == 'op':
                t = g.stmt_text(a)
                if re.search(r'\b(goto|return)\b|label_', t):
                    raise Refuse(f'control in a statement at {a:x}')
                self.raw[a] = t

    def gpr_defs_raw(self, t):
        d = set(int(x) for x in re.findall(r'RC_SET_\w+\(rc_(\d+)\b', t))
        d |= set(int(x) for x in re.findall(r'RC_SET_VEC\(ctx, (\d+),', t))
        d |= set(int(x) for x in re.findall(r'SET_GPR_\w+\(ctx, (\d+),', t))
        return d

    def gpr_uses_raw(self, t):
        u = set(int(x) for x in re.findall(r'\brc_(\d+)\b', t))
        u |= set(int(x) for x in re.findall(r'\bGPR_\w+\(ctx, (\d+)\)', t))
        return u

    def widths(self):
        """GPR local types. A function with 64-bit integer work (the d* shifts and adds, lwu, mflo / mfhi, MMI ...)
        keeps every GPR as uint64_t, as the generated code does: exact by construction. Else a GPR is uint32_t (its
        value the sign-extended low word, as the ABI keeps 32-bit values) unless a 64 / 128-bit transfer (ld other
        than a restore, lq, qmfc2) writes it or a move / bitwise op copies such a value into it (directed: a narrow value
        moved into a wide register stays exact)."""
        W, g = self.g.words, self.g
        wide = set()
        edges = []  # (dest, sources) of the moves / bitwise ops
        self.mode64 = False
        for a in self.addrs:
            t = self.raw.get(a)
            if t is None:
                continue
            m = g.mnem[a].split()[0]
            defs = self.gpr_defs_raw(t) - {0}
            if not defs or a in self.restores:
                continue
            w = W[a]
            if m in ('daddu', 'dsubu', 'dadd', 'dsub'):
                rs, rt = (w >> 21) & 31, (w >> 16) & 31
                if rt == 0 or (m in ('daddu', 'dadd') and rs == 0):
                    m = 'move'
            if m in NARROW:
                continue
            if m in PRESERVE:
                edges.append((defs, self.gpr_uses_raw(t) - {0}))
                continue
            if m in ('lq', 'ld', 'qmfc2.ni', 'qmfc2', 'qmfc2.i'):  # bits moved whole: a wide local keeps them
                wide |= defs
                continue
            self.mode64 = True
        if self.mode64:
            self.wide = set(range(1, 32))
            return
        changed = True
        while changed:
            changed = False
            for d, srcs in edges:
                if srcs & wide and not d <= wide:
                    wide |= d
                    changed = True
        self.wide = wide

    def find_saves(self):
        """Prologue saves (sd / sq of a register no path has written yet, at a fixed offset from sp's entry value)
        and the loads of the same slot (restores: the register's full value back in the context). Fills
        self.restores. sp's offset at each instruction comes from a forward pass over the CFG (only addiu sp, sp, imm
        may change it, the same on every path); anything else: no restores."""
        g, W = self.g, self.g.words
        spoff = {self.order[0]: 0}
        work = [self.order[0]]
        while work:
            a = work.pop()
            o = spoff[a]
            for x in ((a, a + 4) if a + 4 in self.delay else (a,)):
                w = W[x]
                t = self.raw.get(x, '')
                if (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29:
                    o += sx16(w & 0xFFFF)
                elif 29 in self.gpr_defs_raw(t):
                    return {}
            for s_ in self.succ[a]:
                if s_ in spoff:
                    if spoff[s_] != o:
                        return {}
                else:
                    spoff[s_] = o
                    work.append(s_)
        def off_at(x):
            node = x - 4 if x in self.delay else x
            if node not in spoff:
                return None
            o = spoff[node]
            if x in self.delay:  # the branch's own sp change (none: a control op) then the slot
                pass
            return o
        saves, stores = {}, []
        for x in self.addrs:
            w = W[x]
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, sx16(w & 0xFFFF)
            o = off_at(x)
            if o is None:
                continue
            node = x - 4 if x in self.delay else x
            if rs == 29 and op in (0x28, 0x29, 0x2B, 0x39, 0x3E, 0x3F, 0x1F, 0x2A, 0x2E, 0x2C, 0x2D, 0x3C):
                size = {0x28: 1, 0x29: 2, 0x2B: 4, 0x39: 4, 0x3E: 16, 0x3F: 8, 0x1F: 16}.get(op, 16)
                stores.append((o + imm, size, x))
                if op in (0x3F, 0x1F) and rt != 0 and rt in self.undef_in.get(node, set()) and not (x in self.delay and rt == 31 and self.kind[node][0] == 'jal') and rt not in saves:
                    saves[rt] = (o + imm, size, x)
        for x in self.addrs:
            w = W[x]
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, sx16(w & 0xFFFF)
            o = off_at(x)
            if o is None or rs != 29 or op not in (0x37, 0x1E) or rt not in saves:
                continue
            off, size, sa = saves[rt]
            if off == o + imm and size == (8 if op == 0x37 else 16):
                if not [s_ for s_ in stores if s_[2] != sa and s_[0] < off + size and off < s_[0] + s_[1]]:
                    self.restores[x] = (rt, size)
        return saves

    # ---------------------------------------------------------------- emission helpers
    def gname(self, r):
        return GPR[r]

    def is_wide(self, r):
        return r in self.wide

    def v64(self, r, entry):
        """A GPR's 64-bit value as an expression."""
        if r == 0:
            return '0ULL'
        if entry:
            return f'Ps2GprU64(ctx, {r})'
        n = self.gname(r)
        return n if self.is_wide(r) else f'sx64({n})'

    def rename(self, t, a, undefined):
        """Generated statement text -> text over the locals. `undefined`: GPRs no path to `a` has written."""
        self.slots_used = set()

        def use(s):
            self.slots_used.add(s)
            return s

        def regnum(x):
            m = re.fullmatch(r'rc_(\d+)', x.strip())
            if not m:
                raise Refuse(f'unexpected register operand {x!r} at {a:x}')
            return int(m.group(1))

        def set_val(r, val, zext=False):
            n = self.gname(r)
            use(('g', r))
            if self.is_wide(r):
                return f'({n} = (uint64_t)({val}))' if not zext else f'({n} = (uint64_t)(uint32_t)({val}))'
            if zext and self.g.mnem[a].split()[0] not in ('lbu', 'lhu', 'andi'):
                raise Refuse(f'zero-extended write to a narrow register at {a:x}')
            return f'({n} = (uint32_t)({val}))'

        def rset(kind):
            def f(args):
                r = regnum(args[0])
                v = args[1]
                if kind in ('S32', 'U32'):
                    v = f'(int32_t)({v})' if not self.is_wide(r) else f'(int64_t)(int32_t)({v})'
                return set_val(r, v, kind == 'ZE32')
            return f
        for kind in ('S32', 'U32', 'U64', 'S64', 'ZE32'):
            t = macro_sub(t, f'RC_SET_{kind}', rset(kind))

        def rset_vec(args):
            r = int(args[1])
            use(('g', r))
            n = self.gname(r)
            low = f'(uint64_t)_mm_cvtsi128_si64(_v)' if not self.is_wide(r) else 'Ps2GprVecLow(_v)'
            if self.is_wide(r):
                return f'do {{ const __m128i _v = ({args[3]}); ctx->r[{r}] = _v; std::memcpy(&{n}, &_v, 8); }} while (0)'
            return f'do {{ const __m128i _v = ({args[3]}); ctx->r[{r}] = _v; {n} = (uint32_t)_mm_cvtsi128_si32(_v); }} while (0)'
        t = macro_sub(t, 'RC_SET_VEC', rset_vec)

        def rvec(sx):
            def f(args):
                r = int(args[1])
                use(('g', r))
                if r in undefined:
                    return f'ctx->r[{r}]'
                return f'Ps2GprVecWithLow(ctx->r[{r}], {self.v64(r, False)})'
            return f
        t = macro_sub(t, 'RC_VECX', rvec(True))
        t = macro_sub(t, 'RC_VEC', rvec(False))

        def gpr_vec(args):
            r = int(args[1])
            if r == 0:
                return '_mm_setzero_si128()'
            use(('g', r))
            if r in undefined:
                return f'ctx->r[{r}]'
            return f'Ps2GprVecWithLow(ctx->r[{r}], {self.v64(r, False)})'
        t = macro_sub(t, 'GPR_VEC', gpr_vec)

        def rd(kind):
            def f(args):
                x = args[0].strip()
                if x == '0':
                    return {'U32': '0u', 'S32': '0', 'U64X': '0ULL', 'S64X': '0LL', 'U64': '0ULL', 'S64': '0LL'}[kind]
                r = regnum(x)
                n = self.gname(r)
                use(('g', r))
                wide = self.is_wide(r)
                if kind == 'U32':
                    return n if not wide else f'(uint32_t){n}'
                if kind == 'S32':
                    return f'(int32_t){n}'
                if kind in ('U64X', 'S64X'):
                    v = f'sx64({n})' if not wide else f'RC_U64X({n})'
                    return v if kind == 'U64X' else f'(int64_t){v}'
                v = self.v64(r, r in undefined and not wide)
                if wide:
                    v = n
                return v if kind == 'U64' else f'(int64_t){v}'
            return f
        for kind in ('U32', 'S32', 'U64X', 'S64X', 'U64', 'S64'):
            t = macro_sub(t, f'RC_{kind}', rd(kind))

        def gpr_rd(kind):
            def f(args):
                r = int(args[1])
                if r == 0:
                    return '0'
                use(('g', r))
                n = self.gname(r)
                if kind == 'U32':
                    return n if not self.is_wide(r) else f'(uint32_t){n}'
                if kind == 'S32':
                    return f'(int32_t){n}'
                v = self.v64(r, r in undefined and not self.is_wide(r))
                return v if kind == 'U64' else f'(int64_t){v}'
            return f
        for kind in ('U32', 'S32', 'U64', 'S64'):
            t = macro_sub(t, f'GPR_{kind}', gpr_rd(kind))
        t = macro_sub(t, 'RC_SET_ZERO', lambda args: f'(void)({args[0]})')

        def setgpr(kind):
            def f(args):
                r = int(args[1])
                if r == 0:
                    return f'(void)({args[2]})'
                v = args[2]
                if kind in ('U32', 'S32'):
                    v = f'(int32_t)({v})' if not self.is_wide(r) else f'(int64_t)(int32_t)({v})'
                return set_val(r, v, kind == 'ZE32')
            return f
        for kind in ('U32', 'S32', 'U64', 'S64', 'ZE32'):
            t = macro_sub(t, f'SET_GPR_{kind}', setgpr(kind))
        if re.search(r'\brc_\d+', t):
            raise Refuse(f'register operand form at {a:x}: {t[:120]}')
        # FPU, VU0 and the other state
        t = re.sub(r'\brf_(\d+)\b', lambda m: (use(('f', int(m.group(1)))), f'f{m.group(1)}')[1], t)
        t = re.sub(r'ctx->f\[(\d+)\]', lambda m: (use(('f', int(m.group(1)))), f'f{m.group(1)}')[1], t)
        t = re.sub(r'ctx->vu0_vf\[(\d+)\]', lambda m: (use(('v', int(m.group(1)))), f'vf{m.group(1)}')[1], t)
        t = re.sub(r'FPU_SET_ACC\(ctx, ', lambda m: (use(('x', 'f_acc')), 'LIFT_SET_FACC(')[1], t)
        for fld, (loc, _) in FIELDS.items():
            t = re.sub(r'ctx->' + fld + r'\b', lambda m, fld=fld, loc=loc: (use(('x', fld)), loc)[1], t)
        rest = re.sub(r'ctx->r\[\d+\]|Ps2GprU64\(ctx, \d+\)', '', t)
        self.opaque = bool(re.search(r'\bctx\b|runtime->', rest))
        return t

    def text_defs(self, t, raw):
        """Slots a renamed statement may write (conservative)."""
        d = set(('g', r) for r in self.gpr_defs_raw(raw))
        for m in re.finditer(r'\bf(\d+)\s*=(?!=)|memcpy\(&f(\d+)\b', t):
            d.add(('f', int(m.group(1) or m.group(2))))
        for m in re.finditer(r'\bvf(\d+)\s*=(?!=)', t):
            d.add(('v', int(m.group(1))))
        for fld, (loc, _) in FIELDS.items():
            if re.search(r'\b' + loc + r'\s*(=(?!=)|\|=|&=)|memcpy\(&' + loc + r'\b', t) or re.search(r'\(\s*' + loc + r'\s*,', t):
                d.add(('x', fld))
        if 'LIFT_SET_FACC(' in t:
            d.add(('x', 'f_acc'))
        return d

    # ---------------------------------------------------------------- the per-instruction IR
    def build(self):
        """IR: for each non-delay instruction a node with its code parts and effects; CFG successors."""
        self.analyse()
        g = self.g
        # pass 1: may-defined GPRs (forward, union) on a coarse CFG, for the entry-value rule
        succ = {}
        order = [a for a in self.addrs if a not in self.delay]
        nxt = {order[i]: order[i + 1] for i in range(len(order) - 1)}
        for a in order:
            k = self.kind[a]
            s = []
            if k[0] == 'br':
                s = [k[4]]
                if not (k[1] == 'eq' and k[2] == 0 and k[3] == 0):
                    s.append(nxt.get(a))
            elif k[0] == 'j':
                s = [k[1]]
            elif k[0] == 'jr':
                s = list(k[2])
            elif k[0] in ('ret', 'tail'):
                s = []
            else:
                s = [nxt.get(a)]
            if None in s:
                succ[a] = None  # falls off the function's end (refused if reachable)
            else:
                succ[a] = s
        seen, work = {order[0]}, [order[0]]
        while work:
            a = work.pop()
            if succ[a] is None:
                raise Refuse(f'runs off its end at {a:x}')
            for x in succ[a]:
                if x not in seen:
                    seen.add(x)
                    work.append(x)
        for a in order:
            if succ[a] is None:
                succ[a] = []
        self.succ = succ
        self.order = order

        self.restores = {}  # address -> (reg, size)
        self.undef_pass()
        self.find_saves()
        if self.restores:
            self.undef_pass()
        self.widths()

    def raw_opaque(self, t):
        """True when an instruction's generated code touches the context or the runtime beyond the state the locals
        hold (syscall, break, COP0, VU0 integer / control registers ...)."""
        r = re.sub(r'\b(GPR_\w+|SET_GPR_\w+|RC_SET_VEC|RC_VECX?|FPU_SET_ACC)\(ctx, ', '(', t)
        r = re.sub(r'ctx->(?:f\[\d+\]|vu0_vf\[\d+\]|(?:' + '|'.join(FIELDS) + r')\b)', '', r)
        return bool(re.search(r'\bctx\b|runtime->', r))

    def undef_pass(self):
        """Per node: the GPRs whose context register holds their current full value and that the function has not
        written since (entry, a call, an opaque instruction, a restore): a 64-bit read of one reads the context."""
        def step(x, u):
            if x in self.restores:
                u.add(self.restores[x][0])
                return u
            if x in self.raw:
                if self.raw_opaque(self.raw[x]):
                    return set(range(32))
                u -= self.gpr_defs_raw(self.raw[x])
            return u

        def through(a, u):
            u = set(u)
            k = self.kind[a]
            if k[0] == 'op':
                return step(a, u)
            if k[0] == 'jal':
                u.discard(31)
            if k[0] == 'jalr':
                u.discard(k[2])
            if a + 4 in self.delay:
                u = step(a + 4, u)
            if k[0] in ('jal', 'jalr'):
                return set(range(32))
            return u
        order, succ = self.order, self.succ
        undef_in = {a: None for a in order}
        undef_in[order[0]] = set(range(32))
        work = [order[0]]
        while work:
            a = work.pop()
            u = through(a, undef_in[a])
            for s_ in succ[a]:
                old = undef_in[s_]
                new = u if old is None else (old & u)
                if old is None or new != old:
                    undef_in[s_] = new
                    work.append(s_)
        self.undef_in = {a: (v if v is not None else set()) for a, v in undef_in.items()}

    def stmt(self, a, undefined):
        """(code, uses, defs, opaque, clean) of a non-control instruction (or a delay slot)."""
        raw = self.raw[a]
        if a in self.restores:
            r, size = self.restores[a]
            n = self.gname(r)
            sp = self.gname(29)
            imm = sx16(self.g.words[a] & 0xFFFF)
            ea = f'{sp} + {imm}u' if imm >= 0 else f'{sp} - {-imm}u'
            if size == 8:
                code = f'{{ const uint64_t v = READ64({ea}); Ps2SetGpr64(ctx, {r}, v); {n} = ({"uint64_t" if self.is_wide(r) else "uint32_t"})v; }} // restore {n}'
            else:
                lo = f'std::memcpy(&{n}, &v, 8);' if self.is_wide(r) else f'{n} = (uint32_t)_mm_cvtsi128_si32(v);'
                code = f'{{ const __m128i v = READ128({ea}); ctx->r[{r}] = v; {lo} }} // restore {n}'
            return code, {('g', 29), ('g', r)}, {('g', r)}, False, {('g', r)}
        own = self.own(a, undefined)
        t = self.rename(raw, a, undefined)
        uses = set(self.slots_used)
        opaque = self.opaque
        defs = self.text_defs(t, raw)
        if own is not None and not opaque:
            t = own
        return t, uses, defs, opaque, set()

    def own(self, a, undefined):
        """Readable code for the common 32-bit instructions on narrow registers (same results as the generated code)."""
        w = self.g.words[a]
        op, rs, rt, rd, sa, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31, w & 63
        m = self.g.mnem[a].split()[0]
        raw = self.raw[a]
        if not raw or 'READ32(0x' in raw:  # relocated immediates: the generated form
            return None
        regs = {rs, rt, rd} - {0}
        if any(self.is_wide(r) for r in regs):
            return None
        N = self.gname
        R = lambda r: '0u' if r == 0 else N(r)
        S = lambda r: '0' if r == 0 else f'(int32_t){N(r)}'
        imm = sx16(w & 0xFFFF)
        uimm = w & 0xFFFF

        def h(v):
            return f'0x{v:X}u' if v > 9 else f'{v}u'

        def addimm(base, i):
            if base == 0:
                return h(i & 0xFFFFFFFF)
            if i == 0:
                return R(base)
            return f'{R(base)} + {h(i)}' if i > 0 else f'{R(base)} - {h(-i)}'

        def dst(r, e):
            if r == 0:
                return None
            return f'{N(r)} = {e};'
        ea = addimm(rs, imm)
        if op == 0:
            if fn == 0 and w == 0:
                return ''
            if m == 'sll': return dst(rd, f'{R(rt)} << {sa}')
            if m == 'srl': return dst(rd, f'{R(rt)} >> {sa}')
            if m == 'sra': return dst(rd, f'(uint32_t)({S(rt)} >> {sa})')
            if m == 'sllv': return dst(rd, f'{R(rt)} << ({R(rs)} & 31)')
            if m == 'srlv': return dst(rd, f'{R(rt)} >> ({R(rs)} & 31)')
            if m == 'srav': return dst(rd, f'(uint32_t)({S(rt)} >> ({R(rs)} & 31))')
            if m == 'addu': return dst(rd, R(rs) if rt == 0 else (R(rt) if rs == 0 else f'{R(rs)} + {R(rt)}'))
            if m == 'subu': return dst(rd, f'0u - {R(rt)}' if rs == 0 else f'{R(rs)} - {R(rt)}')
            if m == 'negu': return dst(rd, f'0u - {R(rt)}')
            if m == 'and': return dst(rd, f'{R(rs)} & {R(rt)}')
            if m == 'or': return dst(rd, R(rs) if rt == 0 else (R(rt) if rs == 0 else f'{R(rs)} | {R(rt)}'))
            if m == 'xor': return dst(rd, f'{R(rs)} ^ {R(rt)}')
            if m == 'nor': return dst(rd, f'~({R(rs)} | {R(rt)})')
            if m == 'slt': return dst(rd, f'{S(rs)} < {S(rt)} ? 1u : 0u')
            if m == 'sltu': return dst(rd, f'{R(rs)} < {R(rt)} ? 1u : 0u')
            if m == 'daddu' and (rt == 0 or rs == 0): return dst(rd, R(rs if rt == 0 else rt))
            if m == 'movz' and rd != 0: return f'if ({R(rt)} == 0u) {N(rd)} = {R(rs)};' if 'RC_SET_VEC' not in raw else None
            if m == 'movn' and rd != 0: return f'if ({R(rt)} != 0u) {N(rd)} = {R(rs)};' if 'RC_SET_VEC' not in raw else None
            return None
        if m == 'addiu': return dst(rt, addimm(rs, imm))
        if m == 'slti': return dst(rt, f'{S(rs)} < {imm} ? 1u : 0u')
        if m == 'sltiu': return dst(rt, f'{R(rs)} < {h(imm & 0xFFFFFFFF)} ? 1u : 0u')
        if m == 'andi': return dst(rt, f'{R(rs)} & {h(uimm)}')
        if m == 'ori': return dst(rt, f'{R(rs)} | {h(uimm)}' if rs else h(uimm))
        if m == 'xori': return dst(rt, f'{R(rs)} ^ {h(uimm)}')
        if m == 'lui': return dst(rt, h((uimm << 16) & 0xFFFFFFFF))
        if m == 'lw': return dst(rt, f'READ32({ea})')
        if m == 'lh': return dst(rt, f'(uint32_t)(int16_t)READ16({ea})')
        if m == 'lhu': return dst(rt, f'READ16({ea})')
        if m == 'lb': return dst(rt, f'(uint32_t)(int8_t)READ8({ea})')
        if m == 'lbu': return dst(rt, f'READ8({ea})')
        if m == 'sw': return f'WRITE32({ea}, {R(rt)});'
        if m == 'sh': return f'WRITE16({ea}, (uint16_t){R(rt)});'
        if m == 'sb': return f'WRITE8({ea}, (uint8_t){R(rt)});'
        if m == 'lwc1': return f'f{rt} = lift_d1::flt(READ32({ea}));'
        if m == 'swc1': return f'WRITE32({ea}, lift_d1::bits(f{rt}));'
        if m == 'mtc1': return f'f{rd} = lift_d1::flt({R(rt)});'
        if m == 'mfc1': return dst(rt, f'lift_d1::bits(f{rd})')
        if m == 'sd' and rt != 0 and rt in undefined:
            return f'WRITE64({ea}, Ps2GprU64(ctx, {rt}));'
        if m == 'sd': return f'WRITE64({ea}, {self.v64(rt, False)});'
        if m == 'sq' and rt != 0 and rt in undefined:
            return f'WRITE128({ea}, ctx->r[{rt}]);'
        return None

    # ---------------------------------------------------------------- emission
    def lift(self):
        self.build()
        g = self.g
        order = self.order
        # per node: its code pieces, effects
        nodes = {}
        allslots = set()
        for a in order:
            k = self.kind[a]
            und = self.undef_in[a]
            n = {'kind': k}
            if k[0] == 'op':
                code, uses, defs, opq, clean = self.stmt(a, und)
                n.update(code=code, uses=uses, defs=defs, opaque=opq, clean=clean)
            else:
                n.update(uses=set(), defs=set(), opaque=False, clean=set())
                d = a + 4
                dund = und
                if k[0] in ('jal', 'jalr'):
                    dund = und - {31 if k[0] == 'jal' else k[2]}
                code, uses, defs, opq, clean = self.stmt(d, dund)
                if opq:
                    raise Refuse(f'opaque delay slot at {d:x}')
                n.update(dcode=code, duses=uses, ddefs=defs, dclean=clean)
                if k[0] == 'br':
                    n['uses'] |= self.cond_uses(k)
                if k[0] in ('jr', 'jalr'):
                    n['uses'].add(('g', k[1]))
                if k[0] in ('jal', 'jalr'):
                    n['uses'].add(('g', 29))
                    rr = 31 if k[0] == 'jal' else k[2]
                    if rr:
                        n['defs'].add(('g', rr))
                        n['uses'].add(('g', rr))
                if k[0] == 'ret':
                    n['uses'].add(('g', 31))
            allslots |= n['uses'] | n['defs'] | n.get('duses', set()) | n.get('ddefs', set())
            nodes[a] = n
        allslots.discard(('g', 0))
        self.allslots = allslots
        # dirty analysis (forward, union): slots whose local may differ from the context
        dirty_in = {a: None for a in order}
        dirty_in[order[0]] = frozenset()

        def transfer(a, din):
            n = nodes[a]
            k = n['kind']
            d = set(din)
            if k[0] == 'op':
                if n['opaque']:
                    return frozenset(), d  # synced before, its own writes after, everything read back
                d |= n['defs']
                d -= n['clean']
                return frozenset(d), None
            pre = set(d)
            if k[0] in ('jal', 'jalr'):
                rr = 31 if k[0] == 'jal' else k[2]
                if rr:
                    pre.add(('g', rr))
            pre |= n['ddefs']
            pre -= n['dclean']
            if k[0] in ('jal', 'jalr', 'jr'):
                return frozenset(), pre  # synced before the call / jump, everything read back after a call
            return frozenset(pre), pre
        self.sync_at = {}
        work = [order[0]]
        while work:
            a = work.pop()
            out, at = transfer(a, dirty_in[a])
            self.sync_at[a] = at
            for s in self.succ[a]:
                old = dirty_in[s]
                new = out if old is None else (old | out)
                if old is None or new != old:
                    dirty_in[s] = new
                    work.append(s)
        self.dirty_in = dirty_in
        self.nodes = nodes
        reach = set(a for a in order if dirty_in[a] is not None)
        return self.emit(reach)

    def cond_uses(self, k):
        if k[1] in ('c1t', 'c1f'):
            return {('x', 'fcr31')}
        return set(('g', r) for r in (k[2], k[3]) if r)

    def cond(self, k):
        kind, rs, rt = k[1], k[2], k[3]
        N = self.gname
        if kind in ('c1t', 'c1f'):
            return '(fcr31 & 0x800000u) != 0u' if kind == 'c1t' else '(fcr31 & 0x800000u) == 0u'
        wide = (rs and self.is_wide(rs)) or (rt and self.is_wide(rt))

        def v(r):
            if r == 0:
                return '0' if not wide else '0LL'
            if wide:
                return f'(int64_t){N(r)}' if self.is_wide(r) else f'(int64_t)(int32_t){N(r)}'
            return N(r)
        if kind == 'eq':
            if rs == rt:
                return 'true'
            return f'{v(rs)} == {v(rt)}' if rt else f'{v(rs)} == 0'
        if kind == 'ne':
            return f'{v(rs)} != {v(rt)}' if rt else f'{v(rs)} != 0'
        sv = v(rs) if wide else f'(int32_t){N(rs)}'
        return {'lez': f'{sv} <= 0', 'gtz': f'{sv} > 0', 'ltz': f'{sv} < 0', 'gez': f'{sv} >= 0'}[kind]

    def sync(self, slots):
        out = []
        for s in sorted(slots, key=lambda x: (x[0], str(x[1]))):
            kind, r = s
            if kind == 'g':
                if r == 0:
                    continue
                out.append(f'Ps2SetGpr64(ctx, {r}, {self.gname(r) if self.is_wide(r) else "sx64(" + self.gname(r) + ")"});')
            elif kind == 'f':
                out.append(f'ctx->f[{r}] = f{r};')
            elif kind == 'v':
                out.append(f'ctx->vu0_vf[{r}] = vf{r};')
            else:
                loc = FIELDS[r][0] if r in FIELDS else 'facc'
                out.append(f'ctx->{r} = {loc};')
        return ' '.join(out)

    def load(self, s):
        kind, r = s
        if kind == 'g':
            return f'{self.gname(r)} = {"Ps2GprU64(ctx, %d)" % r if self.is_wide(r) else "Ps2GprU32(ctx, %d)" % r};'
        if kind == 'f':
            return f'f{r} = ctx->f[{r}];'
        if kind == 'v':
            return f'vf{r} = ctx->vu0_vf[{r}];'
        loc = FIELDS[r][0]
        return f'{loc} = ctx->{r};'

    def reload_all(self):
        return ' '.join(self.load(s) for s in sorted(self.allslots, key=lambda x: (x[0], str(x[1]))))

    def decls(self):
        out = []
        gs = sorted(r for k, r in self.allslots if k == 'g')
        nar = [r for r in gs if not self.is_wide(r)]
        wid = [r for r in gs if self.is_wide(r)]
        if nar:
            out.append('uint32_t ' + ', '.join(f'{self.gname(r)} = Ps2GprU32(ctx, {r})' for r in nar) + ';')
        if wid:
            out.append('uint64_t ' + ', '.join(f'{self.gname(r)} = Ps2GprU64(ctx, {r})' for r in wid) + ';')
        fs = sorted(r for k, r in self.allslots if k == 'f')
        if fs:
            out.append('float ' + ', '.join(f'f{r} = ctx->f[{r}]' for r in fs) + ';')
        vs = sorted(r for k, r in self.allslots if k == 'v')
        if vs:
            out.append('__m128 ' + ', '.join(f'vf{r} = ctx->vu0_vf[{r}]' for r in vs) + ';')
        for k, r in sorted((s for s in self.allslots if s[0] == 'x'), key=lambda x: x[1]):
            loc, ty = FIELDS[r]
            out.append(f'{ty} {loc} = ctx->{r};')
        return out

    def emit(self, reach):
        g = self.g
        order = [a for a in self.order if a in reach]
        L = []
        pos = {a: i for i, a in enumerate(order)}
        # Structured ifs: a forward branch whose skipped nodes nobody enters from outside becomes `if (!c) { ... }`.
        gotos = []  # (src, target) of every explicit transfer
        for a in order:
            k = self.kind[a]
            for t in self.succ[a]:
                if not (k[0] == 'op' or (k[0] in ('jal', 'jalr')) or (t == self.next_of(a) and k[0] == 'br' and t != k[4])):
                    gotos.append((a, t))
        structured = {}
        ends = []
        for a in order:
            while ends and ends[-1] <= a:
                ends.pop()
            k = self.kind[a]
            if k[0] != 'br' or k[1] in ('eq',) and k[2] == 0 and k[3] == 0:
                continue
            t = k[4]
            if t <= a or t not in pos or (ends and t > ends[-1]):
                continue
            lo, hi = pos[a], pos[t]
            inside = set(order[lo + 1:hi])
            if any(tt in inside and not (lo <= pos.get(ss, -1) < hi) for ss, tt in gotos):
                continue
            structured[a] = t
            ends.append(t)
        self.structured = structured
        labels = set(tt for ss, tt in gotos if structured.get(ss) != tt)
        depth = [1]
        closers = []  # (end address, closing lines)

        def ind():
            return '    ' * depth[0]

        def goto(src, t, ind_):
            out = []
            if t <= src:  # a back edge: the generator's checkpoint (lazy pc), synced in its slow path only
                d = self.dirty_after_branch(src)
                out.append(f'{ind_}if (__builtin_expect(!runtime->m_eeClock.charge(32u), 0)) {{ {self.sync(d)} ctx->pc = 0x{t:X}u; if (runtime->eeCheckpointSlow()) return; {self.reload_all()} }}')
            out.append(f'{ind_}goto L_{t:x};')
            return out
        for i, a in enumerate(order):
            while closers and closers[-1][0] == a:
                for ln in closers.pop()[1]:
                    depth[0] -= 1
                    L.append(ind() + ln)
            n = self.nodes[a]
            k = n['kind']
            if a in labels:
                L.append(f'L_{a:x}:')
            I = ind()
            if k[0] == 'op':
                if n['opaque']:
                    din = self.dirty_in[a]
                    L.append(f'{I}{self.sync(din)} // {g.mnem[a]}: the runtime reads the context')
                    L.append(f'{I}{n["code"]}')
                    L.append(f'{I}{self.sync(n["defs"])} {self.reload_all()}')
                elif n['code']:
                    L.append(I + n['code'])
                continue
            dcode = n['dcode']
            if k[0] == 'br':
                c = self.cond(k)
                t = k[4]
                if a in structured:
                    if k[5] and not dcode:
                        L.append(f'{I}if ({negate(c)}) {{')
                        closers.append((t, ['}']))
                        depth[0] += 1
                    elif k[5]:  # likely: the delay slot only when taken
                        L.append(f'{I}if ({c}) {{')
                        if dcode:
                            L.append(I + '    ' + dcode)
                        L.append(f'{I}}} else {{')
                        closers.append((t, ['}']))
                        depth[0] += 1
                    elif dcode:
                        L.append(f'{I}{{')
                        L.append(f'{I}    const bool c = {c};')
                        L.append(I + '    ' + dcode)
                        L.append(f'{I}    if (!c) {{')
                        closers.append((t, ['}', '}']))
                        depth[0] += 2
                    else:
                        L.append(f'{I}if ({negate(c)}) {{')
                        closers.append((t, ['}']))
                        depth[0] += 1
                    continue
                if k[5]:  # likely: the delay slot only when taken
                    L.append(f'{I}if ({c}) {{')
                    if dcode:
                        L.append(I + '    ' + dcode)
                    L.extend(goto(a, t, I + '    '))
                    L.append(f'{I}}}')
                elif c == 'true':
                    if dcode:
                        L.append(I + dcode)
                    L.extend(goto(a, t, I))
                else:
                    if dcode:
                        L.append(f'{I}{{')
                        L.append(f'{I}    const bool c = {c};')
                        L.append(I + '    ' + dcode)
                        L.append(f'{I}    if (c) {{')
                        L.extend(goto(a, t, I + '        '))
                        L.append(f'{I}    }}')
                        L.append(f'{I}}}')
                    else:
                        L.append(f'{I}if ({c}) {{')
                        L.extend(goto(a, t, I + '    '))
                        L.append(f'{I}}}')
                continue
            if k[0] == 'j':
                if dcode:
                    L.append(I + dcode)
                L.extend(goto(a, k[1], I))
                continue
            if k[0] == 'ret':
                L.append(f'{I}{{')
                L.append(f'{I}    const uint32_t target = {"(uint32_t)ra" if self.is_wide(31) else "ra"};')
                if dcode:
                    L.append(I + '    ' + dcode)
                L.append(f'{I}    {self.sync(self.sync_at[a])}')
                L.append(f'{I}    ctx->pc = target;')
                L.append(f'{I}    return;')
                L.append(f'{I}}}')
                continue
            if k[0] in ('jal', 'jalr', 'tail', 'jr'):
                bt = g.block_text(a).split('\nlabel_')[0]
                call = re.search(r'(if \(!PS2_CALL_GUEST(?:_KNOWN)?\(.*?\)\) \{)', bt)
                jt = re.search(r'const uint32_t jumpTarget = (.*?);\n', bt)
                L.append(f'{I}{{')
                if k[0] in ('jalr', 'jr'):
                    rs = k[1]
                    L.append(f'{I}    const uint32_t jumpTarget = {self.gname(rs) if not self.is_wide(rs) else "(uint32_t)" + self.gname(rs)};')
                elif jt:
                    if 'READ32(' not in jt.group(1) and not re.fullmatch(r'0x[0-9A-Fa-f]+u', jt.group(1)):
                        raise Refuse(f'jump target form at {a:x}')
                    L.append(f'{I}    const uint32_t jumpTarget = {jt.group(1)};')
                if k[0] == 'jal':
                    L.append(f'{I}    ra = 0x{a + 8:X}u;')
                if k[0] == 'jalr' and k[2]:
                    L.append(f'{I}    {self.gname(k[2])} = 0x{a + 8:X}u;')
                if dcode:
                    L.append(I + '    ' + dcode)
                L.append(f'{I}    {self.sync(self.sync_at[a])}')
                if k[0] in ('jal', 'jalr'):
                    if not call:
                        raise Refuse(f'call form at {a:x}')
                    line = call.group(1)
                    t = k[1] if k[0] == 'jal' else None
                    if t is not None and t in self.lifted and 'PS2_CALL_GUEST_KNOWN' in line:
                        line = re.sub(r'PS2_CALL_GUEST_KNOWN\(([^,]+), \w+,', lambda m: f'PS2_CALL_GUEST_KNOWN({m.group(1)}, lift_d1::{self.lifted[t]},', line)
                    tgt = 'jumpTarget' if (jt or k[0] == 'jalr') else f'0x{k[1]:X}u'
                    L.append(f'{I}    ctx->pc = {tgt};')
                    L.append(f'{I}    {line} return; }}')
                    L.append(f'{I}}}')
                    L.append(f'{I}{self.reload_all()}')
                    continue
                if k[0] == 'tail':
                    m = re.search(r'(PS2_TAIL_GUEST_KNOWN\(.*?\); return;|PS2_JUMP_GUEST\(.*?\);)', bt)
                    if not m:
                        raise Refuse(f'tail jump form at {a:x}')
                    tgt = 'jumpTarget' if jt else f'0x{k[1]:X}u'
                    L.append(f'{I}    ctx->pc = {tgt};')
                    L.append(f'{I}    {m.group(1)}' + ('' if m.group(1).endswith('return;') else ' return;'))
                    L.append(f'{I}}}')
                    continue
                # jr through a jump table (its cases inside), else a jump out
                L.append(f'{I}    ctx->pc = jumpTarget;')
                if k[2]:
                    # a jump table's targets: synced already (the generated code's state at a label); read back at the label
                    cases = ' '.join(f'case 0x{t:X}u: goto L_{t:x};' for t in k[2])
                    L.append(f'{I}    switch (jumpTarget) {{ {cases} default: break; }}')
                L.append(f'{I}    PS2_JUMP_GUEST(jumpTarget);')
                L.append(f'{I}    return;')
                L.append(f'{I}}}')
                continue
            raise Refuse(f'kind {k}')
        return L

    def next_of(self, a):
        i = self.order.index(a)
        return self.order[i + 1] if i + 1 < len(self.order) else None

    def dirty_after_branch(self, a):
        n = self.nodes[a]
        d = set(self.dirty_in[a] or ())
        d |= n.get('ddefs', set())
        d -= n.get('dclean', set())
        return d

    def text(self, switch):
        body = self.lift()
        name = self.g.name
        start = self.g.start
        out = [f'// {name} ({start:#x}-{self.g.end:#x})']
        out.append(f'void {name}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)')
        out.append('{')
        for d in self.decls():
            out.append('    ' + d)
        out.extend(body)
        out.append('}')
        return '\n'.join(out)


HEADER = '''// Generated by tools/lift_d1.py (decomp round D1): guest functions as native C++ on host locals, exact (the
// recompiler's own instruction semantics, its call protocol and loop checkpoints), in the function table while
// SOTC_LIFTD1 is on (sotc_decomp::install; an A/B window swaps them back).
// Regenerate with the tool; see docs/decomp.md "Decomp D1".
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include "runtime/ps2_memory.h"
#include <ps2_recompiled_functions.h>

#include <cmath>
#include <cstring>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wparentheses"

#ifndef LIFT_D1_HELPERS
#define LIFT_D1_HELPERS
namespace lift_d1
{
    inline uint64_t sx64(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline uint32_t bits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float flt(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
}
#define LIFT_SET_FACC(x) (facc = (x))
#endif



namespace lift_d1
{
'''

FOOTER = '''}

#pragma GCC diagnostic pop
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-o', '--out', required=True, help='output .cpp ({n} for one file per --chunk functions)')
    ap.add_argument('--switch', default='kLiftD1', help=argparse.SUPPRESS)
    ap.add_argument('--chunk', type=int, default=60)
    ap.add_argument('--refused', help='write the refused functions and why')
    ap.add_argument('--leaf', action='store_true', help='only functions that call nothing and never hand the context to the runtime')
    ap.add_argument('addrs', nargs='+')
    args = ap.parse_args()
    addrs = []
    for x in args.addrs:
        addrs.extend(open(x[1:]).read().split() if x.startswith('@') else [x])
    addrs = [int(x, 16) for x in addrs]
    # pass 1: which lift (names for direct calls between them)
    ok, refused = {}, []
    for a in addrs:
        try:
            lf = Lifter(a, {})
            lf.lift()
            if args.leaf:
                if any(n['kind'][0] in ('jal', 'jalr', 'tail') or (n['kind'][0] == 'jr' and not n['kind'][2]) for n in lf.nodes.values()):
                    raise Refuse('not a leaf (calls)')
                if any(n.get('opaque') for n in lf.nodes.values()):
                    raise Refuse('not a leaf (runtime call)')
            ok[a] = lf.g.name
        except Refuse as e:
            refused.append((a, str(e)))
        except Exception as e:  # a parser bug: refuse, report
            refused.append((a, f'error {type(e).__name__}: {e}'))
    bodies, regs = [], []
    for a in sorted(ok):
        lf = Lifter(a, ok)
        bodies.append(lf.text(args.switch))
        regs.append(f'SOTC_DECOMP_LIFT_FUNCTION(0x{a:08X}u, {lf.g.name}, lift_d1::{lf.g.name}, {lf.g.cname});')
    if '{n}' in args.out:
        groups = [list(range(i, min(i + args.chunk, len(bodies)))) for i in range(0, len(bodies), args.chunk)]
    else:
        groups = [list(range(len(bodies)))]
    for n, grp in enumerate(groups):
        # the batch's functions: called directly across files, named in their own off<>
        decl = ''.join(f'    void {ok[a]}(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);\n' for a in sorted(ok))
        text = HEADER.replace('SWITCH_NAME', args.switch) + decl + '\n'
        text += '\n\n'.join(bodies[i] for i in grp) + '\n' + FOOTER + '\n' + '\n'.join(regs[i] for i in grp) + '\n'
        open(args.out.replace('{n}', f'{n:02d}'), 'w').write(text)
    if args.refused:
        with open(args.refused, 'w') as f:
            for a, why in refused:
                f.write(f'{a:x} {why}\n')
    print(f'{len(ok)} lifted, {len(refused)} refused, {len(groups)} files', file=sys.stderr)


if __name__ == '__main__':
    main()
