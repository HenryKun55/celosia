#include "ThreadNaming.h"
#include "runtime/ps2_thread_load.h"
#include "ps2_runtime_macros.h"
#include "runtime/ps2_stats.h"
#include <cstdlib>
#include <cstdio>
#include "runtime/ps2_memory.h"
#include "runtime/ps2_fast_paths.h"
#include "runtime/ps2_address.h"
#include "runtime/gs/gs_frontend.h"
#include "runtime/gs/gs_async_backend.h"
#include "ps2_log.h"
#include <atomic>
#include <map>
#include <mutex>
#include <iostream>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <optional>
#include "ps2_vif1_compact.h"
#include "ps2_gif_events.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace
{
    inline void inRange(uint32_t offset, size_t bytes, size_t regionSize, const char *op, uint32_t address)
    {
        if (static_cast<uint64_t>(offset) + static_cast<uint64_t>(bytes) > static_cast<uint64_t>(regionSize))
        {
            throw std::runtime_error(std::string(op) + " out-of-bounds at address: 0x" + std::to_string(address));
        }
    }

    template <typename T>
    inline T loadScalar(const uint8_t *base, uint32_t offset, size_t regionSize, const char *op, uint32_t address)
    {
        inRange(offset, sizeof(T), regionSize, op, address);
        T value{};
        std::memcpy(&value, base + offset, sizeof(T));
        return value;
    }

    template <typename T>
    inline void storeScalar(uint8_t *base, uint32_t offset, size_t regionSize, T value, const char *op, uint32_t address)
    {
        inRange(offset, sizeof(T), regionSize, op, address);
        std::memcpy(base + offset, &value, sizeof(T));
    }

    inline bool isGsPrivReg(uint32_t addr)
    {
        return Ps2AddressInRange(addr, PS2_GS_PRIV_REG_BASE, PS2_GS_PRIV_REG_SIZE);
    }

    inline bool isIoRegister(uint32_t addr)
    {
        return Ps2AddressInRange(addr, PS2_IO_BASE, PS2_IO_SIZE);
    }

    inline uint64_t *gsRegPtr(GSRegisters &gs, uint32_t addr)
    {
        // Support both 64-bit base offsets and +4 dword aliases.
        uint32_t off = (addr - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        switch (off)
        {
        case 0x0000:
            return &gs.pmode;
        case 0x0010:
            return &gs.smode1;
        case 0x0020:
            return &gs.smode2;
        case 0x0030:
            return &gs.srfsh;
        case 0x0040:
            return &gs.synch1;
        case 0x0050:
            return &gs.synch2;
        case 0x0060:
            return &gs.syncv;
        case 0x0070:
            return &gs.dispfb1;
        case 0x0080:
            return &gs.display1;
        case 0x0090:
            return &gs.dispfb2;
        case 0x00A0:
            return &gs.display2;
        case 0x00B0:
            return &gs.extbuf;
        case 0x00C0:
            return &gs.extdata;
        case 0x00D0:
            return &gs.extwrite;
        case 0x00E0:
            return &gs.bgcolor;
        // CSR (offset 0x1000) is intentionally not handled here: it is
        // std::atomic<uint64_t> and no longer converts to uint64_t*. Callers must
        // check for offset 0x1000 themselves and go through writeCsrHalf/
        // writeCsrFull/gs.csr.load() instead of gsRegPtr().
        case 0x1010:
            return &gs.imr;
        case 0x1040:
            return &gs.busdir;
        case 0x1080:
            return &gs.siglblid;
        default:
            return nullptr;
        }
    }

    constexpr uint32_t kGsCsrRegOffset = 0x1000u;

    // Atomically apply a 32-bit write to one half (off=0 low dword, off=4 high
    // dword) of the GS CSR register. Bits 0..1 of the low dword (SIGNAL/FINISH) are
    // write-one-to-clear; everything else is a plain merge. Uses compare_exchange
    // so the whole read-modify-write is a single atomic step -- this register is
    // also touched by the vsync worker (FIELD bit) and the GIF (SIGNAL/FINISH) on
    // other threads, so a load-then-store here would race with them.
    inline void writeCsrHalf(std::atomic<uint64_t> &csr, uint32_t off, uint32_t value)
    {
        constexpr uint32_t kW1cMask = 0x3u;
        uint64_t expected = csr.load();
        uint64_t desired;
        do
        {
            if (off == 0u)
            {
                uint32_t oldLow = static_cast<uint32_t>(expected & 0xFFFFFFFFull);
                uint32_t mergedLow = (oldLow & kW1cMask) | (value & ~kW1cMask);
                desired = (expected & 0xFFFFFFFF00000000ull) | static_cast<uint64_t>(mergedLow);
                desired &= ~static_cast<uint64_t>(value & kW1cMask);
            }
            else
            {
                uint64_t mask = 0xFFFFFFFFull << (off * 8u);
                desired = (expected & ~mask) | (static_cast<uint64_t>(value) << (off * 8u));
            }
        } while (!csr.compare_exchange_weak(expected, desired));
    }

    // Same as writeCsrHalf but for a full 64-bit CSR write (bits 0..1 are still
    // write-one-to-clear against the current value).
    inline void writeCsrFull(std::atomic<uint64_t> &csr, uint64_t value)
    {
        constexpr uint64_t kW1cMask = 0x3ull;
        uint64_t expected = csr.load();
        uint64_t desired;
        do
        {
            desired = (expected & kW1cMask) | (value & ~kW1cMask);
            desired &= ~(value & kW1cMask);
        } while (!csr.compare_exchange_weak(expected, desired));
    }

    constexpr std::array<uint32_t, 4> kEeTimerBases = {
        0x10000000u,
        0x10000800u,
        0x10001000u,
        0x10001800u,
    };
    constexpr uint32_t kEeTimerCountOffset = 0x00u;
    constexpr uint32_t kEeTimerModeOffset = 0x10u;
    constexpr uint32_t kEeTimerCompareOffset = 0x20u;
    constexpr uint32_t kEeTimerHoldOffset = 0x30u;
    constexpr uint32_t kEeTimerModeClksMask = 0x3u;
    constexpr uint32_t kEeTimerModeConfigMask = 0x3FFu;
    constexpr uint32_t kEeTimerModeStatusMask = 0xC00u;
    constexpr uint32_t kEeTimerModeZret = 1u << 6;
    constexpr uint32_t kEeTimerModeCue = 1u << 7;
    constexpr uint32_t kEeTimerModeCmpe = 1u << 8;
    constexpr uint32_t kEeTimerModeOvfe = 1u << 9;
    constexpr uint32_t kEeTimerModeEquf = 1u << 10;
    constexpr uint32_t kEeTimerModeOvff = 1u << 11;
    constexpr uint64_t kEeClockHz = 294912000ull;
    constexpr std::array<uint64_t, 4> kEeTimerClockHz = {
        147456000ull,
        9216000ull,
        576000ull,
        15734ull,
    };

    inline bool decodeEeTimerRegister(uint32_t address, size_t &timerIndex, uint32_t &offset)
    {
        for (size_t index = 0; index < kEeTimerBases.size(); ++index)
        {
            const uint32_t candidateOffset = address - kEeTimerBases[index];
            if (candidateOffset == kEeTimerCountOffset ||
                candidateOffset == kEeTimerModeOffset ||
                candidateOffset == kEeTimerCompareOffset ||
                (index < 2u && candidateOffset == kEeTimerHoldOffset))
            {
                timerIndex = index;
                offset = candidateOffset;
                return true;
            }
        }
        return false;
    }

    constexpr uint64_t ticksUntilMatch(uint32_t count, uint32_t target)
    {
        const uint32_t distance = (target - count) & 0xFFFFu;
        return distance == 0u ? 0x10000ull : static_cast<uint64_t>(distance);
    }

    struct DmaTagView
    {
        uint16_t qwc = 0;
        uint8_t id = 0;
        bool irq = false;
        uint32_t addr = 0;
        uint32_t upper = 0;
    };

    inline DmaTagView decodeDmaTag(uint64_t tag)
    {
        DmaTagView out{};
        out.qwc = static_cast<uint16_t>(tag & 0xFFFFu);
        out.id = static_cast<uint8_t>((tag >> 28u) & 0x7u);
        out.irq = ((tag >> 31u) & 0x1ull) != 0ull;
        out.addr = static_cast<uint32_t>((tag >> 32u) & 0x7FFFFFFFu);
        out.upper = static_cast<uint32_t>((tag >> 16u) & 0xFFFFu);
        return out;
    }

    inline uint32_t gifTagNloop(uint64_t tagLo)
    {
        return static_cast<uint32_t>(tagLo & 0x7FFFu);
    }

    inline uint8_t gifTagFlg(uint64_t tagLo)
    {
        return static_cast<uint8_t>((tagLo >> 58u) & 0x3u);
    }

    inline uint32_t gifTagNreg(uint64_t tagLo)
    {
        uint32_t nreg = static_cast<uint32_t>((tagLo >> 60u) & 0xFu);
        return nreg == 0u ? 16u : nreg;
    }

}

// Helpers for GS VRAM addressing (PSMCT32 path).
static inline uint32_t gs_vram_offset(uint32_t basePage, uint32_t x, uint32_t y, uint32_t fbw)
{
    // basePage is in 2048-byte units; fbw is in blocks of 64 pixels.
    uint32_t strideBytes = fbw * 64 * 4;
    return basePage * 2048 + y * strideBytes + x * 4;
}

PS2Memory::PS2Memory()
    : m_rdram(nullptr), m_scratchpad(nullptr), iop_ram(nullptr), m_seenGifCopy(false), m_gsVRAM(nullptr)
{
    ps2SetScratchpadHostPtr(nullptr);
    const char *mtvu = std::getenv("PS2X_MTVU");
    m_mtvuEnabled = !mtvu || std::atoi(mtvu) != 0; // PS2X_MTVU=0: VU1 on the EE thread
}

namespace
{
    thread_local bool t_onVu1Worker = false;
}

const std::align_val_t PS2Memory::kGsVramAlignment{16384};

bool PS2Memory::onVu1Worker()
{
    return t_onVu1Worker;
}

// Each job holds a copy of its DMA data: when the VU1 worker falls behind
// (the Vita in a heavy scene: vu1 100%), the queue grew by tens of MB in
// seconds and ran the heap out. Past the high mark the caller waits for it.
constexpr size_t kMtvuQueueHigh = 16u << 20, kMtvuQueueLow = 8u << 20;

void PS2Memory::mtvuEnqueue(MtvuJob &&job)
{
    {
        std::unique_lock<std::mutex> lock(m_mtvuMutex);
        if (!m_mtvuThread.joinable())
            m_mtvuThread = std::thread([this]
                                       { mtvuWorkerLoop(); });
        if (m_mtvuQueuedBytes > kMtvuQueueHigh && !onVu1Worker())
        {
            ps2_load::Wait waiting;
            m_mtvuSpace.wait(lock, [this]
                             { return m_mtvuQueuedBytes <= kMtvuQueueLow || m_mtvuStop; });
        }
        m_mtvuOutstanding.fetch_add(1, std::memory_order_release);
        m_mtvuQueuedBytes += job.data.size();
        m_mtvuQueue.push_back(std::move(job));
    }
    m_mtvuWork.notify_one();
}

void PS2Memory::mtvuWorkerLoop()
{
    t_onVu1Worker = true;
    ThreadNaming::SetCurrentThreadName("VU1");
    ps2_load::setThreadSlot(ps2_load::kVu1);
    // VU1 float math rounds toward zero, like the game thread's.
    ps2SetEeFloatingPointMode();
    // GS draws from VIF1/GIF reach the GS worker in bulk, once per job.
    GSAsyncBackend::SetThreadStaging(true);
    std::unique_lock<std::mutex> lock(m_mtvuMutex);
    for (;;)
    {
        {
            ps2_load::Wait idle;
            m_mtvuWork.wait(lock, [this]
                            { return m_mtvuStop || !m_mtvuQueue.empty(); });
        }
        if (m_mtvuQueue.empty())
            return;
        MtvuJob job = std::move(m_mtvuQueue.front());
        m_mtvuQueue.pop_front();
        m_mtvuQueuedBytes -= job.data.size();
        if (m_mtvuQueuedBytes <= kMtvuQueueLow)
            m_mtvuSpace.notify_all();
        lock.unlock();
        m_mtvuFbrst = job.fbrst;
        if (job.marker)
            job.marker();
        else if (job.gif)
            submitGifPacket(GifPathId::Path3, job.data.data(), static_cast<uint32_t>(job.data.size()), false);
        else
            processVIF1Data(job.data.data(), static_cast<uint32_t>(job.data.size()));
        if (m_gifArbiter)
            m_gifArbiter->drain();
        GSAsyncBackend::PublishThreadStaged();
        ps2_stats::tlVu1DoneUs.store(ps2_stats::nowUs(), std::memory_order_relaxed);
        ps2_stats::tlDoneVu1IdleUs.store(ps2_load::waitUs[ps2_load::kVu1].load(std::memory_order_relaxed), std::memory_order_relaxed);
        if (m_mtvuJobEndHook)
            m_mtvuJobEndHook();
        lock.lock();
        if (m_mtvuOutstanding.fetch_sub(1, std::memory_order_acq_rel) == 1u)
            m_mtvuIdle.notify_all();
    }
}

void PS2Memory::runInVu1Order(std::function<void()> fn)
{
    if (!m_mtvuEnabled || onVu1Worker() || m_mtvuOutstanding.load(std::memory_order_acquire) == 0u)
    {
        fn();
        return;
    }
    MtvuJob job;
    job.marker = std::move(fn);
    job.fbrst = m_mtvuFbrst;
    mtvuEnqueue(std::move(job));
}

void PS2Memory::mtvuWorkDone()
{
    std::lock_guard<std::mutex> lock(m_mtvuMutex);
    if (m_mtvuOutstanding.fetch_sub(1, std::memory_order_acq_rel) == 1u)
        m_mtvuIdle.notify_all();
}

void PS2Memory::mtvuSyncSlow()
{
    ps2_load::Wait waiting;
    std::unique_lock<std::mutex> lock(m_mtvuMutex);
    m_mtvuIdle.wait(lock, [this]
                    { return m_mtvuOutstanding.load(std::memory_order_acquire) == 0u; });
}

std::vector<uint8_t> PS2Memory::copyTransferData(const PendingTransfer &transfer)
{
    if (!transfer.chainData.empty())
        return transfer.chainData;
    std::vector<uint8_t> out;
    if (transfer.qwc == 0u)
        return out;
    uint32_t srcPhys = 0;
    try
    {
        srcPhys = translateAddress(transfer.srcAddr);
    }
    catch (const std::exception &)
    {
        return out;
    }
    const uint8_t *base = transfer.fromScratchpad ? m_scratchpad : m_rdram;
    const uint32_t limit = transfer.fromScratchpad ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
    uint64_t left = static_cast<uint64_t>(transfer.qwc) * 16u;
    out.reserve(static_cast<size_t>(std::min<uint64_t>(left, limit)));
    while (left > 0u)
    {
        if (srcPhys >= limit)
            srcPhys = 0;
        const uint32_t chunk = static_cast<uint32_t>(std::min<uint64_t>(left, limit - srcPhys));
        out.insert(out.end(), base + srcPhys, base + srcPhys + chunk);
        left -= chunk;
        srcPhys += chunk;
    }
    return out;
}

PS2Memory::~PS2Memory()
{
    {
        std::lock_guard<std::mutex> lock(m_mtvuMutex);
        m_mtvuStop = true;
    }
    m_mtvuWork.notify_all();
    m_mtvuSpace.notify_all();
    if (m_mtvuThread.joinable())
        m_mtvuThread.join();
    if (m_rdram)
    {
        delete[] m_rdram;
        m_rdram = nullptr;
    }

    if (m_scratchpad)
    {
        ps2SetScratchpadHostPtr(nullptr);
        delete[] m_scratchpad;
        m_scratchpad = nullptr;
    }

    if (m_gsVRAM)
    {
        ::operator delete[](m_gsVRAM, kGsVramAlignment);
        m_gsVRAM = nullptr;
    }

    if (m_vu1Code)
    {
        delete[] m_vu1Code;
        m_vu1Code = nullptr;
    }
    if (m_vu1Data)
    {
        delete[] m_vu1Data;
        m_vu1Data = nullptr;
    }
    if (m_vu0Code)
    {
        delete[] m_vu0Code;
        m_vu0Code = nullptr;
    }
    if (m_vu0Data)
    {
        delete[] m_vu0Data;
        m_vu0Data = nullptr;
    }

    if (iop_ram)
    {
        delete[] iop_ram;
        iop_ram = nullptr;
    }
}

bool PS2Memory::initialize(size_t ramSize)
{
    auto cleanup = [this]()
    {
        delete[] m_rdram;
        delete[] m_scratchpad;
        delete[] iop_ram;
        if (m_gsVRAM)
            ::operator delete[](m_gsVRAM, kGsVramAlignment);
        delete[] m_vu0Code;
        delete[] m_vu0Data;
        delete[] m_vu1Code;
        delete[] m_vu1Data;
        m_rdram = nullptr;
        m_scratchpad = nullptr;
        ps2SetScratchpadHostPtr(nullptr);
        iop_ram = nullptr;
        m_gsVRAM = nullptr;
        m_vu0Code = nullptr;
        m_vu0Data = nullptr;
        m_vu1Code = nullptr;
        m_vu1Data = nullptr;
    };

    cleanup();
    m_seenGifCopy = false;
    m_dmaStartCount.store(0, std::memory_order_relaxed);
    m_gifCopyCount.store(0, std::memory_order_relaxed);
    m_gsWriteCount.store(0, std::memory_order_relaxed);
    m_vifWriteCount.store(0, std::memory_order_relaxed);
    {
        std::lock_guard<std::mutex> lock(m_completedDmacMutex);
        m_completedDmacCauses.clear();
    }
    m_codeRegions.clear();
    m_path3Masked = false;
    m_path3MaskedFifo.clear();
    m_vif1PendingPath2ImageQwc = 0u;
    m_vif1PendingPath2DirectHl = false;
    resetEeTimers();

    try
    {
        // Allocate main RAM
        // A 16-byte tail past the end: the guest load / store macros read
        // RAM in place on one compare (ps2_runtime_macros.h READ* / WRITE*),
        // they may touch it on an access
        // that starts in the last bytes (a guest access is aligned: never).
        m_rdram = new uint8_t[ramSize + 16u];
        std::memset(m_rdram, 0, ramSize + 16u);

        // Allocate scratchpad
        m_scratchpad = new uint8_t[PS2_SCRATCHPAD_SIZE];
        std::memset(m_scratchpad, 0, PS2_SCRATCHPAD_SIZE);
        ps2SetScratchpadHostPtr(m_scratchpad);

        // Initialize EE TLB entries (R5900 has 48 entries).
        m_tlbEntries.assign(48, TLBEntry{0, 0, 0, false});

        // Allocate IOP RAM
        iop_ram = new uint8_t[2 * 1024 * 1024]; // 2MB

        // Initialize IOP RAM with zeros
        std::memset(iop_ram, 0, 2 * 1024 * 1024);

        // Initialize I/O registers
        m_ioRegisters.clear();

        // Initialize GS registers
        memset(&gs_regs, 0, sizeof(gs_regs));
        // memset zero-fills std::atomic<uint64_t>::csr's bytes, which is not itself
        // a guaranteed-valid atomic store; make the zero-initialization explicit.
        gs_regs.csr.store(0);
        gs_regs.dispfb1 = (0ULL << 0) | (10ULL << 9) | (0ULL << 15) | (0ULL << 32) | (0ULL << 43);
        gs_regs.display1 = (0ULL << 0) | (0ULL << 12) | (0ULL << 23) | (0ULL << 27) | (639ULL << 32) | (447ULL << 44);
        gs_regs.dispfb2 = gs_regs.dispfb1;
        gs_regs.display2 = gs_regs.display1;

        // Allocate GS VRAM (4MB), page aligned so a GPU rasterizer can share it.
        m_gsVRAM = static_cast<uint8_t *>(::operator new[](PS2_GS_VRAM_SIZE, kGsVramAlignment));
        std::memset(m_gsVRAM, 0, PS2_GS_VRAM_SIZE);

        m_vu0Code = new uint8_t[PS2_VU0_CODE_SIZE];
        m_vu0Data = new uint8_t[PS2_VU0_DATA_SIZE];
        std::memset(m_vu0Code, 0, PS2_VU0_CODE_SIZE);
        std::memset(m_vu0Data, 0, PS2_VU0_DATA_SIZE);

        m_vu1Code = new uint8_t[PS2_VU1_CODE_SIZE];
        m_vu1Data = new uint8_t[PS2_VU1_DATA_SIZE];
        std::memset(m_vu1Code, 0, PS2_VU1_CODE_SIZE);
        std::memset(m_vu1Data, 0, PS2_VU1_DATA_SIZE);
        markVU0CodeModified();
        markVU1CodeModified();

        // Initialize VIF registers
        memset(&vif0_regs, 0, sizeof(vif0_regs));
        memset(&vif1_regs, 0, sizeof(vif1_regs));

        // Initialize DMA registers
        memset(dma_regs, 0, sizeof(dma_regs));

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error initializing PS2 memory: " << e.what() << std::endl;
        cleanup();
        return false;
    }
}

void PS2Memory::resetEeTimers() noexcept
{
    m_eeTimers = {};
}

uint32_t PS2Memory::flushEeTimers() noexcept
{
    const uint64_t cycles = m_timerDeferred;
    m_timerDeferred = 0u;
    uint32_t mask = m_timerIrqLatched;
    m_timerIrqLatched = 0u;
    if (cycles != 0u)
        mask |= advanceEeTimersNow(cycles);
    m_timerBudget = cyclesUntilNextEeTimerInterruptNow();
    if (m_eeClock)
        m_eeClock->closeGate();
    return mask;
}

uint64_t PS2Memory::cyclesUntilNextEeTimerInterrupt() const noexcept
{
    const uint64_t next = cyclesUntilNextEeTimerInterruptNow();
    if (next == std::numeric_limits<uint64_t>::max())
        return next;
    return next > m_timerDeferred ? next - m_timerDeferred : 1u;
}

uint32_t PS2Memory::advanceEeTimersNow(uint64_t eeCycles) noexcept
{
    if (eeCycles == 0u)
    {
        return 0u;
    }

    constexpr uint32_t kGifStat = 0x10003020u;
    constexpr uint32_t kGifFqcMask = 0x1F000000u;
    auto gifStatIt = m_ioRegisters.find(kGifStat);
    if (gifStatIt != m_ioRegisters.end())
        gifStatIt->second &= ~kGifFqcMask;

    uint32_t interruptMask = 0u;
    for (size_t index = 0; index < m_eeTimers.size(); ++index)
    {
        EeTimer &timer = m_eeTimers[index];
        if ((timer.mode & kEeTimerModeCue) == 0u)
        {
            continue;
        }

        const uint64_t clockHz = kEeTimerClockHz[timer.mode & kEeTimerModeClksMask];
        const uint64_t wholeSeconds = eeCycles / kEeClockHz;
        const uint64_t remainingCycles = eeCycles % kEeClockHz;
        const uint64_t scaled = remainingCycles * clockHz + timer.clockRemainder;
        const uint64_t ticks = wholeSeconds * clockHz + scaled / kEeClockHz;
        timer.clockRemainder = scaled % kEeClockHz;
        if (ticks == 0u)
        {
            continue;
        }

        const uint32_t oldCount = timer.count & 0xFFFFu;
        const uint32_t compare = timer.compare & 0xFFFFu;
        const uint64_t compareDistance = ticksUntilMatch(oldCount, compare);
        const uint64_t overflowDistance = 0x10000ull - oldCount;
        const bool zeroReturn = (timer.mode & kEeTimerModeZret) != 0u;
        const bool compareReached = ticks >= compareDistance;
        bool overflowReached = false;
        // How many times each event happened in this batch.
        uint64_t compareEvents = 0, overflowEvents = 0;
        if (compareReached)
        {
            const uint64_t period = zeroReturn ? std::max<uint64_t>(compare, 1u) : 0x10000ull;
            compareEvents = 1u + (ticks - compareDistance) / period;
        }

        if (zeroReturn)
        {
            overflowReached = ticks >= overflowDistance && overflowDistance <= compareDistance;
            if (compareReached)
            {
                const uint64_t remaining = ticks - compareDistance;
                timer.count = compare == 0u
                                  ? static_cast<uint32_t>(remaining & 0xFFFFu)
                                  : static_cast<uint32_t>(remaining % compare);
            }
            else
            {
                timer.count = static_cast<uint32_t>((oldCount + ticks) & 0xFFFFu);
            }
        }
        else
        {
            overflowReached = ticks >= overflowDistance;
            overflowEvents = (static_cast<uint64_t>(oldCount) + ticks) >> 16;
            timer.count = static_cast<uint32_t>((oldCount + ticks) & 0xFFFFu);
        }
        if (overflowReached && overflowEvents == 0u)
            overflowEvents = 1u;

        constexpr uint64_t kMaxBacklog = 256u;
        if (compareEvents != 0u && (timer.mode & kEeTimerModeCmpe) != 0u)
        {
            if ((timer.mode & kEeTimerModeEquf) == 0u)
            {
                timer.mode |= kEeTimerModeEquf;
                interruptMask |= 1u << index;
                --compareEvents;
            }
            timer.compareBacklog = static_cast<uint32_t>(std::min<uint64_t>(kMaxBacklog, timer.compareBacklog + compareEvents));
        }
        if (overflowEvents != 0u && (timer.mode & kEeTimerModeOvfe) != 0u)
        {
            if ((timer.mode & kEeTimerModeOvff) == 0u)
            {
                timer.mode |= kEeTimerModeOvff;
                interruptMask |= 1u << index;
                --overflowEvents;
            }
            timer.overflowBacklog = static_cast<uint32_t>(std::min<uint64_t>(kMaxBacklog, timer.overflowBacklog + overflowEvents));
        }
    }
    return interruptMask;
}

uint64_t PS2Memory::cyclesUntilNextEeTimerInterruptNow() const noexcept
{
    uint64_t nearest = std::numeric_limits<uint64_t>::max();
    for (const EeTimer &timer : m_eeTimers)
    {
        if ((timer.mode & kEeTimerModeCue) == 0u)
        {
            continue;
        }

        const uint32_t count = timer.count & 0xFFFFu;
        const uint32_t compare = timer.compare & 0xFFFFu;
        const uint64_t compareDistance = ticksUntilMatch(count, compare);
        const uint64_t overflowDistance = 0x10000ull - count;
        uint64_t eventTicks = std::numeric_limits<uint64_t>::max();

        if ((timer.mode & kEeTimerModeCmpe) != 0u &&
            (timer.mode & kEeTimerModeEquf) == 0u)
        {
            eventTicks = compareDistance;
        }
        const bool overflowCanOccur = (timer.mode & kEeTimerModeZret) == 0u ||
                                      overflowDistance <= compareDistance;
        if (overflowCanOccur &&
            (timer.mode & kEeTimerModeOvfe) != 0u &&
            (timer.mode & kEeTimerModeOvff) == 0u)
        {
            eventTicks = std::min(eventTicks, overflowDistance);
        }
        if (eventTicks == std::numeric_limits<uint64_t>::max())
        {
            continue;
        }

        const uint64_t clockHz = kEeTimerClockHz[timer.mode & kEeTimerModeClksMask];
        const uint64_t numerator = eventTicks * kEeClockHz - timer.clockRemainder;
        const uint64_t cycles = (numerator + clockHz - 1u) / clockHz;
        nearest = std::min(nearest, std::max<uint64_t>(1u, cycles));
    }
    return nearest;
}

bool PS2Memory::isScratchpad(uint32_t address) const
{
    return ps2IsScratchpadAddress(address);
}

uint8_t *PS2Memory::mapVuMemory(uint32_t physAddr, uint32_t size, uint32_t &offset, uint32_t &limit)
{
    return const_cast<uint8_t *>(static_cast<const PS2Memory *>(this)->mapVuMemory(physAddr, size, offset, limit));
}

const uint8_t *PS2Memory::mapVuMemory(uint32_t physAddr, uint32_t size, uint32_t &offset, uint32_t &limit) const
{
    if (physAddr >= PS2_VU1_CODE_BASE && physAddr < PS2_VU1_DATA_BASE + PS2_VU1_DATA_SIZE)
        const_cast<PS2Memory *>(this)->syncVu1();
    auto mapRange = [&](uint32_t base, uint32_t rangeSize, const uint8_t *ptr) -> const uint8_t *
    {
        if (!ptr || physAddr < base)
        {
            return nullptr;
        }
        const uint32_t local = physAddr - base;
        if (local >= rangeSize || size > (rangeSize - local))
        {
            return nullptr;
        }
        offset = local;
        limit = rangeSize;
        return ptr;
    };

    if (const uint8_t *ptr = mapRange(PS2_VU0_CODE_BASE, PS2_VU0_CODE_SIZE, m_vu0Code))
    {
        return ptr;
    }
    if (const uint8_t *ptr = mapRange(PS2_VU0_DATA_BASE, PS2_VU0_DATA_SIZE, m_vu0Data))
    {
        return ptr;
    }
    if (const uint8_t *ptr = mapRange(PS2_VU1_CODE_BASE, PS2_VU1_CODE_SIZE, m_vu1Code))
    {
        return ptr;
    }
    return mapRange(PS2_VU1_DATA_BASE, PS2_VU1_DATA_SIZE, m_vu1Data);
}

uint32_t PS2Memory::translateAddress(uint32_t virtualAddress)
{
    if (isScratchpad(virtualAddress))
    {
        return ps2ScratchpadOffset(virtualAddress);
    }

    // EE uncached aliases of main RAM (per PS2 memory map):
    //   0x20000000-0x3FFFFFFF -> 32MB mirror of RDRAM
    // This includes the accelerated window rooted at 0x30100000.
    if (Ps2IsUncachedRamMirrorAddress(virtualAddress))
    {
        return virtualAddress & PS2_RAM_MASK;
    }

    // KSEG0/KSEG1 direct-mapped window.
    if (Ps2IsKseg01Address(virtualAddress))
    {
        return Ps2DirectMappedPhysicalAddress(virtualAddress);
    }

    // In this runtime, low segments are treated as physical-style addresses already.
    if (virtualAddress < 0x80000000)
    {
        return virtualAddress;
    }

    // KSEG2/KSEG3 are TLB mapped.
    if (Ps2IsKseg23Address(virtualAddress))
    {
        for (const auto &entry : m_tlbEntries)
        {
            if (entry.valid)
            {
                // PageMask uses bits [24:13]. Build an address-level mask (plus 4KB base page bits).
                const uint32_t mask = entry.mask & 0x01FFE000u;
                const uint32_t compareMask = ~(mask | 0xFFFu);
                if ((virtualAddress & compareMask) == (entry.vpn & compareMask))
                {
                    // TLB hit
                    const uint32_t pageOffsetMask = mask | 0xFFFu;
                    const uint32_t physBase = entry.pfn << 12;
                    return physBase | (virtualAddress & pageOffsetMask);
                }
            }
        }
        throw std::runtime_error("TLB miss for address: 0x" + std::to_string(virtualAddress));
    }

    return virtualAddress;
}

bool PS2Memory::tlbRead(uint32_t index, uint32_t &vpn, uint32_t &pfn, uint32_t &mask, bool &valid) const
{
    if (index >= m_tlbEntries.size())
    {
        return false;
    }

    const TLBEntry &entry = m_tlbEntries[index];
    vpn = entry.vpn;
    pfn = entry.pfn;
    mask = entry.mask;
    valid = entry.valid;
    return true;
}

bool PS2Memory::tlbWrite(uint32_t index, uint32_t vpn, uint32_t pfn, uint32_t mask, bool valid)
{
    if (index >= m_tlbEntries.size())
    {
        return false;
    }

    TLBEntry &entry = m_tlbEntries[index];
    entry.vpn = vpn & 0xFFFFF000u;
    entry.pfn = pfn & 0x000FFFFFu;
    entry.mask = mask & 0x01FFE000u;
    entry.valid = valid;
    return true;
}

int32_t PS2Memory::tlbProbe(uint32_t vpn) const
{
    const uint32_t normalizedVpn = vpn & 0xFFFFF000u;
    for (uint32_t i = 0; i < static_cast<uint32_t>(m_tlbEntries.size()); ++i)
    {
        const TLBEntry &entry = m_tlbEntries[i];
        if (!entry.valid)
        {
            continue;
        }

        const uint32_t mask = entry.mask & 0x01FFE000u;
        const uint32_t compareMask = ~(mask | 0xFFFu);
        if ((normalizedVpn & compareMask) == (entry.vpn & compareMask))
        {
            return static_cast<int32_t>(i);
        }
    }

    return -1;
}

uint8_t PS2Memory::read8(uint32_t address)
{
    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        return m_scratchpad[physAddr];
    }
    if (physAddr < PS2_RAM_SIZE)
    {
        return m_rdram[physAddr];
    }
    uint32_t vuOffset = 0;
    uint32_t vuLimit = 0;
    if (const uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint8_t), vuOffset, vuLimit))
    {
        (void)vuLimit;
        return vuMem[vuOffset];
    }
    else if (isIoRegister(physAddr))
    {
        uint32_t regAddr = physAddr & ~0x3;
        uint32_t value = readIORegister(regAddr);
        uint32_t shift = (physAddr & 3) * 8;
        return static_cast<uint8_t>((value >> shift) & 0xFF);
    }

    return 0;
}

