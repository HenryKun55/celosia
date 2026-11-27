#if defined(PLATFORM_VITA)

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <malloc.h>
#include <new>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/perf.h>
#include <psp2/sysmodule.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <taihen.h>
#include "../../vita/k1/celosia_k1.h" // round K1: the kernel module's argument / result blocks
#include "../../vita/k2/celosia_k2.h" // round K2: the resident module's syscall blocks
#include <psp2/kernel/modulemgr.h>
#include <psp2/power.h>
// libgcc's ARM EHABI unwinder (the SDK ships no unwind.h).
extern "C"
{
    struct _Unwind_Context;
    typedef int _Unwind_Reason_Code;
    _Unwind_Reason_Code _Unwind_Backtrace(_Unwind_Reason_Code (*)(struct _Unwind_Context *, void *), void *);
    int _Unwind_VRS_Get(struct _Unwind_Context *, int regclass, uint32_t regno, int representation, void *value);
}
namespace
{
    constexpr _Unwind_Reason_Code _URC_NO_REASON = 0, _URC_END_OF_STACK = 5;
    inline uint32_t _Unwind_GetIP(struct _Unwind_Context *ctx)
    {
        uint32_t pc = 0;
        _Unwind_VRS_Get(ctx, 0 /* core */, 15, 0 /* uint32 */, &pc);
        return pc & ~1u;
    }
}

namespace native_renderer
{
    void logFlush();
}

// Heap sizes must live in a native object as well(bc of LTO).
// the newlib malloc/new arena (PS2 guest RAM 32MB + GS + buffers live here).
// Round mem99: 160 -> 176 MB (the console has ~78 MB of user memory free
// besides it all session; the r98 session died at 135 MB used, 160 arena).
extern "C" unsigned int _newlib_heap_size_user = 176u * 1024u * 1024u;
extern "C" unsigned int sceLibcHeapSize = 8u * 1024u * 1024u;
// Recompiled guest code nests C++ calls deeply: give the main thread room.
extern "C" unsigned int sceUserMainThreadStackSize = 8u * 1024u * 1024u;
// The pthread default stack (vitasdk reads it at each pthread_create; 32 KB
// when left undefined). Raised around the game thread's creation only.
extern "C" unsigned int _pthread_stack_default_user = 32u * 1024u;

// Round M1 (SOTC_M1RAM=1 in env.txt, off by default; docs/decomp_m1.md): the guest RAM (32 MB + the 16-byte tail) and
// the scratchpad in a memory block of their own, 1 MB aligned and 33 MB long, taken BEFORE newlib's heap and paid for by
// a heap 33 MB smaller (the guest RAM is no longer in it): the same memory in all, the 89 MB boot test unchanged. Off,
// the guest RAM is a 4 KB aligned new[] inside the 176 MB heap block as before. The heap block is allocated by
// _init_vita_heap (crt0, before any constructor or env.txt read), so this switch is read here, from env.txt itself with
// sceIo (no libc yet), and only at boot: SOTC_AB_FLAG cannot rotate it (A/B it with two env files). The block is never
// freed (workers keep raw pointers into the guest RAM). ps2_memory.cpp takes it (ps2x_m1_guest_block).
extern "C" void __real__init_vita_heap(void);
extern "C" void *ps2x_m1_guest_block = nullptr;
extern "C" unsigned int ps2x_m1_guest_block_bytes = 0u;
extern "C" int ps2x_m1_guest_block_aligned = 0; // 1: M1's block, 1 MB aligned; 2: K1's kernel-allocated block
extern "C" void *ps2x_k1_guest_ram = nullptr;    // the guest RAM wherever it is (ps2_memory.cpp), for K1's probes
namespace bootenv
{
    // A line "<key><value>" (key includes the '='; the last such line wins) copied to val (NUL-terminated, at most
    // n - 1 characters); env.txt streamed in 4 KB chunks with sceIo (this runs before libc is up).
    bool get(const char *key, char *val, int n)
    {
        const SceUID fd = sceIoOpen("ux0:data/celosia/env.txt", SCE_O_RDONLY, 0);
        if (fd < 0)
            return false;
        static char buf[4096];
        int kKey = 0;
        while (key[kKey])
            ++kKey;
        int matched = 0; // characters of key matched at a line start; -1: not at a matching line
        int got = -1;    // characters of the value copied for the current matching line
        bool found = false;
        for (;;)
        {
            const int r = sceIoRead(fd, buf, sizeof(buf));
            if (r <= 0)
                break;
            for (int i = 0; i < r; ++i)
            {
                const char c = buf[i];
                if (c == '\n' || c == '\r')
                {
                    matched = 0;
                    got = -1;
                    continue;
                }
                if (matched < 0)
                    continue;
                if (matched < kKey)
                {
                    matched = c == key[matched] ? matched + 1 : -1;
                    if (matched == kKey)
                    {
                        got = 0;
                        val[0] = 0;
                        found = true;
                    }
                }
                else if (got >= 0 && got < n - 1)
                {
                    val[got++] = c;
                    val[got] = 0;
                }
            }
        }
        sceIoClose(fd);
        return found;
    }
    // "<key>v" with v not starting with '0' (M1's rule).
    bool on(const char *key)
    {
        char v[8];
        return get(key, v, sizeof(v)) && v[0] && v[0] != '0';
    }
    // Numbers "0x.." or decimal, separated by ',' / ':' / ' ' (no libc).
    const char *num(const char *s, uint32_t &out, bool &ok)
    {
        while (*s == ' ' || *s == ',' || *s == ':')
            ++s;
        uint32_t v = 0;
        ok = false;
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        {
            s += 2;
            for (;; ++s)
            {
                const char c = *s;
                const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
                if (d < 0)
                    break;
                v = v * 16u + static_cast<uint32_t>(d);
                ok = true;
            }
        }
        else
            for (; *s >= '0' && *s <= '9'; ++s)
            {
                v = v * 10u + static_cast<uint32_t>(*s - '0');
                ok = true;
            }
        out = v;
        return s;
    }
}
namespace m1ram
{
    constexpr unsigned kBlockBytes = 33u << 20; // 32 MB RAM, its tail, the scratchpad at +32 MB + 64 KB; 1 MB multiple
}

