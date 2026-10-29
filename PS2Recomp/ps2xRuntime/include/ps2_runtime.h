#ifndef PS2_RUNTIME_H
#define PS2_RUNTIME_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <functional>
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(USE_SSE2NEON)
#include "sse2neon.h"
#else
#include <immintrin.h> // For SSE/AVX instructions
#include <smmintrin.h> // For SSE4.1 instructions
#endif
#include <atomic>
#include <array>
#include <mutex>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "ps2_log.h"
#include "runtime/ps2_address.h"
#include "runtime/gs/ps2_gif_arbiter.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_clock.h"
#include "runtime/gs/gs_frontend.h"
#include "runtime/ps2_vu1.h"
#include "runtime/ps2_audio.h"
#include "runtime/ps2_pad.h"
#include "runtime/ps2_rom_device.h"
#include "runtime/ps2_vfs.h"
#include "ps2x/iop/iop_types.h"

namespace ps2x::iop
{
    class IopSubsystem;
}

class PS2IopHostAdapter;
class PS2IopTransport;
class EeScheduler;
struct EeEvent;

enum PS2Exception
{
    EXCEPTION_TLB_REFILL = 0x02,          // TLB refill/load exception
    EXCEPTION_ADDRESS_ERROR_LOAD = 0x04,  // Address error on load
    EXCEPTION_ADDRESS_ERROR_STORE = 0x05, // Address error on store
    EXCEPTION_SYSCALL = 0x08,             // SYSCALL instruction
    EXCEPTION_BREAKPOINT = 0x09,          // BREAK instruction
    EXCEPTION_RESERVED_INSTRUCTION = 0x0A,
    EXCEPTION_INTEGER_OVERFLOW = 0x0C, // From MIPS spec
    EXCEPTION_TRAP = 0x0D,             // Trap instruction condition met
};

// PS2 CPU context (R5900)
struct alignas(16) R5900Context
{
    // General Purpose Registers (128-bit)
    __m128i r[32]; // Main registers

    // Control registers
    uint32_t pc;         // Program counter
    uint64_t insn_count; // Instruction counter
    uint64_t hi, lo;     // HI/LO registers for mult/div results
    uint64_t hi1, lo1;   // Secondary HI/LO registers for MULT1/DIV1
    uint32_t sa;         // Shift amount register

    // VU0 registers (when used in macro mode)
    __m128 vu0_vf[32];        // VU0 vector float registers
    uint16_t vi[16];          // VU0 vector integer registers
    float vu0_q;              // VU0 Q register (quotient)
    float vu0_p;              // VU0 P register (EFU result)
    float vu0_i;              // VU0 I register (integer value)
    __m128 vu0_r;             // VU0 R register
    __m128 vu0_acc;           // VU0 ACC accumulator register
    uint16_t vu0_status;      // VU0 status register
    uint32_t vu0_mac_flags;   // VU0 MAC flags
    uint32_t vu0_clip_flags;  // VU0 clipping flags
    uint32_t vu0_clip_flags2; // VU0 clipping flags
    uint32_t vu0_cmsar0;      // VU0 microprogram start address
    uint32_t vu0_cmsar1;      // VU0 microprogram start address
    uint32_t vu0_cmsar2;      // VU0 microprogram start address
    uint32_t vu0_cmsar3;      // VU0 microprogram start address
    uint32_t vu0_vpu_stat;
    uint32_t vu0_vpu_stat2; // extra VPU status (used by CR_VPU_STAT2)
    uint32_t vu0_vpu_stat3; // extra VPU status 3
    uint32_t vu0_vpu_stat4; // extra VPU status 4
    uint32_t vu0_tpc;       // TPC (VU0 PC)
    uint32_t vu0_tpc2;      // second TPC
    uint32_t vu0_fbrst;     // VIF/VU reset register
    uint32_t vu0_fbrst2;    // FBRST2
    uint32_t vu0_fbrst3;    // FBRST3
    uint32_t vu0_fbrst4;    // FBRST4
    uint32_t vu0_itop;
    uint32_t vu0_top;
    uint32_t vu0_info;
    uint32_t vu0_xitop; // VU0 XITOP - input ITOP for VIF/VU sync
    uint32_t vu0_pc;

    float vu0_cf[4]; // VU0 FMAC control floating-point registers

