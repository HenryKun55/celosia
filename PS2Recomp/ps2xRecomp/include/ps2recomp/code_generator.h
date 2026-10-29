#ifndef PS2RECOMP_CODE_GENERATOR_H
#define PS2RECOMP_CODE_GENERATOR_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "ps2recomp/control_flow_analyzer.h"

namespace ps2recomp
{
	struct JumpTableEntry;
    struct JumpTable;
	struct Instruction;
    struct MemoryAccessHint;
	struct Function;
	struct Symbol;
	struct Section;
    class RecompilerReporter;

	extern const std::unordered_set<std::string> kKeywords;

    class CodeGenerator
    {
    public:
	    explicit CodeGenerator(const std::vector<Symbol> &symbols, const std::vector<Section> &sections);
        ~CodeGenerator();

        struct BootstrapInfo
        {
            bool valid = false;
            uint32_t entry = 0;
            uint32_t bssStart = 0;
            uint32_t bssEnd = 0;
            uint32_t gp = 0;
            std::string entryName;
        };

        using AnalysisResult = ControlFlowAnalysisResult;

        std::string generateFunction(const Function &function, const std::vector<Instruction> &instructions, const bool &useHeaders);
        std::string generateFunctionRegistration(const std::vector<Function> &functions, const std::map<uint32_t, std::string> &stubs);
        std::string handleBranchDelaySlots(const Instruction &branchInst, const Instruction &delaySlot,
                                           const Function &function, const AnalysisResult &analysisResult);
        std::string handleBranchDelaySlots(const Instruction &branchInst, const Instruction &delaySlot,
                                           const Function &function, const AnalysisResult &analysisResult,
                                           std::string delaySlotOverride);

        void setRenamedFunctions(const std::unordered_map<uint32_t, std::string> &renames);
        void setBootstrapInfo(const BootstrapInfo &info);
        void setRelocationCallNames(const std::unordered_map<uint32_t, std::string> &callNames);
        void setDynamicImmediates(const std::unordered_set<uint32_t> &addresses);
        bool isDynamicImmediate(uint32_t address) const { return m_dynamicImmediates.count(address) != 0; }
        bool hasDynamicImmediates() const { return !m_dynamicImmediates.empty(); }
        void setConfiguredJumpTables(const std::vector<JumpTable> &jumpTables);
        void setResumeEntryTargets(const std::unordered_map<uint32_t, std::vector<uint32_t>> &resumeTargetsByOwner);
        void setEmitInstructionComments(bool emitInstructionComments);
        void setReporter(RecompilerReporter *reporter);

        AnalysisResult collectInternalBranchTargets(const Function &function,
                                                  const std::vector<Instruction> &instructions,
                                                  const std::vector<Function> *allFunctions = nullptr);