uint16_t PS2Memory::read16(uint32_t address)
{
    if (address & 1)
    {
        throw std::runtime_error("Unaligned 16-bit read at address: 0x" + std::to_string(address));
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        return loadScalar<uint16_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, "read16 scratchpad", address);
    }
    if (physAddr < PS2_RAM_SIZE)
    {
        return loadScalar<uint16_t>(m_rdram, physAddr, PS2_RAM_SIZE, "read16 rdram", address);
    }
    uint32_t vuOffset = 0;
    uint32_t vuLimit = 0;
    if (const uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint16_t), vuOffset, vuLimit))
    {
        return loadScalar<uint16_t>(vuMem, vuOffset, vuLimit, "read16 vu", address);
    }
    else if (isIoRegister(physAddr))
    {
        uint32_t regAddr = physAddr & ~0x3;
        uint32_t value = readIORegister(regAddr);
        uint32_t shift = (physAddr & 2) * 8;
        return static_cast<uint16_t>((value >> shift) & 0xFFFF);
    }

    return 0;
}

uint32_t PS2Memory::read32(uint32_t address)
{
    if (address & 3)
    {
        throw std::runtime_error("Unaligned 32-bit read at address: 0x" + std::to_string(address));
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        uint32_t off = address & 7;
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            uint64_t val = gs_regs.csr.load();
            return (uint32_t)(val >> (off * 8));
        }
        uint64_t *reg = gsRegPtr(gs_regs, address);
        if (!reg)
            return 0;
        uint64_t val = *reg;
        return (uint32_t)(val >> (off * 8));
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        return loadScalar<uint32_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, "read32 scratchpad", address);
    }
    if (physAddr < PS2_RAM_SIZE)
    {
        return loadScalar<uint32_t>(m_rdram, physAddr, PS2_RAM_SIZE, "read32 rdram", address);
    }
    uint32_t vuOffset = 0;
    uint32_t vuLimit = 0;
    if (const uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint32_t), vuOffset, vuLimit))
    {
        return loadScalar<uint32_t>(vuMem, vuOffset, vuLimit, "read32 vu", address);
    }
    else if (isIoRegister(physAddr))
    {
        return readIORegister(physAddr);
    }

    return 0;
}

uint64_t PS2Memory::read64(uint32_t address)
{
    if (address & 7)
    {
        throw std::runtime_error("Unaligned 64-bit read at address: 0x" + std::to_string(address));
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            return gs_regs.csr.load();
        }
        uint64_t *reg = gsRegPtr(gs_regs, address);
        return reg ? *reg : 0;
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        return loadScalar<uint64_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, "read64 scratchpad", address);
    }
    if (physAddr < PS2_RAM_SIZE)
    {
        return loadScalar<uint64_t>(m_rdram, physAddr, PS2_RAM_SIZE, "read64 rdram", address);
    }
    uint32_t vuOffset = 0;
    uint32_t vuLimit = 0;
    if (const uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint64_t), vuOffset, vuLimit))
    {
        return loadScalar<uint64_t>(vuMem, vuOffset, vuLimit, "read64 vu", address);
    }

    // 64-bit IO read: compose from the two adjacent 32-bit IO register slots
    // to avoid any side-effects from read32 handlers.
    if (isIoRegister(address))
    {
        uint32_t lo = m_ioRegisters.count(address) ? m_ioRegisters[address] : 0u;
        uint32_t hi = m_ioRegisters.count(address + 4) ? m_ioRegisters[address + 4] : 0u;
        return static_cast<uint64_t>(lo) | (static_cast<uint64_t>(hi) << 32);
    }
    return (uint64_t)read32(address) | ((uint64_t)read32(address + 4) << 32);
}