// Round K1 (docs/decomp_k1.md; SOTC_K1=1 / SOTC_K1RAM=1 in env.txt, off by default): the kernel module
// celosia_k1.skprx (vita/k1/celosia_k1.c, packed as module/celosia_k1.skprx), loaded by the game itself with
// taiLoadKernelModule + taiStartKernelModule (never from ur0:tai/config.txt: a bad module cannot keep the console from booting). It runs
// once per load and unloads itself (SCE_KERNEL_START_NO_RESIDENT); its answer is a K1Result written into our memory,
// believed only when its magic and this boot's nonce match (Vita3K runs no kernel module: the block stays zero and the
// game carries on exactly as without the switch).
//   SOTC_K1=1     boot: PMUSERENR.EN + the six event counters and the cycle counter on every core, CP15 registers read;
//                 at the first frame: the physical layout of the eboot's text / the guest RAM / the heap ("[k1] layout"),
//                 the TLB probes ("[k1probe]": the page size each is really mapped with), and with SOTC_M1PMU=<n> M1's
//                 "[m1pmu]" lines from these counters (read in user mode, no ScePerf).
//   SOTC_K1RAM=1  boot: the guest RAM block (M1's 33 MB) allocated by the module from candidate memblock types
//                 (SOTC_K1RAM_TYPES=type[:attr],...; default below), the one with the most 1 MB / 64 KB physically
//                 contiguous aligned chunks kept (SOTC_K1RAM_PICK=n: candidate n whatever its layout); none: M1's block
//                 if SOTC_M1RAM=1, else the default new[].
//   SOTC_K1_EVENTS=e0,...,e5  the six ARM event numbers (default M1's: 0x01,0x02,0x03,0x05,0x60,0x61).
//   SOTC_K1PROBE=1  the TLB probes without the module (wall time only).
//   SOTC_K1_FORCE=1  program the PMU even on a core where it is already enabled (PMCR.E; left alone by default).
// Read here at boot only (SOTC_AB_FLAG cannot rotate them).
namespace k1
{
    K1Result g_res;        // the boot load's answer
    K1Result g_resRegions; // the first frame's (K1_OP_REGIONS)
    int g_loadRc = 0, g_loadRc2 = 0;
    struct Step
    {
        int load = 0, start = 0, res = -1, unload = 0x7FFFFFFF; // taiLoadKernelModule, taiStartKernelModule, module_start's, unload
    };
    Step g_step[2]; // boot, first frame
    int g_nsteps = 0;
    bool g_on = false, g_ramOn = false, g_probeOn = false;
    bool g_ok = false; // the boot load answered
    char g_path[96];
    uint32_t g_events = 0x05030201u, g_eventsHi = 0x6160u;
    uint32_t g_pick = 0;
    uint32_t g_nonce = 0;
    uint32_t g_types[K1_CANDS], g_attrs[K1_CANDS];
    int g_ntypes = 0;
    // Default candidates (all 1 MB aligned): USER_RW physically contiguous (attr PHYCONT 0x00200000), USER_MAIN_GAME_RW,
    // plain USER_RW (M1's block, the control). Never USER_MAIN_PHYCONT_RW by default (vitaGL takes that whole 26 MB
    // budget at boot).
    const uint32_t kDefType[3] = {0x0C20D060u, 0x0C50D060u, 0x0C20D060u};
    const uint32_t kDefAttr[3] = {0x00200000u, 0u, 0u};
    bool resOk(const K1Result &r, uint32_t nonce)
    {
        const volatile uint32_t *m = &r.magic; // written by the kernel behind the compiler's back
        const volatile uint32_t *n = &r.nonce;
        return *m == K1_RES_MAGIC && *n == nonce;
    }
    int load(uint32_t ops, K1Result &res, const uint32_t *rva, const uint32_t *rsize, int nreg)
    {
        K1Args a;
        std::memset(&a, 0, sizeof(a));
        std::memset(&res, 0, sizeof(res));
        a.magic = K1_ARGS_MAGIC;
        a.version = K1_VERSION;
        a.ops = ops;
        a.nonce = g_nonce;
        a.result_va = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&res));
        a.result_size = sizeof(K1Result);
        a.events = g_events;
        a.events_hi = g_eventsHi;
        a.ram_bytes = m1ram::kBlockBytes;
        a.ram_align = 1u << 20;
        for (int i = 0; i < g_ntypes && i < K1_CANDS; ++i)
        {
            a.ram_type[i] = g_types[i];
            a.ram_attr[i] = g_attrs[i];
        }
        a.ram_pick = g_pick;
        a.nregions = static_cast<uint32_t>(nreg);
        for (int i = 0; i < nreg && i < K1_REGIONS; ++i)
        {
            a.region_va[i] = rva[i];
            a.region_size[i] = rsize[i];
        }
        // Two steps, not taiLoadStartKernelModule: Vita3K's taiHEN HLE loads a kernel module as a user module and starts
        // it while holding its kernel mutex, which hangs the boot (K1's first Vita3K run); its taiStartKernelModuleForUser
        // is a stub returning 0, so there the module never runs and the result stays zero. On the console both go to
        // taiHEN (ksceKernelLoadModule, then ksceKernelStartModule in this thread's syscall).
        Step &s = g_step[g_nsteps < 2 ? g_nsteps++ : 1];
        s = Step{};
        s.load = taiLoadKernelModule(g_path, 0, nullptr);
        if (s.load < 0)
            return s.load;
        s.start = taiStartKernelModule(s.load, sizeof(a), &a, 0, nullptr, &s.res);
        // The module returns SCE_KERNEL_START_NO_RESIDENT; if the loader kept it anyway, unload it now, unless a core
        // thread of it never ended (then its code must stay).
        bool stuck = false;
        for (int i = 0; i < K1_CORES; ++i)
            stuck |= res.core[i].thread_rc < 0;
        if (resOk(res, g_nonce) && !stuck)
            s.unload = taiUnloadKernelModule(s.load, 0, nullptr);
        return s.start < 0 ? s.start : s.load;
    }
    // The module's path: SOTC_K1_PATH, else ux0:app/<title id>/module/celosia_k1.skprx (the kernel's loader does not
    // see the game's app0: mount).
    void path()
    {
        if (bootenv::get("SOTC_K1_PATH=", g_path, sizeof(g_path)) && g_path[0])
            return;
        char tid[16];
        std::memset(tid, 0, sizeof(tid));
        if (sceAppMgrAppParamGetString(0, 12, tid, sizeof(tid)) < 0 || !tid[0])
            std::memcpy(tid, "CELO00001", 10);
        const char *pre = "ux0:app/", *post = "/module/celosia_k1.skprx";
        int n = 0;
        for (const char *q = pre; *q; ++q)
            g_path[n++] = *q;
        for (int i = 0; tid[i] && i < 12; ++i)
            g_path[n++] = tid[i];
        for (const char *q = post; *q; ++q)
            g_path[n++] = *q;
        g_path[n] = 0;
    }
    // SOTC_K1_EVENTS=e0,...,e5 (K1 and K2)
    void parseEvents()
    {
        char v[96];
        if (bootenv::get("SOTC_K1_EVENTS=", v, sizeof(v)))
        {
            const char *s = v;
            uint32_t ev[6] = {0x01, 0x02, 0x03, 0x05, 0x60, 0x61};
            for (int i = 0; i < 6; ++i)
            {
                uint32_t x;
                bool ok;
                s = bootenv::num(s, x, ok);
                if (!ok)
                    break;
                ev[i] = x & 0xFFu;
            }
            g_events = ev[0] | ev[1] << 8 | ev[2] << 16 | ev[3] << 24;
            g_eventsHi = ev[4] | ev[5] << 8;
        }
    }
    // At boot (before libc): the switches; with SOTC_K1 / SOTC_K1RAM one load for PMU / RAM / CP15. Returns the guest
    // RAM block's base when SOTC_K1RAM gave one.
    void *boot()
    {
        g_on = bootenv::on("SOTC_K1=");
        g_ramOn = bootenv::on("SOTC_K1RAM=");
        g_probeOn = bootenv::on("SOTC_K1PROBE=");
        if (!g_on && !g_ramOn)
            return nullptr;
        char v[96];
        parseEvents();
        if (bootenv::get("SOTC_K1RAM_TYPES=", v, sizeof(v)))
        {
            const char *s = v;
            while (g_ntypes < K1_CANDS)
            {
                uint32_t t, at = 0;
                bool ok;
                s = bootenv::num(s, t, ok);
                if (!ok)
                    break;
                if (*s == ':')
                    s = bootenv::num(s, at, ok);
                g_types[g_ntypes] = t;
                g_attrs[g_ntypes++] = at;
            }
        }
        else
            for (; g_ntypes < 3; ++g_ntypes)
            {
                g_types[g_ntypes] = kDefType[g_ntypes];
                g_attrs[g_ntypes] = kDefAttr[g_ntypes];
            }
        if (bootenv::get("SOTC_K1RAM_PICK=", v, sizeof(v)))
        {
            bool ok;
            bootenv::num(v, g_pick, ok);
        }
        path();
        g_nonce = (static_cast<uint32_t>(sceKernelGetProcessTimeWide()) * 2654435761u) | 1u;
        const uint32_t ops = (g_on ? K1_OP_PMU | K1_OP_SYSREGS : 0u) | (g_ramOn ? K1_OP_RAM : 0u) | (bootenv::on("SOTC_K1_FORCE=") ? K1_OP_PMU_FORCE : 0u);
        g_loadRc = load(ops, g_res, nullptr, nullptr, 0);
        g_ok = resOk(g_res, g_nonce);
        if (!g_ok || !g_ramOn || g_res.ram_chosen < 0 || g_res.ram_uid < 0)
            return nullptr;
        void *base = nullptr;
        // Through our own UID: the block must be ours, in our address space, and where the module said.
        if (sceKernelGetMemBlockBase(g_res.ram_uid, &base) >= 0 && base &&
            reinterpret_cast<uintptr_t>(base) == g_res.ram_base && (g_res.ram_base & 0xFFFFFu) == 0u)
            return base;
        sceKernelFreeMemBlock(g_res.ram_uid);
        g_res.ram_chosen = -2; // logged as "unusable"
        return nullptr;
    }
}
// Round K2 (docs/decomp_k2.md; SOTC_K2=1 / SOTC_K2REMAP=... in env.txt, off by default): the resident kernel module
// celosia_k2.skprx (vita/k2/celosia_k2.c) and its user bridge celosia_k2u.suprx, both packed in module/. K1 learned on
// the console that the kernel keeps the PMU registers per thread (PMUSERENR / PMCR / the counters are part of each
// thread's saved context), so K1's per-core setup never reached the game thread; K2's module stays loaded and the game
// thread asks it, through syscalls, to program and read the counters in its own context.
//   SOTC_K2=1        boot: the module + the bridge; first frame: the process's own translation tables walked
//                    ("[k2] walk": 4 KB / 64 KB / 1 MB per region), the PMU on the game thread ("[k2] pmu": what a
//                    thread switch keeps), the TLB probes with these counters; with SOTC_M1PMU=<n> M1's "[m1pmu]"
//                    lines from them (read in the kernel every frame, re-armed if a frame finds them gone).
//   SOTC_K2REMAP=ram|text|ram,text  boot (implies SOTC_K2): the guest RAM (M1's own 33 MB block, taken for it) and / or
//                    the eboot's text re-mapped with 1 MB sections where their 4 KB / 64 KB pages allow it (same PA and
//                    attributes, every chunk verified, any failure undone); the module puts the original entries back
//                    when the game exits or is killed. SOTC_K2REMAP_DRY=1: check only, change nothing.
//   SOTC_K1_EVENTS   the six events, as K1.
// Read at boot only. Vita3K never runs the module (no answer): the bridge is never loaded there and K2 is off.
// Round C1 (docs/decomp_c1.md; SOTC_C1CORE3=1 | probe in env.txt, off by default; implies the K2 module): core 3
// (SCE_KERNEL_CPU_MASK_SYSTEM, 0x80000) for this process. K2's module (its second library, CelosiaK2C1) hooks
// SceKernelThreadMgr's isIllegalAffinity as CapUnlocker does, but answers "legal" for our pid only; it releases the hook
// when the game exits or is killed. The C1 bridge celosia_c1u.suprx binds to that library (an older resident K2 without
// it: the bridge does not load, no hook). After the hook a test thread is created with affinity 0x80000 and run: if that
// is refused, or the thread does not run, the hook is released at once and the game stays on cores 0-2.
//   SOTC_C1CORE3=probe   only find and read the hook site (SceKernelThreadMgr's NID, the site's bytes), no hook
//   SOTC_C1CORE3_EXPECT=0x<word>  hook only if the site's first word is this one (pins a firmware once a probe told it)
// The thread maps that use core 3 are SOTC_C1MAP's (native_renderer.cpp threads::place).
// Round K3 (docs/decomp_k3.md; SOTC_K3=1 in env.txt, off by default): arch_plan's core-3 enabler. SOTC_K3 implies C1's
// hook and test (as SOTC_C1CORE3=1, never the probe) and so the K2 module; when the test passed, threads::place puts the
// light misc threads (SOTC_K3_SET, default audio,loader,log,main,watch; never game / render) on core 3 alone
// (SOTC_K3_WIDE=1: cores 1-3). Without core 3 (Vita3K, no module answer, an older resident K2, a failed test) every
// thread keeps today's cores. The resume watcher (raylib_vita.cpp) calls ps2x_k3_watch every 250 ms: it places itself,
// registers the exit release and prints the "[k3]" line (each thread's mask from sceKernelGetThreadCpuAffinityMask).
namespace k3
{
    bool g_want = false;
    volatile unsigned long long g_frame = 0; // the game's frame (ps2x_m1_pmu_tick, written only while g_want)
}
namespace c1
{
    bool g_want = false, g_probe = false;
    uint32_t g_expect = 0;
    int g_bridge = 1, g_bridgeStatus = -1, g_rc = 1, g_offRc = 1; // g_bridge 1: not tried (K2 off)
    int g_testCreate = 1, g_testStart = 1, g_testEnd = 1, g_testRan = 0, g_testMask = 0;
    bool g_core3 = false;
    K2C1Bridge g_br;
    K2Core3 g_res, g_off;
    volatile int g_ran = 0;
    void read()
    {
        k3::g_want = bootenv::on("SOTC_K3="); // round K3: core 3 for the misc threads needs C1's hook
        char v[16];
        if (!bootenv::get("SOTC_C1CORE3=", v, sizeof(v)) || !v[0] || v[0] == '0')
        {
            g_want = k3::g_want;
            return;
        }
        g_want = true;
        g_probe = v[0] == 'p' && !k3::g_want;
        char e[24];
        if (bootenv::get("SOTC_C1CORE3_EXPECT=", e, sizeof(e)))
        {
            bool ok;
            bootenv::num(e, g_expect, ok);
            if (!ok)
                g_expect = 0;
        }
    }
    int testEntry(SceSize, void *)
    {
        g_ran = 1;
        return 0;
    }
    void boot()
    {
        if (!g_want)
            return;
        std::memset(&g_br, 0, sizeof(g_br));
        std::memset(&g_res, 0, sizeof(g_res));
        std::memset(&g_off, 0, sizeof(g_off));
        K2C1Bridge *bp = &g_br;
        g_bridge = sceKernelLoadStartModule("app0:module/celosia_c1u.suprx", sizeof(bp), &bp, 0, nullptr, &g_bridgeStatus);
        if (g_bridge < 0 || g_br.magic != K2C1_MAGIC || g_br.version != K2C1_VERSION || !g_br.core3)
            return;
        g_rc = g_br.core3(g_probe ? K2_C3_PROBE : K2_C3_ON, g_expect, &g_res);
        if (g_probe || g_rc < 0 || g_res.hook_uid < 0)
            return;
        // The test: a thread on core 3 alone, created, started and waited for.
        const SceUID t = sceKernelCreateThread("celosia_c1_core3", testEntry, 0x10000100, 0x1000, 0, 0x80000, nullptr);
        g_testCreate = t;
        if (t >= 0)
        {
            SceKernelThreadInfo info;
            std::memset(&info, 0, sizeof(info));
            info.size = sizeof(info);
            if (sceKernelGetThreadInfo(t, &info) >= 0)
                g_testMask = static_cast<int>(info.currentCpuAffinityMask);
            g_testStart = sceKernelStartThread(t, 0, nullptr);
            SceUInt timeout = 500000u; // 0.5 s
            int status = 0;
            g_testEnd = g_testStart >= 0 ? sceKernelWaitThreadEnd(t, &status, &timeout) : -1;
            g_testRan = g_ran;
            if (g_testEnd >= 0 || g_testStart < 0)
                sceKernelDeleteThread(t); // (one that never ended is left alone)
        }
        g_core3 = t >= 0 && g_testStart >= 0 && g_testEnd >= 0 && g_testRan;
        if (!g_core3)
            g_offRc = g_br.core3(K2_C3_OFF, 0, &g_off);
    }
    void log()
    {
        if (!g_want)
            return;
        const K2Core3 &r = g_res;
        std::fprintf(stderr, "[c1] core3 %s: bridge app0:module/celosia_c1u.suprx 0x%08x status %d; call rc %d: SceKernelThreadMgr info rc 0x%08x "
                             "modid 0x%08x nid 0x%08x; site +0x%x at 0x%08x (rc 0x%08x): %08x | %08x %08x %08x %08x; expect %08x; hook 0x%08x, answers "
                             "for pid 0x%08x (calls %u, ours %u); test thread 0x%08x mask 0x%x start 0x%08x end 0x%08x ran %d; release %s (rc %d): %s\n",
                     g_probe ? "probe" : "on", static_cast<unsigned>(g_bridge), g_bridgeStatus, g_rc, static_cast<unsigned>(r.info_rc),
                     static_cast<unsigned>(r.modid), static_cast<unsigned>(r.module_nid), K2_C3_OFFSET, static_cast<unsigned>(r.addr),
                     static_cast<unsigned>(r.addr_rc), static_cast<unsigned>(r.before), static_cast<unsigned>(r.code[0]),
                     static_cast<unsigned>(r.code[1]), static_cast<unsigned>(r.code[2]), static_cast<unsigned>(r.code[3]),
                     static_cast<unsigned>(g_expect), static_cast<unsigned>(r.hook_uid), static_cast<unsigned>(r.pid),
                     static_cast<unsigned>(r.calls), static_cast<unsigned>(r.ours), static_cast<unsigned>(g_testCreate), g_testMask,
                     static_cast<unsigned>(g_testStart), static_cast<unsigned>(g_testEnd), g_testRan, g_offRc == 1 ? "no" : "yes", g_offRc,
                     g_core3 ? "CORE 3 ON (SOTC_C1MAP may use 0x80000)"
                     : g_bridge == 1 ? "core 3 off (K2 off: the C1 bridge was not tried; Vita3K / unsafe homebrew off), the game on cores 0-2"
                     : g_bridge < 0 ? "core 3 off (the C1 bridge did not load: an older K2 module still resident? reboot the console)"
                     : g_probe ? "core 3 off (probe only)"
                               : "core 3 off (hook or test failed), the game on cores 0-2");
    }
}
// SOTC_C1MAP's maps ask this (native_renderer.cpp threads::place): 1 when the core-3 test passed.
extern "C" int ps2x_c1_core3()
{
    return c1::g_core3 ? 1 : 0;
}
// Round K3: threads::place asks this: 0 SOTC_K3 off, 1 on but core 3 not ours (the fallback: today's cores), 2 core 3 ours.
extern "C" int ps2x_k3_state()
{
    return !k3::g_want ? 0 : c1::g_core3 ? 2 : 1;
}
extern "C" int ps2x_threads_k3_line(char *out, unsigned room, unsigned long long frame); // native_renderer.cpp (round K3)
extern "C" int ps2x_threads_k3_count();
extern "C" void ps2x_threads_place_watch();
namespace k3
{
    bool g_started = false, g_resumed = false, g_released = false;
    int g_lines = 0, g_lastCount = 0, g_offRc = 1;
    unsigned long long g_lastFrame = 0, g_every = 6000, g_first = 3000;
    uint64_t g_startUs = 0, g_lastUs = 0, g_resumeUs = 0;
    const char *why()
    {
        if (c1::g_core3)
            return "CORE 3 ON for this process (C1's hook + test passed)";
        if (c1::g_bridge == 1)
            return "core 3 OFF: K2 did not answer (Vita3K / unsafe homebrew off / module not found): fallback, every thread on today's cores";
        if (c1::g_bridge < 0)
            return "core 3 OFF: the C1 bridge did not load (an older K2 module still resident: reboot the console): fallback, today's cores";
        return "core 3 OFF: the hook or the core-3 test thread failed (see [c1]): fallback, today's cores";
    }
    // At exit (atexit, registered when core 3 was ours): the hook released, as A3's exitRestore puts 444 back. The module
    // also releases it when the process exits or is killed (K2's process-event handler) and at module stop.
    void exitRelease()
    {
        if (g_released || !c1::g_core3 || !c1::g_br.core3)
            return;
        g_released = true;
        K2Core3 r;
        std::memset(&r, 0, sizeof(r));
        g_offRc = c1::g_br.core3(K2_C3_OFF, 0, &r);
    }
    void line(const char *when, unsigned long long frame, uint64_t now)
    {
        static char buf[4096];
        int at = std::snprintf(buf, sizeof(buf), "[k3] %s, frame %llu (%u s): %s; ", when, frame,
                               static_cast<unsigned>((now - g_startUs) / 1000000u), why());
        if (at > 0 && at < static_cast<int>(sizeof(buf)))
            ps2x_threads_k3_line(buf + at, static_cast<unsigned>(sizeof(buf) - at), frame);
        std::fprintf(stderr, "%s\n", buf);
    }
}
// Round K3: from the resume watcher (raylib_vita.cpp), every 250 ms; resumed 1 right after a resume. One branch when off.
extern "C" void ps2x_k3_watch(int resumed)
{
    if (!k3::g_want)
        return;
    using namespace k3;
    const uint64_t now = sceKernelGetProcessTimeWide();
    if (!g_started)
    {
        g_started = true;
        g_startUs = g_lastUs = now;
        if (const char *e = std::getenv("SOTC_K3_EVERY"))
            g_every = static_cast<unsigned long long>(std::strtoul(e, nullptr, 10));
        if (const char *e = std::getenv("SOTC_K3_FIRST"))
            g_first = static_cast<unsigned long long>(std::strtoul(e, nullptr, 10));
        ps2x_threads_place_watch(); // the watcher itself (kind "watch")
        if (c1::g_core3)
            std::atexit(exitRelease);
    }
    if (resumed)
    {
        g_resumed = true;
        g_resumeUs = now;
    }
    const unsigned long long f = g_frame;
    const int n = ps2x_threads_k3_count();
    const char *when = nullptr;
    if (g_lines == 0)
    {
        if (f >= g_first || now - g_startUs >= 120000000u)
            when = "threads";
    }
    else if (g_resumed && now - g_resumeUs >= 2000000u)
        when = "threads after a resume";
    else if (n != g_lastCount && now - g_lastUs >= 10000000u)
        when = "threads (new threads)";
    else if (g_every && f - g_lastFrame >= g_every)
        when = "threads";
    if (!when)
        return;
    line(when, f, now);
    ++g_lines;
    g_resumed = false;
    g_lastCount = n;
    g_lastFrame = f;
    g_lastUs = now;
}
namespace k2
{
    bool g_on = false;       // any K2 switch
    uint32_t g_remapOps = 0; // 1: guest RAM, 2: text
    bool g_dry = false;
    bool g_ok = false; // module answered + bridge + hello
    bool g_resident = false; // round P3: the start failed, the bridge was tried on the resident module
    K2Bridge g_br;
    K2StartRes g_start;
    uint32_t g_nonce = 0;
    int g_load = 0, g_startRc = 0, g_startRes = -1, g_bridge = 0, g_bridgeStatus = -1, g_hello = 0;
    uint32_t g_hello4[4];
    char g_path[96], g_upath[96];
    K2Remap g_remapRam, g_remapText;
    int g_remapRamRc = 1, g_remapTextRc = 1; // 1: not tried
    uint32_t g_textVa = 0, g_textSize = 0;
    void paths()
    {
        if (!bootenv::get("SOTC_K2_PATH=", g_path, sizeof(g_path)) || !g_path[0])
        {
            // k1::path() gives ux0:app/<title id>/module/celosia_k1.skprx: the same with k2
            k1::path();
            int n = 0;
            for (; k1::g_path[n] && n < static_cast<int>(sizeof(g_path)) - 1; ++n)
                g_path[n] = k1::g_path[n];
            g_path[n] = 0;
            for (int i = 0; i + 1 < n; ++i)
                if (g_path[i] == 'k' && g_path[i + 1] == '1')
                    g_path[i + 1] = '2';
        }
        const char *u = "app0:module/celosia_k2u.suprx";
        int n = 0;
        for (; u[n]; ++n)
            g_upath[n] = u[n];
        g_upath[n] = 0;
    }
    void bridge()
    {
        std::memset(&g_br, 0, sizeof(g_br));
        K2Bridge *bp = &g_br;
        g_bridge = sceKernelLoadStartModule(g_upath, sizeof(bp), &bp, 0, nullptr, &g_bridgeStatus);
        if (g_bridge < 0 || g_br.magic != K2_BRIDGE_MAGIC || g_br.version != K2_VERSION || !g_br.hello)
            return;
        std::memset(g_hello4, 0, sizeof(g_hello4));
        g_hello = g_br.hello(g_nonce, g_hello4);
        g_ok = g_hello >= 0 && g_hello4[0] == (K2_RES_MAGIC ^ g_nonce) && g_hello4[1] == K2_VERSION;
    }
    // At boot (before libc), from __wrap__init_vita_heap.
    void boot()
    {
        g_on = bootenv::on("SOTC_K2=");
        char v[32];
        if (bootenv::get("SOTC_K2REMAP=", v, sizeof(v)))
            for (int i = 0; v[i]; ++i)
            {
                if (v[i] == 'r' && v[i + 1] == 'a' && v[i + 2] == 'm')
                    g_remapOps |= 1u;
                if (v[i] == 't' && v[i + 1] == 'e' && v[i + 2] == 'x' && v[i + 3] == 't')
                    g_remapOps |= 2u;
            }
        g_dry = bootenv::on("SOTC_K2REMAP_DRY=");
        c1::read();
        g_on = g_on || g_remapOps || c1::g_want;
        if (!g_on)
            return;
        k1::parseEvents();
        paths();
        g_nonce = (static_cast<uint32_t>(sceKernelGetProcessTimeWide()) * 2246822519u) | 1u;
        K2Start a;
        std::memset(&a, 0, sizeof(a));
        std::memset(&g_start, 0, sizeof(g_start));
        a.magic = K2_MAGIC;
        a.version = K2_VERSION;
        a.nonce = g_nonce;
        a.result_va = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_start));
        // Two steps as K1 (Vita3K's taiLoadStartKernelModule HLE hangs; its taiStartKernelModule never runs the module).
        g_load = taiLoadKernelModule(g_path, 0, nullptr);
        if (g_load >= 0)
        {
            g_startRc = taiStartKernelModule(g_load, sizeof(a), &a, 0, nullptr, &g_startRes);
            const volatile uint32_t *m = &g_start.magic, *n = &g_start.nonce;
            if (*m == K2_RES_MAGIC && *n == g_nonce && g_start.version == K2_VERSION)
                bridge();
            else if (g_startRc < 0)
            {
                // Round P3: the second launch without a reboot. The module is still resident (an earlier game exited:
                // its process-event handler released that game's state), so the load hands back the resident module's id
                // and the start fails (0x8002d013 on the console); the bridge binds to the resident module's syscalls and
                // its hello says whether it is this build's version (a module of an older version: K2 off, reboot).
                // Vita3K's start returns 0 without running the module: not reached there.
                g_resident = true;
                bridge();
            }
        }
        else
        {
            g_resident = true;
            bridge(); // the console only (Vita3K's load succeeds): a K2 module left resident by an earlier, killed run
        }
        if (g_ok)
            c1::boot(); // round C1: SOTC_C1CORE3 (core 3 for this process)
    }
    void remapText()
    {
        if (!g_ok || !(g_remapOps & 2u))
            return;
        SceKernelMemBlockInfo info;
        std::memset(&info, 0, sizeof(info));
        info.size = sizeof(info);
        if (sceKernelGetMemBlockInfoByAddr(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(&remapText) & ~static_cast<uintptr_t>(1)), &info) < 0)
            return;
        g_textVa = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(info.mappedBase));
        g_textSize = static_cast<uint32_t>(info.mappedSize);
        g_remapTextRc = g_br.remap(g_textVa, g_textSize, K2_REMAP_TEXT | (g_dry ? K2_REMAP_DRYRUN : 0u), &g_remapText);
    }
    void remapRam(void *block, uint32_t bytes)
    {
        if (!g_ok || !(g_remapOps & 1u) || !block)
            return;
        g_remapRamRc = g_br.remap(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block)), bytes, g_dry ? K2_REMAP_DRYRUN : 0u, &g_remapRam);
    }
}
// Round A3 (docs/decomp_a3cpu.md; the System tab's "CPU at 500 MHz", or SOTC_CPU500=1 in env.txt; off by default): the ARM
// at 500 MHz instead of 444 through K2's library CelosiaK2A3 (vita/k2/celosia_a3.c) and its bridge celosia_a3u.suprx.
// Nothing is loaded while the option is off. Turned on (at start from settings.cfg / env.txt, or in the menu): the K2
// module is loaded now if the boot did not load it (the same two steps as k2::boot, after libc; k2's own state is left
// alone, so none of K2's frame probes start), then the A3 bridge, then celosiaK2Clock(500). Any failure (no module
// answer: Vita3K, unsafe homebrew off, an older resident K2 without the library, a firmware whose sites do not check out,
// a busy loop that did not get faster) leaves the clock at 444 and says why in one "[a3]" line. Turned off: 444 through
// the module (ScePower's own path, the NOP taken out). At exit: 444 (atexit here, and the module itself when the process
// exits or is killed). After a resume / a return from LiveArea ScePower puts the game's 444 back: the resume watcher
// (raylib_vita.cpp) asks ps2x_cpu500_watch every 250 ms, which applies 500 again when ScePower's frequency is not 500.
namespace a3
{
    bool g_k2tried = false, g_k2ok = false, g_k2own = false;
    int g_load = 1, g_startRc = 1, g_startRes = -1, g_k2u = 1, g_k2uStatus = -1, g_hello = 1;
    int g_bridge = 1, g_bridgeStatus = -1;
    uint32_t g_hello4[4];
    K2StartRes g_start;
    K2Bridge g_k2br;
    K2A3Bridge g_br;
    K2Clock g_last;
    int g_lastRc = 1;
    bool g_want = false;  // the option
    int g_state = 0;      // 0 off, 1 500 on, -1 wanted but refused (444)
    unsigned g_reapplies = 0, g_reapplyFails = 0;
    bool g_atexit = false;
    SceUID g_mutex = -1;
    struct Lock
    {
        Lock()
        {
            if (g_mutex < 0)
                g_mutex = sceKernelCreateMutex("celosia_a3", 0, 0, nullptr);
            if (g_mutex >= 0)
                sceKernelLockMutex(g_mutex, 1, nullptr);
        }
        ~Lock()
        {
            if (g_mutex >= 0)
                sceKernelUnlockMutex(g_mutex, 1);
        }
    };
    const char *g_why = "";
    // The K2 module (and its owner claim): the boot's if it answered, else loaded now.
    bool ensureK2()
    {
        if (g_k2tried)
            return g_k2ok;
        g_k2tried = true;
        if (k2::g_ok)
            return g_k2ok = true; // SOTC_K2 / C1 loaded it at boot: this process owns it already
        if (k2::g_on)
        {
            g_why = "K2 was tried at boot and did not answer";
            return false;
        }
        g_k2own = true;
        k2::paths();
        const uint32_t nonce = (static_cast<uint32_t>(sceKernelGetProcessTimeWide()) * 2246822519u) | 1u;
        K2Start a;
        std::memset(&a, 0, sizeof(a));
        std::memset(&g_start, 0, sizeof(g_start));
        a.magic = K2_MAGIC;
        a.version = K2_VERSION;
        a.nonce = nonce;
        a.result_va = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&g_start));
        bool answered = false, resident = false;
        g_load = taiLoadKernelModule(k2::g_path, 0, nullptr);
        if (g_load >= 0)
        {
            g_startRc = taiStartKernelModule(g_load, sizeof(a), &a, 0, nullptr, &g_startRes);
            const volatile uint32_t *m = &g_start.magic, *n = &g_start.nonce;
            answered = *m == K2_RES_MAGIC && *n == nonce && g_start.version == K2_VERSION;
            resident = !answered && g_startRc < 0; // P3's case: the module resident from an earlier launch
        }
        else
            resident = true; // (the console only: a K2 module left by an earlier, killed run)
        if (!answered && !resident)
        {
            g_why = "the K2 module did not answer (Vita3K / unsafe homebrew off / module not found)";
            return false;
        }
        // The K2 bridge: hello claims the module for this process (and tells an older resident version apart).
        std::memset(&g_k2br, 0, sizeof(g_k2br));
        K2Bridge *bp = &g_k2br;
        g_k2u = sceKernelLoadStartModule(k2::g_upath, sizeof(bp), &bp, 0, nullptr, &g_k2uStatus);
        if (g_k2u < 0 || g_k2br.magic != K2_BRIDGE_MAGIC || g_k2br.version != K2_VERSION || !g_k2br.hello)
        {
            g_why = "the K2 bridge celosia_k2u.suprx did not load";
            return false;
        }
        std::memset(g_hello4, 0, sizeof(g_hello4));
        g_hello = g_k2br.hello(nonce, g_hello4);
        if (g_hello < 0 || g_hello4[0] != (K2_RES_MAGIC ^ nonce) || g_hello4[1] != K2_VERSION)
        {
            g_why = g_hello4[0] == (K2_RES_MAGIC ^ nonce) && g_hello4[1] != K2_VERSION ? "the resident K2 module is an older version (reboot the console once)"
                                                                                      : "the K2 module refused this process (hello)";
            return false;
        }
        return g_k2ok = true;
    }
    bool ensureBridge()
    {
        if (g_bridge != 1)
            return g_bridge >= 0 && g_br.magic == K2A3_MAGIC && g_br.version == K2A3_VERSION && g_br.clock;
        if (!ensureK2())
        {
            g_bridge = -2;
            return false;
        }
        std::memset(&g_br, 0, sizeof(g_br));
        K2A3Bridge *bp = &g_br;
        g_bridge = sceKernelLoadStartModule("app0:module/celosia_a3u.suprx", sizeof(bp), &bp, 0, nullptr, &g_bridgeStatus);
        if (g_bridge < 0 || g_br.magic != K2A3_MAGIC || g_br.version != K2A3_VERSION || !g_br.clock)
        {
            g_why = "the A3 bridge celosia_a3u.suprx did not load (an older K2 module still resident? reboot the console once)";
            if (g_bridge >= 0)
                g_bridge = -3;
            return false;
        }
        return true;
    }
    const char *errName(int rc)
    {
        switch (rc)
        {
        case 0: return "ok";
        case K2_ERR_NOT_OWNER: return "not the module's owner";
        case K2_ERR_BUSY: return "module busy";
        case K2_ERR_ARGS: return "bad request";
        case K2_ERR_SITE: return "a site did not check out on this firmware (nothing written)";
        case K2_ERR_CLOCK: return "the CPU did not get faster (undone)";
        default: return "a kernel call failed";
        }
    }
    void logClock(const char *why)
    {
        const K2Clock &c = g_last;
        std::fprintf(stderr, "[a3] cpu %u (%s): rc %d (%s); kernel: busy loop %u us at 444 MHz, %u us before (= %u MHz), %u us after (= %u MHz); "
                             "ScePower says %d -> %d, its words %u/%u -> %u/%u (module nid 0x%08x, info rc 0x%08x, words rc 0x%08x); set444 rc 0x%08x; "
                             "pervasive fn 0x%08x (rc 0x%08x) site %04x|%04x NOP inject 0x%08x (release rc %d) call rc %d; on %u; "
                             "scePowerGetArmClockFrequency now %d MHz: %s\n",
                     static_cast<unsigned>(c.want), why, g_lastRc, errName(g_lastRc), static_cast<unsigned>(c.loop_us444),
                     static_cast<unsigned>(c.loop_us_before), static_cast<unsigned>(c.est_before), static_cast<unsigned>(c.loop_us_after),
                     static_cast<unsigned>(c.est_after), static_cast<int>(c.get_before), static_cast<int>(c.get_after),
                     static_cast<unsigned>(c.var_before[0]), static_cast<unsigned>(c.var_before[1]), static_cast<unsigned>(c.var_after[0]),
                     static_cast<unsigned>(c.var_after[1]), static_cast<unsigned>(c.power_nid), static_cast<unsigned>(c.info_rc),
                     static_cast<unsigned>(c.var_rc), static_cast<unsigned>(c.set444_rc), static_cast<unsigned>(c.fn),
                     static_cast<unsigned>(c.export_rc), static_cast<unsigned>(c.site_prev), static_cast<unsigned>(c.site_hw),
                     static_cast<unsigned>(c.inject_uid), static_cast<int>(c.release_rc), static_cast<int>(c.pervasive_rc),
                     static_cast<unsigned>(c.on), scePowerGetArmClockFrequency(),
                     c.want == 500u ? (g_lastRc == 0 && c.on ? "CPU500 ON: the CPU runs at 500 MHz" : "CPU500 REFUSED: the CPU stays at 444 MHz")
                                    : (g_lastRc == 0 ? "CPU500 OFF: the CPU back at 444 MHz" : "CPU500 OFF: 444 request failed"));
    }
    void exitRestore();
    // Under the lock.
    int apply500(const char *why)
    {
        if (!ensureBridge())
        {
            g_state = -1;
            std::fprintf(stderr, "[a3] cpu 500 (%s): CPU500 REFUSED: the CPU stays at 444 MHz (%s; load 0x%08x start 0x%08x/%d k2u 0x%08x/%d hello 0x%08x "
                                 "a3u 0x%08x/%d)\n",
                         why, g_why, static_cast<unsigned>(g_load), static_cast<unsigned>(g_startRc), g_startRes, static_cast<unsigned>(g_k2u),
                         g_k2uStatus, static_cast<unsigned>(g_hello), static_cast<unsigned>(g_bridge), g_bridgeStatus);
            return 444;
        }
        std::memset(&g_last, 0, sizeof(g_last));
        g_lastRc = g_br.clock(K2_A3_500, &g_last);
        g_state = (g_lastRc == 0 && g_last.on) ? 1 : -1;
        if (g_state < 0) // the fallback: 444 asked again through the system's own call
            scePowerSetArmClockFrequency(444);
        logClock(why);
        if (g_state > 0 && !g_atexit)
        {
            g_atexit = true;
            std::atexit(exitRestore);
        }
        return g_state > 0 ? 500 : 444;
    }
    int apply444(const char *why)
    {
        if (g_bridge == 1 || !ensureBridge())
        {
            g_state = 0;
            return 444; // never loaded: nothing to undo
        }
        std::memset(&g_last, 0, sizeof(g_last));
        g_lastRc = g_br.clock(K2_A3_444, &g_last);
        if (g_lastRc < 0)
            scePowerSetArmClockFrequency(444);
        g_state = 0;
        logClock(why);
        return 444;
    }
    void exitRestore()
    {
        Lock l;
        if (g_state > 0)
            apply444("exit");
    }
}
// The menu / settings (host_menu.cpp): on 1 asks for 500, 0 for 444. Returns the clock the CPU runs at now (500 / 444).
extern "C" int ps2x_cpu500_set(int on)
{
    a3::Lock l;
    a3::g_want = on != 0;
    if (on)
        return a3::g_state > 0 ? 500 : a3::apply500("option on");
    if (a3::g_bridge >= 0 && a3::g_bridge != 1)
        return a3::apply444("option off"); // (the bridge loaded: 444 through the module, the NOP out)
    a3::g_state = 0;
    return 444;
}
// 0 off, 1 running at 500, -1 asked for but refused (444).
extern "C" int ps2x_cpu500_state()
{
    return a3::g_state;
}
// The resume watcher (raylib_vita.cpp), every 250 ms and after it set the clocks again on a resume: 500 applied again
// when the option is on and ScePower's frequency is not 500 any more (a resume, a return from LiveArea, the game's own
// 444). Three failed re-applies in a row: given up (444, "refused").
extern "C" void ps2x_cpu500_watch(int resumed)
{
    if (!a3::g_want || a3::g_state <= 0)
        return;
    if (!resumed && scePowerGetArmClockFrequency() == 500)
        return;
    a3::Lock l;
    if (!a3::g_want || a3::g_state <= 0)
        return;
    if (!resumed && scePowerGetArmClockFrequency() == 500)
        return;
    ++a3::g_reapplies;
    a3::apply500(resumed ? "again after a resume" : "again: ScePower no longer at 500");
    if (a3::g_state > 0)
        a3::g_reapplyFails = 0;
    else if (++a3::g_reapplyFails < 3)
        a3::g_state = 1; // try again at the next wake-up
    else
        std::fprintf(stderr, "[a3] cpu 500: %u re-applies, the last 3 failed: given up, 444\n", a3::g_reapplies);
}
extern "C" void __wrap__init_vita_heap(void)
{
    void *k1base = k1::boot();
    k2::boot(); // round K2: nothing unless SOTC_K2 / SOTC_K2REMAP
    if (k1base && !(_newlib_heap_size_user > m1ram::kBlockBytes + (64u << 20)))
    {
        sceKernelFreeMemBlock(k1::g_res.ram_uid); // (never with the 176 MB heap)
        k1base = nullptr;
    }
    if (k1base)
    {
        ps2x_m1_guest_block = k1base;
        ps2x_m1_guest_block_bytes = m1ram::kBlockBytes;
        ps2x_m1_guest_block_aligned = 2;
        _newlib_heap_size_user -= m1ram::kBlockBytes;
    }
    else if ((bootenv::on("SOTC_M1RAM=") || (k2::g_ok && (k2::g_remapOps & 1u))) && _newlib_heap_size_user > m1ram::kBlockBytes + (64u << 20))
    {
        SceKernelAllocMemBlockOpt opt;
        std::memset(&opt, 0, sizeof(opt));
        opt.size = sizeof(opt);
        opt.attr = SCE_KERNEL_ALLOC_MEMBLOCK_ATTR_HAS_ALIGNMENT;
        opt.alignment = 1u << 20;
        SceUID block = sceKernelAllocMemBlock("celosia_guest_ram", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, m1ram::kBlockBytes, &opt);
        ps2x_m1_guest_block_aligned = block >= 0;
        if (block < 0)
            block = sceKernelAllocMemBlock("celosia_guest_ram", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, m1ram::kBlockBytes, nullptr);
        void *base = nullptr;
        if (block >= 0 && sceKernelGetMemBlockBase(block, &base) >= 0 && base)
        {
            ps2x_m1_guest_block = base;
            ps2x_m1_guest_block_bytes = m1ram::kBlockBytes;
            _newlib_heap_size_user -= m1ram::kBlockBytes;
        }
    }
    // Round K2: the re-maps, while this is the only thread of the process (nothing else touches these pages yet).
    if (ps2x_m1_guest_block && ps2x_m1_guest_block_aligned)
        k2::remapRam(ps2x_m1_guest_block, ps2x_m1_guest_block_bytes);
    k2::remapText();
    __real__init_vita_heap();
}