    public:
        std::unordered_map<uint32_t, Symbol> m_symbols;
        std::unordered_map<uint32_t, std::string> m_renamedFunctions;
        std::unordered_map<uint32_t, std::string> m_relocationCallNames;
        std::unordered_set<uint32_t> m_dynamicImmediates;
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_configJumpTableTargetsByAddress;
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_resumeEntryTargetsByOwner;
        const std::vector<Section>& m_sections;
        BootstrapInfo m_bootstrapInfo;
        bool m_emitInstructionComments = true;
        // ctx->pc is stored only where something can read it: before code
        // that calls into the runtime (exceptions, traps, syscalls, VU0
        // microprograms), branches, calls, checkpoints and returns.
        // PS2X_RECOMP_ALL_PC=1 (at recompile time) stores it before every
        // instruction, as before.
        bool m_allPcStores = std::getenv("PS2X_RECOMP_ALL_PC") != nullptr;
        // Guest GPRs / FPRs in host locals within each function, written back
        // to ctx before anything that can read it (register_cache.cpp).
        // config [general] register_cache = false, or PS2X_RECOMP_REGCACHE=0
        // at recompile time, generates the old code (every access on ctx);
        // PS2X_RECOMP_REGCACHE_FPR=0 keeps the FPRs on ctx;
        // PS2X_RECOMP_REGCACHE_SEXT=0 drops the sign-extension tracking.
        bool m_registerCache = true;
        bool m_registerCacheFprs = true;
        bool m_registerCacheSext = true;
        // PS2X_RECOMP_REGCACHE_SCOPE=function: round 5's whole-function
        // locals; default (block): within a basic block (round 18).
        bool m_registerCacheBlock = true;
        // PS2X_RECOMP_REGCACHE_WT=0: writes stay in the locals until a
        // barrier / edge / return (round 5); default: stored at the write.
        bool m_registerCacheWriteThrough = true;
        // PS2X_RECOMP_REGCACHE_DSE=0: every write-through store kept (round
        // 18); default: none for a write overwritten later in the run.
        // Round 30: a label without a back edge, with no guest memory access
        // or barrier between its first goto and it, keeps the locals its
        // edges agree on (register_cache.h, joinForward).
        // PS2X_RECOMP_REGCACHE_JOIN=0: every label a sync point (round 18).
        bool m_registerCacheJoin = !(std::getenv("PS2X_RECOMP_REGCACHE_JOIN") && std::getenv("PS2X_RECOMP_REGCACHE_JOIN")[0] == '0');
        bool m_registerCacheDeadStores = !(std::getenv("PS2X_RECOMP_REGCACHE_DSE") && std::getenv("PS2X_RECOMP_REGCACHE_DSE")[0] == '0');
        // Round 16's leaner calls: JAL / JALR / J through PS2_CALL_GUEST /
        // PS2_JUMP_GUEST (the table's bounds as constants), and a function's
        // entry tests ctx->pc against its own start before the resume
        // switch. PS2X_RECOMP_LEAN_CALLS=0 at recompile time: the old code.
        // Since round 18 the register cache parses these forms too (the
        // macros are barriers, register_cache.cpp).
        bool m_leanCalls = !(std::getenv("PS2X_RECOMP_LEAN_CALLS") && std::getenv("PS2X_RECOMP_LEAN_CALLS")[0] == '0');
        bool leanCalls() const { return m_leanCalls; }
        // Round 29 (float_known.cpp): VU0 add / sub / mul operands known to
        // be normalised already skip ps2VuOperand (PS2_VADD_K & co.), FPU
        // add / sub / mul of finite operands skip the result's clamp
        // (FPU_ADD_SF & co.). The same bits. PS2X_RECOMP_VUKNOWN=0 /
        // PS2X_RECOMP_FPUKNOWN=0 at recompile time: the old forms (both 0:
        // round 28's output byte for byte).
        bool m_vuKnown = !(std::getenv("PS2X_RECOMP_VUKNOWN") && std::getenv("PS2X_RECOMP_VUKNOWN")[0] == '0');
        bool m_fpuKnown = !(std::getenv("PS2X_RECOMP_FPUKNOWN") && std::getenv("PS2X_RECOMP_FPUKNOWN")[0] == '0');
        // Round 30: a loop's back edge stores ctx->pc only when its
        // checkpoint is taken (PS2_CHECKPOINT_DUE_AT). PS2X_RECOMP_LOOP_PC=0
        // at recompile time: the store before the test (round 29).
        bool m_lazyLoopPc = !(std::getenv("PS2X_RECOMP_LOOP_PC") && std::getenv("PS2X_RECOMP_LOOP_PC")[0] == '0');
        // Round 30: the provenance carried across forward branches and the
        // labels all of whose gotos come before them (what the edges and the
        // fall-through agree on). PS2X_RECOMP_FLOAT_JOIN=0: forgotten at
        // every label and branch (round 29).
        bool m_floatJoin = !(std::getenv("PS2X_RECOMP_FLOAT_JOIN") && std::getenv("PS2X_RECOMP_FLOAT_JOIN")[0] == '0');
        // Round 30: guest loads keep the VU0 provenance (only stores, which
        // can kick VIF0, forget it). PS2X_RECOMP_VU_LOADS=0: every guest
        // memory access forgets it (round 29).
        bool m_vuLoads = !(std::getenv("PS2X_RECOMP_VU_LOADS") && std::getenv("PS2X_RECOMP_VU_LOADS")[0] == '0');
        // The function table's bounds (FunctionTableEmitter::emit).
        uint32_t m_tableBase = 0u;
        uint32_t m_tableSlots = 0u;
        // Whether translated code can read ctx->pc (it calls into the
        // runtime other than for a memory access, or throws).
        static bool codeReadsPc(const std::string &code);
        RecompilerReporter *m_reporter = nullptr;
        std::string m_currentFunctionName;

