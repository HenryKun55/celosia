#ifndef PS2RECOMP_FLOAT_KNOWN_H
#define PS2RECOMP_FLOAT_KNOWN_H

#include <cstdint>
#include <string>

namespace ps2recomp
{
    // Round 29: what the emitter knows about the VU0 / FPU registers' values
    // along a straight-line run of a generated function (float_known.cpp).
    //
    // VU0: which lanes of each vf register and of ACC hold what the runtime's
    // operand normalisation (ps2VuOperand) would make of them, so a VU0 add /
    // sub / mul reading them can skip it (PS2_VADD_K & co.,
    // ps2_runtime_macros.h). FPU: which FPRs (and the FPU ACC) are finite, so
    // an FPU add / sub / mul of two of them can skip the result's clamp
    // (FPU_ADD_SF & co.).
    //
    // The emitter calls reset() wherever control can arrive from elsewhere
    // (labels, branches, the GIF DMA kick captures) and apply() on each
    // instruction's code in order; apply() rewrites the code and updates the
    // state. Anything it does not recognise only makes it forget (the
    // registers the code may write, or everything at a call / memory access /
    // runtime helper).
    class FloatKnownState
    {
    public:
        FloatKnownState(bool vu, bool fpu) : m_vu(vu), m_fpu(fpu) { reset(); }
        void reset();
        std::string apply(const std::string &code);
        bool enabled() const { return m_vu || m_fpu; }
        // MTC1 of a GPR whose value the emitter knows (its constant
        // tracking): finite when the pattern's exponent is not 255.
        void fprFromConstant(int fpr, uint32_t bits);

    private:
        std::string applyVu(const std::string &code, bool &handled);
        std::string applyFpu(const std::string &code, bool &handled);
        void forgetWrites(const std::string &code);

        bool m_vu;
        bool m_fpu;
        uint8_t m_vf[32]{};  // lanes (bit k: __m128 lane k) known normalised
        uint8_t m_acc = 0;
        uint32_t m_fprFinite = 0;
        bool m_faccFinite = false;
    };

    // Totals over every function emitted so far (all threads).
    std::string floatKnownReport();
}

#endif // PS2RECOMP_FLOAT_KNOWN_H
