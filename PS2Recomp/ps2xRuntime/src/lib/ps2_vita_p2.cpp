// Round P2 (docs/decomp_p2.md): which code of the game thread takes the cache misses / stall cycles, on the console.
// Off by default; needs SOTC_K2=1 (the resident module) and its counters on the game thread (K2's first frame).
//
//   SOTC_P2PMUSAMPLE=<event>[:<period>]   PMU overflow sampling. <event>: 0x03 / dmiss (L1 D refill), 0x01 / imiss (L1 I
//        refill), 0x61 / dstall (D-side stall cycles), 0x60 / istall (I-side stall cycles), or any number. Default
//        periods give ~500 samples a riding frame (dmiss 250, imiss 1000, dstall 10000, istall 8000). The game loads the
//        P2 bridge (module/celosia_p2u.suprx), K2's module probes the thread context layout, the GIC distributor and the
//        PMU's interrupt line on core 0 (with interrupts off: a counter made to overflow), registers its handler on that
//        line and reloads K2's counter 3 (M1's "dtlb miss" slot) with -period: every overflow records the interrupted
//        pc + the user lr of the game thread. The game drains them every frame into a pc table (16-byte buckets) and a
//        (pc bucket, lr) table; every SOTC_P2PMUSAMPLE_WINDOW frames (600) it prints them and starts again:
//          [p2] w <k> frames a-b (...): samples ..., kernel ..., irqs ..., other ..., mismatch ..., dropped ...
//          [p2] h <pc>:<n> ...          pc buckets (absolute; '[p2] anchor' gives the load slide)
//          [p2] c <pc>:<lr>:<n> ...     pc bucket + caller (lr), n >= 2
//        tools/p2_report.py err.txt --elf <elf> ranks functions / subsystems from them.
//   SOTC_P2BRACKET=1   counter brackets: a set of top-level guest functions per subsystem (game/overrides/sotc_p2.cpp,
//        + SOTC_P2BRACKET_EXTRA=0xaddr:name,...) wrapped in the function table; on entry and exit the game thread's six
//        counters + cycles are read through K2 (a syscall each); inclusive and self per frame every window:
//          [p2b] w <k> frames a-b: <name> calls/f C | incl e0 e1 e2 e3 e4 e5 cyc | self ...
//        The read's own cost (calibrated at start: "[p2b] read cost") is in every pair; p2_report.py subtracts it.
// Vita3K never runs the module: K2 is off there and both stay off (one getenv at the first frame).
#if defined(PLATFORM_VITA)

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/threadmgr.h>
#include "../../vita/k2/celosia_k2.h"
#include "../../vita/k2/celosia_p2.h"

namespace k1
{
    extern uint32_t g_events, g_eventsHi;
}
namespace k2
{
    extern bool g_ok;
    extern K2Bridge g_br;
}

extern "C" int ps2x_p2_skip = -1; // k2::tick leaves this counter out of the [m1pmu] sums (it is the sampler's)

namespace p2
{
    __attribute__((noinline, used)) void anchor() { asm volatile(""); }

    int g_state = 0; // 0 not asked, 1 prepared, 2 sampling, 3 stopped, -1 off
    uint32_t g_event = 0, g_period = 0, g_counter = P2_COUNTER_DEFAULT, g_window = 600, g_prio = 0;
    P2Bridge g_br;
    int g_bridge = 0, g_bridgeStatus = -1;
    P2Probe g_probe;
    P2Stat g_st, g_stPrev;
    P2Sample *g_buf = nullptr;

    struct Ent
    {
        uint32_t key, n;
    };
    struct Pair
    {
        uint32_t pc, lr, n;
    };
    constexpr uint32_t kPcCap = 1u << 16, kPairCap = 1u << 15;
    Ent *g_pc = nullptr;
    Pair *g_pair = nullptr;
    uint32_t g_pcUsed = 0, g_pairUsed = 0, g_pairLost = 0;
    uint64_t g_samples = 0, g_kernel = 0, g_outside = 0;
    unsigned long long g_f0 = 0;
    uint64_t g_t0 = 0;
    unsigned g_win = 0, g_drains = 0;
    uint32_t g_textLo = 0, g_textHi = 0;