// memcpy / memset: newlib's on the Vita are a branch to sceClibMemcpy /
// sceClibMemset, a call into another module for every copy (in Vita3K an HLE
// trap: ~3% of the game thread, mostly copies of a few dozen bytes: struct
// copies, the game's own memcpy). Small ones here with NEON, large ones still
// go to the system's. The loops must not become memcpy / memset calls again.
extern "C" void *sceClibMemcpy(void *dst, const void *src, size_t len);
extern "C" void *sceClibMemset(void *dst, int ch, size_t len);
#include <arm_neon.h>
#define PS2X_NO_LIBCALL __attribute__((optimize("no-tree-loop-distribute-patterns")))
extern "C" PS2X_NO_LIBCALL void *memcpy(void *__restrict dst, const void *__restrict src, size_t n)
{
    if (n > 512u)
        return sceClibMemcpy(dst, src, n);
    uint8_t *d = static_cast<uint8_t *>(dst);
    const uint8_t *s = static_cast<const uint8_t *>(src);
    for (; n >= 32u; n -= 32u, d += 32u, s += 32u)
    {
        const uint8x16_t a = vld1q_u8(s), b = vld1q_u8(s + 16);
        vst1q_u8(d, a);
        vst1q_u8(d + 16, b);
    }
    if (n >= 16u)
    {
        vst1q_u8(d, vld1q_u8(s));
        n -= 16u, d += 16u, s += 16u;
    }
    if (n >= 8u)
    {
        vst1_u8(d, vld1_u8(s));
        n -= 8u, d += 8u, s += 8u;
    }
    for (; n; --n)
        *d++ = *s++;
    return dst;
}
extern "C" PS2X_NO_LIBCALL void *memset(void *dst, int ch, size_t n)
{
    if (n > 512u)
        return sceClibMemset(dst, ch, n);
    uint8_t *d = static_cast<uint8_t *>(dst);
    const uint8x16_t v = vdupq_n_u8(static_cast<uint8_t>(ch));
    for (; n >= 32u; n -= 32u, d += 32u)
    {
        vst1q_u8(d, v);
        vst1q_u8(d + 16, v);
    }
    if (n >= 16u)
    {
        vst1q_u8(d, v);
        n -= 16u, d += 16u;
    }
    if (n >= 8u)
    {
        vst1_u8(d, vget_low_u8(v));
        n -= 8u, d += 8u;
    }
    for (; n; --n)
        *d++ = static_cast<uint8_t>(ch);
    return dst;
}