__m128i PS2Memory::read128(uint32_t address)
{
    if (address & 15)
    {
        throw std::runtime_error("Unaligned 128-bit read at address: 0x" + std::to_string(address));
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        inRange(physAddr, sizeof(__m128i), PS2_SCRATCHPAD_SIZE, "read128 scratchpad", address);
        return _mm_loadu_si128(reinterpret_cast<__m128i *>(&m_scratchpad[physAddr]));
    }
    if (physAddr < PS2_RAM_SIZE)
    {
        inRange(physAddr, sizeof(__m128i), PS2_RAM_SIZE, "read128 rdram", address);
        return _mm_loadu_si128(reinterpret_cast<__m128i *>(&m_rdram[physAddr]));
    }
    uint32_t vuOffset = 0;
    uint32_t vuLimit = 0;
    if (const uint8_t *vuMem = mapVuMemory(physAddr, sizeof(__m128i), vuOffset, vuLimit))
    {
        inRange(vuOffset, sizeof(__m128i), vuLimit, "read128 vu", address);
        return _mm_loadu_si128(reinterpret_cast<const __m128i *>(vuMem + vuOffset));
    }

    // 128-bit reads are primarily for quad-word loads in the EE, which are only valid for RAM areas
    // Return zeroes for unsupported areas
    return _mm_setzero_si128();
}

void PS2Memory::write8(uint32_t address, uint8_t value)
{
    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        m_scratchpad[physAddr] = value;
    }
    else if (physAddr < PS2_RAM_SIZE)
    {
        m_rdram[physAddr] = value;
    }
    else
    {
        uint32_t vuOffset = 0;
        uint32_t vuLimit = 0;
        if (uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint8_t), vuOffset, vuLimit))
        {
            (void)vuLimit;
            vuMem[vuOffset] = value;
            if (vuMem == m_vu0Code)
                markVU0CodeModified();
            else if (vuMem == m_vu1Code)
                markVU1CodeModified();
            return;
        }
    }
    if (isIoRegister(physAddr))
    {
        // IO registers - handle byte writes by modifying the appropriate byte in the word
        uint32_t regAddr = physAddr & ~0x3;
        uint32_t shift = (physAddr & 3) * 8;
        uint32_t mask = ~(0xFF << shift);
        uint32_t newValue = (m_ioRegisters[regAddr] & mask) | ((uint32_t)value << shift);
        writeIORegister(regAddr, newValue);
    }
}

void PS2Memory::write16(uint32_t address, uint16_t value)
{
    if (address & 1)
    {
        throw std::runtime_error("Unaligned 16-bit write at address: 0x" + std::to_string(address));
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        storeScalar<uint16_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, value, "write16 scratchpad", address);
    }
    else if (physAddr < PS2_RAM_SIZE)
    {
        storeScalar<uint16_t>(m_rdram, physAddr, PS2_RAM_SIZE, value, "write16 rdram", address);
    }
    else
    {
        uint32_t vuOffset = 0;
        uint32_t vuLimit = 0;
        if (uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint16_t), vuOffset, vuLimit))
        {
            storeScalar<uint16_t>(vuMem, vuOffset, vuLimit, value, "write16 vu", address);
            if (vuMem == m_vu0Code)
                markVU0CodeModified();
            else if (vuMem == m_vu1Code)
                markVU1CodeModified();
            return;
        }
    }
    if (isIoRegister(physAddr))
    {
        uint32_t regAddr = physAddr & ~0x3;
        uint32_t shift = (physAddr & 2) * 8;
        uint32_t mask = ~(0xFFFF << shift);
        uint32_t newValue = (m_ioRegisters[regAddr] & mask) | ((uint32_t)value << shift);
        writeIORegister(regAddr, newValue);
    }
}

void PS2Memory::write32(uint32_t address, uint32_t value)
{
    if (address & 3)
    {
        throw std::runtime_error("Unaligned 32-bit write at address: 0x" + std::to_string(address));
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        uint32_t off = address & 7;
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            // CSR: bits 0..1 of the low dword are write-one-to-clear status bits.
            // Done as a single atomic RMW -- see writeCsrHalf's comment.
            writeCsrHalf(gs_regs.csr, off, value);
        }
        else if (uint64_t *reg = gsRegPtr(gs_regs, address))
        {
            uint64_t mask = 0xFFFFFFFFULL << (off * 8);
            uint64_t newVal = (*reg & ~mask) | ((uint64_t)value << (off * 8));
            *reg = newVal;
        }
        return;
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        storeScalar<uint32_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, value, "write32 scratchpad", address);
    }
    else if (physAddr < PS2_RAM_SIZE)
    {
        // Check if this might be code modification
        markModified(address, 4);

        storeScalar<uint32_t>(m_rdram, physAddr, PS2_RAM_SIZE, value, "write32 rdram", address);
    }
    else
    {
        uint32_t vuOffset = 0;
        uint32_t vuLimit = 0;
        if (uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint32_t), vuOffset, vuLimit))
        {
            storeScalar<uint32_t>(vuMem, vuOffset, vuLimit, value, "write32 vu", address);
            if (vuMem == m_vu0Code)
                markVU0CodeModified();
            else if (vuMem == m_vu1Code)
                markVU1CodeModified();
            return;
        }
    }
    if (isIoRegister(physAddr))
    {
        writeIORegister(physAddr, value);
    }
}

void PS2Memory::write64(uint32_t address, uint64_t value)
{
    if (address & 7)
    {
        throw std::runtime_error("Unaligned 64-bit write at address: 0x" + std::to_string(address));
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            // CSR: bits 0..1 are write-one-to-clear status bits. Done as a single
            // atomic RMW -- see writeCsrFull's comment.
            writeCsrFull(gs_regs.csr, value);
        }
        else if (uint64_t *reg = gsRegPtr(gs_regs, address))
        {
            *reg = value;
        }
        return;
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (scratch)
    {
        storeScalar<uint64_t>(m_scratchpad, physAddr, PS2_SCRATCHPAD_SIZE, value, "write64 scratchpad", address);
    }
    else if (physAddr < PS2_RAM_SIZE)
    {
        markModified(address, 8);
        storeScalar<uint64_t>(m_rdram, physAddr, PS2_RAM_SIZE, value, "write64 rdram", address);
    }
    else
    {
        uint32_t vuOffset = 0;
        uint32_t vuLimit = 0;
        if (uint8_t *vuMem = mapVuMemory(physAddr, sizeof(uint64_t), vuOffset, vuLimit))
        {
            storeScalar<uint64_t>(vuMem, vuOffset, vuLimit, value, "write64 vu", address);
            if (vuMem == m_vu0Code)
                markVU0CodeModified();
            else if (vuMem == m_vu1Code)
                markVU1CodeModified();
            return;
        }
    }
    if (isIoRegister(physAddr))
    {
        write32(address, (uint32_t)value);
        write32(address + 4, (uint32_t)(value >> 32));
    }
}

void PS2Memory::write128(uint32_t address, __m128i value)
{
    if (address & 15)
    {
        throw std::runtime_error("Unaligned 128-bit write at address: 0x" + std::to_string(address));
    }

    const bool scratch = isScratchpad(address);
    uint32_t physAddr = translateAddress(address);

    if (!scratch && physAddr == 0x10004000u) // VIF0_FIFO
    {
        alignas(16) uint8_t fifoData[16];
        _mm_storeu_si128(reinterpret_cast<__m128i *>(fifoData), value);
        processVIF0Data(fifoData, sizeof(fifoData));
        return;
    }
    if (!scratch && physAddr == 0x10005000u) // VIF1_FIFO
    {
        syncVu1();
        alignas(16) uint8_t fifoData[16];
        _mm_storeu_si128(reinterpret_cast<__m128i *>(fifoData), value);
        processVIF1Data(fifoData, sizeof(fifoData));
        return;
    }

    if (scratch)
    {
        inRange(physAddr, sizeof(__m128i), PS2_SCRATCHPAD_SIZE, "write128 scratchpad", address);
        _mm_storeu_si128(reinterpret_cast<__m128i *>(&m_scratchpad[physAddr]), value);
    }
    else if (physAddr < PS2_RAM_SIZE)
    {
        markModified(address, 16);
        inRange(physAddr, sizeof(__m128i), PS2_RAM_SIZE, "write128 rdram", address);
        _mm_storeu_si128(reinterpret_cast<__m128i *>(&m_rdram[physAddr]), value);
    }
    else
    {
        uint32_t vuOffset = 0;
        uint32_t vuLimit = 0;
        if (uint8_t *vuMem = mapVuMemory(physAddr, sizeof(__m128i), vuOffset, vuLimit))
        {
            inRange(vuOffset, sizeof(__m128i), vuLimit, "write128 vu", address);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(vuMem + vuOffset), value);
            if (vuMem == m_vu0Code)
                markVU0CodeModified();
            else if (vuMem == m_vu1Code)
                markVU1CodeModified();
            return;
        }
    }
    if (isIoRegister(physAddr))
    {
        // Non-RAM 128-bit stores are modeled as two 64-bit stores.
        uint64_t lo = _mm_extract_epi64(value, 0);
        uint64_t hi = _mm_extract_epi64(value, 1);

        write64(address, lo);
        write64(address + 8, hi);
    }
}

uint32_t (*g_ps2MmioCaller)() = nullptr; // the running guest code's return address (census)
namespace
{
    thread_local bool t_hostIo = false; // register access made by the host itself (not counted)
}

namespace
{
    // PS2X_MMIO_CENSUS=1: which hardware registers (0x10000000..0x1000ffff)
    // the game reads and writes, counted and printed at exit (debugging).
    struct MmioCensus
    {
        bool on = std::getenv("PS2X_MMIO_CENSUS") != nullptr;
        std::atomic<uint64_t> reads[0x4000]{}, writes[0x4000]{};
        std::mutex mutex;
        std::map<std::pair<uint32_t, uint32_t>, uint64_t> callers; // (register, guest ra) -> count
        void count(uint32_t address, bool write)
        {
            if (!on || t_hostIo || (address & 0xFFFF0000u) != 0x10000000u)
                return;
            (write ? writes : reads)[(address & 0xFFFFu) >> 2].fetch_add(1, std::memory_order_relaxed);
            if (g_ps2MmioCaller)
            {
                std::lock_guard<std::mutex> lock(mutex);
                ++callers[{address | (write ? 1u : 0u), g_ps2MmioCaller()}];
            }
        }
        void dump()
        {
            if (!on)
                return;
            for (uint32_t i = 0; i < 0x4000u; ++i)
                if (reads[i] || writes[i])
                    std::fprintf(stderr, "[mmio] %08x reads %llu writes %llu\n", 0x10000000u + i * 4u,
                                 static_cast<unsigned long long>(reads[i].load()), static_cast<unsigned long long>(writes[i].load()));
            for (const auto &[key, n] : callers)
                std::fprintf(stderr, "[mmio-caller] %08x %s ra %08x count %llu\n", key.first & ~1u, (key.first & 1u) ? "write" : "read", key.second,
                             static_cast<unsigned long long>(n));
        }
    };
    MmioCensus g_mmioCensus;
}

void ps2DumpMmioCensus() { g_mmioCensus.dump(); }

namespace
{
    // PS2X_VIF1_FULL=1: VIF1 DMA with the native renderer drawing goes the old
    // way (chain copied without UNPACK data, run on the VU1 worker) instead
    // of being walked in place (Vif1Walker).
    bool vif1Full()
    {
        static const bool full = std::getenv("PS2X_VIF1_FULL") && std::atoi(std::getenv("PS2X_VIF1_FULL")) != 0;
        return full;
    }
    bool vu1RunForced()
    {
        static const bool run = std::getenv("PS2X_VU1_RUN") && std::atoi(std::getenv("PS2X_VU1_RUN")) != 0;
        return run;
    }
}

namespace
{
    // Round 31's packet cache: what one CALL target's packet did, by the
    // target's address (PS2Memory::walkVif1Chain).
    struct Vif1Packet
    {
        Vif1Walker::Record rec;
        std::vector<uint8_t> bytes; // the ranges' contents, in order
        uint32_t cycle = 0;         // CYCLE at the start (UNPACK sizes)
        uint32_t lastTagUpper = 0;
        int tags = 0;
        uint64_t lastChain = 0; // the walk that last stored or replayed it
    };
    // Direct-mapped by the target's address (no division: the Vita's
    // unordered_map paid a software modulo per lookup); a packet whose slot
    // another one takes is walked again next time.
    constexpr uint32_t kVif1PacketSlots = 4096u;
    struct Vif1PacketSlot
    {
        uint32_t key = ~0u;
        Vif1Packet p;
    };
    std::vector<Vif1PacketSlot> g_vif1Packets;
    constexpr size_t kVif1PacketBudget = 4u << 20;
    size_t g_vif1PacketBytes = 0, g_vif1PacketCount = 0;
    uint64_t g_vif1Walks = 0; // walkVif1Chain calls with the cache
    PS2_FORCE_INLINE uint32_t vif1PacketSlot(uint32_t key) { return ((key >> 4) * 2654435761u) >> 20; }
    size_t vif1PacketSize(const Vif1Packet &p)
    {
        return p.rec.ranges.capacity() * sizeof(p.rec.ranges[0]) + p.rec.ops.capacity() * 4u + p.bytes.capacity();
    }
    Vif1Packet *vif1PacketFind(uint32_t key)
    {
        if (g_vif1Packets.empty())
            return nullptr;
        Vif1PacketSlot &s = g_vif1Packets[vif1PacketSlot(key)];
        return s.key == key ? &s.p : nullptr;
    }
    void vif1PacketErase(uint32_t key)
    {
        if (g_vif1Packets.empty())
            return;
        Vif1PacketSlot &s = g_vif1Packets[vif1PacketSlot(key)];
        if (s.key == key)
        {
            s.key = ~0u;
            --g_vif1PacketCount;
        }
    }
    // The slot for key (taken over from whatever was there), its vectors'
    // capacity kept for the new packet.
    Vif1Packet &vif1PacketTake(uint32_t key)
    {
        if (g_vif1Packets.empty())
            g_vif1Packets.resize(kVif1PacketSlots);
        Vif1PacketSlot &s = g_vif1Packets[vif1PacketSlot(key)];
        if (s.key == ~0u)
            ++g_vif1PacketCount;
        s.key = key;
        return s.p;
    }
    struct Vif1PacketStats
    {
        uint64_t calls = 0, hits = 0, stored = 0, bad = 0, misses = 0, tags = 0, hitTags = 0, clears = 0;
    } g_vif1PacketStats;

    // The ranges are whole words at word addresses (tags, VIFcodes, DIRECT
    // qwords, STMASK / STROW / STCOL words): compared 16 bytes at a time
    // (the Vita's newlib memcmp goes byte by byte: 3% of the game thread).
    PS2_FORCE_INLINE bool sameBytes(const uint8_t *a, const uint8_t *b, uint32_t n)
    {
        while (n >= 16u)
        {
            uint64_t x0, x1, y0, y1;
            std::memcpy(&x0, a, 8);
            std::memcpy(&x1, a + 8, 8);
            std::memcpy(&y0, b, 8);
            std::memcpy(&y1, b + 8, 8);
            if (((x0 ^ y0) | (x1 ^ y1)) != 0u)
                return false;
            a += 16;
            b += 16;
            n -= 16u;
        }
        while (n >= 4u)
        {
            uint32_t x, y;
            std::memcpy(&x, a, 4);
            std::memcpy(&y, b, 4);
            if (x != y)
                return false;
            a += 4;
            b += 4;
            n -= 4u;
        }
        return n == 0u || std::memcmp(a, b, n) == 0;
    }

    bool vif1PacketMatches(const uint8_t *rdram, const Vif1Packet &p)
    {
        const uint8_t *b = p.bytes.data();
        for (const auto &[a, n] : p.rec.ranges)
        {
            if (!sameBytes(rdram + a, b, n))
                return false;
            b += n;
        }
        return true;
    }

    // PS2X_DMA_STATS=1: the walk's counts every 600 chains.
    bool dmaStats()
    {
        static const bool on = std::getenv("PS2X_DMA_STATS") && std::atoi(std::getenv("PS2X_DMA_STATS")) != 0;
        return on;
    }
}

