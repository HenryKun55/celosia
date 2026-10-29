// Round 29: the VU0 operand normalisation and the FPU result clamp left out
// where the emitter can prove they change nothing (float_known.h).
//
// VU0. A VU0 add / sub / mul (PS2_VADD / PS2_VSUB / PS2_VMUL) normalises both
// operands and its result with ps2VuOperand (ARMv7 NEON: Inf / NaN ->
// +-FLT_MAX; elsewhere also denormals -> signed zero). That function is
// idempotent, so an operand lane that holds such a result needs nothing:
// PS2_VADD_K(ka, kb, a, b) skips the operand marked 1, the same bits on
// every host. Lanes are tracked one by one (4 bits a register):
// - the lanes a VU0 op's result writes (the instruction's dest mask) become
//   known; a blend / move / broadcast copies the lanes' state with the bits;
// - an op needs only the lanes its dest mask keeps (the ops are lane-wise;
//   the other lanes are computed and dropped), so an operand counts as known
//   when every kept lane (through the broadcast's selection) is;
// - vf0 is 0, 0, 0, 1 (no generated code writes it: counted below, the
//   report says if one does; the runtime keeps it so);
// - every other write to a vf register / ACC (loads, QMTC2, MAX / MINI,
//   VFTOI / ITOF, VRNEXT, ...) makes its lanes unknown;
// - everything is forgotten at labels and branches (the emitter's reset())
//   and at any guest memory access, call, jump, syscall or runtime helper
//   (round 30: loads no longer; PS2X_RECOMP_VU_LOADS=0 for round 29's rule)
//   (a store can start a VU0 microprogram through VIF0, a call or a thread
//   switch can bring any VU0 state).
//
// FPU. FPU_ADD_S / FPU_SUB_S / FPU_MUL_S clamp an Inf / NaN result to
// +-FLT_MAX. With both operands finite and the game thread's round toward
// zero (ps2SetEeFloatingPointMode, where all generated code runs) a sum or
// product overflows to +-FLT_MAX and never to Inf: the clamp is the identity
// and FPU_ADD_SF & co. leave it out. Finite: the result of an FPU add / sub
// / mul / div / sqrt / rsqrt (all clamped), CVT.S.W, a MOV / NEG / ABS of a
// finite register; FPRs are forgotten at labels, branches, calls, jumps,
// syscalls and runtime helpers (guest memory accesses never touch FPRs: the
// register cache relies on the same).
//
// Anything that does not parse as one of the emitter's own templates gets no
// rewrite (only forgetting).

#include "ps2recomp/float_known.h"

