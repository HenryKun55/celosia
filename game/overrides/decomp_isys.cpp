// The kernel's group links (round 11): isysAddGroupWithLinkParam (and its
// NoWait entry, which tail-jumps into it), kernel_sub_001C6180 (a link put
// into a group's list), kernel_sub_001C6408 (a link taken out, back to the
// free list), isysGroupForFirst, isysGroupForExit and iosDlLockObj (a
// display list's object lock: a group link per drawn object). ~1.8% of the
// calibrated Vita3K game thread on the route's walk (the display walk, the
// collision's group walks).
//
// Links are 0x18-byte records in a table at 0x21b900, named by u16
// indices (0: none; index = (address - base) / 0x18, as the code computes
// it: ((address - base) >> 3) * 0xaaaaaaab): +0 the object, +4 the group,
// +8 the link's word, +0xc a byte (set: being removed), +0xe / +0x10 the
// group list's next / previous (+0xe is also the free list's next), +0x12 /
// +0x14 the object's list next / previous. A group: +0 / +2 its first and
// last link. An object: +0xc / +0xe its first and last link, +9 a walk
// count, +0xa flags. The free list's head is *0x26d980.
//
// The lock: a per-thread recursive one (owner *0x1e130c, depth *0x1e1310,
// semaphore *0x26d98c). These run natively only when the calling thread
// owns it already (the display walk and the collision walks do): the depth
// goes up and back down, no semaphore call. Otherwise (and for anything
// that would write outside the table's sane state: an empty free list, a
// zero group or object, a broken list) the recompiled function runs from
// its start, nothing written before; GetThreadId (no side effects) is all
// that ran. iosDlLockObj's fallback sets the registers and frame its
// recompiled code has at that call (its return label may resume there
// after a blocking WaitSema).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "ps2_syscalls.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_isys
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kAdd = 0x001C6550u, kAddNoWait = 0x001C7668u, kInsert = 0x001C6180u, kRemove = 0x001C6408u, kForFirst = 0x001C7110u,
                       kForExit = 0x001C77B8u, kLockObj = 0x001C00C8u;
    constexpr uint32_t kBase = 0x0021B900u, kOwner = 0x001E130Cu, kDepth = 0x001E1310u, kFree = 0x0026D980u;

    struct M
    {
        uint8_t *rdram;
        static bool ok(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE - n; }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put16(uint32_t a, uint16_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 2); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
    };

    inline uint16_t indexOf(uint32_t link)
    {
        if (link == 0u)
            return 0u;
        return static_cast<uint16_t>(static_cast<uint32_t>(static_cast<int32_t>(link - kBase) >> 3) * 0xAAAAAAABu);
    }
    inline uint32_t linkAt(uint16_t index) { return index == 0u ? 0u : kBase + static_cast<uint32_t>(index) * 0x18u; }
    // A link address the code could form (in the table's range, main RAM).
    inline bool sane(uint32_t link) { return link >= kBase && link < kBase + 0x10000u * 0x18u && M::ok(link, 0x18u); }

    // SOTC_ISYS_STATS=1 (measuring): how the lock was found, every 100 000 calls.
    void stat(int how)
    {
        static const bool on = std::getenv("SOTC_ISYS_STATS") != nullptr;
        if (!on)
            return;
        static uint64_t n[3] = {};
        ++n[how];
        if ((n[0] + n[1] + n[2]) % 100000u == 0u)
            std::fprintf(stderr, "[isys] nested %llu taken %llu recompiled %llu\n", (unsigned long long)n[0], (unsigned long long)n[1], (unsigned long long)n[2]);
    }

    constexpr uint32_t kSemaWord = 0x0026D98Cu, kSignalSema = 0x00107000u;
    enum class Lock
    {
        Nested, // ours already: depth up
        Taken,  // free: the semaphore taken (PollSema, as WaitSema does when it can), owner set, depth 1
        No      // another thread's: the recompiled code (it waits); nothing changed
    };
    Lock acquire(const M &m, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ps2_syscalls::GetThreadId(rdram, ctx, runtime);
        const uint32_t tid = GPR_U32(ctx, 2);
        if (tid == m.u32(kOwner))
        {
            const int32_t depth = static_cast<int32_t>(m.u32(kDepth));
            if (depth < 1)
            {
                stat(2);
                return Lock::No;
            }
            m.put32(kDepth, static_cast<uint32_t>(depth + 1));
            stat(0);
            return Lock::Nested;
        }
        SET_GPR_U32(ctx, 4, m.u32(kSemaWord));
        ps2_syscalls::PollSema(rdram, ctx, runtime);
        if (static_cast<int32_t>(GPR_U32(ctx, 2)) < 0)
        {
            stat(2);
            return Lock::No;
        }
        m.put32(kOwner, tid);
        m.put32(kDepth, 1u);
        stat(1);
        return Lock::Taken;
    }
    // Depth down; true when it reached 0: the owner cleared, the caller
    // signals the semaphore (signal()).
    bool release(const M &m)
    {
        const uint32_t depth = m.u32(kDepth) - 1u;
        m.put32(kDepth, depth);
        if (depth != 0u)
            return false;
        m.put32(kOwner, 0xFFFFFFFFu);
        return true;
    }
    // SignalSema as the original's jal at (ra - 8) from its frame: a woken
    // thread can take over there (the scheduler's transfer), and this thread
    // later goes on in the recompiled function at ra, from the frame and
    // registers the caller set up for it.
    void signal(const M &m, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 4, m.u32(kSemaWord));
        sotc_decomp::callGuest(rdram, ctx, runtime, kSignalSema, ra, sp);
    }
    // The callee-saved registers s0..s(n-1) and ra into a frame as a prologue
    // writes them (s at +8 i, ra at raAt).
    void saveFrame(uint8_t *rdram, const R5900Context *ctx, uint32_t sp, int n, uint32_t raAt)
    {
        for (int i = 0; i < n; ++i)
        {
            const uint64_t v = GPR_U64(ctx, 16 + i);
            std::memcpy(rdram + ((sp + 8u * i) & PS2_RAM_MASK), &v, 8);
        }
        const uint64_t r = GPR_U64(ctx, 31);
        std::memcpy(rdram + ((sp + raAt) & PS2_RAM_MASK), &r, 8);
    }

    // kernel_sub_001C6180 with a2 = 1 (after `prev`; 0: the group's only
    // link): the link put into its group's list.
    void insertAfter(const M &m, uint32_t prev, uint32_t link)
    {
        if (prev == 0u)
        {
            const uint32_t group = m.u32(link + 4u);
            m.put16(group, indexOf(link));
            m.put16(group + 2u, indexOf(link));
            m.put16(link + 0x10u, 0u);
            m.put16(link + 0xEu, 0u);
            return;
        }
        m.put16(link + 0x10u, indexOf(prev));
        const uint16_t next = m.u16(prev + 0xEu);
        m.put16(link + 0xEu, next);
        if (next == 0u)
            m.put16(m.u32(prev + 4u) + 2u, indexOf(link));
        else
            m.put16(linkAt(next) + 0x10u, indexOf(link));
        m.put16(prev + 0xEu, indexOf(link));
    }

    // The add's checks (reads only): the link it will take (the free list's
    // head), 0 when the recompiled code must run (an empty free list, a zero
    // or odd object / group, a broken list: its error paths).
    uint32_t addPlan(const M &m, uint32_t object, uint32_t group)
    {
        const uint32_t link = m.u32(kFree);
        if (!sane(link) || object == 0u || group == 0u || !M::ok(object, 0x10u) || !M::ok(group, 4u))
            return 0u;
        const uint32_t last = linkAt(m.u16(group + 2u));
        if (last != 0u)
        {
            if (!sane(last) || !M::ok(m.u32(last + 4u), 4u))
                return 0u;
            if (const uint16_t n = m.u16(last + 0xEu); n != 0u && !sane(linkAt(n)))
                return 0u;
        }
        if (m.u16(object + 0xCu) != 0u && !sane(linkAt(m.u16(object + 0xEu))))
            return 0u;
        return link;
    }

    // The add's writes, in the original's order.
    void addDo(const M &m, uint32_t link, uint32_t object, uint32_t group, uint32_t word)
    {
        m.put32(kFree, linkAt(m.u16(link + 0xEu)));
        m.put32(link + 8u, word);
        m.put32(link, object);
        m.put32(link + 4u, group);
        insertAfter(m, linkAt(m.u16(group + 2u)), link);
        if (m.u16(object + 0xCu) == 0u)
        {
            m.put16(link + 0x14u, 0u);
            m.put16(object + 0xCu, indexOf(link));
        }
        else
        {
            m.put16(link + 0x14u, m.u16(object + 0xEu));
            m.put16(linkAt(m.u16(object + 0xEu)) + 0x12u, indexOf(link));
        }
        m.put16(link + 0x12u, 0u);
        m.put16(object + 0xEu, indexOf(link));
    }

    void addGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t object = GPR_U32(ctx, 4), group = GPR_U32(ctx, 5), word = GPR_U32(ctx, 7);
        const uint32_t spIn = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        const uint32_t link = addPlan(m, object, group);
        const Lock lock = link == 0u ? Lock::No : acquire(m, rdram, ctx, runtime);
        if (lock == Lock::No)
        {
            ctx->pc = kAdd;
            SET_GPR_U32(ctx, 4, object);
            return sotc_decomp::recompiled(kAdd)(rdram, ctx, runtime);
        }
        addDo(m, link, object, group, word);
        if (release(m))
        {
            // The original's frame (sp - 0x40: s0..s6 at 0..0x30, ra at 0x38), s0 the link.
            const uint32_t frame = spIn - 0x40u;
            saveFrame(rdram, ctx, frame, 7, 0x38u);
            const uint64_t s0 = GPR_U64(ctx, 16);
            SET_GPR_U32(ctx, 16, link);
            signal(m, rdram, ctx, runtime, 0x001C6784u, frame);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U32(ctx, 29, spIn);
            SET_GPR_U32(ctx, 31, ra);
        }
        SET_GPR_U64(ctx, 2, 1u);
        returnTo(ctx);
    }

    // isysAddGroupWithLinkParamNoWait: j isysAddGroupWithLinkParam (its own
    // frame popped first), to the native when the table holds it.
    void addGroupNoWait(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = kAdd;
        if (runtime->guestFunctionAt(kAdd) == &addGroup)
            return addGroup(rdram, ctx, runtime);
        sotc_decomp::callGuest(rdram, ctx, runtime, kAdd, GPR_U32(ctx, 31), GPR_U32(ctx, 29));
    }

    void insert(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t prev = GPR_U32(ctx, 4), link = GPR_U32(ctx, 5), mode = GPR_U32(ctx, 6);
        // Mode 1 (after prev) is the add's; the rest (and odd links) recompiled.
        if (GPR_U64(ctx, 6) != 1u || mode != 1u || !sane(link) || (prev != 0u && !sane(prev)))
        {
            ctx->pc = kInsert;
            return sotc_decomp::recompiled(kInsert)(rdram, ctx, runtime);
        }
        if (prev == 0u && !M::ok(m.u32(link + 4u), 4u))
        {
            ctx->pc = kInsert;
            return sotc_decomp::recompiled(kInsert)(rdram, ctx, runtime);
        }
        if (prev != 0u)
        {
            const uint16_t n = m.u16(prev + 0xEu);
            if ((n == 0u && !M::ok(m.u32(prev + 4u), 4u)) || (n != 0u && !sane(linkAt(n))))
            {
                ctx->pc = kInsert;
                return sotc_decomp::recompiled(kInsert)(rdram, ctx, runtime);
            }
        }
        insertAfter(m, prev, link);
        // v0 as the original leaves it: the link's index product (mult's
        // 32-bit result, sign-extended).
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(static_cast<int32_t>(link - kBase) >> 3) * 0xAAAAAAABu));
        returnTo(ctx);
    }

    // kernel_sub_001C6408: a link taken out of its group's and its object's
    // lists and pushed on the free list (+0 zeroed, +0xc cleared). An object
    // 0, or an index out of the table: the recompiled code.
    void remove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t link = GPR_U32(ctx, 4);
        auto recompiled = [&]()
        {
            ctx->pc = kRemove;
            return sotc_decomp::recompiled(kRemove)(rdram, ctx, runtime);
        };
        if (!sane(link))
            return recompiled();
        const uint32_t group = m.u32(link + 4u), object = m.u32(link);
        const uint16_t next = m.u16(link + 0xEu), prev = m.u16(link + 0x10u), objNext = m.u16(link + 0x12u), objPrev = m.u16(link + 0x14u);
        if (object == 0u || !M::ok(object, 0x10u) || (group != 0u && !M::ok(group, 4u)) || (next && !sane(linkAt(next))) || (prev && !sane(linkAt(prev))) ||
            (objNext && !sane(linkAt(objNext))) || (objPrev && !sane(linkAt(objPrev))))
            return recompiled();
        if (next != 0u)
            m.put16(linkAt(next) + 0x10u, m.u16(link + 0x10u));
        else if (group != 0u)
            m.put16(group + 2u, m.u16(link + 0x10u));
        if (const uint16_t p = m.u16(link + 0x10u); p != 0u)
            m.put16(linkAt(p) + 0xEu, m.u16(link + 0xEu));
        else if (group != 0u)
            m.put16(group, m.u16(link + 0xEu));
        if (const uint16_t n = m.u16(link + 0x12u); n != 0u)
            m.put16(linkAt(n) + 0x14u, m.u16(link + 0x14u));
        else
            m.put16(object + 0xEu, m.u16(link + 0x14u));
        if (const uint16_t p = m.u16(link + 0x14u); p != 0u)
            m.put16(linkAt(p) + 0x12u, m.u16(link + 0x12u));
        else
            m.put16(object + 0xCu, m.u16(link + 0x12u));
        m.put32(link, 0u);
        m.put8(link + 0xCu, 0u);
        const uint32_t head = m.u32(kFree);
        const int32_t product = head == 0u ? 0 : static_cast<int32_t>(static_cast<uint32_t>(static_cast<int32_t>(head - kBase) >> 3) * 0xAAAAAAABu);
        m.put16(link + 0xEu, static_cast<uint16_t>(product));
        m.put32(kFree, link);
        SET_GPR_S32(ctx, 2, product);
        returnTo(ctx);
    }

    void forFirst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t group = GPR_U32(ctx, 4), spIn = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        auto recompiled = [&]()
        {
            ctx->pc = kForFirst;
            SET_GPR_U32(ctx, 4, group);
            return sotc_decomp::recompiled(kForFirst)(rdram, ctx, runtime);
        };
        if (!M::ok(group, 2u))
            return recompiled();
        // A dry walk first (reads only): every link and object sane.
        uint32_t found = 0u;
        for (uint32_t link = linkAt(m.u16(group)), steps = 0; link != 0u; link = linkAt(m.u16(link + 0xEu)))
        {
            if (!sane(link) || ++steps > 0x10000u)
                return recompiled();
            if (m.u8(link + 0xCu) != 0u)
                continue;
            const uint32_t object = m.u32(link);
            if (!M::ok(object, 0x10u))
                return recompiled();
            if ((m.u16(object + 0xAu) & 1u) != 0u)
                continue;
            found = link;
            break;
        }
        if (acquire(m, rdram, ctx, runtime) == Lock::No)
            return recompiled();
        if (found != 0u)
        {
            const uint32_t object = m.u32(found);
            m.put8(object + 9u, static_cast<uint8_t>(m.u8(object + 9u) + 1u));
        }
        if (release(m))
        {
            // The frame (sp - 0x30: s0..s3, ra at 0x20), s0 the result.
            const uint32_t frame = spIn - 0x30u;
            saveFrame(rdram, ctx, frame, 4, 0x20u);
            const uint64_t s0 = GPR_U64(ctx, 16);
            SET_GPR_U32(ctx, 16, found);
            signal(m, rdram, ctx, runtime, 0x001C723Cu, frame);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U32(ctx, 29, spIn);
            SET_GPR_U32(ctx, 31, ra);
        }
        SET_GPR_U64(ctx, 2, found);
        returnTo(ctx);
    }

    void forExit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t object = GPR_U32(ctx, 4), spIn = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        if (!M::ok(object, 0x10u) || acquire(m, rdram, ctx, runtime) == Lock::No)
        {
            ctx->pc = kForExit;
            SET_GPR_U32(ctx, 4, object);
            return sotc_decomp::recompiled(kForExit)(rdram, ctx, runtime);
        }
        const uint32_t b = m.u8(object + 9u);
        m.put8(object + 9u, static_cast<uint8_t>(b - 1u));
        // v0: lbu + 0xff (addiu, not wrapped to a byte).
        SET_GPR_U64(ctx, 2, b + 0xFFu);
        if (release(m))
            signal(m, rdram, ctx, runtime, ra, spIn); // the original's tail jump (j SignalSema)
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    // iosDlLockObj (0x1c00c8): isysObjDeleteLock (the object's +8 bits 17..
    // counted up), the object put into the display list's current group
    // (0x2132f8 + 4 x *0x2132f0) with link word 0, the group's count
    // (0x213300 + 4 x *0x2132f0) up; v0 the new count.
    void lockObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const M m{rdram};
        const uint32_t object = GPR_U32(ctx, 4);
        constexpr uint32_t kDl = 0x002132D8u;
        if (!M::ok(object, 0x10u))
        {
            ctx->pc = kLockObj;
            return sotc_decomp::recompiled(kLockObj)(rdram, ctx, runtime);
        }
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x20u;
        // isysObjDeleteLock.
        const uint32_t w = m.u32(object + 8u);
        m.put32(object + 8u, (w & 0xFFFFu) | (((w >> 17) + 1u) << 17));
        const uint32_t group = (m.u32(kDl + 0x18u) << 2) + kDl + 0x20u;
        // The add as the jal at 0x1c0100 makes it (through NoWait's j: same
        // ra and sp), from the frame and registers the original has there
        // (its return label 0x1c0108 can be where this thread goes on after a
        // wait in the add).
        const uint64_t s0 = GPR_U64(ctx, 16), s1 = GPR_U64(ctx, 17);
        saveFrame(rdram, ctx, sp, 2, 0x10u);
        SET_GPR_U32(ctx, 16, kDl);
        SET_GPR_U32(ctx, 17, object);
        SET_GPR_U32(ctx, 4, object);
        SET_GPR_U32(ctx, 5, group);
        SET_GPR_U64(ctx, 6, 0u);
        SET_GPR_U64(ctx, 7, 0u);
        sotc_decomp::callGuest(rdram, ctx, runtime, kAdd, 0x001C0108u, sp);
        SET_GPR_U64(ctx, 16, s0);
        SET_GPR_U64(ctx, 17, s1);
        const uint32_t counter = (m.u32(kDl + 0x18u) << 2) + kDl + 0x28u;
        const uint32_t count = m.u32(counter) + 1u;
        m.put32(counter, count);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(count));
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        returnTo(ctx);
    }

    SOTC_DECOMP_FUNCTION(kAdd, isysAddGroupWithLinkParam, addGroup);
    SOTC_DECOMP_FUNCTION(kAddNoWait, isysAddGroupWithLinkParamNoWait, addGroupNoWait);
    SOTC_DECOMP_FUNCTION(kInsert, kernel_sub_001C6180, insert);
    SOTC_DECOMP_FUNCTION(kRemove, kernel_sub_001C6408, remove);
    SOTC_DECOMP_FUNCTION(kForFirst, isysGroupForFirst, forFirst);
    SOTC_DECOMP_FUNCTION(kForExit, isysGroupForExit, forExit);
    SOTC_DECOMP_FUNCTION(kLockObj, iosDlLockObj, lockObj);
}