bool PS2Memory::writeIORegister(uint32_t address, uint32_t value)
{
    g_mmioCensus.count(address, true);
    if ((address >= 0x10003000u && address < 0x10003100u) || (address >= 0x10003C00u && address < 0x10003E00u))
        syncVu1(); // GIF / VIF1 registers
    size_t timerIndex = 0u;
    uint32_t timerOffset = 0u;
    if (decodeEeTimerRegister(address, timerIndex, timerOffset))
    {
        m_timerIrqLatched |= syncAndFlushEeTimers();
        m_timerBudget = 0u; // the timer changes: re-evaluate at the next cycles
        // PS2X_TRACE_TIMERS=1: the first writes of each timer register (who uses the timers).
        static const bool traceTimers = std::getenv("PS2X_TRACE_TIMERS") != nullptr;
        if (traceTimers)
        {
            static std::map<uint32_t, int> logged;
            if (logged[address]++ < 4)
                std::cerr << "[timer] T" << timerIndex << " +0x" << std::hex << timerOffset << " = 0x" << value << std::dec << std::endl;
        }
        EeTimer &timer = m_eeTimers[timerIndex];
        switch (timerOffset)
        {
        case kEeTimerCountOffset:
            timer.count = value & 0xFFFFu;
            timer.clockRemainder = 0u;
            break;
        case kEeTimerModeOffset:
        {
            const uint32_t previousMode = timer.mode;
            const uint32_t status = (previousMode & kEeTimerModeStatusMask) & ~(value & kEeTimerModeStatusMask);
            timer.mode = (value & kEeTimerModeConfigMask) | status;
            if (((previousMode ^ timer.mode) & (kEeTimerModeClksMask | kEeTimerModeCue)) != 0u)
            {
                timer.clockRemainder = 0u;
                timer.overflowBacklog = timer.compareBacklog = 0u;
            }
            // The guest acknowledged an event: raise the next one it missed.
            const uint32_t cleared = previousMode & ~timer.mode;
            if ((cleared & kEeTimerModeOvff) && timer.overflowBacklog != 0u && (timer.mode & kEeTimerModeOvfe))
            {
                --timer.overflowBacklog;
                timer.mode |= kEeTimerModeOvff;
                m_timerIrqLatched |= 1u << timerIndex;
            }
            if ((cleared & kEeTimerModeEquf) && timer.compareBacklog != 0u && (timer.mode & kEeTimerModeCmpe))
            {
                --timer.compareBacklog;
                timer.mode |= kEeTimerModeEquf;
                m_timerIrqLatched |= 1u << timerIndex;
            }
            break;
        }
        case kEeTimerCompareOffset:
            timer.compare = value & 0xFFFFu;
            break;
        case kEeTimerHoldOffset:
            timer.hold = value & 0xFFFFu;
            break;
        default:
            return false;
        }
        return true;
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        // NB: unreachable from write8/16/32/64 today since those all funnel IO
        // register writes through addresses in PS2_IO_BASE's range, which is
        // disjoint from PS2_GS_PRIV_REG_BASE; kept correct for direct callers.
        m_ioRegisters[address] = value;
        const uint32_t off = address & 7u;
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            writeCsrHalf(gs_regs.csr, off, value);
        }
        else if (uint64_t *reg = gsRegPtr(gs_regs, address))
        {
            const uint64_t mask = 0xFFFFFFFFull << (off * 8u);
            *reg = (*reg & ~mask) | (static_cast<uint64_t>(value) << (off * 8u));
        }
        m_gsWriteCount.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    if (address >= 0x10002000 && address <= 0x10002030)
    {
        if (address == 0x10002010)
        {
            m_ioRegisters[address] = value & ~(1u << 31);
            if (value & (1u << 30))
            {
                m_ioRegisters[0x10002000] = 0;
                m_ioRegisters[0x10002020] = 0;
                m_ioRegisters[0x10002030] = 0;
            }
        }
        else
        {
            m_ioRegisters[address] = value;
        }
        return true;
    }

    if (address == 0x1000E010u)
    {
        const uint32_t current = m_ioRegisters.count(address) ? m_ioRegisters[address] : 0u;
        uint32_t status = current & 0x3FFu;
        uint32_t mask = (current >> 16) & 0x3FFu;

        // D_STAT low bits are W1C status, high bits [16..25] toggle masks on write-one.
        status &= ~(value & 0x3FFu);
        mask ^= ((value >> 16) & 0x3FFu);

        uint32_t next = (current & ~((0x3FFu) | (0x3FFu << 16) | (1u << 31)));
        next |= status | (mask << 16);
        if ((status & mask) != 0u)
            next |= (1u << 31);
        m_ioRegisters[address] = next;
        return true;
    }

    m_ioRegisters[address] = value;

    if (address >= 0x10003C00u && address < 0x10003E00u)
    {
        m_vifWriteCount.fetch_add(1, std::memory_order_relaxed);

        switch (address)
        {
        case 0x10003C10u:     // VIF1_FBRST
            if (value & 0x1u) // RST
            {
                const bool wasPath3Masked = m_path3Masked;
                std::memset(&vif1_regs, 0, sizeof(vif1_regs));
                m_vif1PendingPath2ImageQwc = 0u;
                m_vif1PendingPath2DirectHl = false;
                m_path3Masked = false;
                if (wasPath3Masked)
                    flushMaskedPath3Packets();
            }
            if (value & 0x8u) // STC
            {
                vif1_regs.stat &= ~((1u << 8) | (1u << 9) | (1u << 10) | (1u << 11) | (1u << 12) | (1u << 13));
            }
            break;
        case 0x10003C30u:
            vif1_regs.mark = value & 0xFFFFu;
            vif1_regs.stat &= ~(1u << 6); // clear MRK flag on CPU write
            break;
        case 0x10003C40u:
            vif1_regs.cycle = value & 0xFFFFu;
            break;
        case 0x10003C50u:
            vif1_regs.mode = value & 0x3u;
            break;
        case 0x10003C60u:
            vif1_regs.num = value & 0xFFu;
            break;
        case 0x10003C70u:
            vif1_regs.mask = value;
            break;
        case 0x10003C80u:
            vif1_regs.code = value;
            break;
        case 0x10003C90u:
            vif1_regs.itops = value & 0x3FFu;
            break;
        case 0x10003CA0u:
            vif1_regs.base = value & 0x3FFu;
            break;
        case 0x10003CB0u:
            vif1_regs.ofst = value & 0x3FFu;
            break;
        case 0x10003CC0u:
            vif1_regs.tops = value & 0x3FFu;
            break;
        case 0x10003CD0u:
            vif1_regs.itop = value & 0x3FFu;
            break;
        case 0x10003CE0u:
            vif1_regs.top = value & 0x3FFu;
            break;
        default:
            break;
        }

        return true;
    }

    if (address >= 0x10003800u && address < 0x10003A00u)
    {
        m_vifWriteCount.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    if (address >= 0x10008000 && address < 0x1000F000)
    {
        if ((address & 0xFF) == 0x00 && (value & 0x100))
        {
            const auto dctrlIt = m_ioRegisters.find(0x1000E000u);
            const bool dmacEnabled = (dctrlIt == m_ioRegisters.end()) || ((dctrlIt->second & 0x1u) != 0u);
            if (!dmacEnabled)
            {
                return true;
            }

            const uint32_t channelBase = address & 0xFFFFFF00;
            const uint32_t madr = m_ioRegisters[channelBase + 0x10];
            const uint32_t qwc = m_ioRegisters[channelBase + 0x20];
            m_dmaStartCount.fetch_add(1, std::memory_order_relaxed);

            if (tryProcessScratchpadDma(channelBase, value))
            {
                return true;
            }

            if ((channelBase == 0x1000A000u || channelBase == 0x10009000u || channelBase == 0x10008000u) && (m_gsVRAM || channelBase == 0x10008000u))
            {
                auto enqueueTransfer = [&](uint32_t srcAddr, uint32_t qwCount)
                {
                    if (qwCount == 0)
                        return;
                    const bool scratch = isScratchpad(srcAddr);
                    PendingTransfer pt;
                    pt.fromScratchpad = scratch;
                    pt.srcAddr = srcAddr;
                    pt.qwc = qwCount;
                    if (channelBase == 0x1000A000u)
                        m_pendingGifTransfers.push_back(pt);
                    else if (channelBase == 0x10009000u)
                        m_pendingVif1Transfers.push_back(pt);
                    else if (channelBase == 0x10008000u)
                        m_pendingVif0Transfers.push_back(pt);
                };

                uint32_t chcr = value;
                uint32_t mode = (chcr >> 2) & 0x3;

                if (mode == 0 && qwc > 0 && channelBase == 0x1000A000u && GS::skipRaster() && m_gsEventWrite)
                {
                    GifEventScanner gifEvents(m_gsEventWrite);
                    const bool scratch = isScratchpad(madr);
                    const uint8_t *base = scratch ? m_scratchpad : m_rdram;
                    const uint32_t limit = scratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
                    uint32_t src = translateAddress(madr);
                    uint32_t bytes = qwc * 16u;
                    while (bytes != 0u)
                    {
                        if (src >= limit)
                            src = 0;
                        const uint32_t chunk = std::min(bytes, limit - src);
                        gifEvents.feed(base + src, chunk);
                        bytes -= chunk;
                        src += chunk;
                    }
                    m_gifScanned = true;
                }
                else if (mode == 0 && qwc > 0 && channelBase == 0x10009000u && GS::skipRaster() && m_gsEventWrite && !vif1Full() && !vu1RunForced())
                {
                    // A normal-mode VIF1 transfer, walked in place likewise.
                    syncVu1();
                    GifEventScanner gifEvents(m_gsEventWrite);
                    const Vif1Walker::Path3Mask path3Mask = [this](bool masked)
                    {
                        const bool was = m_path3Masked;
                        m_path3Masked = masked;
                        if (was && !masked)
                            flushMaskedPath3Packets();
                    };
                    Vif1Walker walker(vif1_regs, gifEvents, path3Mask);
                    const bool scratch = isScratchpad(madr);
                    const uint8_t *base = scratch ? m_scratchpad : m_rdram;
                    const uint32_t limit = scratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
                    uint32_t src = translateAddress(madr);
                    uint32_t bytes = qwc * 16u;
                    while (bytes != 0u)
                    {
                        if (src >= limit)
                            src = 0;
                        const uint32_t chunk = std::min(bytes, limit - src);
                        walker.feed(base + src, chunk);
                        bytes -= chunk;
                        src += chunk;
                    }
                    walker.finish();
                    PendingTransfer pt;
                    pt.fromScratchpad = false;
                    pt.srcAddr = 0;
                    pt.qwc = 0;
                    m_pendingVif1Transfers.push_back(std::move(pt));
                }
                else if (mode == 0 && qwc > 0)
                {
                    enqueueTransfer(madr, qwc);
                }
                else if (mode == 1)
                {
                    const uint64_t eventsAtStart = GifEventScanner::s_events[0] + GifEventScanner::s_events[1] + GifEventScanner::s_events[2];
                    uint32_t tagAddr = m_ioRegisters[channelBase + 0x30];
                    uint32_t asr0 = m_ioRegisters[channelBase + 0x40];
                    uint32_t asr1 = m_ioRegisters[channelBase + 0x50];
                    uint32_t asp = (chcr >> 4) & 0x3u;
                    const bool tieEnabled = (chcr & (1u << 7)) != 0u;
                    // Only a guard against malformed loops: real chains exceed 4096 tags
                    // (SotC's canyon frames build ~4,600), and a cut chain desyncs the GIF.
                    const int kMaxChainTags = 1 << 20;
                    std::vector<uint8_t> chainBuf;
                    // No VU1 program runs (native renderer, GS::skipRaster): the
                    // VIF1 chain is gathered without the UNPACK/MPG data.
                    static const bool runVu1 = std::getenv("PS2X_VU1_RUN") && std::atoi(std::getenv("PS2X_VU1_RUN")) != 0;
                    const bool eventsOnly = GS::skipRaster() && m_gsEventWrite;
                    GifEventScanner gifEvents(m_gsEventWrite);
                    std::optional<Vif1Compactor> compactor;
                    // By default the chain is not even gathered: its VIFcodes are
                    // walked where they lie (Vif1Walker), on this thread, with no
                    // VU1 worker job. PS2X_VIF1_FULL=1: the compacted copy
                    // through the VU1 worker as before (A/B).
                    std::optional<Vif1Walker> walker;
                    Vif1Walker::Path3Mask path3Mask;
                    if (channelBase == 0x10009000u && !runVu1 && eventsOnly)
                    {
                        if (vif1Full())
                            compactor.emplace(vif1_regs.cycle, chainBuf, gifEvents);
                        else
                        {
                            syncVu1(); // the worker is done with vif1_regs
                            path3Mask = [this](bool masked)
                            {
                                const bool was = m_path3Masked;
                                m_path3Masked = masked;
                                if (was && !masked)
                                    flushMaskedPath3Packets();
                            };
                            walker.emplace(vif1_regs, gifEvents, path3Mask);
                        }
                    }
                    // The GIF channel's chain, likewise: read for its events only.
                    const bool gifEventsOnly = channelBase == 0x1000A000u && eventsOnly;
                    if (gifEventsOnly)
                        m_gifScanned = true;

                    auto appendData = [&](uint32_t srcAddr, uint32_t qwCount)
                    {
                        const uint64_t bytes64 = static_cast<uint64_t>(qwCount) * 16ull;
                        uint32_t bytes = (bytes64 > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<uint32_t>(bytes64);
                        const bool scratch = isScratchpad(srcAddr);
                        uint32_t src = 0;
                        src = translateAddress(srcAddr);
                        const uint8_t *base2;
                        uint32_t maxSz2;
                        if (scratch)
                        {
                            base2 = m_scratchpad;
                            maxSz2 = PS2_SCRATCHPAD_SIZE;
                        }
                        else
                        {
                            base2 = m_rdram;
                            maxSz2 = PS2_RAM_SIZE;
                        }

                        while (bytes > 0)
                        {
                            if (src >= maxSz2)
                                src = 0;
                            uint32_t chunk = bytes;
                            if (src + chunk > maxSz2)
                                chunk = maxSz2 - src;
                            if (chunk == 0)
                                break;
                            if (walker)
                                walker->feed(base2 + src, chunk);
                            else if (compactor)
                                compactor->feed(base2 + src, chunk);
                            else if (gifEventsOnly)
                                gifEvents.feed(base2 + src, chunk);
                            else
                                chainBuf.insert(chainBuf.end(), base2 + src, base2 + src + chunk);
                            bytes -= chunk;
                            src += chunk;
                        }
                    };

                    auto appendVifTagData = [&](uint32_t localTagAddr)
                    {
                        uint32_t tagPhys = 0u;
                        const bool tagScratch = isScratchpad(localTagAddr);
                        tagPhys = translateAddress(localTagAddr);

                        const uint8_t *localBase = tagScratch ? m_scratchpad : m_rdram;
                        const uint32_t localMax = tagScratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
                        if (tagPhys + 16u > localMax)
                            return;

                        // CHCR.TTE sends the DMAtag's upper 64 bits to the channel before
                        // the tag payload. VIF chains use those bytes for two VIFcodes.
                        if (walker)
                            walker->feed(localBase + tagPhys + 8u, 8u);
                        else if (compactor)
                            compactor->feed(localBase + tagPhys + 8u, 8u);
                        else
                            chainBuf.insert(chainBuf.end(), localBase + tagPhys + 8u, localBase + tagPhys + 16u);
                    };

                    const bool isVifChannel =
                        channelBase == 0x10009000u || channelBase == 0x10008000u;
                    const bool transferTagData = isVifChannel && (chcr & 0x40u) != 0u;

                    int tagsProcessed = 0;
                    uint32_t lastTagUpper = (chcr >> 16) & 0xFFFFu;

                    // Round 31: the walker's chains through walkVif1Chain
                    // (PS2X_DMA_WALK2=0: this loop). SOTC_DMACACHE_CHECK=1:
                    // each chain walked first without the packet cache on
                    // copies (VIF1 registers, GS events and MSKPATH3 calls
                    // collected), then for real with it, and the two compared.
                    static const bool dumping = std::getenv("PS2X_DMA_DUMP") != nullptr;
                    static const bool cacheCheck = std::getenv("SOTC_DMACACHE_CHECK") && std::atoi(std::getenv("SOTC_DMACACHE_CHECK")) != 0;
                    const bool walk2 = walker && transferTagData && channelBase == 0x10009000u && ps2_fast_paths::g_dmaWalk2 && !dumping;
                    if (walk2)
                    {
                        Vif1ChainState st{tagAddr, asr0, asr1, asp, lastTagUpper, 0};
                        if (!cacheCheck)
                            walkVif1Chain(*walker, gifEvents, st, tieEnabled, ps2_fast_paths::g_dmaCache);
                        else
                        {
                            using Events = std::vector<std::pair<uint8_t, uint64_t>>;
                            Events refEvents, realEvents;
                            std::vector<bool> refMasks, realMasks;
                            VIFRegisters refRegs = vif1_regs;
                            uint64_t savedEvents[3];
                            std::memcpy(savedEvents, GifEventScanner::s_events, sizeof(savedEvents));
                            const GifEventScanner::WriteFn refWrite = [&](uint8_t reg, uint64_t v) { refEvents.emplace_back(reg, v); };
                            const Vif1Walker::Path3Mask refMask = [&](bool m) { refMasks.push_back(m); };
                            GifEventScanner refScan(refWrite);
                            Vif1Walker refWalker(refRegs, refScan, refMask);
                            Vif1ChainState ref = st;
                            walkVif1Chain(refWalker, refScan, ref, tieEnabled, false);
                            std::memcpy(GifEventScanner::s_events, savedEvents, sizeof(savedEvents));
                            const GifEventScanner::WriteFn realWrite = [&](uint8_t reg, uint64_t v)
                            {
                                realEvents.emplace_back(reg, v);
                                if (m_gsEventWrite)
                                    m_gsEventWrite(reg, v);
                            };
                            const Vif1Walker::Path3Mask realMask = [&](bool m)
                            {
                                realMasks.push_back(m);
                                path3Mask(m);
                            };
                            GifEventScanner realScan(realWrite);
                            Vif1Walker realWalker(vif1_regs, realScan, realMask);
                            walkVif1Chain(realWalker, realScan, st, tieEnabled, ps2_fast_paths::g_dmaCache);
                            const bool same = std::memcmp(&refRegs, &vif1_regs, sizeof(VIFRegisters)) == 0 && refEvents == realEvents && refMasks == realMasks &&
                                              ref.tagAddr == st.tagAddr && ref.asr0 == st.asr0 && ref.asr1 == st.asr1 && ref.asp == st.asp &&
                                              ref.lastTagUpper == st.lastTagUpper && ref.tags == st.tags && refWalker.countersHash() == realWalker.countersHash() &&
                                              refScan.idle() == realScan.idle();
                            realWalker.finish();
                            static uint64_t checked = 0, mismatches = 0, events = 0;
                            ++checked;
                            events += realEvents.size();
                            if (!same && mismatches++ < 20)
                                std::fprintf(stderr, "[dmacache] chain %llu MISMATCH: tags %d / %d, end %08x / %08x, code %08x / %08x, stat %08x / %08x, top %x / %x, events %zu / %zu\n",
                                             (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), ref.tags, st.tags, ref.tagAddr, st.tagAddr,
                                             refRegs.code, vif1_regs.code, refRegs.stat, vif1_regs.stat, refRegs.top, vif1_regs.top, refEvents.size(), realEvents.size());
                            if (checked % 600u == 0u)
                                std::fprintf(stderr, "[dmacache] check: %llu chains compared, %llu mismatches, %llu GS events\n", (unsigned long long)checked,
                                             (unsigned long long)mismatches, (unsigned long long)events);
                        }
                        tagAddr = st.tagAddr;
                        asr0 = st.asr0;
                        asr1 = st.asr1;
                        asp = st.asp;
                        lastTagUpper = st.lastTagUpper;
                        tagsProcessed = st.tags;
                        if (dmaStats() && ps2_stats::vif1Chains.load(std::memory_order_relaxed) % 600u == 0u)
                        {
                            const auto &k = g_vif1PacketStats;
                            std::fprintf(stderr, "[dmastats] chain %llu: %llu tags walked, %llu CALLs looked up, %llu replayed (%llu tags), %llu walked, %llu not kept, %llu stored, %zu packets (%zu KB, %llu clears)\n",
                                         (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), (unsigned long long)k.tags, (unsigned long long)k.calls,
                                         (unsigned long long)k.hits, (unsigned long long)k.hitTags, (unsigned long long)k.misses, (unsigned long long)k.bad,
                                         (unsigned long long)k.stored, g_vif1PacketCount, g_vif1PacketBytes >> 10, (unsigned long long)k.clears);
                        }
                    }
                    else
                    while (tagsProcessed < kMaxChainTags)
                    {
                        const uint32_t currentTagAddr = tagAddr;
                        const bool tagInSPR = isScratchpad(tagAddr);
                        uint32_t physTag = 0;
                        try
                        {
                            physTag = translateAddress(tagAddr);
                        }
                        catch (...)
                        {
                            break;
                        }
                        const uint8_t *tagBase;
                        uint32_t tagMax;
                        if (tagInSPR)
                        {
                            tagBase = m_scratchpad;
                            tagMax = PS2_SCRATCHPAD_SIZE;
                        }
                        else
                        {
                            tagBase = m_rdram;
                            tagMax = PS2_RAM_SIZE;
                        }
                        if (physTag + 16 > tagMax)
                            break;

                        const uint8_t *tp = tagBase + physTag;
                        uint64_t tag = loadScalar<uint64_t>(tp, 0, 16, "dma chain tag", tagAddr);
                        uint16_t tagQwc = static_cast<uint16_t>(tag & 0xFFFF);
                        uint32_t id = static_cast<uint32_t>((tag >> 28) & 0x7);
                        const bool irq = ((tag >> 31) & 0x1ull) != 0ull;
                        uint32_t addr = static_cast<uint32_t>((tag >> 32) & 0x7FFFFFFF);
                        lastTagUpper = static_cast<uint32_t>((tag >> 16) & 0xFFFFu);
                        ++tagsProcessed;

                        uint32_t dataAddr = 0;
                        bool hasPayload = (tagQwc > 0);
                        bool endChain = false;

                        switch (id)
                        {
                        case 0:
                            dataAddr = addr;
                            tagAddr = tagAddr + 16;
                            endChain = true;
                            break;
                        case 1:
                            dataAddr = tagAddr + 16;
                            tagAddr = dataAddr + static_cast<uint32_t>(tagQwc) * 16u;
                            break;
                        case 2:
                            dataAddr = tagAddr + 16;
                            tagAddr = addr;
                            break;
                        case 3:
                        case 4:
                            dataAddr = addr;
                            tagAddr = tagAddr + 16;
                            break;
                        case 5:
                            dataAddr = tagAddr + 16;
                            {
                                const uint32_t retAddr = dataAddr + static_cast<uint32_t>(tagQwc) * 16u;
                                if (asp == 0u)
                                {
                                    asr0 = retAddr;
                                    asp = 1u;
                                }
                                else if (asp == 1u)
                                {
                                    asr1 = retAddr;
                                    asp = 2u;
                                }
                            }
                            tagAddr = addr;
                            break;
                        case 6:
                            dataAddr = tagAddr + 16;
                            if (asp == 2u)
                            {
                                tagAddr = asr1;
                                asp = 1u;
                            }
                            else if (asp == 1u)
                            {
                                tagAddr = asr0;
                                asp = 0u;
                            }
                            else
                            {
                                endChain = true;
                            }
                            break;
                        case 7:
                            dataAddr = tagAddr + 16;
                            endChain = true;
                            break;
                        default:
                            hasPayload = false;
                            endChain = true;
                            break;
                        }

                        // The walk reads each tag and the VIFcodes of its data
                        // one after another, each a DRAM miss on the Vita (~250
                        // ns, no hardware prefetch): the next tag is read now
                        // and its data's first lines asked for, so they arrive
                        // while this tag's data is walked.
                        if (walker && !endChain && ps2_fast_paths::g_dmaPrefetch && tagAddr < PS2_RAM_SIZE - 16u)
                        {
                            uint64_t next;
                            std::memcpy(&next, m_rdram + tagAddr, 8);
                            const uint32_t nextId = static_cast<uint32_t>((next >> 28) & 0x7u);
                            const uint32_t nextData = (nextId == 3u || nextId == 4u || nextId == 0u) ? static_cast<uint32_t>((next >> 32) & 0x7FFFFFFFu) : tagAddr + 16u;
                            if ((next & 0xFFFFu) != 0u && nextData < PS2_RAM_SIZE - 128u)
                            {
                                __builtin_prefetch(m_rdram + nextData);
                                __builtin_prefetch(m_rdram + nextData + 64u);
                            }
                            __builtin_prefetch(m_rdram + tagAddr + 64u);
                        }

                        // PS2X_DMA_DUMP=<n> (debugging): the n-th VIF1 chain's tags, one line
                        // each (id, qwc, address, the tag's VIFcodes, the payload's first words).
                        static const long dumpChain = std::getenv("PS2X_DMA_DUMP") ? std::atol(std::getenv("PS2X_DMA_DUMP")) : -1;
                        if (dumpChain >= 0 && channelBase == 0x10009000u &&
                            static_cast<long>(ps2_stats::vif1Chains.load(std::memory_order_relaxed)) == dumpChain)
                        {
                            uint32_t w[4] = {};
                            if (hasPayload && dataAddr < PS2_RAM_SIZE - 16u)
                                std::memcpy(w, m_rdram + dataAddr, 16);
                            uint32_t up[2];
                            std::memcpy(up, tp + 8, 8);
                            std::fprintf(stderr, "[dma:tag] %08x id %u qwc %u addr %08x vif %08x %08x data %08x %08x %08x %08x\n", currentTagAddr, id, tagQwc, addr, up[0], up[1],
                                         w[0], w[1], w[2], w[3]);
                        }
                        if (transferTagData)
                        {
                            // The tag just read (tp): its VIFcode words, two NOPs
                            // most of the time (a list's NEXT / CALL links).
                            if (!(walker && ps2_fast_paths::g_dmaWalkFast && walker->feedTagWords(tp + 8)))
                                appendVifTagData(currentTagAddr);
                        }

                        if (hasPayload)
                            appendData(dataAddr, tagQwc);
                        if (irq && tieEnabled)
                            endChain = true;
                        if (endChain)
                            break;
                    }

                    if (channelBase == 0x10009000u)
                    {
                        // PS2X_GIF_EVENT_LOG=1 (debugging): the GS events the scans found, every 600 chains.
                        static const bool eventLog = std::getenv("PS2X_GIF_EVENT_LOG") != nullptr;
                        if (eventLog)
                        {
                            // Round 23: which of them came from VIF1 chains (DIRECT).
                            static uint64_t vif1Events = 0, vif1Chains = 0;
                            const uint64_t now = GifEventScanner::s_events[0] + GifEventScanner::s_events[1] + GifEventScanner::s_events[2];
                            vif1Events += now - eventsAtStart;
                            vif1Chains += now != eventsAtStart;
                            if (ps2_stats::vif1Chains.load(std::memory_order_relaxed) % 600u == 0u)
                                std::fprintf(stderr, "[gif:events] VIF1 chains so far: %llu events in %llu chains\n", (unsigned long long)vif1Events,
                                             (unsigned long long)vif1Chains);
                        }
                        if (eventLog && ps2_stats::vif1Chains.load(std::memory_order_relaxed) % 600u == 0u)
                            std::fprintf(stderr, "[gif:events] chain %llu: signal %llu finish %llu label %llu\n",
                                         (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), (unsigned long long)GifEventScanner::s_events[0],
                                         (unsigned long long)GifEventScanner::s_events[1], (unsigned long long)GifEventScanner::s_events[2]);
                        if (ps2_stats::kickHook)
                            ps2_stats::kickHook();
                        ps2_stats::vif1Chains.fetch_add(1, std::memory_order_relaxed);
                        ps2_stats::frameKicked(ps2_stats::nowUs(), ps2_load::waitUs[ps2_load::kVu1].load(std::memory_order_relaxed));
                    }
                    static const bool traceChains = std::getenv("PS2X_TRACE_DMA") != nullptr;
                    if (traceChains && channelBase == 0x10009000u)
                    {
                        static uint32_t chainLogs = 0;
                        if (chainLogs++ < 2000)
                        {
                            std::cerr << "[dma:vif1] start=0x" << std::hex << m_ioRegisters[channelBase + 0x30]
                                      << " end=0x" << tagAddr << std::dec << " tags=" << tagsProcessed
                                      << " bytes=" << chainBuf.size() << " asp=" << asp
                                      << (tagsProcessed >= kMaxChainTags ? " HIT_TAG_LIMIT" : "") << std::endl;
                        }
                    }
                    if (tagsProcessed >= kMaxChainTags)
                    {
                        static bool reported = false;
                        if (!reported)
                        {
                            reported = true;
                            std::cerr << "[dma] chain at 0x" << std::hex << channelBase << " hit the tag limit" << std::dec << std::endl;
                        }
                    }
                    m_ioRegisters[channelBase + 0x30] = tagAddr;
                    m_ioRegisters[channelBase + 0x40] = asr0;
                    m_ioRegisters[channelBase + 0x50] = asr1;
                    chcr = (chcr & ~(0x3u << 4)) | ((asp & 0x3u) << 4);
                    chcr = (chcr & 0x0000FFFFu) | (lastTagUpper << 16);
                    m_ioRegisters[channelBase + 0x00] = chcr;

                    if (walker)
                    {
                        // Walked already: only the channel's completion is left
                        // (an empty transfer: D_STAT, CHCR.STR, the interrupt).
                        walker->finish();
                        PendingTransfer pt;
                        pt.fromScratchpad = false;
                        pt.srcAddr = 0;
                        pt.qwc = 0;
                        m_pendingVif1Transfers.push_back(std::move(pt));
                    }
                    else if (!chainBuf.empty())
                    {
                        PendingTransfer pt;
                        pt.fromScratchpad = false;
                        pt.srcAddr = 0;
                        pt.qwc = 0;
                        pt.chainData = std::move(chainBuf);
                        if (channelBase == 0x1000A000)
                        {
                            m_pendingGifTransfers.push_back(std::move(pt));
                        }
                        else if (channelBase == 0x10009000u)
                        {
                            m_pendingVif1Transfers.push_back(std::move(pt));
                        }
                        else if (channelBase == 0x10008000u)
                        {
                            m_pendingVif0Transfers.push_back(std::move(pt));
                        }
                    }
                    // else if (channelBase == 0x10009000u)
                    // {

                    // }
                }
                else if (qwc > 0)
                {
                    enqueueTransfer(madr, qwc);
                }

                const bool autoProcessTransfers =
                    (channelBase == 0x1000A000u) ? (m_gifPacketCallback || m_gifArbiter != nullptr) : true;
                if (autoProcessTransfers)
                {
                    processPendingTransfers();
                }
            }
        }
        return true;
    }

    if (address >= 0x10000000 && address < 0x10010000)
    {
        if (address >= 0x10000200 && address < 0x10000300)
        {
            return true;
        }
        if (address >= 0x10000000 && address < 0x10000100)
        {
            return true;
        }
    }

    return false;
}

bool PS2Memory::tryProcessScratchpadDma(uint32_t channelBase, uint32_t chcr)
{
    static constexpr uint32_t kSprFromChannel = 0x1000D000u;
    static constexpr uint32_t kSprToChannel = 0x1000D400u;
    if (channelBase != kSprFromChannel && channelBase != kSprToChannel)
        return false;

    const uint32_t mode = (chcr >> 2u) & 0x3u;
    if (mode != 0u)
        return false;

    const uint32_t qwc = m_ioRegisters[channelBase + 0x20u] & 0xFFFFu;
    const uint32_t byteCount = qwc * 16u;
    const uint32_t originalMadr = m_ioRegisters[channelBase + 0x10u] & 0x7FFFFFF0u;
    const uint32_t originalSadr = m_ioRegisters[channelBase + 0x80u] & 0x3FF0u;

    uint32_t mainOffset = 0u;
    try
    {
        mainOffset = translateAddress(originalMadr);
    }
    catch (const std::exception &)
    {
        return false;
    }

    if (mainOffset > PS2_RAM_SIZE || byteCount > PS2_RAM_SIZE - mainOffset)
        return false;

    const ps2_stats::TimeInto timed{ps2_stats::sprDmaUs};
    ps2_stats::sprDmaBytes.fetch_add(byteCount, std::memory_order_relaxed);
    ps2_stats::sprDmas.fetch_add(1u, std::memory_order_relaxed);
    const bool fromScratchpad = channelBase == kSprFromChannel;
    uint32_t scratchOffset = originalSadr;
    uint32_t bytesLeft = byteCount;
    uint32_t copied = 0u;
    while (bytesLeft != 0u)
    {
        const uint32_t scratchChunk = PS2_SCRATCHPAD_SIZE - scratchOffset;
        const uint32_t chunk = std::min(bytesLeft, scratchChunk);
        if (fromScratchpad)
        {
            std::memcpy(m_rdram + mainOffset + copied, m_scratchpad + scratchOffset, chunk);
            markModified(mainOffset + copied, chunk);
        }
        else
        {
            std::memcpy(m_scratchpad + scratchOffset, m_rdram + mainOffset + copied, chunk);
        }

        copied += chunk;
        bytesLeft -= chunk;
        scratchOffset = (scratchOffset + chunk) & (PS2_SCRATCHPAD_SIZE - 1u);
    }

    m_ioRegisters[channelBase + 0x10u] = (originalMadr + byteCount) & 0x7FFFFFF0u;
    m_ioRegisters[channelBase + 0x20u] = 0u;
    m_ioRegisters[channelBase + 0x80u] = (originalSadr + byteCount) & 0x3FF0u;
    completeDmacChannel(channelBase, fromScratchpad ? 8u : 9u);
    return true;
}

void PS2Memory::completeDmacChannel(uint32_t channelBase, uint32_t cause)
{
    static constexpr uint32_t kDStat = 0x1000E010u;
    m_ioRegisters[channelBase] &= ~0x100u;

    uint32_t dstat = m_ioRegisters.count(kDStat) ? m_ioRegisters[kDStat] : 0u;
    dstat |= 1u << cause;
    const uint32_t status = dstat & 0x3FFu;
    const uint32_t mask = (dstat >> 16u) & 0x3FFu;
    if ((status & mask) != 0u)
        dstat |= 1u << 31u;
    else
        dstat &= ~(1u << 31u);
    m_ioRegisters[kDStat] = dstat;
    queueCompletedDmacCause(cause);
}

void PS2Memory::processPendingTransfers()
{
    if (m_mtvuEnabled && (!m_pendingGifTransfers.empty() || !m_pendingVif1Transfers.empty()))
    {
        // Keep PATH3 and VIF1 in submission order on the worker; the DMA
        // completion below is reported right away, as the data is copied.
        const uint32_t fbrst = m_vu0FbrstProvider ? m_vu0FbrstProvider() : 0u;
        // A gathered chain moves into the job (it is not read again here).
        for (auto &transfer : m_pendingGifTransfers)
        {
            MtvuJob job;
            job.gif = true;
            job.data = transfer.chainData.empty() ? copyTransferData(transfer) : std::move(transfer.chainData);
            if (job.data.size() >= 16u)
            {
                m_seenGifCopy = true;
                m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
                mtvuEnqueue(std::move(job));
            }
        }
        for (auto &transfer : m_pendingVif1Transfers)
        {
            MtvuJob job;
            job.fbrst = fbrst;
            job.data = transfer.chainData.empty() ? copyTransferData(transfer) : std::move(transfer.chainData);
            if (!job.data.empty())
                mtvuEnqueue(std::move(job));
        }
        m_mtvuDeferredGif = !m_pendingGifTransfers.empty();
        m_mtvuDeferredVif1 = !m_pendingVif1Transfers.empty();
        m_pendingGifTransfers.clear();
        m_pendingVif1Transfers.clear();
    }
    else
    {
        m_mtvuDeferredGif = false;
        m_mtvuDeferredVif1 = false;
    }

    const bool hadGif = !m_pendingGifTransfers.empty() || m_mtvuDeferredGif || m_gifScanned;
    m_gifScanned = false;
    uint32_t observedGifQwc = 0u;
    for (const auto &transfer : m_pendingGifTransfers)
    {
        const uint64_t transferQwc = !transfer.chainData.empty()
                                         ? (transfer.chainData.size() / 16u)
                                         : transfer.qwc;
        observedGifQwc = static_cast<uint32_t>(std::min<uint64_t>(16u, static_cast<uint64_t>(observedGifQwc) + transferQwc));
    }
    if (observedGifQwc != 0u)
    {
        constexpr uint32_t kGifStat = 0x10003020u;
        constexpr uint32_t kGifFqcMask = 0x1F000000u;
        uint32_t &gifStat = m_ioRegisters[kGifStat];
        gifStat = (gifStat & ~kGifFqcMask) | (observedGifQwc << 24u);
    }

    for (size_t idx = 0; idx < m_pendingGifTransfers.size(); ++idx)
    {
        auto &p = m_pendingGifTransfers[idx];
        if (!p.chainData.empty())
        {
            m_seenGifCopy = true;
            m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
            submitGifPacket(GifPathId::Path3, p.chainData.data(), static_cast<uint32_t>(p.chainData.size()), false);
        }
        else if (p.qwc > 0)
        {
            const uint64_t bytes64 = static_cast<uint64_t>(p.qwc) * 16ull;
            uint32_t sizeBytes = (bytes64 > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<uint32_t>(bytes64);
            uint32_t srcPhys = 0;
            try
            {
                srcPhys = translateAddress(p.srcAddr);
            }
            catch (const std::exception &)
            {
                continue;
            }
            if (p.fromScratchpad)
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft >= 16)
                {
                    if (srcPhys >= PS2_SCRATCHPAD_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_SCRATCHPAD_SIZE)
                        chunk = PS2_SCRATCHPAD_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    m_seenGifCopy = true;
                    m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
                    submitGifPacket(GifPathId::Path3, m_scratchpad + srcPhys, chunk, false);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
            else
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft >= 16)
                {
                    if (srcPhys >= PS2_RAM_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_RAM_SIZE)
                        chunk = PS2_RAM_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    m_seenGifCopy = true;
                    m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
                    submitGifPacket(GifPathId::Path3, m_rdram + srcPhys, chunk, false);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
        }
    }
    m_pendingGifTransfers.clear();

    const bool hadVif0 = !m_pendingVif0Transfers.empty();
    for (auto &p : m_pendingVif0Transfers)
    {
        if (!p.chainData.empty())
        {
            processVIF0Data(p.chainData.data(), static_cast<uint32_t>(p.chainData.size()));
        }
        else if (p.qwc > 0)
        {
            uint32_t srcPhys = 0;
            const uint64_t bytes64 = static_cast<uint64_t>(p.qwc) * 16ull;
            uint32_t sizeBytes = (bytes64 > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<uint32_t>(bytes64);
            try
            {
                srcPhys = translateAddress(p.srcAddr);
            }
            catch (const std::exception &)
            {
                continue;
            }
            if (p.fromScratchpad)
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft > 0)
                {
                    if (srcPhys >= PS2_SCRATCHPAD_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_SCRATCHPAD_SIZE)
                        chunk = PS2_SCRATCHPAD_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    processVIF0Data(m_scratchpad + srcPhys, chunk);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
            else
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft > 0)
                {
                    if (srcPhys >= PS2_RAM_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_RAM_SIZE)
                        chunk = PS2_RAM_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    processVIF0Data(srcPhys, chunk);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
        }
    }
    m_pendingVif0Transfers.clear();

    const bool hadVif1 = !m_pendingVif1Transfers.empty() || m_mtvuDeferredVif1;
    for (auto &p : m_pendingVif1Transfers)
    {
        if (!p.chainData.empty())
        {
            processVIF1Data(p.chainData.data(), static_cast<uint32_t>(p.chainData.size()));
        }
        else if (p.qwc > 0)
        {
            uint32_t srcPhys = 0;
            const uint64_t bytes64 = static_cast<uint64_t>(p.qwc) * 16ull;
            uint32_t sizeBytes = (bytes64 > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<uint32_t>(bytes64);
            try
            {
                srcPhys = translateAddress(p.srcAddr);
            }
            catch (const std::exception &)
            {
                continue;
            }
            if (p.fromScratchpad)
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft > 0)
                {
                    if (srcPhys >= PS2_SCRATCHPAD_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_SCRATCHPAD_SIZE)
                        chunk = PS2_SCRATCHPAD_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    processVIF1Data(m_scratchpad + srcPhys, chunk);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
            else
            {
                uint32_t bytesLeft = sizeBytes;
                while (bytesLeft > 0)
                {
                    if (srcPhys >= PS2_RAM_SIZE)
                        srcPhys = 0;
                    uint32_t chunk = bytesLeft;
                    if (srcPhys + chunk > PS2_RAM_SIZE)
                        chunk = PS2_RAM_SIZE - srcPhys;
                    if (chunk == 0)
                        break;
                    processVIF1Data(srcPhys, chunk);
                    bytesLeft -= chunk;
                    srcPhys += chunk;
                }
            }
        }
    }
    m_pendingVif1Transfers.clear();

    // With MTVU the arbiter belongs to the worker thread.
    if (m_gifArbiter && !m_mtvuEnabled)
        m_gifArbiter->drain();

    static constexpr uint32_t GIF_CHANNEL = 0x1000A000;
    static constexpr uint32_t VIF0_CHANNEL = 0x10008000;
    static constexpr uint32_t VIF1_CHANNEL = 0x10009000;
    static constexpr uint32_t D_STAT = 0x1000E010u;

    auto raiseDStatChannel = [&](uint32_t channelBit)
    {
        uint32_t dstat = m_ioRegisters.count(D_STAT) ? m_ioRegisters[D_STAT] : 0u;
        dstat |= (1u << channelBit);

        const uint32_t status = dstat & 0x3FFu;
        const uint32_t mask = (dstat >> 16) & 0x3FFu;
        if ((status & mask) != 0u)
            dstat |= (1u << 31);
        else
            dstat &= ~(1u << 31);

        m_ioRegisters[D_STAT] = dstat;
    };

    if (hadGif)
    {
        raiseDStatChannel(2u); // GIF channel
        queueCompletedDmacCause(2u);
        m_ioRegisters[GIF_CHANNEL + 0x00] &= ~0x100u;
        m_ioRegisters[GIF_CHANNEL + 0x20] = 0;
    }
    if (hadVif0)
    {
        raiseDStatChannel(0u); // VIF0 channel
        queueCompletedDmacCause(0u);
        m_ioRegisters[VIF0_CHANNEL + 0x00] &= ~0x100u;
        m_ioRegisters[VIF0_CHANNEL + 0x20] = 0;
    }
    if (hadVif1)
    {
        raiseDStatChannel(1u); // VIF1 channel
        queueCompletedDmacCause(1u);
        m_ioRegisters[VIF1_CHANNEL + 0x00] &= ~0x100u;
        m_ioRegisters[VIF1_CHANNEL + 0x20] = 0;
    }
}

void PS2Memory::queueCompletedDmacCause(uint32_t cause)
{
    std::lock_guard<std::mutex> lock(m_completedDmacMutex);
    m_completedDmacCauses.push_back(cause);
}

std::vector<uint32_t> PS2Memory::consumeCompletedDmacCauses()
{
    std::lock_guard<std::mutex> lock(m_completedDmacMutex);
    std::vector<uint32_t> causes;
    causes.swap(m_completedDmacCauses);
    return causes;
}

void PS2Memory::flushMaskedPath3Packets(bool drainImmediately)
{
    if (m_path3Masked || m_path3MaskedFifo.empty())
        return;

    auto emit = [&](const uint8_t *packetData, uint32_t packetSize)
    {
        if (m_gifArbiter)
            m_gifArbiter->submit(GifPathId::Path3, packetData, packetSize, false);
        else if (m_gifPacketCallback)
            m_gifPacketCallback(packetData, packetSize);
    };

    for (const auto &packet : m_path3MaskedFifo)
    {
        if (packet.size() >= 16u)
            emit(packet.data(), static_cast<uint32_t>(packet.size()));
    }
    m_path3MaskedFifo.clear();

    if (m_gifArbiter && drainImmediately)
        m_gifArbiter->drain();
}

void PS2Memory::submitGifPacket(GifPathId pathId, const uint8_t *data, uint32_t sizeBytes, bool drainImmediately, bool path2DirectHl)
{
    if (!data || sizeBytes < 16)
        return;

    if (pathId == GifPathId::Path3)
    {
        if (m_path3Masked)
        {
            m_path3MaskedFifo.emplace_back(data, data + sizeBytes);
            return;
        }
        flushMaskedPath3Packets(false);
    }

    if (m_gifArbiter)
        m_gifArbiter->submit(pathId, data, sizeBytes, path2DirectHl);
    else if (m_gifPacketCallback)
        m_gifPacketCallback(data, sizeBytes);

    if (m_gifArbiter && drainImmediately)
        m_gifArbiter->drain();
}

void PS2Memory::processGIFPacket(uint32_t srcPhysAddr, uint32_t qwCount)
{
    syncVu1();
    if (!m_rdram || qwCount == 0)
        return;
    const uint64_t bytes64 = static_cast<uint64_t>(qwCount) * 16ull;
    uint32_t sizeBytes = (bytes64 > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<uint32_t>(bytes64);
    uint32_t bytesLeft = sizeBytes;
    while (bytesLeft >= 16)
    {
        if (srcPhysAddr >= PS2_RAM_SIZE)
            srcPhysAddr = 0;
        uint32_t chunk = bytesLeft;
        if (srcPhysAddr + chunk > PS2_RAM_SIZE)
            chunk = PS2_RAM_SIZE - srcPhysAddr;
        if (chunk == 0)
            break;

        m_seenGifCopy = true;
        m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
        submitGifPacket(GifPathId::Path3, m_rdram + srcPhysAddr, chunk);

        bytesLeft -= chunk;
        srcPhysAddr += chunk;
    }
}

void PS2Memory::processGIFPacket(const uint8_t *data, uint32_t sizeBytes)
{
    syncVu1();
    if (m_gifArbiter)
        submitGifPacket(GifPathId::Path3, data, sizeBytes);
    else if (m_gifPacketCallback && data && sizeBytes >= 16)
        m_gifPacketCallback(data, sizeBytes);
}

bool PS2Memory::tryProcessNativeGifImageUploadChain(GS &gs, uint32_t tadr, uint32_t chcr)
{
    if (m_mtvuEnabled)
        return false; // GS packets must stay ordered behind the MTVU queue
    static constexpr uint32_t GIF_CHANNEL = 0x1000A000u;
    static constexpr uint32_t D_STAT = 0x1000E010u;
    static constexpr uint32_t D_CTRL = 0x1000E000u;

    if (!m_rdram || !m_gsVRAM || m_path3Masked)
        return false;
    if (m_gifArbiter && !m_gifArbiter->empty())
        return false;
    if ((chcr & 0x100u) == 0u || ((chcr >> 2u) & 0x3u) != 1u)
        return false;
    if ((chcr & (1u << 7u)) != 0u || ((chcr >> 4u) & 0x3u) != 0u)
        return false;

    const auto dctrlIt = m_ioRegisters.find(D_CTRL);
    if (dctrlIt != m_ioRegisters.end() && ((dctrlIt->second & 0x1u) == 0u))
        return false;

    auto resolveContiguous = [&](uint32_t guestAddr, uint32_t bytes, const uint8_t *&out) -> bool
    {
        try
        {
            const bool scratch = isScratchpad(guestAddr);
            const uint32_t phys = translateAddress(guestAddr);
            const uint8_t *base = scratch ? m_scratchpad : m_rdram;
            const uint32_t limit = scratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
            if (!base || phys > limit || bytes > limit - phys)
                return false;
            out = base + phys;
            return true;
        }
        catch (const std::exception &)
        {
            return false;
        }
    };

    auto loadDmaTagAt = [&](uint32_t guestAddr, DmaTagView &out) -> bool
    {
        const uint8_t *ptr = nullptr;
        if (!resolveContiguous(guestAddr, 16u, ptr))
            return false;
        out = decodeDmaTag(loadScalar<uint64_t>(ptr, 0u, 16u, "native gif dma tag", guestAddr));
        return true;
    };

    auto decodeSetupPayload = [&](const uint8_t *payload, uint64_t (&regs)[4]) -> bool
    {
        const uint64_t tagLo = loadScalar<uint64_t>(payload, 0u, 80u, "native gif setup tag", 0u);
        const uint64_t tagHi = loadScalar<uint64_t>(payload, 8u, 80u, "native gif setup regs", 0u);
        if (gifTagNloop(tagLo) != 4u ||
            gifTagFlg(tagLo) != GIF_FMT_PACKED ||
            gifTagNreg(tagLo) != 1u ||
            (tagHi & 0xFull) != 0x0Eull)
        {
            return false;
        }

        static constexpr uint8_t kExpectedRegs[4] = {
            GS_REG_BITBLTBUF,
            GS_REG_TRXPOS,
            GS_REG_TRXREG,
            GS_REG_TRXDIR,
        };

        uint32_t offset = 16u;
        for (uint32_t i = 0; i < 4u; ++i)
        {
            regs[i] = loadScalar<uint64_t>(payload, offset, 80u, "native gif setup value", 0u);
            const uint64_t reg = loadScalar<uint64_t>(payload, offset + 8u, 80u, "native gif setup register", 0u);
            if ((reg & 0xFFu) != kExpectedRegs[i])
                return false;
            offset += 16u;
        }

        const uint32_t trxdirMode = static_cast<uint32_t>(regs[3] & 0x3ull);
        const uint32_t rrw = static_cast<uint32_t>(regs[2] & 0xFFFull);
        const uint32_t rrh = static_cast<uint32_t>((regs[2] >> 32u) & 0xFFFull);
        return trxdirMode == 0u && rrw != 0u && rrh != 0u;
    };

    DmaTagView setupTag{};
    if (!loadDmaTagAt(tadr, setupTag) ||
        setupTag.id != 1u ||
        setupTag.qwc != 5u ||
        setupTag.irq)
    {
        return false;
    }

    const uint8_t *setupPayload = nullptr;
    const uint32_t setupPayloadAddr = tadr + 16u;
    if (!resolveContiguous(setupPayloadAddr, 5u * 16u, setupPayload))
        return false;

    uint64_t setupRegs[4] = {};
    if (!decodeSetupPayload(setupPayload, setupRegs))
        return false;

    uint32_t imageTagDmaAddr = setupPayloadAddr + 5u * 16u;
    DmaTagView imageTagDma{};
    if (!loadDmaTagAt(imageTagDmaAddr, imageTagDma) ||
        imageTagDma.id != 1u ||
        imageTagDma.qwc != 1u ||
        imageTagDma.irq)
    {
        return false;
    }

    const uint8_t *imageGifTag = nullptr;
    if (!resolveContiguous(imageTagDmaAddr + 16u, 16u, imageGifTag))
        return false;

    const uint64_t imageTagLo = loadScalar<uint64_t>(imageGifTag, 0u, 16u, "native gif image tag", imageTagDmaAddr + 16u);
    if (gifTagFlg(imageTagLo) != GIF_FMT_IMAGE)
        return false;

    const uint32_t imageQwc = gifTagNloop(imageTagLo);
    if (imageQwc == 0u)
        return false;

    const uint64_t imageBytes64 = static_cast<uint64_t>(imageQwc) * 16ull;
    if (imageBytes64 > 0xFFFFFFFFull)
        return false;
    const uint32_t imageBytes = static_cast<uint32_t>(imageBytes64);

    const uint32_t payloadTagAddr = imageTagDmaAddr + 32u;
    DmaTagView payloadTag{};
    if (!loadDmaTagAt(payloadTagAddr, payloadTag) ||
        payloadTag.qwc != imageQwc ||
        payloadTag.irq)
    {
        return false;
    }

    uint32_t imageDataAddr = 0u;
    uint32_t finalTadr = payloadTagAddr;
    uint32_t lastTagUpper = payloadTag.upper;
    if (payloadTag.id == 3u || payloadTag.id == 4u)
    {
        imageDataAddr = payloadTag.addr;
        const uint32_t terminalTagAddr = payloadTagAddr + 16u;
        DmaTagView terminalTag{};
        if (!loadDmaTagAt(terminalTagAddr, terminalTag) ||
            terminalTag.qwc != 0u ||
            terminalTag.irq ||
            (terminalTag.id != 0u && terminalTag.id != 7u))
        {
            return false;
        }
        finalTadr = (terminalTag.id == 0u) ? (terminalTagAddr + 16u) : terminalTagAddr;
        lastTagUpper = terminalTag.upper;
    }
    else if (payloadTag.id == 7u)
    {
        imageDataAddr = payloadTagAddr + 16u;
        finalTadr = payloadTagAddr;
    }
    else
    {
        return false;
    }

    const uint8_t *imageData = nullptr;
    if (!resolveContiguous(imageDataAddr, imageBytes, imageData))
        return false;

    m_dmaStartCount.fetch_add(1, std::memory_order_relaxed);
    m_seenGifCopy = true;
    m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);
    gs.uploadImageNative(setupRegs[0], setupRegs[1], setupRegs[2], setupRegs[3], imageData, imageBytes);

    m_ioRegisters[GIF_CHANNEL + 0x30u] = finalTadr;
    m_ioRegisters[GIF_CHANNEL + 0x40u] = 0u;
    m_ioRegisters[GIF_CHANNEL + 0x50u] = 0u;
    m_ioRegisters[GIF_CHANNEL + 0x00u] = ((chcr & 0x0000FFFFu) | (lastTagUpper << 16u)) & ~0x100u;
    m_ioRegisters[GIF_CHANNEL + 0x20u] = 0u;

    uint32_t dstat = m_ioRegisters.count(D_STAT) ? m_ioRegisters[D_STAT] : 0u;
    dstat |= (1u << 2u);
    const uint32_t status = dstat & 0x3FFu;
    const uint32_t mask = (dstat >> 16u) & 0x3FFu;
    if ((status & mask) != 0u)
        dstat |= (1u << 31u);
    else
        dstat &= ~(1u << 31u);
    m_ioRegisters[D_STAT] = dstat;
    queueCompletedDmacCause(2u);
    return true;
}

bool PS2Memory::tryProcessNativeGifPackedChain(GS &gs, uint32_t tadr, uint32_t chcr)
{
    if (m_mtvuEnabled)
        return false; // GS packets must stay ordered behind the MTVU queue
    static constexpr uint32_t GIF_CHANNEL = 0x1000A000u;
    static constexpr uint32_t D_STAT = 0x1000E010u;
    static constexpr uint32_t D_CTRL = 0x1000E000u;

    if (!m_rdram || !m_gsVRAM || m_path3Masked)
        return false;
    if (m_gifArbiter && !m_gifArbiter->empty())
        return false;
    if ((chcr & 0x100u) == 0u || ((chcr >> 2u) & 0x3u) != 1u)
        return false;
    if ((chcr & (1u << 7u)) != 0u || ((chcr >> 4u) & 0x3u) != 0u)
        return false;

    const auto dctrlIt = m_ioRegisters.find(D_CTRL);
    if (dctrlIt != m_ioRegisters.end() && ((dctrlIt->second & 0x1u) == 0u))
        return false;

    auto resolveContiguous = [&](uint32_t guestAddr, uint32_t bytes, const uint8_t *&out) -> bool
    {
        try
        {
            const bool scratch = isScratchpad(guestAddr);
            const uint32_t phys = translateAddress(guestAddr);
            const uint8_t *base = scratch ? m_scratchpad : m_rdram;
            const uint32_t limit = scratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
            if (!base || phys > limit || bytes > limit - phys)
                return false;
            out = base + phys;
            return true;
        }
        catch (const std::exception &)
        {
            return false;
        }
    };

    const uint8_t *tagPtr = nullptr;
    if (!resolveContiguous(tadr, 16u, tagPtr))
        return false;

    const DmaTagView tag = decodeDmaTag(loadScalar<uint64_t>(tagPtr, 0u, 16u, "native packed gif dma tag", tadr));
    if (tag.id != 7u || tag.qwc == 0u || tag.irq)
        return false;

    const uint64_t payloadBytes64 = static_cast<uint64_t>(tag.qwc) * 16ull;
    if (payloadBytes64 > 0xFFFFFFFFull)
        return false;
    const uint32_t payloadBytes = static_cast<uint32_t>(payloadBytes64);

    const uint8_t *payload = nullptr;
    if (!resolveContiguous(tadr + 16u, payloadBytes, payload))
        return false;
    if (!gs.processNativePackedGIFPacket(payload, payloadBytes))
        return false;

    m_dmaStartCount.fetch_add(1, std::memory_order_relaxed);
    m_seenGifCopy = true;
    m_gifCopyCount.fetch_add(1, std::memory_order_relaxed);

    m_ioRegisters[GIF_CHANNEL + 0x30u] = tadr;
    m_ioRegisters[GIF_CHANNEL + 0x40u] = 0u;
    m_ioRegisters[GIF_CHANNEL + 0x50u] = 0u;
    m_ioRegisters[GIF_CHANNEL + 0x00u] = ((chcr & 0x0000FFFFu) | (tag.upper << 16u)) & ~0x100u;
    m_ioRegisters[GIF_CHANNEL + 0x20u] = 0u;

    uint32_t dstat = m_ioRegisters.count(D_STAT) ? m_ioRegisters[D_STAT] : 0u;
    dstat |= (1u << 2u);
    const uint32_t status = dstat & 0x3FFu;
    const uint32_t mask = (dstat >> 16u) & 0x3FFu;
    if ((status & mask) != 0u)
        dstat |= (1u << 31u);
    else
        dstat &= ~(1u << 31u);
    m_ioRegisters[D_STAT] = dstat;
    queueCompletedDmacCause(2u);
    return true;
}

int PS2Memory::pollDmaRegisters()
{
    return 0;
}

uint32_t PS2Memory::readIORegister(uint32_t address)
{
    g_mmioCensus.count(address, false);
    // VIF1_STAT / GIF_STAT while the VU1 worker still has work: report the
    // paths as busy instead of waiting, like the hardware. Games poll these
    // once per vsync to decide whether to flip (SotC's iosCheckDrawFinish);
    // blocking here held the vsync thread and bunched frames.
    constexpr uint32_t kVif1Stat = 0x10003C00u, kGifStat = 0x10003020u;
    if ((address == kVif1Stat || address == kGifStat) &&
        m_mtvuOutstanding.load(std::memory_order_acquire) != 0u && !onVu1Worker())
    {
        const auto it = m_ioRegisters.find(address);
        const uint32_t value = it != m_ioRegisters.end() ? it->second : 0u;
        if (address == kVif1Stat)
            return value | 0x01000000u; // FQC: data still queued
        m_timerIrqLatched |= syncAndFlushEeTimers();
        return value | 0x00000C00u; // APATH: PATH3 transferring
    }
    if ((address >= 0x10003000u && address < 0x10003100u) || (address >= 0x10003C00u && address < 0x10003E00u))
        syncVu1(); // GIF / VIF1 registers
    if (address == 0x10003020u || address < 0x10002000u)
        m_timerIrqLatched |= syncAndFlushEeTimers(); // timers and GIF_STAT.FQC see the elapsed cycles
    size_t timerIndex = 0u;
    uint32_t timerOffset = 0u;
    if (decodeEeTimerRegister(address, timerIndex, timerOffset))
    {
        const EeTimer &timer = m_eeTimers[timerIndex];
        switch (timerOffset)
        {
        case kEeTimerCountOffset:
            return timer.count & 0xFFFFu;
        case kEeTimerModeOffset:
            return timer.mode & (kEeTimerModeConfigMask | kEeTimerModeStatusMask);
        case kEeTimerCompareOffset:
            return timer.compare & 0xFFFFu;
        case kEeTimerHoldOffset:
            return timer.hold & 0xFFFFu;
        default:
            return 0u;
        }
    }

    if (isGsPrivReg(address))
    {
        syncVu1();
        // NB: unreachable from read8/16/32/64 today, same reasoning as the write
        // path above; kept correct for direct callers.
        const uint32_t off = address & 7u;
        const uint32_t regOff = (address - PS2_GS_PRIV_REG_BASE) & ~0x7u;
        if (regOff == kGsCsrRegOffset)
        {
            return static_cast<uint32_t>((gs_regs.csr.load() >> (off * 8u)) & 0xFFFFFFFFull);
        }
        if (uint64_t *reg = gsRegPtr(gs_regs, address))
        {
            return static_cast<uint32_t>((*reg >> (off * 8u)) & 0xFFFFFFFFull);
        }
        return 0u;
    }

    if (address >= 0x10002000 && address <= 0x10002030)
    {
        uint32_t val = 0;
        switch (address)
        {
        case 0x10002000:
            val = m_ioRegisters[address];
            break;
        case 0x10002010:
            val = m_ioRegisters[address] & ~(1u << 31);
            break;
        case 0x10002020:
        case 0x10002030:
            val = m_ioRegisters[address];
            break;
        default:
            val = 0;
            break;
        }
        return val;
    }

    if (address == 0x10003020u) // GIF_STAT
    {
        uint32_t stat = m_ioRegisters.count(address) ? m_ioRegisters[address] : 0u;
        const uint32_t mode = m_ioRegisters.count(0x10003010u) ? m_ioRegisters[0x10003010u] : 0u;
        const uint32_t ctrl = m_ioRegisters.count(0x10003000u) ? m_ioRegisters[0x10003000u] : 0u;

        // M3R and IMT mirror GIF_MODE, PSE mirrors GIF_CTRL, and M3P is the
        // effective PATH3 mask controlled by the VIF1 MSKPATH3 command.
        stat = (stat & ~0xFu) |
               (mode & 0x1u) |
               (m_path3Masked ? 0x2u : 0u) |
               (mode & 0x4u) |
               (ctrl & 0x8u);
        return stat;
    }

    if (address >= 0x10000000 && address < 0x10010000)
    {
        if (address >= 0x10008000 && address < 0x1000F000)
        {
            if ((address & 0xFF) == 0x00)
            {
                uint32_t channelStatus = m_ioRegisters[address] & ~0x100u;
                m_ioRegisters[address] = channelStatus;
                return channelStatus;
            }
        }

        if (address >= 0x10000200 && address < 0x10000300)
        {
            return 0;
        }

        if (address >= 0x1000F200 && address <= 0x1000F260)
        {
            if (address == 0x1000F230)
            {
                return 0x60000;
            }
            if (address == 0x1000F240)
            {
                return 0xF0000002;
            }
            return 0;
        }
    }

    auto it = m_ioRegisters.find(address);
    if (it != m_ioRegisters.end())
    {
        return it->second;
    }

    return 0;
}

void PS2Memory::registerCodeRegion(uint32_t start, uint32_t end)
{
    if (end <= start)
    {
        std::cerr << "Ignoring invalid code region: start=0x" << std::hex << start
                  << " end=0x" << end << std::dec << std::endl;
        return;
    }

    if ((end - start) > PS2_RAM_SIZE)
    {
        std::cerr << "Ignoring oversized code region: start=0x" << std::hex << start
                  << " end=0x" << end << std::dec << std::endl;
        return;
    }

    for (const auto &existing : m_codeRegions)
    {
        if (existing.start == start && existing.end == end)
        {
            return;
        }
    }

    CodeRegion region;
    region.start = start;
    region.end = end;

    size_t sizeInWords = (end - start + 3u) / 4u;
    region.modified.resize(sizeInWords, false);

    m_codeRegions.push_back(region);
    RUNTIME_LOG("Registered code region: " << std::hex << start << " - " << end << std::dec);
    if (std::getenv("PS2X_CODE_REGION_LOG"))
        std::cerr << "[code-region] " << std::hex << start << " - " << end << std::dec << std::endl;
}

bool PS2Memory::isAddressInRegion(uint32_t address, const CodeRegion &region)
{
    return (address >= region.start && address < region.end);
}

bool PS2Memory::isCodeAddress(uint32_t address) const
{
    for (const auto &region : m_codeRegions)
    {
        if (address >= region.start && address < region.end)
        {
            return true;
        }
    }
    return false;
}

void PS2Memory::markModified(uint32_t address, uint32_t size)
{
    if (size == 0)
    {
        return;
    }

    const uint64_t writeEnd = static_cast<uint64_t>(address) + static_cast<uint64_t>(size);
    for (auto &region : m_codeRegions)
    {
        const uint64_t regionStart = region.start;
        const uint64_t regionEnd = region.end;
        if (writeEnd <= regionStart || static_cast<uint64_t>(address) >= regionEnd)
        {
            continue;
        }

        uint32_t overlapStart = static_cast<uint32_t>(std::max<uint64_t>(address, regionStart));
        uint32_t overlapEnd = static_cast<uint32_t>(std::min<uint64_t>(writeEnd, regionEnd));

        for (uint32_t addr = overlapStart; addr < overlapEnd; addr += 4)
        {
            size_t bitIndex = (addr - region.start) / 4;
            if (bitIndex < region.modified.size())
            {
                region.modified[bitIndex] = true;
                RUNTIME_LOG("Marked code at " << std::hex << addr << std::dec << " as modified");
            }
        }
    }
}

bool PS2Memory::isCodeModified(uint32_t address, uint32_t size)
{
    if (size == 0)
    {
        return false;
    }

    const uint64_t writeEnd = static_cast<uint64_t>(address) + static_cast<uint64_t>(size);
    for (const auto &region : m_codeRegions)
    {
        const uint64_t regionStart = region.start;
        const uint64_t regionEnd = region.end;
        if (writeEnd <= regionStart || static_cast<uint64_t>(address) >= regionEnd)
        {
            continue;
        }

        uint32_t overlapStart = static_cast<uint32_t>(std::max<uint64_t>(address, regionStart));
        uint32_t overlapEnd = static_cast<uint32_t>(std::min<uint64_t>(writeEnd, regionEnd));

        for (uint32_t addr = overlapStart; addr < overlapEnd; addr += 4)
        {
            size_t bitIndex = (addr - region.start) / 4;
            if (bitIndex < region.modified.size() && region.modified[bitIndex])
            {
                return true; // Found modified code
            }
        }
    }

    return false; // No modifications found
}

void PS2Memory::clearModifiedFlag(uint32_t address, uint32_t size)
{
    if (size == 0)
    {
        return;
    }

    const uint64_t writeEnd = static_cast<uint64_t>(address) + static_cast<uint64_t>(size);
    for (auto &region : m_codeRegions)
    {
        const uint64_t regionStart = region.start;
        const uint64_t regionEnd = region.end;
        if (writeEnd <= regionStart || static_cast<uint64_t>(address) >= regionEnd)
        {
            continue;
        }

        uint32_t overlapStart = static_cast<uint32_t>(std::max<uint64_t>(address, regionStart));
        uint32_t overlapEnd = static_cast<uint32_t>(std::min<uint64_t>(writeEnd, regionEnd));

        for (uint32_t addr = overlapStart; addr < overlapEnd; addr += 4)
        {
            size_t bitIndex = (addr - region.start) / 4;
            if (bitIndex < region.modified.size())
            {
                region.modified[bitIndex] = false;
            }
        }
    }
}

// Round 31: the VIF1 chain's tag loop when the walker takes the chain and
// TTE is on (every chain SotC sends with the native renderer drawing).
// The same tags in the same order as the general loop in writeIORegister,
// with:
// - main RAM tags and payloads read in place (an address below 32 MB is
//   main RAM in translateAddress, and the range tests are the same); any
//   other address goes the general way (translateAddress, the scratchpad,
//   the wrap at the region's end);
// - the packet cache (useCache): at a CALL whose target packet (the tags up
//   to the RET back to this level, NEXT / CNT / REF only) was walked before
//   with the same CYCLE, from a clean walker and an idle DIRECT scanner, and
//   every byte that walk read (tags, VIFcodes, DIRECT data; not the UNPACK /
//   MPG data, which the walker never reads) is the same, the walk's effect
//   is replayed (Vif1Walker::replay) instead of walking it again. A packet
//   is kept only when its codes are NOP / FLUSH* / UNPACK / MSCAL / MSCALF /
//   MSCNT / MPG / DIRECT, it raised no GS event, no tag has the IRQ bit, and
//   it ends clean. Those codes' effects depend on CYCLE (kept) and on BASE /
//   OFST / ITOPS / TOPS / DBF, which no kept code changes but MSCAL, which
//   replay() runs in order: the result is the walk's.
void PS2Memory::walkVif1Chain(Vif1Walker &walker, GifEventScanner &gif, Vif1ChainState &st, bool tie, bool useCache)
{
    const int kMaxChainTags = 1 << 20;
    if (useCache)
        ++g_vif1Walks;
    // One record reused (its vectors keep their capacity: no allocation per
    // packet; only the cached walk records).
    static Vif1Walker::Record rec;
    bool recording = false;
    uint32_t recKey = 0, recCycle = 0;
    int recTags0 = 0;
    uint64_t recEvents0 = 0;
    auto events = []()
    { return GifEventScanner::s_events[0] + GifEventScanner::s_events[1] + GifEventScanner::s_events[2]; };
    auto stopRecording = [&]()
    {
        walker.record(nullptr);
        recording = false;
    };
    // A payload or tag word range outside main RAM: the general path.
    auto feedGeneral = [&](uint32_t srcAddr, uint32_t bytes)
    {
        const bool scratch = isScratchpad(srcAddr);
        uint32_t src = translateAddress(srcAddr);
        const uint8_t *base = scratch ? m_scratchpad : m_rdram;
        const uint32_t maxSz = scratch ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
        while (bytes > 0)
        {
            if (src >= maxSz)
                src = 0;
            uint32_t chunk = bytes;
            if (src + chunk > maxSz)
                chunk = maxSz - src;
            if (chunk == 0)
                break;
            walker.feed(base + src, chunk);
            bytes -= chunk;
            src += chunk;
        }
    };

    while (st.tags < kMaxChainTags)
    {
        const uint32_t tagAddr = st.tagAddr;
        const uint8_t *tp;
        bool inRam = tagAddr <= PS2_RAM_SIZE - 16u;
        if (PS2_EXPECT(inRam, 1))
            tp = m_rdram + tagAddr;
        else
        {
            const bool tagInSPR = isScratchpad(tagAddr);
            uint32_t physTag = 0;
            try
            {
                physTag = translateAddress(tagAddr);
            }
            catch (...)
            {
                break;
            }
            const uint8_t *tagBase = tagInSPR ? m_scratchpad : m_rdram;
            const uint32_t tagMax = tagInSPR ? PS2_SCRATCHPAD_SIZE : PS2_RAM_SIZE;
            if (physTag + 16 > tagMax)
                break;
            tp = tagBase + physTag;
        }
        uint64_t tag;
        std::memcpy(&tag, tp, 8);
        const uint16_t tagQwc = static_cast<uint16_t>(tag & 0xFFFF);
        const uint32_t id = static_cast<uint32_t>((tag >> 28) & 0x7);
        const bool irq = ((tag >> 31) & 0x1ull) != 0ull;
        const uint32_t addr = static_cast<uint32_t>((tag >> 32) & 0x7FFFFFFF);
        st.lastTagUpper = static_cast<uint32_t>((tag >> 16) & 0xFFFFu);
        ++st.tags;
        if (recording)
        {
            if (!inRam || irq || id == 0u || id == 5u || id == 7u)
                rec.bad = true;
            else
                rec.add(tp, 8u);
        }

        uint32_t dataAddr = 0;
        bool hasPayload = tagQwc > 0;
        bool endChain = false;
        bool callTarget = false, ret = false;
        const uint32_t aspBefore = st.asp;
        switch (id)
        {
        case 0:
            dataAddr = addr;
            st.tagAddr = tagAddr + 16;
            endChain = true;
            break;
        case 1:
            dataAddr = tagAddr + 16;
            st.tagAddr = dataAddr + static_cast<uint32_t>(tagQwc) * 16u;
            break;
        case 2:
            dataAddr = tagAddr + 16;
            st.tagAddr = addr;
            break;
        case 3:
        case 4:
            dataAddr = addr;
            st.tagAddr = tagAddr + 16;
            break;
        case 5:
            dataAddr = tagAddr + 16;
            {
                const uint32_t retAddr = dataAddr + static_cast<uint32_t>(tagQwc) * 16u;
                if (st.asp == 0u)
                {
                    st.asr0 = retAddr;
                    st.asp = 1u;
                }
                else if (st.asp == 1u)
                {
                    st.asr1 = retAddr;
                    st.asp = 2u;
                }
            }
            st.tagAddr = addr;
            callTarget = aspBefore < 2u;
            break;
        case 6:
            dataAddr = tagAddr + 16;
            if (st.asp == 2u)
            {
                st.tagAddr = st.asr1;
                st.asp = 1u;
            }
            else if (st.asp == 1u)
            {
                st.tagAddr = st.asr0;
                st.asp = 0u;
            }
            else
            {
                endChain = true;
            }
            ret = !endChain;
            break;
        case 7:
            dataAddr = tagAddr + 16;
            endChain = true;
            break;
        default:
            hasPayload = false;
            endChain = true;
            break;
        }

        // As the general loop: the next tag read now, its data's first lines asked for.
        if (!endChain && ps2_fast_paths::g_dmaPrefetch && st.tagAddr < PS2_RAM_SIZE - 16u)
        {
            uint64_t next;
            std::memcpy(&next, m_rdram + st.tagAddr, 8);
            const uint32_t nextId = static_cast<uint32_t>((next >> 28) & 0x7u);
            const uint32_t nextData = (nextId == 3u || nextId == 4u || nextId == 0u) ? static_cast<uint32_t>((next >> 32) & 0x7FFFFFFFu) : st.tagAddr + 16u;
            if ((next & 0xFFFFu) != 0u && nextData < PS2_RAM_SIZE - 128u)
            {
                __builtin_prefetch(m_rdram + nextData);
                __builtin_prefetch(m_rdram + nextData + 64u);
            }
            __builtin_prefetch(m_rdram + st.tagAddr + 64u);
        }

        // The tag's two VIFcodes (TTE), then its payload.
        if (ps2_fast_paths::g_dmaWalkFast && walker.feedTagWords(tp + 8))
        {
            if (recording)
            {
                rec.add(tp + 8, 8u);
                rec.anyCode = true;
                rec.code = 0u;
                rec.num = 0u;
                rec.bytes += 8u;
            }
        }
        else if (PS2_EXPECT(inRam, 1))
            walker.feed(tp + 8, 8u);
        else
        {
            if (recording)
                rec.bad = true;
            walker.feed(tp + 8, 8u);
        }
        if (hasPayload)
        {
            const uint32_t bytes = static_cast<uint32_t>(tagQwc) * 16u;
            if (PS2_EXPECT(dataAddr < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - dataAddr, 1))
                walker.feed(m_rdram + dataAddr, bytes);
            else
            {
                if (recording)
                    rec.bad = true;
                feedGeneral(dataAddr, bytes);
            }
        }
        if (irq && tie)
            endChain = true;
        if (endChain)
            break;

        if (recording && ret)
        {
            // The packet's RET: back at the CALL's level (no CALL inside it).
            stopRecording();
            if (!rec.bad && walker.clean() && gif.idle() && events() == recEvents0)
            {
                if (g_vif1PacketBytes > kVif1PacketBudget)
                {
                    // Over budget (the slots' vectors): all of them freed.
                    std::vector<Vif1PacketSlot>().swap(g_vif1Packets);
                    g_vif1PacketBytes = 0;
                    g_vif1PacketCount = 0;
                    ++g_vif1PacketStats.clears;
                }
                Vif1Packet &p = vif1PacketTake(recKey);
                g_vif1PacketBytes -= vif1PacketSize(p);
                p.cycle = recCycle;
                p.tags = st.tags - recTags0;
                p.lastTagUpper = st.lastTagUpper;
                p.lastChain = g_vif1Walks;
                size_t total = 0;
                for (const auto &[a, n] : rec.ranges)
                    total += n;
                p.bytes.resize(total);
                uint8_t *out = p.bytes.data();
                for (const auto &[a, n] : rec.ranges)
                {
                    std::memcpy(out, m_rdram + a, n);
                    out += n;
                }
                p.rec.ranges.assign(rec.ranges.begin(), rec.ranges.end());
                p.rec.ops.assign(rec.ops.begin(), rec.ops.end());
                p.rec.base = rec.base;
                p.rec.bad = false;
                p.rec.anyCode = rec.anyCode;
                p.rec.intSeen = rec.intSeen;
                p.rec.code = rec.code;
                p.rec.num = rec.num;
                p.rec.mscal = rec.mscal;
                p.rec.mscnt = rec.mscnt;
                p.rec.unpack = rec.unpack;
                p.rec.bytes = rec.bytes;
                g_vif1PacketBytes += vif1PacketSize(p);
                ++g_vif1PacketStats.stored;
            }
            else
            {
                g_vif1PacketStats.bad += rec.bad;
                vif1PacketErase(recKey);
            }
        }
        else if (recording && rec.bad)
        {
            stopRecording();
            ++g_vif1PacketStats.bad;
            vif1PacketErase(recKey);
        }

        if (callTarget && useCache && !recording && st.tagAddr <= PS2_RAM_SIZE - 16u && walker.clean() && gif.idle())
        {
            ++g_vif1PacketStats.calls;
            Vif1Packet *hit = vif1PacketFind(st.tagAddr);
            if (hit && hit->cycle == walker.cycle() && st.tags + hit->tags < kMaxChainTags && vif1PacketMatches(m_rdram, *hit))
            {
                // Replayed: the packet's effect, then its RET.
                Vif1Packet &p = *hit;
                p.lastChain = g_vif1Walks;
                walker.replay(p.rec);
                st.tags += p.tags;
                st.lastTagUpper = p.lastTagUpper;
                if (st.asp == 2u)
                {
                    st.tagAddr = st.asr1;
                    st.asp = 1u;
                }
                else
                {
                    st.tagAddr = st.asr0;
                    st.asp = 0u;
                }
                ++g_vif1PacketStats.hits;
                g_vif1PacketStats.hitTags += p.tags;
                continue;
            }
            ++g_vif1PacketStats.misses;
            rec.ranges.clear();
            rec.ops.clear();
            rec.bad = rec.anyCode = rec.intSeen = false;
            rec.code = rec.num = rec.mscal = rec.mscnt = rec.unpack = 0u;
            rec.bytes = 0u;
            rec.base = m_rdram;
            recKey = st.tagAddr;
            recCycle = walker.cycle();
            recTags0 = st.tags;
            recEvents0 = events();
            recording = true;
            walker.record(&rec);
        }
    }
    if (recording)
        stopRecording();
    g_vif1PacketStats.tags += static_cast<uint64_t>(st.tags);
}

void PS2Memory::hostDmaStart(uint32_t channelBase, uint32_t chcr, uint32_t madr, uint32_t qwc, uint32_t tadr)
{
    t_hostIo = true;
    // As the game's own stores would: QWC, then TADR (chain) or MADR (normal).
    writeIORegister(channelBase + 0x20u, qwc);
    if (chcr & 0x4u)
        writeIORegister(channelBase + 0x30u, tadr);
    else
        writeIORegister(channelBase + 0x10u, madr);
    writeIORegister(channelBase, chcr);
    t_hostIo = false;
}

bool PS2Memory::hostDrawPathsIdle()
{
    t_hostIo = true;
    const bool idle = (readIORegister(0x10009000u) & 0x100u) == 0u && (readIORegister(0x1000A000u) & 0x100u) == 0u &&
                      (readIORegister(0x10003C00u) & 0x1F000003u) == 0u && (readIORegister(0x10003020u) & 0xC00u) == 0u;
    t_hostIo = false;
    return idle;
}

void PS2Memory::hostStopEeTimer(size_t index)
{
    if (index >= m_eeTimers.size())
        return;
    m_timerIrqLatched |= syncAndFlushEeTimers();
    m_timerIrqLatched &= ~(1u << index); // nor an interrupt it raised already
    m_eeTimers[index].mode = 0u;
    m_eeTimers[index].count = 0u;
    m_timerBudget = cyclesUntilNextEeTimerInterruptNow();
}

bool PS2Memory::hostChannelBusy(uint32_t channelBase)
{
    t_hostIo = true;
    const bool busy = (readIORegister(channelBase) & 0x100u) != 0u;
    t_hostIo = false;
    return busy;
}

namespace ps2_fast_paths
{
    bool g_sprFast = []
    {
        const char *env = std::getenv("PS2X_SPR_FAST");
        return !env || std::atoi(env) != 0;
    }();
    bool g_dmaPrefetch = []
    {
        const char *env = std::getenv("PS2X_DMA_PREFETCH");
        return !env || std::atoi(env) != 0;
    }();
    bool g_dmaWalkFast = []
    {
        const char *env = std::getenv("PS2X_DMA_WALK_FAST");
        return !env || std::atoi(env) != 0;
    }();    bool g_dmaWalk2 = []
    {
        const char *env = std::getenv("PS2X_DMA_WALK2");
        return !env || std::atoi(env) != 0;
    }();
    // Off by default (round 31): exact, but slower than the walk it
    // replaces in calibrated Vita3K (walk + area change +1.2 ms at 222).
    bool g_dmaCache = []
    {
        const char *env = std::getenv("PS2X_DMA_CACHE");
        return env && std::atoi(env) != 0;
    }();
}

bool GifEventScanner::fast() { return ps2_fast_paths::g_dmaWalkFast; }

// The out-of-line half of the guest load / store macros (ps2_runtime_macros.h
// READ* / WRITE*): every address at or above 32 MB. The special ranges go to
// the runtime; the rest (the uncached / kseg0 mirrors of main RAM) is main
// RAM, masked.
namespace ps2_guest_mem
{
    // The scratchpad (0x70000000, 16 KB) straight from here: an aligned
    // access there went through PS2Runtime::LoadN / StoreN and
    // PS2Memory::readN / writeN (the GS / IO / mirror tests, the address
    // translation, the range check) to end in the same load or store.
    // SotC's display walk keeps its light work there (LoopExecPreLightColMan
    // copies it in for the frame). PS2X_SPR_FAST=0: the long way, as before.
    namespace
    {
        template <uint32_t N>
        PS2_FORCE_INLINE uint8_t *spr(PS2Runtime *runtime, uint32_t a)
        {
            const uint32_t offset = a - PS2_SCRATCHPAD_BASE;
            if (PS2_EXPECT(offset < PS2_SCRATCHPAD_SIZE && (offset & (N - 1u)) == 0u && ps2_fast_paths::g_sprFast, 1))
                return runtime->memory().getScratchpad() + offset;
            return nullptr;
        }
        template <typename T>
        PS2_FORCE_INLINE T sprLoad(const uint8_t *p)
        {
            T v;
            std::memcpy(&v, p, sizeof(T));
            return v;
        }
    }

    uint8_t read8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        if (const uint8_t *p = spr<1>(runtime, a))
            return *p;
        return Ps2IsSpecialAddress(a) ? runtime->Load8(rdram, ctx, a) : Ps2FastRead8(rdram, a);
    }
    uint16_t read16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        if (const uint8_t *p = spr<2>(runtime, a))
            return sprLoad<uint16_t>(p);
        return Ps2IsSpecialAddress(a) ? runtime->Load16(rdram, ctx, a) : Ps2FastRead16(rdram, a);
    }
    uint32_t read32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        if (const uint8_t *p = spr<4>(runtime, a))
            return sprLoad<uint32_t>(p);
        return Ps2IsSpecialAddress(a) ? runtime->Load32(rdram, ctx, a) : Ps2FastRead32(rdram, a);
    }
    uint64_t read64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        if (const uint8_t *p = spr<8>(runtime, a))
            return sprLoad<uint64_t>(p);
        return Ps2IsSpecialAddress(a) ? runtime->Load64(rdram, ctx, a) : Ps2FastRead64(rdram, a);
    }
    __m128i read128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        if (const uint8_t *p = spr<16>(runtime, a))
            return sprLoad<__m128i>(p);
        return Ps2IsSpecialAddress(a) ? runtime->Load128(rdram, ctx, a) : Ps2FastRead128(rdram, a);
    }
    void write8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint8_t v)
    {
        if (uint8_t *p = spr<1>(runtime, a))
        {
            *p = v;
            return;
        }
        if (Ps2IsSpecialAddress(a))
            return runtime->Store8(rdram, ctx, a, v);
        trace<uint8_t>(rdram, a, v, "WRITE8", ctx);
        Ps2FastWrite8(rdram, a, v);
    }
    void write16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint16_t v)
    {
        if (uint8_t *p = spr<2>(runtime, a))
        {
            std::memcpy(p, &v, sizeof(v));
            return;
        }
        if (Ps2IsSpecialAddress(a))
            return runtime->Store16(rdram, ctx, a, v);
        trace<uint16_t>(rdram, a, v, "WRITE16", ctx);
        Ps2FastWrite16(rdram, a, v);
    }
    void write32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t v)
    {
        if (uint8_t *p = spr<4>(runtime, a))
        {
            std::memcpy(p, &v, sizeof(v));
            return;
        }
        if (Ps2IsSpecialAddress(a))
            return runtime->Store32(rdram, ctx, a, v);
        trace<uint32_t>(rdram, a, v, "WRITE32", ctx);
        Ps2FastWrite32(rdram, a, v);
    }
    void write64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint64_t v)
    {
        if (uint8_t *p = spr<8>(runtime, a))
        {
            std::memcpy(p, &v, sizeof(v));
            return;
        }
        if (Ps2IsSpecialAddress(a))
            return runtime->Store64(rdram, ctx, a, v);
        trace<uint64_t>(rdram, a, v, "WRITE64", ctx);
        Ps2FastWrite64(rdram, a, v);
    }
    void write128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128i v)
    {
        if (uint8_t *p = spr<16>(runtime, a))
        {
            std::memcpy(p, &v, sizeof(v));
            return;
        }
        if (Ps2IsSpecialAddress(a))
            return runtime->Store128(rdram, ctx, a, v);
        trace<__m128i>(rdram, a, v, "WRITE128", ctx);
        Ps2FastWrite128(rdram, a, v);
    }
}

