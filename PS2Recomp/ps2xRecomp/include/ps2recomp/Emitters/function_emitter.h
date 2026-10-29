#ifndef PS2RECOMP_FUNCTION_EMITTER_H
#define PS2RECOMP_FUNCTION_EMITTER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace ps2recomp
{
    struct Function;
    struct Instruction;
    class CodeGenerator;

    class FunctionEmitter
    {
    public:
        explicit FunctionEmitter(CodeGenerator &codeGenerator);

        std::string emit(const Function &function, const std::vector<Instruction> &instructions, bool useHeaders);

    private:
        // Round 30: the gotos to each label in a first pass's text (labels
        // whose every goto comes before them can carry the VU0 / FPU
        // provenance in from their edges). Null: no carry (that first pass,
        // or PS2X_RECOMP_FLOAT_JOIN=0).
        using GotoCounts = std::unordered_map<uint32_t, int>;
        std::string emitPass(const Function &function, const std::vector<Instruction> &instructions, bool useHeaders,
                             bool firstPass, const GotoCounts *gotosBefore, const GotoCounts *gotosAll);
        CodeGenerator &m_codeGenerator;
    };
}

#endif // PS2RECOMP_FUNCTION_EMITTER_H
