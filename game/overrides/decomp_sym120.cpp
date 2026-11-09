// Round 120 (SOTC_SYM120, A/B `sym120`): the module system's symbol
// resolution as plain A9 code. Every frame the sheet manager
// (sheetMgrResolve, 0x115d170, from the frame's resource step) walks its
// pending sheets and their entries, looks each entry's symbol up by its hash
// (isysSearchSymbol -> SearchStrHashKey in the global table at 0x202658) and
// tail-jumps into isysResolveAllProgramModule (0x1b63a8), which walks every
// program module and retries each module's still-unresolved imports
// (kernel_sub_001B4760: for every import whose link still holds the
// "undefined" mark *0x1e0ed4, a hash lookup; found: the link moved onto the
// symbol's reference list and the import given its value). Riding the attract
// demo these five functions were ~1.3% of the game thread (sampler), most of
// it lookups that fail again (the imports and sheets of modules not streamed
// in yet).
//
// The hash table: +0 its size (a power of two), +4 the buckets; an entry +0
// the key (the precomputed hash), +8 the symbol (+4 its value), +0xc the
// next entry in the bucket.
//
// Natively, exact: the same reads and RAM writes in the same order, the
// same guest calls (the group walk, the group add, the tick count's float,
// the module resolve, the tail jump) with the original's ra and frame sp,
// the return values the original leaves (v0; v1 where cheap). Not written:
// the callee-saved registers' spills in the natives' own frames (the dead
// stack below the caller's sp; no guest code runs between the spill and the
// reload in SearchStrHashKey / isysSearchSymbol / 001B4760, and the
// registers themselves are never changed). sheetMgrResolve's frame is
// written where its callees could look at it (sp[0], sp[4]) and its saved
// registers are spilled as the original does (its callees can run guest
// code that unwinds through it). The `break 0` of an entry with neither
// flag goes to the runtime's break handler as the recompiled code does.
// Module code: the jal sites and the boot ELF's addresses are read from the
// live words (relocated).
//
// SOTC_SYM120_LOG=1: "[sym120]" every 600 frames: calls, sheets, entries,
// lookups (found), imports looked at / retried / resolved.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_sym120
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kSearchKey = 0x001CBE70u, kSearchSymbol = 0x001B5F28u, kResolveImports = 0x001B4760u, kSheetResolve = 0x0115D170u;
    constexpr uint32_t kTable = 0x00202658u, kUndef = 0x001E0ED4u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
    };

    // ---- statistics (SOTC_SYM120_LOG=1) ------------------------------------
    struct Stats
    {
        uint64_t sheetCalls = 0, sheets = 0, entries = 0, lookups = 0, found = 0, modules = 0, imports = 0, retried = 0, resolved = 0;
    };
    Stats g_stats;
    int g_log = -1;
    bool logOn()
    {
        if (__builtin_expect(g_log < 0, 0))
        {
            const char *e = ps2_diag::env("SOTC_SYM120_LOG");
            g_log = e && std::atoi(e) != 0;
        }
        return g_log != 0;
    }

    // SearchStrHashKey(table, key): the bucket's chain. v0 the entry or 0; v1
    // / a0 as the walk leaves them.
    struct Search
    {
        uint32_t v0, v1, a0;
    };
    __attribute__((always_inline)) inline Search search(const Ram &ram, uint32_t table, uint32_t key)
    {
        const uint32_t size = ram.u32(table), buckets = ram.u32(table + 4u);
        const uint32_t slot = ((size - 1u) & key) * 4u + buckets;
        uint32_t e = ram.u32(slot);
        Search r{0u, buckets, e};
        // lw a0, 0(v0); beqz a0, out (v0 = 0); v1 = *(a0); beq a1, v1, out (v0 = a0); a0 = *(a0 + 0xc) ...
        while (e != 0u)
        {
            const uint32_t k = ram.u32(e);
            r.v1 = k;
            r.v0 = e;
            if (k == key)
                return r;
            e = ram.u32(e + 0xCu);
            r.a0 = e;
        }
        r.v0 = 0u;
        return r;
    }

    void searchKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kSym120))
        {
            ctx->pc = kSearchKey;
            return sotc_decomp::recompiled(kSearchKey)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        const Search r = search(ram, GPR_U32(ctx, 4), GPR_U32(ctx, 5));
        SET_GPR_U64(ctx, 2, sx(r.v0));
        SET_GPR_U64(ctx, 3, sx(r.v1));
        SET_GPR_U64(ctx, 4, sx(r.a0));
        sotc_decomp::returnTo(ctx);
    }

    // isysSearchSymbol(out, key): key 0: *out = 0, v0 = 1; else the lookup in
    // the global table, found: *out = the symbol's value; v0 = found.
    __attribute__((always_inline)) inline uint32_t searchSymbol(const Ram &ram, uint32_t out, uint32_t key, uint32_t &v1)
    {
        if (key == 0u)
        {
            ram.put32(out, 0u);
            return 1u;
        }
        const Search r = search(ram, kTable, key);
        v1 = r.v1;
        if (r.v0 != 0u)
        {
            v1 = ram.u32(ram.u32(r.v0 + 8u) + 4u);
            ram.put32(out, v1);
        }
        return r.v0 != 0u ? 1u : 0u;
    }

    void searchSymbolFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kSym120))
        {
            ctx->pc = kSearchSymbol;
            return sotc_decomp::recompiled(kSearchSymbol)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        uint32_t v1 = GPR_U32(ctx, 3);
        const uint32_t key = GPR_U32(ctx, 5);
        const uint32_t v0 = searchSymbol(ram, GPR_U32(ctx, 4), key, v1);
        SET_GPR_U64(ctx, 2, v0);
        if (key != 0u)
            SET_GPR_U64(ctx, 3, sx(v1));
        sotc_decomp::returnTo(ctx);
    }

    // kernel_sub_001B4760(module): its unresolved imports looked up again.
    // a0 +0x1c the import descriptor (+0x1c the count, +0x20 the index list,
    // +0x28 the import records, 16 bytes: +4 the value or the undefined mark,
    // +0xd resolved), +0x28 the links (16 bytes: +0 the key, +4 the record
    // it is on, +8 / +0xc the reference list's previous (tagged 0x80000000:
    // the symbol itself) / next).
    void resolveImports(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kSym120))
        {
            ctx->pc = kResolveImports;
            return sotc_decomp::recompiled(kResolveImports)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        const uint32_t module = GPR_U32(ctx, 4);
        const uint32_t desc = ram.u32(module + 0x1Cu);
        uint32_t count = ram.u32(desc + 0x1Cu);
        uint32_t resolvedCount = 0u;
        uint64_t v1 = sx(desc);
        if (count != 0u)
        {
            const uint32_t records = ram.u32(desc + 0x28u);
            uint32_t link = ram.u32(module + 0x28u);
            uint32_t index = ram.u32(desc + 0x20u);
            const bool stats = logOn();
            if (stats)
            {
                ++g_stats.modules;
                g_stats.imports += count;
            }
            for (uint32_t n = count; n != 0u; --n, index += 4u, link += 0x10u)
            {
                const uint32_t rec = records + (ram.u32(index) << 4);
                const uint32_t undef = ram.u32(kUndef);
                if (ram.u32(rec + 4u) != undef)
                    continue;
                if (stats)
                    ++g_stats.retried;
                const Search r = search(ram, kTable, ram.u32(link));
                const uint32_t sym = r.v0;
                if (sym == 0u)
                {
                    if (ram.u8(rec + 0xDu) != 0u)
                        ++resolvedCount;
                    continue;
                }
                const uint32_t value = ram.u32(ram.u32(sym + 8u) + 4u);
                // Off the reference list it is on.
                const uint32_t prev = ram.u32(link + 8u);
                if (prev != 0u)
                {
                    const uint32_t next = ram.u32(link + 0xCu);
                    if ((prev & 0x80000000u) != 0u)
                        ram.put32((prev & 0x7FFFFFFFu) + 4u, next);
                    else
                        ram.put32(prev + 0xCu, next);
                    if (next != 0u)
                        ram.put32(next + 8u, prev);
                }
                const uint32_t old = ram.u32(link + 4u);
                ram.put32(link + 0xCu, 0u);
                ram.put32(link + 8u, 0u);
                ram.put8(old + 0xDu, 1u);
                ram.put32(old + 4u, ram.u32(kUndef));
                // Onto the symbol's list, first.
                const uint32_t head = ram.u32(sym + 4u);
                ram.put32(link + 8u, sym | 0x80000000u);
                ram.put32(link + 0xCu, head);
                ram.put32(sym + 4u, link);
                if (head != 0u)
                    ram.put32(head + 8u, link);
                ram.put8(rec + 0xDu, 1u);
                ram.put32(rec + 4u, value);
                ++resolvedCount; // the byte just set
                if (stats)
                    ++g_stats.resolved;
            }
            v1 = ~0ull; // addiu $v1, $zero, -1 (the loop's test)
        }
        SET_GPR_U64(ctx, 2, resolvedCount);
        SET_GPR_U64(ctx, 3, v1);
        sotc_decomp::returnTo(ctx);
    }


    // MakeStrHashValue(s) (0x1cbdb8): 0 for a null string or the string at
    // 0x1e4608 (strcmp); else over the even characters, then the odd ones,
    // hash = hash * 77 + (c - 0x2d) (32-bit; strlen read again at every
    // step, as the original does, here once: the string is not written).
    // lo / hi as the last mult leaves them.
    constexpr uint32_t kMakeHash = 0x001CBDB8u, kHashSkip = 0x001E4608u;
    void makeHash(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kSym120))
        {
            ctx->pc = kMakeHash;
            return sotc_decomp::recompiled(kMakeHash)(rdram, ctx, runtime);
        }
        const uint32_t str = GPR_U32(ctx, 4);
        // A string outside main RAM (the scratchpad: the game builds names there): the original.
        if (str >= PS2_RAM_SIZE)
        {
            ctx->pc = kMakeHash;
            return sotc_decomp::recompiled(kMakeHash)(rdram, ctx, runtime);
        }
        const Ram ram{rdram};
        uint32_t hash = 0u;
        bool any = false;
        if (str != 0u)
        {
            uint32_t i = 0u;
            for (;; ++i)
            {
                const uint8_t a = ram.u8(kHashSkip + i), b = ram.u8(str + i);
                if (a != b)
                {
                    any = true;
                    break;
                }
                if (a == 0u)
                    break;
            }
        }
        if (any)
        {
            uint32_t len = 0u;
            while (ram.u8(str + len) != 0u)
                ++len;
            for (uint32_t pass = 0u; pass < 2u; ++pass)
                for (uint32_t i = pass; i < len; i += 2u)
                {
                    const int64_t prod = static_cast<int64_t>(static_cast<int32_t>(hash)) * 0x4D;
                    ctx->lo = sx(static_cast<uint32_t>(prod));
                    ctx->hi = sx(static_cast<uint32_t>(static_cast<uint64_t>(prod) >> 32));
                    hash = static_cast<uint32_t>(prod) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(ram.u8(str + i))) - 0x2D);
                }
            // The last pass's failing test is after one more mult.
            const int64_t prod = static_cast<int64_t>(static_cast<int32_t>(hash)) * 0x4D;
            ctx->lo = sx(static_cast<uint32_t>(prod));
            ctx->hi = sx(static_cast<uint32_t>(static_cast<uint64_t>(prod) >> 32));
            // The EE cycles the original's loop charges (a back edge and a call a character): the
            // game polls a module's names through this while the loader runs, which must see
            // the clock move (charging nothing left the boot waiting for good).
            (void)runtime->m_eeClock.charge(40u * (len + 2u));
        }
        static const bool check = ps2_diag::env("SOTC_SYM120_CHECK") && std::atoi(ps2_diag::env("SOTC_SYM120_CHECK")) != 0;
        if (check)
        {
            // The original on the same state (it writes nothing but its frame): v0 compared.
            static uint64_t n = 0, bad = 0;
            const uint64_t lo = ctx->lo, hi = ctx->hi;
            // To its return (its loop can leave for a checkpoint).
            sotc_decomp::callRecompiled(rdram, ctx, runtime, kMakeHash, GPR_U32(ctx, 31), GPR_U32(ctx, 29));
            ++n;
            if (GPR_U32(ctx, 2) != hash || ctx->lo != lo || ctx->hi != hi)
            {
                ++bad;
                if (bad <= 20u)
                {
                    char text[64] = {};
                    for (unsigned i = 0; i < 63u && ram.u8(str + i); ++i)
                        text[i] = static_cast<char>(ram.u8(str + i));
                    std::fprintf(stderr, "[sym120c] MakeStrHashValue %08x \"%s\": native %08x recompiled %08x lo %d hi %d\n", str, text, hash,
                                 GPR_U32(ctx, 2), ctx->lo == lo, ctx->hi == hi);
                }
            }
            if ((n % 5000u) == 0u)
                std::fprintf(stderr, "[sym120c] MakeStrHashValue checks %llu differ %llu\n", (unsigned long long)n, (unsigned long long)bad);
            return;
        }
        SET_GPR_U64(ctx, 2, sx(hash));
        sotc_decomp::returnTo(ctx);
    }

    // ---- sheetMgrResolve (0x115d170) ------------------------------------------
    // The jal sites (module code: read live).
    constexpr uint32_t kSiteTick1 = 0x0115D1C0u, kSiteSearch = 0x0115D204u, kSiteForFirst = 0x0115D294u, kSiteForNext = 0x0115D2A8u,
                       kSiteForExit = 0x0115D2D0u, kSiteAdd = 0x0115D2ECu, kSiteTick2 = 0x0115D360u, kSiteSeconds = 0x0115D36Cu,
                       kSiteResolveOne = 0x0115D390u, kSiteTail = 0x0115D428u;
    constexpr uint32_t kSheets = 0x0118FE28u, kModules = 0x0118FE24u, kKind = 0x0118FE10u; // lui 0x119 (same module)

    void sheetResolveBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        // The sites must still be the calls this code makes (module code moved or patched: the original).
        const uint32_t tTick = jalTarget(rdram, kSiteTick1), tSearch = jalTarget(rdram, kSiteSearch), tFirst = jalTarget(rdram, kSiteForFirst),
                       tNext = jalTarget(rdram, kSiteForNext), tExit = jalTarget(rdram, kSiteForExit), tAdd = jalTarget(rdram, kSiteAdd),
                       tTick2 = jalTarget(rdram, kSiteTick2), tSeconds = jalTarget(rdram, kSiteSeconds), tOne = jalTarget(rdram, kSiteResolveOne),
                       tTail = jalTarget(rdram, kSiteTail);
        const uint32_t undefAt = relocated(rdram, 0x0115D214u, 0x0115D218u);
        if (tTick != 0x001AD510u || tTick2 != 0x001AD510u || tSearch != kSearchSymbol || tFirst != 0x001C7110u || tNext != 0x001C76D0u ||
            tExit != 0x001C77B8u || tAdd != 0x001C6550u || tSeconds != 0x001AD550u || tOne != 0x001B6310u || tTail != 0x001B63A8u ||
            undefAt != kUndef)
        {
            static bool told = false;
            if (!told && logOn())
            {
                told = true;
                std::fprintf(stderr, "[sym120] sheetMgrResolve: a site is not the expected call (%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x undef %08x): recompiled\n",
                             tTick, tSearch, tFirst, tNext, tExit, tAdd, tTick2, tSeconds, tOne, tTail, undefAt);
            }
            ctx->pc = kSheetResolve;
            return sotc_decomp::recompiled(kSheetResolve)(rdram, ctx, runtime);
        }
        const bool stats = logOn();
        if (stats)
            ++g_stats.sheetCalls;

        const uint32_t sp = GPR_U32(ctx, 29) - 0x60u;
        SET_GPR_U32(ctx, 29, sp);
        // The spills (the callees run guest code below this frame).
        ram.put64(sp + 0x18u, GPR_U64(ctx, 17));
        ram.put64(sp + 0x10u, GPR_U64(ctx, 16));
        ram.put64(sp + 0x20u, GPR_U64(ctx, 18));
        ram.put64(sp + 0x28u, GPR_U64(ctx, 19));
        ram.put64(sp + 0x30u, GPR_U64(ctx, 20));
        ram.put64(sp + 0x38u, GPR_U64(ctx, 21));
        ram.put64(sp + 0x40u, GPR_U64(ctx, 22));
        ram.put64(sp + 0x48u, GPR_U64(ctx, 23));
        ram.put64(sp + 0x50u, GPR_U64(ctx, 30));
        ram.put64(sp + 0x58u, GPR_U64(ctx, 31));
        ram.put32(sp + 0u, GPR_U32(ctx, 4));

        // The callee-saved registers as the loop keeps them (written to ctx
        // before each guest call, which may read them only through the frame).
        auto setS = [&](uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, uint32_t s4, uint32_t s5, uint32_t s6, uint32_t s7, uint32_t fp)
        {
            SET_GPR_U64(ctx, 16, sx(s0));
            SET_GPR_U64(ctx, 17, sx(s1));
            SET_GPR_U64(ctx, 18, sx(s2));
            SET_GPR_U64(ctx, 19, sx(s3));
            SET_GPR_U64(ctx, 20, sx(s4));
            SET_GPR_U64(ctx, 21, sx(s5));
            SET_GPR_U64(ctx, 22, sx(s6));
            SET_GPR_U64(ctx, 23, sx(s7));
            SET_GPR_U64(ctx, 30, sx(fp));
        };

        uint32_t s1 = ram.u32(kSheets);
        uint32_t s0 = GPR_U32(ctx, 16), s2 = GPR_U32(ctx, 18), s3 = GPR_U32(ctx, 19), s4 = GPR_U32(ctx, 20), s5 = GPR_U32(ctx, 21);
        uint32_t s6 = GPR_U32(ctx, 22), s7 = GPR_U32(ctx, 23), fp = GPR_U32(ctx, 30);
        uint64_t fp64 = GPR_U64(ctx, 30);
        while (s1 != 0u)
        {
            // 0x115d1ac
            const uint32_t next = ram.u32(s1 + 0x58u);
            s6 = 0u;
            s4 = ram.u32(s1 + 0x38u);
            s7 = 0u;
            ram.put32(sp + 4u, next);
            if (stats)
                ++g_stats.sheets;
            // iosGetTCount (a native: the EE clock's bus ticks); s0 set in its delay slot.
            s0 = ram.u32(s4 + 0x1Cu);
            setS(s0, s1, s2, s3, s4, s5, s6, s7, fp);
            SET_GPR_U64(ctx, 30, fp64);
            callGuest(rdram, ctx, runtime, tTick, kSiteTick1 + 8u, sp);
            const uint64_t t0 = GPR_U64(ctx, 2);
            fp64 = t0;
            fp = static_cast<uint32_t>(t0);
            int32_t a0 = static_cast<int32_t>(ram.u32(s4 + 0x18u));
            if (a0 > 0)
            {
                s2 = s0;
                for (;;)
                {
                    // 0x115d1d8
                    bool reload = false; // a0 = *(s4 + 0x18) again on the way to the test
                    const uint32_t key = ram.u32(s2 + 4u);
                    if (stats)
                        ++g_stats.entries;
                    if (key != 0u)
                    {
                        const uint32_t kind = (ram.u32(s2 + 8u) >> 11) & 0xFu;
                        if (kind == 0u || kind == ram.u32(kKind))
                        {
                            uint32_t v1 = 0u;
                            const uint32_t ok = searchSymbol(ram, s2, key, v1);
                            if (stats)
                            {
                                ++g_stats.lookups;
                                g_stats.found += ok;
                            }
                            bool add = false;
                            if (ok == 0u)
                            {
                                ram.put32(s2, ram.u32(kUndef));
                                ++s7;
                                const uint32_t flags = ram.u32(s1 + 0x44u);
                                if (((flags >> 19) & 1u) != 0u)
                                    reload = true;
                                else
                                {
                                    if (((ram.u32(s2 + 8u) >> 10) & 1u) == 0u)
                                    {
                                        setS(s0, s1, s2, s3, s4, s5, s6, s7, fp);
                                        SET_GPR_U64(ctx, 30, fp64);
                                        ctx->pc = 0x0115D248u;
                                        runtime->handleBreak(rdram, ctx);
                                    }
                                    a0 = static_cast<int32_t>(ram.u32(s4 + 0x18u));
                                }
                            }
                            else
                            {
                                const uint32_t flags = ram.u32(s1 + 0x44u);
                                if (((flags >> 19) & 1u) != 0u)
                                    add = true;
                                else if (((ram.u32(s2 + 8u) >> 10) & 1u) != 0u)
                                    add = true;
                                else
                                    a0 = static_cast<int32_t>(ram.u32(s4 + 0x18u));
                            }
                            if (add)
                            {
                                // 0x115d27c: the sheet in the symbol's object group unless it is there.
                                const uint32_t obj = ram.u32(s2);
                                s5 = 0u;
                                s0 = 0u;
                                s3 = ram.u32(obj + 0x14u) + 0x48u;
                                setS(s0, s1, s2, s3, s4, s5, s6, s7, fp);
                                SET_GPR_U64(ctx, 30, fp64);
                                SET_GPR_U64(ctx, 4, sx(s3));
                                SET_GPR_U64(ctx, 5, 0u);
                                callGuest(rdram, ctx, runtime, tFirst, kSiteForFirst + 8u, sp);
                                uint32_t v0 = GPR_U32(ctx, 2);
                                while (v0 != 0u)
                                {
                                    s0 = ram.u32(v0);
                                    SET_GPR_U64(ctx, 4, sx(v0));
                                    if (s1 == s0)
                                    {
                                        s5 = 1u;
                                        break;
                                    }
                                    SET_GPR_U64(ctx, 16, sx(s0));
                                    SET_GPR_U64(ctx, 5, 0u);
                                    callGuest(rdram, ctx, runtime, tNext, kSiteForNext + 8u, sp);
                                    v0 = GPR_U32(ctx, 2);
                                }
                                SET_GPR_U64(ctx, 16, sx(s0));
                                SET_GPR_U64(ctx, 21, sx(s5));
                                if (s0 != 0u)
                                {
                                    SET_GPR_U64(ctx, 4, sx(s0));
                                    SET_GPR_U64(ctx, 5, 0u);
                                    callGuest(rdram, ctx, runtime, tExit, kSiteForExit + 8u, sp);
                                }
                                if (s5 != 0u)
                                    a0 = static_cast<int32_t>(ram.u32(s4 + 0x18u));
                                else
                                {
                                    SET_GPR_U64(ctx, 4, sx(s1));
                                    SET_GPR_U64(ctx, 5, sx(s3));
                                    SET_GPR_U64(ctx, 6, 0u);
                                    SET_GPR_U64(ctx, 7, 0u);
                                    callGuest(rdram, ctx, runtime, tAdd, kSiteAdd + 8u, sp);
                                    reload = true;
                                }
                            }
                        }
                    }
                    if (reload)
                        a0 = static_cast<int32_t>(ram.u32(s4 + 0x18u));
                    ++s6;
                    s2 += 0xCu;
                    if (!(static_cast<int32_t>(s6) < a0))
                        break;
                }
            }
            // 0x115d308: a sheet with every entry found leaves the list (when +0x5c is set).
            if (s7 == 0u)
            {
                const uint32_t back = ram.u32(s1 + 0x5Cu);
                if (back != 0u)
                {
                    const uint32_t fwd = ram.u32(s1 + 0x58u);
                    if (ram.u32(kSheets) == s1)
                    {
                        ram.put32(kSheets, fwd);
                        if (fwd != 0u)
                            ram.put32(fwd + 0x5Cu, 0xFFFFFFFFu);
                    }
                    else
                    {
                        ram.put32(back + 0x58u, fwd);
                        if (fwd != 0u)
                            ram.put32(fwd + 0x5Cu, ram.u32(s1 + 0x5Cu));
                    }
                    ram.put32(s1 + 0x5Cu, 0u);
                    ram.put32(s1 + 0x58u, 0u);
                }
            }
            // 0x115d360: iosGetSecondFromTCount(iosGetTCount() - start), its value unused.
            setS(s0, s1, s2, s3, s4, s5, s6, s7, fp);
            SET_GPR_U64(ctx, 30, fp64);
            callGuest(rdram, ctx, runtime, tTick2, kSiteTick2 + 8u, sp);
            const uint64_t t1 = GPR_U64(ctx, 2);
            s1 = ram.u32(sp + 4u);
            SET_GPR_U64(ctx, 17, sx(s1));
            SET_GPR_U64(ctx, 4, t1 - fp64);
            callGuest(rdram, ctx, runtime, tSeconds, kSiteSeconds + 8u, sp);
        }

        // 0x115d37c: the pending modules, each resolved once more; resolved
        // ones with a +0x48 back link leave the list.
        s0 = ram.u32(kModules);
        s3 = 0xFFFFFFFFu;
        while (s0 != 0u)
        {
            s1 = ram.u32(s0 + 0x44u);
            setS(s0, s1, s2, s3, s4, s5, s6, s7, fp);
            SET_GPR_U64(ctx, 30, fp64);
            SET_GPR_U64(ctx, 4, sx(s0));
            callGuest(rdram, ctx, runtime, tOne, kSiteResolveOne + 8u, sp);
            if (GPR_U32(ctx, 2) == 0u)
            {
                const uint32_t back = ram.u32(s0 + 0x48u);
                if (back != 0u)
                {
                    const uint32_t fwd = ram.u32(s0 + 0x44u);
                    if (ram.u32(kModules) == s0)
                    {
                        ram.put32(kModules, fwd);
                        if (fwd != 0u)
                            ram.put32(fwd + 0x48u, s3);
                    }
                    else
                    {
                        ram.put32(back + 0x44u, fwd);
                        if (fwd != 0u)
                            ram.put32(fwd + 0x48u, ram.u32(s0 + 0x48u));
                    }
                    ram.put32(s0 + 0x48u, 0u);
                    ram.put32(s0 + 0x44u, 0u);
                }
            }
            s0 = s1;
        }

        // 0x115d3f8: the frame popped; with a0 set the tail jump to isysResolveAllProgramModule.
        const uint32_t arg = ram.u32(sp + 0u);
        SET_GPR_U64(ctx, 2, sx(arg));
        SET_GPR_U64(ctx, 16, ram.u64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, ram.u64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, ram.u64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, ram.u64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, ram.u64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, ram.u64(sp + 0x38u));
        SET_GPR_U64(ctx, 22, ram.u64(sp + 0x40u));
        SET_GPR_U64(ctx, 23, ram.u64(sp + 0x48u));
        SET_GPR_U64(ctx, 30, ram.u64(sp + 0x50u));
        SET_GPR_U64(ctx, 31, ram.u64(sp + 0x58u));
        SET_GPR_U32(ctx, 29, sp + 0x60u);
        if (arg != 0u)
        {
            const uint32_t ra = GPR_U32(ctx, 31);
            callGuest(rdram, ctx, runtime, tTail, ra, sp + 0x60u);
            ctx->pc = ra;
            return;
        }
        ctx->pc = GPR_U32(ctx, 31);

    }
    // The call, timed with SOTC_SYM120_LOG=1 (the tail into
    // isysResolveAllProgramModule included), native or recompiled by the
    // switch (the A/B's windows): "[sym120]" every 60 calls.
    inline uint64_t nowUs()
    {
#if defined(__vita__)
        return sceKernelGetProcessTimeWide();
#else
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
    }
    struct Timing
    {
        uint64_t n[2] = {}, us[2] = {}, max[2] = {};
    };
    Timing g_time;
    void sheetResolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool on = sotc_decomp::switchOn(sotc_decomp::kSym120);
        const bool log = logOn();
        const uint64_t t0 = log ? nowUs() : 0u;
        if (on)
            sheetResolveBody(rdram, ctx, runtime);
        else
        {
            ctx->pc = kSheetResolve;
            sotc_decomp::recompiled(kSheetResolve)(rdram, ctx, runtime);
        }
        if (!log)
            return;
        const uint64_t us = nowUs() - t0;
        g_time.n[on] += 1u;
        g_time.us[on] += us;
        g_time.max[on] = us > g_time.max[on] ? us : g_time.max[on];
        if (g_time.n[0] + g_time.n[1] >= 60u)
        {
            const Stats &st = g_stats;
            const double c = st.sheetCalls ? static_cast<double>(st.sheetCalls) : 1.0;
            std::fprintf(stderr,
                         "[sym120] 60 calls: native %llu avg %.0f us max %llu | recompiled %llu avg %.0f us max %llu | native calls: sheets %.1f entries %.1f "
                         "lookups %.1f (found %.1f) modules %.1f imports %.1f retried %.1f resolved %.2f\n",
                         (unsigned long long)g_time.n[1], g_time.n[1] ? static_cast<double>(g_time.us[1]) / g_time.n[1] : 0.0, (unsigned long long)g_time.max[1],
                         (unsigned long long)g_time.n[0], g_time.n[0] ? static_cast<double>(g_time.us[0]) / g_time.n[0] : 0.0, (unsigned long long)g_time.max[0],
                         st.sheets / c, st.entries / c, st.lookups / c, st.found / c, st.modules / c, st.imports / c, st.retried / c, st.resolved / c);
            g_time = Timing{};
            g_stats = Stats{};
        }
    }
}

SOTC_DECOMP_FUNCTION(0x001CBE70u, SearchStrHashKey, decomp_sym120::searchKey);
SOTC_DECOMP_FUNCTION(0x001B5F28u, isysSearchSymbol, decomp_sym120::searchSymbolFn);
SOTC_DECOMP_FUNCTION(0x001B4760u, kernel_sub_001B4760, decomp_sym120::resolveImports);
SOTC_DECOMP_FUNCTION(0x0115D170u, sheetMgrResolve, decomp_sym120::sheetResolve);
SOTC_DECOMP_FUNCTION(0x001CBDB8u, MakeStrHashValue, decomp_sym120::makeHash);