// Round 31: the Vita trampolines of the 8-64-bit slow paths
// (PS2X_GUEST_MEM_TRAMP, ps2_runtime_macros.h). Each saves what an AAPCS
// call clobbers (r0-r3 but the result, ip, d0-d7, d16-d31), puts the C
// arguments in place (rdram and the runtime from these globals, a store's
// value on the stack) and calls the same slow path as the plain macros. The
// unwind directives let a C++ exception from the slow path pass through.
extern "C"
{
    uint8_t *g_ps2gmRdram = nullptr;
    PS2Runtime *g_ps2gmRuntime = nullptr;
#if PS2X_GUEST_MEM_TRAMP
    uint8_t (*ps2gm_fn_read8)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t) = &ps2_guest_mem::read8;
    uint16_t (*ps2gm_fn_read16)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t) = &ps2_guest_mem::read16;
    uint32_t (*ps2gm_fn_read32)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t) = &ps2_guest_mem::read32;
    uint64_t (*ps2gm_fn_read64)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t) = &ps2_guest_mem::read64;
    void (*ps2gm_fn_write8)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t, uint8_t) = &ps2_guest_mem::write8;
    void (*ps2gm_fn_write16)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t, uint16_t) = &ps2_guest_mem::write16;
    void (*ps2gm_fn_write32)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t, uint32_t) = &ps2_guest_mem::write32;
    void (*ps2gm_fn_write64)(uint8_t *, R5900Context *, PS2Runtime *, uint32_t, uint64_t) = &ps2_guest_mem::write64;