        std::string translateInstruction(const Instruction &inst);
        std::string translateInstruction(const Instruction &inst, const MemoryAccessHint &memoryHint);
        std::string emitUnhandledInstruction(const Instruction &inst, const std::string &message);
        std::string translateMMIInstruction(const Instruction &inst);
        std::string translateVUInstruction(const Instruction &inst);
        std::string translateFPUInstruction(const Instruction &inst);
        std::string translateCOP0Instruction(const Instruction &inst);
        std::string translateRegimmInstruction(const Instruction &inst);
        std::string translateSpecialInstruction(const Instruction &inst);

        // MMI Translation functions
        std::string translateMMI0Instruction(const Instruction &inst);
        std::string translateMMI1Instruction(const Instruction &inst);
        std::string translateMMI2Instruction(const Instruction &inst);
        std::string translateMMI3Instruction(const Instruction &inst);
        std::string translatePMFHLInstruction(const Instruction &inst);
        std::string translatePMTHLInstruction(const Instruction &inst);

        // Instruction Helpers
        std::string translateQFSRV(const Instruction &inst);
        std::string translatePMADDW(const Instruction &inst);
        std::string translatePMULTW(const Instruction &inst);
        std::string translatePMSUBW(const Instruction &inst);
        std::string translatePEXT5(const Instruction &inst);
        std::string translatePPAC5(const Instruction &inst);
        std::string translatePADSBH(const Instruction &inst);
        std::string translatePMSUBH(const Instruction &inst);
        std::string translatePHMSBH(const Instruction &inst);
        std::string translatePMADDUW(const Instruction &inst);
        std::string translatePDIVW(const Instruction &inst);
        std::string translatePCPYLD(const Instruction &inst);
        std::string translatePMADDH(const Instruction &inst);
        std::string translatePHMADH(const Instruction &inst);
        std::string translatePEXEH(const Instruction &inst);
        std::string translatePREVH(const Instruction &inst);
        std::string translatePMULTH(const Instruction &inst);
        std::string translatePDIVBW(const Instruction &inst);
        std::string translatePEXEW(const Instruction &inst);
        std::string translatePROT3W(const Instruction &inst);
        std::string translatePMULTUW(const Instruction &inst);
        std::string translatePDIVUW(const Instruction &inst);
        std::string translatePCPYUD(const Instruction &inst);
        std::string translatePEXCH(const Instruction &inst);
        std::string translatePCPYH(const Instruction &inst);
        std::string translatePEXCW(const Instruction &inst);
        std::string translatePMTHI(const Instruction &inst);
        std::string translatePMTLO(const Instruction &inst);