    const char *eventName(uint32_t e)
    {
        switch (e)
        {
        case 0x01: return "icache miss";
        case 0x03: return "dcache miss";
        case 0x60: return "icache stall cyc";
        case 0x61: return "dcache stall cyc";
        case 0x11: return "cycles";
        case 0x68: return "instructions";
        default: return "event";
        }
    }
    bool parse(const char *v)
    {
        char name[16] = {};
        int i = 0;
        for (; v[i] && v[i] != ':' && i < 15; ++i)
            name[i] = v[i];
        name[i] = 0;
        uint32_t ev = 0, per = 0;
        if (!std::strcmp(name, "dmiss"))
            ev = 0x03;
        else if (!std::strcmp(name, "imiss"))
            ev = 0x01;
        else if (!std::strcmp(name, "dstall"))
            ev = 0x61;
        else if (!std::strcmp(name, "istall"))
            ev = 0x60;
        else
            ev = static_cast<uint32_t>(std::strtoul(name, nullptr, 0));
        if (v[i] == ':')
            per = static_cast<uint32_t>(std::strtoul(v + i + 1, nullptr, 0));
        if (!per)
            per = ev == 0x03 ? 250u : ev == 0x01 ? 1000u : ev == 0x61 ? 10000u : ev == 0x60 ? 8000u : 10000u;
        if (ev == 0 || ev > 0xFF || per < 64u)
            return false;
        g_event = ev;
        g_period = per;
        return true;
    }

    void addPc(uint32_t key)
    {
        uint32_t h = (key >> 4) * 2654435761u;
        for (uint32_t i = 0; i < kPcCap; ++i)
        {
            Ent &e = g_pc[(h + i) & (kPcCap - 1u)];
            if (e.n == 0u)
            {
                e.key = key;
                e.n = 1u;
                ++g_pcUsed;
                return;
            }
            if (e.key == key)
            {
                ++e.n;
                return;
            }
        }
    }
    void addPair(uint32_t pc, uint32_t lr)
    {
        uint32_t h = ((pc >> 4) * 2654435761u) ^ (lr * 40503u);
        for (uint32_t i = 0; i < 64u; ++i)
        {
            Pair &e = g_pair[(h + i) & (kPairCap - 1u)];
            if (e.n == 0u)
            {
                if (g_pairUsed * 4u > kPairCap * 3u)
                    break;
                e.pc = pc;
                e.lr = lr;
                e.n = 1u;
                ++g_pairUsed;
                return;
            }
            if (e.pc == pc && e.lr == lr)
            {
                ++e.n;
                return;
            }
        }
        ++g_pairLost;
    }

    void stLine(const char *tag, const P2Stat &s)
    {
        std::fprintf(stderr, "[p2] %s: rc %d active %u line %u event 0x%02x period %u counter %u cpu %u stop %u | irqs %u samples %u kernel %u other %u "
                             "mismatch %u spurious %u dropped %u badtype %u | register 0x%08x enable 0x%08x disable 0x%08x release 0x%08x | irq sp %08x last pc %08x cpsr %08x thread %08x\n",
                     tag, static_cast<int>(s.rc), s.active, s.line, s.event, s.period, s.counter, s.cpu, s.stop_reason, s.irqs, s.samples, s.kernel, s.other, s.mismatch,
                     s.spurious, s.dropped, s.badtype, s.register_rc, s.enable_rc, s.disable_rc, s.release_rc, s.irq_sp, s.last_pc, s.last_cpsr, s.last_thread);
    }