    // COP0 System control registers
    uint32_t cop0_index;
    uint32_t cop0_random;
    uint32_t cop0_entrylo0;
    uint32_t cop0_entrylo1;
    uint32_t cop0_context;
    uint32_t cop0_pagemask;
    uint32_t cop0_wired;
    uint32_t cop0_badvaddr;
    uint32_t cop0_count;
    uint32_t cop0_entryhi;
    uint32_t cop0_compare;
    uint32_t cop0_status;
    uint32_t cop0_cause;
    uint32_t cop0_epc;
    uint32_t cop0_prid;
    uint32_t cop0_config;
    uint32_t cop0_badpaddr;
    uint32_t cop0_debug;
    uint32_t cop0_perf;
    uint32_t cop0_taglo;
    uint32_t cop0_taghi;
    uint32_t cop0_errorepc;

    // LL/SC reservation state (not part of COP0 Status bits).
    uint32_t llbit;
    uint32_t lladdr;

    // Delay slot state tracking
    bool in_delay_slot;
    uint32_t branch_pc;

    // COP2 control registers (VU0 integer + control)
    uint32_t cop2_ccr[32];

    // FPU registers (COP1)
    float f[32];
    float f_acc;    // FPU accumulator
    uint32_t fcr31; // Control/status register

    R5900Context()
    {
        std::memset(this, 0, sizeof(*this));

        // Initialize VU0 registers
        vu0_q = 1.0f; // Q register usually initialized to 1.0
        // VF00 is hardwired to (0, 0, 0, 1); every thread context must see it.
        vu0_vf[0] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

        // Reset COP0 registers
        cop0_random = 47; // Start at maximum value
        // Status as the EE kernel leaves it at handoff. IE (bit 0) and EIE
        // (bit 16) are separate enables and guest code reads both; libkernel's
        // StartThread refuses to run while IE is clear.
        cop0_status = 0x00010001; // EIE | IE
        cop0_prid = 0x00002e20; // CPU ID for R5900

        in_delay_slot = false;
        branch_pc = 0;
    }

    void dump() const
    {
        std::ios_base::fmtflags flags = std::cout.flags();
        std::cout << std::hex << std::setfill('0');
        std::cout << "--- R5900 Context Dump ---\n";
        std::cout << "PC: 0x" << std::setw(8) << pc << "\n";
        std::cout << "HI: 0x" << std::setw(8) << hi << " LO: 0x" << std::setw(8) << lo << "\n";
        std::cout << "HI1:0x" << std::setw(8) << hi1 << " LO1:0x" << std::setw(8) << lo1 << "\n";
        std::cout << "SA: 0x" << std::setw(8) << sa << "\n";
        for (int i = 0; i < 32; ++i)
        {
            std::cout << "R" << std::setw(2) << std::dec << i << ": 0x" << std::hex
                      << std::setw(8) << static_cast<uint32_t>(_mm_extract_epi32(r[i], 3))
                      << std::setw(8) << static_cast<uint32_t>(_mm_extract_epi32(r[i], 2)) << "_"
                      << std::setw(8) << static_cast<uint32_t>(_mm_extract_epi32(r[i], 1))
                      << std::setw(8) << static_cast<uint32_t>(_mm_extract_epi32(r[i], 0)) << "\n";
        }
        std::cout << "Status: 0x" << std::setw(8) << cop0_status
                  << " Cause: 0x" << std::setw(8) << cop0_cause
                  << " EPC: 0x" << std::setw(8) << cop0_epc << "\n";
        std::cout << "--- End Context Dump ---\n";
        std::cout.flags(flags); // Restore format flags
    }

    ~R5900Context() = default;
};

inline uint32_t getRegU32(const R5900Context *ctx, int reg)
{
    // Check if reg is valid (0-31)
    if (reg < 0 || reg > 31)
        return 0;
    if (reg == 0)
        return 0;
    return static_cast<uint32_t>(_mm_extract_epi32(ctx->r[reg], 0));
}

inline void setReturnU32(R5900Context *ctx, uint32_t value)
{
    // R5900 sign-extends 32-bit results into 64-bit GPR, even for unsigned values.
    ctx->r[2] = _mm_set_epi64x(0, static_cast<int64_t>(static_cast<int32_t>(value))); // $v0
}

inline void setReturnS32(R5900Context *ctx, int32_t value)
{
    // Signed 32-bit return should be sign-extended when observed as 64-bit.
    ctx->r[2] = _mm_set_epi64x(0, static_cast<int64_t>(value)); // $v0
}

inline void setReturnU64(R5900Context *ctx, uint64_t value)
{
    // Keep both conventions: full 64-bit value in $v0 and high 32-bit in $v1.
    ctx->r[2] = _mm_set_epi64x(0, static_cast<int64_t>(value));
    ctx->r[3] = _mm_set_epi64x(0, static_cast<int64_t>(static_cast<uint32_t>(value >> 32)));
}

