// Round 48 (SOTC_ISYS=0 puts all of them back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): the isys object lifetime,
// the boot ELF's object / group / letter system.
//
// The group links (decomp_isys.cpp, round 11): 0x18-byte records in a table
// at 0x21b900 named by u16 indices (0: none; index = ((address - base) >> 3)
// * 0xaaaaaaab, a mult): +0 the object, +4 the group, +0xc a byte (1: being
// removed), +0xe / +0x10 the group list's next / previous, +0x12 / +0x14 the
// object's list next / previous, +0x16 the "being removed" list's next. A
// group: +0 / +2 its first and last link. An object: +8 flags (bits 17..
// the delete lock count, bit 16 delete requested), +9 a walk count, +0xc /
// +0xe its first and last link, +0x10 the delete list's next, +0x14 a
// destructor, +0x18 a pre-delete callback. The links being removed hang on
// *0x26d984 (by +0x16), the objects to delete on *0x26d988 (by +0x10).
//
// The lock: a per-thread recursive one (owner *0x1e130c, depth *0x1e1310,
// semaphore *0x26d98c). The original: GetThreadId; owner == tid: depth up;
// else WaitSema, owner = tid, depth 1; ... depth down; 0: owner -1 and
// SignalSema (a tail jump or a jal). Here (acquire()): GetThreadId (a
// syscall stub, no side effect); owner == tid with depth != 0: nested; else
// PollSema: it succeeds exactly when WaitSema would not block (the same
// count change, no thread switch; Sync.cpp's waitSemaphore / pollSemaphore),
// then owner = tid, depth 1; PollSema failing (WaitSema would block) or a
// nested entry at depth 0: the recompiled code from the start, nothing
// written (v0 / a0 as GetThreadId / PollSema left them: the recompiled
// code sets both before reading them; a0 is put back anyway). The depth
// comes back to 0 at the end only when the lock was taken here: then the
// original's SignalSema goes through the table (decomp_sema.cpp's fast
// path) with the ra / sp the original's call or tail jump has.
//
// - isysDeleteGroupAllObj (0x1c6db8, a0 the group): every link of the group
//   marked as being removed (+0xc = 1, pushed on *0x26d984 by +0x16 unless
//   it was marked already) and its group cleared (+4 = 0); the group's
//   first / last emptied. Called by iosDlSwap with the lock held (nested)
//   and by ReleaseCharacterClassMotSyncAnim (taken here).
// - isysGroupForFirstForce (0x1c7260, a0 the group): the group's first link
//   whatever its state (v0, 0 none), its object's walk count (+9) up.
// - isysObjDeleteUnLock (0x1c76a8, a0 an object): the delete lock count
//   (+8 bits 17..) down.
// - isysObjWaitSema (0x1c7a18) / isysObjSignalSema (0x1c7a90): the lock
//   alone. Native only on their host-neutral paths: WaitSema nested (depth
//   up), SignalSema with the depth staying above 0. Taking (PollSema) or
//   releasing to 0 (SignalSema) alone would change the kernel's count, host
//   state a difftest leg does not put back (two legs would take or signal
//   twice): those go to the recompiled code before anything is written.
// - kernel_sub_001BF590 (0x1bf590): the display list's current buffer reset
//   (0x2132d8: +0 / +4 two words by buffer, +8 / +0xc the packet buffers,
//   +0x10 the current word, +0x14 the packet count, +0x18 the buffer index):
//   each 0x30-byte packet's tags (0x10000000 at +0, a next tag pointing at
//   the following packet at +0x10, the self pointers at +0x20 / +0x24, the
//   rest 0), the end packet (0x1e10c0 << 32 | 2 tags, 0x50000002 words), the
//   GIF tag at 0x1e10c0 (zeroed, then NLOOP 1, EOP, NREG 1 ... A+D 0x61).
// - isysDeleteObjManager (0x1c5e60, the main loop's): the deferred deletes.
//   The links being removed whose object's walk count is 0 are taken out
//   (kernel_sub_001C6408), the others kept on the list (reversed). The
//   objects on the delete list with no delete lock / request bits (+8 &
//   0xfffeff00 == 0) lose all their links (kernel_sub_001C6408 each) and go
//   to a local list; the others stay (reversed). Each deleted object: its
//   destructor (+0x14, a0 the object) when set, then iosFree(+0).
// - isysRequestDeleteObj (0x1c7548, a0 an object): its pre-delete callback
//   (+0x18, then cleared) without the lock, then under the lock: +4 = the
//   caller's ra, +8 |= 0x10000, pushed on the delete list.
// - isysGetGroupSrhNext (0x1c79e0, a0 a link): the next link (+0xe) or 0.
// - isysLetterObjDiscardLetterAll (0x1c82d8, a0 an object): by its type
//   (+8, 6..22) through the jump tables at 0x1e4060 (letters or not) and
//   0x1e40b0 (the object or 0 as the base): its post (+0x1c) opened, every
//   letter discarded, closed (a tail jump). v0 0 for a type without letters.
//   open_letter_post (0x1c8900): +0xc = 0, +0x10 = +4 (the first letter);
//   discard_all_letter_in_post (0x1c8888): each letter (by +4)
//   decrement_letter_refcount'ed and iosFree'd, the post's words cleared;
//   close_letter_post (0x1c8938): +0x10, +0xc = 0.
// - texTransRequest (0x10aad48, a0 the texture, a1 a word, a2 0 / 1 the
//   slot): a texture upload request into the display list. iosDlLockObj
//   (the texture), two packets (iosAllocPacketPtrPath1(0x20)): the request
//   (+0 / +4 0, +0x10 / +0x14 the texture in slot a2, the other 0, +8 a1,
//   +0x18 the tag packet, +0x1c 0) and its tag packet (0x60000000 tags at
//   +0 / +0x10, the rest 0) chained (iosDlChainTailCurrent); the texture's
//   owner count (+0x20 -> +0xc -> +0x24) up; the request appended to the
//   layer's (iosDlGetLayer) list: heads at *0x1215e44, tails at *0x1215e48
//   (+0x1c the next). Hooked by sotc_native.cpp (texTransRequestHook,
//   through sotc_decomp::behindHook). The jal sites are read from the code
//   words as the generated code does (jalTarget: the module's links to the
//   boot ELF).
//
// Left recompiled: iosDlSwap (0x1bff50): its first call is WaitSema on the
// display list's own semaphore (*0x2132d0) and the isys lock's after it
// (isysObjWaitSema) with the buffer index flipped in between: a block in
// the second after the first was taken cannot go back; it runs once a
// frame. Its loop's callees are the natives here (isysGroupForNextForce
// already is: decomp_group.cpp).
//
// The recompiled code's statements in its order; every store (the frames'
// register saves included); the s-registers in the context at every guest
// call as the original holds them (round 47's rule), back from the frame as
// the epilogues reload them; v0 as the code leaves it; LO / HI as the last
// index mult leaves them.
// Not as the original: the scratch GPRs (only the calls' arguments, v0 and
// what a callee reads are set), the loops' EE checkpoints, and two read-only
// walks isysDeleteObjManager makes after its passes (to the end of the kept
// links and the kept objects: no store, no register it leaves that is read).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "ps2_syscalls.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_isys48
{
    using sotc_decomp::callGuest;
    using sotc_decomp::callNative;
    using sotc_decomp::returnTo;
    using sotc_decomp::SFrame;

    constexpr uint32_t kDeleteGroupAllObj = 0x001C6DB8u, kForFirstForce = 0x001C7260u, kDeleteUnLock = 0x001C76A8u, kObjWaitSema = 0x001C7A18u,
                       kObjSignalSema = 0x001C7A90u, kDlReset = 0x001BF590u, kDeleteObjManager = 0x001C5E60u, kRequestDeleteObj = 0x001C7548u,
                       kSrhNext = 0x001C79E0u, kDiscardLetterAll = 0x001C82D8u, kOpenPost = 0x001C8900u, kDiscardPost = 0x001C8888u,
                       kClosePost = 0x001C8938u, kTexTransRequest = 0x010AAD48u;

    // Callees.
    constexpr uint32_t kSignalSema = 0x00107000u, kRemove = 0x001C6408u /* kernel_sub_001C6408 */, kIosFree = 0x001B0328u,
                       kDecrementLetter = 0x001C8510u /* decrement_letter_refcount */;

    constexpr uint32_t kBase = 0x0021B900u;                                         // the link table
    constexpr uint32_t kOwner = 0x001E130Cu, kDepth = 0x001E1310u, kSemaWord = 0x0026D98Cu; // the lock
    constexpr uint32_t kRemoving = 0x0026D984u, kDeleting = 0x0026D988u;           // the deferred lists

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // A link's address from its index as the code makes it ((i * 3) << 3 + base; 0: none).
    inline uint32_t linkAt(uint32_t index) { return index == 0u ? 0u : ((index << 1) + index) * 8u + kBase; }
    // A link's index as the code makes it: sra(link - base, 3) * 0xaaaaaaab
    // (mult: LO / HI set as the generated code sets them), the low 32 bits.
    inline uint32_t indexProduct(R5900Context *ctx, uint32_t link)
    {
        const int32_t d = static_cast<int32_t>(link - kBase) >> 3;
        const int64_t r = static_cast<int64_t>(d) * static_cast<int64_t>(static_cast<int32_t>(0xAAAAAAABu));
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r >> 32)));
        return static_cast<uint32_t>(r);
    }
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    // The lock's entry (see the header): the thread id in `tid` (s0 / s1 of the originals).
    enum class Lock
    {
        Nested, // the caller's already: depth up
        Taken,  // free: the semaphore taken (WaitSema would not have blocked), owner = tid, depth 1
        No      // WaitSema would block (or a nested entry at depth 0): nothing written
    };
    Lock acquire(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t &tid)
    {
        ps2_syscalls::GetThreadId(rdram, ctx, runtime);
        tid = GPR_U64(ctx, 2);
        if (tid == sx(READ32(kOwner)))
        {
            const uint32_t depth = READ32(kDepth);
            if (depth == 0u)
                return Lock::No; // the end would signal a semaphore nobody took here
            WRITE32(kDepth, depth + 1u);
            return Lock::Nested;
        }
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kSemaWord)));
        ps2_syscalls::PollSema(rdram, ctx, runtime);
        if (static_cast<int32_t>(GPR_U32(ctx, 2)) < 0)
            return Lock::No;
        WRITE32(kOwner, static_cast<uint32_t>(tid));
        WRITE32(kDepth, 1u);
        return Lock::Taken;
    }

    // The end shared by the functions whose release is a tail jump: depth
    // down; not 0: the saved registers back, v0 the depth, jr ra; 0: owner -1,
    // a0 the semaphore, the registers back, j SignalSema (ra from the frame,
    // the caller's sp).
    void releaseTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const SFrame &f, uint32_t raAt, uint32_t entrySp)
    {
        const uint32_t depth = READ32(kDepth) - 1u;
        WRITE32(kDepth, depth);
        if (depth != 0u)
        {
            f.restore();
            SET_GPR_U64(ctx, 31, READ64(f.sp + raAt));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(depth));
            SET_GPR_U32(ctx, 29, entrySp);
            returnTo(ctx);
            return;
        }
        SET_GPR_S32(ctx, 3, -1);
        WRITE32(kOwner, 0xFFFFFFFFu);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kSemaWord)));
        f.restore();
        const uint64_t ra = READ64(f.sp + raAt);
        SET_GPR_U64(ctx, 31, ra);
        callGuest(rdram, ctx, runtime, kSignalSema, static_cast<uint32_t>(ra), entrySp);
    }

    // ---- isysDeleteGroupAllObj ----------------------------------------------
    void deleteGroupAllObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDeleteGroupAllObj, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint32_t group = static_cast<uint32_t>(a0);
        uint64_t tid;
        if (acquire(rdram, ctx, runtime, tid) == Lock::No)
        {
            SET_GPR_U64(ctx, 4, a0);
            return fallback(kDeleteGroupAllObj, rdram, ctx, runtime);
        }
        // The frame (sp - 0x30: s0..s3, ra at 0x20). s0 tid, s1 &owner, s2
        // the group, s3 0x1e0000 in the original: no call before the
        // epilogue reloads them, so the context keeps the caller's.
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, 0x18, -1, -1, -1, -1, -1}};
        f.save();
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31));

        for (uint32_t link = linkAt(READ16(group)); link != 0u;)
        {
            if (READ8(link + 0xCu) != 1u)
            {
                // Marked, pushed on the being-removed list (+0x16 the old head's index).
                WRITE8(link + 0xCu, 1u);
                const uint32_t head = READ32(kRemoving);
                WRITE16(link + 0x16u, static_cast<uint16_t>(head == 0u ? 0u : indexProduct(ctx, head)));
                WRITE32(kRemoving, link);
            }
            const uint32_t next = READ16(link + 0xEu);
            WRITE32(link + 4u, 0u); // out of its group
            link = linkAt(next);
        }
        WRITE16(group, 0u);
        WRITE16(group + 2u, 0u);
        releaseTail(rdram, ctx, runtime, f, 0x20u, entrySp);
    }

    // ---- isysGroupForFirstForce ---------------------------------------------
    void groupForFirstForce(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kForFirstForce, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4);
        uint64_t tid;
        if (acquire(rdram, ctx, runtime, tid) == Lock::No)
        {
            SET_GPR_U64(ctx, 4, a0);
            return fallback(kForFirstForce, rdram, ctx, runtime);
        }
        // The frame (sp - 0x30: s0..s3, ra at 0x20).
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, 0x18, -1, -1, -1, -1, -1}};
        f.save();
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31));
        const uint32_t link = linkAt(READ16(static_cast<uint32_t>(a0)));
        if (link != 0u)
        {
            const uint32_t object = READ32(link);
            WRITE8(object + 9u, static_cast<uint8_t>(READ8(object + 9u) + 1u));
        }
        const uint32_t depth = READ32(kDepth) - 1u;
        WRITE32(kDepth, depth);
        if (depth == 0u)
        {
            // jal SignalSema at 0x1c7334 (owner -1 in its delay slot), s0 the result.
            SET_GPR_S32(ctx, 3, -1);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kSemaWord)));
            WRITE32(kOwner, 0xFFFFFFFFu);
            f.setS32(0, link);
            f.setS32(1, kOwner);
            f.set(2, a0);
            f.setS32(3, 0x001E0000u);
            callGuest(rdram, ctx, runtime, kSignalSema, 0x001C733Cu, sp);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(link));
        f.restore();
        SET_GPR_U64(ctx, 31, READ64(sp + 0x20u));
        SET_GPR_U32(ctx, 29, entrySp);
        returnTo(ctx);
    }

    // ---- isysObjDeleteUnLock ------------------------------------------------
    void objDeleteUnLock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDeleteUnLock, rdram, ctx, runtime);
        const uint32_t object = GPR_U32(ctx, 4);
        const uint32_t w = READ32(object + 8u);
        const uint32_t count = ((w >> 17) - 1u) << 17;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(count));
        SET_GPR_S32(ctx, 3, 0x0001FFFF);
        const uint32_t a1 = (w & 0x0001FFFFu) | count;
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(a1));
        WRITE32(object + 8u, a1);
        returnTo(ctx);
    }

    // ---- isysObjWaitSema (nested only) ---------------------------------------
    void objWaitSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kObjWaitSema, rdram, ctx, runtime);
        ps2_syscalls::GetThreadId(rdram, ctx, runtime);
        const uint64_t tid = GPR_U64(ctx, 2);
        if (tid != sx(READ32(kOwner)))
            return fallback(kObjWaitSema, rdram, ctx, runtime); // WaitSema: the kernel's count (see the header)
        // The frame (sp - 0x20: s0, s1, ra at 0x10); s0 / s1 are reloaded with no call between.
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
        WRITE64(sp, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 31));
        const uint32_t depth = READ32(kDepth) + 1u;
        WRITE32(kDepth, depth);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(depth));
        returnTo(ctx);
    }

    // ---- isysObjSignalSema (depth staying above 0 only) ---------------------
    void objSignalSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kObjSignalSema, rdram, ctx, runtime);
        const uint32_t depth = READ32(kDepth) - 1u;
        if (depth == 0u)
            return fallback(kObjSignalSema, rdram, ctx, runtime); // SignalSema: the kernel's count (see the header)
        WRITE64(GPR_U32(ctx, 29) - 0x10u, GPR_U64(ctx, 31)); // the frame's ra
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(kDepth));
        WRITE32(kDepth, depth);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(depth));
        returnTo(ctx);
    }

    // ---- kernel_sub_001BF590: the display list buffer reset ------------------
    void dlReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDlReset, rdram, ctx, runtime);
        constexpr uint32_t kDl = 0x002132D8u, kGifTag = 0x001E10C0u;
        // The current packet buffer (+8 + 4 x the index at +0x18), read again
        // where the code reads it again after a store (the reads it makes
        // back to back, with no store between, are one here).
        auto buffer = [&]() -> uint32_t { return READ32(kDl + 8u + (READ32(kDl + 0x18u) << 2)); };
        // The packet count (+0x14).
        auto count = [&]() -> uint32_t { return READ32(kDl + 0x14u); };

        if (static_cast<int32_t>(count()) > 0)
        {
            int32_t i = 0;
            uint32_t off = 0;
            do
            {
                ++i;
                const uint32_t p = buffer() + off;
                WRITE32(p + 0x1Cu, 0u);
                WRITE32(p + 0x18u, 0u);
                WRITE32(p + 0xCu, 0u);
                WRITE32(p + 8u, 0u);
                WRITE64(buffer() + off, 0x10000000ull);
                {
                    const uint32_t q = buffer() + off;
                    // dsll32 of the next packet's address, or 0x20000000.
                    WRITE64(q + 0x10u, (static_cast<uint64_t>(q + 0x30u) << 32) | 0x20000000ull);
                }
                const uint32_t r = buffer() + off;
                off += 0x30u;
                WRITE32(r + 0x24u, r);
                WRITE32(r + 0x20u, r);
            } while (i < static_cast<int32_t>(count()));
        }
        const uint64_t endTag = (static_cast<uint64_t>(kGifTag) << 32) | 2u; // t2
        constexpr uint32_t kEndWord = 0x50000002u;                            // t4
        WRITE32(kDl + 0x10u, READ32(kDl + (READ32(kDl + 0x18u) << 2)));
        // The packet after the last one (the buffer + count x 0x30).
        auto end = [&]() -> uint32_t { return buffer() + ((count() << 1) + count()) * 16u; };
        {
            const uint32_t e = end();
            WRITE64(e + 0x10u, endTag);
            WRITE64(e, endTag);
        }
        {
            const uint32_t e = end();
            WRITE32(e + 0x18u, 0u);
            WRITE32(e + 8u, 0u);
        }
        {
            const uint32_t e = end();
            WRITE32(e + 0x1Cu, kEndWord);
            WRITE32(e + 0xCu, kEndWord);
        }
        // The GIF tag: zeroed (por / sq), then its fields.
        SET_GPR_VEC(ctx, 2, PS2_POR(GPR_VEC(ctx, 0), GPR_VEC(ctx, 0)));
        WRITE128(kGifTag, GPR_VEC(ctx, 2));
        WRITE8(kGifTag + 1u, static_cast<uint8_t>(READ8(kGifTag + 1u) | 0x80u));
        WRITE8(kGifTag + 7u, static_cast<uint8_t>((READ8(kGifTag + 7u) & 0x0Fu) | 0x10u));
        WRITE8(kGifTag + 8u, static_cast<uint8_t>((READ8(kGifTag + 8u) & 0xF0u) | 0x0Eu));
        WRITE16(kGifTag, static_cast<uint16_t>((READ16(kGifTag) & 0x8000u) | 1u));
        WRITE64(kGifTag + 0x10u, 0ull);
        WRITE64(kGifTag + 0x18u, 0x61ull);
        const uint32_t n = count();
        const uint32_t e = buffer() + ((n << 1) + n) * 16u;
        WRITE32(e + 0x24u, e);
        WRITE32(e + 0x20u, e);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(((n << 1) + n) << 4));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(e));
        returnTo(ctx);
    }

    // ---- isysDeleteObjManager -----------------------------------------------
    void deleteObjManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDeleteObjManager, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4);
        uint64_t tid;
        if (acquire(rdram, ctx, runtime, tid) == Lock::No)
        {
            SET_GPR_U64(ctx, 4, a0);
            return fallback(kDeleteObjManager, rdram, ctx, runtime);
        }
        // The frame (sp - 0x50: s0..s7 at 0..0x38, fp 0x40, ra 0x48).
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x50u;
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40}};
        f.save();
        WRITE64(sp + 0x48u, GPR_U64(ctx, 31));
        f.set(0, tid);
        f.setS32(1, kOwner);

        // 1. The links being removed: out (kernel_sub_001C6408) when their
        //    object is not walked (+9 == 0), else kept (s1, reversed by +0x16).
        f.setS32(4, 0x00270000u);
        uint32_t link = READ32(kRemoving);
        uint32_t kept = 0u;
        f.set(1, 0u);
        if (link != 0u)
        {
            f.setS32(2, kBase);
            f.set(3, 0xFFFFFFFFAAAAAAABull);
            do
            {
                const uint32_t next = linkAt(READ16(link + 0x16u));
                f.setS32(0, next);
                const uint32_t object = READ32(link);
                if (READ8(object + 9u) == 0u)
                {
                    SET_GPR_S32(ctx, 4, static_cast<int32_t>(link));
                    callGuest(rdram, ctx, runtime, kRemove, 0x001C5F44u, sp);
                }
                else
                {
                    WRITE16(link + 0x16u, static_cast<uint16_t>(kept == 0u ? 0u : indexProduct(ctx, kept)));
                    kept = link;
                    f.setS32(1, kept);
                }
                link = next;
            } while (link != 0u);
        }
        WRITE32(kRemoving, kept);

        // 2. The objects to delete: no delete lock / request (+8 & 0xfffeff00
        //    == 0): every link out, onto the local list (s5, by +0x10), +0xc
        //    0; else kept (s4, reversed by +0x10).
        uint32_t keptObjects = 0u, deleted = 0u;
        f.set(4, 0u);
        uint32_t object = READ32(kDeleting);
        f.setS32(1, object);
        f.set(5, 0u);
        if (object != 0u)
        {
            f.set(6, 0xFFFFFFFFFFFEFF00ull);
            f.setS32(SFrame::kFp, 0x00220000u);
            f.setS32(7, kBase);
            do
            {
                const uint32_t flags = READ32(object + 8u);
                const uint32_t nextObject = READ32(object + 0x10u);
                f.setS32(3, nextObject);
                if ((flags & 0xFFFEFF00u) != 0u)
                {
                    WRITE32(object + 0x10u, keptObjects);
                    keptObjects = object;
                    f.setS32(4, object);
                }
                else
                {
                    uint32_t l = linkAt(READ16(object + 0xCu));
                    f.setS32(0, l);
                    if (l != 0u)
                    {
                        f.setS32(2, kBase);
                        do
                        {
                            SET_GPR_S32(ctx, 4, static_cast<int32_t>(l));
                            callGuest(rdram, ctx, runtime, kRemove, 0x001C6040u, sp);
                            l = linkAt(READ16(l + 0x12u));
                            f.setS32(0, l);
                        } while (l != 0u);
                    }
                    WRITE32(object + 0x10u, deleted);
                    deleted = object;
                    f.setS32(5, object);
                    WRITE16(object + 0xCu, 0u);
                }
                object = nextObject;
                f.setS32(1, object);
            } while (object != 0u);
        }
        WRITE32(kDeleting, keptObjects);

        // 3. Each deleted object: its destructor (+0x14) when set, then iosFree(+0).
        f.setS32(1, deleted);
        for (uint32_t o = deleted; o != 0u;)
        {
            const uint32_t destructor = READ32(o + 0x14u);
            const uint32_t next = READ32(o + 0x10u);
            f.setS32(0, next);
            if (destructor != 0u)
            {
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(destructor));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(o));
                callGuest(rdram, ctx, runtime, destructor, 0x001C60B8u, sp);
            }
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(o)));
            f.setS32(1, next);
            callGuest(rdram, ctx, runtime, kIosFree, 0x001C60C4u, sp);
            o = next;
        }
        // (4. the original's walk to the kept objects' end: reads only.)
        releaseTail(rdram, ctx, runtime, f, 0x48u, entrySp);
    }

    // ---- isysRequestDeleteObj -----------------------------------------------
    void requestDeleteObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kRequestDeleteObj, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4), ra = GPR_U64(ctx, 31);
        const uint32_t object = static_cast<uint32_t>(a0);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        // The frame (sp - 0x30: s0..s4, ra at 0x28); s0 the object, s3 the caller's ra.
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, 0x18, 0x20, -1, -1, -1, -1}};
        const uint32_t callback = READ32(object + 0x18u);
        uint64_t tid;
        if (callback == 0u)
        {
            if (acquire(rdram, ctx, runtime, tid) == Lock::No)
            {
                SET_GPR_U64(ctx, 4, a0);
                return fallback(kRequestDeleteObj, rdram, ctx, runtime);
            }
            f.save();
            WRITE64(sp + 0x28u, ra);
            f.set(0, a0);
            f.set(3, ra);
        }
        else
        {
            // The pre-delete callback (jalr, a0 the object), before the lock.
            f.save();
            WRITE64(sp + 0x28u, ra);
            f.set(0, a0);
            f.set(3, ra);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(callback));
            callGuest(rdram, ctx, runtime, callback, 0x001C757Cu, sp);
            // At its return label 0x1c757c. The lock would block: the
            // recompiled function from that label (its entry switches on
            // pc), with the frame and the s-registers it has there (v0 / a0
            // are set again before they are read).
            if (acquire(rdram, ctx, runtime, tid) == Lock::No)
            {
                ctx->pc = 0x001C757Cu;
                SET_GPR_U32(ctx, 29, sp);
                return sotc_decomp::recompiled(kRequestDeleteObj)(rdram, ctx, runtime);
            }
            WRITE32(object + 0x18u, 0u);
        }
        f.setS32(4, 0x001E0000u);
        f.setS32(2, kOwner);
        f.set(1, tid);
        const uint32_t flags = READ32(object + 8u) | 0x00010000u; // delete requested
        WRITE32(object + 4u, static_cast<uint32_t>(ra));
        WRITE32(object + 8u, flags);
        WRITE32(object + 0x10u, READ32(kDeleting));
        WRITE32(kDeleting, object);
        releaseTail(rdram, ctx, runtime, f, 0x28u, entrySp);
    }

    // ---- isysGetGroupSrhNext ------------------------------------------------
    void groupSrhNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kSrhNext, rdram, ctx, runtime);
        const uint32_t index = READ16(GPR_U32(ctx, 4) + 0xEu);
        if (index == 0u)
            SET_GPR_U64(ctx, 2, 0u);
        else
        {
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(kBase));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(linkAt(index)));
        }
        returnTo(ctx);
    }

    // ---- the letter posts -----------------------------------------------------
    void openLetterPost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kOpenPost, rdram, ctx, runtime);
        const uint32_t post = GPR_U32(ctx, 4);
        const uint32_t first = READ32(post + 4u);
        WRITE32(post + 0xCu, 0u);
        WRITE32(post + 0x10u, first);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(first));
        returnTo(ctx);
    }

    void closeLetterPost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kClosePost, rdram, ctx, runtime);
        const uint32_t post = GPR_U32(ctx, 4);
        WRITE32(post + 0x10u, 0u);
        WRITE32(post + 0xCu, 0u);
        returnTo(ctx);
    }

    void discardAllLetterInPost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDiscardPost, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint32_t post = static_cast<uint32_t>(a0);
        // The frame (sp - 0x20: s0..s2, ra at 0x18); s2 the post, s1 the letter, s0 the next.
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x20u;
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, -1, -1, -1, -1, -1, -1}};
        f.save();
        WRITE64(sp + 0x18u, GPR_U64(ctx, 31));
        f.set(2, a0);
        uint32_t letter = READ32(post + 4u);
        f.setS32(1, letter);
        if (letter != 0u)
        {
            uint32_t next = READ32(letter + 4u);
            f.setS32(0, next);
            for (;;)
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(letter));
                callGuest(rdram, ctx, runtime, kDecrementLetter, 0x001C88B8u, sp);
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(letter));
                letter = next;
                f.setS32(1, letter);
                callGuest(rdram, ctx, runtime, kIosFree, 0x001C88C4u, sp);
                if (letter == 0u)
                    break;
                next = READ32(letter + 4u);
                f.setS32(0, next);
            }
        }
        WRITE32(post + 0x10u, 0u);
        WRITE32(post, 0u);
        WRITE32(post + 4u, 0u);
        WRITE32(post + 8u, 0u);
        WRITE32(post + 0xCu, 0u);
        f.restore();
        SET_GPR_U64(ctx, 31, READ64(sp + 0x18u));
        SET_GPR_U32(ctx, 29, entrySp);
        returnTo(ctx);
    }

    // ---- isysLetterObjDiscardLetterAll --------------------------------------
    void discardLetterAll(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kDiscardLetterAll, rdram, ctx, runtime);
        constexpr uint32_t kHasLetters = 0x001E4060u, kPostBase = 0x001E40B0u; // the jump tables (types 6..22)
        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint32_t object = static_cast<uint32_t>(a0);
        const uint32_t type = READ8(object + 8u);
        const uint32_t k = type - 6u; // sltiu 0x11 on the sign-extended value: unsigned
        // Both jump tables decided first (reads only): an unknown target -> the recompiled code.
        uint64_t v0 = 0u, base = a0;
        if (k < 0x11u)
        {
            const uint32_t t1 = READ32(kHasLetters + (k << 2));
            if (t1 == 0x001C8310u)
                v0 = a0;
            else if (t1 != 0x001C8318u)
                return fallback(kDiscardLetterAll, rdram, ctx, runtime);
            if (v0 != 0u)
            {
                const uint32_t t2 = READ32(kPostBase + (k << 2));
                if (t2 == 0x001C8348u)
                    base = 0u;
                else if (t2 != 0x001C834Cu)
                    return fallback(kDiscardLetterAll, rdram, ctx, runtime);
            }
        }
        // The frame (sp - 0x10: s0, ra at 8).
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x10u;
        const uint64_t ra = GPR_U64(ctx, 31);
        WRITE64(sp, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, ra);
        SET_GPR_U64(ctx, 5, type);
        if (v0 == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
            return;
        }
        // s0 the post (+0x1c of the object, or of 0).
        const uint32_t post = READ32(static_cast<uint32_t>(base) + 0x1Cu);
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(post));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(post));
        callNative(rdram, ctx, runtime, kOpenPost, &openLetterPost, 0x001C8358u, sp);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(post));
        callNative(rdram, ctx, runtime, kDiscardPost, &discardAllLetterInPost, 0x001C8360u, sp);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(post));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        SET_GPR_U64(ctx, 16, READ64(sp));
        // j close_letter_post (the frame popped).
        callNative(rdram, ctx, runtime, kClosePost, &closeLetterPost, static_cast<uint32_t>(ra), entrySp);
    }

    // ---- texTransRequest ----------------------------------------------------
    void texTransRequest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::isysOn())
            return fallback(kTexTransRequest, rdram, ctx, runtime);
        const uint64_t texture = GPR_U64(ctx, 4), word = GPR_U64(ctx, 5), slot = GPR_U64(ctx, 6);
        // The frame (sp - 0x30: s0..s3, ra at 0x20); s0 the slot, s1 the texture, s2 the word.
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        const SFrame f{rdram, ctx, sp, {0, 8, 0x10, 0x18, -1, -1, -1, -1, -1}};
        f.save();
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31));
        f.set(0, slot);
        f.set(1, texture);
        f.set(2, word);
        const uint32_t tex = static_cast<uint32_t>(texture);

        // iosDlLockObj(texture).
        callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010AAD68u), 0x010AAD70u, sp);
        // The request packet: iosAllocPacketPtrPath1(0x20).
        SET_GPR_S32(ctx, 4, 0x20);
        callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010AAD70u), 0x010AAD78u, sp);
        const uint64_t req64 = GPR_U64(ctx, 2);
        const uint32_t req = static_cast<uint32_t>(req64);
        f.set(3, req64);
        const uint32_t other = req + (slot != 0u ? 0x10u : 0x14u); // movn: the slot not used
        const uint32_t mine = (static_cast<uint32_t>(slot) << 2) + req;
        f.setS32(0, mine);
        WRITE32(req, 0u);
        WRITE32(req + 4u, 0u);
        WRITE32(mine + 0x10u, tex);
        WRITE32(other, 0u);
        // The tag packet: iosAllocPacketPtrPath1(0x20).
        SET_GPR_S32(ctx, 4, 0x20);
        callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010AADA8u), 0x010AADB0u, sp);
        WRITE32(req + 0x18u, GPR_U32(ctx, 2));
        {
            const uint32_t tag = READ32(req + 0x18u);
            WRITE64(tag, 0x60000000ull);
            WRITE64(tag + 0x10u, 0x60000000ull);
        }
        {
            const uint32_t tag = READ32(req + 0x18u);
            WRITE32(tag + 8u, 0u);
            WRITE32(tag + 0xCu, 0u);
        }
        {
            const uint32_t tag = READ32(req + 0x18u);
            WRITE32(tag + 0x18u, 0u);
            WRITE32(tag + 0x1Cu, 0u);
        }
        WRITE32(req + 8u, static_cast<uint32_t>(word));
        // iosDlChainTailCurrent(tag packet).
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(req + 0x18u)));
        callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010AADE4u), 0x010AADECu, sp);
        // The texture's owner count up (+0x20 -> +0xc -> +0x24), then iosDlGetLayer.
        {
            const uint32_t owner = READ32(READ32(tex + 0x20u) + 0xCu);
            const uint32_t n = READ32(owner + 0x24u) + 1u;
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(owner));
            WRITE32(owner + 0x24u, n);
        }
        callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010AADFCu), 0x010AAE04u, sp);
        // The request appended to the layer's list (heads *0x1215e44, tails *0x1215e48, +0x1c the next).
        constexpr uint32_t kHeads = 0x01215E44u, kTails = 0x01215E48u;
        WRITE32(req + 0x1Cu, 0u);
        const uint32_t layer4 = GPR_U32(ctx, 2) << 2;
        {
            const uint32_t head = layer4 + READ32(kHeads);
            if (READ32(head) == 0u)
                WRITE32(head, req);
        }
        uint32_t tail = layer4 + READ32(kTails);
        if (READ32(tail) != 0u)
        {
            WRITE32(READ32(tail) + 0x1Cu, req);
            tail = layer4 + READ32(kTails);
        }
        WRITE32(tail, req);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(tail));
        f.restore();
        SET_GPR_U64(ctx, 31, READ64(sp + 0x20u));
        SET_GPR_U32(ctx, 29, entrySp);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x001C6DB8u, isysDeleteGroupAllObj, decomp_isys48::deleteGroupAllObj);
