#include "ps2recomp/Emitters/control_flow_emitter.h"

#include "ps2recomp/control_flow_utils.h"
#include "ps2recomp/instructions.h"
#include "ps2recomp/r5900_decoder.h"
#include "ps2_runtime_calls.h"

#include <algorithm>
#include <fmt/format.h>
#include <sstream>
#include <utility>

namespace ps2recomp
{
    ControlFlowEmitter::ControlFlowEmitter(CodeGenerator &generator,
                                           const Instruction &branchInst,
                                           const Instruction &delaySlot,
                                           const Function &function,
                                           const CodeGenerator::AnalysisResult &analysisResult,
                                           std::string delaySlotOverride)
        : m_gen(generator),
          m_branchInst(branchInst),
          m_delaySlot(delaySlot),
          m_function(function),
          m_analysisResult(analysisResult),
          m_delaySlotOverride(std::move(delaySlotOverride))
    {
    }

    uint32_t ControlFlowEmitter::branchPc() const
    {
        return m_branchInst.address;
    }

    uint32_t ControlFlowEmitter::delayPc() const
    {
        return m_branchInst.address + 4u;
    }

    uint32_t ControlFlowEmitter::fallthroughPc() const
    {
        return m_branchInst.address + 8u;
    }

    bool ControlFlowEmitter::hasRealDelaySlot() const
    {
        return !m_delaySlotOverride.empty() || !isGuestNop(m_delaySlot);
    }

    bool ControlFlowEmitter::isCallLikeEdge() const
    {
        return (m_branchInst.opcode == OPCODE_JAL) ||
               (m_branchInst.opcode == OPCODE_SPECIAL && m_branchInst.function == SPECIAL_JALR);
    }

    bool ControlFlowEmitter::isInternalTarget(uint32_t target) const
    {
        return m_analysisResult.entryPoints.contains(target);
    }

    bool ControlFlowEmitter::isLikelyBranch() const
    {
        return (m_branchInst.opcode == OPCODE_BEQL || m_branchInst.opcode == OPCODE_BNEL ||
                m_branchInst.opcode == OPCODE_BLEZL || m_branchInst.opcode == OPCODE_BGTZL ||
                (m_branchInst.opcode == OPCODE_REGIMM &&
                 (m_branchInst.rt == REGIMM_BLTZL || m_branchInst.rt == REGIMM_BGEZL ||
                  m_branchInst.rt == REGIMM_BLTZALL || m_branchInst.rt == REGIMM_BGEZALL)) ||
                (m_branchInst.opcode == OPCODE_COP1 && m_branchInst.rs == COP1_BC &&
                 (m_branchInst.rt == COP1_BC_BCFL || m_branchInst.rt == COP1_BC_BCTL)) ||
                (m_branchInst.opcode == OPCODE_COP2 && m_branchInst.rs == COP2_BC &&
                 (m_branchInst.rt == COP2_BC_BCFL || m_branchInst.rt == COP2_BC_BCTL)));
    }

    std::vector<uint32_t> ControlFlowEmitter::resolvedLocalIndirectTargets() const
    {
        if (m_branchInst.opcode != OPCODE_SPECIAL)
        {
            return {};
        }

        if (!((m_branchInst.function == SPECIAL_JR && m_branchInst.rs != 31u) ||
              m_branchInst.function == SPECIAL_JALR))
        {
            return {};
        }

        auto jtIt = m_analysisResult.jumpTableTargets.find(m_branchInst.address);
        if (jtIt == m_analysisResult.jumpTableTargets.end())
        {
            return {};
        }

        std::vector<uint32_t> targets = jtIt->second;
        std::sort(targets.begin(), targets.end());
        targets.erase(std::unique(targets.begin(), targets.end()), targets.end());
        return targets;
    }