    void flush(unsigned long long frame)
    {
        const uint64_t t = sceKernelGetProcessTimeWide();
        const double nf = frame > g_f0 ? static_cast<double>(frame - g_f0) : 1.0;
        const P2Stat &s = g_st, &p = g_stPrev;
        std::fprintf(stderr, "[p2] w %u frames %llu-%llu (%.2f ms/frame wall): event 0x%02x (%s) period %u: samples %llu (= %.0f events/frame), kernel %llu, outside text %llu, "
                             "irqs %u, other thread %u, mismatch %u, spurious %u, dropped %u, badtype %u, drains %u, pc buckets %u, pairs %u (lost %u)%s\n",
                     g_win, g_f0, frame, static_cast<double>(t - g_t0) / 1000.0 / nf, g_event, eventName(g_event), g_period,
                     static_cast<unsigned long long>(g_samples), static_cast<double>(g_samples) * g_period / nf, static_cast<unsigned long long>(g_kernel),
                     static_cast<unsigned long long>(g_outside), s.irqs - p.irqs, s.other - p.other, s.mismatch - p.mismatch, s.spurious - p.spurious,
                     s.dropped - p.dropped, s.badtype - p.badtype, g_drains, g_pcUsed, g_pairUsed, g_pairLost, s.active ? "" : " (sampling stopped)");
        // pc buckets, most first
        Ent *v = g_pc;
        uint32_t n = 0;
        for (uint32_t i = 0; i < kPcCap; ++i)
            if (g_pc[i].n)
                v[n++] = g_pc[i]; // compacted in place (n <= i)
        std::sort(v, v + n, [](const Ent &a, const Ent &b) { return a.n > b.n; });
        char line[1100];
        int len = 0;
        for (uint32_t i = 0; i < n; ++i)
        {
            if (len == 0)
                len = std::snprintf(line, sizeof(line), "[p2] h");
            len += std::snprintf(line + len, sizeof(line) - len, " %x:%u", v[i].key, v[i].n);
            if (len > 960 || i + 1 == n)
            {
                std::fprintf(stderr, "%s\n", line);
                len = 0;
            }
        }
        std::memset(g_pc, 0, sizeof(Ent) * kPcCap);
        // pairs seen twice or more
        Pair *q = g_pair;
        uint32_t m = 0;
        for (uint32_t i = 0; i < kPairCap; ++i)
            if (g_pair[i].n >= 2u)
                q[m++] = g_pair[i];
        std::sort(q, q + m, [](const Pair &a, const Pair &b) { return a.n > b.n; });
        len = 0;
        for (uint32_t i = 0; i < m; ++i)
        {
            if (len == 0)
                len = std::snprintf(line, sizeof(line), "[p2] c");
            len += std::snprintf(line + len, sizeof(line) - len, " %x:%x:%u", q[i].pc, q[i].lr, q[i].n);
            if (len > 960 || i + 1 == m)
            {
                std::fprintf(stderr, "%s\n", line);
                len = 0;
            }
        }
        std::memset(g_pair, 0, sizeof(Pair) * kPairCap);
        g_pcUsed = g_pairUsed = g_pairLost = 0;
        g_samples = g_kernel = g_outside = 0;
        g_drains = 0;
        g_stPrev = g_st;
        g_t0 = sceKernelGetProcessTimeWide();
        g_f0 = frame;
        ++g_win;
    }

    void drain()
    {
        const int n = g_br.p2(P2_OP_DRAIN, P2_RING, g_buf, &g_st);
        if (n <= 0)
            return;
        ++g_drains;
        for (int i = 0; i < n; ++i)
        {
            const uint32_t pc = g_buf[i].pc, lr = g_buf[i].lr;
            const uint32_t key = pc & ~0xFu;
            g_kernel += pc & 1u;
            g_outside += (pc < g_textLo || pc >= g_textHi) ? 1u : 0u;
            addPc(key);
            addPair(key, lr);
        }
        g_samples += static_cast<uint64_t>(n);
    }
}

namespace p2b
{
    bool g_want = false, g_on = false;
    void calibrate();
    void tick(unsigned long long frame);
}

// m1pmu::start, before K2's first frame arms the counters: the sampled event goes into counter 3.
extern "C" void ps2x_p2_prepare()
{
    using namespace p2;
    const char *v = std::getenv("SOTC_P2PMUSAMPLE");
    const char *b = std::getenv("SOTC_P2BRACKET");
    p2b::g_want = b && std::atoi(b) != 0;
    if (!v || !*v || !std::strcmp(v, "0"))
    {
        g_state = -1;
        return;
    }
    if (!k2::g_ok)
    {
        std::fprintf(stderr, "[p2] off: K2 is off (SOTC_K2=1 needed; Vita3K never runs the module)\n");
        g_state = -1;
        return;
    }
    if (!parse(v))
    {
        std::fprintf(stderr, "[p2] off: SOTC_P2PMUSAMPLE=%s not understood (<event>[:<period>], period >= 64)\n", v);
        g_state = -1;
        return;
    }
    if (const char *w = std::getenv("SOTC_P2PMUSAMPLE_WINDOW"))
        g_window = std::max(60, std::atoi(w));
    if (const char *p = std::getenv("SOTC_P2PMUSAMPLE_PRIO"))
        g_prio = static_cast<uint32_t>(std::strtoul(p, nullptr, 0)) & 0xF0u;
    k1::g_events = (k1::g_events & ~(0xFFu << (8u * g_counter))) | (g_event << (8u * g_counter));
    g_state = 1;
}