inline constexpr uint32_t PS2_PATH_WATCH_ADDR = 0x01EFFFA0u;
inline constexpr uint32_t PS2_PATH_WATCH_BYTES = 0x200u;

inline uint32_t ps2PathWatchPhysAddr()
{
    return PS2_PATH_WATCH_ADDR & PS2_RAM_MASK;
}

inline uint8_t ps2PathWatchExtractByteFromWrite(uint32_t writeAddr, uint32_t watchAddr, uint64_t valueLo, uint64_t valueHi)
{
    const uint32_t byteIndex = watchAddr - writeAddr;
    if (byteIndex < 8u)
    {
        return static_cast<uint8_t>((valueLo >> (byteIndex * 8u)) & 0xFFu);
    }
    return static_cast<uint8_t>((valueHi >> ((byteIndex - 8u) * 8u)) & 0xFFu);
}

inline bool g_ps2WatchWriteEnabled = std::getenv("PS2X_WATCH_WRITE") != nullptr;
// Set by debugging hooks to log every guest store while true.
inline bool g_ps2LogAllWrites = false;

[[gnu::noinline]] inline void ps2TraceGuestWriteSlow(uint8_t *rdram,
                                                     uint32_t guestAddr,
                                                     uint32_t size,
                                                     uint64_t valueLo,
                                                     uint64_t valueHi,
                                                     const char *op,
                                                     const R5900Context *ctx)
{
    (void)rdram;
    if (g_ps2LogAllWrites)
    {
        static int logged = 0;
        if (logged++ < 3000)
            std::fprintf(stderr, "[write] %s addr=0x%08x size=%u lo=0x%016llx pc=0x%08x\n", op, guestAddr, size,
                         static_cast<unsigned long long>(valueLo), ctx ? ctx->pc : 0u);
        return;
    }
    static const std::pair<uint32_t, uint32_t> watch = []
    {
        const char *env = std::getenv("PS2X_WATCH_WRITE");
        if (!env || !std::strchr(env, ':'))
            return std::pair<uint32_t, uint32_t>(0u, 0u);
        return std::pair<uint32_t, uint32_t>(static_cast<uint32_t>(std::strtoul(env, nullptr, 0)),
                                             static_cast<uint32_t>(std::strtoul(std::strchr(env, ':') + 1, nullptr, 0)));
    }();
    if (__builtin_expect(watch.second == 0u, 1))
        return;
    const uint32_t phys = guestAddr & 0x1FFFFFFFu;
    if (phys + size <= watch.first || phys >= watch.second)
        return;
    static std::atomic<uint32_t> logged{0};
    if (logged.fetch_add(1) < 400u)
        std::fprintf(stderr, "[watch] %s addr=0x%08x lo=0x%016llx hi=0x%016llx pc=0x%08x ra=0x%08x\n", op, guestAddr,
                     static_cast<unsigned long long>(valueLo), static_cast<unsigned long long>(valueHi),
                     ctx ? ctx->pc : 0u, ctx ? static_cast<uint32_t>(_mm_cvtsi128_si32(ctx->r[31])) : 0u);
}

// PS2X_WATCH_WRITE=<start>:<end> logs guest stores that touch the range.
PS2_FORCE_INLINE void ps2TraceGuestWrite(uint8_t *rdram, uint32_t guestAddr, uint32_t size, uint64_t valueLo,
                               uint64_t valueHi, const char *op, const R5900Context *ctx)
{
    if (__builtin_expect(g_ps2WatchWriteEnabled, 0))
        ps2TraceGuestWriteSlow(rdram, guestAddr, size, valueLo, valueHi, op, ctx);
}

inline void ps2TraceGuestRangeWrite(uint8_t *rdram,
                                    uint32_t guestAddr,
                                    uint32_t size,
                                    const char *op,
                                    const R5900Context *ctx)
{
    // Bulk writes from host-implemented helpers (memcpy/memset...).
    if (__builtin_expect(!g_ps2WatchWriteEnabled, 1))
        return;
    ps2TraceGuestWrite(rdram, guestAddr, size, 0u, 0u, op, ctx);
}

class PS2Runtime
{
public:
    struct IoPaths
    {
        std::filesystem::path elfPath;
        std::filesystem::path elfDirectory;
        std::filesystem::path hostRoot;
        std::filesystem::path cdRoot;
        std::filesystem::path mcRoot;
        std::filesystem::path cdImage;
    };

    PS2Runtime();
    ~PS2Runtime();