    std::string ControlFlowEmitter::delaySlotCode() const
    {
        if (!hasRealDelaySlot())
        {
            return {};
        }

        if (!m_delaySlotOverride.empty())
        {
            return m_delaySlotOverride;
        }

        std::string code;
        if (m_gen.m_emitInstructionComments)
        {
            code = "// 0x" + fmt::format("{:x}", m_delaySlot.address) + ": 0x" + fmt::format("{:x}", m_delaySlot.raw);
            std::string disassembly = R5900Decoder::disassembleInstruction(m_delaySlot);
            if (!disassembly.empty())
            {
                code += "  " + disassembly;
            }
            code += " (Delay Slot)\n";
        }

        code += m_gen.translateInstruction(m_delaySlot);
        return code;
    }

    void ControlFlowEmitter::emitDelaySlot(std::string_view indent)
    {
        if (!hasRealDelaySlot())
        {
            return;
        }

        const std::string code = delaySlotCode();
        // The delay slot state is for an exception the slot raises: kept
        // only when it can raise one (see CodeGenerator::codeReadsPc).
        const bool slotState = m_gen.m_allPcStores || CodeGenerator::codeReadsPc(code);
        if (slotState)
        {
            m_ss << fmt::format("{}ctx->pc = 0x{:X}u;\n", indent, delayPc());
            m_ss << fmt::format("{}ctx->in_delay_slot = true;\n", indent);
            m_ss << fmt::format("{}ctx->branch_pc = 0x{:X}u;\n", indent, branchPc());
        }

        std::istringstream lines(code);
        std::string line;
        while (std::getline(lines, line))
        {
            if (!line.empty())
            {
                m_ss << indent << line << "\n";
            }
        }

        if (slotState)
        {
            m_ss << fmt::format("{}ctx->in_delay_slot = false;\n", indent);
        }
    }

    void ControlFlowEmitter::emitResumeFromDelaySlotEntry()
    {
        if (!isInternalTarget(delayPc()))
        {
            return;
        }

        m_ss << fmt::format("    if (ctx->pc == 0x{:X}u) {{\n", delayPc());
        emitDelaySlot("        ");
        m_ss << fmt::format("        ctx->pc = 0x{:X}u;\n", fallthroughPc());

        if (isInternalTarget(fallthroughPc()))
        {
            m_ss << fmt::format("        goto label_{:x};\n", fallthroughPc());
        }
        else
        {
            m_ss << fmt::format("        goto label_fallthrough_0x{:x};\n", branchPc());
        }

        m_ss << "    }\n";
    }

    void ControlFlowEmitter::emitInternalTarget(uint32_t target, uint32_t sourcePc, std::string_view indent)
    {
        const bool backEdge = target <= sourcePc && !isCallLikeEdge();
        // Round 30: the back edge's pc store inside the checkpoint's slow
        // path (PS2_CHECKPOINT_DUE_AT); PS2X_RECOMP_LOOP_PC=0 (or
        // PS2X_RECOMP_ALL_PC=1): stored before the test, as before.
        const bool lazyPc = backEdge && m_gen.m_lazyLoopPc && !m_gen.m_allPcStores;
        if (m_gen.m_allPcStores || (backEdge && !lazyPc))
        {
            m_ss << fmt::format("{}ctx->pc = 0x{:X}u;\n", indent, target);
        }
        if (lazyPc)
        {
            m_ss << fmt::format("{}if (PS2_CHECKPOINT_DUE_AT(0x{:X}u)) {{\n", indent, target);
            m_ss << fmt::format("{}    return;\n", indent);
            m_ss << fmt::format("{}}}\n", indent);
        }
        else if (backEdge)
        {
            m_ss << fmt::format("{}if (runtime->eeCheckpointDue()) {{\n", indent);
            m_ss << fmt::format("{}    return;\n", indent);
            m_ss << fmt::format("{}}}\n", indent);
        }
        m_ss << fmt::format("{}goto label_{:x};\n", indent, target);
    }