// m1pmu::start, after K2's first frame (k2ok: K2's counters run on this thread).
extern "C" void ps2x_p2_start(unsigned long long frame, bool k2ok)
{
    using namespace p2;
    if (p2b::g_want && k2ok)
    {
        p2b::calibrate();
        p2b::g_on = true;
    }
    else if (p2b::g_want)
        std::fprintf(stderr, "[p2b] off: K2's counters are not on the game thread\n");
    if (g_state != 1)
        return;
    g_state = -1;
    if (!k2ok)
    {
        std::fprintf(stderr, "[p2] off: K2's counters are not on the game thread\n");
        return;
    }
    std::memset(&g_br, 0, sizeof(g_br));
    P2Bridge *bp = &g_br;
    g_bridge = sceKernelLoadStartModule("app0:module/celosia_p2u.suprx", sizeof(bp), &bp, 0, nullptr, &g_bridgeStatus);
    if (g_bridge < 0 || g_br.magic != P2_MAGIC || g_br.version != P2_VERSION || !g_br.p2)
    {
        std::fprintf(stderr, "[p2] off: bridge app0:module/celosia_p2u.suprx 0x%08x status %d (an older K2 module still resident? reboot the console)\n",
                     static_cast<unsigned>(g_bridge), g_bridgeStatus);
        return;
    }
    SceKernelMemBlockInfo info;
    std::memset(&info, 0, sizeof(info));
    info.size = sizeof(info);
    if (sceKernelGetMemBlockInfoByAddr(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(&anchor) & ~static_cast<uintptr_t>(1)), &info) >= 0)
    {
        g_textLo = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(info.mappedBase));
        g_textHi = g_textLo + static_cast<uint32_t>(info.mappedSize);
    }
    // a sleep first: the thread switch saves this thread's context (with the events as armed now) for the probe's checks
    sceKernelDelayThread(1000);
    std::memset(&g_probe, 0, sizeof(g_probe));
    const uint64_t tp = sceKernelGetProcessTimeWide();
    const int prc = g_br.p2(P2_OP_PROBE, g_counter, nullptr, &g_probe);
    const P2Probe &o = g_probe;
    std::fprintf(stderr, "[p2] probe frame %llu (%.1f ms): rc %d ok 0x%03x cpu %u thread %08x ctx %08x | ctx TPIDRURW %08x/%08x TTBR1 %08x/%08x PMCR %08x CNTENSET %08x/%08x "
                         "types %08x.%04x/%08x.%04x | CBAR %08x GICD pa %08x kva %08x PAR %08x IIDR %08x TYPER %08x | trials %u spins %u candidates %u (%u %u %u %u) noise %u | "
                         "line %u (SPI %d) enabled-before %u pending-after %u cfg %u%s\n",
                 frame, static_cast<double>(sceKernelGetProcessTimeWide() - tp) / 1000.0, prc, o.ok, o.cpu, o.thread, o.ctx, o.ctx_urw, o.live_urw, o.ctx_ttbr1,
                 o.live_ttbr1, o.ctx_pmcr, o.ctx_cntenset, o.live_cntenset, o.ctx_types, o.ctx_types_hi, o.live_types, o.live_types_hi, o.cbar, o.gicd_pa, o.gicd_kva,
                 o.gicd_par, o.gicd_iidr, o.gicd_typer, o.trials, o.spins, o.cand_n, o.cand[0], o.cand[1], o.cand[2], o.cand[3], o.noise, o.line,
                 o.line ? static_cast<int>(o.line) - 32 : -1, o.line_enabled, o.line_after, o.line_cfg, (o.ok & P2_OK_LEVEL) ? " (level)" : "");
    if (prc < 0)
    {
        std::fprintf(stderr, "[p2] off: the probe refused (%s)\n", prc == -2 ? "the thread context does not look as expected" : prc == -3 ? "no GIC distributor"
                                                                     : prc == -4 ? "no single PMU interrupt line" : prc == -5 ? "the line is already in use" : "?");
        return;
    }
    g_buf = static_cast<P2Sample *>(std::malloc(sizeof(P2Sample) * P2_RING));
    g_pc = static_cast<Ent *>(std::calloc(kPcCap, sizeof(Ent)));
    g_pair = static_cast<Pair *>(std::calloc(kPairCap, sizeof(Pair)));
    if (!g_buf || !g_pc || !g_pair)
    {
        std::fprintf(stderr, "[p2] off: no memory for the tables\n");
        std::free(g_buf);
        std::free(g_pc);
        std::free(g_pair);
        g_buf = nullptr;
        g_pc = nullptr;
        g_pair = nullptr;
        return;
    }
    P2Cfg cfg;
    std::memset(&cfg, 0, sizeof(cfg));
    cfg.event = g_event;
    cfg.period = g_period;
    cfg.counter = g_counter;
    cfg.prio = g_prio;
    cfg.max_irqs = 0u; // the module's default (250 000 between two drains)
    std::memset(&g_st, 0, sizeof(g_st));
    const int src = g_br.p2(P2_OP_START, 0, &cfg, &g_st);
    stLine("start", g_st);
    std::fprintf(stderr, "[p2] anchor %08x text %08x-%08x: %s every %u events (0x%02x) on counter %u, window %u frames%s\n",
                 static_cast<unsigned>(reinterpret_cast<uintptr_t>(&anchor) & ~1u), g_textLo, g_textHi, eventName(g_event), g_period, g_event, g_counter, g_window,
                 src < 0 ? ": START FAILED, off" : "");
    if (src < 0)
        return;
    ps2x_p2_skip = static_cast<int>(g_counter);
    std::memset(&g_stPrev, 0, sizeof(g_stPrev));
    g_f0 = frame;
    g_t0 = sceKernelGetProcessTimeWide();
    g_state = 2;
}