    bool initialize(const char *title = "PS2 Game");
    bool syncCoreSubsystems();
    bool loadELF(const std::string &elfPath);
    void run();

    [[nodiscard]] ps2x::iop::ModuleLoadResult loadIopModule(std::string_view path, const void *arguments = nullptr, uint32_t argumentSize = 0);
    [[nodiscard]] ps2x::iop::ModuleLoadResult loadIopModuleBuffer(uint32_t guestAddress, const void *arguments = nullptr, uint32_t argumentSize = 0);
    [[nodiscard]] bool stopIopModule(int32_t moduleId, int32_t *result = nullptr);
    [[nodiscard]] ps2x::iop::DebugSnapshot iopDebugSnapshot() const;
    uint32_t allocateIopMemory(uint32_t size, uint32_t alignment = 16u);
    bool freeIopMemory(uint32_t address);
    bool readIopMemory(uint32_t address, void *destination, size_t size) const;
    bool writeIopMemory(uint32_t address, const void *source, size_t size);
    bool zeroIopMemory(uint32_t address, size_t size);
    bool isIopMemoryRange(uint32_t address, size_t size) const;

    using DebugUiCallback = void (*)(PS2Runtime &runtime, void *userData);
    void setDebugUiCallbacks(DebugUiCallback initCallback,
                             DebugUiCallback drawCallback,
                             DebugUiCallback shutdownCallback,
                             void *userData);

    using RecompiledFunction = void (*)(uint8_t *, R5900Context *, PS2Runtime *);

    enum class GuestBranchKind
    {
        DirectJump,
        DirectCall,
        IndirectJump,
        IndirectCall,
        Return,
    };

    enum class MissingFunctionPolicy : uint32_t
    {
        // Strict mode for tests/CI: log the bad target and request the runtime to stop.
        Stop = 0,

        // Debug mode: log once, leave ctx->pc on the bad target, and let the caller unwind.
        ContinueToTarget = 1,

        // Debug mode: same as ContinueToTarget, but triggers a debugger break once on MSVC.
        BreakOnce = 2,

        // Escape hatch only: skip missing calls by returning to fallthrough (it can hide guest bugs)
        SkipCallDebug = 3,
    };

    bool replaceFunction(uint32_t address, RecompiledFunction func);
    // TODO remove this later need to update all tests
    bool registerFunction(uint32_t address, RecompiledFunction func);
    RecompiledFunction lookupFunction(uint32_t address);
    void dumpDispatchProfile();
    bool hasFunction(uint32_t address) const;
    bool dispatchGuestBranch(uint8_t *rdram,
                             R5900Context *ctx,
                             uint32_t targetPc,
                             uint32_t sourcePc,
                             uint32_t fallthroughPc,
                             GuestBranchKind kind,
                             const char *debugName);
    void reportMissingFunction(uint8_t *rdram,
                               R5900Context *ctx,
                               uint32_t targetPc,
                               uint32_t sourcePc,
                               GuestBranchKind kind,
                               const char *debugName);
    void setMissingFunctionPolicy(MissingFunctionPolicy policy);
    MissingFunctionPolicy missingFunctionPolicy() const;
    void resetMissingFunctionReportOnce();

    static const IoPaths &getIoPaths();
    static void setIoPaths(const IoPaths &paths);
    static void configureIoPathsFromElf(const std::string &elfPath);

    void SignalException(R5900Context *ctx, PS2Exception exception);

    void executeVU0Microprogram(uint8_t *rdram, R5900Context *ctx, uint32_t address);
    void vu0StartMicroProgram(uint8_t *rdram, R5900Context *ctx, uint32_t address);

public:
    void handleSyscall(uint8_t *rdram, R5900Context *ctx);
    void handleSyscall(uint8_t *rdram, R5900Context *ctx, uint32_t encodedSyscallId);
    void handleBreak(uint8_t *rdram, R5900Context *ctx);

    void handleTrap(uint8_t *rdram, R5900Context *ctx);
    void handleTLBR(uint8_t *rdram, R5900Context *ctx);
    void handleTLBWI(uint8_t *rdram, R5900Context *ctx);
    void handleTLBWR(uint8_t *rdram, R5900Context *ctx);
    void handleTLBP(uint8_t *rdram, R5900Context *ctx);
    void clearLLBit(R5900Context *ctx);
    void configureGuestHeap(uint32_t guestBase, uint32_t guestLimit = PS2_RAM_SIZE);
    uint32_t guestMalloc(uint32_t size, uint32_t alignment = 16u);
    uint32_t guestCalloc(uint32_t count, uint32_t size, uint32_t alignment = 16u);
    uint32_t guestRealloc(uint32_t guestAddr, uint32_t newSize, uint32_t alignment = 16u);
    void guestFree(uint32_t guestAddr);
    uint32_t guestHeapBase() const;
    uint32_t guestHeapEnd() const;
    uint32_t guestHeapLimit() const;
    uint32_t reserveAsyncCallbackStack(uint32_t size, uint32_t alignment = 16u);

