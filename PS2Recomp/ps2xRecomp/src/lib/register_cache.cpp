// Guest registers in host locals.
//
// The generated code reads and writes every guest GPR / FPR in the
// R5900Context (GPR_U32(ctx, n), SET_GPR_S32(ctx, n, v), ctx->f[n]). ctx
// escapes and guest memory is char data, so the host compiler reloads a
// register after every guest store and stores every result. This pass
// rewrites a generated function body so that the registers live in host
// locals (rc_<n>: the low 64 bits, rf_<n>) for the whole function:
//
// - a local is loaded where it is first read (lazily: nothing on entry, an
//   entry at a mid-function label finds every local stale too); the
//   function's own accesses use the locals (GPR_VEC / SET_GPR_VEC merge the
//   low half with ctx's high half);
// - a "barrier" is any line that hands ctx to something else (calls through
//   the function table, jumps, syscalls, stubs, runtime helpers that take ctx,
//   VU0 microprograms, throw). Before it the dirty locals are stored; the
//   barrier line itself is left as the old code (it reads and writes ctx);
//   after it every local is stale;
// - before every return the dirty locals are stored (a return right after a
//   barrier stores nothing: ctx is the callee's then);
// - two forward analyses over the body's labels / gotos / blocks, iterated to
//   a fixed point: "dirty" (may differ from ctx: over-approximated, storing
//   an in-sync local is harmless) and "stale" (not loaded: a valid local may
//   be dropped and loaded again, a dirty one never). A loop head keeps dirty
//   what its back edges bring, the loop's entry edge stores the rest once;
//   another label keeps what every edge into it holds dirty, an edge with
//   more stores it; an edge where a local the label needs is stale loads it.
//   Resume entries (the switch on ctx->pc) get trampolines at the end of the
//   function that load what the label needs;
// - a write under a condition in its line (MOVN / MOVZ) counts as a read too
//   (the local must be valid if the write does not happen);
// - sign extension (GPRs): a must analysis of the registers whose local holds
//   a sign-extended 32-bit value (32-bit ALU results, byte / half loads,
//   moves of such, andi / ori / slt ...); their 64-bit reads and stores take
//   the low half only, so the host compiler can drop the high half (a pair of
//   ARMv7 registers otherwise). Nothing is known at a resume entry (a
//   coroutine switch, isysSleepSubThread, resumes with full 64-bit values).
//
// Guest memory accesses (READ* / WRITE*) pass ctx to the out-of-line slow
// path, which never reads or writes GPRs / FPRs (address errors set COP0
// state; thread switches are thrown only from syscalls), so they are not
// barriers.
//
// Anything the pass does not expect (another statement inside a call's
// failure block, labels inside blocks, a register named by a non-constant,
// an FPR whose address is taken other than by memcpy, a multi-line
// statement) makes it give the body back unchanged: that function keeps the
// old code. Checked on the output: clang -Wconditional-uninitialized finds
// no local read before it is set.