// Every frame (ps2x_m1_pmu_tick, the game thread).
extern "C" void ps2x_p2_tick(unsigned long long frame)
{
    using namespace p2;
    if (p2b::g_on)
        p2b::tick(frame);
    if (g_state != 2)
        return;
    drain();
    if (!g_st.active)
    {
        stLine("stopped", g_st);
        flush(frame);
        g_state = 3;
        return;
    }
    if (frame - g_f0 >= g_window || g_pcUsed * 4u > kPcCap * 3u)
        flush(frame);
}

// ---------------------------------------------------------------- counter brackets (SOTC_P2BRACKET=1)
namespace p2b
{
    constexpr int kMax = 64, kDepth = 32, kEv = 7; // six counters + cycles
    struct Acc
    {
        const char *name;
        uint64_t calls, early;
        uint64_t incl[kEv], self[kEv];
        bool off;
    };
    Acc g_acc[kMax];
    int g_n = 0;
    struct Open
    {
        int id;
        uint32_t serial;
        uint32_t start[kEv];
        uint64_t child[kEv];
    };
    Open g_stack[kDepth];
    int g_depth = 0;
    uint32_t g_serial = 0;
    double g_cost[kEv];
    unsigned long long g_f0 = 0;
    bool g_started = false;
    unsigned g_win = 0;