    void ControlFlowEmitter::emitRuntimeBranchDispatch(std::string_view targetExpression,
                                                       uint32_t sourcePc,
                                                       uint32_t returnPc,
                                                       std::string_view runtimeKind,
                                                       std::string_view debugName,
                                                       std::string_view indent,
                                                       bool returnOnTransfer)
    {
        // Calls run as host calls through the function table (an inline
        // checkpoint, no scheduler round trip); jumps out of the function run
        // nested when they can (PS2Runtime::callGuest / jumpGuest).
        if ((runtimeKind == "DirectCall" || runtimeKind == "IndirectCall") && m_gen.leanCalls())
        {
            m_ss << fmt::format("{}if (!PS2_CALL_GUEST({}, 0x{:X}u, 0x{:X}u, {}, \"{}\")) {{\n",
                                indent, targetExpression, returnPc, sourcePc, runtimeKind, debugName);
            m_ss << fmt::format("{}    return;\n", indent);
            m_ss << fmt::format("{}}}\n", indent);
            return;
        }
        if ((runtimeKind == "IndirectJump" || runtimeKind == "DirectJump") && m_gen.leanCalls())
        {
            m_ss << fmt::format("{}PS2_JUMP_GUEST({});\n", indent, targetExpression);
            m_ss << fmt::format("{}return;\n", indent);
            return;
        }
        if (runtimeKind == "DirectCall" || runtimeKind == "IndirectCall")
        {
            m_ss << fmt::format(
                "{}if (!runtime->callGuest(rdram, ctx, {}, 0x{:X}u, 0x{:X}u, PS2Runtime::GuestBranchKind::{}, \"{}\")) {{\n",
                indent, targetExpression, returnPc, sourcePc, runtimeKind, debugName);
            m_ss << fmt::format("{}    return;\n", indent);
            m_ss << fmt::format("{}}}\n", indent);
            return;
        }
        if (runtimeKind == "IndirectJump" || runtimeKind == "DirectJump")
        {
            m_ss << fmt::format("{}runtime->jumpGuest(rdram, ctx, {});\n", indent, targetExpression);
            m_ss << fmt::format("{}return;\n", indent);
            return;
        }
        m_ss << fmt::format(
            "{}if (!runtime->dispatchGuestBranch(rdram, ctx, {}, 0x{:X}u, 0x{:X}u, PS2Runtime::GuestBranchKind::{}, \"{}\")) {{\n",
            indent,
            targetExpression,
            sourcePc,
            returnPc,
            runtimeKind,
            debugName);
        if (returnOnTransfer)
        {
            m_ss << fmt::format("{}    return;\n", indent);
        }
        m_ss << fmt::format("{}}}\n", indent);
    }

    bool ControlFlowEmitter::emitDirectFunctionJumpIfAvailable(uint32_t target, StaticBranchKind kind, std::string_view indent)
    {
        if (kind != StaticBranchKind::Jump)
        {
            return false;
        }

        const std::string functionName = m_gen.getFunctionName(target);
        if (functionName.empty())
        {
            return false;
        }

        if (m_gen.m_tailTable)
        {
            // Round 43: through the table's slot (natives at tail-jump targets run).
            m_ss << fmt::format("{}PS2_TAIL_GUEST_KNOWN(0x{:X}u, {}); return;\n", indent, target, functionName);
            return true;
        }
        m_ss << indent << functionName << "(rdram, ctx, runtime); return;\n";
        return true;
    }

    void ControlFlowEmitter::emitExternalJumpDispatch(uint32_t target, StaticBranchKind kind, std::string_view indent)
    {
        const bool isCall = kind == StaticBranchKind::Call;
        // A JAL to a generated function: the table's slot is still what runs
        // (hooks and natives replace slots), but when it holds that function
        // the call is a direct one (PS2_CALL_GUEST_KNOWN), not through a
        // register (round 16).
        if (isCall && m_gen.leanCalls())
        {
            const std::string functionName = m_gen.getFunctionName(target);
            if (!functionName.empty())
            {
                m_ss << fmt::format("{}if (!PS2_CALL_GUEST_KNOWN(0x{:X}u, {}, 0x{:X}u, 0x{:X}u, DirectCall, \"JAL\")) {{\n",
                                    indent, target, functionName, fallthroughPc(), branchPc());
                m_ss << fmt::format("{}    return;\n", indent);
                m_ss << fmt::format("{}}}\n", indent);
                return;
            }
        }
        emitRuntimeBranchDispatch(fmt::format("0x{:X}u", target),
                                  branchPc(),
                                  isCall ? fallthroughPc() : 0u,
                                  isCall ? "DirectCall" : "DirectJump",
                                  isCall ? "JAL" : "J",
                                  indent,
                                  true);
    }

