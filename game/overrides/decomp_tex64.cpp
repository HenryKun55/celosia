// Round 64: texReset's texture loop on the offload thread (SOTC_TEX64=0 off;
// A/B `tex64`).
//
// What moves. graphFrameInit (the frame's first graphics task) calls texReset
// (decomp_texframe.cpp), whose loop walks the texture list (*0x118c184, ~260
// nodes in the title screen's world) and, per node, clears the texture state
// record's transfer words (s + 0x10..0x1c) and runs texAnim on the texture
// (the scroll offsets s + 0..0xb, the palette in use s + 0xc, the time and
// phase words of the animation record, the animated palette). ~1.5% of the
// game thread in Vita3K's title-screen world (texAnimFast 1.03%, the loop
// 0.37%): four dependent loads a node and the palette copies.
//
// Who reads it, and when (the census, decomp_readcensus.cpp,
// SOTC_READ_CENSUS=tex on the Mac: every load of the generated code from the
// bytes the loop writes, from texReset's return to its next call, 9 000
// frames of the attract demo): texTransRequest, texResetVRAMRequest,
// texTransResolve (its records: decomp_hot46f), texInitialize / texDelete,
// and at a load the module loader (RelocateElfInfoHeader, DecodeSection,
// isysMoveModuleObj). The first read comes 0.4-0.8 ms after texReset on the
// Mac (median ~0.7 ms), at the display walk's first sprite: between them the
// frame's control / AI, camera, wind and particle tasks (the timeline,
// SOTC_SUBSYS_PROF_TIMELINE). The natives that read the state with plain
// loads (the model hooks' scroll rows, the texture uploads) run inside the
// display walk or the hooks.
//
// How. texReset, after its guest calls (the camera, vramReset), hands the
// loop to the offload thread (threads::kOffload, cores 1-2 on the Vita) with
// a copy of the context, and returns with a0 / v1 / v0 the last node's as the
// loop leaves them. The thread runs the same loop: the clears, then
// texAnimFast / texAnimExact on its own context, iosGetSinf / iosGetCosf
// called straight (their natives are pure). A node whose texAnim would go to
// the recompiled code (pointers outside plain RAM: never seen) stops it; the
// game thread runs the rest at the join through the table, as before, on a
// saved context. Joins (the thread's work done) before every reader above
// (their table entries wrapped), the display walk (DispDispObjGroupModel),
// spriteTexFast, modelDisp, meshDisp, iosDlTrans (the DMA kick: the palettes
// go out with the display list), texReset itself, and every
// hook worker join (sotc_native: the frame's end, the heap's moves, the 2D,
// mesh and particle hooks).
//
// Not as the original: the scratch registers texAnim leaves (v0, f0-f5, f12,
// fcr31's condition bit, at / a3 / t0, VU0 vf15-vf19 after a wave) are those
// of the context texReset was entered with. They are dead there: texReset's
// caller (graphFrameInit, native) calls glReset next, which writes f0 and its
// callee setLight v0, at, a3 and t0 before reading them, and neither reads
// the condition bit or vf15-vf19 (iosMakeNormalLightMatrix writes vf15 / vf16
// first).
//
// SOTC_TEX64_VERIFY=1: the loop runs on the game thread as before (the
// truth); the thread runs it on a copy of all of RAM taken at the hand-over;
// at the join the bytes the loop writes are compared (copy against RAM), and
// the bytes it reads (each node's records, the source palette) against the
// copy (changed in flight); on the Mac every load of the generated code from
// the written bytes between the hand-over and the join is counted with its
// reader (a missing join site). "[tex64] verify:" every 600 frames.
#include "sotc_decomp.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_thread_load.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#if !defined(__vita__)
#include <dlfcn.h>
#endif