    inline void read(uint32_t *v)
    {
        K2Pmu p;
        k2::g_br.pmuRead(&p);
        for (int k = 0; k < 6; ++k)
            v[k] = p.cnt[k];
        v[6] = p.ccnt;
    }
    void calibrate()
    {
        uint32_t a[kEv], b[kEv];
        read(a);
        constexpr int n = 64;
        for (int i = 0; i < n; ++i)
            read(b);
        for (int k = 0; k < kEv; ++k)
            g_cost[k] = static_cast<double>(static_cast<uint32_t>(b[k] - a[k])) / n;
        std::fprintf(stderr, "[p2b] read cost (one K2 read, the events counted by it): %.1f %.1f %.1f %.1f %.1f %.1f cyc %.1f (events %02x %02x %02x %02x %02x %02x)\n",
                     g_cost[0], g_cost[1], g_cost[2], g_cost[3], g_cost[4], g_cost[5], g_cost[6], k1::g_events & 0xFFu, (k1::g_events >> 8) & 0xFFu,
                     (k1::g_events >> 16) & 0xFFu, k1::g_events >> 24, k1::g_eventsHi & 0xFFu, (k1::g_eventsHi >> 8) & 0xFFu);
    }
    void tick(unsigned long long frame)
    {
        if (!g_started)
        {
            g_started = true;
            g_f0 = frame;
            return;
        }
        const unsigned long long window = p2::g_window;
        if (frame - g_f0 < window)
            return;
        const double nf = static_cast<double>(frame - g_f0);
        for (int i = 0; i < g_n; ++i)
        {
            Acc &a = g_acc[i];
            if (!a.calls)
                continue;
            std::fprintf(stderr, "[p2b] w %u frames %llu-%llu: %s calls/f %.2f early %llu | incl %.0f %.0f %.0f %.0f %.0f %.0f %.0f | self %.0f %.0f %.0f %.0f %.0f %.0f %.0f%s\n",
                         g_win, g_f0, frame, a.name, a.calls / nf, static_cast<unsigned long long>(a.early), a.incl[0] / nf, a.incl[1] / nf, a.incl[2] / nf,
                         a.incl[3] / nf, a.incl[4] / nf, a.incl[5] / nf, a.incl[6] / nf, a.self[0] / nf, a.self[1] / nf, a.self[2] / nf, a.self[3] / nf,
                         a.self[4] / nf, a.self[5] / nf, a.self[6] / nf, a.off ? " (off: too many calls)" : "");
            if (a.calls / nf > 600.0)
                a.off = true;
            a.calls = a.early = 0;
            std::memset(a.incl, 0, sizeof(a.incl));
            std::memset(a.self, 0, sizeof(a.self));
        }
        std::fprintf(stderr, "[p2b] w %u frames %llu-%llu: events %02x %02x %02x %02x %02x %02x + cycles%s; read cost %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", g_win, g_f0,
                     frame, k1::g_events & 0xFFu, (k1::g_events >> 8) & 0xFFu, (k1::g_events >> 16) & 0xFFu, k1::g_events >> 24, k1::g_eventsHi & 0xFFu,
                     (k1::g_eventsHi >> 8) & 0xFFu, ps2x_p2_skip >= 0 ? " (the sampler's counter: not meaningful)" : "", g_cost[0], g_cost[1], g_cost[2], g_cost[3],
                     g_cost[4], g_cost[5], g_cost[6]);
        g_f0 = frame;
        ++g_win;
    }
}

extern "C" int ps2x_p2b_register(const char *name)
{
    using namespace p2b;
    if (g_n >= kMax)
        return -1;
    std::memset(&g_acc[g_n], 0, sizeof(Acc));
    g_acc[g_n].name = name;
    return g_n++;
}
extern "C" bool ps2x_p2b_wanted()
{
    const char *b = std::getenv("SOTC_P2BRACKET");
    return b && std::atoi(b) != 0;
}
// A bracket opened: returns a token (0: not timed).
extern "C" uint32_t ps2x_p2b_enter(int id)
{
    using namespace p2b;
    if (!g_on || id < 0 || g_acc[id].off || g_depth >= kDepth)
        return 0u;
    Open &o = g_stack[g_depth];
    o.id = id;
    o.serial = ++g_serial;
    std::memset(o.child, 0, sizeof(o.child));
    read(o.start);
    ++g_depth;
    return (o.serial << 6) | static_cast<uint32_t>(g_depth);
}
extern "C" void ps2x_p2b_leave(uint32_t token)
{
    using namespace p2b;
    if (!token)
        return;
    const int depth = static_cast<int>(token & 63u);
    if (depth > g_depth || depth <= 0 || g_stack[depth - 1].serial != (token >> 6))
        return; // closed already (an inner guest thread switch / unwinding)
    uint32_t now[kEv];
    read(now);
    // anything opened above it and not closed left early: dropped
    for (int d = g_depth - 1; d >= depth; --d)
        ++g_acc[g_stack[d].id].early;
    g_depth = depth - 1;
    Open &o = g_stack[g_depth];
    Acc &a = g_acc[o.id];
    ++a.calls;
    for (int k = 0; k < kEv; ++k)
    {
        const uint64_t d = static_cast<uint32_t>(now[k] - o.start[k]);
        a.incl[k] += d;
        a.self[k] += d > o.child[k] ? d - o.child[k] : 0u;
        if (g_depth > 0)
            g_stack[g_depth - 1].child[k] += d;
    }
}

#endif // PLATFORM_VITA