    void drainCompletedDmacHandlers(uint8_t *rdram);
    uint32_t m_reportedGsCsrEvents = 0u;

    void requestStop();
    bool isStopRequested() const;

    EeScheduler &eeScheduler();
    const EeScheduler &eeScheduler() const;
    void postEeEvent(EeEvent event);

    // Generated code's view of the EE scheduler (inline below the class).
    // eeCheckpointDue: charge cycles; true when the code must return to the
    // scheduler. callGuest: a JAL / JALR, run as a host call; false when the
    // code must return. jumpGuest: a jump out of the function (J / JR to
    // another function, a branch or fall-through past its end), run as a
    // nested host call when it can; the code returns after it either way.
    static constexpr uint32_t kEeDispatchCycles = 8u;
    EeClock m_eeClock;
    // Below this host stack address, calls and jumps no longer nest: they
    // return to the scheduler, which runs them from the bottom of the stack.
    // With PS2X_DIRECT_DISPATCH=0 it is the top of the address space (every
    // call takes the slow path, which looks at m_directDispatch): one test
    // for both on the fast path (round 16).
    uintptr_t m_hostStackFloor = 0;
    uint32_t m_guestJumpSerial = 0; // guest jumps taken (see dispatchGuestBranch)
    bool m_directDispatch = true;   // PS2X_DIRECT_DISPATCH=0: every transfer through dispatchGuestBranch / the scheduler
    inline bool eeCheckpointDue(uint32_t cycles = 32u) noexcept;
    // Round 30: a loop's back edge (PS2_CHECKPOINT_DUE_AT): ctx->pc is set to
    // the loop's head only when the checkpoint is taken (the scheduler
    // resumes there). On the fast path nothing reads it: plain instructions
    // and guest memory accesses never get a pc store (CodeGenerator::
    // codeReadsPc), anything that reads it stores its own first.
    inline bool eeCheckpointDueAt(R5900Context *ctx, uint32_t pc, uint32_t cycles = 32u) noexcept;
    bool eeCheckpointSlow() noexcept;
    inline RecompiledFunction guestFunctionAt(uint32_t target) const noexcept;
    inline bool hostStackDeep() const noexcept;
    inline bool callGuest(uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t returnPc,
                          uint32_t sourcePc, GuestBranchKind kind, const char *debugName);
    inline void jumpGuest(uint8_t *rdram, R5900Context *ctx, uint32_t target);
    // The same with the callee already looked up (generated code since
    // round 16: PS2_CALL_GUEST / PS2_JUMP_GUEST in ps2_runtime_macros.h look
    // it up with the table's base and size as constants, so a JAL's slot is
    // a constant address).
    template <uint32_t Base, uint32_t Slots>
    static inline RecompiledFunction guestFunctionIn(uint32_t target) noexcept;
    inline bool callGuestFn(RecompiledFunction fn, uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t returnPc,
                            uint32_t sourcePc, GuestBranchKind kind, const char *debugName);
    inline void jumpGuestFn(RecompiledFunction fn, uint8_t *rdram, R5900Context *ctx, uint32_t target);
    inline bool callGuestKnown(RecompiledFunction fn, RecompiledFunction known, uint8_t *rdram, R5900Context *ctx, uint32_t target,
                               uint32_t returnPc, uint32_t sourcePc, GuestBranchKind kind, const char *debugName);
    void jumpGuestSlow(RecompiledFunction fn);
    bool callGuestSlow(uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t returnPc,
                       uint32_t sourcePc, GuestBranchKind kind, const char *debugName);
    bool finishGuestCall(R5900Context *ctx, uint32_t entryPc, uint32_t returnPc, uint32_t jumpSerial);
    [[noreturn]] void eeWaitVSyncTicks(uint32_t ticks, uint32_t resumePc);

    struct EeExitHandlerRegistration
    {
        uint32_t function = 0;
        uint32_t argument = 0;
    };
    void addEeExitHandler(int threadId, uint32_t function, uint32_t argument);
    std::vector<EeExitHandlerRegistration> takeEeExitHandlers(int threadId);
    void removeEeExitHandlers(int threadId);
    bool findEeSyscallOverride(uint32_t syscallNumber, uint32_t &handler) const;
    void setEeSyscallOverride(uint8_t *rdram, uint32_t syscallNumber, uint32_t handler);
    void initializeEeKernelState(uint8_t *rdram);