    void ControlFlowEmitter::emitExternalRegisterCallDispatch(std::string_view jumpTargetExpression, std::string_view indent)
    {
        emitRuntimeBranchDispatch(jumpTargetExpression,
                                  branchPc(),
                                  fallthroughPc(),
                                  "IndirectCall",
                                  "JALR",
                                  indent,
                                  true);
    }

    void ControlFlowEmitter::emitExternalRegisterJumpDispatch(std::string_view jumpTargetExpression,
                                                              RegisterBranchKind kind,
                                                              uint8_t rsReg,
                                                              std::string_view indent)
    {
        const bool isReturn = kind == RegisterBranchKind::Jump && rsReg == 31u;

        if (isReturn)
        {
            m_ss << indent << "#if defined(PS2X_STRICT_RETURN_DIAGNOSTICS) && PS2X_STRICT_RETURN_DIAGNOSTICS\n";
            m_ss << indent << "(void)runtime->dispatchGuestBranch(rdram, ctx, " << jumpTargetExpression
                 << ", 0x" << fmt::format("{:X}", branchPc())
                 << "u, 0u, PS2Runtime::GuestBranchKind::Return, \"JR $ra\");\n";
            m_ss << indent << "return;\n";
            m_ss << indent << "#else\n";
            m_ss << indent << "ctx->pc = " << jumpTargetExpression << ";\n";
            m_ss << indent << "return;\n";
            m_ss << indent << "#endif\n";
            return;
        }

        emitRuntimeBranchDispatch(jumpTargetExpression,
                                  branchPc(),
                                  0u,
                                  "IndirectJump",
                                  "JR",
                                  indent,
                                  true);
    }

    bool ControlFlowEmitter::emitRelocationCallIfAvailable(StaticBranchKind kind, std::string_view indent)
    {
        const auto relocIt = m_gen.m_relocationCallNames.find(m_branchInst.address);
        if (relocIt == m_gen.m_relocationCallNames.end() || relocIt->second.empty())
        {
            return false;
        }

        const std::string_view resolvedSyscallName = ps2_runtime_calls::resolveSyscallName(relocIt->second);
        const std::string_view resolvedStubName = ps2_runtime_calls::resolveStubName(relocIt->second);
        if (resolvedSyscallName.empty() && resolvedStubName.empty())
        {
            return false;
        }

        const bool isSyscall = !resolvedSyscallName.empty();
        const std::string_view handlerName = isSyscall ? resolvedSyscallName : resolvedStubName;

        m_ss << indent << "{\n";
        if (kind == StaticBranchKind::Call)
        {
            m_ss << fmt::format("{}    ctx->pc = 0x{:X}u;\n", indent, fallthroughPc());
        }
        else
        {
            m_ss << indent << "    ctx->pc = getRegU32(ctx, 31);\n";
        }
        m_ss << indent << "    " << (isSyscall ? "ps2_syscalls::" : "ps2_stubs::")
             << handlerName << "(rdram, ctx, runtime);\n";
        m_ss << indent << "}\n";

        if (kind == StaticBranchKind::Jump)
        {
            m_ss << indent << "return;\n";
        }
        else
        {
            m_ss << fmt::format("{}if (ctx->pc != 0x{:X}u) {{ return; }}\n", indent, fallthroughPc());
        }

        return true;
    }