// Round 50: the unwinder's table lookup. The EE scheduler switches guest
// threads by throwing EeDispatcherTransfer up to its dispatcher (a WaitSema
// that blocks, an interrupt handler's invocation, a thread's exit), and the
// ARM EHABI unwinder finds each frame's entry by a binary search of the
// whole .ARM.exidx table (one entry per function of a 113 MB binary, ~19
// probes, twice a frame: the search and the cleanup pass). In the r48
// Vita3K profile search_EIT_table + selfrel_offset31 were ~0.33% of the game
// thread. libgcc asks __gnu_Unwind_Find_exidx (a weak reference) for the
// table first: this one answers with the one entry a small cache of return
// addresses already found for that pc (checked against the table before it
// is used, so a stale or torn slot only costs a search), and libgcc's own
// search over that one entry returns it, the entry its search of the whole
// table returns. A pc in no function gets the whole table, as before.
// SOTC_EXIDX_CHECK=1: every answer compared with the search of the whole
// table ([exidx] lines); SOTC_FASTNATIVE_STATS=1: lookups and hits every
// 2^18 lookups.
namespace
{
    struct ExidxEntry
    {
        uint32_t fn;
        uint32_t content;
    };
}
extern "C" const ExidxEntry __exidx_start[], __exidx_end[];
namespace
{
    // libgcc's selfrel_offset31: the prel31 offset sign-extended, plus its address.
    inline uint32_t exidxFn(const ExidxEntry *e)
    {
        uint32_t o = e->fn;
        o = (o & 0x40000000u) ? (o | 0x80000000u) : (o & 0x7FFFFFFFu);
        return o + static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&e->fn));
    }
    // libgcc's search_EIT_table, the index it returns (-1: none).
    int32_t exidxSearch(const ExidxEntry *t, int32_t nrec, uint32_t pc)
    {
        if (nrec == 0)
            return -1;
        int32_t left = 0, right = nrec - 1;
        for (;;)
        {
            const int32_t n = (left + right) / 2;
            const uint32_t thisFn = exidxFn(t + n);
            const uint32_t nextFn = n != nrec - 1 ? exidxFn(t + n + 1) - 1u : ~0u;
            if (pc < thisFn)
            {
                if (n == left)
                    return -1;
                right = n - 1;
            }
            else if (pc <= nextFn)
                return n;
            else
                left = n + 1;
        }
    }
    constexpr uint32_t kExidxSlots = 4096u;
    uint32_t g_exidxCache[kExidxSlots]; // entry index + 1, 0 = empty
    uint32_t g_exidxLookups = 0, g_exidxHits = 0, g_exidxChecks = 0, g_exidxBad = 0;
    signed char g_exidxCheck = -1, g_exidxStats = -1;
    bool exidxFlag(signed char &v, const char *name)
    {
        if (__builtin_expect(v < 0, 0))
        {
            const char *e = std::getenv(name);
            v = (e && e[0] == '1') ? 1 : 0;
        }
        return v != 0;
    }
}
extern "C" uintptr_t __gnu_Unwind_Find_exidx(uintptr_t pc, int *nrec)
{
    const ExidxEntry *const t = __exidx_start;
    const int32_t n = static_cast<int32_t>(__exidx_end - __exidx_start);
    const uint32_t p = static_cast<uint32_t>(pc);
    uint32_t *const slot = &g_exidxCache[((p >> 1) ^ (p >> 13)) & (kExidxSlots - 1u)];
    const uint32_t cached = __atomic_load_n(slot, __ATOMIC_RELAXED);
    int32_t i = -1;
    bool hit = false;
    if (cached != 0u && cached <= static_cast<uint32_t>(n))
    {
        const int32_t c = static_cast<int32_t>(cached - 1u);
        if (p >= exidxFn(t + c) && (c == n - 1 || p <= exidxFn(t + c + 1) - 1u))
            i = c, hit = true;
    }
    if (!hit)
    {
        i = exidxSearch(t, n, p);
        if (i >= 0)
            __atomic_store_n(slot, static_cast<uint32_t>(i) + 1u, __ATOMIC_RELAXED);
    }
    if (__builtin_expect(exidxFlag(g_exidxStats, "SOTC_FASTNATIVE_STATS"), 0))
    {
        const uint32_t k = __atomic_add_fetch(&g_exidxLookups, 1u, __ATOMIC_RELAXED);
        if (hit)
            __atomic_add_fetch(&g_exidxHits, 1u, __ATOMIC_RELAXED);
        if ((k & 0x3FFFFu) == 0u)
            std::fprintf(stderr, "[exidx] %u lookups, %u from the cache (%d entries)\n", k, g_exidxHits, n);
    }
    if (__builtin_expect(exidxFlag(g_exidxCheck, "SOTC_EXIDX_CHECK"), 0) && hit)
    {
        const int32_t full = exidxSearch(t, n, p);
        const uint32_t k = __atomic_add_fetch(&g_exidxChecks, 1u, __ATOMIC_RELAXED);
        const bool bad = full != i;
        if (bad)
            __atomic_add_fetch(&g_exidxBad, 1u, __ATOMIC_RELAXED);
        if (bad || (k & 0xFFFFu) == 0u)
            std::fprintf(stderr, "[exidx] check: %u hits checked, %u mismatches%s\n", k, g_exidxBad, bad ? " MISMATCH" : "");
    }
    if (i < 0)
    {
        *nrec = n; // libgcc's search of the whole table finds none either
        return reinterpret_cast<uintptr_t>(t);
    }
    *nrec = 1;
    return reinterpret_cast<uintptr_t>(t + i);
}

// Round mem99: the heap by the code that asked for it. SOTC_HEAP_PROF=<n>
// (env.txt; 0 / unset: off): every malloc / new / realloc / free of the
// newlib heap is counted against its call site (n = 1: the caller of malloc
// or new; n = 2..4: that many frames, by the unwinder: slow), and every
// SOTC_HEAP_PROF_EVERY seconds (default 30) the sites holding the most and
// the ones grown the most since the last report go to stderr ([heap]
// lines; the "base" field is operator new's address at run time: an
// address minus base plus nm's address of _Znwj is the ELF's). The tables
// live in a memory block of their own, outside the heap. Off: one load and
// a branch per call (malloc & co. are linked as __wrap_*).
extern "C" void *__real_malloc(size_t);
extern "C" void __real_free(void *);
extern "C" void *__real_realloc(void *, size_t);
extern "C" void *__real_calloc(size_t, size_t);
extern "C" void *__real_memalign(size_t, size_t);
namespace heapprof
{
    constexpr uint32_t kDepthMax = 4u;
    struct Site
    {
        uint32_t pc[kDepthMax];
        uint32_t liveBytes, liveCount, lastBytes, peakBytes;
        uint32_t allocs, lastAllocs;
    };
    struct Entry
    {
        uint32_t ptr, size;
        uint32_t site;
    };
    constexpr uint32_t kSites = 16384u, kEntryBits = 20u, kEntries = 1u << kEntryBits;
    Site *g_sites = nullptr;
    Entry *g_entries = nullptr;
    volatile int g_on = 0;
    uint32_t g_depth = 1u, g_untracked = 0u, g_full = 0u, g_sitesUsed = 0u;
    uint64_t g_liveBytes = 0u, g_liveCount = 0u;
    volatile int g_lock = 0;

    inline void lock()
    {
        while (__atomic_exchange_n(&g_lock, 1, __ATOMIC_ACQUIRE))
            while (__atomic_load_n(&g_lock, __ATOMIC_RELAXED))
                ;
    }
    inline void unlock() { __atomic_store_n(&g_lock, 0, __ATOMIC_RELEASE); }
    inline uint32_t slotOf(uint32_t ptr) { return ((ptr >> 3) * 2654435761u) >> (32u - kEntryBits); }

    struct Trace
    {
        uint32_t pc[kDepthMax];
        uint32_t n, skip;
    };
    _Unwind_Reason_Code traceStep(struct _Unwind_Context *ctx, void *arg)
    {
        Trace *t = static_cast<Trace *>(arg);
        if (t->skip)
        {
            --t->skip;
            return _URC_NO_REASON;
        }
        t->pc[t->n++] = static_cast<uint32_t>(_Unwind_GetIP(ctx));
        return t->n >= kDepthMax ? _URC_END_OF_STACK : _URC_NO_REASON;
    }

    uint32_t siteOf(uint32_t caller)
    {
        uint32_t pc[kDepthMax] = {caller, 0u, 0u, 0u};
        if (g_depth > 1u)
        {
            // Frames above the caller: the unwinder's list holds this
            // function, added(), the wrapper, then the caller and its callers.
            Trace t{};
            t.skip = 3u;
            _Unwind_Backtrace(traceStep, &t);
            for (uint32_t i = 1; i < g_depth && i < t.n; ++i)
                pc[i] = t.pc[i];
        }
        const uint32_t h = (pc[0] ^ pc[1] * 31u ^ pc[2] * 131u ^ pc[3] * 1031u) * 2654435761u;
        for (uint32_t k = 0; k < 64u; ++k)
        {
            const uint32_t i = (h + k) & (kSites - 1u);
            Site &s = g_sites[i];
            if (s.pc[0] == 0u)
            {
                std::memcpy(s.pc, pc, sizeof(pc));
                ++g_sitesUsed;
                return i;
            }
            if (s.pc[0] == pc[0] && s.pc[1] == pc[1] && s.pc[2] == pc[2] && s.pc[3] == pc[3])
                return i;
        }
        return 0u; // full: counted against slot 0
    }

    void added(void *p, size_t size, uint32_t caller)
    {
        if (!p)
            return;
        lock();
        const uint32_t site = siteOf(caller);
        uint32_t i = slotOf(reinterpret_cast<uint32_t>(p));
        uint32_t k = 0;
        for (; k < 512u && g_entries[i].ptr != 0u; ++k)
            i = (i + 1u) & (kEntries - 1u);
        if (k < 512u)
        {
            g_entries[i] = {reinterpret_cast<uint32_t>(p), static_cast<uint32_t>(size), site};
            Site &s = g_sites[site];
            s.liveBytes += static_cast<uint32_t>(size);
            ++s.liveCount;
            ++s.allocs;
            if (s.liveBytes > s.peakBytes)
                s.peakBytes = s.liveBytes;
            g_liveBytes += size;
            ++g_liveCount;
        }
        else
            ++g_full;
        unlock();
    }