namespace decomp_texanim
{
    extern thread_local PS2Runtime::RecompiledFunction t_trig[2];
    extern thread_local bool t_offThread, t_bail;
    void texAnimFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void texAnimExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void texAnim(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_texframe
{
    void texReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_tex64
{
    constexpr uint32_t kTexReset = 0x010AB2A0u, kTexAnim = 0x010A9EA8u, kSinf = 0x001BDF00u, kCosf = 0x001BE020u;
    constexpr uint32_t kHead = 0x0118C184u, kAnimRa = 0x010AB380u;
    constexpr uint32_t kSizes = 0x01192280u, kWorkBufId = 0x002132F0u;
    constexpr uint32_t kTrigSites[6] = {0x010A9F70u, 0x010A9FFCu, 0x010AA01Cu, 0x010AA0B8u, 0x010AA144u, 0x010AA164u};

    inline uint32_t u32(const uint8_t *rdram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline void clearRec(uint8_t *rdram, uint32_t rec)
    {
        if (__builtin_expect(rec <= PS2_RAM_SIZE - 0x20u, 1))
            std::memset(rdram + rec + 0x10u, 0, 16);
        else
            for (uint32_t o : {0x18u, 0x14u, 0x10u, 0x1Cu})
                std::memset(rdram + ((rec + o) & PS2_RAM_MASK), 0, 4);
    }

    // ---- state ---------------------------------------------------------------
    struct Job
    {
        uint8_t *rdram = nullptr; // RAM (or the verify copy)
        PS2Runtime *runtime = nullptr;
        R5900Context ctx;
        float f12 = 0.0f;
        uint32_t frame = 0;                       // texReset's frame (sp - 0x20): the calls' sp
        PS2Runtime::RecompiledFunction trig[2] = {};
        bool fast = true;
        uint32_t bailNode = 0; // the node the thread stopped at (its record cleared, texAnim not run)
        uint32_t nodes = 0;
        uint64_t us = 0;
    };
    Job g_job;
    std::mutex g_mutex;
    std::condition_variable g_work, g_idle;
    std::atomic<uint64_t> g_posted{0}, g_done{0};
    std::atomic<bool> g_waiting{false};
    bool g_started = false, g_pending = false;
    int g_verify = -1;
    uint64_t g_frame0 = 0;
    // Stats (the game thread's).
    uint64_t g_jobs = 0, g_offNodes = 0, g_bails = 0, g_waits = 0, g_waitUs = 0, g_workUs = 0, g_refused = 0;
    std::unordered_map<int, uint64_t> g_joinSites;

    bool verifyOn()
    {
        if (__builtin_expect(g_verify < 0, 0))
            g_verify = std::getenv("SOTC_TEX64_VERIFY") ? 1 : 0;
        return g_verify != 0;
    }

    void runLoop(Job &j)
    {
        const auto t0 = std::chrono::steady_clock::now();
        uint8_t *const rdram = j.rdram;
        R5900Context *const ctx = &j.ctx;
        decomp_texanim::t_offThread = true;
        decomp_texanim::t_trig[0] = j.trig[0];
        decomp_texanim::t_trig[1] = j.trig[1];
        j.bailNode = 0;
        j.nodes = 0;
        for (uint32_t node = u32(rdram, kHead); node; node = u32(rdram, node + 0xCu))
        {
            const uint32_t tex = u32(rdram, u32(rdram, node + 4u) + 0x28u), mid = u32(rdram, tex + 0x20u), rec = u32(rdram, mid + 0xCu);
            clearRec(rdram, rec);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
            ctx->f[12] = j.f12;
            SET_GPR_U32(ctx, 31, kAnimRa);
            SET_GPR_U32(ctx, 29, j.frame);
            ctx->pc = kTexAnim;
            decomp_texanim::t_bail = false;
            if (j.fast)
                decomp_texanim::texAnimFast(rdram, ctx, j.runtime);
            else
                decomp_texanim::texAnimExact(rdram, ctx, j.runtime);
            if (decomp_texanim::t_bail)
            {
                j.bailNode = node;
                break;
            }
            ++j.nodes;
        }
        decomp_texanim::t_offThread = false;
        j.us = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
    }

    void loop()
    {
        native_renderer::threads::place(native_renderer::threads::kOffload);
        ps2SetEeFloatingPointMode(); // the game thread's rounding (toward zero) and flush-to-zero
        uint64_t next = 0;
        for (;;)
        {
            {
                std::unique_lock<std::mutex> lock(g_mutex);
                g_work.wait(lock, [&] { return g_posted.load(std::memory_order_acquire) > next; });
            }
            next = g_posted.load(std::memory_order_acquire);
            runLoop(g_job);
            g_done.store(next, std::memory_order_release);
            if (g_waiting.load(std::memory_order_seq_cst))
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                g_idle.notify_all();
            }
        }
    }

    // ---- verify ----------------------------------------------------------------
    struct Region
    {
        uint32_t at, bytes;
        bool written; // the loop writes it (else read only)
    };
    std::vector<uint8_t> g_copy; // all of RAM at the hand-over (the thread's)
    std::vector<Region> g_regions;
    uint64_t g_vChecks = 0, g_vDiffer = 0, g_vTouched = 0, g_vReads = 0, g_vReadWindows = 0;
    std::vector<uint8_t> g_truth; // the written bytes as the loop left them (in g_regions' order)
    uint64_t g_vLater = 0;        // written bytes changed by the guest between the loop and the join
#if !defined(__vita__)
    std::vector<uint8_t> g_map;
    std::vector<uint32_t> g_marked;
    std::unordered_map<uintptr_t, uint64_t> g_readers;
    bool g_readInWindow = false;
    void readHook(uint32_t, uint32_t, const void *pc)
    {
        ++g_vReads;
        ++g_readers[reinterpret_cast<uintptr_t>(pc)];
        g_readInWindow = true;
    }
#endif
    bool okRange(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    // The regions of every node (as texAnim's checks would find them), from RAM now.
    void collectRegions(const uint8_t *rdram)
    {
        g_regions.clear();
        const uint32_t id = u32(rdram, kWorkBufId);
        uint32_t count = 0;
        for (uint32_t node = u32(rdram, kHead); node && okRange(node, 0x10u) && ++count < (1u << 16); node = u32(rdram, node + 0xCu))
        {
            const uint32_t tex = u32(rdram, u32(rdram, node + 4u) + 0x28u);
            if (!okRange(tex, 0x24u))
                continue;
            const uint32_t s3 = u32(rdram, tex + 0x20u);
            if (!okRange(s3, 0xA0u))
                continue;
            const uint32_t s1 = u32(rdram, s3 + 0xCu);
            // Written: the animation record's +0x40..+0x47 and +0x7c (r = s3 +
            // 0x20), the state record's +0x0..+0x1f, the destination palette;
            // read: the rest of both records, the source palette.
            g_regions.push_back({s3, 0x60u, false});
            g_regions.push_back({s3 + 0x60u, 8u, true});
            g_regions.push_back({s3 + 0x68u, 0x34u, false});
            g_regions.push_back({s3 + 0x9Cu, 4u, true});
            if (!okRange(s1, 0x60u))
                continue;
            g_regions.push_back({s1, 0x20u, true});
            g_regions.push_back({s1 + 0x20u, 0x40u, false});
            const uint32_t format = u32(rdram, s3 + 0x1Cu) & 0x3Fu;
            const uint32_t size = u32(rdram, kSizes + format * 4u);
            const bool csm = format == 0x13u || format == 0x1Bu;
            const uint32_t bytes = 4u * ((csm && size < 256u) ? 256u : size);
            if (id < 4u && size > 0u && size <= 1024u)
            {
                const uint32_t src = u32(rdram, s1 + 0x40u), dst = u32(rdram, s1 + 0x44u + id * 4u);
                if (okRange(src, bytes))
                    g_regions.push_back({src, bytes, false});
                if (okRange(dst, bytes))
                    g_regions.push_back({dst, bytes, true});
            }
        }
    }

    // ---- the game thread -------------------------------------------------------
    void ensureThread()
    {
        if (g_started)
            return;
        g_started = true;
        std::thread(loop).detach();
    }

    void report()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_frame0 + 600u)
            return;
        const double n = static_cast<double>(frame - g_frame0);
        std::string sites;
        for (const auto &[site, c] : g_joinSites)
        {
            char b[48];
            std::snprintf(b, sizeof(b), " %d:%llu", site, (unsigned long long)c);
            sites += b;
        }
        std::fprintf(stderr, "[tex64] frame %llu: %llu jobs (%.0f nodes each, %.0f us on the thread), %llu refused, %llu stopped early; joins waited %llu (%.3f ms a frame); joined at%s\n",
                     (unsigned long long)frame, (unsigned long long)g_jobs, g_jobs ? static_cast<double>(g_offNodes) / g_jobs : 0.0,
                     g_jobs ? static_cast<double>(g_workUs) / g_jobs : 0.0, (unsigned long long)g_refused, (unsigned long long)g_bails, (unsigned long long)g_waits,
                     g_waitUs / 1000.0 / n, sites.c_str());
        if (verifyOn())
        {
            std::fprintf(stderr, "[tex64] verify: %llu jobs compared, %llu differ, %llu touched in flight, %llu written again before the join, %llu loads of written bytes in flight (%llu windows)\n",
                         (unsigned long long)g_vChecks, (unsigned long long)g_vDiffer, (unsigned long long)g_vTouched, (unsigned long long)g_vLater, (unsigned long long)g_vReads,
                         (unsigned long long)g_vReadWindows);
#if !defined(__vita__)
            for (const auto &[pc, c] : g_readers)
            {
                Dl_info info{};
                const char *name = dladdr(reinterpret_cast<void *>(pc), &info) && info.dli_sname ? info.dli_sname : "?";
                std::fprintf(stderr, "[tex64]   read in flight by %s (%llu loads)\n", name, (unsigned long long)c);
            }
            g_readers.clear();
#endif
        }
        g_joinSites.clear();
        g_jobs = g_offNodes = g_bails = g_waits = g_waitUs = g_workUs = g_refused = 0;
        g_frame0 = frame;
    }

    // The rest of the loop from the node the thread stopped at, through the
    // table as texReset did (the context saved and put back).
    void runRest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t from)
    {
        const R5900Context saved = *ctx;
        const uint32_t frame = GPR_U32(ctx, 29) - 0x100u; // below anything live
        bool first = true;
        for (uint32_t node = from; node; node = u32(rdram, node + 0xCu))
        {
            const uint32_t tex = u32(rdram, u32(rdram, node + 4u) + 0x28u), mid = u32(rdram, tex + 0x20u), rec = u32(rdram, mid + 0xCu);
            if (!first)
                clearRec(rdram, rec);
            first = false;
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
            ctx->f[12] = g_job.f12;
            sotc_decomp::callGuest(rdram, ctx, runtime, kTexAnim, kAnimRa, frame);
        }
        *ctx = saved;
    }

    R5900Context *g_ctx = nullptr;
    PS2Runtime *g_runtime = nullptr;
    uint8_t *g_rdram = nullptr;

    void join(int site)
    {
        if (!g_pending)
            return;
        g_pending = false;
        ++g_joinSites[site];
        const uint64_t seq = g_posted.load(std::memory_order_relaxed);
        if (g_done.load(std::memory_order_acquire) < seq)
        {
            const auto t0 = std::chrono::steady_clock::now();
            ps2_load::Wait waiting(ps2_load::kSiteTex);
            std::unique_lock<std::mutex> lock(g_mutex);
            g_waiting.store(true, std::memory_order_seq_cst);
            while (g_done.load(std::memory_order_acquire) < seq)
                g_idle.wait_for(lock, std::chrono::microseconds(200));
            g_waiting.store(false, std::memory_order_relaxed);
            ++g_waits;
            g_waitUs += static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
        }
        g_offNodes += g_job.nodes;
        g_workUs += g_job.us;
        if (verifyOn())
        {
#if !defined(__vita__)
            g_ps2ReadWatchMap = nullptr;
            for (uint32_t line : g_marked)
                g_map[line] = 0u;
            g_marked.clear();
            g_vReadWindows += g_readInWindow;
            g_readInWindow = false;
#endif
            ++g_vChecks;
            bool differ = false, touched = false, later = false;
            size_t at = 0;
            for (const Region &r : g_regions)
            {
                if (r.written)
                {
                    // the thread's result against the loop's (the truth), and
                    // the truth against RAM now (a guest store before the join)
                    const uint8_t *truth = g_truth.data() + at;
                    at += r.bytes;
                    if (std::memcmp(g_copy.data() + r.at, truth, r.bytes) != 0)
                    {
                        differ = true;
                        if (g_vDiffer < 4u)
                            for (uint32_t k = 0; k < r.bytes; ++k)
                                if (g_copy[r.at + k] != truth[k])
                                {
                                    std::fprintf(stderr, "[tex64] verify: frame %llu: %08x (+%u of %u) thread %02x loop %02x\n", (unsigned long long)sotc_decomp::g_gameFrame,
                                                 r.at + k, k, r.bytes, g_copy[r.at + k], truth[k]);
                                    break;
                                }
                    }
                    if (std::memcmp(truth, g_rdram + r.at, r.bytes) != 0)
                        later = true;
                }
                else if (std::memcmp(g_copy.data() + r.at, g_rdram + r.at, r.bytes) != 0)
                    touched = true; // a byte it reads changed since the hand-over
            }
            g_vDiffer += differ;
            g_vTouched += touched;
            g_vLater += later;
        }
        else if (g_job.bailNode)
        {
            ++g_bails;
            runRest(g_rdram, g_ctx, g_runtime, g_job.bailNode);
        }
        report();
    }

    // Called by texResetFast before its loop (the camera, the slots and
    // vramReset done). True: the loop is the thread's.
    bool post(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float f12, uint32_t frame)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kTex64))
            return false;
        join(0);
        // The loop's callees as the thread runs them: texAnim's native in the
        // table (no difftest checker, profiler wrapper, SOTC_DECOMP_SKIP), and
        // the trig natives for its waves.
        static PS2Runtime::RecompiledFunction nativeSin = nullptr, nativeCos = nullptr;
        static bool sitesOk = false, looked = false;
        if (!looked)
        {
            looked = true;
            for (const sotc_decomp::Function &f : sotc_decomp::registry())
            {
                if (f.address == kSinf)
                    nativeSin = f.native;
                if (f.address == kCosf)
                    nativeCos = f.native;
            }
            sitesOk = true;
            for (int i = 0; i < 6; ++i)
                sitesOk = sitesOk && sotc_decomp::jalTarget(rdram, kTrigSites[i]) == (i < 3 ? kCosf : kSinf);
        }
        if (!sitesOk || !nativeSin || !nativeCos || sotc_decomp::guestFunction(runtime, kTexAnim) != &decomp_texanim::texAnim ||
            sotc_decomp::guestFunction(runtime, kSinf) != nativeSin || sotc_decomp::guestFunction(runtime, kCosf) != nativeCos ||
            sotc_decomp::guestFunction(runtime, kTexReset) != &decomp_texframe::texReset)
        {
            ++g_refused;
            return false;
        }