    void ControlFlowEmitter::emitStaticJump(StaticBranchKind kind)
    {
        if (m_gen.isDynamicImmediate(m_branchInst.address))
        {
            // The guest linker rewrites this J/JAL when it binds an imported symbol,
            // so decode the target from the live instruction word.
            m_ss << "    {\n";
            m_ss << fmt::format("        const uint32_t jumpTarget = 0x{:X}u | ((READ32(0x{:X}u) & 0x3FFFFFFu) << 2);\n",
                                (m_branchInst.address + 4u) & 0xF0000000u, m_branchInst.address);
            if (kind == StaticBranchKind::Call)
            {
                m_ss << fmt::format("        SET_GPR_U32(ctx, 31, 0x{:X}u);\n", fallthroughPc());
            }
            emitDelaySlot("        ");
            m_ss << "        ctx->pc = jumpTarget;\n";
            // The word as the ELF has it (the captured, linked image) names a
            // generated function: a direct call while the live word still
            // finds that function in the table (round 16).
            const uint32_t imageTarget = buildAbsoluteJumpTarget(m_branchInst.address, m_branchInst.target);
            const std::string imageFunction = m_gen.leanCalls() ? m_gen.getFunctionName(imageTarget) : std::string();
            if (kind == StaticBranchKind::Call && !imageFunction.empty())
            {
                m_ss << fmt::format("        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, {}, 0x{:X}u, 0x{:X}u, IndirectCall, \"JALR\")) {{\n",
                                    imageFunction, fallthroughPc(), branchPc());
                m_ss << "            return;\n";
                m_ss << "        }\n";
            }
            else if (kind == StaticBranchKind::Call)
            {
                emitExternalRegisterCallDispatch("jumpTarget", "        ");
            }
            else
            {
                emitExternalRegisterJumpDispatch("jumpTarget", RegisterBranchKind::Jump, 0u, "        ");
            }
            m_ss << "    }\n";
            return;
        }

        if (kind == StaticBranchKind::Call)
        {
            m_ss << fmt::format("    SET_GPR_U32(ctx, 31, 0x{:X}u);\n", fallthroughPc());
        }

        emitDelaySlot("    ");

        const uint32_t target = buildAbsoluteJumpTarget(m_branchInst.address, m_branchInst.target);
        // A JAL into this function (recursion, a call to one of its entries)
        // is a call: a goto would come back through the callee's JR $ra as a
        // return from this whole function, out to the scheduler.
        if (kind != StaticBranchKind::Call && isInternalTarget(target))
        {
            emitInternalTarget(target, branchPc(), "    ");
            return;
        }

        m_ss << fmt::format("    ctx->pc = 0x{:X}u;\n", target);

        if (kind == StaticBranchKind::Call && emitRelocationCallIfAvailable(kind, "    "))
        {
            return;
        }

        if (emitDirectFunctionJumpIfAvailable(target, kind, "    "))
        {
            return;
        }

        emitExternalJumpDispatch(target, kind, "    ");
    }

    void ControlFlowEmitter::emitRegisterJump(RegisterBranchKind kind)
    {
        const uint8_t rsReg = static_cast<uint8_t>(m_branchInst.rs);
        const uint8_t rdReg = static_cast<uint8_t>(m_branchInst.rd);
        const std::vector<uint32_t> sortedInternalTargets = resolvedLocalIndirectTargets();

        m_ss << "    {\n";
        m_ss << "        const uint32_t jumpTarget = GPR_U32(ctx, " << static_cast<int>(rsReg) << ");\n";

        if (kind == RegisterBranchKind::Call && rdReg != 0u)
        {
            m_ss << fmt::format("        SET_GPR_U32(ctx, {}, 0x{:X}u);\n", rdReg, fallthroughPc());
        }

        emitDelaySlot("        ");
        m_ss << "        ctx->pc = jumpTarget;\n";

        if (!sortedInternalTargets.empty())
        {
            m_ss << "        switch (jumpTarget) {\n";
            for (uint32_t target : sortedInternalTargets)
            {
                m_ss << fmt::format("            case 0x{:X}u: goto label_{:x};\n", target, target);
            }
            m_ss << "            default: break;\n";
            m_ss << "        }\n";
        }

        if (kind == RegisterBranchKind::Jump)
        {
            emitExternalRegisterJumpDispatch("jumpTarget", kind, rsReg, "        ");
        }
        else
        {
            emitExternalRegisterCallDispatch("jumpTarget", "        ");
        }

        m_ss << "    }\n";
    }