    void removed(void *p)
    {
        if (!p)
            return;
        lock();
        const uint32_t key = reinterpret_cast<uint32_t>(p);
        uint32_t i = slotOf(key);
        bool found = false;
        for (uint32_t k = 0; k < 512u && g_entries[i].ptr != 0u; ++k, i = (i + 1u) & (kEntries - 1u))
            if (g_entries[i].ptr == key)
            {
                found = true;
                break;
            }
        if (!found)
        {
            ++g_untracked; // allocated before the profiler started (or table full)
            unlock();
            return;
        }
        Site &s = g_sites[g_entries[i].site];
        s.liveBytes -= g_entries[i].size;
        --s.liveCount;
        g_liveBytes -= g_entries[i].size;
        --g_liveCount;
        // Backward-shift deletion (linear probing).
        uint32_t hole = i;
        for (uint32_t j = (i + 1u) & (kEntries - 1u); g_entries[j].ptr != 0u; j = (j + 1u) & (kEntries - 1u))
        {
            const uint32_t home = slotOf(g_entries[j].ptr);
            const bool stays = hole <= j ? (home > hole && home <= j) : (home > hole || home <= j);
            if (!stays)
            {
                g_entries[hole] = g_entries[j];
                hole = j;
            }
        }
        g_entries[hole].ptr = 0u;
        unlock();
    }

    void start()
    {
        const char *e = std::getenv("SOTC_HEAP_PROF");
        const int depth = e ? std::atoi(e) : 0;
        if (depth <= 0)
            return;
        const SceSize bytes = (sizeof(Site) * kSites + sizeof(Entry) * kEntries + 0xFFFFFu) & ~0xFFFFFu;
        const SceUID block = sceKernelAllocMemBlock("celosia_heapprof", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, bytes, nullptr);
        void *base = nullptr;
        if (block < 0 || sceKernelGetMemBlockBase(block, &base) < 0)
        {
            std::fprintf(stdout, "[heap] profiler: no memory block of %u KB\n", static_cast<unsigned>(bytes >> 10));
            return;
        }
        std::memset(base, 0, bytes);
        g_sites = static_cast<Site *>(base);
        g_entries = reinterpret_cast<Entry *>(g_sites + kSites);
        g_depth = static_cast<uint32_t>(depth) > kDepthMax ? kDepthMax : static_cast<uint32_t>(depth);
        g_sites[0].pc[0] = 1u; // "other" (site table full)
        __atomic_store_n(&g_on, 1, __ATOMIC_RELEASE);
        std::fprintf(stdout, "[heap] profiler on, depth %u, %u KB of tables\n", g_depth, static_cast<unsigned>(bytes >> 10));
    }
}

extern "C" void *__wrap_malloc(size_t size)
{
    void *p = __real_malloc(size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}
extern "C" void __wrap_free(void *p)
{
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::removed(p);
    __real_free(p);
}
extern "C" void *__wrap_realloc(void *old, size_t size)
{
    if (!__builtin_expect(heapprof::g_on, 0))
        return __real_realloc(old, size);
    void *p = __real_realloc(old, size);
    if (p || !size)
    {
        heapprof::removed(old);
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    }
    return p;
}
extern "C" void *__wrap_calloc(size_t n, size_t size)
{
    void *p = __real_calloc(n, size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, n * size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}
extern "C" void *__wrap_memalign(size_t align, size_t size)
{
    void *p = __real_memalign(align, size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}

// [heap] report (the [vita:mem] logger calls it; nothing when off).
extern "C" void ps2x_vita_heap_report(double now)
{
    using namespace heapprof;
    if (!g_on)
        return;
    static const double every = []
    {
        const char *e = std::getenv("SOTC_HEAP_PROF_EVERY");
        return e ? std::atof(e) : 30.0;
    }();
    static double next = 0.0;
    if (now < next)
        return;
    next = now + every;
    constexpr uint32_t kTop = 40u, kGrow = 15u;
    static Site copy[kSites]; // .bss, outside the heap
    lock();
    std::memcpy(copy, g_sites, sizeof(copy));
    for (uint32_t i = 0; i < kSites; ++i)
    {
        g_sites[i].lastBytes = g_sites[i].liveBytes;
        g_sites[i].lastAllocs = g_sites[i].allocs;
    }
    const uint64_t live = g_liveBytes, count = g_liveCount;
    const uint32_t untracked = g_untracked, full = g_full, used = g_sitesUsed;
    unlock();
    const struct mallinfo mi = mallinfo();
    std::fprintf(stderr, "[heap] t=%.0f tracked live %.2f MB in %u blocks, %u sites (%u frees of untracked blocks, %u not tracked) | newlib used %.2f MB arena %.2f MB free %.2f MB | base %p\n",
                 now, live / 1048576.0, static_cast<unsigned>(count), used, untracked, full, mi.uordblks / 1048576.0, mi.arena / 1048576.0,
                 mi.fordblks / 1048576.0, reinterpret_cast<void *>(static_cast<void *(*)(size_t)>(&::operator new)));
    static uint16_t order[kSites];
    uint32_t n = 0;
    for (uint32_t i = 0; i < kSites; ++i)
        if (copy[i].pc[0] && (copy[i].liveBytes || copy[i].lastBytes || copy[i].allocs != copy[i].lastAllocs))
            order[n++] = static_cast<uint16_t>(i);
    // which 2 (round M1, "made"): the sites that allocated the most blocks since the last report, freed or not (the
    // per-frame allocations: a frame's new / delete pairs never show as live bytes).
    auto pick = [&](uint32_t top, int which, const char *what)
    {
        auto key = [which](const Site &s) -> int64_t
        {
            return which == 0   ? static_cast<int64_t>(s.liveBytes)
                   : which == 1 ? static_cast<int64_t>(s.liveBytes) - static_cast<int64_t>(s.lastBytes)
                                : static_cast<int64_t>(s.allocs - s.lastAllocs);
        };
        for (uint32_t r = 0; r < top && r < n; ++r)
        {
            uint32_t best = r;
            for (uint32_t j = r + 1; j < n; ++j)
                if (key(copy[order[j]]) > key(copy[order[best]]))
                    best = j;
            const uint16_t t = order[r];
            order[r] = order[best];
            order[best] = t;
            const Site &s = copy[order[r]];
            if (key(s) <= 0)
                break;
            std::fprintf(stderr, "[heap] %s %2u: %9.1f KB in %6u (%+9.1f KB, peak %9.1f KB, %u allocs, %+d) at %08x %08x %08x %08x\n", what,
                         static_cast<unsigned>(r), s.liveBytes / 1024.0, static_cast<unsigned>(s.liveCount),
                         (static_cast<int64_t>(s.liveBytes) - static_cast<int64_t>(s.lastBytes)) / 1024.0, s.peakBytes / 1024.0,
                         static_cast<unsigned>(s.allocs), static_cast<int>(s.allocs - s.lastAllocs), static_cast<unsigned>(s.pc[0]),
                         static_cast<unsigned>(s.pc[1]), static_cast<unsigned>(s.pc[2]), static_cast<unsigned>(s.pc[3]));
        }
    };
    pick(kTop, 0, "live");
    pick(kGrow, 1, "grew");
    pick(kTop, 2, "made");
}

namespace
{
    // Out of heap: written with sceIo and closed at once, so it survives the
    // abort that follows (buffered logs do not, at least in Vita3K). "base"
    // is operator new's address at run time (the console loads the code at
    // another address than the ELF's: subtract it, add nm's _Znwj).
    // Round mem99: an emergency reserve. A throwing new that finds no room
    // gives it back and tries again (once), instead of the uncaught
    // bad_alloc that ended the console's r98 session (a 2D batch's
    // push_back, the heap fragmented at 135 of 160 MB). The renderer's heap
    // pressure then evicts; the reserve is taken again (ps2x_vita_heap_report,
    // every 5 s) once the heap has room for it.
    constexpr size_t kReserveBytes = 6u << 20;
    void *volatile g_reserve = nullptr;
    volatile int g_reserveLock = 0;
    uint32_t g_reserveUsed = 0, g_oomReports = 0;
    void reserveArm()
    {
        if (g_reserve || __atomic_exchange_n(&g_reserveLock, 1, __ATOMIC_ACQUIRE))
            return;
        if (!g_reserve)
            g_reserve = __real_malloc(kReserveBytes);
        __atomic_store_n(&g_reserveLock, 0, __ATOMIC_RELEASE);
    }
    bool reserveRelease()
    {
        while (__atomic_exchange_n(&g_reserveLock, 1, __ATOMIC_ACQUIRE))
            ;
        void *r = g_reserve;
        g_reserve = nullptr;
        __atomic_store_n(&g_reserveLock, 0, __ATOMIC_RELEASE);
        if (!r)
            return false;
        __real_free(r);
        ++g_reserveUsed;
        return true;
    }

    void reportOutOfMemory(size_t size, void *caller, const char *what)
    {
        // Nothrow callers (the renderer's arenas) leave the item out and ask
        // again: the first 64 of a session are written.
        if (__atomic_add_fetch(&g_oomReports, 1u, __ATOMIC_RELAXED) > 64u)
            return;
        const struct mallinfo mi = mallinfo();
        char text[320];
        const int n = std::snprintf(text, sizeof(text), "out of memory: new(%u) from %p, heap used %.1f MB, arena %.1f MB, max %.1f MB, base %p, %s (reserve given back %u times)\n",
                                    static_cast<unsigned>(size), caller, mi.uordblks / 1048576.0, mi.arena / 1048576.0,
                                    _newlib_heap_size_user / 1048576.0,
                                    reinterpret_cast<void *>(static_cast<void *(*)(size_t)>(&::operator new)), what,
                                    static_cast<unsigned>(g_reserveUsed));
        const SceUID fd = sceIoOpen("ux0:data/celosia/oom.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0666);
        if (fd >= 0)
        {
            sceIoWrite(fd, text, n > 0 ? n : 0);
            sceIoClose(fd);
        }
    }

    __attribute__((noinline)) void *allocate(size_t size, void *caller, bool nothrow, size_t align = 0)
    {
        if (!size)
            size = 1;
        void *p = align > 8u ? __real_memalign(align, size) : __real_malloc(size);
        if (__builtin_expect(!p, 0))
        {
            if (!nothrow && reserveRelease())
            {
                p = align > 8u ? __real_memalign(align, size) : __real_malloc(size);
                reportOutOfMemory(size, caller, p ? "reserve given back, retried: ok" : "reserve given back, retried: failed");
            }
            else
                reportOutOfMemory(size, caller, nothrow ? "nothrow" : "no reserve");
        }
        if (__builtin_expect(!p, 0))
        {
            if (nothrow)
                return nullptr;
            throw std::bad_alloc();
        }
        if (__builtin_expect(heapprof::g_on, 0))
            heapprof::added(p, size, reinterpret_cast<uint32_t>(caller));
        return p;
    }
}

// The [vita:mem] logger's, every 5 s: the reserve taken again once the heap
// has room for it and 16 MB more, then SOTC_HEAP_PROF's report.
extern "C" void ps2x_vita_heap_tick(double now, size_t heapUsed)
{
    if (!g_reserve && heapUsed + kReserveBytes + (16u << 20) < _newlib_heap_size_user)
        reserveArm();
    ps2x_vita_heap_report(now);
}

// Each form names its own caller (new[] and the nothrow forms used to reach
// operator new through libstdc++'s, which named libstdc++ as the caller).
void *operator new(size_t size) { return allocate(size, __builtin_return_address(0), false); }
void *operator new[](size_t size) { return allocate(size, __builtin_return_address(0), false); }
void *operator new(size_t size, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true); }
void *operator new[](size_t size, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true); }
void operator delete(void *p) noexcept { __wrap_free(p); }
void operator delete[](void *p) noexcept { __wrap_free(p); }
void operator delete(void *p, size_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, size_t) noexcept { __wrap_free(p); }
void *operator new(size_t size, std::align_val_t a) { return allocate(size, __builtin_return_address(0), false, static_cast<size_t>(a)); }
void *operator new[](size_t size, std::align_val_t a) { return allocate(size, __builtin_return_address(0), false, static_cast<size_t>(a)); }
void *operator new(size_t size, std::align_val_t a, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true, static_cast<size_t>(a)); }
void *operator new[](size_t size, std::align_val_t a, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true, static_cast<size_t>(a)); }
void operator delete(void *p, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete(void *p, size_t, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, size_t, std::align_val_t) noexcept { __wrap_free(p); }

namespace
{
    // The Vita has no console: send stdout/stderr to a file before main().
    struct LogToFile
    {
        LogToFile()
        {
            std::freopen("ux0:data/celosia/log.txt", "w", stdout);
            std::freopen("ux0:data/celosia/err.txt", "w", stderr);
            std::setvbuf(stdout, nullptr, _IOLBF, 0);
            std::setvbuf(stderr, nullptr, _IONBF, 0);
            std::fputs("[vita] log started\n", stdout);
            // The Vita has no environment: ux0:data/celosia/env.txt holds
            // KEY=VALUE lines (the PS2X_* switches of a scripted run).
            if (FILE *env = std::fopen("ux0:data/celosia/env.txt", "r"))
            {
                // Round S7: 64 KB lines (static: off the stack). At 4 KB a long PS2X_INPUT_SCRIPT (S6's ride6, 23 KB) was cut
                // on the Vita while the Mac ran all of it.
                static char line[65536];
                while (std::fgets(line, sizeof(line), env))
                {
                    line[std::strcspn(line, "\r\n")] = '\0';
                    char *eq = std::strchr(line, '=');
                    if (line[0] == '#' || !eq)
                        continue;
                    *eq = '\0';
                    setenv(line, eq + 1, 1);
                    std::fprintf(stdout, "[vita] env %s=%s\n", line, eq + 1);
                }
                std::fclose(env);
            }
            // The port runs native on the Vita (renderer, sound, IOP, system)
            // unless env.txt says otherwise.
            setenv("SOTC_NATIVE", "1", 0);
            heapprof::start(); // round mem99: SOTC_HEAP_PROF
            reserveArm();      // round mem99: the emergency reserve
            // An uncaught exception (bad_alloc...) aborts without a word on
            // the Vita: say what it was and how full the heap is first.
            std::set_terminate([]
            {
                const char *what = "unknown";
                try
                {
                    if (std::exception_ptr e = std::current_exception())
                        std::rethrow_exception(e);
                }
                catch (const std::exception &ex)
                {
                    what = ex.what();
                }
                catch (...)
                {
                }
                native_renderer::logFlush(); // the frame / sound logs' ring (round 33)
                const struct mallinfo mi = mallinfo();
                std::fprintf(stderr, "[vita] terminate: %s (heap used %.1f of %.1f MB)\n", what, mi.uordblks / 1048576.0,
                             _newlib_heap_size_user / 1048576.0);
                std::fprintf(stdout, "[vita] terminate: %s\n", what);
                native_renderer::logFlush(); // round 34: stderr itself goes through the ring (logCaptureStderr)
                std::fflush(stdout);
                std::fflush(stderr);
                std::abort();
            });
        }
    };
    LogToFile g_logToFile;

    void ttyPuts(const char *text)
    {
        if (text)
        {
            std::fputs(text, stdout); // Vita TTY (capturable via PrincessLog etc.)
        }
    }
}

// Overrides raylib's debugnet transport with TTY output.
extern "C"
{
    int debugNetInit(const char *, int, int)
    {
        return 0;
    }

    void debugNetFinish(void)
    {
    }

    int debugNetUDPSend(const char *text)
    {
        ttyPuts(text);
        return 0;
    }

    int debugNetUDPPrintf(const char *fmt, ...)
    {
        char line[512];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(line, sizeof(line), fmt, args);
        va_end(args);
        ttyPuts(line);
        return 0;
    }

    int debugNetPrintf(int level, const char *fmt, ...)
    {
        (void)level;
        char line[512];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(line, sizeof(line), fmt, args);
        va_end(args);
        ttyPuts(line);
        return 0;
    }
}

// Round K1: the Cortex-A9's counters read in user mode once the kernel module set PMUSERENR.EN on every core (the game
// thread is pinned to core 0 by default: the counts are core 0's, i.e. the game thread plus what the system runs there).
// Never executed unless the module answered (k1::g_ok): Vita3K aborts on these CP15 instructions.
namespace k1pmu
{
    inline uint32_t userenr()
    {
        uint32_t v;
        asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(v));
        return v;
    }
    inline bool userOn() { return (userenr() & 1u) != 0u; }
    inline uint32_t counter(uint32_t k)
    {
        uint32_t v;
        asm volatile("mcr p15, 0, %0, c9, c12, 5\n\tisb" ::"r"(k) : "memory");
        asm volatile("mrc p15, 0, %0, c9, c13, 2" : "=r"(v));
        return v;
    }
    inline uint32_t cycles()
    {
        uint32_t v;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(v));
        return v;
    }
    inline void setEvent(uint32_t k, uint32_t ev)
    {
        asm volatile("mcr p15, 0, %0, c9, c12, 5\n\tisb" ::"r"(k) : "memory");
        asm volatile("mcr p15, 0, %0, c9, c13, 1\n\tisb" ::"r"(ev) : "memory");
    }
    void snapshot(SceUInt32 *v, uint32_t &cyc)
    {
        for (uint32_t k = 0; k < 6u; ++k)
            v[k] = counter(k);
        cyc = cycles();
    }
    uint32_t event(int k) { return k < 4 ? (k1::g_events >> (8 * k)) & 0xFFu : (k1::g_eventsHi >> (8 * (k - 4))) & 0xFFu; }
    const char *nameOf(uint32_t ev)
    {
        switch (ev)
        {
        case 0x01: return "icache miss"; // M1's names for M1's six
        case 0x02: return "itlb miss";
        case 0x03: return "dcache miss";
        case 0x04: return "dcache access";
        case 0x05: return "dtlb miss";
        case 0x06: return "loads";
        case 0x07: return "stores";
        case 0x10: return "branch mispredict";
        case 0x12: return "branches";
        case 0x60: return "icache stall cyc";
        case 0x61: return "dcache stall cyc";
        case 0x62: return "main tlb stall cyc";
        case 0x65: return "dcache evictions";
        case 0x66: return "no-dispatch cyc";
        case 0x67: return "issue-empty cyc";
        case 0x68: return "instructions";
        case 0x80: return "pld stall cyc";
        case 0x81: return "write stall cyc";
        case 0x82: return "i main tlb stall cyc";
        case 0x83: return "d main tlb stall cyc";
        case 0x84: return "i utlb stall cyc";
        case 0x85: return "d utlb stall cyc";
        case 0x86: return "dmb stall cyc";
        default: return "event";
        }
    }
    const char *name(int k) { return nameOf(event(k)); }
}