    uint8_t Load8(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr);
    uint16_t Load16(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr);
    uint32_t Load32(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr);
    uint64_t Load64(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr);
    __m128i Load128(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr);

    void Store8(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr, uint8_t value);
    void Store16(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr, uint16_t value);
    void Store32(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr, uint32_t value);
    void Store64(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr, uint64_t value);
    void Store128(uint8_t *rdram, R5900Context *ctx, uint32_t vaddr, __m128i value);
    void kickGifDmaChainFromMMIO(uint8_t *rdram,
                                 R5900Context *ctx,
                                 uint32_t dPcrValue,
                                 uint32_t dStatValue,
                                 uint32_t tadr,
                                 uint32_t chcr);

    static PS2_FORCE_INLINE bool isSpecialAddress(uint32_t addr)
    {
        return Ps2IsSpecialAddress(addr);
    }

public:
    inline R5900Context &cpu() { return m_cpuContext; }
    inline const R5900Context &cpu() const { return m_cpuContext; }

    inline PS2Memory &memory() { return m_memory; }
    inline const PS2Memory &memory() const { return m_memory; }

    inline GS &gs() { return m_gs; }
    inline const GS &gs() const { return m_gs; }
    inline GifArbiter &gifArbiter() { return m_gifArbiter; }
    inline const GifArbiter &gifArbiter() const { return m_gifArbiter; }
    inline VU1Interpreter &vu0() { return m_vu0; }
    inline const VU1Interpreter &vu0() const { return m_vu0; }
    inline VU1Interpreter &vu1() { return m_vu1; }
    inline const VU1Interpreter &vu1() const { return m_vu1; }

    inline PS2AudioBackend &audioBackend() { return m_audioBackend; }
    inline const PS2AudioBackend &audioBackend() const { return m_audioBackend; }
    inline PSPadBackend &padBackend() { return m_padBackend; }
    inline const PSPadBackend &padBackend() const { return m_padBackend; }
    inline PS2RomDevice &romDevice() { return m_romDevice; }
    inline const PS2RomDevice &romDevice() const { return m_romDevice; }
    inline PS2Vfs &vfs() { return m_vfs; }
    inline const PS2Vfs &vfs() const { return m_vfs; }
    inline ps2x::iop::IopSubsystem *iopSubsystem() { return m_iopSubsystem.get(); }

private:
    struct GuestHeapBlock
    {
        uint32_t addr = 0;
        uint32_t size = 0;
        bool free = true;
    };

    static uint32_t alignGuestHeapValue(uint32_t value, uint32_t alignment);
    static bool isGuestHeapAlignmentValid(uint32_t alignment);
    static uint32_t normalizeGuestHeapAlignment(uint32_t alignment);
    uint32_t clampGuestHeapBase(uint32_t guestBase) const;
    uint32_t clampGuestHeapLimit(uint32_t guestLimit) const;
    void resetGuestHeapLocked(uint32_t guestBase, uint32_t guestLimit);
    void ensureGuestHeapInitializedLocked();
    int32_t findGuestHeapBlockIndexLocked(uint32_t guestAddr) const;
    uint32_t allocateGuestBlockLocked(uint32_t size, uint32_t alignment);
    void freeGuestBlockLocked(uint32_t guestAddr);
    void coalesceGuestHeapLocked();
    void HandleIntegerOverflow(R5900Context *ctx);

    [[nodiscard]] ps2x::iop::RpcAbi selectIopRpcAbi(const ps2x::iop::RpcAbiRequest &request) const;
    [[nodiscard]] bool canBindIopRpc(uint32_t sid) const noexcept;
    [[nodiscard]] ps2x::iop::RpcResult handleIopRpc(uint8_t *rdram, R5900Context *ctx, ps2x::iop::RpcRequest request);
    void notifyIopSifTransfer(uint8_t *rdram, const ps2x::iop::SifTransfer &transfer);
    void advanceIopEeCycles(uint64_t eeCycles) noexcept;
    // EE cycles until the IOP's next batch is due (see advanceIopEeCycles).
    [[nodiscard]] static uint64_t iopBatchSlack() noexcept;
    // Brings the IOP up to the EE's current cycle before an EE -> IOP call.
    void catchUpIopNow() const noexcept;
    void resetIop();