#endif
}

#if PS2X_GUEST_MEM_TRAMP
#define PS2GM_LOAD(reg, sym) "movw " #reg ", #:lower16:" #sym "\n\tmovt " #reg ", #:upper16:" #sym "\n\tldr " #reg ", [" #reg "]\n\t"
#define PS2GM_TRAMP_HEAD(name)                                     \
    ".pushsection .text.ps2gm_tramp_" #name ",\"ax\",%progbits\n\t" \
    ".syntax unified\n\t.thumb\n\t.align 2\n\t"                   \
    ".global ps2gm_tramp_" #name "\n\t"                            \
    ".type ps2gm_tramp_" #name ", %function\n\t"                   \
    ".thumb_func\n"                                                \
    "ps2gm_tramp_" #name ":\n\t"                                   \
    ".fnstart\n\t"
#define PS2GM_TRAMP_TAIL(name) ".fnend\n\t.size ps2gm_tramp_" #name ", .-ps2gm_tramp_" #name "\n\t.popsection\n"
// The caller's sp may be 4 mod 8 (the call is an asm statement, so GCC sees
// a leaf function): r4 keeps it, sp is rounded down to 8 for the C call
// (.setfp tells the unwinder where the core registers are).
#define PS2GM_ALIGN_SAVE_VFP "mov r4, sp\n\t.setfp r4, sp\n\tbic ip, r4, #7\n\tmov sp, ip\n\t" \
    "vpush {d0-d7}\n\t.vsave {d0-d7}\n\tvpush {d16-d31}\n\t.vsave {d16-d31}\n\t"