#if !defined(__vita__)
        if (sotc_decomp::g_callGuestStats)
            return false;
#endif
        ensureThread();
        g_rdram = rdram;
        g_ctx = ctx;
        g_runtime = runtime;
        Job &j = g_job;
        j.ctx = *ctx;
        j.runtime = runtime;
        j.f12 = f12;
        j.frame = frame;
        j.trig[0] = nativeSin;
        j.trig[1] = nativeCos;
        j.fast = sotc_decomp::fastNativeOn();
        j.rdram = rdram;
        ++g_jobs;
        if (verifyOn())
        {
            // The thread on a copy of RAM; the loop for real here (below, by the caller).
            g_copy.resize(PS2_RAM_SIZE + 64u);
            std::memcpy(g_copy.data(), rdram, PS2_RAM_SIZE);
            j.rdram = g_copy.data();
            collectRegions(rdram);
        }
        g_posted.fetch_add(1, std::memory_order_release);
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_work.notify_one();
        }
        g_pending = true;
        return !verifyOn(); // verify: the caller runs its loop (the truth)
    }

    // Verify only: after the caller's loop, the written bytes watched until the join.
    void armWatch(uint8_t *rdram)
    {
        if (!verifyOn() || !g_pending)
            return;
        g_truth.clear();
        for (const Region &r : g_regions)
            if (r.written)
                g_truth.insert(g_truth.end(), rdram + r.at, rdram + r.at + r.bytes);
#if !defined(__vita__)
        if (g_map.empty())
            g_map.assign(PS2_RAM_SIZE >> 4, 0u);
        for (const Region &r : g_regions)
            if (r.written)
                for (uint32_t line = r.at >> 4; line <= (r.at + r.bytes - 1u) >> 4; ++line)
                    if (!g_map[line])
                    {
                        g_map[line] = 1u;
                        g_marked.push_back(line);
                    }
        g_ps2ReadWatchHook = readHook;
        g_ps2ReadWatchMap = g_map.data();
#endif
    }

    // ---- the join sites ----------------------------------------------------
    struct Site
    {
        uint32_t address;
        const char *name;
    };
    constexpr Site kSites[] = {
        {0x010AAD48u, "texTransRequest"}, {0x010AAFA8u, "texResetVRAMRequest"}, {0x010A54F0u, "texTransResolve"},
        {0x010AB0F8u, "texInitialize"},   {0x010AB190u, "texDelete"},           {0x010AB3A8u, "texSearchMopByName"},
        {0x010AB618u, "texRecordReset"},  {0x001004B8u, "RelocateElfInfoHeader"}, {0x00100278u, "DecodeSection"},
        {0x001B4C20u, "isysMoveModuleObj"}, {0x010F52C8u, "DispDispObjGroupModel"}, {0x001C5548u, "spriteTexFast"},
        {0x01097118u, "modelDisp"},       {0x0109AB80u, "meshDisp"},            {0x001C0058u, "iosDlTrans"},
    };
    constexpr int kSiteCount = sizeof(kSites) / sizeof(kSites[0]);
    PS2Runtime::RecompiledFunction g_orig[kSiteCount] = {};
    template <int I>
    void wrap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        join(I + 1);
        g_orig[I](rdram, ctx, runtime);
    }
    template <int... I>
    void installAll(PS2Runtime &runtime, std::integer_sequence<int, I...>)
    {
        ((runtime.hasFunction(kSites[I].address) ? (g_orig[I] = runtime.lookupFunction(kSites[I].address), runtime.registerFunction(kSites[I].address, wrap<I>), 0) : 0),
         ...);
    }
    void install(PS2Runtime &runtime)
    {
        installAll(runtime, std::make_integer_sequence<int, kSiteCount>{});
        sotc_decomp::g_offloadJoin = join;
    }
}