    friend class PS2IopTransport;
    friend class EeScheduler;

private:
    PS2Memory m_memory;
    GifArbiter m_gifArbiter;
    GS m_gs;
    std::unique_ptr<PS2IopHostAdapter> m_iopHost;
    std::unique_ptr<ps2x::iop::IopSubsystem> m_iopSubsystem;
    PS2AudioBackend m_audioBackend;
    PSPadBackend m_padBackend;
    PS2RomDevice m_romDevice;
    PS2Vfs m_vfs;
    VU1Interpreter m_vu0{VU1Interpreter::Unit::VU0};
    VU1Interpreter m_vu1{VU1Interpreter::Unit::VU1};
    R5900Context m_cpuContext;
    std::unique_ptr<EeScheduler> m_eeScheduler;
    mutable std::mutex m_eeKernelStateMutex;
    std::unordered_map<int, std::vector<EeExitHandlerRegistration>> m_eeExitHandlers;
    std::unordered_map<uint32_t, uint32_t> m_eeSyscallOverrides;
    std::unordered_set<uint32_t> m_eeSyscallMirrorAddresses;
    mutable std::mutex m_guestHeapMutex;
    mutable std::mutex m_asyncCallbackStackMutex;
    std::vector<GuestHeapBlock> m_guestHeapBlocks;
    uint32_t m_guestHeapBase = 0x00100000u;
    uint32_t m_guestHeapEnd = 0x00100000u;
    uint32_t m_guestHeapLimit = PS2_RAM_SIZE;
    uint32_t m_guestHeapSuggestedBase = 0x00100000u;
    bool m_guestHeapConfigured = false;
    uint32_t m_asyncCallbackStackFloor = 0x00048000u;
    uint32_t m_asyncCallbackStackTop = 0x00074000u;

    std::atomic<uint32_t> m_missingFunctionPolicy{static_cast<uint32_t>(MissingFunctionPolicy::ContinueToTarget)};
    std::atomic<bool> m_missingFunctionReported{false};
    std::atomic<bool> m_stopRequested{false};
    DebugUiCallback m_debugUiInitCallback = nullptr;
    DebugUiCallback m_debugUiDrawCallback = nullptr;
    DebugUiCallback m_debugUiShutdownCallback = nullptr;
    void *m_debugUiUserData = nullptr;
    bool m_debugUiInitialized = false;

public:
    std::atomic<uint32_t> m_debugPc{0};
    std::atomic<uint32_t> m_debugRa{0};
    std::atomic<uint32_t> m_debugSp{0};
    std::atomic<uint32_t> m_debugGp{0};

private:
    struct LoadedModule
    {
        std::string name;
        uint32_t baseAddress;
        size_t size;
        bool active;
    };

    std::vector<LoadedModule> m_loadedModules;
    uint8_t *m_boundRdram = nullptr;
    uint8_t *m_boundGSVram = nullptr;
};

// Generated by ps2xRecomp in ps2xRuntime/src/runner/register_functions.cpp.
extern const uint32_t g_ps2RecompiledFunctionTableBase;
extern const uint32_t g_ps2RecompiledFunctionTableEnd;
extern const uint32_t g_ps2RecompiledFunctionTableSlotCount;
extern PS2Runtime::RecompiledFunction g_ps2RecompiledFunctionTable[];

__attribute__((always_inline)) inline bool PS2Runtime::eeCheckpointDue(uint32_t cycles) noexcept
{
    // A flagged checkpoint has the gate closed (EeScheduler::flagCheckpoint):
    // one compare (round 16; the pending flag was read here too). Round 17:
    // a 32-bit add and compare (EeClock::charge).
    if (__builtin_expect(m_eeClock.charge(cycles), 1))
        return false;
    return eeCheckpointSlow();
}

__attribute__((always_inline)) inline bool PS2Runtime::eeCheckpointDueAt(R5900Context *ctx, uint32_t pc, uint32_t cycles) noexcept
{
    if (__builtin_expect(m_eeClock.charge(cycles), 1))
        return false;
    ctx->pc = pc;
    return eeCheckpointSlow();
}

__attribute__((always_inline)) inline PS2Runtime::RecompiledFunction PS2Runtime::guestFunctionAt(uint32_t target) const noexcept
{
    const uint32_t slot = (target - g_ps2RecompiledFunctionTableBase) >> 2;
    return ((target & 3u) == 0u && slot < g_ps2RecompiledFunctionTableSlotCount) ? g_ps2RecompiledFunctionTable[slot] : nullptr;
}

