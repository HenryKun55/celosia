#!/usr/bin/env python3
"""lift126.py: the camera's and the characters' guest functions as plain host C++ (round 126, `cam126` / `ctl126`).

A port, not an emulator: each listed guest function's MIPS (its words, from the generated code's listing in
build/recomp/game/output) becomes a C++ function whose registers are host locals, whose floats are plain host
floats (add / sub / mul as the A9 does them, VU0 macro ops as plain SSE / NEON lanes: no PS2 rounding or clamps;
div / sqrt / cvt keep the runtime's clamps, which only matter at 0 / huge values), and which writes the context only
at calls and at its return (every register it used, so callee-saved registers come back as the original's epilogue
restores them, and a call that sleeps or switches threads leaves the context exactly as the original would: the
rest then runs in the recompiled function from the return label). Guest memory through the runtime's READ / WRITE
macros; calls (jal, jalr, tail j) through sotc_decomp::callMaySleep (natives, switches and checks still apply; a
sub-thread sleep returns at once); jump tables from the generated code's case lists; the relocated words
(_dynInsn, dynamic jal targets) read from guest RAM as the generated code does. SOTC_LIFT126_EXACT=1 at compile
time: the PS2's FPU add / sub / mul (for the difftest).

Refused (the function stays recompiled): VU0 integer / flag ops and control registers other than Q, MMI,
ldl / ldr / lwl / lwr, syscalls, FPU accumulator ops, any word it does not know, code that runs off its end.
Usage: lift126.py out.cpp switch-name addr|@file... (addresses hex; out may hold {n}: LIFT_CHUNK functions a file).
"""
import glob, os, re, sys

OUT = '/Users/flaviohenrique/Documents/workspace/games/sotc/build/recomp/game/output'
GPR = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
       's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']


class Refuse(Exception):
    pass


def load(addr):
    f = glob.glob(f'{OUT}/*_0x{addr:x}.cpp')
    if not f:
        raise Refuse('no generated file')
    src = open(f[0], errors='replace').read()
    name = os.path.basename(f[0])[:-4]
    m = re.search(r'// Address: 0x([0-9a-f]+) - 0x([0-9a-f]+)', src)
    if not m:
        raise Refuse('no address range (a stub?)')
    start, end = int(m.group(1), 16), int(m.group(2), 16)
    words = {}
    for mm in re.finditer(r'//\s*0x([0-9a-f]+):\s*0x([0-9a-f]+)\s', src):
        a = int(mm.group(1), 16)
        if start <= a < end:
            words[a] = int(mm.group(2), 16)
    dyn = set(int(x, 16) for x in re.findall(r'_dynInsn = READ32\(0x([0-9A-Fa-f]+)u\)', src))
    dynj = set(int(x, 16) for x in re.findall(r'READ32\(0x([0-9A-Fa-f]+)u\) & 0x3FFFFFFu', src))
    # jump tables: each jr's case list in the generated code (its targets inside the function)
    jrcases = {}
    for mm in re.finditer(r'//\s*0x([0-9a-f]+):\s*0x[0-9a-f]+\s+jr\s+\$(?!ra)\w+\s*\n(.*?)PS2_JUMP_GUEST', src, re.S):
        jrcases[int(mm.group(1), 16)] = [int(x, 16) for x in re.findall(r'case 0x([0-9A-Fa-f]+)u: goto', mm.group(2))]
    return name, start, end, words, dyn, dynj, jrcases


def sx16(v):
    return v - 0x10000 if v & 0x8000 else v


