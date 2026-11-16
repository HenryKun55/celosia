#pragma once
// Round A9 sweep (SOTC_A9SWEEP, default on; A/B name a9sweep): the runtime's side.
//
// The Vita's Cortex-A9 has no integer divide instruction: every division whose divisor the compiler does not know is a
// call (__aeabi_uidivmod -> __udivsi3, a bit-by-bit loop), and every 64-bit division is one even by a constant
// (__aeabi_uldivmod / __aeabi_ldivmod -> __udivmoddi4: GCC strength-reduces 32-bit divisions by constants only). The
// sampler found ~0.6% of the game thread in those two helpers, almost all from three scheduler functions
// (syncCyclesToHostClock's two 64-bit divisions, invocationStackTop's and thread()'s hash-table walks). The helpers here
// give the same quotients without the calls; the scheduler uses them while the switch is on.
//
// The switch itself is the game's (sotc_decomp::g_switch[kA9Sweep], flipped frame by frame by an interleaved A/B):
// the game points g_flag at that byte when it installs (a9sweep::init). Until then the runtime reads a constant 0
// (the original code).
#include <cstdint>

namespace ps2_a9
{
    inline const signed char kOff = 0;
    inline const signed char *g_flag = &kOff;
    inline bool on() { return *g_flag > 0; }

    // floor(n / D) for any 64-bit n and a constant 1 < D < 2^16: schoolbook long division in base 2^16. Each step's
    // dividend is (r << 16) | digit with r < D, so below D * 2^16 <= 2^32: a 32-bit division by a constant, which GCC
    // makes a multiply by the reciprocal (exact for every 32-bit dividend). The quotient digits are < 2^16 (as r < D).
    template <uint32_t D>
    inline __attribute__((always_inline)) uint64_t udivSmall(uint64_t n)
    {
        static_assert(D > 1u && D < 65536u, "udivSmall: 1 < D < 2^16");
        const uint32_t hi = static_cast<uint32_t>(n >> 32), lo = static_cast<uint32_t>(n);
        uint32_t cur = hi >> 16;
        const uint32_t q3 = cur / D;
        cur = ((cur - q3 * D) << 16) | (hi & 0xFFFFu);
        const uint32_t q2 = cur / D;
        cur = ((cur - q2 * D) << 16) | (lo >> 16);
        const uint32_t q1 = cur / D;
        cur = ((cur - q1 * D) << 16) | (lo & 0xFFFFu);
        const uint32_t q0 = cur / D;
        return (static_cast<uint64_t>((q3 << 16) | q2) << 32) | ((q1 << 16) | q0);
    }

    // (udiv1e6 / sdiv1000 not inlined: the sampler tells their time apart from the code around them, the A/B's NEW side.)
    // n / 1000000 (unsigned, any n): floor(floor(n / 64) / 15625) = floor(n / 10^6).
    inline __attribute__((noinline)) uint64_t udiv1e6(uint64_t n) { return udivSmall<15625u>(n >> 6); }

    // n / 1000 truncated toward zero (C++'s signed division, any n incl. INT64_MIN): the magnitude's floor(floor(m / 8) / 125)
    // with the sign put back.
    inline __attribute__((noinline)) int64_t sdiv1000(int64_t n)
    {
        const uint64_t m = n < 0 ? 0ull - static_cast<uint64_t>(n) : static_cast<uint64_t>(n);
        const uint64_t q = udivSmall<125u>(m >> 3);
        return n < 0 ? static_cast<int64_t>(0ull - q) : static_cast<int64_t>(q);
    }
}