SOTC_DECOMP_FUNCTION(0x001C7260u, isysGroupForFirstForce, decomp_isys48::groupForFirstForce);
SOTC_DECOMP_FUNCTION(0x001C76A8u, isysObjDeleteUnLock, decomp_isys48::objDeleteUnLock);
SOTC_DECOMP_UNCHECKED(0x001C7A18u, isysObjWaitSema, decomp_isys48::objWaitSema);
SOTC_DECOMP_UNCHECKED(0x001C7A90u, isysObjSignalSema, decomp_isys48::objSignalSema);
SOTC_DECOMP_FUNCTION(0x001BF590u, kernel_sub_001BF590, decomp_isys48::dlReset);
SOTC_DECOMP_FUNCTION(0x001C5E60u, isysDeleteObjManager, decomp_isys48::deleteObjManager);
SOTC_DECOMP_FUNCTION(0x001C7548u, isysRequestDeleteObj, decomp_isys48::requestDeleteObj);
SOTC_DECOMP_FUNCTION(0x001C79E0u, isysGetGroupSrhNext, decomp_isys48::groupSrhNext);
SOTC_DECOMP_FUNCTION(0x001C8900u, open_letter_post, decomp_isys48::openLetterPost);
SOTC_DECOMP_FUNCTION(0x001C8888u, discard_all_letter_in_post, decomp_isys48::discardAllLetterInPost);
SOTC_DECOMP_FUNCTION(0x001C8938u, close_letter_post, decomp_isys48::closeLetterPost);
SOTC_DECOMP_FUNCTION(0x001C82D8u, isysLetterObjDiscardLetterAll, decomp_isys48::discardLetterAll);
SOTC_DECOMP_FUNCTION(0x010AAD48u, texTransRequest, decomp_isys48::texTransRequest);
