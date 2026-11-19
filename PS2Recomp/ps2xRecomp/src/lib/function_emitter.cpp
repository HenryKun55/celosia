#include "ps2recomp/Emitters/function_emitter.h"
#include "ps2recomp/code_generator.h"
#include "ps2recomp/float_known.h"
#include "ps2recomp/gif_dma_kick_analyzer.h"
#include "ps2recomp/instructions.h"
#include "ps2recomp/r5900_decoder.h"
#include "ps2recomp/recompiler_reporter.h"
#include "ps2recomp/register_cache.h"
#include "ps2recomp/types.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace ps2recomp
{
    namespace
    {
        Instruction makeSyntheticDelaySlot(uint32_t address)
        {
            Instruction inst{};
            inst.address = address;
            inst.raw = 0;
            inst.opcode = OPCODE_SPECIAL;
            inst.function = SPECIAL_SLL;
            return inst;
        }
    }

    namespace
    {
        // Round N1: guest accesses of a base plus a MIPS immediate in the base form of ps2_runtime_macros.h
        // (READ32(ADD32(base, K)) -> READ32_B(base, k), WRITE32(ADD32(base, K), v) -> WRITE32_B(base, k, v); 8 to 64
        // bits). The macro tests the base alone, so every access off one base value shares the test (docs/decomp.md
        // "N1"; tools/n1_basemem.py is the same rewrite for tools/lift_d1.py). Exact: the off build
        // (PS2X_N1_BASEMEM=0) expands the _B forms back to the old text. PS2X_RECOMP_BASEMEM=0: not rewritten.
        bool parseImmediate(const std::string &k, int32_t &out)
        {
            if (k.empty())
                return false;
            std::string s = k;
            bool neg = false;
            if (s[0] == '-')
            {
                neg = true;
                s = s.substr(1);
            }
            if (!s.empty() && (s.back() == 'u' || s.back() == 'U'))
                s.pop_back();
            if (s.empty())
                return false;
            uint64_t v = 0;
            if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
            {
                for (size_t i = 2; i < s.size(); ++i)
                {
                    if (!std::isxdigit(static_cast<unsigned char>(s[i])))
                        return false;
                    v = v * 16u + static_cast<uint64_t>(std::isdigit(static_cast<unsigned char>(s[i])) ? s[i] - '0' : (std::tolower(s[i]) - 'a' + 10));
                    if (v > 0xFFFFFFFFull)
                        return false;
                }
            }
            else
            {
                for (char c : s)
                {
                    if (!std::isdigit(static_cast<unsigned char>(c)))
                        return false;
                    v = v * 10u + static_cast<uint64_t>(c - '0');
                    if (v > 0xFFFFFFFFull)
                        return false;
                }
            }
            int64_t sv = neg ? -static_cast<int64_t>(v) : static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(v)));
            if (sv < -0x8000 || sv > 0x7FFF)
                return false;
            out = static_cast<int32_t>(sv);
            return true;
        }

        // The end of a balanced expression starting at i: the first ',' or ')' at depth 0 (npos if none).
        size_t expressionEnd(const std::string &t, size_t i)
        {
            int depth = 0;
            for (; i < t.size(); ++i)
            {
                const char c = t[i];
                if (c == '(')
                    ++depth;
                else if (c == ')')
                {
                    if (depth == 0)
                        return i;
                    --depth;
                }
                else if (c == ',' && depth == 0)
                    return i;
                else if (c == '\n' || c == ';' || c == '"')
                    return std::string::npos;
            }
            return std::string::npos;
        }

        std::string applyBaseMem(const std::string &t)
        {
            std::string out;
            out.reserve(t.size());
            size_t i = 0;
            while (i < t.size())
            {
                const bool isRead = t.compare(i, 4, "READ") == 0;
                const bool isWrite = !isRead && t.compare(i, 5, "WRITE") == 0;
                if ((!isRead && !isWrite) || (i > 0 && (std::isalnum(static_cast<unsigned char>(t[i - 1])) || t[i - 1] == '_')))
                {
                    out += t[i++];
                    continue;
                }
                size_t j = i + (isRead ? 4 : 5);
                size_t d = j;
                while (d < t.size() && std::isdigit(static_cast<unsigned char>(t[d])))
                    ++d;
                const std::string size = t.substr(j, d - j);
                const std::string open = "(ADD32(";
                if ((size != "8" && size != "16" && size != "32" && size != "64") || t.compare(d, open.size(), open) != 0)
                {
                    out += t[i++];
                    continue;
                }
                const size_t baseStart = d + open.size();
                const size_t baseEnd = expressionEnd(t, baseStart);
                if (baseEnd == std::string::npos || t[baseEnd] != ',' || t.compare(baseEnd, 2, ", ") != 0)
                {
                    out += t[i++];
                    continue;
                }
                const size_t kStart = baseEnd + 2;
                const size_t kEnd = expressionEnd(t, kStart);
                int32_t off = 0;
                if (kEnd == std::string::npos || t[kEnd] != ')' || !parseImmediate(t.substr(kStart, kEnd - kStart), off))
                {
                    out += t[i++];
                    continue;
                }
                const std::string base = t.substr(baseStart, baseEnd - baseStart);
                if (isRead && kEnd + 1 < t.size() && t[kEnd + 1] == ')')
                {
                    out += "READ" + size + "_B(" + base + ", " + std::to_string(off) + ")";
                    i = kEnd + 2;
                }
                else if (isWrite && t.compare(kEnd + 1, 2, ", ") == 0)
                {
                    out += "WRITE" + size + "_B(" + base + ", " + std::to_string(off) + ", ";
                    i = kEnd + 3;
                }
                else
                    out += t[i++];
            }
            return out;
        }

        bool baseMemOn()
        {
            static const bool on = !(std::getenv("PS2X_RECOMP_BASEMEM") && std::getenv("PS2X_RECOMP_BASEMEM")[0] == '0');
            return on;
        }
    }

    FunctionEmitter::FunctionEmitter(CodeGenerator &codeGenerator)
        : m_codeGenerator(codeGenerator)
    {
    }

    namespace
    {
        // "goto label_<hex>;" in text: the label's address, or false.
        bool nextGoto(const std::string &text, size_t &pos, uint32_t &target)
        {
            static const std::string kGoto = "goto label_";
            while ((pos = text.find(kGoto, pos)) != std::string::npos)
            {
                size_t q = pos + kGoto.size();
                pos = q;
                uint32_t v = 0;
                size_t digits = 0;
                while (q < text.size() && std::isxdigit(static_cast<unsigned char>(text[q])) && !std::isupper(static_cast<unsigned char>(text[q])))
                {
                    v = v * 16u + static_cast<uint32_t>(std::isdigit(static_cast<unsigned char>(text[q])) ? text[q] - '0' : text[q] - 'a' + 10);
                    ++q;
                    ++digits;
                }
                if (digits == 0 || q >= text.size() || text[q] != ';')
                    continue;
                target = v;
                return true;
            }
            return false;
        }

        // A branch whose edges carry the provenance (round 30): conditional
        // branches without a link, B, J and JR (a jump table's cases). The
        // state on every edge out of it and on its fall-through is what is
        // known both before it and after its delay slot.
        bool carriesFloatState(const Instruction &inst)
        {
            switch (inst.opcode)
            {
            case OPCODE_BEQ:
            case OPCODE_BNE:
            case OPCODE_BLEZ:
            case OPCODE_BGTZ:
            case OPCODE_BEQL:
            case OPCODE_BNEL:
            case OPCODE_BLEZL:
            case OPCODE_BGTZL:
            case OPCODE_J:
                return true;
            case OPCODE_REGIMM:
                return inst.rt == REGIMM_BLTZ || inst.rt == REGIMM_BGEZ || inst.rt == REGIMM_BLTZL || inst.rt == REGIMM_BGEZL;
            case OPCODE_COP1:
                return inst.rs == COP1_BC;
            case OPCODE_SPECIAL:
                return inst.function == SPECIAL_JR;
            default:
                return false;
            }
        }

        // Unconditional transfers: nothing falls through.
        bool noFallThrough(const Instruction &inst)
        {
            return inst.opcode == OPCODE_J || (inst.opcode == OPCODE_SPECIAL && inst.function == SPECIAL_JR) ||
                   (inst.opcode == OPCODE_BEQ && inst.rs == 0 && inst.rt == 0);
        }
    }

    std::string FunctionEmitter::emit(
        const Function &function,
        const std::vector<Instruction> &instructions,
        bool useHeaders)
    {
        CodeGenerator &cg = m_codeGenerator;
        if (!cg.m_floatJoin || !(cg.m_vuKnown || cg.m_fpuKnown))
            return emitPass(function, instructions, useHeaders, false, nullptr, nullptr);
        // Round 30: a first pass (no provenance, no register cache) for the
        // gotos to each label: how many there are and how many come before
        // the label. A label whose gotos all come before it and are all
        // branch edges the second pass sees takes what its edges agree on.
        const std::string first = emitPass(function, instructions, useHeaders, true, nullptr, nullptr);
        GotoCounts before, all;
        std::unordered_map<uint32_t, size_t> labelAt;
        for (size_t pos = first.find("label_"); pos != std::string::npos; pos = first.find("label_", pos + 6))
        {
            size_t q = pos + 6;
            uint32_t v = 0;
            size_t digits = 0;
            while (q < first.size() && std::isxdigit(static_cast<unsigned char>(first[q])) && !std::isupper(static_cast<unsigned char>(first[q])))
            {
                v = v * 16u + static_cast<uint32_t>(std::isdigit(static_cast<unsigned char>(first[q])) ? first[q] - '0' : first[q] - 'a' + 10);
                ++q;
                ++digits;
            }
            if (digits != 0 && q < first.size() && first[q] == ':' && (pos == 0 || first[pos - 1] == '\n') && !labelAt.count(v))
                labelAt[v] = pos;
        }
        size_t pos = 0;
        uint32_t target = 0;
        while (nextGoto(first, pos, target))
        {
            ++all[target];
            auto it = labelAt.find(target);
            if (it != labelAt.end() && pos < it->second)
                ++before[target];
        }
        return emitPass(function, instructions, useHeaders, false, &before, &all);
    }

    std::string FunctionEmitter::emitPass(
        const Function &function,
        const std::vector<Instruction> &instructions,
        bool useHeaders,
        bool firstPass,
        const GotoCounts *gotosBefore,
        const GotoCounts *gotosAll)
    {
        CodeGenerator &cg = m_codeGenerator;
        std::stringstream ss;
        cg.m_currentFunctionName = function.name;

        if (useHeaders)
        {
            ss << "#include <stdexcept>\n";
            ss << "#include \"ps2_runtime_macros.h\"\n";
            ss << "#include \"ps2_runtime.h\"\n";
            ss << "#include <ps2_recompiled_functions.h>\n";
            ss << "#include <ps2_recompiled_stubs.h>\n\n";
            ss << "#include \"ps2_syscalls.h\"\n";
            ss << "#include \"ps2_stubs.h\"\n\n";
            ss << "#ifdef PS2_FUNCTION_LOG_TRACKER\n";
            ss << "#include \"ps2_log.h\"\n";
            ss << "#endif\n\n";
        }

        CodeGenerator::AnalysisResult analysisResult = cg.collectInternalBranchTargets(function, instructions);
        std::vector<uint32_t> resumeTargets(analysisResult.resumeEntryPoints.begin(),
                                            analysisResult.resumeEntryPoints.end());
        auto resumeIt = cg.m_resumeEntryTargetsByOwner.find(function.start);
        if (resumeIt != cg.m_resumeEntryTargetsByOwner.end())
        {
            resumeTargets.insert(resumeTargets.end(), resumeIt->second.begin(), resumeIt->second.end());
        }
        std::sort(resumeTargets.begin(), resumeTargets.end());
        resumeTargets.erase(std::unique(resumeTargets.begin(), resumeTargets.end()), resumeTargets.end());
        for (uint32_t target : resumeTargets)
        {
            analysisResult.entryPoints.insert(target);
        }

        const std::unordered_set<uint32_t> &internalTargets = analysisResult.entryPoints;
        ConstantRegisterState constantRegisters;
        GifDmaKickPlan gifDmaKickPlan{};
        ss << "// Function: " << function.name << "\n";
        ss << "// Address: 0x" << std::hex << function.start << " - 0x" << function.end << std::dec << "\n";

        std::string sanitizedName = cg.getFunctionName(function.start);
        if (sanitizedName.empty())
        {
            std::stringstream nameBuilder;
            nameBuilder << "Errorfunc_" << std::hex << function.start;
            sanitizedName = nameBuilder.str();
        }

        ss << "void " << sanitizedName << "(uint8_t* rdram, R5900Context* ctx, PS2Runtime *runtime) {\n";
        ss << "#ifdef PS2_FUNCTION_LOG_TRACKER\n";
        ss << "    PS_LOG_ENTRY(\"" << sanitizedName << "\");\n";
        ss << "#endif\n";
        // Everything from here to the closing brace is the body the register
        // cache rewrites (CodeGenerator::m_registerCache).
        const size_t bodyStart = ss.str().size();
        ss << "\n";
        if (!resumeTargets.empty() && cg.leanCalls())
        {
            // The usual entry (a call: ctx->pc is the start) skips the resume
            // switch and the start's store (round 16).
            ss << "    if (__builtin_expect(ctx->pc != 0x" << std::hex << function.start << std::dec << "u, 0)) {\n";
            ss << "        switch (ctx->pc) {\n";
            for (uint32_t target : resumeTargets)
            {
                ss << "            case 0x" << std::hex << target << "u: goto label_" << target << ";\n"
                   << std::dec;
            }
            ss << "            default: ctx->pc = 0x" << std::hex << function.start << std::dec << "u; break;\n";
            ss << "        }\n";
            ss << "    }\n\n";
        }
        else if (!resumeTargets.empty())
        {
            ss << "    switch (ctx->pc) {\n";
            for (uint32_t target : resumeTargets)
            {
                ss << "        case 0x" << std::hex << target << "u: goto label_" << target << ";\n"
                   << std::dec;
            }
            ss << "        default: break;\n";
            ss << "    }\n\n";
        }
        if (resumeTargets.empty() || !cg.leanCalls())
        {
            ss << "    ctx->pc = 0x" << std::hex << function.start << "u;\n"
               << std::dec;
            ss << "\n";
        }

        bool lastInstructionWasControlFlow = false;
        // Round 29: VU0 / FPU provenance along straight-line runs.
        FloatKnownState floatKnown(cg.m_vuKnown && !firstPass, cg.m_fpuKnown && !firstPass);
        floatKnown.setLoadsKeepVu(cg.m_vuLoads);
        // Round 30: the state each label's forward branch edges bring (what
        // they all know), the edges counted; a label is poisoned by an edge
        // whose state is not known.
        const bool carry = gotosBefore != nullptr && gotosAll != nullptr && floatKnown.enabled();
        std::unordered_map<uint32_t, FloatKnownState> edgeState;
        std::unordered_map<uint32_t, int> edgesSeen;
        std::unordered_set<uint32_t> poisoned;
        auto recordEdges = [&](const std::string &text, const FloatKnownState *state)
        {
            size_t pos = 0;
            uint32_t target = 0;
            while (nextGoto(text, pos, target))
            {
                ++edgesSeen[target];
                if (!state)
                {
                    poisoned.insert(target);
                    continue;
                }
                auto it = edgeState.find(target);
                if (it == edgeState.end())
                    edgeState.emplace(target, *state);
                else
                    it->second.intersect(*state);
            }
        };

        for (size_t i = 0; i < instructions.size(); ++i)
        {
            const Instruction &inst = instructions[i];
            lastInstructionWasControlFlow = inst.hasDelaySlot;

            if (internalTargets.contains(inst.address))
            {
                constantRegisters.clear();
                // A label whose gotos all came before it from branches seen
                // here: the fall-through's state and its edges' agree.
                bool joined = false;
                if (carry && !poisoned.contains(inst.address))
                {
                    const auto all = gotosAll->find(inst.address);
                    const auto before = gotosBefore->find(inst.address);
                    const int nAll = all == gotosAll->end() ? 0 : all->second;
                    const int nBefore = before == gotosBefore->end() ? 0 : before->second;
                    const auto seen = edgesSeen.find(inst.address);
                    const int nSeen = seen == edgesSeen.end() ? 0 : seen->second;
                    if (nAll == nBefore && nSeen == nAll)
                    {
                        const auto e = edgeState.find(inst.address);
                        if (e != edgeState.end())
                            floatKnown.intersect(e->second);
                        else if (nAll != 0)
                            floatKnown.reset();
                        joined = true;
                        floatKnown.countJoin(nAll != 0);
                    }
                }
                if (!joined)
                    floatKnown.reset();
                ss << "label_" << std::hex << inst.address << std::dec << ":\n";
            }

            if (cg.m_emitInstructionComments)
            {
                ss << "    // 0x" << std::hex << inst.address << ": 0x" << inst.raw << std::dec;
                std::string disassembly = R5900Decoder::disassembleInstruction(inst);
                if (!disassembly.empty())
                {
                    ss << "  " << disassembly;
                }
                ss << "\n";
            }

            try
            {
                if (inst.hasDelaySlot)
                {
                    // Round 30: the state before the branch (nothing if its
                    // delay slot has a label of its own).
                    FloatKnownState beforeBranch = floatKnown;
                    floatKnown.reset();
                    const bool hasDecodedDelaySlot =
                        i + 1 < instructions.size() &&
                        instructions[i + 1].address == inst.address + 4u;

                    Instruction syntheticDelaySlot{};
                    const Instruction *delaySlot = nullptr;
                    if (hasDecodedDelaySlot)
                    {
                        delaySlot = &instructions[i + 1];
                    }
                    else
                    {
                        syntheticDelaySlot = makeSyntheticDelaySlot(inst.address + 4u);
                        delaySlot = &syntheticDelaySlot;
                    }

                    if (hasDecodedDelaySlot && internalTargets.contains(delaySlot->address))
                    {
                        ss << "label_" << std::hex << delaySlot->address << std::dec << ":\n";
                    }

                    const bool delayLabel = hasDecodedDelaySlot && internalTargets.contains(delaySlot->address);
                    if (gifDmaKickPlan.valid &&
                        gifDmaKickPlan.completesInDelaySlot &&
                        gifDmaKickPlan.branchIndex == i &&
                        hasDecodedDelaySlot)
                    {
                        const std::string text = cg.handleBranchDelaySlots(
                            inst,
                            *delaySlot,
                            function,
                            analysisResult,
                            gifDmaDelaySlotOverride(*delaySlot, gifDmaKickPlan, cg.m_emitInstructionComments));
                        if (carry)
                            recordEdges(text, nullptr);
                        ss << text;
                        gifDmaKickPlan = {};
                    }
                    else
                    {
                        const std::string text = cg.handleBranchDelaySlots(inst, *delaySlot, function, analysisResult);
                        // Round 30: the edges' state, what is known before
                        // the branch and after its delay slot both (a likely
                        // branch's fall-through skips the slot). Any call,
                        // jump or helper in the text: nothing.
                        bool carried = false;
                        if (carry && carriesFloatState(inst) && !delayLabel)
                        {
                            FloatKnownState afterSlot = beforeBranch;
                            (void)afterSlot.apply(cg.translateInstruction(*delaySlot));
                            afterSlot.intersect(beforeBranch);
                            if (!FloatKnownState::hasTransfer(text))
                            {
                                recordEdges(text, &afterSlot);
                                if (!noFallThrough(inst))
                                {
                                    floatKnown = afterSlot;
                                    floatKnown.countBranch();
                                }
                                carried = true;
                            }
                        }
                        if (carry && !carried)
                            recordEdges(text, nullptr);
                        ss << text;
                    }

                    if (hasDecodedDelaySlot)
                    {
                        ++i; // Skip delay slot instruction (handled inside branch logic)
                    }
                    constantRegisters.clear();
                }
                else
                {
                    if (!gifDmaKickPlan.valid)
                    {
                        gifDmaKickPlan = tryBuildGifDmaKickPlan(instructions, i, constantRegisters, internalTargets);
                    }

                    if (gifDmaKickPlan.suppresses(i))
                    {
                        floatKnown.reset();
                        const size_t slot = gifDmaKickPlan.slotFor(i);
                        emitGifDmaCapture(ss, gifDmaKickPlan, slot, "    ");

                        if (gifDmaKickPlan.completesAt(i))
                        {
                            ss << "    ctx->pc = 0x" << std::hex << inst.address << "u;\n"
                               << std::dec;
                            ss << "    " << gifDmaKickCall(gifDmaKickPlan) << "\n";
                            gifDmaKickPlan = {};
                        }

                        updateConstantRegisters(inst, constantRegisters);
                        continue;
                    }

                    const MemoryAccessHint memoryHint = resolveMemoryAccessHint(inst, constantRegisters);
                    const std::string code = floatKnown.apply(cg.translateInstruction(inst, memoryHint));
                    if (inst.opcode == OPCODE_COP1 && inst.rs == COP1_MT)
                    {
                        // MTC1 of a known GPR: $zero, or the LUI right before it
                        // (no label between and not a delay slot: the LUI ran).
                        if (inst.rt == 0)
                            floatKnown.fprFromConstant(static_cast<int>(inst.rd), 0u);
                        else if (i > 0 && instructions[i - 1].opcode == OPCODE_LUI && instructions[i - 1].rt == inst.rt &&
                                 instructions[i - 1].address + 4u == inst.address && !internalTargets.contains(inst.address) &&
                                 !(i >= 2 && instructions[i - 2].hasDelaySlot))
                            floatKnown.fprFromConstant(static_cast<int>(inst.rd), static_cast<uint32_t>(instructions[i - 1].immediate) << 16);
                    }
                    if (cg.m_allPcStores || CodeGenerator::codeReadsPc(code))
                    {
                        ss << "    ctx->pc = 0x" << std::hex << inst.address << "u;\n"
                           << std::dec;
                    }
                    ss << "    " << code;
                    if (inst.isMmio)
                    {
                        ss << " // MMIO: 0x" << std::hex << inst.mmioAddress << std::dec;
                    }
                    ss << "\n";

                    updateConstantRegisters(inst, constantRegisters);
                }
            }
            catch (const std::exception &e)
            {
                if (cg.m_reporter)
                {
                    std::ostringstream msg;
                    msg << "translation failed: " << e.what() << " raw=0x" << std::hex << inst.raw;
                    cg.m_reporter->errorAt("codegen", function.name, inst.address, msg.str());
                }

                throw;
            }
        }

        // Fallthrough with no terminating branch: publish the next PC so the EE dispatcher does not re-enter this function.
        if (!instructions.empty() && !lastInstructionWasControlFlow)
        {
            ss << "    ctx->pc = 0x" << std::hex << function.end << "u;\n"
               << std::dec;
        }
        // Execution runs on past the function's end (into the next one).
        if (!instructions.empty())
        {
            ss << "    runtime->jumpGuest(rdram, ctx, ctx->pc);\n";
        }

        if (cg.m_registerCache && !firstPass)
        {
            const std::string all = ss.str();
            RegisterCacheOptions options;
            options.fprs = cg.m_registerCacheFprs;
            options.sext = cg.m_registerCacheSext;
            options.blockScope = cg.m_registerCacheBlock;
            options.writeThrough = cg.m_registerCacheWriteThrough;
            options.deadStores = cg.m_registerCacheDeadStores;
            options.joinForward = cg.m_registerCacheJoin;
            options.joinMem = cg.m_registerCacheJoinMem;
            const std::string body = applyRegisterCache(all.substr(bodyStart), options);
            return all.substr(0, bodyStart) + (baseMemOn() ? applyBaseMem(body) : body) + "}\n";
        }
        ss << "}\n";
        return firstPass || !baseMemOn() ? ss.str() : applyBaseMem(ss.str());
    }
}