        // VU instruction translations
        std::string translateVU_VADD_Field(const Instruction &inst);
        std::string translateVU_VSUB_Field(const Instruction &inst);
        std::string translateVU_VMUL_Field(const Instruction &inst);
        std::string translateVU_VMSUB_Field(const Instruction &inst);
        std::string translateVU_VADDA_Field(const Instruction &inst);
        std::string translateVU_VSUBA_Field(const Instruction &inst);
        std::string translateVU_VMADDA_Field(const Instruction &inst);
        std::string translateVU_VMSUBA_Field(const Instruction &inst);
        std::string translateVU_VMULA_Field(const Instruction &inst);
        std::string translateVU_VADD(const Instruction &inst);
        std::string translateVU_VSUB(const Instruction &inst);
        std::string translateVU_VMUL(const Instruction &inst);
        std::string translateVU_VDIV(const Instruction &inst);
        std::string translateVU_VSQRT(const Instruction &inst);
        std::string translateVU_VRSQRT(const Instruction &inst);
        std::string translateVU_VMTIR(const Instruction &inst);
        std::string translateVU_VMFIR(const Instruction &inst);
        std::string translateVU_VILWR(const Instruction &inst);
        std::string translateVU_VISWR(const Instruction &inst);
        std::string translateVU_VIADD(const Instruction &inst);
        std::string translateVU_VISUB(const Instruction &inst);
        std::string translateVU_VIADDI(const Instruction &inst);
        std::string translateVU_VIAND(const Instruction &inst);
        std::string translateVU_VIOR(const Instruction &inst);
        std::string translateVU_VCALLMS(const Instruction &inst);
        std::string translateVU_VCALLMSR(const Instruction &inst);
        std::string translateVU_VRNEXT(const Instruction &inst);
        std::string translateVU_VRGET(const Instruction &inst);
        std::string translateVU_VRINIT(const Instruction &inst);
        std::string translateVU_VRXOR(const Instruction &inst);
        std::string translateVU_VMADD_Field(const Instruction &inst);
        std::string translateVU_VMINI_Field(const Instruction &inst);
        std::string translateVU_VMAX_Field(const Instruction &inst);
        std::string translateVU_VMADD(const Instruction &inst);
        std::string translateVU_VMADDq(const Instruction &inst);
        std::string translateVU_VMADDi(const Instruction &inst);
        std::string translateVU_VMAX(const Instruction &inst);
        std::string translateVU_VMAXi(const Instruction &inst);
        std::string translateVU_VADDA(const Instruction &inst);
        std::string translateVU_VADDAq(const Instruction &inst);
        std::string translateVU_VADDAi(const Instruction &inst);
        std::string translateVU_VSUBA(const Instruction &inst);
        std::string translateVU_VSUBAq(const Instruction &inst);
        std::string translateVU_VSUBAi(const Instruction &inst);
        std::string translateVU_VMADDA(const Instruction &inst);
        std::string translateVU_VMADDAq(const Instruction &inst);
        std::string translateVU_VMADDAi(const Instruction &inst);
        std::string translateVU_VMSUBA(const Instruction &inst);
        std::string translateVU_VMSUBAq(const Instruction &inst);
        std::string translateVU_VMSUBAi(const Instruction &inst);
        std::string translateVU_VMULA(const Instruction &inst);
        std::string translateVU_VMULAq(const Instruction &inst);
        std::string translateVU_VMULAi(const Instruction &inst);
        std::string translateVU_VOPMULA(const Instruction &inst);
        std::string translateVU_VOPMSUB(const Instruction &inst);
        std::string translateVU_VMINI(const Instruction &inst);
        std::string translateVU_VMINIi(const Instruction &inst);
        std::string translateVU_VMSUB(const Instruction &inst);
        std::string translateVU_VMSUBq(const Instruction &inst);
        std::string translateVU_VMSUBi(const Instruction &inst);
        std::string translateVU_VMULq(const Instruction &inst);
        std::string translateVU_VMULi(const Instruction &inst);
        std::string translateVU_VADDq(const Instruction &inst);
        std::string translateVU_VADDi(const Instruction &inst);
        std::string translateVU_VSUBq(const Instruction &inst);
        std::string translateVU_VSUBi(const Instruction &inst);
        std::string translateVU_VITOF(const Instruction &inst, int shift);
        std::string translateVU_VFTOI(const Instruction &inst, int shift);
        std::string translateVU_VLQI(const Instruction &inst);
        std::string translateVU_VSQI(const Instruction &inst);
        std::string translateVU_VLQD(const Instruction &inst);
        std::string translateVU_VSQD(const Instruction &inst);

        // Jump Table Generation
        std::string generateJumpTableSwitch(const Instruction &inst, uint32_t tableAddress,
                                            const std::vector<JumpTableEntry> &entries);

        const Symbol *findSymbolByAddress(uint32_t address) const;
        std::string getFunctionName(uint32_t address) const;
        std::string sanitizeFunctionName(const std::string& name) const;
    };

}

#endif // PS2RECOMP_CODE_GENERATOR_H