// Round K1: the TLB probes (on the game thread at the first frame). 256 loads, one per page-sized step, each step
// shifted by a cache line so the lines spread over the L1's sets; 1 warm-up pass, then kPasses timed passes:
//   A: stride 4 KB over 1 MB    4 KB pages: 256 TLB entries a pass (> the 32-entry micro TLB and the 128-entry main
//                               TLB: a miss per load); 64 KB pages: 16; 1 MB sections: 1.
//   B: stride 64 KB over 16 MB  64 KB pages: 256 entries (a miss per load); 1 MB sections: 16.
//   C: stride 16 B over 4 KB    one page: the baseline.
// With K1's counters, counters 0-3 are switched to DTLB refill / data main-TLB stall cycles / D-cache refill /
// D-cache access for the probe and back afterwards. Verdict: A misses ~1/load -> 4 KB pages; A ~0 but B ~1 -> 64 KB;
// both ~0 -> 1 MB (or larger).
namespace k1probe
{
    constexpr int kTouches = 256;
    constexpr int kPasses = 256;
    struct Out
    {
        double ns, cyc, ev[4];
    };
    __attribute__((noinline)) uint32_t walk(const uint8_t *base, uint32_t stride, uint32_t lineStep, int passes)
    {
        uint32_t s = 0;
        for (int p = 0; p < passes; ++p)
            for (int i = 0; i < kTouches; ++i)
                s += *reinterpret_cast<const volatile uint32_t *>(base + static_cast<uint32_t>(i) * stride + static_cast<uint32_t>(i & 63) * lineStep);
        return s;
    }
    volatile uint32_t g_sink;
    // Round K2: the counters read through the K2 module (in the kernel, this thread's core) instead of in user mode.
    int (*g_k2read)(K2Pmu *) = nullptr;
    void snap(uint32_t *c, uint32_t &y)
    {
        if (g_k2read)
        {
            K2Pmu p;
            std::memset(&p, 0, sizeof(p));
            g_k2read(&p);
            for (uint32_t k = 0; k < 4; ++k)
                c[k] = p.cnt[k];
            y = p.ccnt;
            return;
        }
        for (uint32_t k = 0; k < 4; ++k)
            c[k] = k1pmu::counter(k);
        y = k1pmu::cycles();
    }
    // stride 4 KB / 64 KB with a cache line's shift per load (lineStep 64); C: stride 16, no shift (4 KB, one page).
    Out run(const uint8_t *base, uint32_t stride, uint32_t lineStep, bool pmu)
    {
        Out o{};
        g_sink += walk(base, stride, lineStep, 1);
        uint32_t c0[4] = {}, y0 = 0;
        if (pmu)
            snap(c0, y0);
        const uint64_t t0 = sceKernelGetProcessTimeWide();
        g_sink += walk(base, stride, lineStep, kPasses);
        const uint64_t t1 = sceKernelGetProcessTimeWide();
        const double n = static_cast<double>(kTouches) * kPasses;
        if (pmu)
        {
            uint32_t c1[4] = {}, y1 = 0;
            snap(c1, y1);
            for (uint32_t k = 0; k < 4; ++k)
                o.ev[k] = static_cast<uint32_t>(c1[k] - c0[k]) / n;
            o.cyc = static_cast<uint32_t>(y1 - y0) / n;
        }
        o.ns = static_cast<double>(t1 - t0) * 1000.0 / n;
        return o;
    }
    void probe(const char *what, const void *start, uint32_t size, bool pmu)
    {
        const uintptr_t lo = (reinterpret_cast<uintptr_t>(start) + 0xFFFFFu) & ~static_cast<uintptr_t>(0xFFFFFu);
        const uintptr_t hi = reinterpret_cast<uintptr_t>(start) + size;
        if (!start || hi < lo + (17u << 20))
        {
            std::fprintf(stderr, "[k1probe] %s at %p (%u KB): too small for the probe\n", what, start, size >> 10);
            return;
        }
        const uint8_t *b = reinterpret_cast<const uint8_t *>(lo);
        const Out a = run(b, 4096u, 64u, pmu), bb = run(b, 65536u, 64u, pmu), c = run(b, 16u, 0u, pmu);
        const char *verdict = !pmu ? "? (no counters: compare the times)" : a.ev[0] > 0.5 ? "4 KB pages" : bb.ev[0] > 0.5 ? "64 KB pages" : a.ev[0] < 0.1 && bb.ev[0] < 0.1 ? "1 MB sections (or larger)" : "unclear";
        std::fprintf(stderr, "[k1probe] %s at %p: A (4 KB stride, 1 MB) %.1f ns %.1f cyc/load, dtlb %.3f, d-mainTLB stall %.1f cyc, dcache miss %.3f; "
                             "B (64 KB stride, 16 MB) %.1f ns %.1f cyc, dtlb %.3f, stall %.1f cyc, dcache miss %.3f; C (one page) %.1f ns %.1f cyc, dtlb %.3f: %s\n",
                     what, static_cast<const void *>(b), a.ns, a.cyc, a.ev[0], a.ev[1], a.ev[2], bb.ns, bb.cyc, bb.ev[0], bb.ev[1], bb.ev[2], c.ns, c.cyc,
                     c.ev[0], verdict);
    }
}

// Round K1: the boot load's answer in err.txt (ps2_memory.cpp calls this after its "[mem] guest RAM" line: libc is up).
extern "C" void ps2x_k2_boot_log();
// Round P2 (src/lib/ps2_vita_p2.cpp)
extern "C" void ps2x_p2_prepare();
extern "C" void ps2x_p2_start(unsigned long long frame, bool k2ok);
extern "C" void ps2x_p2_tick(unsigned long long frame);
extern "C" int ps2x_p2_skip;
extern "C" void ps2x_k1_boot_log()
{
    ps2x_k2_boot_log(); // round K2
    using namespace k1;
    static bool logged = false;
    if ((!g_on && !g_ramOn) || logged)
        return;
    logged = true;
    std::fprintf(stderr, "[k1] load %s (ops%s%s): taiLoadKernelModule 0x%08x, taiStartKernelModule 0x%08x, module_start %d, unload 0x%08x, %s\n", g_path, g_on ? " pmu+sysregs" : "", g_ramOn ? " ram" : "",
                 static_cast<unsigned>(g_step[0].load), static_cast<unsigned>(g_step[0].start), g_step[0].res, static_cast<unsigned>(g_step[0].unload),
                 g_ok ? "the module answered" : "no answer from the module (Vita3K / not found / refused: unsafe homebrew off?): K1 off, the game as without it");
    if (!g_ok)
        return;
    const K1Result &r = g_res;
    if (r.ops_done & K1_OP_SYSREGS)
        std::fprintf(stderr, "[k1] pid 0x%08x MIDR %08x ACTLR %08x NSACR %08x SCTLR %08x TTBCR %08x TTBR0 %08x TTBR1 %08x DACR %08x CONTEXTIDR %08x\n",
                     static_cast<unsigned>(r.pid), static_cast<unsigned>(r.midr), static_cast<unsigned>(r.actlr), static_cast<unsigned>(r.nsacr),
                     static_cast<unsigned>(r.sctlr), static_cast<unsigned>(r.ttbcr), static_cast<unsigned>(r.ttbr0), static_cast<unsigned>(r.ttbr1),
                     static_cast<unsigned>(r.dacr), static_cast<unsigned>(r.contextidr));
    for (int i = 0; i < K1_CORES && (r.ops_done & (K1_OP_PMU | K1_OP_SYSREGS)); ++i)
    {
        const K1Core &c = r.core[i];
        std::fprintf(stderr, "[k1] core %d: thread 0x%08x ran %u on cpu %u | PMCR %08x -> %08x (%u counters) PMUSERENR %x -> %x CNTENSET %08x -> %08x INTENSET %08x%s | ACTLR %08x | events %02x %02x %02x %02x %02x %02x\n",
                     i, static_cast<unsigned>(c.thread_rc), static_cast<unsigned>(c.ran), static_cast<unsigned>(c.mpidr & 3u), static_cast<unsigned>(c.pmcr_before),
                     static_cast<unsigned>(c.pmcr_after), static_cast<unsigned>((c.pmcr_before >> 11) & 0x1Fu), static_cast<unsigned>(c.userenr_before),
                     static_cast<unsigned>(c.userenr_after), static_cast<unsigned>(c.cntenset_before), static_cast<unsigned>(c.cntenset_after),
                     static_cast<unsigned>(c.intenset), (c.intenset || ((c.pmcr_before & 1u) && !(r.ops_done & K1_OP_PMU_FORCE))) ? " (already in use: left alone)" : "", static_cast<unsigned>(c.actlr),
                     static_cast<unsigned>(c.evtype[0] & 0xFFu), static_cast<unsigned>(c.evtype[1] & 0xFFu), static_cast<unsigned>(c.evtype[2] & 0xFFu),
                     static_cast<unsigned>(c.evtype[3] & 0xFFu), static_cast<unsigned>(c.evtype[4] & 0xFFu), static_cast<unsigned>(c.evtype[5] & 0xFFu));
    }
    if (r.ops_done & K1_OP_RAM)
    {
        for (uint32_t k = 0; k < r.ncands && k < K1_CANDS; ++k)
        {
            const K1Cand &c = r.cand[k];
            std::fprintf(stderr, "[k1] ram candidate %u: type 0x%08x attr 0x%08x: rc 0x%08x base 0x%08x | %u pages, %u unmapped, %u physical runs, 64 KB chunks %u/%u, 1 MB chunks %u/%u, first pa 0x%08x\n",
                         static_cast<unsigned>(k + 1), static_cast<unsigned>(c.type), static_cast<unsigned>(c.attr), static_cast<unsigned>(c.rc),
                         static_cast<unsigned>(c.base), static_cast<unsigned>(c.pages), static_cast<unsigned>(c.unmapped), static_cast<unsigned>(c.pa_runs),
                         static_cast<unsigned>(c.ok64k), static_cast<unsigned>(c.n64k), static_cast<unsigned>(c.ok1m), static_cast<unsigned>(c.n1m),
                         static_cast<unsigned>(c.pa_first));
        }
        if (r.ram_chosen >= 0)
            std::fprintf(stderr, "[k1] ram: candidate %d kept for the guest RAM (uid 0x%08x, base 0x%08x)\n", static_cast<int>(r.ram_chosen) + 1,
                         static_cast<unsigned>(r.ram_uid), static_cast<unsigned>(r.ram_base));
        else
            std::fprintf(stderr, "[k1] ram: no candidate kept (%s): the guest RAM as without SOTC_K1RAM\n",
                         r.ram_chosen == -2 ? "the block was not usable" : "none had large-page-capable chunks, or user uid failed");
    }
}

