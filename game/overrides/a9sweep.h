#pragma once
// Round A9 sweep (SOTC_A9SWEEP, default on; A/B name a9sweep): A9 codegen costs in hot code, without a change of behaviour.
//
// 1. Function-local statics with a dynamic initialiser (`static const bool x = getenv(...)`, `static T y = init()`): GCC
//    guards each with a load of the guard word and a `dmb ish` on every pass on ARMv7 (`ldr; dmb ish; lsls; bpl`), and
//    the call to __cxa_guard_acquire the first time. A hot function with several pays them on every call. Here those
//    values are hoisted: a plain global per value (A9S_HOIST, at namespace scope), set once by a9sweep::init() when the
//    overrides install (after env.txt is in the environment: ps2_vita_runtime.cpp's LogToFile sets it before main), read
//    with one load. The original static stays as the switch's off path (A9S_VAL), so an interleaved A/B compares the two.
//    The same expression is evaluated both ways from the same environment (nothing sets these variables later: the only
//    setenv after start is SOTC_DIFFTEST's); SOTC_A9SWEEP_LOG=1 prints every hoisted value at init and, the first time
//    each original static is initialised (the off path), its value and whether it differs ("[a9sweep]" lines).
// 2. The runtime's side (PS2Recomp ps2_a9.h): 64-bit divisions and hash-table walks in the scheduler (the A9 has no
//    divide instruction). ps2_a9::g_flag points at this switch's byte once init() ran.
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include "sotc_decomp.h"

namespace a9sweep
{
    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kA9Sweep); }

    // A registered initialiser of hoisted values (a namespace-scope object's constructor, before main: no guard at use).
    struct Hoist
    {
        void (*fn)();
        const Hoist *next;
        explicit Hoist(void (*f)());
    };
    inline const Hoist *g_head = nullptr;
    inline Hoist::Hoist(void (*f)()) : fn(f), next(g_head) { g_head = this; }
    inline bool g_log = false;
    inline bool g_ready = false;

    template <class T>
    inline void logNew(const char *name, T v)
    {
        if (!g_log)
            return;
        if constexpr (std::is_floating_point_v<T>)
            std::fprintf(stderr, "[a9sweep] new %s=%g\n", name, static_cast<double>(v));
        else
            std::fprintf(stderr, "[a9sweep] new %s=%lld\n", name, static_cast<long long>(v));
    }
    // The off path's static, the first time it is initialised: logged with the hoisted value it must equal.
    template <class T>
    inline T old(const char *name, T v, T hoisted)
    {
        if (g_log || (g_ready && !(v == hoisted)))
        {
            if constexpr (std::is_floating_point_v<T>)
                std::fprintf(stderr, "[a9sweep] old %s=%g%s\n", name, static_cast<double>(v), v == hoisted ? "" : " DIFFERS");
            else
                std::fprintf(stderr, "[a9sweep] old %s=%lld%s\n", name, static_cast<long long>(v), v == hoisted ? "" : " DIFFERS");
        }
        return v;
    }

    // Runs every registered initialiser once (sotc_overrides' install, before any native is registered).
    void init();
}

// A hoisted value at namespace scope: `A9S_HOIST(bool, g_x, std::getenv("X") != nullptr);`
#define A9S_HOIST(T, gv, ...)                                         \
    T gv{};                                                           \
    static const ::a9sweep::Hoist gv##_a9sHoist([] {                  \
        gv = static_cast<T>(__VA_ARGS__);                             \
        ::a9sweep::logNew<T>(#gv, gv);                                \
    })
// The same in a header (inline variables: one per program).
#define A9S_HOIST_INLINE(T, gv, ...)                                  \
    inline T gv{};                                                    \
    inline const ::a9sweep::Hoist gv##_a9sHoist{[] {                  \
        gv = static_cast<T>(__VA_ARGS__);                             \
        ::a9sweep::logNew<T>(#gv, gv);                                \
    }}
// The value in a function: on (sw), the hoisted global; off, the original function-local static with the same
// initialiser (evaluated at its first call as before). sw is a9sweep::on() read once by the caller.
#define A9S_VAL(sw, gv, T, ...)                                                                                       \
    (__builtin_expect((sw), 1) ? (gv) : ([]() -> T {                                                                   \
        static const T a9sOld_ = ::a9sweep::old<T>(#gv, static_cast<T>(__VA_ARGS__), gv);                              \
        return a9sOld_;                                                                                                \
    }()))

// 3. memcmp for equality of a constant size: newlib's memcmp is a call and a loop (6 instructions a word when both
//    pointers are 4-aligned, 5 a byte otherwise); the sampler found it at 1% of the game thread (cache validations:
//    16-byte rows, keys). a9eq::eq<N> compares the N bytes as words (unaligned loads through memcpy, which the A9
//    allows for ldr) folded with xor / or: equal words <=> equal bytes, so the answer is memcmp's == 0 for every input.
namespace a9eq
{
    template <size_t N>
    inline __attribute__((always_inline)) bool eq(const void *a, const void *b)
    {
        const unsigned char *pa = static_cast<const unsigned char *>(a), *pb = static_cast<const unsigned char *>(b);
        uint32_t acc = 0u;
        size_t i = 0;
        for (; i + 4u <= N; i += 4u)
        {
            uint32_t x, y;
            std::memcpy(&x, pa + i, 4);
            std::memcpy(&y, pb + i, 4);
            acc |= x ^ y;
        }
        for (; i < N; ++i)
            acc |= static_cast<uint32_t>(pa[i] ^ pb[i]);
        return acc == 0u;
    }
}
// memcmp(a, b, n) == 0 for a constant n: a9eq under SOTC_A9SWEEP, the library call off.
#define A9_MEMEQ(a, b, n) (__builtin_expect(::a9sweep::on(), 1) ? ::a9eq::eq<(n)>((a), (b)) : std::memcmp((a), (b), (n)) == 0)