    std::string ControlFlowEmitter::conditionalBranchExpression() const
    {
        const uint8_t rsReg = static_cast<uint8_t>(m_branchInst.rs);
        const uint8_t rtReg = static_cast<uint8_t>(m_branchInst.rt);

        switch (m_branchInst.opcode)
        {
        case OPCODE_BEQ:
        case OPCODE_BEQL:
            return fmt::format("GPR_U64(ctx, {}) == GPR_U64(ctx, {})", rsReg, rtReg);
        case OPCODE_BNE:
        case OPCODE_BNEL:
            return fmt::format("GPR_U64(ctx, {}) != GPR_U64(ctx, {})", rsReg, rtReg);
        case OPCODE_BLEZ:
        case OPCODE_BLEZL:
            return fmt::format("GPR_S32(ctx, {}) <= 0", rsReg);
        case OPCODE_BGTZ:
        case OPCODE_BGTZL:
            return fmt::format("GPR_S32(ctx, {}) > 0", rsReg);
        case OPCODE_REGIMM:
            switch (m_branchInst.rt)
            {
            case REGIMM_BLTZ:
            case REGIMM_BLTZL:
            case REGIMM_BLTZAL:
            case REGIMM_BLTZALL:
                return fmt::format("GPR_S32(ctx, {}) < 0", rsReg);
            case REGIMM_BGEZ:
            case REGIMM_BGEZL:
            case REGIMM_BGEZAL:
            case REGIMM_BGEZALL:
                return fmt::format("GPR_S32(ctx, {}) >= 0", rsReg);
            default:
                return "false";
            }
        case OPCODE_COP1:
            if (m_branchInst.rs == COP1_BC)
            {
                const uint8_t bcCond = static_cast<uint8_t>(m_branchInst.rt);
                return (bcCond == COP1_BC_BCF || bcCond == COP1_BC_BCFL)
                           ? "!(ctx->fcr31 & 0x800000)"
                           : "(ctx->fcr31 & 0x800000)";
            }
            break;
        case OPCODE_COP2:
            if (m_branchInst.rs == COP2_BC)
            {
                const uint8_t bcCond = static_cast<uint8_t>(m_branchInst.rt);
                return (bcCond == COP2_BC_BCF || bcCond == COP2_BC_BCFL)
                           ? "!(ctx->vu0_status & 0x1)"
                           : "(ctx->vu0_status & 0x1)";
            }
            break;
        default:
            break;
        }

        return "false";
    }

    uint32_t ControlFlowEmitter::conditionalBranchTarget() const
    {
        const int32_t offsetBytes = static_cast<int32_t>(static_cast<int16_t>(m_branchInst.simmediate)) << 2;
        return static_cast<uint32_t>(static_cast<int64_t>(m_branchInst.address + 4u) +
                                     static_cast<int64_t>(offsetBytes));
    }