extern "C" void _init(void);
extern "C" void _fini(void);
// Round K1, first frame (game thread; from m1pmu::start, whatever SOTC_M1PMU says): the physical layout of the text /
// the guest RAM / the heap through a second (one-shot) load, the probes, and whether the counters are usable here.
extern "C" bool ps2x_k1_first_frame(unsigned long long frame)
{
    using namespace k1;
    const bool pmu = g_ok && g_on && (g_res.ops_done & K1_OP_PMU) && k1pmu::userOn();
    if (!(g_on && g_ok) && !g_probeOn)
        return false;
    struct Reg
    {
        const char *name;
        const void *base;
        uint32_t size;
    } reg[3];
    int nreg = 0;
    SceKernelMemBlockInfo info;
    auto block = [&](const char *name, const void *at)
    {
        std::memset(&info, 0, sizeof(info));
        info.size = sizeof(info);
        const int rc = sceKernelGetMemBlockInfoByAddr(const_cast<void *>(at), &info);
        std::fprintf(stderr, "[k1] block %s (holding %p): rc 0x%08x base %p size %u KB memory type 0x%08x access 0x%x\n", name, at, static_cast<unsigned>(rc),
                     info.mappedBase, static_cast<unsigned>(info.mappedSize >> 10), static_cast<unsigned>(info.memoryType), static_cast<unsigned>(info.access));
        return rc >= 0;
    };
    // (a Thumb function pointer has bit 0 set: the address of its first byte is that minus 1)
    if (block("text", reinterpret_cast<const void *>(reinterpret_cast<uintptr_t>(&ps2x_k1_first_frame) & ~static_cast<uintptr_t>(1))))
        reg[nreg++] = {"text", info.mappedBase, static_cast<uint32_t>(info.mappedSize)};
    else
    {
        // (Vita3K knows no memblock for a module's segment) crt's _init opens .text, _fini sits at its end.
        const uintptr_t lo = reinterpret_cast<uintptr_t>(&_init) & ~static_cast<uintptr_t>(0xFFF);
        const uintptr_t hi = reinterpret_cast<uintptr_t>(&_fini) & ~static_cast<uintptr_t>(1);
        if (hi > lo)
            reg[nreg++] = {"text (_init.._fini)", reinterpret_cast<const void *>(lo), static_cast<uint32_t>(hi - lo)};
    }
    if (ps2x_k1_guest_ram && block("guest RAM", ps2x_k1_guest_ram))
        reg[nreg++] = {"guest RAM", ps2x_k1_guest_ram, 32u << 20};
    void *heapPtr = std::malloc(64);
    if (heapPtr && block("heap", heapPtr) && !(ps2x_k1_guest_ram >= info.mappedBase && static_cast<const uint8_t *>(ps2x_k1_guest_ram) <
                                                                                       static_cast<const uint8_t *>(info.mappedBase) + info.mappedSize))
        reg[nreg++] = {"heap", info.mappedBase, static_cast<uint32_t>(info.mappedSize)};
    std::free(heapPtr);
    if (g_on && g_ok)
    {
        uint32_t va[K1_REGIONS], sz[K1_REGIONS];
        for (int i = 0; i < nreg; ++i)
        {
            va[i] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(reg[i].base));
            sz[i] = reg[i].size;
        }
        g_loadRc2 = load(K1_OP_REGIONS, g_resRegions, va, sz, nreg);
        const bool ok = resOk(g_resRegions, g_nonce);
        std::fprintf(stderr, "[k1] frame %llu: layout load 0x%08x start 0x%08x module_start %d unload 0x%08x: %s\n", frame, static_cast<unsigned>(g_step[1].load), static_cast<unsigned>(g_step[1].start), g_step[1].res, static_cast<unsigned>(g_step[1].unload), ok ? "answered" : "no answer");
        for (uint32_t i = 0; ok && i < g_resRegions.nregions && static_cast<int>(i) < nreg; ++i)
        {
            const K1Region &g = g_resRegions.region[i];
            std::fprintf(stderr, "[k1] layout %s 0x%08x + %u KB: %u pages, %u unmapped, %u physical runs, 64 KB chunks %u/%u, 1 MB chunks %u/%u (physically contiguous + aligned: what a large mapping needs), first pa 0x%08x\n",
                         reg[i].name, static_cast<unsigned>(g.va), static_cast<unsigned>(g.size >> 10), static_cast<unsigned>(g.pages), static_cast<unsigned>(g.unmapped),
                         static_cast<unsigned>(g.pa_runs), static_cast<unsigned>(g.ok64k), static_cast<unsigned>(g.n64k), static_cast<unsigned>(g.ok1m),
                         static_cast<unsigned>(g.n1m), static_cast<unsigned>(g.pa_first));
        }
    }
    SceKernelThreadInfo ti;
    std::memset(&ti, 0, sizeof(ti));
    ti.size = sizeof(ti);
    sceKernelGetThreadInfo(sceKernelGetThreadId(), &ti);
    std::fprintf(stderr, "[k1] frame %llu: game thread cores 0x%x, PMUSERENR here %s\n", frame, static_cast<unsigned>(ti.currentCpuAffinityMask),
                 !(g_ok && g_on) ? "not read (no module)" : pmu ? "on" : "off");
    if (pmu)
        for (uint32_t k = 0; k < 4; ++k)
            k1pmu::setEvent(k, k == 0 ? 0x05u : k == 1 ? 0x83u : k == 2 ? 0x03u : 0x04u);
    for (int i = 0; i < nreg; ++i)
        k1probe::probe(reg[i].name, reg[i].base, reg[i].size, pmu);
    if (pmu)
        for (uint32_t k = 0; k < 4; ++k)
            k1pmu::setEvent(k, k1pmu::event(static_cast<int>(k)));
    return pmu;
}

// Round K2: the boot's answers (from ps2x_k1_boot_log: libc is up).
namespace k2
{
    const char *kindName(int k)
    {
        switch (k)
        {
        case K2_PAGE_FAULT: return "unmapped";
        case K2_PAGE_4K: return "4 KB";
        case K2_PAGE_64K: return "64 KB";
        case K2_PAGE_1M: return "1 MB section";
        case K2_PAGE_16M: return "16 MB supersection";
        case K2_PAGE_TABLE_UNREADABLE: return "table not readable";
        default: return "?";
        }
    }
    const char *howName(int h)
    {
        return h == K2_HOW_IDENTITY ? "identity" : h == K2_HOW_LINEAR ? "kernel linear map" : h == K2_HOW_ALIAS ? "alias" : "none";
    }
    void logRemap(const char *what, int rc, const K2Remap &r)
    {
        if (rc == 1)
            return;
        std::fprintf(stderr, "[k2] remap %s 0x%08x + %u KB%s: rc %d, 1 MB chunks %u, eligible %u, now sections %u (skipped: not a page table %u, "
                             "L2 unreadable %u, not one aligned run %u, mixed attributes %u, full %u); table access %s PAR %08x; sample L1 %08x -> %08x "
                             "(L2 %08x); held %u; first failed check va %08x got %08x want %08x rc %d\n",
                     what, static_cast<unsigned>(r.va), static_cast<unsigned>(r.size >> 10), g_dry ? " (dry run)" : "", rc,
                     static_cast<unsigned>(r.chunks), static_cast<unsigned>(r.eligible), static_cast<unsigned>(r.done),
                     static_cast<unsigned>(r.skip_notcoarse), static_cast<unsigned>(r.skip_l2), static_cast<unsigned>(r.skip_contig),
                     static_cast<unsigned>(r.skip_attr), static_cast<unsigned>(r.skip_full), howName(r.how), static_cast<unsigned>(r.l1_par),
                     static_cast<unsigned>(r.sample_l1_old), static_cast<unsigned>(r.sample_l1_new), static_cast<unsigned>(r.sample_l2),
                     static_cast<unsigned>(r.held), static_cast<unsigned>(r.verify_va), static_cast<unsigned>(r.verify_got),
                     static_cast<unsigned>(r.verify_want), static_cast<int>(r.verify_rc));
    }
}
extern "C" void ps2x_k2_boot_log()
{
    using namespace k2;
    static bool logged = false;
    if (!g_on || logged)
        return;
    logged = true;
    std::fprintf(stderr, "[k2] load %s: taiLoadKernelModule 0x%08x, taiStartKernelModule 0x%08x, module_start %d, answer %s (instance %08x, "
                         "process-event handler 0x%08x); bridge %s: 0x%08x status %d; hello 0x%08x held %u: %s\n",
                 g_path, static_cast<unsigned>(g_load), static_cast<unsigned>(g_startRc), g_startRes,
                 g_start.magic == K2_RES_MAGIC && g_start.nonce == g_nonce ? "yes" : "no", static_cast<unsigned>(g_start.instance),
                 static_cast<unsigned>(g_start.procevent_rc), g_upath, static_cast<unsigned>(g_bridge), g_bridgeStatus,
                 static_cast<unsigned>(g_hello), static_cast<unsigned>(g_hello4[3]),
                 g_ok ? (g_resident ? "K2 on (the module resident from an earlier launch, bridged: no reboot needed)" : "K2 on")
                 : g_resident && g_bridge >= 0 && g_br.magic == K2_BRIDGE_MAGIC && g_hello4[0] == (K2_RES_MAGIC ^ g_nonce) && g_hello4[1] != K2_VERSION
                     ? "K2 off (the resident module is an older version: reboot the console once)"
                 : g_resident ? "K2 off (the resident module did not answer the bridge: reboot the console)"
                              : "K2 off (no answer: Vita3K / not found / unsafe homebrew off), the game as without it");
    if (g_resident)
        std::fprintf(stderr, "[k2] resident module: hello rc 0x%08x answer %08x version %u (this build %u) instance %08x held %u\n", static_cast<unsigned>(g_hello),
                     static_cast<unsigned>(g_hello4[0] ^ g_nonce), static_cast<unsigned>(g_hello4[1]), static_cast<unsigned>(K2_VERSION),
                     static_cast<unsigned>(g_hello4[2]), static_cast<unsigned>(g_hello4[3]));
    c1::log();
    logRemap("guest RAM", g_remapRamRc, g_remapRam);
    logRemap("text", g_remapTextRc, g_remapText);
    if ((g_remapOps & 1u) && g_remapRamRc == 1)
        std::fprintf(stderr, "[k2] remap guest RAM: not tried (%s)\n", !g_ok ? "K2 off" : "no own 1 MB-aligned block");
}

// Round K2, first frame (game thread, from m1pmu::start): the walk, the PMU's behaviour across a thread switch, the
// probes with K2's counters. Returns true when the per-frame counters can run (SOTC_M1PMU then counts through K2).
namespace k2
{
    struct Region
    {
        const char *name;
        uint32_t va, size;
    };
    void walkRegion(const Region &g)
    {
        // every MB (at most 96) and the first MB's 16 64 KB steps: how many of each mapping size
        uint32_t va[128];
        uint32_t n = 0;
        for (uint32_t off = 0; off < g.size && n < 96u; off += 0x100000u)
            va[n++] = g.va + off;
        for (uint32_t off = 0x10000u; off < 0x100000u && off < g.size && n < 128u; off += 0x10000u)
            va[n++] = g.va + off;
        uint32_t count[7] = {}, mismatch = 0;
        K2Walk w;
        K2WalkEnt first{};
        bool haveFirst = false;
        int how = 0;
        uint32_t l1par = 0;
        for (uint32_t i = 0; i < n; i += K2_WALK_MAX)
        {
            const uint32_t m = n - i < K2_WALK_MAX ? n - i : K2_WALK_MAX;
            std::memset(&w, 0, sizeof(w));
            if (g_br.walk(&va[i], m, &w) < 0)
                break;
            how = w.how;
            l1par = w.l1_par;
            if (i == 0u && g.name[0] == 't')
                std::fprintf(stderr, "[k2] walk: TTBCR %08x TTBR0 %08x TTBR1 %08x CONTEXTIDR %08x SCTLR %08x; L1 table read through %s at 0x%08x "
                                     "(write PAR %08x); module data VA %08x PA %08x\n",
                             static_cast<unsigned>(w.ttbcr), static_cast<unsigned>(w.ttbr0), static_cast<unsigned>(w.ttbr1),
                             static_cast<unsigned>(w.contextidr), static_cast<unsigned>(w.sctlr), howName(w.how), static_cast<unsigned>(w.l1_kva),
                             static_cast<unsigned>(w.l1_par), static_cast<unsigned>(w.mod_va), static_cast<unsigned>(w.mod_pa));
            for (uint32_t k = 0; k < m && k < w.nent; ++k)
            {
                const K2WalkEnt &e = w.ent[k];
                const int idx = e.kind >= 0 && e.kind <= 4 ? e.kind : 5;
                ++count[idx];
                if (e.kind > 0 && (e.sys_rc < 0 || e.pa_walk != e.pa_sys))
                    ++mismatch;
                if (!haveFirst)
                {
                    first = e;
                    haveFirst = true;
                }
            }
        }
        std::fprintf(stderr, "[k2] walk %s 0x%08x + %u KB (%u samples: every MB + 64 KB steps in the first): 1 MB sections %u, 64 KB pages %u, "
                             "4 KB pages %u, 16 MB %u, unmapped %u, unreadable %u; walk PA != system PA %u; first: L1 %08x L2 %08x PA %08x/%08x "
                             "user PAR %08x (%s); access %s, PAR %08x\n",
                     g.name, static_cast<unsigned>(g.va), static_cast<unsigned>(g.size >> 10), static_cast<unsigned>(n), count[K2_PAGE_1M],
                     count[K2_PAGE_64K], count[K2_PAGE_4K], count[K2_PAGE_16M], count[K2_PAGE_FAULT], count[5], mismatch,
                     static_cast<unsigned>(first.l1), static_cast<unsigned>(first.l2), static_cast<unsigned>(first.pa_walk),
                     static_cast<unsigned>(first.pa_sys), static_cast<unsigned>(first.par), kindName(first.kind), howName(how),
                     static_cast<unsigned>(l1par));
    }
    void pmuLine(const char *tag, const K2Pmu &p)
    {
        std::fprintf(stderr, " %s: cpu %u PMCR %08x USERENR %x CNTENSET %08x INTENSET %08x events %02x %02x %02x %02x %02x %02x cycles %u",
                     tag, static_cast<unsigned>(p.cpu), static_cast<unsigned>(p.pmcr), static_cast<unsigned>(p.userenr),
                     static_cast<unsigned>(p.cntenset), static_cast<unsigned>(p.intenset), static_cast<unsigned>(p.evtype[0] & 0xFFu),
                     static_cast<unsigned>(p.evtype[1] & 0xFFu), static_cast<unsigned>(p.evtype[2] & 0xFFu), static_cast<unsigned>(p.evtype[3] & 0xFFu),
                     static_cast<unsigned>(p.evtype[4] & 0xFFu), static_cast<unsigned>(p.evtype[5] & 0xFFu), static_cast<unsigned>(p.ccnt));
    }
    uint32_t g_cpu = 0;
    // Programmed the way we asked (enabled, six + cycle counters on, our events, same core)?
    bool intact(const K2Pmu &p, uint32_t lo, uint32_t hi)
    {
        if (!(p.pmcr & 1u) || (p.cntenset & 0x8000003Fu) != 0x8000003Fu || p.cpu != g_cpu)
            return false;
        for (uint32_t k = 0; k < 6u; ++k)
            if ((p.evtype[k] & 0xFFu) != (k < 4u ? (lo >> (8u * k)) & 0xFFu : (hi >> (8u * (k - 4u))) & 0xFFu))
                return false;
        return true;
    }
    bool g_pmuOk = false;
}
extern "C" bool ps2x_k2_first_frame(unsigned long long frame)
{
    using namespace k2;
    if (!g_ok)
        return false;
    // 1) the walk
    Region reg[4];
    int nreg = 0;
    SceKernelMemBlockInfo info;
    std::memset(&info, 0, sizeof(info));
    info.size = sizeof(info);
    if (sceKernelGetMemBlockInfoByAddr(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(&ps2x_k2_first_frame) & ~static_cast<uintptr_t>(1)), &info) >= 0)
        reg[nreg++] = {"text", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(info.mappedBase)), static_cast<uint32_t>(info.mappedSize)};
    if (ps2x_k1_guest_ram)
        reg[nreg++] = {"guest RAM", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ps2x_k1_guest_ram)), 32u << 20};
    void *heapPtr = std::malloc(64);
    std::memset(&info, 0, sizeof(info));
    info.size = sizeof(info);
    if (heapPtr && sceKernelGetMemBlockInfoByAddr(heapPtr, &info) >= 0)
        reg[nreg++] = {"heap", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(info.mappedBase)), static_cast<uint32_t>(info.mappedSize)};
    std::free(heapPtr);
    uint32_t local = 0;
    reg[nreg++] = {"game thread stack", static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&local)) & ~0xFFFu, 0x1000u};
    const uint64_t tw0 = sceKernelGetProcessTimeWide();
    for (int i = 0; i < nreg; ++i)
        walkRegion(reg[i]);
    std::fprintf(stderr, "[k2] frame %llu: walk took %.1f ms\n", frame, static_cast<double>(sceKernelGetProcessTimeWide() - tw0) / 1000.0);
    // 2) the PMU on this (the game) thread: armed in its own context, then read straight away and after a 2 ms sleep
    // (other threads run on the core meanwhile): what the thread switch keeps.
    const uint32_t lo = k1::g_events, hi = k1::g_eventsHi;
    K2Pmu ab[2], r0, r1;
    std::memset(ab, 0, sizeof(ab));
    std::memset(&r0, 0, sizeof(r0));
    std::memset(&r1, 0, sizeof(r1));
    const int arm = g_br.pmuArm(lo, hi, K2_ARM_RESET | K2_ARM_USERENR, ab);
    g_br.pmuRead(&r0);
    const uint32_t u0 = k1pmu::userenr(); // the console allows this read at PL0; never reached in Vita3K (no module)
    const uint64_t t0 = sceKernelGetProcessTimeWide();
    sceKernelDelayThread(2000);
    const uint64_t t1 = sceKernelGetProcessTimeWide();
    g_br.pmuRead(&r1);
    const uint32_t u1 = k1pmu::userenr();
    g_cpu = ab[1].cpu;
    const int mhz = scePowerGetArmClockFrequency();
    const double coreCyc = static_cast<double>(t1 - t0) * (mhz > 0 ? mhz : 444);
    const bool kept = intact(r1, lo, hi);
    std::fprintf(stderr, "[k2] pmu frame %llu: arm 0x%08x |", frame, static_cast<unsigned>(arm));
    pmuLine("before", ab[0]);
    pmuLine("| armed", ab[1]);
    pmuLine("| read", r0);
    pmuLine("| after a 2 ms sleep", r1);
    std::fprintf(stderr, " | user-mode PMUSERENR %x -> %x | %s; cycles over the sleep %u = %.2f of the core's %.0f (%d MHz): %s\n",
                 static_cast<unsigned>(u0), static_cast<unsigned>(u1), kept ? "kept across the switch" : "LOST across the switch (re-armed per frame)",
                 static_cast<unsigned>(r1.ccnt - r0.ccnt), static_cast<double>(static_cast<uint32_t>(r1.ccnt - r0.ccnt)) / coreCyc, coreCyc, mhz,
                 static_cast<uint32_t>(r1.ccnt - r0.ccnt) < coreCyc * 0.2 ? "per-thread counters (stopped while this thread slept)" : "core-wide counters");
    g_pmuOk = arm >= 0 && intact(ab[1], lo, hi);
    // 3) the TLB probes with these counters (counters 0-3: dTLB refill / d main-TLB stall / dcache refill / dcache access)
    if (g_pmuOk)
    {
        const uint32_t plo = 0x04038305u;
        g_br.pmuArm(plo, hi, 0u, ab);
        k1probe::g_k2read = g_br.pmuRead;
        for (int i = 0; i < nreg - 1; ++i)
            k1probe::probe(reg[i].name, reinterpret_cast<const void *>(static_cast<uintptr_t>(reg[i].va)), reg[i].size, true);
        k1probe::g_k2read = nullptr;
        g_br.pmuArm(lo, hi, K2_ARM_RESET, ab);
    }
    return g_pmuOk;
}
// Per frame (mode 3 of m1pmu): read in the kernel; a frame whose counters are not as armed is not counted, and they are
// armed again.
namespace k2
{
    K2Pmu g_prev;
    unsigned g_lost = 0, g_valid = 0;
    bool g_havePrev = false;
    bool tick(uint64_t *sum, uint64_t &sumCyc)
    {
        K2Pmu p;
        if (g_br.pmuRead(&p) < 0)
            return false;
        const uint32_t lo = k1::g_events, hi = k1::g_eventsHi;
        if (intact(p, lo, hi) && g_havePrev)
        {
            for (int k = 0; k < 6; ++k)
                if (k != ps2x_p2_skip) // round P2: the sampler's counter is reloaded by its interrupts
                    sum[k] += static_cast<uint32_t>(p.cnt[k] - g_prev.cnt[k]);
            sumCyc += static_cast<uint32_t>(p.ccnt - g_prev.ccnt);
            ++g_valid;
            g_prev = p;
            return true;
        }
        if (!intact(p, lo, hi))
        {
            ++g_lost;
            K2Pmu ab[2];
            if (g_br.pmuArm(lo, hi, 0u, ab) < 0)
                return false;
            p = ab[1];
        }
        g_prev = p;
        g_havePrev = true;
        return true;
    }
}