class Lifter:
    def __init__(self, addr):
        self.addr = addr
        self.name, self.start, self.end, self.words, self.dyn, self.dynj, self.jrcases = load(addr)
        self.gused = set()
        self.fused = set()
        self.lohi = False
        self.labels = set()
        self.vused = set()
        self.vacc = False
        self.vq = False

    # ---- VU0 macro mode (plain floats through the SSE intrinsics: sse2neon on the Vita) ----
    def v(self, r):
        if r == 0:
            return 'lift126::kVf0'
        self.vused.add(r)
        return f'v{r}'

    def vset(self, r, dest, expr):
        if r == 0:
            return f'(void)({expr});'
        self.vused.add(r)
        if dest == 15:
            return f'v{r} = {expr};'
        return f'v{r} = lift126::blend<{dest}>(v{r}, {expr});'

    def vu(self, a, w):
        op = w >> 26
        rs = (w >> 21) & 31
        base, rt = (w >> 21) & 31, (w >> 16) & 31
        if op == 0x36:  # lqc2
            ea = f'(uint32_t)((uint32_t){self.g(base)} + (uint32_t){self.imm(a, w)})'
            return self.vset(rt, 15, f'lift126::ldq(rdram, ctx, runtime, {ea})')
        if op == 0x3E:  # sqc2
            ea = f'(uint32_t)((uint32_t){self.g(base)} + (uint32_t){self.imm(a, w)})'
            return f'lift126::stq(rdram, ctx, runtime, {ea}, {self.v(rt)});'
        if op != 0x12:
            raise Refuse(f'op {op:x} at {a:x}')
        fdr = (w >> 11) & 31
        if rs == 1:  # qmfc2 rt, fd
            if rt == 0:
                return ''
            self.g(rt)
            return f'{{ uint64_t lh[2]; std::memcpy(lh, &{self.v(fdr)}, 16); g{rt} = (int64_t)lh[0]; h{rt} = lh[1]; }}'
        if rs == 5:  # qmtc2 rt, fd
            hv = '0ULL' if rt == 0 else f'h{rt}'
            gv = self.g(rt)
            return self.vset(fdr, 15, f'lift126::fromBits((uint64_t){gv}, {hv})')
        if rs == 2 and fdr == 22:  # cfc2 rt, $vi22: Q's bits
            self.vq = True
            return self.setg(rt, '(int64_t)(int32_t)lift126::bits(q)')
        if rs < 0x10:
            raise Refuse(f'cop2 rs {rs:x} at {a:x}')
        dest = (w >> 21) & 15
        ft, fs, fd = (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31
        fn = w & 63
        bc = w & 3
        V = self.v
        B = lambda r: f'lift126::bcast<{bc}>({V(r)})'

        def acc_set(expr):
            self.vacc = True
            if dest == 15:
                return f'acc = {expr};'
            return f'acc = lift126::blend<{dest}>(acc, {expr});'

        def accv():
            self.vacc = True
            return 'acc'

        def qv():
            self.vq = True
            return '_mm_set1_ps(q)'
        S = self.vset
        if fn < 0x3C:
            if 0x00 <= fn <= 0x03: return S(fd, dest, f'_mm_add_ps({V(fs)}, {B(ft)})')
            if 0x04 <= fn <= 0x07: return S(fd, dest, f'_mm_sub_ps({V(fs)}, {B(ft)})')
            if 0x08 <= fn <= 0x0B: return S(fd, dest, f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {B(ft)}))')
            if 0x0C <= fn <= 0x0F: return S(fd, dest, f'_mm_sub_ps({accv()}, _mm_mul_ps({V(fs)}, {B(ft)}))')
            if 0x10 <= fn <= 0x13: return S(fd, dest, f'_mm_max_ps({V(fs)}, {B(ft)})')
            if 0x14 <= fn <= 0x17: return S(fd, dest, f'_mm_min_ps({V(fs)}, {B(ft)})')
            if 0x18 <= fn <= 0x1B: return S(fd, dest, f'_mm_mul_ps({V(fs)}, {B(ft)})')
            if fn == 0x1C: return S(fd, dest, f'_mm_mul_ps({V(fs)}, {qv()})')
            if fn == 0x20: return S(fd, dest, f'_mm_add_ps({V(fs)}, {qv()})')
            if fn == 0x21: return S(fd, dest, f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {qv()}))')
            if fn == 0x24: return S(fd, dest, f'_mm_sub_ps({V(fs)}, {qv()})')
            if fn == 0x25: return S(fd, dest, f'_mm_sub_ps({accv()}, _mm_mul_ps({V(fs)}, {qv()}))')
            if fn == 0x28: return S(fd, dest, f'_mm_add_ps({V(fs)}, {V(ft)})')
            if fn == 0x29: return S(fd, dest, f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {V(ft)}))')
            if fn == 0x2A: return S(fd, dest, f'_mm_mul_ps({V(fs)}, {V(ft)})')
            if fn == 0x2B: return S(fd, dest, f'_mm_max_ps({V(fs)}, {V(ft)})')
            if fn == 0x2C: return S(fd, dest, f'_mm_sub_ps({V(fs)}, {V(ft)})')
            if fn == 0x2D: return S(fd, dest, f'_mm_sub_ps({accv()}, _mm_mul_ps({V(fs)}, {V(ft)}))')
            if fn == 0x2E: return S(fd, dest, f'_mm_sub_ps({accv()}, lift126::outer({V(fs)}, {V(ft)}))')
            if fn == 0x2F: return S(fd, dest, f'_mm_min_ps({V(fs)}, {V(ft)})')
            raise Refuse(f'vu op {fn:x} at {a:x}')
        f2 = (w & 3) | ((w >> 4) & 0x7C)
        if 0x00 <= f2 <= 0x03: return acc_set(f'_mm_add_ps({V(fs)}, {B(ft)})')
        if 0x04 <= f2 <= 0x07: return acc_set(f'_mm_sub_ps({V(fs)}, {B(ft)})')
        if 0x08 <= f2 <= 0x0B: return acc_set(f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {B(ft)}))')
        if 0x0C <= f2 <= 0x0F: return acc_set(f'_mm_sub_ps({accv()}, _mm_mul_ps({V(fs)}, {B(ft)}))')
        if f2 in (0x10, 0x11, 0x12, 0x13):
            sc = {0x10: '1.0f', 0x11: '0.0625f', 0x12: '0.000244140625f', 0x13: '0.000030517578125f'}[f2]
            return S(ft, dest, f'_mm_mul_ps(_mm_cvtepi32_ps(_mm_castps_si128({V(fs)})), _mm_set1_ps({sc}))')
        if f2 in (0x14, 0x15, 0x16, 0x17):
            sc = {0x14: '1.0f', 0x15: '16.0f', 0x16: '4096.0f', 0x17: '32768.0f'}[f2]
            return S(ft, dest, f'lift126::ftoi({V(fs)}, {sc})')
        if 0x18 <= f2 <= 0x1B: return acc_set(f'_mm_mul_ps({V(fs)}, {B(ft)})')
        if f2 == 0x1C: return acc_set(f'_mm_mul_ps({V(fs)}, {qv()})')
        if f2 == 0x1D: return S(ft, dest, f'lift126::vabs({V(fs)})')
        if f2 == 0x20: return acc_set(f'_mm_add_ps({V(fs)}, {qv()})')
        if f2 == 0x21: return acc_set(f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {qv()}))')
        if f2 == 0x28: return acc_set(f'_mm_add_ps({V(fs)}, {V(ft)})')
        if f2 == 0x29: return acc_set(f'_mm_add_ps({accv()}, _mm_mul_ps({V(fs)}, {V(ft)}))')
        if f2 == 0x2A: return acc_set(f'_mm_mul_ps({V(fs)}, {V(ft)})')
        if f2 == 0x2C: return acc_set(f'_mm_sub_ps({V(fs)}, {V(ft)})')
        if f2 == 0x2D: return acc_set(f'_mm_sub_ps({accv()}, _mm_mul_ps({V(fs)}, {V(ft)}))')
        if f2 == 0x2E: return acc_set(f'lift126::outer({V(fs)}, {V(ft)})')
        if f2 == 0x2F: return ''
        if f2 == 0x30: return S(ft, dest, V(fs))
        if f2 == 0x31: return S(ft, dest, f'lift126::mr32({V(fs)})')
        fsf, ftf = (w >> 21) & 3, (w >> 23) & 3
        if f2 == 0x38:
            self.vq = True
            return f'q = lift126::vdiv(lift126::lane<{fsf}>({V(fs)}), lift126::lane<{ftf}>({V(ft)}));'
        if f2 == 0x39:
            self.vq = True
            return f'q = std::sqrt(std::fabs(lift126::lane<{ftf}>({V(ft)})));'
        if f2 == 0x3A:
            self.vq = True
            return f'q = lift126::vdiv(lift126::lane<{fsf}>({V(fs)}), std::sqrt(std::fabs(lift126::lane<{ftf}>({V(ft)}))));'
        if f2 == 0x3B: return ''
        raise Refuse(f'vu special {f2:x} at {a:x}')

    def g(self, r):
        if r == 0:
            return '0LL'
        self.gused.add(r)
        return f'g{r}'

    def f(self, r):
        self.fused.add(r)
        return f'f{r}'

    def imm(self, a, w, signed=True):
        if a in self.dyn:
            return f'((int64_t)(int16_t)(READ32(0x{a:X}u) & 0xFFFFu))' if signed else f'((int64_t)(READ32(0x{a:X}u) & 0xFFFFu))'
        v = w & 0xFFFF
        return f'{sx16(v)}LL' if signed else f'{v}LL'

    def setg(self, r, expr):
        if r == 0:
            return f'(void)({expr});'
        return f'{self.g(r)} = {expr};'

    # one non-branch instruction -> C statement
    def stmt(self, a, w):
        op = w >> 26
        rs, rt, rd, sa, fn = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31, w & 63
        G, S = self.g, self.setg
        s32 = lambda e: f'(int64_t)(int32_t)(uint32_t)({e})'
        if w == 0:
            return ''
        if op == 0:
            if fn == 0x00: return S(rd, s32(f'(uint32_t){G(rt)} << {sa}'))
            if fn == 0x02: return S(rd, s32(f'(uint32_t){G(rt)} >> {sa}'))
            if fn == 0x03: return S(rd, s32(f'(int32_t){G(rt)} >> {sa}'))
            if fn == 0x04: return S(rd, s32(f'(uint32_t){G(rt)} << ({G(rs)} & 31)'))
            if fn == 0x06: return S(rd, s32(f'(uint32_t){G(rt)} >> ({G(rs)} & 31)'))
            if fn == 0x07: return S(rd, s32(f'(int32_t){G(rt)} >> ({G(rs)} & 31)'))
            if fn == 0x0A: return f'if ({G(rt)} == 0) {S(rd, G(rs))}'
            if fn == 0x0B: return f'if ({G(rt)} != 0) {S(rd, G(rs))}'
            if fn == 0x10: self.lohi = True; return S(rd, 'hi')
            if fn == 0x12: self.lohi = True; return S(rd, 'lo')
            if fn == 0x18:
                self.lohi = True
                return f'{{ const int64_t p = (int64_t)(int32_t){G(rs)} * (int64_t)(int32_t){G(rt)}; lo = (int64_t)(int32_t)p; hi = (int64_t)(int32_t)(p >> 32); {S(rd, "lo")} }}'
            if fn == 0x19:
                self.lohi = True
                return f'{{ const uint64_t p = (uint64_t)(uint32_t){G(rs)} * (uint64_t)(uint32_t){G(rt)}; lo = (int64_t)(int32_t)(uint32_t)p; hi = (int64_t)(int32_t)(uint32_t)(p >> 32); {S(rd, "lo")} }}'
            if fn == 0x1A:  # div (by zero: lo = rs < 0 ? 1 : -1, hi = rs, as the EE)
                self.lohi = True
                return (f'{{ const int32_t n = (int32_t){G(rs)}, d = (int32_t){G(rt)}; '
                        f'if (d == 0) {{ lo = n < 0 ? 1 : -1; hi = n; }} else if (n == INT32_MIN && d == -1) {{ lo = n; hi = 0; }} '
                        f'else {{ lo = n / d; hi = n % d; }} }}')
            if fn == 0x1B:  # divu
                self.lohi = True
                return (f'{{ const uint32_t n = (uint32_t){G(rs)}, d = (uint32_t){G(rt)}; '
                        f'if (d == 0) {{ lo = -1; hi = (int32_t)n; }} else {{ lo = (int32_t)(n / d); hi = (int32_t)(n % d); }} }}')
            if fn == 0x0D: return ''  # break (the divide-by-zero trap after a div)
            if fn in (0x20, 0x21): return S(rd, s32(f'(uint32_t){G(rs)} + (uint32_t){G(rt)}'))
            if fn in (0x22, 0x23): return S(rd, s32(f'(uint32_t){G(rs)} - (uint32_t){G(rt)}'))
            if fn == 0x24: return S(rd, f'{G(rs)} & {G(rt)}')
            if fn == 0x25: return S(rd, f'{G(rs)} | {G(rt)}')
            if fn == 0x26: return S(rd, f'{G(rs)} ^ {G(rt)}')
            if fn == 0x27: return S(rd, f'~({G(rs)} | {G(rt)})')
            if fn == 0x2A: return S(rd, f'(int64_t)({G(rs)} < {G(rt)})')
            if fn == 0x2B: return S(rd, f'(int64_t)((uint64_t){G(rs)} < (uint64_t){G(rt)})')
            if fn in (0x2C, 0x2D): return S(rd, f'(int64_t)((uint64_t){G(rs)} + (uint64_t){G(rt)})')
            if fn in (0x2E, 0x2F): return S(rd, f'(int64_t)((uint64_t){G(rs)} - (uint64_t){G(rt)})')
            if fn == 0x38: return S(rd, f'(int64_t)((uint64_t){G(rt)} << {sa})')
            if fn == 0x3A: return S(rd, f'(int64_t)((uint64_t){G(rt)} >> {sa})')
            if fn == 0x3B: return S(rd, f'{G(rt)} >> {sa}')
            if fn == 0x3C: return S(rd, f'(int64_t)((uint64_t){G(rt)} << {sa + 32})')
            if fn == 0x3E: return S(rd, f'(int64_t)((uint64_t){G(rt)} >> {sa + 32})')
            if fn == 0x3F: return S(rd, f'{G(rt)} >> {sa + 32}')
            if fn == 0x0F: return ''  # sync
            raise Refuse(f'special {fn:x} at {a:x}')
        if op in (8, 9): return S(rt, s32(f'(uint32_t){G(rs)} + (uint32_t){self.imm(a, w)}'))
        if op in (0x18, 0x19): return S(rt, f'(int64_t)((uint64_t){G(rs)} + (uint64_t){self.imm(a, w)})')
        if op == 0xA: return S(rt, f'(int64_t)({G(rs)} < {self.imm(a, w)})')
        if op == 0xB: return S(rt, f'(int64_t)((uint64_t){G(rs)} < (uint64_t){self.imm(a, w)})')
        if op == 0xC: return S(rt, f'{G(rs)} & {self.imm(a, w, False)}')
        if op == 0xD: return S(rt, f'{G(rs)} | {self.imm(a, w, False)}')
        if op == 0xE: return S(rt, f'{G(rs)} ^ {self.imm(a, w, False)}')
        if op == 0xF: return S(rt, s32(f'(uint32_t){self.imm(a, w, False)} << 16'))
        ea = f'(uint32_t)((uint32_t){G(rs)} + (uint32_t){self.imm(a, w)})'
        if op == 0x20: return S(rt, f'(int64_t)(int8_t)READ8({ea})')
        if op == 0x24: return S(rt, f'(int64_t)READ8({ea})')
        if op == 0x21: return S(rt, f'(int64_t)(int16_t)READ16({ea})')
        if op == 0x25: return S(rt, f'(int64_t)READ16({ea})')
        if op == 0x23: return S(rt, f'(int64_t)(int32_t)READ32({ea})')
        if op == 0x27: return S(rt, f'(int64_t)READ32({ea})')
        if op == 0x37: return S(rt, f'(int64_t)READ64({ea})')
        if op == 0x28: return f'WRITE8({ea}, (uint8_t){G(rt)});'
        if op == 0x29: return f'WRITE16({ea}, (uint16_t){G(rt)});'
        if op == 0x2B: return f'WRITE32({ea}, (uint32_t){G(rt)});'
        if op == 0x3F: return f'WRITE64({ea}, (uint64_t){G(rt)});'
        if op == 0x1E:  # lq
            if rt == 0: return ''
            G(rt)
            return f'{{ const uint32_t e = {ea} & ~0xFu; g{rt} = (int64_t)READ64(e); h{rt} = READ64(e + 8u); }}'
        if op == 0x1F:  # sq
            hv = '0ULL' if rt == 0 else f'h{rt}'
            if rt: G(rt)
            return f'{{ const uint32_t e = {ea} & ~0xFu; WRITE64(e, (uint64_t){G(rt)}); WRITE64(e + 8u, {hv}); }}'
        if op == 0x31: return f'{{ const uint32_t b = READ32({ea}); std::memcpy(&{self.f(rt)}, &b, 4); }}'
        if op == 0x39: return f'{{ uint32_t b; std::memcpy(&b, &{self.f(rt)}, 4); WRITE32({ea}, b); }}'
        if op == 0x11:
            fs, ft, fd = rd, rt, sa
            if rs == 0: return S(rt, f'(int64_t)(int32_t)lift126::bits({self.f(fs)})')
            if rs == 4: return f'{self.f(fs)} = lift126::flt((uint32_t){G(rt)});'
            if rs == 0x10:
                F = self.f
                if fn == 0: return f'{F(fd)} = LIFT_ADD({F(fs)}, {F(ft)});'
                if fn == 1: return f'{F(fd)} = LIFT_SUB({F(fs)}, {F(ft)});'
                if fn == 2: return f'{F(fd)} = LIFT_MUL({F(fs)}, {F(ft)});'
                if fn == 3: return f'{F(fd)} = FPU_DIV_S({F(fs)}, {F(ft)});'
                if fn == 4: return f'{F(fd)} = FPU_SQRT_S({F(ft)});'
                if fn == 5: return f'{F(fd)} = std::fabs({F(fs)});'
                if fn == 6: return f'{F(fd)} = {F(fs)};'
                if fn == 7: return f'{F(fd)} = -{F(fs)};'
                if fn == 0x24: return f'{{ const int32_t t = FPU_CVT_W_S({F(fs)}); std::memcpy(&{F(fd)}, &t, 4); }}'
                if fn == 0x28: return f'{F(fd)} = {F(fs)} > {F(ft)} ? {F(fs)} : {F(ft)};'  # max.s
                if fn == 0x29: return f'{F(fd)} = {F(fs)} < {F(ft)} ? {F(fs)} : {F(ft)};'  # min.s
                if fn == 0x30: return 'cc = false;'
                if fn == 0x32: return f'cc = {F(fs)} == {F(ft)};'
                if fn == 0x34: return f'cc = {F(fs)} < {F(ft)};'
                if fn == 0x36: return f'cc = {F(fs)} <= {F(ft)};'
                raise Refuse(f'cop1.s {fn:x} at {a:x}')
            if rs == 0x14 and fn == 0x20:
                return f'{{ int32_t t; std::memcpy(&t, &{self.f(fs)}, 4); {self.f(fd)} = (float)t; }}'
            raise Refuse(f'cop1 rs {rs:x} at {a:x}')
        if op in (0x12, 0x36, 0x3E):
            return self.vu(a, w)
        raise Refuse(f'op {op:x} at {a:x} ({w:08x})')

    def branch(self, a, w):
        """(condition C expr or None if not a branch, target, likely, kind)"""
        op = w >> 26
        rs, rt = (w >> 21) & 31, (w >> 16) & 31
        tgt = (a + 4 + (sx16(w & 0xFFFF) << 2)) & 0xFFFFFFFF
        G = self.g
        if op in (4, 0x14): return (f'{G(rs)} == {G(rt)}', tgt, op == 0x14)
        if op in (5, 0x15): return (f'{G(rs)} != {G(rt)}', tgt, op == 0x15)
        if op in (6, 0x16): return (f'{G(rs)} <= 0', tgt, op == 0x16)
        if op in (7, 0x17): return (f'{G(rs)} > 0', tgt, op == 0x17)
        if op == 1 and rt in (0, 2): return (f'{G(rs)} < 0', tgt, rt == 2)
        if op == 1 and rt in (1, 3): return (f'{G(rs)} >= 0', tgt, rt == 3)
        if op == 0x11 and rs == 8:
            return ('cc' if rt & 1 else '!cc', tgt, bool(rt & 2))
        return None

    def lift(self):
        addrs = sorted(self.words)
        if not addrs or addrs[0] != self.start:
            raise Refuse('listing')
        body = []
        # the code must not run off its end into the next function (a shared tail)
        def ends(w):
            op = w >> 26
            return (op == 0 and (w & 63) == 8) or op == 2 or (op == 4 and ((w >> 16) & 0x3FF) == 0)
        nz = [a for a in addrs if self.words[a] != 0]
        if not nz or not (ends(self.words[nz[-1]]) or ends(self.words.get(nz[-1] - 4, 0))):
            raise Refuse('runs off its end')
        # pass 1: labels
        for a in addrs:
            w = self.words[a]
            if (w >> 26) == 0 and (w & 63) == 8 and ((w >> 21) & 31) != 31:
                ts = self.jrcases.get(a)
                if not ts or any(not (self.start <= t < self.end) for t in ts):
                    raise Refuse(f'jr {(w >> 21) & 31} at {a:x}')
                for t in ts:
                    pw = self.words.get(t - 4, 0)
                    if self.branch(t - 4, pw) or (pw >> 26) in (2, 3) or ((pw >> 26) == 0 and (pw & 63) in (8, 9)):
                        raise Refuse(f'jump table target {t:x} in a delay slot')
                self.labels.update(ts)
            b = self.branch(a, w)
            if b:
                if not (self.start <= b[1] < self.end):
                    raise Refuse(f'branch out at {a:x}')
                self.labels.add(b[1])
            op = w >> 26
            if op == 2:
                pass
        skip = set()
        for a in addrs:
            if a in skip:
                continue
            w = self.words[a]
            op, fn = w >> 26, w & 63
            line = []
            if a in self.labels:
                body.append(f'L_{a:x}:')
            ds = self.words.get(a + 4, 0)
            b = self.branch(a, w)
            if b:
                cond, tgt, likely = b
                dstmt = self.stmt(a + 4, ds)
                if self.branch(a + 4, ds) or (ds >> 26) in (2, 3) or ((ds >> 26) == 0 and (ds & 63) in (8, 9)):
                    raise Refuse('branch in delay slot')
                if likely:
                    body.append(f'    if ({cond}) {{ {dstmt} goto L_{tgt:x}; }}')
                else:
                    body.append(f'    {{ const bool c = {cond}; {dstmt} if (c) goto L_{tgt:x}; }}')
                skip.add(a + 4)
                if likely and a + 4 in self.labels:
                    raise Refuse('label on a likely delay slot')
                if not likely and a + 4 in self.labels:
                    raise Refuse('label on a delay slot')
                continue
            if op == 0 and fn == 8:  # jr
                rs = (w >> 21) & 31
                if rs != 31:
                    cases = ' '.join(f'case 0x{t:X}u: goto L_{t:x};' for t in self.jrcases[a])
                    body.append(f'    {{ const uint32_t jt = (uint32_t){self.g(rs)}; {self.stmt(a + 4, ds)} switch (jt) {{ {cases} default: SYNC_OUT(); ctx->pc = jt; PS2_JUMP_GUEST(jt); return; }} }}')
                    skip.add(a + 4)
                    continue
                body.append(f'    {self.stmt(a + 4, ds)}')
                body.append('    goto L_ret;')
                skip.add(a + 4)
                continue
            if op == 0 and fn == 9:  # jalr rd, rs: an indirect call (callbacks) through callGuest
                rs, rd = (w >> 21) & 31, (w >> 11) & 31
                body.append(f'    {{ const uint32_t jt = (uint32_t){self.g(rs)}; {self.setg(rd, f"0x{a + 8:X}LL")} {self.stmt(a + 4, ds)} SYNC_OUT(); if (!sotc_decomp::callMaySleep(rdram, ctx, runtime, jt, 0x{a + 8:X}u, (uint32_t)g29)) return; SYNC_IN(); }}')
                skip.add(a + 4)
                continue
            if op == 0 and fn == 0xC:
                raise Refuse(f'syscall at {a:x}')
            if op in (2, 3):
                tgt = ((a + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                texpr = f'(((READ32(0x{a:X}u) & 0x3FFFFFFu) << 2))' if a in self.dynj else f'0x{tgt:X}u'
                if op == 3:
                    body.append(f'    {self.setg(31, f"0x{a + 8:X}LL")}')
                body.append(f'    {self.stmt(a + 4, ds)}')
                if op == 3:
                    body.append(f'    SYNC_OUT(); if (!sotc_decomp::callMaySleep(rdram, ctx, runtime, {texpr}, 0x{a + 8:X}u, (uint32_t)g29)) return; SYNC_IN();')
                else:
                    if self.start <= tgt < self.end and a not in self.dynj:
                        body.append(f'    goto L_{tgt:x};')
                        self.labels.add(tgt)
                    else:
                        body.append(f'    SYNC_OUT(); (void)sotc_decomp::callMaySleep(rdram, ctx, runtime, {texpr}, (uint32_t)g31, (uint32_t)g29); return;')
                skip.add(a + 4)
                continue
            st = self.stmt(a, w)
            if st:
                body.append('    ' + st)
        body.append('    goto L_ret;')
        return body

    def emit(self, switch):
        body = self.lift()
        # labels referenced by forward j inside (added late) must exist
        text = '\n'.join(body)
        for t in list(self.labels):
            if f'L_{t:x}:' not in text:
                raise Refuse(f'label {t:x} not at an instruction')
        self.gused |= {29, 31}
        gs = sorted(self.gused)
        fs = sorted(self.fused)
        hs = [r for r in gs if re.search(rf'\bh{r}\b', text)]
        out = [f'// {self.name} ({self.start:#x}-{self.end:#x}), lifted by tools/lift126.py']
        out.append(f'void {self.name}_lift(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)')
        out.append('{')
        out.append(f'    if (!sotc_decomp::switchOn(sotc_decomp::{switch}))')
        out.append(f'        return sotc_decomp::recompiled(0x{self.start:X}u)(rdram, ctx, runtime);')
        if gs:
            out.append('    int64_t ' + ', '.join(f'g{r} = GPR_S64(ctx, {r})' for r in gs) + ';')
            if hs:
                out.append('    uint64_t ' + ', '.join(f'h{r} = lift126::hi(ctx, {r})' for r in hs) + ';')
        if fs:
            out.append('    float ' + ', '.join(f'f{r} = ctx->f[{r}]' for r in fs) + ';')
        vs = sorted(self.vused)
        if vs:
            out.append('    __m128 ' + ', '.join(f'v{r} = ctx->vu0_vf[{r}]' for r in vs) + ';')
        if self.vacc:
            out.append('    __m128 acc = ctx->vu0_acc;')
        if self.vq:
            out.append('    float q = ctx->vu0_q;')
        out.append('    bool cc = (ctx->fcr31 & 0x800000u) != 0u;')
        if self.lohi:
            out.append('    int64_t lo = 0, hi = 0;')
        # only what the body writes goes back to the context (the rest still holds the values it was read from)
        gw = set(int(x) for x in re.findall(r'\bg(\d+) = ', text))
        hw = set(int(x) for x in re.findall(r'\bh(\d+) = ', text))
        fw = set(int(x) for x in re.findall(r'\bf(\d+) = ', text)) | set(int(x) for x in re.findall(r'std::memcpy\(&f(\d+),', text))
        vw = set(int(x) for x in re.findall(r'\bv(\d+) = ', text))
        accw = re.search(r'\bacc = ', text) is not None
        qw = re.search(r'\bq = ', text) is not None
        ccw = re.search(r'\bcc = ', text) is not None
        sync_out = ' '.join(f'SET_GPR_S64(ctx, {r}, g{r});' for r in gs if r in gw) + ' ' + ' '.join(f'lift126::setHi(ctx, {r}, h{r});' for r in hs if r in hw) + ' ' + \
            ' '.join(f'ctx->f[{r}] = f{r};' for r in fs if r in fw) + \
            ' ' + ' '.join(f'ctx->vu0_vf[{r}] = v{r};' for r in vs if r in vw) + (' ctx->vu0_acc = acc;' if self.vacc and accw else '') + \
            (' ctx->vu0_q = q;' if self.vq and qw else '') + (' ctx->fcr31 = cc ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);' if ccw else '')
        sync_in = ' '.join(f'g{r} = GPR_S64(ctx, {r});' for r in (2, 3) if r in gs) + ' ' + ' '.join(f'f{r} = ctx->f[{r}];' for r in (0, 1) if r in fs)
        out.append(f'#define SYNC_OUT() do {{ {sync_out} }} while (0)')
        out.append(f'#define SYNC_IN() do {{ {sync_in} }} while (0)')
        out.extend(body)
        out.append('L_ret:')
        out.append('    SYNC_OUT();')
        out.append('    ctx->pc = (uint32_t)g31;')
        out.append('#undef SYNC_OUT')
        out.append('#undef SYNC_IN')
        out.append('}')
        out.append(f'SOTC_DECOMP_FUNCTION(0x{self.start:08X}u, {self.name}_lift126, {self.name}_lift);')
        return '\n'.join(out)


HEADER = '''// Generated by tools/lift126.py (round 126): the camera's small integer / FPU functions as plain host code.
// A port, not an emulator: registers are host locals, add / sub / mul are plain floats; see the tool's notes.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <climits>
#include <cstring>

namespace lift126
{
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
    inline uint64_t hi(const R5900Context *ctx, int r)
    {
        uint64_t v;
        std::memcpy(&v, reinterpret_cast<const uint8_t *>(&ctx->r[r]) + 8, 8);
        return v;
    }
    inline void setHi(R5900Context *ctx, int r, uint64_t v) { std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[r]) + 8, &v, 8); }
    // VU0 macro mode in plain floats (x, y, z, w = lanes 0..3; dest bits x 8, y 4, z 2, w 1).
    static const __m128 kVf0 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
#if defined(__ARM_NEON)
    // On the Vita (__m128 is NEON's float32x4_t through sse2neon): the lane moves as single NEON ops.
    template <int D>
    inline __m128 blend(__m128 old, __m128 nw)
    {
        const uint32x4_t m = {(D & 8) ? ~0u : 0u, (D & 4) ? ~0u : 0u, (D & 2) ? ~0u : 0u, (D & 1) ? ~0u : 0u};
        return vbslq_f32(m, nw, old);
    }
    template <int L>
    inline __m128 bcast(__m128 v) { return L < 2 ? vdupq_lane_f32(vget_low_f32(v), L & 1) : vdupq_lane_f32(vget_high_f32(v), L & 1); }
    template <int L>
    inline float lane(__m128 v) { return vgetq_lane_f32(v, L); }
    // (y, z, x, w) and (z, x, y, w) of a vector
    inline float32x4_t yzxw(float32x4_t v)
    {
        const float32x4_t r = vextq_f32(v, v, 1); // y z w x
        return vsetq_lane_f32(vgetq_lane_f32(v, 3), vsetq_lane_f32(vgetq_lane_f32(v, 0), r, 2), 3);
    }
    inline float32x4_t zxyw(float32x4_t v)
    {
        const float32x4_t r = vextq_f32(v, v, 2); // z w x y
        return vsetq_lane_f32(vgetq_lane_f32(v, 3), vsetq_lane_f32(vgetq_lane_f32(v, 0), vsetq_lane_f32(vgetq_lane_f32(v, 1), r, 2), 1), 3);
    }
    inline __m128 outer(__m128 s, __m128 t) { return vmulq_f32(yzxw(s), zxyw(t)); }
    inline __m128 mr32(__m128 v) { return vextq_f32(v, v, 1); }
#else
    template <int D>
    inline __m128 blend(__m128 old, __m128 nw)
    {
        const __m128 m = _mm_castsi128_ps(_mm_set_epi32((D & 1) ? -1 : 0, (D & 2) ? -1 : 0, (D & 4) ? -1 : 0, (D & 8) ? -1 : 0));
        return _mm_or_ps(_mm_and_ps(m, nw), _mm_andnot_ps(m, old));
    }
    template <int L>
    inline __m128 bcast(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    template <int L>
    inline float lane(__m128 v)
    {
        float f[4];
        std::memcpy(f, &v, 16);
        return f[L];
    }
    // (s.y t.z, s.z t.x, s.x t.y, s.w t.w): vopmula / vopmsub's product (their dest is xyz)
    inline __m128 outer(__m128 s, __m128 t) { return _mm_mul_ps(_mm_shuffle_ps(s, s, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 1, 0, 2))); }
    inline __m128 mr32(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1)); }
#endif
    inline __m128 vabs(__m128 v) { return _mm_and_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))); }
    inline float vdiv(float a, float b)
    {
        const float r = a / b;
        return std::isfinite(r) ? r : (std::signbit(a) != std::signbit(b) ? -3.4028235e38f : 3.4028235e38f);
    }
    inline __m128 ftoi(__m128 v, float scale)
    {
        float f[4];
        int32_t o[4];
        std::memcpy(f, &v, 16);
        for (int k = 0; k < 4; ++k)
        {
            const float x = f[k] * scale;
            o[k] = x >= 2147483520.0f ? 0x7FFFFFFF : (x <= -2147483648.0f ? static_cast<int32_t>(0x80000000u) : static_cast<int32_t>(x));
        }
        __m128 r;
        std::memcpy(&r, o, 16);
        return r;
    }
    inline __m128 fromBits(uint64_t lo, uint64_t hi)
    {
        const uint64_t b[2] = {lo, hi};
        __m128 r;
        std::memcpy(&r, b, 16);
        return r;
    }
    inline __m128 ldq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        const uint32_t e = a & ~0xFu;
        const uint64_t b[2] = {READ64(e), READ64(e + 8u)};
        __m128 r;
        std::memcpy(&r, b, 16);
        return r;
    }
    inline void stq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128 v)
    {
        uint64_t b[2];
        std::memcpy(b, &v, 16);
        const uint32_t e = a & ~0xFu;
        WRITE64(e, b[0]);
        WRITE64(e + 8u, b[1]);
    }
}

// SOTC_LIFT126_EXACT=1 (a check build only): the PS2's add / sub / mul as the generated code does them, so the
// difftest (SOTC_DIFFTEST=<name>_lift126) can prove the translation itself (branches, delay slots, memory) exact.
#ifndef SOTC_LIFT126_EXACT
#define SOTC_LIFT126_EXACT 0
#endif
#if SOTC_LIFT126_EXACT
#define LIFT_ADD(a, b) FPU_ADD_S(a, b)
#define LIFT_SUB(a, b) FPU_SUB_S(a, b)
#define LIFT_MUL(a, b) FPU_MUL_S(a, b)
#else
#define LIFT_ADD(a, b) ((a) + (b))
#define LIFT_SUB(a, b) ((a) - (b))
#define LIFT_MUL(a, b) ((a) * (b))
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-label"

'''


def main():
    """out may contain {n}: one file per LIFT_CHUNK (default 120) functions, numbered from 0."""
    out, switch = sys.argv[1], sys.argv[2]
    chunk = int(os.environ.get('LIFT_CHUNK', '120'))
    bodies = []
    args = []
    for x in sys.argv[3:]:
        args.extend(open(x[1:]).read().split() if x.startswith('@') else [x])
    for x in args:
        a = int(x, 16)
        try:
            bodies.append(Lifter(a).emit(switch))
        except Refuse as e:
            print(f'refused {a:x}: {e}', file=sys.stderr)
    groups = [bodies] if '{n}' not in out else [bodies[i:i + chunk] for i in range(0, len(bodies), chunk)]
    for n, g in enumerate(groups):
        open(out.replace('{n}', f'{n:02d}'), 'w').write(HEADER + '\n\n'.join(g) + '\n')
    print(f'{len(bodies)} lifted in {len(groups)} files', file=sys.stderr)


if __name__ == '__main__':
    main()