    void ControlFlowEmitter::emitConditionalBranch()
    {
        const uint32_t target = conditionalBranchTarget();
        const bool likely = isLikelyBranch();
        const std::string branchTakenVar = fmt::format("branch_taken_0x{:x}", m_branchInst.address);
        std::string unconditionalLinkCode;
        std::string conditionalLinkCode;

        if (m_branchInst.opcode == OPCODE_REGIMM)
        {
            if (m_branchInst.rt == REGIMM_BLTZAL || m_branchInst.rt == REGIMM_BGEZAL)
            {
                unconditionalLinkCode = fmt::format("SET_GPR_U32(ctx, 31, 0x{:X}u);", fallthroughPc());
            }
            else if (m_branchInst.rt == REGIMM_BLTZALL || m_branchInst.rt == REGIMM_BGEZALL)
            {
                conditionalLinkCode = fmt::format("SET_GPR_U32(ctx, 31, 0x{:X}u);", fallthroughPc());
            }
        }

        m_ss << "    {\n";
        m_ss << "        const bool " << branchTakenVar << " = (" << conditionalBranchExpression() << ");\n";

        if (!unconditionalLinkCode.empty())
        {
            m_ss << "        " << unconditionalLinkCode << "\n";
        }

        if (likely)
        {
            m_ss << "        if (" << branchTakenVar << ") {\n";
            if (!conditionalLinkCode.empty())
            {
                m_ss << "            " << conditionalLinkCode << "\n";
            }
            emitDelaySlot("            ");

            if (isInternalTarget(target))
            {
                emitInternalTarget(target, branchPc(), "            ");
            }
            else
            {
                m_ss << (m_gen.leanCalls() ? fmt::format("            PS2_JUMP_GUEST(0x{:X}u);\n", target)
                                           : fmt::format("            runtime->jumpGuest(rdram, ctx, 0x{:X}u);\n", target));
                m_ss << "            return;\n";
            }
            m_ss << "        }\n";
        }
        else
        {
            if (!conditionalLinkCode.empty())
            {
                m_ss << "        if (" << branchTakenVar << ") { " << conditionalLinkCode << " }\n";
            }

            emitDelaySlot("        ");

            m_ss << "        if (" << branchTakenVar << ") {\n";
            if (isInternalTarget(target))
            {
                emitInternalTarget(target, branchPc(), "            ");
            }
            else
            {
                m_ss << (m_gen.leanCalls() ? fmt::format("            PS2_JUMP_GUEST(0x{:X}u);\n", target)
                                           : fmt::format("            runtime->jumpGuest(rdram, ctx, 0x{:X}u);\n", target));
                m_ss << "            return;\n";
            }
            m_ss << "        }\n";
        }

        m_ss << "    }\n";
    }

    void ControlFlowEmitter::emitFallbackInstruction()
    {
        m_ss << "    " << m_gen.translateInstruction(m_branchInst) << "\n";
        emitDelaySlot("    ");
    }

    void ControlFlowEmitter::emitFallthroughLabelIfNeeded()
    {
        if (isInternalTarget(delayPc()) && !isInternalTarget(fallthroughPc()))
        {
            m_ss << fmt::format("label_fallthrough_0x{:x}:\n", branchPc());
        }
    }

    void ControlFlowEmitter::emitFinalFallthrough()
    {
        // Past a call ctx->pc is the return address already (callGuest and
        // the stub calls return only then); a jump does not fall through. A
        // branch not taken needs it only at the function's end, where the
        // function hands ctx->pc to the next one.
        if (m_gen.m_allPcStores || fallthroughPc() >= m_function.end)
        {
            m_ss << fmt::format("    ctx->pc = 0x{:X}u;\n", fallthroughPc());
        }
    }

    std::string ControlFlowEmitter::emit()
    {
        emitResumeFromDelaySlotEntry();
        if (m_gen.m_allPcStores)
        {
            m_ss << fmt::format("    ctx->pc = 0x{:X}u;\n", branchPc());
        }

        if (m_branchInst.opcode == OPCODE_J || m_branchInst.opcode == OPCODE_JAL)
        {
            emitStaticJump(m_branchInst.opcode == OPCODE_JAL ? StaticBranchKind::Call : StaticBranchKind::Jump);
        }
        else if (m_branchInst.opcode == OPCODE_SPECIAL &&
                 (m_branchInst.function == SPECIAL_JR || m_branchInst.function == SPECIAL_JALR))
        {
            emitRegisterJump(m_branchInst.function == SPECIAL_JALR ? RegisterBranchKind::Call : RegisterBranchKind::Jump);
        }
        else if (m_branchInst.isBranch)
        {
            emitConditionalBranch();
        }
        else
        {
            emitFallbackInstruction();
        }

        emitFallthroughLabelIfNeeded();
        emitFinalFallthrough();
        return m_ss.str();
    }
}