#include <atomic>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ps2recomp
{
    namespace
    {
        std::atomic<uint64_t> g_vuOps{0};
        std::atomic<uint64_t> g_vuOperands{0};
        std::atomic<uint64_t> g_vuKnown{0};
        std::atomic<uint64_t> g_vuRewritten{0};
        std::atomic<uint64_t> g_vf0Writes{0};
        std::atomic<uint64_t> g_fpuOps{0};
        std::atomic<uint64_t> g_fpuFinite{0};
        std::atomic<uint64_t> g_fpuConstants{0};
        std::atomic<uint64_t> g_joinsEdges{0};
        std::atomic<uint64_t> g_joinsFall{0};
        std::atomic<uint64_t> g_branchesKept{0};

        std::string trim(std::string_view s)
        {
            size_t b = 0, e = s.size();
            while (b < e && std::isspace(static_cast<unsigned char>(s[b])))
                ++b;
            while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
                --e;
            return std::string(s.substr(b, e - b));
        }

        bool isIdent(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

        bool hasToken(const std::string &code, const char *t)
        {
            return code.find(t) != std::string::npos;
        }

        // Splits at top-level separators (outside parentheses).
        std::vector<std::string> splitTop(std::string_view s, char sep)
        {
            std::vector<std::string> out;
            int depth = 0;
            size_t start = 0;
            for (size_t i = 0; i < s.size(); ++i)
            {
                const char c = s[i];
                if (c == '(' || c == '[' || c == '{')
                    ++depth;
                else if (c == ')' || c == ']' || c == '}')
                    --depth;
                else if (c == sep && depth == 0)
                {
                    out.push_back(trim(s.substr(start, i - start)));
                    start = i + 1;
                }
            }
            out.push_back(trim(s.substr(start)));
            return out;
        }

        // "name(args)" as a whole expression: the name and its top-level args.
        bool parseCall(const std::string &e, std::string &name, std::vector<std::string> &args)
        {
            const size_t open = e.find('(');
            if (open == std::string::npos || open == 0 || e.back() != ')')
                return false;
            for (size_t i = 0; i < open; ++i)
                if (!isIdent(e[i]))
                    return false;
            int depth = 0;
            for (size_t i = open; i < e.size(); ++i)
            {
                if (e[i] == '(')
                    ++depth;
                else if (e[i] == ')' && --depth == 0 && i + 1 != e.size())
                    return false; // "f(x) + g(y)"
            }
            name = e.substr(0, open);
            args = splitTop(std::string_view(e).substr(open + 1, e.size() - open - 2), ',');
            return true;
        }

        // "ctx->vu0_vf[N]" / "ctx->f[N]" -> N
        int regIndex(const std::string &e, const char *prefix)
        {
            const size_t n = std::strlen(prefix);
            if (e.compare(0, n, prefix) != 0 || e.back() != ']')
                return -1;
            const std::string num = e.substr(n, e.size() - n - 1);
            if (num.empty() || num.size() > 2)
                return -1;
            for (char c : num)
                if (!std::isdigit(static_cast<unsigned char>(c)))
                    return -1;
            const int r = std::atoi(num.c_str());
            return r < 32 ? r : -1;
        }

        // _mm_set_epi32(i3, i2, i1, i0) of 0 / -1 -> lane bits
        bool maskLanes(const std::string &e, uint8_t &lanes)
        {
            std::string name;
            std::vector<std::string> args;
            if (!parseCall(e, name, args) || name != "_mm_set_epi32" || args.size() != 4)
                return false;
            lanes = 0;
            for (int k = 0; k < 4; ++k)
            {
                const std::string &a = args[3 - k];
                if (a == "-1")
                    lanes |= static_cast<uint8_t>(1u << k);
                else if (a != "0")
                    return false;
            }
            return true;
        }

        // _MM_SHUFFLE(z, y, x, w): result lane k takes source lane sel[k].
        bool shuffleSel(const std::string &e, int sel[4])
        {
            std::string name;
            std::vector<std::string> args;
            if (!parseCall(e, name, args) || name != "_MM_SHUFFLE" || args.size() != 4)
                return false;
            for (int k = 0; k < 4; ++k)
            {
                const std::string &a = args[3 - k];
                if (a.size() != 1 || a[0] < '0' || a[0] > '3')
                    return false;
                sel[k] = a[0] - '0';
            }
            return true;
        }

        const char *kBarriers[] = {"runtime", "rdram", "PS2_CALL", "PS2_JUMP", "return", "goto", "throw", "yscall", "xception", "ctx->pc"};
        // VU0 state also goes at guest memory accesses and anything touching
        // the VU0 control side.
        const char *kVuBarriers[] = {"READ", "WRITE", "&ctx->vu0", "fbrst", "cmsar", "Vu0", "VU0", "memcpy(ctx"};
    }

    void FloatKnownState::reset()
    {
        std::memset(m_vf, 0, sizeof(m_vf));
        m_vf[0] = 0xF;
        m_acc = 0;
        m_fprFinite = 0;
        m_faccFinite = false;
    }

    void FloatKnownState::intersect(const FloatKnownState &other)
    {
        for (int r = 0; r < 32; ++r)
            m_vf[r] &= other.m_vf[r];
        m_acc &= other.m_acc;
        m_fprFinite &= other.m_fprFinite;
        m_faccFinite = m_faccFinite && other.m_faccFinite;
    }

    bool FloatKnownState::hasTransfer(const std::string &text)
    {
        static const char *const kTransfers[] = {"PS2_CALL", "callGuest", "dispatchGuestBranch", "throw", "yscall", "xception",
                                                 "handleBreak", "ps2_syscalls", "ps2_stubs", "Vu0", "VU0", "runtime->jumpGuest(rdram, ctx, ctx->pc)"};
        for (const char *t : kTransfers)
        {
            if (text.find(t) != std::string::npos)
                return true;
        }
        // A runtime helper other than the jumps out and the checkpoint.
        for (size_t at = text.find("runtime->"); at != std::string::npos; at = text.find("runtime->", at + 9u))
        {
            const std::string_view rest(text.data() + at + 9u, text.size() - at - 9u);
            if (rest.rfind("jumpGuest(", 0) != 0 && rest.rfind("eeCheckpointDue", 0) != 0)
                return true;
        }
        return false;
    }

    void FloatKnownState::countJoin(bool withEdges) { ++(withEdges ? g_joinsEdges : g_joinsFall); }
    void FloatKnownState::countBranch() { ++g_branchesKept; }

    void FloatKnownState::fprFromConstant(int fpr, uint32_t bits)
    {
        if (!m_fpu || fpr < 0 || fpr >= 32)
            return;
        if ((bits & 0x7F800000u) != 0x7F800000u)
        {
            m_fprFinite |= 1u << fpr;
            ++g_fpuConstants;
        }
        else
            m_fprFinite &= ~(1u << fpr);
    }

    void FloatKnownState::forgetWrites(const std::string &code)
    {
        // vf / ACC assignments
        for (size_t p = code.find("vu0_vf["); p != std::string::npos; p = code.find("vu0_vf[", p + 1))
        {
            const size_t close = code.find(']', p);
            if (close == std::string::npos)
                break;
            size_t q = close + 1;
            while (q < code.size() && code[q] == ' ')
                ++q;
            if (q < code.size() && code[q] == '=' && (q + 1 >= code.size() || code[q + 1] != '='))
            {
                const int r = std::atoi(code.c_str() + p + 7);
                if (r >= 0 && r < 32)
                {
                    m_vf[r] = 0;
                    if (r == 0)
                        ++g_vf0Writes;
                }
            }
        }
        if (hasToken(code, "vu0_acc ="))
            m_acc = 0;
        // FPR writes: assignments and memcpy destinations (&ctx->f[N])
        for (size_t p = code.find("ctx->f["); p != std::string::npos; p = code.find("ctx->f[", p + 1))
        {
            const int r = std::atoi(code.c_str() + p + 7);
            const size_t close = code.find(']', p);
            if (close == std::string::npos || r < 0 || r >= 32)
                break;
            size_t q = close + 1;
            while (q < code.size() && code[q] == ' ')
                ++q;
            const bool assign = q < code.size() && code[q] == '=' && (q + 1 >= code.size() || code[q + 1] != '=');
            const bool addr = p > 0 && code[p - 1] == '&';
            if (assign || addr)
                m_fprFinite &= ~(1u << r);
        }
        if (hasToken(code, "f_acc =") || hasToken(code, "&ctx->f_acc"))
            m_faccFinite = false;
    }

    std::string FloatKnownState::applyVu(const std::string &code, bool &handled)
    {
        handled = false;
        if (code.size() < 4 || code.compare(0, 2, "{ ") != 0 || code.compare(code.size() - 3, 3, "; }") != 0)
            return code;
        if (!hasToken(code, "PS2_V") && !hasToken(code, "_mm_blendv_ps"))
            return code;
        const std::string inner = code.substr(2, code.size() - 5);
        std::vector<std::string> stmts = splitTop(inner, ';');
        for (const std::string &s : stmts)
            if (s.empty())
                return code;
        {
            std::string again = "{ ";
            for (size_t i = 0; i < stmts.size(); ++i)
                again += stmts[i] + "; ";
            again += "}";
            if (again != code)
                return code; // not the emitter's one-line form
        }

        std::unordered_map<std::string, uint8_t> localMask; // __m128 locals: known lanes
        std::unordered_map<std::string, uint8_t> maskLocal; // __m128i locals: lanes
        int destIndex = -2;                                 // -1: ACC
        size_t destStmt = 0;
        uint8_t M = 0;
        std::string destY;
        // The dest statement first: its lanes are what every op needs.
        for (size_t i = 0; i < stmts.size(); ++i)
        {
            const std::string &s = stmts[i];
            if (s.compare(0, 4, "ctx-") != 0)
            {
                std::string decl = s.compare(0, 6, "const ") == 0 ? s.substr(6) : s;
                if (decl.compare(0, 8, "__m128i ") == 0)
                {
                    const size_t eq = decl.find(" = ");
                    uint8_t lanes = 0;
                    if (eq == std::string::npos || !maskLanes(trim(decl.substr(eq + 3)), lanes))
                        return code;
                    maskLocal[trim(decl.substr(8, eq - 8))] = lanes;
                }
                continue;
            }
            const size_t eq = s.find(" = ");
            if (eq == std::string::npos || destIndex != -2)
                return code;
            const std::string lhs = s.substr(0, eq), rhs = trim(s.substr(eq + 3));
            int d = -1;
            if (lhs != "ctx->vu0_acc")
            {
                d = regIndex(lhs, "ctx->vu0_vf[");
                if (d < 0)
                    return code;
            }
            std::string name;
            std::vector<std::string> args;
            if (!parseCall(rhs, name, args) || (name != "_mm_blendv_ps" && name != "PS2_VBLEND") || args.size() != 3 || args[0] != lhs)
                return code;
            // the mask: _mm_castsi128_ps(<mask local or _mm_set_epi32(...)>)
            std::string mname;
            std::vector<std::string> margs;
            if (!parseCall(args[2], mname, margs) || mname != "_mm_castsi128_ps" || margs.size() != 1)
                return code;
            auto it = maskLocal.find(margs[0]);
            if (it != maskLocal.end())
                M = it->second;
            else if (!maskLanes(margs[0], M))
                return code;
            destIndex = d;
            destStmt = i;
            destY = args[1];
        }
        if (destIndex == -2 || M == 0)
            return code;

        auto regMask = [&](const std::string &e, uint8_t &mask) -> bool
        {
            if (e == "ctx->vu0_acc")
            {
                mask = m_acc;
                return true;
            }
            const int r = regIndex(e, "ctx->vu0_vf[");
            if (r < 0)
                return false;
            mask = m_vf[r];
            return true;
        };
        bool rewritten = false;
        // An expression's known lanes; ops rewritten in place (pre-state).
        std::function<uint8_t(std::string &)> eval = [&](std::string &e) -> uint8_t
        {
            uint8_t m = 0;
            if (regMask(e, m))
                return m;
            auto lit = localMask.find(e);
            if (lit != localMask.end())
                return lit->second;
            std::string name;
            std::vector<std::string> args;
            if (!parseCall(e, name, args))
                return 0;
            if (name == "_mm_shuffle_ps" && args.size() == 3)
            {
                int sel[4];
                if (!shuffleSel(args[2], sel))
                    return 0;
                std::string a = args[0], b = args[1];
                const uint8_t ma = eval(a), mb = eval(b);
                if (a != args[0] || b != args[1])
                    return 0; // no ops inside shuffles in the templates
                uint8_t out = 0;
                for (int k = 0; k < 4; ++k)
                {
                    const uint8_t src = k < 2 ? ma : mb;
                    if (src & (1u << sel[k]))
                        out |= static_cast<uint8_t>(1u << k);
                }
                return out;
            }
            if ((name == "PS2_VADD" || name == "PS2_VSUB" || name == "PS2_VMUL") && args.size() == 2)
            {
                ++g_vuOps;
                std::string a = args[0], b = args[1];
                const uint8_t ma = eval(a), mb = eval(b);
                const int ka = (ma & M) == M ? 1 : 0, kb = (mb & M) == M ? 1 : 0;
                g_vuOperands += 2;
                g_vuKnown += static_cast<uint64_t>(ka + kb);
                if (ka || kb || a != args[0] || b != args[1])
                {
                    e = name + (ka || kb ? "_K(" + std::to_string(ka) + ", " + std::to_string(kb) + ", " : "(") + a + ", " + b + ")";
                    rewritten = true;
                }
                return 0xF;
            }
            return 0;
        };

        for (size_t i = 0; i < stmts.size(); ++i)
        {
            std::string &s = stmts[i];
            if (i == destStmt)
                continue;
            std::string decl = s.compare(0, 6, "const ") == 0 ? s.substr(6) : s;
            const size_t prefix = s.size() - decl.size();
            if (decl.compare(0, 7, "__m128 ") != 0)
                continue; // the __m128i masks
            const size_t eq = decl.find(" = ");
            if (eq == std::string::npos)
                return code;
            const std::string name = trim(decl.substr(7, eq - 7));
            std::string rhs = trim(decl.substr(eq + 3));
            const std::string before = rhs;
            localMask[name] = eval(rhs);
            if (rhs != before)
                s = s.substr(0, prefix) + decl.substr(0, eq + 3) + rhs;
        }
        // The value blended in.
        std::string y = destY;
        const uint8_t my = eval(y);
        if (y != destY)
            return code; // an op written into the blend itself: not a template
        uint8_t &dst = destIndex < 0 ? m_acc : m_vf[destIndex];
        if (destIndex == 0)
            ++g_vf0Writes;
        dst = static_cast<uint8_t>((dst & ~M) | (my & M));
        handled = true;
        if (!rewritten)
            return code;
        ++g_vuRewritten;
        std::string out = "{ ";
        for (const std::string &s : stmts)
            out += s + "; ";
        out += "}";
        return out;
    }

    std::string FloatKnownState::applyFpu(const std::string &code, bool &handled)
    {
        handled = false;
        // CVT.S.W: { int32_t tmp; std::memcpy(&tmp, &ctx->f[S], sizeof(tmp)); ctx->f[D] = FPU_CVT_S_W(tmp); }
        if (code.compare(0, 2, "{ ") == 0 && hasToken(code, "= FPU_CVT_S_W(tmp); }"))
        {
            const size_t p = code.rfind("ctx->f[");
            const int d = std::atoi(code.c_str() + p + 7);
            if (p != std::string::npos && code.compare(0, 46, "{ int32_t tmp; std::memcpy(&tmp, &ctx->f[") == 0 && d >= 0 && d < 32)
            {
                m_fprFinite |= 1u << d;
                handled = true;
            }
            return code;
        }
        if (code.empty() || code.back() != ';' || code.find(';') != code.size() - 1)
            return code;
        const std::string stmt = code.substr(0, code.size() - 1);
        bool rewritten = false;
        std::function<bool(std::string &)> eval = [&](std::string &e) -> bool
        {
            if (e == "ctx->f_acc")
                return m_faccFinite;
            const int r = regIndex(e, "ctx->f[");
            if (r >= 0)
                return (m_fprFinite >> r) & 1u;
            std::string name;
            std::vector<std::string> args;
            if (!parseCall(e, name, args))
                return false;
            if ((name == "FPU_ADD_S" || name == "FPU_SUB_S" || name == "FPU_MUL_S") && args.size() == 2)
            {
                ++g_fpuOps;
                std::string a = args[0], b = args[1];
                const bool fa = eval(a), fb = eval(b);
                if (fa && fb)
                {
                    ++g_fpuFinite;
                    e = name + "F(" + a + ", " + b + ")";
                    rewritten = true;
                }
                else if (a != args[0] || b != args[1])
                {
                    e = name + "(" + a + ", " + b + ")";
                    rewritten = true;
                }
                return true; // clamped, or finite operands in round toward zero
            }
            if ((name == "ps2FpuDivide" || name == "ps2FpuSqrt" || name == "ps2FpuRsqrt") && !args.empty() && args[0] == "ctx->fcr31")
                return true; // clamped (or +-FLT_MAX)
            if ((name == "FPU_MOV_S" || name == "FPU_NEG_S" || name == "FPU_ABS_S") && args.size() == 1)
            {
                std::string a = args[0];
                const bool fa = eval(a);
                if (a != args[0])
                {
                    e = name + "(" + a + ")";
                    rewritten = true;
                }
                return fa;
            }
            return false;
        };
        if (stmt.compare(0, 7, "ctx->f[") == 0)
        {
            const size_t eq = stmt.find(" = ");
            if (eq == std::string::npos)
                return code;
            const int d = regIndex(stmt.substr(0, eq), "ctx->f[");
            if (d < 0)
                return code;
            std::string rhs = trim(stmt.substr(eq + 3));
            const bool fin = eval(rhs);
            m_fprFinite = fin ? (m_fprFinite | (1u << d)) : (m_fprFinite & ~(1u << d));
            handled = true;
            return rewritten ? stmt.substr(0, eq + 3) + rhs + ";" : code;
        }
        if (stmt.compare(0, 17, "FPU_SET_ACC(ctx, ") == 0 && stmt.back() == ')')
        {
            std::string rhs = trim(stmt.substr(17, stmt.size() - 18));
            const std::string before = rhs;
            m_faccFinite = eval(rhs);
            handled = true;
            return rewritten ? "FPU_SET_ACC(ctx, " + rhs + ");" : code;
        }
        return code;
    }

    std::string FloatKnownState::apply(const std::string &code)
    {
        if (!enabled())
            return code;
        bool barrier = false;
        for (const char *t : kBarriers)
            barrier = barrier || hasToken(code, t);
        bool vuBarrier = barrier;
        for (const char *t : kVuBarriers)
        {
            if (m_loadsKeepVu && std::strcmp(t, "READ") == 0)
                continue;
            vuBarrier = vuBarrier || hasToken(code, t);
        }
        if (barrier)
        {
            forgetWrites(code);
            reset();
            return code;
        }
        std::string out = code;
        bool vuHandled = false, fpuHandled = false;
        if (vuBarrier)
        {
            forgetWrites(code);
            std::memset(m_vf, 0, sizeof(m_vf));
            m_vf[0] = 0xF;
            m_acc = 0;
            return code;
        }
        if (m_vu && hasToken(code, "vu0_"))
            out = applyVu(code, vuHandled);
        if (!vuHandled && m_fpu && hasToken(code, "ctx->f"))
            out = applyFpu(code, fpuHandled);
        if (!vuHandled && !fpuHandled)
            forgetWrites(code);
        if (!m_vu)
        {
            std::memset(m_vf, 0, sizeof(m_vf));
            m_acc = 0;
        }
        if (!m_fpu)
        {
            m_fprFinite = 0;
            m_faccFinite = false;
        }
        return out;
    }

    std::string floatKnownReport()
    {
        std::ostringstream s;
        s << "float provenance: VU0 ops " << g_vuOps.load() << ", operands known " << g_vuKnown.load() << " of "
          << g_vuOperands.load() << " (" << g_vuRewritten.load() << " instructions rewritten), vf0 writes "
          << g_vf0Writes.load() << "; FPU ops " << g_fpuOps.load() << ", finite operands (no clamp) " << g_fpuFinite.load()
          << "; MTC1 of a known constant " << g_fpuConstants.load() << "; labels joined " << g_joinsEdges.load()
          << " (+ " << g_joinsFall.load() << " with no goto), branches kept " << g_branchesKept.load();
        return s.str();
    }
}