// Round M1 (SOTC_M1PMU=<n>, off by default): the Cortex-A9's performance counters on the game thread, if this firmware
// lets a game read them. At the first call (sotc_native's endFrame, the game thread): PMUSERENR (readable at PL0 on
// ARMv7: bit 0 says whether user code may read the counters itself), then ScePerf (libperf, loaded with
// sceSysmoduleLoadModule; per-thread counters kept by the system) with six events; every call adds the counters' growth,
// every n frames one "[m1pmu]" line of per-frame means. Every return code is logged once; any failure turns it off.
// Nothing is installed: a console that refuses says so in err.txt.
namespace m1pmu
{
    constexpr int kN = 6;
    const SceUInt8 kEvent[kN] = {SCE_PERF_ARM_PMON_ICACHE_MISS, SCE_PERF_ARM_PMON_ITLB_MISS, SCE_PERF_ARM_PMON_DCACHE_MISS,
                                 SCE_PERF_ARM_PMON_DTLB_MISS, SCE_PERF_ARM_PMON_ICACHE_STALL, SCE_PERF_ARM_PMON_DCACHE_STALL};
    const char *const kName[kN] = {"icache miss", "itlb miss", "dcache miss", "dtlb miss", "icache stall cyc", "dcache stall cyc"};
    int g_state = 0; // 0: not started, 1: counting, -1: off
    unsigned g_every = 0;
    SceUInt32 g_last[kN];
    uint64_t g_sum[kN];
    uint64_t g_t0 = 0;
    unsigned long long g_f0 = 0;
    uint32_t g_lastCyc = 0;
    uint64_t g_sumCyc = 0;
    int g_mode = 0; // 1: ScePerf (M1), 2: the counters themselves (K1: PMUSERENR.EN set by the kernel module)
    bool start(unsigned long long frame)
    {
        const char *e = std::getenv("SOTC_M1PMU");
        g_every = e ? static_cast<unsigned>(std::atoi(e)) : 0u;
        ps2x_p2_prepare(); // round P2: the sampled event into K2's counter 3 (SOTC_P2PMUSAMPLE; nothing when off)
        const bool k2counting = ps2x_k2_first_frame(frame); // round K2: walk, PMU on this thread, probes
        ps2x_p2_start(frame, k2counting); // round P2: the sampler / the brackets (SOTC_P2PMUSAMPLE / SOTC_P2BRACKET)
        const bool k1counting = ps2x_k1_first_frame(frame); // round K1: layouts + probes; true: the counters are ours
        if (!g_every)
            return false;
        if (k2counting)
        {
            for (int i = 0; i < kN; ++i)
                g_sum[i] = 0u;
            g_sumCyc = 0u;
            k2::g_havePrev = false;
            k2::tick(g_sum, g_sumCyc);
            k2::g_valid = k2::g_lost = 0u;
            g_t0 = sceKernelGetProcessTimeWide();
            g_f0 = frame;
            g_mode = 3;
            std::fprintf(stderr, "[m1pmu] frame %llu: counting with K2's counters on the game thread (events %s / %s / %s / %s / %s / %s + cycles)\n", frame,
                         k1pmu::name(0), k1pmu::name(1), k1pmu::name(2), k1pmu::name(3), k1pmu::name(4), k1pmu::name(5));
            return true;
        }
        if (k1counting && !k2::g_ok)
        {
            for (int i = 0; i < kN; ++i)
                g_sum[i] = 0u;
            g_sumCyc = 0u;
            k1pmu::snapshot(g_last, g_lastCyc);
            g_t0 = sceKernelGetProcessTimeWide();
            g_f0 = frame;
            g_mode = 2;
            std::fprintf(stderr, "[m1pmu] frame %llu: counting with K1's counters (events %s / %s / %s / %s / %s / %s + cycles)\n", frame,
                         k1pmu::name(0), k1pmu::name(1), k1pmu::name(2), k1pmu::name(3), k1pmu::name(4), k1pmu::name(5));
            return true;
        }
        g_mode = 1;
        // PMUSERENR only with SOTC_M1PMU_USERENR=1: Vita3K aborts on the CP15 read ("Unhandled CP15 MRC"), the console
        // allows it at PL0 (ARMv7).
        uint32_t userenr = 0xFFFFFFFFu;
        const char *ue = std::getenv("SOTC_M1PMU_USERENR");
        if (ue && std::atoi(ue) != 0)
            asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(userenr));
        const int load = sceSysmoduleLoadModule(SCE_SYSMODULE_PERF);
        int rc[kN + 2] = {};
        bool ok = load >= 0;
        if (ok)
        {
            rc[0] = scePerfArmPmonReset(SCE_PERF_ARM_PMON_THREAD_ID_SELF);
            for (int i = 0; i < kN; ++i)
                rc[1 + i] = scePerfArmPmonSelectEvent(SCE_PERF_ARM_PMON_THREAD_ID_SELF, static_cast<SceUInt32>(i), kEvent[i]);
            rc[kN + 1] = scePerfArmPmonStart(SCE_PERF_ARM_PMON_THREAD_ID_SELF);
            for (int i = 0; i < kN + 2; ++i)
                ok = ok && rc[i] >= 0;
        }
        std::fprintf(stderr, "[m1pmu] frame %llu: PMUSERENR 0x%08x (user access %s; 0xffffffff: not read), sysmodule perf 0x%08x, reset 0x%08x, select %08x %08x %08x %08x %08x %08x, start 0x%08x: %s\n",
                     frame, static_cast<unsigned>(userenr), userenr == 0xFFFFFFFFu ? "?" : (userenr & 1u) ? "on" : "off", static_cast<unsigned>(load), static_cast<unsigned>(rc[0]),
                     static_cast<unsigned>(rc[1]), static_cast<unsigned>(rc[2]), static_cast<unsigned>(rc[3]), static_cast<unsigned>(rc[4]),
                     static_cast<unsigned>(rc[5]), static_cast<unsigned>(rc[6]), static_cast<unsigned>(rc[7]), ok ? "counting" : "off");
        if (!ok)
            return false;
        for (int i = 0; i < kN; ++i)
        {
            g_last[i] = 0u;
            if (scePerfArmPmonGetCounterValue(SCE_PERF_ARM_PMON_THREAD_ID_SELF, static_cast<SceUInt32>(i), &g_last[i]) < 0)
                return false;
            g_sum[i] = 0u;
        }
        g_t0 = sceKernelGetProcessTimeWide();
        g_f0 = frame;
        return true;
    }
}
extern "C" void ps2x_m1_pmu_tick(unsigned long long frame)
{
    using namespace m1pmu;
    ps2x_p2_tick(frame); // round P2 (two loads and a branch when off)
    if (k3::g_want)
        k3::g_frame = frame; // round K3: the [k3] line's frame (one load and a branch when off)
    if (__builtin_expect(g_state < 0, 1))
        return;
    if (g_state == 0)
    {
        g_state = start(frame) ? 1 : -1;
        return;
    }
    uint32_t cyc = 0u;
    if (g_mode == 3)
    {
        if (!k2::tick(g_sum, g_sumCyc))
        {
            std::fprintf(stderr, "[m1pmu] frame %llu: K2 counters unreadable: off\n", frame);
            g_state = -1;
            return;
        }
    }
    else if (g_mode == 2)
    {
        // Round K1: the counters themselves. PMUSERENR is checked first every frame: a core reset (suspend / resume)
        // clears it, and a counter read without it would be an undefined instruction.
        if (__builtin_expect(!k1pmu::userOn(), 0))
        {
            std::fprintf(stderr, "[m1pmu] frame %llu: PMUSERENR cleared (suspend / resume?): K1 counting off\n", frame);
            g_state = -1;
            return;
        }
        SceUInt32 v[kN];
        k1pmu::snapshot(v, cyc);
        for (int i = 0; i < kN; ++i)
        {
            g_sum[i] += static_cast<uint32_t>(v[i] - g_last[i]);
            g_last[i] = v[i];
        }
        g_sumCyc += static_cast<uint32_t>(cyc - g_lastCyc);
        g_lastCyc = cyc;
    }
    else
        for (int i = 0; i < kN; ++i)
        {
            SceUInt32 v = 0u;
            if (scePerfArmPmonGetCounterValue(SCE_PERF_ARM_PMON_THREAD_ID_SELF, static_cast<SceUInt32>(i), &v) < 0)
            {
                std::fprintf(stderr, "[m1pmu] frame %llu: counter %d unreadable: off\n", frame, i);
                g_state = -1;
                return;
            }
            g_sum[i] += static_cast<SceUInt32>(v - g_last[i]); // 32-bit counters: read every frame, the growth summed
            g_last[i] = v;
        }
    const unsigned long long n = frame - g_f0;
    if (n < g_every)
        return;
    const uint64_t t = sceKernelGetProcessTimeWide();
    const double dn = static_cast<double>(n);
    if (g_mode == 3)
    {
        const double dv = k2::g_valid ? static_cast<double>(k2::g_valid) : 1.0;
        const double ms = static_cast<double>(t - g_t0) / 1000.0 / dn;
        const int mhz = scePowerGetArmClockFrequency();
        std::fprintf(stderr, "[m1pmu] frames %llu-%llu (%.2f ms/frame wall): per frame %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f, cycles %.0f "
                             "(= %.1f ms at %d MHz) (k2, game thread; %u of %llu frames counted, %u re-armed)\n",
                     g_f0, frame, ms, k1pmu::name(0), g_sum[0] / dv, k1pmu::name(1), g_sum[1] / dv, k1pmu::name(2), g_sum[2] / dv, k1pmu::name(3),
                     g_sum[3] / dv, k1pmu::name(4), g_sum[4] / dv, k1pmu::name(5), g_sum[5] / dv, static_cast<double>(g_sumCyc) / dv,
                     mhz > 0 ? static_cast<double>(g_sumCyc) / dv / (mhz * 1000.0) : 0.0, mhz, k2::g_valid, n, k2::g_lost);
        k2::g_valid = k2::g_lost = 0u;
    }
    else if (g_mode == 2)
        std::fprintf(stderr, "[m1pmu] frames %llu-%llu (%.2f ms/frame wall): per frame %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f, cycles %.0f (k1, core 0 counters)\n",
                     g_f0, frame, static_cast<double>(t - g_t0) / 1000.0 / dn, k1pmu::name(0), g_sum[0] / dn, k1pmu::name(1), g_sum[1] / dn,
                     k1pmu::name(2), g_sum[2] / dn, k1pmu::name(3), g_sum[3] / dn, k1pmu::name(4), g_sum[4] / dn, k1pmu::name(5), g_sum[5] / dn,
                     static_cast<double>(g_sumCyc) / dn);
    else
        std::fprintf(stderr, "[m1pmu] frames %llu-%llu (%.2f ms/frame wall): per frame %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f, %s %.0f\n",
                     g_f0, frame, static_cast<double>(t - g_t0) / 1000.0 / dn, kName[0], g_sum[0] / dn,
                     kName[1], g_sum[1] / dn, kName[2], g_sum[2] / dn, kName[3], g_sum[3] / dn,
                     kName[4], g_sum[4] / dn, kName[5], g_sum[5] / dn);
    for (int i = 0; i < kN; ++i)
        g_sum[i] = 0u;
    g_sumCyc = 0u;
    g_t0 = t;
    g_f0 = frame;
}


#endif // PLATFORM_VITA