template <uint32_t Base, uint32_t Slots>
__attribute__((always_inline)) inline PS2Runtime::RecompiledFunction PS2Runtime::guestFunctionIn(uint32_t target) noexcept
{
    const uint32_t slot = (target - Base) >> 2;
    return ((target & 3u) == 0u && slot < Slots) ? g_ps2RecompiledFunctionTable[slot] : nullptr;
}

// The host stack's depth. GCC 14+ reads sp (__builtin_stack_address); the
// frame address made every generated function on the Vita set up a frame
// pointer (r7 taken from the register allocator) for this one compare.
__attribute__((always_inline)) inline bool PS2Runtime::hostStackDeep() const noexcept
{
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 14
    return reinterpret_cast<uintptr_t>(__builtin_stack_address()) < m_hostStackFloor;
#else
    return reinterpret_cast<uintptr_t>(__builtin_frame_address(0)) < m_hostStackFloor;
#endif
}

// Generated code's JAL / JALR: the callee runs as a host call through the
// function table (overrides included) after a checkpoint. Inline: no call
// into the runtime on the way (the Vita reaches it through a veneer).
__attribute__((always_inline)) inline bool PS2Runtime::callGuestFn(RecompiledFunction fn, uint8_t *rdram, R5900Context *ctx, uint32_t target,
                                                                   uint32_t returnPc, uint32_t sourcePc, GuestBranchKind kind, const char *debugName)
{
    ctx->pc = target;
    // m_hostStackFloor holds !m_directDispatch too (callGuestSlow tells them apart).
    if (__builtin_expect(!fn || hostStackDeep(), 0))
        return callGuestSlow(rdram, ctx, target, returnPc, sourcePc, kind, debugName);
    if (eeCheckpointDue(kEeDispatchCycles))
        return false;
    const uint32_t jumpSerial = m_guestJumpSerial;
    fn(rdram, ctx, this);
    if (__builtin_expect(ctx->pc == returnPc && !m_eeClock.unwinding, 1))
        return true;
    return finishGuestCall(ctx, target, returnPc, jumpSerial);
}

// callGuestFn with the function the recompiler sees at the target: called
// directly (a bl, not a blx through a register) while the slot holds it.
__attribute__((always_inline)) inline bool PS2Runtime::callGuestKnown(RecompiledFunction fn, RecompiledFunction known, uint8_t *rdram, R5900Context *ctx,
                                                                      uint32_t target, uint32_t returnPc, uint32_t sourcePc, GuestBranchKind kind,
                                                                      const char *debugName)
{
    ctx->pc = target;
    if (__builtin_expect(!fn || hostStackDeep(), 0))
        return callGuestSlow(rdram, ctx, target, returnPc, sourcePc, kind, debugName);
    if (eeCheckpointDue(kEeDispatchCycles))
        return false;
    const uint32_t jumpSerial = m_guestJumpSerial;
    if (__builtin_expect(fn == known, 1))
        known(rdram, ctx, this);
    else
        fn(rdram, ctx, this);
    if (__builtin_expect(ctx->pc == returnPc && !m_eeClock.unwinding, 1))
        return true;
    return finishGuestCall(ctx, target, returnPc, jumpSerial);
}

__attribute__((always_inline)) inline bool PS2Runtime::callGuest(uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t returnPc,
                                                                 uint32_t sourcePc, GuestBranchKind kind, const char *debugName)
{
    return callGuestFn(guestFunctionAt(target), rdram, ctx, target, returnPc, sourcePc, kind, debugName);
}

// A jump out of a generated function (J / JR to another function, a branch
// or fall-through past its end): the target runs nested when it can, else
// ctx->pc is left on it for the scheduler. The caller returns either way.
// Jumps can loop (J and JR between two functions): past the host stack
// budget they unwind to the scheduler.
__attribute__((always_inline)) inline void PS2Runtime::jumpGuestFn(RecompiledFunction fn, uint8_t *rdram, R5900Context *ctx, uint32_t target)
{
    ctx->pc = target;
    ++m_guestJumpSerial;
    if (__builtin_expect(!fn || hostStackDeep(), 0))
    {
        jumpGuestSlow(fn); // no function, PS2X_DIRECT_DISPATCH=0, or the stack's floor
        return;
    }
    if (eeCheckpointDue(kEeDispatchCycles))
        return;
    fn(rdram, ctx, this);
}

__attribute__((always_inline)) inline void PS2Runtime::jumpGuest(uint8_t *rdram, R5900Context *ctx, uint32_t target)
{
    jumpGuestFn(guestFunctionAt(target), rdram, ctx, target);
}


#endif // PS2_RUNTIME_H