#include "ps2recomp/register_cache.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ps2recomp
{
    namespace
    {
        std::atomic<uint64_t> g_functions{0};
        std::atomic<uint64_t> g_fallbacks{0};
        std::atomic<uint64_t> g_gprReads{0};
        std::atomic<uint64_t> g_gprWrites{0};
        std::atomic<uint64_t> g_fprReads{0};
        std::atomic<uint64_t> g_fprWrites{0};
        std::atomic<uint64_t> g_oldReads{0};
        std::atomic<uint64_t> g_oldWrites{0};
        std::atomic<uint64_t> g_barriers{0};
        std::atomic<uint64_t> g_flushStores{0};
        std::atomic<uint64_t> g_loads{0};
        std::atomic<uint64_t> g_sextReads{0};
        std::atomic<uint64_t> g_sextStores{0};

        constexpr uint64_t kFprBase = 32;

        enum class Kind
        {
            Blank,
            Label,
            PreIf,
            PreElse,
            PreEndif,
            Open,
            Close,
            Stmt
        };

        struct Line
        {
            std::string text;      // original
            std::string code;      // strings blanked, comment cut, trimmed
            std::string rewritten; // non-barrier lines: on the locals
            Kind kind = Kind::Blank;
            bool barrier = false;
            bool bareOpen = false;  // "{"
            bool rawReturn = false; // a return right after a barrier
            uint64_t writes = 0;
            uint64_t reads = 0;
            uint64_t touched = 0;   // Open: reads | writes of the whole block
            // Sign extension (GPRs): writes whose value is a sign-extended 32-bit
            // one, and writes that are one when their sources are.
            uint64_t kAlways = 0;
            std::vector<std::pair<int, uint64_t>> kDeps;
            bool multiWrite = false;
            size_t close = 0;       // Open: the line of its closing brace
            std::vector<int> gotos; // label indices
            int label = -1;         // Label lines
        };

        struct Fail
        {
            const char *why;
        };

        bool isIdent(char c)
        {
            return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
        }

        std::string trim(std::string_view s)
        {
            size_t b = 0, e = s.size();
            while (b < e && std::isspace(static_cast<unsigned char>(s[b])))
                ++b;
            while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
                --e;
            return std::string(s.substr(b, e - b));
        }

        // The code part of a line: string / char literals blanked, a // comment cut.
        std::string codeOf(const std::string &line)
        {
            std::string out;
            out.reserve(line.size());
            for (size_t i = 0; i < line.size(); ++i)
            {
                const char c = line[i];
                if (c == '"' || c == '\'')
                {
                    const char q = c;
                    out.push_back(q);
                    ++i;
                    while (i < line.size() && line[i] != q)
                    {
                        if (line[i] == '\\')
                            ++i;
                        ++i;
                    }
                    out.push_back(q);
                    continue;
                }
                if (c == '/' && i + 1 < line.size() && line[i + 1] == '/')
                    break;
                out.push_back(c);
            }
            return trim(out);
        }

        size_t skipSpaces(const std::string &s, size_t i)
        {
            while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
                ++i;
            return i;
        }

        // At s[i] (just after "NAME("): "ctx , <digits>" then ')' or ','.
        // Returns the register and the index just past the digits.
        bool parseCtxReg(const std::string &s, size_t i, int &reg, size_t &end)
        {
            i = skipSpaces(s, i);
            if (s.compare(i, 3, "ctx") != 0 || (i + 3 < s.size() && isIdent(s[i + 3])))
                return false;
            i = skipSpaces(s, i + 3);
            if (i >= s.size() || s[i] != ',')
                return false;
            i = skipSpaces(s, i + 1);
            if (i >= s.size() || !std::isdigit(static_cast<unsigned char>(s[i])))
                return false;
            int r = 0;
            while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])))
            {
                r = r * 10 + (s[i] - '0');
                ++i;
            }
            if (r < 0 || r > 31)
                return false;
            reg = r;
            end = i;
            return true;
        }

        struct Pattern
        {
            const char *name;   // macro name
            bool set;           // SET_GPR_*
            const char *local;  // replacement macro (reg != 0)
            const char *zero;   // replacement for reg 0 (nullptr: keep the original)
        };

        // Longest names first (SET_GPR_U32 contains GPR_U32: the identifier
        // boundary check keeps them apart anyway).
        const Pattern kPatterns[] = {
            {"SET_GPR_ZE32", true, "RC_SET_ZE32", "RC_SET_ZERO"},
            {"SET_GPR_U32", true, "RC_SET_U32", "RC_SET_ZERO"},
            {"SET_GPR_S32", true, "RC_SET_S32", "RC_SET_ZERO"},
            {"SET_GPR_U64", true, "RC_SET_U64", "RC_SET_ZERO"},
            {"SET_GPR_S64", true, "RC_SET_S64", "RC_SET_ZERO"},
            {"SET_GPR_VEC", true, "RC_SET_VEC", nullptr},
            {"GPR_U32", false, "RC_U32", "RC_U32"},
            {"GPR_S32", false, "RC_S32", "RC_S32"},
            {"GPR_U64", false, "RC_U64", "RC_U64"},
            {"GPR_S64", false, "RC_S64", "RC_S64"},
            {"GPR_VEC", false, "RC_VEC", nullptr},
        };

        struct Body
        {
            std::vector<Line> lines;
            std::unordered_map<std::string, int> labels;
            std::vector<std::string> labelNames;
            std::vector<size_t> labelLine;
            uint64_t used = 0; // registers with a local
            bool fprs = true;
            // Per label, settled over the walks: the dirty set on entry (what
            // is dirty on every forward edge, plus what is dirty on any back
            // edge) and the stale set (locals not loaded: stale on every
            // edge). An edge that holds more dirty locals stores them, one
            // that holds fewer stale locals loads them.
            std::vector<uint64_t> inDirty, backDirty, inStale, backStale;
            std::vector<char> hasBack; // a goto from at or below the label
            struct Trampoline
            {
                std::string code;
                std::vector<std::pair<std::string, std::string>> entries; // case value, label
            };
            std::vector<Trampoline> trampolines;
            // Known sign-extended: a GPR whose local is a sign-extended 32-bit
            // value on every path (its high half need not be kept: a 64-bit
            // read or a store makes it from the low one). Per label, the
            // intersection over every edge; a resume entry knows nothing.
            std::vector<uint64_t> inKnown, backKnown;
            uint64_t checkpointKnown = ~0ull;
            bool sext = true;
            uint64_t sextReadCount = 0, sextStores = 0;
            // The checkpoint returns (cold) share one exit storing the union
            // of their dirty sets (sites where one of those is stale store
            // their own).
            uint64_t checkpointExit = 0;
            bool usesCheckpointExit = false;
            uint64_t gprReads = 0, gprWrites = 0, fprReads = 0, fprWrites = 0;
            uint64_t oldReads = 0, oldWrites = 0;
            uint64_t barriers = 0, flushStores = 0, loads = 0;
        };

        // Whether the code hands ctx to anything but the GPR macros (and FPU_SET_ACC).
        bool isBarrier(const std::string &code)
        {
            for (size_t i = 0; i + 3 <= code.size(); ++i)
            {
                if (code.compare(i, 5, "throw") == 0 && (i == 0 || !isIdent(code[i - 1])) &&
                    (i + 5 >= code.size() || !isIdent(code[i + 5])))
                    return true;
                if (code.compare(i, 3, "ctx") != 0)
                    continue;
                if ((i > 0 && isIdent(code[i - 1])) || (i + 3 < code.size() && isIdent(code[i + 3])))
                    continue;
                const size_t j = skipSpaces(code, i + 3);
                if (j + 1 < code.size() && code[j] == '-' && code[j + 1] == '>')
                    continue; // a field: checked by the rewrite
                // Bare ctx: an argument. Allowed only as the first argument
                // of the GPR macros / FPU_SET_ACC.
                size_t k = i;
                while (k > 0 && (code[k - 1] == ' ' || code[k - 1] == '\t'))
                    --k;
                if (k == 0 || code[k - 1] != '(')
                    return true;
                size_t e = k - 1;
                while (e > 0 && (code[e - 1] == ' ' || code[e - 1] == '\t'))
                    --e;
                size_t b = e;
                while (b > 0 && isIdent(code[b - 1]))
                    --b;
                const std::string name = code.substr(b, e - b);
                bool allowed = name == "FPU_SET_ACC";
                for (const Pattern &p : kPatterns)
                {
                    if (name == p.name)
                        allowed = true;
                }
                if (!allowed)
                    return true;
            }
            return false;
        }

        // Rewrites a non-barrier line onto the locals; collects writes.
        std::string rewriteLine(Body &body, const std::string &text, uint64_t &writes, uint64_t &reads)
        {
            std::string out;
            out.reserve(text.size() + 16);
            uint64_t vecWrites = 0; // SET_GPR_VEC: ctx and the local both written (not dirty)
            size_t i = 0;
            while (i < text.size())
            {
                const char c = text[i];
                if (c == '"' || c == '\'')
                {
                    const size_t start = i++;
                    while (i < text.size() && text[i] != c)
                    {
                        if (text[i] == '\\')
                            ++i;
                        ++i;
                    }
                    ++i;
                    out.append(text, start, i - start);
                    continue;
                }
                if (c == '/' && i + 1 < text.size() && text[i + 1] == '/')
                {
                    out.append(text, i, std::string::npos);
                    break;
                }
                const bool boundary = i == 0 || !isIdent(text[i - 1]);
                if (boundary && (c == 'S' || c == 'G'))
                {
                    bool matched = false;
                    for (const Pattern &p : kPatterns)
                    {
                        const size_t n = std::strlen(p.name);
                        if (text.compare(i, n, p.name) != 0 || i + n >= text.size() || isIdent(text[i + n]))
                            continue;
                        size_t j = skipSpaces(text, i + n);
                        if (j >= text.size() || text[j] != '(')
                            throw Fail{"GPR macro without arguments"};
                        int reg = 0;
                        size_t end = 0;
                        if (!parseCtxReg(text, j + 1, reg, end))
                            throw Fail{"GPR macro with a non-constant register"};
                        end = skipSpaces(text, end);
                        const bool vec = std::strcmp(p.name, "SET_GPR_VEC") == 0 || std::strcmp(p.name, "GPR_VEC") == 0;
                        if (p.set)
                        {
                            if (end >= text.size() || text[end] != ',')
                                throw Fail{"SET_GPR without a value"};
                            ++end;
                            if (reg == 0 && !p.zero)
                            {
                                out.append(text, i, end - i); // SET_GPR_VEC(ctx, 0, : nothing happens
                            }
                            else if (reg == 0)
                            {
                                out += p.zero;
                                out += '(';
                            }
                            else if (vec)
                            {
                                out += "RC_SET_VEC(ctx, " + std::to_string(reg) + ", rc_" + std::to_string(reg) + ",";
                                body.used |= 1ull << reg;
                                vecWrites |= 1ull << reg;
                                ++body.gprWrites;
                            }
                            else
                            {
                                out += std::string(p.local) + "(rc_" + std::to_string(reg) + ",";
                                body.used |= 1ull << reg;
                                writes |= 1ull << reg;
                                ++body.gprWrites;
                            }
                        }
                        else
                        {
                            if (end >= text.size() || text[end] != ')')
                                throw Fail{"GPR read not closed"};
                            ++end;
                            if (reg == 0 && !p.zero)
                            {
                                out.append(text, i, end - i); // GPR_VEC(ctx, 0): zero
                            }
                            else if (reg == 0)
                            {
                                out += std::string(p.zero) + "(0)";
                            }
                            else if (vec)
                            {
                                out += "RC_VEC(ctx, " + std::to_string(reg) + ", rc_" + std::to_string(reg) + ")";
                                body.used |= 1ull << reg;
                                reads |= 1ull << reg;
                                ++body.gprReads;
                            }
                            else
                            {
                                out += std::string(p.local) + "(rc_" + std::to_string(reg) + ")";
                                body.used |= 1ull << reg;
                                reads |= 1ull << reg;
                                ++body.gprReads;
                            }
                        }
                        i = end;
                        matched = true;
                        break;
                    }
                    if (matched)
                        continue;
                }
                if (boundary && c == 'c' && text.compare(i, 3, "ctx") == 0 && i + 3 < text.size() && !isIdent(text[i + 3]))
                {
                    size_t j = skipSpaces(text, i + 3);
                    if (j + 1 < text.size() && text[j] == '-' && text[j + 1] == '>')
                    {
                        j = skipSpaces(text, j + 2);
                        if (text.compare(j, 2, "r[") == 0 || (text.compare(j, 1, "r") == 0 && j + 1 < text.size() && !isIdent(text[j + 1])))
                            throw Fail{"direct ctx->r access"};
                        if (body.fprs && text.compare(j, 2, "f[") == 0)
                        {
                            size_t k = j + 2;
                            if (k >= text.size() || !std::isdigit(static_cast<unsigned char>(text[k])))
                                throw Fail{"FPR with a non-constant index"};
                            int reg = 0;
                            while (k < text.size() && std::isdigit(static_cast<unsigned char>(text[k])))
                                reg = reg * 10 + (text[k++] - '0');
                            if (reg > 31 || k >= text.size() || text[k] != ']')
                                throw Fail{"FPR index"};
                            ++k;
                            bool write = false;
                            // Address taken: memcpy's destination writes, a later argument reads.
                            if (i > 0 && text[i - 1] == '&')
                            {
                                const std::string before = trim(std::string_view(out).substr(0, out.size() - 1));
                                if (before.size() >= 7 && before.compare(before.size() - 7, 7, "memcpy(") == 0)
                                    write = true;
                                else if (!before.empty() && before.back() == ',')
                                    write = false;
                                else
                                    throw Fail{"FPR address taken"};
                            }
                            else
                            {
                                const size_t a = skipSpaces(text, k);
                                if (a < text.size() && text[a] == '=' && (a + 1 >= text.size() || text[a + 1] != '='))
                                    write = true;
                                else if (a + 1 < text.size() && text[a + 1] == '=' && std::strchr("+-*/%&|^", text[a]))
                                    throw Fail{"FPR compound assignment"};
                                else if (a + 1 < text.size() && (text.compare(a, 2, "++") == 0 || text.compare(a, 2, "--") == 0))
                                    throw Fail{"FPR increment"};
                            }
                            out += "rf_" + std::to_string(reg);
                            body.used |= 1ull << (kFprBase + reg);
                            if (write)
                            {
                                writes |= 1ull << (kFprBase + reg);
                                ++body.fprWrites;
                            }
                            else
                            {
                                reads |= 1ull << (kFprBase + reg);
                                ++body.fprReads;
                            }
                            i = k;
                            continue;
                        }
                    }
                }
                out.push_back(c);
                ++i;
            }
            // A 128-bit write stores ctx and loads the local: it makes the
            // local valid without making it dirty. Kept simple: a line that
            // also writes the register otherwise is not expected.
            if (vecWrites & writes)
                throw Fail{"SET_GPR_VEC and SET_GPR of one register in a line"};
            writes |= vecWrites;
            return out;
        }

        // Counts the old code's ctx register accesses in a line (statistics).
        void countOld(Body &body, const std::string &code)
        {
            size_t pos = 0;
            while ((pos = code.find("GPR_", pos)) != std::string::npos)
            {
                const bool set = pos >= 4 && code.compare(pos - 4, 4, "SET_") == 0;
                if (pos == 0 || !isIdent(code[pos - 1]) || set)
                {
                    if (set)
                        ++body.oldWrites;
                    else
                        ++body.oldReads;
                }
                pos += 4;
            }
            pos = 0;
            while ((pos = code.find("ctx->f[", pos)) != std::string::npos)
            {
                const size_t close = code.find(']', pos);
                const size_t a = close == std::string::npos ? close : skipSpaces(code, close + 1);
                const bool dest = pos > 0 && code[pos - 1] == '&' && pos >= 8 && code.compare(pos - 8, 7, "memcpy(") == 0;
                if (dest || (a != std::string::npos && a < code.size() && code[a] == '=' && (a + 1 >= code.size() || code[a + 1] != '=')))
                    ++body.oldWrites;
                else
                    ++body.oldReads;
                pos += 7;
            }
        }

        int netBraces(const std::string &code)
        {
            int n = 0;
            for (char c : code)
            {
                if (c == '{')
                    ++n;
                else if (c == '}')
                    --n;
            }
            return n;
        }

        bool startsWith(const std::string &s, const char *p)
        {
            return s.compare(0, std::strlen(p), p) == 0;
        }

        bool hasIfToken(const std::string &code)
        {
            size_t pos = 0;
            while ((pos = code.find("if", pos)) != std::string::npos)
            {
                if ((pos == 0 || !isIdent(code[pos - 1])) && (pos + 2 >= code.size() || !isIdent(code[pos + 2])))
                    return true;
                pos += 2;
            }
            return false;
        }

        // The value text of a macro call starting at `open` (just past its
        // '('), after `skipArgs` commas: up to the matching ')'.
        bool macroValue(const std::string &t, size_t open, int skipArgs, std::string &value)
        {
            int depth = 0, commas = 0;
            size_t start = open;
            for (size_t i = open; i < t.size(); ++i)
            {
                const char c = t[i];
                if (c == '"' || c == '\'')
                    return false;
                if (c == '(')
                    ++depth;
                else if (c == ')')
                {
                    if (depth == 0)
                    {
                        if (commas < skipArgs)
                            return false;
                        value = trim(std::string_view(t).substr(start, i - start));
                        return true;
                    }
                    --depth;
                }
                else if (c == ',' && depth == 0 && commas < skipArgs)
                {
                    ++commas;
                    start = i + 1;
                }
            }
            return false;
        }

        uint64_t operandMask(const std::string &m)
        {
            return m.empty() ? 0 : 1ull << std::stoi(m); // "" : $zero
        }

        // Which writes of a rewritten line leave a sign-extended value.
        void classifyWrites(Line &line)
        {
            static const std::string op = R"(RC_U64\((?:rc_(\d+)|0)\))";
            static const std::regex add(R"(^\(uint64_t\))" + op + R"( \+ \(uint64_t\))" + op + "$");
            static const std::regex orImm("^" + op + R"( (?:\||\^) \(uint64_t\)\(uint16_t\)\d+$)");
            static const std::regex andImm("^" + op + R"( & \(uint64_t\)\(uint16_t\)\d+$)");
            static const std::regex logic("^" + op + R"( (?:\||&|\^) )" + op + "$");
            const std::string &t = line.rewritten;
            int count = 0;
            size_t pos = 0;
            while ((pos = t.find("RC_SET_", pos)) != std::string::npos)
            {
                const size_t paren = t.find('(', pos);
                if (paren == std::string::npos)
                    break;
                const std::string kind = t.substr(pos + 7, paren - pos - 7);
                pos = paren + 1;
                if (kind == "ZERO")
                    continue;
                ++count;
                if (kind == "VEC")
                    continue; // killed (in writes)
                const size_t r = t.find("rc_", paren);
                if (r != paren + 1)
                    throw Fail{"RC_SET layout"};
                const int dest = std::stoi(t.substr(r + 3));
                if (kind == "U32" || kind == "S32")
                {
                    line.kAlways |= 1ull << dest;
                    continue;
                }
                std::string value;
                if (!macroValue(t, paren + 1, 1, value))
                    continue;
                if (kind == "ZE32")
                {
                    if (startsWith(value, "(uint8_t)") || startsWith(value, "(uint16_t)"))
                        line.kAlways |= 1ull << dest;
                    continue;
                }
                if (kind != "U64" && kind != "S64")
                    continue;
                std::smatch m;
                if (std::regex_match(value, m, add))
                {
                    // A move (daddu x, y, $zero) or zero; a real 64-bit add is not one.
                    if (m[1].str().empty() || m[2].str().empty())
                        line.kDeps.push_back({dest, operandMask(m[1].str()) | operandMask(m[2].str())});
                }
                else if (std::regex_match(value, m, orImm))
                    line.kDeps.push_back({dest, operandMask(m[1].str())});
                else if (std::regex_match(value, m, andImm))
                    line.kAlways |= 1ull << dest;
                else if (std::regex_match(value, m, logic))
                    line.kDeps.push_back({dest, operandMask(m[1].str()) | operandMask(m[2].str())});
                else if (value.size() > 8 && value.compare(value.size() - 8, 8, "? 1 : 0)") == 0)
                    line.kAlways |= 1ull << dest;
                else if (value.size() > 7 && value.compare(value.size() - 7, 7, "? 1 : 0") == 0)
                    line.kAlways |= 1ull << dest;
            }
            line.multiWrite = count > 1;
            if (line.multiWrite)
            {
                // Kept simple: the order of several writes is not followed.
                line.kAlways = 0;
                line.kDeps.clear();
            }
        }

        uint64_t knownAfter(const Line &line, uint64_t known)
        {
            uint64_t after = (known & ~line.writes) | line.kAlways;
            for (const auto &[dest, src] : line.kDeps)
            {
                if ((src & ~known & ~1ull) == 0)
                    after |= 1ull << dest;
            }
            return after & ~(~0ull << 32) & ~1ull; // GPRs only
        }

        // The 64-bit reads of known registers in a line as sign extensions of
        // their low halves.
        void sextReads(Body &body, std::string &text, uint64_t known)
        {
            for (int r = 1; r < 32; ++r)
            {
                if (!(known & (1ull << r)))
                    continue;
                const std::string n = std::to_string(r);
                const std::pair<std::string, std::string> forms[] = {
                    {"RC_U64(rc_" + n + ")", "RC_U64X(rc_" + n + ")"},
                    {"RC_S64(rc_" + n + ")", "RC_S64X(rc_" + n + ")"},
                    {"RC_VEC(ctx, " + n + ", rc_" + n + ")", "RC_VECX(ctx, " + n + ", rc_" + n + ")"}};
                for (const auto &[from, to] : forms)
                {
                    size_t pos = 0;
                    while ((pos = text.find(from, pos)) != std::string::npos)
                    {
                        text.replace(pos, from.size(), to);
                        pos += to.size();
                        ++body.sextReadCount;
                    }
                }
            }
        }

        void parse(Body &body, const std::string &text)
        {
            std::istringstream in(text);
            std::string raw;
            while (std::getline(in, raw))
            {
                Line line;
                line.text = raw;
                line.code = codeOf(raw);
                const std::string &code = line.code;
                if (code.empty())
                    line.kind = Kind::Blank;
                else if (code[0] == '#')
                {
                    if (startsWith(code, "#if"))
                        line.kind = Kind::PreIf;
                    else if (startsWith(code, "#else"))
                        line.kind = Kind::PreElse;
                    else if (startsWith(code, "#endif"))
                        line.kind = Kind::PreEndif;
                    else
                        throw Fail{"preprocessor line"};
                }
                else if (startsWith(code, "label_") && code.back() == ':' && code.find(' ') == std::string::npos)
                {
                    line.kind = Kind::Label;
                    const std::string name = code.substr(0, code.size() - 1);
                    if (body.labels.count(name))
                        throw Fail{"label defined twice"};
                    line.label = static_cast<int>(body.labels.size());
                    body.labels.emplace(name, line.label);
                    body.labelNames.push_back(name);
                }
                else
                {
                    const int net = netBraces(code);
                    if (net == 1 && code.back() == '{')
                    {
                        line.kind = Kind::Open;
                        line.bareOpen = code == "{";
                        if (!line.bareOpen && !startsWith(code, "if (") && !startsWith(code, "if(") && !startsWith(code, "switch (") && !startsWith(code, "switch("))
                            throw Fail{"unknown block"};
                    }
                    else if (net == -1 && code == "}")
                        line.kind = Kind::Close;
                    else if (net == 0)
                        line.kind = Kind::Stmt;
                    else
                        throw Fail{"unbalanced line"};
                }
                body.lines.push_back(std::move(line));
            }

            for (Line &line : body.lines)
            {
                if (line.kind != Kind::Open && line.kind != Kind::Stmt)
                    continue;
                countOld(body, line.code);
                line.barrier = isBarrier(line.code);
                if (line.barrier)
                {
                    if (line.code.find("goto") != std::string::npos)
                        throw Fail{"goto in a barrier line"};
                    ++body.barriers;
                    continue;
                }
                line.rewritten = rewriteLine(body, line.text, line.writes, line.reads);
                // A write under a condition in the line (MOVN / MOVZ:
                // "if (rt != 0) SET_GPR_VEC(...)") may not happen: the local
                // must hold the register before it, as for a read.
                if (hasIfToken(line.code))
                    line.reads |= line.writes;
                if (body.sext)
                    classifyWrites(line);
                size_t pos = 0;
                while ((pos = line.code.find("goto ", pos)) != std::string::npos)
                {
                    pos += 5;
                    size_t e = pos;
                    while (e < line.code.size() && isIdent(line.code[e]))
                        ++e;
                    auto it = body.labels.find(line.code.substr(pos, e - pos));
                    if (it == body.labels.end())
                        throw Fail{"goto to an unknown label"};
                    line.gotos.push_back(it->second);
                }
            }
            body.labelLine.assign(body.labels.size(), 0);
            for (size_t i = 0; i < body.lines.size(); ++i)
            {
                if (body.lines[i].kind == Kind::Label)
                    body.labelLine[body.lines[i].label] = i;
            }
            // Blocks: where each closes, what it touches.
            std::vector<size_t> open;
            for (size_t i = 0; i < body.lines.size(); ++i)
            {
                Line &line = body.lines[i];
                for (size_t o : open)
                    body.lines[o].touched |= line.reads | line.writes;
                if (line.kind == Kind::Open)
                {
                    line.touched |= line.reads | line.writes;
                    open.push_back(i);
                }
                else if (line.kind == Kind::Close)
                {
                    if (open.empty())
                        throw Fail{"unmatched }"};
                    body.lines[open.back()].close = i;
                    open.pop_back();
                }
            }
            if (!open.empty())
                throw Fail{"unclosed block"};
            const size_t n = body.labels.size();
            body.inDirty.assign(n, 0);
            body.backDirty.assign(n, 0);
            body.inStale.assign(n, 0);
            body.backStale.assign(n, 0);
            body.hasBack.assign(n, 0);
            for (size_t i = 0; i < body.lines.size(); ++i)
            {
                for (int g : body.lines[i].gotos)
                {
                    if (body.labelLine[g] <= i)
                        body.hasBack[g] = 1;
                }
            }
            body.inKnown.assign(n, ~0ull);
            body.backKnown.assign(n, ~0ull);
        }

        std::string indentOf(const std::string &text)
        {
            size_t i = 0;
            while (i < text.size() && (text[i] == ' ' || text[i] == '\t'))
                ++i;
            return text.substr(0, i);
        }


        std::string flushCode(Body &body, uint64_t dirty, uint64_t known, bool count)
        {
            std::string s;
            dirty &= body.used;
            for (int r = 1; r < 64; ++r)
            {
                if (!(dirty & (1ull << r)))
                    continue;
                if (count)
                    ++body.flushStores;
                if (r < 32 && (known & (1ull << r)))
                {
                    s += "Ps2SetGpr64(ctx, " + std::to_string(r) + ", RC_U64X(rc_" + std::to_string(r) + ")); ";
                    if (count)
                        ++body.sextStores;
                }
                else if (r < 32)
                    s += "Ps2SetGpr64(ctx, " + std::to_string(r) + ", rc_" + std::to_string(r) + "); ";
                else
                    s += "ctx->f[" + std::to_string(r - 32) + "] = rf_" + std::to_string(r - 32) + "; ";
            }
            return s;
        }

        std::string loadCode(Body &body, uint64_t stale, bool count)
        {
            std::string s;
            stale &= body.used;
            for (int r = 1; r < 64; ++r)
            {
                if (!(stale & (1ull << r)))
                    continue;
                if (count)
                    ++body.loads;
                if (r < 32)
                    s += "rc_" + std::to_string(r) + " = Ps2GprU64(ctx, " + std::to_string(r) + "); ";
                else
                    s += "rf_" + std::to_string(r - 32) + " = ctx->f[" + std::to_string(r - 32) + "]; ";
            }
            return s;
        }

        // Each "return;" in the code part of a rewritten line, with the
        // dirty locals stored first.
        std::string withFlushedReturns(const std::string &line, const std::string &flush)
        {
            if (flush.empty())
                return line;
            std::string out;
            size_t i = 0;
            const size_t comment = line.find("//");
            while (true)
            {
                const size_t pos = line.find("return;", i);
                if (pos == std::string::npos || (comment != std::string::npos && pos > comment))
                {
                    out.append(line, i, std::string::npos);
                    break;
                }
                out.append(line, i, pos - i);
                out += "{ " + flush + "return; }";
                i = pos + 7;
            }
            return out;
        }

        enum class Frame
        {
            Bare,
            Cond,
            PostBarrier,
            Pre
        };

        struct FrameState
        {
            Frame kind;
            uint64_t dirty, stale; // on entry
            bool reachable;        // on entry
            uint64_t known = 0;    // on entry
            bool checkpoint = false; // if (runtime->eeCheckpointDue()) {
            bool resume = false;     // switch (ctx->pc) {: the entries at labels
            // Pre: the state at the end of the first branch.
            uint64_t accDirty = 0, accStale = 0, accKnown = 0;
            bool accReachable = false, inElse = false;
        };

        struct Walk
        {
            Body &body;
            std::vector<std::string> *out;
            bool emit;
            bool changed = false;
            uint64_t dirty = 0, stale = 0, known = 0;
            bool reachable = true;
            std::vector<FrameState> stack;
            std::vector<uint64_t> fwdDirty, fwdStale, inDirty, inStale, fwdKnown, inKnown;
            std::vector<char> fwdSeen;

            Walk(Body &b, std::vector<std::string> *o) : body(b), out(o), emit(o != nullptr)
            {
                const size_t n = body.labels.size();
                fwdDirty.assign(n, ~0ull);
                fwdStale.assign(n, 0);
                fwdSeen.assign(n, 0);
                inDirty.assign(n, 0);
                inStale.assign(n, ~0ull);
                fwdKnown.assign(n, ~0ull);
                inKnown.assign(n, ~0ull);
            }

            FrameState frame(Frame kind)
            {
                FrameState f{kind, dirty, stale, reachable};
                f.known = known;
                return f;
            }

            void put(const std::string &s)
            {
                if (emit && s.find_first_not_of(" \t") != std::string::npos)
                    out->push_back(s);
            }

            void check(const char *where)
            {
                if (dirty & stale & body.used)
                    throw Fail{where};
            }

            // An edge from line `from` to a label: its contribution to the
            // label's sets.
            void edge(size_t from, int label, bool resume, uint64_t edgeKnown)
            {
                if (body.labelLine[label] > from)
                {
                    // A resume entry (cold: nothing dirty, everything loaded
                    // by its trampoline) fits any dirty / stale state, but
                    // its registers hold whatever ctx holds: nothing is
                    // known sign extended there (a coroutine switch resumes
                    // with full 64-bit values, isysSleepSubThread).
                    if (resume)
                    {
                        fwdKnown[label] = 0;
                        return;
                    }
                    fwdKnown[label] &= edgeKnown;
                    fwdDirty[label] &= dirty;
                    fwdSeen[label] = 1;
                    fwdStale[label] |= stale;
                    return;
                }
                if ((body.backDirty[label] | dirty) != body.backDirty[label])
                {
                    body.backDirty[label] |= dirty;
                    changed = true;
                }
                if ((body.backStale[label] | stale) != body.backStale[label])
                {
                    body.backStale[label] |= stale;
                    changed = true;
                }
                if ((body.backKnown[label] & edgeKnown) != body.backKnown[label])
                {
                    body.backKnown[label] &= edgeKnown;
                    changed = true;
                }
            }

            // What the edge to a label does: store what the label does not
            // hold dirty, load what it does not hold stale.
            std::string edgeCode(int label, uint64_t edgeKnown, bool count)
            {
                check("edge: dirty and stale");
                const uint64_t store = dirty & ~body.inDirty[label];
                const uint64_t load = stale & ~body.inStale[label];
                return flushCode(body, store, edgeKnown, count) + loadCode(body, load, count);
            }

            void gotos(size_t idx, Line &line, bool resume, uint64_t edgeKnown)
            {
                std::vector<int> done;
                for (int g : line.gotos)
                {
                    edge(idx, g, resume, edgeKnown);
                    if (!emit || std::find(done.begin(), done.end(), g) != done.end())
                        continue;
                    done.push_back(g);
                    const std::string code = edgeCode(g, edgeKnown, true);
                    if (code.empty())
                        continue;
                    const std::string target = "goto " + body.labelNames[g] + ";";
                    std::string with = "{ " + code + target + " }";
                    if (resume)
                    {
                        // Cold: entries with the same loads share one
                        // trampoline at the end of the function, which
                        // switches on ctx->pc again.
                        const size_t c = line.code.find("case ");
                        const size_t colon = line.code.find(':');
                        if (c == std::string::npos || colon == std::string::npos || colon < c)
                            throw Fail{"resume case layout"};
                        const std::string value = trim(std::string_view(line.code).substr(c + 5, colon - c - 5));
                        size_t k = 0;
                        while (k < body.trampolines.size() && body.trampolines[k].code != code)
                            ++k;
                        if (k == body.trampolines.size())
                            body.trampolines.push_back({code, {}});
                        body.trampolines[k].entries.push_back({value, body.labelNames[g]});
                        with = "goto rc_resume_" + std::to_string(k) + ";";
                    }
                    std::string &text = line.rewritten;
                    size_t pos = 0;
                    while ((pos = text.find(target, pos)) != std::string::npos)
                    {
                        text.replace(pos, target.size(), with);
                        pos += with.size();
                    }
                }
            }

            void afterBarrier()
            {
                dirty = 0;
                stale = body.used;
                known = 0;
            }

            void run()
            {
                stale = body.used; // nothing loaded on entry
                for (size_t idx = 0; idx < body.lines.size(); ++idx)
                    line(idx);
                if (!stack.empty())
                    throw Fail{"unclosed block"};
            }

            void line(size_t idx)
            {
                Line &line = body.lines[idx];
                const bool inPostBarrier = !stack.empty() && stack.back().kind == Frame::PostBarrier;
                const std::string indent = indentOf(line.text);
                switch (line.kind)
                {
                case Kind::Blank:
                    put(line.text);
                    return;
                case Kind::Label:
                {
                    if (!stack.empty())
                        throw Fail{"label inside a block"};
                    const int l = line.label;
                    if (reachable && idx > 0)
                    {
                        edge(idx - 1, l, false, known); // the fall-through
                        if (emit)
                            put("    " + edgeCode(l, known, true));
                    }
                    // A loop head keeps dirty what its back edges bring (its
                    // entry stores the rest once); another label what every
                    // edge into it holds. Stale: what any edge holds stale
                    // and is not dirty (a valid local may be dropped, it is
                    // loaded again where read).
                    inDirty[l] = body.hasBack[l] ? body.backDirty[l] : (fwdSeen[l] ? fwdDirty[l] : 0);
                    inStale[l] = (fwdStale[l] | body.backStale[l]) & ~inDirty[l];
                    inKnown[l] = fwdKnown[l] & body.backKnown[l] & ~inStale[l] & (body.sext ? ~0ull : 0);
                    if (inDirty[l] != body.inDirty[l] || inStale[l] != body.inStale[l] || inKnown[l] != body.inKnown[l])
                    {
                        if (emit)
                            throw Fail{"entry sets not settled"};
                        changed = true;
                    }
                    dirty = inDirty[l];
                    stale = inStale[l];
                    known = inKnown[l];
                    check("label: dirty and stale");
                    reachable = true;
                    put(line.text);
                    return;
                }
                case Kind::PreIf:
                    stack.push_back(frame(Frame::Pre));
                    put(line.text);
                    return;
                case Kind::PreElse:
                {
                    if (stack.empty() || stack.back().kind != Frame::Pre || stack.back().inElse)
                        throw Fail{"#else"};
                    FrameState &f = stack.back();
                    f.accDirty = dirty;
                    f.accStale = stale;
                    f.accKnown = known;
                    f.accReachable = reachable;
                    f.inElse = true;
                    dirty = f.dirty;
                    stale = f.stale;
                    known = f.known;
                    reachable = f.reachable;
                    put(line.text);
                    return;
                }
                case Kind::PreEndif:
                {
                    if (stack.empty() || stack.back().kind != Frame::Pre)
                        throw Fail{"#endif"};
                    const FrameState f = stack.back();
                    stack.pop_back();
                    if (f.inElse)
                    {
                        if (f.accReachable && reachable && f.accStale != stale)
                            throw Fail{"#if branches load differently"};
                        if (f.accReachable && !reachable)
                        {
                            stale = f.accStale;
                            known = f.accKnown;
                        }
                        else if (f.accReachable)
                            known &= f.accKnown;
                        dirty |= f.accReachable ? f.accDirty : 0;
                        reachable = reachable || f.accReachable;
                    }
                    else
                    {
                        // #if without #else: the skipped path.
                        if (reachable && stale != f.stale)
                            throw Fail{"#if loads"};
                        dirty |= f.dirty;
                        known = reachable ? (known & f.known) : f.known;
                        stale = f.stale;
                        reachable = true;
                    }
                    put(line.text);
                    return;
                }
                case Kind::Open:
                    if (inPostBarrier)
                        throw Fail{"block inside a call's failure block"};
                    if (line.barrier)
                    {
                        check("flush: dirty and stale");
                        if (emit)
                            put(indent + flushCode(body, dirty, known, true));
                        put(line.text);
                        stack.push_back(frame(Frame::PostBarrier));
                        return;
                    }
                    {
                        // A conditional block loads first what it touches.
                        const uint64_t need = (line.bareOpen ? line.reads : line.touched) & stale;
                        if (emit)
                            put(indent + loadCode(body, need, true));
                        stale &= ~need;
                        known &= ~need;
                        const uint64_t before = known;
                        if (emit)
                            sextReads(body, line.rewritten, line.multiWrite ? before & ~line.writes : before);
                        stale &= ~line.writes;
                        dirty |= line.writes;
                        known = knownAfter(line, before);
                        gotos(idx, line, false, before & known);
                        FrameState f = frame(line.bareOpen ? Frame::Bare : Frame::Cond);
                        f.checkpoint = line.code.find("eeCheckpointDue()") != std::string::npos;
                        f.resume = startsWith(line.code, "switch (ctx->pc)");
                        stack.push_back(f);
                        put(line.rewritten);
                    }
                    return;
                case Kind::Close:
                {
                    if (stack.empty() || stack.back().kind == Frame::Pre)
                        throw Fail{"unmatched }"};
                    const FrameState f = stack.back();
                    stack.pop_back();
                    if (f.kind == Frame::PostBarrier)
                    {
                        put(line.text);
                        afterBarrier();
                        reachable = f.reachable;
                        return;
                    }
                    if (f.kind == Frame::Cond)
                    {
                        // The path through the block meets the one around it:
                        // what the block left stale (a barrier inside) is
                        // loaded at its end.
                        if (reachable)
                        {
                            // Dirty after the block: valid on both paths.
                            const uint64_t after = dirty | f.dirty;
                            const uint64_t load = stale & after;
                            if (emit)
                                put(indent + "    " + loadCode(body, load, true));
                            dirty = after;
                            stale = ((stale & ~load) | f.stale) & ~after;
                            known &= f.known & ~load & ~stale;
                        }
                        else
                        {
                            dirty = f.dirty;
                            known = f.known;
                            stale = f.stale;
                        }
                        reachable = f.reachable;
                        check("block end: dirty and stale");
                    }
                    put(line.text);
                    return;
                }
                case Kind::Stmt:
                    if (inPostBarrier)
                    {
                        if (line.code != "return;")
                            throw Fail{"statement inside a call's failure block"};
                        put(line.text);
                        reachable = false;
                        return;
                    }
                    if (line.rawReturn)
                    {
                        put(line.text);
                        reachable = false;
                        return;
                    }
                    if (line.barrier)
                    {
                        check("flush: dirty and stale");
                        if (emit)
                            put(indent + flushCode(body, dirty, known, true));
                        put(line.text);
                        const std::string &code = line.code;
                        if (code.find("return;") != std::string::npos)
                        {
                            const bool endsReturn = code.size() >= 7 && code.compare(code.size() - 7, 7, "return;") == 0;
                            if (!endsReturn || hasIfToken(code) || code.find("return;") != code.size() - 7)
                                throw Fail{"barrier line with a conditional return"};
                            reachable = false;
                            return;
                        }
                        // A return right after it: ctx is the callee's.
                        size_t j = idx + 1;
                        while (j < body.lines.size() && body.lines[j].kind == Kind::Blank)
                            ++j;
                        if (j < body.lines.size() && body.lines[j].kind == Kind::Stmt && body.lines[j].code == "return;")
                            body.lines[j].rawReturn = true;
                        afterBarrier();
                        return;
                    }
                    {
                        const uint64_t need = line.reads & stale;
                        if (emit)
                            put(indent + loadCode(body, need, true));
                        known &= ~need;
                        const uint64_t before = known;
                        if (emit)
                            sextReads(body, line.rewritten, line.multiWrite ? before & ~line.writes : before);
                        stale &= ~(line.reads | line.writes);
                        dirty |= line.writes;
                        known = knownAfter(line, before);
                        // Inside the line (a goto or return before or after its
                        // write): what is known both before and after.
                        const uint64_t mid = before & known;
                        gotos(idx, line, !stack.empty() && stack.back().resume, mid);
                        if (line.code.find("return;") != std::string::npos)
                        {
                            check("return: dirty and stale");
                            const bool cold = !stack.empty() && stack.back().checkpoint && line.code == "return;";
                            if (cold && (dirty & body.used))
                            {
                                body.checkpointExit |= dirty;
                                body.checkpointKnown &= mid;
                                if (emit && !(stale & body.checkpointExit & body.used))
                                {
                                    body.usesCheckpointExit = true;
                                    put(indent + "goto rc_checkpoint_return;");
                                }
                                else if (emit)
                                {
                                    put(withFlushedReturns(line.rewritten, flushCode(body, dirty, mid, true)));
                                }
                            }
                            else if (emit)
                            {
                                put(withFlushedReturns(line.rewritten, flushCode(body, dirty, mid, true)));
                            }
                        }
                        else
                        {
                            put(line.rewritten);
                        }
                        if (line.code == "return;" ||
                            (line.gotos.size() == 1 && line.code == "goto " + body.labelNames[line.gotos[0]] + ";"))
                            reachable = false;
                    }
                    return;
                }
            }
        };

        // One walk over the body: with out == nullptr it only updates the
        // labels' sets (returns whether any changed).
        bool walk(Body &body, std::vector<std::string> *out)
        {
            for (Line &line : body.lines)
                line.rawReturn = false;
            Walk w(body, out);
            w.run();
            body.inDirty = w.inDirty;
            body.inStale = w.inStale;
            body.inKnown = w.inKnown;
            return w.changed;
        }
    }

    std::string applyRegisterCache(const std::string &text, const RegisterCacheOptions &options)
    {
        ++g_functions;
        Body body;
        body.fprs = options.fprs;
        body.sext = options.sext;
        std::vector<std::string> out;
        try
        {
            parse(body, text);
            int rounds = 0;
            while (walk(body, nullptr))
            {
                if (++rounds > 500)
                    throw Fail{"no fixed point"};
            }
            // The emit walk edits the rewritten lines (edge code in front of
            // gotos): keep copies for nothing else.
            walk(body, &out);
        }
        catch (const Fail &fail)
        {
            ++g_fallbacks;
            if (std::getenv("PS2X_RECOMP_REGCACHE_DEBUG"))
            {
                const size_t at = text.find("ctx->pc = 0x");
                std::cerr << "[regcache] old code kept (" << fail.why << ") near "
                          << (at == std::string::npos ? std::string("?") : text.substr(at, 22)) << std::endl;
            }
            return text;
        }

        std::string result;
        result.reserve(text.size() + text.size() / 4);
        // The locals: loaded where first read (nothing on entry: an entry at
        // a mid-function label finds them all stale too).
        for (int r = 1; r < 64; ++r)
        {
            if (!(body.used & (1ull << r)))
                continue;
            if (r < 32)
                result += "    uint64_t rc_" + std::to_string(r) + ";\n";
            else
                result += "    float rf_" + std::to_string(r - 32) + ";\n";
        }
        for (const std::string &line : out)
        {
            result += line;
            result += '\n';
        }
        if (body.usesCheckpointExit)
            result += "rc_checkpoint_return:\n    " + flushCode(body, body.checkpointExit, body.checkpointKnown, true) + "return;\n";
        for (size_t k = 0; k < body.trampolines.size(); ++k)
        {
            const auto &t = body.trampolines[k];
            result += "rc_resume_" + std::to_string(k) + ":\n    " + t.code + "\n";
            if (t.entries.size() == 1)
            {
                result += "    goto " + t.entries[0].second + ";\n";
                continue;
            }
            result += "    switch (ctx->pc) {\n";
            for (const auto &[value, label] : t.entries)
                result += "        case " + value + ": goto " + label + ";\n";
            result += "        default: goto " + t.entries[0].second + ";\n    }\n";
        }

        g_gprReads += body.gprReads;
        g_gprWrites += body.gprWrites;
        g_fprReads += body.fprReads;
        g_fprWrites += body.fprWrites;
        g_oldReads += body.oldReads;
        g_oldWrites += body.oldWrites;
        g_barriers += body.barriers;
        g_flushStores += body.flushStores;
        g_loads += body.loads;
        g_sextReads += body.sextReadCount;
        g_sextStores += body.sextStores;
        return result;
    }

    std::string registerCacheReport()
    {
        std::ostringstream s;
        s << "register cache: " << g_functions.load() << " functions, " << g_fallbacks.load() << " kept the old code; "
          << "ctx register accesses in the old code " << g_oldReads.load() << " reads + " << g_oldWrites.load() << " writes; "
          << "on locals " << g_gprReads.load() << " GPR reads, " << g_gprWrites.load() << " GPR writes, "
          << g_fprReads.load() << " FPR reads, " << g_fprWrites.load() << " FPR writes; "
          << g_barriers.load() << " barriers; added " << g_loads.load() << " loads, " << g_flushStores.load()
          << " stores; sign extension known at " << g_sextReads.load() << " 64-bit reads and " << g_sextStores.load()
          << " stores (static counts)";
        return s.str();
    }
}