#define PS2GM_RESTORE_VFP "vpop {d16-d31}\n\tvpop {d0-d7}\n\tmov sp, r4\n\t"
// in: r0 = address, r1 = ctx; out: r0. Keeps r1-r3, ip and the VFP registers.
#define PS2GM_TRAMP_READ(name)                                              \
    asm(PS2GM_TRAMP_HEAD(name) "push {r1, r2, r3, r4, ip, lr}\n\t.save {r1, r2, r3, r4, ip, lr}\n\t" \
        PS2GM_ALIGN_SAVE_VFP "mov r3, r0\n\t" PS2GM_LOAD(r0, g_ps2gmRdram) PS2GM_LOAD(r2, g_ps2gmRuntime) \
        PS2GM_LOAD(ip, ps2gm_fn_##name) "blx ip\n\t" PS2GM_RESTORE_VFP "pop {r1, r2, r3, r4, ip, pc}\n\t" PS2GM_TRAMP_TAIL(name));
// in: r0 = address, r1 = ctx, r2 / r3 = the value (low / high). Keeps r0-r3, ip and the VFP registers.
#define PS2GM_TRAMP_WRITE(name)                                             \
    asm(PS2GM_TRAMP_HEAD(name) "push {r0, r1, r2, r3, r4, ip, lr}\n\t.save {r0, r1, r2, r3, r4, ip, lr}\n\t" \
        PS2GM_ALIGN_SAVE_VFP "sub sp, sp, #8\n\t.pad #8\n\tstrd r2, r3, [sp]\n\tmov r3, r0\n\t" \
        PS2GM_LOAD(r0, g_ps2gmRdram) PS2GM_LOAD(r2, g_ps2gmRuntime) PS2GM_LOAD(ip, ps2gm_fn_##name) \
        "blx ip\n\tadd sp, sp, #8\n\t" PS2GM_RESTORE_VFP "pop {r0, r1, r2, r3, r4, ip, pc}\n\t" PS2GM_TRAMP_TAIL(name));
PS2GM_TRAMP_READ(read8)
PS2GM_TRAMP_READ(read16)
PS2GM_TRAMP_READ(read32)
// in: r0 = address, r1 = ctx; out: r0 / r1. Keeps r2, r3, ip and the VFP registers.
asm(PS2GM_TRAMP_HEAD(read64) "push {r2, r3, r4, ip, lr}\n\t.save {r2, r3, r4, ip, lr}\n\t" PS2GM_ALIGN_SAVE_VFP
    "mov r3, r0\n\t" PS2GM_LOAD(r0, g_ps2gmRdram) PS2GM_LOAD(r2, g_ps2gmRuntime) PS2GM_LOAD(ip, ps2gm_fn_read64)
    "blx ip\n\t" PS2GM_RESTORE_VFP "pop {r2, r3, r4, ip, pc}\n\t" PS2GM_TRAMP_TAIL(read64));
PS2GM_TRAMP_WRITE(write8)
PS2GM_TRAMP_WRITE(write16)
PS2GM_TRAMP_WRITE(write32)
PS2GM_TRAMP_WRITE(write64)
#endif
